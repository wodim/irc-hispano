/* Copyright (C) 2003, 2004 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#define RCSID "$Id: irc_userbase.c,v 1.5 2004/05/02 12:39:10 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_userbase.h"
#include "irc_membership.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dbprim/dbprim.h>
#include <peak/peak.h>
#include <arpa/inet.h>

#include "cfgloader.h"
#include "debug.h"
#include "evreg.h"
#include "hash_fun.h"
#include "irc_network.h"
#include "irc_string.h"
#include "irc_yxx.h"
#include "pxyservd_log.h"
#include "scan.h"

struct addrcount
  {
  struct in_addr addr;
  int count;
  };

/* User hash table */
static hash_table_t sUHtab = HASH_TABLE_INIT(HASH_FLAG_AUTOGROW|
                                             HASH_FLAG_AUTOSHRINK,
                                             strhash, ircd_strcomp,
                                             NULL, NULL);
static int sCTotalCount;

peak_mem_pool user_pool;

static void irc_userbase_proxycount_callback(struct Server *sptr,
                                             struct addrcount *ac);
static struct Client * irc_userbase_remove_callback(struct Client *cptr,
                                                    void *ctx);

void
irc_userbase_init()
  {
  user_pool = peak_mem_pool_create(sizeof(struct Client), 0);
  }

void
irc_userbase_finalize()
  {
  peak_release(user_pool);
  }

int
irc_userbase_get_count()
  {
  return ht_count(&sUHtab);
  }

size_t
irc_userbase_get_hash_table_size()
  {
  return ht_size(&sUHtab);
  }

int
irc_userbase_proxycount(const struct in_addr *addr)
  {
  struct addrcount ac;
  ac.addr = *addr;
  ac.count = 0;
  
  irc_network_map_servers((irc_network_map_servers_callback)
                          irc_userbase_proxycount_callback, &ac);
  return ac.count;
  }

static void
irc_userbase_proxycount_callback(struct Server *sptr, struct addrcount *ac)
  {
  struct Client *cptr;

  for (cptr = sptr->cliHead; cptr; cptr = cptr->next)
    if (!(cptr->flags & CLIENT_FLAG_IPV6)
        && (cptr->addr.ip4.s_addr == ac->addr.s_addr))
      {
      ac->count++;
      sptr->proxy_count++;
      }
  }

unsigned int
irc_userbase_count_channels(struct Client *cptr)
  {
  return sh_count(&cptr->mhead);
  }

void
irc_userbase_add(
  const char *nick,       /* Client nickname */
  const char *user,       /* Client username */
  const char *host,       /* Client hostname */
  time_t firsttime,       /* Creation time */
  const char *mode,       /* Client mode; can be NULL */
  unsigned int flags,     /* Flags (reserved for IPv6) */
  ClientAddr addr,        /* IP */
  const char *numnick)    /* Client base64 network numeric */
  {
  struct Client *cptr;
  yxx target_yxx;
  db_key_t dk;
  unsigned long err;
  
  cptr = (struct Client *)peak_mem_pool_new(user_pool);
  
  cptr->scan_next = NULL;
  cptr->scan_prev_p = NULL;
  
  strncpy(cptr->nick, nick, NICKLEN + 1);
  if (cptr->nick[NICKLEN] != '\0')
    {
    log_system("irc_userbase_add: [FATAL] nickname too long (%s); "
               "please consider using the configure option "
               "--with-nicklen=NICKLEN", nick);
    exit(1);
    }
  strncpy(cptr->user, user, USERLEN);
  cptr->user[USERLEN] = '\0';

  strncpy(cptr->host, host, HOSTLEN);
  cptr->host[HOSTLEN] = '\0';
  
  target_yxx = yxx_to_int(numnick);
  if (target_yxx.bogus)
    log_system("irc_userbase_add: bogus numnick: %s", numnick);
  assert(!target_yxx.bogus);
  cptr->nserv = target_yxx.server_n;
  cptr->nnick = target_yxx.client_n;
  cptr->flags = flags;
  if (*user == '~')
    cptr->flags |= CLIENT_FLAG_IDENT;
#ifdef IRC_HISPANO
  if (mode && strchr(mode, 'h'))
#else
  if (mode && strchr(mode, 'o'))
#endif
    cptr->flags |= CLIENT_FLAG_OPER;
  
  cptr->addr = addr;
  
  cptr->firsttime = firsttime;
  cptr->scan_timestamp = 0;
  
  /* Initialize channel sparse matrix list head */
  sh_init(&cptr->mhead, MEMBERSHIP_LOC_CLIENT, cptr);
  
  /* Add client to server */
  irc_network_add_client(target_yxx, cptr);
  
  /* Add client to clients hash table */
  dk_key(&dk) = (void*)cptr->nick;
  dk_len(&dk) = strlen(cptr->nick);
  
  err = he_init(&cptr->hentry, (void*)cptr);
  assert(err == 0);
  err = ht_add(&sUHtab, &cptr->hentry, &dk);
  assert(err == 0);
  
  sCTotalCount++;
  
  Debug((DL_MAX, "irc_userbase_add: added client %s (ID %s) count=%d [%s]",
         nick, numnick, ht_count(&sUHtab), inet_ntoa(cptr->addr.ip4)));
  
  if (gConfig->log.clients != NULL)
    {
    char ipbuf[32];
    
    inet_ntop(flags & CLIENT_FLAG_IPV6 ? AF_INET6 : AF_INET, &cptr->addr,
              ipbuf, sizeof(ipbuf));
    log_write(LOGID_CLIENTS, "%s %s %s", peak_time(), ipbuf, nick);
    }
  
  scan_new_user(cptr);
  }

void
irc_userbase_nick_change(const char *numnick,
                         const char *new_nick)
  {
  struct Client *cptr;
  
  if ((cptr = irc_network_find_client(yxx_to_int(numnick))))
    {
    db_key_t dk;
    
    Debug((DL_MAX, "irc_userbase_nick_change: new nick %s for clientID %s",
           new_nick, numnick));
    
    strncpy(cptr->nick, new_nick, NICKLEN);
    cptr->nick[NICKLEN] = '\0';
    dk.dk_key = (void*)cptr->nick;
    dk.dk_len = strlen(cptr->nick);
    ht_move(&sUHtab, &cptr->hentry, &dk);
    return;
    }
  
  Debug((DL_BASIC, "irc_userbase_nick_change: clientID %s not found",
         numnick));
  }

void
irc_userbase_remove(const char *numnick)
  {
  struct Client *cptr;
  yxx target_yxx = yxx_to_int(numnick);
  
  if ((cptr = irc_network_find_client(target_yxx)))
    {
    Debug((DL_MAX, "irc_userbase_remove: removing client %s (ID %s)",
           cptr->nick, numnick));
    
    evreg_exit(cptr);
    scan_client_remove(cptr);
    irc_membership_remove_client(cptr);
    irc_network_remove_client(target_yxx);
    ht_remove(&sUHtab, &cptr->hentry);
    peak_mem_pool_delete(user_pool, cptr);
    return;
    }
  
  Debug((DL_BASIC, "irc_userbase_remove: clientID %s not found", numnick));
  }

void
irc_userbase_purgeserv(struct Server *sptr)
  {
  Debug((DL_BASIC, "irc_userbase_purgeserv: purging %s's clients",
         sptr->name));
  irc_network_map_clients(sptr, irc_userbase_remove_callback, NULL);
  }

static struct Client *
irc_userbase_remove_callback(struct Client *cptr, void *ctx)
  {
  Debug((DL_MAX, "irc_userbase_remove_callback: removing client %s",
         cptr->nick));
  
  evreg_exit(cptr);
  scan_client_remove(cptr);
  irc_membership_remove_client(cptr);
  ht_remove(&sUHtab, &cptr->hentry);
  peak_mem_pool_delete(user_pool, cptr);
  cptr = NULL;
  return cptr;
  }

struct Client *
irc_userbase_get_by_nick(const char *nick)
  {
  hash_entry_t *he;
  unsigned long err;
  db_key_t dk = DB_KEY_INIT((void*)nick, strlen(nick));
  
  err = ht_find(&sUHtab, &he, &dk);
  if (err == DB_ERR_NOENTRY)
    return NULL;
  assert(err == 0);
  return he_value(he);
  }

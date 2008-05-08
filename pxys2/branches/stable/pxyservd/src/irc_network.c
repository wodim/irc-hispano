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
#define RCSID "$Id: irc_network.c,v 1.6 2004/01/17 17:48:22 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_network.h"
#define PROXYTOP_AUTO_SAVE_DELAY 600

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>

#include <dbprim/dbprim.h>
#include <libbcl/memory.h>

#include "cfgloader.h"
#include "debug.h"
#include "hash_fun.h"
#include "irc_numnicks.h"
#include "irc_string.h"
#include "irc_struct.h"
#include "irc_userbase.h"

static void irc_network_timer_callback(peak_timer ti, void *context);
static unsigned int get_server_record(const char *server_name, int id);
static void set_server_records(const char *server_name,
                               unsigned int maxusers_count,
                               time_t maxusers_time,
                               unsigned int proxy_count,
                               time_t proxy_since);

time_t proxy_count_last_reset_time;

struct Server gMe;

static unsigned int sDownlink;
static struct Server **sSArray;

static peak_timer records_timer;
static hash_table_t sSHtab = HASH_TABLE_INIT(HASH_FLAG_AUTOGROW|
                                             HASH_FLAG_AUTOSHRINK,
                                             strhash, ircd_strcomp,
                                             NULL, NULL);

void
irc_network_init()
  {
  unsigned int max_clients = 16;
  size_t size;
  db_key_t dk;
  unsigned long err;
  
  while (max_clients < gConfig->server.maxclients)
    max_clients <<= 1;
    
  gMe.next = NULL;
  gMe.prev_p = NULL;
  gMe.uplink = NULL;
  
  err = he_init(&gMe.hentry, (void*)&gMe);
  assert(err == 0);
  
  size = max_clients * sizeof(struct Client *);
  gMe.cliT = (struct Client **)memory_alloc(size);
  memset(gMe.cliT, 0, size);
  
  strncpy(gMe.name, gConfig->server.name, HOSTLEN);
  gMe.name[HOSTLEN] = '\0';
  gMe.nserv = gConfig->server.numeric;
  inttobase64(gMe.yy, gMe.nserv, 2);
  gMe.nn_mask = max_clients - 1;
  gMe.clients = 0;
  gMe.maxclients = 0;
  gMe.flags = SERVER_FLAG_SERVICE|SERVER_FLAG_NOSCAN;
  gMe.timestamp = 0;
  gMe.proxy_count = 0; /* Heheh. */
  gMe.proxy_since = 0;
  
  sSArray = (struct Server **)memory_alloc(sizeof(struct Server *)
                                           * NN_MAX_SERVER);
  memset(sSArray, 0, sizeof(struct Server *) * NN_MAX_SERVER);
  
  sSArray[gMe.nserv] = &gMe;
  
  dk.dk_key = (void*)gMe.name;
  dk.dk_len = strlen(gMe.name);
  err = ht_add(&sSHtab, &gMe.hentry, &dk);
  assert(err == 0);
  
  Debug((DL_MAX, "irc_network_init: server initialized (%s) %s",
         gMe.yy, gMe.name));
  
  proxy_count_last_reset_time = get_server_record(NULL, -1);
  if (proxy_count_last_reset_time == 0)
    proxy_count_last_reset_time = peak_time();
  
  records_timer = peak_timer_create(PROXYTOP_AUTO_SAVE_DELAY,
                                    PROXYTOP_AUTO_SAVE_DELAY,
                                    irc_network_timer_callback, NULL);
  peak_task_timer_add(peak_task_self(), records_timer);
  peak_release(records_timer);
  }

static void
irc_network_timer_callback(peak_timer ti, void *context)
  {
  irc_network_set_servers_records();
  }

int
irc_network_get_server_count()
  {
  return ht_count(&sSHtab);
  }

int
irc_network_get_remote_server_count()
  {
  return ht_count(&sSHtab) - 1;
  }

size_t
irc_network_get_hash_table_size()
  {
  return ht_size(&sSHtab);
  }

int
irc_network_get_server_count_flagged(unsigned int flags)
  {
  struct Server *sptr;
  int cnt = 0;
  
  for (sptr = &gMe; sptr; sptr = sptr->next)
    if (sptr->flags & flags)
      cnt++;
  return cnt;
  }

struct Server *
irc_network_get_server(unsigned int nserv)
  {
  if (nserv < NN_MAX_SERVER)
    return sSArray[nserv];
  else
    return NULL;
  }

int
irc_network_server_is_bursting(struct Server *sptr)
  {
  struct Server *up = sptr;
  int bursting = sptr->flags & SERVER_FLAG_BURSTING;
  
  while (!bursting && (up = up->uplink))
    if (up->flags & SERVER_FLAG_BURSTING)
      bursting = 1;
  
  return bursting;
  }

int
irc_network_is_my_downlink(unsigned int nserv)
  {
  struct Server *serv = irc_network_get_server(nserv);
  if (serv == NULL)
    return 0;
  return serv->uplink == &gMe;
  }

void
irc_network_add_server(
  unsigned int nserv_up, /* Uplink numeric */
  const char *name,      /* Server name */
  unsigned int nserv,    /* Server numeric ID */
  unsigned int ncap,     /* Server capacity ID */
  int bursting,          /* Is server bursting? */
  int hub,               /* Is server a hub? */
  int service)           /* Is server a service? */
  {
  struct Server *sptr;
  unsigned int nn_mask = 16;
  size_t size;
  db_key_t dk;
  unsigned long err;
  
  if (nserv_up == gMe.nserv)
    {
    assert(gMe.next == NULL);
    sDownlink = nserv; /* Remember our hub */
    }
  
  while (nn_mask < ncap)
    nn_mask <<= 1;
  nn_mask--;
  
  sptr = (struct Server *)memory_alloc(sizeof(struct Server));
  
  sptr->uplink = irc_network_get_server(nserv_up);
  /* TODO: desync chk */
  err = he_init(&sptr->hentry, (void*)sptr);
  assert(err == 0);
  
  strncpy(sptr->name, name, HOSTLEN);
  sptr->name[HOSTLEN] = '\0';
  
  sptr->cliHead = NULL;
  
  /* Allocate client pointers array */
  size = sizeof(struct Client *) * (nn_mask + 1);
  sptr->cliT = (struct Client **)memory_alloc(size);
  memset(sptr->cliT, 0, size);
  
  sptr->nserv = nserv;
  inttobase64(sptr->yy, nserv, 2);
  sptr->nn_mask = nn_mask;
  sptr->clients = 0;
  sptr->maxclients = get_server_record(sptr->name, 0);
  sptr->maxclients_time = (time_t)get_server_record(sptr->name, 1);
  if (sptr->maxclients_time == 0)
    sptr->maxclients_time = peak_time();
  
  if (bursting)
    sptr->flags = SERVER_FLAG_BURSTING;
  else
    sptr->flags = SERVER_FLAG_NONE;
  if (hub)
    sptr->flags |= SERVER_FLAG_HUB;
  if (service)
    sptr->flags |= SERVER_FLAG_SERVICE;
  sptr->timestamp = peak_time();
  
  sptr->proxy_count = get_server_record(sptr->name, 2);
  sptr->proxy_since = get_server_record(sptr->name, 3);
  if (sptr->proxy_since == 0)
    sptr->proxy_since = peak_time();
  
  sSArray[nserv] = sptr;
  
  dk.dk_key = (void*)sptr->name;
  dk.dk_len = strlen(sptr->name);
  err = ht_add(&sSHtab, &sptr->hentry, &dk);
  assert(err == 0);
  
  /* Linked list stuffs */
  sptr->next = gMe.next;
  if (sptr->next)
    sptr->next->prev_p = &sptr->next;
  gMe.next = sptr;
  sptr->prev_p = &gMe.next;
  
  Debug((DL_MAX, "irc_network_add_server: %s (%d->%d)\n", name, nserv,
         nserv_up));
  }

void
irc_network_ack_end_of_burst(unsigned int nserv)
  {
  struct Server *serv = irc_network_get_server(nserv);
  if (serv != NULL)
    serv->flags &= ~SERVER_FLAG_BURSTING;
  }

void
irc_network_squit_downlink()
  {
  if (ht_count(&sSHtab) > 1)
    irc_network_remove_server(irc_network_get_server(sDownlink));
  }

void
irc_network_remove_server_name(const char *name)
  {
  struct Server *sptr;
  hash_entry_t *hentry_p;
  unsigned long err;
  db_key_t dk = DB_KEY_INIT((void*)name, strlen(name));
  
  err = ht_find(&sSHtab, &hentry_p, &dk);
  assert(err == 0);
  
  if ((sptr = (struct Server *)he_value(hentry_p)))
    {
    irc_network_remove_server(sptr);
    return;
    }
  Debug((DL_BASIC, "irc_network_remove_server_name: server '%s' not found",
         name));
  }

struct Server *
irc_network_remove_server(struct Server *sptr)
  {
  struct Server *s, *next;
  unsigned long err;
  
  /* Delete all users from this server. */
  irc_userbase_purgeserv(sptr);
  sptr->clients = 0;
  
  /* Remove it from the hash table. */
  err = ht_remove(&sSHtab, &sptr->hentry);
  assert(err == 0);
  
  /* Process recursively to purge downlinks. */
  for (s = gMe.next; s;
       s = s->uplink == sptr ? irc_network_remove_server(s) : s->next)
    ;
    
  /* Unlink from server list (after purge, so that ->next will be updated. */
  if (sptr->prev_p)
    *sptr->prev_p = sptr->next;
  if (sptr->next)
    sptr->next->prev_p = sptr->prev_p;
  
  sSArray[sptr->nserv] = NULL;
  
  /* Save server's records (stats purpose). */
  set_server_records(sptr->name, sptr->maxclients, sptr->maxclients_time,
                     sptr->proxy_count, sptr->proxy_since);
  
  /* Return next server (valid ptr possibly updated when purging downlinks). */
  next = sptr->next;
  
  /* Free memory. */
  memory_free(sptr->cliT);
  memory_free(sptr);
  return next;
  }

/* Clients manipulations */

/* O(1) */
void
irc_network_add_client(yxx target, struct Client *cptr)
  {
  struct Server *sptr = irc_network_get_server(target.server_n);
  if (sptr == NULL)
    {
    /* TODO */
    exit(1);
    }
  
  /* Link at top */
  cptr->next = sptr->cliHead;
  cptr->prev_p = &sptr->cliHead;
  if (cptr->next)
    cptr->next->prev_p = &cptr->next;
  sptr->cliHead = cptr;
  
  sptr->cliT[target.client_n & sptr->nn_mask] = cptr;
  sptr->clients++;
  /* If needed, update the maxclients record for this server */
  if (sptr->maxclients < sptr->clients)
    sptr->maxclients = sptr->clients;
  }

/* O(1) */
void
irc_network_remove_client(yxx target)
  {
  struct Client *cptr;
  struct Server *sptr = irc_network_get_server(target.server_n);
  if (sptr == NULL)
    {
    /* TODO */
    exit(1);
    }
  cptr = sptr->cliT[target.client_n & sptr->nn_mask];
  
  /* Unlink */
  if (cptr->next)
    cptr->next->prev_p = cptr->prev_p;
  if (cptr->prev_p)
    *cptr->prev_p = cptr->next;
  cptr->next = NULL;
  cptr->prev_p = NULL;

  sptr->cliT[target.client_n & sptr->nn_mask] = NULL;
  sptr->clients--;
  }

/* O(1) */
struct Client *
irc_network_find_client(yxx target)
  {
  struct Server *sptr = irc_network_get_server(target.server_n);
  if (sptr == NULL)
    return NULL;
  return sptr->cliT[target.client_n & sptr->nn_mask];
  }

void
irc_network_map_servers(irc_network_map_servers_callback cb, void *ctx)
  {
  struct Server *sptr;

  for (sptr = gMe.next; sptr; sptr = sptr->next)
    (*cb)(sptr, ctx);
  }

void
irc_network_map_clients(struct Server *sptr,
                        irc_network_map_clients_callback cb, void *ctx)
  {
  if (sptr != NULL)
    {
    struct Client **cptr = sptr->cliT;
    int count = sptr->clients;
        
    for (; count > 0; cptr++)
      {
      if (*cptr != NULL)
        {
        *cptr = (*cb)(*cptr, ctx);
        count--;
        }
      }
    }
  }

/*****************************************************************************/

/* id = -1  -> last reset time (server_name=NULL)
 * id =  0  -> maxusers record
 * id =  1  -> maxusers record date
 * id =  2  -> proxy found record (glined clients count)
 * id =  3  -> last proxy count reset time
 */
static unsigned int
get_server_record(const char *server_name, int id)
  {
  unsigned int count = 0;
  int i;
  char buffer[256];
  char *p;
  FILE *fp;
  
  fp = fopen(gConfig->log.proxytop, "r");
  if (fp != NULL)
    {
    if (fgets(buffer, sizeof(buffer), fp))
      {
      if (id == -1)
        {
        fclose(fp);
        return strtoul(buffer, NULL, 10);
        }
      }
    while (fgets(buffer, sizeof(buffer), fp))
      {
      if ((p = strchr(buffer, ' ')))
        {
        *p++ = '\0';
        if (!strcasecmp(buffer, server_name))
          {
          char *tok = p;
          for (i = 0; (i <= id) && p; i++)
            {
            tok = p;
            if ((p = strchr(tok, ' ')) || (p = strchr(tok, '\n')))
              *p++ = '\0';
            }
          count = strtoul(tok, NULL, 10);
          break;
          }
        }
      }
    fclose(fp);
    }
  return count;
  }

static void
set_server_records(const char *server_name,
                   unsigned int maxusers_count,
                   time_t maxusers_time,
                   unsigned int proxy_count,
                   time_t proxy_since)
  {
  FILE *oldfp, *fp;
  char *p;
  char tempfile[MAXPATHLEN];
  char buffer[256];
  
  snprintf(tempfile, sizeof(tempfile), "%s.old", gConfig->log.proxytop);
  unlink(tempfile);
  rename(gConfig->log.proxytop, tempfile);
  
  if ((fp = fopen(gConfig->log.proxytop, "w")) == NULL)
    return;
  
  if ((oldfp = fopen(tempfile, "r")))
    {
    fprintf(fp, "%ld\n", (long)proxy_count_last_reset_time);
    while (fgets(buffer, sizeof(buffer), oldfp))
      {
      if ((p = strchr(buffer, ' ')))
        {
        *p = '\0';
        if (strcasecmp(buffer, server_name))
          {
          *p = ' ';
          fprintf(fp, "%s", buffer);
          }
        }
      }
    fclose(oldfp);
    }
  else
    fprintf(fp, "%ld\n", (long)proxy_count_last_reset_time);
  fprintf(fp, "%s %lu %ld %lu %ld\n", server_name,
          (unsigned long)maxusers_count, (long)maxusers_time,
          (unsigned long)proxy_count, (long)proxy_since);
  fclose(fp);
  }

static void
irc_network_set_servers_records_cb(struct Server *sptr, void *context)
  {
  set_server_records(sptr->name, sptr->maxclients, sptr->maxclients_time,
                     sptr->proxy_count, sptr->proxy_since);
  }

void
irc_network_set_servers_records()
  {
  irc_network_map_servers(irc_network_set_servers_records_cb, NULL);
  }

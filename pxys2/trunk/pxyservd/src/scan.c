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
#define RCSID "$Id: scan.c,v 1.7 2004/01/12 12:31:03 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef IP_L1_CACHE_SIZE
#define IP_L1_CACHE_SIZE 10000
#endif
#ifndef IP_L1_CACHE_EXPIRE_DELAY
#define IP_L1_CACHE_EXPIRE_DELAY 21600
#endif

#define IP4_L1_CACHE_SIZE IP_L1_CACHE_SIZE
#define IP6_L1_CACHE_SIZE IP_L1_CACHE_SIZE

/* SCAN_TIMER_DELAY defines the delay of which pxyservd will check if
 * some queries have failed (timeout) in order to retry.
 */
#define SCAN_TIMER_DELAY        10

/* SCAN_QUERY_TIMEOUT defines a minimal timeout delay in seconds for a query.
 * A query won't be resent until this time is elapsed.
 * Should not be too low, or pxyscand will receive each time
 * several queries for the same IP, for example.
 * Shouldn't be too high, or it will alter the reactivity, when for
 * example, pxyscand restarts.
 * 3-5 minutes is probably a good value.
 */
#define SCAN_QUERY_TIMEOUT     200

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "scan.h"
#include <arpa/inet.h>

#include <opas/opas.h>
#include <peak/peak.h>

#include "opas_support.h"
#include "cfgloader.h"
#include "debug.h"
#include "evreg.h"
#include "ipcache.h"
#include "irc_gline.h"
#include "irc_network.h"
#include "irc_numnicks.h"
#include "irc_send.h"
#include "irc_userbase.h"
#include "irc_yxx.h"
#include "match.h"
#include "pxyservd_log.h"

#include "PXServiceMsg.h"

extern void cmd_status_reply(struct Client *cptr, PXSStatus *status,
                             size_t length);
extern void cmd_pxstats_reply(struct Client *cptr, PXSStats *stats);
extern void cmd_grem_reply(struct Client *cptr, PXSRemove4 *rem_reply);
extern void cmd_recheck_reply(struct Client *cptr, PXSRemove4 *rem_reply);

static void scan_timer_callback(peak_timer ti, void *context);
static int  scan_check_cache(unsigned int flags, void *addrData,
                             time_t *out_scan_ts);
static void scan_query(unsigned int flags, const void *addrData,
                       uint32_t user_data);
static void scan_client_add(struct Client *cptr);

uint32_t l1_cache_hits; /* stats */

/* no-proxy little caches */
static ipcache_t ipcache4;
#ifdef ENABLE_IPV6
static ipcache_t ipcache6;
#endif
static struct Client *scan_client_head, **scan_client_tail_p;
static int scan_client_count;
static int unscannable_client_count;
static peak_timer scan_timer;

void
scan_init()
  {
  ipcache4 = ipcache_create_in4(IP4_L1_CACHE_SIZE, 0);
#ifdef ENABLE_IPV6
  ipcache6 = ipcache_create_in6(IP6_L1_CACHE_SIZE, 0);
#endif
  if (opas_support_init() == -1)
    {
    Debug((DL_BASIC, "scan_init: opas_support_init failed!"));
    /* XXX */
    abort();
    }
  
  scan_client_head = NULL;
  scan_client_tail_p = NULL;
  scan_client_count = 0;
  l1_cache_hits = 0;
  unscannable_client_count = 0;
  
  scan_timer = peak_timer_create(SCAN_TIMER_DELAY, SCAN_TIMER_DELAY,
                                 scan_timer_callback, NULL);
  peak_task_timer_add(peak_task_self(), scan_timer);
  peak_release(scan_timer); /* so that peak_task_timer_remove() releases it */
  }

void
scan_finalize()
  {
  peak_task_timer_remove(peak_task_self(), scan_timer); /* implicit release */
  
  opas_support_finalize();

#ifdef ENABLE_IPV6
  ipcache_dispose(ipcache6);
#endif
  ipcache_dispose(ipcache4);
  }

static void
scan_timer_callback(peak_timer ti, void *context)
  {
  struct Client *cptr, *cnext;
  time_t now = peak_time();
  
  if (!scan_client_count)
    return;
  
  /* Check for timed out scan queries.
   */
  for (cptr = scan_client_head; cptr; cptr = cnext)
    {
    cnext = cptr->scan_next;
    
    /* List is ordered, we know we can break. */
    if (cptr->scan_timestamp + SCAN_QUERY_TIMEOUT > now)
      break;
    
    /* Try again ! It's our job damnit. */
    scan_client_remove(cptr);
    scan_start(cptr);
    }
  }

static int
scan_check_cache(unsigned int flags, void *addrData, time_t *out_scan_ts)
  {
  time_t ts = 0;
  
  if (!(flags & CLIENT_FLAG_IPV6))
    ts = ipcache_find_in4(ipcache4, (struct in_addr *)addrData, NULL);
#ifdef ENABLE_IPV6
  else
    ts = ipcache_find_in6(ipcache6, (struct in6_addr *)addrData, NULL);
#endif
  
  if (ts > 0 && (peak_time() - ts <= IP_L1_CACHE_EXPIRE_DELAY))
    {
    *out_scan_ts = ts;
    return 1;
    }
  return 0;
  }

int
scan_check_noscan_server(struct Server *sptr)
  {
  CNoScanLink *lk;
  
  for (lk = gConfig->noscanlist; lk; lk = lk->next)
    {
    if (lk->noscan.type == NOSCAN_TYPE_SERVER
        && !match(lk->noscan.u.server, sptr->name))
      return 1; /* noscan! */
    }
  return 0;
  }

int
scan_check_noscan(const struct Client *cptr)
  {
  CNoScanLink *lk;
  struct Server *sptr;
  
  for (lk = gConfig->noscanlist; lk; lk = lk->next)
    {
    switch (lk->noscan.type)
      {
      case NOSCAN_TYPE_SERVER:
        sptr = irc_network_get_server(cptr->nserv);
        if (!match(lk->noscan.u.server, sptr->name))
          return 1; /* noscan! */
        break;
        
      case NOSCAN_TYPE_USERIP:
        {
        if (cptr->flags & CLIENT_FLAG_IPV6)
          break; /* No IPv6 noscan support yet */
        
        if ((cptr->addr.ip4.s_addr & lk->noscan.u.userip.netmask.s_addr)
            == (lk->noscan.u.userip.network.s_addr
                & lk->noscan.u.userip.netmask.s_addr))
          return 1; /* noscan! */
        break;
        }
      }
    }
  return 0;
  }

static void
scan_query(unsigned int flags, const void *addrData, uint32_t user_data)
  {
  int res;
  
  if (!(flags & CLIENT_FLAG_IPV6))
    res = opas_support_query((struct in_addr *)addrData, user_data);
  else
    res = opas_support_query6((struct in6_addr *)addrData, user_data);
  
  if (res == -1)
    {
    char ipbuf[32];
    
    inet_ntop(flags & CLIENT_FLAG_IPV6 ? AF_INET6 : AF_INET, addrData,
              ipbuf, sizeof(ipbuf));
    Debug((DL_BASIC, "scan_query: opas_support_query failed for %s\n",
           ipbuf));
    }
  }

static void
scan_client_add(struct Client *cptr)
  {
  assert(!(cptr->flags & CLIENT_FLAG_SCANNING));
  
  /* Set scanning flag for this client. */
  cptr->flags |= CLIENT_FLAG_SCANNING;
  
  /* Link at tail */
  cptr->scan_next = NULL;
  cptr->scan_prev_p = scan_client_tail_p;
  
  if (cptr->scan_prev_p)
    *cptr->scan_prev_p = cptr;
  else
    scan_client_head = cptr;
  scan_client_tail_p = &cptr->scan_next;
  
  scan_client_count++;
  }

void
scan_client_remove(struct Client *cptr)
  {
  if (cptr->flags & CLIENT_FLAG_SCANFAIL)
    unscannable_client_count--;
  
  if (!(cptr->flags & CLIENT_FLAG_SCANNING))
    return;
  
  if (cptr->scan_prev_p)
    *cptr->scan_prev_p = cptr->scan_next;
  else
    scan_client_head = cptr->scan_next;
  
  if (cptr->scan_next)
    cptr->scan_next->scan_prev_p = cptr->scan_prev_p;
  else
    scan_client_tail_p = cptr->scan_prev_p;
  
  cptr->scan_next = NULL;
  cptr->scan_prev_p = NULL;
  
  scan_client_count--;
  
  cptr->flags &= ~CLIENT_FLAG_SCANNING;
  }

int
scan_client_get_count()
  {
  return scan_client_count;
  }

int
scan_client_get_unscannable_count()
  {
  return unscannable_client_count;
  }

/* External method scan_new_user()
 * A new user has just connected to the network.
 */
void
scan_new_user(struct Client *cptr)
  {
  /* First, check the noscan list and ignore user if it matchs
   */
  if (scan_check_noscan(cptr))
    return;
  
  /* Then, check our local "level one" cache of scanned IPs.
   */
  if (scan_check_cache(cptr->flags, &cptr->addr, &cptr->scan_timestamp))
    {
    l1_cache_hits++;
    return;
    }
  
  /* Lame notice? ;)
   */
  if (gConfig->noticelist && 
      !irc_network_server_is_bursting(irc_network_get_server(cptr->nserv)))
    {
    CNoticeLink *notice;
    
    for (notice = gConfig->noticelist; notice; notice = notice->next)
      {
      char dst[6];
      inttobase64(dst, cptr->nserv, 2);
      inttobase64(dst + 2, cptr->nnick, 3);
      dst[5] = '\0';
      send_to_one(dst, "%s", notice->line);
      }
    }
  
  /* No luck, start scan procedure.
   */
  scan_start(cptr);
  }

void
scan_start(struct Client *cptr)
  {
  cptr->scan_timestamp = peak_time();
  scan_client_add(cptr);
  
  if (opas_support_is_ready())
    scan_query(cptr->flags, &cptr->addr, yxx_pack_int(cptr->nserv,
                                                      cptr->nnick));
  }

int
scan_send_command(struct Client *cptr, uint32_t sig, uint32_t cmd,
                  const void *data, size_t length)
  {
  PXSHeader *hp = (PXSHeader *)data;
  
#if 0
  if (cptr->flags & CLIENT_FLAG_COMMAND)
    return 0;
#endif
  
  if (!opas_support_is_ready())
    return -1; /* failure */
  
  cptr->flags |= CLIENT_FLAG_COMMAND;
  
  hp->sig = htonl(sig);
  hp->ver = htonl(PX_VERSION);
  hp->cmd = htonl(cmd);
  
  opas_support_send_msg_user(hp, length, yxx_pack_int(cptr->nserv,
                                                      cptr->nnick));
  return 0;
  }

int
scan_send_simple_command(struct Client *cptr, uint32_t sig, uint32_t cmd)
  {
  PXSHeader head;
  return scan_send_command(cptr, sig, cmd, &head, sizeof(head));
  }

/* Replies from opas_support module.. */
void
scan_reply_noproxy(const struct in_addr *addrp, uint32_t ud, int cached)
  {
  struct Client *cptr = irc_network_find_client(yxx_unpack(ud));
  /* Verify that, if a client still exists for this numeric, he has the same
   * IP that we've just scanned.
   */
  if (cptr && cptr->addr.ip4.s_addr == addrp->s_addr
      && (cptr->flags & CLIENT_FLAG_SCANNING))
    {
    scan_client_remove(cptr);
    ipcache_add_in4(ipcache4, addrp, peak_time(), NULL);
    Debug((DL_BASIC, "scan_reply_noproxy: scan took %d secs",
           peak_time() - cptr->scan_timestamp));
    }
  }

void
scan_reply_proxy(const struct in_addr *addrp, uint32_t ud, int cached,
                 int proxy_type, uint16_t proxy_port, const char *proxy_descr)
  {
  const char *reason;
  struct Client *cptr = irc_network_find_client(yxx_unpack(ud));
  
  /* Verify that, if a client still exists for this numeric, he has the same
   * IP that we've just scanned.
   */
  if (cptr && cptr->addr.ip4.s_addr == addrp->s_addr
      && (cptr->flags & CLIENT_FLAG_SCANNING))
    {
    char ipbuf[16];
    int cnt;
    time_t scantime;
    
    scan_client_remove(cptr);
    
    scantime = peak_time() - cptr->scan_timestamp;
    
    if (!inet_ntop(AF_INET, &cptr->addr.ip4, ipbuf, sizeof(ipbuf)))
      return;
    
    /* /!\ O(n) count but everyone likes it...
     *     Used for proxytop's stats too.
     */
    cnt = irc_userbase_proxycount(&cptr->addr.ip4);
    
    if (cached)
      {
#ifdef SPANISH
      log_write(LOGID_CURRENT, "*@%s [%ld] %s (%d) cacheado", ipbuf, cnt,
                proxy_descr, proxy_port);

      if (gConfig->client.show_cached)
      {
        if (proxy_type != 10)
        send_msg_client_to_console("PG *@%s [%ld] %s en el puerto %u (cached)",
                                   ipbuf, cnt, proxy_descr, proxy_port);
        else
        send_msg_client_to_console("ATENCION: [%ld] La IP %s tiene el router ADSL abierto en el puerto 23!. No se Glinea. (cached)", cnt, ipbuf);

      }

      evreg_broadcast(EVREG_FLAG_CACHED,
                      "[EV] PG *@%s [%ld] %s en el puerto %u (cached)",
                      ipbuf, cnt, proxy_descr, proxy_port);
#else
      log_write(LOGID_CURRENT, "*@%s [%ld] %s (%d) cached", ipbuf, cnt,
                proxy_descr, proxy_port);
      
      if (gConfig->client.show_cached)
        send_msg_client_to_console("PG *@%s [%ld] %s at port %u (cached)",
                                   ipbuf, cnt, proxy_descr, proxy_port);
      
      evreg_broadcast(EVREG_FLAG_CACHED,
                      "[EV] PG *@%s [%ld] %s at port %u (cached)",
                      ipbuf, cnt, proxy_descr, proxy_port);
#endif
      }
    else
      {
      /* Logging */
      log_write(LOGID_CURRENT, "*@%s [%ld] %s (%d)", ipbuf, cnt,
                proxy_descr, proxy_port);

#ifdef SPANISH
      /* Console channel */
      if (proxy_type != 10)
      send_msg_client_to_console("PG *@%s [%ld] %s en el puerto %u (%ds)", ipbuf,
                                 cnt, proxy_descr, proxy_port, scantime);
      else
      send_msg_client_to_console("ATENCION: [%ld] La IP %s tiene el router ADSL abierto en el puerto 23!. No se Glinea. (%ds)", cnt, ipbuf, scantime);

      /* Private event notification */
      evreg_broadcast(EVREG_FLAG_NEWPROXY,
                      "[EV] PG *@%s [%ld] %s en el puerto %u (%ds)",
                      ipbuf, cnt, proxy_descr, proxy_port, scantime);
#else      
      /* Console channel */
      send_msg_client_to_console("PG *@%s [%ld] %s at port %u (%ds)", ipbuf,
                                 cnt, proxy_descr, proxy_port, scantime);
      
      /* Private event notification */
      evreg_broadcast(EVREG_FLAG_NEWPROXY,
                      "[EV] PG *@%s [%ld] %s at port %u (%ds)",
                      ipbuf, cnt, proxy_descr, proxy_port, scantime);
#endif
      }
    
    if (proxy_type >= 0 && proxy_type < 13)
      reason = gConfig->gline.reason[proxy_type];
    else
      reason = gConfig->gline.reason[0];

#if 1 /* Temporal, puerto 23 Router ADSL abierto */
    if (proxy_type != 10)
      irc_gline_send(addrp, cnt, reason, proxy_port);
/*
    else
      send_msg_client_to_console("ATENCION: La IP %s tiene el router ADSL abierto!", ipbuf);
*/
#else
    irc_gline_send(addrp, cnt, reason, proxy_port);
#endif
    }
  }

void
scan_reply_error(const struct in_addr *addrp, uint32_t ud, uint32_t error)
  {
  /* Ok, can't scan this IP.. no luck ! */
  struct Client *cptr = irc_network_find_client(yxx_unpack(ud));
  
  if (cptr && cptr->addr.ip4.s_addr == addrp->s_addr
      && (cptr->flags & CLIENT_FLAG_SCANNING) && error != 1000)
    {
    char ipbuf[16];
    
    scan_client_remove(cptr);
    cptr->flags |= CLIENT_FLAG_SCANFAIL;
    unscannable_client_count++;
    
    /* Log it. */
    if (!inet_ntop(AF_INET, &cptr->addr.ip4, ipbuf, sizeof(ipbuf)))
      return;
    
    log_write(LOGID_UNREACH, "%ld,%s", peak_time(), ipbuf);
    }
  }

void
scan_reply6_noproxy(const struct in6_addr *addrp, uint32_t ud, int cached)
  {
  /* Unused for now (who could we blame ?? :) */
  }

void
scan_reply6_proxy(const struct in6_addr *addrp, uint32_t ud, int cached,
                  int proxy_type, uint16_t proxy_port, const char *proxy_descr)
  {
  /* Unused for now */
  }

void
scan_reply6_error(const struct in6_addr *addrp, uint32_t ud, uint32_t error)
  {
  /* Unused for now */
  }

void
scan_reply_command(void *data, size_t length, uint32_t ud)
  {
  struct Client *cptr = irc_network_find_client(yxx_unpack(ud));
  
  if (cptr && (cptr->flags & CLIENT_FLAG_COMMAND)
      && (length > sizeof(PXSHeader)))
    {
    PXSHeader *head = (PXSHeader *)data;
    
    head->sig = ntohl(head->sig);
    head->ver = ntohl(head->ver);
    head->cmd = ntohl(head->cmd);
    
    cptr->flags &= ~CLIENT_FLAG_COMMAND;
    
    if (head->sig != PXYSCAND_SIG || head->ver != PX_VERSION)
      return;
    
    /* Reply looks fine */
    switch (head->cmd)
      {
      case PX_CMD_STATUS:
        cmd_status_reply(cptr, (PXSStatus *)data, length);
        break;
      case PX_CMD_STATS:
        cmd_pxstats_reply(cptr, (PXSStats *)data);
        break;
      case PX_CMD_REMOVE:
        if (cptr->flags & CLIENT_FLAG_GREM)
          cmd_grem_reply(cptr, (PXSRemove4 *)data);
        else
          cmd_recheck_reply(cptr, (PXSRemove4 *)data);
        break;
      default:
        break;
      }
    }
  }

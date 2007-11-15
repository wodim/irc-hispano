/* Copyright (C) 2003 Stephane Thiell
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
#define RCSID "$Id: cmd_status.c,v 1.3 2005/05/19 00:49:38 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_network.h"
#include "irc_numnicks.h"
#include "irc_userbase.h"
#include "opas_support.h"
#include "scan.h"
#include "PXServiceMsg.h"

#include <string.h>
#include <peak/peak.h>
#include <arpa/inet.h>

extern time_t gBirthTime;
extern time_t opas_support_last_connection;
extern time_t opas_support_last_disconnection;
extern int msg_bogus;

static void
server_status(const char *dst)
  {
  time_t t;
  
  send_client_to_one(dst, "\2IRC server status\2");
  send_client_to_one(dst, "  Servers |  Clients | Channels ");
  send_client_to_one(dst, "--------------------------------");
  send_client_to_one(dst, "%9d |%9d |%9d", irc_network_get_server_count(),
                     irc_userbase_get_count(), irc_channel_get_count());
  send_client_to_one(dst, "--------------------------------");
  
  if (msg_bogus > 0)
    send_client_to_one(dst, "Bogus IRC messages: %d", msg_bogus);
  
  send_client_to_one(dst, "Scan ClientQ: %d users", scan_client_get_count());
  send_client_to_one(dst, "Unscannable clients: %d",
                     scan_client_get_unscannable_count());
  
  t = peak_time() - gBirthTime;
  if (t >= 86400)
    send_client_to_one(dst, "Server uptime: %dd %d:%02d:%02d", t / 86400,
                       (t / 3600) % 24, (t / 60) % 60, t % 60);
  else
    send_client_to_one(dst, "Server uptime: %d:%02d:%02d",
                       (t / 3600) % 24, (t / 60) % 60, t % 60);
  
  send_client_to_one(dst, "--------------------------------");
  }

static void
scanner_status(struct Client *cptr, const char *dst)
  {
  peak_tz tz;
  peak_time_date gdate;
  
  tz = peak_tz_create_system();
  
  send_client_to_one(dst, "\2Scanner status\2");
  send_client_to_one(dst, "Connection with pxyscand is %sestablished",
                     opas_support_is_ready() ? "" : "NOT ");
  
  if (opas_support_is_ready())
    {
    gdate = peak_time_get_date(opas_support_last_connection, tz);
    send_client_to_one(dst, "Connected since %d-%d-%d %d:%02d:%02d %s",
                       gdate.year, gdate.month, gdate.day, gdate.hour,
                       gdate.minute, (int)gdate.second,
                       peak_tz_get_abbreviation(
                         tz, opas_support_last_connection));
    }
  else
    {
    gdate = peak_time_get_date(opas_support_last_disconnection, tz);
    send_client_to_one(dst, "Disconnected since %d-%d-%d %d:%02d:%02d %s",
                       gdate.year, gdate.month, gdate.day, gdate.hour,
                       gdate.minute, (int)gdate.second,
                       peak_tz_get_abbreviation(
                         tz, opas_support_last_disconnection));
    }
  
  if (scan_send_simple_command(cptr, PXYSCAND_SIG, PX_CMD_STATUS) == -1)
    send_client_to_one(dst, "/!\\ Scanner daemon not connected."
                       " Can't retrieve status data from pxyscand.");
  
  peak_release(tz);
  }

void
cmd_status(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  
  if (ttab->size > 4)
    {
    if (!strcasecmp(ttab->tok[4], "-serv"))
      server_status(dst);
    else if (!strcasecmp(ttab->tok[4], "-scan"))
      scanner_status(cptr, dst);
    else
      send_client_to_one(dst, "Syntax: STATUS [-serv|-scan]");
    }
  else
    {
    server_status(dst);
    scanner_status(cptr, dst);
    }
  }

void
cmd_status_reply(struct Client *cptr, PXSStatus *status, size_t length)
  {
  char dst[6];
  time_t t;
  int cur, tot;
  uint32_t total;
  double activity;
  int n, i;
  
  inttobase64(dst, cptr->nserv, 2);
  inttobase64(dst + 2, cptr->nnick, 3);
  dst[5] = '\0';
  
  cur = (int)ntohl(status->curIPScan);
  tot = (int)ntohl(status->maxIPScan);
  if (tot <= 0) tot = 1;
  activity = 100.0 * (double)cur/(double)tot;
  
  send_client_to_one(dst, "Scanning activity: \2%.1f%%\2 "
                     "(%lu/%lu IPs, %lu/%lu scans)", activity, cur, tot,
                     ntohl(status->curRunScan), ntohl(status->maxRunScan));
  
  total = ntohl(status->sessScannedCount);
  t = peak_time() - opas_support_last_connection;
  send_client_to_one(dst, "IP scans completed: \2%lu\2 (%lu) - "
                     "average: \2%.0f/h\2", total,
                     ntohl(status->servScannedCount),
                     3600.0 * (double)total / (double)t);
  send_client_to_one(dst, "ScanQ size: %lu IPs", ntohl(status->scanQSize));
  send_client_to_one(dst, "Target: %s:%u (last checked %d secs ago)",
                     inet_ntoa(status->targetAddr),
                     (uint16_t)status->targetPort,
                     ntohl(status->targetLastCheck));
  send_client_to_one(dst, "Scan connect() timeout: %lu secs",
                     ntohl(status->timeout));
  
  send_client_to_one(dst, "--------------------------------");
  
  t = (time_t)ntohl(status->uptime);
  total = ntohl(status->servProxyCount);
  if (t >= 86400)
    send_client_to_one(dst, "Proxy found counter: \2%lu\2 in "
                       "%dd %d:%02d:%02d", total,
                       t / 86400, (t / 3600) % 24, (t / 60) % 60, t % 60);
  else
    send_client_to_one(dst, "Proxy found counter: \2%lu\2 in %d:%02d:%02d",
                       total, (t / 3600) % 24, (t / 60) % 60, t % 60);
  
  n = ntohl(status->numMInfo);
  for (i = 0; i < n; i++)
    {
    PXSMInfo *info = ((PXSMInfo *)(status + 1) + i);
    uint32_t count;
    double percent;
    
    count = ntohl(info->proxyCount);
    percent = total > 0 ? 100.0 * (double)count / (double)total : 0.0;
    
    send_client_to_one(dst, " - %s/%u: \2%lu\2 [%.1f%%] (%lu open)",
                       info->shortName,
                       (uint16_t)ntohl(info->port),
                       count, percent, ntohl(info->connCount));
    }
  }

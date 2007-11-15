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
#define RCSID "$Id: cmd_proxytop.c,v 1.2 2004/01/10 14:32:29 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_cmd.h"
#include "irc_network.h"
#include "irc_send.h"
#include "irc_struct.h"

#include <stdlib.h>
#include <string.h>

#include <peak/peak.h>

extern time_t proxy_count_last_reset_time;

static void
proxytop_server_cb(struct Server *sptr, void *context)
  {
  struct Server ***p = (struct Server ***)context;
  *(*p)++ = sptr;
  }

static int
proxytop_comparator(const void *s1, const void *s2)
  {
  struct Server *server1 = *(struct Server **)s1;
  struct Server *server2 = *(struct Server **)s2;
  unsigned int t1 = server1->proxy_count;
  unsigned int t2 = server2->proxy_count;
  int result;
  
  if (t1 < t2)
    result = 1;
  else if (t1 > t2)
    result = -1;
  else if (server1->proxy_since > server2->proxy_since)
    result = 1;
  else if (server1->proxy_since < server2->proxy_since)
    result = -1;
  else
    result = strcmp(server1->name, server2->name);
  
  return result;
  }

void
cmd_proxytop(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  struct Server **base, **p, *s;
  unsigned int total = 0;
  int count, i, rank;
  peak_tz tz = peak_tz_create_system();
  peak_time_date gdate;
  
  count = irc_network_get_remote_server_count();
  
#ifdef HAVE_ALLOCA
  base = (struct Server **)alloca(count * sizeof(struct Server *));
#else
  base = (struct Server **)peak_allocate(count * sizeof(struct Server *));
#endif
  
  p = base;
  irc_network_map_servers(proxytop_server_cb, &p);
  
  qsort(base, count, sizeof(struct Server *), proxytop_comparator);
  
  for (i = 0, p = base; i < count; i++, p++)
    total += (*p)->proxy_count;
  
  if (!total)
    total = 1;
  
  send_client_to_one(dst, "RANK SERVER PROXY-COUNT/PERCENTAGE (MAXUSERS)");
  
  for (i = 0, p = base, rank = 1; i < count; i++)
    {
    s = *p++;
    if (s->proxy_count || s->maxclients)
      send_client_to_one(dst, "%02d %s%s \2%lu\2 %.2f%% (%lu)",
                         rank++,
                         (s->flags & SERVER_FLAG_BURSTING) ? "\2!\2" : " ",
                         s->name,
                         s->proxy_count,
                         100.0 * (double)s->proxy_count/(double)total,
                         s->maxclients);
    }
  if (tz)
    {
    time_t last = proxy_count_last_reset_time;
    gdate = peak_time_get_date(last, tz);
    send_client_to_one(dst, "PROXYTOP SINCE %d-%d-%d %d:%02d:%02d %s",
                       gdate.year, gdate.month, gdate.day, gdate.hour,
                       gdate.minute, (int)gdate.second,
                       peak_tz_get_abbreviation(tz, last));
    }
#ifndef HAVE_ALLOCA
  peak_deallocate(base);
#endif
  peak_release(tz);
  }

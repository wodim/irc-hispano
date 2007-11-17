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
#define RCSID "$Id: cmd_info.c,v 1.1 2004/01/10 18:15:36 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_userbase.h"
#include "scan.h" /* scan_check_noscan() */

#include <peak/peak.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct _iter_pack
  {
  const char *dst;
  peak_tz tz;
  };

static unsigned long
info_chan_iter_cb(smat_table_t *table, smat_entry_t *e, void *extra)
  {
  struct Client *cptr;
  struct _iter_pack *pack = (struct _iter_pack *)extra;
  const char *dst = pack->dst;
  peak_tz tz = pack->tz;
  peak_time_date gdate;
  char host[64], sbuf[128];
  int af;
  
  cptr = se_object(e, MEMBERSHIP_LOC_CLIENT);
  
  af = (cptr->flags & CLIENT_FLAG_IPV6) ? AF_INET6 : AF_INET;
  
  inet_ntop(af, &cptr->addr, host, sizeof(host));
  
  if (cptr == MYCLIENT_PTR)
    strcpy(sbuf, "myself!");
  else if (cptr->flags & CLIENT_FLAG_SCANNING)
#ifdef SPANISH
    snprintf(sbuf, sizeof(sbuf), "en proceso de escaneo (lleva %ld segundos)",
#else
    snprintf(sbuf, sizeof(sbuf), "being scanned (query %ld secs ago)",
#endif
             peak_time() - cptr->scan_timestamp);
  else if (cptr->flags & CLIENT_FLAG_SCANFAIL)
#ifdef SPANISH
    snprintf(sbuf, sizeof(sbuf), "no escaneable (ha fallado hace %ld segundos)",
#else
    snprintf(sbuf, sizeof(sbuf), "unscannable (failed %ld secs ago)",
#endif
             peak_time() - cptr->scan_timestamp);
  else if (!scan_check_noscan(cptr))
    {
    if (!cptr->scan_timestamp)
#ifdef SPANISH
      strcpy(sbuf, "no escaneado todavia");
#else
      strcpy(sbuf, "not scanned yet");
#endif
    else
      {
      gdate = peak_time_get_date(cptr->scan_timestamp, tz);
#ifdef SPANISH
      snprintf(sbuf, sizeof(sbuf), "escaneado el dia %d-%d-%d a las %d:%02d:%02d %s",
#else
      snprintf(sbuf, sizeof(sbuf), "scanned @ %d-%d-%d %d:%02d:%02d %s",
#endif
               gdate.year, gdate.month, gdate.day,
               gdate.hour, gdate.minute, (int)gdate.second,
               peak_tz_get_abbreviation(tz, cptr->scan_timestamp));
      }
    }
  else
    strcpy(sbuf, "NOSCAN");
  
  send_client_to_one(dst, "%s %s %s (%s)", cptr->nick, cptr->user, host, sbuf);
  
  return 0;
  }

static void
info_chan(const char *dst, toktabptr ttab)
  {
  struct Channel *c;
  unsigned long err, cnt;
  peak_tz tz;
  
  if (!(c = irc_channel_get(ttab->tok[4])))
    {
#ifdef SPANISH
    send_client_to_one(dst, "Canal no encontrado.");
#else
    send_client_to_one(dst, "Channel not found.");
#endif
    return;
    }
  
  if (ttab->size > 5)
    tz = peak_tz_create(ttab->tok[5]);
  else
    tz = peak_tz_create_system();
  
  if (!tz)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Lo siento, el 'time zone' es desconocido para mi");
#else
    send_client_to_one(dst, "Sorry, this time zone is unknown for me");
#endif
    return;
    }
  
  cnt = sh_count(&c->mhead);
  if (cnt > 0)
    {
    struct _iter_pack pack = { dst, tz };
    err = sh_iter(&c->mhead, NULL, info_chan_iter_cb, &pack, 0);
    assert(err == 0);
#ifdef SPANISH
    send_client_to_one(dst, "%s - %lu usuarios", c->chname, cnt);
#else
    send_client_to_one(dst, "%s - %lu users", c->chname, cnt);
#endif
    }
  else
    {
    /* Channel chucked, not yet really deleted. */
#ifdef SPANISH
    send_client_to_one(dst, "El canal ha sido borrado: no hay usuarios");
#else
    send_client_to_one(dst, "Channel's just been deleted: no user found");
#endif
    }
  
  peak_release(tz);
  }

/* Get info about a nickname (ex whois command)
 */
static void
info_nick(const char *dst, toktabptr ttab)
  {
  struct Client *u;
  struct Server *sptr;
  peak_tz tz;
  peak_time_date gdate;
  char host[64];
  int af;
  
  if (!(u = irc_userbase_get_by_nick(ttab->tok[4])))
    {
#ifdef SPANISH
    send_client_to_one(dst, "Usuario no encontrado.");
#else
    send_client_to_one(dst, "User not found.");
#endif
    return;
    }
  
  if (ttab->size > 5)
    tz = peak_tz_create(ttab->tok[5]);
  else
    tz = peak_tz_create_system();
  
  if (!tz)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Lo siento, el 'time zone' es desconocido para mi");
#else
    send_client_to_one(dst, "Sorry, this time zone is unknown for me");
#endif
    return;
    }
  
  if (!(u->flags & CLIENT_FLAG_IPV6))
    af = AF_INET;
  else
    af = AF_INET6;
  
  inet_ntop(af, &u->addr, host, sizeof(host));
#ifdef SPANISH
  send_client_to_one(dst, "%s es %s@%s", u->nick, u->user, host);
  if (u->flags & CLIENT_FLAG_OPER)
    send_client_to_one(dst, "%s es un \"IRC Operator\"", u->nick);
#else
  send_client_to_one(dst, "%s is %s@%s", u->nick, u->user, host);
  if (u->flags & CLIENT_FLAG_OPER)
    send_client_to_one(dst, "%s is an IRC Operator", u->nick);
#endif
  
  sptr = irc_network_get_server(u->nserv);
#ifdef SPANISH
  send_client_to_one(dst, "%s esta en IRC utilizando el servidor %s", u->nick, sptr->name);
#else
  send_client_to_one(dst, "%s is on IRC via %s", u->nick, sptr->name);
#endif
  gdate = peak_time_get_date(u->firsttime, tz);
#ifdef SPANISH
  send_client_to_one(dst, "%s esta conectado desde el dia %d-%d-%d a las %d:%02d:%02d %s", u->nick,
#else
  send_client_to_one(dst, "%s signed on at %d-%d-%d %d:%02d:%02d %s", u->nick,
#endif
                     gdate.year, gdate.month, gdate.day, gdate.hour,
                     gdate.minute, (int)gdate.second,
                     peak_tz_get_abbreviation(tz, u->firsttime));
  
  /* Scan stuffs */
  if (u->flags & CLIENT_FLAG_SCANNING)
#ifdef SPANISH
    send_client_to_one(dst, "%s esta siendo escaneado (desde hace %d segundos)",
#else
    send_client_to_one(dst, "%s is being scanned (scan query %d secs ago)",
#endif
                       u->nick, peak_time() - u->scan_timestamp);
  else if (u->flags & CLIENT_FLAG_SCANFAIL)
#ifdef SPANISH
    send_client_to_one(dst, "%s no es escaneable (ultimo escaneo ha fallado hace %s segundos)",
#else
    send_client_to_one(dst, "%s is unscannable (last scan failed %d secs ago)",
#endif
                       u->nick, peak_time() - u->scan_timestamp);
  else if (!scan_check_noscan(u))
    {
    if (!u->scan_timestamp)
#ifdef SPANISH
      send_client_to_one(dst, "%s todavia no ha sido escaneado", u->nick);
#else
      send_client_to_one(dst, "%s has not been scanned yet", u->nick);
#endif
    else
      {
      gdate = peak_time_get_date(u->scan_timestamp, tz);
      
#ifdef SPANISH
      send_client_to_one(dst, "%s fue escaneado el dia "
                         "%d-%d-%d a las %d:%02d:%02d %s",
#else
      send_client_to_one(dst, "%s was last scanned at "
                         "%d-%d-%d %d:%02d:%02d %s",
#endif
                         u->nick, gdate.year, gdate.month, gdate.day,
                         gdate.hour, gdate.minute, (int)gdate.second,
                         peak_tz_get_abbreviation(tz, u->scan_timestamp));
      }
    }
  else
#ifdef SPANISH
    send_client_to_one(dst, "Los atributos de %s coincide con las reglas de \"NOSCAN\" "
                       " - escaneo deshabilitado", u->nick);
#else
    send_client_to_one(dst, "Attributes of %s match \"NOSCAN\" rules"
                       " - scanning disabled", u->nick);

#endif
  
  peak_release(tz);
  }

void
cmd_info(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  
  if (ttab->size < 5)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Sintaxis: INFO <nick|canal> [Timezone]");
#else
    send_client_to_one(dst, "Syntax: INFO <nickname|channel> [TZ]");
#endif
    return;
    }
  
  if (IsChannelName(ttab->tok[4]))
    info_chan(dst, ttab);
  else
    info_nick(dst, ttab);
  }

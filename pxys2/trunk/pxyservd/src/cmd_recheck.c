/* Copyright (C) 2004 Stephane Thiell
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
#define RCSID "$Id: cmd_recheck.c,v 1.1 2004/01/12 12:32:34 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_userbase.h"
#include "scan.h" /* scan_check_noscan() */
#include "PXServiceMsg.h"

#include <peak/peak.h>

#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct _iter_pack
  {
  struct Client *cptr;
  const char *dst;
  uint32_t noscan_cnt;
  uint32_t scan_cnt;
  };

static unsigned long
recheck_chan_iter_cb(smat_table_t *table, smat_entry_t *e, void *extra)
  {
  struct _iter_pack *pack = (struct _iter_pack *)extra;
  struct Client *target = se_object(e, MEMBERSHIP_LOC_CLIENT);
  PXSRemove4 rem_request;
  
  if (scan_check_noscan(target))
    {
    pack->noscan_cnt++;
    return 0;
    }
  
  if (!(target->flags & (CLIENT_FLAG_IPV6|CLIENT_FLAG_SCANNING)))
    {
    rem_request.addr = target->addr.ip4;
    if (scan_send_command(pack->cptr, PXYSCAND_SIG, PX_CMD_REMOVE,
                          &rem_request, sizeof(rem_request)) == -1)
      {
#ifdef SPANISH
      send_client_to_one(pack->dst, "/!\\ El demonio de Escaneo no está conectado."
                         " No se puede rescanear en este momento.");
#else
      send_client_to_one(pack->dst, "/!\\ Scanner daemon not connected."
                         " Can't recheck at the moment.");
#endif
      return 1; /* stop iterate */
      }
    pack->scan_cnt++;
    scan_start(target); /* start scan */
    }
  
  return 0;
  }

static void
recheck_chan(struct Client *cptr, const char *dst, toktabptr ttab)
  {
  struct Channel *c;
  unsigned long err, cnt;
  
  if (!(c = irc_channel_get(ttab->tok[4])))
    {
#ifdef SPANISH
    send_client_to_one(dst, "Canal no encontrado.");
#else
    send_client_to_one(dst, "Channel not found.");
#endif
    return;
    }
  
  cnt = sh_count(&c->mhead);
  if (cnt > 0)
    {
    struct _iter_pack pack = { cptr, dst, 0, 0 };
    err = sh_iter(&c->mhead, NULL, recheck_chan_iter_cb, &pack, 0);
    if (err == 0)
      {
      char ipbuf[64];
      
#ifdef SPANISH
      send_client_to_one(dst, "RECHECK: %s - %lu usuarios (%lu agregados a la cola de escaneos,"
                         " %lu en NOSCAN)", c->chname, cnt, pack.scan_cnt,
                         pack.noscan_cnt);
#else
      send_client_to_one(dst, "RECHECK: %s - %lu users (%lu added to ScanQ,"
                         " %lu in NOSCAN)", c->chname, cnt, pack.scan_cnt,
                         pack.noscan_cnt);
#endif
      
#ifdef IRC_HISPANO
      send_to_console("[%s] %s RECHECK %s", gConfig->server.id,
                      cptr->nick, c->chname);
#else
      inet_ntop(cptr->flags & CLIENT_FLAG_IPV6 ? AF_INET6 : AF_INET,
                &cptr->addr, ipbuf, sizeof(ipbuf));
      send_to_console("[%s] %s[%s] RECHECK %s", gConfig->server.id,
                      cptr->nick, ipbuf, c->chname);
#endif
      }
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
  }

static void
recheck_nick(struct Client *cptr, const char *dst, toktabptr ttab)
  {
  struct Client *target;
  PXSRemove4 rem_request;
  char ipbuf[16];
  
  if (!(target = irc_userbase_get_by_nick(ttab->tok[4])))
    {
#ifdef SPANISH
    send_client_to_one(dst, "Usuario no encontrado.");
#else
    send_client_to_one(dst, "User not found.");
#endif
    return;
    }
  
  if (scan_check_noscan(target))
    {
#ifdef SPANISH
    send_client_to_one(dst, "El usuario está marcado como NOSCAN; no se puede reescanear");
#else
    send_client_to_one(dst, "User's tagged as NOSCAN; can't recheck");
#endif
    return;
    }
  
  if (target->flags & CLIENT_FLAG_IPV6)
    return; /* not supported yet */
  
  inet_ntop(AF_INET, &target->addr, ipbuf, sizeof(ipbuf));
  
  if (target->flags & CLIENT_FLAG_SCANNING)
    {
#ifdef SPANISH
    send_client_to_one(dst, "El usuario %s (%s) está todavía en la cola de escaneos"
                       " (esperando o empezando a escanear)", target->nick, ipbuf);
#else
    send_client_to_one(dst, "User %s (%s) is already in ScanQ"
                       " (waiting or being scanned)", target->nick, ipbuf);
#endif
    return;
    }
  
  /* Passed "test"; recheck client */
  rem_request.addr = target->addr.ip4;
  if (scan_send_command(cptr, PXYSCAND_SIG, PX_CMD_REMOVE,
                        &rem_request, sizeof(rem_request)) == -1)
    {
#ifdef SPANISH
    send_client_to_one(dst, "/!\\ El demonio de Escaneo no está conectado."
                       " No se puede reescanear en este momento.");
#else
    send_client_to_one(dst, "/!\\ Scanner daemon not connected."
                       " Can't recheck at the moment.");
#endif
    return;
    }
  scan_start(target); /* start scan */
#ifdef SPANISH
  send_client_to_one(dst, "El usuario %s (%s) agregado a la cola de escaneos"
                     " (sera escaneado ASAP)", target->nick, ipbuf);
#else
  send_client_to_one(dst, "User %s (%s) added to ScanQ"
                     " (will be scanned ASAP)", target->nick, ipbuf);
#endif
  }

void
cmd_recheck(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  
  if (ttab->size < 5)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Sintaxis: RECHECK <nick|canal>");
#else
    send_client_to_one(dst, "Syntax: RECHECK <nickname|channel>");
#endif
    return;
    }
  
  if (IsChannelName(ttab->tok[4]))
    recheck_chan(cptr, dst, ttab);
  else
    recheck_nick(cptr, dst, ttab);
  }

void
cmd_recheck_reply(struct Client *cptr, PXSRemove4 *rem_reply)
  {
  /* Be quiet. */
  }

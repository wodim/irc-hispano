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
#define RCSID "$Id: cmd_grem.c,v 1.3 2006/09/10 22:00:27 spale Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"

#include "irc_network.h"
#include "irc_numnicks.h"
#include "scan.h"
#include "PXServiceMsg.h"

#include <peak/peak.h>
#include <arpa/inet.h>
#include <time.h>

static void
grem_usage(const char *dst)
  {
  send_client_to_one(dst, "Syntax: GREM <IP>");
  }

void
cmd_grem(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  const char *arg;
  PXSRemove4 rem_request;
  
  if (ttab->size < 5)
    {
    grem_usage(dst);
    return;
    }
  
  arg = ttab->tok[4];
  if ((arg[0] == '*') && (arg[1] == '@'))
    arg += 2;
  
  if (!*arg) /* "*@" ?? */
    return;
  
  if (inet_pton(AF_INET, arg, &rem_request.addr) != 1)
    {
    grem_usage(dst);
    return;
    }
  
  if (scan_send_command(cptr, PXYSCAND_SIG, PX_CMD_REMOVE,
                        &rem_request, sizeof(rem_request)) == -1)
    send_client_to_one(dst, "/!\\ Scanner daemon not connected."
                       " Can't ask it to remove IP from cache at the moment.");
  else
    cptr->flags |= CLIENT_FLAG_GREM;
  }

void
cmd_grem_reply(struct Client *cptr, PXSRemove4 *rem_reply)
  {
  char dst[6], ipbuf[16];
  int status;
  inttobase64(dst, cptr->nserv, 2);
  inttobase64(dst + 2, cptr->nnick, 3);
  dst[5] = '\0';
  
  cptr->flags &= ~CLIENT_FLAG_GREM;
  
  status = ntohl(rem_reply->status) ? 1 : 0;
  
  if (inet_ntop(AF_INET, &rem_reply->addr, ipbuf, sizeof(ipbuf)))
    {
    time_t now = time(NULL);
    if (status)
      {
      send_client_to_one(dst, "GREM: Cache REMOVE successful for IP %s "
                         "(from pxyscand)", ipbuf);
      send_client_to_one(dst, "GREM: Sending remgline to IRC network");
      }
    else
      {
      send_client_to_one(dst, "GREM: Cache REMOVE failed for IP %s "
                         "(from pxyscand). Probably not in cache anymore",
                         ipbuf);
      send_client_to_one(dst, "GREM: Sending remgline anyway");
      }
    send_raw("%s " TOK_GLINE " * -*@%s %li %li" CRLF, gMe.yy, ipbuf, (long int)now, (long int)now);
    }
  }

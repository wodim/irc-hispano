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
#define RCSID "$Id: cmd_noscan.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_send.h"
#include "irc_struct.h"
#include "cfgloader.h"

#include <peak/peak.h>
#include <arpa/inet.h>

void
cmd_noscan(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  CNoScanLink *noscan;
  char netbuf[16], maskbuf[16];
  
  send_client_to_one(dst, "\2NOSCAN LIST\2");
  
  for (noscan = gConfig->noscanlist; noscan; noscan = noscan->next)
    {
    switch (noscan->noscan.type)
      {
      case NOSCAN_TYPE_SERVER:
        send_client_to_one(dst, "Server: %s", noscan->noscan.u.server);
        break;
      case NOSCAN_TYPE_USERIP:
        if (inet_ntop(AF_INET, &noscan->noscan.u.userip.network,
                      netbuf, sizeof(netbuf))
            && inet_ntop(AF_INET, &noscan->noscan.u.userip.netmask,
                         maskbuf, sizeof(maskbuf)))
          send_client_to_one(dst, "Network: %s/%s", netbuf, maskbuf);
        break;
      default:
        break;
      }
    }
  }

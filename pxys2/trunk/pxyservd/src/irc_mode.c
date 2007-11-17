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
#define RCSID "$Id: irc_mode.c,v 1.2 2004/01/01 14:33:27 mbuna Exp $"

#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_mode.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "evreg.h"
#include "irc_network.h"
#include "irc_struct.h"
#include "irc_yxx.h"

static void operchange(int * ioOper, const char * mode);

void
irc_mode_handle(const char *numnick, const char *mode_change)
  {
  struct Client *cptr = irc_network_find_client(yxx_to_int(numnick));
  
  if (cptr)
    {
    char ipbuf[32];
    int oper = (cptr->flags & CLIENT_FLAG_OPER) ? 1 : 0;
    
    operchange(&oper, mode_change);
    if (!oper ^ !(cptr->flags & CLIENT_FLAG_OPER))
      {
      if (oper)
        {
        cptr->flags |= CLIENT_FLAG_OPER;
        if (inet_ntop(cptr->flags & CLIENT_FLAG_IPV6 ? AF_INET6 : AF_INET,
                      &cptr->addr, ipbuf, sizeof(ipbuf)))
          evreg_broadcast(EVREG_FLAG_OPER, "[EV] Oper %s!%s@%s on %s",
                          cptr->nick, cptr->user, ipbuf,
                          irc_network_get_server(cptr->nserv)->name);
        }
      else
        cptr->flags &= ~CLIENT_FLAG_OPER;
      }
    }
  }

static void
operchange(int * ioOper, const char * mode)
  {
  int tog = 1;
  int ch;
  
  while((ch = *mode++))
    switch(ch)
      {
      case '+':
        tog = 1;
        continue;
      case '-':
        tog = 0;
        continue;
#ifdef IRC_HISPANO
      case 'h':
#else
      case 'o':
#endif
        *ioOper = tog;
      default:
        break;
      }
  }

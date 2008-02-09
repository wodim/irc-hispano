/* Copyright (C) 2003, 2004 Stephane Thiell
 * Copyright (C) 2008 Toni Garcia <zoltan@irc-dev.net>
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
#include "irc_send.h"
#include "irc_struct.h"

#include "irc_channel.h" 
#include "irc_client.h" 
#include "irc_numnicks.h"
#include "irc_userbase.h"

#include <stdio.h>
#include <string.h>

void
cmd_say(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];

#ifdef IRC_HISPANO
  if (!(cptr->flags & CLIENT_FLAG_HDDVIEWER))
    {
/*    send_client_to_one(dst, "La curiosidad mató al gato ;)"); */
      send_client_to_one(dst, "Tú eres muy listo..............");
    return;
    }
#endif
  
  if (ttab->size < 6)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Sintaxis: SAY <nick|canal> <mensaje>");
#else
    send_client_to_one(dst, "Syntax: SAY <nickname|channel> <message>");
#endif
    return;
    }
  
  if (IsChannelName(ttab->tok[4]))
    {
    struct Channel *c;

    if (!(c = irc_channel_get(ttab->tok[4])))
      {
#ifdef SPANISH
      send_client_to_one(dst, "Canal no encontrado.");
#else
      send_client_to_one(dst, "Channel not found.");
#endif
      return;
      }
    send_msg_client_to_channel(c, "%s", untokenize(ttab, 5));
    }
  else
    {
    struct Client *u;
    char dst_say[6];

    if (!(u = irc_userbase_get_by_nick(ttab->tok[4])))
      {
#ifdef SPANISH
      send_client_to_one(dst, "Usuario no encontrado.");
#else
      send_client_to_one(dst, "User not found.");
#endif
      return;
      }
    inttobase64(dst_say, u->nserv, 2);
    inttobase64(dst_say + 2, u->nnick, 3);
    dst_say[5] = '\0';

    send_client_to_one(dst_say, untokenize(ttab, 5));
    }
  }

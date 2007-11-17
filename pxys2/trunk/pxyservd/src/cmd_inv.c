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
#define RCSID "$Id: cmd_inv.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"


void
cmd_inv(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  struct Channel *ch;
  
  if (!gConfig->client.channel)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Lo siento, no te puedo invitarte; mi canal de consola"
                       " esta deshabilitado.");
#else
    send_client_to_one(dst, "Sorry, can't invite you; my console channel"
                       " is disabled.");
#endif
    return;
    }
  
  ch = irc_channel_get(gConfig->client.channel);
  if (!ch)
    return;
  
  if (irc_membership_exists(ch, cptr))
    {
#ifdef SPANISH
    send_client_to_one(dst, "Hazte un WHOIS! Tu estas en %s :-)",
                       ch->chname);
#else
    send_client_to_one(dst, "Just whois yourself! You are on %s :-)",
                       ch->chname);
#endif
    return;
    }
  
  send_raw("%s%s " TOK_INVITE " %s %s" CRLF,
           gMe.yy, MYCLIENT_NUM64, cptr->nick, ch->chname);
  }

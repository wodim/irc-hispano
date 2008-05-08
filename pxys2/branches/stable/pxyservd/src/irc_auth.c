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
#define RCSID "$Id: irc_auth.c,v 1.2 2004/01/01 02:13:10 mbuna Exp $"

#include "irc_auth.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "cfgloader.h"
#include "irc_client.h"
#include "irc_msg.h"
#include "irc_send.h"

extern time_t gBirthTime;

void
irc_auth(const char *password)
  {
  assert(gMe.name != NULL); /* I have to be initialized at this point */
  
  send_raw("%s :%s" CRLF, MSG_PASS, password);
  
  /* Register as a service server (+s) */
  send_raw("%s %s %d %ld %ld J10 %s%s +s :%s" CRLF, MSG_SERVER,
           gMe.name, 1, gBirthTime, peak_time(), gMe.yy, MYCLIENT_NUM64,
           gConfig->server.info);
  
  irc_client_register();
  irc_client_burst();
  
  send_raw("%s %s" CRLF, gMe.yy, TOK_END_OF_BURST);
  }

void
irc_auth_finalize()
  {
  irc_client_unregister();
  }

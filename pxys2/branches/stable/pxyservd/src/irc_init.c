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
#define RCSID "$Id: irc_init.c,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_init.h"

#include "irc_auth.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_gline.h"
#include "irc_parser.h"
#include "irc_membership.h"
#include "irc_network.h"
#include "irc_userbase.h"
#include "debug.h"

void
irc_init()
    {
    irc_client_init();     /* once */
    irc_parser_init();     /* once */
    irc_network_init();    /* once */
    irc_gline_init();
    irc_membership_init();
    irc_userbase_init();
    irc_channel_init();
    }

void
irc_reinit()
  {
  Debug((DL_BASIC, "irc_reinit: reinitializing IRC modules..."));
  
  irc_auth_finalize();
  irc_membership_finalize();
  irc_channel_finalize();
  irc_userbase_finalize();
  irc_gline_finalize();
  
  irc_gline_init();
  irc_membership_init();
  irc_userbase_init();
  irc_channel_init();
  }

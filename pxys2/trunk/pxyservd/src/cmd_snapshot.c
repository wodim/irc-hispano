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
#define RCSID "$Id: cmd_snapshot.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"

#include <peak/peak.h>

static void
snapshot_matrix(struct Channel *channel, struct Client *client, void *uptr)
  {
  send_client_to_one((char*)uptr, "membership:(%s, %s)",
                     channel->chname, client->nick);
  }

void
cmd_snapshot(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  peak_time_date gdate;
  peak_tz tz;
  
  tz = peak_tz_create_system();
  if (tz)
    {
    gdate = peak_time_get_date(peak_time_float(), tz);
    send_client_to_one(dst, "SNAPSHOT @ %d-%d-%d %d:%02d:%02d %s",
                       gdate.year, gdate.month, gdate.day,
                       gdate.hour, gdate.minute, (int)gdate.second,
                       peak_tz_get_abbreviation(tz, peak_time()));
    peak_release(tz);
    }
  
  irc_membership_iterate(snapshot_matrix, (void*)dst);
  
  send_client_to_one(dst, "END OF SNAPSHOT");
  }

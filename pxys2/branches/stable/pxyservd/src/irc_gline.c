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
#define RCSID "$Id: irc_gline.c,v 1.2 2004/01/10 14:32:29 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef MAX_GLINEQ_ELEMS
#define MAX_GLINEQ_ELEMS 500
#endif

#define GLINE_PREFIX "AUTO "

#include "irc_gline.h"
#include "pxyservd_log.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include <peak/peak.h>
#include <arpa/inet.h>

#include "cfgloader.h"
#include "glineq.h"
#include "irc_send.h"
#include "irc_userbase.h"

/* This module contains a transparent glines rate limiter to avoid flooding
 * our hub. It uses glineq to queue the glines, and a libpeak's timer to
 * delay sendings.
 */

static void irc_gline_timer_callback(peak_timer ti, void *context);
static peak_timer gti;

void
irc_gline_init()
  {
  glineq_init(MAX_GLINEQ_ELEMS);

  gti = peak_timer_create(-1, -1, irc_gline_timer_callback, NULL);
  peak_task_timer_add(peak_task_self(), gti);
  peak_release(gti);
  }

void
irc_gline_finalize()
  {
  peak_task_timer_remove(peak_task_self(), gti);
  glineq_finalize();
  }

static void
irc_gline_timer_callback(peak_timer ti, void *context)
  {
  if (glineq_empty())
    {
    peak_timer_configure(gti, -1, -1); /* Off */
    }
  else
    {
    const struct g_args *ap = glineq_first();
    assert(ap != NULL);
    
    log_write(LOGID_GLINES, "+*@%s %ld :" GLINE_PREFIX "[%ld] %s",
              inet_ntoa(ap->addr), gConfig->gline.delay,
              ap->hitcnt, ap->reason);
    
#ifdef SPANISH
#ifdef NIKOLAS
    {
      char fake_addr64[8];
      fake_inttobase64(fake_addr64, ap->addr, 6);
      
      send_gline(ap->addr, gConfig->gline.delay, GLINE_PREFIX "[%ld] (%s) (puerto %u) %s",
                   ap->hitcnt, fake_addr64, ap->port, ap->reason);
    }
#else
    send_gline(ap->addr, gConfig->gline.delay, GLINE_PREFIX "[%ld] (puerto %u) %s",
               ap->hitcnt, ap->port, ap->reason);
#endif
#else
    send_gline(ap->addr, gConfig->gline.delay, GLINE_PREFIX "[%ld] (port %u) %s",
               ap->hitcnt, ap->port, ap->reason);
#endif
    
    glineq_pop();
    }
  }

void
irc_gline_send(const struct in_addr *addr, int hitcnt, const char *reason, uint16_t port)
  {
  struct g_args args;
  double ft;
  
  args.addr = *addr;
  args.hitcnt = hitcnt;
  args.reason = reason;
  args.port = port;
  
  ft = peak_timer_get_firetime(gti);
  
  if (glineq_push(&args) == -1)
    {
    /* Ugh !! */
    log_system("irc_gline_send: max glineQ exceeded! (delay: %fs -> %fs)",
               ft, ft / 2.0);
    peak_timer_configure(gti, 0, ft / 2.0);
    
    /* no choice but to call out */
    irc_gline_timer_callback(gti, NULL);
    glineq_push(&args);
    }
  
  if (ft > peak_time() + 10.0) /* not configured ? */
    peak_timer_configure(gti, 0, 0.5); /* Fire ASAP with 0.5s repeating */
  }

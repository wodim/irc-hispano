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
#define RCSID "$Id: session.c,v 1.4 2004/01/10 18:15:36 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "session.h"

#include <peak/peak.h>

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "cfgloader.h"
#include "debug.h"
#include "evreg.h"
#include "irc_auth.h"
#include "irc_init.h"
#include "irc_network.h" /* irc_network_squit_downlink() */
#include "irc_parser.h"
#include "irc_send.h"

#include "scan.h" /* scan_init() */

peak_stream gIRCStream; /* IRC Stream reference - global */

time_t link_timestamp;

static CHubLink *hubLink;
static peak_timer timer_recon;

#define LINK_STATUS_DISCONNECTED 0
#define LINK_STATUS_CONNECTED    1
#define LINK_STATUS_SQUITTED     2
static int link_status = LINK_STATUS_DISCONNECTED;

static void session_init(void);
static int session_try_hub(void);
static int session_open(CHubLink *hublink);

static void session_stream_event(peak_stream s, int type, void *context);
static void session_stream_error(peak_stream s, int error_type, void *context);
static void session_timer_recon(peak_timer ti, void *context);

/* Initialize session module, create reconnect timer and interruptions
 * notification object for handling signals.
 */
static void
session_init()
  {
  /* It's wise to use a reconnection timer for several reasons. We use it as a
   * starter too here (it will be called immediately when the task starts).
   */
  timer_recon = peak_timer_create(0, -1, session_timer_recon, NULL);
  peak_task_timer_add(peak_task_self(), timer_recon);
  }

static int
session_try_hub()
  {
  int i;
  
  for (i = 0; i < gConfig->nhubs; i++)
    {
    /* The first hub in the configuration file has got the highest priority
     */
    hubLink = cfgloader_next_hublink(hubLink);
    assert(hubLink != NULL);
    
    /* Try to establish connection */
    if (session_open(hubLink) == 0)
      break;
    }
  cfgloader_retain_hublink(hubLink); /* Be sure we won't dispose this hub */
  return 0;
  }

static int
session_open(CHubLink *hublink)
  {
  struct sockaddr_in sin;
  
  Debug((DL_BASIC, "session_open"));
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = hublink->hub.address;
  sin.sin_port = htons((unsigned short)hublink->hub.port);
  
  /* Create socket stream */
  gIRCStream = peak_stream_socket_create((struct sockaddr *)&sin,
                                         sizeof(sin),
                                         PEAK_STREAM_OPT_LINEMODE,
                                         session_stream_event,
                                         NULL);
  if (!gIRCStream)
    {
    Debug((DL_BASIC, "session_open: peak_stream_socket_create failed!"));
    return -1;
    }
  
  if (hublink->hub.bind_address.s_addr != INADDR_ANY)
    {
    struct sockaddr_in local_sin;
    memset(&local_sin, 0, sizeof(local_sin));
    local_sin.sin_family = AF_INET;
    local_sin.sin_addr = hublink->hub.bind_address;
    local_sin.sin_port = htons(0);
    
    peak_stream_set_address(gIRCStream, (struct sockaddr *)&local_sin,
                            sizeof(local_sin));
    }
  
  peak_stream_set_buffered(gIRCStream, 1, MSGBUF_SIZE,
                           gConfig->server.sendq,
                           session_stream_error);
  
  /* Tell peak library that a connection should be established... */
  if (peak_stream_connect(gIRCStream) != 0)
    {
    Debug((DL_BASIC, "session_open: peak_stream_connect failed!"));
    peak_release(gIRCStream);
    return -1;
    }
  
  /* Schedule connection stream with our current task */
  peak_stream_schedule(gIRCStream, peak_task_self());
  return 0;
  }

static void
session_close()
  {
  gIRCStream = NULL;
  
  irc_network_squit_downlink();
  
  assert(irc_network_get_remote_server_count() == 0);
  
  peak_timer_configure(timer_recon, 30, -1);   /* First, wait a bit. */
  peak_task_timer_add(peak_task_self(), timer_recon);
  
  irc_reinit();                                /* Then, reinit IRC modules */
  evreg_finalize();
  evreg_init();
  }

static void
session_stream_event(peak_stream s, int type, void *context)
  {
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      link_status = LINK_STATUS_CONNECTED;
      link_timestamp = peak_time();
      irc_auth(hubLink->hub.password);
      break;
    case PEAK_STREAM_EVT_READ:
      if (link_status == LINK_STATUS_CONNECTED)
        irc_parser_parse_line(peak_stream_get_line(s));
      break;
    case PEAK_STREAM_EVT_WRITE:
      /* In buffered mode, it means "write completed"...
       * Anyway we don't use it at all for now.
       */
      break;
    case PEAK_STREAM_EVT_END:
      Debug((DL_BASIC, "[EOF] end from server"));
      link_status = LINK_STATUS_DISCONNECTED;
      peak_release(s);
      session_close();
      break;
    case PEAK_STREAM_EVT_ERROR:
      Debug((DL_BASIC, "[ERR] stream error with server"));
      link_status = LINK_STATUS_DISCONNECTED;
      peak_release(s);
      session_close();
      break;
    default:
      fprintf(stderr, "type=%d\n", type);
      assert(0);
      break;
    }
  }

static void
session_stream_error(peak_stream s, int error_type, void *context)
  {
  Debug((DL_BASIC, "session_stream_error: MAX SENDQ EXCEEDED (%d)",
         error_type));
  if (link_status == LINK_STATUS_CONNECTED)
    {
    send_squit("Max SendQ Exceeded");
    link_status = LINK_STATUS_SQUITTED;
    peak_timer_configure(timer_recon, 5, -1);
    peak_task_timer_add(peak_task_self(), timer_recon);
    }
  }

static void
session_timer_recon(peak_timer ti, void *context)
  {
  Debug((DL_BASIC, "session_recon: trying to reconnect pxyservd"));
  
  if (link_status == LINK_STATUS_SQUITTED)
    {
    peak_stream_disconnect(gIRCStream);         /* Enough! */
    peak_timer_configure(timer_recon, 5, -1);   /* Wait, again. */
    return;
    }
  
  assert(link_status == LINK_STATUS_DISCONNECTED);
  
  session_try_hub();
  peak_task_timer_remove(peak_task_self(), timer_recon);
  }

/* Public methods */
void
session_run(void)
  {
  session_init(); /* Setup some things like auto reconnection timer */
  evreg_init();   /* EVREG management */
  irc_init();     /* Initialize IRC modules */
  scan_init();    /* Initialize scan stuffs and PCKP/UDP server */
  
  /* Run task event loop
   */
  Debug((DL_BASIC, "Running task..."));
  peak_task_run(peak_task_self());
  Debug((DL_BASIC, "Exiting from peak_task_run"));
  }

void
session_reload(void)
  {
  Debug((DL_BASIC, "session_cfg_reload: reloading configuration file..."));
  hubLink = NULL; /* reset hub list as it's gonna change */
  
  /* TODO */
  
#if 0
  scan_finalize();
  scan_init(session_runloop);
#endif
  }

void
session_stop(const char *message)
  {
  Debug((DL_BASIC, "session_stop (%s)", message));
  send_squit(message); /* can put in sendq but we don't care */
  peak_task_break(peak_task_self());
  }

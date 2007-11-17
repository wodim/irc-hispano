/* Copyright (C) 2003-2005 Stephane Thiell
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
#define RCSID "$Id: opas_support.c,v 1.6 2006/09/04 12:48:08 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define RETRY_DELAY   10
#define WARNING_DELAY 600

/* Module interface */
#include "opas_support.h"

/* OPAS library */
#include <opas/opas.h>

/* PEAK library for asynchronous socket support */
#include <peak/peak.h>

/* System includes */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* Other includes */
#include "cfgloader.h"
#include "debug.h"
#include "evreg.h"
#include "scan.h"
#include "irc_send.h"  /* send_to_console() */
#include "pxyservd_log.h"

static void opas_support_retry_later();
static void opas_support_timer_callback(peak_timer ti, void *context);
static void opas_support_connect();
static void opas_support_stream_cleanup(peak_stream s);
static void opas_support_stream_read(peak_stream s);
static void opas_support_stream_event(peak_stream s, int type, void *context);

static void opas_support_send_cb(const void *data, uint32_t length,
                                 void *uptr);
static void opas_support_reply_noproxy_cb(struct opas_msg_query *replyp,
                                          void *context);
static void opas_support_reply6_noproxy_cb(struct opas_msg_query6 *replyp,
                                           void *context);
static void opas_support_reply_proxy_cb(struct opas_msg_reply_proxy *replyp,
                                        void *context);
static void opas_support_reply6_proxy_cb(struct opas_msg_reply6_proxy *replyp,
                                         void *context);
static void opas_support_reply_error_cb(struct opas_msg_reply_error *replyp,
                                        void *context);
static void opas_support_reply6_error_cb(struct opas_msg_reply6_error *replyp,
                                         void *context);
static void opas_support_msg_user_cb(struct opas_msg_user_header *umsgp,
                                     void *data, void *context);

time_t opas_support_last_connection;
time_t opas_support_last_disconnection;
static time_t opas_support_last_warning;

static opas_session_t session;
static int session_open, session_timeout;
static peak_timer retry_timer;

/* Callback functions required by the OPAS library: */

static const struct opas_callbacks opas_support_callbacks =
  {
  opas_support_send_cb,
  NULL, /* we don't handle query */
  opas_support_reply_noproxy_cb,
  opas_support_reply_proxy_cb,
  opas_support_reply_error_cb,
  NULL, /* we don't handle query6 */
  opas_support_reply6_noproxy_cb,
  opas_support_reply6_proxy_cb,
  opas_support_reply6_error_cb,
  opas_support_msg_user_cb
  };

int
opas_support_init()
  {
  opas_support_last_connection = opas_support_last_disconnection = 0;
  
  retry_timer = peak_timer_create(-1, -1, opas_support_timer_callback, NULL);
  if (retry_timer == NULL)
    return -1;
  
  session = opas_open_session(&opas_support_callbacks, NULL);
  if (session == NULL)
    return -1;
  
  /* Try to connect immediately... why wait? */
  opas_support_connect();
  return 0; /* OK */
  }

void
opas_support_finalize()
  {
  peak_stream s;
  
  peak_task_timer_remove(peak_task_self(), retry_timer);
  peak_release(retry_timer);
  
  if ((s = (peak_stream)opas_get_context(session)))
    {
    peak_release(s);
    s = NULL;
    }
  
  opas_close_session(session);
  }

int
opas_support_is_ready()
  {
  return session_open;
  }

int
opas_support_query(const struct in_addr *addr, uint32_t user_data)
  {
  if (!session_open)
    return -1;
  
  opas_query(session, addr, user_data);
  return 0; /* OK */
  }

int
opas_support_query6(const struct in6_addr *addr, uint32_t user_data)
  {
  if (!session_open)
    return -1;
  
  opas_query6(session, addr, user_data);
  return 0; /* OK */
  }

void
opas_support_send_msg_user(void *data, size_t length, uint32_t user_data)
  {
  opas_send_msg_user(session, data, length, 0, user_data);
  }

static void
opas_support_retry_later()
  {
  Debug((DL_BASIC, "opas_support_retry_later"));
  
  peak_timer_configure(retry_timer, RETRY_DELAY, RETRY_DELAY);
  peak_task_timer_add(peak_task_self(), retry_timer);
  }

static void
opas_support_timer_callback(peak_timer ti, void *context)
  {
  if (peak_time() - opas_support_last_warning >= WARNING_DELAY)
    {
    opas_support_last_warning = peak_time();
#ifdef SPANISH
    send_to_console("[%s] [PELIGRO!] No esta conectado al demonio de Escaneo!",
#else
    send_to_console("[%s] [WARNING] Not connected to the proxy scanner!",
#endif
                    gConfig->server.id);
    }
  
  opas_support_connect();
  }

static void
opas_support_connect()
  {
  struct sockaddr_in sin;
  peak_stream s;
  
  Debug((DL_BASIC, "opas_support_connect"));
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = gConfig->scanner.address;
  sin.sin_port = htons((uint16_t)gConfig->scanner.port);
  
  s = peak_stream_socket_create((struct sockaddr *)&sin,
                                sizeof(sin),
                                PEAK_STREAM_OPT_DEFAULT,
                                opas_support_stream_event,
                                session);
  if (!s)
    {
    Debug((DL_BASIC, "opas_support_connect: "
           "peak_stream_socket_create failed!"));
    return;
    }
  
  /* TODO: max sendq handler */
  peak_stream_set_buffered(s, 1, 0, 0x1000000, NULL);
  
  peak_stream_set_timeout(s, 60);
  session_timeout = 0;
  
  if (peak_stream_connect(s) != 0)
    {
    Debug((DL_BASIC, "opas_support_connect: peak_stream_connect failed!"));
    /* Will retry later */
    opas_support_stream_cleanup(s);
    return;
    }
  
  opas_set_context(session, s);
  
  peak_task_timer_remove(peak_task_self(), retry_timer);
  
  /* Schedule stream socket */
  peak_stream_schedule(s, peak_task_self());
  }

static void
opas_support_stream_cleanup(peak_stream s)
  {
  peak_release(s);
  opas_set_context(session, NULL);
  if (session_open)
    {
    opas_support_last_disconnection = peak_time();
    opas_support_last_warning = opas_support_last_disconnection;
    session_open = 0;
#ifdef SPANISH
    send_to_console("[%s] Conexion con el demonio de Escaneo cerrada.",
                   gConfig->server.id);
    evreg_broadcast(EVREG_FLAG_PXYSCAND,
                    "[EV] Conexion con el demonio de Escaneo cerrada.");
#else
    send_to_console("[%s] Connection with pxyscand closed.",
                    gConfig->server.id);
    evreg_broadcast(EVREG_FLAG_PXYSCAND,
                    "[EV] Connection with pxyscand closed.");
#endif
    }
  opas_support_retry_later();
  }

static void
opas_support_stream_read(peak_stream s)
  {
  char buffer[4096];
  int result;
  
  if (session_timeout)
    session_timeout = 0;
  
  result = peak_stream_read(s, buffer, sizeof(buffer));
  if (opas_feed(session, buffer, result) == -1)
    {
    Debug((DL_BASIC, "opas_support_stream_read: opas_feed failed!"));
    log_system("opas_support_stream_read: opas_feed failed!");
    opas_support_stream_cleanup(s);
    }
  }

static void
opas_support_stream_event(peak_stream s, int type, void *context)
  {
  switch (type)
    {
    case PEAK_STREAM_EVT_READ:
      opas_support_stream_read(s);
      break;
    case PEAK_STREAM_EVT_OPEN:
      session_open = 1;
      opas_support_last_connection = peak_time();
      if (!opas_support_last_disconnection)
        {
#ifdef SPANISH
        send_to_console("[%s] Conexion con el demonio de Escaneo establecida.",
                        gConfig->server.id);
        evreg_broadcast(EVREG_FLAG_PXYSCAND,
                        "[EV] Conexion con el demonio de Escaneo establecida.");
#else
        send_to_console("[%s] Connection with pxyscand established.",
                        gConfig->server.id);
        evreg_broadcast(EVREG_FLAG_PXYSCAND,
                        "[EV] Connection with pxyscand established.");
#endif
        }
      else
        {
        time_t t =
          opas_support_last_connection - opas_support_last_disconnection;
#ifdef SPANISH
        send_to_console("[%s] Conexion con el demonio de Escaneo establecida"
               " (downtime: %d:%02d:%02d).", gConfig->server.id,
                        (t / 3600) % 24, (t / 60) % 60, t % 60);
        evreg_broadcast(EVREG_FLAG_PXYSCAND,
                        "[EV] Conexion con el demonio de Escaneo establecida"
                        " (downtime: %d:%02d:%02d).",
                        (t / 3600) % 24, (t / 60) % 60, t % 60);
#else
        send_to_console("[%s] Connection with pxyscand established"
                        " (downtime: %d:%02d:%02d).", gConfig->server.id,
                        (t / 3600) % 24, (t / 60) % 60, t % 60);
        evreg_broadcast(EVREG_FLAG_PXYSCAND,
                        "[EV] Connection with pxyscand established"
                        " (downtime: %d:%02d:%02d).",
                        (t / 3600) % 24, (t / 60) % 60, t % 60);
#endif
        }
      break;
    case PEAK_STREAM_EVT_END:
      Debug((DL_BASIC, "opas_support_stream_event: EOF"));
      opas_support_stream_cleanup(s);
      break;
    case PEAK_STREAM_EVT_ERROR:
      Debug((DL_BASIC, "opas_support_stream_event: ERROR (error=%d)",
             peak_stream_get_error(s)));
      opas_support_stream_cleanup(s);
      break;
    case PEAK_STREAM_EVT_TIMEDOUT:
      if (!session_timeout)
        {
        session_timeout = 1;
        opas_ping(session);
        break;
        }
#ifdef SPANISH
      send_to_console("[%s] No hay respuesta desde el demonio de Escaneo, cerrando la conexion.",
                      gConfig->server.id);
#else
      send_to_console("[%s] No response from pxyscand, closing connection.",
                      gConfig->server.id);
#endif
      opas_support_stream_cleanup(s);
      break;
    default:
      break;
    }
  }

static void
opas_support_send_cb(const void *data, uint32_t length, void *uptr)
  {
  peak_stream_write_buffered((peak_stream)uptr, data, length);
  }

static void
opas_support_reply_noproxy_cb(struct opas_msg_query *replyp, void *context)
  {
  scan_reply_noproxy(&replyp->addr, replyp->user_data,
                     replyp->head & OPAS_FLAG_CACH);
  }

static void
opas_support_reply6_noproxy_cb(struct opas_msg_query6 *replyp, void *context)
  {
  scan_reply6_noproxy(&replyp->addr, replyp->user_data,
                      replyp->head & OPAS_FLAG_CACH);
  }

static void
opas_support_reply_proxy_cb(struct opas_msg_reply_proxy *replyp, void *context)
  {
  scan_reply_proxy(&replyp->addr, replyp->user_data,
                   replyp->head & OPAS_FLAG_CACH,
                   replyp->proxy_type,
                   replyp->proxy_port,
                   replyp->proxy_descr);
  }

static void
opas_support_reply6_proxy_cb(struct opas_msg_reply6_proxy *replyp,
                             void *context)
  {
  scan_reply6_proxy(&replyp->addr, replyp->user_data,
                    replyp->head & OPAS_FLAG_CACH,
                    replyp->proxy_type,
                    replyp->proxy_port,
                    replyp->proxy_descr);
  }

static void
opas_support_reply_error_cb(struct opas_msg_reply_error *replyp,
                            void *context)
  {
  scan_reply_error(&replyp->addr, replyp->user_data, replyp->error);
  }

static void
opas_support_reply6_error_cb(struct opas_msg_reply6_error *replyp,
                             void *context)
  {
  scan_reply6_error(&replyp->addr, replyp->user_data, replyp->error);
  }

static void
opas_support_msg_user_cb(struct opas_msg_user_header *umsgp, void *data,
                         void *context)
  {
  if (umsgp->head & OPAS_FLAG_REPL)
    scan_reply_command(data, umsgp->data_length, umsgp->user_data);
  }


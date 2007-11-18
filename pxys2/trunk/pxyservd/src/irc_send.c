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
#define RCSID "$Id: irc_send.c,v 1.3 2006/09/10 22:00:27 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_send.h"
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include "cfgloader.h"
#include "debug.h"
#include "irc_channel.h"
#include "irc_client.h"  /* MYCLIENT_NUM64 */
#include "irc_msg.h"     /* IRCU messages definitions */
#include "irc_network.h" /* "gMe" */
#include "session.h"

static char myCli64[] = MYCLIENT_NUM64;

void
send_raw(const char *fmt, ...)
  {
  va_list vl;
    
  va_start(vl, fmt);
  if (gIRCStream)
    peak_stream_msgbuf_vmake(gIRCStream, fmt, vl);
  va_end(vl);
  }

void
send_gline(struct in_addr addr, time_t delay, const char *fmt, ...)
  {
  const char *res;
  va_list vl;
  char ipbuf[16] = { 0 };
  char msgbuf[MSGBUF_SIZE];
#if 0 /* P10 IRC-Hispano */
  time_t now = time(NULL);
#endif
  
  res = inet_ntop(AF_INET, &addr, ipbuf, sizeof(ipbuf));
  assert(res != NULL);
  
  if (strlen(ipbuf) < 7)
    {
    Debug((DL_BASIC, "send_gline: bad IP number '%s'", ipbuf));
    return;
    }
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  
#if 1 /* P10 IRC-Hispano */
  send_raw("%s " TOK_GLINE " * +*@%s %ld :%s" CRLF,
           gMe.yy, ipbuf, delay, msgbuf);
#else
  send_raw("%s " TOK_GLINE " * +*@%s %ld %li :%s" CRLF,
           gMe.yy, ipbuf, delay, now, msgbuf);
#endif
  }

void
send_squit(const char *msg)
  {
  Debug((DL_BASIC, "send_squit: \"%s\"", msg));
  
  if (gIRCStream)
    peak_stream_write_format(gIRCStream, "%s " TOK_SQUIT " %s :%s" CRLF,
                             gMe.yy, gMe.name, msg);
  }

void
send_to_console(const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!gIRCStream)
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
#ifdef IRC_HISPANO
    peak_stream_msgbuf_make(gIRCStream, "%s " TOK_PRIVATE " %s :%s" CRLF,
#else
    peak_stream_msgbuf_make(gIRCStream, "%s " TOK_NOTICE " %s :%s" CRLF,
#endif
                            gMe.yy, gConfig->client.channel, msgbuf);
  }

void
send_to_one(const char *dst, const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  
  if (gIRCStream)
    peak_stream_msgbuf_make(gIRCStream, "%s " TOK_NOTICE " %s :%s" CRLF,
                            gMe.yy, dst, msgbuf);
  }

void
send_client_to_one(const char *dst, const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
#ifdef IRC_HISPANO
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_PRIVATE " %s :%s" CRLF,
#else
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_NOTICE " %s :%s" CRLF,
#endif
                            gMe.yy, myCli64, dst, msgbuf);
  }

void
send_client_to_channel(struct Channel *channel, const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  if (!channel)
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_NOTICE " %s :%s" CRLF,
                            gMe.yy, myCli64, channel->chname, msgbuf);
  }

void
send_client_to_console(const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_NOTICE " %s :%s" CRLF,
                            gMe.yy, myCli64, gConfig->client.channel, msgbuf);
  }

void
send_msg_client_to_channel(struct Channel *channel, const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  if (!channel)
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_PRIVATE " %s :%s" CRLF,
                            gMe.yy, myCli64, channel->chname, msgbuf);
  }

void
send_msg_client_to_console(const char *fmt, ...)
  {
  va_list vl;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  if (gIRCStream)
    peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_PRIVATE " %s :%s" CRLF,
                            gMe.yy, myCli64, gConfig->client.channel, msgbuf);
  }

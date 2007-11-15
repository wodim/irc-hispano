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
#ifndef INCLUDED_SEND_H_
#define INCLUDED_SEND_H_

#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define CRLF "\r\n"
#define MSGBUF_SIZE 512

struct Channel;

extern void send_queued(void);
extern void send_raw(const char *fmt, ...);
extern void send_gline(struct in_addr addr, time_t delay, const char *fmt,...);
extern void send_squit(const char *msg);
extern void send_to_console(const char *fmt, ...);
extern void send_to_one(const char *dst, const char *fmt, ...);
extern void send_client_to_one(const char *dst, const char *fmt, ...);
extern void send_client_to_channel(struct Channel *channel,
                                   const char *fmt, ...);
extern void send_client_to_console(const char *fmt, ...);
extern void send_msg_client_to_console(const char *fmt, ...);

#endif /* INCLUDED_SEND_H_ */

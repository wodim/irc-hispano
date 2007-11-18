/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of libopas (from pxys)
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
 * $Id: opas.h,v 1.2 2006/09/06 10:11:32 spale Exp $
 */
#ifndef INCLUDED_OPAS_OPAS_H
#define INCLUDED_OPAS_OPAS_H

#include <opas/opas_msg.h>
#include <opas/opas_stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* OPAS proxy types ...
 */
enum opas_proxy_type_t
  {
  OPAS_TYPE_UNKNOWN = 0,
  OPAS_PROXY_TYPE_WINGATE = 1,
  OPAS_PROXY_TYPE_SOCKS4 = 2,
  OPAS_PROXY_TYPE_SOCKS5 = 3,
  OPAS_PROXY_TYPE_HTTPPROXY = 4,
  OPAS_PROXY_TYPE_HTTPPOST = 5,
  OPAS_PROXY_TYPE_CISCOROUTER = 6,
  OPAS_PROXY_TYPE_IRCBOUNCER = 7,
  OPAS_PROXY_TYPE_HTTPGET = 8,
  OPAS_PROXY_TYPE_3COM812 = 9
  };

enum
  {
  OPAS_FLAG_ERRO = 1 << 8,
  OPAS_FLAG_CACH = 1 << 9,
  OPAS_FLAG_PROX = 1 << 10,
  OPAS_FLAG_REPL = 1 << 11,
  OPAS_FLAG_UMSG = 1 << 12,
  OPAS_FLAG_IPV6 = 1 << 13,
  OPAS_FLAG_PING = 1 << 14,
  OPAS_FLAG_SERV = 1 << 15
  };

enum
  {
  OPAS_ERROR_UNKNOWN = 0,
  OPAS_ERROR_NETDOWN = 1,
  OPAS_ERROR_NETUNREACH = 2
  };

typedef struct opas_session_s * opas_session_t;

typedef void (*opas_send_func)(const void *data, uint32_t length, void *uptr);

typedef void (*opas_query_callback)(struct opas_msg_query *queryp,
                                    void *context);
typedef void (*opas_query6_callback)(struct opas_msg_query6 *queryp,
                                     void *context);
typedef void (*opas_reply_noproxy_callback)(struct opas_msg_query *replyp,
                                            void *context);
typedef void (*opas_reply6_noproxy_callback)(struct opas_msg_query6 *replyp,
                                             void *context);
typedef void (*opas_reply_error_callback)(struct opas_msg_reply_error *replyp,
                                          void *context);
typedef void (*opas_reply6_error_callback)(struct opas_msg_reply6_error *rp,
                                           void *context);
typedef void (*opas_reply_proxy_callback)(struct opas_msg_reply_proxy *replyp,
                                          void *context);
typedef void (*opas_reply6_proxy_callback)(struct opas_msg_reply6_proxy *rp,
                                           void *context);
typedef void (*opas_msg_user_callback)(struct opas_msg_user_header *umsgp,
                                       void *data,
                                       void *context);

struct opas_callbacks
  {
  opas_send_func send_fun;
  opas_query_callback query_fun;
  opas_reply_noproxy_callback reply_noproxy_fun;
  opas_reply_proxy_callback reply_proxy_fun;
  opas_reply_error_callback reply_error_fun;
  opas_query6_callback query6_fun;
  opas_reply6_noproxy_callback reply6_noproxy_fun;
  opas_reply6_proxy_callback reply6_proxy_fun;
  opas_reply6_error_callback reply6_error_fun;
  opas_msg_user_callback msg_user_fun;
  };

#if defined(__cplusplus)
extern "C" {
#endif

extern const char * opas_get_version(void);

extern opas_session_t opas_open_session(const struct opas_callbacks *callbacks,
                                        void *context);

extern void opas_close_session(opas_session_t session);

extern void * opas_get_context(opas_session_t session);

extern void opas_set_context(opas_session_t session, void *context);

extern int opas_feed(opas_session_t session, void *data, uint32_t size);


extern void opas_ping(opas_session_t session);

extern void opas_query(opas_session_t session, const struct in_addr *addr,
                       uint32_t user_data);

extern void opas_query6(opas_session_t session, const struct in6_addr *addr,
                        uint32_t user_data);

extern void opas_reply_noproxy(opas_session_t session,
                               struct opas_msg_query *queryp, int from_cache);

extern void opas_reply6_noproxy(opas_session_t session,
                                struct opas_msg_query6 *queryp,
                                int from_cache);

extern void opas_reply_proxy(opas_session_t session,
                             struct opas_msg_query *queryp, int from_cache,
                             time_t ts, uint16_t type, uint16_t port,
                             const char *descr);

extern void opas_reply6_proxy(opas_session_t session,
                              struct opas_msg_query6 *queryp, int from_cache,
                              time_t ts, uint16_t type, uint16_t port,
                              const char *descr);

extern void opas_reply_error(opas_session_t session,
                             struct opas_msg_query *queryp, uint32_t error);

extern void opas_reply6_error(opas_session_t session,
                              struct opas_msg_query6 *queryp, uint32_t error);

extern void opas_send_msg_user(opas_session_t session, const void *data,
                               uint32_t data_length, int reply,
                               uint32_t user_data);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_OPAS_OPAS_H */

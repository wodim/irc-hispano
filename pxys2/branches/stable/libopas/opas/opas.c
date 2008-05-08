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
 */
#define RCSID "$Id: opas.c,v 1.1.1.2 2004/01/15 14:18:23 mbuna Exp $"

#include "opas.h"

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal includes */
#include "opas_msg.h"

#define OPAS_VERSION (((OPAS_MAJOR) << 24) + ((OPAS_MINOR) << 16))

#define MSG_USER_MAXSMALL (1024 - 12)

#define OPAS_HEADER_MAKE(serv, ping, ipv6, umsg, repl, prox, cach, erro) \
    (OPAS_VERSION + ((serv) << 15) + ((ping) << 14) + ((ipv6) << 13) \
                  + ((umsg) << 12) + ((repl) << 11) + ((prox) << 10) \
                  + ((cach) << 9)  + ((erro) << 8))

#define OPAS_HEADER_SETSIZE(h, siz) do { \
     h = (h & ~0xff) + (unsigned char)(siz); } while (0)

/* After ntohl conversion:
 */
#define OPAS_GET_VERSION(h) ((h) & 0xffff0000)
#define OPAS_GET_MSGSIZE(h) (4 + ((h) & 0xff))
#define OPAS_OK_VERSION(h) (((h) & 0xff000000) == ((OPAS_MAJOR) << 24) \
                            && (((h) & 0x00ff0000) >= ((OPAS_MINOR) << 16)))

/* Conversion macros
 */
#define OPAS_QUERY_NTOH(qp) do { \
    (qp)->head = ntohl((qp)->head); } while (0)

#define OPAS_QUERY_HTON(qp) do { \
    (qp)->head = htonl((qp)->head); } while (0)
   
#define OPAS_REPLY_ERROR_NTOH(re) do { \
    (re)->head = ntohl((re)->head); \
    (re)->error = ntohl((re)->error); } while (0)

#define OPAS_REPLY_ERROR_HTON(re) do { \
    (re)->head = htonl((re)->head); \
    (re)->error = htonl((re)->error); } while (0)

#define OPAS_REPLY_PROXY_NTOH(rp) do { \
    (rp)->head = ntohl((rp)->head); \
    (rp)->timestamp = ntohl((rp)->timestamp); \
    (rp)->proxy_type = ntohs((rp)->proxy_type); \
    (rp)->proxy_port = ntohs((rp)->proxy_port); } while (0)

#define OPAS_REPLY_PROXY_HTON(rp) do { \
    (rp)->head = htonl((rp)->head); \
    (rp)->timestamp = htonl((rp)->timestamp); \
    (rp)->proxy_type = htons((rp)->proxy_type); \
    (rp)->proxy_port = htons((rp)->proxy_port); } while (0)

#define OPAS_MSG_USER_NTOH(mu) do { \
    (mu)->head = ntohl((mu)->head); \
    (mu)->data_length = ntohl((mu)->data_length); } while (0)

#define OPAS_MSG_USER_HTON(mu) do { \
    (mu)->head = htonl((mu)->head); \
    (mu)->data_length = htonl((mu)->data_length); } while (0)

struct opas_session_s
  {
  struct opas_callbacks cbs;
  void *context;
  void *ptr;
  int size;
  int end;
  int minisize;
  char minibuf[OPAS_MSG_MINSIZE];
  };

#ifndef OPAS_VERSION_STRING
#define OPAS_VERSION_STRING "<Unknown>"
#endif

const char *
opas_get_version()
  {
  return OPAS_VERSION_STRING;
  }

opas_session_t
opas_open_session(const struct opas_callbacks *callbacks, void *context)
  {
  opas_session_t session =
    (opas_session_t)malloc(sizeof(struct opas_session_s));
  
  if (session)
    {
    session->cbs = *callbacks;
    session->context = context;
    session->ptr = NULL;
    session->size = 0;
    session->end = 0;
    session->minisize = 0;
    session->minibuf[0] = '\0';
    }
  
  return session;
  }

void
opas_close_session(opas_session_t session)
  {
  if (session->ptr)
    free(session->ptr);
  free(session);
  }

void *
opas_get_context(opas_session_t session)
  {
  return session->context;
  }

void
opas_set_context(opas_session_t session, void *context)
  {
  session->context = context;
  }

static int
__opas_callout(opas_session_t session, uint32_t head, uint32_t msgsize,
               void *data)
  {
  if (!(head & OPAS_FLAG_IPV6))
    {
    if (head & OPAS_FLAG_REPL)
      {
      if (head & OPAS_FLAG_ERRO)
        {
        if (msgsize == sizeof(struct opas_msg_reply_error))
          {
          struct opas_msg_reply_error *re = (struct opas_msg_reply_error*)data;
          OPAS_REPLY_ERROR_NTOH(re);
          if (session->cbs.reply_error_fun)
            (*session->cbs.reply_error_fun)(re, session->context);
          return 0;
          }
        }
      else if (head & OPAS_FLAG_PROX)
        {
        if (msgsize >= sizeof(struct opas_msg_reply_proxy))
          {
          struct opas_msg_reply_proxy *rp = (struct opas_msg_reply_proxy*)data;
          OPAS_REPLY_PROXY_NTOH(rp);
          if (session->cbs.reply_proxy_fun)
            (*session->cbs.reply_proxy_fun)(rp, session->context);
          return 0;
          }
        }
      else /* not an error nor a proxy... cool ! */
        {
        if (msgsize == sizeof(struct opas_msg_query))
          {
          OPAS_QUERY_NTOH((struct opas_msg_query *)data);
          if (session->cbs.reply_noproxy_fun)
            (*session->cbs.reply_noproxy_fun)((struct opas_msg_query *)data,
                                              session->context);
          return 0;
          }
        }
      }
    else /* OPAS_FLAG_REPL */
      {
      if (msgsize == sizeof(struct opas_msg_query))
        {
        OPAS_QUERY_NTOH((struct opas_msg_query *)data);
        if (session->cbs.query_fun)
          (*session->cbs.query_fun)((struct opas_msg_query *)data,
                                    session->context);
        return 0;
        }
      }
    }
  else
    {
    if (head & OPAS_FLAG_REPL)
      {
      if (head & OPAS_FLAG_ERRO)
        {
        if (msgsize == sizeof(struct opas_msg_reply6_error))
          {
          struct opas_msg_reply6_error *re
            = (struct opas_msg_reply6_error*)data;
          OPAS_REPLY_ERROR_NTOH(re);
          if (session->cbs.reply6_error_fun)
            (*session->cbs.reply6_error_fun)(re, session->context);
          return 0;
          }
        }
      else if (head & OPAS_FLAG_PROX)
        {
        if (msgsize >= sizeof(struct opas_msg_reply6_proxy))
          {
          OPAS_REPLY_PROXY_NTOH((struct opas_msg_reply6_proxy *)data);
          if (session->cbs.reply6_proxy_fun)
            (*session->cbs.reply6_proxy_fun)(
              (struct opas_msg_reply6_proxy *)data,
              session->context);
          return 0;
          }
        }
      else /* OPAS_FLAG_ERRO, OPAS_FLAG_PROX */
        {
        if (msgsize == sizeof(struct opas_msg_query6))
          {
          OPAS_QUERY_NTOH((struct opas_msg_query6 *)data);
          if (session->cbs.reply6_noproxy_fun)
            (*session->cbs.reply6_noproxy_fun)((struct opas_msg_query6 *)data,
                                               session->context);
          return 0;
          }
        }
      }
    else /* OPAS_FLAG_REPL */
      {
      if (msgsize == sizeof(struct opas_msg_query6))
        {
        OPAS_QUERY_NTOH((struct opas_msg_query6 *)data);
        if (session->cbs.query6_fun)
          (*session->cbs.query6_fun)((struct opas_msg_query6 *)data,
                                     session->context);
        return 0;
        }
      }
    }
  return -1;
  }

static void
__opas_pong(opas_session_t session)
  {
  uint32_t head = htonl(OPAS_HEADER_MAKE(0,1,0,0,1,0,0,0));
  (*session->cbs.send_fun)(&head, sizeof(head), session->context);
  }

#define FEED_ERROR do { fprintf(stderr, "%s:%d feed error\n", \
                                __FILE__, __LINE__); \
                        result = -1; goto feed_error; } while (0)

#define VERIFY_HEADER(h) if (!OPAS_OK_VERSION(h)) FEED_ERROR

int
opas_feed(opas_session_t session, void *data, uint32_t size)
  {
  int result = 0;
  uint32_t head, msgsize, umlen;
  char *d = (char *)data;
  char *dsave = NULL;
  
  if (session->minisize > 0)
    {
    if (session->minisize + size < OPAS_MSG_MINSIZE)
      {
      memcpy(session->minibuf + session->minisize, data, size);
      session->minisize += size;
      return 0;
      }
    else
      {
      d = dsave = malloc(session->minisize + size);
      if (!d)
        return -1;
      memcpy(d, session->minibuf, session->minisize);
      memcpy((char *)d + session->minisize, data, size);
      size += session->minisize;
      session->minisize = 0;
      }
    }

  while (size > 0)
    {
    if (session->end == 0)
      {
      if (size < OPAS_MSG_MINSIZE)
        {
        if (size >= sizeof(uint32_t))
          {
          head = ntohl(*((uint32_t *)d));
          VERIFY_HEADER(head);
          if (head & OPAS_FLAG_PING)
            {
            if (!(head & OPAS_FLAG_REPL))
              __opas_pong(session);
            size -= sizeof(uint32_t);
            d += sizeof(uint32_t);
            continue;
            }
          }
        memcpy(session->minibuf, d, size);
        session->minisize = size;
        size = 0;
        }
      else
        {
        head = ntohl(*((uint32_t *)d));
        VERIFY_HEADER(head);
        if (head & OPAS_FLAG_PING)
          {
          if (!(head & OPAS_FLAG_REPL))
            __opas_pong(session);
          size -= sizeof(uint32_t);
          d += sizeof(uint32_t);
          continue;
          }
        msgsize = OPAS_GET_MSGSIZE(head);
        if (!(head & OPAS_FLAG_UMSG))
          {
          if (msgsize <= size)
            {
            if (__opas_callout(session, head, msgsize, d) == -1)
              FEED_ERROR;
            size -= msgsize;
            d += msgsize;
            }
          else
            {
            assert(msgsize <= OPAS_MSG_MAXSIZE);
            if (session->size < OPAS_MSG_MAXSIZE)
              {
              if (session->ptr)
                free(session->ptr);
              session->ptr = malloc(OPAS_MSG_MAXSIZE);
              if (!session->ptr)
                return -1;
              session->size = OPAS_MSG_MAXSIZE;
              }
            memcpy(session->ptr, d, size);
            session->end = size;
            size = 0;
            }
          }
        else
          {
        /* User msg ! */
          if (msgsize != sizeof(struct opas_msg_user_header))
            FEED_ERROR;
          
          umlen = ntohl(*((uint32_t *)((char *)d + 8)));
          if (umlen > OPAS_UMSG_MAXSIZE)
            FEED_ERROR;
          
          if (msgsize + umlen <= size)
            {
            /* We are lucky */
            OPAS_MSG_USER_NTOH((struct opas_msg_user_header *)d);
            if (session->cbs.msg_user_fun)
              (*session->cbs.msg_user_fun)((struct opas_msg_user_header *)d,
                                           (void *)((char *)d + 12),
                                           session->context);
            size -= msgsize + umlen;
            d += msgsize + umlen;
            }
          else
            {
            if (session->size < msgsize + umlen)
              {
              if (session->ptr)
                free(session->ptr);
              session->ptr = malloc(msgsize + umlen);
              if (!session->ptr)
                return -1;
              session->size = msgsize + umlen;
              }
            memcpy(session->ptr, d, size);
            session->end = size;
            size = 0;
            }
          }
        }
      }
    else    /* if (session->end == 0) */
      {
      head = ntohl(*((uint32_t *)session->ptr));
      VERIFY_HEADER(head);
      msgsize = OPAS_GET_MSGSIZE(head);
      umlen = ntohl(*((uint32_t *)((char *)session->ptr + 8)));
      
      if (!(head & OPAS_FLAG_UMSG))
        {
        if (msgsize - session->end <= size)
          {
          /* full message */
          memcpy((char *)session->ptr + session->end, d,
                 msgsize - session->end);
          
          if (__opas_callout(session, head, msgsize, session->ptr) == -1)
            FEED_ERROR;
          size -= msgsize - session->end;
          d += msgsize - session->end;
          session->end = 0; /* flushed */
          }
        else
          {
          /* still partial */
          memcpy((char *)session->ptr + session->end, d, size);
          session->end += size;
          size = 0;
          }
        }
      else
        {
        /* User msg ! */
        if (umlen - session->end <= size)
          {
          memcpy((char *)session->ptr + session->end, d, umlen - session->end);
          
          OPAS_MSG_USER_NTOH((struct opas_msg_user_header *)session->ptr);
          if (session->cbs.msg_user_fun)
            (*session->cbs.msg_user_fun)(
              (struct opas_msg_user_header *)session->ptr,
              (void *)((char *)session->ptr + 12),
              session->context);
          size -= umlen - session->end;
          d += umlen - session->end;
          session->end = 0; /* flushed */
          }
        else
          {
          memcpy((char *)session->ptr + session->end, d, size);
          session->end += size;
          size = 0;
          }
        }
      }
    } /* while */
  
feed_error:
  if (dsave)
    free(dsave);
  return result;
  }

void
opas_ping(opas_session_t session)
  {
  uint32_t head = htonl(OPAS_HEADER_MAKE(0,1,0,0,0,0,0,0));
  (*session->cbs.send_fun)(&head, sizeof(head), session->context);
  }

void
opas_query(opas_session_t session, const struct in_addr *addr,
           uint32_t user_data)
  {
  struct opas_msg_query msg;
  
  msg.head = OPAS_HEADER_MAKE(0,0,0,0,0,0,0,0);
  msg.user_data = user_data;
  msg.addr = *addr;
  OPAS_HEADER_SETSIZE(msg.head, 8);
  
  OPAS_QUERY_HTON(&msg);
  (*session->cbs.send_fun)(&msg, sizeof(msg), session->context);
  }

void
opas_query6(opas_session_t session, const struct in6_addr *addr,
            uint32_t user_data)
  {
  struct opas_msg_query6 msg;
  
  msg.head = OPAS_HEADER_MAKE(0,0,1,0,0,0,0,0);
  msg.user_data = user_data;
  msg.addr = *addr;
  OPAS_HEADER_SETSIZE(msg.head, 20);
  
  OPAS_QUERY_HTON(&msg);
  (*session->cbs.send_fun)(&msg, sizeof(msg), session->context);
  }

void
opas_reply_noproxy(opas_session_t session, struct opas_msg_query *queryp,
                   int from_cache)
  {
  queryp->head |= OPAS_FLAG_REPL; /* Set reply bit */
  if (from_cache)
    queryp->head |= OPAS_FLAG_CACH;
  OPAS_QUERY_HTON(queryp);
  (*session->cbs.send_fun)(queryp, sizeof(*queryp), session->context);
  }

void
opas_reply6_noproxy(opas_session_t session, struct opas_msg_query6 *queryp,
                    int from_cache)
  {
  queryp->head |= OPAS_FLAG_REPL; /* Reply bit */
  if (from_cache)
    queryp->head |= OPAS_FLAG_CACH;
  OPAS_QUERY_HTON(queryp);
  (*session->cbs.send_fun)(queryp, sizeof(*queryp), session->context);
  }

void
opas_reply_proxy(opas_session_t session, struct opas_msg_query *queryp,
                 int from_cache, time_t ts, uint16_t type, uint16_t port,
                 const char *descr)
  {
  struct opas_msg_reply_proxy *replyp;
  size_t len = sizeof(struct opas_msg_reply_proxy) + strlen(descr);
  
#ifdef HAVE_ALLOCA
  replyp = (struct opas_msg_reply_proxy *)alloca(len);
#else
  replyp = (struct opas_msg_reply_proxy *)malloc(len);
#endif
  
  replyp->head = OPAS_HEADER_MAKE(0,0,0,0,1,1,from_cache,0);
  replyp->user_data = queryp->user_data;
  replyp->addr = queryp->addr;
  replyp->timestamp = ts;
  replyp->proxy_type = type;
  replyp->proxy_port = port;
  strcpy(replyp->proxy_descr, descr);
  OPAS_HEADER_SETSIZE(replyp->head, len - 4);
  OPAS_REPLY_PROXY_HTON(replyp);
  (*session->cbs.send_fun)(replyp, len, session->context);
  
#ifndef HAVE_ALLOCA
  free(replyp);
#endif
  }

void
opas_reply6_proxy(opas_session_t session, struct opas_msg_query6 *queryp,
                  int from_cache, time_t ts, uint16_t type, uint16_t port,
                  const char *descr)
  {
  struct opas_msg_reply6_proxy *replyp;
  size_t len = sizeof(struct opas_msg_reply6_proxy) + strlen(descr);
  
#ifdef HAVE_ALLOCA
  replyp = (struct opas_msg_reply6_proxy *)alloca(len);
#else
  replyp = (struct opas_msg_reply6_proxy *)malloc(len);
#endif
  
  replyp->head = OPAS_HEADER_MAKE(0,0,1,0,1,1,from_cache,0);
  replyp->user_data = queryp->user_data;
  replyp->addr = queryp->addr;
  replyp->timestamp = ts;
  replyp->proxy_type = type;
  replyp->proxy_port = port;
  strcpy(replyp->proxy_descr, descr);
  OPAS_HEADER_SETSIZE(replyp->head, len - 4);
  OPAS_REPLY_PROXY_HTON(replyp);
  (*session->cbs.send_fun)(replyp, len, session->context);
  
#ifndef HAVE_ALLOCA
  free(replyp);
#endif
  }

void
opas_reply_error(opas_session_t session, struct opas_msg_query *queryp,
                 uint32_t error)
  {
  struct opas_msg_reply_error reply;
  
  reply.head = queryp->head | OPAS_FLAG_REPL | OPAS_FLAG_ERRO;
  reply.user_data = queryp->user_data;
  reply.addr = queryp->addr;
  reply.error = error;
  OPAS_HEADER_SETSIZE(reply.head, 12);
  
  OPAS_REPLY_ERROR_HTON(&reply);
  (*session->cbs.send_fun)(&reply, sizeof(reply), session->context);
  }

void
opas_reply6_error(opas_session_t session, struct opas_msg_query6 *queryp,
                  uint32_t error)
  {
  struct opas_msg_reply6_error reply;
  
  reply.head = queryp->head | OPAS_FLAG_REPL | OPAS_FLAG_ERRO;
  reply.user_data = queryp->user_data;
  reply.addr = queryp->addr;
  reply.error = error;
  OPAS_HEADER_SETSIZE(reply.head, 24);
  
  OPAS_REPLY_ERROR_HTON(&reply);
  (*session->cbs.send_fun)(&reply, sizeof(reply), session->context);
  }

void
opas_send_msg_user(opas_session_t session, const void *msg_data,
                   uint32_t msg_length, int reply, uint32_t user_data)
  {
  struct opas_msg_user_header *ucheadp;
  
#ifdef HAVE_ALLOCA
  if (msg_length <= MSG_USER_MAXSMALL)
    {
    ucheadp = (struct opas_msg_user_header *)alloca(sizeof(*ucheadp)
                                                    + msg_length);
    }
  else
    {
#endif
    ucheadp = (struct opas_msg_user_header *)malloc(sizeof(*ucheadp)
                                                    + msg_length);
#ifdef HAVE_ALLOCA
    }
#endif
  
  ucheadp->head = OPAS_HEADER_MAKE(0,0,0,1,reply,0,0,0);
  ucheadp->user_data = user_data;
  ucheadp->data_length = msg_length;
  OPAS_HEADER_SETSIZE(ucheadp->head, 8);
  
  if (msg_length > 0)
    memcpy(ucheadp + 1, msg_data, msg_length);
  
  OPAS_MSG_USER_HTON(ucheadp);
  (*session->cbs.send_fun)(ucheadp, sizeof(*ucheadp) + msg_length,
                           session->context);
  
#ifdef HAVE_ALLOCA
  if (msg_length > MSG_USER_MAXSMALL)
#endif
    free(ucheadp);
  }

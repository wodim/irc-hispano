/* PEAK Library
 *
 * Copyright (c) 2003, 2004
 *      Stephane Thiell <mbuna@bugged.org>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#define RCSID "$Id: stream.c,v 1.7 2005/01/27 16:31:50 mbuna Exp $"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/stream.h>
#include <peak/time.h>
#include <peak/timer.h>
#include "engine_client.h"
#include "internal.h"
#include "socket.h"
#include "task_private.h"

#include <assert.h>
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#if defined(UIO_FASTIOV)
#define PEAK_STREAM_MAXIOV UIO_FASTIOV   /* Found on Linux */
#elif defined(UIO_SMALLIOV)
#define PEAK_STREAM_MAXIOV UIO_SMALLIOV  /* Found on BSD */
#else
#define PEAK_STREAM_MAXIOV 16            /* 1003.1g */
#endif

#ifndef SOCK_MAXADDRLEN
#define SOCK_MAXADDRLEN 255
#endif

#define IsEOL(c) ((c) == '\r') || ((c) == '\n')
#define PEAK_MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct __peak_stream_linemode_buffer
  {
  size_t size;
  size_t pos;
  char *ptr;
  } peak_stream_linemode_buffer;

typedef struct __peak_stream_msgbuf
  {
  struct __peak_stream_msgbuf * next;
  size_t length;
  size_t sent;
  
  /* flexible array: GNU extension is buf[0], C99 want buf[];
   * the more portable seems to be the GNU extension for now.
   */
  char buf[0];
  } peak_stream_msgbuf;

#define PEAK_STREAM_MSGBUF_H_SIZE sizeof(peak_stream_msgbuf)

typedef struct __peak_stream_buffered_output
  {
  size_t msg_size;
  size_t max_msgs;
  size_t allocated_msgs;
  peak_stream_error_callback error_cb;
  struct __msg_queue
    {
    peak_stream_msgbuf *head;
    peak_stream_msgbuf *tail;
    size_t count;
    size_t bytes;
    } ready_queue;
  struct __msg_stack
    {
    peak_stream_msgbuf *top;
    size_t count;
    } free_stack;
  } peak_stream_buffered_output;

struct __peak_stream
  {
  PEAK_STRUCT_ENGINE_CLIENT_HEADER;
  uint32_t _opt;
  union
    {
    struct sockaddr_in _sin;
    struct sockaddr * _sap;
    } _addr_u;
  int _sa_len;
  peak_stream_event_callback _cb;
  void *_context;
  
  /* internal buffer for linemode */
  peak_stream_linemode_buffer _lmbuf;
  
  /* buffered output */
  peak_stream_buffered_output _obuf;
  
  int _error;
  
  /* internal timer */
  peak_timer _timer;
  
  /* timeout value */
  int _timeout;
  
  /* last event time */
  time_t _last_evt;
  
  /* stats */
  size_t _read;
  size_t _write;
  };

#define PEAK_STREAM_GET_SA_PTR(s) \
        ((s->_opt & PEAK_STREAM_OPT_INTERNAL_INET) ? \
        (struct sockaddr *)&s->_addr_u._sin : s->_addr_u._sap)

static void __peak_stream_init(peak_stream s, va_list vp, void *ctcx);
static void __peak_stream_finalize(peak_stream s);
static int __peak_stream_listen(peak_stream s);
static void __peak_stream_linemode_read(peak_stream s);
static int __peak_stream_did_write(peak_stream s, int wres);

static void __peak_stream_timer_callback(peak_timer ti, void *context);

static int __peak_stream_send_queued(peak_stream s);
static inline peak_stream_msgbuf * __peak_stream_msgbuf_free_pop(
  peak_stream s);
static inline void __peak_stream_msgbuf_free_push(
  peak_stream s, peak_stream_msgbuf * mb);
static int __peak_stream_msgbuf_mapiov(
  peak_stream s, struct iovec *iov, int count, size_t *len);
static peak_stream_msgbuf * __peak_stream_msgbuf_new(peak_stream s);
static void __peak_stream_msgbuf_delete(peak_stream s, size_t length);
static void __peak_stream_msgbuf_commit(
  peak_stream s, peak_stream_msgbuf * mb);
static void __peak_stream_event_process(peak_stream s, int ioevent, int info);

/*
 * [01..08] Available creation options
 * [09..16] Reserved for internal use
 * [17..32] User data flags
 */

PEAK_CLASS_DECLARE(stream, engine_client);

enum
  {
  /* A call to connect() has been made
   */
  PEAK_STREAM_OPT_INTERNAL_CONNECT = 1 << 8,
  
  /* The stream comes from an accepted connection
   */
  PEAK_STREAM_OPT_INTERNAL_ACCEPTED = 1 << 9,
  
  /* Socket stream is open
   */
  PEAK_STREAM_OPT_INTERNAL_OPEN = 1 << 10,
  
  /* Raw stream (user provides the file descriptor)
   */
  PEAK_STREAM_OPT_INTERNAL_RAW = 1 << 11,
    
  /* Use INET (IPv4) in-line struct allocation simplification
   */
  PEAK_STREAM_OPT_INTERNAL_INET = 1 << 12,
  
  /* Output-buffered stream
   */
  PEAK_STREAM_OPT_INTERNAL_BUFFERED = 1 << 13
  };


peak_stream
peak_stream_socket_create(
  struct sockaddr *name,
  int namelen,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context)
  {
  return PEAK_CLASS_CONSTRUCT5(stream, opt, name, namelen, cb, context);
  }

peak_stream
peak_stream_accept_create(
  peak_stream ss,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context)
  {
  opt |= PEAK_STREAM_OPT_INTERNAL_ACCEPTED;
  if (opt & PEAK_STREAM_OPT_LISTEN)
    opt &= ~PEAK_STREAM_OPT_LISTEN; /* Can't have listen flag. */
  return PEAK_CLASS_CONSTRUCT4(stream, opt, ss, cb, context);
  }

#if 0
peak_stream
peak_stream_file_create(
  const char *path,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context)
  {
  opt |= PEAK_STREAM_OPT_INTERNAL_FILE;
  return PEAK_CLASS_CONSTRUCT4(stream, opt, path, cb, context);
  }
#endif

peak_stream
peak_stream_raw_create(
  int fd,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context)
  {
  opt |= PEAK_STREAM_OPT_INTERNAL_RAW;
  return PEAK_CLASS_CONSTRUCT4(stream, opt, fd, cb, context);
  }

static void
__peak_stream_init(peak_stream s, va_list vp, void *ctcx)
  {
  int fd;
  int namelen = 0;
  struct sockaddr *name;
  uint32_t state_init;
  uint32_t opt = va_arg(vp, uint32_t);
  
  s->_opt = opt;
  
  if (opt & PEAK_STREAM_OPT_INTERNAL_RAW)
    {
    fd = va_arg(vp, int);
    s->_sa_len = 0;
    if (opt & PEAK_STREAM_OPT_LISTEN)
      state_init = CS_SOCKET|CS_READING;
    else
      state_init = CS_SOCKET|CS_READING|CS_WRITING;
    }
#if 0
  else if (opt & PEAK_STREAM_OPT_INTERNAL_FILE)
    {
    int fflags = O_NONBLOCK;
    if (opt & PEAK_STREAM_OPT_RDWR)
      fflags |= O_CREAT|O_RDWR;
    else if (opt & PEAK_STREAM_OPT_WRONLY)
      fflags |= O_CREAT|O_WRONLY;
    else
      fflags |= O_RDONLY;
    
    if ((fd = open(va_arg(vp, const char*), fflags, 0)) < 0)
      PEAK_CT_RAISE("failed to open file", errno);
    
    state_init = CS_PEEKABLE|CS_READING; /*|CS_WRITING;*/
    }
#endif
  else if (opt & PEAK_STREAM_OPT_INTERNAL_ACCEPTED)
    {
    peak_stream ls = va_arg(vp, peak_stream);
        
    if (ls->_opt & PEAK_STREAM_OPT_INTERNAL_INET)
      {
      s->_opt |= PEAK_STREAM_OPT_INTERNAL_INET; /* inherit */
      name = (struct sockaddr *)&s->_addr_u._sin;
      namelen = sizeof(struct sockaddr_in);
      }
    else
      {
      namelen = ls->_sa_len;
      name = (struct sockaddr *)peak_allocate((size_t)namelen);
      s->_addr_u._sap = name;
      }
    s->_sa_len = namelen;
    
    fd = peak_socket_accept(ls->_c._ident, name, &s->_sa_len);
    if (fd == -1)
      {
      if (!(s->_opt & PEAK_STREAM_OPT_INTERNAL_INET))
        peak_deallocate(s->_addr_u._sap);
      PEAK_CT_RAISE("peak_socket_accept", errno);
      }
    _peak_engine_client_did_accept((peak_engine_client)ls);
    
    s->_opt |= PEAK_STREAM_OPT_INTERNAL_CONNECT;
    
    state_init = CS_SOCKET
                 | CS_PEEKABLE /* XXX */
                 | CS_READING
                 | CS_WRITING;
    }
  else
    {
    int type;
    name = va_arg(vp, struct sockaddr *);
    namelen = va_arg(vp, int);
    
    if (opt & PEAK_STREAM_OPT_DGRAM)
      type = SOCK_DGRAM;
    else
      type = SOCK_STREAM;
    
    fd = peak_socket_create(name->sa_family, type);
    assert(fd >= 0);
    PEAK_CT_RAISE_IF_ERROR(fd, "peak_socket_create", errno);
    
    if (name->sa_family == AF_INET)
      {
      if ((size_t)namelen != sizeof(struct sockaddr_in))
        PEAK_HALT;
      
      s->_opt |= PEAK_STREAM_OPT_INTERNAL_INET;
      s->_addr_u._sin = *((struct sockaddr_in *)name);
      }
    else
      {
      if (namelen > SOCK_MAXADDRLEN)
        PEAK_HALT;
      s->_addr_u._sap = (struct sockaddr *)peak_allocate((size_t)namelen);
      memcpy(s->_addr_u._sap, name, (size_t)namelen);
      }
    s->_sa_len = namelen;
    
    state_init = CS_SOCKET;
    if (!(opt & PEAK_STREAM_OPT_LISTEN))
      state_init |= CS_PEEKABLE;
    }
  
  s->_cb = va_arg(vp, peak_stream_event_callback);
  s->_context = va_arg(vp, void *);
  
  if (opt & PEAK_STREAM_OPT_LINEMODE)
    peak_stream_set_linemode(s);
  else
    {
    s->_lmbuf.size = 0;
    s->_lmbuf.pos = 0;
    s->_lmbuf.ptr = NULL;
    }
  s->_error = 0;
  s->_timer = NULL;
  s->_timeout = 0;
  s->_last_evt = 0;
  s->_read = s->_write = 0;
  
  if (state_init & CS_SOCKET)
    {
    peak_socket_set_nosigpipe(fd);
    peak_socket_set_reuseaddr(fd);
    if (peak_socket_set_nonblock(fd) == -1)
      PEAK_FATAL("peak_socket_set_nonblock failed", peak_socket_error());
    }
  
  _peak_engine_client_configure((peak_engine_client)s, fd, state_init,
          (peak_engine_client_event_process_func)&__peak_stream_event_process);
  
  if ((opt & PEAK_STREAM_OPT_LISTEN) && (__peak_stream_listen(s) != 0))
    PEAK_CT_RAISE("peak_stream_listen", errno);
  
  if (s->_opt & PEAK_STREAM_OPT_AUTOSCHEDULE)
    _peak_task_schedule_engine_client(peak_task_self(), (peak_engine_client)s);
  }

static void
__peak_stream_finalize(peak_stream s)
  {
  peak_stream_msgbuf * mb, * mbnext;
  
  if (s->_opt & PEAK_STREAM_OPT_LINEMODE)
    peak_deallocate(s->_lmbuf.ptr);
  
  if (s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED)
    {
    /* Deallocate all allocated msgbufs. */
    if (s->_obuf.ready_queue.count > 0)
      {
      for (mb = s->_obuf.ready_queue.head; mb; mb = mbnext)
        {
        mbnext = mb->next;
        peak_deallocate(mb);
        }
      s->_obuf.ready_queue.count = 0;
      }
    if (s->_obuf.free_stack.count > 0)
      {
      for (mb = s->_obuf.free_stack.top; mb; mb = mbnext)
        {
        mbnext = mb->next;
        peak_deallocate(mb);
        }
      s->_obuf.free_stack.count = 0;
      }
    }
  
  if (s->_timer)
    peak_stream_set_timeout(s, 0);
  }

int
peak_stream_is_open(peak_stream s)
  {
  return s->_opt & PEAK_STREAM_OPT_INTERNAL_OPEN;
  }

int
peak_stream_get_error(peak_stream s)
  {
  return s->_error;
  }

int
peak_stream_get_address(peak_stream s, struct sockaddr *name, int *namelen)
  {
  int i;
  char *dst, *src;
  
  if (*namelen < s->_sa_len)
    return -1;
  
  dst = (char *)name;
  src = (char *)PEAK_STREAM_GET_SA_PTR(s);
  
  for (i = s->_sa_len; i--; ) /* Copy forward */
    *dst++ = *src++;
  
  *namelen = s->_sa_len;
  return 0;
  }

int
peak_stream_set_address(peak_stream s, struct sockaddr *name, int namelen)
  {
  if (!(s->_opt & PEAK_STREAM_OPT_LISTEN))
    return peak_socket_bind(s->_c._ident, name, namelen);
  
  PEAK_FATAL("peak_stream_set_address cannot be called on a listening"
             " socket stream", 0);
  return -1; /* stfu, gcc! */
  }

int
peak_stream_is_linemode_enabled(peak_stream s)
  {
  return (s->_opt & PEAK_STREAM_OPT_LINEMODE);
  }

void
peak_stream_set_linemode(peak_stream s)
  {
  s->_opt |= PEAK_STREAM_OPT_LINEMODE;
  s->_lmbuf.size = LINEMODE_BUFFER_SIZE;
  s->_lmbuf.pos = 0;
  s->_lmbuf.ptr = (char *)peak_allocate(s->_lmbuf.size);
  }

int
peak_stream_connect(peak_stream s)
  {
  struct sockaddr *sap;
  
  if (s->_opt & PEAK_STREAM_OPT_INTERNAL_CONNECT)
    PEAK_HALT; /* Already connected... */
  
  /* XXX Write for UDP? */
  s->_c._state |= (s->_opt & PEAK_STREAM_OPT_DGRAM) ? CS_CONNECTING :
    CS_CONNECTING;
  
  s->_opt |= PEAK_STREAM_OPT_INTERNAL_CONNECT;
  
  sap = PEAK_STREAM_GET_SA_PTR(s);
    
  return peak_socket_connect(s->_c._ident, sap, s->_sa_len);
  }

void
peak_stream_disconnect(peak_stream s)
  {
  if (s->_opt & PEAK_STREAM_OPT_INTERNAL_CONNECT)
    {
    s->_opt &= ~PEAK_STREAM_OPT_INTERNAL_CONNECT;
    peak_socket_shutdown(s->_c._ident);
    }
  }

static int
__peak_stream_listen(peak_stream s)
  {
  struct sockaddr *sap;
  
  sap = PEAK_STREAM_GET_SA_PTR(s);
  
  peak_socket_bind(s->_c._ident, sap, s->_sa_len);
    
  if (s->_opt & PEAK_STREAM_OPT_DGRAM)
    {
    s->_c._state |= CS_READING;
    return 0;
    }
  else
    {
    s->_c._state |= CS_ACCEPTING;
    return peak_socket_listen(s->_c._ident);
    }
  }

int
peak_stream_read(peak_stream s, void *buffer, size_t bufsize)
  {
  int r = peak_socket_read(s->_c._ident, buffer, bufsize);
  if (r > 0)
    s->_read += r;
  _peak_engine_client_did_read((peak_engine_client)s);
  return r;
  }

int
peak_stream_recvfrom(peak_stream s, void *buffer, size_t bufsize,
                     struct sockaddr *from, int *fromlen)
  {
  int r = peak_socket_recvfrom(s->_c._ident, buffer, bufsize, from, fromlen);
  if (r > 0)
    s->_read += r;
  _peak_engine_client_did_read((peak_engine_client)s);
  return r;
  }

char*
peak_stream_get_line(peak_stream s)
  {
  if (!(s->_opt & PEAK_STREAM_OPT_LINEMODE))
    PEAK_FATAL("not in LINEMODE", 0);
  _peak_engine_client_did_read((peak_engine_client)s);
  return s->_lmbuf.ptr;
  }

static void
__peak_stream_linemode_read(peak_stream s)
  {
  char stbuf[LINEMODE_BUFFER_SIZE];
  const char *src = stbuf;
  char *p;
  int callback = 0;
  int r;
  
  /* Read chunk */
  r = peak_socket_read(s->_c._ident, (char*)stbuf, sizeof(stbuf));
  if (r <= 0)
    return;
  
  s->_read += r;
  
  /* Use stream linemode buffer. For each line found, generate a
   * stream_event_read event.
   */
  p = s->_lmbuf.ptr + s->_lmbuf.pos;
  
  while (r-- > 0)
    {
    *p = *src++;
        
    if (IsEOL(*p))
      {
      if (p == s->_lmbuf.ptr)
        continue;
      *p = '\0';
      
      (*s->_cb)(s, PEAK_STREAM_EVT_READ, s->_context);
      callback = 1;
      
      if (s->_c._rt._rc <= 1) /* hack! object destroyed. */
        return;
      
      p = s->_lmbuf.ptr;
      }
    else if (p < s->_lmbuf.ptr + LINEMODE_BUFFER_SIZE - 1)
      ++p;
    }
  
  s->_lmbuf.pos = p - s->_lmbuf.ptr;
  
  if (!callback)
    _peak_engine_client_did_read((peak_engine_client)s); /* We want more */
  }

size_t
peak_stream_get_read_count(peak_stream s)
  {
  return s->_read;
  }

int
peak_stream_write(peak_stream s, const void *buffer, size_t bufsize)
  {
  return __peak_stream_did_write(s, peak_socket_write(s->_c._ident, buffer,
                                                      bufsize));
  }

int
peak_stream_write_format(peak_stream s, const char *format, ...)
  {
  va_list vl;
  int result;
  
  va_start(vl, format);
  result = peak_stream_write_vformat(s, format, vl);
  va_end(vl);
  return result;
  }

int
peak_stream_write_vformat(peak_stream s, const char *format, va_list vl)
  {
  int length;
  char buffer[WRITE_FORMAT_BUFFER_SIZE];
  
  /* vsnprintf returns the number of characters that would have been printed
   * if the size were unlimited.
   */
  length = vsnprintf(buffer, sizeof(buffer), format, vl);
  if (length > sizeof(buffer) - 1)
    length = sizeof(buffer) - 1;
  
  return peak_stream_write(s, buffer, length);
  }

int
peak_stream_writev(peak_stream s, const struct iovec *iov, int iovcnt)
  {
  return __peak_stream_did_write(s, peak_socket_writev(s->_c._ident, iov,
                                                       iovcnt));
  }

size_t
peak_stream_get_write_count(peak_stream s)
  {
  return s->_write;
  }

static int
__peak_stream_did_write(peak_stream s, int wres)
  {
  if (wres > 0)
    {
    s->_write += wres;
    _peak_engine_client_did_write((peak_engine_client)s);
    }
  else if ((wres == -1) && peak_socket_error_blocked(peak_socket_error()))
    wres = 0;
  return wres;
  }

void*
peak_stream_get_context(peak_stream s)
  {
  return s->_context;
  }

void
peak_stream_set_context(peak_stream s, void *context)
  {
  s->_context = context;
  }

/* Timeout feature */

void
peak_stream_set_timeout(peak_stream s, int timeout)
  {
  if (timeout > 0)
    {
    double t;
    
    /* Set or change timer */
    s->_timeout = timeout;
    
    if (!s->_timer)
      {
      /* Set */
      t = (double)timeout;
      s->_timer = peak_timer_create(t, t,  __peak_stream_timer_callback,
                                    (void *)s); /* context is our stream ref */
      }
    else
      {
      /* Change */
      t = (double)(timeout - (peak_time() - s->_last_evt));
      peak_timer_configure(s->_timer, t > 0.0 ? t : 0.0, (double)timeout);
      }
    }
  else if (s->_timer)
    {
    /* Remove timer */
    if (s->_c._task)
      peak_task_timer_remove(s->_c._task, s->_timer);
    peak_release(s->_timer);
    s->_timer = NULL;
    s->_timeout = 0;
    }
  }

static void
__peak_stream_timer_callback(peak_timer ti, void *context)
  {
  peak_stream s = (peak_stream)context;
  int nextfire;
  
  /* Avoid collision */
  if (s->_c._state & CS_HANDLED)
    return;
  
  /* Verify real event idle time. */
  nextfire = s->_timeout - (peak_time() - s->_last_evt);
  
  /* Call out event handler, or synchronize timer. */
  if (nextfire <= 0)
    (*s->_cb)(s, PEAK_STREAM_EVT_TIMEDOUT, s->_context);
  else
    peak_timer_configure(s->_timer, (double)nextfire, (double)s->_timeout);
  }


/* Buffered write API */

void
peak_stream_set_buffered(peak_stream s, int enable, size_t msg_size,
                         size_t max_size, peak_stream_error_callback cb)
  {
  if (enable)
    {
    if (s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED)
      {
      /* Already configured */
      if (max_size < 2)
        PEAK_FATAL("peak_stream_set_buffered: max_size too low", 0);
      
      /* Allow max_size and callback changes only */
      s->_obuf.max_msgs = (max_size - 1)/s->_obuf.msg_size + 1;
      s->_obuf.error_cb = cb;
      }
    else
      {
      if (max_size < 2)
        PEAK_FATAL("peak_stream_set_buffered: max_size too low", 0);
      
      if (msg_size == 0)
        {
#ifdef HAVE_GETPAGESIZE
        s->_obuf.msg_size = getpagesize();
#else
        s->_obuf.msg_size = 4096;
#endif
        }
      else
        s->_obuf.msg_size = msg_size;
      
      /* calculate max number of messages needed, we have always at least
       * one message...
       */
      s->_obuf.max_msgs = (max_size - 1)/s->_obuf.msg_size + 1;
      s->_obuf.allocated_msgs = 0;
      s->_obuf.error_cb = cb;
      s->_obuf.ready_queue.head = NULL;
      s->_obuf.ready_queue.tail = NULL;
      s->_obuf.ready_queue.count = 0;
      s->_obuf.ready_queue.bytes = 0;
      s->_obuf.free_stack.top = NULL;
      s->_obuf.free_stack.count = 0;
      s->_opt |= PEAK_STREAM_OPT_INTERNAL_BUFFERED;
      }
    }
  else
    {
    peak_stream_msgbuf * mb, * mbnext;
    
    s->_opt &= ~PEAK_STREAM_OPT_INTERNAL_BUFFERED;
    
    /* Deallocate all allocated msgbufs. */
    if (s->_obuf.ready_queue.count > 0)
      {
      for (mb = s->_obuf.ready_queue.head; mb; mb = mbnext)
        {
        mbnext = mb->next;
        peak_deallocate(mb);
        }
      s->_obuf.ready_queue.count = 0;
      }
    if (s->_obuf.free_stack.count > 0)
      {
      for (mb = s->_obuf.free_stack.top; mb; mb = mbnext)
        {
        mbnext = mb->next;
        peak_deallocate(mb);
        }
      s->_obuf.free_stack.count = 0;
      }
    
    s->_obuf.msg_size = 0;
    s->_obuf.max_msgs = 0;
    s->_obuf.allocated_msgs = 0;
    s->_obuf.error_cb = NULL;
    s->_obuf.ready_queue.head = NULL;
    s->_obuf.ready_queue.tail = NULL;
    s->_obuf.ready_queue.bytes = 0;
    s->_obuf.free_stack.top = NULL;
    }
  }

void
peak_stream_msgbuf_get_info(peak_stream s, size_t *msg_size, size_t *max_msgs,
                            size_t *alloc_msgs, size_t *queue_msgs,
                            size_t *queue_size)
  {
  if (!(s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED))
    PEAK_FATAL("not in buffered mode", 0);
  
  if (msg_size)
    *msg_size = s->_obuf.msg_size;
  if (max_msgs)
    *max_msgs = s->_obuf.max_msgs;
  if (alloc_msgs)
    *alloc_msgs = s->_obuf.allocated_msgs;
  if (queue_msgs)
    *queue_msgs = s->_obuf.ready_queue.count;
  if (queue_size)
    *queue_size = s->_obuf.ready_queue.bytes;
  }

void
peak_stream_write_buffered(peak_stream s, const void *buffer, size_t bufsize)
  {
  int res = peak_stream_write(s, buffer, bufsize);
  
#if 0
  if (res != bufsize)
    fprintf(stderr, "peak_stream_write_buffered uncomplete write\n");
#endif
  
  if (res == -1)
    {
    int err = errno;
    
    fprintf(stderr, "peak_stream_write_buffered write failure (errno=%d)\n",
            err);
    /* Not fatal if:
     * - Would block ! The socket is set non blocking and the application is
     *   trying to send outside a write event (its right). Normal case.
     * Why ENOTCONN ?
     * - Very similar to the blocking situation, the stream is not open yet,
     *   eg. the users called set_buffered(), connect() and immediately
     *   write_buffered().
     * Why EPIPE ?
     * - The socket is unable to send anymore data (SS_CANTSENDMORE has
     *   been set on the socket). Found that on FreeBSD 5.1 in a recent
     *   documentation addition.
     */
    if (peak_socket_error_blocked(err) || err == ENOTCONN
        || err == EPIPE || errno == EINTR)
      res = 0;
    else
      /* Other errors are ignored and should be handled by the engine at the
       * next event loop.
       */
      return;
    }
  
  if (s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED)
    {
    peak_stream_msgbuf * mb;
        
    for (; res < bufsize; res += mb->length)
      {
#if 0
      fprintf(stderr, "peak_stream_send> buffering needed (%lu/%lu)\n",
              bufsize - res, bufsize);
#endif
      
      if ((mb = __peak_stream_msgbuf_new(s)) == NULL)
        break;
      mb->length = PEAK_MIN(bufsize - res, s->_obuf.msg_size);
      mb->sent = 0;
      memcpy(mb->buf, (char *)buffer + res, mb->length);
      __peak_stream_msgbuf_commit(s, mb);
      }
    }
  }

static int
__peak_stream_send_queued(peak_stream s)
  {
  struct iovec *iov;
  size_t iovcnt, wlen = 0;
  int count, bytes;
  
  iovcnt = s->_obuf.ready_queue.count;
  if (iovcnt > PEAK_STREAM_MAXIOV)
    iovcnt = PEAK_STREAM_MAXIOV;
  
#ifdef HAVE_ALLOCA
  iov = (struct iovec *)alloca(sizeof(struct iovec) * iovcnt);
#else
  iov = (struct iovec *)peak_allocate(sizeof(struct iovec) * iovcnt);
#endif
  
  count = __peak_stream_msgbuf_mapiov(s, iov, iovcnt, &wlen);
  
  switch ((bytes = peak_stream_writev(s, iov, count)))
    {
    case -1:
      /* Ignore error, let it deal by the engine in the next event loop.
       */
      break;
    case 0:
      /* Would block ?? */
      PEAK_HALT;
      break;
    default:
      /* Success */
      assert(bytes <= wlen);
      __peak_stream_msgbuf_delete(s, bytes);
      break;
    }
#ifndef HAVE_ALLOCA
  peak_deallocate(iov);
#endif
  return bytes;
  }

static inline peak_stream_msgbuf *
__peak_stream_msgbuf_free_pop(peak_stream s)
  {
  peak_stream_msgbuf * mb;
    
  if ((mb = s->_obuf.free_stack.top))
    {
    s->_obuf.free_stack.top = mb->next;
    s->_obuf.free_stack.count--;
    }
  return mb;
  }

static inline void
__peak_stream_msgbuf_free_push(peak_stream s, peak_stream_msgbuf * mb)
  {
  mb->next = s->_obuf.free_stack.top;
  s->_obuf.free_stack.top = mb;
  s->_obuf.free_stack.count++;
  }

static int
__peak_stream_msgbuf_mapiov(
  peak_stream s, struct iovec *iov, int count, size_t *len)
  {
  peak_stream_msgbuf * q;
  int i = 0;
    
  q = s->_obuf.ready_queue.head;
    
  assert(q != NULL);
  assert(q->sent < q->length);
    
  /* Partial msg ? */
  if (q->sent > 0)
    {
    iov[i].iov_base = q->buf + q->sent;
    iov[i].iov_len = q->length - q->sent;
    *len += iov[i].iov_len;
    q = q->next;
    i++;
    if (--count <= 0)
      return i;
    }
  /* Rest of the queue */
  for (; q; q = q->next)
    {
    iov[i].iov_base = q->buf;
    iov[i].iov_len = q->length;
    *len += iov[i].iov_len;
    i++;
    if (--count <= 0)
      break;
    }
  return i;
  }

static peak_stream_msgbuf *
__peak_stream_msgbuf_new(peak_stream s)
  {
  peak_stream_msgbuf * mb;
  
  if (!(s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED))
    PEAK_FATAL("not in buffered mode", 0);
  
  if ((mb = __peak_stream_msgbuf_free_pop(s)) != NULL)
    return mb;
  
  if (s->_obuf.allocated_msgs >= s->_obuf.max_msgs)
    {
    if (s->_obuf.error_cb)
      (*s->_obuf.error_cb)(s, PEAK_STREAM_ERR_MAX_MSGBUF_EXCEEDED,
                           s->_context);
    return NULL;
    }
  
#if 0
  printf("__peak_stream_msgbuf_new> new buffer (%d)\n",
         s->_obuf.allocated_msgs + 1);
#endif
  mb = (peak_stream_msgbuf *)peak_allocate(PEAK_STREAM_MSGBUF_H_SIZE
                                           + s->_obuf.msg_size);
  mb->next = NULL;
  s->_obuf.allocated_msgs++;
  return mb;
  }

void*
peak_stream_msgbuf_new(peak_stream s)
  {
  peak_stream_msgbuf * mb;
  
  mb = __peak_stream_msgbuf_new(s);
  return mb ? (void*)mb->buf : NULL;
  }

static void
__peak_stream_msgbuf_delete(peak_stream s, size_t length)
  {
  peak_stream_msgbuf * mb;
  size_t msglen;
  
  s->_obuf.ready_queue.bytes -= length;
  
  while (length > 0)
    {
    mb = s->_obuf.ready_queue.head;
    assert(mb != NULL);
    msglen = mb->length - mb->sent;
    if (length >= msglen)
      {
      length -= msglen;
            
      /* Dequeue ready msgbuf */
      s->_obuf.ready_queue.count--;
      s->_obuf.ready_queue.head = mb->next;
      if (s->_obuf.ready_queue.count == 0)
        s->_obuf.ready_queue.tail = NULL;
            
      mb->next = NULL;
      mb->length = 0;
      mb->sent = 0;
            
      __peak_stream_msgbuf_free_push(s, mb);
      }
    else
      {
      /* Partial */
      mb->sent += length;
      break;
      }
    }
  }

static void
__peak_stream_msgbuf_commit(peak_stream s, peak_stream_msgbuf * mb)
  {
  if (s->_obuf.ready_queue.count == 0)
    {
    s->_obuf.ready_queue.head = s->_obuf.ready_queue.tail = mb;
    }
  else
    {
    s->_obuf.ready_queue.tail->next = mb;
    s->_obuf.ready_queue.tail = mb;
    }
  s->_obuf.ready_queue.count++;
  s->_obuf.ready_queue.bytes += mb->length;
    
  /* To be sure the engine will poll for write events, we simulate
   * a "did write" notification.
   */
  _peak_engine_client_did_write((peak_engine_client)s);
  }

void
peak_stream_msgbuf_commit(peak_stream s, void *buffer, int length)
  {
  peak_stream_msgbuf * mb;
  /* TODO special case length==0 */
  /* TODO verify length<msg_size */
  
  mb = (peak_stream_msgbuf *)((char*)buffer - PEAK_STREAM_MSGBUF_H_SIZE);
  mb->length = length; /* Set msg length */
  mb->sent = 0;        /* Nothing sent yet */
  __peak_stream_msgbuf_commit(s, mb);
  }


/* buffered mode only, to deal with lines */

void
peak_stream_msgbuf_make(peak_stream s, const char *format, ...)
  {
  va_list vl;
    
  va_start(vl, format);
  peak_stream_msgbuf_vmake(s, format, vl);
  va_end(vl);
  }

void
peak_stream_msgbuf_vmake(peak_stream s, const char *format, va_list vl)
  {
  peak_stream_msgbuf * mb;
  
  if ((mb = __peak_stream_msgbuf_new(s)))
    {
    /* vsnprintf returns the number of characters that would have been printed
     * if the size were unlimited.
     */
    mb->length = vsnprintf(mb->buf, s->_obuf.msg_size, format, vl);
    if (mb->length > s->_obuf.msg_size - 1)
      mb->length = s->_obuf.msg_size - 1;
    
    mb->sent = 0;
    __peak_stream_msgbuf_commit(s, mb);
    }
  }

/* Scheduling */

void
peak_stream_schedule(peak_stream s, peak_task task)
  {
  _peak_task_schedule_engine_client(task, (peak_engine_client)s);
  if (s->_timer)
    peak_task_timer_add(task, s->_timer);
  }

void
peak_stream_unschedule(peak_stream s, peak_task task)
  {
  if (s->_timer)
    peak_task_timer_remove(task, s->_timer);
  _peak_task_unschedule_engine_client(task, (peak_engine_client)s);
  }

static void
__peak_stream_event_process(peak_stream s, int ioevent, int info)
  {
  s->_last_evt = peak_time();
  
  switch (ioevent)
    {
    case IOEVENT_CONNECT: /* Open completed */
      s->_opt |= PEAK_STREAM_OPT_INTERNAL_OPEN;
      (*s->_cb)(s, PEAK_STREAM_EVT_OPEN, s->_context);
      break;
    case IOEVENT_ACCEPT:
      (*s->_cb)(s, PEAK_STREAM_EVT_ACCEPT, s->_context);
      break;
    case IOEVENT_READ:
      if (s->_opt & PEAK_STREAM_OPT_INTERNAL_ACCEPTED)
        {
        s->_opt &= ~PEAK_STREAM_OPT_INTERNAL_ACCEPTED;
        (*s->_cb)(s, PEAK_STREAM_EVT_OPEN, s->_context);
        }
      if (peak_stream_is_linemode_enabled(s))
        __peak_stream_linemode_read(s);
      else
        (*s->_cb)(s, PEAK_STREAM_EVT_READ, s->_context);
      break;
    case IOEVENT_WRITE:
      if (s->_opt & PEAK_STREAM_OPT_INTERNAL_ACCEPTED)
        {
        s->_opt &= ~PEAK_STREAM_OPT_INTERNAL_ACCEPTED;
        (*s->_cb)(s, PEAK_STREAM_EVT_OPEN, s->_context);
        }
      /* Process buffered pending writes... */
      if (s->_opt & PEAK_STREAM_OPT_INTERNAL_BUFFERED
          && s->_obuf.ready_queue.count > 0)
        __peak_stream_send_queued(s);
      else
        (*s->_cb)(s, PEAK_STREAM_EVT_WRITE, s->_context);
      break;
    case IOEVENT_EOF:
      (*s->_cb)(s, PEAK_STREAM_EVT_END, s->_context);
      break;
    case IOEVENT_ERROR:
      s->_error = info;
      (*s->_cb)(s, PEAK_STREAM_EVT_ERROR, s->_context);
      break;
    case IOEVENT_NONE:
    default:
      assert(0);
    }
  }


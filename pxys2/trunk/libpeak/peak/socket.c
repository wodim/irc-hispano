/* PEAK Library
 *
 * Copyright (c) 2003
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
 * $Id: socket.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $
 */
#define RCSID "$Id: socket.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "socket.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#ifndef SOMAXCONN
#define SOMAXCONN 64
#endif

__private_extern__ int
peak_socket_create(int domain, int type)
  {
  return socket(domain, type, 0);
  }

__private_extern__ int
peak_socket_close(int fd)
  {
  return close(fd);
  }

__private_extern__ int
peak_socket_shutdown(int fd)
  {
  return shutdown(fd, SHUT_RDWR);
  }

__private_extern__ int
peak_socket_get_error(int fd)
  {
  int err = 0;
  unsigned int len;
  errno = 0;
#if defined(SO_ERROR)
  len = sizeof(err);
  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
    err = errno; /* Solaris workaround */
#else
#warning SO_ERROR not defined
#endif
  return err;
  }

__private_extern__ int
peak_socket_error(void)
  {
  return errno;
  }

__private_extern__ int
peak_socket_error_pending(int error)
  {
  return (error == EINPROGRESS || error == EALREADY);
  }

__private_extern__ int
peak_socket_error_recoverable(int error)
  {
  return (error == 0 || error == EAGAIN || error == EINTR ||
          error == EINPROGRESS || error == EWOULDBLOCK);
  }

__private_extern__ int
peak_socket_error_blocked(int error)
  {
  return (error == EAGAIN
#ifdef EWOULDBLOCK
          || error == EWOULDBLOCK
#endif
          || error == EAGAIN
          || error == ENOBUFS
          || error == ENOMEM
#ifdef ENOSR
          || error == ENOSR /* Solaris */
#endif
    );
  }


__private_extern__ int
peak_socket_error_success(int error)
  {
  return error == 0;
  }

__private_extern__ int
peak_socket_set_bufs(int fd, size_t size)
  {
  unsigned int opt = (unsigned int)size;
  return ((0 == setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*) &opt,
                           sizeof(opt)) &&
           0 == setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*) &opt,
                           sizeof(opt))) ? 0 : -1);
  }

__private_extern__ int
peak_socket_set_keepalive(int fd)
  {
  unsigned int opt = 1;
  return setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&opt,
                    sizeof(opt));
  }

__private_extern__ int
peak_socket_set_nolinger(int fd)
  {
  struct linger lin = { 0, 0 };
  return setsockopt(fd, SOL_SOCKET, SO_LINGER, (void *)&lin,
                    sizeof(struct linger));
  }

__private_extern__ int
peak_socket_set_nosigpipe(int fd)
  {
#ifdef SO_NOSIGPIPE
  int sz = 1;
  return setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &sz, sizeof(sz));
#else
  (void)fd;
  return 0;
#endif
  }

__private_extern__ int
peak_socket_set_reuseaddr(int fd)
  {
  unsigned int opt = 1;
  return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt,
                    sizeof(opt));
  }

#define NBLOCK_BSD
__private_extern__ int
peak_socket_set_nonblock(int fd)
  {
  int res;
#ifndef NBLOCK_SYSV
  int nonb = 0;
#endif
    
#ifdef  NBLOCK_POSIX
  nonb |= O_NONBLOCK;
#endif
#ifdef  NBLOCK_BSD
  nonb |= O_NDELAY;
#endif
#ifdef  NBLOCK_SYSV
  res = 1;
    
  if (ioctl(desc, FIONBIO, &res) == -1)
    return 0;
#else
  if ((res = fcntl(fd, F_GETFL, 0)) == -1)
    return 0;
  else if (fcntl(fd, F_SETFL, res | nonb) == -1)
    return 0;
#endif
  return 1;
  }

__private_extern__ int
peak_socket_accept(int s, struct sockaddr *addr, int *addrlen)
  {
  return accept(s, addr, addrlen);
  }

__private_extern__ int
peak_socket_bind(int fd, struct sockaddr *addr, int addrlen)
  {
  return bind(fd, addr, addrlen);
  }

__private_extern__ int
peak_socket_connect(int fd, struct sockaddr *addr, int addrlen)
  {
  errno = 0;
    
  if (connect(fd, addr, addrlen) < 0 &&
      !peak_socket_error_pending(peak_socket_error()))
    return -1;
  return 0;
  }

__private_extern__ int
peak_socket_listen(int fd)
  {
  errno = 0;
  return listen(fd, SOMAXCONN);
  }

__private_extern__ int
peak_socket_read(int fd, void *buf, size_t bufsize)
  {
  return read(fd, buf, bufsize);
  }

__private_extern__ int
peak_socket_recvfrom(int fd, void *buf, size_t bufsize,
                     struct sockaddr *from, int *fromlen)
  {
  return recvfrom(fd, buf, bufsize, 0, from, fromlen);
  }

__private_extern__ int
peak_socket_write(int fd, const void *buf, size_t bufsize)
  {
  return write(fd, buf, bufsize);
  }

int
peak_socket_writev(int fd, const struct iovec *iov, int iovcnt)
  {
  return writev(fd, iov, iovcnt);
  }

__private_extern__ int
peak_socket_peek(int fd)
  {
  char c;
  return recv(fd, &c, 1, MSG_PEEK);
  }

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
 * $Id: socket.h,v 1.1.1.1 2003/12/30 02:29:22 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_SOCKET_H_
#define INCLUDED_PEAK_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "internal.h"

struct iovec;

__private_extern__ int peak_socket_create(int domain, int type);
__private_extern__ int peak_socket_shutdown(int fd);
__private_extern__ int peak_socket_close(int fd);

__private_extern__ int peak_socket_get_error(int fd);
__private_extern__ int peak_socket_error(void);
__private_extern__ int peak_socket_error_pending(int error);
__private_extern__ int peak_socket_error_recoverable(int error);
__private_extern__ int peak_socket_error_blocked(int error);
__private_extern__ int peak_socket_error_success(int error);
__private_extern__ int peak_socket_set_bufs(int fd, size_t size);
__private_extern__ int peak_socket_set_keepalive(int fd);
__private_extern__ int peak_socket_set_nolinger(int fd);
__private_extern__ int peak_socket_set_nosigpipe(int fd);
__private_extern__ int peak_socket_set_reuseaddr(int fd);
__private_extern__ int peak_socket_set_nonblock(int fd);

__private_extern__ int peak_socket_accept(int s, struct sockaddr *addr,
                                          int *addrlen);
__private_extern__ int peak_socket_bind(int fd, struct sockaddr *addr,
                                        int addrlen);
__private_extern__ int peak_socket_connect(int fd, struct sockaddr *addr,
                                           int addrlen);
__private_extern__ int peak_socket_listen(int fd);
__private_extern__ int peak_socket_read(int fd, void *buf, size_t bufsize);
__private_extern__ int peak_socket_recvfrom(int fd, void *buf, size_t bufsize,
                                            struct sockaddr *from,
                                            int *fromlen);
__private_extern__ int peak_socket_write(int fd, const void *buf,
                                         size_t bufsize);
__private_extern__ int peak_socket_writev(int fd, const struct iovec *iov,
                                          int iovcnt);
__private_extern__ int peak_socket_peek(int fd);

#endif /* INCLUDED_PEAK_SOCKET_H_ */

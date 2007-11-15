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
 */
#define RCSID "$Id: fileutils.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fileutils.h"
#include "internal.h"
#include <peak/timer.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


__private_extern__ int
peak_read_file(const char *path,
               void **bufp,
               long *length)
  {
  struct stat sb;
  int fd;
  
  if ((fd = open(path, O_RDONLY)) < 0)
    return 0;
  
  if (fstat(fd, &sb) < 0)
    {
    int err = errno;
    close(fd);
    errno = err;
    return 0;
    }
  if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
    close(fd);
    errno = EACCES;
    return 0;
    }
  
  if (sb.st_size == 0)
    {
    *bufp = NULL;
    length = 0;
    }
  else
    {
    size_t size = sb.st_size;
    ssize_t len;
    
    *bufp = peak_allocate(size);
    if ((len = read(fd, *bufp, size)) < 0)
      {
      int err = errno;
      peak_deallocate(*bufp);
      *bufp = NULL;
      close(fd);
      errno = err;
      return 0;
      }
    *length = len;
    }
  close(fd);
  return 1;
  }



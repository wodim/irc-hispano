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
#define RCSID "$Id: errors.c,v 1.2 2004/01/08 19:03:54 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "internal.h" /* errors.h is part of internal */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

__private_extern__ void
_peak_fatal(const char *file, int line, const char *m, int e)
  {
  fprintf(stderr, "PEAK FATAL (%s:%d): %s\n", file, line, m);
  if (e != 0)
    fprintf(stderr, "-> error %d (%s)\n", e, strerror(e));
  abort();
  }

__private_extern__ void
_peak_halt(const char *file, int line)
  {
  fprintf(stderr, "PEAK HALT (%s:%d)\n", file, line);
  abort();
  }

__private_extern__ void
_peak_warn(const char *file, int line, const char *m)
  {
  fprintf(stderr, "PEAK WARNING (%s:%d): %s\n", file, line, m);
  }

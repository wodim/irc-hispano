/* PEAK Library
 *
 * Copyright (c) 2003-2005
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
#define RCSID "$Id: alloc.c,v 1.2 2005/01/27 16:31:49 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/alloc.h>
#include "internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static peak_alloc_malloc_func __malloc_fun = malloc;
static peak_alloc_free_func __free_fun = free;

void
peak_alloc_configure(peak_alloc_malloc_func malloc_fun,
                     peak_alloc_free_func free_fun)
  {
  __malloc_fun = (malloc_fun) ? malloc_fun : malloc;
  __free_fun = (free_fun) ? free_fun : free;
  }

void*
peak_allocate(size_t size)
  {
  void *ptr = (*__malloc_fun)(size);
  if (ptr == NULL)
    PEAK_HALT;
  return ptr;
  }

void
peak_deallocate(void *ptr)
  {
  (*__free_fun)(ptr);
  }

char *
peak_strdup(const char *str)
  {
  size_t len = strlen(str) + 1;
  return memcpy(peak_allocate(len), str, len);
  }

/* peak_retain and peak_release are defined in runtime.c
 */

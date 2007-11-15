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
#define RCSID "$Id: task_runloop_stackpool.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "task_runloop_stackpool.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "internal.h"
#include "utilities.h"

static void __peak_task_runloop_stackpool_init(
  peak_task_runloop_stackpool sp,
  va_list vp, void *ctcx);
static void __peak_task_runloop_stackpool_finalize(
  peak_task_runloop_stackpool sp);

PEAK_CLASS_BASE_DECLARE(task_runloop_stackpool);

peak_task_runloop_stackpool
peak_task_runloop_stackpool_create()
  {
  return PEAK_CLASS_CONSTRUCT0(task_runloop_stackpool);
  }

static void
__peak_task_runloop_stackpool_init(peak_task_runloop_stackpool sp, va_list vp,
                                   void *ctcx)
  {
  sp->_count = 0;
  sp->_size = 2;
  
  /* 0 is important as an optimization in task.c depends on it */
  sp->_ring = 0;
  
  sp->_base = peak_allocate(sizeof(peak_task_runloop) * (size_t)sp->_size);
  sp->_top = sp->_base;
  }

static void
__peak_task_runloop_stackpool_finalize(peak_task_runloop_stackpool sp)
  {
  peak_deallocate(sp->_base);
  sp->_base = NULL; /* safety debug */
  }

void
peak_task_runloop_stackpool_push(peak_task_runloop_stackpool sp,
                                 peak_task_runloop rl)
  {
  assert(rl != NULL);
  
  if (sp->_count == sp->_size)
    {
    peak_task_runloop *ptr;
    sp->_size <<= 1;
    ptr = (peak_task_runloop *)peak_allocate(sizeof(peak_task_runloop)
                                             * (size_t)sp->_size);
    memcpy(ptr, sp->_base, sizeof(peak_task_runloop) * (size_t)sp->_count);
    sp->_top = ptr + sp->_count;
    peak_deallocate(sp->_base);
    sp->_base = ptr;
    }
  
  *sp->_top++ = rl;
  sp->_count++;
  }


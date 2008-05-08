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
#define RCSID "$Id: garbage.c,v 1.3 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/garbage.h>
#include <peak/alloc.h>
#include <peak/task.h>
#include <peak/timer.h>
#include "internal.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

enum
  {
  PEAK_GARBAGE_FLAG_VACUUMING = 1
  };

static void __peak_garbage_init(peak_garbage g, va_list vp, void *ctcx);
static void __peak_garbage_finalize(peak_garbage g);
static void __peak_garbage_timer_callback(peak_timer ti, peak_garbage g);
static void __peak_garbage_timer_enable(peak_garbage g);
static void __peak_garbage_timer_disable(peak_garbage g);
static void __peak_garbage_vacuum(peak_garbage g);

struct __peak_garbage
  {
  PEAK_STRUCT_RT_HEADER;
  int _flags;
  int _ptr_max;
  int _ptr_count;
  void **_ptr_table;
  peak_timer _cleanup_timer;
  peak_garbage_callback _callout;
  peak_task _task;
  void *_context;
  };

PEAK_CLASS_BASE_DECLARE(garbage);

peak_garbage
peak_garbage_create(int max_ptrs, double frequency, peak_task task,
                    peak_garbage_callback callout, void *context)
  {
  return PEAK_CLASS_CONSTRUCT5(garbage, max_ptrs, frequency, task,
                               callout, context);
  }

static void
__peak_garbage_init(peak_garbage g, va_list vp, void *ctcx)
  {
  g->_flags = 0;
  
  g->_ptr_max = va_arg(vp, int);
  if (g->_ptr_max <= 0)
    PEAK_CT_RAISE("peak_garbage_init: illegal max_ptrs value", 0);
  
  g->_ptr_count = 0; /* table is empty */
  
  /* Allocate garbage collector pointers table */
  g->_ptr_table = (void **)peak_allocate(sizeof(void*) * (size_t)g->_ptr_max);
  
  /* Create cleanup timer */
  g->_cleanup_timer =
    peak_timer_create(-1, va_arg(vp, double) /* frequency */,
                      (peak_timer_callback)__peak_garbage_timer_callback,
                      (void*)g);
  
  if (!g->_cleanup_timer)
    PEAK_CT_RAISE("peak_garbage_init: failed to create cleanup timer", 0);
  
  g->_task = va_arg(vp, peak_task);
  if (!g->_task)
    g->_task = peak_task_self();
  g->_callout = va_arg(vp, peak_garbage_callback);
  g->_context = va_arg(vp, void*);
  }

static void
__peak_garbage_finalize(peak_garbage g)
  {
  peak_garbage_vacuum(g); /* Avoid leaking */
  peak_deallocate(g->_ptr_table);
  peak_release(g->_cleanup_timer);
  }

static void
__peak_garbage_timer_callback(peak_timer ti, peak_garbage g)
  {
  peak_garbage_vacuum(g);
  }

static void
__peak_garbage_timer_enable(peak_garbage g)
  {
  double interval = peak_timer_get_interval(g->_cleanup_timer);
  peak_timer_configure(g->_cleanup_timer, interval, interval);
  peak_task_timer_add(g->_task, g->_cleanup_timer);
  }

static void
__peak_garbage_timer_disable(peak_garbage g)
  {
  peak_task_timer_remove(g->_task, g->_cleanup_timer);
/*  peak_timer_configure(g->_cleanup_timer, -1, -1); */
  }

void
peak_garbage_collect(peak_garbage g, void *ptr)
  {
  if (g->_flags & PEAK_GARBAGE_FLAG_VACUUMING)
    PEAK_FATAL("Can't collect garbage pointer while vacuuming it", 0);
  
  if (!g->_ptr_count)
    __peak_garbage_timer_enable(g);
  
  if (g->_ptr_count == g->_ptr_max)
    __peak_garbage_vacuum(g); /* force vacuuming */
  
  g->_ptr_table[g->_ptr_count++] = ptr;
  }

int
peak_garbage_get_collected_count(peak_garbage g)
  {
  return g->_ptr_count;
  }

static void
__peak_garbage_vacuum(peak_garbage g)
  {
  int i;
  
  if (!g->_ptr_count)
    return;
  
  if (g->_flags & PEAK_GARBAGE_FLAG_VACUUMING)
    PEAK_FATAL("Can't vacuum garbage object while already vacuuming it", 0);
  
  g->_flags |= PEAK_GARBAGE_FLAG_VACUUMING;
  
  for (i = 0; i < g->_ptr_count; i++)
    (*g->_callout)(g, g->_ptr_table[i], g->_context);
  
  g->_ptr_count = 0;
  
  g->_flags &= ~PEAK_GARBAGE_FLAG_VACUUMING;
  }

void
peak_garbage_vacuum(peak_garbage g)
  {
  if (!g->_ptr_count)
    return;
  
  __peak_garbage_vacuum(g);
  __peak_garbage_timer_disable(g);
  }

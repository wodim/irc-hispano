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
 * $Id: task_runloop_stackpool.h,v 1.1.1.1 2003/12/30 02:29:34 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TASK_RUNLOOP_STACKPOOL_H_
#define INCLUDED_PEAK_TASK_RUNLOOP_STACKPOOL_H_

#include "task_runloop.h"

/* A specialized pool for runloop objects implemented using a stack,
 * features a random access method.
 */

struct __peak_task_runloop_stackpool
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task_runloop *_base;
  peak_task_runloop *_top;
  int _count;
  int _size;
  int _ring;
  };

typedef struct __peak_task_runloop_stackpool * peak_task_runloop_stackpool;

#if defined(__cplusplus)
extern "C" {
#endif

/* Get the number of items.
 */
static inline int
peak_task_runloop_stackpool_count(peak_task_runloop_stackpool sp)
  {
  return sp->_count;
  }

/* Remove an item from the stackpool.
 */
static inline void
peak_task_runloop_stackpool_pop(peak_task_runloop_stackpool sp)
  {
  sp->_top--;
  sp->_count--;
  }

/* Top accessor.
 */
static inline peak_task_runloop
peak_task_runloop_stackpool_top(peak_task_runloop_stackpool sp)
  {
  return sp->_count > 0 ? *(sp->_top - 1) : NULL;
  }

/* Uniform accessor.
 */
static inline peak_task_runloop
peak_task_runloop_stackpool_get(peak_task_runloop_stackpool sp)
  {
  return sp->_base[sp->_ring++ % sp->_count];
  }

/* Reset uniform accessor state.
 */
static inline void
peak_task_runloop_stackpool_reset(peak_task_runloop_stackpool sp)
  {
  sp->_ring = 0;
  }


/* Create a new a stackpool.
 */
extern peak_task_runloop_stackpool peak_task_runloop_stackpool_create(void);


/* Add an item to the stackpool.
 */
extern void peak_task_runloop_stackpool_push(peak_task_runloop_stackpool sp,
                                             peak_task_runloop rl);


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_TASK_RUNLOOP_STACKPOOL_H_ */

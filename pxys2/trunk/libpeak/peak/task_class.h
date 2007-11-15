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
 * $Id: task_class.h,v 1.3 2004/01/09 00:24:38 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TASK_CLASS_H_
#define INCLUDED_PEAK_TASK_CLASS_H_

#include "task_private.h"
#include "task_runloop.h"
#include "task_runloop_stackpool.h"
#include "semaphore.h"

#include "atomic.h"
#include "spinlock.h"

struct __peak_task
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task_runloop _master;         /* Master runloop */
  peak_task_runloop_stackpool _pool; /* Pool of runloops (1 per thread) */
  peak_semaphore _hdlsem;            /* Parallel processing semaphore */
  peak_semaphore _runsem;            /* RunLoops semaphore */
  peak_semaphore _exsem;             /* Exclusivity semaphore */
  peak_semaphore _excosem;           /* Exclusivity completion semaphore */
  peak_spinlock_t _exlock;           /* Lock used for exclusivity stuffs */
  peak_spinlock_t _lock;             /* Task's structure access general lock */
  int _ncpus;                        /* Number of CPUs (cache) */
  int _nthreads;                     /* Number of task's threads */
  int _nevts;                        /* Number of events being processed */
  peak_atomic_t _nruns;              /* Number of running loops */
  peak_atomic_t _nexs;               /* Number of exclusivity requests */
  uint32_t _flags;                   /* Task's flags */
  peak_engine _engine;               /* Task's associated engine */
  peak_timer _tifirst;               /* Fast access to first pending timer */
  peak_timer _tiroot;                /* Root of task's timer splay tree */
  double _current_time;              /* Time cache */
  };


#endif /* INCLUDED_PEAK_TASK_CLASS_H_ */

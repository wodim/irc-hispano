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
 * $Id: task_runloop.h,v 1.1.1.1 2003/12/30 02:29:34 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TASK_RUNLOOP_H_
#define INCLUDED_PEAK_TASK_RUNLOOP_H_

#include "internal.h"
#include "mem_pool.h"
#include "task_private.h"

/* Note: task_runloop is a private/internal class
 */
typedef struct __peak_task_runloop * peak_task_runloop;


struct __peak_task_runloop       /* "private" class */
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task _task;               /* Owner task */
  peak_mem_pool _oppool;         /* Pool task_op allocator for this runloop */
  peak_task_op _sched_ophead;    /* [SCHEDULER] Head of task_ops queue */
  peak_task_op * _sched_optailp; /* [SCHEDULER] Tail ptr of task_ops queue */
  peak_task_op _runloop_ophead;  /* [RUNLOOP] Head of task_ops queue */
  int _exclusivity;              /* Task's exclusivity boolean */
  };

#if defined(__cplusplus)
extern "C" {
#endif

__private_extern__ peak_task_runloop _peak_task_runloop_create(peak_task task);

__private_extern__ void * _peak_task_runloop_start(void *arg);

__private_extern__ void   _peak_task_runloop_quit(peak_task_runloop rl);

__private_extern__ void   _peak_task_runloop_op_schedule(peak_task_runloop rl,
                                                         peak_task_op op);

__private_extern__ void   _peak_task_runloop_run(peak_task_runloop rl,
                                                 int master);

#if defined(__cplusplus)
}
#endif


#endif /* INCLUDED_PEAK_TASK_RUNLOOP_H_ */

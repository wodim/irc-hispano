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
 * $Id: task_private.h,v 1.5 2004/01/12 01:52:11 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TASK_PRIVATE_H_
#define INCLUDED_PEAK_TASK_PRIVATE_H_

#include "engine_client.h"
#include "internal.h"
#include <sys/time.h>

enum
    {
    TASK_FLAG_DEFAULT       = 0,
    TASK_FLAG_RUNNING       = 1,
    TASK_FLAG_ON_TIME       = 2
    };

#define TASK_IS_RUNNING(t)     ((t)->_flags & TASK_FLAG_RUNNING)
#define TASK_SET_RUNNING(t)     (t)->_flags |= TASK_FLAG_RUNNING
#define TASK_SET_STOPPED(t) \
    (t)->_flags &= ~(TASK_FLAG_RUNNING|TASK_FLAG_ON_TIME)

#define TASK_IS_ON_TIME(t)     ((t)->_flags & TASK_FLAG_ON_TIME)
#define TASK_SET_ON_TIME(t)     (t)->_flags |= TASK_FLAG_ON_TIME
#define TASK_SET_NOT_ON_TIME(t) (t)->_flags &= ~TASK_FLAG_ON_TIME


/* TASK_OP definition */

#define TASK_OP_TAG_INVALID     0
#define TASK_OP_TAG_MANAGER     1
#define TASK_OP_TAG_IOEVENT     2
#define TASK_OP_TAG_TIMER       3


typedef struct __peak_task_op
  {
  struct __peak_task_op *next;
  int tag;
  union
    {
    /* TAG MANAGER */
    int cmd_id;
    
    /* TAG IOEVENT */
    struct
      {
      peak_engine_client client;
      int event;
      int info;
      } ioevent;
    
    /* TAG TIMER */
    peak_timer timer;
    
    } utag;
  } __peak_task_op;

typedef __peak_task_op * peak_task_op;

#define TASK_OP_MANAGER_GEN(op, rl, i) do { \
    op = (peak_task_op)peak_mem_pool_new(rl->_oppool); \
    op->next = NULL; \
    op->tag = TASK_OP_TAG_MANAGER; \
    op->utag.cmd_id = (i); } while (0)

#define TASK_OP_IOEVENT_GEN(op, rl, c, e, i) do { \
    op = (peak_task_op)peak_mem_pool_new(rl->_oppool); \
    op->next = NULL; \
    op->tag = TASK_OP_TAG_IOEVENT; \
    op->utag.ioevent.client = (peak_engine_client)peak_retain(c); \
    op->utag.ioevent.event = e; \
    op->utag.ioevent.info = i; } while (0)

#define TASK_OP_TIMER_GEN(op, rl, ti) do { \
    op = (peak_task_op)peak_mem_pool_new(rl->_oppool); \
    op->next = NULL; \
    op->tag = TASK_OP_TAG_TIMER; \
    op->utag.timer = (peak_timer)peak_retain(ti); } while (0)

#define TASK_MANAGER_CMD_ID_QUIT  (-1)
#define TASK_MANAGER_CMD_ID_ABORT (-2)

/* TIMER definition */

/* Timer's modes */
enum
  {
  PEAK_TIMER_MODE_ONCE = 0,
  PEAK_TIMER_MODE_REQUEUE = 1 << 0
  };

/* Peak structure for timers */
struct __peak_timer
  {
  PEAK_STRUCT_RT_HEADER;
  
  /* Splay tree node children pointers. */
  struct __peak_timer *left;
  struct __peak_timer *right;
  
  double _fire;                 /* next fire date */
  double _interval;
  uint32_t _mode;               /* we can't trust double "special values" */
  peak_timer_callback _callout;
  void *_context;
  peak_task _task;              /* associated task */
  };


#define PEAK_TIMER_COMPARE(t1, t2) \
    ((t1) == (t2) \
      ? 0 \
      : ((t1)->_fire == (t2)->_fire \
        ? ((intptr_t)t1 < (intptr_t)t2 \
          ? -1 \
          : 1) \
        : (t1)->_fire < (t2)->_fire \
          ? -1 \
          : 1))

#if defined(__cplusplus)
extern "C" {
#endif

__private_extern__ peak_task _peak_task_create();

__private_extern__ void _peak_task_schedule_engine_client(
  peak_task task,
  peak_engine_client c);

__private_extern__ void _peak_task_unschedule_engine_client(
  peak_task task,
  peak_engine_client c);

__private_extern__ int _peak_task_timer_schedule_fire(peak_task task);

__private_extern__ void _peak_task_op_ioevent_schedule(
  peak_task task,
  peak_engine_client c, int event,
  int info);

__private_extern__ void _peak_task_process_pending_events(
  peak_task task,
  int nevts);

/* TIMERS */

PEAK_CLASS_BASE_DECLARE_VIRTUAL(timer);

__private_extern__ void _peak_task_timer_lock_configure(peak_task task,
                                                        peak_timer ti,
                                                        double fire,
                                                        double interval);

__private_extern__ int _peak_task_time(time_t *nowp);

__private_extern__ int _peak_task_time_float(double *nowp);

__private_extern__ void _peak_task_set_on_time(double now);

__private_extern__ int _peak_task_timer_mswait(peak_task task);

__private_extern__ struct timeval* _peak_task_timer_tvwait(peak_task task,
                                                           struct timeval *tv);
__private_extern__ struct timespec* _peak_task_timer_tswait(
  peak_task task,
  struct timespec *ts);

__private_extern__ void _peak_task_timer_fire(peak_task task);

/* Defined in timer.c */

__private_extern__ double _peak_timer_now(void);

__private_extern__ void _peak_timer_configure(peak_timer ti, double fire,
                                              double interval);

__private_extern__ void _peak_timer_rearm(peak_timer ti, double t_limit);

__private_extern__ void _peak_timer_fire(peak_timer ti);

__private_extern__ double _peak_timer_expire_relative(peak_timer ti);

__private_extern__ struct timeval* _peak_timer_expire_relative_tv(
  peak_timer ti,
  struct timeval *tv);

__private_extern__ struct timespec* _peak_timer_expire_relative_ts(
  peak_timer ti,
  struct timespec *ts);

#if defined(__cplusplus)
}
#endif


#endif /* INCLUDED_PEAK_TASK_PRIVATE_H_ */

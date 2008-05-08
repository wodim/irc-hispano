/* PEAK Library - Test file
 *
 * Copyright (c) 2004
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
#define RCSID "$Id: t_signal2.c,v 1.1 2004/01/09 20:19:48 mbuna Exp $"

/* t_signal2.c - Signal handling test program.
 *
 * Purpose
 * =======
 * Configure some signals and see if they are handled properly on a multi-
 * threaded task. Use a peak timer to send signals.
 *
 * Returns
 * =======
 * 0 -> test ok
 * 1 -> peak signal handler creation failed (error)
 * 2 -> bad signum parameter (error)
 * 3 -> bad context pointer management (error)
 * 4 -> one or more signals lost (warning)
 * 5 -> other timer error (error)
 * 6 -> unable to get or set task info (error)
 *
 */

#define T_ERR_SIGNAL_CREATE_FAILED 1
#define T_ERR_SIGNAL_BAD_SIGNUM    2
#define T_ERR_SIGNAL_BAD_CONTEXT   3
#define T_WARN_SIGNAL_LOST         4
#define T_ERR_TIMER_ERROR          5
#define T_ERR_TASK_INFO            6

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define SIGNAL1_CONTEXT ((void*)0x9f6942e8)

static peak_atomic_t cnt = PEAK_ATOMIC_INIT(0);
static int sent;

static void
signal_event_cb(peak_signal i, int value, void *context)
  {
  peak_atomic_inc(&cnt);
  
  if (peak_atomic_read(&cnt) > 30)
    peak_task_break(peak_task_self());
  
  if (value != SIGHUP)
    exit(T_ERR_SIGNAL_BAD_SIGNUM);
  if (context != SIGNAL1_CONTEXT)
    exit(T_ERR_SIGNAL_BAD_CONTEXT);
  }


static void
timer_callback(peak_timer ti, void *context)
  {
  kill(getpid(), SIGHUP);
  sent++;
#if 0
  kill(getpid(), SIGHUP);
  sent++;
#endif
  }

int
main(int argc, char *argv[])
  {
  peak_task task = peak_task_self();
  peak_signal si;
  peak_timer ti;
  
  /* Get autoconfigured threads number. */
  int val = -255;
  if (peak_task_get_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val) || val <= 0)
    return T_ERR_TASK_INFO;
  
  /* If on UP, force several threads. */
  if (val == 1 && (val = 2)
      && peak_task_set_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val))
    return T_ERR_TASK_INFO;
  
  /* Create signal object. */
  si = peak_signal_create(SIGHUP, signal_event_cb, SIGNAL1_CONTEXT);
  if (si == 0)
    exit(T_ERR_SIGNAL_CREATE_FAILED);
  
  /* Schedule task. */
  peak_signal_schedule(si, task);
  
  /* Create repeating timer.
   */
  ti = peak_timer_create(0.4, 0.04, timer_callback, 0);
  if (ti == 0)
    exit(T_ERR_TIMER_ERROR);
  
  /* Add timer to task.
   */
  peak_task_timer_add(task, ti);
  
  /* Run task.
   */
  peak_task_run(task);
  peak_release(si);
  peak_release(ti);
  
  if (peak_atomic_read(&cnt) != sent)
    exit(T_WARN_SIGNAL_LOST);
  
  return 0;
  }


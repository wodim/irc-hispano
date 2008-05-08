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
#define RCSID "$Id: t_atomic1.c,v 1.1 2004/01/09 20:19:48 mbuna Exp $"

/* t_atomic1.c - Atomic primitives verification program.
 *
 * Purpose
 * =======
 * Configure the (main) task with several threads and test atomic access and
 * modifications on PEAK's atomic integer type (peak_atomic_t). This file
 * tests the static initialization, peak_atomic_inc(), peak_atomic_dec()
 * and peak_atomic_read(). See t_atomic2.c for more tests.
 *
 * Returns
 * =======
 * 0 -> test ok
 * 1 -> misc timer error (error)
 * 2 -> unable to set task info (error)
 * 3 -> atomic failure/result not coherent (error)
 *
 */

#define T_ERR_TIMER_ERROR          1
#define T_ERR_TASK_INFO            2
#define T_ERR_ATOMIC_FAILURE       3

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>
#include <stdio.h>

#define CNT_START 857
#define MAX_ITER 100000
#define N_TIMERS 60

static peak_atomic_t cnt = PEAK_ATOMIC_INIT(CNT_START);
static time_t start_time;

static void
timer_inc_callback(peak_timer ti, void *context)
  {
  int i;
  for (i = MAX_ITER; i-- > 0; )
    peak_atomic_inc(&cnt);
  }

static void
timer_dec_callback(peak_timer ti, void *context)
  {
  int i;
  for (i = MAX_ITER; i-- > 0; )
    peak_atomic_dec(&cnt);
  }

static void
timer_cnt_callback(peak_timer ti, void *context)
  {
  /* Print current cnt number in STDOUT.log */
  fprintf(stdout, "%ld t_atomic1: cnt=%d\n", peak_time(),
          peak_atomic_read(&cnt));
  fflush(stdout);
  
  /* Break task after working for 5 seconds */
  if (peak_time() - start_time > 5)
    peak_task_break(peak_task_self());
  }

int
main(int argc, char *argv[])
  {
  peak_task task = peak_task_self();
  peak_timer ti;
  int i;
  
  /* Force several threads. */
  int val = 8;
  if (peak_task_set_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val))
    return T_ERR_TASK_INFO;
  
  start_time = peak_time();
  
  /* Create some repeating timers with a very low repeat delay to improve
   * threads concurrency for this test.
   */
  for (i = 0; i < N_TIMERS; i++)
    {
    ti = peak_timer_create(0.5, 0.000001,
                           i & 1 ? timer_dec_callback : timer_inc_callback,
                           0);
    if (ti == 0)
      exit(T_ERR_TIMER_ERROR);
    
    /* Add timer to task.
     */
    peak_task_timer_add(task, ti);
    }
  
  /* Create monitor */
  ti = peak_timer_create(0.5, 0.5, timer_cnt_callback, 0);
  if (ti == 0)
    exit(T_ERR_TIMER_ERROR);
  peak_task_timer_add(task, ti);
  
  /* Run task.
   */
  peak_task_run(task);
  
  /* Verify coherence */
  if ((peak_atomic_read(&cnt) - CNT_START) % MAX_ITER)
    exit(T_ERR_ATOMIC_FAILURE);
  
  return 0;
  }


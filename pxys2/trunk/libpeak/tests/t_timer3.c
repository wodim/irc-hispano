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
#define RCSID "$Id: t_timer3.c,v 1.5 2004/01/18 19:41:44 mbuna Exp $"

/* t_timer3.c - Simple timer test program.
 *
 * Purpose
 * =======
 * Very similar to t_timer2.c but adds thread support constraint: it adds
 * several timers in a threaded task (forced if needed) and see if they
 * work properly.
 *
 * Returns
 * =======
 * 0 -> test ok
 * 1 -> timer creation failed (error)
 * 2 -> bad context pointer management (error)
 * 3 -> unable to get or set task info (error)
 * 
 * Alarm
 * =====
 * 10 secs -> timer didn't fire or repeat?
 */

#define T_ERR_CREATE_FAILED 1
#define T_ERR_BAD_CONTEXT   2
#define T_ERR_TASK_INFO     3


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>
#include <stdio.h>
#include <stdlib.h>

#define T_NTIMERS 5000

static peak_atomic_t t_cnt = PEAK_ATOMIC_INIT(T_NTIMERS);
static unsigned char t_arraychk[T_NTIMERS];

static void
timer3_callback(peak_timer ti, void *context)
  {
  int idx = (int)context;
  
  if (idx < 0 || idx >= T_NTIMERS)
    exit(T_ERR_BAD_CONTEXT);
  
  /* Let them fire + repeat 5 times, then release them.
   */
  if (t_arraychk[idx]++ == 5)
    {
    peak_atomic_dec(&t_cnt);
    peak_release(ti);
    }
  
  if (peak_atomic_read(&t_cnt) == 0)
    peak_task_break(peak_task_self());
  }

int
main(int argc, char *argv[])
  {
  peak_task task = peak_task_self();
  peak_timer *ti_array = (peak_timer *)peak_allocate(sizeof(peak_timer)
                                                     * T_NTIMERS);
  int i;
  
  /* Get autoconfigured threads number. */
  int val = -255;
  if (peak_task_get_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val) || val <= 0)
    return T_ERR_TASK_INFO;
  
  /* If on UP, force several threads. */
  if (val == 1 && (val = 2)
      && peak_task_set_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val))
    return T_ERR_TASK_INFO;
  
  /* Create T_NTIMERS (5000!) repeating timers, using the index as a fake
   * context pointer.
   */
  for (i = 0; i < T_NTIMERS; i++)
    {
    ti_array[i] = peak_timer_create(1.0, 0.5, timer3_callback, (void*)i);
    if (ti_array[i] == 0)
      exit(T_ERR_CREATE_FAILED);
    
    /* Add timer to task.
     */
    peak_task_timer_add(task, ti_array[i]);
    }
  
  /* Run task.
   */
  peak_task_run(task);
  peak_deallocate(ti_array);
  return 0;
  }


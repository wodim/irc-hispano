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
#define RCSID "$Id: t_timer2.c,v 1.3 2004/01/18 19:41:44 mbuna Exp $"

/* t_timer2.c - Simple timer test program.
 *
 * Purpose
 * =======
 * Add several timers and see if they work (one thread task).
 *
 * Returns
 * =======
 * 0 -> test ok
 * 1 -> timer creation failed (error)
 * 2 -> bad context pointer management (error)
 * 
 * Alarm
 * =====
 * 10 secs -> timer didn't fire or repeat?
 */

#define T_ERR_CREATE_FAILED 1
#define T_ERR_BAD_CONTEXT   2

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>
#include <stdio.h>
#include <stdlib.h>

#define T_NTIMERS 5000

static int t_cnt = T_NTIMERS;
static unsigned char t_arraychk[T_NTIMERS];

static void
timer2_callback(peak_timer ti, void *context)
  {
  int idx = (int)context;
  
  if (idx < 0 || idx >= T_NTIMERS)
    exit(T_ERR_BAD_CONTEXT);
  
  /* Let them fire + repeat 5 times, then release them.
   */
  if (t_arraychk[idx]++ == 5)
    {
    t_cnt--;
    peak_release(ti);
    }
  
  if (t_cnt == 0)
    peak_task_break(peak_task_self());
  }

int
main(int argc, char *argv[])
  {
  peak_task task = peak_task_self();
  peak_timer *ti_array = (peak_timer *)peak_allocate(sizeof(peak_timer)
                                                     * T_NTIMERS);
  int i;
  
  /* Force use of 1 thread max only. */
  int val = 1;
  peak_task_set_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val);
  
  /* Create T_NTIMERS (5000!) repeating timers, using the index as a fake
   * context pointer.
   */
  for (i = 0; i < T_NTIMERS; i++)
    {
    ti_array[i] = peak_timer_create(1.0, 0.5, timer2_callback, (void*)i);
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


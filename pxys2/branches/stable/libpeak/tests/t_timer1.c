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
#define RCSID "$Id: t_timer1.c,v 1.2 2004/01/05 18:46:16 mbuna Exp $"

/* t_timer1.c - Simple timer test program.
 *
 * Purpose
 * =======
 * Add one timer and see if it works.
 *
 * Returns
 * =======
 * 0 -> test ok
 * 1 -> timer creation failed (error)
 * 2 -> bad context pointer management (error)
 * 
 * Alarm
 * =====
 * 5 secs -> timer didn't fire or repeat?
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>
#include <stdlib.h>

#define TIMER1_CONTEXT1 ((void*)0x67fbc904)
#define TIMER1_CONTEXT2 ((void*)0X886e0483)

static void
timer1_callback(peak_timer ti, void *context)
  {
  if (context == TIMER1_CONTEXT1)
    peak_timer_set_context(ti, TIMER1_CONTEXT2);
  else if (context == TIMER1_CONTEXT2)
    {
    peak_release(ti);
    peak_task_break(peak_task_self());
    }
  else
    exit(2);
  }

int
main(int argc, char *argv[])
  {
  peak_task task = peak_task_self();
  peak_timer ti;
  
  /* Force use of 1 thread max only. */
  int val = 1;
  peak_task_set_info(task, PEAK_TASK_FLAVOR_NTHREADS, &val);
  
  /* Create 1 sec repeating timer.
   */
  ti = peak_timer_create(1.0, 1.0, timer1_callback, TIMER1_CONTEXT1);
  if (ti == 0)
    exit(1);
  
  /* Add timer to task.
   */
  peak_task_timer_add(task, ti);
  
  /* Run task.
   */
  peak_task_run(task);
  return 0;
  }


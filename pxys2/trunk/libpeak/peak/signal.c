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
 */
#define RCSID "$Id: signal.c,v 1.2 2004/01/08 19:03:54 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/signal.h>

#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#include <unistd.h>

#include "engine_client.h"
#include "internal.h"
#include "task_private.h"

static void __peak_signal_init(peak_signal i, va_list vp, void *ctcx);
static void __peak_signal_finalize(peak_signal i);
static void __peak_signal_event_process(peak_signal i, int ioevent, int data);

struct __peak_signal
  {
  PEAK_STRUCT_ENGINE_CLIENT_HEADER;
  peak_signal_event_callback _cb;
  void *_context;
  };

PEAK_CLASS_DECLARE(signal, engine_client);

peak_signal
peak_signal_create(int signum, peak_signal_event_callback cb,
                      void *context)
  {
  return PEAK_CLASS_CONSTRUCT3(signal, signum, cb, context);
  }

static void
__peak_signal_init(peak_signal i, va_list vp, void *ctcx)
  {
  int signum = va_arg(vp, int);
  
  _peak_engine_client_configure((peak_engine_client)i, signum, CS_SIGNAL,
      (peak_engine_client_event_process_func)&__peak_signal_event_process);
  
  i->_cb = va_arg(vp, peak_signal_event_callback);
  i->_context = va_arg(vp, void*);
  }

static void
__peak_signal_finalize(peak_signal i)
  {
  }

/* Scheduling */

void
peak_signal_schedule(peak_signal i, peak_task task)
  {
  _peak_task_schedule_engine_client(task, (peak_engine_client)i);
  }

void
peak_signal_unschedule(peak_signal i, peak_task task)
  {
  _peak_task_unschedule_engine_client(task, (peak_engine_client)i);
  }

/* Convenience */
int
peak_signal_ignore(int signum)
  {
  struct sigaction act;
  act.sa_handler = SIG_IGN;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  return sigaction(signum, &act, 0);
  }

/* Processing */

static void
__peak_signal_event_process(peak_signal i, int ioevent, int data)
  {
  assert(ioevent == IOEVENT_SIGNAL);
  
  (*i->_cb)(i, data, i->_context);
  }


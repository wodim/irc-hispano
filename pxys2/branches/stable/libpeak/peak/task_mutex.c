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
 */
#define RCSID "$Id: task_mutex.c,v 1.1.1.1 2003/12/30 02:29:31 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/task.h>

#include "task_private.h"

#include <assert.h>
#include <pthread.h>


struct __peak_task_mutex
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task _task;               /* Owner task */
  pthread_mutex_t _mutex;
  };

static void __peak_task_mutex_init(peak_task_mutex mutex, va_list vp,
                                   void *ctcx);
static void __peak_task_mutex_finalize(peak_task_mutex mutex);

PEAK_CLASS_BASE_DECLARE(task_mutex);


peak_task_mutex
peak_task_mutex_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(task_mutex, task);
  }

static void
__peak_task_mutex_init(peak_task_mutex mutex, va_list vp, void *ctcx)
  {
  mutex->_task = va_arg(vp, peak_task);
  mutex->_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  }

static void
__peak_task_mutex_finalize(peak_task_mutex mutex)
  {
  pthread_mutex_destroy(&mutex->_mutex);
  }

void
peak_task_mutex_lock(peak_task_mutex mutex)
  {
  if (_peak_is_threaded)
    pthread_mutex_lock(&mutex->_mutex);
  }

void
peak_task_mutex_trylock(peak_task_mutex mutex)
  {
  if (_peak_is_threaded)
    pthread_mutex_trylock(&mutex->_mutex);
  }

void
peak_task_mutex_unlock(peak_task_mutex mutex)
  {
  if (_peak_is_threaded)
    pthread_mutex_unlock(&mutex->_mutex);
  }

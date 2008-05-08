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
#define RCSID "$Id: semaphore.c,v 1.3 2004/01/18 19:41:44 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "semaphore.h"
#include "internal.h"

#include <assert.h>
#ifdef HAVE_SYS_SIGNAL_H
/* Fix for darwin 6 in POSIX mode */
#include <sys/signal.h>
#endif
#include <pthread.h>
#ifdef HAVE_MACH_MACH_INIT_H
#include <mach/mach_init.h>
#endif
#ifdef HAVE_MACH_TASK_H
#include <mach/task.h>
#endif
#ifdef HAVE_MACH_SEMAPHORE_H
#include <mach/semaphore.h>
#endif

#ifdef PEAK_USE_MACH_SEMAPHORE
struct __peak_semaphore
  {
  PEAK_STRUCT_RT_HEADER;
  semaphore_t _mach_sem;
  };

#else

struct __peak_semaphore
  {
  PEAK_STRUCT_RT_HEADER;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
  int _value;
  };

#endif

static void __peak_semaphore_init(peak_semaphore s, va_list vp);
static void __peak_semaphore_finalize(peak_semaphore s);

PEAK_CLASS_BASE_DECLARE(semaphore);

peak_semaphore
peak_semaphore_create(int value)
  {
  return PEAK_CLASS_CONSTRUCT1(semaphore, value);
  }

static void
__peak_semaphore_init(peak_semaphore s, va_list vp)
  {
#ifdef PEAK_USE_MACH_SEMAPHORE
  kern_return_t rv;
  
  rv = semaphore_create(mach_task_self(), &s->_mach_sem, SYNC_POLICY_FIFO,
                        va_arg(vp, int));
  assert(rv == KERN_SUCCESS);
#else
  pthread_mutex_init(&s->_mutex, NULL);
  pthread_cond_init(&s->_cond, NULL);
  s->_value = va_arg(vp, int);
#endif
  }

static void
__peak_semaphore_finalize(peak_semaphore s)
  {
#ifdef PEAK_USE_MACH_SEMAPHORE
  kern_return_t rv;
  
  rv = semaphore_destroy(mach_task_self(), s->_mach_sem);
  assert(rv == KERN_SUCCESS);
#else
  pthread_cond_destroy(&s->_cond);
  pthread_mutex_destroy(&s->_mutex);
#endif
  }

int
peak_semaphore_signal(peak_semaphore semaphore)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_signal(semaphore->_mach_sem);
#else
  pthread_mutex_lock(&semaphore->_mutex);
  if (++semaphore->_value <= 0)
    result = pthread_cond_signal(&semaphore->_cond);
  pthread_mutex_unlock(&semaphore->_mutex);
#endif
  return result;
  }

int
peak_semaphore_signal_all(peak_semaphore semaphore)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_signal_all(semaphore->_mach_sem);
#else
  pthread_mutex_lock(&semaphore->_mutex);
  if (semaphore->_value < 0)
    result = pthread_cond_broadcast(&semaphore->_cond);
  semaphore->_value = 0;
  pthread_mutex_unlock(&semaphore->_mutex);
#endif
  return result;
  }

int
peak_semaphore_wait(peak_semaphore semaphore)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_wait(semaphore->_mach_sem);
#else
  pthread_mutex_lock(&semaphore->_mutex);
  if (--semaphore->_value < 0)
    result = pthread_cond_wait(&semaphore->_cond, &semaphore->_mutex);
  pthread_mutex_unlock(&semaphore->_mutex);
#endif
  return result;
  }

int
peak_semaphore_timedwait(peak_semaphore semaphore, peak_timespec_t wait_time)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_signal(semaphore->_mach_sem);
#else
  pthread_mutex_lock(&semaphore->_mutex);
  if (--semaphore->_value < 0)
    result = pthread_cond_timedwait(&semaphore->_cond, &semaphore->_mutex,
                                    &wait_time);
  pthread_mutex_unlock(&semaphore->_mutex);
#endif
  return result;
  }

int
peak_semaphore_wait_signal(peak_semaphore wait_semaphore,
                           peak_semaphore signal_semaphore)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_wait_signal(wait_semaphore->_mach_sem,
                                 signal_semaphore->_mach_sem);
#else
  pthread_mutex_lock(&wait_semaphore->_mutex);
  pthread_mutex_lock(&signal_semaphore->_mutex);
  if (++signal_semaphore->_value <= 0)
    result = pthread_cond_signal(&signal_semaphore->_cond);
  pthread_mutex_unlock(&signal_semaphore->_mutex);
  if (--wait_semaphore->_value < 0)
    result = pthread_cond_wait(&wait_semaphore->_cond,
                               &wait_semaphore->_mutex);
  pthread_mutex_unlock(&wait_semaphore->_mutex);
#endif
  return result;
  }

int
peak_semaphore_timedwait_signal(peak_semaphore wait_semaphore,
                                peak_semaphore signal_semaphore,
                                peak_timespec_t wait_time)
  {
  int result = 0;
  
#ifdef PEAK_USE_MACH_SEMAPHORE
  result = semaphore_timedwait_signal(wait_semaphore->_mach_sem,
                                      signal_semaphore->_mach_sem, wait_time);
#else
  pthread_mutex_lock(&wait_semaphore->_mutex);
  pthread_mutex_lock(&signal_semaphore->_mutex);
  if (++signal_semaphore->_value <= 0)
    result = pthread_cond_signal(&signal_semaphore->_cond);
  pthread_mutex_unlock(&signal_semaphore->_mutex);
  if (--wait_semaphore->_value < 0)
    result = pthread_cond_timedwait(&wait_semaphore->_cond,
                                    &wait_semaphore->_mutex, &wait_time);
  pthread_mutex_unlock(&wait_semaphore->_mutex);
#endif
  return result;
  }

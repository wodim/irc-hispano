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
#define RCSID "$Id: timer.c,v 1.6 2004/01/12 01:52:11 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/timer.h>
#include <peak/time.h>
#include "internal.h"
#include "task_private.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

/* "FreeBSD is constricting for the max timeval value passed to select(),
 * which might return EINVAL. The following value should be ok on machines
 * with standard HZ (100). It seems to avoid some additional calculations
 * and roundings on Linux, too."
 */
#define TV_MAX_SECONDS ((long) (LONG_MAX / 100) - 1)


static void __peak_timer_init(peak_timer ti, va_list vp, void *ctcx);
static void __peak_timer_finalize(peak_timer ti);

PEAK_CLASS_BASE_DEFINE_VIRTUAL(timer);

/* Local
 */

static inline double
__peak_timer_abmax(void)
  {
  return (double)LONG_MAX;
  }

static inline  double
__peak_timer_tv2ab(struct timeval *tv)
  {
  return (double)tv->tv_sec + 1.0E-6 * (double)tv->tv_usec;
  }

static inline struct timeval
__peak_timer_ab2tv(double date)
  {
  struct timeval tv;
  double floor_date = (double)((long)date);
  tv.tv_sec = (date < 0. || TV_MAX_SECONDS <= date)
    ? TV_MAX_SECONDS
    : (time_t)floor_date;
  tv.tv_usec = (long)((date - floor_date) * 1.0E+6);
  return tv;
  }

static inline struct timespec
__peak_timer_ab2ts(double date)
  {
  struct timespec ts;
  double floor_date = (double)((long)date);
  ts.tv_sec = (date < 0. || (double)TV_MAX_SECONDS <= date)
    ? TV_MAX_SECONDS
    : (time_t)floor_date;
  ts.tv_nsec = (long)((date - floor_date) * 1.0E+9);
  return ts;
  }

peak_timer
peak_timer_create(double fire, double interval, peak_timer_callback callout,
                  void *context)
  {
  return PEAK_CLASS_CONSTRUCT4(timer, fire, interval, callout, context);
  }

static void
__peak_timer_init(peak_timer ti, va_list vp, void *ctcx)
  {
  double fire, interval;
  double fire_date;
    
  fire = va_arg(vp, double);
  interval = va_arg(vp, double);
    
  ti->left = NULL;
  ti->right = NULL;
    
  if (fire < -0.5)
    {
    ti->_fire = __peak_timer_abmax();
    }
  else
    {
    fire_date = peak_time_float() + fire;
        
    if (__peak_timer_abmax() < fire_date)
      ti->_fire = __peak_timer_abmax();
    else
      ti->_fire = fire_date;
    }
  if (interval <= 0. || __peak_timer_abmax() < interval)
    {
    ti->_interval = __peak_timer_abmax();
    ti->_mode = PEAK_TIMER_MODE_ONCE;
    }
  else
    {
    ti->_interval = interval;
    ti->_mode = PEAK_TIMER_MODE_REQUEUE;
    }
  
  ti->_callout = va_arg(vp, peak_timer_callback);
  ti->_context = va_arg(vp, void *);
  ti->_task = NULL;
  }

static void
__peak_timer_finalize(peak_timer ti)
  {
  if (ti->_task)
    peak_task_timer_remove(ti->_task, ti);
  }

void
peak_timer_configure(peak_timer ti, double fire, double interval)
  {
  _peak_task_timer_lock_configure(ti->_task, ti, fire, interval);
  }

__private_extern__ void
_peak_timer_configure(peak_timer ti, double fire, double interval)
  {
  if (fire < -0.5)
    {
    ti->_fire = __peak_timer_abmax();
    }
  else
    {
    double fire_date = peak_time_float() + fire;
        
    if (__peak_timer_abmax() < fire)
      ti->_fire = __peak_timer_abmax();
    else
      ti->_fire = fire_date;
    }

  /* -1.0 ? do not repeat then */
  if (interval <= 0.0 || __peak_timer_abmax() < interval)
    {
    ti->_interval = __peak_timer_abmax();
    ti->_mode = PEAK_TIMER_MODE_ONCE;
    }
  else
    {
    ti->_interval = interval;
    ti->_mode = PEAK_TIMER_MODE_REQUEUE;
    }
  }

double
peak_timer_get_firetime(peak_timer ti)
  {
  return ti->_fire;
  }

double
peak_timer_get_interval(peak_timer ti)
  {
  return ti->_interval;
  }

void*
peak_timer_get_context(peak_timer ti)
  {
  return ti->_context;
  }

void
peak_timer_set_context(peak_timer t, void *context)
  {
  t->_context = context;
  }

/* Private
 */

__private_extern__ void
_peak_timer_rearm(peak_timer ti, double t_limit)
  {
  ti->_fire += ti->_interval;
  
  if (ti->_fire > __peak_timer_abmax()) /* sanity */
    ti->_fire = __peak_timer_abmax();
  
  /* Adjust fire date if needed (for slow machine, etc.) */
  if (ti->_fire < t_limit)
    ti->_fire = t_limit;
  }

__private_extern__ void
_peak_timer_fire(peak_timer ti)
  {
  (*ti->_callout)(ti, ti->_context);
  }

__private_extern__ double
_peak_timer_expire_relative(peak_timer ti)
  {
  struct timeval tv_now;
  double result;
  
  /* Don't use current time here. We could but... next events or timers
   * will be much more accurate if events we've just processed took
   * some time.
   */
  gettimeofday(&tv_now, NULL);
  
  result = ti->_fire - __peak_timer_tv2ab(&tv_now);
  
#if 0
  if (result < 0.0)
    printf("_peak_timer_expire_relative: %f now: %f\n", ti->_fire,
           __peak_timer_tv2ab(&tv_now));
#endif
  return (result > 0.0) ? result : 0.0;
  }

__private_extern__ struct timeval*
_peak_timer_expire_relative_tv(peak_timer ti, struct timeval *tv)
  {
  *tv = __peak_timer_ab2tv(_peak_timer_expire_relative(ti));
  return tv;
  }

__private_extern__ struct timespec*
_peak_timer_expire_relative_ts(peak_timer ti, struct timespec *ts)
  {
  *ts = __peak_timer_ab2ts(_peak_timer_expire_relative(ti));
  return ts;
  }


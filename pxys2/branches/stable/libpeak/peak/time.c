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
#define RCSID "$Id: time.c,v 1.3 2004/01/04 23:49:20 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/time.h>
#include "internal.h"
#include "task_private.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h> /* gettimeofday */
#include <unistd.h>
#ifdef HAVE_TZFILE_H
#include <tzfile.h>
#endif

/* Accurate only for the past couple of centuries;
 * that will probably do. (from FreeBSD)
 */
#define ISLEAP(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

#ifndef EPOCH_YEAR
#define EPOCH_YEAR 1970
#elif EPOCH_YEAR != 1970
/* Uh ho, funky. */
#warning EPOCH_YEAR != 1970
#endif

static inline  double
__peak_time_tv2ab(struct timeval *tv)
  {
  return (double)tv->tv_sec + 1.0E-6 * (double)tv->tv_usec;
  }

static double
_peak_time_float()
  {
  struct timeval tv;
  
  gettimeofday(&tv, NULL);
  return __peak_time_tv2ab(&tv);
  }

time_t
peak_time()
  {
  time_t now;
  double nowf;
  
  /* Get from task's cache if possible. */
  if (_peak_task_time(&now))
    return now;
  
  /* No, so get it now. */
  nowf = _peak_time_float();
  
  /* Cache it. */
  _peak_task_set_on_time(nowf);
  return (time_t)nowf;
  }

double
peak_time_float()
  {
  double now;
  
  /* Get from task's cache if possible. */
  if (_peak_task_time_float(&now))
    return now;
  
  /* No, so get it now. */
  now = _peak_time_float();
  
  /* Cache it. */
  _peak_task_set_on_time(now);
  return now;
  }

static const uint16_t days_before_month[16] =
  {
  0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 0, 0
  };

static inline uint16_t
__days_before_month(int month, int32_t year, int leap)
  {
  return days_before_month[month] + (2 < month && leap);
  }

peak_time_date
peak_time_get_date(double t, peak_tz tz)
  {
  peak_time_date d;
  double tc = t; /* time with tz correction */
  int32_t absdays, ydays, b, y;
  int leap, tci;
  
  if (tz)
    tc += (double)peak_tz_get_gmt_offset(tz, (time_t)t);
  
  absdays = (int32_t)(tc / 86400.0);
  b = absdays / 146097;                         /* 400 * 365.2425 */
  y = b * 400 + EPOCH_YEAR;
  
  absdays -= b * 146097;
  while (absdays < 0)
    {
    y--;
    absdays += 365 + ISLEAP(y);
    }
  
  for (ydays = 365 + ISLEAP(y); ydays <= absdays;
       ydays = 365 + ISLEAP(y))
    {
    y++;
    absdays -= ydays;
    }
  
  d.year = y;
  d.month = absdays / 33 + 1;
  leap = ISLEAP(y);
  
  while (__days_before_month(d.month + 1, y, leap) <= absdays)
    d.month++;
  
  d.day = absdays - __days_before_month(d.month, y, leap) + 1;
  
  d.second = tc - (double)((int)(tc / 60.0)) * 60.0;
  if (d.second < 0.0)
    d.second += 60.0;
  
  tci = (int)tc;
  tci /= 60;
  d.minute = tci - (tci / 60) * 60;
  if (d.minute < 0)
    d.minute += 60;
  
  tci /= 60;
  d.hour = tci - (tci / 24) * 24;
  if (d.hour < 0)
    d.hour += 24;
  
  return d; /* copy struct */
  }

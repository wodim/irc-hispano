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
 * $Id: time.h,v 1.2 2004/01/04 23:49:20 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TIME_H_
#define INCLUDED_PEAK_TIME_H_

/*!
 * @defgroup time Time
 * 
 * This little Time module has been added in order to provide optimized
 * simple time related methods. Systems provide similar features but you
 * are invited to use PEAK's ones instead.
 */

#include <peak/stdint.h>
#include <peak/tz.h>
#include <time.h>

/*!
 * @ingroup time
 * @brief Gregorian date structure.
 *
 * Structure used to represent a date using the Gregorian calendar.
 */
typedef struct
  {
  /*! full year */
  int32_t year;
  /*! month number 1-12 */
  int8_t month;
  /*! day number 1-31 */
  int8_t day;
  /*! hour 0-23 */
  int8_t hour;
  /*! minute 0-59 */
  int8_t minute;
  /*! second */
  double second;
  } peak_time_date;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup time
 * @brief Get current time.
 *
 * @return  The peak_time() function returns the value of time in seconds
 * since 0 hours, 0 minutes, 0 seconds, January 1, 1970, Coordinated
 * Universal Time (the "epoch").
 */
extern time_t peak_time(void);

/*!
 * @ingroup time
 * @brief Get current time with precision.
 *
 * @return   The peak_time_float() function returns the value of time in
 *           seconds since 0 hours, 0 minutes, 0 seconds, January 1, 1970,
 *           Coordinated Universal Time (the "epoch").
 */
extern double peak_time_float(void);

/*!
 * @ingroup time
 * @brief Converts an Unix time value into a Gregorian date.
 *
 * @param t  Unix time value in second. Pass the result of peak_time_float()
 *           to get current date.
 * @param tz Optional Time Zone reference. See peak_tz_create(). If provided,
 *           necessary time zone's conversions will be performed.
 * 
 * @return   The Gregorian date equivalent for the specified time value since
 *           0 hours, 0 minutes, 0 seconds, January 1, 1970, Coordinated
 *           Universal Time (the "epoch").
 */
extern peak_time_date peak_time_get_date(double t, peak_tz tz);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_TIME_H_ */

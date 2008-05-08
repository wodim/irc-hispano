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
 * $Id: timer.h,v 1.2 2004/01/04 17:38:49 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TIMER_H_
#define INCLUDED_PEAK_TIMER_H_

/*!
 * @defgroup timer Timer
 * 
 * Lightweight timer/repeater support within a task. Units are floats in
 * seconds with an internal precision up to usec, but subjects to client's
 * event handlers so don't expect a lot.\n
 * For best results, please use in conjunction with the Time module
 * (\p peak_time*).
 */

/*!
 * @ingroup timer
 * @brief Opaque timer pointer type.
 */
typedef struct __peak_timer * peak_timer;

/*!
 * @ingroup timer
 * @brief Timer callback.
 */
typedef void (*peak_timer_callback)(peak_timer t, void *context);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup timer
 * @brief Create a timer.
 * 
 * Note that the timer isn't activated until you add it to the task of your
 * choice (usually peak_task_self()) with peak_task_timer_add().
 * 
 * @param fire     Relative fire time in second. 0 means immediate fire (as
 *                 soon as possible, usually during the next event loop).
 *                 A value of -1.0 means never fire, and can be useful to
 *                 temporarily "disable" a timer.
 * @param interval Repeat interval in second. Use -1.0 for a one-shot timer.
 *                 Value must be strictly positive for repeating timer.
 * @param callout  A pointer to your timer callback function which is
 *                 triggered when the timer fires.
 * @param context  An extra application-defined pointer that will be passed
 *                 to your timer callback function (it's not used by the
 *                 library).
 *
 * @return         A newly allocated \p peak_timer reference or \p NULL if the
 *                 timer cannot be created.
 */
extern peak_timer peak_timer_create(double fire,
                                    double interval,
                                    peak_timer_callback callout,
                                    void *context);

/*!
 * @ingroup timer
 * @brief (Re)configure a timer.
 * 
 * @param t        The timer reference to configure.
 * @param fire     Relative fire time in second. 0 means immediate fire (as
 *                 soon as possible, usually during the next event loop).
 *                 A value of -1.0 means never fire, and can be useful to
 *                 temporarily "disable" a timer.
 * @param interval Repeat interval in second. Use -1.0 for a one-shot timer.
 *                 Value must be strictly positive for repeating timer.
 */
extern void peak_timer_configure(peak_timer t, double fire, double interval);

/*!
 * @ingroup timer
 * @brief Get timer's fire date time.
 * 
 * @param t        The timer reference.
 * 
 * @return         Absolute time in second before the timer fires (if it
 *                 is added to a task).
 */
extern double peak_timer_get_firetime(peak_timer t);

/*!
 * @ingroup timer
 * @brief Get timer's repeat-interval time.
 * 
 * @param t        The timer reference.
 * 
 * @return         Interval time in second.
 */
extern double peak_timer_get_interval(peak_timer t);

/*!
 * @ingroup timer
 * @brief Get timer's extra application-defined context.
 * 
 * @param t        The timer reference.
 * 
 * @return         Context pointer.
 */
extern void* peak_timer_get_context(peak_timer t);

/*!
 * @ingroup timer
 * @brief Change the context pointer of a timer.
 * 
 * @param t        The timer reference.
 * @param context  An extra application-defined pointer that will be passed
 *                 to your timer callback function (it's not used by the
 *                 library).
 */
extern void peak_timer_set_context(peak_timer t, void *context);


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_TIMER_H_ */

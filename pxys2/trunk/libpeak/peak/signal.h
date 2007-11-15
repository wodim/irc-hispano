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
 * $Id: signal.h,v 1.4 2004/01/08 19:03:54 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_SIGNAL_H_
#define INCLUDED_PEAK_SIGNAL_H_

#include <peak/task.h>

/*!
 * @defgroup signal Signal notifications
 * The PEAK Library wraps signal interruptions to provide safe signal
 * notifications (via a callback). Now you can consider handling signals
 * like any other events (stream, timer, etc.). The API is very simple here,
 * when you want to handle a signal (defined by a number), use
 * peak_signal_create() and specify a pointer to a callback function. Then,
 * schedule a task for signal notifications using peak_signal_schedule().
 * For convenience, especially for use with SIGPIPE, a peak_signal_ignore()
 * method is provided to ignore a specified signal.
 */

/*!
 * @ingroup signal
 * @brief Opaque type for the signal object.
 */
typedef struct __peak_signal * peak_signal;

/*!
 * @ingroup signal
 * @brief Signal notification callback type.
 */
typedef void (*peak_signal_event_callback)(peak_signal i, int value,
                                           void *context);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup signal
 * @brief Create a new signal notification object.
 * 
 * @param signum  The signal number to handle.
 * @param cb      A pointer to your notification callback function that handles
 *                signal events that may occur once scheduled.
 * @param context An extra application-defined pointer that will be passed
 *                to your signal event callback function (it's not used by the
 *                library).
 *
 * @return        A newly allocated peak_signal reference or NULL if an error
 *                was encountered.
 */
extern peak_signal peak_signal_create(int signum,
                                      peak_signal_event_callback cb,
                                      void *context);

/*!
 * @ingroup signal
 * @brief Ignore a signal.
 * 
 * Note that some signals can't be handled nor ignored, like SIGKILL.
 * 
 * @param signum  The signal number to ignore.
 *
 * @retval 0      The operation was successful.
 * @retval -1     The signal can't be ignored, error might be set to indicate
 *                the error (just like sigaction(2) does).
 */
extern int peak_signal_ignore(int signum);

/*!
 * @ingroup signal
 * @brief Schedule a task for signal event notification.
 * 
 * This will enable you to receive signal notifications (using the provided
 * callback in peak_signal_create()) within the specified task. If the task
 * has several threads, one of its thread calls the callback.
 * 
 * @param  i      The signal object.
 * @param task    The task to schedule (usually peak_task_self()).
 */
extern void peak_signal_schedule(peak_signal i, peak_task task);

/*!
 * @ingroup signal
 * @brief Unschedule a task for signal event notification.
 * 
 * This will disable notifications for the specified signal object.
 * 
 * @param  i      The signal object.
 * @param task    The task to unschedule (usually peak_task_self()).
 */
extern void peak_signal_unschedule(peak_signal i, peak_task task);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_SIGNAL_H_ */

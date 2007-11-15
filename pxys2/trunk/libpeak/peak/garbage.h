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
 * $Id: garbage.h,v 1.1.1.1 2003/12/30 02:29:14 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_GARBAGE_H_
#define INCLUDED_PEAK_GARBAGE_H_

/*!
 * @defgroup garbage Garbage collector
 * 
 * @par
 * In brief: this module adds a feature to delay the destruction of pointers
 * of different sizes, thus allowing the application to reuse some pointers
 * not really deleted yet. See the \p mem_pool module for another type of
 * allocation optimization feature.
 * 
 * @par
 * A simple garbage collector (with explicit collection) which provides
 * asynchronous deletion. It is probably only useful when an application
 * could avoid to recreate the same objects, when all objects are not of
 * the same size. If it's better for you to just keep them a few seconds
 * or minutes before really delete them, in case you want to reuse them, so
 * this module is probably for you. When it's time to really delete an object,
 * the library calls your callback: if the object has been reused since, just
 * ignore the call... or really delete it otherwise.
 * 
 * @par
 * This module has been created in order to optimize allocations of IRC
 * channels, which are allocated structures of different sizes (different
 * names) and commonly deleted and re-allocated just after (part/join of an
 * empty channel). I am not sure it will suit for something else.
 * 
 * @par
 * Usage is very easy: initialize a garbage collector object with
 * peak_garbage_create() providing a generic deletion callback for all
 * collected objects, then call peak_garbage_collect() for each object you
 * want to delete using the collector. The objects will be automatically
 * deleted (you shouldn't predict the deletion, as the frequency is only a
 * hint - note it's usually less because all collected objects are always
 * deleted at the same time). If needed, you can force all objects deletion
 * with the peak_garbage_vacuum() function.
 */

#include <peak/task.h>

/*!
 * @ingroup garbage
 * @brief Opaque garbage collector pointer type.
 */
typedef struct __peak_garbage * peak_garbage;

/*!
 * @ingroup garbage
 * @brief Deletion's callback definition.
 */
typedef void (*peak_garbage_callback)(peak_garbage g, void *ptr,
                                      void *context);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup garbage
 * @brief Create a garbage object.
 * 
 * @param max_ptrs  Max pointers that the garbage object can collect at the
 *                  same time. Must be strictly positive. If reached, a
 *                  vacuuming is performed and the object properly collected.
 * @param frequency Frequency hint in seconds at which you want to do the
 *                  vacuuming (deletions), it's usually less because when
 *                  vacuuming the callback is called for all objects already
 *                  collected.
 * @param task      Task to schedule, can be \p NULL to use current task.
 * @param callout   A pointer to your deletion callback function which is
 *                  triggered when automatic of manual vacuuming.
 * @param context   An extra application-defined pointer that will be passed
 *                  to your garbage callback function.
 * 
 * @return          A newly allocated \p peak_garbage reference or \p NULL
 *                  if the object cannot be created.
 */
extern peak_garbage peak_garbage_create(
  int max_ptrs,
  double frequency,
  peak_task task,
  peak_garbage_callback callout,
  void *context
);

/*!
 * @ingroup garbage
 * @brief Collect a pointer and schedules the garbage's task for deletion.
 * 
 * Guaranteed O(1) unless \a max_ptrs is reached.
 * 
 * @param g         The garbage object reference.
 * @param ptr       Pointer to collect for deletion.
 */
extern void peak_garbage_collect(
  peak_garbage g,
  void *ptr
);

/*!
 * @ingroup garbage
 * @brief Get the current number of pointers collected for deletion.
 * 
 * @param g         The garbage object reference.
 * 
 * @return          Collected pointers count.
 */
extern int peak_garbage_get_collected_count(
  peak_garbage g
  );

/*!
 * @ingroup garbage
 * @brief Perform manual vacuuming.
 * 
 * @param g        The garbage object reference.
 */
extern void peak_garbage_vacuum(
  peak_garbage g
);


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_GARBAGE_H_ */

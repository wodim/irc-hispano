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
 * $Id: task.h,v 1.3 2004/01/10 14:33:26 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TASK_H_
#define INCLUDED_PEAK_TASK_H_

/*!
 * @defgroup task Task
 *
 * In the PEAK library, a task is an abstract object that is eventually
 * made of several threads to process asynchronous events... It's a bit like
 * a micro process. Then when you schedule a task for a stream for example,
 * it will takes the advantage (or the disadvantage) of having several
 * threads available to process events.
 */
/*!
 * @ingroup task
 * @defgroup task_common Task common
 *
 * Task common stuffs.
 */
/*!
 * @ingroup task
 * @defgroup task_sync Thread's synchronization
 *
 * @par
 * A task, in the PEAK's context, might process events in parallel, with
 * the help of kernel threads (pthread(3)).
 *
 * @par
 * The PEAK library provides a set of functions for kernel threads
 * synchronization within a task. If the task has only one thread to process
 * events, these functions do nothing. Otherwise, they are prefered to more
 * low level primitives as some optimizations can be done within a task. Be
 * careful if you need to synchronize with other threads which are not
 * related to a PEAK's task (eg. another thread in your program created
 * explicitely): in that case you need more general primitives.
 *
 * @par
 * When in doubt, for example if you are not very familiar with threads
 * synchronization primitives like mutex, conditions or semaphores, you have
 * always the choice to configure your PEAK's task to not use more
 * than one thread (see peak_task_set_info()) Then, all the problems are over
 * because events aren't processed in parallel anymore and you have only one
 * excecution stream. For most applications, that will do... several events
 * libraries act this way and are already very efficient. But, although you
 * don't block when processing events (and you must not!), if your event's
 * processings are still consuming significative CPU time (eg. cache search
 * for each event, huge I/O's, etc), PEAK task capabilities of processing
 * multiple events in the same time has shown global improvement for the
 * application.
 *
 * @par
 * Note: Synchronization between several PEAK's tasks are not yet available,
 * as you can only have one task in the current version of the library.
 */
/*!
 * @ingroup task
 * @defgroup task_timer Task and timer
 *
 * Timers are special objects which must be explicitely added to the task.
 * See peak_timer_create() and other timer functions for more info.
 */
#include <peak/stdint.h>
#include <peak/timer.h>


/*!
 * @ingroup task_common
 * @brief Opaque task type.
 *
 * One task to rule them all...
 */
typedef struct __peak_task * peak_task;

/*!
 * @ingroup task_sync
 * @brief Opaque task lock type.
 */
typedef struct __peak_task_lock * peak_task_lock;

/*!
 * @ingroup task_sync
 * @brief Opaque task mutex type.
 */
typedef struct __peak_task_mutex * peak_task_mutex;


/*!
 * @ingroup task_common
 * @brief Task info's flavors.
 */
enum _peak_task_flavor_e
  {
  /*!
   * @ingroup task_common
   * @brief Specify the number of threads a task should run.
   *
   * @par Info's value
   *
   * Use 0 for automatic detection which depends on the number of available
   * CPUs when you *run* the application.
   */
  PEAK_TASK_FLAVOR_NTHREADS,
  /*!
   * @ingroup task_common
   * @brief Specify the max number of open files allowed.
   * 
   * This defines only the max number of open files allowed to open by the
   * PEAK library and doesn't count application-level file descriptors.
   * WARNING: This is not true when using the basic select() engine, due to an
   * implementation limitation.
   * 
   * @par Info's value
   * 
   * Use 0 to set the default specified at the library compile time
   * (usually 256).
   */
  PEAK_TASK_FLAVOR_MAXFDS
  };

/*!
 * @ingroup task_common
 * @brief Task info's flavors.
 * 
 * See the documentation for the enumeration #_peak_task_flavor_e.
 */
typedef enum _peak_task_flavor_e peak_task_flavor_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup task_common
 * @brief Get the owning task of the current execution stream.
 * 
 * A task can have several threads, but this function will reference the
 * same task in all of them.
 */
extern peak_task peak_task_self(void);

/*!
 * @ingroup task_common
 * @brief Get task information.
 * 
 * @param task   The task reference.
 * @param flavor One of the available flavor.
 * @param info   A pointer to a varying array of int, depending on the flavor.
 *               You must properly allocate it.
 *
 * @return 0 if successful
 */
extern int peak_task_get_info(
  peak_task task,
  peak_task_flavor_t flavor,
  int *info
  );

/*!
 * @ingroup task_common
 * @brief Set task information.
 * 
 * @param task   The task reference.
 * @param flavor One of the available flavor.
 * @param info   A pointer to a varying array of int, depending on the flavor,
 *               containing the value you want to set.
 *
 * @return 0 if successful
 */
extern int peak_task_set_info(
  peak_task task,
  peak_task_flavor_t flavor,
  int *info
  );

/*!
 * @ingroup task_common
 * @brief Get task's underlying engine name.
 * 
 * @param task   The task reference.
 *
 * @return       A pointer to the name of engine (constant).
 */
extern const char * peak_task_get_engine_name(
  peak_task task
  );

/*!
 * @ingroup task_common
 * @brief Enter and process the event loop.
 * 
 * Block the task's master thread if no event to process nor timers to fire
 * are found. If called with peak_task_self(), blocks the current thread and
 * use it as the master thread.\n
 * Returns only if there is nothing to process (ie. no timer, no registered
 * signal, no object scheduled) or after a call to peak_task_break().\n
 * Usually called once at the beginning (after initializations).
 *
 * @param task The task to run (usually peak_task_self()).
 */
extern void peak_task_run(peak_task task);

/*!
 * @ingroup task_common
 * @brief Terminate task's event loop.
 * 
 * This is done asynchronously: the engine waits for events being currently
 * processed and the blocked peak_task_run() exits.
 *
 * @param task The task to break (usually peak_task_self()).
 */
extern void peak_task_break(peak_task task);

/*!
 * @ingroup task_sync
 * @brief Acquire task execution exclusivity.
 *
 * This function acquires temporary exclusive task execution among all task's
 * threads. It's usually called at the beginning of an event-callback to avoid
 * conflicts with other events (as they might be processed in parallel), when
 * you deal with a lot of shared data in the callback.\n
 * This function does nothing if the task has only one running thread, and
 * slow down event processing otherwise. Exclusive execution is garanteed
 * in the whole callback, in fact, it's garanteed until the next event or
 * timer on the current task.\n
 * Note there is no parameter at all: you can only acquire exclusive execution
 * on the current task.
 */
extern void peak_task_exclusivity(void);

/*!
 * @ingroup task_sync
 * @brief Create a task's lock.
 * 
 * Allow you to lock threads in order to create critical regions, for
 * example, within a task, with peak_task_lock_acquire(),
 * peak_task_lock_release(), etc. Like other PEAK's objets, you can destroy
 * a lock with peak_release(). THEY ARE ACTIVE LOCKS FOR SMALL DATA
 * STRUCTURES PROTECTION ONLY. If you think you will have almost no collision
 * for a critical region, they are for you.
 * 
 * @param task  The task to associated with the lock
 *              (usually peak_task_self()).
 *
 * @result A new \p peak_task_lock reference.
 */
extern peak_task_lock peak_task_lock_create(peak_task task);

/*!
 * @ingroup task_sync
 * @brief Acquire a task's lock.
 *
 * This function acquires a task's lock. If the lock is already owned by
 * another thread of the task, then the calling thread will block.
 * However, this function does nothing if the task has only one thread.
 * If a deadlock is detected, the program will abort, so be careful with
 * recursions! Possibly, use peak_task_lock_try().\n
 * Note that you can only acquire a lock for the current task.
 */
extern void peak_task_lock_acquire(peak_task_lock lock);

/*!
 * @ingroup task_sync
 * @brief Try to acquire a task's lock.
 *
 * This function attempts to acquire a task's lock without blocking. The
 * return value indicatees whether the lock was acquired.
 * This function does nothing if the task has only one running thread and
 * in that case always succeeds.\n
 * Note that you can only try a lock for the current task.
 *
 * @retval 1 if the lock was acquired
 * @retval 0 if the lock is already owned (busy)
 */
extern int peak_task_lock_try(peak_task_lock lock);

/*!
 * @ingroup task_sync
 * @brief Release a task's lock.
 *
 * This function releases a task's lock, so one another thread of the task
 * can acquire it. However, this function does nothing if the task has only
 * one thread.\n
 * Note that you can only release a lock for the current task.
 */
extern void peak_task_lock_release(peak_task_lock lock);

/*!
 * @ingroup task_sync
 * @brief Hand-off a task's lock.
 *
 * To assume the lock's ownership, this function passes a task's lock from
 * the calling thread to another thread of the task. The lock must be already
 * owned by the calling thread. If no other thread is waiting for acquiring
 * the lock, this call will block until it happens. If the task has only one
 * running thread, this function will abort the program (because if you use
 * this function, you want a special synchronization behaviour that can't
 * happen with one thread only). See peak_task_set_info() to properly
 * configure your task.\n
 * Note that you can only hand-off a lock for the current task.
 */
extern void peak_task_lock_handoff(peak_task_lock lock);



/*!
 * @ingroup task_sync
 * @brief Create a task's mutex.
 * 
 * Allow you to create critical regions within a task, with
 * peak_task_mutex_lock(), peak_task_mutex_unlock(), etc. Like other PEAK's
 * objets, you can destroy a mutex with peak_release(). Task's mutex are
 * more suitable to create large mutual exclusion regions than task's locks
 * are, as they shouldn't spinlock much. However, if you need to protect basic
 * and small data structures, it might be lighter to use task's locks (eg.
 * for a simple operation like "object->i++;" ).
 * 
 * @param task  The task to associated with the mutex
 *              (usually peak_task_self()).
 *
 * @result A new \p peak_task_mutex reference.
 */
extern peak_task_mutex peak_task_mutex_create(peak_task task);

/*!
 * @ingroup task_sync
 * @brief Lock a task's mutex.
 *
 * This function locks a task's \a mutex. If the mutex is already locked
 * then the calling thread will block until the mutex becomes available.
 * However, this function does nothing if the task has only one thread.
 * If a deadlock is detected, the program will abort. Possibly, use
 * peak_task_mutex_trylock().\n
 * Note that you can only lock a mutex for the current task.
 */
extern void peak_task_mutex_lock(peak_task_mutex mutex);

/*!
 * @ingroup task_sync
 * @brief Try to lock a task's mutex.
 *
 * This function locks a task's \a mutex. If the mutex is already locked
 * then the calling thread will block until the mutex becomes available.
 * However, this function does nothing if the task has only one thread.\n
 * Note that you can only lock a mutex for the current task.
 */
extern void peak_task_mutex_trylock(peak_task_mutex mutex);

/*!
 * @ingroup task_sync
 * @brief Unlock a task's mutex.
 *
 * This function unlocks a task's \a mutex. However, this function does
 * nothing if the task has only one thread.\n
 * Note that you can only unlock a mutex for the current task.
 */
extern void peak_task_mutex_unlock(peak_task_mutex mutex);



/*!
 * @ingroup task_timer
 * @brief Add a peak timer.
 * 
 * Add a previously configured peak timer to the specified task. You can
 * create a timer with peak_timer_create(). This function increases the
 * timer's retain count, so you can safely call peak_release() on it if needed.
 * If the timer is already added to a task, then it is first removed then
 * added.
 *
 * @param task The task reference.
 * @param ti   The timer to add.
 */
extern void peak_task_timer_add(peak_task task, peak_timer ti);

/*!
 * @ingroup task_timer
 * @brief Remove a peak timer.
 * 
 * Remove a timer previously added to the task and decrease its retain count.
 * If the retain count is 0 then the timer is deleted. If the timer wasn't
 * previously added to the task, then this function does nothing.
 *
 * @param task The task reference.
 * @param ti   The timer to remove.
 */
extern void peak_task_timer_remove(peak_task task, peak_timer ti);


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_TASK_H_ */

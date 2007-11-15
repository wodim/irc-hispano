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
#define RCSID "$Id: task.c,v 1.10 2005/02/02 10:07:20 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/task.h>
#include <peak/time.h>

#include "task_class.h"
#include "task_private.h"
#include "task_runloop.h"
#include "timer_tree.h"
#include "init_private.h"
#include "utilities.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


extern int _peak_is_threaded;


static void __peak_task_init(peak_task task, va_list vp, void *ctcx);
static void __peak_task_finalize(peak_task task);

static void __peak_task_autospawn(peak_task task);
static void __peak_task_respawn(peak_task task, int n);
static void __peak_task_spawn(peak_task task, int n);

static void __peak_task_timer_insert(peak_task task, peak_timer ti);
static void __peak_task_timer_remove(peak_task task, peak_timer ti);
static int  __peak_task_timer_prefetch(double now, peak_timer n,
                                       peak_timer *tp, int idx);
static void __peak_task_op_timer_schedule_fire(peak_task task, peak_timer ti);




PEAK_CLASS_BASE_DECLARE(task);


__private_extern__ peak_task
_peak_task_create()
  {
  return PEAK_CLASS_CONSTRUCT0(task);
  }

static void
__peak_task_init(peak_task task, va_list vp, void *ctcx)
  {
  task->_pool = peak_task_runloop_stackpool_create();
  task->_hdlsem = peak_semaphore_create(0);
  task->_runsem = peak_semaphore_create(0);
  task->_exsem = peak_semaphore_create(0);
  task->_excosem = peak_semaphore_create(0);
  task->_exlock = PEAK_SPINLOCK_INITIALIZER;
  task->_lock = PEAK_SPINLOCK_INITIALIZER;
  
#ifdef PEAK_SIMULATED_NCPUS
  task->_ncpus = PEAK_SIMULATED_NCPUS;
#else
  if ((task->_ncpus = peak_get_ncpus()) < 1)
    task->_ncpus = 1;
#endif
  
  task->_nevts = 0;
  peak_atomic_set(&task->_nruns, 0);
  peak_atomic_set(&task->_nexs, 0);
  
  /* Create underlying task's engine. */
  task->_engine = _peak_engine_create(task);
  
  /* Initialize timer splay tree. */
  task->_tifirst = NULL;
  task->_tiroot = NULL;
  
  task->_flags = TASK_FLAG_DEFAULT;
  
  /* "Built-in" master runloop. */
  task->_master = _peak_task_runloop_create(task);
  peak_task_runloop_stackpool_push(task->_pool, task->_master);
  task->_nthreads = -1; /* Special value: autospawn wanted */
  
  _peak_init_thread_task(task);
  }

static void
__peak_task_finalize(peak_task task)
  {
  PEAK_HALT; /* shouldn't happen *for now* */
  peak_release(task->_engine);
  peak_release(task->_excosem);
  peak_release(task->_exsem);
  peak_release(task->_runsem);
  peak_release(task->_hdlsem);
  peak_release(task->_pool);
  }

static inline void
__peak_task_lock(peak_task task)
  {
  _peak_spinlock_lock(&task->_lock);
  }

static inline void
__peak_task_unlock(peak_task task)
  {
  _peak_spinlock_unlock(&task->_lock);
  }

int
peak_task_get_info(peak_task task, peak_task_flavor_t flavor, int *info)
  {
  peak_task_exclusivity(); /* Way to dangerous otherwise... */
  
  switch (flavor)
    {
    case PEAK_TASK_FLAVOR_NTHREADS:
      info[0] = task->_nthreads == -1 ? task->_ncpus : task->_nthreads;
      break;
    case PEAK_TASK_FLAVOR_MAXFDS:
      info[0] = _peak_engine_get_maxfds(task->_engine);
      break;
    default:
      return -1;
    }
  return 0;
  }

int
peak_task_set_info(peak_task task, peak_task_flavor_t flavor, int *info)
  {
  peak_task_exclusivity(); /* Way to dangerous otherwise... */
  
  switch (flavor)
    {
    case PEAK_TASK_FLAVOR_NTHREADS:    /* Configure task's threads number */
      if (info[0] == 0)                /* 0 means auto configuration */
        {
        __peak_task_autospawn(task);
        return 0;
        }
      else if (info[0] > 0)
        {
        __peak_task_respawn(task, info[0]);
        return (task->_nthreads == info[0]) ? 0 : -1;
        }
      break;
      
    case PEAK_TASK_FLAVOR_MAXFDS:      /* Configure max number of open files */
      if (info[0] > 0)
        return _peak_engine_set_maxfds(task->_engine, info[0]);
      break;
      
    default:
      break;
    }
  return -1;
  }

const char *
peak_task_get_engine_name(peak_task task)
  {
  return _peak_engine_get_name(task->_engine);
  }

void
peak_task_run(peak_task task)
  {
  if (task->_nthreads == -1) /* Special autospawn default value */
    {
    task->_nthreads = 1;     /* Only the master thread has been created */
  __peak_task_autospawn(task);
    }
  TASK_SET_RUNNING(task);
  _peak_engine_loop(task->_engine);
  }

void
peak_task_break(peak_task task)
  {
  _peak_engine_break(task->_engine);
  TASK_SET_STOPPED(task);
  }

static void
__peak_task_autospawn(peak_task task)
  {
  if (task->_nthreads == -1) /* Special autospawn default value */
    task->_nthreads = 1;     /* Only the master thread has been created */
  
  /* In most applications, the best we have is number of threads == ncpus */
  __peak_task_respawn(task, task->_ncpus);
  }

static void
__peak_task_respawn(peak_task task, int n)
  {
  if (task->_nthreads == -1) /* Special autospawn default value */
    task->_nthreads = 1;     /* Only the master thread has been created */
  
  if (task->_nthreads == n)
    return; /* alright */
  
  if (task->_nthreads < n)
    {
    /* Spawn missing threads */
    __peak_task_spawn(task, n - task->_nthreads);
    }
  else
    {
    /* If too many threads, make some quit... */
    do
      {
      _peak_task_runloop_quit(peak_task_runloop_stackpool_top(task->_pool));
      peak_task_runloop_stackpool_pop(task->_pool);
      }
    while (task->_nthreads != n);
    }
  }

static void
__peak_task_spawn(peak_task task, int n)
  {
  while (n--)
    {
    pthread_t t;
    
    if (pthread_create(&t, NULL, _peak_task_runloop_start, task) == -1)
      {
      PEAK_WARN("pthread_create failed; cannot spawn as desired");
      break;
      }
    peak_semaphore_wait(task->_hdlsem);
    }
  task->_nthreads = peak_task_runloop_stackpool_count(task->_pool);
  if (task->_nthreads > 1 && !_peak_is_threaded)
    _peak_is_threaded = 1;
  }


/* Thread's synchronization: Acquire exclusive task execution.
 * If you try to understand, see along with:
 * _peak_task_process_pending_events()
 * __peak_task_runloop_run().
 */
void
peak_task_exclusivity()
  {
  peak_task task;
  peak_task_runloop rl;
  int alone;
  
  if (!_peak_is_threaded)
    return;
  
  task = peak_task_self();
#if 0
  /* Enable this when peak supports multiple tasks */
  if (task->_nthreads <= 1)
    return;
#endif
  
  /* If no or only one event being processed, don't bother :) */
  if (task->_nevts <= 1)
    return;
  
  /* Multiple events, multiple threads configuration */
  if ((rl = _peak_task_runloop_self()) == NULL)
    PEAK_HALT;
  
  _peak_spinlock_lock(&task->_exlock);
  peak_atomic_inc(&task->_nexs);             /* Increment exclusive count */
  alone = (peak_atomic_read(&task->_nruns) == 1); /* Do the test */
  _peak_spinlock_unlock(&task->_exlock);     /* Test done, unlock */
  if (alone)
    peak_atomic_dec(&task->_nexs);           /* We are the only one running! */
  else
    peak_semaphore_wait(task->_exsem);       /* Wait for exclusivity. */
  
  /* EXCLUSIVE EXECUTION */
  
  /* Set runloop exclusivity bit. */
  rl->_exclusivity = 1;
  }

__private_extern__ void
_peak_task_schedule_engine_client(peak_task task, peak_engine_client c)
  {
  if (c->_task != NULL)
    PEAK_HALT;
  
  _peak_engine_add_client(task->_engine, c);
  
  c->_task = task;
  }

__private_extern__ void
_peak_task_unschedule_engine_client(peak_task task, peak_engine_client c)
  {
  if (c->_task == NULL)
    PEAK_HALT;
  
  _peak_engine_remove_client(task->_engine, c);
  
  c->_task = NULL;
  }

void
peak_task_timer_add(peak_task task, peak_timer ti)
  {
  __peak_task_lock(task);
  
  if (ti->_task)
    {
    assert(ti->_task == task);
    __peak_task_timer_remove(ti->_task, ti);
    }
  else
    peak_retain(ti);
  ti->_task = task;
  __peak_task_timer_insert(task, ti);
  
  __peak_task_unlock(task);
  }

void
peak_task_timer_remove(peak_task task, peak_timer ti)
  {
  __peak_task_lock(task);
  
  if (ti->_task == task)
    {
    __peak_task_timer_remove(task, ti);
    ti->_task = NULL;
    peak_release(ti);
    }
  
  __peak_task_unlock(task);
  }

__private_extern__ void
_peak_task_timer_lock_configure(peak_task task, peak_timer ti,
                                double fire, double interval)
  {
  if (task)
    {
    __peak_task_lock(task);
    
    /* Be sure to remove it from the pending timers list first. */
    __peak_task_timer_remove(task, ti);
    }
  
  /* Configure timer */
  _peak_timer_configure(ti, fire, interval);
  
  /* Apply necessary timer's requeueing if the timer t is associated
   * with a task.
   */
  if (task)
    {
    __peak_task_timer_insert(task, ti);
    __peak_task_unlock(task);
    }
  }

static void
__peak_task_timer_insert(peak_task task, peak_timer ti)
  {
  assert(ti->left == NULL && ti->right == NULL);
  
  if (task->_tiroot)
    {
    int cmp;
    
    /* Splay tree around timer. */
    _peak_timer_tree_splay(ti, &task->_tiroot, NULL, NULL);
    
    cmp = PEAK_TIMER_COMPARE(task->_tiroot, ti);
    
    /* Insert new timer at the root of the tree. */
    if (cmp > 0)
      {
      ti->right = task->_tiroot;
      ti->left = ti->right->left;
      ti->right->left = NULL;
      }
    else
      {
      ti->left = task->_tiroot;
      ti->right = ti->left->right;
      ti->left->right = NULL;
      }
    
    /* Update our fast first timer accessor. */
    if (!ti->left)
      task->_tifirst = ti;
    }
  else
    {
    /* Tree was empty. */
    ti->left = ti->right = NULL;
    task->_tifirst = ti;
    }
  
  task->_tiroot = ti;
  }

static void
__peak_task_timer_remove(peak_task task, peak_timer ti)
  {
  if (!task->_tiroot)
    return; /* nothing to remove at all */
  
  _peak_timer_tree_splay(ti, &task->_tiroot, NULL, NULL);
  
  if (task->_tiroot != ti)
    return; /* Ignore for convenience, if not found. */
  
  /* Update tifirst accessor. */
  if (ti == task->_tifirst)
    {
    peak_timer node;
    assert(ti->left == NULL); /* We were the first, damn it. */
    
    node = ti->right;
    if (node)
      {
      /* Find the leftmost element of the right subtree.  */
      while (node->left)
        node = node->left;
      task->_tifirst = node;
      }
    else
      {
      assert(ti->left == NULL && ti->right == NULL);
      task->_tifirst = NULL;
      }
    }
  
  if (ti->left)
    {
    task->_tiroot = ti->left;
    
    if (ti->right)
      {
      peak_timer left = ti->left;
      
      /* Hang the old root right child off the right-most leaf of the
       * left child.
       */
      while (left->right)
        left = left->right;
      left->right = ti->right;
      }
    }
  else
    task->_tiroot = ti->right;
  
  ti->left = NULL;
  ti->right = NULL;
  }

__private_extern__ int
_peak_task_time(time_t *nowp)
  {
  peak_task task = peak_task_self();
  
  /* Use current time (cached) of associated task, if possible
   */
  if (task && TASK_IS_ON_TIME(task))
    {
    *nowp = (time_t)task->_current_time;
    return 1;
    }
  return 0;
  }

__private_extern__ int
_peak_task_time_float(double *nowp)
  {
  peak_task task = peak_task_self();
  
  /* Use current time (cached) of associated task, if possible
   */
  if (task && TASK_IS_ON_TIME(task))
    {
    *nowp = task->_current_time;
    return 1;
    }
  return 0;
  }

__private_extern__ void
_peak_task_set_on_time(double now)
  {
  peak_task task = peak_task_self();
  if (task && TASK_IS_RUNNING(task))
    {
    task->_current_time = now;
    task->_flags |= TASK_FLAG_ON_TIME;
    }
  }

__private_extern__ int
_peak_task_timer_mswait(peak_task task)
  {
  if (task->_tifirst == NULL)
    return -1; /* INFTIM */
  
  /* +1 for ceiling */
  return (int)((_peak_timer_expire_relative(task->_tifirst) * 1000.0) + 1);
  }

struct timeval*
_peak_task_timer_tvwait(peak_task task, struct timeval *tv)
  {
#if 1
  return task->_tifirst
    ? _peak_timer_expire_relative_tv(task->_tifirst, tv)
    : NULL;
#else
  struct timeval *itv =
    (task->_tifirst) ? _peak_timer_expire_relative_tv(task->_tifirst, tv)
                     : NULL;
  
  if (itv)
    printf("_peak_task_timer_timewait: tv->tv_sec=%d tv->tv_usec=%d\n",
           itv->tv_sec, itv->tv_usec);
  
  return itv;
#endif
  }

__private_extern__ struct timespec*
_peak_task_timer_tswait(peak_task task, struct timespec *ts)
  {
#if 1
  return task->_tifirst
    ? _peak_timer_expire_relative_ts(task->_tifirst, ts)
    : NULL; 
#else
  struct timespec *_ts;
  
  if (task->_tifirst == NULL)
    return NULL;
  
  _ts = _peak_timer_expire_relative_ts(task->_tifirst, ts);
  
  if (_ts)
    printf("ts->ts_sec=%d ts->nsec=%d\n", _ts->tv_sec, _ts->tv_nsec);
  return _ts;
#endif
  }

/* Max number of timers that can be fetched/scheduled in one event loop.
 */
#define TIMER_PREFETCH_MAX 100

/* Check every node below n following an in-order traversal. This function
 * returns the number of timers fetched.
 */
static int
__peak_task_timer_prefetch(double now, peak_timer n, peak_timer *tp, int idx)
  {
  if (!n || idx >= TIMER_PREFETCH_MAX)
    return idx;
  
  idx = __peak_task_timer_prefetch(now, n->left, tp, idx);
  
  if (n->_fire > now || idx >= TIMER_PREFETCH_MAX)
    return idx;
  
  tp[idx++] = n;
  
  return __peak_task_timer_prefetch(now, n->right, tp, idx);
  }

__private_extern__ int
_peak_task_timer_schedule_fire(peak_task task)
  {
  peak_timer ti, tistore[TIMER_PREFETCH_MAX];
  double now;
  int i, result;
  
  /* Don't waste time if we don't have any scheduled timer. */
  if (!task->_tifirst)
    return 0;
  
  /* Get time: this call should cache the current time "set_on_time". */
  now = peak_time_float();
  
  /* Splay the tree before to move the first item up (prefetching should be
   * faster and the recursion depth reduced).
   */
  _peak_timer_tree_splay(task->_tifirst, &task->_tiroot, NULL, NULL);
  
  /* Prefetch pending timers so that we can then modify the splay tree as
   * our wish.
   */
  result = __peak_task_timer_prefetch(now, task->_tiroot, tistore, 0);
  
  /* Process prefetched list. */
  for (i = 0; i < result; i++)
    {
    ti = tistore[i];
    
    /* Remove timer */
    __peak_task_timer_remove(task, ti);
    
    /* Queue operation */
    __peak_task_op_timer_schedule_fire(task, ti);
    
    /* Rearm timer, but not before now. */
    _peak_timer_rearm(ti, now);
    
    /* Re-insert if in requeue mode */
    if (ti->_mode & PEAK_TIMER_MODE_REQUEUE)
      __peak_task_timer_insert(task, ti);
    }
  return result;
  }

__private_extern__ void
_peak_task_op_ioevent_schedule(peak_task task, peak_engine_client c,
                               int event, int info)
  {
  peak_task_runloop rl;
  peak_task_op op;
  
  rl = peak_task_runloop_stackpool_get(task->_pool);
  
  if (rl == NULL)
    PEAK_HALT;
  
  TASK_OP_IOEVENT_GEN(op, rl, c, event, info);
  assert(op != NULL);
  
  _peak_task_runloop_op_schedule(rl, op);
  }

static void
__peak_task_op_timer_schedule_fire(peak_task task, peak_timer ti)
  {
  peak_task_runloop rl;
  peak_task_op op;
  
  rl = peak_task_runloop_stackpool_get(task->_pool);
  
  if (rl == NULL)
    PEAK_HALT;
  
  TASK_OP_TIMER_GEN(op, rl, ti);
  assert(op != NULL);
  
  _peak_task_runloop_op_schedule(rl, op);
  }

__private_extern__ void
_peak_task_process_pending_events(peak_task task, int nevts)
  {
  int i;
  
  if (nevts == 0)
    {
    PEAK_WARN("_peak_task_process_pending_events() called without event!");
    task->_flags &= ~TASK_FLAG_ON_TIME;
    return;
    }
  
  /* Optimization: bypass some checks elsewhere if only one event
   * is being processed, even if we have several threads.
   */
  task->_nevts = nevts;
  
  /* Optimization: don't broadcast for a single event as we are sure
   * it's gonna be dealed by the master runloop.
   */
  if (nevts > 1)
    {
    /* Number of running threads we are going to use: all. */
    peak_atomic_set(&task->_nruns, task->_nthreads);
    
    /* Signal our worker threads
     */
    if (task->_nthreads == 2)
      peak_semaphore_signal(task->_runsem);
    else if (task->_nthreads > 2)
      peak_semaphore_signal_all(task->_runsem);
    }
  else
    {
    /* Number of running threads we are going to use: one. */
    peak_atomic_set(&task->_nruns, 1);
    }
  
  /* "Master thread" now acts like another runloop, and always runs. */
  _peak_task_runloop_run(task->_master, 1);
  
  if (nevts > 1)
    {
    /* Wait for completion. */
    for (i = task->_nthreads - 1; i > 0; i--)
      peak_semaphore_wait(task->_hdlsem);
    }
  /* All done. */
  peak_atomic_set(&task->_nruns, 0);
  peak_task_runloop_stackpool_reset(task->_pool);
  task->_nevts = 0;
  
  /* Consider task not on time anymore */
  if (task->_flags & TASK_FLAG_ON_TIME)
    task->_flags &= ~TASK_FLAG_ON_TIME;
  }

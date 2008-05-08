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
#define RCSID "$Id: task_runloop.c,v 1.4 2005/01/27 16:31:50 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "task_runloop.h"

#include "init_private.h"
#include "task_class.h"

#include <assert.h>

static void __peak_task_runloop_init(peak_task_runloop rl, va_list vp,
                                     void *ctcx);
static void __peak_task_runloop_finalize(peak_task_runloop rl);

static int __peak_task_runloop_op_process(peak_task_runloop rl);
static int __peak_task_runloop_process(peak_task_runloop rl, peak_task_op op);


PEAK_CLASS_BASE_DECLARE(task_runloop);


/* Capture-and-create runloop function - for master thread only
 */
__private_extern__ peak_task_runloop
_peak_task_runloop_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(task_runloop, task);
  }

/* Spawned child thread entry point - for worker thread
 */
__private_extern__ void *
_peak_task_runloop_start(void *arg)
  {
  peak_task task = (peak_task)arg;
  peak_task_runloop rl = PEAK_CLASS_CONSTRUCT1(task_runloop, task);
  peak_task_runloop_stackpool_push(task->_pool, rl);  
  _peak_task_runloop_run(rl, 0);
  peak_release(rl);
  return NULL;
  }

static void
__peak_task_runloop_init(peak_task_runloop rl, va_list vp, void *ctcx)
  {
  rl->_task = va_arg(vp, peak_task);
  rl->_oppool = peak_mem_pool_create(sizeof(__peak_task_op), 0);
  rl->_sched_ophead = NULL;
  rl->_sched_optailp = NULL;
  rl->_runloop_ophead = NULL;
  rl->_exclusivity = 0;
  
  _peak_init_thread_task(rl->_task);
  _peak_init_thread_runloop(rl);
  }

static void
__peak_task_runloop_finalize(peak_task_runloop rl)
  {
  _peak_init_thread_task(NULL);
  _peak_init_thread_runloop(NULL);
  peak_release(rl->_oppool);
  }

__private_extern__ void
_peak_task_runloop_quit(peak_task_runloop rl)
  {
  peak_task_op op;
  
  rl->_task->_nthreads--;
  
  TASK_OP_MANAGER_GEN(op, rl, TASK_MANAGER_CMD_ID_QUIT);
  assert(op != NULL);
  
  _peak_task_runloop_op_schedule(rl, op);
  }

__private_extern__ void
_peak_task_runloop_op_schedule(peak_task_runloop rl, peak_task_op op)
  {
  /* POST CLEANUP */
  while (rl->_sched_ophead != rl->_runloop_ophead)
    {
    peak_task_op _op;
    
    assert (rl->_sched_ophead != NULL);
    
    _op = rl->_sched_ophead;
    rl->_sched_ophead = rl->_sched_ophead->next;
    peak_mem_pool_delete(rl->_oppool, _op);
    }
  
  if (rl->_sched_ophead == NULL)
    {
    rl->_sched_ophead = op;
    rl->_sched_optailp = &rl->_sched_ophead->next;
    rl->_runloop_ophead = rl->_sched_ophead; /* COMMIT CHANGE */
    }
  else
    {
    *rl->_sched_optailp = op;
    rl->_sched_optailp = &(*rl->_sched_optailp)->next;
    }
  }

static int
__peak_task_runloop_op_process(peak_task_runloop rl)
  {
  peak_task_op _op;
  int result = 0;
    
  if (rl->_runloop_ophead != NULL)
    {
    _op = rl->_runloop_ophead;
    rl->_runloop_ophead = rl->_runloop_ophead->next;
    result = __peak_task_runloop_process(rl, _op);
    }
  return result;
  }

#define IOEVT(x) (op->utag.ioevent.x)

static int
__peak_task_runloop_process(peak_task_runloop rl, peak_task_op op)
  {
  int result = 0;
  
  switch (op->tag)
    {
    case TASK_OP_TAG_IOEVENT:
      
      /* We need to verify if the task is still scheduled for this client */
      if (IOEVT(client)->_task == rl->_task) /* Call out if owned */
        (*IOEVT(client)->_ep)(IOEVT(client), IOEVT(event), IOEVT(info));
      
      /* Engine post processing */
      _peak_engine_event_postprocess(IOEVT(client));
      
      peak_release(IOEVT(client));
      
      /* One event processed! */
      result = 1;
      break;
    case TASK_OP_TAG_TIMER:
      if (op->utag.timer->_task == rl->_task) /* Call out if owned */
        _peak_timer_fire(op->utag.timer);
      peak_release(op->utag.timer);
      result = 1;
      break;
    case TASK_OP_TAG_MANAGER:
      assert(op->utag.cmd_id == TASK_MANAGER_CMD_ID_QUIT
             || op->utag.cmd_id == TASK_MANAGER_CMD_ID_ABORT);
      
      result = op->utag.cmd_id;
      break;
    case TASK_OP_TAG_INVALID:
    default:
      PEAK_HALT;
    }
  return result;
  }

__private_extern__ void
_peak_task_runloop_run(peak_task_runloop rl, int master)
  {
  int result, quit = 0;
  peak_task owner = rl->_task;
  
  do
    {
    result = __peak_task_runloop_op_process(rl);
    
    peak_atomic_dec(&owner->_nruns);
    
    /* Check if we have some exclusivity requests pending in other threads.
     * Note task->_nexs changes are atomic.
     */
    if (peak_atomic_read(&owner->_nexs) > 0)
      {
      if (rl->_exclusivity)
        {
        /* End of exclusivity. */
        peak_atomic_dec(&owner->_nexs);
        rl->_exclusivity = 0;
        /* Hrm, wait.. Are there other exclusivity requests in pending? */
        if (peak_atomic_read(&owner->_nexs) > 0)
          {
          /* Yes. Handoff! */
          peak_semaphore_wait_signal(owner->_excosem, owner->_exsem);
          }
        else
          {
          /* No. Wake up! */
          peak_semaphore_signal_all(owner->_excosem);
          }
        }
      else
        {
        /* Lock exlock, we are not going to run anymore. */
        _peak_spinlock_lock(&owner->_exlock);
        assert(peak_atomic_read(&owner->_nruns) > 0);
        if (peak_atomic_read(&owner->_nruns) == 1) /*Last thread to suspend?*/
          peak_semaphore_signal(owner->_exsem); /* Wake one for exclusivity */
        _peak_spinlock_unlock(&owner->_exlock);
        peak_semaphore_wait(owner->_excosem); /* Exclusivity completion sem */
        }
      }
    
    if (result == TASK_MANAGER_CMD_ID_QUIT)
      quit = 1;
    
    if (result == 0)
      {
      /* special case: master thread doesn't block */
      if (master || quit)
        break;
      
      peak_semaphore_wait_signal(owner->_runsem, owner->_hdlsem);
      continue;
      }
    
    peak_atomic_inc(&owner->_nruns);
    
    } while (result != TASK_MANAGER_CMD_ID_ABORT);
  }

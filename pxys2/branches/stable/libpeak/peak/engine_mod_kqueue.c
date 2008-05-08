/* PEAK Library
 *
 * Copyright (c) 2003, 2004, 2005
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
 * $Id: engine_mod_kqueue.c,v 1.6 2005/01/27 16:31:50 mbuna Exp $
 */
#define RCSID "$Id: engine_mod_kqueue.c,v 1.6 2005/01/27 16:31:50 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "engine.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/event.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "internal.h"
#include "socket.h"
#include "spinlock.h"
#include "task_private.h"
#include "utilities.h"

/* We don't systematically register for both read and write events, so we
 * bother to save if we did or not, so that we can properly remove the events
 * later, without error.
 */
#define CS_KEVENT_READ  CS_CUSTOM1
#define CS_KEVENT_WRITE CS_CUSTOM2

static void __peak_engine_init(peak_engine e, va_list vp);
static void __peak_engine_finalize(peak_engine e);
static void __peak_engine_set_or_clear(peak_engine e, peak_engine_client c,
                                       uint32_t set, uint32_t clear);
static void __peak_engine_add_signal(peak_engine e, peak_engine_client c);
static void __peak_engine_ioevent_generate(peak_engine e, peak_engine_client c,
                                           int event, int info);



struct __peak_engine
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task _task;
  int _maxfds;
  int _nfds;
  int _kq;
  int _ne;
  volatile int _running;
  };

PEAK_CLASS_BASE_DECLARE(engine);

__private_extern__ const char *
_peak_engine_get_name(peak_engine e)
  {
  return "kqueue";
  }

__private_extern__ peak_engine
_peak_engine_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(engine, task);
  }

static void
__peak_engine_init(peak_engine e, va_list vp)
  {
  e->_task = va_arg(vp, peak_task);
  e->_maxfds = PEAK_DEFAULT_FLAVOR_MAXFDS;
  e->_nfds = 0;
  
  if ((e->_kq = kqueue()) == -1)
    PEAK_HALT;
  
  e->_running = 0;
  }

static void
__peak_engine_finalize(peak_engine e)
  {
  }

__private_extern__ int
_peak_engine_get_maxfds(peak_engine e)
  {
  return e->_maxfds;
  }

__private_extern__ int
_peak_engine_set_maxfds(peak_engine e, int maxfds)
  {
  if (maxfds <= 0)
    return -1;
  
  e->_maxfds = peak_set_fdlimit(maxfds);
  return (e->_maxfds == maxfds) ? 0 : -1;
  }

static void
__peak_engine_set_or_clear(peak_engine e, peak_engine_client c,
                           uint32_t set, uint32_t clear)
  {
  int i = 0;
  struct kevent chglist[2];
  
  if ((clear ^ set) & (CS_ACCEPTING|CS_READING)) /* readable has changed */
    {
    EV_SET(&chglist[i], c->_ident, EVFILT_READ, EV_ADD, 0, 0, c);
    
    if (set & (CS_ACCEPTING|CS_READING)) /* it's set */
      chglist[i].flags |= EV_ENABLE;
    else /* clear it */
      chglist[i].flags |= EV_DISABLE;
    
    c->_state |= CS_KEVENT_READ;
    i++;
    }
  
  if ((clear ^ set) & (CS_CONNECTING|CS_WRITING)) /* writable has changed */
    {
    EV_SET(&chglist[i], c->_ident, EVFILT_WRITE, EV_ADD, 0, 0, c);
    
    if (set & (CS_CONNECTING|CS_WRITING)) /* it's set */
      chglist[i].flags |= EV_ENABLE;
    else /* clear it */
      chglist[i].flags |= EV_DISABLE;
    
    c->_state |= CS_KEVENT_WRITE;
    i++;
    }
  
  if (i == 0)
    PEAK_FATAL("State of engine's client cannot generate event", 0);
  
  if (kevent(e->_kq, chglist, i, 0, 0, 0) == -1)
    PEAK_FATAL("kevent failure", errno);
  }

static void
__peak_engine_add_signal(peak_engine e, peak_engine_client c)
  {
  struct kevent sigevent;
  struct sigaction act;
  
  assert(c->_state & CS_SIGNAL);
  
  EV_SET(&sigevent, c->_ident, EVFILT_SIGNAL, EV_ADD | EV_ENABLE, 0, 0, c);
  
  if (kevent(e->_kq, &sigevent, 1, 0, 0, 0) == -1)
    PEAK_FATAL("kevent", errno);
  
  act.sa_handler = SIG_IGN; /* ignore the signal */
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(c->_ident, &act, 0);
  }

__private_extern__ void
_peak_engine_add_client(peak_engine e, peak_engine_client c)
  {
  if (c->_state & CS_SIGNAL)
    {
    __peak_engine_add_signal(e, c);
    return;
    }
  
  if (++e->_nfds >= e->_maxfds)
    PEAK_HALT;
  
  __peak_engine_set_or_clear(e, c, c->_state, 0);
  
  c->_engine = e;
  }

__private_extern__ void
_peak_engine_remove_client(peak_engine e, peak_engine_client c)
  {
  int i = 0;
  struct kevent dellist[2];
  
  assert(c != NULL);
  
  e->_nfds--;
  c->_engine = NULL;
  
  if (c->_state & CS_SIGNAL)
    {
    struct sigaction act;
    
    EV_SET(&dellist[i], c->_ident, EVFILT_SIGNAL, EV_DELETE, 0, 0, 0);
    i++;
    
    act.sa_handler = SIG_IGN; /* reset default */
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(c->_ident, &act, 0);
    }
  else
    {
    if (c->_state & CS_KEVENT_READ)
      {
      EV_SET(&dellist[i], c->_ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
      i++;
      }
    if (c->_state & CS_KEVENT_WRITE)
      {
      EV_SET(&dellist[i], c->_ident, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
      i++;
      }
    }
  
  if (i > 0 && kevent(e->_kq, dellist, i, 0, 0, 0) == -1)
    PEAK_FATAL("kevent", errno);
  }

/* Precondition: Always called under the protection of c->_lock.
 */
__private_extern__ void
_peak_engine_edit_client(peak_engine e, peak_engine_client c)
  {
  assert(!(c->_state & CS_HANDLED));
    
  if (c->_sstate != c->_state)
    {
    c->_sstate = c->_state;
    __peak_engine_set_or_clear(e, c, c->_state, CS_ANY);
    }
  }

__private_extern__ void
_peak_engine_loop(peak_engine e)
  {
  struct timespec ts;
  peak_engine_client c;
  int i, nevs, err;
  int events_count = 24;
  struct kevent events[24];
  
  e->_running = 1;
  
  do  {
    nevs = kevent(e->_kq, 0, 0, events, events_count,
                  _peak_task_timer_tswait(e->_task, &ts));
    if (nevs < 0)
      {
      fprintf(stderr, "kevent failure\n");
      continue;
      }
    
    e->_ne = 0;
    
    for (i = 0; i < nevs; i++)
      {
      if ((c = (peak_engine_client)events[i].udata) == NULL)
        PEAK_HALT;
      
      /* Although implementations of kqueue support it, the library's
       * design doesn't allow us to handle more than one event at a time for
       * the same client.
       */
      if (c->_state & CS_HANDLED)
        continue;
      
      switch (events[i].filter)
        {
        case EVFILT_SIGNAL:
          __peak_engine_ioevent_generate(e, c, IOEVENT_SIGNAL,
              events[i].ident);
          break;
        case EVFILT_READ:
          if ((err = peak_socket_get_error(events[i].ident)) != 0)
            {
            __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, err);
            continue;
            }
          
          if (c->_state & CS_ACCEPTING)
            __peak_engine_ioevent_generate(e, c, IOEVENT_ACCEPT, 0);
          else
            {
            if (c->_state & CS_READING)
            __peak_engine_ioevent_generate(e, c,
                events[i].flags & EV_EOF ? IOEVENT_EOF : IOEVENT_READ, 0);
            }
          break;
        case EVFILT_WRITE:
          if ((err = peak_socket_get_error(events[i].ident)) != 0)
            {
            __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, err);
            continue;
            }
          
          if (c->_state & CS_CONNECTING)
            __peak_engine_ioevent_generate(e, c, IOEVENT_CONNECT, 0);
          else /* CS_CONNECTED or accepted socket */
            {
            if (c->_state & CS_WRITING)
            __peak_engine_ioevent_generate(e, c, IOEVENT_WRITE, 0);
            }
          break;
          
        default:
          PEAK_HALT;
        }
      }
    
    /* Prepare to fire any pending timers
     */
    e->_ne += _peak_task_timer_schedule_fire(e->_task);
    _peak_task_process_pending_events(e->_task, e->_ne);
    
    } while (e->_running);
  }

__private_extern__ void
_peak_engine_break(peak_engine e)
  {
  e->_running = 0;
  }

static void
__peak_engine_ioevent_generate(peak_engine e, peak_engine_client c,
                               int event, int info)
  {
  uint16_t mclear = 0, mset = 0;
  
  switch (event)
    {
    case IOEVENT_CONNECT:
      mclear = CS_CONNECTING;
      mset = CS_CONNECTED|CS_READING|CS_WRITING;
      break;
    case IOEVENT_ACCEPT:
      mclear = CS_ACCEPTING;
      break;
    case IOEVENT_READ:
      mclear = CS_READING;
      break;
    case IOEVENT_WRITE:
      mclear = CS_WRITING;
      break;
    case IOEVENT_EOF:
    case IOEVENT_ERROR:
      mclear = CS_CONNECTED|CS_READING|CS_WRITING;
      break;
    case IOEVENT_SIGNAL:
      break;
    default:
      PEAK_HALT;
      break;
    }

#if 0
  printf("gen: c->_state=%x\n", c->_state);
  if (c->_state & CS_HANDLED)
    {
    printf("gen: handling several events for the same object fd=%d\n",
           c->_ident);
    }
#endif
  
  /* Set "event handled" bit */
  c->_state |= CS_HANDLED;
  
  /* Cache state */
  c->_sstate = c->_state;
  
  /* Prepare */
  c->_state &= ~mclear;
  c->_state |= mset;
  
  /* Schedule for processing */
  _peak_task_op_ioevent_schedule(e->_task, c, event, info);
  
  e->_ne++;
  }

__private_extern__ void
_peak_engine_event_postprocess(peak_engine_client c)
  {
  peak_engine e = c->_engine;
  
  /* Commit changes if necessary, restore stuffs.
   */
  _peak_engine_client_lock(c);
  
  if (c->_sstate != c->_state && e != NULL)
    __peak_engine_set_or_clear(e, c, c->_state, CS_ANY);
  
  c->_sstate = 0;              /* invalidate cache */
  c->_state &= ~CS_HANDLED;    /* we don't handle it anymore */
  
  _peak_engine_client_unlock(c);
  }

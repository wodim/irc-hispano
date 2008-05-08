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
 * $Id: engine_mod_epoll.c,v 1.5 2007/03/06 21:02:43 mbuna Exp $
 */
#define RCSID "$Id: engine_mod_epoll.c,v 1.5 2007/03/06 21:02:43 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "engine.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "internal.h"
#include "socket.h"
#include "spinlock.h"
#include "task_private.h"
#include "utilities.h"

static peak_spinlock_t epollSignalLock = PEAK_SPINLOCK_INITIALIZER;

static void __peak_engine_init(peak_engine e, va_list vp);
static void __peak_engine_finalize(peak_engine e);
static void __peak_engine_set_or_clear(peak_engine e, peak_engine_client c,
                                       uint32_t set, uint32_t clear);
static void __peak_engine_add_signal(peak_engine e, peak_engine_client c);
static void __peak_engine_signal_trap(int signum);
static void __peak_engine_ioevent_generate(peak_engine e, peak_engine_client c,
                                           int event, int info);

#ifndef MAX_SIGNUM
#ifdef NSIG
#define MAX_SIGNUM (NSIG-1)
#else
#define MAX_SIGNUM 31
#endif
#endif

struct __peak_engine
  {
  PEAK_STRUCT_RT_HEADER;
  peak_task _task;
  int _epfd;
  int _maxfds;
  peak_engine_client _signals[MAX_SIGNUM + 1];
  int _signal_status;
  int _ne;
  volatile int _running;
  };

static int interrupt_read_fd, interrupt_write_fd;

PEAK_CLASS_BASE_DECLARE(engine);

__private_extern__ const char *
_peak_engine_get_name(peak_engine e)
  {
  return "epoll";
  }

__private_extern__ peak_engine
_peak_engine_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(engine, task);
  }

static void
__peak_engine_init(peak_engine e, va_list vp)
  {
  int i, p[2];
  
  e->_task = va_arg(vp, peak_task);
  
  e->_maxfds = peak_set_fdlimit(PEAK_DEFAULT_FLAVOR_MAXFDS);
  e->_epfd = epoll_create(e->_maxfds);
  if (e->_epfd == -1)
    PEAK_FATAL("epoll_create failure", errno);
  
  for (i = 0; i <= MAX_SIGNUM; i++)
    e->_signals[i] = NULL;
  
  e->_signal_status = -1;
  
  if (interrupt_read_fd == 0)
    {
    if (pipe(p) == -1)
      PEAK_HALT;
    interrupt_read_fd = p[0];
    interrupt_write_fd = p[1];
    }
  
  e->_running = 0;
  }

static void
__peak_engine_finalize(peak_engine e)
  {
  close(e->_epfd);
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
  
  maxfds += 2; /* for signal fds */
  
  e->_maxfds = peak_set_fdlimit(maxfds);
  
  return maxfds == e->_maxfds ? 0 : -1;
  }

static void
__peak_engine_set_or_clear(peak_engine e, peak_engine_client c,
                           uint32_t set, uint32_t clear)
  {
  struct epoll_event ev;
  
  ev.events = c->_events;
  ev.data.ptr = (void*)c;
  
  if ((clear ^ set) & (CS_ACCEPTING|CS_READING)) /* readable has changed */
    {
    if (set & (CS_ACCEPTING|CS_READING)) /* it's set */
      ev.events |= EPOLLIN;
    else /* clear it */
      ev.events &= ~EPOLLIN;
    }
  
  if ((clear ^ set) & (CS_CONNECTING|CS_WRITING)) /* writable has changed */
    {
    if (set & (CS_CONNECTING|CS_WRITING)) /* it's set */
      ev.events |= EPOLLOUT;
    else /* clear it */
      ev.events &= ~EPOLLOUT;
    }
  
  if (ev.events != c->_events)
    {
    if (epoll_ctl(e->_epfd, EPOLL_CTL_MOD, c->_ident, &ev) != 0)
      PEAK_FATAL("epoll_ctl failure", errno);
    c->_events = ev.events;
    }
  }

static void
__peak_engine_add_signal(peak_engine e, peak_engine_client c)
  {
  struct sigaction action;
  sigset_t stop_signal;
  
  sigemptyset(&stop_signal);
  sigaddset(&stop_signal, c->_ident);
  
  action.sa_handler = __peak_engine_signal_trap;
  action.sa_mask = stop_signal;
  action.sa_flags = 0;
  
  if (c->_ident > MAX_SIGNUM)
    PEAK_HALT;
  
  if (sigaction(c->_ident, &action, NULL) == 0)
    {
    e->_signals[c->_ident] = c;
    
    _peak_spinlock_lock(&epollSignalLock);
    
    /* Register one real client for all signals, the first signal client is
     * effectively used for convenience.
     */
    if (e->_signal_status == -1)
      {
      struct epoll_event ev;
      
      c->_events = EPOLLIN;
      ev.events = c->_events;
      ev.data.ptr = (void*)c;
      
      c->_state |= CS_READING;
      if (epoll_ctl(e->_epfd, EPOLL_CTL_ADD, interrupt_read_fd, &ev) != 0)
        PEAK_FATAL("epoll_ctl failure", errno);
      
      e->_signal_status = 0;
      c->_engine = e;
      }
    _peak_spinlock_unlock(&epollSignalLock);
    }
  else
    PEAK_HALT;
  }

static void
__peak_engine_signal_trap(int signum)
  {
  assert (interrupt_write_fd >= 0);
  write(interrupt_write_fd, &signum, sizeof(interrupt_write_fd));
  }

__private_extern__ void
_peak_engine_add_client(peak_engine e, peak_engine_client c)
  {
  struct epoll_event ev;
  
  if (c->_state & CS_SIGNAL)
    {
    __peak_engine_add_signal(e, c);
    return;
    }
  
  /* Bound checking? */
  
  c->_events = 0;
  ev.events = c->_events;
  ev.data.ptr = (void*)c;
  if (epoll_ctl(e->_epfd, EPOLL_CTL_ADD, c->_ident, &ev) != 0)
    PEAK_FATAL("epoll_ctl failure", errno);
  
  __peak_engine_set_or_clear(e, c, c->_state, 0);
  
  c->_engine = e;
  }

__private_extern__ void
_peak_engine_remove_client(peak_engine e, peak_engine_client c)
  {
  assert(c != NULL);
  
  if (c->_state & CS_SIGNAL)
    {
    struct sigaction action;
    
    /* Remove a signal: restore default action. */
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    
    sigaction(c->_ident, &action, NULL);
    e->_signals[c->_ident] = NULL;
    }
  else if (epoll_ctl(e->_epfd, EPOLL_CTL_DEL, c->_ident, NULL) != 0)
      PEAK_FATAL("epoll_ctl failure", errno);
  
  c->_engine = NULL;
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
  peak_engine_client c;
  struct epoll_event events[24];
  int maxevents = 24;
  int i, nfds;
  int err;
  
  e->_running = 1;
  
  do  {
    nfds = epoll_wait(e->_epfd, events, maxevents,
                      _peak_task_timer_mswait(e->_task));
    if (nfds < 0)
      {
      fprintf(stderr, "epoll_wait failure\n");
      continue;
      }
    e->_ne = 0;
    
    for (i = 0; i <  nfds; i++)
      {
      if ((c = (peak_engine_client)events[i].data.ptr) == NULL)
        continue;
      
      if (c->_state & CS_SIGNAL)
        {
        if (events[i].events & EPOLLIN)
          {
          int signum;
          
          if (read(interrupt_read_fd, &signum, sizeof(signum))
              == sizeof(signum))
            {
            if (signum > 0 && signum <= MAX_SIGNUM)
              __peak_engine_ioevent_generate(e, e->_signals[signum],
                                             IOEVENT_SIGNAL, signum);
            }
          }
        continue;
        }
      
      assert(!(c->_state & CS_SIGNAL));
      
      if (events[i].events & EPOLLERR &&
          (err = peak_socket_get_error(c->_ident)) != 0)
        {
        __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, err);
        continue;
        }
      
      if (events[i].events & EPOLLIN)
        {
        if (c->_state & CS_ACCEPTING)  /* ready for accept */
          __peak_engine_ioevent_generate(e, c, IOEVENT_ACCEPT, 0);
        else
          {
          assert(c->_state & CS_READING);
          
          if (events[i].events & EPOLLHUP)
            __peak_engine_ioevent_generate(e, c, IOEVENT_EOF, 0);
          else
            __peak_engine_ioevent_generate(e, c, IOEVENT_READ, 0);
          }
        }
      else if (events[i].events & EPOLLOUT)
        {
        if (c->_state & CS_CONNECTING)
          __peak_engine_ioevent_generate(e, c, IOEVENT_CONNECT, 0);
        else
          {
          assert (c->_state & CS_WRITING);
          
          __peak_engine_ioevent_generate(e, c, IOEVENT_WRITE, 0);
          }
        }
      }
    
    /* Prepare to fire any pending timers
     */
    e->_ne += _peak_task_timer_schedule_fire(e->_task);
    
    /* Process events...
     */
    _peak_task_process_pending_events(e->_task, e->_ne);
    
    } while (e->_running);
  }

void
_peak_engine_break(peak_engine e)
  {
  e->_running = 0;
  }

static void
__peak_engine_ioevent_generate(peak_engine e, peak_engine_client c,
                               int ioevent, int info)
  {
  uint16_t mclear = 0, mset = 0;
  
  assert(c != NULL);
  
  switch (ioevent)
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
  
  /* Set "event handled" bit */
  c->_state |= CS_HANDLED;
  
  /* Cache state */
  c->_sstate = c->_state;
  
  /* Prepare */
  c->_state &= ~mclear;
  c->_state |= mset;
  
  /* Schedule for processing */
  _peak_task_op_ioevent_schedule(e->_task, c, ioevent, info);
  
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


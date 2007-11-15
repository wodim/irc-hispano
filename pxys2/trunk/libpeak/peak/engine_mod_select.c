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
 * $Id: engine_mod_select.c,v 1.3 2005/02/28 15:25:37 mbuna Exp $
 */
#define RCSID "$Id: engine_mod_select.c,v 1.3 2005/02/28 15:25:37 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PEAK_FD_SETSIZE
#define FD_SETSIZE PEAK_FD_SETSIZE
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
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif
#include <time.h>
#include <unistd.h>

#include "internal.h"
#include "socket.h"
#include "spinlock.h"
#include "task_private.h"
#include "utilities.h"

static peak_spinlock_t fdsetLock = 0;

static void __peak_engine_init(peak_engine e, va_list vp);
static void __peak_engine_finalize(peak_engine e);
static void __peak_engine_set_or_clear(peak_engine e, int fd,
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
  peak_engine_client _clients[FD_SETSIZE];
  peak_engine_client _signals[MAX_SIGNUM + 1];
  int _maxfds;
  int _highest_fd;
  int _ne;
  fd_set _read_set;
  fd_set _write_set;
  volatile int _running;
  };

static int interrupt_read_fd, interrupt_write_fd;

PEAK_CLASS_BASE_DECLARE(engine);

__private_extern__ const char *
_peak_engine_get_name(peak_engine e)
  {
  return "select";
  }

__private_extern__ peak_engine
_peak_engine_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(engine, task);
  }

static void
__peak_engine_init(peak_engine e, va_list vp)
  {
  int i;
  int p[2];
  
  e->_task = va_arg(vp, peak_task);
  
  for (i = 0; i < FD_SETSIZE; i++)
    e->_clients[i] = NULL;
  for (i = 0; i <= MAX_SIGNUM; i++)
    e->_signals[i] = NULL;
  
  e->_maxfds = FD_SETSIZE;
  e->_highest_fd = -1;
  
  if (interrupt_read_fd == 0)
    {
    if (pipe(p) == -1)
      PEAK_HALT;
    interrupt_read_fd = p[0];
    interrupt_write_fd = p[1];
    }
  
  FD_ZERO(&e->_read_set);
  FD_ZERO(&e->_write_set);
  
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
  if (maxfds <= 0) /* doh */
    return -1;
  
  maxfds += 2;
  
  if (maxfds > FD_SETSIZE)
    PEAK_FATAL("Failed to increase max file descriptors. select() requires "
               "compilation time setup. See libpeak documentation.", 0);
  
  e->_maxfds = peak_set_fdlimit(maxfds);
  return (e->_maxfds == maxfds) ? 0 : 1;
  }

static void
__peak_engine_set_or_clear(peak_engine e, int fd, uint32_t set, uint32_t clear)
  {
  if ((clear ^ set) & (CS_ACCEPTING|CS_READING)) /* readable has changed */
    {
    if (set & (CS_ACCEPTING|CS_READING)) /* it's set */
      FD_SET(fd, &e->_read_set);
    else /* clear it */
      FD_CLR(fd, &e->_read_set);
    }
    
  if ((clear ^ set) & (CS_CONNECTING|CS_WRITING)) /* writable has changed */
    {
    if (set & (CS_CONNECTING|CS_WRITING)) /* it's set */
      FD_SET(fd, &e->_write_set);
    else /* clear it */
      FD_CLR(fd, &e->_write_set);
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
        
    /* Register one real client for all signals, the first signal client is
     * effectively used for convenience.
     */
    if (interrupt_read_fd >= FD_SETSIZE)
      PEAK_HALT;
        
    if (e->_clients[interrupt_read_fd] == NULL)
      {
      _peak_spinlock_lock(&fdsetLock);
      
      assert(interrupt_read_fd < FD_SETSIZE);
      
      e->_clients[interrupt_read_fd] = c;
      
      if (interrupt_read_fd >= e->_highest_fd)
        e->_highest_fd = interrupt_read_fd;
      
      c->_state |= CS_READING;
      __peak_engine_set_or_clear(e, interrupt_read_fd, c->_state, 0);
      c->_engine = e;
      
      _peak_spinlock_unlock(&fdsetLock);
      }
    }
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
  if (c->_state & CS_SIGNAL)
    {
    __peak_engine_add_signal(e, c);
    return;
    }
  
  _peak_spinlock_lock(&fdsetLock);
  
  assert(c->_ident < FD_SETSIZE);
  assert(e->_clients[c->_ident] == NULL);
  
  e->_clients[c->_ident] = c;
  
  if (c->_ident >= e->_highest_fd)
    e->_highest_fd = c->_ident;
  
  __peak_engine_set_or_clear(e, c->_ident, c->_state, 0);
  
  c->_engine = e;
  
  _peak_spinlock_unlock(&fdsetLock);
  }

__private_extern__ void
_peak_engine_remove_client(peak_engine e, peak_engine_client c)
  {
  _peak_spinlock_lock(&fdsetLock);
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
  else
    {
    FD_CLR(c->_ident, &e->_read_set);
    FD_CLR(c->_ident, &e->_write_set);
    
    e->_clients[c->_ident] = NULL;
    
    while (e->_highest_fd > -1 && e->_clients[e->_highest_fd] == NULL)
      e->_highest_fd--;
    }
  c->_engine = NULL;
  _peak_spinlock_unlock(&fdsetLock);
  }

/* Always called under the protection of cEditLock
 */
__private_extern__ void
_peak_engine_edit_client(peak_engine e, peak_engine_client c)
  {
  assert(!(c->_state & CS_HANDLED));
  
  if (c->_sstate != c->_state)
    {
    c->_sstate = c->_state;
    _peak_spinlock_lock(&fdsetLock);
    __peak_engine_set_or_clear(e, c->_ident, c->_state, CS_ANY);
    _peak_spinlock_unlock(&fdsetLock);
    }
  }

__private_extern__ void
_peak_engine_loop(peak_engine e)
  {
  fd_set read_set;
  fd_set write_set;
  struct timeval tv;
  peak_engine_client c;
  int i, nfds;
  int err;
  
  e->_running = 1;
  
  do  {
    read_set = e->_read_set;
    write_set = e->_write_set;
    
    nfds = select(e->_highest_fd + 1, &read_set, &write_set, 0,
                  _peak_task_timer_tvwait(e->_task, &tv));
    
    if (nfds < 0)
      continue;
    
    e->_ne = 0;
    
    for (i = 0; nfds > 0 && i <= e->_highest_fd; i++)
      {
      if ((c = e->_clients[i]) == NULL)
        continue;
      
      if (c->_state & CS_SIGNAL)
        {
        if (FD_ISSET(i, &read_set))
          {
          int signum;
          
          if (read(interrupt_read_fd, &signum, sizeof(signum))
              == sizeof(signum))
            {
            if (signum > 0 && signum <= MAX_SIGNUM)
              __peak_engine_ioevent_generate(e, e->_signals[signum],
                                             IOEVENT_SIGNAL, signum);
            }
          nfds--;
          }
        continue;
        }
      
      if ((err = peak_socket_get_error(i)) != 0)
        {
        __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, err);
        nfds--;
        continue;
        }
      
      if (FD_ISSET(i, &read_set))
        {
        if (c->_state & CS_ACCEPTING)  /* ready for accept */
          __peak_engine_ioevent_generate(e, c, IOEVENT_ACCEPT, 0);
        else
          {
          assert(c->_state & CS_READING);
          
          /* PEEK TEST */
          if (c->_state & CS_PEEKABLE)
            {
            switch (peak_socket_peek(i))
              {
              case -1:
                if (errno == EAGAIN)
                  {
                  PEAK_WARN("select/peak_socket_peek: EAGAIN");
                  continue; /* Resource temporarily unavailable */
                  }
                __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR,
                                               peak_socket_error());
                PEAK_FATAL("peak_socket_peek failed", errno);
                break;
              case 0: /* EOF */
                __peak_engine_ioevent_generate(e, c, IOEVENT_EOF, 0);
                break;
              default:
                __peak_engine_ioevent_generate(e, c, IOEVENT_READ, 0);
                break;
              }
            }
          else
            __peak_engine_ioevent_generate(e, c, IOEVENT_READ, 0);
          }
        nfds--;
        }
      else if (FD_ISSET(i, &write_set))
        {
        if (c->_state & CS_CONNECTING)
          __peak_engine_ioevent_generate(e, c, IOEVENT_CONNECT, 0);
        else
          {
          assert (c->_state & CS_WRITING);
          
          __peak_engine_ioevent_generate(e, c, IOEVENT_WRITE, 0);
          }
        nfds--;
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
    __peak_engine_set_or_clear(e, c->_ident, c->_state, CS_ANY);
  
  c->_sstate = 0;              /* invalidate cache */
  c->_state &= ~CS_HANDLED;    /* we don't handle it anymore */
  
  _peak_engine_client_unlock(c);
  }


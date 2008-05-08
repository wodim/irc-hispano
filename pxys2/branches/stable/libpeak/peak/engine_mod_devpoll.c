/* PEAK Library
 *
 * Copyright (c) 2004
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
 * $Id: engine_mod_devpoll.c,v 1.1 2007/05/24 13:11:53 mbuna Exp $
 */
#define RCSID "$Id: engine_mod_devpoll.c,v 1.1 2007/05/24 13:11:53 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "engine.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/devpoll.h>
#include <fcntl.h>
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

/* Figure out what bits to set for read */
#if defined(POLLMSG) && defined(POLLIN) && defined(POLLRDNORM)
#  define POLLREADFLAGS (POLLMSG|POLLIN|POLLRDNORM)
#elif defined(POLLIN) && defined(POLLRDNORM)
#  define POLLREADFLAGS (POLLIN|POLLRDNORM)
#elif defined(POLLIN)
#  define POLLREADFLAGS POLLIN
#elif defined(POLLRDNORM)
#  define POLLREADFLAGS POLLRDNORM
#endif

/* Figure out what bits to set for write */
#if defined(POLLOUT) && defined(POLLWRNORM)
#  define POLLWRITEFLAGS (POLLOUT|POLLWRNORM)
#elif defined(POLLOUT)
#  define POLLWRITEFLAGS POLLOUT
#elif defined(POLLWRNORM)
#  define POLLWRITEFLAGS POLLWRNORM
#endif

static peak_spinlock_t pollfdLock = PEAK_SPINLOCK_INITIALIZER;

static void __peak_engine_init(peak_engine e, va_list vp);
static void __peak_engine_finalize(peak_engine e);
static void __peak_engine_allocate_clients(peak_engine e);
static void __peak_engine_set_or_clear(peak_engine e, int fd, uint32_t set,
                                       uint32_t clear);
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
  peak_engine_client* _clients;
  int _alloctotal;
  int _dpfd;
  int _maxfds;
  peak_engine_client _signals[MAX_SIGNUM + 1];
  int _ne;
  volatile int _running;
  };

static int interrupt_read_fd, interrupt_write_fd;

PEAK_CLASS_BASE_DECLARE(engine);

__private_extern__ const char *
peak_engine_get_name(peak_engine e)
  {
  return "/dev/poll";
  }

__private_extern__ peak_engine
peak_engine_create(peak_task task)
  {
  return PEAK_CLASS_CONSTRUCT1(engine, task);
  }

static void
__peak_engine_init(peak_engine e, va_list vp)
  {
  int i, p[2];
  
  e->_task = va_arg(vp, peak_task);
  e->_maxfds = peak_set_fdlimit(PEAK_DEFAULT_FLAVOR_MAXFDS);
  e->_clients = NULL;
  e->_alloctotal = 0;
  
  /* Open /dev/poll device. */
  if ((e->_dpfd = open("/dev/poll", O_RDWR)) < 0)
    PEAK_HALT;
  
  for (i = 0; i <= MAX_SIGNUM; i++)
    e->_signals[i] = NULL;
  
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
  if (e->_clients)
    peak_deallocate(e->_clients);
  close(e->_dpfd);
  }

__private_extern__ int
peak_engine_get_maxfds(peak_engine e)
  {
  return e->_maxfds;
  }

__private_extern__ int
peak_engine_set_maxfds(peak_engine e, int maxfds)
  {
  if (maxfds <= 0) /* doh */
    return -1;
  
  maxfds += 2; /* for signal fds */
  
  if (e->_alloctotal > 0 && maxfds > e->_alloctotal)
    {
    peak_engine_client * n_clients;
    int i;
    
    n_clients = (peak_engine_client *) peak_allocate(sizeof(peak_engine_client)
                                                     * maxfds);
    
    for (i = 0; i < e->_maxfds; i++)
      n_clients[i] = e->_clients[i];
    for (i = e->_maxfds; i < maxfds; i++)
      n_clients[i] = NULL;
    
    _peak_spinlock_lock(&pollfdLock);
    e->_alloctotal = maxfds;
    e->_maxfds = maxfds;
    peak_deallocate(e->_clients);
    e->_clients = n_clients;
    _peak_spinlock_unlock(&pollfdLock);
    }
  else
    {
    _peak_spinlock_lock(&pollfdLock);
    e->_maxfds = peak_set_fdlimit(maxfds);
    _peak_spinlock_unlock(&pollfdLock);
    }
  return maxfds == e->_maxfds ? 0 : -1;
  }

/* Should be called under the protection of pollfdLock for eventual
 * concurrent allocation (eg. 2 timers are fired at the same time to create
 * the first engine's clients on different threads).
 */
static void
__peak_engine_allocate_clients(peak_engine e)
  {
  int i;
  
  assert(e->_clients == NULL);
  
  e->_clients = (peak_engine_client *) peak_allocate(sizeof(peak_engine_client)
                                                     * e->_maxfds);
  for (i = 0; i < e->_maxfds; i++)
    e->_clients[i] = NULL;
  e->_alloctotal = e->_maxfds;
  }


static void
__peak_engine_set_or_clear(peak_engine e, int fd, uint32_t set, uint32_t clear)
  {
  struct pollfd pfd;
//  printf("fd=%d set=%x clear=%x\n", fd, set, clear);
  pfd.fd = fd;
  if (clear != 0)
    {
    pfd.events = POLLREMOVE;
    if (write(e->_dpfd, &pfd, sizeof(pfd)) != sizeof(pfd))
      PEAK_HALT;
    }
  
  if (!(set & CS_ANY))
    return;
  
  pfd.events = 0;
  if (set & (CS_ACCEPTING|CS_READING))
    pfd.events |= POLLREADFLAGS;
  if (set & (CS_CONNECTING|CS_WRITING))
    pfd.events |= POLLWRITEFLAGS;
  
  if (write(e->_dpfd, &pfd, sizeof(pfd)) != sizeof(pfd))
    PEAK_HALT;
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
    _peak_spinlock_lock(&pollfdLock);
    if (!e->_alloctotal)
      __peak_engine_allocate_clients(e);
    
    if (e->_clients[interrupt_read_fd] == NULL)
      {
      if (interrupt_read_fd >= e->_maxfds)
        PEAK_HALT; /* TODO */
      
      e->_clients[interrupt_read_fd] = c;
      
      c->_state |= CS_READING;
      __peak_engine_set_or_clear(e, interrupt_read_fd, c->_state, 0);
      c->_engine = e;
      }
    _peak_spinlock_unlock(&pollfdLock);
    }
  }

static void
__peak_engine_signal_trap(int signum)
  {
  assert (interrupt_write_fd >= 0);
  write(interrupt_write_fd, &signum, sizeof(interrupt_write_fd));
  }

__private_extern__ void
peak_engine_add_client(peak_engine e, peak_engine_client c)
  {
  if (c->_state & CS_SIGNAL)
    {
    __peak_engine_add_signal(e, c);
    return;
    }
  
  _peak_spinlock_lock(&pollfdLock);
  
  if (!e->_alloctotal)
    __peak_engine_allocate_clients(e);
  
  if (c->_ident >= e->_maxfds)
    PEAK_HALT; /* Sorry, that's too much. */
  
  e->_clients[c->_ident] = c;
  
  /* Set start flags */
  __peak_engine_set_or_clear(e, c->_ident, c->_state, 0);
  
  c->_engine = e;
  
  _peak_spinlock_unlock(&pollfdLock);
  }

__private_extern__ void
peak_engine_remove_client(peak_engine e, peak_engine_client c)
  {
  _peak_spinlock_lock(&pollfdLock);
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
    /* Remove it. */
    __peak_engine_set_or_clear(e, c->_ident, 0, CS_ANY);
    
    /* Then we can clear the slot. */
    e->_clients[c->_ident] = NULL;
    }
  c->_engine = NULL;
  _peak_spinlock_unlock(&pollfdLock);
  }

/* Precondition: Always called under the protection of c->_lock.
 */
__private_extern__ void
peak_engine_edit_client(peak_engine e, peak_engine_client c)
  {
  assert(!(c->_state & CS_HANDLED));
  
  if (c->_sstate != c->_state)
    {
    c->_sstate = c->_state;
    _peak_spinlock_lock(&pollfdLock);
    __peak_engine_set_or_clear(e, c->_ident, c->_state, CS_ANY);
    _peak_spinlock_unlock(&pollfdLock);
    }
  }

#define PEAK_POLLS_PER_LOOP 24
__private_extern__ void
peak_engine_loop(peak_engine e)
  {
  peak_engine_client c;
  struct dvpoll dopoll;
  struct pollfd polls[PEAK_POLLS_PER_LOOP];
  int maxpolls = PEAK_POLLS_PER_LOOP;
  int i, nfds;
  int err;
  
  e->_running = 1;
  
  do  {
    dopoll.dp_fds = polls;
    dopoll.dp_nfds = maxpolls;
    dopoll.dp_timeout = _peak_task_timer_mswait(e->_task);
    nfds = ioctl(e->_dpfd, DP_POLL, &dopoll);
    
    if (nfds < 0)
      {
      fprintf(stderr, "/dev/poll: ioctl error\n");
      continue;
      }
    
    e->_ne = 0;
    
    for (i = 0; i < nfds; i++)
      {
      if ((c = e->_clients[polls[i].fd]) == NULL)
        continue;
      
      if (c->_state & CS_SIGNAL)
        {
        if (polls[i].revents & POLLREADFLAGS)
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
      assert(polls[i].fd == c->_ident);
      
      if ((err = peak_socket_get_error(polls[i].fd)) != 0)
        {
        __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, err);
        continue;
        }
      
#ifdef POLLHUP
      if (polls[i].revents & POLLHUP)
        __peak_engine_ioevent_generate(e, c, IOEVENT_EOF, 0);
      else
#endif
      if (polls[i].revents & POLLREADFLAGS)
        {
        if (c->_state & CS_ACCEPTING)  /* ready for accept */
          __peak_engine_ioevent_generate(e, c, IOEVENT_ACCEPT, 0);
        else
          {
          assert(c->_state & CS_READING);
          
          /* PEEK TEST */
          if (c->_state & CS_PEEKABLE)
            {
            switch (peak_socket_peek(polls[i].fd))
              {
              case -1:
                if (errno == EAGAIN)
                  {
                  PEAK_WARN("peak_socket_peek triggered EAGAIN");
                  continue; /* Resource temporarily unavailable */
                  }
                __peak_engine_ioevent_generate(e, c, IOEVENT_ERROR, errno);
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
        }
      else if (polls[i].revents & POLLWRITEFLAGS)
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

__private_extern__ void
peak_engine_break(peak_engine e)
  {
  e->_running = 0;
  }

static void
__peak_engine_ioevent_generate(peak_engine e, peak_engine_client c,
                               int ioevent, int info)
  {
  uint16_t mclear = 0, mset = 0;
  
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
peak_engine_event_postprocess(peak_engine_client c)
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

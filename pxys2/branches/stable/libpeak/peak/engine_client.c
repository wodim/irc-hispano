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
 * $Id: engine_client.c,v 1.1.1.1 2003/12/30 02:29:09 mbuna Exp $
 */
#define RCSID "$Id: engine_client.c,v 1.1.1.1 2003/12/30 02:29:09 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "engine_client.h"
#include "internal.h"
#include "task_private.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
/*
__private_extern__ peak_spinlock_t cEditLock = PEAK_SPINLOCK_INITIALIZER;
*/
static void __peak_engine_client_init(peak_engine_client c, va_list vp,
                                      void *ctcx);
static void __peak_engine_client_finalize(peak_engine_client c);

PEAK_CLASS_BASE_DEFINE_VIRTUAL(engine_client);

/* No creation method: "pure virtual class" */

static void
__peak_engine_client_init(peak_engine_client c, va_list vp, void *ctcx)
    {
    c->_task = NULL;
    c->_lock = PEAK_SPINLOCK_INITIALIZER;
    c->_ident = -1;
    c->_state = 0;
    c->_ep = NULL;
    }

#include <stdio.h>
static void
__peak_engine_client_finalize(peak_engine_client c)
    {
    if (c->_task)
        _peak_task_unschedule_engine_client(c->_task, c);
    
    if (c->_ident >= 0)
        close(c->_ident);
    }

void
_peak_engine_client_configure(peak_engine_client c, int fd,
                              uint32_t state_init,
                              peak_engine_client_event_process_func ep)
    {
    c->_ident = fd;
    c->_state = state_init;
    c->_sstate = 0;
    c->_ep = ep;
    }


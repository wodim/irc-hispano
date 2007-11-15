/* PEAK Library
 *
 * Copyright (c) 2003-2005
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
 * $Id: engine.h,v 1.3 2005/01/27 16:31:50 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_ENGINE_H_
#define INCLUDED_PEAK_ENGINE_H_

#include <peak/task.h>
#include "internal.h"

typedef struct __peak_engine * peak_engine;
typedef struct __peak_engine_client * peak_engine_client;


/* Engine states
 */
enum {
    CS_CONNECTING = 1 << 0,
    CS_ACCEPTING = 1 << 1,
    CS_READING = 1 << 2,
    CS_WRITING = 1 << 3,
    
    CS_CONNECTED = 1 << 4,
    CS_SOCKET = 1 << 5,
    CS_PEEKABLE = 1 << 6,
    CS_SIGNAL = 1 << 7,
    
    /* 7 bits (9 to 15) available for specific engine needs */
    CS_CUSTOM1 = 1 << 8,
    CS_CUSTOM2 = 1 << 9,
    CS_CUSTOM3 = 1 << 10,
    CS_CUSTOM4 = 1 << 11,
    CS_CUSTOM5 = 1 << 12,
    CS_CUSTOM6 = 1 << 13,
    CS_CUSTOM7 = 1 << 14,
    
    CS_HANDLED = 1 << 15,
    
    CS_ANY = CS_CONNECTING|CS_ACCEPTING|CS_READING|CS_WRITING
    };


/* IO Events */
enum {
    IOEVENT_NONE = 0,
    IOEVENT_CONNECT,
    IOEVENT_ACCEPT,
    IOEVENT_READ,
    IOEVENT_WRITE,
    IOEVENT_EOF,
    IOEVENT_ERROR,
    IOEVENT_SIGNAL
    };

#if defined(__cplusplus)
extern "C" {
#endif

__private_extern__ const char * _peak_engine_get_name(peak_engine e);

__private_extern__ peak_engine _peak_engine_create(peak_task task);

__private_extern__ int _peak_engine_get_maxfds(peak_engine e);

__private_extern__ int _peak_engine_set_maxfds(peak_engine e, int maxfds);

__private_extern__ void _peak_engine_add_client(peak_engine e,
                                                peak_engine_client c);

__private_extern__ void _peak_engine_remove_client(peak_engine e,
                                                   peak_engine_client c);

__private_extern__ void _peak_engine_edit_client(peak_engine e,
                                                 peak_engine_client c);

__private_extern__ void _peak_engine_loop(peak_engine e);

__private_extern__ void _peak_engine_break(peak_engine e);

__private_extern__ void _peak_engine_event_postprocess(peak_engine_client c);


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_ENGINE_H_ */

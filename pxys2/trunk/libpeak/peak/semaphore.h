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
 * $Id: semaphore.h,v 1.4 2004/01/12 01:52:11 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_SEMAPHORE_H_
#define INCLUDED_PEAK_SEMAPHORE_H_

/* This enables struct timespec on AIX and Linux.
 */
#undef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199506L
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_MACH_SEMAPHORE_H
#include <mach/semaphore.h>
#endif
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#endif
#include <time.h>

#if defined(HAVE_SEMAPHORE_CREATE) && defined(HAVE_SEMAPHORE_SIGNAL)
#define PEAK_USE_MACH_SEMAPHORE
#endif

#ifdef PEAK_USE_MACH_SEMAPHORE
typedef mach_timespec_t peak_timespec_t;
#else
typedef struct timespec peak_timespec_t;
#endif

typedef struct __peak_semaphore * peak_semaphore;

#if defined(__cplusplus)
extern "C" {
#endif

extern peak_semaphore peak_semaphore_create(int value);

extern int peak_semaphore_signal(peak_semaphore semaphore);
extern int peak_semaphore_signal_all(peak_semaphore semaphore);
extern int peak_semaphore_wait(peak_semaphore semaphore);
extern int peak_semaphore_timedwait(peak_semaphore semaphore,
                                    peak_timespec_t wait_time);
extern int peak_semaphore_wait_signal(peak_semaphore wait_semaphore,
                                      peak_semaphore signal_semaphore);
extern int peak_semaphore_timedwait_signal(peak_semaphore wait_semaphore,
                                           peak_semaphore signal_semaphore,
                                           peak_timespec_t wait_time);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_SEMAPHORE_H_ */

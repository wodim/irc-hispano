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
 * $Id: peak.h,v 1.2 2004/01/09 00:24:38 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_PEAK_H_
#define INCLUDED_PEAK_PEAK_H_

/*! @mainpage libpeak
 *
 * Parallel Events and Asynchronous tasKing Library
 * 
 * @par
 * This library implements an environnement suitable for event-based
 * application and provides support for necessary multi-threading.
 * With libpeak, it is possible to process events in
 * parallel. It might bring gain on SMP computers. However, try to bring
 * kernel threads overhead to its minimum, so it shouldn't be used on
 * mono-processor machine, unless your application needs to do, for example,
 * lots of I/O operations that aren't handled by the library, along with other
 * calculation.
 * 
 * @par
 * But the peak library is more than that. It provides a set of modules
 * for efficient coding in an event-based environment and even extra modules
 * which can be found useful on classic applications too.
 * 
 * @par
 * @image html "../images/peak-task.png"
 */

#include <peak/alloc.h>
#include <peak/atomic.h>
#include <peak/db.h>
#include <peak/garbage.h>
#include <peak/mem_pool.h>
#include <peak/runtime.h>
#include <peak/signal.h>
#include <peak/stdint.h>
#include <peak/stream.h>
#include <peak/task.h>
#include <peak/time.h>
#include <peak/timer.h>
#include <peak/tz.h>
#include <peak/version.h>

#endif /* INCLUDED_PEAK_PEAK_H_ */

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
 * $Id: runtime.h,v 1.2 2005/01/27 16:31:50 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_RUNTIME_H_
#define INCLUDED_PEAK_RUNTIME_H_

#include <sys/types.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdlib.h>

#include <peak/stdint.h>

typedef void (*peak_runtime_init)(void *p, va_list vp, void *ctcx);
typedef void (*peak_runtime_finalize)(void *p);

struct _peak_runtime_class
    {
    const char *name;
    int size;
    const struct _peak_runtime_class *base;
    peak_runtime_init init;
    peak_runtime_finalize finalize;
    };

typedef const struct _peak_runtime_class peak_runtime_class;

#define PEAK_CLASS(name) (&_peak_##name##_class)

#define PEAK_CLASS_BASE_DECLARE(name) \
        static peak_runtime_class _peak_##name##_class = \
            { \
            "peak_" #name, \
            sizeof(struct __peak_##name), \
            NULL, \
            (peak_runtime_init)__peak_##name##_init, \
            (peak_runtime_finalize)__peak_##name##_finalize \
            }

#define PEAK_CLASS_DECLARE(name, base) \
        static peak_runtime_class _peak_##name##_class = \
            { \
            "peak_" #name, \
            sizeof(struct __peak_##name), \
            &_peak_##base##_class, \
            (peak_runtime_init)__peak_##name##_init, \
            (peak_runtime_finalize)__peak_##name##_finalize \
            }

#define PEAK_CLASS_BASE_DECLARE_VIRTUAL(name) \
        extern peak_runtime_class _peak_##name##_class

#define PEAK_CLASS_BASE_DEFINE_VIRTUAL(name) \
        peak_runtime_class _peak_##name##_class = \
            { \
            "peak_" #name, \
            sizeof(struct __peak_##name), \
            NULL, \
            (peak_runtime_init)__peak_##name##_init, \
            (peak_runtime_finalize)__peak_##name##_finalize \
            }

#define PEAK_CLASS_DECLARE_VIRTUAL(name, base) \
        extern peak_runtime_class _peak_##name##_class

#define PEAK_CLASS_DEFINE_VIRTUAL(name, base) \
        peak_runtime_class _peak_##name##_class = \
            { \
            "peak_" #name, \
            sizeof(struct __peak_##name), \
            &_peak_##base##_class, \
            (peak_runtime_init)__peak_##name##_init, \
            (peak_runtime_finalize)__peak_##name##_finalize \
            }

#define PEAK_CLASS_CONSTRUCT0(name) \
    (peak_##name)peak_construct(&_peak_##name##_class)
#define PEAK_CLASS_CONSTRUCT1(name, arg1) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1)
#define PEAK_CLASS_CONSTRUCT2(name, arg1, arg2) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2)
#define PEAK_CLASS_CONSTRUCT3(name, arg1, arg2, arg3) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2, arg3)
#define PEAK_CLASS_CONSTRUCT4(name, arg1, arg2, arg3, arg4) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2, arg3, arg4)
#define PEAK_CLASS_CONSTRUCT5(name, arg1, arg2, arg3, arg4, arg5) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2, arg3, \
                                arg4, arg5)
#define PEAK_CLASS_CONSTRUCT6(name, arg1, arg2, arg3, arg4, arg5, arg6) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2, arg3, \
                                arg4, arg5, arg6)
#define PEAK_CLASS_CONSTRUCT7(name, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (peak_##name)peak_construct(&_peak_##name##_class, arg1, arg2, arg3, \
                                arg4, arg5, arg6, arg7)


/* _rc -> refcount
 * bit 31 -> constant flag
 *
 */
struct _peak_runtime_base
  {
  peak_runtime_class * _cls;
  uint32_t _rc;
  };

typedef struct _peak_runtime_base peak_runtime_base;

#define PEAK_STRUCT_RT_HEADER peak_runtime_base _rt


#if defined(__cplusplus)
extern "C" {
#endif

extern void peak_ct_raise(const char *msg, int err, void *ctcx);

#define PEAK_CT_RAISE(m, e) peak_ct_raise(m, e, ctcx)
#define PEAK_CT_RAISE_IF_ERROR(r, m, e) if (r == -1) PEAK_CT_RAISE(m, e)

extern void * peak_construct(peak_runtime_class * cls, ...);

static inline peak_runtime_class *
peak_get_class(void *obj)
  {
  return ((peak_runtime_base *)obj)->_cls;
  }

#if defined(__cplusplus)
}
#endif


#endif /* INCLUDED_PEAK_RUNTIME_H_ */

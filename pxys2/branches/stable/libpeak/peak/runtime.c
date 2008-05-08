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
#define RCSID "$Id: runtime.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "internal.h" /* runtime.h is part of internal */
#include "spinlock.h"

#include <peak/alloc.h>

#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <setjmp.h>
#include <stdio.h>

/* runtime class ConsTructor ConteXt
 */
struct __peak_runtime_ctcx
  {
  jmp_buf jbuf; /* context buffer */
  const char *msg;
  int err;
  };

/* A global lock for convenience. This is a good way (tm).
 */
static peak_spinlock_t retain_lock = PEAK_SPINLOCK_INITIALIZER;

static void __peak_vconstruct(peak_runtime_base *p,
                              peak_runtime_class *cls,
                              va_list vp,
                              void *ctcx);
static void __peak_destroy(void *p);

void
peak_ct_raise(const char *msg, int err, void *ctcx)
  {
  struct __peak_runtime_ctcx *cx = (struct __peak_runtime_ctcx *)ctcx;
  cx->msg = msg;
  cx->err = err;
  longjmp(cx->jbuf, 1);
  }

void
__peak_vconstruct(peak_runtime_base *p, peak_runtime_class *cls,
                  va_list vp, void *ctcx)
  {
  p->_rc++;
  
  if (cls->base != NULL)
    __peak_vconstruct(p, cls->base, vp, ctcx); /* init base classes first */
  
  if (cls->init != NULL)
    cls->init((void *)p, vp, ctcx);

  p->_rc--;
  }

void *
peak_construct(peak_runtime_class * cls, ...)
  {
  peak_runtime_base *p;
  peak_runtime_class *c;
  va_list vl;
  struct __peak_runtime_ctcx ctcx; /* constructor context */
  
  p = (peak_runtime_base *)peak_allocate((size_t)cls->size);
  p->_cls = cls;
  
  /* During construction, _rc acts as a class inheritance depth counter */
  p->_rc = 0;
  
  va_start(vl, cls);
  ctcx.msg = NULL;
  if (!setjmp(ctcx.jbuf))
    {
    __peak_vconstruct(p, cls, vl, &ctcx);
    va_end(vl);
    p->_rc = 1; /* real retain count */
    return p;
    }
  /* "exception" raised */
  fprintf(stderr, "exception raised %s (%d)\n", ctcx.msg, ctcx.err);
  
  va_end(vl);
  assert(p->_rc > 0);
  
  /* do NOT leak: finalize already-constructed object's levels
   * (but we don't finalize failed level neither higher ones)
   * [like a C++ exception during ct]
   */
  for (p->_rc--, c = cls->base; c; c = c->base)
    {
    if (!p->_rc)
      {
      if (c->finalize)
        c->finalize(p);
      }
    else
      p->_rc--;
    }
  peak_deallocate(p);
  return NULL; /* Failed */
  }

static void
__peak_destroy(void *p)
  {
  peak_runtime_base *b = (peak_runtime_base *)p;
  peak_runtime_class *c;
  
  for (c = b->_cls; c; c = c->base)
    if (c->finalize)
      c->finalize(p);
  }

void *
peak_retain(void *obj)
  {
  uint32_t bits;
  peak_runtime_base * o = (peak_runtime_base *)obj;
    
  _peak_spinlock_lock(&retain_lock);
  bits = o->_rc;
  if (bits & 0x80000000)
    {
    PEAK_HALT; /* TODO */
    _peak_spinlock_unlock(&retain_lock);
    return obj;
    }
  bits++;
  o->_rc = bits;
  if ((bits & 0x7fffffff) == 0)
    {
    /* Leak: object constantification */
    _peak_spinlock_unlock(&retain_lock);
    return obj;
    }
  _peak_spinlock_unlock(&retain_lock);
  return obj;
  }

/* obj != NULL */
void
peak_release(void *obj)
  {
  uint32_t bits;
  peak_runtime_base * o = (peak_runtime_base *)obj;
  
  _peak_spinlock_lock(&retain_lock);
  bits = o->_rc;
  if (bits & 0x80000000)
    {
    /* Constant peak_t */
    _peak_spinlock_unlock(&retain_lock);
    return;
    }
  switch (bits)
    {
    case 0:
      PEAK_HALT;
    case 1:
      o->_rc = 0; /* for debugging */
      _peak_spinlock_unlock(&retain_lock);
      __peak_destroy(obj);
      peak_deallocate(obj);
      return;
    default:
      o->_rc = --bits;
      _peak_spinlock_unlock(&retain_lock);
      break;
    }
  }


int
peak_get_retcnt(void *obj)
  {
  peak_runtime_base * o = (peak_runtime_base *)obj;
  uint32_t rc = o->_rc; 
  return (int)rc;
  }

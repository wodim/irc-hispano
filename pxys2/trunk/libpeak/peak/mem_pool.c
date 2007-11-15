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
#define RCSID "$Id: mem_pool.c,v 1.2 2004/01/08 16:39:55 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/mem_pool.h>
#include <peak/alloc.h>
#include "internal.h"

#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>

#define PEAK_MEM_ALIGN(x,s) \
    (int)(((uint32_t)(x) + ((s)-1)) & ~((signed)((s)-1)))

static void __peak_mem_pool_init(peak_mem_pool pool, va_list vp, void *ctcx);
static void __peak_mem_pool_finalize(peak_mem_pool pool);


typedef struct __peak_mem_pool_entry
  {
  struct __peak_mem_pool_entry * next;
  } __peak_mem_pool_entry;

typedef struct __peak_mem_pool_chunk
  {
  struct __peak_mem_pool_chunk * next;
  __peak_mem_pool_entry * chunk;
  int size;
  } __peak_mem_pool_chunk;

struct __peak_mem_pool
  {
  PEAK_STRUCT_RT_HEADER;
  __peak_mem_pool_chunk * _heaphead;  /* associated heap chunks list */
  __peak_mem_pool_entry * _freehead;  /* head of available
                                       * mem_pool_entry list
                                       */
  unsigned int _object_size;
  unsigned int _power;                /* last power used */
  unsigned int _size;                 /* actual total size */
  unsigned int _allocated;            /* allocated pointers */
  };

PEAK_CLASS_BASE_DECLARE(mem_pool);

peak_mem_pool
peak_mem_pool_create(int object_size, int log_size)
  {
  return PEAK_CLASS_CONSTRUCT2(mem_pool, object_size, log_size);
  }

static void
__peak_mem_pool_init(peak_mem_pool pool, va_list vp, void *ctcx)
  {
  unsigned int i, size, logsize;
  __peak_mem_pool_entry * e;
  
  size = (unsigned int)va_arg(vp, int);
  logsize = (unsigned int)va_arg(vp, int);
  
  if (size < sizeof(void*))
    size = sizeof(void*);
  else
    size = PEAK_MEM_ALIGN(size, sizeof(void*));
  /*TODO: vector/altivec align*/
  
  pool->_object_size = size;
  pool->_power = logsize > 2 ? logsize : 2;
  pool->_size = 1 << pool->_power;
  pool->_allocated = 0;
  
  pool->_heaphead =
    (__peak_mem_pool_chunk *)peak_allocate(sizeof(__peak_mem_pool_chunk));
  pool->_heaphead->next = NULL;
  pool->_heaphead->chunk =
    (__peak_mem_pool_entry *)peak_allocate(pool->_size * pool->_object_size);
  pool->_heaphead->size = pool->_size;
  
  pool->_freehead = pool->_heaphead->chunk;
  e = pool->_freehead;
  
  for (i = 1; i < pool->_heaphead->size; i++, e = e->next)
    e->next = (__peak_mem_pool_entry *)(((intptr_t)pool->_heaphead->chunk)
                                        + (size_t)(i * pool->_object_size));
  e->next = NULL;
  }

static void
__peak_mem_pool_finalize(peak_mem_pool pool)
  {
  __peak_mem_pool_chunk *chunk, *next_chunk;
  
  for (chunk = pool->_heaphead; chunk; chunk = next_chunk)
    {
    next_chunk = chunk->next;
    peak_deallocate(chunk->chunk);
    peak_deallocate(chunk);
    }
  }

static void
__peak_mem_pool_grow(peak_mem_pool pool)
  {
  __peak_mem_pool_chunk *h, *p;
  __peak_mem_pool_entry *e;
  unsigned int size;
  unsigned int i;
  
  pool->_power++;
  size = 1 << pool->_power;
  
#if 0
  fprintf(stderr, "__peak_mem_pool_grow(%p) -> %d\n", pool, size);
#endif
  
  for (h = pool->_heaphead; h->next != NULL; h = h->next)
    ;
  
  p = (__peak_mem_pool_chunk *)peak_allocate(sizeof(__peak_mem_pool_chunk));
  p->next = NULL;
  p->chunk =
    (__peak_mem_pool_entry *)peak_allocate(size * pool->_object_size);
  
  p->size = size;
  
  h->next = p;
    
  e = p->chunk;
  for (i = 1; i < size; i++, e = e->next)
    e->next = (__peak_mem_pool_entry *)(((intptr_t)p->chunk)
                                        + (size_t)(i * pool->_object_size));
  e->next = NULL;
  
  if (pool->_freehead != NULL)
    e->next = pool->_freehead;
  pool->_freehead = p->chunk;

  pool->_size += size;
  }

void*
peak_mem_pool_new(peak_mem_pool pool)
  {
  __peak_mem_pool_entry * e = pool->_freehead;
  
  if (e == NULL)
    {
    __peak_mem_pool_grow(pool);
    e = pool->_freehead;
    if (e == NULL)
      PEAK_HALT; /* not cool */
    }
  pool->_freehead = pool->_freehead->next;
  pool->_allocated++;
  return e;
  }

void
peak_mem_pool_delete(peak_mem_pool pool, void *ptr)
  {
  __peak_mem_pool_entry * e = (__peak_mem_pool_entry *)ptr;
  
  e->next = pool->_freehead;
  pool->_freehead = e;
  pool->_allocated--;
  }

int
peak_mem_pool_get_used_count(peak_mem_pool pool)
  {
  return pool->_allocated;
  }

int
peak_mem_pool_get_free_count(peak_mem_pool pool)
  {
  return pool->_size - pool->_allocated;
  }

int
peak_mem_pool_get_size(peak_mem_pool pool)
  {
  return pool->_size;
  }

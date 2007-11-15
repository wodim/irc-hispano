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
#define RCSID "$Id: dict.c,v 1.3 2005/01/27 16:31:50 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/dict.h>
#include <peak/alloc.h>
#include <peak/mem_pool.h>
#include <peak/stdint.h>
#include "internal.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static const uint32_t __peak_dict_capacities[64] =
  {
          3,         7,        11,        17,        29,        41,        59,
         83,       113,       157,       211,       283,       379,       509,
        683,       919,      1229,      1657,      2213,      2953,      3943,
       5261,      7019,      9371,     12497,     16673,     22247,     29669,
      39563,     52757,     70351,     93809,    125093,    166799,    222403,
     296551,    395407,    527237,    702991,    937331,   1249799,   1666403,
    2221889,   2962549,   3950099,   5266823,   7022441,   9363283,  12484397,
   16645901,  22194539,  29592721,  39456979,  52609313,  70145797,  93527741,
  124703659, 166271551, 221695403, 295593889, 394125211, 525500351, 700667141,
  934222873 /* next rollover would be 1245630497 (prime 1245630509) */
  };

struct __peak_dict_entry
  {
  const void *key;
  const void *value;
  struct __peak_dict_entry *next;
  };

struct __peak_dict
  {
  PEAK_STRUCT_RT_HEADER;
  uint32_t _count;
  uint32_t _capacity;   /* not a fixed capacity (can be overloaded) */
  uint32_t _rollover;
  uint32_t _rollunder;
  peak_dict_key_ops_s _key_ops;
  peak_dict_value_ops_s _value_ops;
  struct __peak_dict_entry **_table;
  peak_mem_pool _pool;
  void *_context;
  };

static int __peak_dict_ptr_equal(const void *val1, const void *val2);
static uint32_t __peak_dict_ptr_hash(const void *value);
static int __peak_dict_string_equal(const void *val1, const void *val2);
static uint32_t __peak_dict_string_hash(const void *value);
static int __peak_dict_stringcase_equal(const void *val1, const void *val2);
static uint32_t __peak_dict_stringcase_hash(const void *value);

static void __peak_dict_init(peak_dict dict, va_list vp, void *ctcx);
static void __peak_dict_finalize(peak_dict dict);

static uint32_t __peak_dict_rollover(uint32_t capacity);
static uint32_t __peak_dict_rollunder(uint32_t capacity);
static void __peak_dict_grow(peak_dict dict, uint32_t count);
static void __peak_dict_add(peak_dict dict, const void *key,
                            const void *value, uint32_t h);

PEAK_CLASS_BASE_DECLARE(dict);

/* Built-in callbacks:
 * - no retain, no release, pointer equal and hash
 * - for strings: no retain, no release, string-based equal and hash
 * - for peak objects only: retain, release, peak-based equal and hash
 */

const peak_dict_key_ops_s peak_dict_null_key_ops =
  {
  NULL, NULL, __peak_dict_ptr_equal, __peak_dict_ptr_hash
  };

const peak_dict_key_ops_s peak_dict_string_key_ops =
  {
  NULL, NULL, __peak_dict_string_equal, __peak_dict_string_hash
  };

const peak_dict_key_ops_s peak_dict_string_copy_key_ops =
  {
  (peak_dict_retain_callback)peak_strdup,
  (peak_dict_release_callback)peak_deallocate,
  __peak_dict_string_equal,
  __peak_dict_string_hash
  };

const peak_dict_key_ops_s peak_dict_stringcase_key_ops =
  {
  NULL, NULL, __peak_dict_stringcase_equal, __peak_dict_stringcase_hash
  };

const peak_dict_key_ops_s peak_dict_stringcase_copy_key_ops =
  {
  (peak_dict_retain_callback)peak_strdup,
  (peak_dict_release_callback)peak_deallocate,
  __peak_dict_stringcase_equal,
  __peak_dict_stringcase_hash
  };

#if 0
const peak_dict_key_ops_s peak_dict_peak_key_ops =
  {
  peak_retain, peak_release, peak_equal, peak_hash
  };
#endif

const peak_dict_value_ops_s peak_dict_null_value_ops =
  {
  NULL, NULL
  };

const peak_dict_value_ops_s peak_dict_string_value_ops =
  {
  NULL, NULL
  };

const peak_dict_value_ops_s peak_dict_string_copy_value_ops =
  {
  (peak_dict_retain_callback)peak_strdup,
  (peak_dict_release_callback)peak_deallocate
  };




/* for poor pointers */

static int
__peak_dict_ptr_equal(const void *val1, const void *val2)
  {
  return !(val1 == val2);
  }

static uint32_t
__peak_dict_ptr_hash(const void *value)
  {
  unsigned char *c = (unsigned char *)&value;
  uint32_t hash = 0;
  int i;
  
  for (i = 0; i < (int)sizeof(void *); i++)
    hash = (hash * 33) + c[i];
  
  return hash;
  }

/* for strings */

static int
__peak_dict_string_equal(const void *val1, const void *val2)
  {
  const char *s1 = (const char *)val1;
  const char *s2 = (const char *)val2;
  
  while (*s1 == *s2++)
    if (*s1++ == 0)
      return 0;
  return 1; /* we don't need to compare characters here.. */
  }

static uint32_t
__peak_dict_string_hash(const void *value)
  {
  const unsigned char *p = (unsigned char *)value;
  uint32_t hash = 0;
  
  for (; *p; p++)
    hash = hash * 33 + *p;
  
  return hash;
  }

/* for strings, ignoring case */

static int
__peak_dict_stringcase_equal(const void *val1, const void *val2)
  {
  const char *s1 = (const char *)val1;
  const char *s2 = (const char *)val2;
  
  while (tolower(*s1) == tolower(*s2++))
    if (*s1++ == 0)
      return 0;
  return 1; /* we don't need to compare characters here.. */
  }

static uint32_t
__peak_dict_stringcase_hash(const void *value)
  {
  const unsigned char *p = (unsigned char *)value;
  uint32_t hash = 0;
  
  for (; *p; p++)
    hash = hash * 33 + tolower(*p);
  
  return hash;
  }


static uint32_t
__peak_dict_capacity(uint32_t capacity)
  {
  int i;
  for (i = 0; i < 63 && __peak_dict_capacities[i] < capacity; i++)
    ;
  return __peak_dict_capacities[i];
  }

static uint32_t
__peak_dict_rollover(uint32_t capacity)
  {
  return ((capacity * 4) / 3);
  }

static uint32_t
__peak_dict_rollunder(uint32_t capacity)
  {
  return ((capacity * 3) / 4);
  }

peak_dict
peak_dict_create(const peak_dict_key_ops_s *key_ops,
                 const peak_dict_value_ops_s *value_ops,
                 const peak_dict_init_entry *init_values,
                 int num_values)
  {
  return PEAK_CLASS_CONSTRUCT4(dict, key_ops, value_ops,
                               init_values, num_values);
  }

static void
__peak_dict_init(peak_dict dict, va_list vp, void *ctcx)
  {
  const peak_dict_key_ops_s *key_ops;
  const peak_dict_value_ops_s *value_ops;
  const peak_dict_init_entry *values;
  int i;
  int num_values;
  
  dict->_count = 0;
  dict->_capacity = 0;
  key_ops = va_arg(vp, const peak_dict_key_ops_s *);
  value_ops = va_arg(vp, const peak_dict_value_ops_s *);
  
  if (!key_ops)
    key_ops = &peak_dict_null_key_ops;
  
  if (!value_ops)
    value_ops = &peak_dict_null_value_ops;
  
  /* Copy key operations */
  dict->_key_ops = *key_ops;
  
  /* If equal is NULL, pointer equality is used */
  if (!dict->_key_ops.equal)
    dict->_key_ops.equal = __peak_dict_ptr_equal;
  /* If hash is NULL, pointer hashing is used */
  if (!dict->_key_ops.hash)
    dict->_key_ops.hash = __peak_dict_ptr_hash;
  
  /* Copy value operations */
  dict->_value_ops = *value_ops;
  
  values = va_arg(vp, const peak_dict_init_entry *);
  num_values = va_arg(vp, int);
  
  if (num_values == 0)
    {
    dict->_rollover = 0;
    dict->_rollunder = 0;
    dict->_table = NULL;
    dict->_pool = NULL;
    }
  else if (num_values < 0)
    {
    PEAK_CT_RAISE("negative num_values invalid", 0);
    }
  else
    {
    /* init values provided => immediate grow */
    __peak_dict_grow(dict, num_values);
    
    /* add (key,value) pairs */
    for (i = 0; i < num_values; i++)
      peak_dict_add(dict, values[i].key, values[i].value);
    }
  }

static void
__peak_dict_finalize(peak_dict dict)
  {
  struct __peak_dict_entry *e;
  uint32_t i;
  
  /* Release table's entries if needed - faster than a real _clear() */
  if (dict->_key_ops.release || dict->_value_ops.release)
    {
    for (i = 0; i < dict->_capacity && dict->_count > 0; i++)
      {
      for(e = dict->_table[i]; e; e = e->next)
        {
        if (dict->_key_ops.release)
          (*dict->_key_ops.release)(e->key);
        
        if (dict->_value_ops.release)
          (*dict->_value_ops.release)(e->value);
        
        dict->_count--;
        }
      }
    assert(dict->_count == 0);
    }
  
  if (dict->_pool)
    peak_release(dict->_pool);
  
  peak_deallocate(dict->_table);
  }

static void
__peak_dict_grow(peak_dict dict, uint32_t count)
  {
  uint32_t new_capacity, h;
  uint32_t i;
  
  new_capacity = __peak_dict_capacity(count);
  if (new_capacity <= dict->_capacity) /* uh */
    return;
  
  if (dict->_capacity == 0)
    {
    /* Set a first (average) capacity */
    dict->_capacity = new_capacity;
    
    /* Allocate a fresh table */
    dict->_table =
      peak_allocate(dict->_capacity * sizeof(struct __peak_dict_entry *));
    for (i = 0; i < dict->_capacity; i++)
      dict->_table[i] = NULL;
    
    /* And a fresh memory pool to recycle our entries */
    
    /* Calculate a log size large enough */
    for (i = 1; i < 31; i++)
      if ((1 << i) >= count)
        break;
    
    dict->_pool = peak_mem_pool_create(sizeof(struct __peak_dict_entry), i);
    }
  else
    {
    struct __peak_dict_entry **new_table, *e;
    
    new_table =
      peak_allocate(new_capacity * sizeof(struct __peak_dict_entry *));
    for (i = 0; i < new_capacity; i++)
      new_table[i] = NULL;
    
    /* Rehash the table */
    for (i = 0; i < dict->_capacity; i++)
      {
      for (e = dict->_table[i]; e; e =  dict->_table[i])
        {
        /* Remove from list */
        dict->_table[i] = e->next;
        /* Calculate new hash value */
        h = (*dict->_key_ops.hash)(e->key) % new_capacity;
        /* Reposition */
        e->next = new_table[h];
        new_table[h] = e;
        }
      }
    
    /* Update values and replace table */
    dict->_capacity = new_capacity;
    
    peak_deallocate(dict->_table);
    dict->_table = new_table;
    }
  
  /* Finally, update bounds in which we won't modify the capacity */
  dict->_rollover = __peak_dict_rollover(dict->_capacity);
  dict->_rollunder = __peak_dict_rollunder(dict->_capacity);
  }

int
peak_dict_get_count(peak_dict dict)
  {
  return dict->_count;
  }

const void*
peak_dict_get_value(peak_dict dict, const void *key)
  {
  struct __peak_dict_entry *e;
  uint32_t h;
  
  if (dict->_count > 0)
    {
    h = (*dict->_key_ops.hash)(key) % dict->_capacity;
    for (e = dict->_table[h]; e; e = e->next)
      if (!(*dict->_key_ops.equal)(e->key, key))
        return e->value;
    }
  return NULL;
  }

void
peak_dict_get_all(peak_dict dict, const void **keys, const void **values)
  {
  struct __peak_dict_entry *e;
  uint32_t i;
  uint32_t cnt = dict->_count;
  
  for (i = 0; i < dict->_capacity && cnt > 0; i++)
    {
    for(e = dict->_table[i]; e; e = e->next)
      {
      if (keys)
        *keys++ = e->key;
      if (values)
        *values++ = e->value;
      cnt--;
      }
    }
  }

void
peak_dict_apply(peak_dict dict, peak_dict_apply_func applier, void *context)
  {
  struct __peak_dict_entry *e;
  uint32_t i;
  uint32_t cnt = dict->_count;
  
  for (i = 0; i < dict->_capacity && cnt > 0; i++)
    {
    for(e = dict->_table[i]; e; e = e->next)
      {
      (*applier)(e->key, e->value, context);
      cnt--;
      }
    }
  }

/* Note: h is a real hash value without the modulus operation
 */
static void
__peak_dict_add(peak_dict dict, const void *key, const void *value, uint32_t h)
  {
  struct __peak_dict_entry *e;
  
  /* Verify hash table size, grow it if needed (capacity will change) */
  if (++dict->_count > dict->_rollover)
    __peak_dict_grow(dict, dict->_count);
  
  /* Allocate new entry via our memory pool */
  e = peak_mem_pool_new(dict->_pool);
  
  /* Retain key if needed */
  if (dict->_key_ops.retain)
    (*dict->_key_ops.retain)(key);
  
  /* Retain value if needed */
  if (dict->_value_ops.retain)
    (*dict->_value_ops.retain)(value);
  
  /* Set entry's fields */
  e->key = key;
  e->value = value;
  
  /* Do the modulus operation on the hash value */
  h %= dict->_capacity;
  
  /* Link entry */
  e->next = dict->_table[h];
  dict->_table[h] = e;
  }

void
peak_dict_add(peak_dict dict, const void *key, const void *value)
  {
  struct __peak_dict_entry *e;
  uint32_t h;
  
  /* Hash the key but don't do the modulus operation */
  h = (*dict->_key_ops.hash)(key);
  
  /* "Add if absent" - so, try to find it first */
  if (dict->_count > 0)
    {
    for (e = dict->_table[h % dict->_capacity]; e; e = e->next)
      if (!(*dict->_key_ops.equal)(e->key, key))
        return; /* found ! */
    }
  
  /* Do low level add */
  __peak_dict_add(dict, key, value, h);
  }

void
peak_dict_set(peak_dict dict, const void *key, const void *value)
  {
  struct __peak_dict_entry *e;
  uint32_t h;
  
  /* Hash the key */
  h = (*dict->_key_ops.hash)(key);
  
  /* "Add if absent, replace if present" - so, try to find it first */
  if (dict->_count > 0)
    {
    for (e = dict->_table[h % dict->_capacity]; e; e = e->next)
      if (!(*dict->_key_ops.equal)(e->key, key))
        {
        /* Entry exists => Replace */
        if (dict->_value_ops.release)
          (*dict->_value_ops.release)(e->value); /* release old value */
        if (dict->_value_ops.retain)
          (*dict->_value_ops.retain)(value); /* retain new value if needed */
        e->value = value;
        return;
        }
    }
  
  /* Do low level add */
  __peak_dict_add(dict, key, value, h);
  }

void
peak_dict_replace(peak_dict dict, const void *key, const void *value)
  {
  struct __peak_dict_entry *e;
  uint32_t h;
  
  /* Hash the key */
  h = (*dict->_key_ops.hash)(key);
  
  /* "Replace if present" - try to find it */
  if (dict->_count > 0)
    {
    for (e = dict->_table[h % dict->_capacity]; e; e = e->next)
      if (!(*dict->_key_ops.equal)(e->key, key))
        {
        /* We found it, replace the value ! */
        if (dict->_value_ops.release)
          (*dict->_value_ops.release)(e->value); /* release old value */
        if (dict->_value_ops.retain)
          (*dict->_value_ops.retain)(value); /* retain new value if needed */
        e->value = value;
        return;
        }
    }
  
  /* Do nothing if we didn't find it. */
  }

void
peak_dict_remove(peak_dict dict, const void *key)
  {
  struct __peak_dict_entry *e, *eprev, *enext;
  uint32_t h;
  
  /* Hash the key */
  h = (*dict->_key_ops.hash)(key);
  
  /* Remove only if present (what a damn logic) */
  if (dict->_count > 0)
    {
    eprev = NULL;
    for (e = dict->_table[h % dict->_capacity]; e; eprev = e, e = enext)
      {
      enext = e->next;
      if (!(*dict->_key_ops.equal)(e->key, key))
        {
        /* We found it ! Unlink, release and free memory.*/
        if (eprev)
          eprev->next = enext;
        
        if (dict->_key_ops.release)
          (*dict->_key_ops.release)(e->key);
        
        if (dict->_value_ops.release)
          (*dict->_value_ops.release)(e->value);
        
        peak_mem_pool_delete(dict->_pool, e);
        return;
        }
      }
    }
  
  /* Do nothing if we didn't find it. */
  }

void
peak_dict_clear(peak_dict dict)
  {
  struct __peak_dict_entry *e, *enext;
  uint32_t i;
  
  /* Release table's entries if needed */
  for (i = 0; i < dict->_capacity && dict->_count > 0; i++)
    {
    /* skip empty slots */
    if (!(e = dict->_table[i]))
      continue;
    
    /* for each slot... */
    for(; e; e = enext)
      {
      enext = e->next;
      
      /* release stuffs if needed */
      if (dict->_key_ops.release)
        (*dict->_key_ops.release)(e->key);
      
      if (dict->_value_ops.release)
        (*dict->_value_ops.release)(e->value);
      
      /* free entry */
      peak_mem_pool_delete(dict->_pool, e);
      dict->_count--;
      }
    /* clear table */
    dict->_table[i] = NULL;
    }
  
  assert(dict->_count == 0);
  }



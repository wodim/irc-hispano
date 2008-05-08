/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#define RCSID "$Id"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ipcache.h"

#include <peak/peak.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef IPCACHE_TEST
#include <arpa/inet.h>
#endif

struct __ipcache_in4_entry
  {
  struct __ipcache_in4_entry *next;
  struct in_addr key;
  int lru_ring_refcnt;
  time_t ts;
  char data[0]; /* append optional data here */
  };

typedef struct __ipcache_in4_entry hentry4;

struct __ipcache_in6_entry
  {
  struct __ipcache_in6_entry *next;
  struct in6_addr key;
  int lru_ring_refcnt;
  time_t ts;
  char data[0]; /* append optional data here */
  };

typedef struct __ipcache_in6_entry hentry6;

typedef struct __ipcache_lru_ring *ipcache_lru_ring_t;

struct __ipcache
  {
  int buckets;
  int count;
  int maxcount;
  size_t datasize;
  int flags;
  peak_mem_pool mem_pool;
  union
    {
    struct __ipcache_in4_entry **v4;
    struct __ipcache_in6_entry **v6;
    } table;
  ipcache_lru_ring_t lru_ring;
  };

typedef void (*ipcache_lru_ring_release_func)(ipcache_t cp, void *obj);

struct __ipcache_lru_ring
  {
  int size;
  int count;
  int pos_start;
  int pos_end;
  ipcache_t cache;
  void **address;
  ipcache_lru_ring_release_func rel_fun;
  };

static uint32_t hashfun_in4(const struct in_addr *addrptr);
static uint32_t hashfun_in6(const struct in6_addr *addrptr);

static int ipcache_hash_remove_in4(ipcache_t cp, struct in_addr *addr);
static int ipcache_hash_remove_in6(ipcache_t cp, struct in6_addr *addr);
static void ipcache_release_in4(ipcache_t cp, hentry4 *e);
static int ipcache_dispose_in4(ipcache_t cp, hentry4 *e);
static void ipcache_release_in6(ipcache_t cp, hentry6 *e);
static int ipcache_dispose_in6(ipcache_t cp, hentry6 *e);

static ipcache_lru_ring_t ipcache_lru_ring_create(ipcache_t cp, size_t size,
                                 ipcache_lru_ring_release_func rel_fun);
static void ipcache_lru_ring_dispose(ipcache_lru_ring_t ring);
static void ipcache_lru_ring_add(ipcache_lru_ring_t ring, void *obj);
static int ipcache_lru_ring_clear(ipcache_lru_ring_t ring);

#ifdef IPCACHE_TEST
static int coll;
static int llen;
#endif

static uint32_t
hashfun_in4(const struct in_addr *addrptr)
  {
  return (uint32_t)addrptr->s_addr;
  }

static uint32_t
hashfun_in6(const struct in6_addr *addrptr)
  {
  const u_char *key = (u_char *)addrptr;
  uint32_t h = 0;
  
#define HASH4 \
  h = h * 33 + *key++; \
  h = h * 33 + *key++; \
  h = h * 33 + *key++; \
  h = h * 33 + *key++
  
  HASH4;
  HASH4;
  HASH4;
  HASH4;
  
  return (h);
  }

ipcache_t
ipcache_create_in4(int size, size_t datasize)
  {
  int pos;
  ipcache_t cp = (ipcache_t)peak_allocate(sizeof(struct __ipcache));
  cp->buckets = size;
  cp->count = 0;
  cp->maxcount = size;
  cp->datasize = datasize;
  cp->flags = 0;
  cp->mem_pool = peak_mem_pool_create(sizeof(hentry4) + cp->datasize, 0);
  cp->table.v4 = (hentry4 **)peak_allocate(sizeof(hentry4 *) * cp->buckets);
  
  for (pos = cp->buckets; pos--; )
    cp->table.v4[pos] = NULL;
  
  cp->lru_ring = ipcache_lru_ring_create(cp, size,
                         (ipcache_lru_ring_release_func)ipcache_release_in4);
  
  return cp;
  }

ipcache_t
ipcache_create_in6(int size, size_t datasize)
  {
  int pos;
  ipcache_t cp = (ipcache_t)peak_allocate(sizeof(struct __ipcache));
  cp->buckets = size;
  cp->count = 0;
  cp->maxcount = size;
  cp->datasize = datasize;
  cp->flags = 1;
  cp->mem_pool = peak_mem_pool_create(sizeof(hentry6) + cp->datasize, 0);
  cp->table.v6 = (hentry6 **)peak_allocate(sizeof(hentry6 *) * cp->buckets);
  
  for (pos = cp->buckets; pos--; )
    cp->table.v6[pos] = NULL;
  
  cp->lru_ring = ipcache_lru_ring_create(cp, size,
                         (ipcache_lru_ring_release_func)ipcache_release_in6);
  
  return cp;
  }

void
ipcache_dispose(ipcache_t cp)
  {
  ipcache_lru_ring_dispose(cp->lru_ring);
  peak_release(cp->mem_pool);
  peak_deallocate((void *)cp->table.v4);
  peak_deallocate(cp);
  }

int
ipcache_clear(ipcache_t cp)
  {
  int result = ipcache_lru_ring_clear(cp->lru_ring);
  assert (cp->count == 0);
  return result;
  }

void
ipcache_add_in4(ipcache_t cp, const struct in_addr *addr, time_t t,
                const void *data)
  {
  hentry4 *e;
  uint32_t slot = hashfun_in4(addr) % cp->buckets;
  
  for (e = cp->table.v4[slot]; e; e = e->next)
    {
    if (e->key.s_addr == addr->s_addr)
      {
      e->lru_ring_refcnt++;
      e->ts = t;
      ipcache_lru_ring_add(cp->lru_ring, e);
      return;
      }
    }
  
  e = (hentry4 *)peak_mem_pool_new(cp->mem_pool);
  e->key = *addr;
  e->lru_ring_refcnt = 0;
  e->ts = t;
  if (cp->datasize > 0)
    memcpy(e->data, data, cp->datasize);
  
  e->next = cp->table.v4[slot];

#ifdef IPCACHE_TEST
  if (e->next != NULL)
    {
    int cnt;
    hentry4 *ti = e;
    coll++;
    for (cnt = 0; ti->next != NULL; ti = ti->next)
      cnt++;
    if (cnt > llen)
      llen = cnt;
    }
#endif
  cp->table.v4[slot] = e;
  cp->count++;
  
  e->lru_ring_refcnt++;
  ipcache_lru_ring_add(cp->lru_ring, e);
  }

void
ipcache_add_in6(ipcache_t cp, const struct in6_addr *addr, time_t t,
                const void *data)
  {
  hentry6 *e;
  uint32_t slot = hashfun_in6(addr) % cp->buckets;
  
  for (e = cp->table.v6[slot]; e; e = e->next)
    {
    if (memcmp(&e->key, addr, sizeof(struct in6_addr)) == 0)
      {
      e->lru_ring_refcnt++;
      e->ts = t;
      ipcache_lru_ring_add(cp->lru_ring, e);
      return;
      }
    }
  
  e = (hentry6 *)peak_mem_pool_new(cp->mem_pool);
  e->key = *addr;
  e->lru_ring_refcnt = 0;
  e->ts = t;
  
  if (cp->datasize > 0)
    memcpy(e->data, data, cp->datasize);
  e->next = cp->table.v6[slot];
  cp->table.v6[slot] = e;
  cp->count++;
    
  e->lru_ring_refcnt++;
  ipcache_lru_ring_add(cp->lru_ring, e);
  }

int
ipcache_invalidate_in4(ipcache_t cp, const struct in_addr *addr)
  {
  hentry4 *e;
  uint32_t slot = hashfun_in4(addr) % cp->buckets;
  
  for (e = cp->table.v4[slot]; e; e = e->next)
    {
    if (e->key.s_addr == addr->s_addr && e->ts != 0)
      {
      printf("ipcache_invalidate_in4: found!\n");
      e->ts = 0;
      return 0;
      }
    }
  printf("ipcache_invalidate_in4: not found!\n");
  return -1;
  }

int
ipcache_invalidate_in6(ipcache_t cp, const struct in6_addr *addr)
  {
  hentry6 *e;
  uint32_t slot = hashfun_in6(addr) % cp->buckets;
    
  for (e = cp->table.v6[slot]; e; e = e->next)
    {
    if (memcmp(&e->key, addr, sizeof(struct in6_addr)) == 0
        && e->ts != 0)
      {
      e->ts = 0;
      return 0;
      }
    }
  return -1;
  }

static int
ipcache_hash_remove_in4(ipcache_t cp, struct in_addr *addr)
  {
  hentry4 **e, *kill;
  uint32_t slot = hashfun_in4(addr) % cp->buckets;
  
  for (e = &cp->table.v4[slot]; *e; e = &((*e)->next))
    {
    if ((*e)->key.s_addr == addr->s_addr)
      {
      cp->count--;
      kill = *e;
      *e = (*e)->next;
      peak_mem_pool_delete(cp->mem_pool, kill);
      return 0;
      }
    }
  return -1;
  }

static int
ipcache_hash_remove_in6(ipcache_t cp, struct in6_addr *addr)
  {
  hentry6 **e, *kill;
  uint32_t slot = hashfun_in6(addr) % cp->buckets;
  
  for (e = &cp->table.v6[slot]; *e; e = &((*e)->next))
    {
    if (memcmp(&(*e)->key, addr, sizeof(struct in6_addr)) == 0)
      {
      cp->count--;
      kill = *e;
      *e = (*e)->next;
      peak_mem_pool_delete(cp->mem_pool, kill);
      return 0;
      }
    }
  return -1;
  }


static void
ipcache_release_in4(ipcache_t cp, hentry4 *e)
  {
  int err;
  
  assert(e->lru_ring_refcnt > 0);
  
  if (!--e->lru_ring_refcnt)
    {
    err = ipcache_dispose_in4(cp, e);
    assert (err == 0);
    }
  }

static int
ipcache_dispose_in4(ipcache_t cp, hentry4 *e)
  {
  return ipcache_hash_remove_in4(cp, &e->key);
  }

static void
ipcache_release_in6(ipcache_t cp, hentry6 *e)
  {
  int err;
  
  assert(e->lru_ring_refcnt > 0);
  
  if (!--e->lru_ring_refcnt)
    {
    err = ipcache_dispose_in6(cp, e);
    assert (err == 0);
    }
  }

static int
ipcache_dispose_in6(ipcache_t cp, hentry6 *e)
  {
  return ipcache_hash_remove_in6(cp, &e->key);
  }

time_t
ipcache_find_in4(ipcache_t cp, const struct in_addr *addr, void **od)
  {
  hentry4 *e;
  uint32_t slot = hashfun_in4(addr) % cp->buckets;
  
  for (e = cp->table.v4[slot]; e; e = e->next)
    if (e->key.s_addr == addr->s_addr)
      {
      if (od && cp->datasize)
        *od = e->data;
      return e->ts;
      }
  
  return 0;
  }

time_t
ipcache_find_in6(ipcache_t cp, const struct in6_addr *addr, void **od)
  {
  hentry6 *e;
  uint32_t slot = hashfun_in6(addr) % cp->buckets;
  
  for (e = cp->table.v6[slot]; e; e = e->next)
    if (memcmp(&e->key, addr, sizeof(struct in6_addr)) == 0)
      {
      if (od && cp->datasize)
        *od = e->data;
      return e->ts;
      }
  
  return 0;
  }

size_t
ipcache_get_count(ipcache_t cp)
  {
  return cp->lru_ring->count;
  }

/**************** Associated internal LRU Ring *****************/

static ipcache_lru_ring_t
ipcache_lru_ring_create(ipcache_t cp, size_t size,
                        ipcache_lru_ring_release_func rel_fun)
  {
  ipcache_lru_ring_t ring =
    (ipcache_lru_ring_t)peak_allocate(sizeof(struct __ipcache_lru_ring));
  ring->size = size;
  ring->count = 0;
  ring->address = (void **)peak_allocate(sizeof(void *) * ring->size);
  ring->pos_start = 0;
  ring->pos_end = -1;
  ring->cache = cp;
  ring->rel_fun = rel_fun;
  return ring;
  }

static void
ipcache_lru_ring_dispose(ipcache_lru_ring_t ring)
  {
  peak_deallocate(ring->address);
  peak_deallocate(ring);
  }

static void
ipcache_lru_ring_add(ipcache_lru_ring_t ring, void *obj)
  {
  if (++ring->pos_end >= ring->size)
    ring->pos_end = 0;
  
  if (ring->count >= ring->size)
    {
    assert (ring->pos_end == ring->pos_start);
    assert (ring->count == ring->size);
    
    (*ring->rel_fun)(ring->cache, ring->address[ring->pos_end]);
    
    if (++ring->pos_start >= ring->size)
      ring->pos_start = 0;
    }
  else
    ring->count++;
  
  ring->address[ring->pos_end] = obj;
  }

static int
ipcache_lru_ring_clear(ipcache_lru_ring_t ring)
  {
  int result = ring->count;
  
  while (ring->count > 0)
    {
    (*ring->rel_fun)(ring->cache, ring->address[ring->pos_start]);
    if (++ring->pos_start >= ring->size)
      ring->pos_start = 0;
    ring->count--;
    }
  assert(ring->pos_start == ring->pos_end);
  
  return result;
  }

/************************ Special cleanup ************************/

size_t
ipcache_cleanup(ipcache_t cp, time_t expire_ts)
  {
  ipcache_lru_ring_t ring = cp->lru_ring;
  size_t count = 0;
  
  if (cp->flags & 1) /* ipv6 */
    {
    hentry6 *e6;
    
    for (e6 = (hentry6 *)ring->address[ring->pos_start];
         ring->count > 0 && e6->ts <= expire_ts;
         e6 = (hentry6 *)ring->address[ring->pos_start])
      {
      (*ring->rel_fun)(ring->cache, ring->address[ring->pos_start]);
      if (++ring->pos_start >= ring->size)
        ring->pos_start = 0;
      ring->count--;
      count++;
      }
    }
  else
    {
    hentry4 *e4;
    
    for (e4 = (hentry4 *)ring->address[ring->pos_start];
         ring->count > 0 && e4->ts <= expire_ts;
         e4 = (hentry4 *)ring->address[ring->pos_start])
      {
      (*ring->rel_fun)(ring->cache, ring->address[ring->pos_start]);
      if (++ring->pos_start >= ring->size)
        ring->pos_start = 0;
      ring->count--;
      count++;
      }
    }
  return count;
  }


/************************ Cache storage ************************/

typedef struct __ipcache_file_header
  {
  uint32_t sig;             // file signature
  int32_t last_update;      // file last update time
  uint32_t sizeof_entry;    // size of an entry in bytes
  uint32_t count;           // number of entries
  } ipcache_file_header;

typedef struct __ipcache_file_entry4
  {
  struct in_addr addr;
  time_t ts;
  char data[0];
  } ipcache_file_entry4;

typedef struct __ipcache_file_entry6
  {
  struct in6_addr addr;
  time_t ts;
  char data[0];
  } ipcache_file_entry6;


#define SWAP_NMEMB 16

int
ipcache_swapin(ipcache_t cp, const char *file, uint32_t sig)
  {
  int res = -1;
  FILE *fp;
  ipcache_file_header head;
  ipcache_file_entry4 *e4p, *e4p2;
  ipcache_file_entry6 *e6p, *e6p2;
  size_t size, i, n;
  uint32_t cnt = 0;
  
  if ((fp = fopen(file, "r"))  == NULL)
    return res;
  
  if (fread(&head, sizeof(head), 1, fp) < 1 || (head.sig != sig))
    goto swapin_failure;
  
  if (cp->flags & 1)
    {
    size = sizeof(ipcache_file_entry6) + cp->datasize;
    if (head.sizeof_entry != size)
      goto swapin_failure;
    
    e6p = (ipcache_file_entry6 *)peak_allocate(size * SWAP_NMEMB);
    
    while ((n = fread(e6p, size, SWAP_NMEMB, fp)) > 0)
      for (i = 0, e6p2 = e6p; i < n; i++, cnt++, e6p2 = (void*)((char*)e6p2 + size))
        ipcache_add_in6(cp, &e6p2->addr, e6p2->ts, e6p2->data);
    
    peak_deallocate(e6p);
    }
  else
    {
    size = sizeof(ipcache_file_entry4) + cp->datasize;
    if (head.sizeof_entry != size)
      goto swapin_failure;
    
    e4p = (ipcache_file_entry4 *)peak_allocate(size * SWAP_NMEMB);
    
    while ((n = fread(e4p, size, SWAP_NMEMB, fp)) > 0)
      for (i = 0, e4p2 = e4p; i < n; i++, cnt++, e4p2 = (void*)((char*)e4p2) + size)
        ipcache_add_in4(cp, &e4p2->addr, e4p2->ts, e4p2->data);
    
    peak_deallocate(e4p);
    }
  
  if (head.count == cnt)
    res = 0; /* success */
  
swapin_failure:
  fclose(fp);
  return res;
  }

int
ipcache_swapout(ipcache_t cp, const char *file, uint32_t sig)
  {
  int res = -1;
  FILE *fp;
  ipcache_file_header head;
  ipcache_lru_ring_t ring = cp->lru_ring;
  size_t size;
  int i, pos;
  
  if ((fp = fopen(file, "w"))  == NULL)
    return res;
  
  size = (cp->flags & 1) ? sizeof(ipcache_file_entry6)
                         : sizeof(ipcache_file_entry4);
  size += cp->datasize;
  
  head.sig = sig;
  head.last_update = peak_time();
  head.sizeof_entry = size;
  head.count = ring->count;
  
  if (fwrite(&head, sizeof(head), 1, fp) < 1)
    goto swapout_failure;
  
  if (cp->flags & 1) /* ipv6 */
    {
    ipcache_file_entry6 *fe6p = 
      (ipcache_file_entry6 *)peak_allocate(size);
    hentry6 *he6p;
    
    for (i = ring->count, pos = ring->pos_start; i > 0; i--)
      {
      he6p = (hentry6 *)ring->address[pos];
      fe6p->addr = he6p->key;
      fe6p->ts = he6p->ts;
      if (cp->datasize > 0)
        memcpy(fe6p->data, he6p->data, cp->datasize);
      if (fwrite(fe6p, size, 1, fp) < 1)
        goto swapout_failure;
      
      if (++pos >= ring->size)
        pos = 0;
      }
    peak_deallocate(fe6p);
    }
  else
    {
    ipcache_file_entry4 *fe4p =
      (ipcache_file_entry4 *)peak_allocate(size);
    hentry4 *he4p;
    
    for (i = ring->count, pos = ring->pos_start; i > 0; i--)
      {
      he4p = (hentry4 *)ring->address[pos];
      fe4p->addr = he4p->key;
      fe4p->ts = he4p->ts;
      if (cp->datasize > 0)
        memcpy(fe4p->data, he4p->data, cp->datasize);
      if (fwrite(fe4p, size, 1, fp) < 1)
        goto swapout_failure;
      
      if (++pos >= ring->size)
        pos = 0;
      }
    peak_deallocate(fe4p);
    }
  res = 0;
swapout_failure:
  fclose(fp);
  return res;
  }



/* Test */

#ifdef IPCACHE_TEST
int
main(int argc, char *argv[])
  {
  ipcache_t cache = ipcache_create_in4(CACHESIZE, 0);
  struct in_addr addr;
  int i, j, res;
  int found = 0, loaded = 0;
  FILE *fp;
  char buf[32];
  struct in_addr *randips = malloc(sizeof(struct in_addr) * LOOPS);
  
  fp = fopen("urandip.txt", "r");
  assert(fp);

  i = 0;
  while(fgets(buf, sizeof(buf), fp))
    {
    buf[strlen(buf)-1] = '\0';
    randips[i++].s_addr = inet_addr(buf);
    loaded++;
    }
  
  for (j = 0; i < LOOPS; i++, j++)
    {
    randips[i] = randips[j];
    randips[i].s_addr += 2;
    }
  fclose(fp);
  
  for (i = 0; i < LOOPS; i++)
    {
    addr = randips[i];
    if (ipcache_find_in4(cache, &addr, NULL) != 0)
      found++;
    
    ipcache_add_in4(cache, &addr, peak_time(), NULL);
    }
  
#if CACHESIZE >= LOOPS
  for (i = 0; i < LOOPS; i++)
    {
    addr = randips[i];
    assert (ipcache_find_in4(cache, &addr, NULL) != 0);
    }
#endif
  printf("coll=%d loaded=%d %f%% found=%d llen=%d\n", coll, loaded,
         100.0 - (100.0 * (double)coll/(double)(LOOPS-found)), found, llen);
  
  printf("swapout...\n");
  res = ipcache_swapout(cache, "test.cache", 42);
  assert(res == 0);
  ipcache_dispose(cache);
  cache = ipcache_create_in4(CACHESIZE, 0);
  printf("swapin...\n");
  res = ipcache_swapin(cache, "test.cache", 42);
  assert(res == 0);
#if CACHESIZE >= LOOPS
  printf("recovery test...");
  for (i = 0; i < LOOPS; i++)
    {
    addr = randips[i];
    assert (ipcache_find_in4(cache, &addr, NULL) != 0);
    }
#endif
  printf("ok\n");
  return 0;
  }
#endif /* IPCACHE_TEST */

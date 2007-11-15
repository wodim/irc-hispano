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
 * $Id: ipcache.h,v 1.1.1.1 2003/12/30 17:08:35 mbuna Exp $
 */
#ifndef INCLUDED_IPCACHE_H
#define INCLUDED_IPCACHE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <peak/stdint.h>

/* IP LRU cache module for pxyservd and pxyscand, optimized using libpeak.
 */


/* Opaque type for IP cache objects
 */
typedef struct __ipcache * ipcache_t;


#if defined(__cplusplus)
extern "C" {
#endif


/* IP cache creation/destruction methods. IP cache uses a fixed number of max
 * entries. This API offers two methods when needed, to distinguish IPv4 and
 * IPv6 IP caches. You can NOT mix both in one IP cache object (ipcache_t).
 * You can associate a fixed amount of data with an IP, specify it here (can
 * be 0).
 */
extern ipcache_t ipcache_create_in4(int size, size_t datasize);
extern ipcache_t ipcache_create_in6(int size, size_t datasize);
extern void ipcache_dispose(ipcache_t cp);


/* You can explicitly clear all entries in the cache with the following method.
 */
extern int ipcache_clear(ipcache_t cp);


/* Add an IP entry in the cache. Will drop the oldest entry if the cache is
 * full. So always the least recently used IPs are in the cache.
 * "Add if absent, update timestamp if present"
 * PRECONDITION: "t" must be >= "t" of previous call! Usually, pass time(0),
 *               but it's ok to load them from a file too, if they are ordered
 *               (ipcache_swapin() uses this feature).
 */
extern void ipcache_add_in4(ipcache_t cp, const struct in_addr *addr,
                            time_t t, const void *data);
extern void ipcache_add_in6(ipcache_t cp, const struct in6_addr *addr,
                            time_t t, const void *data);


/* Mark an IP entry in the cache as invalid (that's the only way to "remove"
 * an IP from the cache).
 */
extern int ipcache_invalidate_in4(ipcache_t cp, const struct in_addr *addr);
extern int ipcache_invalidate_in6(ipcache_t cp, const struct in6_addr *addr);


/* Find function, returns 0 if not found or the last seen timestamp otherwise.
 * If found and a non-NULL output data (od) pointer is provided, the data
 * associated with the IP, if any, will be returned. Data is mutable and its
 * content can be modified (eg. to update a cache hit counter, etc.).
 */
extern time_t ipcache_find_in4(ipcache_t cp, const struct in_addr *addr,
                               void **od);
extern time_t ipcache_find_in6(ipcache_t cp, const struct in6_addr *addr,
                               void **od);


extern size_t ipcache_get_count(ipcache_t cp);

/* Special cleanup method. Delete all cache entries with ts <= expire_ts.
 */
extern size_t ipcache_cleanup(ipcache_t cp, time_t expire_ts);

/* Cache media storage methods.
 */
extern int ipcache_swapin(ipcache_t cp, const char *file, uint32_t sig);
extern int ipcache_swapout(ipcache_t cp, const char *file, uint32_t sig);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_IPCACHE_H */

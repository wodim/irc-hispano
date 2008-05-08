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
 * $Id: glineq.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_GLINEQ_H
#define INCLUDED_GLINEQ_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* This module is used to queue the glines. It should be efficient.
 * g_args is just a convenience structure used to pack the arguments.
 */

struct g_args
  {
  struct in_addr addr;
  int hitcnt;
  const char *reason;
  uint16_t port;
  };

/* max_queue_elems is the max number of glines in the queue.
 * 0 means unlimited (but resources limitation could happen), but is not
 * recommended.
 */
extern void glineq_init(int max_queue_elems);

extern void glineq_finalize(void);

/* Those are usually O(1). push() may do some memory allocations if the queue
 * grows too much. If push fails, it will be a memory allocation failure, thus
 * an exception should be thrown (using libbcl). You can dispose the args
 * pointer after the call.
 */
extern int glineq_empty(void);

/* Returns -1 if max_queue_elems is reached (failure), 0 otherwise (success).
 */
extern int glineq_push(const struct g_args *args);

/* _pop()'s precondition: glineq_first() != NULL (queue not empty)
 */
extern void glineq_pop(void);

/* Returns a pointer still owned by the glineq module, thus you shouldn't
 * modify it. If no gline left in the queue, returns NULL.
 */
extern const struct g_args *glineq_first(void);


#endif /* INCLUDED_GLINEQ_H */

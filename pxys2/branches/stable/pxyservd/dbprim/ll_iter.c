/*
** Copyright (C) 2002 by Kevin L. Mitchell <klmitch@mit.edu>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
** MA 02111-1307, USA
**
** @(#)$Id: ll_iter.c,v 1.6 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ll_iter().
 *
 * This file contains the implementation of the ll_iter() function,
 * used to iterate over every element in a linked list.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ll_iter.c,v 1.6 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ll_iter(link_head_t *list, link_elem_t *start,
	link_iter_t iter_func, void *extra, unsigned long flags)
{
  unsigned long retval;
  link_elem_t *elem, *next = 0;

  initialize_dbpr_error_table(); /* initialize error table */

  /* verify arguments */
  if (!ll_verify(list) || (start && !le_verify(start)) || !iter_func)
    return DB_ERR_BADARGS;

  /* If the start element is specified, check that it's in this list... */
  if (start && list != start->le_head)
    return DB_ERR_WRONGTABLE;

  /* select the starting element */
  if (!start)
    start = (flags & DB_FLAG_REVERSE) ? list->lh_last : list->lh_first;

  /* Walk through list and return first non-zero return value */
  for (elem = start; elem; elem = next) {
    next = (flags & DB_FLAG_REVERSE) ? elem->le_prev : elem->le_next;
    if ((retval = (*iter_func)(list, elem, extra)))
      return retval;
  }

  return 0;
}

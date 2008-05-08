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
** @(#)$Id: ll_find.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ll_find().
 *
 * This file contains the implementation of the ll_find() function,
 * used to locate a specific element within a linked list.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ll_find.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ll_find(link_head_t *list, link_elem_t **elem_p, link_comp_t comp_func,
	link_elem_t *start, db_key_t *key)
{
  link_elem_t *elem;

  initialize_dbpr_error_table(); /* initialize error table */

  /* Verify arguments */
  if (!ll_verify(list) || !elem_p || !comp_func || !key ||
      (start && !le_verify(start)))
    return DB_ERR_BADARGS;

  /* Verify that the start element is in this list */
  if (start && list != start->le_head)
    return DB_ERR_WRONGTABLE;

  /* search the list... */
  for (elem = start ? start : list->lh_first; elem; elem = elem->le_next)
    if (!(*comp_func)(key, elem->le_object)) { /* Compare... */
      *elem_p = elem; /* comparison function must return "0" on match */
      return 0;
    }

  return DB_ERR_NOENTRY; /* Couldn't find the element */
}

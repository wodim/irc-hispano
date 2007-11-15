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
** @(#)$Id: ll_move.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ll_move().
 *
 * This file contains the implementation of the ll_move() function,
 * used to move a linked list element to another location within the
 * linked list.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ll_move.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ll_move(link_head_t *list, link_elem_t *new, link_loc_t loc,
	link_elem_t *elem)
{
  initialize_dbpr_error_table(); /* initialize error table */

  /* Verify arguments--if elem is set, must be a valid element; if
   * location is before or after, elem must be set
   */
  if (!ll_verify(list) || !le_verify(new) || (elem && !le_verify(elem)) ||
      ((loc == LINK_LOC_BEFORE || loc == LINK_LOC_AFTER) && !elem))
    return DB_ERR_BADARGS;

  /* elements can't be the same element */
  if (elem && new == elem)
    return DB_ERR_BUSY;

  /* elements must be in the same list */
  if (list != new->le_head)
    return new->le_head ? DB_ERR_WRONGTABLE : DB_ERR_UNUSED;
  if (elem && list != elem->le_head)
    return elem->le_head ? DB_ERR_WRONGTABLE : DB_ERR_UNUSED;

  if (new->le_next) /* Clip element out of it previous location */
    new->le_next->le_prev = new->le_prev;
  if (new->le_prev)
    new->le_prev->le_next = new->le_next;
  else
    list->lh_first = new->le_next;

  if (list->lh_last == new) /* Make sure we know where the last element is */
    list->lh_last = new->le_prev;

  switch (loc) { /* put it in the right place in the list */
  case LINK_LOC_HEAD:
    if (!(elem = list->lh_first)) { /* insert before first element in list */
      list->lh_first = new; /* list was empty, add element to list */
      list->lh_last = new;
      return 0; /* and return, since the list was empty before. */
    }
    /*FALLTHROUGH*/
  case LINK_LOC_BEFORE:
    new->le_next = elem; /* prepare new element for its location */
    new->le_prev = elem->le_prev;

    elem->le_prev = new; /* insert element into list */
    if (new->le_prev)
      new->le_prev->le_next = new; /* update previous element */
    else /* update head of list */
      list->lh_first = new;
    break;

  case LINK_LOC_TAIL:
    if (!(elem = list->lh_last)) { /* insert after last element in list */
      list->lh_first = new; /* list was empty, add element to list */
      list->lh_last = new;
      return 0; /* and return, since the list was empty before. */
    }
    /*FALLTHROUGH*/
  case LINK_LOC_AFTER:
    new->le_next = elem->le_next; /* prepare new element for its location */
    new->le_prev = elem;

    elem->le_next = new; /* insert element into list */
    if (new->le_next)
      new->le_next->le_prev = new; /* update next element */
    else /* update tail of list */
      list->lh_last = new;
    break;
  }

  return 0;
}

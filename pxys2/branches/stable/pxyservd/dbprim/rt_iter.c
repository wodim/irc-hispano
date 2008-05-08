/*
** Copyright (C) 2003 by Kevin L. Mitchell <klmitch@mit.edu>
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
** @(#)$Id: rt_iter.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rt_iter().
 *
 * This file contains the implementation of the rt_iter() function,
 * used to iterate over all nodes in a red-black tree.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rt_iter.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
rt_iter(rb_tree_t *tree, rb_node_t *start,
	rb_iter_t iter_func, void *extra, unsigned long flags)
{
  unsigned long retval = 0;

  initialize_dbpr_error_table(); /* initialize error table */

  /* verify arguments */
  if (!rt_verify(tree) || (start && !rn_verify(start)) || !iter_func ||
      !(flags & RBT_ORDER_MASK))
    return DB_ERR_BADARGS;

  /* If the start node is specified, check that it's in this tree */
  if (start && tree != start->rn_tree)
    return DB_ERR_WRONGTABLE;

  /* select the starting element */
  if (!start && (retval = rt_next(tree, &start, flags)))
    return retval;

  tree->rt_flags |= RBT_FLAG_FREEZE; /* freeze the tree */

  /* Walk through the tree... */
  while (start)
    if ((retval = (*iter_func)(tree, start, extra)) ||
	(retval = rt_next(tree, &start, flags)))
      break;

  tree->rt_flags &= ~RBT_FLAG_FREEZE; /* unfreeze the tree */

  return retval;
}

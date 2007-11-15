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
** @(#)$Id: rt_move.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rt_move().
 *
 * This file contains the implementation of the rt_move() function,
 * used to move a node within a red-black tree to correspond to a new
 * key.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rt_move.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
rt_move(rb_tree_t *tree, rb_node_t *node, db_key_t *key)
{
  unsigned long retval;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!rt_verify(tree) || !rn_verify(node) || !key) /* verify arguments */
    return DB_ERR_BADARGS;

  if (!node->rn_tree) /* it's not in a tree */
    return DB_ERR_UNUSED;
  if (node->rn_tree != tree) /* it's in the wrong tree */
    return DB_ERR_WRONGTABLE;

  if (tree->rt_flags & RBT_FLAG_FREEZE) /* don't mess with frozen trees */
    return DB_ERR_FROZEN;

  if (_rb_locate(tree, 0, key)) /* don't permit duplicates */
    return DB_ERR_DUPLICATE;

  /* OK, remove node from the tree... */
  if ((retval = rt_remove(tree, node)))
    return retval;

  /* Now re-add it to the tree... */
  return rt_add(tree, node, key) ? DB_ERR_READDFAILED : 0;
}

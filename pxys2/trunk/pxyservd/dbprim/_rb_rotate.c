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
** @(#)$Id: _rb_rotate.c,v 1.2 2006/07/13 19:16:23 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of _rb_rotate().
 *
 * This file contains the implementation of the _rb_rotate() function,
 * used to perform a tree node rotation for tree balancing.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: _rb_rotate.c,v 1.2 2006/07/13 19:16:23 klmitch Exp $");

/* Swap a child with its parent, effecting a rotation */
void
_rb_rotate(rb_tree_t *tree, rb_node_t *child)
{
  rb_node_t *parent;

  if (!(parent = child->rn_parent)) /* make sure it has a parent... */
    return;

  /* Figure out what points to the parent and repoint it to the child */
  if (!parent->rn_parent) /* is the parent the root? */
    tree->rt_root = child;
  else if (rn_isleft(parent)) /* OK, is it a left node? */
    parent->rn_parent->rn_left = child;
  else /* ok, must be a right node */
    parent->rn_parent->rn_right = child;

  /* Now update the parent pointers of parent and child... */
  child->rn_parent = parent->rn_parent;
  parent->rn_parent = child;

  /* Finally, move the child nodes around a little... */
  if (parent->rn_right == child) { /* right child? */
    if (child->rn_left) /* update grandchild's parent pointer */
      child->rn_left->rn_parent = parent;
    parent->rn_right = child->rn_left; /* then move the child to the parent */
    child->rn_left = parent; /* Finally, link the parent to the child */
  } else { /* OK, left child! */
    if (child->rn_right) /* update grandchild's parent pointer */
      child->rn_right->rn_parent = parent;
    parent->rn_left = child->rn_right; /* then move the child to the parent */
    child->rn_right = parent; /* Finally, link the parent to the child */
  }
}

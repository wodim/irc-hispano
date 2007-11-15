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
** @(#)$Id: rt_next.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rt_next().
 *
 * This file contains the implementation of the rt_next() function,
 * used to locate the next node in a given iteration scheme.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rt_next.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
rt_next(rb_tree_t *tree, rb_node_t **node_io, unsigned long flags)
{
  rb_node_t *node = 0;

  initialize_dbpr_error_table(); /* initialize error table */

  /* verify arguments */
  if (!rt_verify(tree) || !node_io ||
      ((node = *node_io) && !rn_verify(node)) || !(flags & RBT_ORDER_MASK))
    return DB_ERR_BADARGS;

  if (node && node->rn_tree != tree) /* node isn't in this tree */
    return DB_ERR_WRONGTABLE;

  if (!tree->rt_root) { /* empty tree? */
    *node_io = 0; /* clear the input node... */
    return 0;
  }

  switch (flags & (DB_FLAG_REVERSE | RBT_ORDER_MASK)) {
  case RBT_ORDER_PRE:
    if (!node) /* first node of preorder is the root node */
      node = tree->rt_root;
    else if (node->rn_left) /* go to the left subtree... */
      node = node->rn_left;
    else if (node->rn_right) /* go to the right subtree... */
      node = node->rn_right;
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isleft(node) && node->rn_parent->rn_right)
      node = node->rn_parent->rn_right; /* return sibling node to right */
    else { /* OK, must locate the next node */
      for (node = node->rn_parent;
	   node->rn_parent && (rn_isright(node) || !node->rn_parent->rn_right);
	   node = node->rn_parent)
	; /* empty loop */

      if (!node->rn_parent)
	node = 0; /* hit the top of the tree, no next node */
      else
	node = node->rn_parent->rn_right; /* return the sibling node */
    }
    break;
  case (DB_FLAG_REVERSE | RBT_ORDER_POST):
    if (!node) /* first node of reverse postorder is the root node */
      node = tree->rt_root;
    else if (node->rn_right) /* go to the right subtree... */
      node = node->rn_right;
    else if (node->rn_left) /* go to the left subtree... */
      node = node->rn_left;
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isright(node) && node->rn_parent->rn_left)
      node = node->rn_parent->rn_left; /* return sibling node to left */
    else { /* OK, must locate the next node */
      for (node = node->rn_parent;
	   node->rn_parent && (rn_isleft(node) || !node->rn_parent->rn_left);
	   node = node->rn_parent)
	; /* empty loop */

      if (!node->rn_parent)
	node = 0; /* hit the top of the tree, no next node */
      else
	node = node->rn_parent->rn_left; /* return the sibling node */
    }
    break;

  case RBT_ORDER_IN:
    if (!node) /* first node of inorder is the left-most node */
      for (node = tree->rt_root; node->rn_left; node = node->rn_left)
	; /* empty loop */
    else if (node->rn_right) /* go to left-most node of right subtree */
      for (node = node->rn_right; node->rn_left; node = node->rn_left)
	; /* empty loop */
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isleft(node)) /* is it a left node? */
      node = node->rn_parent; /* go up, then */
    else { /* OK, must find the next node */
      for (node = node->rn_parent; node->rn_parent && rn_isright(node);
	   node = node->rn_parent)
	; /* empty loop */
      if (!node->rn_parent)
	node = 0; /* hit the top of the tree, no next node */
      else
	node = node->rn_parent; /* return the node's parent */
    }
    break;
  case (DB_FLAG_REVERSE | RBT_ORDER_IN):
    if (!node) /* first node of reverse inorder is the right-most node */
      for (node = tree->rt_root; node->rn_right; node = node->rn_right)
	; /* empty loop */
    else if (node->rn_left) /* go to right-most node of left subtree */
      for (node = node->rn_left; node->rn_right; node = node->rn_right)
	; /* empty loop */
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isright(node)) /* is it a right node? */
      node = node->rn_parent; /* go up, then */
    else { /* OK, must find the next node */
      for (node = node->rn_parent; node->rn_parent && rn_isleft(node);
	   node = node->rn_parent)
	; /* empty loop */
      if (!node->rn_parent)
	node = 0; /* hit the top of the tree, no next node */
      else
	node = node->rn_parent; /* return the node's parent */
    }
    break;

  case RBT_ORDER_POST:
    if (!node) /* first node of postorder is bottom-most node */
      for (node = tree->rt_root; node->rn_left || node->rn_right;
	   node = node->rn_left ? node->rn_left : node->rn_right)
	; /* empty loop */
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isleft(node) && node->rn_parent->rn_right) /* go right! */
      for (node = node->rn_parent->rn_right; node->rn_left || node->rn_right;
	   node = node->rn_left ? node->rn_left : node->rn_right)
	; /* empty loop */
    else
      node = node->rn_parent; /* go up to the node's parent */
    break;
  case (DB_FLAG_REVERSE | RBT_ORDER_PRE):
    if (!node) /* first node of reverse preorder is bottom-most node */
      for (node = tree->rt_root; node->rn_right || node->rn_left;
	   node = node->rn_right ? node->rn_right : node->rn_left)
	; /* empty loop */
    else if (!node->rn_parent) /* node has no parent? */
      node = 0;
    else if (rn_isright(node) && node->rn_parent->rn_left) /* go left! */
      for (node = node->rn_parent->rn_left; node->rn_right || node->rn_left;
	   node = node->rn_right ? node->rn_right : node->rn_left)
	; /* empty loop */
    else
      node = node->rn_parent; /* go up to the node's parent */
    break;
  }

  *node_io = node; /* return the next node... */

  return 0;
}

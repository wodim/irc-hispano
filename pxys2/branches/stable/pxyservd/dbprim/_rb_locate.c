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
** @(#)$Id: _rb_locate.c,v 1.3 2006/07/13 19:16:23 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of _rb_locate().
 *
 * This file contains the implementation of the _rb_locate() function,
 * used to locate a given node in a red-black tree.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: _rb_locate.c,v 1.3 2006/07/13 19:16:23 klmitch Exp $");

/* Locate a given node, placing the given node if necessary */
rb_node_t *
_rb_locate(rb_tree_t *tree, rb_node_t *node, db_key_t *key)
{
  rb_node_t *tmp;
  int comp;

  if (!tree->rt_root) {
    tree->rt_root = node; /* store node at root of tree... */
    if (node) {
      node->rn_color = RB_COLOR_BLACK; /* it's at the root, make it black */
      node->rn_tree = tree; /* what tree is it in? */
      node->rn_parent = 0; /* what's its parent? */
      node->rn_left = 0; /* node has no children... */
      node->rn_right = 0;
      node->rn_key = *key; /* fill in the key! */
      tree->rt_count++; /* keep a count of the number of nodes in the tree */
    }
    return node;
  } else if (!(comp = (*tree->rt_comp)(tree, key,
				       rn_key(tmp = tree->rt_root))))
    return tree->rt_root; /* Oh, the root of the tree matches the key */

  while (1) /* Search for the key in the tree... */
    if (comp < 0) { /* desired node is to the left */
      if (!tmp->rn_left) {
	tmp->rn_left = node; /* add the node here */
	break; /* We're done with the loop... */
      } else if (!(comp = (*tree->rt_comp)(tree, key,
					   rn_key(tmp = tmp->rn_left))))
	return tmp; /* found the node in the tree, return it */
    } else { /* desired node is to the right */
      if (!tmp->rn_right) {
	tmp->rn_right = node; /* add the node here */
	break; /* We're done with the loop... */
      } else if (!(comp = (*tree->rt_comp)(tree, key,
					   rn_key(tmp = tmp->rn_right))))
	return tmp; /* found the node in the tree, return it */
    }

  if (node) {
    node->rn_color = RB_COLOR_RED; /* color it red... */
    node->rn_tree = tree; /* what tree is it in? */
    node->rn_parent = tmp; /* what's its parent? */
    node->rn_left = 0; /* node has no children... */
    node->rn_right = 0;
    node->rn_key = *key; /* fill in the key! */
    tree->rt_count++; /* keep a count of the number of nodes in the tree */
  }

  return node; /* return the node */
}

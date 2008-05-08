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
** @(#)$Id: rt_add.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rt_add().
 *
 * This file contains the implementation of the rt_add() function,
 * used to add a red-black tree node to a given tree.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rt_add.c,v 1.2 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Locate the uncle of a node.
 *
 * This macro is used to locate the "uncle"--parent's sibling--of a
 * given red-black tree node.
 *
 * \param[in]		node	The #rb_node_t to look up the uncle
 *				for.
 *
 * \return	The #rb_node_t representing the uncle of \p node.
 */
#define uncle(node)	(rn_isleft((node)->rn_parent) ? \
			 (node)->rn_parent->rn_parent->rn_right : \
			 (node)->rn_parent->rn_parent->rn_left)

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Flip the color of a node.
 *
 * This macro is used to flip the color of a specific node.
 *
 * \param[in]		node	The #rb_node_t to flip.
 */
#define flip(node)	((node)->rn_color = \
			 ((node)->rn_color == RB_COLOR_BLACK) ? \
			 RB_COLOR_RED : RB_COLOR_BLACK)

unsigned long
rt_add(rb_tree_t *tree, rb_node_t *node, db_key_t *key)
{
  initialize_dbpr_error_table(); /* initialize error table */

  if (!rt_verify(tree) || !rn_verify(node) || !key) /* verify arguments */
    return DB_ERR_BADARGS;

  if (node->rn_tree) /* it's already in a tree... */
    return DB_ERR_BUSY;

  if (tree->rt_flags & RBT_FLAG_FREEZE) /* don't add to frozen trees */
    return DB_ERR_FROZEN;

  if (_rb_locate(tree, node, key) != node) /* add it to the tree... */
    return DB_ERR_DUPLICATE; /* We don't allow duplication! */

  /* OK, node has been added, now let's rebalance the tree... */
  while (!rn_isblack(node) && !rn_isblack(node->rn_parent))
    if (rn_isred(uncle(node))) {
      flip(node->rn_parent); /* Flip the colors of parent and grandparent */
      flip(node->rn_parent->rn_parent);
      flip(uncle(node)); /* and flip the color of the uncle */
      node = node->rn_parent->rn_parent; /* grandparent need balancing? */
    } else if ((rn_isleft(node->rn_parent) && rn_isright(node)) ||
	       (rn_isright(node->rn_parent) && rn_isleft(node))) {
      flip(node); /* flip the colors that need flipping... */
      flip(node->rn_parent->rn_parent);
      _rb_rotate(tree, node); /* rotate the node up two levels */
      _rb_rotate(tree, node);
    } else {
      flip(node->rn_parent); /* flip the colors that need flipping... */
      flip(node->rn_parent->rn_parent);
      _rb_rotate(tree, node->rn_parent); /* move the parent up a level */
      node = node->rn_parent; /* new parent need balancing? */
    }

  /* Make sure the root node is black */
  tree->rt_root->rn_color = RB_COLOR_BLACK;

  return 0;
}

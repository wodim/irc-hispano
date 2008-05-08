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
** @(#)$Id: rt_remove.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rt_remove().
 *
 * This file contains the implementation of the rt_remove() function,
 * used to remove a node from a red-black tree.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rt_remove.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Clear a node.
 *
 * This macro is used to clear a red-black tree node, so that it is no
 * longer in the tree.
 *
 * \param[in]		node	The #rb_node_t to clear.
 */
#define _rn_clear(node)							      \
do {									      \
  (node)->rn_color = RB_COLOR_NONE; /* no color... */			      \
  (node)->rn_tree = 0; /* no tree... */					      \
  (node)->rn_parent = 0; /* no parent... */				      \
  (node)->rn_left = 0; /* and no children */				      \
  (node)->rn_right = 0;							      \
} while (0)

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Update a node's parent.
 *
 * This macro is used to update the parent of the given \p node to
 * point to the new child \p new.
 *
 * \param[in]		tree	The #rb_tree_t that the node is in.
 * \param[in]		node	The #rb_node_t whose parent is being
 *				updated.
 * \param[in]		new	The new #rb_node_t for the parent to
 *				point to.
 */
#define _rt_update_parent(tree, node, new)				      \
do {									      \
  if (!(node)->rn_parent) /* node must be at root of tree... */		      \
    (tree)->rt_root = (new); /* so update the root */			      \
  else if (rn_isleft(node)) /* maybe it's a left child? */		      \
    (node)->rn_parent->rn_left = (new); /* OK, save new value there */	      \
  else									      \
    (node)->rn_parent->rn_right = (new); /* OK, right child, save tere */     \
} while (0)

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Determine if node is a left child of its parent.
 *
 * This macro simply tests whether a given node \p n is the left child
 * of its parent \p par.
 *
 * \param[in]		par	The parent being tested.
 * \param[in]		n	The node being tested.
 *
 * \return	Boolean \c true if \p n is the left child of \p par,
 *		\c false otherwise.
 */
#define isleft(par, n)	((par)->rn_left == (n))

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Select a child node based on a condition.
 *
 * This macro simply returns a pointer to the right child of \p n if
 * the condition \p t is \c true.  If not, it returns the left child
 * of \p n.
 *
 * \param[in]		t	The condition being tested.
 * \param[in]		n	The node whose child is being
 *				selected.
 *
 * \return	The left child of \p n if \p t is \c true, otherwise,
 *		the right child of \p n.
 */
#define sel_lr(t, n)	((t) ? (n)->rn_right : (n)->rn_left)

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Locate the sibling of a node.
 *
 * This macro retrieves the sibling of the node \p n.
 *
 * \warning	This macro evaluates the \p par argument twice.
 *
 * \param[in]		par	The parent of the node \p n.
 * \param[in]		n	The node to find the sibling of.
 *
 * \return	The sibling of the node \p n.
 */
#define sibling(par, n)	(sel_lr(isleft((par), (n)), (par)))

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Locate "closer" nephew of a node.
 *
 * This macro retrieves the "closer" nephew of the node \p n.
 *
 * \warning	This macro evaluates both of its arguments multiple
 *		times.
 *
 * \param[in]		par	The parent of the node \p n.
 * \param[in]		n	The node to find the nephew of.
 *
 * \return	The "closer" nephew of the node \p n.
 */
#define l_neph(par, n)	(sel_lr(!isleft((par), (n)), sibling((par), (n))))

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Locate "further" nephew of a node.
 *
 * This macro retrieves the "further" nephew of the node \p n.
 *
 * \warning	This macro evaluates both of its arguments multiple
 *		times.
 *
 * \param[in]		par	The parent of the node \p n.
 * \param[in]		n	The node to find the nephew of.
 *
 * \return	The "further" nephew of the node \p n.
 */
#define r_neph(par, n)	(sel_lr(isleft((par), (n)), sibling((par), (n))))

unsigned long
rt_remove(rb_tree_t *tree, rb_node_t *node)
{
  rb_color_t col;
  rb_node_t *nchild, *nparent;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!rt_verify(tree) || !rn_verify(node)) /* verify arguments */
    return DB_ERR_BADARGS;

  if (!node->rn_tree) /* it's not in a tree... */
    return DB_ERR_UNUSED;
  if (node->rn_tree != tree) /* it's in the wrong tree */
    return DB_ERR_WRONGTABLE;

  if (tree->rt_flags & RBT_FLAG_FREEZE) /* don't remove from frozen trees */
    return DB_ERR_FROZEN;

  if (tree->rt_count == 1) { /* only the one node... */
    _rn_clear(node); /* clear the node */

    tree->rt_root = 0; /* clear the root pointer... */
    tree->rt_count--; /* update the node count */

    return 0; /* and we're done...gee, that was simple */
  }

  /* If the node has both children, swap it with one of its great-*
   * grandchildren that has only one child.
   */
  if (node->rn_left && node->rn_right) {
    rb_node_t *ggchild;

    for (ggchild = node->rn_right; ggchild->rn_left;
	 ggchild = ggchild->rn_left)
      ; /* find the left-most great-* grandchild on the right subtree */

    /* remember that node's parent... */
    nparent = (ggchild->rn_parent == node) ? ggchild : ggchild->rn_parent;
    nchild = ggchild->rn_right; /* ...child... */
    col = ggchild->rn_color; /* ...and color */

    /* next, groom great-* grandchild to replace node */
    ggchild->rn_left = node->rn_left;
    ggchild->rn_right = node->rn_right == ggchild ? 0 : node->rn_right;
    ggchild->rn_parent = node->rn_parent;
    ggchild->rn_color = node->rn_color;

    /* let's update the node's parent to point at great-* grandchild */
    _rt_update_parent(tree, node, ggchild);

    ggchild->rn_left->rn_parent = ggchild; /* ...and its children... */
    if (ggchild->rn_right)
      ggchild->rn_right->rn_parent = ggchild;

    /* Then update great-* grandchild's original parent... */
    if (nparent != ggchild)
      nparent->rn_left = nchild; /* it's by definition a left child */
    else
      nparent->rn_right = nchild; /* don't lose the thing! */
  } else { /* it's already an external node, so remove it... */
    nparent = node->rn_parent; /* remember node's parent... */
    nchild = node->rn_left ? node->rn_left : node->rn_right; /* ...child... */
    col = node->rn_color; /* ...and color */

    /* update node's parent to point at node's child */
    _rt_update_parent(tree, node, nchild);
  }

  if (nchild) /* now, if there was a child... */
    nchild->rn_parent = nparent; /* update its parent pointer */

  if (col == RB_COLOR_BLACK) { /* removed a black node, gotta rebalance... */
    /* work our way up the tree, stopping on the first red node */
    while (nparent && rn_isblack(nchild)) {
      /* If the sibling is red... */
      if (rn_isred(sibling(nparent, nchild))) {
	_rb_rotate(tree, sibling(nparent, nchild)); /* move it up... */
	nparent->rn_color = RB_COLOR_RED; /* and recolor the nodes */
	nparent->rn_parent->rn_color = RB_COLOR_BLACK; /* old sibling */
      }

      /* Sibling is black, are the nephews? */
      if (rn_isblack(l_neph(nparent, nchild)) &&
	  rn_isblack(r_neph(nparent, nchild))) {
	sibling(nparent, nchild)->rn_color = RB_COLOR_RED; /* recolor sib... */
	nchild = nparent; /* then move up the tree... */
	nparent = nparent->rn_parent;
	continue; /* go through loop once more... */
      }

      /* OK, one or the other (or both!) of the nephews is red. */
      if (rn_isred(l_neph(nparent, nchild))) /* is the closer one red? */
	_rb_rotate(tree, l_neph(nparent, nchild)); /* then rotate it up */
      _rb_rotate(tree, sibling(nparent, nchild)); /* now rotate sibling up */

      /* set old sibling color to be the same as parent. */
      nparent->rn_parent->rn_color = nparent->rn_color;
      nparent->rn_color = RB_COLOR_BLACK; /* set parent black... */
      /* now set the parent's new sibling black */
      sibling(nparent->rn_parent, nparent)->rn_color = RB_COLOR_BLACK;
      break; /* this is a terminal case; we're done. */
    }

    if (rn_isred(nchild)) /* if we hit a red node, turn it black */
      nchild->rn_color = RB_COLOR_BLACK;
  }

  _rn_clear(node); /* clear the node */

  tree->rt_count--; /* update the node count */

  return 0; /* and we're done...gee, that was simple */
}

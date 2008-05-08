/*
** Copyright (C) 2006 by Kevin L. Mitchell <klmitch@mit.edu>
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
** @(#)$Id: rbtree_comp.c,v 1.3 2006/09/04 15:12:14 spale Exp $
*/
/** \internal
 * \file
 * \brief Implementation of rbtree_comp().
 *
 * This file contains the implementation of the rbtree_comp()
 * function, a generic red-black tree comparison callback utilizing
 * memcmp().
 */
#include <string.h>

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: rbtree_comp.c,v 1.3 2006/09/04 15:12:14 spale Exp $");

long
rbtree_comp(rb_tree_t *tree, db_key_t *key1, db_key_t *key2)
{
  int tmp;

  if (!key1 || !dk_len(key1) || !dk_key(key1) || /* invalid keys? */
      !key2 || !dk_len(key2) || !dk_key(key2))
    return 1; /* return "no match" */

  if ((tmp = memcmp(dk_key(key1), dk_key(key2), dk_len(key1) < dk_len(key2) ?
		    dk_len(key1) : dk_len(key2))))
    return tmp;

  return dk_len(key1) - dk_len(key2);
}

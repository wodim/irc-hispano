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
** @(#)$Id: sh_find.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of sh_find().
 *
 * This file contains the implementation of the sh_find() function,
 * used to look up a specific #smat_entry_t in a sparse matrix head.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: sh_find.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix head find shim structure.
 *
 * This structure is used by sh_find() in its call to ll_find().  The
 * structure contains state that ll_find() cannot directly pass to
 * _sh_find_comp().
 */
struct _sh_find_s {
  smat_comp_t	sf_comp;	/**< Comparison function. */
  db_key_t     *sf_key;		/**< Original key. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix linked list comparision function.
 *
 * This function is a #link_comp_t comparison callback that is used as
 * a shim between the sh_find() function and the ll_find function,
 * which it uses internally.
 *
 * \param[in]		key	The database key being searched for.
 * \param[in]		data	The sparse matrix linked list entry.
 *
 * \return	Zero if the sparse matrix linked list entry matches
 *		the \p key, non-zero otherwise.
 */
static unsigned long
_sh_find_comp(db_key_t *key, void *data)
{
  struct _sh_find_s *sf;

  sf = dk_key(key);

  /* Call the user's comparison function--with some translation */
  return (*sf->sf_comp)(sf->sf_key, data);
}

unsigned long
sh_find(smat_head_t *head, smat_entry_t **elem_p, smat_comp_t comp_func,
	smat_entry_t *start, db_key_t *key)
{
  unsigned long retval;
  link_elem_t *elem;
  struct _sh_find_s sf;
  db_key_t fkey;

  initialize_dbpr_error_table(); /* initialize error table */

  /* verify arguments */
  if (!sh_verify(head) || !elem_p || !comp_func || !key ||
      (start && !se_verify(start)))
    return DB_ERR_BADARGS;

  /* Set up for the call to ll_find()... */
  sf.sf_comp = comp_func;
  sf.sf_key = key;
  dk_key(&fkey) = &sf;

  /* call into the linked list library to find the element */
  if ((retval = ll_find(&head->sh_head, &elem, _sh_find_comp,
			start ? &start->se_link[head->sh_elem] : 0, &fkey)))
    return retval;

  *elem_p = le_object(elem); /* set the entry pointer correctly */

  return 0;
}

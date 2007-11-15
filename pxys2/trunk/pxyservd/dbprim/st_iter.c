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
** @(#)$Id: st_iter.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of st_iter().
 *
 * This file contains the implementation of the st_iter() function,
 * used to iterate over all entries in a sparse matrix table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: st_iter.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix iteration function shim structure.
 *
 * This structure is used by st_iter() in its call to ht_iter().  The
 * structure contains state that ht_iter() cannot directly pass to
 * _st_iter_iter().
 */
struct _st_iter_s {
  smat_table_t *si_table;	/**< Pointer to the smat table. */
  smat_iter_t	si_iter;	/**< Iter function. */
  void	       *si_extra;	/**< Extra data. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix hash iteration callback.
 *
 * This function is a #hash_iter_t iteration callback that is used as
 * a shim between the st_iter() function and the ht_iter() function,
 * which it uses internally.
 *
 * \param[in]		table	The hash table being iterated over.
 * \param[in]		ent	The specific hash table entry being
 *				processed.
 * \param[in]		extra	Extra caller-specific data to pass to
 *				the iteration function.  In this case,
 *				a pointer to a struct _st_iter_s is
 *				passed.
 *
 * \return	Zero to continue iteration, non-zero otherwise.
 */
static unsigned long
_st_iter_iter(hash_table_t *table, hash_entry_t *ent, void *extra)
{
  struct _st_iter_s *si;

  si = extra;

  /* call the user iteration function--with appropriate translation */
  return (*si->si_iter)(si->si_table, he_value(ent), si->si_extra);
}

unsigned long
st_iter(smat_table_t *table, smat_iter_t iter_func, void *extra)
{
  struct _st_iter_s si;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!st_verify(table) || !iter_func) /* verify arguments */
    return DB_ERR_BADARGS;

  /* initialize extra data... */
  si.si_table = table;
  si.si_iter = iter_func;
  si.si_extra = extra;

  /* call into linked list library to iterate over the list */
  return ht_iter(&table->st_table, _st_iter_iter, &si);
}

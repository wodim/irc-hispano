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
** @(#)$Id: st_flush.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of st_flush().
 *
 * This file contains the implementation of the st_flush() function,
 * used to release all entries in a sparse matrix table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: st_flush.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix flush function shim structure.
 *
 * This structure is used by st_flush() in its call to ht_flush().
 * The structure contains state that ht_flush() cannot directly pass
 * to _st_flush_iter().
 */
struct _st_flush_s {
  smat_table_t *sf_table;	/**< Pointer to the smat table. */
  smat_iter_t	sf_flush;	/**< Flush function. */
  void	       *sf_extra;	/**< Extra data. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix hash flush callback.
 *
 * This function is a #hash_iter_t iteration callback that is used as
 * a shim between the st_flush() function and the ht_flush() function,
 * which it uses internally.
 *
 * \param[in]		table	The hash table being iterated over.
 * \param[in]		ent	The specific hash table entry being
 *				processed.
 * \param[in]		extra	Extra caller-specific data to pass to
 *				the iteration function.  In this case,
 *				a pointer to a struct _st_flush_s is
 *				passed.
 *
 * \return	Zero to continue flushing, non-zero otherwise.
 */
static unsigned long
_st_flush_iter(hash_table_t *table, hash_entry_t *ent, void *extra)
{
  unsigned long retval = 0;
  struct _st_flush_s *sf;

  sf = extra;

  /* Remove the object from all lists first */
  _st_remove(sf->sf_table, he_value(ent), ST_REM_FIRST | ST_REM_SECOND);

  /* call the user flush function if so desired */
  if (sf->sf_flush)
    retval = (*sf->sf_flush)(sf->sf_table, he_value(ent), sf->sf_extra);

  _smat_free(he_value(ent)); /* destroy the entry */

  return retval;
}

unsigned long
st_flush(smat_table_t *table, smat_iter_t flush_func, void *extra)
{
  struct _st_flush_s sf;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!st_verify(table)) /* verify arguments */
    return DB_ERR_BADARGS;

  /* initialize extra data... */
  sf.sf_table = table;
  sf.sf_flush = flush_func;
  sf.sf_extra = extra;

  /* call into linked list library to flush the list */
  return ht_flush(&table->st_table, _st_flush_iter, &sf);
}

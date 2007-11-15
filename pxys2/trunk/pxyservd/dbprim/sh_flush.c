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
** @(#)$Id: sh_flush.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of sh_flush().
 *
 * This file contains the implementation of the sh_flush() function,
 * used to release all entries in a sparse matrix linked list.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: sh_flush.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix flush function shim structure.
 *
 * This structure is used by sh_flush() in its call to ll_flush().
 * The structure contains state that ll_flush() cannot directly pass
 * to _sh_flush_iter().
 */
struct _sh_flush_s {
  smat_table_t *sf_table;	/**< Pointer to the smat table. */
  smat_loc_t	sf_elem;	/**< Which list we're traversing. */
  smat_iter_t	sf_flush;	/**< Flush function. */
  void	       *sf_extra;	/**< Extra data. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix linked list flush callback.
 *
 * This function is a #link_iter_t iteration callback that is used as
 * a shim between the sh_flush() function and the ll_flush() function,
 * which it uses internally.
 *
 * \param[in]		head	The linked list being iterated over.
 * \param[in]		elem	The specific linked list element being
 *				processed.
 * \param[in]		extra	Extra caller-specific data to pass to
 *				the iteration function.  In this case,
 *				a pointer to a struct _sh_flush_s is
 *				passed.
 *
 * \return	Zero to continue flushing, non-zero otherwise.
 */
static unsigned long
_sh_flush_iter(link_head_t *head, link_elem_t *elem, void *extra)
{
  unsigned long retval = 0;
  struct _sh_flush_s *sf;

  sf = extra;

  /* Remove the object from all lists first */
  if (_st_remove(sf->sf_table, le_object(elem), ST_REM_HASH |
		 (sf->sf_elem == SMAT_LOC_FIRST ? ST_REM_SECOND :
		  ST_REM_FIRST)))
    return DB_ERR_UNRECOVERABLE;

  /* call the user flush function if so desired */
  if (sf->sf_flush)
    retval = (*sf->sf_flush)(sf->sf_table, le_object(elem), sf->sf_extra);

  _smat_free(le_object(elem)); /* destroy the entry */

  return retval;
}

unsigned long
sh_flush(smat_head_t *head, smat_iter_t flush_func, void *extra)
{
  struct _sh_flush_s sf;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!sh_verify(head)) /* verify arguments */
    return DB_ERR_BADARGS;

  /* initialize extra data... */
  sf.sf_table = head->sh_table;
  sf.sf_elem = head->sh_elem;
  sf.sf_flush = flush_func;
  sf.sf_extra = extra;

  /* call into linked list library to flush the list */
  return ll_flush(&head->sh_head, _sh_flush_iter, &sf);
}

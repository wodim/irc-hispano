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
** @(#)$Id: st_init.c,v 1.7 2006/07/15 18:06:18 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of st_init().
 *
 * This file contains the implementation of the st_init() function,
 * used to dynamically initialize a sparse matrix table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: st_init.c,v 1.7 2006/07/15 18:06:18 klmitch Exp $");

unsigned long
st_init(smat_table_t *table, unsigned long flags, smat_resize_t resize,
	void *extra, unsigned long init_mod)
{
  unsigned long retval;

  initialize_dbpr_error_table(); /* set up error tables */

  if (!table) /* verify arguments */
    return DB_ERR_BADARGS;

  table->st_extra = extra;
  table->st_resize = resize;

  /* initialize the hash table */
  if ((retval = ht_init(&table->st_table, flags, hash_fnv1a, hash_comp,
			_smat_resize, table, init_mod)))
    return retval;

  table->st_magic = SMAT_TABLE_MAGIC; /* initialize the rest of the table */

  return 0;
}

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
** @(#)$Id: ht_iter.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_iter().
 *
 * This file contains the implementation of the ht_iter() function,
 * used to iterate over every entry in a hash table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_iter.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_iter(hash_table_t *table, hash_iter_t iter_func, void *extra)
{
  unsigned long retval;
  int i;
  link_elem_t *elem;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!ht_verify(table) || !iter_func) /* verify arguments */
    return DB_ERR_BADARGS;

  if (table->ht_flags & HASH_FLAG_FREEZE) /* don't mess with frozen tables */
    return DB_ERR_FROZEN;

  table->ht_flags |= HASH_FLAG_FREEZE; /* freeze the table */

  /* walk through all the elements and call the iteration function */
  for (i = 0; i < table->ht_modulus; i++)
    for (elem = ll_first(&table->ht_table[i]); elem; elem = le_next(elem))
      if ((retval = (*iter_func)(table, le_object(elem), extra))) {
	table->ht_flags &= ~HASH_FLAG_FREEZE; /* unfreeze the table */
	return retval;
      }

  table->ht_flags &= ~HASH_FLAG_FREEZE; /* unfreeze the table */

  return 0;
}

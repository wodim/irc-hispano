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
** @(#)$Id: ht_find.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_find().
 *
 * This file contains the implementation of the ht_find() function,
 * used to locate a specific entry in a hash table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_find.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_find(hash_table_t *table, hash_entry_t **entry_p, db_key_t *key)
{
  unsigned long hash;
  link_elem_t *elem;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!ht_verify(table) || !key) /* verify arguments */
    return DB_ERR_BADARGS;

  if (!table->ht_count) /* no entries in table... */
    return DB_ERR_NOENTRY;

  hash = (*table->ht_func)(table, key) % table->ht_modulus; /* get hash */

  /* walk through each element in that section */
  for (elem = ll_first(&table->ht_table[hash]); elem; elem = le_next(elem))
    /* compare keys... */
    if (!(*table->ht_comp)(table, key,
			   he_key((hash_entry_t *)le_object(elem)))) {
      /* found one, return it */
      if (entry_p)
	*entry_p = le_object(elem);
      return 0;
    }

  return DB_ERR_NOENTRY; /* couldn't find a matching entry */
}

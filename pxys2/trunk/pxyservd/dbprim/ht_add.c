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
** @(#)$Id: ht_add.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_add().
 *
 * This file contains the implementation of the ht_add() function,
 * used to add an entry to a hash table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_add.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_add(hash_table_t *table, hash_entry_t *entry, db_key_t *key)
{
  unsigned long retval;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!ht_verify(table) || !he_verify(entry) || !key) /* verify arguments */
    return DB_ERR_BADARGS;

  if (entry->he_table) /* it's already in a table... */
    return DB_ERR_BUSY;

  if (table->ht_flags & HASH_FLAG_FREEZE) /* don't add to frozen tables */
    return DB_ERR_FROZEN;

  if (!table->ht_table && !(table->ht_flags & HASH_FLAG_AUTOGROW))
    return DB_ERR_NOTABLE;

  /* It looks like we could optimize here, but don't be deceived--the table
   * may grow between here and when we fill in the entry's hash value, and
   * that would change the hash value.
   */
  if (!ht_find(table, 0, key)) /* don't permit duplicates */
    return DB_ERR_DUPLICATE;

  /* grow the table if necessary and allowed */
  if ((table->ht_count + 1) > table->ht_rollover &&
      (table->ht_flags & HASH_FLAG_AUTOGROW) &&
      (retval = ht_resize(table, _hash_fuzz(table->ht_count + 1))))
    return retval;

  /* Force the link element to point to the entry */
  le_object(&entry->he_elem) = entry;

  /* copy key value into the hash entry */
  entry->he_key = *key; /* thank goodness for structure copy! */

  /* get the hash value for the entry */
  entry->he_hash =
    (*table->ht_func)(table, &entry->he_key) % table->ht_modulus;

  /* Now add the entry to the table... */
  if ((retval = ll_add(&table->ht_table[entry->he_hash], &entry->he_elem,
		       LINK_LOC_HEAD, 0)))
    return retval;

  table->ht_count++; /* OK, now increment element count */

  entry->he_table = table; /* point entry at table */

  return 0;
}

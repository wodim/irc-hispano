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
** @(#)$Id: ht_remove.c,v 1.5 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_remove().
 *
 * This file contains the implementation of the ht_remove() function,
 * used to remove an entry from a hash table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_remove.c,v 1.5 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_remove(hash_table_t *table, hash_entry_t *entry)
{
  unsigned long retval = 0;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!ht_verify(table) || !he_verify(entry)) /* verify arguments */
    return DB_ERR_BADARGS;

  if (!entry->he_table) /* it's not in a table */
    return DB_ERR_UNUSED;
  if (entry->he_table != table) /* it's in the wrong table */
    return DB_ERR_WRONGTABLE;

  if (table->ht_flags & HASH_FLAG_FREEZE) /* don't remove from frozen tables */
    return DB_ERR_FROZEN;

  /* try to shrink the table if necessary... */
  if ((table->ht_count - 1) < table->ht_rollunder &&
      (table->ht_flags & HASH_FLAG_AUTOSHRINK) &&
      (retval = ht_resize(table, table->ht_count - 1)))
    return retval; /* if we try to shrink and it won't, error out */

  /* remove the entry from the table */
  if ((retval = ll_remove(&table->ht_table[entry->he_hash], &entry->he_elem)))
    return retval;

  entry->he_table = 0; /* reset the table */

  table->ht_count--; /* decrement element count */

  return retval;
}

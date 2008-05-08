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
** @(#)$Id: ht_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_init().
 *
 * This file contains the implementation of the ht_init() function,
 * used to dynamically initialize a hash table.
 */
#include <errno.h>
#include <stdlib.h>

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_init(hash_table_t *table, unsigned long flags, hash_func_t func,
	hash_comp_t comp, hash_resize_t resize, void *extra,
	unsigned long init_mod)
{
  int i;
  unsigned long retval;

  initialize_dbpr_error_table(); /* set up error tables */

  if (!table || !func || !comp) /* verify arguments */
    return DB_ERR_BADARGS;

  /* initialize the table */
  table->ht_flags = flags & (HASH_FLAG_AUTOGROW | HASH_FLAG_AUTOSHRINK);
  table->ht_modulus = _hash_prime(init_mod);
  table->ht_count = 0;
  table->ht_rollover = _hash_rollover(table->ht_modulus);
  table->ht_rollunder = _hash_rollunder(table->ht_modulus);
  table->ht_table = 0;
  table->ht_func = func;
  table->ht_comp = comp;
  table->ht_resize = resize;
  table->ht_extra = extra;

  if (table->ht_modulus) { /* have an initial size? */
    if (!(table->ht_table =
	  (link_head_t *)malloc(table->ht_modulus * sizeof(link_head_t))))
      return errno; /* failed to allocate memory? */

    for (i = 0; i < table->ht_modulus; i++) /* initialize the listhead array */
      if ((retval = ll_init(&table->ht_table[i], table))) {
	free(table->ht_table);
	return retval;
      }
  }

  table->ht_magic = HASH_TABLE_MAGIC; /* set the magic number */

  return 0;
}

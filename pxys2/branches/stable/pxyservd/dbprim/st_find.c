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
** @(#)$Id: st_find.c,v 1.4 2006/07/13 20:27:36 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of st_find().
 *
 * This file contains the implementation of the st_find() function,
 * used to locate a specific entry in a sparse matrix table.
 */
#include <string.h>

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: st_find.c,v 1.4 2006/07/13 20:27:36 klmitch Exp $");

unsigned long
st_find(smat_table_t *table, smat_entry_t **entry_p, smat_head_t *head1,
	smat_head_t *head2)
{
  hash_entry_t *ent;
  unsigned long retval;
  void *object[2];
  db_key_t key;

  initialize_dbpr_error_table(); /* initialize error table */

  /* Verify arguments */
  if (!st_verify(table) || !sh_verify(head1) || !sh_verify(head2) ||
      head1->sh_elem != SMAT_LOC_FIRST || head2->sh_elem != SMAT_LOC_SECOND)
    return DB_ERR_BADARGS;

  /* If there are no entries in one of the lists, then return "no entry" */
  if (!head1->sh_table || !head2->sh_table || head1->sh_head.lh_count == 0 ||
      head2->sh_head.lh_count == 0)
    return DB_ERR_NOENTRY;

  /* verify that everything's in the right tables */
  if (head1->sh_table != table || head2->sh_table != table)
    return DB_ERR_WRONGTABLE;

  /* Build the search key */
  memset(object, 0, sizeof(object));
  object[SMAT_LOC_FIRST] = sh_object(head1);
  object[SMAT_LOC_SECOND] = sh_object(head2);
  dk_key(&key) = object;
  dk_len(&key) = sizeof(object);

  /* look up the entry */
  if ((retval = ht_find(&table->st_table, &ent, &key)))
    return retval;

  /* If the user wants the object, return it to him */
  if (entry_p)
    *entry_p = he_value(ent);

  return 0; /* search successful */
}

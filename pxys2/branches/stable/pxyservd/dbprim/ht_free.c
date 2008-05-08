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
** @(#)$Id: ht_free.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ht_free().
 *
 * This file contains the implementation of the ht_free() function,
 * used to release memory still in use by an empty hash table.
 */
#include <stdlib.h>

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ht_free.c,v 1.4 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ht_free(hash_table_t *table)
{
  initialize_dbpr_error_table(); /* initialize error table */

  if (!ht_verify(table)) /* verify argument */
    return DB_ERR_BADARGS;

  if (table->ht_flags & HASH_FLAG_FREEZE) /* don't free from frozen tables */
    return DB_ERR_FROZEN;

  if (table->ht_count) /* make sure the table's empty */
    return DB_ERR_NOTEMPTY;

  if (!table->ht_modulus && !table->ht_table) /* short-circuit */
    return 0;

  free(table->ht_table); /* free allocated memory */

  table->ht_modulus = 0; /* zero the table and modulus */
  table->ht_rollover = 0; /* must also zero the rollover/rollunder */
  table->ht_rollunder = 0;
  table->ht_table = 0;

  return 0;
}

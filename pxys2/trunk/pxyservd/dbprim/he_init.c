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
** @(#)$Id: he_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of he_init().
 *
 * This file contains the implementation of the he_init() function,
 * used to dynamically initialize a hash table entry.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: he_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
he_init(hash_entry_t *entry, void *value)
{
  unsigned long retval;

  initialize_dbpr_error_table(); /* initialize error table */

  if (!entry) /* verify arguments */
    return DB_ERR_BADARGS;

  /* initialize the link entry */
  if ((retval = le_init(&entry->he_elem, entry)))
    return retval;

  entry->he_table = 0; /* initialize the rest of the hash entry */
  entry->he_hash = 0;
  entry->he_key.dk_key = 0;
  entry->he_key.dk_len = 0;
  entry->he_value = value;

  entry->he_magic = HASH_ENTRY_MAGIC; /* set the magic number */

  return 0;
}

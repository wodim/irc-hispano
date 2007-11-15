/*
** Copyright (C) 2006 by Kevin L. Mitchell <klmitch@mit.edu>
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
** @(#)$Id: hash_fnv1a.c,v 1.3 2006/09/04 15:12:14 spale Exp $
*/
/** \internal
 * \file
 * \brief Implementation of hash_fnv1a().
 *
 * This file contains the implementation of the hash_fnv1a() function,
 * a generic hash function callback implementing the FNV-1a hash
 * algorithm.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: hash_fnv1a.c,v 1.3 2006/09/04 15:12:14 spale Exp $");

unsigned long
hash_fnv1a(hash_table_t *table, db_key_t *key)
{
  int i;
  unsigned long hash = HASH_FNV_OFFSET;
  unsigned char *c;

  if (!key || !dk_len(key) || !dk_key(key)) /* invalid key?  return 0 */
    return 0;

  c = (unsigned char *)dk_key(key);
  for (i = 0; i < dk_len(key); i++) { /* FNV-1a algorithm... */
    hash ^= *c; /* hash in the data octet */
    hash *= HASH_FNV_PRIME; /* multiply by the prime... */
  }

  return hash;
}

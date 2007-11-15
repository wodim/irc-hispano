/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#define RCSID "$Id: hash_fun.c,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "hash_fun.h"
#include "irc_chattr.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

unsigned long
strhash(hash_table_t *ht, db_key_t *key)
  {
  const unsigned char *p;
  unsigned long hash = 0;
  
  assert(dk_key(key) != NULL);
  
  for (p = dk_key(key); *p; p++)
    hash = hash * 33 + ToLower(*p);
  
  return hash;
  }

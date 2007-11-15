/* Copyright (C) 2003 Stephane Thiell
 * 
 * IRC - Internet Relay Chat, include/hash.h (Undernet ircu)
 * Copyright (C) 1998 by Andrea "Nemesi" Cocito
 *
 * This program is free software; you can redistribute it and/or modify
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
 * $Id: hash_fun.h,v 1.2 2003/12/30 21:03:55 mbuna Exp $
 */

#ifndef INCLUDED_HASH_FUN_H
#define INCLUDED_HASH_FUN_H

#include <dbprim/dbprim.h>

extern unsigned long strhash(hash_table_t *ht, db_key_t *key);


#endif /* INCLUDED_HASH_FUN_H */

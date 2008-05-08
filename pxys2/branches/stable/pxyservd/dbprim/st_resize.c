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
** @(#)$Id: st_resize.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of st_resize().
 *
 * This file contains the implementation of the st_resize() function,
 * used to force a resize of the sparse matrix hash table.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: st_resize.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
st_resize(smat_table_t *table, unsigned long new_size)
{
  initialize_dbpr_error_table(); /* initialize error table */

  if (!st_verify(table)) /* verify that it's really a table */
    return DB_ERR_BADARGS;

  return ht_resize(&table->st_table, new_size); /* call out to hash */
}

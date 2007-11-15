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
** @(#)$Id: sh_init.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of sh_init().
 *
 * This file contains the implementation of the sh_init() function,
 * used to dynamically initialize a sparse matrix head structure.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: sh_init.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
sh_init(smat_head_t *head, smat_loc_t elem, void *object)
{
  unsigned long retval;

  initialize_dbpr_error_table(); /* initialize error table */

  /* verify arguments... */
  if (!head || (elem != SMAT_LOC_FIRST && elem != SMAT_LOC_SECOND))
    return DB_ERR_BADARGS;

  /* initialize list head */
  if ((retval = ll_init(&head->sh_head, object)))
    return retval;

  head->sh_elem = elem; /* initialize list head */
  head->sh_table = 0;

  head->sh_magic = SMAT_HEAD_MAGIC; /* set magic number */

  return 0;
}

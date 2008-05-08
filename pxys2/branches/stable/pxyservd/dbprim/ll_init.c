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
** @(#)$Id: ll_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of ll_init().
 *
 * This file contains the implementation of the ll_init() function,
 * used to dynamically initialize a linked list head.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: ll_init.c,v 1.3 2006/07/13 05:36:16 klmitch Exp $");

unsigned long
ll_init(link_head_t *list, void *extra)
{
  initialize_dbpr_error_table(); /* set up error tables */

  if (!list) /* must have a list head */
    return DB_ERR_BADARGS;

  list->lh_count = 0; /* initialize the list head */
  list->lh_first = 0;
  list->lh_last = 0;
  list->lh_extra = extra;

  list->lh_magic = LINK_HEAD_MAGIC; /* set the magic number */

  return 0;
}

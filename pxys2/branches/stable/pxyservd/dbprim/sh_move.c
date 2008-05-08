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
** @(#)$Id: sh_move.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Implementation of sh_move().
 *
 * This file contains the implementation of the sh_move() function,
 * used to move sparse matrix entries from one place to another in a
 * sparse matrix head list.
 */
#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: sh_move.c,v 1.3 2006/07/13 19:16:24 klmitch Exp $");

unsigned long
sh_move(smat_head_t *head, smat_entry_t *elem, link_loc_t loc,
	smat_entry_t *elem2)
{
  initialize_dbpr_error_table(); /* initialize error table */

  /* Verify arguments--if elem is set, must be a valid element; if
   * location is before or after, elem must be set
   */
  if (!sh_verify(head) || !se_verify(elem) || (elem2 && !se_verify(elem2)))
    return DB_ERR_BADARGS;

  /* OK, call out to the linked list operation--it'll figure
   * everything else out
   */
  return ll_move(&head->sh_head, &elem->se_link[head->sh_elem], loc,
		 elem2 ? &elem2->se_link[head->sh_elem] : 0);
}

/*
** Copyright (C) 2002, 2006 by Kevin L. Mitchell <klmitch@mit.edu>
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
** @(#)$Id: dbprim_int.h,v 1.7 2006/07/15 18:06:18 klmitch Exp $
*/
/** \internal
 * \file
 * \brief Database Primitives Library internal header.
 *
 * This file contains the \#define's and function declarations used
 * internally by the Database Primitives Library.
 */
#ifndef __include_dbprim_int_h__
#define __include_dbprim_int_h__

#include "dbprim_err.h"
#include "dbprim_version.h"

#ifndef DBPRIM_DOXYGEN
# ifdef __GNUC__
#  if (__GNUC__ < 2) || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#   define __attribute__(A)
#  endif
# else
#  define __attribute__(A)
# endif
#endif /* DBPRIM_DOXYGEN */

/** \internal
 * \brief Embed RCS revision information.
 *
 * Embeds the \p tag (a string including the RCS Id tag) into the
 * binary.  This can be useful when tracking down version skew issues.
 */
#define RCSTAG(tag) static char rcsid[] __attribute__((unused)) = tag

/** \internal
 * \ingroup dbprim_hash
 * \brief Select a prime number.
 *
 * This function is used by the hash table system to find the first
 * prime number larger than \p start.  This prime number will be used
 * as the hash table modulus.
 *
 * \param[in]		start	The number from which to start looking
 *				for the next largest prime.
 *
 * \return	The first prime number larger than \p start.
 */
unsigned long _hash_prime(unsigned long start);

/** \internal
 * \ingroup dbprim_hash
 * \brief Select hash table roll over size.
 *
 * This macro is used to compute the "roll over" size--the size at
 * which the hash table will be grown (assuming that the table has
 * #HASH_FLAG_AUTOGROW set).
 *
 * \param[in]		mod	The table modulus.
 *
 * \return	The "roll over" size.
 */
#define _hash_rollover(mod)	(((mod) * 4) / 3)

/** \internal
 * \ingroup dbprim_hash
 * \brief Select hash table roll under size.
 *
 * This macro is used to compute the "roll under" size--the size at
 * which the hash table will be shrunk (assuming that the table has
 * #HASH_FLAG_AUTOSHRINK set).
 *
 * \param[in]		mod	The table modulus.
 *
 * \return	The "roll under" size.
 */
#define _hash_rollunder(mod)	(((mod) * 3) / 4)

/** \internal
 * \ingroup dbprim_hash
 * \brief Fuzz the initial hash table size.
 *
 * This macro is used to apply a fudge factor to a user-supplied size
 * for the hash table, causing a slightly larger bucket to be
 * allocated.
 *
 * \param[in]		mod	The requested table modulus.
 *
 * \return	The "fuzzed" size.
 */
#define _hash_fuzz(mod)		(((mod) * 4) / 3)

/** \internal
 * \ingroup dbprim_hash
 * \brief FNV offset basis parameter.
 *
 * This is the 32-bit offset basis for the FNV hash algorithm.  See
 * http://www.isthe.com/chongo/tech/comp/fnv/ for more information
 * about the FNV hash algorithm.
 */
#define HASH_FNV_OFFSET		2166136261UL

/** \internal
 * \ingroup dbprim_hash
 * \brief FNV prime parameter.
 *
 * This is the 32-bit multiplication prime for the FNV hash algorithm.
 * See http://www.isthe.com/chongo/tech/comp/fnv/ for more information
 * about the FNV hash algorithm.
 */
#define HASH_FNV_PRIME		16777619UL

/** \internal
 * \ingroup dbprim_smat
 * \brief Remove an entry from a sparse matrix (internal).
 *
 * This function implements the actual logic that removes a sparse
 * matrix entry from a sparse matrix table.  Sparse matrix entries
 * must be removed from three different locations before they can be
 * passed to free(), and there are several places in the library where
 * they are removed from one location and must subsequently be removed
 * from the others.  This routine allows the caller to specify exactly
 * what must be removed through the use of the \p remflag argument.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		entry	A pointer to a #smat_entry_t to be
 *				removed from the table.
 * \param[in]		remflag	A bitwise mask of #ST_REM_FIRST,
 *				#ST_REM_SECOND, #ST_REM_HASH, and
 *				#ST_REM_FREE indicating which portions
 *				of the removal logic must be executed.
 *
 * \return	The function returns zero if the requested operations
 *		were completed successfully.  A non-zero return value
 *		indicates a catastrophic failure condition, but is
 *		unlikely to occur.
 */
unsigned long _st_remove(smat_table_t *table, smat_entry_t *entry,
			 unsigned int remflag);

/** \internal
 * \ingroup dbprim_smat
 * \brief Flag requesting removal from first list.
 *
 * This flag may be passed to _st_remove() to request that a sparse
 * matrix entry should be removed from the #SMAT_LOC_FIRST linked
 * list.
 */
#define ST_REM_FIRST	0x0001	/* remove from first list */

/** \internal
 * \ingroup dbprim_smat
 * \brief Flag requesting removal from second list.
 *
 * This flag may be passed to _st_remove() to request that a sparse
 * matrix entry should be removed from the #SMAT_LOC_SECOND linked
 * list.
 */
#define ST_REM_SECOND	0x0002	/* remove from second list */

/** \internal
 * \ingroup dbprim_smat
 * \brief Flag requesting removal from hash table.
 *
 * This flag may be passed to _st_remove() to request that a sparse
 * matrix entry should be removed from the hash table.
 */
#define ST_REM_HASH	0x0004	/* remove from hash table */

/** \internal
 * \ingroup dbprim_smat
 * \brief Flag requesting memory release.
 *
 * This flag may be passed to _st_remove() to request that a sparse
 * matrix entry be passed to _smat_free().
 */
#define ST_REM_FREE	0x0008	/* free the entry */

/** \internal
 * \ingroup dbprim_smat
 * \brief Allocate a sparse matrix entry.
 *
 * This function is used to allocate a sparse matrix entry.  In
 * cooperation with _smat_free(), it maintains a freelist in order to
 * reduce memory fragmentation.  If a sparse matrix entry cannot be
 * allocated from the freelist, a new one will be allocated with
 * malloc().
 *
 * \return	A pointer to an initialized sparse matrix entry.
 */
smat_entry_t *_smat_alloc(void);

/** \internal
 * \ingroup dbprim_smat
 * \brief Release a sparse matrix entry.
 *
 * This function is used to release a sparse matrix entry.  In
 * cooperatio with _smat_alloc(), it maintains a freelist in order to
 * reduce memory fragmentation.  Sparse matrix entries passed to this
 * function will generally be added to the free list.
 *
 * \param[in]		entry	The entry to release.
 */
void _smat_free(smat_entry_t *entry);

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix resize function.
 *
 * This function is a hash table-compatible resize callback for use by
 * sparse matrices.
 *
 * \param[in]		table	The hash table being resized.
 * \param[in]		new_mod	The new hash table bucket size.
 *
 * \return	Zero if the resize operation should be performed,
 *		non-zero otherwise.
 */
unsigned long _smat_resize(hash_table_t *table, unsigned long new_mod);

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Locate or insert a red-black tree node.
 *
 * This function is used to locate a red-black tree node with a key
 * matching \p key.  If the node does not exist, but \p node is
 * non-<CODE>NULL</CODE>, \p node will be inserted into the tree at an
 * appropriate place, although please note that rebalancing will be
 * necessary.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		node	A pointer to a #rb_node_t to be added
 *				to the tree.
 * \param[in]		key	A pointer to a #db_key_t containing
 *				the key to be looked up or inserted.
 *
 * \return	A pointer to the #rb_node_t found or inserted, or \c
 *		NULL if one could not be found.
 */
rb_node_t *_rb_locate(rb_tree_t *tree, rb_node_t *node, db_key_t *key);

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Rotate tree nodes.
 *
 * This function is used to swap \p child with its parent, effecting a
 * tree-balancing rotation.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		child	A pointer to a #rb_node_t to be
 *				swapped with its parent.
 */
void _rb_rotate(rb_tree_t *tree, rb_node_t *child);

#endif /* __include_dbprim_int_h__ */

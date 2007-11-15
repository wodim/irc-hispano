/* -*- c -*-
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
** @(#)$Id: dbprim.h,v 1.13 2006/07/15 18:06:18 klmitch Exp $
*/
#ifndef __include_dbprim_h__
#define __include_dbprim_h__
/** \mainpage Database Primitives Library
 *
 * This library contains a set of database primitives.  The primitives
 * defined by this library include a powerful linked list abstraction,
 * a hash table with optional automatic resizing, a powerful and
 * efficient sparse matrix implementation, and a red-black tree with
 * iterative traversals.  All of the necessary declarations for using
 * this library are found in the header file dbprim.h.  For more
 * information about the components of this library, see the module
 * list.
 */

/** \file dbprim/dbprim.h
 * \brief Core Database Primitives header file.
 *
 * This header file contains the necessary structures, \#define's, and
 * function declarations to make use of the Database Primitives
 * library.
 */

/** \defgroup dbprim Database Primitives
 *
 * This module describes interfaces common to all database
 * modules--mainly the macros concerned with manipulating database
 * keys and the definition of the key structure.
 *
 * The key may be any arbitrary pointer, including a pointer to a
 * string.  Everything that handles a key either copies the contents
 * of the #db_key_t structure or passes it to a user-defined
 * function.  If required, as in the case of a string, a length may
 * also be represented in the key structure.
 */

/** \defgroup dbprim_link Linked lists
 * \brief Operations for linked lists.
 *
 * Linked lists are a very basic data structure used in building
 * databases.  This library provides a simple yet powerful
 * implementation of generic linked lists, based on two
 * caller-allocated structures.  The #link_head_t structure describes
 * the head of a linked list and contains information regarding the
 * number of elements in the linked list as well as pointers
 * referencing the first and last elements in the list.  The
 * #link_elem_t structure describes a specific element in the linked
 * list and contains pointers referencing the next and previous
 * elements in the list, as well as a pointer to the object, a pointer
 * to the head of the linked list, and a set of user-specified flags.
 *
 * Elements may be added at any arbitrary location in the linked list
 * with ll_add(); moved to any other arbitrary location in the linked
 * list with ll_move(); or removed from the list with ll_remove().  In
 * addition, the user may search the list using a user-defined
 * comparison function with ll_find(); iterate over every element in
 * the list with ll_iter(); or remove all items from the list with
 * ll_flush(), optionally executing a user-specified clean-up
 * function.
 */

/** \defgroup dbprim_hash Hash tables
 * \brief Operations for hash tables.
 *
 * Hash tables are a basic data structure used in building databases.
 * Hash tables provide a means of storing data such that an arbitrary
 * entry may be looked up efficiently.  This library implements a hash
 * table that may optionally grow and shrink to provide maximum
 * efficiency.  The implementation is with two kinds of
 * caller-allocated structures--a #hash_table_t structure that
 * describes the table and a #hash_entry_t structure for each entry in
 * the table.  The library allocates a bucket array which must be
 * released with the ht_free() function when the hash table has been
 * emptied.  Additionally, the hash table may be manually resized with
 * the ht_resize() function.
 *
 * Entries may be added to and removed from the table using the
 * ht_add() and ht_remove() functions.  Additionally, the key on a
 * given entry may be changed using the ht_move() function.  Of
 * course, any given entry may be looked up using the ht_find()
 * function, and ht_iter() will execute a user-defined function for
 * each entry in the hash table (in an unspecified order).  The
 * ht_flush() function will remove all entries from the hash table,
 * optionally executing a user-specified clean-up function.
 */

/** \defgroup dbprim_smat Sparse matrices
 * \brief Operations for sparse matrices.
 *
 * Sparse matrices are advanced data structures used to represent
 * associations.  For instance, a manager may have several employees,
 * but several of those employees may report to more than one
 * manager.  (Yes, this is a contrived example, so sue me.)  The
 * simplest way to represent such assocations is with a matrix, or a
 * two-dimensional array.  However, such an implementation cannot
 * easily be extended dynamically--imagine if a manager retires and
 * two more are hired, for instance.  It would also use an enormous
 * amount of memory, as most employees would only report to one or two
 * managers.
 *
 * A sparse matrix solves this problem by only allocating memory for
 * the cells in the full matrix which are actually used.  That is, no
 * memory is allocated to represent Alice reporting to Bob unless
 * Alice actually does report to Bob.  This is a simple concept, but
 * fairly difficult to implement efficiently--how do you tell if Alice
 * reports to Bob?  The solution utilized by this library is to
 * combine the strengths of linked lists and hash tables.  Each cell
 * is in two linked lists, rooted at the rows and columns of the
 * matrix, but a hash table is used when attempting to look up a given
 * cell.  If the cell is allocated, then there will be an entry in the
 * hash table, and finding the given cell is as fast as a hash table
 * look-up.
 *
 * Because sparse matrices are so complicated, there are three
 * structures and a variety of operations used.  Two of the
 * structures, #smat_table_t and #smat_head_t, are caller-allocated.
 * However, the third structure, #smat_entry_t, must be allocated by
 * the library.  To avoid too much overhead from malloc(), a free list
 * is used.  The free list may be managed with the smat_cleanup() and
 * smat_freemem() calls.
 *
 * The #smat_table_t contains the hash table.  Only one of these need
 * be allocated per type of association--for instance, in the above
 * example, only one #smat_table_t needs to be allocated to represent
 * the manager-employee relationship.
 *
 * The #smat_head_t contains the linked list.  There are actually two
 * kinds of these structures--one is #SMAT_LOC_FIRST, which could be
 * regarded as a ``row,'' and the other is #SMAT_LOC_SECOND, which
 * could be regarded as a ``column.''  Which one is used for which
 * type of data is irrelevant, as long as consistency is maintained.
 * For the above example, a #smat_head_t for a manager may be
 * #SMAT_LOC_FIRST, and one for an employee must then be
 * #SMAT_LOC_SECOND.  (These values are set when initializing the
 * #smat_head_t structure.)
 *
 * An association may be created with the st_add() function, which
 * allows an arbitrary ordering in the associated linked lists by the
 * same mechanism as for the linked list component of the library.  An
 * association may be removed with st_remove(), or looked up with
 * st_find().  If iteration over all associations is desired, use the
 * st_iter() function.  Removing all associations from a table may be
 * performed with st_flush(), which optionally calls a user-defined
 * clean-up function.  The associated hash table may be resized with
 * st_resize(), and the bucket table may be released with st_free().
 *
 * An association may also be reordered within the linked lists using
 * the sh_move() function.  If a particular entry is desired, use the
 * sh_find() function with a user-defined comparison function to
 * locate it.  Iteration may be performed with the sh_iter() function,
 * and all entries in a given linked list may be removed with the
 * sh_flush() function, which again may optionally call a user-defined
 * clean-up function.
 */

/** \defgroup dbprim_rbtree Red-black trees
 * \brief Operations for red-black trees.
 *
 * Red-black trees are a form of binary search tree.  One essential
 * feature of binary search trees is that they need to be balanced in
 * order to be efficient.  Many algorithms exist for keeping trees
 * balanced, but among the easiest to implement is the red-black
 * tree.  In a red-black tree, every node is given a color--either red
 * or black--and there are various rules for what color nodes can be
 * present where in the tree.  This library implements these rules,
 * along with functions for traversing the tree in any desired tree
 * order.
 *
 * A red-black tree is represented by a caller-allocated #rb_tree_t
 * structure.  This structure describes various characteristics of the
 * tree, such as the number of nodes in the tree, and includes a
 * pointer to the root node of the tree.  Nodes may be added to the
 * tree using rt_add() or removed from the tree using rt_remove().
 * Additionally, the key on a given node may be changed using the
 * rt_move() function.  Nodes may be looked up with rt_find(), and
 * rt_iter() will execute a user-defined function for each node in the
 * tree in the specified order.  To remove all entries in the tree,
 * simply call the rt_flush() function.  If you must manually iterate
 * through the tree, you may call the rt_next() and rt_prev()
 * functions to determine the next or previous nodes to visit.
 *
 * There are three ways to traverse a binary tree.  The first, known
 * as "preorder," visits the root node, then traverses the left
 * subtree in preorder, then traverses the right subtree in preorder.
 * The second, known an "inorder," traverses the left subtree in
 * inorder, then the root node, then the right subtree in inorder.
 * (This particular ordering retrieves the nodes in lexical order;
 * thus its name.)  The third ordering is known as "postorder"; this
 * ordering traverses the left subtree, the right subtree, then visits
 * the root node.  To iterate over the tree in one of these orderings,
 * simply call rt_iter() (or rt_next() or rt_prev()) with the
 * #RBT_ORDER_PRE, #RBT_ORDER_IN, or #RBT_ORDER_POST flags.  You may
 * OR these flags with #DB_FLAG_REVERSE to reverse the traversal
 * ordering, if you wish.
 */

/** \internal
 * \def DBPRIM_BEGIN_C_DECLS
 * \brief Begin declaration in C namespace.
 *
 * This macro is defined to <CODE>extern "C" {</CODE> when compiling
 * with a C++ compiler.  See #DBPRIM_END_C_DECLS.
 */
/** \internal
 * \def DBPRIM_END_C_DECLS
 * \brief End declaration in C namespace.
 *
 * This macro is defined to <CODE>}</CODE> when compiling with a C++
 * compiler.  See #DBPRIM_BEGIN_C_DECLS.
 */
#undef DBPRIM_BEGIN_C_DECLS
#undef DBPRIM_END_C_DECLS
#ifdef __cplusplus
# define DBPRIM_BEGIN_C_DECLS extern "C" {
# define DBPRIM_END_C_DECLS }
#else
# define DBPRIM_BEGIN_C_DECLS /* empty */
# define DBPRIM_END_C_DECLS   /* empty */
#endif

DBPRIM_BEGIN_C_DECLS

#ifndef __DBPRIM_LIBRARY__
#include <dbprim/dbprim_err.h>
#include <dbprim/dbprim_version.h>
#endif /* __DBPRIM_LIBRARY__ */

/** \ingroup dbprim
 * \brief Database key.
 *
 * This structure is a generic key containing a void * pointer and a
 * length parameter.  It should be accessed with dk_key() and
 * dk_len().
 */
typedef struct _db_key_s db_key_t;

/** \ingroup dbprim_link
 * \brief Linked list head.
 *
 * This structure is the head of all linked lists maintained by this
 * library.
 */
typedef struct _link_head_s  link_head_t;

/** \ingroup dbprim_link
 * \brief Linked list element.
 *
 * This structure represents a single element of a linked list.
 */
typedef struct _link_elem_s  link_elem_t;

/** \ingroup dbprim_hash
 * \brief Hash table.
 *
 * This structure is the basis of all hash tables maintained by this
 * library.
 */
typedef struct _hash_table_s hash_table_t;

/** \ingroup dbprim_hash
 * \brief Hash table entry.
 *
 * This structure represents a single entry of a hash table.
 */
typedef struct _hash_entry_s hash_entry_t;

/** \ingroup dbprim_smat
 * \brief Sparse matrix table.
 *
 * This structure is the basis of all sparse matrices maintained by
 * this library.
 */
typedef struct _smat_table_s smat_table_t;

/** \ingroup dbprim_smat
 * \brief Sparse matrix list head.
 *
 * This structure is the head of a linked list of sparse matrix
 * entries.
 */
typedef struct _smat_head_s  smat_head_t;

/** \ingroup dbprim_smat
 * \brief Sparse matrix entry.
 *
 * This structure is allocated by the library and represents a single
 * element in a sparse matrix.
 */
typedef struct _smat_entry_s smat_entry_t;

/** \ingroup dbprim_rbtree
 * \brief Red-black tree.
 *
 * This structure is the basis of all red-black trees maintained by
 * this library.
 */
typedef struct _rb_tree_s rb_tree_t;

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node.
 *
 * This structure represents a single node in a red-black tree.
 */
typedef struct _rb_node_s rb_node_t;

/** \ingroup dbprim_link
 * \brief Linked list iteration callback.
 *
 * This function pointer references a callback used by ll_iter() and
 * ll_flush().  It should return 0 for success.  A non-zero return
 * value will terminate the operation and will become the return value
 * of the ll_iter() or ll_flush() call.
 *
 * \param[in]		head	A pointer to a #link_head_t.
 * \param[in]		elem	A pointer to the #link_elem_t being
 *				considered.
 * \param[in]		extra	A \c void pointer passed to ll_iter()
 *				or ll_flush().
 *
 * \return	Zero for success, or non-zero to terminate the
 *		iteration.
 */
typedef unsigned long (*link_iter_t)(link_head_t *list, link_elem_t *elem,
				     void *extra);

/** \ingroup dbprim_link
 * \brief Linked list comparison callback.
 *
 * This function pointer references a callback used by ll_find().  It
 * should return 0 if the entry passed as the second argument matches
 * the key passed as the first argument.
 *
 * \param[in]		key	The database key being searched for.
 * \param[in]		obj	The object to compare with the key.
 *
 * \return	Zero if \p key matches \p obj, non-zero otherwise.
 */
typedef unsigned long (*link_comp_t)(db_key_t *key, void *obj);

/** \ingroup dbprim_hash
 * \brief Hash table iteration callback.
 *
 * This function pointer references a callback used by ht_iter() and
 * ht_flush().  It should return 0 for success.  A non-zero return
 * value will terminate the operation and will become the return value
 * of the ht_iter() or ht_flush() call.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		ent	A pointer to the #hash_entry_t being
 *				considered.
 * \param[in]		extra	A \c void pointer passed to ht_iter()
 *				or ht_flush().
 *
 * \return	Zero for success, or non-zero to terminate the
 *		iteration.
 */
typedef unsigned long (*hash_iter_t)(hash_table_t *table, hash_entry_t *ent,
				     void *extra);

/** \ingroup dbprim_hash
 * \brief Hash function callback.
 *
 * This function is associated with a hash table, and is responsible
 * for generating a hash value.  The full 32-bit range of an
 * <CODE>unsigned long</CODE> should be used--do *not* reduce the hash
 * value by the modulus of the hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		key	The database key to hash.
 *
 * \return	A 32-bit hash value for \p key.
 */
typedef unsigned long (*hash_func_t)(hash_table_t *table, db_key_t *key);

/** \ingroup dbprim_hash
 * \brief Hash table comparison callback.
 *
 * This function pointer references a callback used to compare entries
 * in a hash table.  It should return 0 for identical entries and
 * non-zero otherwise.  No assumptions should be made about the order
 * in which the two keys are passed to this function.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		key1	The first key being compared.
 * \param[in]		key2	The second key being compared.
 *
 * \return	Zero if the keys match, non-zero otherwise.
 */
typedef unsigned long (*hash_comp_t)(hash_table_t *table, db_key_t *key1,
				     db_key_t *key2);

/** \ingroup dbprim_hash
 * \brief Hash table resize callback.
 *
 * This function pointer references a callback that will be called
 * with both the old and new hash table sizes whenever a hash table is
 * resized.  It should return non-zero only when the resize should be
 * inhibited.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		new_mod	The new table modulus.
 *
 * \return	Zero to permit the table resize, non-zero otherwise.
 */
typedef unsigned long (*hash_resize_t)(hash_table_t *table,
				       unsigned long new_mod);

/** \ingroup dbprim_smat
 * \brief Sparse matrix table resize callback.
 *
 * This function pointer references a callback that will be called
 * with both the old and new sparse matrix table sizes whenever a
 * sparse matrix's hash table table is resized.  It should return
 * non-zero only when the resize should be inhibited.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		new_mod	The new table modulus.
 *
 * \return	Zero to permit the table resize, non-zero otherwise.
 */
typedef unsigned long (*smat_resize_t)(smat_table_t *table,
				       unsigned long new_mod);

/** \ingroup dbprim_smat
 * \brief Sparse matrix iteration callback.
 *
 * This function pointer references a callback used by st_iter(),
 * st_flush(), sh_iter(), and sh_flush().  It should return 0 for
 * success.  A non-zero return value will terminate the operation and
 * will become the return value of the call.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		ent	A pointer to the #smat_entry_t being
 *				considered.
 * \param[in]		extra	A \c void pointer passed to st_iter(),
 *				st_flush(), sh_iter(), or sh_flush().
 *
 * \return	Zero for success, or non-zero to terminate the
 *		iteration.
 */
typedef unsigned long (*smat_iter_t)(smat_table_t *table, smat_entry_t *ent,
				     void *extra);

/** \ingroup dbprim_smat
 * \brief Sparse matrix comparison callback.
 *
 * This function pointer references a callback used by sh_find().  It
 * should return 0 if the sparse matrix entry represented by the
 * second argument matches the key passed as the first argument.
 *
 * \param[in]		key	The database key being searched for.
 * \param[in]		ent	A pointer to the #smat_entry_t being
 *				considered.
 *
 * \return	Zero if \p key matches \p ent, non-zero otherwise.
 */
typedef unsigned long (*smat_comp_t)(db_key_t *key, smat_entry_t *ent);

/** \ingroup dbprim_rbtree
 * \brief Red-black tree iteration callback.
 *
 * This function pointer references a callback used by rb_iter() and
 * rb_flush().  It should return 0 for success.  A non-zero return
 * value will terminate the operation and will become the return
 * value of the call.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		node	A pointer to the #rb_node_t being
 *				considered.
 * \param[in]		extra	A \c void pointer passed to rt_iter()
 *				or rt_flush().
 *
 * \return	Zero for success, or non-zero to terminate the
 *		iteration.
 */
typedef unsigned long (*rb_iter_t)(rb_tree_t *tree, rb_node_t *node,
				   void *extra);

/** \ingroup dbprim_rbtree
 * \brief Red-black tree comparison callback.
 *
 * This function pointer references a callback used to compare nodes
 * in a red-black tree.  It should return 0 for identical entries,
 * less than 0 if the first key is less than the second, or greater
 * than 0 if the first key is greater than the second.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		key1	The first key being compared.
 * \param[in]		key2	The second key being compared.
 *
 * \return	Zero if the keys match, less than zero if the first
 *		key orders before the second key, or greater than zero
 *		if the first key orders after the second key.
 */
typedef long (*rb_comp_t)(rb_tree_t *tree, db_key_t *key1, db_key_t *key2);

/** \ingroup dbprim_link
 * \brief Linked list location.
 *
 * This enumeration is used to specify where an element in a linked
 * list should be placed.  It should be referenced by the typedef
 * #link_loc_t.
 */
enum _link_loc_e {
  LINK_LOC_HEAD,	/**< Element should be inserted at head of list. */
  LINK_LOC_TAIL,	/**< Element should be inserted at tail of list. */
  LINK_LOC_BEFORE,	/**< Element should be inserted before specified
			     element. */
  LINK_LOC_AFTER	/**< Element should be inserted after specified
			     element. */
};

/** \ingroup dbprim_link
 * \brief Linked list location.
 *
 * See the documentation for the enumeration #_link_loc_e.
 */
typedef enum _link_loc_e link_loc_t;

/** \ingroup dbprim_smat
 * \brief Sparse matrix location.
 *
 * This enumeration is used to specify whether an element is a row or
 * column element.  It should be referenced by the typedef
 * #smat_loc_t.
 */
enum _smat_loc_e {
  SMAT_LOC_FIRST,	/**< First entry (``row''). */
  SMAT_LOC_SECOND	/**< Second entry (``column''). */
};

/** \ingroup dbprim_smat
 * \brief Sparse matrix location.
 *
 * See the documentation for the enumeration #_smat_loc_e.
 */
typedef enum _smat_loc_e smat_loc_t;

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node color.
 *
 * This enumeration is used to specify the color of a node of a
 * red-black tree.
 */
enum _rb_color_e {
  RB_COLOR_NONE,	/**< Node is uncolored as of yet. */
  RB_COLOR_RED,		/**< Node is red. */
  RB_COLOR_BLACK	/**< Node is black. */
};

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node color.
 *
 * See the documentation for the enumeration #_rb_color_e.
 */
typedef enum _rb_color_e rb_color_t;

/** \internal
 * \ingroup dbprim
 * \brief Database key structure.
 *
 * This is the implementation of the #db_key_t type.
 */
struct _db_key_s {
  void *dk_key;		/**< Pointer to the key. */
  int   dk_len;		/**< Length of the key, if that has any meaning. */
};

/** \ingroup dbprim
 * \brief Database key static initializer.
 *
 * This macro allows a #db_key_t to be initialized statically.
 *
 * \param[in]		key	A pointer to the key.
 * \param[in]		size	Size of the key.
 */
#define DB_KEY_INIT(key, size) { (key), (size) }

/** \ingroup dbprim
 * \brief Database key accessor macro.
 *
 * This macro allows access to the key field of a #db_key_t.  It may
 * be used as an lvalue in order to assign a key to a #db_key_t.
 *
 * \param[in]		key	A pointer to a #db_key_t.
 *
 * \return	A pointer to a key (<CODE>void *</CODE>).
 */
#define dk_key(key)	((key)->dk_key)

/** \ingroup dbprim
 * \brief Database key length accessor macro.
 *
 * This macro allows access to the key length field of a #db_key_t.
 * It may be used as an lvalue in order to assign a length to a
 * #db_key_t.
 *
 * \param[in]		key	A pointer to a #db_key_t.
 *
 * \return	An \c int describing the length of the key.
 */
#define dk_len(key)	((key)->dk_len)

/** \ingroup dbprim
 * \brief Reverse flag.
 *
 * This flag can be passed to ordered iterations to reverse the
 * order of the iterations.
 */
#define DB_FLAG_REVERSE		0x80000000

/** \internal
 * \ingroup dbprim_link
 * \brief Linked list head structure.
 *
 * This is the implementation of the #link_head_t type.
 */
struct _link_head_s {
  unsigned long	lh_magic;	/**< Magic number. */
  unsigned long	lh_count;	/**< Number of entries in the linked list. */
  link_elem_t  *lh_first;	/**< First element in the list. */
  link_elem_t  *lh_last;	/**< Last element in the list. */
  void	       *lh_extra;	/**< Extra data associated with list. */
};

/** \internal
 * \ingroup dbprim_link
 * \brief Linked list head magic number.
 *
 * This is the magic number used for the linked list head structure.
 */
#define LINK_HEAD_MAGIC	0x4c6155d7

/** \ingroup dbprim_link
 * \brief Linked list head static initializer.
 *
 * This macro statically initializes a #link_head_t.
 *
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the list head.
 */
#define LINK_HEAD_INIT(extra)	{ LINK_HEAD_MAGIC, 0, 0, 0, (extra) }

/** \ingroup dbprim_link
 * \brief Linked list head verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * linked list head.
 *
 * \warning	This macro evaluates the \p list argument twice.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 *
 * \return	Boolean \c true if \p list is a valid linked list head
 *		or \c false otherwise.
 */
#define ll_verify(list)		((list) && \
				 (list)->lh_magic == LINK_HEAD_MAGIC)

/** \ingroup dbprim_link
 * \brief Linked list count.
 *
 * This macro retrieves the number of elements in a linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing a count of
 *		the number of elements in the linked list.
 */
#define ll_count(list)	((list)->lh_count)

/** \ingroup dbprim_link
 * \brief First element in linked list.
 *
 * This macro retrieves the first element in a linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 *
 * \return	A pointer to a #link_elem_t.
 */
#define ll_first(list)	((list)->lh_first)

/** \ingroup dbprim_link
 * \brief Last element in a linked list.
 *
 * This macro retrieves the last element in a linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 *
 * \return	A pointer to a #link_elem_t.
 */
#define ll_last(list)	((list)->lh_last)

/** \ingroup dbprim_link
 * \brief Extra pointer data in a linked list.
 *
 * This macro retrieves the extra pointer data associated with a
 * particular linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 *
 * \return	A pointer to \c void.
 */
#define ll_extra(list)	((list)->lh_extra)

/** \ingroup dbprim_link
 * \brief Dynamically initialize a linked list head.
 *
 * This function dynamically initializes a linked list head.
 *
 * \param[in]		list	A pointer to a #link_head_t to be
 *				initialized.
 * \param[in]		extra	A pointer to \c void containing extra
 *				pointer data associated with the
 *				linked list.
 *
 * \retval DB_ERR_BADARGS	A \c NULL pointer was passed for \p
 *				list.
 */
unsigned long ll_init(link_head_t *list, void *extra);

/** \ingroup dbprim_link
 * \brief Add an element to a linked list.
 *
 * This function adds a given element to a specified linked list in
 * the specified location.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[in]		new	A pointer to the #link_elem_t to be
 *				added to the linked list.
 * \param[in]		loc	A #link_loc_t indicating where the
 *				entry should be added.
 * \param[in]		elem	A pointer to a #link_elem_t describing
 *				another element in the list if \p loc
 *				is #LINK_LOC_BEFORE or
 *				#LINK_LOC_AFTER.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_BUSY		The element is already in a list.
 * \retval DB_ERR_WRONGTABLE	\p elem is in a different list.
 * \retval DB_ERR_UNUSED	\p elem is not in any list.
 */
unsigned long ll_add(link_head_t *list, link_elem_t *new, link_loc_t loc,
		     link_elem_t *elem);

/** \ingroup dbprim_link
 * \brief Move an element within a linked list.
 *
 * This function moves a specified element within the linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[in]		elem	A pointer to the #link_elem_t
 *				describing the element to be moved.
 * \param[in]		loc	A #link_loc_t indicating where the
 *				entry should be moved to.
 * \param[in]		elem2	A pointer to a #link_elem_t describing
 *				another element in the list if \p loc
 *				is #LINK_LOC_BEFORE or
 *				#LINK_LOC_AFTER.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_BUSY		\p new and \p elem are the same
 *				element.
 * \retval DB_ERR_WRONGTABLE	\p new or \p elem are in a different
 *				list.
 * \retval DB_ERR_UNUSED	\p new or \p elem are not in any
 *				list.
 */
unsigned long ll_move(link_head_t *list, link_elem_t *elem, link_loc_t loc,
		      link_elem_t *elem2);

/** \ingroup dbprim_link
 * \brief Remove an element from a linked list.
 *
 * This function removes a specified element from a linked list.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[in]		elem	A pointer to the #link_elem_t
 *				describing the element to be removed.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_UNUSED	\p elem is not in a linked list.
 * \retval DB_ERR_WRONGTABLE	\p elem is not in this linked list.
 */
unsigned long ll_remove(link_head_t *list, link_elem_t *elem);

/** \ingroup dbprim_link
 * \brief Find an element in a linked list.
 *
 * This function iterates through a linked list looking for an element
 * that matches the given \p key.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[out]		elem_p	A pointer to a pointer to a
 *				#link_elem_t.  \c NULL is an invalid
 *				value.
 * \param[in]		comp_func
 *				A pointer to a comparison function
 *				used to compare the key to a
 *				particular element.  See the
 *				documentation for #link_comp_t for
 *				more information.
 * \param[in]		start	A pointer to a #link_elem_t describing
 *				where in the linked list to start.  If
 *				\c NULL is passed, the beginning of
 *				the list will be assumed.
 * \param[in]		key	A key to search for.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this linked list.
 * \retval DB_ERR_NOENTRY	No matching entry was found.
 */
unsigned long ll_find(link_head_t *list, link_elem_t **elem_p,
		      link_comp_t comp_func, link_elem_t *start,
		      db_key_t *key);

/** \ingroup dbprim_link
 * \brief Iterate over each entry in a linked list.
 *
 * This function iterates over a linked list, executing the given \p
 * iter_func for each entry.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[in]		start	A pointer to a #link_elem_t describing
 *				where in the linked list to start.  If
 *				\c NULL is passed, the beginning of
 *				the list will be assumed.
 * \param[in]		iter_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an element in a linked list.  \c NULL
 *				is an invalid value.  See the
 *				documentation for #link_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 * \param[in]		flags	If #DB_FLAG_REVERSE is given,
 *				iteration will be done from the end of
 *				the list backwards towards the head.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this linked list.
 */
unsigned long ll_iter(link_head_t *list, link_elem_t *start,
		      link_iter_t iter_func, void *extra, unsigned long flags);

/** \ingroup dbprim_link
 * \brief Flush a linked list.
 *
 * This function flushes a linked list--that is, it removes each
 * element from the list.  If a \p flush_func is specified, it will be
 * called on the entry after it has been removed from the list, and
 * may safely call <CODE>free()</CODE>.
 *
 * \param[in]		list	A pointer to a #link_head_t.
 * \param[in]		flush_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an element after removing it from the
 *				list.  May be \c NULL.  See the
 *				documentation for #link_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p flush_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 */
unsigned long ll_flush(link_head_t *list, link_iter_t flush_func, void *extra);

/** \internal
 * \ingroup dbprim_link
 * \brief Linked list element structure.
 *
 * This is the implementation of the #link_elem_t type.
 */
struct _link_elem_s {
  unsigned long	le_magic;	/**< Magic number. */
  link_elem_t  *le_next;	/**< Next element in list. */
  link_elem_t  *le_prev;	/**< Previous element in list. */
  void	       *le_object;	/**< The object pointed to by this link. */
  link_head_t  *le_head;	/**< The head of the list. */
  unsigned long	le_flags;	/**< Flags associated with this element. */
};

/** \internal
 * \ingroup dbprim_link
 * \brief Linked list element magic number.
 *
 * This is the magic number used for the linked list element
 * structure.
 */
#define LINK_ELEM_MAGIC	0x97cdf72a

/** \ingroup dbprim_link
 * \brief Linked list element static initializer.
 *
 * This macro statically initializes a #link_elem_t.
 *
 * \param[in]		obj	A pointer to \c void representing the
 *				object associated with the element.
 */
#define LINK_ELEM_INIT(obj) { LINK_ELEM_MAGIC, 0, 0, (obj), 0, 0 }

/** \ingroup dbprim_link
 * \brief Linked list element verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * linked list element.
 *
 * \warning	This macro evaluates the \p element argument twice.
 *
 * \param[in]		element	A pointer to a #link_elem_t.
 *
 * \return	Boolean \c true if \p element is a valid linked list
 *		element or \c false otherwise.
 */
#define le_verify(element)	((element) && \
				 (element)->le_magic == LINK_ELEM_MAGIC)

/** \ingroup dbprim_link
 * \brief Linked list element next pointer.
 *
 * This macro retrieves a pointer to the next element in the linked
 * list.
 *
 * \param[in]		elem	A pointer to a #link_elem_t.
 *
 * \return	A pointer to a #link_elem_t representing the next
 *		element in the linked list.
 */
#define le_next(elem)	((elem)->le_next)

/** \ingroup dbprim_link
 * \brief Linked list element previous pointer.
 *
 * This macro retrieves a pointer to the previous element in the
 * linked list.
 *
 * \param[in]		elem	A pointer to a #link_elem_t.
 *
 * \return	A pointer to a #link_elem_t representing the previous
 *		element in the linked list.
 */
#define le_prev(elem)	((elem)->le_prev)

/** \ingroup dbprim_link
 * \brief Linked list element object pointer.
 *
 * This macro retrieves a pointer to the object represented by the
 * element.  It may be used as an lvalue to change the object pointed
 * to.  Care should be taken when using this feature.
 *
 * \param[in]		elem	A pointer to a #link_elem_t.
 *
 * \return	A pointer to \c void representing the object
 *		associated with the linked list element.
 */
#define le_object(elem)	((elem)->le_object)

/** \ingroup dbprim_link
 * \brief Linked list element head pointer.
 *
 * This macro retrieves a pointer to the head of the linked list that
 * the element is in.
 *
 * \param[in]		elem	A pointer to a #link_elem_t.
 *
 * \return	A pointer to a #link_head_t representing the head of
 *		the linked list the element is in.
 */
#define le_head(elem)	((elem)->le_head)

/** \ingroup dbprim_link
 * \brief Linked list element flags.
 *
 * This macro retrieves a set of user-defined flags associated with
 * the element.  It may be used as an lvalue to set those flags.
 *
 * \param[in]		elem	A pointer to a #link_elem_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags
 *		associated with the element.
 */
#define le_flags(elem)	((elem)->le_flags)

/** \ingroup dbprim_link
 * \brief Dynamically initialize a linked list element.
 *
 * This function dynamically initializes a linked list element.
 *
 * \param[in]		elem	A pointer to a #link_elem_t to be
 *				initialized.
 * \param[in]		object	A pointer to \c void used to represent
 *				the object associated with the
 *				element.
 *
 * \retval DB_ERR_BADARGS	A \c NULL pointer was passed for \p
 *				elem or \p object.
 */
unsigned long le_init(link_elem_t *elem, void *object);

/** \ingroup dbprim_hash
 * \brief FNV-1 hash function.
 *
 * This is a hash function, compatible with #hash_func_t, based around
 * the FNV-1 hash algorithm.  See
 * http://www.isthe.com/chongo/tech/comp/fnv/ for more information
 * about FNV-1.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		key	The database key to hash.
 *
 * \return	A 32-bit hash value for \p key.
 */
unsigned long hash_fnv1(hash_table_t *table, db_key_t *key);

/** \ingroup dbprim_hash
 * \brief FNV-1a hash function.
 *
 * This is a hash function, compatible with #hash_func_t, based around
 * the FNV-1a hash algorithm.  See
 * http://www.isthe.com/chongo/tech/comp/fnv/ for more information
 * about FNV-1a.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		key	The database key to hash.
 *
 * \return	A 32-bit hash value for \p key.
 */
unsigned long hash_fnv1a(hash_table_t *table, db_key_t *key);

/** \ingroup dbprim_hash
 * \brief Hash comparison function.
 *
 * This is a hash comparison function, compatible with #hash_comp_t,
 * based around memcmp().
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		key1	The first key being compared.
 * \param[in]		key2	The second key being compared.
 *
 * \return	Zero if the keys match, non-zero otherwise.
 */
unsigned long hash_comp(hash_table_t *table, db_key_t *key1, db_key_t *key2);

/** \internal
 * \ingroup dbprim_hash
 * \brief Hash table structure.
 *
 * This is the implementation of the #hash_table_t type.
 */
struct _hash_table_s {
  unsigned long	ht_magic;	/**< Magic number. */
  unsigned long	ht_flags;	/**< Flags associated with the table. */
  unsigned long	ht_modulus;	/**< Size (modulus) of the hash
				     table--must be prime. */
  unsigned long	ht_count;	/**< Number of elements in the table. */
  unsigned long	ht_rollover;	/**< Size at which the table grows. */
  unsigned long	ht_rollunder;	/**< Size at which the table shrinks. */
  link_head_t  *ht_table;	/**< Actual table entries. */
  hash_func_t	ht_func;	/**< Function for computing the hash. */
  hash_comp_t	ht_comp;	/**< Function for comparing hash keys. */
  hash_resize_t	ht_resize;	/**< Function for resize notify/inhibit. */
  void	       *ht_extra;	/**< Extra data associated with the table. */
};

/** \internal
 * \ingroup dbprim_hash
 * \brief Hash table magic number.
 *
 * This is the magic number used for the hash table structure.
 */
#define HASH_TABLE_MAGIC 0x2da7ffd9

/** \ingroup dbprim_hash
 * \brief Flag permitting a hash table to automatically grow.
 *
 * If passed in to #HASH_TABLE_INIT() or #ht_init(), allows the hash
 * table to grow automatically.
 */
#define HASH_FLAG_AUTOGROW   0x00000001 /* let table automatically grow */

/** \ingroup dbprim_hash
 * \brief Flag permitting a hash table to automatically shrink.
 *
 * If passed in to #HASH_TABLE_INIT() or #ht_init(), allows the hash
 * table to shrink automatically.
 */
#define HASH_FLAG_AUTOSHRINK 0x00000002 /* let table automatically shrink */

/** \ingroup dbprim_hash
 * \brief Hash table flags that may be set by the user.
 *
 * This flag mask may be used to obtain flags that the hash table user
 * may set on a hash table.
 */
#define HASH_FLAG_MASK	     (HASH_FLAG_AUTOGROW | HASH_FLAG_AUTOSHRINK)

/** \internal
 * \brief Flag indicating hash table is frozen.
 *
 * This flag, if set on a hash table, indicates that the table is
 * frozen and may not be modified.
 */
#define HASH_FLAG_FREEZE     0x80000000 /* hash table frozen */

/** \ingroup dbprim_hash
 * \brief Hash table static initializer.
 *
 * This macro statically initializes a #hash_table_t.
 *
 * \param[in]		flags	A bit-wise OR of #HASH_FLAG_AUTOGROW
 *				and #HASH_FLAG_AUTOSHRINK.  If neither
 *				behavior is desired, use 0.
 * \param[in]		func	A #hash_func_t function pointer for a
 *				hash function.
 * \param[in]		comp	A #hash_comp_t function pointer for a
 *				comparison function.
 * \param[in]		resize	A #hash_resize_t function pointer for
 *				determining whether resizing is
 *				permitted and/or for notification of
 *				the resize.
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the hash table.
 */
#define HASH_TABLE_INIT(flags, func, comp, resize, extra) \
	{ HASH_TABLE_MAGIC, (flags) & HASH_FLAG_MASK, 0, 0, 0, 0, 0, \
	  (func), (comp), (resize), (extra) }

/** \ingroup dbprim_hash
 * \brief Hash table verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * hash table.
 *
 * \warning	This macro evaluates the \p table argument twice.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	Boolean \c true if \p table is a valid hash table or
 *		\c false otherwise.
 */
#define ht_verify(table)	((table) && \
				 (table)->ht_magic == HASH_TABLE_MAGIC)

/** \ingroup dbprim_hash
 * \brief Hash table flags.
 *
 * This macro retrieves the flags associated with the hash table.
 * Only #HASH_FLAG_AUTOGROW and #HASH_FLAG_AUTOSHRINK have any meaning
 * to the application; all other bits are reserved for use in the
 * library.  This macro may be used as an lvalue, but care must be
 * taken to avoid modifying the library-specific bits.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags for
 *		the hash table.
 */
#define ht_flags(table)	  ((table)->ht_flags)

/** \ingroup dbprim_hash
 * \brief Determine if a hash table is frozen.
 *
 * This macro returns a non-zero value if the table is currently
 * frozen.  The hash table may be frozen if there is an iteration in
 * progress.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A zero value if the table is not frozen or a non-zero
 *		value if the table is frozen.
 */
#define ht_frozen(table)  ((table)->ht_flags & HASH_FLAG_FREEZE)

/** \ingroup dbprim_hash
 * \brief Hash table modulus.
 *
 * This macro retrieves the number of buckets allocated for the hash
 * table.  An application may wish to save this value between
 * invocations to avoid the overhead of growing the table while
 * filling it with data.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the number of
 *		buckets allocated for the hash table.
 */
#define ht_modulus(table) ((table)->ht_modulus)

/** \ingroup dbprim_hash
 * \brief Hash table count.
 *
 * This macro retrieves the total number of items actually in the hash
 * table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing a count of
 *		the number of items in the hash table.
 */
#define ht_count(table)	  ((table)->ht_count)

/** \ingroup dbprim_hash
 * \brief Hash table hash function.
 *
 * This macro retrieves the hash function pointer.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A #hash_func_t.
 */
#define ht_func(table)	  ((table)->ht_func)

/** \ingroup dbprim_hash
 * \brief Hash table comparison function.
 *
 * This macro retrieves the comparison function pointer.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A #hash_comp_t.
 */
#define ht_comp(table)	  ((table)->ht_comp)

/** \ingroup dbprim_hash
 * \brief Hash table resize callback function.
 *
 * This macro retrieves the resize callback function pointer.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A #hash_resize_t.
 */
#define ht_rsize(table)	  ((table)->ht_resize)

/** \ingroup dbprim_hash
 * \brief Extra pointer data in a hash table.
 *
 * This macro retrieves the extra pointer data associated with a
 * particular hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A pointer to \c void.
 */
#define ht_extra(table)	  ((table)->ht_extra)

/** \ingroup dbprim_hash
 * \brief Hash table memory size.
 *
 * This macro returns the physical size of the bucket array allocated
 * by the library for this hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \return	A \c size_t.
 */
#define ht_size(table)	  ((table)->ht_modulus * sizeof(link_head_t))

/** \ingroup dbprim_hash
 * \brief Dynamically initialize a hash table.
 *
 * This function dynamically initializes a hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t to be
 *				initialized.
 * \param[in]		flags	A bit-wise OR of #HASH_FLAG_AUTOGROW
 *				and #HASH_FLAG_AUTOSHRINK.  If neither
 *				behavior is desired, use 0.
 * \param[in]		func	A #hash_func_t function pointer for a
 *				hash function.
 * \param[in]		comp	A #hash_comp_t function pointer for a
 *				comparison function.
 * \param[in]		resize	A #hash_resize_t function pointer for
 *				determining whether resizing is
 *				permitted and/or for notification of
 *				the resize.
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the hash table.
 * \param[in]		init_mod
 *				An initial modulus for the table.
 *				This will presumably be extracted by
 *				ht_modulus() in a previous invocation
 *				of the application.  A 0 value is
 *				valid.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval ENOMEM		Unable to allocate memory.
 */
unsigned long ht_init(hash_table_t *table, unsigned long flags,
		      hash_func_t func, hash_comp_t comp,
		      hash_resize_t resize, void *extra,
		      unsigned long init_mod);

/** \ingroup dbprim_hash
 * \brief Add an entry to a hash table.
 *
 * This function adds an entry to a hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		entry	A pointer to a #hash_entry_t to be
 *				added to the table.
 * \param[in]		key	A pointer to a #db_key_t containing
 *				the key for the entry.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_BUSY		The entry is already in a table.
 * \retval DB_ERR_FROZEN	The table is currently frozen.
 * \retval DB_ERR_NOTABLE	The bucket table has not been
 *				allocated and automatic growth is not
 *				enabled.
 * \retval DB_ERR_DUPLICATE	The entry is a duplicate of an
 *				existing entry.
 * \retval DB_ERR_UNRECOVERABLE	An unrecoverable error occurred while
 *				resizing the table.
 */
unsigned long ht_add(hash_table_t *table, hash_entry_t *entry, db_key_t *key);

/** \ingroup dbprim_hash
 * \brief Move an entry in the hash table.
 *
 * This function moves an existing entry in the hash table to
 * correspond to the new key.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		entry	A pointer to a #hash_entry_t to be
 *				moved.  It must already be in the hash
 *				table.
 * \param[in]		key	A pointer to a #db_key_t describing
 *				the new key for the entry.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_UNUSED	Entry is not in a hash table.
 * \retval DB_ERR_WRONGTABLE	Entry is not in this hash table.
 * \retval DB_ERR_FROZEN	Hash table is frozen.
 * \retval DB_ERR_DUPLICATE	New key is a duplicate of an existing
 *				key.
 * \retval DB_ERR_READDFAILED	Unable to re-add entry to table.
 */
unsigned long ht_move(hash_table_t *table, hash_entry_t *entry, db_key_t *key);

/** \ingroup dbprim_hash
 * \brief Remove an element from a hash table.
 *
 * This function removes the given element from the specified hash
 * table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		entry	A pointer to a #hash_entry_t to be
 *				removed from the table.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_UNUSED	Entry is not in a hash table.
 * \retval DB_ERR_WRONGTABLE	Entry is not in this hash table.
 * \retval DB_ERR_FROZEN	Hash table is frozen.
 * \retval DB_ERR_UNRECOVERABLE	An unrecoverable error occurred while
 *				resizing the table.
 */
unsigned long ht_remove(hash_table_t *table, hash_entry_t *entry);

/** \ingroup dbprim_hash
 * \brief Find an entry in a hash table.
 *
 * This function looks up an entry matching the given \p key.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[out]		entry_p	A pointer to a pointer to a
 *				#hash_entry_t.  If \c NULL is passed,
 *				the lookup will be performed and an
 *				appropriate error code returned. 
 * \param[in]		key	A pointer to a #db_key_t describing
 *				the item to find.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_NOENTRY	No matching entry was found.
 */
unsigned long ht_find(hash_table_t *table, hash_entry_t **entry_p,
		      db_key_t *key);

/** \ingroup dbprim_hash
 * \brief Iterate over each entry in a hash table.
 *
 * This function iterates over every entry in a hash table (in an
 * unspecified order), executing the given \p iter_func on each entry.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		iter_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an entry in a hash table. \c NULL is
 *				an invalid value.  See the
 *				documentation for #hash_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The hash table is frozen.
 */
unsigned long ht_iter(hash_table_t *table, hash_iter_t iter_func, void *extra);

/** \ingroup dbprim_hash
 * \brief Flush a hash table.
 *
 * This function flushes a hash table--that is, it removes each entry
 * from the table.  If a \p flush_func is specified, it will be called
 * on the entry after it has been removed from the table, and may
 * safely call <CODE>free()</CODE>.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		flush_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an entry after removing it from the
 *				table.  May be \c NULL.  See the
 *				documentation for #hash_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p flush_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The hash table is frozen.
 */
unsigned long ht_flush(hash_table_t *table, hash_iter_t flush_func,
		       void *extra);

/** \ingroup dbprim_hash
 * \brief Resize a hash table.
 *
 * This function resizes a hash table to the given \p new_size.  If \p
 * new_size is 0, then an appropriate new size based on the current
 * number of items in the hash table will be selected.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 * \param[in]		new_size
 *				A new size value for the table.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The table is currently frozen.
 * \retval DB_ERR_UNRECOVERABLE	A catastrophic error was encountered.
 *				The table is now unusable.
 * \retval ENOMEM		No memory could be allocated for the
 *				new bucket table.
 */
unsigned long ht_resize(hash_table_t *table, unsigned long new_size);

/** \ingroup dbprim_hash
 * \brief Free memory used by an empty hash table.
 *
 * This function releases the memory used by the bucket table in an
 * empty hash table.
 *
 * \param[in]		table	A pointer to a #hash_table_t.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_FROZEN	The table is frozen.
 * \retval DB_ERR_NOTEMPTY	The table is not empty.
 */
unsigned long ht_free(hash_table_t *table);

/** \internal
 * \ingroup dbprim_hash
 * \brief Hash table entry structure.
 *
 * This is the implementation of the #hash_entry_t type.
 */
struct _hash_entry_s {
  unsigned long he_magic;	/**< Magic number. */
  link_elem_t	he_elem;	/**< Link element. */
  hash_table_t *he_table;	/**< Hash table we're in. */
  unsigned long	he_hash;	/**< Hash value. */
  db_key_t	he_key;		/**< Entry's key. */
  void	       *he_value;	/**< Actual entry. */
};

/** \internal
 * \ingroup dbprim_hash
 * \brief Hash table entry magic number.
 *
 * This is the magic number used for the hash table entry structure.
 */
#define HASH_ENTRY_MAGIC 0x35afaf51

/** \ingroup dbprim_hash
 * \brief Hash table entry static initializer.
 *
 * This macro statically initializes a #hash_entry_t.
 *
 * \param[in]		value	A pointer to \c void representing the
 *				object associated with the entry.
 */
#define HASH_ENTRY_INIT(value) \
      { HASH_ENTRY_MAGIC, LINK_ELEM_INIT(0), 0, 0, DB_KEY_INIT(0, 0), (value) }

/** \ingroup dbprim_hash
 * \brief Hash table entry verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * hash table entry.
 *
 * \warning	This macro evaluates the \p entry argument twice.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	Boolean \c true if \p entry is a valid hash table
 *		entry or \c false otherwise.
 */
#define he_verify(entry)	((entry) && \
				 (entry)->he_magic == HASH_ENTRY_MAGIC)

/** \ingroup dbprim_hash
 * \brief Hash table entry linked list element.
 *
 * This macro provides access to the linked list element buried in the
 * hash table entry.  It should *not* be used on entries currently in
 * a hash table.  The purpose of this macro is to allow an object
 * containing a hash table entry to be placed upon a free list.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	A pointer to a #link_elem_t.
 */
#define he_link(entry)	(&((entry)->he_elem))

/** \ingroup dbprim_hash
 * \brief Hash table entry flags.
 *
 * This macro retrieves a set of user-defined flags associated with
 * the entry.  It may be used as an lvalue to set those flags.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags
 *		associated with the entry.
 */
#define he_flags(entry)	((entry)->he_elem.le_flags)

/** \ingroup dbprim_hash
 * \brief Hash table entry table pointer.
 *
 * This macro retrieves a pointer to the hash table the entry is in.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	A pointer to a #hash_table_t.
 */
#define he_table(entry)	((entry)->he_table)

/** \ingroup dbprim_hash
 * \brief Hash table entry hash value.
 *
 * This macro retrieves the hash value of the given hash entry.  If
 * the hash table has been resized, this value may not be the same as
 * a previous value.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the hash code
 *		for the entry.
 */
#define he_hash(entry)	((entry)->he_hash)

/** \ingroup dbprim_hash
 * \brief Hash table entry key pointer.
 *
 * This macro retrieves the key associated with the hash table entry.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	A pointer to a #db_key_t.
 */
#define he_key(entry)	(&((entry)->he_key))

/** \ingroup dbprim_hash
 * \brief Hash table entry value pointer.
 *
 * This macro retrieves the value associated with the hash table
 * entry.  It may be treated as an lvalue to change that value.  Care
 * should be taken when using this option.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t.
 *
 * \return	A pointer to \c void representing the value associated
 *		with this entry.
 */
#define he_value(entry)	((entry)->he_value)

/** \ingroup dbprim_hash
 * \brief Dynamically initialize a hash table entry.
 *
 * This function dynamically initializes a hash table entry.
 *
 * \param[in]		entry	A pointer to a #hash_entry_t to be
 *				initialized.
 * \param[in]		value	A pointer to \c void which will be the
 *				value of the hash table entry.
 *
 * \retval DB_ERR_BADARGS	A \c NULL pointer was passed for \p
 *				entry.
 */
unsigned long he_init(hash_entry_t *entry, void *value);

/** \ingroup dbprim_smat
 * \brief Clean up the smat free list.
 *
 * This function frees all smat_entry_t objects on the internal free
 * list.  It is always successful and returns 0.
 *
 * \return	This function always returns 0.
 */
unsigned long smat_cleanup(void);

/** \ingroup dbprim_smat
 * \brief Report how much memory is used by the free list.
 *
 * This function returns the amount of memory being used by the
 * internal free list of smat_entry_t objects.
 *
 * \return	A number indicating the size, in bytes, of the memory
 *		allocated for smat_entry_t objects on the free list.
 */
unsigned long smat_freemem(void);

/** \internal
 * \ingroup dbprim_smat
 * \brief Retrieve pointer to sparse matrix entry.
 *
 * This simple macro simply retrieves a pointer to the sparse matrix
 * entry from a linked list element.  It is a helper macro for the
 * other macros that access sparse matrix entries.
 *
 * \param[in]		ent	A pointer to a #link_elem_t.
 *
 * \return	A pointer to the relevant #smat_entry_t.
 */
#define _smat_ent(ent)	((smat_entry_t *)le_object(ent))

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix table structure.
 *
 * This is the implementation of the #smat_table_t type.
 */
struct _smat_table_s {
  unsigned long	st_magic;	/**< Magic number. */
  smat_resize_t	st_resize;	/**< Function pointer for resize callback. */
  void	       *st_extra;	/**< Extra data pointer. */
  hash_table_t	st_table;	/**< Hash table. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix table magic number.
 *
 * This is the magic number used for the sparse matrix table
 * structure.
 */
#define SMAT_TABLE_MAGIC 0x2f92a7b1

/** \ingroup dbprim_smat
 * \brief Sparse matrix table verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * sparse matrix table.
 *
 * \warning	This macro evaluates the \p table argument twice.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	Boolean \c true if \p table is a valid sparse matrix
 *		table or \c false otherwise.
 */
#define st_verify(table)	((table) && \
				 (table)->st_magic == SMAT_TABLE_MAGIC)

/** \ingroup dbprim_smat
 * \brief Sparse matrix table flags.
 *
 * This macro retrieves the flags associated with the sparse matrix
 * table.  Only #HASH_FLAG_AUTOGROW and #HASH_FLAG_AUTOSHRINK have any
 * meaning to the application; all other bits are reserved for use in
 * the library.  This macro may be used as an lvalue, but care must be
 * taken to avoid modifying the library-specific bits.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags for
 *		the sparse matrix table.
 */
#define st_flags(table)	  ((table)->st_table.ht_flags)

/** \ingroup dbprim_smat
 * \brief Determine if a sparse matrix is frozen.
 *
 * This macro returns a non-zero value if the matrix is currently
 * frozen.  The sparse matrix may be frozen if there is an iteration
 * in progress.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	A zero value if the matrix is not frozen or a non-zero
 *		value if the matrix is frozen.
 */
#define st_frozen(table)  ((table)->st_table.ht_flags & HASH_FLAG_FREEZE)

/** \ingroup dbprim_smat
 * \brief Sparse matrix table modulus.
 *
 * This macro retrieves the number of buckets allocated for the sparse
 * matrix table.  An application may wish to save this value between
 * invocations to avoid the overhead of growing the table while
 * filling it with data.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the number of
 *		buckets allocated for the sparse matrix table.
 */
#define st_modulus(table) ((table)->st_table.ht_modulus)

/** \ingroup dbprim_smat
 * \brief Sparse matrix table count.
 *
 * This macro retrieves the total number of items actually in the
 * sparse matrix table.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing a count of
 *		the number of items in the sparse matrix table.
 */
#define st_count(table)	  ((table)->st_table.ht_count)

/** \ingroup dbprim_hash
 * \brief Sparse matrix table resize callback function.
 *
 * This macro retrieves the resize callback function pointer.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	A #smat_resize_t.
 */
#define st_rsize(table)	  ((table)->st_resize)

/** \ingroup dbprim_smat
 * \brief Extra pointer data in a sparse matrix table.
 *
 * This macro retrieves the extra pointer data associated with a
 * particular sparse matrix table.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	A pointer to \c void.
 */
#define st_extra(table)	  ((table)->st_extra)

/** \ingroup dbprim_smat
 * \brief Sparse matrix table memory size.
 *
 * This macro returns the physical size of the memory allocated by the
 * library for this sparse matrix table.
 *
 * \note The st_size() macro already counts the memory for each list
 * in the table.  Summing the results of sh_size() and st_size() will
 * over-count the amount of memory actually in use. 
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \return	A \c size_t.
 */
#define st_size(table) ((table)->st_table.ht_modulus * sizeof(link_head_t) + \
			(table)->st_table.ht_count * sizeof(smat_entry_t))

/** \ingroup dbprim_smat
 * \brief Dynamically initialize a sparse matrix table.
 *
 * This function initializes a sparse matrix table.
 *
 * \param[in]		table	A pointer to a #smat_table_t to be
 *				initialized.
 * \param[in]		flags	A bit-wise OR of #HASH_FLAG_AUTOGROW
 *				and #HASH_FLAG_AUTOSHRINK.  If neither
 *				behavior is desired, use 0.
 * \param[in]		resize	A #hash_resize_t function pointer for
 *				determining whether resizing is
 *				permitted and/or for notification of
 *				the resize.
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the sparse matrix
 *				table.
 * \param[in]		init_mod
 *				An initial modulus for the table.
 *				This will presumably be extracted by
 *				st_modulus() in a previous invocation
 *				of the application.  A 0 value is
 *				valid.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval ENOMEM		Unable to allocate memory.
 */
unsigned long st_init(smat_table_t *table, unsigned long flags,
		      smat_resize_t resize, void *extra,
		      unsigned long init_mod);

/** \ingroup dbprim_smat
 * \brief Add an entry to a sparse matrix.
 *
 * This function adds an entry to a sparse matrix.  The entry is
 * referenced in three different places, thus the complex set of
 * arguments.  This function will allocate a #smat_entry_t and return
 * it through the \c entry_p result parameter.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[out]		entry_p	A pointer to a pointer to a
 *				#smat_entry_t.  If \c NULL is passed,
 *				the addition will be performed and an
 *				appropriate error code returned.
 * \param[in]		head1	A pointer to a #smat_head_t
 * 				representing a #SMAT_LOC_FIRST sparse
 *				matrix list.
 * \param[in]		loc1	A #link_loc_t indicating where the
 *				entry should be added for \c head1.
 * \param[in]		ent1	A pointer to a #smat_entry_t
 *				describing another element in the list
 *				represented by \c head1 if \p loc1 is
 *				#LINK_LOC_BEFORE or #LINK_LOC_AFTER.
 * \param[in]		head2	A pointer to a #smat_head_t
 *				representing a #SMAT_LOC_SECOND sparse
 *				matrix list.
 * \param[in]		loc2	A #link_loc_t indicating where the
 *				entry should be added for \c head2.
 * \param[in]		ent2	A pointer to a #smat_entry_t
 *				describing another element in the list
 *				represented by \c head2 if \p loc2 is
 *				#LINK_LOC_BEFORE or #LINK_LOC_AFTER.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_BUSY		One of the arguments is already in the
 *				table.
 * \retval DB_ERR_FROZEN	The table is currently frozen.
 * \retval DB_ERR_NOTABLE	The bucket table has not been
 *				allocated and automatic growth is not
 *				enabled.
 * \retval DB_ERR_WRONGTABLE	One of the arguments was not in the
 *				proper table or list.
 * \retval DB_ERR_UNUSED	One of the \c ent arguments is not
 *				presently in a list.
 * \retval DB_ERR_UNRECOVERABLE	An unrecoverable error occurred while
 *				resizing the table.
 * \retval ENOMEM		No memory could be allocated for the
 *				#smat_entry_t structure.
 */
unsigned long st_add(smat_table_t *table, smat_entry_t **entry_p,
		     smat_head_t *head1, link_loc_t loc1, smat_entry_t *ent1,
		     smat_head_t *head2, link_loc_t loc2, smat_entry_t *ent2);

/** \ingroup dbprim_smat
 * \brief Remove an entry from a sparse matrix.
 *
 * This function removes the given entry from the specified sparse
 * matrix.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		entry	A pointer to a #smat_entry_t to be
 *				removed from the table.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_WRONGTABLE	Entry is not in this sparse matrix.
 * \retval DB_ERR_UNRECOVERABLE	An unrecoverable error occurred while
 *				removing the entry from the table.
 */
unsigned long st_remove(smat_table_t *table, smat_entry_t *entry);

/** \ingroup dbprim_smat
 * \brief Find an entry in a sparse matrix.
 *
 * This function looks up the entry matching the given \p head1 and \p
 * head2.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[out]		entry_p	A pointer to a pointer to a
 *				#smat_entry_t.  If \c NULL is passed,
 *				the lookup will be performed and an
 *				appropriate error code returned.
 * \param[in]		head1	A pointer to a #smat_head_t
 *				initialized to #SMAT_LOC_FIRST.
 * \param[in]		head2	A pointer to a #smat_head_t
 *				initialized to #SMAT_LOC_SECOND.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	One or both of \p head1 or \p head2
 *				are not referenced in this table.
 * \retval DB_ERR_NOENTRY	No matching entry was found.
 */
unsigned long st_find(smat_table_t *table, smat_entry_t **entry_p,
		      smat_head_t *head1, smat_head_t *head2);

/** \ingroup dbprim_smat
 * \brief Iterate over each entry in a sparse matrix.
 *
 * This function iterates over every entry in a sparse matrix (in an
 * unspecified order), executing the given \p iter_func on each entry.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		iter_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an entry in a sparse matrix.  \c NULL
 *				is an invalid value.  See the
 *				documentation for #smat_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The sparse matrix is frozen.
 */
unsigned long st_iter(smat_table_t *table, smat_iter_t iter_func, void *extra);

/** \ingroup dbprim_smat
 * \brief Flush a sparse matrix.
 *
 * This function flushes a sparse matrix--that is, it removes each
 * entry from the matrix.  If a \p flush_func is specified, it will be
 * called on the entry after it has been removed from the table, and
 * may safely call <CODE>free()</CODE>.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		flush_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an entry after removing it from the
 *				table.  May be \c NULL.  See the
 *				documentation for #smat_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The sparse matrix is frozen.
 */
unsigned long st_flush(smat_table_t *table, smat_iter_t flush_func,
		       void *extra);

/** \ingroup dbprim_smat
 * \brief Resize a sparse matrix table.
 *
 * This function resizes the hash table associated with a sparse
 * matrix based on the \p new_size parameter.  See the documentation
 * for ht_resize() for more information.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 * \param[in]		new_size
 *				A new size value for the table.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The table is currently frozen.
 * \retval DB_ERR_UNRECOVERABLE	A catastrophic error was encountered.
 *				The table is now unusable.
 * \retval ENOMEM		No memory could be allocated for the
 *				new bucket table.
 */
unsigned long st_resize(smat_table_t *table, unsigned long new_size);

/** \ingroup dbprim_smat
 * \brief Free memory used by an empty sparse matrix table.
 *
 * This function releases the memory used by the bucket table of the
 * empty hash table associated with a sparse matrix.
 *
 * \param[in]		table	A pointer to a #smat_table_t.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_FROZEN	The table is frozen.
 * \retval DB_ERR_NOTEMPTY	The table is not empty.
 */
unsigned long st_free(smat_table_t *table);

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix list head structure.
 *
 * This is the implementation of the #smat_head_t type.
 */
struct _smat_head_s {
  unsigned long sh_magic;	/**< Magic number. */
  smat_loc_t	sh_elem;	/**< 0 or 1 to indicate first or second. */
  smat_table_t *sh_table;	/**< Table this object's in. */
  link_head_t	sh_head;	/**< Linked list head. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix list head magic number.
 *
 * This is the magic number used for the sparse matrix list head
 * structure.
 */
#define SMAT_HEAD_MAGIC 0x4e5d9b8e

/** \ingroup dbprim_smat
 * \brief Sparse matrix list head static initializer.
 *
 * This macro statically initializes a #smat_head_t.
 *
 * \param[in]		elem	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND specifing whether the
 *				object is a member of the set of rows
 *				or columns.
 * \param[in]		object	A pointer to \c void representing the
 *				object associated with the list head.
 */
#define SMAT_HEAD_INIT(elem, object) \
	{ SMAT_HEAD_MAGIC, (elem), 0, LINK_HEAD_INIT(object) }

/** \ingroup dbprim_smat
 * \brief Sparse matrix list head verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * sparse matrix head.
 *
 * \warning	This macro evaluates the \p head argument twice.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 *
 * \return	Boolean \c true if \p head is a valid sparse matrix
 *		head or \c false otherwise.
 */
#define sh_verify(head)		((head) && \
				 (head)->sh_magic == SMAT_HEAD_MAGIC)

/** \ingroup dbprim_smat
 * \brief Sparse matrix list head element macro.
 *
 * This macro retrieves the position indicator for the sparse matrix
 * head.  It will return one of #SMAT_LOC_FIRST or #SMAT_LOC_SECOND.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	An #smat_loc_t.
 */
#define sh_elem(head)	((head)->sh_elem)

/** \ingroup dbprim_smat
 * \brief Sparse matrix list head table pointer.
 *
 * If there are any elements in this sparse matrix list head, this
 * macro will retrieve a pointer to the table in which they reside.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to #smat_table_t.
 */
#define sh_table(head)	((head)->sh_table)

/** \ingroup dbprim_smat
 * \brief Determine if a sparse matrix is frozen.
 *
 * This macro returns a non-zero value if the matrix is currently
 * frozen.  The sparse matrix may be frozen if there is an iteration
 * in progress.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 *
 * \return	A zero value if the matrix is not frozen or a non-zero
 *		value if the matrix is frozen.
 */
#define sh_frozen(head) (st_frozen((head)->sh_table))

/** \ingroup dbprim_smat
 * \brief Sparse matrix list count.
 *
 * This macro retrieves the number of elements in the sparse matrix
 * list rooted at \p head.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing a count of
 *		the number of elements in the sparse matrix list.
 */
#define sh_count(head)	((head)->sh_head.lh_count)

/** \internal
 * \ingroup dbprim_smat
 * \brief Access the first element pointer in a #smat_head_t.
 *
 * This helper macro is used to directly access the first linked list
 * element of a #smat_head_t.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to a #link_elem_t indicating the first
 *		element in the list.
 */
#define _sh_first(head)	((head)->sh_head.lh_first)

/** \ingroup dbprim_smat
 * \brief First element in sparse matrix list.
 *
 * This macro retrieves a pointer to the #smat_entry_t for the first
 * element in the sparse matrix list.
 *
 * \warning	This macro evaluates the \p head argument twice.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to #smat_entry_t.
 */
#define sh_first(head)	(_sh_first(head) ? _smat_ent(_sh_first(head)) : 0)

/** \internal
 * \ingroup dbprim_smat
 * \brief Access the last element pointer in a #smat_head_t.
 *
 * This helper macro is used to directly access the last linked list
 * element of a #smat_head_t.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to a #link_elem_t indicating the last
 *		element in the list.
 */
#define _sh_last(head)	((head)->sh_head.lh_last)

/** \ingroup dbprim_smat
 * \brief Last element in sparse matrix list.
 *
 * This macro retrieves a pointer to the #smat_entry_t for the last
 * element in the sparse matrix list.
 *
 * \warning	This macro evaluates the \p head argument twice.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to #smat_entry_t.
 */
#define sh_last(head)	(_sh_last(head) ? _smat_ent(_sh_last(head)) : 0)

/** \ingroup dbprim_smat
 * \brief Object represented by a sparse matrix list head.
 *
 * This macro retrieves a pointer to the object referenced by the
 * sparse matrix list head.
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A pointer to \c void.
 */
#define sh_object(head)	((head)->sh_head.lh_extra)

/** \ingroup dbprim_smat
 * \brief Sparse matrix list memory size.
 *
 * This macro returns the physical size of the memory allocated by the
 * library for this sparse matrix list.
 *
 * \note The st_size() macro already counts the memory for each list
 * in the table.  Summing the results of sh_size() and st_size() will
 * over-count the amount of memory actually in use. 
 *
 * \param[in]		head	A pointer to #smat_head_t.
 *
 * \return	A \c size_t.
 */
#define sh_size(head)	((head)->sh_elem.lh_count * sizeof(smat_entry_t))

/** \ingroup dbprim_smat
 * \brief Dynamically initialize a sparse matrix row or column head.
 *
 * This function dynamically initializes a sparse matrix row or column
 * linked list head.  The \p elem argument specifies whether the
 * object is to be associated with a #SMAT_LOC_FIRST list or a
 * #SMAT_LOC_SECOND list.
 *
 * \param[in]		head	A pointer to a #smat_head_t to be
 *				initialized.
 * \param[in]		elem	Either #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND.
 * \param[in]		object	A pointer to the object containing the
 *				sparse matrix row or column head.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 */
unsigned long sh_init(smat_head_t *head, smat_loc_t elem, void *object);

/** \ingroup dbprim_smat
 * \brief Move an entry within a row or column list.
 *
 * This function allows the specified entry to be shifted within the
 * linked list describing the row or column.  It is very similar to
 * the ll_move() function.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 * \param[in]		elem	A pointer to the #smat_entry_t
 *				describing the entry to be moved.
 * \param[in]		loc	A #link_loc_t indicating where the
 *				entry should be moved to.
 * \param[in]		elem2	A pointer to a #smat_entry_t
 *				describing another entry in the list
 *				if \p loc is #LINK_LOC_BEFORE or
 *				#LINK_LOC_AFTER.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_BUSY		\p elem and \p elem2 are the same
 *				entry.
 * \retval DB_ERR_WRONGTABLE	\p elem or \p elem2 are in a different
 *				row or column.
 * \retval DB_ERR_UNUSED	\p elem or \p elem2 are not in any row
 *				or column.
 */
unsigned long sh_move(smat_head_t *head, smat_entry_t *elem, link_loc_t loc,
		      smat_entry_t *elem2);

/** \ingroup dbprim_smat
 * \brief Find an entry in a row or column of a sparse matrix.
 *
 * This function iterates through the given row or column of a
 * sparse matrix looking for an element that matches the given \p key.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 * \param[out]		elem_p	A pointer to a pointer to a
 *				#smat_entry_t.  \c NULL is an invalid
 *				value.
 * \param[in]		comp_func
 *				A pointer to a comparison function
 *				used to compare the key to a
 *				particular entry.  See the
 *				documentation for #smat_comp_t for
 *				more information.
 * \param[in]		start	A pointer to a #smat_entry_t
 *				describing where in the row or column
 *				to start.  If \c NULL is passed, the
 *				beginning of the row or column will be
 *				assumed.
 * \param[in]		key	A key to search for.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this row or column.
 * \retval DB_ERR_NOENTRY	No matching entry was found.
 */
unsigned long sh_find(smat_head_t *head, smat_entry_t **elem_p,
		      smat_comp_t comp_func, smat_entry_t *start,
		      db_key_t *key);

/** \ingroup dbprim_smat
 * \brief Iterate over each entry in a row or column of a sparse
 * matrix.
 *
 * This function iterates over a row or column of a sparse matrix,
 * executing the given \p iter_func for each entry.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 * \param[in]		start	A pointer to a #smat_entry_t
 *				describing where in the row or column
 *				to start.  If \c NULL is passed, the
 *				beginning of the row or column will be
 *				assumed.
 * \param[in]		iter_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on
 *				an entry in a row or column of a
 *				sparse matrix.  \c NULL is an invalid
 *				value.  See the documentation for
 *				#smat_iter_t for more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 * \param[in]		flags	If #DB_FLAG_REVERSE is given,
 *				iteration will be done from the end of
 *				the list backwards towards the head.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this row or column.
 */
unsigned long sh_iter(smat_head_t *head, smat_entry_t *start,
		      smat_iter_t iter_func, void *extra, unsigned long flags);

/** \ingroup dbprim_smat
 * \brief Flush a row or column of a sparse matrix.
 *
 * This function flushes a sparse matrix row or column--that is, it
 * removes each element from that row or column.  If a \p flush_func
 * is specified, it will be called on the entry after it has been
 * removed from the row or column, and may safely call
 * <CODE>free()</CODE>.
 *
 * \param[in]		head	A pointer to a #smat_head_t.
 * \param[in]		flush_func
 *				A pointer to a callback function used
 *				to perform user-specifed actions on an
 *				entry after removing it from the row
 *				or column.  May be \c NULL.  See the
 *				documentation for #smat_iter_t for
 *				more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p flush_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 */
unsigned long sh_flush(smat_head_t *head, smat_iter_t flush_func, void *extra);

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix entry structure.
 *
 * This is the implementation of the #smat_entry_t type.
 */
struct _smat_entry_s {
  unsigned long	se_magic;	/**< Magic number. */
  smat_table_t *se_table;	/**< Sparse matrix table. */
  hash_entry_t	se_hash;	/**< Hash table entry. */
  link_elem_t	se_link[2];	/**< Linked list elements. */
  void	       *se_object[2];	/**< Objects. */
};

/** \internal
 * \ingroup dbprim_smat
 * \brief Sparse matrix entry magic number.
 *
 * This is the magic number used for the sparse matrix entry
 * structure.
 */
#define SMAT_ENTRY_MAGIC 0x466b34b5

/** \ingroup dbprim_smat
 * \brief Sparse matrix entry verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * sparse matrix entry.
 *
 * \warning	This macro evaluates the \p entry argument twice.
 *
 * \param[in]		entry	A pointer to a #smat_entry_t.
 *
 * \return	Boolean \c true if \p entry is a valid sparse matrix
 *		entry or \c false otherwise.
 */
#define se_verify(entry)	((entry) && \
				 (entry)->se_magic == SMAT_ENTRY_MAGIC)

/** \ingroup dbprim_smat
 * \brief Sparse matrix entry table.
 *
 * This macro retrieves a pointer to the table that the sparse matrix
 * entry is in.
 *
 * \param[in]		entry	A pointer to a #smat_entry_t.
 *
 * \return	A pointer to a #smat_table_t.
 */
#define se_table(entry)	    ((entry)->se_table)

/** \ingroup dbprim_smat
 * \internal
 * \brief Sparse matrix entry linked list element.
 *
 * This macro provides access to the linked list element buried in the
 * sparse matrix entry for use by the free list routines.
 *
 * \param[in]		entry	A pointer to a #smat_entry_t.
 *
 * \return	A pointer to a #link_elem_t.
 */
#define _se_link(entry)	    (&((entry)->se_hash.he_elem))

/** \ingroup dbprim_smat
 * \brief Sparse matrix entry flags.
 *
 * This macro retrieves a set of user-defined flags associated with
 * the entry.  It may be used as an lvalue to set those flags.
 *
 * \param[in]		entry	A pointer to a #smat_entry_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags
 *		associated with the entry.
 */
#define se_flags(entry)     ((entry)->se_hash.he_elem.le_flags)

/** \ingroup dbprim_smat
 * \brief Sparse matrix table entry hash value.
 *
 * This macro retrieves the hash value of the given sparse matrix
 * entry.  If the sparse matrix hash been resized, this value may not
 * be the same as a previous value.
 *
 * \param[in]		entry	A pointer to a #smat_entry_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing the hash code
 *		for the entry.
 */
#define se_hash(entry)	    ((entry)->se_hash.he_hash)

/** \internal
 * \ingroup dbprim_smat
 * \brief Access the next element pointer in a #smat_entry_t.
 *
 * This helper macro is used to directly access the next linked list
 * element in a specific sparse matrix linked list.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	A pointer to a #link_elem_t indicating the next
 *		element in the list.
 */
#define _se_next(entry, n)  ((entry)->se_link[(n)].le_next)

/** \ingroup dbprim_smat
 * \brief Next element in sparse matrix list.
 *
 * This macro retrieves a pointer to the #link_elem_t for the next
 * element in the sparse matrix list.
 *
 * \warning	This macro evaluates the \p entry and \p n arguments
 *		twice.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	A pointer to #smat_entry_t.
 */
#define se_next(entry, n)   (_se_next(entry, n) ? \
			     _smat_ent(_se_next(entry, n)) : 0)

/** \internal
 * \ingroup dbprim_smat
 * \brief Access the previous element pointer in a #smat_entry_t.
 *
 * This helper macro is used to directly access the previous linked
 * list element in a specific sparse matrix linked list.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	A pointer to a #link_elem_t indicating the previous
 *		element in the list.
 */
#define _se_prev(entry, n)  ((entry)->se_link[(n)].le_prev)

/** \ingroup dbprim_smat
 * \brief Previous element in sparse matrix list.
 *
 * This macro retrieves a pointer to the #link_elem_t for the previous
 * element in the sparse matrix list.
 *
 * \warning	This macro evaluates the \p entry and \p n arguments
 *		twice.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	A pointer to #smat_entry_t.
 */
#define se_prev(entry, n)   (_se_prev(entry, n) ? \
			     _smat_ent(_se_prev(entry, n)) : 0)

/** \ingroup dbprim_smat
 * \brief Flags associated with an entry in a sparse matrix list.
 *
 * This macro retrieves a set of user-defined flags associated with
 * the entry in a sparse matrix list.  It may be used as an lvalue to
 * set those flags.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	An <CODE>unsigned long</CODE> containing the flags
 *		associated with the entry.
 */
#define se_lflags(entry, n) ((entry)->se_link[(n)].le_flags)

/** \ingroup dbprim_smat
 * \brief Object associated with an entry in a sparse matrix list.
 *
 * This macro retrieves a pointer to one of the objects represented by
 * the entry.  It may be used as an lvalue to change the object
 * pointed to.  Care should be taken when using this feature.
 *
 * \param[in]		entry	A pointer to #smat_entry_t.
 * \param[in]		n	One of #SMAT_LOC_FIRST or
 *				#SMAT_LOC_SECOND to specify which list
 *				thread is desired.
 *
 * \return	A pointer to \c void representing the object.
 */
#define se_object(entry, n) ((entry)->se_object[(n)])

/** \ingroup dbprim_rbtree
 * \brief Red-black tree comparison function.
 *
 * This is a red-black tree comparison function, compatible with
 * #rb_comp_t, based around memcmp().
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		key1	The first key being compared.
 * \param[in]		key2	The second key being compared.
 *
 * \return	Zero if the keys match, less than zero if the first
 *		key orders before the second key, or greater than zero
 *		if the first key orders after the second key.
 */
long rbtree_comp(rb_tree_t *tree, db_key_t *key1, db_key_t *key2);

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Red-black tree structure.
 *
 * This is the implementation of the #rb_tree_t type.
 */
struct _rb_tree_s {
  unsigned long	rt_magic;	/**< Magic number. */
  unsigned long	rt_flags;	/**< Flags associated with the table. */
  unsigned long	rt_count;	/**< Number of nodes in the tree. */
  rb_node_t    *rt_root;	/**< Pointer to the root node of the tree. */
  rb_comp_t	rt_comp;	/**< Function for comparing tree keys. */
  void	       *rt_extra;	/**< Extra data associated with the tree. */
};

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Red-black tree magic number.
 *
 * This is the magic number used for the red-black tree structure.
 */
#define RB_TREE_MAGIC 0xd52695be

/** \internal
 * \brief Flag indicating red-black tree is frozen.
 *
 * This flag, if set on a red-black tree, indicates that the tree is
 * frozen and may not be modified.
 */
#define RBT_FLAG_FREEZE 0x80000000 /* tree frozen */

/** \ingroup dbprim_rbtree
 * \brief Red-black tree static initializer.
 *
 * This macro statically initializes a #rb_tree_t.
 *
 * \param[in]		comp	A #rb_comp_t function pointer for a
 *				comparison function.
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the red-black tree.
 */
#define RB_TREE_INIT(comp, extra)   { RB_TREE_MAGIC, 0, 0, 0, (comp), (extra) }

/** \ingroup dbprim_rbtree
 * \brief Red-black tree verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * red-black tree.
 *
 * \warning	This macro evaluates the \p tree argument twice.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	Boolean \c true if \p tree is a valid red-black tree
 *		or \c false otherwise.
 */
#define rt_verify(tree)		((tree) && \
				 (tree)->rt_magic == RB_TREE_MAGIC)

/** \ingroup dbprim_rbtree
 * \brief Determine if a red-black tree is frozen.
 *
 * This macro returns a non-zero value if the tree is currently
 * frozen.  The red-black tree may be frozen if there is an
 * iteration in progress.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	A zero value if the table is not frozen or a non-zero
 *		value if the table is frozen.
 */
#define rt_frozen(tree)		((tree)->rt_flags & RBT_FLAG_FREEZE)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree count.
 *
 * This macro retrieves the total number of items actually in the
 * red-black tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	An <CODE>unsigned long</CODE> containing a count of
 *		the number of items in the red-black tree.
 */
#define rt_count(tree)		((tree)->rt_count)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree root node.
 *
 * This macro retrieves the root node of the tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	A pointer to a #rb_node_t.
 */
#define rt_root(tree)		((tree)->rt_root)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree comparison function.
 *
 * This macro retrieves the comparison function pointer.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	A #rb_comp_t.
 */
#define rt_comp(tree)		((tree)->rt_comp)

/** \ingroup dbprim_rbtree
 * \brief Extra pointer data in a red-black tree.
 *
 * This macro retrieves the extra pointer data associated with a
 * particular red-black tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 *
 * \return	A pointer to \c void.
 */
#define rt_extra(tree)		((tree)->rt_extra)

/** \ingroup dbprim_rbtree
 * \brief Preorder tree traversal method.
 *
 * If this flag is passed to rt_iter(), a preorder iteration will
 * be performed.
 */
#define RBT_ORDER_PRE	1

/** \ingroup dbprim_rbtree
 * \brief Inorder tree traversal method.
 *
 * If this flag is passed to rt_iter(), an inorder iteration will
 * be performed.
 */
#define RBT_ORDER_IN	2

/** \ingroup dbprim_rbtree
 * \brief Postorder tree traversal method.
 *
 * If this flag is passed to rt_iter(), a postorder iteration will
 * be performed.
 */
#define RBT_ORDER_POST	3

/** \ingroup dbprim_rbtree
 * \brief Tree traversal method mask.
 *
 * This flag mask may be used to obtain the tree traversal order.
 */
#define RBT_ORDER_MASK	0x00000003

/** \ingroup dbprim_rbtree
 * \brief Get the previous node.
 *
 * Obtains the previous node in the given iteration scheme.  See
 * rt_next() for more information.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in,out]	node_io	A pointer to a pointer to a
 *				#rb_node_t.  If the pointer to which
 *				node_io points is \c NULL, the first
 *				node will be loaded, otherwise the
 *				next node will be loaded.
 * \param[in]		flags	One of RBT_ORDER_PRE, RBT_ORDER_IN, or
 *				RBT_ORDER_POST, possibly ORed with
 *				DB_FLAG_REVERSE to reverse the order
 *				of iteration.  Zero is not allowed.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this red-black
 *				tree.
 */
#define rt_prev(tree, node_io, flags) \
	(rt_next((tree), (node_io), (flags) ^ DB_FLAG_REVERSE))

/** \ingroup dbprim_rbtree
 * \brief Dynamically initialize a red-black tree.
 *
 * This function dynamically initializes a red-black tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t to be
 *				initialized.
 * \param[in]		comp	A #rb_comp_t function pointer for a
 *				comparison function.
 * \param[in]		extra	Extra pointer data that should be
 *				associated with the tree.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 */
unsigned long rt_init(rb_tree_t *tree, rb_comp_t comp, void *extra);

/** \ingroup dbprim_rbtree
 * \brief Add a node to a red-black tree.
 *
 * This function adds a node to a red-black tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		node	A pointer to a #rb_node_t to be added
 *				to the tree.
 * \param[in]		key	A pointer to a #db_key_t containing
 *				the key for the node.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_BUSY		The node is already in a tree.
 * \retval DB_ERR_FROZEN	The tree is currently frozen.
 * \retval DB_ERR_DUPLICATE	The entry is a duplicate of an
 *				existing node.
 */
unsigned long rt_add(rb_tree_t *tree, rb_node_t *node, db_key_t *key);

/** \ingroup dbprim_rbtree
 * \brief Move a node in a red-black tree.
 *
 * This function moves an existing node in the red-black tree to
 * correspond to the new key.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		node	A pointer to a #rb_node_t to be moved.
 *				It must already be in the red-black
 *				tree.
 * \param[in]		key	A pointer to a #db_key_t describing
 *				the new key for the node.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_UNUSED	Node is not in a red-black tree.
 * \retval DB_ERR_WRONGTABLE	Node is not in this tree.
 * \retval DB_ERR_FROZEN	Red-black tree is frozen.
 * \retval DB_ERR_DUPLICATE	New key is a duplicate of an existing
 *				key.
 * \retval DB_ERR_READDFAILED	Unable to re-add node to tree.
 */
unsigned long rt_move(rb_tree_t *tree, rb_node_t *node, db_key_t *key);

/** \ingroup dbprim_rbtree
 * \brief Remove a node from a red-black tree.
 *
 * This function removes the given node from the specified red-black
 * tree.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		node	A pointer to a #rb_node_t to be
 *				removed from the tree.
 *
 * \retval DB_ERR_BADARGS	An invalid argument was given.
 * \retval DB_ERR_UNUSED	Node is not in a red-black tree.
 * \retval DB_ERR_WRONGTABLE	Node is not in this tree.
 * \retval DB_ERR_FROZEN	Red-black tree is frozen.
 */
unsigned long rt_remove(rb_tree_t *tree, rb_node_t *node);

/** \ingroup dbprim_rbtree
 * \brief Find an entry in a red-black table.
 *
 * This function looks up an entry matching the given \p key.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[out]		node_p	A pointer to a pointer to a
 *				#rb_node_t.  If \c NULL is passed, the
 *				lookup will be performed and an
 *				appropriate error code returned.
 * \param[in]		key	A pointer to a #db_key_t describing
 *				the item to find.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_NOENTRY	No matching entry was found.
 */
unsigned long rt_find(rb_tree_t *tree, rb_node_t **node_p, db_key_t *key);

/** \ingroup dbprim_rbtree
 * \brief Get the next node.
 *
 * This function obtains the next node in the given iteration scheme.
 * The \p node_io parameter is a value-result parameter--if the node
 * pointer to which it points is \c NULL, the first node for the given
 * iteration order will be loaded; otherwise, the next node in the
 * given iteration order will be loaded.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in,out]	node_io	A pointer to a pointer to a
 *				#rb_node_t.  If the pointer to which
 *				node_io points is \c NULL, the first
 *				node will be loaded, otherwise the
 *				next node will be loaded.
 * \param[in]		flags	One of RBT_ORDER_PRE, RBT_ORDER_IN, or
 *				RBT_ORDER_POST, possibly ORed with
 *				DB_FLAG_REVERSE to reverse the order
 *				of iteration.  Zero is not allowed.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this red-black
 *				tree.
 */
unsigned long rt_next(rb_tree_t *tree, rb_node_t **node_io,
		      unsigned long flags);

/** \ingroup dbprim_rbtree
 * \brief Iterate over each entry in a red-black tree.
 *
 * This function iterates over every node in a red-black tree in the
 * given traversal order, executing the given \p iter_func on each
 * node.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		start	A pointer to a #rb_node_t describing
 *				where in the tree to start.  If \c
 *				NULL is passed, the first element of
 *				the tree for the specified order will
 *				be assumed.
 * \param[in]		iter_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on a
 *				node in the red-black tree.  \c NULL
 *				is an invalid value.  See the
 *				documentation for #rb_iter_t for more
 *				information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p iter_func.
 * \param[in]		flags	One of RBT_ORDER_PRE, RBT_ORDER_IN, or
 *				RBT_ORDER_POST, possibly ORed with
 *				DB_FLAG_REVERSE to reverse the order
 *				of iteration.  Zero is not allowed.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_WRONGTABLE	\p start is not in this red-black
 *				tree.
 */
unsigned long rt_iter(rb_tree_t *tree, rb_node_t *start,
		      rb_iter_t iter_func, void *extra, unsigned long flags);

/** \ingroup dbprim_rbtree
 * \brief Flush a red-black tree.
 *
 * This function flushes a red-black tree--that is, it removes each
 * node from the tree.  If a \p flush_func is specified, it will be
 * called on the node after it has been removed from the tree, and may
 * safely call <CODE>free()</CODE>.
 *
 * \param[in]		tree	A pointer to a #rb_tree_t.
 * \param[in]		flush_func
 *				A pointer to a callback function used
 *				to perform user-specified actions on a
 *				node after removing it from the tree.
 *				May be \c NULL.  See the documentation
 *				for #rb_iter_t for more information.
 * \param[in]		extra	A \c void pointer that will be passed
 *				to \p flush_func.
 *
 * \retval DB_ERR_BADARGS	An argument was invalid.
 * \retval DB_ERR_FROZEN	The red-black tree is frozen.
 */
unsigned long rt_flush(rb_tree_t *tree, rb_iter_t flush_func, void *extra);

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Red-black tree node structure.
 *
 * This is the implementation of the #rb_node_t type.
 */
struct _rb_node_s {
  unsigned long	rn_magic;	/**< Magic number. */
  rb_color_t	rn_color;	/**< Node's color. */
  rb_tree_t    *rn_tree;	/**< Tree node is in. */
  rb_node_t    *rn_parent;	/**< Parent of this node. */
  rb_node_t    *rn_left;	/**< Left child of this node. */
  rb_node_t    *rn_right;	/**< Right child of this node. */
  db_key_t	rn_key;		/**< Node's key. */
  void	       *rn_value;	/**< Node's value. */
};

/** \internal
 * \ingroup dbprim_rbtree
 * \brief Red-black tree node magic number.
 *
 * This is the magic number used for the red-black tree structure.
 */
#define RB_NODE_MAGIC 0x3dea4d01

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node static initializer.
 *
 * This macro statically initializes a #rb_node_t.
 *
 * \param[in]		value	A pointer to \c void representing the
 *				object associated with the node.
 */
#define RB_NODE_INIT(value) \
	{ RB_NODE_MAGIC, RB_COLOR_NONE, 0, 0, 0, 0, DB_KEY_INIT(0, 0), (value)}

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node verification macro.
 *
 * This macro verifies that a given pointer actually does point to a
 * red-black tree node.
 *
 * \warning	This macro evaluates the \p node argument twice.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	Boolean \c true if \p entry is a valid red-black tree
 *		node or \c false otherwise.
 */
#define rn_verify(node)		((node) && \
				 (node)->rn_magic == RB_NODE_MAGIC)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node color.
 *
 * This macro retrieves the color of the \p node.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A #rb_color_t value expressing the color of the
 *		\p node.
 */
#define rn_color(node)		((node)->rn_color)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's tree pointer.
 *
 * This macro retrieves a pointer to the red-black tree the node
 * is in.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to a #rb_tree_t.
 */
#define rn_tree(node)		((node)->rn_tree)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's parent pointer.
 *
 * This macro retrieves a pointer to the node's parent node.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to a #rb_node_t representing the parent
 *		of the given \p node.
 */
#define rn_parent(node)		((node)->rn_parent)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's left pointer.
 *
 * This macro retrieves a pointer to the node's left node.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to a #rb_node_t representing the left
 *		node of the given \p node.
 */
#define rn_left(node)		((node)->rn_left)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's right pointer.
 *
 * This macro retrieves a pointer to the node's right node.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to a #rb_node_t representing the right
 *		node of the given \p node.
 */
#define rn_right(node)		((node)->rn_right)

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's key pointer.
 *
 * This macro retrieves the key associated with the red-black tree
 * node.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to a #db_key_t.
 */
#define rn_key(node)		(&((node)->rn_key))

/** \ingroup dbprim_rbtree
 * \brief Red-black tree node's value pointer.
 *
 * This macro retrieves the value associated with the red-black
 * tree's node.  It may be treated as an lvalue to change that value.
 * Care should be taken when using this option.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	A pointer to \c void representing the value associated
 *		with this node.
 */
#define rn_value(node)		((node)->rn_value)

/** \ingroup dbprim_rbtree
 * \brief Test if a given node is black.
 *
 * This macro safely tests whether a given red-black tree node is
 * black.
 *
 * \warning	This macro evaluates the \p node argument twice.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	Boolean \c true if \p node is black or \c false
 *		otherwise.
 */
#define rn_isblack(node)	(!(node) || (node)->rn_color == RB_COLOR_BLACK)

/** \ingroup dbprim_rbtree
 * \brief Test if a given node is red.
 *
 * This macro safely tests whether a given red-black tree node is
 * red.
 *
 * \warning	This macro evaluates the \p node argument twice.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	Boolean \c true if \p node is red or \c false
 *		otherwise.
 */
#define rn_isred(node)		((node) && (node)->rn_color == RB_COLOR_RED)

/** \ingroup dbprim_rbtree
 * \brief Test if a given node is the left node of its parent.
 *
 * This macro safely tests whether a given red-black tree node is
 * the left node of its parent.
 *
 * \warning	This macro evaluates the \p node argument three
 *		times.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	Boolean \c true if \p node is the left node of its
 *		parent or \c false otherwise.
 */
#define rn_isleft(node)		((node)->rn_parent && \
				 (node)->rn_parent->rn_left == (node))

/** \ingroup dbprim_rbtree
 * \brief Test if a given node is the right node of its parent.
 *
 * This macro safely tests whether a given red-black tree node is
 * the right node of its parent.
 *
 * \warning	This macro evaluates the \p node argument three
 *		times.
 *
 * \param[in]		node	A pointer to a #rb_node_t.
 *
 * \return	Boolean \c true if \p node is the right node of its
 *		parent or \c false otherwise.
 */
#define rn_isright(node)	((node)->rn_parent && \
				 (node)->rn_parent->rn_right == (node))

/** \ingroup dbprim_rbtree
 * \brief Dynamically initialize a red-black tree node.
 *
 * This function dynamically initializes a red-black tree node.
 *
 * \param[in]		node	A pointer to a #rb_tree_t to be
 *				initialized.
 * \param[in]		value	A pointer to \c void which will be the
 *				value of the red-black tree entry.
 *
 * \retval DB_ERR_BADARGS	A \c NULL pointer was passed for \p
 *				node.
 */
unsigned long rn_init(rb_node_t *node, void *value);

DBPRIM_END_C_DECLS

#endif /* __include_dbprim_h__ */

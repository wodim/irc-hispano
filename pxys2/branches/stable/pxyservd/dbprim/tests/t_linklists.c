/*
** Copyright (C) 2004 by Kevin L. Mitchell <klmitch@mit.edu>
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
** @(#)$Id: t_linklists.c,v 1.3 2006/09/04 15:12:27 spale Exp $
*/
#include <stdio.h>

#include "test-harness.h"

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: t_linklists.c,v 1.3 2006/09/04 15:12:27 spale Exp $");

TEST_PROG(t_linklists, "Test linked list functionality")

struct tdata_s {
  link_loc_t	loc;
  int		elem;
} tdata[] = {
  { LINK_LOC_HEAD, -1 },
  { LINK_LOC_HEAD, -1 },
  { LINK_LOC_TAIL, -1 },
  { LINK_LOC_TAIL, -1 },
  { LINK_LOC_BEFORE, 0 },
  { LINK_LOC_AFTER, 2 }
};

#define LINK_ELEM_CNT	(sizeof(tdata) / sizeof(struct tdata_s))

int order_add[] = { 1, 4, 0, 2, 5, 3, -1 };

struct tmove_s {
  int		elem;
  link_loc_t	loc;
  int		elem2;
} tmove[] = {
  { 2, LINK_LOC_HEAD, -1 },
  { 0, LINK_LOC_TAIL, -1 },
  { 5, LINK_LOC_AFTER, 2 },
  { 4, LINK_LOC_BEFORE, 0 }
};

#define LINK_MOVE_CNT	(sizeof(tmove) / sizeof(struct tmove_s))

int order_move[] = { 2, 5, 1, 3, 4, 0, -1 };

int tremove[] = { 2, 0, 3 };

#define LINK_REMOVE_CNT	(sizeof(tremove) / sizeof(int))

int order_remove[] = { 5, 1, 4, -1 };

static unsigned long
t_comp(db_key_t *key, void *comp)
{
  return dk_len(key) != (int)comp;
}

struct iter_desc {
  int		elem;
  unsigned long	err;
  int		start;
  unsigned long	flags;
  unsigned long	visited;
  unsigned long	expected;
};

static unsigned long
t_iter(link_head_t *head, link_elem_t *elem, void *extra)
{
  struct iter_desc *desc = (struct iter_desc *)extra;

  desc->visited |= 1 << (int)le_object(elem); /* mark that we visited node */

  /* return the error we want for testing... */
  return ((int)le_object(elem) == desc->elem) ? desc->err : 0;
}

struct iter_desc desc_iter[] = {
  { 1, 55, -1, 0, 0, (1 << 5) | (1 << 1) },
  { -1, 55, -1, 0, 0, (1 << 5) | (1 << 1) | (1 << 4) },
  { 1, 55, -1, DB_FLAG_REVERSE, 0, (1 << 4) | (1 << 1) },
  { -1, 55, -1, DB_FLAG_REVERSE, 0, (1 << 4) | (1 << 1) | (1 << 5) },
  { 1, 55, 1, 0, 0, (1 << 1) }
};

#define LINK_ITER_CNT	(sizeof(desc_iter) / sizeof(struct iter_desc))

struct iter_desc desc_flush[] = {
  { 1, 55, -1, 0, 0, (1 << 5) | (1 << 1) },
  { -1, 55, -1, 0, 0, (1 << 4) }
};

int order_flush[][2] = {
  { 4, -1 },
  { -1, -1 }
};

#define LINK_FLUSH_CNT	(sizeof(desc_flush) / sizeof(struct iter_desc))

#define ChkOrder(name, order, fatal, go)				      \
do {									      \
  int _err = 0;								      \
  char *_name = TEST_NAME(name);					      \
  int *_order = (order);						      \
  link_elem_t *_le = ll_first(&head), *_ple = ll_first(&head);		      \
  for (; *_order >= 0 && _le; _order++, _ple = _le, _le = le_next(_le))	      \
    if (_le != &elems[*_order]) {					      \
      _err++;								      \
      fprintf(stderr, "%s ordering error: Expected %d, got %d\n", _name,      \
	      *_order, _le - elems);					      \
    }									      \
  if (_err) {								      \
    FAIL(_name, (fatal), "Ordering failure: %d errors", _err);		      \
    go									      \
  } else if (*_order != -1 || _le != 0 || _ple != ll_last(&head)) {	      \
    FAIL(_name, (fatal), "List incorrectly terminated");		      \
    go									      \
  }									      \
} while (0)

int
main(int argc, char **argv)
{
  unsigned long err;
  link_head_t head; /* a linked list to play with... */
  link_elem_t elems[LINK_ELEM_CNT]; /* some elements to operate on... */
  link_elem_t *le = 0;
  db_key_t tkey = DB_KEY_INIT(0, 1);
  int i;

  /* First, test ll_init()... */
  TEST(t_linklists, ll_init, "Test that ll_init() may be called",
       (!(err = ll_init(&head, 0))), FATAL(0),
       ("ll_init() call successful"),
       ("ll_init() call failed with error %lu", err));

  /* Now, check le_init()... */
  TEST_DECL(t_linklists, le_init, "Test that le_init() may be called")
  for (i = 0; i < LINK_ELEM_CNT; i++)
    if ((err = le_init(&elems[i], (void *)i)))
      FAIL(TEST_NAME(le_init), FATAL(0), "le_init() failed with error %lu",
	   err);
  PASS(TEST_NAME(le_init), "le_init() calls successful");

  /* OK, does ll_add() work correctly? */
  TEST_DECL(t_linklists, ll_add, "Test that ll_add() works properly")
  for (i = 0; i < LINK_ELEM_CNT; i++)
    if ((err = ll_add(&head, &elems[i], tdata[i].loc,
		      tdata[i].elem < 0 ? 0 : &elems[tdata[i].elem])))
      FAIL(TEST_NAME(ll_add), FATAL(0), "ll_add() failed with error %lu",
	   err);
  /* check that the order is right... */
  ChkOrder(ll_add, order_add, FATAL(0),);
  PASS(TEST_NAME(ll_add), "ll_add() calls successful");

  /* Now let's try a few ll_move()'s... */
  TEST_DECL(t_linklists, ll_move, "Test that ll_move() works properly")
  for (i = 0; i < LINK_MOVE_CNT; i++)
    if ((err = ll_move(&head, &elems[tmove[i].elem], tmove[i].loc,
		       tmove[i].elem2 < 0 ? 0 : &elems[tmove[i].elem2])))
      FAIL(TEST_NAME(ll_move), FATAL(0), "ll_move() failed with error %lu",
	   err);
  /* check that the order is right... */
  ChkOrder(ll_move, order_move, FATAL(0),);
  PASS(TEST_NAME(ll_move), "ll_move() calls successful");

  /* Now test ll_remove()... */
  TEST_DECL(t_linklists, ll_remove, "Test that ll_remove() works properly")
  for (i = 0; i < LINK_REMOVE_CNT; i++)
    if ((err = ll_remove(&head, &elems[tremove[i]])))
      FAIL(TEST_NAME(ll_remove), FATAL(0), "ll_remove() failed with error %lu",
	   err);
  /* check that the order is right... */
  ChkOrder(ll_remove, order_remove, FATAL(0),);
  PASS(TEST_NAME(ll_remove), "ll_remove() calls successful");

  /* Let's test to see if ll_find() works... */
  TEST(t_linklists, ll_find, "Test that ll_find() works properly",
       (!(err = ll_find(&head, &le, t_comp, 0, &tkey)) &&
	le == &elems[dk_len(&tkey)] &&
	(err = ll_find(&head, &le, t_comp, &elems[4],
		       &tkey)) == DB_ERR_NOENTRY &&
	le == &elems[dk_len(&tkey)]), 0,
       ("ll_find() call successful"),
       ("ll_find() call failed or failed to find key; error %lu, elem %d (%d)",
	err, le ? (le - elems) : -1, dk_len(&tkey)));

  /* Now test ll_iter()... */
  TEST_DECL(t_linklists, ll_iter, "Test that ll_iter() works properly")
  for (i = 0; i < LINK_ITER_CNT; i++) {
    err = ll_iter(&head, (desc_iter[i].start < 0 ?
			  0 : &elems[desc_iter[i].start]),
		  t_iter, &desc_iter[i], desc_iter[i].flags);
    if (err != ((desc_iter[i].elem < 0) ? 0 : desc_iter[i].err)) {
      FAIL(TEST_NAME(ll_iter), 0, "ll_iter() returned unexpected error "
	   "code %lu", err);
      goto iter_fail; /* yeah, yeah, I know; this is just the easiest way */
		      /* to get out of this loop and avoid calling PASS	  */
		      /* that I can think of...				  */
    }
    if (desc_iter[i].visited != desc_iter[i].expected) {
      FAIL(TEST_NAME(ll_iter), 0, "ll_iter() failed to visit expected items; "
	   "visited 0x%04lx, expected 0x%04lx", desc_iter[i].visited,
	   desc_iter[i].expected);
      goto iter_fail;
    }
  }
  PASS(TEST_NAME(ll_iter), "ll_iter() calls successful");
 iter_fail:

  /* And, finally, ll_flush()... */
  TEST_DECL(t_linklists, ll_flush, "Test that ll_flush() works properly")
  for (i = 0; i < LINK_FLUSH_CNT; i++) {
    err = ll_flush(&head, t_iter, &desc_flush[i]);
    if (err != ((desc_flush[i].elem < 0) ? 0 : desc_flush[i].err)) {
      FAIL(TEST_NAME(ll_flush), 0, "ll_flush() returned unexpected error "
		     "code %lu", err);
      goto flush_fail; /* yeah, yeah, I know; this is just the easiest way */
		       /* to get out of this loop and avoid calling PASS   */
		       /* that I can think of...			   */
    }
    if (desc_flush[i].visited != desc_flush[i].expected) {
      FAIL(TEST_NAME(ll_flush), 0, "ll_flush() failed to visit expected "
	   "items; visited 0x%04lx, expected 0x%04lx", desc_flush[i].visited,
	   desc_iter[i].expected);
      goto flush_fail;
    }
    ChkOrder(ll_flush, order_flush[i], 0, goto flush_fail;);
  }
  PASS(TEST_NAME(ll_flush), "ll_flush() calls successful");
 flush_fail:

  return 0;
}

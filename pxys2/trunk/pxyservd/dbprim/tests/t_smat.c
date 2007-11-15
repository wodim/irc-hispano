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
** @(#)$Id: t_smat.c,v 1.3 2006/09/04 15:12:27 spale Exp $
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test-harness.h"

#include "dbprim.h"
#include "dbprim_int.h"

RCSTAG("@(#)$Id: t_smat.c,v 1.3 2006/09/04 15:12:27 spale Exp $");

TEST_PROG(t_smat, "Test sparse matrix functionality")

#define SMAT_HEAD_CNT	32

/* this is so complicated because SMAT_HEAD_CNT is likely the maximum number
 * of bits representable in an unsigned long, so we have to look out for
 * overflows.
 */
#define SMAT_HEAD_MASK	((((0x01lu << (SMAT_HEAD_CNT - 1)) - 1) << 1) | 0x01lu)

static smat_head_t rows[SMAT_HEAD_CNT];
static smat_head_t columns[SMAT_HEAD_CNT];
static struct assoc_s {
  unsigned long assoc[SMAT_HEAD_CNT];
} associations;

#define SMAT_ASSOC_CNT	(32 * 4)

/* Set all bits in the struct assoc_s to one... */
static void
set_ones(struct assoc_s *assoc)
{
  int i;

  for (i = 0; i < SMAT_HEAD_CNT; i++) /* walk through them all... */
    assoc->assoc[i] = SMAT_HEAD_MASK; /* set the bits */
}

/* Clear all bits in the struct assoc_s... */
static void
set_zeros(struct assoc_s *assoc)
{
  int i;

  for (i = 0; i < SMAT_HEAD_CNT; i++) /* walk through them all... */
    assoc->assoc[i] = 0; /* set the bits */
}

/* is everything zeroed? */
static int
check_zeros(struct assoc_s *assoc)
{
  int i;

  for (i = 0; i < SMAT_HEAD_CNT; i++) /* walk through them all... */
    if (assoc->assoc[i]) /* if an element's not zero... */
      return 0;

  return 1; /* everything is zeroed. */
}

/* macros to diddle individual bits in the association tracker */
#define set_assoc(ass, row, column) \
			((ass)->assoc[(row)] |=  (0x01lu << (column)))
#define clr_assoc(ass, row, column) \
			((ass)->assoc[(row)] &= ~(0x01lu << (column)))
#define chk_assoc(ass, row, column) \
			((ass)->assoc[(row)] &   (0x01lu << (column)))

int
main(int argc, char **argv)
{
  unsigned long err;
  smat_table_t tab; /* a sparse matrix table to play with... */
  smat_entry_t *se = 0;
  int i, rt, ct;
  struct assoc_s visited;

  srand(time(0)); /* seed random number generator */

  /* Let's test st_init first... */
  TEST(t_smat, st_init, "Test that st_init() may be called",
       (!(err = st_init(&tab, 0, 0, 0, 6)) &&
	st_modulus(&tab) == 7), FATAL(0),
       ("st_init() call successful"),
       ("st_init() call failed with error %lu modulus %lu", err,
	st_modulus(&tab)));

  /* Now try some sh_inits... */
  TEST_DECL(t_smat, sh_init, "Test that sh_init() may be called")
  for (i = 0; i < SMAT_HEAD_CNT; i++)
    if ((err = sh_init(&rows[i], SMAT_LOC_FIRST, (void *)i)) ||
	(err = sh_init(&columns[i], SMAT_LOC_SECOND, (void *)i)))
      FAIL(TEST_NAME(sh_init), FATAL(0), "sh_init() failed with error %lu",
	   err);
  PASS(TEST_NAME(sh_init), "sh_init() calls successful");

  /* OK, create some associations... */
  TEST_DECL(t_smat, st_add, "Test that st_add() adds elements to a sparse "
	    "matrix")
  set_zeros(&associations); /* clear the associations */
  for (i = 0; i < SMAT_ASSOC_CNT; i++) {
    rt = rand() % SMAT_HEAD_CNT; /* select row and column elements... */
    ct = rand() % SMAT_HEAD_CNT;

    if (chk_assoc(&associations, rt, ct))
      continue; /* only create unique associations */

    fprintf(stderr, "Creating association %d<->%d\n", rt, ct);

    /* add the association; note that ll_add()'s been tested, so
     * we don't have to test with other LINK_LOC_* values.  (Well,
     * perhaps we should, but it's tough to do it here...)
     */
    if ((err = st_add(&tab, 0, &rows[rt], LINK_LOC_HEAD, 0, &columns[ct],
		      LINK_LOC_HEAD, 0)))
      FAIL(TEST_NAME(st_add), FATAL(0), "st_add() failed with error %lu", err);

    set_assoc(&associations, rt, ct); /* remember the association */
  }
  PASS(TEST_NAME(st_add), "st_add() added associations properly");

  /* Can we look them up? */
  TEST_DECL(t_smat, st_find, "Test that st_find() can locate elements in a "
	    "sparse matrix")
  for ( \
    rt = 0, ct = 0, set_ones(&visited); \
    !check_zeros(&visited) && rt <= SMAT_HEAD_CNT && ct <= SMAT_HEAD_CNT; \
    clr_assoc(&visited, rt, ct), rt++ ) {

    if ( rt == SMAT_HEAD_CNT ) { rt = 0; ct++; }

    fprintf(stderr, "Looking up association %d<->%d\n", rt, ct);

    /* look up the association */
    err = st_find(&tab, &se, &rows[rt], &columns[ct]);

    /* check the error value... */
    if (err != (chk_assoc(&associations, rt, ct) ? 0 : DB_ERR_NOENTRY))
      FAIL(TEST_NAME(st_find), FATAL(0), "st_find() failed with error %lu",
	   err);
    if (!err && ((int)se_object(se, SMAT_LOC_FIRST) != rt ||
		 (int)se_object(se, SMAT_LOC_SECOND) != ct))
      FAIL(TEST_NAME(st_find), FATAL(0), "st_find() found wrong entry; "
	   "expected %d/%d, found %d/%d", rt, ct,
	   (int)se_object(se, SMAT_LOC_FIRST),
	   (int)se_object(se, SMAT_LOC_SECOND));

    fprintf(stderr, err ? "No such association\n" : "Found association\n");
  }
  PASS(TEST_NAME(st_find), "st_find() calls successful");

  /* Now try to remove some entries... */
  TEST_DECL(t_smat, st_remove, "Test that st_remove() can remove elements in "
	    "a sparse matrix")
  while (st_count(&tab) > SMAT_ASSOC_CNT / 2) {
    rt = rand() % SMAT_HEAD_CNT; /* select row and column elements... */
    ct = rand() % SMAT_HEAD_CNT;

    if (!chk_assoc(&associations, rt, ct))
      continue; /* only try to remove associations that exist */

    fprintf(stderr, "Looking up association %d<->%d for removal\n", rt, ct);

    if ((err = st_find(&tab, &se, &rows[rt], &columns[ct])) ||
	(err = st_remove(&tab, se)))
      FAIL(TEST_NAME(st_remove), FATAL(0), "st_find() failed to find "
	   "association %d/%d or st_remove() failed to remove it", rt, ct);

    fprintf(stderr, "Association removed\n");

    clr_assoc(&associations, rt, ct);
  }
  PASS(TEST_NAME(st_remove), "st_remove() calls successful");

  return 0;
}

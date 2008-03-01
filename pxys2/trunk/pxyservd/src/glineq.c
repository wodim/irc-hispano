/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#define RCSID "$Id: glineq.c,v 1.4 2006/09/09 18:54:02 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "glineq.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#if 0
#include <string.h>
#include <stdio.h>
#endif

struct g_data
  {
  struct in_addr addr;
  int hitcnt;
  char reason[248];
  };

struct g_elem
  {
  struct g_elem *prev;
  struct g_elem *next;
  struct g_data data;
  };

static struct g_elem *lhead = NULL;
static struct g_elem *ltail = NULL;

void
glineq_init(int max_queue_elems)
  {
    max_queue_elems = 0; /* parameter now ignored */

    if ( lhead != NULL ) glineq_finalize();
    lhead = NULL;
    ltail = NULL;
  }

void
glineq_finalize()
  {
    struct g_elem *next;
    struct g_elem *node;

    for(node=lhead; node!=NULL; node=next)
      {
      next = node->next;
      free(node);
      }
  }

int
glineq_empty()
  {
  return lhead == NULL;
  }

int
glineq_push(const struct g_args *args)
  {
    struct g_elem *node;

    node = malloc(sizeof(struct g_elem));
    assert(node!=NULL);

    node->data.addr = args->addr;
    node->data.hitcnt = args->hitcnt;
    snprintf(node->data.reason, sizeof(node->data.reason), "%s", args->reason);

    node->prev = NULL;
    node->next = lhead;

    if ( lhead != NULL )
      lhead->prev = node;

    lhead = node;

    if ( ltail == NULL )
      ltail = lhead;
  
  return 0;
  }

const struct g_args *
glineq_first()
  {
  static struct g_args args;
  struct g_args *argsp = NULL;

  if ( ltail != NULL )
    {
    args.addr   = ltail->data.addr;
    args.hitcnt = ltail->data.hitcnt;
    args.reason = ltail->data.reason;
    argsp = &args;
    }

  return argsp;
  }

void
glineq_pop()
  {
  struct g_elem *node = ltail;
  assert(ltail!=NULL);

  if ( ltail == lhead )
    {
    free(ltail);
    ltail = NULL;
    lhead = NULL;
    }
  else
    {
    ltail->prev->next = NULL;
    ltail = ltail->prev;
    free(node);
    }
  }

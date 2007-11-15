/* PEAK Library
 *
 * Copyright (c) 2004
 *      Stephane Thiell <mbuna@bugged.org>. All rights reserved.
 *
 * Specialized splay tree for peak's timers. Written with the big help of
 * GCC's internal splay-tree from Mark Mitchell. The algorithm's description
 * itself originated from Lewis, Harry R. and Denenberg, Larry.  Data
 * Structures and Their Algorithms.  Harper-Collins, Inc.  1991.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#define RCSID "$Id: timer_tree.c,v 1.2 2004/01/06 21:39:16 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "timer_tree.h"
#include "task_private.h"

/* Precondition for non recursive calls: *node != NULL
 */
__private_extern__ peak_timer
_peak_timer_tree_splay(peak_timer keytimer, peak_timer *node,
                       peak_timer *parent, peak_timer *grandparent)
  {
  peak_timer *next;
  peak_timer n;
  int cmp;
  
  n = *node;
  
  if (!n)
    return *parent;
  
  cmp = PEAK_TIMER_COMPARE(keytimer, n);
  switch (cmp)
    {
    case 0:
      next = NULL;
      break;
    case -1:
      next = &n->left;
      break;
    case 1:
    default:
      next = &n->right;
      break;
    }
  
  if (next)
    {
    /* Go down deep the tree. */
    n = _peak_timer_tree_splay(keytimer, next, node, parent);
    
    if (*node != n)
      return n;
    }
  
  if (!parent)
    return n; /* node is the root */
  
  /* If *parent is the root of the tree: */
  if (!grandparent)
    {
    if (n == (*parent)->left)
      {
      *node = n->right;
      n->right = *parent;
      }
    else
      {
      *node = n->left;
      n->left = *parent;
      }
    *parent = n;
    return n;
    }
  
  /* If n and *parent are children of the same side... */
  if (n == (*parent)->left && *parent == (*grandparent)->left)
    {
    peak_timer p = *parent;
    
    (*grandparent)->left = p->right;
    p->right = *grandparent;
    p->left = n->right;
    n->right = p;
    *grandparent = n;
    return n;
    }
  else if (n == (*parent)->right && *parent == (*grandparent)->right)
    {
    peak_timer p = *parent;
    
    (*grandparent)->right = p->left;
    p->left = *grandparent;
    p->right = n->left;
    n->left = p;
    *grandparent = n;
    return n;
    }
  
  /* Otherwise, they are not at the same side. */
  if (n == (*parent)->left)
    {
    (*parent)->left = n->right;
    n->right = *parent;
    (*grandparent)->right = n->left;
    n->left = *grandparent;
    *grandparent = n;
    return n;
    }
  else
    {
    (*parent)->right = n->left;
    n->left = *parent;
    (*grandparent)->left = n->right;
    n->right = *grandparent;
    *grandparent = n;
    return n;
    }
  }

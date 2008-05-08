/* Copyright (C) 2001-2003 Stephane Thiell <mbuna@undernet.org>
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
#define RCSID "$Id: tokenizer.c,v 1.2 2006/09/04 12:48:08 spale Exp $"

/* #define TOKENIZER_TEST */

#define MAXTOKSIZE 64

#ifndef TOKENIZER_TEST
# ifdef HAVE_CONFIG_H
#  include "config.h"
# endif
#endif

#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

#include <libbcl/memory.h>

static toktab ttab;

/* *MUST* be called before the first call of tokenize! */
void
tokenizer_init()
  {
  ttab.tok = (char **)tokenizer_malloc(sizeof(char *) * MAXTOKSIZE);
  ttab.size = 0;
  }

void
tokenizer_destroy()
  {
  tokenizer_free((char **)ttab.tok);
  ttab.size = -1;
  }

toktabptr
tokenize(char *s, int maxtok)
  {
  register int c, tog = 1, i = 0;
  char * p;
   
  /* Sanity checks */
  if (NULL == s)
    return NULL;
   
  if (maxtok > MAXTOKSIZE)
    maxtok = MAXTOKSIZE;
   
  /* Fill the array */
  for (p = s; i < maxtok && (c = (unsigned char)*p); p++)
    {
    if (c == ' ')  /* if (isspace(c)) */
      {
      tog = 1;
      *p = 0;
      }
    else if (tog)
      {
      tog = 0;
      ttab.tok[i++] = p;
      }
    }
   
  /* Set array size */
  ttab.size = i;
   
  return (i ? &ttab : NULL);
  }

char*
untokenize(toktabptr ttab, int start)
  {
  char *p, *ret = NULL;
  int i;
   
  for (i = start + 1; i < ttab->size; i++)
    {
    p = ttab->tok[i];
    while (*--p == '\0')
      *p = ' ';
    }
   
  if (ttab->size > start)
    ret = ttab->tok[start];
   
  return ret;
  }


#ifdef TOKENIZER_TEST
#define LINEINJECT_SIZE 50000000
int
main(void)
  {
  int i;
  toktabptr t;
  const char tpl[] = "AeFq0 P C0QwM :Hey abc def g h ijklmmmnop qq r s ttt uuuuuuuuuuuuuv vvvvv wwww x@yzzz\n";
  char buf[512];
   
  tokenizer_init();
   
  for (i=0; i<LINEINJECT_SIZE; i++)
    {
    strcpy(buf, tpl);
    tokenize(buf, 10);
    }
   
  strcpy(buf, tpl);
   
  printf("%s", buf);
   
  t = tokenize(buf, 10);
   
  for (i=0; i<t->size; i++)
    printf("tok[%d]=%s\n", i, t->tok[i]);
   
  return(0);
  }
#endif


/* tokenizer.c */

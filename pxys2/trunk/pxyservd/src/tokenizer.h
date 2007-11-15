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
#ifndef INCLUDED_TOKENIZER_H
#define INCLUDED_TOKENIZER_H

/* Zero copy tokenizer: one use only, max tokens not extensible at run time,
 * and modify the original buffer (must be mutable). You still want to use it?
 * Originally written for the pxys v1 IRC parser.
 */

struct toktab
  {
  char **tok; /* Array of char pointers to each word (token) of the string */
  int size;   /* Size of the array (the number of words) */
  };

typedef struct toktab toktab;
typedef struct toktab * toktabptr;

#define tokenizer_malloc memory_alloc
#define tokenizer_free memory_free

/* Initialize tokenizer */
extern void tokenizer_init();

/* Destroy tokenizer */
extern void tokenizer_destroy();

/* Do the tokenization from a mutable buffer, but no more than maxtok tokens.
 * Ownership of original buffer is momentarily transfered to the tokenizer, you
 * shouldn't modify it until the end of tokenizer operations.
 */
extern toktabptr tokenize(char *s, int maxtok);

/* Untokenize a toktabptr: restore original buffer content
 * Still zerocopy. Use the original buffer too
 */
extern char* untokenize(toktabptr ttab, int start);

#endif /* INCLUDED_TOKENIZER_H */

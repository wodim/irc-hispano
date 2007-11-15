/* This file is part of pxyservd (from pxys)
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
#define RCSID "$Id: match.c,v 1.1.1.1 2003/12/30 17:09:32 mbuna Exp $"

#include "irc_chattr.h"

/*
 * Compare if a given string (name) matches the given
 * mask (which can contain wild cards: '*' - match any
 * number of chars, '?' - match any single character.
 *
 * return  0, if match
 *         1, if no match
 *
 *  Originally by Douglas A Lewis (dalewis@acsu.buffalo.edu)
 *  Rewritten by Timothy Vogelsang (netski), net@astrolink.org
 */

int
match(const char *mask, const char *name)
  {
  const char *m = mask, *n = name;
  const char *m_tmp = mask, *n_tmp = name;
  int wild = 0;

  for (;;)
    {
    if (*m == '*') {
      while (*m == '*')  /* clean up any additional wildcards */
        m++;

      m_tmp = m;
      n_tmp = n;
      wild = 1;
      }
    if (*m == '\\')  /* next wildcard is disregarded */
      m++;

    if (!*m) {
      if (!*n)
        return 0;  /* match */

      for (m--; (m > mask) && (*m == '?'); m--)
      ;

      if (*m == '*' && (m > mask))
        return 0;  /* match */

      if (!wild)
        return 1;

      m = m_tmp;
      n = ++n_tmp;
      }
    else if (!*n) {
      while (*m == '*')  /* clean up any additional wildcards */
        m++;

      return (*m != 0);
      }
    if (ToLower(*m) != ToLower(*n) && *m != '?') {
      if (!wild)
        return 1;  /* failure! */

      m = m_tmp;
      n = ++n_tmp;
      }
    else {
      if (*m)
        m++;
      if (*n)
        n++;
      }
    }

  return 1;  /* no match! */
  }

/*
 * collapse()
 * Collapse a pattern string into minimal components.
 * This particular version is "in place", so that it changes the pattern
 * which is to be reduced to a "minimal" size.
 *
 * (C) Carlo Wood - 6 Oct 1998
 * Speedup rewrite by Andrea Cocito, December 1998.
 * Note that this new optimized alghoritm can *only* work in place.
 */

char *
collapse(char *mask)
  {
  int star = 0;
  char *m = mask;
  char *b;

  if (m)
    {
    do
      {
      if ((*m == '*') && ((m[1] == '*') || (m[1] == '?')))
        {
        b = m;
        do
          {
          if (*m == '*')
            star = 1;
          else
            {
            if (star && (*m != '?'))
              {
              *b++ = '*';
              star = 0;
              }
            *b++ = *m;
            if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
              *b++ = *++m;
            }
          }
        while (*m++);
        break;
        }
      else
        {
        if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
          m++;
        }
      }
    while (*m++);
    }
  return mask;
  }

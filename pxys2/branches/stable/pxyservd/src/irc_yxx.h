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
 * $Id: irc_yxx.h,v 1.1.1.1 2003/12/30 17:09:32 mbuna Exp $
 */
#ifndef INCLUDED_IRC_YXX_H
#define INCLUDED_IRC_YXX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sys/types.h>
#include <peak/stdint.h>

struct yxx {
  int server_n;
  int client_n;
  int bogus;
  };

struct stryxx {
  char server_sn[3];
  char client_sn[4];
  size_t length;
  };

typedef struct yxx      yxx;
typedef struct stryxx   stryxx;

extern yxx yxx_to_int(const char *numeric);
extern void yxx_cut(const char *numeric, stryxx * outfnum);


/* Methods for packing/unpacking a yxx numeric in a 32 bits integer
 */

static inline uint32_t
yxx_pack_int(int server_n, int client_n)
  {
  return (uint32_t)(server_n << 18) + client_n;
  }

static inline uint32_t
yxx_pack(yxx x)
  {
  return yxx_pack_int(x.server_n, x.client_n);
  }

static inline yxx
yxx_unpack(uint32_t n)
  {
  yxx x;
  x.server_n = n >> 18;
  x.client_n = n & 0x0003ffff;
  x.bogus = 0;
  return x;
  }


#endif /* INCLUDED_IRC_YXX_H */

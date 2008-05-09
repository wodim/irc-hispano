/* IRC - Internet Relay Chat, ircd/channel.c
 * Copyright (C) 1996 Carlo Wood
 *
 * [Taken from Undernet ircd]
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
#ifndef INCLUDED_IRC_NUMNICKS_H
#define INCLUDED_IRC_NUMNICKS_H

#if 1
#define EXTENDED_NUMERICS
#else
#undef EXTENDED_NUMERICS
#endif

#define NUMNICKLOG 6
#define NUMNICKMAXCHAR 'z'      /* See convert2n[] */
#define NUMNICKBASE 64          /* (2 << NUMNICKLOG) */
#define NUMNICKMASK 63          /* (NUMNICKBASE-1) */
#define NN_MAX_SERVER 4096      /* (NUMNICKBASE * NUMNICKBASE) */

#ifdef EXTENDED_NUMERICS
#define NN_MAX_CLIENT 262144    /* NUMNICKBASE ^ 3 */
#else
#define NN_MAX_CLIENT 4096      /* (NUMNICKBASE * NUMNICKBASE) */
#endif

const char convert2y[64];
const char fake_convert2y[64];
const unsigned int convert2n[256];


extern unsigned int base64toint(const char *s);
extern const char * inttobase64(char *buf, unsigned int v, unsigned int count);
extern const char * fake_inttobase64(char *buf, unsigned int v, unsigned int count);

#endif /* INCLUDED_IRC_NUMNICKS_H */

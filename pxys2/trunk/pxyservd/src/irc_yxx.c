/* 
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
#define RCSID "$Id: irc_yxx.c,v 1.1.1.1 2003/12/30 17:09:32 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_yxx.h"
#include <string.h>

#include "irc_numnicks.h"

#define INVALID_NUMERIC			-1
#define SERVER_ONLY_NUMERIC		-2

/* Adaptation of nextie <future@break.net> yxx.cxx methods.
 * It's better in C++ :)
 */

yxx
yxx_to_int(const char *numeric)
  {
  yxx out_yxx;   /* returned yxx struct */
  stryxx fnum;   /* formated numeric */
   
  /* Default */
  out_yxx.server_n = out_yxx.client_n = INVALID_NUMERIC;
  out_yxx.bogus = 0;
  
  /* Format numeric string */
  yxx_cut(numeric, &fnum);
   
  /* Fill yxx struct */
  switch(fnum.length)
    {
    case 1:
    case 2:
      out_yxx.server_n = base64toint(fnum.server_sn);
      out_yxx.client_n = SERVER_ONLY_NUMERIC;
      break;
    case 3:
      out_yxx.server_n = base64toint(fnum.server_sn);
      out_yxx.client_n = base64toint(fnum.client_sn);
      break;
    case 4:  /* bogus! */
      out_yxx.server_n = base64toint(fnum.server_sn);
      out_yxx.client_n = base64toint(fnum.client_sn);
      out_yxx.bogus = 1;
      break;
    case 5:
      out_yxx.server_n = base64toint(fnum.server_sn);
      out_yxx.client_n = base64toint(fnum.client_sn);
      /* out_yxx.extended = 1; */
      break;
    }
   
  return out_yxx;
  }

void
yxx_cut(const char *numeric, stryxx * outfnum)
  {
  outfnum->length = strlen(numeric);
   
  switch(outfnum->length)
    {
    case 1: /* Y */
      outfnum->server_sn[0] = numeric[0];
      outfnum->server_sn[1] = 0;
      break;
    case 2: /* YY */
      outfnum->server_sn[0] = numeric[0];
      outfnum->server_sn[1] = numeric[1];
      outfnum->server_sn[2] = 0;
      break;
    case 3: /* YXX */
      outfnum->server_sn[0] = numeric[0];
      outfnum->server_sn[1] = 0;
      outfnum->client_sn[0] = numeric[1];
      outfnum->client_sn[1] = numeric[2];
      outfnum->client_sn[2] = 0;
      break;
    case 4: /* YYXX - Bogus */
      outfnum->server_sn[0] = numeric[0];
      outfnum->server_sn[1] = numeric[1];
      outfnum->server_sn[2] = 0;
      outfnum->client_sn[0] = numeric[2];
      outfnum->client_sn[1] = numeric[3];
      outfnum->client_sn[2] = 0;
      break;
    case 5: /* YYXXX */
      outfnum->server_sn[0] = numeric[0];
      outfnum->server_sn[1] = numeric[1];
      outfnum->server_sn[2] = 0;
      outfnum->client_sn[0] = numeric[2];
      outfnum->client_sn[1] = numeric[3];
      outfnum->client_sn[2] = numeric[4];
      outfnum->client_sn[3] = 0;
      break;
    }
  }


/* yxx.c */

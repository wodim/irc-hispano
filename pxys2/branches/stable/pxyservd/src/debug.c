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
#define RCSID "$Id: debug.c,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $"

#include "debug.h"

#ifdef DEBUG

#ifndef DEBUG_FILE
#define DEBUG_FILE "debug.log"
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <peak/peak.h>

static FILE *fp;
static int debug_level;
static int output_stdout;

static void vprint_debug(int level, const char *pattern, va_list vl);

void
debug_init(int max_level, int out_stdout)
  {
  time_t t;
  char *ct;

  debug_level = max_level;
  output_stdout = out_stdout;

  if ((fp = fopen(DEBUG_FILE, "a")) == NULL)
    exit(1);

  t = peak_time();
  ct = ctime(&t);
  ct[strlen(ct) - 1] = '\0';
  debug(DL_BASIC, "[%s] Debug service started", ct);
  }

void
debug_dispose(void)
  {
  time_t t = peak_time();
  debug(DL_BASIC, "[%s] Stopping debug service", ctime(&t));
  fclose(fp);
  }

void
debug(int level, const char *fmt, ...)
  {
  va_list vl;
  va_start(vl, fmt);
  vprint_debug(level, fmt, vl);
  va_end(vl);
  }

static void
vprint_debug(int level, const char *fmt, va_list vl)
  {
  char buffer[1024];
    
  if (level <= debug_level)
    {
    vsnprintf(buffer, sizeof(buffer), fmt, vl);
    fprintf(fp, "%s\n", buffer);
    if (output_stdout)
      fprintf(stdout, "%s\n", buffer);
    }
  }

#endif /* DEBUG */

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
 * $Id: debug.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_DEBUG_H_
#define INCLUDED_DEBUG_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>

enum {
    DL_BASIC=0,
    DL_IRCMSG,
    DL_MAX
    };

#ifdef DEBUG
#define Debug(x) debug x
#define DebugInit(level, stdo) debug_init(level, stdo)
#define DebugDispose() debug_dispose()
#else
#define Debug(x)
#define DebugInit(level, stdo)
#define DebugDispose()
#endif

#ifdef DEBUG
extern void debug_init(int max_level, int out_stdout);
extern void debug_dispose(void);
extern void debug(int level, const char *fmt, ...);
#endif

#endif /* INCLUDED_DEBUG_H_ */

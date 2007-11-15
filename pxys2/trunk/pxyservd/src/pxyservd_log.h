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
 * $Id: pxyservd_log.h,v 1.2 2004/05/02 12:39:10 mbuna Exp $
 */
#ifndef INCLUDED_PXYSERVD_LOG_H
#define INCLUDED_PXYSERVD_LOG_H

#include <stdarg.h>
#include <time.h>

enum LogIDs
  {
  LOGID_CURRENT=0,   /* "Current" proxy logging file */
  LOGID_SYSTEM,      /* pxyservd system logging */
  LOGID_GLINES,      /* Raw glines output logging */
  LOGID_OPERCMDS,    /* Opers commands logging */
  LOGID_STATS,       /* Misc. statistics output */
  LOGID_UNREACH,     /* No route to host logging */
  LOGID_CLIENTS      /* Clients connection debug */
  };

#define MAX_LOGID LOGID_CLIENTS

/* Proto' */

extern void  log_init(void);
extern void log_finalize(void);

/* Generic */
extern void log_write(enum LogIDs id, const char *format, ...);
extern void log_vwrite(enum LogIDs id, const char *format, va_list vl);

/* Generic without TS */
extern void log_write_raw(enum LogIDs id, const char *format, ...);
extern void log_vwrite_raw(enum LogIDs id, const char *format, va_list vl);

/* Convenience */
extern void log_system(const char *format, ...);

/* Log files rotation */
extern int  log_rotate_check();
extern void log_rotate_sync();
extern int  log_rotate_cut();
extern int log_rotate_hist(int resync);

#endif /* INCLUDED_PXYSERVD_LOG_H */

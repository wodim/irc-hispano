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
 * $Id: evreg.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_EVREG_H
#define INCLUDED_EVREG_H

#include "irc_struct.h"
#include <peak/peak.h>

#define EVREG_FLAG_NEWPROXY 1
#define EVREG_FLAG_CACHED   2
#define EVREG_FLAG_OPER     4
#define EVREG_FLAG_PXYSCAND 8

extern void evreg_init(void);
extern void evreg_finalize(void);
extern uint32_t evreg_mod(struct Client *cptr, uint32_t set, uint32_t clear);
extern void evreg_exit(struct Client *cptr);
extern void evreg_broadcast(uint32_t filter, const char *fmt, ...);
extern void evreg_show(const char *dst);


#endif /* INCLUDED_EVREG_H */

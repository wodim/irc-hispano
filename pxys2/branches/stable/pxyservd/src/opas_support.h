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
 * $Id: opas_support.h,v 1.1.1.1 2003/12/30 17:09:32 mbuna Exp $
 */
#ifndef INCLUDED_OPAS_SUPPORT_H
#define INCLUDED_OPAS_SUPPORT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <peak/stdint.h>

/* OPAS (Open Proxy Ascertainment Service) support module.
 */

extern int opas_support_init(void);
extern void opas_support_finalize(void);
extern int opas_support_is_ready();
extern int opas_support_query(const struct in_addr *addr, uint32_t user_data);
extern int opas_support_query6(const struct in6_addr *addr,
                               uint32_t user_data);

extern void opas_support_send_msg_user(void *data, size_t length,
                                       uint32_t user_data);

#endif /* INCLUDED_OPAS_SUPPORT_H */

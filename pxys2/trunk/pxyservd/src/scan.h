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
 * $Id: scan.h,v 1.2 2004/01/12 12:31:03 mbuna Exp $
 */
#ifndef INCLUDED_SCAN_H
#define INCLUDED_SCAN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_struct.h"
#include <peak/stdint.h>

extern void scan_init(void);
extern void scan_finalize(void);

extern int scan_check_noscan(const struct Client *cptr);
extern int scan_check_noscan_server(struct Server *sptr);

extern int scan_client_get_count(void);
extern int scan_client_get_unscannable_count(void);

extern void scan_client_remove(struct Client *cptr);
extern void scan_new_user(struct Client *cptr);
extern void scan_start(struct Client *cptr); /* for recheck command only */

extern int scan_send_command(struct Client *cptr, uint32_t sig, uint32_t cmd,
                             const void *data, size_t length);

extern int scan_send_simple_command(struct Client *cptr, uint32_t sig,
                                    uint32_t cmd);

/* "callbacks" */
extern void scan_reply_noproxy(const struct in_addr *addrp, uint32_t ud,
                               int cached);
extern void scan_reply_proxy(const struct in_addr *addrp, uint32_t ud,
                             int cached, int proxy_type, uint16_t proxy_port,
                              const char *proxy_descr);
extern void scan_reply_dnsbl(const struct in_addr *addrp, uint32_t ud,
                             int cached, int dnsbl_type, const char *server,
                             const char *dnsbl_descr);
extern void scan_reply_error(const struct in_addr *addrp, uint32_t ud,
                             uint32_t error);
extern void scan_reply6_noproxy(const struct in6_addr *addrp, uint32_t ud,
                                int cached);
extern void scan_reply6_proxy(const struct in6_addr *addrp, uint32_t ud,
                              int cached, int proxy_type, uint16_t proxy_port,
                              const char *proxy_descr);
extern void scan_reply6_dnsbl(const struct in6_addr *addrp, uint32_t ud,
                              int cached, int dnsbl_type, const char *server,
                              const char *dnsbl_descr);
extern void scan_reply6_error(const struct in6_addr *addrp, uint32_t ud,
                              uint32_t error);
extern void scan_reply_command(void *data, size_t length, uint32_t ud);

#endif /* INCLUDED_SCAN_H */

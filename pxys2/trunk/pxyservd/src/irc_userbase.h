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
 * $Id: irc_userbase.h,v 1.1.1.1 2003/12/30 17:09:32 mbuna Exp $
 */
#ifndef INCLUDED_IRC_USERBASE_H
#define INCLUDED_IRC_USERBASE_H

#include "irc_struct.h"

extern smat_head_t sUSmatHead;

extern void            irc_userbase_init(void);
extern void            irc_userbase_finalize(void);

extern int             irc_userbase_get_count(void);
extern size_t          irc_userbase_get_hash_table_size();
extern int             irc_userbase_proxycount(const struct in_addr *addr);
extern unsigned int    irc_userbase_count_channels(struct Client *cptr);

extern void            irc_userbase_add(const char *nick, const char *user,
                                        time_t firsttime,  const char *mode,
                                        unsigned int flags, ClientAddr addr,
                                        const char *numnick);
extern void            irc_userbase_nick_change(const char *numnick,
                                                const char *new_nick);
extern void            irc_userbase_remove(const char *numnick);
extern void            irc_userbase_purgeserv(struct Server *sptr);
extern struct Client * irc_userbase_get_by_nick(const char *nick);


#endif /* INCLUDED_IRC_USERBASE_H */

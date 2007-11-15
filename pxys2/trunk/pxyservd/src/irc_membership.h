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
 * $Id: irc_membership.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_IRC_MEMBERSHIP_H
#define INCLUDED_IRC_MEMBERSHIP_H

#include <sys/types.h>

#define MEMBERSHIP_LOC_CHANNEL SMAT_LOC_SECOND
#define MEMBERSHIP_LOC_CLIENT  SMAT_LOC_FIRST

struct Channel;
struct Client;

typedef void (*irc_membership_iter_func)(struct Channel *channel,
                                         struct Client *client,
                                         void *ud);

extern void   irc_membership_init(void);
extern void   irc_membership_finalize(void);
extern int    irc_membership_get_count(void);
extern size_t irc_membership_get_smat_size(void);
extern void   irc_membership_add(struct Channel *channel,
                                 struct Client *client);
extern int    irc_membership_exists(struct Channel *channel,
                                    struct Client *client);
extern void   irc_membership_remove(struct Channel *channel,
                                    struct Client *client);
extern void   irc_membership_remove_client(struct Client *client);
extern void   irc_membership_iterate(irc_membership_iter_func iter, void *ud);

#endif /* INCLUDED_IRC_MEMBERSHIP_H */

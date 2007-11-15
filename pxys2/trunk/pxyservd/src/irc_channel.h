/* Copyright (C) 2003, 2004 Stephane Thiell
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
 * $Id: irc_channel.h,v 1.2 2004/01/10 18:15:36 mbuna Exp $
 */
#ifndef INCLUDED_IRC_CHANNEL_H
#define INCLUDED_IRC_CHANNEL_H

#include <time.h>
#include <sys/types.h>

struct Channel;

#define IsGlobalChannel(name)   (*(name) == '#')
#define IsLocalChannel(name)    (*(name) == '&')
#define IsModelessChannel(name) (*(name) == '+')
#define IsChannelName(name)     (IsGlobalChannel(name) || \
                                IsModelessChannel(name) || \
                                IsLocalChannel(name))

extern void             irc_channel_init(void);
extern void             irc_channel_finalize(void);
extern struct Channel * irc_channel_get(const char *chname);
extern int              irc_channel_get_count();
extern int              irc_channel_get_hash_table_count();
extern size_t           irc_channel_get_hash_table_size();
extern struct Channel * irc_channel_create(const char *chname,
                                           time_t creationtime);
extern void             irc_channel_chuck(struct Channel *channel);
extern int              irc_channel_get_chucked_count();


#endif /* INCLUDED_IRC_CHANNEL_H */

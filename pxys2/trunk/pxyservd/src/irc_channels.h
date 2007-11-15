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
 * $Id: irc_channels.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_IRC_CHANNELS_H
#define INCLUDED_IRC_CHANNELS_H

#error deprecated

#define IsGlobalChannel(name)   (*(name) == '#')
#define IsLocalChannel(name)    (*(name) == '&')
#define IsModelessChannel(name) (*(name) == '+')
#define IsChannelName(name)     (IsGlobalChannel(name) || \
                                IsModelessChannel(name) || IsLocalChannel(name))

#endif /* INCLUDED_IRC_CHANNELS_H */

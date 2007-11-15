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
 * $Id: irc_cmd.h,v 1.4 2004/01/12 12:31:03 mbuna Exp $
 */
#ifndef INCLUDED_IRC_CMD_H
#define INCLUDED_IRC_CMD_H

#include "tokenizer.h"

struct Client;

typedef void (*cmd_func)(struct Client *cptr, toktabptr ttab);

#define DEF_CMD(n) \
    extern void cmd_##n(struct Client *cptr, toktabptr ttab)

DEF_CMD(evreg);
DEF_CMD(evshow);
DEF_CMD(info);
DEF_CMD(inv);
DEF_CMD(grem);
DEF_CMD(help);
DEF_CMD(noscan);
DEF_CMD(proxytop);
DEF_CMD(pxstats);
DEF_CMD(recheck);
DEF_CMD(servers);
DEF_CMD(showcmds);
DEF_CMD(stats);
DEF_CMD(status);
DEF_CMD(whois);

#endif /* INCLUDED_IRC_CMD_H */

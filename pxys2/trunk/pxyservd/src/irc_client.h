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
 */
#ifndef INCLUDED_IRC_CLIENT_H
#define INCLUDED_IRC_CLIENT_H

#include "irc_network.h"
#include "tokenizer.h"

#define MYCLIENT_NUM        0
#define MYCLIENT_NUM64      "AAA"

#define MYCLIENT_PTR        (gMe.cliT[MYCLIENT_NUM])
#define MYCLIENT_EXISTS()   (gMe.cliT[MYCLIENT_NUM] != NULL)

extern void irc_client_init(void);
extern void irc_client_register(void);
extern void irc_client_unregister();
extern void irc_client_burst(void);
extern void irc_client_handle_private(toktabptr ttab);

extern char *get_host(struct Client *client, const char *yxx_dest);
extern char *get_ip(struct Client *client, const char *yxx_dest);

#endif /* INCLUDED_IRC_CLIENT_H */

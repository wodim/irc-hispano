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
 * $Id: irc_network.h,v 1.2 2004/01/17 17:48:22 mbuna Exp $
 */
#ifndef INCLUDED_IRC_NETWORK_H
#define INCLUDED_IRC_NETWORK_H

#include "irc_struct.h"
#include "irc_yxx.h"

typedef void (*irc_network_map_servers_callback)(struct Server *sptr,
                                                 void *ctx);

typedef struct Client* (*irc_network_map_clients_callback)(struct Client *cptr,
                                                           void *ctx);

/* Must be called once to initialize this module.
 */
extern void            irc_network_init(void);

extern int             irc_network_get_server_count(void);
extern int             irc_network_get_remote_server_count(void);
extern size_t          irc_network_get_hash_table_size(void);
extern int             irc_network_get_server_count_flagged(unsigned int f);
extern struct Server * irc_network_get_server(unsigned int nserv);
extern int             irc_network_server_is_bursting(struct Server *sptr);
extern int             irc_network_is_my_downlink(unsigned int nserv);

extern void            irc_network_add_server(unsigned int nserv_up,
                                              const char *name,
                                              unsigned int nserv,
                                              unsigned int ncap,
                                              int bursting,
                                              int hub,
                                              int service);

extern void            irc_network_ack_end_of_burst(unsigned int nserv);
extern void            irc_network_squit_downlink();
extern void            irc_network_remove_server_name(const char *name);
extern struct Server * irc_network_remove_server(struct Server *sptr);

extern void            irc_network_add_client(yxx target, struct Client *cptr);
extern void            irc_network_remove_client(yxx target);
extern void            irc_network_remove_clients(unsigned int nserv);
extern struct Client * irc_network_find_client(yxx target);

extern void irc_network_map_servers(irc_network_map_servers_callback cb,
                                    void *ctx);
extern void irc_network_map_clients(struct Server *sptr,
                                    irc_network_map_clients_callback cb,
                                    void *ctx);
extern void irc_network_set_servers_records();

extern struct Server gMe;


#endif /* INCLUDED_IRC_NETWORK_H */

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
 */
#ifndef INCLUDED_PARSER_H_
#define INCLUDED_PARSER_H_

#include <sys/types.h>

/* Generic API for the IRC service parser
 */


/* Initialize the parser. Call once.
 */
extern void irc_parser_init(void);

/* Send a line from the network to the parser
 * The line must be mutable and null-terminated
 */
extern void irc_parser_parse_line(char *msg);

#if 0
/* Send a buffer from the network to the parser
 * May be used if an ircd use a binary server to server protocol
 */
extern void irc_parser_parse_buffer(const char *buffer, size_t bufsize);
#endif


#endif /* INCLUDED_PARSER_H_ */

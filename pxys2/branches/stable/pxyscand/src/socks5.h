// Copyright (C) 2003 Stephane Thiell
//
// This file is part of pxyscand (from pxys)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// SOCKS Protocol Version 5 constants (RFC1928).
//
// $Id: socks5.h,v 1.2 2006/11/24 23:23:01 spale Exp $
//
#ifndef INCLUDED_SOCKS5_H_
#define INCLUDED_SOCKS5_H_

#define	S5_VN               05          /* Socks version 5 */

/* Reply from method request */
#define	S5_R_METHOD_NOAUTH  0x00        /* NO AUTHENTICATION REQUIRED */
#define	S5_R_METHOD_GSSAPI  0x01        /* GSSAPI */
#define	S5_R_METHOD_USERPW  0x02        /* USERNAME/PASSWORD */
#define	S5_R_METHOD_NOTACC  0xff        /* NO ACCEPTABLE METHODS */

/* SOCKS request: CMD */
#define	S5_CMD_CONNECT      0x01        /* CONNECT */
#define	S5_CMD_BIND         0x02        /* BIND */
#define	S5_CMD_UDP_ASSOC    0x03        /* UDP ASSOCIATE */

/* SOCKS request: address type of following address */
#define	S5_ATYP_IPV4        0x01        /* IP V4 address */
#define	S5_ATYP_DOMAIN      0x03        /* DOMAINNAME */
#define	S5_ATYP_IPV6        0x04        /* IP V6 address */

/* Reply field */
#define	S5_R_SUCCEEDED      0x00        /* Succeeded */
#define	S5_R_FAILURE        0x01        /* General SOCKS server failure */
#define	S5_R_NO_RULE        0x02        /* Connection not allowed by ruleset */
#define	S5_R_NET_UNREACH    0x03        /* Network unreachable */
#define	S5_R_HOST_UNREACH   0x04        /* Host unreachable */
#define	S5_R_REFUSED        0x05        /* Connection refused */
#define	S5_R_TTL_EXPIRED    0x06        /* TTL expired */
#define	S5_R_UNSUPPORTED    0x07        /* Command not supported */
#define	S5_R_ATYP_UNSUPP    0x08        /* Address type not supported */

struct socks5
  {
  unsigned char vn;
  unsigned char nbr_of_auth;
  unsigned char auth1;
  };

struct socks5_response
  {
  unsigned char vn;
  unsigned char method;
  };

#endif // INCLUDED_SOCKS5_H_

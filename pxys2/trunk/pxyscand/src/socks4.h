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
// SOCKS Protocol Version 4 constants.
// http://www.socks.nec.com/protocol/socks4.protocol
//
// $Id: socks4.h,v 1.1.1.1 2003/12/30 17:09:02 mbuna Exp $
//
#ifndef INCLUDED_SOCKS4_H_
#define INCLUDED_SOCKS4_H_

#define	S4_VN               04    /* Socks version 4 */

/* SOCKS request */
#define	S4_CMD_CONNECT      0x01  /* CONNECT */
#define	S4_CMD_BIND         0x02  /* BIND */

/* Reply field */
#define S4_R_VNCODE         0x00  /* Reply VN */
#define	S4_REQ_GRANTED      0x5a  /* Request granted */
#define	S4_REQ_REJECTED     0x5b  /* Request rejected or failed */
#define	S4_REQ_CANNOTIDENT  0x5c  /* Request rejected because SOCKS server
                                   * cannot connect to identd on the client
                                   */
#define	S4_REQ_NOTSAMEIDENT 0x5d  /* Request rejected because the client
                                   * program and identd report different
                                   * user-ids
                                   */

struct socks4
  {
  unsigned char vn;
  unsigned char cd;
  unsigned char dstport[2];
  unsigned char dstip[4];
  /* char userid[256]; */
  unsigned char nullfield;
  };

struct socks4_response
  {
  unsigned char vn;
  unsigned char cd;
  unsigned char dstport[2];
  unsigned char dstip[4];
  };

#endif // INCLUDED_SOCKS4_H_

/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of libopas (from pxys)
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
 * $Id: opas_msg.h,v 1.1.1.1 2003/12/30 17:27:47 mbuna Exp $
 */
#ifndef INCLUDED_OPAS_OPAS_MSG_H
#define INCLUDED_OPAS_OPAS_MSG_H

#include <opas/opas_stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>


/* fixed-length OPAS messages structs
 */
struct opas_msg_query
  {
  uint32_t head;
  uint32_t user_data;
  struct in_addr addr;
  };

struct opas_msg_query6
  {
  uint32_t head;
  uint32_t user_data;
  struct in6_addr addr;
  };

struct opas_msg_reply_error
  {
  uint32_t head;
  uint32_t user_data;
  struct in_addr addr;
  uint32_t error;
  };

struct opas_msg_reply6_error
  {
  uint32_t head;
  uint32_t user_data;
  struct in6_addr addr;
  uint32_t error;
  };



struct opas_msg_reply_proxy
  {
  uint32_t head;
  uint32_t user_data;
  struct in_addr addr;
  uint32_t timestamp;
  uint16_t proxy_type;
  uint16_t proxy_port;
  char proxy_descr[1];
  };

struct opas_msg_reply6_proxy
  {
  uint32_t head;
  uint32_t user_data;
  struct in6_addr addr;
  uint32_t timestamp;
  uint16_t proxy_type;
  uint16_t proxy_port;
  char proxy_descr[1];
  };

struct opas_msg_user_header
  {
  uint32_t head;
  uint32_t user_data;
  uint32_t data_length;
  };

#define OPAS_MSG_MINSIZE  (12)
#define OPAS_MSG_MAXSIZE  (4 + 255)
#define OPAS_UMSG_MAXSIZE (65536)

#endif /* INCLUDED_OPAS_OPAS_MSG_H */

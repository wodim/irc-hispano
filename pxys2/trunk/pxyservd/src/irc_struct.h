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
 * $Id: irc_struct.h,v 1.3 2004/01/12 12:31:03 mbuna Exp $
 */
#ifndef INCLUDED_IRC_STRUCT_H
#define INCLUDED_IRC_STRUCT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

#include <dbprim/dbprim.h>

#ifndef NICKLEN
#define NICKLEN 9
#endif

#ifndef USERLEN
#define USERLEN 10
#endif

#ifndef HOSTLEN
#define HOSTLEN 63
#endif

enum
  {
  CLIENT_FLAG_NONE      = 0,
  CLIENT_FLAG_IDENT     = 1 << 0,
  CLIENT_FLAG_IPV6      = 1 << 1, /* To recognize ipv6 clients */
  CLIENT_FLAG_OPER      = 1 << 2, /* Is global IRC operator */
  CLIENT_FLAG_ACCOUNT   = 1 << 3, /* Is authenticated (cservice) - for info*/
  CLIENT_FLAG_EVREG     = 1 << 4, /* As special event registered */
  CLIENT_FLAG_SCANNING  = 1 << 5, /* Client being scanned */
  CLIENT_FLAG_SCANFAIL  = 1 << 6, /* Client IP scan failed (eg. unreachable) */
  CLIENT_FLAG_COMMAND   = 1 << 7, /* Pending OPAS command for this client */
  CLIENT_FLAG_GREM      = 1 << 8, /* Flag to distinguish GREM and RECHECK */
  CLIENT_FLAG_HIDDEN    = 1 << 9  /* +x, host oculto */
#ifdef IRC_HISPANO
, CLIENT_FLAG_HELPER    = 1 << 10, /* +h Helper */
  CLIENT_FLAG_HDDVIEWER = 1 << 11, /* +X Ve ips */
#endif
  };

enum
  {
  SERVER_FLAG_NONE      = 0,
  SERVER_FLAG_BURSTING  = 1 << 0, /* Server is bursting on the network */
  SERVER_FLAG_HUB       = 1 << 1, /* +h server mode of ircu */
  SERVER_FLAG_SERVICE   = 1 << 2, /* +s server mode of ircu */
  SERVER_FLAG_NOSCAN    = 1 << 3, /* Configuration forced noscan */
  SERVER_FLAG_MAP       = 1 << 4  /* Help flag for the servers command */
  };

enum
  {
  CHANNEL_FLAG_NONE     = 0,
  CHANNEL_FLAG_CHUCKED  = 1 << 16
  };

union ClientAddr
  {
  struct in_addr ip4;
#ifdef ENABLE_IPV6
  struct in6_addr ip6;
#endif
  };

typedef union ClientAddr ClientAddr;

struct Client
  {
  struct Client *next;          /* Client linkage */
  struct Client **prev_p;       /* Client linkage */
  struct Client *scan_next;     /* ScanQ linkage */
  struct Client **scan_prev_p;  /* ScanQ linkage */
  
  hash_entry_t hentry;
  smat_head_t mhead;
  
  char nick[NICKLEN+1];
  char user[USERLEN+1];
  char host[HOSTLEN+1];
  unsigned int nserv;
  unsigned int nnick;
  unsigned int flags;    /* operator? ident? ipv6?... */
  ClientAddr addr;       /* Real IP */
  
  time_t firsttime;      /* time client was created */
  time_t scan_timestamp; /* Last scanned date, or if FLAG_SCANNING is set,
                          * scan start date.
                          */
  };

struct Server
  {
  struct Server *next;       /* Linked list */
  struct Server **prev_p;
  struct Server *uplink;     /* Uplink (server one closer to me) */
  hash_entry_t hentry;
  struct Client *cliHead;    /* Clients linked list head */
  struct Client **cliT;      /* Clients table */
  
  char name[HOSTLEN+1];      /* Server name */
  
  unsigned int nserv;        /* Server numeric */
  char yy[2+1];              /* ...and its base64 representation */
  unsigned int nn_mask;      /* Numeric nick mask */
  unsigned int clients;      /* Number of clients */
  unsigned int maxclients;   /* Max clients record */
  time_t maxclients_time;    /* Max clients record date */
  unsigned int flags;        /* Bursting, NoScan, etc. */
  time_t timestamp;
  
  unsigned int proxy_count;  /* Proxies found on this server */
  time_t proxy_since;        /* Last proxy counter reset time */
  };

struct Channel
  {
  hash_entry_t hentry;
  smat_head_t mhead;
  
  unsigned int flags;
  unsigned int mode;
  time_t creationtime;
  char chname[1];
  };



#endif /* INCLUDED_IRC_STRUCT_H */

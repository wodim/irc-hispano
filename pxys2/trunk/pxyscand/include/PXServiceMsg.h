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
 * $Id: PXServiceMsg.h,v 1.1.1.1 2003/12/30 17:08:59 mbuna Exp $
 */
#ifndef INCLUDED_PXSERVICEMSG_H
#define INCLUDED_PXSERVICEMSG_H

/* This C/C++ header defines service messages implemented by pxyscand, which
 * are available thru the OPAS protocol as custom user messages. Integers are
 * in network byte order (big endian).
 */

/* PXSHeader - pxyscand service header
 * sig: 32 bit vector to identify this service. I think I should add this to
 *      the OPAS protocol as there is currently no way to provide completely
 *      independant services over OPAS user msgs.
 * ver: version of this header, to avoid header version conflict (bumped for
 *      any structure changes).
 * cmd: command ID (as defined below).
 */
typedef struct
  {
  uint32_t sig;
  uint32_t ver;
  uint32_t cmd;
  } PXSHeader;

#define PX_VERSION   1
#define PXYSCAND_SIG 0x50585953


/* STATUS command
 * --------------
 * Query: only the header
 * Reply: header + a structure with status info in it + PXSMInfo's appended
 * 
 */
#define PX_CMD_STATUS 1

// activity, timeout, uptime, scans completed
// open gates found, cache hits
// cache times: proxy, noproxy
// current target IP and port, last target_check
// scan queue size

typedef struct
  {
  PXSHeader head;
  
  int32_t uptime;            // Daemon's uptime in seconds
  uint32_t curIPScan;        // Current number of IPs being scanned
  uint32_t maxIPScan;        // Max simult. number of IPs that can be scanned
  uint32_t curRunScan;       // Current number of launched scans
  uint32_t maxRunScan;       // Max number of scans that can be launched
  uint32_t scanQSize;        // Current number of IPs in scan queue
  
  struct in_addr targetAddr; // Current target IP#
  uint32_t targetPort;       // Current target port
  int32_t targetLastCheck;   // Last target check time in seconds
  
  uint32_t sessScannedCount; // Number of scans done under the current session
  uint32_t sessProxyCount;   // Number of proxy found under the current session
  
  uint32_t servScannedCount; // Total number of scans done by the daemon
  uint32_t servProxyCount;   // Total number of proxy found by the daemon
  
  int32_t timeout;           // Connections timeout (from config)
  
  uint32_t numMInfo;         // Number of PXM info structures appended
  } PXSStatus;

typedef struct
  {
  uint32_t port;
  uint32_t connCount;
  uint32_t proxyCount;
  char shortName[16];
  } PXSMInfo;


/* STATS command
 * -------------
 * Query: only the header
 * Reply: header + a simple structure with stats info in it
 * 
 */

#define PX_CMD_STATS 2

// number of sessions opened
// total queries
// unreach, proxy cache size

typedef struct
  {
  PXSHeader head;
  
  uint32_t servQueryCount;   // Total number of queries
  uint32_t servScannedCount; // Total number of scans done by the daemon
  uint32_t servProxyCount;   // Total number of proxy found by the daemon
  uint32_t servNoScanHits;   // Number of queries that matched the NoScan list
  uint32_t servIPCacheHits;  // Number of successful IP cache hits (no-proxy)
  uint32_t servPXCacheHits;  // Number of successful proxy cache hits
  uint32_t servInProgressCount; // Number of scan query already in progress
  
  uint32_t servIP4CacheCount;   // IPv4 "no proxy" cache current count
  uint32_t servIP4CacheSize;    // Max IPv4 in "no proxy" cache
  int32_t servIP4CacheExpire;   // Expiration delay in seconds
  uint32_t servPX4CacheCount;   // IPv4 "proxy" cache current count
  uint32_t servPX4CacheSize;    // Max IPv4 in "proxy" cache
  int32_t servPX4CacheExpire;   // Expiration delay in seconds
  
  uint32_t sessQueryCount;   // Number of queries for the current session
  uint32_t sessScannedCount; // Number of scans done under the current session
  uint32_t sessProxyCount;   // Number of proxy found under the current session
  uint32_t sessErrorCount;   // Number of errors encountered under the session
  
  uint32_t sessCurrent;      // Number of OPAS sessions currently opened
  uint32_t sessCount;        // OPAS sessions count
  uint32_t sessRejectedCount; // Rejected OPAS sessions count
  
  uint32_t sessReadBytes;    // Bytes read during the current OPAS session
  uint32_t sessWriteBytes;   // Bytes written during the current OPAS session
  
  char version[64];          // Server version
  
  } PXSStats;



/* REMOVE command
 * --------------
 * Invalidate an address in the cache
 * Query: header + address + empty status
 * Reply: same + status=1 if address found in the cache and removed
 * 
 */

#define PX_CMD_REMOVE 3

typedef struct
  {
  PXSHeader head;
  
  struct in_addr addr;
  uint32_t status;
  
  } PXSRemove4;

typedef struct
  {
  PXSHeader head;
  
  struct in6_addr addr;
  uint32_t status;
  
  } PXSRemove6;



#endif /* INCLUDED_PXSERVICEMSG_H */


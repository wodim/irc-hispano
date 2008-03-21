/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyscand (from pxys)
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
#ifndef INCLUDED_PXCONFIG_H_
#define INCLUDED_PXCONFIG_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>

#include <vector>

#include "PXMutex.h"

#define PXOPAS_PROTO_TCP 1
#define PXOPAS_PROTO_UDP 2

using std::vector;

struct PXConfigOPAS
  {
  int port;
  in_addr local_address;
  vector<int> proto;
  vector<in_addr> allow;
  int limit;
  };

struct PXConfigModule
  {
  int id;
  int port;
  };

enum
  {
  CONFIG_MODULE_WINGATE,
  CONFIG_MODULE_SOCKS,
  CONFIG_MODULE_HTTP,
  CONFIG_MODULE_CRAZYBANDIT,
  /* Add new module type id here. */
  CONFIG_MODULE_3COM812,
  CONFIG_MODULE_3COM812WEB,
  CONFIG_MODULE_TROJAN,
  CONFIG_MODULE_TOR,
  CONFIG_MODULE_BOUNCERHISPANO
  };

struct PXConfigScannerTarget
  {
  in_addr address;
  int port;
  char *secret;
  };

struct PXConfigScanner
  {
  int maxscans;
  vector<PXConfigModule> modules;
  vector<in_addr> source_pool;
  vector<PXConfigScannerTarget> targets;
  time_t target_check;
  char *log_agent;
  time_t timeout;
  };
  
struct NetworkNetmask
  {
  in_addr network;
  in_addr netmask;
  };

struct Country
  {
  char *country;
  };
    
struct PXConfigNoDNSBL
  {
  vector<Country> country;
  vector<NetworkNetmask> address;
  };
         
struct TypesDNSBL
  {
  int id;
  char *reason;;
  };
  
struct PXConfigDNSBLServer
  {
  char *name;
  char *domain;
  vector<TypesDNSBL> types;
  };
  
struct PXConfigDNSBL
  {
  vector<PXConfigDNSBLServer> servers;
  vector<PXConfigNoDNSBL> nocheck;
  };

struct PXConfigCache
  {
  char *dir;
  int expire;
  int proxy_expire;
  int maxips;
  };

struct PXConfigNoScan
  {
  vector<NetworkNetmask> address;
  };

struct PXConfig
  {
  PXConfig();
  ~PXConfig();
  
  bool OPASIsAllowed(const in_addr &in) const;
  bool HasSourcePool() const { return scanner.source_pool.size() > 0; }
  in_addr GetOneFromSourcePool() const;
  bool CheckNoScan(const in_addr &addr) const;
  
  PXConfigOPAS opas;
  PXConfigScanner scanner;
  PXConfigDNSBL dnsbl;
  PXConfigCache cache;
  PXConfigNoScan noscan;
protected:
  mutable PXMutex mutex;
  };

#endif /* INCLUDED_PXCONFIG_H_ */

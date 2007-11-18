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
#define RCSID "$Id: PXConfigLoader.cc,v 1.7 2006/09/11 22:02:54 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXConfigLoader.h"
#include "PXConfig.h"

#include <cassert>
#include <iostream>

#include <arpa/inet.h>

// XPaths...

#define XP_OPAS                        "/pxyscand/opas"
#define XP_OPAS_PORT                   "port"
#define XP_OPAS_BIND_ADDRESS           "bind-address"
#define XP_OPAS_PROTOCOL               "protocol"
#define XP_OPAS_ALLOW                  "allow"
// TODO: deny
#define XP_OPAS_LIMIT                  "limit"

#define XP_SCANNER                     "/pxyscand/scanner"
#define XP_SCANNER_MAXSCANS            "maxscans"
#define XP_SCANNER_MODULE_TYPE_ATTR    "module/@type"
#define XP_SCANNER_MODULE              "module[@type]"
#define XP_SCANNER_SOURCE_POOL         "source-pool"
#define XP_SCANNER_SOURCE_POOL_ADDRESS "address"
#define XP_SCANNER_TARGET              "target"
#define XP_SCANNER_TARGET_ADDRESS      "address"
#define XP_SCANNER_TARGET_PORT         "port"
#define XP_SCANNER_TARGET_CHECK        "target-check"
#define XP_SCANNER_TARGET_SECRET       "secret"
#define XP_SCANNER_LOG_AGENT           "log-agent"
#define XP_SCANNER_TIMEOUT             "timeout"

#define XP_CACHE                       "/pxyscand/cache"
#define XP_CACHE_DIRECTORY             "directory"
#define XP_CACHE_EXPIRE                "expire"
#define XP_CACHE_PROXY_EXPIRE          "proxy-expire"
#define XP_CACHE_MAXIPS                "maxips"

#define XP_NOSCAN                      "/pxyscand/noscan"
#define XP_NOSCAN_ADDRESS              "address"

using std::clog;
using std::cout;
using std::endl;

PXConfigLoader::PXConfigLoader(const char *inFile)
  : PXXMLXPathLoaderImp(inFile)
  {
  }

PXConfigLoader::~PXConfigLoader()
  {
  }


bool
PXConfigLoader::StringToNetworkNetmask(const char *str,
                                       in_addr &oNet,
                                       in_addr &oMask)
  {
  char ipbuf[16];
  size_t len, j;
  int k, dot = 0;
  char *p;
  
  if ((p = strchr(str, '/')) == NULL)
    oMask.s_addr = htonl(0xffffffff);
  else
    {
    *p++ = '\0';
    if (!strchr(p, '.'))
      {
      int nbits;
            
      if ((nbits = atoi(p)) > 0 && nbits <= 32)
        oMask.s_addr = htonl(~(0xffffffff >> nbits));
      }
    else if (inet_pton(AF_INET, p, &oNet) == -1)
      return false;
    }
  
  len = strlen(str);
  len = len < sizeof(ipbuf) ? len : sizeof(ipbuf) - 1;
  for (j = 0; j < len; j++)
    {
    if (str[j] == '.')
      dot++;
    ipbuf[j] = str[j];
    }
  if (dot > 4 || len >= sizeof(ipbuf) - ((3 - dot) * 2))
    return false;
  
  for (k = 0; k < 3 - dot; k++)
    {
    ipbuf[j++] = '.';
    ipbuf[j++] = '0';
    }
  
  assert(j < sizeof(ipbuf));
  ipbuf[j] = '\0';
  
  if (inet_pton(AF_INET, ipbuf, &oNet) == -1)
    return false;
  
  return true;
  }

PXConfig *
PXConfigLoader::Load()
  {
  PXConfig *cfg = new PXConfig;
  try
    {
    this->DoLoad(cfg);
    }
  catch (...)
    {
    delete cfg;
    throw;
    }
  return cfg;
  }

void
PXConfigLoader::DoLoad(PXConfig *cfg)
  {
  xmlXPathObjectPtr o, o2;
  
  if ((o = this->EvalUnique(XP_OPAS)))
    {
    StPXXPathContextNode cxNode(&mCx->node, o->nodesetval->nodeTab[0]);
    
    /* OPAS Port */
    cfg->opas.port = this->GetInteger(XP_OPAS_PORT, 0, 1);
    
    /* OPAS local address (bind) */
    this->GetAddress(XP_OPAS_BIND_ADDRESS, 0, 0, "opas/bind-address",
                     &cfg->opas.local_address);
    
    /* Protocol(s) to enable (tcp, udp) */
    o2 = this->Eval(XP_OPAS_PROTOCOL);
    if (!o2 || !o2->nodesetval->nodeNr)
      PXXMLException::Throw("Missing parameter", "opas/protocol");
    cfg->opas.proto.reserve(o2->nodesetval->nodeNr);
    for (int i = 0; i < o2->nodesetval->nodeNr; i++)
      {
      xmlChar *s;
      xmlNodePtr n;
      n = o2->nodesetval->nodeTab[i]->xmlChildrenNode;
      if (!(s = xmlNodeListGetString(mDoc, n, 1)))
        PXXMLException::Throw("Empty protocol, use: tcp or udp",
                              "opas/protocol");
      if (!strcmp((char *)s, "tcp"))
        cfg->opas.proto.push_back(PXOPAS_PROTO_TCP);
      else if (!strcmp((char *)s, "udp"))
        cfg->opas.proto.push_back(PXOPAS_PROTO_UDP);
      else
        {
        xmlFree(s);
        PXXMLException::Throw("Unknown protocol (use: tcp or udp)",
                              "opas/protocol");
        }
      xmlFree(s);
      }
    xmlXPathFreeObject(o2);
    
    /* Allowed IPs */
    o2 = this->Eval(XP_OPAS_ALLOW);
    if (!o2 || !o2->nodesetval->nodeNr)
      PXXMLException::Throw("Missing parameter", "opas/allow");
    cfg->opas.allow.reserve(o2->nodesetval->nodeNr);
    for (int i = 0; i < o2->nodesetval->nodeNr; i++)
      {
      xmlChar *s;
      xmlNodePtr n;
      in_addr addr;
      
      n = o2->nodesetval->nodeTab[i]->xmlChildrenNode;
      if (!(s = xmlNodeListGetString(mDoc, n, 1)))
        PXXMLException::Throw("Empty allow field", "opas/allow");
      int res = inet_pton(AF_INET, (char *)s, &addr);
      xmlFree(s);
      if (res == -1)
        PXXMLException::Throw("Bad IP address", "opas/allow");
      cfg->opas.allow.push_back(addr);
      }
    xmlXPathFreeObject(o2);
    
    /* OPAS limit */
    cfg->opas.limit = this->GetInteger(XP_OPAS_LIMIT, 0, 1);
    
    xmlXPathFreeObject(o);
    }
  
  if ((o = this->EvalUnique(XP_SCANNER)))
    {
    StPXXPathContextNode cxNode(&mCx->node, o->nodesetval->nodeTab[0]);
    
    cfg->scanner.maxscans = this->GetInteger(XP_SCANNER_MAXSCANS, 0, 1);
    if (cfg->scanner.maxscans <= 0)
      cfg->scanner.maxscans = 100; // bah
    
    if (!(o2 = this->Eval(XP_SCANNER_MODULE_TYPE_ATTR)))
      PXXMLException::Throw("Missing parameter", "scanner/module");
    else
      {
      cfg->scanner.modules.reserve(o2->nodesetval->nodeNr);
      for (int i = 0; i < o2->nodesetval->nodeNr; i++)
        {
        PXConfigModule m;
        xmlChar *s;
        xmlNodePtr n = o2->nodesetval->nodeTab[i]->xmlChildrenNode;
        
        if (!(s = xmlNodeListGetString(mDoc, n, 1)))
          PXXMLException::Throw("Empty type attribute not allowed",
                                "scanner/module");
        
        m.port = 0; // only used for http scan module
        
        if (!xmlStrcasecmp(s, (xmlChar*)"wingate"))
          m.id = CONFIG_MODULE_WINGATE;
        else if (!xmlStrcasecmp(s, (xmlChar*)"socks"))
	{
          m.id = CONFIG_MODULE_SOCKS;
	  try {
	    m.port = this->GetInteger(XP_SCANNER_MODULE, i, 65536);
	  } catch (PXXMLException &e) {
	    clog << "Please note that socks ports are now mandatory and must be specified in the config file" << endl;
	    throw;
	  }
	  // Note: port 0 not allowed for convenience.
	  if (m.port <= 0 || m.port >= 65536)
	    PXXMLException::Throw("Bad socks port number", "scanner/module");
	}
        else if (!xmlStrcmp(s, (xmlChar*)"http"))
          {
          m.id = CONFIG_MODULE_HTTP;
          m.port = this->GetInteger(XP_SCANNER_MODULE, i, 65536);
          // Note: port 0 not allowed for convenience.
          if (m.port <= 0 || m.port >= 65536)
            PXXMLException::Throw("Bad proxy port number", "scanner/module");
          }
        else if (!xmlStrcasecmp(s, (xmlChar*)"crazybandit"))
          m.id = CONFIG_MODULE_CRAZYBANDIT;
        
        /* Handle new module type here. */
        else if (!xmlStrcasecmp(s, (xmlChar*)"3com812"))
          m.id = CONFIG_MODULE_3COM812;        
        else
          PXXMLException::Throw("Unknown scanner/module type attribute",
                                "scanner/module");
        
        cfg->scanner.modules.push_back(m);
        }
      xmlXPathFreeObject(o2);
      }
    
    
    if ((o2 = this->Eval(XP_SCANNER_SOURCE_POOL)))
      {
      StPXXPathContextNode cxNode(&mCx->node, o2->nodesetval->nodeTab[0]);
      
      xmlXPathObjectPtr o3 = this->Eval(XP_SCANNER_TARGET_ADDRESS);
      if (o3)
        {
        int cnt = o3->nodesetval->nodeNr;
        cfg->scanner.source_pool.reserve(cnt);
        xmlXPathFreeObject(o3);
        for (int i = 0; i < cnt; i++)
          {
          sockaddr_in sin;
          memset(&sin, 0, sizeof(sin));
          sin.sin_family = AF_INET;
          sin.sin_port = htons(0);
          
          this->GetAddress(XP_SCANNER_TARGET_ADDRESS, i, 0,
                           "scanner/source-pool/address", &sin.sin_addr);
          
          // Test the local address now.
          int testFD = socket(AF_INET, SOCK_STREAM, 0);
          if (testFD == -1) // doh !
            PXXMLException::Throw("Source pool address test failed",
                                  "socket() failed!");
          if (bind(testFD, (sockaddr *)&sin, sizeof(sin)) == -1)
            PXXMLException::Throw("Source pool address test failed",
                                  "bind() failed!");
          close(testFD);
          cfg->scanner.source_pool.push_back(sin.sin_addr);
          }
        }
      xmlXPathFreeObject(o2);
      }
    
    if (!(o2 = this->Eval(XP_SCANNER_TARGET)))
      PXXMLException::Throw("Missing parameter", "scanner/target");
    
    cfg->scanner.targets.reserve(o2->nodesetval->nodeNr);
    for (int i = 0; i < o2->nodesetval->nodeNr; i++)
      {
      StPXXPathContextNode cxNodeTarget(&mCx->node,
                                        o2->nodesetval->nodeTab[i]);
      PXConfigScannerTarget target;
      
      this->GetAddress(XP_SCANNER_TARGET_ADDRESS, 0, 1,
                       "scanner/target/address", &target.address);
      target.port = this->GetInteger(XP_SCANNER_TARGET_PORT, 0, 1);

      target.secret = (char*)CopyString(XP_SCANNER_TARGET_SECRET, 0, 1);
      
      cfg->scanner.targets.push_back(target);
      }
    xmlXPathFreeObject(o2);

    cfg->scanner.target_check = (time_t)this->GetInteger(
                                                XP_SCANNER_TARGET_CHECK, 0, 1);
    cfg->scanner.log_agent = (char *)this->CopyString(XP_SCANNER_LOG_AGENT,
                                                      0, 0);
    cfg->scanner.timeout = (time_t)this->GetInteger(XP_SCANNER_TIMEOUT, 0, 1);
    if (cfg->scanner.timeout <= 0)
      cfg->scanner.timeout = 30;
    
    xmlXPathFreeObject(o);
    }

  if ((o = this->EvalUnique(XP_CACHE)))
    {
    StPXXPathContextNode cxNode(&mCx->node, o->nodesetval->nodeTab[0]);
    
    // Cache directory
    cfg->cache.dir = (char *)this->CopyString(XP_CACHE_DIRECTORY, 0, 1);
    // Cache expire seconds
    cfg->cache.expire = this->GetInteger(XP_CACHE_EXPIRE, 0, 1);
    // Proxy cache expire seconds
    cfg->cache.proxy_expire = this->GetInteger(XP_CACHE_PROXY_EXPIRE, 0, 1);
    // Cache max IPs
    cfg->cache.maxips = this->GetInteger(XP_CACHE_MAXIPS, 0, 1);
    
    xmlXPathFreeObject(o);
    }
  
  if ((o = this->EvalUnique(XP_NOSCAN)))
    {
    StPXXPathContextNode cxNode(&mCx->node, o->nodesetval->nodeTab[0]);
    
    o2 = this->Eval(XP_NOSCAN_ADDRESS);
    if (o2)
      {
      cfg->noscan.address.reserve(o2->nodesetval->nodeNr);
      for (int i = 0; i < o2->nodesetval->nodeNr; i++)
        {
        xmlChar *s;
        xmlNodePtr n;
        n = o2->nodesetval->nodeTab[i]->xmlChildrenNode;
        if (!(s = xmlNodeListGetString(mDoc, n, 1)))
          PXXMLException::Throw("Empty allow field", "noscan/address");
        
        NetworkNetmask nwnm;
        bool res = StringToNetworkNetmask((char *)s, nwnm.network,
                                          nwnm.netmask);
        xmlFree(s);
        if (!res)
          PXXMLException::Throw("Invalid address", "noscan/address");
        cfg->noscan.address.push_back(nwnm);
        }
      xmlXPathFreeObject(o2);
      }
    xmlXPathFreeObject(o);
    }
  }

void
PXConfigLoader::Dump(PXConfig *cfg)
  {
  size_t i;
  
  cout << "OPAS config" << endl;
  cout << "port: " << cfg->opas.port << endl;
  cout << "local_address: ";
  if (cfg->opas.local_address.s_addr == INADDR_ANY)
    cout << "default (any)";
  else
    cout << inet_ntoa(cfg->opas.local_address);
  cout << endl;
  cout << "proto: ";
  for (i = 0; i < cfg->opas.proto.size(); i++)
    if (cfg->opas.proto[i] == PXOPAS_PROTO_TCP)
      cout << "tcp ";
    else
      cout << "udp";
  cout << endl;
  
  cout << "allow: ";
  for (size_t i = 0; i < cfg->opas.allow.size(); i++)
    cout << inet_ntoa(cfg->opas.allow[i]) << " ";
  cout << endl;
  
  cout << "limit: ";
  if (cfg->opas.limit == 0)
    cout << "unlimited";
  else
    cout << cfg->opas.limit;
  cout << endl << endl;
  
  cout << "SCANNER config" << endl;
  cout << "maxscans: " << cfg->scanner.maxscans << endl;
  if (cfg->scanner.modules.size() == 0)
    {
    cout << "NO SCANNER MODULE ENABLED !!!" << endl;
    exit(1);
    }
  else
    {
    cout << "Scanner modules (" << cfg->scanner.modules.size() << "):" << endl;
    for (i = 0; i < cfg->scanner.modules.size(); i++)
      {
      cout << "id: " << cfg->scanner.modules[i].id << endl;
      if (cfg->scanner.modules[i].id == CONFIG_MODULE_SOCKS || cfg->scanner.modules[i].id == CONFIG_MODULE_HTTP)
      {
	if (cfg->scanner.modules[i].port > 0 && cfg->scanner.modules[i].port < 65536)
	  cout << "port: " << cfg->scanner.modules[i].port << endl;
	else
	  {
	  cout << "BAD PORT NUMBER: " << cfg->scanner.modules[i].port << endl;
	  exit(1);
	  }
        }
      }
    }
  if (cfg->scanner.source_pool.size() == 0)
    cout << "No source pool." << endl;
  else
    {
    cout << "Source pool (" << cfg->scanner.source_pool.size() << "):" << endl;
    for (i = 0; i < cfg->scanner.source_pool.size(); i++)
      cout << "address: " << inet_ntoa(cfg->scanner.source_pool[i]) << endl;
    }
  cout << "Targets (" << cfg->scanner.targets.size() << "):" << endl;
  for (i = 0; i < cfg->scanner.targets.size(); i++)
    {
    cout << "address: " << inet_ntoa(cfg->scanner.targets[i].address);
    cout << " port: " << cfg->scanner.targets[i].port;
    cout << endl;
    }
  cout << "target_check: " << cfg->scanner.target_check << endl;

  cout << "CACHE config:" << endl;
  cout << "expire: " << cfg->cache.expire << " (seconds)" << endl;
  cout << "maxips: " << cfg->cache.maxips << endl << endl;
  
  cout << "NOSCAN config:" << endl;
  for (i = 0; i < cfg->noscan.address.size(); i++)
    {
    cout << "noscan: " << inet_ntoa(cfg->noscan.address[i].network);
    cout << "/" << inet_ntoa(cfg->noscan.address[i].netmask) << endl;
    }
  }

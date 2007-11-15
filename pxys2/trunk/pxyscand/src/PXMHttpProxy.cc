// Copyright (C) 2003-2005 Stephane Thiell
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
#define RCSID "$Id: PXMHttpProxy.cc,v 1.3 2005/05/19 00:49:38 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define HTTPPROXY_SHORTNAME "HttpProxy"

#define HTTPPROXY_DESCR     "Vulnerable http proxy"


#define MAX_LINES_TRY 2
#define CRLF "\r\n"

#include "PXMHttpProxy.h"

#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>

using std::clog;
using std::endl;

map<uint16_t, uint32_t*> PXMHttpProxy::sConnCountMap;
map<uint16_t, uint32_t*> PXMHttpProxy::sProxyCountMap;

PXMHttpProxy::PXMHttpProxy(PXScan *inScan, int inPort)
  : PXScanModule(inScan), mLinesTry(0), mPort(inPort), mIsProxy(false)
  {
  if (sConnCountMap.find(mPort) == sConnCountMap.end())
    {
    sConnCountMap[mPort] = new uint32_t;
    *sConnCountMap[mPort] = 0;
    sProxyCountMap[mPort] = new uint32_t;
    *sProxyCountMap[mPort] = 0;
    }
  }

PXMHttpProxy::~PXMHttpProxy()
  {
  }

void
PXMHttpProxy::InitModule()
  {
  RegisterPXM(HTTPPROXY_SHORTNAME, mPort, sConnCountMap[mPort], sProxyCountMap[mPort]);
  }

bool
PXMHttpProxy::StartScan()
  {
  peak_task task = peak_task_self();
  
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = this->GetAddress();
  sin.sin_port = htons((uint16_t)mPort);
  
  mStream = peak_stream_socket_create((struct sockaddr *)&sin, sizeof(sin),
                                      PEAK_STREAM_OPT_LINEMODE,
                                      EventCallback,
                                      this);
  
  if (!mStream)
    return false;
  
  if (this->IsLocalAddressSet())
    {
    sockaddr_in local_sin;
    memset(&local_sin, 0, sizeof(local_sin));
    local_sin.sin_family = AF_INET;
    local_sin.sin_addr = this->GetLocalAddress();
    local_sin.sin_port = htons(0);
    
    peak_stream_set_address(mStream, (sockaddr*)&local_sin, sizeof(local_sin));
    }
  
  /* Enable built-in timeout option, this is so useful here. */
  peak_stream_set_timeout(mStream, GetTimeout());
  
  /* Connect (don't block) */
  if (peak_stream_connect(mStream) == -1)
    {
    this->Cleanup();
    this->ProxyNotFound();
    }
  else
    peak_stream_schedule(mStream, task);
  return true;
  }

void
PXMHttpProxy::Cleanup()
  {
  assert(mStream != NULL);
  peak_release(mStream);
  mStream = NULL;
  }

void
PXMHttpProxy::SendConnectRequest(peak_stream s)
  {
  in_addr targetAddr = GetTargetAddress();
  char uriHostname[16];
  int uriPort = GetTargetPort();
  
  inet_ntop(AF_INET, &targetAddr, uriHostname, sizeof(uriHostname));
  
  peak_stream_set_buffered(s, 1, 64, 64*2, NULL);
  
  peak_stream_msgbuf_make(s, "CONNECT %s:%d HTTP/1.0" CRLF,
                          uriHostname, uriPort);
  peak_stream_msgbuf_make(s, "User-Agent: pxyscand/" VERSION CRLF CRLF);
  }

void
PXMHttpProxy::CommitFound()
  {
  (*sProxyCountMap[mPort])++;
  this->Cleanup();
  this->ProxyFound(OPAS_PROXY_TYPE_HTTPPROXY, mPort, HTTPPROXY_DESCR);
  }

void
PXMHttpProxy::ProcessEvent(peak_stream s, int type)
  {
  char *line;
  int err;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      (*sConnCountMap[mPort])++;
      this->SendConnectRequest(s);
      break;
    case PEAK_STREAM_EVT_READ:
      line = peak_stream_get_line(s);
#if 0
      clog << "PXMHttpProxy:: line: " << line << endl;
#endif
      // The algorithm used here looks like a bit weird at the first time,
      // but that's because we try to log the httpd/proxy agent for stats
      // purpose.
      if (!strncmp(line, "HTTP/1.0 200 Connection established", 35)
          || !strncmp(line, "HTTP/1.1 200 Connection established", 35))
        mIsProxy = true; // Confirmed
      else if (!strncmp(line, "HTTP/1.0 2", 10)
               || !strncmp(line, "HTTP/1.1 2", 10))
        clog << "PXMHttpProxy:: HTTP 2xx reply: " << line << endl;
      else if (!strncmp(line, "Proxy-agent:", 12)
        || !strncmp(line, "User-agent:", 11)
        || !strncmp(line, "Server:", 7))
        {
        if (LogAgent())
          {
          char buf[16], *p;
          in_addr addr = GetAddress();
          inet_ntop(AF_INET, &addr, buf, sizeof(buf));
          while (*line++ != ':')
            ;
          if (*line == ' ')
            line++;
          if ((p = strchr(line, ',')))
            *p = '\0';
          mLogMutex.Lock();
          GetFAgent() << peak_time() << ',' << buf << ',' << mPort
                      << ',' << line << endl;
          mLogMutex.Unlock();
          }
        }
      else if (mLinesTry++ < MAX_LINES_TRY)
        break;
      
      /* fall through */
    case PEAK_STREAM_EVT_ERROR:
    case PEAK_STREAM_EVT_TIMEDOUT:
      if (mIsProxy)
        {
        this->CommitFound();
        return; /* done! */
        }
      this->Cleanup();
      this->ProxyNotFound();
      break;
    case PEAK_STREAM_EVT_END:
      if (mIsProxy)
        {
        this->CommitFound();
        return; /* done! */
        }
      err = peak_stream_get_error(s);
      this->Cleanup();
      if (err == ENETUNREACH)
        this->ScanError(OPAS_ERROR_NETUNREACH); // Can't scan !
      else if (err == ENETDOWN)
        this->ScanError(OPAS_ERROR_NETDOWN);    // Even worst !
      else
        this->ProxyNotFound();
      break;
    default:
      break;
    }
  }

void
PXMHttpProxy::EventCallback(peak_stream s, int type, void *context)
  {
  PXMHttpProxy *pxm = reinterpret_cast<PXMHttpProxy*>(context);
  pxm->ProcessEvent(s, type);
  }

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
#define RCSID "$Id: PXMHttpGet.cc,v 1.2 2006/09/10 22:17:24 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define HTTPGET_SHORTNAME "HttpGet"

#define HTTPGET_DESCR     "Vulnerable http (GET) proxy"


#define MAX_LINES_TRY 10
#define CRLF "\r\n"

#include "PXMHttpGet.h"

#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>

#include "PXSecret.h"

using std::clog;
using std::endl;

map<uint16_t, uint32_t*> PXMHttpGet::sConnCountMap;
map<uint16_t, uint32_t*> PXMHttpGet::sProxyCountMap;

PXMHttpGet::PXMHttpGet(PXScan *inScan, int inPort)
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

PXMHttpGet::~PXMHttpGet()
  {
  }

void
PXMHttpGet::InitModule()
  {
  RegisterPXM(HTTPGET_SHORTNAME, mPort, sConnCountMap[mPort], sProxyCountMap[mPort]);
  }

bool
PXMHttpGet::StartScan()
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
PXMHttpGet::Cleanup()
  {
  assert(mStream != NULL);
  peak_release(mStream);
  mStream = NULL;
  }

void
PXMHttpGet::SendConnectRequest(peak_stream s)
  {
  in_addr targetAddr = GetTargetAddress();
  char uriHostname[16];
  int uriPort = GetTargetPort();
  
  inet_ntop(AF_INET, &targetAddr, uriHostname, sizeof(uriHostname));
  
  peak_stream_set_buffered(s, 1, 64, 64*2, NULL);
  
  peak_stream_msgbuf_make(s, "GET http://%s:%d HTTP/1.0" CRLF,
                          uriHostname, uriPort);
  peak_stream_msgbuf_make(s, "Host: %s" CRLF, uriHostname);
  peak_stream_msgbuf_make(s, "User-Agent: pxyscand/" VERSION CRLF CRLF);
  }

void
PXMHttpGet::CommitFound()
  {
  (*sProxyCountMap[mPort])++;
  this->Cleanup();
  this->ProxyFound(OPAS_PROXY_TYPE_HTTPGET, mPort, HTTPGET_DESCR);
  }

void
PXMHttpGet::ProcessEvent(peak_stream s, int type)
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
      clog << "PXMHttpGet:: line: " << line << endl;
#endif
      if (ScannerSha256StringCheck(line,strlen(line),GetTargetSecret()))
        mIsProxy = true; // Confirmed
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
PXMHttpGet::EventCallback(peak_stream s, int type, void *context)
  {
  PXMHttpGet *pxm = reinterpret_cast<PXMHttpGet*>(context);
  pxm->ProcessEvent(s, type);
  }

// Copyright (C) 2003, 2004 Stephane Thiell
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
#define RCSID "$Id: PXMWingate.cc,v 1.4 2006/09/10 22:16:39 spale Exp $"

// This class scans for Open Wingate or Cisco (with "cisco" password).

#define WINGATE_SHORTNAME "WinGate"
#define CISCO_SHORTNAME   "Cisco"

#define WINGATE_DESCR     "Insecure WinGate server"
#define CISCO_DESCR       "Insecure Cisco router"

#define WINGATE_PORT      23
#define CISCO_PORT        WINGATE_PORT

#define MAX_LINES_TRY     30
#define CRLF              "\r\n"

#include "PXMWingate.h"
#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <opas/opas.h>

#include "PXSecret.h"

using std::clog;
using std::endl;

uint32_t PXMWingate::sConnCount = 0;
uint32_t PXMWingate::sProxyCount1 = 0;
uint32_t PXMWingate::sProxyCount2 = 0;

PXMWingate::PXMWingate(PXScan *inScan)
  : PXScanModule(inScan), mStream(NULL), mCiscoRequest(false), mLinesTry(0)
  {
  }

PXMWingate::~PXMWingate()
  {
  }

void
PXMWingate::InitModule()
  {
  RegisterPXM(WINGATE_SHORTNAME, WINGATE_PORT, &sConnCount, &sProxyCount1);
  RegisterPXM(CISCO_SHORTNAME, CISCO_PORT, &sConnCount, &sProxyCount2);
  }

bool
PXMWingate::StartScan()
  {
  peak_task task = peak_task_self();
  
  sockaddr_in sin;
  memset(&sin, 0, sizeof(sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = this->GetAddress();
  sin.sin_port = htons(WINGATE_PORT);
  
  mStream = peak_stream_socket_create((sockaddr *)&sin, sizeof(sin),
                                      PEAK_STREAM_OPT_DEFAULT,
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
  
  /* Set write buffered mode. */
  peak_stream_set_buffered(mStream, 1, 2, 512, NULL);
  
  /* Enable built-in timeout option, this is so useful here. */
  peak_stream_set_timeout(mStream, GetTimeout() / 2 + 1);
  
  /* Connect (don't block) */
  if (peak_stream_connect(mStream) == -1)
    {
    peak_release(mStream);
    this->ProxyNotFound();
    }
  else
    peak_stream_schedule(mStream, task);
  return true;
  }

void
PXMWingate::SendCiscoRequest(peak_stream s)
  {
  in_addr targetAddr = GetTargetAddress();
  char uriHostname[16];
  int uriPort = GetTargetPort();
  
  inet_ntop(AF_INET, &targetAddr, uriHostname, sizeof(uriHostname));
  
  peak_stream_set_buffered(s, 1, 64, 64*2, NULL);
  peak_stream_msgbuf_make(s, "cisco" CRLF);
  peak_stream_msgbuf_make(s, "telnet %s %d" CRLF, uriHostname, uriPort);
  }

void
PXMWingate::ProcessEvent(peak_stream s, int type)
  {
  char *line;
  char winReadBuf[64];
  int err;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      sConnCount++;
      break;
    case PEAK_STREAM_EVT_READ:
      if (!mCiscoRequest)
        {
        memset(winReadBuf, 0, sizeof(winReadBuf));
        if (peak_stream_read(s, winReadBuf, sizeof(winReadBuf)) > 0)
          {
          if (!strncmp(winReadBuf, "WinGate>", 8)
              || !strncmp(winReadBuf, "Too m", 5))
            {
            sProxyCount1++;
#if 0
            clog << "PXMWingate::ProcessEvent: proxy found: "
                 << winReadBuf << endl;
#endif
            peak_release(s);
            this->ProxyFound(OPAS_PROXY_TYPE_WINGATE, WINGATE_PORT,
                             WINGATE_DESCR);
            return; /* we are done */
            }
          }
        if (mLinesTry++ < MAX_LINES_TRY)
          break;
        }
      else
        {
        line = peak_stream_get_line(s);
        
	if (ScannerSha256StringCheck(line,strlen(line),GetTargetSecret()))
          {
          sProxyCount2++;
          peak_release(s);
          this->ProxyFound(OPAS_PROXY_TYPE_CISCOROUTER, CISCO_PORT,
                           CISCO_DESCR);
          return; /* done! */
          }
        else if (mLinesTry++ < MAX_LINES_TRY)
          break;
        }
      /* fall through */
    case PEAK_STREAM_EVT_END:
      this->ProxyNotFound();
      peak_release(s);
      break;
      
    case PEAK_STREAM_EVT_ERROR:
      err = peak_stream_get_error(s);
      if (err == ENETUNREACH || err == EHOSTUNREACH)
        this->ScanError(OPAS_ERROR_NETUNREACH); // Can't scan !
      else if (err == ENETDOWN || err == EHOSTDOWN)
        this->ScanError(OPAS_ERROR_NETDOWN);    // Even worst !
      else
        this->ProxyNotFound();
      peak_release(s);
      break;
      
    case PEAK_STREAM_EVT_TIMEDOUT:
      if (!mCiscoRequest && peak_stream_is_open(s))
        {
        mCiscoRequest = true;
        peak_stream_set_linemode(s);
        peak_stream_set_timeout(s, GetTimeout());
        this->SendCiscoRequest(s);
        }
      else
        {
        this->ProxyNotFound();
        peak_release(s);
        }
      break;
    default:
      break;
    }
  }

void
PXMWingate::EventCallback(peak_stream s, int type, void *context)
  {
  PXMWingate *pxm = reinterpret_cast<PXMWingate*>(context);
  pxm->ProcessEvent(s, type);
  }

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
#define RCSID "$Id: PXMCrazyBandit.cc,v 1.2 2004/01/01 02:18:10 mbuna Exp $"

// This class scans for "MalayBouncer ver2.0 by CrazyBandit Co. Ltd."
// (thanks puppet for the info). It's not a misconfigured proxy but a trojan
// (mIRC script) that can act like a proxy. Although there is a generic
// password, I decided to not check it as such a bouncer on port 407 shouldn't
// really exist anyway.
// 

#define CRAZYBANDIT_SHORTNAME "CrazyBandit"
#define CRAZYBANDIT_DESCR     "MalayBouncer by CrazyBandit"
#define CRAZYBANDIT_PORT      407

#include "PXMCrazyBandit.h"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>

uint32_t PXMCrazyBandit::sConnCount = 0;
uint32_t PXMCrazyBandit::sProxyCount = 0;

PXMCrazyBandit::PXMCrazyBandit(PXScan *inScan)
  : PXScanModule(inScan)
  {
  }

PXMCrazyBandit::~PXMCrazyBandit()
  {
  }

void
PXMCrazyBandit::InitModule()
  {
  RegisterPXM(CRAZYBANDIT_SHORTNAME, CRAZYBANDIT_PORT,
              &sConnCount, &sProxyCount);
  }

bool
PXMCrazyBandit::StartScan()
  {
  peak_task task = peak_task_self();
  
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = this->GetAddress();
  sin.sin_port = htons((uint16_t)CRAZYBANDIT_PORT);
  
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
PXMCrazyBandit::Cleanup()
  {
  peak_release(mStream);
  }

void
PXMCrazyBandit::ProcessEvent(peak_stream s, int type)
  {
  char *line;
  int err;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      sConnCount++;
      break;
    case PEAK_STREAM_EVT_READ:
      line = peak_stream_get_line(s);
      
      // :210.186.115.64 NOTICE AUTH :Sila taip /quote PASS <kata laluan>
      if (strstr(line, "NOTICE AUTH :Anda perlu taip")
          || strstr(line, "NOTICE AUTH :Sila taip"))
        {
        sProxyCount++;
        this->Cleanup();
        this->ProxyFound(OPAS_PROXY_TYPE_IRCBOUNCER, CRAZYBANDIT_PORT,
                         CRAZYBANDIT_DESCR);
        return; /* done! */
        }
      /* fall through */
    case PEAK_STREAM_EVT_ERROR:
    case PEAK_STREAM_EVT_TIMEDOUT:
      this->Cleanup();
      this->ProxyNotFound();
      break;
    case PEAK_STREAM_EVT_END:
      this->Cleanup();
      err = peak_stream_get_error(s);
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
PXMCrazyBandit::EventCallback(peak_stream s, int type, void *context)
  {
  PXMCrazyBandit *pxm = reinterpret_cast<PXMCrazyBandit*>(context);
  pxm->ProcessEvent(s, type);
  }

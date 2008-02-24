// Copyright (C) 2007 Toni Garcia <zoltan@irc-dev.net>
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
#define RCSID "$Id: PXM3Com812Web.cc,v 1.4 2006/09/10 22:16:39 spale Exp $"

#define R3COM812WEB_SHORTNAME "3Com812"

#define R3COM812WEB_DESCR    "Insecure 3Com 812 ADSL router"

#define R3COM812WEB_PORT  80

#define MAX_LINES_TRY     6
#define CRLF              "\r\n"

#include "PXM3Com812Web.h"

#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <opas/opas.h>

#include "PXSecret.h"

using std::clog;
using std::endl;

uint32_t PXM3Com812Web::sConnCount = 0;
uint32_t PXM3Com812Web::sProxyCount = 0;

PXM3Com812Web::PXM3Com812Web(PXScan *inScan)
  : PXScanModule(inScan)
  {
  }

PXM3Com812Web::~PXM3Com812Web()
  {
  }

void
PXM3Com812Web::InitModule()
  {
  RegisterPXM(R3COM812WEB_SHORTNAME, R3COM812WEB_PORT, &sConnCount, &sProxyCount);
  }

bool
PXM3Com812Web::StartScan()
  {
  peak_task task = peak_task_self();
  
  sockaddr_in sin;
  memset(&sin, 0, sizeof(sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = this->GetAddress();
  sin.sin_port = htons(R3COM812WEB_PORT);
  
  mStream = peak_stream_socket_create((sockaddr *)&sin, sizeof(sin),
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
  
  /* Set write buffered mode. */
  peak_stream_set_buffered(mStream, 1, 2, 512, NULL);
  
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
PXM3Com812Web::Cleanup()
  {
  assert(mStream != NULL);
  peak_release(mStream);
  mStream = NULL;
  }

void
PXM3Com812Web::ProcessEvent(peak_stream s, int type)
  {
  char *line;
  int err;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      sConnCount++;
      peak_stream_set_buffered(s, 1, 64, 64*2, NULL);

//      peak_stream_msgbuf_make(s, "GET /adsl_reset HTTP1.1" CRLF);
      peak_stream_msgbuf_make(s, "GET / HTTP1.1" CRLF CRLF);
      break;

    case PEAK_STREAM_EVT_READ:
      line = peak_stream_get_line(s);
#if 1
      clog << "PXM3Com812Web:: line: " << line << endl;
#endif
//      if (!strncmp(line, "HTTP/1.0 200 OK", 15))
      if (!strncmp(line, "OCR-812", 7))
        {
        sProxyCount++;
        this->Cleanup();
        this->ProxyFound(OPAS_PROXY_TYPE_3COM812, R3COM812WEB_PORT,
                         R3COM812WEB_DESCR);
        return; /* we are done */
        }
      else if (mLinesTry++ < MAX_LINES_TRY)
        break;

      /* fall through */
    case PEAK_STREAM_EVT_ERROR:
    case PEAK_STREAM_EVT_TIMEDOUT:
      this->Cleanup();
      this->ProxyNotFound();
      break;

    case PEAK_STREAM_EVT_END:
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
PXM3Com812Web::EventCallback(peak_stream s, int type, void *context)
  {
  PXM3Com812Web *pxm = reinterpret_cast<PXM3Com812Web*>(context);
  pxm->ProcessEvent(s, type);
  }

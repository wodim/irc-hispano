// Copyright (C) 2007 Toni Garcia <zoltan@irc-dev.net>
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
#define RCSID "$Id: PXMBouncerHispano.cc,v 1.2 2004/01/01 02:18:10 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define BOUNCERHISPANO_SHORTNAME "BouncerHispano"
#define BOUNCERHISPANO_DESCR     "Insecure Bouncer to IRC-Hispano"


#include "PXMBouncerHispano.h"

#include <iostream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>

using std::clog;
using std::endl;

map<uint16_t, uint32_t*> PXMBouncerHispano::sConnCountMap;
map<uint16_t, uint32_t*> PXMBouncerHispano::sBouncerCountMap;

PXMBouncerHispano::PXMBouncerHispano(PXScan *inScan, int inPort)
  : PXScanModule(inScan), mPort(inPort)
  {
  if (sConnCountMap.find(mPort) == sConnCountMap.end())
    {
    sConnCountMap[mPort] = new uint32_t;
    *sConnCountMap[mPort] = 0;
    sBouncerCountMap[mPort] = new uint32_t;
    *sBouncerCountMap[mPort] = 0;
    }
  }

PXMBouncerHispano::~PXMBouncerHispano()
  {
  }

void
PXMBouncerHispano::InitModule()
  {
  RegisterPXM(BOUNCERHISPANO_SHORTNAME, mPort, sConnCountMap[mPort], sBouncerCountMap[mPort]);
  }

bool
PXMBouncerHispano::StartScan()
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
PXMBouncerHispano::Cleanup()
  {
  assert(mStream != NULL);
  peak_release(mStream);
  mStream = NULL;
  }

void
PXMBouncerHispano::ProcessEvent(peak_stream s, int type)
  {
  char *line;
  int err;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      (*sConnCountMap[mPort])++;
      break;
    case PEAK_STREAM_EVT_READ:
      line = peak_stream_get_line(s);
      
      if (strstr(line, ".irc-hispano.org"))
        {
        (*sBouncerCountMap[mPort])++;
        this->Cleanup();
        this->ProxyFound(OPAS_PROXY_TYPE_BOUNCERHISPANO, mPort, BOUNCERHISPANO_DESCR);
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
PXMBouncerHispano::EventCallback(peak_stream s, int type, void *context)
  {
  PXMBouncerHispano *pxm = reinterpret_cast<PXMBouncerHispano*>(context);
  pxm->ProcessEvent(s, type);
  }

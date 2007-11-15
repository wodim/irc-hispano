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
#define RCSID "$Id: PXMSocks.cc,v 1.6 2006/11/24 23:23:01 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define SOCKS4_SHORTNAME "Socks4"
#define SOCKS5_SHORTNAME "Socks5"

#define SOCKS4_DESCR     "Insecure Socks 4 server"
#define SOCKS5_DESCR     "Insecure Socks 5 server"

#define MAX_LINES_TRY 4

#include "PXMSocks.h"
#include "socks4.h"
#include "socks5.h"
#include "PXSecret.h"

#include <cassert>
#include <cerrno>
#include <cstring>

map<uint16_t, uint32_t*> PXMSocks::sConn4CountMap;
map<uint16_t, uint32_t*> PXMSocks::sProxy4CountMap;
map<uint16_t, uint32_t*> PXMSocks::sConn5CountMap;
map<uint16_t, uint32_t*> PXMSocks::sProxy5CountMap;

PXMSocks::PXMSocks(PXScan *inScan, int inPort)
  : PXScanModule(inScan), mStatus(SCAN_NOSOCKS), mLinesTry(0), mPort(inPort)
  {
  if (sConn4CountMap.find(mPort) == sConn4CountMap.end())
    {
    sConn4CountMap[mPort] = new uint32_t;
    *sConn4CountMap[mPort] = 0;
    sProxy4CountMap[mPort] = new uint32_t;
    *sProxy4CountMap[mPort] = 0;
    sConn5CountMap[mPort] = new uint32_t;
    *sConn5CountMap[mPort] = 0;
    sProxy5CountMap[mPort] = new uint32_t;
    *sProxy5CountMap[mPort] = 0;
    }
  }

PXMSocks::~PXMSocks()
  {
  }

void
PXMSocks::InitModule()
  {
  RegisterPXM(SOCKS4_SHORTNAME, mPort, sConn4CountMap[mPort], sProxy4CountMap[mPort]);
  RegisterPXM(SOCKS5_SHORTNAME, mPort, sConn5CountMap[mPort], sProxy5CountMap[mPort]);
  }

bool
PXMSocks::StartScan()
  {
  peak_task task = peak_task_self();
  
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = this->GetAddress();
  sin.sin_port = htons(mPort);
  
  mStream = peak_stream_socket_create((struct sockaddr *)&sin, sizeof(sin),
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
PXMSocks::Cleanup()
  {
  peak_release(mStream);
  }

void
PXMSocks::ProxyNotFound()
  {
  if (mStatus == SCAN_SOCKS4)
    {
    mStatus = SCAN_SOCKS5;
    this->StartScan();
    return;
    }
  
  PXScanModule::ProxyNotFound();
  }

void
PXMSocks::SendSocks4Request(peak_stream s)
  {
  struct socks4 socks4out = { S4_VN, S4_CMD_CONNECT, { 0, 0 },
                              { 0, 0, 0, 0 }, 0 };
  in_addr targetAddr = GetTargetAddress();
  uint16_t targetPort = htons(GetTargetPort());
  
  /* Set target address */
  memcpy(&socks4out.dstip, &targetAddr, sizeof(socks4out.dstip));
  memcpy(&socks4out.dstport, &targetPort, sizeof(socks4out.dstport));
  
  peak_stream_set_buffered(s, 1, sizeof(socks4out), sizeof(socks4out), NULL);
  peak_stream_write_buffered(s, &socks4out, sizeof(socks4out));
  }

void
PXMSocks::SendSocks5Request(peak_stream s)
  {
  struct socks5 socks5out = { S5_VN, 0x01, S5_R_METHOD_NOAUTH };
  
  peak_stream_set_buffered(s, 1, sizeof(socks5out), sizeof(socks5out), NULL);
  peak_stream_write_buffered(s, &socks5out, sizeof(socks5out));
  }

void
PXMSocks::ProcessEvent(peak_stream s, int type)
  {
  int err;

  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      if (mStatus == SCAN_NOSOCKS)
        {
        mStatus = SCAN_SOCKS4;
	(*sConn4CountMap[mPort])++;
        this->SendSocks4Request(s);
        }
      else
        {
        assert(mStatus == SCAN_SOCKS5);
	(*sConn5CountMap[mPort])++;
        this->SendSocks5Request(s);
        }
      break;
    case PEAK_STREAM_EVT_READ:
      if (mStatus == SCAN_SOCKS4)
        {
	int len;
	char line[8192];
	len = peak_stream_read(s, line, sizeof(line));

	if (ScannerSha256StringCheck(line,len,GetTargetSecret()))
          {
	  (*sProxy4CountMap[mPort])++;
          this->Cleanup();
          this->ProxyFound(OPAS_PROXY_TYPE_SOCKS4, mPort, SOCKS4_DESCR);
          return; /* done! */
          }
        if (mLinesTry++ < MAX_LINES_TRY)
          break;
        }
      else
        {
        // Socks v5
        struct socks5_response socks5in;
        if (peak_stream_read(s, &socks5in, sizeof(socks5in)) > 1)
          {
          if (socks5in.vn == S5_VN)
            {
            switch(socks5in.method)
              {
              case S5_R_METHOD_NOAUTH:
		(*sProxy5CountMap[mPort])++;
                this->Cleanup();
                this->ProxyFound(OPAS_PROXY_TYPE_SOCKS5, mPort, SOCKS5_DESCR);
                return; /* done! */
              case S5_R_METHOD_GSSAPI:
              case S5_R_METHOD_USERPW:
              case S5_R_METHOD_NOTACC:
              default:
                break;
              }
            }
          }
        }
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
PXMSocks::EventCallback(peak_stream s, int type, void *context)
  {
  PXMSocks *pxm = reinterpret_cast<PXMSocks*>(context);
  pxm->ProcessEvent(s, type);
  }

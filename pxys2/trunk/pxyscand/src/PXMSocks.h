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
// $Id: PXMSocks.h,v 1.3 2005/05/19 00:49:38 mbuna Exp $
//
#ifndef INCLUDED_PXMSOCKS_H_
#define INCLUDED_PXMSOCKS_H_

#include <map>

#include "PXScanModule.h"

#include <peak/peak.h>

using std::map;

class PXMSocks : public PXScanModule
  {
  // Status
  enum { SCAN_NOSOCKS, SCAN_SOCKS4, SCAN_SOCKS5 };
  
public:
  PXMSocks(PXScan *inScan, int inPort);
  virtual ~PXMSocks();
  
  void InitModule();
  bool StartScan();
  
  // Convenience redefinition
  virtual void ProxyNotFound();
  
protected:
  void Cleanup();
  void SendSocks4Request(peak_stream s);
  void SendSocks5Request(peak_stream s);
  void ProcessEvent(peak_stream s, int type);
  static void EventCallback(peak_stream s, int type, void *context);
  
  peak_stream mStream;
  int mStatus;
  int mLinesTry;
  int mPort;

  static map<uint16_t, uint32_t*> sConn4CountMap;
  static map<uint16_t, uint32_t*> sConn5CountMap;
  static map<uint16_t, uint32_t*> sProxy4CountMap;
  static map<uint16_t, uint32_t*> sProxy5CountMap;
  };

#endif // INCLUDED_PXMSOCKS_H_

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
// $Id: PXMHttpGet.h,v 1.1 2006/09/06 10:08:08 spale Exp $
//
#ifndef INCLUDED_PXMHTTPGET_H_
#define INCLUDED_PXMHTTPGET_H_

#include <map>

#include "PXScanModule.h"
#include "PXMutex.h"

#include <peak/peak.h>

using std::map;

class PXMHttpGet : public PXScanModule
  {
public:
  PXMHttpGet(PXScan *inScan, int inPort);
  virtual ~PXMHttpGet();
  
  void InitModule();
  bool StartScan();
  
protected:
  void Cleanup();
  void SendConnectRequest(peak_stream s);
  void CommitFound();
  void ProcessEvent(peak_stream s, int type);
  static void EventCallback(peak_stream s, int type, void *context);
  
  peak_stream mStream;
  int mLinesTry;
  int mPort;
  bool mIsProxy;
  PXMutex mLogMutex;
  
  static map<uint16_t, uint32_t*> sConnCountMap;
  static map<uint16_t, uint32_t*> sProxyCountMap;
  };

#endif // INCLUDED_PXMHTTPGET_H_

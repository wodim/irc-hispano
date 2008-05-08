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
// $Id: PXMTrojan.h,v 1.1.1.1 2003/12/30 17:09:00 mbuna Exp $
//
#ifndef INCLUDED_PXMTROJAN_H_
#define INCLUDED_PXMTROJAN_H_

#include <map>

#include "PXScanModule.h"
#include "PXMutex.h"

#include <peak/peak.h>

using std::map;

class PXMTrojan : public PXScanModule
  {
public:
  PXMTrojan(PXScan *inScan, int inPort);
  virtual ~PXMTrojan();
  
  void InitModule();
  bool StartScan();
  
protected:
  void Cleanup();
  void ProcessEvent(peak_stream s, int type);
  static void EventCallback(peak_stream s, int type, void *context);
  
  peak_stream mStream;
  int mPort;
  bool mIsTrojan;
  PXMutex mLogMutex;

  static map<uint16_t, uint32_t*> sConnCountMap;
  static map<uint16_t, uint32_t*> sTrojanCountMap;
  };

#endif // INCLUDED_PXMTROJAN_H_

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
// $Id: PXM3Com812Web.h,v 1.1.1.1 2003/12/30 17:09:00 mbuna Exp $
//
#ifndef INCLUDED_PXM3COM812WEB_H_
#define INCLUDED_PXM3COM812WEB_H_

#include "PXScanModule.h"

#include <peak/peak.h>

class PXM3Com812Web : public PXScanModule
  {
public:
  PXM3Com812Web(PXScan *inScan);
  virtual ~PXM3Com812Web();
  
  void InitModule();
  bool StartScan();
  
protected:
  void Cleanup();
  void ProcessEvent(peak_stream s, int type);
  static void EventCallback(peak_stream s, int type, void *context);
  
  peak_stream mStream;
  int mLinesTry;
  
  static uint32_t sConnCount;
  static uint32_t sProxyCount;
  };

#endif // INCLUDED_PXM3COM812WEB_H_

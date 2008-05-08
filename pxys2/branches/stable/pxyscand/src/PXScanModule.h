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
// $Id: PXScanModule.h,v 1.4 2006/09/11 22:02:54 spale Exp $
//
#ifndef INCLUDED_PXSCANMODULE_H_
#define INCLUDED_PXSCANMODULE_H_

#include <fstream>
#include <cstdlib>
#include <ctime>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <opas/opas.h>

using std::ofstream;

class PXScan;

class PXScanModule
  {
public:
  PXScanModule(PXScan *inScan);
  virtual ~PXScanModule();
  
  // Module interface (implement this when you create a new module)
  
  // You're advised to override this  and call RegisterPXM() to enable stats
  virtual void InitModule() {}
  
  // You only need to implement this to start the scan (see examples...)
  virtual bool StartScan() = 0;
  
  // --End of interface--
  
  // Recommended call in InitModule() (will enable stats for this module)
  
  void RegisterPXM(const char *inShortName, uint16_t inPort,
                   uint32_t *inConnCountPtr, uint32_t *inProxyCountPtr) const;
  
  // Utilities available for modules
  
  virtual in_addr GetAddress() const;
  
  virtual bool IsLocalAddressSet() const;
  virtual in_addr GetLocalAddress() const;
  
  virtual time_t GetTimeout() const;
  
  // You can use this target to perform your proxy scan
  virtual in_addr GetTargetAddress() const;
  virtual uint16_t GetTargetPort() const;
  virtual char* GetTargetSecret() const;
  
  // Special for http proxy agent logging
  virtual bool LogAgent() const;
  virtual ofstream& GetFAgent();
  
  // Finally, the result methods.. A scan modules should reply
  // (asynchronously) one of the 3 following methods
  
  // Not found.
  virtual void ProxyNotFound();
  // Proxy found, fill the info.
  virtual void ProxyFound(uint16_t inTypeOfProxy, uint16_t inPort,
                          const char *inDescr);
  // An fatal error occured (usually only ENETUNREACH)
  virtual void ScanError(int error);
  
private:
  PXScan *mScan;               // Opaque scan object, do not use.
  };

#endif // INCLUDED_PXSCANMODULE_H_

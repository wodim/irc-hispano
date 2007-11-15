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
// $Id: PXScan.h,v 1.2 2004/01/10 14:32:29 mbuna Exp $
//
#ifndef INCLUDED_PXSCAN_H_
#define INCLUDED_PXSCAN_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <list>
#include <vector>

#include <opas/opas.h>
#include <peak/peak.h>

#include "PXMutex.h"

using std::list;
using std::vector;

class PXSession;
class PXScanManager;
class PXScanModule;

class PXScan
  {
  friend class PXScanManager;
  friend class PXScanModule;
  
  struct PXScanQuery
    {
    opas_msg_query query;
    PXSession *session;
    };
  
  // Internal status
  enum { PXSCAN_QUEUED=1, PXSCAN_SCANNING, PXSCAN_PROXY_FOUND,  PXSCAN_ERROR };
  
public:
  PXScan(const opas_msg_query &inQuery, PXSession *inSession,
         PXScanManager *inScanManager);
  
  int GetMaxNeededFDs() const { return mModules.size(); }
  
  void AppendQuery(const opas_msg_query &inQuery, PXSession *inSession);
  bool IsSessionReferenced(const PXSession *inSession);
  void Schedule();
  
  // Called from the wrapper in PXScanModule
  void Completed(PXScanModule *inCompletedModule);
  
  void ProxyFound(uint16_t inTypeOfProxy, uint16_t inPort,
                  const char *inDescr);
  void Error(int error);
  
  bool ScanFlagIsSet(int inFlag) { return mScanFlags & inFlag; }
  void ScanFlagSet(int inFlag) { mScanFlags |= inFlag; }
  void ScanFlagClear(int inFlag) { mScanFlags &= ~inFlag; }
  void ScanFlagReset() { mScanFlags = 0; }
  
  in_addr GetAddress() const { return mScanQueryList.begin()->query.addr; }
  
protected:
  virtual ~PXScan();
  void InitModules();
  void RegisterModule(PXScanModule *inScanModule);
  void SneakModule(const char *inShortName, uint16_t inPort,
                   uint32_t *inConnCountPtr, uint32_t *inProxyCountPtr) const;
  
  // Real start of the scanning process, called by PXScanManager only.
  void Start();
  
  void Launch();
  void Done();
  void ProxyNotFound();
  
  void Lock() { mMutex.Lock(); }
  void Unlock() { mMutex.Unlock(); }
  
  void TimerStop();
  static void TimerCallback(peak_timer ti, void *context);
  
  list<PXScanQuery> mScanQueryList;
  PXScanManager *mScanManager;
  int mStatus;
  int mScanFlags;    // custom flags, reserved for the scan modules
  
  peak_timer mScanTimer;
  
  // Modules
  vector<PXScanModule*> mModules;
  vector<PXScanModule*>::const_iterator mModuleIterator;
  // Number of modules completed, for terminaison detection
  int mCompletedCount;
  int mCompletedMax;
  int mLaunched;
  
  // Mutex to control concurrent access on this class members
  PXMutex mMutex;
  
  static bool sInitialized;
  };

#endif // INCLUDED_PXSCAN_H_

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
// $Id: PXScanManager.h,v 1.2 2006/09/10 22:09:30 spale Exp $
//
#ifndef INCLUDED_PXSCANMANAGER_H_
#define INCLUDED_PXSCANMANAGER_H_

#include <deque>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "PXConfig.h"
#include "PXMutex.h"
#include "PXRepeater.h"

#include <peak/peak.h>

using std::queue;
using std::deque;
using std::ofstream;
using std::vector;

class PXConfig;
class PXScan;

typedef PXScan * PXScanPtr;

struct PXModuleInfo
  {
  PXModuleInfo(const char *inShortName, uint16_t inPort,
               uint32_t *inConnCountPtr, uint32_t *inProxyCountPtr)
    {
    shortName = peak_strdup(inShortName);
    port = inPort;
    connCountPtr = inConnCountPtr;
    proxyCountPtr = inProxyCountPtr;
    }
  ~PXModuleInfo() { peak_deallocate(shortName); }
  
  char *shortName;
  uint16_t port;
  uint32_t *connCountPtr;
  uint32_t *proxyCountPtr;
  };

class PXScanManager : public PXRepeater
  {
public:
  PXScanManager(PXConfig *inConfig);
  virtual ~PXScanManager();
  
  void Rehash();
  size_t GetModuleInfoCount() const { return mMInfo.size(); }
  void UseModule(PXModuleInfo *inNewMInfo) { mMInfo.push_back(inNewMInfo); }
  const PXModuleInfo *GetMInfo(int idx) const { return mMInfo[idx]; }
  
  bool AddScan(PXScan *inScan);
  void ScanCompleted(PXScan *inScan);
  
  in_addr GetTargetAddress() const { return mScanTarget.address; }
  uint16_t GetTargetPort() const { return (uint16_t)mScanTarget.port; }
  char *GetTargetSecret() const { return mScanTarget.secret; }
  time_t GetLastTargetCheck() const { return mLastTargetCheck; }
  
  ofstream& GetFAgent() { return mFAgent; }
  
  // Status readers (not sync):
  
  // Current number of IPs being scanned.
  int GetIPScanCount() const { return mIPScanCount; }
  // Max simultaneous scannable IPs
  int GetIPScanMax() const { return sNeededFDs ? mRunScanMax/sNeededFDs : 0; }
  // Current "allocated" socket scans (overestimation).
  int GetRunScanCount() const { return mRunScanCount; }
  // Max socket scans (from config)
  int GetRunScanMax() const { return mRunScanMax; }
  
  size_t GetScanQSize() const { return mScanQ.size(); }
  
  PXConfig *mConfig;
  
protected:
  void TargetCheck();
  void TargetCheckNext();
  void RepeaterFire();
  void TCEvent(peak_stream s, int type);
  
  static void TCEventCallback(peak_stream s, int type, void *context);
  
  ofstream mFAgent;
  vector<PXConfigScannerTarget> mScanTargets;
  vector<PXConfigScannerTarget>::const_iterator mScanTargetIterator;
  PXConfigScannerTarget mScanTarget;
  time_t mLastTargetCheck;
  int mIPScanCount, mRunScanMax, mRunScanCount;
  queue<PXScanPtr> mScanQ;
  PXMutex mMutex;
  vector<PXModuleInfo*> mMInfo;
  static int sNeededFDs;
  };

#endif // INCLUDED_PXSCANMANAGER_H_

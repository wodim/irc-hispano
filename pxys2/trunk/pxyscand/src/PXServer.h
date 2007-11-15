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
// $Id: PXServer.h,v 1.3 2004/01/10 14:32:29 mbuna Exp $
//
#ifndef INCLUDED_PXSERVER_H_
#define INCLUDED_PXSERVER_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXIPCacheT.h" // IP cache template class
#include "PXRepeater.h"
#include "PXServiceMsg.h"

#include <list>
#include <map>
#include <opas/opas.h>
#include <peak/peak.h>

#define MAX_SESSIONS 64

using std::list;

class PXConfig;
class PXCore;
class PXScan;
class PXScanManager;
class PXSession;

struct IPLess
  {
  bool operator()(const in_addr &x, const in_addr &y) const
    {
    return x.s_addr < y.s_addr;
    }
  };

struct PXProxyBinInfo
  {
  uint32_t cache_hits;
  uint16_t type;
  uint16_t port;
  char descr[100];
  };

class PXServer : public PXRepeater
  {
  typedef std::map<in_addr, PXScan*, IPLess> scanMapType;
  typedef scanMapType::value_type scanMapPairType;
  
public:
  PXServer(PXConfig *inConfig, PXScanManager *inScanManager, PXCore *inCore);
  ~PXServer();
  
  int Start();
  void Rehash();
  
  void SessionClosed(PXSession *inSession);
  void CreateStatus(PXSStatus **outStatus, size_t *outSize) const;
  void GetStats(PXSStats &oStats);
  
  void NewScanIP4(const opas_msg_query &inQuery, PXSession *inSession);
  void DeleteScanIP4(const opas_msg_query &inQuery);
  void ScanIP4ResultNoProxy(const opas_msg_query &inQuery);
  void ScanIP4ResultProxy(const opas_msg_query &inQuery,
                          uint16_t inTypeOfProxy, uint16_t inPort,
                          const char *inDescr);
  bool RemoveAddressFromCache(const in_addr &inAddress);
  bool RemoveAddressFromCache(const in6_addr &inAddress);
  
protected:
  void ProcessEvent(peak_stream s, int type);
  void EventRead(peak_stream s);
  void NewConnection(peak_stream s);
  
  void RepeaterFire();
  
  static void EventCallback(peak_stream s, int type, void *context);
  
  PXConfig *mConfig;
  PXScanManager *mScanManager;
  PXCore *mCore;
  
  PXIPCacheT<in_addr> * mIP4Cache;
  PXIPCacheT<in_addr> * mPX4Cache;
  
  peak_stream mLStream;
  peak_task_mutex mCacheMutex, mScanMutex;
  list<PXSession*> mSessions;
  uint32_t mSessionCount;
  uint32_t mSessionRejectedCount;
  scanMapType mScans;
  
  uint32_t mQueryCount;
  uint32_t mScannedCount;
  uint32_t mProxyCount;
  uint32_t mNoScanHits;
  uint32_t mIPCacheHits;
  uint32_t mPXCacheHits;
  uint32_t mInProgressCount;
  
  };

#endif // INCLUDED_PXSERVER_H_

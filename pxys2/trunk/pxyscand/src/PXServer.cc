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
#define RCSID "$Id: PXServer.cc,v 1.7 2006/09/04 12:47:18 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXServer.h"
#include "PXConfig.h"
#include "PXCore.h"
#include "PXScan.h"
#include "PXScanManager.h"
#include "PXSession.h"
#include <cassert>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::clog;
using std::endl;

#define CACHE_AUTOSAVE_DELAY 900

#define NOPROXY_CACHE_FILE "noproxy"
#define PROXY_CACHE_FILE   "proxy"

const uint32_t kProxyCacheSize = 1009; /* a prime number please */

PXServer::PXServer(PXConfig *inConfig, PXScanManager *inScanManager,
                   PXCore *inCore)
  : PXRepeater(CACHE_AUTOSAVE_DELAY),
    mConfig(inConfig), mScanManager(inScanManager), mCore(inCore),
    mSessionCount(0), mSessionRejectedCount(0), mQueryCount(0),
    mScannedCount(0), mProxyCount(0), mNoScanHits(0), mIPCacheHits(0),
    mPXCacheHits(0), mInProgressCount(0)
  {
  mCacheMutex = peak_task_mutex_create(peak_task_self());
  mScanMutex = peak_task_mutex_create(peak_task_self());
  
  // Initialize IPv4 cache (non-proxy)
  mIP4Cache = new PXIPCacheT<in_addr>(
    mConfig->cache.dir,
    htonl(0x50584346), /* 'PXCF' */
    mConfig->cache.maxips,
    mConfig->cache.expire,
    0 // no associated data
    );
  if (mIP4Cache->SwapIn(NOPROXY_CACHE_FILE))
    clog << "PXServer::PXServer: IP4Cache SwapIn failed" << endl;
  
  // Initialize IPv4 proxy cache
  mPX4Cache = new PXIPCacheT<in_addr>(
    mConfig->cache.dir,
    htonl(0x50585046), /* 'PXPF' */
    kProxyCacheSize,
    mConfig->cache.proxy_expire,
    sizeof(PXProxyBinInfo)
    );
  if (mPX4Cache->SwapIn(PROXY_CACHE_FILE))
    clog << "PXServer::PXServer: PX4Cache SwapIn failed" << endl;
  }

PXServer::~PXServer()
  {
  delete mPX4Cache;
  delete mIP4Cache;
  peak_release(mScanMutex);
  peak_release(mCacheMutex);
  }

int
PXServer::Start()
  {
  struct sockaddr_in sin;
  
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr = mConfig->opas.local_address;
  sin.sin_port = htons(mConfig->opas.port);
  
  mLStream = peak_stream_socket_create((struct sockaddr *)&sin, sizeof(sin),
                                       PEAK_STREAM_OPT_LISTEN
                                       |PEAK_STREAM_OPT_AUTOSCHEDULE,
                                       EventCallback,
                                       this);
  
  if (!mLStream)
    {
    clog << "PXServer::Start: peak_stream_socket_create failed" << endl;
    return -1;
    }
  return 0;
  }

void
PXServer::Rehash()
  {
  mIP4Cache->SwapOut(NOPROXY_CACHE_FILE);
  mPX4Cache->SwapOut(PROXY_CACHE_FILE);
  }

void
PXServer::SessionClosed(PXSession *inSession)
  {
  mSessions.remove(inSession);
  }

void
PXServer::CreateStatus(PXSStatus **outStatus, size_t *outSize) const
  {
  size_t n = mScanManager->GetModuleInfoCount();
  size_t size = sizeof(PXSStatus) + n * sizeof(PXSMInfo);
  
  PXSStatus *statusPtr;
  
  *outStatus = statusPtr = (PXSStatus *)peak_allocate(size);
  *outSize = size;
  memset(statusPtr, 0, size);
  
  statusPtr->head.sig = htonl(PXYSCAND_SIG);
  statusPtr->head.ver = htonl(PX_VERSION);
  statusPtr->head.cmd = htonl(PX_CMD_STATUS);
  
  statusPtr->curIPScan = htonl(mScanManager->GetIPScanCount());
  statusPtr->maxIPScan = htonl(mScanManager->GetIPScanMax());
  statusPtr->curRunScan = htonl(mScanManager->GetRunScanCount());
  statusPtr->maxRunScan = htonl(mScanManager->GetRunScanMax());
  statusPtr->scanQSize = htonl(mScanManager->GetScanQSize());
  
  statusPtr->targetAddr = mScanManager->GetTargetAddress();
  statusPtr->targetPort = htonl((uint32_t)mScanManager->GetTargetPort());
  
  statusPtr->targetLastCheck =
    htonl(peak_time() - mScanManager->GetLastTargetCheck());
  
  statusPtr->servScannedCount = htonl(mScannedCount);
  statusPtr->servProxyCount = htonl(mProxyCount);
  
  statusPtr->uptime = htonl(peak_time() - mCore->GetBirthTime());
  statusPtr->timeout = htonl(mConfig->scanner.timeout);
  
  statusPtr->numMInfo = htonl(n);
  
  for (size_t i = 0; i < n; i++)
    {
    PXSMInfo *info = (reinterpret_cast<PXSMInfo *>(statusPtr + 1) + i);
    const PXModuleInfo *minfo = mScanManager->GetMInfo(i);
    info->port = htonl(minfo->port);
    info->connCount = htonl(*minfo->connCountPtr);
    info->proxyCount = htonl(*minfo->proxyCountPtr);
    snprintf(info->shortName, sizeof(info->shortName), "%s", minfo->shortName);
    }
  }

void
PXServer::GetStats(PXSStats &oStats)
  {
  oStats.servQueryCount = htonl(mQueryCount);
  oStats.servScannedCount = htonl(mScannedCount);
  oStats.servProxyCount = htonl(mProxyCount);
  oStats.servNoScanHits = htonl(mNoScanHits);
  oStats.servIPCacheHits = htonl(mIPCacheHits);
  oStats.servPXCacheHits = htonl(mPXCacheHits);
  oStats.servInProgressCount = htonl(mInProgressCount);
  
  oStats.servIP4CacheCount = htonl(mIP4Cache->GetCount());
  oStats.servIP4CacheSize = htonl(mConfig->cache.maxips);
  oStats.servIP4CacheExpire = htonl(mConfig->cache.expire);
  oStats.servPX4CacheCount = htonl(mPX4Cache->GetCount());
  oStats.servPX4CacheSize = htonl(kProxyCacheSize);
  oStats.servPX4CacheExpire = htonl(mConfig->cache.proxy_expire);
  
  oStats.sessCurrent = htonl(mSessions.size());
  oStats.sessCount = htonl(mSessionCount);
  oStats.sessRejectedCount = htonl(mSessionRejectedCount);
  
  snprintf(oStats.version, sizeof(oStats.version), "pxyscand %s "
           "(service msg v%d)", VERSION, PX_VERSION);
  }

//  Query           Complete
// 
// Solution with 1 mutex
// 
// lock A         lock A
//  CHK CACHES     PUT IN CACHE
//  CHK SCANS      REMOVE SCAN
// unlock A       unlock A
// 
// Solution with 2 mutex [choosen]
// 
// lock A         lock A
//  CHK CACHES     PUT IN CACHE
// lock B         unlock A
// unlock A       lock B
//  CHK SCANS      REMOVE SCAN
// unlock B       unlock B


void
PXServer::NewScanIP4(const opas_msg_query &inQuery, PXSession *inSession)
  {
  void *pinfo;
  time_t t;
  
  mQueryCount++;
  
  if (mConfig->CheckNoScan(inQuery.addr))
    {
    mNoScanHits++;
    inSession->ScanResultNoProxy(inQuery, true);
    return;
    }
  
  peak_task_mutex_lock(mCacheMutex);   // lock the cache
  
  if ((t = mPX4Cache->Get(inQuery.addr, &pinfo)) > 0)
    {
    PXProxyBinInfo *infop = static_cast<PXProxyBinInfo*>(pinfo);
    // OMG!
    mPXCacheHits++;
    
    // Update cache hits count, just for the fun.
    infop->cache_hits = htonl(ntohl(infop->cache_hits) + 1);
    
    peak_task_mutex_unlock(mCacheMutex);
    
    inSession->ScanResultProxy(inQuery, true, t,
                               ntohs(infop->type), ntohs(infop->port),
                               infop->descr);
    return;
    }
  
  if ((t = mIP4Cache->Get(inQuery.addr, NULL)) > 0)
    {
    // In cache? If so it's quick!
    mIPCacheHits++;
    
    peak_task_mutex_unlock(mCacheMutex);
    
    inSession->ScanResultNoProxy(inQuery, true);
    return;
    }
  
  peak_task_mutex_lock(mScanMutex);    // lock first (important)
  peak_task_mutex_unlock(mCacheMutex); // ok, others can consult the cache
  
  // Search for existing scan session for this IP...
  PXScan *scan;
  bool newScan;
  scanMapType::const_iterator i = mScans.find(inQuery.addr);
  
  if (i == mScans.end())
    {
    // Normal case, I suppose. Create new "scan" instance and remember it.
    scan = new PXScan(inQuery, inSession, mScanManager);
    mScans.insert(scanMapPairType(inQuery.addr, scan));
    newScan = true;
    }
  else
    {
    // Found... do not scan twice.
    mInProgressCount++;
    scan = i->second;
    if (!scan->IsSessionReferenced(inSession))
      scan->AppendQuery(inQuery, inSession);
    newScan = false;
    }
  
  peak_task_mutex_unlock(mScanMutex);  // "secure!"
  
  if (newScan)
    scan->Schedule();
  }

void
PXServer::DeleteScanIP4(const opas_msg_query &inQuery)
  {
  peak_task_mutex_lock(mScanMutex);
  
  mScannedCount++;
  
  scanMapType::iterator i = mScans.find(inQuery.addr);
  assert(i != mScans.end());
  mScans.erase(i);
  
  peak_task_mutex_unlock(mScanMutex);
  }

void
PXServer::ScanIP4ResultNoProxy(const opas_msg_query &inQuery)
  {
  peak_task_mutex_lock(mCacheMutex);
  mIP4Cache->Put(inQuery.addr, peak_time(), NULL);
  peak_task_mutex_unlock(mCacheMutex); // safe
  
  this->DeleteScanIP4(inQuery);
  }

void
PXServer::ScanIP4ResultProxy(const opas_msg_query &inQuery,
                             uint16_t inTypeOfProxy, uint16_t inPort,
                             const char *inDescr)
  {
  PXProxyBinInfo info;
  memset(&info, 0, sizeof(info));
  info.cache_hits = htonl(0);
  info.type = htons(inTypeOfProxy);
  info.port = htons(inPort);
  strncpy(info.descr, inDescr, sizeof(info.descr) - 1);
  
  peak_task_mutex_lock(mCacheMutex);
  mPX4Cache->Put(inQuery.addr, peak_time(), &info);
  peak_task_mutex_unlock(mCacheMutex);
  
  mProxyCount++;
  
  this->DeleteScanIP4(inQuery);
  }

bool
PXServer::RemoveAddressFromCache(const in_addr &inAddress)
  {
  peak_task_mutex_lock(mCacheMutex);
  bool res = mPX4Cache->Invalidate(inAddress);
  mIP4Cache->Invalidate(inAddress); // Better safe than sorry.
  peak_task_mutex_unlock(mCacheMutex);
  return res;
  }

bool
PXServer::RemoveAddressFromCache(const in6_addr &inAddress)
  {
  bool res = false;
#if 0
  peak_task_mutex_lock(mCacheMutex);
  res = mPX6Cache->Invalidate(inAddress);
  mIP6Cache->Invalidate(inAddress);
  peak_task_mutex_unlock(mCacheMutex);
#endif
  return res;
  }

void
PXServer::ProcessEvent(peak_stream s, int type)
  {
  if (type == PEAK_STREAM_EVT_ACCEPT)
    {
    this->NewConnection(s);
    }
  else
    {
    assert(0);
    }
  }

void
PXServer::NewConnection(peak_stream s)
  {
  peak_stream ns;
  struct sockaddr_in sin;
  int len = sizeof(sin);
  
  if (!(ns = peak_stream_accept_create(s, PEAK_STREAM_OPT_DEFAULT,
                                       PXSession::EventCallback, this)))
    return;

  if (mSessions.size() >= MAX_SESSIONS)
    {
    peak_release(ns);
    mSessionRejectedCount++;
    return;
    }
  
  // Limit DoS attacks... check the peer ASAP (it's already too late!)
  peak_stream_get_address(ns, (struct sockaddr *)&sin, &len);
  
  if (!mConfig->OPASIsAllowed(sin.sin_addr))
    {
    peak_release(ns); // Reject
    mSessionRejectedCount++;
    return;
    }
  
  mSessions.push_back(new PXSession(this, ns));
  mSessionCount++;
  }

void
PXServer::RepeaterFire()
  {
  peak_task_mutex_lock(mCacheMutex);
  this->Rehash();
  peak_task_mutex_unlock(mCacheMutex);
  }

void
PXServer::EventCallback(peak_stream s, int type, void *context)
  {
  ((PXServer *)context)->ProcessEvent(s, type);
  }

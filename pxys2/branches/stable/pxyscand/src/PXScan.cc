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
#define RCSID "$Id: PXScan.cc,v 1.8 2006/09/06 10:09:22 spale Exp $"

#define SCAN_TIMING 1.0

#include "PXScan.h"
#include "PXScanManager.h"
#include "PXSession.h"

/* Module headers */
#include "PXMWingate.h"
#include "PXMSocks.h"
#include "PXMHttpProxy.h"
#include "PXMHttpGet.h"
#include "PXMCrazyBandit.h"
#include "PXM3Com812.h"
#include "PXM3Com812Web.h"
#include "PXMTrojan.h"
#include "PXMTheOnionRouter.h"
#include "PXMBouncerHispano.h"

#include <algorithm>
#include <cassert>
#include <peak/peak.h>

bool PXScan::sInitialized = false;

PXScan::PXScan(const opas_msg_query &inQuery, PXSession *inSession,
               PXScanManager *inScanManager)
  : mScanManager( inScanManager ), mStatus(0), mScanFlags(0), mScanTimer(NULL)
  {
  size_t nmods = mScanManager->mConfig->scanner.modules.size();
  
  // Optimize memory allocation.
  mModules.reserve(nmods);
  
  // Instantiate and register our scan modules
  for (size_t i = 0; i < nmods; i++)
    {
    PXConfigModule *mod = &mScanManager->mConfig->scanner.modules[i];
    switch (mod->id)
      {
      case CONFIG_MODULE_HTTP:
        this->RegisterModule(new PXMHttpProxy(this, mod->port));
        this->RegisterModule(new PXMHttpGet(this, mod->port));
        break;
      case CONFIG_MODULE_WINGATE:
        this->RegisterModule(new PXMWingate(this));
        break;
      case CONFIG_MODULE_SOCKS:
        this->RegisterModule(new PXMSocks(this, mod->port));
        break;
      case CONFIG_MODULE_CRAZYBANDIT:
        this->RegisterModule(new PXMCrazyBandit(this));
        break;
      case CONFIG_MODULE_3COM812:
        this->RegisterModule(new PXM3Com812(this));
        break;
      case CONFIG_MODULE_3COM812WEB:
        this->RegisterModule(new PXM3Com812Web(this));
        break;
#if 0
      case CONFIG_MODULE_TROJAN:
        this->RegisterModule(new PXMTrojan(this, mod->port));
        break;
#endif
      case CONFIG_MODULE_TOR:
        this->RegisterModule(new PXMTheOnionRouter(this, mod->port));
        break;
      case CONFIG_MODULE_BOUNCERHISPANO:
        this->RegisterModule(new PXMBouncerHispano(this, mod->port));
        break;
      default:
        abort();
      }
    }
  
  if (sInitialized)
    {
    // Shuffle scans order; an idea of Jeb.
    std::random_shuffle(mModules.begin(), mModules.end());
    }
  else
    {
    sInitialized = true;
    this->InitModules();
    }
  
  // And now store our first query for this scan.
  this->AppendQuery(inQuery, inSession);
  }

PXScan::~PXScan()
  {
  assert(mScanTimer == NULL);
  
  mScanManager->ScanCompleted(this);
  
  // Free resources allocated by the scan modules.
  for (int i = mModules.size(); i--; )
    delete mModules[i];
  }

void
PXScan::RegisterModule(PXScanModule *inScanModule)
  {
  /* We store instantiated modules in a vector. */
  mModules.push_back(inScanModule);
  }

void
PXScan::InitModules()
  {
  for (size_t i = 0; i < mModules.size(); i++)
    mModules[i]->InitModule();
  }

void
PXScan::SneakModule(const char *inShortName, uint16_t inPort,
                    uint32_t *inConnCountPtr, uint32_t *inProxyCountPtr) const
  {
  mScanManager->UseModule(new PXModuleInfo(inShortName,
                                           inPort,
                                           inConnCountPtr,
                                           inProxyCountPtr));
  }

void
PXScan::AppendQuery(const opas_msg_query &inQuery, PXSession *inSession)
  {
  PXScanQuery query = { inQuery, inSession };
  mScanQueryList.push_back(query);
  }

bool
PXScan::IsSessionReferenced(const PXSession *inSession)
  {
  list<PXScanQuery>::const_iterator i = mScanQueryList.begin();
  for (; i != mScanQueryList.end(); i++)
    if ((*i).session == inSession)
      return true;
  return false;
  }

void
PXScan::Schedule()
  {
  if (!mScanManager->AddScan(this))
    {
    this->Error(1000);
    this->Done();
    }
  }

void
PXScan::Start()
  {
  // Sanity check, no module is no fun.
  assert(mModules.size() > 0);
  
  // Initialize stuffs. Iterator points to first module.
  mModuleIterator = mModules.begin();
  mCompletedCount = 0;
  mCompletedMax = mModules.size();
  mLaunched = 0;
  mStatus = PXSCAN_SCANNING;
  
  // Prepare for next scan, with a little delay
  // 
  mScanTimer = peak_timer_create(SCAN_TIMING, SCAN_TIMING, // repeat!
                                 TimerCallback, this);
  if (!mScanTimer)
    abort();
  
  peak_task_timer_add(peak_task_self(), mScanTimer);
  peak_release(mScanTimer);
  
  // Launch first scan type.
  this->Launch();
  }

void
PXScan::Launch()
  {
  if (mModuleIterator == mModules.end())
    {
    // All scan types are now launched.
    mMutex.Lock();
    this->TimerStop();
    mMutex.Unlock();
    return;
    }
  
  mMutex.Lock();
  if (mStatus == PXSCAN_SCANNING)
    {
    // Normal way.
    mLaunched++;
    mMutex.Unlock();
    
    if (!(*mModuleIterator)->StartScan())
      {
      this->Error(1000);
      this->Completed(*mModuleIterator);
      }
    else
      mModuleIterator++;
    }
  else
    {
    // A proxy has already been found or an error occured.
    if (mCompletedCount == mLaunched)
      {
      // Special case: all launched modules have been completed!
      this->TimerStop();
      mMutex.Unlock();
      // -serialized-
      this->Done(); // We are done.
      }
    else
      {
      mCompletedMax = mLaunched; // Done at next module completion
      this->TimerStop();
      mMutex.Unlock();
      }
    }
  }

void
PXScan::Completed(PXScanModule * /* inCompletedModule */)
  {
  mMutex.Lock();
  assert(mCompletedCount < mCompletedMax);
  if (++mCompletedCount == mCompletedMax)
    {
    this->TimerStop();
    mMutex.Unlock();
    // -serialized-
    this->Done(); // We are done.
    return;
    }
  mMutex.Unlock();
  }

void
PXScan::Done()
  {
  if (mStatus == PXSCAN_SCANNING)
    this->ProxyNotFound();
  
  delete this;
  }

void
PXScan::ProxyNotFound()
  {
  list<PXScanQuery>::iterator i = mScanQueryList.begin();
  i->session->ScanCompletedNoProxy(i->query);
  
  for (; i != mScanQueryList.end(); i++)
    i->session->ScanResultNoProxy(i->query, false);
  }

void
PXScan::ProxyFound(uint16_t inTypeOfProxy, uint16_t inPort,
                   const char *inDescr)
  {
  mMutex.Lock();
  if (mStatus == PXSCAN_SCANNING)
    {
    mStatus = PXSCAN_PROXY_FOUND;
    
    list<PXScanQuery>::iterator i = mScanQueryList.begin();
    i->session->ScanCompletedProxy(i->query, inTypeOfProxy, inPort, inDescr);
    
    for (; i != mScanQueryList.end(); i++)
      i->session->ScanResultProxy(i->query, false, peak_time(),
                                  inTypeOfProxy, inPort,
                                  inDescr);
    }
  mMutex.Unlock();
  }

void
PXScan::Error(int error)
  {
  mMutex.Lock();
  if (mStatus == PXSCAN_SCANNING)
    {
    mStatus = PXSCAN_ERROR;
    
    list<PXScanQuery>::iterator i = mScanQueryList.begin();
    i->session->ScanCompletedWithError(i->query);
    
    for (; i != mScanQueryList.end(); i++)
      i->session->ScanResultError(i->query, error);
    }
  mMutex.Unlock();
  }

void
PXScan::TimerStop()
  {
  if (mScanTimer)
    {
    peak_task_timer_remove(peak_task_self(), mScanTimer); /* auto release */
    mScanTimer = NULL;
    }
  }

void
PXScan::TimerCallback(peak_timer ti, void *context)
  {
  PXScan *scan = reinterpret_cast<PXScan*>(context);
  
  assert(scan->mScanTimer == NULL || ti == scan->mScanTimer);
  
  scan->Launch();
  }

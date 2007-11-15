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
#define RCSID "$Id: PXScanManager.cc,v 1.4 2004/01/16 18:05:37 mbuna Exp $"

#include "PXScanManager.h"
#include "PXScan.h"
#include "PXConfig.h"

#include <iostream>
#include <cassert>
#include <cstring>

using std::clog;
using std::endl;
using std::deque;
using std::queue;
using std::ios;

int PXScanManager::sNeededFDs = 0;

PXScanManager::PXScanManager(PXConfig *inConfig)
  : PXRepeater(inConfig->scanner.target_check), // Setup the target check timer
    mConfig(inConfig), mLastTargetCheck(0), mIPScanCount(0), mRunScanCount(0)
  {
  mRunScanMax = mConfig->scanner.maxscans;
  assert(mRunScanMax > 0);
  
  if (mConfig->scanner.log_agent && *mConfig->scanner.log_agent)
    {
    mFAgent.open(mConfig->scanner.log_agent, ios::out | ios::app);
    if (!mFAgent.is_open())
      clog << "Cannot open agent log file: " << mConfig->scanner.log_agent
           << endl;
    }
  
  // The scan manager handles target assignment. Because socks4, http proxy,
  // and other proxy checks need to specify a target host, we want to be sure
  // it's up. So this class regularly tries a (direct) connection.
  // Interval delay is configurable but shouldn't be too low to avoid unwanted
  // DoS. However, it's only a connection and then we close immediately...
  
  // Check first time
  mScanTarget.port = -1;
  mScanTargets = mConfig->scanner.targets; // copy targets
  mScanTargetIterator = mScanTargets.begin();
  this->TargetCheck();
  }

PXScanManager::~PXScanManager()
  {
  }

void
PXScanManager::Rehash()
  {
  peak_timer_configure(mRepeaterTimer, 10, mConfig->scanner.target_check);
  }

bool
PXScanManager::AddScan(PXScan *inScan)
  {
  if (sNeededFDs == 0)
    sNeededFDs = inScan->GetMaxNeededFDs();
  
  if (mScanTarget.port < 0)
    return false;
  
  mMutex.Lock();
  if (mRunScanCount + sNeededFDs <= mRunScanMax)
    {
    mIPScanCount++;
    mRunScanCount += sNeededFDs;
    mMutex.Unlock();
    inScan->Start();
    }
  else
    {
    // Queue needed
    inScan->mStatus = PXScan::PXSCAN_QUEUED;
    mScanQ.push(inScan);
    mMutex.Unlock();
    }
  return true;
  }

void
PXScanManager::ScanCompleted(PXScan *inScan)
  {
  mMutex.Lock();
  if (mScanQ.size() > 0)
    {
    PXScan *queuedScan = mScanQ.front();
    mScanQ.pop();
    mMutex.Unlock();
    
    queuedScan->Start();
    }
  else
    {
    // If we don't launch another scan, we need to decrease these values.
    mIPScanCount--;
    mRunScanCount -= sNeededFDs;
    mMutex.Unlock();
    }
  }

void
PXScanManager::TargetCheck()
  {
  peak_stream s;
  int timeout;
  
  sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = mScanTargetIterator->address;
  sin.sin_port = htons((uint16_t)mScanTargetIterator->port);
  
  s = peak_stream_socket_create((struct sockaddr *)&sin, sizeof(sin),
                                PEAK_STREAM_OPT_DEFAULT,
                                TCEventCallback,
                                this);
  
  if (!s)
    return;
  
  if (mConfig->HasSourcePool())
    {
    sockaddr_in local_sin;
    memset(&local_sin, 0, sizeof(local_sin));
    local_sin.sin_family = AF_INET;
    local_sin.sin_addr = mConfig->GetOneFromSourcePool();
    local_sin.sin_port = htons(0);
    peak_stream_set_address(s, (sockaddr*)&local_sin, sizeof(local_sin));
    }
  
  timeout = mConfig->scanner.timeout / 2;
  if (timeout < 1) // bah
    timeout = 1;
  
  peak_stream_set_timeout(s, timeout);
  if (peak_stream_connect(s) == -1)
    {
    peak_release(s);
    this->TargetCheckNext();
    return;
    }
  peak_stream_schedule(s, peak_task_self());
  }

void
PXScanManager::TargetCheckNext()
  {
  // try next target
  mScanTargetIterator++;
  
  if (mScanTargetIterator != mScanTargets.end())
    this->TargetCheck();
  else
    this->Rehash(); // retry in a few from the beginning
  }

void
PXScanManager::RepeaterFire()
  {
  peak_task_exclusivity();
  mScanTargets.clear();
  mScanTargets = mConfig->scanner.targets; // copy targets
  mScanTargetIterator = mScanTargets.begin();
  
  this->TargetCheck();
  }

void
PXScanManager::TCEvent(peak_stream s, int type)
  {
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      /* ok ! */
      mScanTarget = *mScanTargetIterator; // copy
      mLastTargetCheck = peak_time();
      peak_release(s);
      break;
    case PEAK_STREAM_EVT_END:
    case PEAK_STREAM_EVT_ERROR:
    case PEAK_STREAM_EVT_TIMEDOUT:
      peak_release(s);
      this->TargetCheckNext();
      break;
    }
  }

void
PXScanManager::TCEventCallback(peak_stream s, int type, void *context)
  {
  PXScanManager *me = reinterpret_cast<PXScanManager*>(context);
  me->TCEvent(s, type);
  }


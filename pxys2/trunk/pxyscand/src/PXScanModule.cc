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
#define RCSID "$Id: PXScanModule.cc,v 1.4 2006/09/11 22:02:54 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXScanModule.h"
#include "PXScan.h"
#include "PXScanManager.h"
#include "PXConfig.h"
#include <iostream>

using std::clog;
using std::endl;

PXScanModule::PXScanModule(PXScan *inScan)
  : mScan(inScan)
  {
  }

PXScanModule::~PXScanModule()
  {
  }

void
PXScanModule::RegisterPXM(const char *inShortName,
                          uint16_t inPort,
                          uint32_t *inConnCountPtr,
                          uint32_t *inProxyCountPtr) const
  {
  mScan->SneakModule(inShortName, inPort, inConnCountPtr, inProxyCountPtr);
  }

/* Cheap wrapper to hide class PXScan and others from the modules */
in_addr
PXScanModule::GetAddress() const
  {
  return mScan->GetAddress();
  }

bool
PXScanModule::IsLocalAddressSet() const
  {
  return mScan->mScanManager->mConfig->HasSourcePool();
  }

in_addr
PXScanModule::GetLocalAddress() const
  {
  return mScan->mScanManager->mConfig->GetOneFromSourcePool();
  }

time_t
PXScanModule::GetTimeout() const
  {
  return mScan->mScanManager->mConfig->scanner.timeout;
  }

in_addr
PXScanModule::GetTargetAddress() const
  {
  return mScan->mScanManager->GetTargetAddress();
  }

uint16_t
PXScanModule::GetTargetPort() const
  {
  return mScan->mScanManager->GetTargetPort();
  }

char *
PXScanModule::GetTargetSecret() const
  {
  return mScan->mScanManager->GetTargetSecret();
  }

bool
PXScanModule::LogAgent() const
  {
  return mScan->mScanManager->GetFAgent().is_open();
  }

ofstream&
PXScanModule::GetFAgent()
  {
  return mScan->mScanManager->GetFAgent();
  }

void
PXScanModule::ProxyNotFound()
  {
  mScan->Completed(this);
  }

void
PXScanModule::ProxyFound(uint16_t inTypeOfProxy, uint16_t inPort,
                         const char *inDescr)
  {
  mScan->ProxyFound(inTypeOfProxy, inPort, inDescr);
  mScan->Completed(this);
  }

void
PXScanModule::ScanError(int error)
  {
  mScan->Error(error);
  mScan->Completed(this);
  }

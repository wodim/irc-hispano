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
// $Id: PXCore.h,v 1.1.1.1 2003/12/30 17:09:00 mbuna Exp $
//
#ifndef INCLUDED_PXCORE_H_
#define INCLUDED_PXCORE_H_

#include <peak/peak.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class PXConfig;
class PXScanManager;
class PXServer;

class PXCore
  {
public:
  PXCore(int argc, char *argv[]);
  ~PXCore();
  
  int Start();
  void Stop();
  
  time_t GetBirthTime() const { return mBirthTime; }
  
  PXConfig *const GetConfig() const { return mConfig; }
  
protected:
  void ParseArguments(int argc, char *argv[]);
  void SetMaxFDs();
  void InitializeSignals();
  static void SignalsCallback(peak_signal s, int sig, void *context);
  void DisplayUsage() const;
  void DisplayVersion() const;
  void LogPID() const;
  
  time_t mBirthTime;
  int mOptions;
  char * mConfigFile;
  PXConfig * mConfig;
  PXScanManager * mScanManager;
  PXServer * mServer;
  };

#endif // INCLUDED_PXCORE_H_

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
#define RCSID "$Id: PXCore.cc,v 1.2 2004/01/02 03:08:16 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXCore.h"
#include "PXConfig.h"
#include "PXConfigLoader.h"
#include "PXScanManager.h"
#include "PXServer.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

using std::clog;
using std::endl;

enum {
  OPT_FORK = 1 << 0,
  OPT_CHKCFG = 1 << 1,
  OPT_DLMAX = 1 << 2
  };

PXCore::PXCore(int argc, char *argv[])
  {
  this->ParseArguments(argc, argv);
  
  // Pre fork
  if ((mOptions & OPT_FORK) && fork())
    exit(0);
  
  mBirthTime = time(0);
  srand((unsigned int)mBirthTime ^ getpid());
  
  try
    {
    PXConfigLoader *cfgLoader  = new PXConfigLoader(mConfigFile);
    mConfig = cfgLoader->Load();
    }
  catch (PXXMLException &e)
    {
    clog << "Failed to load configuration file \"" << mConfigFile
         << "\":" << endl;
    clog << e.GetErrorString() << " - hint: " << e.GetHint() << endl;
    exit(EXIT_FAILURE);
    }
  
  this->SetMaxFDs();
  }

PXCore::~PXCore()
  {
  free(mConfigFile);
  }

void
PXCore::ParseArguments(int argc, char *argv[])
  {
  extern char *optarg;
  int c, options = OPT_FORK; /* default */

  while ((c = getopt(argc, argv, "cdf:tv")) != -1)
    {
    switch (c)
      {
      case 'c':
        options = (options | OPT_CHKCFG) & ~OPT_FORK;
        break;
      case 'd':
        options |= OPT_DLMAX;
        break;
      case 'f':
        mConfigFile = strdup(optarg);
        break;
      case 't':
        options &= ~OPT_FORK;
        break;
      case 'v':
        DisplayVersion();
        break;
      case '?':
      default:
        DisplayUsage();
        break;
      }
    }
  
  if (!mConfigFile)
    DisplayUsage();
  
  mOptions = options;
  }

void
PXCore::SetMaxFDs()
  {
  int val = mConfig->scanner.maxscans + MAX_SESSIONS;
  if (peak_task_set_info(peak_task_self(), PEAK_TASK_FLAVOR_MAXFDS, &val) != 0)
    {
    clog << "PXCore::SetMaxFDs: failed to set PEAK_TASK_FLAVOR_MAXFDS" << endl;
    clog << "Maybe reduce <maxscans> ?" << endl;
    exit(1);
    }
  }

int
PXCore::Start()
  {
  if (mOptions & OPT_CHKCFG)
    {
    PXConfigLoader::Dump(mConfig);
    return EXIT_SUCCESS;
    }
  
  this->InitializeSignals();
  this->LogPID();
  
  mScanManager = new PXScanManager(mConfig);
  
  mServer = new PXServer(mConfig, mScanManager, this);
  if (mServer->Start() != 0)
    return EXIT_FAILURE;
  
  // Finish daemonize
  if (mOptions & OPT_FORK)
    {
    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    setsid();
    }
  
  // And run !
  peak_task_run(peak_task_self());
  return 0;
  }

void
PXCore::Stop()
  {
  mServer->Rehash(); // Save caches
  peak_task_break(peak_task_self());
  }

void
PXCore::InitializeSignals()
  {
  int sn[3] = { SIGHUP, SIGINT, SIGTERM };
  
  for (int i = 0; i < 3; i++)
    peak_signal_schedule(peak_signal_create(sn[i], SignalsCallback, this),
                         peak_task_self());
  
  peak_signal_ignore(SIGPIPE);
  }

void
PXCore::SignalsCallback(peak_signal s, int sig, void *context)
  {
  PXCore *core = (PXCore *)context;
  
  peak_task_exclusivity();
  
  switch (sig)
    {
    case SIGHUP:
      core->mServer->Rehash(); // Save caches
      // TODO: reload conf
      break;
    case SIGINT:
      if (core->mOptions & OPT_FORK)
        {
        clog << "Received SIGINT. Use SIGTERM to terminate pxyscand." << endl;
        break;
        }
    case SIGTERM:
      core->Stop();
      break;
    }
  }

void
PXCore::DisplayUsage() const
  {
  clog << "*** Usage: pxyscand [-ctv] -f <config_file>" << endl;
  clog << "***   -c      check configuration file" << endl;
  clog << "***   -t      run in foreground" << endl;
  clog << "***   -v      show version and exit" << endl;
  exit(EXIT_FAILURE);
  }

void
PXCore::DisplayVersion() const
  {
  clog << "*** pxyscand " << VERSION << endl; // TODO
  clog << "*** Part of pxys v.2 software suite" << endl;
  clog << "*** " << RCSID << endl;
  exit(EXIT_SUCCESS);
  }

void
PXCore::LogPID() const
  {
  std::ofstream f("pxyscand.pid");
  if (f)
    f << (int)getpid() << endl;
  else
    clog << "PXCore::LogPID: pid file open failure" << endl;
  }

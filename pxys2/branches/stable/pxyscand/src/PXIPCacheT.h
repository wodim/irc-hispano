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
// $Id: PXIPCacheT.h,v 1.3 2006/09/04 12:47:18 spale Exp $
//
#ifndef INCLUDED_PXIPCACHET_H_
#define INCLUDED_PXIPCACHET_H_

// IPCache template class wrapper on pxys2/ipcache for pxyscand.
// To cache IPv4 or IPv6 associated with any binary info. Features basic
// swapin/swapout methods to save/restore to a file.
// mbuna, 2003-12-12

#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <sys/param.h>
#include <peak/alloc.h>
#include <peak/time.h>

#include "ipcache.h"

template<typename Addr>
class PXIPCacheT
  {
public:
  PXIPCacheT(const char *cachedir, uint32_t sig, int size, int expiredelay,
             size_t datasize);
  virtual ~PXIPCacheT();
  
  virtual time_t Get(const Addr &addr, void **oinfo);
  virtual void Put(const Addr &addr, time_t t, const void *info);
  virtual bool Invalidate(const Addr &addr);
  virtual size_t GetCount();
  virtual void Cleanup();
  
  virtual int SwapIn(const char *inFileName);
  virtual int SwapOut(const char *inFileName);
  
protected:
  
  ipcache_t    mIPCache;
  char *       mCacheDir;
  uint32_t     mSig;
  int          mExpireDelay;
  };

template<typename Addr>
PXIPCacheT<Addr>::~PXIPCacheT()
  {
  ipcache_dispose(mIPCache);
  peak_deallocate(mCacheDir);
  }

template<typename Addr>
inline size_t
PXIPCacheT<Addr>::GetCount()
  {
  this->Cleanup();
  return ipcache_get_count(mIPCache);
  }

template<typename Addr>
inline void
PXIPCacheT<Addr>::Cleanup()
  {
  ipcache_cleanup(mIPCache, peak_time() - mExpireDelay);
  }

template<typename Addr>
int
PXIPCacheT<Addr>::SwapIn(const char *inFileName)
  {
  char path[MAXPATHLEN];
  snprintf(path, sizeof(path), "%s/%s", mCacheDir, inFileName);
  return ipcache_swapin(mIPCache, path, mSig);
  }

template<typename Addr>
int
PXIPCacheT<Addr>::SwapOut(const char *inFileName)
  {
  char path[MAXPATHLEN];
  snprintf(path, sizeof(path), "%s/%s", mCacheDir, inFileName);
  this->Cleanup();
  return ipcache_swapout(mIPCache, path, mSig);
  }


#endif // INCLUDED_PXIPCACHET_H_

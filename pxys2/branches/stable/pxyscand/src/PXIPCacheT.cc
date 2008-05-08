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
#define RCSID "$Id: PXIPCacheT.cc,v 1.1.1.1 2003/12/30 17:09:00 mbuna Exp $"

#include "PXIPCacheT.h"
#include <peak/peak.h>

// Specialized methods

template<>
PXIPCacheT<in_addr>::PXIPCacheT(const char *cachedir, uint32_t sig, int size,
                                int expiredelay, size_t datasize)
  : mSig(sig), mExpireDelay(expiredelay)
  {
  mCacheDir = peak_strdup(cachedir);
  mIPCache = ipcache_create_in4(size, datasize);
  }

template<>
PXIPCacheT<in6_addr>::PXIPCacheT(const char *cachedir, uint32_t sig, int size,
                                 int expiredelay, size_t datasize)
  : mSig(sig), mExpireDelay(expiredelay)
  {
  mCacheDir = peak_strdup(cachedir);
  mIPCache = ipcache_create_in6(size, datasize);
  }

template<>
inline time_t
PXIPCacheT<in_addr>::Get(const in_addr &addr, void **oinfo)
  {
  this->Cleanup();
  return ipcache_find_in4(mIPCache, &addr, oinfo);
  }

template<>
inline time_t
PXIPCacheT<in6_addr>::Get(const in6_addr &addr, void **oinfo)
  {
  this->Cleanup();
  return ipcache_find_in6(mIPCache, &addr, oinfo);
  }

template<>
inline void
PXIPCacheT<in_addr>::Put(const in_addr &addr, time_t t, const void *info)
  {
  ipcache_add_in4(mIPCache, &addr, t, info);
  }

template<>
inline void
PXIPCacheT<in6_addr>::Put(const in6_addr &addr, time_t t, const void *info)
  {
  ipcache_add_in6(mIPCache, &addr, t, info);
  }

template<>
inline bool
PXIPCacheT<in_addr>::Invalidate(const in_addr &addr)
  {
  return ipcache_invalidate_in4(mIPCache, &addr) == 0;
  }

template<>
inline bool
PXIPCacheT<in6_addr>::Invalidate(const in6_addr &addr)
  {
  return ipcache_invalidate_in6(mIPCache, &addr) == 0;
  }

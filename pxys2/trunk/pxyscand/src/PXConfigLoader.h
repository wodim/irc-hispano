/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyscand (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef INCLUDED_PXCONFIGLOADER_H_
#define INCLUDED_PXCONFIGLOADER_H_

#include "PXXMLXPathLoaderImp.h"

class PXConfig;

class PXConfigLoader : protected PXXMLXPathLoaderImp
  {
public:
  PXConfigLoader(const char *inFile);
  virtual ~PXConfigLoader();
  
  static bool StringToNetworkNetmask(const char *str,
                                     in_addr &oNet,
                                     in_addr &oMask);
  virtual PXConfig * Load();
  static void Dump(PXConfig *cfg);
  
protected:
  virtual void DoLoad(PXConfig *cfg);
  };

#endif /* INCLUDED_PXCONFIGLOADER_H_ */

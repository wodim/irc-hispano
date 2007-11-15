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
// $Id: PXRepeater.h,v 1.1.1.1 2003/12/30 17:09:00 mbuna Exp $
//
#ifndef INCLUDED_PXREPEATER_H_
#define INCLUDED_PXREPEATER_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/peak.h>

// Convenience abstract class for objects that want to receive a function
// call at regular intervals.

class PXRepeater
  {
public:
  PXRepeater(int inInterval);
  virtual ~PXRepeater();
  
  // Pure virtual, concrete subclasses must override.
  virtual void RepeaterFire() = 0;
  
protected:
  static void TimerCallback(peak_timer ti, void *context);
  
  peak_timer mRepeaterTimer;
  };

#endif // INCLUDED_PXREPEATER_H_

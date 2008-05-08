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
#define RCSID "$Id: PXRepeater.cc,v 1.2 2004/01/10 14:32:29 mbuna Exp $"

#include "PXRepeater.h"

PXRepeater::PXRepeater(int inInterval)
  {
  mRepeaterTimer = peak_timer_create(inInterval, inInterval, TimerCallback,
                                     this);
  peak_task_timer_add(peak_task_self(), mRepeaterTimer);
  }

PXRepeater::~PXRepeater()
  {
  peak_task_timer_remove(peak_task_self(), mRepeaterTimer);
  peak_release(mRepeaterTimer);
  }

void
PXRepeater::TimerCallback(peak_timer t, void *context)
  {
  PXRepeater *repeater = reinterpret_cast<PXRepeater*>(context);
  
  repeater->RepeaterFire();
  }

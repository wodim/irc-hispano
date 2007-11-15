/*
 * IRC - Internet Relay Chat, ircd/opercmds.c (formerly ircd/s_serv.c)
 * Copyright (C) 1990 Jarkko Oikarinen and
 *                    University of Oulu, Computing Center
 *
 * See file AUTHORS in IRC package for additional names of
 * the programmers.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: irc_militime.c,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_militime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

char *
militime(char* sec, char* usec)
  {
  struct timeval tv;
  static char timebuf[18];
  
  gettimeofday(&tv, NULL);
  if (sec && usec)
    snprintf(timebuf, sizeof(timebuf), "%ld",
             (long)((tv.tv_sec - atoi(sec)) * 1000
                    + (tv.tv_usec - atoi(usec)) / 1000));
  else
    snprintf(timebuf, sizeof(timebuf), "%ld %ld",
             (long)tv.tv_sec, (long)tv.tv_usec);
  return timebuf;
  }

char *
militime_float(char* start)
  {
  struct timeval tv;
  static char timebuf[18];
  char *p;
  
  gettimeofday(&tv, NULL);
  if (start)
    {
    if ((p = strchr(start, '.')))
      {
      p++;
      snprintf(timebuf, sizeof(timebuf), "%ld",
               (long)((tv.tv_sec - atoi(start)) * 1000
                      + (tv.tv_usec - atoi(p)) / 1000));
      }
    else 
      strcpy(timebuf, "0");
    }
  else
    snprintf(timebuf, sizeof(timebuf), "%ld.%ld",
             (long)tv.tv_sec, (long)tv.tv_usec);
  return timebuf;
  }

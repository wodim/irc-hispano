/* Copyright (C) 2003, 2004 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
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
#define RCSID "$Id: cmd_help.c,v 1.8 2006/09/11 21:37:40 spale Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_cmd.h"
#include "irc_send.h"
#include "cfgloader.h"

#include <peak/peak.h>

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <string.h>

static void
cmd_help_dump(const char *dst, const char *text, int length)
  {
  char *content;
  char *line, *endp;
  int empty = 1;
  
#ifdef HAVE_ALLOCA
  if (!(content = alloca(length + 1)))
    return;
#else
  if (!(content = peak_allocate(length + 1)))
    return;
#endif
  
  memcpy(content, text, length + 1);
  content[length] = '\0';
  line = endp = content;
  
  while (*endp)
    {
    if (empty && *endp != ' ' && *endp != '\t')
      empty = 0;
    
    if (*endp == '\r' || *endp == '\n')
      {
      if (endp - line > 0)
        {
        *endp++ = '\0';
        send_client_to_one(dst, line);
        line = endp;
        empty = 1;
        continue;
        }
      else
        line++;
      }
    endp++;
    }
  if (endp - line > 0 && !empty)
    send_client_to_one(dst, line);
  
#ifndef HAVE_ALLOCA
  peak_deallocate(content);
#endif
  }

static void
cmd_help_general(const char *dst)
  {
  cmd_help_dump(dst, gConfig->helplist->topic.content,
                gConfig->helplist->topic.content_length);
  
  send_client_to_one(dst, "Powered by %s/%s", PACKAGE, PACKAGE_VERSION);
  }

static void
cmd_help_specific(const char *dst, const char *topic)
  {
  CHelpLink *link = gConfig->helplist->next;
  
  for ( ; link; link = link->next)
    if (!xmlStrcasecmp((unsigned char*)link->topic.title, (unsigned char*)topic))
      {
      cmd_help_dump(dst, link->topic.content, link->topic.content_length);
      return;
      }
  
#ifdef SPANISH
  send_client_to_one(dst, "No existe ayuda del comando %s.", topic);
#else  
  send_client_to_one(dst, "HELP topic not found.");
#endif
  }

void
cmd_help(struct Client *cptr, toktabptr ttab)
  {
return;
  const char *dst = ttab->tok[0];
  
  if (ttab->size > 4)
    cmd_help_specific(dst, untokenize(ttab, 4));
  else
    cmd_help_general(dst);
  }

/* Copyright (C) 2003 Stephane Thiell
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
#define RCSID "$Id: cmd_evreg.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"

#include "evreg.h"

void
cmd_evreg(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  char *flags;
  int ch, addflags = 0, clrflags = 0, flag, tog = 1; /* + by default */
  
  if (ttab->size != 5)
    {
#ifdef SPANISH
    send_client_to_one(dst, "Sintaxis: EVREG <+/-><flags>");
#else
    send_client_to_one(dst, "Syntax: EVREG <+/-><flags>");
#endif
    return;
    }
  
  flags = ttab->tok[4];
  while((ch = *flags++))
    {
    switch(ch)
      {
      case '+':
        tog = 1;
        continue;
      case '-':
        tog = 0;
        continue;
      default:
        switch (ch)
          {
          case 'p':
            flag = EVREG_FLAG_NEWPROXY;
            break;
          case 'c':
            flag = EVREG_FLAG_CACHED;
            break;
          case 'o':
            flag = EVREG_FLAG_OPER;
            break;
          case 'x':
            flag = EVREG_FLAG_PXYSCAND;
            break;
          default:
#ifdef SPANISH
            send_client_to_one(dst, "EVREG: Flag invalido '%c'", ch);
#else
            send_client_to_one(dst, "EVREG: Invalid flag '%c'", ch);
#endif
            return;
          }
        if (tog)
          {
          clrflags &= ~flag;
          addflags |= flag;
          }
        else
          {
          addflags &= ~flag;
          clrflags |= flag;
          }
        break;
      }
    }
  
  if (!addflags && !(cptr->flags & CLIENT_FLAG_EVREG))
#ifdef SPANISH
    send_client_to_one(dst, "EVREG: No estas registrado");
#else
    send_client_to_one(dst, "EVREG: You are not registered");
#endif
  else
    {
    if ((flag = evreg_mod(cptr, addflags, clrflags)) == 0)
#ifdef SPANISH
      send_client_to_one(dst, "EVREG: Tus flags de eventos ahora estan vacios");
#else
      send_client_to_one(dst, "EVREG: Your event flags are now empty");
#endif
    else
      {
      char flagbuf[16];
      int i = 0;
      
      flagbuf[i++] = '+';
      if (flag & EVREG_FLAG_NEWPROXY)
        flagbuf[i++] = 'p';
      if (flag & EVREG_FLAG_CACHED)
        flagbuf[i++] = 'c';
      if (flag & EVREG_FLAG_OPER)
        flagbuf[i++] = 'o';
      if (flag & EVREG_FLAG_PXYSCAND)
        flagbuf[i++] = 'x';
      flagbuf[i] = '\0';
#ifdef SPANISH
      send_client_to_one(dst, "EVREG: Tus flags de eventos ahora son: %s", flagbuf);
#else
      send_client_to_one(dst, "EVREG: Your event flags are now %s", flagbuf);
#endif
      }
    }
  }

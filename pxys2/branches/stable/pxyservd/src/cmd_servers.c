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
#define RCSID "$Id: cmd_servers.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_userbase.h"
#include "scan.h"
#include "session.h"

#include <assert.h>
#include <string.h>
#include <peak/dict.h>

static void
servers_end_map(const char *dst)
  {
  int total, hubs, services, leaves;
  
  total = irc_network_get_server_count();
  hubs = irc_network_get_server_count_flagged(SERVER_FLAG_HUB);
  services = irc_network_get_server_count_flagged(SERVER_FLAG_SERVICE);
  leaves = total - irc_network_get_server_count_flagged(SERVER_FLAG_HUB
                                                        |SERVER_FLAG_SERVICE);

#ifdef SPANISH
  send_client_to_one(dst, "Fin del map (%d servidores: %d hubs, %d services, "
                     "%d leaves)", total, hubs, services, leaves);
#else  
  send_client_to_one(dst, "End of map (%d servers: %d hubs, %d services, "
                     "%d leaves)", total, hubs, services, leaves);
#endif
  }

void
cmd_servers(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  char prompt[64];
  char *p = prompt;
  struct Server *sptr, *sstart, *s, *s2;
  int i, nolast = 0, ccnt, details = 0;
  unsigned int cnt;
  struct Client **cptr_p;
  struct Server **ssp;       /* Downlink server stack pointer */
  struct Server *sst[256];
  
  if (irc_network_get_server_count() > 256) /* bah */
    return;
  
  for (i = 4; i <= 5; i++)
    {
    if (ttab->size <= i)
      break;
    
    if (!strcmp(ttab->tok[i], "-info"))
      details |= 2;
    else if (!strcmp(ttab->tok[i], "-scan"))
      details |= 1;
    }
  
  /* Servers tree map display.
   */
  ssp = sst;
  sptr = sstart = &gMe;
  if (details)
    send_client_to_one(dst, "%u:%s", sptr->nserv, sptr->name);
  else
    send_client_to_one(dst, "%s", sptr->name);
  *p = '\0';
  
  for (;;)
    {
    sstart = sstart->next;
    
    for (s = sstart; s; s = s->next)
      {
      if (s->uplink == sptr)
        {
        int noscan = 1;
        
        for (s2 = s->next; s2; s2 = s2->next)
          {
          if (s2->uplink == sptr)
            {
            nolast = 1;
            break;
            }
          }
        
        *ssp++ = sptr = s;
        
        if (nolast)
          p[0] = '|', p[1] = '-';
        else
          p[0] = '`', p[1] = '-';
        p[2] = '\0';
        
        if (details & 2)
          {
          cnt = 0;
          cptr_p = sptr->cliT;
          for (ccnt = sptr->clients; ccnt > 0; cptr_p++)
            {
            if (*cptr_p != NULL)
              {
              cnt += irc_userbase_count_channels(*cptr_p);
              ccnt--;
              }
            }
          
          if (details & 1)
            noscan = scan_check_noscan_server(sptr);
          
#ifdef SPANISH
          send_client_to_one(dst, "%s%u:%s%s%s   [%u clientes]   [%u chanrefs]",
#else          
          send_client_to_one(dst, "%s%u:%s%s%s   [%u clients]   [%u chanrefs]",
#endif
                             prompt, sptr->nserv, noscan ? "" : "\2",
                             sptr->name, noscan ? "" : "\2",
                             sptr->clients, cnt);
          }
        else
          {
          
          if (details & 1)
            noscan = scan_check_noscan_server(sptr);
          
          send_client_to_one(dst, "%s%s%s%s", prompt, noscan ? "" : "\2",
                             sptr->name, noscan ? "" : "\2");
          }
        
        if (!nolast)
          p[0] = ' ';
        else
          nolast = 0;
        p[1] = ' ';
        if (p - prompt > sizeof(prompt) - 4)
          return;
        p += 2;
        s = &gMe;
        }
      }
    
    if (ssp == sst)
      break;
    
    sstart = *--ssp;
    sptr = sstart->uplink;
    *(p -= 2) = '\0';
    }
  servers_end_map(dst);
  }

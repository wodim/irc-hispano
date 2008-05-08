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
#define RCSID "$Id: cmd_showcmds.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_msg.h"
#include "irc_send.h"
#include "cfgloader.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"

#include <libbcl/memory.h>

#include <assert.h>
#include <string.h>
#include <peak/dict.h>

extern peak_dict cmds_map;

static int
showcmds_sort(const void *val1, const void *val2)
  {
  const char *cmd1 = *(const char **)val1;
  const char *cmd2 = *(const char **)val2;
  
  return strcmp(cmd1, cmd2);
  }

void
cmd_showcmds(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  int cnt;
  const char *cmds_buf[100];
  const char **cmds, *p;
  int i;

#if IRC_HISPANO
  if (!((cptr->flags & CLIENT_FLAG_OPER) || (cptr->flags & CLIENT_FLAG_HELPER)))
#else 
  if (!(cptr->flags & CLIENT_FLAG_OPER))
#endif
    return; /* Non opers go play balls. */
  
  cnt = peak_dict_get_count(cmds_map);
  assert(cnt > 0);
  
#ifdef SPANISH
  send_client_to_one(dst, "%d COMANDOS HABILITADOS", cnt);
#else  
  send_client_to_one(dst, "%d COMMANDS ENABLED", cnt);
#endif
  send_client_to_one(dst, "------------------------", cnt);
  
  /* Allocate on stack if possible. Could use peak_dict_apply() too, but
   * it's less readable.
   */
  cmds = (cnt > 100)
    ? (const char **)memory_alloc(cnt * sizeof(const char *))
    : cmds_buf;
  
  peak_dict_get_all(cmds_map, (const void **)cmds, NULL);
  
  qsort(cmds, cnt, sizeof(const char *), showcmds_sort);
  
  for (i = 0; i < cnt; i += 2)
    {
    p = *cmds++;
    if (i + 1 < cnt)
      send_client_to_one(dst, " %-10s  %s", p, *cmds++);
    else
      send_client_to_one(dst, " %-10s", p);
    }
  
  if (cnt > 100)
    memory_free(cmds);
  
  send_client_to_one(dst, "------------------------");
  }

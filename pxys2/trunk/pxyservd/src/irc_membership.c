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
#define RCSID "$Id: irc_membership.c,v 1.6 2004/01/17 18:22:35 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_membership.h"
#include "irc_channel.h"    /* irc_channel_chuck() */
#include "irc_struct.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dbprim/dbprim.h>
#include <libbcl/memory.h>

#include "debug.h"
#include "pxyservd_log.h"

struct _iter_pack
  {
  irc_membership_iter_func iter;
  void *ud;
  };

/* Channels<->Users relationship table */
static smat_table_t sSmatTable;

#if 1
static unsigned long
irc_membership_resize(smat_table_t *table, unsigned long newsize)
  {
  assert(table == &sSmatTable);
  Debug((DL_BASIC, "irc_membership_resize: newsize=%lu", newsize));
  return 0;
  }
#endif

void
irc_membership_init()
  {
  st_init(&sSmatTable, HASH_FLAG_AUTOGROW|HASH_FLAG_AUTOSHRINK,
          irc_membership_resize, NULL, 0);
  }

void
irc_membership_finalize()
  {
  assert(st_count(&sSmatTable) == 0);
  
  st_free(&sSmatTable);
  }

int
irc_membership_get_count()
  {
  return st_count(&sSmatTable);
  }

size_t
irc_membership_get_smat_size()
  {
  return st_size(&sSmatTable);
  }

void
irc_membership_add(struct Channel *channel, struct Client *client)
  {
  smat_entry_t *e;
  unsigned long err;
  
  err = st_add(&sSmatTable, &e,
               &client->mhead, LINK_LOC_TAIL, 0,
               &channel->mhead, LINK_LOC_TAIL, 0);
  Debug((DL_MAX, "irc_membership_add: st_add err=%lu count=%lu",
         err, st_count(&sSmatTable)));
  
  assert(err == 0 || err == DB_ERR_DUPLICATE);
  
  if (err == DB_ERR_DUPLICATE)
    {
    /* log error */
    Debug((DL_MAX, "irc_membership_add: duplicate membership for %s on %s",
           client->nick, channel->chname));
    log_system("irc_membership_add: duplicate membership for %s on %s",
              client->nick, channel->chname);
    }
  }

int
irc_membership_exists(struct Channel *channel, struct Client *client)
  {
  return st_find(&sSmatTable, NULL, &client->mhead, &channel->mhead) ? 0 : 1;
  }

void
irc_membership_remove(struct Channel *channel, struct Client *client)
  {
  smat_entry_t *e;
  unsigned long err;
  
  if (st_find(&sSmatTable, &e, &client->mhead, &channel->mhead))
    {
    Debug((DL_MAX, "irc_membership_remove: user %s not found in channel %s",
           client->nick, channel->chname));
    return;
    }
  
  err = st_remove(&sSmatTable, e);
  assert(err == 0);
  
  if (sh_count(&channel->mhead) == 0)
    irc_channel_chuck(channel);
  }

static unsigned long
irc_membership_remove_client_cb(smat_table_t *table, smat_entry_t *e,
                                void *extra)
  {
  unsigned long err;
  struct Channel *channel;
  
  channel = se_object(e, MEMBERSHIP_LOC_CHANNEL);
  
  err = st_remove(&sSmatTable, e);
  assert(err == 0);
  
  if (sh_count(&channel->mhead) == 0)
    irc_channel_chuck(channel);
  
  return 0;
  }

void
irc_membership_remove_client(struct Client *client)
  {
  unsigned long err;
  
  err = sh_iter(&client->mhead, NULL, irc_membership_remove_client_cb, 0, 0);
  assert(err == 0);
  }

static unsigned long
irc_membership_iter_cb(smat_table_t *table, smat_entry_t *entry, void *extra)
  {
  struct _iter_pack *p = (struct _iter_pack *)extra;
  struct Channel *channel = se_object(entry, MEMBERSHIP_LOC_CHANNEL);
  struct Client *client = se_object(entry, MEMBERSHIP_LOC_CLIENT);
  
  (*p->iter)(channel, client, p->ud);
  
  return 0;
  }

void
irc_membership_iterate(irc_membership_iter_func iter, void *ud)
  {
  struct _iter_pack p = { iter, ud };
  unsigned long err;
  
  err = st_iter(&sSmatTable, irc_membership_iter_cb, &p);
  assert(err == 0);
  }

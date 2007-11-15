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
#define RCSID "$Id: irc_channel.c,v 1.4 2004/01/10 18:15:36 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Max empty channels to preserve.
 */
#define CHANNEL_EMPTY_MAX       200

/* Max idle duration allowed for an empty channel before being destroyed
 * (in seconds).
 */
#define CHANNEL_EMPTY_TIMEOUT   90

#include "irc_channel.h"
#include "irc_membership.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dbprim/dbprim.h>
#include <libbcl/memory.h>
#include <peak/peak.h>

#include "debug.h"
#include "hash_fun.h"
#include "irc_string.h"
#include "irc_struct.h"

/* Stats counters */
unsigned int current_chucked;
unsigned int rehabilitated_channels, destructed_channels;

static peak_garbage sCGarbage;

static hash_table_t sCHtab = HASH_TABLE_INIT(HASH_FLAG_AUTOGROW|
                                             HASH_FLAG_AUTOSHRINK,
                                             strhash, ircd_strcomp,
                                             NULL, NULL);

static void irc_channel_destruct(peak_garbage g, void *ptr, void *context);

void
irc_channel_init()
  {
  sCGarbage = peak_garbage_create(CHANNEL_EMPTY_MAX,
                                  CHANNEL_EMPTY_TIMEOUT,
                                  NULL,
                                  irc_channel_destruct,
                                  NULL);
  current_chucked = 0;
  rehabilitated_channels = 0;
  destructed_channels = 0;
  }

void
irc_channel_finalize()
  {
  peak_release(sCGarbage);
  }

struct Channel *
irc_channel_get(const char *chname)
  {
  hash_entry_t *he;
  unsigned long err;
  db_key_t dk = DB_KEY_INIT((void*)chname, strlen(chname));
  
  err = ht_find(&sCHtab, &he, &dk);
  if (err == DB_ERR_NOENTRY)
    return NULL;
  assert(err == 0);
  return he_value(he);
  }

int
irc_channel_get_count()
  {
  return ht_count(&sCHtab) - current_chucked;
  }

int
irc_channel_get_hash_table_count()
  {
  return ht_count(&sCHtab);
  }

size_t
irc_channel_get_hash_table_size()
  {
  return ht_size(&sCHtab);
  }

void
irc_channel_chuck(struct Channel *channel)
  {
  Debug((DL_MAX, "irc_channel_chuck: %s", channel->chname));
  
  if (channel->flags & CHANNEL_FLAG_CHUCKED)
    return;
  
  channel->flags |= CHANNEL_FLAG_CHUCKED;
  peak_garbage_collect(sCGarbage, channel);
  current_chucked++;
  }

int
irc_channel_get_chucked_count()
  {
  return peak_garbage_get_collected_count(sCGarbage);
  }

struct Channel *
irc_channel_create(const char *chname, time_t creationtime)
  {
  struct Channel *channel;
  size_t length;
  db_key_t dk;
  unsigned long err;
  
  channel = irc_channel_get(chname);
  if (channel)
    {
    channel->creationtime = creationtime; /* resync */
    return channel;
    }
  
  length = strlen(chname);
  channel = memory_alloc(sizeof(struct Channel) + length);
  
  /* Fill structure */
  err = he_init(&channel->hentry, (void*)channel);
  assert(err == 0);
  
  channel->flags = CHANNEL_FLAG_NONE;
  channel->creationtime = creationtime;
  strcpy(channel->chname, chname);
  
  /* Initialize channel sparse matrix list head */
  sh_init(&channel->mhead, MEMBERSHIP_LOC_CHANNEL, channel);
  
  /* Add channel to hash table */
  dk_key(&dk) = (void*)channel->chname;
  dk_len(&dk) = length;
  
  err = ht_add(&sCHtab, &channel->hentry, &dk);
  assert(err == 0);
  
  return channel;
  }

/* Called by the garbage collector when it's time to vacuum
 */
static void
irc_channel_destruct(peak_garbage g, void *ptr, void *context)
  {
  unsigned long err;
  struct Channel *channel = (struct Channel *)ptr;
  
  current_chucked--;
  
  /* We can't cancel this call after garbage collection, so we need to
   * check here if the channel hasn't been referenced again. We just
   * return without doing anything if that's the case.
   */
  if (sh_count(&channel->mhead) > 0)
    {
    channel->flags &= ~CHANNEL_FLAG_CHUCKED;
    rehabilitated_channels++; /* stats */
    return; /* can't destroy it! */
    }
  
  Debug((DL_MAX, "irc_channel_destruct: really destroying %s",
         channel->chname));
  
  /* Remove channel from hash table */
  err = ht_remove(&sCHtab, &channel->hentry);
  assert(err == 0);
  
  /* Free allocated memory */
  memory_free(channel);
  destructed_channels++;
  }


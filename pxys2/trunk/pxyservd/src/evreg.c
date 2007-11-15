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
#define RCSID "$Id: evreg.c,v 1.3 2004/01/02 16:34:21 mbuna Exp $"

#include "evreg.h"
#include "irc_client.h"
#include "irc_msg.h"
#include "irc_numnicks.h"
#include "irc_send.h"
#include "session.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <peak/peak.h>

struct EVRegClient
  {
  struct EVRegClient *   next;
  struct EVRegClient **  prev_p;
  struct Client *        client;
  uint32_t               flags;
  };

static struct EVRegClient *evreg_clist_head;
static peak_mem_pool evreg_mpool;

void
evreg_init()
  {
  evreg_mpool = peak_mem_pool_create(sizeof(struct EVRegClient), 0);
  }

void
evreg_finalize()
  {
  evreg_clist_head = NULL;
  peak_release(evreg_mpool);
  }

static struct EVRegClient *
evreg_add(struct Client *cptr, uint32_t flags)
  {
  struct EVRegClient *rcptr =
    (struct EVRegClient *)peak_mem_pool_new(evreg_mpool);
  
  rcptr->client = cptr;
  rcptr->flags = flags;
  
  rcptr->next = evreg_clist_head;
  rcptr->prev_p = &evreg_clist_head;
  if (evreg_clist_head)
    evreg_clist_head->prev_p = &rcptr->next;
  evreg_clist_head = rcptr;
  return rcptr;
  }

static void
evreg_remove(struct EVRegClient *rcptr)
  {
  if (rcptr->next)
    rcptr->next->prev_p = rcptr->prev_p;
  if (rcptr->prev_p)
    *rcptr->prev_p = rcptr->next;
  
  peak_mem_pool_delete(evreg_mpool, rcptr);
  }

static struct EVRegClient *
evreg_find(const struct Client *cptr)
  {
  struct EVRegClient *rcptr;
  
  for (rcptr = evreg_clist_head; rcptr; rcptr = rcptr->next)
    if (rcptr->client == cptr)
      return rcptr;
  return NULL;
  }

uint32_t
evreg_mod(struct Client *cptr, uint32_t set, uint32_t clear)
  {
  struct EVRegClient *rcptr;
  
  if (cptr->flags & CLIENT_FLAG_EVREG)
    {
    rcptr = evreg_find(cptr);
    assert(rcptr != NULL);
    
    if (set)
      rcptr->flags |= set;
    if (clear)
      rcptr->flags &= ~clear;
    
    if (rcptr->flags == 0)
      {
      cptr->flags &= ~CLIENT_FLAG_EVREG;
      evreg_remove(rcptr);
      }
    }
  else
    {
    assert(set != 0);
    rcptr = evreg_add(cptr, set);
    cptr->flags |= CLIENT_FLAG_EVREG;
    }
  return rcptr->flags;
  }

void
evreg_exit(struct Client *cptr)
  {
  if (cptr->flags & CLIENT_FLAG_EVREG)
    {
    cptr->flags &= ~CLIENT_FLAG_EVREG;
    evreg_remove(evreg_find(cptr));
    }
  }

void
evreg_broadcast(uint32_t filter, const char *fmt, ...)
  {
  va_list vl;
  struct EVRegClient *rcptr;
  char msgbuf[MSGBUF_SIZE];
  
  if (!MYCLIENT_EXISTS())
    return;
  
  va_start(vl, fmt);
  vsnprintf(msgbuf, sizeof(msgbuf), fmt, vl);
  va_end(vl);
  
  for (rcptr = evreg_clist_head; rcptr; rcptr = rcptr->next)
    {
    if (rcptr->flags & filter)
      {
      char dst[6];
      inttobase64(dst, rcptr->client->nserv, 2);
      inttobase64(dst + 2, rcptr->client->nnick, 3);
      dst[5] = '\0';
      peak_stream_msgbuf_make(gIRCStream, "%s%s " TOK_NOTICE " %s :%s" CRLF,
                              gMe.yy, MYCLIENT_NUM64, dst, msgbuf);
      }
    }
  }

void
evreg_show(const char *dst)
  {
  struct EVRegClient *rcptr;
  
  send_client_to_one(dst, "EVREG Client List");
  for (rcptr = evreg_clist_head; rcptr; rcptr = rcptr->next)
    {
    char flagbuf[16];
    int i = 0;
    
    flagbuf[i++] = '+';
    if (rcptr->flags & EVREG_FLAG_NEWPROXY)
      flagbuf[i++] = 'p';
    if (rcptr->flags & EVREG_FLAG_CACHED)
      flagbuf[i++] = 'c';
    if (rcptr->flags & EVREG_FLAG_OPER)
      flagbuf[i++] = 'o';
    if (rcptr->flags & EVREG_FLAG_PXYSCAND)
      flagbuf[i++] = 'x';
    flagbuf[i] = '\0';
    send_client_to_one(dst, "  %s%s (%s): %s", rcptr->client->nick,
                       rcptr->client->flags & CLIENT_FLAG_OPER ? "*" : "",
                       irc_network_get_server(rcptr->client->nserv)->name,
                       flagbuf);
    }
  send_client_to_one(dst, "End of EVREG Client List");
  }

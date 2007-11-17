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
#define RCSID "$Id: cmd_stats.c,v 1.3 2004/01/12 12:31:03 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_send.h"

#include "irc_membership.h"
#include "irc_channel.h"
#include "irc_client.h"
#include "irc_membership.h"
#include "irc_network.h"
#include "irc_userbase.h"
#include "session.h" /* gIRCStream */

#include <peak/peak.h>

extern unsigned long smat_freemem(void);

extern time_t gBirthTime;
extern peak_mem_pool user_pool;
extern unsigned int rehabilitated_channels, destructed_channels;
extern time_t link_timestamp;
extern uint32_t l1_cache_hits;

void
cmd_stats(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  size_t msg_size, max_msgs, alloc_msgs, queue_msgs, queue_size;
  size_t read_count, write_count;
  double up;
  
  read_count = peak_stream_get_read_count(gIRCStream);
  write_count = peak_stream_get_write_count(gIRCStream);
  
#ifdef SPANISH
  send_client_to_one(dst, "\2STATS\2");
  send_client_to_one(dst, "Usando libpeak %s/%s", peak_version_get_string(),
                     peak_task_get_engine_name(peak_task_self()));
  send_client_to_one(dst, "IRC socket: recibidos %u enviados %u (bytes)",
                     read_count, write_count);

  up = (double)(peak_time() - link_timestamp) * 1024.0;

  send_client_to_one(dst, "IRC socket: recibidos %.2fKB/s enviados %.2fKB/s",
                     (double)read_count/up, (double)write_count/up);

  send_client_to_one(dst, "Entradas cache local de IP: %lu", l1_cache_hits);

  send_client_to_one(dst, " ");
  send_client_to_one(dst, "ESTADO DE ESTRUCTURAS INTERNAS (dbprim):");

  peak_stream_msgbuf_get_info(gIRCStream, &msg_size, &max_msgs, &alloc_msgs,
                              &queue_msgs, &queue_size);

  send_client_to_one(dst, "Buffer de mensajess: tamano %u contador %u/%u usados %u "
                     "(cola envios %u)", msg_size, alloc_msgs, max_msgs,
                     queue_msgs, queue_size);

  send_client_to_one(dst, "Tabla Hash de Servidores: contador %d tamano en memoria %u",
                     irc_network_get_server_count(),
                     irc_network_get_hash_table_size());

  send_client_to_one(dst, "Tabla Hash de Clientes: contador %d tamano en memoria %u",
                     irc_userbase_get_count(),
                     irc_userbase_get_hash_table_size());

  send_client_to_one(dst, "Area de memoria de Clientes: usados %d libres %d (tamanio %d)",
                     peak_mem_pool_get_used_count(user_pool),
                     peak_mem_pool_get_free_count(user_pool),
                     peak_mem_pool_get_size(user_pool));

  send_client_to_one(dst, "Tabla Hash de Canales: contador %d tamano en memoria %u",
                     irc_channel_get_hash_table_count(),
                     irc_channel_get_hash_table_size());

  up = 100.0 * (double)rehabilitated_channels/(double)destructed_channels;
  send_client_to_one(dst, "Garbage de Canales: chucked %d rehabilitados %u"
                     " (%.1f%%) destruidos %u",
                     irc_channel_get_chucked_count(), rehabilitated_channels,
                     up, destructed_channels);

  send_client_to_one(dst, "Matriz de Usuarios en Canales: contador %d tamao en memoria %u",
                     irc_membership_get_count(),
                     irc_membership_get_smat_size());

  send_client_to_one(dst, "Matriz de Usuarios en Canales memoria liberada: %lu",
                     smat_freemem());
#else  
  send_client_to_one(dst, "\2STATS\2");
  send_client_to_one(dst, "Using libpeak %s/%s", peak_version_get_string(),
                     peak_task_get_engine_name(peak_task_self()));
  send_client_to_one(dst, "IRC socket: recv %u send %u (bytes)", 
                     read_count, write_count);
  
  up = (double)(peak_time() - link_timestamp) * 1024.0;
  
  send_client_to_one(dst, "IRC socket: recv %.2fKB/s send %.2fKB/s", 
                     (double)read_count/up, (double)write_count/up);
  
  send_client_to_one(dst, "Local IP cache hits: %lu", l1_cache_hits);
  
  send_client_to_one(dst, " ");
  send_client_to_one(dst, "INTERNAL STRUCTS STATE (dbprim):");
  
  peak_stream_msgbuf_get_info(gIRCStream, &msg_size, &max_msgs, &alloc_msgs,
                              &queue_msgs, &queue_size);
  
  send_client_to_one(dst, "Msg bufs: size %u count %u/%u used %u "
                     "(sendq %u)", msg_size, alloc_msgs, max_msgs,
                     queue_msgs, queue_size);
  
  send_client_to_one(dst, "Server hash table: count %d memory size %u",
                     irc_network_get_server_count(),
                     irc_network_get_hash_table_size());
  
  send_client_to_one(dst, "Client hash table: count %d memory size %u",
                     irc_userbase_get_count(),
                     irc_userbase_get_hash_table_size());
  
  send_client_to_one(dst, "Client mempool: used %d free %d (size %d)",
                     peak_mem_pool_get_used_count(user_pool),
                     peak_mem_pool_get_free_count(user_pool),
                     peak_mem_pool_get_size(user_pool));
  
  send_client_to_one(dst, "Channel hash table: count %d memory size %u",
                     irc_channel_get_hash_table_count(),
                     irc_channel_get_hash_table_size());
  
  up = 100.0 * (double)rehabilitated_channels/(double)destructed_channels;
  send_client_to_one(dst, "Channel garbage: chucked %d rehabilitated %u"
                     " (%.1f%%) destructed %u",
                     irc_channel_get_chucked_count(), rehabilitated_channels,
                     up, destructed_channels);
  
  send_client_to_one(dst, "Membership sparse matrix: count %d memory size %u",
                     irc_membership_get_count(),
                     irc_membership_get_smat_size());
  
  send_client_to_one(dst, "Membership sparse matrix freemem: %lu",
                     smat_freemem());
#endif
  }

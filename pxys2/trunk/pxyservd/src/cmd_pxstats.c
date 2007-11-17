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
#define RCSID "$Id: cmd_pxstats.c,v 1.1.1.1 2003/12/30 17:09:29 mbuna Exp $"

#include "irc_cmd.h"
#include "irc_send.h"

#include "irc_numnicks.h"
#include "scan.h"
#include "PXServiceMsg.h"

#include <peak/peak.h>

void
cmd_pxstats(struct Client *cptr, toktabptr ttab)
  {
  const char *dst = ttab->tok[0];
  
  send_client_to_one(dst, "\2PXSTATS\2");
  
  if (scan_send_simple_command(cptr, PXYSCAND_SIG, PX_CMD_STATS) == -1)
#ifdef SPANISH
    send_client_to_one(dst, "/!\\ El demonio de Escaneo no esta conectado."
                       " No puedo recibir datos de estadisticas desde el demonio.");
#else
    send_client_to_one(dst, "/!\\ Scanner daemon not connected."
                       " Can't retrieve stats data from pxyscand.");
#endif
  }

void
cmd_pxstats_reply(struct Client *cptr, PXSStats *stats)
  {
  char dst[6];
  uint32_t v1, v2;
  double pt;
  
  inttobase64(dst, cptr->nserv, 2);
  inttobase64(dst + 2, cptr->nnick, 3);
  dst[5] = '\0';
  
#ifdef SPANISH
  send_client_to_one(dst, "Estasticas recibidas desde %s", stats->version);
  send_client_to_one(dst, "--------------------------------");
  send_client_to_one(dst, "Contadores globales: peticiones %lu  scaneados %lu  "
                     "proxies %lu",
                     ntohl(stats->servQueryCount),
                     ntohl(stats->servScannedCount),
                     ntohl(stats->servProxyCount));
  send_client_to_one(dst, "Entradas Cache: IPCache %lu  PXCache %lu",
                     ntohl(stats->servIPCacheHits),
                     ntohl(stats->servPXCacheHits));
  send_client_to_one(dst, "Contador de escaneos en proceso: %lu",
                     ntohl(stats->servInProgressCount));
  send_client_to_one(dst, "Entradas lista de \"noscan\" del escaneador: %lu",
                     ntohl(stats->servNoScanHits));
  send_client_to_one(dst, "--------------------------------");
  send_client_to_one(dst, "IP CACHES:");
  v1 = ntohl(stats->servIP4CacheCount);
  v2 = ntohl(stats->servIP4CacheSize);
  pt = v2 > 0 ? 100.0 * (double)v1/(double)v2 : 0.;
  send_client_to_one(dst, "IPv4 \"no proxy\" cache: %lu/%lu IPs (%.1f%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "IPv4 \"no proxy\" cache expiracion en: %d segundos",
                     ntohl(stats->servIP4CacheExpire));
  v1 = ntohl(stats->servPX4CacheCount);
  v2 = ntohl(stats->servPX4CacheSize);
  pt = v2 > 0 ? 100.0 * (double)v1/(double)v2 : 0.;
  send_client_to_one(dst, "IPv4 \"proxy\" cache: %lu/%lu IPs (%.1f%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "IPv4 \"proxy\" cache expiracion en: %d segundos",
                     ntohl(stats->servPX4CacheExpire));
  send_client_to_one(dst, "--------------------------------");

  send_client_to_one(dst, "Sesiones OPAS actuales:");

  send_client_to_one(dst, "Contadores: peticiones %lu  error %lu",
                     ntohl(stats->sessQueryCount),
                     ntohl(stats->sessErrorCount));

  v1 = ntohl(stats->sessScannedCount);
  v2 = ntohl(stats->sessProxyCount);
  pt = v1 > 0 ? 100.0 * (double)v2 / (double) v1 : 0.;
  send_client_to_one(dst, "Contadores: escaneados %lu  proxy %lu (%g%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "Trafico de sesiones: leidos %lu  escritos %lu (bytes)",
                     ntohl(stats->sessReadBytes),
                     ntohl(stats->sessWriteBytes));
  send_client_to_one(dst, "--------------------------------");

  send_client_to_one(dst, "Contador de sesiones OPAS %lu  total %lu  "
                     "rechazados %lu",
                     ntohl(stats->sessCurrent),
                     ntohl(stats->sessCount),
                     ntohl(stats->sessRejectedCount));
#else
  send_client_to_one(dst, "Received stats from %s", stats->version);
  send_client_to_one(dst, "--------------------------------");
  send_client_to_one(dst, "Global counters: query %lu  scanned %lu  "
                     "proxy %lu",
                     ntohl(stats->servQueryCount),
                     ntohl(stats->servScannedCount),
                     ntohl(stats->servProxyCount));
  send_client_to_one(dst, "Cache hits: IPCache %lu  PXCache %lu",
                     ntohl(stats->servIPCacheHits),
                     ntohl(stats->servPXCacheHits));
  send_client_to_one(dst, "Scan already in progress counter: %lu",
                     ntohl(stats->servInProgressCount));
  send_client_to_one(dst, "Scanner's \"noscan\" list hits: %lu",
                     ntohl(stats->servNoScanHits));
  send_client_to_one(dst, "--------------------------------");
  send_client_to_one(dst, "IP CACHES:");
  v1 = ntohl(stats->servIP4CacheCount);
  v2 = ntohl(stats->servIP4CacheSize);
  pt = v2 > 0 ? 100.0 * (double)v1/(double)v2 : 0.;
  send_client_to_one(dst, "IPv4 \"no proxy\" cache: %lu/%lu IPs (%.1f%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "IPv4 \"no proxy\" cache expiration delay: %d secs",
                     ntohl(stats->servIP4CacheExpire));
  v1 = ntohl(stats->servPX4CacheCount);
  v2 = ntohl(stats->servPX4CacheSize);
  pt = v2 > 0 ? 100.0 * (double)v1/(double)v2 : 0.;
  send_client_to_one(dst, "IPv4 \"proxy\" cache: %lu/%lu IPs (%.1f%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "IPv4 \"proxy\" cache expiration delay: %d secs",
                     ntohl(stats->servPX4CacheExpire));
  send_client_to_one(dst, "--------------------------------");
  
  send_client_to_one(dst, "Current OPAS session:");
  
  send_client_to_one(dst, "Counters: query %lu  error %lu",
                     ntohl(stats->sessQueryCount),
                     ntohl(stats->sessErrorCount));
  
  v1 = ntohl(stats->sessScannedCount);
  v2 = ntohl(stats->sessProxyCount);
  pt = v1 > 0 ? 100.0 * (double)v2 / (double) v1 : 0.;
  send_client_to_one(dst, "Counters: scanned %lu  proxy %lu (%g%%)",
                     v1, v2, pt);
  send_client_to_one(dst, "Session traffic: read %lu  write %lu (bytes)",
                     ntohl(stats->sessReadBytes),
                     ntohl(stats->sessWriteBytes));
  send_client_to_one(dst, "--------------------------------");
  
  send_client_to_one(dst, "OPAS sessions count: current %lu  total %lu  "
                     "rejected %lu",
                     ntohl(stats->sessCurrent),
                     ntohl(stats->sessCount),
                     ntohl(stats->sessRejectedCount));
#endif
  }

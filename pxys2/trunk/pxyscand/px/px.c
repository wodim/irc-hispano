/* Copyright (C) 2003,2004 Stephane Thiell
 *
 * This file is part of pxyscand/px (from pxys)
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
#define RCSID "$Id: px.c,v 1.2 2004/01/03 01:03:16 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define DEFAULT_SERVER     "127.0.0.1"
#define DEFAULT_PORT       9601

/* OPAS library header */
#include <opas/opas.h>

/* PEAK library header */
#include <peak/peak.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PERROR(e) do { perror(e); exit(1); } while(0)

/* Callback prototypes */
static void opas_send_cb(const void *data, uint32_t length, void *uptr);
static void opas_reply_noproxy_cb(struct opas_msg_query *replyp,
                                  void *context);
static void opas_reply_proxy_cb(struct opas_msg_reply_proxy *replyp,
                                void *context);
static void opas_reply_error_cb(struct opas_msg_reply_error *replyp,
                                void *context);
static void opas_reply6_noproxy_cb(struct opas_msg_query6 *replyp,
                                   void *context);
static void opas_reply6_proxy_cb(struct opas_msg_reply6_proxy *replyp,
                                 void *context);
static void opas_reply6_error_cb(struct opas_msg_reply6_error *replyp,
                                 void *context);
static void opas_msg_user_cb(struct opas_msg_user_header *umsgp, void *data,
                             void *context);

static uint16_t opas_port = DEFAULT_PORT;
static char *opas_server = DEFAULT_SERVER;
static opas_session_t session; /* Our current OPAS session */

/* To open a new session, you need to define a few callbacks contained in
   a struct opas_callbacks. You don't define callbacks you don't want.
*/
static const struct opas_callbacks opas_cbs =
  {
  opas_send_cb, /* This one is called by the OPAS library when data needs to
                 * be send.
                 */
  
  /* IPv4 callbacks.
   */
  NULL,         /* We don't plan to receive any queries, we are only a
                 * client! :)
                 */
  opas_reply_noproxy_cb,
  opas_reply_proxy_cb,
  opas_reply_error_cb,
  
  /* IPv6 callbacks defined as an example, but pxyscand doesn't support IPv6
   * scanning yet.
   */
  NULL,         /* No ipv6 query wanted neither. */
  opas_reply6_noproxy_cb,
  opas_reply6_proxy_cb,
  opas_reply6_error_cb,
  opas_msg_user_cb
  };

static void
display_usage()
  {
  fprintf(stderr, "*** Usage: px [-v] [-s <server>] [-p <port>] <host>\n");
  exit(1);
  }

static void
display_version()
  {
  fprintf(stderr, "*** px (%s/%s) - OPAS command line client\n",
          PACKAGE, PACKAGE_VERSION);
  fprintf(stderr, "*** Part of pxys v.2 - http://pxys.sf.net\n");
  fprintf(stderr, "*** %s\n", RCSID);
  exit(0);
  }

static int
parse_options(int *argc, char **argv[])
  {
  extern char *optarg;
  extern int optind;
  int c;
  
  while ((c = getopt(*argc, *argv, "p:s:v")) != -1)
    {
    switch (c)
      {
      case 'p':
        opas_port = (uint16_t)atoi(optarg);
        break;
      case 's':
        opas_server = strdup(optarg);
        break;
      case 'v':
        display_version();
        break;
      case '?':
      default:
        display_usage();
        break;
      }
    }
  
  *argc -= optind;
  *argv += optind;
  return 0;
  }

static void
stream_event_read(peak_stream s, opas_session_t session)
  {
  char buffer[1024];
  int result;
  
  /* When we have something to read, we feed the OPAS library. If something
   * interesting is available, the library call one of the previously defined
   * callbacks.
   */
  result = peak_stream_read(s, buffer, sizeof(buffer));
  if (opas_feed(session, buffer, result) == -1)
    {
    fprintf(stderr, "opas_feed() failure - bad reply from server\n");
    exit(1);
    }
  }

static void
stream_event(peak_stream s, int type, void *context)
  {
  char ipbuf[16];
  struct sockaddr_in sin;
  int len;
  
  switch (type)
    {
    case PEAK_STREAM_EVT_OPEN:
      len = sizeof(sin);
      peak_stream_get_address(s, (struct sockaddr *)&sin, &len);
      if (!inet_ntop(AF_INET, &sin.sin_addr, ipbuf, sizeof(ipbuf)))
        abort();
      printf("Connected to %s.\n", ipbuf);
      break;
    case PEAK_STREAM_EVT_READ:
      stream_event_read(s, session);
      break;
    case PEAK_STREAM_EVT_END:
      fprintf(stderr, "Connection closed by server\n");
      exit(1);
      break;
    case PEAK_STREAM_EVT_ERROR:
      fprintf(stderr, "Socket stream error (server refused the"
              " connection?)\n");
      exit(1);
      break;
    default:
      break;
    }
  }

/* Called by the OPAS library when something needs to be send.
 */
static void
opas_send_cb(const void *data, uint32_t length, void *uptr)
  {
  peak_stream_write_buffered((peak_stream)uptr, data, length);
  }

/* Called by the OPAS library when a negative reply arrives.
 */
static void
opas_reply_noproxy_cb(struct opas_msg_query *replyp, void *context)
  {
  char ipbuf[16];
  
  printf("Got reply from OPAS server:\n");
  
  if (!inet_ntop(AF_INET, &replyp->addr, ipbuf, sizeof(ipbuf)))
    abort();
  
  printf("NOPROXY : No proxy found on %s\n", ipbuf);
  
  exit(0); /* Don't bother. */
  }

/* Called by the OPAS library when a proxy has been found!
 */
static void
opas_reply_proxy_cb(struct opas_msg_reply_proxy *replyp, void *context)
  {
  char ipbuf[16];
  
  printf("Got reply from OPAS server:\n");
  
  if (!inet_ntop(AF_INET, &replyp->addr, ipbuf, sizeof(ipbuf)))
    abort();
  
  printf("PROXY : Proxy found on address %s\n", ipbuf);
  
  printf("%ld,type=%u,%s at port %u\n", replyp->timestamp,
         replyp->proxy_type, replyp->proxy_descr, replyp->proxy_port);
  
  if (replyp->head & OPAS_FLAG_CACH)
    printf("This proxy has been found in cache.\n");
  
  exit(0); /* Don't bother. */
  }

static void
opas_reply_error_cb(struct opas_msg_reply_error *replyp, void *context)
  {
  char ipbuf[16];
  
  printf("Got reply from OPAS server:\n");
  
  if (!inet_ntop(AF_INET, &replyp->addr, ipbuf, sizeof(ipbuf)))
    abort();
  
  printf("ERROR : An error occured; unknown result for %s\n", ipbuf);
  
  switch (replyp->error)
    {
    case OPAS_ERROR_UNKNOWN:
      printf("        Unknown OPAS error\n");
      break;
    case OPAS_ERROR_NETDOWN:
      printf("        NETDOWN: network/host is down\n");
      break;
    case OPAS_ERROR_NETUNREACH:
      printf("        NETUNREACH: network/host is not reachable\n");
      break;
    case 1000:
      printf("        pxyscand resource error\n");
      break;
    default:
      printf("        Really unknown error!\n");
      break;
    }
  
  exit(0);
  }

/* IPv6: Called by the OPAS library when a negative reply arrives.
 */
static void
opas_reply6_noproxy_cb(struct opas_msg_query6 *replyp, void *context)
  {
  printf("opas_reply6_noproxy_cb\n");
  exit(0);
  }

/* IPv6: Called by the OPAS library when a proxy has been found!
 */
static void
opas_reply6_proxy_cb(struct opas_msg_reply6_proxy *replyp, void *context)
  {
  printf("opas_reply6_proxy_cb\n");
  exit(0);
  }

/* IPv6: Called by the OPAS library when an host can't be scanned.
 */
static void
opas_reply6_error_cb(struct opas_msg_reply6_error *replyp, void *context)
  {
  printf("opas_reply6_error_cb\n");
  exit(0);
  }

/* User message over OPAS
 */
static void
opas_msg_user_cb(struct opas_msg_user_header *umsgp, void *data,
                 void *context)
  {
  printf("OPAS msg user (not handled)\n");
  }

static void
create_tcp_session(struct in_addr *in)
  {
  struct sockaddr_in sin;
  peak_stream s;
  int err;
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_addr = *in;
  sin.sin_port = htons(opas_port);
  
  s = peak_stream_socket_create((struct sockaddr *)&sin,
                                sizeof(struct sockaddr_in),
                                PEAK_STREAM_OPT_DEFAULT,
                                stream_event,
                                NULL);
  assert(s != NULL);
  
  peak_stream_set_buffered(s, 1, 0, 8192, NULL);
  err = peak_stream_connect(s);
  if (err)
    {
    fprintf(stderr, "Can't connect to host\n");
    exit(1);
    }
  
  peak_stream_schedule(s, peak_task_self());
  
  session = opas_open_session(&opas_cbs, (void*)s);
  assert(session != NULL);
  }

  
static void
send_query(struct in_addr *in)
  {
  opas_query(session, in, 0xc0ffee);
  }

int
main(int argc, char *argv[])
  {
  struct hostent *h;
  struct in_addr in_server, in_host;
  
  parse_options(&argc, &argv);
  if (argc < 1)
    display_usage();
  
  if ((h = gethostbyname(opas_server)) == NULL)
    {
    fprintf(stderr, "%s: unknown host\n", opas_server);
    return 1;
    }
  memcpy((void *)&in_server.s_addr, *h->h_addr_list, h->h_length);
  
  if ((h = gethostbyname(argv[0])) == NULL)
    {
    fprintf(stderr, "%s: unknown host\n", argv[0]);
    return 1;
    }
  memcpy((void *)&in_host.s_addr, *h->h_addr_list, h->h_length);
  
  create_tcp_session(&in_server);
  send_query(&in_host);
  peak_task_run(peak_task_self());
  return 0;
  }

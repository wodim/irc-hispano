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
#define RCSID "$Id: irc_parser.c,v 1.6 2006/09/04 12:48:08 spale Exp $"

/* Here is my exotic/experimental partial IRCU P10 commands parser...
 * Its main goal is performance.
 * If i had more time, i would try with a "static" peak's dictionary
 * and compare the performance, but as IRC doesn't have tons of commands,
 * not sure the internal peak's dictionary hash table can beat a few integer
 * comparaisons (see parser_dispatch())...
 * Some compilers like gcc on i386 worry about multi-character character
 * constant and print a warning by default. You can usually safely ignore them
 * for this file. But if there is a problem, please let me know! --mbuna
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "irc_parser.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cfgloader.h"
#include "debug.h"
#include "pxyservd_log.h"
#include "session.h"
#include "tokenizer.h"

#include "irc_channel.h"
#include "irc_client.h"
#include "irc_membership.h"
#include "irc_militime.h"
#include "irc_mode.h"
#include "irc_msg.h"
#include "irc_network.h"
#include "irc_numnicks.h"
#include "irc_send.h"
#include "irc_userbase.h"
#include "irc_yxx.h"

typedef void (*parse_func)(toktabptr ttab);

#define DEFINE_PARSEFUN(name) static void parse_##name(toktabptr ttab)

DEFINE_PARSEFUN(private);
DEFINE_PARSEFUN(mode);
DEFINE_PARSEFUN(nick);
DEFINE_PARSEFUN(server);
DEFINE_PARSEFUN(quit);
DEFINE_PARSEFUN(squit);
DEFINE_PARSEFUN(kill);
DEFINE_PARSEFUN(ping);
DEFINE_PARSEFUN(pong);
DEFINE_PARSEFUN(rping);
DEFINE_PARSEFUN(eb);
DEFINE_PARSEFUN(ea);
DEFINE_PARSEFUN(burst);
DEFINE_PARSEFUN(create);
DEFINE_PARSEFUN(join);
DEFINE_PARSEFUN(part);
DEFINE_PARSEFUN(kick);

DEFINE_PARSEFUN(null);

/* Helper */
static void burst_clients(struct Channel *channel, char *param);

#define NOT_BURSTING 0
#define BURSTING     1

#if 0
#define RET_BOGUS do { msg_bogus++; return; } while (0)
#else
#define RET_BOGUS do { msg_bogus++; \
                       log_system("irc_parser:%d BOGUS!", __LINE__); \
                       return; } while (0)
#endif

/* Eccentric macros to parse multichannels in one line.
 */
#define FOREACH_CHANNEL_INIT const char *CHANNEL; char *_s; int _cont

#define FOREACH_CHANNEL(_C_,_EXP_) do { \
  for (CHANNEL = _s = (_C_); ; _s++) \
    { \
    if (!*_s || *_s == ',') \
      { \
      _cont = (*_s) ? *_s = '\0', 1 : 0; \
      _EXP_ \
      if (!_cont) \
        break; \
      CHANNEL = ++_s; \
      } \
    }                                 } while (0)  

int msg_bogus; /* exported to cmd_status */
static time_t burst_start, burst_delay; /* fun */


/* Generate an host independent integer representation of the IRC token.
 */
static inline long
parser_mkkey(const char *tok)
  {
  long k;
  
  k = *tok++;
  if (!*tok) /* Most token */
    return k;
  k += *tok++ << 8;
  if (!*tok) /* Some (QS, IR, BE..) */
    return k;
  k += *tok++ << 16;
  if (!*tok) /* None (for now)...*/
    return k;
  k += *tok << 24;
  return k;
  }

/* Get the appropriate parsing function from the token. If the token
 * is not supported by the parser, we return parse_null, for convenience.
 */
static parse_func
parser_dispatch(const char *tok)
  {
  switch (parser_mkkey(tok))
    {
    case 'P':             return parse_private;
    case 'M':             return parse_mode;
    case 'N':             return parse_nick;
    case 'J':             return parse_join;
    case 'Q':             return parse_quit;
    case 'L':             return parse_part;
    case 'K':             return parse_kick;
    case 'S':             return parse_server;
    case ('Q'<<8) | 'S':  return parse_squit;
    case 'D':             return parse_kill;
    case 'G':             return parse_ping;
    case 'Z':             return parse_pong;
    case ('I'<<8) | 'R':  return parse_rping;
    case ('B'<<8) | 'E':  return parse_eb;
    case ('A'<<8) | 'E':  return parse_ea;
    case 'C':             return parse_create;
    case 'B':             return parse_burst;
    default:
      break;
    }
  return parse_null;
  }

void
irc_parser_init()
  {
  tokenizer_init();
  }

void
irc_parser_parse_line(char *msg)
  {
  toktabptr ttab;
  
  Debug((DL_IRCMSG, "[IN]: %s", msg));
  
  ttab = tokenize(msg, 14);
  if (ttab->size < 2)
    {
    Debug((DL_BASIC, "BOGUS IRC MSG (%s)", ttab->tok[0]));
    RET_BOGUS;
    }
  
  if (irc_network_get_remote_server_count() > 0)
    {
    (*parser_dispatch(ttab->tok[1]))(ttab);
    }
  else
    {
    /* Our direct link only */
    msg_bogus = 0;
    
    if (strcmp(ttab->tok[0], MSG_SERVER) == 0)
      {
      yxx numyxx;
      char *p;
      int hub = 0;
      
      if (ttab->size < 8)
        RET_BOGUS;
      
      if (*ttab->tok[7] == '+')
        for (p = ttab->tok[7]; *p; p++)
          {
          if (*p == 'h')
            {
            hub = 1;
            break;
            }
          }
      
      numyxx = yxx_to_int(ttab->tok[6]);
      irc_network_add_server(gMe.nserv, ttab->tok[1], numyxx.server_n,
                             numyxx.client_n,
                             BURSTING, 0, 0);
      
      burst_start = peak_time(); /* = time(0) */
      }
    }
  }

/* CJAAA P BjAAA :blah */
static void
parse_private(toktabptr ttab)
  {
  if (ttab->size < 3)
    RET_BOGUS;
  
  /* Shouldn't receive any channel messages unless the client isn't +d */
  if (!IsChannelName(ttab->tok[2]))
    irc_client_handle_private(ttab);
  }

/* CJAAH M testouil :+w */
static void
parse_mode(toktabptr ttab)
  {
  char *mode_change;
  
  if (ttab->size <= 3)
    RET_BOGUS;
  
  mode_change = ttab->tok[3];
  if (*mode_change == ':')
    mode_change++;
  
  /* Ignore channel modes changes. */
  if (!IsChannelName(ttab->tok[2]))
    irc_mode_handle(ttab->tok[0], mode_change);
  }

/* AB N TomSOYer 4 1070318877 soyer thorongil.arda +i DAqAFk ABAAC :So
 * CJAAA N mbu 1053984498
 */
static void
parse_nick(toktabptr ttab)
  {
  /* Check for nick change */
  if (ttab->size == 4)
    {
    irc_userbase_nick_change(ttab->tok[0], ttab->tok[2]);
    }
  else
    {
    char *ip, *nn, *t1;
    time_t firsttime;
    ClientAddr addr;
    int flags = 0;

    if (ttab->size < 10)
      RET_BOGUS;
    
    firsttime = atoi(ttab->tok[4]);
    
    if (*ttab->tok[7] == '+')
      {
      /* Modes ! */
      for (t1 = ttab->tok[7]; *t1; t1++)
        {
        switch (*t1)
          {
          case 'o':
            flags |= CLIENT_FLAG_OPER;
            break;
          case 'r':
            flags |= CLIENT_FLAG_NICKREG;
            break;
          case 'x':
            flags |= CLIENT_FLAG_HIDDEN;
            break;
#ifdef IRC_HISPANO
          case 'h':
            flags |= CLIENT_FLAG_HELPER;
            break;
          case 'X':
            flags |= CLIENT_FLAG_HDDVIEWER;
            break;
#endif
          default:
            break;
          }
        }

#if 1 /* P10-Hispano */
      ip = ttab->tok[8];      /* base64 IP */
      nn = ttab->tok[9];      /* base64 numnick */
#else
      ip = ttab->tok[k];      /* base64 IP */
      nn = ttab->tok[k + 1];  /* base64 numnick */
#endif
      }
    else
      {
      ip = ttab->tok[7];
      nn = ttab->tok[8];
      }
    
    /* Add IPv6 support here when available.
     */
    if (strlen(ip) != 6)
      RET_BOGUS;
    
    addr.ip4.s_addr = htonl(base64toint(ip));
    irc_userbase_add(ttab->tok[2], ttab->tok[5], ttab->tok[6], firsttime, flags, addr, nn);
    }
  }

static void
parse_server(toktabptr ttab)
  {
  unsigned int from;
  char *t1;
  int hub = 0, service = 0;
  
  if (ttab->size < 9)
    RET_BOGUS;
  
  from = base64toint(ttab->tok[0]);
  t1 = ttab->tok[6];
  
  if (*ttab->tok[8] == '+')
    for (t1 = ttab->tok[8]; *t1; t1++)
      {
      switch (*t1)
        {
        case 'h':
          hub = 1;
          break;
        case 's':
          service = 1;
          break;
        default:
          break;
        }
      }
  
  t1 = ttab->tok[6];
#if 1 /* P10-Hispano */
  if (atoi(t1+1) >= 9)
#else
  if (atoi(t1+1) >= 10)
#endif
    {
    yxx nyxx = yxx_to_int(ttab->tok[7]);
    irc_network_add_server(from, ttab->tok[2], nyxx.server_n, nyxx.client_n,
                           (*t1 == 'J'), hub, service);
    }
  }

static void
parse_quit(toktabptr ttab)
  {
  irc_userbase_remove(ttab->tok[0]);
  }

static void
parse_squit(toktabptr ttab)
  {
  if (ttab->size < 4)
    RET_BOGUS;
  
  irc_network_remove_server_name(ttab->tok[2]);
  }

static void
parse_kill(toktabptr ttab)
  {
  if (ttab->size < 4)
    RET_BOGUS;
  
  irc_userbase_remove(ttab->tok[2]);
  }

/* CJ G !1053960261.525240 proxyscan.undernet.org 1053960261.525240
 * -> CJ G !1053966530.81669 hub5.eu.jeb.com.fr 1053966530.81669
 * <- Bk Z Bk !1053966530.81669 1053966530.81669 95363 1053966625.445512
 */
static void
parse_ping(toktabptr ttab)
  {
  if (irc_network_is_my_downlink(base64toint(ttab->tok[0])))
    {
    char *dest = ttab->tok[2];
    if (*dest == '!' && ttab->size > 4)
      {
      int asll = atoi(militime_float(ttab->tok[4]));
      send_raw("%s %s %s %s %s %d %s" CRLF, gMe.yy, TOK_PONG, gMe.yy,
               ttab->tok[2], ttab->tok[4], asll, militime_float(NULL));
      }
#if 1 /* P10-Hispano */
      /* Como no tiene AsLL ping, hay que
       * poner el ping normal
       */
      else if (ttab->size == 3)
       send_raw("%s %s %s %s" CRLF, gMe.yy, TOK_PONG, gMe.yy,
                ttab->tok[2]);
#endif
    }
  }

static void
parse_pong(toktabptr ttab)
  {
  }

static void
parse_rping(toktabptr ttab)
  {
  struct Server *sptr;
  
  if (ttab->size <= 6)
    RET_BOGUS;
  
  sptr = irc_network_get_server(base64toint(ttab->tok[0]));
  if (sptr != NULL)
    send_raw("%s %s %s %s %s %s %s" CRLF, gMe.yy, TOK_RPONG,
             sptr->name, ttab->tok[3], ttab->tok[4],
             militime(ttab->tok[4], ttab->tok[5]), untokenize(ttab, 6));
  }

static void
parse_eb(toktabptr ttab)
  {
  /* Only reply to EB from our direct downlink. */
  if (irc_network_is_my_downlink(base64toint(ttab->tok[0])))
    {
    burst_delay = peak_time();
    burst_delay -= burst_start;
    
    send_raw("%s %s" CRLF, gMe.yy, TOK_END_OF_BURST_ACK);
    }
  }

static void
parse_ea(toktabptr ttab)
  {
  unsigned int yy_int;
  
  yy_int = base64toint(ttab->tok[0]);
  irc_network_ack_end_of_burst(yy_int);
  
  if (irc_network_is_my_downlink(yy_int))
    {
    size_t recvBytes = peak_stream_get_read_count(gIRCStream);
#ifdef SPANISH
    send_to_console("[%s] Cargados %ld clientes en %ld segundos "
#else
    send_to_console("[%s] Loaded %ld clients in %ld seconds "
#endif
                    "(%ldKB - %4.2fKB/s)",
                    gConfig->server.id, irc_userbase_get_count(), burst_delay,
                    (int)(recvBytes/1024),
                    burst_delay ? (recvBytes/1024.)/burst_delay
                    : recvBytes/1024.);
    }
  }

/* CJ B #Funky 1069176136 +tn BkABs:o,CJAAB
 */
static void
parse_burst(toktabptr ttab)
  {
  struct Channel *channel;
  const char *mode;
  int ch, i;
  int last_arg = 0;
  
  if (ttab->size < 5 || !IsChannelName(ttab->tok[2]))
    RET_BOGUS;
  
  if (!(channel = irc_channel_get(ttab->tok[2])))
    channel = irc_channel_create(ttab->tok[2], atoi(ttab->tok[3]));
  
  for (i = 4; !last_arg && i < ttab->size; i++)
    {
    if (*ttab->tok[i] == ':')
      {
      /* TODO: avoid this hack with the help of a better irc tokenizer!
       */
      ttab->tok[i]++;
      last_arg = 1;
      }
    switch (*ttab->tok[i])
      {
      case '+': /* parameter introduces a mode string */
        for (mode = ttab->tok[i] + 1; (ch = *mode++); )
          {
          switch (ch)
            {
            case 'b':
              abort();
            case 'k':
            case 'l':
              i++;
              continue;
            }
          }
        break;
      case '%':  /* parameter contains bans */
        break;
      case '\0': /* doh */
        break;
      default:  /* parameter contains clients */
        burst_clients(channel, ttab->tok[i]);
        break;
      }
    }
  }

static void
burst_clients(struct Channel *channel, char *param)
  {
  char *p1, *p2;
  struct Client *client;
  int cont = 0;
  yxx client_yxx;
  
  p1 = p2 = param;
  
  do
    {
    switch (*p2)
      {
      case ':': /* skip channel user modes */
        *p2++ = '\0';
        for ( ; *p2 && *p2 != ','; ++p2)
          ;
        if (!*p2)
          cont++;
        /*FALLTHROUGH*/
      case ',':
        *p2 = '\0';
        cont++;
        /*FALLTHROUGH*/
      case '\0':
        client_yxx = yxx_to_int(p1);
        if (client_yxx.bogus)
          RET_BOGUS;
        
        p1 = p2 + 1;
        if ((client = irc_network_find_client(client_yxx)))
          irc_membership_add(channel, client);
        else
          abort();/*tmp*/
        /* else ignore client */
        
        if (cont == 1)    /* only continue if needed */
          {
#if 0
          cont = 0, p2++; /* /!\ this assumes there is no ',' at EOL */
#else
          cont = 0, *p2 = ','; /* ',' could be anything else.. */
#endif
          }
        break;
      default:
        break;
      }
    } while (*p2++);
  }

/*
[IN]: CJAAB C #testChan 1069264461
[IN]: CJAAB L #Funky
[IN]: CJAAB J #Funky 1064841137
[IN]: BkABs K #Funky CJAAB :Jeb
[IN]: AEAA2 C #prout,#pouet 1069332936
[IN]: AEAA6 J 0
*/
static void
parse_create(toktabptr ttab)
  {
  struct Channel *channel;
  struct Client *client;
  time_t timestamp;
  FOREACH_CHANNEL_INIT;
  
  if (ttab->size < 4)
    RET_BOGUS;
  
  timestamp = atoi(ttab->tok[3]);
  
  FOREACH_CHANNEL(ttab->tok[2],
    channel = irc_channel_create(CHANNEL, timestamp);
    if (!(client = irc_network_find_client(yxx_to_int(ttab->tok[0]))))
      abort();
    irc_membership_add(channel, client);
    );
  }

static void
parse_join(toktabptr ttab)
  {
  struct Channel *channel;
  struct Client *client;
  FOREACH_CHANNEL_INIT;
  
  if (ttab->size < 3)
    RET_BOGUS;
  
  if (ttab->tok[2][0] == '0' && ttab->tok[2][1] == '\0')
    {
    /* special channel 0 */
    if (!(client = irc_network_find_client(yxx_to_int(ttab->tok[0]))))
      abort();
    irc_membership_remove_client(client);
    return;
    }

#if 0 /* P10 IRC-HISPANO */
  if (ttab->size < 4)
    RET_BOGUS;
#endif
  
  FOREACH_CHANNEL(ttab->tok[2],
    if (!(channel = irc_channel_get(CHANNEL)))
      {
      /* channel not created. bug? */
      Debug((DL_BASIC, "parse_join: channel %s not created", CHANNEL));
      log_system("parse_join: channel %s not created", CHANNEL);
#if 1 /* P10 IRC-HISPANO */
      channel = irc_channel_create(CHANNEL, 0);
#else
      channel = irc_channel_create(CHANNEL, (time_t)atoi(ttab->tok[3]));
#endif
      }
    if (!(client = irc_network_find_client(yxx_to_int(ttab->tok[0]))))
      abort();
    irc_membership_add(channel, client);
    );
  }

static void
parse_part(toktabptr ttab)
  {
  struct Channel *channel;
  struct Client *client;
  FOREACH_CHANNEL_INIT;
  
  if (ttab->size < 3)
    RET_BOGUS;

  FOREACH_CHANNEL(ttab->tok[2],
    if ((channel = irc_channel_get(CHANNEL)))
      {
      if (!(client = irc_network_find_client(yxx_to_int(ttab->tok[0]))))
        abort();
      irc_membership_remove(channel, client);
      }
    );
  }

/* [IN]: BkABs K #Funky CJAAB :Jeb
 */
static void
parse_kick(toktabptr ttab)
  {
  struct Channel *channel;
  struct Client *client;
  FOREACH_CHANNEL_INIT;
  
  if (ttab->size < 4)
    RET_BOGUS;
  
  FOREACH_CHANNEL(ttab->tok[2],
    if ((channel = irc_channel_get(CHANNEL)))
      {
      if (!(client = irc_network_find_client(yxx_to_int(ttab->tok[3]))))
        abort();
      irc_membership_remove(channel, client);
      }
    );
  }

static void
parse_null(toktabptr ttab)
  {
  }


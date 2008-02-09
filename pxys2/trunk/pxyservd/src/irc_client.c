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
#define RCSID "$Id: irc_client.c,v 1.6 2004/01/17 19:44:29 mbuna Exp $"

#include "irc_client.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <peak/peak.h>

#include "cfgloader.h"
#include "debug.h"
#include "irc_channel.h"
#include "irc_cmd.h"
#include "irc_membership.h"
#include "irc_msg.h"
#include "irc_network.h"
#include "irc_numnicks.h"
#include "irc_send.h"
#include "irc_struct.h"
#include "irc_userbase.h"
#include "pxyservd_log.h"

#if NICKLEN > 9
#define MYNICKLEN 9
#else
#define MYNICKLEN NICKLEN
#endif

extern time_t gBirthTime;

peak_dict cmds_map;

static void irc_client_nick_change_timer_cb(peak_timer ti, void *context);
static void randnick_gen(char *nickbuf, size_t nicklen);

static char my_netnum[6];

static const peak_dict_init_entry my_commands[] =
  {
  { "evreg", cmd_evreg },
  { "evshow", cmd_evshow },
  { "grem", cmd_grem },
  { "help", cmd_help },
  { "info", cmd_info },
  { "inv", cmd_inv },
  { "noscan", cmd_noscan },
  { "proxytop", cmd_proxytop },
  { "pxstats", cmd_pxstats },
  { "recheck", cmd_recheck },
  { "say", cmd_say },
  { "servers", cmd_servers },
  { "showcmds", cmd_showcmds },
  { "stats", cmd_stats },
  { "status", cmd_status }
  };

void
irc_client_init()
  {
  cmds_map = peak_dict_create(&peak_dict_stringcase_key_ops,
                              &peak_dict_null_value_ops,
                              my_commands,
                              PEAK_DICT_INIT_ENTRY_COUNT(my_commands));
  assert(cmds_map != NULL);
  
  if (gConfig->client.nick_change_delay > 0)
    peak_task_timer_add(peak_task_self(),
                        peak_timer_create(gConfig->client.nick_change_delay,
                                          gConfig->client.nick_change_delay,
                                          irc_client_nick_change_timer_cb,
                                          NULL));
  }

void
irc_client_register()
  {
  char addr64[8];
  ClientAddr caddr;
  char *nick;
  char randnick[MYNICKLEN + 1];
  
  inttobase64(addr64, gConfig->client.userip.s_addr, 6);
  snprintf(my_netnum, sizeof(my_netnum), "%s%s", gMe.yy, MYCLIENT_NUM64);
  
  if (gConfig->client.nick_change_delay > 0)
    {
    strncpy(randnick, gConfig->client.nickname, MYNICKLEN);
    randnick[MYNICKLEN] = '\0';
    randnick_gen(randnick, MYNICKLEN);
    nick = randnick;
    }
  else
    nick = gConfig->client.nickname;
  
  send_raw("%s %s %s %d %ld %s %s %s %s %s :%s" CRLF, gMe.yy, TOK_NICK,
           nick, 1, gBirthTime,
           gConfig->client.username, gConfig->client.hostname,
           gConfig->client.usermode, addr64, my_netnum,
           gConfig->client.realname);
  
  caddr.ip4 = gConfig->client.userip;
  
  irc_userbase_add(nick,
                   gConfig->client.username,
                   gConfig->client.hostname,
                   gBirthTime,
                   0,
                   caddr,
                   my_netnum);
  }

void
irc_client_unregister()
  {
  irc_userbase_remove(my_netnum);
  }

void
irc_client_burst()
  {
  struct Channel *ch;
  struct Client *me;
  
  if (!(me = irc_network_find_client(yxx_to_int(my_netnum))))
    return;
  
  /* Burst our client on the console channel */
  if (!(ch = irc_channel_get(gConfig->client.channel)))
    ch = irc_channel_create(gConfig->client.channel, gConfig->client.chanTS);
  
  irc_membership_add(ch, me);
  
  /* Send something concrete to our uplink */
  send_raw("%s %s %s %ld %s %s%s:o" CRLF, gMe.yy, TOK_BURST,
           gConfig->client.channel, gConfig->client.chanTS,
           gConfig->client.chanmode, gMe.yy, MYCLIENT_NUM64);
  }

static void
irc_client_nick_change_timer_cb(peak_timer ti, void *context)
  {
  char new_nick[MYNICKLEN + 1];
  char nn[6];
  
  /* Find an available nickname */
  do
    {
    strncpy(new_nick, gConfig->client.nickname, MYNICKLEN);
    new_nick[MYNICKLEN] = '\0';
    randnick_gen(new_nick, MYNICKLEN);
    }
  while (irc_userbase_get_by_nick(new_nick));
  
  snprintf(nn, sizeof(nn), "%s%s", gMe.yy, MYCLIENT_NUM64);
  irc_userbase_nick_change(nn, new_nick);
  
  send_raw("%s%s %s %s %ld" CRLF, gMe.yy, MYCLIENT_NUM64, TOK_NICK,
           new_nick, peak_time());
  }

static void
randnick_gen(char *nickbuf, size_t nicklen)
  {
  size_t length;
  
  if ((length = strlen(gConfig->client.nickname)) >= nicklen)
    length = nicklen - 2; /* grr */
  
  snprintf(nickbuf + length, nicklen - length + 1, "%lu",
           (unsigned long)rand() * (unsigned long)rand());
  }

void
irc_client_handle_private(toktabptr ttab)
  {
  cmd_func f;
  struct Client *cptr;
  const char *dst = ttab->tok[0];
  const char *cmd = ttab->tok[3];
  char ipbuf[32];
  
  assert(cmd != NULL && ttab->size >= 4);
  if (*cmd == ':')
    cmd++;
  
  if (!*cmd)
    return;
  
  if ((cptr = irc_network_find_client(yxx_to_int(dst))) == NULL)
    return; /* doh */

#ifdef IRC_HISPANO
  if (!((cptr->flags & CLIENT_FLAG_OPER) || (cptr->flags & CLIENT_FLAG_HELPER)))
#else  
  if (!(cptr->flags & CLIENT_FLAG_OPER))
#endif
    return; /* Non opers go play balls. */
  
  if (!inet_ntop(cptr->flags & CLIENT_FLAG_IPV6 ? AF_INET6 : AF_INET,
                 &cptr->addr, ipbuf, sizeof(ipbuf)))
    return;
  
  switch (ttab->size)
    {
    case 4:
      log_write(LOGID_OPERCMDS, "<%s!%s@%s> %s", cptr->nick, cptr->user,
                ipbuf, cmd);
      break;
    case 5:
      log_write(LOGID_OPERCMDS, "<%s!%s@%s> %s %s", cptr->nick, cptr->user,
                ipbuf, cmd, ttab->tok[4]);
      break;
    case 6:
      log_write(LOGID_OPERCMDS, "<%s!%s@%s> %s %s %s", cptr->nick, cptr->user,
                ipbuf, cmd, ttab->tok[4], ttab->tok[5]);
      break;
    default:
      log_write(LOGID_OPERCMDS, "<%s!%s@%s> %s %s %s (...)", cptr->nick,
                cptr->user, ipbuf, cmd, ttab->tok[4], ttab->tok[5]);
      break;
    }
  
  /* Get client pointer here instead of in every cmd handlers.. */
  if ((f = (cmd_func)peak_dict_get_value(cmds_map, cmd)))
    (*f)(cptr, ttab);
  else
#ifdef SPANISH
    send_client_to_one(dst, "Lo siento, comando no encontrado.");
#else
    send_client_to_one(dst, "Sorry, command not found.");
#endif
  }


char *
get_host(struct Client *client, const char *yxx_dest)
{
  struct Client *cptr;

  if (!(client->flags & CLIENT_FLAG_HIDDEN))
    return client->host;

  if (!yxx_dest)
    return "host.hidden";

  if (!(cptr = irc_network_find_client(yxx_to_int(yxx_dest))))
    return "host.hidden";

#ifdef IRC_HISPANO
  if ((cptr == client) ||(cptr->flags & CLIENT_FLAG_HDDVIEWER))
#else
  if ((cptr == client) ||(cptr->flags & CLIENT_FLAG_OPER))
#endif
    return client->host;

  return "host.hidden";
}

char *
get_ip(struct Client *client, const char *yxx_dest)
{
  struct Client *cptr;
  char host[64];
  char *ip_r = NULL;
  int af;


  af = (client->flags & CLIENT_FLAG_IPV6) ? AF_INET6 : AF_INET;
  inet_ntop(af, &client->addr, host, sizeof(host));
  ip_r = host;

  if (!(client->flags & CLIENT_FLAG_HIDDEN))
    return ip_r;

  if (!yxx_dest)
    return "x";

  if (!(cptr = irc_network_find_client(yxx_to_int(yxx_dest))))
    return "x";

  af = (client->flags & CLIENT_FLAG_IPV6) ? AF_INET6 : AF_INET;
  inet_ntop(af, &client->addr, host, sizeof(host));
  ip_r = host;

#ifdef IRC_HISPANO
  if ((cptr == client) ||(cptr->flags & CLIENT_FLAG_HDDVIEWER))
#else
  if ((cptr == client) ||(cptr->flags & CLIENT_FLAG_OPER))
#endif
    return ip_r;

  return "x";
}

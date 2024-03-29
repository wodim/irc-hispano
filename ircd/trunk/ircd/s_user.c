/*
 * IRC-Dev IRCD - An advanced and innovative IRC Daemon, ircd/s_user.c
 *
 * Copyright (C) 2002-2014 IRC-Dev Development Team <devel@irc-dev.net>
 * Copyright (C) 1990 Jarkko Oikarinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/** @file
 * @brief Miscellaneous user-related helper functions.
 * @version $Id$
 */
#include "config.h"

#include "s_user.h"
#include "IPcheck.h"
#include "channel.h"
#include "class.h"
#include "client.h"
#if defined(DDB)
#include "ddb.h"
#endif
#include "hash.h"
#include "ircd.h"
#include "ircd_alloc.h"
#include "ircd_chattr.h"
#include "ircd_features.h"
#include "ircd_log.h"
#include "ircd_reply.h"
#include "ircd_snprintf.h"
#if defined(USE_SSL)
#include "ircd_ssl.h"
#endif
#include "ircd_string.h"
#include "ircd_tea.h"
#include "list.h"
#include "match.h"
#include "motd.h"
#include "msg.h"
#include "msgq.h"
#include "numeric.h"
#include "numnicks.h"
#include "parse.h"
#include "querycmds.h"
#include "random.h"
#include "s_auth.h"
#include "s_bsd.h"
#include "s_conf.h"
#include "s_debug.h"
#include "s_misc.h"
#include "s_serv.h" /* max_client_count */
#include "send.h"
#include "struct.h"
#include "userload.h"
#include "version.h"
#include "watch.h"
#include "whowas.h"

#include "handlers.h" /* m_motd and m_lusers */

/* #include <assert.h> -- Now using assert in ircd_log.h */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/** Count of allocated User structures. */
static int userCount = 0;

static
void send_umode(struct Client *cptr, struct Client *sptr, struct Flags *old,
                int sendset);
static
unsigned int umode_make_snomask(unsigned int oldmask, char *arg, int what);

/** Makes sure that \a cptr has a User information block.
 * If cli_user(cptr) != NULL, does nothing.
 * @param[in] cptr Client to attach User struct to.
 * @return User struct associated with \a cptr.
 */
struct User *make_user(struct Client *cptr)
{
  assert(0 != cptr);

  if (!cli_user(cptr)) {
    cli_user(cptr) = (struct User*) MyMalloc(sizeof(struct User));
    assert(0 != cli_user(cptr));

    /* All variables are 0 by default */
    memset(cli_user(cptr), 0, sizeof(struct User));
    ++userCount;
    cli_user(cptr)->refcnt = 1;
  }
  return cli_user(cptr);
}

/** Dereference \a user.
 * User structures are reference-counted; if the refcount of \a user
 * becomes zero, free it.
 * @param[in] user User to dereference.
 */
void free_user(struct User* user)
{
  assert(0 != user);
  assert(0 < user->refcnt);

  if (--user->refcnt == 0) {
    if (user->away)
      MyFree(user->away);
    /*
     * sanity check
     */
    assert(0 == user->joined);
    assert(0 == user->invited);
    assert(0 == user->channel);

    MyFree(user);
    assert(userCount>0);
    --userCount;
  }
}

/** Find number of User structs allocated and memory used by them.
 * @param[out] count_out Receives number of User structs allocated.
 * @param[out] bytes_out Receives number of bytes used by User structs.
 */
void user_count_memory(size_t* count_out, size_t* bytes_out)
{
  assert(0 != count_out);
  assert(0 != bytes_out);
  *count_out = userCount;
  *bytes_out = userCount * sizeof(struct User);
}


/** Find the next client (starting at \a next) with a name that matches \a ch.
 * Normal usage loop is:
 * for (x = client; x = next_client(x,mask); x = x->next)
 *     HandleMatchingClient;
 *
 * @param[in] next First client to check.
 * @param[in] ch Name mask to check against.
 * @return Next matching client found, or NULL if none.
 */
struct Client *next_client(struct Client *next, const char* ch)
{
  struct Client *tmp = next;

  if (!tmp)
    return NULL;

  next = FindClient(ch);
  next = next ? next : tmp;
  if (cli_prev(tmp) == next)
    return NULL;
  if (next != tmp)
    return next;
  for (; next; next = cli_next(next))
    if (!match(ch, cli_name(next)))
      break;
  return next;
}

/** Find the destination server for a command, and forward it if that is not us.
 *
 * \a server may be a nickname, server name, server mask (if \a from
 * is a local user) or server numnick (if \a is a server or remote
 * user).
 *
 * @param[in] from Client that sent the command to us.
 * @param[in] cmd Long-form command text.
 * @param[in] tok Token-form command text.
 * @param[in] one Client that originated the command (ignored).
 * @param[in] MustBeOper If non-zero and \a from is not an operator, return HUNTED_NOSUCH.
 * @param[in] pattern Format string of arguments to command.
 * @param[in] server Index of target name or mask in \a parv.
 * @param[in] parc Number of valid elements in \a parv (must be less than 9).
 * @param[in] parv Array of arguments to command.
 * @return One of HUNTED_ISME, HUNTED_NOSUCH or HUNTED_PASS.
 */
int hunt_server_cmd(struct Client *from, const char *cmd, const char *tok,
                    struct Client *one, int MustBeOper, const char *pattern,
                    int server, int parc, char *parv[])
{
  struct Client *acptr;
  char *to;

  /* Assume it's me, if no server or an unregistered client */
  if (parc <= server || EmptyString((to = parv[server])) || IsUnknown(from))
    return (HUNTED_ISME);

  if (MustBeOper && !IsPrivileged(from))
  {
    send_reply(from, ERR_NOPRIVILEGES);
    return HUNTED_NOSUCH;
  }

  /* Make sure it's a server */
  if (MyUser(from)) {
    /* Make sure it's a server */
    if (!strchr(to, '*')) {
      if (0 == (acptr = FindClient(to))) {
        send_reply(from, ERR_NOSUCHSERVER, to);
        return HUNTED_NOSUCH;
      }

      if (cli_user(acptr))
        acptr = cli_user(acptr)->server;
    } else if (!(acptr = find_match_server(to))) {
      send_reply(from, ERR_NOSUCHSERVER, to);
      return (HUNTED_NOSUCH);
    }
  } else if (!(acptr = FindNServer(to))) {
    send_reply(from, SND_EXPLICIT | ERR_NOSUCHSERVER, "* :Server has disconnected");
    return (HUNTED_NOSUCH);        /* Server broke off in the meantime */
  }

  if (IsMe(acptr))
    return (HUNTED_ISME);

  if (MustBeOper && !IsPrivileged(from)) {
    send_reply(from, ERR_NOPRIVILEGES);
    return HUNTED_NOSUCH;
  }

  /* assert(!IsServer(from)); */

  parv[server] = (char *) acptr; /* HACK! HACK! HACK! ARGH! */

  sendcmdto_one(from, cmd, tok, acptr, pattern, parv[1], parv[2], parv[3],
                parv[4], parv[5], parv[6], parv[7], parv[8]);

  return (HUNTED_PASS);
}

/** Find the destination server for a command, and forward it (as a
 * high-priority command) if that is not us.
 *
 * \a server may be a nickname, server name, server mask (if \a from
 * is a local user) or server numnick (if \a is a server or remote
 * user).
 * Unlike hunt_server_cmd(), this appends the message to the
 * high-priority message queue for the destination server.
 *
 * @param[in] from Client that sent the command to us.
 * @param[in] cmd Long-form command text.
 * @param[in] tok Token-form command text.
 * @param[in] one Client that originated the command (ignored).
 * @param[in] MustBeOper If non-zero and \a from is not an operator, return HUNTED_NOSUCH.
 * @param[in] pattern Format string of arguments to command.
 * @param[in] server Index of target name or mask in \a parv.
 * @param[in] parc Number of valid elements in \a parv (must be less than 9).
 * @param[in] parv Array of arguments to command.
 * @return One of HUNTED_ISME, HUNTED_NOSUCH or HUNTED_PASS.
 */
int hunt_server_prio_cmd(struct Client *from, const char *cmd, const char *tok,
			 struct Client *one, int MustBeOper,
			 const char *pattern, int server, int parc,
			 char *parv[])
{
  struct Client *acptr;
  char *to;

  /* Assume it's me, if no server or an unregistered client */
  if (parc <= server || EmptyString((to = parv[server])) || IsUnknown(from))
    return (HUNTED_ISME);

  /* Make sure it's a server */
  if (MyUser(from)) {
    /* Make sure it's a server */
    if (!strchr(to, '*')) {
      if (0 == (acptr = FindClient(to))) {
        send_reply(from, ERR_NOSUCHSERVER, to);
        return HUNTED_NOSUCH;
      }

      if (cli_user(acptr))
        acptr = cli_user(acptr)->server;
    } else if (!(acptr = find_match_server(to))) {
      send_reply(from, ERR_NOSUCHSERVER, to);
      return (HUNTED_NOSUCH);
    }
  } else if (!(acptr = FindNServer(to)))
    return (HUNTED_NOSUCH);        /* Server broke off in the meantime */

  if (IsMe(acptr))
    return (HUNTED_ISME);

  if (MustBeOper && !IsPrivileged(from)) {
    send_reply(from, ERR_NOPRIVILEGES);
    return HUNTED_NOSUCH;
  }

  /* assert(!IsServer(from)); SETTIME to particular destinations permitted */

  parv[server] = (char *) acptr; /* HACK! HACK! HACK! ARGH! */

  sendcmdto_prio_one(from, cmd, tok, acptr, pattern, parv[1], parv[2], parv[3],
		     parv[4], parv[5], parv[6], parv[7], parv[8]);

  return (HUNTED_PASS);
}


/*
 * register_user
 *
 * This function is called when both NICK and USER messages
 * have been accepted for the client, in whatever order. Only
 * after this the USER message is propagated.
 *
 * NICK's must be propagated at once when received, although
 * it would be better to delay them too until full info is
 * available. Doing it is not so simple though, would have
 * to implement the following:
 *
 * 1) user telnets in and gives only "NICK foobar" and waits
 * 2) another user far away logs in normally with the nick
 *    "foobar" (quite legal, as this server didn't propagate it).
 * 3) now this server gets nick "foobar" from outside, but
 *    has already the same defined locally. Current server
 *    would just issue "KILL foobar" to clean out dups. But,
 *    this is not fair. It should actually request another
 *    nick from local user or kill him/her...
 */
/** Finish registering a user who has sent both NICK and USER.
 * For local connections, possibly check IAuth; make sure there is a
 * matching Client config block; clean the username field; check
 * K/k-lines; check for "hacked" looking usernames; assign a numnick;
 * and send greeting (WELCOME, ISUPPORT, MOTD, etc).
 * For all connections, update the invisible user and operator counts;
 * run IPcheck against their address; and forward the NICK.
 *
 * @param[in] cptr Client who introduced the user.
 * @param[in,out] sptr Client who has been fully introduced.
 * @return Zero or CPTR_KILLED.
 */
int register_user(struct Client *cptr, struct Client *sptr)
{
  char*            parv[4];
  char*            tmpstr;
  struct User*     user = cli_user(sptr);
  char             ip_base64[25];

  user->last = CurrentTime;
  parv[0] = cli_name(sptr);
  parv[1] = parv[2] = NULL;

  if (MyConnect(sptr))
  {
    assert(cptr == sptr);

    Count_unknownbecomesclient(sptr, UserStats);

    /*
     * Set user's initial modes
     */
    tmpstr = (char*)client_get_default_umode(sptr);
    if (tmpstr) {
      char *umodev[] = { NULL, NULL, NULL, NULL };
      umodev[2] = tmpstr;
      set_user_mode(cptr, sptr, 3, umodev, ALLOWMODES_ANY);
    }

#if defined(USE_SSL)
    if (cli_socket(cptr).s_ssl)
      SetSSL(sptr);
#endif

    SetUser(sptr);

#if defined(ESNET_NEG)
  config_resolve_speculative(cptr);
#endif
    cli_handler(sptr) = CLIENT_HANDLER;
    SetLocalNumNick(sptr);
    send_reply(sptr,
               RPL_WELCOME,
               feature_str(FEAT_NETWORK),
               feature_str(FEAT_PROVIDER) ? " via " : "",
               feature_str(FEAT_PROVIDER) ? feature_str(FEAT_PROVIDER) : "",
               cli_name(sptr));
    /*
     * This is a duplicate of the NOTICE but see below...
     */
    send_reply(sptr, RPL_YOURHOST, cli_name(&me), version);
    send_reply(sptr, RPL_CREATED, creation);
    send_reply(sptr, RPL_MYINFO, cli_name(&me), version, infousermodes,
               infochanmodes, infochanmodeswithparams);

#if defined(USE_SSL)
    if (IsSSL(sptr))
      sendcmdto_one(&me, CMD_NOTICE, sptr, "%C :You are connected to %s with %s", sptr,
                    cli_name(&me), ssl_get_cipher(cli_socket(sptr).s_ssl));
#endif

    send_supported(sptr);
    m_lusers(sptr, sptr, 1, parv);
    m_users(sptr, sptr, 1, parv);
    update_load();
    motd_signon(sptr);
    if (cli_snomask(sptr) & SNO_NOISY)
      set_snomask(sptr, cli_snomask(sptr) & SNO_NOISY, SNO_ADD);
    if (feature_bool(FEAT_CONNEXIT_NOTICES))
      sendto_opmask(0, SNO_CONNEXIT,
                    "Client connecting: %s (%s@%s) [%s] {%s} [%s] <%s%s>",
                    cli_name(sptr), user->username, user->host,
                    cli_sock_ip(sptr), get_client_class(sptr),
                    cli_info(sptr), NumNick(cptr) /* two %s's */);

    IPcheck_connect_succeeded(sptr);
  }
  else {
    struct Client *acptr = user->server;

    if (cli_from(acptr) != cli_from(sptr))
    {
      sendcmdto_one(&me, CMD_KILL, cptr, "%C :%s (%s != %s[%s])",
                    sptr, cli_name(&me), cli_name(user->server), cli_name(cli_from(acptr)),
                    cli_sockhost(cli_from(acptr)));
      SetFlag(sptr, FLAG_KILLED);
      return exit_client(cptr, sptr, &me, "NICK server wrong direction");
    }

    /*
     * Check to see if this user is being propagated
     * as part of a net.burst, or is using protocol 9.
     * FIXME: This can be sped up - its stupid to check it for
     * every NICK message in a burst again  --Run.
     */
    for (; acptr != &me; acptr = cli_serv(acptr)->up)
    {
      if (IsBurst(acptr) || Protocol(acptr) < 10)
        break;
    }
    if (!IPcheck_remote_connect(sptr, (acptr != &me)))
    {
      /*
       * We ran out of bits to count this
       */
      sendcmdto_one(&me, CMD_KILL, sptr, "%C :%s (Too many connections from your host -- Ghost)",
                    sptr, cli_name(&me));
      return exit_client(cptr, sptr, &me,"Too many connections from your host -- throttled");
    }
    SetUser(sptr);
  }

  /* If they get both +x and an account during registration, hide
   * their hostmask here.  Calling hide_hostmask() from IAuth's
   * account assignment causes a numeric reply during registration.
   */
  if (HasHiddenHost(sptr))
    hide_hostmask(sptr, 0, FLAG_HIDDENHOST);
  if (IsInvisible(sptr))
    ++UserStats.inv_clients;
  if (IsOper(sptr))
    ++UserStats.opers;

  tmpstr = umode_str(sptr);

  /* Send full IP address to IPv6-grokking servers. */
  sendcmdto_flag_serv(user->server, CMD_NICK, cptr,
                      FLAG_IPV6, FLAG_LAST_FLAG,
                      "%s %d %Tu %s %s %s%s%s%s %s%s :%s",
                      cli_name(sptr), cli_hopcount(sptr) + 1,
                      cli_lastnick(sptr),
                      user->username, user->realhost,
                      *tmpstr ? "+" : "", tmpstr, *tmpstr ? " " : "",
                      iptobase64(ip_base64, &cli_ip(sptr), sizeof(ip_base64), 1),
                      NumNick(sptr), cli_info(sptr));
  /* Send fake IPv6 addresses to pre-IPv6 servers. */
  sendcmdto_flag_serv(user->server, CMD_NICK, cptr,
                      FLAG_LAST_FLAG, FLAG_IPV6,
                      "%s %d %Tu %s %s %s%s%s%s %s%s :%s",
                      cli_name(sptr), cli_hopcount(sptr) + 1,
                      cli_lastnick(sptr),
                      user->username, user->realhost,
                      *tmpstr ? "+" : "", tmpstr, *tmpstr ? " " : "",
                      iptobase64(ip_base64, &cli_ip(sptr), sizeof(ip_base64), 0),
                      NumNick(sptr), cli_info(sptr));

  /* Send user mode to client */
  if (MyUser(sptr))
  {
    static struct Flags flags; /* automatically initialized to zeros */
#if defined(UNDERNET)
    /* To avoid sending +r to the client due to auth-on-connect, set
     * the "old" FLAG_ACCOUNT bit to match the client's value.
     */
    if (IsAccount(cptr))
      FlagSet(&flags, FLAG_ACCOUNT);
    else
      FlagClr(&flags, FLAG_ACCOUNT);
#endif
    client_set_privs(sptr, NULL);
    send_umode(cptr, sptr, &flags, ALL_UMODES);
    if ((cli_snomask(sptr) != SNO_DEFAULT) && HasFlag(sptr, FLAG_SERVNOTICE))
      send_reply(sptr, RPL_SNOMASK, cli_snomask(sptr), cli_snomask(sptr));
  }

  /* Notify new local user */
  check_status_watch(sptr, RPL_LOGON);

  return 0;
}

/** List of user mode characters. */
static const struct UserMode {
  unsigned int flag; /**< User mode constant. */
  char         c;    /**< Character corresponding to the mode. */
} userModeList[] = {
  { FLAG_OPER,        'o' },
  { FLAG_LOCOP,       'O' },
  { FLAG_INVISIBLE,   'i' },
  { FLAG_WALLOP,      'w' },
  { FLAG_SERVNOTICE,  's' },
  { FLAG_DEAF,        'd' },
  { FLAG_CHSERV,      'k' },
  { FLAG_DEBUG,       'g' },
#if defined(UNDERNET)
  { FLAG_ACCOUNT,     'r' },
#elif defined(DDB) || defined(SERVICES)
  { FLAG_NICKREG,     'r' },
  { FLAG_NICKSUSPEND, 'S' },
  { FLAG_ADMIN,       'a' },
  { FLAG_CODER,       'C' },
  { FLAG_HELPER,      'h' },
  { FLAG_BOT,         'B' },
  { FLAG_USERBITCH,   'P' },
  { FLAG_USERNOJOIN,  'J' },
#endif
  { FLAG_MSGONLYREG,  'R' },
  { FLAG_STRIPCOLOUR, 'c' },
  { FLAG_HIDDENHOST,  'x' },
  { FLAG_VIEWHIDDENHOST, 'X' },
  { FLAG_SSL,         'z' },
  { FLAG_NOCHAN,      'n' },
  { FLAG_NOIDLE,      'I' },
  { FLAG_WHOIS,       'W' }
};

/** Length of #userModeList. */
#define USERMODELIST_SIZE sizeof(userModeList) / sizeof(struct UserMode)

/*
 * XXX - find a way to get rid of this
 */
/** Nasty global buffer used for communications with umode_str() and others. */
static char umodeBuf[BUFSIZE];

#if defined(DDB)
/** Verify the passwords
 * @param[in] nick Nickname
 * @param[in] cryptpass Crypt password
 * @param[in] password Password
 * @return 1 if a correct password, else 0.
 */
int verify_pass_nick(char *nick, char *cryptpass, char *pass)
{
  unsigned int v[2], k[2], x[2];
  int nicklen = strlen(nick);
#if 1
  int cont=(nicklen < 16) ? 2 : ((nicklen + 8) / 8);
#else
  int cont = ((nicklen + 8) / 8);
#endif
  char tmpnick[8 * cont + 1];
  char tmppass[12 + 1];
  unsigned int *p = (unsigned int *)tmpnick;    /* int == 32bits */
  unsigned int numpass[2];

  memset(tmpnick, 0, sizeof(tmpnick));
  strncpy(tmpnick, nick, sizeof(tmpnick) - 1);

  memset(tmppass, 0, sizeof(tmppass));
  strncpy(tmppass, cryptpass, sizeof(tmppass) - 1);

  numpass[1] = base64toint(tmppass + 6);
  tmppass[6] = '\0';
  numpass[0] = base64toint(tmppass);

  memset(tmppass, 0, sizeof(tmppass));

  strncpy(tmppass, pass, sizeof(tmppass) - 1);

  /* relleno   ->   123456789012 */
  strncat(tmppass, "AAAAAAAAAAAA", sizeof(tmppass) - strlen(tmppass) - 1);

  x[0] = x[1] = 0;

  k[1] = base64toint(tmppass + 6);
  tmppass[6] = '\0';
  k[0] = base64toint(tmppass);

  while (cont--)
  {
    v[0] = ntohl(*p++);         /* 32 bits */
    v[1] = ntohl(*p++);         /* 32 bits */
    ircd_tea(v, k, x);
  }

  if ((x[0] == numpass[0]) && (x[1] == numpass[1]))
    return 1;

  return 0;
}
#endif /* defined(DDB) */

/** Try to set a user's nickname.
 * If \a sptr is a server, the client is being introduced for the first time.
 * @param[in] cptr Client to set nickname.
 * @param[in] sptr Client sending the NICK.
 * @param[in] nick New nickname.
 * @param[in] parc Number of arguments to NICK.
 * @param[in] parv Argument list to NICK.
 * @param[in] flags Flags of the nick.
 * @return CPTR_KILLED if \a cptr was killed, else 0.
 */
int set_nick_name(struct Client* cptr, struct Client* sptr,
                  const char* nick, int parc, char* parv[], int flags)
{
#if defined(DDB)
  struct Ddb *ddb;
  int nick_suspend = 0;
  char *botname = 0;

  /*
   * Comprueba si el nick esta registrado, y
   * si lo esta que se ponga la clave correcta
   *
   * Esta comprobacion solo se va a hacer si
   * el usuario es local y no esta haciendo nick flood.
   */
  if (!IsServer(cptr) && MyConnect(cptr) && !IsIdentify(flags)
      && (ddb = ddb_find_key(DDB_NICKDB, (char *)nick))
      && (CurrentTime >= cli_nextnick(sptr)))
  {
    const char *name;
    int nick_forbid = 0;

    botname = ddb_get_botname(DDB_NICKSERV);
    /*
     * Si el ultimo caracter de la clave (bdd->content) contiene:
     *  '+'  El nick esta suspendido.
     *  '*'  El nick esta prohibido (forbid).
     * Cualquier otro caracter, el nick esta activo.
     */
    if (ddb_content(ddb)[strlen(ddb_content(ddb)) - 1] == '+')
      nick_suspend = 1;
    else if (*(ddb_content(ddb)) == '*')
      nick_forbid = 1;

    /*
     * Si el usuario ha hecho ghost, no se necesita
     * verificar la clave de nuevo ya que se ha hecho
     * antes al hacer el ghost. En un nick prohibido
     * tampoco.
     */
    if (!(IsGhost(flags) && !IsIdentify(flags) && !nick_forbid))
    {
      if ((IsNickRegistered(sptr) || IsNickSuspended(sptr))
          && (0 == ircd_strcmp(parv[0], nick)))
      {
        SetNickEquivalent(flags);
        SetIdentify(flags);
      }
      else if (verify_pass_nick(ddb_key(ddb), ddb_content(ddb),
          (parc >= 3 ? parv[2] : cli_passwd(cptr))))
        SetIdentify(flags);
    }

    if (*parv[0])
      name = parv[0];
    else
      name = nick;

    if (IsIdentify(flags))
    {
      /*
       * Si no tiene nick anterior (acaba de conectarse)
       * mandar este notice.
       * El notice de autentificacion de cuando cambia nick
       * esta mas abajo.
       */
      if (!cli_name(cptr)[0] && !IsNickEquivalent(flags))
      {
        sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                         "%s :*** Password accepted. Welcome to %s",
                         nick, feature_str(FEAT_NETWORK));
        if (nick_suspend)
          sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                           "%s :*** Your nick %s is suspended",
                           nick, nick);
      }
    }
    else if (nick_forbid)
    {
      sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                       "%s :*** The nick %s is forbided, cannot be used",
                       name, nick);
      send_reply(cptr, ERR_NICKNAMEINUSE, nick);
      return 0;
    }
    else
    {
      if (parc < 3)
        sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                         "%s :*** The nick %s is Registered, you need password",
                         name, nick);
      else
        sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                         "%s :*** Incorrect password for nick %s",
                         name, nick);

      sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                       "%s :*** It uses \002/NICK %s%spassword\002 for identify",
                       name, nick, IsGhost(flags) ? "!" : ":");

      send_reply(cptr, ERR_NICKNAMEINUSE, nick);
      return 0;
    }
  }
#endif /* defined(DDB) */

  if (IsServer(sptr)) {

    /*
     * A server introducing a new client, change source
     */
    struct Client* new_client = make_client(cptr, STAT_UNKNOWN);
    assert(0 != new_client);

    cli_hopcount(new_client) = atoi(parv[2]);
    cli_lastnick(new_client) = atoi(parv[3]);

    /*
     * Set new nick name.
     */
    strcpy(cli_name(new_client), nick);
    cli_user(new_client) = make_user(new_client);
    cli_user(new_client)->server = sptr;
    SetRemoteNumNick(new_client, parv[parc - 2]);
    /*
     * IP# of remote client
     */
    base64toip(parv[parc - 3], &cli_ip(new_client));

    add_client_to_list(new_client);
    hAddClient(new_client);

    cli_serv(sptr)->ghost = 0;        /* :server NICK means end of net.burst */
    ircd_strncpy(cli_username(new_client), parv[4], USERLEN);
    ircd_strncpy(cli_user(new_client)->username, parv[4], USERLEN);
    ircd_strncpy(cli_user(new_client)->host, parv[5], HOSTLEN);
    ircd_strncpy(cli_user(new_client)->realhost, parv[5], HOSTLEN);
    ircd_strncpy(cli_info(new_client), parv[parc - 1], REALLEN);

    Count_newremoteclient(UserStats, sptr);

    if (parc > 7 && *parv[6] == '+') {
      /* (parc-4) -3 for the ip, numeric nick, realname */
      set_user_mode(cptr, new_client, parc-7, parv+4, ALLOWMODES_ANY);
    }

    /* Notify new remote user */
    check_status_watch(sptr, RPL_LOGOFF);

    return register_user(cptr, new_client);
  }
  else if ((cli_name(sptr))[0]) {
    /*
     * Client changing its nick
     *
     * If the client belongs to me, then check to see
     * if client is on any channels where it is currently
     * banned.  If so, do not allow the nick change to occur.
     */
    if (MyUser(sptr)) {
      const char* channel_name;
      struct Membership *member;
#if defined(DDB)
      char *botname2;
#endif

      if ((channel_name = find_no_nickchange_channel(sptr)) && !IsRenamed(flags)) {
        return send_reply(cptr, ERR_BANNICKCHANGE, channel_name);
      }
      /*
       * Refuse nick change if the last nick change was less
       * then 30 seconds ago. This is intended to get rid of
       * clone bots doing NICK FLOOD. -SeKs
       * If someone didn't change their nick for more then 60 seconds
       * however, allow to do two nick changes immediately after another
       * before limiting the nick flood. -Run
       */
      if ((CurrentTime < cli_nextnick(cptr)) && !IsRenamed(flags))
      {
        cli_nextnick(cptr) += 2;
        send_reply(cptr, ERR_NICKTOOFAST, parv[1],
                   cli_nextnick(cptr) - CurrentTime);
        /* Send error message */
        sendcmdto_one(cptr, CMD_NICK, cptr, "%s", cli_name(cptr));
        /* bounce NICK to user */
        return 0;                /* ignore nick change! */
      }
      else {
        /* Limit total to 1 change per NICK_DELAY seconds: */
        cli_nextnick(cptr) += NICK_DELAY;
        /* However allow _maximal_ 1 extra consecutive nick change: */
        if (cli_nextnick(cptr) < CurrentTime)
          cli_nextnick(cptr) = CurrentTime;
      }
      /* Invalidate all bans against the user so we check them again */
#if defined(DDB)
      botname2 = ddb_get_botname(DDB_CHANSERV);
#endif

      for (member = (cli_user(cptr))->channel; member;
	   member = member->next_channel)
      {
	ClearBanValid(member);
#if defined(DDB)
        if (MyUser(member->user) && IsChanOwner(member))
        {
          ClearChanOwner(member);

          sendcmdbotto_channel(botname2, CMD_MODE,
                               member->channel, NULL, SKIP_SERVERS, "%H -q %C",
                               member->channel, member->user);
          sendcmdto_serv(&me, CMD_BMODE, NULL, "%s %H -q %C",
                         DDB_CHANSERV, member->channel, member->user);
        }
#endif
      }
    }

#if defined(DDB)
    if (IsIdentify(flags) && !IsNickEquivalent(flags))
    {
      if (!botname)
        botname = ddb_get_botname(DDB_NICKSERV);

      sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                       "%C :*** Password accepted. Welcome to %s",
                       cptr, feature_str(FEAT_NETWORK));

      if (nick_suspend)
        sendcmdbotto_one(botname, CMD_NOTICE, cptr,
                         "%C :*** Your nick %s is suspended", cptr, nick);
    }
#endif

    /*
     * Also set 'lastnick' to current time, if changed.
     */
    if (0 != ircd_strcmp(parv[0], nick))
      cli_lastnick(sptr) = (sptr == cptr) ? TStime() : atoi(parv[2]);

    /*
     * Client just changing his/her nick. If he/she is
     * on a channel, send note of change to all clients
     * on that channel. Propagate notice to other servers.
     */
    if (IsUser(sptr)) {
      if (!IsNickEquivalent(flags))
      {
#if defined(DDB)
        struct Flags oldflags;
#endif

        /* Notify exit user */
        check_status_watch(sptr, RPL_LOGOFF);

#if defined(DDB)
        oldflags = cli_flags(sptr);
        ClearNickRegistered(sptr);
        ClearNickSuspended(sptr);

        send_umode_out(cptr, sptr, &oldflags, 0, IsRegistered(sptr));
#endif
      }
      sendcmdto_common_channels(sptr, CMD_NICK, NULL, ":%s", nick);
      add_history(sptr, 1);
      sendcmdto_serv(sptr, CMD_NICK, cptr, "%s %Tu", nick, cli_lastnick(sptr));
    }
    else
      sendcmdto_one(sptr, CMD_NICK, sptr, ":%s", nick);

    if ((cli_name(sptr))[0])
      hRemClient(sptr);
    strcpy(cli_name(sptr), nick);
    hAddClient(sptr);
  }
  else {
    /* Local client setting NICK the first time */
    strcpy(cli_name(sptr), nick);
    hAddClient(sptr);
    return auth_set_nick(cli_auth(sptr), nick);
  }

  if (!IsNickEquivalent(flags))
  {
#if defined(DDB)
    struct Ddb *ddbv;

    ddbv = ddb_find_key(DDB_VHOSTDB, cli_name(sptr));

    if (MyConnect(sptr))
    {
      struct Flags oldflags;

      if (IsIdentify(flags))
      {
        oldflags = cli_flags(sptr);

        if (nick_suspend)
          SetNickSuspended(sptr);
        else
          SetNickRegistered(sptr);

        if (ddbv && !IsHiddenHost(sptr))
          SetHiddenHost(sptr);

        send_umode_out(cptr, sptr, &oldflags, 0, IsRegistered(sptr));
      }
    }
    if (ddbv)
      hide_hostmask(sptr, ddb_content(ddbv), FLAG_HIDDENHOST);

#endif /* defined(DDB) */

    /* Notify change nick local/remote user */
    check_status_watch(sptr, RPL_LOGON);
  }

  return 0;
}

/** Calculate the hash value for a target.
 * @param[in] target Pointer to target, cast to unsigned int.
 * @return Hash value constructed from the pointer.
 */
static unsigned char hash_target(unsigned int target)
{
  return (unsigned char) (target >> 16) ^ (target >> 8);
}

/** Records \a target as a recent target for \a sptr.
 * @param[in] sptr User who has sent to a new target.
 * @param[in] target Target to add.
 */
void
add_target(struct Client *sptr, void *target)
{
  /* Ok, this shouldn't work esp on alpha
  */
  unsigned char  hash = hash_target((unsigned long) target);
  unsigned char* targets;
  int            i;
  assert(0 != sptr);
  assert(cli_local(sptr));

  targets = cli_targets(sptr);

  /*
   * Already in table?
   */
  for (i = 0; i < MAXTARGETS; ++i) {
    if (targets[i] == hash)
      return;
  }
  /*
   * New target
   */
  memmove(&targets[RESERVEDTARGETS + 1],
          &targets[RESERVEDTARGETS], MAXTARGETS - RESERVEDTARGETS - 1);
  targets[RESERVEDTARGETS] = hash;
}

/** Check whether \a sptr can send to or join \a target yet.
 * @param[in] sptr User trying to join a channel or send a message.
 * @param[in] target Target of the join or message.
 * @param[in] name Name of the target.
 * @param[in] created If non-zero, trying to join a new channel.
 * @return Non-zero if too many target changes; zero if okay to send.
 */
int check_target_limit(struct Client *sptr, void *target, const char *name,
    int created)
{
  unsigned char hash = hash_target((unsigned long) target);
  int            i;
  unsigned char* targets;

  assert(0 != sptr);
  assert(cli_local(sptr));
  targets = cli_targets(sptr);

  if (IsChannelService(sptr))
    return 0;

  /*
   * Same target as last time?
   */
  if (targets[0] == hash)
    return 0;
  for (i = 1; i < MAXTARGETS; ++i) {
    if (targets[i] == hash) {
      memmove(&targets[1], &targets[0], i);
      targets[0] = hash;
      return 0;
    }
  }
  /*
   * New target
   */
  if (!created) {
    if (CurrentTime < cli_nexttarget(sptr)) {
      /* If user is invited to channel, give him/her a free target */
      if (IsChannelName(name) && is_invited(sptr, target))
        return 0;

      if (cli_nexttarget(sptr) - CurrentTime < TARGET_DELAY + 8) {
        /*
         * No server flooding
         */
        cli_nexttarget(sptr) += 2;
        send_reply(sptr, ERR_TARGETTOOFAST, name,
                   cli_nexttarget(sptr) - CurrentTime);
      }
      return 1;
    }
    else {
      cli_nexttarget(sptr) += TARGET_DELAY;
      if (cli_nexttarget(sptr) < CurrentTime - (TARGET_DELAY * (MAXTARGETS - 1)))
        cli_nexttarget(sptr) = CurrentTime - (TARGET_DELAY * (MAXTARGETS - 1));
    }
  }
  memmove(&targets[1], &targets[0], MAXTARGETS - 1);
  targets[0] = hash;
  return 0;
}

/** Allows a channel operator to avoid target change checks when
 * sending messages to users on their channel.
 * @param[in] source User sending the message.
 * @param[in] nick Destination of the message.
 * @param[in] channel Name of channel being sent to.
 * @param[in] text Message to send.
 * @param[in] is_notice If non-zero, use CNOTICE instead of CPRIVMSG.
 */
/* Added 971023 by Run. */
int whisper(struct Client* source, const char* nick, const char* channel,
            const char* text, int is_notice)
{
  struct Client*     dest;
  struct Channel*    chptr;
  struct Membership* membership;

  assert(0 != source);
  assert(0 != nick);
  assert(0 != channel);
  assert(MyUser(source));

  if (!(dest = FindUser(nick))) {
    return send_reply(source, ERR_NOSUCHNICK, nick);
  }
  if (!(chptr = FindChannel(channel))) {
    return send_reply(source, ERR_NOSUCHCHANNEL, channel);
  }
  /*
   * compare both users channel lists, instead of the channels user list
   * since the link is the same, this should be a little faster for channels
   * with a lot of users
   */
  for (membership = cli_user(source)->channel; membership; membership = membership->next_channel) {
    if (chptr == membership->channel)
      break;
  }
  if (0 == membership) {
    return send_reply(source, ERR_NOTONCHANNEL, chptr->chname);
  }
  if (!IsVoicedOrOpped(membership)) {
    return send_reply(source, ERR_VOICENEEDED, chptr->chname);
  }
  /*
   * lookup channel in destination
   */
  assert(0 != cli_user(dest));
  for (membership = cli_user(dest)->channel; membership; membership = membership->next_channel) {
    if (chptr == membership->channel)
      break;
  }
  if (0 == membership || IsZombie(membership)) {
    return send_reply(source, ERR_USERNOTINCHANNEL, cli_name(dest), chptr->chname);
  }
  if (is_silenced(source, dest))
    return 0;

#if defined(DDB) || defined(SERVICES)
  if (IsMsgOnlyReg(dest) && !IsNickRegistered(source) && !IsAnOper(source)) {
/*    if(!is_notice) */
      send_reply(source, ERR_NONONREG, cli_name(source));
    return 0;
  }
#endif

  if (is_notice)
    sendcmdto_one(source, CMD_NOTICE, dest, "%C :%s", dest, text);
  else
  {
    if (cli_user(dest)->away)
      send_reply(source, RPL_AWAY, cli_name(dest), cli_user(dest)->away);
    sendcmdto_one(source, CMD_PRIVATE, dest, "%C :%s", dest, text);
  }
  return 0;
}


/** Send a user mode change for \a cptr to neighboring servers.
 * @param[in] cptr User whose mode is changing.
 * @param[in] sptr Client who sent us the mode change message.
 * @param[in] old Prior set of user flags.
 * @param[in] prop If non-zero, also include FLAG_OPER.
 */
void send_umode_out(struct Client *cptr, struct Client *sptr,
                    struct Flags *old, int prop, int isregister)
{
  int i;
  struct Client *acptr;

  send_umode(NULL, sptr, old, prop ? SEND_UMODES : SEND_UMODES_BUT_OPER);

  if (isregister) {
    for (i = HighestFd; i >= 0; i--)
    {
      if ((acptr = LocalClientArray[i]) && IsServer(acptr) &&
          (acptr != cptr) && (acptr != sptr) && *umodeBuf)
        sendcmdto_one(sptr, CMD_MODE, acptr, "%s :%s", cli_name(sptr), umodeBuf);
    }
  }
  if (cptr && MyUser(cptr))
    send_umode(cptr, sptr, old, ALL_UMODES);
}


/** Call \a fmt for each Client named in \a names.
 * @param[in] sptr Client requesting information.
 * @param[in] names Space-delimited list of nicknames.
 * @param[in] rpl Base reply string for messages.
 * @param[in] fmt Formatting callback function.
 */
void send_user_info(struct Client* sptr, char* names, int rpl, InfoFormatter fmt)
{
  char*          name;
  char*          p = 0;
  int            arg_count = 0;
  int            users_found = 0;
  struct Client* acptr;
  struct MsgBuf* mb;

  assert(0 != sptr);
  assert(0 != names);
  assert(0 != fmt);

  mb = msgq_make(sptr, rpl_str(rpl), cli_name(&me), cli_name(sptr));

  for (name = ircd_strtok(&p, names, " "); name; name = ircd_strtok(&p, 0, " ")) {
    if ((acptr = FindUser(name))) {
      if (users_found++)
	msgq_append(0, mb, " ");
      (*fmt)(acptr, sptr, mb);
    }
    if (5 == ++arg_count)
      break;
  }
  send_buffer(sptr, mb, 0);
  msgq_clean(mb);
}

/** Set \a flag on \a cptr and possibly hide the client's hostmask.
 * @param[in,out] cptr User who is getting a new flag.
 * @param[in] vhost Vhost of the user.
 * @param[in] flag Some flag that affects host-hiding (FLAG_HIDDENHOST, FLAG_ACCOUNT).
 * @return Zero.
 */
int
hide_hostmask(struct Client *cptr, const char *vhost, unsigned int flag)
{
  struct Membership *chan;

  switch (flag) {
  case FLAG_HIDDENHOST:
    /* Local users cannot set +x unless FEAT_HOST_HIDING is true. */
    if (MyConnect(cptr) && !feature_bool(FEAT_HOST_HIDING))
      return 0;
    break;
#if defined(UNDERNET)
  case FLAG_ACCOUNT:
    /* Invalidate all bans against the user so we check them again */
    for (chan = (cli_user(cptr))->channel; chan;
         chan = chan->next_channel)
      ClearBanValid(chan);
    break;
#elif defined(DDB) || defined(SERVICES)
  case FLAG_NICKREG:
    /* Invalidate all bans against the user so we check them again */
    for (chan = (cli_user(cptr))->channel; chan;
         chan = chan->next_channel)
      ClearBanValid(chan);
    break;
#endif
  default:
    return 0;
  }

  SetFlag(cptr, flag);
#if defined(UNDERNET)
  if (!HasFlag(cptr, FLAG_HIDDENHOST) || !HasFlag(cptr, FLAG_ACCOUNT))
#elif defined(DDB) || defined(SERVICES)
/* TODO-ZOLTAN: Revisar */
 /*  if (!HasFlag(cptr, FLAG_HIDDENHOST) || !HasFlag(cptr, FLAG_NICKREG)) */
  if (1 == 0)
#else
  if (!HasFlag(cptr, FLAG_HIDDENHOST))
#endif
    return 0;

#if defined(UNDERNET) || defined(SERVICES)
  sendcmdto_common_channels(cptr, CMD_QUIT, cptr, ":Registered");
  ircd_snprintf(0, cli_user(cptr)->host, HOSTLEN, "%s.%s",
# if defined(UNDERNET)
                cli_user(cptr)->account, feature_str(FEAT_HIDDEN_HOST));
# else
                cli_yxx(cptr), feature_str(FEAT_HIDDEN_HOST));
# endif
#else
  sendcmdto_common_channels(cptr, CMD_QUIT, cptr, ":Changing virtualhost");
#if 0
  if (!vhost)
  {
    char ipcrypted[HOSTLEN + 1];

    if (!ip_crypt_key)
    {
      strncpy(ipcrypted, "No.there.is.a.crypt.key", HOSTLEN);
    }
    /* IPv4 */
    else if (irc_in_addr_is_ipv4(&cli_ip(cptr)))
    {
      unsigned int v[2], x[2];
      unsigned int ts = 0;

      do {
        x[0] = x[1] = 0;
        v[0] = (binary_ip_crypt_key[0] & 0xffff0000) + ts;
        v[1] = (ntohl(cli_ip(cptr).in6_16[6]) << 16) | ntohl(cli_ip(cptr).in6_16[7]);

        ircd_tea(v, binary_ip_crypt_key, x);

        /* Virtualhost format: qWeRty.AsDfGh.v4 */
        inttobase64(ipcrypted, x[0], 6);
        ipcrypted[6] = '.';
        inttobase64(ipcrypted + 7, x[1], 6);
        strncpy(ipcrypted + 13, ".v4", HOSTLEN);

        /* No deber�a ocurrir nunca... */
        if (++ts == 65535)
        {
          strncpy(ipcrypted, cli_user(cptr)->realhost, HOSTLEN);
          strncat(ipcrypted, ".v4", HOSTLEN);
          break;
        }
      } while (strchr(ipcrypted, ']') || strchr(ipcrypted, '['));
    }
    /* IPv6 */
    else
    {
      strncpy(ipcrypted, cli_user(cptr)->realhost, HOSTLEN);
      strncat(ipcrypted, ".v6", HOSTLEN);
    }
    ircd_snprintf(0, cli_user(cptr)->host, HOSTLEN, ipcrypted);
  }
  else
#endif
    ircd_snprintf(0, cli_user(cptr)->host, HOSTLEN, vhost);
#endif

  /* ok, the client is now fully hidden, so let them know -- hikari */
  if (MyConnect(cptr))
   send_reply(cptr, RPL_HOSTHIDDEN, cli_user(cptr)->host);

  /*
   * Go through all channels the client was on, rejoin him
   * and set the modes, if any
   */
  for (chan = cli_user(cptr)->channel; chan; chan = chan->next_channel)
  {
    if (IsZombie(chan))
      continue;
    /* Send a JOIN unless the user's join has been delayed. */
    if (!IsDelayedJoin(chan))
      sendcmdto_channel(cptr, CMD_JOIN, chan->channel, cptr, SKIP_SERVERS,
                        "%H", chan->channel);

#if defined(DDB) || defined(SERVICES)
    if (IsChanOwner(chan) || IsChanOp(chan) || HasVoice(chan))
      /* TODO-ZOLTAN: Optimizar esto */
      sendcmdto_channel(&his, CMD_MODE, chan->channel, cptr, SKIP_SERVERS,
                        "%H +%s%s%s %s%s%s%s%s", chan->channel,
                        IsChanOwner(chan) ? "q" : "",
                        IsChanOp(chan) ? "o" : "",
                        HasVoice(chan) ? "v" : "",
                        IsChanOwner(chan) ? cli_name(cptr) : "",
                        IsChanOwner(chan) ? " " : "",
                        IsChanOp(chan) ? cli_name(cptr) : "",
                        IsChanOp(chan) ? " " : "",
                        HasVoice(chan) ? cli_name(cptr) : "");
#else
    if (IsChanOp(chan) && HasVoice(chan))
      sendcmdto_channel(&his, CMD_MODE, chan->channel, cptr, SKIP_SERVERS,
                        "%H +ov %C %C", chan->channel, cptr, cptr);
    else if (IsChanOp(chan))
      sendcmdto_channel(&his, CMD_MODE, chan->channel, cptr, SKIP_SERVERS,
                        "%H +o %C", chan->channel, cptr);
    else if (HasVoice(chan))
      sendcmdto_channel(&his, CMD_MODE, chan->channel, cptr, SKIP_SERVERS,
                        "%H +v %C", chan->channel, cptr);
#endif
  }
  return 0;
}

/**
 * Check to see if this resembles a sno_mask.  It is if 1) there is
 * at least one digit and 2) The first digit occurs before the first
 * alphabetic character.
 * @param[in] word Word to check for sno_mask-ness.
 * @return Non-zero if \a word looks like a server notice mask; zero if not.
 */
static
int is_snomask(char *word)
{
  if (word)
  {
    for (; *word; word++)
      if (IsDigit(*word))
        return 1;
      else if (IsAlpha(*word))
        return 0;
  }
  return 0;
}

/** Set a user's mode.  This function checks that \a cptr is trying to
 * set his own mode, prevents local users from setting inappropriate
 * modes through this function, and applies any other side effects of
 * a successful mode change.
 *
 * @param[in,out] cptr User setting someone's mode.
 * @param[in] sptr Client who sent the mode change message.
 * @param[in] parc Number of parameters in \a parv.
 * @param[in] parv Parameters to MODE.
 * @param[in] allow_modes ALLOWMODES_ANY for any mode, ALLOWMODES_DEFAULT for
 *                        only permitting legitimate default user modes.
 * @return Zero.
 */
int set_user_mode(struct Client *cptr, struct Client *sptr, int parc,
		char *parv[], int allow_modes)
{
  char** p;
  char*  m;
  int what;
  unsigned int i;
  struct Flags setflags;
  unsigned int tmpmask = 0;
  int snomask_given = 0;
  char buf[BUFSIZE];
  int prop = 0;
  int do_host_hiding = 0;
#if defined(UNDERNET)
  char* account = NULL;
#endif

  what = MODE_ADD;

  if (parc < 3)
  {
    m = buf;
    *m++ = '+';
    for (i = 0; i < USERMODELIST_SIZE; i++)
    {
      if (HasFlag(sptr, userModeList[i].flag)
#if defined(UNDERNET)
	 && (userModeList[i].flag != FLAG_ACCOUNT)
#endif
	 )
        *m++ = userModeList[i].c;
    }
    *m = '\0';
    send_reply(sptr, RPL_UMODEIS, buf);
    if (HasFlag(sptr, FLAG_SERVNOTICE) && MyConnect(sptr)
        && cli_snomask(sptr) !=
        (unsigned int)(IsAnOper(sptr) ? SNO_OPERDEFAULT : SNO_DEFAULT))
      send_reply(sptr, RPL_SNOMASK, cli_snomask(sptr), cli_snomask(sptr));
    return 0;
  }

  /*
   * find flags already set for user
   * why not just copy them?
   */
  setflags = cli_flags(sptr);

  if (MyConnect(sptr))
    tmpmask = cli_snomask(sptr);

  /*
   * parse mode change string(s)
   */
  for (p = &parv[2]; *p && p<&parv[parc]; p++) {       /* p is changed in loop too */
    for (m = *p; *m; m++) {
      switch (*m) {
      case '+':
        what = MODE_ADD;
        break;
      case '-':
        what = MODE_DEL;
        break;
      case 's':
        if (*(p + 1) && is_snomask(*(p + 1))) {
          snomask_given = 1;
          tmpmask = umode_make_snomask(tmpmask, *++p, what);
          tmpmask &= (IsAnOper(sptr) ? SNO_ALL : SNO_USER);
        }
        else
          tmpmask = (what == MODE_ADD) ?
              (IsAnOper(sptr) ? SNO_OPERDEFAULT : SNO_DEFAULT) : 0;
        if (tmpmask)
	  SetServNotice(sptr);
        else
	  ClearServNotice(sptr);
        break;
      case 'w':
        if (what == MODE_ADD)
          SetWallops(sptr);
        else
          ClearWallops(sptr);
        break;
      case 'o':
        if (what == MODE_ADD)
          SetOper(sptr);
        else {
          ClrFlag(sptr, FLAG_OPER);
          ClrFlag(sptr, FLAG_LOCOP);
          if (MyConnect(sptr) && !IsAnOper(sptr))
          {
            tmpmask = cli_snomask(sptr) & ~SNO_OPER;
            cli_handler(sptr) = CLIENT_HANDLER;
          }
        }
        break;
      case 'O':
        if (what == MODE_ADD)
          SetLocOp(sptr);
        else
        {
          ClrFlag(sptr, FLAG_OPER);
          ClrFlag(sptr, FLAG_LOCOP);
          if (MyConnect(sptr) && !IsAnOper(sptr))
          {
            tmpmask = cli_snomask(sptr) & ~SNO_OPER;
            cli_handler(sptr) = CLIENT_HANDLER;
          }
        }
        break;
      case 'i':
        if (what == MODE_ADD)
          SetInvisible(sptr);
        else
          ClearInvisible(sptr);
        break;
      case 'd':
        if (what == MODE_ADD)
          SetDeaf(sptr);
        else
          ClearDeaf(sptr);
        break;
      case 'k':
        if (what == MODE_ADD)
          SetChannelService(sptr);
        else
          ClearChannelService(sptr);
        break;
      case 'g':
        if (what == MODE_ADD)
          SetDebug(sptr);
        else
          ClearDebug(sptr);
        break;
      case 'x':
        if (what == MODE_ADD)
	  do_host_hiding = 1;
	break;
      case 'X':
        if (what == MODE_ADD)
          SetViewHiddenHost(sptr);
	      else
	        ClearViewHiddenHost(sptr);
	      break;
#if defined(UNDERNET)
      case 'r':
	if (*(p + 1) && (what == MODE_ADD)) {
	  account = *(++p);
	  SetAccount(sptr);
	}
	/* There is no -r */
	break;
#endif
#if defined(DDB) || defined(SERVICES)
      case 'r':
        if (what == MODE_ADD)
          SetNickRegistered(sptr);
        else
          ClearNickRegistered(sptr);
        break;
      case 'S':
        if (what == MODE_ADD)
          SetNickSuspended(sptr);
        else
          ClearNickSuspended(sptr);
        break;
      case 'a':
        if (what == MODE_ADD)
          SetAdmin(sptr);
        else
          ClearAdmin(sptr);
        break;
      case 'N':
        if (what == MODE_ADD)
          SetCoder(sptr);
      else
          ClearCoder(sptr);
      break;
      case 'h':
        if (what == MODE_ADD)
          SetHelpOper(sptr);
        else
          ClearHelpOper(sptr);
        break;
      case 'B':
        if (what == MODE_ADD)
          SetBot(sptr);
        else
          ClearBot(sptr);
        break;
      case 'D':
        if (what == MODE_ADD)
          SetUserDeaf(sptr);
        else
          ClearUserDeaf(sptr);
        break;
      case 'P':
        if (what == MODE_ADD)
          SetUserBlind(sptr);
        else
          ClearUserBlind(sptr);
        break;
      case 'C':
        if (what == MODE_ADD)
          SetUserNoJoin(sptr);
        else
          ClearUserNoJoin(sptr);
        break;
#endif /* defined(DDB) || defined(SERVICES) */
      case 'R':
        if (what == MODE_ADD)
          SetMsgOnlyReg(sptr);
        else
          ClearMsgOnlyReg(sptr);
        break;
      case 'c':
        if (what == MODE_ADD)
          SetStripColour(sptr);
        else
          ClearStripColour(sptr);
        break;
      case 'Z':
        if (what == MODE_ADD)
          SetSSL(sptr);
        else
          ClearSSL(sptr);
        break;

       /* if none of the given case is valid then compain by
        * sending raw 501 ( ERR_UMODEUNKNOWNFLAG )
        */
      default:
        send_reply(sptr, ERR_UMODEUNKNOWNFLAG, *m);
        break;
      }
    }
  }
  /*
   * Evaluate rules for new user mode
   * Stop users making themselves operators too easily:
   */
  if (!IsServer(cptr))
  {
#if defined(DDB1)
  /* Si es admin puede hacerse ircop en cualquier momento */
  if (MyUser(sptr) && (!(setflags & FLAGS_OPER))
      && IsOper(sptr))
  {
    struct db_reg *reg = db_buscar_registro(BDD_OPERDB, sptr->name);
    if (!reg || atoi(reg->valor) < 10)
      ClearOper(sptr);
  }
#else
    if (!FlagHas(&setflags, FLAG_OPER) && IsOper(sptr))
      ClearOper(sptr);
#endif
    if (!FlagHas(&setflags, FLAG_LOCOP) && IsLocOp(sptr))
      ClearLocOp(sptr);
#if defined(UNDERNET)
    if (!FlagHas(&setflags, FLAG_ACCOUNT) && IsAccount(sptr))
      ClrFlag(sptr, FLAG_ACCOUNT);
#endif
    /*
     * new umode; servers can set it, local users cannot;
     * prevents users from /kick'ing or /mode -o'ing
     */
    if (!FlagHas(&setflags, FLAG_CHSERV) && !HasPriv(sptr, PRIV_CHANSERV))
      ClearChannelService(sptr);
    if (!FlagHas(&setflags, FLAG_VIEWHIDDENHOST) && HasPriv(sptr, PRIV_SEE_HIDDEN_HOSTS))
      ClearViewHiddenHost(sptr);
    /*
     * only send wallops to opers
     */
    if (feature_bool(FEAT_WALLOPS_OPER_ONLY) && !IsAnOper(sptr) &&
	!FlagHas(&setflags, FLAG_WALLOP))
      ClearWallops(sptr);
    if (feature_bool(FEAT_HIS_SNOTICES_OPER_ONLY) && MyConnect(sptr) &&
        !IsAnOper(sptr) && !FlagHas(&setflags, FLAG_SERVNOTICE))
    {
      ClearServNotice(sptr);
      set_snomask(sptr, 0, SNO_SET);
    }
    if (feature_bool(FEAT_HIS_DEBUG_OPER_ONLY) &&
        !IsAnOper(sptr) && !FlagHas(&setflags, FLAG_DEBUG))
      ClearDebug(sptr);
    if (!FlagHas(&setflags, FLAG_SSL) && IsSSL(sptr))
      ClearSSL(sptr);
#if defined(DDB) || defined(SERVICES)
    if (!FlagHas(&setflags, FLAG_NICKREG) && IsNickRegistered(sptr))
      ClearNickRegistered(sptr);
    if (!FlagHas(&setflags, FLAG_NICKSUSPEND) && IsNickSuspended(sptr))
      ClearNickSuspended(sptr);
    if (!FlagHas(&setflags, FLAG_ADMIN))
      ClearAdmin(sptr);
    if (!FlagHas(&setflags, FLAG_CODER))
      ClearCoder(sptr);
    if (!FlagHas(&setflags, FLAG_HELPER))
      ClearHelpOper(sptr);
    if (!FlagHas(&setflags, FLAG_BOT))
      ClearBot(sptr);
#endif /* defined(DDB) || defined(SERVICES) */
  }
  if (MyConnect(sptr))
  {
    if ((FlagHas(&setflags, FLAG_OPER) || FlagHas(&setflags, FLAG_LOCOP)) &&
        !IsAnOper(sptr))
    {
      det_confs_butmask(sptr, CONF_CLIENT & ~CONF_OPERATOR);
      client_set_privs(sptr, NULL);
    }

    if (SendServNotice(sptr))
    {
      if (tmpmask != cli_snomask(sptr))
	set_snomask(sptr, tmpmask, SNO_SET);
      if (cli_snomask(sptr) && snomask_given)
	send_reply(sptr, RPL_SNOMASK, cli_snomask(sptr), cli_snomask(sptr));
    }
    else
      set_snomask(sptr, 0, SNO_SET);
  }
  /*
   * Compare new flags with old flags and send string which
   * will cause servers to update correctly.
   */
#if defined(UNDERNET)
  if (!FlagHas(&setflags, FLAG_ACCOUNT) && IsAccount(sptr)) {
      int len = ACCOUNTLEN;
      char *ts;
      if ((ts = strchr(account, ':'))) {
	len = (ts++) - account;
	cli_user(sptr)->acc_create = atoi(ts);
	Debug((DEBUG_DEBUG, "Received timestamped account in user mode; "
	      "account \"%s\", timestamp %Tu", account,
	      cli_user(sptr)->acc_create));
      }
      ircd_strncpy(cli_user(sptr)->account, account, len);
  }
#endif
  if (!FlagHas(&setflags, FLAG_HIDDENHOST) && do_host_hiding && allow_modes != ALLOWMODES_DEFAULT)
    hide_hostmask(sptr, 0, FLAG_HIDDENHOST); /* TODO-ZOLTAN: Revisar */

  if (IsRegistered(sptr)) {
    if (!FlagHas(&setflags, FLAG_OPER) && IsOper(sptr)) {
      /* user now oper */
      ++UserStats.opers;
      client_set_privs(sptr, NULL); /* may set propagate privilege */
    }
    /* remember propagate privilege setting */
    if (HasPriv(sptr, PRIV_PROPAGATE)) {
      prop = 1;
    }
    if (FlagHas(&setflags, FLAG_OPER) && !IsOper(sptr)) {
      /* user no longer oper */
      assert(UserStats.opers > 0);
      --UserStats.opers;
      client_set_privs(sptr, NULL); /* will clear propagate privilege */
    }
    if (FlagHas(&setflags, FLAG_INVISIBLE) && !IsInvisible(sptr)) {
      assert(UserStats.inv_clients > 0);
      --UserStats.inv_clients;
    }
    if (!FlagHas(&setflags, FLAG_INVISIBLE) && IsInvisible(sptr)) {
      ++UserStats.inv_clients;
    }
    assert(UserStats.opers <= UserStats.clients + UserStats.unknowns);
    assert(UserStats.inv_clients <= UserStats.clients + UserStats.unknowns);
    send_umode_out(cptr, sptr, &setflags, prop, IsRegistered(sptr));
  }

  return 0;
}

/** Build a mode string to describe modes for \a cptr.
 * @param[in] cptr Some user.
 * @return Pointer to a static buffer.
 */
char *umode_str(struct Client *cptr)
{
  /* Maximum string size: "owidgrx\0" */
  char *m = umodeBuf;
  unsigned int i;
  struct Flags c_flags = cli_flags(cptr);

  if (!HasPriv(cptr, PRIV_PROPAGATE))
    FlagClr(&c_flags, FLAG_OPER);

  for (i = 0; i < USERMODELIST_SIZE; ++i)
  {
    if (FlagHas(&c_flags, userModeList[i].flag) &&
        userModeList[i].flag >= FLAG_GLOBAL_UMODES)
      *m++ = userModeList[i].c;
  }

#if defined(UNDERNET)
  if (IsAccount(cptr))
  {
    char* t = cli_user(cptr)->account;

    *m++ = ' ';
    while ((*m++ = *t++))
      ; /* Empty loop */

    if (cli_user(cptr)->acc_create) {
      char nbuf[20];
      Debug((DEBUG_DEBUG, "Sending timestamped account in user mode for "
	     "account \"%s\"; timestamp %Tu", cli_user(cptr)->account,
	     cli_user(cptr)->acc_create));
      ircd_snprintf(0, t = nbuf, sizeof(nbuf), ":%Tu",
		    cli_user(cptr)->acc_create);
      m--; /* back up over previous nul-termination */
      while ((*m++ = *t++))
	; /* Empty loop */
    }
  }
#endif

  *m = '\0';

  return umodeBuf;                /* Note: static buffer, gets
                                   overwritten by send_umode() */
}

/** Send a mode change string for \a sptr to \a cptr.
 * @param[in] cptr Destination of mode change message.
 * @param[in] sptr User whose mode has changed.
 * @param[in] old Pre-change set of modes for \a sptr.
 * @param[in] sendset One of ALL_UMODES, SEND_UMODES_BUT_OPER,
 * SEND_UMODES, to select which changed user modes to send.
 */
static
void send_umode(struct Client *cptr, struct Client *sptr, struct Flags *old,
                int sendset)
{
  unsigned int i;
  int flag;
  char *m;
  int what = MODE_NULL;

  /*
   * Build a string in umodeBuf to represent the change in the user's
   * mode between the new (cli_flags(sptr)) and 'old', but skipping
   * the modes indicated by sendset.
   */
  m = umodeBuf;
  *m = '\0';
  for (i = 0; i < USERMODELIST_SIZE; ++i)
  {
    flag = userModeList[i].flag;
    if (FlagHas(old, flag)
        == HasFlag(sptr, flag))
      continue;
    switch (sendset)
    {
    case ALL_UMODES:
      break;
    case SEND_UMODES_BUT_OPER:
      if (flag == FLAG_OPER)
        continue;
      /* and fall through */
    case SEND_UMODES:
      if (flag < FLAG_GLOBAL_UMODES)
        continue;
      break;
    }
    if (FlagHas(old, flag))
    {
      if (what == MODE_DEL)
        *m++ = userModeList[i].c;
      else
      {
        what = MODE_DEL;
        *m++ = '-';
        *m++ = userModeList[i].c;
      }
    }
    else /* !FlagHas(old, flag) */
    {
      if (what == MODE_ADD)
        *m++ = userModeList[i].c;
      else
      {
        what = MODE_ADD;
        *m++ = '+';
        *m++ = userModeList[i].c;
      }
    }
  }
  *m = '\0';
  if (*umodeBuf && cptr)
    sendcmdto_one(sptr, CMD_MODE, cptr, "%s :%s", cli_name(sptr), umodeBuf);
}


/** Update snomask \a oldmask according to \a arg and \a what.
 * @param[in] oldmask Original user mask.
 * @param[in] arg Update string (either a number or '+'/'-' followed by a number).
 * @param[in] what MODE_ADD if adding the mask.
 * @return New value of service notice mask.
 */
static
unsigned int umode_make_snomask(unsigned int oldmask, char *arg, int what)
{
  unsigned int sno_what;
  unsigned int newmask;
  if (*arg == '+')
  {
    arg++;
    if (what == MODE_ADD)
      sno_what = SNO_ADD;
    else
      sno_what = SNO_DEL;
  }
  else if (*arg == '-')
  {
    arg++;
    if (what == MODE_ADD)
      sno_what = SNO_DEL;
    else
      sno_what = SNO_ADD;
  }
  else
    sno_what = (what == MODE_ADD) ? SNO_SET : SNO_DEL;
  /* pity we don't have strtoul everywhere */
  newmask = (unsigned int)atoi(arg);
  if (sno_what == SNO_DEL)
    newmask = oldmask & ~newmask;
  else if (sno_what == SNO_ADD)
    newmask |= oldmask;
  return newmask;
}

/** Remove \a cptr from the singly linked list \a list.
 * @param[in] cptr Client to remove from list.
 * @param[in,out] list Pointer to head of list containing \a cptr.
 */
static void delfrom_list(struct Client *cptr, struct SLink **list)
{
  struct SLink* tmp;
  struct SLink* prv = NULL;

  for (tmp = *list; tmp; tmp = tmp->next) {
    if (tmp->value.cptr == cptr) {
      if (prv)
        prv->next = tmp->next;
      else
        *list = tmp->next;
      free_link(tmp);
      break;
    }
    prv = tmp;
  }
}

/** Set \a cptr's server notice mask, according to \a what.
 * @param[in,out] cptr Client whose snomask is updating.
 * @param[in] newmask Base value for new snomask.
 * @param[in] what One of SNO_ADD, SNO_DEL, SNO_SET, to choose operation.
 */
void set_snomask(struct Client *cptr, unsigned int newmask, int what)
{
  unsigned int oldmask, diffmask;        /* unsigned please */
  int i;
  struct SLink *tmp;

  oldmask = cli_snomask(cptr);

  if (what == SNO_ADD)
    newmask |= oldmask;
  else if (what == SNO_DEL)
    newmask = oldmask & ~newmask;
  else if (what != SNO_SET)        /* absolute set, no math needed */
    sendto_opmask(0, SNO_OLDSNO, "setsnomask called with %d ?!", what);

  newmask &= (IsAnOper(cptr) ? SNO_ALL : SNO_USER);

  diffmask = oldmask ^ newmask;

  for (i = 0; diffmask >> i; i++) {
    if (((diffmask >> i) & 1))
    {
      if (((newmask >> i) & 1))
      {
        tmp = make_link();
        tmp->next = opsarray[i];
        tmp->value.cptr = cptr;
        opsarray[i] = tmp;
      }
      else
        /* not real portable :( */
        delfrom_list(cptr, &opsarray[i]);
    }
  }
  cli_snomask(cptr) = newmask;
}

/** Check whether \a sptr is allowed to send a message to \a acptr.
 * If \a sptr is a remote user, it means some server has an outdated
 * SILENCE list for \a acptr, so send the missing SILENCE mask(s) back
 * in the direction of \a sptr.  Skip the check if \a sptr is a server.
 * @param[in] sptr Client trying to send a message.
 * @param[in] acptr Destination of message.
 * @return Non-zero if \a sptr is SILENCEd by \a acptr, zero if not.
 */
int is_silenced(struct Client *sptr, struct Client *acptr)
{
  struct Ban *found;
  struct User *user;
  size_t buf_used, slen;
  char buf[BUFSIZE];

  if (IsServer(sptr) || !(user = cli_user(acptr))
      || !(found = find_ban(sptr, user->silence)))
    return 0;
  assert(!(found->flags & BAN_EXCEPTION));
  if (!MyConnect(sptr)) {
    /* Buffer positive silence to send back. */
    buf_used = strlen(found->banstr);
    memcpy(buf, found->banstr, buf_used);
    /* Add exceptions to buffer. */
    for (found = user->silence; found; found = found->next) {
      if (!(found->flags & BAN_EXCEPTION))
        continue;
      slen = strlen(found->banstr);
      if (buf_used + slen + 4 > 400) {
        buf[buf_used] = '\0';
        sendcmdto_one(acptr, CMD_SILENCE, cli_from(sptr), "%C %s", sptr, buf);
        buf_used = 0;
      }
      if (buf_used)
        buf[buf_used++] = ',';
      buf[buf_used++] = '+';
      buf[buf_used++] = '~';
      memcpy(buf + buf_used, found->banstr, slen);
      buf_used += slen;
    }
    /* Flush silence buffer. */
    if (buf_used) {
      buf[buf_used] = '\0';
      sendcmdto_one(acptr, CMD_SILENCE, cli_from(sptr), "%C %s", sptr, buf);
      buf_used = 0;
    }
  }
  return 1;
}

/** Describes one element of the ISUPPORT list. */
struct ISupport {
    const char *is_name; /**< Name of supported feature. */
    enum {
        OPT_NONE,
        OPT_INT,
        OPT_STRING
    } is_type; /**< Type of the feature's value. */
    union {
        int iv;
        char *sv;
    } is_value; /**< Feature's value. */
    struct ISupport *is_next; /**< Pointer to next feature. */
};

static struct ISupport *isupport; /**< List of supported ISUPPORT features. */
static struct SLink *isupport_lines; /**< List of formatted ISUPPORT lines. */

/** Mark #isupport_lines as dirty and needing a rebuild. */
static void
touch_isupport()
{
  while (isupport_lines) {
    struct SLink *link = isupport_lines;
    isupport_lines = link->next;
    MyFree(link->value.cp);
    free_link(link);
  }
}

/** Get (or create) an ISupport element from #isupport with the
 * specified name and OPT_NONE type.
 * @param[in] name Name of ISUPPORT feature to describe.
 * @return Pre-existing or newly allocated ISupport structure.
 */
static struct ISupport *
get_clean_isupport(const char *name)
{
  struct ISupport *isv, *prev;

  for (isv = isupport, prev = 0; isv; prev = isv, isv = isv->is_next) {
    if (strcmp(isv->is_name, name))
      continue;
    if (isv->is_type == OPT_STRING)
      MyFree(isv->is_value.sv);
    break;
  }

  if (!isv) {
    isv = MyMalloc(sizeof(*isv));
    if (prev)
        prev->is_next = isv;
    else
        isupport = isv;
    isv->is_next = NULL;
  }

  isv->is_name = name;
  isv->is_type = OPT_NONE;
  touch_isupport();
  return isv;
}

/** Declare support for a feature with no parameter.
 * @param[in] name Name of ISUPPORT feature to announce.
 */
static
void add_isupport(const char *name)
{
  get_clean_isupport(name);
}

/** Declare support for a feature with an integer parameter.
 * @param[in] name Name of ISUPPORT feature to announce.
 * @param[in] value Value associated with the feature.
 */
void add_isupport_i(const char *name, int value)
{
  struct ISupport *isv = get_clean_isupport(name);
  isv->is_type = OPT_INT;
  isv->is_value.iv = value;
}

/** Declare support for a feature with a string parameter.
 * @param[in] name Name of ISUPPORT feature to announce.
 * @param[in] value Value associated with the feature.
 */
void add_isupport_s(const char *name, const char *value)
{
  struct ISupport *isv = get_clean_isupport(name);
  isv->is_type = OPT_STRING;
  DupString(isv->is_value.sv, value);
}

/** Stop announcing support for a feature.
 * @param[in] name Name of ISUPPORT feature to revoke.
 */
void del_isupport(const char *name)
{
  struct ISupport *isv, *prev;

  for (isv = isupport, prev = 0; isv; prev = isv, isv = isv->is_next) {
    if (strcmp(isv->is_name, name))
      continue;
    if (isv->is_type == OPT_STRING)
      MyFree(isv->is_value.sv);
    if (prev)
      prev->is_next = isv->is_next;
    else
      isupport = isv->is_next;
    break;
  }
  touch_isupport();
}

/** Populate #isupport_lines from #isupport. */
static void
build_isupport_lines()
{
  struct ISupport *is;
  struct SLink **plink;
  char buf[BUFSIZE];
  int used, len, usable;

  /* Extra buffer space for :me.name 005 ClientNick <etc> */
  assert(isupport_lines == 0);
  usable = BUFSIZE - 10
      - strlen(cli_name(&me))
      - strlen(get_error_numeric(RPL_ISUPPORT)->format)
      - feature_uint(FEAT_NICKLEN);
  plink = &isupport_lines;
  used = 0;

  /* For each ISUPPORT feature, */
  for (is = isupport; is; ) {
    /* Try to append it to the buffer. */
    switch (is->is_type) {
    case OPT_NONE:
      len = ircd_snprintf(NULL, buf + used, usable - used,
                          " %s", is->is_name);
      break;
    case OPT_INT:
      len = ircd_snprintf(NULL, buf + used, usable - used,
                          " %s=%d", is->is_name, is->is_value.iv);
      break;
    case OPT_STRING:
      len = ircd_snprintf(NULL, buf + used, usable - used,
                          " %s=%s", is->is_name, is->is_value.sv);
      break;
    default:
      assert(0 && "Unhandled ISUPPORT option type");
      len = 0;
      break;
    }

    /* If it fits, move on; else flush buffer and try again. */
    if (len + used < usable) {
      used += len;
      is = is->is_next;
    } else {
      assert(used > 0);
      *plink = make_link();
      DupString((*plink)->value.cp, buf + 1);
      (*plink)->next = 0;
      plink = &(*plink)->next;
      used = 0;
    }
  }

  /* Terminate buffer and flush last bit of it out. */
  buf[used] = '\0';
  *plink = make_link();
  DupString((*plink)->value.cp, buf + 1);
  (*plink)->next = 0;
}

/** Announce fixed-parameter and parameter-free ISUPPORT features
 * provided by ircu's core code.
 */
void init_isupport(void)
{
  add_isupport("WHOX");
  add_isupport("WALLCHOPS");
  add_isupport("WALLVOICES");
  add_isupport("USERIP");
  add_isupport("CPRIVMSG");
  add_isupport("CNOTICE");
  add_isupport_i("MODES", MAXMODEPARAMS);
  add_isupport_i("MAXNICKLEN", NICKLEN);
  add_isupport_i("TOPICLEN", TOPICLEN);
  add_isupport_i("AWAYLEN", AWAYLEN);
  add_isupport_i("KICKLEN", KICKLEN);
  add_isupport_i("MAXCHANNELLEN", CHANNELLEN);
#if defined(DDB) || defined(SERVICES)
  add_isupport_s("PREFIX", "(qov).@+");
  add_isupport_s("STATUSMSG", ".@+");
#else
  add_isupport_s("PREFIX", "(ov)@+");
  add_isupport_s("STATUSMSG", "@+");
#endif
  add_isupport_s("CASEMAPPING", "rfc1459");
#if 0
  sprintf(buf, "CHANMODES=b,k,l,imnpst");
  strcat(buf, "crRMCNu");
  sprintf(buf, "%s CHANTYPES=#&+ MAXBANS=%d", buf, MAXBANS);
  sprintf(buf, "MAXCHANNELS=%d  MAXTARGETS=%d ",
      MAXCHANNELSPERUSER, CHANNELLEN, MAXTARGETS, MAXMODEPARAMS, nicklen);

  sprintf(buf, "SILENCE=%d",
      MAXSILES);
  sprintf(buf,
     " NETWORK=%s",NETWORK_NAME);

  sprintf(buf, "MAP SAFELIST QUITLEN=%d", QUITLEN);
  sprintf(buf, "%s FNC GHOST", buf);
  sprintf(buf, "%s WATCH=%d", buf, MAXWATCH);
#endif
}

/** Send RPL_ISUPPORT lines to \a cptr.
 * @param[in] cptr Client to send ISUPPORT to.
 * @return Zero.
 */
int
send_supported(struct Client *cptr)
{
  struct SLink *line;

  if (isupport && !isupport_lines)
    build_isupport_lines();

  for (line = isupport_lines; line; line = line->next)
    send_reply(cptr, RPL_ISUPPORT, line->value.cp);

  return 0; /* convenience return, if it's ever needed */
}

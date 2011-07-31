/* MusIRCD functions
 *
 * (C) 2004-2007 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: musircd.c 31 2007-07-18 11:39:31Z Hal9000 $
 *
 * last tested with MusIRCd-2.0.2-beta1
 *
 */

#include "denora.h"
#include "musircd.h"

IRCDVar myIrcd[] = {
    {"MusIRCD 2.0x",            /* ircd name                 */
     "+oS",                     /* StatServ mode             */
     IRCD_ENABLE,               /* Vhost                     */
     IRCD_DISABLE,              /* Supports SGlines          */
     IRCD_DISABLE,              /* sgline sql table          */
     IRCD_ENABLE,               /* Supports SQlines          */
     IRCD_ENABLE,               /* sqline sql table          */
     IRCD_DISABLE,              /* Supports SZlines          */
     IRCD_ENABLE,               /* Has exceptions +e         */
     IRCD_DISABLE,              /* vidents                   */
     IRCD_DISABLE,              /* NICKIP                    */
     IRCD_DISABLE,              /* VHOST ON NICK             */
     IRCD_DISABLE,              /* +f                        */
     IRCD_DISABLE,              /* +j                        */
     IRCD_DISABLE,              /* +L                        */
     IRCD_DISABLE,              /* +f Mode                   */
     IRCD_DISABLE,              /* +j                        */
     IRCD_DISABLE,              /* +L Mode                   */
     NULL,                      /* CAPAB Chan Modes          */
     IRCD_DISABLE,              /* We support TOKENS         */
     IRCD_ENABLE,               /* TOKENS are CASE Sensitive */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64    */
     IRCD_ENABLE,               /* +I support                */
     IRCD_DISABLE,              /* SJOIN ban char            */
     IRCD_DISABLE,              /* SJOIN except char         */
     IRCD_DISABLE,              /* SJOIN invite char         */
     IRCD_DISABLE,              /* umode for vhost           */
     IRCD_DISABLE,              /* owner                     */
     IRCD_ENABLE,               /* protect                   */
     IRCD_ENABLE,               /* halfop                    */
     NULL,                      /* User modes                */
     NULL,                      /* Channel modes             */
     IRCD_DISABLE,              /* flood                     */
     IRCD_DISABLE,              /* flood other               */
     IRCD_DISABLE,              /* vhost                     */
     IRCD_DISABLE,              /* vhost other               */
     IRCD_DISABLE,              /* channek linking           */
     IRCD_DISABLE,              /* p10                       */
     IRCD_DISABLE,              /* TS6                       */
     IRCD_ENABLE,               /* numeric ie.. 350 etc      */
     IRCD_DISABLE,              /* channel mode gagged       */
     IRCD_DISABLE,              /* spamfilter                */
     'b',                       /* ban char                  */
     'e',                       /* except char               */
     'I',                       /* invite char               */
     IRCD_DISABLE,              /* zip                       */
     IRCD_DISABLE,              /* ssl                       */
     IRCD_ENABLE,               /* uline                     */
     NULL,                      /* nickchar                  */
     IRCD_DISABLE,              /* svid                      */
     IRCD_DISABLE,              /* hidden oper               */
     IRCD_ENABLE,               /* extra warning             */
     IRCD_ENABLE                /* Report sync state         */
     }
    ,
};

IRCDCAPAB myIrcdcap[] = {
    {
     0,                         /* NOQUIT       */
     0,                         /* TSMODE       */
     0,                         /* UNCONNECT    */
     0,                         /* NICKIP       */
     0,                         /* SJOIN        */
     0,                         /* ZIP          */
     0,                         /* BURST        */
     0,                         /* TS5          */
     0,                         /* TS3          */
     0,                         /* DKEY         */
     0,                         /* PT4          */
     0,                         /* SCS          */
     CAPAB_QS,                  /* QS           */
     0,                         /* UID          */
     0,                         /* KNOCK        */
     0,                         /* CLIENT       */
     0,                         /* IPV6         */
     0,                         /* SSJ5         */
     0,                         /* SN2          */
     0,                         /* TOKEN        */
     0,                         /* VHOST        */
     0,                         /* SSJ3         */
     0,                         /* NICK2        */
     0,                         /* UMODE2       */
     0,                         /* VL           */
     0,                         /* TLKEXT       */
     0,                         /* DODKEY       */
     0,                         /* DOZIP        */
     0, 0, 0}
};

/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_D, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_I, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_b, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_c, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_d, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_f, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_k, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_l, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_n, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_s, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_u, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_y, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);
    CreateChanBanMode(CMODE_I, add_invite, del_invite);

    /* Channel Modes */
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_k, set_key, get_key);
    CreateChanMode(CMODE_l, set_limit, get_limit);
    CreateChanMode(CMODE_m, NULL, NULL);
    CreateChanMode(CMODE_n, NULL, NULL);
    CreateChanMode(CMODE_p, NULL, NULL);
    CreateChanMode(CMODE_r, NULL, NULL);
    CreateChanMode(CMODE_s, NULL, NULL);
    CreateChanMode(CMODE_t, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);

    ModuleUpdateSQLChanMode();

}

void musircd_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void musircd_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void musircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $$%s :%s", dest, msg);
}

void musircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $$%s :%s", dest, msg);
}

void musircd_cmd_global(char *source, char *buf)
{
    send_cmd(source, "SMO 8388609 2 :%s", buf);
}

int denora_event_sjoin(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_sjoin(source, ac, av);
    return MOD_CONT;
}

/*
  1 : const char *source
  2 : char *nick
  3 : char *username
  4 : char *host,
  5 : char *server
  6 : char *realname
  7 : time_t ts
  8 : uint32 svid
  9 : uint32 ip
 10 : char *vhost 
 11 : char *uid
 12 : int hopcount
 13 : char *modes

[Jul 06 22:41:58.066397 2005] ac[0] = Trystan
[Jul 06 22:41:58.066441 2005] ac[1] = 1
[Jul 06 22:41:58.066485 2005] ac[2] = 1120703355
[Jul 06 22:41:58.066529 2005] ac[3] = +i
[Jul 06 22:41:58.066573 2005] ac[4] = tslee
[Jul 06 22:41:58.066631 2005] ac[5] = c-24-2-101-227.hsd1.ut.comcast.net
[Jul 06 22:41:58.066677 2005] ac[6] = musircd.nomadirc.net
[Jul 06 22:41:58.066721 2005] ac[7] = Dreams are answers to questions not yet asked

*/
int denora_event_nick(char *source, int ac, char **av)
{
    User *user;
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 2) {
        ipchar = host_resolve(av[5]);
        user = do_nick(source, av[0], av[4], av[5], av[6], av[7],
                       strtoul(av[2], NULL, 10), 0, ipchar, NULL, NULL,
                       strtoul(av[1], NULL, 10), av[3], NULL);
        free(ipchar);
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
                NULL);
    }
    return MOD_CONT;
}

int denora_event_topic(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    do_topic(ac, av);
    return MOD_CONT;
}

int denora_event_tburst(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 5)
        return MOD_CONT;

    av[0] = av[1];
    av[1] = av[3];
    av[3] = av[4];
    do_topic(4, av);
    return MOD_CONT;
}

int denora_event_436(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;

    m_nickcoll(av[0]);
    return MOD_CONT;
}

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
    return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      denora_event_pass); addCoreMessage(IRCD,m);
    m = createMessage("PING",      denora_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   denora_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      denora_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    denora_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     denora_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     denora_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("USER",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     denora_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("AKILL",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GNOTICE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GOPER",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("RAKILL",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB",     denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     denora_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    denora_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("EOB",       denora_event_eob); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",     denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("TBURST",    denora_event_tburst); addCoreMessage(IRCD,m);
    m = createMessage("RESV",      denora_event_sqline); addCoreMessage(IRCD,m);
    m = createMessage("UNRESV",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("GLINE",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNGLINE",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("DLINE",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("UNDLINE",   denora_event_null); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

int denora_event_sqline(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    sql_do_sqline(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_sethost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

void musircd_cmd_join(char *user, char *channel, time_t chantime)
{
    send_cmd(NULL, "SJOIN %ld %s + :%s", (long int) chantime,
             channel, user);
}

void musircd_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
}

void musircd_cmd_motd(char *sender, char *server)
{
    send_cmd(sender, "MOTD %s", server);
}

/*
 * SVINFO
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
void musircd_cmd_svinfo(void)
{
    send_cmd(NULL, "SVINFO 5 3 0 :%ld", (long int) time(NULL));
}

/* CAPAB */
/*
  QS     - Can handle quit storm removal
  EOB    - Can do EOB message
  HUB    - This server is a HUB 
  UID    - Can do UIDs
  PARA	 - supports invite broadcasting for +p
*/
void musircd_cmd_capab(void)
{
    send_cmd(NULL, "CAPAB :QS");
}

void musircd_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

/* PASS */
void musircd_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

/* SERVER name hop descript */
void musircd_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d 0 :%s", servname, hop, descript);
}

void musircd_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    musircd_cmd_pass(RemotePassword);
    musircd_cmd_capab();
    musircd_cmd_server(ServerName, 1, ServerDesc);
    musircd_cmd_svinfo();
}

void musircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                          char *modes)
{
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
             (long int) time(NULL), modes, user, host, ServerName, real);
}

void musircd_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    musircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_away(source, (ac ? av[0] : NULL));
    return MOD_CONT;
}

int denora_event_kill(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2)
        return MOD_CONT;

    m_kill(source, av[0], av[1]);
    return MOD_CONT;
}

int denora_event_kick(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 3)
        return MOD_CONT;
    do_kick(source, ac, av);
    return MOD_CONT;
}

int denora_event_eob(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac) {
        update_sync_state(source, SYNC_COMPLETE);
    }
    return MOD_CONT;
}

void musircd_cmd_eob(void)
{
    send_cmd(ServerName, "EOB");
}

void musircd_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

int denora_event_join(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
        return MOD_CONT;
    do_join(source, ac, av);
    return MOD_CONT;
}

int denora_event_motd(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_motd(source);
    return MOD_CONT;
}

int denora_event_privmsg(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }
    m_privmsg(source, av[0], av[1]);
    return MOD_CONT;
}

int denora_event_part(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    do_part(source, ac, av);
    return MOD_CONT;
}

int denora_event_whois(char *source, int ac, char **av)
{

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (source && ac >= 1) {
        m_whois(source, av[0]);
    }
    return MOD_CONT;
}

/* EVENT: SERVER */
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        do_server(source, av[0], av[1], av[2], NULL);
    } else {
        do_server(source, av[0], av[1], av[2], NULL);
    }
    return MOD_CONT;
}

int denora_event_squit(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2)
        return MOD_CONT;
    do_squit(av[0]);
    return MOD_CONT;
}

int denora_event_quit(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
        return MOD_CONT;
    do_quit(source, ac, av);
    return MOD_CONT;
}

void musircd_cmd_mode(char *source, char *dest, char *buf)
{
    send_cmd(source, "MODE %s %s", dest, buf);
}

void musircd_cmd_nick(char *nick, char *name, const char *mode)
{
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s %s :%s", nick,
             (long int) time(NULL), mode, ServiceUser, ServiceHost,
             ServerName, name);
}

/* QUIT */
void musircd_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

int denora_event_pass(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    return MOD_CONT;
}

/* PONG */
void musircd_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/* SQUIT */
void musircd_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 2) {
        return MOD_CONT;
    }

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
    return MOD_CONT;
}

/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    capab_parse(ac, av);
    return MOD_CONT;
}

/*
 * SVINFO
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
int denora_event_svinfo(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

int denora_event_error(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac >= 1) {
        alog(LOG_ERROR, "ERROR: %s", av[0]);
    }
    return MOD_CONT;
}

void musircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

int denora_event_notice(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        return MOD_CONT;
    }
    m_notice(source, av[0], av[1]);
    return MOD_CONT;
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_nick(musircd_cmd_nick);
    pmodule_cmd_mode(musircd_cmd_mode);
    pmodule_cmd_bot_nick(musircd_cmd_bot_nick);
    pmodule_cmd_notice(musircd_cmd_notice);
    pmodule_cmd_privmsg(musircd_cmd_privmsg);
    pmodule_cmd_serv_notice(musircd_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(musircd_cmd_serv_privmsg);
    pmodule_cmd_quit(musircd_cmd_quit);
    pmodule_cmd_pong(musircd_cmd_pong);
    pmodule_cmd_join(musircd_cmd_join);
    pmodule_cmd_part(musircd_cmd_part);
    pmodule_cmd_global(musircd_cmd_global);
    pmodule_cmd_squit(musircd_cmd_squit);
    pmodule_cmd_connect(musircd_cmd_connect);
    pmodule_cmd_eob(musircd_cmd_eob);
    pmodule_cmd_ctcp(musircd_cmd_ctcp);
    pmodule_cmd_version(musircd_cmd_version);
    pmodule_cmd_stats(musircd_cmd_stats);
    pmodule_cmd_motd(musircd_cmd_motd);
    pmodule_cmd_ping(musircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion("$Id: musircd.c 31 2007-07-18 11:39:31Z Hal9000 $");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("MusIRCD 2.0x");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_MUSIRCD);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}

/* Sorcery IRCD functions
 *
 * (C) 2004-2007 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 *
 */

#include "denora.h"
#include "sorcery.h"

IRCDVar myIrcd[] = {
    {"Sorcery IRCD*",           /* ircd name                  */
     "+oiS",                    /* StatServ mode              */
     IRCD_ENABLE,               /* Vhost                      */
     IRCD_DISABLE,              /* Supports SGlines           */
     IRCD_DISABLE,              /* sgline sql table           */
     IRCD_ENABLE,               /* Supports SQlines           */
     IRCD_ENABLE,               /* sqline sql table           */
     IRCD_DISABLE,              /* Supports SZlines           */
     IRCD_DISABLE,              /* Has exceptions +e          */
     IRCD_ENABLE,               /* vidents                    */
     IRCD_DISABLE,              /* NICKIP                     */
     IRCD_ENABLE,               /* VHOST ON NICK              */
     IRCD_DISABLE,              /* +f                         */
     IRCD_DISABLE,              /* +j                         */
     IRCD_DISABLE,              /* +L                         */
     IRCD_DISABLE,              /* +f Mode                    */
     IRCD_DISABLE,              /* +j Mode                    */
     IRCD_DISABLE,              /* +L Mode                    */
     NULL,                      /* CAPAB Chan Modes           */
     IRCD_ENABLE,               /* We support Unreal TOKENS   */
     IRCD_DISABLE,              /* TOKENS are CASE Sensitive  */
     IRCD_DISABLE,              /* TIME STAMPS are BASE64     */
     IRCD_DISABLE,              /* +I support                 */
     IRCD_DISABLE,              /* SJOIN ban char             */
     IRCD_DISABLE,              /* SJOIN except char          */
     IRCD_DISABLE,              /* SJOIN invite char          */
     UMODE_x,                   /* umode for vhost            */
     IRCD_DISABLE,              /* owner                      */
     IRCD_DISABLE,              /* protect                    */
     IRCD_DISABLE,              /* halfop                     */
     NULL,                      /* user modes                 */
     NULL,                      /* channel modes              */
     'f',                       /* flood                      */
     IRCD_DISABLE,              /* flood other                */
     'x',                       /* vhost                      */
     IRCD_DISABLE,              /* vhost other                */
     IRCD_DISABLE,              /* channek linking            */
     IRCD_DISABLE,              /* p10                        */
     IRCD_DISABLE,              /* ts6                        */
     IRCD_ENABLE,               /* numeric ie.. 350 etc       */
     IRCD_DISABLE,              /* channel mode gagged        */
     IRCD_DISABLE,              /* spamfilter                 */
     'b',                       /* ban char                   */
     IRCD_DISABLE,              /* except char                */
     IRCD_DISABLE,              /* invite char                */
     IRCD_DISABLE,              /* zip                        */
     IRCD_DISABLE,              /* ssl                        */
     IRCD_ENABLE,               /* uline                      */
     NULL,                      /* nickchar                   */
     IRCD_ENABLE,               /* svid                       */
     IRCD_DISABLE,              /* hidden oper                */
     IRCD_ENABLE,               /* extra warning             */
     IRCD_DISABLE               /* Report sync state         */
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
     0,                         /* QS           */
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
     0,                         /* CHANMODE             */
     0, 0}
};

/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_A, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_P, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_a, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_m, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_p, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);

    /* Channel Modes */
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_k, set_key, get_key);
    CreateChanMode(CMODE_l, set_limit, get_limit);
    CreateChanMode(CMODE_m, NULL, NULL);
    CreateChanMode(CMODE_n, NULL, NULL);
    CreateChanMode(CMODE_p, NULL, NULL);
    CreateChanMode(CMODE_r, NULL, NULL);
    CreateChanMode(CMODE_s, NULL, NULL);
    CreateChanMode(CMODE_t, NULL, NULL);
    CreateChanMode(CMODE_u, NULL, NULL);
    CreateChanMode(CMODE_z, NULL, NULL);

    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);

    ModuleUpdateSQLChanMode();

}

int denora_event_pong(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    server_store_pong(source, time(NULL));
    return MOD_CONT;
}

int denora_event_setname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(source, av[0]);
    return MOD_CONT;
}

int denora_event_chgname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(av[0], av[1]);
    return MOD_CONT;
}

void sorcery_cmd_stats(char *sender, const char *letter, char *server)
{
    send_cmd(sender, "STATS %s %s", letter, server);
}

int denora_event_setident(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_username(source, av[0]);
    return MOD_CONT;
}

int denora_event_chgident(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_username(av[0], av[1]);
    return MOD_CONT;
}

int denora_event_sethost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(source, av[0]);
    return MOD_CONT;
}

int denora_event_nick(char *source, int ac, char **av)
{
    char *ipchar = NULL;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2) {
        ipchar = host_resolve(av[4]);
        if (ac == 7) {
            do_nick(source, av[0], av[3], av[4], av[5], av[6],
                    strtoul(av[2], NULL, 10), 0, ipchar, NULL, NULL,
                    strtoul(av[1], NULL, 10), NULL, NULL);
        } else {
            do_nick(source, av[0], av[3], av[4], av[5], av[7],
                    strtoul(av[2], NULL, 10), strtoul(av[6], NULL, 0),
                    ipchar, NULL, NULL, strtoul(av[1], NULL, 10), NULL,
                    NULL);
        }
        free(ipchar);
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
                NULL);
    }
    return MOD_CONT;
}

int denora_event_chghost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(av[0], av[1]);
    return MOD_CONT;
}

void sorcery_cmd_version(char *server)
{
    send_cmd(s_StatServ, "VERSION %s", server);
}

void sorcery_cmd_motd(char *sender, char *server)
{
    send_cmd(sender, "MOTD %s", server);
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

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("401",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("402",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      denora_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      denora_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      denora_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      denora_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    denora_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      denora_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      denora_event_null); addCoreMessage(IRCD,m);
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
    m = createMessage("SQLINE",    denora_event_sqline); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("PROTOCTL",  denora_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("CHGHOST",   denora_event_chghost); addCoreMessage(IRCD,m);
    m = createMessage("CHGIDENT",  denora_event_chgident); addCoreMessage(IRCD,m);
    m = createMessage("NETINFO",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SNETINFO",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SETHOST",   denora_event_sethost); addCoreMessage(IRCD,m);
    m = createMessage("SETIDENT",  denora_event_setident); addCoreMessage(IRCD,m);
    m = createMessage("SETNAME",   denora_event_setname); addCoreMessage(IRCD,m);
    m = createMessage("VCTRL",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",      denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",      denora_event_error); addCoreMessage(IRCD,m);
    m = createMessage("PONG",       denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("NETG",       denora_event_null); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */

/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    capab_parse(ac, av);
    return MOD_CONT;
}

void sorcery_cmd_join(char *user, char *channel, time_t chantime)
{
    alog(LOG_PROTOCOL, "User %s joins %s at %ld", user, channel,
         (long int) chantime);
    send_cmd(user, "JOIN %s", channel);
}

void sorcery_cmd_capab(void)
{
}

/* PASS */
void sorcery_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS :%s", pass);
}

/* SERVER name hop descript */
void sorcery_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/* PONG */
void sorcery_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

void sorcery_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    sorcery_cmd_capab();
    sorcery_cmd_pass(RemotePassword);
    sorcery_cmd_server(ServerName, 1, ServerDesc);
}

/* PART */
void sorcery_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

/* GLOBOPS */
void sorcery_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

/* SQUIT */
void sorcery_cmd_squit(char *servname, char *message)
{
    send_cmd(NULL, "SQUIT %s :%s", servname, message);
}

/* Events */

int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1) {
        return MOD_CONT;
    }
    sorcery_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
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

int denora_event_topic(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 4)
        return MOD_CONT;
    do_topic(ac, av);
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
    if (ac != 1)
        return MOD_CONT;
    do_quit(source, ac, av);
    return MOD_CONT;
}

int denora_event_mode(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 2)
        return MOD_CONT;

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
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

/* EVENT: SERVER */
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[1], "1")) {
        denora->uplink = sstrdup(av[0]);
        do_server(source, av[0], av[1], av[2], NULL);
        return MOD_CONT;
    }
    do_server(source, av[0], av[1], av[2], NULL);
    return MOD_CONT;
}

int denora_event_privmsg(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 2)
        return MOD_CONT;
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

void sorcery_cmd_notice(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void sorcery_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void sorcery_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void sorcery_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

void sorcery_cmd_nick(char *nick, char *name, const char *mode)
{
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s 0 :%s", nick,
             (long int) time(NULL), ServiceUser, ServiceHost, ServerName,
             name);
    denora_cmd_mode(nick, nick, "%s", mode);
}

/* QUIT */
void sorcery_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

int denora_event_sqline(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    sql_do_sqline(av[0], av[1]);
    return MOD_CONT;
}

void sorcery_cmd_mode(char *source, char *dest, char *buf)
{
    send_cmd(source, "MODE %s %s", dest, buf);
}

void sorcery_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                          char *modes)
{
    send_cmd(NULL, "NICK %s 1 %ld %s %s %s 0 :%s", nick,
             (long int) time(NULL), user, host, ServerName, real);
    denora_cmd_mode(nick, nick, "%s", modes);
}

void sorcery_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

int denora_event_error(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (av[0]) {
        alog(LOG_ERROR, "ERROR: %s", av[0]);
    }
    return MOD_CONT;
}

void sorcery_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void sorcery_cmd_eob(void)
{
}

void moduleAddIRCDCmds(void)
{
    pmodule_cmd_eob(sorcery_cmd_eob);
    pmodule_cmd_nick(sorcery_cmd_nick);
    pmodule_cmd_mode(sorcery_cmd_mode);
    pmodule_cmd_bot_nick(sorcery_cmd_bot_nick);
    pmodule_cmd_notice(sorcery_cmd_notice);
    pmodule_cmd_privmsg(sorcery_cmd_privmsg);
    pmodule_cmd_serv_notice(sorcery_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(sorcery_cmd_serv_privmsg);
    pmodule_cmd_quit(sorcery_cmd_quit);
    pmodule_cmd_pong(sorcery_cmd_pong);
    pmodule_cmd_join(sorcery_cmd_join);
    pmodule_cmd_part(sorcery_cmd_part);
    pmodule_cmd_global(sorcery_cmd_global);
    pmodule_cmd_squit(sorcery_cmd_squit);
    pmodule_cmd_connect(sorcery_cmd_connect);
    pmodule_cmd_ctcp(sorcery_cmd_ctcp);
    pmodule_cmd_version(sorcery_cmd_version);
    pmodule_cmd_stats(sorcery_cmd_stats);
    pmodule_cmd_motd(sorcery_cmd_motd);
    pmodule_cmd_ping(sorcery_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion("$Id: sorcery.c 31 2007-07-18 11:39:31Z Hal9000 $");
    moduleSetType(PROTOCOL);
    pmodule_ircd_version("Sorcery IRCD");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);
    pmodule_irc_var(IRC_SORCERY);
    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();
    return MOD_CONT;
}

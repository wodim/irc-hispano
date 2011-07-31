
/* InspIRCd 1.0 Beta 6 functions
 *
 * (C) 2005-2006 Craig Edwards <brain@inspircd.org>
 * (C) 2004-2007 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: inspircd.c 31 2007-07-18 11:39:31Z Hal9000 $
 *
 * Following modules loaded during testing
 * <module name="m_chanprotect.so">
 * <module name="m_globops.so">
 * <module name="m_noinvite.so">
 * <module name="m_services.so">
 * <module name="m_chghost.so">
 * <module name="m_nokicks.so">
 * <module name="m_sethost.so">
 * <module name="m_cloaking.so">
 * <module name="m_nonicks.so">
 * <module name="m_sajoin.so">
 * <module name="m_setidle.so">
 * <module name="m_blockcolor.so">
 * <module name="m_nonotice.so">
 * <module name="m_samode.so">
 * <module name="m_park.so">
 * <module name="m_setname.so">
 * <module name="m_botmode.so">
 * <module name="m_knock.so">
 * <module name="m_operchans.so">
 * <module name="m_showwhois.so">
 * <module name="m_stripcolor.so">
 * <module name="m_censor.so">
 * <module name="m_redirect.so">
 * <module name="m_chanfilter.so">
 * <module name="m_noctcp.so">
 *
 */

/*************************************************************************/

/* ***** WARNING ******
 *
 * While InspIRCd beta 6 is relatively stable, it is very picky
 * about the sources commands come from. If i've made commands
 * come from nicks or servers here, where it looks odd to you,
 * this is NORMAL and the way it should be done.
 *
 * Also, some commands arent implemented yet (like remote MOTD
 * and some work differently (VERSION) but its now a proper
 * protocol that links to a proper network (none of this mesh bs).
 */

#include "denora.h"
#include "inspircd.h"

IRCDVar myIrcd[] = {
    {"InspIRCd 1.0 Beta 6",     /* ircd name                    */
     "+io",                     /* StatServ mode                */
     IRCD_ENABLE,               /* Vhost                        */
     IRCD_ENABLE,               /* Supports SGlines             */
     IRCD_DISABLE,              /* sgline sql table             */
     IRCD_ENABLE,               /* Supports SQlines             */
     IRCD_DISABLE,              /* sqline sql table             */
     IRCD_ENABLE,               /* Supports SZlines             */
     IRCD_ENABLE,               /* Has exceptions +e            */
     IRCD_ENABLE,               /* vidents                      */
     IRCD_ENABLE,               /* NICKIP                       */
     IRCD_ENABLE,               /* VHOST ON NICK                */
     IRCD_ENABLE,               /* +f                           */
     IRCD_DISABLE,              /* +j                           */
     IRCD_ENABLE,               /* +L                           */
     IRCD_DISABLE,              /* +f Mode                      */
     IRCD_DISABLE,              /* +j Mode                      */
     CMODE_L,                   /* +L Mode                      */
     NULL,                      /* CAPAB Chan Modes             */
     IRCD_ENABLE,               /* We support Unreal TOKENS     */
     IRCD_DISABLE,              /* TOKENS are CASE Sensitive    */
     IRCD_ENABLE,               /* TIME STAMPS are BASE64       */
     IRCD_DISABLE,              /* +I support                   */
     '&',                       /* SJOIN ban char               */
     '\"',                      /* SJOIN except char            */
     IRCD_DISABLE,              /* SJOIN invite char            */
     UMODE_x,                   /* umode for vhost              */
     IRCD_ENABLE,               /* channel owner                */
     IRCD_ENABLE,               /* channel mode protect         */
     IRCD_ENABLE,               /* channel mode halfop          */
     NULL,
     NULL,
     'f',                       /* flood                        */
     IRCD_DISABLE,              /* flood other                  */
     'x',                       /* vhost                        */
     't',                       /* vhost other                  */
     'L',                       /* channek linking              */
     IRCD_DISABLE,              /* p10 protocol                 */
     IRCD_DISABLE,              /* ts6 protocol                 */
     IRCD_ENABLE,               /* numeric ie.. 350 etc         */
     IRCD_DISABLE,              /* channel mode gagged          */
     IRCD_DISABLE,              /* spamfilter                   */
     'b',                       /* ban char                     */
     'e',                       /* except char                  */
     IRCD_DISABLE,              /* invite char                  */
     IRCD_DISABLE,              /* zip                          */
     IRCD_DISABLE,              /* ssl                          */
     IRCD_ENABLE,               /* uline                        */
     NULL,                      /* nickchar                     */
     IRCD_ENABLE,               /* svid                         */
     IRCD_DISABLE,              /* hidden oper                  */
     IRCD_ENABLE,               /* extra warning                */
     IRCD_ENABLE                /* Report sync state            */
     }
    ,
};


IRCDCAPAB myIrcdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
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
     CAPAB_SSJ3,                /* SSJ3         */
     CAPAB_NICK2,               /* NICK2        */
     CAPAB_UMODE2,              /* UMODE2       */
     CAPAB_VL,                  /* VL           */
     CAPAB_TLKEXT,              /* TLKEXT       */
     0,                         /* DODKEY       */
     0,                         /* DOZIP        */
     0,
     0,
     0,
     }
};


/*************************************************************************/

void IRCDModeInit(void)
{
    ModuleSetUserMode(UMODE_B, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_G, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_R, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_S, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_W, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_g, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_h, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_i, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_o, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_r, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_w, IRCD_ENABLE);
    ModuleSetUserMode(UMODE_x, IRCD_ENABLE);
    ModuleUpdateSQLUserMode();
    CreateChanBanMode(CMODE_b, add_ban, del_ban);
    CreateChanBanMode(CMODE_e, add_exception, del_exception);


    /* Channel Modes */
    CreateChanMode(CMODE_C, NULL, NULL);
    CreateChanMode(CMODE_G, NULL, NULL);
    CreateChanMode(CMODE_J, NULL, NULL);
    CreateChanMode(CMODE_K, NULL, NULL);
    CreateChanMode(CMODE_L, set_redirect, get_redirect);
    CreateChanMode(CMODE_M, NULL, NULL);
    CreateChanMode(CMODE_N, NULL, NULL);
    CreateChanMode(CMODE_O, NULL, NULL);
    CreateChanMode(CMODE_P, NULL, NULL);
    CreateChanMode(CMODE_Q, NULL, NULL);
    CreateChanMode(CMODE_R, NULL, NULL);
    CreateChanMode(CMODE_S, NULL, NULL);
    CreateChanMode(CMODE_T, NULL, NULL);
    CreateChanMode(CMODE_V, NULL, NULL);
    CreateChanMode(CMODE_c, NULL, NULL);
    CreateChanMode(CMODE_g, NULL, NULL);
    CreateChanMode(CMODE_i, NULL, NULL);
    CreateChanMode(CMODE_j, set_flood_alt, get_flood_alt);
    CreateChanMode(CMODE_k, set_key, get_key);
    CreateChanMode(CMODE_l, set_limit, get_limit);
    CreateChanMode(CMODE_m, NULL, NULL);
    CreateChanMode(CMODE_n, NULL, NULL);
    CreateChanMode(CMODE_p, NULL, NULL);
    CreateChanMode(CMODE_r, NULL, NULL);
    CreateChanMode(CMODE_s, NULL, NULL);
    CreateChanMode(CMODE_t, NULL, NULL);
    CreateChanMode(CMODE_z, NULL, NULL);

    ModuleSetChanUMode('%', 'h', STATUS_HALFOP);
    ModuleSetChanUMode('+', 'v', STATUS_VOICE);
    ModuleSetChanUMode('@', 'o', STATUS_OP);
    ModuleSetChanUMode('~', 'a', STATUS_PROTECTED);
    ModuleSetChanUMode('*', 'q', STATUS_OWNER);

    ModuleUpdateSQLChanMode();

}


/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("436",       denora_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      denora_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      denora_event_join); addCoreMessage(IRCD,m);
    m = createMessage("FJOIN",     denora_event_fjoin); addCoreMessage(IRCD,m);
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
    m = createMessage("IDLE",      denora_event_idle); addCoreMessage(IRCD,m);

    m = createMessage("FHOST",     denora_event_fhost); addCoreMessage(IRCD,m);
    m = createMessage("FNAME",     denora_event_fname); addCoreMessage(IRCD,m);
    m = createMessage("PONG",      denora_event_pong); addCoreMessage(IRCD,m);
    m = createMessage("METADATA",  denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("FMODE",     denora_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("FTOPIC",    denora_event_ftopic); addCoreMessage(IRCD,m);
    m = createMessage("VERSION",   denora_event_version); addCoreMessage(IRCD,m);
    m = createMessage("OPERTYPE",  denora_event_opertype); addCoreMessage(IRCD,m);
    m = createMessage("ADDLINE",   denora_event_addline); addCoreMessage(IRCD,m);
    m = createMessage("GLINE",     denora_event_gline); addCoreMessage(IRCD,m);
    m = createMessage("ZLINE",     denora_event_zline); addCoreMessage(IRCD,m);
    m = createMessage("QLINE",     denora_event_qline); addCoreMessage(IRCD,m);
    m = createMessage("KLINE",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ELINE",     denora_event_eline); addCoreMessage(IRCD,m);
    m = createMessage("BURST",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("ENDBURST",  denora_event_eob); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB",     denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("AES",       denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   denora_event_svsnick); addCoreMessage(IRCD,m);
    m = createMessage("SANICK",    denora_event_sanick); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   denora_event_svsmode); addCoreMessage(IRCD,m);
    m = createMessage("SAMODE",    denora_event_svsmode); addCoreMessage(IRCD,m);
    m = createMessage("SVSJOIN",   denora_event_svsjoin); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SAJOIN",    denora_event_null); addCoreMessage(IRCD,m);
    m = createMessage("SAPART",    denora_event_null); addCoreMessage(IRCD,m);

}

/* *INDENT-ON* */

int denora_event_eob(char *source, int ac, char **av)
{
    update_sync_state(denora->uplink, SYNC_COMPLETE);
    return MOD_CONT;
}


int denora_event_svsnick(char *source, int ac, char **av)
{
    do_nick(av[0], av[1], NULL, NULL, NULL,
            NULL, strtoul(av[2], NULL, 10), 0, NULL, NULL, NULL, 0, NULL,
            NULL);
    return MOD_CONT;
}

int denora_event_sanick(char *source, int ac, char **av)
{
    do_nick(av[0], av[1], NULL, NULL, NULL,
            NULL, (int) time(NULL), 0, NULL, NULL, NULL, 0, NULL, NULL);
    return MOD_CONT;
}

int denora_event_svsmode(char *source, int ac, char **av)
{
    denora_event_mode(av[0], 2, av);
    return MOD_CONT;
}

/*
[Dec 30 16:03:09.300934 2005] Source inspircd.nomadirc.net
[Dec 30 16:03:09.300990 2005] av[0] = Q
[Dec 30 16:03:09.301046 2005] av[1] = ChanServ
[Dec 30 16:03:09.301102 2005] av[2] = <Config>
[Dec 30 16:03:09.301158 2005] av[3] = 1135968312
[Dec 30 16:03:09.301214 2005] av[4] = 0
[Dec 30 16:03:09.301270 2005] av[5] = Reserved For Services

*/
int denora_event_addline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur;
    char buf[BUFSIZE];
    checkdur = atoi(av[4]);

    if (checkdur != 0) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) checkdur + time(NULL));
    } else {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) checkdur);
    }

    if (!stricmp(av[0], "G") || !stricmp(av[0], "E")) {
        user = myStrGetToken(av[1], '@', 0);
        host = myStrGetToken(av[1], '@', 1);
        sql_do_server_bans_add(av[0], user, host, av[2], av[3], buf,
                               av[5]);
    } else if (!stricmp(av[0], "Q")) {
        sql_do_server_bans_add(av[0], (char *) "*", av[1], av[2], av[3],
                               buf, av[5]);
    } else {
        sql_do_server_bans_add(av[0], (char *) "", av[1], av[2], av[3],
                               buf, av[5]);
    }
    return MOD_CONT;
}

/* :<source nickname> GLINE <user@host> <duration> :<reason> */

int denora_event_gline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    user = myStrGetToken(av[0], '@', 0);
    host = myStrGetToken(av[0], '@', 1);

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "G", user, host, source, buf, buf2,
                               av[2]);
    } else {
        sql_do_server_bans_remove((char *) "G", user, host);
    }
    return MOD_CONT;
}

int denora_event_eline(char *source, int ac, char **av)
{
    char *user, *host;
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    user = myStrGetToken(av[0], '@', 0);
    host = myStrGetToken(av[0], '@', 1);

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "E", user, host, source, buf, buf2,
                               av[2]);
    } else {
        sql_do_server_bans_remove((char *) "E", user, host);
    }
    return MOD_CONT;
}

int denora_event_zline(char *source, int ac, char **av)
{
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "Z", (char *) "", av[0], source,
                               buf, buf2, av[2]);
    } else {
        sql_do_server_bans_remove((char *) "Z", (char *) "", av[0]);
    }
    return MOD_CONT;
}

int denora_event_qline(char *source, int ac, char **av)
{
    int checkdur;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];

    if (ac >= 3) {
        ircsnprintf(buf, BUFSIZE, "%ld", (long int) time(NULL));
        checkdur = atoi(av[1]);
        if (checkdur != 0) {
            ircsnprintf(buf2, BUFSIZE, "%ld",
                        (long int) checkdur + time(NULL));
        } else {
            ircsnprintf(buf2, BUFSIZE, "%ld", (long int) checkdur);
        }
        sql_do_server_bans_add((char *) "Q", (char *) "*", av[0], source,
                               buf, buf2, av[2]);
    } else {
        sql_do_server_bans_remove((char *) "Q", (char *) "*", av[0]);
    }
    return MOD_CONT;
}

int denora_event_ftopic(char *source, int ac, char **av)
{
    /* :source FTOPIC channel ts setby :topic */
    char *temp;
    if (ac < 4)
        return MOD_CONT;
    temp = av[1];               /* temp now holds ts */
    av[1] = av[2];              /* av[1] now holds set by */
    av[2] = temp;               /* av[2] now holds ts */
    do_topic(ac, av);
    return MOD_CONT;
}

int denora_event_version(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    sql_do_server_version(source, ac, av);
    return MOD_CONT;
}

int denora_event_opertype(char *source, int ac, char **av)
{
    /* opertype is equivalent to mode +o because servers
       dont do this directly */
    char *newav[2];
    newav[0] = source;
    newav[1] = (char *) "+o";
    return denora_event_mode(source, 2, newav);
}

/* Event: PROTOCTL */
int denora_event_capab(char *source, int ac, char **av)
{
    return MOD_CONT;
}

void inspircd_cmd_nick(char *nick, char *name, const char *modes)
{
    /* :test.chatspike.net NICK 1133519355 Brain synapse.brainbox.winbot.co.uk netadmin.chatspike.net ~brain +xwsioS 10.0.0.2 :Craig Edwards */
    send_cmd(ServerName, "NICK %ld %s %s %s %s +%s 0.0.0.0 :%s",
             (long int) time(NULL), nick, ServiceHost, ServiceHost,
             ServiceUser, modes, name);
    send_cmd(ServerName, "OPERTYPE Service");
}

void inspircd_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                           char *modes)
{
    send_cmd(ServerName, "NICK %ld %s %s %s %s +%s 0.0.0.0 :%s",
             (long int) time(NULL), nick, host, host, user, modes, real);
    send_cmd(ServerName, "OPERTYPE Bot");
}

void inspircd_cmd_notice(char *source, char *dest, char *buf)
{
    if (!buf) {
        return;
    }
    send_cmd(source, "NOTICE %s :%s", dest, buf);
}

void inspircd_cmd_stats(char *sender, const char *letter, char *server)
{
}

void inspircd_cmd_privmsg(char *source, char *dest, char *buf)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void inspircd_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void inspircd_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

/* QUIT */
void inspircd_cmd_quit(char *source, char *buf)
{
    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT :Leaving");
    }
}

/* PROTOCTL */
void inspircd_cmd_capab()
{
}

static char currentpass[1024];

/* PASS */
void inspircd_cmd_pass(char *pass)
{
    strncpy(currentpass, pass, 1024);
}

/* SERVER name hop descript */
void inspircd_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(ServerName, "SERVER %s %s %d :%s", servname, currentpass,
             hop + 1, descript);
}

/* PONG */
void inspircd_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/* JOIN */
void inspircd_cmd_join(char *user, char *channel, time_t chantime)
{
    alog(LOG_PROTOCOL, "User %s joins %s at %ld", user, channel,
         (long int) chantime);
    send_cmd(user, "JOIN %s", channel);
}

/* PART */
void inspircd_cmd_part(char *nick, char *chan, char *buf)
{
    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s :Leaving", chan);
    }
}

/* GLOBOPS */
void inspircd_cmd_global(char *source, char *buf)
{
    send_cmd(source, "GLOBOPS :%s", buf);
}

/* SQUIT */
void inspircd_cmd_squit(char *servname, char *message)
{
    send_cmd(ServerName, "SQUIT %s :%s", servname, message);
}

void inspircd_cmd_connect(void)
{
    me_server =
        do_server(NULL, ServerName, (char *) "0", ServerDesc, NULL);

    inspircd_cmd_capab();
    send_cmd(NULL, "SERVER %s %s %d :%s", ServerName, RemotePassword, 0,
             ServerDesc);
    send_cmd(NULL, "BURST");
    send_cmd(ServerName, "VERSION :Denora");
}

/* Events */
int denora_event_ping(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac < 1)
        return MOD_CONT;
    inspircd_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

int denora_event_436(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int denora_event_away(char *source, int ac, char **av)
{
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

/* Normal RFC style topic: :source TOPIC chan :topic */
int denora_event_topic(char *source, int ac, char **av)
{
    char *newav[127];

    if (ac < 2) {
        return MOD_CONT;
    }

    newav[0] = sstrdup(av[0]);
    newav[1] = sstrdup(source);
    newav[2] = itostr(time(NULL));
    newav[3] = sstrdup(av[1]);
    do_topic(4, newav);
    if (newav[0]) {
        free(newav[0]);
    }
    if (newav[1]) {
        free(newav[1]);
    }
    if (newav[3]) {
        free(newav[3]);
    }
    return MOD_CONT;
}

/* :source SQUIT dest :reason */
int denora_event_squit(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
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

/* Both FMODE (which can only come from a server)
 * and MODE (which can only come from a user) are
 * both routed to this handler as denora can handle
 * them both as the same thing.
 */

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

int denora_event_svsjoin(char *source, int ac, char **av)
{
    char *newav[127];

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (ac != 1)
        return MOD_CONT;

    newav[0] = av[1];
    do_join(av[0], 1, newav);
    return MOD_CONT;
}

/* We implement this, but for now, InspIRCd doesnt
 * support remote MOTD.
 */

int denora_event_motd(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    m_motd(source);
    return MOD_CONT;
}

/* FHOST and FNAME are equivalent to SETHOST and SETNAME,
 * CHGHOST and CHGNAME generate FHOST and FNAME commands
 * and are therefore redundant
 */

int denora_event_fname(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_realname(source, av[0]);
    return MOD_CONT;
}

int denora_event_fhost(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    change_user_host(source, av[0]);
    return MOD_CONT;
}


/* FJOIN is like SJOIN, but not quite. It doesnt sync
 * simple-modes, or bans/excepts.
 */
int denora_event_fjoin(char *source, int ac, char **av)
{

    char *newav[127];
    char people[1024];
    int i = 0;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac < 3)
        return MOD_CONT;

    newav[0] = av[1];
    newav[1] = av[0];
    newav[2] = (char *) "+";
    newav[3] = people;

    *people = '\0';

    for (i = 2; i < ac; i++) {
        if (i > 2)
            strncat(people, " ", 1024);
        strncat(people, av[i], 1024);
    }
    do_sjoin(source, 4, newav);

    return MOD_CONT;
}

char *inspircd_nickip(char *host)
{
    struct in_addr addy;
    struct in_addr addr;

    uint32 *ad = (uint32 *) & addy;
    inet_aton(host, &addy);

    addr.s_addr = htonl(*ad);
    return sstrdup(inet_ntoa(addr));
}

int denora_event_nick(char *source, int ac, char **av)
{
    User *user;

    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac != 1) {
        if (ac == 8) {
            user = do_nick("", av[1],   /* nick */
                           av[4],       /* username */
                           av[2],       /* realhost */
                           source,      /* server */
                           av[7],       /* realname */
                           strtoul(av[0], NULL, 10),
                           0, inspircd_nickip(av[6]), av[3], NULL, 1,
                           av[5], NULL);
        }
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                0, 0, NULL, NULL, NULL, 0, NULL, NULL);
    }

    return MOD_CONT;
}


/* EVENT: SERVER */
/*
 av[0] = inspircd.nomadirc.net
 av[1] = linkpass
 av[2] = 0
 av[3] = Waddle World
*/
int denora_event_server(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    if (!stricmp(av[2], "0")) {
        denora->uplink = sstrdup(av[0]);
    }
    do_server(source, av[0], av[2], av[3], NULL);
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

/* In InspIRCd, instead of sending back the entire WHOIS reply
 * as numerics on remote WHOIS, the remote server sends back
 * a reply to the IDLE command, which returns the users signon
 * and idle times, as these are the only different pieces of
 * information given between remote and local whois
 */
int denora_event_idle(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }

    if (ac == 1) {
        send_cmd(av[0], "IDLE %s %ld 0", source, (long int) time(NULL));
    }

    return MOD_CONT;
}

void inspircd_cmd_ping(char *server)
{
    send_cmd(ServerName, "PING %s :%s", ServerName, server);
}

void inspircd_cmd_ctcp(char *source, char *dest, char *buf)
{
    send_cmd(source, "NOTICE %s :\1%s \1", dest, buf);
}

void inspircd_cmd_version(char *server)
{
    /* TODO: InspIRCd sends you all servers version strings as they burst.
     * These can be cached, rather than having to request them.
     */
}

void inspircd_cmd_motd(char *sender, char *server)
{
    /* Remote MOTD not implemented in beta 6 */
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

void inspircd_cmd_mode(char *source, char *dest, char *buf)
{
    if (!stricmp(source, ServerName)) {
        send_cmd(source, "FMODE %s %s", dest, buf);
    } else {
        send_cmd(source, "MODE %s %s", dest, buf);
    }
}

void inspircd_cmd_eob(void)
{
    send_cmd(NULL, "ENDBURST");
}

void moduleAddIRCDCmds()
{
    pmodule_cmd_eob(inspircd_cmd_eob);
    pmodule_cmd_nick(inspircd_cmd_nick);
    pmodule_cmd_mode(inspircd_cmd_mode);
    pmodule_cmd_bot_nick(inspircd_cmd_bot_nick);
    pmodule_cmd_notice(inspircd_cmd_notice);
    pmodule_cmd_privmsg(inspircd_cmd_privmsg);
    pmodule_cmd_serv_notice(inspircd_cmd_serv_notice);
    pmodule_cmd_serv_privmsg(inspircd_cmd_serv_privmsg);
    pmodule_cmd_quit(inspircd_cmd_quit);
    pmodule_cmd_pong(inspircd_cmd_pong);
    pmodule_cmd_join(inspircd_cmd_join);
    pmodule_cmd_part(inspircd_cmd_part);
    pmodule_cmd_global(inspircd_cmd_global);
    pmodule_cmd_squit(inspircd_cmd_squit);
    pmodule_cmd_connect(inspircd_cmd_connect);
    pmodule_cmd_ctcp(inspircd_cmd_ctcp);
    pmodule_cmd_version(inspircd_cmd_version);
    pmodule_cmd_stats(inspircd_cmd_stats);
    pmodule_cmd_motd(inspircd_cmd_motd);
    pmodule_cmd_ping(inspircd_cmd_ping);
}

int DenoraInit(int argc, char **argv)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion("$Id: inspircd.c 31 2007-07-18 11:39:31Z Hal9000 $");
    moduleSetType(PROTOCOL);

    pmodule_ircd_version("InspIRCd 1.0 Beta 6+");
    pmodule_ircd_cap(myIrcdcap);
    pmodule_ircd_var(myIrcd);
    pmodule_ircd_useTSMode(0);
    pmodule_irc_var(IRC_INSPIRCD);
    IRCDModeInit();
    pmodule_oper_umode(UMODE_o);

    moduleAddIRCDCmds();
    moduleAddIRCDMsgs();

    return MOD_CONT;
}

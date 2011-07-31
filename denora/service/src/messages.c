/*
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: messages.c 398 2011-01-31 22:37:12Z hal9000 $
 *
 */

#include "denora.h"

/*************************************************************************/

/**
 * Handle nick collide messages to make sure we are still connected
 *
 * @param user is the nick of the person whom was part of the collide
 *
 * @return always returns MOD_CONT
 */
int m_nickcoll(char *user)
{
    if (nickIsServices(user)) {
        introduce_user(user);
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle AWAY messages
 *
 * @param source is the nick of the person whom is now away
 * @param msg is the text of the away message
 *
 * @return always returns MOD_CONT
 */
int m_away(char *source, char *msg)
{
    User *u;

    if (!source) {
        return MOD_CONT;
    }
    u = user_find(source);
    if (!u) {
        return MOD_CONT;
    }

    SET_SEGV_LOCATION();

    if (msg) {
        stats->away++;
        u->isaway = 1;
        u->awaymsg = sstrdup(msg);
    } else {
        stats->away--;
        u->isaway = 0;
        u->awaymsg = NULL;
    }
    if (denora->do_sql) {
        rdb_query
            (QUERY_LOW,
             "UPDATE %s SET away=\'%s\', awaymsg=\'%s\' WHERE nickid=%d",
             UserTable, (u->isaway ? (char *) "Y" : (char *) "N"),
             rdb_escape(u->awaymsg), db_getnick(u->sqlnick));
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle KILL messages
 *
 * @param source is the server or nick that send the message
 * @param nick is the user name that was killed
 * @param msg is the text of the kill message
 *
 * @return void - no returend value
 */
void m_kill(char *source, char *nick, char *msg)
{
    User *u;
    ServStats *s;
    int id;

    if ((u = user_find(source))) {
        if ((s = findserverstats(u->server->name))) {
            s->ircopskills++;
        }
        send_event(EVENT_GLOBAL_KILL, 3, source, nick, msg);
    } else if ((s = findserverstats(source))) {
        s->serverkills++;
        send_event(EVENT_SERVER_KILL, 3, source, nick, msg);
    }
    do_kill(nick, msg);
    if (denora->do_sql) {
        nick = rdb_escape(nick);
        db_removenick(nick, msg);
        if (UserCacheTime) {
            db_cleanuser();
        }
        free(nick);
    }
    if (s && denora->do_sql) {
        id = db_getserver(s->name);
        if (id) {
            rdb_query
                (QUERY_LOW,
                 "UPDATE %s SET ircopskills=%d, serverkills=%ld WHERE servid=%d",
                 ServerTable, s->ircopskills, s->serverkills, id);
        }
    }
    if (nickIsServices(nick)) {
        introduce_user(nick);
    }
}

/*************************************************************************/

/**
 * Handle TIME messages
 *
 * @param source is the nick of the person whom requested
 * @param ac is the parameter count
 * @param av is the array
 *
 * @return always returns MOD_CONT
 */
int m_time(char *source, int ac, char **av)
{
    time_t t;
    struct tm tm;
    char buf[64];

    USE_VAR(ac);
    USE_VAR(av);

    *buf = '\0';

    if (!source) {
        return MOD_CONT;
    }

    time(&t);
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    tm = *localtime(&t);
#endif
    strftime(buf, 63, "%a %b %d %H:%M:%S %Y %Z", &tm);
    denora_cmd_391(source, buf);

    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle MOTD messages
 *
 * @param source is the nick of the person whom requested
 *
 * @return always returns MOD_CONT
 */
int m_motd(char *source)
{
    FILE *f;
    char buf[BUFSIZE];
    *buf = '\0';

    SET_SEGV_LOCATION();

    if (!source) {
        return MOD_CONT;
    }

    denora_cmd_375(source);
    if (!MOTDFilename) {
        denora_cmd_422(source);
    } else {
        if ((f = FileOpen(MOTDFilename, FILE_READ)) != NULL) {
            SET_SEGV_LOCATION();
            while (fgets(buf, BUFSIZE - 1, f)) {
                buf[strlen(buf) - 1] = 0;
                denora_cmd_372(source, buf);
            }
            fclose(f);
        } else {
            denora_cmd_422(source);
        }
    }
    SET_SEGV_LOCATION();
    denora_cmd_376(source);
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle NOTICE commands
 *
 * @param source is the nick of the person whom sent the notice
 * @param receiver is the nick whom it was sent to
 * @param msg is the message that was sent
 *
 * @return always returns MOD_CONT
 */
int m_notice(char *source, char *receiver, char *msg)
{
    char *temp = NULL;
    char *version;
    char *clean;

    if (BadPtr(source) || BadPtr(receiver) || BadPtr(msg)) {
        return MOD_CONT;
    }

    if (!stricmp(receiver, s_StatServ)
        || (s_StatServ_alias && !stricmp(receiver, s_StatServ_alias))) {
        clean = normalizeBuffer(msg);
        doCleanBuffer((char *) clean);
        temp = myStrGetToken(clean, ' ', 0);
        if (!temp) {
            free(clean);
            return MOD_CONT;
        }
        if (!stricmp(temp, "VERSION")) {
            version = myStrGetTokenRemainder(clean, ' ', 1);
            handle_ctcp_version(source, version);
            free(version);
        }
        free(clean);
        free(temp);
    }

    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle PRIVMSG commands
 *
 * @param source is the nick of the person whom sent the privmsg
 * @param receiver is the nick whom it was sent to
 * @param msg is the message that was sent
 *
 * @return always returns MOD_CONT
 */
int m_privmsg(char *source, char *receiver, char *msg)
{
    char *s;
    User *u;
    Server *srv;
    PrivMsg *p;

    if (BadPtr(source) || BadPtr(receiver) || BadPtr(msg)) {
        return MOD_CONT;
    }

    u = user_find(source);

    if (!u) {
    	srv = server_find(source);
    	/* We just ignore the message if sent from a server (inspircd) */
    	if (!srv) {
			alog(LOG_NONEXISTANT, "%s: user record for %s not found", msg,
				 source);
			denora_cmd_notice(receiver, source,
							  getstring(NULL, USER_RECORD_NOT_FOUND));
    	}
        return MOD_CONT;
    }

    if (*receiver == '#') {
        if (denora->do_sql && !LargeNet) {
            if (stricmp(u->nick, s_StatServ)
                || stricmp(u->nick, s_StatServ_alias)) {
                do_cstats(u, receiver, msg);
            }
        }
    } else {
        /* If a server is specified (nick@server format), make sure it matches
         * us, and strip it off. */
        s = strchr(receiver, '@');
        if (s) {
            *s++ = 0;
            if (stricmp(s, ServerName) != 0)
                return MOD_CONT;
        }
        p = findPrivMsg(receiver);
        if (p) {
            p->handler(u, msg);
            SET_SEGV_LOCATION();
        }
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle STATS commands
 *
 * @param source is the nick of the person whom sent the stats command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int m_stats(char *source, int ac, char **av)
{
    User *u;
    Dadmin *a;
    int i;

    if (ac < 1)
        return MOD_CONT;

    switch (*av[0]) {
    case 'l':
        u = user_find(source);

        if (u && is_oper(u)) {

            denora_cmd_numeric
                (source, 211,
                 "Server SendBuf SentBytes SentMsgs RecvBuf RecvBytes RecvMsgs ConnTime");
            denora_cmd_numeric(source, 211, "%s %d %d %d %d %d %d %ld",
                               RemoteServer, write_buffer_len(),
                               total_written, total_recmsg,
                               read_buffer_len(), total_read,
                               total_sendmsg,
                               time(NULL) - denora->start_time);
        }

        denora_cmd_219(source, NULL);
        break;
    case 'o':
    case 'O':
/* Check whether the user is an operator */
        u = user_find(source);
        if (u && !is_oper(u) && HideStatsO) {
            denora_cmd_219(source, av[0]);
        } else {
            for (i = 0; i < 1024; i++) {
                for (a = adminlists[i]; a; a = a->next) {
                    denora_cmd_numeric(source, 243, "O * * %s Admin 0",
                                       a->name);
                }
            }
            denora_cmd_219(source, av[0]);
        }

        break;

    case 'u':{
            int uptime = time(NULL) - denora->start_time;
            denora_cmd_numeric(source, 242,
                               ":Stats up %d day%s, %02d:%02d:%02d",
                               uptime / 86400,
                               (uptime / 86400 == 1) ? "" : "s",
                               (uptime / 3600) % 24, (uptime / 60) % 60,
                               uptime % 60);
            denora_cmd_numeric(source, 250,
                               ":Current users: %d (%d ops); maximum %d",
                               stats->users, stats->opers,
                               stats->users_max);
            denora_cmd_219(source, av[0]);
            break;
        }                       /* case 'u' */

    default:
        denora_cmd_219(source, av[0]);
        break;
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Handle VERSION commands
 *
 * @param source is the nick of the person whom sent the version command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int m_version(char *source, int ac, char **av)
{

    USE_VAR(ac);
    USE_VAR(av);

    if (source) {
        denora_cmd_351(source);
    }
    return MOD_CONT;
}


/*************************************************************************/

/**
 * Handle WHOIS commands
 *
 * @param source is the nick of the person whom sent the stats command
 * @param who is the person being whois'd
 *
 * @return always returns MOD_CONT
 */
int m_whois(char *source, char *who)
{
    const char *clientdesc;

    if (source && who) {
        if (stricmp(who, s_StatServ) == 0) {
            clientdesc = desc_StatServ;
        } else if (s_StatServ_alias && stricmp(who, s_StatServ_alias) == 0) {
            clientdesc = desc_StatServ_alias;
        } else {
            denora_cmd_401(source, who);
            return MOD_CONT;
        }
        denora_cmd_numeric(source, 311, "%s %s %s * :%s", who,
                           ServiceUser, ServiceHost, clientdesc);
        /* denora_cmd_219(source, who); */
        denora_cmd_numeric(source, 317,
                           "%s %ld %ld :seconds idle, signon time", who,
                           time(NULL) - denora->start_time,
                           denora->start_time);
        denora_cmd_318(source, who);
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * NULL route events
 *
 * @param source is the nick of the person whom sent the command
 * @param ac is the parameter count
 * @param av is the parameter array
 *
 * @return always returns MOD_CONT
 */
int denora_event_null(char *source, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(source, ac, av);
    }
    return MOD_CONT;
}

/*************************************************************************/

/**
 * Add Internal Messages that are not wired to any ircd
 *
 * @return void - no returend value
 *
 */
void moduleAddMsgs(void)
{
    Message *m;
    int status;

    m = createMessage("STATS", m_stats);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for STATS [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("TIME", m_time);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for TIME [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("VERSION", m_version);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for VERSION [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    m = createMessage("005", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 005 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("020", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 020 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("105", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 105 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("216", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 216 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("217", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 217 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("219", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 219 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    /* /stats k */
    m = createMessage("223", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 223 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("244", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 244 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("249", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 249 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("250", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 250 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("263", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 263 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("304", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 304 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("401", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 401 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("402", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 402 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("422", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 422 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("451", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 451 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("461", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 461 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("481", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 481 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("482", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 482 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("503", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 503 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    /* Ithildin Adds some replies */
    m = createMessage("771", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 771 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("773", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 773 [%d][%s]", status,
             ModuleGetErrStr(status));
    }

    m = createMessage("774", denora_event_null);
    status = addCoreMessage(IRCD, m);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for 774 [%d][%s]", status,
             ModuleGetErrStr(status));
    }
}

/*************************************************************************/

/* StatServ core functions
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: ss_set.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */
/*************************************************************************/

#include "denora.h"

static int do_set(User * u, int ac, char **av);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    Command *c;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion("$Id: ss_set.c 386 2011-01-04 17:16:43Z hal9000 $");
    moduleSetType(CORE);

    c = createCommand("SET", do_set, is_stats_admin, -1, -1, -1,
                      STAT_HELP_SET);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/*************************************************************************/

int do_set(User * u, int ac, char **av)
{
    char *option = NULL;
    char *setting = NULL;
    int setindex;
    char buf[BUFSIZE];

    if (ac >= 2) {
        option = av[0];
        setting = av[1];
    } else if (ac == 1) {
        option = av[0];
    } else {
        alog(LOG_DEBUG, "ac %d", ac);
    }

    if (!option) {
        syntax_error(s_StatServ, u, "SET", STAT_SET_SYNTAX);
    } else if (stricmp(option, "LIST") == 0) {
        setindex =
            (denora->debug ? STAT_SET_LIST_OPTION_ON :
             STAT_SET_LIST_OPTION_OFF);
        notice_lang(s_StatServ, u, setindex, "DEBUG");
        setindex =
            (denora->do_html ? STAT_SET_LIST_OPTION_ON :
             STAT_SET_LIST_OPTION_OFF);
        notice_lang(s_StatServ, u, setindex, "HTML");
        setindex =
            (denora->do_sql ? STAT_SET_LIST_OPTION_ON :
             STAT_SET_LIST_OPTION_OFF);
        notice_lang(s_StatServ, u, setindex, "SQL");
    } else if (!setting) {
        syntax_error(s_StatServ, u, "SET", STAT_SET_SYNTAX);
    } else if (stricmp(option, "SQL") == 0) {
        if (!u->confadmin) {
            notice_lang(s_StatServ, u, PERMISSION_DENIED);
            return MOD_CONT;
        }
        if (stricmp(setting, "on") == 0) {
            if (rdb_init()) {
                notice_lang(s_StatServ, u, STAT_SET_SQL_ON);
                /* we need to restart denora so sql is resynced */
#ifdef STATS_BIN
                denora->qmsg = calloc(512 + strlen(u->nick), 1);
                if (!denora->qmsg) {
                    ircsnprintf(denora->qmsg, 512,
                                "RESTART command received, but out of memory!");
                } else {
                    ircsnprintf(buf, BUFSIZE,
                                "RESTART command received from %s",
                                u->nick);
                    denora->qmsg = sstrdup(buf);
                }
                do_restart_denora();
#else
                notice_lang(s_StatServ, u, STAT_CANNOT_RESTART);
#endif
            } else {
                notice_lang(s_StatServ, u, STAT_SET_SQL_ERROR_INIT);
            }
        } else if (stricmp(setting, "off") == 0) {
            rdb_close();
            notice_lang(s_StatServ, u, STAT_SET_SQL_OFF);
        } else {
            notice_lang(s_StatServ, u, STAT_SET_SQL_ERROR);
        }
    } else if (stricmp(option, "HTML") == 0) {
        if (stricmp(setting, "on") == 0) {
            denora->do_html = 1;
            notice_lang(s_StatServ, u, STAT_SET_HTML_ON);
        } else if (stricmp(setting, "off") == 0) {
            denora->do_html = 0;
            notice_lang(s_StatServ, u, STAT_SET_HTML_OFF);
        } else {
            notice_lang(s_StatServ, u, STAT_SET_HTML_ERROR);
        }
    } else if (stricmp(option, "DEBUG") == 0) {
        if (stricmp(setting, "on") == 0) {
            denora->debug = 1;
            alog(LOG_NORMAL, "Debug mode activated");
            notice_lang(s_StatServ, u, STAT_SET_DEBUG_ON);
        } else if (stricmp(setting, "off") == 0 ||
                   (*setting == '0' && atoi(setting) == 0)) {
            alog(LOG_NORMAL, "Debug mode deactivated");
            denora->debug = 0;
            notice_lang(s_StatServ, u, STAT_SET_DEBUG_OFF);
        } else if (isdigit(*setting) && atoi(setting) <= 2) {
            denora->debug = atoi(setting);
            alog(LOG_NORMAL, "Debug mode activated (level %d)",
                 denora->debug);
            notice_lang(s_StatServ, u, STAT_SET_DEBUG_LEVEL,
                        denora->debug);
        } else {
            notice_lang(s_StatServ, u, STAT_SET_DEBUG_ERROR);
        }
    } else {
        notice_lang(s_StatServ, u, STAT_SET_UNKNOWN_OPTION, option);
    }
    return MOD_CONT;
}

/*************************************************************************/

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
 * $Id: ss_fantasy_gstats.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */

/*************************************************************************/

#include "denora.h"

int do_fantasy(int argc, char **argv);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);

/**
 * Create the hook, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    EvtHook *hook;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }

    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id: ss_fantasy_gstats.c 386 2011-01-04 17:16:43Z hal9000 $");
    moduleSetType(CORE);

    hook = createEventHook(EVENT_FANTASY, do_fantasy);
    moduleAddEventHook(hook);

    if (!denora->do_sql) {
        return MOD_STOP;
    }

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

/**
 * Handle seen fantasy command.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT or MOD_STOP
 **/
int do_fantasy(int argc, char **argv)
{
    User *u;
    char *chan;
#ifdef USE_MYSQL
    MYSQL_RES *mysql_res;
#endif
    ChannelStats *cs = NULL;

    if (argc < 3)
        return MOD_CONT;

    if (stricmp(argv[0], "gstats") == 0) {
        u = finduser(argv[1]);
        if (!denora->do_sql) {
            return MOD_CONT;
        }
        if (!u->sgroup) {
            return MOD_CONT;
        }
        chan = rdb_escape(argv[2]);
        cs = find_cs(argv[2]);
        rdb_query
            (QUERY_HIGH,
             "SELECT * FROM %s WHERE chan=\'global\' AND type=0 AND uname=\'%s\';",
             UStatsTable, u->sgroup);
#ifdef USE_MYSQL
        mysql_res = mysql_store_result(mysql);
        if (mysql_num_rows(mysql_res) > 0) {
            SET_SEGV_LOCATION();
            while ((mysql_row = mysql_fetch_row(mysql_res)) != NULL) {
                if (cs->flags & CS_NOTICE) {
                    notice_lang(s_StatServ, u, STATS_USER_NETWORK,
                                u->nick);
                    notice_lang(s_StatServ, u, STATS_MESSAGE_ONE,
                                mysql_row[3], mysql_row[4],
                                mysql_row[5], mysql_row[7], mysql_row[6]);
                } else {
                    denora_cmd_privmsg(s_StatServ, argv[2],
                                       getstring(NULL,
                                                 STATS_USER_NETWORK),
                                       u->nick);
                    denora_cmd_privmsg(s_StatServ, argv[2],
                                       getstring(NULL,
                                                 STATS_MESSAGE_ONE),
                                       mysql_row[3], mysql_row[4],
                                       mysql_row[5], mysql_row[7],
                                       mysql_row[6]);
                }

            }
        }
        SET_SEGV_LOCATION();
        mysql_free_result(mysql_res);
#endif
    }

    return MOD_CONT;
}

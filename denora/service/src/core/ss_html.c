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
 * $Id: ss_html.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */
/*************************************************************************/

#include "denora.h"

static int do_htmlexport(User * u, int ac, char **av);
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
    moduleAddVersion("$Id: ss_html.c 386 2011-01-04 17:16:43Z hal9000 $");
    moduleSetType(CORE);

    c = createCommand("HTML", do_htmlexport, is_stats_admin, -1, -1, -1,
                      STAT_HELP_HTML);
    moduleAddCommand(STATSERV, c, MOD_UNIQUE);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

static int do_htmlexport(User * u, int ac, char **av)
{
    if (denora->protocoldebug) {
        protocol_debug(NULL, ac, av);
    }
    do_html();
    notice_lang(s_StatServ, u, STATS_HTML_EXPORTED);
    return MOD_CONT;
}

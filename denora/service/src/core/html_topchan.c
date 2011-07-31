/* Html Topchan
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: html_topchan.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */
/*************************************************************************/

#include "denora.h"

void html_current_channels(FILE * ptr);
void html_top10_onlinechan_title(FILE * ptr);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int top10chan_sort(const void *v, const void *v2);

/**
 * Create the command, and tell Denora about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int DenoraInit(int argc, char **argv)
{
    HTMLTag *h;
    int status;

    if (denora->debug >= 2) {
        protocol_debug(NULL, argc, argv);
    }
    moduleAddAuthor("Denora");
    moduleAddVersion
        ("$Id: html_topchan.c 386 2011-01-04 17:16:43Z hal9000 $");
    moduleSetType(CORE);

    h = createHTMLtag("!DAILYTOPCHAN!", html_current_channels);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !DAILYTOPCHAN! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!TOP10ONLINECHANTITLE!",
                      html_top10_onlinechan_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TOP10ONLINECHANTITLE! [%d][%s]",
             status, ModuleGetErrStr(status));
    }


    return MOD_CONT;
}

/**
 * Unload the module
 **/
void DenoraFini(void)
{

}

int top10chan_sort(const void *v, const void *v2)
{
    const StatsChannel *t = (void *) v;
    const StatsChannel *t2 = (void *) v2;
    return (t2->usercount - t->usercount);
}


/*************************************************************************/

void html_current_channels(FILE * ptr)
{
    StatsChannel *s;
    lnode_t *tn;
    char *tempc;
    Exclude *e;
    int x = 0;

    if (ptr) {
        fprintf(ptr, "%s", getstring(NULL, HTML_CHANNEL_HEADER));
        SET_SEGV_LOCATION();
        list_sort(StatsChanhead, top10chan_sort);
        tn = list_first(StatsChanhead);
        while (tn) {
            s = lnode_get(tn);
            if (SP_HTML && (s->secret || s->private)) {
                tn = list_next(StatsChanhead, tn);
                continue;
            }
            if (s) {
                e = find_exclude(s->name, NULL);
                if (!e) {
                    if (s->in_use) {
                        tempc = char_encode(s->name);
                        dfprintf(ptr, langstring(HTML_CHANNEL_CONTENT),
                                 tempc, s->usercount);
                        free(tempc);
                        x++;
                        if (x > 10) {
                            break;
                        }
                    }
                }
            }
            tn = list_next(StatsChanhead, tn);

        }
        fprintf(ptr, "%s", getstring(NULL, HTML_CHANNEL_FOOTER));
        SET_SEGV_LOCATION();
    }
}

/*************************************************************************/

void html_top10_onlinechan_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstr(HTML_TOP10_ONLINE_CHANS_TITLE));
    }
}

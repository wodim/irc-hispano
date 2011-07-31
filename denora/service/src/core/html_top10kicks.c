/* HTML Top 10 Kicks
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: html_top10kicks.c 386 2011-01-04 17:16:43Z hal9000 $
 *
 */
/*************************************************************************/

#include "denora.h"

void html_kicks_title(FILE * ptr);
void html_top10kicks(FILE * ptr);
void html_mostkick_title(FILE * ptr);
int DenoraInit(int argc, char **argv);
void DenoraFini(void);
int top10kicks_sort(const void *v, const void *v2);

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
        ("$Id: html_top10kicks.c 386 2011-01-04 17:16:43Z hal9000 $");
    moduleSetType(CORE);

    h = createHTMLtag("!TOP10KICKS!", html_top10kicks);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TOP10KICKS! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!TOP10KICKSTITLE!", html_kicks_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !TOP10KICKSTITLE! [%d][%s]",
             status, ModuleGetErrStr(status));
    }

    h = createHTMLtag("!MOSTKICKTITLE!", html_mostkick_title);
    status = addHTMLTag(h);
    if (status != MOD_ERR_OK) {
        alog(LOG_NORMAL,
             "Error Occurried setting message for !MOSTKICKTITLE! [%d][%s]",
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

int top10kicks_sort(const void *v, const void *v2)
{
    const StatsChannel *t = (void *) v;
    const StatsChannel *t2 = (void *) v2;
    return (t2->kickcount - t->kickcount);
}

/*************************************************************************/

void html_top10kicks(FILE * ptr)
{
    StatsChannel *s;
    lnode_t *tn;
    char *tempc;
    int x = 0;
    Exclude *e;

    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_CHAN_TOP10_KICKS_HEADER));
        SET_SEGV_LOCATION();
        list_sort(StatsChanhead, top10kicks_sort);
        tn = list_first(StatsChanhead);
        while (tn) {
            s = lnode_get(tn);
            if (SP_HTML && (s->secret || s->private)) {
                tn = list_next(StatsChanhead, tn);
                continue;
            }
            if (s) {
                if (s->kickcount) {
                    e = find_exclude(s->name, NULL);
                    if (!e) {
                        tempc = char_encode(s->name);
                        if (s->in_use) {
                            dfprintf(ptr,
                                     langstring
                                     (HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE),
                                     tempc, s->kickcount);
                        } else {
                            dfprintf(ptr,
                                     langstring
                                     (HTML_CHAN_TOP10_KICKS_CONTENT),
                                     tempc, s->kickcount);
                        }
                        free(tempc);
                    }
                }
                x++;
                if (x > 10) {
                    break;
                }
            }
            tn = list_next(StatsChanhead, tn);

        }
        fprintf(ptr, "%s", langstring(HTML_CHAN_TOP10_KICKS_FOOTER));
        SET_SEGV_LOCATION();
    }
}

/*************************************************************************/

void html_kicks_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstring(HTML_CHAN_TOP10_KICKS_TITLE));
    }
}

/*************************************************************************/

void html_mostkick_title(FILE * ptr)
{
    SET_SEGV_LOCATION();
    if (ptr) {
        fprintf(ptr, "%s", langstr(HTML_MOST_UNWELCOME_TITLE));
    }
}

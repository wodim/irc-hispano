/*
 * irc_militime.h (from Undernet ircu's opercmds.h)
 *
 * $Id: irc_militime.h,v 1.1.1.1 2003/12/30 17:09:31 mbuna Exp $
 */
#ifndef INCLUDED_IRC_MILITIME_H
#define INCLUDED_IRC_MILITIME_H
#ifndef INCLUDED_sys_types_h
#include <sys/types.h>
#define INCLUDED_sys_types_h
#endif

extern char * militime(char* sec, char* usec);
extern char * militime_float(char *start);

#endif /* INCLUDED_IRC_MILITIME_H */

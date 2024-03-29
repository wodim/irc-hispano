/*
 * IRC-Dev IRCD - An advanced and innovative IRC Daemon, ircd/ircd_lexer.l
 *
 * Copyright (C) 2002-2014 IRC-Dev Development Team <devel@irc-dev.net>
 * Copyright (C) 2006 Michael Poole
 * Copyright (C) 2001 Andrew Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: ircd_lexer.l,v 1.17 2007-12-11 23:38:25 zolty Exp $
 *
 */

%option never-interactive case-insensitive warn nodefault nounput yylineno

%{

#include "config.h"
#include "fileio.h"
#include "ircd_alloc.h"
#include "ircd_string.h"
#include "ircd_parser.h"

extern void yyserror(const char *pattern, ...);

#define lexer_input ((FBFILE*)yyin)

#undef YY_INPUT
#define YY_INPUT(buf, res, size) res = (fbgets(buf, size, lexer_input) ? strlen(buf) : 0)

int
init_lexer(const char *configfile)
{
  yyin = (FILE*)fbopen(configfile, "r");
  if (lexer_input == NULL)
  {
#ifdef YY_FATAL_ERROR
    YY_FATAL_ERROR("Could not open the configuration file.");
#else
    fprintf(stderr, "Could not open the configuration file.");
#endif
    return 0;
  }
  return 1;
}

void
lexer_include(const char *filename)
{
  if (filename == NULL)
  {
    yypush_buffer_state(yy_scan_string(""));
    return;
  }

  yyin = fopen(filename, "r");
  if (yyin == NULL)
  {
    yyserror("Unable to open included file '%s'.", filename);
    yypush_buffer_state(yy_scan_string(""));
    return;
  }
  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
}

void deinit_lexer(void)
{
  if (lexer_input != NULL)
  {
    fbclose(lexer_input);
    yyin = NULL;
  }
}

%}

%%

\"[^"\n]+[\"\n]	{yytext[yyleng-1] = 0; DupString(yylval.text, yytext+1); return QSTRING;}
[0-9]+		{yylval.num = strtoul(yytext, NULL, 10); return NUMBER;}
[ \t\r\n]+	;
#.*		;
<<EOF>>		{ yypop_buffer_state(); if (YY_CURRENT_BUFFER) return TEOF; else { yy_init = 0; yyterminate(); } }

\&\&            return LOGICAL_AND;
\|\|            return LOGICAL_OR;
ADMIN		return ADMIN;
ADMINISTRATOR	return ADMIN;
APASS_OPMODE	return TPRIV_APASS_OPMODE;
ALL		return ALL;
AUTO		return AUTOCONNECT;
AUTOCONNECT	return AUTOCONNECT;
B		return BYTES;
BADCHAN		return TPRIV_BADCHAN;
BYTES		return BYTES;
CHAN_LIMIT	return TPRIV_CHAN_LIMIT;
CLASS		return CLASS;
CLIENT		return CLIENT;
CONNECT		return CONNECT;
CONNECTED       return CONNECTED;
CONNECTFREQ	return CONNECTFREQ;
CONTACT		return CONTACT;
CRULE		return CRULE;
DAYS		return DAYS;
DECADES		return DECADES;
DEOP_LCHAN	return TPRIV_DEOP_LCHAN;
DESCRIPTION	return DESCRIPTION;
DIE		return TPRIV_DIE;
DIRECTCON       return DIRECTCON;
DIRECTOP        return DIRECTOP;
DISPLAY		return TPRIV_DISPLAY;
DNS             return DNS;
EXCEPT		return EXCEPT;
EXEMPT          return EXEMPT;
FAST		return FAST;
FEATURES	return FEATURES;
FILE		return TFILE;
FORCE_LOCAL_OPMODE	return TPRIV_FORCE_LOCAL_OPMODE;
FORCE_OPMODE	return TPRIV_FORCE_OPMODE;
FROM		return FROM;
GB		return GBYTES;
GBYTES		return GBYTES;
GENERAL		return GENERAL;
GIGABYTES	return GBYTES;
GLINE		return TPRIV_GLINE;
HIDDEN		return HIDDEN;
HOST		return HOST;
HOURS		return HOURS;
HUB		return HUB;
IAUTH		return IAUTH;
INCLUDE		return INCLUDE;
IP		return IP;
IPV4		return TOK_IPV4;
IPV6		return TOK_IPV6;
JUPE		return JUPE;
KB		return KBYTES;
KBYTES		return KBYTES;
KILOBYTES	return KBYTES;
KILL		return KILL;
LEAF		return LEAF;
LINESYNC	return LINESYNC;
LIST_CHAN	return TPRIV_LIST_CHAN;
LOCAL		return LOCAL;
LOCAL_BADCHAN	return TPRIV_LOCAL_BADCHAN;
LOCAL_GLINE	return TPRIV_LOCAL_GLINE;
LOCAL_JUPE	return TPRIV_LOCAL_JUPE;
LOCAL_OPMODE	return TPRIV_LOCAL_OPMODE;
LOCATION	return LOCATION;
MASK		return MASK;
MAXHOPS		return MAXHOPS;
MAXLINKS	return MAXLINKS;
MB		return MBYTES;
MBYTES		return MBYTES;
MEGABYTES	return MBYTES;
MINUTES		return MINUTES;
MODE_LCHAN	return TPRIV_MODE_LCHAN;
MONTHS		return MONTHS;
MOTD		return MOTD;
NAME		return NAME;
NICK		return NICK;
NO		return NO;
NUMERIC		return NUMERIC;
OPER		return OPER;
OPERATOR	return OPER;
OPMODE		return TPRIV_OPMODE;
PASS		return PASS;
PASSWORD	return PASS;
PINGFREQ	return PINGFREQ;
PORT		return PORT;
PREPEND		return PREPEND;
PROGRAM		return PROGRAM;
PROPAGATE	return TPRIV_PROPAGATE;
PSEUDO		return PSEUDO;
QUARANTINE	return QUARANTINE;
REAL		return REAL;
REALNAME	return REAL;
REASON		return REASON;
REHASH		return TPRIV_REHASH;
RESTART		return TPRIV_RESTART;
RULE		return RULE;
SECONDS		return SECONDS;
SEE_CHAN	return TPRIV_SEE_CHAN;
SEE_OPERS	return TPRIV_SEE_OPERS;
SENDQ		return SENDQ;
SERVER		return SERVER;
SET		return TPRIV_SET;
SHOW_ALL_INVIS	return TPRIV_SHOW_ALL_INVIS;
SHOW_INVIS	return TPRIV_SHOW_INVIS;
SSL		return SSLPORT;
TB		return TBYTES;
TBYTES		return TBYTES;
TERABYTES	return TBYTES;
UNLIMIT_QUERY	return TPRIV_UNLIMIT_QUERY;
USERMODE	return USERMODE;
USERNAME	return USERNAME;
UWORLD		return UWORLD;
VHOST		return VHOST;
VIA             return VIA;
WALK_LCHAN	return TPRIV_WALK_LCHAN;
WEEKS		return WEEKS;
WIDE_GLINE	return TPRIV_WIDE_GLINE;
WHOX		return TPRIV_WHOX;
YEARS		return YEARS;
YES		return YES;
ZLIB		return ZLIB;
ZLIB_IN		return ZLIB_IN;
ZLIB_OUT	return ZLIB_OUT;

. return yytext[0];

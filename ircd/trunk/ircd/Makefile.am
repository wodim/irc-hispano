## AutoMake Makefile for the IRC-Dev IRC Daemon sources
##
## Copyright (C) 2005-2014 IRC-Dev Development Team <devel@irc-dev.net>
## Copyright (C) 2005 by Kevin L. Mitchell <klmitch@mit.edu>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##
## @(#)$Id: Makefile.am,v 1.2 2007-11-11 21:53:06 zolty Exp $

#SUBDIRS = test
noinst_PROGRAMS = table_gen
bin_PROGRAMS = convert-conf umkpasswd ircd

AM_CPPFLAGS = -I$(top_srcdir)/include -I../libs/zlib -DDPATH="\"$(sysconfdir)\"" \
	-DSPATH="\"$(bindir)/ircd\"" -DCPATH="\"ircd.conf\"" \
	-DLPATH="\"ircd.log\""

AM_LDFLAGS = -lresolv -lrt

BUILT_SOURCES = ircd_parser.h
AM_YFLAGS = -d

EXTRA_DIST = version.c.SH

DISTCLEANFILES = version.c

umkpasswd_SOURCES = \
	ircd_md5.c \
	ircd_crypt_plain.c \
	ircd_crypt_smd5.c \
	ircd_crypt_native.c \
	ircd_alloc.c \
	ircd_string.c \
	memdebug.c \
	umkpasswd.c

nodist_ircd_SOURCES = version.c
ircd_SOURCES = \
    IPcheck.c \
    channel.c \
    class.c \
    client.c \
    crule.c \
    dbuf.c \
    destruct_event.c \
    fileio.c \
    gline.c \
    hash.c \
    ircd.c \
    ircd_alloc.c \
    ircd_crypt.c \
    ircd_crypt_plain.c \
    ircd_crypt_smd5.c \
    ircd_crypt_native.c \
    ircd_events.c \
    ircd_features.c \
    ircd_lexer.l \
    ircd_log.c \
    ircd_md5.c \
    ircd_parser.y \
    ircd_relay.c \
    ircd_reply.c \
    ircd_res.c \
    ircd_reslib.c \
    ircd_signal.c \
    ircd_snprintf.c \
    ircd_ssl.c \
    ircd_string.c \
    ircd_tea.c \
    ircd_zlib.c \
    jupe.c \
    list.c \
    listener.c \
    m_admin.c \
    m_asll.c \
    m_away.c \
    m_burst.c \
    m_cap.c \
    m_clearmode.c \
    m_close.c \
    m_connect.c \
    m_cprivmsg.c \
    m_create.c \
    m_defaults.c \
    m_destruct.c \
    m_desynch.c \
    m_die.c \
    m_endburst.c \
    m_error.c \
    m_get.c \
    m_gline.c \
    m_help.c \
    m_info.c \
    m_invite.c \
    m_ison.c \
    m_join.c \
    m_jupe.c \
    m_kick.c \
    m_kill.c \
    m_links.c \
    m_list.c \
    m_lusers.c \
    m_map.c \
    m_mode.c \
    m_motd.c \
    m_names.c \
    m_nick.c \
    m_notice.c \
    m_oper.c \
    m_opmode.c \
    m_part.c \
    m_pass.c \
    m_ping.c \
    m_pong.c \
    m_privmsg.c \
    m_privs.c \
    m_proto.c \
    m_pseudo.c \
    m_quit.c \
    m_rehash.c \
    m_reset.c \
    m_restart.c \
    m_rping.c \
    m_rpong.c \
    m_server.c \
    m_set.c \
    m_settime.c \
    m_silence.c \
    m_squit.c \
    m_stats.c \
    m_time.c \
    m_topic.c \
    m_trace.c \
    m_uping.c \
    m_user.c \
    m_users.c \
    m_userhost.c \
    m_userip.c \
    m_version.c \
    m_wallchops.c \
    m_wallops.c \
    m_wallusers.c \
    m_wallvoices.c \
    m_watch.c \
    m_who.c \
    m_whois.c \
    m_whowas.c \
    m_xquery.c \
    m_xreply.c \
    match.c \
    memdebug.c \
    motd.c \
    msgq.c \
    numnicks.c \
    opercmds.c \
    os_generic.c \
    packet.c \
    parse.c \
    querycmds.c \
    random.c \
    s_auth.c \
    s_bsd.c \
    s_conf.c \
    s_debug.c \
    s_err.c \
    s_misc.c \
    s_numeric.c \
    s_serv.c \
    s_stats.c \
    s_user.c \
    send.c \
    uping.c \
    userload.c \
    watch.c \
    whowas.c

if PCRE_MATCH
ircd_SOURCES += \
    pcre_match.c
endif
	
if ENVIRONMENT_UNDERNET
ircd_SOURCES += \
    m_account.c
endif

if ENVIRONMENT_DDB
ircd_SOURCES += \
    ddb.c \
    ddb_events.c \
    m_bmode.c \
    m_db.c \
    m_dbq.c \
    m_ghost.c \
    m_svsnick.c \
    m_svsmode.c \
    m_svsjoin.c \
    m_svskick.c \
    m_svspart.c \
    persistent_malloc.c
endif

if ENVIRONMENT_SERVICES
ircd_SOURCES += \
    m_svsnick.c \
    m_svsmode.c \
    m_svsjoin.c \
    m_svskick.c \
    m_svspart.c
endif


if ENGINE_POLL
ircd_SOURCES += engine_poll.c
else
ircd_SOURCES += engine_select.c
endif

if ENGINE_DEVPOLL
ircd_SOURCES += engine_devpoll.c
endif
if ENGINE_EPOLL
ircd_SOURCES += engine_epoll.c
endif
if ENGINE_KQUEUE
ircd_SOURCES += engine_kqueue.c
endif

ircd_LDADD = $(LEXLIB)

ircd_string.$(OBJEXT): ircd_string.c chattr.tab.c
chattr.tab.c: table_gen
	./table_gen > chattr.tab.c

version.c: ../include/patchlist.h version.c.SH umkpasswd \
	../include/version.h ../include/patchlevel.h $(ircd_SOURCES)
	$(SHELL) $(srcdir)/version.c.SH $(top_srcdir)
../include/patchlist.h:
	cd $(top_srcdir) && ./ircd-patch update

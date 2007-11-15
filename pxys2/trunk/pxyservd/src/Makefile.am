AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = pxyservd

pxyservd_SOURCES = \
	cfgloader.h debug.h evreg.h glineq.h hash_fun.h irc_auth.h irc_channel.h irc_channels.h irc_chattr.h irc_client.h irc_cmd.h irc_gline.h irc_init.h irc_membership.h irc_militime.h irc_mode.h irc_msg.h irc_network.h irc_numnicks.h irc_parser.h irc_send.h irc_string.h irc_struct.h irc_userbase.h irc_yxx.h match.h opas_support.h pxyservd_log.h scan.h session.h tokenizer.h \
	cfgloader.c cmd_evreg.c cmd_evshow.c cmd_grem.c cmd_help.c cmd_info.c cmd_inv.c cmd_noscan.c cmd_proxytop.c cmd_pxstats.c cmd_recheck.c cmd_servers.c cmd_showcmds.c cmd_stats.c cmd_status.c debug.c evreg.c glineq.c hash_fun.c irc_auth.c irc_channel.c irc_chattr.c irc_client.c irc_gline.c irc_init.c irc_membership.c irc_mode.c irc_militime.c irc_network.c irc_numnicks.c irc_parser.c irc_send.c irc_string.c irc_userbase.c irc_yxx.c match.c opas_support.c pxyservd.c pxyservd_log.c scan.c session.c tokenizer.c
pxyservd_SOURCES += ../../ipcache/ipcache.c

pxyservd_LDADD = @dbprim_ldadd@ \
                 ../libbcl/.libs/libbcl.a \
                 ../../libopas/opas/.libs/libopas.a

INCLUDES = -Wall -I../../pxyscand/include

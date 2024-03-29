# Russian language file.
#
# Denora (c) 2004-2011 Denora Team
# Contact us at dev@denorastats.org
#
# This program is free but copyrighted software; see the file COPYING for
# details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id: ru.l 391 2011-01-11 14:22:48Z hal9000 $
# Charaster set is cp1251
#
# When translating this file to another language, keep in mind that the
# order of parameters for sprintf() is fixed in the source code, so your
# messages need to take the same parameters in the same order as the
# English messages do.  (Obviously, this doesn't hold for the strftime()
# format lines immediately below.)  If you can't get a natural translation
# of a message without changing the order of the parameters, let us know
# (dev@denorastats.org) which message is causing a problem and I'll see
# what I can do.
#
# In help messages, "%S" (capital S, not lowercase) refers to the name of
# the service sending the message; for example, in StatServ help messages,
# "%S" is replaced by "StatServ" (or whatever it is renamed to in
# denora.conf).  The %S's do not count as sprintf() parameters, so they can be
# rearranged, removed, or added as necessary.
#
# Also in help messages, please try to limit line lengths to 60 characters
# of text (not including the leading tab).  This length was chosen because
# it does not cause line wrap under default settings on most current IRC
# clients.  Remember that format characters (control-B, control-_) are not
# included in that 60-character limit (since they don't show on the user's
# screen).  Also remember that format specifiers (%S, etc.) will expand
# when displayed, so remember to take this into account; you can assume
# that the length of a pseudoclient name (%S replacement) will be eight
# characters, so reduce the maximum line length by 6 for every %S on a
# line.
#
# Finally, remember to put a tab at the beginning of every line of text
# (including empty lines).  This has to be a tab, not spaces.

###########################################################################
#
# Name of this language
#
###########################################################################

# For languages other than English, this string should have the following
# format:
#     language-name-in-language (language-name-in-English)
# For example, "Espa�ol (Spanish)" or "Fran�ais (French)".

LANG_NAME
	������� (Russian)

###########################################################################
#
# General messages
#
###########################################################################

# strftime() format strings.  man 3 strftime for information on the
# meanings of the format specifiers.  Short version:
#	%a = weekday name (short)	%H = hour
#	%b = month name (short)		%M = minute
#	%d = day of month		%S = second
#	%Y = year			%Z = time zone

# This is used as the format string for strftime() for a date and time
# together.
STRFTIME_DATE_TIME_FORMAT
	%b %d %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %b %d %Y
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%b %d %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	���
	��
	��
	��
	��
	��
	��
# %A
STRFTIME_DAYS_LONG
	�����������
	�����������
	�������
	�����
	�������
	�������
	�������
# %b
STRFTIME_MONTHS_SHORT
	���
	���
	���
	���
	���
	���
	���
	���
	���
	���
	���
	���
# %B
STRFTIME_MONTHS_LONG
	������
	�������
	����
	������
	���
	����
	����
	������
	��������
	�������
	������
	�������


# This is used in StatServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	���������� ������ - ���������� ��������� ������.
UNKNOWN_COMMAND
	����������� ������� %s.
UNKNOWN_COMMAND_HELP
	����������� ������� %s. ����������� "/msg %s HELP" ��� ������ �� ��������.
SYNTAX_ERROR
	���������: %s
MORE_INFO
	/msg %s HELP %s ��� ����� ��������� ����������.
NO_HELP_AVAILABLE
	���������� ���������� �� %s �����������.

SERVICE_OFFLINE
	%s �� ������ ������ ��������� (offline).

SQL_DISABLED
	�� �� ������ ������������ ������ ������� - ������ � SQL ���������.

STATS_UPTIME_DHMS
	���������� ������� %d ����, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	���������� ������� %d ����, %02d:%02d:%02d
STATS_UPTIME_HMS
	���������� ������� %d �����, %d �����, %d ������
STATS_UPTIME_HM1S
	���������� ������� %d �����, %d �����, %d �������
STATS_UPTIME_H1MS
	���������� ������� %d �����, %d ������, %d ������
STATS_UPTIME_H1M1S
	���������� ������� %d �����, %d ������, %d �������
STATS_UPTIME_1HMS
	���������� ������� %d ���, %d �����, %d ������
STATS_UPTIME_1HM1S
	���������� ������� %d ���, %d �����, %d �������
STATS_UPTIME_1H1MS
	���������� ������� %d ���, %d ������, %d ������
STATS_UPTIME_1H1M1S
	���������� ������� %d ���, %d ������, %d �������
STATS_UPTIME_MS
	���������� ������� %d �����, %d ������
STATS_UPTIME_M1S
	���������� ������� %d �����, %d ������
STATS_UPTIME_1MS
	���������� ������� %d ������, %d ������
STATS_UPTIME_1M1S
	���������� ������� up %d ������, %d ������

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stats admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	������ ��������.
PERMISSION_DENIED
	� ��� ������������ ���� ��� ������������� ������ �������.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	��
SAY_NO
	���

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN �� ������ - ���������� ������� RESTART ����������.
	������������� ���������������� ������ \2configure\2 �
	������������ Denora'� � ���������� ������� RESTART.

# Module strings

STAT_MODULE_LOADED
	������ %s ������� ��������.
STAT_MODULE_UNLOADED
	������ %s ������� ��������.
STAT_MODULE_LOAD_FAIL
	���������� ��������� ������ %s.
STAT_MODULE_REMOVE_FAIL
	���������� ��������� ������ %s.
STAT_MODULE_LOAD_SYNTAX
	MODLOAD ��������_�����
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD ��������_�����
STAT_MODULE_NO_UNLOAD
	������ ������ �� ����� ���� ��������.
STAT_MODULE_LIST_HEADER
	������ ������� �������:
STAT_MODULE_LIST
	������: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	����� ������� ���������: %d
STAT_MODULE_INFO_LIST
	������: %s ������: %s �����: %s ��������: %s
STAT_MODULE_CMD_LIST
	��������������� �������: /msg %s %s
STAT_MODULE_MSG_LIST
	��������������� IRCD-���������� ���: %s
STAT_MODULE_NO_LIST
	������ ���������� ������� ����.
STAT_MODULE_NO_INFO
	���������� � ������ %s ����������.
STAT_MODULE_INFO_SYNTAX
	MODINFO ��������_�����
MODULE_HELP_HEADER
	������ ������ ������������� ������� ��������� �������:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	������ ������ %S:
	    STATUS       ���������� ���������� ����.
	    LOGIN       ������������� �� �������� ����������.
	    LOGOUT      ��������������� �� �������� ����������.

STAT_HELP_STATUS
	���������: STATUS
	
	������� ���������� � ���������� �������� ����, ����������
	��������� ������� � � ���������� ������������� ����.

STAT_HELP_SHUTDOWN
	���������: SHUTDOWN
	
	��������� ���� ������ �������� ���������� � ��������� �� ������.
	
	������ ������� �������� ������ ��������������� ����������

STAT_HELP_RESTART
	���������: RESTART
	
	������������� ������� ����������.
	(�� ���� ��������� �� ������ � ��� �� �������� ������)

STAT_HELP_RELOAD
	���������: RELOAD
	
	������������ ����������������� ����� �������� ����������.

STAT_HELP_ADMIN_CMD
	������ ������, ��������� ������ ��������������� ����������:
	    ADMIN       ���������� ������� ���������������.
	    CHANSTATS   ���������� ������� ������� ����������.
	    ENVINFO     ����� ��������� �������������� ����������.
	    EXCLUDE     ������ �������� ���������� �� ����������.
	    EXPORT      ������� ������� ������ � XML.
	    HTML        �������� HTML-����� ����������.
	    MODLOAD     �������� ������.
	    MODUNLOAD   �������� ������.
	    MODLIST     ������ ����������� �������.
	    MODINFO     ������ ���������� � ����������� ������.
	    RESTART     ���������� �������� ����������.
	    RELOAD      ������������ ����������������� �����.
	    SET         ��������� ��� ����� ��������� ����� "�� ����".
	    SHUTDOWN    ���������� ������ ��������.

STAT_HELP_MODLOAD
	���������: MODLOAD ��������_�����

	������� MODLOAD ��������� ��� ��������� �����-����
	�������������� ������ �� ���������� � �������� (modules/).

	������ ������� �������� ������ ��������������� ����������

STAT_HELP_MODUNLOAD
	���������: MODUNLOAD ��������_�����

	������� MODUNLOAD ��������� ��� ��������� �����-����
	�������������� ������ �� ������ ������� ����������� �������.

	������ ������� �������� ������ ��������������� ����������.

STAT_HELP_MODINFO
	���������: MODINFO ��������_�����

	������� ��������� ���������� ���������� �� ��������� ������.

	������ ������� �������� ������ ��������������� ����������.

STAT_HELP_MODLIST
	���������: MODLIST

	������� ������ ���� ����������� �������.

	������ ������� �������� ������ ��������������� ����������.

STAT_HELP_EXCLUDE
	���������: EXCLUDE {ADD|DEL|LIST} ���|#�����
	
	��������� ��������� ������ �� ����������.
	
	������ ������� �������� ������ ��������������� ����������

STAT_HELP_EXPORT
	���������: EXPORT ���_������ 
	
	������������ ��������� ������ � XML.
	���_������ � ������ ������ ����� ����:
	<channels|users|tld|servers|stats|all>
	
	������ ������� �������� ������ ��������������� ����������

STAT_HELP_HTML
	���������: HTML
	
	���������� ������� ���������� ������������� HTML-�����.

STAT_HELP_USERDEBUG
	���������: USERDEBUG user 
	
	���������� ���������� ���������� �� ��������� ������������.

STAT_HELP_LOGIN
	���������: LOGIN ����� ������
	
	��������� ��� ������������������ �� �������� ����������
	� �������� �������������� ����������

STAT_HELP_LOGOUT
	���������: LOGOUT
	
	���������������� ��� �� �������� ����������.
	(������ �������� �������������)

STAT_HELP_SET
	���������: SET OPTION [ON|OFF]

	������ ������� ��������� ��� ������������� ���������
	����� �������� ���������� "�� ����".

	��������� �����:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	������ ������� �������� ������ ��������������� ����������.

STAT_HELP_ENVINFO
	���������: ENVINFO
	
	���������� ��������� �������������� ����������.
	
	������ ������� �������� ������ ��������������� ����������.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	�� ������� ������������������ ��� ������������� ����������.

STAT_ADMIN_LOGGED_OUT
	�� ���� ������������������.

STAT_ADMIN_ALREADY_LOGGED
	�� ��� ���������������� �� �������� ����������.

STAT_LOGIN_SYNTAX
	LOGIN ����� ������

STAT_RELOAD
	���������������� ���� �������� ���������� ������� ������������.

###########################################################################
#
# STATUS messages
#
###########################################################################

STAT_STATUS_SERVER_COUNT
	���������� � �������� ���� %s:
	������� ���������� ��������:  %d
	����������� ������������ ��������: %d

STAT_STATUS_CHAN_COUNT
	���������� � ������� ���� %s:
	������� ���������� �������:  %d
	����������� ������� �������: %d

STAT_STATUS_USER_COUNT
	���������� � ������������� ���� %s
	������� ���-�� �������������:  %d
	������������ ���-�� �������������: %d

STAT_STATUS_SYNTAX
	STATUS [SERVERS|CHANNELS|USERS]

STAT_STATUS_PAGE
	��� ����� ��������� ���������� ��. %s

# SET responses
STAT_SET_SYNTAX
	SET ����� ��������
STAT_SET_DEBUG_ON
	������� ���������� ������� � ����� �������.
STAT_SET_DEBUG_OFF
	����� ������� ��������.
STAT_SET_DEBUG_LEVEL
	������� ���������� ������� � ����� ������� (������� %d).
STAT_SET_DEBUG_ERROR
	�������� ��� ����� DEBUG ������ ���� ON, OFF, ��� ������������� ������.
STAT_SET_UNKNOWN_OPTION
	����������� ����� %s.
STAT_SET_SQL_ON
	����� ������ �������� ���������� � SQL �� ������� �����������.
STAT_SET_SQL_OFF
	����� ������ �������� ���������� � SQL �� ������� �������������.
STAT_SET_SQL_ERROR
	�������� ��� ����� SQL ������ ���� ���� ON ���� OFF.
STAT_SET_SQL_ERROR_INIT
	������ ������������� SQL, ��������� ���� �� ������� ������.
STAT_SET_SQL_ERROR_DISABLED
	��������� ��� ������ �������� � SQL �� ������ ���� �� �������.
	������� ����������� �����/���������� � ���� denora.conf.
STAT_SET_LIST_OPTION_ON
	����� %s: �������.
STAT_SET_LIST_OPTION_OFF
	����� %s: ��������.
STAT_SET_HTML_ON
	����� ��������� HTML-������ ������� ������������.
STAT_SET_HTML_OFF
	����� ��������� HTML-������ ������� ��������������.
STAT_SET_HTML_ERROR
	�������� ��� ����� HTML ������ ���� ���� ON ���� OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	������ ������� �������� ��� ������������ ���� ���������� ����������
	������. ������� ����� �������� �������������� ���������� � ���-��
	�������� ���� �� ������, ���-�� ���� � ������������ � �. �.
	
	CHANSTATS ADD #�����
	CHANSTATS DEL #�����
	CHANSTATS LIST
	CHANSTATS SET #����� FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER ���1 ���2
	CHANSTATS RENAME ���1 ���2
	
	CHANSTATS ADD - ��������� ��������� ����� � ������ "��������������"
	�������.
	
	CHANSTATS DEL - ������� ���������� �������, ������� ����� �� ������
	"��������������" �������.
	
	CHANSTATS LIST - ������� ������� ������ "��������������" �������,
	�� ���� ��� �������, �� ������� ������� ���� ����������.
	
	CHANSTATS SET - ��������� ������������ ��� �������������� ���������
	fantasy-������� �� ��������� ������. ����� NOTICE ���������� ����� ON,
	�� ��� ���� �����������, ��� ����� �� fantasy-������� ����� ����������
	����������� ������ (�����������).
	
	CHANSTATS SUMUSER - ��������� �������������� ������ ����1 � ����2,
	� ������� ���2 (���1 ����� �������� � ����� ����. �����������).
	
	HANSTATS RENAME - ��������� ������������� ���1 � ���2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	����� %s ������� �������� � ������ ������� ��� ����� ����������.

STAT_CHANSTATS_CHAN_LARGENET
	�� �� ������ ��������� ����� ������ ��� ����� ����������,
	��� ��� �������� ����� LargeNet.

STAT_CHANSTATS_LOGCHAN
	��������� ����� �� ����� ���� �������� � ������ "��������������" �������.

STAT_CHANSTATS_NOT_VALID_CHAN
	������� ������������ ��� ������.

STAT_CHANSTATS_CHAN_ALREADY
	����� %s ��� ������������ � ������ "��������������" �������
	� ��� ���� ��� ������� ���� ����������.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD #����� 

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL #�����

STAT_CHANSTATS_CHAN_DEL
	����� %s ������� ������ �� ������ "��������������" �������.

STAT_CHANSTATS_CHAN_NOTFOUND
	����� %s �� ������ � ������ "��������������" �������.

STATS_CHANSTATS_PART
	���� ���������� ��� ������ %s ���������.

STATS_CHANSTATS_NOCHANNELS
	� ������ "��������������" ������� ��� �� ����� ������...

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET #����� FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	����� fantasy-������ ��� ������ %s �����������.

STATS_CHANSTATS_FANTASY_OFF
	����� fantasy-������ ��� ������ %s �������������.

STATS_CHANSTATS_FANTASY_NOTICE
	����� fantasy-������ ��� ������ %s �����������, �����
	���������� ����� �������������� ����������� �������

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER ���1 ���2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME ���1 ���2

STATS_CHANSTATS_SUMUSER_DONE
	��� �������������� ������ ������������ %s ���������� ������������ %s

STATS_CHANSTATS_RENAMEUSER_DONE
	������������ %s ��� ������������ � %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} ���|#�����

STAT_EXCLUDE_ADDED
	� ������ ���������� �� ���������� �������� ����� ������: %s

STAT_EXCLUDE_ALREADY
	������ %s ��� ���������� � ������ ����������.

STAT_EXCLUDE_DELETED
	������ %s ������� ������ �� ������ ���������� ����������.

STAT_EXCLUDE_NOT_FOUND
	������ %s �� ��������� � ������ ���������� �� ����������, ������� ������.

STAT_EXCLUDE_LIST_HEADER
	������ ����������:

# EXPORT responses
STATS_EXPORT_CHANNELS
	���� ������� ������� �������������� � %s

STATS_EXPORT_USERS
	���� ������������� ������� �������������� � %s

STATS_EXPORT_TLD
	TLD-���� (������� ������� ������) ������� �������������� � %s

STATS_EXPORT_SERVERS
	���� �������� ������� �������������� � %s

STATS_EXPORT_STATS
	����� ���������� ������� �������������� � %s

STATS_EXPORT_ALL
	��� ���������� ������� �������������� � %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	������������ %s �� ������ � ���� ������ ����������.

STATS_RENAMEUSER_EXISTING
	������������ %s ��� ���������� � ���� ������ ����������.

STATS_HELP_CHANNEL_1
	�� ������ �������� ��������� fantsasy-�������:

STATS_HELP_CHANNEL_2
	%stop10  :   ������ 10-�� ����� ����������� ������������� ������

STATS_HELP_CHANNEL_3
	%stop    :   ������ 3-�� ����� ����������� ������������� ������

STATS_HELP_CHANNEL_4
	%sgtop10 :   ������ 10-�� ����� ����������� ������������� ���� ����

STATS_HELP_CHANNEL_5
	%sgtop   :   ������ 3-�� ����� ����������� ������������� ���� ����

STATS_HELP_CHANNEL_6
	%sstats  :   ���� ������� ��������� ���������� ������������ ����� ������

STATS_HELP_CHANNEL_7
	%sgstats :   ���� ���������� ���������� � ����

STATS_HELP_CHANNEL_8
	%speak   :   ��� ��������� ������ (���������� �������)

STATS_TOP10_CHANNEL
	������ 10-�� ��� %s

STATS_TOP_CHANNEL
	������ 3-�� ��� %s

STATS_TOP10_NETWORK
	������ 10-�� ���� ����

STATS_TOP_NETWORK
	������ 3-�� ���� ����

STATS_USER_NETWORK
	����������� ���������� ��� %s

STATS_USER_CHANNEL
	��������� ���������� ��� %s �� %s

STATS_MESSAGE
	%2i %-16s ����: %s, ����: %s, �����: %s, �������: %s, ��������: %s

STATS_MESSAGE_ONE
	����: %s, ����: %s, �����: %s, �������: %s, ��������: %s

STATS_CHANNEL_PEAK
	������������ ���������� ������������� ��������� %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	�������������� ���������� � �������� ����������:
STAT_ENVINFO_VERSION
	������ ��������:            %s
STAT_ENVINFO_PROTOCOL
	IRCd-��������:              %s
STAT_ENVINFO_UPLINK
	Uplink-������:              %s
STAT_ENVINFO_SQL
	SQL-�����:                  %s
STAT_ENVINFO_HTML
	HTML-�����:                 %s
STAT_ENVINFO_DEBUG
	����� �������:              %s
STAT_ENVINFO_MODULESLOADED
	���-�� ����������� �������: %d
STAT_ENVINFO_LANGUAGE
	���� ����:                  %s
STAT_ENVINFO_FOOTER
	����� ������ �������������� ����������.

STATS_HTML_EXPORTED
	HTML-���� �� ��������������� ������� ������� ������������.

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
	<meta http-equiv="Content-Language" content="Russian">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>���������� ����:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>������� ���-�� �������.: </td> <td> %ld </td> <td>������������ ���-�� �������.: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>������� ���-�� �������.: </td> <td> %ld </td> <td>������������ ���-�� �������.: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>������� ���-�� IRC-����: </td> <td> %ld </td> <td>������������ ���-�� IRC-����: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>������� ���-�� ��������.: </td> <td> %ld </td> <td>������������ ���-�� ��������: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">������������� � away: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	���������� ��� ���� %s

HTML_TLD_TITLE
	������� Top-������ ������� ������� ������

HTML_TLD_HEADER
	<table><tr><th>��� ������</th><th>������</th><th>���-��</th><th>�����</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>��� �������</th></tr>

HTML_SERVER_CONTENT
	<tr><td>������: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>������: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) ��������, ��� ������ �� ������ ������ � ����<br>(�������� �������� �� ����� ������� ��� ����� ��������� ����������)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>������� ���������� ����:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(��� ������� ���������� � ������� ����������)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">������������� �� �������: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">������� �� �������:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">IRC-���� �� �������: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">�������� �� �������: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>������</th><th align=right>���-��</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>������:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>������� ���-�� ���������.:</td><td>%ld (%ld)</td><td>����. %ld �� [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>������� ���-�� IRC-����:</td><td>%ld (%ld)</td><td>����. %ld �� [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>���� �������</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>����� ���-�� ���������.:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>����� ������� ����</td><td>%ld</td><td>�� [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>���-�� ������� �������</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>��������� ��� ��� � ����:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>���-�� Kill'�� �� IRC-����</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>���-�� Kill'�� �� �������</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>�����</th><th align=right>�����������</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>�����</th><th align=right>����� �������</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) ��������� �� ��, ��� ����� ����������� � ��� ������� �� ������ ��������� ������

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> "�������������" ������� (������ �� �����)

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>�����</th><th>����� �����</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) ��������� �� ��, ��� ����� ����������� � ��� ������� �� ������ ��������� ������

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>�����</th><th>����� �������</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) ��������� �� ��, ��� ����� ���������� �� ������ ������

HTML_TOP10_TOPIC_TITLE
	������ 10-�� ������� � ������ ������ ������

HTML_CUR_NETSTATS_TITLE
	������� ���������� ����

HTML_DAILY_TITLE
	������� ���������� ����

HTML_TOP10_ONLINE_CHANS_TITLE
	������ 10-�� �������� ������� ����

HTML_SERVERLIST_TITLE
	������ ��������

HTML_TOP10_CHANEVER_TITLE
	������ 10-�� ���� ������� � �����

HTML_SERVERDETAILS_TITLE
	�������� � ��������

HTML_TOP10_CLIENT_TITLE
	���������� �� ������� IRC-��������

HTML_NETMAP_TITLE
	����� ����

HTML_VERSION
	���������� ������������� ��������� <a href="http://www.denorastats.org/">Denora</a> ������ %s %s

HTML_MOST_UNWELCOME_TITLE
	����� "�������������" ������

HTML_POPULAR_TITLE
	���������� ������

HTML_MOST_TOPIC_TITLE
	���������� �� �������

HTML_NETSTATS_TITLE
	������� ����������

NOW_STATS_ADMIN
	%s (%s) ����������������� ��� ������������� ����������.

STATS_AUTH_FAILED
	��������� ������� ������������� %s �� �������� ����������.

STATS_ADMIN_LOGOUT
	%s ������������������� �� �������� ����������.

SHUTDOWN_SAVING_DB
	���������� ���� ������ ����� �����������, ���������� ���������.

CANNOT_OPEN_LOG
	��������: ���������� ������� ���-����: %s

GLOBAL_LOAD_MOD
	%s �������� ������ %s

GLOBAL_UNLOAD_MOD
	%s �������� ������ %s

GLOBAL_FATAL_ERROR
	FATAL ERROR! %s: %s

CONFIG_PORT_ERROR
	����� ����� ������ ���� � ��������� ����� 1025..65535

CONFIG_HOSTNAME_ERROR
	�� ����� ������� ��� ����������� ����� hostname � ����� connect.

CONFIG_PROTOCOL_ERROR
	�� ����� ������ ��� ����������� ������ IRCD-��������� � ����� connect.

CONFIG_PORT_ERROR_NOTDEF
	�� ����� ������ ��� ����������� ����� ����� � ����� connect.

CONFIG_PASSWD_ERROR
	�� ����� ������� ��� ����������� ����� passwd � ����� connect.

CONFIG_PING_TO_LOW
	����� pingfreq ������ ���� ����� 2-�� ��� ����� �������.

CONFIG_SQLPING_TO_LOW
	����� sqlfreq ������ ���� ����� 2-�� ��� ����� �������.

CONFIG_UPTIME_TO_LOW
	����� uptimefreq ������ ���� ����� 2-�� ��� ����� �������.

CONFIG_DELAYMOD_T0MANY
	������� ������� ���-�� delayed-������� (����. 128)

CONFIG_TABLE_USER_ERROR
	�� ����� ������� ��� ����������� ����� users � ����� tables.

CONFIG_TABLE_CHANBANS_ERROR
	�� ����� ������� ��� ����������� ����� chanbans � ����� tables.

CONFIG_TABLE_CHANEXCEPT_ERROR
	�� ����� ������� ��� ����������� ����� chanexcept � ����� tables.

CONFIG_TABLE_ISON_ERROR
	�� ����� ������� ��� ����������� ����� ison � ����� tables.

CONFIG_TABLE_SERVER_ERROR
	�� ����� ������� ��� ����������� ����� server � ����� tables.

CONFIG_TABLE_GLINE_ERROR
	�� ����� ������� ��� ����������� ����� gline � ����� tables.

CONFIG_INVALID_LANG
	������������ ����� �����

CONFIG_ID_HOST_ERROR
	�� ����� ������� ��� ����������� ����� user(@host) � ����� identify.

CONFIG_ID_LANG_ERROR
	�� ����� ������� ��� ����������� ����� language � ����� identify.

CONFIG_ID_USER_ERROR
	�� ����� ������� ��� ����������� ����� user � ����� identify.

CONFIG_ID_DESC_ERROR
	�� ����� ������� ��� ����������� ����� desc � ����� identify.

CONFIG_ID_NAME_ERROR
	�� ����� ������� ��� ����������� ����� name � ����� identify.

CONFIG_ERROR_POSTIVE_VALUE
	��������� ������������� �������� ���������.

CONFIG_SQL_USER_ERROR
	�� ����� ������� ��� ����������� ����� user � ����� sql.

CONFIG_SQL_NAME_ERROR
	�� ����� ������� ��� ����������� ����� name � ����� sql.

CONFIG_ADMIN_NAME_ERROR
	�� ����� ������� ��� ����������� ����� name � ����� admin.

CONFIG_ADMIN_HOST_ERROR
	�� ����� ������� ��� ����������� ����� hostname � ����� admin.

CONFIG_ADMIN_PASS_ERROR
	�� ����� ������� ��� ����������� ����� passwd � ����� admin.

CONFIG_TIMEOUT_PING_ERROR
	�� ����� ������� ��� ����������� ����� pingfreq � ����� timeout.

CONFIG_TIMEOUT_SQL_ERROR
	�� ����� ������� ��� ����������� ����� sqlfreq � ����� timeout.

CONFIG_TIMEOUT_UPTIME_ERROR
	�� ����� ������� ��� ����������� ����� uptimefreq � ����� timeout.

CONFIG_TIMEOUT_HTML_ERROR
	�� ����� ������� ��� ����������� ����� htmlupdate � ����� timeout.

CONFIG_TIMEOUT_UPDATE_ERROR
	�� ����� ������� ��� ����������� ����� update � ����� timeout.

CONFIG_TIMEOUT_WARNING_ERROR
	�� ����� ������� ��� ����������� ����� warning � ����� timeout.

CONFIG_TIMEOUT_READ_ERROR
	�� ����� ������� ��� ����������� ����� read � ����� timeout.

CONFIG_INVALID_TIME
	������� �� ������ �������� �� �������.

CONFIG_HTML_UPDATE_TIME
	����� htmlupdate ������ ���� ����� 2-�� ��� ����� �������.

CONFIG_NETINFO_NAME_ERROR
	�� ����� ������� ��� ����������� ����� name � ����� netinfo.

CONFIG_FILENAME_PID_ERROR
	�� ����� ������� ��� ����������� ����� pid � ����� filenames.

CONFIG_FILENAME_MOTD_ERROR
	�� ����� ������� ��� ����������� ����� motd � ����� filenames.

CONFIG_FILENAME_CHANDB_ERROR
	�� ����� ������� ��� ����������� ����� channeldb � ����� filenames.

CONFIG_FILENAME_CTCP_ERROR
	�� ����� ������� ��� ����������� ����� ctcpdb � ����� filenames.

CONFIG_FILENAME_SERVER_ERROR
	�� ����� ������� ��� ����������� ����� serverdb � ����� filenames.

CONFIG_FILENAME_CHANSTATS_ERROR
	�� ����� ������� ��� ����������� ����� chanstatsdb � ����� filenames.

CONFIG_FILENAME_TLD_ERROR
	�� ����� ������� ��� ����������� ����� tlddb � ����� filenames.

CONFIG_FILENAME_EXCLUDE_ERROR
	�� ����� ������� ��� ����������� ����� excludedb � ����� filenames.

CONFIG_SS_DESC_ERROR
	�� ����� ������� ��� ����������� ����� real � ����� statserv.

CONFIG_SS_NAME_ERROR
	�� ����� ������� ��� ����������� ����� nick � ����� statserv.

CONFIG_SS_TOLONG
	������ ������� ������� ���.

CONFIG_SS_ALIASDESC_ERROR
	�� ����� ������� ��� ����������� ����� aliasreal � ����� statserv.

ALOG_FAILED_SQL_CONNECT
	Failed to connect to database : %s

ALOG_SQL_ENABLED
	%s: has been enabled.

ALOG_SQL_DISABLED
	MySQL: has been disabled.

ALOG_SQL_NOTSET
	MySQL Error: Set all required configuration options.

ALOG_HTML_ERR_WRITE
	Error: unable to access file for writing: %s

ALOG_DB_ERROR
	WARNING! DB_READ_ERROR in %s

ALOG_DEBUG_DB_OK
	debug: %s read successfully

ALOG_DEBUG_UNKNOW_CMODE
	debug: unknown channel mode %c

ALOG_DEBUG_LAST_LINE
	debug: last line is %s

ALOG_DEBUG_REPORT_AT
	debug: report these to the bugtracker at http://bugs.anope.org

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Channel: received join of non-existing user %s on channel ID %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Changing modes for %s to %s

ALOG_DEBUG_BAD_CHAN_MODE
	Debug: Malformed mode detected on %s.

ALOG_DEBUG_MODE_NO_PARAM
	channel: mode %c%c with no parameter (?) for channel %s

ALOG_DEBUG_FINDCHAN_BAD
	Error: findchan() called with NULL values

ALOG_DEBUG_FINDSTATCHAN_BAD
	Error: findstatschan() called with NULL values

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: SJOIN for nonexistent user %s on %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: JOIN for nonexistent user %s on %s

ALOG_DEBUG_JOIN
	debug: %s joins %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: KICK for nonexistent user %s on %s: %s

ALOG_DEBUG_KICKED
	debug: kicking %s from %s

ALOG_DEBUG_PART_NONEXISTANT
	user: PART from nonexistent user %s: %s

ALOG_DEBUG_PART
	debug: %s leaves %s

ALOG_DEBUG_PART_BUG
	user: BUG parting %s: channel entry found but c->chan NULL

ALOG_NON_TS_MODE
	TSMODE enabled but MODE has no valid TS

ALOG_DEBUG_NO_FIND_CHAN
	debug: can't find chan %s

ALOG_DEBUG_TOPIC_TS
	debug: encoded TOPIC TS %s converted to %d

ALOG_TOPIC_NONEXISTANT
	channel: TOPIC %s for nonexistent channel %s

ALOG_ADD_BAN_BAD
	debug: add_ban called with NULL values

ALOG_ADD_BAN_ADDED
	debug: Added ban %s to channel %s

ALOG_ADD_EXCEPTION_ADDED
	debug: Added except %s to channel %s

ALOG_ADD_INVITE_ADDED
	debug: Added invite %s to channel %s

ALOG_CREATE_CHAN
	debug: Creating channel %s

ALOG_DELETE_CHAN
	debug: Deleting channel %s

ALOG_BAN_FREE_ERROR
	channel: BUG freeing %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	channel: BUG freeing %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	debug: Deleted ban %s from channel %s

ALOG_EXCEPTION_DEL_CHAN
	debug: Deleted except %s from channel %s

ALOG_INVITE_DEL_CHAN
	debug: Deleted invite %s from channel %s

ALOG_DEBUG_FLOOD_MODE
	debug: Flood mode for channel %s set to %s

ALOG_NO_FLOOD
	no flood settings

ALOG_KEY_SET_TO
	debug: Key of channel %s set to %s

ALOG_NO_KEY
	no key

ALOG_LIMIT_SET_TO
	debug: Limit of channel %s set to %u

ALOG_REDIRECT_SET_TO
	debug: Redirect of channel %s set to %s

ALOG_NO_REDIRECT
	no redirect

ALOG_DEBUG_BAD_FINDSGROUP
	Error: findsgroup() called with NULL values

ALOG_LARGENET_WARNING_1
	You have selected to run Denora with the least amount of data

ALOG_LARGENET_WARNING_2
	stored in SQL, you should only use this option if you are on a

ALOG_LARGENET_WARNING_3
	very large network of 100,000+ users or have a remote sql database

CONFIG_ERROR_BASE
	Conf Error:  %s near line %d of %s

CONFIG_UNKNOWN_FILE
	unknown

CONFIG_ERROR_OPENING
	Unable to open config file %s

ALOG_DEBUG_RESOLVED_HOST
	debug: Resolved %s to %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() called with NULL-arg

ALOG_RESET_DAILY
	resetting daily stats

ALOG_RESETTING_WEEKLY
	resetting weekly stats

ALOG_RESETTING_MONTHLY
	resetting monthly stats

ALOG_TRY_TO_DISPLAY
	debug: trying to display message %s

ALOG_DONE_DISPLAY
	debug: done displaying message %s

ALOG_UKNOWN_USER_MODE
	debug: unknown user mode %c (in %s)

ALOG_SWHOIS_ERROR
	SWHOIS set for nonexistent user %s.

ALOG_VHOST_FOR
	debug: %s changes its vhost to %s

ALOG_REALNAME_FOR
	debug: %s changes its realname to %s

ALOG_USERNAME_FOR
	debug: %s changes its username to %s

ALOG_UMODE_NONEXISTANT
	user: MODE %s for nonexistent nick %s: %s

ALOG_UNKNWON_MSG
	debug: unknown message from server (%s)

ALOG_TRYING_LOAD_MOD
	trying to load [%s]

ALOG_STATUS
	status: [%d]

ALOG_TRYING_UNLOAD_MOD
	trying to unload [%s]

RESTARTING
	Restarting

RESTART_FAILED
	Restart failed

SAVING_FFF
	debug: Saving FFF information to disk

ALOG_SIGPIPE_WARNING
	Denora will execute a backtrace, and continue on, however Denora may become unstable

ALOG_NO_STATS_BIN
	Restart attempt failed--STATS_BIN not defined (rerun configure)

ALOG_ERR_REHASH_NO_MEM
	Error during the reload of the configuration file, but out of memory!

ALOG_ERR_REHASH
	Error Reading Configuration File (Received SIGHUP)

BACKTRACE_LINES
	Backtrace: report the following lines

BACKTRACE_NOT_HERE
	Backtrace: not available on this platform - use GDB

ALOG_DEBUG_SERVINTRO
	debug: Server introduced (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: Server introduced (%s) from %s

ALOG_DEBUG_PINGTIME
	debug: Ping Time from %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Error: findserver() called with NULL values

ALOG_ERR_FINDSERVERSTATS
	Error: findserverstats() called with NULL values

ALOG_ERR_FINDSERVERSUID
	Error: findserver_suid() called with NULL values

ALOG_ERR_DEL_SERVSTATS
	Error: delete_serverstats() called with NULL values

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() called for %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() completed

ALOG_ERR_DEL_SERVER
	Error: delete_server() called with NULL values

ALOG_DEL_SERVER_FOR
	debug: delete_server() called for %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() cleared all users for NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() completed

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version received unknown version format

ALOG_MOD_TRY_LOAD
	trying to load [%s]

ALOG_MOD_STATUS
	status: [%d][%s]

ALOG_MOD_ERROR
	Module error: %s

ALOG_MOD_BE_ONLY_ONE
	You cannot have 2 protocol modules loaded.

ALOG_MOD_UNLOAD_SELF
	debug: %s requested unload...

ALOG_CMD_DISPLAY_START
	debug: trying to display command %s

ALOG_CMD_DISPLAY_END
	debug: done displaying command %s

ALOG_CMD_DISPLAY_LIST
	debug: Displaying command list for %s

ALOG_MSG_DISPLAY_START
	debug: trying to display message %s

ALOG_MSG_DISPLAY_END
	debug: done displaying message %s

ALOG_MSG_DISPLAY_LIST
	debug: Displaying message list for %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: Added module CallBack: [%s] due to execute at %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST for nonexistent user %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME for nonexistent user %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME for nonexistent user %s

STAT_ADMIN_ALREADY
	������������ %s ��� ������������ � ������ ��������������� ����������.

STAT_ADMIN_CREATED
	������������ %s �������� � ������ ��������������� ����������.

STAT_ADMIN_DELETED
	������������ %s ������ �� ������ ��������������� ����������.

STAT_ADMIN_NOTADMIN
	������������ %s �� ���������� � ������ ��������������� ����������.

STAT_ADMIN_SHOW
	���: %s ���������: %s ����� �����: %d

STAT_HELP_ADMIN
	��������� ������� ��� ��������� ����� ��������������� ����������.
	������ ������ �������� � �������������� �������� �� ������
	��������������� � ���������������� ����� ��������.
	
	ADMIN ADD ��� ������ [���������] [�����_�����]
	ADMIN DEL ���
	ADMIN SETPASS ��� �����_������
	ADMIN SHOW ���
	ADMIN LIST
	
	ADMIN ADD - ��������� �������� ������ �������������� ����������
	
	ADMIN DEL - ������� ���������� �������, ������� ������������� ��������������.
	
	ADMIN SETPASS - ��������� �������� ������ ���������� ��������������.
	
	ADMIN SHOW - ���������� ��������� ���������� �� �������� ��������������.
	
	ADMIN LIST - ������� ������ ���� ������������ ��������������� ����������.
	
	������ ������� �������� ������ ��������������� ����������

STAT_ADMIN_SYNTAX
	ADMIN {ADD|DEL|SETPASS|SHOW|LIST} [���|������|���������|�����_�����]

STAT_ADMIN_DELETE_CONFIG
	������������� %s �� ����� ���� ������ �� ������,
	��� ��� �� �������� � ���������������� ����� ��������.

STAT_CHGPASS_OK
	������ ��� �������� %s ������� �������.

STAT_CHGPASS_CONFIG
	�� �� ������ �������� ������ ��� ���������������,
	����������� � ���������������� ����� ��������.

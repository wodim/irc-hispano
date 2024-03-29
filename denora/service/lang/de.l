# German language file.
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
# $Id: de.l 386 2011-01-04 17:16:43Z hal9000 $
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
	Deutsch (German)

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
	%d.%b.%Y, %H:%M:%S %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a, %d.%B.%Y
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%d.%b.%Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	So
	Mo
	Di
	Mi
	Do
	Fr
	Sa
# %A
STRFTIME_DAYS_LONG
	Sonntag
	Montag
	Dienstag
	Mittwoch
	Donnerstag
	Freitag
	Samstag
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Feb
	M�r
	Apr
	Mai
	Jun
	Jul
	Aug
	Sep
	Okt
	Nov
	Dez
# %B
STRFTIME_MONTHS_LONG
	Januar
	Februar
	M�rz
	April
	Mai
	Juni
	Juli
	August
	September
	Oktober
	November
	Dezember

# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Interner Fehler - kann die Anfrage nicht verarbeiten.
UNKNOWN_COMMAND
	Unbekannter Befehl %s.
UNKNOWN_COMMAND_HELP
	Unbekannter Befehl %s.  "/msg %s HELP" f�r Hilfe.
SYNTAX_ERROR
	Syntax: %s
MORE_INFO
	/msg %s HELP %s f�r weitere Informationen.
NO_HELP_AVAILABLE
	Keine Hilfe verf�gbar f�r %s.

SERVICE_OFFLINE
	%s ist derzeit nicht verf�gbar, bitte versuche es sp�ter noch einmal.

SQL_DISABLED
	Du kannst diesen Befehl nicht benutzen, SQL ist ausgeschaltet.

STATS_UPTIME_DHMS
	Die Stats laufen seit %d Tagen, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	Die Stats laufen seit %d Tag, %02d:%02d:%02d
STATS_UPTIME_HMS
	Die Stats laufen seit %d Stunden, %d Minuten, %d Sekunden
STATS_UPTIME_HM1S
	Die Stats laufen seit %d Stunden, %d Minuten, %d Sekunden
STATS_UPTIME_H1MS
	Die Stats laufen seit %d Stunden, %d Minuten, %d Sekunden
STATS_UPTIME_H1M1S
	Die Stats laufen seit %d Stunden, %d Minuten, %d Sekunde
STATS_UPTIME_1HMS
	Die Stats laufen seit %d Stunde, %d Minuten, %d Sekunde
STATS_UPTIME_1HM1S
	Die Stats laufen seit %d Stunde, %d Minuten, %d Sekunde
STATS_UPTIME_1H1MS
	Die Stats laufen seit %d Stunde, %d Minute, %d Sekunden
STATS_UPTIME_1H1M1S
	Die Stats laufen seit %d Stunde, %d Minute, %d Sekunde
STATS_UPTIME_MS
	Die Stats laufen seit %d Minuten, %d Sekunden
STATS_UPTIME_M1S
	Die Stats laufen seit %d Minuten, %d Sekunde
STATS_UPTIME_1MS
	Die Stats laufen seit %d Minute, %d Sekunden
STATS_UPTIME_1M1S
	Die Stats laufen seit %d Minute, %d Sekunde

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stas admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Zugriff verweigert.
PERMISSION_DENIED
	Berechtigung nicht ausreichend.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Ja
SAY_NO
	Nein

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN wurde nicht definiert. Neustart nicht m�glich. Installiere Denora erneut mit aktiviertem RESTART-Befehl.

# Module strings

STAT_MODULE_LOADED
	Modul %s geladen
STAT_MODULE_UNLOADED
	Modul %s entladen
STAT_MODULE_LOAD_FAIL
	Kann Modul %s nicht laden
STAT_MODULE_REMOVE_FAIL
	Kann Modul %s nicht entladen
STAT_MODULE_LOAD_SYNTAX
	MODLOAD Modul-Name
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD Modul-Name
STAT_MODULE_NO_UNLOAD
	Modul KANN NICHT entfernen werden.
STAT_MODULE_LIST_HEADER
	Aktuelle Module-Liste:
STAT_MODULE_LIST
	Module: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d Module geladen.
STAT_MODULE_INFO_LIST
	Module: %s Version: %s Autor: %s geladen: %s
STAT_MODULE_CMD_LIST
	Unterst�tzte Befehle: /msg %s %s
STAT_MODULE_MSG_LIST
	Unterst�tzte IRCD's f�r: %s
STAT_MODULE_NO_LIST
	Derzeit keine Module geladen
STAT_MODULE_NO_INFO
	Es sind keine Informationen �ber das Modul %s verf�gbar
STAT_MODULE_INFO_SYNTAX
	MODINFO Modul-Name
MODULE_HELP_HEADER
	Die folgenden Befehle wurden durch ein Modul geladen:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	%S Befehle:
	    STATUS       Zeigt eine �bersicht des Netzwerkes.
	    LOGIN       Macht dich zum Stats Administrator.
	    LOGOUT      Meldet dich von deinem Administrator-Konto ab.

STAT_HELP_STATUS
	Syntax: STATUS
	
	Zeigt die Anzahl der Server, Channels und Benutzer, die 
	gegenw�rtig zum Netzwerk verbunden sind.

STAT_HELP_SHUTDOWN
	Syntax: SHUTDOWN
	
	Denora wird mit diesem Befehl beendet und die
	Datenbanken werden gespeichert.
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_RESTART
	Syntax: RESTART
	
	Denora wird mit diesem Befehl restartet.
	(beendet und sofort wieder gestartet).
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_RELOAD
	Syntax: RELOAD
	
	Veranla�t Denora die Konfigurationsdatei neu einzulesen.
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_ADMIN_CMD
	 Befehle die nur f�r Stats Administartoren Verf�gbar sind:
	    ADMIN       Erstellt/L�scht Stats Admins
	    CHANSTATS   �berwacht Channels f�r verschiedene Statistiken
	    ENVINFO     Zeigt Informationen zur Umgebung von Denora
	    EXCLUDE     Schlie� Users von Channelstats aus
	    EXPORT      Exportiert die Daten zu XML
	    HTML        Erstellt die HTML Datei
	    MODLOAD     L�dt ein Modul
	    MODUNLOAD   Entl�dt ein Modul
	    MODLIST     Listet geladenen Module auf
	    MODINFO     Info �ber ein geladenes Modul
	    RESTART     Veranla�t Denora zu restarten
	    RELOAD      Veranla�t Denora die Konfiguration erneut zu laden
	    SET         �ndert verschiedene Runtime Einstellungen
	    SHUTDOWN    Beendet Denora ( die Datenbanken werden gespeichert )

STAT_HELP_MODLOAD
	Syntax: MODLOAD Modul-Name

	Dieser Befehl l�dt das benannte Modul aus dem
	Module - Verzeichnis.

	Beschr�nkt auf Stats Administratoren.

STAT_HELP_MODUNLOAD
	Syntax: MODUNLOAD Modul-Name

	Dieser Befehl entl�dt das benannte Modul aus dem
	Module - Verzeichnis.
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_MODINFO
	Syntax: MODINFO Modul-Name

	Dieser Befehl listet Informationen �ber das benannte Modul auf

	Beschr�nkt auf Stats Administratoren.

STAT_HELP_MODLIST
	Syntax: MODLIST

	Listet alle derzeit geladenen Module auf.

	Beschr�nkt auf Stats Administratoren.

STAT_HELP_EXCLUDE
	Syntax: EXCLUDE {ADD|DEL|LIST} Ziel
	
	Schlie�t das angegebenen Ziel von Stats aus
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_EXPORT
	Syntax: EXPORT was
	
	Exportiert Daten zu XML. Die Option was
	kann nur : channels, users, tld, servers,
	stats oder all sein.
	
	Beschr�nkt auf Stats Administratoren.

STAT_HELP_HTML
	Syntax: HTML
	
	Erzwingt die Erstellung der HTML Datei.


STAT_HELP_USERDEBUG
	Syntax: USERDEBUG User 
	
	Zeigt Debug Informationen �ber den
	angegebenen Benutzer an.

	Beschr�nkt auf Stats Administratoren.

STAT_HELP_LOGIN
	Syntax: LOGIN Name Passwort
	
	Macht Dich zum Stats Administrator.

STAT_HELP_LOGOUT
	Syntax: LOGOUT
	
	Damit kannst Du Dich aus Deinem Administrator-Konto abmelden.

STAT_HELP_SET
	Syntax: SET OPTION [ON|OFF]

	�ndert verschiedene Laufzeiteinstellungen.

	M�gliche Optionen sind:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Beschr�nkt auf Stats Administratoren.

STAT_HELP_ENVINFO
	Syntax: ENVINFO
	
	Zeigt Informationen zur Umgebung von Denora
	
	Beschr�nkt auf Stats Administratoren.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Du bist jetzt ein Stats Administrator.

STAT_ADMIN_LOGGED_OUT
	Du bist jetzt aus deinem Administrator-Konto abgemeldet.

STAT_ADMIN_ALREADY_LOGGED
	Du bist bereits ein Stats Administrator!

STAT_LOGIN_SYNTAX
	LOGIN Name Passwort

STAT_RELOAD
	Die Stats Konfigurationsdatei wurde neu eingelesen.

###########################################################################
#
# STATUS messages
#
###########################################################################

STAT_STATUS_SERVER_COUNT
	Server Statistiken f�r %s:
	Die aktuelle Anzahl verbundener Server: %d
	Die maximale Anzahl verbundener Server: %d

STAT_STATUS_CHAN_COUNT
	Channel Statistiken f�r %s:
	Die aktuelle Anzahl benutzter Channels: %d
	Die maximale Anzahl benutzter Channels: %d

STAT_STATUS_USER_COUNT
	User Statistiken f�r %s
	Die aktuelle Anzahl verbundener User: %d
	Die maximale Anzahl verbundener User: %d

STAT_STATUS_SYNTAX
	STATUS [SERVERS|CHANNELS|USERS]

STAT_STATUS_PAGE
	F�r mehr Stats: %s

# SET responses
STAT_SET_SYNTAX
	SET Option Einstellung
STAT_SET_DEBUG_ON
	Die Stats sind jetzt im Debug-Modus.
STAT_SET_DEBUG_OFF
	Die Stats sind jetzt im Non-Debug-Modus.
STAT_SET_DEBUG_LEVEL
	Die Stats sind jetzt im Debug-Modus (Level %d).
STAT_SET_DEBUG_ERROR
	Die Einstellung f�r DEBUG k�nnen nur ON, OFF, oder eine positive Zahl sein.
STAT_SET_UNKNOWN_OPTION
	Unbekannter SET Option %s.
STAT_SET_SQL_ON
	SQL wird jetzt benutzt.
STAT_SET_SQL_OFF
	SQL wird jetzt NICHT mehr benutzt.
STAT_SET_SQL_ERROR
	Einstellungen f�r SQL k�nnen nur ON oder OFF sein.
STAT_SET_SQL_ERROR_INIT
	SQL FEHLER, schaue bitte in Deinen Logs nach um zu sehen wie du es berichtigen kannst.
STAT_SET_SQL_ERROR_DISABLED
	SQL ist nicht konfiguriert. Um SQL zu benutzen
	mu�t du es in deiner denora.conf richtig konfiguriert haben.
STAT_SET_LIST_OPTION_ON
	%s ist eingeschaltet
STAT_SET_LIST_OPTION_OFF
	%s ist ausgeschaltet
STAT_SET_HTML_ON
	HTML wird benutzt.
STAT_SET_HTML_OFF
	HTML wird NICHT mehr benutzt.
STAT_SET_HTML_ERROR
	Einstellungen f�r HTML k�nnen nur ON oder OFF sein.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Channel Stats erlaubt Channels zu �berwachen um verschiedene 
	Statistiken �ber die Benutzer zu erstellen.
	
	CHANSTATS ADD Channel
	CHANSTATS DEL Channel
	CHANSTATS LIST
	CHANSTATS SET Channel FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER User1 User2
	CHANSTATS RENAME User1 User2
	
	Der CHANSTATS ADD Befehl f�gt den angegebenen Channel zu der 
	Stats-Channels-Liste hinzu.
	
	Der CHANSTATS DEL Befehl l�scht den angegebenen Channel von der
	Stats-Channels-Liste.
	
	Der CHANSTATS LIST Befehl zeigt eine Liste der Channels die f�r
	Statistiken �berwacht werden.
	
	Das CHANSTATS SET Befehl aktiviert die Fantasy Optionen f�r den
	angegebenen Channel. Mit der Option ON wird StatServ private Nachrichten 
	in den Channel sende , mit der Option NOTICE , Noticen zu den Benutzer.
	
	Der CHANSTATS SUMUSER Befehl �bertr�gt alle Statistiken von Benutzer2 
	zu Benutzer1 und l�scht Benutzer2
	
	Der CHANSTATS RENAME Befehl benennt Benutzer1 in Benutzer2 um.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s wird jetzt f�r die Channel-Stats �berwacht.

STAT_CHANSTATS_CHAN_LARGENET
	Die LargeNet Option ist eingeschaltet, du kannst keine Channels hinzugef�gen

STAT_CHANSTATS_LOGCHAN
	Der LogChannel kann nicht zum Channel-Stats hinzugef�gt werden

STAT_CHANSTATS_NOT_VALID_CHAN
	Das ist ein invalider Channel-Name

STAT_CHANSTATS_CHAN_ALREADY
	%s wird bereits f�r die Channel-Stats �berwacht.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD Channel

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL Channel

STAT_CHANSTATS_CHAN_DEL
	L�sche %s von der Channel-Stats �berwachung.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s wurde in der Channel-Stats �berwachung nicht gefunden.

STATS_CHANSTATS_PART
	%s wird nicht mehr �berwacht.

STATS_CHANSTATS_NOCHANNELS
	Keine �berwachte Channels in der Liste

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET Channel FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Fantasy Befehle f�r %s sind jetzt eingeschaltet

STATS_CHANSTATS_FANTASY_OFF
	Fantasy Befehle f�r %s sind jetzt ausgeschaltet

STATS_CHANSTATS_FANTASY_NOTICE
	Fantasy wird in %s Notices an die Benutzer senden

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER Benutzer1 Benutzer2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME Benutzer1 Benutzer2

STATS_CHANSTATS_SUMUSER_DONE
	Die Daten von %s wurden zu %s hinzugef�gt

STATS_CHANSTATS_RENAMEUSER_DONE
	Der Benutzer %s wurde in %s umbenannt

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} Ziel

STAT_EXCLUDE_ADDED
	%s wird jetzt von Stats ausgeschlossen

STAT_EXCLUDE_ALREADY
	%s wird bereits von Stats ausgeschlossen

STAT_EXCLUDE_DELETED
	%s ist von der Exclude-Liste entfernt worden

STAT_EXCLUDE_NOT_FOUND
	%s ist nicht auf der Exclude-Liste

STAT_EXCLUDE_LIST_HEADER
	Exclude-Liste

# EXPORT responses
STATS_EXPORT_CHANNELS
	Exportiere die Channels zu %s

STATS_EXPORT_USERS
	Exportiere die Benutzer zu %s

STATS_EXPORT_TLD
	Exportiere 'Top Level Domain' zu %s

STATS_EXPORT_SERVERS
	Exportiere die Server zu %s

STATS_EXPORT_STATS
	Exportiere die Stats zu %s

STATS_EXPORT_ALL
	Exportiere alle Informationen zu %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s existiert nicht in der Stats-db.

STATS_RENAMEUSER_EXISTING
	Der Benutzer %s existiert bereits in der Stats-db.

STATS_HELP_CHANNEL_1
	Die folgenden Phantasiebefehle stehen zu Verf�gung

STATS_HELP_CHANNEL_2
	%stop10  :   Die top 10 User vom gegenw�rtigen Channel

STATS_HELP_CHANNEL_3
	%stop    :   Die top 3  User vom gegenw�rtigen Channel

STATS_HELP_CHANNEL_4
	%sgtop10 :   Die top 10 User vom ganzen Netzwerk

STATS_HELP_CHANNEL_5
	%sgtop   :   Die top 3  User vom ganzen Netzwerk

STATS_HELP_CHANNEL_6
	%sstats  :   Benutzer Statistiken vom Channel

STATS_HELP_CHANNEL_7
	%sgstats :   Benutzer Statistiken vom ganzen Netzwerk

STATS_HELP_CHANNEL_8
	%speak   :   Benutzer-Spitze vom aktuellen Channel

STATS_TOP10_CHANNEL
	Top 10 von %s

STATS_TOP_CHANNEL
	Top 3 von %s

STATS_TOP10_NETWORK
	Top 10 vom Netzwerk

STATS_TOP_NETWORK
	Top 3 vom Netzwerk

STATS_USER_NETWORK
	Netzwerkstats f�r %s

STATS_USER_CHANNEL
	Channelstats f�r %s in %s

STATS_MESSAGE
	%2i %-16s Buchstaben: %s, W�rter: %s, Zeilen: %s, Smileys: %s, Aktionen: %s

STATS_MESSAGE_ONE
	Buchstaben: %s, W�rter: %s, Zeilen: %s, Smileys: %s, Aktionen: %s

STATS_CHANNEL_PEAK
	Channel Benutzer-Spitze war %d

# ENVINFO responses
STAT_ENVINFO_HEADER
	Informationen zur Umgebung von Stats:
STAT_ENVINFO_VERSION
	Stats Version:            %s
STAT_ENVINFO_PROTOCOL
	IRCd Protokoll:            %s
STAT_ENVINFO_UPLINK
	Stats Uplink:             %s
STAT_ENVINFO_SQL
	SQL:                      %s
STAT_ENVINFO_HTML
	HTML:                     %s
STAT_ENVINFO_DEBUG
	Debug:                    %s
STAT_ENVINFO_MODULESLOADED
	Geladene Module:          %d
STAT_ENVINFO_LANGUAGE
	Sprache:                  %s
STAT_ENVINFO_FOOTER
	Ende der Umgebungs Informationen

STATS_HTML_EXPORTED
	Stats HTML wurde erstellt

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
	<meta http-equiv="Content-Language" content="German">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Netzwerk Statistiken:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>Aktuelle User: </td> <td> %ld </td> <td>Maximale Anzahl der Benutzer: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Aktuelle Channels: </td> <td> %ld </td> <td>Maximale Anzahl der Channels: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Aktuelle Opers: </td> <td> %ld </td> <td>Maximale Anzahl der Opers: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Aktuelle Server: </td> <td> %ld </td> <td>Maximale Anzahl der Server: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Benutzer die Away sind: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Netzwerk Statistiken f�r %s

HTML_TLD_TITLE
	Top Level Domain Statistiken

HTML_TLD_HEADER
	<table><tr><th>Landeskennzahl</th><th>Land</th><th>Anzahl</th><td>Gesamt</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Server Name</th></tr>

HTML_SERVER_CONTENT
	<tr><td>Server: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>Server: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) zeigt Server an die momentan online sind<br>(Klicken Sie ein ServerName f�r mehr Informationen)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>T�gliche Netzwerk Statistiken:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(Alle t�gliche Statistiken werden um Mitternacht zur�ckgestellt)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Maximale Ahzahl der t�glichen Benutzer: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Maximale Anzahl der t�glichen Channels:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Maximale Anzahl der t�glichen Opers: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Maximale Anzahl der t�glichen Server: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Version</th><th align=right>Anzahl</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>Server:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Aktuelle User:</td><td>%ld (%ld)</td><td>Maximum %ld am [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Aktuelle Opers:</td><td>%ld (%ld)</td><td>Maximum %ld am [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Aktueller Ping</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Gesamte Anzahl verbundener Benutzer:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>H�chster Ping</td><td>%ld</td><td>at [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Server Splits</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Letztens gesehen:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>IrcOp Kills</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Server Kills</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Channel</th><th align=right>Mitglieder</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Channel</th><th align=right>Total Joins</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) zeigt an das der Channel momentan aktiv ist

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Unbeliebte Channels

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Channel</th><th>Total Kicks</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) zeigt die momentan aktivsten Channels an

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Channel</th><th>Total Topics</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) zeigt an das der Channel momentan aktiv ist

HTML_TOP10_TOPIC_TITLE
	Top10 Topic �nderung Channels

HTML_CUR_NETSTATS_TITLE
	Aktuelle Netzwerk Statistiken

HTML_DAILY_TITLE
	T�gliche Netzwerk Statistiken

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 Online Channels

HTML_SERVERLIST_TITLE
	Server-Liste

HTML_TOP10_CHANEVER_TITLE
	Top10 Channels �berhaupt

HTML_SERVERDETAILS_TITLE
	Server Details

HTML_TOP10_CLIENT_TITLE
	Top10 Client Versionen

HTML_NETMAP_TITLE
	Netzwerk Map

HTML_VERSION
	Powered by <a href="http://www.denorastats.org/">Denora</a> version %s %s

HTML_MOST_UNWELCOME_TITLE
	Unbeliebte Channels

HTML_POPULAR_TITLE
	Popul�re Channels

HTML_MOST_TOPIC_TITLE
	Meiste Topics Channels

HTML_NETSTATS_TITLE
	Netzwerk Statistiken

NOW_STATS_ADMIN
	%s (%s) ist jetzt ein Stats Administrator.

STATS_AUTH_FAILED
	Authentifizierung f�r %s schlug fehl.

STATS_ADMIN_LOGOUT
	%s ist kein Stats Administrator mehr.

SHUTDOWN_SAVING_DB
	Aktualisiere die Datenbanken und schalte ab, bittte warten.

CANNOT_OPEN_LOG
	Warnung: Kann die LogDatei %s nicht �ffnen.

GLOBAL_LOAD_MOD
	%s hat das Modul %s geladen.

GLOBAL_UNLOAD_MOD
	%s hat das Modul %s entladen.

GLOBAL_FATAL_ERROR
	FATALER FEHLER! %s: %s

CONFIG_PORT_ERROR
	Die Portnummer muss zwischen 1025 und 65535 sein

CONFIG_HOSTNAME_ERROR
	Fehlerhafte hostname Definition in dem connect block

CONFIG_PROTOCOL_ERROR
	Fehlerhafte IRCDModule Definition (protocol) in dem connect block

CONFIG_PORT_ERROR_NOTDEF
	Fehlerhafte port Definition in dem connect block

CONFIG_PASSWD_ERROR
	Fehlerhafte passwd Definition in dem connect block

CONFIG_PING_TO_LOW
	Pingfrequenz mu� 2 Minuten oder gr�sser sein

CONFIG_SQLPING_TO_LOW
	SQL Pingfrequenz mu� 2 Minuten oder gr�sser sein

CONFIG_UPTIME_TO_LOW
	Uptime frequenz mu� 2 Minuten oder gr�sser sein

CONFIG_DELAYMOD_T0MANY
	Zu viele Module (max 128)

CONFIG_TABLE_USER_ERROR
	Fehlerhafte users Definition im tables block

CONFIG_TABLE_CHANBANS_ERROR
	Fehlerhafte chanbans Definition im tables block

CONFIG_TABLE_CHANEXCEPT_ERROR
	Fehlerhafte chanexcept Definition im tables block

CONFIG_TABLE_ISON_ERROR
	Fehlerhafte ison Definition im tables block

CONFIG_TABLE_SERVER_ERROR
	Fehlerhafte server Definition im tables block

CONFIG_TABLE_GLINE_ERROR
	Fehlerhafte gline Definition im tables block

CONFIG_INVALID_LANG
	Unzul�ssige Sprachen Nummer

CONFIG_ID_HOST_ERROR
	Fehlerhafte user (host) Definition im identity block

CONFIG_ID_LANG_ERROR
	Fehlerhafte language Definition im identity block

CONFIG_ID_USER_ERROR
	Fehlerhafte user Definition im identity block

CONFIG_ID_DESC_ERROR
	Fehlerhafte desc Definition im identity block

CONFIG_ID_NAME_ERROR
	Fehlerhafte name Definition im identity block

CONFIG_ERROR_POSTIVE_VALUE
	Ein positiver Wert wird erwartet

CONFIG_SQL_USER_ERROR
	Fehlerhafte user Definition im sql block

CONFIG_SQL_NAME_ERROR
	Fehlerhafte name Definition im sql block

CONFIG_ADMIN_NAME_ERROR
	Fehlerhafte name Definition im admin block

CONFIG_ADMIN_HOST_ERROR
	Fehlerhafte hostname Definition im admin block

CONFIG_ADMIN_PASS_ERROR
	Fehlerhafte passwd Definition im admin block

CONFIG_TIMEOUT_PING_ERROR
	Fehlerhafte pingfreq Definition im timeout block

CONFIG_TIMEOUT_SQL_ERROR
	Fehlerhafte sqlfreq Definition im timeout block

CONFIG_TIMEOUT_UPTIME_ERROR
	Fehlerhafte uptimefreq Definition im timeout block

CONFIG_TIMEOUT_HTML_ERROR
	Fehlerhafte html Definition im timeout block

CONFIG_TIMEOUT_UPDATE_ERROR
	Fehlerhafte update Definition im timeout block

CONFIG_TIMEOUT_WARNING_ERROR
	 Fehlerhafte warning Definition im timeout block

CONFIG_TIMEOUT_READ_ERROR
	 Fehlerhafte read Definition im timeout block

CONFIG_INVALID_TIME
	Das ist kein g�ltiger Zeit Wert

CONFIG_HTML_UPDATE_TIME
	HTML Time Out mu� 2 Minuten oder gr�sser sein

CONFIG_NETINFO_NAME_ERROR
	Fehlerhafte name Definition im netinfo block

CONFIG_FILENAME_PID_ERROR
	Fehlerhafte pid Definition im files block

CONFIG_FILENAME_MOTD_ERROR
	Fehlerhafte motd Definition im files block

CONFIG_FILENAME_CHANDB_ERROR
	Fehlerhafte ChannelDB Definition im files block

CONFIG_FILENAME_CTCP_ERROR
	Fehlerhafte CtcpDB Definition im files block

CONFIG_FILENAME_SERVER_ERROR
	Fehlerhafte ServerDB Definition im files block

CONFIG_FILENAME_CHANSTATS_ERROR
	Fehlerhafte ChannelStatsDB Definition im files block

CONFIG_FILENAME_TLD_ERROR
	 Fehlerhafte TldDB Definition im files block

CONFIG_FILENAME_EXCLUDE_ERROR
	Fehlerhafte ExcludeDB Definition im files block

CONFIG_SS_DESC_ERROR
	Fehlerhafte real Definition im statserv block

CONFIG_SS_NAME_ERROR
	Fehlerhafte nick Definition im statserv block

CONFIG_SS_TOLONG
	Nickname ist zu lang

CONFIG_SS_ALIASDESC_ERROR
	Fehlerhafte aliasreal Definition im statserv block

ALOG_FAILED_SQL_CONNECT
	Kann nicht zu der Datenbank %s verbinden

ALOG_SQL_ENABLED
	MySQL: wurde eingeschaltet.

ALOG_SQL_DISABLED
	MySQL: wurde ausgeschaltet.

ALOG_SQL_NOTSET
	MySQL Fehler: �berpr�fe bitte deine MySql Konfiguration.

ALOG_HTML_ERR_WRITE
	Fehler: Es ist nicht m�glich auf %s zu schreiben. Schreibgesch�tzt ??

ALOG_DB_ERROR
	Warnung! DB_READ_ERROR in %s

ALOG_DEBUG_DB_OK
	Debug: %s erfolgreich gelesen

ALOG_DEBUG_UNKNOW_CMODE
	Debug: Unbekannter Mode: Channel Mode %c

ALOG_DEBUG_LAST_LINE
	Debug: Die letzte Zeile ist %s

ALOG_DEBUG_REPORT_AT
	Debug: Bitte melde das im Bugtracker : http://bugs.anope.org

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Channel: Join vom nicht vorhandenen User %s  Channel ID %d

ALOG_DEBUG_CHANGE_MODE_CHAN
	Debug: �ndere Modes in %s zu %s

ALOG_DEBUG_BAD_CHAN_MODE
	Debug: Unbekannter Mode entdeckt in %s.

ALOG_DEBUG_MODE_NO_PARAM
	Channel: Mode %c%c ohne Parameter (?) f�r Channel %s

ALOG_DEBUG_FINDCHAN_BAD
	Fehler: findchan() aufgerufen mit Parameter NULL

ALOG_DEBUG_FINDSTATCHAN_BAD
	Fehler: findstatschan() aufgerufen mit Parameter NULL

ALOG_DEBUG_SJOIN_NONEXISTANT
	User: SJOIN f�r nicht vorhandenen Benutzer %s in %s

ALOG_DEBUG_JOIN_NONEXISTANT
	User: JOIN f�r nicht vorhandenen Benutzer %s in %s

ALOG_DEBUG_JOIN
	Debug: %s join %s

ALOG_DEBUG_KICK_NONEXISTANT
	User: KICK f�r nicht vorhandenen Benutzer %s in %s: %s

ALOG_DEBUG_KICKED
	Debug: kicke %s von %s

ALOG_DEBUG_PART_NONEXISTANT
	User: PART von nicht vorhandenem Benutzer %s: %s

ALOG_DEBUG_PART
	Debug: %s verl��t %s

ALOG_DEBUG_PART_BUG
	User: BUG parting %s: Channel gefunden aber c->chan ist NULL

ALOG_NON_TS_MODE
	TSMODE an aber MODE hat keinen g�ltigen TS

ALOG_DEBUG_NO_FIND_CHAN
	Debug: Kann Channel %s nicht finden

ALOG_DEBUG_TOPIC_TS
	Debug: Kodierter TOPIC TS %s umgewandelt zu %d

ALOG_TOPIC_NONEXISTANT
	Channel: TOPIC %s von nicht vorhandenem Channel %s

ALOG_ADD_BAN_BAD
	Debug: add_ban aufgerufen mit Parameter NULL

ALOG_ADD_BAN_ADDED
	Debug: Ban %s zu Channel %s addiert

ALOG_ADD_EXCEPTION_ADDED
	Debug: Except %s zu Channel %s addiert

ALOG_ADD_INVITE_ADDED
	Debug: Einladung %s zu Channel %s addiert

ALOG_CREATE_CHAN
	Debug: Erstelle Channel %s

ALOG_DELETE_CHAN
	Debug: L�sche Channel %s

ALOG_BAN_FREE_ERROR
	Channel: BUG freeing %s: bans[%d] ist NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] ist NULL!

ALOG_INVITE_FREE_ERROR
	Channel: BUG freeing %s: invite[%d] ist NULL!

ALOG_BAN_DEL_CHAN
	Debug: Ban %s von Channel %s gel�scht

ALOG_EXCEPTION_DEL_CHAN
	Debug: Except %s von Channel %s gel�scht

ALOG_INVITE_DEL_CHAN
	Debug: Einladung %s von Channel %s gel�scht

ALOG_DEBUG_FLOOD_MODE
	Debug: Flood-Mode von Channel %s gesetzt zu %s

ALOG_NO_FLOOD
	Keine flood Einstellungen

ALOG_KEY_SET_TO
	Debug: Schl�ssel von Channel %s gesetzt zu %s

ALOG_NO_KEY
	Kein Schl�ssel

ALOG_LIMIT_SET_TO
	Debug: Limit von Channel %s gesetzt zu %u

ALOG_REDIRECT_SET_TO
	debug: Umleitung von Channel %s gesetzt zu %s

ALOG_NO_REDIRECT
	Kein Redirect

ALOG_DEBUG_BAD_FINDSGROUP
	Fehler: findsgroup() bennant mit NULL Wert

ALOG_LARGENET_WARNING_1
	Sie habe sich entschieden Denora mit der mimimale Menge an SQL Daten

ALOG_LARGENET_WARNING_2
	laufen zu lassen. Die Option sollte NUR f�r ein grosses Netzwerk mit �ber

ALOG_LARGENET_WARNING_3
	100,000+ Usern oder mit einer "remote SQL Datenbank" benutzt werden.

CONFIG_ERROR_BASE
	Conf Fehler:  %s nahe Zeile %d von %s

CONFIG_UNKNOWN_FILE
	unbekannt

CONFIG_ERROR_OPENING
	Kann die Konfigurationdatei %s nicht �ffnen

ALOG_DEBUG_RESOLVED_HOST
	Debug: L�se %s zu %s auf

ALOG_DEBUG_BAD_SSTRDUP
	Debug: sstrdup() benannt mit NULL-arg

ALOG_RESET_DAILY
	Stelle die T�glichen Stats zur�ck

ALOG_RESETTING_WEEKLY
	Stelle die Wochen-Stats zur�ck

ALOG_RESETTING_MONTHLY
	Stelle die Monats-Stats zur�ck

ALOG_TRY_TO_DISPLAY
	Debug: Versuche die Nachricht %s anzuzeigen

ALOG_DONE_DISPLAY
	Debug: Erledigt, zeige die Nachrichte %s an

ALOG_UKNOWN_USER_MODE
	Debug: Unbekannter Mode : usermode %c (in %s)

ALOG_SWHOIS_ERROR
	SWHOIS f�r nicht vorhandenen Benutzer %s.

ALOG_VHOST_FOR
	Debug: %s hat seine vHost zu %s ge�ndert

ALOG_REALNAME_FOR
	Debug: %s hat sein realname zu %s ge�ndert

ALOG_USERNAME_FOR
	Debug: %s hat sein Nickname zu %s ge�ndert

ALOG_UMODE_NONEXISTANT
	User: MODE %s f�r nicht vorhandenen Nickname %s: %s

ALOG_UNKNWON_MSG
	Debug: Unbekannte Nachricht von Server (%s)

ALOG_TRYING_LOAD_MOD
	versuche [%s] zu laden

ALOG_STATUS
	Status: [%d]

ALOG_TRYING_UNLOAD_MOD
	versuche [%s] zu entladen

RESTARTING
	Neustart

RESTART_FAILED
	Restart missgl�ckt

SAVING_FFF
	Debug: Speichere die FFF Informationen

ALOG_SIGPIPE_WARNING
	Denora f�hrt ein Backtrace durch und bleibt weiterhin online. Denora k�nnte instabil werden 

ALOG_NO_STATS_BIN
	Restart versuch missgl�ckt!! STATS_BIN ist NICHT definiert (wiederhole ./Config)

ALOG_ERR_REHASH_NO_MEM
	Fehler beim Einlesen der Konfigurationsdatei, aber OUT of MEMORY!

ALOG_ERR_REHASH
	Fehler beim Lesen der Konfigurationsdatei (SIGHUP empfangen)

BACKTRACE_LINES
	Backtrace: Bitte berichten Sie die folgenden Zeilen

BACKTRACE_NOT_HERE
	Backtrace nicht vorhanden auf diese Plattform - benutze GDB

ALOG_DEBUG_SERVINTRO
	Debug: Server (%s) eingef�hrt

ALOG_DEBUG_SERVINTRO_FORM
	Debug: Server (%s) eingef�hrt von %s

ALOG_DEBUG_PINGTIME
	Debug: Ping Zeit von %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	Debug: Fehler: findserver() bennant mit NULL Wert

ALOG_ERR_FINDSERVERSTATS
	Fehler: findserverstats() bennant mit NULL Wert

ALOG_ERR_FINDSERVERSUID
	Fehler: findserver_suid() bennant mit NULL Wert

ALOG_ERR_DEL_SERVSTATS
	Fehler: delete_serverstats() bennant mit NULL Wert

ALOG_DEL_SERVSTATS_FOR
	Debug: delete_serverstats() f�r %s

ALOG_DEL_SERVSTATS_DONE
	Debug: delete_serverstats() fertig

ALOG_ERR_DEL_SERVER
	Fehler: delete_server() bennant mit NULL Wert

ALOG_DEL_SERVER_FOR
	Debug: delete_server() f�r %s

ALOG_DEL_SERVER_NOQUIT
	Debug: delete_server() alle Benutzer f�r NOQUIT gel�scht

ALOG_DEL_SERVER_DONE
	Debug: delete_server() fertig

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	Debug: sql_do_server_version hat ein unbekannten version format empfangen

ALOG_MOD_TRY_LOAD
	Versuche [%s] zu laden

ALOG_MOD_STATUS
	Status: [%d][%s]

ALOG_MOD_ERROR
	Module Fehler: %s

ALOG_MOD_BE_ONLY_ONE
	DU kannst NICHT 2 Protocol Module geladen haben.

ALOG_MOD_UNLOAD_SELF
	Debug: %s requested unload...

ALOG_CMD_DISPLAY_START
	Debug: versuche die %s Befehle anzuzeigen

ALOG_CMD_DISPLAY_END
	Debug: fertig mit anzeigen der Befehlen %s

ALOG_CMD_DISPLAY_LIST
	Debug: Zeige Befehls-Liste f�r %s an

ALOG_MSG_DISPLAY_START
	Debug: Versuche die %s Nachricht anzuzeigen 

ALOG_MSG_DISPLAY_END
	Debug: fertig mit anzeigen der Nachrichten %s

ALOG_MSG_DISPLAY_LIST
	Debug: Zeige Nachrichten-Liste f�r %s an

ALOG_MOD_CALLBACK_DISPLAY
	Debug: Added module CallBack: [%s] due to execute at %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST f�r nicht existenten User %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME f�r nicht existenten User %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME f�r nicht existenten User %s

STAT_ADMIN_ALREADY
	%s ist schon ein Stats Admin

STAT_ADMIN_CREATED
	%s ist nun ein Stats Admin

STAT_ADMIN_DELETED
	%s wurde von Stats Admin entfernt

STAT_ADMIN_NOTADMIN
	%s ist kein Stats Admin

STAT_ADMIN_SHOW
	Benutzername: %s Hostmaske: %s Sprachen-ID: %d

STAT_HELP_ADMIN
	Das Hinzuf�gen oder L�schen von Stats Admin hat keinen Einfluss
	auf Admins die sich in der Konfigurationsdatei befinden.
	
	ADMIN ADD Benutzer Passwort [Hostmaske] [Sprachnummer]
	ADMIN DEL Benutzer
	ADMIN SETPASS Benutzer neues-Passwort
	ADMIN SHOW Benutzer
	ADMIN LIST
	
	Der ADMIN ADD Befehl f�gt den angegebenen Stats Admin hinzu
	
	Der ADMIN DEL Befehl l�scht den angegebenen Stats Admin
	
	Der ADMIN SETPASS Befehl erlaubt es das Passwort eines Admins
	zu �ndern
	
	Der ADMIN SHOW Befehl zeigt Informationen zum angegebenen
	Admin an
	
	Der ADMIN LIST Befehl listet alle konfigurierten Admins
	
	Beschr�nkt auf Stats Admin

STAT_ADMIN_SYNTAX
	ADMIN [ADD|DEL|LIST] user pass [host] [langnum]

STAT_ADMIN_DELETE_CONFIG
	Kann nicht %s entfernen weil er sich in der Konfigurationsdatei befindet

STAT_CHGPASS_OK
	Passwort von %s wurde ge�ndert

STAT_CHGPASS_CONFIG
	Kann nicht Passw�rter der Konfigurationsdatei �ndern

<?php
/* German Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	crazy		<da.crew@gmx.net>
Updates:	Kaetzchen	<Ezpharess@aol.com>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'de_DE.UTF-8@euro', 'de_DE.UTF-8', 'de.UTF-8', 'ge.UTF-8', 'de_DE@euro', 'de_DE', 'de', 'ge');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Home');
DEFINE('_TM_NETWORK','Netzwerk');
DEFINE('_TM_GRAPHS','Diagramme');
DEFINE('_TM_CHANS','Channel');
DEFINE('_TM_USERS','Benutzer');
DEFINE('_TM_SEARCH','Suchen');
DEFINE('_TM_CONFIG','Optionen');
DEFINE('_TM_TITLE','IRC Statistiken');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Netz Status');
DEFINE('_SM_NETDETAILS','Details');
DEFINE('_SM_SERVERS','Server');
DEFINE('_SM_SRVLIST','Server Liste');
DEFINE('_SM_SRVDETAILS','Server Details');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Netzwerk Diagramme');
DEFINE('_SM_CUSTOM','Nach Maß...');
DEFINE('_SM_NETSPLIT','NetSplit.de Diagramme');
DEFINE('_SM_SEARCHIRC','SearchIRC Diagramme');
DEFINE('_SM_CHANLIST','Channel-Liste');
DEFINE('_SM_TOPCHANS','Channel Statistiken');
DEFINE('_SM_SEARCHCHAN','Channel suchen...');
DEFINE('_SM_CHANNELSTATUS','Channel Status');
DEFINE('_SM_CHANNELACT','Channel Aktivität');
DEFINE('_SM_WHOIN','Wer ist Online');
DEFINE('_SM_TOPUSERS','Benutzer Statistiken');
DEFINE('_SM_SEARCHUSER','Benutzer suchen...');
DEFINE('_SM_GLOBALSTATS','Globale Statistiken');
DEFINE('_SM_CHANSTATS','Channel Statistiken');
DEFINE('_SM_CHANINFO','Channel Informationen');
DEFINE('_SM_TLDPIE','Länder Statistiken');
DEFINE('_SM_CLIENTPIE','Client Statistiken');
DEFINE('_SM_INVITES','Einladungs-Ausnahmen');
DEFINE('_SM_BANS','Bans');
DEFINE('_SM_EXCEPTS','Ban-Ausnahmen');
DEFINE('_SM_TSTATS','Total');
DEFINE('_SM_MSTATS','Dieser Monat');
DEFINE('_SM_WSTATS','Diese Woche');
DEFINE('_SM_DSTATS','Heute');
DEFINE('_SM_ASTATS','Tagsdurchschnitt');
DEFINE('_SM_LANGUAGE','Sprache');
DEFINE('_SM_THEME','Theme');
DEFINE('_SM_SET','Übernehmen');
DEFINE('_SM_BEHAVIOR','Einstellungen');
DEFINE('_SM_OPERS','Opers');
DEFINE('_SM_PYSTATS','Vergangenes Jahr');
DEFINE('_SM_PMSTATS','Vergangener Monat');
DEFINE('_SM_PWSTATS','Vergangene Woche');
DEFINE('_SM_NSWEEKS','Letzte zwei Wochen');
DEFINE('_SM_NSMONTHS','Letzte zwei Monate');
DEFINE('_SM_NSYEARS','Letzt zwei Jahre');
DEFINE('_SM_NSHISTORY','Complette Chronologie');

/* Front Page */
DEFINE('_FP_RANKING','Aktuelles Ranking');
DEFINE('_FP_BIGGESTCHANS','Aktuell %s größten Channels');
DEFINE('_FP_TOPCHANS','Top %s Channels Heute');
DEFINE('_FP_TOPUSERS','Top %s Benutzer Heute');
DEFINE('_FP_STATUS','Netzwerk Status');
DEFINE('_FP_MAXSTATS','Max Stats');
DEFINE('_FP_USERS','Benutzer:');
DEFINE('_FP_CHANS','Channels:');
DEFINE('_FP_OPERS','Opers:');
DEFINE('_FP_SERVERS','Server:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Navigation');
DEFINE('_FP_QSTATS','Direkte Stats');
DEFINE('_FP_LSTATUS','Aktueller Status');
DEFINE('_FP_QUSER','Nickname');
DEFINE('_FP_QCHAN','Channel');
DEFINE('_FP_GO','Los');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Netzwerkstatus');
DEFINE('_PT_SERVERS','Server-Liste');
DEFINE('_PT_DETAILS','Server Details von %s');
DEFINE('_PT_MOTD','MOTD');
DEFINE('_PT_CHANLIST','Channel-Liste (/list)');
DEFINE('_PT_TCSTATS','Channel Aktivität');
DEFINE('_PT_CHANNELACT','Durchschnittliche Channel Aktivität');
DEFINE('_PT_WHOIN','Aktuell in %s');
DEFINE('_PT_SEARCHCHAN','Channel suchen');
DEFINE('_PT_USERACT','Durchschnittliche Benutzer Aktivität');
DEFINE('_PT_SEARCHUSER','Benutzer suchen');
DEFINE('_PT_TSTATS','Benutzer Aktivität');
DEFINE('_PT_STATSFOR','Statistiken für %s auf %s');
DEFINE('_PT_TLDPIE','Länder Statistiken für %s');
DEFINE('_PT_CLIENTPIE','Client Statistiken für %s');
DEFINE('_PT_MSTATS','dieser monat');
DEFINE('_PT_WSTATS','diese woche');
DEFINE('_PT_DSTATS','heute');
DEFINE('_PT_SEARCHRESULTS','Suchergebnisse');
DEFINE('_PT_OPERS','Aktuelle Opers');
DEFINE('_PT_NSU2W','Benutzer und Channels während der letzten zwei Wochen');
DEFINE('_PT_NSC2W','Channels während der letzten zwei Wochen');
DEFINE('_PT_NSS2W','Server während der letzten zwei Wochen');
DEFINE('_PT_NSU2M','Benutzer und Channels während der letzten zwei Monate');
DEFINE('_PT_NSC2M','Channels während der letzten zwei Monate');
DEFINE('_PT_NSS2M','Server während der letzten zwei Monate');
DEFINE('_PT_NSU2Y','Benutzer und Channels während der letzten zwei Jahre');
DEFINE('_PT_NSC2Y','Channels während der letzten zwei Jahre');
DEFINE('_PT_NSS2Y','Server während der letzten zwei Jahre');
DEFINE('_PT_NSHYS','Komplette gemessene Chronologie von Benutzer und Channels');

/* Network Status */
DEFINE('_NS_USERDET','Aktuelle Benutzer: %s Max heute: %s Max: %s am %s');
DEFINE('_NS_CHANDET','Aktuelle Channels: %s Max: %s am %s');
DEFINE('_NS_OPERDET','Aktuelle Opers: %s Max: %s am %s');
DEFINE('_NS_SERVDET','Aktuelle Server: %s Max: %s am %s');

/* Search */
DEFINE('_SP_PROMPT','Geben Sie bitte die Suchkriterien ein. Wildcards werden auch unterstützt.');
DEFINE('_SP_SUBMIT','Suchen');
DEFINE('_SP_CHANINFO','Channel Info');
DEFINE('_SP_CHANSTATS','Channel Stats');
DEFINE('_SP_USERINFO','User Info');
DEFINE('_SP_USERSTATS','User Stats');

/* Configuration Page */
DEFINE('_CP_SETSORT','Einstellung der Standart Sortierkriterien');
DEFINE('_CP_SETTYPE','Einstellung des Standart Statistiktyps');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','User');
DEFINE('_GD_HOST','Hostname');
DEFINE('_GD_CHANNEL','Channel');
DEFINE('_GD_USERS','Benutzer');
DEFINE('_GD_LETTERS','Buchstaben');
DEFINE('_GD_WORDS','Wörter');
DEFINE('_GD_LINES','Zeilen');
DEFINE('_GD_ACTIONS','Aktionen');
DEFINE('_GD_SMILEYS','Smileys');
DEFINE('_GD_KICKS','Kicks');
DEFINE('_GD_MODES','Modis');
DEFINE('_GD_TOPICS','Topics');
DEFINE('_GD_ON','auf');
DEFINE('_GD_TYPE','Art');
DEFINE('_GD_TOTAL','total');
DEFINE('_GD_OTHER','andere');
DEFINE('_GD_UNKNOWN','unbekannt');
DEFINE('_GD_NEVER','nie');
DEFINE('_GD_NONE','keine');
DEFINE('_GD_STATUS','Status');
DEFINE('_GD_YES','Ja');
DEFINE('_GD_NO','Nein');
DEFINE('_GD_HOUR','Stunde');
DEFINE('_GD_DAY','Tag');
DEFINE('_GD_MONTH','Monat');
DEFINE('_GD_YEAR','Jahr');
DEFINE('_GD_OR','oder');
DEFINE('_GD_AND','und');

/* Channel Status */
DEFINE('_CS_INFO0','Channel: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Geschrieben von: %s am: %s');
DEFINE('_CS_INFO3','Derzeitige Benutzer: %s Max: %s am %s');
DEFINE('_CS_INFO4','Channel Modes: %s');
DEFINE('_CS_INFO5','Einladungs-Ausnahmen: %s Bans: %s Ban-Ausnahmen: %s');

/* User Info */
DEFINE('_UI_UINFO','Informationen für den Nickname %s (Stats-Gruppe %s)');
DEFINE('_UI_NODATA','Der Benutzer %s erscheint als Benutzer %s und es wurden keine Informationen gespeichert.');
DEFINE('_UI_UCHANS','Überwachte Channels: %s');
DEFINE('_UI_WHOIS1','Nickname: %s Realer Name: %s');
DEFINE('_UI_WHOIS2','Mask: %s');
DEFINE('_UI_WHOIS3','Status: %s');
DEFINE('_UI_WHOIS4','Verbunden seit: %s');
DEFINE('_UI_WHOIS5','Letzter Quit: %s');
DEFINE('_UI_WHOIS6','Abwesenheitsstatus: %s');
DEFINE('_UI_WHOIS7','Abwesenheitsnachricht: %s');
DEFINE('_UI_WHOIS8','Benutzter Klient: %s');
DEFINE('_UI_WHOIS9','Verbunden von: %s Am Server: %s');
DEFINE('_UI_WHOIS10','Zusätzliche Information: %s');

/* Oper list */
DEFINE('_OL_NICK','Nickname');
DEFINE('_OL_LEVEL','Niveau');
DEFINE('_OL_ONLINE','Online seit');
DEFINE('_OL_LASTQUIT','Zulezt gesehen');
DEFINE('_OL_BOT','Bot Service');
DEFINE('_OL_SERVICE','Netzwerk Service');
DEFINE('_OL_AWAY','Abwesend');
DEFINE('_OL_AVAILABLE','Verfügbar für Hilfe');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Zeige Channels %s-%s von %s');
DEFINE('_PS_USERSHOW','Zeige Users %s-%s von %s');
DEFINE('_PS_ASCENDING','Aufsteigend');
DEFINE('_PS_DESCENDING','Absteigend');

/* Errors */
DEFINE('_ER_ERROR','FEHLER');
DEFINE('_ER_WARNING','ACHTUNG');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Keine Daten');
DEFINE('_ER_NOINVITES','Keine Einladungs-Ausnahmen');
DEFINE('_ER_NOBANS','Keine Bans');
DEFINE('_ER_NOEXCEPTS','Keine Ban-Ausnahmen');
DEFINE('_ER_NORESULTS','Keine Ergebnisse');
DEFINE('_ER_MINCHARS','Es werden mindestens %s Zeichen benötigt um zu suchen');
DEFINE('_ER_MODE','Der gewünschte Ausführungsmodus %s ist unbekannt');
DEFINE('_ER_NOSTATS','Keine statistischen Daten für diesen Chan/User gesammelt');
DEFINE('_ER_NOCHAN','Dieser Channel existiert nicht');
DEFINE('_ER_EMPTYCHAN','Dieser Channel ist momentan leer');
DEFINE('_ER_DENIED','Zugriff Verweigert');

/* API */
DEFINE('_DA_CURRENTUSR','Aktuelle Benutzer');
DEFINE('_DA_MAXUSR','Benutzer maximum');
DEFINE('_DA_SERVER','Servername');
DEFINE('_DA_SRVDET1','Version %s An seit %s');
DEFINE('_DA_SRVDET2','Aktuell %s');
DEFINE('_DA_SRVDET3','seit %s');
DEFINE('_DA_SRVDET4','Letzter split: %s');
DEFINE('_DA_SRVDET5','Aktueller Ping: %s  Max: %s am %s');
DEFINE('_DA_SRVDET6','Aktuelle Benutzer: %s  Max: %s am %s');
DEFINE('_DA_SRVDET7','Aktuelle Opers: %s  Max: %s am %s');
DEFINE('_DA_SRVDET8','Ircop Kills: %s  Server Kills: %s');
DEFINE('_DA_UPTIME','%s Tage und %s Stunden');

/* Graphs */
DEFINE('_GR_CUSTOM','Diagramme Nach Maß');
DEFINE('_GR_SELECT','Wähle...');
DEFINE('_GR_GENERATE','Erzeugen');
DEFINE('_GR_SDATE','Start Datum');
DEFINE('_GR_EDATE','Datum Ende');
DEFINE('_GR_OPTIONS','Optionen');
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Avg');

/* Other */
DEFINE('_OT_GENTIME','Seite in %s Sekunden mit %s sql Anfragen erstellt');
DEFINE('_OT_CGENTIME','Komprimierte Seite in %s Sekunden mit %s sql Anfragen erstellt');

?>

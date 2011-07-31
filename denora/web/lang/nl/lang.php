<?php
/* Dutch Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	iScripters	<iScripters@gmail.com>
Updates:    OUTsider    <outsider@scarynet.org>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Directe toegang tot deze locatie is niet toegestaan.' );

setlocale(LC_ALL, 'nl_NL.UTF-8@euro', 'nl_NL.UTF-8', 'nl.UTF-8', 'nla.UTF-8', 'nl_NL@euro', 'nl_NL', 'nl', 'nla');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Begin');
DEFINE('_TM_NETWORK','Netwerk');
DEFINE('_TM_GRAPHS','Grafieken');
DEFINE('_TM_CHANS','Kanalen');
DEFINE('_TM_USERS','Gebruikers');
DEFINE('_TM_SEARCH','Zoeken');
DEFINE('_TM_CONFIG','Instellingen');
DEFINE('_TM_TITLE','IRC Statistieken');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Netwerk Status');
DEFINE('_SM_NETDETAILS','Details');
DEFINE('_SM_SERVERS','Servers');
DEFINE('_SM_SRVLIST','Server Lijst');
DEFINE('_SM_SRVDETAILS','Server Details');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Netwerk Grafieken');
DEFINE('_SM_CUSTOM','Wijzig...');
DEFINE('_SM_NETSPLIT','NetSplit.de Grafieken');
DEFINE('_SM_SEARCHIRC','SearchIRC Grafieken');
DEFINE('_SM_CHANLIST','Kanalen lijst');
DEFINE('_SM_TOPCHANS','Kanalen statistieken');
DEFINE('_SM_SEARCHCHAN','Zoek kanaal...');
DEFINE('_SM_CHANNELSTATUS','Kanaal Status');
DEFINE('_SM_CHANNELACT','Kanaal activiteit');
DEFINE('_SM_WHOIN','Wie is Online');
DEFINE('_SM_TOPUSERS','Gebruikers statistieken');
DEFINE('_SM_SEARCHUSER','Zoek gebruiker...');
DEFINE('_SM_GLOBALSTATS','Globale Statistieken');
DEFINE('_SM_CHANSTATS','Kanaal Statistieken');
DEFINE('_SM_CHANINFO','Kanaal Informatie');
DEFINE('_SM_TLDPIE','Landelijke Statistieken');
DEFINE('_SM_CLIENTPIE','Client Statistieken');
DEFINE('_SM_INVITES','Uitnodigingsuitzonderingen');
DEFINE('_SM_BANS','Bans');
DEFINE('_SM_EXCEPTS','Ban-uitzonderingen');
DEFINE('_SM_TSTATS','Totaal');
DEFINE('_SM_MSTATS','Deze maand');
DEFINE('_SM_WSTATS','Deze week');
DEFINE('_SM_DSTATS','Vandaag');
DEFINE('_SM_ASTATS','Dagelijks gemiddelde');
DEFINE('_SM_LANGUAGE','Taal');
DEFINE('_SM_THEME','Thema');
DEFINE('_SM_SET','Opslaan');
DEFINE('_SM_BEHAVIOR','Gedrag');
DEFINE('_SM_OPERS','Opers');
DEFINE('_SM_PYSTATS','Afgelopen Jaar');
DEFINE('_SM_PMSTATS','Afgelopen Maand');
DEFINE('_SM_PWSTATS','Afgelopen Week');
DEFINE('_SM_NSWEEKS','Afgelopen 2 weken');
DEFINE('_SM_NSMONTHS','Afgelopen 2 maanden');
DEFINE('_SM_NSYEARS','Afgelopen 2 jaar');
DEFINE('_SM_NSHISTORY','Complete geschiedenis');

/* Front Page */
DEFINE('_FP_RANKING','Huidige Ranking');
DEFINE('_FP_BIGGESTCHANS','Huidige Top %s Kanalen');
DEFINE('_FP_TOPCHANS','Top %s Kanalen vandaag');
DEFINE('_FP_TOPUSERS','Top %s Gebruik. vandaag');
DEFINE('_FP_STATUS','Netwerk Status');
DEFINE('_FP_MAXSTATS','Max Statistieken');
DEFINE('_FP_USERS','Gebruikers:');
DEFINE('_FP_CHANS','Kanalen:');
DEFINE('_FP_OPERS','Opers:');
DEFINE('_FP_SERVERS','Servers:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Snelle Navigatie');
DEFINE('_FP_QSTATS','Snelle Statistieken');
DEFINE('_FP_LSTATUS','Live Status');
DEFINE('_FP_QUSER','Gebruiker');
DEFINE('_FP_QCHAN','Kanaal');
DEFINE('_FP_GO','Ga');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Netwerk Status');
DEFINE('_PT_SERVERS','Server Lijst');
DEFINE('_PT_DETAILS','Server Details voor %s');
DEFINE('_PT_MOTD','MOTD');
DEFINE('_PT_CHANLIST','Kanaal Lijst (/list)');
DEFINE('_PT_TCSTATS','Kanaal activiteit');
DEFINE('_PT_CHANNELACT','Gemiddelde kanaal activiteit');
DEFINE('_PT_WHOIN','Op dit moment op %s');
DEFINE('_PT_SEARCHCHAN','Kanaal zoeken');
DEFINE('_PT_USERACT','Gemiddelde gebruiker activiteit');
DEFINE('_PT_SEARCHUSER','Gebruiker zoeken');
DEFINE('_PT_TSTATS','Gebruikers activiteit');
DEFINE('_PT_STATSFOR','Statistieken voor %s op %s');
DEFINE('_PT_TLDPIE','Landelijke statistieken voor %s');
DEFINE('_PT_CLIENTPIE','Client Statistieken voor %s');
DEFINE('_PT_MSTATS','deze maand');
DEFINE('_PT_WSTATS','deze week');
DEFINE('_PT_DSTATS','vandaag');
DEFINE('_PT_SEARCHRESULTS','Zoek Resultaten');
DEFINE('_PT_OPERS','Huidige Opers');
DEFINE('_PT_NSU2W','Relatie tussen users en kanalen gedurende de afgelopen 2 weken');
DEFINE('_PT_NSC2W','Kanalen gedurende de afgelopen 2 weken');
DEFINE('_PT_NSS2W','Servers gedurende de afgelopen 2 weken');
DEFINE('_PT_NSU2M','Relatie tussen users en kanalen gedurende de afgelopen 2 maanden');
DEFINE('_PT_NSC2M','Kanalen gedurende de afgelopen 2 maanden');
DEFINE('_PT_NSS2M','Servers gedurende de afgelopen 2 maanden');
DEFINE('_PT_NSU2Y','Relatie tussen users en kanalen gedurende de afgelopen 2 jaar');
DEFINE('_PT_NSC2Y','Kanalen gedurende de afgelopen 2 jaar');
DEFINE('_PT_NSS2Y','Servers gedurende de afgelopen 2 jaar');
DEFINE('_PT_NSHYS','Volledig gemeten geschiedenis over users en kanalen');

/* Network Status */
DEFINE('_NS_USERDET','Huidige gebruikers: %s Max vandaag: %s Max: %s on %s');
DEFINE('_NS_CHANDET','Huidige kanalen: %s Max: %s on %s');
DEFINE('_NS_OPERDET','Huidige opers: %s Max: %s on %s');
DEFINE('_NS_SERVDET','Huidige servers: %s Max: %s on %s');

/* Search */
DEFINE('_SP_PROMPT','Geef uw zoekcriteria op. Het gebruik van wildcards is toegestaan');
DEFINE('_SP_SUBMIT','Zoek');
DEFINE('_SP_CHANINFO','Kanaal Info');
DEFINE('_SP_CHANSTATS','Kanaal Stats');
DEFINE('_SP_USERINFO','Gebruiker Info');
DEFINE('_SP_USERSTATS','Gebruiker Stats');

/* Configuration Page */
DEFINE('_CP_SETSORT','Standaard sortering instellen');
DEFINE('_CP_SETTYPE','Standaard statistieken type instellen');

/* General Definitions */
DEFINE('_GD_NUMBER','#');
DEFINE('_GD_NICK','nicknaam');
DEFINE('_GD_HOST','hostnaam');
DEFINE('_GD_CHANNEL','kanaal');
DEFINE('_GD_USERS','gebruikers');
DEFINE('_GD_LETTERS','letters');
DEFINE('_GD_WORDS','woorden');
DEFINE('_GD_LINES','regels');
DEFINE('_GD_ACTIONS','acties');
DEFINE('_GD_SMILEYS','smilies');
DEFINE('_GD_KICKS','kicks');
DEFINE('_GD_MODES','modes');
DEFINE('_GD_TOPICS','topics');
DEFINE('_GD_ON','aan');
DEFINE('_GD_TYPE','type');
DEFINE('_GD_TOTAL','totaal');
DEFINE('_GD_OTHER','overige');
DEFINE('_GD_UNKNOWN','onbekend');
DEFINE('_GD_NEVER','nooit');
DEFINE('_GD_NONE','geen');
DEFINE('_GD_STATUS','Status');
DEFINE('_GD_YES','Ja');
DEFINE('_GD_NO','Nee');
DEFINE('_GD_HOUR','Uur');
DEFINE('_GD_DAY','Dag');
DEFINE('_GD_MONTH','Maand');
DEFINE('_GD_YEAR','Jaar');
DEFINE('_GD_OR','of');
DEFINE('_GD_AND','en');

/* Channel Status */
DEFINE('_CS_INFO0','Kanaal: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Door: %s On: %s');
DEFINE('_CS_INFO3','Huidige gebruikers: %s Hoogste: %s op %s');
DEFINE('_CS_INFO4','Kanaal modes: %s');
DEFINE('_CS_INFO5','Uitnodigingsuitzonderingen: %s Bans: %s Ban-uitzonderingen: %s');

/* User Info */
DEFINE('_UI_UINFO','Informatie over %s (stats group %s)');
DEFINE('_UI_NODATA','Gebruiker %s is op %s en er is geen informatie gecached.');
DEFINE('_UI_UCHANS','Aanwezig in: %s');
DEFINE('_UI_WHOIS1','Nickname: %s Echte naam: %s');
DEFINE('_UI_WHOIS2','Mask: %s');
DEFINE('_UI_WHOIS3','Status: %s');
DEFINE('_UI_WHOIS4','Verbonden sinds: %s');
DEFINE('_UI_WHOIS5','Laatste quit: %s');
DEFINE('_UI_WHOIS6','Afwezig-status: %s');
DEFINE('_UI_WHOIS7','Bericht: %s');
DEFINE('_UI_WHOIS8','Gebruikte Client: %s');
DEFINE('_UI_WHOIS9','Verbonden via: %s Server: %s');
DEFINE('_UI_WHOIS10','Aanvullende informatie: %s');

/* Oper list */
DEFINE('_OL_NICK','Nickname');
DEFINE('_OL_LEVEL','Niveau');
DEFINE('_OL_ONLINE','Online sinds');
DEFINE('_OL_LASTQUIT','Laatste quit');
DEFINE('_OL_BOT','Bot Service');
DEFINE('_OL_SERVICE','Netwerk Service');
DEFINE('_OL_AWAY','Weg');
DEFINE('_OL_AVAILABLE','Beschikbaar voor Hulp');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Toont kanalen %s-%s van %s');
DEFINE('_PS_USERSHOW','Toont gebruikers %s-%s van %s');
DEFINE('_PS_ASCENDING','Oplopend');
DEFINE('_PS_DESCENDING','Aflopend');

/* Errors */
DEFINE('_ER_ERROR','FOUT');
DEFINE('_ER_WARNING','WAARSCHUWING');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Geen data'); 
DEFINE('_ER_NOINVITES','Geen uitnodigingsuitzonderingen');
DEFINE('_ER_NOBANS','Geen bans');
DEFINE('_ER_NOEXCEPTS','Geen ban-uitzonderingen');
DEFINE('_ER_NORESULTS','Geen resultaten');
DEFINE('_ER_MINCHARS','Minimaal %s tekens nodig om te zoeken');
DEFINE('_ER_MODE','De gevraagde mode %s is onbekend');
DEFINE('_ER_NOSTATS','Geen statistieken bekend voor dit kanaal/deze gebruiker');
DEFINE('_ER_NOCHAN','Dit kanaal bestaat niet');
DEFINE('_ER_EMPTYCHAN','Dit kanaal is op het moment leeg');
DEFINE('_ER_DENIED','Geen toegang');

/* API */
DEFINE('_DA_CURRENTUSR','Huidige gebruikers');
DEFINE('_DA_MAXUSR','Max. gebruikers');
DEFINE('_DA_SERVER','Server naam');
DEFINE('_DA_SRVDET1','Gebruikt %s Aktief sinds %s');
DEFINE('_DA_SRVDET2','Momenteel %s');
DEFINE('_DA_SRVDET3','sinds %s');
DEFINE('_DA_SRVDET4','Laatste split: %s');
DEFINE('_DA_SRVDET5','Huidige Ping: %s  Max: %s op %s');
DEFINE('_DA_SRVDET6','Huidige Users: %s  Max: %s op %s');
DEFINE('_DA_SRVDET7','Huidige Opers: %s  Max: %s op %s');
DEFINE('_DA_SRVDET8','Ircop Kills: %s  Server Kills: %s');
DEFINE('_DA_UPTIME','%s dagen en %s uur');

/* Graphs */
DEFINE('_GR_CUSTOM','Eigen Grafieken');
DEFINE('_GR_SELECT','Selecteer...');
DEFINE('_GR_GENERATE','Genereer');
DEFINE('_GR_SDATE','Begin Datum');
DEFINE('_GR_EDATE','Eind Datum');
DEFINE('_GR_OPTIONS','Opties');
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Gem');

/* Other */
DEFINE('_OT_GENTIME','Pagina gegenereerd in %s secondes met behulp van %s sql queries');
DEFINE('_OT_CGENTIME','Gecomprimeerde pagina gegenereerd in %s seconden, met %s sql queries');

?>

<?php
/* Italian Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	Hal9000		<hal9000@denorastats.org>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'it_IT.UTF-8@euro', 'it_IT.UTF-8', 'it.UTF-8', 'ita.UTF-8', 'it_IT@euro', 'it_IT', 'it', 'ita');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Home');
DEFINE('_TM_NETWORK','Rete');
DEFINE('_TM_GRAPHS','Grafici');
DEFINE('_TM_CHANS','Canali');
DEFINE('_TM_USERS','Utenti');
DEFINE('_TM_SEARCH','Cerca');
DEFINE('_TM_CONFIG','Opzioni');
DEFINE('_TM_TITLE','Statistiche IRC');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Stato del Network');
DEFINE('_SM_NETDETAILS','Dettagli');
DEFINE('_SM_SERVERS','Server');
DEFINE('_SM_SRVLIST','Lista Server');
DEFINE('_SM_SRVDETAILS','Dettagli Server');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Statistiche Grafiche');
DEFINE('_SM_CUSTOM','Personalizzate...');
DEFINE('_SM_NETSPLIT','Grafici NetSplit.de');
DEFINE('_SM_SEARCHIRC','Grafici SearchIRC');
DEFINE('_SM_CHANLIST','Lista Canali');
DEFINE('_SM_TOPCHANS','Statistiche Canali');
DEFINE('_SM_SEARCHCHAN','Cerca Canale...');
DEFINE('_SM_CHANNELSTATUS','Stato Canale');
DEFINE('_SM_CHANNELACT','Attività Canale');
DEFINE('_SM_WHOIN','Chi è Online');
DEFINE('_SM_TOPUSERS','Statistiche Utenti');
DEFINE('_SM_SEARCHUSER','Cerca Utente...');
DEFINE('_SM_GLOBALSTATS','Statistiche Globali');
DEFINE('_SM_CHANSTATS','Statistiche Canale');
DEFINE('_SM_CHANINFO','Informazioni sul Canale');
DEFINE('_SM_TLDPIE','Statistiche Paese');
DEFINE('_SM_CLIENTPIE','Statistiche Client');
DEFINE('_SM_INVITES','Eccezioni Inviti');
DEFINE('_SM_BANS','Ban');
DEFINE('_SM_EXCEPTS','Eccezioni Ban');
DEFINE('_SM_TSTATS','Totale');
DEFINE('_SM_MSTATS','Questo mese');
DEFINE('_SM_WSTATS','Questa settimana');
DEFINE('_SM_DSTATS','Oggi');
DEFINE('_SM_ASTATS','Media giornaliera');
DEFINE('_SM_LANGUAGE','Lingua');
DEFINE('_SM_THEME','Stile');
DEFINE('_SM_SET','Applica');
DEFINE('_SM_BEHAVIOR','Comportamento');
DEFINE('_SM_OPERS','Operatori');
DEFINE('_SM_PYSTATS','Ultimo anno');
DEFINE('_SM_PMSTATS','Ultimo mese');
DEFINE('_SM_PWSTATS','Ultima settimana');
DEFINE('_SM_NSWEEKS','Ultime due settimane');
DEFINE('_SM_NSMONTHS','Ultimi due mesi');
DEFINE('_SM_NSYEARS','Ultimi due anni');
DEFINE('_SM_NSHISTORY','Cronologia completa');

/* Front Page */
DEFINE('_FP_RANKING','Ranking Attuale');
DEFINE('_FP_BIGGESTCHANS','Attuali %s Canali più grandi');
DEFINE('_FP_TOPCHANS','Top %s Canali Oggi');
DEFINE('_FP_TOPUSERS','Top %s Utenti Oggi');
DEFINE('_FP_STATUS','Stato del Network');
DEFINE('_FP_MAXSTATS','Statistiche Massime');
DEFINE('_FP_USERS','Utenti:');
DEFINE('_FP_CHANS','Canali:');
DEFINE('_FP_OPERS','Operatori:');
DEFINE('_FP_SERVERS','Server:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Navigazione Veloce');
DEFINE('_FP_QSTATS','Statistiche Veloci');
DEFINE('_FP_LSTATUS','Stato Attuale');
DEFINE('_FP_QUSER','Utente');
DEFINE('_FP_QCHAN','Canale');
DEFINE('_FP_GO','Vai');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Stato della Rete');
DEFINE('_PT_SERVERS','Lista Server');
DEFINE('_PT_DETAILS','Dettagli per %s');
DEFINE('_PT_MOTD','Messaggio del Giorno (MOTD)');
DEFINE('_PT_CHANLIST','Lista Canali (/list)');
DEFINE('_PT_TCSTATS','Attività Canali');
DEFINE('_PT_CHANNELACT','Attività Media del Canale');
DEFINE('_PT_WHOIN','Attualmente in %s');
DEFINE('_PT_SEARCHCHAN','Ricerca Canale');
DEFINE('_PT_USERACT','Attività Media dell\'Utente');
DEFINE('_PT_SEARCHUSER','Ricerca Utente');
DEFINE('_PT_TSTATS','Attività Utenti');
DEFINE('_PT_STATSFOR','Statistiche per %s in %s');
DEFINE('_PT_TLDPIE','Statistiche Paese per %s');
DEFINE('_PT_CLIENTPIE','Statistiche Client per %s');
DEFINE('_PT_MSTATS','questo mese');
DEFINE('_PT_WSTATS','questa settimana');
DEFINE('_PT_DSTATS','oggi');
DEFINE('_PT_SEARCHRESULTS','Risultati Ricerca');
DEFINE('_PT_OPERS','Operatori Attuali');
DEFINE('_PT_NSU2W','Utenti e canali durante le ultime due settimane');
DEFINE('_PT_NSC2W','Canali durante le ultime due settimane');
DEFINE('_PT_NSS2W','Server durante le ultime due settimane');
DEFINE('_PT_NSU2M','Utenti e canali durante gli ultimi due mesi');
DEFINE('_PT_NSC2M','Canali durante gli ultimi due mesi');
DEFINE('_PT_NSS2M','Server durante gli ultimi due mesi');
DEFINE('_PT_NSU2Y','Utenti e canali durante gli ultimi due anni');
DEFINE('_PT_NSC2Y','Canali durante gli ultimi due anni');
DEFINE('_PT_NSS2Y','Server durante gli ultimi due anni');
DEFINE('_PT_NSHYS','Cronologia misurata completa su utenti e canali');

/* Network Status */
DEFINE('_NS_USERDET','Utenti attuali: %s Picco odierno: %s Picco: %s il %s');
DEFINE('_NS_CHANDET','Canali attuali: %s Picco: %s il %s');
DEFINE('_NS_OPERDET','Operatori attuali: %s Picco: %s il %s');
DEFINE('_NS_SERVDET','Server attuali: %s Picco: %s il %s');

/* Search */
DEFINE('_SP_PROMPT','Immetti i criteri di ricerca. Sono ammesse anche le wildcard (*).');
DEFINE('_SP_SUBMIT','Cerca');
DEFINE('_SP_CHANINFO','Info Canale');
DEFINE('_SP_CHANSTATS','Stats Canale');
DEFINE('_SP_USERINFO','Info Utente');
DEFINE('_SP_USERSTATS','Stats Utente');

/* Configuration Page */
DEFINE('_CP_SETSORT','Imposta il criterio di ordinamento di base');
DEFINE('_CP_SETTYPE','Imposta il tipo di statistiche di base');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','nickname');
DEFINE('_GD_HOST','hostname');
DEFINE('_GD_CHANNEL','canale');
DEFINE('_GD_USERS','utenti');
DEFINE('_GD_LETTERS','lettere');
DEFINE('_GD_WORDS','parole');
DEFINE('_GD_LINES','linee');
DEFINE('_GD_ACTIONS','azioni');
DEFINE('_GD_SMILEYS','faccine');
DEFINE('_GD_KICKS','kick');
DEFINE('_GD_MODES','modi');
DEFINE('_GD_TOPICS','topic');
DEFINE('_GD_ON','su');
DEFINE('_GD_TYPE','tipo');
DEFINE('_GD_TOTAL','totale');
DEFINE('_GD_OTHER','altro');
DEFINE('_GD_UNKNOWN','sconosciuto');
DEFINE('_GD_NEVER','mai');
DEFINE('_GD_NONE','nessuno');
DEFINE('_GD_STATUS','Stato');
DEFINE('_GD_YES','Sì');
DEFINE('_GD_NO','No');
DEFINE('_GD_HOUR','Ora');
DEFINE('_GD_DAY','Giorno');
DEFINE('_GD_MONTH','Mese');
DEFINE('_GD_YEAR','Anno');
DEFINE('_GD_OR','o');
DEFINE('_GD_AND','e');

/* Channel Status */
DEFINE('_CS_INFO0','Canale: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Impostato da: %s il: %s');
DEFINE('_CS_INFO3','Utenti attuali: %s Picco: %s (%s)');
DEFINE('_CS_INFO4','Modi canale: %s');
DEFINE('_CS_INFO5','Eccezioni Inviti: %s Ban: %s Eccezioni Ban: %s');

/* User Info */
DEFINE('_UI_UINFO','Informazioni sul nickname %s (gruppo stats %s)');
DEFINE('_UI_NODATA','L\'utente %s sembra essere %s e non ci sono informazioni in cache.');
DEFINE('_UI_UCHANS','Canali monitorati: %s');
DEFINE('_UI_WHOIS1','Nickname: %s Real Name: %s');
DEFINE('_UI_WHOIS2','Mask: %s');
DEFINE('_UI_WHOIS3','Stato: %s');
DEFINE('_UI_WHOIS4','Collegato da: %s');
DEFINE('_UI_WHOIS5','Ultimo Quit: %s');
DEFINE('_UI_WHOIS6','Assente: %s');
DEFINE('_UI_WHOIS7','Messaggio: %s');
DEFINE('_UI_WHOIS8','Client Utilizzato: %s');
DEFINE('_UI_WHOIS9','Collegato da: %s Al server: %s');
DEFINE('_UI_WHOIS10','Informazioni Aggiuntive: %s');

/* Oper list */
DEFINE('_OL_NICK','Nickname');
DEFINE('_OL_LEVEL','Livello');
DEFINE('_OL_ONLINE','Online da');
DEFINE('_OL_LASTQUIT','Uscito');
DEFINE('_OL_BOT','Servizio Bot');
DEFINE('_OL_SERVICE','Servizio di Rete');
DEFINE('_OL_AWAY','Assente');
DEFINE('_OL_AVAILABLE','Disponibile per Aiuto');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Canali %s-%s di %s');
DEFINE('_PS_USERSHOW','Utenti %s-%s di %s');
DEFINE('_PS_ASCENDING','Ordine Crescente');
DEFINE('_PS_DESCENDING','Ordine Decrescente');

/* Errors */
DEFINE('_ER_ERROR','ERRORE');
DEFINE('_ER_WARNING','ATTENZIONE');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Nessun Dato');
DEFINE('_ER_NOINVITES','Nessuna Eccezione Invito');
DEFINE('_ER_NOBANS','Nessun Ban');
DEFINE('_ER_NOEXCEPTS','Nessuna Eccezione Ban');
DEFINE('_ER_NORESULTS','Nessun Risultato');
DEFINE('_ER_MINCHARS','Devi immettere almeno %s caratteri per cercare');
DEFINE('_ER_MODE','La modalità richiesta %s è sconosciuta');
DEFINE('_ER_NOSTATS','Nessun dato statistico rilevato per questo utente/canale');
DEFINE('_ER_NOCHAN','Questo canale non esiste');
DEFINE('_ER_EMPTYCHAN','Questo canale è attualmente vuoto');
DEFINE('_ER_DENIED','Accesso Negato');

/* API */
DEFINE('_DA_CURRENTUSR','Utenti attuali');
DEFINE('_DA_MAXUSR','Picco utenti');
DEFINE('_DA_SERVER','Nome server');
DEFINE('_DA_SRVDET1','Versione %s Acceso da %s');
DEFINE('_DA_SRVDET2','Attualmente %s');
DEFINE('_DA_SRVDET3','da %s');
DEFINE('_DA_SRVDET4','Ultimo split: %s');
DEFINE('_DA_SRVDET5','Ping Attuale: %s  Max: %s (%s)');
DEFINE('_DA_SRVDET6','Utenti Attuali: %s  Max: %s (%s)');
DEFINE('_DA_SRVDET7','Opers Attuali: %s  Max: %s (%s)');
DEFINE('_DA_SRVDET8','Ircop Kills: %s  Server Kills: %s');
DEFINE('_DA_UPTIME','%s giorni e %s ore');

/* Graphs */
DEFINE('_GR_CUSTOM','Grafici Personalizzati');
DEFINE('_GR_SELECT','Seleziona...');
DEFINE('_GR_GENERATE','Genera');
DEFINE('_GR_SDATE','Data inizio');
DEFINE('_GR_EDATE','Data fine');
DEFINE('_GR_OPTIONS','Opzioni');
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Med');

/* Other */
DEFINE('_OT_GENTIME','Pagina generata in %s secondi con %s richieste sql');
DEFINE('_OT_CGENTIME','Pagina compressa generata in %s secondi con %s richieste sql');

?>

<?php
/* Romanian Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

	Author:		RA3OR		<ra3or@as.ro>
Updates:	crazy		<crazy@frugalware.org>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'ro_RO.UTF-8', 'ro.UTF-8', 'rom.UTF-8', 'ro_RO', 'ro', 'rom');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Acasa');
DEFINE('_TM_NETWORK','Retea');
DEFINE('_TM_GRAPHS','Diagrame');
DEFINE('_TM_CHANS','Canale');
DEFINE('_TM_USERS','Utilizatori');
DEFINE('_TM_SEARCH','Cauta');
DEFINE('_TM_CONFIG','Setari');
DEFINE('_TM_TITLE','Statistici IRC');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Informatii Retea');
DEFINE('_SM_NETDETAILS','Detalii');
DEFINE('_SM_SERVERS','Informatii Servere');
DEFINE('_SM_SRVLIST','Lista Servere');
DEFINE('_SM_SRVDETAILS','Detalii Server');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Diagrame');
DEFINE('_SM_CUSTOM','Customizeaza...');
DEFINE('_SM_NETSPLIT','Diagrame NetSplit.de');
DEFINE('_SM_SEARCHIRC','Diagrame SearchIRC');
DEFINE('_SM_CHANLIST','Listare Canale');
DEFINE('_SM_TOPCHANS','Canale Monitorizate');
DEFINE('_SM_SEARCHCHAN','Cautare Canal');
DEFINE('_SM_CHANNELSTATUS','Status Canal');
DEFINE('_SM_CHANNELACT','Activitate Canal');
DEFINE('_SM_WHOIN','Cine-i Online');
DEFINE('_SM_TOPUSERS','Listare Utilizatori');
DEFINE('_SM_SEARCHUSER','Cautare Utilizator');
DEFINE('_SM_GLOBALSTATS','Status Global');
DEFINE('_SM_CHANSTATS','Status Canal');
DEFINE('_SM_CHANINFO','Informatii Canal');
DEFINE('_SM_TLDPIE','Statistici Tara');
DEFINE('_SM_CLIENTPIE','Statistici Client');
DEFINE('_SM_INVITES','Invitari per Canal');
DEFINE('_SM_BANS','Banuri per Canal');
DEFINE('_SM_EXCEPTS','Exceptii per Canal');
DEFINE('_SM_TSTATS','Per Total');
DEFINE('_SM_MSTATS','Luna Curenta');
DEFINE('_SM_WSTATS','Saptamana Curenta');
DEFINE('_SM_DSTATS','Astazi');
DEFINE('_SM_ASTATS','Media Zilnica');
DEFINE('_SM_LANGUAGE','Limba');
DEFINE('_SM_THEME','Tema');
DEFINE('_SM_SET','Aplica');
DEFINE('_SM_BEHAVIOR','Comportament');
DEFINE('_SM_OPERS','Informatii Operatori');
DEFINE('_SM_PYSTATS','Anul trecut');
DEFINE('_SM_PMSTATS','Luna trecuta');
DEFINE('_SM_PWSTATS','Saptamina trecuta');
DEFINE('_SM_NSWEEKS','Ultimele Doua Saptamani');
DEFINE('_SM_NSMONTHS','Ultimele Doua Luni');
DEFINE('_SM_NSYEARS','Ultimii Doi Ani');
DEFINE('_SM_NSHISTORY','Istoria Completa');

/* Front Page */
DEFINE('_FP_RANKING','Rang Curent');
DEFINE('_FP_BIGGESTCHANS','Top %s Canale Curente');
DEFINE('_FP_TOPCHANS','Top %s Canale de Azi');
DEFINE('_FP_TOPUSERS','Top %s Utilizatori de Azi');
DEFINE('_FP_STATUS','Info Retea');
DEFINE('_FP_MAXSTATS','Maxime');
DEFINE('_FP_USERS','Utilizatori:');
DEFINE('_FP_CHANS','Canale:');
DEFINE('_FP_OPERS','Operatori:');
DEFINE('_FP_SERVERS','Servere:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Navigare rapida');
DEFINE('_FP_QSTATS','Status rapid');
DEFINE('_FP_LSTATUS','Status Live');
DEFINE('_FP_QUSER','Utilizator');
DEFINE('_FP_QCHAN','Canal');
DEFINE('_FP_GO','OK');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Informatii Retea');
DEFINE('_PT_SERVERS','Lista de Servere');
DEFINE('_PT_DETAILS','Detalii Server - %s');
DEFINE('_PT_MOTD','Mesajul Zilei (MOTD)');
DEFINE('_PT_CHANLIST','Lista Canale (/list)');
DEFINE('_PT_TCSTATS','Activitate Canal');
DEFINE('_PT_CHANNELACT','Activitate Medie per Canal');
DEFINE('_PT_WHOIN','Momentan pe %s');
DEFINE('_PT_SEARCHCHAN','Cautare Canal');
DEFINE('_PT_USERACT','Activitate Medie Utilizator');
DEFINE('_PT_SEARCHUSER','Cautare Utilizator');
DEFINE('_PT_TSTATS','Activitate Utilizator');
DEFINE('_PT_STATSFOR','Status pentru %s in %s');
DEFINE('_PT_TLDPIE','Statistici Tara pentru %s');
DEFINE('_PT_CLIENTPIE','Statistici Client pentru %s');
DEFINE('_PT_MSTATS','Luna Curenta');
DEFINE('_PT_WSTATS','Saptamana Curenta');
DEFINE('_PT_DSTATS','Astazi');
DEFINE('_PT_SEARCHRESULTS','Rezultatul Cautarii');
DEFINE('_PT_OPERS','Operatori Online');
DEFINE('_PT_NSU2W','Relatiile dintre Utilizatori si Canale din ultimele 2 saptamani');
DEFINE('_PT_NSC2W','Canalele din ultimele 2 saptamani');
DEFINE('_PT_NSS2W','Serverele din ultimele 2 saptamani');
DEFINE('_PT_NSU2M','Relatiile dintre Utilizatori si Canale din ultimii 2 luni');
DEFINE('_PT_NSC2M','Canalele din ultimii 2 luni');
DEFINE('_PT_NSS2M','Serverele din ultimii 2 luni');
DEFINE('_PT_NSU2Y','Relatiile dintre Utilizatori si Canale din ultimii 2 ani');
DEFINE('_PT_NSC2Y','Canalele din ultimii 2 ani');
DEFINE('_PT_NSS2Y','Serverele din ultimii 2 ani');
DEFINE('_PT_NSHYS','Istoria Completa Canale si Utilizatori');

/* Network Status */
DEFINE('_NS_USERDET','Utilizatori momentan: %s Max azi: %s Max: %s Cand: %s');
DEFINE('_NS_CHANDET','Canale momentan: %s Max: %s Cand: %s');
DEFINE('_NS_OPERDET','Operatori momentan: %s Max: %s Cand: %s');
DEFINE('_NS_SERVDET','Servere momentan: %s Max: %s Cand: %s');

/* Search */
DEFINE('_SP_PROMPT','Introdu datele cautarii. Wildcards sunt, de asemeni, suportate.');
DEFINE('_SP_SUBMIT','Cauta');
DEFINE('_SP_CHANINFO','Informatii Canal');
DEFINE('_SP_CHANSTATS','Statistici Canal');
DEFINE('_SP_USERINFO','Informatii Utilizator');
DEFINE('_SP_USERSTATS','Statistici Utilizator');

/* Configuration Page */
DEFINE('_CP_SETSORT','Sorteaza dupa');
DEFINE('_CP_SETTYPE','Listeaza');

/* General Definitions */
DEFINE('_GD_NUMBER','nr.');
DEFINE('_GD_NICK','nickname');
DEFINE('_GD_HOST','hostname');
DEFINE('_GD_CHANNEL','canal');
DEFINE('_GD_USERS','utilizatori');
DEFINE('_GD_LETTERS','cifre');
DEFINE('_GD_WORDS','cuvinte');
DEFINE('_GD_LINES','linii');
DEFINE('_GD_ACTIONS','actiuni');
DEFINE('_GD_SMILEYS','emoticoane');
DEFINE('_GD_KICKS','kickuri');
DEFINE('_GD_MODES','moduri');
DEFINE('_GD_TOPICS','topicuri');
DEFINE('_GD_ON','pe');
DEFINE('_GD_TYPE','tip');
DEFINE('_GD_TOTAL','total');
DEFINE('_GD_OTHER','altele');
DEFINE('_GD_UNKNOWN','necunoscut');
DEFINE('_GD_NEVER','niciodata');
DEFINE('_GD_NONE','nimic');
DEFINE('_GD_STATUS','Status');
DEFINE('_GD_YES','Da');
DEFINE('_GD_NO','Nu');
DEFINE('_GD_HOUR','Ora');
DEFINE('_GD_DAY','Zi');
DEFINE('_GD_MONTH','Luna');
DEFINE('_GD_YEAR','An');
DEFINE('_GD_OR','sau');
DEFINE('_GD_AND','si');

/* Channel Status */
DEFINE('_CS_INFO0','Canal: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Setat de: %s La data: %s');
DEFINE('_CS_INFO3','Utilizatori Momentan: %s Varf: %s pe %s');
DEFINE('_CS_INFO4','Moduri Canal: %s');
DEFINE('_CS_INFO5','Invitatii: %s Banari: %s Exceptii: %s');

/* User Info */
DEFINE('_UI_UINFO','Informatii pentru %s (stats group %s)');
DEFINE('_UI_NODATA','Utilizatorul %s se pare ca este %s si nu sunt infomatii \'stocate\'.');
DEFINE('_UI_UCHANS','Canale Monitorizate: %s');
DEFINE('_UI_WHOIS1','Nickname: %s Nume Real: %s');
DEFINE('_UI_WHOIS2','Masca: %s');
DEFINE('_UI_WHOIS3','Status: %s');
DEFINE('_UI_WHOIS4','Conectat de la: %s');
DEFINE('_UI_WHOIS5','Ultima Iesire: %s');
DEFINE('_UI_WHOIS6','Status Away: %s');
DEFINE('_UI_WHOIS7','Mesaj: %s');
DEFINE('_UI_WHOIS8','Client Folosit: %s');
DEFINE('_UI_WHOIS9','Conectat de la: %s Server: %s');
DEFINE('_UI_WHOIS10','Informatii Aditionale: %s');

/* Oper list */
DEFINE('_OL_NICK','Nickname');
DEFINE('_OL_LEVEL','Nivel Acces');
DEFINE('_OL_ONLINE','Online de');
DEFINE('_OL_LASTQUIT','Ultima data vazut');
DEFINE('_OL_BOT','Serviciu Bot');
DEFINE('_OL_SERVICE','Serviciu de Retea');
DEFINE('_OL_AWAY','Away');
DEFINE('_OL_AVAILABLE','Disponibil pentru Ajutor');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Afisez Canalele %s-%s din %s');
DEFINE('_PS_USERSHOW','Afisez Utilizatorii %s-%s din %s');
DEFINE('_PS_ASCENDING','Ascendent');
DEFINE('_PS_DESCENDING','Descendent');

/* Errors */
DEFINE('_ER_ERROR','EROARE');
DEFINE('_ER_WARNING','ATENTIE');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Nu sunt Date');
DEFINE('_ER_NOINVITES','Nu sunt Invitatii');
DEFINE('_ER_NOBANS','Nu sunt Banari');
DEFINE('_ER_NOEXCEPTS','Nu sunt Exceptii'); 
DEFINE('_ER_NORESULTS','Nu sunt Rezultate');
DEFINE('_ER_MINCHARS','Iti trebuie minimum %s litere pentru a cauta');
DEFINE('_ER_MODE','Operatia Necunoscuta %s');
DEFINE('_ER_NOSTATS','Nu s-au gasit Informatii Stocate');
DEFINE('_ER_NOCHAN','Canalul nu exista');
DEFINE('_ER_EMPTYCHAN','Canalul este momentan gol');
DEFINE('_ER_DENIED','Acces Refuzat');

/* API */
DEFINE('_DA_CURRENTUSR','Utilizatori Momentan');
DEFINE('_DA_MAXUSR','Top Utilizatori');
DEFINE('_DA_SERVER','Nume Server');
DEFINE('_DA_SRVDET1','Rulez %s timp de %s');
DEFINE('_DA_SRVDET2','Sunt %s');
DEFINE('_DA_SRVDET3','de %s');
DEFINE('_DA_SRVDET4','Ultimul Split: %s');
DEFINE('_DA_SRVDET5','Ping: %s Max: %s Cand: %s');
DEFINE('_DA_SRVDET6','Utilizatori: %s Max: %s Cand: %s');
DEFINE('_DA_SRVDET7','Operatori: %s Max: %s Cand: %s');
DEFINE('_DA_SRVDET8','Killuri de catre IrcOp: %s  Killuri de catre Server: %s');
DEFINE('_DA_UPTIME','%s zile si %s ore');

/* Graphs */
DEFINE('_GR_CUSTOM','Grafice Customizate');
DEFINE('_GR_SELECT','Selecteaza...'); 
DEFINE('_GR_GENERATE','Generat');
DEFINE('_GR_SDATE','De la data');
DEFINE('_GR_EDATE','Pina la data');
DEFINE('_GR_OPTIONS','Optiuni'); 
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Med');

/* Other */
DEFINE('_OT_GENTIME','pagina generata in %s secunde in urma a %s interogari sql');
DEFINE('_OT_CGENTIME','pagina compresata generata in %s secunde in urma a %s interogari sql');

?>

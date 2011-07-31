<?php
/* Greek Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	  ToXiC 	<ToXiC@freemail.gr>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'el_GR.UTF-8', 'el_GR.UTF-8@euro', 'el.UTF-8', 'ell.UTF-8', 'gre.UTF-8', 'el_GR@euro', 'el_GR', 'el', 'ell', 'gre');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Arxikh');
DEFINE('_TM_NETWORK','Dyktio');
DEFINE('_TM_GRAPHS','Grafika');
DEFINE('_TM_CHANS','Kanalia');
DEFINE('_TM_USERS','Xrhstes');
DEFINE('_TM_SEARCH','Anazhthsh');
DEFINE('_TM_CONFIG','Epiloges');
DEFINE('_TM_TITLE','Statistika IRC');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Katastash Dyktiou');
DEFINE('_SM_NETDETAILS','Leptomeries');
DEFINE('_SM_SERVERS','Diakomistes');
DEFINE('_SM_SRVLIST','Lista Diakomistwn');
DEFINE('_SM_SRVDETAILS','Leptomeries Diakomistwn');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Grafika Dyktiou');
DEFINE('_SM_CUSTOM','Idiokataskeuh...');
DEFINE('_SM_NETSPLIT','Grafika NetSplit.de');
DEFINE('_SM_SEARCHIRC','Grafika SearchIRC');
DEFINE('_SM_CHANLIST','Lista Kanaliwn');
DEFINE('_SM_TOPCHANS','Statistika Kanaliwn');
DEFINE('_SM_SEARCHCHAN','Anazhthsh Kanaliou...');
DEFINE('_SM_CHANNELSTATUS','Katastash Kanaliou');
DEFINE('_SM_CHANNELACT','Energotita Kanaliou');
DEFINE('_SM_WHOIN','Pios Einai Sindedemenos');
DEFINE('_SM_TOPUSERS','Statistika Xrhsth');
DEFINE('_SM_SEARCHUSER','Anazhthsh Xrhsth...');
DEFINE('_SM_GLOBALSTATS','Genika Statistika');
DEFINE('_SM_CHANSTATS','Statistika Kanaliwn');
DEFINE('_SM_CHANINFO','Plirofories Kanaliwn');
DEFINE('_SM_TLDPIE','Statistika Xwrwn');
DEFINE('_SM_CLIENTPIE','Statistika Pelatwn');
DEFINE('_SM_INVITES','Ekseresi Prosklisewn Kanaliwn');
DEFINE('_SM_BANS','Apagorefseis Kanaliwn');
DEFINE('_SM_EXCEPTS','Ekseresi Apagorefsewn Kanaliwn');
DEFINE('_SM_TSTATS','Genika');
DEFINE('_SM_MSTATS','Afto To Mina');
DEFINE('_SM_WSTATS','Afth Thn Evdomada');
DEFINE('_SM_DSTATS','Shmera');
DEFINE('_SM_ASTATS','Hmerhsios Mesos Oros');
DEFINE('_SM_LANGUAGE','Glwssa');
DEFINE('_SM_THEME','Thema');
DEFINE('_SM_SET','Efarmogh');
DEFINE('_SM_BEHAVIOR','Epiloges Provolhs');
DEFINE('_SM_OPERS','Xeiristes');
DEFINE('_SM_PYSTATS','Ton Prohgoumeno Xrono');
DEFINE('_SM_PMSTATS','Ton Prohgoumeno Mhna');
DEFINE('_SM_PWSTATS','Thn Prohgoumenh Evdomada');
DEFINE('_SM_NSWEEKS','Teleytaies 2 Evdomades');
DEFINE('_SM_NSMONTHS','Teleytaia 2 Mhna');
DEFINE('_SM_NSYEARS','Teleytaia 2 Xronia');
DEFINE('_SM_NSHISTORY','Plhres Istoriko');

/* Front Page */
DEFINE('_FP_RANKING','Trexousa Taksinomish');
DEFINE('_FP_BIGGESTCHANS','Ta %s Pio Megala Kanalia');
DEFINE('_FP_TOPCHANS','Ta %s Megalitera Kanalia Simera');
DEFINE('_FP_TOPUSERS','Oi %s Poio Energoi Xrhstes Simera');
DEFINE('_FP_STATUS','Katastash Dyktiou');
DEFINE('_FP_MAXSTATS','Megista Statistika');
DEFINE('_FP_USERS','Xrhstes:');
DEFINE('_FP_CHANS','Kanalia:');
DEFINE('_FP_OPERS','Xeiristes:');
DEFINE('_FP_SERVERS','Diakosmites:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','GRIGORH PLOHGHSH');
DEFINE('_FP_QSTATS','GRIGORA STATISTIKA');
DEFINE('_FP_LSTATUS','STATISTIKA AFTH TH STIGMH');
DEFINE('_FP_QUSER','Xrhsths');
DEFINE('_FP_QCHAN','Kanali');
DEFINE('_FP_GO','Ksekina');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Katastash Dyktiou');
DEFINE('_PT_SERVERS','Lista Diakomistwn');
DEFINE('_PT_DETAILS','Leptomeries Diakomisth gia %s');
DEFINE('_PT_MOTD','Minima Ths Hmeras (MOTD)');
DEFINE('_PT_CHANLIST','Lista Kanaliwn (/list)');
DEFINE('_PT_TCSTATS','Energotita Kanaliwn');
DEFINE('_PT_CHANNELACT','Mesos Oros Energotitas Kanaliwn');
DEFINE('_PT_WHOIN','Afth Th Stigmh Sto %s');
DEFINE('_PT_SEARCHCHAN','Anazhthsh Kanaliwn');
DEFINE('_PT_USERACT','Mesos Oros Energotitas Xrhstwn');
DEFINE('_PT_SEARCHUSER','Anazhthsh Xrhstwn');
DEFINE('_PT_TSTATS','Energotita Xrhstwn');
DEFINE('_PT_STATSFOR','Statistika Gia %s sto %s');
DEFINE('_PT_TLDPIE','Statistika Xwrwn gia %s');
DEFINE('_PT_CLIENTPIE','Statistika Pelatwn Gia %s');
DEFINE('_PT_MSTATS','Afto To Mhna');
DEFINE('_PT_WSTATS','Afth Thn Evdomada');
DEFINE('_PT_DSTATS','Shmera');
DEFINE('_PT_SEARCHRESULTS','Apotelesmata Anazhthshs');
DEFINE('_PT_OPERS','Opers Afth Th Stigmh');
DEFINE('_PT_NSU2W','Sxesh Twn Xrhstwn Kai Twn Kanaliwn Kata Th Diarkeia Twn Teleutaiwn 2 Evdomadwn');
DEFINE('_PT_NSC2W','Kanalia Tis Teleytaies 2 Evdomades');
DEFINE('_PT_NSS2W','Diakomistes Tis Teleytaies 2 Evdomades');
DEFINE('_PT_NSU2M','Sxesh Twn Xrhstwn Kai Twn Kanaliwn Kata Th Diarkeia Twn Teleutaiwn 2 Mhnwn');
DEFINE('_PT_NSC2M','Kanalia Tis Teleytaies 2 Mhna');
DEFINE('_PT_NSS2M','Diakomistes Tis Teleytaies 2 Mhna');
DEFINE('_PT_NSU2Y','Sxesh twn Xrhstwn kai Twn Kanaliwn Kata Th Diarkeia Twn Teleutaiwn 2 Xronwn');
DEFINE('_PT_NSC2Y','Kanalia Ta Teleytaia 2 Xronia');
DEFINE('_PT_NSS2Y','Diakomistes Ta Teleytaia 2 Xronia');
DEFINE('_PT_NSHYS','Plhrhs Katametrhmenh Istoria Gia Tous Xrhstes Kai Ta Kanalia');

/* Network Status */
DEFINE('_NS_USERDET','Twrinoi Xrhstes: %s Megisto Shmera: %s Megisto: %s ( %s )');
DEFINE('_NS_CHANDET','Twrina Kanalia: %s Megisto: %s (%s)');
DEFINE('_NS_OPERDET','Twrinoi Xeiristes: %s Megisto: %s ( %s )');
DEFINE('_NS_SERVDET','Twrinoi Servers: %s Megisto: %s ( %s )');

/* Search */
DEFINE('_SP_PROMPT','Dwste Ta Kritiria Anazhthshs. Ta Wildcards ypostirizontai.');
DEFINE('_SP_SUBMIT','Anazhthsh');
DEFINE('_SP_CHANINFO','Plirofories Kanaliwn');
DEFINE('_SP_CHANSTATS','Statistika Kanaliwn');
DEFINE('_SP_USERINFO','Plirofories Xrhstwn');
DEFINE('_SP_USERSTATS','Statistika Xrhstwn');

/* Configuration Page */
DEFINE('_CP_SETSORT','Dwste Ta Krithria Sintomefshs');
DEFINE('_CP_SETTYPE','Epilekste Ton Proepilegmeno Typo Statistikwn');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','psevdonimo');
DEFINE('_GD_HOST','diefthinsi');
DEFINE('_GD_CHANNEL','kanali');
DEFINE('_GD_USERS','xrhstes');
DEFINE('_GD_LETTERS','grammata');
DEFINE('_GD_WORDS','lekseis');
DEFINE('_GD_LINES','grammes');
DEFINE('_GD_ACTIONS','energies');
DEFINE('_GD_SMILEYS','xamogela');
DEFINE('_GD_KICKS','kicks');
DEFINE('_GD_MODES','modes');
DEFINE('_GD_TOPICS','topics');
DEFINE('_GD_ON','on');
DEFINE('_GD_TYPE','tipos');
DEFINE('_GD_TOTAL','genika');
DEFINE('_GD_OTHER','alla');
DEFINE('_GD_UNKNOWN','agnosto');
DEFINE('_GD_NEVER','pote');
DEFINE('_GD_NONE','tipota');
DEFINE('_GD_STATUS','Katastash');
DEFINE('_GD_YES','Nai');
DEFINE('_GD_NO','Oxi');
DEFINE('_GD_HOUR','Wra');
DEFINE('_GD_DAY','Mera');
DEFINE('_GD_MONTH','Mhnas');
DEFINE('_GD_YEAR','Etos');
DEFINE('_GD_OR','Eitai');
DEFINE('_GD_AND','Kai');

/* Channel Status */
DEFINE('_CS_INFO0','Kanali: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Graftike Apo Ton: %s Stis: %s');
DEFINE('_CS_INFO3','Twrinoi Xrhstes: %s Megisto: %s Stis %s');
DEFINE('_CS_INFO4','Modes Kanaliou: %s');
DEFINE('_CS_INFO5','Eksereseis Prosklisewn: %s Apagorefseis: %s Eksereseis Apagorefsewn: %s');

/* User Info */
DEFINE('_UI_UINFO','Plhrofories Xrhsth Gia To %s (stats group %s)');
DEFINE('_UI_NODATA','O Xrhsths %s fenetai san %s kai den iparxoun apothikevmenes plhrofories.');
DEFINE('_UI_UCHANS','Epivlepomena Kanalia: %s');
DEFINE('_UI_WHOIS1','Psevdonima: %s Alithino Onoma: %s');
DEFINE('_UI_WHOIS2','Maska: %s');
DEFINE('_UI_WHOIS3','Katastash: %s');
DEFINE('_UI_WHOIS4','Sindedemenos Apo: %s');
DEFINE('_UI_WHOIS5','Teleftea Eksodos: %s');
DEFINE('_UI_WHOIS6','Katastash Away: %s');
DEFINE('_UI_WHOIS7','Minima: %s');
DEFINE('_UI_WHOIS8','Xrisimopoieitai Apo: %s');
DEFINE('_UI_WHOIS9','Sindedemenos Apo: %s Sto Diakomisth: %s');
DEFINE('_UI_WHOIS10','Epiprosthetes Plirofories: %s');

/* Oper list */
DEFINE('_OL_NICK','Psevdonimo');
DEFINE('_OL_LEVEL','Epipedo');
DEFINE('_OL_ONLINE','Sindedemenos Apo Tote');
DEFINE('_OL_LASTQUIT','Teleftea Eksodos');
DEFINE('_OL_BOT','Ipiresia Bot');
DEFINE('_OL_SERVICE','Ipiresia Dyktiou');
DEFINE('_OL_AWAY','Away');
DEFINE('_OL_AVAILABLE','Diathesimos Gia Voithia');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Provalontai Ta Kanalia %s-%s apo %s');
DEFINE('_PS_USERSHOW','Provalontai Oi Xrhstes %s-%s apo %s');
DEFINE('_PS_ASCENDING','Anodika');
DEFINE('_PS_DESCENDING','Kathodika');

/* Errors */
DEFINE('_ER_ERROR','LATHOS');
DEFINE('_ER_WARNING','PROIDOPOIHSH');
DEFINE('_ER_INFO','PLHROFORIES');
DEFINE('_ER_NODATA','Oxi Dedomena');
DEFINE('_ER_NOINVITES','Oxi Eksereseis Prosklhsewn');
DEFINE('_ER_NOBANS','Oxi Apagorefseis');
DEFINE('_ER_NOEXCEPTS','Oxi Eksereseis Apagorefsewn');
DEFINE('_ER_NORESULTS','Oxi Apotelesmata');
DEFINE('_ER_MINCHARS','Xriazesai Toulaxiston %s Xarakthres Gia Anazhthsh');
DEFINE('_ER_MODE','The requested operation mode %s is unknown');
DEFINE('_ER_NOSTATS','Den Iparxoun Sigedromena Statistika Gia Afton Ton Xrhsth/Kanali');
DEFINE('_ER_NOCHAN','Afto To Kanali Den Iparxi');
DEFINE('_ER_EMPTYCHAN','Afto To Kanali Einai Adio');
DEFINE('_ER_DENIED','Den Rpitrapike H Prosvash');

/* API */
DEFINE('_DA_CURRENTUSR','Xrhstes Afti Ti Stigmh');
DEFINE('_DA_MAXUSR','Megistoi Xrhstes');
DEFINE('_DA_SERVER','Onoma Diakomisth');
DEFINE('_DA_SRVDET1','Trexw %s Energo Apo %s');
DEFINE('_DA_SRVDET2','Afth Th Stigmh %s');
DEFINE('_DA_SRVDET3','apo %s');
DEFINE('_DA_SRVDET4','To Telefteo Split: %s');
DEFINE('_DA_SRVDET5','Twrino Ping: %s  Megisto: %s ( %s )');
DEFINE('_DA_SRVDET6','Twrinoi Xrhstes: %s  Max: %s ( %s )');
DEFINE('_DA_SRVDET7','Twrinoi Xeiristes: %s  Max: %s ( %s )');
DEFINE('_DA_SRVDET8','Kills Apo Xeiristes: %s  Kills Apo Diakomhstes: %s');
DEFINE('_DA_UPTIME','%s meres kai %s wres');

/* Graphs */
DEFINE('_GR_CUSTOM','Grafika Idiokataskeuhs');
DEFINE('_GR_SELECT','Epilekse...');
DEFINE('_GR_GENERATE','Dimiourgise');
DEFINE('_GR_SDATE','Hmerominia Enarkshs');
DEFINE('_GR_EDATE','Hmerominia Telous');
DEFINE('_GR_OPTIONS','Epiloges');
DEFINE('_GR_MIN','Mikrotero');
DEFINE('_GR_MAX','Megisto');
DEFINE('_GR_AVG','Mesos Oros');

/* Other */
DEFINE('_OT_GENTIME','H Selida Dimiourgithike Se %s Defterolepta Me %s Aithseis SQL');
DEFINE('_OT_CGENTIME','H Sympiesmenh Selida Dimiourgithike se %s Defterolepta Me %s Aithseis SQL');

?>

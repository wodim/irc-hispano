<?php
/* French Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	Trinity		<trinity@united-irc.com>
Updates:	Mateo		<mathieuduluc@free.fr>
			Elephantman <elephantman@europnet.org>
			Psychik		<psychik@neurotropic.info>
			illu		<illu@icedslash.org>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'fr_FR.UTF-8@euro', 'fr_FR.UTF-8', 'fr.UTF-8', 'fra.UTF-8', 'fre.UTF-8', 'fr_FR@euro', 'fr_FR', 'fr', 'fra', 'fre');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Accueil');
DEFINE('_TM_NETWORK','Réseau');
DEFINE('_TM_GRAPHS','Graphiques');
DEFINE('_TM_CHANS','Canaux');
DEFINE('_TM_USERS','Utilisateurs');
DEFINE('_TM_SEARCH','Chercher');
DEFINE('_TM_CONFIG','Configuration');
DEFINE('_TM_TITLE','Statistiques IRC');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Statut Réseau');
DEFINE('_SM_NETDETAILS','Details');
DEFINE('_SM_SERVERS','Serveurs');
DEFINE('_SM_SRVLIST','Liste de Serveurs');
DEFINE('_SM_SRVDETAILS','Details Serveur');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Stats du réseau');
DEFINE('_SM_CUSTOM','Personnalisé...');
DEFINE('_SM_NETSPLIT','Graphiques NetSplit.de');
DEFINE('_SM_SEARCHIRC','Graphiques SearchIRC');
DEFINE('_SM_CHANLIST','Liste de Chans');
DEFINE('_SM_TOPCHANS','Stats du Chan');
DEFINE('_SM_SEARCHCHAN','Chercher Chan...');
DEFINE('_SM_CHANNELSTATUS','Status du Chan');
DEFINE('_SM_CHANNELACT','Activité du Chan');
DEFINE('_SM_WHOIN','Qui est connecté?');
DEFINE('_SM_TOPUSERS','Stats Utilisateurs');
DEFINE('_SM_SEARCHUSER','Chercher Utilisateur...');
DEFINE('_SM_GLOBALSTATS','Stats Globales');
DEFINE('_SM_CHANSTATS','Stats Chans');
DEFINE('_SM_CHANINFO','Informations du Chan');
DEFINE('_SM_TLDPIE','Statistiques Pays');
DEFINE('_SM_CLIENTPIE','Statistiques Clients');
DEFINE('_SM_INVITES','Exceptions de Invitations');
DEFINE('_SM_BANS','Bannis');
DEFINE('_SM_EXCEPTS','Exceptions de Bannis');
DEFINE('_SM_TSTATS','Total');
DEFINE('_SM_MSTATS','Ce mois');
DEFINE('_SM_WSTATS','Cette semaine');
DEFINE('_SM_DSTATS','Aujourd\'hui');
DEFINE('_SM_ASTATS','Moyenne journalière');
DEFINE('_SM_LANGUAGE','Langue');
DEFINE('_SM_THEME','Thème');
DEFINE('_SM_SET','Appliquer');
DEFINE('_SM_BEHAVIOR','Comportement');
DEFINE('_SM_OPERS','Opers');
DEFINE('_SM_PYSTATS','Année passée');
DEFINE('_SM_PMSTATS','Mois passé');
DEFINE('_SM_PWSTATS','Semaine passée');
DEFINE('_SM_NSWEEKS','Deux dernières semaines');
DEFINE('_SM_NSMONTHS','Deux dernières mois');
DEFINE('_SM_NSYEARS','Deux dernières années');
DEFINE('_SM_NSHISTORY','Historique complet');

/* Front Page */
DEFINE('_FP_RANKING','Position actuelle');
DEFINE('_FP_BIGGESTCHANS','Top %s des Chans courant');
DEFINE('_FP_TOPCHANS','Top %s Chans aujourd\'hui');
DEFINE('_FP_TOPUSERS','Top %s Utilis. aujourd\'hui');
DEFINE('_FP_STATUS','Status Réseau');
DEFINE('_FP_MAXSTATS','Stats Max');
DEFINE('_FP_USERS','Utilisateurs:');
DEFINE('_FP_CHANS','Chans:');
DEFINE('_FP_OPERS','Opers:');
DEFINE('_FP_SERVERS','Serveurs:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Navigation Rapide');
DEFINE('_FP_QSTATS','Access Direct'); 
DEFINE('_FP_LSTATUS','Status Actuel'); 
DEFINE('_FP_QUSER','Nick');
DEFINE('_FP_QCHAN','Chan');
DEFINE('_FP_GO','Go');
   
/* Titles on Pages */
DEFINE('_PT_LUSERS','Status Réseau');
DEFINE('_PT_SERVERS','Liste des Serveurs');
DEFINE('_PT_DETAILS','Server Details pour %s');
DEFINE('_PT_MOTD','Message du Jour (MOTD)');
DEFINE('_PT_CHANLIST','Liste des Chans (/list)');
DEFINE('_PT_TCSTATS','Activité des Chans');
DEFINE('_PT_CHANNELACT','Activité Moyenne du Chan');
DEFINE('_PT_WHOIN','Actuellement sur %s');
DEFINE('_PT_SEARCHCHAN','Recherche Chan');
DEFINE('_PT_USERACT','Activité de l\'utilisateur en moyenne');
DEFINE('_PT_SEARCHUSER','Recherche utilisateur');
DEFINE('_PT_TSTATS','Activité des Utilisateurs');
DEFINE('_PT_STATSFOR','Stats pour %s sur %s');
DEFINE('_PT_TLDPIE','Statistiques Pays %s');
DEFINE('_PT_CLIENTPIE','Statistiques Client pour %s');
DEFINE('_PT_MSTATS','ce mois');
DEFINE('_PT_WSTATS','cette semaine');
DEFINE('_PT_DSTATS','aujourd hui');
DEFINE('_PT_SEARCHRESULTS','Résultats Recherche');
DEFINE('_PT_OPERS','Opers Actuels');
DEFINE('_PT_NSU2W','Rapport des utilisateurs et cannaux des 2 dernières semaines');
DEFINE('_PT_NSC2W','Cannaux ces 2 dernières semaines');
DEFINE('_PT_NSS2W','Serveurs ces 2 dernières semaines');
DEFINE('_PT_NSU2M','Rapport des utilisateurs et cannaux des 2 dernières mois');
DEFINE('_PT_NSC2M','Cannaux ces 2 dernières mois');
DEFINE('_PT_NSS2M','Serveurs ces 2 dernières mois');
DEFINE('_PT_NSU2Y','Rapport des utilisateurs et cannaux des 2 dernières années');
DEFINE('_PT_NSC2Y','Cannaux ces 2 dernières années');
DEFINE('_PT_NSS2Y','Serveurs ces 2 dernieres années');
DEFINE('_PT_NSHYS','Mesures complètes à propos des utilisateurs et cannaux');

/* Network Status */
DEFINE('_NS_USERDET','Utilisateurs actuels: %s Max aujourd\'hui: %s Max: %s le %s');
DEFINE('_NS_CHANDET','Chans actuels: %s Max: %s le %s');
DEFINE('_NS_OPERDET','Opers actuels: %s Max: %s le %s');
DEFINE('_NS_SERVDET','Serveurs actuels: %s Max: %s le %s');

/* Search */
DEFINE('_SP_PROMPT','Entrez un critere de recherche. Jokers accepté.');
DEFINE('_SP_SUBMIT','Chercher');
DEFINE('_SP_CHANINFO','Channel Info');
DEFINE('_SP_CHANSTATS','Channel Stats');
DEFINE('_SP_USERINFO','User Info');
DEFINE('_SP_USERSTATS','User Stats');

/* Configuration Page */
DEFINE('_CP_SETSORT','Choisissez le critère de classement par défaut');
DEFINE('_CP_SETTYPE','Choisissez le type de statistiques par défaut');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','nick');
DEFINE('_GD_HOST','hôte');
DEFINE('_GD_CHANNEL','chan');
DEFINE('_GD_USERS','utilisateurs');
DEFINE('_GD_LETTERS','caractères');
DEFINE('_GD_WORDS','mots');
DEFINE('_GD_LINES','lignes');
DEFINE('_GD_ACTIONS','actions');
DEFINE('_GD_SMILEYS','smileys');
DEFINE('_GD_KICKS','kicks');
DEFINE('_GD_MODES','modes');
DEFINE('_GD_TOPICS','sujets');
DEFINE('_GD_ON','sur');
DEFINE('_GD_TYPE','type');
DEFINE('_GD_TOTAL','total');
DEFINE('_GD_OTHER','autres');
DEFINE('_GD_UNKNOWN','inconnu');
DEFINE('_GD_NEVER','jamais');
DEFINE('_GD_NONE','Aucun');
DEFINE('_GD_STATUS','Etat');
DEFINE('_GD_YES','Oui');
DEFINE('_GD_NO','Non');
DEFINE('_GD_HOUR','Heure');
DEFINE('_GD_DAY','Jour');
DEFINE('_GD_MONTH','Mois');
DEFINE('_GD_YEAR','Année');
DEFINE('_GD_OR','ou');
DEFINE('_GD_AND','et');

/* Channel Status */
DEFINE('_CS_INFO0','Chan: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Entré par: %s Le: %s');
DEFINE('_CS_INFO3','Utilisateurs actuels: %s Max: %s sur %s');
DEFINE('_CS_INFO4','Modes du Chan: %s');
DEFINE('_CS_INFO5','Exceptions d\'Invitations: %s Bans: %s Exceptions de Bans: %s');

/* User Info */
DEFINE('_UI_UINFO','Informations pour le nickname %s (groupe stats %s)');
DEFINE('_UI_NODATA','L\'utilisateur %s semble %s et il n\'y a pas d\'information en cache.');
DEFINE('_UI_UCHANS','Chans monitorés: %s');
DEFINE('_UI_WHOIS1','Pseudo: %s Nom réel: %s');
DEFINE('_UI_WHOIS2','Masque: %s');
DEFINE('_UI_WHOIS3','Etat: %s');
DEFINE('_UI_WHOIS4','Connecté depuis: %s');
DEFINE('_UI_WHOIS5','Dernier quit: %s');
DEFINE('_UI_WHOIS6','Absent: %s');
DEFINE('_UI_WHOIS7','Message: %s');
DEFINE('_UI_WHOIS8','Client utilisé: %s');
DEFINE('_UI_WHOIS9','Connecté depuis: %s Sur le serveur: %s');
DEFINE('_UI_WHOIS10','Information additionnel: %s');

/* Oper list */
DEFINE('_OL_NICK','Pseudo');
DEFINE('_OL_LEVEL','Statut');
DEFINE('_OL_ONLINE','Online depuis');
DEFINE('_OL_LASTQUIT','Dernier Quit');
DEFINE('_OL_BOT','Service Bot');
DEFINE('_OL_SERVICE','Service du Réseau');
DEFINE('_OL_AWAY','Absent');
DEFINE('_OL_AVAILABLE','Disponible pour Aide');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Chans %s-%s sur %s');
DEFINE('_PS_USERSHOW','Utilisateurs %s-%s sur %s');
DEFINE('_PS_ASCENDING','Croissant');
DEFINE('_PS_DESCENDING','Décroissant');

/* Errors */
DEFINE('_ER_ERROR','ERREUR');
DEFINE('_ER_WARNING','ATTENTION');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Aucune Donnée');
DEFINE('_ER_NOINVITES','Aucune Exception d\'Invitation');
DEFINE('_ER_NOBANS','Aucun Ban');
DEFINE('_ER_NOEXCEPTS','Aucune Exception de Ban');
DEFINE('_ER_NORESULTS','Aucun Résultat');
DEFINE('_ER_MINCHARS','Il faut au moins %s caractères pour rechercher');
DEFINE('_ER_MODE','Le mode %s est inconnu');
DEFINE('_ER_NOSTATS','Aucune donnée statistique collectée pour cet utilisateur/chan');
DEFINE('_ER_NOCHAN','Ce chan n\'existe pas');
DEFINE('_ER_EMPTYCHAN','Ce chan est actuellement vide');
DEFINE('_ER_DENIED','Accès refusé');

/* API */
DEFINE('_DA_CURRENTUSR','utilisateurs actuellement');
DEFINE('_DA_MAXUSR','max utilisateurs');
DEFINE('_DA_SERVER','nom du serveur');
DEFINE('_DA_SRVDET1','Version %s. En ligne depuis %s');
DEFINE('_DA_SRVDET2','Actuellement %s');
DEFINE('_DA_SRVDET3','depuis %s');
DEFINE('_DA_SRVDET4','Dernier split: %s');
DEFINE('_DA_SRVDET5','Ping actuel: %s  Max: %s sur %s');
DEFINE('_DA_SRVDET6','Utilisateurs actuels: %s  Max: %s sur %s');
DEFINE('_DA_SRVDET7','Opers actuellement: %s  Max: %s sur %s');
DEFINE('_DA_SRVDET8','Kills par Ircop: %s  Kills par Serveur: %s');
DEFINE('_DA_UPTIME','%s jours et %s heures');

/* Graphs */
DEFINE('_GR_CUSTOM','Stats personnalisées');
DEFINE('_GR_SELECT','Au choix..');
DEFINE('_GR_GENERATE','Générer');
DEFINE('_GR_SDATE','Date début');
DEFINE('_GR_EDATE','Date fin');
DEFINE('_GR_OPTIONS','Options');
DEFINE('_GR_MIN','Minimale');
DEFINE('_GR_MAX','Maximale');
DEFINE('_GR_AVG','Moyenne');

/* Other */
DEFINE('_OT_GENTIME','Page générée en  %s secondes avec %s requetes sql');
DEFINE('_OT_CGENTIME','Page compressé générée en %s secondes avec %s requetes sql');

?>

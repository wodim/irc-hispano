# French language file.
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
# $Id: fr.l 391 2011-01-11 14:22:48Z hal9000 $
#
# French tracution by XaF
# Traduction Fran�aise par XaF
#			(root@sbird.org - irc.sbird.org)
# Revision by illu (11/07/06)
# Revu par illu (07/11/06)
#                       (illu@icedslash.org - irc.icedslash.org)
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
	Fran�ais (French)

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
	%d %b %H:%M:%S %Y %Z
# This is used as the format string for strftime() for a date alone in long
# format (English: including weekday).
STRFTIME_LONG_DATE_FORMAT
	%a %d %b %Y
# This is used as the format string for strftime() for a date alone in
# short format (English: without weekday).
STRFTIME_SHORT_DATE_FORMAT
	%d %b %Y

# These tell our strftime() what the names of months and days are.  If you
# don't use %a, %A, %b, or %B in your strftime() strings above, you can
# leave these empty.  However, if you enter names, they MUST stay in order,
# one per line, and the list MUST be complete!

# %a
STRFTIME_DAYS_SHORT
	Dim
	Lun
	Mar
	Mer
	Jeu
	Ven
	Sam
# %A
STRFTIME_DAYS_LONG
	Dimanche
	Lundi
	Mardi
	Mercredi
	Jeudi
	Vendredi
	Samedi
# %b
STRFTIME_MONTHS_SHORT
	Jan
	Fev
	Mar
	Avr
	Mai
	Juin
	Juil
	Aou
	Sep
	Oct
	Nov
	Dec
# %B
STRFTIME_MONTHS_LONG
	Janvier
	F�vrier
	Mars
	Avril
	Mai
	Juin
	Juillet
	Ao�t
	Septembre
	Octobre
	Novembre
	D�cembre


# This is used in ChanServ/NickServ INFO displays.
COMMA_SPACE
	, 


# Various error messages.
USER_RECORD_NOT_FOUND
	Erreur interne - impossible de proc�der � la requ�te.
UNKNOWN_COMMAND
	Commande %s inconnue.
UNKNOWN_COMMAND_HELP
	Commande %s inconnue. Tapez "/msg %s HELP" pour obtenir de l'aide.
SYNTAX_ERROR
	Synthaxe: %s
MORE_INFO
	/msg %s HELP %s pour plus d'informations.
NO_HELP_AVAILABLE
	Aucune aide disponible sur %s.

SERVICE_OFFLINE
	%s sont actuellement hors-ligne.

SQL_DISABLED
	Vous ne pouvez pas utiliser cette commande, le code SQL est d�sactiv�.

STATS_UPTIME_DHMS
	En ligne depuis %d jours, %02d:%02d:%02d
STATS_UPTIME_1DHMS
	En ligne depuis %d jour, %02d:%02d:%02d
STATS_UPTIME_HMS
	En ligne depuis %d heures, %d minutes, %d secondes
STATS_UPTIME_HM1S
	En ligne depuis %d heures, %d minutes, %d seconde
STATS_UPTIME_H1MS
	En ligne depuis %d heures, %d minute, %d secondes
STATS_UPTIME_H1M1S
	En ligne depuis %d heures, %d minute, %d seconde
STATS_UPTIME_1HMS
	En ligne depuis %d heure, %d minute, %d secondes
STATS_UPTIME_1HM1S
	En ligne depuis %d heure, %d minutes, %d seconde
STATS_UPTIME_1H1MS
	En ligne depuis %d heure, %d minute, %d secondes
STATS_UPTIME_1H1M1S
	En ligne depuis %d heure, %d minute, %d seconde
STATS_UPTIME_MS
	En ligne depuis %d minutes, %d secondes
STATS_UPTIME_M1S
	En ligne depuis %d minutes, %d seconde
STATS_UPTIME_1MS
	En ligne depuis %d minute, %d secondes
STATS_UPTIME_1M1S
	En ligne depuis %d minute, %d seconde

# What's the difference between "Access denied" and "Permission denied"?
# Very little; you can safely make them the same message with no real loss
# of meaning.  If you want to make a distinction, "Access denied" is
# usually used more generally; for example, a non-oper trying to access
# StatServ gets "Access denied", while a non-Stas admin trying to use
# StatServ RESTART gets "Permission denied".
ACCESS_DENIED
	Acc�s refus�.
PERMISSION_DENIED
	Permission refus�e.

# Oh yeah, too simple messages, but useful. Here yes means yes and
# no means no, so take it easy.
SAY_YES
	Oui
SAY_NO
	Non

###########################################################################
#
# StatServ messages
#
###########################################################################

# RESTART responses
STAT_CANNOT_RESTART
	STATS_BIN n'est pas d�fini; impossible de restart. Relancez le script \2configure\2
	et recompilez Denora pour activer la commande RESTART.

# Module strings

STAT_MODULE_LOADED
	Module %s charg�
STAT_MODULE_UNLOADED
	Module %s d�charg�
STAT_MODULE_LOAD_FAIL
	Impossible de charger le module %s
STAT_MODULE_REMOVE_FAIL
	Impossible de d�charger le module %s
STAT_MODULE_LOAD_SYNTAX
	MODLOAD nom du fichier
STAT_MODULE_UNLOAD_SYNTAX
	MODUNLOAD nom du fichier
STAT_MODULE_NO_UNLOAD
	Ce module ne peux pas �tre d�charg�.
STAT_MODULE_LIST_HEADER
	Liste des modules actuellement charg�s:
STAT_MODULE_LIST
	Module: %s [%s] [%s]
STAT_MODULE_LIST_FOOTER
	%d Modules charg�s.
STAT_MODULE_INFO_LIST
	Module: %s Version: %s Auteur: %s Charg�: %s
STAT_MODULE_CMD_LIST
	Commande ajout�e: /msg %s %s
STAT_MODULE_MSG_LIST
	Providing IRCD handler for: %s
STAT_MODULE_NO_LIST
	Aucun module charg� actuellement
STAT_MODULE_NO_INFO
	Aucune information sur le module %s n'est disponible
STAT_MODULE_INFO_SYNTAX
	MODINFO nom du fichier
MODULE_HELP_HEADER
	Les commandes suivantes ont �t� charg�es par un module:

###########################################################################
#
# StatServ help messages
#
###########################################################################

STAT_HELP
	%S commands:
	    STATUS       Afficher les statistiques du r�seau
	    LOGIN       Vous identifier comme un Administrateur des Stats
	    LOGOUT      Retirer votre statut d'admin

STAT_HELP_STATUS
	Synthaxe: STATUS
	
	Affiche le nombre de serveurs, salons et utilisateurs
	connect�s au r�seau actuellement.

STAT_HELP_SHUTDOWN
	Synthaxe: SHUTDOWN
	
	Arr�te les services apr�s avoir sauvegard� toutes les bases de donn�es.
	
	Limit� aux Administrateurs des Stats.

STAT_HELP_RESTART
	Synthaxe: RESTART
	
	Relance Denora.
	(i.e. coupe puis relance imm�diatement l'ex�cutable).

STAT_HELP_RELOAD
	Synthaxe: RELOAD
	
	Permet de recharger le fichier de configuration de Denora.

STAT_HELP_ADMIN_CMD
	Commandes r�serv�es aux Administrateurs des Stats:
	    ADMIN       Ajoute/Supprime des administrateurs
	    CHANSTATS   Ajoute un salon � monitorer
	    ENVINFO     Voir les informations sur l'environement
	    EXCLUDE     Exclus des utilisateurs des chanstats
	    EXPORT      Exporte les donn�es dans un fichier XML
	    HTML        Force la cr�ation du fichier HTML
	    MODLOAD     Charge un module
	    MODUNLOAD   D�charge un module
	    MODLIST     Liste les modules charg�s
	    MODINFO     Informations sur un module charg�
	    RESTART     Relance Denora
	    RELOAD      Recharger le fichier de configuration
	    SET         Configure les options
	    SHUTDOWN    Arr�tes Denora en sauvegardant les bdd

STAT_HELP_MODLOAD
	Synthaxe: MODLOAD nom du fichier

	Cette commande charge le module nomm� "nom du fichier" �tant
	dans le dossier "modules".

	Limit� aux Administrateurs des Stats.

STAT_HELP_MODUNLOAD
	Synthaxe: MODUNLOAD nom du fichier

	Cette commande d�charge le module nomm� "nom du fichier" �tant
	dans le dossier "modules".

	Limit� aux Administrateurs des Stats.

STAT_HELP_MODINFO
	Synthaxe: MODINFO nom du fichier

	Cette commande affiche les informations sur le module charg�
	sp�cifi�.

	Limit� aux Administrateurs des Stats.

STAT_HELP_MODLIST
	Synthaxe: MODLIST

	Liste les modules charg�s actuellement.

	Limit� aux Administrateurs des Stats.

STAT_HELP_EXCLUDE
	Synthaxe: EXCLUDE {ADD|DEL|LIST} cible
	
	Exclus la cible des statistiques.
	
	Limit� aux Administrateurs des Stats.

STAT_HELP_EXPORT
	Syntax: EXPORT quoi
	
	Exporte la donn�e requise vers XML, quoi
	est une des options suivantes:
	
	channels,users,tld,servers,stats,all
	
	Limit� aux Aministrateurs des Stats

STAT_HELP_HTML
	Synthaxe: HTML
	
	Force la cr�ation du fichier HTML

STAT_HELP_USERDEBUG
	Synthaxe: USERDEBUG utilisateur 
	
	Retourne les informations de DEBUG sur l'utilisateur
	donn�.

STAT_HELP_LOGIN
	Synthaxe: LOGIN name password
	
	Vous identifier comme un Administrateur des Stats

STAT_HELP_LOGOUT
	Synthaxe: LOGOUT
	
	Retirer votre statut d'admin

STAT_HELP_SET
	Synthaxe: SET OPTION [ON|OFF]

	Diverses options avec r�gles d'ex�cution.

	Options possibles:
	
	HTML [ON|OFF]
	SQL  [ON|OFF]
	DEBUG [ON|OFF|1|2]

	Limit� aux Administrateurs des Stats.

STAT_HELP_ENVINFO
	Syntax: ENVINFO
	
	Affiche les informations sur l'environement.
	
	Limit� aux Administrateurs des Stats.

###########################################################################
#
# ADMIN messages
#
###########################################################################

STAT_ADMIN_LOGGED_IN
	Vous �tes maintenant Administrateur des Stats.

STAT_ADMIN_LOGGED_OUT
	Vous n'�tes plus Administrateur des Stats.

STAT_ADMIN_ALREADY_LOGGED
	Vous �tes d�j� Administrateur des Stats!

STAT_LOGIN_SYNTAX
	LOGIN id mot de passe

STAT_RELOAD
	Le fichier de configuration des Stats a �t� recharg�.

###########################################################################
#
# STATUS messages
#
###########################################################################

STAT_STATUS_SERVER_COUNT
	Statistiques des serveurs pour %s:
	Serveurs actuellement reli�s:  %d
	Maximum de serveurs reli�s: %d

STAT_STATUS_CHAN_COUNT
	Statistiques des salons pour %s:
	Salons actuellement utilis�s:  %d
	Maximum de salons cr��s: %d

STAT_STATUS_USER_COUNT
	Statistiques des utilisateurs pour %s:
	Utilisateurs actuellement connect�s:  %d
	Maximum d'utilisateurs connect�s: %d

STAT_STATUS_SYNTAX
	STATUS [SERVERS|CHANNELS|USERS]

STAT_STATUS_PAGE
	Pour plus de stats, visitez %s

# SET responses
STAT_SET_SYNTAX
	SET option param�tres
STAT_SET_DEBUG_ON
	DEBUG mode des stats activ�.
STAT_SET_DEBUG_OFF
	DEBUG mode des stats d�sactiv�.
STAT_SET_DEBUG_LEVEL
	DEBUG mode des stats activ� (niveau %d).
STAT_SET_DEBUG_ERROR
	Les param�tres pour DEBUG doivent �tre ON, OFF, ou un nombre positif.
STAT_SET_UNKNOWN_OPTION
	Option SET %s inconnue.
STAT_SET_SQL_ON
	Le code SQL sera utilis�.
STAT_SET_SQL_OFF
	Le code SQL ne sera pas utilis�.
STAT_SET_SQL_ERROR
	Les param�tres pour SQL doivent �tre ON ou OFF.
STAT_SET_SQL_ERROR_INIT
	Erreur pendant l'initialisation d'SQL, regardez les logs.
STAT_SET_SQL_ERROR_DISABLED
	SQL n'est pas configur� pour ce r�seau, merci d'�diter le
	fichier de configuration (denora.conf).
STAT_SET_LIST_OPTION_ON
	%s est activ�
STAT_SET_LIST_OPTION_OFF
	%s est d�sactiv�
STAT_SET_HTML_ON
	La pages HTML seront g�n�r�es.
STAT_SET_HTML_OFF
	La pages HTML ne seront pas g�n�r�es.
STAT_SET_HTML_ERROR
	Les param�tres pour HTML doivent �tre ON ou OFF.

# CHANSTATS responses
STAT_HELP_CHANSTATS
	Les statistiques de salon vous permettent de g�rer ceux-ci comme
	les eggdrops et d'obtenir des informations sur le nombre de lettres,
	le nombre de lignes, etc...
	
	CHANSTATS ADD canal
	CHANSTATS DEL canal
	CHANSTATS LIST
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}
	CHANSTATS SUMUSER utilisateur1 utilisateur2
	CHANSTATS RENAME utilisateur1 utilisateur2
	
	La commande CHANSTATS ADD ajoute le canal indiqu� � la liste des
	canaux monitor�.
	
	La commande CHANSTATS DEL retire le canal indiqu� de la liste
	des canaux monitor�.
	
	La commande CHANSTATS LIST affiche la liste des canaux monitor�
	sur le r�seau.
	
	La commande CHANSTATS SET permet d'activer les commandes fantaisistes
	sur le canal indiqu�. Vous avez le choix pour que StatServ envoie
	des messages normaux (ON), ou utiliser des notices.
	
	La commande CHANSTATS SUMUSER permet de transferer toutes les stats
	de l'utilisateur2 vers l'utilisateur1 et supprime l'utilisateur2.
	
	La commande CHANSTATS RENAME renome l'utilisateur1 en utilisateur2.

STAT_CHANSTATS_SYNTAX
	CHANSTATS {ADD|DEL|LIST|SET|SUMUSER|RENAME} [CHAN|USER|OPTION]

STAT_CHANSTATS_CHAN_ADDED
	%s est maintenant param�tr� pour �tre monitor� par les stats salons.

STAT_CHANSTATS_CHAN_LARGENET
	LargeNet est maintenant activ�, vous ne pouvez plus ajouter de nouveaux
	cannaux.

STAT_CHANSTATS_LOGCHAN
	LogChannel ne peut etre ajout� � un canal stat�.

STAT_CHANSTATS_NOT_VALID_CHAN
	Vous n'avez pas entrer un nom de canal valide

STAT_CHANSTATS_CHAN_ALREADY
	%s est d�j� param�tr� pour �tre monitor� par les stats salons.

STAT_CHANSTATS_ADD_SYNTAX
	CHANSTATS ADD canal

STAT_CHANSTATS_DEL_SYNTAX
	CHANSTATS DEL canal

STAT_CHANSTATS_CHAN_DEL
	%s n'est d�sormais plus param�tr� pour �tre monitor� par les stats salons.

STAT_CHANSTATS_CHAN_NOTFOUND
	%s n'est pas param�tr� pour �tre monitor� par les stats salons.

STATS_CHANSTATS_PART
	Arr�t de la surveillance de %s

STATS_CHANSTATS_NOCHANNELS
	Aucun salon monitor� � lister

STATS_CHANSTATS_SET_SYNTAX
	CHANSTATS SET canal FANTASY {ON|OFF|NOTICE}

STATS_CHANSTATS_FANTASY_ON
	Les commandes de fantaisie pour %s sont d�sormais activ�es

STATS_CHANSTATS_FANTASY_OFF
	Les commandes de fantaisie pour %s sont d�sormais d�sactiv�es

STATS_CHANSTATS_FANTASY_NOTICE
	Les commandes de fantaisie pour %s seront d�sormais notic�es

STATS_CHANSTATS_SUMUSER_SYNTAX
	CHANSTATS SUMUSER utilisateur1 utilisateur2

STATS_CHANSTATS_RENAMEUSER_SYNTAX
	CHANSTATS RENAME utilisateur1 utilisateur2

STATS_CHANSTATS_SUMUSER_DONE
	Les donn�es de %s ont �t�s transfer�s vers %s

STATS_CHANSTATS_RENAMEUSER_DONE
	L'utilisateur %s � �t� renom� en %s

# EXCLUDE responses
STAT_EXCLUDE_SYNTAX
	EXCLUDE {ADD|DEL} cible

STAT_EXCLUDE_ADDED
	%s sera d�sormais exclus des statistiques

STAT_EXCLUDE_ALREADY
	%s est d�j� exclus des statistiques

STAT_EXCLUDE_DELETED
	%s ne sera d�sormais plus exclus des statistiques

STAT_EXCLUDE_NOT_FOUND
	%s n'est pas exclus des statistiques

STAT_EXCLUDE_LIST_HEADER
	Exclude List

# EXPORT responses
STATS_EXPORT_CHANNELS
	Export des salons sur %s

STATS_EXPORT_USERS
	Export des utilisateurs sur %s

STATS_EXPORT_TLD
	Export des TLD sur %s

STATS_EXPORT_SERVERS
	Export des serveurs sur %s

STATS_EXPORT_STATS
	Export des stats sur %s

STATS_EXPORT_ALL
	Export de toutes les informations sur %s

STATS_EXPORT_SYNTAX
	EXPORT {CHANNELS|USERS|TLD|SERVERS|STATS|ALL}

STATS_SUMUSER_NOTFOUND
	%s n'existe pas dans la base de donn�es des statistiques.

STATS_RENAMEUSER_EXISTING
	L'utilisateur %s existe d�ja dans le base de donn�es des stats.

STATS_HELP_CHANNEL_1
	Ces commandes fantaisistes sont acc�ssibles

STATS_HELP_CHANNEL_2
	%stop10  :   Top 10 des utilisateurs du canal courant

STATS_HELP_CHANNEL_3
	%stop    :   Top 3 des utilisateurs du canal courant

STATS_HELP_CHANNEL_4
	%sgtop   :   Top 10 de tout les utilisateurs du r�seau

STATS_HELP_CHANNEL_5
	%sgtop   :   Top 3 de tout les utilisateurs du r�seau

STATS_HELP_CHANNEL_6
	%sstats  :   Stats de l'utilisateur sur le canal

STATS_HELP_CHANNEL_7
	%sgstats :   Stats de l'utilisateur sur le r�seau

STATS_HELP_CHANNEL_8
	%speak   :   Nombre max d'utilisateurs sur le canal courant

STATS_TOP10_CHANNEL
	Top 10 de %s

STATS_TOP_CHANNEL
	Top 3 de %s

STATS_TOP10_NETWORK
	Top 10 du R�seau

STATS_TOP_NETWORK
	Top 3 du R�seau

STATS_USER_NETWORK
	Stats sur le r�seau pour %s

STATS_USER_CHANNEL
	Stats de %s sur le canal %s

STATS_MESSAGE
	%2i %-16s lettres: %s, mots: %s, lignes: %s, sourires: %s, actions: %s

STATS_MESSAGE_ONE
	lettres: %s, mots: %s, lignes: %s, smileys: %s, actions: %s

STATS_CHANNEL_PEAK
	Nombre max d'utilisateurs sur le canal: %d 

# ENVINFO responses
STAT_ENVINFO_HEADER
	Informations sur l'environement des Stats:
STAT_ENVINFO_VERSION
	Version:                %s
STAT_ENVINFO_PROTOCOL
	Protocole IRCd:         %s
STAT_ENVINFO_UPLINK
	Stats link:             %s
STAT_ENVINFO_SQL
	Code SQL:               %s
STAT_ENVINFO_HTML
	Code HTML:              %s
STAT_ENVINFO_DEBUG
	Debogage:               %s
STAT_ENVINFO_MODULESLOADED
	Nombre de modules charg�s: %d
STAT_ENVINFO_LANGUAGE
	Langue du r�seau:       %s
STAT_ENVINFO_FOOTER
	Fin des informations sur l'environement

STATS_HTML_EXPORTED
	Le fichier de Stats HTML a �t� g�n�r�

# HTML code
HTML_LANG
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
	<meta http-equiv="Content-Language" content="French">

HTML_NETSTATS_HEADER
	<table border = 0><tr><th colspan="4"><b>Statistiques R�seau:</b></th></tr>

HTML_NETSTATS_FOOTER
	</table>

HTML_NETSTATS_USERS
	<tr><td>Utilisateurs Actuels: </td> <td> %ld </td> <td>Utilisateurs Maximum: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_CHANS
	<tr><td>Salons Actuels: </td> <td> %ld </td> <td>Salons Maximum: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_OPERS
	<tr><td>Op�rateurs Actuels: </td> <td> %ld </td> <td>Op�rateurs Maximum: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_SERVERS
	<tr><td>Serveurs Actuels: </td> <td> %ld </td> <td>Serveurs Maximum: </td> <td> %ld [%s] </td></tr>

HTML_NETSTATS_AWAY
	<tr><td colspan="2">Utilisateurs Absents: </td> <td colspan="2"> %ld </td></tr>

HTML_TITLE
	Statistiques R�seau pour %s

HTML_TLD_TITLE
	Statistiques TLD (Top Level Domain)

HTML_TLD_HEADER
	<table><tr><th>Code Pays</th><th>Pays</th><th>Nombre</th><th>De fa�ons g�n�rale</th></tr>

HTML_TLD_CONTENT
	<tr><td>%s</td><td>%s</td><td>%d</td><td>%d</td></tr>

HTML_TLD_FOOTER
	</table>

HTML_SERVER_HEADER
	<table border=0><tr><th colspan = 2>Nom du Serveur</th></tr>

HTML_SERVER_CONTENT
	<tr><td>Serveur: </td><td><a href="#%s"> %s </a></td></tr>

HTML_SERVER_CONTENT_ONLINE
	<tr><td>Serveur: </td><td><a href="#%s"> %s (*) </a></td></tr>

HTML_SERVER_FOOTER
	</table>(*) indique que le serveur est reli� au r�seau actuellement<br>(Cliquez sur le nom d'un serveur pour plus d'informations)

HTML_DAILY_HEADER
	<table border = 0><tr><th colspan="4"><b>Statistiques Journali�res du R�seau:</b></th></tr>

HTML_DAILY_FOOTER
	</table>

HTML_DAILY_RESET_AT
	<tr><td colspan="4">(Toutes les Statistiques Journali�res sont remises � z�ro � minuit)</td></tr>

HTML_DAILY_USER
	<tr><td colspan="2">Utilisateurs Max du jour: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_CHAN
	<tr><td colspan="2">Salons Max du jour:</td><td colspan="2"> %ld</td></tr>

HTML_DAILY_OPERS
	<tr><td colspan="2">Op�rateurs Max du jour: </td><td colspan="2"> %ld [%s] </td></tr>

HTML_DAILY_SERVERS
	<tr><td colspan="2">Serveurs Max du jour: </td><td colspan="2"> %ld  [%s] </td></tr>

HTML_CTCP_HEADER
	<table border = 0><tr><th>Version</th><th align=right>Nombre</th></tr>

HTML_CTCP_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>	

HTML_CTCP_FOOTER
	</table>

HTML_SERVER_DETAIL_HEADER
	<table border=0><tr><th><a name="%s"></a>Serveur:</th><th colspan = 2><b>%s</b></th></tr>

HTML_SERVER_DETAIL_FOOTER
	</table>

HTML_SERVER_DETAIL_USERS
	<tr><td>Utilisateurs Actuels:</td><td>%ld (%ld)</td><td>Max %ld le [%s]</td></tr>

HTML_SERVER_DETAIL_OPERS
	<tr><td>Op�rateurs Actuels:</td><td>%ld (%ld)</td><td>Max %ld le [%s]</td></tr>

HTML_SERVER_DETAIL_CURPING
	<tr><td>Ping Actuel</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_TOTALUSERSEVER
	 <tr><td>Nombre total de connexions:</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_HIGHESTPING
	<tr><td>Plus gros Ping</td><td>%ld</td><td>le [%s]</td></tr>

HTML_SERVER_DETAIL_SPLITS
	<tr><td>Nombre de Splits</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_LAST_SEEN
	<tr><td>Vu la derni�re fois:</td><td colspan = 2>[%s]</td></tr>

HTML_SERVER_DETAIL_IRCOPKILLS
	<tr><td>Nombre de Kill des IRCOP</td><td colspan = 2>%ld</td></tr>

HTML_SERVER_DETAIL_SERVERKILLS
	<tr><td>Nombre de KILL du Serveur</td><td colspan = 2>%ld</td></tr>

HTML_CHANNEL_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Membres</th></tr>

HTML_CHANNEL_CONTENT
	<tr><td>%s</td><td align=right>%ld</td></tr>

HTML_CHANNEL_FOOTER
	</table>

HTML_CHAN_TOP10_HEADER
	<table border = 0><tr><th>Canal</th><th align=right>Nombre d'entr�es</th></tr>

HTML_CHAN_TOP10_FOOTER
	</table>(*) indique que le canal est ouvert actuelement

HTML_CHAN_TOP10_CONTENT
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_OFFLINE
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_TITLE
	<b>Top10</b> Salons F�cheux

HTML_CHAN_TOP10_KICKS_HEADER
	<table border = 0><tr><th>Canal</th><th>Nombre de Kicks</th></tr>

HTML_CHAN_TOP10_KICKS_FOOTER
	</table>(*) indique que le salon est ouvert actuellement

HTML_CHAN_TOP10_KICKS_CONTENT
	<tr><td>%s </td><td align=right>%ld</td></tr>

HTML_CHAN_TOP10_KICKS_CONTENT_IN_USE
	<tr><td>%s (*)</td><td align=right>%ld</td></tr>

HTML_TOP10_TOPIC_HEADER
	<table border = 0><tr><th>Canal</th><th>Nombre de Topics</th></tr>

HTML_TOP10_TOPIC_FOOTER
	</table>(*) indique que le canal est ouvert actuelement

HTML_TOP10_TOPIC_TITLE
	Top10 des topics des cannaux chang�s

HTML_CUR_NETSTATS_TITLE
	Statistiques courante du r�seau

HTML_DAILY_TITLE
	Statistiques journali�re du r�seau

HTML_TOP10_ONLINE_CHANS_TITLE
	Top10 des canaux en ligne

HTML_SERVERLIST_TITLE
	Liste des serveurs

HTML_TOP10_CHANEVER_TITLE
	Top10 des canaux les plus joins

HTML_SERVERDETAILS_TITLE
	D�tails des serveurs

HTML_TOP10_CLIENT_TITLE
	Top10 des versions des clients

HTML_NETMAP_TITLE
	Map du r�seau

HTML_VERSION
	Powered by <a href="http://www.denorastats.org/">Denora</a> version %s %s

HTML_MOST_UNWELCOME_TITLE
	Canaux les plus facheux

HTML_POPULAR_TITLE
	Canaux les plus populaires

HTML_MOST_TOPIC_TITLE
	Canaux avec le plus de topic

HTML_NETSTATS_TITLE
	Statistiques r�seau

NOW_STATS_ADMIN
	%s (%s) est d�sormais Administrateur des Stats.

STATS_AUTH_FAILED
	Authentification �chou� pour %s

STATS_ADMIN_LOGOUT
	%s n'est plus Administrateur des Stats.

SHUTDOWN_SAVING_DB
	Sauvegarde des bases de donn�es lors d'un SHUTDOWN, merci de patienter.

CANNOT_OPEN_LOG
	Attention: impossible d'ouvrir le fichier de log: %s

GLOBAL_LOAD_MOD
	%s charge le module %s

GLOBAL_UNLOAD_MOD
	%s d�charge le module %s

GLOBAL_FATAL_ERROR
	ERREUR FATALE! %s: %s

CONFIG_PORT_ERROR
	Le port doit �tre un nombre entre 1025 et 65535

CONFIG_HOSTNAME_ERROR
	D�finition de hostname manquante dans le bloc de connexion.

CONFIG_PROTOCOL_ERROR
	D�finition de IRCDModule (protocol) manquante dans le bloc de connexion

CONFIG_PORT_ERROR_NOTDEF
	D�finition de port manquante dans le bloc de connexion

CONFIG_PASSWD_ERROR
	D�finition de passwd manquante dans le bloc de connexion

CONFIG_PING_TO_LOW
	La fr�quence de Ping doit �tre de 2 minutes ou plus

CONFIG_SQLPING_TO_LOW
	La fr�quence de Ping doit �tre de 2 minutes ou plus

CONFIG_UPTIME_TO_LOW
	La fr�quence d'Uptime doit �tre de 2 minutes ou plus

CONFIG_DELAYMOD_T0MANY
	Modules retard�s excessifs (max 128)

CONFIG_TABLE_USER_ERROR
	D�finition de users manquante dans les blocs "tables"

CONFIG_TABLE_CHANBANS_ERROR
	D�finition de chanbans manquante dans les blocs "tables"

CONFIG_TABLE_CHANEXCEPT_ERROR
	D�finition de chanexcept manquante dans les blocs "tables"

CONFIG_TABLE_ISON_ERROR
	D�finition de ison manquante dans les blocs "tables"

CONFIG_TABLE_SERVER_ERROR
	D�finition de server manquante dans les blocs "tables"

CONFIG_TABLE_GLINE_ERROR
	D�finition de gline manquante dans les blocs "tables"

CONFIG_INVALID_LANG
	Langue choisie invalide.

CONFIG_ID_HOST_ERROR
	D�finition de user (host) manquante dans les blocs d'identification

CONFIG_ID_LANG_ERROR
	D�finition de language manquante dans les blocs d'identification

CONFIG_ID_USER_ERROR
	D�finition de user manquante dans les blocs d'identification

CONFIG_ID_DESC_ERROR
	D�finition de desc manquante dans les blocs d'identification

CONFIG_ID_NAME_ERROR
	D�finition de name manquante dans les blocs d'identification

CONFIG_ERROR_POSTIVE_VALUE
	Une valeur positive est requise.

CONFIG_SQL_USER_ERROR
	D�finition de user manquante dans le bloc SQL

CONFIG_SQL_NAME_ERROR
	D�finition de name manquante dans le bloc SQL

CONFIG_ADMIN_NAME_ERROR
	D�finition de name manquante dans le bloc admin

CONFIG_ADMIN_HOST_ERROR
	D�finition de hostname manquante dans le bloc admin

CONFIG_ADMIN_PASS_ERROR
	D�finition de passwd manquante dans le bloc admin

CONFIG_TIMEOUT_PING_ERROR
	D�finition de pingfreq manquante dans le bloc timeout

CONFIG_TIMEOUT_SQL_ERROR
	D�finition de sqlfreq manquante dans le bloc timeout

CONFIG_TIMEOUT_UPTIME_ERROR
	D�finition de uptimefreq manquante dans le bloc timeout

CONFIG_TIMEOUT_HTML_ERROR
	D�finition de html manquante dans le bloc de timeout

CONFIG_TIMEOUT_UPDATE_ERROR
	D�finition de update manquante dans le bloc de timeout

CONFIG_TIMEOUT_WARNING_ERROR
	D�finition de warning manquante dans le bloc de timeout

CONFIG_TIMEOUT_READ_ERROR
	D�finition de read manquante dans le bloc de timeout

CONFIG_INVALID_TIME
	Ce n'est pas une valeur correcte pour la dur�e.

CONFIG_HTML_UPDATE_TIME
	Le temps d'update du fichier HTML doit �tre de 2 minutes ou plus

CONFIG_NETINFO_NAME_ERROR
	D�finition de name manquante dans le bloc de netinfo

CONFIG_FILENAME_PID_ERROR
	D�finition de pid manquante dans le bloc de fichiers

CONFIG_FILENAME_MOTD_ERROR
	D�finition de motd manquante dans le bloc de fichiers

CONFIG_FILENAME_CHANDB_ERROR
	D�finition de ChannelDB manquante dans le bloc de fichiers

CONFIG_FILENAME_CTCP_ERROR
	D�finition de ctcpDB manquante dans le bloc de fichiers

CONFIG_FILENAME_SERVER_ERROR
	D�finition de ServerDB manquante dans le bloc de fichiers

CONFIG_FILENAME_CHANSTATS_ERROR
	D�finition de ChannelStatsDB manquante dans le bloc de fichiers

CONFIG_FILENAME_TLD_ERROR
	D�finition de TLDDB manquante dans le bloc de fichiers

CONFIG_FILENAME_EXCLUDE_ERROR
	D�finition de EXCLUDEDB manquante dans le bloc de fichiers

CONFIG_SS_DESC_ERROR
	D�finition de real manquante dans le bloc de statserv

CONFIG_SS_NAME_ERROR
	D�finition de nick manquante dans le bloc de statserv

CONFIG_SS_TOLONG
	Pseudo trop long

CONFIG_SS_ALIASDESC_ERROR
	D�finition de aliasreal manquante dans le bloc de statserv

ALOG_FAILED_SQL_CONNECT
	Impossible de se connecter � la base de donn�es : %s

ALOG_SQL_ENABLED
	MySQL: � �t� activ�.

ALOG_SQL_DISABLED
	MySQL: � �t� d�sactiv�.

ALOG_SQL_NOTSET
	Erreur MySQL: Configurez toutes les options requise.

ALOG_HTML_ERR_WRITE
	Errur: Impossible d'acc�der en ecriture: %s

ALOG_DB_ERROR
	ATTENTION! DB_READ_ERROR dans %s

ALOG_DEBUG_DB_OK
	debug: %s lecture r�ussie

ALOG_DEBUG_UNKNOW_CMODE
	debug: mode canal inconnu mode %c

ALOG_DEBUG_LAST_LINE
	debug: la derni�re ligne est %s

ALOG_DEBUG_REPORT_AT
	debug: veuillez reporter cette erreur sur le bugtracker au http://bugs.anope.org

ALOG_DEBUG_NONEXISTANT_USER_JOIN
	Canal: reception d'un join pour l'utilisateur %s non existant sur le canal ID %s

ALOG_DEBUG_CHANGE_MODE_CHAN
	debug: Changement des modes pour %s en %s

ALOG_DEBUG_BAD_CHAN_MODE
	Debug: mode malform� d�tect� sur %s.

ALOG_DEBUG_MODE_NO_PARAM
	canal: mode %c%c sans aucuns parametres (?) pour le canal %s

ALOG_DEBUG_FINDCHAN_BAD
	Erreur: findchan() appel� avec la valeur NULL

ALOG_DEBUG_FINDSTATCHAN_BAD
	Erreur: findstatschan() appel� avec la valeur NULL

ALOG_DEBUG_SJOIN_NONEXISTANT
	user: SJOIN pour l'utilisateur in�xistant %s sur %s

ALOG_DEBUG_JOIN_NONEXISTANT
	user: JOIN pour l'utilisateur in�xistant %s sur %s

ALOG_DEBUG_JOIN
	debug: %s � join %s

ALOG_DEBUG_KICK_NONEXISTANT
	user: KICK pour l'utilisateur in�xistant %s sur %s: %s

ALOG_DEBUG_KICKED
	debug: kick de %s de %s

ALOG_DEBUG_PART_NONEXISTANT
	user: PART de l'utilisateur in�xistant %s: %s

ALOG_DEBUG_PART
	debug: %s � quitt� %s

ALOG_DEBUG_PART_BUG
	user: BUG depart de %s: l'entr� du canal a �t� trouv� mais c->chan NULL

ALOG_NON_TS_MODE
	TSMODE activ� mais MODE n'est pas valide TS

ALOG_DEBUG_NO_FIND_CHAN
	debug: impossible de trouver le chan %s

ALOG_DEBUG_TOPIC_TS
	debug: TOPIC %s encod� TS converti en %d

ALOG_TOPIC_NONEXISTANT
	canal: TOPIC %s pour le canal in�xistant %s

ALOG_ADD_BAN_BAD
	debug! add_ban appel� avec la valeur NULL

ALOG_ADD_BAN_ADDED
	debug: ban %s ajout� au canal %s

ALOG_ADD_EXCEPTION_ADDED
	debug: except %s ajout� au canal %s

ALOG_ADD_INVITE_ADDED
	debug: invite %s ajout� au canal %s

ALOG_CREATE_CHAN
	debug: Cr�ation du canal %s

ALOG_DELETE_CHAN
	debug: Suppr�ssion du canal %s

ALOG_BAN_FREE_ERROR
	channel: BUG freeing %s: bans[%d] is NULL!

ALOG_EXCEPTION_FREE_ERROR
	BUG freeing %s: excepts[%d] is NULL!

ALOG_INVITE_FREE_ERROR
	channel: BUG freeing %s: invite[%d] is NULL!

ALOG_BAN_DEL_CHAN
	debug: ban %s supprim� du canal %s

ALOG_EXCEPTION_DEL_CHAN
	debug: except %s supprim� du canal %s

ALOG_INVITE_DEL_CHAN
	debug: invite %s supprim� du canal %s

ALOG_DEBUG_FLOOD_MODE
	debug: mode flood pour le canal %s configur� sur %s

ALOG_NO_FLOOD
	aucune configuration anti flood

ALOG_KEY_SET_TO
	debug: la cl� du canal %s est configur� sur %s

ALOG_NO_KEY
	aucune cl�

ALOG_LIMIT_SET_TO
	debug: Limite du canal %s configur� sur %u

ALOG_REDIRECT_SET_TO
	debug: redirection du canal %s configur� sur %s

ALOG_NO_REDIRECT
	aucune redirection

ALOG_DEBUG_BAD_FINDSGROUP
	Erreur: findsgroup() appel� avec la valeur NULL

ALOG_LARGENET_WARNING_1
	Vous avez choisi de d�marrer Denora avec la moindre quantit� de donn�e

ALOG_LARGENET_WARNING_2
	enregistr� dans la SQL, vous devez utiliser cette option si vous etes

ALOG_LARGENET_WARNING_3
	sur un r�seau de plus de 100,000 utilisateurs ou avez une base SQL distante

CONFIG_ERROR_BASE
	Erreur config: %s � la ligne %d de %s

CONFIG_UNKNOWN_FILE
	inconnu

CONFIG_ERROR_OPENING
	Impossible d'ouvrir le fichier de configuration %s

ALOG_DEBUG_RESOLVED_HOST
	debug: Resolution de %s en %s

ALOG_DEBUG_BAD_SSTRDUP
	debug: sstrdup() appel� avec un argument NULL

ALOG_RESET_DAILY
	Remise � z�ro des stats journali�re

ALOG_RESETTING_WEEKLY
	Remise � z�ro des stats hebdomadaires

ALOG_RESETTING_MONTHLY
	Remise � z�ro des stats mensuelles

ALOG_TRY_TO_DISPLAY
	debug: essais d'affichage du message %s

ALOG_DONE_DISPLAY
	debug: affichage du message %s termin�

ALOG_UKNOWN_USER_MODE
	debug: mode unconnu: usermode %c (dans %s)

ALOG_SWHOIS_ERROR
	SWHOIS configur� pour l'utilisateur in�xistant %s

ALOG_VHOST_FOR
	debug: %s � chang� son vhost en %s

ALOG_REALNAME_FOR
	debug: %s � chang� son realname en %s

ALOG_USERNAME_FOR
	debug: %s � chang� son username en %s

ALOG_UMODE_NONEXISTANT
	user: MODE %s pour le pseudo %s in�xistant: %s

ALOG_UNKNWON_MSG
	debug: message inconnu provenant du serveur (%s)

ALOG_TRYING_LOAD_MOD
	essais de charger [%s]

ALOG_STATUS
	status: [%d]

ALOG_TRYING_UNLOAD_MOD
	essais de decharger [%s]

RESTARTING
	Red�marrage

RESTART_FAILED
	Red�marrage �chou�

SAVING_FFF
	debug: sauvegarde des informations FFF sur le disque

ALOG_SIGPIPE_WARNING
	Denora ex�cutera un backtrace, et continue dessus, toutefois Denora peut devenir instable

ALOG_NO_STATS_BIN
	Tentative de Restart �chou� --STATS_BIN n'est pas d�fini (reexecuter configure)

ALOG_ERR_REHASH_NO_MEM
	Erreur durant le rechargement du fichier de configuration, m�moire insuffisante!

ALOG_ERR_REHASH
	Erreur durant la lecture du fichier de configuration (SIGHUP recu)

BACKTRACE_LINES
	Backtrace: repportez les lignes suivantes

BACKTRACE_NOT_HERE
	Backtrace: n'est pas disponible sur cette plateforme - utilisez GDB

ALOG_DEBUG_SERVINTRO
	debug: serveur pr�sent� (%s)

ALOG_DEBUG_SERVINTRO_FORM
	debug: serveur (%s) pr�sent� par %s

ALOG_DEBUG_PINGTIME
	debug: Ping Time de %s : %ld = %ld - %ld

ALOG_ERR_FINDSERVER
	debug: Erreur: findserver() appel� avec une valeur NULL

ALOG_ERR_FINDSERVERSTATS
	Erreur: findserverstats() appel� avec une valeur NULL

ALOG_ERR_FINDSERVERSUID
	Erreur: findserver_suid() appel� avec une valeur NULL

ALOG_ERR_DEL_SERVSTATS
	Erreur: delete_serverstats() appel� avec une valeur NULL

ALOG_DEL_SERVSTATS_FOR
	debug: delete_serverstats() appel� pour %s

ALOG_DEL_SERVSTATS_DONE
	debug: delete_serverstats() termin�

ALOG_ERR_DEL_SERVER
	Erreur: delete_server() appel� avec une valeur NULL

ALOG_DEL_SERVER_FOR
	debug: delete_server() appel� pour %s

ALOG_DEL_SERVER_NOQUIT
	debug: delete_server() retrait de tout les utilisateurs pour NOQUIT

ALOG_DEL_SERVER_DONE
	debug: delete_server() termin�

ALOG_DEBUG_UNKNOWN_VERSION_REPLY
	debug: sql_do_server_version a re�u un format de version inconnu

ALOG_MOD_TRY_LOAD
	Essaye de charger [%s]

ALOG_MOD_STATUS
	status: [%d][%s]

ALOG_MOD_ERROR
	Erreur module: %s

ALOG_MOD_BE_ONLY_ONE
	Vous ne pouvez avoir 2 modules pour le protocole de charg�.

ALOG_MOD_UNLOAD_SELF
	debug: %s demand� pour etre d�charg�...

ALOG_CMD_DISPLAY_START
	debug: essaye d'afficher la commande %s

ALOG_CMD_DISPLAY_END
	debug: fin de l'affichage de la commande %s

ALOG_CMD_DISPLAY_LIST
	debug: Affichage de la liste des commandes pour %s

ALOG_MSG_DISPLAY_START
	debug: essaye d'afficher le message %s

ALOG_MSG_DISPLAY_END
	debug: fin de l'affichage du message %s

ALOG_MSG_DISPLAY_LIST
	debug: Affichage de la liste des message pour %s

ALOG_MOD_CALLBACK_DISPLAY
	debug: Added module CallBack: [%s] due to execute at %ld

ALOG_VHOST_NONEXISTANT_USER
	user: SETHOST/CHGHOST pour l'utilisateur in�xistant %s

ALOG_CHGNAME_NONEXISTANT_USER
	user: SETNAME/CHGNAME pour l'utilisateur in�xistant %s

ALOG_CHGIDENT_NONEXISTANT_USER
	user: SETNAME/CHGNAME pour l'utilisateur in�xistant %s

STAT_ADMIN_ALREADY
	%s est d�j� administrateur des stats

STAT_ADMIN_CREATED
	%s est maintenant administrateur des stats

STAT_ADMIN_DELETED
	%s n'est plus administrateur des stats

STAT_ADMIN_NOTADMIN
	%s n'est pas un administrateur des stats

STAT_ADMIN_SHOW
	Username: %s Hostmask: %s Language ID: %d

STAT_HELP_ADMIN
	Ajoute ou retire un admin des stats, cela n'affecte pas
	les admins ajout�s dans le fichier de configuration.
	
	ADMIN ADD username password [hostmask] [lang-number]
	ADMIN DEL username
	ADMIN SETPASS username new-password
	ADMIN SHOW username
	ADMIN LIST
	
	The ADMIN ADD command adds the specified user as a Stats Admin
	
	The ADMIN DEL command removes the specified user from Stats Admin
	
	The ADMIN SETPASS command allows you to change the password of
	the specified admin.
	
	The ADMIN SHOW command allows you to see the details of the
	specified admin
	
	The ADMIN LIST command will display a list of all currently
	available Stat Admins accounts
	
	R�serv� aux Administrateurs des stats

STAT_ADMIN_SYNTAX
	ADMIN [ADD|DEL|LIST] utilisateur mot_de_passe [host] [langnum]

STAT_ADMIN_DELETE_CONFIG
	Impossible de supprimer %s tant qu'il est enregistr�
	dans le fichier de configuration.

STAT_CHGPASS_OK
	Le mot de passe de %s � �t� chang�

STAT_CHGPASS_CONFIG
	Impossible de changer le mot de passe des admins
	stock�s dans le fichier de configuration.

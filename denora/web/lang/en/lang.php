<?php
/* English Langfile for phpDenora 1.3.0
$Id: lang.php 349 2009-03-03 17:04:14Z hal9000 $

 Author:	Hal9000		<hal9000@denorastats.org>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'en_US.UTF-8', 'en_GB.UTF-8', 'en.UTF-8', 'eng.UTF-8', 'en_US', 'en_GB', 'en', 'eng');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %I:%M %p %Z";
$denora_format_short = "%m/%d/%y %I:%M:%S %p";

/* Top Menu */
DEFINE('_TM_HOME','Home');
DEFINE('_TM_NETWORK','Network');
DEFINE('_TM_GRAPHS','Graphs');
DEFINE('_TM_CHANS','Channels');
DEFINE('_TM_USERS','Users');
DEFINE('_TM_SEARCH','Search');
DEFINE('_TM_CONFIG','Settings');
DEFINE('_TM_TITLE','IRC Statistics');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Network Status');
DEFINE('_SM_NETDETAILS','Details');
DEFINE('_SM_SERVERS','Servers');
DEFINE('_SM_SRVLIST','Server List');
DEFINE('_SM_SRVDETAILS','Server Details');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Network Graphs');
DEFINE('_SM_CUSTOM','Custom...');
DEFINE('_SM_NETSPLIT','NetSplit.de Graphs');
DEFINE('_SM_SEARCHIRC','SearchIRC Graphs');
DEFINE('_SM_CHANLIST','Channel List');
DEFINE('_SM_TOPCHANS','Channel Statistics');
DEFINE('_SM_SEARCHCHAN','Search Channel...');
DEFINE('_SM_CHANNELSTATUS','Channel Status');
DEFINE('_SM_CHANNELACT','Channel Activity');
DEFINE('_SM_WHOIN','Who is Online');
DEFINE('_SM_TOPUSERS','User Statistics');
DEFINE('_SM_SEARCHUSER','Search User...');
DEFINE('_SM_GLOBALSTATS','Global Stats');
DEFINE('_SM_CHANSTATS','Channel Stats');
DEFINE('_SM_CHANINFO','Channel Information');
DEFINE('_SM_TLDPIE','Country Statistics');
DEFINE('_SM_CLIENTPIE','Client Statistics');
DEFINE('_SM_INVITES','Invite Exceptions');
DEFINE('_SM_BANS','Bans');
DEFINE('_SM_EXCEPTS','Ban Exceptions');
DEFINE('_SM_TSTATS','Total');
DEFINE('_SM_MSTATS','This Month');
DEFINE('_SM_WSTATS','This Week');
DEFINE('_SM_DSTATS','Today');
DEFINE('_SM_ASTATS','Daily Average');
DEFINE('_SM_LANGUAGE','Language');
DEFINE('_SM_THEME','Theme');
DEFINE('_SM_SET','Apply');
DEFINE('_SM_BEHAVIOR','Behavior');
DEFINE('_SM_OPERS','Operators');
DEFINE('_SM_PYSTATS','Past Year');
DEFINE('_SM_PMSTATS','Past Month');
DEFINE('_SM_PWSTATS','Past Week');
DEFINE('_SM_NSWEEKS','Last Two Weeks');
DEFINE('_SM_NSMONTHS','Last Two Months');
DEFINE('_SM_NSYEARS','Last Two Years');
DEFINE('_SM_NSHISTORY','Complete History');

/* Front Page */
DEFINE('_FP_RANKING','Current Ranking');
DEFINE('_FP_BIGGESTCHANS','Current %s Biggest Chans');
DEFINE('_FP_TOPCHANS','Top %s Channels Today');
DEFINE('_FP_TOPUSERS','Top %s Users Today');
DEFINE('_FP_STATUS','Network Status');
DEFINE('_FP_MAXSTATS','Max Stats');
DEFINE('_FP_USERS','Users:');
DEFINE('_FP_CHANS','Channels:');
DEFINE('_FP_OPERS','Opers:');
DEFINE('_FP_SERVERS','Servers:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Quick Navigation');
DEFINE('_FP_QSTATS','Quick Stats');
DEFINE('_FP_LSTATUS','Current Status');
DEFINE('_FP_QUSER','User');
DEFINE('_FP_QCHAN','Channel');
DEFINE('_FP_GO','Go');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Network Status');
DEFINE('_PT_SERVERS','Server List');
DEFINE('_PT_DETAILS','Server Details for %s');
DEFINE('_PT_MOTD','Message Of The Day (MOTD)');
DEFINE('_PT_CHANLIST','Channel List (/list)');
DEFINE('_PT_TCSTATS','Channel Activity');
DEFINE('_PT_CHANNELACT','Average Channel Activity');
DEFINE('_PT_WHOIN','Currently on %s');
DEFINE('_PT_SEARCHCHAN','Channel Search');
DEFINE('_PT_USERACT','Average User Activity');
DEFINE('_PT_SEARCHUSER','User Search');
DEFINE('_PT_TSTATS','User Activity');
DEFINE('_PT_STATSFOR','Stats for %s on %s');
DEFINE('_PT_TLDPIE','Country Statistics for %s');
DEFINE('_PT_CLIENTPIE','Client Statistics for %s');
DEFINE('_PT_MSTATS','this month');
DEFINE('_PT_WSTATS','this week');
DEFINE('_PT_DSTATS','today');
DEFINE('_PT_SEARCHRESULTS','Search Results');
DEFINE('_PT_OPERS','Current Opers');
DEFINE('_PT_NSU2W','Relation of users and channels during the last 2 weeks');
DEFINE('_PT_NSC2W','Channels during the last 2 weeks');
DEFINE('_PT_NSS2W','Servers during the last 2 weeks');
DEFINE('_PT_NSU2M','Relation of users and channels during the last 2 months');
DEFINE('_PT_NSC2M','Channels during the last 2 months');
DEFINE('_PT_NSS2M','Servers during the last 2 months');
DEFINE('_PT_NSU2Y','Relation of users and channels during the last 2 years');
DEFINE('_PT_NSC2Y','Channels during the last 2 years');
DEFINE('_PT_NSS2Y','Servers during the last 2 years');
DEFINE('_PT_NSHYS','Complete measured history about users and channels');

/* Network Status */
DEFINE('_NS_USERDET','Current users: %s Max today: %s Max: %s on %s');
DEFINE('_NS_CHANDET','Current channels: %s Max: %s on %s');
DEFINE('_NS_OPERDET','Current opers: %s Max: %s on %s');
DEFINE('_NS_SERVDET','Current servers: %s Max: %s on %s');

/* Search */
DEFINE('_SP_PROMPT','Enter the search criteria. Wildcards are also supported.');
DEFINE('_SP_SUBMIT','Search');
DEFINE('_SP_CHANINFO','Channel Info');
DEFINE('_SP_CHANSTATS','Channel Stats');
DEFINE('_SP_USERINFO','User Info');
DEFINE('_SP_USERSTATS','User Stats');

/* Configuration Page */
DEFINE('_CP_SETSORT','Set the default sorting criteria');
DEFINE('_CP_SETTYPE','Set the default statistics type');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','nickname');
DEFINE('_GD_HOST','hostname');
DEFINE('_GD_CHANNEL','channel');
DEFINE('_GD_USERS','users');
DEFINE('_GD_LETTERS','letters');
DEFINE('_GD_WORDS','words');
DEFINE('_GD_LINES','lines');
DEFINE('_GD_ACTIONS','actions');
DEFINE('_GD_SMILEYS','smileys');
DEFINE('_GD_KICKS','kicks');
DEFINE('_GD_MODES','modes');
DEFINE('_GD_TOPICS','topics');
DEFINE('_GD_ON','on');
DEFINE('_GD_TYPE','type');
DEFINE('_GD_TOTAL','total');
DEFINE('_GD_OTHER','other');
DEFINE('_GD_UNKNOWN','unknown');
DEFINE('_GD_NEVER','never');
DEFINE('_GD_NONE','none');
DEFINE('_GD_STATUS','Status');
DEFINE('_GD_YES','Yes');
DEFINE('_GD_NO','No');
DEFINE('_GD_HOUR','Hour');
DEFINE('_GD_DAY','Day');
DEFINE('_GD_MONTH','Month');
DEFINE('_GD_YEAR','Year');
DEFINE('_GD_OR','or');
DEFINE('_GD_AND','and');

/* Channel Status */
DEFINE('_CS_INFO0','Channel: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Set by: %s On: %s');
DEFINE('_CS_INFO3','Current users: %s Peak: %s on %s');
DEFINE('_CS_INFO4','Channel modes: %s');
DEFINE('_CS_INFO5','Invite Exceptions: %s Bans: %s Ban Exceptions: %s');

/* User Info */
DEFINE('_UI_UINFO','Information for nickname <strong>%s</strong> (stats group <strong>%s</strong>)');
DEFINE('_UI_NODATA','The user %s appears to be %s and there is no information cached.');
DEFINE('_UI_UCHANS','Monitored channels: %s');
DEFINE('_UI_WHOIS1','Nickname: %s Real Name: %s');
DEFINE('_UI_WHOIS2','Mask: %s');
DEFINE('_UI_WHOIS3','Status: %s');
DEFINE('_UI_WHOIS4','Connected Since: %s');
DEFINE('_UI_WHOIS5','Last Quit: %s');
DEFINE('_UI_WHOIS6','Away Status: %s');
DEFINE('_UI_WHOIS7','Message: %s');
DEFINE('_UI_WHOIS8','Used Client: %s');
DEFINE('_UI_WHOIS9','Connected from: %s On server: %s');
DEFINE('_UI_WHOIS10','Additional Information: %s');

/* Oper list */
DEFINE('_OL_NICK','Nickname');
DEFINE('_OL_LEVEL','Level');
DEFINE('_OL_ONLINE','Online since');
DEFINE('_OL_LASTQUIT','Last Quit');
DEFINE('_OL_BOT','Bot Service');
DEFINE('_OL_SERVICE','Network Service');
DEFINE('_OL_AWAY','Away');
DEFINE('_OL_AVAILABLE','Available for Help');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Showing Channels %s-%s of %s');
DEFINE('_PS_USERSHOW','Showing Users %s-%s of %s');
DEFINE('_PS_ASCENDING','Ascending');
DEFINE('_PS_DESCENDING','Descending');

/* Errors */
DEFINE('_ER_ERROR','ERROR');
DEFINE('_ER_WARNING','WARNING');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','No Data');
DEFINE('_ER_NOINVITES','No Invite Exceptions');
DEFINE('_ER_NOBANS','No Bans');
DEFINE('_ER_NOEXCEPTS','No Ban Exceptions');
DEFINE('_ER_NORESULTS','No Results');
DEFINE('_ER_MINCHARS','You need at least %s chars to search');
DEFINE('_ER_MODE','The requested operation mode %s is unknown');
DEFINE('_ER_NOSTATS','No statistical data collected for this user/channel');
DEFINE('_ER_NOCHAN','This channel does not exist');
DEFINE('_ER_EMPTYCHAN','This channel is currently empty');
DEFINE('_ER_DENIED','Access Denied');

/* API */
DEFINE('_DA_CURRENTUSR','Current users');
DEFINE('_DA_MAXUSR','Max users');
DEFINE('_DA_SERVER','Server name');
DEFINE('_DA_SRVDET1','Running %s Up since %s');
DEFINE('_DA_SRVDET2','Currently %s');
DEFINE('_DA_SRVDET3','since %s');
DEFINE('_DA_SRVDET4','Last split: %s');
DEFINE('_DA_SRVDET5','Current Ping: %s  Max: %s on %s');
DEFINE('_DA_SRVDET6','Current Users: %s  Max: %s on %s');
DEFINE('_DA_SRVDET7','Current Opers: %s  Max: %s on %s');
DEFINE('_DA_SRVDET8','Ircop Kills: %s  Server Kills: %s');
DEFINE('_DA_UPTIME','%s days and %s hours');

/* Graphs */
DEFINE('_GR_CUSTOM','Custom Graphs');
DEFINE('_GR_SELECT','Select...');
DEFINE('_GR_GENERATE','Generate');
DEFINE('_GR_SDATE','Start Date');
DEFINE('_GR_EDATE','End Date');
DEFINE('_GR_OPTIONS','Options');
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Avg');

/* Other */
DEFINE('_OT_GENTIME','Page generated in %s seconds using %s sql queries');
DEFINE('_OT_CGENTIME','Compressed page generated in %s seconds using %s sql queries');

?>

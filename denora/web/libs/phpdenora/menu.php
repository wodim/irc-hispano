<?php
// $Id: menu.php 361 2009-05-14 22:24:48Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

function getMenu($mode)
{
	$menu_array = array();
	$i = 0;
	
	switch ($mode) {
		case 'h': // Home Menu
			global $pd_netname, $pd_neturl, $pd_ennetgraphs, $pd_netsplit_graphs, $pd_searchirc_graphs;
			$menu_array[$i++] = array(1, $pd_netname, $pd_neturl);
			$menu_array[$i++] = array(0, _FP_QNAVI, NULL);
			$menu_array[$i++] = array(1, _SM_NETSTATUS, "?m=n");
			if ($pd_ennetgraphs == true || $pd_netsplit_graphs == true || $pd_searchirc_graphs == true) {
				$menu_array[$i++] = array(1, _TM_GRAPHS, "?m=g");
				if ($pd_ennetgraphs == true) {
					$menu_array[$i++] = array(2, _SM_NETSTATS, "?m=g&amp;p=netstats");
				}
				if ($pd_netsplit_graphs == true) {
					$menu_array[$i++] = array(2, _SM_NETSPLIT, "?m=g&amp;p=netsplit");
				}
				if ($pd_searchirc_graphs == true) {
					$menu_array[$i++] = array(2, _SM_SEARCHIRC, "?m=g&amp;p=searchirc");
				}
			}
			$menu_array[$i++] = array(1, _TM_CHANS, "?m=c");
			$menu_array[$i++] = array(2, _SM_CHANLIST, "?m=c&amp;p=list");
			$menu_array[$i++] = array(2, _SM_TOPCHANS, "?m=c&amp;p=top");
			$menu_array[$i++] = array(2, _SM_SEARCHCHAN, "?m=s&amp;p=chaninfo");
			$menu_array[$i++] = array(1, _TM_USERS, "?m=u");
			$menu_array[$i++] = array(2, _SM_TOPUSERS, "?m=u&amp;p=top");
			$menu_array[$i++] = array(2, _SM_SEARCHUSER, "?m=s&amp;p=userinfo");
			break;
		case 'n': // Network Menu
			global $page, $pd_entldstats, $pd_enverstats, $srv;
			$menu_array[$i++] = array(1, _SM_NETSTATUS, "?m=n&amp;p=status");
			if ($page == "status") {
				$menu_array[$i++] = array(2, _SM_NETDETAILS, "#details");
				$menu_array[$i++] = array(2, _SM_SRVLIST, "#servers");
				$menu_array[$i++] = array(2, _SM_OPERS, "#opers");
				if ($pd_entldstats == true) { $menu_array[$i++] = array(2, _SM_TLDPIE, "#country"); }
				if ($pd_enverstats == true) { $menu_array[$i++] = array(2, _SM_CLIENTPIE, "#client"); }
			}
			if ($page == "servers") {
				$menu_array[$i++] = array(0, $srv, NULL);
				$menu_array[$i++] = array(1, _SM_SRVDETAILS, "?m=n&amp;p=srvdetails&amp;srv=$srv");
				$menu_array[$i++] = array(2, _SM_NETDETAILS, "#details");
				$menu_array[$i++] = array(2, _SM_MOTD, "#motd");
			}
			break;
		case 'g': // Graphs Menu
			global $pd_ennetgraphs, $pd_netsplit_graphs, $page, $pd_netsplit_years, $pd_netsplit_history, $pd_searchirc_graphs;
			if ($pd_ennetgraphs == true) {
				$menu_array[$i++] = array(1, _SM_NETSTATS, "?m=g&amp;p=netstats");
				if ($page == "netstats") {
					$menu_array[$i++] = array(2, _SM_PYSTATS, "?m=g&amp;p=netstats&amp;t=y");
					$menu_array[$i++] = array(2, _SM_PMSTATS, "?m=g&amp;p=netstats&amp;t=m");
					$menu_array[$i++] = array(2, _SM_PWSTATS, "?m=g&amp;p=netstats&amp;t=w");
					$menu_array[$i++] = array(2, _SM_DSTATS, "?m=g&amp;p=netstats&amp;t=d");
					$menu_array[$i++] = array(2, _TM_USERS, "?m=g&amp;p=netstats&amp;t=u");
					$menu_array[$i++] = array(2, _TM_CHANS, "?m=g&amp;p=netstats&amp;t=c");
					$menu_array[$i++] = array(2, _SM_SERVERS, "?m=g&amp;p=netstats&amp;t=s");
					$menu_array[$i++] = array(2, _SM_CUSTOM, "?m=g&amp;p=netstats&amp;t=x");
				}
			}
			if ($pd_netsplit_graphs == true) {
				$menu_array[$i++] = array(1, _SM_NETSPLIT, "?m=g&amp;p=netsplit");
				if ($page == "netsplit") {
					$menu_array[$i++] = array(2, _SM_NSWEEKS, "?m=g&amp;p=netsplit&amp;t=w");
					$menu_array[$i++] = array(2, _SM_NSMONTHS, "?m=g&amp;p=netsplit&amp;t=m");
					if ($pd_netsplit_years == true) { $menu_array[$i++] = array(2, _SM_NSYEARS, "?m=g&amp;p=netsplit&amp;t=y"); }
					if ($pd_netsplit_history == true) { $menu_array[$i++] = array(2, _SM_NSHISTORY, "?m=g&amp;p=netsplit&amp;t=h"); }
				}
			}
			if ($pd_searchirc_graphs == true) {
				$menu_array[$i++] = array(1, _SM_SEARCHIRC, "?m=g&amp;p=searchirc");
			}
			break;
		case 'c': // Channels Menu
			global $page, $sort, $chan, $pd_entldstats, $pd_enverstats, $ircd, $user, $nick, $type, $denora_cstats_db;
			$menu_array[$i++] = array(1, _SM_CHANLIST, "?m=c&amp;p=list");
			$menu_array[$i++] = array(1, _SM_TOPCHANS, "?m=c&amp;p=top");
			if ($page == "top") {
				$menu_array[$i++] = array(2, _SM_TSTATS, "?m=c&amp;p=top&amp;sort=".$sort."&amp;type=0");
				$menu_array[$i++] = array(2, _SM_MSTATS, "?m=c&amp;p=top&amp;sort=".$sort."&amp;type=3");
				$menu_array[$i++] = array(2, _SM_WSTATS, "?m=c&amp;p=top&amp;sort=".$sort."&amp;type=2");
				$menu_array[$i++] = array(2, _SM_DSTATS, "?m=c&amp;p=top&amp;sort=".$sort."&amp;type=1");
			}
			$menu_array[$i++] = array(1, _SM_SEARCHCHAN, "?m=s&amp;p=chaninfo");
			if ($chan && $chan != "global") {
				$menu_array[$i++] = array(0, $chan, NULL);
				$menu_array[$i++] = array(1, _SM_CHANNELSTATUS, "?m=c&amp;chan=".urlencode(html_entity_decode($chan))."&amp;p=status");
				if ($page == "status" && $chan) {
					$menu_array[$i++] = array(2, _SM_CHANINFO, "#info");
					if ($pd_entldstats == true) { $menu_array[$i++] = array(2, _SM_TLDPIE, "#country"); }
					if ($pd_enverstats == true) { $menu_array[$i++] = array(2, _SM_CLIENTPIE, "#client"); }
					$menu_array[$i++] = array(2, _SM_WHOIN, "#who");
				}
				//Checking if channel is being monitored
				$cscount = sql_query_num_rows("SELECT chan FROM ".$denora_cstats_db." WHERE chan = '".sql_escape_string($chan)."';");
				if ($cscount != 0) {
					$menu_array[$i++] = array(1, _SM_CHANNELACT, "?m=c&amp;chan=".urlencode(html_entity_decode($chan))."&amp;p=chan");
					if ($page == "chan" && $chan) {
						$menu_array[$i++] = array(2, _SM_TSTATS, "?m=c&amp;p=chan&amp;sort=".$sort."&amp;type=0&amp;chan=".urlencode(html_entity_decode($chan)));
						$menu_array[$i++] = array(2, _SM_MSTATS, "?m=c&amp;p=chan&amp;sort=".$sort."&amp;type=3&amp;chan=".urlencode(html_entity_decode($chan)));
						$menu_array[$i++] = array(2, _SM_WSTATS, "?m=c&amp;p=chan&amp;sort=".$sort."&amp;type=2&amp;chan=".urlencode(html_entity_decode($chan)));
						$menu_array[$i++] = array(2, _SM_DSTATS, "?m=c&amp;p=chan&amp;sort=".$sort."&amp;type=1&amp;chan=".urlencode(html_entity_decode($chan)));
					}
				}
			}
			if (isset($user)) {
				$menu_array[$i++] = array(0, $user, NULL);
				$menu_array[$i++] = array(1, _SM_CHANSTATS, "?m=c&amp;p=ustats&amp;type=0&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user);
				if ($page == "ustats" && $user) {
					$menu_array[$i++] = array(2, _SM_TSTATS, "?m=c&amp;p=ustats&amp;type=0&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_MSTATS, "?m=c&amp;p=ustats&amp;type=3&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_WSTATS, "?m=c&amp;p=ustats&amp;type=2&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_DSTATS, "?m=c&amp;p=ustats&amp;type=1&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user);
				}
				$menu_array[$i++] = array(1, _SM_GLOBALSTATS, "?m=u&amp;p=ustats&amp;type=".$type."&amp;chan=global&amp;user=".$user);
			}
			elseif (isset($nick)) {
				$menu_array[$i++] = array(0, $nick, NULL);
				$menu_array[$i++] = array(1, _SM_CHANSTATS, "?m=c&amp;p=ustats&amp;type=0&amp;chan=".urlencode(html_entity_decode($chan))."&amp;nick=".$nick);
				if ($page == "ustats" && $nick) {
					$menu_array[$i++] = array(2, _SM_TSTATS, "?m=c&amp;p=ustats&amp;type=0&amp;chan=".urlencode(html_entity_decode($chan))."&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_MSTATS, "?m=c&amp;p=ustats&amp;type=3&amp;chan=".urlencode(html_entity_decode($chan))."&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_WSTATS, "?m=c&amp;p=ustats&amp;type=2&amp;chan=".urlencode(html_entity_decode($chan))."&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_DSTATS, "?m=c&amp;p=ustats&amp;type=1&amp;chan=".urlencode(html_entity_decode($chan))."&amp;nick=".$nick);
				}
				$menu_array[$i++] = array(1, _SM_GLOBALSTATS, "?m=u&amp;p=ustats&amp;type=".$type."&amp;chan=global&amp;nick=".$nick);
			}
			break;
		case 'u': // Users Menu
			global $page, $sort, $user, $nick;
			$menu_array[$i++] = array(1, _SM_TOPUSERS, "?m=u&amp;p=top");
			if ($page == "top") {
				$menu_array[$i++] = array(2, _SM_TSTATS, "?m=u&amp;p=top&amp;sort=".$sort."&amp;type=0");
				$menu_array[$i++] = array(2, _SM_MSTATS, "?m=u&amp;p=top&amp;sort=".$sort."&amp;type=3");
				$menu_array[$i++] = array(2, _SM_WSTATS, "?m=u&amp;p=top&amp;sort=".$sort."&amp;type=2");
				$menu_array[$i++] = array(2, _SM_DSTATS, "?m=u&amp;p=top&amp;sort=".$sort."&amp;type=1");
			}
			$menu_array[$i++] = array(1, _SM_SEARCHUSER, "?m=s&amp;p=userinfo");
			if (isset($user)) {
				$menu_array[$i++] = array(0, $user, NULL);
				$menu_array[$i++] = array(1, _SM_GLOBALSTATS, "?m=u&amp;p=ustats&amp;type=0&amp;user=".$user);
				if ($page == "ustats" && $user) {
					$menu_array[$i++] = array(2, _SM_TSTATS, "?m=u&amp;p=ustats&amp;type=0&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_MSTATS, "?m=u&amp;p=ustats&amp;type=3&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_WSTATS, "?m=u&amp;p=ustats&amp;type=2&amp;user=".$user);
					$menu_array[$i++] = array(2, _SM_DSTATS, "?m=u&amp;p=ustats&amp;type=1&amp;user=".$user);
				}
			}
			elseif (isset($nick)) {
				$menu_array[$i++] = array(0, $nick, NULL);
				$menu_array[$i++] = array(1, _SM_GLOBALSTATS, "?m=u&amp;p=ustats&amp;type=0&amp;nick=".$nick);
				if ($page == "ustats" && $nick) {
					$menu_array[$i++] = array(2, _SM_TSTATS, "?m=u&amp;p=ustats&amp;type=0&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_MSTATS, "?m=u&amp;p=ustats&amp;type=3&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_WSTATS, "?m=u&amp;p=ustats&amp;type=2&amp;nick=".$nick);
					$menu_array[$i++] = array(2, _SM_DSTATS, "?m=u&amp;p=ustats&amp;type=1&amp;nick=".$nick);
				}
			}
			break;
		case 's': // Search Menu
			$menu_array[$i++] = array(1, _TM_SEARCH, "?m=s");
			$menu_array[$i++] = array(2, _SP_CHANINFO, "?m=s&amp;p=chaninfo");
			$menu_array[$i++] = array(2, _SP_CHANSTATS, "?m=s&amp;p=chanstats");
			$menu_array[$i++] = array(2, _SP_USERINFO, "?m=s&amp;p=userinfo");
			$menu_array[$i++] = array(2, _SP_USERSTATS, "?m=s&amp;p=userstats");
			break;
		case 'k': // Settings Menu
			$menu_array[$i++] = array(1, _TM_CONFIG, "?m=k");
			$menu_array[$i++] = array(2, _SM_LANGUAGE, "#language");
			break;
		default: return NULL;
	}
	
	return $menu_array;
}
?>
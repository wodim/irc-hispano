<?php
// $Id: channels.php 402 2010-05-29 15:06:07Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if (!$page || $page == "list") {
	echo "<div class=\"title\">"._PT_CHANLIST."</div>";
	echo denora_list_chans(NULL);
}
elseif ((!$page && $chan == "global") || $page == "top") {
	echo "<div class=\"title\">"._PT_TCSTATS." ";
	if ($type == "1") { echo _PT_DSTATS; }
	elseif ($type == "2") { echo _PT_WSTATS; }
	elseif ($type == "3") { echo _PT_MSTATS; }
	echo " "._GD_ON." ".$pd_netname."</div>";
	phpdenora_chanstats("top",NULL,NULL);
}
elseif ($page == "status") {
	$chan_access = check_channel($chan);
	
	if ($chan_access == 0)
	{
		phpdenora_error(3,_ER_NOCHAN);
	}
	elseif ($chan_access == 1)
	{
		phpdenora_error(2,_ER_DENIED);
	}
	else
	{
		$chantopic = denora_get_chan_topic($chan);
		echo "<div class=\"title\"><a name=\"info\"></a>"._SM_CHANINFO."</div>";
		echo sprintf("<p>"._CS_INFO0,"<span class=\"color\"><strong>$chan</strong></span>");
		if ($pd_mirc == true) {
			echo "&nbsp;<a href=\"".$pd_mirc_url.urlencode(substr($chan,1))."\"><img src=\"themes/".$theme."/img/".$tpl_mirc[0]."\" alt=\"Chat\" title=\"Chat\" /></a>";
		}
		if ($pd_webchat == true) {
			echo "&nbsp;<a href=\"".$pd_webchat_url.urlencode(html_entity_decode($chan))."\"><img src=\"themes/".$theme."/img/".$tpl_webchat[0]."\" alt=\"Web Chat\" title=\"Web Chat\" /></a>";
		}
		if ($pd_mibbit == true) {
			$mibbit_url = sprintf("http://widget.mibbit.com/?settings=%s&amp;server=%s&amp;channel=%s&amp;promptPass=true", $pd_mibbit_key, $denora_irc_server, urlencode(html_entity_decode($chan)));
			echo "&nbsp;<a href=\"".$mibbit_url."\"><img src=\"themes/".$theme."/img/".$tpl_webchat[0]."\" alt=\"Mibbit\" title=\"Mibbit\" /></a>";
		}
		echo "</p>";
		if ($chantopic['topic'] == NULL) { $chantopic['topic'] = _GD_NONE; }
		$topictime = ($chantopic['topictime']) ? strftime($denora_format_time,strtotime($chantopic['topictime'])) : _GD_NEVER;
		echo sprintf("<p>"._CS_INFO1."</p>","<span class=\"color\">".denora_colorconvert($chantopic['topic'])."</span>");
		echo sprintf("<p>"._CS_INFO2."</p>","<span class=\"color\">".$chantopic['topicauthor']."</span>","<span class=\"color\">".$topictime."</span>");
		$chanpeak = denora_peak_chan($chan);
		echo sprintf("<p>"._CS_INFO3."</p>","<span class=\"color\">".denora_num_in_chan($chan)."</span>","<span class=\"color\">".$chanpeak['maxusers']."</span>","<span class=\"color\">".strftime($denora_format_time, $chanpeak['maxusertime'])."</span>");
		echo sprintf("<p>"._CS_INFO4."</p>","<span class=\"color\">".denora_chan_modes($chan)."</span>");
		echo sprintf("<p>"._CS_INFO5."</p>","<span class=\"color\">".denora_num_chan_invites($chan)."</span>","<span class=\"color\">".denora_num_chan_bans($chan)."</span>","<span class=\"color\">".denora_num_chan_except($chan)."</span>");
		if ($pd_entldstats == true) {
			echo "<div class=\"title\"><a name=\"country\"></a>".sprintf(_PT_TLDPIE,$chan)."</div>";
			echo "<img src=\"libs/phpdenora/graphs/pie.php?theme=$theme&amp;lang=$lang&amp;mode=country&amp;chan=".urlencode(html_entity_decode($chan))."\" alt=\"\" />";
			$gqcount = $gqcount + 2;
		}
		if ($pd_enverstats == true) {
			echo "<div class=\"title\"><a name=\"client\"></a>".sprintf(_PT_CLIENTPIE,$chan)."</div>";
			echo "<img src=\"libs/phpdenora/graphs/pie.php?theme=$theme&amp;lang=$lang&amp;mode=version&amp;chan=".urlencode(html_entity_decode($chan))."\" alt=\"\" />";
			$gqcount = $gqcount + 2;
		}
		echo "<div class=\"title\"><a name=\"who\"></a>".sprintf(_PT_WHOIN,$chan)."</div>";
		$whoinlist = denora_who_in_channel($chan);
		if ($whoinlist == 0)
		{
			phpdenora_error(3,_ER_EMPTYCHAN);
		}
		else
		{
			$qsum = sizeof($whoinlist);
			$qlimit = ( $subpage - 1 ) * $pd_rlimit;
			$rchan2 = (($subpage * $pd_rlimit) < $qsum) ? ($subpage * $pd_rlimit) : $qsum;
			echo "<table width=\"95%\" border=\"0\" class=\"center\">
			  <tr>
				<td align=\"left\">";
			echo sprintf(_PS_USERSHOW,"<strong>".($subpage * $pd_rlimit - $pd_rlimit + 1)."</strong>","<strong>".$rchan2."</strong>","<strong>".$qsum."</strong><br />");
			echo "</td>
				<td align=\"right\">";	
			echo "<h1>";
			// display the << button
			if ($subpage == 1) { echo "&laquo; "; }
			else { echo "<a href=\"?m=c&amp;p=status&amp;chan=".urlencode(html_entity_decode($chan))."&amp;sub=1\">&laquo;</a> "; }
			// display the < button
			if ($subpage > 1) { echo "<a href=\"?m=c&amp;p=status&amp;chan=".urlencode(html_entity_decode($chan))."&amp;sub=" . ($subpage - 1) . "\">&#8249;</a> "; }
			else { echo "&#8249; "; }
			// display the > button
			if ($subpage < $qsum/$pd_rlimit) { echo "<a href=\"?m=c&amp;p=status&amp;chan=".urlencode(html_entity_decode($chan))."&amp;sub=" . ($subpage + 1) . "\">&#8250;</a> "; }
			else { echo "&#8250; "; }
			// display the >> button
			if ($rchan2 == $qsum) { echo "&raquo; "; }
			else { echo "<a href=\"?m=c&amp;p=status&amp;chan=".urlencode(html_entity_decode($chan))."&amp;sub=".ceil($qsum / $pd_rlimit)."\">&raquo;</a> "; }
			echo "</h1>";
			echo "</td>
			  </tr>
			</table>";
			
			echo "<table class=\"stats\" width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">
				<tr><th class=\"color\" scope=\"col\" align=\"center\">n.</th><th scope=\"col\">&nbsp;</th><th scope=\"col\">&nbsp;</th><th class=\"color\" scope=\"col\"><b>"._GD_NICK."</b></th><th class=\"color\" scope=\"col\"><b>"._GD_MODES."</b></th><th class=\"color\" scope=\"col\"><b>mask</b></th></tr>";
			for ($x = $qlimit; $x < $rchan2; $x++)  {
				if ($whoinlist[$x]['bot'] == "Y") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_bot[0]."\" alt=\""._OL_BOT."\" title=\""._OL_BOT."\" />";
				}
				elseif ($whoinlist[$x]['uline'] == "1") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_service[0]."\" alt=\""._OL_SERVICE."\" title=\""._OL_SERVICE."\" />";
				}
				elseif ($whoinlist[$x]['away'] == "Y") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_away[0]."\" alt=\""._OL_AWAY."\" title=\""._OL_AWAY."\" />";
				}
				elseif ($whoinlist[$x]['helper'] == "Y") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_helper[0]."\" alt=\""._OL_AVAILABLE."\" title=\""._OL_AVAILABLE."\" />";
				}
				elseif ($whoinlist[$x]['online'] == "Y") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_online[0]."\" alt=\"Online\" title=\"Online\" />";
				}
				if ($whoinlist[$x]['online'] == "N") {
					$user_status = "<img src=\"themes/$theme/img/".$tpl_status_offline[0]."\" alt=\"Offline\" title=\"Offline\" />";
				}
				$flagfile = "libs/phpdenora/flags/".strtolower($whoinlist[$x]['countrycode']).".png";
				$countryflag = file_exists($flagfile) ? "<img src=\"".$flagfile."\" alt=\"".$whoinlist[$x]['country']."\" title=\"".$whoinlist[$x]['country']."\" />" : "&nbsp;";
				echo sprintf("<tr class=\"bg\"><td class=\"b l t n\" align=\"center\" valign=\"middle\"><strong>%s</strong></td><td class=\"t b\" align=\"center\" valign=\"middle\">%s</td><td class=\"t b\" align=\"center\" valign=\"middle\">%s</td><td class=\"b t\"><a href=\"?m=c&amp;p=ustats&amp;type=0&amp;chan=%s&amp;nick=%s\">%s</a></td><td class=\"t b\">%s&nbsp;</td><td class=\"t r b\">%s</td></tr><tr><td colspan=\"4\"></td></tr>",$x + 1,$user_status,$countryflag,urlencode(html_entity_decode($chan)),$whoinlist[$x]['nick'],$whoinlist[$x]['nick'],$whoinlist[$x]['modes'],$whoinlist[$x]['username']."@".$whoinlist[$x]['host']);
			}
			echo "</table>";
		}
	}
}
elseif ((!$page && $chan) || $page == "chan") {
	$chan_access = check_channel($chan);	
	$cscount = sql_query_num_rows("SELECT chan FROM ".$denora_cstats_db." WHERE LOWER(chan) = LOWER('".sql_escape_string($chan)."');");
	
	if ($chan_access == 0)
	{
		phpdenora_error(3,_ER_NOCHAN);
	}
	elseif ($chan_access == 1)
	{
		phpdenora_error(2,_ER_DENIED);
	}
	elseif (@$cscount == 0) {
		phpdenora_error(3,_ER_NOSTATS);
	}
	else {
		echo "<div class=\"title\">"._PT_CHANNELACT." ";
		if ($type == "1") { echo _PT_DSTATS; }
		elseif ($type == "2") { echo _PT_WSTATS; }
		elseif ($type == "3") { echo _PT_MSTATS; }
		echo " </div>";
		echo "<img src=\"libs/phpdenora/graphs/bar.php?theme=$theme&amp;lang=$lang&amp;mode=chan&amp;chan=".urlencode(html_entity_decode($chan))."&amp;type=$type\" alt=\"\" />";
		$gqcount++;
		echo "<div class=\"title\">"._PT_TSTATS." ";
		if ($type == "1") { echo _PT_DSTATS; }
		elseif ($type == "2") { echo _PT_WSTATS; }
		elseif ($type == "3") { echo _PT_MSTATS; }
		echo " "._GD_ON." $chan</div>";
		phpdenora_chanstats("channel",$chan,NULL);
	}
}
elseif ($page == "ustats") {
	if (isset($nick)) {
		$user = denora_get_uname($nick);
		$user = ($user) ? $user : $nick;
	} elseif (isset($user)) {
		$nick = denora_get_alias($user);
		$nick = ($nick) ? $nick : $user;
	} else {
		$user = NULL; $nick = NULL;
	}
	
	echo "<div class=\"title\">".sprintf(_UI_UINFO, $nick, $user)."</div>";
	$usrdets = phpdenora_userinfo($nick);
	if ($usrdets['hostname'] == NULL)
	{
		$str = sprintf(_UI_NODATA,$nick,"<span style=\"color:red\">Offline</span>")."<br />";
	}
	else
	{
		$umask = $usrdets['nick'] . "!" . $usrdets['username'] . "@" . $usrdets['hostname'];
		$str = sprintf(_UI_WHOIS1,"<span class=\"color\">".$usrdets['nick']."</span>","<span class=\"color\">".denora_colorconvert($usrdets['realname'])."</span><br />");
		$str .= sprintf(_UI_WHOIS2,"<span class=\"color\">".$umask."</span><br />");
		if (@$usrdets['swhois'] != NULL) { $str .=sprintf(_UI_WHOIS10, "<span class=\"color\">".denora_colorconvert($usrdets['swhois'])."</span><br />"); }
		if ($usrdets['online'] == "Y")
		{
			$str .= sprintf(_UI_WHOIS3,"<span style=\"color:green\">Online</span> ");
			$str .= sprintf(_UI_WHOIS4,"<span class=\"color\">".strftime($denora_format_time,$usrdets['connecttime'])."</span>") . "<br />";
			if ($usrdets['away'] == "Y")
			{
				$awaymsg = ($usrdets['awaymsg'] == NULL) ? _GD_YES : $usrdets['awaymsg'];
				$str .= sprintf(_UI_WHOIS6,"<span class=\"color\">".denora_colorconvert($awaymsg)."</span><br />");
			}
		}
		else
		{
			$str .= sprintf(_UI_WHOIS3,"<span style=\"color:red\">Offline</span> ");
			$str .= sprintf(_UI_WHOIS5,"<span class=\"color\">".strftime($denora_format_time,$usrdets['lastquit'])."</span>") . "<br />";
			if (@$usrdets['quitmsg'] != NULL) { $str .= sprintf(_UI_WHOIS7,"<span class=\"color\">".denora_colorconvert($usrdets['quitmsg'])."</span>") . "<br />"; }
		}
		$flagfile = "libs/phpdenora/flags/".strtolower($usrdets['countrycode']).".png";
		$userflag = file_exists($flagfile) ?  "<img src=\"".$flagfile."\" alt=\"".$usrdets['country']."\" title=\"".$usrdets['country']."\" />" : NULL;
		$usercountry = ($usrdets['country'] == NULL) ? _GD_UNKNOWN : $usrdets['country'];
		$userver = (denora_ishidden($usrdets['server']) == true) ? _GD_UNKNOWN : "<a href=\"?m=n&amp;p=srvdetails&amp;srv=".$usrdets['server']."\">".$usrdets['server']."</a>";
		$str .= sprintf(_UI_WHOIS9,"<span class=\"color\">$usercountry</span> $userflag",$userver) . "<br />";
		$userclient = ($usrdets['ctcpversion'] == NULL) ? _GD_UNKNOWN : $usrdets['ctcpversion'];
		if ($pd_enverstats == true)
		{
			$str .= sprintf(_UI_WHOIS8,"<span class=\"color\">".denora_colorconvert($userclient)."</span>") . "<br />";
		}
	}
	echo "<p>$str</p>";
	$userchans = phpdenora_userstats("channels",$user,$nick,NULL);
	if ($userchans != NULL)
	{
		echo "<p>".sprintf(_UI_UCHANS,"<span class=\"color\">".$userchans."</span>")."</p>";
	}
	$userstats = phpdenora_userstats("user",$user,$chan,NULL);
	if ($userstats == NULL) {
		phpdenora_error(3,_ER_NOSTATS);
	}
	else {
		echo "<a name=\"chart\"></a><div class=\"title\">"._PT_USERACT." ";
		if ($type == "1") { echo _PT_DSTATS; }
		elseif ($type == "2") { echo _PT_WSTATS; }
		elseif ($type == "3") { echo _PT_MSTATS; }
		echo " </div>";
		echo "<img src=\"libs/phpdenora/graphs/bar.php?theme=$theme&amp;lang=$lang&amp;mode=user&amp;user=".urlencode($user)."&amp;chan=".urlencode(html_entity_decode($chan))."&amp;type=$type\" alt=\"\" />";
		$gqcount++;
		echo "<div class=\"title\">".sprintf(_PT_STATSFOR,$user,$chan)."</div>";
		echo "<table class=\"stats\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\">";
		echo "<tr>\n",
			"<th class=\"color\">"._GD_TYPE."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_LETTERS."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_WORDS."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_LINES."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_ACTIONS."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_SMILEYS."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_KICKS."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_MODES."</th>\n",
			"<th class=\"color\" align=\"right\">"._GD_TOPICS."</th>\n",
			"</tr>\n";
		for ($i = 0; $i < count($userstats); $i++) {
			if ($userstats[$i][0] == 0) { $stattype = _GD_TOTAL; }
			if ($userstats[$i][0] == 3) { $stattype = _PT_MSTATS; }
			if ($userstats[$i][0] == 2) { $stattype = _PT_WSTATS; }
			if ($userstats[$i][0] == 1) { $stattype = _PT_DSTATS; }
			echo "<tr><td class=\"color\">\n",
				"<a href=\"?m=".$mode."&amp;p=".$page."&amp;sort=".$sort."&amp;type=".$userstats[$i][0]."&amp;chan=".urlencode(html_entity_decode($chan))."&amp;user=".$user."\">\n", $stattype, "</a>\n", // type
				"</td><td align=\"right\">\n", $userstats[$i][1], // letters 
				"</td><td align=\"right\">\n", $userstats[$i][2], // words 
				"</td><td align=\"right\">\n", $userstats[$i][3], // line
				"</td><td align=\"right\">\n", $userstats[$i][4], // actions
				"</td><td align=\"right\">\n", $userstats[$i][5], // smileys
				"</td><td align=\"right\">\n", $userstats[$i][6], // kicks
				"</td><td align=\"right\">\n", $userstats[$i][7], // modes
				"</td><td align=\"right\">\n", $userstats[$i][8], // topics
				"</td></tr>\n";
		}
		echo "</table>\n";
	}
}
else {
	phpdenora_error(1,$page);
}
?>
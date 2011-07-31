<?php
// $Id: users.php 352 2009-03-04 07:30:25Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if ((!$page && !$user) || $page == "top") {
	echo "<div class=\"title\">".sprintf(_PT_TSTATS,$pd_rlimit)." ";
	if ($type == "1") { echo _PT_DSTATS; }
	elseif ($type == "2") { echo _PT_WSTATS; }
	elseif ($type == "3") { echo _PT_MSTATS; }
	echo " "._GD_ON." ".$pd_netname."</div>";
	phpdenora_userstats("top",NULL,NULL,NULL);
}
elseif ((!$page && $user) || $page == "ustats") {
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
		if (extension_loaded('gd') == 1) {
			echo "<img src=\"libs/phpdenora/graphs/bar.php?theme=$theme&amp;lang=$lang&amp;mode=user&amp;user=".urlencode($user)."&amp;chan=global&amp;type=$type\" alt=\"\" />";
			$gqcount++;
		}
		else {
			phpdenora_colcharts("user","global",$user);
		}
		echo "<div class=\"title\">".sprintf(_PT_STATSFOR,$user,$pd_netname)."</div>";
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
				"<a href=\"?m=".$mode."&amp;p=".$page."&amp;sort=".$sort."&amp;type=".$userstats[$i][0]."&amp;chan=".$chan."&amp;user=".$user."\">\n", $stattype, "</a>\n", // type
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
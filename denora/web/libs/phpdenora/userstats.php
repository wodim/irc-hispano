<?php
// $Id: userstats.php 414 2011-02-03 23:28:00Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

function phpdenora_userstats($what, $user, $chan, $search) {
	global $ircd, $denora_ustats_db, $denora_chan_db, $denora_user_db, $pd_rlimit, $sort, $order, $type, $page, $subpage, $mode, $theme, $pd_statsort, $tpl_asc, $tpl_desc, $tpl_status_bot, $tpl_status_service, $tpl_status_away, $tpl_status_oper, $tpl_status_online, $tpl_status_offline, $tpl_status_helper, $denora_noshow_chans, $pd_statuslookup;

	switch ($what) {

	case 'top':
		$search = str_replace("*", "%", $search);
		if ($search == NULL)
		{
			$a1 = NULL;
			$l1 = "m=u&amp;p=top";
			$l2 = NULL;
		}
		else
		{
			$a1 = "AND uname LIKE '".sql_escape_string($search)."%'";
			$l1 = "m=s&amp;p=userstats";
			$l2 = "&amp;search=".stripslashes($search);
			$type = 0;
		}
		$qsum = sql_query_num_rows("SELECT uname FROM $denora_ustats_db WHERE type='$type' AND chan='global' AND letters > 0 $a1");
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
		else { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order&amp;type=$type$l2&amp;sub=1\">&laquo;</a> "; }
		// display the < button
		if ($subpage > 1) { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order&amp;type=$type$l2&amp;sub=" . ($subpage - 1) . "\">&#8249;</a> "; }
		else { echo "&#8249; "; }
		// display the > button
		if ($subpage < $qsum/$pd_rlimit) { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order&amp;type=$type$l2&amp;sub=" . ($subpage + 1) . "\">&#8250;</a> "; }
		else { echo "&#8250; "; }
		// display the >> button
		if ($rchan2 == $qsum) { echo "&raquo; "; }
		else { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order&amp;type=$type$l2&amp;sub=".ceil($qsum / $pd_rlimit)."\">&raquo;</a> "; }
		echo "</h1>";
		echo "</td>
		  </tr>
		</table>";
		
		$q = sql_query("SELECT uname,letters,words,line,actions,smileys,kicks,modes,topics FROM ".$denora_ustats_db." WHERE (type=".$type." AND chan=\"global\" AND letters > 0 $a1) ORDER BY ".$denora_ustats_db.".".sql_escape_string($sort)." ".strtoupper($order)." LIMIT $qlimit , $pd_rlimit;");
		$maxtype = ($search == NULL) ? $type : 0;
		$q2 = sql_query("SELECT MAX(letters), MAX(words), MAX(line), MAX(actions), MAX(smileys), MAX(kicks), MAX(modes), MAX(topics) FROM $denora_ustats_db WHERE type='$maxtype' AND chan='global'");
		$maxvalues = sql_fetch_array($q2);
		
		$opic = array(); $olink = array("uname" => NULL, "letters" => NULL, "words" => NULL, "line" => NULL, "actions" => NULL, "smileys" => NULL, "kicks" => NULL, "modes" => NULL, "topics" => NULL);

		if ($order == "desc")
		{
			$olink = array("uname" => "asc", "letters" => "desc", "words" => "desc", "line" => "desc", "actions" => "desc", "smileys" => "desc", "kicks" => "desc", "modes" => "desc", "topics" => "desc");
			$olink[$sort] = "asc";
			$opic[$sort] = " <img src=\"themes/".$theme."/img/".$tpl_desc[0]."\" alt=\""._PS_DESCENDING."\" title=\""._PS_DESCENDING."\" />";
		}
		elseif ($order == "asc")
		{
			if ($sort == "uname")
			{
				$olink = array("uname" => "desc", "letters" => "desc", "words" => "desc", "line" => "desc", "actions" => "desc", "smileys" => "desc", "kicks" => "desc", "modes" => "desc", "topics" => "desc");
			}
			else {
				$olink = array("uname" => "asc", "letters" => "asc", "words" => "asc", "line" => "asc", "actions" => "asc", "smileys" => "asc", "kicks" => "asc", "modes" => "asc", "topics" => "asc");
				$olink[$sort] = "desc";
			}
			$opic[$sort] = " <img src=\"themes/".$theme."/img/".$tpl_asc[0]."\" alt=\""._PS_ASCENDING."\" title=\""._PS_ASCENDING."\" />";
		}
		
		echo "<table class=\"stats\" width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">";
		echo "<tr>",
			"\n<th class=\"color\" scope=\"col\">"._GD_NUMBER."</th>",
			"\n<th class=\"color\" scope=\"col\">&nbsp;</th>",
			"\n<th class=\"color\" scope=\"col\">&nbsp;</th>";
		echo "\n<th scope=\"col\"><a href=\"?$l1$l2&amp;sort=uname&amp;order=".$olink['uname']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_NICK."</a>".@$opic['uname']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=letters&amp;order=".$olink['letters']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_LETTERS."</a>".@$opic['letters']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=words&amp;order=".$olink['words']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_WORDS."</a>".@$opic['words']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=line&amp;order=".$olink['line']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_LINES."</a>".@$opic['line']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=actions&amp;order=".$olink['actions']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_ACTIONS."</a>".@$opic['actions']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=smileys&amp;order=".$olink['smileys']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_SMILEYS."</a>".@$opic['smileys']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=kicks&amp;order=".$olink['kicks']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_KICKS."</a>".@$opic['kicks']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=modes&amp;order=".$olink['modes']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_MODES."</a>".@$opic['modes']."</th>",
		"\n<th align=\"right\" scope=\"col\"><a href=\"?$l1$l2&amp;sort=topics&amp;order=".$olink['topics']."&amp;type=".$type."&amp;sub=$subpage\">"._GD_TOPICS."</a>".@$opic['topics']."</th>";
		echo "</tr>\n";
		$i=1;
		while ($r = sql_fetch_array($q)) {
			$user_status = denora_get_user_status($r[0]);
			if ($pd_statuslookup == true) {
				if ($user_status['bot'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_bot[0]."\" alt=\""._OL_BOT."\" title=\""._OL_BOT."\" />";
				}
				elseif ($user_status['service'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_service[0]."\" alt=\""._OL_SERVICE."\" title=\""._OL_SERVICE."\" />";
				}
				elseif ($user_status['away'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_away[0]."\" alt=\""._OL_AWAY."\" title=\""._OL_AWAY."\" />";
				}
				elseif ($user_status['helper'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_helper[0]."\" alt=\""._OL_AVAILABLE."\" title=\""._OL_AVAILABLE."\" />";
				}
				elseif ($user_status['online'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_online[0]."\" alt=\"Online\" title=\"Online\" />";
				}
				else {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_offline[0]."\" alt=\"Offline\" title=\"Offline\" />";
				}
				if ($user_status['offline'] == 1) {
					$user_icon = "<img src=\"themes/$theme/img/".$tpl_status_offline[0]."\" alt=\"Offline\" title=\"Offline\" />";
				}
			}
			else {
				$user_icon = "&nbsp;";
			}
			$flagfile = "libs/phpdenora/flags/".strtolower($user_status['countrycode']).".png";
			echo "<tr class=\"bg\">",
			"\n<td rowspan=\"2\" align=\"center\" class=\"l t b n\"><strong>", $i + ($pd_rlimit * ($subpage - 1)),
			"\n</strong>",
			"</td><td rowspan=\"2\" class=\"t b\" align=\"center\" valign=\"middle\">$user_icon</td>",
			"<td rowspan=\"2\" class=\"t b\" align=\"center\" valign=\"middle\">";
			if (file_exists($flagfile)) { echo "<img src=\"".$flagfile."\" alt=\"".$user_status['country']."\" title=\"".$user_status['country']."\" />"; }
			else { echo "&nbsp;"; }
			echo "</td><td rowspan=\"2\" align=\"left\" class=\"t b";
			if ($sort == "uname") { echo " s"; }
			echo "\">";
			echo"<a href=\"?m=u&amp;p=ustats&amp;type=".$type."&amp;user=".$r[0]."\">".$r[0]."</a> "; // uname
			echo "\n</td><td align=\"right\" class=\"t";
			if ($sort == "letters") { echo " s"; }
			echo "\">", $r[1], // letters 
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "words") { echo " s"; }
			echo "\">", $r[2], // words 
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "line") { echo " s"; }
			echo "\">", $r[3], // line
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "actions") { echo " s"; }
			echo "\">", $r[4], // actions
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "smileys") { echo " s"; }
			echo "\">", $r[5], // smileys
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "kicks") { echo " s"; }
			echo "\">", $r[6], // kicks
			"\n</td><td align=\"right\" class=\"t";
			if ($sort == "modes") { echo " s"; }
			echo "\">", $r[7], // modes
			"\n</td><td align=\"right\" class=\"r t";
			if ($sort == "topics") { echo " s"; }
			echo "\">", $r[8], // topics
			"\n</td></tr>",
			"\n<tr class=\"bg\">",
			"<td align=\"right\" class=\"b";
			if ($sort == "letters") { echo " s"; }
			echo "\">";
			makeBarTable($r[1],$maxvalues[0],50); // letters
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "words") { echo " s"; }
			echo "\">";
			makeBarTable($r[2],$maxvalues[1],50); // words 
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "line") { echo " s"; }
			echo "\">";
			makeBarTable($r[3],$maxvalues[2],50); // line
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "actions") { echo " s"; }
			echo "\">";
			makeBarTable($r[4],$maxvalues[3],50); // actions
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "smileys") { echo " s"; }
			echo "\">";
			makeBarTable($r[5],$maxvalues[4],50); // smileys
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "kicks") { echo " s"; }
			echo "\">";
			makeBarTable($r[6],$maxvalues[5],50); // kicks
			echo "\n</td><td align=\"right\" class=\"b";
			if ($sort == "modes") { echo " s"; }
			echo "\">";
			makeBarTable($r[7],$maxvalues[6],50); // modes
			echo "\n</td><td align=\"right\" class=\"b r";
			if ($sort == "topics") { echo " s"; }
			echo "\">";
			makeBarTable($r[8],$maxvalues[7],50); // topics
			echo "\n</td></tr>";
			echo "<tr><td colspan=\"11\"></td></tr>";
			$i++;
		}
		echo "</table>\n";
		break;

	case 'user':
		$i = 0; $data = array();
		if ($chan == "global") {
			$q = sql_query("SELECT type,letters,words,line,actions,smileys,kicks,modes,topics FROM ".$denora_ustats_db." WHERE uname=\"".sql_escape_string($user)."\" AND chan=\"global\" ORDER BY \"".$denora_ustats_db.".letters\" DESC;");
		}
		else {
			$d1 = NULL; $noshow = array();
			$no = explode(",",$denora_noshow_chans);
			for ($x = 0; $x < count($no); $x++) { $d1 .= " AND LOWER($denora_chan_db.channel) NOT LIKE '".sql_escape_string(strtolower($no[$x]))."'"; }
			$q = sql_query("SELECT type,letters,words,line,actions,smileys,kicks,modes,topics FROM ".$denora_ustats_db.", ".$denora_chan_db." WHERE ".$denora_ustats_db.".uname=\"".sql_escape_string($user)."\" AND ".$denora_ustats_db.".chan=\"".sql_escape_string($chan)."\" AND BINARY LOWER(".$denora_ustats_db.".chan)=LOWER(".$denora_chan_db.".channel) ".$d1." ORDER BY \"".$denora_ustats_db.".letters\" DESC;");
		}
		while ($r = sql_fetch_array($q)) {
			$data[$i++] = $r;
		}
		return $data;
		break;

	case 'channels':
		$b1 = NULL; $c1 = NULL; $d1 = NULL; $noshow = array();
		$nick = $chan; //Workaround :P
		if ($ircd['secretchan'] == 1) { $b1 = "AND ".$denora_chan_db.".".$ircd['secretchan_mode']."=\"N\""; }
		if ($ircd['chanhide'] == 1) { $c1 = "AND ".$denora_user_db.".".$ircd['chanhide_mode']."=\"N\""; }
		$no = explode(",",$denora_noshow_chans);
		for ($i = 0; $i < count($no); $i++) { $d1 .= " AND LOWER(channel) NOT LIKE '".sql_escape_string(strtolower($no[$i]))."'"; }
		$q = sql_query("SELECT DISTINCT chan FROM ".$denora_ustats_db.", ".$denora_chan_db.", ".$denora_user_db." WHERE ".$denora_ustats_db.".uname=\"".sql_escape_string($user)."\" AND ".$denora_ustats_db.".type=\"0\" AND BINARY LOWER(".$denora_ustats_db.".chan)=LOWER(".$denora_chan_db.".channel) AND ".$denora_user_db.".nick=\"".sql_escape_string($nick)."\" ".$b1." ".$c1." ".$d1.";");
		$str = NULL;
		while ($r = sql_fetch_array($q)) {
			if ($r[0] != "global") {
				$str .= "<a href=\"?m=c&amp;p=ustats&amp;chan=".urlencode($r[0])."&amp;user=".$user."\">".htmlspecialchars($r[0])."</a>\n";
			}
		}
		return $str;
		break;

	default:
		phpdenora_error(1,$what);
	}
}

?>
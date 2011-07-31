<?php
// $Id: search.php 352 2009-03-04 07:30:25Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if (isset($search))
{
	$search_ = str_replace("*", "", $search);
	$search_ = str_replace("%", "", $search_);
	if (strlen($search_) < $pd_minchars) {
		phpdenora_error(2,sprintf(_ER_MINCHARS,$pd_minchars));
	}
	else {
		if ($page == "chanstats")
		{
			echo "<div class=\"title\">"._PT_SEARCHRESULTS."</div>";
			phpdenora_chanstats("top",NULL,$search);
		}
		if ($page == "chaninfo")
		{
			echo "<div class=\"title\">"._PT_SEARCHRESULTS."</div>";
			echo denora_list_chans($search);
		}
		elseif ($page == "userstats")
		{
			echo "<div class=\"title\">"._PT_SEARCHRESULTS."</div>";
			phpdenora_userstats("top",NULL,NULL,$search);
		}
		elseif ($page == "userinfo")
		{
			echo "<div class=\"title\">"._PT_SEARCHRESULTS."</div>";
			list($qsum,$seendata) = phpdenora_seenuser($search);
			if ($qsum == 0) {
				phpdenora_error(3,_ER_NORESULTS);
			}
			else {
				$ruser = (($subpage * $pd_rlimit) < $qsum) ? ($subpage * $pd_rlimit) : $qsum;
				echo "<table width=\"95%\" border=\"0\" class=\"center\">
				  <tr>
					<td align=\"left\">";
				echo sprintf(_PS_USERSHOW,"<strong>".($subpage * $pd_rlimit - $pd_rlimit + 1)."</strong>","<strong>".$ruser."</strong>","<strong>".$qsum."</strong><br />");
				echo "</td>
					<td align=\"right\">";	
				echo "<h1>";
				// display the << button
				if ($subpage == 1) { echo "&laquo; "; }
				else { echo "<a href=\"?m=s&amp;p=userinfo&amp;search=$search&amp;sort=$sort&amp;order=$order&amp;sub=1\">&laquo;</a> "; }
				// display the < button
				if ($subpage > 1) { echo "<a href=\"?m=s&amp;p=userinfo&amp;search=$search&amp;sort=$sort&amp;order=$order&amp;sub=" . ($subpage - 1) . "\">&#8249;</a> "; }
				else { echo "&#8249; "; }
				// display the > button
				if ($subpage < $qsum/$pd_rlimit) { echo "<a href=\"?m=s&amp;p=userinfo&amp;search=$search&amp;sort=$sort&amp;order=$order&amp;sub=" . ($subpage + 1) . "\">&#8250;</a> "; }
				else { echo "&#8250; "; }
				// display the >> button
				if ($ruser == $qsum) { echo "&raquo; "; }
				else { echo "<a href=\"?m=s&amp;p=userinfo&amp;search=$search&amp;sort=$sort&amp;order=$order&amp;sub=".ceil($qsum / $pd_rlimit)."\">&raquo;</a> "; }
				echo "</h1>";
				echo "</td>
				  </tr>
				</table>";
				
				echo"<table class=\"stats\" width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">
				  <tr>
					<th scope=\"col\" align=\"center\"><span class=\"color\">n.</span></th>
					<th scope=\"col\">&nbsp;</th>
					<th scope=\"col\">&nbsp;</th>
					<th scope=\"col\" align=\"left\"><span class=\"color\">"._OL_NICK."</span></th>
					<th scope=\"col\" align=\"left\"><span class=\"color\">Realname</span></th>
					<th scope=\"col\" align=\"left\"><span class=\"color\">"._OL_ONLINE." / "._OL_LASTQUIT."</span></th>
				  </tr>";	
				for ($i = 0; $i < count($seendata); $i++) {
					if ($seendata[$i]['bot'] == "Y") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_bot[0]."\" alt=\""._OL_BOT."\" title=\""._OL_BOT."\" />";
					}
					elseif ($seendata[$i]['uline'] == "1") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_service[0]."\" alt=\""._OL_SERVICE."\" title=\""._OL_SERVICE."\" />";
					}
					elseif ($seendata[$i]['away'] == "Y") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_away[0]."\" alt=\""._OL_AWAY."\" title=\""._OL_AWAY."\" />";
					}
					elseif ($seendata[$i]['helper'] == "Y") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_helper[0]."\" alt=\""._OL_AVAILABLE."\" title=\""._OL_AVAILABLE."\" />";
					}
					elseif ($seendata[$i]['online'] == "Y") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_online[0]."\" alt=\"Online\" title=\"Online\" />";
					}
					if ($seendata[$i]['online'] == "N") {
						$user_status = "<img src=\"themes/$theme/img/".$tpl_status_offline[0]."\" alt=\"Offline\" title=\"Offline\" />";
						$user_time = $seendata[$i]['lastquit'];
					}
					else {
						$user_time = $seendata[$i]['connecttime'];
					}
					$flagfile = "libs/phpdenora/flags/".strtolower($seendata[$i]['countrycode']).".png";
					$countryflag = file_exists($flagfile) ? "<img src=\"".$flagfile."\" alt=\"".$seendata[$i]['country']."\" title=\"".$seendata[$i]['country']."\" />" : "&nbsp;";
					echo "<tr><td rowspan=\"2\" align=\"center\" class=\"l t b n\"><strong>".($i + 1 +($pd_rlimit * ($subpage - 1)))."</strong></td>";
					echo "<td rowspan=\"2\" class=\"t b\" align=\"center\" valign=\"middle\">".$user_status."</td>";
					echo "<td rowspan=\"2\" class=\"t b\" align=\"center\" valign=\"middle\">".$countryflag."</td>";
					echo "<td align=\"left\" class=\"t\"><a href=\"?m=u&amp;p=ustats&amp;nick=".urlencode($seendata[$i]['nick'])."\">".$seendata[$i]['nick']."</a></td>";
					echo "<td align=\"left\" class=\"t\">".denora_colorconvert($seendata[$i]['realname'])."</td>";
					echo "<td align=\"left\" class=\"t r\">".strftime($denora_format_time,$user_time)."</td>";
					echo "</tr>";
					echo "<tr>";
					echo "<td colspan=\"4\" align=\"left\" class=\"b r\">".$seendata[$i]['username']."@".$seendata[$i]['hostname']."</td>";
					echo "</tr>";
					echo "<tr><td colspan=\"5\"></td></tr>";
				}
				echo "</table>";
			}
		}
	}
}
else
{
	$page = isset($page) ? $page : "chaninfo";
	echo "<div class=\"title\"><a name=\"chansearch\"></a>"._TM_SEARCH."</div>
		<form id=\"searchform\" method=\"get\" action=\"?m=s\">
			<div>
				<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"5\">
					<tr>
						<td style=\"width: 50%;\" valign=\"top\">
							<fieldset><legend>"._TM_SEARCH."</legend>
								<p>&nbsp;</p>
								<p>"._SP_PROMPT."</p>
								<p>&nbsp;</p>
								<label for=\"search\">"._TM_SEARCH."</label>
								<input type=\"text\" name=\"search\" id=\"search\" />
								<p>&nbsp;</p>
							</fieldset>
						</td>
						<td style=\"width: 50%;\" valign=\"top\">
							<fieldset><legend>"._GR_OPTIONS."</legend>
								<p>&nbsp;</p>
								<p>
								<input type=\"hidden\" name=\"m\" value=\"s\" />
								<label><input type=\"radio\" name=\"p\" value=\"chaninfo\" ";
								if ($page == "chaninfo") { echo "checked=\"checked\" "; }
								echo "/>"._SP_CHANINFO."</label><br />
								<label><input type=\"radio\" name=\"p\" value=\"chanstats\" ";
								if ($page == "chanstats") { echo "checked=\"checked\" "; }
								echo "/>"._SP_CHANSTATS."</label><br />
								<label><input type=\"radio\" name=\"p\" value=\"userinfo\" ";
								if ($page == "userinfo") { echo "checked=\"checked\" "; }
								echo "/>"._SP_USERINFO."</label><br />
								<label><input type=\"radio\" name=\"p\" value=\"userstats\" ";
								if ($page == "userstats") { echo "checked=\"checked\" "; }
								echo "/>"._SP_USERSTATS."</label><br />
								</p>
								<p>&nbsp;</p>
							</fieldset>
						</td>
					</tr>
				</table>
				<p>&nbsp;</p>
				<p class=\"right\"><input type=\"submit\" value=\""._SP_SUBMIT."\" /></p>
			</div>
		</form>";
}

?>
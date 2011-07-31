<?php
// $Id: graphs.php 337 2008-02-05 17:19:40Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if ($page == "netstats") {
	switch ($t) {
		case 'y':
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 year")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'm':
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 month")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'w':
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 week")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'd':
			$graphdate = date('j') . " " . strftime("%B",mktime(0,0,0,date('n'))) . " " . date('Y');
			echo "<div class=\"title\">"._TM_USERS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users\" alt=\"\" />";
			echo "<div class=\"title\">"._TM_CHANS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels\" alt=\"\" />";
			echo "<div class=\"title\">"._SM_SERVERS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'u':
			$graphdate = date('j') . " " . strftime("%B",mktime(0,0,0,date('n'))) . " " . date('Y');
			echo "<div class=\"title\">"._TM_USERS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 week")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 month")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 year")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_USERS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'c':
			$graphdate = date('j') . " " . strftime("%B",mktime(0,0,0,date('n'))) . " " . date('Y');
			echo "<div class=\"title\">"._TM_CHANS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 week")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 month")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 year")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._TM_CHANS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 's':
			$graphdate = date('j') . " " . strftime("%B",mktime(0,0,0,date('n'))) . " " . date('Y');
			echo "<div class=\"title\">"._SM_SERVERS." - $graphdate</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 week")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PWSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 month")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PMSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$start_date = explode("/",strftime('%m/%d/%Y',strtotime("-1 year")));
			$date_url = "&amp;sy=".$start_date[2]."&amp;sm=".$start_date[0]."&amp;sd=".$start_date[1];
			echo "<div class=\"title\">"._SM_SERVERS." - "._SM_PYSTATS."</div>";
			echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers$date_url\" alt=\"\" />";
			$gqcount = $gqcount + 9;
			break;
		case 'x':
			$sy = (isset($_GET['sy'])) ? htmlspecialchars($_GET['sy']) : NULL;
			$sm = (isset($_GET['sm'])) ? htmlspecialchars($_GET['sm']) : NULL;
			$sd = (isset($_GET['sd'])) ? htmlspecialchars($_GET['sd']) : NULL;
			$ey = (isset($_GET['ey'])) ? htmlspecialchars($_GET['ey']) : NULL;
			$em = (isset($_GET['em'])) ? htmlspecialchars($_GET['em']) : NULL;
			$ed = (isset($_GET['ed'])) ? htmlspecialchars($_GET['ed']) : NULL;
			$users = (isset($_GET['users'])) ? htmlspecialchars($_GET['users']) : NULL;
			$channels = (isset($_GET['channels'])) ? htmlspecialchars($_GET['channels']) : NULL;
			$servers = (isset($_GET['servers'])) ? htmlspecialchars($_GET['servers']) : NULL;
			
			if ($sy == NULL) { /* Show the submit form */
				$q = sql_query("SELECT DISTINCT `year` FROM `$denora_userstats`");
				$ystr = NULL; $mstr = NULL; $dstr = NULL;
				while ($r = sql_fetch_array($q)) { $ystr .= "<option value=\"".$r[0]."\">".$r[0]."</option>\n"; }
				for ($i = 1; $i <= 12; $i++) { $mstr .= "<option value=\"$i\">$i</option>\n"; }
				for ($i = 1; $i <= 31; $i++) { $dstr .= "<option value=\"$i\">$i</option>\n"; }
				
				echo "<div class=\"title\">"._GR_CUSTOM."</div>
					<form id=\"customgraphs\" method=\"get\" action=\"\">
					<p>
  				    <input name=\"m\" type=\"hidden\" value=\"g\" />
					<input name=\"p\" type=\"hidden\" value=\"netstats\" />
					<input name=\"t\" type=\"hidden\" value=\"x\" />
					</p>
					<table border=\"0\" cellspacing=\"2\" cellpadding=\"2\"><tr><td valign=\"top\">
					<fieldset><legend>"._GR_SDATE."</legend>
						<table border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
							<tr>
								<td>
									<label for=\"sy\">"._GD_YEAR."</label>
								</td>
								<td>
									<select name=\"sy\" id=\"sy\">
										<option value=\"".date('Y')."\" selected=\"selected\">"._GR_SELECT."</option>
										$ystr
									</select><br />
								</td>
							</tr>
							<tr>
								<td>
									<label for=\"sm\">"._GD_MONTH."</label>
								</td>
								<td>
									<select name=\"sm\" id=\"sm\">
										<option value=\"".date('n')."\" selected=\"selected\">"._GR_SELECT."</option>
										$mstr
									</select><br />
								</td>
							</tr>
							<tr>
								<td>
									<label for=\"sd\">"._GD_DAY."</label>
								</td>
								<td>
									<select name=\"sd\" id=\"sd\">
										<option value=\"".date('j')."\" selected=\"selected\">"._GR_SELECT."</option>
										$dstr
									</select>
								</td>
							</tr>
						</table>
					</fieldset>
					</td><td>
					<fieldset><legend>"._GR_EDATE."</legend>
						<table border=\"0\" cellspacing=\"2\" cellpadding=\"2\">
							<tr>
								<td>
									<label for=\"ey\">"._GD_YEAR."</label>
								</td>
								<td>
									<select name=\"ey\" id=\"ey\">
										<option value=\"".date('Y')."\" selected=\"selected\">"._GR_SELECT."</option>
										$ystr
									</select>
								</td>
							</tr>
							<tr>
								<td>
									<label for=\"em\">"._GD_MONTH."</label>
								</td>
								<td>
									<select name=\"em\" id=\"em\">
										<option value=\"".date('n')."\" selected=\"selected\">"._GR_SELECT."</option>
										$mstr
									</select>
								</td>
							</tr>
							<tr>
								<td>
									<label for=\"ed\">"._GD_DAY."</label>
								</td>
								<td>
									<select name=\"ed\" id=\"ed\">
										<option value=\"".date('j')."\" selected=\"selected\">"._GR_SELECT."</option>
										$dstr
									</select>
								</td>
							</tr>
						</table>
					</fieldset>
					</td></tr>
					<tr><td colspan=\"2\" valign=\"top\">
					<fieldset><legend>"._GR_OPTIONS."</legend>
				  <p>
					<input name=\"users\" type=\"checkbox\" id=\"users\" value=\"true\" checked=\"checked\" />
					<label for=\"users\">"._TM_USERS."</label>
					<input name=\"channels\" type=\"checkbox\" id=\"channels\" value=\"true\" checked=\"checked\" />
					<label for=\"channels\">"._TM_CHANS."</label>
					<input name=\"servers\" type=\"checkbox\" id=\"servers\" value=\"true\" checked=\"checked\" />
					<label for=\"servers\">"._SM_SERVERS."</label>
				  </p>
				  </fieldset>
				  </td></tr>
				  <tr><td colspan=\"2\" align=\"right\">
				  <p class=\"right\">
					<input type=\"submit\" name=\"Submit\" value=\""._GR_GENERATE."\" id=\"Submit\" />
				  </p>
				  </td></tr></table>
				</form>";
			}
			else { /* Show the Graphs */
				$graphdate = strftime('%D',mktime(0,0,0,$sm,$sd,$sy))." - ".strftime('%D',mktime(0,0,0,$em,$ed,$ey));
				if ($users == "true") {
					echo "<div class=\"title\">"._TM_USERS.", $graphdate</div>";
					echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=users&amp;sy=$sy&amp;sm=$sm&amp;sd=$sd&amp;ey=$ey&amp;em=$em&amp;ed=$ed\" alt=\"\" />";
					$gqcount = $gqcount + 3;
				}
				if ($channels == "true") {
					echo "<div class=\"title\">"._TM_CHANS.", $graphdate</div>";
					echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=channels&amp;sy=$sy&amp;sm=$sm&amp;sd=$sd&amp;ey=$ey&amp;em=$em&amp;ed=$ed\" alt=\"\" />";
					$gqcount = $gqcount + 3;
				}
				if ($servers == "true") {
					echo "<div class=\"title\">"._SM_SERVERS.", $graphdate</div>";
					echo "<img src=\"libs/phpdenora/graphs/line.php?theme=$theme&amp;lang=$lang&amp;mode=servers&amp;sy=$sy&amp;sm=$sm&amp;sd=$sd&amp;ey=$ey&amp;em=$em&amp;ed=$ed\" alt=\"\" />";
					$gqcount = $gqcount + 3;
				}
				
			}
			break;
		default: phpdenora_error(1,$t);
	}
}
elseif ($page == "netsplit") {
	switch ($t) {
		case 'w':
			$graphtest = @imagecreatefrompng("http://irc.netsplit.de/tmp/networks/weeks_".$pd_netsplit."_uc.png");
			if (!$graphtest) {
				$graphforce = @fopen("http://irc.netsplit.de/networks/details.php.en?net=".$pd_netsplit."&submenu=weeks",'r');
				if ($graphforce) {
					fclose($graphforce);
				}
			} else {
				imagedestroy($graphtest);
			}		
			echo "<div class=\"title\">"._PT_NSU2W."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/weeks_".$pd_netsplit."_uc.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSC2W."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/weeks_".$pd_netsplit."_c.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSS2W."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/weeks_".$pd_netsplit."_s.png\" alt=\"\" />";
			break;
		case 'm':
			$graphtest = @imagecreatefrompng("http://irc.netsplit.de/tmp/networks/months_".$pd_netsplit."_uc.png");
			if (!$graphtest) {
				$graphforce = @fopen("http://irc.netsplit.de/networks/details.php.en?net=".$pd_netsplit."&submenu=months",'r');
				if ($graphforce) {
					fclose($graphforce);
				}
			} else {
				imagedestroy($graphtest);
			}		
			echo "<div class=\"title\">"._PT_NSU2M."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/months_".$pd_netsplit."_uc.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSC2M."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/months_".$pd_netsplit."_c.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSS2M."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/months_".$pd_netsplit."_s.png\" alt=\"\" />";
			break;
		case 'y':
			$graphtest = @imagecreatefrompng("http://irc.netsplit.de/tmp/networks/years_".$pd_netsplit."_uc.png");
			if (!$graphtest) {
				$graphforce = @fopen("http://irc.netsplit.de/networks/details.php.en?net=".$pd_netsplit."&submenu=years",'r');
				if ($graphforce) {
					fclose($graphforce);
				}
			} else {
				imagedestroy($graphtest);
			}		
			echo "<div class=\"title\">"._PT_NSU2Y."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/years_".$pd_netsplit."_uc.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSC2Y."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/years_".$pd_netsplit."_c.png\" alt=\"\" />";
			echo "<div class=\"title\">"._PT_NSS2Y."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/years_".$pd_netsplit."_s.png\" alt=\"\" />";		
			break;
		case 'h':
			$graphtest = @imagecreatefrompng("http://irc.netsplit.de/tmp/networks/history_".$pd_netsplit."_uc.png");
			if (!$graphtest) {
				$graphforce = @fopen("http://irc.netsplit.de/networks/details.php.en?net=".$pd_netsplit."&submenu=years",'r');
				if ($graphforce) {
					fclose($graphforce);
				}
			} else {
				imagedestroy($graphtest);
			}
			echo "<div class=\"title\">"._PT_NSHYS."</div>";
			echo "<img src=\"http://irc.netsplit.de/tmp/networks/history_".$pd_netsplit."_uc.png\" alt=\"\" />";
			break;
		default: phpdenora_error(1,$t);
	}
}
elseif ($page == "searchirc") {
	echo "<div class=\"title\">"._SM_DSTATS."</div>";
	echo "<img src=\"http://searchirc.com/graphs/img.php?n=".$pd_searchirc."&amp;ts=1&amp;t=7&amp;d=graphs\" alt=\"\" />";
	echo "<div class=\"title\">"._SM_PWSTATS."</div>";
	echo "<img src=\"http://searchirc.com/graphs/img.php?n=".$pd_searchirc."&amp;ts=2&amp;t=7&amp;d=graphs\" alt=\"\" />";
	echo "<div class=\"title\">"._SM_PMSTATS."</div>";
	echo "<img src=\"http://searchirc.com/graphs/img.php?n=".$pd_searchirc."&amp;ts=3&amp;t=7&amp;d=graphs\" alt=\"\" />";
	echo "<div class=\"title\">"._SM_PYSTATS."</div>";
	echo "<img src=\"http://searchirc.com/graphs/img.php?n=".$pd_searchirc."&amp;ts=4&amp;t=7&amp;d=graphs\" alt=\"\" />";
}
else {
	phpdenora_error(1,$page);
}
?>

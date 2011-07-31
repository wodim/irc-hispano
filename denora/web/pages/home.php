<?php
// $Id: home.php 415 2011-02-03 23:45:46Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if (!isset($page) || $page == "home") {
	if (file_exists('welcome.php')) {
		include('welcome.php');
	}
	else {
		echo "<p style=\"color:red;\">Please edit welcome.dist.php to your needs and rename it to welcome.php</p>\n";
	}
	echo "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\"><tr>";
	if ($pd_searchirc_ranking == true) echo "<td valign=\"top\" align=\"center\">";
	else echo "<td valign=\"top\" align=\"center\" colspan=\"3\">";
			echo "<div class=\"title\">"._FP_STATUS."</div>";
			$fp_current = phpdenora_current();
			$fp_max = phpdenora_maxstats();			
	echo "<table width=\"250\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">
			<tr>
			  <td align=\"center\"><img src=\"themes/".$theme."/img/".$tpl_network_users[0]."\" width=\"".$tpl_network_opers[1]."\" height=\"".$tpl_network_opers[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_USERS."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_current['value'][0]."</span></td>
			  <td align=\"center\" valign=\"middle\"><img src=\"themes/".$theme."/img/".$tpl_network_max[0]."\" width=\"".$tpl_network_max[1]."\" height=\"".$tpl_network_max[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_MAX."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_max['value'][0]."</span></td>
			</tr>
			<tr>
			  <td align=\"center\"><img src=\"themes/".$theme."/img/".$tpl_network_chans[0]."\" width=\"".$tpl_network_opers[1]."\" height=\"".$tpl_network_opers[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_CHANS."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_current['value'][1]."</span></td>
			  <td align=\"center\" valign=\"middle\"><img src=\"themes/".$theme."/img/".$tpl_network_max[0]."\" width=\"".$tpl_network_max[1]."\" height=\"".$tpl_network_max[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_MAX."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_max['value'][1]."</span></td>
			</tr>
			<tr>
			  <td align=\"center\"><img src=\"themes/".$theme."/img/".$tpl_network_opers[0]."\" width=\"".$tpl_network_opers[1]."\" height=\"".$tpl_network_opers[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_OPERS."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_current['value'][2]."</span></td>
			  <td align=\"center\" valign=\"middle\"><img src=\"themes/".$theme."/img/".$tpl_network_max[0]."\" width=\"".$tpl_network_max[1]."\" height=\"".$tpl_network_max[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_MAX."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_max['value'][2]."</span></td>
			</tr>
			<tr>
			  <td align=\"center\"><img src=\"themes/".$theme."/img/".$tpl_network_servers[0]."\" width=\"".$tpl_network_servers[1]."\" height=\"".$tpl_network_servers[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_SERVERS."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_current['value'][3]."</span></td>
			  <td align=\"center\" valign=\"middle\"><img src=\"themes/".$theme."/img/".$tpl_network_max[0]."\" width=\"".$tpl_network_max[1]."\" height=\"".$tpl_network_max[2]."\" alt=\"\" /></td>
			  <td align=\"left\">"._FP_MAX."</td>
			  <td align=\"right\"><span class=\"color\">".$fp_max['value'][3]."</span></td>
			</tr>
		  </table>
		</td>";
			if ($pd_searchirc_ranking == true) {
				echo "<td>&nbsp;</td>
				<td valign=\"top\" align=\"center\">
				<div class=\"title\">"._FP_RANKING."</div>
				<p class=\"center\"><script type=\"text/javascript\" src=\"http://searchirc.com/official_rank.php?ID=".$pd_searchirc."&amp;outof=1\"></script></p>
				</td>";
			}
	echo "</tr><tr>
		<td valign=\"top\" colspan=\"3\">
			<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\"><tr>
			<td valign=\"top\" style=\"width:33%;\"><div class=\"title\">".sprintf(_FP_BIGGESTCHANS,$pd_fptop)."</div>
			<table class=\"stats\" width=\"95%\" cellspacing=\"0\" cellpadding=\"1\">
			<tr><th align=\"right\" class=\"color\">"._GD_NUMBER."</th><th class=\"color\">"._GD_CHANNEL."</th><th align=\"right\" class=\"color\">"._GD_USERS."</th></tr>";
			$chanarray = phpdenora_chanlist(1,$pd_fptop);
			$i = 1;
			foreach ($chanarray as $chanvals) {
				$bchan = (strlen($chanvals['chan']) > 20) ? substr($chanvals['chan'], 0, 17) . "..." : $chanvals['chan'];
				$bchan = htmlspecialchars($bchan);
				echo "<tr class=\"bg\"><td align=\"right\" class=\"t l b n\"><strong>".$i++."</strong></td>
					<td class=\"t b\"><a href=\"?m=c&amp;p=status&amp;chan=".urlencode($chanvals['chan'])."\">".$bchan."</a></td>
					<td align=\"right\" class=\"t r b\">".$chanvals['users']."</td></tr><tr><td colspan=\"3\"></td></tr>";
			}
	echo "</table></td><td>&nbsp;</td><td valign=\"top\" style=\"width:33%;\"><div class=\"title\">".sprintf(_FP_TOPCHANS,$pd_fptop)."</div>
		<table class=\"stats\" width=\"95%\" cellspacing=\"0\" cellpadding=\"1\">
		<tr><th align=\"right\" class=\"color\">"._GD_NUMBER."</th><th class=\"color\">"._GD_CHANNEL."</th><th align=\"right\" class=\"color\">"._GD_LINES."</th></tr>";
			$topchans = phpdenora_topchans($pd_fptop);
			$i = 1;
			foreach($topchans as $topchan) {
				$pchan = (strlen($topchan['chan']) > 20) ? substr($topchan['chan'], 0, 17) . "..." : $topchan['chan'];
				$pchan = htmlspecialchars($pchan);
				echo "<tr class=\"bg\"><td align=\"right\" class=\"t l b n\"><strong>".$i++."</strong></td>
					<td class=\"t b\"><a href=\"?chan=".urlencode($topchan['chan'])."&amp;type=1\">".$pchan."</a></td>
					<td align=\"right\" class=\"t r b\">".$topchan['lines']."</td></tr><tr><td colspan=\"3\"></td></tr>";
			}
	echo "</table></td><td>&nbsp;</td><td valign=\"top\" style=\"width:33%;\"><div class=\"title\">".sprintf(_FP_TOPUSERS,$pd_fptop)."</div>
			<table class=\"stats\" width=\"95%\" cellspacing=\"0\" cellpadding=\"1\">
			<tr><th align=\"right\" class=\"color\">"._GD_NUMBER."</th><th class=\"color\">"._GD_NICK."</th><th align=\"right\" class=\"color\">"._GD_LINES."</th></tr>";
			$topusers = phpdenora_topusers($pd_fptop);
			$i = 1;
			foreach($topusers as $topuser) {
				$pnick = (strlen($topuser['user']) > 20) ? substr($topuser['user'], 0, 17) . "..." : $topuser['user'];
				echo "<tr class=\"bg\"><td align=\"right\" class=\"t l b n\"><strong>".$i++."</strong></td>
					<td class=\"t b\"><a href=\"?user=".urlencode($topuser['user'])."&amp;type=1\">".$pnick."</a></td>
					<td align=\"right\" class=\"t r b\">".$topuser['lines']."</td></tr><tr><td colspan=\"3\"></td></tr>";
			}
	echo "</table>
			</td></tr></table>
		</td></tr><tr>
		<td valign=\"top\" colspan=\"3\">
			<div class=\"title\">"._FP_QSTATS."</div>
			<form id=\"quick\" method=\"get\" action=\"\">
			<p>
			  <label for=\"nick\"><strong>Nick</strong></label>
			  <input type=\"text\" name=\"nick\" tabindex=\"1\" id=\"nick\" size=\"18\" /> "._GD_OR."
			  <label for=\"user\"><strong>"._FP_QUSER."</strong></label>
			  <input type=\"text\" name=\"user\" tabindex=\"2\" id=\"user\" size=\"18\" /> "._GD_AND."/"._GD_OR."
			  <label for=\"chan\"><strong>"._FP_QCHAN."</strong></label>
			  <strong>#</strong>
			  <input type=\"text\" name=\"chan\" tabindex=\"3\" id=\"chan\" size=\"18\" />
			  <input type=\"submit\" value=\""._FP_GO."\" tabindex=\"3\" />
			</p>
			</form>
		</td></tr><tr>
		<td valign=\"top\" colspan=\"3\">";
			if (extension_loaded('gd') == 1 && $pd_ennetgraphs == true) {
				echo"<div class=\"title\">"._FP_LSTATUS."</div>
				<img src=\"libs/phpdenora/graphs/line.php?size=small&amp;theme=$theme&amp;lang=$lang&amp;mode=users\" alt=\"\" />
				<img src=\"libs/phpdenora/graphs/line.php?size=small&amp;theme=$theme&amp;lang=$lang&amp;mode=channels\" alt=\"\" />
				<img src=\"libs/phpdenora/graphs/line.php?size=small&amp;theme=$theme&amp;lang=$lang&amp;mode=servers\" alt=\"\" />\n";
				$gqcount = $gqcount + 9;
			}
			elseif ($pd_searchirc_graphs == true) {
				echo"<div class=\"title\">"._FP_LSTATUS."</div>
				<img src=\"http://searchirc.com/graphs/img.php?n=".$pd_searchirc."&amp;ts=1&amp;t=7&amp;d=graphs\" alt=\"\" />";
			}
		echo "</td>
		</tr></table>";	
}
else {
	phpdenora_error(1,$page);
}
?>
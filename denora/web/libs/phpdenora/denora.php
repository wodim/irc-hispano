<?php
// $Id: denora.php 415 2011-02-03 23:45:46Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

/* Prints a list of the current channels */
function denora_list_chans($search)
{
	// Initialize needed variables
	global $ircd, $denora_irc_server, $denora_chan_db, $denora_ison_db, $order, $pd_rlimit, $theme, $tpl_asc, $tpl_desc, $sort, $subpage, $pd_mirc, $pd_mirc_url, $pd_webchat, $pd_webchat_url, $pd_mibbit, $pd_mibbit_key, $tpl_mirc, $tpl_webchat, $denora_noshow_chans;
	$search = str_replace("*", "%", $search); $i = 1; $array = array();
	if ($sort != "channel") { $sort = "currentusers"; }
	
	// Get the stuff FROM the database
	$a1 = ($ircd['secretchan'] == 1) ? "AND ".$ircd['secretchan_mode']."='N' " : NULL;
	$a1 .= ($ircd['privatechan'] == 1) ? "AND ".$ircd['privatechan_mode']."='N' " : NULL;
	$a2 = ($search) ? "AND LOWER(channel) LIKE '%".sql_escape_string(strtolower($search))."%'" : "";
	$no = array(); $c1 = NULL;
	$no = explode(",",$denora_noshow_chans);
	for ($i = 0; $i < count($no); $i++) {
		$c1 .= " AND LOWER(channel) NOT LIKE '".sql_escape_string(strtolower($no[$i]))."'";
	}
	$qsumq = sql_query("SELECT COUNT(channel) FROM $denora_chan_db WHERE currentusers > 0 $a1 $a2 $c1;");
	$qsum = sql_fetch_array($qsumq);
	$qlimit = ( $subpage - 1 ) * $pd_rlimit;
	$rchan2 = (($subpage * $pd_rlimit) < $qsum[0]) ? ($subpage * $pd_rlimit) : $qsum[0];
	echo "<table width=\"95%\" border=\"0\" class=\"center\">
		  <tr>
			<td align=\"left\">";
	echo sprintf(_PS_CHANSHOW,"<strong>".($subpage * $pd_rlimit - $pd_rlimit + 1)."</strong>","<strong>".$rchan2."</strong>","<strong>".$qsum[0]."</strong><br />");
	echo "</td>
			<td align=\"right\">";	
	if ($search == NULL)
    {
		$l1 = "m=c&amp;p=list";
		$l2 = NULL;
	}
	else
	{
		$l1 = "m=s&amp;p=chaninfo";
		$l2 = "&amp;search=".stripslashes($search);
	}
	echo "<h1>";
	// display the << button
	if ($subpage == 1) { echo "&laquo; "; }
	else { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order$l2&amp;sub=1\">&laquo;</a> "; }
	// display the < button
	if ($subpage > 1) { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order$l2&amp;sub=" . ($subpage - 1) . "\">&#8249;</a> "; }
	else { echo "&#8249; "; }
	// display the > button
	if ($subpage < $qsum[0]/$pd_rlimit) { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order$l2&amp;sub=" . ($subpage + 1) . "\">&#8250;</a> "; }
	else { echo "&#8250; "; }
	// display the >> button
	if ($rchan2 == $qsum[0]) { echo "&raquo; "; }
	else { echo "<a href=\"?$l1&amp;sort=$sort&amp;order=$order$l2&amp;sub=".ceil($qsum[0] / $pd_rlimit)."\">&raquo;</a> "; }
	echo "</h1>";
	echo "</td>
		  </tr>
		</table>";
		
	$query = sql_query("SELECT * FROM $denora_chan_db WHERE currentusers > 0 $a1 $a2 $c1 ORDER BY $denora_chan_db.".sql_escape_string($sort)." ".strtoupper($order)." LIMIT $qlimit , $pd_rlimit;");
	for ($i = 0; $data = sql_fetch_array($query); $i++)
	{
		$array[$i]['channel'] = $data['channel'];
		$array[$i]['users'] = $data['currentusers'];
		$array[$i]['topic'] = $data['topic'];

		$cmodes = NULL; $j=97;
		while ($j <= 122)
		{
			if (@$data['mode_l'.chr($j)] == "Y")
			{
				$cmodes .= chr($j);
			}
			if (@$data['mode_u'.chr($j)] == "Y")
			{
				$cmodes .= chr($j-32);
			}
			$j++;
		}
		if (@$data['mode_lf_data'] != NULL)
		{
			$cmodes .= " ".$data['mode_lf_data'];
		}
		if (@$data['mode_lj_data'] != NULL)
		{
			$cmodes .= " ".$data['mode_lj_data'];
		}
		if (@$data['mode_ll_data'] > 0)
		{
			$cmodes .= " ".$data['mode_ll_data'];
		}
		if (@$data['mode_uf_data'] != NULL)
		{
			$cmodes .= " ".$data['mode_uf_data'];
		}
		if (@$data['mode_uj_data'] > 0)
		{
			$cmodes .= " ".$data['mode_uj_data'];
		}
		if (@$data['mode_ul_data'] != NULL)
		{
			$cmodes .= " ".$data['mode_ul_data'];
		}
		
		$array[$i]['modes'] = ($cmodes == NULL) ? NULL : "+" . $cmodes;
	}
	
	// Generate the output
	$str = "<table class=\"stats\" width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">
		  <tr>
		   <th class=\"color\" scope=\"col\">n.</th>
		   <th class=\"color\" scope=\"col\">&nbsp;</th>
		   <th class=\"color\" scope=\"col\">&nbsp;</th>";
		   
			if ($order == "desc")
			{
				if ($sort == "channel")
				{
					$olink1 = "asc";
					$olink2 = "desc";
					$pic1 = " <img src=\"themes/".$theme."/img/".$tpl_desc[0]."\" alt=\""._PS_DESCENDING."\" title=\""._PS_DESCENDING."\" />";
					$pic2 = "";
				}
				elseif ($sort == "currentusers")
				{
					$olink1 = "asc";
					$olink2 = "asc";
					$pic1 = "";
					$pic2 = " <img src=\"themes/".$theme."/img/".$tpl_desc[0]."\" alt=\""._PS_DESCENDING."\" title=\""._PS_DESCENDING."\" />";
				}
			}
			elseif ($order == "asc")
			{
				if ($sort == "channel")
				{
					$olink1 = "desc";
					$olink2 = "desc";
					$pic1 = " <img src=\"themes/".$theme."/img/".$tpl_asc[0]."\" alt=\""._PS_ASCENDING."\" title=\""._PS_ASCENDING."\" />";
					$pic2 = "";
				}
				elseif ($sort == "currentusers")
				{
					$olink1 = "asc";
					$olink2 = "desc";
					$pic1 = "";
					$pic2 = " <img src=\"themes/".$theme."/img/".$tpl_asc[0]."\" alt=\""._PS_ASCENDING."\" title=\""._PS_ASCENDING."\" />";
				}
			}
			
		    $str .= "<th class=\"color\" scope=\"col\"><a href=\"?$l1&amp;sort=channel&amp;order=$olink1$l2&amp;sub=$subpage\">"._GD_CHANNEL."</a>$pic1</th>
		   		<th class=\"color\" scope=\"col\"><a href=\"?$l1&amp;sort=currentusers&amp;order=$olink2$l2&amp;sub=$subpage\">"._GD_USERS."</a>$pic2</th>";
			$str .= "<th class=\"color\" scope=\"col\">"._GD_MODES."</th></tr>";
	for ($i = 0; $i < count($array); $i++)
	{
		$topic = (strlen($array[$i]['topic']) == 0) ? "&nbsp;" : denora_colorconvert($array[$i]['topic']);
		$cmodes = (strlen($array[$i]['modes']) == 0) ? "&nbsp;" : $array[$i]['modes'];
		$icons = NULL;
		if ($pd_mirc == true) {
			$icons .= "<a href=\"".$pd_mirc_url.urlencode(substr($array[$i]['channel'],1))."\"><img src=\"themes/".$theme."/img/".$tpl_mirc[0]."\" alt=\"Chat\" title=\"Chat\" /></a>";
		}
		if ($pd_webchat == true) {
			$icons .= "<a href=\"".$pd_webchat_url.urlencode($array[$i]['channel'])."\"><img src=\"themes/".$theme."/img/".$tpl_webchat[0]."\" alt=\"Web Chat\" title=\"Web Chat\" /></a>";
		}
		if ($pd_mibbit == true) {
			$mibbit_url = sprintf("http://widget.mibbit.com/?settings=%s&amp;server=%s&amp;channel=%s&amp;promptPass=true", $pd_mibbit_key, $denora_irc_server, urlencode($array[$i]['channel']));
			$icons .= "<a href=\"".$mibbit_url."\"><img src=\"themes/".$theme."/img/".$tpl_webchat[0]."\" alt=\"Mibbit\" title=\"Mibbit\" /></a>";
		}
		$icons = ($icons == NULL) ? "&nbsp;" : $icons;
		$str .= sprintf("
			<tr class=\"bg\">
				<td rowspan=\"2\" align=\"center\" class=\"l t b n\"><strong>%s</strong></td>
				<td rowspan=\"2\" class=\"t b\">&nbsp;</td>
				<td align=\"center\" class=\"t\" style=\"white-space: nowrap;\">%s</td>
				<td class=\"t\"><a href=\"%s\">%s</a></td>
				<td class=\"t\">%s</td>
				<td class=\"r t\">%s</td>
			</tr>
			<tr class=\"bg\">
				<td colspan=\"4\" class=\"r b\">%s</td>
			</tr>
			<tr>
				<td colspan=\"5\"></td>
			</tr>
			",
			$i + 1 + ($pd_rlimit * ($subpage - 1)),
			$icons,
			"?m=c&amp;p=status&amp;chan=".urlencode($array[$i]['channel']),
			htmlspecialchars($array[$i]['channel']),
			$array[$i]['users'],
			$cmodes,
			$topic);
	}
	$str .= "</table>";
	
	return $str;
}

function makeBarTable($value,$maxvalue,$width)
{
	$gwidth = ($maxvalue != 0) ? ceil($width * $value / $maxvalue) : 0;
	echo "
	<table cellpadding=\"0\" cellspacing=\"0\" class=\"bar\">
		<tr>
			<td class=\"fill\" style=\"width: ". $gwidth ."px;\"></td>
			<td class=\"nofill\" style=\"width: ". ($width - $gwidth) ."px;\"></td>
		</tr>
	</table>
	";
}

function displayFooter()
{
	global $pd_valid, $pd_exec, $querycount, $pd_graphcache, $gqcount, $qcount, $start, $compressed, $denora_server_type;
	
	echo "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td align=\"left\">";
	if ($pd_valid == true) {	
		echo "<h6><a href=\"http://validator.w3.org/check/referer\">XHTML 1.0 Validator</a> - <a href=\"http://jigsaw.w3.org/css-validator/check/referer\">CSS Validator</a></h6>";
	}
	else {
		echo "&nbsp;";
	}
	echo "</td><td align=\"center\">";
	if ($pd_exec == true) {
		$querycount = ($pd_graphcache == true && $gqcount != 0) ? ($qcount . "-" . ($qcount + $gqcount)) : ($qcount + $gqcount);
		if ($compressed == true) {
			echo sprintf("<h6>"._OT_CGENTIME, round(((time() + microtime())-$start),2), $querycount);
		}
		else {
			echo sprintf("<h6>"._OT_GENTIME, round(((time() + microtime())-$start),2), $querycount);
		}
		echo "</h6>";
	}
	else {
		echo "&nbsp;";
	}
	echo "</td> 
        <td><h6 class=\"right\">Powered by <a href=\"http://denorastats.org/\">phpDenora</a> v".VERSION_FULL."</h6></td> 
      </tr>
    </table>";
}
?>

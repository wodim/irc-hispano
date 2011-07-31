<?php
// $Id: network.php 366 2009-09-04 09:42:22Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

if (!isset($page) || $page == "status"){
	echo "<div class=\"title\"><a name=\"details\"></a>"._PT_LUSERS."</div>";
	$fp_current = phpdenora_current();
	$fp_max = phpdenora_maxstats();
	for ($i=0; $i<4; $i++) {
		$fp_max['time'][$i] = $fp_max['time'][$i] == "0000-00-00 00:00:00" ? _GD_UNKNOWN : $fp_max['time'][$i];
	}
	echo "<p>";
	echo sprintf(_NS_USERDET, "<span class=\"color\">" . $fp_current['value'][0] . "</span>", "<span class=\"color\">" . $fp_current['value'][4] . "</span>", "<span class=\"color\">" . $fp_max['value'][0] . "</span>", "<span class=\"color\">" . strftime($denora_format_time,$fp_max['time'][0]) . "</span><br />");
	echo sprintf(_NS_CHANDET, "<span class=\"color\">" . $fp_current['value'][1] . "</span>", "<span class=\"color\">" . $fp_max['value'][1] . "</span>", "<span class=\"color\">" . strftime($denora_format_time,$fp_max['time'][1]) . "</span><br />");
	echo sprintf(_NS_OPERDET, "<span class=\"color\">" . $fp_current['value'][2] . "</span>", "<span class=\"color\">" . $fp_max['value'][2] . "</span>", "<span class=\"color\">" . strftime($denora_format_time,$fp_max['time'][2]) . "</span><br />");
	echo sprintf(_NS_SERVDET, "<span class=\"color\">" . $fp_current['value'][3] . "</span>", "<span class=\"color\">" . $fp_max['value'][3] . "</span>", "<span class=\"color\">" . strftime($denora_format_time,$fp_max['time'][3]) . "</span></p>");
	echo "<div class=\"title\"><a name=\"servers\"></a>"._PT_SERVERS."</div>";
	$srvarray = denora_server_array(0);
	$srvarray = sort_by_field($srvarray,"server","asc"); 
	echo "<table class=\"stats\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\"><tr><th class=\"color\">"._DA_SERVER."</th><th class=\"color\">"._DA_CURRENTUSR."</th><th class=\"color\">"._DA_MAXUSR."</th><th class=\"color\">"._GD_STATUS."</th></tr>";
	foreach($srvarray as $server)
	{
		if ($server['online'] == "Y")
		{
			$srvstatus = "<span style=\"color:green\">Online</span>";
		}
		else
		{
			$srvstatus = "<span style=\"color:red\">Offline</span>";
		}
		echo sprintf("<tr class=\"bg\"><td class=\"b l t\"><a href=\"?m=n&amp;p=srvdetails&amp;srv=%s\">%s</a></td><td align=\"right\" class=\"t b\">%s</td><td align=\"right\" class=\"t b\">%s</td><td align=\"center\" class=\"t r b\">%s</td></tr><tr><td></td></tr>", $server['server'], $server['server'], $server['currentusers'], $server['maxusers'], $srvstatus);
	}
	echo "</table>";
	echo "<div class=\"title\"><a name=\"opers\"></a>"._PT_OPERS."</div>";
	$opers = denora_get_opers();
	if ($opers) {
		echo"<table class=\"stats\" width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">
		  <tr>
			<th scope=\"col\" class=\"color\">"._OL_NICK."</th>";
			if ($denora_server_type == "unreal32") {
				echo "<th scope=\"col\" class=\"color\">"._OL_LEVEL."</th>";
			}
			echo "<th scope=\"col\" class=\"color\">"._GD_STATUS."</th>
			<th scope=\"col\" class=\"color\">"._OL_ONLINE."</th>
		  </tr>";
		for ($i = 0; $i < sizeof($opers); $i++) {
			if ($opers[$i]['bot'] == "Y") {
				$oper_status = "<span style=\"color: gray;\">"._OL_BOT."</span>";
			}
			elseif ($opers[$i]['uline'] == "1") {
				$oper_status = "<span style=\"color: gray;\">"._OL_SERVICE."</span>";
			}
			elseif ($opers[$i]['away'] == "Y") {
				$oper_status = "<span style=\"color: purple;\">"._OL_AWAY."</span>";
			}
			elseif ($opers[$i]['helper'] == "Y") {
				$oper_status = "<span style=\"color: blue;\">"._OL_AVAILABLE."</span>";
			}
			else {
				$oper_status = "<span style=\"color: green;\">Online</span>";
			}
			echo "<tr class=\"bg\">
				<td class=\"b l t\"><a href=\"?nick=".$opers[$i]['nick']."\">".$opers[$i]['nick']."</a></td>";
				if ($denora_server_type == "unreal32") {
					echo"<td class=\"t b\">".$opers[$i]['level']."</td>";
				}
				echo"<td class=\"t b\">".$oper_status."</td>
				<td class=\"t r b\">".strftime($denora_format_time,$opers[$i]['connecttime'])."</td>
			  </tr><tr><td></td></tr>";
		}
		echo "</table>";
	}
	else {
		phpdenora_error(3,_ER_NODATA);
	}
	if ($pd_entldstats == true) {
		echo "<div class=\"title\"><a name=\"country\"></a>".sprintf(_PT_TLDPIE,$pd_netname)."</div>";
		echo "<img src=\"libs/phpdenora/graphs/pie.php?theme=$theme&amp;lang=$lang&amp;mode=country&amp;chan=global\" alt=\"\" />";
		$gqcount = $gqcount + 2;
	}
	if ($pd_enverstats == true) {
		echo "<div class=\"title\"><a name=\"client\"></a>".sprintf(_PT_CLIENTPIE,$pd_netname)."</div>";
		echo "<img src=\"libs/phpdenora/graphs/pie.php?theme=$theme&amp;lang=$lang&amp;mode=version&amp;chan=global\" alt=\"\" />";
		$gqcount = $gqcount + 2;
	}
}
elseif ($page == "srvdetails") {
	echo "<div class=\"title\"><a name=\"details\"></a>".sprintf(_PT_DETAILS,$srv)."</div>";
	echo "<p>";
	if (denora_ishidden($srv) == false) {
		$srvdet = phpdenora_srvdetails($srv);
		$uptime = seconds_to_time($srvdet['uptime']);
		$struptime = sprintf(_DA_UPTIME,$uptime['days'],$uptime['hours']);
		echo "<strong>".denora_colorconvert($srvdet['description'])."</strong><br /><br />",
		sprintf(_DA_SRVDET1,"<span class=\"color\">".$srvdet['version']."</span>","<span class=\"color\">".$struptime."</span>");
		if ($srvdet['online'] == "Y")
		{
			echo sprintf("<br />"._DA_SRVDET2,"<span style=\"color:green\">Online</span> "),
			sprintf(_DA_SRVDET3," <span class=\"color\">".strftime($denora_format_time,$srvdet['connecttime'])."</span>");
		}
		else
		{
			echo sprintf("<br />"._DA_SRVDET2,"<span style=\"color:red\">Offline</span>");
		}
		if ($srvdet['lastsplit'] > 0)
		{
			echo sprintf("<br />"._DA_SRVDET4,"<span class=\"color\">".strftime($denora_format_time,$srvdet['lastsplit'])."</span>");
		}
		elseif ($srvdet['lastsplit'] == 0)
		{
			echo sprintf("<br />"._DA_SRVDET4,"<span class=\"color\">"._GD_UNKNOWN."</span>");
		}
		else
		{
			echo sprintf("<br />"._DA_SRVDET4,"<span class=\"color\">"._GD_NEVER."</span>");
		}
		$srvdet['maxpingtime'] = ($srvdet['maxpingtime'] == 0 ? _GD_UNKNOWN : strftime($denora_format_time, $srvdet['maxpingtime']));
		$srvdet['maxusertime'] = ($srvdet['maxusertime'] == 0 ? _GD_UNKNOWN : strftime($denora_format_time, $srvdet['maxusertime']));
		$srvdet['maxopertime'] = ($srvdet['maxopertime'] == 0 ? _GD_UNKNOWN : strftime($denora_format_time, $srvdet['maxopertime']));
		echo sprintf("<br />"._DA_SRVDET5,"<span class=\"color\">".$srvdet['ping']."</span>","<span class=\"color\">".$srvdet['highestping']."</span>","<span class=\"color\">".$srvdet['maxpingtime']."</span>"),
		sprintf("<br /><br />"._DA_SRVDET6,"<span class=\"color\">".$srvdet['currentusers']."</span>","<span class=\"color\">".$srvdet['maxusers']."</span>","<span class=\"color\">".$srvdet['maxusertime']."</span>"),
		sprintf("<br />"._DA_SRVDET7,"<span class=\"color\">".$srvdet['opers']."</span>","<span class=\"color\">".$srvdet['maxopers']."</span>","<span class=\"color\">".$srvdet['maxopertime']."</span>"),
		sprintf("<br /><br />"._DA_SRVDET8,"<span class=\"color\">".$srvdet['ircopskills']."</span>","<span class=\"color\">".$srvdet['serverkills']."</span>");
		echo "</p><div class=\"title\"><a name=\"motd\"></a>"._PT_MOTD."</div>";
		echo "<pre class=\"left\"><span>";
		echo denora_colorconvert($srvdet['motd']);
	}
	echo "</span></pre>";
}
else {
	phpdenora_error(1,$page);
}
?>

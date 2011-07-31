<?php
// $Id: phpdenora.php 383 2009-09-17 07:57:04Z hal9000 $

/* ---------------------------------------------.
	This file is an example on how the remote   |
	data fetch feature of phpDenora can be used |
   ---------------------------------------------´
*/

die('This is an example file and should be used for testing only.<br />If you want to test, edit me and comment the \'die\' line');

# Set this to the URL of your phpDenora installation, trailing slash included
$phpdenora_url = "http://localhost/stats/";
# Set this to the wished time format
$denora_format_time = "%A %d %B %Y, %I:%M %p"; # 12 hour format
#$denora_format_time = "%A %d %B %Y, %R"; # 24 hour format

ini_set('display_errors','off');

/* This useful function parses the data and returns an array */
function remote_data($func,$param)
{
	global $phpdenora_url;
	flush();
	$data = fopen($phpdenora_url."remote.php?func=".$func."&param=".$param, "rt");
	if (!$data) {
		die("Unable to connect to $phpdenora_url");
	}
	$i = 0; $array = array();
	while ($line = fgets($data)) {
		$line = str_replace("\n", "", $line);
		$vals = explode("\t", $line);
		if (count($vals) > 1) {
			$j = 0;
			foreach ($vals as $val) {
				$array[$i][$j++] = $val;
			}
		}
		else {
			$array[$i] = $line;
		}
		$i++;
	}
	return $array;
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>phpDenora Remote Example</title>
<style type="text/css">
<!--
body,td,th {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 10px;
	color: #333333;
}
body {
	background-color: #F5F3EB;
	margin: 0px;
}
div.section {
	width: 600px;
	margin-left: auto;
	margin-right: auto;
	margin-bottom: 5px;
	padding: 3px;
	background-color: #FFFFFF;
	border: 1px solid #CCCCCC;
}
-->
</style></head>

<body>
<div style="border-bottom: 1px solid #CCCCCC; background-color: #66CCFF; margin: 0px 0px 5px 0px; padding: 3px;"><h1 style="text-align: center;">phpDenora Remote Example</h1></div>
<div class="section"><h2>Welcome</h2>
<p>This file can serve you as an example of how to access and process the remote information phpDenora makes available, so you can easilly integrate that into your web site. </p>
<p>You can find more information in docs/REMOTE </p>
</div>
<?php
/* ------------------.
	Some Examples... |
   ------------------´
*/

echo "<div class=\"section\"><h2>Network Stats</h2>";

/* Get the Top 5 users of the day */
echo "<h3>Top 5 Users Today</h3>";
if ($data = remote_data("topusers",5)) {
	echo "<p>";
	$i = 1;
	foreach ($data as $user) {
		echo $i++ . ". " . $user . "<br />";
	}
	echo "</p>";
} else { echo "<p>No data</p>"; }

/* Get the Top 5 channels of the day */
echo "<h3>Top 5 Channels Today</h3>";
if ($data = remote_data("topchans",5)) {
	echo "<p>";
	$i = 1;
	foreach ($data as $chan) {
		echo $i++ . ". " . $chan . "<br />";
	}
	echo "</p>";
} else { echo "<p>No data</p>"; }

/* Get the Max Stats information */
echo "<h3>Current Stats</h3>";
if ($data = remote_data("currentstats",NULL)) {
	echo "<p>Current users: <strong>" . $data[0][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[0][1]) . "</strong><br />";
	echo "Current channels: <strong>" . $data[1][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[1][1]) . "</strong><br />";
	echo "Current opers: <strong>" . $data[2][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[2][1]) . "</strong><br />";
	echo "Current servers: <strong>" . $data[3][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[3][1]) . "</strong><br />";
	echo "Daily users peak: <strong>" . $data[4][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[4][1]) . "</strong></p>";
} else { echo "<p>No data</p>"; }

/* Get the Max Stats information */
echo "<h3>Max Stats</h3>";
if ($data = remote_data("maxstats",NULL)) {
	echo "<p>Max users: <strong>" . $data[0][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[0][1]) . "</strong><br />";
	echo "Max channels: <strong>" . $data[1][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[1][1]) . "</strong><br />";
	echo "Max opers: <strong>" . $data[2][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[2][1]) . "</strong><br />";
	echo "Max servers: <strong>" . $data[3][0] . "</strong> on <strong>" . strftime($denora_format_time, $data[3][1]) . "</strong></p>";
} else { echo "<p>No data</p>"; }

echo "</div><div class=\"section\"><h2>Server Stats</h2>";
echo "<h3>Servers list</h3>";
if ($data = remote_data("serverlist",NULL)) {
	echo "<ul>";
	foreach ($data as $server) {
		echo "<li>$server</li>";
	}
	echo "</ul></div>";
} else { echo "<p>No data</p>"; }

/* Get the list of channels on the network */
echo "<div class=\"section\"><h2>Channel List</h2>";
if ($data = remote_data("chanlist",0)) {
	echo "<table><tr><th align=\"left\">Channel</th><th align=\"left\">Users</th><th align=\"left\">Topic</th></tr>";
	foreach ($data as $channel) {
		echo "<tr><td valign=\"top\"><strong>".htmlspecialchars($channel[0])."</strong></td><td valign=\"top\"><strong>$channel[1]</strong></td><td>$channel[2]</td></tr>";
	}
	echo "</table></div>";
} else { echo "<p>No data</p>"; }

/* Get the number of users in a channel */
$chan = "#denora";
echo "<div class=\"section\"><h2>Channel Stats for $chan</h2>";
echo "<h3>Channel users</h3>";
if ($data = remote_data("numinchan",urlencode(html_entity_decode($chan)))) {
	if (count($data) >= 1 && $data[0])
		echo "<p>There are currently <strong>".$data[0]."</strong> users on <strong>$chan</strong>.</p>";
	else
		echo "<p>The channel <strong>$chan</strong> seems to be empty</p>";
} else { echo "<p>No data</p>"; }

/* Get the user list */
if ($data = remote_data("userlist",urlencode(html_entity_decode($chan)))) {
	echo "<ul>";
	foreach ($data as $user) {
		echo "<li>$user</li>";
	}
	echo "</ul>";
} else { echo "<p>No data</p>"; }

/* Get the user peak of a channel */
if ($data = remote_data("chanpeak",urlencode(html_entity_decode($chan)))) {
	if (count($data) >= 2 && $data[0])
		echo "<p>User peak: <strong>".$data[0]."</strong> on <strong>".strftime($denora_format_time, $data[1])."</strong>.</p>";
} else { echo "<p>No data</p>"; }

/* Get the channel topic */
echo "<h3>Channel topic</h3>";
if ($data = remote_data("chantopic",urlencode(html_entity_decode($chan)))) {
	if (count($data) >= 3 && $data[0]) {
		echo "<p>Topic: " . $data[0] . "</p>";
		echo "<p>Set by: <strong>" . $data[1] . "</strong> on <strong>" . strftime($denora_format_time, $data[2]) . "</strong></p>";
	} else {
		echo "<p><em>No topic set</em></p>";
	}
} else { echo "<p>No data</p>"; }

echo "</div><div class=\"section\"><h2>User Stats</h2>";

/* Get the opers list */
echo "<h3>Current Operators</h3>";
if ($data = remote_data("operlist",urlencode(html_entity_decode($chan)))) {
	echo "<ul>";
	foreach ($data as $user) {
		echo "<li>$user</li>";
	}
	echo "</ul>";
} else { echo "<p>No data</p>"; }

echo "</div>";

?>

</body>
</html>

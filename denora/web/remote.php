<?php
// $Id: remote.php 415 2011-02-03 23:45:46Z hal9000 $
// Read the documentation in docs/REMOTE for how to use the remote functionalities

define( '_VALID_PARENT', 1 );

ini_set('display_errors','on');
error_reporting(E_ALL);

require ("libs/phpdenora/init.php");	# Load the initialization functions
switch ($pd_enremote) {
	case true:
		$link = sql_db_connect();
		$func = isset($_GET['func']) ? htmlspecialchars($_GET['func']) : exit;
		$param = isset($_GET['param']) ? htmlspecialchars($_GET['param']) : NULL;
		switch ($func) {
			/* Network Stats */
			case 'currentstats':
				if ($currentstats = phpdenora_current()) {
					for ($i = 0; $i < 5; $i++) {
						echo $currentstats['value'][$i] . "\t" . $currentstats['time'][$i] . "\n";
					}
				}
				break;
			case 'maxstats':
				if ($maxstats = phpdenora_maxstats()) {
					for ($i = 0; $i < 4; $i++) {
						echo $maxstats['value'][$i] . "\t" . $maxstats['time'][$i] . "\n";
					}
				}
				break;
			case 'topusers':
				if ($topusers = phpdenora_topusers($param)) {
					foreach($topusers as $topuser) {
						echo $topuser['user'] . "\n";
					}
				}
				break;
			case 'topchans':
				if ($topchans = phpdenora_topchans($param)) {
					foreach($topchans as $topchan) {
						echo htmlspecialchars($topchan['chan']) . "\n";
					}
				}
				break;
				/* Server Stats */
			case 'serverlist':
				if ($serverlist = denora_server_array(0)) {
					foreach ($serverlist as $server) {
						echo $server['server'] . "\n";
					}
				}
				break;
				/* Channel Stats */
			case 'chanlist':
				if ($chanlist = phpdenora_chanlist($param)) {
					foreach ($chanlist as $chan) {
						echo htmlspecialchars($chan['chan']) . "\t";
						echo $chan['users'] . "\t";
						echo $chan['topic'] . "\n";
					}
				}
				break;
			case 'userlist':
				if ($userlist = denora_who_in_channel($param)) {
					$users = array();
					foreach($userlist as $user) {
						$output = $user['nick'];
						if ($user['modes']) {
							$output .= ' '.$user['modes'];
						}
						echo "{$output}\n";
					}
				}
				break;
			case 'numinchan':
				echo denora_num_in_chan($param);
				break;
			case 'chanpeak':
				if ($chanpeak = denora_peak_chan($param)) {
					echo $chanpeak['maxusers'] . "\n";
					echo $chanpeak['maxusertime'] . "\n";
				}
				break;
			case 'chantopic':
				if ($chantopic = denora_get_chan_topic($param)) {
					echo denora_colorconvert($chantopic['topic']) . "\n";
					echo $chantopic['topicauthor'] . "\n";
					echo strtotime($chantopic['topictime']) . "\n";
				}
				break;
				/* User Stats */
			case 'operlist':
				if ($operlist = denora_get_opers()) {
					foreach ($operlist as $oper) {
						echo $oper['nick'] . "\n";
					}
				}
				break;
			case 'seenuser':
				if ($seenuser = phpdenora_seenuser($param)) {
					foreach ($seenuser[1] as $user) {
						echo $user['nick'] . "\t";
						echo $user['realname'] . "\t";
						echo $user['username'] . "\t";
						echo $user['hostname'] . "\t";
						echo $user['online'] . "\t";
						echo $user['away'] . "\t";
						echo $user['connecttime'] . "\t";
						echo $user['lastquit'] . "\t";
						echo $user['uline'] . "\n";
					}
				}
				break;
			default: exit;
		}
		sql_db_close($link);
		break;
	default: exit;
}

?>
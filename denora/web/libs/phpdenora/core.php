<?php
// $Id: core.php 415 2011-02-03 23:45:46Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

/* ------------------------.
    Network Core functions |
   ------------------------´
*/

/* Returns the current network stats values */
function phpdenora_current()
{
	global $denora_current_db; $vals = array(); $data = array();
	$q = sql_query("SELECT * FROM ".$denora_current_db.";");
	while ($r = sql_fetch_array($q)) {
		$vals[$r[0]] = array($r[1],$r[2]);
	}
	$data['value'] = array($vals['users'][0], $vals['chans'][0], $vals['opers'][0], $vals['servers'][0], $vals['daily_users'][0]);
	$data['time'] = array($vals['users'][1], $vals['chans'][1], $vals['opers'][1], $vals['servers'][1], $vals['daily_users'][1]);
	return $data;
}

/* Returns the max network stats values */
function phpdenora_maxstats()
{
	global $denora_maxvalues; $vals = array(); $data = array();
	$q = sql_query("SELECT * FROM ".$denora_maxvalues.";");
	while ($r = sql_fetch_array($q)) {
		$vals[$r[0]] = array($r[1],$r[2]);
	}
	$data['value'] = array($vals['users'][0], $vals['channels'][0], $vals['opers'][0], $vals['servers'][0]);
	$data['time'] = array(strtotime($vals['users'][1]), strtotime($vals['channels'][1]), strtotime($vals['opers'][1]), strtotime($vals['servers'][1]));
	return $data;
}

/* Prints a list of the most active channels */
function phpdenora_topchans($pd_fptop)
{
	global $ircd, $denora_cstats_db, $denora_chan_db, $denora_noshow_chans;
	$a1 = ($ircd['secretchan'] == 1) ? "AND ".$denora_chan_db.".".$ircd['secretchan_mode']."='N' " : NULL;
	$a1 .= ($ircd['privatechan'] == 1) ? "AND ".$denora_chan_db.".".$ircd['privatechan_mode']."='N' " : NULL;
	$b1 = NULL;
	$no = explode(",",$denora_noshow_chans);
	for ($i = 0; $i < count($no); $i++) {
		$b1 .= " AND ".$denora_cstats_db.".chan NOT LIKE '".sql_escape_string(strtolower($no[$i]))."'";
	}
	$q = sql_query("SELECT chan,line FROM ".$denora_cstats_db.", ".$denora_chan_db." WHERE BINARY LOWER(".$denora_cstats_db.".chan)=LOWER(".$denora_chan_db.".channel) AND ".$denora_cstats_db.".type=1 ".$a1." AND ".$denora_cstats_db.".line >= 1 ".$b1." ORDER BY ".$denora_cstats_db.".line DESC LIMIT ".$pd_fptop.";");
	$i = 0;	$array = array();
	while ($data = sql_fetch_array($q)) {
		$array[$i]['chan'] = $data['chan'];
		$array[$i]['lines'] = $data['line'];
		$i++;
	}
	return $array;
}

/* Prints a list of the most active users */
function phpdenora_topusers($pd_fptop)
{
	global $denora_ustats_db;
	$q = sql_query("SELECT uname,line FROM ".$denora_ustats_db." WHERE type=1 AND chan=\"global\" AND line >= 1 ORDER BY ".$denora_ustats_db.".line DESC LIMIT ".$pd_fptop.";");
	$i = 0;	$array = array();
	while ($data = sql_fetch_array($q)) {
		$array[$i]['user'] = $data['uname'];
		$array[$i]['lines'] = $data['line'];
		$i++;
	}
	return $array;
}

/* ------------------------.
    Server Core functions  |
   ------------------------´
*/

/* Returns the server list as an array */
function denora_server_array($show)
{
	global $denora_noshow_servers, $denora_noshow_ulined, $denora_server_db;
	$noshow = array(); $n = array(); $c = 0;
	
	$no = explode(",",$denora_noshow_servers);
	for ($i = 0; $i <= count($no) - 1; $i++)
	{
		$noshow[$i] = strtolower($no[$i]);
	}
	$q2 = ($denora_noshow_ulined == true) ? "WHERE uline=0" : "";
	
	$query = sql_query("SELECT * FROM $denora_server_db $q2");
	while ($data = sql_fetch_array($query))
	{
		if ($show || !in_array(strtolower($data['server']),$noshow))
		{
			$n[$c++] = $data;
		}
	}
	return $n;
}

/* Checks wether the given server should be hidden from output */
function denora_ishidden($server)
{
	global $denora_noshow_servers, $denora_noshow_ulined, $denora_server_db;
	$noshow = array();
	
	$no = explode(",",$denora_noshow_servers);
	for ($i = 0; $i < count($no); $i++)
	{
		$noshow[$i] = strtolower($no[$i]);
	}
	if ($denora_noshow_ulined == true) {
		$query = sql_query("SELECT server FROM $denora_server_db WHERE uline='1'");
		while ($data = sql_fetch_array($query)) {
			$noshow[$i++] = $data[0];
		}
	}
	$n = (in_array(strtolower($server),$noshow) ? TRUE : FALSE);
	return $n;
}

/* Returns details of a given server */
function phpdenora_srvdetails($srv)
{
	global $denora_server_db, $denora_format_time;
	$array = array();
	
	$query = sql_query("SELECT * FROM $denora_server_db WHERE server='".sql_escape_string($srv)."';");
	$data = sql_fetch_array($query);
	
	$array['description'] = $data['comment'];
	$array['version'] = $data['version'];
	$array['connecttime'] = ($data['connecttime'] == "0000-00-00 00:00:00" || $data['connecttime'] == NULL) ? -1 : strtotime($data['connecttime']);
	$array['online'] = $data['online'];
	$array['lastsplit'] = ($data['lastsplit'] == "0000-00-00 00:00:00" || $data['lastsplit'] == NULL) ? -1 : strtotime($data['lastsplit']);
	$array['uptime'] = $data['uptime'];
	$array['ping'] = $data['ping'];
	$array['highestping'] = $data['highestping'];
	$array['maxpingtime'] = $data['maxpingtime'];
	$array['currentusers'] = $data['currentusers'];
	$array['maxusers'] = $data['maxusers'];
	$array['maxusertime'] = $data['maxusertime'];
	$array['opers'] = $data['opers'];
	$array['maxopers'] = $data['maxopers'];
	$array['maxopertime'] = $data['maxopertime'];
	$array['ircopskills'] = $data['ircopskills'];
	$array['serverkills'] = $data['serverkills'];
	$array['motd'] = $data['motd'];
	
	return $array;
}

/* ------------------------.
    Channel Core functions |
   ------------------------´
*/

/* Returns a list of the current channels on the network */
function phpdenora_chanlist($minusers)
{
	$numargs = func_num_args();
	$arg_list = func_get_args();
	global $denora_chan_db, $ircd, $denora_noshow_chans;
	$array = array(); $no = array(); $c1 = NULL;
	
	$limit = ($numargs > 1) ? $arg_list[1] : 0;
	$b1 = ($limit > 0) ? "LIMIT $limit" : NULL;
	$a1 = ($ircd['secretchan'] == 1) ? "AND ".$ircd['secretchan_mode']."='N' " : NULL;
	$a1 .= ($ircd['privatechan'] == 1) ? "AND ".$ircd['privatechan_mode']."='N' " : NULL;
	$no = explode(",",$denora_noshow_chans);
	for ($i = 0; $i < count($no); $i++) { $c1 .= " AND LOWER(channel) NOT LIKE '".sql_escape_string(strtolower($no[$i]))."'"; }
	
	$query = sql_query("SELECT * FROM $denora_chan_db WHERE currentusers >= $minusers $a1 $c1 ORDER BY `$denora_chan_db`.`currentusers` DESC $b1;");
	$i = 0;
	while ($data = sql_fetch_array($query)) {
		$array[$i]['chan'] = $data['channel'];
		$array[$i]['users'] = $data['currentusers'];
		$array[$i]['topic'] = denora_colorconvert($data['topic']);
		$i++;
	}
	
	return $array;
}

/* Checks if given channel can be displayed
 * 0 = not existing, 1 = denied, 2 = ok */
function check_channel($chan)
{
	global $denora_chan_db, $denora_noshow_chans, $denora_block_spchans, $ircd;
	
	$no = explode(",",$denora_noshow_chans);
	for ($i = 0; $i < count($no); $i++) {
		$noshow[$i] = strtolower($no[$i]);
	}
	if (in_array(strtolower($chan),$noshow))
		return 1;
	
	$query = sql_query("SELECT * FROM $denora_chan_db WHERE BINARY LOWER(channel)=LOWER('".sql_escape_string($chan)."');");
	$data = sql_fetch_array($query);
	
	if (!$data) {
		return 0;
	}
	else {
		if ($denora_block_spchans) {
			if ( $ircd['secretchan'] && @$data[$ircd['secretchan_mode']] ) return 1;
			if ( $ircd['privatechan'] && @$data[$ircd['privatechan_mode']] ) return 1;
		}
		if (@$data['mode_li'] == "Y" || @$data['mode_lk'] == "Y" || @$data['mode_uo'] == "Y") return 1;
		else return 2;
	}
}

/* Returns a list of users currently in the given channel */
function denora_who_in_channel($chan)
{
	if (check_channel($chan) < 2)
		return 0;
	
	global $denora_chan_db, $denora_ison_db, $denora_user_db, $denora_server_db, $denora_server_type, $ircd;
	$array = array(); $i = 0;
	$q1 = ($ircd['helper']) ? "$denora_user_db.".$ircd['helper_mode']." AS helper," : NULL;
	$query = sql_query("SELECT $q1$denora_user_db.*,$denora_ison_db.*,$denora_server_db.uline FROM $denora_ison_db,$denora_chan_db,$denora_user_db,$denora_server_db WHERE LOWER($denora_chan_db.channel)=LOWER('".sql_escape_string($chan)."') AND $denora_ison_db.chanid=$denora_chan_db.chanid AND $denora_ison_db.nickid=$denora_user_db.nickid AND $denora_user_db.server = $denora_server_db.server ORDER BY $denora_user_db.nick ASC");
	while ($data = sql_fetch_array($query))
	{
		if (isset($data['nick']))
		{
			$mode = NULL;
			if (@$data['mode_lq'] == 'Y') { $mode .= "q"; }
			if (@$data['mode_la'] == 'Y') { $mode .= "a"; }
			if ($data['mode_lo'] == 'Y') { $mode .= "o"; }
			if (@$data['mode_lh'] == 'Y') { $mode .= "h"; }
			if ($data['mode_lv'] == 'Y') { $mode .= "v"; }
			$array[$i]['nick'] = $data['nick'];
			$array[$i]['modes'] = ($mode ? "+".$mode : "");
			$array[$i]['host'] = ((!empty($data['hiddenhostname']) && $data['hiddenhostname'] != "(null)") ? $data['hiddenhostname'] : $data['hostname']);
			$array[$i]['username'] = $data['username'];
			$array[$i]['countrycode'] = $data['countrycode'];
			$array[$i]['country'] = $data['country'];
			$array[$i]['bot'] = $ircd['bot'] ? $data[$ircd['bot_mode']] : 'N';
			$array[$i]['away'] = $data['away'];
			$array[$i]['online'] = $data['online'];
			$array[$i]['uline'] = $data['uline'];
			$array[$i]['helper'] = $ircd['helper'] ? $data['helper'] : 'N';
			$i++;
		}
	}
	
	return $array;
}

/* Returns current number user in a channel */
function denora_num_in_chan($chan)
{
	global $denora_chan_db;
	
	if (check_channel($chan) < 2)
		return 0;
	
	if ($denora_chan_db)
	{
		$query = sql_query("SELECT currentusers FROM $denora_chan_db WHERE BINARY LOWER(channel)=LOWER('".sql_escape_string($chan)."')");
		$data = sql_fetch_array($query);
        if($data['currentusers'])
		{
			return $data['currentusers'];
		}
        else
		{
            return 0;
		}
	}
	else
	{
		return 0;
	}
}

/* Returns channel user peak */
function denora_peak_chan($chan)
{
	global $denora_chan_db;
	
	if (check_channel($chan) < 2)
		return 0;
	
	if ($denora_chan_db)
	{
		$query = sql_query("SELECT maxusers,maxusertime FROM $denora_chan_db WHERE BINARY LOWER(channel)=LOWER('".sql_escape_string($chan)."')");
		$data = sql_fetch_array($query);
		return $data;
	}
	else
	{
		return 0;
	}
}

/* Returns channel modes */
function denora_chan_modes($chan)
{
	global $denora_chan_db;
	$i = 0;	$array = array(); $cmodes = ""; $j=97;
	
	$query = sql_query("SELECT * FROM $denora_chan_db WHERE BINARY LOWER(channel)=LOWER('".sql_escape_string($chan)."');");
	while ($data = sql_fetch_array($query))
	{
		$array[$i] = $data;
		$i++;
	}
	for ($i = 0; $i <= count($array) - 1; $i++) {
		while ($j <= 122)
		{
			if (@$array[$i]['mode_l'.chr($j)] == "Y")
			{
				$cmodes .= chr($j);
			}
			if (@$array[$i]['mode_u'.chr($j)] == "Y")
			{
				$cmodes .= chr($j-32);
			}
			$j++;
		}
		if (@$array[$i]['mode_lf_data'] != NULL)
		{
			$cmodes .= " ".$array[$i]['mode_lf_data'];
		}
		if (@$array[$i]['mode_lj_data'] != NULL)
		{
			$cmodes .= " ".$array[$i]['mode_lj_data'];
		}
		if (@$array[$i]['mode_ll_data'] > 0)
		{
			$cmodes .= " ".$array[$i]['mode_ll_data'];
		}
		if (@$array[$i]['mode_uf_data'] != NULL)
		{
			$cmodes .= " ".$array[$i]['mode_uf_data'];
		}
		if (@$array[$i]['mode_uj_data'] > 0)
		{
			$cmodes .= " ".$array[$i]['mode_uj_data'];
		}
		if (@$array[$i]['mode_ul_data'] != NULL)
		{
			$cmodes .= " ".$array[$i]['mode_ul_data'];
		}
	}
	if ($cmodes != "")
	{
		$cmodes = "+" . $cmodes;
	}
	return $cmodes;
}

/* Returns the number of channel bans */
function denora_num_chan_bans($chan)
{
	global $denora_chanbans_db;
	return sql_query_num_rows("SELECT bans FROM $denora_chanbans_db WHERE chan='".sql_escape_string($chan)."'");
}

/* Returns a list of the channel bans */
function denora_list_chan_bans($chan)
{
	global $denora_chanbans_db;
	$array = array();
	
	$query = sql_query("SELECT bans FROM $denora_chanbans_db WHERE chan='".sql_escape_string($chan)."'");
	for ($i = 0; $data = sql_fetch_array($query); $i++)
	{
		$array[$i] =$data[0];
	}
	return $array;
}

/* Returns the number of channel excepts */
function denora_num_chan_except($chan)
{
	global $denora_chanexcept_db, $ircd;
	return ($ircd['chanexception'] ? sql_query_num_rows("SELECT DISTINCT mask FROM $denora_chanexcept_db WHERE chan='".sql_escape_string($chan)."'") : 0); # DISTINCT is a workaround for denora bug 137
}

/* Returns a list of the channel excepts */
function denora_list_chan_except($chan)
{
	global $denora_chanexcept_db;
	$array = array();
	
	$query = sql_query("SELECT DISTINCT `mask` FROM $denora_chanexcept_db WHERE chan='".sql_escape_string($chan)."'");
	for ($i = 0; $data = sql_fetch_array($query); $i++)
	{
		$array[$i] =$data[0];
	}
	return $array;
}

/* Returns the number of channel invites */
function denora_num_chan_invites($chan)
{
	global $denora_chaninvite_db, $ircd;
	
	return ($ircd['chaninvites'] ? sql_query_num_rows("SELECT * FROM $denora_chaninvite_db WHERE chan='".sql_escape_string($chan)."'") : 0);
}

/* Returns a list of the channel invites */
function denora_list_chan_invites($chan)
{
	global $denora_chaninvite_db;
	$array = array();
	
	$query = sql_query("SELECT `mask` FROM $denora_chaninvite_db WHERE chan='".sql_escape_string($chan)."'");
	for ($i = 0; $data = sql_fetch_array($query); $i++)
	{
		$array[$i] = $data[0];
	}
	return $array;
}

/* Returns topic information */
function denora_get_chan_topic($chan)
{
	global $denora_chan_db;
	
	if (check_channel($chan) < 2)
		return 0;
	
	$query = sql_query("SELECT topic,topictime,topicauthor FROM $denora_chan_db WHERE BINARY LOWER(channel)=LOWER('".sql_escape_string($chan)."')");
	$data = sql_fetch_array($query);
	return ($data ? $data : NULL);
}

/* ------------------------.
    User Core functions    |
   ------------------------´
*/

/* Acts like a seen user function */
function phpdenora_seenuser($search)
{
	global $denora_user_db, $denora_server_db, $pd_rlimit, $ircd, $subpage; $array = array(); $i = 0;
	$search = str_replace("*", "%", $search);
	$split1 = explode("@",$search);
	$split2 = explode("!",$split1[0]);
	$seennick = ($split2[0] == NULL) ? "%" : $split2[0];
	$seenuname = (@$split2[1] == NULL) ? "%" : $split2[1];
	$seenhost = (@$split1[1] == NULL) ? "%" : $split1[1];
	$qlimit = ( $subpage - 1 ) * $pd_rlimit;
	
	$hostq = ($ircd['host_cloaking'] == true) ? "hiddenhostname" : "hostname";
	$qsum = sql_query_num_rows("SELECT $denora_user_db.* FROM $denora_user_db WHERE nick LIKE '".sql_escape_string($seennick)."%' AND username LIKE '".sql_escape_string($seenuname)."' AND $hostq LIKE '".sql_escape_string($seenhost)."'");
	$query = sql_query("SELECT $denora_user_db.*,$denora_server_db.uline FROM $denora_user_db,$denora_server_db WHERE $denora_user_db.nick LIKE '".sql_escape_string($seennick)."%' AND $denora_user_db.username LIKE '".sql_escape_string($seenuname)."' AND $denora_user_db.$hostq LIKE '".sql_escape_string($seenhost)."' AND $denora_user_db.server = $denora_server_db.server ORDER BY $denora_user_db.online,$denora_user_db.lastquit DESC, connecttime ASC LIMIT $qlimit , $pd_rlimit;");
	while ($data = sql_fetch_array($query)) {
		$array[$i]['nick'] = $data['nick'];
		$array[$i]['realname'] = $data['realname'];
		$array[$i]['username'] = $data['username'];
		$array[$i]['hostname'] = ((!empty($data['hiddenhostname']) && $data['hiddenhostname'] != "(null)") ? $data['hiddenhostname'] : $data['hostname']);
		$array[$i]['online'] = $data['online'];
		$array[$i]['away'] = $data['away'];
		$array[$i]['connecttime'] = $data['connecttime'] == NULL ? NULL : strtotime($data['connecttime']);
		$array[$i]['lastquit'] = $data['lastquit'] == NULL ? NULL : strtotime($data['lastquit']);
		$array[$i]['helper'] = ($ircd['helper']) ? $data[$ircd['helper_mode']] : 'N';
		$array[$i]['bot'] = ($ircd['bot']) ? $data[$ircd['bot_mode']] : 'N';
		$array[$i]['uline'] = @$data['uline'];
		$array[$i]['countrycode'] = $data['countrycode'];
		$array[$i]['country'] = $data['country'];
		$i++;
	}
	return array($qsum,$array);
}

/* Returns status for the given user */
function denora_get_user_status($nick)
{
	global $denora_user_db, $denora_aliases_db, $denora_server_db, $pd_statuslookup, $ircd;
	$array = array();
	
	if ($pd_statuslookup == true) {
		$query = sql_query("SELECT $denora_user_db.*,$denora_server_db.uline FROM $denora_aliases_db,$denora_user_db,$denora_server_db WHERE $denora_aliases_db.uname = '".sql_escape_string($nick)."' AND $denora_aliases_db.nick = $denora_user_db.nick AND $denora_user_db.server = $denora_server_db.server ORDER BY $denora_user_db.online,$denora_user_db.lastquit DESC, $denora_user_db.connecttime ASC");
		$data = sql_fetch_array($query);
		if ($data)
		{
			$array['online'] = ($data['online'] == 'Y' ? 1 : 0);
			$array['offline'] = ($data['online'] == 'N' ? 1 : 0);
			$array['oper'] = ($data['mode_lo'] == 'Y' ? 1 : 0);
			$array['away'] = ($data['away'] == 'Y' ? 1 : 0);
			$array['bot'] = ($ircd['bot'] && @$data[$ircd['bot_mode']] == 'Y') ? 1 : 0;
			$array['helper'] = ($ircd['helper'] && @$data[$ircd['helper_mode']] == 'Y') ? 1 : 0;
			$array['service'] = ($data['uline'] == 'Y' ? 1 : 0);
			$array['country'] = $data['country'];
			$array['countrycode'] = $data['countrycode'];
			return $array;
		}
		else
		{
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

/* Returns information about a given user */
function phpdenora_userinfo($user)
{
	global $denora_user_db, $denora_format_time;
	$str = NULL; $umodes = NULL; $array = array();

	$query = sql_query("SELECT * FROM $denora_user_db WHERE nick = '".sql_escape_string($user)."';");
	$data = sql_fetch_array($query);
	
	$array['nick'] = $data['nick'];
	$array['username'] = $data['username'];
	$array['realname'] = $data['realname'];
	$array['hostname'] = (!empty($data['hiddenhostname']) && $data['hiddenhostname'] != "(null)") ? $data['hiddenhostname'] : $data['hostname'];
	if (isset($data['swhois'])) { $array['swhois'] = $data['swhois']; }
	$array['connecttime'] = $data['connecttime'] ? strtotime($data['connecttime']) : NULL;
	$array['server'] = $data['server'];
	$array['away'] = $data['away'];
	$array['awaymsg'] = $data['awaymsg'];
	$array['ctcpversion'] = $data['ctcpversion'];
	$array['online'] = $data['online'];
	$array['lastquit'] = $data['lastquit'] ? strtotime($data['lastquit']) : NULL;
	if (isset($data['lastquitmsg'])) { $array['quitmsg'] = $data['lastquitmsg']; }
	$array['countrycode'] = $data['countrycode'];
	$array['country'] = $data['country'];
	
	return $array;
}

/* Get a list of operators currently online */
function denora_get_opers()
{
	global $denora_user_db, $denora_server_db, $denora_noshow_ulined, $ircd, $denora_server_type;
	
	$ho = ($ircd['supports_hidden_ops'] == 1) ? "AND $denora_user_db.".$ircd['hidden_op_mode']." = 'N'" : NULL;
	$hu = ($denora_noshow_ulined == true) ? "AND $denora_server_db.uline = '0'" : NULL;
	$hs = ($ircd['services_protection'] == 1) ? "AND $denora_user_db.".$ircd['services_protection_mode']." = 'N'" : NULL;
	if ($denora_server_type == "unreal32") {
		$query = sql_query("SELECT $denora_user_db.*,$denora_server_db.uline FROM $denora_user_db,$denora_server_db WHERE ($denora_user_db.mode_un = 'Y' OR $denora_user_db.mode_ua = 'Y' OR $denora_user_db.mode_la = 'Y' OR $denora_user_db.mode_uc = 'Y' OR $denora_user_db.mode_lo = 'Y') AND $denora_user_db.online = 'Y' $ho $hs AND $denora_user_db.server = $denora_server_db.server $hu ORDER BY $denora_user_db.mode_un,$denora_user_db.mode_ua,$denora_user_db.mode_la,$denora_user_db.mode_uc,$denora_user_db.mode_lo,$denora_user_db.nick ASC");
	}
	else {
		$query = sql_query("SELECT $denora_user_db.*,$denora_server_db.uline FROM $denora_user_db,$denora_server_db WHERE $denora_user_db.mode_lo = 'Y' AND $denora_user_db.online = 'Y' $ho $hs AND $denora_user_db.server = $denora_server_db.server $hu ORDER BY $denora_user_db.nick ASC");
	}
	$i = 0; $array = array();
	while ($data = sql_fetch_array($query)) {
		$array[$i]['nick'] = $data['nick'];
		$array[$i]['connecttime'] = $data['connecttime'] == NULL ? NULL : strtotime($data['connecttime']);
		$array[$i]['away'] = $data['away'];
		if ($denora_server_type == "unreal32") {
			if ($data['mode_un'] == "Y") { $level = "Network Admin"; }
			elseif ($data['mode_ua'] == "Y") { $level = "Server Admin"; }
			elseif ($data['mode_la'] == "Y") { $level = "Services Admin"; }
			elseif ($data['mode_uc'] == "Y") { $level = "Co-Admin"; }
			elseif ($data['mode_lo'] == "Y") { $level = "Global Operator"; }
		}
		else { $level = "Operator"; }
		$array[$i]['level'] = $level;
		$array[$i]['bot'] = $ircd['bot'] ? $data[$ircd['bot_mode']] : 'N';
		$array[$i]['helper'] = $ircd['helper'] ? $data[$ircd['helper_mode']] : 'N';
		$array[$i]['uline'] = $data['uline'];
		$i++;
	}
	return $array;
}

/* Returns the uname (for nick tracking purposes) */
function denora_get_uname($nick)
{
	global $denora_aliases_db;
	$data = array();
	
	$query = sql_query("SELECT uname FROM $denora_aliases_db WHERE nick='".sql_escape_string($nick)."'");
	$uname = sql_fetch_array($query);
	
	return $uname[0];
}

/* Returns the nick alias (for nick tracking purposes) */
function denora_get_alias($user)
{
	global $denora_user_db, $denora_aliases_db;
	$data = array();
	
	$query = sql_query("SELECT $denora_aliases_db.nick FROM $denora_aliases_db,$denora_user_db WHERE $denora_aliases_db.uname='".sql_escape_string($user)."' AND $denora_aliases_db.nick=$denora_user_db.nick ORDER BY $denora_user_db.online,$denora_user_db.lastquit DESC, $denora_user_db.connecttime ASC");
	$alias = sql_fetch_array($query);

	return $alias[0];
}

?>

<?php
// $Id: mysql.php 392 2009-10-16 07:27:33Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );


function sql_db_connect() 
{
	global $np_db_host, $np_db_user, $np_db_pass, $np_db_name, $np_db_port, $charset;
	
	$host = $np_db_port ? "$np_db_host:$np_db_port" : $np_db_host;
	$link_id = mysql_connect($host, $np_db_user, $np_db_pass);
	if (!$link_id) {
	    die('ERROR: Failed to connect to the database : ' . mysql_error());
	}
	
	mysql_query("set names 'utf8'");
	
	$db_selected = mysql_select_db($np_db_name, $link_id);
	if (!$db_selected) {
	    die('ERROR: Failed to select the '.$np_db_name.' database : ' . mysql_error());
	}
	return $link_id;
}

function sql_query($qstr)
{
	global $page, $pd_debug, $pd_exec, $qcount;
	
	$resc = @func_get_arg(1);
	$r = ($resc) ? @mysql_query($qstr,$resc) : @mysql_query($qstr);
	$num = @mysql_errno();
	$msg = @mysql_error();
	if ($pd_debug >= 2 && $num != 0) {
		echo("<pre>MySQL error ".$num.": ".$msg."</pre>");
	}
	if ($pd_exec == true) {
		$qcount++;
	}
	if ($pd_debug == 3) {
		echo "<pre class=\"left\">debug: $qstr</pre>\n";
	}
	
	return $r;
}

function sql_fetch_array($resc)
{
	return @mysql_fetch_array($resc);
}

function sql_db_close($link) 
{
	$result = @func_get_arg(1);
	@mysql_close($link);
	if ($result) { @mysql_free_result($result); }
}

function sql_num_rows($result) 
{
	return @mysql_num_rows($result);
}

function sql_escape_string($string)
{
	return @mysql_real_escape_string(html_entity_decode($string));
}

function sql_errno()
{
	return @mysql_errno();
}

function sql_error()
{
	return @mysql_error();
}

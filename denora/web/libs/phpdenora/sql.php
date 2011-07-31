<?php
// $Id: sql.php 336 2008-02-05 17:16:22Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

// Detect MySQL Module
if (file_exists("libs/phpdenora/sql/mysql.php")) {
	require("libs/phpdenora/sql/mysql.php");
} else {
	require("sql/mysql.php");
}

function sql_query_num_rows($str)
{
	$query = sql_query($str);
	$num = sql_num_rows($query);
	return ($num > 0 ? $num : "0");
}

function sql_update_data($table,$field,$value,$wherefield,$where)
{
	$result = sql_query("UPDATE $table SET $field = '$value' WHERE $wherefield = '$where'");
	$n = sql_num_rows($result);
	return ($n ? TRUE : FALSE); 
}

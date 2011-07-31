<?php
// $Id: index.php 412 2011-02-01 10:17:43Z hal9000 $

/* -------------------------------------- *
 *    phpDenora 1.4 - Pimp Your Stats!    *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *
 *       http://www.denorastats.org/      *
 * (c) 2005-2010 hal9000@denorastats.org  *
 *     GPL License - see docs/COPYING     *
 * -------------------------------------- *
 */

define( '_VALID_PARENT', 1 );

/* Turns reporting on until config is loaded */
ini_set('display_errors','on');
error_reporting(E_ALL);

/* Check if required PHP extensions are present on the system */
if (extension_loaded('mysql') != 1) { die('<html><head><title>phpDenora :: Error</title></head><body><p><span style="color:red">ERROR</span>: PHP MySQL extension is missing!<br />This component is required to run phpDenora. Please contact your Administrator.</p></body></html>'); }
if (extension_loaded('gd') != 1) { die('<html><head><title>phpDenora :: Error</title></head><body><p><span style="color:red">ERROR</span>: PHP GD extension is missing!<br />This component is required to run phpDenora. Please contact your Administrator.</p></body></html>'); }

require ("libs/phpdenora/init.php");	# Load the initialization functions
$link = sql_db_connect();

if ($_SERVER['HTTP_HOST'] != 'localhost' && file_exists('setup.php')) {
	include('setup.php');				# Load the setup page
	exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title><?php echo $pd_netname; ?> :: phpDenora</title>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>" />
<meta name="ROBOTS" content="INDEX, NOFOLLOW" />
<meta name="Keywords" content="<?php echo $pd_netname; ?> phpDenora IRC Chat Statistics" />
<meta name="Description" content="IRC Statistics for <?php echo $pd_netname; ?> powered by phpDenora" />
<link href="themes/<?php echo $theme ?>/css/default.css" rel="stylesheet" type="text/css" />
</head>
<body>
<?php
	$tplfile = 'themes/' . $theme . '/main.php';
	if (file_exists($tplfile)) {
		include($tplfile);
	}
	else {
		echo "<div id=\"pd_main\">Theme Error: main.php missing in theme ".$theme."</div>";
	}
	if (sql_errno() != 0) {
		echo "<div id=\"pd_main\">MySQL Error " . sql_errno() . ": " . sql_error(). "</div>";
	}
	sql_db_close($link);
?>
</body>
</html>

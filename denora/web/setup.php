<?php
// $Id: setup.php 404 2010-07-05 12:10:36Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die("<html><head><meta http-equiv=\"Refresh\" content=\"0;URL=index.php\" /></head><body><a href=\"index.php\">Start Setup</a></body></html>");

/* Turns reporting on until config is loaded */
ini_set('display_errors','on');
error_reporting(E_ALL);

/* Step 1 Checks */
$denoraver = phpdenora_versioncheck($denora_server_name,'full');
$denoranum = phpdenora_versioncheck($denora_server_name,'num');

if ($denoraver == NULL) {
	if (sql_errno() != 0) {
		echo "<span style=\"color:red\">MySQL Error</span> " . sql_errno() . ": " . sql_error(). "<br />";
		die("Please ensure that the database is setup correctly, and that the MySQL login data you specified in phpdenora.cfg.php is correct.");
	} else {
		die("<span style=\"color:red\">ERROR:</span> The database seems to be empty.<br />
			Please do the following checks:<br />
			<ul><li>Ensure that your Denora server is setup correctly, connected to your irc network and running.<br />
			Yes, you NEED a Denora server running on your irc network in order to use phpDenora.<br />
			Get Denora from <a href=\"http://www.denorastats.org/\">www.denorastats.org</a></li>
			<li>Check your Denora log file for any mysql-related errors.</li>
			<li>In the phpDenora configuration file you specified <em>$denora_server_name</em> as Denora server name.<br />
			Please ensure that this is the EXACT same name you gave to your Denora server on your irc network.</li></ul>
			");
	}
}
if ($denoranum < '1.4' || $denoranum >= '1.5') {
	$check_str1 = "<span style=\"color:red\">Incompatible</span><br />You need Denora 1.4.x to use this version of phpDenora!";
} else {
	$check_str1 = "<span style=\"color:green\">Supported</span>";
}
$phpver = phpversion();
if (version_compare("5.2.0", phpversion(), "<") == 1) { $check_str2 = "<span style=\"color:green\">Supported</span>"; }
else { $check_str2 = "<span style=\"color:red\">Not Supported</span><br />You need at least version 5.2.0"; }

if (extension_loaded('gd')) {
	$check_str3 = "<span style=\"color:green\">Present</span>";
} else {
	$check_str3 = "<span style=\"color:red\">Missing</span><br />Please make sure that the php-gd extension is installed and enabled and the webserver reloaded";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>phpDenora :: Setup</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<style type="text/css">
<!--
body {
	background-color: #555967;
	margin: 0px;
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 10px;
	color: #666666;
}

div#frame {
	margin: 7px auto 5px auto;
	padding: 0px;
	text-align: left;
	width: 550px;
}

div#header {
	padding: 5px 2px 2px 2px;
	margin: 0px;
	text-align:left;
	height: 58px;
	color: #003366;
	border-bottom: 2px solid #FF9900;
	border-left: 1px solid #CCCCCC;
	border-top: 1px solid #CCCCCC;
	border-right: 1px solid #CCCCCC;
	background-color:#FFFFFF;
}

div#main {
	padding: 0px;
	margin: 0px;
	background-color: #FFFFFF;
	border-left: 1px solid #CCCCCC;
	border-right: 1px solid #CCCCCC;
	text-align:left;
}

div#footer {
	padding: 0px;
	margin: 0px 0px 15px 0px;
	border-top: 1px solid #CCCCCC;
	font-size: 7pt;
	color: #999999;
	text-align: right;
}

img {
	border: 0;
	margin: 0;
}

p,h1,h3 {
	margin: 0px 10px 10px 10px;
	padding: 0px 5px 0px 5px;
}

p{
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 8pt;
	font-style: normal;
	line-height: normal;
	font-weight: normal;
	font-variant: normal;
	text-transform: none;
	color: #666666;
	background-color: transparent;
	letter-spacing: normal;
	text-indent: 0pt;
	vertical-align: baseline;
	margin: 0px;
	text-align: justify;
}

h1{
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 12pt;
	font-weight: bold;
	font-style: normal;
	font-variant: small-caps;
	color: #333333;
	background-color: transparent;
	margin: 0px;
}

h3 {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 10pt;
	font-weight: bold;
	font-style: normal;
	font-variant: small-caps;
	color: #666666;
	background-color: transparent;
	margin: 0px;
	text-align: left;
}


a, a:link, a:visited {
	color: #FF9900;
	text-decoration: none;
}
a:hover {
	text-decoration: underline;
}

form {
	padding: 5px;
	margin: 0px auto 0px auto;
	text-align: left;
}
input,select,textarea{ font-size: 8pt; }

-->
</style>
</head>
<body>
<div id="frame">
  <div id="header">
    <table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td valign="middle"><a href="http://www.denorastats.org/"><img src="img/logo.png" alt="phpDenora" /></a> </td>
        <td align="right" valign="bottom"><h1>phpDenora Setup</h1></td>
      </tr>
    </table>
  </div>
  <div id="main">
    <p>&nbsp;</p>
    <p><strong>Welcome to phpDenora! </strong></p>
    <p>If you are reading this you already configured phpDenora and it successfully connected to the Denora database.<br />
      This is good. However, there are still a couple of steps to complete.</p>
    <p>&nbsp;</p>
    <h3>1. Compatibility Check</h3>
    <table border="0" cellspacing="2" cellpadding="2">
      <tr>
        <th align="right"><?php echo $denoraver; ?>:</th>
        <td align="left"><?php echo $check_str1; ?></td>
      </tr>
      <tr>
        <th align="right">PHP <?php echo $phpver; ?>:</th>
        <td align="left"><?php echo $check_str2; ?></td>
      </tr>
      <tr>
        <th align="right">PHP GD2 extension</th>
        <td align="left"><?php echo $check_str3; ?></td>
      </tr>
        
    </table>
    <p>&nbsp;</p>
    <h3>2. Network Registration <em>(optional) </em></h3>
    <p>Please tell us about your network, so we can list you on the phpDenora web site and inform you about new releases via the announce mailing list.<br />
    You only need to submit this form once. </p>
    <form id="registration" method="post" action="http://www.denorastats.org/register.php">
      <table border="0" cellspacing="2" cellpadding="2">
        <tr>
          <td align="right"><strong>Network Name:</strong></td>
          <td><?php echo $pd_netname;?></td>
        </tr>
        <tr>
          <td align="right"><strong>Network Homepage URL: </strong></td>
          <td><?php echo $pd_neturl;?></td>
        </tr>
        <tr>
          <td align="right"><strong>phpDenora URL: </strong></td>
          <td>http://<?php echo $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI']; ?></td>
        </tr>
        <tr>
          <td align="right"><strong>Email address:</strong></td>
          <td><input name="email" type="text" size="30" /></td>
        </tr>
        <tr>
          <td>&nbsp;</td>
          <td>
		  <input type="hidden" name="netname" value="<?php echo $pd_netname; ?>" />
		  <input type="hidden" name="netpage" value="<?php echo $pd_neturl; ?>" />
		  <input type="hidden" name="homepage" value="<?php echo "http://" . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI']; ?>" />
		  <input type="hidden" name="version" value="<?php echo VERSION_FULL; ?>" />
		  <input type="hidden" name="ircd" value="<?php echo $denora_server_type; ?>" />
            <input type="submit" name="Submit" value="Submit" /></td>
        </tr>
      </table>
    </form>
    <p>&nbsp;</p>
    <h3>3. Finalize the Installation</h3>
    <ul>
      <li><strong>Edit welcome.dist.php and rename it to welcome.php<br />
        </strong>You can customize the welcome page file to your needs and taste. You can welcome your users, describe your network, and put whatever information you want in there. 
        Please use XHTML 1.0 Strict compilant markup language to avoid breaking validation.</li>
      <li><strong>Remove setup.php</strong><br />
        You must REMOVE the setup.php file in order to start using phpDenora! </li>
    </ul>
    <h3>4. Important Notes</h3>
    <p>To submit any bugs please use the <a href="http://dev.denorastats.org/projects/phpdenora/">Bug Tracker</a>.</p>
    <p>&nbsp;</p>
  </div>
  <div id="footer">
    Powered by <a href="http://www.denorastats.org/">phpDenora</a> v<?php echo VERSION_FULL; ?>
  </div>
</div>
</body>
</html>

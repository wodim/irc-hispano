<?php
// $Id: init.php 416 2011-02-07 13:44:18Z hal9000 $

define('VERSION_MAJOR', 1 );
define('VERSION_MINOR', 4 );
define('VERSION_PATCH', 5 );
define('VERSION_EXTRA', '' );
define('VERSION_BUILD', 416 );

if (get_magic_quotes_gpc()) {
	function stripslashes_deep($value) {
		return is_array($value) ? array_map('stripslashes_deep', $value) : stripslashes($value);
	}
	$_POST = array_map('stripslashes_deep', $_POST);
	$_GET = array_map('stripslashes_deep', $_GET);
	$_COOKIE = array_map('stripslashes_deep', $_COOKIE);
	$_REQUEST = array_map('stripslashes_deep', $_REQUEST);
}

// Check Configuration
if (file_exists("phpdenora.cfg.php"))
{
	require ("phpdenora.cfg.php");	# Load the configuration
	$error = "";
	$qcount = 0;
	$gqcount = 0;
	if (!isset($pd_debug)) { $pd_debug = 0; }
	if (!isset($pd_exec)) { $pd_exec = false; }
	if ($pd_exec == true)
	{
		$start = doubleval(time() + microtime());	# Start execution timer
	}
	if ($pd_debug < 2)
	{
		ini_set('display_errors','off');
		error_reporting(E_ERROR);
	}
	if (!isset($pd_timezone)) { $error .= "\$pd_timezone, "; }
	if (!isset($pd_netname)) { $error .= "\$pd_netname, "; }
	if (!isset($pd_neturl)) { $error .= "\$pd_neturl, "; }
	if (!isset($pd_style)) { $pd_style = "futura"; }
	if (!isset($pd_lang)) { $pd_lang = "en"; }
	if (!isset($pd_statsort)) { $pd_statsort = "line"; }
	if (!isset($pd_stattype)) { $pd_stattype = "0"; }
	if (!isset($np_db_host)) { $np_db_host = "localhost"; }
	if (!isset($np_db_port)) { $np_db_port = "3306"; }
	if (!isset($np_db_user)) { $error .= "\$np_db_user, "; }
	if (!isset($np_db_pass)) { $error .= "\$np_db_pass, "; }
	if (!isset($np_db_name)) { $error .= "\$np_db_name, "; }
	if (!isset($denora_user_db)) { $denora_user_db = "user"; }
	if (!isset($denora_chan_db)) { $denora_chan_db = "chan"; }
	if (!isset($denora_chanbans_db)) { $denora_chanbans_db = "chanbans"; }
	if (!isset($denora_chanexcept_db)) { $denora_chanexcept_db = "chanexcept"; }
	if (!isset($denora_chaninvite_db)) { $denora_chaninvite_db = "chaninvites"; }
	if (!isset($denora_gline_db)) { $denora_gline_db = "glines"; }
	if (!isset($denora_sqline_db)) { $denora_sqline_db = "sqline"; }
	if (!isset($denora_maxvalues)) { $denora_maxvalues = "maxvalues"; }
	if (!isset($denora_server_db)) { $denora_server_db = "server"; }
	if (!isset($denora_ison_db)) { $denora_ison_db = "ison"; }
	if (!isset($denora_tld_db)) { $denora_tld_db = "tld"; }
	if (!isset($denora_cstats_db)) { $denora_cstats_db = "cstats"; }
	if (!isset($denora_ustats_db)) { $denora_ustats_db = "ustats"; }
	if (!isset($denora_current_db)) { $denora_current_db = "current"; }
	if (!isset($denora_serverstats)) { $denora_serverstats = "serverstats"; }
	if (!isset($denora_channelstats)) { $denora_channelstats = "channelstats"; }
	if (!isset($denora_userstats)) { $denora_userstats = "stats"; }
	if (!isset($denora_aliases_db)) { $denora_aliases_db = "aliases"; }
	if (!isset($denora_server_type)) { $error .= "\$denora_server_type, "; }
	if (!isset($denora_server_name)) { $error .= "\$denora_server_name, "; }
	if (!isset($denora_irc_server)) { $error .= "\$denora_irc_server, "; }
	if (!isset($denora_noshow_ulined)) { $denora_noshow_ulined = true; }
	if (!isset($denora_noshow_servers)) { $denora_noshow_servers = ""; }
	if (!isset($denora_noshow_chans)) { $denora_noshow_chans = ""; }
	if (!isset($denora_block_spchans)) { $denora_block_spchans = false; }
	if (!isset($pd_hidesecret)) { $pd_hidesecret = true; }
	if (!isset($pd_rlimit)) { $pd_rlimit = "20"; }
	if (!isset($pd_fptop)) { $pd_fptop = "5"; }
	if (!isset($pd_minchars)) { $pd_minchars = "3"; }
	if (!isset($pd_statuslookup)) { $pd_statuslookup = true; }
	if (!isset($pd_entldstats)) { $pd_entldstats = true; }
	if (!isset($pd_enverstats)) { $pd_enverstats = true; }
	if (!isset($pd_invitelists)) { $pd_invitelists = false; }
	if (!isset($pd_banlists)) { $pd_banlists = false; }
	if (!isset($pd_exceptlists)) { $pd_exceptlists = false; }
	if (!isset($pd_ennetgraphs)) { $pd_ennetgraphs = true; }
	if (!isset($pd_mirc)) { $pd_mirc = false; }
	if (!isset($pd_mirc_url)) { $pd_mirc_url = NULL; }
	if (!isset($pd_webchat)) { $pd_webchat = false; }
	if (!isset($pd_webchat_url)) { $pd_webchat_url = NULL; }
	if (!isset($pd_mibbit)) { $pd_mibbit = false; }
	if (!isset($pd_mibbit_key)) { $pd_mibbit_key = NULL; }
	if (!isset($pd_netsplit)) { $pd_netsplit = NULL; }
	if (!isset($pd_netsplit_graphs)) { $pd_netsplit_graphs = false; }
	if (!isset($pd_netsplit_years)) { $pd_netsplit_years = true; }
	if (!isset($pd_netsplit_history)) { $pd_netsplit_history = false; }
	if (!isset($pd_searchirc)) { $pd_searchirc = NULL; }
	if (!isset($pd_searchirc_ranking)) { $pd_searchirc_ranking = false; }
	if (!isset($pd_searchirc_graphs)) { $pd_searchirc_graphs = false; }
	if (!isset($pd_adsense)) { $pd_adsense = false; }
	if (!isset($pd_adsense_client)) { $pd_adsense_client = NULL; }
	if (!isset($pd_adsense_channel)) { $pd_adsense_channel = NULL; }	
	if (!isset($pd_graphcache) || !isset($pd_cachedir)) { $pd_graphcache = false; }
	if (!isset($pd_cache_graph)) { $pd_cache_graph = 60; }
	if (!isset($pd_cache_pie)) { $pd_cache_pie = 1; }
	if (!isset($pd_cache_bar)) { $pd_cache_bar = 1; }
	if (!isset($pd_gzip)) { $pd_gzip = false; }
	if (!isset($pd_enremote)) { $pd_enremote = false; }
	if (!isset($pd_valid)) { $pd_valid = false; }
	if ($error != "")
	{
		die("<html><head><title>phpDenora :: Error</title></head><body><p>ERROR: The following required configuration variables are missing from your configuration file: <strong>". $error ."</strong><br/>Please look at <strong>example.cfg.php</strong> and configure the required items in <strong>phpdenora.cfg.php</strong></p></body></html>");
	}
	if (!date_default_timezone_set($pd_timezone)) {
		die("<html><head><title>phpDenora :: Error</title></head><body><p>ERROR: Invalid timezone setting.<br/>Please check your configuration file</p></body></html>");
	}
}
else
{
	die("<html><head><title>phpDenora :: Error</title></head><body><p>ERROR: Configuration file missing! Please configure <strong>example.cfg.php</strong> and rename it to <strong>phpdenora.cfg.php</strong></p></body></html>");
}

if (VERSION_EXTRA == '-svn') {
	define('VERSION_FULL', VERSION_MAJOR . "." . VERSION_MINOR . "." . VERSION_PATCH . "." . VERSION_BUILD . "-" . $denora_server_type);
}
else {
	define('VERSION_FULL', VERSION_MAJOR . "." . VERSION_MINOR . "." . VERSION_PATCH . VERSION_EXTRA);
}

// Load the libraries
require("libs/phpdenora/sql.php");
require("libs/phpdenora/ircds/$denora_server_type.php");
require("libs/phpdenora/core.php");
require("libs/phpdenora/denora.php");
require("libs/phpdenora/utils.php");
require("libs/phpdenora/chanstats.php");
require("libs/phpdenora/userstats.php");
require("libs/phpdenora/menu.php");

// Get URL variables and set things accordignly
if (isset($_GET['chan'])) {
	$chan = stripslashes($_GET['chan']);
	$chan = htmlspecialchars($chan);
	if ($chan == "") {
		unset($chan);
	}
	else {
		if ($chan != "global" && $chan{0} != "#" && $chan{0} != "&" && $chan{0} != "'") {
			$chan = "#" . $chan;
		}
	}
}
if (isset($_GET['user'])) {
	$user = stripslashes($_GET['user']);
	$user = htmlspecialchars($user);
	if ($user == "") { unset($user); }
}
if (isset($_GET['nick'])) {
	$nick = stripslashes($_GET['nick']);
	$nick = htmlspecialchars($nick);
	if ($nick == "") { unset($nick); }
}

// Set the mode to load
if (isset($_GET['m']))
{
	$mode = htmlspecialchars($_GET['m']);
}
else
{
	if (isset($chan))
	{
		$mode = "c";
	}
	elseif (isset($user) || isset($nick))
	{
		$mode = "u";
	}
	else
	{
		$mode = "h";
	}
}

// Set the page to load
if (isset($_GET['p']))
{
	$page = htmlspecialchars($_GET['p']);
}
else
{
	if (isset($user) || isset($nick))
	{
		$page = "ustats";
	}
	elseif (isset($chan))
	{
		$page = "chan";
	}
	elseif ($mode == "c")
	{
		$page = "list";
	}
	elseif ($mode == "u")
	{
		$page = "top";
	}
	elseif ($mode == "n")
	{
		$page = "status";
	}
	elseif ($mode == "g")
	{
		if ($pd_ennetgraphs == true && extension_loaded('gd') == 1) {
			$page = "netstats";
		}
		elseif ($pd_netsplit_graphs == true) {
			$page = "netsplit";
		}
		elseif ($pd_searchirc_graphs == true) {
			$page = "searchirc";
		}
	}
}

if (!isset($chan)) { $chan = "global"; }

// Sub pages
$subpage = (isset($_GET['sub'])) ? htmlspecialchars($_GET['sub']) : 1;

// IRC Server
$srv = (isset($_GET['srv']) ? htmlspecialchars($_GET['srv']) : NULL);

// Sorting
if (isset($_GET['sort'])) { # From URL
	$sort = htmlspecialchars($_GET['sort']);
}
elseif (isset($_POST['sort'])) # From Settings Page
{
	$sort = htmlspecialchars($_POST['sort']);
	setcookie("sort", $sort, time()+60*60*24*30);
}
elseif (isset($_COOKIE["sort"])) # From Cookie
{
	$sort = $_COOKIE["sort"];
}
else # Default Value
{
	$sort = $pd_statsort;
}
if (!in_array($sort, array('channel','currentusers','chan','uname','letters','words','line','actions','smileys','kicks','modes','topics')))
{
	$sort = $pd_statsort;
}
$order = (@$_GET['order'] == "asc") ? "asc" : "desc";

// Stats Type
if (isset($_GET['type']))# From URL
{
	$type = $_GET['type'];
}
elseif (isset($_POST['type'])) # From Settings Page
{
	$type = $_POST['type'];
	setcookie("type", $type, time()+60*60*24*30);
}
elseif (isset($_COOKIE["type"])) # From Cookie
{
	$type = $_COOKIE["type"];
}
else # Default Value
{
	$type = $pd_stattype;
}
settype($type, 'integer');

$t = isset($_GET['t']) ? htmlspecialchars($_GET['t']) : "m";

// Search Query
if (isset($_POST['search'])) { $search = htmlspecialchars($_POST['search']); }
elseif (isset($_GET['search'])) { $search = htmlspecialchars($_GET['search']); }

// Set the correct language
if (isset($_GET['lang']))
{
	$lang = htmlspecialchars($_GET['lang']);
	setcookie("lang", $lang, time()+60*60*24*30);
}
elseif (isset($_POST['lang']))
{
	$lang = htmlspecialchars($_POST['lang']);
	setcookie("lang", $lang, time()+60*60*24*30);
}
elseif (isset($_COOKIE["lang"]))
{
	$lang = $_COOKIE["lang"];
}
else {
	// Automatic language detection
	$i = 0; $langs = array(); $pref = array();
	foreach (glob("lang/*") as $filename)
	{
		if (substr($filename, -4, 4) != ".php" && file_exists($filename."/index.php"))
		{
			$filename = explode("/", $filename);
			$langs[$i++] = $filename[1];
		}
	}
	$i = 0;
	foreach(preg_split('/,/', @$_SERVER["HTTP_ACCEPT_LANGUAGE"]) as $blang)
	{
		$pref[$i++] = substr($blang, 0, 2);
	}
	$tl = array_intersect($pref, $langs);
	$lang = isset($tl[0]) ? $tl[0] : $pd_lang;
}
if (!isset($lang)) { $lang = $pd_lang; }
$lang = basename($lang);
$langfile = "lang/".$lang."/lang.php";
if (file_exists($langfile))
{
	if (file_exists("lang/".$lang."/index.php")) {
		require("lang/".$lang."/index.php");
		if ($lang_ver < "1.3.0") {
			setcookie("lang", $pd_lang, time()+60*60*24*30);
			die("<span style=\"color:red;\">ERROR</span>: The installed \"$lang_name\" language file (lang/$lang) is not compatible with this version of phpDenora.<br />
				Please tell your webmaster to remove it or to read docs/TRANSLATIONS for instructions to upgrade it!<br /><a href=\"?m=h\">Reload</a>");
		}
	}
	include ($langfile);
}
else {
	$lang = "en";
	include ( 'lang/en/lang.php' );
}

// Set the language encoding
if (!isset($charset)) { $charset = "utf-8"; }
ini_set('default_charset',$charset);

// Set the date/time format
$denora_format_time = (!isset($denora_format_time) ? "%A %d %B %Y, %I:%M %p" : $denora_format_time );
$denora_format_short = (!isset($denora_format_short) ? "%D %T" : $denora_format_short );

// Set the correct theme
if (isset($_GET['theme']))
{
	$theme = htmlspecialchars($_GET['theme']);
	setcookie("theme", $theme, time()+60*60*24*30);
}
elseif (isset($_POST['theme']))
{
	$theme = htmlspecialchars($_POST['theme']);
	setcookie("theme", $theme, time()+60*60*24*30);
}
elseif (isset($_COOKIE["theme"]))
{
	$theme = $_COOKIE["theme"];
}
else
{
	$theme = $pd_style;
}
$theme = basename($theme);
$themefile = "themes/".$theme."/theme.php";
if (file_exists($themefile))
{
	if (file_exists("themes/".$theme."/index.php")) {
		require("themes/".$theme."/index.php");
		if ($theme_ver < "1.2.0") {
			setcookie("theme", $pd_style, time()+60*60*24*30);
			die("<span style=\"color:red;\">ERROR</span>: The installed \"$theme_name\" theme (themes/$theme) is not compatible with this version of phpDenora.<br />
				Please tell the webmaster to remove it, or to read docs/THEMES for instructions to upgrade it!<br /><a href=\"?m=h\">Reload</a>");
		}
	}
	require ($themefile);
}
else
{
	$theme = "futura";
	require ( 'themes/futura/theme.php' );
}

if ($pd_gzip == true) {
	$compressed = ob_start("ob_gzhandler");
}

?>
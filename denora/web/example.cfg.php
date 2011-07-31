<?php
// $Id: example.cfg.php 413 2011-02-03 20:49:30Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

/* ================================.
   REQUIRED CONFIGURATION SETTINGS |
   ================================´
   These settings are required for phpDenora to work
*/

/* PHP Configuration
   ----------------- */
# You MUST set a valid timezone! For example Europe/London
# Please refer to http://php.net/manual/en/timezones.php for a list of timezones
$pd_timezone = "";

/* IRC Network Information
   ----------------------- */

# The name of your Network
$pd_netname = "MyNetwork";

# The URL of the Homepage of your Network
$pd_neturl = "http://www.mynet.tld/";

/* MySQL Configuration
   ------------------- */

# Username of the MySQL Database
$np_db_user = "denora";

# Database Password
$np_db_pass = "";

# Database name
$np_db_name = "denora";

/* Server Configuration
   -------------------- */

# Available server types: asuka, bahamut, beware, charybdis, hybrid, hyperion,
# inspircd, ircu, nefarious, nefarious13, ngircd, plexus3, ratbox, scarynet,
# solidircd, ultimate3, unreal32
$denora_server_type = "unreal32";

# This is the name of the Denora server, as specified in the Denora server configuration file.
# Yes, you NEED a Denora server running on your irc network in order to use phpDenora.
# Get Denora from http://www.denorastats.org/
$denora_server_name = "stats.mynet.tld";

# This is the hostname used to connect to your irc network
$denora_irc_server = "irc.mynet.tld";

# Comment or delete the following line to get phpdenora working...
die("You should really read the configuration file carefully!");

/* ================================.
   OPTIONAL CONFIGURATION SETTINGS |
   ================================´
   These settings are not required and their defaults
   should be fine, but you can change them as you wish
*/

/* Appearance
   ---------- */

# Default theme
# Available Themes: alternative, classic, futura, modern
# Must match the name of the corresponding theme directory
$pd_style = "futura";

# Default language
# Available Languages: en (English), de (German), es (Spanish),
# fr (French), gr (Greek), it (Italian), nl (Dutch), pt (Portuguese),
# pt_br (Brazilian Portuguese), ro (Romanian), ru (Russian), tr (Turkish)
# This will NOT override the automatic browser language detection
$pd_lang = "en";

/* Behavior
   -------- */

# Set the default ordering column for user/channel stat tables.
# Available options: letters, words, line, actions, smileys, kicks, modes, topics
$pd_statsort = "line";

# Set the default stats type for user/channel stat tables.
# Available options: 0: Total, 1: Today, 2: This Week, 3: This Month
$pd_stattype = "3";

/* Server Configuration
   -------------------- */

# Hide ulined servers
$denora_noshow_ulined = true;

# Servers you don't want phpDenora to show
# Separate with commas without spaces, example: "hub.mynet.tld,hub2.mynet.tld"
$denora_noshow_servers = "";

# Channels you don't want phpDenora to show
# Separate with commas, without spaces
$denora_noshow_chans = "#opers,#services";

# Block channel info for secret/private channels
$denora_block_spchans = false;

/* General Configuration
   --------------------- */

# Sets the limit for table listed outputs
$pd_rlimit = 20;

# Sets the limit for TOP channels/users outputs on the front page
$pd_fptop = 10;

# Minimum amount of characters needed (excluding wildcards) to make a search query
$pd_minchars = 3;

# Enables online status and country lookups in user listings. This will require an additional
# query for each user, set this to false if you want to keep sql load low
$pd_statuslookup = true;

# Enables country statistics.
$pd_entldstats = true;

# Enables client version statistics. Set to false if your Denora does not version your clients.
$pd_enverstats = true;

# Enables network statistics graphs.
$pd_ennetgraphs = true;

# Enable the mirc icon in the channel list
$pd_mirc = false;

# The URL for the mirc icon, including trailing slash
$pd_mirc_url = "irc://irc.mynet.tld:6667/";

# Enable the web chat icon in the channel list
$pd_webchat = false;

# The URL for the web chat icon
$pd_webchat_url = "http://webchat.mynet.tld/?chan=";

# Enable mibbit icon in the channel list
# Please note you need to sign up at mibbit.com to register your network
$pd_mibbit = false;

# Mibbit settings key. Leave empty if you have none
$pd_mibbit_key = "";

# Enable the remote api interface, which enables to easilly embed data in web sites
$pd_enremote = false;

# The URL parameter for the Netsplit.de features, usually your network name.
# For more information about being ranked on Netsplit.de visit http://irc.netsplit.de/
$pd_netsplit = "";

# Enable Netsplit.de Graphs integration
$pd_netsplit_graphs = false;

# Enable Netsplit.de "Complete History" graphs
# If your network is too young this will not work
$pd_netsplit_history = false;

# Set your network ID of SearchIRC features.
# For more information about being ranked on SearchIRC visit http://searchirc.com/
# To find out your ID go to your network information page (usually http://searchirc.com/network/YourNetwork)
# then right click on the Users graph on the right to get its path and get the number from the 'n=' parameter.
$pd_searchirc = "";

# Enable SearchIRC Ranking icon on the front page
$pd_searchirc_ranking = false;

# Enable SearchIRC Graphs integration
$pd_searchirc_graphs = false;

/* Advertisement Options
   --------------------- */

# Enable Google AdSense advertisements
$pd_adsense = false;

# Set your Google AdSense Client ID
# If you would like to support phpDenora you can use "pub-2514457845805307" ;)
$pd_adsense_client = "";

# Set the Ad Channel (optional)
$pd_adsense_channel = "";
		
/* Performance Options
   ------------------- */

# Enable the caching of graph images. This speeds up things a bit.
$pd_graphcache = false;

# Specify the directory used for caching.
# The directory MUST be writeable by the web server AND contain a trailing /slash!
#$pd_cachedir = "/tmp/phpdenora-cache/";
$pd_cachedir = "../../../cache/";

# How long should network graph be cached. 60 (minutes) is the recommended value
$pd_cache_graph = 60;

# How long should pie graphs be cached. A low value such as 1 (minute) is the recommended value
$pd_cache_pie = 1;

# How long should bar graphs be cached. A low value such as 1 (minute) is the recommended value
$pd_cache_bar = 1;

# Enable page compression
$pd_gzip = false;

/* Debug Options
   ------------- */

# 0: off, 1: deprecated (same as 0), 2: php warnings, sql errors (used for development)
$pd_debug = 0;

# Set to true to show script execution time and number of sql queries
$pd_exec = false;

# Show links to xhtml and css validators
$pd_valid = false;

/* Advanced MySQL Configuration
   ---------------------------- */

# Hostname for your MySQL server
$np_db_host = "localhost";

# TCP port the mysql server is listening on, or socket path
$np_db_port = "3306";

# You should leave these settings as they are, unless you specified different tables in the denora config.
$denora_user_db = "user"; # Name of the Users table
$denora_chan_db = "chan"; # Name of the Channel table
$denora_chanbans_db = "chanbans"; # Name of the Channel Bans table
$denora_chanexcept_db = "chanexcept"; # Name of the Channel Exceptions table
$denora_chaninvite_db = "chaninvites"; # Name of the Channel Invites table
$denora_gline_db = "glines"; # Name of the Glines table
$denora_sqline_db = "sqline"; # Name of the SQLine table (required for Bahamut based ircd)
$denora_maxvalues = "maxvalues"; # Name of the Maxvalues table
$denora_server_db = "server"; # Name of the Servers table
$denora_ison_db = "ison"; # Name of the ISON table
$denora_tld_db = "tld"; # Name of the TLD table
$denora_cstats_db = "cstats"; # Name of the Chan Stats Channel table (cstats)
$denora_ustats_db = "ustats"; # Name of the Chan Stats User table (ustats)
$denora_current_db = "current"; # Name of the current stats table
$denora_serverstats = "serverstats"; # Name of the server hourly stats table
$denora_channelstats = "channelstats"; # Name of the channel hourly stats table
$denora_userstats = "stats"; # Name of the user hourly stats table
$denora_aliases_db = "aliases"; # Name of the aliases table

?>
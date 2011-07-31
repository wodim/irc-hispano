<?php
// Classic Theme Structure for phpDenora 1.2.0
// $Id: theme.php 328 2007-11-29 13:47:11Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

// ("filename", width, height)

// Network Status
$tpl_network_users = array("user.png", 22, 22);
$tpl_network_chans = array("channel.png", 22, 22);
$tpl_network_opers = array("oper.png", 22, 22);
$tpl_network_servers = array("server.png", 22, 22);
$tpl_network_max = array("max.png", 16, 16);

// User Status
$tpl_status_online = array("online.png", 16, 16);
$tpl_status_offline = array("offline.png", 16, 16);
$tpl_status_away = array("away.png", 16, 16);
$tpl_status_helper = array("helper.png", 16, 16);
$tpl_status_service = array("service.png", 16, 16);
$tpl_status_bot = array("bot.png", 16, 16);

// Chat
$tpl_mirc = array("mirc.gif", 16, 16);
$tpl_webchat = array("webchat.png", 16, 16);

// Sorting
$tpl_asc = array("s_asc.png", 11, 9);
$tpl_desc = array("s_desc.png", 11, 9);

// Bar Graph (non-GD)
$tpl_col = array("col.png", 15);

// Graphs
$tpl_graph_bg = "black"; # frame and plot background
$tpl_graph_axis = "darkgray"; # axis color
$tpl_graph_labels = "lightgray"; # labels color

// Line Plots
$tpl_line_color = "lightgray"; # color of the line
$tpl_maxline_color = "red"; # color of the line
$tpl_minline_color = "blue"; # color of the line
$tpl_grid_fill = array('black','#202020'); # alternated grid fill
$tpl_line_users = "lightgreen@0.1"; # users line fill
$tpl_line_channels = "lightpink@0.1"; # channels line fill
$tpl_line_servers = "lightblue@0.1"; # servers line fill

// Bar Graphs
$tpl_bar_border = "darkgray";
$tpl_bar_fill = array("green","darkgreen");

// Pie Graphs
$tpl_pie_theme = "pastel"; # Available: earth, pastel, sand, water

// Error handling
$tpl_err_info = array("info.png", 32, 32);
$tpl_err_warning = array("warning.png", 32, 32);
$tpl_err_error = array("error.png", 32, 32);

?>

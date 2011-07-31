<?php
// Futura Theme Structure for phpDenora 1.2.0
// $Id: theme.php 210 2006-08-17 23:48:48Z Hal9000 $

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
$tpl_graph_bg = "white"; # frame and plot background
$tpl_graph_axis = "lightgray"; # axis color
$tpl_graph_labels = "darkgray"; # labels color

// Line Plots
$tpl_line_color = "darkgray"; # color of the line
$tpl_maxline_color = "red"; # color of the line
$tpl_minline_color = "blue"; # color of the line
$tpl_grid_fill = array('#EFEFEF@0.5','#BBCCFF@0.5'); # alternated grid fill
$tpl_line_users = "lightgreen@0.5"; # users line fill
$tpl_line_channels = "lightpink@0.5"; # channels line fill
$tpl_line_servers = "lightblue@0.5"; # servers line fill

// Bar Graphs
$tpl_bar_border = "lightgray";
$tpl_bar_fill = array("orange","darkorange");

// Pie Graphs
$tpl_pie_theme = "water"; # Available: earth, pastel, sand, water

// Error handling
$tpl_err_info = array("info.png", 32, 32);
$tpl_err_warning = array("warning.png", 32, 32);
$tpl_err_error = array("error.png", 32, 32);

?>

<?php
// $Id: content.php 201 2006-08-16 21:46:38Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

switch ($mode) {
	case 'h':
		include "pages/home.php";
		break;
	case 'n':
		include "pages/network.php";
		break;
	case 'g':
		include "pages/graphs.php";
		break;
	case 'c':
		include "pages/channels.php";
		break;
	case 'u':
		include "pages/users.php";
		break;
	case 's':
		include "pages/search.php";
		break;
	case 'k':
		include "pages/config.php";
		break;
	default: phpdenora_error(1,$mode);
}

?> 
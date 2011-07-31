<?php
// $Id: config.php 353 2009-03-04 08:51:01Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

	echo "<div><form id=\"appearance\" method=\"post\" action=\"?m=k\">\n",
	"<div class=\"title\"><a name=\"language\" id=\"language\"></a>"._SM_LANGUAGE."</div><div>\n";
	foreach (glob("lang/*") as $filename) {
		if (substr($filename, -4, 4) != ".php" && file_exists($filename."/index.php")) {
			require($filename."/index.php");
			if ($lang_ver >= "1.3.0") {
				$langlist = explode("/", $filename);
				echo "<input name=\"lang\" type=\"radio\" value=\"".$langlist[1]."\"";
				if ($lang == $langlist[1]) { echo " checked=\"checked\""; }
				echo " /><img src=\"lang/".$langlist[1]."/".$lang_flag[0]."\" width=\"".$lang_flag[1]."\" height=\"".$lang_flag[2]."\" alt=\"".$lang_name."\" /> ".$lang_name."<br />\n";
			}
		}
	}
	echo "<p>&nbsp;</p></div>";

	echo "<hr /><div class=\"right\"><input type=\"submit\" name=\"Submit\" value=\""._SM_SET."\" /></div>\n",
	"</form></div>";
?>
<?php
// Futura Theme Main Layout for phpDenora 1.2.0
// $Id: main.php 206 2006-08-17 19:31:15Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );
?>

<div id="frame">
  <table width="100%" border="0" cellspacing="0" cellpadding="0" id="maintable">
    <tr>
      <td align="center" id="sitelogo"><a href="<?php echo $pd_neturl ?>">&nbsp;</a></td>
	  <?php if ($pd_adsense == true) { ?>
      <td align="center" id="siteads">
		<p class="center">
        <script type="text/javascript"><!--
			google_ad_client = "<?php echo $pd_adsense_client ?>";
			google_ad_width = 468;
			google_ad_height = 60;
			google_ad_format = "468x60_as";
			google_ad_type = "text_image";
			google_ad_channel = "<?php echo $pd_adsense_channel ?>";
			google_color_border = "FFFFFF";
			google_color_bg = "FFFFFF";
			google_color_link = "32527A";
			google_color_text = "333333";
			google_color_url = "008000";	
			//--></script>
        <script type="text/javascript"
			  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
			</script>
		</p>
      </td>
	  <?php } else { ?>
	  <td align="right" valign="bottom" id="nositeads">
	  <h1><?php echo $pd_netname ." - "._TM_TITLE; ?></h1>
	  </td>
	  <?php } ?>
    </tr>
    <tr>
      <td valign="top" id="sitemenu"><table width="100%" border="0" cellspacing="0" cellpadding="0" id="menutable">
          <tr>
            <td align="center"><a href="?m=h"><img src="themes/futura/img/home.png" alt="" /><br />
              <?php echo _TM_HOME; ?></a></td>
          </tr>
          <tr>
            <td align="center"><a href="?m=n"><img src="themes/futura/img/network.png" alt="" /><br />
              <?php echo _TM_NETWORK; ?></a></td>
          </tr>
          <?php if ($pd_ennetgraphs == true || $pd_netsplit_graphs == true || $pd_searchirc_graphs == true) { ?>
          <tr>
            <td align="center"><a href="?m=g"><img src="themes/futura/img/graphs.png" alt="" /><br />
              <?php echo _TM_GRAPHS; ?></a></td>
          </tr>
          <?php } ?>
          <tr>
            <td align="center"><a href="?m=c"><img src="themes/futura/img/channels.png" alt="" /><br />
              <?php echo _TM_CHANS; ?></a></td>
          </tr>
          <tr>
            <td align="center"><a href="?m=u"><img src="themes/futura/img/users.png" alt="" /><br />
              <?php echo _TM_USERS; ?></a></td>
          </tr>
        </table>
        <?php if ($pd_adsense == true) { ?>
        <p class="center" style="margin: 10px;">
          <script type="text/javascript"><!--
			google_ad_client = "<?php echo $pd_adsense_client ?>";
			google_ad_width = 120;
			google_ad_height = 600;
			google_ad_format = "120x600_as";
			google_ad_type = "text_image";
			google_ad_channel = "<?php echo $pd_adsense_channel ?>";
			google_color_border = "E6E6E6";
			google_color_bg = "E6E6E6";
			google_color_link = "32527A";
			google_color_text = "333333";
			google_color_url = "008000";	
			//--></script>
          <script type="text/javascript"
			  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
		</script>
        </p>
        <?php } else { ?>&nbsp;<?php } ?>
      </td>
      <td valign="top" id="sitecontent" rowspan="2">
<?php
$menu = getMenu($mode);
$previous = 3;
$tcount = 0;
$submenu = NULL;
if ($mode != "h" && $mode != "s" && $mode != "k") {
	foreach($menu as $item) {
		if ($item[0] == 0) {
			if ($previous == 1 && $tcount == 0) {
				echo "</div><div style=\"background-color: #FFFFFF; padding: 3px; border-bottom: 1px dotted #333333; margin: 0px; text-align: left;\">";
			}
			elseif ($previous == 3 && $tcount == 0) {
				echo "<div style=\"background-color: #FFFFFF; padding: 3px; border-bottom: 1px dotted #333333; margin: 0px; text-align: left;\">";
			}
			echo "<strong>".$item[1]."</strong>: ";
			$previous = 0;
			$tcount++;
		}
		elseif ($item[0] == 1) {
			if ($previous == 3) {
				echo "<div style=\"background-color: #E6E6E6; padding: 3px; border-bottom: 1px solid #666666; margin: 0px; text-align: left;\">";
				if ($mode == "n") {
					echo "<strong>"._TM_NETWORK."</strong>: ";
				}
				elseif ($mode == "g") {
					echo "<strong>"._TM_GRAPHS."</strong>: ";
				}
				elseif ($mode == "c") {
					echo "<strong>"._TM_CHANS."</strong>: ";
				}
				elseif ($mode == "u") {
					echo "<strong>"._TM_USERS."</strong>: ";
				}
			}
			elseif ($previous == 1 || $previous == 2) {
				echo " - ";
			}
			echo "<a href=\"".$item[2]."\">".$item[1]."</a>\n";
			$previous = 1;
		}
		elseif ($item[0] == 2) {
			if ($item[2]{0} != "#") {
				if ($previous == 2) {
					$submenu .= "</td><td>";
				}
				$submenu .= "<a href=\"".$item[2]."\">".$item[1]."</a>\n";
				$previous = 2;
			}
		}
	}
	if ($previous != 3) {
		echo "</div>";
	}
}
?>
		<div id="content">
		<?php
		if ($submenu != NULL) {
			echo "<table cellpadding=\"2\" cellspacing=\"0\" id=\"submenu\"><tr><td>".$submenu."</td></tr></table>";
		}
        include "pages/content.php";
		?>
		</div>
      </td>
    </tr>
    <tr>
      <td valign="bottom" id="bottombuttons">
        <table width="100%" id="bottomtable" cellspacing="0" cellpadding="0">
          <tr>
            <td align="center"><a href="?m=s"><img src="themes/futura/img/search.png" alt="" /><br /><?php echo _TM_SEARCH; ?></a></td>
            <td align="center"><a href="?m=k"><img src="themes/futura/img/config.png" alt="" /><br /><?php echo _TM_CONFIG; ?></a></td>
          </tr>
        </table>
      </td>
    </tr>
  </table>
  <?php displayFooter(); ?>
</div>

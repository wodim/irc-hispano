<?php
// Classic Theme Main Layout for phpDenora 1.2.0
// $Id: main.php 331 2008-02-05 16:53:52Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );
?>

<div id="pd_frame">
  <div id="pd_header">
    <table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td valign="middle"><a href="<?php echo $pd_neturl ?>"><img src="themes/classic/img/logo.png" alt="" id="logo" /></a></td>
        <td valign="middle"><table border="0" cellspacing="0" cellpadding="2">
            <tr align="center">
              <td class="topmenu"><a href="?m=h"><img src="themes/classic/img/home.png" alt="" /><br />
                <?php echo _TM_HOME; ?></a></td>
              <td class="topmenu"><a href="?m=n"><img src="themes/classic/img/network.png" alt="" /><br />
                <?php echo _TM_NETWORK; ?></a></td>
              <?php if ($pd_ennetgraphs == true || $pd_netsplit_graphs == true || $pd_searchirc_graphs == true) { ?>
              <td class="topmenu"><a href="?m=g"><img src="themes/classic/img/graphs.png" alt="" /><br />
                <?php echo _TM_GRAPHS; ?></a></td>
              <?php } ?>
              <td class="topmenu"><a href="?m=c"><img src="themes/classic/img/channels.png" alt="" /><br />
                <?php echo _TM_CHANS; ?></a></td>
              <td class="topmenu"><a href="?m=u"><img src="themes/classic/img/users.png" alt="" /><br />
                <?php echo _TM_USERS; ?></a></td>
              <td class="topmenu"><a href="?m=s"><img src="themes/classic/img/search.png" alt="" /><br />
                <?php echo _TM_SEARCH; ?></a></td>
              <td class="topmenu"><a href="?m=k"><img src="themes/classic/img/config.png" alt="" /><br />
                <?php echo _TM_CONFIG; ?></a></td>
            </tr>
          </table></td>
        <td align="right" valign="bottom"><h1><?php echo $pd_netname ."<br />"._TM_TITLE; ?></h1></td>
      </tr>
    </table>
  </div>
  <div id="pd_main">
    <table id="maintable">
      <tr>
        <td valign="top" id="pd_menutd"><div id="pd_menu">
            <?php
		  $menu = getMenu($mode);
		  $previous = 3;
		  foreach($menu as $item) {
		  	if ($item[0] == 0) {
				if ($previous == 1 || $previous == 2) {
					echo "</ul></div>\n";
				}
				$itemtitle = (strlen($item[1]) > 14) ? substr($item[1], 0, 11) . "..." : $item[1];
				echo "<p>&nbsp;</p><h3>".$itemtitle."</h3><p>&nbsp;</p>\n";
				$previous = 0;
			}
			elseif ($item[0] == 1) {
				if ($previous == 0 || $previous == 3) {
					echo "<div class=\"idmenu\"><ul>\n";
				}
				elseif ($previous == 2) {
					echo "</ul></div><div class=\"idmenu\">\n<ul>\n";
				}
				echo "<li><a href=\"".$item[2]."\">".$item[1]."</a></li>\n";
				$previous = 1;
			}
			elseif ($item[0] == 2) {
				if ($previous == 1) {
					echo "</ul></div><div class=\"idsubmenu\"><ul>\n";
				}
				echo "<li><a href=\"".$item[2]."\">".$item[1]."</a></li>\n";
				$previous = 2;
			}
		  }
		  if ($previous != 3) {
			  echo "</ul></div>\n";
			  if ($previous == 2) {
			  	echo "<div class=\"idmenu\"></div>\n";
			  }
		  }
		  ?>
          </div>
          <?php if ($pd_adsense == true) { ?>
          <div style="text-align:center;">
            <script type="text/javascript"><!--
			google_ad_client = "<?php echo $pd_adsense_client ?>";
			google_ad_width = 120;
			google_ad_height = 600;
			google_ad_format = "120x600_as";
			google_ad_type = "text_image";
			google_ad_channel ="<?php echo $pd_adsense_channel ?>";
			google_alternate_color = "1d2a62";
			google_color_border = "1d2a62";
			google_color_bg = "1d2a62";
			google_color_link = "71C2FF";
			google_color_text = "666666";
			google_color_url = "00D993";
			//--></script>
            <script type="text/javascript"
			  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
			</script>
          </div>
          <?php } ?>
        </td>
        <td valign="top"><div id="pd_content">
            <?php if ($pd_adsense == true) { ?>
            <script type="text/javascript"><!--
				google_ad_client = "<?php echo $pd_adsense_client ?>";
				google_ad_width = 468;
				google_ad_height = 60;
				google_ad_format = "468x60_as";
				google_ad_type = "text_image";
				google_ad_channel = "<?php echo $pd_adsense_channel ?>";
				google_color_border = "000000";
				google_color_bg = "000000";
				google_color_link = "00D993";
				google_color_text = "999999";
				google_color_url = "666666";	
				//--></script>
            <script type="text/javascript"
				  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
				</script>
            <?php } include "pages/content.php"; ?>
          </div></td>
      </tr>
    </table>
  </div>
  <div id="pd_footer">
    <?php displayFooter(); ?>
  </div>
</div>

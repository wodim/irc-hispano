<?php
// $Id: utils.php 403 2010-05-29 18:43:12Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

/* -------------------------------.
	General use utility functions |
   -------------------------------´
*/

/* Sorts an array by field */
function sort_by_field($a,$f,$t) 
{
	$s1 = array(); $s2 = array();
	
	$x = (@func_get_arg(3) ? func_get_arg(3) : "0");
	$count = ($x == "0" ? count($a) - 1 : count($a));
	for ($i = $x; $i <= $count; $i++)
	{
		$s1[$i] = $a[$i][$f];
	}
	natcasesort($s1);
	if (preg_match('/^(?:2|desc)$/i', $t))
	{
		$s1 = array_reverse($s1,TRUE);
	}
	$i = $x; 
	foreach ($s1 as $k => $v)
	{
		$s2[$i++] = $a[$k];
	}
	return $s2;
}

/* Returns an array containing number of days, hours, minutes and seconds */
function seconds_to_time($seconds)
{
   $days = 0;
   $hours = 0;
   $minutes = 0;
   while ($seconds >= 60) {
       if ($seconds >= 86400) {
           // there is more than 1 day
           $days = floor($seconds / 86400);
           $seconds = $seconds - ($days * 86400);
       } elseif ($seconds >= 3600) {
           $hours = floor($seconds / 3600);
           $seconds = $seconds - ($hours * 3600);
       } elseif ($seconds >= 60) {
           $minutes = floor($seconds / 60);
           $seconds = $seconds - ($minutes * 60);
       }
   }
   return array('days' => $days, 'hours' => $hours, 'minutes' => $minutes, 'seconds' => $seconds);
}
	
function translatecolorcode($matches) {
	$color = array(
		'#FFFFFF',
		'#000000',
		'#00007F',
		'#009300',
		'#FF0000',
		'#7F0000',
		'#9C009C',
		'#FC7F00',
		'#FFFF00',
		'#00FC00',
		'#009393',
		'#00FFFF',
		'#0000FC',
		'#FF00FF',
		'#7F7F7F',
		'#D2D2D2',
	);
	$options = '';
	
	if($matches[2] != '') {
		$bgcolor = trim(substr($matches[2],1));
		if (isset($color[(int)$bgcolor])) {
			$options .= 'background-color: ' . $color[(int)$bgcolor] . '; ';
		}
	}
	
	$forecolor = trim($matches[1]);
	if($forecolor != '') {
		if (isset($color[(int)$forecolor])) {
			$options .= 'color: ' . $color[(int)$forecolor] . ';';
		}
	}
	
	if($options != '') {
		return '<span style="' . $options . '">' . $matches[3] . '</span>';
	} else {
		return $matches[3];
	}
}

function denora_colorconvert($body) {
	global $charset;
	$lines = explode("\n", $body);
	$out = '';
	
	foreach($lines as $line) {
		$line = nl2br(htmlentities($line,ENT_COMPAT,$charset));
		// replace control codes
		$line = preg_replace_callback('/[\003](\d{0,2})(,\d{1,2})?([^\003\x0F]*)(?:[\003](?!\d))?/','translatecolorcode',$line);
		$line = preg_replace('/[\002]([^\002\x0F]*)(?:[\002])?/','<strong>$1</strong>',$line);
		$line = preg_replace('/[\x1F]([^\x1F\x0F]*)(?:[\x1F])?/','<span style="text-decoration: underline;">$1</span>',$line);
		$line = preg_replace('/[\x12]([^\x12\x0F]*)(?:[\x12])?/','<span style="text-decoration: line-through;">$1</span>',$line);
		$line = preg_replace('/[\x16]([^\x16\x0F]*)(?:[\x16])?/','<span style="font-style: italic;">$1</span>',$line);
		$line = preg_replace('@(https?://([-\w\.]+)+(:\d+)?(/([\S+]*(\?\S+)?)?)?)@', "<a href='$1' class='topic'>$1</a>", $line);	    
		// remove dirt
		$line = preg_replace('/[\x00-\x1F]/', '', $line);
		$line = preg_replace('/[\x7F-\xFF]/', '', $line);
		// append line
		if($line != '') {
			$out .= $line;
		}
	}
	
	return $out;
}

/* Returns the Denora version. Used by the setup page. */
function phpdenora_versioncheck($server,$what)
{
	global $denora_server_db;
	$q = sql_query("SELECT version FROM ".$denora_server_db." WHERE server='".$server."';");
	$r = sql_fetch_array($q);
	switch ($what) {
		case 'full':
			return $r[0];
			break;
		case 'num':
			$pattern = '/([0-9.]+)/';
			preg_match($pattern, $r[0], $num);
			return @$num[1];
			break;
		case 'ext':
			$pattern = '/([0-9.]+)/';
			preg_match($pattern, $r[0], $num);
			if (!$num) {
				$pattern = '/\-(.*)\s/';
				preg_match($pattern, $r[0], $ext);
			} else {
				$ext = explode("(",substr($r[0], 0, -1));	
			}
			return @$ext[1];
			break;
		case 'rev':
			$pattern = '/([0-9.]+)/';
			preg_match($pattern, $r[0], $num);
			if (!$num) {
				$rev = explode("(",substr($r[0], 0, -1));
			} else {
				$pattern = '/(\.[0-9]+)\s/';
				preg_match($pattern, $r[0], $rev);
				$rev[1] = substr($rev[1], 1);
			}
			return @$rev[1];
			break;
		default:
			return NULL;
 	}
}

/* Error handling function */
function phpdenora_error($what,$info)
{
	global $theme, $tpl_err_error, $tpl_err_warning, $tpl_err_info;
	$info = $info ? $info : _ER_NODATA;
	switch ($what) {
		case 1:
			$icon = $tpl_err_error[0];
			$title = "<h3 class=\"center\" style=\"color:red\">"._ER_ERROR."</h3>";
			$message = sprintf(_ER_MODE,"<br />\"<span class=\"color\">$info</span>\"<br />");
			break;
		case 2:
			$icon = $tpl_err_warning[0];
			$title = "<h3 class=\"center\" style=\"color:orange\">"._ER_WARNING."</h3>";
			$message = $info;
			break;
		case 3:
			$icon = $tpl_err_info[0];
			$title = "<h3 class=\"center\" style=\"color:blue\">"._ER_INFO."</h3>";
			$message = $info;
			break;
		default:
			$icon = $tpl_err_error[0];
			$title = "<h3 class=\"center\" style=\"color:red\">"._ER_ERROR."</h3>";
			$message = "Unknown internal error";
	}
	echo "<div class=\"error\">
		<table class=\"error\" border=\"0\" cellspacing=\"0\" cellpadding=\"5\">
			<tr>
				<td class=\"lerr\" align=\"center\">
					<img src=\"themes/$theme/img/$icon\" alt=\"\" />
					$title
				</td>
				<td class=\"rerr\" align=\"center\">
					<p class=\"center\">$message</p>
				</td>
			</tr>
		</table>
	</div>";
}

?>

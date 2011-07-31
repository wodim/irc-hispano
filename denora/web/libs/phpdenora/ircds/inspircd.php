<?php
// $Id: inspircd.php 321 2007-09-13 08:00:43Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

$ircd = array('supports_hidden_ops' => 1,
              'hidden_op_mode' => 'mode_uh',
              'helper'         => 1,
              'helper_mode'    => 'mode_lh',
              'bot'            => 1,
              'bot_mode'       => 'mode_ub',
              'chanexception'  => 1,
              'chaninvites'    => 1,
              'sqlinetable'    => 0,
              'glinetable'     => 1,
              'services_protection' => 0,
              'services_protection_mode' => '',
              'chanhide'       => 1,
              'chanhide_mode'  => 'mode_ui',
              'secretchan'     => 1,
              'secretchan_mode' => 'mode_ls',
              'privatechan'    => 1,
              'privatechan_mode' => 'mode_lp',
			  'host_cloaking'  => 1);

?>
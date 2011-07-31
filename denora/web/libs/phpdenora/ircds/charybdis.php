<?php
// $Id: charybdis.php 330 2008-02-03 16:26:13Z Hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

$ircd = array('supports_hidden_ops' => 0,
              'hidden_op_mode' => '',
              'helper'         => 0,
              'helper_mode'    => '',
              'bot'            => 0,
              'bot_mode'       => '',
              'chanexception'  => 1,
              'chaninvites'    => 1,
              'sqlinetable'    => 0,
              'glinetable'     => 1,
              'services_protection' => 1,
              'services_protection_mode' => 'mode_us',
              'chanhide'       => 0,
              'chanhide_mode'  => '',
              'secretchan'     => 1,
              'secretchan_mode' => 'mode_ls',
              'privatechan'    => 0,
              'privatechan_mode' => '',
			  'host_cloaking'  => 1);

?>
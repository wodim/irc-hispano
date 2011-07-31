<?php
// $Id: hybrid.php 286 2007-01-15 11:42:44Z Hal9000 $

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
              'sqlinetable'    => 1,
              'glinetable'     => 0,
              'services_protection' => 0,
              'services_protection_mode' => '',
              'chanhide'       => 0,
              'chanhide_mode'  => '',
              'secretchan'     => 1,
              'secretchan_mode' => 'mode_ls',
              'privatechan'    => 0,
              'privatechan_mode' => '',
			  'host_cloaking'  => 1);

?>
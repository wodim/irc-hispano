<?php
// $Id: nefarious.php 409 2010-07-05 13:05:14Z hal9000 $

/** ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

$ircd = array('supports_hidden_ops' => 0,
              'hidden_op_mode' => '',
              'helper'         => 0,
              'helper_mode'    => '',
              'bot'            => 1,
              'bot_mode'       => 'mode_ub',
              'chanexception'  => 1,
              'chaninvites'    => 0,
              'sqlinetable'    => 0,
              'glinetable'     => 0,
              'services_protection' => 1,
              'services_protection_mode' => 'mode_lk',
              'chanhide'       => 1,
              'chanhide_mode'  => 'mode_ln',
              'secretchan'     => 1,
              'secretchan_mode' => 'mode_ls',
              'privatechan'    => 1,
              'privatechan_mode' => 'mode_lp',
			  'host_cloaking'  => 1);

?>

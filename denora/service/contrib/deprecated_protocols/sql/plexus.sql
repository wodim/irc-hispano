-- --------------------------------------------------------
-- $Id: plexus.sql 31 2007-07-18 11:39:31Z Hal9000 $
-- IMPORTANT: Load denora.sql FIRST!
-- --------------------------------------------------------

ALTER TABLE `chan` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_lm` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_um` enum('Y','N') NOT NULL default 'N',
ADD `mode_un` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lb` enum('Y','N') NOT NULL default 'Y',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ld` enum('Y','N') NOT NULL default 'N',
ADD `mode_lf` enum('Y','N') NOT NULL default 'Y',
ADD `mode_lg` enum('Y','N') NOT NULL default 'N',
ADD `mode_lh` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'Y',
ADD `mode_ll` enum('Y','N') NOT NULL default 'Y',
ADD `mode_ln` enum('Y','N') NOT NULL default 'Y',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'Y',
ADD `mode_lu` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_ly` enum('Y','N') NOT NULL default 'Y',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N';

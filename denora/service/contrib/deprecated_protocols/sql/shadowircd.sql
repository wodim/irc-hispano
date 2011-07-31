-- --------------------------------------------------------
-- $Id: shadowircd.sql 31 2007-07-18 11:39:31Z Hal9000 $
-- IMPORTANT: Load denora.sql FIRST!
-- --------------------------------------------------------

ALTER TABLE `chan` ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ll` enum('Y','N') NOT NULL default 'N',
ADD `mode_lm` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lp` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_lu` enum('Y','N') NOT NULL default 'N',
ADD `mode_ly` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_ua` enum('Y','N') NOT NULL default 'N',
ADD `mode_uc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ud` enum('Y','N') NOT NULL default 'N',
ADD `mode_ug` enum('Y','N') NOT NULL default 'N',
ADD `mode_uh` enum('Y','N') NOT NULL default 'N',
ADD `mode_uk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ul` enum('Y','N') NOT NULL default 'N',
ADD `mode_um` enum('Y','N') NOT NULL default 'N',
ADD `mode_un` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_uq` enum('Y','N') NOT NULL default 'N',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N',
ADD `mode_ut` enum('Y','N') NOT NULL default 'N',
ADD `mode_uv` enum('Y','N') NOT NULL default 'N',
ADD `mode_ux` enum('Y','N') NOT NULL default 'N',
ADD `mode_uu` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0',
ADD `mode_ul_data` varchar(33) NOT NULL default '';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lq` enum('Y','N') NOT NULL default 'N',
ADD `mode_lv` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_uk` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N';

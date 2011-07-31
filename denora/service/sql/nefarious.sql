-- --------------------------------------------------------
-- $Id: nefarious.sql 324 2009-09-10 16:53:30Z hal9000 $
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
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lt` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_uc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ul` enum('Y','N') NOT NULL default 'N',
ADD `mode_um` enum('Y','N') NOT NULL default 'N',
ADD `mode_un` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_uq` enum('Y','N') NOT NULL default 'N',
ADD `mode_us` enum('Y','N') NOT NULL default 'N',
ADD `mode_ut` enum('Y','N') NOT NULL default 'N',
ADD `mode_uz` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk_data` varchar(23) NOT NULL default '',
ADD `mode_ll_data` int(10) NOT NULL default '0';

-- --------------------------------------------------------

ALTER TABLE `user` ADD `mode_la` enum('Y','N') NOT NULL default 'N',
ADD `mode_lc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ld` enum('Y','N') NOT NULL default 'N',
ADD `mode_lf` enum('Y','N') NOT NULL default 'N',
ADD `mode_lg` enum('Y','N') NOT NULL default 'N',
ADD `mode_lh` enum('Y','N') NOT NULL default 'N',
ADD `mode_li` enum('Y','N') NOT NULL default 'N',
ADD `mode_lk` enum('Y','N') NOT NULL default 'N',
ADD `mode_ln` enum('Y','N') NOT NULL default 'N',
ADD `mode_lo` enum('Y','N') NOT NULL default 'N',
ADD `mode_lr` enum('Y','N') NOT NULL default 'N',
ADD `mode_ls` enum('Y','N') NOT NULL default 'N',
ADD `mode_lx` enum('Y','N') NOT NULL default 'N',
ADD `mode_lw` enum('Y','N') NOT NULL default 'N',
ADD `mode_lz` enum('Y','N') NOT NULL default 'N',
ADD `mode_ub` enum('Y','N') NOT NULL default 'N',
ADD `mode_uc` enum('Y','N') NOT NULL default 'N',
ADD `mode_ud` enum('Y','N') NOT NULL default 'N',
ADD `mode_ui` enum('Y','N') NOT NULL default 'N',
ADD `mode_uo` enum('Y','N') NOT NULL default 'N',
ADD `mode_ur` enum('Y','N') NOT NULL default 'N',
ADD `mode_uw` enum('Y','N') NOT NULL default 'N',
ADD `mode_ux` enum('Y','N') NOT NULL default 'N';
    
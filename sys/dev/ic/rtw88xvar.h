#ifndef _DEV_IC_RTW88XVAR_H
#define _DEV_IC_RTW88XVAR_H 1

/* hardware configuration for each IC */
struct rtw_chip_info {
#if 0
	const struct rtw_chip_ops *ops;
	u8 id;

	const char *fw_name;
	enum rtw_wlan_cpu wlan_cpu;
	u8 tx_pkt_desc_sz;
	u8 tx_buf_desc_sz;
	u8 rx_pkt_desc_sz;
	u8 rx_buf_desc_sz;
	u32 phy_efuse_size;
	u32 log_efuse_size;
	u32 ptct_efuse_size;
	u32 txff_size;
	u32 rxff_size;
	u32 fw_rxff_size;
	u16 rsvd_drv_pg_num;
	u8 band;
	u16 page_size;
	u8 csi_buf_pg_num;
	u8 dig_max;
	u8 dig_min;
	u8 txgi_factor;
	bool is_pwr_by_rate_dec;
	bool rx_ldpc;
	bool tx_stbc;
	u8 max_power_index;
	u8 ampdu_density;

	u16 fw_fifo_addr[RTW_FW_FIFO_MAX];
	const struct rtw_fwcd_segs *fwcd_segs;

	bool amsdu_in_ampdu;
	u8 usb_tx_agg_desc_num;
	bool hw_feature_report;
	u8 c2h_ra_report_size;
	bool old_datarate_fb_limit;

	u8 default_1ss_tx_path;

	bool path_div_supported;
	bool ht_supported;
	bool vht_supported;
	u8 lps_deep_mode_supported;

	/* init values */
	u8 sys_func_en;
	const struct rtw_pwr_seq_cmd * const *pwr_on_seq;
	const struct rtw_pwr_seq_cmd * const *pwr_off_seq;
	const struct rtw_rqpn *rqpn_table;
	const struct rtw_prioq_addrs *prioq_addrs;
	const struct rtw_page_table *page_table;
	const struct rtw_intf_phy_para_table *intf_table;

	const struct rtw_hw_reg *dig;
	const struct rtw_hw_reg *dig_cck;
	u32 rf_base_addr[RTW_RF_PATH_MAX];
	u32 rf_sipi_addr[RTW_RF_PATH_MAX];
	const struct rtw_rf_sipi_addr *rf_sipi_read_addr;
	u8 fix_rf_phy_num;
	const struct rtw_ltecoex_addr *ltecoex_addr;

	const struct rtw_table *mac_tbl;
	const struct rtw_table *agc_tbl;
	const struct rtw_table *bb_tbl;
	const struct rtw_table *rf_tbl[RTW_RF_PATH_MAX];
	const struct rtw_table *rfk_init_tbl;

	const struct rtw_rfe_def *rfe_defs;
	u32 rfe_defs_size;

	bool en_dis_dpd;
	u16 dpd_ratemask;
	u8 iqk_threshold;
	u8 lck_threshold;

	u8 bfer_su_max_num;
	u8 bfer_mu_max_num;

	const struct rtw_hw_reg_offset *edcca_th;
	s8 l2h_th_ini_cs;
	s8 l2h_th_ini_ad;

	const char *wow_fw_name;
	const struct wiphy_wowlan_support *wowlan_stub;
	const u8 max_sched_scan_ssids;
	const u16 max_scan_ie_len;

	/* coex paras */
	u32 coex_para_ver;
	u8 bt_desired_ver;
	bool scbd_support;
	bool new_scbd10_def; /* true: fix 2M(8822c) */
	bool ble_hid_profile_support;
	bool wl_mimo_ps_support;
	u8 pstdma_type; /* 0: LPSoff, 1:LPSon */
	u8 bt_rssi_type;
	u8 ant_isolation;
	u8 rssi_tolerance;
	u8 table_sant_num;
	u8 table_nsant_num;
	u8 tdma_sant_num;
	u8 tdma_nsant_num;
	u8 bt_afh_span_bw20;
	u8 bt_afh_span_bw40;
	u8 afh_5g_num;
	u8 wl_rf_para_num;
	u8 coex_info_hw_regs_num;
	const u8 *bt_rssi_step;
	const u8 *wl_rssi_step;
	const struct coex_table_para *table_nsant;
	const struct coex_table_para *table_sant;
	const struct coex_tdma_para *tdma_sant;
	const struct coex_tdma_para *tdma_nsant;
	const struct coex_rf_para *wl_rf_para_tx;
	const struct coex_rf_para *wl_rf_para_rx;
	const struct coex_5g_afh_map *afh_5g;
	const struct rtw_hw_reg *btg_reg;
	const struct rtw_reg_domain *coex_info_hw_regs;
	u32 wl_fw_desired_ver;
#endif
};

struct rtw88x_softc {
	struct device				 sc_dev;

	const struct rtw_chip_info	*sc_chipinfo;
};

extern const struct rtw_chip_info rtw8822b_hw_spec;

#endif /* _DEV_IC_RTW88XVAR_H */

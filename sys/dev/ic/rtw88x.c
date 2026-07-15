#include "bpfilter.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/endian.h>

#include <machine/bus.h>
#include <machine/intr.h>	/* splnet */

#include <net/if.h>
#include <net/if_media.h>

#if NBPFILTER > 0
#include <net/bpf.h>
#endif

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_radiotap.h>

#include "rtw88xvar.h"

struct cfdriver rtw88x_cd = {
	NULL, "rtw88x", DV_IFNET
};

const struct rtw_chip_info rtw8822b_hw_spec = {
#if 0
	.ops = &rtw8822b_ops,
	.id = RTW_CHIP_TYPE_8822B,
	.fw_name = "rtw88/rtw8822b_fw.bin",
	.wlan_cpu = RTW_WCPU_3081,
	.tx_pkt_desc_sz = 48,
	.tx_buf_desc_sz = 16,
	.rx_pkt_desc_sz = 24,
	.rx_buf_desc_sz = 8,
	.phy_efuse_size = 1024,
	.log_efuse_size = 768,
	.ptct_efuse_size = 96,
	.txff_size = 262144,
	.rxff_size = 24576,
	.fw_rxff_size = 12288,
	.rsvd_drv_pg_num = 8,
	.txgi_factor = 1,
	.is_pwr_by_rate_dec = true,
	.max_power_index = 0x3f,
	.csi_buf_pg_num = 0,
	.band = RTW_BAND_2G | RTW_BAND_5G,
	.page_size = TX_PAGE_SIZE,
	.dig_min = 0x1c,
	.amsdu_in_ampdu = true,
	.usb_tx_agg_desc_num = 3,
	.hw_feature_report = true,
	.c2h_ra_report_size = 7,
	.old_datarate_fb_limit = false,
	.ht_supported = true,
	.vht_supported = true,
	.lps_deep_mode_supported = BIT(LPS_DEEP_MODE_LCLK),
	.sys_func_en = 0xDC,
	.pwr_on_seq = card_enable_flow_8822b,
	.pwr_off_seq = card_disable_flow_8822b,
	.page_table = page_table_8822b,
	.rqpn_table = rqpn_table_8822b,
	.prioq_addrs = &prioq_addrs_8822b,
	.intf_table = &phy_para_table_8822b,
	.dig = rtw8822b_dig,
	.dig_cck = NULL,
	.rf_base_addr = {0x2800, 0x2c00},
	.rf_sipi_addr = {0xc90, 0xe90},
	.ltecoex_addr = &rtw8822b_ltecoex_addr,
	.mac_tbl = &rtw8822b_mac_tbl,
	.agc_tbl = &rtw8822b_agc_tbl,
	.bb_tbl = &rtw8822b_bb_tbl,
	.rf_tbl = {&rtw8822b_rf_a_tbl, &rtw8822b_rf_b_tbl},
	.rfe_defs = rtw8822b_rfe_defs,
	.rfe_defs_size = ARRAY_SIZE(rtw8822b_rfe_defs),
	.iqk_threshold = 8,
	.bfer_su_max_num = 2,
	.bfer_mu_max_num = 1,
	.rx_ldpc = true,
	.edcca_th = rtw8822b_edcca_th,
	.l2h_th_ini_cs = 10 + EDCCA_IGI_BASE,
	.l2h_th_ini_ad = -14 + EDCCA_IGI_BASE,
	.ampdu_density = IEEE80211_HT_MPDU_DENSITY_2,
	.max_scan_ie_len = IEEE80211_MAX_DATA_LEN,

	.coex_para_ver = 0x20070206,
	.bt_desired_ver = 0x6,
	.scbd_support = true,
	.new_scbd10_def = false,
	.ble_hid_profile_support = false,
	.wl_mimo_ps_support = false,
	.pstdma_type = COEX_PSTDMA_FORCE_LPSOFF,
	.bt_rssi_type = COEX_BTRSSI_RATIO,
	.ant_isolation = 15,
	.rssi_tolerance = 2,
	.wl_rssi_step = wl_rssi_step_8822b,
	.bt_rssi_step = bt_rssi_step_8822b,
	.table_sant_num = ARRAY_SIZE(table_sant_8822b),
	.table_sant = table_sant_8822b,
	.table_nsant_num = ARRAY_SIZE(table_nsant_8822b),
	.table_nsant = table_nsant_8822b,
	.tdma_sant_num = ARRAY_SIZE(tdma_sant_8822b),
	.tdma_sant = tdma_sant_8822b,
	.tdma_nsant_num = ARRAY_SIZE(tdma_nsant_8822b),
	.tdma_nsant = tdma_nsant_8822b,
	.wl_rf_para_num = ARRAY_SIZE(rf_para_tx_8822b),
	.wl_rf_para_tx = rf_para_tx_8822b,
	.wl_rf_para_rx = rf_para_rx_8822b,
	.bt_afh_span_bw20 = 0x24,
	.bt_afh_span_bw40 = 0x36,
	.afh_5g_num = ARRAY_SIZE(afh_5g_8822b),
	.afh_5g = afh_5g_8822b,

	.coex_info_hw_regs_num = ARRAY_SIZE(coex_info_hw_regs_8822b),
	.coex_info_hw_regs = coex_info_hw_regs_8822b,

	.fw_fifo_addr = {0x780, 0x700, 0x780, 0x660, 0x650, 0x680},
#endif
};


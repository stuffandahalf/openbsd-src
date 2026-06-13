#ifndef _DEV_IC_RTW88XREG_H
#define _DEV_IC_RTW88XREG_H 1

enum rtw_tx_queue_type {
	/* the order of AC queues matters */
	RTW_TX_QUEUE_BK = 0x0,
	RTW_TX_QUEUE_BE = 0x1,
	RTW_TX_QUEUE_VI = 0x2,
	RTW_TX_QUEUE_VO = 0x3,

	RTW_TX_QUEUE_BCN = 0x4,
	RTW_TX_QUEUE_MGMT = 0x5,
	RTW_TX_QUEUE_HI0 = 0x6,
	RTW_TX_QUEUE_H2C = 0x7,
	/* keep it last */
	RTK_MAX_TX_QUEUE_NUM
};

enum rtw_rx_queue_type {
	RTW_RX_QUEUE_MPDU = 0x0,
	RTW_RX_QUEUE_C2H = 0x1,
	/* keep it last */
	RTK_MAX_RX_QUEUE_NUM
};

#endif /* _DEV_IC_RTW88XREG_H */

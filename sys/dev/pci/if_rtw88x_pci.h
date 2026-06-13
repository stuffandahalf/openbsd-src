#ifndef _RTW88X_PCI_H
#define _RTW88X_PCI_H 1

#define BIT(n) (1 << (n))

/*
 * Interrupt mask registers
 */
#define RTK_PCI_HIMR0		0x0B0
#define RTK_PCI_HISR0		0x0B4
#define RTK_PCI_HIMR1		0x0B8
#define RTK_PCI_HISR1		0x0BC
#define RTK_PCI_HIMR2		0x10B0
#define RTK_PCI_HISR2		0x10B4
#define RTK_PCI_HIMR3		0x10B8
#define RTK_PCI_HISR3		0x10BC
/* IMR 0 */
#define IMR_TIMER2		BIT(31)
#define IMR_TIMER1		BIT(30)
#define IMR_PSTIMEOUT		BIT(29)
#define IMR_GTINT4		BIT(28)
#define IMR_GTINT3		BIT(27)
#define IMR_TBDER		BIT(26)
#define IMR_TBDOK		BIT(25)
#define IMR_TSF_BIT32_TOGGLE	BIT(24)
#define IMR_BCNDMAINT0		BIT(20)
#define IMR_BCNDOK0		BIT(16)
#define IMR_HSISR_IND_ON_INT	BIT(15)
#define IMR_BCNDMAINT_E		BIT(14)
#define IMR_ATIMEND		BIT(12)
#define IMR_HISR1_IND_INT	BIT(11)
#define IMR_C2HCMD		BIT(10)
#define IMR_CPWM2		BIT(9)
#define IMR_CPWM		BIT(8)
#define IMR_HIGHDOK		BIT(7)
#define IMR_MGNTDOK		BIT(6)
#define IMR_BKDOK		BIT(5)
#define IMR_BEDOK		BIT(4)
#define IMR_VIDOK		BIT(3)
#define IMR_VODOK		BIT(2)
#define IMR_RDU			BIT(1)
#define IMR_ROK			BIT(0)
/* IMR 1 */
#define IMR_TXFIFO_TH_INT	BIT(30)
#define IMR_BTON_STS_UPDATE	BIT(29)
#define IMR_MCUERR		BIT(28)
#define IMR_BCNDMAINT7		BIT(27)
#define IMR_BCNDMAINT6		BIT(26)
#define IMR_BCNDMAINT5		BIT(25)
#define IMR_BCNDMAINT4		BIT(24)
#define IMR_BCNDMAINT3		BIT(23)
#define IMR_BCNDMAINT2		BIT(22)
#define IMR_BCNDMAINT1		BIT(21)
#define IMR_BCNDOK7		BIT(20)
#define IMR_BCNDOK6		BIT(19)
#define IMR_BCNDOK5		BIT(18)
#define IMR_BCNDOK4		BIT(17)
#define IMR_BCNDOK3		BIT(16)
#define IMR_BCNDOK2		BIT(15)
#define IMR_BCNDOK1		BIT(14)
#define IMR_ATIMEND_E		BIT(13)
#define IMR_ATIMEND		BIT(12)
#define IMR_TXERR		BIT(11)
#define IMR_RXERR		BIT(10)
#define IMR_TXFOVW		BIT(9)
#define IMR_RXFOVW		BIT(8)
#define IMR_CPU_MGQ_TXDONE	BIT(5)
#define IMR_PS_TIMER_C		BIT(4)
#define IMR_PS_TIMER_B		BIT(3)
#define IMR_PS_TIMER_A		BIT(2)
#define IMR_CPUMGQ_TX_TIMER	BIT(1)
/* IMR 3 */
#define IMR_H2CDOK		BIT(16)

#endif /* _RTW88X_PCI_H */

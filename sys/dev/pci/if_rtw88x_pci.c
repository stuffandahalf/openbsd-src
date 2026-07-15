/*	$OpenBSD: if_rtw88x.c,v 1.0 2026/01/23 23:09:28 ganorton Exp $	*/

#include <sys/param.h>
#include <sys/systm.h> 
#include <sys/device.h>
#include <sys/rwlock.h>
 
#include <net/if.h>
#include <net/if_media.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <net80211/ieee80211_radiotap.h>
#include <net80211/ieee80211_var.h>

#include <machine/bus.h>
#include <machine/intr.h>

#include <dev/ic/rtw88xreg.h>
#include <dev/ic/rtw88xvar.h>

#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcidevs.h>

#include "if_rtw88x_pci.h"

struct rtw88x_pci_softc {
	struct rtw88x_softc				psc_sc;

	/* pci bus info */
	bus_space_tag_t					psc_st;
	bus_space_handle_t				psc_sh;
	bus_size_t						psc_sz;
	bus_dma_tag_t					psc_dmat;

	/* pci interrupt */
	struct rwlock					psc_hwirqlock;

	/* TX irq details */
	struct rwlock					psc_irqlock;
	uint32_t						psc_irqmask[4];

	/* RX/RX DMA ring buffers */
	//struct rtw88x_pci_rx_ring		psc_rxrings[RTK_MAX_RX_DESC_NUM];
	//struct rtw88x_pci_tx_ring		psc_txrings[RTK_MAX_TX_QUEUE_NUM];
};

void rtw88x_pci_attach(struct device *, struct device *, void *);
int rtw88x_pci_detach(struct device *, int);

static const struct pci_matchid rtw88x_devices[] = {
	{ PCI_VENDOR_REALTEK, PCI_PRODUCT_REALTEK_RTL8822BE }
};

int
rtw88x_pci_match(struct device *parent, void *match __unused, void *aux)
{
	struct pci_attach_args *pa = aux;
	return pci_matchbyid(pa, rtw88x_devices, nitems(rtw88x_devices));
}

const struct cfattach rtw88x_ca = {
	sizeof(struct rtw88x_pci_softc),
	rtw88x_pci_match,
	rtw88x_pci_attach,
#if 1
	NULL /* detach */,
#else
	rtw88x_detach,
#endif
	NULL /* activate */
};

void
rtw88x_pci_attach(struct device *parent, struct device *self, void *aux)
{
	int err;
	struct rtw88x_pci_softc *psc = (void *)self;
	struct rtw88x_softc *sc = &psc->psc_sc;
	struct pci_attach_args *pa = aux;
	pcireg_t /*reg,*/ memtype;

	switch (PCI_PRODUCT(pa->pa_id)) {
	//case PCI_PRODUCT_REALTEK_RTL8822B:
	case PCI_PRODUCT_REALTEK_RTL8822BE:
		sc->sc_chipinfo = &rtw8822b_hw_spec;
		break;
	default:
		return;
	}

	memtype = pci_mapreg_type(pa->pa_pc, pa->pa_tag, PCI_MAPREG_START);
	err = pci_mapreg_map(pa, PCI_MAPREG_START, memtype, 0, &psc->psc_st,
			&psc->psc_sh, NULL, &psc->psc_sz, 0);
	if (err) {
		printf("%s: can't map mem space\n", self->dv_xname);
		return;
	}

	psc->psc_dmat = pa->pa_dmat;
	psc->psc_irqmask[0] = IMR_HIGHDOK |
			      IMR_MGNTDOK |
			      IMR_BKDOK |
			      IMR_BEDOK |
			      IMR_VIDOK |
			      IMR_VODOK |
			      IMR_ROK |
			      IMR_BCNDMAINT_E |
			      IMR_C2HCMD;
	psc->psc_irqmask[1] = IMR_TXFOVW;
	psc->psc_irqmask[3] = IMR_H2CDOK;

	rw_init(&psc->psc_irqlock, "rtw88x irq");
	rw_init(&psc->psc_hwirqlock, "rtw88x hw irq");

	/* allocate dma rings for tx/rx */

	printf(": ATTACHED (%p, %lu, %lu)\n", psc->psc_st, psc->psc_sh, psc->psc_sz);
	//printf("%s: ATTACHED\n", DEVNAME(sc));
}

int
rtw88x_pci_detach(struct device *dev, int flags)
{
	printf("DETACHED WITH FLAGS 0x%X", flags);
	return 0;
}


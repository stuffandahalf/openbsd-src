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
	struct rtw88x_softc	sc_sc;

	/* pci bus info */
	bus_space_tag_t					sc_st;
	bus_space_handle_t				sc_sh;
	bus_size_t						sc_sz;
	bus_dma_tag_t					sc_dmat;

	/* pci interrupt */
	struct rwlock					sc_hwirqlock;

	/* TX irq details */
	struct rwlock					sc_irqlock;
	uint32_t						sc_irqmask[4];

	/* RX/RX DMA ring buffers */
	//struct rtw88x_pci_rx_ring		sc_rxrings[RTK_MAX_RX_DESC_NUM];
	//struct rtw88x_pci_tx_ring		sc_txrings[RTK_MAX_TX_QUEUE_NUM];
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
	struct rtw88x_pci_softc *sc = (void *)self;
	struct pci_attach_args *pa = aux;
	pcireg_t /*reg,*/ memtype;

	memtype = pci_mapreg_type(pa->pa_pc, pa->pa_tag, PCI_MAPREG_START);
	err = pci_mapreg_map(pa, PCI_MAPREG_START, memtype, 0, &sc->sc_st,
			&sc->sc_sh, NULL, &sc->sc_sz, 0);
	if (err) {
		printf("%s: can't map mem space\n", self->dv_xname);
		return;
	}

	sc->sc_dmat = pa->pa_dmat;
	sc->sc_irqmask[0] = IMR_HIGHDOK |
			      IMR_MGNTDOK |
			      IMR_BKDOK |
			      IMR_BEDOK |
			      IMR_VIDOK |
			      IMR_VODOK |
			      IMR_ROK |
			      IMR_BCNDMAINT_E |
			      IMR_C2HCMD;
	sc->sc_irqmask[1] = IMR_TXFOVW;
	sc->sc_irqmask[3] = IMR_H2CDOK;

	rw_init(&sc->sc_irqlock, "rtw88x irq");
	rw_init(&sc->sc_hwirqlock, "rtw88x hw irq");

	/* allocate dma rings for tx/rx */

	printf(": ATTACHED (%p, %lu, %lu)\n", sc->sc_st, sc->sc_sh, sc->sc_sz);
	//printf("%s: ATTACHED\n", DEVNAME(sc));
}

int
rtw88x_pci_detach(struct device *dev, int flags)
{
	printf("DETACHED WITH FLAGS 0x%X", flags);
	return 0;
}


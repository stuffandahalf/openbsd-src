/*	$OpenBSD: if_rtw88x.c,v 1.0 2026/01/23 23:09:28 ganorton Exp $	*/

#include <sys/device.h>
#include <sys/systm.h>
#include <sys/types.h>

#include <dev/pci/pcidevs.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

#if 0
struct ne_pci_softc {
	struct ne2000_softc sc_ne2000;		/* real "ne2000" softc */

	/* PCI-specific goo */
	void *sc_ih;				/* interrupt handle */
};
#endif

void rtw88x_attach(struct device *, struct device *, void *);
void rtw88x_detach(struct device *, int);

static const struct pci_matchid rtw88x_devices[] = {
	{ PCI_VENDOR_REALTEK, PCI_PRODUCT_REALTEK_RTL8822BE }
};

int
rtw88x_match(struct device *parent, iwx_match_t match __unused, void *aux)
{
	struct pci_attach_args *pa = aux;
	int r = pci_matchbyid(pa, iwx_devices, nitems(rtw88x_devices));

	if (r) {
		printf("Found rtw88x device\n");
	}
	return 0;
}
const struct cfattach rtw88x_ca = {
	/*sizeof(struct rtw88x_softc)*/0,
	rtw88x_match,
	rtw88x_attach,
	rtw88x_detach,
#if 0
	NULL /* detach */,
#endif
	NULL /* activate */
};

void
rtw88x_attach(struct device *dev, struct device *parent, void *softc)
{
	printf("ATTACHED");
}

void
rtw88x_detach(struct device *dev, int flags)
{
	printf("DETACHED WITH FLAGS 0x%X", flags);
}


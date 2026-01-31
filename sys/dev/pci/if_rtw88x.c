/*	$OpenBSD: if_rtw88x.c,v 1.0 2026/01/23 23:09:28 ganorton Exp $	*/

#include <sys/param.h>
#include <sys/device.h>
#include <sys/systm.h>

#include <dev/pci/pcidevs.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>

struct rtw88x_softc {
	struct device sc_dev;

	/*bus_space_tag_t sc_st;
	bus_space_handle_t sc_sh;
	bus_size_t sc_sz;*/
};

struct cfdriver rtw88x_cd = {
	NULL, "rtw88x", DV_IFNET
};

void rtw88x_attach(struct device *, struct device *, void *);
int rtw88x_detach(struct device *, int);

static const struct pci_matchid rtw88x_devices[] = {
	{ PCI_VENDOR_REALTEK, PCI_PRODUCT_REALTEK_RTL8822BE }
};

int
rtw88x_match(struct device *parent, void *match __unused, void *aux)
{
	struct pci_attach_args *pa = aux;
	return pci_matchbyid(pa, rtw88x_devices, nitems(rtw88x_devices));
}

const struct cfattach rtw88x_ca = {
	sizeof(struct rtw88x_softc),
	rtw88x_match,
	rtw88x_attach,
#if 1
	NULL /* detach */,
#else
	rtw88x_detach,
#endif
	NULL /* activate */
};

void
rtw88x_attach(struct device *parent, struct device *dev, void *aux)
{
	/*int err;
	struct pci_attach_args *pa = aux;
	pcireg_t reg, memtype;

	memtype = pci_mapreg_type(pa->pa_pc, pa->pa_tag, PCI_MAPREG_START);
	err = pci_mapreg_map(pa, PCI_MAPREG_START, memtype, 0,
	    &sc->sc_st, &sc->sc_sh, NULL, &sc->sc_sz, 0);
	if (err) {
		printf("%s: can't map mem space\n", DEVNAME(sc));
		return;
	}*/

	printf(": ATTACHED\n");
	//printf("%s: ATTACHED\n", DEVNAME(sc));
}

int
rtw88x_detach(struct device *dev, int flags)
{
	printf("DETACHED WITH FLAGS 0x%X", flags);
	return 0;
}


/*	$OpenBSD: smtsdhc.c,v 1.0 2026/10/20 00:00:00 ganorton Exp $	*/

/*
 * Driver for the SD/eMMC host controller on the SpacemiT K1 SoC.
 *
 * The controller is an SDHCI-standard host (SD Host Controller Simplified
 * Specification v3.00) with a vendor-specific PHY that must be explicitly
 * enabled.  This driver rides on the generic sdhc(4) host framework,
 * similar to cdsdhc(4) for the Cadence host, and adds the SpacemiT-specific
 * PHY initialization.
 *
 * References:
 *   - SpacemiT K1 User Manual, section 10.5 "SD/eMMC"
 *   - Linux drivers/mmc/host/sdhci-of-k1.c
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/device.h>

#include <machine/bus.h>
#include <machine/fdt.h>

#include <dev/ofw/fdt.h>
#include <dev/ofw/openfirm.h>
#include <dev/ofw/ofw_clock.h>
#include <dev/ofw/ofw_pinctrl.h>

#include <dev/sdmmc/sdhcreg.h>
#include <dev/sdmmc/sdhcvar.h>
#include <dev/sdmmc/sdmmcvar.h>
#include <dev/sdmmc/sdmmcchip.h>

/* SpacemiT vendor-specific registers (SDHCI vendor area). */
#define SMTSDHC_OP_EXT_REG		0x108
#define  SMTSDHC_OVRRD_CLK_OEN		(1U << 11)
#define  SMTSDHC_FORCE_CLK_ON		(1U << 12)

#define SMTSDHC_LEGACY_CTRL_REG		0x10c
#define  SMTSDHC_GEN_PAD_CLK_ON		(1U << 6)

#define SMTSDHC_MMC_CTRL_REG		0x114
#define  SMTSDHC_MISC_INT_EN		(1U << 1)
#define  SMTSDHC_MISC_INT		(1U << 2)
#define  SMTSDHC_ENHANCE_STROBE_EN	(1U << 8)
#define  SMTSDHC_MMC_HS400		(1U << 9)
#define  SMTSDHC_MMC_HS200		(1U << 10)
#define  SMTSDHC_MMC_CARD_MODE		(1U << 12)

#define SMTSDHC_PHY_CTRL_REG		0x160
#define  SMTSDHC_PHY_FUNC_EN		(1U << 0)
#define  SMTSDHC_PHY_PLL_LOCK		(1U << 1)

#define SMTSDHC_PHY_FUNC_REG		0x164
#define  SMTSDHC_HS200_USE_RFIFO	(1U << 15)

#define SMTSDHC_PHY_DLLCFG		0x168
#define  SMTSDHC_DLL_PREDLY_NUM		(0x3 << 2)
#define  SMTSDHC_DLL_FULLDLY_RANGE	(0x3 << 4)
#define  SMTSDHC_DLL_VREG_CTRL		(0x3 << 6)
#define  SMTSDHC_DLL_ENABLE		(1U << 31)

#define SMTSDHC_PHY_DLLCFG1		0x16c
#define  SMTSDHC_DLL_REG1_CTRL		(0xff << 0)

#define SMTSDHC_PHY_DLLSTS		0x170
#define  SMTSDHC_DLL_LOCK_STATE		(1U << 0)

#define SMTSDHC_PHY_PADCFG_REG		0x178
#define  SMTSDHC_PHY_DRIVE_SEL		(0x7 << 0)
#define  SMTSDHC_RX_BIAS_CTRL		(1U << 5)

struct smtsdhc_softc {
	struct sdhc_softc	sc_sdhc;
	bus_space_tag_t		sc_iot;
	bus_space_handle_t	sc_ioh;
	void			*sc_ih;

	struct sdhc_host	*sc_host;
};

#define HREAD4(sc, reg) \
	(bus_space_read_4((sc)->sc_iot, (sc)->sc_ioh, (reg)))
#define HWRITE4(sc, reg, val) \
	bus_space_write_4((sc)->sc_iot, (sc)->sc_ioh, (reg), (val))
#define HSET4(sc, reg, bits) \
	HWRITE4((sc), (reg), HREAD4((sc), (reg)) | (bits))
#define HCLR4(sc, reg, bits) \
	HWRITE4((sc), (reg), HREAD4((sc), (reg)) & ~(bits))

int	smtsdhc_match(struct device *, void *, void *);
void	smtsdhc_attach(struct device *, struct device *, void *);
void	smtsdhc_bus_clock_pre(struct sdhc_softc *, int, int);
void	smtsdhc_phy_init(struct smtsdhc_softc *);

const struct cfattach smtsdhc_ca = {
	sizeof(struct smtsdhc_softc), smtsdhc_match, smtsdhc_attach
};

struct cfdriver smtsdhc_cd = {
	NULL, "smtsdhc", DV_DULL
};

int
smtsdhc_match(struct device *parent, void *match, void *aux)
{
	struct fdt_attach_args *faa = aux;

	if (faa->fa_nreg < 1)
		return 0;
	return OF_is_compatible(faa->fa_node, "spacemit,k1-sdhci");
}

void
smtsdhc_attach(struct device *parent, struct device *self, void *aux)
{
	struct fdt_attach_args *faa = aux;
	struct smtsdhc_softc *sc = (struct smtsdhc_softc *)self;
	uint64_t capmask = 0, capset = 0;

	sc->sc_iot = faa->fa_iot;

	if (bus_space_map(sc->sc_iot, faa->fa_reg[0].addr, faa->fa_reg[0].size,
	    0, &sc->sc_ioh) != 0) {
		printf(": can't map registers\n");
		return;
	}

	pinctrl_byname(faa->fa_node, "default");

	clock_set_assigned(faa->fa_node);
	clock_enable_all(faa->fa_node);
	reset_deassert_all(faa->fa_node);

	sc->sc_ih = fdt_intr_establish(faa->fa_node, IPL_BIO,
	    sdhc_intr, sc, sc->sc_sdhc.sc_dev.dv_xname);
	if (sc->sc_ih == NULL) {
		printf(": can't establish interrupt\n");
		goto disable;
	}

	printf("\n");

	sc->sc_sdhc.sc_host = &sc->sc_host;
	sc->sc_sdhc.sc_dmat = faa->fa_dmat;
	sc->sc_sdhc.sc_bus_clock_pre = smtsdhc_bus_clock_pre;

	/*
	 * The controller only does 32-bit ADMA; the 64-bit DMA
	 * capability bit is broken on the K1.
	 */
	sc->sc_sdhc.sc_flags |= SDHC_F_32BIT_ACCESS;

	/*
	 * eMMC is non-removable and has no card-detect GPIO; treat it
	 * as always present.  An SD socket with a cd-gpios would use the
	 * generic GPIO card-detect path instead.
	 */
	if (OF_getproplen(faa->fa_node, "non-removable") >= 0 ||
	    OF_getproplen(faa->fa_node, "cd-gpios") < 0)
		sc->sc_sdhc.sc_flags |= SDHC_F_NONREMOVABLE;

	/*
	 * sdhc_host_found() resets the host controller (SDHC_RESET_ALL),
	 * which clears the vendor PHY configuration; smtsdhc_bus_clock_pre()
	 * re-applies it on the first bus clock change.
	 */
	sdhc_host_found(&sc->sc_sdhc, sc->sc_iot, sc->sc_ioh,
	    faa->fa_reg[0].size, 1, capmask, capset);
	return;

disable:
	clock_disable_all(faa->fa_node);
	bus_space_unmap(sc->sc_iot, sc->sc_ioh, faa->fa_reg[0].size);
}

void
smtsdhc_phy_init(struct smtsdhc_softc *sc)
{
	uint32_t val;
	int timo;

	/* Enable the PHY and lock its PLL. */
	HSET4(sc, SMTSDHC_PHY_CTRL_REG, SMTSDHC_PHY_FUNC_EN |
	    SMTSDHC_PHY_PLL_LOCK);

	/* Configure pad drive strength and receiver bias. */
	val = HREAD4(sc, SMTSDHC_PHY_PADCFG_REG);
	val &= ~SMTSDHC_PHY_DRIVE_SEL;
	val |= SMTSDHC_RX_BIAS_CTRL | 0x4;
	HWRITE4(sc, SMTSDHC_PHY_PADCFG_REG, val);

	for (timo = 1000; timo > 0; timo--) {
		if (HREAD4(sc, SMTSDHC_PHY_CTRL_REG) & SMTSDHC_PHY_PLL_LOCK)
			break;
		sdmmc_delay(10);
	}
	if (timo == 0)
		printf("%s: PHY PLL lock timeout\n",
		    sc->sc_sdhc.sc_dev.dv_xname);

	/*
	 * Configure the delay-line DLL (used for HS200/HS400) and
	 * wait for it to lock.
	 */
	val = HREAD4(sc, SMTSDHC_PHY_DLLCFG);
	val &= ~(SMTSDHC_DLL_PREDLY_NUM | SMTSDHC_DLL_FULLDLY_RANGE |
	    SMTSDHC_DLL_VREG_CTRL);
	val |= (1U << 2) | (1U << 4) | (1U << 6);
	HWRITE4(sc, SMTSDHC_PHY_DLLCFG, val);

	val = HREAD4(sc, SMTSDHC_PHY_DLLCFG1);
	val &= ~SMTSDHC_DLL_REG1_CTRL;
	val |= 0x92;
	HWRITE4(sc, SMTSDHC_PHY_DLLCFG1, val);

	val = HREAD4(sc, SMTSDHC_PHY_DLLCFG);
	val |= SMTSDHC_DLL_ENABLE;
	HWRITE4(sc, SMTSDHC_PHY_DLLCFG, val);

	for (timo = 1000; timo > 0; timo--) {
		if (HREAD4(sc, SMTSDHC_PHY_DLLSTS) & SMTSDHC_DLL_LOCK_STATE)
			break;
		sdmmc_delay(10);
	}
	if (timo == 0)
		printf("%s: PHY DLL lock timeout\n",
		    sc->sc_sdhc.sc_dev.dv_xname);
}

void
smtsdhc_bus_clock_pre(struct sdhc_softc *sc_sdhc, int freq, int timing)
{
	struct smtsdhc_softc *sc = (struct smtsdhc_softc *)sc_sdhc;
	uint32_t val;

	/* Re-apply vendor PHY config after a controller reset. */
	smtsdhc_phy_init(sc);

	/*
	 * Select the eMMC bus mode.  The generic sdhc(4) driver does not
	 * advertise HS200/HS400 (no tuning support), so this is mostly
	 * cosmetic today, but keeps the vendor register consistent.
	 */
	val = HREAD4(sc, SMTSDHC_MMC_CTRL_REG);
	val &= ~(SMTSDHC_MMC_HS200 | SMTSDHC_MMC_HS400);
	if (ISSET(sc->sc_sdhc.sc_flags, SDHC_F_NONREMOVABLE))
		val |= SMTSDHC_MMC_CARD_MODE;
	switch (timing) {
	case SDMMC_TIMING_MMC_HS200:
		val |= SMTSDHC_MMC_HS200;
		break;
	}
	HWRITE4(sc, SMTSDHC_MMC_CTRL_REG, val);

	/*
	 * For SD (non-eMMC) the controller needs the clock output forced
	 * on; harmless for eMMC.
	 */
	if (freq != SDMMC_SDCLK_OFF) {
		HSET4(sc, SMTSDHC_OP_EXT_REG, SMTSDHC_OVRRD_CLK_OEN |
		    SMTSDHC_FORCE_CLK_ON);
		HSET4(sc, SMTSDHC_LEGACY_CTRL_REG, SMTSDHC_GEN_PAD_CLK_ON);
	}
}

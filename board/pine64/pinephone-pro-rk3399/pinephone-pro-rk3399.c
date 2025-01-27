// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2019 Vasily Khoruzhick <anarsoul@gmail.com>
 * (C) Copyright 2021 Dragan Simic <dsimic@buserror.io>
 */

/*
 * TODO: Disable debugging
 */
#define DEBUG

#include <common.h>
#include <dm.h>
#include <init.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch-rockchip/grf_rk3399.h>
#include <asm/arch-rockchip/hardware.h>
#include <asm/arch-rockchip/misc.h>
#include <power/regulator.h>
#include <power/rk8xx_pmic.h>

#define GRF_IO_VSEL_BT565_SHIFT 0
#define PMUGRF_CON0_VSEL_SHIFT 8

#ifdef CONFIG_MISC_INIT_R
static void setup_iodomain(void)
{
	struct rk3399_grf_regs *grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	struct rk3399_pmugrf_regs *pmugrf = syscon_get_first_range(ROCKCHIP_SYSCON_PMUGRF);

	/* BT565 is in 1.8 V domain */
	rk_setreg(&grf->io_vsel, 1 << GRF_IO_VSEL_BT565_SHIFT);

	/* Set GPIO1 1.8/3.0 V source select to PMU1830_VOL */
	rk_setreg(&pmugrf->soc_con0, 1 << PMUGRF_CON0_VSEL_SHIFT);
}

int misc_init_r(void)
{
	const u32 cpuid_offset = 0x7;
	const u32 cpuid_length = 0x10;
	u8 cpuid[cpuid_length];
	int ret;

	setup_iodomain();

	ret = rockchip_cpuid_from_efuse(cpuid_offset, cpuid_length, cpuid);
	if (ret)
		return ret;

	ret = rockchip_cpuid_set(cpuid, cpuid_length);
	if (ret)
		return ret;

	ret = rockchip_setup_macaddr();

	return ret;
}
#endif /* CONFIG_MISC_INIT_R */

/*
 * TODO: Change CONFIG_SPL_POWER_SUPPORT to CONFIG_SPL_POWER, to match newer U-Boot versions.
 *       The same applies to CONFIG_SPL_I2C_SUPPORT.
 */

#ifdef CONFIG_SPL_BUILD
#if !defined(CONFIG_SPL_OF_PLATDATA)
static int setup_usb_power(void)
{
	struct udevice *pmic;
	int ret;

	ret = uclass_first_device_err(UCLASS_PMIC, &pmic);
	if (ret)
		return ret;

#if defined(CONFIG_SPL_POWER)
	/* set USB current limit to 2.5 A */
	ret = rk818_spl_configure_usb_input_current(pmic, 2500);
	if (ret)
		return ret;

	/* set USB low voltage threshold to 3.26 V */
	ret = rk818_spl_configure_usb_chrg_shutdown(pmic, 3260000);
	if (ret)
		return ret;
#endif /* CONFIG_SPL_POWER */

	return 0;
}
#endif /* !CONFIG_SPL_OF_PLATDATA */

void power_setup(void)
{
	int ret;

	ret = setup_usb_power();
	if (ret)
		debug("Failed to configure USB power settings: %d\n", ret);
}
#endif /* CONFIG_SPL_BUILD */

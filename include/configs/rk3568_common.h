/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd
 */

#ifndef __CONFIG_RK3568_COMMON_H
#define __CONFIG_RK3568_COMMON_H

#include "rockchip-common.h"

#define CONFIG_SYS_CBSIZE		1024

#define COUNTER_FREQUENCY               24000000
#define CONFIG_ROCKCHIP_STIMER_BASE	0xfdd1c020

#define CONFIG_IRAM_BASE		0xfdcc0000

#define CONFIG_SYS_INIT_SP_ADDR		0x00c00000

#define CONFIG_SPL_STACK		0x00400000
#define CONFIG_SPL_MAX_SIZE		0x20000
#define CONFIG_SPL_BSS_START_ADDR	0x4000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x4000

#define CONFIG_SYS_BOOTM_LEN		(64 << 20)	/* 64M */

#define CONFIG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0xf0000000

#ifdef CONFIG_OF_SYSTEM_SETUP
#define SDRAM_LOWER_ADDR_MAX		0xf0000000
#define SDRAM_UPPER_ADDR_MIN		0x100000000
#endif

#ifndef CONFIG_SPL_BUILD
#define ENV_MEM_LAYOUT_SETTINGS		\
	"scriptaddr=0x00c00000\0"	\
	"pxefile_addr_r=0x00e00000\0"	\
	"fdt_addr_r=0x0a100000\0"	\
	"kernel_addr_r=0x02080000\0"	\
	"ramdisk_addr_r=0x0a200000\0" \
	"kernel_comp_addr_r=0x0f000000\0" \
	"kernel_comp_size=0x2000000\0"

#include <config_distro_bootcmd.h>
#define CONFIG_EXTRA_ENV_SETTINGS		\
	ENV_MEM_LAYOUT_SETTINGS			\
	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"partitions=" PARTS_DEFAULT		\
	ROCKCHIP_DEVICE_SETTINGS		\
	BOOTENV
#endif

#endif

#define UPGRADE_IS_BOOT 0
#ifndef BOARD_OSC_HZ
#define BOARD_OSC_HZ 33300000UL
#endif
#ifndef UPGRADE_CONFIG_H
#define UPGRADE_CONFIG_H
#ifndef BOARD_OSC_HZ
#define BOARD_OSC_HZ 30000000UL
#endif
#if BOARD_OSC_HZ == 30000000UL
#if UPGRADE_IS_BOOT
#define UPGRADE_PLL 4
#define UPGRADE_DIVSEL 2
#define UPGRADE_CPU_HZ 60000000UL
#else
#define UPGRADE_PLL 10
#define UPGRADE_DIVSEL 2
#define UPGRADE_CPU_HZ 150000000UL
#endif
#elif BOARD_OSC_HZ == 33300000UL
#if UPGRADE_IS_BOOT
#define UPGRADE_PLL 7
#define UPGRADE_DIVSEL 1
#define UPGRADE_CPU_HZ 58275000UL
#else
#define UPGRADE_PLL 9
#define UPGRADE_DIVSEL 2
#define UPGRADE_CPU_HZ 149850000UL
#endif
#else
#error Supported crystal frequencies are 30000000 and 33300000 Hz
#endif
#if UPGRADE_IS_BOOT
#define UPGRADE_LSP_DIV 1
#define UPGRADE_FLASH_WAIT 3
#define UPGRADE_OTP_WAIT 5
#else
#define UPGRADE_LSP_DIV 2
#define UPGRADE_FLASH_WAIT 5
#define UPGRADE_OTP_WAIT 8
#endif
#define UPGRADE_LSP_HZ (UPGRADE_CPU_HZ / (2UL * UPGRADE_LSP_DIV))
#define UPGRADE_BRR ((UPGRADE_LSP_HZ + 460800UL) / 921600UL - 1UL)
#define DSP_CLOCK (UPGRADE_CPU_HZ / 1000000UL)
#include "../../../UpgradeLayout.h"
#define UPGRADE_BOOT_VERSION 201UL
#endif

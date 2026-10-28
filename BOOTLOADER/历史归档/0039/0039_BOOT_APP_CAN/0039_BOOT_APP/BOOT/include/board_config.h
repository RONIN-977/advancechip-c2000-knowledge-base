#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H
#include "driverlib.h"
/* Project pin selection; TI mux constants are in vendor/device/driverlib/pin_map.h.
 * CCS ProjectSpec copies this file to include/board_config.h in BOTH projects.
 * boot_ports_init() uses BOOT_* macros, not vendor/device/device.h board defaults.
 */
#define BOOT_SCI_BASE SCIA_BASE
#define BOOT_SCI_RX_GPIO 28U
#define BOOT_SCI_TX_GPIO 29U
#define BOOT_SCI_RX_CONFIG GPIO_28_SCIA_RX
#define BOOT_SCI_TX_CONFIG GPIO_29_SCIA_TX
#define BOOT_CAN_BASE CANA_BASE
#define BOOT_CAN_RX_GPIO 30U
#define BOOT_CAN_TX_GPIO 31U
#define BOOT_CAN_RX_CONFIG GPIO_30_CANA_RX
#define BOOT_CAN_TX_CONFIG GPIO_31_CANA_TX
/* Application LED: one transition every 500 ms; one full cycle per second.
 * Initialized only by the application, not by the bootloader.
 */
#define APP_LED_GPIO 17U
#define APP_LED_CONFIG GPIO_17_GPIO17
#define APP_LED_TOGGLE_MS 500UL
#ifndef BOOT_SCI_BAUD
#define BOOT_SCI_BAUD 115200UL
#endif
#ifndef BOOT_CAN_BAUD
#define BOOT_CAN_BAUD 500000UL
#endif
#define BOOT_NODE_ID 1U
#define BOOT_CAN_RX_ID (0x600U + BOOT_NODE_ID)
#define BOOT_CAN_TX_ID (0x580U + BOOT_NODE_ID)
#define BOOT_LISTEN_MS 5000UL
#define BOOT_SESSION_MS 30000UL
#define BOOT_PARTIAL_MS 200UL
#define BOOT_METADATA 0x08F000UL
/* Optional board-level force-boot input disabled until a pin is allocated. */
#define BOOT_FORCE_GPIO 0xFFFFU
/* Clock source is configured in vendor/device/device.h: 20 MHz XTAL, 120 MHz SYSCLK. */
#endif

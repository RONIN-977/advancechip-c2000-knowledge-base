#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H
#include "driverlib.h"
/* [移植必需 客户适配] 本文件随升级模块复制。
 * 改 CAN 控制器、引脚、速率或节点时，配套 BOOT 必须同步；
 * 客户已有业务 ID 时，确认 0x600+节点/0x580+节点没有冲突。
 * GPIO17 LED 宏为旧示例参数，当前 main 模板不使用。
 */
/* Project pin selection; TI mux constants are in vendor/device/driverlib/pin_map.h.
 * CCS ProjectSpec copies this file to include/board_config.h in BOTH projects.
 * APP uses customer CAN initialization; BOOT pin/baud must be adapted separately.
 */
#define BOOT_CAN_BASE CANA_BASE
#define BOOT_CAN_RX_GPIO 12U
#define BOOT_CAN_TX_GPIO 13U
#define BOOT_CAN_RX_CONFIG GPIO_12_CANA_RX
#define BOOT_CAN_TX_CONFIG GPIO_13_CANA_TX
/* Application LED: one transition every 500 ms; one full cycle per second.
 * Legacy demo parameters; unused by the customer integration main template.
 */
#define APP_LED_GPIO 17U
#define APP_LED_CONFIG GPIO_17_GPIO17
#define APP_LED_TOGGLE_MS 500UL
#ifndef BOOT_CAN_BAUD
/* [K01 待确认] 客户注释为 250k，实际速率需核对 CAN 输入时钟。
 * 客户参数 (40,5,4) 在 100MHz 下为 250k，在 120MHz 下为 300k。
 * 本宏只记录目标速率，APP 不再用它重新配置 CAN；BOOT/上位机须同步。
 */
#define BOOT_CAN_BAUD 250000UL
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
/* [K02 必填] 客户 CAN_Config_Init 源码尚未提供。
 * 填入两个不同且未使用的消息对象号 1..32；0 只是待填写标记。
 * 接收、发送和 TX 完成掩码都统一使用以下宏。
 */
#define BOOT_CAN_RX_OBJ 0U
#define BOOT_CAN_TX_OBJ 0U
#if BOOT_CAN_RX_OBJ < 1U || BOOT_CAN_RX_OBJ > 32U || \
    BOOT_CAN_TX_OBJ < 1U || BOOT_CAN_TX_OBJ > 32U || \
    BOOT_CAN_RX_OBJ == BOOT_CAN_TX_OBJ
#error "K02: choose two unused CAN message objects in board_config.h"
#endif
/* [K03 待确认] 时钟由客户系统初始化负责。
 * device.h 中 DEVICE_SYSCLK_FREQ/延时常量须与客户真实时钟一致。
 */
#endif

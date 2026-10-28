#ifndef BOOT_IO_H
#define BOOT_IO_H
#include "boot_core.h"
/* [移植必需] 保留时基、CAN 完整应答、RAM 请求标记及复位接口。
 * transport 固定传 2U。boot_take_request 供 BOOT 消费标记，客户 APP 不调用。
 */
/* [K05] 独立 Timer2 时基，由 upgrade_entry_init 调用一次。 */
void boot_timer2_init(void);
uint32_t boot_millis(void);
/* [K07] 客户 CAN 初始化后调用一次，只配置升级消息对象。 */
void boot_can_objects_init(void);
uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count);
/* Reserve M0 word addresses 0x3F8..0x3FB in the application's linker file.
 * Watchdog reset keeps this cookie; power-cycle is not required. */
#define BOOT_REQUEST_ADDR 0x0003F8UL
uint16_t boot_take_request(void);
void boot_request_reset(void);
#endif

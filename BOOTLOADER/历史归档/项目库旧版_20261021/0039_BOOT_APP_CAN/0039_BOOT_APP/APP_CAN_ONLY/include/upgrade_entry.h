#ifndef UPGRADE_ENTRY_H
#define UPGRADE_ENTRY_H
/* Call init once after Device_init. Call poll frequently (<1 ms preferred).
 * In an existing application, adapt this to that application's own CAN
 * receive dispatcher instead of configuring the peripheral a second time.
 * Valid ENTER_BOOT is acknowledged, then resets into BOOT via the original flow. */
/* [K08] 客户设备初始化、boot_can_objects_init 后调用一次；初始化 Timer2 和协议状态，只调用一次。 */
void upgrade_entry_init(void);
/* [移植必需] 后台主循环频繁调用，优选间隔 <1 ms；不在控制 ISR 中调用。 */
void upgrade_entry_poll(void);
#endif

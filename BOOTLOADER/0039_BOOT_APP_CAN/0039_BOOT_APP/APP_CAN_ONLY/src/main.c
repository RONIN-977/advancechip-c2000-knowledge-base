/**
 * @file main.c
 * @brief [K04 已改] 客户集成调用模板，不再初始化示例 LED/心跳。
 * 将下面调用合并到客户原 main；无需移植整份 main.c。
 * Customer_* 接口尚需客户接线，本次为初步修改，不作为可烧录版本。
 */
#include "customer_upgrade_port.h"
#include "boot_io.h"
#include "upgrade_entry.h"

void main(void)
{
    /* [K04] 使用客户真实系统初始化：包含 PLL、InitPeriherals 等。
     * 此包装接口尚未实现；整合到客户 main 时沿用原初始化即可。
     */
    Customer_SystemInit();
    /* [K07] 必须在客户 CAN_Config_Init 后，且避免后续再次复位 CAN。 */
    boot_can_objects_init();
    /* [K05] 内部初始化 Timer2 为 1MHz；调用一次，客户不得复用 Timer2。 */
    upgrade_entry_init();

    for (;;)
    {
        Customer_BackgroundTask();
        upgrade_entry_poll();
    }
}

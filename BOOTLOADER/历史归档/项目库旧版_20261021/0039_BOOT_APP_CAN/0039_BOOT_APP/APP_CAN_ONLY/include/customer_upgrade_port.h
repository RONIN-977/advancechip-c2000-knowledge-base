#ifndef CUSTOMER_UPGRADE_PORT_H
#define CUSTOMER_UPGRADE_PORT_H
#include <stdint.h>
/* [K04 客户实现] 完整系统初始化：时钟、GPIO、CAN、定时器、中断。
 * 可在客户自己的 main 中沿用原初始化，不必实现这个模板包装接口。
 */
void Customer_SystemInit(void);
/* [K04 客户实现] 原有后台任务；不能长期阻塞升级收帧。 */
void Customer_BackgroundTask(void);
/* 升级计时使用独立 Timer2，不再需要客户毫秒接口或停机回调。 */
#endif

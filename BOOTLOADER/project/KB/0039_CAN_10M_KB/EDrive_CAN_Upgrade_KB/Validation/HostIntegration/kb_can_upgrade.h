#ifndef KB_CAN_UPGRADE_H
#define KB_CAN_UPGRADE_H
/* 后台唯一调用点：客户外设初始化结束后，在 while(1) 中频繁调用。 */
void KB_CAN_UpgradeTask(void);
/* 客户实现：完成停机/禁止重新启动后返回；默认实现为空。 */
void Customer_ShutdownForUpgrade(void);
/* 0=未初始化，1=就绪，2=应答发送中，3=邮箱已被占用（未覆盖客户配置）。 */
extern volatile unsigned int kb_upgrade_status;
#endif

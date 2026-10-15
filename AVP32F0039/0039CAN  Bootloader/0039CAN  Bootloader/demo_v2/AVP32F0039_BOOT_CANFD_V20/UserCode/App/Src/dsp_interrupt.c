/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_interrupt.c
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "dsp_interrupt.h"
#include "main.h"
#include "can_config.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
void (*run)(void);//PC跳转函数

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/
MCAN_RxBufElement canfdisr_rxMsg1;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern void Can_receive_write(uint32_t frame_id, uint16_t len, uint16_t *buf);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
__interrupt void WatchDog_isr(void)
{
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
__interrupt void CPU_Timer0_ISR(void)
{
    TICK0_SET();
    SYSTICK_SET();

	//清除定时器0中断标志位
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
__interrupt void CanFD_ISR(void)
{
    uint32_t intrStatus;
    uint16_t msg_len = 0;
    MCAN_RxNewDataStatus newData;

    intrStatus = MCAN_getIntrStatus(MCANA_DRIVER_BASE);
    MCAN_clearIntrStatus(MCANA_DRIVER_BASE, intrStatus);
    MCAN_clearInterrupt(MCANA_DRIVER_BASE, 0x2);

    if((MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG & intrStatus) == MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG)
    {
        MCAN_getNewDataStatus(MCANA_DRIVER_BASE, &newData);

        //  If message is received in buffer element 0
        if((newData.statusLow & (1UL << 0U)) != 0)
        {
            MCAN_readMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_BUF, 0U, 0, &canfdisr_rxMsg1);
            if (canfdisr_rxMsg1.dlc <= 8)
            {
                msg_len = 8;
            }
            else if (canfdisr_rxMsg1.dlc == 9)
            {
                msg_len = 12;
            }
            else if (canfdisr_rxMsg1.dlc == 10)
            {
                msg_len = 16;
            }
            else if (canfdisr_rxMsg1.dlc == 11)
            {
                msg_len = 20;
            }
            else if (canfdisr_rxMsg1.dlc == 12)
            {
                msg_len = 24;
            }
            else if (canfdisr_rxMsg1.dlc == 13)
            {
                msg_len = 32;
            }
            else if (canfdisr_rxMsg1.dlc == 14)
            {
                msg_len = 48;
            }
            else if (canfdisr_rxMsg1.dlc == 15)
            {
                msg_len = 64;
            }

            //把canfdisr_rxMsg1消息结构体读取的数据转移到应用层
            Can_receive_write(canfdisr_rxMsg1.id, msg_len, (uint16_t*)canfdisr_rxMsg1.data);
        }

        MCAN_clearNewDataStatus(MCANA_DRIVER_BASE, &newData);
    }

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}





























/* End of file */

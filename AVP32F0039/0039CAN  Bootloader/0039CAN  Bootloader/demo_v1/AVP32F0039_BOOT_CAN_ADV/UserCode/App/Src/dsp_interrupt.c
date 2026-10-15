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

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern void Can_up_receive_write(uint32_t frame_id, uint16_t frame_type, uint16_t *buf);

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
__interrupt void Can_ISR(void)
{
    uint32_t status;
    uint16_t rxMsgData[8]={0};
    status = CAN_getInterruptCause(CANA_BASE);
    if(status == CAN_INT_INT0ID_STATUS)
    {
        status = CAN_getStatus(CANA_BASE);
        if(((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_MSK) &&
           ((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_NONE))
        {
            ;
        }
    }

    status = CAN_readMessage(CANA_BASE, RX_MSG_OBJ_ID, (uint16_t*)&rxMsgData);
    if(status == true)
    {
        Can_up_receive_write(0x7A0, 0, (uint16_t*)&rxMsgData);
    }
    CAN_clearInterruptStatus(CANA_BASE, RX_MSG_OBJ_ID);

    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}





























/* End of file */

/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :main.c
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
#include "main.h"
#include "dsp_base.h"
#include "watchdog_config.h"
#include "timer_config.h"
#include "gpio_config.h"
#include "can_config.h"
#include "sci_config.h"
#include "dsp_program.h"
#include "CAN_upgrade_protocol.h"
#include "uart_transfer_protocol.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/
TASK_T Task;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:主函数入口
*Input:
*Output:
-----------------------------------------------------------------------------*/
void main(void)
{
	//DSP系统时钟初始化
	DSP_Base_Init();

	//外设初始化
	Timer0_Init();
    GPIO_Init();
//    CAN_Init();
    SCI_A_Init();//物理层使用串口传输，数据层使用CAN的升级协议
	
	//应用初始化
	Flash_API_Init();
    Task_Init();
    Protocol_Init();
    Uart_Transfer_Init();
    
	//开启总中断
	DSP_INT_Enable();
	while(1)
	{
		//1ms任务
		if(CHECK_TASK_0())
        {
            TICK0_CLEAR();
            TICK12_SET();
            Uart_Receive_Data();
            Receive_Data_Frame();
            Uart_run();
        }
		
		//10ms任务
		if(CHECK_TASK_1())
        {
            TICK1_CLEAR();
            DSP_JumpToBoot_delay();
            LED_Task_Run();
        }
		
		//100ms任务
		if(CHECK_TASK_2())
        {
            TICK2_CLEAR();
        }
	}
}





























/* End of file */

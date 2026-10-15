/******************************************************************************
*Copyright(C),2025,Hunan Advancechip Technology CO.,LTD.
*FileName   :dsp_base.c
*Author     :
*Version    :v1.0
*Date       :2025.7.2
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "gpio_config.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    LED1_ON_TIME     (10) //10 * 10ms = 100ms
#define    LED1_OFF_TIME    (10) //10 * 10ms = 100ms
#define    LED1_PERIOD      (LED1_ON_TIME + LED1_OFF_TIME)

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/
uint16_t led_tick;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
void GPIO_Init(void)
{
    EALLOW;

    LED1_SET_MUX();
    LED1_SET_DIR();
    LED1_SET_HIGH();

    GPIO_setPinConfig(GPIO_20_SPIB_SIMO);


    GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_OUT);
    GPIO_togglePin(20);

    GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_OUT);
    GPIO_togglePin(21);

    GPIO_setPadConfig(22, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);
    GPIO_togglePin(22);

    GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_OUT);
    GPIO_togglePin(23);


    EDIS;

    led_tick = 0;
}

/******************************************************************************
*Description:LED闪烁函数放在10ms任务运行
-----------------------------------------------------------------------------*/
void LED_Task_Run(void)
{
//    ++led_tick >= LED1_ON_TIME ? ((led_tick >= LED1_PERIOD) ? (led_tick = 0) : (LED1_SET_HIGH())) : (LED1_SET_LOW());

    GPIO_togglePin(20);
    GPIO_togglePin(21);
    GPIO_togglePin(22);
    GPIO_togglePin(23);
}
















/* End of file */

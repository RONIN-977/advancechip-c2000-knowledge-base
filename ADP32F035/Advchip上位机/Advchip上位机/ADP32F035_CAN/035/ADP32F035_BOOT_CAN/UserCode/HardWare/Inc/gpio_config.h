/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :gpio_config.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "ADP32F03x_Project.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    GPIO_IO_MODE     0

#define    GPIO_PULL_UP     0
#define    GPIO_PULL_NON    1

#define    GPIO_INPUT       0
#define    GPIO_OUTPUT      1

#define    LED1_SET_MUX    GpioCtrlRegs.GPAMUX1.bit.GPIO13 // 0是GPIO
#define    LED1_SET_PUD    GpioCtrlRegs.GPAPUD.bit.GPIO13 // 0上拉，1禁止上拉
#define    LED1_SET_DIR    GpioCtrlRegs.GPADIR.bit.GPIO13 // 0输入，1输出
#define    LED1_SET_LOW()    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1
#define    LED1_SET_HIGH()   GpioDataRegs.GPASET.bit.GPIO13 = 1
#define    LED1_SET_TOGG()   GpioDataRegs.GPATOGGLE.bit.GPIO13 = 1

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void GPIO_Init(void);
void LED_Task_Run(void);


















#endif

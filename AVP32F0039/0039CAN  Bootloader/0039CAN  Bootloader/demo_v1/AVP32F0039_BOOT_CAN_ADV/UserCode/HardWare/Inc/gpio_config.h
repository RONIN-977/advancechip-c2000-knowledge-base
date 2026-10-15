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
#include "device.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    LEVEL_LOW      0
#define    LEVEL_HIGH     1

#define    LED1_IO        20U

#define    LED1_SET_MUX()    GPIO_setPadConfig(LED1_IO, GPIO_PIN_TYPE_STD)
#define    LED1_SET_DIR()    GPIO_setDirectionMode(LED1_IO, GPIO_DIR_MODE_OUT)
#define    LED1_SET_LOW()    GPIO_writePin(LED1_IO, LEVEL_LOW)
#define    LED1_SET_HIGH()   GPIO_writePin(LED1_IO, LEVEL_HIGH)
#define    LED1_SET_TOGG()   GPIO_togglePin(LED1_IO)

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

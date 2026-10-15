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
#include "sci_config.h"

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

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern __interrupt void SCIA_ISR(void);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
void SCI_A_Init(void)
{
    GPIO_setControllerCore(28U, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_28_SCIA_RX);
    GPIO_setDirectionMode(28U, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(28U , GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(28U , GPIO_QUAL_ASYNC);

    GPIO_setControllerCore(29U, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_29_SCIA_TX);
    GPIO_setDirectionMode(29U, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(29U, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(29U, GPIO_QUAL_ASYNC);

    Interrupt_register(INT_SCIA_RX, SCIA_ISR);

    SCI_performSoftwareReset(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 115200, (SCI_CONFIG_WLEN_8 |
                                                 SCI_CONFIG_STOP_ONE |
                                                 SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

    SCI_setFIFOInterruptLevel(SCIA_BASE, SCI_FIFO_TX0, SCI_FIFO_RX1);
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_RXFF);

    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    Interrupt_enable(INT_SCIA_RX);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

}

/******************************************************************************
*Description:调用一次发送1个字节
-----------------------------------------------------------------------------*/
void SCI_A_Send_Byte(uint16_t data)
{
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)&data, 1);
}














/* End of file */

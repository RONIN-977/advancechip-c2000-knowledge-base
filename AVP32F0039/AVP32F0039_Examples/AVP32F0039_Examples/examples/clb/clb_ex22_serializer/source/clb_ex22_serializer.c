/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex22_serializer.c
*                         TITLE: CLB Serializer Example
*******************************************************************************************/



//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"


__interrupt void clb1ISR(void);

//
// Shift register GP REG bits
//
#define GPREG_0_EVENT_DATA_SHIFT  0U
#define GPREG_1_MODE0_EN_SHIFT    1U

//
// Main
//
void main(void)
{
    uint32_t counterValue = 0;

    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_CLB1, &clb1ISR);
    Interrupt_enable(INT_CLB1);


    Board_init();

    //
    // Serializer mode
    // Match1 EQUAL will generate interrupt at 0b110101
    // Match2 TAP Select bit enable
    // Match 2 Generate interrupt on the rising edge of BIT6
    // MODE0 and EVENT controlled through GPREG
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    counterValue = CLB_getRegister(myCLBForTILE1_BASE, CLB_REG_CTR_C0);
    if (counterValue != 0)
    {
        //
        // Error
        //
        ESTOP0;
    }

    //
    // Now we will shift in new values
    //

    //
    // Write 1 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    //
    // The value is now 0b1
    //

    //
    // Write 1 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    //
    // The value is now 0b11
    //

    //
    // Write 0 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (0U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (0U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    //
    // The value is now 0b110
    //

    //
    // Write 1 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    //
    // The value is now 0b1101
    //

    //
    // Write 0 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (0U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (0U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    //
    // The value is now 0b11010
    //

    counterValue = CLB_getRegister(myCLBForTILE1_BASE, CLB_REG_CTR_C0);
    if (counterValue != 0b11010)
    {
        //
        // Error
        //
        ESTOP0;
    }

    //
    // Shift successful!
    // The value is
    //
    ESTOP0;

    //
    // Match 1 is set to 0b110101, should get an interrupt
    //

    //
    // Write 1 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    DEVICE_DELAY_US(1000);
    ESTOP0;

    //
    // Match 2 TAP SELECT for bit 6, 0b"1"101011 should get an interrupt
    //

    //
    // Write 1 to DATA
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (0U << GPREG_1_MODE0_EN_SHIFT));
    //
    // Write 1 to EN
    //
    CLB_setGPREG(myCLBForTILE1_BASE, (1U << GPREG_0_EVENT_DATA_SHIFT) | (1U << GPREG_1_MODE0_EN_SHIFT));

    DEVICE_DELAY_US(1000);
    ESTOP0;

    while(1)
    {

    }
}


__interrupt void clb1ISR(void)
{
    uint32_t intTag = CLB_getInterruptTag(myCLBForTILE1_BASE);

    if (intTag == 1)
    {
        //
        // Match 1 value EQUAL
        //
        ESTOP0;
    }
    else if (intTag == 2)
    {
        //
        // Match 2 TAP Select Bit Set
        //
        ESTOP0;
    }

    CLB_clearInterruptTag(CLB1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}


//
// End of File
//

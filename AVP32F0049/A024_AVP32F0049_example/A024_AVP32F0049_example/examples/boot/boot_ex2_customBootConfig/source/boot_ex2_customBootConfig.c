/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   boot_ex2_customBootConfig.c
*                         TITLE:  Customized Boot Configuration Example
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "boot_ex2_customBootConfig.h"

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    #if STANDALONE_BOOT

        //
        // Enable writing to the EALLOW protected registers
        //
        EALLOW;

        //
        // Write a value of 0xA5 to the emulation equivalent
        // of Z1_OTP_BOOTPIN_CONFIG in order to emulate
        // the standalone boot flow
        //
        HWREG(EMU_BOOTPIN_CONFIG) = (EMU_STANDALONE_KEYVAL  << 24);

        //
        // Disable writing to the EALLOW protected registers
        //
        EDIS;

    #else

        //
        // If the ZERO_BMSPS build configuration is selected, set up a custom
        // boot table that uses 0 BMSPs
        //
        #ifdef ZERO_BMSPS

            configBMSPS0();

        #endif

        //
        // If the ONE_BMSP build configuration is selected, set up a custom
        // boot table that uses 1 BMSP
        //
        #ifdef ONE_BMSP

            configBMSPS1();

        #endif

        //
        // If the THREE_BMSPS build configuration is selected, set up a custom
        // boot table that uses 3 BMSPs
        //
        #ifdef THREE_BMSPS

            configBMSPS3();

        #endif

    #endif

    //
    // Loop Forever
    //
    for(;;)
    {
        //
        // Do nothing. NOP == No Operation
        //
        asm("    NOP");
    }
}

//
// Functions
//

//
// configBMSPS0 - Configures a custom boot EMU configuration when zero boot mode
//                select pins are being used
//
//                Boot Mode Number                          Boot Mode
//                      0                                   Flash Boot
//
void configBMSPS0(void)
{
    //
    // Enable writing to the EALLOW protected registers
    //
    EALLOW;

    //
    // Write a value of 0x5A to the Key field of
    // EMU_BOOTPIN_CONFIG in order to execute
    // a custom boot process. Write the values of
    // BMSPs to be used.
    //
    HWREG(EMU_BOOTPIN_CONFIG) = (BOOTPIN_CONFIG_KEY  << 24) + 
                                (BOOTPIN_CONFIG_BMSPDISABLE << 16) + 
                                (BOOTPIN_CONFIG_BMSPDISABLE << 8) + 
                                (BOOTPIN_CONFIG_BMSPDISABLE);

    //
    // Write boot mode option 0 to the EMU_BOOTDEF_LOW register
    //
    HWREG(EMU_BOOTDEF_LOW) = BOOTDEF_LOW_0;

    //
    // Disable writing to the EALLOW protected registers
    //
    EDIS;
}

//
// configBMSPS1 - Configures a custom EMU boot configuration when 1 boot mode
//                select pin is being used.
//
//                Boot Mode Number     BMSP0          Boot Mode
//                      0                0            Flash Boot
//                      1                1            SCI Boot
//
void configBMSPS1(void)
{
    //
    // Enable writing to the EALLOW protected registers
    //
    EALLOW;

    //
    // Write a value of 0x5A to the Key field of
    // EMU_BOOTPIN_CONFIG in order to execute
    // a custom boot process. Write the values of
    // BMSPs to be used.
    //
    HWREG(EMU_BOOTPIN_CONFIG) = (BOOTPIN_CONFIG_KEY  << 24) + 
                                (BOOTPIN_CONFIG_BMSPDISABLE << 16) + 
                                (BOOTPIN_CONFIG_BMSPDISABLE << 8) + 
                                (BOOTPIN_CONFIG_BMSP0);

    //
    // Write boot mode options 1 and 0 to the EMU_BOOTDEF_LOW register
    //
    HWREG(EMU_BOOTDEF_LOW) = (BOOTDEF_LOW_1 << 8) + BOOTDEF_LOW_0;

    //
    // Disable writing to the EALLOW protected registers
    //
    EDIS;
}

//
// configBMSPS3 - Configures a custom EMU boot configuration when 3 boot
//                mode select pins are used.
//
//                Boot Mode Number  BMSP2  BMSP1  BMSP0    Boot Mode
//                     0              0      0      0      Flash Boot
//                     1              0      0      1      SCI Boot
//                     2              0      1      0      Flash Boot Alt.1
//                     3              0      1      1      SCI Boot Alt.1
//                     4              1      0      0      CAN Boot
//                     5              1      0      1      SPI Boot
//                     6              1      1      0      RAM Boot
//                     7              1      1      1      I2C Boot
//
void configBMSPS3(void)
{
    //
    // Enable writing to the EALLOW protected registers
    //
    EALLOW;

    //
    // Write a value of 0x5A to the Key field of
    // EMU_BOOTPIN_CONFIG in order to execute
    // a custom boot process. Write the values of
    // BMSPs to be used.
    //
    HWREG(EMU_BOOTPIN_CONFIG) = (BOOTPIN_CONFIG_KEY  << 24) +
                                (BOOTPIN_CONFIG_BMSP2 << 16) + 
                                (BOOTPIN_CONFIG_BMSP1 << 8) + 
                                (BOOTPIN_CONFIG_BMSP0);

    //
    // Write boot mode options 3, 2, 1, and 0 to the EMU_BOOTDEF_LOW register
    //
    HWREG(EMU_BOOTDEF_LOW) = (BOOTDEF_LOW_3 << 24) + (BOOTDEF_LOW_2 << 16) + 
                             (BOOTDEF_LOW_1 << 8) + BOOTDEF_LOW_0;

    //
    // Write boot mode options 7, 6, 5, and 4 to the EMU_BOOTDEF_HIGH register
    //
    HWREG(EMU_BOOTDEF_HIGH) = (BOOTDEF_HIGH_7 << 24) + (BOOTDEF_HIGH_6 << 16)
                              + (BOOTDEF_HIGH_5 << 8) + BOOTDEF_HIGH_4;

    //
    // Disable writing to the EALLOW protected registers
    //
    EDIS;
}

// 
// End of File 
//

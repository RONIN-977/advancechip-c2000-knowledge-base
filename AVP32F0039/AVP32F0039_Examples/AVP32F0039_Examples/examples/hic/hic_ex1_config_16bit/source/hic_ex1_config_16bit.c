/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  hic_ex1_DataWidth16bit.c
*                         TITLE: Host Interface Controller 16 bit Data Width Memory Access
*******************************************************************************************/

//
// Included Files
//
#include "device.h"
#include "driverlib.h"
#include "hic.h"


void initGPIOHIC(void)
{
    //
    //This routine does PinMux for 16 Bit configuration
    //of HIC module.These are for a TI Internal Board
    //
    
    //
    //For Chip Select Pin
    //
    GPIO_setPinConfig(GPIO_29_HIC_NCS);
    
    //
    //For Base Select Pins
    //
    GPIO_setPinConfig(GPIO_230_HIC_BASESEL2);
    GPIO_setPinConfig(GPIO_231_HIC_BASESEL1);
    GPIO_setPinConfig(GPIO_232_HIC_BASESEL0);
    GPIO_setQualificationMode(230,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(231,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(232,GPIO_QUAL_ASYNC);
    GPIO_setAnalogMode(230, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(231, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(232, GPIO_ANALOG_DISABLED);
    
    //
    //HINT Pin
    //
    GPIO_setPinConfig(GPIO_32_HIC_INT);
    
    //
    //Output Enable
    //
    GPIO_setPinConfig(GPIO_28_HIC_NOE);
    GPIO_setQualificationMode(28,GPIO_QUAL_ASYNC);
    
    //
    //Write Enable pin
    //
    GPIO_setPinConfig(GPIO_225_HIC_NWE);
    GPIO_setQualificationMode(225,GPIO_QUAL_ASYNC);
    GPIO_setAnalogMode(225, GPIO_ANALOG_DISABLED);
    
    //
    //Byte Enables
    //
    GPIO_setPinConfig(GPIO_227_HIC_NBE0);
    GPIO_setPinConfig(GPIO_241_HIC_NBE1);
    GPIO_setQualificationMode(227,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(241,GPIO_QUAL_ASYNC);
    GPIO_setAnalogMode(227, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(241, GPIO_ANALOG_DISABLED);
    
    //
    //Ready Pin
    //
    GPIO_setPinConfig(GPIO_9_HIC_NRDY);
    GPIO_setQualificationMode(9,GPIO_QUAL_ASYNC);
    
    //
    //HIC Data Pins
    //
    GPIO_setPinConfig(GPIO_33_HIC_D0);
    GPIO_setPinConfig(GPIO_16_HIC_D1);
    GPIO_setPinConfig(GPIO_17_HIC_D2);
    GPIO_setPinConfig(GPIO_24_HIC_D3);
    GPIO_setPinConfig(GPIO_5_HIC_D4);
    GPIO_setPinConfig(GPIO_44_HIC_D5);
    GPIO_setPinConfig(GPIO_45_HIC_D6);
    GPIO_setPinConfig(GPIO_39_HIC_D7);
    GPIO_setPinConfig(GPIO_8_HIC_D8);
    GPIO_setPinConfig(GPIO_34_HIC_D9);
    GPIO_setPinConfig(GPIO_1_HIC_D10);
    GPIO_setPinConfig(GPIO_23_HIC_D11);
    GPIO_setPinConfig(GPIO_15_HIC_D12);
    GPIO_setPinConfig(GPIO_22_HIC_D13);
    GPIO_setPinConfig(GPIO_7_HIC_D14);
    GPIO_setPinConfig(GPIO_14_HIC_D15);
    GPIO_setQualificationMode(33,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(16,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(17,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(24,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(5,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(44,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(45,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(39,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(8,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(34,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(1,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(23,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(15,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(22,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(7,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(14,GPIO_QUAL_ASYNC);
    
    //
    //HIC Address Pins
    //
    GPIO_setPinConfig(GPIO_228_HIC_A0);
    GPIO_setPinConfig(GPIO_226_HIC_A1);
    GPIO_setPinConfig(GPIO_242_HIC_A2);
    GPIO_setPinConfig(GPIO_224_HIC_A3);
    GPIO_setPinConfig(GPIO_233_HIC_A4);
    GPIO_setPinConfig(GPIO_239_HIC_A5);
    GPIO_setPinConfig(GPIO_237_HIC_A6);
    GPIO_setPinConfig(GPIO_244_HIC_A7);
    GPIO_setQualificationMode(228,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(226,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(242,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(224,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(233,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(239,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(237,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(244,GPIO_QUAL_ASYNC);
    GPIO_setAnalogMode(228, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(226, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(242, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(224, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(233, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(239, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(237, GPIO_ANALOG_DISABLED);
    GPIO_setAnalogMode(244, GPIO_ANALOG_DISABLED);
}

//
// Main
//
void main(void)
{
    uint32_t testData;
    uint16_t iteration;
    uint32_t baseAddress;
    
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    initGPIOHIC();

    HIC_enableHostInterface();
    //
    //Set HIC Configuration
    //16 Bit mode with Separate
    //Read and Write Enable pins
    //with Ready pin
    //H2D Buffer writable by Device
    //D2H buffer writable by host
    //
    HIC_setConfig(HIC_MODE_DW_16BIT |
                   HIC_MODE_RW_PIN_SEPARATE |
                   HIC_MODE_RDY_PRESENT |
                   HIC_MODE_H2DBUF_DEVWREN |
                   HIC_MODE_D2HBUF_HOST_WR_ENABLE |
                   HIC_MODE_DEVICE_HOST_ACCESS_ENABLE |
                   HIC_MODE_HOST_WREALLOW_ENABLE
                  );
                  
    //
    //Enable EALLOW access from Host
    //
    HIC_configureHostAccessMode(HIC_HOST_CONTROL_EALLOW_EN);

    //
    //Write test data to D2H buffer
    //
    testData = 0xA5A5A5A5;
    for(iteration=0; iteration < 16; iteration ++)
    {
        HIC_writeD2HBuffer(iteration,testData);
        testData += 0x11111111;
    }

    //
    //Flag to the host by writing to the Token
    //This will generate a D2H interrupt
    //
    HIC_setD2HToken(0xFA);

    //
    //Wait for Message from Host Containing the Base address
    //to be configured and clear the interrupt
    //
    while(HIC_getH2DInterruptStatus() != HIC_H2DINTFLG_H2D_FLG);
    HIC_clearH2DInterrupt(HIC_H2DINTFLG_H2D_FLG);
    while(HIC_getH2DInterruptStatus() != 0);

    if(HIC_readH2DBuffer(0) == 0x1)
    {
        baseAddress = HIC_readH2DBuffer(1);
        HIC_setBaseAddress(0,baseAddress);
    }
    
    //
    //Flag to the host by writing to the Token
    //This will generate a D2H interrupt
    //
    HIC_setD2HToken(0xFB);

    //
    //Now the Host side can read and write to the memory region
    //
    while(1)
    {
    }
}
//
// End of File
//

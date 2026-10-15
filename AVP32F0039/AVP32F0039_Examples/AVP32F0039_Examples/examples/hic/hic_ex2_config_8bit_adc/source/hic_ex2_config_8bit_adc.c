/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  hic_ex2_config_8bit_adc.c
*                         TITLE: Host Interface Controller 8 bit Data Width Access
*******************************************************************************************/
//
// Included Files
//
#include "device.h"
#include "driverlib.h"
#include "hic.h"

//
//Function Prototypes
//
void initGPIOHIC(void);
void initADC(void);
void initADCSOC(void);
void initDMA(void);
void configCPUTimer(uint32_t, uint32_t);

//
//Interrupt Handlers
//
__interrupt void dmaCh6ISR(void);
__interrupt void hicISR(void);

//
//Global Defines
//
#define HIC_INIT_DONE_TOKEN     0xFAU
#define HIC_START_TOKEN         0xFBU
#define HIC_DATA_TOKEN          0xFCU
#define HIC_TEST_NUM_SAMPLES    100U

//
//Global Variables
//

uint16_t   hicISRCount = 0;
volatile uint16_t startReceived = 0;
volatile uint16_t numDMAInterrupts = 0;

//
// Main
//
void main(void)
{

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    //Do HIC GPIO Initialization
    //
    initGPIOHIC();

    //
    //Configure ADC and ADC SOC
    //
    initADC();
    initADCSOC();

    //
    //Configure CPU Timer 0 for 1 second interrupt
    //
    configCPUTimer(CPUTIMER0_BASE, 1000000);

    //
    //Initialize DMA for this use case
    //
    initDMA();


    //
    //Initialize and Enable interrupt for DMA Channel
    //
    Interrupt_register(INT_DMA_CH6, &dmaCh6ISR);
    Interrupt_enable(INT_DMA_CH6);

    //
    //Initialize interrupts for HIC
    //
    Interrupt_register(INT_HICA, &hicISR);
    Interrupt_enable(INT_HICA);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    HIC_enableHostInterface();

    //
    //Set HIC Configuration
    //8 Bit mode with Separate
    //Read and Write Enable pins
    //with Ready pin
    //H2D Buffer writable by Device
    //D2H buffer writable by host
    //Device memory access is not accessible to Host
    //
    HIC_setConfig(HIC_MODE_DW_8BIT |
                   HIC_MODE_RW_PIN_SINGLE |
                   HIC_MODE_H2DBUF_DEVWREN |
                   HIC_MODE_D2HBUF_HOST_WR_ENABLE |
                   HIC_MODE_HOST_WREALLOW_ENABLE
                  );

    //
    //Enable HIC_INT for D2H Data ready event
    //
    HIC_enableD2HInterrupt(HIC_D2HINTEN_D2H_INTEN);

    //
    //Enable Interrupt for Host to Device Data ready
    //
    HIC_enableH2DInterrupt(HIC_H2DINTEN_H2D_INTEN);



    //
    //Flag to the host by writing to the Token
    //This will generate a D2H interrupt
    //
    HIC_setD2HToken(HIC_INIT_DONE_TOKEN);

    //
    //Wait for Message from Host Containing the Base address
    //to be configured
    //
    while(0U == startReceived);

    //
    //Start the timer which will trigger the ADC sampling
    //The completion of ADC sampling will trigger DMA event
    //The DMA interrupt handler will then notify the host
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);


    //
    //Now the Host side will be interrupted for each ADC result message
    //
    while(numDMAInterrupts != HIC_TEST_NUM_SAMPLES)
    {
    }

    //
    //Halt the execution here after HIC_TEST_NUM_SAMPLES of data
    //
    ESTOP0;
}
void
configCPUTimer(uint32_t cpuTimer, uint32_t period)
{
    uint32_t temp, freq = DEVICE_SYSCLK_FREQ;

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);

    //
    // Initialize timer period:
    //
    temp = ((freq / 1000000U) * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(cpuTimer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
}

//
// This function initializes the GPIOs for
// HIC module in 8 bit mode
//
void initGPIOHIC(void)
{
    //
    //This routine does PinMux for 8 Bit configuration
    //of HIC module.These are for a TI Internal Board
    //

    //
    //For Chip Select Pin
    //
    GPIO_setPinConfig(GPIO_29_HIC_NCS);
    GPIO_setQualificationMode(29,GPIO_QUAL_ASYNC);

    //
    //Output Enable
    //
    GPIO_setPinConfig(GPIO_28_HIC_NOE);
    GPIO_setQualificationMode(28,GPIO_QUAL_ASYNC);


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
    GPIO_setQualificationMode(33,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(16,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(17,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(24,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(5,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(44,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(45,GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(39,GPIO_QUAL_ASYNC);

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

    //
    //HIC_INT Pin
    //
    GPIO_setPinConfig(GPIO_32_HIC_INT);
}

//
// initADC - Function to configure and power up ADCA.
//
void initADC(void)
{
    //
    // Setup VREF as internal
    //
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    //
    // Set ADCCLK divider to /4
    //
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);


    //
    // Power up the ADC and then delay for 1 ms
    //
    ADC_enableConverter(ADCA_BASE);
    DEVICE_DELAY_US(1000);
}

//
// initADCSOCs - Function to configure SOC
//
void initADCSOC(void)
{
    uint16_t i;
    //
    // Configure SOCs of ADCA
    // - SOC0-15 will convert pin A7 with a sample window of 15 SYSCLK cycles.
    //
    for(i = 0; i < 16; i++)
   {
        ADC_setupSOC(ADCA_BASE, (ADC_SOCNumber)i, ADC_TRIGGER_CPU1_TINT0,
                      ADC_CH_ADCIN7, 15);
   }

    //
    // Set SOC15 to set the interrupt 1 flag. Enable the interrupt and make
    // sure its flag is cleared.
    //
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER15);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    //
    // Enable continuous mode
    //
    ADC_enableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
}

//
// DMA setup
//
void initDMA(void)
{
    //
    // Initialize DMA
    //
    DMA_initController();


    //
    // Configure DMA Ch6
    // Copy ADC Result(s) to HIC D2H Buffer
    //
    DMA_configAddresses(DMA_CH6_BASE, (uint16_t *) (HIC_BASE + HIC_O_D2H_BUF0),
                        (uint16_t *)(ADCARESULT_BASE));
    //
    //Configure the 16 SOC Results of ADC
    //Each D2H buffer is of size 32 bits
    //Hence destination step size is 2
    //Source is 16 bit wide hence step size is 1
    //
    DMA_configBurst(DMA_CH6_BASE, 16, 1, 2);

    //
    //At the end of each burst adjust Offset to point to first
    //locations of ADCRESULT0 and HIC D2H Buffer
    //
    DMA_configTransfer(DMA_CH6_BASE, 1, -15, -30);

    //
    //Configure Continuous mode to enable the channel after each transfer
    //
    DMA_configMode(DMA_CH6_BASE, DMA_TRIGGER_ADCA1, DMA_CFG_ONESHOT_DISABLE |
                   DMA_CFG_CONTINUOUS_ENABLE | DMA_CFG_SIZE_16BIT);
    DMA_disableOverrunInterrupt(DMA_CH1_BASE);

    //
    // Configure DMA Ch6 interrupts
    //
    DMA_setInterruptMode(DMA_CH6_BASE, DMA_INT_AT_END);
    DMA_enableInterrupt(DMA_CH6_BASE);
    DMA_enableTrigger(DMA_CH6_BASE);

    //
    // Start the DMA channels
    //
    DMA_startChannel(DMA_CH6_BASE);
}

//
// DMA Channel 6 ISR
//
 __interrupt void dmaCh6ISR(void)
{
     numDMAInterrupts++;

     if(HIC_TEST_NUM_SAMPLES == numDMAInterrupts)
     {
        //
        //Stop the DMA Channel and Timer after required
        //number of samples/iterations
        //
        DMA_stopChannel(DMA_CH6_BASE);
        CPUTimer_stopTimer(CPUTIMER0_BASE);
     }

    //
    //Clear the ADC interrupt Status
    //
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    //
    //Flag to the host by writing to the Token
    //This will generate a D2H interrupt
    //
    HIC_setD2HToken(HIC_DATA_TOKEN);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);

}

//
//HIC ISR
//
__interrupt void hicISR(void)
{
    hicISRCount++;
    if(HIC_getH2DInterruptStatus() == HIC_H2DINTFLG_H2D_FLG)
    {
        if(HIC_getH2DToken() == HIC_START_TOKEN)
            startReceived = 1;
        HIC_clearH2DInterrupt(HIC_H2DINTFLG_H2D_FLG);
    }
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}
//
// End of File
//

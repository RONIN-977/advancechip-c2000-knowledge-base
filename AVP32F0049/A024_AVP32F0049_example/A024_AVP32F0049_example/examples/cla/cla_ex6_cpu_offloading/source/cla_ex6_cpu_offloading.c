/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex6_cpu_offloading.c
*                         TITLE: Optimal offloading of control algorithms to CLA
*******************************************************************************************/

//
// Included Files
//
#include <cla_ex6_cpu_offloading_shared.h>
#include "DCLF32.h"
#include "board.h"

bool system_OFF = false; // Input used to shut down the system
bool shut_down_flag = false; // Shut down status flag

//
// Allocate all the below shared variables across C28x and CLA to a
// "cla_shared" section. This section is allocated to RAMLS1 in linker
// cmd file
//
#pragma SET_DATA_SECTION("cla_shared")
volatile reference ref_data = {3500,3500}; // Target digital values for loops
volatile sensed sense_data = {0,0};   // ADC sensed data for both loops
volatile control_out control_out_data = {0.0,0.0}; // Outputs of controllers
#if run_loop1_cla == 1
volatile DCL_PI_CLA pi_loop1 = PI_CLA_DEFAULTS;  // Controller definition
#else
volatile DCL_PI pi_loop1 = PI_DEFAULTS;
#endif
volatile DCL_PI pi_loop2 = PI_DEFAULTS;
volatile float32_t duty = 0.0f, duty_loop1 = 0.0f, duty_loop2 = 0.0f;     // Denotes duty of EPWM1 output
#pragma SET_DATA_SECTION()   // Reset section to default

//
// Function Prototypes
//
void initEPWM(void);
void setupADCSOC_trigger(void);
void initADC(void);
void initADCSOC(void);
void controller_initialize(void);
void setuploop1ProfileGpio(void);
void setuploop2ProfileGpio(void);


#if run_loop1_cla == 0
#if defined(_FLASH)
#pragma CODE_SECTION(loop1ISR,".TI.ramfunc");
#endif
__interrupt void loop1ISR(void);
#endif
#if defined(_FLASH)
#pragma CODE_SECTION(loop2ISR,".TI.ramfunc");
#endif
__interrupt void loop2ISR(void);


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
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // GPIO0 is set to EPWM1A
    //
    GPIO_setControllerCore(0, GPIO_CORE_CPU1);
    GPIO_setPadConfig(0,GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_0_EPWM1_A);

    //
    // Setup Profiling GPIOs for loop1 and loop2
    //
    setuploop1ProfileGpio();
    setuploop2ProfileGpio();

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
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Set up ADCA and ADCB, initialize the SOsC and configure EPWM4 and EPWM5
    // as trigger sources
    //
    initADC();
    initADCSOC();
    setupADCSOC_trigger();

    //
    // Initialize EPWM1 module to generate the PWM waveform
    //
    initEPWM();

    //
    // Initialize the PI controllers with the desired constants for
    // both the loops
    //
    controller_initialize();

#if run_loop1_cla == 1
    //
    // Setup the CLA if loop1 is offloaded to CLA
    //
    Board_init();
#endif

    //
    // Register the ADC interrupts for the control loops
    //
#if run_loop1_cla == 0U
    Interrupt_register(INT_ADCA1, &loop1ISR);
#endif
    Interrupt_register(INT_ADCB1, &loop2ISR);

    //
    // Enable ADCA and ADCB interrupts
    //
#if run_loop1_cla == 0U
    Interrupt_enable(INT_ADCA1);
#endif
    Interrupt_enable(INT_ADCB1);

    //
    // Enable global interrupts.
    //
    EINT;

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    for(;;)
    {
        if(system_OFF == true)
        {
            //
            // Force One-shot Trip EPWM1A output and disable the
            // control loops if it's not already shut down
            //
            if (shut_down_flag == false)
            {
                EPWM_forceTripZoneEvent(EPWM1_BASE, EPWM_TZ_FORCE_EVENT_OST);
                ADC_disableInterrupt(ADCB_BASE, ADC_INT_NUMBER1);
#if run_loop1_cla == 0U
                ADC_disableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
#else
                CLA_disableTasks(CLA1_BASE, CLA_TASKFLAG_1);
#endif
                shut_down_flag = true;
            }
        }

        else
        {
            //
            // Clear the one-shot trip for EPWM1 and enable the control
            // loops back only if it's shut down
            //
            if (shut_down_flag == true)
            {
                EPWM_clearTripZoneFlag(EPWM1_BASE, EPWM_TZ_FLAG_OST);
                ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER1);
#if run_loop1_cla == 0U
                ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
#else
                CLA_enableTasks(CLA1_BASE, CLA_TASKFLAG_1);
#endif
                shut_down_flag = false;
            }

        }
    }
}

//
// ADCA and ADCB initialization
// Function to configure and power up ADCs
//
void initADC(void)
{
    //
    // Setup VREF as internal
    //
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(ADCB_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    //
    // Set ADCCLK divider to /4
    //
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);
    ADC_setPrescaler(ADCB_BASE, ADC_CLK_DIV_4_0);

    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCB_BASE, ADC_PULSE_END_OF_CONV);

    //
    // Power up the ADCs and then delay for 1 ms
    //
    ADC_enableConverter(ADCA_BASE);
    ADC_enableConverter(ADCB_BASE);

    DEVICE_DELAY_US(1000);
}

//
// ADCA and ADCB SOC Initialization
//
// Description: ADCA and ADCB will oversample the Ch0 and Ch2 inputs
// on reception of a trigger from EPWM4 (EPMW4SOCA) and EPWM5 (EPMW5SOCA).
// Both ADCs are configured to sample the channels continuously.
//
void initADCSOC(void)
{
    //
    // First input parameter oversampling configuration using ADCA
    // - SOC0 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    // - SOC1 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    // - SOC2 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    // - SOC3 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    //
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN0, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN0, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN0, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN0, 10);

    //
    // Second input parameter oversampling configuration using ADCB
    // - SOC0 will convert pin B2 with a sample window of 10 SYSCLK cycles.
    // - SOC1 will convert pin B2 with a sample window of 10 SYSCLK cycles.
    // - SOC2 will convert pin B2 with a sample window of 10 SYSCLK cycles.
    // - SOC3 will convert pin B2 with a sample window of 10 SYSCLK cycles.
    //
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM5_SOCA,
                 ADC_CH_ADCIN2, 10);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM5_SOCA,
                 ADC_CH_ADCIN2, 10);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM5_SOCA,
                 ADC_CH_ADCIN2, 10);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM5_SOCA,
                 ADC_CH_ADCIN2, 10);

    //
    // - SOCs will sample on each trigger regardless of the interrupt flag
    //
    ADC_enableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_enableContinuousMode(ADCB_BASE, ADC_INT_NUMBER1);

    //
    // - EOC3 will be generated at the end of conversion of SOC3
    // - EOC3 will generate the ADCA interrupt 1
    //
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    //
    // - EOC3 will be generated at the end of conversion of SOC3
    // - EOC3 will generate the ADCB interrupt 1
    //
    ADC_setInterruptSource(ADCB_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
    ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER1);
}

//
// Configuring EPWM4 and EPWM5 as trigger sources for ADCA and ADCB
// Providing phase shift between the modules
//
void setupADCSOC_trigger(void)
{
    //
    // Set up EPWM4 to
    // - run on a base clock of SYSCLK
    // - have a period of EPWM4_PERIOD (200 KHz)
    // - run in count up mode
    // - Start counting from 0
    EPWM_setClockPrescaler(EPWM4_BASE, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(EPWM4_BASE, EPWM4_PERIOD - 1U);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setTimeBaseCounter(EPWM4_BASE, 0U);

    //
    // Enable SOC-A and set it to assert when the counter hits
    // zero. It asserts on every event
    //
    EPWM_enableADCTrigger(EPWM4_BASE, EPWM_SOC_A);
    EPWM_setADCTriggerSource(EPWM4_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(EPWM4_BASE, EPWM_SOC_A, 1U);

    //
    // Set up EPWM5 to
    // - run on a base clock of SYSCLK
    // - have a period of EPWM5_PERIOD (20 KHz)
    // - run in count up mode
    // - Start counting from 0
    EPWM_setClockPrescaler(EPWM5_BASE, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(EPWM5_BASE, EPWM5_PERIOD - 1U);
    EPWM_setTimeBaseCounterMode(EPWM5_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setTimeBaseCounter(EPWM5_BASE, 0U);

    //
    // Enable SOC-A and set it to assert when the counter hits
    // zero. It asserts on every event
    //
    EPWM_enableADCTrigger(EPWM5_BASE, EPWM_SOC_A);
    EPWM_setADCTriggerSource(EPWM5_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(EPWM5_BASE, EPWM_SOC_A, 1U);

    //
    // EPWM 4 and 5 should run freely in emulation mode
    //
    EPWM_setEmulationMode(EPWM4_BASE, EPWM_EMULATION_FREE_RUN);
    EPWM_setEmulationMode(EPWM5_BASE, EPWM_EMULATION_FREE_RUN);
}

//
// EPWM1 Initialization
// EPWM1 : generates output of frequency EPWM1_FREQ (1 MHz)
//
void initEPWM(void)
{
    //
    // Set up EPWM1 to
    // - run on a base clock of SYSCLK
    // - have a period of EPWM1_PERIOD
    // - run in count up-down mode
    //
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(EPWM1_BASE, EPWM1_PERIOD);
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM1_PERIOD/10U);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0U);

    //
    // Configuring action-qualifiers for EPWM1 to generate symmetric
    // waveform on channel A
    //
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A,
                         EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A,
                             EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    //
    // Enabling Counter Compare shadow mode
    //
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_SYNC_CNTR_ZERO);

    //
    // Enable TZ1 as one shot trip sources
    //
    EPWM_enableTripZoneSignals(EPWM1_BASE, EPWM_TZ_SIGNAL_OSHT1);

    //
    // Action on TZ1
    //
    EPWM_setTripZoneAction(EPWM1_BASE,
                            EPWM_TZ_ACTION_EVENT_TZA,
                            EPWM_TZ_ACTION_LOW);
    //
    // Run freely in emulation mode
    //
    EPWM_setEmulationMode(EPWM1_BASE, EPWM_EMULATION_FREE_RUN);
}

//
// Configuring the loop1 and loop2 controllers with desired values
//
void controller_initialize(void)
{
    pi_loop1.Kp = 0.7;
    pi_loop1.Ki = 0.01;
    pi_loop1.Umax = 1.0;
    pi_loop1.Umin = 0.0;

    pi_loop2.Kp = 0.7;
    pi_loop2.Ki = 0.01;
    pi_loop2.Umax = 1.0;
    pi_loop2.Umin = 0.0;
}

//
// Setting up GPIO2 for profiling loop1
// Also set it's controller to CLA if loop1 is running on CLA
//
void setuploop1ProfileGpio(void)
{
        GPIO_setDirectionMode(2,GPIO_DIR_MODE_OUT);
        GPIO_setQualificationMode(2,GPIO_QUAL_SYNC);
        GPIO_setPinConfig(GPIO_2_GPIO2);
        GPIO_writePin(2,0);
#if run_loop1_cla == 1
        GPIO_setControllerCore(2, GPIO_CORE_CPU1_CLA1);
#endif
}

//
// Setting up GPIO3 for profiling loop2
//
void setuploop2ProfileGpio(void)
{
        GPIO_setDirectionMode(3,GPIO_DIR_MODE_OUT);
        GPIO_setQualificationMode(3,GPIO_QUAL_SYNC);
        GPIO_setPinConfig(GPIO_3_GPIO3);
        GPIO_writePin(3,0);
}

#if run_loop1_cla == 0
//
// Loop1 control ISR
//
__interrupt void loop1ISR(void)
{
    //
    // Turn ON Loop1 Profiling GPIO
    //
    GPIO_writePin(2,1);

    //
    // Run the core loop1 logic
    //
    loop1_task();

    //
    // Turn OFF Loop1 Profiling GPIO
    //
    GPIO_writePin(2,0);

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);


}
#endif

//
// Loop2 control ISR
//
__interrupt void loop2ISR(void)
{
    //
    // Turn ON Loop2 Profiling GPIO
    //
    GPIO_writePin(3,1);

#if  run_loop1_cla == 0
    //
    // Code to enable nesting of high priority loop1 interrupt
    //
    uint16_t temp_Pieier = HWREGH(PIECTRL_BASE + PIE_O_IER1);
    IER |= 0x1U;
    IER &= 0x1U;
    HWREGH(PIECTRL_BASE + PIE_O_IER1) &= 0x1U;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    NOP;
    EINT;
#endif

    //
    // Read the oversampled input captured by 4 SOCs and calculate
    // the average
    //
    sense_data.loop2 = (ADC_readResult(ADCBRESULT_BASE,ADC_SOC_NUMBER0) +
                        ADC_readResult(ADCBRESULT_BASE,ADC_SOC_NUMBER1) +
                         ADC_readResult(ADCBRESULT_BASE,ADC_SOC_NUMBER2) +
                          ADC_readResult(ADCBRESULT_BASE,ADC_SOC_NUMBER3)) >> 2;

    //
    // Run the parallel form PI controller
    //
    control_out_data.loop2 = DCL_runPI_C3(&pi_loop2, __divf32(ref_data.loop2, 4096.0f),
                                          __divf32(sense_data.loop2, 4096.0f));

    //
    // Calculate the duty contribution based on the controller output with 20% weightage
    //
    duty_loop2 = 0.2f * control_out_data.loop2;

    //
    // Turn OFF Loop2 Profiling GPIO
    //
    GPIO_writePin(3,0);

#if  run_loop1_cla == 0
    //
    // Disabling global interrupt and restoring PIEIER1 register
    //
    DINT;
    HWREGH(PIECTRL_BASE + PIE_O_IER1) = temp_Pieier;
#endif

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

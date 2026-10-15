/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   epwm_ex14_global_load_use_case.c
*                         TITLE:  Use Case Example Code for EPWM Developers Guide
*******************************************************************************************/
// The use case described in the application report is the following:
//
// Output frequency of 400 kHz for EPWM1/2/3
// Phase shift of 120 degrees for EPWM2 with respect to EPWM1
// Phase shift of 240 degrees for EPWM3 with respect to EPWM1
// Duty cycle of 45% for EPWM1/2/3
// Active high complementary signal pairs with 200 nsec of rising/falling edge delay for EPWM1/2/3
// Cycle-by-Cycle trip zone protection through comparator signal on EPWM2
// One-Shot trip protection through GPIO on EPWM3
// Interrupt generation everytime the time base counter equals zero on EPWM1
// Global loading to support asynchronous updates of action qualifier settings
// Link CMPA/CMPB of EPWM1 to EPWM2 and EPWM3
//

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t  perform_one_shot_load=0;

//
// Function Prototypes
//
__interrupt void INT_myEPWM1_ISR(void);
__interrupt void INT_myEPWM2_TZ_ISR(void);
__interrupt void INT_myEPWM3_TZ_ISR(void);

//
// Main
//
void main(void)
{
    // Initialize device clock and peripherals
      //
      Device_init();

      //
      // Disable pin locks and enable internal pull-ups.
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
      // Disable sync(Freeze clock to PWM as well)
      //
      SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

      //
      // Configure ePWM1, ePWM2, and TZ GPIOs
      //
      Board_init();

      //
      // Workaround for Global Loading feature in SysConfig version 1.11 or below
      // Un-comment the following section for a SysConfig version of 1.11 or below (CCS v11 & C2000Ware version 4.01.00.00)
      //
      /*
      EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
      EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
      EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
      EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

      EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
      EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
      EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
      EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

      EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
      EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
      EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
      EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);


      // EPWM1, EPWM2, EPWM3 are linked so this will update both
      EPWM_setGlobalLoadOneShotLatch(myEPWM1_BASE);
      */

      //
      // Enable sync and clock to PWM
      //
      SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

      //
      // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
      //
      EINT;
      ERTM;

      //
      // IDLE loop. Just sit and loop forever (optional):
      //
      for(;;)
      {

          if(perform_one_shot_load==1)
          {
              //
              // EPWM1, EPWM2, EPWM3 are linked so only EPWM1 needs to be updated
              //
              EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, 100);
              EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, 100);

              //
              // Change the Action Qualifier Settings for EPWM1, EPWM2, and EPWM3
              //
              EPWM_setActionQualifierAction(myEPWM1_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_LOW,
                                            EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
              EPWM_setActionQualifierAction(myEPWM1_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_HIGH,
                                            EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
              EPWM_setActionQualifierAction(myEPWM2_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_LOW,
                                            EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
              EPWM_setActionQualifierAction(myEPWM2_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_HIGH,
                                                          EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
              EPWM_setActionQualifierAction(myEPWM3_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_LOW,
                                            EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
              EPWM_setActionQualifierAction(myEPWM3_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_HIGH,
                                            EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

              // EPWM1, EPWM2, EPWM3 are linked
              EPWM_setGlobalLoadOneShotLatch(myEPWM1_BASE);

              // Clear the setting
              perform_one_shot_load = 0;
          }
      }
}


void INT_myEPWM1_ISR(void){
    //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP2);
}

void INT_myEPWM2_TZ_ISR(void){

     //
     // Clear the flags
     //
     EPWM_clearTripZoneFlag(myEPWM2_BASE, (EPWM_TZ_INTERRUPT | EPWM_TZ_FLAG_CBC));

     //
     // Acknowledge this interrupt to receive more interrupts from group 2
     //
     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP2);
}

void INT_myEPWM3_TZ_ISR(void){

    //
    // Re-enable the OST Interrupt
    //
     EPWM_clearTripZoneFlag(myEPWM3_BASE, (EPWM_TZ_INTERRUPT | EPWM_TZ_FLAG_OST));

    //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP2);
}

//
// End of File
//


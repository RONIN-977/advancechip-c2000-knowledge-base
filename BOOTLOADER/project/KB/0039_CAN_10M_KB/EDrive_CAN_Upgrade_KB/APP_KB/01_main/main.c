/*
 * main.c
 *
 *  Created on: Feb 9, 2022
 *      Author: rand C
 */

#include "main.h"
#include "NVM.h"
#include "kb_can_upgrade.h"
//#include <05_Input/eeprom.h>  //123
u8 HwFo_trigger = 0;
u16  TaskCycle;

void GPIO17_Blink(void)
{
    static uint16_t initialized = 0;
    static uint32_t last;
    uint32_t now = CpuTimer1Regs.TIM.all;

    if (!initialized)
    {
        EALLOW;
        GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0;
        GpioCtrlRegs.GPAMUX2.bit.GPIO17  = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO17  = 1;
        GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
        EDIS;

        last = now;
        initialized = 1;
    }

    /* 当前APP为100MHz，Timer1不分频：500ms = 50000000个计数。 */
    if ((uint32_t)(last - now) >= 50000000UL)
    {
        last = now;
        GpioDataRegs.GPATOGGLE.bit.GPIO17 = 1;
    }
}

void main(void){
    u32  Tick_Realtime,Tick_Interval;
    u16  m_TickCnt;

    // Initialize device clock and peripherals
    //
    InitSysCtrl();
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);

//    DINT; // DINT is performed in PIE Init func
    InitPeriherals();
	flash_hal_init();
//    ResDemodulation_Init();
//    EepromDataRead();//123
    EALLOW;
    WdRegs.WDCR.all = 0x0028;
    EDIS;
    EINT;
    ERTM;  // Enable Global realtime interrupt DBGM

//    Task_App_Init();//123
    config_load();
    TaskCycle = 0;
    Tick_Realtime = SysTick();
    while(1){
        KB_CAN_UpgradeTask(); /* KB_UPGRADE: only added background call */
//        GPIO17_Blink();
        Tick_Interval = Tick_Realtime - SysTick();
        if (Tick_Interval > SYS_TICK_05MS){
            TaskCycle++;
            m_TickCnt = (TaskCycle & 0x03);
            Tick_Realtime -= SYS_TICK_05MS;
            ServiceDog();

//            Task_App_05ms();//123

            if (0 == m_TickCnt)
            {
//                Task_App_A_2ms();//123
                TpTx();
            }
//            else if (1 == m_TickCnt) Task_App_B_2ms();
//            else if (2 == m_TickCnt) Task_App_C_2ms();
//            else Task_App_D_2ms();//123

        }
    }
}

//__interrupt void ResAdcEndIsr(void){
//
//    /*
//     *  add user specific code here
//    */
//    ResDemodulation_ISR();
//
//    // Clear the interrupt flag
//   //
//   AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
//
//   //
//   // Check if overflow has occurred
//   //
//   if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT1)
//   {
//       AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
//       AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
//   }
//
//   //
//   // Acknowledge the interrupt
//   //
//   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}//123

__interrupt void ResPwmIsr(void){

    /*
     *  add user specific code here
    */
    //ResDemodulation_ISR();

    EPwm4Regs.ETCLR.bit.INT = 1; /* clear ePWM peripheral flag */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; /* clear PIE flag */

}

//__interrupt void ResAdcFOCIsr(void){
//
//    volatile u16 m_PIEIER;
//
//    //if (0 == gRes_Diag.resInit_Pass){
//    if (0 == gRes_Diag.ucResOffsetCheck){
//        m_PIEIER = PieCtrlRegs.PIEIER1.all;
//
//        IER |= M_INT1;
//        IER &= M_INT1;
//
//        PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
//        PieCtrlRegs.PIEACK.all = 0xFFFF;
//
//        asm("       NOP");
//        EINT;
//    }
//
//    /*
//     *  add user specific code here
//    */
//    Task_FVC_ISR();
//
//    // Clear the interrupt flag
//   //
//    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
//
//    //if (0 == gRes_Diag.resInit_Pass){
//    if (0 == gRes_Diag.ucResOffsetCheck){
//        DINT;
//        PieCtrlRegs.PIEIER1.all = m_PIEIER;
//    }
//
//    //
//    // Check if overflow has occurred
//    //
//    if(1 == AdccRegs.ADCINTOVF.bit.ADCINT1)
//    {
//       AdccRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
//       AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
//   }
//
//   //
//   // Acknowledge the interrupt
//   //
//   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}

//__interrupt void EPWMTZ_IsrTask(void)
//{
//    if((StateMachine.RuningState != STOP_STATE)&&((StateMachine.RuningState != ShutDown_STATE))){
//        HwFo_trigger = 1;
//    }
//
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;     // Acknowledge this interrupt
//}  //123

/*
 * can_Init.c
 *
 *  Created on: 2022Äê2ÔÂ20ÈÕ
 *      Author: 86176
 */
#include "sysInit_28002x.h"

void init_eCAN_A ( u16 bitRatePrescaler, u16 timeSeg1, u16
                  timeSeg2, u16 sbg, u16 sjw, u16 sam)
{
  u32 ui32RegValue;
  u16 ui16CANCTL;
  EALLOW;
  CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;  /* Enable pull-up on GPIO13 */
  GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 3;
  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;
  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;  /* Enable pull-up on GPIO12 */
  GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 3;
  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
  EDIS;
  ui32RegValue = (((u32)((bitRatePrescaler-1) & 0x03C0))<<10)|((timeSeg2-1)<<
    12)|((timeSeg1-1)<<8)|((sjw-1)<<6)|((bitRatePrescaler-1) & 0x3F);
//   EALLOW;
//  CanaRegs.CAN_CTL.bit.Init = 1;//CAN module ignores bus activity
//  CanaRegs.CAN_CTL.bit.PMD = 5;//Parity function disabled
//  CanaRegs.CAN_RAM_INIT.bit.CAN_RAM_INIT = 1;
//  CanaRegs.CAN_RAM_INIT.bit.KEY3 = 1;
//  CanaRegs.CAN_RAM_INIT.bit.KEY1 = 1;
//  while(!((CanaRegs.CAN_RAM_INIT.all & 0x003FU) == 0x25)){
//  }
//  CanaRegs.CAN_CTL.bit.SWR = 1;
//  SysCtl_delay(1U);
//  CanaRegs.CAN_CTL.bit.CCE = 1;// Configuration Change Enable
  CAN_initModule(CANA_BASE);
//  ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = 0;//CANA Bit-Clock Source Select PERx.SYSCLK
  CAN_selectClockSource(CANA_BASE, (CAN_ClockSource)0);

  //Set init mode
//  CanaRegs.CAN_CTL.bit.CCE = 1;// Configuration Change Enable
//  CanaRegs.CAN_CTL.bit.Init = 1;//CAN module ignores bus activity
  ui16CANCTL = HWREGH(CANA_BASE + CAN_O_CTL);
  HWREGH(CANA_BASE + CAN_O_CTL) = ui16CANCTL | CAN_CTL_INIT | CAN_CTL_CCE;

  //Set Bit Timing register
 //  CanaRegs.CAN_BTR.bit.BRP = bitRatePrescaler-1;
//  CanaRegs.CAN_BTR.bit.SJW = sjw-1;
//  CanaRegs.CAN_BTR.bit.TSEG1 = timeSeg1-1;
//  CanaRegs.CAN_BTR.bit.TSEG2 = timeSeg2-1;
//  CanaRegs.CAN_BTR.bit.BRPE = 0;//Baud Rate Prescaler Extension
  HWREGH(CANA_BASE + CAN_O_BTR) = (ui32RegValue & 0xFFFFU);
  HWREGH(CANA_BASE + CAN_O_BTR + 2) = (ui32RegValue >> 16);

  // Restore the saved CAN Control register.
  //  CanaRegs.CAN_IP_MUX21 = 0;
  HWREGH(CANA_BASE + CAN_O_CTL) = ui16CANCTL;
  HWREGH(CANA_BASE + CAN_O_IP_MUX21) = 0;
  HWREGH(CANA_BASE + CAN_O_IP_MUX21 + 2) = 0;

  // Enable the CAN for operation.
    //  CanaRegs.CAN_CTL.bit.CCE = 0;// Configuration Change disEnable
  //  CanaRegs.CAN_CTL.bit.Init = 0;//CAN module processes messages normally
  CAN_enableController(CANA_BASE);

//  EDIS;
}




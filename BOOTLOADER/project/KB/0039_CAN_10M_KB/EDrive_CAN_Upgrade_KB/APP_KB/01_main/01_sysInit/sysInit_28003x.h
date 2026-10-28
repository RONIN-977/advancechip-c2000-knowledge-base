/*
 * SysInit_28002x.h
 *
 *  Created on: Feb 11, 2022
 *      Author: rand C
 */

#ifndef SYSINIT_28003X_H_
#define SYSINIT_28003X_H_

#include "uC_Inc.h"

extern void InitPeriherals(void);
extern void init_eCAN_A ( u16 bitRatePrescaler, u16 timeSeg1, u16
                               timeSeg2, u16 sbg, u16 sjw, u16 sam);
extern void CAN_Config_Init(void);
extern __interrupt void ResPwmIsr(void);
extern __interrupt void ResAdcEndIsr(void);
extern __interrupt void ResAdcFOCIsr(void);
extern __interrupt void EPWMTZ_IsrTask(void);

#endif /* 01_SYSINIT_SYSINIT_28003X_H_ */

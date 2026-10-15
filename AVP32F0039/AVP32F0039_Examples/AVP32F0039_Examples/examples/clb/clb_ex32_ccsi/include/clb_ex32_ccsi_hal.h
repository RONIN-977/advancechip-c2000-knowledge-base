/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   clb_ex32_ccsi_hal.h
*                         TITLE:  Prototype definitions for CCSI hardware abstraction layer.
*******************************************************************************************/

#include <clb_ex32_lp5891.h>
#include <clb_ex32_system_info.h>
#include "driverlib.h"
#include "board.h"

#ifndef CCSI_HAL_H_
#define CCSI_HAL_H_

extern uint32_t clbXmtBuffer[MAX_DATA_LENGTH]; //Stores the data bytes to be sent
extern uint16_t clbRcvBuffer[MAX_DATA_LENGTH]; //Stores the bytes received
extern volatile uint16_t clbXmtDone;

void CCSI_HAL_initClbLogic(void);
void CCSI_HAL_setupInterrupts(void);
void CCSI_HAL_startLedClocks(void);
void CCSI_HAL_clbTransferBlocking(uint32_t *xmtBuffer, uint32_t numXmtWords);
void CCSI_HAL_clbTransferNonBlocking(uint32_t *xmtBuffer, uint32_t numXmtWords);
int16_t CCSI_HAL_clbRxRingBuffRead (uint16_t *data);
void CCSI_HAL_clbSyncTransferStart(void);
void CCSI_HAL_ringBufferReset(void);
void CCSI_HAL_enableClbReceiver(void);
void CCSI_HAL_disableClbReceiver(void);

#endif /* CCSI_HAL_H_ */

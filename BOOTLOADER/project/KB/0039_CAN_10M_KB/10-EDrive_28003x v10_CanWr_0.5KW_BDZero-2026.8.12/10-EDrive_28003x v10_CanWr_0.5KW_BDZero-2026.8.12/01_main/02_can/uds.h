/*
 * uds.h
 *
 *  Created on: 2022Äê3ÔÂ6ÈÕ
 *      Author: 86176
 */

#ifndef UDS_H_
#define UDS_H_
#include <DataTypeDef.h>
#include "can_message.h"
#include "can.h"

typedef struct 
{
	u8  DIDIndex;
	u8  StateMachine;
	u8  sendTotalNum;
	u8  sendIndex;
	u16 TimeoutCnt;
	u8  txdata[30];
	
}uds_Tp_Status_S;


void TpTx();

#endif /* 01_MAIN_02_CAN_USERCAN_H_ */

/*
 * userCan.h
 *
 *  Created on: 2022Äê3ÔÂ6ÈÕ
 *      Author: 86176
 */

#ifndef CAN_USERCAN_H_
#define CAN_USERCAN_H_
#include <DataTypeDef.h>
#include "can_message.h"
#include "can.h"
//#include "StateMachine.h"//123
//#include "TroqueFbk.h"  //123
extern void CAN_Transmit_Task();

extern void CAN_Receive_Task();
extern void TransCanData(u16 transType);
extern void ReceiveCanData(CAN_DATATYPE *canRecbuf);
extern u16 Constant1_Value_i[8];

extern CAN_DATATYPE can_Rx;
extern CAN_DATATYPE can_Tx;
typedef struct{
    u8  VCU_VehDrvMdReq;
    u8  VCU_MotSopCmd;
    u8  VCU_ActDcrhgCmd;
    u8  VCU_ActPowerDownReq;
    u8  VCU_DirecEn;
    u8  VCU_FaultQuery;
    float VCU_MotToqReq;
    float VCU_MotSpdReq;
    u8   VCU_VehicleSpeedValid;
    float VCU_VehicleSpeed;
    u8  VCU_119_CheckSum;
    u8  VCU_119_RollingCounter;
    u8  VCU_118_CheckSum;
    u8  VCU_118_RollingCounter;
} VCU_MSG_STRUCT;
typedef struct{
    float  MCU_MotActuToq;
    float  MCU_MotActuSpd;
    float  MCU_MotToqLmt;
    u8  MCU_DirecEn;
    u8  MCU_MotActuMd;
    u8  MCU_ShutdownEnable;
    u8  MCU_Sts;
    u8  MCU_130_CheckSum;
    u8  MCU_130_RollingCounter;
    u8  HVILClsdFlt;
    u8  MCU_132_CheckSum;
    u8  MCU_132_RollingCounter;
    u8  MCU_631_CheckSum;
    u8  MCU_631_RollingCounter;
    u8  MCU_181_CheckSum;
    u8  MCU_181_RollingCounter;
} MCU_MSG_STRUCT;
extern VCU_MSG_STRUCT vcu_Msg;
extern MCU_MSG_STRUCT mcu_Msg;

extern u8 calCrc(u8 *data,u8 len);

#endif /* 01_MAIN_02_CAN_USERCAN_H_ */

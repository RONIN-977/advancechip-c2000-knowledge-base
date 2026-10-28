/*
 * debugCan.h
 *
 *  Created on: 2022��2��21��
 *      Author: 86176
 */

#ifndef CAN_DEBUGCAN_H_
#define CAN_DEBUGCAN_H_
#include <DataTypeDef.h>
#include "can_message.h"
//#include "ParaTrans.h" //123
//#include "Acr.h"  //123
#include "userCan.h"
//#include "ADC_Sample.h"  //123
//#include"Error.h"  //123
//#include "RdcApp.h"  //123
//#include "CurSet_Calib.h"  //123
//#include "PwmMod3ph.h"  //123
//#include "DeadBandComp.h"  //123
typedef struct{
    float VCU_MotSpdReq;
    u8   VCU_VehicleSpeedValid;
    float VCU_VehicleSpeed;
    u8  VCU_119_CheckSum;
    u8  VCU_119_RollingCounter;
} DEBUGCAN_FDBK_STRUCT;
typedef struct{
    float  IsCMD;
    float  ThetaCMD;
    u8  Enable;
    u8  ModelSelect;
    float  Offset;
} DEBUGCAN_CMD_STRUCT;

extern DEBUGCAN_FDBK_STRUCT debugMCU_Msg;
extern DEBUGCAN_CMD_STRUCT debug_Msg;

extern void debugTransCanData(u16 transType);
extern void debugReceiveCanData(CAN_DATATYPE *canRecbuf);

extern f32 gSpdRpm;

#endif /* 01_MAIN_02_CAN_DEBUGCAN_H_ */


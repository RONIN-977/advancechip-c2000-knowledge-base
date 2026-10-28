/*
 * canConfig.h
 *
 *  Created on: 2022Äê3ÔÂ7ÈÕ
 *      Author: 86176
 */

#ifndef CAN_CANCONFIG_H_
#define CAN_CANCONFIG_H_
#include <DataTypeDef.h>
#include "can.h"
extern void MCU_CAN_Transmit_Task_Init(void);
extern void MCU_CAN_Receive_Task_Init(void);
extern void CAN_Config_Init(void);
#define CanBoxOffset  10

#define Debugcan_VCU102  0x102
#define Debugcan_MCU200  0x200
#define Debugcan_MCU23F  0x23F
#define Debugcan_MCU202  0x202
#define Debugcan_MCU203  0x203
#define Debugcan_MCU204  0x204

#define Usercan_VCU118  0x118
#define Usercan_VCU119  0x119
#define Usercan_VCU126  0x126
#define Usercan_VCU620  0x620
#define Usercan_UDS721  0x721
#define Usercan_UDS7df  0x7df

#define Usercan_MCU130  0x130
#define Usercan_MCU132  0x132
#define Usercan_MCU181  0x181
#define Usercan_MCU631  0x631
#define Usercan_MCU111  0x111
#define Usercan_MCU112  0x112
#define Usercan_MCU101  0x101
#define Usercan_MCU118  0x118
#define Usercan_PhyRes729  0x729
#define dbg_mode    1  //1:debug  0:normal
#define EnableABO   0  //1:ABO auto-bus-off   0:manual bus off
#define acce_mode   0  //1:accelerator  0:normal

typedef enum
{
   DEBUG_102_msg,
   VCU_118_msg,
   VCU_119_msg,
   VCU_126_msg,
   VCU_620_msg,
   UDS_721_msg,
   UDS_7df_msg,
   NUM_USERCAN_REC

}usercanRecMap_e;
typedef enum
{
   MCU_130_msg,
   MCU_132_msg,
   MCU_181_msg,
   MCU_631_msg,
   MCU_111_msg,
   MCU_729_msg,
   MCU_112_msg,
   MCU_101_msg,
   MCU_118_msg,
   NUM_USERCAN_TRANS

}usercanTransMap_e;
typedef enum
{
   MCU_200_msg,
   MCU_23F_msg,
   MCU_202_msg,
   MCU_203_msg,
//   MCU_204_msg,
   NUM_DEBUGCAN_TRANS

}debugcanTransMap_e;
typedef struct{
    u32  Rx_Mail_ID[8];
    u32  Reporter_Mail_ID[8];
    u8  Reporter_Length;
    u8  Reporter_Enable;
    u8  Timeout_Error;
    float  Offset;
} CANSETTING_STRUCT;

extern CANSETTING_STRUCT CAN_Setting;
#endif /* 01_MAIN_02_CAN_CANCONFIG_H_ */

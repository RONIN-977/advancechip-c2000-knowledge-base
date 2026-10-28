/*
 * debugCan.c
 *
 *  Created on: 2022��2��21��
 *      Author: 86176
 */
#include "debugCan.h"
#include "canConfig.h"
//#include "Resolver_FaultDiag.h"  //123

DEBUGCAN_CMD_STRUCT debug_Msg;

//extern s16 Tload;

void debugReceiveCanData(CAN_DATATYPE *canRecbuf)
{
//    u16 u16temp = 0;
   if(canRecbuf->ID == 0x102)//100swj102
    {
       debug_Msg.IsCMD= ((f32)((u16)canRecbuf->Data[1]*256+canRecbuf->Data[0])-10000)*0.1f;
       debug_Msg.ThetaCMD= ((f32)((u16)canRecbuf->Data[3]*256+canRecbuf->Data[2])-10000)*0.1f;
       debug_Msg.Enable = (canRecbuf->Data[4] & 0x1);
       debug_Msg.ModelSelect = (canRecbuf->Data[4] & 0xF0)>>4;
       debug_Msg.Offset = ((f32)((u16)canRecbuf->Data[6]*256+canRecbuf->Data[5])-10000)*0.1f;
       if(debug_Msg.IsCMD >=620.0f)
           debug_Msg.IsCMD = 620.0f;
       if(debug_Msg.IsCMD <=-620.0f)
           debug_Msg.IsCMD = -620.0f;
//       gEdrive_Ctrl.Pwm_SwtFreq = (u16)canRecbuf->Data[7];
//       if(gEdrive_Ctrl.Pwm_SwtFreq == 0)
//           gEdrive_Ctrl.Pwm_SwtFreq = 100;
    }
}
//Idc = 1.5*((gAcr.UdFlted*gAcr.IdSet+gAcr.UqFlted*gAcr.IqSet)>>16)
void debugTransCanData(u16 transType)
{
    unsigned char* ucTXMsgData;
    u16 u16temp = 0;
    switch (transType)
    {
        case 0: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack' */
            can_Tx.ID = 200U;
            can_Tx.Length = 8U;
/*
//            u16temp=(u16)(gAcr.IdSet+10000);
            u16temp=(u16)(gResolver.raw_sin+10000);
            //u16temp=(u16)((SW_R2D_TO.PosEstTemp>>6)+10000);
            //u16temp=(u16)(gResolver.DCoffUpdateSin+10000);
            //u16temp=(u16)(gResolver.DCoffsetSIN+10000);
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
//            u16temp=(u16)(gAcr.IqSet+10000);
            u16temp=(u16)(gResolver.raw_cos+10000);
            //u16temp=(u16)(gResolver.DCoffUpdateCos+10000);
            //u16temp=(u16)(gResolver.DCoffsetCOS+10000);
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)(gAcr.UdFlted + +10000);//gAcr.UdFlted+10000);//(CurrentSet.Uerr+10000);//gAcr.UdFlted
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=(u16)(gAcr.UqFlted + +10000);//gAcr.UqFlted+10000);//(gAcr.UmagFltd+10000);//(gAcr.UqFlted+10000);
            can_Tx.Data[6] = u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_200_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 1: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack1' */
            can_Tx.ID = 201U;//swj 201
            can_Tx.Length = 8U;
/*            u16temp=(u16)(debug_Msg.IsCMD+1000.0F)*10.0f;
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
            u16temp=(u16)(debug_Msg.ThetaCMD+1000.0F)*10.0f;
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)(gMeas.I_d+10000);
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=(u16)(gMeas.I_q+10000);
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_23F_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 2: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
/*            can_Tx.ID = 202U;
            can_Tx.Length = 8U;
            u16temp=(u16)(gSpdRpm + 15000.0F);
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
           u16temp=(u16)(gPWM3ph.MI_fltd);//gPWM3ph.MI);
//            u16temp=(u16)(Tload);
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)((CurrentSet.IsThetaAply+1000.0F)*10.0f);
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=(u16)(((f32)gMeas.Udc*0.1f+1000.0F)*10.0f);
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_202_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 3: //
////            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 203U;
            can_Tx.Length = 8U;
/*            canfaulttest[0] = (u8)(0|(Err.OverVoltage<<1)|(Err.FO<<2)|(Err.IGBTOverHeat<<3)|(Err.MotorOverHeat<<4)|(Err.CanFault<<5)|(Err.DisChargeFail<<6)|(0<<7));
            canfaulttest[1] = (u8)(0|(Err.LowVoltage<<1)|(Err.RDC_Err<<2)|(Err.OverSpd<<3)|(Err.SelfCheckFail<<4)|(Err.IGBTTempSensorError<<5)|(Err.MotorTempSensorError<<6)|(Err.CurrentSensorError<<7));
            canfaulttest[2] = (u8)(Err.OverCurrent|(0<<1)|(Err.MotorStock<<2)|(Err.usStallWarning<<3)|(Err.CanInvalidErr<<4)|(Err.IGBTOverHeatWarning<<5)|(Err.MotorOverHeatWarning<<6)|(Err.OverSpdWarning<<7));
            canfaulttest[3] = (u8)(Err.OverVoltageWarning|(Err.LowVoltageWarning<<1)|(Err.V12OverVoltage<<2)|(Err.V12LowVoltage<<3)|(Err.V12OverVoltageWarning<<4)|(Err.V12LowVoltageWarning<<5)|(0<<6)|(0<<7));
            can_Tx.Data[0] = canfaulttest[0];//(u8)(0);
            can_Tx.Data[1] = canfaulttest[1];//(u8)(0);
            u16temp=(u16)(0+1000.0F)*10.0f;
            can_Tx.Data[2] = canfaulttest[2];//(u8)u16temp;
            can_Tx.Data[3] = canfaulttest[3];//(u8)(u16temp>>8);
            u16temp=(u16)(0+1000.0F)*10.0f;
            can_Tx.Data[4] = (u8)(gADC_Info.TempMotor + 50);
            can_Tx.Data[5] = (u8)(gADC_Info.TempIGBT + 50);
            can_Tx.Data[6] = (u8)Err.ErrState;
            can_Tx.Data[7] = (u8)gADC_Info.V12dc;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_203_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
//        case 4: //
//////            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
//            can_Tx.ID = 204U;
//            can_Tx.Length = 8U;
////            can_Tx.Data[0] = canfaulttest[0];
////            can_Tx.Data[1] = canfaulttest[1];
////            can_Tx.Data[2] = canfaulttest[2];
////            can_Tx.Data[3] = canfaulttest[3];
////            can_Tx.Data[4] = canfaulttest[4];
////            can_Tx.Data[5] = canfaulttest[5];
//            //            can_Tx.Data[6] = 0;
//            //            can_Tx.Data[7] = 0;
//            u16temp=(u16)(gPWM3ph.MI_fltd);
//            can_Tx.Data[6] = (u8)u16temp;
//            can_Tx.Data[7] = (u8)(u16temp>>8);
//              ucTXMsgData = (unsigned char*)can_Tx.Data;
//           CAN_sendMessage(CANA_BASE, MCU_204_msg+1, can_Tx.Length, (u16*)ucTXMsgData);
//            break;
        default:
            ;
            break;
     }
}


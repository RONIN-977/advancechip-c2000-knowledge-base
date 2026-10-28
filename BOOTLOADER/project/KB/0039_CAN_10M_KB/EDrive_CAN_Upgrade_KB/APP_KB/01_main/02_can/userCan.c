/*
 * userCan.c
 *
 *  Created on: 2022Äê3ÔÂ6ÈÕ
 *      Author: 86176
 */
#include "userCan.h"
#include "debugCan.h"
#include "canConfig.h"
//#include "Resolver_FaultDiag.h"   //123
#include "uds.h"
#include "config.h"
#include "NVM.h"
//#include <05_Input/cal_tse_ref.h>  //123
//#include <05_Input/input_signal.h>//123
//#include <05_Input/eeprom.h>//123

VCU_MSG_STRUCT vcu_Msg;
MCU_MSG_STRUCT mcu_Msg;
u16 Constant1_Value_i[8] = {0,1,2,3,4,5,6,7};
u8 VehicleModel=2;
u8 Version =1,SubVersion=9;
//u8 Date=12;
//u8 Month =5;
u8 Year=22,Month =5,Date=12;
u8 Factory =2;
//u8 SubVersion=9;
CAN_DATATYPE can_Rx;
CAN_DATATYPE can_Tx;
u16 CANTxCount = 0U;
extern uds_Tp_Status_S uds_Tp_Status;

//extern u8 flashwrite;

u8 calCrc(u8 *data,u8 len);
void uds_process(u8 *data);

u8 calCrc(u8 *data,u8 len)
{
    u32 temp = 0U;
    u8 i;
    u8 result;
    for(i = 0;i<len;i++)
    {
        temp+=data[i];
    }
    result = (u8)(temp^0xf);
    return result;
}

void CAN_Transmit_Task(void)
{
    CANTxCount++;
#if dbg_mode
    if (CANTxCount%5 == 1)
    {
        debugTransCanData(0);
    }
    if (CANTxCount%20 == 2)
    {
        debugTransCanData(1);
    }
    if (CANTxCount%20 == 3)
    {
        debugTransCanData(2);
    }
    if (CANTxCount%20 == 4)
    {
        debugTransCanData(3);
    }
//    if (CANTxCount%10 == 5)
//    {
//        debugTransCanData(4);
//    }
#else
    if (CANTxCount%10 == 1)
    {
        TransCanData(0);
    }
    if (CANTxCount%20 == 2)
    {
        TransCanData(1);
    }
    if (CANTxCount%5 == 3)
    {
        TransCanData(2);
    }
    if (CANTxCount%20 == 4)
    {
        TransCanData(3);
    }
    if (CANTxCount%5 == 2)
    {
        TransCanData(4);
    }
    if (CANTxCount%10 == 2)
    {
        TransCanData(5);
    }
    if (CANTxCount%20 == 3)
    {
        TransCanData(6);
    }
    if (CANTxCount%10 == 3)
    {
        TransCanData(7);
    }
#endif
}
void CAN_Receive_Task(void)
{
    unsigned char ucRXMsgData[8]= { 0, 0, 0, 0, 0, 0, 0, 0 };

    u16 status = 0;
    u16 i = 0;
    CAN_MsgFrameType frameType = 0;
    u32 messageID = 0;
#if 0
    status = CAN_readMessageWithID(CANA_BASE, 10, &frameType, &messageID,(u16*)ucRXMsgData);
    if (status > 0) {
        can_Rx.Data[0] = ucRXMsgData[0];
        can_Rx.Data[1] = ucRXMsgData[1];
        can_Rx.Data[2] = ucRXMsgData[2];
        can_Rx.Data[3] = ucRXMsgData[3];
        can_Rx.Data[4] = ucRXMsgData[4];
        can_Rx.Data[5] = ucRXMsgData[5];
        can_Rx.Data[6] = ucRXMsgData[6];
        can_Rx.Data[7] = ucRXMsgData[7];
        can_Rx.ID = messageID;

  //        ReceiveCanData(&can_Rx);
        debugReceiveCanData(&can_Rx);
    }
#else
        for(i=10;i<NUM_USERCAN_REC+CanBoxOffset;i++)
        {
            status = CAN_readMessageWithID(CANA_BASE, i, &frameType, &messageID,(u16*)ucRXMsgData);
              if (status > 0) {
                  if((messageID == Usercan_UDS721)||(messageID == Usercan_UDS7df))
                  {
                      uds_process(ucRXMsgData);
                      continue;
                  }

                  can_Rx.Data[0] = ucRXMsgData[0];
                  can_Rx.Data[1] = ucRXMsgData[1];
                  can_Rx.Data[2] = ucRXMsgData[2];
                  can_Rx.Data[3] = ucRXMsgData[3];
                  can_Rx.Data[4] = ucRXMsgData[4];
                  can_Rx.Data[5] = ucRXMsgData[5];
                  can_Rx.Data[6] = ucRXMsgData[6];
                  can_Rx.Data[7] = ucRXMsgData[7];
                  can_Rx.ID = messageID;
#if dbg_mode
                  debugReceiveCanData(&can_Rx);
#else
                  ReceiveCanData(&can_Rx);
#endif

              }
        }
#endif
}

void TransCanData(u16 transType)
{
    unsigned char* ucTXMsgData;
    u16 u16temp = 0;
    switch (transType)
    {
        case 0: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack' */
            can_Tx.ID = 130U;
            can_Tx.Length = 8U;

/*            u16temp=(u16)((TroqueFbk_Y.TrqFbk+3000.0F)*10.0f);//mcu_Msg.MCU_MotActuToq
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
            u16temp=(u16)(gSpdRpm+20000.0F);//mcu_Msg.MCU_MotActuSpd
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)((lookuptable_U.Trq+3000.0F)*10.0f);//mcu_Msg.MCU_MotToqLmt  Err.ErrTorLimt  lookuptable_U.Trq
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            if(StateMachine.MotWorkState == ShutDown_STATE)
                mcu_Msg.MCU_ShutdownEnable = 1;
            u16temp=(u16)((mcu_Msg.MCU_MotActuMd)|(mcu_Msg.MCU_ShutdownEnable<<4)|(StateMachine.MotWorkState<<5));//mcu_Msg.MCU_Sts
            can_Tx.Data[6] = (u8)u16temp;
            mcu_Msg.MCU_130_RollingCounter=((++mcu_Msg.MCU_130_RollingCounter)%16);
            mcu_Msg.MCU_130_CheckSum = calCrc(can_Tx.Data,7);
            u16temp=(u16)((mcu_Msg.MCU_130_CheckSum)|(mcu_Msg.MCU_130_RollingCounter<<4));
            can_Tx.Data[7] = (u8)u16temp;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_130_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 1: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack1' */
            can_Tx.ID = 132U;
            can_Tx.Length = 8U;

/*            can_Tx.Data[0] = (u8)(gADC_Info.TempMotor + 40);
            can_Tx.Data[1] = (u8)(gADC_Info.TempIGBT + 40);
            //Idc = 1.5*((gAcr.UdFlted*gAcr.IdSet+gAcr.UqFlted*gAcr.IqSet)>>16)
            u16temp=(u16)(((f32)gMeas.I_mag*0.625+10000.0f));//*10.0F);
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)((f32)gMeas.Udc);
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=(u16)((Err.ErrState)|(mcu_Msg.HVILClsdFlt<<3));
            can_Tx.Data[6] = (u8)u16temp;
            mcu_Msg.MCU_132_RollingCounter=((++mcu_Msg.MCU_132_RollingCounter)%16);
            mcu_Msg.MCU_132_CheckSum = calCrc(can_Tx.Data,7);
            u16temp=(u16)((mcu_Msg.MCU_132_CheckSum)|(mcu_Msg.MCU_132_RollingCounter<<4));
            can_Tx.Data[7] = (u8)u16temp;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_132_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 2: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 181U;
            can_Tx.Length = 8U;
/*            can_Tx.Data[0] = (u8)(0|(Err.OverVoltage<<1)|(Err.FO<<2)|(Err.IGBTOverHeat<<3)|(Err.MotorOverHeat<<4)|(Err.CanFault<<5)|(Err.DisChargeFail<<6)|(0<<7));
            can_Tx.Data[1] = (u8)(Err.EepromFault|(Err.LowVoltage<<1)|(Err.RDC_Err<<2)|(Err.OverSpd<<3)|(Err.SelfCheckFail<<4)|(Err.IGBTTempSensorError<<5)|(Err.MotorTempSensorError<<6)|(Err.CurrentSensorError<<7));
            can_Tx.Data[2] = (u8)(Err.OverCurrent|(0<<1)|(Err.MotorStock<<2)|(Err.usStallWarning<<3)|(Err.CanInvalidErr<<4)|(Err.IGBTOverHeatWarning<<5)|(Err.MotorOverHeatWarning<<6)|(Err.OverSpdWarning<<7));
            can_Tx.Data[3] = (u8)(Err.OverVoltageWarning|(Err.LowVoltageWarning<<1)|(Err.V12OverVoltage<<2)|(Err.V12LowVoltage<<3)|(Err.V12OverVoltageWarning<<4)|(Err.V12LowVoltageWarning<<5)|(Err.PedalFault<<6)|(0<<7));
            can_Tx.Data[4] = (u8)(gRes_Diag.Exc_ErrFlg|(gRes_Diag.Cos_ErrFlg<<1)|(gRes_Diag.Sin_ErrFlg<<2)|(Err.CanTimeOut<<3)|(Err.CanBusOff<<4)|(0<<5)|(0<<6)|(0<<7));
            can_Tx.Data[5] = (u8)(0|(0<<1)|(0<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6)|(0<<7));
            if (Err.ErrState != 0 && EepromCtrl.FlashErr == 0)
            {
                EepromCtrl.FlashErr = 1;
                EepromCtrl.FlashErr0 = can_Tx.Data[0];
                EepromCtrl.FlashErr1 = can_Tx.Data[1];
                EepromCtrl.FlashErr2 = can_Tx.Data[2];
                EepromCtrl.FlashErr3 = can_Tx.Data[3];
                EepromCtrl.FlashErr4 = can_Tx.Data[4];
                EepromCtrl.FlashErr5 = can_Tx.Data[5];
                EepromCtrl.FlashErr6 = 0;
                EepromCtrl.FlashErr7 = 0;
            }
            if (vcu_Msg.VCU_FaultQuery != 0)
            {
                can_Tx.Data[0] = EepromCtrl.FlashQuery0;
                can_Tx.Data[1] = EepromCtrl.FlashQuery1;
                can_Tx.Data[2] = EepromCtrl.FlashQuery2;
                can_Tx.Data[3] = EepromCtrl.FlashQuery3;
                can_Tx.Data[4] = EepromCtrl.FlashQuery4;
                can_Tx.Data[5] = EepromCtrl.FlashQuery5;
                //can_Tx.Data[6] = EepromCtrl.FlashQuery6;
            }
            u16temp += DivCount(can_Tx.Data[0] & 0x00FF);
            u16temp += DivCount(can_Tx.Data[1] & 0x00FF);
            u16temp += DivCount(can_Tx.Data[2] & 0x00FF);
            u16temp += DivCount(can_Tx.Data[3] & 0x00FF);
            u16temp += DivCount(can_Tx.Data[4] & 0x00FF);
            u16temp += DivCount(can_Tx.Data[5] & 0x00FF);
            can_Tx.Data[6] = (u8)u16temp;
            mcu_Msg.MCU_181_RollingCounter=((++mcu_Msg.MCU_181_RollingCounter)%16);
            mcu_Msg.MCU_181_CheckSum = calCrc(can_Tx.Data,7);
            u16temp=(u16)((mcu_Msg.MCU_181_CheckSum)|(mcu_Msg.MCU_181_RollingCounter<<4));
            can_Tx.Data[7] = (u8)u16temp;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_181_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 3: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 631U;
            can_Tx.Length = 8U;

            can_Tx.Data[0] = (u8)VehicleModel;
            can_Tx.Data[1] = (u8)Version;

            can_Tx.Data[2] = (u8)Date;
            can_Tx.Data[3] = (u8)Month;

            can_Tx.Data[4] = (u8)Year;
            can_Tx.Data[5] = (u8)Factory;

            can_Tx.Data[6] = (u8)SubVersion;
            mcu_Msg.MCU_631_RollingCounter=((++mcu_Msg.MCU_631_RollingCounter)%16);
            mcu_Msg.MCU_631_CheckSum = calCrc(can_Tx.Data,7);
            u16temp=(u8)((mcu_Msg.MCU_631_CheckSum)|(mcu_Msg.MCU_631_RollingCounter<<4));
            can_Tx.Data[7] = (u8)u16temp;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_631_msg+1, can_Tx.Length, (u16*)ucTXMsgData);
            break;
        case 4: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 111U;
            can_Tx.Length = 8U;
/*            u16temp=(u16)(gMeas.I_d+10000);
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
            u16temp=(u16)(gMeas.I_q+10000);//gMeas.I_q  gResolver.DCoffsetCOS
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)(gAcr.IdSet+10000);//gAcr.IdSet  gAcr.UdFlted  gResolver.DCoffsetSIN
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=(u16)(gAcr.IqSet+10000);//gAcr.IqSet+10000 gAcr.UqFlted  gResolver.DCoffsetEXC  TseRefCtrl.fIdcFbk
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_111_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 5: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 112U;
            can_Tx.Length = 8U;
/*            u16temp=(u16)((TseRefCtrl.fTseBufferNew+3000.0F)*10.0f);
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)(u16temp>>8);
            u16temp=(u16)(gResolver.DCoffsetCOS+10000);
            can_Tx.Data[2] = (u8)u16temp;
            can_Tx.Data[3] = (u8)(u16temp>>8);
            u16temp=(u16)(gResolver.DCoffsetSIN+10000);
            can_Tx.Data[4] = (u8)u16temp;
            can_Tx.Data[5] = (u8)(u16temp>>8);
            u16temp=0;//(u16)(gResolver.DCoffsetEXC+10000);
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_112_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 6: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 101U;
            can_Tx.Length = 8U;
/*            can_Tx.Data[0] = (u8)(InSigCtrl.ucForFlag|(InSigCtrl.ucRevFlag<<1)|(InSigCtrl.ucBrakeFlag<<2)|(InSigCtrl.ucEcoFlag<<3)|(InSigCtrl.ucStaFlag<<4)|(InSigCtrl.ucOpenFlag<<5)|(0<<6)|(0<<7));
            can_Tx.Data[1] = (u8)(0|(0<<1)|(0<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6)|(0<<7));
            can_Tx.Data[2] = (u8)(0|(0<<1)|(0<<2)|(Err.SelfCheck<<3)|(0<<5)|(0<<6)|(0<<7));
            can_Tx.Data[3] = (u8)(0|(0<<1)|(0<<2)|(0<<3)|(0<<4)|(0<<5)|(0<<6)|(0<<7));
            can_Tx.Data[4] = (u8)gADC_Info.V12dc;
            can_Tx.Data[5] = (u8)(((u32)SpdAdcCtrl.uiSpeedValue * 50) >> 12);
            u16temp=(u16)((TseRefCtrl.fIdcFbk+3000.0F)*10.0f);
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_101_msg+1, can_Tx.Length, (u16*)ucTXMsgData);123*/
            break;
        case 7: //
            /* S-Function (scan_Tx): '<S3>/CAN Pack2' */
            can_Tx.ID = 118U;
            can_Tx.Length = 8U;
            if (mcu_Msg.MCU_DirecEn == 0)      u16temp = 3; // 1 rev 2 Neu 3 Dri  4 Spo 5 Park
            else if (mcu_Msg.MCU_DirecEn == 1) u16temp = 1;
            else                               u16temp = 2;
            can_Tx.Data[0] = (u8)u16temp;
            can_Tx.Data[1] = (u8)0x00;
            can_Tx.Data[2] = (u8)0x00;
            can_Tx.Data[3] = (u8)0x00;
            can_Tx.Data[4] = (u8)0x00;
            can_Tx.Data[5] = (u8)0x00;
//            u16temp=(u16)(gPWM3ph.MI_fltd);//123
            can_Tx.Data[6] = (u8)u16temp;
            can_Tx.Data[7] = (u8)(u16temp>>8);
            //can_Tx.Data[6] = (u8)0x00;
            //can_Tx.Data[7] = (u8)0x00;
            ucTXMsgData = (unsigned char*)can_Tx.Data;
            CAN_sendMessage(CANA_BASE, MCU_118_msg+1, can_Tx.Length, (u16*)ucTXMsgData);
            break;
        default:
            ;
            break;
     }
}
void ReceiveCanData(CAN_DATATYPE *canRecbuf)
{
//    unsigned char* ucTXMsgData;
//    u16 u16temp = 0;
    if(canRecbuf->ID == 0x119)
    {
       vcu_Msg.VCU_VehDrvMdReq = (canRecbuf->Data[0] & 0x7);
       vcu_Msg.VCU_MotSopCmd =  (canRecbuf->Data[0] & 0x18)>>3;
       vcu_Msg.VCU_ActDcrhgCmd = (canRecbuf->Data[0] & 0x20)>>5;
       vcu_Msg.VCU_ActPowerDownReq = (canRecbuf->Data[0] & 0x40)>>6;
       vcu_Msg.VCU_MotToqReq = ((f32)((u16)canRecbuf->Data[3]*256+canRecbuf->Data[2])-30000)*0.1f;
       vcu_Msg.VCU_MotSpdReq = ((f32)((u16)canRecbuf->Data[5]*256+canRecbuf->Data[4])-20000);
       vcu_Msg.VCU_FaultQuery = (u8)canRecbuf->Data[6];
//       gEdrive_Ctrl.Pwm_SwtFreq = (u16)canRecbuf->Data[6];
//       if(gEdrive_Ctrl.Pwm_SwtFreq == 0)
//           gEdrive_Ctrl.Pwm_SwtFreq = 100;
       vcu_Msg.VCU_119_CheckSum = (canRecbuf->Data[7] & 0xF);
       vcu_Msg.VCU_119_RollingCounter = (canRecbuf->Data[7] & 0xF0)>>4;
//       calCrc(canRecbuf->Data,7);
//       CanFault.canRecNewMsgFlag = 1;//123
//       checkChesum(VCU_119_msg,calCrc(canRecbuf->Data,7),vcu_Msg.VCU_119_CheckSum);
//       checkRollingCnt(VCU_119_msg,vcu_Msg.VCU_119_RollingCounter);
    }
    else if(canRecbuf->ID == 0x118)
    {
        vcu_Msg.VCU_DirecEn = (canRecbuf->Data[0] & 0x7);
        vcu_Msg.VCU_118_CheckSum = (canRecbuf->Data[7] & 0xF);
        vcu_Msg.VCU_118_RollingCounter = (canRecbuf->Data[7] & 0xF0)>>4;
//       calCrc(canRecbuf->Data,7);
//        CanFault.canRecNewMsgFlag = 1;//123
//       checkChesum(VCU_119_msg,calCrc(canRecbuf->Data,7),vcu_Msg.VCU_119_CheckSum);
//       checkRollingCnt(VCU_119_msg,vcu_Msg.VCU_119_RollingCounter);
    }
}



char sysname[16] ="A00";//"xx_xx_xx";
char SWVer[10] ="1.00.10";
void UDS_setDirectBootCb(void)
{
    Bootloader_Configuration config;

    config.bootDirectFlags = BOOTLOADER_PROG_REQ;
    config_set((u8 *)&config, 0, BL_CONFIG_BOOT_FLAG);
}

void uds_process(u8 *data)
{
    u8 txreq=0;
    u8 i;
    unsigned char* ucTXMsgData;
    if((data[0] == 0x03)&&(data[1] == 0x22))
    {
        can_Tx.ID = 0x721;
        can_Tx.Length = 8U;
        uds_Tp_Status.StateMachine = 1;
        if((data[2] == 0xf1)&&(data[3] == 0x97))
        {
            uds_Tp_Status.DIDIndex = 0;
            uds_Tp_Status.sendTotalNum = 16;
            uds_Tp_Status.sendIndex = 3;
            uds_Tp_Status.TimeoutCnt = 500;
            for(i=0;i<uds_Tp_Status.sendTotalNum;i++)
            {
                uds_Tp_Status.txdata[i]=(u8)sysname[i];
            }
            can_Tx.Data[0] = 0x10;
            can_Tx.Data[1] = (u8)(uds_Tp_Status.sendTotalNum+3);
            can_Tx.Data[2] = (u8)0x62;
            can_Tx.Data[3] = (u8)0xf1;
            can_Tx.Data[4] = (u8)0x97;
            can_Tx.Data[5] = (u8)uds_Tp_Status.txdata[0];
            can_Tx.Data[6] = (u8)uds_Tp_Status.txdata[1];
            can_Tx.Data[7] = (u8)uds_Tp_Status.txdata[2];
            txreq = 1;
        }
        else if((data[2] == 0xf1)&&(data[3] == 0x89))
        {
            uds_Tp_Status.DIDIndex = 0;
            uds_Tp_Status.sendTotalNum = 0x0a;
            uds_Tp_Status.sendIndex = 3;
            uds_Tp_Status.TimeoutCnt = 500;
            for(i=0;i<uds_Tp_Status.sendTotalNum;i++)
            {
                uds_Tp_Status.txdata[i]=(u8)SWVer[i];
            }
            can_Tx.Data[0] = 0x10;
            can_Tx.Data[1] = (u8)(uds_Tp_Status.sendTotalNum+3);
            can_Tx.Data[2] = (u8)0x62;
            can_Tx.Data[3] = (u8)0xf1;
            can_Tx.Data[4] = (u8)0x89;
            can_Tx.Data[5] = (u8)uds_Tp_Status.txdata[0];
            can_Tx.Data[6] = (u8)uds_Tp_Status.txdata[1];
            can_Tx.Data[7] = (u8)uds_Tp_Status.txdata[2];
            txreq = 1;
        }
        else if((data[2] == 0xf1)&&(data[3] == 0x50))
        {
//            uds_Tp_Status.DIDIndex = 0;
//            uds_Tp_Status.sendTotalNum = 0x0d;
//            uds_Tp_Status.sendIndex = 3;
//            uds_Tp_Status.TimeoutCnt = 500;
//            for(i=0;i<uds_Tp_Status.sendTotalNum;i++)
//            {
//                uds_Tp_Status.txdata[i]=(u8)sysname[i];
//            }
            can_Tx.Data[0] = 0x03;
            can_Tx.Data[1] = (u8)(0x7f);
            can_Tx.Data[2] = (u8)0x22;
            can_Tx.Data[3] = (u8)0x31;
            can_Tx.Data[4] = (u8)0x89;
            can_Tx.Data[5] = (u8)uds_Tp_Status.txdata[0];
            can_Tx.Data[6] = (u8)uds_Tp_Status.txdata[1];
            can_Tx.Data[7] = (u8)uds_Tp_Status.txdata[2];
            txreq = 1;
        }

    }


    if((data[1] == 0x85)&&(data[2] == 0x02))
    {
        can_Tx.Data[0] = 0x02;
        can_Tx.Data[1] = (u8)(0xc5);
        can_Tx.Data[2] = (u8)0x02;
        can_Tx.Data[3] = (u8)0x31;
        can_Tx.Data[4] = (u8)0x89;
        can_Tx.Data[5] = (u8)0xff;
        can_Tx.Data[6] = (u8)0xff;
        can_Tx.Data[7] = (u8)0xff;
        txreq = 1;
    }
    else if((data[1] == 0x28)&&(data[2] == 0x03)&&(data[3] == 0x01))
    {
        can_Tx.Data[0] = 0x02;
        can_Tx.Data[1] = (u8)(0x68);
        can_Tx.Data[2] = (u8)0x03;
        can_Tx.Data[3] = (u8)0xff;
        can_Tx.Data[4] = (u8)0xff;
        can_Tx.Data[5] = (u8)0xff;
        can_Tx.Data[6] = (u8)0xff;
        can_Tx.Data[7] = (u8)0xff;
        txreq = 1;
    }


    if((data[0] == 0x02)&&(data[1] == 0x10)&&(data[2] == 0x02))
    {
        DisableDog();
        UDS_setDirectBootCb();
        EALLOW;
        WdRegs.WDCR.all = 0x0028;
        EDIS;
        can_Tx.Data[0] = 0x06;
        can_Tx.Data[1] = (u8)0x50;
        can_Tx.Data[2] = (u8)0x02;
        can_Tx.Data[3] = (u8)0xff;
        can_Tx.Data[4] = (u8)0xff;
        can_Tx.Data[5] = (u8)0xff;
        can_Tx.Data[6] = (u8)0xff;
        can_Tx.Data[7] = (u8)0xff;
        txreq = 2;
    }
   if((data[0] == 0x02)&&(data[1] == 0x10)&&(data[2] == 0x03))
   {
       can_Tx.Data[0] = 0x06;
       can_Tx.Data[1] = (u8)0x50;
       can_Tx.Data[2] = (u8)0x03;
       can_Tx.Data[3] = (u8)0xff;
       can_Tx.Data[4] = (u8)0xff;
       can_Tx.Data[5] = (u8)0xff;
       can_Tx.Data[6] = (u8)0xff;
       can_Tx.Data[7] = (u8)0xff;
       txreq = 1;
   }

    if((data[0] == 0x30)&&(data[1] == 0x00)&&(data[2] == 0x00)&&(uds_Tp_Status.StateMachine == 1))//flowControl
    {
        uds_Tp_Status.StateMachine = 2;
    }
    if(txreq > 0)
    {
        ucTXMsgData = (unsigned char*)can_Tx.Data;
        CAN_sendMessage(CANA_BASE, MCU_729_msg+1, can_Tx.Length, (u16*)ucTXMsgData);
        if(txreq == 2)
        {
            SysCtl_resetDevice();
        }
    }
}





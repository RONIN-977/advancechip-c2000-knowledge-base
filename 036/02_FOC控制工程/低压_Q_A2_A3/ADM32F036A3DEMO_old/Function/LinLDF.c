///************************************************************************
// Project:             Automotive water pumps
// Filename:            LDF.c
// Partner Filename:    N/A
// Description:         Partner file of LDF.h
// Complier:            Code Composer Studio for ADM32F036, CCS Systems.
// CPU TYPE :           ADM32F036A2
//*************************************************************************
// Copyright (c) 2024 **** Co., Ltd.
// All rights reserved.
//*************************************************************************
//*************************************************************************
// Revising History (ECL of this file):
//
//************************************************************************/
//
///************************************************************************
// Beginning of File, do not put anything above here except notes
// Compiler Directives
//*************************************************************************/
//
//#include "LinLDF.h"
//#include "Main.h"
//#include "MotorInclude.h"
//#include "Parameter.h"
//
/////////////////////////////////////////////////////////////////////////////////////////
//// File scope Definition and constant
/////////////////////////////////////////////////////////////////////////////////////////
//
//Lin_COMMUNICATION Lin_Communication;
//TY_Lin_COMMUNICATION TY_Lin_Communication;
//Uint16 ReciveDataIDCorrect[8];
//Uint16 ReciveDataIDInCorrect[8];
//LIN_DATA0 SenddDataA;
//LIN_DATA1 SenddDataB;
//
///***************************************************************
// Function: ReveiveData;
// Description: Put the received data into the corresponding variables
// Call by: functions CommsProtocalLayerTask2ms which is in main loop;
// Input Variables: N/A
// Output/Return Variables: N/A
// Subroutine Call: N/A;
// Reference: N/A
///****************************************************************/
////void UpdateRecivedLinDataToVars(void)
////{
////    /* ------------------------
////    PDCS_EWP1_Fr4:0x31,PDCS,3{
////        EcwpSafeMod,16;
////        EcwpSpdReq,0;
////        EcwpEna,18;
////    }
////    -------------------------*/
////    Lin_Communication.EcwpSpdReq   = (ReciveDataIDCorrect[1] << 8) | ReciveDataIDCorrect[0];
////    Lin_Communication.EcwpSafeMode = (ReciveDataIDCorrect[2] & 0x03);
////    Lin_Communication.EcwpEna      = (ReciveDataIDCorrect[2] & 0x04) >> 2;
////}
//void UpdateRecivedLinDataToVars(void)
//{
//    /* ------------------------
//    PDCS_EWP1_Fr4:0x31,PDCS,3{
//        EcwpSafeMod,16;
//        EcwpSpdReq,0;
//        EcwpEna,18;
//    }
//    -------------------------*/
//    TY_Lin_Communication.EcwpSetSpeed   = (ReciveDataIDCorrect[1] << 8) | ReciveDataIDCorrect[0];
//    RefSet = TY_Lin_Communication.EcwpSetSpeed;
//    TY_Lin_Communication.EcwpPostRunning = (ReciveDataIDCorrect[2] & 0x70)>>4;
//
//}
///***************************************************************
// Function: ReveiveData;
// Description: Put the the corresponding variables into send data
// Call by: functions CommsProtocalLayerTask2ms which is in main loop;
// Input Variables: N/A
// Output/Return Variables: N/A
// Subroutine Call: N/A;
// Reference: N/A
//****************************************************************/
////void UpdateLinDataToSent(void)
////{
////    SenddDataA.bit.TD0  = Lin_Communication.EcwpActSpd;
////    SenddDataA.bit.TD1  = Lin_Communication.EcwpActSpd>>8;
////    SenddDataA.bit.TD2  = Lin_Communication.EcwpLoVolt;
////    SenddDataA.bit.TD3  = Lin_Communication.EcwpBdT;
////
////    SenddDataB.bit.TD4  = Lin_Communication.EcwpActCur;
////    SenddDataB.bit.TD5  = Lin_Communication.EcwpActCur >> 8;
////    SenddDataB.bit.TD6  = (Lin_Communication.EcwpOverCurErr)        |
////                          (Lin_Communication.EcwpDryRunErr << 2)    |
////                          (Lin_Communication.EcwpVoltErr   << 4)    |
////                          (Lin_Communication.EcwpBlkErr    << 6);
////    SenddDataB.bit.TD7  = (Lin_Communication.EcwpTempErr)           |
////                          (Lin_Communication.EcwpRespErr << 7);
////}
//void UpdateLinDataToSent(void)
//{
//    //SenddDataA.bit.TD0  = TY_Lin_Communication.EcwpSetSpeed;
//    SenddDataA.bit.TD0  = TY_Lin_Communication.EcwpActSpeed;
//    SenddDataA.bit.TD1  = TY_Lin_Communication.EcwpActSpeed>>8;
//    SenddDataA.bit.TD2  = TY_Lin_Communication.EcwpLoVolt;
//    SenddDataA.bit.TD3  = TY_Lin_Communication.EcwpBdT;
//
//    SenddDataB.bit.TD4  = TY_Lin_Communication.EcwpActCur;
//    SenddDataB.bit.TD5  = (TY_Lin_Communication.EcwpPREnd)             |
//                          (TY_Lin_Communication.EcwpRPMLow << 2)       |
//                          (TY_Lin_Communication.EcwpPWMMOD << 4)       |
//                          (TY_Lin_Communication.EcwpTempErr   << 6);
//    SenddDataB.bit.TD6  = (TY_Lin_Communication.EcwpOverCurErr)        |
//                          (TY_Lin_Communication.EcwpDryRunErr << 2)    |
//                          (TY_Lin_Communication.EcwpVoltErr << 4)      |
//                          (TY_Lin_Communication.EcwpBlkErr << 6);
//    SenddDataB.bit.TD7  = (TY_Lin_Communication.EcwpNodErr << 2)       |
//                          (TY_Lin_Communication.EcwpPostRunning << 4)  |
//                          (TY_Lin_Communication.EcwpRespErr << 7);
//}
///* -----------------------------------------------------------
//EWP1_1_Message:0x36,EWP1,8{
//    EcwpActSpd,0;
//    EcwpLoVolt,16;
//    EcwpBdT,24;
//    EcwpActCur,32;
//    EcwpOverCurErr,48;
//    EcwpDryRunErr,50;
//    EcwpVoltErr,52;
//    EcwpBlkErr,54;
//    EcwpTempErr,56;
//    EcwpRespErr,63;
//}
//------------------------------------------------------------*/
////void UpdateLinDataToVar(void)
////{
////    /*反馈频率，单位：0.1hz*/
////    Lin_Communication.EcwpActSpd            = FbackFry;
////
////    /*反馈电压，单位：0.1V*/
////    Lin_Communication.EcwpLoVolt            = gUDC.uDCFilter;
////
////    /*反馈温度，单位：*/
////    Lin_Communication.EcwpBdT               = InternalTemperatureDegree;
////
////    /*反馈相电流有效值，单位：0.1A*/
////    Lin_Communication.EcwpActCur            = ((Uint32)gLineCur.CurPer*MOTOR_RATED_CUR)>>12;
////
////    /*故障反馈*/
////    if (gMainStatus.ErrorCode               == ERROR_OC_HARDWARE) {
////        Lin_Communication.EcwpOverCurErr    = 1;
////    } else {
////        Lin_Communication.EcwpOverCurErr    = 0;
////    }
////    if (gMainStatus.ErrorCode               == ERROR_OV_ACC_SPEED ||
////        gMainStatus.ErrorCode               == ERROR_UV) {
////        Lin_Communication.EcwpVoltErr       = 1;
////    } else {
////        Lin_Communication.EcwpVoltErr       = 0;
////    }
////    if (gMainStatus.ErrorCode               == ERROR_STALL) {
////        Lin_Communication.EcwpBlkErr        = 1;
////    } else {
////        Lin_Communication.EcwpBlkErr        = 0;
////    }
////    if (gMainStatus.ErrorCode               == ERROR_LIN) {
////        Lin_Communication.EcwpRespErr       = 1;
////    } else {
////        Lin_Communication.EcwpRespErr       = 0;
////    }
////    if (gMainStatus.ErrorCode               == ERROR_NTC) {
////        Lin_Communication.EcwpTempErr       = 1;
////    } else {
////        Lin_Communication.EcwpTempErr       = 0;
////    }
////    if (gMainStatus.ErrorCode               == ERROR_NO_LOAD) {
////        Lin_Communication.EcwpDryRunErr     = 1;
////    } else {
////        Lin_Communication.EcwpDryRunErr     = 0;
////    }
////}
//void UpdateLinDataToVar(void)
//{
//    /*反馈频率，单位：0.1hz*/
//    TY_Lin_Communication.EcwpActSpeed            = (FbackFry*20);
//
//    /*反馈电压，单位：0.1V*/
//    TY_Lin_Communication.EcwpLoVolt            = gUDC.uDCFilter;
//
//    /*反馈温度，单位：*/
//    TY_Lin_Communication.EcwpBdT               = InternalTemperatureDegree;
//
//    /*反馈相电流有效值，单位：0.1A*/
//    TY_Lin_Communication.EcwpActCur            = ((Uint32)gLineCur.CurPer*MOTOR_RATED_CUR)>>12;
//
//    /*故障反馈*/
//    if (gMainStatus.ErrorCode               == ERROR_OC_HARDWARE) {
//        TY_Lin_Communication.EcwpOverCurErr    = 1;
//    } else {
//        TY_Lin_Communication.EcwpOverCurErr    = 0;
//    }
//    if (gMainStatus.ErrorCode               == ERROR_OV_ACC_SPEED ||
//        gMainStatus.ErrorCode               == ERROR_UV) {
//        TY_Lin_Communication.EcwpVoltErr       = 1;
//    } else {
//        TY_Lin_Communication.EcwpVoltErr       = 0;
//    }
//    if (gMainStatus.ErrorCode               == ERROR_STALL) {
//        TY_Lin_Communication.EcwpBlkErr        = 1;
//    } else {
//        TY_Lin_Communication.EcwpBlkErr        = 0;
//    }
//    if (gMainStatus.ErrorCode               == ERROR_LIN) {
//        TY_Lin_Communication.EcwpRespErr       = 1;
//    } else {
//        TY_Lin_Communication.EcwpRespErr       = 0;
//    }
//    if (gMainStatus.ErrorCode               == ERROR_NTC) {
//        TY_Lin_Communication.EcwpTempErr       = 1;
//    } else {
//        TY_Lin_Communication.EcwpTempErr       = 0;
//    }
//    if (gMainStatus.ErrorCode               == ERROR_NO_LOAD) {
//        TY_Lin_Communication.EcwpDryRunErr     = 1;
//    } else {
//        TY_Lin_Communication.EcwpDryRunErr     = 0;
//    }
//}
////void LinData_init(void)
////{
////    Lin_Communication.EcwpSpdReq            = 0;
////    Lin_Communication.EcwpSafeMode          = 0;
////    Lin_Communication.EcwpEna               = 0;
////}
//void LinData_init(void)
//{
//    TY_Lin_Communication.EcwpSetSpeed            = 0;
//    TY_Lin_Communication.EcwpPostRunning         = 0;
//    //TY_Lin_Communication.EcwpEna               = 0;
//}
////unsigned short GetSetupFreComs(void)
////{
////    unsigned short ReturnValue;
////    ReturnValue = Lin_Communication.EcwpSpdReq;
////    return ReturnValue;
////}
//unsigned short GetSetupFreComs(void)
//{
//    unsigned short ReturnValue;
//    ReturnValue = TY_Lin_Communication.EcwpSetSpeed;
//    return ReturnValue;
//}
////unsigned short GetStartCMDComs(void)
////{
////    return Lin_Communication.EcwpEna;
////}
///*************************************************************************
// End of this File (EOF)!
// Do not put anything after this part!
//*************************************************************************/
#include "LinLDF.h"
#include "Main.h"
#include "MotorInclude.h"
//#include "param_lr.h"
///////////////////////////////////////////////////////////////////////////////////////
// File scope Definition and constant
///////////////////////////////////////////////////////////////////////////////////////

Lin_COMMUNICATION Lin_Communication;
Uint16 ReciveDataIDCorrect[8];
Uint16 ReciveDataIDInCorrect[8];
LIN_DATA0 SenddDataA;
LIN_DATA1 SenddDataB;
extern volatile unsigned int RefSet;
/***************************************************************
 Function: ReveiveData;
 Description: Put the received data into the corresponding variables
 Call by: functions CommsProtocalLayerTask2ms which is in main loop;
 Input Variables: N/A
 Output/Return Variables: N/A
 Subroutine Call: N/A;
 Reference: N/A
****************************************************************/
void UpdateRecivedLinDataToVars(void)
{
    /* ------------------------
    PDCS_EWP1_Fr4:0x31,PDCS,3{
        EcwpSafeMod,16;
        EcwpSpdReq,0;
        EcwpEna,18;
    }
    -------------------------*/
    Uint32 speed;
    speed = (ReciveDataIDCorrect[1] << 8) | ReciveDataIDCorrect[0];
    Lin_Communication.EcwpSpdReq   = speed*10/20;
    RefSet = Lin_Communication.EcwpSpdReq;
    Lin_Communication.EcwpSafeMode = (ReciveDataIDCorrect[2] & 0x03);
    Lin_Communication.EcwpEna      = (ReciveDataIDCorrect[2] & 0x04) >> 2;
}

/***************************************************************
 Function: ReveiveData;
 Description: Put the the corresponding variables into send data
 Call by: functions CommsProtocalLayerTask2ms which is in main loop;
 Input Variables: N/A
 Output/Return Variables: N/A
 Subroutine Call: N/A;
 Reference: N/A
****************************************************************/
void UpdateLinDataToSent(void)
{
    SenddDataA.bit.TD0  = Lin_Communication.EcwpActSpd;
    SenddDataA.bit.TD1  = Lin_Communication.EcwpActSpd>>8;
    SenddDataA.bit.TD2  = Lin_Communication.EcwpLoVolt;
    SenddDataA.bit.TD3  = Lin_Communication.EcwpLoVolt>>8;
    SenddDataB.bit.TD4  = Lin_Communication.EcwpBdT;

    SenddDataB.bit.TD5  = Lin_Communication.EcwpActCur;
    SenddDataB.bit.TD6  = Lin_Communication.EcwpActPCur ;
    SenddDataB.bit.TD7  = (Lin_Communication.EcwpOverCurErr)        |
                          (Lin_Communication.EcwpDryRunErr << 1)    |
                          (Lin_Communication.EcwpVoltErr   << 2)    |
                          (Lin_Communication.EcwpBlkErr    << 3)    |
                          (Lin_Communication.EcwpTempErr   << 4)    |
                          (Lin_Communication.EcwpRespErr   << 5);
}
/* -----------------------------------------------------------
EWP1_1_Message:0x36,EWP1,8{
    EcwpActSpd,0;
    EcwpLoVolt,16;
    EcwpBdT,24;
    EcwpActCur,32;
    EcwpOverCurErr,48;
    EcwpDryRunErr,50;
    EcwpVoltErr,52;
    EcwpBlkErr,54;
    EcwpTempErr,56;
    EcwpRespErr,63;
}
------------------------------------------------------------*/
void UpdateLinDataToVar(void)
{
    /*反馈频率，单位：0.1hz*/

    Lin_Communication.EcwpActSpd            = pm_est_omg/80;

    /*反馈电压，单位：0.1V*/
    Lin_Communication.EcwpLoVolt            = gUDC.uDCFilter;

    /*反馈温度，单位：*/
    Lin_Communication.EcwpBdT               = InternalTemperatureDegree;

    /*反馈母线电流有效值，单位：0.1A*/
    Lin_Communication.EcwpActCur            = 0;//OutPutPower/(gUDC.uDCFilter/10);

    /*反馈相电流有效值，单位：0.1A*/
    Lin_Communication.EcwpActPCur            = gLineCur.CurPer*3;

    /*故障反馈*/
    if (gMainStatus.ErrorCode               == ERROR_OC_HARDWARE) {
        Lin_Communication.EcwpOverCurErr    = 1;
    } else {
        Lin_Communication.EcwpOverCurErr    = 0;
    }
    if (gMainStatus.ErrorCode               == ERROR_OV_ACC_SPEED ||
        gMainStatus.ErrorCode               == ERROR_UV) {
        Lin_Communication.EcwpVoltErr       = 1;
    } else {
        Lin_Communication.EcwpVoltErr       = 0;
    }
    if (gMainStatus.ErrorCode               == ERROR_STALL) {
        Lin_Communication.EcwpBlkErr        = 1;
    } else {
        Lin_Communication.EcwpBlkErr        = 0;
    }
    if (gMainStatus.ErrorCode               == ERROR_NTC) {
        Lin_Communication.EcwpTempErr       = 1;
    } else {
        Lin_Communication.EcwpTempErr       = 0;
    }
}

void LinData_init(void)
{
    Lin_Communication.EcwpSpdReq            = 0;
    Lin_Communication.EcwpSafeMode          = 0;
    Lin_Communication.EcwpEna               = 0;
}

unsigned short GetSetupFreComs(void)
{
    unsigned short ReturnValue;
    ReturnValue = Lin_Communication.EcwpSpdReq;
    return ReturnValue;
}

unsigned short GetStartCMDComs(void)
{
    return Lin_Communication.EcwpEna;
}

/*************************************************************************
 End of this File (EOF)!
 Do not put anything after this part!
*************************************************************************/

/************************************************************************
 Project:             Automotive water pumps
 Filename:            LDF.c
 Partner Filename:    N/A
 Description:         Partner file of LDF.h
 Complier:            Code Composer Studio for ADM32F036, CCS Systems.
 CPU TYPE :           ADM32F036A2
*************************************************************************
 Copyright (c) 2024 **** Co., Ltd.
 All rights reserved.
*************************************************************************
*************************************************************************
 Revising History (ECL of this file):

************************************************************************/

/************************************************************************
 Beginning of File, do not put anything above here except notes
 Compiler Directives
*************************************************************************/

#include "LinLDF.h"
#include "Main.h"
#include "MotorInclude.h"
#include "Parameter.h"

///////////////////////////////////////////////////////////////////////////////////////
// File scope Definition and constant
///////////////////////////////////////////////////////////////////////////////////////

Lin_COMMUNICATION Lin_Communication;
Uint16 ReciveDataIDCorrect[8];
Uint16 ReciveDataIDInCorrect[8];
LIN_DATA0 SenddDataA;
LIN_DATA1 SenddDataB;

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
    Lin_Communication.EcwpSpdReq   = (ReciveDataIDCorrect[1] << 8) | ReciveDataIDCorrect[0];
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
    SenddDataA.bit.TD3  = Lin_Communication.EcwpBdT;

    SenddDataB.bit.TD4  = Lin_Communication.EcwpActCur;
    SenddDataB.bit.TD5  = Lin_Communication.EcwpActCur >> 8;
    SenddDataB.bit.TD6  = (Lin_Communication.EcwpOverCurErr)        |
                          (Lin_Communication.EcwpDryRunErr << 2)    |
                          (Lin_Communication.EcwpVoltErr   << 4)    |
                          (Lin_Communication.EcwpBlkErr    << 6);
    SenddDataB.bit.TD7  = (Lin_Communication.EcwpTempErr)           |
                          (Lin_Communication.EcwpRespErr << 7);
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
    Lin_Communication.EcwpActSpd            = ObserverMotorSpeedINOnePointHZ;

    /*反馈电压，单位：0.1V*/
    Lin_Communication.EcwpLoVolt            = gUDC.uDCFilter;

    /*反馈温度，单位：*/
    Lin_Communication.EcwpBdT               = InternalTemperatureDegree;

    /*反馈相电流有效值，单位：0.1A*/
    Lin_Communication.EcwpActCur            = ((Uint32)gLineCur.CurPer*MOTOR_RATED_CUR)>>12;

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
    if (gMainStatus.ErrorCode               == ERROR_LIN) {
        Lin_Communication.EcwpRespErr       = 1;
    } else {
        Lin_Communication.EcwpRespErr       = 0;
    }
    if (gMainStatus.ErrorCode               == ERROR_NTC) {
        Lin_Communication.EcwpTempErr       = 1;
    } else {
        Lin_Communication.EcwpTempErr       = 0;
    }
    if (gMainStatus.ErrorCode               == ERROR_NO_LOAD) {
        Lin_Communication.EcwpDryRunErr     = 1;
    } else {
        Lin_Communication.EcwpDryRunErr     = 0;
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

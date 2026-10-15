//#############################################################################
//
// FILE:   LinLDF.h
//
// TITLE:  LIN communication data declarations.
//
// Description:
// This file declares the LIN communication data layout, transfer buffers, and
// helper interfaces used by the application.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_
#define HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Device.h>
#include "Define.h"

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/
typedef struct Lin_Communication
{
    /* Host transmission data */
    Uint32 EcwpSpdReq; // Motor speed setting
    Uint32 EcwpSafeMode; // Mode setting
    Uint32 EcwpEna; // Motor enable
    /* Host reception data */
    Uint32 EcwpActSpd;             // Motor speed
    Uint32 EcwpLoVolt;             // Board voltage
    Uint32 EcwpBdT;                // Board temperature
    Uint32 EcwpActCur;             // Motor current
    Uint32 EcwpOverCurErr;         // Overcurrent fault
    Uint32 EcwpDryRunErr;          // Dry running fault
    Uint32 EcwpVoltErr;            // Voltage fault
    Uint32 EcwpBlkErr;             // Blocking fault
    Uint32 EcwpTempErr;            // Overtemperature fault
    Uint32 EcwpRespErr;            // Communication fault
} Lin_COMMUNICATION;

struct LINTD0 {                // bit    description
    Uint16  TD3:8;             // 7:0    Transmit Buffer 3
    Uint16  TD2:8;             // 15:8   Transmit Buffer 2
    Uint16  TD1:8;             // 23:16  Transmit Buffer 1
    Uint16  TD0:8;             // 31:24  Transmit Buffer 0
};


typedef union LIN_data0 {
   Uint32         all;
   struct LINTD0  bit;
} LIN_DATA0;


struct LINTD1 {                // bit    description
    Uint16  TD7:8;             // 7:0    Transmit Buffer 7
    Uint16  TD6:8;             // 15:8   Transmit Buffer 6
    Uint16  TD5:8;             // 23:16  Transmit Buffer 5
    Uint16  TD4:8;             // 31:24  Transmit Buffer 4
};

typedef union LIN_data1 {
   Uint32          all;
   struct LINTD1   bit;
} LIN_DATA1;

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern Lin_COMMUNICATION Lin_Communication;

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern void UpdateRecivedLinDataToVars(void);
extern void UpdateLinDataToSent(void);
extern void LinData_init(void);
extern void UpdateLinDataToVar(void);

extern unsigned short GetSetupFreComs(void);
extern unsigned short GetStartCMDComs(void);

extern Uint16 ReciveDataIDCorrect[8];
extern Uint16 ReciveDataIDInCorrect[8];
extern LIN_DATA0 SenddDataA;
extern LIN_DATA1 SenddDataB;

#endif /* HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_ */

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

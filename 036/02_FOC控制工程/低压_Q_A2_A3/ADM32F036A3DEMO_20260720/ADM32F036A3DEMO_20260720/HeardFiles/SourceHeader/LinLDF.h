#ifndef LIN_LDF_H
#define LIN_LDF_H

#include "Define.h"
#include "ADP32F03x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 *  LIN communication data structures
 *============================================================================*/
typedef struct
{
    Uint32 EcwpSpdReq;
    Uint32 EcwpSafeMode;
    Uint32 EcwpEna;

    Uint32 EcwpActSpd;
    Uint32 EcwpLoVolt;
    Uint32 EcwpBdT;
    Uint32 EcwpActCur;
    Uint32 EcwpOverCurErr;
    Uint32 EcwpDryRunErr;
    Uint32 EcwpVoltErr;
    Uint32 EcwpBlkErr;
    Uint32 EcwpTempErr;
    Uint32 EcwpRespErr;
} Lin_COMMUNICATION;

typedef struct
{
    Uint16 TD3:8;
    Uint16 TD2:8;
    Uint16 TD1:8;
    Uint16 TD0:8;
} LINTD0_BITS;

typedef union
{
    Uint32      all;
    LINTD0_BITS bit;
} LIN_DATA0;

typedef struct
{
    Uint16 TD7:8;
    Uint16 TD6:8;
    Uint16 TD5:8;
    Uint16 TD4:8;
} LINTD1_BITS;

typedef union
{
    Uint32      all;
    LINTD1_BITS bit;
} LIN_DATA1;

/*=============================================================================
 *  Public variables
 *============================================================================*/
extern Lin_COMMUNICATION Lin_Communication;

extern Uint16 ReciveDataIDCorrect[8];
extern Uint16 ReciveDataIDInCorrect[8];

extern LIN_DATA0 SenddDataA;
extern LIN_DATA1 SenddDataB;

/*=============================================================================
 *  Public functions
 *============================================================================*/
void UpdateRecivedLinDataToVars(void);
void UpdateLinDataToSent(void);
void LinData_init(void);
void UpdateLinDataToVar(void);


#ifdef __cplusplus
}
#endif

#endif /* LIN_LDF_H */

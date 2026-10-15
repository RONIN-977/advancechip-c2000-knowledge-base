
#ifndef  FUNC_USER_H
#define  FUNC_USER_H

#include "Main.h"

#if 1
#define  IU_INV      0
#define  IU_INW      1
#define  IV_INU      2
#define  IV_INW      3
#define  IW_INU      4
#define  IW_INV      5
#define  IU_INV_EST  6
#define  IU_INW_EST  7
#define  IV_INU_EST  8
#define  IV_INW_EST  9
#define  IW_INU_EST  10
#define  IW_INV_EST  11

#define  IU_IV        12
#define  IU_IW        13
#define  IV_IU        14
#define  IV_IW        15
#define  IW_IU        16
#define  IW_IV        17

#define  IUEST_INV    18
#define  IUEST_INW    19
#define  IVEST_INU    20
#define  IVEST_INW    21
#define  IWEST_INU    22
#define  IWEST_INV    23

#else
#define  IU_INV      0
#define  IU_INW      1
#define  IV_INU      2
#define  IV_INW      3
#define  IW_INU      4
#define  IW_INV      5
#define  IU_INV_EST  6
#define  IU_INW_EST  7
#define  IV_INU_EST  8
#define  IV_INW_EST  9
#define  IW_INU_EST  10
#define  IW_INV_EST  11
#endif

//#define  T_DELAY      150//250//200//150L  // 60 * 2 -- 2us
//#define  T_SAMPLE     150//50 //100//150L
//#define  CURR_SAMPLE_DELAY         (T_SAMPLE - T_DELAY)
//#define  MIN_SAMPLE_TIME           (T_SAMPLE + T_DELAY)
//#define  TWO_MIN_SAMPLE_TIME       (MIN_SAMPLE_TIME*2)

typedef struct
{
    Uint16 U_CMP_U;
	Uint16 U_CMP_V;
	Uint16 U_CMP_W;

	Uint16 D_CMP_U;
	Uint16 D_CMP_V;
	Uint16 D_CMP_W;

	Uint16 D_CMP_U_BAK;
	Uint16 D_CMP_V_BAK;
	Uint16 D_CMP_W_BAK;

	Uint16 T1CMP;
	Uint16 T2CMP;

	int16  CurrSampleNumAct;
	int16  CurrSampleNextNum;
	int16  CurrSampleNextNum_bak;
} PWM_CMP_DEF;

extern PWM_CMP_DEF gPWM_CMP;

#endif

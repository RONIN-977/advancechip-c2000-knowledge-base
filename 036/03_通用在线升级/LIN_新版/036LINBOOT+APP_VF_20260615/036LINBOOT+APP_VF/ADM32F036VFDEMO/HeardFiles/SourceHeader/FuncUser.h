
#ifndef  _FUNC_USER_H_
#define  _FUNC_USER_H_

#ifdef   _FUNC_USER_VAR_
#define  _FUNC_USER_DEF_
#else
#define  _FUNC_USER_DEF_  extern
#endif


typedef struct
{
    unsigned short U_CMP_U;
	unsigned short U_CMP_V;
	unsigned short U_CMP_W;

	unsigned short D_CMP_U;
	unsigned short D_CMP_V;
	unsigned short D_CMP_W;

	unsigned short D_CMP_U_BAK;
	unsigned short D_CMP_V_BAK;
	unsigned short D_CMP_W_BAK;

	unsigned short T1CMP;
	unsigned short T2CMP;

	int  CurrSampleNumAct;
	int  CurrSampleNextNum;
	int  CurrSampleNextNum_bak;
} PWM_CMP_DEF;


extern PWM_CMP_DEF    gPWM_CMP;

#endif


/******************************************************************************
File Name  : StaticVModelObs.h
Author     : GRHAM XU
Date       : 2025/12/30
Description: DSP初始化头文件
*******************************************************************************/

#ifndef _STATICVMODELOBS_H_
#define _STATICVMODELOBS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    long   Id;
    long   Iq;
    long   LastId;
    long   LastIq;
    long   Vd;
    long   Vq;

    long   RsId;
    long   RsIq;
    long   LdDivDt;
    long   LqDivDt;
    long   OmgaLdId;
    long   OmgaLqIq;

    long   Ed;
    long   Eq;
    long   Rs_pu;          // Rs
    long   Lq_pu;          // Lq
    long   Ld_pu;          // Ld
    long   TsDivFlux_pu;   // Flux
    long   Ts_pu;          // Ts
    long   LsDivTs_pu;

    long   Alpha0;
    long   Lambda;
    long   Omega;
    long   Alpha_bw_lpf;

    long   FilterCnt;
    long   Speed_pu;
    long   Angle_pu;
    long   AngleCoff;
    long   rad_sToPu;
//    void  (*calc)();        // Pointer to calculation function
} STATICVMODELOBS;

typedef STATICVMODELOBS *STATICVMODELOBS_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the SMOPOS object.
-----------------------------------------------------------------------------*/
#define STATICVMODELOBS_DEFAULTS {                              \
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    \
    (void (*)(Uint32))Est_calc                                  \
}

/*------------------------------------------------------------------------------
Prototypes for the functions in _STATICVMODELOBS_H_.C
------------------------------------------------------------------------------*/
extern void Est_calc();
extern void Est_Rest(void);
extern void Est_Init(void);
extern void Est_SetParams(void);

extern STATICVMODELOBS gEst;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

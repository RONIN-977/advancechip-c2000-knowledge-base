
#ifndef MOTOR_INCLUDE_H
#define MOTOR_INCLUDE_H

#include "Main.h"
#include "Parameter.h"
#include "SubPrgInclude.h"
#include "MotorParVars.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BIT32_REG_DEF {
   Uint16  LSW;
   Uint16  MSW;
}BIT32_REG;

typedef union BIT32_GROUP_DEF {
   Uint32     all;
   BIT32_REG  half;
}BIT32_GROUP;

typedef struct INV_STRUCT_DEF {
    Uint16     InvSimpleFlag;
    Uint16     InvVoltageType;
    Uint16     InvTypeApply;
    Uint16     InvType;
    Uint16     GPType;
    Uint16     InvCurrent;
    Uint16     InvOlCurrent;
    Uint16     InvVolt;
    Uint16     CurrentCoff;
    Uint16     UDCCoff;
    Uint16     TempType;
    Uint16     InvOverCurrent;
    Uint16     InvUpUDC;
    Uint16     InvUpUDCCoef;
    Uint16     InvLowUDC;
    Uint16     InvLowUDCCoef;
    Uint16     BaseUdc;
} INV_STRUCT;

typedef struct MOTOR_STRUCT_DEF {
    Uint16     Type;
    Uint16     Power;
    Uint16     Votage;
    Uint16     CurrentGet;
    Uint16     Frequency;
    Uint16     FreqPer;
    Uint16     Current;
    Uint16     CurBaseCoff;
} MOTOR_STRUCT;

struct ERROR_FLAG_SIGNAL {
    Uint16   OvCurFlag:1;
    Uint16   OvUdcFlag:1;
    Uint16   Res:14;
};

union ERROR_FLAG_SIGNAL_DEF {
    Uint16                 all;
    struct ERROR_FLAG_SIGNAL bit;
};

typedef struct RUN_STATUS_STRUCT_DEF {
    Uint16     RunStep;
    Uint16     SubStep;
    Uint16     VFStatus;
    Uint16     VCStatus;
    Uint16     ErrorCode;
    Uint16     LastErrorCode;
    union ERROR_FLAG_SIGNAL_DEF   ErrFlag;
    SEND_STATUS_UNION             StatusWord;
} RUN_STATUS_STRUCT;

typedef struct UDC_LIMIT_IT_STRUCT_DEF {
    int16      UDCLimit;
    int16      UDCBak;
    int16      UDCBakCnt;
    int16      UDCDeta;
    int16      FirstOvUdcFlag;
    int16      FirstOvUdcFlag2;
    PID_STRUCT_2   UdcPid;
} UDC_LIMIT_IT_STRUCT;

typedef struct BASE_COMMAND_STRUCT_DEF {
    union MAIN_COMMAND_UNION_DEF   Command;
    int16     FreqSet;
    int16     FreqSetApply;
    int32     FreqPreWs;
    int32     FreqLast;
    int32     FreqFeed;
    int32     FreqDesired;
    int32     VCTorqueLim;
    int32     FreqReal;
    Uint16     SpeedFalg;
} BASE_COMMAND_STRUCT;

typedef struct BASE_PAR_STRUCT_DEF {
    int32    FullFreq;
    Uint16     MaxFreq;
    Uint16     UpFreq;
    Uint16     LowFreq;
    Uint16     FcSet;
    Uint16     FcSetApply;
} BASE_PAR_STRUCT;

typedef struct UDC_STRUCT_DEF {
    int16     DetaUdc;
    Uint16     uDCBak;
    Uint16     uDC;
    Uint16     uDCFilter;
    Uint16     uDCBigFilter;
    Uint16     Coff;
    Uint16     Power;
} UDC_STRUCT;

typedef struct UVW_STRUCT_DEF {
    int16      U;
    int16      V;
    int16      W;
} UVW_STRUCT;

typedef struct IUVW_SAMPLING_STRUCT_DEF {
    int16      U;
    int16      V;
    int16      W;
    int16      UErr;
    int16      VErr;
    int32     Coff;
} IUVW_SAMPLING_STRUCT;

typedef struct ADC_STRUCT_DEF {
    Uint16     DelaySet;
    Uint16     ResetTime;
    int32    ZeroTotal;
    int16    ZeroCnt;
    int16    Comp;
} ADC_STRUCT;

typedef struct ALPHABETA_STRUCT_DEF {
    int16      Alph;
    int16      Beta;
} ALPHABETA_STRUCT;

typedef struct MT_STRUCT_DEF {
    int16      M;
    int16      T;
} MT_STRUCT;

typedef struct AMPTHETA_STRUCT_DEF {
    Uint16     Amp;
    int16      Theta;
    int16      ThetaFilter;
} AMPTHETA_STRUCT;

typedef struct LINE_CURRENT_STRUCT_DEF {
    Uint16     CurPer;
    Uint16     RealCurPer;
    Uint16     CurBaseInv;
    Uint32    CurBaseInvFilter;
    Uint32    CurPerFilter;
    Uint16     CurPerShow;
    Uint16     ErrorShow;
} LINE_CURRENT_STRUCT;

typedef struct CBC_PROTECT_STRUCT_DEF {
    int16    EnableFlag;
    int16    CntU;
    int16    CntV;
    int16    CntW;
    Uint16     CbcFlag;
    Uint16     CbcTimes;
    Uint16     CBCTimeSet;
    Uint16     NoCbcTimes;
} CBC_PROTECT_STRUCT;

typedef struct ANGLE_STRUCT_DEF {
    int32     StepPhase;
    int32     StepPhaseApply;
    int32     IMPhase;
    int16     OutPhase;
    int16     CompPhase;
    int16     RotorPhase;
} ANGLE_STRUCT;

typedef struct JUDGE_POWER_LOW_DEF {
    Uint16     PowerUpFlag;
    Uint16     WaiteTime;
    Uint16     UDCOld;
} JUDGE_POWER_LOW;

typedef struct CUR_EXCURSION_STRUCT_DEF {
    int32     TotalIu;
    int32     TotalIv;
    int32     TotalIw;
    int32     TotalUu;
    int32     TotalIbus;
    int16     Iu;
    int16     Iv;
    int16     Iw;
    int16     Ibus;
    int16     ErrUu;
    int16     ErrIu;
    int16     ErrIv;
    int16     ErrIw;
    int16     ErrIbus;
    int16     Count;
    int16     EnableCount;
    int16     ErrCnt;
} CUR_EXCURSION_STRUCT;

typedef struct UV_AMP_COFF_STRUCT_DEF {
    Uint32    TotalU;
    Uint32    TotalV;
    Uint32    TotalVoltL;
    Uint32    TotalIL;
    Uint32    TotalVolt;
    Uint32    TotalI;
    Uint16     Number;
    Uint16     Comper;
    Uint16     ComperL;
    Uint16     UDivVGet;
    Uint16     UDivV;
    Uint16     UDivVSave;
} UV_AMP_COFF_STRUCT;

typedef struct TEMPLETURE_STRUCT_DEF {
    Uint16     TempAD;
    Uint16     Temp;
    Uint16     TempBak;
    Uint16     ErrCnt;
} TEMPLETURE_STRUCT;

typedef struct FC_CAL_STRUCT_DEF {
    Uint16     Cnt;
    Uint16     Time;
    Uint16     FcBak;
} FC_CAL_STRUCT;

typedef struct DC_BRAKE_STRUCT_DEF {
    int16        Time;
    PID_STRUCT   PID;
} DC_BRAKE_STRUCT;

typedef enum ZERO_LENGTH_PHASE_SELECT_ENUM_DEF {
    ZERO_VECTOR_U,
    ZERO_VECTOR_V,
    ZERO_VECTOR_W,
    ZERO_VECTOR_NONE = 100
} ZERO_LENGTH_PHASE_SELECT_ENUM;

typedef struct OUT_VOLT_STRUCT_DEF {
    int16     Volt;
    int16     VoltPhase;
    int16     VoltApply;
    int16     VoltDisplay;
    int16     VoltPhaseApply;
} OUT_VOLT_STRUCT;

typedef struct PWM_OUT_STRUCT_DEF {
    int32    U;
    int32    V;
    int32    W;
    ZERO_LENGTH_PHASE_SELECT_ENUM  gZeroLengthPhase;
    Uint16    gPWMPrd;
    Uint16    gPWMPrdApply;
    Uint16    AsynModle;
    Uint16    PWMModle;
    int16   SoftPWMCoff;
} PWM_OUT_STRUCT;

typedef struct DEAD_BAND_STRUCT_DEF {
    int16   DeadBand;
    int16   Comp;
    Uint16    CompMode;
    int16   CompU;
    int16   CompV;
    int16   CompW;
    int16   MTPhase;
    Uint16    InvCurFilter;
} DEAD_BAND_STRUCT;

struct CBC_FLAG_STRUCT {
    Uint16  CBC_U:1;
    Uint16  CBC_V:1;
    Uint16  CBC_W:1;
    Uint16  RESV:13;
};

typedef struct PHASE_LOSE_STRUCT_DEF {
    Uint32   Time;
    Uint32   TotalU;
    Uint32   TotalV;
    Uint32   TotalW;
    Uint16    Cnt;
} PHASE_LOSE_STRUCT;

typedef struct SYN_PWM_STRUCT_DEF {
    Uint32   FcApply;
} SYN_PWM_STRUCT;

typedef struct SHORT_GND_STRUCT_DEF {
    Uint16    Comper;
    Uint16    BaseUDC;
    Uint16    Flag;
    int16   ShortCur;
} SHORT_GND_STRUCT;

typedef struct CPU_TIME_STRUCT_DEF {
    Uint32   Motor2MsBase;
    Uint32   Function2MsBase;
    Uint32   ADCIntBase;
    Uint32   PWMIntBase;
    Uint16    Motor2Ms;
    Uint16    Function2Ms;
    Uint16    ADCInt;
    Uint16    PWMInt;
    Uint16    System05OverRunCnt;
    Uint16    Fun2ms;
    Uint16    Fun2msOverRunCnt;
    Uint16    Fun2msOverRunFlag;
    Uint16    Motor2msOverRunCnt;
    Uint16    Motor2msOverRunFlag;
    Uint16  flash_IAP_flag;
    Uint16  IAP_Delay;
} CPU_TIME_STRUCT;

typedef struct FEISU_STRUCT_DEF {
    Uint16    VoltCNT;
    int16     SpeedLast;
} FEISU_STRUCT;

typedef struct PM_INIT_POSITION_TYPE {
    int16    SubStep;
    int16    PeriodCnt;
    int16    Section;
    int32    PWMTs;
    int32    PWMTs1;
    int32    CurFirst;
    int32    Cur[7];
    int16    Num;
} PM_INIT_POSITION;

typedef struct PM_FLUX_WEAK_DEF {
    int32    Id;
    int32    Iq;
    int16    IdMax;
    int16    IqMax;
    int16    FluxWeakCurr;
    int16    IqLimit;
    int32    TorqLim;
    int16    RatioRef;
    int16    Ratio;
    int16    UdcLpf;
    int32    UdMaxLpf;
    int16    VoltOut;
    int32    IqLpf;
    int32    AdjustId;
    int32    AdjustId1;
    int32    Omg;
    int32    VoltMax;
    int32    CurrCoef;
    int32    VoltCoef;
    int16    Num;
    int16    CoefFlux;
    int32    CoefIqMax;
    int16    CoefIdComp;
    int32    VoltLpf;
    int16    Mode;
    int16    ud;
    int16    uq;
    int32    AdIqMaxIntg;
    int16    UdForIqMax;
    int16    KpIqMax;
} PM_FLUX_WEAK;

typedef struct BEMF_STRUCT_DEF {
    int32    Ua;
    int32    Ub;
    int32    Uc;
    Uint16     BEMF_MAX;
    Uint16     BEMF_MIN;
    int16    BEMF_Alpha;
    int16    BEMF_Beta;
    Uint16     SectorCurr_Candidate;
    Uint16     Sector_Candidate;
    Uint16     Sector_Confirm_Count;
    Uint16     Sector;
    Uint16     SectorLast;
    Uint16     DIR;
    Uint16     PWMCnt;
    Uint16     PWMCntFilter;
    int16    Spd_Hz;
    int16    LastSpd_Hz;
    int32    OutVolt;
} BEMF_STRUCT;
/************************************************************
 END
************************************************************/

/***************************************************************************
  Public Variables
 ***************************************************************************/
extern PM_INIT_POSITION         gIPMInitPos;
extern PM_FLUX_WEAK             gFluxWeak;
extern INV_STRUCT               gInvInfo;
extern MOTOR_STRUCT             gMotorInfo;
extern RUN_STATUS_STRUCT        gMainStatus;
extern BASE_COMMAND_STRUCT      gMainCmd;
extern BASE_PAR_STRUCT          gBasePar;
extern ADC_STRUCT               gADC;
extern UDC_STRUCT               gUDC;
extern IUVW_SAMPLING_STRUCT     gCurSamp;
extern UVW_STRUCT               gIUVW;
extern ALPHABETA_STRUCT         gIAlphBeta;
extern MT_STRUCT                gIMT;
extern AMPTHETA_STRUCT          gIAmpTheta;
extern LINE_CURRENT_STRUCT      gLineCur;
extern ANGLE_STRUCT             gPhase;
extern JUDGE_POWER_LOW          gLowPower;
extern CUR_EXCURSION_STRUCT     gExcursionInfo;
extern DEAD_BAND_STRUCT         gDeadBand;
extern OUT_VOLT_STRUCT          gOutVolt;
extern Uint16                     gRatio;
extern PWM_OUT_STRUCT           gPWM;
extern FC_CAL_STRUCT            gFcCal;
extern DC_BRAKE_STRUCT          gDCBrake;
extern TEMPLETURE_STRUCT        gTemperature;
extern PHASE_LOSE_STRUCT        gPhaseLose;
extern CBC_PROTECT_STRUCT       gCBCProtect;
extern SYN_PWM_STRUCT           gSynPWM;
extern SHORT_GND_STRUCT         gShortGnd;
extern CPU_TIME_STRUCT          gDSPActiveTime;
extern FEISU_STRUCT             gFeisu;
extern BEMF_STRUCT              gBemf;
extern CUR_SAMPLE_TYPE          CurrentSampleType2;

extern Uint16 DCBrakeCur;
extern Uint16 SpeedFeedbackFilter;
extern Uint16 MinCurrentLowSpeed;
extern Uint16 SpecailFactor;
extern Uint16 SpeedMeasureFactor1;
extern Uint16 SpeedMeasureFactor2;
extern Uint16 StartPresetCurrent;
extern Uint16 FieldWeakMode;
extern Uint16 FieldWeakCurScale;
extern Uint16 FieldWeakScale;
extern Uint16 FieldWeakRef;
extern Uint16 IPDAutoDectEnable;
extern Uint16 IPDAutoDectScaling;

extern int16 pm_ki_speed;
extern int16 pm_kp_speed;
extern int32 pm_ki_d;
extern int32 pm_ki_q;
extern int32 pm_kp_q;
extern int32 pm_kp_d;
extern int32 pm_r;
extern Uint32 l_ld;
extern Uint32 l_lq;
extern int32 pm_ref_id;
extern int32 pm_ref_iq;
extern Uint16 pm_control_mode;
extern int16 pm_curr_limitL;
extern int16 pm_curr_limitH;

extern int32  pm_est_lpf_omg1;
extern int16 pm_check_ip_flag;
extern int16 pm_ud;
extern int16 pm_uq;
extern int32 pm_est_omg,pm_est_omg_1;
extern Uint16 pm_bem_kf;
extern int32 pm_est_angel;
extern int16 pm_kp_speed_app;
extern int16 pm_ki_speed_app;
extern int16 cof_uwMaxThetaPerCntTcPu;
extern int16 pm_min_curr;
extern int16 pm_speed_lpf_k;
extern int16 pm_speed_lpf_k1;
extern int32 pm_ref_speed;
extern int32 pm_integral_speed;
extern Uint16 IdMaxSet;
extern Uint16 IqFWLimit;
extern Uint16 AVREnableMode;
extern Uint16 MotorTestData1;
extern Uint16 NoNegativeOMG;
extern Uint16 MotorTestData4;
extern Uint16 MotorTestData5;
extern Uint16 MotorPWMModeForNoise;
extern Uint16 RsSecond;
extern Uint16 PaseShiftEnable;
extern Uint16 LowNoseElmPWMMode;
extern Uint16 LastLowNoseElmPWMMode;
extern int16 InternalTemperatureDegree;
extern int16 IbusActual;
extern int16 ActualPower;
extern int16 pm_dq_angle;
extern int16 CURR_SAMPLE_DELAY;//   = MotorTestData2 - MotorTestData1;    // (T_SAMPLE - T_DELAY)
extern int16 MIN_SAMPLE_TIME;//     = MotorTestData2 + MotorTestData1;    // (T_SAMPLE + T_DELAY)
extern int16 TWO_MIN_SAMPLE_TIME;// = MIN_SAMPLE_TIME * 2;                // MIN_SAMPLE_TIME * 2;
extern int16 pm_forbid_revtorq;
extern int16 pm_maxout_vol;
extern Uint16 RunBoostState;
extern int16 DCStart_flag;
extern int32 pm_integral_d;
extern int32 pm_integral_q;
extern int16 VolOffset;
extern int16 ReadSpeedBasedonFinished;
extern Uint16 QPR_EN;
extern int32 temp_id;
extern int32 temp_iq;
extern int32 tmp_swDeltaThetaPu;

/***************************************************************************
  Public Functions
 ***************************************************************************/
void CalOutVotInVFStatus(void);
void CalOutVoltInDCBrakeStatus(void);
void OutPutPWMVF(void);
void UpdatPWMRegs(void);
void InitSetPWM(void);
void InitSetAdc(void);
void OutPutPWMPhaseShift(void);
void OutPutPWMPhaseShiftForLowNoiseFullMode(void);
void ParGetFromFunction(void);
void PMFluxWeaking();
void VFFreqCal(void);
void pmsvc_init();
void PMFluxWeakInit();
void CalDeadBandComp(void);
void PMCalcLineCurr();
void SynInitPosDetect(void);
void CalRatioFromVot(void);
void AllowSelectBottomOnly(Uint16 Selection);
void ADCProcess(void);
void GetUDCInfo(void);
void GetIDCInfo(void);
void SVPWM_1ShuntGetPhaseCurrent(void);
void SVPWM_3ShuntGetPhaseCurrent(void);
void GetAIInfo(void);
void GetTemperatureInfo(void);
void InvDeviceKontrol(void);
void OutputLoseAdd(void);
void CurrentLoop();
void VolVectCalation();
void FluxPostionCalation(void);
int16 GetPWMSector(void);
void SynInitPosDetCal(void);
void GetBemf_DirSpeed(void);
void UVWToAlphBetaAxes(UVW_STRUCT* , ALPHABETA_STRUCT* );
void AlphBetaToDQ(ALPHABETA_STRUCT* , int16 , MT_STRUCT* );
void ChangeCurrent(void);
void PMSMSVC_2ms(void);
void ResetADCAndPWM();
void ClearAllTZFlags(void);
void ClearAllTZFlagsForOCAtFristPowerUP(void);
void calc_pm_coef();
int32 GetCurrentIPD(int16 SectionData);
void CalStepAngle(void);
void SoftWareErrorDeal(void);
void CalOutputPhase(void);
void ParSendToFunction(void);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================

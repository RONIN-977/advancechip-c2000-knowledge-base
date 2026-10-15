/***************************************************************************
  File: MCVar.c

  Description:
    Defines global motor-control variables, runtime status structures,
    parameter structures, sampling data structures, PWM data, protection data,
    and motor control interaction variables.
 ***************************************************************************/

#include "FuncUser.h"
#include "MotorInclude.h"

INV_STRUCT              gInvInfo;
MOTOR_STRUCT            gMotorInfo;
RUN_STATUS_STRUCT       gMainStatus;
BASE_COMMAND_STRUCT     gMainCmd;
BASE_PAR_STRUCT         gBasePar;
ADC_STRUCT              gADC;
UDC_STRUCT              gUDC;
IUVW_SAMPLING_STRUCT    gCurSamp;
UVW_STRUCT              gIUVW;
ALPHABETA_STRUCT        gIAlphBeta;
MT_STRUCT               gIMT;
AMPTHETA_STRUCT         gIAmpTheta;
LINE_CURRENT_STRUCT     gLineCur;
ANGLE_STRUCT            gPhase;
JUDGE_POWER_LOW         gLowPower;
CUR_EXCURSION_STRUCT    gExcursionInfo;
DEAD_BAND_STRUCT        gDeadBand;
CBC_PROTECT_STRUCT      gCBCProtect;
TEMPLETURE_STRUCT       gTemperature;
PHASE_LOSE_STRUCT       gPhaseLose;
FC_CAL_STRUCT           gFcCal;
DC_BRAKE_STRUCT         gDCBrake;
OUT_VOLT_STRUCT         gOutVolt;
Uint16                  gRatio;
PWM_OUT_STRUCT          gPWM;
SYN_PWM_STRUCT          gSynPWM;
FEISU_STRUCT            gFeisu;
CPU_TIME_STRUCT         gDSPActiveTime;
PM_INIT_POSITION        gIPMInitPos;
PM_FLUX_WEAK            gFluxWeak;
SHORT_GND_STRUCT        gShortGnd;
BEMF_STRUCT             gBemf;
CUR_SAMPLE_TYPE         CurrentSampleType2;
PWM_CMP_DEF             gPWM_CMP;

Uint16                  SpeedFeedbackFilter;
Uint16                  MinCurrentLowSpeed;
Uint16                  SpecailFactor;
Uint16                  SpeedMeasureFactor1;
Uint16                  SpeedMeasureFactor2;
Uint16                  StartPresetCurrent;
Uint16                  FieldWeakMode;
Uint16                  FieldWeakCurScale;
Uint16                  FieldWeakScale;
Uint16                  FieldWeakRef;

Uint16                  IPDAutoDectScaling;
Uint16                  IdMaxSet;
Uint16                  IqFWLimit;
Uint16                  AVREnableMode;
Uint16                  MotorTestData1;

Uint16                  NoNegativeOMG;
Uint16                  MotorTestData4;
Uint16                  MotorTestData5;
Uint16                  QPR_EN;
Uint16                  MotorPWMModeForNoise;

Uint16                  DCBrakeCur;
Uint16                  RsSecond;
Uint16                  PaseShiftEnable = 1;
Uint16                  LowNoseElmPWMMode;
Uint16                  LastLowNoseElmPWMMode;
int16                   InternalTemperatureDegree;
int16                   IbusActual;
int16                   ActualPower;
Uint32                  l_ld;
Uint32                  l_lq;
int16                   DCStart_flag;



#include "MotorInclude.h"


int  gSendToFunctionDataBuff[MOTOR_TO_FUNCTION_DATA_NUM];
Uint gSendToMotorDataBuff[FUNCTION_TO_MOTOR_DATA_NUM];				//传递给性能模块的数组

INV_STRUCT 				gInvInfo;		//变频器信息
MOTOR_STRUCT 			gMotorInfo;		//电机信息

RUN_STATUS_STRUCT 		gMainStatus;	//主运行状态
BASE_COMMAND_STRUCT		gMainCmd;		//主命令

/************************************************************/
/**********以下为和电机控制相关设定参数定义******************/
BASE_PAR_STRUCT			gBasePar;		//基本运行参数

ADC_STRUCT				gADC;			//ADC数据采集结构
UDC_STRUCT				gUDC;			//母线电压数据
IUVW_SAMPLING_STRUCT	gCurSamp;
UVW_STRUCT				gIUVW;			//定子三相坐标轴电流
ALPHABETA_STRUCT		gIAlphBeta;		//定子两相坐标轴电流
MT_STRUCT				gIMT;			//MT轴系下的电流
AMPTHETA_STRUCT			gIAmpTheta;		//极坐标表示的电流
LINE_CURRENT_STRUCT		gLineCur;	
ANGLE_STRUCT			gPhase;			//角度结构
JUDGE_POWER_LOW			gLowPower;		//上电缓冲判断使用数据结构
JUDGE_ACC_DEC_STRUCT	gSpeedFlag;		//判断加减速标志的结构
CUR_EXCURSION_STRUCT	gExcursionInfo;	//检测零漂使用的结构
DEAD_BAND_STRUCT		gDeadBand;

CBC_PROTECT_STRUCT      gCBCProtect;
TEMPLETURE_STRUCT		gTemperature;
PHASE_LOSE_STRUCT		gPhaseLose;

FC_CAL_STRUCT			gFcCal;

DC_BRAKE_STRUCT         gDCBrake;
OUT_VOLT_STRUCT			gOutVolt;
Uint					gRatio;			//调制系数
PWM_OUT_STRUCT			gPWM;

SYN_PWM_STRUCT			gSynPWM;
FEISU_STRUCT			gFeisu;			//转速跟踪用变量
CPU_TIME_STRUCT			gDSPActiveTime;

PM_INIT_POSITION        gIPMInitPos;
PM_FLUX_WEAK		    gFluxWeak;
SHORT_GND_STRUCT        gShortGnd;
BEMF_STRUCT             gBemf;

//---------------------------------------------------------------
//电机控制模块和控制模块的交互表
//---------------------------------------------------------------
Uint MotorPWMRandom;
Uint DCBrakeCur;
Uint TorqueCurrentLimit;
Uint OverspeedScaling;
Uint SpeedFeedbackFilter;
Uint SFLowSpeed;
Uint MinCurrentLowSpeed;
Uint SpecailFactor;
Uint SpeedMeasureFactor1;
Uint SpeedMeasureFactor2;
Uint StartPresetCurrent;
Uint LowLimitFrequency;
Uint FieldWeakMode;
Uint FieldWeakCurScale;
Uint FieldWeakScale;
Uint FieldWeakRef;
Uint DataRsrd;
Uint IPDAutoDectEnable;
Uint IPDAutoDectScaling;

Uint IdMaxSet;
Uint IqFWLimit;

Uint AVREnableMode;
Uint MotorTestData1;
Uint MotorTestData2;
Uint NoNegativeOMG;
Uint MotorTestData4;
Uint MotorTestData5;
Uint MotorPWMModeForNoise;
Uint ReserdData;
Uint DCBrakeCur;
Uint RsSecond;
Uint PaseShiftEnable = 1;
Uint LowNoseElmPWMMode;
Uint LastLowNoseElmPWMMode;
int  InternalTemperatureDegree;
int IbusActual;
int ActualPower;

Uint * const gSendToMotorTable[FUNCTION_TO_MOTOR_DATA_NUM] = 
{
	(Uint *)&gMainCmd.Command.all,	(Uint *)&gMainCmd.FreqSet,			//0、1
	(Uint *)&gMainCmd.VCTorqueLim,	&DataRsrd,		                    //2、3
	&gBasePar.FcSet,				&gBasePar.UpFreq,					//4、5
	&DataRsrd,													        //6
	&gBasePar.MaxFreq,				&gMotorInfo.Power,					//7、8
	&DataRsrd,											                //9
	&gMotorInfo.Power,                      							//10
	&gMotorInfo.Votage,				&gMotorInfo.CurrentGet,				//11、12
	&gMotorInfo.Frequency,												//13
	&IdMaxSet,                      &DataRsrd,        	                //14、15
	&gInvInfo.InvUpUDCCoef,			&gInvInfo.InvLowUDCCoef,			//16、17
	&DataRsrd,		                &ReserdData,                        //18、19
	&gInvInfo.UDCCoff,				&gInvInfo.CurrentCoff,              //20、21
	&MotorPWMRandom, 	    		&IqFWLimit,			                //22、23
	&gADC.DelaySet,                         							//24
	&DCBrakeCur,                     				//25

    &TorqueCurrentLimit,
    &OverspeedScaling,
    &SpeedFeedbackFilter,
    &SFLowSpeed,
    &MinCurrentLowSpeed,
    &SpecailFactor,
    &SpeedMeasureFactor1,
    &SpeedMeasureFactor2,
    &StartPresetCurrent,
    &LowLimitFrequency,
    &FieldWeakMode,
    &FieldWeakCurScale,
    &FieldWeakScale,
    &FieldWeakRef,
    &IPDAutoDectEnable,
    &IPDAutoDectScaling,

    &AVREnableMode,
    &MotorTestData1,
    (Uint *)&cof_uwMaxThetaPerCntTcPu,
    &NoNegativeOMG,
    &MotorTestData4,
    &MotorTestData5,
}; 
 
Uint gSoftVersion = SOFT_VERSION;
Uint * const gSendToFuncTable[MOTOR_TO_FUNCTION_DATA_NUM] = 
{
	&gMainStatus.StatusWord.all,	&gMainStatus.ErrorCode,	//0、1
	&gSoftVersion,	                &gSoftVersion,	        //2、3
	&gSoftVersion,		            &gSoftVersion,			//4、5
	&gSoftVersion,	                &gSoftVersion,			//6、7
	&gSoftVersion,	                &gSoftVersion,		    //8、9
	&gSoftVersion,			        &gSoftVersion           //10、11
};

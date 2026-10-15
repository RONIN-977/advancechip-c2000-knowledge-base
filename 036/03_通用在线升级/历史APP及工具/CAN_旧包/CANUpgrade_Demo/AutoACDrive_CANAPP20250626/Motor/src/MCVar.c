

#include "MotorInclude.h"
#include "param_lr.h"

int gSendToFunctionDataBuff[MOTOR_TO_FUNCTION_DATA_NUM];
Uint gSendToMotorDataBuff[FUNCTION_TO_MOTOR_DATA_NUM];				//传递给性能模块的数组

INV_STRUCT 				gInvInfo;		//变频器信息
MOTOR_STRUCT 			gMotorInfo;		//电机信息
MOTOR_EXTERN_STRUCT		gMotorExtInfo;	//电机扩展信息（实际值表示）
MOTOR_EXTERN_STRUCT		gMotorExtReg;	//电机扩展信息（电机参数辨识得到的数据）
MOTOR_EXTERN_STRUCT		gMotorExtPer;	//电机扩展信息（标么值表示）
RUN_STATUS_STRUCT 		gMainStatus;	//主运行状态
BASE_COMMAND_STRUCT		gMainCmd;		//主命令

/************************************************************/
/**********以下为和电机控制相关设定参数定义******************/
BASE_PAR_STRUCT			gBasePar;		//基本运行参数
COM_PAR_INFO_STRUCT		gComPar;		//公共参数
VF_INFO_STRUCT			gVFPar;			//VF参数

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
TORQUE_UP_STRUCT		gTorqueUp;		//转矩提升用变量
CBC_PROTECT_STRUCT      gCBCProtect;
TEMPLETURE_STRUCT		gTemperature;
PHASE_LOSE_STRUCT		gPhaseLose;
	
FC_CAL_STRUCT			gFcCal;
OVER_UDC_CTL_STRUCT		gOvUdc;
DC_BRAKE_STRUCT         gDCBrake;
OUT_VOLT_STRUCT			gOutVolt;
Uint					gRatio;			//调制系数
PWM_OUT_STRUCT			gPWM;

SYN_PWM_STRUCT			gSynPWM;
UV_AMP_COFF_STRUCT		gUVCoff;
FEISU_STRUCT			gFeisu;			//转速跟踪用变量
PID_STRUCT				gSpeedCheckPID;
CPU_TIME_STRUCT			gDSPActiveTime;

PM_INIT_POSITION        gIPMInitPos;
PM_FLUX_WEAK		    gFluxWeak;
/*
Uint const gDeadBandTable[1] =
{
	200
};

#if 1
Uint const gDeadCompTable[1] =
{
    70
};
#else
Uint const gDeadCompTable[1] =
{
	25
};
#endif
*/
Uint const m_pTable_y[17] = {
       0,
     273,
     396,
     683,
     956,
    1229,
    1536,
    1816,
    2048,
    2335,
    2608,
    2901,
    3127,
    3413,
    3700,
    4041,
    4267
};
int const m_pTable_x[17] = {
   -10,
     0,
    10,
    20,
    30,
    40,
    50,
    60,
    70,
    80,
    90,
    100,
    110,
    120,
    130,
    140,
    150
};

//---------------------------------------------------------------
//电机控制模块和控制模块的交互表
//---------------------------------------------------------------
Uint MotorPWMRandom;
Uint MotorSpeedScaling2ONKB10;

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
Uint IPDCurrentScaling;
Uint IPDAutoDectScaling;

Uint IdMaxSet;
Uint IqFWLimit;
Uint FiveSegModeEn;

Uint AVREnableMode;
Uint MotorTestData1;
Uint MotorTestData2;
Uint MotorTestData3;

Uint * const gSendToMotorTable[FUNCTION_TO_MOTOR_DATA_NUM] = 
{
	(Uint *)&gMainCmd.Command.all,	(Uint *)&gMainCmd.FreqSet,			//0、1
	(Uint *)&gMainCmd.VCTorqueLim,	&DataRsrd,		                    //2、3
	&gBasePar.FcSet,				&gBasePar.UpFreq,					//4、5
	&DataRsrd,													        //6
	&gBasePar.MaxFreq,				&DataRsrd,					        //7、8
	&DataRsrd,													        //9
	&DataRsrd,                      							        //10
	&gMotorInfo.Votage,				&gMotorInfo.CurrentGet,				//11、12
	&gMotorInfo.Frequency,												//13
	&IdMaxSet,                      (Uint *)&cof_uwMaxThetaPerCntTcPu,  //14、15
	&gInvInfo.InvUpUDCCoef,			&gInvInfo.InvLowUDCCoef,			//16、17
	&gComPar.SubCommand.all,		&DataRsrd,                          //18、19
	&DataRsrd,				        &DataRsrd,                          //20、21
	&MotorPWMRandom, 	    		&IqFWLimit,			                //22、23
	&DataRsrd,                         							        //24
	&DCBrakeCur,                     				                    //25

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
    &IPDCurrentScaling,
    &IPDAutoDectScaling,

    &AVREnableMode,
    &DataRsrd,
    &DataRsrd,
    &DataRsrd,
}; 
 
Uint gSoftVersion = SOFT_VERSION;
Uint * const gSendToFuncTable[MOTOR_TO_FUNCTION_DATA_NUM] = 
{
	&gMainStatus.StatusWord.all,	&gMainStatus.ErrorCode,				//0、1
	(Uint *)&gLineCur.ErrorShow,	(Uint *)&gMainCmd.FreqPreWs,				//2、3
	(Uint *)&gUDC.uDCFilter,		(Uint *)&gLineCur.CurPer,			//4、5
	(Uint *)&gOutVolt.VoltApply,	(Uint *)&gTemperature.Temp,					//6、7
	(Uint *)&gSoftVersion,			(Uint *)&pm_iq_show,				//8、9
	(Uint *)&pm_omg_show,			&gInvInfo.InvCurrent
};

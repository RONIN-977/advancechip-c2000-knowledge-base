
#ifndef MOTOR_INCLUDE_H
#define MOTOR_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MainInclude.h"
#include "SubPrgInclude.h"

#define ERROR_NONE                0      // 0   -- 无
#define ERROR_OL_INVERTER         10     // 10 -- 变频器过载
#define ERROR_OC_HARDWARE         12     // 12  -- 硬件电流
#define ERROR_OV_ACC_SPEED        13     // 13  -- 过电压
#define ERROR_UV                  14     // 14  -- 欠压故障
#define ERROR_LOSE_PHASE_OUTPUT   15     // 15  -- 输出缺相
#define ERROR_OT_IGBT             16     // 16  -- 散热器过热
#define ERROR_CURRENT_SAMPLE      17     // 17  -- 电流检测故障
#define ERROR_TUNE                18     // 18  -- 电机调谐故障
#define ERROR_STALL               20     // 20  -- 电机堵转故障
#define ERROR_NTC	              21     // 21  -- NTC故障
#define ERROR_IPD_NO_MOTOR        32     // 21  -- IPD 无电机
#define ERROR_OC_SOFTWARE         33     // 21  -- 软件过流
#define ERROR_CBC                 41     // 40 -- 快速限流超时故障
#define ERROR_ERROR_KE            44     // 44  -- 软件过流

// 与芯片相关的寄存器定义
#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT1                    //ADC中断向量
#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1    //清除ADC模块的中断标志
#define ADC_RESET_SEQUENCE      AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20  //reset the sequence
#define ADC_START_CONVERSION   	AdcRegs.ADCSOCFRC1.all = 0xFFFF 		//软件启动AD
#define ADC_END_CONVERSIN       AdcRegs.ADCINTFLG.bit.ADCINT1           //AD转换完成标志位  

#define  ADC_TRIG_SOURCE       13   //set SOC0 start trigger on EPWM1A, due to round-robin SOC0 converts first then SOC1, then SOC2
#define  ACQPS_SYS_CLKS        8    //set SOC0 S/H Window to 9 ADC Clock Cycles, (8 ACQPS plus 1)

#define  ADC_PIN_S0            0    //A0
#define  ADC_PIN_S1            1    //A1
#define  ADC_PIN_S2            2    //A2
#define  ADC_PIN_S3            3    //A3
#define  ADC_PIN_S4            4    //A4
#define  ADC_PIN_S5            5    //A5
#define  ADC_PIN_S6            6    //A6
#define  ADC_PIN_S7            7    //A7
#define  ADC_PIN_S8            8    //B0
#define  ADC_PIN_S9            9    //B1
#define  ADC_PIN_S10           10   //B2
#define  ADC_PIN_S11           11   //B3
#define  ADC_PIN_S12           12   //B4
#define  ADC_PIN_S13           13   //B5
#define  ADC_PIN_S14           14   //B6
#define  ADC_PIN_S15           15   //B7

#define ADCTRIG_SOFT        0
#define ADCTRIG_CPU_TINT0   1
#define ADCTRIG_CPU_TINT1   2
#define ADCTRIG_CPU_TINT2   3
#define ADCTRIG_XINT2       4
#define ADCTRIG_EPWM1_SOCA  5
#define ADCTRIG_EPWM1_SOCB  6
#define ADCTRIG_EPWM2_SOCA  7
#define ADCTRIG_EPWM2_SOCB  8
#define ADCTRIG_EPWM3_SOCA  9
#define ADCTRIG_EPWM3_SOCB  10
#define ADCTRIG_EPWM4_SOCA  11
#define ADCTRIG_EPWM4_SOCB  12
#define ADCTRIG_EPWM5_SOCA  13
#define ADCTRIG_EPWM5_SOCB  14
#define ADCTRIG_EPWM6_SOCA  15
#define ADCTRIG_EPWM6_SOCB  16
#define ADCTRIG_EPWM7_SOCA  17
#define ADCTRIG_EPWM7_SOCB  18


 
/************************************************************
	 定义DSP的ADC输入口、GPIO口输出口、GPIO输入口
************************************************************/
#define DisConnectRelay()   	//GpioDataRegs.GPACLEAR.bit.GPIO21  = 1
#define ConnectRelay()    	    //GpioDataRegs.GPASET.bit.GPIO21  = 1    //上电缓冲继电器控制

#define EnableDrive()							\
												\
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;			\
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;         \
	EDIS;

#define DisableDrive()							\
												\
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;			\
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;         \
	EDIS;


/************************************************************
与芯片相关的IO口输出定义
************************************************************/


/************************************************************
与芯片相关的寄存器定义
************************************************************/
#define ADC_GND              (AdcResult.ADCRESULT0<<4)          //28035采样结果右对齐，为了跟2808兼容，左移4位
#define	ADC_IU				 (AdcResult.ADCRESULT1<<4)
#define ADC_IV               (AdcResult.ADCRESULT2<<4)
#define	ADC_IW               (AdcResult.ADCRESULT3<<4)          // 这个地方是由于硬件驱动板到DSP版时接口反掉了
#define	ADC_UDC				 (AdcResult.ADCRESULT4<<4)
#define ADC_TEMP             (AdcResult.ADCRESULT5<<4)

/************************************************************
 	端口定义END
************************************************************/

typedef struct BIT32_REG_DEF {
   Uint16  LSW;
   Uint16  MSW;
}BIT32_REG;

typedef union BIT32_GROUP_DEF {
   Uint32     all;
   BIT32_REG  half;
}BIT32_GROUP;

/************************************************************
	结构定义 BEGIN
************************************************************/
/*****************以下为基本信息数据结构********************/
typedef struct INV_STRUCT_DEF {
	Uint 	InvSimpleFlag;			//0：简易变频器；1：有控制板变频器
	Uint    InvVoltageType;         //变频器电压等级信息
	Uint 	InvTypeApply;			//实际使用的机型（P型机 = G型机-1）
	Uint 	InvType;				//用户设置的机型
	Uint 	GPType;					//1 G型机； 2 P型机器
	Uint 	InvCurrent;				//查表得到的变频器电流，	单位0.1A
	Uint    InvOlCurrent;           //过载保护时使用的额定电流
	Uint 	InvVolt;				//查表得到的变频器电压，	单位1V
	Uint 	CurrentCoff;			//变频器电流矫正系数  ，	单位0.1%
	Uint 	UDCCoff;				//变频器母线电压矫正系数，	单位0.1%
	Uint 	TempType;				//变频器温度曲线选择
	Uint 	InvOverCurrent;			//过流点					单位0.1%
	Uint 	InvUpUDC;				//母线过压点				单位0.1V
	Uint 	InvUpUDCCoef;			//母线过压点系数
	Uint 	InvLowUDC;				//欠压点
	Uint 	InvLowUDCCoef;			//欠压点系数

	Uint 	BaseUdc;				//母线电压基准 380V机器为537.4V
}INV_STRUCT;//变频器硬件信息结构

typedef struct MOTOR_STRUCT_DEF {
	Uint 	Type;					//异步机、同步机、其它机器
	Uint 	Power;					//电机功率					单位0.1KW
	Uint 	Votage;					//电机电压					单位1V
	Uint 	CurrentGet;				//电机电流					单位0.1A
	Uint 	Frequency;				//电机频率					单位0.01Hz
	Uint 	FreqPer;				//标么值电机频率
	Uint 	Current;				//电流基值					单位0.1A
	Uint	CurBaseCoff;			//电流基值的放大倍数
}MOTOR_STRUCT;//电机基本信息结构

typedef struct MOTOR_EXTERN_STRUCT_DEF {
	Uint 	R1;						//定子相电阻				单位0.001欧姆
//	Uint 	R2;						//转子相电阻				单位0.001欧姆
//	Uint 	L0;						//漏感						单位0.01mH
//	Uint 	LM;						//互感						单位0.1mH
//	Uint 	I0;						//空载电流					单位0.1A
//	Uint 	Rpm;					//电机转速					单位1rpm
//	Uint 	Poles;					//电机极数
//	Ulong   LR;
}MOTOR_EXTERN_STRUCT;//电机扩展信息结构

struct ERROR_FLAG_SIGNAL {
    Uint16  OvCurFlag:1;  //bit0=1表示发生了过流中断
    Uint16  OvUdcFlag:1;  //bit1=1表示发生了过压中断
	Uint16  Res:14;
};
union ERROR_FLAG_SIGNAL_DEF {
   Uint16                	all;
   struct ERROR_FLAG_SIGNAL  bit;
};
typedef struct RUN_STATUS_STRUCT_DEF {
	Uint 	RunStep;				//主步骤
	Uint 	SubStep;				//辅步骤
	Uint 	VFStatus;				//VF状态，按照bit位定义
	Uint 	VCStatus;				//VC状态，按照bit位定义
	Uint 	ErrorCode;				//出错标志
	Uint    LastErrorCode;          //上次故障代码，用于输出电压相位切换
	union   ERROR_FLAG_SIGNAL_DEF	ErrFlag;									
    SEND_STATUS_UNION		StatusWord;	
} RUN_STATUS_STRUCT;//变频器运行状态结构

typedef struct UDC_LIMIT_IT_STRUCT_DEF {
    int    UDCLimit;
    int    UDCBak;
    int    UDCBakCnt;
    int    UDCDeta;
    int    FirstOvUdcFlag;
    int    FirstOvUdcFlag2;
    PID_STRUCT_MD  UdcPid;
} UDC_LIMIT_IT_STRUCT;

typedef struct BASE_COMMAND_STRUCT_DEF {
	union MAIN_COMMAND_UNION_DEF Command;	//主命令字结构
	int 	FreqSet;				//给定速度
	int 	FreqSetApply;			//实际速度（同步速度）
	long 	FreqPreWs;				//转差补偿前的同步速度
	long 	FreqLast;				//0速前一拍速度
	long 	FreqFeed;				//反馈频率
	long 	FreqDesired;			//目标速度
	long 	VCTorqueLim;			//VC转矩限定
	long 	FreqReal;				//0.01Hz表示的频率
	long    FreqDesiredReal;        //0.01Hz表示的目标频率
	Uint	SpeedFalg;				//加减速标志
}BASE_COMMAND_STRUCT;//实时修改的命令结构

/************************************************************/
/**********以下为和电机控制相关设定参数定义数据结构***********/
typedef struct BASE_PAR_STRUCT_DEF {
	long 	FullFreq;				//32767表示的频率
	Uint 	MaxFreq;				//最大频率
	Uint 	UpFreq;					//上限频率
	Uint 	LowFreq;				//下限频率
	Uint 	FcSet;					//设定载波频率		
	Uint 	FcSetApply;				//实际载波频率		
}BASE_PAR_STRUCT;	//基本运行信息结构

typedef struct COM_PAR_INFO_STRUCT_DEF {
	union SUB_COMMAND_UNION  SubCommand;	//辅命令字结构
}COM_PAR_INFO_STRUCT;	//和运行方式无关的参数设置峁?

typedef struct VF_INFO_STRUCT_DEF {
	Uint 	VFLineType;				//VF曲线选择
	Uint    ovGain;
}VF_INFO_STRUCT;	//VF参数设置数据结构

typedef struct  VF_AUTO_TORQUEBOOST_VAR_DEF{
    int         VfCurrentIs;
}VF_AUTO_TORQUEBOOST_VAR;	//自动转矩提升用变量结构

typedef struct  VF_WS_COMP_VAR_DEF{
    int     CoffApply;
}VF_WS_COMP_VAR;			//转差补偿用变量结构

typedef struct VC_INFO_STRUCT_DEF {
	Uint 	ASRKpLow;				//低频速度环KP
}VC_INFO_STRUCT;	//和矢量控制相关的参数设置数据结构

/************************************************************/
/********************以下为实时信息数据结构******************/
typedef struct UDC_STRUCT_DEF {
	int		DetaUdc;
	Uint	uDCBak;
	Uint 	uDC;				//			单位0.1V
	Uint 	uDCFilter;			//大滤波母线电压	单位0.1V
	Uint 	uDCBigFilter;		//过压、欠压判断用母线电压	单位0.1V
	Uint 	Coff;				//母线电压计算系数
	Uint 	Power;
}UDC_STRUCT;	//母线电压数据

typedef struct UVW_STRUCT_DEF{					
	int  	U;					//Q12格式，以电机额定值为标么值基值
	int  	V;
	int  	W;
}UVW_STRUCT;	//定子三相坐标轴电流

typedef struct IUVW_SAMPLING_STRUCT_DEF{					
	int  	U;					//Q12格式，以电机额定电流为标么值基值
	int  	V;
	int  	W;
    int     ULast;              //Q12格式，以电机额定电流为标么值基值
    int     VLast;
    int     WLast;
    int     ULast2;              //Q12格式，以电机额定电流为标么值基值
    int     VLast2;
    int     WLast2;
	int		UErr;				//U相毛刺滤波
	int 	VErr;				//V相毛刺滤波
	long    Coff;				//采样电流转换为标么值电流的系数
}IUVW_SAMPLING_STRUCT;	//电流采样结构

typedef struct ADC_STRUCT_DEF{					
	Uint	DelaySet;			//ADC采样延时时间(0.1us单位)
	Uint	DelayApply;			//ADC采样延时时间(PWM定时器周期单位)
	Uint  	ResetTime;			//ADC已经启动的次数
	long	ZeroTotal;
	int		ZeroCnt;
	int		Comp;
}ADC_STRUCT;	//定子三相坐标轴电流

typedef struct ALPHABETA_STRUCT_DEF{
	int  	Alph;				//Q12
	int  	Beta;
}ALPHABETA_STRUCT;//定子两相坐标轴电流、电压结构

typedef struct MT_STRUCT_DEF{
	int  	M;					//Q12
	int  	T;
}MT_STRUCT;	//MT轴系下的电流、电压结构

typedef struct AMPTHETA_STRUCT_DEF{
	Uint  	Amp;				//Q12
	int  	Theta;				//Q15
	int     ThetaFilter;        //Q15 电流电压夹角的滤波值
}AMPTHETA_STRUCT;//极坐标表示的电流、电压结构

typedef struct LINE_CURRENT_STRUCT_DEF{
	Uint  	CurPer;				//Q12 以电机电流为基值表示的线电流有效值
	Uint  	RealCurPer;			//Q12 以电机电流为基值表示南叩缌餍效值(考虑增益)
	Uint  	CurBaseInv;			//Q12 以变频器电流为基值表示的线电流有效值
	Ulong  	CurBaseInvFilter;	//Q12 以变频器电流为基值表示的线电流有效值
	Ulong  	CurPerFilter;		//Q12 以电机电流为基值表示的线电流有效值
	Uint  	CurPerShow;			//Q12 以电机电流为基值表示的线电流有效值
	Uint  	ErrorShow;			//过流时刻记录的线电流有效值
}LINE_CURRENT_STRUCT;//计算过程中使用的线电流表示

typedef struct CBC_PROTECT_STRUCT_DEF{
   int      EnableFlag;
   int      CntU;       //电流大于1.6倍峰值电流后的累加
   int      CntV;
   int      CntW;
   Uint     CbcFlag;     //当前2ms内发生了逐波限流
   Uint     CbcTimes;    //逐波限流次数
   Uint     CBCTimeSet;  //逐波限流动作次数设定值
   Uint     NoCbcTimes;  //逐波限流没用发生的持续时间
}CBC_PROTECT_STRUCT;    //逐波限流保护数据结构

typedef struct ANGLE_STRUCT_DEF {
	long 	StepPhase;			//步长角度（计算出来）
	long 	StepPhaseApply;		//步长角度（实际使用）
	long 	IMPhase; 			//M轴角度
	int 	OutPhase; 			//PWM角度
	int  	CompPhase;			//相位延迟补偿角度
	int  	RotorPhase;			//转子角度
}ANGLE_STRUCT;

/************************************************************/
/*****************以下为独立模块使用的数据结?***************/
typedef struct JUDGE_POWER_LOW_DEF {
	Uint 	PowerUpFlag;
	Uint 	WaiteTime;
	Uint 	UDCOld;
}JUDGE_POWER_LOW;	//上电缓冲判断使用数据结构

typedef struct JUDGE_ACC_DEC_STRUCT_DEF {
	int 	LastFreq;
	int 	Cnt;
}JUDGE_ACC_DEC_STRUCT;	//判断加减速标志的数据结构

typedef struct CUR_EXCURSION_STRUCT_DEF{
	long	TotalIu;
	long	TotalIv;
	long    TotalIw;
	int		Iu;
	int		Iv;
	int     Iw;
	int		ErrIu;				//U相零漂大小
	int		ErrIv;				//V相零漂大小
	int     ErrIw;				//W相零漂大小
	int  	Count;
	int  	EnableCount;
	int  	ErrCnt;
}CUR_EXCURSION_STRUCT;//检测零漂使用的结构

typedef struct UV_AMP_COFF_STRUCT_DEF {
	Ulong	TotalU;
	Ulong	TotalV;
	Ulong	TotalVoltL;
	Ulong	TotalIL;
	Ulong	TotalVolt;
	Ulong	TotalI;
	Uint	Number;
	Uint	Comper;
	Uint	ComperL;
	Uint 	UDivVGet;
	Uint 	UDivV;
	Uint 	UDivVSave;
}UV_AMP_COFF_STRUCT;//检测两相电流检测增益偏差的数据结构

typedef struct TEMPLETURE_STRUCT_DEF{
	Uint	TempAD;				//AD获取值，由于温度查表
	int	    Temp;				//用度表示的实际温度值
	Uint	TempBak;			//用度表示的实际温度值
	Uint	ErrCnt;
}TEMPLETURE_STRUCT;//和变频器温度相关的数据结构

typedef struct FC_CAL_STRUCT_DEF{
	Uint	Cnt;
	Uint	Time;
	Uint	FcBak;
}FC_CAL_STRUCT;//计算载波频率程序使用的数据结构

typedef struct OVER_CUR_CTL_STRUCT_DEF{
	int		StepApply;			//实际使用的步长
	int		StepLow;			//电流小于0.88限流点的步长
	int		StepLowLim;			//电流小于0.88限流点的步长上限
	int		StepHigh;			//电流大于0.88限流点的步长
	int		CurBak;				//上一拍电流值
	int		LowFreq;			//低频转折点
	int		MaxStepLow;			//低于低频转折点的最大步长
	int		MaxStepHigh;		//高于低频转折点的最大步长
	int		CurLim;
	int		SubStep;			//电流大于0.88限流点时步长减小速度
	int		Flag;				//标志
								//BIT0	=1 表示速度已经追上设定速度
								//BIT15 =1 表示电流已经超过限流点的0.88倍
}OVER_CUR_CTL_STRUCT;//过流抑制模块使用的数据结构

typedef struct OVER_UDC_CTL_STRUCT_DEF{
	int		CoffApply;
	int		CoffAdd;
	int		Limit;
	int		StepApply;
    int     LastStepApply;
	int		StepBak;
	int		ExeCnt;
	int		UdcBak;
	int		Flag;
	int		FreqMax;			//减速第一拍的频率
	int     AccTimes;           //过压抑制导致的频率增加次数
	int		OvUdcLimitTime;
}OVER_UDC_CTL_STRUCT;//过流抑制模块使用的数据结构

typedef struct TORQUE_UP_STRUCT_DEF{
	int		CompPhase;
}TORQUE_UP_STRUCT;//转矩提升模块使用的数据结构

typedef struct DC_BRAKE_STRUCT_DEF{
	int			Time;			//计数用
	PID_STRUCT	PID;			//制动电压
}DC_BRAKE_STRUCT;//直流制动模块使用的数据结构

typedef struct BRAKE_CONTROL_STRUCT_DEF{
	Uint	Flag;				//当前开通/关断状态
}BRAKE_CONTROL_STRUCT;//制动电阻控制模块使用的数据结构

typedef enum ZERO_LENGTH_PHASE_SELECT_ENUM_DEF{
    ZERO_VECTOR_U,        //DPWM调制时，U相发全脉宽
    ZERO_VECTOR_V,        //DPWM调制时，V相发全脉宽
    ZERO_VECTOR_W,        //DPWM调制时，W相发全脉宽
    ZERO_VECTOR_NONE=100  //没有发全脉宽的相
}ZERO_LENGTH_PHASE_SELECT_ENUM;//PWM输出，指明哪一相发的是全脉宽

typedef struct OUT_VOLT_STRUCT_DEF {
	int  	Volt;				//Q12中间计算过程的输出电压和相位
	int   	VoltPhase;

	int  	VoltApply;			//Q12计算调制系数使用的输出电压和相位
	int     VoltDisplay;        //显示输出电压，基值为变频器额定电压
	int   	VoltPhaseApply;
}OUT_VOLT_STRUCT;

typedef struct PWM_OUT_STRUCT_DEF {
	long	U;
	long	V;
	long	W;				//该结构前面的参数不要改变

	ZERO_LENGTH_PHASE_SELECT_ENUM    gZeroLengthPhase; //DPWM调制时，指明哪一相发的是全脉宽
	Uint	gPWMPrd;		//计算得到载波周期（20ns为单位）
	Uint	gPWMPrdApply;	//中断中实际使用载波周期

	Uint	AsynModle;		//异步调制模式/同步调制模式选择
	Uint	PWMModle;		//连续调制模式/离散调制模窖≡?
	int    	SoftPWMCoff;
}PWM_OUT_STRUCT;	        //作为PWM输出的结构

typedef struct DEAD_BAND_STRUCT_DEF{
   int		DeadBand;			//死区时间(20ns单位)
   int		Comp;				//补偿时间
   Uint     CompMode;           // 0－补偿模式与通用300一致，1－280当前采用的死区补偿方式
   int     	CompU;
   int     	CompV;
   int     	CompW;
   int		MTPhase;
   Uint		InvCurFilter;
}DEAD_BAND_STRUCT; 	//死区补偿结构体定义

struct CBC_FLAG_STRUCT{
    Uint16  CBC_U:1;
    Uint16  CBC_V:1;
    Uint16  CBC_W:1;
    Uint16  RESV:13;
};

typedef struct OVER_LOAD_PROTECT_DEF{
   int		Cnt;
   int		FilterRealFreq;
   int		FilterInvCur;
   int		FilterMotorCur;
   BIT32_GROUP		InvTotal;
   BIT32_GROUP		MotorTotal;
}OVER_LOAD_PROTECT; 	//过载保护数据结构

typedef struct PHASE_LOSE_STRUCT_DEF{
   Ulong	Time;
   Ulong	TotalU;
   Ulong	TotalV;
   Ulong	TotalW;
   Uint		Cnt;
}PHASE_LOSE_STRUCT; 	//输出缺相判断程序

typedef struct INPUT_LOSE_STRUCT_DEF{
   Uint		Cnt;
}INPUT_LOSE_STRUCT; 	//输入缺相判断程序

typedef struct FAN_CTRL_STRUCT_DEF{
   Uint		EnableCnt;	//判断上电后至少1秒才能够开始启动
   Uint		RunCnt;		//判断运行时间至少10秒
}FAN_CTRL_STRUCT; 	//风扇控制程蚴姑结构变量

typedef struct VF_WS_COMP_STRUCT_DEF {
	Uint 	   Coff;
	long int   CompFreq;
    Uint       RatedComp;
	int		   FilterIT;
    int        DelayTime;
    int        FilterCoff; //滤波系数
    MT_STRUCT  WsCompMT;
    MT_STRUCT  WsCompMTApply;
}VF_WS_COMP_STRUCT;	//

typedef struct AI_STRUCT_DEF {
	Uint 	gAI1;
	Uint 	gAI2;
}AI_STRUCT;	//

typedef struct SYN_PWM_STRUCT_DEF {
	Ulong   FcApply;
}SYN_PWM_STRUCT;	//

typedef struct SHORT_GND_STRUCT_DEF {
	Uint 	Comper;
	Uint 	BaseUDC;
	Uint	Flag;
	int		ShortCur;
}SHORT_GND_STRUCT;	//上电对地短路检测数据结构

typedef struct LOAD_LOSE_STRUCT_DEF {
	Uint 	ErrCnt;
}LOAD_LOSE_STRUCT;	//输出掉载检测数据结构

typedef struct CPU_TIME_STRUCT_DEF {
	Ulong 	Motor2MsBase;
	Ulong	Function2MsBase;
	Ulong	ADCIntBase;
	Ulong	PWMIntBase;

	Uint 	Motor2Ms;			//电机控制2ms执行时间
	Uint	Function2Ms;		//功能部分2摸索执行时间
	Uint	ADCInt;				//ADC中断执行时间
	Uint	PWMInt;				//PWM中断执行时间
}CPU_TIME_STRUCT;	//统计模块执行时间的数据结构

struct FEISU_SIGNAL1 {     	// bits  description
    Uint16  UdcHigh:1;      // 表示母线电压过高
	Uint16  CHECK_GET:1; 	// 表示已经搜索到速度
    Uint16  REM2:1;   	
    Uint16  REM3:1;   // 
    Uint16  REM4:1;   // 
    Uint16  REM5:1;   // 
    Uint16  REM6:1;   //
    Uint16  REM7:1;   //
    Uint16  REM8:1;   //
    Uint16  REM9:1;   //
	Uint16  REM10:1;  //
	Uint16  REM11:1;  //
	Uint16  REM12:1;  //
	Uint16  SpdSig:1;  		// 速度的符号
	Uint16  CHECK_BIT14:1;  //
	Uint16  CHECK_BIT15:1;  //
};//转速跟踪标志1的bit位定义

union FEISU_SIG1_DEF {
   Uint16                all;
   struct FEISU_SIGNAL1  bit;
};

struct FEISU_SIGNAL2 {     // bits  description
    Uint16  FEISUS2REM0:1;  //
    Uint16  FEISUS2REM1:1;      // 
	Uint16  FEISUS2REM2:1; //get the speed already
    Uint16  FEISUS2REM3:1;   // 
    Uint16  FEISUS2REM4:1;   // 
    Uint16  FEISUS2REM5:1;   // 
    Uint16  AlmostOver:1;   // 
    Uint16  DelayOver:1;   //
    Uint16  FEISUS2REM8:1;   //
    Uint16  FEISUS2REM9:1;   //
    Uint16  FEISUS2REM10:1;   //
	Uint16  FEISUS2REM11:1;  //
	Uint16  FEISUS2REM12:1;  //
	Uint16  FEISUS2REM13:1;  //
	Uint16  FEISUS2REM14:1;  //
	Uint16  TwoCyclesOver:1;  //
};//转速跟踪标志2的bit位定义

union FEISU_SIG2_DEF {
   Uint16                all;
   struct FEISU_SIGNAL2  bit;
};

typedef struct FEISU_STRUCT_DEF{
	Uint	VoltCNT;

	int		SpeedLast;
}FEISU_STRUCT;	//转速跟踪使用变量的结构定义

typedef struct PM_INIT_POSITION_TYPE{
	int SubStep;
	int PeriodCnt;
	int Section;
	long PWMTs;
	long PWMTs1;
	long CurFirst;
	long Cur[7];
	int  Num;
}PM_INIT_POSITION;


typedef struct PM_FLUX_WEAK_DEF
{
	long Id;
	long Iq;
	int  IdMax;
	int  IqMax;
	int  FluxWeakCurr;
	int  IqLimit;
	long  TorqLim;
	
	
	int  RatioRef;
	int  Ratio;
	int  UdcLpf;
	long  UdMaxLpf;
	int  VoltOut;

	long IqLpf;
	long AdjustId;
	long AdjustId1; 
	long Omg;

	long VoltMax;
	long CurrCoef;
	long VoltCoef;

	int  Num;
	int  CoefFlux;
	
	long CoefIqMax;
	int  CoefIdComp;
	long VoltLpf;
	int  Mode;
	int  ud;
	int  uq;
	
	
	long AdIqMaxIntg;
	int  UdForIqMax;
	int  KpIqMax;

}PM_FLUX_WEAK;

/************************************************************
结构定义 END
************************************************************/

/************************************************************
变量引用 BEGIN
************************************************************/
/************************************************************/
/*****************以下为基本变量定义*************************/
extern PM_INIT_POSITION         gIPMInitPos;
extern PM_FLUX_WEAK				gFluxWeak;

extern INV_STRUCT 				gInvInfo;		//变频器信息
extern MOTOR_STRUCT 			gMotorInfo;		//电机信息
extern MOTOR_EXTERN_STRUCT		gMotorExtInfo;	//电机扩展信息
extern MOTOR_EXTERN_STRUCT		gMotorExtReg;	//电机扩展信息（电机参数辨识得到的数据）
extern MOTOR_EXTERN_STRUCT		gMotorExtPer;	//电机扩展信息
extern RUN_STATUS_STRUCT 		gMainStatus;	//主运行状态
extern BASE_COMMAND_STRUCT		gMainCmd;		//主命令

/************************************************************/
/**********以下为和电机控制相关设定参数定义******************/
extern BASE_PAR_STRUCT			gBasePar;	//基本运行参数
extern COM_PAR_INFO_STRUCT		gComPar;	//公共参数
extern VF_INFO_STRUCT			gVFPar;		//VF参数

extern VC_INFO_STRUCT			gVCPar;		//VC参数

extern ADC_STRUCT				gADC;		//ADC数据采集结构
extern UDC_STRUCT				gUDC;		//母线电压数据
extern IUVW_SAMPLING_STRUCT		gCurSamp;
extern UVW_STRUCT				gIUVW;		//定子三相电流
extern ALPHABETA_STRUCT			gIAlphBeta;	//定子两相坐标轴电?
extern MT_STRUCT				gIMT;		//MT轴系下的电流
extern AMPTHETA_STRUCT			gIAmpTheta;	//极坐标表示的电流
extern LINE_CURRENT_STRUCT		gLineCur;	

extern ANGLE_STRUCT				gPhase;		//角度结构
extern JUDGE_POWER_LOW			gLowPower;	//上电缓冲判断使用数据结构
extern JUDGE_ACC_DEC_STRUCT		gSpeedFlag;	//判断加减速标志的结构
extern CUR_EXCURSION_STRUCT		gExcursionInfo;//检测零漂使用的结构
extern DEAD_BAND_STRUCT			gDeadBand;
extern OUT_VOLT_STRUCT			gOutVolt;
extern Uint						gRatio;		//调制系数
extern PWM_OUT_STRUCT			gPWM;
extern FC_CAL_STRUCT			gFcCal;		//计算载波频率使用的数据结构
extern OVER_CUR_CTL_STRUCT		gOvCur;
extern OVER_UDC_CTL_STRUCT		gOvUdc;

extern TORQUE_UP_STRUCT			gTorqueUp;	//转矩提升用变量

extern DC_BRAKE_STRUCT			gDCBrake;	//直流制动用变量
extern BRAKE_CONTROL_STRUCT		gBrake;		//制动电阻控制用变量
extern TEMPLETURE_STRUCT		gTemperature;

extern OVER_LOAD_PROTECT		gOverLoad;
extern AI_STRUCT				gAI;
extern VF_WS_COMP_STRUCT		gWsComp;
extern PHASE_LOSE_STRUCT		gPhaseLose;
extern CBC_PROTECT_STRUCT       gCBCProtect;
extern SYN_PWM_STRUCT			gSynPWM;
extern SHORT_GND_STRUCT			gShortGnd;
extern LOAD_LOSE_STRUCT			gLoadLose;
extern FAN_CTRL_STRUCT			gFanCtrl;
extern CPU_TIME_STRUCT			gDSPActiveTime;

extern UV_AMP_COFF_STRUCT		gUVCoff;
extern FEISU_STRUCT				gFeisu;			//转速跟踪用变量
extern PID_STRUCT				gSpeedCheckPID;
extern Ulong					gBuffResCnt;	//缓冲电阻保护变量

extern Uint IdMaxSet;
extern Uint IqFWLimit;

extern Uint DCBrakeCur;
extern Uint AVREnableMode;
extern Uint MotorTestData1;
extern Uint MotorTestData2;
extern Uint MotorTestData3;

extern Uint const gInvCurrentTable[];
extern Uint const gInvOlCurrentTable[];

extern Uint const gDeadBandTable[];
extern Uint const gDeadCompTable[];

extern int  const m_pTable_x[];
extern Uint const m_pTable_y[];

extern Uint const gInvOverLoadTable[];
extern Uint const gMotorOverLoadTable[];
extern Uint * const gSendToMotorTable[];
extern Uint * const gSendToFuncTable[]; 

extern int pm_maxout_vol;
extern Uint MotorPWMRandom;
extern Uint MotorSpeedScaling2ONKB10;
extern int DCStart_flag;
extern int pm_forbid_revtorq;
extern int pm_especial_pi;
extern Uint FiveSegModeEn;
extern int pm_dq_angle;
extern int StartupState;

/************************************************************
END
************************************************************/



/************************************************************
函数引?BEGIN
************************************************************/
//主程序中的函数说明

extern void OverCurrentControl(void);
extern void OverUDCControl(void);
extern void CalOutVotInVFStatus(void);
extern void CalOutVoltInDCBrakeStatus(void);
extern void OutPutPWMVF(void);
extern void SendPWM(void);
extern void DeadBandComp(void);

extern void InitSetPWM(void);
extern void InitSetAdc(void);
extern void OutPutPWMPhaseShift(void);

// 导入到CCS5.2中有警告，增加下面声明
extern void ChangeCurrent(void);
extern void ParGetFromFunction(void);		
extern void PMFluxWeaking();
extern void ParSendToFunction(void);
extern void speed_pi();
extern void VFFreqCal(void);
extern void pmsvc_init();
extern void PMFluxWeakInit();
extern void CalDeadBandComp(void);
extern void PMCalcLineCurr();
extern void SynInitPosDetect(void);
extern void CBCLimitCurPrepare(void);
extern void BrakeResControl(void);
extern void AllowSelectBottomOnly(unsigned short Selection);

extern void CalRatioFromVot(void);

extern void SynPWMAngleCal(void);
extern void ADCProcess(void);
extern void GetUDCInfo(void);
extern void GetCurrentInfo(void);
extern void GetAIInfo(void);
extern void	GetTemperatureInfo(void);

extern void InvDeviceKontrol(void);
extern void OutputLoseAdd(void);
extern Uint MaxUVWCurrent(void);


extern void calc_bem_coef();
extern void current_pi();
extern void calc_out_volvect();
extern void calc_out_angle();
extern void calc_rotor_position1();
extern int GetPWMSector(void);
extern void SynInitPosDetCal(void);

//子函数说明
extern void UVWToAlphBetaAxes(UVW_STRUCT* , ALPHABETA_STRUCT* );
extern void AlphBetaToDQ(ALPHABETA_STRUCT* , int , MT_STRUCT* );
extern void DQToAmpTheta(MT_STRUCT* ,AMPTHETA_STRUCT* );

extern void ChangeCurrent(void);
extern void ChangeCurrent_2ms(void);
extern void PMSMSVC_2ms(void);

/************************************************************
函数引用 END
************************************************************/


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================

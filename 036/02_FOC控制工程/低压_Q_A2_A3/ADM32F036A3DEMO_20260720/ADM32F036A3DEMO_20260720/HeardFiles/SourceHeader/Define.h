/************************************************************
------------------该文件是程序的公共定义头文件---------------
************************************************************/
#ifndef MAIN_DEFINE_H
#define MAIN_DEFINE_H

#include "ADP32F03x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
	定义DSP芯片和时钟
************************************************************/
#define    DSP_CLOCK100		    100		//100MHz时钟
#define     DSP_CLOCK           100     //100MHz时钟
#define     PWM_CLK_DIV         0


#define     C_TIME_05MS     (DSP_CLOCK*500L)    //0.5ms对应的定时器1计数值
#define     C_TIME_2MS      (DSP_CLOCK*2000L)   //0.5ms对应的定时器1计数值
#define     TIME_ACCUMULATOR_INIT {0, 0, 0, 0, 0, 0, 0}

/************************************************************
    定义预编译宏定义
  See CCS Build/C2000 Complier/Predefined Symbols.....
************************************************************/
//#define    LIN_CONTROL_MODE
//#define    OPEN_LOOP_START             1
//#define    SHORT_GND_TEST              1        // 上电对地短路的检测。
//#define    IPD_PERIOD_AUTO_DECT	     1        // 脉冲注入启动,屏蔽为直流制动启动

/************************************************************
	定义软件版本和变量类型
************************************************************/
#define  	SOFT_VERSION		9999	    // 软件版本号
// Version  Date        Description
// 9999     20240904    Frist Draft1
// 9998

//typedef	long long 				llong;
//typedef	unsigned int			Uint;
//typedef	unsigned long			Ulong;
//typedef	unsigned long long 		Ullong;

#define  MOTOR_TO_FUNCTION_DATA_NUM	12
#define  FUNCTION_TO_MOTOR_DATA_NUM	26+22
#define  FUNCTION_TO_MOTOR_DATA_NUM1 34
#define  MOTOR_TO_FUNCTION_DATA_NUM1 10

/************************************************************
    定义Flash 等待时间：035B/036 相同。
************************************************************/
#define  READ_RAND_FLASH_WAITE		4//3		//Flash中运行的等待时间
#define  READ_PAGE_FLASH_WAITE		4//3		//
#define  READ_OTP_WAITE				5		//OTP中读数据的等待时间

#define CURRENT_SAMPLE_1SHUNT           1
#define CURRENT_SAMPLE_2SHUNT           2
#define CURRENT_SAMPLE_3SHUNT           3
#define CURRENT_SAMPLE_MOSFET           4

#define NORMAL_NOISE_PWM_MODE           0   // 常规模式               UP-DOWN,
#define LOW_NOISE_PWM_MODE              1   // 静音模式               UP-DOWN
#define LOW_NOISE_PWM_MODE_2            2   // 全转速静音模式。UP MODE

#define ADM32F036_A2                    1
#define ADM32F036_A3                    2

typedef enum {
    CURRENT_1SHUNT_NORAML_NOISE = 0,
    CURRENT_1SHUNT_LOW_NOISE    = 1,
    CURRENT_1SHUNT_LOW_NOISE_2  = 2,
    CURRENT_1SHUNT_LOW_NOISE_3  = 3,
    CURRENT_2SHUNT_NORAML_NOISE = 4
} CUR_SAMPLE_TYPE; // CurrentSampleType2;

#define FIXED_SWICTH_FREQUENCY          0
#define RANDOM_SWICTH_FREQUENCY         1

#define DPOINT15        15
#define PRE_CONST       0.5
#define F2D_CONST       (int32)((int32)1<<DPOINT15)
#define DQPRE_CONST     (int32)(PRE_CONST*F2D_CONST)

/************************************************************
    定义电机运行状态和控制模式变量
************************************************************/
#define STATUS_LOW_POWER		1		//欠压状态
#define STATUS_STOP				3		//停机状态
#define STATUS_GET_PAR			2		//参数辨识阶段
#define STATUS_SPEED_CHECK		4		//转速跟踪阶段
#define STATUS_RUN				5		//运行状态
#define STATUS_SHORT_GND		6		//对地短路检测阶段
#define STATUS_BRAKE            7       //逆风启动刹车阶段
#define OPEN_ACC                8       //开环启动
#define HALF_OPEN_ACC           9       //开环启动

enum {
    PMSM_CONTROL_MODE_VF,
    PMSM_CONTROL_MODE_FOC
};

enum {
    AVR_MODE_DEFAULT,   // 0: 传统方式，固定gRatio = m_Ratio / gUDC.DC
    AVR_MODE_DEFAULT_1, // 1: 方式1，固定 gRatio = m_Ratio / gUDC.DCFilter
    AVR_MODE_DEFAULT_2, // 2: 方式2，禁止AVR. gRatio = m_Ratio
    AVR_MODE_DEFAULT_3, // 3: 方式3，综合方式0和2，启动时候0，调制比大于3600 时候切换到2
};

enum {
    U_V_CUR_SEN,
    U_W_CUR_SEN,
    V_W_CUR_SEN
};

/************************************************************
    定义其他变量状态
************************************************************/
#define  COM_PHASE_DEADTIME		600	    //电流极性判断的超前角度

#define  NULL           0
#define  TRUE           1       //通用TRUE和FALSE定义
#define  FALSE          0

#define  SW_RANDOM_DISABLE        0
#define  SW_RANDOM_ENABLE         1

#define  MODLE_SEVEN_PWM     0       // 7 段 SVPWM
#define  MODLE_FIVE_DPWM     1       // 5 段 SVPWM


#define C_INIT_PRD          5000    //初始(10KHz)的PWM周期
#define C_MAX_DB            100     //初始死区大小1.0us
#define SHORT_GND_CMPR_INC  100     //1us对应计数器值

/************************************************************
常数定义 END
************************************************************/

/************************************************************
	共用的结构定义
************************************************************/
struct MAIN_COMMAND_STRUCT_DEF{
   Uint16    Start:1;
   Uint16    SpeedSearch:1;
   Uint16    StopDC:1;
   Uint16    TuneFull:1;
   Uint16    VcSvc:1;
   Uint16    VfVc:1;
   Uint16    StartDC:1;
   Uint16    TunePart:1;
   Uint16    SpeedRev:1;
   Uint16    ErrorOK:1;
   Uint16    SpeedStatus:2;
   Uint16    ShortGnd:2;
   Uint16    SecondDCPhase:1;
   Uint16    LastDCPhase:1;
}; //主命令字的bit安排
union MAIN_COMMAND_UNION_DEF {
   Uint16   all;
   struct MAIN_COMMAND_STRUCT_DEF  	bit;
};

struct SEND_STATUS_STRUCT_DEF{
   Uint16    TuneStep:1;
   Uint16    TuneOver:1;
   Uint16    SpeedSearchOver:1;
   Uint16    TorqueLimit:1;
   Uint16    StartStop:1;
   Uint16    CurrADErr:1;
   Uint16    LowUDC:1;
   Uint16    PerOvLoadInv:1;
   Uint16    PerOvLoadMotor:1;
   Uint16    Reserved:1;
   Uint16    TuneDelay:1;
   Uint16    PDPLow:1;
   Uint16    ShortGndOver:1;
   Uint16    OutOff:1;
   Uint16    RunEnable:2;
}; //状态字的bit安排
typedef union SEND_STATUS_UNION_DEF {
   Uint16   all;
   struct SEND_STATUS_STRUCT_DEF  	bit;
}SEND_STATUS_UNION;

#define LOCALF
#define LOCALD extern

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/

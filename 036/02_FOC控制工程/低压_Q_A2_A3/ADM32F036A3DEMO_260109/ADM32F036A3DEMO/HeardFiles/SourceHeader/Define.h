/************************************************************
------------------该文件是程序的公共定义头文件---------------
************************************************************/
#ifndef MAIN_DEFINE_H
#define MAIN_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
	定义DSP芯片和时钟
************************************************************/
#define    DSP_CLOCK100		    100		//100MHz时钟
//#define   DSP_CLOCK60			60		//60MHz时钟
//#define   DSP_CLOCK40			40		//60MHz时钟

#ifdef      DSP_CLOCK100
    #define     DSP_CLOCK           100     //100MHz时钟
    #define     PWM_CLK_DIV         0
#elif       DSP_CLOCK60
    #define     DSP_CLOCK           60      //60MHz时钟
    #define     PWM_CLK_DIV         0
#else
    #define     DSP_CLOCK           40      //40MHz时钟
    #define     PWM_CLK_DIV         0
#endif

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

typedef	long long 				llong;
typedef	unsigned int			Uint;
typedef	unsigned long			Ulong;
typedef	unsigned long long 		Ullong;

/************************************************************
	定义功能和算法传递数组大小
************************************************************/
#define  MOTOR_TO_FUNCTION_DATA_NUM	12		//性能部分传递给功能部分的参数个数
#define  FUNCTION_TO_MOTOR_DATA_NUM	26+22	//功能部分传递给性能部分的参数个数
#define  FUNCTION_TO_MOTOR_DATA_NUM1 34		//功能部分传递给性能部分的参数个数
#define  MOTOR_TO_FUNCTION_DATA_NUM1 10		//性能部分传递给功能部分的参数个数

/************************************************************
    定义Flash 等待时间：035B/036 相同。
************************************************************/
#define  READ_RAND_FLASH_WAITE		3		//Flash中运行的等待时间
#define  READ_PAGE_FLASH_WAITE		3		//
#define  READ_OTP_WAITE				5		//OTP中读数据的等待时间

#define CURRENT_SAMPLE_1SHUNT           1
#define CURRENT_SAMPLE_2SHUNT           2
#define CURRENT_SAMPLE_3SHUNT           3
#define CURRENT_SAMPLE_MOSFET           4

#define ADM32F036_A2                    1
#define ADM32F036_A3                    2

#define NORMAL_NOISE_PWM_MODE           0   // 常规模式               UP-DOWN,
#define LOW_NOISE_PWM_MODE              1   // 静音模式               UP-DOWN
#define LOW_NOISE_PWM_MODE_2            2   // 全转速静音模式。UP MODE

#define FIXED_SWICTH_FREQUENCY          0
#define RANDOM_SWICTH_FREQUENCY         1

#define DPOINT15        15
#define PRE_CONST       0.5
#define F2D_CONST       (long)((long)1<<DPOINT15)
#define DQPRE_CONST     (long)(PRE_CONST*F2D_CONST)

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

#ifdef  DSP_CLOCK100
    #define C_INIT_PRD          5000    //初始(10KHz)的PWM周期
    #define C_MAX_DB            100     //初始死区大小1.0us
    #define SHORT_GND_CMPR_INC  100     //1us对应计数器值
#elif   DSP_CLOCK60
#endif

/************************************************************
常数定义 END
************************************************************/

/************************************************************
	共用的结构定义
************************************************************/
struct MAIN_COMMAND_STRUCT_DEF{
   Uint    Start:1;					// 1 起动； 0 停机
   Uint    SpeedSearch:1;			// 1 速度搜索命令
   Uint    StopDC:1;				// 1 停机直流制动命令
   Uint    TuneFull:1;				// 1 保留)
   Uint    VcSvc:1;					// 1 VC；  0 SVC(固定为0)
   Uint    VfVc:1;					// 1 VF；  0 VC(固定为1)
   Uint    StartDC:1;				// 1 起动直流制动
   Uint    TunePart:1;				// 1 部分调谐(电流通道增益偏差检测允许)
   Uint    SpeedRev:1;				// 1 码盘方向反向标志(保留)
   Uint    ErrorOK:1;				// 1 故障处理完毕标志
   Uint    SpeedStatus:2;			// 0 恒速； 1 加速； 2 减速 (保留)
   Uint    ShortGnd:2;				// 11上电对地短路检测标志
   Uint    SecondDCPhase:1;         // 二次定位
   Uint    LastDCPhase:1;           // 二次定位,零电流
}; //主命令字的bit安排
union MAIN_COMMAND_UNION_DEF {
   Uint   all;
   struct MAIN_COMMAND_STRUCT_DEF  	bit;
};

struct SEND_STATUS_STRUCT_DEF{
   Uint    TuneStep:1;				// 1 参数辨识标志(保留)
   Uint    TuneOver:1;				// 1 参数辨识结束标志(保留)
   Uint    SpeedSearchOver:1;		// 1 转速跟踪结束标志
   Uint    TorqueLimit:1;			// 1 转矩限定生效标志(保留)
   Uint    StartStop:1;				// 1 运行/停机状态标志
   Uint    CurrADErr:1;				// 1 电流检测故障标志
   Uint    LowUDC:1;				// 1 母线电压欠压故障标志
   Uint    PerOvLoadInv:1;			// 1 变频器过载预报警标志
   Uint    PerOvLoadMotor:1;		// 1 电机过载预报警标志
   Uint    Reserved:1;				// 1 保留
   Uint    TuneDelay:1;				// 1 参数辨识结束后延时完成(保留)
   Uint    PDPLow:1;				// 1 已经有一次PDP生效标志(保留)
   Uint    ShortGndOver:1;			// 1 对地短路检测完毕标志
   Uint    OutOff:1;				// 1 变频器输出空开断开标志
   Uint    RunEnable:2;				// 11初始化完成，可以运行标志
}; //状态字的bit安排
typedef union SEND_STATUS_UNION_DEF {
   Uint   all;
   struct SEND_STATUS_STRUCT_DEF  	bit;
}SEND_STATUS_UNION;

struct SUB_COMMAND_STRUCT_DEF{
   Uint    SoftPWM:1;				// 1 随机PWM使能(保留)
   Uint    VarFcByTem:1;			// 1 载波频率随温度调整(固定为1)
   Uint    MotorType:1;				// 1 普通AC；0 变频AC(保留)
   Uint    MotorOvLoad:1;			// 1 电机过载保护使能
   Uint    InputLost:1;				// 1 输入缺陷保护使能
   Uint    OutputLost:1;			// 1 输出缺陷检测使能
   Uint    NoStop:1;				// 1 瞬停不停使能(保留)
   Uint    OverMod:1;				// 1 过调制使能(保留)
   Uint    FanNoStop:1;				// 1 停机直流制动等待时间内风扇运行标志
   Uint	   CBCEnable:1;				// 1 逐波限流功能使能标志
   Uint	   LoadLose:1;				// 1 输出掉载保护使能标志
}; //辅命令字的bit安排
union SUB_COMMAND_UNION {
   Uint   all;
   struct SUB_COMMAND_STRUCT_DEF  	bit;
};

#define LOCALF
#define LOCALD extern

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/

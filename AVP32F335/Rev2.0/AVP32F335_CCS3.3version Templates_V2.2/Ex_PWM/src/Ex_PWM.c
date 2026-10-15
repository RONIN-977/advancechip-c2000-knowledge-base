/******************************************************************************
文件名			: Example_AVP33xEPwmUpDownAQ.c
作者           	: 湖南进芯电子科技有限公司
功能描述      	: ePWM例程
                  本例程实现的功能：演示对ePWM1~3配置各种比较值和不同的动作形
                  式（具体见各组ePWM模块的初始化），输出相应的PWM波形。（定时
                  器向上/向下计数）
                  管脚分配：
               		- EPWM1A：GPIO0
               		- EPWM1B：GPIO1
               		- EPWM2A：GPIO2
               		- EPWM2B：GPIO3
               		- EPWM3A：GPIO4
               		- EPWM3B：GPIO5
创建时间      	: 06-25-2019
版本           	: V1.0
修订时间       	:
修订内容       	:
*******************************************************************************/

#include "AVP33x_Project.h"     // Device Headerfile and Examples Include File

// 自定义一个结构体，包含的信息有：ePWM模块结构体指针、比较值的变化方向、比较值的变化范围以及中断计数值
typedef struct
{
    volatile struct EPWM_REGS *EPwmRegHandle;
    Uint16 EPwm_CMPA_Direction;
    Uint16 EPwm_CMPB_Direction;
    Uint16 EPwmTimerIntCount;
    Uint16 EPwmMaxCMPA;
    Uint16 EPwmMinCMPA;
    Uint16 EPwmMaxCMPB;
    Uint16 EPwmMinCMPB;
}EPWM_INFO;

// 函数声明
void InitEPwm1Example(void);
void InitEPwm2Example(void);
void InitEPwm3Example(void);
__interrupt void epwm1_isr(void);
__interrupt void epwm2_isr(void);
__interrupt void epwm3_isr(void);
void update_compare(EPWM_INFO*);

// 本例程中用到的全局变量定义
EPWM_INFO epwm1_info;
EPWM_INFO epwm2_info;
EPWM_INFO epwm3_info;

// 各组ePWM模块的周期值和比较值宏定义
#define EPWM1_TIMER_TBPRD  2000  // Period register
#define EPWM1_MAX_CMPA     1950
#define EPWM1_MIN_CMPA       50
#define EPWM1_MAX_CMPB     1950
#define EPWM1_MIN_CMPB       50

#define EPWM2_TIMER_TBPRD  2000  // Period register
#define EPWM2_MAX_CMPA     1950
#define EPWM2_MIN_CMPA       50
#define EPWM2_MAX_CMPB     1950
#define EPWM2_MIN_CMPB       50

#define EPWM3_TIMER_TBPRD  2000  // Period register
#define EPWM3_MAX_CMPA      950
#define EPWM3_MIN_CMPA       50
#define EPWM3_MAX_CMPB     1950
#define EPWM3_MIN_CMPB     1050

// 比较值变化方向宏定义
#define EPWM_CMP_UP   1
#define EPWM_CMP_DOWN 0

void main(void)
{

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。初始化PLL、看门狗、使能外设时钟等
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
    // 配置ePWM1~3的GPIO引脚
    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表

    // 重新映射中断向量
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    PieVectTable.EPWM2_INT = &epwm2_isr;
    PieVectTable.EPWM3_INT = &epwm3_isr;
    EDIS;


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example

    // 停止ePWM的同步信号
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
    // 初始化ePWM1~3
    InitEPwm1Example();
    InitEPwm2Example();
    InitEPwm3Example();
    // 启动ePWM的同步信号
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;


// 步骤5：用户自己的代码

    // 使能ePWM1-3的CPU级中断
    IER |= M_INT3;

    // 使能ePWM1-3的PIE级中断
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;

    EINT;   // 使能全局中断
    ERTM;   // 使能全局实时中断


// 步骤6：空循环（可选）
    for(;;)
    {
        __asm("          NOP");
    }
}

// ePWM1中断服务程序
__interrupt void epwm1_isr(void)
{
    update_compare(&epwm1_info);                    // 更新CMPA和CMPB
    EPwm1Regs.ETCLR.bit.INT = 1;                    // 清除中断标志
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;         // 响应PIE同组其他中断
}

// ePWM2中断服务程序
__interrupt void epwm2_isr(void)
{
    update_compare(&epwm2_info);                    // 更新CMPA和CMPB
    EPwm2Regs.ETCLR.bit.INT = 1;                    // 清除中断标志
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;         // 响应PIE同组其他中断
}

// ePWM3中断服务程序
__interrupt void epwm3_isr(void)
{
    update_compare(&epwm3_info);                    // 更新CMPA和CMPB
    EPwm3Regs.ETCLR.bit.INT = 1;                    // 清除中断标志
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;         // 响应PIE同组其他中断
}

// 初始化ePWM1模块
void InitEPwm1Example()
{
    // 设置定时器模块
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // 定时器向上/向下计数
    EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;            // 设置定时周期
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // 禁止加载相位寄存器
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;            // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                       // 清定时器
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // ePWM模块时钟1分频：TBCLK = SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // 配置比较寄存器的装载模式：当定时计数值=0时从影子寄存器加载。
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // 置比较值
    EPwm1Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;
    EPwm1Regs.CMPB = EPWM1_MAX_CMPB;

    // 设置动作
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;              // 当定时器处于向上计数且计数值等于CMPA时，将ePWMxA拉高
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;            // 当定时器处于向下计数且计数值等于CMPA时，将ePWMxA拉低

    EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;              // 当定时器处于向上计数且计数值等于CMPB时，将ePWMxB拉高
    EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;            // 当定时器处于向下计数且计数值等于CMPB时，将ePWMxB拉低

    // 配置中断
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // 当定时器计数值等于0时，产生中断事件
    EPwm1Regs.ETSEL.bit.INTEN = 1;                  // 使能中断
    EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;             // 每3次事件触发一次中断

    // Information this example uses to keep track
    // of the direction the CMPA/CMPB values are
    // moving, the min and max allowed values and
    // a pointer to the correct ePWM registers
    epwm1_info.EPwm_CMPA_Direction = EPWM_CMP_UP;   // 初始化CMPA的变化方向为增大
    epwm1_info.EPwm_CMPB_Direction = EPWM_CMP_DOWN; // 初始化CMPB的变化方向为减小
    epwm1_info.EPwmTimerIntCount = 0;               // 初始化中断计数值为0
    epwm1_info.EPwmRegHandle = &EPwm1Regs;          // 初始化ePWM模块寄存器指针
    epwm1_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;        // 设置CMPA和CMPB的最大/最小值
    epwm1_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
    epwm1_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
    epwm1_info.EPwmMinCMPB = EPWM1_MIN_CMPB;
}

// 初始化ePWM2模块
void InitEPwm2Example()
{
    // 设置定时器模块
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // 定时器向上/向下计数
    EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;            // 设置定时周期
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // 禁止加载相位寄存器
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;            // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                       // 清定时器
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // ePWM模块时钟1分频：TBCLK = SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // 配置比较寄存器的装载模式：当定时计数值=0时从影子寄存器加载。
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // 设置比较值
    EPwm2Regs.CMPA.half.CMPA = EPWM2_MIN_CMPA;
    EPwm2Regs.CMPB = EPWM2_MIN_CMPB;

    // 设置动作
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;              // 当定时器处于向上计数且计数值等于CMPA时，将ePWMxA拉高
    EPwm2Regs.AQCTLA.bit.CBD = AQ_CLEAR;            // 当定时器处于向下计数且计数值等于CMPB时，将ePWMxA拉低

    EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // 当定时器计数值等于0时，将ePWMxB拉低
    EPwm2Regs.AQCTLB.bit.PRD = AQ_SET  ;            // 当定时器计数值等于周期值时，将ePWMxB拉高

    // 配置中断
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // 当定时器计数值等于0时，产生中断事件
    EPwm2Regs.ETSEL.bit.INTEN = 1;                  // 使能中断
    EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;             // 每3次事件触发一次中断

    epwm2_info.EPwm_CMPA_Direction = EPWM_CMP_UP;   // 初始化CMPA的变化为增大方向
    epwm2_info.EPwm_CMPB_Direction = EPWM_CMP_UP;   // 初始化CMPB的变化为增大方向
    epwm2_info.EPwmTimerIntCount = 0;               // 初始化中断计数值为0
    epwm2_info.EPwmRegHandle = &EPwm2Regs;          // 初始化ePWM模块寄存器指针
    epwm2_info.EPwmMaxCMPA = EPWM2_MAX_CMPA;        // 设置CMPA和CMPB的最大/最小值
    epwm2_info.EPwmMinCMPA = EPWM2_MIN_CMPA;
    epwm2_info.EPwmMaxCMPB = EPWM2_MAX_CMPB;
    epwm2_info.EPwmMinCMPB = EPWM2_MIN_CMPB;
}

// 初始化ePWM3模块
void InitEPwm3Example(void)
{
    // 设置定时器模块
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // 定时器向上/向下计数
    EPwm3Regs.TBPRD = EPWM3_TIMER_TBPRD;            // 设置定时周期
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // 禁止加载相位寄存器
    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;            // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                       // 清定时器
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // ePWM模块时钟1分频：TBCLK = SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // 配置比较寄存器的装载模式：当定时计数值=0时从影子寄存器加载。
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // S设置比较值
    EPwm3Regs.CMPA.half.CMPA = EPWM3_MIN_CMPA;
    EPwm3Regs.CMPB = EPWM3_MAX_CMPB;

    // 设置动作
    EPwm3Regs.AQCTLA.bit.PRD = AQ_SET;              // 当定时器计数值等于周期值时，将ePWMxA拉高
    EPwm3Regs.AQCTLA.bit.CBD = AQ_CLEAR;            // 当定时器处于向下计数且计数值等于CMPB时，将ePWMxA拉低

    EPwm3Regs.AQCTLB.bit.PRD = AQ_CLEAR;            // 当定时器计数值等于周期值时，将ePWMxB拉低
    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;              // 当定时器处于向上计数且计数值等于CMPA时，将ePWMxB拉高

    // 配置中断
    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // 当定时器计数值等于0时，产生中断事件
    EPwm3Regs.ETSEL.bit.INTEN = 1;                  // 使能中断
    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;             // 每3次事件触发一次中断

    epwm3_info.EPwm_CMPA_Direction = EPWM_CMP_UP;   // 初始化CMPA的变化为增大方向
    epwm3_info.EPwm_CMPB_Direction = EPWM_CMP_DOWN; // 初始化CMPB的变化为减小方向
    epwm3_info.EPwmTimerIntCount = 0;               // 初始化中断计数值为0
    epwm3_info.EPwmRegHandle = &EPwm3Regs;          // 初始化ePWM模块寄存器指针
    epwm3_info.EPwmMaxCMPA = EPWM3_MAX_CMPA;        // 设置CMPA和CMPB的最大/最小值
    epwm3_info.EPwmMinCMPA = EPWM3_MIN_CMPA;
    epwm3_info.EPwmMaxCMPB = EPWM3_MAX_CMPB;
    epwm3_info.EPwmMinCMPB = EPWM3_MIN_CMPB;
}

// 更新各个比较寄存器值的函数
void update_compare(EPWM_INFO *epwm_info)
{
    // 每10次中断更新一次CMPA和CMPB的值
    if(epwm_info->EPwmTimerIntCount == 10)
    {
        epwm_info->EPwmTimerIntCount = 0;

        // 控制CMPA的值在最小值和最大值之间按三角波的形式变化：增大->减小->增大->减小……
        if(epwm_info->EPwm_CMPA_Direction == EPWM_CMP_UP)
        {
            if(epwm_info->EPwmRegHandle->CMPA.half.CMPA < epwm_info->EPwmMaxCMPA)
            {
                epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
            }
            else
            {
                epwm_info->EPwm_CMPA_Direction = EPWM_CMP_DOWN;
                epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
            }
        }
        else
        {
            if(epwm_info->EPwmRegHandle->CMPA.half.CMPA == epwm_info->EPwmMinCMPA)
            {
                epwm_info->EPwm_CMPA_Direction = EPWM_CMP_UP;
                epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
            }
            else
            {
                epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
            }
        }

        // 控制CMPA的值在最小值和最大值之间按三角波的形式变化：增大->减小->增大->减小……
        if(epwm_info->EPwm_CMPB_Direction == EPWM_CMP_UP)
        {
            if(epwm_info->EPwmRegHandle->CMPB < epwm_info->EPwmMaxCMPB)
            {
                epwm_info->EPwmRegHandle->CMPB++;
            }
            else
            {
                epwm_info->EPwm_CMPB_Direction = EPWM_CMP_DOWN;
                epwm_info->EPwmRegHandle->CMPB--;
            }
        }
        else
        {
        if(epwm_info->EPwmRegHandle->CMPB == epwm_info->EPwmMinCMPB)
        {
        epwm_info->EPwm_CMPB_Direction = EPWM_CMP_UP;
        epwm_info->EPwmRegHandle->CMPB++;
        }
        else
        {
        epwm_info->EPwmRegHandle->CMPB--;
        }
        }
    }
    else
    {
        epwm_info->EPwmTimerIntCount++;
    }

    return;
}

//===========================================================================
// No more.
//===========================================================================

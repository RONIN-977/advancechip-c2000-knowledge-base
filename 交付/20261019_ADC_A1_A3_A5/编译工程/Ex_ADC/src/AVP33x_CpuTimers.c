/******************************************************************************
文件名                     : AVP33x_CpuTimers.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片32位CPU定时器模块初始化及函数定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // Headerfile Include File
#include "AVP33x_Examples.h"   // Examples Include File

struct CPUTIMER_VARS CpuTimer0;

// 如果用到BIOS或其他实时操作系统，请注释下面两条语句
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// InitCpuTimers:
//---------------------------------------------------------------------------
// 初始化CPU定时器寄存器函数
//
void InitCpuTimers(void)
{

    /* CPU定时器0初始化*/

    CpuTimer0.RegsAddr = &CpuTimer0Regs;    // 初始化地址指针指向相应的定时器寄存器
    CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;    // 初始化定时器周期寄存器为最大值
    CpuTimer0Regs.TPR.all  = 0;             // 初始化预分频系数为1，即就等于系统时钟
    CpuTimer0Regs.TPRH.all = 0;
    CpuTimer0Regs.TCR.bit.TSS = 1;          // 停止CPU定时器0
    CpuTimer0Regs.TCR.bit.TRB = 1;          // 将周期寄存器的值重新载入定时器寄存器（TIMH:TIM和PSCH:PSC）
    CpuTimer0.InterruptCount = 0;           // 复位中断计数器


    /* CPU定时器1和CPU定时器2初始化（CPU定时器1和CPU定时器2是为BIOS或其他实时操作系统预留的，如果有这方面打算请不要用这个定时器）*/

    CpuTimer1.RegsAddr = &CpuTimer1Regs;    // 初始化地址指针指向相应的定时器寄存器
    CpuTimer2.RegsAddr = &CpuTimer2Regs;
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;    // 初始化定时器周期寄存器为最大值
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    CpuTimer1Regs.TCR.bit.TSS = 1;          // 停止CPU定时器0
    CpuTimer2Regs.TCR.bit.TSS = 1;
    CpuTimer1Regs.TCR.bit.TRB = 1;          // 将周期寄存器的值重新载入定时器寄存器（TIMH:TIM和PSCH:PSC）
    CpuTimer2Regs.TCR.bit.TRB = 1;
    CpuTimer1.InterruptCount = 0;           // 复位中断计数器
    CpuTimer2.InterruptCount = 0;
}

//---------------------------------------------------------------------------
// ConfigCpuTimer:
//---------------------------------------------------------------------------
// 定时器配置函数：
// 根据输入参数"Freq"和"Period"初始化被选用的定时器。"Freq"的单位是MHz，"Period"的单位是us。本函
// 数被调用后，定时器处于停止状态，需要重新启动定时器才能开始定时功能。
//
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
    Uint32  temp;

    // 初始化定时周期
    Timer->CPUFreqInMHz = Freq;
    Timer->PeriodInUSec = Period;
    temp = (long) (Freq * Period);
    Timer->RegsAddr->PRD.all = temp;

    // 设置预分频系数为1，即定时时钟=系统时钟
    Timer->RegsAddr->TPR.all  = 0;
    Timer->RegsAddr->TPRH.all  = 0;

    // 初始化定时器控制寄存器
    Timer->RegsAddr->TCR.bit.TSS = 1;   // 0：启动计数；1：停止计数
    Timer->RegsAddr->TCR.bit.TRB = 1;   // 重载定时器
    Timer->RegsAddr->TCR.bit.SOFT = 1;
    Timer->RegsAddr->TCR.bit.FREE = 1;  // 仿真控制位在减计数到0时停止
    Timer->RegsAddr->TCR.bit.TIE = 1;   // 定时器中断使能控制位（0：禁止；1：使能）

    // 复位中断计数器
    Timer->InterruptCount = 0;
}

//===========================================================================
// End of file.
//===========================================================================

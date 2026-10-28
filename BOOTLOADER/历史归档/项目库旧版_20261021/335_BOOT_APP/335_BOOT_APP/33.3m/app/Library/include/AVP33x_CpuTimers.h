/******************************************************************************
文件名                     : AVP33x_CpuTimers.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x的32位CPU定时器寄存器定义：
             CpuTimer1和CpuTimer2是为实时操作系统预留的。
                                        如果有计划将来会用到DSP-BIOS或其他实时操作系统，请不要用这两个定时器，即注释掉对这两个定时器进
                                        行操作的相关代码。
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_CPU_TIMERS_H
#define AVP33x_CPU_TIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// CPU定时器寄存器位定义：
//
//
// TCR: 控制寄存器位定义:
struct  TCR_BITS {          // 位描述
    Uint16    rsvd1:4;      // 3:0   预留
    Uint16    TSS:1;        // 4     定时器启/停控制位
    Uint16    TRB:1;        // 5     定时器重载位
    Uint16    rsvd2:4;      // 9:6   预留
    Uint16    SOFT:1;       // 10    仿真模式位
    Uint16    FREE:1;       // 11
    Uint16    rsvd3:2;      // 12:13 预留
    Uint16    TIE:1;        // 14    中断使能位
    Uint16    TIF:1;        // 15    中断标志位
};

union TCR_REG {
    Uint16           all;
    struct TCR_BITS  bit;
};

// TPR: 预分频（低）寄存器位定义:
struct  TPR_BITS {          // 位描述
    Uint16     TDDR:8;      // 7:0   分配系数（低）
    Uint16     PSC:8;       // 15:8  分配计数（低）
};

union TPR_REG {
    Uint16           all;
    struct TPR_BITS  bit;
};

// TPRH: 预分频（高）寄存器位定义:
struct  TPRH_BITS {         // 位描述
    Uint16     TDDRH:8;     // 7:0   分配系数（低）
    Uint16     PSCH:8;      // 15:8  分配计数（高）
};

union TPRH_REG {
    Uint16           all;
    struct TPRH_BITS bit;
};

// TIM, TIMH: 定时器寄存器定义:
struct TIM_REG {
    Uint16  LSW;
    Uint16  MSW;
};

union TIM_GROUP {
    Uint32          all;
    struct TIM_REG  half;
};

// PRD, PRDH: 周期寄存器定义:
struct PRD_REG {
    Uint16  LSW;
    Uint16  MSW;
};

union PRD_GROUP {
    Uint32          all;
    struct PRD_REG  half;
};

//---------------------------------------------------------------------------
// CPU定时器寄存器文件:
//
struct CPUTIMER_REGS {
    union TIM_GROUP TIM;    // 定时器计数寄存器
    union PRD_GROUP PRD;    // 定时器周期寄存器
    union TCR_REG   TCR;    // 定时器控制寄存器
    Uint16          rsvd1;  // 与刘
    union TPR_REG   TPR;    // 定时器预分频寄存器（低）
    union TPRH_REG  TPRH;   // 定时器预分频寄存器（高）
};

//---------------------------------------------------------------------------
// CPU定时器相关变量：
//
struct CPUTIMER_VARS {
   volatile struct  CPUTIMER_REGS  *RegsAddr;
   Uint32    InterruptCount;
   float   CPUFreqInMHz;
   float   PeriodInUSec;
};

//---------------------------------------------------------------------------
// 函数原型和外部定义:
//
void InitCpuTimers(void);
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period);

extern volatile struct CPUTIMER_REGS CpuTimer0Regs;
extern struct CPUTIMER_VARS CpuTimer0;

// CpuTimer1和CpuTimer2是为DSP BIOS和其他实时操作系统预留的。如果用到了DSP BIOS或启停实时
// 操作系统，请注释掉CpuTimer1和CpuTimer2。
extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
extern volatile struct CPUTIMER_REGS CpuTimer2Regs;

extern struct CPUTIMER_VARS CpuTimer1;
extern struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// 有用的CPU定时器操作宏定义：
//
// 启动定时器:
#define StartCpuTimer0()   CpuTimer0Regs.TCR.bit.TSS = 0

// 停止定时器:
#define StopCpuTimer0()   CpuTimer0Regs.TCR.bit.TSS = 1

// 重新加载定时器的周期值:
#define ReloadCpuTimer0() CpuTimer0Regs.TCR.bit.TRB = 1

// 读定时器的值:
#define ReadCpuTimer0Counter() CpuTimer0Regs.TIM.all

// 读定时器的周期值:
#define ReadCpuTimer0Period() CpuTimer0Regs.PRD.all


// CpuTimer1和CpuTimer2是为DSP BIOS和其他实时操作系统预留的。如果用到了DSP BIOS或启停实时操作系统，
// 请注释掉CpuTimer1和CpuTimer2。

// 启动定时器:
#define StartCpuTimer1()   CpuTimer1Regs.TCR.bit.TSS = 0
#define StartCpuTimer2()   CpuTimer2Regs.TCR.bit.TSS = 0

// 停止定时器:
#define StopCpuTimer1()   CpuTimer1Regs.TCR.bit.TSS = 1
#define StopCpuTimer2()   CpuTimer2Regs.TCR.bit.TSS = 1

// 重新加载定时器的周期值:
#define ReloadCpuTimer1() CpuTimer1Regs.TCR.bit.TRB = 1
#define ReloadCpuTimer2() CpuTimer2Regs.TCR.bit.TRB = 1

// 读定时器的值:
#define ReadCpuTimer1Counter() CpuTimer1Regs.TIM.all
#define ReadCpuTimer2Counter() CpuTimer2Regs.TIM.all

// 读定时器的周期值:
#define ReadCpuTimer1Period() CpuTimer1Regs.PRD.all
#define ReadCpuTimer2Period() CpuTimer2Regs.PRD.all


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_CPU_TIMERS_H definition

//===========================================================================
// End of file.
//===========================================================================

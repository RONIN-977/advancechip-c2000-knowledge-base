/************************************************************************
 Project:             Automotive water pumps
 Filename:            PwmComm.c
 Partner Filename:    N/A
 Description:         Partner file of PwmComm.h
 Complier:            Code Composer Studio for ADM32F036, CCS Systems.
 CPU TYPE :           ADM32F036A3
*************************************************************************
 Copyright (c) 2024 **** Co., Ltd.
 All rights reserved.
*************************************************************************
*************************************************************************
 Revising History (ECL of this file):

************************************************************************/

/************************************************************************
 Beginning of File, do not put anything above here except notes
 Compiler Directives
*************************************************************************/

#include "PWMCommFunc.h"
#include "MotorInclude.h"

/*=============================================================================
 *   本文件内部私有结构体和变量声明
 *============================================================================*/
typedef struct
{
    Uint32 PwmLowCntTemp;
    Uint32 PwmHighCntTemp;
} PWM_COMM_DEF;
volatile PWM_CAPT_TypeDef pwm_capt_data;
static volatile PWM_COMM_DEF gPWM_COMM;
static volatile Uint32 g_pwm_high_valid = 0;

#define PWM_TIMER_CLK_HZ            100000000ULL
#define PWM_DEAL_PERIOD_MS          2U
#define PWM_LOSS_TIMEOUT_MS         2000U
#define PWM_LOSS_TIMEOUT_CNT        (PWM_LOSS_TIMEOUT_MS / PWM_DEAL_PERIOD_MS)
#define PWMFREQ_LIMIT_MIN           100U     // 最小捕获PWM频率限制   单位1HZ
#define PWMFREQ_LIMIT_MAX           1000U    // 最大捕获PWM频率限制   单位1HZ
#define FILTER_WIN_SIZE             16
#define PWM_DUTY_JUMP_LIMIT         100U    // 毛刺判断阈值，100代表10%

/*******************************************************************************
* Function Name  : PWMCommInit
* Description    : PWM 调速模块初始化函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWMCommInit(void)
{
    EALLOW;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15  = 2;     //设置GPIO滤波
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 5;     //5个时钟周期
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 15; //将 GPIO15 配置为外部中断触发源。
    XIntruptRegs.XINT1CR.bit.POLARITY = 3;     //外部中断双边沿触发
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;       //外部中断使能
    EDIS;

    gPWM_COMM.PwmHighCntTemp = 0;
    gPWM_COMM.PwmLowCntTemp = 0;
    pwm_capt_data.pwm_reg_periodCount = 0;
    pwm_capt_data.pwm_reg_high_periodCount = 0;
    pwm_capt_data.pwm_reg_low_periodCount = 0;
    pwm_capt_data.pwm_int_flag = 0;
    pwm_capt_data.pwm_cur_duty = 0;
    pwm_capt_data.pwm_final_duty = 0;
    pwm_capt_data.pwm_freq_err_flag = 0;
    pwm_capt_data.Pwm_freq = 0;

    g_pwm_high_valid = 0;
}

/*******************************************************************************
* Function Name  : Xint1_isr
* Description    : GPIO15 外部中断服务函数，用于捕获 PWM 输入信号的高、低电平计数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
interrupt void Xint1_isr(void)
{
    if (GpioDataRegs.GPADAT.bit.GPIO15 == 0)
    {
        gPWM_COMM.PwmHighCntTemp = 0xFFFFFFFF - CpuTimer0.RegsAddr->TIM.all;
        g_pwm_high_valid = 1;
    } else {
        if (g_pwm_high_valid == 1) {
            gPWM_COMM.PwmLowCntTemp = 0xFFFFFFFF - CpuTimer0.RegsAddr->TIM.all;
            pwm_capt_data.pwm_reg_low_periodCount  = gPWM_COMM.PwmLowCntTemp;
            pwm_capt_data.pwm_reg_high_periodCount = gPWM_COMM.PwmHighCntTemp;
            pwm_capt_data.pwm_reg_periodCount = pwm_capt_data.pwm_reg_low_periodCount + pwm_capt_data.pwm_reg_high_periodCount;
            pwm_capt_data.pwm_int_flag = 1;
        }
    }
    CpuTimer0.RegsAddr->TCR.bit.TRB = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/*******************************************************************************
* Function Name  : PwmInDeal
* Description    : PWM 捕获数据处理函数，用于计算 PWM 占空比、频率并进行异常检测和滤波处理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PwmInDeal(void)
{
    static int32 PWMH_timer = 0;
    static int32 PWML_timer = 0;
    static int32 FERQ_0vertimer = 0;

    static int32 Duty_Buffer[32] = {0};
    static int32 Buffer_Index = 0;

    Uint32 flag = 0;
    Uint64 period = 0;
    Uint64 high = 0;
    Uint32 cur_duty = 0;
    Uint32 cur_freq = 0;

    int32 Dutydiff_flag = 0;
    int32 Dutysum = 0;
    int32 i = 0;

    /* ========== PWM 占空比滤波缓存复位宏 ========== */
#define PWM_DUTY_FILTER_RESET()                 \
    do                                          \
    {                                           \
        Buffer_Index = 0;                       \
        Dutysum = 0;                            \
        Dutydiff_flag = 0;                      \
    } while (0)

    /* ========== 故障处理 ========== */
    if (gMainStatus.ErrorCode != 0)
    {
        pwm_capt_data.pwm_final_duty = 0;
        pwm_capt_data.pwm_cur_duty = 0;
        pwm_capt_data.Pwm_freq = 0;
        pwm_capt_data.pwm_freq_err_flag = 0;

        PWM_DUTY_FILTER_RESET();

        return;
    }

    /* ========== 信号丢失检测 ========== */
    if (GpioDataRegs.GPADAT.bit.GPIO15 == 0)
    {
        PWMH_timer = 0;
        if (++PWML_timer > PWM_LOSS_TIMEOUT_CNT)
        {
            pwm_capt_data.pwm_final_duty = 0;
            pwm_capt_data.pwm_cur_duty = 0;
            pwm_capt_data.Pwm_freq = 0;
            pwm_capt_data.pwm_freq_err_flag = 1;

            FERQ_0vertimer = 0;
            PWM_DUTY_FILTER_RESET();
            return;
        }
    }
    else
    {
        PWML_timer = 0;
        if (++PWMH_timer > PWM_LOSS_TIMEOUT_CNT)
        {
            pwm_capt_data.pwm_final_duty = 1000;
            pwm_capt_data.pwm_cur_duty = 1000;
            pwm_capt_data.Pwm_freq = 0;
            pwm_capt_data.pwm_freq_err_flag = 1;

            FERQ_0vertimer = 0;
            PWM_DUTY_FILTER_RESET();
            return;
        }
    }

    /* ========== 原子快照：避免中断正在改数据时主循环读取 ========== */
    DINT;
    flag = pwm_capt_data.pwm_int_flag;

    if (flag == 1)
    {
        pwm_capt_data.pwm_int_flag = 0;

        period = pwm_capt_data.pwm_reg_periodCount;
        high   = pwm_capt_data.pwm_reg_high_periodCount;
    }
    EINT;

    if (flag != 1)
    {
        return;
    }

    /* ========== 除零保护：必须保留 ========== */
    if ((period == 0) || (high > period))
    {
        pwm_capt_data.pwm_cur_duty = 0;
        pwm_capt_data.Pwm_freq = 0;
        pwm_capt_data.pwm_freq_err_flag = 1;

        if (++FERQ_0vertimer > 5)
        {
            pwm_capt_data.pwm_final_duty = 0;
            PWM_DUTY_FILTER_RESET();
        }
        return;
    }

    /* ========== 正常计算 ========== */
    cur_duty = (Uint32)((high * 1000ULL) / period);
    cur_freq = (Uint32)(PWM_TIMER_CLK_HZ / period);

    pwm_capt_data.pwm_cur_duty = cur_duty;
    pwm_capt_data.Pwm_freq = cur_freq;

    /* ========== 频率限制处理 ========== */
    if ((cur_freq < PWMFREQ_LIMIT_MIN) ||
        (cur_freq > PWMFREQ_LIMIT_MAX))
    {
        pwm_capt_data.pwm_freq_err_flag = 1;
        if (++FERQ_0vertimer > 5)
        {
            pwm_capt_data.pwm_final_duty = 0;
            pwm_capt_data.pwm_cur_duty = 0;
            PWM_DUTY_FILTER_RESET();
        }
        return;
    }

    pwm_capt_data.pwm_freq_err_flag = 0;
    FERQ_0vertimer = 0;

    Duty_Buffer[Buffer_Index] = cur_duty;
    Buffer_Index++;

    if (Buffer_Index >= FILTER_WIN_SIZE)
    {
        Buffer_Index = 0;
        Dutysum = 0;
        Dutydiff_flag = 0;

        for (i = 0; i < FILTER_WIN_SIZE; i++)
        {
            Dutysum += Duty_Buffer[i];
            if (i > 0)
            {
                int32 Dutydiff;
                if (Duty_Buffer[i] > Duty_Buffer[i - 1]) {
                    Dutydiff = Duty_Buffer[i] - Duty_Buffer[i - 1];
                } else {
                    Dutydiff = Duty_Buffer[i - 1] - Duty_Buffer[i];
                }
                if (Dutydiff >= PWM_DUTY_JUMP_LIMIT) {
                    Dutydiff_flag = 1;
                }
            }
        }

        if (Dutydiff_flag == 0)
        {
            pwm_capt_data.pwm_final_duty = Dutysum / FILTER_WIN_SIZE;
        }
    }
#undef PWM_DUTY_FILTER_RESET
}

/*******************************************************************************
* Function Name  : ReportError
* Description    : 故障反馈输出处理函数，根据当前故障码控制 GPIO14 输出对应的故障指示信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReportError(void)
{
    static int32 fault_hold_2ms = 0;   // 故障维持时间（2ms单位）
    static int32 counter_2ms = 0;      // 2ms计数器
    static int32 counter_1s = 0;       // 秒计数器
    static int32 latched_error = 0;

    // 无故障：恢复GPIO复用功能
    if (gMainStatus.ErrorCode == 0)
    {
        latched_error = 0;
        fault_hold_2ms = 0;
        counter_2ms = 0;
        counter_1s = 0;

        EALLOW;
        GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;  // 恢复复用功能
        EDIS;
        return;
    }

    if (latched_error == 0)
    {
        latched_error = gMainStatus.ErrorCode;
    }

    // 反馈信号延迟输出时间设置
    if (fault_hold_2ms <= 500)
    {
        fault_hold_2ms++;
        return;
    }

    // 设置GPIO为普通IO输出
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    EDIS;

    counter_2ms++;
    if (counter_2ms >= 500)  // 500 * 2ms = 1s
    {
        counter_2ms = 0;
        counter_1s++;
    }

    int32 GPIO_L_time1S = 10;   // 拉低默认10秒
    int32 GPIO_H_time1S = 5;    // 拉高默认5秒

    switch (latched_error)
    {
        case ERROR_OC_HARDWARE:  // 过流故障
            GPIO_H_time1S = 10;
            break;
        case ERROR_STALL:        // 堵转故障
            GPIO_H_time1S = 15;
            break;
        case ERROR_OT_IGBT:      // 过温故障
            GPIO_H_time1S = 20;
            break;
        default:
            GPIO_H_time1S = 5;
            break;
    }

    if (counter_1s < GPIO_L_time1S) {
        GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
    } else if (counter_1s < (GPIO_L_time1S + GPIO_H_time1S)) {
        GpioDataRegs.GPASET.bit.GPIO14 = 1;
    } else {
        counter_1s = 0;
    }
}

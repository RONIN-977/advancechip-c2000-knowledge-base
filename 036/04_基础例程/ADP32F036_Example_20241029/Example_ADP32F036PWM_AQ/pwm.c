#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

pwm PWM5B={UP,0,200};

//pwm 10K 频率，中央对其PWM模式（即计数器增减模式）
void InitEPwm5(void)
{
    volatile struct EPWM_REGS *PWMDef = &EPwm5Regs;
    //配置GPIO40 为 PWM7A功能引脚;配置GPIO41 为 PWM5B功能引脚
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
    EDIS;


    EALLOW;
    //T_PWM =2 x TBPRD x T_TBCLK
    PWMDef->TBPRD = frequency-1;     //频率
    //   Setup compare
    PWMDef->CMPA.half.CMPA = 0;  //PWMxA占空比
    PWMDef->CMPB = 0;  //PWMxB占空比

    PWMDef->TBPHS.half.TBPHS = 0x0000;            // Phase is 0
    PWMDef->TBCTR = 0x0000;                       // Clear counter

    // Setup TBCLK
    PWMDef->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up down，中间对其计数模式
    PWMDef->TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    PWMDef->TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    PWMDef->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT;
    PWMDef->TBCTL.bit.CLKDIV =    TB_DIV1;

    //Configuration Options for the Dead-Band Submodule
    PWMDef->DBCTL.bit.OUT_MODE = DB_DISABLE;        //S1 = 0; S0 = 0 关闭死区控制

    // Set actions AQ设置输出，即为当TBCTR到达CxU/CxD时，AQ的PWMxA/PWMxB输出的高低
    PWMDef->AQCTLA.bit.CAU = AQ_SET ;
    PWMDef->AQCTLA.bit.CAD = AQ_CLEAR;
    PWMDef->AQCTLB.bit.CBU = AQ_SET ;
    PWMDef->AQCTLB.bit.CBD = AQ_CLEAR;

    EDIS;
}

void InitEPwm1_AQ_DB(void)
{
    volatile struct EPWM_REGS *PWMDef = &EPwm1Regs;
    InitEPwm1Gpio();

    EALLOW;
    //T_PWM =2 x TBPRD x T_TBCLK
    PWMDef->TBPRD = frequency-1;            //频率
    //EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    PWMDef->CMPA.half.CMPA = 0;  //占空比
    PWMDef->TBPHS.half.TBPHS = 0x0000;            // Phase is 0
    PWMDef->TBCTR = 0x0000;                       // Clear counter

    // Setup TBCLK
    PWMDef->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up down
    PWMDef->TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    PWMDef->TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    PWMDef->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT;
    PWMDef->TBCTL.bit.CLKDIV =    TB_DIV1;
    PWMDef->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    PWMDef->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    PWMDef->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    PWMDef->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Set actions
    PWMDef->AQCTLA.bit.CAU = AQ_SET;
    PWMDef->AQCTLA.bit.CAD = AQ_CLEAR;

    //Configuration Options for the Dead-Band Submodule
    PWMDef->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        //S1 = 1; S0 = 1 死区控制有效
    //死区控制S0和S1同时关闭时，以下2个配置忽略
    PWMDef->DBCTL.bit.OUT_MODE = DBA_ALL;           //S5 = 0; S4 = 0
    PWMDef->DBCTL.bit.POLSEL = DB_ACTV_HIC;         //S3 = 1; S2 = 0

    //配置死区（死区有效情况下，以下配置才有效）
    PWMDef->DBFED = 100; // FED = 100 TBCLKs, 1us
    PWMDef->DBRED = 100; // RED = 100 TBCLKs  1us
    EDIS;
}

//改变占空比
void ChangePWMDutyCycle()
{
    switch(PWM5B.dutyDir)
    {
        case UP:
               if(PWM5B.dutyCycle >= frequency)
               {
                   PWM5B.dutyCycle = frequency-PWM5B.PWMstep;
                   PWM5B.dutyDir = DOWM;          //PWM呼吸灯减计数，渐灭
                   LED1_TOGGLE();

               }
               else
                   PWM5B.dutyCycle += PWM5B.PWMstep ;
        break;

        case DOWM:
               if(PWM5B.dutyCycle <= PWM5B.PWMstep )
               {
                   PWM5B.dutyCycle = PWM5B.PWMstep;
                   PWM5B.dutyDir = UP;            //PWM呼吸灯增计数，渐亮
                   LED1_TOGGLE();

               }
               else
                   PWM5B.dutyCycle -= PWM5B.PWMstep ;
        break;

        case STOP:
            PWM5B.dutyCycle = PWM5B.dutyCycle ;
        break;
    }
    EPwm5Regs.CMPB= PWM5B.dutyCycle;            //改变PWM5B占空比
    EPwm5Regs.CMPA.half.CMPA = PWM5B.dutyCycle; //改变PWM7A占空比
}



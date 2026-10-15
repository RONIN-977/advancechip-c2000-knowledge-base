#define  _PWM_COMM_FUNC_FILE_
#include "PWMCommFunc.h"
#include "MotorInclude.h"
PWM_CAPT_TypeDef pwm_capt_data;


void PWMCommInit(void)
{
    gPWM_COMM.PWMState = 0;
    gPWM_COMM.PWMStateBak = 0;

    gPWM_COMM.PwmHighCnt = 0;
    gPWM_COMM.PwmLowCnt = 0;

    gPWM_COMM.DutyCycle = 0;
    gPWM_COMM.DutyCycleLast = 0;
    gPWM_COMM.DutyCycleFilter = 0;
    gPWM_COMM.DutyCycleAcc = 0;

    gPWM_COMM.RunCmd = 0;
    gPWM_COMM.RunTimeCnt = 0;
    gPWM_COMM.SpeedRPM = 0;
    gPWM_COMM.SpeedRef = 0;

    gPWM_COMM.OverFreqTimeCnt = 0;

    gPWM_COMM.FirstPowerOn = 1;
    gPWM_COMM.FirstPowerOnCnt = 0;
    gPWM_COMM.PullDownPWMFlag = 0;

    gPWM_COMM.LowPowerCnt = 2500;

    gPWM_COMM.ErrorCodeState = 0;
    gPWM_COMM.ErrorCodeRestCnt = 0;
    gPWM_COMM.ErrorCodePeriodCnt = 0;
    gPWM_COMM.ErrorCodeSendCnt = 0;

    gPWM_COMM.HeartBitCnt = 0;
}

interrupt void Xint1_isr(void)
{
    //GpioDataRegs.GPASET.bit.GPIO16  = 1;
    if(GpioDataRegs.GPADAT.bit.GPIO15 == 0)
    {
        gPWM_COMM.PwmHighCntTemp = 0xFFFFFFFF - CpuTimer0.RegsAddr->TIM.all;       
    }
    else
    {
        gPWM_COMM.PwmLowCntTemp = 0xFFFFFFFF - CpuTimer0.RegsAddr->TIM.all;
		
        pwm_capt_data.pwm_reg_low_periodCount  = gPWM_COMM.PwmLowCntTemp>>9;
        pwm_capt_data.pwm_reg_high_periodCount = gPWM_COMM.PwmHighCntTemp>>9;

        pwm_capt_data.pwm_reg_periodCount = pwm_capt_data.pwm_reg_low_periodCount + pwm_capt_data.pwm_reg_high_periodCount;
    }
    pwm_capt_data.pwm_int_flag = 1;

    CpuTimer0.RegsAddr->TCR.bit.TRB = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    //GpioDataRegs.GPACLEAR.bit.GPIO16  = 1;
}

void LowerPowerSleep(void)
{
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;       //LOWER POWER

    asm(" RPT #199 || NOP ");
    asm(" RPT #199 || NOP ");

    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;
    EDIS;
}





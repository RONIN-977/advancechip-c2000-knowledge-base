
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

int f_KickDogM = 1,f_KickDogI = 1;                //喂狗变量
int looptimes = 0;                                //定时器中断循环变量
int ckfa_test = 0;
int ileak_rms_cnt = 0,ileak_ave_cnt = 0;
int Ileak_rms = 0,Ileak_ave = 0,Ileak_ave_z1 = 0;
long Ileak_rms_temp = 0;
long Ileak_rms_sum = 0;
long Ileak_ave_sum = 0;


//===================CPU定时器中断服务函数=======================//
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs");
interrupt void cpu_timer0_isr(void)
{
    //AD_Deal();                                                 //读取AD采样值
    switch(looptimes)
    {
        case 0:
            looptimes = 1;
            if(f_KickDogI>0)
            {
                f_KickDogI = 0;
                f_KickDogM = 1;
                ServiceDog();
            }
            timer_10ms++;
            timer_125ms++;
            timer_1s++;

            //-------停止喂狗,进行复位-------
            if(f_ckfa_stop==1)
            {
                if(++ckfa_sci_cnt>PRD_1S5)
                {
                    ckfa_sci_cnt = PRD_1S5;
                    f_ckfa_sci = 1;
                }
            }
            else
            {
                ckfa_sci_cnt = 0;
                f_ckfa_sci = 0;
            }

            //-------SCIL通讯处理-------
            ScilOverTimeDeal();
            ScilByteDeal();
        break;
        case 1:
            looptimes=0;
            if(f_i2cw_stop == 1)
            {
                if(++timer_i2c>PRD_10MS)
                {
                    timer_i2c = 0;
                    f_i2cw_stop = 0;
                }
            }
            else
            {
                timer_i2c = 0;
            }
        break;
        default:
            looptimes=0;
        break;
    }
    CpuTimer0Regs.TCR.bit.TIF=1;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

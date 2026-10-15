#include "Main.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
#include "Record.h"

void SynInitPosDetSetTs(void);
void SynInitPosDetCal(void);
 
//#define NEW_IPD_MODE    1
static int16 PWMPeriodBasedONMotorANDVDC;

/***************************************************************************
  Private Functions
 ***************************************************************************/
static void TurnToStopStatus(void);
static void SynInitPosDetSetPwm(Uint16 Section);


static void TurnToStopStatus()
{
    DisableDrive();
    ResetADCAndPWM();
}

// 启动时候检测初始位置....
// RunCaseStop 中先检测运行命令，设置 pm_check_ip_flag = 1，然后中断函数就会
// 调用SynInitPosDetect... 第一步进入gIPMInitPos.SubStep = 1
void SynInitPosDetect(void)
{
    int32  m_Cur;
    static int16 DisableCNT = 0;

    if ((gMainStatus.ErrorCode != 0) || (gMainCmd.Command.bit.Start == 0))
    {
        TurnToStopStatus();
        gMainStatus.RunStep = STATUS_STOP;
        return;
    }

    switch(gIPMInitPos.SubStep)
    {
        case 1:
        {                                   //检测合适的脉冲宽度
            gIPMInitPos.PeriodCnt   = 0;
            gIPMInitPos.Section     = 0;

            DisableDrive();

            // 10K, 50*10^3/10 = 5000. 1 分频。周期寄存器设置5000 为10KHZ。100US
            // 4 分频，则设置5000 周期为 400US.....
            // e,g:12VDC, L = (LD + LQ) = 430 + 470 = 900UH
            //     U = L*DI/DT = 0.9 * 5.5A / 0.4us = 12.37V
            //     DT = L * DI / U = 900 * 55 / 120 = 410US
            //        = (Ld + Lq) * gMotorInfo.Current / gUDC.DC
            //     DT 就是脉冲宽度的值，单位US。由此计算出4分频下对应的周期寄存器值
            //     TBPRD = 50*10^6*DT*10^-6/4 = 12.5*DT
            PWMPeriodBasedONMotorANDVDC = ((int32)(l_ld + l_lq) * (int32)gMotorInfo.Current) / gUDC.uDC;
            PWMPeriodBasedONMotorANDVDC = ((int32)PWMPeriodBasedONMotorANDVDC * 1050) >> 6;

            EALLOW;
            EPwm2Regs.TBCTL.bit.CLKDIV = 2;     //4分频
            EPwm3Regs.TBCTL.bit.CLKDIV = 2;
            EPwm4Regs.TBCTL.bit.CLKDIV = 2;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
            EPwm5Regs.TBCTL.bit.CLKDIV = 2;
#endif
            EPwm2Regs.TBPRD = PWMPeriodBasedONMotorANDVDC;
            EPwm3Regs.TBPRD = PWMPeriodBasedONMotorANDVDC;
            EPwm4Regs.TBPRD = PWMPeriodBasedONMotorANDVDC;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
            EPwm5Regs.TBPRD = PWMPeriodBasedONMotorANDVDC;
            EPwm5Regs.CMPA.half.CMPA = PWMPeriodBasedONMotorANDVDC - 10;
            EPwm5Regs.CMPB = 10;
#else
            EPwm2Regs.CMPB = PWMPeriodBasedONMotorANDVDC - 10;
            EPwm2Regs.ETSEL.bit.SOCASEL = 6;
#endif
            EDIS;

            SynInitPosDetSetPwm(7);
            gIPMInitPos.CurFirst = 0;
            gIPMInitPos.Num = 0;

            // 40 * 0.125MS = 5MS..
            DisableCNT++;
            if (DisableCNT == 40)
            {
                gIPMInitPos.SubStep = 4;
                DisableCNT = 0;
            }
            break;
        }
        case 4:                         //检测电流IV、IW
        {
            EnableDrive();

            m_Cur = GetCurrentIPD(gIPMInitPos.Section);
            switch(gIPMInitPos.PeriodCnt)
            {
                case 0:
                {
                    SynInitPosDetSetPwm(gIPMInitPos.Section);
                    break;
                }
                case 1:
                {
                    break;
                }
                case 2:
                {
                    gIPMInitPos.CurFirst = m_Cur;
                    SynInitPosDetSetPwm(7);
                    break;
                }
                case 3:
                {
                    gIPMInitPos.Section++;
                    gIPMInitPos.Cur[gIPMInitPos.Section] = m_Cur - gIPMInitPos.CurFirst;
                    if(gIPMInitPos.Section == 6)
                    {
                        gIPMInitPos.SubStep = 5;
                        gIPMInitPos.PeriodCnt = 0;
                    }
                }
                break;
                case 4:
                {
                    break;
                }
            }
            gIPMInitPos.PeriodCnt++;
            if (gIPMInitPos.PeriodCnt >= 5)
            {
                gIPMInitPos.PeriodCnt = 0;
            }
            break;
        }
        case 5:
        {
            gIPMInitPos.PeriodCnt ++;
            TurnToStopStatus();
            if(gIPMInitPos.PeriodCnt >= 4)
            {
                gIPMInitPos.SubStep = 6;
            }
            break;
        }
        case 6:
        {
            gIPMInitPos.Num ++;
            if(gIPMInitPos.Num >= 4)
            {
                gIPMInitPos.SubStep = 7;
                gIPMInitPos.Num = 4;
            }
            break;
        }
        default:
        {
            SynInitPosDetSetPwm(6);
            SynInitPosDetCal();
            TurnToStopStatus();
#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
            EPwm2Regs.ETSEL.bit.SOCASEL = 1;
#endif
            gIPMInitPos.SubStep = 0;
            pm_check_ip_flag = 0;
            EnableDrive();
            break;
        }
    }
}

static void SynInitPosDetSetPwm(Uint16 Section)
{
    EALLOW;
#ifdef POSTIVE_DRIVE_LOGIC
    switch(Section)
    {
        case 0:                                 //检测A+
        {// A+, B-, C-
            EPwm2Regs.AQCSFRC.all = 0x04;
            EPwm3Regs.AQCSFRC.all = 0x01;
            EPwm4Regs.AQCSFRC.all = 0x01;
            break;
        }
        case 1:                                 //检测A-
        {// A-, B+, C+
            EPwm2Regs.AQCSFRC.all = 0x01;
            EPwm3Regs.AQCSFRC.all = 0x04;
            EPwm4Regs.AQCSFRC.all = 0x04;
            break;
        }
        case 2:                                 //检测B+
        {// A-, B+, C-
            EPwm2Regs.AQCSFRC.all = 0x01;
            EPwm3Regs.AQCSFRC.all = 0x04;
            EPwm4Regs.AQCSFRC.all = 0x01;
            break;
        }
        case 3:                                 //检测B-
        {// A+, B-, C+
            EPwm2Regs.AQCSFRC.all = 0x04;
            EPwm3Regs.AQCSFRC.all = 0x01;
            EPwm4Regs.AQCSFRC.all = 0x04;
            break;
        }
        case 4:                                 //检测C+
        {// A-, B-, C+
            EPwm2Regs.AQCSFRC.all = 0x01;
            EPwm3Regs.AQCSFRC.all = 0x01;
            EPwm4Regs.AQCSFRC.all = 0x04;
            break;
        }
        case 5:                                 //检测C-
        {// A+, B+, C-
            EPwm2Regs.AQCSFRC.all = 0x04;
            EPwm3Regs.AQCSFRC.all = 0x04;
            EPwm4Regs.AQCSFRC.all = 0x01;
            break;
        }
        case 6:                                 //恢复PWM模块的寄存器设置
        {
            EPwm2Regs.DBCTL.all     = 0x000B;
            EPwm2Regs.AQCTLA.all    = 0x0061;
            EPwm2Regs.AQCTLB.all    = 0x00;
            EPwm2Regs.AQCSFRC.all   = 0x00;

            EPwm3Regs.DBCTL.all     = 0x000B;
            EPwm3Regs.AQCTLA.all    = 0x0061;
            EPwm3Regs.AQCTLB.all    = 0x00;
            EPwm3Regs.AQCSFRC.all   = 0x00;

            EPwm4Regs.DBCTL.all     = 0x000B;
            EPwm4Regs.AQCTLA.all    = 0x0061;
            EPwm4Regs.AQCTLB.all    = 0x00;
            EPwm4Regs.AQCSFRC.all   = 0x00;
            break;
        }
        default:                                //同步机初始位置角检测初始化寄存器
        {// A-, B-, C-
            EPwm2Regs.AQCSFRC.all   = 0x05;     //所有桥臂设置为低电平
            EPwm2Regs.DBCTL.all     = 0;        //没有死区时间
            EPwm2Regs.AQCTLA.all    = 0x000C;
            EPwm2Regs.AQCTLB.all    = 0x000C;

            EPwm3Regs.AQCSFRC.all   = 0x05;     //所有桥臂设置为低电平
            EPwm3Regs.DBCTL.all     = 0;
            EPwm3Regs.AQCTLA.all    = 0x000C;
            EPwm3Regs.AQCTLB.all    = 0x000C;

            EPwm4Regs.AQCSFRC.all   = 0x05;     //所有桥臂设置为低电平
            EPwm4Regs.DBCTL.all     = 0;
            EPwm4Regs.AQCTLA.all    = 0x000C;
            EPwm4Regs.AQCTLB.all    = 0x000C;
            break;
        }
    }
#else
    switch(Section)
    {
        case 0:                                 //检测A+
        {// A+, B-, C-
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x02;
            break;
        }
        case 1:                                 //检测A-
        {// A-, B+, C+
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x08;
            break;
        }
        case 2:                                 //检测B+
        {// A-, B+, C-
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x02;
            break;
        }
        case 3:                                 //检测B-
        {// A+, B-, C+
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x08;
            break;
        }
        case 4:                                 //检测C+
        {// A-, B-, C+
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x08;
            break;
        }
        case 5:                                 //检测C-
        {// A+, B+, C-
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x02;
            break;
        }
        case 6:                                 //恢复PWM模块的寄存器设置
        {
            EPwm2Regs.DBCTL.all     = 0x0007;
            EPwm2Regs.AQCTLA.all    = 0x0061;
            EPwm2Regs.AQCTLB.all    = 0x00;
            EPwm2Regs.AQCSFRC.all   = 0x00;

            EPwm3Regs.DBCTL.all     = 0x0007;
            EPwm3Regs.AQCTLA.all    = 0x0061;
            EPwm3Regs.AQCTLB.all    = 0x00;
            EPwm3Regs.AQCSFRC.all   = 0x00;

            EPwm4Regs.DBCTL.all     = 0x0007;
            EPwm4Regs.AQCTLA.all    = 0x0061;
            EPwm4Regs.AQCTLB.all    = 0x00;
            EPwm4Regs.AQCSFRC.all   = 0x00;
            break;
        }
        default:                                //同步机初始位置角检测初始化寄存器
        {
            EPwm2Regs.AQCSFRC.all   = 0x0A;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm2Regs.AQCTLA.all    = 0x000C;
            EPwm2Regs.AQCTLB.all    = 0x000C;

            EPwm3Regs.AQCSFRC.all   = 0x0A;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm3Regs.AQCTLA.all    = 0x000C;
            EPwm3Regs.AQCTLB.all    = 0x000C;

            EPwm4Regs.AQCSFRC.all   = 0x0A;
            EPwm4Regs.DBCTL.all     = 0;
            EPwm4Regs.AQCTLA.all    = 0x000C;
            EPwm4Regs.AQCTLB.all    = 0x000C;
            break;
        }
    }
#endif
    EDIS;
}

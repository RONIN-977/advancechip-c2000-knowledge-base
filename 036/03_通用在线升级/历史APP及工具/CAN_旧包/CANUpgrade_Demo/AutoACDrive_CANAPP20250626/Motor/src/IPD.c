
#include "MainInclude.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
#include "param_lr.h"
#include "Record.h"

void SynInitPosDetSetTs(void);
void SynInitPosDetSetPwm(Uint Section);
void SynInitPosDetCal(void);
 
//#define NEW_IPD_MODE    1
#define TS_L_VDC    60

void TurnToStopStatus()
{
	DisableDrive();
	reset_peripherals();	
}

// 启动时候检测初始位置....
// RunCaseStop 中先检测运行命令，设置 pm_check_ip_flag = 1，然后中断函数就会
// 调用SynInitPosDetect... 第一步进入gIPMInitPos.SubStep = 1
void SynInitPosDetect(void)
{
	long  m_Cur;long GetValue;
	enum {
		PWMTsState_init,
		PWMTsState_PeriodSet,
		PWMTsState_Delay1,
		PWMTsState_CurCheck,
		PWMTsState_Finished,
		PWMTsState_Failed
	};
	static int DisableCNT = 0;
    enum {
        PERIOD_0,
        PERIOD_1,
        PERIOD_2,
        PERIOD_3
    };

    if ((gMainStatus.ErrorCode != 0) || (gMainCmd.Command.bit.Start == 0))
	{
		TurnToStopStatus();
		gMainStatus.RunStep = STATUS_STOP;
		return;
	}

	switch(gIPMInitPos.SubStep)
	{
		case 1:	{									//检测合适的脉冲宽度
			gIPMInitPos.PeriodCnt 	= 0;
			gIPMInitPos.Section 	= 0;

			DisableDrive();

			// 40 * 0.125MS = 5MS..
			DisableCNT++;
			if (DisableCNT == 40) {
				gIPMInitPos.SubStep 	= 4;
				DisableCNT = 0;
			}

			// 之前是80，过压恢复时候可能会电压较高导致脉冲电流较大。
			// TS_L_VDC
			// 100MHZ, CLKDIV = 7, TS_L_VDC * 128 * 1 / 100MHZ = 75... ... ... ...
			// Toggle EPWMxA Output: at Period time, so 75 * 2 = 150US ... ... ...
			// U = L*I/T, SO I = U*T/(Ld+Lq) = 310.0V * 0.150 / 40MH = 1.2A... ...
			// T = ((l_ld + l_lq) * gMotorInfo.Current) / gUDC.uDCFilter = 40000 * 20 / 3100 = 258
			// 60 ~ 150, * 0.4,
			int PWMTsValue;
			PWMTsValue = ((l_ld + l_lq) * gMotorInfo.Current) / gUDC.uDCFilter;
			PWMTsValue = ((long)PWMTsValue * 410) >> 10;
			PWMTsValue = ((long)PWMTsValue * IPDCurrentScaling) >> 10;
			gIPMInitPos.PWMTs = PWMTsValue;
			SynInitPosDetSetPwm(7);
			gIPMInitPos.CurFirst = 0;
			gIPMInitPos.Num = 0;
			SynInitPosDetSetTs();
		}
		break;

		case 4:							//检测电流IV、IW
		    EnableDrive();
#ifndef NEW_IPD_MODE
            gIPMInitPos.PeriodCnt++;
            if (gIPMInitPos.PeriodCnt >= 3)
            {
                gIPMInitPos.PeriodCnt = 0;
            }
#else
            switch (PeriodState) {
            case PERIOD_0:
                gIPMInitPos.PeriodCnt = 1;
                PeriodState = 1;
            break;
            case PERIOD_1:
                delayCnt++;
                if (delayCnt > 500) {
                    delayCnt = 0;
                    gIPMInitPos.PeriodCnt = 2;
                    PeriodState = 2;
                }
            break;
            case PERIOD_2:
                gIPMInitPos.PeriodCnt = 3;
                PeriodState = 3;
            break;
            case PERIOD_3:
                gIPMInitPos.PeriodCnt = 0;
                PeriodState = 0;
            break;
            default:
                gIPMInitPos.PeriodCnt = 0;
                PeriodState = 0;
            break;
            }
#endif

			// m_Cur 正负对结果并没有影响... ...
			GetValue = GetCurrentIPD(gIPMInitPos.Section);
			m_Cur  = GetValue;

			switch(gIPMInitPos.PeriodCnt)
			{
				case 0:
					SynInitPosDetSetPwm(7);
					gIPMInitPos.Section++;
					gIPMInitPos.Cur[gIPMInitPos.Section]  = m_Cur - gIPMInitPos.CurFirst;

					if(gIPMInitPos.Section == 6)
					{
						gIPMInitPos.SubStep = 5;
						gIPMInitPos.PeriodCnt = 0;
					}
					break;

#ifdef NEW_IPD_MODE
                case 1:
                    SynInitPosDetSetPwm(7);
                    break;

                case 2:
                    SynInitPosDetSetPwm(gIPMInitPos.Section);
                    break;

                case 3:
                    gIPMInitPos.CurFirst = m_Cur;
                    break;
#else
                case 1:
                    SynInitPosDetSetPwm(gIPMInitPos.Section);
                    break;

                case 2:
                    gIPMInitPos.CurFirst = m_Cur;
                    break;
#endif
			}
			break;
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
			SynInitPosDetSetPwm(6);
			SynInitPosDetCal();
			TurnToStopStatus();

            StartupState = 4;

			gIPMInitPos.SubStep = 0;
			pm_check_ip_flag = 0;
			EnableDrive();
			break;
	}
}

void SynInitPosDetSetTs(void)
{
	// 确定脉冲宽度阶段和后面6个脉冲阶段 通用的PWMTs 实际的PWM 宽度不一样
	// 这在设置PWMTs = 30,实际测试宽度仍然不一样...
	// CMPA = 1248, Ts = 2500...
	// SubStep = 5,后 有一个TurnToStop 中会重新设置TBCTL（Hardware...)
	gIPMInitPos.PWMTs = __IQsat(gIPMInitPos.PWMTs,63000L,0);
	EALLOW;
	EPwm2Regs.TBCTL.bit.CLKDIV = 7;//128分频
    EPwm3Regs.TBCTL.bit.CLKDIV = 7;
    EPwm4Regs.TBCTL.bit.CLKDIV = 7;
	EPwm2Regs.TBPRD = gIPMInitPos.PWMTs;
	EPwm2Regs.CMPB  = EPwm2Regs.TBPRD;

	if (gIPMInitPos.SubStep == 1) {
		EPwm2Regs.ETSEL.bit.SOCASEL = 1;
	} else {
		EPwm2Regs.ETSEL.bit.SOCASEL = 2;
	}

	EPwm3Regs.TBPRD = gIPMInitPos.PWMTs;
	EPwm4Regs.TBPRD = gIPMInitPos.PWMTs;
	EDIS;
}

void SynInitPosDetSetPwm(Uint Section)
{
	Uint m_Section;

	m_Section = Section;

	EALLOW;
	switch(m_Section)
	{
#ifdef CUT_DOWN_VER_HARDWARE
	    case 0:									//检测A+
			// A+, B-, C-
			// 0X08，0000 1000， force  u- disabled.
			// 0X02，0000 0010， force  v+ disabled.
			// 0X02，0000 0010， force  w+ disabled.
			// so only u+ and v- is active................
			//////////////////////////////////////////////
			// 0x80 -- High on B
			// 0x20 -- High on A
			// 所以，老板硬件是U-,V+,W+全关断.新板也是这样，只是强制低
			///////////////////////////////////////////////////////
			EPwm2Regs.AQCSFRC.all = 0x04;
			EPwm3Regs.AQCSFRC.all = 0x01;
			EPwm4Regs.AQCSFRC.all = 0x01;
		break;

		case 1:									//检测A-
			// A+, B-, C-
			// 0X02，0000 0010， force  u+ disabled.
			// 0X08，0000 1000， force  v- disabled.
			// 0X08，0000 1000， force  w- disabled.
			// so only u+ and v- is active................
			//////////////////////////////////////////////
			EPwm2Regs.AQCSFRC.all = 0x01;
			EPwm3Regs.AQCSFRC.all = 0x04;
			EPwm4Regs.AQCSFRC.all = 0x04;
		break;

		case 2:									//检测B+
			// A+, B-, C-
			// 0X02，0000 0010， force  u+ disabled.
			// 0X08，0000 1000， force  v- disabled.
			// 0X02，0000 0010， force  w+ disabled.
			// so only u+ and v- is active................
			//////////////////////////////////////////////
			EPwm2Regs.AQCSFRC.all = 0x01;
			EPwm3Regs.AQCSFRC.all = 0x04;
			EPwm4Regs.AQCSFRC.all = 0x01;
		break;

		case 3:									//检测B-
			EPwm2Regs.AQCSFRC.all = 0x04;
			EPwm3Regs.AQCSFRC.all = 0x01;
			EPwm4Regs.AQCSFRC.all = 0x04;
		break;

		case 4:									//检测C+
			EPwm2Regs.AQCSFRC.all = 0x01;
			EPwm3Regs.AQCSFRC.all = 0x01;
			EPwm4Regs.AQCSFRC.all = 0x04;
		break;

		case 5:									//检测C-
			EPwm2Regs.AQCSFRC.all = 0x04;
			EPwm3Regs.AQCSFRC.all = 0x04;
			EPwm4Regs.AQCSFRC.all = 0x01;
		break;

		case 6:									//恢复PWM模块的寄存器设置
			EPwm2Regs.DBCTL.all 	= 0x000B;

			// TMS2802x, that is
			// CAD - 01, ePWMA low decreasing
			// CAU - 10, ePWMA high increasing
			// PRD - 00, do nothing
			// ZRO - 01, ePWMA low
			///////////////////////
			EPwm2Regs.AQCTLA.all 	= 0x0061;
			EPwm2Regs.AQCTLB.all 	= 0x00;
			EPwm2Regs.AQCSFRC.all	= 0x00;
			EPwm3Regs.DBCTL.all 	= 0x000B;

			EPwm3Regs.AQCTLA.all 	= 0x0061;
			EPwm3Regs.AQCTLB.all 	= 0x00;
			EPwm3Regs.AQCSFRC.all 	= 0x00;
			EPwm4Regs.DBCTL.all 	= 0x000B;

			EPwm4Regs.AQCTLA.all 	= 0x0061;
			EPwm4Regs.AQCTLB.all 	= 0x00;
			EPwm4Regs.AQCSFRC.all 	= 0x00;
		break;

		default:								//同步机初始位置角检测初始化寄存器
			EPwm2Regs.AQCSFRC.all 	= 0x05;		//所有桥臂设置为低电平
			EPwm2Regs.DBCTL.all 	= 0;		//没有死区时间
			EPwm2Regs.AQCTLA.all 	= 0x000C;
			EPwm2Regs.AQCTLB.all 	= 0x000C;

			EPwm3Regs.AQCSFRC.all 	= 0x05;		//所有桥臂设置为低电平
			EPwm3Regs.DBCTL.all 	= 0;
			EPwm3Regs.AQCTLA.all 	= 0x000C;
			EPwm3Regs.AQCTLB.all 	= 0x000C;

			EPwm4Regs.AQCSFRC.all 	= 0x05;		//所有桥臂设置为低电平
			EPwm4Regs.DBCTL.all 	= 0;
			EPwm4Regs.AQCTLA.all 	= 0x000C;
			EPwm4Regs.AQCTLB.all 	= 0x000C;
		break;
#else
        case 0:                                 //检测A+
            // A+, B-, C-
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x02;
        break;

        case 1:                                 //检测A-
            // A+, B-, C-
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x08;
        break;

        case 2:                                 //检测B+
            // A+, B-, C-
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x02;
        break;

        case 3:                                 //检测B-
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x08;
        break;

        case 4:                                 //检测C+
            EPwm2Regs.AQCSFRC.all = 0x02;
            EPwm3Regs.AQCSFRC.all = 0x02;
            EPwm4Regs.AQCSFRC.all = 0x08;
        break;

        case 5:                                 //检测C-
            EPwm2Regs.AQCSFRC.all = 0x08;
            EPwm3Regs.AQCSFRC.all = 0x08;
            EPwm4Regs.AQCSFRC.all = 0x02;
        break;

        case 6:
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

        default:
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
#endif
	}
	EDIS;
}

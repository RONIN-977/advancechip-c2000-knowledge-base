#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

pwm PWM7B={UP,0,200};

//pwm 10K 频率，中央对其PWM模式（即计数器增减模式）
//-----------------------------------------------
void InitEPwm7(void)
{
	volatile struct EPWM_REGS *PWMDef = &EPwm7Regs;
	//配置GPIO40 为 PWM7A功能引脚;配置GPIO41 为 PWM7B功能引脚
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO40 = 1;    // Disable pull-up on GPIO40 (EPWM7A)
	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 1;   // Configure GPIO40 as EPWM7A
	GpioCtrlRegs.GPBPUD.bit.GPIO41 = 1;    // Disable pull-up on GPIO41 (EPWM7B)
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 1;   // Configure GPIO41 as EPWM7B
	EDIS;


	EALLOW;
    //T_PWM =2 x TBPRD x T_TBCLK
    PWMDef->TBPRD = frequency;	 //频率
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

	// ----->Figure 31. Configuration Options for the Dead-Band Submodule
	PWMDef->DBCTL.bit.OUT_MODE = DB_DISABLE;	    //S1 = 0; S0 = 0 关闭死区控制

	// Set actions 设置输出，即为当TBCTR到达CxU/CxD时，PWM引脚输出的高低电平
	PWMDef->AQCTLA.bit.CAU = AQ_SET ;
	PWMDef->AQCTLA.bit.CAD = AQ_CLEAR;
	PWMDef->AQCTLB.bit.CBU = AQ_SET ;
	PWMDef->AQCTLB.bit.CBD = AQ_CLEAR;
	EDIS;
}


//dutyDir = 1 ,PWM输出
//改变占空比
void ChangePWMDutyCycle()
{
	switch(PWM7B.dutyDir)
	{
		case UP:
		{
			   if(PWM7B.dutyCycle >= frequency)
			   {
				   PWM7B.dutyCycle = frequency;
				   PWM7B.dutyDir = DOWM;			//PWM减计数
			   }
			   else
			   {
				   PWM7B.dutyCycle += PWM7B.PWMstep ;
			   }
		}
		break;

		case DOWM:
		{
			   if(PWM7B.dutyCycle <= PWM7B.PWMstep )
			   {
				   PWM7B.dutyCycle = 0;
				   PWM7B.dutyDir = UP;			//PWM增计数
			   }
			   else
			   {
				   PWM7B.dutyCycle -= PWM7B.PWMstep ;
			   }
		}
		break;

		case STOP:
		{
			 PWM7B.dutyCycle = PWM7B.dutyCycle ;
		}
		break;
	}
	EPwm7Regs.CMPA.half.CMPA = PWM7B.dutyCycle; //改变PWM7A占空比
	EPwm7Regs.CMPB= PWM7B.dutyCycle; //改变PWM7B占空比
}


//扫描按键处理函数
void handleScanKeyforPWM(Scan * PtrSegTask)
{
    Uint16 i=0;

    //这里做串口显示哪个按键按下处理
    for(i=0;i<KeyNUM;i++)
    {
        if((PtrSegTask ->key[i].ClickCNT == 1)  && (PtrSegTask ->key[i].pressTick ==0))
        {
            printf("\r\nscan key is %s",keyStr[i]);
        }
    }

    //暂停或者启动
	if((PtrSegTask ->key[KEY1].ClickCNT == 1)  && (PtrSegTask ->key[KEY1].pressTick ==0))
	{
	    PtrSegTask ->key[KEY1].ClickCNT = 0;
	    PWM7B.PWMstep = PWM7B.PWMstep;
		if(PWM7B.dutyDir != STOP)
		{
			PWM7B.dutyDir = STOP;
			//暂停时，数码管显示当前占空比
			Running.showNum = ((float)PWM7B.dutyCycle/(float)(frequency))*100;
			Running.showhex[3]=0x0f;
			printf("停止PWMstep变化,占空比为 %0.4f%%",Running.showNum);
		}
		else
		{
			PWM7B.dutyCycle = 0;
			PWM7B.dutyDir = UP;
			Running.showNum =  PWM7B.PWMstep;
			Running.showhex[3]=SEGHexBLK;
			printf("启动PWMstep变化");
		}

	}

	//PWM占空比改变增大
	if((PtrSegTask ->key[KEY2].ClickCNT == 1)  && (PtrSegTask ->key[KEY2].pressTick ==0))
	{
	    PtrSegTask ->key[KEY2].ClickCNT = 0;
	    PWM7B.PWMstep = (PWM7B.PWMstep >= frequency/2)?frequency/2:PWM7B.PWMstep+100;
		printf("PWMstep值为 %d",PWM7B.PWMstep);
		Running.showNum = PWM7B.PWMstep;
	}

	//PWM占空比改变减小
	if((PtrSegTask ->key[KEY3].ClickCNT == 1)  && (PtrSegTask ->key[KEY3].pressTick ==0))
	{
	    PtrSegTask ->key[KEY3].ClickCNT = 0;
	    PWM7B.PWMstep = (PWM7B.PWMstep <= 100)?100:PWM7B.PWMstep-100;
		printf("PWMstep值为 %d",PWM7B.PWMstep);
		Running.showNum = PWM7B.PWMstep;
	}

}


void InitEPWM_AQ_DB(void)
{
	char i;
	volatile struct EPWM_REGS *PWMDef[] = {&EPwm1Regs,&EPwm2Regs,&EPwm3Regs};
	//配置GPIO40 为 PWM7A功能引脚
	InitEPwm1Gpio();
	InitEPwm2Gpio();
	InitEPwm3Gpio();

	for(i=0;i<3;i++)
	{
		EALLOW;
		//T_PWM =2 x TBPRD x T_TBCLK
		PWMDef[i]->TBPRD = 1500;			//频率
		//EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
		PWMDef[i]->CMPA.half.CMPA = 0;  //占空比
		PWMDef[i]->TBPHS.half.TBPHS = 0x0000;            // Phase is 0
		PWMDef[i]->TBCTR = 0x0000;                       // Clear counter

		// Setup TBCLK
		PWMDef[i]->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up down
		PWMDef[i]->TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
		PWMDef[i]->TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
		PWMDef[i]->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT;
		PWMDef[i]->TBCTL.bit.CLKDIV =    TB_DIV1;
		PWMDef[i]->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		PWMDef[i]->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
		PWMDef[i]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		PWMDef[i]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

		// Set actions
		PWMDef[i]->AQCTLA.bit.CAU = AQ_SET;
		PWMDef[i]->AQCTLA.bit.CAD = AQ_CLEAR;

		//Configuration Options for the Dead-Band Submodule
		PWMDef[i]->DBCTL.bit.OUT_MODE = DBA_ALL;           //S5 = 0; S4 = 0
		PWMDef[i]->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//S1 = 1; S0 = 1
		PWMDef[i]->DBCTL.bit.POLSEL = DB_ACTV_HIC; 		//S3 = 1; S2 = 0

		PWMDef[i]->DBFED = 100; // FED = 100 TBCLKs, 1us
		PWMDef[i]->DBRED = 100; // RED = 100 TBCLKs  1us
		EDIS;
}

#ifdef PWMADC
   // Interrupt where we will change the Deadband   //HIKE , ePWMInterrupt (EPWMx_INT) SelectionOptions
//   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event //HIKE , Enable event time-base counter equal to zero.(TBCTR =0x0000)
//   EPwm1Regs.ETSEL.bit.INTEN = 1;               // Enable INT
//   EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;          // Generate INT on 3rd event,  第ET_3RD 产生PWM中断
	   EPwm1Regs.ETSEL.bit.SOCAEN	= 1;		// Enable SOC on A group
	   EPwm1Regs.ETSEL.bit.SOCASEL	= ET_CTR_PRD;		//
	   EPwm1Regs.ETPS.bit.SOCAPRD 	= 1;		// Generate pulse on 1st event

#endif

}


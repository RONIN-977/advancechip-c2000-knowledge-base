//-----------------------------------------------------------------------------
#include "MainInclude.h"

CPU_TIME_STRUCT   gCpuTime;

static void SelectMode_Boot(void);
static void JumpApp_DoAction(void);

void LED_blinky(void)
{
    gCpuTime.nLEDSumCnt ++;
    if(gCpuTime.nLEDSumCnt >= 100)
    {
       gCpuTime.nLEDSumCnt = 0;
//       GpioDataRegs.GPADAT.bit.GPIO3 = ~GpioDataRegs.GPADAT.bit.GPIO3;
    }
}


void SelectMode_Boot(void)
{
    if (gCpuTime.nBootMode == 0)            //LIN引导
    {
        LIN_Boot();
    } else if (gCpuTime.nBootMode == 2)     //其他引导
    {

    } else{
    }
}


void Main05msFunction(void)
{
     SelectMode_Boot();
}

// 从Boot跳转到APP实现方法
// asm("    MOVL XAR7, #0x300000"); //0x300000是APP程序的入口地址
// asm("    LB *XAR7");
void JumpApp_DoAction(void)
{
    //当nBootFlag == 0xAA 表示已经有APP程序，不需要升级可以跳转APP。
    //在延时跳转期间接收到上位机的升级指令nBootFlag = 0,Boot程序进入升级流程。
    gCpuTime.nBootFlag = *(Uint16 *)Boot_FlagAdd;

    if ((gCpuTime.nBootFlag == 0xAA)&&(RemoteModbus.nAppJumpFlag == 1))
    {
        gCpuTime.nBootSumCnt++;
        if (gCpuTime.nBootSumCnt >= 500) //1s
        {
            gCpuTime.nBootSumCnt = 0;
            DINT;
            (*Jump_App)();
        }
    }
}

void Main2msFunction(void)
{
    LED_blinky();
    JumpApp_DoAction();
}

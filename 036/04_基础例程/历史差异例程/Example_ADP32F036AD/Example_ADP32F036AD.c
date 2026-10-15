
//
// Included Files
//
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"     // Device Headerfile and Examples Include File

//
// Prototype statements
//
void InitEPwm1Example(void);
void InitEPwm2Example(void);
void InitEPwm3Example(void);
__interrupt void epwm1_tzint_isr(void);
__interrupt void epwm2_tzint_isr(void);
__interrupt void epwm3_tzint_isr(void);
//
// Globals
//
Uint32  EPwm1TZIntCount;
Uint32  EPwm2TZIntCount;
Uint32  EPwm3TZIntCount;
//
// Main
//
void main(void)
{
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the DSP2803x_SysCtrl.c file.
    //
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the DSP2803x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example

    //
    // For this case just init GPIO pins for ePWM1, ePWM2, and TZ pins
    //
    InitEPwm1Gpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
      // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
      // RAM里面运行的代码拷贝到flash运行
 #if DEBUG_EN    > 0
      //
 #else
    //  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
     // InitFlash();            //初始化flash
 #endif

      // 第二步: 清除所有中断标志并初始化PIE中断向量表
      DINT;           //先禁止CPU中断
      InitPieCtrl();  //给PIE寄存器赋初值
      IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
      IFR = 0x0000;
      InitPieVectTable();//初始化中断向量表

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;
    PieVectTable.EPWM2_TZINT = &epwm2_tzint_isr;
    PieVectTable.EPWM3_TZINT = &epwm3_tzint_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize all the Device Peripherals:
    // Not required for this example
    //
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;        // Enable Clock to the ADC

    //
    // Comparator shares the internal BG reference of the ADC,
    // must be powered even if ADC is unused
    //
    AdcRegs.ADCCTL1.bit.ADCBGPWD = 1;

    DELAY_US(1000L);             // Delay to allow BG reference to settle.

    //
    // Enable clock to the Comparator 1 block
    //
    SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1;
    SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 1;

    Comp1Regs.COMPCTL.bit.COMPDACEN = 1;     // Power up Comparator 1 locally
    Comp2Regs.COMPCTL.bit.COMPDACEN = 1;
    Comp3Regs.COMPCTL.bit.COMPDACEN = 1;
    //
    // Connect the inverting input to pin COMP1B
    //
    Comp1Regs.COMPCTL.bit.COMPSOURCE = 1;
    Comp2Regs.COMPCTL.bit.COMPSOURCE = 1;
    Comp3Regs.COMPCTL.bit.COMPSOURCE = 1;
    //
    // Uncomment following 2 lines to use DAC instead of pin COMP1B
    //
    // Connect the inverting input to the internal DAC
    //
    //Comp1Regs.COMPCTL.bit.COMPSOURCE = 0;
    //Comp1Regs.DACVAL.bit.DACVAL = 512;        // Set DAC output to midpoint
    //

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    InitEPwm1Example();
    InitEPwm2Example();
    InitEPwm3Example();
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    //
    // Step 5. User specific code, enable interrupts
    // Initialize counters:
    //
    EPwm1TZIntCount = 0;
    EPwm2TZIntCount = 0;
    EPwm3TZIntCount = 0;
    //
    // Enable CPU INT3 which is connected to EPWM1-3 INT:
    //
    IER |= M_INT2;

    //
    // Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
    //
    PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx3 = 1;
    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional):
    //
    for(;;)
    {
        __asm("          NOP");
    }
}

//
// epwm1_tzint_isr - EPwm1 TZ Interrupt
//
__interrupt void
epwm1_tzint_isr(void)
{
    EPwm1TZIntCount++;

    //
    // Leave these flags set so we only take this
    // interrupt once
    //
     EALLOW;
     EPwm3Regs.TZCLR.bit.OST = 1;
     EPwm3Regs.TZCLR.bit.INT = 1;
//     EPwm1Regs.TZCLR.bit.OST = 1;
//     EPwm1Regs.TZCLR.bit.INT = 1;
     EDIS;

    //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

__interrupt void
epwm2_tzint_isr(void)
{
    EPwm2TZIntCount++;
    EALLOW;
//     EPwm2Regs.TZCLR.bit.OST = 1;
//     EPwm2Regs.TZCLR.bit.INT = 1;
    EPwm1Regs.TZCLR.bit.OST = 1;
    EPwm1Regs.TZCLR.bit.INT = 1;
     EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}


__interrupt void
epwm3_tzint_isr(void)
{
    EPwm3TZIntCount++;
    EALLOW;
//     EPwm3Regs.TZCLR.bit.OST = 1;
//     EPwm3Regs.TZCLR.bit.INT = 1;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm2Regs.TZCLR.bit.INT = 1;
     EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

void InitEPwm1Example()
{
    EALLOW;


    //
    // DCAH = Comparator 1 output
    //
    EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;

    EPwm1Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ1;         // DCAL = TZ2

    //
    // DCAEVT1 =  DCAH low (will become active as Comparator output goes low)
    //
    EPwm1Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_HI;

    //
    // DCAEVT1 = DCAEVT1 (not filtered)
    //
    EPwm1Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;

    EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;   // Take async path


    //
    EPwm1Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm1Regs.TZSEL.bit.DCBEVT1 = 0;
    EPwm1Regs.TZSEL.bit.DCBEVT2 = 0;

    //
    // What do we want the DCAEVT1 and DCBEVT1 events to do?
    // DCAEVTx events can force EPWMxA
    // DCBEVTx events can force EPWMxB
    //
    EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_HI;           // EPWM1A will go high
    EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;           // EPWM1B will go low

    //
    // Enable TZ interrupt
    //
    EPwm1Regs.TZEINT.bit.OST = 1;
    EDIS;
}

void InitEPwm2Example()
{
    EALLOW;
    // DCAH = Comparator 2 输出
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT;
    // DCAL = TZ2（对应 COMP2 触发 TZ2）
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;
    // DCAEVT1 = DCAH 低电平触发
    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;
    // 事件源选择
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    // 使能 DCAEVT1 作为 EPWM2 单触发源
    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm2Regs.TZSEL.bit.DCBEVT1 = 0;
    EPwm2Regs.TZSEL.bit.DCBEVT2 = 0;

    // TZ 动作：EPWM2A 置高，EPWM2B 置低
    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    // 使能 TZ 中断
    EPwm2Regs.TZEINT.bit.OST = 1;
    EDIS;
}

void InitEPwm3Example()
{
    EALLOW;
    // DCAH = Comparator 3 输出
    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP3OUT;
    // DCAL = TZ3（对应 COMP3 触发 TZ3）
    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ3;
    // DCAEVT1 = DCAH 低电平触发
    EPwm3Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;
    // 事件源选择
    EPwm3Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    // 使能 DCAEVT1 作为 EPWM3 单触发源
    EPwm3Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm3Regs.TZSEL.bit.DCBEVT1 = 0;
    EPwm3Regs.TZSEL.bit.DCBEVT2 = 0;

    // TZ 动作：EPWM3A 置高，EPWM3B 置低
    EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
    EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    // 使能 TZ 中断
    EPwm3Regs.TZEINT.bit.OST = 1;
    EDIS;
}

//
// End of File
//


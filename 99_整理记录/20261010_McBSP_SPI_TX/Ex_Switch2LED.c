#include "AVP33x_Project.h"

static void InitMcbspaSpiTx(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2; // MDXA: MOSI
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2; // MCLKXA: SCLK
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2; // MFSXA: active-low CS
    EDIS;

    McbspaRegs.SPCR2.all = 0;
    McbspaRegs.SPCR1.all = 0;           // RX held in reset, no loopback
    McbspaRegs.MFFINT.all = 0;          // No interrupts
    McbspaRegs.PCR.all = 0x0F08;
    McbspaRegs.SPCR1.bit.CLKSTP = 2;    // SPI mode 0: CPOL=0, CPHA=0
    McbspaRegs.XCR2.all = 0;
    McbspaRegs.XCR2.bit.XDATDLY = 1;
    McbspaRegs.XCR1.all = 0;
    McbspaRegs.XCR1.bit.XWDLEN1 = 2;    // 16 bits, MSB first
    McbspaRegs.SRGR2.all = 0x2000;      // LSPCLK, frame starts on DXR write
    McbspaRegs.SRGR1.all = 36;          // 150MHz / 4 / 37 = 1.0135MHz

    DELAY_US(10);
    McbspaRegs.SPCR2.bit.GRST = 1;
    DELAY_US(10);                       // More than two generated clock cycles
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR2.bit.FRST = 1;
}

void main(void)
{
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();
    InitGpio();

    DINT;
    InitPieCtrl();
    IER = 0;
    IFR = 0;
    InitPieVectTable();

    InitMcbspaSpiTx();
    for (;;)
    {
        while (McbspaRegs.SPCR2.bit.XRDY == 0) {}
        McbspaRegs.DXR1.all = 0xA5A5;
    }
}

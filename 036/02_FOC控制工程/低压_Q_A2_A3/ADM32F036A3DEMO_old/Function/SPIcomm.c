#include "SPIcomm.h"
#include "Main.h"
#include "MotorInclude.h"

Uint16 SpiSendData[11];

void InitSpi(void)
{
   // Initialize SPI-A
    EALLOW;

    SpiaRegs.SPICCR.bit.SPISWRESET=0;

    SpiaRegs.SPICCR.all = 0x000F;
    SpiaRegs.SPICTL.all = 0x000E;
    SpiaRegs.SPISTS.all = 0x0000;
    SpiaRegs.SPIBRR = 0x0009;

    SpiaRegs.SPIFFTX.all = 0xC002;
    SpiaRegs.SPIFFCT.all = 0x00;
    SpiaRegs.SPIPRI.all = 0x0010;

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;

    EDIS;
}

void InitSpiIO(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;
}

extern int gCurSampULast, gCurSampVLast, gCurSampWLast;
extern unsigned int g_InitPos4;
int SPIDataTest[5];
void SPISendData(void)
{
    // SPIDataTest[1] -- 0.244HZ
    // SPIDataTest[2] -- 2.93HZ
    SPIDataTest[0]++;
    if (SPIDataTest[0] == 12) {
        SPIDataTest[0] = 0;
        SPIDataTest[1]++;
        if (SPIDataTest[1] == 4096) {
            SPIDataTest[1] = 0;
        }
    }
    SPIDataTest[2]++;
    if (SPIDataTest[2] == 2048) {
        SPIDataTest[2] = 0;
    }
    SPIDataTest[3] = ((long)gPhase.OutPhase + 32767) >> 4;//(gCurSamp.U + 4096) >> 1;
    SPIDataTest[4] = pm_est_angel / 202;//>> 8;//gIMT.T;//(gCurSamp.ULast + 4096) >> 1;//gIMT.T;//gPWM.U >> 1;

    SpiSendData[0] = SPIDataTest[2];//(gCurSamp.W + 4096) >> 1;//ADC_TEMP >> 4;//(gCurSamp.U + 4096) >> 1;//gUDC.uDC;
    SpiSendData[1] = SPIDataTest[1];//(gCurSamp.V + 4096) >> 1;//SPIDataTest[3];//gIUVW.U;
    SpiSendData[2] = gPWM.U >> 2;//ADC_IU >> 4;//gIMT.T;//ADC_IU >> 4;//SPIDataTest[4];//gIUVW.V;
    SpiSendData[3] = 0;//g_InitPos4 >> 4;//SPIDataTest[2];//gIUVW.W;
    SpiSendData[0] = (SpiSendData[0]+1)&0xFFF;
    SpiSendData[1] = (SpiSendData[1]+2)&0xFFF;
    SpiSendData[2] = (SpiSendData[2]+4)&0xFFF;
    SpiSendData[3] = (SpiSendData[3]+8)&0xFFF;

    while(SpiaRegs.SPIFFTX.bit.TXFFST >= 4);
    SpiaRegs.SPITXBUF = SpiSendData[0];

    while(SpiaRegs.SPIFFTX.bit.TXFFST >= 4);
    SpiaRegs.SPITXBUF = SpiSendData[1]+0x1000;

    while(SpiaRegs.SPIFFTX.bit.TXFFST >= 4);
    SpiaRegs.SPITXBUF = SpiSendData[2]+0x2000;

    while(SpiaRegs.SPIFFTX.bit.TXFFST >= 4);
    SpiaRegs.SPITXBUF = SpiSendData[3]+0x3000;
}



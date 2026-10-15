/******************************************************************************
ÎÄ¼şÃû                     : AVP33x_SysCtrl.c
×÷Õß                         : ºşÄÏ½øĞ¾µç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾
¹¦ÄÜÃèÊö                 : ÏµÍ³³õÊ¼»¯
´´½¨Ê±¼ä                 : 06-25-2019
°æ±¾                         : V1.0
ĞŞ¶©Ê±¼ä                 :
ĞŞ¶©ÄÚÈİ                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // Headerfile Include File
#include "AVP33x_Examples.h"   // Examples Include File

// ½«flash³õÊ¼»¯´úÂë°áÖÁRAMÔËĞĞ
#pragma CODE_SECTION(InitFlash, "ramfuncs");
// ½«PLL³õÊ¼»¯´úÂë°áÖÁRAMÔËĞĞ
#pragma CODE_SECTION(InitPll, "ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// ÏµÍ³³õÊ¼»¯º¯Êı¡£
// - ½ûÖ¹¿´ÃÅ¹·
// - ÉèÖÃPLL
// - ÉèÖÃ¸ßËÙÍâÉèÊ¼ÖÕºÍµÍËÙÍâÉèÊ¼ÖÕ
// - Ê¹ÄÜÍâÉèÊ±ÖÓ
void InitSysCtrl(void)
{
    // ½ûÖ¹¿´ÃÅ¹·
    DisableDog();

    // ³õÊ¼»¯PLL
    InitPll(DSP28_PLLCR,DSP28_DIVSEL);  // Íâ²¿Ê±ÖÓ30MHz£¬30MHz*10/2 = 150MHz

    // ³õÊ¼»¯ÍâÉèÊ±ÖÓ
    InitPeripheralClocks();
}

//---------------------------------------------------------------------------
// InitFlash:
//---------------------------------------------------------------------------
// flash³õÊ¼»¯º¯Êı£º½«flashµÄ¼Ä´æÆ÷ÉèÖÃÎªÒ»¸ö¹Ì¶¨µÄ×´Ì¬¡£
// ×¢Òâ£¡±¾º¯Êı±ØĞëÔÚRAMÖĞÔËĞĞ£¬·ñÔò»á³öÏÖÒâÁÏ²»µ½µÄ½á¹û¡£
void InitFlash(void)
{
    EALLOW;

    // Ê¹ÄÜflashµÄ¹ÜµÀÄ£Ê½£¬¿É¸ÄÉÆ´úÂëÔÚflashÖĞÔËĞĞµÄĞ§¹û¡£
    FlashRegs.FOPT.bit.ENPIPE = 1;

    // ÉèÖÃflashÔËĞĞµÄ×îĞ¡µÈ´ı×´Ì¬£¬Çë²Î¿¼×îĞÂµÄÊı¾İÊÖ²á¡£
#if CPU_FRQ_150MHZ

    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;   // ÉèÖÃÒ³µÈ´ı
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;   // ÉèÖÃËæ»ú·ÃÎÊµÈ´ı
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 8;     // ÉèÖÃOTPµÈ´ı

#endif

#if CPU_FRQ_120MHZ

    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;   // ÉèÖÃÒ³µÈ´ı
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;   // ÉèÖÃËæ»ú·ÃÎÊµÈ´ı
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;     // ÉèÖÃOTPµÈ´ı

#endif

    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;     // Ä¬ÈÏÉèÖÃ£¬ÇëÎğ¸ü¸Ä
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   // Ä¬ÈÏÉèÖÃ£¬ÇëÎğ¸ü¸Ä

    EDIS;

    asm(" RPT #7 || NOP");                  // µÈ´ı£¬È·±£ÉÏÃæÅäÖÃ²ÎÊıĞ´³É¹¦
}

//---------------------------------------------------------------------------
// ServiceDog:
//---------------------------------------------------------------------------
// ¿´ÃÅ¹·Î¹¹·º¯Êı¡£
void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// DisableDog:
//---------------------------------------------------------------------------
// ½ûÖ¹¿´ÃÅ¹·º¯Êı¡£
void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// InitPll:
//---------------------------------------------------------------------------
// ³õÊ¼»¯PLLº¯Êı
void InitPll(Uint16 val, Uint16 divsel)
{

    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        asm("        ESTOP0");
    }

    // ÖØĞÂÉèÖÃPLLCR¼Ä´æÆ÷Ö®Ç°ÒªÈ·±£DIVSELÎ»Îª0
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    // ÅäÖÃPLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        DisableDog();

        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // Èç¹û¿´ÃÅ¹·±»Ê¹ÄÜ£¬ÔòÒª´ò¿ªÏÂÃæÕâÌõÎ¹¹·Óï¾ä
            // ServiceDog();
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // ÉèÖÃ·ÖÆµÏµÊıÎª1»ò2
    if((divsel == 1)||(divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // ÉèÖÃ·ÖÆµÏµÊıÎª3¡£ĞèÒªÏÈÉèÖÃÎª2·ÖÆµ£¬È»ºóµÈ´ıµçÑ¹½¨Á¢£¬×îºó²ÅÄÜÉèÖÃÎª3·ÖÆµ¡£
    if(divsel == 3)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
        DELAY_US(50L);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
}

//--------------------------------------------------------------------------
// InitPeripheralClocks:
//---------------------------------------------------------------------------
// ³õÊ¼»¯ÍâÉèÊ±ÖÓº¯Êı¡£
// Ê×ÏÈ£¬ÉèÖÃ¸ßËÙºÍµÍËÙÍâÉèÊ±ÖÓµÄÔ¤·ÖÆµÏµÊı£»
// È»ºó£¬Ê¹ÄÜÒªÓÃµ½µÄÍâÉèÊ±ÖÓ¡££¨ÎªÁË½µµÍ¹¦ºÄ£¬Ã»ÓÃµ½µÄÍâÉè½¨Òé½«ÆäÍâÉèÊ±ÖÓ¹Øµô£©
// ĞèÒª×¢ÒâµÄÊÇ£ºÈç¹ûÄÄ¸öÍâÉèÊ±ÖÓÎ´Ê¹ÄÜ£¬ÔòÏàÓ¦µÄÍâÉè¼Ä´æÆ÷ÊÇÎŞ·¨¶ÁĞ´µÄ¡£
void InitPeripheralClocks(void)
{
    EALLOW;

    // ¸ß/µÍËÙÍâÉèÊ±ÖÓÔ¤·ÖÆµÏµÊıÉèÖÃ£¬Ò»°ã½¨ÒéÉèÎªÈçÏÂÄ¬ÈÏÖµ
    SysCtrlRegs.HISPCP.all = 0x0001;
    SysCtrlRegs.LOSPCP.all = 0x0002;

    // ÉèÖÃXINTF×ÜÏßÊ±ÖÓ
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;         // ×ÜÏßÊ±ÖÓ£¬XTIMCLK = SYSCLKOUT/2
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;         // Ê±ÖÓÊä³ö£¬XCLKOUT = XTIMCLK/2
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;          // ½ûÖ¹×ÜÏßÊ±ÖÓÊä³ö


    // This function is not written to be an example of efficient code.

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;       // ADC

	// ADCĞ£×¼(ÓÉÓÚ²úÆ·Åú´ÎÎÊÌâ£¬´Ë´¦ĞèÓÃÒÔÏÂÁ½Ìõ´úÂëÌæ´úÔ­À´µÄADC_cal()º¯Êı)
	AdcRegs.ADCREFSEL.all = *(Uint16 *)0x380083;  // load gain
	AdcRegs.ADCOFFTRIM.all = *(Uint16 *)0x380085; // load offset


    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;      // I2C
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;      // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;      // SCI-B
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;      // SCI-C
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;      // SPI-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1;    // McBSP-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 1;    // McBSP-B
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;       // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=1;       // eCAN-B

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;     // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;     // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;     // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;     // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;     // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;     // ePWM6
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // Enable TBCLK within the ePWM

    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1;     // eCAP3
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1;     // eCAP4
    SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 1;     // eCAP5
    SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 1;     // eCAP6
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;     // eCAP1
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;     // eCAP2
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;     // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;     // eQEP2

    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2

    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;       // DMA Clock
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;     // XTIMCLK
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock

    EDIS;
}

//---------------------------------------------------------------------------
// CsmUnlock:
//---------------------------------------------------------------------------
// ÃÜÂë½âËøº¯Êı¡£ÓÃ»§±ØĞë½«ËùÉèÖÃµÄÃÜÂëÌæ»»ÏÂÁĞµÄ0xFFFF¡£ ½âËø³É¹¦·µ»Ø1.
#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // ÓÃ»§½«×ÔºÉèÖÃµÄÃÜÂëÌîÈëÏÂÁĞ¼Ä´æÆ÷ÖĞ
    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;

    // ¶Ô¼üÈëÄÜÂë½øĞĞĞéÄâ¶ÁÈ¡£¬Èç¹ûÆ¥ÅäÔò½âËø³É¹¦
    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    if (CsmRegs.CSMSCR.bit.SECURE == 0)
        return STATUS_SUCCESS;
    else
        return STATUS_FAIL;
}

//===========================================================================
// End of file.
//===========================================================================

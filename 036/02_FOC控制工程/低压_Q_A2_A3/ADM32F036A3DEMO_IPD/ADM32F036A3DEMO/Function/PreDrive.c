
#include "PreDriver.h"
#include "Main.h"
#include "MotorInclude.h"
#include "ADP32F03x_I2c_defines.h"

struct PER_DVR_REGS PreDriverRegs;

void InitI2C(void)
{
    // Initialize I2C-A:
    EALLOW;

    I2caRegs.I2CMDR.bit.IRS = 0;
    I2caRegs.I2CSAR = PRE_DRIVER_ADDR;

    I2caRegs.I2CPSC.all = 9;
    I2caRegs.I2CCLKL = 5;
    I2caRegs.I2CCLKH = 5;

    I2caRegs.I2CIER.all = 0x00;
    I2caRegs.I2CMDR.bit.IRS = 1;

    I2caRegs.I2CFFTX.all = 0x6040;
    I2caRegs.I2CFFRX.all = 0x2040;

    I2caRegs.I2CCNT =  0x0000;
    I2caRegs.I2CSTR.all = 0xFFFF;

    EDIS;
}

void InitI2CIO(void)
{
    EALLOW;
#ifdef  F036_DEVICE_I2C
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;

    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;

    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;   // SDAA
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;   // SCLA
#else
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;

    GpioDataRegs.GPASET.bit.GPIO28 = 1;
    GpioDataRegs.GPASET.bit.GPIO29 = 1;

    GpioCtrlRegs.GPADIR.bit.GPIO28  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO29  = 1;

#endif
    EDIS;
}

#ifdef  F036_DEVICE_I2C
Uint16 PreDriverWriteData(Uint16 Addr, Uint16 Data)
{
    Uint16 u16Cnt = 0;

    while ((1 == I2caRegs.I2CMDR.bit.STP) || (1 == I2caRegs.I2CSTR.bit.BB))
    {
        if (++u16Cnt > I2CDELAY)
        {
            return I2C_STP_NOT_READY_ERROR;
        }
    }

    //I2caRegs.I2CSTR.bit.SCD = 1;

    I2caRegs.I2CSAR = PRE_DRIVER_ADDR;

    I2caRegs.I2CDXR = Addr & 0x7F;
    I2caRegs.I2CDXR = Data & 0xFF;

    I2caRegs.I2CCNT = 2;
    I2caRegs.I2CMDR.all = 0x6E20;

    u16Cnt = 0;
    while (I2caRegs.I2CSTR.bit.SCD == 0)
    {
        if (++u16Cnt > I2CDELAY)
        {
             return I2C_ERROR;
        }
    }
    for(u16Cnt=0; u16Cnt<200; u16Cnt++);

    return I2C_SUCCESS;
}

Uint16 PreDriverReadData(Uint16 Addr, Uint16* Data)
{
    Uint16 u16Cnt = 0;

    while ((1 == I2caRegs.I2CMDR.bit.STP) || (1 == I2caRegs.I2CSTR.bit.BB))
    {
        if (++u16Cnt > I2CDELAY)
        {
            return I2C_STP_NOT_READY_ERROR;
        }
    }

    I2caRegs.I2CSTR.bit.ARDY = 1;

    I2caRegs.I2CSAR = PRE_DRIVER_ADDR;
    I2caRegs.I2CCNT = 1;

    I2caRegs.I2CDXR = (Addr | 0x80)&0xFF;
    I2caRegs.I2CMDR.all = 0x6620;

    u16Cnt = 0;
    while (I2caRegs.I2CSTR.bit.ARDY == 0)
    {
        if (++u16Cnt > I2CDELAY)
        {
            //return I2C_ERROR;
            break;
        }
    }

    I2caRegs.I2CSTR.bit.SCD = 1;
    I2caRegs.I2CCNT = 1;
    I2caRegs.I2CMDR.all = 0x6C20;

    u16Cnt = 0;
    while (I2caRegs.I2CSTR.bit.SCD == 0)
    {
        if (++u16Cnt > I2CDELAY)
        {
            //return I2C_ERROR;
            break;
        }
    }
    *Data = I2caRegs.I2CDRR;

    return I2C_SUCCESS;
}
#else
//extern void ADP32F03x_usDelay(Uint32 Count);
void PreDriverWriteData(Uint16 Addr, Uint16 Data)
{
    Uint16 i;
    Uint16 SendData;

    //start
    SDA_OUT;
    SDA_H;
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SDA_L;
    ADP32F03x_usDelay(START_DELAY);
    SCL_L;

    SendData = PRE_DRIVER_ADDR;
    for(i=0; i<8; i++)
    {
        if(SendData & 0x80) SDA_H;
        else SDA_L;
        SendData = SendData<<1;

        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    SDA_OUT;
    SendData = Addr & 0x7F;
    for(i=0; i<8; i++)
    {
        if(SendData & 0x80) SDA_H;
        else SDA_L;
        SendData = SendData<<1;

        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    SDA_OUT;
    SendData = Data;
    for(i=0; i<8; i++)
    {
        if(SendData & 0x80) SDA_H;
        else SDA_L;
        SendData = SendData<<1;

        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);

    //STOP
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(STOP_DELAY);
    SDA_H;
}

Uint16 PreDriverReadData(Uint16 Addr)
{
    Uint16 i;
    Uint16 SendData;
    Uint16 ReadData;

    //start
    SDA_OUT;
    SDA_H;
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SDA_L;
    ADP32F03x_usDelay(START_DELAY);
    SCL_L;

    SendData = PRE_DRIVER_ADDR;
    for(i=0; i<8; i++)
    {
        if(SendData & 0x80) SDA_H;
        else SDA_L;
        SendData = SendData<<1;

        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    SDA_OUT;
    SendData = Addr | 0x80;
    for(i=0; i<8; i++)
    {
        if(SendData & 0x80) SDA_H;
        else SDA_L;
        SendData = SendData<<1;

        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    ReadData = 0;
    for(i=0; i<8; i++)
    {
        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ReadData = ReadData<<1;
        if(SDA_DATA) ReadData = ReadData | 0x01;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }

    //ACK
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    //STOP
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(STOP_DELAY);
    SDA_H;

    return(ReadData);
}
#endif

void PreDriverInit(void)
{
    //PreDriverWriteData(STATE_REG_ADDR,0x00);                  //0x00只读寄存器
    //PreDriverWriteData(VDS_GDF_STATE_ADDR,0x00);              //0x01只读寄存器
    PreDriverWriteData(CONTROL_REG_ADDR,DEFAULT_DROCES_CONFIG);
    PreDriverWriteData(TDEAD_WD_REG_ADDR,TDEAD_WD_CONFIG);
    PreDriverWriteData(VDS12_CONTROL_ADDR,VDS12_CONFIG);
    PreDriverWriteData(CONFIG_REG_ADDR,IBUSOCP_CONFIG);
                                                                // 0011 1110
                                                                // 母线过流设置
                                                                // 0x05:I = VREF/Rs*Av = 0.4V/0.005*16 = 5A
                                                                // [5]使能母线过流保护 0b使能，1b禁止
                                                                // [4-3]VREF:00b 3V,01b 2V,10b 1.2V,11b 0.4 V
                                                                // [2]:分流放大器使能 1使能，0禁止
                                                                // [1-0]Av:放大器增益 00b:2,01b:4,10b:8,11b:16
                                                                // KP = 8, 0.4V = 0.0025*I*16, I = 10A
                                                                ////////////////////////////////////////////////
    PreDriverWriteData(VDS3_GDF3_REG_ADDR,VDS3_GDF3_CONFIG);    //[3-0]只读
    PreDriverWriteData(VDS3_CONTROL_ADDR,VDS3_SPSN_CONFIG);
}

void GetPerDriverState(void)
{
//GpioDataRegs.GPADAT.bit.GPIO16  = 1;
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;                                        //GPIOmode
    EDIS;
    PreDriverRegs.ErrFlag.all = PreDriverReadData(STATE_REG_ADDR);              //故障反馈Ox00
    PreDriverRegs.VDSGDF12Status.all = PreDriverReadData(VDS_GDF_STATE_ADDR);   //VDS12Status Ox01
    PreDriverRegs.VDSGDF3Status.all = PreDriverReadData(VDS3_GDF3_REG_ADDR);    //VDS3Status Ox07
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;//TZmode
    EDIS;
//GpioDataRegs.GPACLEAR.bit.GPIO16  = 1;
}

void DriverClaerFault(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;        //GPIOmode
    EDIS;
    PreDriverWriteData(CONTROL_REG_ADDR,0x19);  //故障清除，历史故障需要手动清除
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;        //TZmode
    EDIS;
}

///////////////////////////////////////test/////////////////////
Uint16 DataTest[8];
void PreDriverTest(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;//GPIOmode
    EDIS;

    DataTest[0] = PreDriverReadData(STATE_REG_ADDR);
    DataTest[1] = PreDriverReadData(VDS_GDF_STATE_ADDR);

    DataTest[2] = PreDriverReadData(CONTROL_REG_ADDR);
    DataTest[3] = PreDriverReadData(TDEAD_WD_REG_ADDR);
    DataTest[4] = PreDriverReadData(VDS12_CONTROL_ADDR);
    DataTest[5] = PreDriverReadData(CONFIG_REG_ADDR);

    DataTest[6] = PreDriverReadData(VDS3_GDF3_REG_ADDR);
   // PreDriverWriteData(VDS3_CONTROL_ADDR,0x21);
    DataTest[7] = PreDriverReadData(VDS3_CONTROL_ADDR);

    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;//TZmode
    EDIS;
}

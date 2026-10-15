
#include "PreDriver.h"
#include "Main.h"
#include "MotorInclude.h"
#include "ADP32F03x_I2c_defines.h"

/*=============================================================================
 *   本文件内部私有函数声明（static）
 *============================================================================*/
// static struct PER_DVR_REGS PreDriverRegs;
struct PER_DVR_REGS PreDriverRegs;  // 变量没有使用（USED），避免警告

/***************************************************************************
  Private Functions
 ***************************************************************************/
static void PreDriverWriteData(Uint16 Addr, Uint16 Data);
static Uint16 PreDriverReadData(Uint16 Addr);
static Uint16 WaitForSclSdaHigh(void);
static Uint16 PreDriverWriteVerify(Uint16 Addr, Uint16 Data, Uint16 Mask);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_SendByte(Uint16 Data);
static void I2C_ClockAck(void);
static Uint16 I2C_RecvByte(void);
static void I2C_SendAck(void);


void InitI2CIO(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
    GpioDataRegs.GPASET.bit.GPIO28 = 1;
    GpioDataRegs.GPASET.bit.GPIO29 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO28  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO29  = 1;
    EDIS;
}

/***************************************************************************
 Description: Generate I2C start condition on SDA/SCL.
 ***************************************************************************/
static void I2C_Start(void)
{
    SDA_OUT;
    SDA_H;
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SDA_L;
    ADP32F03x_usDelay(START_DELAY);
    SCL_L;
}

static void I2C_Stop(void)
{
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(STOP_DELAY);
    SDA_H;
}

static void I2C_SendByte(Uint16 Data)
{
    Uint16 i;
    SDA_OUT;
    for (i = 0; i < 8; i++)
    {
        if (Data & 0x80) SDA_H;
        else SDA_L;
        Data <<= 1;
        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }
}

static void I2C_ClockAck(void)
{
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;
}

static Uint16 I2C_RecvByte(void)
{
    Uint16 i;
    Uint16 ReadData;
    ReadData = 0;
    for (i = 0; i < 8; i++)
    {
        ADP32F03x_usDelay(L_LEVEL_DELAY);
        SCL_H;
        ReadData = ReadData << 1;
        if (SDA_DATA) ReadData = ReadData | 0x01;
        ADP32F03x_usDelay(H_LEVEL_DELAY);
        SCL_L;
    }
    return ReadData;
}

static void I2C_SendAck(void)
{
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;
}

static void PreDriverWriteData(Uint16 Addr, Uint16 Data)
{
    I2C_Start();
    I2C_SendByte(PRE_DRIVER_ADDR);
    I2C_ClockAck();
    I2C_SendByte(Addr & 0x7F);
    I2C_ClockAck();
    I2C_SendByte(Data);
    I2C_ClockAck();
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    I2C_Stop();
}

static Uint16 PreDriverReadData(Uint16 Addr)
{
    Uint16 ReadData;

    I2C_Start();
    I2C_SendByte(PRE_DRIVER_ADDR);
    I2C_ClockAck();
    I2C_SendByte(Addr | 0x80);
    I2C_ClockAck();
    ReadData = I2C_RecvByte();
    I2C_SendAck();
    I2C_Stop();

    return ReadData;
}

static Uint16 WaitForSclSdaHigh(void)
{
    Uint32 stableCnt;
    Uint32 timeout = 50000;

    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;
    EDIS;

    while (timeout > 0)
    {
        while ((GpioDataRegs.GPADAT.bit.GPIO28 != 1 || GpioDataRegs.GPADAT.bit.GPIO29 != 1) && timeout > 0)
        {
            timeout--;
            ADP32F03x_usDelay(1);
        }

        if (timeout == 0)
        {
            return 0;
        }

        stableCnt = 0;
        while (stableCnt < 5000 && timeout > 0)
        {
            if (GpioDataRegs.GPADAT.bit.GPIO28 != 1 || GpioDataRegs.GPADAT.bit.GPIO29 != 1)
            {
                break;
            }
            ADP32F03x_usDelay(1);
            stableCnt++;
            timeout--;
        }

        if (stableCnt >= 5000)
        {
            return 1;
        }
    }

    return 0;
}

static Uint16 PreDriverWriteVerify(Uint16 Addr, Uint16 Data, Uint16 Mask)
{
    Uint16 retryCnt = 0;
    Uint16 readData;

    for (retryCnt = 0; retryCnt < 3; retryCnt++)
    {
        PreDriverWriteData(Addr, Data);
        readData = PreDriverReadData(Addr);

        // VDS3_GDF3_REG_ADDR (0x06) 的 [3:0] 位是只读状态位。直接比较 readData == Data
        // 会因只读位值不同而 永远失败 ，误报 ERROR_DRIVER。
        if ((readData & Mask) == (Data & Mask))
        {
            return 1;
        }
        ADP32F03x_usDelay(100);
    }

    return 0;
}

void PreDriverInit(void)
{
    if (!WaitForSclSdaHigh())
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;
    EDIS;

    if (!PreDriverWriteVerify(CONTROL_REG_ADDR, DEFAULT_DROCES_CONFIG, 0xFF))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    if (!PreDriverWriteVerify(TDEAD_WD_REG_ADDR, TDEAD_WD_CONFIG, 0xFF))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    if (!PreDriverWriteVerify(VDS12_CONTROL_ADDR, VDS12_CONFIG, 0xFF))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    if (!PreDriverWriteVerify(CONFIG_REG_ADDR, IBUSOCP_CONFIG, 0xFF))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    if (!PreDriverWriteVerify(VDS3_GDF3_REG_ADDR, VDS3_GDF3_CONFIG, 0xF0))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    if (!PreDriverWriteVerify(VDS3_CONTROL_ADDR, VDS3_SPSN_CONFIG, 0xFF))
    {
        gMainStatus.ErrorCode = ERROR_DRIVER;
        return;
    }

    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;                                        //GPIOmode
    EDIS;
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

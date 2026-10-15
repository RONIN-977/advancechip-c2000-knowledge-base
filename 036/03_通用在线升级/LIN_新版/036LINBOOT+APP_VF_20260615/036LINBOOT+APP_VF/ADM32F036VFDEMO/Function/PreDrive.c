#include "ADP32F03x_I2c_defines.h"
#include "PreDriver.h"
#include "MotorInclude.h"

struct PER_DVR_REGS PreDriverRegs;

/******************************************************************************
Function Name : PreDriverWriteData
Function description : Write data to PreDriver via I2C communication
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : Addr - Address to write, Data - Data to write
Returns : N/A
******************************************************************************/
void PreDriverWriteData(Uint16 Addr, Uint16 Data)
{
    Uint16 i;
    Uint16 SendData;

    // Start condition
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

    // Acknowledge
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

    // Acknowledge
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

    // Acknowledge
    SDA_IN;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);

    // Stop condition
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(STOP_DELAY);
    SDA_H;
}

/******************************************************************************
Function Name : PreDriverReadData
Function description : Read data from PreDriver via I2C communication
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : Addr - Address to read from
Returns : Read data
******************************************************************************/
Uint16 PreDriverReadData(Uint16 Addr)
{
    Uint16 i;
    Uint16 SendData;
    Uint16 ReadData;

    // Start condition
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

    // Acknowledge
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

    // Acknowledge
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

    // Acknowledge
    SDA_OUT;
    SDA_L;
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(H_LEVEL_DELAY);
    SCL_L;

    // Stop condition
    ADP32F03x_usDelay(L_LEVEL_DELAY);
    SCL_H;
    ADP32F03x_usDelay(STOP_DELAY);
    SDA_H;

    return(ReadData);
}

/******************************************************************************
Function Name : PreDriverInit
Function description : Initialize PreDriver with default configuration
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void PreDriverInit(void)
{
    PreDriverWriteData(CONTROL_REG_ADDR,DEFAULT_DROCES_CONFIG);
    PreDriverWriteData(TDEAD_WD_REG_ADDR,TDEAD_WD_CONFIG);
    PreDriverWriteData(VDS12_CONTROL_ADDR,VDS12_CONFIG);
    PreDriverWriteData(CONFIG_REG_ADDR,IBUSOCP_CONFIG);
    PreDriverWriteData(VDS3_GDF3_REG_ADDR,VDS3_GDF3_CONFIG);
    PreDriverWriteData(VDS3_CONTROL_ADDR,VDS3_SPSN_CONFIG);
}

/******************************************************************************
Function Name : GetPerDriverState
Function description : Read PreDriver state registers
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void GetPerDriverState(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;                                        // GPIO mode
    EDIS;
    PreDriverRegs.ErrFlag.all = PreDriverReadData(STATE_REG_ADDR);              // Read register 0x00
    PreDriverRegs.VDSGDF12Status.all = PreDriverReadData(VDS_GDF_STATE_ADDR);   // VDS12 Status register 0x01
    PreDriverRegs.VDSGDF3Status.all = PreDriverReadData(VDS3_GDF3_REG_ADDR);    // VDS3 Status register 0x07
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3; // TZ mode
    EDIS;
}

/******************************************************************************
Function Name : DriverClaerFault
Function description : Clear fault condition in PreDriver
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void DriverClaerFault(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;        // GPIO mode
    EDIS;
    PreDriverWriteData(CONTROL_REG_ADDR,0x19);  // Write specific value to clear fault
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;        //TZmode
    EDIS;
}

///////////////////////////////////////test/////////////////////
Uint16 DataTest[8];

/******************************************************************************
Function Name : PreDriverTest
Function description : Test function to read PreDriver registers for debugging
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void PreDriverTest(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0; // GPIO mode
    EDIS;

    DataTest[0] = PreDriverReadData(STATE_REG_ADDR);
    DataTest[1] = PreDriverReadData(VDS_GDF_STATE_ADDR);
    DataTest[2] = PreDriverReadData(CONTROL_REG_ADDR);
    DataTest[3] = PreDriverReadData(TDEAD_WD_REG_ADDR);
    DataTest[4] = PreDriverReadData(VDS12_CONTROL_ADDR);
    DataTest[5] = PreDriverReadData(CONFIG_REG_ADDR);
    DataTest[6] = PreDriverReadData(VDS3_GDF3_REG_ADDR);
    DataTest[7] = PreDriverReadData(VDS3_CONTROL_ADDR);

    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3; // TZ mode
    EDIS;
}

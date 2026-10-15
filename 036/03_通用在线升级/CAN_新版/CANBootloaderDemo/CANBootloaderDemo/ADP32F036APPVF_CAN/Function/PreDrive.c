//#############################################################################
//
// FILE:   PreDrive.c
//
// TITLE:  Pre-driver communication support functions.
//
// Description:
// This file implements the software I2C access routines and status helpers
// used to configure and monitor the external pre-driver device.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "ADP32F03x_I2c_defines.h"
#include "PreDriver.h"
#include "MotorInclude.h"

/*============================================================================*/
/*                          GLOBAL DATA DEFINITIONS                           */
/*============================================================================*/
struct PER_DVR_REGS PreDriverRegs;
Uint16 DataTest[8];

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Write one register value to the pre-driver over software I2C.
//!
//! \param Addr is the target register address.
//! \param Data is the 8-bit register value to write.
//!
//! This function emits the start condition, device address, register address,
//! data byte, and stop condition using the bit-banged I2C control macros.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Read one register value from the pre-driver over software I2C.
//!
//! \param Addr is the target register address.
//!
//! This function sends the register address, reads back one byte from the
//! pre-driver, and then returns the sampled value.
//!
//! \return The register value read from the target address.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Initialize the pre-driver with the default register configuration.
//!
//! This function writes the control, dead-time, VDS, and bus overcurrent
//! configuration values required by the application startup sequence.
//!
//! \return None.
//
//*****************************************************************************
void PreDriverInit(void)
{
    PreDriverWriteData(CONTROL_REG_ADDR,DEFAULT_DROCES_CONFIG);
    PreDriverWriteData(TDEAD_WD_REG_ADDR,TDEAD_WD_CONFIG);
    PreDriverWriteData(VDS12_CONTROL_ADDR,VDS12_CONFIG);
    PreDriverWriteData(CONFIG_REG_ADDR,IBUSOCP_CONFIG);
    PreDriverWriteData(VDS3_GDF3_REG_ADDR,VDS3_GDF3_CONFIG);
    PreDriverWriteData(VDS3_CONTROL_ADDR,VDS3_SPSN_CONFIG);
}

//*****************************************************************************
//
//! Read the current pre-driver fault and status registers.
//!
//! This function temporarily switches GPIO28 into GPIO mode, reads the status
//! registers from the pre-driver, and restores the trip-zone pin function.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Clear the active pre-driver fault condition.
//!
//! This function switches GPIO28 into GPIO mode, writes the required control
//! value to clear the fault latch, and then restores the trip-zone pin mode.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Read back the main pre-driver registers for debug capture.
//!
//! This function stores a fixed set of pre-driver register reads into the test
//! buffer so they can be inspected during debugging.
//!
//! \return None.
//
//*****************************************************************************
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

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

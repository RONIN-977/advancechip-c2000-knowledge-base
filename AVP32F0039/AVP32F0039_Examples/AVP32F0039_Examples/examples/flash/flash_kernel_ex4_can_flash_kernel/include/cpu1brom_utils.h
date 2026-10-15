/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   cpu1brom_utils.h
*                         TITLE:  Common Macros Used and Helper APIs
*******************************************************************************************/

#ifndef CPU1BROM_UTILS_H
#define CPU1BROM_UTILS_H

#include "cpu1bootrom.h"
#include "gpio.h"

//
//Convert integer to asm NOP repeat command
//
#define CYCLES_TO_ASM(x)                              " RPT #" #x " || NOP"


//
//Macro to execute 'x' number of NOPs
//
#define NOP_CYCLES(x)                                 asm(CYCLES_TO_ASM(x))

//
//Macro for debug label
//
#define DEBUGLABEL(name) asm(" .global TSTLBL_" #name "\n" \
                             "TSTLBL_" #name ":")

//
// Macros to build 16b and 32b words
//
#define BUILD_WORD(LOW, HIGH) (((uint16_t)HIGH << 8U) | (uint16_t)LOW)
#define BUILD_DWORD(LOW1, LOW2, HIGH1, HIGH2) (((uint32_t)HIGH2 << 24U) | \
                                               ((uint32_t)HIGH1 << 16U) | \
                                               ((uint32_t)LOW2 << 8U) | (uint32_t)LOW1)

//
// Set ERRORSTS pin
//
static inline void triggerERRORSTSPin(void)
{
    asm("   ESTOP0");

    //
    // Trigger ERRORSTS
    //
    EALLOW;
    HWREGH(NMI_BASE + NMI_O_ERRORSTSFRC) |= NMI_ERRORSTSFRC_ERROR;
    EDIS;
}

/**
* Double Read - 16 Bit
* Reads value in given address twice. Triggers ERRORSTS pin
* and watchdog reset in case of mismatch
*
* @param address Memory Address of desired location
* @return uint16_t Result of read operation
*
* Design: \ref did_safety_register_doubleread_interface
* Requirement: REQ_TAG(C2000BROM-195)
*/
static inline uint16_t regDoubleRead_16(uint32_t address)
{
    //
    //Regular HWREGH read
    //
    volatile uint16_t valueRead = HWREGH(address);

    //
    //Read value again
    //
    volatile uint16_t valueInternal = HWREGH(address);

    //
    //Check if both read attempts yield same value
    //
    if(valueInternal != valueRead)
    {
        asm("   ESTOP0");

        //
        //Trigger ERRORSTS pin upon failure
        //
        SysCtl_resetDevice();
    }

    return valueRead;
}

/**
* Double Read - 32 Bit
* Reads value in given address twice. Triggers ERRORSTS pin
* and watchdog reset in case of mismatch
*
* NOTE: Did not make this API Static as it is being accessed in
*       cpubrom_Init_Boot.asm as well as other .c files
* @param address Memory Address of desired location
* @return uint32_t Result of read operation
*
* Design: \ref did_safety_register_doubleread_interface did_safety_double_read
*
* Requirement: REQ_TAG(C2000BROM-195), REQ_TAG(C2000BROM-276)
*/
static inline uint32_t regDoubleRead_32(uint32_t address)
{
    //
    //Regular HWREGH read
    //
    volatile uint32_t valueRead = HWREG(address);

    //
    //Read value again
    //
    volatile uint32_t valueInternal = HWREG(address);

    if(valueInternal != valueRead)
    {
        asm("   ESTOP0");

        //
        //Trigger ERRORSTS pin upon failure
        //
        SysCtl_resetDevice();
    }

    return valueRead;
}

/**
* Read after Write - 16 Bit
* Reads value of register after performing write operation on it.
* Triggers ERRORSTS pin and watchdog reset in case of mismatch.
*
* @param address Memory Address of desired location
* @param mask    Mask value to write to specific bitfield
* @param value   Value to be written into location
*
* Design: \ref did_safety_register_readback_afterwrite_interface did_safety_read_after_write
* Requirement: REQ_TAG(C2000BROM-194), REQ_TAG(C2000BROM-277)
*/
static inline void regReadAfterWrite_16(uint32_t address, uint16_t mask, uint16_t value)
{
    //
    //Write Value to Register
    //
    HWREGH(address) = (value & mask);

    //
    //Read from memory
    //
    uint16_t valueInternal = (HWREGH(address) & mask);

    if(valueInternal != (value & mask))
    {
        asm("   ESTOP0");

        //
        //Trigger ERRORSTS pin upon failure
        //
        SysCtl_resetDevice();
    }

    return;
}

/**
* Read after Write - 32 Bit
* Reads value of register after performing write operation on it.
* Triggers ERRORSTS pin and watchdog reset in case of mismatch.
*
* @param address Memory Address of desired location
* @param mask    Mask value to write to specific bitfield
* @param value Value to be written into location
*
* Design: \ref did_safety_register_readback_afterwrite_interface
* Requirement: REQ_TAG(C2000BROM-194)
*/
static inline void regReadAfterWrite_32(uint32_t address, uint32_t mask, uint32_t value)
{
    //
    //Write Value to Register
    //
    HWREG(address) = (value & mask);

    //
    //Read from memory
    //
    uint32_t valueInternal = (HWREG(address) & mask);

    if(valueInternal != (value & mask))
    {
        asm("   ESTOP0");

        //
        //Trigger ERRORSTS pin upon failure
        //
        SysCtl_resetDevice();
    }

    return;
}

#endif

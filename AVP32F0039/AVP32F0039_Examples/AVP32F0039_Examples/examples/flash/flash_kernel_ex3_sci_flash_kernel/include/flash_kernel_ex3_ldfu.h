/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   flash_kernel_ex3_ldfu.h
*                         TITLE:  Live DFU Header
*******************************************************************************************/

#ifndef __FLASH_KERNEL_EX3_LDFU_H__
#define __FLASH_KERNEL_EX3_LDFU_H__

//
// Includes
//
#include "cpu1bootrom.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"
#include "flash_kernel_ex3_erase.h"
#include "flash_kernel_ex3_verify.h"
#include "driverlib.h"
#include "flash_kernel_ex3_commands.h"
#include "assert.h"
#include "device.h"

//
// Defines
//

//
// START: Value written to the address of 'B0_START_ADD' or 'B1_START_ADD' after the erase operation is done
//        Indicates that programming/verifying is about to start.
//
#define START                     0x5A5A

//
// KEY: Value written to the address of 'B0_KEY_ADD', 'B1_KEY_ADD', and 'B2_KEY_ADD' after programming/verifying is done
//
#define KEY                       0x5B5B5B5B

//
// B0_START_ADD: The address at which the START value is written to for bank 0
//
#define B0_START_ADD              0x82000

//
// B0_KEY_ADD: The address at which the KEY value is written to for bank 0
//
#define B0_KEY_ADD                0x82004

//
// B0_REV_ADD: The address at which the REV value is written to for bank 0
//
#define B0_REV_ADD                0x82006

//
// B0_RESERVED: A program being loaded during an update cannot be written below this adddress inside of bank 0. 
//              0x80000 - 0x82007 is reserved for the flash kernel
//
#define B0_RESERVED               0x82008

//
// B1_START_ADD: The address at which the START value is written to for bank 1
//
#define B1_START_ADD              0x92000

//
// B1_KEY_ADD: The address at which the KEY value is written to for bank 1
//
#define B1_KEY_ADD                0x92004

//
// B1_REV_ADD: The address at which the REV value is written to for bank 1
//
#define B1_REV_ADD                0x92006

//
// B1_RESERVED: A program being loaded during an update cannot be written below this adddress inside of bank 1. 
//              0x90000 - 0x92007 is reserved for the flash kernel
//
#define B1_RESERVED               0x92008

//
// B2_START_ADD: The address at which the START value is written to for bank 2
//
#define B2_START_ADD              0xA2000

//
// B2_KEY_ADD: The address at which the KEY value is written to for bank 2
//
#define B2_KEY_ADD                0xA2004

//
// B2_REV_ADD: The address at which the REV value is written to for bank 2
//
#define B2_REV_ADD                0xA2006

//
// B2_RESERVED: A program being loaded during an update cannot be written below this address inside of bank 2.
//              0xA0000 - 0xA2007 is reserved for the flash kernel
//
#define B2_RESERVED               0xA2008

//
// WORDS_IN_FLASH_BUFFER: Specifies the size of the buffer used to write the 
//                        START, KEY, and REV values for bank 1 and bank 0
//
#define  WORDS_IN_FLASH_BUFFER    0x04

//
// Do not change BUFFER_SIZE; BUFFER_SIZE is the size of a buffer
// that stores the bytes of a program received through SCI
//
#define BUFFER_SIZE               0x80

//
// Function Prototypes
//
void liveDFU(void);
uint32_t ldfuLoad(void);
void ldfuCopyData(void);
void bankSelect(void);
void fmstatFail(void);
uint32_t getLongData(void);
void revInit(void);
void readReservedFn(void);
uint16_t findSize(uint32_t address);
extern uint16_t sciaGetWordData(void);
extern uint32_t sciGetFunction(uint32_t  BootMode);
extern void initFlashSectors(void);
extern void exampleError(Fapi_StatusType status);
extern void sciSendChecksum(void);

#endif // __FLASH_KERNEL_EX3_LDFU_H__

//
// End of file
//

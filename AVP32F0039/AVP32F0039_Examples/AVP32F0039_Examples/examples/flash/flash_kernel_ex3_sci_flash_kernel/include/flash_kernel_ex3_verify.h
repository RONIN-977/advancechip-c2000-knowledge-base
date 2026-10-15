/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   flash_kernel_ex3_verify.h
*                         TITLE:  Verify Data functions
*******************************************************************************************/


#ifndef __FLASH_KERNEL_EX3_VERIFY_H__
#define __FLASH_KERNEL_EX3_VERIFY_H__

//
// Included Files
//
#include "cpu1bootrom.h"
#include "flash_kernel_ex3_commands.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"


#include "assert.h"

//
// Function Prototypes
//
extern void sciSendChecksum(void);
void verifyData(void);
extern uint32_t getLongData(void);
extern void readReservedFn(void);
extern uint16_t sciaGetWordData(void);
extern void setFlashAPIError(Fapi_StatusType status);

#endif //__FLASH_KERNEL_EX3_VERIFY_H__

//
// End of file
//

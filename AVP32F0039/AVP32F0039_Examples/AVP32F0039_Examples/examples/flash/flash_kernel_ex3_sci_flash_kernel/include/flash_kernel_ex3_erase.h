/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   flash_kernel_ex3_erase.h
*                         TITLE:  Erase shared functions
*******************************************************************************************/


#ifndef __SHARED_ERASE_H__
#define __SHARED_ERASE_H__

//
// Included Files
//
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h" // Flash API example header file
#include "cpu1bootrom.h"
#include "Types.h"
#include "flash_kernel_ex3_commands.h"

//
// Function Prototypes
//
void sharedErase(uint32_t sectors, uint32_t bank);
extern void setFlashAPIError(Fapi_StatusType status);

#endif

//
// End of file
//

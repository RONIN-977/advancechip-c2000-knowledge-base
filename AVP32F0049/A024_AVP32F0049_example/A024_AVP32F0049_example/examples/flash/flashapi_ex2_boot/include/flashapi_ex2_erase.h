/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   flashapi_ex2_erase.h
*                         TITLE:  Erase Header
*******************************************************************************************/

#ifndef __FLASHAPI_EX2_ERASE_H__
#define __FLASHAPI_EX2_ERASE_H__

//
// Includes
//
#include "flashapi_ex2_bootrom.h"
#include "flashapi_ex2_commands.h"
#include "F021_F28004x_C28x.h"
#include "flashapi_ex2_flash_programming_c28.h"
#include "Types.h"

void sharedErase(uint32_t sectors);

#endif // __FLASHAPI_EX2_ERASE_H__

//
// End of file
//


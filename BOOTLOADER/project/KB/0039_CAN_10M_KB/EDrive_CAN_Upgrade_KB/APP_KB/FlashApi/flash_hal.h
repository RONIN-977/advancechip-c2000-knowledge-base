/*

 */

#ifndef FLASH_HAL_H_
#define FLASH_HAL_H_
#include "uC_hal.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"
uint8 flash_hal_init(void);
uint8 flash_hal_erase(uint32 startAddr,uint32 len);
void flash_hal_read(uint32 startAddr,uint32 Len,uint8 *Data);
//uint8 flash_hal_write(uint8 type,uint32 startAddr,uint32 Len,uint8 *Data);
uint8 flash_hal_write(uint32 startAddr,uint32 Len,uint8 *Data);
#endif 

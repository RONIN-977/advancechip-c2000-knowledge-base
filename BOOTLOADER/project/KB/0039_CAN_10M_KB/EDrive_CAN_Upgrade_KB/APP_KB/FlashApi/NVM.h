/*

 */

#ifndef NVM_H_
#define NVM_H_
#include "uC_hal.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"

typedef struct app_configuration_data_t
{
    u16 pad1[128];
} APP_Cfg_S;
extern APP_Cfg_S APPCfg;

extern void save_cfg(void);
extern void read_cfg(void);
#endif 

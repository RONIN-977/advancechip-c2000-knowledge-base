/*

 */
#include "flash_hal.h"
#include "NVM.h"

APP_Cfg_S APPCfg;

void save_cfg(void);
void read_cfg(void);

u32 App_config_addr()
{
    return 0x086000U;
}
void read_cfg(void)
{
    memcpy((uint8 *)&APPCfg,(uint8 *)App_config_addr(), sizeof(APP_Cfg_S));
}

void save_cfg(void)
{
    flash_hal_erase(0x86000U,0x1000);
    flash_hal_write(0x86000U,sizeof(APP_Cfg_S),(uint8 *)&APPCfg);
}

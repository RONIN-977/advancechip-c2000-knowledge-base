#ifndef TEST_PLATFORM_H
#define TEST_PLATFORM_H
#include <string.h>
#define UPGRADE_HOST_TEST 1
#ifndef UPGRADE_IS_BOOT
#define UPGRADE_IS_BOOT 1
#endif
#include "../firmware/common/UpgradeMd5.h"
#include "../firmware/common/UpgradeConfig.h"
#include "../firmware/common/SciUpgrade.h"
typedef struct { Uint32 address; } FLASH_ST;
#define STATUS_SUCCESS 0
#define SECTORB 2
#define SECTORC 4
#define SECTORD 8
#define SECTORE 16
#define SECTORF 32
#define SECTORG 64
#define SECTORH 128
#define DINT
#define EINT
extern Uint16 fakeFlash[0x38000];
#define UPGRADE_FLASH_PTR(address) (&fakeFlash[(address)-UPGRADE_APP_START])
Uint16 Flash_Program(Uint16 *,Uint16 *,Uint16,FLASH_ST *);
Uint16 Flash_Verify(Uint16 *,Uint16 *,Uint16,FLASH_ST *);
Uint16 Flash_Erase(Uint16,FLASH_ST *);
#endif

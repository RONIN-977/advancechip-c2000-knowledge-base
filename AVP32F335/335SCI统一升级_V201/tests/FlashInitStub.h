#include "TestPlatform.h"
#include "../firmware/AVP32F335_Bootloader/Main/AVP33x_API_Config.h"
#define EALLOW
#define EDIS
Uint16 Flash28_API_LoadStart, Flash28_API_LoadEnd, Flash28_API_RunStart;
Uint32 Flash_CPUScaleFactor;
void (*Flash_CallbackPtr)(void);
void UpgradeWatchdog(void);
/* AVP33x_SysCtrl.c CsmUnlock: 1 = unlocked, 0 = locked.
 * Flash API STATUS_SUCCESS is independently defined as 0. */
Uint16 TestCsmResult = 1;
Uint16 CsmUnlock(void) { return TestCsmResult; }
void MemCopy(Uint16 *source, Uint16 *end, Uint16 *dest) {}
Uint16 TestApiReady(void) { return RemoteModbus.apiReady; }

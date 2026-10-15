#include "FlashInitStub.h"
void InitFlashAPI(void)
{
    Uint16 Status;

    MemCopy((Uint16*)&Flash28_API_LoadStart, (Uint16*)&Flash28_API_LoadEnd, (Uint16*)&Flash28_API_RunStart);

    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    Flash_CallbackPtr = UpgradeWatchdog;
    EDIS;

    Status = CsmUnlock();
    /* CsmUnlock returns 1 on success; Flash API STATUS_SUCCESS is 0. */
    RemoteModbus.apiReady = (Status == 1U);
    if(Status != 1U)
    {
      //
    }
}
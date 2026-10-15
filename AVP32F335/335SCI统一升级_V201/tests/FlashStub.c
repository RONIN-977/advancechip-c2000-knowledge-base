#include "TestPlatform.h"
Uint16 fakeFlash[0x38000];
Uint32 eraseCount, programCount, verifyCount, commitCount, firstErase;
Uint16 failErase, failProgram, failVerify;
Uint16 Flash_Erase(Uint16 sectors, FLASH_ST *status)
{
    Uint32 sector,i; eraseCount++; if(eraseCount==1) firstErase=sectors;
    if(failErase) return 1;
    for(sector=0;sector<7;sector++) if(sectors&(128>>sector))
        for(i=sector*0x8000;i<(sector+1)*0x8000;i++) fakeFlash[i]=0xffff;
    return 0;
}
Uint16 Flash_Program(Uint16 *dest,Uint16 *data,Uint16 words,FLASH_ST *status)
{
    Uint16 i; programCount++;
    if(failProgram) return 1;
    for(i=0;i<words;i++) { if((dest[i]&data[i])!=data[i]) return 1; dest[i]&=data[i]; }
    if(dest==UPGRADE_FLASH_PTR(UPGRADE_FLAG_ADDR) && *data==0xaa) commitCount++;
    return 0;
}
Uint16 Flash_Verify(Uint16 *dest,Uint16 *data,Uint16 words,FLASH_ST *status)
{
    Uint16 i; verifyCount++; if(failVerify) return 1;
    for(i=0;i<words;i++) if(dest[i]!=data[i]) return 1;
    return 0;
}
void TestDigest(Uint16 *input,Uint32 length,Uint16 *digest)
{
    UPGRADE_MD5 context; Uint32 i;
    UpgradeMd5Init(&context);
    for(i=0;i<length;i++) UpgradeMd5Byte(&context,input[i]);
    UpgradeMd5Finish(&context,digest);
}
void TestReady(void) { RemoteModbus.apiReady=1; }
Uint16 *TestReceive(void) { return RemoteModbus.ReceiveBuf; }
Uint16 *TestReply(void) { return RemoteModbus.SendBuf; }
Uint16 TestReplyLength(void) { return RemoteModbus.TransmitLength; }
Uint16 TestHold(void) { return RemoteModbus.nAppJumpFlag; }
Uint16 TestAction(void) { return RemoteModbus.pendingAction; }

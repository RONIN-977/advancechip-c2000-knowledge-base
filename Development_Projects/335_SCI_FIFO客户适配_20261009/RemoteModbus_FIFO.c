#ifdef UPGRADE_HOST_TEST
#include "TestPlatform.h"
#else
#include "MainInclude.h"
#endif
#if UPGRADE_IS_BOOT
#include "UpgradeMd5.h"
FLASH_ST FlashStatus;
#endif

TRemoteModbus RemoteModbus;
static Uint16 cachedRequest[160], cachedReply[48], cachedLength, cachedReplyLength;
static Uint16 command, sequence, requestLength;
static Uint16 crc8(const Uint16 *data, Uint16 count)
{
    Uint16 crc=0,i;
    while(count--) { crc^=*data++&255; for(i=0;i<8;i++) crc=(crc>>1)^((crc&1)?0x8c:0); }
    return crc;
}
static Uint16 crc16(const Uint16 *data, Uint16 count)
{
    Uint16 crc=0,i;
    while(count--) { crc^=*data++&255; for(i=0;i<8;i++) crc=(crc>>1)^((crc&1)?0xa001:0); }
    return crc;
}
static Uint32 get32(const Uint16 *data)
{
    return (Uint32)data[0] | ((Uint32)data[1]<<8) | ((Uint32)data[2]<<16) | ((Uint32)data[3]<<24);
}
static void put32(Uint16 *data, Uint32 value)
{
    Uint16 i; for(i=0;i<4;i++) data[i]=(Uint16)(value>>(8*i))&255;
}
static void reply(Uint16 status, Uint16 payloadLength)
{
    Uint16 *data=RemoteModbus.SendBuf, size=11+payloadLength, check;
    data[0]=0xcc; data[1]=(size<<3)&255; data[2]=(size<<3)>>8; data[3]=crc8(data,3);
    data[4]=UPGRADE_IS_BOOT?0x89:0x81;
    data[5]=command&255; data[6]=command>>8; data[7]=sequence&255; data[8]=sequence>>8;
    data[9]=status; check=crc16(data,size-2); data[size-2]=check&255; data[size-1]=check>>8;
    RemoteModbus.TransmitLength=size;
}

#if UPGRADE_IS_BOOT
#ifndef UPGRADE_FLASH_PTR
#define UPGRADE_FLASH_PTR(address) ((Uint16 *)(address))
#endif
#ifndef UPGRADE_HOST_TEST
#pragma CODE_SECTION(UpgradeWatchdog,"ramfuncs");
#endif
void UpgradeWatchdog(void)
{
#ifndef UPGRADE_HOST_TEST
    /* The Flash API calls back with EALLOW set; preserve its status register. */
    asm(" PUSH ST1");
    EALLOW; SysCtrlRegs.WDKEY=0x55; SysCtrlRegs.WDKEY=0xaa;
    asm(" POP ST1");
#endif
}
static Uint16 program(Uint32 address, Uint16 *data, Uint16 words)
{
    Uint16 writeStatus, verifyStatus;
    DINT;
    writeStatus=Flash_Program(UPGRADE_FLASH_PTR(address),data,words,&FlashStatus);
    verifyStatus=Flash_Verify(UPGRADE_FLASH_PTR(address),data,words,&FlashStatus);
    EINT;
    return writeStatus==STATUS_SUCCESS && verifyStatus==STATUS_SUCCESS;
}
static Uint16 beginUpdate(void)
{
    Uint16 status, invalid=0;
    Uint32 bytes;
    if(requestLength!=15 || !RemoteModbus.apiReady) return 1;
    bytes=get32(&RemoteModbus.ReceiveBuf[9]);
    if(bytes<4 || (bytes&1) || bytes>UPGRADE_MAX_BYTES) return 5;
    RemoteModbus.sessionActive=0;
    if(*UPGRADE_FLASH_PTR(UPGRADE_FLAG_ADDR)==0xaa && !program(UPGRADE_FLAG_ADDR,&invalid,1)) return 1;
    /* Erase B (metadata) first. Any interruption after this point leaves an invalid APP. */
    DINT; status=Flash_Erase(SECTORB,&FlashStatus); EINT;
    if(status!=STATUS_SUCCESS) return 1;
    if(UPGRADE_APP_ERASE_MASK) {
        DINT; status=Flash_Erase(UPGRADE_APP_ERASE_MASK,&FlashStatus); EINT;
        if(status!=STATUS_SUCCESS) return 1;
    }
    RemoteModbus.firmwareBytes=bytes; RemoteModbus.receivedBytes=0; RemoteModbus.nextPacket=0;
    RemoteModbus.sessionActive=1; return 0;
}
static Uint16 writePacket(void)
{
    Uint32 index, offset, remaining;
    Uint16 bytes,i,words[64];
    if(!RemoteModbus.sessionActive || requestLength<17) return 5;
    index=get32(&RemoteModbus.ReceiveBuf[9]);
    bytes=RemoteModbus.ReceiveBuf[13]|(RemoteModbus.ReceiveBuf[14]<<8);
    if(index!=RemoteModbus.nextPacket || bytes==0 || bytes>128 || (bytes&1) || requestLength!=17+bytes) return 5;
    offset=index*128UL;
    if(offset!=RemoteModbus.receivedBytes || offset>=RemoteModbus.firmwareBytes) return 5;
    remaining=RemoteModbus.firmwareBytes-offset;
    if(bytes!=(remaining>128?128:(Uint16)remaining) || offset+bytes>UPGRADE_MAX_BYTES) return 5;
    for(i=0;i<bytes/2;i++) words[i]=RemoteModbus.ReceiveBuf[15+2*i]|(RemoteModbus.ReceiveBuf[16+2*i]<<8);
    if(!program(UPGRADE_APP_START+offset/2,words,bytes/2)) { RemoteModbus.sessionActive=0; return 1; }
    RemoteModbus.receivedBytes+=bytes; RemoteModbus.nextPacket++; return 0;
}
static Uint16 finishUpdate(void)
{
    UPGRADE_MD5 md5;
    Uint16 digest[16],word,valid=0xaa,i;
    Uint32 offset;
    if(requestLength!=27 || !RemoteModbus.sessionActive || RemoteModbus.receivedBytes!=RemoteModbus.firmwareBytes) return 5;
    UpgradeMd5Init(&md5);
    for(offset=0;offset<RemoteModbus.firmwareBytes;offset+=2) {
        word=*UPGRADE_FLASH_PTR(UPGRADE_APP_START+offset/2);
        UpgradeMd5Byte(&md5,word&255); UpgradeMd5Byte(&md5,word>>8);
        if((offset&127)==0) UpgradeWatchdog();
    }
    UpgradeMd5Finish(&md5,digest);
    for(i=0;i<16;i++) if(digest[i]!=RemoteModbus.ReceiveBuf[9+i]) { RemoteModbus.sessionActive=0; return 3; }
    if(!program(UPGRADE_FLAG_ADDR,&valid,1)) { RemoteModbus.sessionActive=0; return 1; }
    RemoteModbus.sessionActive=0; return 0;
}
#endif

void RemoteModbus_Initialize(void)
{
    memset(&RemoteModbus,0,sizeof(RemoteModbus)); cachedLength=0; cachedReplyLength=0;
    RemoteModbus.nAppJumpFlag=1;
}
/* Used by the SCI state machine and by the native regression harness. */
void UpgradeHandleFrame(void)
{
    Uint16 i,same,status=5,payload=1;
    Uint16 *data=RemoteModbus.ReceiveBuf;
    requestLength=RemoteModbus.RxBufferIndex;
    RemoteModbus.TransmitLength=0;
    if(requestLength<11 || requestLength>160 || data[0]!=0xcc || ((data[1]|(data[2]<<8))>>3)!=requestLength ||
       data[3]!=crc8(data,3) || (data[4]&0x80) || (data[requestLength-2]|(data[requestLength-1]<<8))!=crc16(data,requestLength-2)) return;
    command=data[5]|(data[6]<<8); sequence=data[7]|(data[8]<<8);
#if UPGRADE_IS_BOOT
    RemoteModbus.nAppJumpFlag=0;
#endif
    same=requestLength==cachedLength;
    for(i=0;same && i<requestLength;i++) if(data[i]!=cachedRequest[i]) same=0;
    if(same) { memcpy(RemoteModbus.SendBuf,cachedReply,cachedReplyLength*sizeof(Uint16)); RemoteModbus.TransmitLength=cachedReplyLength; return; }
    memset(RemoteModbus.SendBuf,0,sizeof(RemoteModbus.SendBuf));
    switch(command) {
    case 0x01:
        if(requestLength!=11) break;
        payload=26; status=0;
        put32(&RemoteModbus.SendBuf[27],UPGRADE_BOOT_VERSION);
#if !UPGRADE_IS_BOOT
        put32(&RemoteModbus.SendBuf[31],UPGRADE_BOOT_VERSION);
#endif
        break;
    case 0x71:
        if(requestLength!=11) break;
        payload=3; status=UPGRADE_IS_BOOT?1:0; break; /* State is the first byte, not an error code. */
    case 0x72:
        if(requestLength!=11) break;
        status=0;
#if !UPGRADE_IS_BOOT
        RemoteModbus.pendingAction=1;
#endif
        break;
#if UPGRADE_IS_BOOT
    case 0x70:
        status=beginUpdate(); payload=3; RemoteModbus.SendBuf[10]=128; break;
    case 0x73:
        status=writePacket(); payload=5;
        if(requestLength>=15) put32(&RemoteModbus.SendBuf[10],get32(&data[9]));
        break;
    case 0x74: status=finishUpdate(); break;
    case 0x75:
    case 0x02:
        if(requestLength==11 && *UPGRADE_FLASH_PTR(UPGRADE_FLAG_ADDR)==0xaa) { status=0; RemoteModbus.pendingAction=2; }
        break;
#else
    case 0x02: if(requestLength==11) { status=0; RemoteModbus.pendingAction=3; } break;
#endif
    default: break;
    }
    reply(status,payload);
    memcpy(cachedRequest,data,requestLength*sizeof(Uint16)); cachedLength=requestLength;
    cachedReplyLength=RemoteModbus.TransmitLength; memcpy(cachedReply,RemoteModbus.SendBuf,cachedReplyLength*sizeof(Uint16));
}


#include "uC_Inc.h"
#include "boot_core.h"
#include "kb_can_upgrade.h"
#include "kb_can_upgrade_config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#define EXPORT __declspec(dllexport)
#define CHECK(x) do { if(!(x)){fprintf(stderr,"MODEL ASSERT line %d: %s\n",__LINE__,#x);abort();} } while(0)
volatile uint16_t request_words[4];
uint16_t model_dlc;
static bl_context context;
static bl_can_rx request_rx;
static uint16_t flash[BL_APP_END-BL_APP_START],written[(BL_APP_END-BL_APP_START)/4];
static uint32_t erases,programs,commits,resets,wordcount,frames_rx,frames_tx,ticks,counter=UINT32_MAX;
static uint32_t valid_objects;
static uint16_t mode,valid,fault,pending,rx_frame[16],tx_frame[8],tx_dlc,tx_busy,shutdown_calls;
static uint16_t queue[64][8],queue_dlc[64],queue_read,queue_write;
static bool disabled;
static jmp_buf reset_jump;
static void enqueue(const uint16_t *frame,uint16_t dlc)
{ CHECK(queue_write<64);memcpy(queue[queue_write],frame,dlc*sizeof(uint16_t));queue_dlc[queue_write++]=dlc;frames_tx++; }
static uint32_t image_crc(uint32_t count)
{
    uint32_t crc=UINT32_MAX;
    for(uint32_t i=0;i<count;i++)for(unsigned j=0;j<2;j++){
        crc^=(flash[i]>>(8*j))&255;
        for(unsigned k=0;k<8;k++)crc=(crc>>1)^((crc&1)?UINT32_C(0xedb88320):0);
    }
    return ~crc;
}
static uint16_t begin(void *u,const bl_manifest *m)
{
    (void)u;(void)m;valid=0;if(fault==1)return 0;
    memset(flash,255,sizeof flash);memset(written,0,sizeof written);wordcount=0;erases++;return 1;
}
static uint16_t program(void *u,uint32_t addr,const uint16_t *words,uint16_t count)
{
    (void)u;uint32_t off=addr-BL_APP_START;
    if(fault==2||addr<BL_APP_START||addr>=BL_APP_END||count>BL_APP_END-addr||(off&3)||(count&3))return 0;
    for(unsigned i=0;i<count;i+=4)if(written[(off+i)/4])return 0;
    for(unsigned i=0;i<count;i++)if((flash[off+i]&words[i])!=words[i])return 0;
    for(unsigned i=0;i<count;i+=4)written[(off+i)/4]=1;
    memcpy(flash+off,words,count*sizeof(uint16_t));programs++;wordcount+=count;return 1;
}
static uint16_t verify(void *u,const bl_manifest *m) {(void)u;return fault!=3&&image_crc(m->count)==m->crc;}
static uint16_t commit(void *u,const bl_manifest *m) {(void)u;(void)m;if(fault==4)return 0;valid=1;commits++;return 1;}
static const bl_flash_ops ops={0,begin,program,verify,commit};
uint32_t boot_millis(void){return ++ticks;}
uint16_t boot_send_reply(uint16_t,const bl_octet *,uint16_t);

bool Interrupt_disableMaster(void){bool old=disabled;disabled=true;return old;}
void Interrupt_enableMaster(void){disabled=false;}
uint32_t CAN_getValidMessageObjects(uint32_t b){(void)b;CHECK(disabled);return valid_objects;}
void CAN_setupMessageObject(uint32_t b,uint16_t obj,uint32_t id,unsigned type,unsigned direction,uint32_t mask,uint32_t flags,uint16_t dlc)
{
    (void)b;(void)id;(void)type;(void)direction;(void)mask;(void)flags;
    CHECK(disabled&&dlc<=8&&(obj==31||obj==32));valid_objects|=UINT32_C(1)<<(obj-1);
}
bool CAN_readMessageWithID(uint32_t b,uint16_t obj,CAN_MsgFrameType *type,uint32_t *id,uint16_t *frame)
{
    (void)b;CHECK(disabled&&obj==31);if(!pending)return false;
    pending=0;*type=CAN_MSG_FRAME_STD;*id=KB_UPGRADE_RX_ID;memcpy(frame,rx_frame,model_dlc*sizeof(uint16_t));return true;
}
uint32_t CAN_getTxRequests(uint32_t b){(void)b;return (!mode&&tx_busy)?UINT32_C(0x80000000):0;}
void CAN_sendMessage(uint32_t b,uint16_t obj,uint16_t dlc,const uint16_t *data)
{(void)b;CHECK(disabled&&obj==32&&!tx_busy);memcpy(tx_frame,data,dlc*sizeof(uint16_t));tx_dlc=dlc;tx_busy=1;}
void CAN_sendMessage_updateDLC(uint32_t b,uint32_t obj,uint16_t dlc,const uint16_t *data)
{(void)b;CHECK(mode==1&&obj==2);enqueue(data,dlc);}
void SysCtl_enablePeripheral(unsigned x){CHECK(disabled&&x==2);}
void CPUTimer_stopTimer(unsigned x){CHECK(disabled&&x==2);}
void CPUTimer_disableInterrupt(unsigned x){CHECK(disabled&&x==2);}
void CPUTimer_setPreScaler(unsigned x,unsigned n){CHECK(disabled&&x==2&&n==99);}
void CPUTimer_setPeriod(unsigned x,uint32_t n){CHECK(disabled&&x==2&&n==UINT32_MAX);}
void CPUTimer_reloadTimerCounter(unsigned x){CHECK(disabled&&x==2);counter=UINT32_MAX;}
void CPUTimer_startTimer(unsigned x){CHECK(disabled&&x==2);}
uint32_t CPUTimer_getTimerCount(unsigned x){CHECK(x==2);return counter;}
void SysCtl_serviceWatchdog(void){}
void SysCtl_delay(uint32_t x){CHECK(x==100000000UL/3000);}
void SysCtl_resetDevice(void)
{
    CHECK(disabled&&!tx_busy&&frames_tx==4&&shutdown_calls==1);
    CHECK(request_words[0]==0xb007&&request_words[1]==0x4ff8&&request_words[2]==0x5aa5&&request_words[3]==0xa55a);
    mode=1;resets++;bl_init(&context,&ops);bl_can_reset(&request_rx);longjmp(reset_jump,1);
}
/* Compiler function instrumentation observes the actual empty customer hook,
 * without editing adapter/hook source. Other function entries are ignored. */
__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void *fn,void *caller)
{
    (void)caller;
    if(fn==(void *)(uintptr_t)&Customer_ShutdownForUpgrade){CHECK(!disabled&&!tx_busy&&frames_tx==4);shutdown_calls++;}
}
__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void *fn,void *caller){(void)fn;(void)caller;}

EXPORT void native_reset(void)
{
    erases=programs=commits=resets=wordcount=frames_rx=frames_tx=ticks=0;
    mode=valid=fault=pending=queue_read=queue_write=tx_busy=shutdown_calls=0;
    disabled=false;valid_objects=0;kb_upgrade_status=0;counter=UINT32_MAX;
    memset(flash,255,sizeof flash);memset(written,0,sizeof written);memset((void*)request_words,0,sizeof request_words);
    bl_init(&context,&ops);bl_can_reset(&request_rx);KB_CAN_UpgradeTask();CHECK(kb_upgrade_status==1&&!disabled);
}
EXPORT void native_fault(uint16_t x){fault=x;}
EXPORT uint16_t native_read_word(uint32_t addr){return addr>=BL_APP_START&&addr<BL_APP_END?flash[addr-BL_APP_START]:0;}
EXPORT uint32_t native_stat(uint16_t i)
{
    switch(i){case 0:return mode;case 1:return erases;case 2:return programs;case 3:return commits;case 4:return valid;
    case 5:return resets;case 6:return wordcount;case 7:return image_crc(context.image.count);case 8:return frames_rx;
    case 9:return frames_tx;case 10:return context.state;case 11:return shutdown_calls;case 12:return disabled;
    case 13:return BOOT_METADATA_ADDRESS;case 14:return BL_APP_START;default:return 0;}
}
EXPORT void native_step(uint16_t count)
{
    for(unsigned i=0;i<count&&!mode;i++){
        counter-=1000;
        if(tx_busy&&fault!=5){enqueue(tx_frame,tx_dlc);tx_busy=0;}
        if(!setjmp(reset_jump)){bool old=disabled;KB_CAN_UpgradeTask();CHECK(old==disabled);}
    }
}
EXPORT void native_feed_frame(const uint16_t *frame,uint16_t dlc)
{
    CHECK(dlc<=8);frames_rx++;
    if(!mode){CHECK(!pending);memcpy(rx_frame,frame,dlc*sizeof(uint16_t));model_dlc=dlc;pending=1;native_step(1);}
    else{
        uint16_t count=bl_can_feed(&request_rx,frame,dlc),out[BL_MAX_PACKET];
        if(count){count=bl_handle(&context,request_rx.data,count,out);
            if(count&&boot_send_reply(2,out,count)&&request_rx.data[3]==BL_RESET&&out[14]==BL_OK){CHECK(valid);mode=0;resets++;}}
    }
}
EXPORT uint16_t native_pop_frame(uint16_t *out)
{
    if(queue_read==queue_write){queue_read=queue_write=0;return 0;}
    uint16_t n=queue_dlc[queue_read];memcpy(out,queue[queue_read++],n*sizeof(uint16_t));return n;
}

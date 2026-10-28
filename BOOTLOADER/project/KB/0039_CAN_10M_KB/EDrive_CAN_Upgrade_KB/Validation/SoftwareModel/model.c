#include "uC_Inc.h"
#include "boot_core.h"
#include "kb_can_upgrade.h"
#include "kb_can_upgrade_config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#undef assert
#define assert(x) do { if (!(x)) { fprintf(stderr,"ASSERT %s:%d: %s\n",__FILE__,__LINE__,#x); abort(); } } while (0)

volatile uint16_t request_words[4];
uint16_t model_dlc;
static bool disabled,available,busy;
static uint32_t valid,counter=UINT32_MAX,in_id=KB_UPGRADE_RX_ID;
static unsigned in_type=CAN_MSG_FRAME_STD,setup_calls,tx_calls,reset_calls,watchdogs,delays;
static unsigned shutdown_calls,shutdown_disable_seen;
static bool initial_irq_state,shutdown_irq_state;
static uint16_t incoming[16],out[BL_MAX_PACKET],out_len;
static jmp_buf reset_jump;

bool Interrupt_disableMaster(void)
{
    bool old=disabled;
    if(shutdown_calls){
        assert(shutdown_calls==1&&shutdown_disable_seen==0);
        assert(disabled==shutdown_irq_state);
        shutdown_disable_seen=1;
    }
    disabled=true;return old;
}
void Interrupt_enableMaster(void) { disabled=false; }
uint32_t CAN_getValidMessageObjects(uint32_t b) { (void)b; assert(disabled);return valid; }
void CAN_setupMessageObject(uint32_t b,uint16_t obj,uint32_t id,unsigned type,unsigned direction,uint32_t mask,uint32_t flags,uint16_t dlc)
{
    (void)b;(void)type;(void)mask;
    assert(disabled);assert(obj==31 || obj==32);assert(dlc<=8);
    if(obj==31){assert(id==0x601);assert(direction==CAN_MSG_OBJ_TYPE_RX);assert(flags==7);}
    else {assert(id==0x581);assert(direction==CAN_MSG_OBJ_TYPE_TX);assert(flags==0);}
    valid|=UINT32_C(1)<<(obj-1);++setup_calls;
}
uint32_t CAN_getTxRequests(uint32_t b) { (void)b;return busy?UINT32_C(0x80000000):0; }
void CAN_sendMessage(uint32_t b,uint16_t obj,uint16_t dlc,const uint16_t *data)
{
    (void)b;assert(disabled);assert(!busy);assert(obj==32);assert(dlc>=3&&dlc<=8);
    assert((data[1]&0x7f)==tx_calls%4);assert((tx_calls%4)?!(data[1]&0x80):(data[1]&0x80));
    for(unsigned i=2;i<dlc;i++)out[out_len++]=data[i];
    ++tx_calls;busy=true;
}
bool CAN_readMessageWithID(uint32_t b,uint16_t obj,CAN_MsgFrameType *type,uint32_t *id,uint16_t *frame)
{
    (void)b;assert(disabled);assert(obj==31);
    if(!available)return false;
    available=false;*type=in_type;*id=in_id;
    for(unsigned i=0;i<model_dlc;i++)frame[i]=incoming[i];
    return true;
}
void SysCtl_enablePeripheral(unsigned x) { assert(disabled&&x==2); }
void CPUTimer_stopTimer(unsigned x) { assert(disabled&&x==2); }
void CPUTimer_disableInterrupt(unsigned x) { assert(disabled&&x==2); }
void CPUTimer_setPreScaler(unsigned x,unsigned n) { assert(disabled&&x==2&&n==99); }
void CPUTimer_setPeriod(unsigned x,uint32_t n) { assert(disabled&&x==2&&n==UINT32_MAX); }
void CPUTimer_reloadTimerCounter(unsigned x) { assert(disabled&&x==2);counter=UINT32_MAX; }
void CPUTimer_startTimer(unsigned x) { assert(disabled&&x==2); }
uint32_t CPUTimer_getTimerCount(unsigned x) { assert(x==2);return counter; }
void SysCtl_serviceWatchdog(void) { ++watchdogs; }
void SysCtl_delay(uint32_t n) { assert(n==100000000UL/3000);++delays; }
/* Instrumented test implementation replaces the separately byte-checked empty
 * delivery function. It must run once, after the final ACK completes and before
 * the reset path changes INTM or writes any request marker.
 */
void Customer_ShutdownForUpgrade(void)
{
    assert(shutdown_calls==0&&reset_calls==0);
    assert(!busy&&tx_calls==4&&out_len==24&&out[14]==BL_OK);
    assert(disabled==initial_irq_state);
    assert(request_words[0]==0&&request_words[1]==0&&request_words[2]==0&&request_words[3]==0);
    shutdown_irq_state=disabled;++shutdown_calls;
}
void SysCtl_resetDevice(void)
{
    assert(disabled);assert(!busy);assert(tx_calls==4);assert(out_len==24);assert(out[14]==BL_OK);
    assert(request_words[0]==0xb007&&request_words[1]==0x4ff8&&request_words[2]==0x5aa5&&request_words[3]==0xa55a);
    assert(shutdown_calls==1&&shutdown_disable_seen==1&&shutdown_irq_state==initial_irq_state);
    assert(watchdogs==20&&delays==20);
    assert(bl_crc32(out+2,18)==((uint32_t)out[20]|((uint32_t)out[21]<<8)|((uint32_t)out[22]<<16)|((uint32_t)out[23]<<24)));
    ++reset_calls;longjmp(reset_jump,1);
}
static void task(void) { bool old=disabled;KB_CAN_UpgradeTask();assert(disabled==old); }
static void init(void) { task();assert(kb_upgrade_status==1&&setup_calls==2); }
static void packet(uint16_t *p,uint16_t cmd)
{
    memset(p,0,18*sizeof(*p));p[0]=0xa5;p[1]=0x5a;p[2]=1;p[3]=cmd;
    uint32_t crc=bl_crc32(p+2,12);
    for(unsigned i=0;i<4;i++)p[14+i]=(crc>>(8*i))&255;
}
static void fragment(const uint16_t *p,unsigned index)
{
    incoming[0]=17;incoming[1]=index|(index?0:0x80);
    for(unsigned i=0;i<6;i++)incoming[i+2]=p[index*6+i];
    model_dlc=8;available=true;task();
}
static void feed(uint16_t *p) { for(unsigned i=0;i<3;i++)fragment(p,i); }
static void no_reset(void) { assert(reset_calls==0&&request_words[0]==0&&tx_calls==0&&shutdown_calls==0); }
static void complete(void)
{
    if(!setjmp(reset_jump)){
        for(unsigned i=0;i<4;i++){
            busy=false;task();assert(tx_calls==i+1);assert(reset_calls==0&&request_words[0]==0&&shutdown_calls==0);
            task();assert(tx_calls==i+1);assert(reset_calls==0&&request_words[0]==0&&shutdown_calls==0);
        }
        busy=false;task();assert(!"expected reset");
    }
    assert(reset_calls==1&&shutdown_calls==1&&shutdown_disable_seen==1);
}
int main(int argc,char **argv)
{
    assert(argc==2);const char *name=argv[1];uint16_t p[18];packet(p,BL_ENTER_BOOT);
    if(!strcmp(name,"occupied_rx")||!strcmp(name,"occupied_tx")){
        valid=UINT32_C(1)<<(!strcmp(name,"occupied_rx")?30:31);uint32_t before=valid;
        disabled=true;task();assert(kb_upgrade_status==3&&setup_calls==0&&valid==before&&disabled);
        task();assert(setup_calls==0);no_reset();
    }else{
        if(!strcmp(name,"irq_disabled"))disabled=true;
        initial_irq_state=disabled;
        init();
        if(!strcmp(name,"enter")||!strcmp(name,"irq_disabled")){feed(p);assert(kb_upgrade_status==2);complete();}
        else if(!strcmp(name,"info")||!strcmp(name,"data")){
            packet(p,!strcmp(name,"info")?BL_INFO:BL_DATA);feed(p);task();assert(kb_upgrade_status==1);no_reset();
        }else if(!strcmp(name,"bad_crc")){p[17]^=1;feed(p);task();no_reset();}
        else if(!strcmp(name,"out_of_order")){fragment(p,0);fragment(p,2);fragment(p,1);task();no_reset();}
        else if(!strcmp(name,"partial_timeout")){fragment(p,0);counter-=201000;fragment(p,1);fragment(p,2);task();no_reset();}
        else if(!strcmp(name,"partial_200_boundary")){fragment(p,0);counter-=200000;fragment(p,1);fragment(p,2);assert(kb_upgrade_status==2);complete();}
        else if(!strcmp(name,"tx_timeout_recovery")){
            feed(p);task();assert(tx_calls==1&&busy);counter-=501000;task();assert(kb_upgrade_status==1&&reset_calls==0&&request_words[0]==0&&shutdown_calls==0);
            busy=false;tx_calls=out_len=0;feed(p);assert(kb_upgrade_status==2);complete();
        }else if(!strcmp(name,"timer_wrap")){
            counter=1000;task();fragment(p,0);counter-=1500;fragment(p,1);fragment(p,2);assert(kb_upgrade_status==2);complete();
        }else if(!strcmp(name,"wrong_id")) { in_id=0x602;feed(p);task();no_reset(); }
        else if(!strcmp(name,"extended_frame")) { in_type=1;feed(p);task();no_reset(); }
        else if(!strcmp(name,"bad_dlc")) { model_dlc=15;available=true;task();no_reset(); }
        else assert(!"unknown test");
    }
    printf("PASS %s\n",name);return 0;
}

/* Host C protocol model: actual latest KB core, APP entry and reply fragmenter.
 * Hardware CAN, timer, watchdog and Flash API are deliberately modeled. */
#include "boot_core.h"
#include "boot_io.h"
#include "upgrade_entry.h"
#include "driverlib.h"
#include "board_config.h"
#include <string.h>
#define EXPORT __declspec(dllexport)
static bl_context context;
static bl_can_rx request_rx, reply_rx;
static uint16_t flash[BL_APP_END-BL_APP_START], written[(BL_APP_END-BL_APP_START)/4];
static uint32_t erases,programs,commits,resets,wordcount,frames_rx,frames_tx,ticks;
static uint16_t mode,valid,fault,pending,rx_frame[8],*output,output_count;
uint16_t mock_dlc;
static uint32_t image_crc(uint32_t count)
{
    uint32_t crc=0xffffffffU,i;uint16_t j,k;
    for(i=0;i<count;i++)for(j=0;j<2;j++) {
        crc^=(flash[i]>>(8*j))&255U;
        for(k=0;k<8;k++)crc=(crc>>1)^((crc&1)?0xedb88320U:0);
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
    uint32_t off=addr-BL_APP_START;uint16_t i;(void)u;
    if(fault==2||addr<BL_APP_START||addr>=BL_APP_END||count>BL_APP_END-addr||(off&3)||(count&3))return 0;
    for(i=0;i<count;i+=4)if(written[(off+i)/4])return 0;
    for(i=0;i<count;i++)if((flash[off+i]&words[i])!=words[i])return 0;
    for(i=0;i<count;i+=4)written[(off+i)/4]=1;
    memcpy(flash+off,words,count*sizeof(uint16_t));programs++;wordcount+=count;return 1;
}
static uint16_t verify(void *u,const bl_manifest *m)
{(void)u;return fault!=3&&image_crc(m->count)==m->crc;}
static uint16_t commit(void *u,const bl_manifest *m)
{(void)u;(void)m;if(fault==4)return 0;valid=1;commits++;return 1;}
static const bl_flash_ops ops={0,begin,program,verify,commit};
void boot_timers_init(void){}
void boot_ports_init(void){}
uint32_t boot_millis(void){return ++ticks;}
void boot_request_reset(void)
{mode=1;resets++;bl_init(&context,&ops);bl_can_reset(&request_rx);}
bool CAN_readMessageWithID(uint32_t base,uint32_t obj,CAN_MsgFrameType *type,uint32_t *id,uint16_t *data)
{(void)base;(void)obj;if(!pending)return false;pending=0;*type=CAN_MSG_FRAME_STD;*id=BOOT_CAN_RX_ID;memcpy(data,rx_frame,mock_dlc*sizeof(uint16_t));return true;}
uint32_t CAN_getTxRequests(uint32_t base){(void)base;return fault==5?2:0;}
void CAN_sendMessage_updateDLC(uint32_t base,uint32_t obj,uint16_t dlc,const uint16_t *data)
{
    uint16_t count;(void)base;(void)obj;frames_tx++;
    count=bl_can_feed(&reply_rx,data,dlc);
    if(count){memcpy(output,reply_rx.data,count*sizeof(uint16_t));output_count=count;}
}
EXPORT void native_reset(void)
{
    erases=programs=commits=resets=wordcount=frames_rx=frames_tx=ticks=0;
    mode=valid=fault=pending=output_count=0;memset(flash,255,sizeof flash);
    memset(written,0,sizeof written);bl_init(&context,&ops);
    bl_can_reset(&request_rx);bl_can_reset(&reply_rx);upgrade_entry_init();
}
EXPORT void native_fault(uint16_t value){fault=value;}
EXPORT uint16_t native_read_word(uint32_t address)
{return address>=BL_APP_START&&address<BL_APP_END?flash[address-BL_APP_START]:0;}
EXPORT uint32_t native_stat(uint16_t index)
{
    switch(index){case 0:return mode;case 1:return erases;case 2:return programs;
    case 3:return commits;case 4:return valid;case 5:return resets;case 6:return wordcount;
    case 7:return image_crc(context.image.count);case 8:return frames_rx;
    case 9:return frames_tx;case 10:return context.state;default:return 0;}
}
static void feed(const uint16_t *frame,uint16_t dlc)
{
    uint16_t count,out[BL_MAX_PACKET];frames_rx++;
    if(!mode){memcpy(rx_frame,frame,dlc*sizeof(uint16_t));mock_dlc=dlc;pending=1;upgrade_entry_poll();}
    else {
        count=bl_can_feed(&request_rx,frame,dlc);
        if(count){
            count=bl_handle(&context,request_rx.data,count,out);
            if(count&&boot_send_reply(2,out,count)&&request_rx.data[3]==BL_RESET&&out[14]==BL_OK){
                mode=0;resets++;upgrade_entry_init();
            }
        }
    }
}
EXPORT uint16_t native_exchange(uint16_t transport,const bl_octet *input,uint16_t size,bl_octet *out)
{
    static uint16_t transfer;uint16_t frames[25][8],dlcs[25],offset,n,i,k,total;
    if(transport!=2||size>BL_MAX_PACKET||size<1)return 0;
    output=out;output_count=0;bl_can_reset(&reply_rx);transfer=(transfer+1)&255;
    total=0;
    for(offset=0;offset<size;offset+=n){
        n=size-offset>6?6:size-offset;frames[total][0]=transfer;
        frames[total][1]=total|(total?0:0x80);
        for(i=0;i<n;i++)frames[total][i+2]=input[offset+i];
        dlcs[total++]=n+2;
    }
    for(k=0;k<total;k++){
        if(fault==6&&k==1)continue;
        i=k;if(fault==7&&total>2){if(k==1)i=2;else if(k==2)i=1;}
        feed(frames[i],dlcs[i]);
    }
    return output_count;
}

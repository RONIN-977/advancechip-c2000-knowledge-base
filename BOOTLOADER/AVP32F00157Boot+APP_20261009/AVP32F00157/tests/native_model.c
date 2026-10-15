/* Host-only test adapter. Executes the exact core/boot_core.c; no TI hardware API.
 * Every wire octet still occupies uint16_t, matching the C28x protocol contract.
 */
#include "boot_core.h"
#include <string.h>
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
static bl_context context;
static bl_sci_rx sci;
static bl_can_rx can;
static uint16_t flash[BL_APP_END - BL_APP_START];
static uint16_t written[(BL_APP_END - BL_APP_START) / 4];
static uint32_t erases, programs, commits;
static uint16_t valid, fault;
static uint32_t crc_words(const uint16_t *words, uint32_t count)
{
    uint32_t crc=0xffffffffU, i;
    uint16_t k,b;
    for(i=0;i<count;i++) for(k=0;k<2;k++) {
        crc ^= (words[i] >> (8*k)) & 255U;
        for(b=0;b<8;b++) crc=(crc>>1)^((crc&1)?0xedb88320U:0);
    }
    return ~crc;
}
static uint16_t begin(void *u, const bl_manifest *m)
{
    (void)u;(void)m;
    valid=0;
    if(fault==1) return 0;
    memset(flash,255,sizeof(flash)); memset(written,0,sizeof(written));
    erases++; return 1;
}
static uint16_t program(void *u,uint32_t address,const uint16_t *words,uint16_t count)
{
    uint32_t offset=address-BL_APP_START; uint16_t i;
    (void)u;
    if(fault==2 || address<BL_APP_START || address>=BL_APP_END || count>BL_APP_END-address || (offset&3) || (count&3)) return 0;
    for(i=0;i<count;i+=4) if(written[(offset+i)/4]) return 0;
    for(i=0;i<count;i+=4) written[(offset+i)/4]=1;
    memcpy(flash+offset,words,count*sizeof(uint16_t)); programs++; return 1;
}
static uint16_t verify(void *u,const bl_manifest *m)
{
    (void)u; return fault!=3 && crc_words(flash,m->count)==m->crc;
}
static uint16_t commit(void *u,const bl_manifest *m)
{
    (void)u;(void)m;
    if(fault==4) return 0;
    commits++; valid=1; return 1;
}
static const bl_flash_ops ops={0,begin,program,verify,commit};
EXPORT void native_reset(void)
{
    erases=programs=commits=0; valid=fault=0;
    bl_init(&context,&ops); bl_sci_reset(&sci); bl_can_reset(&can);
}
EXPORT void native_fault(uint16_t value) { fault=value; }
EXPORT uint32_t native_stat(uint16_t index)
{
    switch(index){case 0:return erases;case 1:return programs;case 2:return commits;case 3:return valid;case 4:return context.state;default:return 0;}
}
EXPORT uint16_t native_exchange(uint16_t transport,const bl_octet *in,uint16_t length,bl_octet *out)
{
    uint16_t size=0, offset,i,n,index=0; bl_octet frame[8];
    if(transport==1){
        for(i=0;i<length;i++) size=bl_sci_feed(&sci,in[i]);
        return size ? bl_handle(&context,sci.data,size,out) : 0;
    }
    for(offset=0;offset<length;offset+=n){
        n=length-offset>6?6:length-offset;
        frame[0]=17; frame[1]=index|(index?0:0x80); index++;
        for(i=0;i<n;i++)frame[i+2]=in[offset+i];
        size=bl_can_feed(&can,frame,n+2);
    }
    return size ? bl_handle(&context,can.data,size,out) : 0;
}

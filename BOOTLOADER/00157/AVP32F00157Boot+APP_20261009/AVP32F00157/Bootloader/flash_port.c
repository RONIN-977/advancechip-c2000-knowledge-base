#include "device.h"
#include "FlashTech_F280015x_C28x.h"
#include "board_config.h"
#include "flash_port.h"

/* Same wrapper address as TI flash_programming_f280015x.h. */
#define FLASH_WRAPPER_PROGRAM_BASE 0x51000UL
#define FlashTech_CPU0_BASE_ADDRESS ((Fapi_FmcRegistersType *)FLASH_WRAPPER_PROGRAM_BASE)

/* Single flash bank: all code executing while FSM is busy resides in RAM. */
volatile uint16_t boot_debug_flash_status=0, boot_debug_flash_stage=0;
volatile uint16_t boot_debug_flash_ram_ok=0;
volatile uint32_t boot_debug_flash_fsm=0, boot_debug_flash_address=0;
volatile Fapi_FlashStatusWordType boot_debug_flash_detail;
#pragma DATA_ALIGN(program_buffer, 4)
static uint16_t program_buffer[4];

#pragma CODE_SECTION(wait_ready,".TI.ramfunc")
#pragma CODE_SECTION(prepare_operation,".TI.ramfunc")
#pragma CODE_SECTION(erase_sector,".TI.ramfunc")
#pragma CODE_SECTION(program_words,".TI.ramfunc")
#pragma CODE_SECTION(boot_flash_init,".TI.ramfunc")

static void wait_ready(void)
{
    uint32_t start=CPUTimer_getTimerCount(CPUTIMER0_BASE);
    while(Fapi_checkFsmForReady()!=Fapi_Status_FsmReady)
    {
        if(start-CPUTimer_getTimerCount(CPUTIMER0_BASE)>5000000UL)
        {
            boot_debug_flash_status=0xFFF0U;
            /* Never return into Flash while FSM is stuck busy. Debug in RAM. */
            for (;;) {}
        }
    }
}
static uint16_t prepare_operation(void)
{
    wait_ready();
    if(Fapi_getFsmStatus()!=0)
    {
        boot_debug_flash_status=Fapi_issueAsyncCommand(Fapi_ClearStatus);
        wait_ready();
        if(boot_debug_flash_status!=Fapi_Status_Success)return 0;
        if(Fapi_getFsmStatus()!=0){boot_debug_flash_status=0xFFF1U;return 0;}
    }
    /* Protection follows app_partition.h, including B register groups.
     * Software bounds reject BOOT addresses even in a shared protection group. */
    Fapi_setupBankSectorEnable(FLASH_WRAPPER_PROGRAM_BASE+FLASH_O_CMDWEPROTA,BOOT_PROTECT_A);
    Fapi_setupBankSectorEnable(FLASH_WRAPPER_PROGRAM_BASE+FLASH_O_CMDWEPROTB,BOOT_PROTECT_B);
    return 1;
}
uint16_t boot_flash_init(void)
{
    uint32_t n=(uint32_t)&RamfuncsLoadSize;
    const uint16_t *load=&RamfuncsLoadStart,*run=&RamfuncsRunStart;
    if(!n || (uint32_t)Fapi_issueProgrammingCommand<0x8000UL ||
       (uint32_t)Fapi_issueProgrammingCommand>=0xA000UL)
    {boot_debug_flash_status=0xFFFC;return 0;}
    while(n--)if(*load++!=*run++){boot_debug_flash_status=0xFFFD;return 0;}
    boot_debug_flash_ram_ok=1;
    boot_debug_flash_status=Fapi_initializeAPI(FlashTech_CPU0_BASE_ADDRESS,DEVICE_SYSCLK_FREQ/1000000U);
    if(boot_debug_flash_status!=Fapi_Status_Success)return 0;
    boot_debug_flash_status=Fapi_setActiveFlashBank(Fapi_FlashBank0);
    return boot_debug_flash_status==Fapi_Status_Success;
}
static uint16_t erase_sector(uint32_t address)
{
    /* Only metadata and APP sectors are legal. Each sector is 0x400 words. */
    if(address<BOOT_METADATA || address>=BL_APP_END || (address&0x3FFUL))return 0;
    boot_debug_flash_address=address;
    DINT;
    if(!prepare_operation())return 0;
    boot_debug_flash_status=Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32_t *)address);
    wait_ready();
    boot_debug_flash_fsm=Fapi_getFsmStatus();
    if(boot_debug_flash_status!=Fapi_Status_Success || boot_debug_flash_fsm!=3U)return 0;
    boot_debug_flash_status=Fapi_doBlankCheck((uint32_t *)address,BOOT_SECTOR_WORDS/2,
        (Fapi_FlashStatusWordType *)&boot_debug_flash_detail);
    return boot_debug_flash_status==Fapi_Status_Success;
}
static uint16_t program_words(uint32_t address,const uint16_t *data,uint16_t count)
{
    uint16_t i,j;
    if(!count || (address&3UL) || (count&3U))return 0;
    if(!((address>=BL_APP_START && address<BL_APP_END && count<=BL_APP_END-address) ||
         (address>=BOOT_METADATA && address<BOOT_METADATA+24UL && count<=BOOT_METADATA+24UL-address)))return 0;
    DINT;
    for(i=0;i<count;i+=4)
    {
        for(j=0;j<4;++j)program_buffer[j]=data[i+j];
        boot_debug_flash_address=address+i;
        if(!prepare_operation())return 0;
        boot_debug_flash_status=Fapi_issueProgrammingCommand((uint32_t *)(address+i),
            program_buffer,4,0,0,Fapi_AutoEccGeneration);
        wait_ready();
        boot_debug_flash_fsm=Fapi_getFsmStatus();
        if(boot_debug_flash_status!=Fapi_Status_Success || boot_debug_flash_fsm!=3U)return 0;
        boot_debug_flash_status=Fapi_doVerify((uint32_t *)(address+i),2,
            (uint32_t *)program_buffer,(Fapi_FlashStatusWordType *)&boot_debug_flash_detail);
        if(boot_debug_flash_status!=Fapi_Status_Success)return 0;
    }
    return 1;
}
static uint32_t words_crc(const uint16_t *p,uint32_t count)
{
    uint32_t crc=0xFFFFFFFFUL,i;uint16_t k,b;
    for(i=0;i<count;++i)for(k=0;k<2;++k)
    {crc^=(p[i]>>(8U*k))&255U;for(b=0;b<8;++b)crc=(crc>>1)^((crc&1UL)?0xEDB88320UL:0UL);}
    return ~crc;
}
static uint32_t get32(const uint16_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<16);}
static void put32(uint16_t *p,uint32_t x){p[0]=(uint16_t)x;p[1]=(uint16_t)(x>>16);}
static uint16_t begin_image(void *user,const bl_manifest *m)
{
    uint32_t sector; (void)user;
    if(m->start!=BL_APP_START || !m->count || (m->count&3UL) ||
       m->count>BL_APP_END-BL_APP_START || m->entry!=BL_APP_START)return 0;
    /* Invalidate by erasing metadata FIRST. Do not reprogram a used ECC line.
     * Interrupted metadata erase leaves the previous APP untouched. */
    boot_debug_flash_stage=BOOT_FLASH_ERASE_METADATA;
    if(!erase_sector(BOOT_METADATA))return 0;
    boot_debug_flash_stage=BOOT_FLASH_ERASE_APP;
    for(sector=BL_APP_START;sector<BL_APP_START+m->count;sector+=BOOT_SECTOR_WORDS)
        if(!erase_sector(sector))return 0;
    return 1;
}
static uint16_t program_image(void *user,uint32_t address,const uint16_t *data,uint16_t count)
{
    (void)user;
    if(address<BL_APP_START || address>=BL_APP_END || count>BL_APP_END-address)return 0;
    boot_debug_flash_stage=BOOT_FLASH_PROGRAM_APP;
    return program_words(address,data,count);
}
static uint16_t verify_image(void *user,const bl_manifest *m)
{
    (void)user;
    return m->start==BL_APP_START && m->count && m->count<=BL_APP_END-BL_APP_START &&
        words_crc((const uint16_t *)m->start,m->count)==m->crc;
}
static uint16_t commit_image(void *user,const bl_manifest *m)
{
    uint16_t h[24]; (void)user;
    h[0]=0x0157;h[1]=0xFEA8;
    put32(h+2,BL_DEVICE_ID);put32(h+4,BL_BOARD_ID);put32(h+6,m->start);put32(h+8,m->count);
    put32(h+10,m->entry);put32(h+12,m->crc);put32(h+14,m->version);put32(h+16,words_crc(h,16));
    h[18]=h[19]=0xFFFF;h[20]=0xA55A;h[21]=0x5AA5;h[22]=0x1570;h[23]=0xEA8F;
    boot_debug_flash_stage=BOOT_FLASH_PROGRAM_HEADER;
    if(!program_words(BOOT_METADATA,h,20))return 0;
    boot_debug_flash_stage=BOOT_FLASH_COMMIT;
    return program_words(BOOT_METADATA+20UL,h+20,4); /* Separate fresh ECC line. */
}
uint16_t boot_image_valid(void)
{
    uint16_t h[24],i;uint32_t count;
    /* Erased/incomplete metadata may have no ECC. Raw copy only; header CRC
     * and final commit tag must both pass before accessing APP with ECC on. */
    Flash_disableECC(FLASH0ECC_BASE);
    for(i=0;i<24;++i)h[i]=((const volatile uint16_t *)BOOT_METADATA)[i];
    Flash_enableECC(FLASH0ECC_BASE);
    if(h[20]!=0xA55A || h[21]!=0x5AA5 || h[22]!=0x1570 || h[23]!=0xEA8F ||
       h[0]!=0x0157 || h[1]!=0xFEA8 || get32(h+2)!=BL_DEVICE_ID || get32(h+4)!=BL_BOARD_ID ||
       get32(h+6)!=BL_APP_START || get32(h+10)!=BL_APP_START || get32(h+16)!=words_crc(h,16))return 0;
    count=get32(h+8);
    return count && !(count&3UL) && count<=BL_APP_END-BL_APP_START &&
        words_crc((const uint16_t *)BL_APP_START,count)==get32(h+12);
}
const bl_flash_ops boot_flash_ops={0,begin_image,program_image,verify_image,commit_image};

#include "device.h"
#include "boot_io.h"
#include "board_config.h"

volatile uint32_t boot_debug_can_errors = 0;
static uint16_t can_transfer = 0;

void boot_device_init(void)
{
    DINT;
    Device_init();              /* TI original: ramfuncs, Flash, XTAL/PLL. */
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFFUL);
    CPUTimer_setPreScaler(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ/1000000UL - 1UL);
    CPUTimer_disableInterrupt(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER0_BASE);
}

uint32_t boot_millis(void)
{
    static uint32_t previous = 0xFFFFFFFFUL, remainder = 0, elapsed = 0;
    uint32_t current = CPUTimer_getTimerCount(CPUTIMER0_BASE);
    uint32_t delta = previous - current;
    previous = current;
    elapsed += delta/1000UL;
    remainder += delta%1000UL;
    if (remainder >= 1000UL) { ++elapsed; remainder -= 1000UL; }
    return elapsed;
}

void boot_ports_init(void)
{
    GPIO_setPinConfig(GPIO_5_CANA_RX);
    GPIO_setPinConfig(GPIO_4_CANA_TX);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(5, GPIO_QUAL_ASYNC);
    boot_sci_init();
    CAN_initModule(CANA_BASE);
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, BOOT_CAN_BAUD, 20);
    CAN_setupMessageObject(CANA_BASE, 1, BOOT_CAN_RX_ID, CAN_MSG_FRAME_STD,
        CAN_MSG_OBJ_TYPE_RX, 0x7FF, CAN_MSG_OBJ_USE_ID_FILTER, 8);
    CAN_setupMessageObject(CANA_BASE, 2, BOOT_CAN_TX_ID, CAN_MSG_FRAME_STD,
        CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
    CAN_startModule(CANA_BASE);
}

uint16_t boot_can_read(bl_octet *frame, uint16_t *dlc)
{
    uint16_t i, ctrl, temporary[16];
    if (!(CAN_getNewDataFlags(CANA_BASE)&1UL)) return 0;
    if (!CAN_readMessage(CANA_BASE, 1, temporary)) return 0;
    ctrl = HWREGH(CANA_BASE + CAN_O_IF2MCTL);
    *dlc = ctrl & CAN_IF2MCTL_DLC_M;
    if ((ctrl & CAN_IF2MCTL_MSGLST) || *dlc > 8 ||
        (HWREG_BP(CANA_BASE + CAN_O_IF2ARB) & CAN_IF2ARB_XTD))
    { ++boot_debug_can_errors; return 0; }
    for (i=0; i<*dlc; ++i) frame[i]=temporary[i]&255U;
    return 1;
}
static uint16_t can_send(const bl_octet *frame, uint16_t count)
{
    uint32_t start=boot_millis();
    CAN_sendMessage_updateDLC(CANA_BASE, 2, count, frame);
    while (CAN_getTxRequests(CANA_BASE)&2UL)
        if (boot_millis()-start > 500UL)
        {
            ++boot_debug_can_errors;
            CAN_disableMessageObject(CANA_BASE, 2);
            CAN_setupMessageObject(CANA_BASE, 2, BOOT_CAN_TX_ID, CAN_MSG_FRAME_STD,
                CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
            return 0;
        }
    return 1;
}
uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count)
{
    bl_octet frame[8];
    uint16_t offset, index=0, n, i;
    if (transport==1) return boot_sci_send(data,count);
    can_transfer=(can_transfer+1U)&255U;
    for (offset=0; offset<count; offset+=n)
    {
        n=count-offset>6 ? 6 : count-offset;
        frame[0]=can_transfer; frame[1]=index|(index ? 0U : 0x80U); ++index;
        for(i=0;i<n;++i) frame[i+2]=data[offset+i];
        if(!can_send(frame,n+2))return 0;
    }
    return 1;
}
uint16_t boot_take_request(void)
{
    volatile uint16_t *p=(volatile uint16_t *)BOOT_REQUEST_ADDR;
    uint16_t valid=p[0]==0xB007 && p[1]==0x4FF8 && p[2]==0x5AA5 && p[3]==0xA55A;
    p[0]=p[1]=p[2]=p[3]=0;
    return valid;
}
static void jump_image(uint32_t entry)
{
    DINT; IER=0; IFR=0;
    Interrupt_disablePIE();
    SCI_disableModule(SCIA_BASE);
    CAN_disableController(CANA_BASE);
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    SysCtl_disableWatchdog();
    ((void (*)(void))entry)(); /* codestart -> _c_int00 rebuilds C runtime. */
    for (;;) {}
}
void boot_enter_boot(void)
{
    volatile uint16_t *p=(volatile uint16_t *)BOOT_REQUEST_ADDR;
    p[0]=0xB007; p[1]=0x4FF8; p[2]=0x5AA5; p[3]=0xA55A;
    DEVICE_DELAY_US(20000);
    jump_image(BOOT_ENTRY_ADDR);
}
void boot_jump_app(void) { jump_image(BL_APP_START); }
void boot_reset(void) { SysCtl_resetDevice(); for (;;) {} }

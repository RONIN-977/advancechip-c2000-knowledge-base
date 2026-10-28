/**
 * @file main.c
 * @brief Bootloader 启动策略与通信调度。
 * 协议处理见 core/boot_core.c；Flash 操作见 flash_port.c；引脚见 board_config.h。
 */
#include "driverlib.h"
#include "device.h"
#include "board_config.h"
#include "boot_core.h"
#include "flash_port.h"
#include "boot_io.h"

static bl_context boot;
static bl_sci_rx sci;
static bl_can_rx can;
static bl_octet response[BL_MAX_PACKET];
/* owner: 0=无会话，1=SCI，2=CAN；同一升级会话只允许一个接口写入。 */
static uint16_t owner, stay_in_boot;
static uint32_t last_request;

/* 完整帧经协议层处理后回复；RESET 必须等应答发送完成再复位。 */
static void process(uint16_t transport, const bl_octet *data, uint16_t length)
{
    uint16_t size;
    if (boot.state == BL_RECEIVING && owner && owner != transport)
    {
        return;
    }
    size = bl_handle(&boot, data, length, response);
    if (!size)
    {
        return;
    }
    stay_in_boot = 1;
    if (boot.state == BL_RECEIVING)
    {
        owner = transport;
    }
    else
    {
        owner = 0;
    }
    last_request = boot_millis();
    if (boot_send_reply(transport, response, size) && data[3] == BL_RESET && response[14] == BL_OK)
    {
        DEVICE_DELAY_US(20000);
        SysCtl_resetDevice();
    }
}

/* 停止 Bootloader 外设后跳到应用 codestart，让 C 运行库初始化全局变量。
 * 不直接调用应用 main()；新增外设时应同步补充这里的退出清理。
 */
static void jump_to_app(void)
{
    DINT;
    IER = 0;
    IFR = 0;
    SCI_disableModule(BOOT_SCI_BASE);
    CAN_disableController(BOOT_CAN_BASE);
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    ((void (*)(void))BL_APP_START)(); /* codestart -> _c_int00, not main() */
    for (;;)
    {
    }
}

void main(void)
{
    uint32_t now, startup, sci_last = 0, can_last = 0, msg_id;
    uint16_t size, dlc;
    CAN_MsgFrameType frame_type;
    bl_octet frame[16]; /* Driver copies raw DLC before the protocol rejects DLC > 8. */
    DINT;
    /* 消费应用留下的 RAM 请求标记；命中时保持升级模式，不自动跳转。 */
    stay_in_boot = boot_take_request();
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Interrupt_register(INT_NMI, &boot_flash_nmi);
    boot_timers_init();
    bl_init(&boot, &boot_flash_ops);
    bl_sci_reset(&sci);
    bl_can_reset(&can);
    if (!boot_flash_init())
    {
        for (;;)
        {
        }
    }
    /* 只有元数据、镜像 CRC 和 ECC 均正确，才允许进入应用。 */
    if (boot_image_valid())
    {
        boot.state = BL_VALID;
    }
    boot_ports_init();
    startup = boot_millis();
    if (BOOT_FORCE_GPIO != 0xFFFFU && !GPIO_readPin(BOOT_FORCE_GPIO))
    {
        stay_in_boot = 1;
    }
    for (;;)
    {
        now = boot_millis();
        if (now - sci_last > BOOT_PARTIAL_MS)
        {
            bl_sci_reset(&sci);
        }
        if (now - can_last > BOOT_PARTIAL_MS)
        {
            bl_can_reset(&can);
        }
        if (SCI_getRxStatus(BOOT_SCI_BASE) & SCI_RXSTATUS_ERROR)
        {
            SCI_performSoftwareReset(BOOT_SCI_BASE);
            SCI_resetRxFIFO(BOOT_SCI_BASE);
            bl_sci_reset(&sci);
        }
        while (SCI_getRxFIFOStatus(BOOT_SCI_BASE) != SCI_FIFO_RX0)
        {
            sci_last = boot_millis();
            size = bl_sci_feed(&sci, SCI_readCharNonBlocking(BOOT_SCI_BASE) & 255U);
            if (size)
            {
                process(1, sci.data, size);
            }
        }
        if (CAN_readMessageWithID(BOOT_CAN_BASE, 1, &frame_type, &msg_id, frame))
        {
            dlc = HWREGH(BOOT_CAN_BASE + CAN_O_IF2MCTL) & CAN_IF2MCTL_DLC_M;
            if (frame_type == CAN_MSG_FRAME_STD && msg_id == BOOT_CAN_RX_ID)
            {
                can_last = boot_millis();
                size = bl_can_feed(&can, frame, dlc);
                if (size)
                {
                    process(2, can.data, size);
                }
            }
        }
        now = boot_millis();
        /* 中断的下载不能启动；释放接口所有权，等待主机重新开始升级。 */
        if (boot.state == BL_RECEIVING && now - last_request > BOOT_SESSION_MS)
        {
            boot.state = BL_FAILED;
            boot.cached_len = 0;
            owner = 0;
            bl_sci_reset(&sci);
            bl_can_reset(&can);
        }
        if (boot.state == BL_VALID && !stay_in_boot && now - startup >= BOOT_LISTEN_MS)
        {
            jump_to_app();
        }
    }
}

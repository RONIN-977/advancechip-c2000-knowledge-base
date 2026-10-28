#include "device.h"
#include "board_config.h"
#include "boot_io.h"
#include "update_service.h"

static bl_context context;
static bl_sci_rx sci;
static bl_can_rx can;
static bl_octet response[BL_MAX_PACKET];
static uint16_t owner = 0, app_mode = 0, stay = 0;
static uint32_t last_request = 0, sci_last = 0, can_last = 0;
volatile uint16_t boot_debug_stage = 0, boot_debug_last_rx = 0;
volatile uint32_t boot_debug_sci_bytes = 0, boot_debug_sci_packets = 0;
volatile uint32_t boot_debug_sci_replies = 0, boot_debug_tx_failures = 0;

void update_service_init(const bl_flash_ops *ops, uint16_t valid_image)
{
    static const bl_flash_ops no_flash = {0, 0, 0, 0, 0};
    app_mode = ops == 0;
    owner = stay = 0;
    sci_last = can_last = last_request = boot_millis();
    bl_init(&context, ops ? ops : &no_flash);
    if (valid_image) context.state = BL_VALID;
    bl_sci_reset(&sci); bl_can_reset(&can);
}

static void dispatch(uint16_t transport, const bl_octet *packet, uint16_t length)
{
    uint16_t size;
    if (app_mode && packet[3] != BL_ENTER_BOOT) return;
    if (context.state == BL_RECEIVING && owner && owner != transport) return;
    size = bl_handle(&context, packet, length, response);
    if (!size) return;
    owner = context.state == BL_RECEIVING ? transport : 0;
    stay = 1;
    last_request = boot_millis();
    if (!boot_send_reply(transport, response, size))
    { ++boot_debug_tx_failures; return; }
    if (transport == 1) ++boot_debug_sci_replies;
    if (response[14] != BL_OK) return;
    if (app_mode) boot_enter_boot();
    else if (packet[3] == BL_RESET)
    { DEVICE_DELAY_US(20000L); boot_reset(); }
}

void update_service_poll(void)
{
    uint32_t now = boot_millis();
    uint16_t result, size, dlc;
    bl_octet value, frame[8];
    if (now - sci_last > BOOT_PARTIAL_MS) bl_sci_reset(&sci);
    if (now - can_last > BOOT_PARTIAL_MS) bl_can_reset(&can);
    while ((result = boot_sci_read(&value)) != 0)
    {
        if (result == 2) { bl_sci_reset(&sci); break; }
        sci_last = boot_millis();
        boot_debug_last_rx = value; ++boot_debug_sci_bytes;
#ifdef BOOT_SCI_ECHO_TEST
        if (!boot_sci_send(&value, 1)) ++boot_debug_tx_failures;
#else
        size = bl_sci_feed(&sci, value);
        if (size) { ++boot_debug_sci_packets; dispatch(1, sci.data, size); }
#endif
    }
    if (boot_can_read(frame, &dlc))
    {
        can_last = boot_millis();
        size = bl_can_feed(&can, frame, dlc);
        if (size) dispatch(2, can.data, size);
    }
    if (context.state == BL_RECEIVING && boot_millis() - last_request > BOOT_SESSION_MS)
    {
        context.state = BL_FAILED; context.cached_len = 0; owner = 0;
        bl_sci_reset(&sci); bl_can_reset(&can);
    }
}
uint16_t update_service_keep_boot(void) { return stay; }
uint16_t update_service_image_valid(void) { return context.state == BL_VALID; }

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "boot_io.h"
#include "update_service.h"

static bl_octet rx[BL_MAX_PACKET], reply[BL_MAX_PACKET];
static uint16_t len, cursor, transport, frame_index, can_frames, replies, resets;
static int tx_ok = 1;
static uint16_t bad_session;
static uint32_t now;
uint32_t boot_millis(void) { return now; }
uint16_t boot_sci_read(bl_octet *v)
{ if (transport != 1 || cursor == len) return 0; *v = rx[cursor++]; return 1; }
uint16_t boot_can_read(bl_octet *frame, uint16_t *dlc)
{
    uint16_t count, i;
    if (transport != 2 || cursor == len) return 0;
    count = len - cursor > 6 ? 6 : len - cursor;
    frame[0] = 1; frame[1] = frame_index | (frame_index ? 0 : 0x80);
    for (i = 0; i < count; ++i) frame[i+2] = rx[cursor++];
    *dlc = count + 2; ++frame_index; ++can_frames; return 1;
}
uint16_t boot_send_reply(uint16_t channel, const bl_octet *data, uint16_t count)
{ assert(channel == transport); memcpy(reply,data,count*sizeof(*data)); ++replies; return tx_ok; }
uint16_t boot_sci_send(const bl_octet *p, uint16_t n) { (void)p; (void)n; return 1; }
void boot_enter_boot(void) { assert(replies && reply[14] == BL_OK && tx_ok); ++resets; }
void boot_reset(void) { ++resets; }
static void packet(uint16_t command, uint16_t bad_crc)
{
    uint32_t crc; uint16_t i;
    memset(rx,0,sizeof(rx)); len = 18; cursor = frame_index = 0;
    rx[0]=0xA5; rx[1]=0x5A; rx[2]=1; rx[3]=command;
    rx[4]=bad_session;
    crc=bl_crc32(rx+2,12);
    for(i=0;i<4;++i)rx[14+i]=(crc>>(8*i))&255;
    if(bad_crc)rx[17]^=1;
    while(cursor<len)update_service_poll();
}
int main(void)
{
    uint16_t channel;
    for(channel=1;channel<=2;++channel)
    {
        transport=channel; replies=resets=can_frames=0; tx_ok=1;
        update_service_init(0,0);
        packet(BL_ENTER_BOOT,1); assert(!resets && !replies);
        packet(BL_INFO,0); assert(!resets && !replies);
        bad_session=1; packet(BL_ENTER_BOOT,0);
        assert(!resets && replies==1 && reply[14]==BL_BAD_LENGTH);
        bad_session=0; replies=0;
        packet(BL_ENTER_BOOT,0); assert(resets==1 && replies==1);
        assert(reply[3]==(BL_ENTER_BOOT|0x80));
        if(channel==2)assert(can_frames>=3);
        update_service_init(0,0); replies=resets=0; tx_ok=0;
        packet(BL_ENTER_BOOT,0); assert(replies==1 && !resets);
        tx_ok=1; packet(BL_ENTER_BOOT,0); assert(resets==1);
        /* Model the destination BOOT initialization after APP handoff. */
        {
            static const bl_flash_ops boot_ops={0,0,0,0,0};
            update_service_init(&boot_ops,1); replies=resets=0;
            packet(BL_INFO,0);
            assert(replies==1 && !resets && reply[3]==(BL_INFO|0x80));
            assert(reply[12]==24 && update_service_keep_boot());
            /* A subsequent APP run must accept the next upgrade request. */
            update_service_init(0,0); replies=resets=0;
            packet(BL_ENTER_BOOT,0); assert(replies==1 && resets==1);
        }
    }
    puts("PASS APP SCI/CAN: invalid request rejected, ACK before BOOT handoff, TX retry, BOOT INFO and repeated upgrade entry");
    return 0;
}

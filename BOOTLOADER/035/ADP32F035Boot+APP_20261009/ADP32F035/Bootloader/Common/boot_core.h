#ifndef BOOT_CORE_H
#define BOOT_CORE_H
#include <stdint.h>
#include "partition_config.h"

/* One wire octet per uint16_t element: C28x CHAR_BIT is 16. Never memcpy a
 * packed host structure to wire. Length parameters count wire OCTETS. */
typedef uint16_t bl_octet;
#define BL_MAX_PAYLOAD 128U
#define BL_MAX_PACKET 146U
#define BL_DEVICE_ID 0x28003501UL
#define BL_BOARD_ID 0x55505301UL
#define BL_APP_START PART_APP_START
#define BL_APP_END PART_APP_END
#define BL_CPU 1U
#define BL_INFO 1U
#define BL_BEGIN 2U
#define BL_DATA 3U
#define BL_FINISH 4U
#define BL_ABORT 5U
#define BL_RESET 6U
#define BL_ENTER_BOOT 7U

enum bl_status
{
    BL_OK,
    BL_BAD_COMMAND,
    BL_BAD_LENGTH,
    BL_BAD_IMAGE,
    BL_BAD_STATE,
    BL_BAD_SEQUENCE,
    BL_FLASH_ERROR,
    BL_VERIFY_ERROR,
    BL_BUSY
};

enum bl_state
{
    BL_IDLE,
    BL_RECEIVING,
    BL_VALID,
    BL_FAILED
};

typedef struct
{
    uint32_t start;   /* 应用起始字地址，必须为 BL_APP_START。 */
    uint32_t count;   /* 镜像包含的 16 位字数，必须为 4 的倍数。 */
    uint32_t entry;   /* codestart 字地址；不能填写 main() 地址。 */
    uint32_t crc;     /* 镜像按低字节在前展开后的 CRC32。 */
    uint32_t version; /* 业务版本号，当前仅记录，不执行版本回退限制。 */
} bl_manifest;

/* Hardware adapter contract; ADP32F035 implementation is in Bootloader/flash_port.c.
 * Return nonzero ONLY on success. begin MUST invalidate previous boot metadata
 * durably before erasing any application sector. verify reads back programmed
 * flash and checks CRC/ECC; commit writes valid metadata last, then verifies it.
 * All flash-busy code/callees/constants and possible interrupt handlers must be
 * in RAM. The supplied synchronous AVP Flash2803x API must return before flash execution
 * resumes. No callback may erase boot/OTP/CSM regions. */
typedef struct
{
    void *user;
    uint16_t (*begin)(void *user, const bl_manifest *image);
    uint16_t (*program)(void *user, uint32_t address, const uint16_t *words, uint16_t count);
    uint16_t (*verify)(void *user, const bl_manifest *image);
    uint16_t (*commit)(void *user, const bl_manifest *image);
} bl_flash_ops;

typedef struct
{
    bl_flash_ops flash;
    bl_manifest image;
    uint32_t session;  /* 当前升级会话标识。 */
    uint32_t sequence; /* 最近接受的有状态请求序号。 */
    uint32_t next;     /* 下一次 DATA 应写入的字地址。 */
    uint16_t state;    /* enum bl_state。 */
    uint16_t cached_len;
    uint16_t reply_len;
    bl_octet cached[BL_MAX_PACKET]; /* 最近请求，用于识别重传。 */
    bl_octet reply[BL_MAX_PACKET];  /* 对应应答，重传时直接返回。 */
} bl_context;

void bl_init(bl_context *ctx, const bl_flash_ops *ops);
uint32_t bl_crc32(const bl_octet *data, uint16_t length);
/* out has BL_MAX_PACKET elements. CRC-invalid input is silently discarded.
 * RX and TX arrays must not alias. Caller serializes access from CAN and SCI;
 * one transport owns an active session. Boot policy is in target/main.c. */
uint16_t bl_handle(bl_context *ctx, const bl_octet *in, uint16_t length, bl_octet *out);

typedef struct
{
    bl_octet data[BL_MAX_PACKET];
    uint16_t length;
} bl_sci_rx;

void bl_sci_reset(bl_sci_rx *rx);
/* Returns complete CRC-valid packet size in rx->data, or zero. */
uint16_t bl_sci_feed(bl_sci_rx *rx, bl_octet value);

typedef struct
{
    bl_octet data[BL_MAX_PACKET];
    uint16_t length, active, transfer, next;
} bl_can_rx;

void bl_can_reset(bl_can_rx *rx);
uint16_t bl_can_feed(bl_can_rx *rx, const bl_octet *frame, uint16_t dlc);
/* Reset partial SCI/CAN RX on a monotonic 200 ms inter-frame/byte timeout. */
#endif

#ifndef UPGRADE_MD5_H
#define UPGRADE_MD5_H
#ifdef UPGRADE_HOST_TEST
#include <stdint.h>
typedef uint16_t Uint16;
typedef uint32_t Uint32;
#else
#include "AVP33x_Device.h"
#endif
typedef struct {
    Uint32 state[4];
    Uint32 bytes;
    Uint16 block[64]; /* Byte values in C28x 16-bit addressable cells. */
    Uint16 used;
} UPGRADE_MD5;
void UpgradeMd5Init(UPGRADE_MD5 *ctx);
void UpgradeMd5Byte(UPGRADE_MD5 *ctx, Uint16 value);
void UpgradeMd5Finish(UPGRADE_MD5 *ctx, Uint16 digest[16]);
#endif

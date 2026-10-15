#include "UpgradeMd5.h"
static const Uint32 constants[64] = {
    0xd76aa478UL,0xe8c7b756UL,0x242070dbUL,0xc1bdceeeUL,0xf57c0fafUL,0x4787c62aUL,0xa8304613UL,0xfd469501UL,
    0x698098d8UL,0x8b44f7afUL,0xffff5bb1UL,0x895cd7beUL,0x6b901122UL,0xfd987193UL,0xa679438eUL,0x49b40821UL,
    0xf61e2562UL,0xc040b340UL,0x265e5a51UL,0xe9b6c7aaUL,0xd62f105dUL,0x02441453UL,0xd8a1e681UL,0xe7d3fbc8UL,
    0x21e1cde6UL,0xc33707d6UL,0xf4d50d87UL,0x455a14edUL,0xa9e3e905UL,0xfcefa3f8UL,0x676f02d9UL,0x8d2a4c8aUL,
    0xfffa3942UL,0x8771f681UL,0x6d9d6122UL,0xfde5380cUL,0xa4beea44UL,0x4bdecfa9UL,0xf6bb4b60UL,0xbebfbc70UL,
    0x289b7ec6UL,0xeaa127faUL,0xd4ef3085UL,0x04881d05UL,0xd9d4d039UL,0xe6db99e5UL,0x1fa27cf8UL,0xc4ac5665UL,
    0xf4292244UL,0x432aff97UL,0xab9423a7UL,0xfc93a039UL,0x655b59c3UL,0x8f0ccc92UL,0xffeff47dUL,0x85845dd1UL,
    0x6fa87e4fUL,0xfe2ce6e0UL,0xa3014314UL,0x4e0811a1UL,0xf7537e82UL,0xbd3af235UL,0x2ad7d2bbUL,0xeb86d391UL
};
static const Uint16 shifts[16] = {7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21};
static void transform(UPGRADE_MD5 *ctx)
{
    Uint32 words[16], a=ctx->state[0], b=ctx->state[1], c=ctx->state[2], d=ctx->state[3], f, sum, next;
    Uint16 i, g, shift;
    for(i=0;i<16;i++) words[i]=(Uint32)ctx->block[4*i] | ((Uint32)ctx->block[4*i+1]<<8) |
        ((Uint32)ctx->block[4*i+2]<<16) | ((Uint32)ctx->block[4*i+3]<<24);
    for(i=0;i<64;i++) {
        if(i<16) { f=(b&c)|((~b)&d); g=i; }
        else if(i<32) { f=(d&b)|((~d)&c); g=(5*i+1)&15; }
        else if(i<48) { f=b^c^d; g=(3*i+5)&15; }
        else { f=c^(b|(~d)); g=(7*i)&15; }
        shift=shifts[(i/16)*4+(i&3)];
        sum=a+f+constants[i]+words[g];
        next=b+((sum<<shift)|(sum>>(32-shift)));
        a=d; d=c; c=b; b=next;
    }
    ctx->state[0]+=a; ctx->state[1]+=b; ctx->state[2]+=c; ctx->state[3]+=d;
}
void UpgradeMd5Init(UPGRADE_MD5 *ctx)
{
    ctx->state[0]=0x67452301UL; ctx->state[1]=0xefcdab89UL;
    ctx->state[2]=0x98badcfeUL; ctx->state[3]=0x10325476UL;
    ctx->bytes=0; ctx->used=0;
}
void UpgradeMd5Byte(UPGRADE_MD5 *ctx, Uint16 value)
{
    ctx->block[ctx->used++]=value&255; ctx->bytes++;
    if(ctx->used==64) { transform(ctx); ctx->used=0; }
}
void UpgradeMd5Finish(UPGRADE_MD5 *ctx, Uint16 digest[16])
{
    Uint32 bits=ctx->bytes<<3, high=ctx->bytes>>29;
    Uint16 i;
    UpgradeMd5Byte(ctx,128);
    while(ctx->used!=56) UpgradeMd5Byte(ctx,0);
    for(i=0;i<4;i++) UpgradeMd5Byte(ctx,(Uint16)(bits>>(8*i)));
    for(i=0;i<4;i++) UpgradeMd5Byte(ctx,(Uint16)(high>>(8*i)));
    for(i=0;i<16;i++) digest[i]=(Uint16)(ctx->state[i/4]>>(8*(i&3)))&255;
}

/*
 * Boo_ fil_ encryption.c
 *
 *  Created on: 2025年11月11日
 *      Author: 11033
 */
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include "DSP28x_Project.h"
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
extern void generateRandomArray(int size);
void secure_transform(int *Array_ptr);
// 定义指向0x9F00地址的指针
#pragma DATA_SECTION(Array_Boot, "myDataSection1")
int Array_Boot[6];
#pragma DATA_SECTION(Version_Boot, "myVersionSection1")
int Version_Boot=102;
#define ROUNDS 8  // 安全迭代轮次


void Boot_encryption(void)
{
    generateRandomArray(4);

    //=========Calculate two numbers using four random numbers============
    secure_transform(Array_Boot);
}
void secure_transform(int *Array_ptr)
{
    int i=0;

    // 初始化MD5派生常量（基于黄金分割率）
    const long K = 0x9E3779B9;  // MD5常量派生值[^1]          // 非线性初始化状态（引入输入交叉依赖）
    long state1=0,state2=0,temp=0;

    state1 = ((long)Array_ptr[0] << 16) | Array_ptr[1];
    state2 = ((long)Array_ptr[2] << 16) | Array_ptr[3];
    state1 ^= (state2 * K) + 0x76BCD3E;
    state2 ^= (state1 * K) + 0x5A82799;     // 多重混淆轮次

    for(i = 0; i < ROUNDS; i++)
    {         // 移位-异或扩散（参考MD5移位特性[^1]）
        state1 = (state1 << 7) | (state1 >> 25);
        state2 = (state2 << 12) | (state2 >> 20);          // 非线性变换（模乘加组合）
        state1 = (state1 * (K + i)) ^ state2;
        state2 = (state2 * (K - i)) ^ state1;              // 交叉混合
        temp = state1;
        state1 = state2;
        state2 = temp ^ (i * 0x1111F3A5);
    }     // 折叠输出（保持16位范围）

    Array_ptr[4] = (int)(state1 ^ (state1 >> 16));
    Array_ptr[5] = (int)(state2 ^ (state2 >> 16));
}
void generateRandomArray(int size)
{
    int i=0;
    unsigned int noiseValue=0;
    //srand((unsigned int)time(NULL));
    noiseValue = (AdcResult.ADCRESULT3>>1);
    srand(noiseValue);
    for(i = 0; i < size; i++)
    {
        Array_Boot[i] = rand();  // 生成随机数
    }
}






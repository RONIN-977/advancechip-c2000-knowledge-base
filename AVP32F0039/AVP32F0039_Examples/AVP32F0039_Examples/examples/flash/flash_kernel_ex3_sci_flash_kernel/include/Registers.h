/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   Registers.h
*                         TITLE:  F021 Flash API v1.58 
*******************************************************************************************/

/*!
    \file F021\Registers.h
    \brief A complete mapping of the F021 Flash Registers

    Allows named access to the F021 Flash Registers.
*/
#ifndef REGISTERS_H
#define REGISTERS_H

#include "Types.h"

/*  Flash Configuration Registers */
typedef volatile struct FMC_REGISTERS
{
    uint32 u32Register[300];
} Fapi_FmcRegistersType;

#endif /* REGISTERS_H */

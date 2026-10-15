/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   Init.h
*                         TITLE:  F021 Flash API v1.58 
*******************************************************************************************/

/*!
    \file F021\Init.h
    \brief 
*/
#ifndef FAPI_INIT_H_
#define FAPI_INIT_H_

typedef struct 
{
   Fapi_FmcRegistersType *m_poFlashControlRegisters;
   uint8 m_u8MainBankWidth;
   uint8 m_u8MainEccWidth;
   uint8 m_u8CurrentRwait;
   uint16 m_u16HclkFrequency;
} Fapi_InitStruct;

#endif /* FAPI_INIT_H_ */

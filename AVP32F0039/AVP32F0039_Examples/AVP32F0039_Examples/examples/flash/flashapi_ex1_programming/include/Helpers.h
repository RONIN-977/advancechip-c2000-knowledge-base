/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   Helpers.h
*                         TITLE:  F021 Flash API v1.58 
*******************************************************************************************/

/*!
    \file F021\Helpers.h
    \brief A set of helper Macros.

    These helper Macros are to facilitate common
    operations.
*/
#ifndef F021_HELPERS_H_
#define F021_HELPERS_H_

#define EI16(idx)   (idx ^ 1U)
#define EI8(idx)    (idx ^ 3U)

#ifndef EALLOW
	#define  EALLOW __eallow()
#endif

#ifndef EDIS
	#define  EDIS   __edis()
#endif

/*!
    \brief Quick Register referencing macro
*/
//#define REGISTER(mRegister) (* (volatile uint32 *)(mRegister))
#if defined(_C28X)
	#define FAPI_WRITE_LOCKED_FSM_REGISTER(mRegister,mValue)                                      \
		{                                                                                         \
			EALLOW;                                                                               \
			Fapi_GlobalInit.m_poFlashControlRegisters->FsmWrEna.FSM_WR_ENA_BITS.WR_ENA    = 0x5U; \
			mRegister = mValue;                                                                   \
			Fapi_GlobalInit.m_poFlashControlRegisters->FsmWrEna.FSM_WR_ENA_BITS.WR_ENA    = 0x2U; \
			EDIS;                                                                                 \
		}
#endif

//*****************************************************************************
//
// Extern compiler intrinsic prototypes. See compiler User's Guide for details.
//
//*****************************************************************************
extern void __eallow(void);
extern void __edis(void);

#endif /* F021_HELPERS_H_ */

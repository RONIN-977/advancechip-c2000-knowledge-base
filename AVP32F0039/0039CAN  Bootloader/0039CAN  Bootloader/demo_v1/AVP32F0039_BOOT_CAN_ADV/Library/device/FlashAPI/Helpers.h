/*--------------------------------------------------------*/
/* F021\Helpers.h                                         */
/* F021 Flash API v1.58                                   */
/*--------------------------------------------------------*/

// $TI Release: F28003x Support Library v5.00.00.00 $
// $Release Date: 11-17-2023 $
// $Copyright:
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $

/*!
    \file F021\Helpers.h
    \brief A set of helper Macros.

    These helper Macros are to facilitate common
    operations.
*/
#ifndef F021_HELPERS_H_
#define F021_HELPERS_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

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

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* F021_HELPERS_H_ */

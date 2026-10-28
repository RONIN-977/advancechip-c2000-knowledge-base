/*--------------------------------------------------------*/
/* F65\FlashTech.h                                            */
/* F65 Flash API v2.00                                   */
/*--------------------------------------------------------*/

// $TI Release: F280015x Support Library v5.05.00.00 $
// 
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
    \file F65\FlashTech.h
    \brief Main include file for F65 devices.
*/
#ifndef F65_H_
#define F65_H_

#include "Types.h"
#include "Registers.h"
#include "hw_flash_command.h"
#include "Constants/F280015x.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(F65)
   #define F65
#endif

/* Flash C API functions*/

/* Flash State Machine commands */
extern Fapi_FlashStatusType Fapi_getFsmStatus(void);

extern Fapi_StatusType Fapi_checkFsmForReady(void);

extern Fapi_StatusType Fapi_setActiveFlashBank(Fapi_FlashBankType oNewFlashBank);

extern Fapi_StatusType Fapi_issueFsmSuspendCommand(void);

extern void Fapi_flushPipeline(void);

extern Fapi_StatusType Fapi_setupBankSectorEnable(uint32_t reg_address, uint32_t value);

/* Device Family Dependent FSM commands */
extern uint32 * Fapi_remapEccAddress(uint32 u32EccAddress);

extern boolean Fapi_isAddressEcc(uint32 u32Address);

/* Async Commands */
extern Fapi_StatusType Fapi_issueAsyncCommandWithAddress(
                                                        Fapi_FlashStateCommandsType oCommand,
                                                        uint32 *pu32StartAddress
                                                        );

extern Fapi_StatusType Fapi_issueAsyncCommand(Fapi_FlashStateCommandsType oCommand);

extern Fapi_StatusType Fapi_issueBankEraseCommand(uint32 *pu32StartAddress);

/* Info Commands */
extern Fapi_LibraryInfoType Fapi_getLibraryInfo(void);

/* Init */
extern Fapi_StatusType Fapi_initializeAPI(
                                         Fapi_FmcRegistersType *poFlashControlRegister,
                                         uint32 u32HclkFrequency
                                         );

/* Read commands */
extern Fapi_StatusType Fapi_doBlankCheck(
                                        uint32 *pu32StartAddress,
                                        uint32 u32Length,
                                        Fapi_FlashStatusWordType *poFlashStatusWord
                                        );

extern Fapi_StatusType Fapi_doVerify(
                                    uint32 *pu32StartAddress,
                                    uint32 u32Length,
                                    uint32 *pu32CheckValueBuffer,
                                    Fapi_FlashStatusWordType *poFlashStatusWord
                                    );

extern Fapi_StatusType Fapi_doVerifyBy16bits(
                             uint16 *pu16StartAddress,
                             uint32 u16Length,
                             uint16 *pu16CheckValueBuffer,
                             Fapi_FlashStatusWordType *poFlashStatusWord
                             );

/* Programming Commands */
extern Fapi_StatusType Fapi_issueProgrammingCommand(
                                            uint32 *pu32StartAddress,
                                            uint16 *pu16DataBuffer,
                                            uint16  u16DataBufferSizeInWords,
                                            uint16 *pu16EccBuffer,
                                            uint16  u16EccBufferSizeInBytes,
                                            Fapi_FlashProgrammingCommandsType oMode
                                            );

extern Fapi_StatusType Fapi_issueDataOnly512ProgrammingCommand(
                                            uint32 *pu32StartAddress,
                                            uint16 *pu16DataBuffer,
                                            uint16  u16DataBufferSizeInWords
                                            );
											
extern Fapi_StatusType Fapi_issueAutoEcc512ProgrammingCommand(
                                            uint32 *pu32StartAddress,
                                            uint16 *pu16DataBuffer,
                                            uint16  u16DataBufferSizeInWords
                                            );											

extern Fapi_StatusType Fapi_issueDataAndEcc512ProgrammingCommand(
                                            uint32 *pu32StartAddress,
                                            uint16 *pu16DataBuffer,
                                            uint16  u16DataBufferSizeInWords,
                                            uint16 *pu16EccBuffer,
                                            uint16  u16EccBufferSizeInBytes
                                            );

extern Fapi_StatusType Fapi_issueEccOnly64ProgrammingCommand(
                                            uint32 *pu32StartAddress,
                                            uint16 *pu16EccBuffer,
                                            uint16  u16EccBufferSizeInBytes
                                            );

extern Fapi_StatusType Fapi_issueProgrammingCommandForEccAddresses(
                                                           uint32 *pu32StartAddress,
                                                           uint16 *pu16EccBuffer,
                                                           uint16 u16EccBufferSizeInBytes
                                                           );


/* Utility Commands */
extern uint32 Fapi_calculateFletcherChecksum(
                                            uint16 const * pu16Data,
                                            uint16 u16Length
                                            );

extern uint8 Fapi_calculateEcc(
                               uint32 u32Address,
                               uint64 u64Data
                               );

extern Fapi_StatusType Write_32bp(uint32_t reg_offset, uint32_t mask, uint32_t shift, uint32_t value);

extern Fapi_StatusType Write_32(uint32_t ctrlBase, uint32_t reg_offset, uint32_t mask, uint32_t shift, uint32_t value);

extern Fapi_StatusType Fapi_regReadback(uint32_t reg_offset, uint32_t readback_value);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /*F65_H_*/

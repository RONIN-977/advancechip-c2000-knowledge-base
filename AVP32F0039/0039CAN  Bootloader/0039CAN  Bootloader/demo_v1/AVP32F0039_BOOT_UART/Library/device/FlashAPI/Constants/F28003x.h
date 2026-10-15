/*--------------------------------------------------------*/
/* F021\Constants\F28003x.h                                */
/*                                                        */
/* Copyright (c) 2019-2020 Texas Instruments Incorporated */
/*                                                        */
/*--------------------------------------------------------*/

/*!
    \file F021\Constants\F28003x.h
    \brief A set of Constant Values for the F28003x Family.
*/
#ifndef F021_CONSTANTS_F28003x_H_
#define F021_CONSTANTS_F28003x_H_

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

/*!
    \brief Specifies the Offset to the TI OTP
*/

#define F021_PROGRAM_TIOTP_OFFSET    0x00070000U //TI OTP start on C28x



/* Final values to be determined */

/*!
    \brief Maximum FClck Value
	\This value is not used in API anymore
	\instead this value is programmed in TI OTP for API usage
*/
#define F021_FCLK_MAX    0xA 


/*!
    \brief PGM_OSU Max Value
*/
#define F021_PGM_OSU_MAX    0xFF

/*!
    \brief PGM_OSU Min Value
*/
#define F021_PGM_OSU_MIN    0x02

/*!
    \brief ERA_OSU Max Value
*/
#define F021_ERA_OSU_MAX    0xFF

/*!
    \brief ERA_OSU Min Value
*/
#define F021_ERA_OSU_MIN    0x02

/*!
    \brief ADD_EXZ Max Value
*/
#define F021_ADD_EXZ_MAX    0x0F

/*!
    \brief ADD_EXZ Min Value
*/
#define F021_ADD_EXZ_MIN    0x02

/*!
    \brief EXE_VALD Max Value
*/
#define F021_EXE_VALD_MAX    0x0F

/*!
    \brief EXE_VALD Min Value
*/
#define F021_EXE_VALD_MIN    0x02

/*!
    \brief PROG_PUL_WIDTH Max Value
*/
#define F021_PROG_PUL_WIDTH_MAX    0xFFFF

/*!
    \brief PROG_PUL_WIDTH Min Value
*/
#define F021_PROG_PUL_WIDTH_MIN    0x0002

/*!
    \brief ERA_PUL_WIDTH Max Value
*/
#define F021_ERA_PUL_WIDTH_MAX    0xFFFFFFFF

/*!
    \brief ERA_PUL_WIDTH Min Value
*/
#define F021_ERA_PUL_WIDTH_MIN    0x00000002

/*!
 *  FMC memory map defines
 */
#if defined (_F28003x) 
	#define F021_FLASH_MAP_BEGIN            (uint32)(0x80000)
	#define F021_FLASH_MAP_END              (uint32)(0xAFFFF)
    #define F021_TIOTP_MAP_BEGIN            (uint32)(0x70000)
    #define F021_TIOTP_MAP_END              (uint32)(0x70BFF)
	#define F021_OTP_MAP_BEGIN              (uint32)(0x78000) //Customer OTP start
	#define F021_OTP_MAP_END                (uint32)(0x78BFF) //Customer OTP End
    #define F021_TIOTPECC_MAP_BEGIN         (uint32)(0x01070000)
    #define F021_TIOTPECC_MAP_END           (uint32)(0x0107017F)
    #define F021_TIOTPECC_MAP_END_PLUS1     (uint32)(0x01070180)
	#define F021_OTPECC_MAP_BEGIN           (uint32)(0x1071000)
	#define F021_OTPECC_MAP_END             (uint32)(0x107117F)
    #define F021_OTPECC_MAP_END_PLUS1       (uint32)(0x1071180)
	#define F021_FLASHECC_MAP_BEGIN         (uint32)(0x1080000)
	#define F021_FLASHECC_MAP_END           (uint32)(0x1085FFF)
    #define F021_FLASHECC_MAP_END_PLUS1     (uint32)(0x1086000)
    #define BANK0_START                     (uint32)(0x80000)
    #define BANK1_START                     (uint32)(0x90000)
    #define BANK2_START                     (uint32)(0xA0000)
    #define BANK2_END_PLUS1                 (uint32)(0xB0000)
    #define BANK0_DCSMOTP_START             (uint32)(0x78000)
    #define BANK1_USEROTP_START             (uint32)(0x78400)
    #define BANK2_USEROTP_START             (uint32)(0x78800)
    #define BANK2_USEROTP_END_PLUS1         (uint32)(0x78C00)
	
	#define BANK0_MID                       (uint32)(0x88000)
	#define BANK1_MID                       (uint32)(0x98000)	
#endif

/*!
    \brief Define to map the direct access to the FMC registers.
*/
	#define F021_CPU0_REGISTER_ADDRESS 0x0005F800

/*!
 *  Specific TI OTP Offsets
 */
	#define F021_TIOTP_PER_BANK_SIZE 0x400U
	#define F021_TIOTP_SETTINGS_BASE 0xA8U

/* 32-bit CLKSRCCTL1 Register */
    #define  CLKSRCCTL1      (*(volatile uint32*)(0x5D208))

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* F021_CONSTANTS_F28003x_H_ */

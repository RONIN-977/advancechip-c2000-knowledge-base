/*--------------------------------------------------------*/
/* F65\Constants\F280015x.h                                */
/*                                                        */
/* Copyright (c) 2022-2023 Texas Instruments Incorporated */
/*                                                        */
/*--------------------------------------------------------*/

/*!
    \file F65\Constants\F280015x.h
    \brief A set of Constant Values for the F280015x Family.
*/
#ifndef CONSTANTS_F280015x_H_
#define CONSTANTS_F280015x_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 *  FMC memory map defines
 */
#if defined (_F280015x) 
	#define FLASH_MAP_BEGIN            (uint32)(0x80000)
	#define FLASH_MAP_END              (uint32)(0x9FFFF)
	#define OTP_MAP_BEGIN              (uint32)(0x78000) //Customer OTP start
	#define OTP_MAP_END                (uint32)(0x783FF) //Customer OTP End
	#define OTPECC_MAP_BEGIN           (uint32)(0x1071000)
	#define OTPECC_MAP_END             (uint32)(0x107107F)
    #define OTPECC_MAP_END_PLUS1       (uint32)(0x1071080)
	#define FLASHECC_MAP_BEGIN         (uint32)(0x1080000)
	#define FLASHECC_MAP_END           (uint32)(0x1083FFF)
    #define FLASHECC_MAP_END_PLUS1     (uint32)(0x1084000)
    #define BANK0_START                (uint32)(0x80000)
    #define BANK0_END_PLUS1            (uint32)(0xA0000)
    #define BANK0_DCSMOTP_START        (uint32)(0x78000)
    #define BANK0_DCSMOTP_END_PLUS1    (uint32)(0x78400)
	
#endif

/*!
    \brief Define to map the direct access to the FMC registers.
*/
	#define CPU0_REGISTER_ADDRESS    0x0005F800

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif /* CONSTANTS_F280015x_H_ */

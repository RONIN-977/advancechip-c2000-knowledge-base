/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   Constants.h 
*                         TITLE:  F021 Flash API v1.58 
*******************************************************************************************/


/*!
    \file F021\Constants\Constants.h
    \brief A set of Constant Values.
*/
#ifndef F021_CONSTANTS_H_
#define F021_CONSTANTS_H_

#include "F28003x.h"	


/*!
	\brief Define to map the direct access to the TI OTP memory.
*/
#define F021_TIOTP_BASE_ADDRESS ((Fapi_TiOtpBytesType *)(F021_PROGRAM_TIOTP_OFFSET + F021_TIOTP_SETTINGS_BASE))

/*!
	\brief Define to map the direct access to the FMC registers.
*/
#define F021_CPU0_BASE_ADDRESS ((Fapi_FmcRegistersType *)F021_CPU0_REGISTER_ADDRESS)
	


#endif /* F021_CONSTANTS_H_ */

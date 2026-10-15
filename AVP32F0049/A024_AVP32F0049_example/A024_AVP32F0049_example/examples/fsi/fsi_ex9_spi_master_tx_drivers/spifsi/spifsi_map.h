/******************************* Copyright(C) *********************************************
*
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   spifsi_map.h
*                         TITLE:  SPI to FSI communication user-defined function mapping
*******************************************************************************************/


#ifndef SPIFSI_MAP_H
#define SPIFSI_MAP_H

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


//
// Include
//
// User will include headers containing the mapped functions
// For C2000:
#include "spifsi_spi.h"

//*****************************************************************************
//
//!
//! @{
//
//*****************************************************************************

//
// Mapping
//
#define SPI_read16Bits            SPI_read16Bits
#define SPI_write16Bits           SPI_write16Bits
#define SPIFSI_calcCRC8           SPIFSI_calcCRC8

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus

#endif

#endif // SPIFSI_MAPPING_H

//
// End of File
//

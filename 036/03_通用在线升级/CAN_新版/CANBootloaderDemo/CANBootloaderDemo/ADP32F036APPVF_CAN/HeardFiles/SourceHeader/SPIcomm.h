//#############################################################################
//
// FILE:   SPIcomm.h
//
// TITLE:  SPI communication helper declarations.
//
// Description:
// This file declares the SPI initialization and transmit helpers used by the
// application.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef _SPI_CANCOMM_H_
#define _SPI_CANCOMM_H_

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern void InitSpi(void);
extern void InitSpiIO(void);
extern void SPISendData(void);

#endif /* _SPI_CANCOMM_H_ */

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

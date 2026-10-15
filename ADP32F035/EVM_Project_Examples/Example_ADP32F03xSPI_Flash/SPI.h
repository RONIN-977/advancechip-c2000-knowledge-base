#ifndef SPI_H_H_
#define SPI_H_H_

#include "ADP32F03x_Project.h"


void SPIA_MsterConfig(Uint16 spi_mode, Uint16 data_len);
void SPIB_MsterConfig(Uint16 spi_mode, Uint16 data_len);
Uint16 SPIA_WriteRead(Uint16 data);
Uint16 SPIB_WriteRead(Uint16 data);

Uint16 SPIA_WriteRead_8bit(Uint16 data);
Uint16 SPIA_Write_8bit(Uint16 data);
void SPIA_SpeedSet(Uint16 brr);
void SPIB_SpeedSet(Uint16 brr);

#endif


/******************************* Copyright(C) *********************************************
*
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   spifsi_spi.c
*                         TITLE:  SPI to FSI communication user-specific file
*******************************************************************************************/
#include "spifsi_spi.h"


//*****************************************************************************
//
// SPIFSI_calcCrc8 - calculate CRC8 by either using VCU or not.
//
//*****************************************************************************
#ifdef VCU_CRC
uint16_t SPIFSI_calcCRC8(uint32_t input_crc8_accum, uint16_t *msg, uint16_t parity, uint16_t rxLen) {
    return((uint16_t) getCRC8_vcu(input_crc8_accum, msg, (CRC_parity_e)parity, rxLen));
}
#else
uint16_t SPIFSI_calcCRC8(uint32_t input_crc8_accum, uint16_t *msg, uint16_t parity, uint16_t rxLen) {
    //
    // Build CRC struct.
    //
    CRC_Obj crcObj;
    crcObj.seedValue = input_crc8_accum;
    crcObj.numBytes = rxLen;
    crcObj.parity = (CRC_Parity) parity;
    crcObj.msgBuffer = msg;

    //
    // Point to CRC struct.
    //
    CRC_Handle hndCRC = &crcObj;

    //
    // Calculate.
    //
    CRC_calculate(hndCRC);

    return(hndCRC->crcResult);
}
#endif

//
// End of File
//



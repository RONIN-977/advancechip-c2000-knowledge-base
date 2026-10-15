/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   sdspi.c
*                         TITLE:  SD FATFS Library Example.
*******************************************************************************************/

/*
 *  ======== SDSPI.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <driverlib.h>
#include <device.h>

#include <sdspi.h>

void GPIO_init();
void SPI_init();

/* Definitions for MMC/SDC command */
#define CMD0                      (0x40+0)     /* GO_IDLE_STATE */
#define CMD1                      (0x40+1)     /* SEND_OP_COND */
#define CMD8                      (0x40+8)     /* SEND_IF_COND */
#define CMD9                      (0x40+9)     /* SEND_CSD */
#define CMD10                     (0x40+10)    /* SEND_CID */
#define CMD12                     (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16                     (0x40+16)    /* SET_BLOCKLEN */
#define CMD17                     (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18                     (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23                     (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24                     (0x40+24)    /* WRITE_BLOCK */
#define CMD25                     (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41                     (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55                     (0x40+55)    /* APP_CMD */
#define CMD58                     (0x40+58)    /* READ_OCR */
#define START_BLOCK_TOKEN         (0xFE)
#define START_MULTIBLOCK_TOKEN    (0xFC)
#define STOP_MULTIBLOCK_TOKEN     (0xFD)

#define SD_SECTOR_SIZE            (512)

#define DRIVE_NOT_MOUNTED         ((uint16_t) ~0)

int16_t SDSPI_control(SDSPI_Handle handle, uint16_t cmd,
    void *arg);

static inline void assertCS(uint16_t spiCsGpioIndex);
static inline void deassertCS(uint16_t spiCsGpioIndex);
static bool recvDataBlock(uint32_t spi_handle, void *buf, uint32_t count);
static uint8_t sendCmd(uint32_t spi_handle, uint8_t cmd, uint32_t arg);
static int16_t spiTransfer(uint32_t spi_handle, void *rxBuf,
    void *txBuf, size_t count);
static bool waitUntilReady(uint32_t spi_handle);
static bool transmitDataBlock(uint32_t spi_handle, void *buf, uint32_t count,
    uint8_t token);

void
SPI_pollingFIFOTransactionWithNullSupport(uint32_t base, uint16_t charLength,
                           uint16_t *pTxBuffer, uint16_t *pRxBuffer,
                           uint16_t numOfWords, uint16_t txDelay);

/*
 *  ======== SDSPI_close ========
 */
void SDSPI_close(SDSPI_Handle handle)
{
    if (handle->spiHandle) {
        SPI_disableModule(handle->spiHandle);
        handle->spiHandle = NULL;
    }


    handle->cardType = SD_NOCARD;
    handle->isOpen = false;
}

/*
 *  ======== SDSPI_control ========
 */
int16_t SDSPI_control(SDSPI_Handle handle, uint16_t cmd, void *arg)
{
    return (SD_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== SDSPI_getNumSectors ========
 */
uint32_t SDSPI_getNumSectors(SDSPI_Handle handle)
{
    uint8_t              n;
    uint8_t              csd[16];
    uint32_t             csize;
    uint32_t             sectors = 0;

    assertCS(handle->spiCsGpioIndex);

    /* Get number of sectors on the disk (uint32_t) */
    if ((sendCmd(handle->spiHandle, CMD9, 0) == 0) &&
        recvDataBlock(handle->spiHandle, csd, 16)) {
        /* SDC ver 2.00 */
        if ((csd[0] >> 6) == 1) {
            csize = csd[9] + (csd[8] << 8) + 1;
            sectors = (uint32_t)csize << 10;
        }
        /* MMC or SDC ver 1.XX */
        else {
            n = (csd[5] & 15) + ((csd[10] & 128) >> 7) +
                ((csd[9] & 3) << 1) + 2;

            csize = (csd[8] >> 6) + ((uint16_t) csd[7] << 2) +
                ((uint16_t) (csd[6] & 3) << 10) + 1;
            sectors = (uint32_t)csize << (n - 9);
        }
    }

    deassertCS(handle->spiCsGpioIndex);


    return (sectors);
}

/*
 *  ======== SDSPI_getSectorSize ========
 */
uint32_t SDSPI_getSectorSize(SDSPI_Handle handle)
{
    return (SD_SECTOR_SIZE);
}

/*
 *  ======== SDSPI_init ========
 */
void SDSPI_init(SDSPI_Handle handle)
{
    GPIO_init();
    SPI_init();
}

/*
 *  ======== SDSPI_initialize ========
 */
int16_t SDSPI_initialize(SDSPI_Handle handle)
{
    SD_CardType          cardType = SD_NOCARD;
    uint8_t              i;
    uint8_t              ocr[4];
    uint8_t              txDummy[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    int16_t              status;
    uint32_t             timeout;


    /*
     * The CS line should not be asserted when attempting to put the
     * SD card into SPI mode.
     */
    deassertCS(handle->spiCsGpioIndex);

    /*
     * To put the SD card in SPI mode we must keep the TX line high while
     * toggling the clock line several times. To do this we transmit 0xFF
     * 10 times. Do not assert CS during this time
     */
    status = spiTransfer(handle->spiHandle, NULL, &txDummy, 10);
    if (status != SD_STATUS_SUCCESS) {
        return (status);
    }

    /* Now select the SD Card's chip select to send CMD0 command */
    assertCS(handle->spiCsGpioIndex);

    /*
     * Send CMD0 to put the SD card in idle mode. Depending on the previous
     * state of the SD card, this may take up to a couple hundred milliseconds.
     * Rather than delay between attempts, we try up to 255 attempts.
     * Failure is returned if the card does not respond will a valid byte
     * within 255 attempts. When the card will respond with 0x1 when its
     * in idle mode.
     */
    for (i = 255, status = 0xFF; i > 0 && status != 0x1; i--) {
        status = sendCmd(handle->spiHandle, CMD0, 0);
    }

    /* If the card never transitioned into idle mode */
    if (status != 0x1) {
        deassertCS(handle->spiCsGpioIndex);
        return (SD_STATUS_ERROR);
    }

    /*
     * Proceed with initialization since the SD Card is in the idle state
     * Determine what SD Card version we are dealing with
     * Depending on which SD Card version, we need to send different SD
     * commands to the SD Card, which will have different response fields.
     */
    if (sendCmd(handle->spiHandle, CMD8, 0x1AA) == 1) {
        /* SD Version 2.0 or higher */
        status = spiTransfer(handle->spiHandle, &ocr, &txDummy, 4);
        if (status == SD_STATUS_SUCCESS) {
            /*
             * Ensure that the card's voltage range is valid
             * The card can work at VDD range of 2.7-3.6V
             */
            if ((ocr[2] == 0x01) && (ocr[3] == 0xAA)) {
                /*
                 * Wait for data packet in timeout of 300000 attempts - status used to
                 * indicate if a timeout occurred before operation
                 * completed.
                 */
                status = SD_STATUS_ERROR;
                timeout = 300000;
                do {
                    /* ACMD41 with HCS bit */
                    if ((sendCmd(handle->spiHandle, CMD55, 0) <= 1) &&
                        (sendCmd(handle->spiHandle, CMD41, 1UL << 30) == 0)) {
                        status = SD_STATUS_SUCCESS;
                        break;
                    }
                    timeout--;
                } while (timeout);

                /*
                 * Check CCS bit to determine which type of capacity we are
                 * dealing with
                 */
                if ((status == SD_STATUS_SUCCESS) &&
                    sendCmd(handle->spiHandle, CMD58, 0) == 0) {
                    status = spiTransfer(handle->spiHandle, &ocr, &txDummy, 4);
                    if (status == SD_STATUS_SUCCESS) {
                        cardType = (ocr[0] & 0x40) ? SD_SDHC : SD_SDSC;
                    }
                }
            }
        }
    }
    else {
        /* SDC Version 1 or MMC */
        /*
         * The card version is not SDC V2+ so check if we are dealing with a
         * SDC or MMC card
         */
        if ((sendCmd(handle->spiHandle, CMD55, 0) <= 1) &&
            (sendCmd(handle->spiHandle, CMD41, 0) <= 1)) {
            cardType = SD_SDSC;
        }
        else {
            cardType = SD_MMC;
        }

        /*
         * Wait for data packet in timeout of 1s - status used to
         * indicate if a timeout occurred before operation
         * completed.
         */
        status = SD_STATUS_ERROR;
        timeout = 300000;
        do {
            if (cardType == SD_SDSC) {
                /* ACMD41 */
                if ((sendCmd(handle->spiHandle, CMD55, 0) <= 1) &&
                    (sendCmd(handle->spiHandle, CMD41, 0) == 0)) {
                    status = SD_STATUS_SUCCESS;
                    break;
                }
            }
            else {
                /* CMD1 */
                if (sendCmd(handle->spiHandle, CMD1, 0) == 0) {
                    status = SD_STATUS_SUCCESS;
                    break;
                }
            }
            timeout--;
        } while (timeout);

        /* Select R/W block length */
        if ((status == SD_STATUS_ERROR) ||
            (sendCmd(handle->spiHandle, CMD16, SD_SECTOR_SIZE) != 0)) {
            cardType = SD_NOCARD;
        }
    }

    deassertCS(handle->spiCsGpioIndex);

    handle->cardType = cardType;

    /* Check to see if a card type was determined */
    if (cardType == SD_NOCARD) {
        status = SD_STATUS_ERROR;
    }
    else {
        /* Reconfigure the SPI to operate @ 2.5 MHz */

        // spiHandle initialization
        SPI_disableModule(handle->spiHandle);
        SPI_setConfig(handle->spiHandle, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA1,
                      SPI_MODE_CONTROLLER, 2500000, 8);
        SPI_enableFIFO(handle->spiHandle);
        SPI_disableLoopback(handle->spiHandle);
        SPI_setEmulationMode(handle->spiHandle, SPI_EMULATION_STOP_AFTER_TRANSMIT);
        SPI_enableModule(handle->spiHandle);

    }


    return (status);
}

/*
 *  ======== SDSPI_open ========
 */
SDSPI_Handle SDSPI_open(SDSPI_Handle handle)
{
    if (handle->isOpen) {
        return (NULL);
    }
    handle->isOpen = true;

    /* Configure the SPI CS pin as output set high */
    GPIO_setDirectionMode(handle->spiCsGpioIndex, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(handle->spiCsGpioIndex, GPIO_PIN_TYPE_STD);
    //
    // GPIO_setMasterCore(handle->spiCsGpioIndex, GPIO_CORE_CPU1);
    //
    GPIO_setQualificationMode(handle->spiCsGpioIndex, GPIO_QUAL_SYNC);
    GPIO_writePin(handle->spiCsGpioIndex, 1);

    /*
     * SPI is initially set to 400 kHz to perform SD initialization.  This is
     * is done to ensure compatibility with older SD cards.  Once the card has
     * been initialized (in SPI mode) the SPI peripheral will be closed &
     * reopened at 2.5 MHz.
     */

    //spiHandle initialization
    SPI_disableModule(handle->spiHandle);
    SPI_setConfig(handle->spiHandle, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA1,
                  SPI_MODE_CONTROLLER, 400000, 8);
    SPI_enableFIFO(handle->spiHandle);
    SPI_disableLoopback(handle->spiHandle);
    SPI_setEmulationMode(handle->spiHandle, SPI_EMULATION_STOP_AFTER_TRANSMIT);
    SPI_enableModule(handle->spiHandle);


    /* Ensure the CS line is de-asserted. */
    deassertCS(handle->spiCsGpioIndex);

    return (handle);
}

/*
 *  ======== SDSPI_read ========
 */
int16_t SDSPI_read(SDSPI_Handle handle, void *buf, int32_t sector,
    uint32_t sectorCount)
{
    uint8_t             ffByte = 0xFF;
    int16_t             status = SD_STATUS_ERROR;
    uint8_t             sendCmdResult = 0;
    bool                recvStatus = false;

    if (sectorCount == 0) {
        return (SD_STATUS_ERROR);
    }


    /*
     * On a SDSC card, the sector address is a byte address on the SD Card
     * On a SDHC card, the sector addressing is via sector blocks
     */
    if (handle->cardType != SD_SDHC) {
        /* Convert to byte address */
        sector *= SD_SECTOR_SIZE;
    }

    assertCS(handle->spiCsGpioIndex);

    /* Single block read */
    if (sectorCount == 1) {
        sendCmdResult = sendCmd(handle->spiHandle, CMD17, sector);
        recvStatus = recvDataBlock(handle->spiHandle, buf, SD_SECTOR_SIZE);
        if ((sendCmdResult == 0) &&
             recvStatus) {
            status = SD_STATUS_SUCCESS;
        }
        else
        {
            ESTOP0;
        }
    }
    /* Multiple block read */
    else {
        if (sendCmd(handle->spiHandle, CMD18, sector) == 0) {
            do {
                if (!recvDataBlock(handle->spiHandle, buf, SD_SECTOR_SIZE)) {
                    break;
                }
                buf = (void *) (((uint32_t) buf) + SD_SECTOR_SIZE);
            } while (--sectorCount);

            /*
             * STOP_TRANSMISSION - order is important; always want to send
             * stop signal
             */
            if (sendCmd(handle->spiHandle, CMD12, 0) == 0 && sectorCount == 0) {
                status = SD_STATUS_SUCCESS;
            }
            else
            {
                ESTOP0;
            }
        }
        else
        {
            ESTOP0;
        }
    }

    deassertCS(handle->spiCsGpioIndex);

    /* Send a 0xFF with CS high to try to put SD card into low power mode */
    spiTransfer(handle->spiHandle, NULL, &ffByte, 1);

    return (status);
}

/*
 *  ======== SDSPI_write ========
 */
int16_t SDSPI_write(SDSPI_Handle handle, const void *buf,
    int32_t sector, uint32_t sectorCount)
{
    int16_t        status = SD_STATUS_SUCCESS;

    if (sectorCount == 0) {
        return (SD_STATUS_ERROR);
    }


    /*
     * On a SDSC card, the sector address is a byte address on the SD Card
     * On a SDHC card, the sector addressing is via sector blocks
     */
    if (handle->cardType != SD_SDHC) {
        /* Convert to byte address if needed */
        sector *= SD_SECTOR_SIZE;
    }

    assertCS(handle->spiCsGpioIndex);

    /* Single block write */
    if (sectorCount == 1) {
        if ((sendCmd(handle->spiHandle, CMD24, sector) == 0) &&
            transmitDataBlock(handle->spiHandle, (void *) buf, SD_SECTOR_SIZE,
                START_BLOCK_TOKEN)) {
            sectorCount = 0;
        }
    }
    /* Multiple block write */
    else {
        if ((handle->cardType == SD_SDSC) || (handle->cardType == SD_SDHC)) {
            if (sendCmd(handle->spiHandle, CMD55, 0) != 0) {
                status = SD_STATUS_ERROR;
            }

            /* ACMD23 */
            if ((status == SD_STATUS_SUCCESS) &&
                (sendCmd(handle->spiHandle, CMD23, sectorCount) != 0)) {
                status = SD_STATUS_ERROR;
            }
        }

        /* WRITE_MULTIPLE_BLOCK command */
        if ((status == SD_STATUS_SUCCESS) &&
            (sendCmd(handle->spiHandle, CMD25, sector) == 0)) {
            do {
                if (!transmitDataBlock(handle->spiHandle, (void *) buf,
                    SD_SECTOR_SIZE, START_MULTIBLOCK_TOKEN)) {
                    break;
                }
                buf = (void *) (((uint32_t) buf) + SD_SECTOR_SIZE);
            } while (--sectorCount);

            /* STOP_TRAN token */
            if (!transmitDataBlock(handle->spiHandle, NULL, 0,
                STOP_MULTIBLOCK_TOKEN)) {
                sectorCount = 1;
            }
        }
    }

    /*
     * Wait for SD card to finish storing the data it received. This may help
     * the card go into low power mode.
     */
    waitUntilReady(handle->spiHandle);

    deassertCS(handle->spiCsGpioIndex);


    return ((sectorCount) ? SD_STATUS_ERROR : SD_STATUS_SUCCESS);
}

/*
 *  ======== assertCS ========
 */
static inline void assertCS(uint16_t spiCsGpioIndex)
{
    GPIO_writePin(spiCsGpioIndex, 0);
}

/*
 *  ======== deassertCS ========
 */
static inline void deassertCS(uint16_t spiCsGpioIndex)
{
    GPIO_writePin(spiCsGpioIndex, 1);
}

/*
 *  ======== recvDataBlock ========
 *  Function to receive a block of data from the SDCard
 */
static bool recvDataBlock(uint32_t spi_handle, void *buf, uint32_t count)
{
    uint8_t      rxBuf[2];
    uint8_t      txBuf[2] = {0xFF, 0xFF};
    int16_t      status;
    uint32_t     timeout;

    /*
     * Wait for SD card to be ready up to 300000 attempts.  SD card is ready when the
     * START_BLOCK_TOKEN is received.
     */
    timeout = 300000;
    do {
        status = spiTransfer(spi_handle, &rxBuf, &txBuf, 1);
        timeout--;
    } while ((status == SD_STATUS_SUCCESS) && (rxBuf[0] == 0xFF) &&
        timeout);

    if (rxBuf[0] != START_BLOCK_TOKEN) {
        /* Return error if valid data token was not received */
        return (false);
    }

    /* Receive the data block into buffer */
    if (spiTransfer(spi_handle, buf, NULL, count) != SD_STATUS_SUCCESS) {
        return (false);
    }

    /* Read the 16 bit CRC, but discard it */
    if (spiTransfer(spi_handle, &rxBuf, &txBuf, 2) != SD_STATUS_SUCCESS) {
        return (false);
    }

    /* Return with success */
    return (true);
}

/*
 *  ======== sendCmd ========
 *  Function to send a command to the SD card.  Command responses from
 *  SD card are returned.  (0xFF) is returned on failures.
 */
static uint8_t sendCmd(uint32_t spi_handle, uint8_t cmd, uint32_t arg)
{
    uint8_t      i;
    uint8_t      rxBuf;
    uint8_t      txBuf[6];
    int16_t status;

    if ((cmd != CMD0) && !waitUntilReady(spi_handle)) {
        return (0xFF);
    }

    /* Setup SPI transaction */
    txBuf[0] = cmd;                  /* Command */
    txBuf[1] = (uint8_t)(arg >> 24); /* Argument[31..24] */
    txBuf[2] = (uint8_t)(arg >> 16); /* Argument[23..16] */
    txBuf[3] = (uint8_t)(arg >> 8);  /* Argument[15..8] */
    txBuf[4] = (uint8_t) arg;        /* Argument[7..0] */

    if (cmd == CMD0) {
        /* CRC for CMD0(0) */
        txBuf[5] = 0x95;
    }
    else if (cmd == CMD8) {
        /* CRC for CMD8(0x1AA) */
        txBuf[5] = 0x87;
    }
    else {
        /* Default CRC should be at least 0x01 */
        txBuf[5] = 0x01;
    }

    if (spiTransfer(spi_handle, NULL, &txBuf, 6) != SD_STATUS_SUCCESS) {
        return (0xFF);
    }

    /* Prepare to receive SD card response (send 0xFF) */
    txBuf[0] = 0xFF;

    /*
     * CMD 12 has R1b response which transfers an additional
     * "busy" byte
     */
    if ((cmd == CMD12) &&
        (spiTransfer(spi_handle, &rxBuf, &txBuf, 1) != SD_STATUS_SUCCESS)) {
            return (0xFF);
    }

    /* Wait for a valid response; 10 attempts */
    i = 10;
    do {
        status = spiTransfer(spi_handle, &rxBuf, &txBuf, 1);
    } while ((status == SD_STATUS_SUCCESS) && (rxBuf & 0x80) && (--i));

    /* Return with the response value */
    return (rxBuf);
}

/*
 *  ======== spiTransfer ========
 *  Returns SD_STATUS_SUCCESS when transfer is completed;
 *  SD_STATUS_ERROR otherwise.
 */
static int16_t spiTransfer(uint32_t spi_handle, void *rxBuf,
    void *txBuf, size_t count) {

    SPI_pollingFIFOTransactionWithNullSupport(spi_handle, 8,
                               txBuf, rxBuf,
                               count, 0);

    return (0);
}


void
SPI_pollingFIFOTransactionWithNullSupport(uint32_t base, uint16_t charLength,
                           uint16_t *pTxBuffer, uint16_t *pRxBuffer,
                           uint16_t numOfWords, uint16_t txDelay)
{
    ASSERT((charLength >= 1U) && (charLength <= 16U));
    SPI_setcharLength(base, charLength);

    //
    // Reset the TX / RX FIFO buffers to default state
    //
    SPI_disableFIFO(base); // Disable FIFO register
    SPI_enableFIFO(base);  // Enable FIFO register

    //
    // Configure the FIFO Transmit Delay
    //
    SPI_setTxFifoTransmitDelay(base, txDelay);

    //
    // Determine the number of 16-level words from number of words to be
    // transmitted / received
    //
    uint16_t numOfSixteenWords = numOfWords / SPI_FIFO_TXFULL;

    //
    // Determine the number of remaining words from number of words to be
    // transmitted / received
    //
    uint16_t remainingWords = numOfWords % SPI_FIFO_TXFULL;

    uint16_t count = 0;
    uint16_t i = 0;
    uint16_t txBuffer_pos = 0;
    uint16_t rxBuffer_pos = 0;

    //
    // Number of transactions is based on numOfSixteenWords
    // Each transaction will transmit and receive 16 words.
    //
    while(count < numOfSixteenWords)
    {
        //
        // Fill-up the SPI Transmit FIFO buffers
        //
        for(i = 1; i <= SPI_FIFO_TXFULL; i++)
        {
            if (pTxBuffer == NULL)
            {
                SPI_writeDataBlockingFIFO(base, 0xFFFF << (16U - charLength));
            }
            else
            {
                SPI_writeDataBlockingFIFO(base, pTxBuffer[txBuffer_pos++] <<
                                          (16U - charLength));
            }
        }

        //
        // Wait till SPI Receive FIFO buffer is full
        //
        while(SPI_getRxFIFOStatus(base) < SPI_FIFO_RXFULL);

        //
        // Read the SPI Receive FIFO buffers
        //
        for(i = 1U; i <= SPI_FIFO_RXFULL; i++)
        {
            if(pRxBuffer == NULL)
            {
                SPI_readDataBlockingFIFO(base);
            }
            else
            {
                pRxBuffer[rxBuffer_pos++] = SPI_readDataBlockingFIFO(base);
            }
        }

        count++;
    }

    //
    // Number of transactions is based on remainingWords
    //
    for(i = 0U; i < remainingWords; i++)
    {
        if (pTxBuffer == NULL)
        {
            SPI_writeDataBlockingFIFO(base, 0xFFFF << (16U - charLength));
        }
        else
        {
            SPI_writeDataBlockingFIFO(base, pTxBuffer[txBuffer_pos++] <<
                                      (16U - charLength));
        }
    }

    //
    // Wait till SPI Receive FIFO buffer remaining words
    //
    while(SPI_getRxFIFOStatus(base) < remainingWords);

    //
    // Read the SPI Receive FIFO buffers
    //
    for(i = 0; i < remainingWords; i++)
    {
        if(pRxBuffer == NULL)
        {
            SPI_readDataBlockingFIFO(base);
        }
        else
        {
            pRxBuffer[rxBuffer_pos++] = SPI_readDataBlockingFIFO(base);
        }
    }

    //
    // Disable SPI FIFO
    //
    SPI_disableFIFO(base);
}

/*
 *  ======== transmitDataBlock ========
 *  Function to transmit a block of data to the SD card.  A valid command
 *  token must be sent to the SD card prior to sending the data block.
 *  The available tokens are:
 *      START_BLOCK_TOKEN
 *      START_MULTIBLOCK_TOKEN
 *      STOP_MULTIBLOCK_TOKEN
 */
static bool transmitDataBlock(uint32_t spi_handle, void *buf, uint32_t count,
    uint8_t token)
{
    uint8_t rxBuf;
    uint8_t txBuf[2] = {0xFF, 0xFF};

    if (!waitUntilReady(spi_handle)) {
        return (false);
    }

    /* transmit data token */
    txBuf[0] = token;
    if (spiTransfer(spi_handle, NULL, &txBuf, 1) != SD_STATUS_SUCCESS) {
        return (false);
    }

    /* Send data only when token != STOP_MULTIBLOCK_TOKEN */
    if (token != STOP_MULTIBLOCK_TOKEN) {
        /* Write data to the SD card */
        if (spiTransfer(spi_handle, NULL, buf, count) != SD_STATUS_SUCCESS) {
            return (false);
        }

        /* Receive the 16 bit CRC, but discard it */
        txBuf[0] = (0xFF);
        if (spiTransfer(spi_handle, NULL, &txBuf, 2) != SD_STATUS_SUCCESS) {
            return (false);
        }

        /* Receive data response token from SD card */
        if (spiTransfer(spi_handle, &rxBuf, &txBuf, 1) != SD_STATUS_SUCCESS) {
            return (false);
        }

        /* Check data response; return error if data was rejected  */
        if ((rxBuf & 0x1F) != 0x05) {
            return (false);
        }
    }

    return (true);
}

/*
 *  ======== waitUntilReady ========
 *  Function to check if the SD card is busy.
 *
 *  Returns true if SD card is ready; false indicates the SD card is still busy
 *  & a timeout occurred.
 */
static bool waitUntilReady(uint32_t spi_handle)
{
    uint8_t      rxDummy;
    uint8_t      txDummy = 0xFF;
    int16_t      status;
    uint32_t     timeout;

    /* Wait up to 1s for data packet */
    timeout = 300000;
    do {
        status = spiTransfer(spi_handle, &rxDummy, &txDummy, 1);
        timeout--;
    } while ((status == SD_STATUS_SUCCESS) && (rxDummy != 0xFF) &&
        timeout);

    return (rxDummy == 0xFF);
}

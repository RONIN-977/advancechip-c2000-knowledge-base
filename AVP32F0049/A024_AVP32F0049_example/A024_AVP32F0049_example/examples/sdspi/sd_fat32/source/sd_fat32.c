/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   sd_fat32.c
*                         TITLE:  SD FATFS Library Example.
*******************************************************************************************/
//! This example demonstrates how to use the FATFS library.
//!
//! \b External \b Connections \n
//!  - Connect the SPI signals identifed in the SysConfig to an SD CARD.
//!
//! \b Watch \b Variables \n
//!  - None.
//!

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

#include <sdspi.h>
#include <SDFatFS.h>

uint16_t SDFatFS_config_count = 1;
SDFatFS_Object sdfatfsObject;

SDSPI_Object sdspiObject = {
        .spiHandle = mySDCardSPI_BASE,
        .spiCsGpioIndex = mySDCardCS
};

SDFatFS_Object* SDFatFS_config [] = {&sdfatfsObject};

SDSPI_Handle sdspiHandle = &sdspiObject;


/* String conversion macro */
#define STR_(n)             #n
#define STR(n)              STR_(n)

/* Drive number used for FatFs */
#define DRIVE_NUM           0

const char inputfile[] = STR(DRIVE_NUM)":input.txt";
const char outputfile[] = STR(DRIVE_NUM)":output.txt";

FIL src;
FIL dst;

unsigned int filesize;
FRESULT fresult;

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board initialization
    //
    Board_init();

    SDFatFS_init();
    SDFatFS_Handle sdFatFs_handle = SDFatFS_open(sdspiHandle, DRIVE_NUM);
    if (sdFatFs_handle == NULL)
    {
        while(1);
    }


    /* Create a new file object for the file copy */
    fresult = f_open(&dst, outputfile, FA_CREATE_ALWAYS|FA_WRITE);
    if (fresult != FR_OK) {
        while(1);
    }


    fresult = f_sync(&dst);
    if (fresult != FR_OK) {
        while(1);
    }
    /* Get the filesize of the source file */
    filesize = f_size(&src);

    /* Close outputfile[] */
    fresult = f_close(&dst);
    if (fresult != FR_OK) {
        while(1);
    }

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    SDFatFS_close(sdFatFs_handle);
}


int32_t fatfs_getFatTime(void)
{
    return 0;
}
//
// End File
//

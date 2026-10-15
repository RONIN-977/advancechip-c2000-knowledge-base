/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   SDFatFS.c
*******************************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif

#include <SDFatFS.h>

#include <ff.h>

/* SDFatFS Specific Defines */
#define DRIVE_NOT_MOUNTED    (~(0U))

extern SDFatFS_Object* SDFatFS_config[];
extern uint16_t SDFatFS_config_count;

static bool isInitialized = false;

/*
 * Array of SDFatFS_Handles to determine the association of the
 * FatFs drive number with a SDFatFS_Handle.
 * FF_VOLUMES is defined in <third_party/fatfs/ffconf.h>.
 */
static SDFatFS_Handle sdFatFSHandles[FF_VOLUMES];

/* FatFS function prototypes */
DSTATUS SDFatFS_diskInitialize(BYTE drive);
DRESULT SDFatFS_diskIOctrl(BYTE drive, BYTE ctrl, void *buffer);
DRESULT SDFatFS_diskRead(BYTE drive, BYTE *buffer,
    DWORD sector, UINT secCount);
DSTATUS SDFatFS_diskStatus(BYTE drive);
DRESULT SDFatFS_diskWrite(BYTE drive, const BYTE *buffer,
    DWORD sector, UINT secCount);


void uart_log0(char * msg)
{
    // Implement this log if the library is not working
}

void uart_log1(char * msg, uint32_t arg1)
{
    // Implement this log if the library is not working
}

void uart_log2(char * msg, uint32_t arg1, uint32_t arg2)
{
    // Implement this log if the library is not working
}

/*
 *  ======== SDFatFS_close ========
 */
void SDFatFS_close(SDFatFS_Handle handle)
{
    TCHAR           path[3];
    DRESULT         dresult;
    FRESULT         fresult;
    SDFatFS_Object *obj = handle;

    /* Construct base directory path */
    path[0] = (TCHAR)'0' + obj->driveNum;
    path[1] = (TCHAR)':';
    path[2] = (TCHAR)'\0';

    /* Close the SD driver */
    SDSPI_close(obj->sdspiHandle);

    /* Unmount the FatFs drive */
    fresult = f_mount(NULL, path, 0);
    if (fresult != FR_OK) {
        uart_log1("SDFatFS: Could not unmount FatFs volume @ drive"
            " number %d", obj->driveNum);
    }

    /* Unregister the disk_*() functions */
    dresult = disk_unregister(obj->driveNum);
    if (dresult != RES_OK) {
        uart_log1("SDFatFS: Error unregistering disk"
            " functions @ drive number %d", obj->driveNum);
    }

    obj->driveNum = DRIVE_NOT_MOUNTED;
    uart_log0("SDFatFS closed");

    isInitialized = (bool) false;
}

/*
 *  ======== SDFatFS_diskInitialize ========
 */
DSTATUS SDFatFS_diskInitialize(BYTE drive)
{
    int_fast8_t     result;
    SDFatFS_Object *obj = sdFatFSHandles[drive];

    result = SDSPI_initialize(obj->sdspiHandle);

    /* Convert lower level driver status code */
    if (result == SD_STATUS_SUCCESS) {
        obj->diskState = ((DSTATUS) obj->diskState) & ~((DSTATUS)STA_NOINIT);
    }

    return (obj->diskState);
}

/*
 *  ======== SDFatFS_diskIOctrl ========
 *  Function to perform specified disk operations. This function is called by the
 *  FatFs module and must not be called by the application!
 */
DRESULT SDFatFS_diskIOctrl(BYTE drive, BYTE ctrl, void *buffer)
{
    SDFatFS_Object *obj   = sdFatFSHandles[drive];
    DRESULT         fatfsRes = RES_ERROR;

    switch (ctrl) {
        case CTRL_SYNC:
            fatfsRes = RES_OK;
            break;

        case (BYTE)GET_SECTOR_COUNT:
            *(uint32_t*)buffer = (uint32_t)SDSPI_getNumSectors(obj->sdspiHandle);

            uart_log1("SDFatFS: Disk IO control: sector count: %d",
                *(uint32_t*)buffer);
            fatfsRes = RES_OK;
            break;

        case (BYTE)GET_SECTOR_SIZE:
            *(WORD*)buffer = (WORD)SDSPI_getSectorSize(obj->sdspiHandle);
            uart_log1("SDFatFS: Disk IO control: sector size: %d",
                *(WORD*)buffer);
            fatfsRes = RES_OK;
            break;

        case (BYTE)GET_BLOCK_SIZE:
            *(WORD*)buffer = (WORD)SDSPI_getSectorSize(obj->sdspiHandle);
            uart_log1("SDFatFS: Disk IO control: block size: %d",
                *(WORD*)buffer);
            fatfsRes = RES_OK;
            break;

        default:
            uart_log0("SDFatFS: Disk IO control parameter error");
            fatfsRes = RES_PARERR;
            break;
    }
    return (fatfsRes);
}

/*
 *  ======== SDFatFS_diskRead ========
 *  Function to perform a disk read from the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 */
DRESULT SDFatFS_diskRead(BYTE drive, BYTE *buffer,
    DWORD sector, UINT secCount)
{
    int_fast32_t    result;
    DRESULT         fatfsRes = RES_ERROR;
    SDFatFS_Object *obj   = sdFatFSHandles[drive];

    /* Return if disk not initialized */
    if ((obj->diskState & (DSTATUS)STA_NOINIT) != 0) {
        fatfsRes = RES_PARERR;
    }
    else {
        result = SDSPI_read(obj->sdspiHandle, (uint8_t *)buffer,
            (int32_t)sector, (uint32_t)secCount);

        /* Convert lower level driver status code */
        if (result == SD_STATUS_SUCCESS) {
            fatfsRes = RES_OK;
        }
        else
        {
            ESTOP0;
        }
    }

    return (fatfsRes);
}

/*
 *  ======== SDFatFS_diskStatus ========
 *  Function to return the current disk status. This function is called by
 *  the FatFs module and must not be called by the application!
 */
DSTATUS SDFatFS_diskStatus(BYTE drive)
{
    return (((SDFatFS_Object *)sdFatFSHandles[drive])->diskState);
}


#if (_READONLY == 0)
/*
 *  ======== SDFatFS_diskWrite ========
 *  Function to perform a write to the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 */
DRESULT SDFatFS_diskWrite(BYTE drive, const BYTE *buffer, DWORD sector,
    UINT secCount)
{
    int_fast32_t    result;
    DRESULT         fatfsRes = RES_ERROR;
    SDFatFS_Object *obj = sdFatFSHandles[drive];

    /* Return if disk not initialized */
    if ((obj->diskState & (DSTATUS)STA_NOINIT) != 0) {
        fatfsRes = RES_PARERR;
    }
    else {
        result = SDSPI_write(obj->sdspiHandle, (const uint8_t *)buffer,
            (int32_t)sector, (uint32_t)secCount);

        /* Convert lower level driver status code */
        if (result == SD_STATUS_SUCCESS) {
            fatfsRes = RES_OK;
        }
    }

    return (fatfsRes);
}
#endif

/*
 *  ======== SDFatFS_init ========
 */
void SDFatFS_init(void)
{
    SDFatFS_Object *obj;
    uint16_t i = 0;

    if (!isInitialized) {
        isInitialized = (bool) true;
        for (i = 0; i < SDFatFS_config_count; i++)
        {
            /* Initialize each SDFatFS object */
            obj = ((SDFatFS_Handle)(SDFatFS_config[i]));

            obj->diskState = STA_NOINIT;
            obj->driveNum = DRIVE_NOT_MOUNTED;

            /* Initialize the SD Driver */
            SDSPI_init(obj->sdspiHandle);
        }
    }

}


/*
 *  ======== SDFatFS_open ========
 *  Note: The index passed into this function must correspond directly
 *  to the SD driver index.
 */
SDFatFS_Handle SDFatFS_open(SDSPI_Handle sdspiHandle, uint8_t drive)
{
    DRESULT         dresult;
    FRESULT         fresult;
    TCHAR           path[3];
    SDFatFS_Handle  handle = NULL;
    uint16_t        i = 0;
    /* Verify driver index and state */
    if (isInitialized) {
        for (i = 0; i < SDFatFS_config_count; i++)
        {
            /* Get handle for this driver instance */
            handle = (SDFatFS_Handle)(SDFatFS_config[i]);

            /* Determine if the device was already opened */
            if (handle->driveNum != DRIVE_NOT_MOUNTED) {
                uart_log1("SDFatFS Drive %d already in use!", handle->driveNum);
                handle = NULL;
            }
            else {
                handle->driveNum = drive;

                /* Open SD Driver */
                handle->sdspiHandle = SDSPI_open(sdspiHandle);

                if (handle->sdspiHandle == NULL) {
                    handle->driveNum = DRIVE_NOT_MOUNTED;
                    /* Error occurred in lower level driver */
                    handle = NULL;
                }
                else {

                    /* Register FATFS Functions */
                    dresult = disk_register(handle->driveNum,
                        SDFatFS_diskInitialize,
                        SDFatFS_diskStatus,
                        SDFatFS_diskRead,
                        SDFatFS_diskWrite,
                        SDFatFS_diskIOctrl);

                    /* Check for drive errors */
                    if (dresult != RES_OK) {
                        uart_log0("SDFatFS: Disk functions not registered");
                        SDFatFS_close(handle);
                        handle = NULL;
                    }
                    else {

                        /* Construct base directory path */
                        path[0] = (TCHAR)'0' + handle->driveNum;
                        path[1] = (TCHAR)':';
                        path[2] = (TCHAR)'\0';

                        /*
                         * Register the filesystem with FatFs. This operation does
                         * not access the SDCard yet.
                         */
                        fresult = f_mount(&(handle->filesystem), path, 0);
                        if (fresult != FR_OK) {
                            uart_log1("SDFatFS: Drive %d not mounted",
                                        handle->driveNum);

                            SDFatFS_close(handle);
                            handle = NULL;
                        }
                        else {

                            /*
                             * Store the new sdfatfs handle for the input drive
                             * number
                             */
                            sdFatFSHandles[handle->driveNum] = handle;

                            uart_log0("SDFatFS: opened");
                        }
                    }
                }
            }
        }
    }

    return (handle);
}

/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   diskio.c
*                         TITLE:  SD FATFS Library Example.
*******************************************************************************************/

#include <stddef.h>
#include <stdint.h>

#include "ffconf.h"
#include "diskio.h"

static diskio_fxns drive_fxn_table[FF_VOLUMES] = {
    {NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL, NULL}
};

extern int32_t fatfs_getFatTime(void);

/*
 * ======== disk_register ========
 */
DRESULT disk_register(
        BYTE drive,
        DSTATUS (*d_init)  (BYTE drive),
        DSTATUS (*d_status)(BYTE drive),
        DRESULT (*d_read)  (BYTE drive, BYTE *buf, DWORD sector, UINT num),
        DRESULT (*d_write) (BYTE drive, const BYTE *buf, DWORD sector, UINT num),
        DRESULT (*d_ioctl) (BYTE drive, BYTE cmd, void * buf))
{

    if (drive >= FF_VOLUMES) {
        return RES_PARERR;
    }

    drive_fxn_table[drive].d_init   = d_init;
    drive_fxn_table[drive].d_status = d_status;
    drive_fxn_table[drive].d_read   = d_read;
    drive_fxn_table[drive].d_write  = d_write;
    drive_fxn_table[drive].d_ioctl  = d_ioctl;

    return RES_OK;
}

/*
 * ======== disk_unregister ========
 */
DRESULT disk_unregister(BYTE drive)
{

    if (drive >= FF_VOLUMES) {
        return RES_PARERR;
    }

    drive_fxn_table[drive].d_init   = NULL;
    drive_fxn_table[drive].d_status = NULL;
    drive_fxn_table[drive].d_read   = NULL;
    drive_fxn_table[drive].d_write  = NULL;
    drive_fxn_table[drive].d_ioctl  = NULL;

    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    if (drive_fxn_table[pdrv].d_status == NULL) {
        return RES_PARERR;
    }
    else {
        /* call registered status function */
        return ((*(drive_fxn_table[pdrv].d_status)) (pdrv));
    }
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    if (drive_fxn_table[pdrv].d_init == NULL) {
        return RES_PARERR;
    }
    else {
        /* call registered init function */
        return ((*(drive_fxn_table[pdrv].d_init)) (pdrv));
    }
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    if (drive_fxn_table[pdrv].d_read == NULL) {
        return RES_PARERR;
    }
    else {
        /* call registered read function */
        return ((*(drive_fxn_table[pdrv].d_read)) (pdrv, buff, sector, count));
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
    if (drive_fxn_table[pdrv].d_write == NULL) {
        return RES_PARERR;
    }
    else {
        /* call registered write function */
        return ((*(drive_fxn_table[pdrv].d_write)) (pdrv, buff, sector, count));
    }
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    if (drive_fxn_table[pdrv].d_ioctl == NULL) {
        return RES_PARERR;
    }
    else {
        /* call registered write function */
        return ((*(drive_fxn_table[pdrv].d_ioctl)) (pdrv, cmd, buff));
    }
}

/*
 * ======== get_fattime ========
 */
DWORD get_fattime(void)
{
    /* call TI dummy implementation or user defined hook function */
    return (fatfs_getFatTime());
}

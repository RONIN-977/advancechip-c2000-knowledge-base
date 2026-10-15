/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   SDFatFS.h
*******************************************************************************************/

#ifndef ti_drivers_SDFatFS__include
#define ti_drivers_SDFatFS__include

#include <stdint.h>

#include <ff.h>
#include <diskio.h>
#include <sdspi.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief SDFatFS Object
 *  The application must not access any member variables of this structure!
 */
typedef struct {
    uint_fast32_t driveNum;
    DSTATUS       diskState;
    FATFS         filesystem; /* FATFS data object */
    SDSPI_Handle  sdspiHandle;
} SDFatFS_Object;

/*!
 *  @brief SDFatFS Global configuration
 *
 *  The #SDFatFS_Config structure contains a single pointer used to characterize
 *  the SDFatFS driver implementation.
 *
 *  This structure needs to be defined before calling SDFatFS_init() and it must
 *  not be changed thereafter.
 *
 *  @sa SDFatFS_init()
 */

/*!
 *  @brief A handle that is returned from a SDFatFS_open() call.
 */
typedef SDFatFS_Object *SDFatFS_Handle;

/*!
 *  @brief Function to open a SDFatFS instance on the specified drive.
 *
 *  Function to mount the FatFs filesystem and register the SDFatFS disk
 *  I/O functions with the FatFS module.
 *
 *  @param idx Logical peripheral number indexed into the HWAttrs
 *             table.
 *  @param drive Drive Number
 */
extern SDFatFS_Handle SDFatFS_open(SDSPI_Handle sdspiHandle, uint8_t drive);

/*!
 *  @brief Function to close a SDFatFS instance specified by the SDFatFS
 *         handle.
 *
 *         This function unmounts the file system mounted by SDFatFS_open() and
 *         unregisters the SDFatFS driver from the FatFs module.
 *
 *  @pre SDFatFS_open() had to be called first.
 *
 *  @param handle A #SDFatFS_Handle returned from SDFatFS_open()
 *
 *  @sa SDFatFS_open()
 */
extern void SDFatFS_close(SDFatFS_Handle handle);

/*!
 *  Function to initialize a SDFatFS instance
 */
extern void SDFatFS_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_SDFatFS__include */

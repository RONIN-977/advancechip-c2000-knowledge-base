/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   sdspi.h
*******************************************************************************************/


#ifndef ti_drivers_sd_SDSPI__include
#define ti_drivers_sd_SDSPI__include

#include <stdint.h>
#include <driverlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SD_NOCARD = 0, /*!< Unrecognized Card */
    SD_MMC = 1,    /*!< Multi-media Memory Card (MMC) */
    SD_SDSC = 2,   /*!< Standard SDCard (SDSC) */
    SD_SDHC = 3,    /*!< High Capacity SDCard (SDHC) */
} SD_CardType;


/**
 *  @defgroup SD_CONTROL SD_control command and status codes
 *  @{
 */

/*!
 * Common SD_control() command code reservation offset.
 * SD driver implementations should offset command codes with
 * SD_CMD_RESERVED growing positively.
 *
 * Example implementation specific command codes:
 * @code
 * #define SDXYZ_CMD_COMMAND0    (SD_CMD_RESERVED + 0)
 * #define SDXYZ_CMD_COMMAND1    (SD_CMD_RESERVED + 1)
 * @endcode
 */
#define SD_CMD_RESERVED    (32)

/*!
 * Common SD_control status code reservation offset.
 * SD driver implementations should offset status codes with
 * SD_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define SDXYZ_STATUS_ERROR0    (SD_STATUS_RESERVED - 0)
 * #define SDXYZ_STATUS_ERROR1    (SD_STATUS_RESERVED - 1)
 * #define SDXYZ_STATUS_ERROR2    (SD_STATUS_RESERVED - 2)
 * @endcode
 */
#define SD_STATUS_RESERVED    (-32)

/**
 *  @defgroup SD_STATUS Status Codes
 *  SD_STATUS_* macros are general status codes returned by SD_control()
 *  @{
 *  @ingroup SD_CONTROL
 */

/*!
 * @brief Successful status code returned by SD_control().
 *
 * SD_control() returns SD_STATUS_SUCCESS if the control code was executed
 * successfully.
 */
#define SD_STATUS_SUCCESS    (0)

/*!
 * @brief Generic error status code returned by SD_control().
 *
 * SD_control() returns SD_STATUS_ERROR if the control code
 * was not executed successfully.
 */
#define SD_STATUS_ERROR    (-1)

/*!
 * @brief   An error status code returned by SD_control() for
 * undefined command codes.
 *
 * SD_control() returns SD_STATUS_UNDEFINEDCMD if the
 * control code is not recognized by the driver implementation.
 */
#define SD_STATUS_UNDEFINEDCMD    (-2)
/** @}*/

/*!
 *  @brief  SDSPI Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct SDSPI_Object{
    uint32_t          spiHandle;
    SD_CardType       cardType;
    bool              isOpen;
    uint16_t          spiCsGpioIndex;
} SDSPI_Object;

typedef struct SDSPI_Object *SDSPI_Handle;


void SDSPI_close(SDSPI_Handle handle);
uint32_t SDSPI_getNumSectors(SDSPI_Handle handle);
uint32_t SDSPI_getSectorSize(SDSPI_Handle handle);
int16_t SDSPI_initialize(SDSPI_Handle handle);
void SDSPI_init(SDSPI_Handle handle);
SDSPI_Handle SDSPI_open(SDSPI_Handle handle);
int16_t SDSPI_read(SDSPI_Handle handle, void *buf,
    int32_t sector, uint32_t sectorCount);
int16_t SDSPI_write(SDSPI_Handle handle, const void *buf,
    int32_t sector, uint32_t sectorCount);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_sd_SDSPI__include */

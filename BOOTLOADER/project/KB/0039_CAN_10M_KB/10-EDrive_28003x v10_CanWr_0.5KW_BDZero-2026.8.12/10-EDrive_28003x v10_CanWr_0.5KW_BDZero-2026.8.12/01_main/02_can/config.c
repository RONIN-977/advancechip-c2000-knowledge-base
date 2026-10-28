/*******************************************************************************
 * config.c
 *
 * Modification: 
 *    Date     : 
 *    Revision :
 *    Author   :
 *    Contents :
 *******************************************************************************/



/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "osa.h"
#include "config.h"
//#include "crc16.h"
//#include "Platform_Types.h"
#include "flash_hal.h"
//#include "api_eeprom.h"


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* 配置控制结构体，大小为4字节 */
typedef struct configure_control_t
{
	u32 magicNumber;	/* 魔幻数，必须为`A00` */
} Configure_Control;


typedef struct configure_context_t
{
	Configure_Control control;
	Bootloader_Configuration config;
} Configure_Context;


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

Configure_Context s_configureContext;

u32 config_addr()
{
    return 0x085000U;
}
/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*******************************************************************************
* 函数名  : config_load
* 描  述  : 从配置区域中加载配置到内存中
* 输  入  : 无
* 输  出  : 无
* 返回值  : OSA_SOK  : 成功
*           OSA_EFAIL: 失败
*******************************************************************************/
int config_load(void)
{
    Configure_Context *pFConfig = (Configure_Context *)config_addr();
    Configure_Context *pContext = &s_configureContext;

    memcpy((uint8 *)pContext, (uint8 *)pFConfig, sizeof(Configure_Context)/2);

    if (pContext->control.magicNumber != BOOTLOADER_MAGIC)
    {
        memset((uint8 *)pContext, 0xFF, sizeof(Configure_Context));

        pContext->control.magicNumber = 0X3041;//0x454c;
        pContext->config.bootDirectFlags = BOOTLOADER_NON_PROG_REQ;
        //pContext->config.configCrc = crc16((uint8_t *)&pContext->config, CONFIG_DATA_SIZE - sizeof(uint32_t));

//        return E2PROM_write(0, sizeof(Configure_Context), (uint8_t *)pContext);
    }

    return OSA_SOK;
}



/*******************************************************************************
* 函数名  : config_set
* 描  述  : 设置参数到配置区域
* 输  入  : - pData      : 参数，指向Bootloader_Configuration结构的指针
*         : - commandSize: 参数大小，pData大小，必须为sizeof(Bootloader_Configuration)
*         : - commandMask: 参数命令掩码，支持多个参数一起配置，参考Bootloader_Command
* 输  出  : 无
* 返回值  : OSA_SOK  : 成功
*           OSA_EFAIL: 失败
*******************************************************************************/
int config_set(u8 * pData, u32 commandSize, u32 commandMask)
{
	Bootloader_Configuration *pUsrConfig = (Bootloader_Configuration *)pData;
	Configure_Context *pContext = &s_configureContext;
	Bootloader_Configuration *pConfig = NULL;

	if (pContext->control.magicNumber != 0X3041)//0x454c
	{
		return OSA_EFAIL;
	}

	pConfig = &pContext->config;

	if (BL_CONFIG_BOOT_FLAG & commandMask)
	{
		pConfig->bootDirectFlags = pUsrConfig->bootDirectFlags;
	}

	if (BL_CONFIG_CRC & commandMask)
	{
		pConfig->crcByteCount     = pUsrConfig->crcByteCount;
		pConfig->crcExpectedValue = pUsrConfig->crcExpectedValue;
		pConfig->crcStartAddress  = pUsrConfig->crcStartAddress;
	}

	if (BL_CONFIG_PROG_DATE & commandMask)
	{
		memcpy(pConfig->programmingDate, pUsrConfig->programmingDate, 4);
	}

	if (BL_CONFIG_FINGER_PRINT & commandMask)
	{
		memcpy(pConfig->appSoftwareFingerprint, pUsrConfig->appSoftwareFingerprint, 16);
	}

	if (BL_CONFIG_APP_VERSION & commandMask)
	{
		memcpy(pConfig->appSoftwareVersion, pUsrConfig->appSoftwareVersion, 10);
	}

	//pConfig->configCrc = crc16((uint8_t *)pConfig, CONFIG_DATA_SIZE - sizeof(uint32_t));

    flash_hal_erase(0x85000U,0x1000);
    flash_hal_write(0x85000U,sizeof(Configure_Context),(uint8 *)pContext);
    return 0;

//    if(flash_hal_erase(config_addr(),0x1000) != 0)
//    {
//        return  flash_hal_write(config_addr(),sizeof(Configure_Context),(uint8 *)pContext);
//    }
//    else
//    {
//        return 1;
//    }
//    return E2PROM_write(0, sizeof(Configure_Context), (uint8_t *)pContext);
}


/*******************************************************************************
* 函数名  : config_get
* 描  述  : 读EEPROM中的参数
* 输  入  : 无
* 输  出  : 无
* 返回值  : OSA_SOK  : 成功
*           OSA_EFAIL: 失败
*******************************************************************************/
int config_get(Bootloader_Configuration *pUsrConfig)
{
	Configure_Context *pContext = &s_configureContext;

	if (NULL == pUsrConfig)
	{
		return OSA_EINVAL;
	}

	memcpy(pUsrConfig, &pContext->config, sizeof(Bootloader_Configuration));
	
	return OSA_SOK;
}


/*******************************************************************************
* 函数名  : config_isDirectBoot
* 描  述  : 是否是直接启动模式
* 输  入  : 无
* 输  出  : 无
* 返回值  : true  : 直接启动应用
*           false : 不启动应用，等待升级
*******************************************************************************/
bool config_isDirectBoot(void)
{
	Configure_Context *pContext = &s_configureContext;

	return (pContext->config.bootDirectFlags == BOOTLOADER_PROG_REQ);
}





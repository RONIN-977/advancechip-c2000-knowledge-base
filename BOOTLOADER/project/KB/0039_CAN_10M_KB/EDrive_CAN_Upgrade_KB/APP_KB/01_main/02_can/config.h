/*******************************************************************************
 * config.h
 *
 * Modification: 
 *    Date     : 
 *    Revision :
 *    Author   :
 *    Contents :
 *******************************************************************************/



#ifndef _CONFIG_H_
#define _CONFIG_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

//#include <stdint.h>
#include <stdbool.h>
//#include "Platform_Types.h"
#include "DataTypeDef.h"
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* 0xAA代表进入升级模式 */
#define BOOTLOADER_PROG_REQ 0xAAU

/* 0x55代表直接启动APP */
#define BOOTLOADER_NON_PROG_REQ 0x55U

//#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
//
//#define BOOTLOADER_MAGIC FOUR_CHAR_CODE('L', 'E', 'A', 'P')
#define FOUR_CHAR_CODE(a, b, c) (((c) << 16) | ((b) << 8) | ((a)))

#define BOOTLOADER_MAGIC FOUR_CHAR_CODE('A', '0', '0')
/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* Bootloader配置字节命令 */
typedef enum bootloader_config_command_t
{
	/* 写升级标记 */
	BL_CONFIG_BOOT_FLAG = 0x1,

	/* 写CRC校验信息 */
	BL_CONFIG_CRC = 0x2,

	/* 写升级标记和CRC校验信息 */
	BL_CONFIG_BOOT_FLAG_CRC = 0x3,

	/* 写升级日期 */
	BL_CONFIG_PROG_DATE = 0x4,

	/* 写指纹信息 */
	BL_CONFIG_FINGER_PRINT = 0x8,

	/* 写APP版本信息 */
	BL_CONFIG_APP_VERSION = 0x10,

} Blconfig_Command;


/* Bootloader配置结构体，大小为124字节 */
typedef struct bootloader_configuration_data_t
{
	u32 crcStartAddress;	/*  CRC校验起始地址: APP起始地址 */
	u32 crcByteCount;		/*  CRC大小: APP程序大小 */
	u32 crcExpectedValue;	/*  CRC校验值: 由上位机提供，用于本地检查一致性 */
	u16 bootDirectFlags;	/*  是否直接启动, 0xAA代表进入升级模式，0x55代表直接启动APP */

	u8  appSoftwareVersion[10];		/* APP版本号，给UDS使用 */
	u8  programmingDate[4];			/* 升级日期，给UDS使用 */
	u8  appSoftwareFingerprint[16];	/* 指纹，给UDS使用 */
	u8  pad1[86];						/*  pad1: 预留，用于扩展 */
	u32 configCrc;						/* 配置区域的CRC校验值 */
} Bootloader_Configuration;


#define CONFIG_DATA_SIZE (sizeof(Bootloader_Configuration))




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
int config_load(void);


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
int config_set(u8 * pData, u32 commandSize, u32 commandMask);


/*******************************************************************************
* 函数名  : config_get
* 描  述  : 读EEPROM中的参数
* 输  入  : 无
* 输  出  : 无
* 返回值  : OSA_SOK  : 成功
*           OSA_EFAIL: 失败
*******************************************************************************/
int config_get(Bootloader_Configuration *pUsrConfig);


/*******************************************************************************
* 函数名  : config_isDirectBoot
* 描  述  : 是否是直接启动模式
* 输  入  : 无
* 输  出  : 无
* 返回值  : true  : 直接启动应用
*           false : 不启动应用，等待升级
*******************************************************************************/
bool config_isDirectBoot(void);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif



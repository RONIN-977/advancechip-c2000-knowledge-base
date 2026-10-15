/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   version.c
*                     TITLE:  API to return the version number of the driverlib.lib in use.
*******************************************************************************************/

#include "version.h"

//*****************************************************************************
//
// Version_getLibVersion
//
//*****************************************************************************
uint32_t
Version_getLibVersion(void)
{
    return(VERSION_NUMBER);
}

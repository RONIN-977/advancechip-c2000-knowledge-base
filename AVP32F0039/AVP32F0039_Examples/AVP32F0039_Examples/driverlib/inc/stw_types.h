/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   stw_types.h
*******************************************************************************************/

#ifndef STW_TYPES_H_
#define STW_TYPES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief  This macro defines the Boolean that is used for True.
 */
#define SUCCESS                                        ((uint32_t) 1U)

/**
 * \brief  This macro defines the Boolean that is used for False.
 */
#define FAIL                                           ((uint32_t) 0U)

#define INVALID_PARAM                                   (-((int32_t) 1))

/*
 * =========== Error codes returned by STW functions ===========
 */
#define STW_SOK                         ((int32_t) 0)
#define STW_EFAIL                       (-((int32_t) 1))
#define STW_EBADARGS                    (-((int32_t) 2))
#define STW_EINVALID_PARAMS             (-((int32_t) 3))
#define STW_ETIMEOUT                    (-((int32_t) 4))
#define STW_EOUT_OF_RANGE               (-((int32_t) 5))
#define STW_EUNSUPPORTED_CMD            (-((int32_t) 6))
#define STW_EUNSUPPORTED_OPS            (-((int32_t) 7))

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}

#endif

#endif


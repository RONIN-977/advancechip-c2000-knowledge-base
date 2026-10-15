/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   debug.h
*                         TITLE:  Assert definition macro for debug.
*******************************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

//*****************************************************************************
//
// Prototype for the function that is called when an invalid argument is passed
// to an API.  This is only used when doing a DEBUG build. It is the
// application's responsibility to define the __error__ function.
//
//*****************************************************************************
extern void __error__(const char *filename, uint32_t line);

//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#ifdef DEBUG
#ifdef __TMS320C28XX__
//
// When called from C28x application
//
#define ASSERT(expr) do                                                       \
                     {                                                        \
                         if(!(expr))                                          \
                         {                                                    \
                             __error__(__FILE__, __LINE__);                   \
                         }                                                    \
                     }                                                        \
                     while((_Bool)0)
#else
//
// When called from CLA application. Update as needed.
//
#define ASSERT(expr) do                                                       \
                     {                                                        \
                         if(!(expr))                                          \
                         {                                                    \
                             __mdebugstop();                                  \
                         }                                                    \
                     }                                                        \
                     while((_Bool)0)
#endif
#else
#define ASSERT(expr)
#endif

#endif // DEBUG_H

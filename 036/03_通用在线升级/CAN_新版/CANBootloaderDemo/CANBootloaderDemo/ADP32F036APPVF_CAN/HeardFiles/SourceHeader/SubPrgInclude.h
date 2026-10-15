//#############################################################################
//
// FILE:   SubPrgInclude.h
//
// TITLE:  Subprogram math helper declarations.
//
// Description:
// This file declares the low-level helper functions used by the application
// subprogram support layer.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef SUBPRG_INCLUDE_H
#define SUBPRG_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern Uint swap(Uint);				//
extern int 	abs(int);				//
extern int 	absl(long int);			//
extern int 	qsin(int);				//sin
extern int 	qatan(long int);		//atan
extern int  atan(int x, int y);		//
extern Uint qsqrt(Ulong);			//sqrt


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

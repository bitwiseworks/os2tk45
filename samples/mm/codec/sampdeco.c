/*****************************************************************************/
/*                                                                           */
/* SAMPDECO.C: Sample Codec Decompression/Display Routines.                  */
/*                                                                           */
/* Copyright (c) IBM Corporation 1991,1994             All Rights Reserved   */
/*                                                                           */
/*****************************************************************************/

#define  INCL_WIN
#define  INCL_GPI
#define  INCL_DOS
#define  INCL_OS2MM
#define  INCL_MMIO_CODEC
#include <os2.h>
#include <os2me.h>
#include <string.h>
#include "sampinst.h"
#include "sampdeco.h"



/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: DecompressFrame                                          */
/*     This procedure is the main decompression routine.  In this example,   */
/*     we'll simply copy the data to the destination buffer.                 */
/*                                                                           */
/*****************************************************************************/

ULONG DecompressFrame ( PMAIN_INST pMainInst, PBYTE pbSrc, PBYTE pbDst )
   {
   memcpy ( pbDst, pbSrc, pMainInst->ulWidth * pMainInst->ulHeight );
   return ( 0 );
   }

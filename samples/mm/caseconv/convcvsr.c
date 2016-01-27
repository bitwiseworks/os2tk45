/*static char *SCCSID = "@(#)convcvsr.c	13.2 92/02/15";*/
#pragma title ("CONVCVSR.C - Case Converter I/O Proc Conversion Routines")

/********************** START OF SPECIFICATIONS *****************************/
/*                                                                          */
/* SOURCE FILE NAME: CONVCVSR.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: CONV Procedure Conversion Support Routines.            */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*                  Copyright (c) IBM Corporation  1992, 1993               */
/*                         All Rights Reserved                              */
/*                                                                          */
/* STATUS: MMPM/2 Release 1.10                                              */
/*                                                                          */
/* FUNCTION: This module contains resource input routines for the           */
/*           Case Converter I/O Procedure.                                  */
/*                                                                          */
/* NOTES:                                                                   */
/*    DEPENDENCIES: NONE                                                    */
/*    RESTRICTIONS: Runs in protect mode only                               */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*                                                                          */
/*   ROUTINES: convhlpGetFormatStringLength                                 */
/*             convhlpGetFormatString                                       */
/*             convhlpGetNLSData                                            */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*                                                                          */
/*********************** END OF SPECIFICATIONS ******************************/

/*
 * Set up defines to include the necessary definitions from the OS/2
 * include files.
 */

#define INCL_NOPMAPI                   // no PM include files required
#define INCL_DOSRESOURCES
#define INCL_DOSMODULEMGR
#define INCL_CONVPROC                  // defines and prototypes for CONV

#include <os2.h>
#include <mmioos2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "convproc.h"

#pragma subtitle ("convhlpGetFormatStringLength - ")
#pragma page ()

/**************************************************************************
**   convhlpGetFormatStringLength                                        **
***************************************************************************
*
* ARGUMENTS:
*
*     fccIOProc         - Fourcc of the IOProc requested.
*     plNameLength      - Address where to return string length.
*
* RETURN:
*
*     Error code or 0.
*
* DESCRIPTION:
*
*     This function will retrieve the length of the format string
*     for the specified IOProc from the resource file that contains
*     the strings.
*
* GLOBAL VARS REFERENCED:
*
* GLOBAL VARS MODIFIED:
*
* NOTES:
*
* SIDE EFFECTS:
*
***************************************************************************/

ULONG APIENTRY convhlpGetFormatStringLength( FOURCC fccIOProc,
                                             PLONG plNameLength )
{
   HMODULE hModHandle;
   PVOID   pResourceData;
   CHAR    *pTemp;
   FOURCC  fccTemp;

   /*
    * Retrieve the data out of the resource, which is contained in the DLL.
    * Select the entry for the specified IOProc and return the string
    * length. Use the API call to get the DLL module handle from MMIO.
    */

   if (mmioQueryIOProcModuleHandle( (PMMIOPROC)&CONVProc,
                                    &hModHandle ))
      {
      return (MMIO_ERROR);
      }

   if (DosGetResource( hModHandle,
                       RT_RCDATA,
                       CONV_IOPROC_NAME_TABLE,
                       &pResourceData ))
      {
      return (MMIO_ERROR);
      }

   /*
    * The resource table is of the form : FOURCC String\0
    * Loop until a match is found, then return the length.
    */

   (*plNameLength) = 0L;
   pTemp = (CHAR *)pResourceData;

   while (pTemp)
      {
      memmove( &fccTemp, pTemp, sizeof(FOURCC) );

      if (fccTemp == 0L)
         {
         break;
         }

      if (fccTemp == fccIOProc)
         {
         pTemp += sizeof(FOURCC);

         if (pTemp == NULL)
            {
            break;
            }

         (*plNameLength) = strlen( pTemp );
         break;
         }

      pTemp += sizeof(FOURCC);

      if (pTemp == NULL)
         {
         break;
         }

      pTemp += (strlen( pTemp ) + 1);
      }

   DosFreeResource( pResourceData );

   return ((((*plNameLength) > 0L) ? MMIO_SUCCESS : MMIO_ERROR));
}

#pragma subtitle ("convhlpGetFormatString - ")
#pragma page ()

/**************************************************************************
**   convhlpGetFormatString                                              **
***************************************************************************
*
* ARGUMENTS:
*
*     fccIOProc         - Fourcc of the IOProc requested.
*     pszFormatString   - Address where to return string.
*     lBytes            - Number of bytes to copy.
*
* RETURN:
*
*     Error code or 0.
*
* DESCRIPTION:
*
*     This function will retrieve the format string for the specified
*     IOProc from the resource file that contains the strings.
*
* GLOBAL VARS REFERENCED:
*
* GLOBAL VARS MODIFIED:
*
* NOTES:
*
* SIDE EFFECTS:
*
***************************************************************************/

LONG APIENTRY convhlpGetFormatString( FOURCC fccIOProc,
                                      PSZ pszFormatString,
                                      LONG lBytes )
{
   HMODULE hModHandle;
   PVOID   pResourceData;
   CHAR    *pTemp;
   LONG    lSizeCopied;
   FOURCC  fccTemp;

   /*
    * Retrieve the data out of the resource, which is contained in the DLL.
    * Select the entry for the specified IOProc and return the string.
    */

   if (mmioQueryIOProcModuleHandle( (PMMIOPROC)&CONVProc,
                                    &hModHandle ))
      {
      return (MMIO_ERROR);
      }

   if (DosGetResource( hModHandle,
                       RT_RCDATA,
                       CONV_IOPROC_NAME_TABLE,
                       &pResourceData ))
      {
      return (MMIO_ERROR);
      }

   /*
    * The resource table is of the form : FOURCC String\0
    * Loop until a match is found, then return the string.
    * Copy up to the number of bytes specified.
    */

   lSizeCopied = 0L;
   pTemp = (CHAR *)pResourceData;

   while (pTemp)
      {
      memmove( &fccTemp, pTemp, sizeof(FOURCC) );

      if (fccTemp == 0L)
         {
         break;
         }

      if (fccTemp == fccIOProc)
         {
         pTemp += sizeof(FOURCC);

         if (pTemp == NULL)
            {
            break;
            }

         lSizeCopied = (LONG)(min( (LONG)strlen( pTemp ), lBytes ));
         strncpy( pszFormatString, pTemp, (ULONG)lSizeCopied );
         break;
         }

      pTemp += sizeof(FOURCC);

      if (pTemp == NULL)
         {
         break;
         }

      pTemp += (strlen( pTemp ) + 1);
      }

   DosFreeResource( pResourceData );

   return (lSizeCopied);
}

#pragma subtitle ("convhlpGetNLSData - ")
#pragma page ()

/**************************************************************************
**   convhlpGetNLSData                                                   **
***************************************************************************
*
* ARGUMENTS:
*
*     pulCodePage       - Address where to return the code page/country.
*     pulLanguage       - Address where to return the language/dialect.
*
* RETURN:
*
*     Error code or 0.
*
* DESCRIPTION:
*
*     This function will retrieve the NLS information for the IOProc
*     strings contained in the resource file.
*
* GLOBAL VARS REFERENCED:
*
* GLOBAL VARS MODIFIED:
*
* NOTES:
*
* SIDE EFFECTS:
*
***************************************************************************/

ULONG APIENTRY convhlpGetNLSData( PULONG pulCodePage,
                                  PULONG pulLanguage )
{
   HMODULE hModHandle;
   PVOID   pResourceData;
   CHAR    *pTemp;

   /*
    * Retrieve the data out of the resource, which is contained in the DLL.
    */

   if (mmioQueryIOProcModuleHandle( (PMMIOPROC)&CONVProc,
                                    &hModHandle ))
      {
      return (MMIO_ERROR);
      }

   if (DosGetResource( hModHandle,
                       RT_RCDATA,
                       CONV_NLS_CHARSET_INFO,
                       &pResourceData ))
      {
      return (MMIO_ERROR);
      }

   /*
    * The resource table is of the form :
    *   usCodePage     Low
    *   usCountryCode  High
    *   usLanguage     Low
    *   usDialect      High
    */

   pTemp = (CHAR *)pResourceData;

   while (pTemp)
      {
      memmove( pulCodePage, pTemp, sizeof(ULONG) );
      pTemp += sizeof(ULONG);

      if (pTemp == NULL)
         {
         break;
         }

      memmove( pulLanguage, pTemp, sizeof(ULONG) );

      break;
      }

   DosFreeResource( pResourceData );

   return (MMIO_SUCCESS);
}

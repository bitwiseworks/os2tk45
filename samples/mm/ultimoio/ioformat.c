/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOFORMAT.C                                          */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc format access module             */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains routines to get format info.     */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      IOProcGetFormatInfo                                               */
/*      IOProcGetFormatName                                               */
/*      ioGetFormatString                                                 */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/
#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <memory.h>

#define         INCL_DOS                        /* #define  INCL_DOSPROCESS.*/
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                         /* OS/2 headers.*/
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                       /* Multi-Media IO extensions.*/
#include        <ioi.h>


#define FORMAT_NAME  "UMB (UltiMotion Beta) IO Procedure"
#define NAME_LENGTH  sizeof(FORMAT_NAME)

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcGetFormatInfo                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Get Format Information                                 */
/*                                                                          */
/* FUNCTION: This function provides access pre-defined structure values.    */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcGetFormatInfo                                         */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMFORMATINFO  pmmformatinfo - Pointer to format info struct*/
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcGetFormatInfo ( PMMFORMATINFO pmmformatinfo )

{
   LONG         rc;                              /* Return code.            */

  /*
   * Validates parameters passed.
   */
  if (pmmformatinfo == NULL) {
     return(MMIOERR_INVALID_STRUCTURE);
     }

  if (CheckMem((PVOID)pmmformatinfo, sizeof(MMFORMATINFO), PAG_WRITE))
     return (MMIOERR_INVALID_STRUCTURE);

  /**************************************************************************/
  /* Copies IOProc_Entry information and capability flags to pmmformatinfo. */
  /**************************************************************************/

   pmmformatinfo->ulStructLen  = sizeof (MMFORMATINFO);
   pmmformatinfo->fccIOProc    = mmioFOURCC('U','L','I','O');
   pmmformatinfo->ulIOProcType = MMIO_IOPROC_FILEFORMAT;
   pmmformatinfo->ulMediaType  = MMIO_MEDIATYPE_MOVIE;
   pmmformatinfo->ulFlags      = VALID_FORMAT_FLAGS;
                                 
   memset( pmmformatinfo->szDefaultFormatExt, '\0',
          sizeof( pmmformatinfo->szDefaultFormatExt) );
   strcpy ((PSZ) pmmformatinfo->szDefaultFormatExt, "UMB");

   pmmformatinfo->ulCodePage   = MMIO_DEFAULT_CODE_PAGE;
   pmmformatinfo->ulLanguage   = MMIO_LC_US_ENGLISH;
   pmmformatinfo->lNameLength  = NAME_LENGTH;

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcGetFormatName                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Get Format Name.                                       */
/*                                                                          */
/* FUNCTION: This function provides access to the format name.              */
/*                                                                          */
/* NOTES: String is not NULL terminated.                                    */
/*                                                                          */
/* ENTRY POINT: IOProcGetFormatName                                         */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PSZ        pszNameBuf - Pointer to caller's text buffer.       */
/*           LONG       NameBufLen - Length of caller's text buffer.        */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              Length of format name string. (MaxLen)                      */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcGetFormatName ( PSZ  pszNameBuf,
                           LONG NameBufLen )

{
   LONG         rc;

   /*
    * Validates parameters passed.
    */
   if (pszNameBuf == NULL) {               /* Must have text buffer.  */
      return(0L);
      }

   if ((NameBufLen <= 0L) ||
       CheckMem((PVOID)pszNameBuf, NameBufLen, PAG_WRITE))
      return (0L);

   if (NameBufLen < NAME_LENGTH)
      return (0L);

   memcpy(pszNameBuf, FORMAT_NAME, NAME_LENGTH);

   return(NAME_LENGTH);
}



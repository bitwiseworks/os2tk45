/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOSEEK.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc routine for MMIOM_SEEK           */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the seek routine.                */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      IOProcSeek                                                        */
/*      ioSeekFile                                                        */
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


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  IOProcSeek                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Seek a requested position.                             */
/*                                                                          */
/* FUNCTION: This function calculate new value of file pointer index,       */
/*           clip file pointer index to reasonable value, return new        */
/*           value of file pointer index.                                   */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcSeek                                                  */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:     PMMIOINFO  pmmioinfo - pointer to MMIOINFO instance structure */
/*            LONG       lSeekValue - position of file to be seeked to.     */
/*            LONG       lSeekType  - position of file to be seeked from.   */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              lNewPosition                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*              Message specific error                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcSeek ( PMMIOINFO pmmioinfo,
                  LONG lSeekValue,
                  LONG lSeekType )
{
   LONG rc;
   PINSTANCE  pinstance;
   LONG  lNewPosition;


   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(rc);

   /***************************************************************************/
   /* determine seek position.                                                */
   /***************************************************************************/
   switch (lSeekType) {

      /**************************************/
      /* Seek offset from beginning of file.*/
      /**************************************/
      case SEEK_SET :
         lSeekValue = lSeekValue;
         break;

      /**************************************/
      /* Seek offset from current.          */
      /**************************************/
      case SEEK_CUR :
         lSeekValue += pinstance->lFileCurrentPosition;
         break;

      /**************************************/
      /* Seek to "end of file".             */
      /**************************************/
      case SEEK_END :
         lSeekValue = (LONG)(pinstance->ulFileLen - (ULONG)lSeekValue);
         break;

      /**************************************/
      /* Invalid seek type value.           */
      /**************************************/
      default :
         pmmioinfo->ulErrorRet = MMIOERR_INVALID_PARAMETER;
         return(MMIO_ERROR);

      } /* end switch */

   /***************************************************************************/
   /* Verify seek destination is not before beginning of file.                */
   /***************************************************************************/
   if (lSeekValue < 0L) {
      pmmioinfo->ulErrorRet = MMIOERR_SEEK_BEFORE_BEGINNING;
      return(MMIO_ERROR);
      }

   /***************************************************************************/
   /* Seek within a file                                                      */
   /***************************************************************************/
   if (pinstance->lCurrentTrack == MMIO_RESETTRACKS) {

      /************************************************************************/
      /* Translated Seek (Seek past header if seek position = 0)              */
      /*  else do:                                                            */
      /* Raw Seek (Seek to absolute position without regard for headers)      */
      /************************************************************************/
//----if (pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA) {       //TEMP BUG WORKAROUND!!!!!!!!!!!!!
         if (lSeekValue == 0L) {
            lNewPosition = ffSeekToDataBegin(pmmioinfo,pinstance);
            }
         else
            lNewPosition = ioSeekFile((PLONG)&pmmioinfo->ulErrorRet,
                                      pinstance->hmmioFileHandle,
                                      lSeekValue);
//-------}
//    else
//       lNewPosition = ioSeekFile(pmmioinfo,
//                                 pinstance->hmmioFileHandle,
//---------------------------------lSeekValue);
      }

   /***************************************************************************/
   /* Seek within a track                                                     */
   /***************************************************************************/
   else {
      lNewPosition = ffSeekTrack(pmmioinfo,
                                 pinstance->hmmioFileHandle,
                                 lSeekValue, lSeekType ); /* File format specific */
      }

   if (lNewPosition != MMIO_ERROR) {
      pinstance->lFileCurrentPosition = lNewPosition;
      pinstance->ulLastBufferOffset = 0L;
      pinstance->pLastBuffer = 0L;
      pinstance->ulDataInTempBuffer = 0L;
      }

   return(lNewPosition);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioSeekFile                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Seek within a file to absolute position.               */
/*                                                                          */
/* FUNCTION: This function seeks to an absolute position (raw seek)         */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioSeekFile                                                  */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:     PLONG      plRc      - Pointer to error return code           */
/*            HMMIO      hmmio     - mmio handle to file instance           */
/*            LONG       lSeekValue - Seek position                         */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              lNewPosition                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ioSeekFile ( PLONG plRc,
                  HMMIO hmmio,
                  LONG lSeekValue )

{
   LONG  lNewPosition;

   /***************************************************************************/
   /* Do actual Seek within a file                                            */
   /***************************************************************************/
   lNewPosition = mmioSeek(hmmio,lSeekValue,SEEK_SET);
   if ((lNewPosition <= MMIO_ERROR) || (lNewPosition != lSeekValue)) {
      *plRc = MMIOERR_SEEK_FAILED;
      return(MMIO_ERROR);
      }

   return(lNewPosition);
}

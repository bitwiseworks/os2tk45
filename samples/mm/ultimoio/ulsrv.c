/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULSRV.C                                             */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format Specific IO Proc service routines        */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains service routines for the IOProc. */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      ffSeekTrack                                                       */
/*      ffSeekToDataBegin                                                 */
/*      ffFindExtAudioTrack                                               */
/*      ffCleanUp                                                         */
/*                                                                        */
/************************** END OF SPECIFICATIONS *************************/

#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <memory.h>

#define         INCL_DOS                      /* #define  INCL_DOSPROCESS  */
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                       /* OS/2 headers              */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                     /* Multi-Media IO extensions */
#include        <hhpheap.h>
#include        <ioi.h>


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ffSeekTrack                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Do seek within a track of a file                       */
/*                                                                          */
/* FUNCTION: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: SeekTrack                                                   */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              CCCCCT     CCCC                                             */
/*              CCCCC      CCCCCCCC                                         */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              NULL (0L)                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffSeekTrack ( PMMIOINFO pmmioinfo,
                   HMMIO hmmio,
                   LONG lSeekValue,
                   LONG lSeekType )

{
   LONG   rc = MMIO_SUCCESS;

   /************************************************************************/
   /* Translated Seek (Seek past header if seek position = 0)              */
   /************************************************************************/
   if (pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA) {
      //
      //  ##BILL currently un-supported
      //
      }

   /************************************************************************/
   /* Raw Seek (Seek to absolute position without regard for headers)      */
   /************************************************************************/
   else {
      //
      //  ##BILL currently un-supported
      //
      }


  return(rc);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffSeekToDataBegin                                      */
/*                                                                          */
/* DESCRIPTIVE NAME: Seek to the beginning of the data (in file not track)  */
/*                                                                          */
/* FUNCTION: This function seeks to the beginning of the file data.         */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffSeekToDataBegin                                           */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:     PMMIOINFO  pmmioinfo - pointer to MMIOINFO instance structure */
/*            PINSTANCE  pinstance - pointer to instance structure          */
/*                                                                          */
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
LONG ffSeekToDataBegin ( PMMIOINFO pmmioinfo,
                         PINSTANCE pinstance)

{
   LONG            rc = MMIO_SUCCESS;                      /* Return code.         */
   LONG            lFilePosition;                          /* File position.       */
   MMCKINFO        mmckinfo;

   /* Check if we already know where data begins, if so, then no need to do descend again */
   if (pinstance->ulMisc1) {
      lFilePosition = ioSeekFile((PLONG)&pmmioinfo->ulErrorRet,pinstance->hmmioFileHandle,pinstance->ulMisc2);
      if (lFilePosition < MMIO_SUCCESS) {
         return(MMIO_ERROR);
         }
      }

   else {
      /* set file pointer to beginning of file.*/
      lFilePosition = ioSeekFile((PLONG)&pmmioinfo->ulErrorRet,pinstance->hmmioFileHandle,0L);
      if (lFilePosition < MMIO_SUCCESS) {
         return(MMIO_ERROR);
         }

      /* Descend into the RIFF chunk */
      memset (&mmckinfo, '\0', sizeof(MMCKINFO));
      mmckinfo.fccType = mmioStringToFOURCC(UMFORM_MOVIE ,0L);
      if (rc = mmioDescend(pinstance->hmmioFileHandle,
                           &mmckinfo,
                           NULL,
                           MMIO_FINDRIFF)) {
         pmmioinfo->ulErrorRet = rc;
         return (MMIO_ERROR);
         }

      /* Find the Data Frame List Chunk                                          */
      memset (&mmckinfo, '\0', sizeof(MMCKINFO));
      mmckinfo.fccType = mmioStringToFOURCC(UMFORM_FRAMEDATALIST,0L);
      if (rc = mmioDescend(pinstance->hmmioFileHandle,
                           &mmckinfo,
                           NULL,
                           MMIO_FINDLIST)) {
         pmmioinfo->ulErrorRet = rc;
         return (MMIO_ERROR);
         }

      lFilePosition = mmckinfo.ulDataOffset + SIZEOF_FORM;
      pinstance->ulMisc1 = lFilePosition + mmckinfo.ckSize - SIZEOF_FORM;  /* Save end of chunk list for read routine */
      pinstance->ulMisc2 = lFilePosition;      /* Save start of chunk list for read routine */
      }

   return(lFilePosition);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ffFindExtAudioTrack                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Find an external audio track and return ptr to tracki. */
/*                                                                          */
/* FUNCTION: Finds the first external audio file tracki structure.          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffFindExtAudioTrack                                         */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              CCCCCT     CCCC                                             */
/*              CCCCC      CCCCCCCC                                         */
/*                                                                          */
/* EXIT-NORMAL:  ptracki                                                    */
/*                                                                          */
/* EXIT-ERROR:  NULL (0L)                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
PTRACKI ffFindExtAudioTrack ( PINSTANCE pinstance,
                              ULONG ulNumTracks,
                              PTRACKMAP patrackmapList)

{
//   PTRACKI    ptracki;      /* Pointer to internal track structure */
//   ULONG      i;
//
//   /**************************************************************************/
//   /* For each track to see if there is a reference to an external audio     */
//   /* file track. AND make sure that at least one entry is provided for      */
//   /* in the trackmap table for the record information.                      */
//   /**************************************************************************/
//   for ( i = 0; i < ulNumTracks; i++) {
//      ptracki = ioFindTracki(pinstance,patrackmapList[i].ulTrackID);
//      if ((ptracki->hmmioTrackFileHandle) && (ptracki->ulNumEntries)) {
//         return(ptracki);
//         }
//      }
    return(NULL);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ffCleanUp                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Ultimotion specific clean up routine                   */
/*                                                                          */
/* FUNCTION: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffCleanUp                                                   */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              CCCCCT     CCCC                                             */
/*              CCCCC      CCCCCCCC                                         */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffCleanUp ( PINSTANCE pinstance )

{
//-PTRACKI         ptracki;                       /* Pointer to track list. */   /*OLD_EXTAUDIO*/
//                                                                               /*OLD_EXTAUDIO*/
///**************************************************************************/   /*OLD_EXTAUDIO*/
///* ptrackiList is a link list, CleanUp all external audio read buffers.   */   /*OLD_EXTAUDIO*/
///**************************************************************************/   /*OLD_EXTAUDIO*/
// while (pinstance->ptrackiList) {                                              /*OLD_EXTAUDIO*/
//    ptracki = pinstance->ptrackiList;                                          /*OLD_EXTAUDIO*/
//    if (ptracki) {                                                             /*OLD_EXTAUDIO*/
//       if (ptracki->ulMisc2) {                                                 /*OLD_EXTAUDIO*/
//          ENTERCRITX;                                                          /*OLD_EXTAUDIO*/ 
//          HhpFreeMem(hheap,(PVOID)ptracki->ulMisc2);                           /*OLD_EXTAUDIO*/
//          EXITCRIT;                                                            /*OLD_EXTAUDIO*/
//          ptracki->ulMisc2 = 0L;                                               /*OLD_EXTAUDIO*/
//          }                                                                    /*OLD_EXTAUDIO*/
//       pinstance->ptrackiList = ptracki->ptrackiNext;                          /*OLD_EXTAUDIO*/
//       }                                                                       /*OLD_EXTAUDIO*/
//----} /* end loop */                                                           /*OLD_EXTAUDIO*/

   return(MMIO_SUCCESS);
}

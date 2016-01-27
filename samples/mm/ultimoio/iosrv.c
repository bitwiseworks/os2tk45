/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOSRV.C                                             */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc service routines                 */
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
/*      ioGetPtrInstance                                                  */
/*      ioFindTracki                                                      */
/*      ioFindTrackMapEntry                                               */
/*      ioIdentifyStorageSystem                                           */
/*      ioInstanceInit                                                    */
/*      ioAddTracksToMovieHeader                                          */
/*      ioValidateRead                                                    */
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
/* SUBROUTINE NAME: ioGetPtrInstance                                        */
/*                                                                          */
/* DESCRIPTIVE NAME: Validate input parameters, get ptr to instance         */
/*                                                                          */
/* FUNCTION:                                                                */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioGetPtrInstance                                            */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   PMMIOINFO             pmmioinfo          - ptr to mmio info            */
/*   PINSTANCE             pinstance          - ptr to instance info        */
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

LONG ioGetPtrInstance ( PMMIOINFO pmmioinfo,
                        PINSTANCE *ppinstance)

{
   /*
    * Validates parameters passed.
    */
   if (pmmioinfo == NULL){
      return(MMIO_ERROR);
      }

//   if (CheckMem((PVOID)pmmioinfo, sizeof(MMIOINFO), PAG_WRITE))
//      return (MMIO_ERROR);

   if (pmmioinfo->hmmio == 0){
      pmmioinfo->ulErrorRet = MMIOERR_INVALID_HANDLE;
      return(MMIO_ERROR);
      }

   *ppinstance = (PINSTANCE)pmmioinfo->pExtraInfoStruct;

   if (*ppinstance == NULL) {
      pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
      return(MMIO_ERROR);
      }

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioFindTracki                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: Verify that the track number is a valid  movie track.  */
/*                                                                          */
/* FUNCTION: This function searches all known movie tracks for a match.     */
/*           and returns a ptr to movie track entry.                        */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioFindTracki                                                */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PINSTANCE      pinstance  - Ptr to instance structure.         */
/*           ULONG          ulTrackID - Track number to search for.         */
/*                                                                          */
/* EXIT-NORMAL:    Ptr to movie track entry                                 */
/*                                                                          */
/*                                                                          */
/* EXIT-ERROR:     NULL, not found                                          */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

PTRACKI ioFindTracki ( PINSTANCE pinstance,
                       ULONG ulTrackID )

{
   PTRACKI ptracki;

   for (ptracki = pinstance->ptrackiList; ptracki; ptracki = ptracki->ptrackiNext) {
      if (ptracki->ulTrackID == ulTrackID) {
          return(ptracki);
          }
      }

  return(NULL);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioFindTrackMapEntry                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Find Track Map Entry from Track list.                  */
/*                                                                          */
/* FUNCTION: This function matches Track from Track list.                   */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioFindTrackMapEntry                                         */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*                                                                          */
/*              PATRACKMAP     patrackmap                                   */
/*              ULONG          ulNumTracks                                  */
/*              ULONG          ulTrackID                                    */
/* EXIT-NORMAL:                                                             */
/*                                                                          */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

PTRACKMAP ioFindTrackMapEntry ( PTRACKMAP patrackmapList,
                                ULONG ulNumTracks,
                                ULONG ulTrackID)

{
   ULONG i;
   PTRACKMAP ptrackmap = patrackmapList;

   for(i=0; i < ulNumTracks; i++, ptrackmap++ ) {

      if (ptrackmap->ulTrackID == ulTrackID) {
         return(ptrackmap);
         }
      }

   return(NULL);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioIdentifyStorageSystem                                */
/*                                                                          */
/* DESCRIPTIVE NAME: IOProc identifies storage system.                      */
/*                                                                          */
/* FUNCTION: This function identifies the storage system for the current    */
/*           file.                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioIdentifyStorageSystem                                     */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*         PMMIOINFO pmmioinfo   - MMIOINFO structure.                      */
/*         PSZ       pszFileName - File name.                               */
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

LONG ioIdentifyStorageSystem ( PMMIOINFO pmmioinfo,
                               PSZ pszFileName )

{
   FOURCC  fccStorageSystem;                      /* fcc of SS I/O Proc.    */
   LONG    rc;                                    /* return code.           */

   fccStorageSystem = pmmioinfo->fccIOProc;

   if (!fccStorageSystem) {

      if (pszFileName == NULL) {
         fccStorageSystem = FOURCC_DOS;
         }
      else {
         rc = mmioIdentifyStorageSystem(pszFileName,
                                        pmmioinfo,
                                        &fccStorageSystem);
         if (rc != MMIO_SUCCESS) {
            return(rc);
            }
         }
      }
   pmmioinfo->fccIOProc = fccStorageSystem;

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioInstanceInit                                          */
/*                                                                          */
/* DESCRIPTIVE NAME: Initialize all fields.                                 */
/*                                                                          */
/* FUNCTION: Initialize all fields to NULL and zero.                        */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioInstanceInit                                              */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PINSTANCE    pinstance -- pointer to instance structure.    */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              (none)                                                      */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              (none)                                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

VOID ioInstanceInit (PINSTANCE pinstance)

{
   pinstance->pRawHdr              = NULL;
   pinstance->ulRawHdrLength       = 0L;
   pinstance->pmmhdr               = NULL;
   pinstance->ulMmHdrLength        = 0L;
   pinstance->ptrackiList          = NULL;
   pinstance->lLastTrack           = -1L;
   pinstance->lCurrentTrack        = -1L;
   pinstance->pccbList             = NULL;
   pinstance->hmmioFileHandle      = 0L;
   pinstance->lFileCurrentPosition = 0L;
   pinstance->ulFileLen            = 0L;
   pinstance->ulFlags              = 0L;
   pinstance->ulMode               = 0L;
   pinstance->pTempBuffer          = NULL;
   pinstance->ulTempBufferSize     = 0L;
   pinstance->ulDataInTempBuffer   = 0L;
   pinstance->pLastBuffer          = NULL;
   pinstance->ulLastBufferOffset   = 0L;
   pinstance->pccbLastUsed         = NULL;
   pinstance->ulMisc1              = 0L;
   pinstance->ulMisc2              = 0L;
   pinstance->ulMisc3              = 0L;
   pinstance->ulMisc4              = 0L;
   pinstance->ulDeltaCount         = 0;
   pinstance->ulCurrentFrame       = 0;
   pinstance->pmmVideoHdr          = NULL;
   pinstance->ulCurrentVideoTrack  = 0xFFFFFFFF;
   pinstance->ulCurrentAudioTrack  = 0xFFFFFFFF;
   return;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioAddTracksToMovieHeader                                */
/*                                                                          */
/* DESCRIPTIVE NAME: Updates tracks to track list in MMOVIEHEADER.          */
/*                                                                          */
/* FUNCTION: This function copies the current file's valid tracks to caller.*/
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioAddTracksToMovieHeader                                    */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*          PINSTANCE     pinstance    - IOProc instant structure.          */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO Error message.                                         */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG ioAddTracksToMovieHeader ( PINSTANCE pinstance )

{
   ULONG    i;                      /* Loop index.            */
   PTRACKI  ptracki;                /* Track list of pinstance. */

   /**************************************************************************/
   /* Copy track information into the movie header                           */
   /**************************************************************************/
   for (i = 0, ptracki = pinstance->ptrackiList;
        (i < pinstance->pmmhdr->ulNumEntries) && ptracki;
        i++, ptracki = ptracki->ptrackiNext ) {

      pinstance->pmmhdr->pmmTrackInfoList[i].ulTrackID   = ptracki->ulTrackID;
      pinstance->pmmhdr->pmmTrackInfoList[i].ulMediaType = ptracki->ulMediaType;
      pinstance->pmmhdr->pmmTrackInfoList[i].ulCountry   = 0L;                        /* TEMP TEMP */
      pinstance->pmmhdr->pmmTrackInfoList[i].ulCodePage  = 0L;                        /* TEMP TEMP */
      }

   if (i > pinstance->pmmhdr->ulNumEntries)
      return(MMIOERR_INVALID_BUFFER_LENGTH);

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioValidateRead                                          */
/*                                                                          */
/* DESCRIPTIVE NAME: Validate parameters passed from user.                  */
/*                                                                          */
/* FUNCTION:                                                                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ioValidateRead                                              */
/*   LINKAGE:   CALL Near (00:32)                                           */
/*                                                                          */
/* INPUT:       PINSTANCE              pinstance                            */
/*              PVOID                  pReadBuffer                          */
/*              ULONG                  ulNumTracks                          */
/*              PTRACKMAP              patrackmapList                       */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO error return code                                      */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG ioValidateRead ( PINSTANCE pinstance,
                      PVOID pReadBuffer,
                      ULONG ulNumTracks,
                      PTRACKMAP patrackmapList)

{
   PTRACKI    ptracki;                         /* Ptr to ptracki entry */
   ULONG      i;
   ULONG      j;


   /**************************************************************************/
   /* For each track to read, validate that it is a valid movie track.       */
   /* And for each valid track, save the number of entries and reset the     */
   /* srcbuftab entries. Also init each srcbuftab entry.                     */
   /**************************************************************************/
   for ( i = 0; i < ulNumTracks; i++) {

      if (ptracki = ioFindTracki(pinstance,patrackmapList[i].ulTrackID)) {
         ptracki->ulNumEntries = patrackmapList[i].ulNumEntries;
         patrackmapList[i].ulNumEntries = 0;

        /***************************************************************/
        /* initialize the pasrcbuftabs' pBuffers and pRecords.         */
        /***************************************************************/
         for ( j = 0; j < ptracki->ulNumEntries; j++) {
             patrackmapList[i].pRecordTabList[j].pRecord  = (PVOID)NULL;
             patrackmapList[i].pRecordTabList[j].ulLength = (ULONG)0L;
             }
         }

      else {
         return(MMIOERR_READ_FAILED);
         }
      }

   if (pinstance->lFileCurrentPosition >= pinstance->ulFileLen) {
      return(MMIOERR_EOF_SEEN);
      }

   if (pReadBuffer == NULL) {
       return(MMIOERR_NO_BUFFER_ALLOCATED);
       }

   pinstance->pLastBuffer = pReadBuffer; /* set this ptr for read_notdone used.  */
   return (MMIO_SUCCESS);
}

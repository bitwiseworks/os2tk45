/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOOPEN.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc routine for MMIOM_OPEN.          */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the open routine, and other      */
/*           routines that support IOProcOpen.                            */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*     IOProcOpen                                                         */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/

#define         INCL_DOS
#define         INCL_DOSPROCESS
#define         INCL_DOSDATETIME
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                         /* OS/2 headers.            */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                      /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#define         INCL_DBCSCRT
#include        <mmiocrt.h>
#include        <ioi.h>

   extern HHUGEHEAP hheap;
   extern HMTX hmtxGlobalHeap;

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  IOProcOpen                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: open Digital Video file.                               */
/*                                                                          */
/* FUNCTION: This function opens digital video file, allocates instance     */
/*           structure, reads header, calls file format specific open       */
/*           routine to init the track information.                         */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcOpen                                                  */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMIOINFO pmmioinfo - Pointer to MMIOINFO staus structure.  */
/*              PSZ     pszFileName - Name of file to be opened.            */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*              MMIOERR_INVALID_ACCESS_FLAG                                 */
/*              MMIOERR_OUTOFMEMORY                                         */
/*              io proc specific error                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG IOProcOpen (PMMIOINFO pmmioinfo, PSZ pszFileName) {
   LONG            rc = MMIO_SUCCESS, openType = -1;
   LONG            lFilePosition;         // Logical file position
   MMIOINFO        Localmmioinfo;         // Locally used
   PINSTANCE       pinstance;
   PTIB            ptib;                  // thread info block
   PPIB            ppib;                  // process info block

   if (pmmioinfo == NULL) return MMIO_ERROR;

   if (CheckMem((PVOID)pmmioinfo, sizeof(MMIOINFO), PAG_WRITE))
      return MMIO_ERROR;

   // Validate the open flags for this File Format IO Proc
   // INVALID_OPEN_FLAGS must be defined in the ff.h - file format
   // specific header file

   if (pmmioinfo->ulFlags  & INVALID_OPEN_FLAGS) {
       pmmioinfo->ulErrorRet = MMIOERR_INVALID_ACCESS_FLAG;
       return MMIOERR_INVALID_ACCESS_FLAG;
//     return MMIO_ERROR;
   }

   // Allocate local instance structure

   ENTERCRITX;
   if ((pinstance = (PINSTANCE)HhpAllocMem(hheap,sizeof(INSTANCE))) == NULL) {
       EXITCRIT;
       pmmioinfo->ulErrorRet = MMIOERR_OUTOFMEMORY;
       return MMIOERR_OUTOFMEMORY;
//     return MMIO_ERROR;
   }
   EXITCRIT;

   pmmioinfo->pExtraInfoStruct = (PVOID)pinstance;    // save it's address
   pmmioinfo->fccIOProc = HEX_FOURCC_FFIO;            // Make sure this is set for codec loading
   ioInstanceInit(pinstance);

   // Allocate memory for pTempBuffer which is used when
   // IOProcReadInterLeaved is called.

   if (ENTERCRIT(rc)) {
      ioCleanUp(pmmioinfo);
      return MMIO_ERROR;
   }

   pinstance->pTempBuffer = HhpAllocMem(hheap, DEFAULTBUFFERSIZE);

   if (pinstance->pTempBuffer == NULL) {
      EXITCRIT;
      pmmioinfo->ulErrorRet = MMIOERR_OUTOFMEMORY;
      ioCleanUp(pmmioinfo);
      return MMIO_ERROR;
   }
   EXITCRIT;

   pinstance->ulTempBufferSize     = DEFAULTBUFFERSIZE;
   pinstance->lFileCurrentPosition = 0;

   // set up the local MMIOINFO structure

   Localmmioinfo = *pmmioinfo;               // copy the original
   Localmmioinfo.pIOProc = NULL;
   Localmmioinfo.fccIOProc = pmmioinfo->fccChildIOProc;
   Localmmioinfo.ulFlags |= MMIO_NOIDENTIFY; // Eliminate callbacks
   Localmmioinfo.ulFlags &= ~MMIO_ALLOCBUF;  // Force non-buffered open

   // Determine if it is open for readonly or for write access

   if ((pmmioinfo->ulFlags  & MMIO_READ) && !(pmmioinfo->ulFlags & INVALID_READ_FLAGS))
      openType = MMIO_READ;
   else
      if ((pmmioinfo->ulFlags & (MMIO_READWRITE | MMIO_WRITE)) &&!(pmmioinfo->ulFlags & INVALID_WRITE_FLAGS))
         openType = MMIO_WRITE;

   // Perform either read or write specific open processing

   switch (openType) {

      case MMIO_READ:

         rc = ioIdentifyStorageSystem(&Localmmioinfo,pszFileName); // IOProc identifies Storage System
         if (rc != MMIO_SUCCESS) break;

         // Open the Movie file

         if (pmmioinfo->fccChildIOProc != FOURCC_MEM ) {
            Localmmioinfo.cchBuffer = 0;
            Localmmioinfo.pchBuffer = NULL;
         }

         pinstance->hmmioFileHandle = mmioOpen(pszFileName, &Localmmioinfo, MMIO_NOIDENTIFY);

         if (pinstance->hmmioFileHandle == 0) {
            rc = Localmmioinfo.ulErrorRet;
            break;
         }

         // call file format specific routine

         rc = ffOpenRead(pmmioinfo, pinstance);
         if (rc != MMIO_SUCCESS) break;

         rc = ioAddTracksToMovieHeader(pinstance);
         if (rc != MMIO_SUCCESS) break;

         lFilePosition = ffSeekToDataBegin(pmmioinfo,pinstance);
         pinstance->lFileCurrentPosition = lFilePosition;
         if (lFilePosition < 0) rc = MMIOERR_INTERNAL_SYSTEM;

         break;

      case MMIO_WRITE:

         if (!(pmmioinfo->ulFlags & MMIO_CREATE)) {
            rc = ioIdentifyStorageSystem(&Localmmioinfo, pszFileName);
            if (rc != MMIO_SUCCESS) break;
         }

         // Open the movie file

         pinstance->hmmioFileHandle = mmioOpen(pszFileName, &Localmmioinfo, Localmmioinfo.ulFlags);

         if (pinstance->hmmioFileHandle == 0) {
            rc = Localmmioinfo.ulErrorRet;
            break;
         }

         // call file format specific routine

         rc = ffOpenWrite(pmmioinfo, pinstance);   // call file format specific open routine
         if (rc != MMIO_SUCCESS) break;

         if (!(pmmioinfo->ulFlags & MMIO_CREATE)) {
            rc = ioAddTracksToMovieHeader(pinstance);
            if (rc != MMIO_SUCCESS) break;

            lFilePosition = ffSeekToDataBegin(pmmioinfo, pinstance);
            pinstance->lFileCurrentPosition = lFilePosition;
            if (lFilePosition < 0) rc = MMIOERR_INTERNAL_SYSTEM;
         }
         break;

      default:
         rc = MMIOERR_INTERNAL_SYSTEM;
         break;
   }

   if (rc != MMIO_SUCCESS) {
      ioCleanUp(pmmioinfo);
      pmmioinfo->ulErrorRet = rc;
      return rc;

//    valid return codes for MMIOM_OPEN are MMIO_SUCCESS or either
//    a DOS error code or a MMIO error code is returned.
//    return MMIO_ERROR;
   }

#ifndef CANT_EDIT
   // obtain PID used to mark clipboard entries and a dummy time stamp

   DosGetInfoBlocks(&ptib, &ppib);
   pinstance->pidPID = ppib->pib_ulpid;
   DosGetDateTime(&(pinstance->dtClipbrdTime));

   // set up the pathname in the instance structure for clipboard editing

   if ((pszFileName != NULL) && (strlen(pszFileName) < CCHMAXPATH)) {
      rc = ioEditGetFullName(pszFileName, (PSZ)&(pinstance->szFileName));
      if (rc != MMIO_SUCCESS) {
         pmmioinfo->ulErrorRet = rc;
         return rc;
//       return MMIO_ERROR;
      }
      pinstance->ulEditFlags |= FULLY_QUALIFIED_PATH;
   }
   else {
      pinstance->szFileName[0] = '\0';
      pinstance->ulEditFlags |= CANNOT_CUT_OR_COPY;   // don't know the file name
   }                                                  // can't put stuff into the clipboard
#endif

   pmmioinfo->ulErrorRet = 0;
   return MMIO_SUCCESS;
}

/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOCLOSE.C                                           */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc routine for MMIOM_CLOSE          */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the close routine.               */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      IOProcClose                                                       */
/*      ioCleanUp                                                         */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/

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
#include        <hhpheap.h>
#include        <ioi.h>

   extern HHUGEHEAP hheap;
   extern HMTX hmtxGlobalHeap;

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  IOProcClose                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: close file.                                            */
/*                                                                          */
/* FUNCTION: This function closes Digital Video file if opened,             */
/*           free allocated buffers.                                        */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcClose                                                 */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:    PMMIOINFO  pmmioinfo - pointer to MMIOINFO instance structure. */
/*                                                                          */
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

LONG IOProcClose(PMMIOINFO pmmioinfo) {
   LONG rc;
   PINSTANCE  pinstance;

   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(rc);

   pmmioinfo->ulErrorRet = 0L;

   ioCleanUp(pmmioinfo); /* mmioClose is called in CleanUp */

   if (pmmioinfo->ulErrorRet != 0L) return MMIO_ERROR;

   return MMIO_SUCCESS;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioCleanUp                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Clean up on error or before closing file.              */
/*                                                                          */
/* FUNCTION: Close any open file and free allocated instance control        */
/*           blocks and any temporary buffers created.                      */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioCleanUp                                                   */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMIOINFO pmmioinfo - pointer to MMIOINFO status structure. */
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

VOID ioCleanUp(PMMIOINFO pmmioinfo) {
   LONG            rc;              /* Return code.           */
   PINSTANCE       pinstance;       /* Local work structure.  */
   PTRACKI         ptracki;         /* Pointer to track list. */
   PCCB            pccb;            /* Pointer to CCB list.   */
   PMMVIDEOHEADER  pmmVideoHdr;     /* Video header node.     */
   ULONG           ulTrack;

   // Make sure the instant structure has been allocated, no memory need to
   // be freed otherwise

   if (pmmioinfo->pExtraInfoStruct) {
      pinstance = (PINSTANCE)pmmioinfo->pExtraInfoStruct;
      if (!pinstance) {
         pmmioinfo->ulErrorRet |= MMIOERR_INVALID_STRUCTURE;
         return;
      }
   }
   else {
      pmmioinfo->ulErrorRet |= MMIOERR_INVALID_STRUCTURE;
      return;
   }

   // Call File Format specific clean up routine

   ffCleanUp(pinstance);

   // If file is opened, close it

   if (pinstance->hmmioFileHandle) {
      rc = (LONG)mmioClose(pinstance->hmmioFileHandle,(WORD)0L);

      if (rc != MMIO_SUCCESS) pmmioinfo->ulErrorRet = rc;

      pinstance->hmmioFileHandle = 0;
   }

   pmmioinfo->hmmio = 0L;

   if (ENTERCRIT(rc)) return;

   // Free pTempBuffer in instant structure

   if(pinstance->pTempBuffer) {
      HhpFreeMem(hheap,pinstance->pTempBuffer);
      pinstance->pTempBuffer = NULL;
   }

   // pccbList is a link list, CleanUp free every node of link list

//
// pccbList is an array of link list for each track in the file.
// WLS 3.15.94
//

   if (pinstance->pccbList) {
      for (ulTrack = 0; ulTrack < pinstance->pmmhdr->ulNumEntries; ulTrack++) {
         while (pinstance->pccbList[ulTrack]) {
            pccb = pinstance->pccbList[ulTrack];
            if (pccb) {
               pinstance->pccbList[ulTrack] = pccb->pccbNext;
               ioCloseCodec(pccb);
            }
         }
      }
      HhpFreeMem(hheap, pinstance->pccbList);
   }

   if (pinstance->pccbLastUsed) {
      HhpFreeMem(hheap, pinstance->pccbLastUsed);
   }


   // ptrackiList is a link list, CleanUp free every node of link list

   while (pinstance->ptrackiList) {
      ptracki = pinstance->ptrackiList;

      if (ptracki) {

         if (ptracki->pTrackHeader) {

            // Delete movie video track specific data

            if (IS_VIDEO_TRACK(ptracki->ulMediaType)) {
               pmmVideoHdr = (PMMVIDEOHEADER)ptracki->pTrackHeader;

               if (pmmVideoHdr->pmmXDIBHeader)
                  HhpFreeMem(hheap,(PVOID)pmmVideoHdr->pmmXDIBHeader);

               if (pmmVideoHdr->genpalVideo.prgb2Entries)
                  HhpFreeMem(hheap,(PVOID)pmmVideoHdr->genpalVideo.prgb2Entries);

            }

            HhpFreeMem(hheap,(PVOID)ptracki->pTrackHeader);
         }

         if (ptracki->pRawHdr) HhpFreeMem(hheap,(PVOID)ptracki->pRawHdr);

         pinstance->ptrackiList = ptracki->ptrackiNext;
         HhpFreeMem(hheap,(PVOID)ptracki);
      }
   }

   if (pinstance->pRawHdr) {
      HhpFreeMem(hheap,(PVOID) pinstance->pRawHdr);       /* Free header */
      pinstance->pRawHdr = NULL;
   }

   if (pinstance->pmmhdr) {
      HhpFreeMem(hheap, (PVOID)pinstance->pmmhdr);        /* Free header */
      pinstance->pmmhdr = NULL;
   }

#ifndef CANT_EDIT
   ioEditCleanup(pinstance);                 // free clipboard editing control blocks
#endif

   HhpFreeMem(hheap, (PVOID)pinstance);      // Free instance structure
   pmmioinfo->pExtraInfoStruct = (PVOID)NULL;
   EXITCRIT;
}

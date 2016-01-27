/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOHEADER.C                                          */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc file header access module        */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains routines to access the file      */
/*           header.                                                      */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      IOProcGetHeader                                                   */
/*      IOProcQueryHeaderLength                                           */
/*      ioGetTrackHeader                                                  */
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
#include        <os2.h>                         /* OS/2 headers.            */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                      /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#include        <ioi.h>



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcGetHeader                                         */
/*                                                                          */
/* DESCRIPTIVE NAME: Read MM Digital video header structure                 */
/*                                                                          */
/* FUNCTION: copy either the standard presentation header or the file       */
/*           format specific header to the caller's buffer.                 */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcGetHeader                                             */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMIOINFO  pmmioinfo - Pointer to MMIOINFO status structure */
/*              LONG       Unknown1  - Pointer to caller's buffer.          */
/*              LONG       Unknown2  - Length of caller's buffer.           */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              Length of header returned                                   */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcGetHeader ( PMMIOINFO pmmioinfo,
                       PVOID pHeader,
                       LONG  lHeaderLength,
                       LONG  lCurrentTrack )

{
   LONG             rc;                       /* return code                */
   PTRACKI          ptracki;
   PINSTANCE        pinstance;                /* Local work structure.      */

   /**************************************************************************/
   /* Validates parameters passed:                                           */
   /* -- Must have valid pmmioinfo.                                          */
   /* -- File must be opened.                                                */
   /**************************************************************************/
   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(0L);

  if (pHeader == NULL) {
     pmmioinfo->ulErrorRet = MMIOERR_NO_BUFFER_ALLOCATED;
     return(0L);
     }

  if (CheckMem((PVOID)pHeader, lHeaderLength, PAG_WRITE)) {
     pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
     return(0L);
     }

   if (pinstance->lCurrentTrack == MMIO_RESETTRACKS){

      if(pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER){
         if (lHeaderLength < pinstance->ulMmHdrLength) {
            pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
            return(0L);
            }
         lHeaderLength = pinstance->ulMmHdrLength;
         memcpy(pHeader,pinstance->pmmhdr,lHeaderLength);  // copy all fields
         return (lHeaderLength);
         }

      else { /* not translateheader. */
         if (lHeaderLength < pinstance->ulRawHdrLength) {
            pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
            return(0L);
            }
         lHeaderLength = pinstance->ulRawHdrLength;
         memcpy(pHeader,pinstance->pRawHdr,lHeaderLength);
         return(lHeaderLength);
         }
      }

   else { /* Get a raw or translated track header */
      if (ptracki = ioFindTracki(pinstance,(ULONG)pinstance->lCurrentTrack)) {
         rc = ioGetTrackHeader(ptracki,
                               pHeader,
                               (ULONG)lHeaderLength,
                               pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER);
         if (rc)
            return(rc);      /* ok, return length */
         else {
            pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
            return(0L);
            }
         }

      else { /* lCurrrent Track is not a valid track. */
         pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
         return(0L);
         }
      }
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioGetTrackHeader                                        */
/*                                                                          */
/* DESCRIPTIVE NAME: Get Track header.                                      */
/*                                                                          */
/* FUNCTION: This function copies track header information to caller.       */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioGetTracksHeader                                           */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*     PTRACKI     ptracki        - Pointer to track info                   */
/*     PVOID       pHeader        - pointer to header.                      */
/*     ULONG       ulHeaderLength - Length of header.                       */
/*     ULONG       ulFlag         - Raw or Translated                       */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              ulHeaderLength                                              */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ioGetTrackHeader ( PTRACKI ptracki,
                        PVOID   pTrackHeader,
                        ULONG   ulHeaderLength,
                        ULONG   ulFlag )

{

   if (ulFlag & MMIO_TRANSLATEHEADER) {
      /* Get translated header */
      if (ulHeaderLength < ptracki->ulTrackHdrLength) {
         return(0L);
         }
      ulHeaderLength = ptracki->ulTrackHdrLength;
      memcpy(pTrackHeader,ptracki->pTrackHeader,ulHeaderLength);
      }

   else { /* Get raw header */
      if (ulHeaderLength < ptracki->ulRawHdrLength) {
         return(0L);
         }
      ulHeaderLength = ptracki->ulRawHdrLength;
      memcpy(pTrackHeader,ptracki->pRawHdr,ulHeaderLength);
      }

   return(ulHeaderLength);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcQueryHeaderLength                                 */
/*                                                                          */
/* DESCRIPTIVE NAME: Query header length of Digital Video file.             */
/*                                                                          */
/* FUNCTION: Returns either the length of translated header or the          */
/*           length of movieheader.                                         */
/*                                                                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcQueryHeaderLength                                     */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:       PMMIOINFO pmmioinfo - pointer to MMIOINFO status structure  */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcQueryHeaderLength ( PMMIOINFO pmmioinfo,
                               LONG      lCurrentTrack )

{
   LONG            rc; 
   PINSTANCE       pinstance;                   /* Local work structure.     */
   PTRACKI         ptracki;


   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(0L);

   if (pinstance->lCurrentTrack == MMIO_RESETTRACKS) {

      if (pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER) {
         return (pinstance->ulMmHdrLength);
         }
      else { /* Raw Header */
         return(pinstance->ulRawHdrLength);
         }
      }
   else {
      if (ptracki = ioFindTracki(pinstance,(ULONG)pinstance->lCurrentTrack)) {

         if (pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER) {
            return(ptracki->ulTrackHdrLength);
            }
         else {
            return(ptracki->ulRawHdrLength);
            }
         }
      else {
         pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
         return(MMIO_ERROR);
         }
      }
}



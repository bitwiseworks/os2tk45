/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULCODEC.C                                           */
/*                                                                        */
/* DESCRIPTIVE NAME: Ultimotion  IO Proc codec interfaces                 */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the routine.                     */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*     IOProcDecompress                                                   */
/*     ffOpenCodec                                                        */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/


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


   extern HHUGEHEAP hheap;                        /* Heap of memory.        */
   extern ULONG    ulNumColors;
   extern HMTX hmtxGlobalHeap;


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  IOProcDecompress                                       */
/*                                                                          */
/* DESCRIPTIVE NAME:                                                        */
/*                                                                          */
/* FUNCTION: This function calls DCIOProc to calcultate invalid clipping    */
/*           region. If the passed in is a palette block, this function     */
/*           will directly perform the palette translation and return to    */
/*           caller.                                                        */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcDecompress                                            */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PMMIOINFO pmmioinfo       - Pointer to MMIOINFO status structure. */
/*        PMMDECOMPRESS pmmdec      - Decompress structure.                 */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcDecompress ( PMMIOINFO pmmioinfo,
                        PMMDECOMPRESS pmmdec )

{
   LONG            rc = MMIO_SUCCESS;      /* Return code of IOProc's call.  */
   PCCB            pccb;                   /* Pointer to CCB structure       */
   PVIDEOFRAME     pumvfHdr = NULL;        /* Pointer to record prefix block.*/
   PINSTANCE       pinstance;

   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(rc);

   /*****************************************************/
   /* Check for invalid input flags                     */
   /*****************************************************/
   if (pmmdec->ulFlags & ~(VALID_DECOMPRESS_INPUTFLAGS)) {
      pmmioinfo->ulErrorRet = MMIOERR_INVALID_PARAMETER;
      return(MMIO_ERROR);
      }

   /*****************************************************/
   /* Fill in necessary information for DC IOProc;      */
   /* Send msg to DC IOProc to perform decompression.   */
   /*****************************************************/
   pmmdec->ulFlags &= ~(MMIO_IS_PALETTE | MMIO_IS_KEY_FRAME);

   /*****************************************************/
   /* Tricky - Multi-aperture check. Must make sure that*/
   /* We skip the frame header only for the first call  */
   /* of many multi-aperture calls. Each calls decomp-  */
   /* resses only a portion of the frame.               */
   /*****************************************************/
   if (pinstance->pccbLastUsed) {   /* Multiaperture non-first call */
      pccb = pinstance->pccbLastUsed;
      if (pccb->ulLastSrcBuf != (ULONG)pmmdec->pSrcBuf) {
         pmmioinfo->ulErrorRet = MMIOERR_INVALID_PARAMETER;
         return(MMIO_ERROR);
         }
      }

   else {   /* Multiaperture first call or single aperture call */
      /***************************************************************************/
      /* Since only one video track for current file format...video track = 0L.  */
      /* Validate the buffer as a video frame chunk.                             */
      /* Find the tracki structure for THE video track.                          */
      /***************************************************************************/
      pumvfHdr = (PVIDEOFRAME)pmmdec->pSrcBuf;       /* Point at beginning of chunk data */

      /* Tell stream handler we had reference frame or not */
      if (!(pumvfHdr->ulFrameFlags & UM_VIDEO_FRAME_FLAG_DELTAFRAME)) {
         pmmdec->ulFlags |= MMIO_IS_KEY_FRAME;
         }

      if (!(pccb = ioFindCodec(pinstance,pumvfHdr->ulCompressionType))) {
         pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
         return(MMIO_ERROR);
         }

      /***************************************************************************/
      /* If Buffer passed contains not a palette block, do the following:        */
      /* Check if DC List has valid information for DC IOProc.                   */
      /***************************************************************************/
      if (!pccb->hCodec) {
         pmmioinfo->ulErrorRet = MMIOERR_INTERNAL_SYSTEM;
         return(MMIO_ERROR);
         }

      /*************************************/
      /* Skip header at beginning of frame */
      /*************************************/
      pmmdec->pSrcBuf = (PVOID) &(pumvfHdr->bData[0]);
      pmmdec->ulSrcBufLen  -= (sizeof(VIDEOFRAME) - 1);
      }  /* Multiaperture first call */


   rc = pccb->pmmioproc(&pccb->hCodec,
                        MMIOM_CODEC_DECOMPRESS,
                        (LONG)pmmdec,
                        0L);

   if(rc != NO_ERROR) {
      pmmioinfo->ulErrorRet = rc;
      rc = MMIO_ERROR;
      } /* end if */


   /***********************************************/
   /* Check for Multiaperture case.               */
   /***********************************************/
   if (pmmdec->ulSrcBufLen && pmmdec->pSrcBuf) {
      pccb->ulLastSrcBuf = (ULONG)pmmdec->pSrcBuf;
      pinstance->pccbLastUsed = pccb;
      }

   else {
      /* Make sure buffer length is zero */
      pmmdec->ulSrcBufLen = 0;
      pccb->ulLastSrcBuf = 0L;
      pinstance->pccbLastUsed = 0L;
      }

   return(rc);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffOpenCodec                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:                                                        */
/*                                                                          */
/* FUNCTION: This function Opens  a codec instance for a movie instance.    */
/*           This is called at movie open time once for each codec required */
/*           for this particular movie instance.                            */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffOpenCodec                                                 */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PMMIOINFO pmmioinfo    - Pointer to MMIOINFO status structure. */
/*           PINSTANCE   pinstance  - Movie instance structure              */
/*           PCCB        pccb       - Pointer to codec control structure.   */
/*           ULONG       hCodec     - Handle to codec to associate with     */
/*                                    this codec. This is used where there  */
/*                                    are multiple codecs per DLL, but the  */
/*                                    share common data area.               */
/*             PTRACKI ptracki      - Track specific information            */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffOpenCodec ( PMMIOINFO pmmioinfo,
                   PINSTANCE pinstance,
                   PCCB pccb,
                   ULONG hCodec,
                   PTRACKI ptracki)

{
   LONG              rc = MMIO_SUCCESS;       /* Return code of IOProc's call. */
   PMMVIDEOHEADER     pmmVideoHdr;            /* Video header node.      */
   PCODECVIDEOHEADER  pcodecvidhdr;

   /**************************************************************************/
   /* Fill in necessary information for DC IOProc.                           */
   /**************************************************************************/
   pmmVideoHdr = (PMMVIDEOHEADER)ptracki->pTrackHeader; /* Get standard track header */

   pccb->codecopen.pControlHdr = NULL;               // FOR AVI, fill in from CODEC SPECIFIC DATA SECTION OF HEADER!
   pccb->codecopen.pOtherInfo = NULL;

   ENTERCRITX;
   /********************************************/
   /* Create Source Video Header               */
   /********************************************/
   if (pcodecvidhdr = (PCODECVIDEOHEADER)HhpAllocMem(hheap,(ULONG)sizeof(CODECVIDEOHEADER))) {
      pccb->codecopen.pSrcHdr = (PVOID)pcodecvidhdr;

      pcodecvidhdr->ulStructLen = sizeof(CODECVIDEOHEADER);
      pcodecvidhdr->cx = pmmVideoHdr->ulWidth;
      pcodecvidhdr->cy = pmmVideoHdr->ulHeight;
      pcodecvidhdr->cPlanes = 1;                          // Hardcoded
      pcodecvidhdr->cBitCount = 16;                       // Hardcoded
      pcodecvidhdr->ulColorEncoding = MMIO_COMPRESSED;    // Hardcoded

      /********************************************/
      /* Create Destination Video Header          */
      /********************************************/
      if (pcodecvidhdr = (PCODECVIDEOHEADER)HhpAllocMem(hheap,(ULONG)sizeof(CODECVIDEOHEADER))) {
         pccb->codecopen.pDstHdr = (PVOID)pcodecvidhdr;

         pcodecvidhdr->ulStructLen = sizeof(CODECVIDEOHEADER);
         pcodecvidhdr->cx = pmmVideoHdr->ulWidth;
         pcodecvidhdr->cy = pmmVideoHdr->ulHeight;
         pcodecvidhdr->cPlanes = 1;                          // Hardcoded

         /********************************************/
         /* Initialize the Flags and color encoding  */
         /********************************************/
         pccb->codecopen.ulFlags = pccb->cifi.ulCapsFlags & (VALID_CODECOPEN_INPUTFLAGS);

         /* Set the color depth for the CODEC we want */
         if (ulNumColors == 16) {
            pccb->codecopen.ulFlags |= CODEC_4_BIT_COLOR;
            pcodecvidhdr->cBitCount = 16;
            pcodecvidhdr->ulColorEncoding = MMIO_PALETTIZED;
            }
         else if (ulNumColors > 256) {
            pccb->codecopen.ulFlags |= CODEC_16_BIT_COLOR;
            pcodecvidhdr->cBitCount = 256;
            pcodecvidhdr->ulColorEncoding = MMIO_RGB_5_6_5;
            }
         else { /* 256 and anything else */
            pccb->codecopen.ulFlags |= CODEC_8_BIT_COLOR;
            pcodecvidhdr->cBitCount = 8;
            pcodecvidhdr->ulColorEncoding = MMIO_PALETTIZED;
            }

         /*****************/
         /* Open the Codec
         /*****************/
         rc = pccb->pmmioproc(&hCodec,
                              MMIOM_CODEC_OPEN,
                              (LONG)&pccb->codecopen,
                              0L);
         if (!rc) {
            pccb->hCodec = hCodec;
            }
         }
      }

   EXITCRIT;
   return(rc);
}


LONG ffAssociateCodec ( PINSTANCE pinstance,
                        PMMEXTENDINFO pmmextendinfo )
{
   LONG  rc = MMIO_SUCCESS;

   return(rc);
}

/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULOPEN.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: Ultimotion IO Proc routine for MMIOM_OPEN.           */
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
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*     ffOpenRead                                                         */
/*     ffOpenWrite                                                        */
/*     ffReadHeader                                                       */
/*     ffProcessVideoHeader                                               */
/*     ffInitCodecs                                                       */
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


   extern HHUGEHEAP hheap;
   extern HMTX hmtxGlobalHeap;


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffOpenWrite                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: PhotoMotion specific open routines                     */
/*                                                                          */
/* FUNCTION: Creates track lists and codec lists for the file.              */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffOpenWrite                                                 */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMIOINFO pmmioinfo - Pointer to MMIOINFO staus structure.  */
/*              PINSTANCE pinstance - Pointer to file instance structure.   */
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
LONG ffOpenWrite( PMMIOINFO pmmioinfo,
                  PINSTANCE pinstance )

{
   return(MMIO_SUCCESS);                      /* Nothing failed, return OK. */
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffOpenRead                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Ulitmotion file format specific open routine           */
/*                                                                          */
/* FUNCTION: This function opens UltiMotion file, allocates UltiMotion      */
/*           status structure, reads header, creates track list; DCIO proc  */
/*           list; track header.                                            */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffOpenRead                                                  */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PMMIOINFO pmmioinfo - Pointer to MMIOINFO staus structure.  */
/*              PINSTANCE pinstance - Pointer to instance structure.        */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*             MMIO_ERROR, pmmioinfo->ulErrorRet = rc                       */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffOpenRead ( PMMIOINFO pmmioinfo,
                  PINSTANCE pinstance )

{
   LONG            rc = MMIO_SUCCESS;
   MMCKINFO        mmckinfoHdr;
   PBYTE           pPosition;
   PVOID           pHdr;
   PUMCHUNKHEADER  pumchPosition;
   ULONG           ulNumTracks;      /* Keep # of tracks handy until mmhdr is allocated */
   ULONG           ulSize;
   PULONG          pulListForm;


   ENTERCRITX;
   ulNumTracks = 0L;
   if (!(rc = ffReadHeader(pmmioinfo,pinstance,&mmckinfoHdr))) {

      /**********************************************************************/
      /* Identify each chunk in the header LIST and save in the appropriate */
      /* place in the instance structure.                                   */
      /**********************************************************************/
      pPosition = (PBYTE)pinstance->pRawHdr;     /* point past form in hdr list */
      while (((ULONG)pPosition <
              ((ULONG)pinstance->pRawHdr + pinstance->ulRawHdrLength)) &&
             !rc) {

         /****************************************************/
         /* Match the chunk id up with the known chunk types */
         /****************************************************/
         pumchPosition = (PUMCHUNKHEADER)pPosition;
         pHdr = pPosition + sizeof(UMCHUNKHEADER);
         switch (pumchPosition->ulChunkID)  {

            /************************/
            case HEX_UMID_VIDEOHEADER:
            /************************/
               if (!(rc = ffProcessVideoHeader(pinstance,pHdr,pmmioinfo))) {
                  ulNumTracks++;
                  }
               break;

            /************************/
            case  HEX_UMID_VIDEOTITLE:
            /************************/
//##BILL-------rc = ffProcessVideoTitle(pinstance,pHdr);
               break;

            /***************/
            case FOURCC_LIST:
            /***************/
               pulListForm = (PULONG)pHdr;  /* point to form fourcc */
               pHdr = (PVOID)((ULONG)pHdr + SIZEOF_FORM); /* Point to real header start */
               ulSize = pumchPosition->ulChunkLength - SIZEOF_FORM;

               switch (*pulListForm) {

                  /***************************/
                  case HEX_UMFORM_AUDIOHDRLIST:
                  /***************************/
                     rc = ffProcessAudioHeaderList(pinstance,
                                                   pHdr,
                                                   ulSize,
                                                   &ulNumTracks);
                     break;

                  /**************************/
                  case HEX_UMFORM_TEXTHDRLIST:
                  /**************************/
//##BILL-------------rc = ffProcessTextHeaderList(pinstance,
//                                                pHdr,
//                                                ulSize,
//------------------------------------------------&ulNumTracks);
                     break;

                  /******/
                  default:  /* skip unknown LIST chunks */
                  /******/
                     break;
                  }
               break;

            /******/
            default: /* skip unknown LIST chunks */
            /******/
               break;
            }

         if (!rc) {
            /* Move position pointer to beginning of next chunk */
            pPosition += sizeof(UMCHUNKHEADER) + pumchPosition->ulChunkLength;
            if ((ULONG)pPosition % 2)  /* account for even chunk boundary */
               pPosition++;
            }

         } /* while */

      /***************************************************************************/
      /* Allocate MMHDR and update some of the information.                      */
      /***************************************************************************/
      if (!rc) {
         ulSize = (sizeof(MMTRACKINFO)*ulNumTracks) + sizeof(MMMOVIEHEADER);     //Jack this line changed!
         if (!(pinstance->pmmhdr = (PMMMOVIEHEADER)HhpAllocMem(hheap, ulSize))) {
            rc = MMIOERR_OUTOFMEMORY;
            }
         else {
            pinstance->ulMmHdrLength = ulSize;
            pinstance->pmmhdr->ulStructLen = sizeof(MMMOVIEHEADER);
            pinstance->pmmhdr->ulMediaType = MMIO_MEDIATYPE_MOVIE;
            pinstance->pmmhdr->ulNumEntries = ulNumTracks;
            pinstance->pmmhdr->pmmTrackInfoList = (PMMTRACKINFO)((ULONG)pinstance->pmmhdr + sizeof(MMMOVIEHEADER));
            pinstance->pmmhdr->ulMaxBytesPerSec = 0L;                       //TEMP TEMP
            pinstance->pmmhdr->ulPaddingGranularity = 0L;                   //TEMP TEMP
            pinstance->pmmhdr->ulSuggestedBufferSize = 0L;                  //TEMP TEMP
            pinstance->pmmhdr->ulStart = 0L;                                //TEMP TEMP
            pinstance->pmmhdr->ulLength = 0L;                               //TEMP TEMP
            pinstance->pmmhdr->ulNextTrackID = 0L;                          //TEMP TEMP
            pinstance->pmmhdr->pszMovieTitle = 0L;                          //TEMP TEMP
            pinstance->pmmhdr->ulCountry = 0L;                              //TEMP TEMP
            pinstance->pmmhdr->ulCodePage = 0L;                             //TEMP TEMP
            pinstance->pmmhdr->ulMovieCapsFlags = MOVIE_HAS_VIDEO+          //TEMP TEMP
                                                  MOVIE_HAS_AUDIO+
                                                  MOVIE_CAN_SCAN;
            }
         }
      }

   EXITCRIT;
   if (rc) {
      pmmioinfo->ulErrorRet = rc;
      return(MMIO_ERROR);
      }
   else {
      pinstance->ulFlags = OPENED_READONLY;
      return(MMIO_SUCCESS);                      /* Nothing failed, return OK. */
      }
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ffReadHeader                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: Read header                                            */
/*                                                                          */
/* FUNCTION: Validate that the file is a valid SMV RIFF file.               */
/*           Determines file size                                           */
/*           Read SMV Header list into allocated memory for parsing         */
/*                                                                          */
/* NOTES:                                                                   */
/*           (Caller must deallocate header memory)                         */
/*                                                                          */
/* ENTRY POINT: ffReadHeader                                                */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:     PMMIOINFO   pmmioinfo - Pointer to MMIOINFO staus structure.  */
/*            PINSTANCE   pinstance - instance structure of I/O Procedure.  */
/*            PMMCKINFO   pmmckinfoHdr - Return pointer to Headr chunk      */
/*                                       (Parent of header descends)        */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*              Message specific error                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffReadHeader ( PMMIOINFO pmmioinfo,
                    PINSTANCE pinstance,
                    PMMCKINFO pmmckinfoHdr )

{
   LONG            rc = MMIO_SUCCESS;                      /* Return code.         */
   LONG            lFilePosition;                          /* File position.       */


   /* set file pointer to beginning of file.*/
   lFilePosition = ioSeekFile((PLONG)&pmmioinfo->ulErrorRet,pinstance->hmmioFileHandle,0L);
   if (lFilePosition < MMIO_SUCCESS) {
      return(MMIO_ERROR);
      } /* end if */

   /* Find SMV Riff chunk (This verifies the file is an SMV movie file also */
   memset (pmmckinfoHdr, '\0', sizeof(MMCKINFO));
   pmmckinfoHdr->fccType = HEX_UMFORM_MOVIE;
   if (rc = mmioDescend(pinstance->hmmioFileHandle,
                        pmmckinfoHdr,
                        NULL,
                        MMIO_FINDRIFF))
      return (rc);

   /* update length of file */
   pinstance->ulFileLen = pmmckinfoHdr->ckSize + sizeof(UMCHUNKHEADER);

   /* Find the SMV headers list (All headers are in this chunk) */
   memset (pmmckinfoHdr, '\0', sizeof(MMCKINFO));
   pmmckinfoHdr->fccType = HEX_UMFORM_HEADERLIST;
   if (rc = mmioDescend(pinstance->hmmioFileHandle,
                        pmmckinfoHdr,
                        NULL,
                        MMIO_FINDLIST))
      return (rc);
   pinstance->ulRawHdrLength = pmmckinfoHdr->ckSize - SIZEOF_FORM;

   /* allocate enough memory to read in the entire header LIST */
   if ((pinstance->pRawHdr =
        (PRAWFILEHEADER)HhpAllocMem(hheap,
                                    pinstance->ulRawHdrLength)) == NULL) {
      return(MMIOERR_OUTOFMEMORY);
      }

   /* Read in the entire header LIST */
   rc = mmioRead(pinstance->hmmioFileHandle,
                 pinstance->pRawHdr,
                 pinstance->ulRawHdrLength);
   if (rc != pinstance->ulRawHdrLength) {
       return (MMIO_ERROR);
       }
   pinstance->lFileCurrentPosition = pmmckinfoHdr->ulDataOffset +
                                     rc + SIZEOF_FORM;

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffInitCodecs                                           */
/*                                                                          */
/* DESCRIPTIVE NAME: Initialize all codecs for this movie file.             */
/*                                                                          */
/* FUNCTION: This function is a file format specific routine that determines*/
/*           the codecs that need to be loaded to process this file.        */
/*           The codecs are then loaded.                                    */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffInitCodecs                                                */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PINSTANCE      pinstance  - Ptr to instance structure.            */
/*        PTRACKI        ptracki    - Ptr to track instance structure.      */
/*        PMMIOINFO      pmmioinfo - Pointer to MMIOINFO staus structure.   */
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
LONG ffInitCodecs ( PINSTANCE pinstance,
                    PTRACKI ptracki,
                    PMMIOINFO pmmioinfo )

{
   LONG              rc;
   PUMVIDEOHEADER    pumvh;
   USHORT            i;
   CODECINIFILEINFO  cifi;
   PCCB              pccb;

   pumvh = (PUMVIDEOHEADER)ptracki->pRawHdr;

   for (i=0; i < pumvh->usNumCompressionTypes; i++) {
      memset (&cifi, '\0', sizeof(CODECINIFILEINFO));
      cifi.ulStructLen = sizeof(CODECINIFILEINFO);
      cifi.fcc = pmmioinfo->fccIOProc;
      cifi.ulCompressType = pumvh->aulCompressionTypes[i];
      cifi.ulCompressSubType = 0L;
      cifi.ulCapsFlags = CODEC_DECOMPRESS;
      cifi.szHWID[0] = 0L;               /* Software algorithm */

      if (rc = ioDetermineCodec(pinstance, 0, &cifi)) {
         return(rc);
         }
      else {
         if (!(pccb = ioLoadCodec(pmmioinfo, pinstance, ptracki, &cifi)))
            return(MMIO_ERROR);
         }
      }

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffProcessVideoHeader                                   */
/*                                                                          */
/* DESCRIPTIVE NAME: Process a video header from the header LIST            */
/*                                                                          */
/* FUNCTION: This function creates a track header node based on the track   */
/*           media type.                                                    */
/*                                                                          */
/* NOTES:                                                                   */
/*        ** This code assumes that there is only one video track!          */
/*                                                                          */
/* ENTRY POINT: ffProcessVideoHeader                                        */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PINSTANCE      pinstance  - Ptr to instance structure.            */
/*        PUMVIDEOHEADER pumvhHdr   - Ptr to header (data in chunk)         */
/*        PMMIOINFO      pmmioinfo - Pointer to MMIOINFO staus structure.   */
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
LONG ffProcessVideoHeader ( PINSTANCE pinstance,
                            PUMVIDEOHEADER  pumvhHdr,
                            PMMIOINFO pmmioinfo )

{
   LONG              rc = MMIO_SUCCESS;            /* Return code.         */
   PTRACKI           ptracki;                      /* New node of ptracki. */
   PMMVIDEOHEADER    pmmVideoHdr;                  /* Video header node.      */
   PVOID             pRawHdr;

   if (ptracki = ioFindTracki(pinstance,0L)) {
      /* duplicate video track headers found */
      return(MMIOERR_INVALID_FILE);
      }
   else {
      if (pumvhHdr == NULL)
         return(MMIO_ERROR);

      /**********************************************************/
      /* If this is a new track, creates a track node, fill     */
      /* in necessary information to node.                      */
      /**********************************************************/
      if (!(ptracki = (PTRACKI)HhpAllocMem(hheap,(ULONG)sizeof(TRACKI)))) {
         return(MMIOERR_OUTOFMEMORY);
         }

      ptracki->ulTrackID     = 0L;       /* Assume only one track for now */
      ptracki->ulMediaType = MMIO_MEDIATYPE_DIGITALVIDEO;

      /**********************************************************/
      /* adds new node to the beginning of track list, sets     */
      /* pointer ptracki of record map table to ptrackiList     */
      /* that directs to decompress IOProc.                     */
      /**********************************************************/
      ptracki->ptrackiNext = pinstance->ptrackiList;
      pinstance->ptrackiList    = ptracki;

      /***************************************************************************/
      /* Allocates Standard Presentation Video Header and fills in the info.     */
      /***************************************************************************/
      if (!(pmmVideoHdr = (PMMVIDEOHEADER)HhpAllocMem(hheap,sizeof(MMVIDEOHEADER)))) {
         return(MMIOERR_OUTOFMEMORY);
         }

      pmmVideoHdr->ulStructLen = sizeof(MMVIDEOHEADER);
      pmmVideoHdr->ulContentType = MMIO_VIDEO_DATA;
      pmmVideoHdr->ulMediaType = ptracki->ulMediaType;
      pmmVideoHdr->ulVideoCapsFlags = 0L;
      pmmVideoHdr->ulWidth = (ULONG)pumvhHdr->usMaxFrameX;
      pmmVideoHdr->ulHeight = (ULONG)pumvhHdr->usMaxFrameY;
      pmmVideoHdr->ulScale = 1L;
      pmmVideoHdr->ulStart = 0L;                                    //TEMP TEMP
      pmmVideoHdr->ulLength = 0L;                                   //TEMP TEMP
      pmmVideoHdr->ulTotalFrames = 0L;                              //TEMP TEMP
      pmmVideoHdr->ulInitialFrames = 0L;                            //TEMP TEMP
//    pmmVideoHdr->ulSuggestedBufferSize = 0L;                      //TEMP TEMP

      if (pumvhHdr->usNomFrameDuration == 0) {
         pmmVideoHdr->mmtimePerFrame = 0L;
         pmmVideoHdr->ulRate = 0L;
         }
      else {
         pmmVideoHdr->mmtimePerFrame = (MMTIME)pumvhHdr->usNomFrameDuration;
         pmmVideoHdr->ulRate = (ULONG)(MMTIMEUNIT / (ULONG)pumvhHdr->usNomFrameDuration);
         }

      ptracki->pTrackHeader = (PVOID)pmmVideoHdr;
      ptracki->ulTrackHdrLength = sizeof(MMVIDEOHEADER);

      /***************************************************************************/
      /* Allocates Raw Video Header and fills in the info.                       */
      /***************************************************************************/
      if (!(pRawHdr = (PVOID)HhpAllocMem(hheap,sizeof(UMVIDEOHEADER)))) {
         return(MMIOERR_OUTOFMEMORY);
         }
      memcpy(pRawHdr,pumvhHdr,sizeof(UMVIDEOHEADER));
      ptracki->pRawHdr = pRawHdr;
      ptracki->ulRawHdrLength = sizeof(UMVIDEOHEADER);

      /****************************************************/
      /* Initialize all of the codecs needed for this     */
      /* video track. Load the codec IO Procs if necessary*/
      /****************************************************/
      if (rc = ffInitCodecs(pinstance, ptracki, pmmioinfo))
         return(rc);
      } /* else */

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffProcessAudioHeaderList                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Process an audio header LIST.                          */
/*                                                                          */
/* FUNCTION: This function creates a track header node based on the track   */
/*           media type. One track is created for each audio header         */
/*           found in the LIST.                                             */
/*                                                                          */
/* NOTES: none                                                              */
/*                                                                          */
/* ENTRY POINT: ffProcessAudioHeaderList                                    */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PINSTANCE      pinstance  - Ptr to instance structure.            */
/*        PVOID          pHdr       - Ptr to header (data in chunk)         */
/*        ULONG          ulChunkLength - length of LIST chunk               */
/*        PULONG         pulNumTracks - Update number of tracks             */
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
LONG ffProcessAudioHeaderList ( PINSTANCE pinstance,
                                PVOID  pListHdr,
                                ULONG  ulChunkLength,
                                PULONG pulNumTracks )

{
   LONG            rc = MMIO_SUCCESS;            /* Return code.         */
   PBYTE           pPosition;
   ULONG           ulLastTrackFound = 0L;
   PUMCHUNKHEADER  pumchPosition;
   PVOID           pHdr;

   /**********************************************************************/
   /* Identify each chunk in the Audio header LIST and save in the       */
   /* appropriate place in the instance structure.                       */
   /**********************************************************************/
   pPosition = (PBYTE)pListHdr;  /* point past form in hdr list */
   while (((ULONG)pPosition <
           ((ULONG)pListHdr + ulChunkLength)) &&
          !rc) {

      /****************************************************/
      /* Match the chunk id up with the known chunk types */
      /****************************************************/
      pumchPosition = (PUMCHUNKHEADER)pPosition;
      pHdr = pPosition + sizeof(UMCHUNKHEADER);
      switch (pumchPosition->ulChunkID)  {

         /*************************/
         case HEX_UMID_EXTAUDIONAME:
         /*************************/
//----------rc = ffProcessExtAudioHeader(pinstance,
//                                       (PEXTAUDIONAME)pHdr,
//                                       pumchPosition->ulChunkLength);
//----------(*pulNumTracks)++;
            break;

         /*************************/
         case  HEX_UMID_AUDIOHEADER:
         /*************************/
            if (!(rc = ffProcessIntAudioHeader(pinstance,
                                         (PAUDIOHEADER)pHdr,
                                         pumchPosition->ulChunkLength))) {
               (*pulNumTracks)++;
               }
            break;

         /************************/
         case  HEX_UMID_AUDIOTITLE:
         /************************/
//----------if (ulLastTrackFound)   /* ignore if no audio track defined */
//--##BILL-----rc = ffProcessAudioTitle(pinstance,pHdr,ulLastTrackFound);
            break;

         /******/
         default: /* skip unknown chunks */
         /******/
            break;
         }

      if (!rc) {
         /* Move position pointer to beginning of next chunk */
         pPosition += sizeof(UMCHUNKHEADER) + pumchPosition->ulChunkLength;
         if ((ULONG)pPosition % 2)  /* account for even chunk boundary */
            pPosition++;
         }

      } /* while */

   return(rc);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffProcessExtAudioHeader                                */
/*                                                                          */
/* DESCRIPTIVE NAME: Process an audio header LIST.                          */
/*                                                                          */
/* FUNCTION: Process an external audio file name header.                    */
/*                                                                          */
/* NOTES: none                                                              */
/*                                                                          */
/* ENTRY POINT: ffProcessExtAudioHeader                                     */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PINSTANCE      pinstance  - Ptr to instance structure.            */
/*        PEXTAUDIONAME  pumeaHdr   - Ptr to header (data in chunk)         */
/*        ULONG          ulHdrLength - Length of header                     */
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
LONG ffProcessExtAudioHeader ( PINSTANCE pinstance,
                               PEXTAUDIONAME pumeaHdr,
                               ULONG ulHdrLength )

{
//   LONG              rc = MMIO_SUCCESS;            /* Return code.         */
//   PTRACKI           ptracki;                      /* New node of ptracki. */
//   WAVE_HEADER      *pRawHdr;
//   PEXTAUDIONAME     pumea;
//   PMMAUDIOHEADER    pmmAudioHdr;                  /* Audio header node.      */
//   MMIOINFO          Localmmioinfo;                /* For locally used.    */
//   PMMCKINFO         pmmckinfoHdr;
//
//   if (ulHdrLength != sizeof(EXTAUDIONAME)) {
//      return(MMIOERR_INVALID_FILE);
//      }
//
//   if (pumeaHdr == NULL)
//      return(MMIO_ERROR);
//
//   if (ptracki = ioFindTracki(pinstance,pumeaHdr->usTrackNumber)) {
//      /* duplicate audio track headers found */
//      return(MMIOERR_INVALID_FILE);
//      }
//
//   /**********************************************************/
//   /* If this is a new track, creates a track node, fill     */
//   /* in necessary information to node.                      */
//   /**********************************************************/
//   if (!(ptracki = (PTRACKI)HhpAllocMem(hheap,(ULONG)sizeof(TRACKI)))) {
//      return(MMIOERR_OUTOFMEMORY);
//      }
//
//   /**********************************************************/
//   /* Open the waveform file and read the header. It should  */
//   /* be a riff wave file. Descend to the WAVE fmt chunk.    */
//   /**********************************************************/
//   ptracki->hmmioTrackFileHandle = mmioOpen(pumea->szFileName,&Localmmioinfo,MMIO_NOIDENTIFY);
//   if (ptracki->hmmioTrackFileHandle == (HMMIO)0L) {    /* Test file open error.*/
//      rc = Localmmioinfo.ulErrorRet;
//      }
//
//   memset (pmmckinfoHdr, '\0', sizeof(MMCKINFO));
//   pmmckinfoHdr->fccType = mmioFOURCC('W','A','V','E');
//   if (rc = mmioDescend(ptracki->hmmioTrackFileHandle,
//                        pmmckinfoHdr,
//                        NULL,
//                        MMIO_FINDRIFF))
//      return (rc);
//
//   /* Find the WAVE fmt chunk */
//   memset (pmmckinfoHdr, '\0', sizeof(MMCKINFO));
//   pmmckinfoHdr->fccType = mmioFOURCC('f','m','t',' ');
//   if (rc = mmioDescend(ptracki->hmmioTrackFileHandle,
//                        pmmckinfoHdr,
//                        NULL,
//                        MMIO_FINDLIST))
//      return (rc);
//
//   /***************************************************************************/
//   /* Allocates Raw Audio Header and read header from file.                   */
//   /***************************************************************************/
//   if (!(pRawHdr = (WAVE_HEADER *)HhpAllocMem(hheap,sizeof(WAVE_HEADER)))) {
//      return(MMIOERR_OUTOFMEMORY);
//      }
//   ptracki->pRawHdr = (PVOID)pRawHdr;
//   ptracki->ulRawHdrLength = sizeof(WAVE_HEADER);
//
//   /* Read in the entire WAV header */
//   rc = mmioRead(ptracki->hmmioTrackFileHandle,
//                 ptracki->pRawHdr,
//                 ptracki->ulRawHdrLength);
//   if (rc != ptracki->ulRawHdrLength) {
//       return (MMIO_ERROR);
//       }
//
//   ptracki->ulTrackID     = (ULONG)pumeaHdr->usTrackNumber;
//   ptracki->ulMediaType = MMIO_MEDIATYPE_AUDIO;                //##BILL - see comment below!
//
//   /**********************************************************/
//   /* adds new node to the beginning of track list, sets     */
//   /* pointer ptracki of record map table to ptrackiList     */
//   /* that directs to decompress IOProc.                     */
//   /**********************************************************/
//   ptracki->ptrackiNext = pinstance->ptrackiList;
//   pinstance->ptrackiList = ptracki;
//
//   /***************************************************************************/
//   /* Allocates Standard Presentation Audio Header and fills in the info.     */
//   /***************************************************************************/
//   if (!(pmmAudioHdr = (PMMAUDIOHEADER)HhpAllocMem(hheap,sizeof(MMAUDIOHEADER)))) {
//      return(MMIOERR_OUTOFMEMORY);
//      }
//
//   pmmAudioHdr->ulHeaderLength = sizeof(MMAUDIOHEADER);
//   pmmAudioHdr->ulContentType  = MMIO_AUDIO_MUSIC;            //UMB specific
//   pmmAudioHdr->ulMediaType    = MMIO_MEDIATYPE_AUDIO;        //UMB specific
//
//   /*
//    * Assign information to MMXWAV_HEADER.
//    */
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.usFormatTag      = pRawHdr->usFormatTag;
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.usChannels       = pRawHdr->usChannels;
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.ulSamplesPerSec  = pRawHdr->ulSamplesPerSec;
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.ulAvgBytesPerSec = pRawHdr->ulAvgBytesPerSec;
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBlockAlign     = pRawHdr->usBlockAlign;
//   pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBitsPerSample  = pRawHdr->usBitsPerSample;
//
//   ptracki->pTrackHeader = (PVOID)pmmAudioHdr;
//   ptracki->ulTrackHdrLength = sizeof(MMAUDIOHEADER);
//
//   /* Find the Data chunk */
//   memset (pmmckinfoHdr, '\0', sizeof(MMCKINFO));
//   pmmckinfoHdr->fccType = mmioFOURCC('d','a','t','a');
//   if (rc = mmioDescend(ptracki->hmmioTrackFileHandle,
//                        pmmckinfoHdr,
//                        NULL,
//                        MMIO_FINDLIST))
//      return (rc);
//
//   ptracki->ulMisc1 = 0L; /* init the amount of left-over audio data by which we need to ketchup! */
//
////-ptracki->ulMisc1 = pmmckinfoHdr->ckSize;  /* Save audio data length */              /*OLD_EXTAUDIO*/
////                                                                                     /*OLD_EXTAUDIO*/
//// /************************************************************/                      /*OLD_EXTAUDIO*/
//// /* Allocates Read buffer for external audio file reads.     */                      /*OLD_EXTAUDIO*/
//// /************************************************************/                      /*OLD_EXTAUDIO*/
//// if ((ptracki->ulMisc2 = (ULONG)HhpAllocMem(hheap, DEFAULTEXTAUDIOBUFFER)) == NULL) {/*OLD_EXTAUDIO*/
////    return (MMIOERR_OUTOFMEMORY);                                                    /*OLD_EXTAUDIO*/
////    }                                                                                /*OLD_EXTAUDIO*/
////                                                                                     /*OLD_EXTAUDIO*/
//// /***************************************/                                           /*OLD_EXTAUDIO*/
//// /* Pre-fill the read buffer with data. */                                           /*OLD_EXTAUDIO*/
//// /***************************************/                                           /*OLD_EXTAUDIO*/
//// rc = mmioRead(ptracki->hmmioTrackFileHandle,                                        /*OLD_EXTAUDIO*/
////               ptracki->ulMisc2,                                                     /*OLD_EXTAUDIO*/
////               DEFAULTEXTAUDIOBUFFER);                                               /*OLD_EXTAUDIO*/
//// ptracki->ulMisc3 = ptracki->ulMisc2;  /* Point to beginning of data buffer */       /*OLD_EXTAUDIO*/
////-ptracki->ulMisc4 = rc;                /* Fill in length of data */                  /*OLD_EXTAUDIO*/

   return(MMIO_SUCCESS);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffProcessIntAudioHeader                                */
/*                                                                          */
/* DESCRIPTIVE NAME: Process an audio header LIST.                          */
/*                                                                          */
/* FUNCTION: Process an interleaved audio track header.                     */
/*                                                                          */
/* NOTES: none                                                              */
/*                                                                          */
/* ENTRY POINT: ffProcessIntAudioHeader                                     */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PINSTANCE      pinstance  - Ptr to instance structure.            */
/*        PAUDIOHEADER   pumahHdr   - Ptr to header (data in chunk)         */
/*        ULONG          ulHdrLength - Length of header                     */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*              MMIOERR_INVALID_FILE                                        */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ffProcessIntAudioHeader ( PINSTANCE pinstance,
                               PAUDIOHEADER pumahHdr,
                               ULONG ulHdrLength )

{
   LONG              rc = MMIO_SUCCESS;            /* Return code.         */
   PTRACKI           ptracki;                      /* New node of ptracki. */
   PVOID             pRawHdr;
   PMMAUDIOHEADER    pmmAudioHdr;                  /* Audio header node.      */

   if (ulHdrLength != sizeof(AUDIOHEADER)) {
      return(MMIOERR_INVALID_FILE);
      }

   if (pumahHdr == NULL)
      return(MMIO_ERROR);

   if (ptracki = ioFindTracki(pinstance,pumahHdr->usTrackNumber)) {
      /* duplicate audio track headers found */
      return(MMIOERR_INVALID_FILE);
      }
   else {

      /**********************************************************/
      /* If this is a new track, creates a track node, fill     */
      /* in necessary information to node.                      */
      /**********************************************************/
      if (!(ptracki = (PTRACKI)HhpAllocMem(hheap,(ULONG)sizeof(TRACKI)))) {
         return(MMIOERR_OUTOFMEMORY);
         }
      ptracki->ulTrackID  = (ULONG)pumahHdr->usTrackNumber;
      ptracki->ulMediaType = MMIO_MEDIATYPE_AUDIO;                //##BILL - see comment below!


      /**********************************************************/
      /* adds new node to the beginning of track list, sets     */
      /* pointer ptracki of record map table to ptrackiList     */
      /* that directs to decompress IOProc.                     */
      /**********************************************************/
      ptracki->ptrackiNext = pinstance->ptrackiList;
      pinstance->ptrackiList = ptracki;

      /***************************************************************************/
      /* Allocates Standard Presentation Audio Header and fills in the info.     */
      /***************************************************************************/
      if (!(pmmAudioHdr = (PMMAUDIOHEADER)HhpAllocMem(hheap,sizeof(MMAUDIOHEADER)))) {
         return(MMIOERR_OUTOFMEMORY);
         }

      pmmAudioHdr->ulHeaderLength = sizeof(MMAUDIOHEADER);
      pmmAudioHdr->ulContentType  = MMIO_AUDIO_MUSIC;            //UMB specific
      pmmAudioHdr->ulMediaType    = MMIO_MEDIATYPE_AUDIO;        //UMB specific

      /*
       * Assign information to MMXWAV_HEADER.
       */
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.usFormatTag      = pumahHdr->usFormatTag;
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.usChannels       = pumahHdr->usChannels;
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.ulSamplesPerSec  = pumahHdr->ulSamplesPerSec;
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.ulAvgBytesPerSec = pumahHdr->ulAvgBytesPerSec;
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBlockAlign     = pumahHdr->usBlockAlign;
      pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBitsPerSample  = pumahHdr->usBitsPerSample;

      ptracki->pTrackHeader = (PVOID)pmmAudioHdr;
      ptracki->ulTrackHdrLength = sizeof(MMAUDIOHEADER);

      /***************************************************************************/
      /* Allocates Raw Audio Header and fills in the info.                       */
      /***************************************************************************/
      if (!(pRawHdr = (PVOID)HhpAllocMem(hheap,sizeof(AUDIOHEADER)))) {
         return(MMIOERR_OUTOFMEMORY);
         }
      memcpy(pRawHdr,pumahHdr,sizeof(AUDIOHEADER));
      ptracki->pRawHdr = pRawHdr;
      ptracki->ulRawHdrLength = sizeof(AUDIOHEADER);
      } /* else */

   return(MMIO_SUCCESS);
}


//################################################################################################
//################################################################################################
//################################################################################################
//LONG ffProcessVideoTitle ( PINSTANCE pinstance,
//                           PVOID  pHdr )
//
//{
//   // BETA - UNSUPPORTED FUNCTION!!!!!!!
//   //
//   // Cases to handle:
//   //          - Search last video tracki (Only one in theis case)
//   //          - if none, error
//   //          - else, add the video title to the list of video titles in tracki structure!
//   //
//   return(MMIO_SUCCESS);
//}
//################################################################################################
///* pHdr points to the data in the chunk */
//LONG ffProcessTextHeaderList ( PINSTANCE pinstance,
//                               PVOID  pHdr,
//                               ULONG  ulChunkLength )
//
//{
//   // BETA - UNSUPPORTED FUNCTION!!!!!!!
//   //
//   // Cases to handle:
//   //          - case HEX_UMID_TEXTHEADER
//   //          - Create track
//   //          - Read video header
//   //
//   return(MMIO_SUCCESS);
//}

/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULREAD.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: Ultimotion IO Proc routine for MMIOM_READ            */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the read routine.                */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*       IOProcRead                                                       */
/*       IOProcReadInterLeaved                                            */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/


#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <memory.h>

#define         INCL_DOS                        /* #define  INCL_DOSPROCESS. */
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                         /* OS/2 headers.             */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                       /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#include        <ioi.h>

   extern HHUGEHEAP hheap;
   extern HMTX hmtxGlobalHeap;


//#define CHUNKSIZE          (pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER))
// Fixes files to be parsed as true RIFF files - 0 padded if odd size chunk.
#define CHUNKSIZE          ((pumchHdr->ulChunkLength % 2)                                        \
                                  ? (pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER)+1)   \
                                  : (pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER)))



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  IOProcRead                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Read data from file.                                   */
/*                                                                          */
/* FUNCTION: This function will read data from previously opened UltiMotion */
/*           file, and return number of bytes read.                         */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcRead                                                  */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:     PMMIOINFO pmmioinfo  - Pointer to MMIOINFO status structure.  */
/*            PBYTE     pbBuffer   - Pointer to caller's buffer             */
/*            LONG      lBytes     - Number of bytes to read.               */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              0                                                           */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcRead ( PMMIOINFO pmmioinfo,
                  PBYTE pbBuffer,
                  LONG  lBytes )

{
  LONG  rc = MMIO_SUCCESS;
//LONG  lByteCount  = 0L;
//PVOID pTmpReadBuffer = XXXBuffer;
//PVOID pTmpRec;
//PSZ   pLocalBuffer;
//PRECMAP precmap;
//PINSTANCE  pinstance;
//PUMCHUNKHEADER pRecord;
//ULONG TrackWanted = 0L;
//ULONG BufferLengthCount = 0L;
//ULONG DataOffset = 0L;
//
//
// if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
//      return(rc);
//
//  if (XXXBuffer == (PSZ) 0L){
//     pmmioinfo->ulErrorRet = MMIOERR_INVALID_PARAMETER;
//     return(MMIO_ERROR);
//     }
// /* Test byte count requested.*/
//  if (lBytes <= 0L) {
//     pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
//     return(MMIO_ERROR);
//     }
//
//  if (pinstance->lFileCurrentPosition >= pinstance->ulFileLen){
//     pmmioinfo->ulErrorRet = MMIOERR_EOF_SEEN;
//     return(0L);
//     }
//
//  if ((pinstance->lFileCurrentPosition + lBytes) >= pinstance->ulFileLen){
//      lBytes = pinstance->ulFileLen - pinstance->lFileCurrentPosition;
//      }
//
// /***************************************************************************/
// /* if a particular track is not requested to be read, do the normal read   */
// /* which is to read the requested lBytes to the buffer passed, then return.*/
// /***************************************************************************/
//  if (pinstance->lCurrentTrack == RESETTRACKS){
//      if (!(pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA)){
//          lBytes = mmioRead(pinstance->hmmioFileHandle,XXXBuffer,lBytes);
//          if (lBytes < 0L){
//             pmmioinfo->ulErrorRet = mmioGetLastError(pinstance->hmmioFileHandle);
//             return(MMIO_ERROR);
//             }
//          pinstance->lFileCurrentPosition += lBytes;
//          return(lBytes);
//          }/* end if */
//      else{
//          pmmioinfo->ulErrorRet = MMIOERR_INVALID_ACCESS_FLAG;
//          return(MMIO_ERROR);
//          }
//      }/* end if lCurrentTrack */
//
// /***************************************************************************/
// /* If a particular track is requested:                                     */
// /* the following process will read only data from track.                   */
// /***************************************************************************/
//  if (ioFindTracki(pinstance,(ULONG)pinstance->lCurrentTrack)){
//      if (pinstance->pTempBuffer == NULL){
// ENTERCRIT
//          if ((pinstance->pTempBuffer = HhpAllocMem(hheap,DEFAULTBUFFERSIZE)) == NULL){
//               pmmioinfo->ulErrorRet = MMIOERR_OUTOFMEMORY;
//               return(MMIO_ERROR);
//               }
// EXITCRIT
//          pinstance->ulTempBufferSize;
//          }/* end if */
//
//      while (lByteCount < lBytes){
//            if (pinstance->lLastTrack == pinstance->lCurrentTrack){
//                if (pinstance->pLastBuffer){
//                    memcpy(pTmpReadBuffer,pinstance->pLastBuffer,pinstance->ulLastBufferOffset);
//                    BufferLengthCount   = pinstance->ulLastBufferOffset;
//                    }
//                }
//            if (!pinstance->lLastTrack){
//                pRecord = (PUMCHUNKHEADER)pinstance->pTempBuffer;// move to 1st stat. after while.??
//                pTmpRec = pinstance->pTempBuffer;
//                rc = mmioRead(pinstance->hmmioFileHandle,
//                              (PSZ)pinstance->pTempBuffer,
//                              pinstance->ulTempBufferSize);
//                pinstance->lFileCurrentPosition += pinstance->ulTempBufferSize;
//                }/* end if */
//            while(lByteCount < lBytes){
//                 /***********************************************************/
//                 /* IF last temp buffer has copied a portion of the data of */
//                 /* a record to user buffer,now we want to copy the remained*/
//                 /* data of the record to user buffer. ELSE we ignore that  */
//                 /* block of data and advance pRecord to next record.       */
//                 /***********************************************************/
//                  if (TrackWanted){
//                      memcpy(pTmpReadBuffer,pinstance->pTempBuffer,BufferLengthCount);
//                      pTmpRec = (PVOID)(BufferLengthCount + (ULONG)pTmpRec);
//                      pRecord = (PUMCHUNKHEADER)pTmpRec;
//                      TrackWanted = 0L;
//                      }/* end if */
//                  else{
//                      pTmpRec = (PVOID)(BufferLengthCount + (ULONG)pTmpRec);
//                      pRecord = (PUMCHUNKHEADER)pTmpRec;
//                      }/* end else */
//                 /***********************************************************/
//                 /* Base on pRecord->usID, we find record map entry.        */
//                 /***********************************************************/
//                  precmap = ffFindRecordMap(pRecord->usID,VOIDPARM);
//                  if (precmap == NULL){  /* precmap must be found. */
//                      pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
//                      return(MMIO_ERROR);
//                      }/* end if */
//                 /***********************************************************/
//                 /* Increments BufferLengthCount by a whole block of record.*/
//                 /* Check if the pRecord spans temp buffer,then see if that */
//                 /* record has a track# we're interested in, if yes, check  */
//                 /* user buffer space then copy a whole block of record to  */
//                 /* user buffer. If user buffer can't store the whole block */
//                 /* of data, copy as much as possible and return.           */
//                 /***********************************************************/
//                  BufferLengthCount += RECSIZE;
//                  if (BufferLengthCount <= pinstance->ulTempBufferSize){
//                      if (precmap->ulTrackID == pinstance->lCurrentTrack){
//                          if((lByteCount + RECSIZE) <= lBytes){
//                              memcpy(pTmpReadBuffer,pinstance->pTempBuffer,RECSIZE);
//                              lByteCount += RECSIZE;
//                              pTmpReadBuffer = (PVOID)(RECSIZE + (ULONG)pTmpReadBuffer);
//                              }/* end if */
//                          else{/* copy as much as possible. */
//                              memcpy(pTmpReadBuffer,pRecord,(lBytes - lByteCount));
//                              pinstance->pLastBuffer        = (PVOID)((lBytes - lByteCount) + (LONG)pTmpRec);
//                              pinstance->ulLastBufferOffset = (pinstance->ulTempBufferSize -
//                                                            (RECSIZE - (lBytes - lByteCount)) );
//                              lByteCount = lBytes;
//                              break;
//                              }/* end else */
//                          }/* end if precmap */
//                 //     if (pTmpRec = (PVOID)(RECSIZE + (ULONG)pTmpRec)){
//                 //         pRecord = pTmpRec;
//                 //         }
//                 //     else{ break; }
//                      }/* end if */
//                 /***********************************************************/
//                 /* A pRecord block spans the temp buffer: check if that is */
//                 /* a record need to be copied to user buffer, if yes, copy */
//                 /* it to user buffer though that is not a whole block, the */
//                 /* rest will be copied after the temp buffer is refilled.  */
//                 /***********************************************************/
//                  else{
//                      DataOffset = (pRecord->ulLength -
//                                   (BufferLengthCount - pinstance->ulTempBufferSize) +
//                                   (sizeof(UMCHUNKHEADER)) );
//
//                      if (precmap->ulTrackID == pinstance->lCurrentTrack){
//                          if((lByteCount + DataOffset) <= lBytes){
//                              memcpy(pTmpReadBuffer,pRecord,DataOffset);
//                              pTmpReadBuffer = (PVOID)(DataOffset + (ULONG)pTmpReadBuffer);
//                              lByteCount += DataOffset;
//                              }
//                          else{ /* copy as much as possible. */
//                              memcpy(pTmpReadBuffer,pRecord,(lBytes - lByteCount));
//                              pinstance->pLastBuffer        = (PVOID)((lBytes - lByteCount) + (LONG)pTmpRec);
//                              pinstance->ulLastBufferOffset = (pinstance->ulTempBufferSize -
//                                                            (DataOffset - (lBytes - lByteCount)) );
//                              lByteCount = lBytes;
//                              break;
//                              }
//                          TrackWanted = 1L;
//                          }/* end if precmap */
//
//                      BufferLengthCount   = RECSIZE - DataOffset;
//                      pinstance->lLastTrack = 0L;
//                      break;
//                      }/* end else */
//                  }/* end while */
//            }/* end while */
//      pinstance->lLastTrack = pinstance->lCurrentTrack;
//      precmap->ptracki->ulLastReadPos = 0L; //???????????
//
//      }/* end if */
//  else{/* not valid track */
//      pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
//      return(MMIO_ERROR);
//      }
//  return(lBytes);
//
   return(rc);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffRoundChunkLength                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Round out the length of chunk to even boundary         */
/*                                                                          */
/* FUNCTION: This function checks for BETA content files and ignores this   */
/*           rounding effect. Otherwise, round chunk length to even bounds. */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffRoundChunkLength                                          */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              ulChunkSize                                                 */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
ULONG ffRoundChunkLength ( PINSTANCE pinstance,
                           PUMCHUNKHEADER  pumchHdr)

{
   ULONG           ulChunkSize;
   PUMVIDEOHEADER  pumvh;

   /*******************************************************************/
   /* Parse BETA (semi-riff) files ok (BETA PROVIDED CONTENT ONLY)    */
   /*******************************************************************/
   pumvh = (PUMVIDEOHEADER)((ULONG)pinstance->pRawHdr + sizeof(UMCHUNKHEADER));

   if (pumvh->Reserved[0] == HEX_FOURCC_FFIO) {                               /*BETAONLY*/
      ulChunkSize = pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER);   /*BETAONLY*/
      }                                                                       /*BETAONLY*/
   else {
      if (pumchHdr->ulChunkLength % 2) {
         ulChunkSize = pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER)+1;
         }
      else {
         ulChunkSize = pumchHdr->ulChunkLength + (ULONG)sizeof(UMCHUNKHEADER);
         }
      }

   return(ulChunkSize);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ffSavePartialFrame                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Save the leftover of a frame to temp buffer.           */
/*                                                                          */
/* FUNCTION: This function saves any leftover data from this read into a    */
/*           temp buffer for the next read to use.                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ffSavePartialFrame                                          */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
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
LONG ffSavePartialFrame(PINSTANCE pinstance,
                        PMMMULTITRACKREAD pmtread,
                        ULONG  ulSaveLen,
                        PVOID pSaveData)

{
   pinstance->ulDataInTempBuffer = ulSaveLen;

   /* if temp buffer is not big enough, enlarges the size of buffer. */
   if (ulSaveLen > pinstance->ulTempBufferSize) {
      ENTERCRITX;
      HhpFreeMem(hheap,pinstance->pTempBuffer);

      if ((pinstance->pTempBuffer  = HhpAllocMem(hheap,ulSaveLen)) == NULL) {
         EXITCRIT;
         return(MMIOERR_OUTOFMEMORY);
         }
      EXITCRIT;
      }

   pinstance->ulLastBufferOffset    = (ULONG)0L;

   if (ulSaveLen) {
      memcpy(pinstance->pTempBuffer,pSaveData,ulSaveLen);
      pmtread->ulFlags = (ULONG)0L;
      }

   return(MMIO_SUCCESS);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcMultiTrackRead                                    */
/*                                                                          */
/* DESCRIPTIVE NAME: Read and sorts data, send them to a proper track.      */
/*                                                                          */
/* FUNCTION: MMMULTITRACKREAD     contains a pBuffer, a track list which    */
/*           has valid track numbers of the current file. Each track carries*/
/*           a table, each element of the table is a pvoid pointer(pRecord).*/
/*           This function reads data to the pBuffer passed, goes through   */
/*           every record, maps the track number based on record ID;        */
/*           If a track number of the record is found, and the table is     */
/*           not full, then sets pRecord to the record.  The function       */
/*           also supports two frequently happened situations:              */
/*           1) The last record in pBuffer is often read incompletely, this */
/*              means that record length spans pBuffer.  When this happens, */
/*              we don't set a pRecord to that record, copy the data to the */
/*              Temp Buffer instead.  When next IOProcReadInterLeaved is    */
/*              called, that record will be returned.                       */
/*           2) If one of the tables in track list is filled, the sorting   */
/*              process is terminated, return whatever has been sorted to   */
/*              caller, the remainded data will be sorted when next         */
/*              IOProcReadInterLeaved is called.                            */
/*                                                                          */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: IOProcMultiTrackRead                                        */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:       PMMIOINFO              pmmioinfo                            */
/*              PMMMULTITRACKREAD      pmtread                              */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              0                                                           */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcMultiTrackRead ( PMMIOINFO pmmioinfo,
                             PMMMULTITRACKREAD pmtread )

{
   LONG            rc;                           /* Return code */
   PINSTANCE       pinstance;                    /* Ptr to movie instance structure */
   PVOID           pReadBuffer;                  /* Ptr to buffer to read data from storage device into */
   PVOID           pTemp;                        /* Ptr to each record.     */
   ULONG           BufferLengthCounter = 0L;     /* Counter of bufferlength.*/
//-BOOL            fFirstPalette = FALSE;        /* First palette flag.     */
   BOOL            fReturnFrame = TRUE;          /* Return this frame data to caller */
   ULONG           Counter = 0L;
   LONG            lBytes;                       /* Buffer length.     */
   PTRACKMAP       ptrackmap;                    /* Ptr to track map.       */
   PESRCBUFTAB     pesrcbuftab;                  /* Ptr to escrbuftab array. */
   PUMCHUNKHEADER  pumchHdr;                     /* Pointer to record prefix block.*/
   PUSHORT         pusTrackNum;                  /* Pointer to record prefix block.*/
   PTRACKI         ptracki;                      /* Pointer to internal track structure */
   PTRACKI         ptrackiExtAudio;              /* Pointer to an external audio file track */ /*EXTAUDIO*/
   ULONG           ulReadBufferLen = 0L;         /* Read buffer real size (minus ext audio part) */ /*EXTAUDIO*/
   ULONG           ulExtAudBufLen = 0L;          /* Ext audio buffer size */                   /*EXTAUDIO*/
   ULONG           ulVidDuration = 0L;           /* Duration of video frames read */           /*EXTAUDIO*/
   PMMAUDIOHEADER  pmmAudioHdr;                  /* Audio header ptr        */                 /*EXTAUDIO*/


   if (rc = ioGetPtrInstance(pmmioinfo,&pinstance))
      return(rc);     // Always MMIO_ERROR

//-if (pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA){          /* Seek needs this flag set, so don't fail here */
//    pmmioinfo->ulErrorRet = MMIOERR_INVALID_ACCESS_FLAG;
//    return(MMIO_ERROR);
//----}

   pReadBuffer = pmtread->pBuffer;

   if (rc = ioValidateRead(pinstance,
                           pReadBuffer,
                           pmtread->ulNumTracks,
                           pmtread->pTrackMapList)) {
      pmmioinfo->ulErrorRet = rc;
      return(MMIO_ERROR);
      }


   /*******************************************************************/  /*EXTAUDIO*/
   /* Determine if any of the tracks refer to an external audio file  */  /*EXTAUDIO*/
   /*******************************************************************/  /*EXTAUDIO*/
   if (ptrackiExtAudio = ffFindExtAudioTrack(pinstance,                   /*EXTAUDIO*/
                                             pmtread->ulNumTracks,        /*EXTAUDIO*/
                                             pmtread->pTrackMapList)) {  /*EXTAUDIO*/

      /**************************************************************************/
      /* Compute the percentage of buffer should be reserved for external audio */
      /**************************************************************************/
      pmmAudioHdr = (PMMAUDIOHEADER)ptrackiExtAudio->pTrackHeader;                            /*EXTAUDIO*/
      ulExtAudBufLen =  (ULONG) ((float)pmtread->ulLength / EXTAUDIOPERCENT);                 /*EXTAUDIO*/
      ulExtAudBufLen += (ulExtAudBufLen % pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBlockAlign); /*EXTAUDIO*/
      }
   ulReadBufferLen = pmtread->ulLength - ulExtAudBufLen;                                      /*EXTAUDIO*/

   /**************************************************************************/
   /* check to make sure that we are still within the Data Frame List chunk  */
   /**************************************************************************/
   if ((pinstance->lFileCurrentPosition >= pinstance->ulMisc1) ||
       (pinstance->lFileCurrentPosition < pinstance->ulMisc2)) {
      if (ptrackiExtAudio) {    /* Check for external audio data left to read */     /*EXTAUDIO*/
         //                                                                          /*EXTAUDIO*/
         // ##BILL - add call to the code to check for audio data! - make the code   /*EXTAUDIO*/
         //          at the end of this routine into a routine that is callable      /*EXTAUDIO*/
         //          at this point to recover real audio data!!!!!!!                 /*EXTAUDIO*/
         //                                                                          /*EXTAUDIO*/
#ifdef MMDEBUG                                                                       /*EXTAUDIO*/
         DosBeep(500,600);     // Let them know !                                    /*EXTAUDIO*/
#endif                                                                               /*EXTAUDIO*/
         // if no external audio data                                                /*EXTAUDIO*/
         return(MMIOERR_CHUNKNOTFOUND);                                              /*EXTAUDIO*/
         }                                                                           /*EXTAUDIO*/
      else                                                                           /*EXTAUDIO*/
         return(MMIOERR_CHUNKNOTFOUND);
      }                                                                              /*EXTAUDIO*/

   /**************************************************************************/
   /* Sets pumchHdr to a first chunk header         which is expected in the */
   /* beginning of the pBuffer after read.                                   */
   /**************************************************************************/
   pumchHdr = (PUMCHUNKHEADER)pReadBuffer;
   pTemp = pReadBuffer;
   lBytes = ulReadBufferLen;

   /**************************************************************************/
   /* check if there is data need to be copied from TempBuffer to pBuffer.   */
   /**************************************************************************/
   if (pinstance->ulDataInTempBuffer) {

      if (ulReadBufferLen <= pinstance->ulDataInTempBuffer) {
         pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
         return(MMIO_ERROR);
         }

      memcpy(pReadBuffer,pinstance->pTempBuffer,pinstance->ulDataInTempBuffer);
      pTemp = (PVOID)(pinstance ->ulDataInTempBuffer + (ULONG)pReadBuffer);
      lBytes  -= pinstance->ulDataInTempBuffer;
      Counter  = pinstance->ulDataInTempBuffer;
      pinstance->ulDataInTempBuffer = 0L;
      }

   /**************************************************************************/
   /* if previous sorting encountered READINTERLEAVED_NOTDONE, caller has    */
   /* to pass back the previous pBuffer (if not, error returned), then sets  */
   /* pumchHdr to a position where the last sorting stopped at.  This read   */
   /* doesn't read data to pBuffer, and only sorts record from previous      */
   /* pBuffer.                                                               */
   /**************************************************************************/
   if (pinstance->ulLastBufferOffset) {

      if(pinstance->pLastBuffer != pReadBuffer) {
         pmmioinfo->ulErrorRet = MMIOERR_INVALID_PARAMETER;
         return(MMIO_ERROR);
         }

      else {
         BufferLengthCounter = pinstance->ulLastBufferOffset;
         pTemp = (PVOID)(pinstance->ulLastBufferOffset + (ULONG)pTemp);
         pumchHdr = (PUMCHUNKHEADER)pTemp;
         }
      }

   /**************************************************************************/
   /* else read data to buffer, and update file pos in instant structure.    */
   /*   If Buffer length is greater than the data leaves in the file, read   */
   /*   the rest of the data to buffer, set the END_OF_FILE flag on.         */
   /**************************************************************************/
   else {
      if ((pinstance->ulMisc1 - (ULONG)pinstance->lFileCurrentPosition) <= lBytes) {
         lBytes = (pinstance->ulMisc1 - (ULONG)pinstance->lFileCurrentPosition);
         pmtread->ulFlags |= MULTITRACKREAD_EOF;
         }

      lBytes = mmioRead(pinstance->hmmioFileHandle,pTemp,lBytes);
      pinstance->lFileCurrentPosition += lBytes;
      Counter += lBytes;
      pTemp = pumchHdr;
      }

   /**************************************************************************/
   /* Starts sorting frames into the corresponding tracks                    */
   /**************************************************************************/
   while (pumchHdr != NULL) {

      /***********************************************************************/
      /* Check for some bytes left at end of buffer. Not enough to form      */
      /* a chunk header and ushort track number. So must save them away in   */
      /* temp buffer for next time. (Boundary condition check)               */
      /***********************************************************************/
      if (((ULONG)pumchHdr + sizeof(UMCHUNKHEADER) + sizeof(USHORT)) >
          ((ULONG)pReadBuffer + ulReadBufferLen)) {

         if (rc = ffSavePartialFrame(pinstance,
                                     pmtread,
                                     ((ULONG)pReadBuffer + ulReadBufferLen) - (ULONG)pumchHdr,
                                     (PVOID)pumchHdr)) {
            pmmioinfo->ulErrorRet = rc;
            return(MMIO_ERROR);
            }
//-------return(MMIO_SUCCESS);
         break;
         }


      /* Check for chapter list chunk headers, ignore them */
      if ((pumchHdr->ulChunkID == FOURCC_LIST) &&
          ((pumchHdr+1)->ulChunkID == HEX_UMFORM_CHAPTERDATALIST)) {

         pTemp   = (PVOID) (sizeof(UMCHUNKHEADER) + SIZEOF_FORM + (ULONG)pTemp);
         pumchHdr = (PUMCHUNKHEADER) pTemp;
         BufferLengthCounter += sizeof(UMCHUNKHEADER)+SIZEOF_FORM;   /* implement counter by the chunk size. */
         }

      BufferLengthCounter += ffRoundChunkLength(pinstance,pumchHdr);   /* implement counter by the chunk size. */
      pusTrackNum = (PUSHORT)(pumchHdr+1);  /* Point at first ushort after chunk hdr */

      /* Find track structure */
      if (!(ptracki = ioFindTracki(pinstance,*pusTrackNum))) {
         pmmioinfo->ulErrorRet = MMIOERR_MATCH_NOT_FOUND;
         return(MMIO_ERROR);
         }


      /**********************************************************************/
      /* If a record is not completely read and stored in the Buffer, we    */
      /* don't return that record, and we want to copy the data to a        */
      /* Temp Buffer.                                                       */
      /**********************************************************************/
      if (BufferLengthCounter > ulReadBufferLen) {
         /*
          * if a record's length is > than buffer length, returns error.
          */
         if ((pumchHdr == pReadBuffer) && (ffRoundChunkLength(pinstance,pumchHdr) > ulReadBufferLen)) {
             pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
             return(MMIO_ERROR);
             }

//-------/*
//        * if first palette, returns without copying data to temp buffer.
//        */
//       if (IS_PALETTE_RECORD(precmap)) {
//          if (pinstance->ulFlags & HAVE_PALETTE) {            //##BILL - need to be on track basis!!
//             pinstance->ulFlags &= ~HAVE_PALETTE;
//     #no->   return(MMIO_SUCCESS);       (break;)                //FIX-READ_BYTES
//             }
//----------}

         if (rc = ffSavePartialFrame(pinstance,
                                     pmtread,
                                     (ffRoundChunkLength(pinstance,pumchHdr) -
                                      (BufferLengthCounter - ulReadBufferLen)),
                                     (PVOID)pumchHdr)) {
            pmmioinfo->ulErrorRet = rc;
            return(MMIO_ERROR);
            }
         break;
         }  /* end if BufferLengthCounter > */

      /**********************************************************************/
      /* Before setting pesrcbuftab->pRecord to a track:                    */
      /* 1/FindRecordMap matches usID, which is gotten from data, with      */
      /*   the recmaptable,if found, address of an entry in table will be   */
      /*   returned.                                                        */
      /* 2/FindTrackMapEntry matches track# of pTrackMapList, that is in    */
      /*   pmtread, with track# of precmap. If found, address               */
      /*   of the entry in pTrackMapList will be returned.                  */
      /* 3/The 1st and 2nd steps are successful, next we validate track,    */
      /*   make sure the sorted track that the IOProc supports.             */
      /**********************************************************************/
//----if (IS_PALETTE_RECORD(precmap)) {
//       if (pinstance->ulFlags & HAVE_PALETTE) {            //##BILL - need to be on track basis!!
//          fFirstPalette = TRUE;
//          fReturnFrame = FALSE;
//          pinstance->ulFlags &= ~HAVE_PALETTE;
//          }
//-------}
//----else if () {


      /*************************************************************/
      /* Check for SCAN Mode, return only Key Frames               */
      /*************************************************************/
      if (pinstance->ulMode == MODE_SCANREAD) {
         if (((PVIDEOFRAME)(pumchHdr+1))->ulFrameFlags == UM_VIDEO_FRAME_FLAG_DELTAFRAME) {
            fReturnFrame = FALSE;
            }
         }

      if (fReturnFrame) {
         if(ptrackmap = ioFindTrackMapEntry(pmtread->pTrackMapList,
                                            pmtread->ulNumTracks,
                                            *pusTrackNum))      {

            /*************************************************************/
            /* check paesrcbuftab did not overflow. This is the NOT DONE */
            /* condition that implies that no more records entries are   */
            /* available for this buffer.                                */
            /*************************************************************/
            if (ptrackmap->ulNumEntries < ptracki->ulNumEntries) {
               pesrcbuftab = (PESRCBUFTAB)ptrackmap->pRecordTabList;
               pesrcbuftab += ptrackmap->ulNumEntries;
               pesrcbuftab->pBuffer = pReadBuffer;
               pesrcbuftab->pRecord  = (PVOID)(pumchHdr+1);
               pesrcbuftab->ulLength = pumchHdr->ulChunkLength;

               if (!IS_VIDEO_TRACK(ptracki->ulMediaType)) {  /* Skip the track # if not video frame */
                  pesrcbuftab->pRecord  = (PVOID) ((ULONG)pesrcbuftab->pRecord + sizeof(USHORT));
                  pesrcbuftab->ulLength -= sizeof(USHORT);
                  }
               else if (ptrackiExtAudio) {                                                   /*EXTAUDIO*/
                  ulVidDuration += ((PVIDEOFRAME)pesrcbuftab->pRecord)->ulDuration;           /*EXTAUDIO*/
                  }                                                                          /*EXTAUDIO*/

               ptrackmap->ulNumEntries++;
               }

            else {
               pinstance->ulLastBufferOffset = BufferLengthCounter - ffRoundChunkLength(pinstance,pumchHdr);
               pmtread->ulFlags |= MULTITRACKREAD_NOTDONE;
               break;
               }
            }
         } /* end if FirstPalette. */

      else
         fReturnFrame = TRUE;     /* Reset after processing this frame */

      /**********************************************************************/
      /* if end of file, must set pumchHdr = NULL to exit loop.             */
      /**********************************************************************/
      if ((pinstance->lFileCurrentPosition >= (LONG)pinstance->ulMisc1) &&
         (BufferLengthCounter >= Counter)){
          pumchHdr = NULL;
          } /* end if */
      else{
          pTemp   = (PVOID)(ffRoundChunkLength(pinstance,pumchHdr) + (ULONG)pTemp);
          pumchHdr = (PUMCHUNKHEADER)pTemp;
          } /* end else */
      } /* End while loop */


//   /*******************************************************************************************/
//   /* Read some audio into the buffer and set the first record in this trackmap to the length */
//   /*******************************************************************************************/
//   if (ptrackiExtAudio) {                                                   /*EXTAUDIO*/
//      ULONG       ulTemp;                                                   /*EXTAUDIO*/
//
//      /* Compute # bytes for each MMTIME unit */                            /*EXTAUDIO*/
//      ulTemp = pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBitsPerSample / 8;   /*EXTAUDIO*/
//      ulTemp *= pmmAudioHdr->mmXWAVHeader.WAVEHeader.ulSamplesPerSec;      /*EXTAUDIO*/
//      ulTemp /= 3000;       /* # bytes/MMTIME unit */                      /*EXTAUDIO*/
//      ulTemp *= ulVidDuration;                                             /*EXTAUDIO*/
//      ulTemp += (ulTemp % pmmAudioHdr->mmXWAVHeader.WAVEHeader.usBlockAlign); /*EXTAUDIO*/
//
//      if (ulTemp >= ulExtAudBufLen) { /* too much audio, remember leftover */ /*EXTAUDIO*/
//         ptrackiExtAudio->ulMisc1 += (ulTemp - ulExtAudBufLen);            /*EXTAUDIO*/
//         ulTemp = ulExtAudBufLen;                                          /*EXTAUDIO*/
//         }                                                                  /*EXTAUDIO*/
//      else { /* Too little audio, check for any leftover audio from previous buffers */ /*EXTAUDIO*/
//         lBytes = ulExtAudBufLen - ulTemp;      /* leftover amount */      /*EXTAUDIO*/
//
//         if (lBytes >= ptrackiExtAudio->ulMisc1) {  /* Take what is there */ /*EXTAUDIO*/
//            ulTemp += ptrackiExtAudio->ulMisc1;                            /*EXTAUDIO*/
//            ptrackiExtAudio->ulMisc1 = 0L;                                  /*EXTAUDIO*/
//            }
//         else { /* Take what I can, leave the rest */
//            ulTemp += lBytes;                                              /*EXTAUDIO*/
//            ptrackiExtAudio->ulMisc1 -= lBytes;                             /*EXTAUDIO*/
//            }                                                               /*EXTAUDIO*/
//
//         ptrackiExtAudio->ulMisc1 -= (ulExtAudBufLen - ulTemp);            /*EXTAUDIO*/
//         ulTemp = ulExtAudBufLen;                                          /*EXTAUDIO*/
//         }                                                                  /*EXTAUDIO*/
//
//      /* Read audio data into buffer */                                     /*EXTAUDIO*/
//      rc = mmioRead(ptracki->hmmioTrackFileHandle,                          /*EXTAUDIO*/
//                    (PVOID)((ULONG)pReadBuffer+ulReadBufferLen),            /*EXTAUDIO*/
//                    ulTemp);                                               /*EXTAUDIO*/
//      if (rc < ulTemp)  /* Reset EOF, if more External Audio to read */    /*EXTAUDIO*/
//         pmtread->ulFlags &= ~MULTITRACKREAD_EOF;                          /*EXTAUDIO*/
//
//      /* Find the trackmap table for this track - We know it exists */      /*EXTAUDIO*/
//      ptrackmap = ioFindTrackMapEntry(pmtread->pTrackMapList,              /*EXTAUDIO*/
//                                      pmtread->ulNumTracks,                  /*EXTAUDIO*/
//                                      ptrackiExtAudio->ulTrackID);            /*EXTAUDIO*/
//                                                                            /*EXTAUDIO*/
//      /* Fill in record information for this track */                       /*EXTAUDIO*/
//      pesrcbuftab = (PESRCBUFTAB)ptrackmap->pRecordTabList;  /* always use the first entry */ /*EXTAUDIO*/
//      pesrcbuftab->pBuffer = pReadBuffer;                                    /*EXTAUDIO*/
//      pesrcbuftab->pRecord  = (PVOID)((ULONG)pReadBuffer+ulReadBufferLen);   /*EXTAUDIO*/
//      pesrcbuftab->ulLength = ulTemp;                                       /*EXTAUDIO*/
//      ptrackmap->ulNumEntries++;                                            /*EXTAUDIO*/
//      }                                                                     /*EXTAUDIO*/


   /**************************************************************************/
   /* Reset READINTERLEAVED_NOTDONE condition if EOF                         */
   /**************************************************************************/
   lBytes = Counter;                                              //FIX-READ_BYTES
//-lBytes = pinstance->ulLastBufferOffset;                        //FIX-READ_BYTES
   if (pmtread->ulFlags & MULTITRACKREAD_EOF) {
      pinstance->ulLastBufferOffset = (ULONG)0L;
      pmtread->ulFlags &= ~MULTITRACKREAD_NOTDONE;
      }

   return(MMIO_SUCCESS);
}

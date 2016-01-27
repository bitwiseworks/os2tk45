/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOCODEC.C                                           */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc codec interfaces                 */
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
/*     ioDetermineCodec                                                   */
/*     ioLoadCodecDLL                                                     */
/*     ioLoadCodec                                                        */
/*     ioFindCodec                                                        */
/*     ioCloseCodec                                                       */
/*     ioInitCodecopen                                                    */
/*     ioAssociateCodec                                                   */
/*                                                                        */
/* DATE      DEVELOPER   DEFECT  CHANGE DESCRIPTION           CHANGE FLAG */
/* ---       ---------   ------  ------------------------     ----------- */
/* 07/29/93  DJ Dobkin   9983    Associate codec to new file  @D01        */
/* 08-02-93  LL Post     9744    ioAssociateCodecPlay and                 */
/*                               ioDetermineCodecPlay (new                */
/*                               functions)                               */
/* 04-20-94  LL Post     12236   Add MM_DosLoadModule for speed @LP01     */
/* 05.31.94  WL Sharp    12384   Integrate Compressed Audio Support       */
/* 06.22.94  WL Sharp    12757   Add MPEG Hardware Playback Support @W1   */
/* 08.29.94  W. Sharp    13674   DIVE fixes for I-frame healing           */
/* 09.15.94  WL Sharp    14079   Default to comprssn if codec does both   */
/* 09.20.94  LL Post     14079   Dont in case of audio compression @LP02  */
/* 09.23.94  WL Sharp    14235   Fix RT record of MONDO                   */
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
#include        <mmdsload.h>               /* MM_DosLoadModule defines @LP01 */


   extern HHUGEHEAP hheap;                        /* Heap of memory.        */
   extern CODECINIFILEINFO acifiTable[];
   extern ULONG    ulNumColors;                   /* added to correct multiinstance problem */
   extern HMTX hmtxGlobalHeap;


LONG ioCheckCodecList(PINSTANCE pinstance)

{

//
// Check for existance of pccbList and create it if it doesn't exist
// Is there a better place to do this???
// WLS 3.15.94
//

   if (!pinstance->pccbList) {
      pinstance->pccbList = HhpAllocMem(hheap, pinstance->pmmhdr->ulNumEntries * sizeof (PCCB));
      if (!pinstance->pccbList) {
         return(MMIOERR_OUTOFMEMORY);
         }
      }

   return(MMIO_SUCCESS);
}

LONG ioDetermineBitCount(PCODECINIFILEINFO pcifi)

{
   HPS               hps;                /* only used to query color support */
   HAB               hab;                /* anchor block */

//
//WLS 3.15.94 - Don't get color stuff for Audio Codec
//There should be a better way to determine the color depth needed!!!!!!
//

   if ((pcifi->ulCapsFlags & CODEC_DECOMPRESS) && (pcifi->ulMediaType != MMIO_MEDIATYPE_AUDIO)) {
      /* Query the display mode */
      if (ulNumColors == 0) {       /* Get this info once per process */
         hab  = WinInitialize(0);

         hps  = WinGetPS(HWND_DESKTOP);
         DevQueryCaps ( GpiQueryDevice(hps),
                        CAPS_COLORS,
                        1L,
                        (PLONG)&ulNumColors);

         WinReleasePS (hps);
         WinTerminate (hab);
         }

      /* Set the color depth for the CODEC we want */
      if (ulNumColors == 16)
         pcifi->ulCapsFlags |= CODEC_4_BIT_COLOR;
      else if (ulNumColors == 65536)
         pcifi->ulCapsFlags |= CODEC_16_BIT_COLOR;
      else if (ulNumColors == 16777216)
         pcifi->ulCapsFlags |= CODEC_24_BIT_COLOR;
      else  /* 256 and anything else */
         pcifi->ulCapsFlags |= CODEC_8_BIT_COLOR;
      }

   return(MMIO_SUCCESS);
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioDetermineCodec                                        */
/*                                                                          */
/* DESCRIPTIVE NAME: Determine which codec should be loaded for this        */
/*                   FOURCC, COMPRESSTYPE, CAPSFLAGS, ColorDepth,           */
/*                   COMPRESS or DECOMPRESS.                                */
/*                                                                          */
/* FUNCTION: This function Determines the CODEC to load.                    */
/*                                                                          */
/* NOTES: Picks the default if there is one.                                */
/*                                                                          */
/* ENTRY POINT: ioDetermineCodec                                            */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PINSTANCE  pinstance                                        */
/*              ULONG      ulSearchFlags                                    */
/*              PCODECINIFILEINFO pcifi                                     */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              pcifi  contains found codec info                            */
/*              rc = MMIO_SUCCESS                                           */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*              MMIOERR_CODEC_NOT_SUPPORTED                                 */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ioDetermineCodec ( PINSTANCE pinstance,
                        ULONG ulSearchFlags,
                        PCODECINIFILEINFO pcifi )

{
   LONG              rc = MMIO_SUCCESS;  /* Return code of IOProc's call. */
   USHORT            i;                  /* Loop index.       */
   ULONG             ulFlags = 0;
   ULONG             ulExtraFlags = 0;

// ADD EXTRA SEARCH FLAGS IF VALUES ARE NOT EMPTY

   if (strcmp(pcifi->szDLLName, "")) {
      ulExtraFlags |= MMIO_MATCHDLL;
      }

   if (strcmp(pcifi->szProcName, "")) {
      ulExtraFlags |= MMIO_MATCHPROCEDURENAME;
      }

   /***************************************************************************/
   /* Search for the DEFAULT codec of this type from the MMIO INI file        */
   /***************************************************************************/
   /***************************************************************************/
   /* If no default, find first one and use it from the MMIO INI file         */
   /***************************************************************************/

   pcifi->ulCapsFlags |= CODEC_DEFAULT;   /* Pick default */
   for (i = 0; i < 2; i++) {

      ulFlags = ulSearchFlags | ulExtraFlags |
                 MMIO_MATCHFOURCC |
                 MMIO_MATCHCOMPRESSTYPE |
                 MMIO_MATCHCAPSFLAGS |
                 MMIO_MATCHFIRST |
                 MMIO_FINDPROC;

      if (!(rc = mmioIniFileCODEC(pcifi,ulFlags))) {
         return(MMIO_SUCCESS);
         }

      pcifi->ulCapsFlags &= ~CODEC_DEFAULT;
      }

//
// LESSEN THE SEARCH - DON'T INCLUDE DLLNAME OR PROCNAME
//

   /***************************************************************************/
   /* Search for the DEFAULT codec of this type from the MMIO INI file        */
   /***************************************************************************/
   /***************************************************************************/
   /* If no default, find first one and use it from the MMIO INI file         */
   /***************************************************************************/

   pcifi->ulCapsFlags |= CODEC_DEFAULT;   /* Pick default */
   for (i = 0; i < 2; i++) {
      ulFlags = ulSearchFlags |
                MMIO_MATCHFOURCC |
                MMIO_MATCHCOMPRESSTYPE |
                MMIO_MATCHCAPSFLAGS |
                MMIO_MATCHFIRST |
                MMIO_FINDPROC;

      if (!(rc = mmioIniFileCODEC(pcifi,ulFlags))) {
         return(MMIO_SUCCESS);
         }

      pcifi->ulCapsFlags &= ~CODEC_DEFAULT;
      }

   /***************************************************************************/
   /* Search any internal CODEC tables for the necessary CODEC to load.       */
   /* Note: This is used for debugging new CODEC's that have not been added   */
   /*       to the mmpmmmio.ini file.                                         */
   /***************************************************************************/
   for (i = 0; i < NUMBER_CODEC_TABLE; i++) {

      if ((acifiTable[i].ulCompressType == pcifi->ulCompressType) &&
          ((acifiTable[i].ulCapsFlags & pcifi->ulCapsFlags) == pcifi->ulCapsFlags)) {

         *pcifi = acifiTable[i];         /* Copy contents */
         return(MMIO_SUCCESS);
         }
      }


   return(MMIOERR_CODEC_NOT_SUPPORTED);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioLoadCodecDLL                                         */
/*                                                                          */
/* DESCRIPTIVE NAME: Load a CODEC IO Proc and add it the PCCB list          */
/*                                                                          */
/* FUNCTION: This function loads a CODEC IO Proc and adds it to the linked  */
/*           list of CODECs currently loaded for this movie instance.       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ioLoadCodecDLL                                              */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*             PINSTANCE pinstance     - Instant structure.                 */
/*             PCODECINIFILEINFO pcifi - CODEC ini file information         */
/*             PULONG phCodec          - Returns hCodec of any sibling codec*/
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              pccb                                                        */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
PCCB ioLoadCodecDLL ( PINSTANCE pinstance,
                      PCODECINIFILEINFO pcifi,
                      PULONG phCodec,
                      ULONG ulTrack )

{
   LONG      rc = MMIO_SUCCESS;           /* Return code of IOProc's call. */
   SZ        szLoadError[260];            /* Error returns.          */
   PCCB      pccbNew;
   PCCB      pccb;
   HMODULE   hmod = 0L;
   PMMIOPROC pmmioproc;
   ULONG     hCodec = 0L;
   ULONG     ulCount;

   ioCheckCodecList(pinstance);

   /**************************************************************************/
   /* Search if the CCB entry has been created for the passed in             */
   /* pszDLLName and pszProcName,if yes, then sets pccb    pointer of        */
   /* ptracki to that node.(different track may share a same CCB)            */
   /**************************************************************************/
   for (ulCount = 0; ulCount < pinstance->pmmhdr->ulNumEntries; ulCount++) {
      for (pccb = pinstance->pccbList[ulCount]; pccb; pccb = pccb->pccbNext) {

         if (!stricmp(pcifi->szDLLName,pccb->cifi.szDLLName)) {
            hCodec = pccb->hCodec;

            if (!stricmp(pcifi->szProcName,pccb->cifi.szProcName)) {
               /* Proc entry names match */
               return(pccb);
               }
            }
         } /* end loop */
      }

   /**************************************************************************/
   /* the above searching can't find the DCIO node, if a same DLLName was    */
   /* found, query IOProc address directly, else load module then query I/O  */
   /* address before allocates a new pccb node.                              */
   /**************************************************************************/
   if (MM_DosLoadModule(szLoadError,                         //@LP01
                     (ULONG)sizeof(szLoadError),
                     pcifi->szDLLName,
                     &hmod))   {
      /* Load Error - MMIOERR_INVALID_DLLNAME */
      return(NULL);
      }

   if (DosQueryProcAddr(hmod,
                        0L,
                        pcifi->szProcName,
                        (PFN *)&pmmioproc))   {
      /* Query Error - MMIOERR_INVALID_PROCEDURENAME */
      return(NULL);
      }

   /**************************************************************************/
   /* The above loading and querying was successful, then create a new node  */
   /* for the DCIO. If HhpAllocMem fails, call DosFreeModule to free DCIO    */
   /* module before returning error.                                         */
   /**************************************************************************/
   if (ENTERCRIT(rc)) {
      return(NULL);
      }

   pccbNew = (PCCB)HhpAllocMem(hheap,(ULONG)sizeof(CCB));

   EXITCRIT;

   if(!pccbNew) {
      DosFreeModule(hmod);
      /* Allocate error - MMIOERR_OUTOFMEMORY */
      return(NULL);
      }

   /**************************************************************************/
   /* Assigns the Decompress IOProc information, which is in record map      */
   /* table, to the new DCIO node.                                           */
   /**************************************************************************/
   pccbNew->cifi = *pcifi;
   pccbNew->hmodule = hmod;
   pccbNew->pmmioproc = pmmioproc;
   pccbNew->hCodec = 0L;
   pccbNew->ulLastSrcBuf = 0L;
   pccbNew->ulMisc1 = 0L;
   pccbNew->ulMisc2 = 0L;

   /**************************************************************************/
   /* adds new node to the beginning of ccb list.                            */
   /**************************************************************************/
   pccbNew->pccbNext = pinstance->pccbList[ulTrack];
   pinstance->pccbList[ulTrack] = pccbNew;

   *phCodec = hCodec;
   return(pccbNew);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioLoadCodec                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: Load a CODEC IO Proc and add it the PCCB list          */
/*                                                                          */
/* FUNCTION: This function loads a CODEC IO Proc and adds it to the linked  */
/*           list of CODECs currently loaded for this movie instance.       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ioLoadCodec                                                 */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*             PINSTANCE pinstance     - Instant structure.                 */
/*             PTRACKI ptracki         - Track specific information         */
/*             PCODECINIFILEINFO pcifi - CODEC ini file information         */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              pccb                                                        */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              0L                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
PCCB ioLoadCodec ( PMMIOINFO pmmioinfo,         //@W1
                   PINSTANCE pinstance,
                   PTRACKI ptracki,
                   PCODECINIFILEINFO pcifi )

{
   LONG      rc = MMIO_SUCCESS;           /* Return code of IOProc's call. */
   PCCB      pccbNew;
   ULONG     hCodec = 0L;
   ULONG     ulTrack = ptracki->ulTrackID;

   ioCheckCodecList(pinstance);

   if (pccbNew = ioLoadCodecDLL(pinstance,pcifi,&hCodec, ulTrack)) {

      /**************************************************************************/
      /* Open the Codec IO Proc and save the hCodec in the pccb structure       */
      /**************************************************************************/
      if (rc = ffOpenCodec(pmmioinfo, pinstance, pccbNew, hCodec, ptracki)) {  //@W1

         pinstance->pccbList[ulTrack] = pccbNew->pccbNext;   /* unlink from list */
         ioCloseCodec(pccbNew);
         pccbNew = NULL;  /* return error condition */
         }
      }

   return(pccbNew);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioFindCodec                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:                                                        */
/*                                                                          */
/* FUNCTION: This function finds a compression/decompression control block  */
/*           for this compression type.                                     */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioFindCodec                                                 */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PINSTANCE   pinstance  - Movie instance structure              */
/*           ULONG       ulCompressType - Compression type                  */
/*                                                                          */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              pccb                                                        */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              NULL - 0L                                                   */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
PCCB ioFindCodec ( PINSTANCE pinstance,
                   ULONG ulCompressType )

{
   PCCB   pccb;
   ULONG  ulCount;

   ioCheckCodecList(pinstance);

   for (ulCount = 0; ulCount < pinstance->pmmhdr->ulNumEntries; ulCount++) {
      for (pccb = pinstance->pccbList[ulCount]; pccb; pccb = pccb->pccbNext) {
         if (pccb->cifi.ulCompressType == ulCompressType)
            return(pccb);
         }
      }
   return(NULL);  /* not found */
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME:  ioCloseCodec                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:                                                        */
/*                                                                          */
/* FUNCTION: This function Closes a codec instance for a movie instance.    */
/*           This is called upon a unrecoverable error or on movie close.   */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioCloseCodec                                                */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PCCB        pccb       - Pointer to codec control structure.   */
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
LONG ioCloseCodec ( PCCB pccb )

{
   LONG       rc = MMIO_SUCCESS;           /* Return code of IOProc's call. */

   ENTERCRITX;
   if (pccb->codecopen.pSrcHdr) {
      HhpFreeMem(hheap,(PVOID)pccb->codecopen.pSrcHdr);
      }

   if (pccb->codecopen.pDstHdr) {
      HhpFreeMem(hheap,(PVOID)pccb->codecopen.pDstHdr);
      }

   if (pccb->codecopen.pControlHdr) {
      HhpFreeMem(hheap,(PVOID)pccb->codecopen.pControlHdr);
      }

   if (pccb->codecopen.pOtherInfo) {
      HhpFreeMem(hheap,(PVOID)pccb->codecopen.pOtherInfo);
      }

   if (pccb->hCodec) {
      rc = pccb->pmmioproc(&pccb->hCodec,
                           MMIOM_CODEC_CLOSE,
                           0L,
                           0L);

      if (!rc) {
         pccb->hCodec = 0L;
         }
      }

   if (pccb->hmodule) {
//----DosFreeModule(pccb->hmodule);
      pccb->hmodule = 0;
      }

   HhpFreeMem(hheap,(PVOID)pccb);
   pccb = NULL;
   EXITCRIT;

   return(rc);
}


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioInitCodecopen                                         */
/*                                                                          */
/* DESCRIPTIVE NAME: Allocate and initialize a CODECOPEN structure to be    */
/*                   saved in the CCB. Copy info from input CODECOPEN.      */
/*                                                                          */
/* FUNCTION: This function allocates a CODECOPEN structure.                 */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioInitCodecopen                                             */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PCCB          pccb                                          */
/*              PCODECOPEN    pcodecopen                                    */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              rc = MMIO_SUCCESS                                           */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ioInitCodecopen ( PCCB pccb,
                       PCODECOPEN pcodecopen)

{
   ULONG             ulSize;


   ENTERCRITX;
   pccb->codecopen.ulFlags = pcodecopen->ulFlags;

   /* Create and copy Pointers to structures in CODECOPEN structure */
   if (pcodecopen->pControlHdr) {
      ulSize = *((PULONG)pcodecopen->pControlHdr);
      if (!(pccb->codecopen.pControlHdr = (PVOID)HhpAllocMem(hheap,ulSize))) {
         return(MMIO_ERROR);
         }
      memcpy(pccb->codecopen.pControlHdr, pcodecopen->pControlHdr, ulSize);
      }

   if (pcodecopen->pSrcHdr) {
      ulSize = *((PULONG)pcodecopen->pSrcHdr);
      if (!(pccb->codecopen.pSrcHdr = (PVOID)HhpAllocMem(hheap,ulSize))) {
         return(MMIO_ERROR);
         }
      memcpy(pccb->codecopen.pSrcHdr, pcodecopen->pSrcHdr, ulSize);
      }

   if (pcodecopen->pDstHdr) {
      ulSize = *((PULONG)pcodecopen->pDstHdr);
      if (!(pccb->codecopen.pDstHdr = (PVOID)HhpAllocMem(hheap,ulSize))) {
         return(MMIO_ERROR);
         }
      memcpy(pccb->codecopen.pDstHdr, pcodecopen->pDstHdr, ulSize);
      }

   if (pcodecopen->pOtherInfo) {
      ulSize = *((PULONG)pcodecopen->pOtherInfo);
      if (!(pccb->codecopen.pOtherInfo = (PVOID)HhpAllocMem(hheap,ulSize))) {
         return(MMIO_ERROR);
         }
      memcpy(pccb->codecopen.pOtherInfo, pcodecopen->pOtherInfo, ulSize);
      }

   EXITCRIT;
   return(MMIO_SUCCESS);
}



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioAssociateCodec                                        */
/*                                                                          */
/* DESCRIPTIVE NAME: Associate CODECs with a file(track). Use this codec    */
/*                   to compress frames to be written to a file.            */
/*                                                                          */
/* FUNCTION: This function associates a CODEC with a file (track).          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioAssociateCodec                                            */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PINSTANCE  pinstance                                        */
/*              PMMEXTENDEDINFO pmmextendedinfo                             */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              rc = MMIO_SUCCESS                                           */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*              Codec is added to CCB link-list and the codecs are opened.  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/


#ifdef MMDEBUG
int print_codec_structure(PCODECOPEN pco)

{
   if (pco->pControlHdr) {
      printf("pControlHdr (CODEC SPECIFIC INFO) : %0x%x\n", pco->pControlHdr);
      }

   if (pco->pSrcHdr) {
      PCODECVIDEOHEADER pcoHdr = (PCODECVIDEOHEADER) (pco->pSrcHdr);
      printf("pSrcHdr (CODECVIDEOHEADER)\n");
      printf("ulStructLen     : %d\n",   pcoHdr->ulStructLen     );
      printf("cx              : %d\n",   pcoHdr->cx              );
      printf("cy              : %d\n",   pcoHdr->cy              );
      printf("cPlanes         : %d\n",   pcoHdr->cPlanes         );
      printf("cBitCount       : %d\n",   pcoHdr->cBitCount       );
      printf("ulColorEncoding : 0x%x\n", pcoHdr->ulColorEncoding );
      printf("genpal\n");
      printf("   ulStartIndex: %d\n",   pcoHdr->genpal.ulStartIndex);
      printf("   ulNumColors : %d\n",   pcoHdr->genpal.ulNumColors);
      printf("   prgb2Entries: 0x%x\n", pcoHdr->genpal.prgb2Entries);
      printf("\n");
      }

   if (pco->pDstHdr) {
      PCODECVIDEOHEADER pcoHdr = (PCODECVIDEOHEADER) (pco->pDstHdr);
      printf("pDstHdr (CODECVIDEOHEADER)\n");
      printf("ulStructLen     : %d\n",   pcoHdr->ulStructLen     );
      printf("cx              : %d\n",   pcoHdr->cx              );
      printf("cy              : %d\n",   pcoHdr->cy              );
      printf("cPlanes         : %d\n",   pcoHdr->cPlanes         );
      printf("cBitCount       : %d\n",   pcoHdr->cBitCount       );
      printf("ulColorEncoding : 0x%x\n", pcoHdr->ulColorEncoding );
      printf("genpal          : 0x%x\n", pcoHdr->genpal          );
      printf("genpal\n");
      printf("   ulStartIndex: %d\n",   pcoHdr->genpal.ulStartIndex);
      printf("   ulNumColors : %d\n",   pcoHdr->genpal.ulNumColors);
      printf("   prgb2Entries: 0x%x\n", pcoHdr->genpal.prgb2Entries);
      printf("\n");
      }

   if (pco->pOtherInfo) {
      PMMVIDEOOPEN mmvo = (PMMVIDEOOPEN) pco->pOtherInfo;
      printf("pOtherInfo (MMVIDEOOPEN)\n");
      printf("ulStructLen          : %d\n", mmvo->ulStructLen          );
      printf("ulQuality            : %d\n", mmvo->ulQuality            );
      printf("ulKeyFrameRate       : %d\n", mmvo->ulKeyFrameRate       );
      printf("ulScale              : %d\n", mmvo->ulScale              );
      printf("ulRate               : %d\n", mmvo->ulRate               );
      printf("ulDataConstraint     : %d\n", mmvo->ulDataConstraint     );
      printf("ulConstraintInterval : %d\n", mmvo->ulConstraintInterval );
      printf("\n");
      }
}
#endif


#define CODEC_BIT_FLAGS ~(CODEC_4_BIT_COLOR | CODEC_16_BIT_COLOR | \
                          CODEC_8_BIT_COLOR | CODEC_24_BIT_COLOR)

LONG ioAssociateCodec ( PMMIOINFO pmmioinfo,
                        PINSTANCE pinstance,
                        PCODECASSOC pcodecassoc,
                        ULONG fUseDefaultBitCount)

{
   PCODECINIFILEINFO  pcifi = pcodecassoc->pCODECIniFileInfo;
   PCODECOPEN         pco;
   PCODECVIDEOHEADER  pcoDstHdr;
   PCODECVIDEOHEADER  pcoSrcHdr;
   PMMVIDEOHEADER     pmmVidHdr;
   PMMVIDEOOPEN       pmmVidOpn;
   PTRACKI            ptracki;
   LONG   rc = MMIO_SUCCESS;  /* Return code of IOProc's call. */
   PCCB   pccb;
   ULONG  hCodec;  /* Possibly returned from ioLoadCodecDLL */
   ULONG  ulType = 0;
   ULONG  ulTrack = pinstance->lCurrentTrack;
   ULONG  ulFPS;

   ioCheckCodecList(pinstance);

   /* Check for NULL pointers */
   if (!pcodecassoc->pCodecOpen || !pcodecassoc->pCODECIniFileInfo) {
      return (MMIOERR_INVALID_PARAMETER);
      }

   /* Force the correct values into the codecinfileinfo structure */
   pcifi->ulStructLen = sizeof(CODECINIFILEINFO);
// Can't do for WAVE stuff
   if (!pcifi->fcc)
      pcifi->fcc = pmmioinfo->fccIOProc;


//
// If we are associating a new compressor, we must update the header
// information in the file and load the codec based upon the colorspace
// defined in the CAPSFLAG.  If we are opening up a decompressor, we don't
// need to modify the file at all.  If no CAPSFLAGs are defined for the
// bitcount, we will use the bit count from the Destination Header.
//

// if the codec can do both, default to compression - Mondo, Indeo 2.1

//   if (pcifi->ulCapsFlags & CODEC_DECOMPRESS) {
//   if (!(pcifi->ulCapsFlags & CODEC_COMPRESS)) {
   if ((!(pcifi->ulCapsFlags & CODEC_COMPRESS)) && (pcifi->ulCapsFlags & CODEC_DECOMPRESS)) { //14235
      ulType = CODEC_DECOMPRESS;
      pcifi->ulCapsFlags = CODEC_DECOMPRESS;
      if (fUseDefaultBitCount) {
         ioDetermineBitCount(pcifi);
         }
      else {

         if (pcifi->ulMediaType != MMIO_MEDIATYPE_AUDIO) {

            pco       = (PCODECOPEN)        (pcodecassoc->pCodecOpen);
            pcoDstHdr = (PCODECVIDEOHEADER) (pco->pDstHdr);
            pcoSrcHdr = (PCODECVIDEOHEADER) (pco->pSrcHdr);

//////////////////////////////////////////////////////////////
////////HACK HACK HACK HACK - RECORDER HACK

            if (pcoDstHdr->genpal.ulNumColors == 65536) {
#ifdef MMDEBUG
               if (pcoDstHdr->cBitCount != 16)
                  printf("HACK : Change Dst->cBitCount from %d to 16\n", pcoDstHdr->cBitCount);
#endif
               pcoDstHdr->cBitCount = 16;
               }

            if (pcoSrcHdr->genpal.ulNumColors == 65536) {
#ifdef MMDEBUG
               if (pcoSrcHdr->cBitCount != 16)
                  printf("HACK : Change Src->cBitCount from %d to 16\n", pcoSrcHdr->cBitCount);
#endif
               pcoSrcHdr->cBitCount = 16;
               }

////////DON'T BLAME ME - NOT MY FAULT!!
//////////////////////////////////////////////////////////////

//
// This limits the CAPSFLAG search to just a decompressor with a certain
// pixel depth.  None of the other flags are used during this search. If
// the user passes in a DLL name or a Proc name, that will be used during
// the first search.  If the first search fails, DLL name and Proc name
// will not be used.  (See ioDetermineCodec in this module)
//

            switch(pcoDstHdr->cBitCount) {
               case 4:
#ifdef VERBOSE
   if (!(pcifi->ulCapsFlags & CODEC_4_BIT_COLOR)) {
      printf("DstHdr->cBitCount does not match ulCapsFlags (4)\n");
      }
#endif
                  pcifi->ulCapsFlags |= CODEC_4_BIT_COLOR;
                  break;
               case 16:
#ifdef VERBOSE
   if (!(pcifi->ulCapsFlags & CODEC_16_BIT_COLOR)) {
      printf("DstHdr->cBitCount does not match ulCapsFlags (16)\n");
      }
#endif
                  pcifi->ulCapsFlags |= CODEC_16_BIT_COLOR;
                  break;
               case 24:
#ifdef VERBOSE
   if (!(pcifi->ulCapsFlags & CODEC_24_BIT_COLOR)) {
      printf("DstHdr->cBitCount does not match ulCapsFlags (24)\n");
      }
#endif
                  pcifi->ulCapsFlags |= CODEC_24_BIT_COLOR;
                  break;
               default:
#ifdef VERBOSE
   if (!(pcifi->ulCapsFlags & CODEC_8_BIT_COLOR)) {
      printf("DstHdr->cBitCount does not match ulCapsFlags (8)\n");
      }
#endif
                  pcifi->ulCapsFlags |= CODEC_8_BIT_COLOR;
                  break;
               }
            }
         }
      }
   else { // if its not a decompressor, it better be a compressor
#ifdef MMDEBUG
   printf("ioAssociateCodec : Compressor\n");
#endif
      ulType = CODEC_COMPRESS; // save flag in case it is overwritten

      /* Don't allow a COMPRESSOR association for read only files */
      if ((pinstance->ulFlags & OPENED_READONLY) &&
          (!(pcifi->ulFlags & CODEC_DECOMPRESS)) &&
          (pcifi->ulFlags & CODEC_COMPRESS)) {
         return(MMIOERR_INVALID_PARAMETER);
         }

//////////////////////////////////////////////////////////////
////////HACK HACK HACK HACK - RECORDER HACK

      if (pcifi->ulMediaType != MMIO_MEDIATYPE_AUDIO) {
         pco       = (PCODECOPEN)        (pcodecassoc->pCodecOpen);
         pmmVidOpn = (PMMVIDEOOPEN)      (pco->pOtherInfo);
         pcoDstHdr = (PCODECVIDEOHEADER) (pco->pDstHdr);
         pcoSrcHdr = (PCODECVIDEOHEADER) (pco->pSrcHdr);

#ifdef MMDEBUG
         print_codec_structure(pco);
#endif

         if (pmmVidOpn) {
            if (pmmVidOpn->ulDataConstraint) {
               ptracki = ioFindTracki (pinstance, ulTrack);
               pmmVidHdr = ptracki->pTrackHeader;
               ulFPS = (ULONG) (((float) (pmmVidHdr->ulRate) /
                                 (float) (pmmVidHdr->ulScale)) + 0.5);

               if (!pmmVidOpn->ulConstraintInterval) {
#ifdef MMDEBUG
                  printf("HACK : Change pmmVidOpn->ulConstraintInterval to %d\n", ulFPS);
#endif
                  pmmVidOpn->ulConstraintInterval = ulFPS;
                  }
               if (!pmmVidOpn->ulRate) {
#ifdef MMDEBUG
                  printf("HACK : Change pmmVidOpn->ulRate to %d\n", ulFPS);
#endif
                  pmmVidOpn->ulRate  = ulFPS;
                  }
               }
            }

         if (pcoDstHdr->genpal.ulNumColors == 65536) {
#ifdef MMDEBUG
            if (pcoDstHdr->cBitCount != 16)
               printf("HACK : Change Dst->cBitCount from %d to 16\n", pcoDstHdr->cBitCount);
#endif
            pcoDstHdr->cBitCount = 16;
            }

         if (pcoSrcHdr->genpal.ulNumColors == 65536) {
#ifdef MMDEBUG
            if (pcoSrcHdr->cBitCount != 16)
               printf("HACK : Change Src->cBitCount from %d to 16\n", pcoSrcHdr->cBitCount);
            pcoSrcHdr->cBitCount = 16;
#endif
            }
         }
////////DON'T BLAME ME - NOT MY FAULT!!
//////////////////////////////////////////////////////////////

      }


   /* Find the codec to load */
   if (rc = ioDetermineCodec(pinstance, 0, pcifi)) {
      return(rc); /* return error */
      }

   else { /* load and open the compression codec */

      /***********************************************/
      /* Check for previously installed codecs.      */
      /* de-installed any loaded, load new one       */
      /* allows only 1 codec to be loaded at a time  */
      /***********************************************/
      if (pinstance->pccbList[ulTrack]) {
         pccb = pinstance->pccbList[ulTrack];
         pinstance->pccbList[ulTrack] = pccb->pccbNext;   /* unlink from list */
         ioCloseCodec(pccb);
         }

      /* Load the codec dll */
      if (pccb = ioLoadCodecDLL(pinstance,
                                pcifi,
                                &hCodec, ulTrack)) {

/////////if (ulType == CODEC_DECOMPRESS) {
         if ((ulType == CODEC_DECOMPRESS) &&
             (pcifi->ulMediaType != MMIO_MEDIATYPE_AUDIO)){      //@LP02
            /* Save the codec open information in the ccb */
            ((PCODECOPEN)pcodecassoc->pCodecOpen)->ulFlags =
                           pcodecassoc->pCODECIniFileInfo->ulCapsFlags & ~CODEC_COMPRESS;

// don't let COMPRESS flag be set if decompress - indeo.dll doesn't like it!?!
            }
         else { /* Force open of compressor */
           ((PCODECOPEN)pcodecassoc->pCodecOpen)->ulFlags |= CODEC_COMPRESS;
            }

         if (!(rc = ioInitCodecopen(pccb,(PCODECOPEN)pcodecassoc->pCodecOpen))) {

            /* Open the codec */
            if (!(rc = pccb->pmmioproc(&hCodec,
                                       MMIOM_CODEC_OPEN,
                                       (LONG)&pccb->codecopen,
                                       0L)))  {
               pccb->hCodec = hCodec;       /* save handle to codec */
               }
            }

         /* handle error conditions */
         if (rc) {
            pinstance->pccbList[ulTrack] = pccb->pccbNext;   /* unlink from list */
            ioCloseCodec(pccb);
            }
         }
      else {
         rc = MMIO_ERROR;
         }
      }


   if (rc == MMIO_SUCCESS) {
      if (ulType == CODEC_COMPRESS) {
         pcodecassoc->pCodecOpen = (PVOID) &pccb->codecopen;
         rc = ffAssociateCodec(pinstance, pcodecassoc);
         }
      }

   return(rc);
}




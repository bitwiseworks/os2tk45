/**************************START OF SPECIFICATIONS **************************/
/*                                                                          */
/* SOURCE FILE NAME:  FF.H                                                  */
/*                                                                          */
/* DESCRIPTIVE NAME: Ultimotion specific IO Proc defintions (Internal)      */
/*                   Multi-Media I/O Procedure for digital video files      */
/*                                                                          */
/* COPYRIGHT:     IBM - International Business Machines                     */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993               */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: OS/2 Release 2.0                                                 */
/*                                                                          */
/************************** END OF SPECIFICATIONS ***************************/

#include "umformat.h"              /* include file format specific header file */
#include "ulionls.h"

/* XLATOFF */
#pragma pack(4)
/* XLATON */


/******************
 * Defines:
 ******************/
#define FFIO_FORMATNAMEKEY ULIO_FORMATNAMEKEY
#define FFIO_DEF_FMT_EXT   "UMB"
#define DLLNAME            "ULIO"                       /* Filename of this "DLL"        */
#define EXTAUDIOPERCENT   .15
#define NUMBER_CODEC_TABLE 3

#define INVALID_OPEN_FLAGS   (MMIO_WRITE |      \
                              MMIO_READWRITE |  \
                              MMIO_BUFSHARED |  \
                              MMIO_CREATE |     \
                              MMIO_DELETE |     \
                              MMIO_ALLOCBUF |   \
                              MMIO_APPEND)

#define INVALID_READ_FLAGS  ( MMIO_WRITE | INVALID_OPEN_FLAGS )

#define INVALID_WRITE_FLAGS  ( MMIO_READ | INVALID_OPEN_FLAGS )

#define VALID_FORMAT_FLAGS  ( MMIO_CANREADUNTRANSLATED |            \
                              MMIO_CANWRITEUNTRANSLATED |           \
                              MMIO_CANREADWRITEUNTRANSLATED |       \
                              MMIO_CANSEEKUNTRANSLATED |            \
                              MMIO_CANSAVEUNTRANSLATED |            \
                              MMIO_CANMULTITRACKREADUNTRANSLATED |  \
                              MMIO_CANMULTITRACKWRITEUNTRANSLATED | \
                              MMIO_CANTRACKSEEKUNTRANSLATED |       \
                              MMIO_CANTRACKREADUNTRANSLATED)

#ifdef WORKSHOP
#define CLIPBRD_FORMAT  0     // no clip board support provided
#endif

/***************************************************************************************/
/* Ultimotion specific define!                                                         */
/*                                                                                     */
/*  TRACKI.ulMisc1 = Length of left-over data that need to be picked up in next        */
/*                   call.(Otherwise, out of sync)                                     */
/*  (NOT USED) TRACKI.ulMisc1 = Length of data chunk for external audio files.         */
/*  (NOT USED) TRACKI.ulMisc2 = Pointer to external audio buffer to use for reads.     */
/*  (NOT USED) TRACKI.ulMisc3 = Pointer to begin of data in external audio read buffer.*/
/*  (NOT USED) TRACKI.ulMisc4 = Pointer to end of data in external audio read buffer.  */
/*                                                                                     */
/*  INSTANCE.ulMisc1 = Offset of Data Frame List Chunk end                             */
/*  INSTANCE.ulMisc2 = Offset of Data Frame List Chunk Begin                           */
/*                                                                                     */
/***************************************************************************************/
#define RAWFILEHEADER   VOID                /*rawhdr */
#define PRAWFILEHEADER  PVOID               /*prawhdr */


/*****************************************************************************/
/*  ULTIMOTION PROTOTYPE DECLARATIONS                                        */
/*****************************************************************************/

/*****************
 * ulopen.c
 *****************/
LONG ffOpenRead ( PMMIOINFO pmmioinfo,
                  PINSTANCE pinstance );

LONG ffOpenWrite ( PMMIOINFO pmmioinfo,
                   PINSTANCE pinstance );

LONG ffReadHeader ( PMMIOINFO pmmioinfo,
                    PINSTANCE pinstance,
                    PMMCKINFO pmmckinfoHdr );

LONG ffInitCodecs ( PINSTANCE pinstance,
                    PTRACKI ptracki,
                    PMMIOINFO pmmioinfo );

LONG ffProcessVideoHeader ( PINSTANCE pinstance,
                            PUMVIDEOHEADER  pumvhHdr,
                            PMMIOINFO pmmioinfo );

LONG ffProcessAudioHeaderList ( PINSTANCE pinstance,
                                PVOID  pListHdr,
                                ULONG  ulChunkLength,
                                PULONG pulNumTracks );

LONG ffProcessExtAudioHeader ( PINSTANCE pinstance,
                               PEXTAUDIONAME pumeaHdr,
                               ULONG ulHdrLength );

LONG ffProcessIntAudioHeader ( PINSTANCE pinstance,
                               PAUDIOHEADER pumahHdr,
                               ULONG ulHdrLength );

/*****************
 * ulident.c
 *****************/
LONG IOProcIdentifyFile ( PMMIOINFO pmmioinfo,
                          PSZ pszFileName,
                          HMMIO FileHandle );

/*****************
 * ulread.c
 *****************/
LONG IOProcRead ( PMMIOINFO pmmioinfo,
                  PBYTE pbBuffer,
                  LONG  lBytes );

LONG IOProcMultiTrackRead ( PMMIOINFO pmmioinfo,
                             PMMMULTITRACKREAD pmtread );

/*****************
 * ulsrv.c
 *****************/
LONG ffSeekTrack ( PMMIOINFO pmmioinfo,
                   HMMIO hmmio,
                   LONG lSeekValue,
                   LONG lSeekType );

LONG ffSeekToDataBegin ( PMMIOINFO pmmioinfo,
                         PINSTANCE pinstance);

PTRACKI ffFindExtAudioTrack ( PINSTANCE pinstance,
                              ULONG ulNumTracks,
                              PTRACKMAP patrackmapList);

LONG ffCleanUp ( PINSTANCE pinstance );

/*****************
 * ulcodec.c
 *****************/
LONG IOProcDecompress ( PMMIOINFO pmmioinfo,
                        PMMDECOMPRESS pmmdec );

LONG ffOpenCodec ( PMMIOINFO pmmioinfo,
                   PINSTANCE pinstance,
                   PCCB pccb,
                   ULONG hCodec,
                   PTRACKI ptracki);

LONG ffAssociateCodec( PINSTANCE pinstance,
                       PMMEXTENDINFO pmmextendinfo );


/* XLATOFF */
#pragma pack()
/* XLATON */



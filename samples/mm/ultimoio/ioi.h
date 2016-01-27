/**************************START OF SPECIFICATIONS **************************/
/*                                                                          */
/* SOURCE FILE NAME:  IOI.H                                                 */
/*                                                                          */
/* DESCRIPTIVE NAME: File Format IO Proc defintions (Internal)              */
/*                   Multi-Media I/O Procedure for digital video files      */
/*                                                                          */
/* COPYRIGHT:     IBM - International Business Machines                     */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993               */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: OS/2 Release 2.1                                                 */
/*                                                                          */
/************************** END OF SPECIFICATIONS ***************************/

/* XLATOFF */
#pragma pack(4)
/* XLATON */

/***********************
 * Forward Declarations:
 ***********************/
typedef struct  _TRACKI          *PTRACKI;       /* Ptr to a track list */
typedef struct  _CCB             *PCCB;          /* Ptr to Compression Control Block */
typedef struct  _INSTANCE        *PINSTANCE;     /* pinstance */
typedef struct  _SUBINDEX        *PSUBINDEX;     /* Ptr to subindex structure */
typedef struct  _CLIPBRDENTRY    *PCLIPBRDENTRY; /* Ptr to clipboard entry */
typedef struct  _EDITELEMENT     *PEDITELEMENT;  /* Ptr to edit element */
typedef struct  _FILEELEMENT     *PFILEELEMENT;  /* Ptr to file element */

#include "ff.h"              /* Include the File Format Specific header info */

/******************
 * Defines:
 ******************/
#define VOIDPARM          111                          /* Void parameter                */
#define HEAP_SIZE         4096                         /* Heap size for DLL             */
#define MMTIMEUNIT        3000                         /* MMTIME = 1/3000s.             */
#define DEFAULTBUFFERSIZE 16384                        /* Default size of Temp buffer.  */

#define IS_AUDIO_TRACK(ulMediaType)  (ulMediaType == MMIO_MEDIATYPE_AUDIO)               // Should include MIDI in future...
#define IS_VIDEO_TRACK(ulMediaType)  (ulMediaType == MMIO_MEDIATYPE_DIGITALVIDEO)

/*******************************************
 *  TRACKI    - List of valid tracks for
 *              this file instance.
 *******************************************/
typedef struct _TRACKI { /* tracki */
   PTRACKI     ptrackiNext;           /* Ptr to next tracki list entry */
   ULONG       ulTrackID;             /* Track number */
   ULONG       ulLastReadPos;         /* Last read track position. */
   PVOID       pTrackHeader;          /* Pointer to specific media type header for this track */
   ULONG       ulTrackHdrLength;      /* track header length */
   PVOID       pRawHdr;               /* Ptr to raw File header.*/
   ULONG       ulRawHdrLength;        /* Raw header length */
   ULONG       ulMediaType;           /* Media type for this track */
   ULONG       ulNumEntries;          /* (Temp) Current srcbuftab entry in trackmap table */
   ULONG       ulMisc1;               /* File Format specific field */
   ULONG       ulMisc2;               /* File Format specific field */
   ULONG       ulMisc3;               /* File Format specific field */
   ULONG       ulMisc4;               /* File Format specific field */
} TRACKI;

/************************************************
 *  CCB - Compression Control Block.
 ************************************************/
typedef struct _CCB  {  /* ccb */
   PCCB             pccbNext;        /* Ptr to next CCB entry. */
   HMODULE          hmodule;         /* Module handle for Compression IO Proc. */
   PMMIOPROC        pmmioproc;       /* Pointer to Compression entry point procedure. */
   ULONG            hCodec;          /* Handle to Codec IO Proc instance */
   CODECINIFILEINFO cifi;            /* Codec ini file info structure */
   CODECOPEN        codecopen;       /* Codec open structure */
   ULONG            ulLastSrcBuf;    /* Ptr to last src buffer, for Multiaperture decompress calls */
   ULONG            ulMisc1;         /* File Format specific field */
   ULONG            ulMisc2;         /* File Format specific field */
} CCB;

/************************************************
 *  SUBINDEX - Sub-Index Structure
 ************************************************/
typedef struct _SUBINDEX {                /* subindex */
   PSUBINDEX        psubindexNext;
   ULONG            ulFrameFlags;
   ULONG            ulFrameLength;
   ULONG            ulFrameOffset;
} SUBINDEX;

/************************************************
 *  INDEX - Index Structure
 ************************************************/
typedef struct _INDEX {                   /* index */
   ULONG            ulFrameFlags;         /* Flags*/
   ULONG            ulFrameLength;
   ULONG            ulFrameOffset;
   PSUBINDEX        psubindexList;
} INDEX;
typedef INDEX *PINDEX;

/************************************************
 *  INSTANCE - IO proc instance structure.
 ************************************************/
typedef struct _INSTANCE {                          /* instance */
   PRAWFILEHEADER           pRawHdr;                /* Ptr to raw File header.*/
   ULONG                    ulRawHdrLength;         /* Raw header length */
   PMMMOVIEHEADER           pmmhdr;                 /* Ptr to Standard Presentation header for this media type.*/
   ULONG                    ulMmHdrLength;          /* MMOVIEHEADER length.*/
   PTRACKI                  ptrackiList;            /* Pointer to track list of valid tracks for this file.*/
   LONG                     lLastTrack;             /* Last track mmioRead work on. */
   LONG                     lCurrentTrack;          /* Current processing track.*/
   PCCB                     *pccbList;              /* Pointer to Compression IO Proc list.*/
   HMMIO                    hmmioFileHandle;        /* File handle. */
   LONG                     lFileCurrentPosition;   /* File's current position.*/
   ULONG                    ulFileLen;              /* Length of file.*/
   ULONG                    ulFlags;                /* Flags*/
   ULONG                    ulMode;                 /* Current IO Proc operation mode*/
   PVOID                    pTempBuffer;            /* Temp buffer for read track.*/
   ULONG                    ulTempBufferSize;       /* Temp buffer size.*/
   ULONG                    ulDataInTempBuffer;     /* Bytes in temp buffer copied from read buffer.*/
   PVOID                    pLastBuffer;            /* Last buffer read into.*/
   ULONG                    ulLastBufferOffset;     /* Offset into last buffer.*/
   PINDEX                   paindexList;            /* Ptr to Frame index (array ) */
   ULONG                    ulNumIndex;             /* Number of entries in the index List */
   PCCB                     pccbLastUsed;           /* Ptr to last used codec for Multiaperture decompress calls */
   ULONG                    ulEditFlags;            /* Editng state flags */
   PEDITELEMENT             pEditList;              /* Dynamic descrition of an edited file */
   PEDITELEMENT             pUndoList;              /* Stack for undo command */
   PEDITELEMENT             pRedoList;              /* Stack for redo command */
   PFILEELEMENT             pFileList;              /* List of pasted files */
   PEDITELEMENT             pCurrentElement;        /* Element being played now */
   DATETIME                 dtClipbrdTime;          /* time stamp for clipboard data */
   PID                      pidPID;                 /* process id */
   ULONG                    ulFrameCount;           /* Number of frames played in current element */
   ULONG                    ulMisc1;                /* File Format specific field */
   ULONG                    ulMisc2;                /* File Format specific field */
   ULONG                    ulMisc3;                /* File Format specific field */
   ULONG                    ulMisc4;                /* File Format specific field */
   SZ                       szFileName[CCHMAXPATH]; /* file name */
   ULONG                    ulDeltaCount;           /* Number of remaining delta frames before the next I-frame */
   ULONG                    ulCurrentFrame;         /* current frame of video track */
   PMMVIDEOHEADER           pmmVideoHdr;            /* pointer to header of main video track */
   ULONG                    ulCurrentVideoTrack;    /* current video track */
   ULONG                    ulCurrentAudioTrack;    /* current audio track */
} INSTANCE;

/* ulFlags defines */
#define     HAVE_PALETTE        0x0001L             /*  Have searched file for first palette is exists */
                                                    /*  so skip it on any MMIOM_MULTITRACKREAD calls   */
#define     OPENED_READONLY     0x0002L             /*  Movie file was opened for Read only (play back) */
#define     OPENED_READWRITE    0x0004L             /*  Movie file was opened for Read/Write (recording/editing) */
#define     OPENED_WRITECREATE  0x0008L             /*  Movie file was opened for Write/Create (recording) */

/* ulMode defines */
#define     MODE_NORMALREAD   0x0000L               /* Normal reading operation.*/
#define     MODE_SCANREAD     0x0001L               /* Read only Key frames for video tracks only.*/
                                                    /*  Turn off audio tracks. (return error if.*/
                                                    /*  requested thru MMIOM_MULTITRACKREAD).*/
#define     MODE_REVERSEREAD  0x0002L               /* Read in reverse order. (No audio reads) */

// ulEditFlags defines

#define     ACTIVE_CLIPBRD_REFERENCE   0x00000001   /* cut or copy outstanding */
#define     DIRTY_BY_EDIT              0x00000002   /* has been edited, needs to be saved */
#define     FULLY_QUALIFIED_PATH       0x00000004   /* szFileName is fully qualified */
#define     CANNOT_CUT_OR_COPY         0x00000008   /* file name is not known */

// FFIO specific optional capabilities function defines:

typedef struct _FFIOFNCT {

   PFN   pfnIOProcGetCF;               // MMIOM_GETCF
   PFN   pfnIOProcGetCFEntry;          // MMIOM_GETCFENTRY

   PFN   pfnIOProcOpen;                // MMIOM_CLOSE
   PFN   pfnIOProcClose;               // MMIOM_OPEN
   PFN   pfnIOProcRead;                // MMIOM_READ
   PFN   pfnIOProcSeek;                // MMIOM_SEEK
   PFN   pfnIOProcWrite;               // MMIOM_WRITE

   PFN   pfnIOProcIdentifyFile;        // MMIOM_IDENTIFYFILE
   PFN   pfnIOProcGetHeader;           // MMIOM_GETHEADER
   PFN   pfnIOProcSetHeader;           // MMIOM_SETHEADER
   PFN   pfnIOProcQueryHeaderLength;   // MMIOM_QUERYHEADERLENGTH
   PFN   pfnIOProcGetFormatName;       // MMIOM_GETFORMATNAME
   PFN   pfnIOProcGetFormatInfo;       // MMIOM_GETFORMATINFO
   PFN   pfnIOProcSeekByTime;          // MMIOM_SEEKBYTIME
   PFN   pfnIOProcTempChange;          // MMIOM_TEMPCHANGE
   PFN   pfnIOProcBeginInsert;         // MMIOM_BEGININSERT
   PFN   pfnIOProcEndInsert;           // MMIOM_ENDINSERT
   PFN   pfnIOProcSave;                // MMIOM_SAVE
   PFN   pfnIOProcSet;                 // MMIOM_SET
   PFN   pfnIOProcCompress;            // MMIOM_COMPRESS
   PFN   pfnIOProcDecompress;          // MMIOM_DECOMPRESS
   PFN   pfnIOProcMultiTrackRead;      // MMIOM_MULTITRACKREAD
   PFN   pfnIOProcMultiTrackWrite;     // MMIOM_MULTITRACKWRITE

   PFN   pfnIOProcDelete;              // MMOIM_DELETE
   PFN   pfnIOProcBeginGroup;          // MMIOM_BEGINGROUP
   PFN   pfnIOProcEndGroup;            // MMIOM_ENDGROUP
   PFN   pfnIOProcUndo;                // MMIOM_UNDO
   PFN   pfnIOProcRedo;                // MMIOM_REDO
   PFN   pfnIOProcBeginStream;         // MMIOM_BEGINSTREAM
   PFN   pfnIOProcEndStream;           // MMIOM_ENDSTREAM

   PFN   pfnIOProcCopy;                // MMIOM_COPY
   PFN   pfnIOProcCut;                 // MMIOM_CUT
   PFN   pfnIOProcPaste;               // MMIOM_PASTE
   PFN   pfnIOProcClear;               // MMIOM_CLEAR
   PFN   pfnIOProcStatus;              // MMIOM_STATUS
   PFN   pfnIOProcWinMsg;              // MMIOM_WIN_MESSAGE

} FFIOFNCT, *PFFIOFNCT;

extern FFIOFNCT ffiofnct;

// Macros

#define ENTERCRITX             if (DosRequestMutexSem(hmtxGlobalHeap, SEM_INDEFINITE_WAIT)) return(MMIO_ERROR);
#define ENTERCRIT(rc)          (rc = DosRequestMutexSem(hmtxGlobalHeap, SEM_INDEFINITE_WAIT))
#define EXITCRIT               (DosReleaseMutexSem(hmtxGlobalHeap))

// Structures for clipboard editing:

// A linked list of CLIPBRDENTRY structures is placed in the clipboard
// during cut and copy operations. They are read during paste operations.

typedef struct _CLIPBRDENTRY {
   ULONG          ulStructLen;            // length of this structure
   PCLIPBRDENTRY  pNext;                  // pointer to next in linked list
   USEC           ulStartTime;            // starting time in microseconds
   USEC           ulDuration;             // duration in microseconds
   ULONG          ulFormat;               // the CF_ clipboard format type
   DATETIME       dtClipbrdTime;          // time stamp for clipboard data
   PID            pidPID;                 // originator's process id
   SZ             szFileName[CCHMAXPATH]; // fully qualified file name
} CLIPBRDENTRY;

/* A linked list of EDITELEMENT structures, anchored from the INSTANCE
 * structure, is used to describe the dynamic contents of edited files.
 * The files logical sequence which is used during play and seek operations
 * is determined by ulStart and ulEnd.  The physical data associated with
 * this range is determined from ulFrom and ulTo.
 */

typedef struct _EDITELEMENT {
   PEDITELEMENT   pNext;                  // forward pointer in linked list
   PEDITELEMENT   pPrevious;              // back pointer in linked list
   PEDITELEMENT   pUndoRedoChain;         // links edit lists on the undo/redo lists
   PINSTANCE      pInstance;              // pointer to it's INSTANCE structure
   ULONG          ulFilePosition;         // file position to restore for an undo
   ULONG          ulStart;                // logical starting frame number
   ULONG          ulDuration;             // number of frames
   ULONG          ulPhysical;             // physical starting frame number
   ULONG          ulFlags;                // used by each operation
} EDITELEMENT;

// defines for ulFlags - see also individual files for LSB defines

#define AVIO_EDIT_PLAYBACK_CUED          0x80000000

/* A FILEELEMENT structure is created for each file that is opened
 * during a paste operation. It is used to keep track of those files
 * which must be closed when the target file is saved.
 */

typedef struct _FILEELEMENT {
   PFILEELEMENT   pNext;                  // pointer to next in linked list
   PINSTANCE      pInstance;              // pointer files INSTANCE structure
   MMIOINFO       mmioInfo;               // MMIOINFO structure
} FILEELEMENT;

// Prototype Declarations

/*****************
 * checkmem.c
 *****************/
RC APIENTRY CheckMem( PVOID, ULONG, ULONG );

/*****************
 * ioclose.c
 *****************/
LONG IOProcClose ( PMMIOINFO pmmioinfo );
VOID ioCleanUp ( PMMIOINFO pmmioinfo );

/*****************
 * iocodec.c
 *****************/

LONG ioCheckCodecList(PINSTANCE pinstance);

LONG ioDetermineBitCount(PCODECINIFILEINFO pcifi);

LONG ioDetermineCodec ( PINSTANCE pinstance,
                        ULONG ulSearchFlags,
                        PCODECINIFILEINFO pcifi );

PCCB ioLoadCodecDLL ( PINSTANCE pinstance,
                      PCODECINIFILEINFO pcifi,
                      PULONG phCodec,
                      ULONG ulTrack );

PCCB ioLoadCodec ( PMMIOINFO pmmioinfo,
                   PINSTANCE pinstance,
                   PTRACKI ptracki,
                   PCODECINIFILEINFO pcifi );

PCCB ioFindCodec ( PINSTANCE pinstance,
                   ULONG ulCompressType );

LONG ioCloseCodec ( PCCB pccb );

LONG ioAssociateCodec ( PMMIOINFO pmmioinfo,
                        PINSTANCE pinstance,
                        PCODECASSOC pcodecassoc,
                        ULONG ulDefaultBitCount );

/*****************
 * ioentry.c
 *****************/
LONG APIENTRY IOProc_Entry ( PVOID pmmiostr,
                             WORD wMessage,
                             LONG lParam1,
                             LONG lParam2  );

LONG ioCheckReturnCode ( PMMIOINFO pmmioinfo,
                         WORD wMsg,
                         LONG returncode );

LONG ioInvalidEntry( PMMIOINFO pmmioinfo,
                     PINSTANCE *ppinstance,
                     WORD wMessage,
                     LONG lParam1,
                     LONG lParam2 );

/*****************
 * ioformat.c
 *****************/
LONG IOProcGetFormatName ( PSZ  pszNameBuf,
                           LONG NameBufLen );

LONG IOProcGetFormatInfo ( PMMFORMATINFO pmmformatinfo );

/*****************
 * ioheader.c
 *****************/
LONG IOProcGetHeader ( PMMIOINFO pmmioinfo,
                       PVOID pHeader,
                       LONG  lHeaderLength,
                       LONG  lCurrentTrack );

LONG IOProcQueryHeaderLength ( PMMIOINFO pmmioinfo,
                               LONG      lCurrentTrack );

LONG ioGetTrackHeader ( PTRACKI ptracki,
                        PVOID   pTrackHeader,
                        ULONG   ulHeaderLength,
                        ULONG   ulFlag );

/*****************
 * ioopen.c
 *****************/
LONG IOProcOpen ( PMMIOINFO pmmioinfo,
                  PSZ pszFileName );

/*****************
 * ioseek.c
 *****************/
LONG IOProcSeek ( PMMIOINFO pmmioinfo,
                  LONG lSeekValue,
                  LONG lSeekType );

LONG ioSeekFile ( PLONG plRc,
                  HMMIO hmmio,
                  LONG lSeekValue );

/*****************
 * ioset.c
 *****************/
LONG IOProcSet ( PMMIOINFO pmmioinfo,
                 LONG lParam1,
                 LONG lParam2 );

/*****************
 * iosrv.c
 *****************/
LONG ioGetPtrInstance ( PMMIOINFO pmmioinfo,
                        PINSTANCE *ppinstance );

PTRACKI ioFindTracki ( PINSTANCE pinstance,
                       ULONG ulTrackID );

PTRACKMAP ioFindTrackMapEntry ( PTRACKMAP patrackmapList,
                                ULONG ulNumTracks,
                                ULONG ulTrackID );

LONG ioIdentifyStorageSystem ( PMMIOINFO pmmioinfo,
                               PSZ pszFileName );

VOID ioInstanceInit ( PINSTANCE pinstance );

LONG ioAddTracksToMovieHeader ( PINSTANCE pinstance );

LONG ioValidateRead ( PINSTANCE pinstance,
                      PVOID pReadBuffer,
                      ULONG ulNumTracks,
                      PTRACKMAP patrackmapList );

/************************** Editing Prototypes *************************/

LONG           ioEditFindHeaders(PINSTANCE, PMMVIDEOHEADER *, PMMAUDIOHEADER *);
LONG           ioEditSetup(PMMIOINFO, PMMIO_EDIT_PARMS);
PEDITELEMENT   ioEditCloneEditList(PINSTANCE);
LONG           ioEditPushEditToUndo(PINSTANCE);
void           ioEditCleanup(PINSTANCE);
ULONG          ioEditTimeToFrame(PMMVIDEOHEADER, ULONG);
ULONG          ioEditFrameToTime(PMMVIDEOHEADER, ULONG);
ULONG          ioEditDuration(PINSTANCE, PMMIO_EDIT_PARMS);
LONG           ioEditGetFullName(PSZ, PSZ);

LONG           IOProcUndo(PMMIOINFO,  PMMIO_EDIT_PARMS);
LONG           IOProcRedo(PMMIOINFO,  PMMIO_EDIT_PARMS);
LONG           IOProcClear(PMMIOINFO, PMMIO_EDIT_PARMS);
LONG           IOProcCut(PMMIOINFO,   PMMIO_EDIT_PARMS);
LONG           IOProcCopy(PMMIOINFO,  PMMIO_EDIT_PARMS);
LONG           IOProcPaste(PMMIOINFO, PMMIO_EDIT_PARMS);

LONG           IOProcSave(PMMIOINFO, PSZ);
LONG           IOProcStatus(PMMIOINFO, ULONG, PMMIO_STATUS_PARMS);
LONG           IOProcWinMsg(PMMIOINFO, PMMIO_WINMSG);

/* XLATOFF */
#pragma pack()
/* XLATON */

/***************************************************************************\
*
* Module Name: MMIOOS2.H
*
* OS/2 2.0 Multimedia Extensions Input/Output Manager Definitions file
*  NOTE:  All structures are PACKED(4)
*
* Copyright (c) International Business Machines Corporation 1990,1991,1992
*                         All Rights Reserved
*
*
\****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif



#ifndef _MMIO_H_
#define _MMIO_H_

#pragma pack(4)

/*
 * MMIO symbols.
 */

#ifndef _OS2MEDEF_H
typedef ULONG  HMMIO;
#endif

typedef HMMIO  HMMCF;
typedef ULONG  FOURCC;
typedef FOURCC *PFOURCC;

/*
 * MS compat defines.
 */

#define HPSTR PSZ

/*
 * Define MMIO public data structures.
 */

typedef struct _MMCKINFO {    /* mmckinfo                             */
   FOURCC   ckid;             /* Chunk id (FOURCC)                    */
   ULONG    ckSize;           /* Chunk Size (bytes)                   */
   FOURCC   fccType;          /* FourCC Type (if ckid RIFF or LIST)   */
   ULONG    ulDataOffset;     /* File Offset of data portion of chunk */
   ULONG    ulFlags;          /* MMIO_DIRTY (if new chunk)            */
   } MMCKINFO;

typedef MMCKINFO FAR *PMMCKINFO;
#define PMMCKINFO  PMMCKINFO     /* MS compat define */

typedef LONG (APIENTRY MMIOPROC)                    /*  Format must       */
                                ( PVOID pmmioinfo,  /*  appear this       */
                                  USHORT usMsg,     /*  way for h2inc     */
                                  LONG lParam1,     /*  to work properly. */
                                  LONG lParam2 );

typedef MMIOPROC FAR *PMMIOPROC;
typedef MMIOPROC FAR **PPMMIOPROC;
#define PMMIOPROC  PMMIOPROC     /* MS compat define */

typedef struct _MMIOINFO {       /* mmioinfo                    */
   ULONG       ulFlags;          /* Open flags                  */
   FOURCC      fccIOProc;        /* FOURCC of the IOProc to use */
   PMMIOPROC   pIOProc;          /* Function Pointer to IOProc to use */
   ULONG       ulErrorRet;       /* Extended Error return code  */
   LONG        cchBuffer;        /* I/O buff size (if used), Fsize if MEM */
   PCHAR       pchBuffer;        /* Start of I/O buff           */
   PCHAR       pchNext;          /* Next char to read or write in buff */
   PCHAR       pchEndRead;       /* Last char in buff can be read + 1  */
   PCHAR       pchEndWrite;      /* Last char in buff can be written + 1 */
   LONG        lBufOffset;       /* Offset in buff to pchNext */
   LONG        lDiskOffset;      /* Disk offset in file       */
   ULONG       aulInfo[4];       /* IOProc specific fields    */
   LONG        lLogicalFilePos;  /* Actual file position, buffered or not */
   ULONG       ulTranslate;      /* Translation field         */
   FOURCC      fccChildIOProc;   /* FOURCC of Child IOProc    */
   PVOID       pExtraInfoStruct; /* Pointer to a structure of related data */
   HMMIO       hmmio;            /* Handle to media element   */
   } MMIOINFO;

typedef MMIOINFO FAR *PMMIOINFO;
#define PMMIOINFO  PMMIOINFO     /* MS compat define */

typedef struct _MMCFINFO {       /* mmcfinfo                  */
   ULONG    ulHeaderSize;        /* CTOC header size          */
   ULONG    ulEntriesTotal;      /* Num of CTOC table entries */
   ULONG    ulEntriesDeleted;    /* Num of CTOC table entries deleted */
   ULONG    ulEntriesUnused;     /* Num of unused CTOC entries */
   ULONG    ulBytesTotal;        /* Combined byte size of all CGRP elements */
   ULONG    ulBytesDeleted;      /* Byte size of all deleted CGRP elements */
   ULONG    ulHeaderFlags;       /* Info about entire compound file (CF)  */
   USHORT   usEntrySize;         /* Size of each CTOC table entry */
   USHORT   usNameSize;          /* Size of name field in entry, default 13 */
   USHORT   usExHdrFields;       /* Num CTOC header extra fields */
   USHORT   usExEntFields;       /* Num CTOC entry extra fields  */
   } MMCFINFO;

/*
 * Note:  The are variable length ULONG arrays that may be present
 *        at the end of the MMCFINFO structure.
 */

/*   ULONG    (*aulExHdrFldUsage)[];  * Array of header extra usage fields */
/*   ULONG    (*aulExtEntFldUsage)[]; * Array of entry extra usage fields  */
/*   ULONG    (*aulExHdrField)[];     * Array of header extra fields       */

typedef MMCFINFO FAR *PMMCFINFO;
#define PMMCFINFO  PMMCFINFO     /* MS compat define */

typedef struct _MMCTOCENTRY {    /* mmctocentry                   */
   ULONG    ulOffset;            /* Offset of element within CGRP */
   ULONG    ulSize;              /* Size of element               */
   ULONG    ulMedType;           /* Fourcc of element             */
   ULONG    ulMedUsage;          /* Possible sub type             */
   ULONG    ulCompressTech;      /* Compression technique used    */
   ULONG    ulUncompressBytes;   /* Actual size of uncompressed element */
   } MMCTOCENTRY;

/*
 * Note:  A variable length name field and possibly a ULONG array may
 *        be present at the end of the MMCTOCENTRY structure.
 */

/*   ULONG    (*aulExEntField)[];  * Array of entry extra fields */
/*   PSZ      pszElementName[];    * Name of element, variable length */

typedef MMCTOCENTRY FAR *PMMCTOCENTRY;
#define PMMCTOCENTRY  PMMCTOCENTRY /* MS compat define */

/*
 * Conversion Flags/Data Structures:
 */

#define MMIO_MEDIATYPE_IMAGE        0x00000001L  /* Image media */
#define MMIO_MEDIATYPE_AUDIO        0x00000002L  /* Audio media */
#define MMIO_MEDIATYPE_MIDI         0x00000004L  /* MIDI media  */
#define MMIO_MEDIATYPE_COMPOUND     0x00000008L  /* Cmpd media  */
#define MMIO_MEDIATYPE_OTHER        0x00000010L  /* Other media */
#define MMIO_MEDIATYPE_UNKNOWN      0x00000020L  /* Unknown media */
#define MMIO_MEDIATYPE_DIGITALVIDEO 0x00000040L  /* Digital Video */
#define MMIO_MEDIATYPE_ANIMATION    0x00000080L  /* Future: Not Supported */
#define MMIO_MEDIATYPE_MOVIE        0x00000100L  /* Movie File */

#define MMIO_CANREADTRANSLATED              0x00000001L /*IOProc Cpbilty Flgs*/
#define MMIO_CANWRITETRANSLATED             0x00000002L /* "       "       " */
#define MMIO_CANREADWRITETRANSLATED         0x00000004L /* "       "       " */
#define MMIO_CANREADUNTRANSLATED            0x00000008L /* "       "       " */
#define MMIO_CANWRITEUNTRANSLATED           0x00000010L /* "       "       " */
#define MMIO_CANREADWRITEUNTRANSLATED       0x00000020L /* "       "       " */
#define MMIO_CANSEEKTRANSLATED              0x00000040L /* "       "       " */
#define MMIO_CANSEEKUNTRANSLATED            0x00000080L /* "       "       " */
#define MMIO_CANINSERTUNTRANSLATED          0x00000100L /* "       "       " */
#define MMIO_CANSAVEUNTRANSLATED            0x00000200L /* "       "       " */
#define MMIO_CANINSERTTRANSLATED            0x00000400L /* "       "       " */
#define MMIO_CANSAVETRANSLATED              0x00000800L /* "       "       " */
#define MMIO_CANMULTITRACKREADTRANSLATED    0x00001000L /* "       "       " */
#define MMIO_CANMULTITRACKREADUNTRANSLATED  0x00002000L /* "       "       " */
#define MMIO_CANMULTITRACKWRITETRANSLATED   0x00004000L /* "       "       " */
#define MMIO_CANMULTITRACKWRITEUNTRANSLATED 0x00008000L /* "       "       " */
#define MMIO_CANTRACKSEEKTRANSLATED         0x00010000L /* "       "       " */
#define MMIO_CANTRACKSEEKUNTRANSLATED       0x00020000L /* "       "       " */
#define MMIO_CANTRACKREADTRANSLATED         0x00040000L /* "       "       " */
#define MMIO_CANTRACKREADUNTRANSLATED       0x00080000L /* "       "       " */
#define MMIO_CANTRACKWRITETRANSLATED        0x00100000L /* "       "       " */
#define MMIO_CANTRACKWRITEUNTRANSLATED      0x00200000L /* "       "       " */

#define MMIO_IOPROC_STORAGESYSTEM     0x00000001L  /* IOProc Types */
#define MMIO_IOPROC_FILEFORMAT        0x00000002L
#define MMIO_IOPROC_DATAFORMAT        0x00000004L

typedef struct _MMFORMATINFO {         /* mmformatinfo             */
   ULONG    ulStructLen;               /* Length of this structure */
   FOURCC   fccIOProc;                 /* IOProc identifier        */
   ULONG    ulIOProcType;              /* Type of IOProc           */
   ULONG    ulMediaType;               /* Media Type               */
   ULONG    ulFlags;                   /* IOProc capability flags  */
   CHAR     szDefaultFormatExt[sizeof(FOURCC)  + 1];
                                       /* Default extension 4 + null */
   ULONG    ulCodePage;                /* Code Page                  */
   ULONG    ulLanguage;                /* Language                   */
   LONG     lNameLength;               /* length of identifier string */
   } MMFORMATINFO;

typedef MMFORMATINFO *PMMFORMATINFO;

#define MMIO_IMAGE_UNKNOWN          0x00000000L  /* Unknown image content */
#define MMIO_IMAGE_DRAWING          0x00000001L  /* Simple drawing        */
#define MMIO_IMAGE_GRAPH            0x00000002L  /* Graphs & Cartoons     */
#define MMIO_IMAGE_PHOTO            0x00000004L  /* Varying Color & Shades */

#ifdef INCL_GPI
#ifdef INCL_GPIBITMAPS

#define MAX_PALETTE  256

typedef struct _XDIBHDR_PREFIX {             /* XDIB Header Prefix */
   ULONG ulMemSize;                          /* Length of bitmap   */
   ULONG ulPelFormat;
   USHORT usTransType;
   ULONG ulTransVal;
   } XDIBHDR_PREFIX;

typedef struct _MMXDIBHEADER {               /* XDIB Header */
   XDIBHDR_PREFIX    XDIBHeaderPrefix;
   BITMAPINFOHEADER2 BMPInfoHeader2;
   } MMXDIBHEADER;

typedef MMXDIBHEADER *PMMXDIBHEADER;

typedef struct _MMIMAGEHEADER {              /* mmimageheader   */
   ULONG             ulHeaderLength;         /* Length in Bytes */
   ULONG             ulContentType;          /* Image content   */
   ULONG             ulMediaType;            /* Media Type      */
   MMXDIBHEADER      mmXDIBHeader;           /* OS/2 2.0 PM compat header */
   RGB2              bmiColors[MAX_PALETTE]; /* PM compatible palette */
   } MMIMAGEHEADER;

typedef MMIMAGEHEADER *PMMIMAGEHEADER;

#endif
#endif

#define MMIO_AUDIO_UNKNOWN          0x00000000L  /* Unknown image content  */
#define MMIO_AUDIO_VOICE            0x00000001L  /* Limited Range          */
#define MMIO_AUDIO_MUSIC            0x00000002L  /* FM Radio or equivalent */
#define MMIO_AUDIO_HIFI             0x00000004L  /* High quality recording */

typedef struct _WAVE_HEADER {                /* waveheader          */
   USHORT          usFormatTag;              /* Type of wave format */
   USHORT          usChannels;               /* Number of channels  */
   ULONG           ulSamplesPerSec;          /* Sampling rate       */
   ULONG           ulAvgBytesPerSec;         /* Avg bytes per sec   */
   USHORT          usBlockAlign;             /* Block Alignment in bytes */
   USHORT          usBitsPerSample;          /* Bits per sample     */
   } WAVE_HEADER;

typedef struct _XWAV_HEADERINFO {            /* xwaveheader info        */
   ULONG           ulAudioLengthInMS;        /* Audio data in millisecs */
   ULONG           ulAudioLengthInBytes;     /* Audio data in bytes     */
   PVOID           pAdditionalInformation;
   } XWAV_HEADERINFO;

typedef struct _MMXWAV_HEADER {              /* mmxwaveheader            */
   WAVE_HEADER     WAVEHeader;               /* Per RIFF WAVE Definition */
   XWAV_HEADERINFO XWAVHeaderInfo;           /* Extended wave definition */
   } MMXWAV_HEADER;

typedef struct _MMAUDIOHEADER {              /* mmaudioheader   */
   ULONG           ulHeaderLength;           /* Length in Bytes */
   ULONG           ulContentType;            /* Image content   */
   ULONG           ulMediaType;              /* Media Type      */
   MMXWAV_HEADER   mmXWAVHeader;             /* header          */
   } MMAUDIOHEADER;

typedef MMAUDIOHEADER *PMMAUDIOHEADER;

#define MMIO_MIDI_UNKNOWN          0x00000000L  /* Unknown midi content   */
#define MMIO_MIDI_VOICE            0x00000001L  /* Limited Range          */
#define MMIO_MIDI_MUSIC            0x00000002L  /* FM Radio or equivalent */
#define MMIO_MIDI_HIFI             0x00000004L  /* High quality recording */

/*
 * MMPMMMIO.INI file structure and definitions.
 */

#define DLLNAME_SIZE     CCHMAXPATH
#define PROCNAME_SIZE    32
#define MAX_EXTENSION_NAME  4

typedef struct _MMINIFILEINFO {               /* mminifileinfo          */
   FOURCC      fccIOProc;                     /* IOProc identifier      */
   CHAR        szDLLName[DLLNAME_SIZE];       /* DLL name string        */
   CHAR        szProcName[PROCNAME_SIZE];     /* Procedure name string  */
   ULONG       ulFlags;                       /* Capability Flags       */
   ULONG       ulExtendLen;                   /* Length of ext fields   */
   ULONG       ulMediaType;                   /* Media type             */
   ULONG       ulIOProcType;                  /* Type of IOProc         */
   CHAR        szDefExt[sizeof(MAX_EXTENSION_NAME)];
                                              /* Default file extension */
   } MMINIFILEINFO;

typedef MMINIFILEINFO *PMMINIFILEINFO;

/*
 * CODEC Structures and type definitions for Rel. 1.1
 */


#define CODEC_INFO_SIZE    8
#define CODEC_HW_NAME_SIZE 32

#ifdef INCL_MMIO_CODEC

typedef MMIOPROC FAR *PCODECPROC;

typedef struct _CODECINIFILEINFO {           /* codecinifileinfo            */
   ULONG       ulStructLen;                  /* length of this structure    */
   FOURCC      fcc;                          /* File Format ID              */
   CHAR        szDLLName[DLLNAME_SIZE];      /* DLL name string             */
   CHAR        szProcName[PROCNAME_SIZE];    /* Procedure name string       */
   ULONG       ulCompressType;               /* Compression Type            */
   ULONG       ulCompressSubType;            /* Compression SubType         */
   ULONG       ulMediaType;                  /* Media type                  */
   ULONG       ulCapsFlags;                  /* capabilities flags          */
   ULONG       ulFlags;                      /* flags                       */
   CHAR        szHWID[CODEC_HW_NAME_SIZE];   /* specific information        */
   ULONG       ulMaxSrcBufLen;               /* max source buffer length    */
   ULONG       ulSyncMethod;                 /* Synchronization method      */
   ULONG       fccPreferredFormat;           /* Preferred output format     */
   ULONG       ulXalignment;                 /* x alignment - video only    */
   ULONG       ulYalignment;                 /* y alignment - video only    */
   ULONG       ulSpecInfo[CODEC_INFO_SIZE];  /* specific information        */
   } CODECINIFILEINFO;

typedef CODECINIFILEINFO *PCODECINIFILEINFO;

/*
 * CODECINIFILEINFO synchronization method (ulSyncMethod) values.
 */

#define CODEC_SYNC_METHOD_NO_DROP_FRAMES                  0
#define CODEC_SYNC_METHOD_DROP_FRAMES_IMMEDIATELY         1
#define CODEC_SYNC_METHOD_DROP_FRAMES_PRECEDING_KEY       2
#define CODEC_SYNC_METHOD_HARDWARE                        3



/*
 * CODECINIFILEINFO capabilities (ulCapsFlags) values.
 */

#define CODEC_COMPRESS            0x00000001
#define CODEC_DECOMPRESS          0x00000002
#define CODEC_WINDOW_CLIPPING     0x00000004
#define CODEC_PALETTE_TRANS       0x00000008
#define CODEC_SELFHEAL            0x00000010
#define CODEC_SCALE_PEL_DOUBLE    0x00000020
#define CODEC_SCALE_PEL_HALVED    0x00000040
#define CODEC_SCALE_CONTINUOUS    0x00000080
#define CODEC_MULAPERTURE         0x00000100
#define CODEC_4_BIT_COLOR         0x00000200
#define CODEC_8_BIT_COLOR         0x00000400
#define CODEC_16_BIT_COLOR        0x00000800
#define CODEC_24_BIT_COLOR        0x00001000
#define CODEC_HARDWARE            0x00002000
#define CODEC_SYMMETRIC           0x00004000
#define CODEC_ASYMMETRIC          0x00008000
#define CODEC_DIRECT_DISPLAY      0x00010000
#define CODEC_DEFAULT             0x00020000
#define CODEC_ORIGIN_LOWERLEFT    0x00040000
#define CODEC_ORIGIN_UPPERLEFT    0x00080000
#define CODEC_SET_QUALITY         0x00100000  /* quality level is settable */
#define CODEC_DATA_CONSTRAINT     0x00200000  /* data constraint supported */
#define CODEC_HW_OVERLAY          0x00400000
#define CODEC_MULTI_BUFFER        0x00800000
#define CODEC_DITHER_OUTPUT       0x01000000


/* Audio related flags */
#define CODEC_COMP_REALTIME       0x00020000
#define CODEC_DECOMP_REALTIME     0x00040000


/*
 * CODECINIFILEINFO Flag Values (ulFlags) values.
 */


/* Bit definitions for mmioSet() */
#define         MMIO_SET_EXTENDEDINFO                   0X0001
#define         MMIO_QUERY_EXTENDEDINFO_BASE            0x0002
#define         MMIO_QUERY_EXTENDEDINFO_ALL             0x0004


/* CODECASSOC structure */
typedef  struct _CODECASSOC{             /* codec information */
   PVOID  pCodecOpen;                    /* codec specific open header */
   PCODECINIFILEINFO pCODECIniFileInfo;  /* codecinifileinfo           */
   } CODECASSOC;

/* PCODECASSOC definition */
typedef CODECASSOC      *PCODECASSOC;


/* MMEXTENDINFO structure */
typedef  struct _MMEXTENDINFO{            /* extended file information */
   ULONG        ulStructLen;              /* length of this structure  */
   ULONG        ulBufSize;                /* total buffer size */
   ULONG        ulFlags;                  /* flags */
   ULONG        ulTrackID;                /* track ID */
   ULONG        ulNumCODECs;              /* number of codec entries */
   PCODECASSOC  pCODECAssoc;              /* pointer to codec info array */
   } MMEXTENDINFO;

/* PMMEXTENDINFO definition */
typedef MMEXTENDINFO    *PMMEXTENDINFO;

/*
 * MMEXTENDINFO operation (ulFlags) values.
 */
#define MMIO_TRACK                0x00000001
#define MMIO_NORMAL_READ          0x00000002
#define MMIO_SCAN_READ            0x00000004
#define MMIO_REVERSE_READ         0x00000008
#define MMIO_CODEC_ASSOC          0x00000100

/* Audio Related defines */

#define MMIO_REALTIME_CODEC       0x00000200

#define MMIO_RESETTRACKS          -1       /*  Turns off the active track number. */

#define CODEC_START              0x0EC0
#define CODEC_END                0x0EFF

#define MMIOM_CODEC_CLOSE             CODEC_START + 1
#define MMIOM_CODEC_OPEN              CODEC_START + 2
#define MMIOM_CODEC_QUERYNAME         CODEC_START + 3
#define MMIOM_CODEC_QUERYNAMELENGTH   CODEC_START + 4
#define MMIOM_CODEC_COMPRESS          CODEC_START + 5
#define MMIOM_CODEC_DECOMPRESS        CODEC_START + 6
#define MMIOM_CODEC_HW_CONTROL        CODEC_START + 7

#define MMIOMP_CODEC_HW_CONTROL_STOP_DISCARD   0
#define MMIOMP_CODEC_HW_CONTROL_STOP_FLUSH     1
#define MMIOMP_CODEC_HW_CONTROL_STOP_PAUSE     2
#define MMIOMP_CODEC_HW_CONTROL_DATATYPE       3

#endif
/* End CODEC include */

/*
 * Flags:
 */

#define MMIO_CREATE     0x00000001L       /* Open */
#define MMIO_CTOCFIRST  0x00000002L       /* Open */
#define MMIO_READ       0x00000004L       /* Open */
#define MMIO_WRITE      0x00000008L       /* Open */
#define MMIO_READWRITE  0x00000010L       /* Open */
#define MMIO_COMPAT     0x00000020L       /* Open */
#define MMIO_EXCLUSIVE  0x00000040L       /* Open */
#define MMIO_DENYWRITE  0x00000080L       /* Open */
#define MMIO_DENYREAD   0x00000100L       /* Open */
#define MMIO_DENYNONE   0x00000200L       /* Open */
#define MMIO_ALLOCBUF   0x00000400L       /* Open */
#define MMIO_DELETE     0x00000800L       /* Open */

#define MMIO_USE_TEMP   0x00001000L       /* Open/Close/Save */

#define MMIO_INSERTON   0x00000001L       /* Insert */
#define MMIO_INSERTOFF  0x00000002L       /* Insert */

#define MMIO_RWMODE     0x00001000L       /* Open */
#define MMIO_SHAREMODE  0x00002000L       /* Open */

#define MMIO_DIRTY      0x00004000L       /* Write */

#define MMIO_VERTBAR     0x00008000L       /* Open */
#define MMIO_BUFSHARED   0x00010000L       /* Open */
#define MMIO_APPEND      0x00020000L       /* Open */
#define MMIO_NOIDENTIFY  0x00040000L       /* Open */


#define MMIO_FINDFIRST   0x00000001L       /* CF Find Entry */
#define MMIO_FINDNEXT    0x00000002L       /* CF Find Entry */
#define MMIO_FINDUNUSED  0x00000004L       /* CF Find Entry */
#define MMIO_FINDDELETED 0x00000008L       /* CF Find Entry */

#define MMIO_CHANGEDELETED 0x0001         /* CF Change Entry */

#define MMIO_CF_FQNAME     0x0001         /* CF Compact      */

#define MMIO_FHOPEN      0x0001           /* Close */

#define MMIO_EMPTYBUF    0x0001           /* Flush */

#define MMIO_CREATERIFF  0x0001           /* CreateChunk */
#define MMIO_CREATELIST  0x0002           /* CreateChunk */
#define MMIO_FINDCHUNK   0x0004           /* Descend     */
#define MMIO_FINDRIFF    0x0008           /* Descend     */
#define MMIO_FINDLIST    0x0010           /* Descend     */

#define CTOC_HF_SEQUENTIAL 0x00000001L    /* CTOC ulHeaderFlags */
#define CTOC_HF_MEDSUBTYPE 0x00000002L    /* CTOC ulHeaderFlags */

#define CTOC_EFU_UNUSED          0x00000000L /* CTOC extra usage code */
#define CTOC_EFU_LASTMODTIME     0x00000001L /* CTOC extra usage code */
#define CTOC_EFU_CODEPAGE        0x00000002L /* CTOC extra usage code */
#define CTOC_EFU_LANGUAGE        0x00000003L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM0  0x00000005L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM1  0x00000006L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM2  0x00000007L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM3  0x00000008L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM4  0x00000009L /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM5  0x0000000AL /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM6  0x0000000BL /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM7  0x0000000CL /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM8  0x0000000DL /* CTOC extra usage code */
#define CTOC_EFU_COMPRESSPARAM9  0x0000000EL /* CTOC extra usage code */
#define CTOC_CHARSET_STANDARD    0x00000000L /* CTOC charset value    */

#define MMIO_INSTALLPROC         0x00000001L /* Install IO Proc */
#define MMIO_REMOVEPROC          0x00000002L /* Install IO Proc */
#define MMIO_FINDPROC            0x00000004L /* Install IO Proc */

#define MMIO_MATCHFIRST          0x00000010L /* Ini File Handler */
#define MMIO_MATCHNEXT           0x00000020L /* Ini File Handler */
#define MMIO_MATCHFOURCC         0x00000040L /* Ini File Handler */
#define MMIO_MATCHDLL            0x00000080L /* Ini File Handler */
#define MMIO_MATCHPROCEDURENAME  0x00000100L /* Ini File Handler */
#define MMIO_FULLPATH            0x00000200L /* Ini File Handler */
#define MMIO_NOVERIFY            0x00000400L /* Ini File Handler */
#define MMIO_MATCHCOMPRESSTYPE   0x00000800L /* Ini File Handler */
#define MMIO_EXTENDED_STRUCT     0x00001000L /* Ini File ulFlags */
#define MMIO_MATCHCOMPRESSSUBTYPE 0x00002000L /* Ini File Handler */
#define MMIO_MATCHHWID           0x00004000L /* Ini File Handler */
#define MMIO_MATCHCAPSFLAGS      0x00008000L /* Ini File Handler */
#define MMIO_SKIPMATCH           0x00010000L /* Ini/Load Handler */

#define MMIO_TOUPPER             0x0001      /* StringToFourcc */

#define MMIO_CF_ENTRY_EXISTS     0x00000001L /* Add CGRP element */

#define MMIO_FORCE_IDENTIFY_SS   0x00000001L /* Identify */
#define MMIO_FORCE_IDENTIFY_FF   0x00000002L /* Identify */

#define MMIO_NOTRANSLATE         0x00000000L /* Translation */
#define MMIO_TRANSLATEDATA       0x00000001L /* Translation */
#define MMIO_TRANSLATEHEADER     0x00000002L /* Translation */
#define MMIO_DECOMPRESS          0x00000004L /* CODEC Decompress */

#define MMIO_DEFAULTBUFFER  8192    /* two pages under OS/2 2.0 */

#define MMIO_SEEK_IFRAME         0x00010000L  /* Seek to nearest previous IFRAME */



/*
 * Messages :
 */

#define MMIOM_START              0x0E00
#define MMIOM_END                0x0EFF

#define MMIOM_GETCF              MMIOM_START + 1
#define MMIOM_GETCFENTRY         MMIOM_START + 2

#define MMIOM_CLOSE              MMIOM_START + 3
#define MMIOM_OPEN               MMIOM_START + 4
#define MMIOM_READ               MMIOM_START + 5
#define MMIOM_SEEK               MMIOM_START + 6
#define MMIOM_WRITE              MMIOM_START + 7

#define MMIOM_IDENTIFYFILE       MMIOM_START + 8
#define MMIOM_GETHEADER          MMIOM_START + 9
#define MMIOM_SETHEADER          MMIOM_START + 10
#define MMIOM_QUERYHEADERLENGTH  MMIOM_START + 11
#define MMIOM_GETFORMATNAME      MMIOM_START + 12
#define MMIOM_GETFORMATINFO      MMIOM_START + 13
#define MMIOM_SEEKBYTIME         MMIOM_START + 14
#define MMIOM_TEMPCHANGE         MMIOM_START + 15
#define MMIOM_BEGININSERT        MMIOM_START + 16
#define MMIOM_ENDINSERT          MMIOM_START + 17
#define MMIOM_SAVE               MMIOM_START + 18
#define MMIOM_SET                MMIOM_START + 19
#define MMIOM_COMPRESS           MMIOM_START + 20
#define MMIOM_DECOMPRESS         MMIOM_START + 21
#define MMIOM_MULTITRACKREAD     MMIOM_START + 22
#define MMIOM_MULTITRACKWRITE    MMIOM_START + 23
#define MMIOM_DELETE             MMIOM_START + 24
#define MMIOM_BEGINGROUP         MMIOM_START + 25
#define MMIOM_ENDGROUP           MMIOM_START + 26
#define MMIOM_UNDO               MMIOM_START + 27
#define MMIOM_REDO               MMIOM_START + 28
#define MMIOM_BEGINSTREAM        MMIOM_START + 29
#define MMIOM_ENDSTREAM          MMIOM_START + 30


#define MMIOM_CUT                MMIOM_START + 31
#define MMIOM_COPY               MMIOM_START + 32
#define MMIOM_PASTE              MMIOM_START + 33
#define MMIOM_CLEAR              MMIOM_START + 34
#define MMIOM_STATUS             MMIOM_START + 35
#define MMIOM_WINMSG             MMIOM_START + 36
#define MMIOM_BEGINRECORD        MMIOM_START + 37
#define MMIOM_ENDRECORD          MMIOM_START + 38


/* These 3 new messages were added with feature 11710 */

#define MMIOM_QUERYIMAGE         MMIOM_START + 39
#define MMIOM_QUERYIMAGECOUNT    MMIOM_START + 40
#define MMIOM_SETIMAGE           MMIOM_START + 41


#define MMIO_REALTIME             0x00000001
#define MMIO_NONREALTIME          0x00000002

#define MMIOM_USER                0x0F00
#define MMIOM_USER_END            0x0FFF


/* Parameter structure for MMIOM_STATUS */

typedef struct _MMIO_STATUS_PARMS {       /* this comment needed by h2inc */
    HWND    hwndWindow;                   /* Some items require a window handle */
    ULONG   ulReturn;                     /* Return field */
    ULONG   ulItem;                       /* Use MCI_STATUS_... flags here */
    ULONG   ulValue;                      /* Status value field */
    ULONG   ulType;                       /* MCI_FORMAT_... of ulReturn */
} MMIO_STATUS_PARMS, *PMMIO_STATUS_PARMS;

/* Parameter structure for MMIOM_COPY, MMIOM_CUT, MMIOM_CLEAR and MMIOM_PASTE */

typedef LONG USEC;                        /* microsecond time format */

typedef struct _MMIO_MEDIT_PARMS {        /* this comment needed by h2inc */
   ULONG    ulStrucLen;                   /* length of this structure */
   HWND     hwndWindow;                   /* window handle */
   USEC     ulStartTime;                  /* starting time in usec */
   USEC     ulDuration;                   /* duration in usec */
   ULONG    ulCurrentFilePosition;        /* current file position in usec */
   ULONG    ulNewFilePosition;            /* returned by IO proc in usec, MCD will issue a seek */
   ULONG    ulNewFileLength;              /* return by IO proc in usec, MCD updates its headers */
   PVOID    pBuffer;                      /* optional buffer */
   ULONG    ulBufferLength;               /* optional buffer's length */
   PVOID    pHeader;                      /* optional pointer to header for buffer */
   BOOL     fUseBuffer;                   /* flag to use Buffer instead of clipboard */
} MMIO_EDIT_PARMS, *PMMIO_EDIT_PARMS;

/* Parameter structure for MMIOM_WIN_MESSAGE */

typedef struct _MMIO_WINMSG {             /* this comment needed by h2inc */
   HWND     hwndWindow;                   /* these are the parameters */
   USHORT   usMessage;                    /*  ... passed to the       */
   PVOID    pParam1;                      /*  ... window procedure    */
   PVOID    pParam2;                      /*  ... by PM               */
} MMIO_WINMSG, *PMMIO_WINMSG;

/* JPEG IOproc specific structure */

#define YUV_YVU           0x0080  /*v013 Compressed data is YUV */
#define DST_Y              20
#define DST_YY             25

typedef struct _JPEGOPTIONS   {            /* this comment needed by h2inc         */
    ULONG    ulStructLen;                    /* size of this sturcture                   */
    USHORT  usQuantization[4];              /* Each number may be 1 - 65535       */
    USHORT  usScale;                        /* 1 (1/8 Size) - 8 (Full Size; default)   */
    ULONG    ulColorOrder;                   /* YUV_YVU (Default) or ~YUV_YVU      */
    USHORT  usColorSpaceOut;               /* DST_YY (Default) or DST_Y            */
    } JPEGOPTIONS;

typedef JPEGOPTIONS *PJPEGOPTIONS;

/*
 * Include error codes for MMIO only.
 */

#ifndef _MEERROR_H_
#define INCL_MMIO_ERRORS
#include <meerror.h>
#endif

#define MMIO_SUCCESS                    0L
#define MMIO_WARNING                    2L
#define MMIO_ERROR                      0xFFFFFFFF
#define MMIOERR_UNSUPPORTED_MESSAGE     0xFFFFFFFE

#define MMIO_CF_SUCCESS                 0L
#define MMIO_CF_FAILURE                 1L

#define mmioFOURCC( ch0, ch1, ch2, ch3 )                         \
                  ( (ULONG)(BYTE)(ch0) | ( (ULONG)(BYTE)(ch1) << 8 ) |    \
                  ( (ULONG)(BYTE)(ch2) << 16 ) | ( (ULONG)(BYTE)(ch3) << 24 ) )

#define FOURCC_RIFF   mmioFOURCC( 'R', 'I', 'F', 'F' )
#define FOURCC_LIST   mmioFOURCC( 'L', 'I', 'S', 'T' )
#define FOURCC_MEM    mmioFOURCC( 'M', 'E', 'M', ' ' )
#define FOURCC_DOS    mmioFOURCC( 'D', 'O', 'S', ' ' )
#define FOURCC_BND    mmioFOURCC( 'B', 'N', 'D', ' ' )
#define FOURCC_FREE   mmioFOURCC( 'F', 'R', 'E', 'E' )
#define FOURCC_DEL    mmioFOURCC( 'D', 'E', 'L', ' ' )
#define FOURCC_CTOC   mmioFOURCC( 'C', 'T', 'O', 'C' )
#define FOURCC_CGRP   mmioFOURCC( 'C', 'G', 'R', 'P' )
#define FOURCC_CF     mmioFOURCC( 'C', 'F', ' ', ' ' )

#define MMIO_NLS_CHARSET_INFO    8000   /* RCDATA Name ID for NLS          */
#define MMIO_IOPROC_NAME_TABLE   8500   /* RCDATA Name ID for string table */
#define MMIO_CODEC_NAME_TABLE    9000   /* RCDATA Name ID for Codec  table */

/*
 * Numeric equivalents of fourcc's.  These are needed for the resource
 * compiler.
 */

#define HEX_FOURCC_DOS 0x20534f44L
#define HEX_FOURCC_MEM 0x204d454dL
#define HEX_FOURCC_BND 0x20444e42L
#define HEX_FOURCC_CF  0x20204643L

/*
 * Country codes (CC), languages (LC), and dialects (DC).
 */

#define MMIO_DEFAULT_CODE_PAGE            437

#define MMIO_CC_NONE                        0
#define MMIO_CC_USA                         1
#define MMIO_CC_CANADA                      2
#define MMIO_CC_LATIN_AMERICA               3
#define MMIO_CC_GREECE                     30
#define MMIO_CC_NETHERLANDS                31
#define MMIO_CC_BELGIUM                    32
#define MMIO_CC_FRANCE                     33
#define MMIO_CC_SPAIN                      34
#define MMIO_CC_ITALY                      39
#define MMIO_CC_SWITZERLAND                41
#define MMIO_CC_AUSTRIA                    43
#define MMIO_CC_UNITED_KINGDOM             44
#define MMIO_CC_DENMARK                    45
#define MMIO_CC_SWEDEN                     46
#define MMIO_CC_NORWAY                     47
#define MMIO_CC_WEST_GERMANY               49
#define MMIO_CC_MEXICO                     52
#define MMIO_CC_BRAZIL                     55
#define MMIO_CC_AUSTRALIA                  61
#define MMIO_CC_NEW_ZEALAND                64
#define MMIO_CC_JAPAN                      81
#define MMIO_CC_KOREA                      82
#define MMIO_CC_CHINA                      86
#define MMIO_CC_TAIWAN                     88
#define MMIO_CC_TURKEY                     90
#define MMIO_CC_PORTUGAL                  351
#define MMIO_CC_LUXEMBOURG                352
#define MMIO_CC_ICELAND                   354
#define MMIO_CC_FINLAND                   358

#define MMIO_LC_NONE                        0
#define MMIO_DC_NONE                        0
#define MMIO_LC_ARABIC                      1
#define MMIO_DC_ARABIC                      1
#define MMIO_LC_BULGARIAN                   2
#define MMIO_DC_BULGARIAN                   1
#define MMIO_LC_CATALAN                     3
#define MMIO_DC_CATALAN                     1
#define MMIO_LC_TRADITIONAL_CHINESE         4
#define MMIO_DC_TRADITIONAL_CHINESE         1
#define MMIO_LC_SIMPLE_CHINESE              4
#define MMIO_DC_SIMPLE_CHINESE              2
#define MMIO_LC_CZECH                       5
#define MMIO_DC_CZECH                       1
#define MMIO_LC_DANISH                      6
#define MMIO_DC_DANISH                      1
#define MMIO_LC_GERMAN                      7
#define MMIO_DC_GERMAN                      1
#define MMIO_LC_SWISS_GERMAN                7
#define MMIO_DC_SWISS_GERMAN                2
#define MMIO_LC_GREEK                       8
#define MMIO_DC_GREEK                       1
#define MMIO_LC_US_ENGLISH                  9
#define MMIO_DC_US_ENGLISH                  1
#define MMIO_LC_UK_ENGLISH                  9
#define MMIO_DC_UK_ENGLISH                  2
#define MMIO_LC_SPANISH                    10
#define MMIO_DC_SPANISH                     1
#define MMIO_LC_SPANISH_MEXICAN            10
#define MMIO_DC_SPANISH_MEXICAN             2
#define MMIO_LC_FINNISH                    11
#define MMIO_DC_FINNISH                     1
#define MMIO_LC_FRENCH                     12
#define MMIO_DC_FRENCH                      1
#define MMIO_LC_BELGIAN_FRENCH             12
#define MMIO_DC_BELGIAN_FRENCH              2
#define MMIO_LC_CANADIAN_FRENCH            12
#define MMIO_DC_CANADIAN_FRENCH             3
#define MMIO_LC_SWISS_FRENCH               12
#define MMIO_DC_SWISS_FRENCH                4
#define MMIO_LC_HEBREW                     13
#define MMIO_DC_HEBREW                      1
#define MMIO_LC_HUNGARIAN                  14
#define MMIO_DC_HUNGARIAN                   1
#define MMIO_LC_ICELANDIC                  15
#define MMIO_DC_ICELANDIC                   1
#define MMIO_LC_ITALIAN                    16
#define MMIO_DC_ITALIAN                     1
#define MMIO_LC_SWISS_ITALIAN              16
#define MMIO_DC_SWISS_ITALIAN               2
#define MMIO_LC_JAPANESE                   17
#define MMIO_DC_JAPANESE                    1
#define MMIO_LC_KOREAN                     18
#define MMIO_DC_KOREAN                      1
#define MMIO_LC_DUTCH                      19
#define MMIO_DC_DUTCH                       1
#define MMIO_LC_BELGIAN_DUTCH              19
#define MMIO_DC_BELGIAN_DUTCH               2
#define MMIO_LC_NORWEGIAN_BOKMAL           20
#define MMIO_DC_NORWEGIAN_BOKMAL            1
#define MMIO_LC_NORWEGIAN_NYNORSK          20
#define MMIO_DC_NORWEGIAN_NYNORSK           2
#define MMIO_LC_POLISH                     21
#define MMIO_DC_POLISH                      1
#define MMIO_LC_BRAZILIAN_PORTUGUESE       22
#define MMIO_DC_BRAZILIAN_PORTUGUESE        1
#define MMIO_LC_PORTUGUESE                 22
#define MMIO_DC_PORTUGUESE                  2
#define MMIO_LC_RHAETO_ROMANIC             23
#define MMIO_DC_RHAETO_ROMANIC              1
#define MMIO_LC_ROMANIAN                   24
#define MMIO_DC_ROMANIAN                    1
#define MMIO_LC_RUSSIAN                    25
#define MMIO_DC_RUSSIAN                     1
#define MMIO_LC_SERBO_CROATIAN_LATIN       26
#define MMIO_DC_SERBO_CROATIAN_LATIN        1
#define MMIO_LC_SERBO_CROATIAN_CYRILLIC    26
#define MMIO_DC_SERBO_CROATIAN_CYRILLIC     2
#define MMIO_LC_SLOVAK                     27
#define MMIO_DC_SLOVAK                      1
#define MMIO_LC_ALBANIAN                   28
#define MMIO_DC_ALBANIAN                    1
#define MMIO_LC_SWEDISH                    29
#define MMIO_DC_SWEDISH                     1
#define MMIO_LC_THAI                       30
#define MMIO_DC_THAI                        1
#define MMIO_LC_TURKISH                    31
#define MMIO_DC_TURKISH                     1
#define MMIO_LC_URDU                       32
#define MMIO_DC_URDU                        1
#define MMIO_LC_BAHASA                     33
#define MMIO_DC_BAHASA                      1

/*
 * Base function prototypes:
 */

USHORT APIENTRY mmioAdvance( HMMIO hmmio,
                           PMMIOINFO pmmioinfo,
                           USHORT usFlags );

USHORT APIENTRY mmioAscend( HMMIO hmmio,
                          PMMCKINFO pckinfo,
                          USHORT usFlags );

USHORT APIENTRY mmioClose( HMMIO hmmio,
                         USHORT usFlags );

USHORT APIENTRY mmioCreateChunk( HMMIO hmmio,
                               PMMCKINFO pckinfo,
                               USHORT usFlags );

USHORT APIENTRY mmioDescend( HMMIO hmmio,
                           PMMCKINFO pckinfo,
                           PMMCKINFO pckinfoParent,
                           USHORT usFlags );

USHORT APIENTRY mmioFlush( HMMIO hmmio,
                         USHORT usFlags );

USHORT APIENTRY mmioGetInfo( HMMIO hmmio,
                           PMMIOINFO pmmioinfo,
                           USHORT usFlags );

ULONG APIENTRY mmioGetLastError( HMMIO hmmio );

PMMIOPROC APIENTRY mmioInstallIOProc( FOURCC fccIOProc,
                                      PMMIOPROC pIOProc,
                                      ULONG ulFlags );

LONG APIENTRY mmioIOProc( PMMIOINFO pmmioinfo,
                          USHORT usMsg,
                          LONG lParam1,
                          LONG lParam2 );

HMMIO APIENTRY mmioOpen( PSZ pszFileName,
                         PMMIOINFO pmmioinfo,
                         ULONG ulOpenFlags );

LONG APIENTRY mmioRead( HMMIO hmmio,
                        PCHAR pchBuffer,
                        LONG cBytes );

LONG APIENTRY mmioSeek( HMMIO hmmio,
                        LONG lOffset,
                        LONG lOrigin );

LONG APIENTRY mmioSendMessage( HMMIO hmmio,
                               USHORT usMsg,
                               LONG lParam1,
                               LONG lParam2 );

USHORT APIENTRY mmioSetBuffer( HMMIO hmmio,
                             PCHAR pchBuffer,
                             LONG cBytes,
                             USHORT usFlags );

USHORT APIENTRY mmioSetInfo( HMMIO hmmio,
                           PMMIOINFO pmmioinfo,
                           USHORT usFlags );

FOURCC APIENTRY mmioStringToFOURCC( PSZ pszString,
                                    USHORT usFlags );

LONG APIENTRY mmioWrite( HMMIO hmmio,
                         PCHAR pchBuffer,
                         LONG cBytes );

/*
 * Compound File function prototypes:
 */

HMMCF APIENTRY mmioCFOpen( PSZ pszFileName,
                           PMMCFINFO pmmcfinfo,
                           PMMIOINFO pmmioinfo,
                           ULONG ulFlags );

ULONG APIENTRY mmioCFClose( HMMCF hmmcf,
                            ULONG ulFlags );

ULONG APIENTRY mmioCFGetInfo( HMMCF hmmcf,
                              PMMCFINFO pmmcfinfo,
                              ULONG cBytes );

ULONG APIENTRY mmioCFSetInfo( HMMCF hmmcf,
                              PMMCFINFO pmmcfinfo,
                              ULONG cBytes );

ULONG APIENTRY mmioCFFindEntry( HMMCF hmmcf,
                                PMMCTOCENTRY pmmctocentry,
                                ULONG ulFlags );

ULONG APIENTRY mmioCFAddEntry( HMMCF hmmcf,
                               PMMCTOCENTRY pmmctocentry,
                               ULONG ulFlags );

ULONG APIENTRY mmioCFChangeEntry( HMMCF hmmcf,
                                  PMMCTOCENTRY pmmctocentry,
                                  ULONG ulFlags );

ULONG APIENTRY mmioCFDeleteEntry( HMMCF hmmcf,
                                  PMMCTOCENTRY pmmctocentry,
                                  ULONG ulFlags );

ULONG APIENTRY mmioCFAddElement( HMMCF hmmcf,
                                 PSZ pszElementName,
                                 FOURCC fccType,
                                 PCHAR pchBuffer,
                                 LONG cchBytes,
                                 ULONG ulFlags );

ULONG APIENTRY mmioCFCopy( HMMCF hmmcfSource,
                           PSZ pszDestFileName,
                           ULONG ulFlags );

/*
 * Conversion Utility function prototypes:
 */

ULONG APIENTRY mmioQueryFormatCount( PMMFORMATINFO pmmformatinfo,
                                     PLONG plNumFormats,
                                     ULONG ulReserved,
                                     ULONG ulFlags );

ULONG APIENTRY mmioGetFormats( PMMFORMATINFO pmmformatinfo,
                               LONG lNumFormats,
                               PVOID pFormatInfoList,
                               PLONG plFormatsRead,
                               ULONG ulReserved,
                               ULONG ulFlags );

ULONG APIENTRY mmioGetFormatName( PMMFORMATINFO pmmformatinfo,
                                  PSZ pszFormatName,
                                  PLONG plBytesRead,
                                  ULONG ulReserved,
                                  ULONG ulFlags );

ULONG APIENTRY mmioIdentifyFile( PSZ pszFileName,
                                 PMMIOINFO pmmioinfo,
                                 PMMFORMATINFO pmmformatinfo,
                                 PFOURCC pfccStorageSystem,
                                 ULONG ulReserved,
                                 ULONG ulFlags );

ULONG APIENTRY mmioQueryHeaderLength( HMMIO hmmio,
                                      PLONG plHeaderLength,
                                      ULONG ulReserved,
                                      ULONG ulFlags );

ULONG APIENTRY mmioGetHeader( HMMIO hmmio,
                              PVOID pHeader,
                              LONG lHeaderLength,
                              PLONG plBytesRead,
                              ULONG ulReserved,
                              ULONG ulFlags );

ULONG APIENTRY mmioSetHeader( HMMIO hmmio,
                              PVOID pHeader,
                              LONG lHeaderLength,
                              PLONG plBytesWritten,
                              ULONG ulReserved,
                              ULONG ulFlags );

ULONG APIENTRY mmioIniFileHandler( PMMINIFILEINFO pmminifileinfo,
                                   ULONG ulFlags );

ULONG APIENTRY mmioIdentifyStorageSystem( PSZ pszFileName,
                                          PMMIOINFO pmmioinfo,
                                          PFOURCC pfccStorageSystem );

ULONG APIENTRY mmioDetermineSSIOProc( PSZ pszFileName,
                                      PMMIOINFO pmmioinfo,
                                      PFOURCC pfccStorageSystem,
                                      PSZ pszParsedRemainder );

ULONG APIENTRY mmioQueryIOProcModuleHandle( PMMIOPROC pIOProc,
                                            PHMODULE phmodIOProc );

ULONG APIENTRY mmioCFCompact( PSZ pszFileName, ULONG ulFlags );


/*
 * MMPMMMIO.INI file migration utility
 */

ULONG APIENTRY mmioMigrateIniFile( ULONG ulFlags );

/*
 * MMIO CODEC APIs
 */

#ifdef INCL_MMIO_CODEC

ULONG APIENTRY mmioIniFileCODEC( PCODECINIFILEINFO pCODECIniFileInfo,
                                 ULONG ulFlags );
ULONG APIENTRY mmioSet(HMMIO hmmio,
                       PMMEXTENDINFO pUserExtendmminfo,
                       ULONG ulFlags);

ULONG APIENTRY mmioQueryCODECName( PCODECINIFILEINFO pCODECIniFileinfo,
                                  PSZ pszCODECName,
                                  PULONG pulBytesRead );

ULONG APIENTRY mmioQueryCODECNameLength( PCODECINIFILEINFO pCODECIniFileinfo,
                                         PULONG pulNameLength );

PCODECPROC  APIENTRY mmioLoadCODECProc( PCODECINIFILEINFO pCODECIniFileInfo,
                                      PHMODULE phMod, ULONG ulFlags );

USHORT APIENTRY mmioGetData( HMMIO hmmio,
                           PMMIOINFO pmmioinfo,
                           USHORT usFlags );

#endif


#pragma pack()

#endif /* _MMIO_H_ */

#ifdef __cplusplus
}
#endif


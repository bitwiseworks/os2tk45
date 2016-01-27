/**************************START OF SPECIFICATIONS **************************/
/*                                                                          */
/* COPYRIGHT:     IBM - International Business Machines                     */
/*              Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                               */
/*                                                                          */
/*                                                                          */
/* SOURCE FILE NAME:  UMFORMAT.H                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: Ultimotion File Format Headers (Beta)                  */
/*                                                                          */
/*   An Ultimotion file consists of interlevaed audio and video and text    */
/*   chunks within the data list chunk.                                     */
/*                                                                          */
/*   A file may contain interleaved audio chunks or the soundtrack may      */
/*   be contained in a seperate file specified in the header chunk, or      */
/*   there may be no associated soundtrack (a silent movie).  Multiple      */
/*   audio tracks are supported by this file format, either interleaved     */
/*   or in seperate files.  Only one video track, track 0, is supported in  */
/*   a single file.  Audio tracks are identified by a track number, with    */
/*   the first audio track being track 1.                                   */
/*                                                                          */
/*   If a file specifies both interleaved audio tracks and external         */
/*   audio tracks (.WAV files), the external audio tracks are numbered      */
/*   consecutively following the interleaved audio tracks.  For             */
/*   example, if a file specifies two interleaved audio tracks and two      */
/*   external audio tracks, the interleaved audio tracks will be tracks     */
/*   0 and 1, and the external audio tracks will be tracks 2 and 3.         */
/*   Note that tracks can carry distinct audio information such as          */
/*   multiple languages, and that stereo audio can be carried in a          */
/*   single track.                                                          */
/*                                                                          */
/************************** END OF SPECIFICATIONS ***************************/
#ifdef __cplusplus
   extern "C" {
#endif


/* XLATOFF */
#pragma pack(1)
/* XLATON */


/* The hex fourcc value is also the SPCBKEY.ulDataSubType value for the */
/* Ultimotion file format - OS2MEDEF.H                                  */
#define     FOURCC_FFIO                 mmioFOURCC('U', 'L', 'T', 'I')
#define     HEX_FOURCC_FFIO             0x49544c55L

/****************************************************************************/
/* Ultimotion File Format:                                                  */
/*                                                                          */
/*                                                                          */
/*    Conventions used in this format:                                      */
/*                                                                          */
/*    - A file is a collection of chunks                                    */
/*    - The first ULONG in a chunk is the chunk type (chunk ID)             */
/*    - The second ULONG in a chunk is the length of the chunk,             */
/*         including the header and the data, but excluding the chunk       */
/*         ID and chunk length.                                             */
/*    - For each interleaved audio track there is one audio frame that      */
/*         corresponds to each video frame                                  */
/*    - An audio frame has the same duration as its corresponding video     */
/*         frame                                                            */
/*    - Audio frames always follow their corresponding video frame, and     */
/*         as such preceed the next video frame                             */
/*                                                                          */
/* Legend:                                                                  */
/*     [<element name>]      optional                                       */
/*     <element name>        1                                              */
/*     <element name>...     1 or more                                      */
/*     [<element name>]...   0 or more                                      */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*  SMV form                                                                */
/*  ---------                                                               */
/*                                                                          */
/* <RIFF( 'ummv'                                                            */
/*       <LIST( 'umhl'                             - Header LIST            */
/*             <Videoheader-ck>                    - Video header chunk     */
/*             <videotitle-ck>...                  - Video title chunk(s)   */
/*                                                 -                        */
/*             [<LIST( 'umal'                      - Audio LIST(s)          */
/*                    {<extaudiofilename-ck> |     - Ext. WAV files         */
/*                     <audioheader-ck>}           - Audio header           */
/*                    <audiotitle-ck>...)>]...     - Audio title            */
/*                                                 -                        */
/*             [<LIST( 'umtl'                      - Text LIST              */
/*                     <textheader-ck>...)>])>     - Text header            */
/*                                                 -                        */
/*       <LIST( 'umfd'                             - Frame data LIST        */
/*             <LIST( 'umcd' {                     - Chapter data LIST      */
/*                   <videoframe-ck>               - Video frame            */
/*                   [<audioframe-ck>]             - Audio frame            */
/*                   [<textframe-ck>]}...)>...)    - Text frame             */
/*                                                 -                        */
/*        [<LIST( 'umci'                           - Chap index table LIST  */
/*               <LIST( 'umce'                     - Chap index entry LIST  */
/*                     <chapterheader-ck>          - Chapter index hdr      */
/*                     <chapttitle-ck>...          - Chapter title          */
/*                     <frameindex-ck>)>...)>])>   - frame index            */
/*                                                 -  table                 */
/*                                                                          */
/****************************************************************************/

#define UMAUDIOFILENAMELENGTH   60
#define SIZEOF_FORM         4

/* ULTIMOTION FORMS */
#define  UMFORM_MOVIE                   "ummv"
#define  UMFORM_HEADERLIST              "umhl"
#define  UMFORM_AUDIOHDRLIST            "umal"
#define  UMFORM_TEXTHDRLIST             "umtl"
#define  UMFORM_FRAMEDATALIST           "umfd"
#define  UMFORM_CHAPTERDATALIST         "umcd"
#define  UMFORM_CHAPTERTABLELIST        "umci"
#define  UMFORM_CHAPTERENTRYLIST        "umce"

/* byte swapped hex defines for ulong assignments... */
#define  HEX_UMFORM_MOVIE               0x766d6d75L     /* vmmu */
#define  HEX_UMFORM_HEADERLIST          0x6c686d75L     /* lhmu */
#define  HEX_UMFORM_AUDIOHDRLIST        0x6c616d75L     /* lamu */
#define  HEX_UMFORM_TEXTHDRLIST         0x6c746d75L     /* ltmu */
#define  HEX_UMFORM_FRAMEDATALIST       0x64666d75L     /* dfmu */
#define  HEX_UMFORM_CHAPTERDATALIST     0x64636d75L     /* dcmu */
#define  HEX_UMFORM_CHAPTERTABLELIST    0x69636d75L     /* icmu */
#define  HEX_UMFORM_CHAPTERENTRYLIST    0x65636d75L     /* ecmu */

/* ULTIMOTION CHUNK IDS */
#define  UMID_VIDEOHEADER               "umvh"
#define  UMID_EXTAUDIONAME              "umea"
#define  UMID_AUDIOHEADER               "umah"
#define  UMID_TEXTHEADER                "umth"
#define  UMID_VIDEOTITLE                "umvt"
#define  UMID_AUDIOTITLE                "umat"
#define  UMID_TEXTFRAME                 "umtf"
#define  UMID_VIDEOFRAME                "umvf"
#define  UMID_AUDIOFRAME                "umaf"
#define  UMID_CHAPTERHEADER             "umch"
#define  UMID_CHAPTERTITLE              "umct"
#define  UMID_FRAMETABLE                "umfi"

/* byte swapped hex defines for ulong assignments... */
#define  HEX_UMID_VIDEOHEADER           0x68766d75L      /* hvmu */
#define  HEX_UMID_EXTAUDIONAME          0x61656d75L      /* aemu */
#define  HEX_UMID_AUDIOHEADER           0x68616d75L      /* hamu */
#define  HEX_UMID_TEXTHEADER            0x68746d75L      /* htmu */
#define  HEX_UMID_VIDEOTITLE            0x74766d75L      /* tvmu */
#define  HEX_UMID_AUDIOTITLE            0x74616d75L      /* tamu */
#define  HEX_UMID_TEXTFRAME             0x66746d75L      /* ftmu */
#define  HEX_UMID_VIDEOFRAME            0x66766d75L      /* fvmu */
#define  HEX_UMID_AUDIOFRAME            0x66616d75L      /* famu */
#define  HEX_UMID_CHAPTERHEADER         0x68636d75L      /* hcmu */
#define  HEX_UMID_CHAPTERTITLE          0x74636d75L      /* tcmu */
#define  HEX_UMID_FRAMETABLE            0x69666d75L      /* ifmu */



/****************************************************************************/
/* CHUNK HEADER STRUCTURE - APPEARS AT START OF EACH CHUNK                  */
/****************************************************************************/
typedef struct _UMCHUNKHEADER {  /* umch */
   ULONG          ulChunkID;              /* ID for this chunk                   */
   ULONG          ulChunkLength;          /* Length of chunk that follows        */
}  UMCHUNKHEADER;
typedef UMCHUNKHEADER *PUMCHUNKHEADER;   /* pumch */


/****************************************************************************/
/* VIDEO HEADER CHUNK -                                                     */
/*                                                                          */
/* The UMVIDEOHEADER structure is followed by a list of                     */
/* usInterleavedTracks null-terminated external audio (.WAV) file           */
/* names.  The audio file name fields are fixed size to better enable       */
/* changing the file names without re-writing the file.                     */
/****************************************************************************/
typedef struct _UMVIDEOHEADER {   /* umvh */
   ULONG         ulTotalDuration;        /* Video duration in MMTIME            */
   ULONG         ulMaxFrameSize;         /* Max video frame size in bytes       */
   ULONG         ulMaxAvgDataRate;       /* Max avergage data rate              */
   ULONG         ulMaxBurstInterval;     /* Max interval for max data rate      */
   ULONG         ulCompressionRatioX100; /* Compression ratio                   */
   ULONG         ulPosterFrameOffset;    /* Poster for video (from beginning)   */
   USHORT        usMaxFrameX;            /* Max horizontal frame size           */
   USHORT        usMaxFrameY;            /* Max vertical frame size             */
   USHORT        usNomFrameDuration;     /* Nominal recorded frame duration     */
   USHORT        usNomIFrameInterval;    /* I-frame every this many frames      */
   USHORT        usNumCompressionTypes;  /* number of different CODECS used in file */
   ULONG         aulCompressionTypes[20];/* List of CODECs found in this file */
   ULONG         Reserved[16];           /* Reserved space */
}  UMVIDEOHEADER;
typedef UMVIDEOHEADER *PUMVIDEOHEADER;  /* pumvh */


/****************************************************************************/
/* EXTERNAL AUDIO FILE NAME CHUNK                                           */
/****************************************************************************/
typedef struct _EXTAUDIONAME  {   /* umea */
   USHORT        usTrackNumber;          /* Audio track number for this format  */
   CHAR          szFileName[UMAUDIOFILENAMELENGTH];
}  EXTAUDIONAME;
typedef EXTAUDIONAME *PEXTAUDIONAME;  /* pumea */


/****************************************************************************/
/* INTERLEAVED AUDIO TRACK HEADER CHUNK                                     */
/****************************************************************************/
typedef struct  _AUDIOHEADER {   /* umah */
   USHORT        usTrackNumber;          /* Audio track number for this format  */
   USHORT        usFormatTag;            /* Type of wave format                 */
   USHORT        usChannels;             /* Number of channels (1=mono 2=stereo)*/
   ULONG         ulSamplesPerSec;        /* Sampling rate                       */
   ULONG         ulAvgBytesPerSec;       /* Avg bytes per sec                   */
   USHORT        usBlockAlign;           /* Block alignment in bytes            */
   USHORT        usBitsPerSample;        /* Bits per sample                     */
   USHORT        usCountryCode;          /* Country code for this title         */
   } AUDIOHEADER;
typedef AUDIOHEADER *PAUDIOHEADER; /* pumah */


/****************************************************************************/
/* INTERLEAVED TEXT TRACK HEADER CHUNK                                      */
/****************************************************************************/
typedef struct  _TEXTHEADER {   /* umth */
   USHORT        usTrackNumber;          /* Audio track number for this format  */
   USHORT        usCountryCode;          /* Country code for this title         */
   } TEXTHEADER;
typedef TEXTHEADER *PTEXTHEADER; /* pumth */


/****************************************************************************/
/* TITLE CHUNK                                                              */
/****************************************************************************/
typedef struct _TITLE {  /* ttl */
   USHORT        usCountryCode;         /* Country code for this title         */
   CHAR          szTitle[1];            /* Video title null-terminated         */
   } TITLE;
typedef TITLE *PTITLE; /* pttl */


/****************************************************************************/
/* AUDIO FRAME CHUNK                                                        */
/****************************************************************************/
typedef struct _AUDIOFRAME { /* umaf */
   USHORT        usTrackNumber;         /* audio track number                  */
   BYTE          bData[1];
}  AUDIOFRAME;
typedef AUDIOFRAME *PAUDIOFRAME; /* pumaf */


/****************************************************************************/
/* TEXT FRAME CHUNK                                                         */
/****************************************************************************/
typedef struct _TEXTFRAME {  /* umtf */
   USHORT        usTrackNumber;         /* Text track number                   */
   CHAR          szText[1];             /* Text null-terminated                */
   } TEXTFRAME;


/****************************************************************************/
/* VIDEO FRAME CHUNK                                                        */
/****************************************************************************/
typedef struct _VIDEOFRAME { /* umvf */
   USHORT        usTrackNumber;         /* Video track number  (0L only for now) */
   ULONG         ulDuration;            /* Frame duration in MMTIME            */
   ULONG         ulFrameFlags;          /* Frame flags                         */
   ULONG         ulCompressionType;     /* Compression type                    */
   BYTE          bData[1];
} VIDEOFRAME;
typedef VIDEOFRAME *PVIDEOFRAME; /* pumvf */

/* ulCompressionType defines: */
#define  UM_VIDEO_COMPRESSION_TYPE_RAWRGB565        1L
#define  UM_VIDEO_COMPRESSION_TYPE_RAWUVY556        2L
#define  UM_VIDEO_COMPRESSION_TYPE_RAWYUV411        3L
#define  UM_VIDEO_COMPRESSION_TYPE_BH146           10L   /* BETA-RGB16 */
#define  UM_VIDEO_COMPRESSION_TYPE_BH211SCS4       11L   /* 1.1 YUV16 subsampled chroma sub4 */
#define  UM_VIDEO_COMPRESSION_TYPE_BH211UCS4       12L   /* unique chroma sub4 */

/* ulVideoFrameFlags defines: */
#define  UM_VIDEO_FRAME_FLAG_DELTAFRAME   0x1L   /* 1: delta frame, 0: I frame */
#define  UM_VIDEO_FRAME_FLAG_SCENECHANGE  0x2L


/****************************************************************************/
/* CHAPTER INDEX HEADER (header for each entry in chapter index LIST        */
/****************************************************************************/
typedef struct _UMCHAPTERINDEX {  /* umch */
   ULONG    ulChapterOffset;            /* Offset from beginning of file       */
   ULONG    ulChapterDuration;          /* Duration of chapter in MMTIME       */
   ULONG    ulPosterFrameOffset;        /* Poster for chapter (offset from chapter)*/
}  UMCHAPTERINDEX;
typedef UMCHAPTERINDEX *PUMCHAPTERINDEX;  /* pumch */


/****************************************************************************/
/* FRAME INDEX ENTRY                                                        */
/****************************************************************************/
typedef struct _UMFRAMEINDEX {  /* umfi */
   ULONG        ulFrameOffsets;         /* Offset from beginning of chapter    */
   ULONG        ulFrameFlags;           /* Frame flags (Refer to frame header) */
}  UMFRAMEINDEX;
typedef UMFRAMEINDEX *PUMFRAMEINDEX;  /* pumfi */


/* XLATOFF */
#pragma pack()
/* XLATON */

#ifdef __cplusplus
}
#endif


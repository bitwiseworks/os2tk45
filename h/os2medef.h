/****************************************************************************/
/*                                                                          */
/* Module Name: OS2MEDEF.H                                                  */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions Common Definitions file                   */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1991, 1992     */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
/* NOINC */
#ifdef __cplusplus
   extern "C" {
#endif
/* INC */


/* XLATOFF */
#ifndef  _OS2MEDEF_H
#define  _OS2MEDEF_H

#ifndef __WORDBYTEDWORD__
  #define __WORDBYTEDWORD__
  #ifndef WORD
    typedef USHORT WORD;
  #endif

  #ifndef DWORD
    typedef ULONG  DWORD;
  #endif
#endif

typedef WORD  VERSION;
typedef char FAR * LPSTR;
typedef FAR * LPTR;
typedef FAR * LPRECT;
typedef HWND HANDLE;
typedef WORD *  PW;
typedef WORD *  PAW;
typedef WORD * PWORD;
typedef PFN  FAR *  PPFN;
typedef DWORD (APIENTRY FNMCI) (LPTR, WORD, DWORD, DWORD, WORD);
typedef FNMCI *PFNMCI;

/* XLATON */

#ifdef XXXXX
typedef PVOID PFNMCI;
typedef PVOID LPTR;
#endif

typedef char   SZ;                    /* ASCIIZ char string type*/
typedef ULONG  MMTIME;                /* universal Chinatown time (1/3000 second)*/
typedef MMTIME FAR *PMMTIME;          /* Ptr to MMTIME unit*/

typedef ULONG HMMIO;                  /* Handle to an MMIO object*/
typedef HMMIO FAR *PHMMIO;            /* Handle to an MMIO object*/


/**********************************************
 *
 * SPCBKEY - Stream Protocol Key
 *
 **********************************************/
typedef struct _SPCBKEY { /* spcbkey SPCB key (Stream data type) */
   ULONG    ulDataType;
   ULONG    ulDataSubType;
   ULONG    ulIntKey;                 /* generated internal key            */
   } SPCBKEY;
typedef SPCBKEY FAR *PSPCBKEY;
#define  _SPCBKEY_DEFINED   1


/**********************************************
 *
 * MMTRACKINFO - This structure is used to
 *            represent a video, audio or
 *            some other type of track
 *            within a digital video movie
 *            file.
 *
 **********************************************/
typedef struct _MMTRACKINFO {       /* mmtrackinfo */
   ULONG       ulTrackID;         /* track identifier */
   ULONG       ulMediaType;       /* media type */
   ULONG       ulCountry;         /* country code for the track */
   ULONG       ulCodePage;        /* country code page for the track */
   ULONG       ulReserved1;       /* reserved must be 0 */
   ULONG       ulReserved2;       /* reserved must be 0 */
   } MMTRACKINFO;
typedef MMTRACKINFO  *PMMTRACKINFO;/* Ptr to a track table entry */


/************************
 * Clipboard formats
 ************************/
#define         CF_RMID                 10
#define         CF_RIFF                 11
#define         CF_WAVE                 12
#define         CF_AVI                  13

/************************
 * Resource formats
 ************************/
#define         RT_RMID                 100
#define         RT_RIFF                 101
#define         RT_WAVE                 102
#define         RT_AVI                  103
#define         RT_AVS                  104

/************************
 * Drag formats
 ************************/

#define         DRT_WAVE               "Digital audio"
#define         DRT_AVI                "Digital video"
#define         DRT_MIDI               "MIDI"

/************************
 * Sampling Rate
 ************************/
#define         HZ_8000                 8000            /* 8.0 kHz*/
#define         HZ_11025                11025           /* 11.025 kHz*/
#define         HZ_14700                14700           /* 14.700 kHz (SPV/2)*/
#define         HZ_18900                18900           /* 18.900 kHz (CD/XA LVL C)*/
#define         HZ_22050                22050           /* 22.050 kHz*/
#define         HZ_37800                37800           /* 37.800 kHz (CD/XA LVL B)*/
#define         HZ_44100                44100           /* 44.100 kHz*/

/************************
 * Bits per Sample
 ************************/
#define         BPS_4                   4               /* 4 bits/sample (ADPCM)*/
#define         BPS_8                   8               /* 8 bits/sample (PCM)*/
#define         BPS_16                  16              /* 16 bits/sample(PCM)*/

/************************
 * Channels
 ************************/
#define         CH_1                    1               /* Mono*/
#define         CH_2                    2               /* Stereo*/
#define         CH_4                    4               /* Quad*/

/*************************************************************************
 * Multimedia Datatypes (spcbkey values)
 *************************************************************************/


/****************
 * NULL datatype
 ****************/
#define  DATATYPE_NULL           0x0000L     /* NULL stream data type (NULL)*/
#define    SUBTYPE_NONE            0x0000L   /* No subtype for this data type*/


/*********************
 * WAVEFORM datatypes
 *********************/
#define  DATATYPE_WAVEFORM       0x0001L     /* WAVEFORM audio (PCM)*/

/* Subtypes for DATATYPE_WAVEFORM:*/
#define    WAVE_FORMAT_1M08        0x0001L   /* 11.025kHz, Mono  , 8-bit*/
#define    WAVE_FORMAT_1S08        0x0002L   /* 11.025kHz, Stereo, 8-bit*/
#define    WAVE_FORMAT_1M16        0x0003L   /* 11.025kHz, Mono  , 16-bit*/
#define    WAVE_FORMAT_1S16        0x0004L   /* 11.025kHz, Stereo, 16-bit*/
#define    WAVE_FORMAT_2M08        0x0005L   /* 22.05kHz , Mono  , 8-bit*/
#define    WAVE_FORMAT_2S08        0x0006L   /* 22.05kHz , Stereo, 8-bit*/
#define    WAVE_FORMAT_2M16        0x0007L   /* 22.05kHz , Mono  , 16-bit*/
#define    WAVE_FORMAT_2S16        0x0008L   /* 22.05kHz , Stereo, 16-bit*/
#define    WAVE_FORMAT_4M08        0x0009L   /* 44.1kHz  , Mono  , 8-bit*/
#define    WAVE_FORMAT_4S08        0x000aL   /* 44.1kHz  , Stereo, 8-bit*/
#define    WAVE_FORMAT_4M16        0x000bL   /* 44.1kHz  , Mono  , 16-bit*/
#define    WAVE_FORMAT_4S16        0x000cL   /* 44.1kHz  , Stereo, 16-bit*/
#define    WAVE_FORMAT_8M08        0x000dL   /*  8.0kHz  , Mono  , 8-bit*/
#define    WAVE_FORMAT_8S08        0x000eL   /*  8.0kHz  , Stereo, 8-bit*/
#define    WAVE_FORMAT_8M16        0x000fL   /*  8.0kHz  , Mono  , 16-bit*/
#define    WAVE_FORMAT_8S16        0x0010L   /*  8.0kHz  , Stereo, 16-bit*/


/*********************
 * DIGVIDEO datatypes
 *********************/
#define  DATATYPE_DIGVIDEO       0x0002L     /* Digital Video */

/* Subtypes for DATATYPE_DIGVIDEO:*/
#define    DIGVIDEO_PHOTOM         0x544F4850L   /* Photmotion video, FOURCC value for phio */
#define    DIGVIDEO_ULTIM          0x49544c55L   /* Ultimotion video, FOURCC value for ulio */
#define    DIGVIDEO_AVI            0x20495641L   /* AVI video, FOURCC value for avio */
#define    DIGVIDEO_MONITOR        0x206E6F6DL   /* MONITOR video, FOURCC value for monitoring */
#define    DIGVIDEO_FLC            0x63696C66L   /* FLIC video, FOURCC value for flic */
#define    DIGVIDEO_MPEG           0x4745504DL   /* MPEG video, FOURCC value for MPEG */


/******************
 * MuLaw datatypes
 ******************/
#define DATATYPE_MULAW           0x0101L     /* MuLaw*/
#define DATATYPE_RIFF_MULAW      0x0007L    /* MuLaw*/

/* Subtypes for DATATYPE_MULAW:*/
#define    MULAW_8B8KS             0x0001L   /* 8bit 8kHz stereo*/
#define    MULAW_8B11KS            0x0002L   /* 8bit 11kHz stereo*/
#define    MULAW_8B22KS            0x0003L   /* 8bit 22kHz stereo*/
#define    MULAW_8B44KS            0x0004L   /* 8bit 44kHz stereo*/
#define    MULAW_8B8KM             0x0005L   /* 8bit 8kHz mono*/
#define    MULAW_8B11KM            0x0006L   /* 8bit 11kHz mono*/
#define    MULAW_8B22KM            0x0007L   /* 8bit 22kHz mono*/
#define    MULAW_8B44KM            0x0008L   /* 8bit 44kHz mono*/

/*****************
 * ALaw datatypes
 *****************/
#define DATATYPE_ALAW            0x0102L     /* ALaw*/
#define DATATYPE_RIFF_ALAW       0x0006L     /* ALaw*/

/* Subtypes for DATATYPE_ALAW:*/
#define    ALAW_8B8KS              0x0001L   /* 8bit 8kHz stereo*/
#define    ALAW_8B11KS             0x0002L   /* 8bit 11kHz stereo*/
#define    ALAW_8B22KS             0x0003L   /* 8bit 22kHz stereo*/
#define    ALAW_8B44KS             0x0004L   /* 8bit 44kHz stereo*/
#define    ALAW_8B8KM              0x0005L   /* 8bit 8kHz mono*/
#define    ALAW_8B11KM             0x0006L   /* 8bit 11kHz mono*/
#define    ALAW_8B22KM             0x0007L   /* 8bit 22kHz mono*/
#define    ALAW_8B44KM             0x0008L   /* 8bit 44kHz mono*/


/*******************************
 * AVC ADPCM WAVEFORM datatypes
 *******************************/
#define  DATATYPE_ADPCM_AVC      0x0103L     /* ADPCM audio*/

/* Subtypes for DATATYPE_ADPCM_AVC:*/
#define    ADPCM_AVC_VOICE         0x0001L
#define    ADPCM_AVC_MUSIC         0x0002L
#define    ADPCM_AVC_STEREO        0x0003L
#define    ADPCM_AVC_HQ            0x0004L

/******************
 * CT_ADPCM datatypes
 ******************/
#define DATATYPE_CT_ADPCM          0x0200L     /* Creative technologies */

/* Subtypes for DATATYPE_CT_ADPCM:*/
#define    CT_ADPCM_16B8KS          0x0001L   /* 16bit 8kHz stereo*/
#define    CT_ADPCM_16B11KS         0x0002L   /* 16bit 11kHz stereo*/
#define    CT_ADPCM_16B22KS         0x0003L   /* 16bit 22kHz stereo*/
#define    CT_ADPCM_16B44KS         0x0004L   /* 16bit 44kHz stereo*/
#define    CT_ADPCM_16B8KM          0x0005L   /* 16bit 8kHz mono*/
#define    CT_ADPCM_16B11KM         0x0006L   /* 16bit 11kHz mono*/
#define    CT_ADPCM_16B22KM         0x0007L   /* 16bit 22kHz mono*/
#define    CT_ADPCM_16B44KM         0x0008L   /* 16bit 44kHz mono*/

/****************
 * MIDI datatype
 ****************/
#define  DATATYPE_MIDI           0x0201L     /* MIDI*/
/*         SUBTYPE_NONE            0x0000L      No subtype for this data type*/


/********************
 * GENERIC datatypes
 ********************/
#define  DATATYPE_GENERIC        0x0202L     /* Generic for files / memory. This*/
                                             /* datatype will match any other*/
                                             /* datatype at stream creation time.*/

/**************************
 * Speech Viewer datatypes
 **************************/
#define  DATATYPE_SPV2           0x0203L     /* Speech Viewer*/

/* Subtypes for DATATYPE_SPV2:*/
#define    SPV2_BPCM               0x0001L
#define    SPV2_PCM                0x0002L
#define    SPV2_NONE               0x0003L


/******************
 * CD-XA datatypes
 ******************/
#define  DATATYPE_CDXA_VIDEO     0x0204L     /* CD-XA Video datatype*/
#define  DATATYPE_CDXA_DATA      0x0205L     /* CD-XA sub-band data datatype*/
#define  DATATYPE_CDXA_AUDIO     0x0206L     /* CD-XA ADPCM Audio datatype*/
#define  DATATYPE_CDXA_AUDIO_HD  0x0207L     /* CD-XA ADPCM Audio with Headers*/
#define  DATATYPE_CDXA_HEADERS   0x0208L     /* CD-XA Headers and Subheaders*/


/* Subtypes for DATATYPE_CDXA_AUDIO:*/
#define    CDXA_LEVELB             0x0000L /* LEVEL B Audio Stereo*/
#define    CDXA_LEVELC             0x0001L /* LEVEL C Audio Stereo*/
#define    CDXA_LEVELB_MONO        0x0002L /* LEVEL B Audio Mono*/
#define    CDXA_LEVELC_MONO        0x0003L /* LEVEL C Audio Mono*/


/*********************
 * DIGISPEECH datatype
 *********************/
#define  DATATYPE_DIGISPEECH     0x0208L     /* DIGISPEECH */
#define    DIGISPEECH_NATIVE       0x0001L   /* Native digispeech */


/*********************
 * MPEG-1 Audio datatypes
 *********************/
#define  DATATYPE_MPEG1AUDIO       0x0050L     /* MPEG-1 Audio */

/* Subtypes for DATATYPE_MPEG1AUDIO:*/
#define    MPEG1_FORMAT_3M16        0x0001L   /* 32.000kHz,  Mono  , 16-bit*/
#define    MPEG1_FORMAT_3S16        0x0002L   /* 32.000kHz,  Stereo, 16-bit*/
#define    MPEG1_FORMAT_4M16        0x0003L   /* 44.1/48kHz, Mono  , 16-bit*/
#define    MPEG1_FORMAT_4S16        0x0004L   /* 44.1/48kHz, Stereo, 16-bit*/

/*******************
 * UNKNOWN datatype
 *******************/
#define  DATATYPE_UNKNOWN        0xFFFFFFFFL   /* Unknown datatype */
/*         SUBTYPE_NONE            0x0000L      No subtype for this data type*/


/* XLATOFF */
#endif
/* XLATON */

/* NOINC */
#ifdef __cplusplus
}
#endif
/* INC */


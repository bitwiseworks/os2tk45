/****************************************************************************/
/*                                                                          */
/* Module Name: VSDCMDS.H                                                   */
/*                                                                          */
/* This is the multimedia include file that has the typedefs, defines and   */
/* function prototyes for Vendor Specific Devices                           */
/*                                                                          */
/*         Copyright (c) International Business Machines Corporation        */
/*                          1991, 1992, 1993                                */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _VSDCMDS_H_
#define _VSDCMDS_H_

/* VSD common functions/commands                                                         */
#define VSD_DEVUNITS                  1
#define VSD_GETPRODINFO               2 /* naming consistency */
#define VSD_QUERYPRODINFO             2
#define VSD_OPEN                      3
#define VSD_CLOSE                     4
#define VSD_RESOURCE                  5 /* fix spelling error */
#define VSD_RESOUCRCE                 5
#define VSD_SAVE                      7
#define VSD_RESTORE                   8
#define VSD_INSERTSETTINGSPAGE        9
#define VSD_GETDEVCAPS               10
#define VSD_ESCAPE                   11
#define VSD_PLAY                     12
#define VSD_RECORD                   13
#define VSD_READ                     14
#define VSD_PEEKQUEUE                15
#define VSD_COMPLETE                 16
#define VSD_FLUSH                    17
#define VSD_EJECT                    18
#define VSD_LOAD                     19
#define VSD_RESET                    20
#define VSD_PARK                     21
#define VSD_EVENTS                   22
#define VSD_CAPTUREIMAGE             23
#define VSD_RESTOREIMAGE             24
#define VSD_FREEZE                   25
#define VSD_SET                      26
#define VSD_QUERY                    27
#define VSD_SEEK                     28
#define VSD_STEP                     29
#define VSD_DDCMD                    31     /* DDMCD information */
#define VSD_STOP                     40
#define VSD_USER                     1000


/* VSD common Return Codes                                                                */
#ifndef INCL_AUDIO_VSD
#define VSDERR_SUCCESS                       0
#define VSDERR_REQUEST_BUF_TOO_SMALL         1
#define VSDERR_UNSUPPORTED_COMMAND           2
#define VSDERR_INVALID_HVSD                  3
#define VSDERR_INVALID_DATATYPE              4
#define VSDERR_INVALID_CATEGORY              5
#define VSDERR_CANNOT_READ                   6
#define VSDERR_CANNOT_WRITE                  7
#define VSDERR_INSUFFICIENT_MEMORY           8
#define VSDERR_NO_DEVICE_DRIVER              9
#define VSDERR_HARDWARE                     10
#define VSDERR_CANNOT_PARK                  11
#define VSDERR_UNSUPPORTED_INPUT            12
#define VSDERR_UNSUPPORTED_OUTPUT           13
#define VSDERR_INVALID_INPUT                14
#define VSDERR_INVALID_OUTPUT               15
#define VSDERR_TOO_MANY_INPUTS              16
#define VSDERR_TOO_MANY_OUTPUTS             17
#define VSDERR_INVALID_EVENT                18
#define VSDERR_CANNOT_DISABLE               19
#define VSDERR_EVENT_OUT_OF_RANGE           20
#define VSDERR_UNSUPPORTED_EVENT            21
#define VSDERR_VOL_OUT_OF_RANGE             22
#define VSDERR_VECT_OUT_OF_RANGE            23
#define VSDERR_OUTOFRANGE                   24
#define VSDERR_UNSUPPORTED_DATA_TYPE        25
#define VSDERR_INVALID_DATA_TYPE            26
#define VSDERR_UNSUPPORTED_AUDIO_SETTING    27
#define VSDERR_INVALID_AUDIO_SETTING        28
#define VSDERR_UNSUPPORTED_CONNECTOR        29
#define VSDERR_INVALID_CONNECTOR            30
#define VSDERR_MISSING_PARAMETER            31
#define VSDERR_INSTANCE_INACTIVE            32
#define VSDERR_WARNING                      33
#define VSDERR_CANNOT_CLOSE                 34
#define VSDERR_TOO_FEW_SLOTS                35
#define VSDERR_INVALID_MODE                 36
#define VSDERR_INVALID_BUFF                 37
#define VSDERR_DEVICE_REJECTED              38
#define VSDERR_UNSUPPORTED_FLAG             39
#define VSDERR_DEVICE_IN_USE                99
                                                /* KLL Tuner Support - Start */
/*    SetTuner_Channel return codes       */
#define VSDERR_CHANNEL_LOW                  41
#define VSDERR_CHANNEL_HIGH                 42
#define VSDERR_CHANNEL_SKIP                 43
#define VSDERR_CHANNEL_NO_HW                48
                                                /* KLL Tuner Support - End   */
#else

/* These are the valid Audio VSD errors */

#define VSDERR_SUCCESS                      MCIERR_SUCCESS
#define VSDERR_REQUEST_BUF_TOO_SMALL        MCIERR_INVALID_BUFFER
#define VSDERR_UNSUPPORTED_COMMAND          MCIERR_UNSUPPORTED_FUNCTION
#define VSDERR_UNSUPPORTED_FUNCTION         MCIERR_UNSUPPORTED_FUNCTION
#define VSDERR_UNRECOGNIZED_COMMAND         MCIERR_UNRECOGNIZED_COMMAND
#define VSDERR_INVALID_HVSD                 MCIERR_INVALID_DEVICE_ID
#define VSDERR_FILE_NOT_FOUND               MCIERR_FILE_NOT_FOUND
#define VSDERR_OUT_OF_MEMORY                MCIERR_OUT_OF_MEMORY
#define VSDERR_NO_DEVICE_DRIVER             MCIERR_NO_DEVICE_DRIVER
#define VSDERR_HARDWARE                     MCIERR_HARDWARE
#define VSDERR_UNSUPPORTED_CONN_TYPE        MCIERR_UNSUPPORTED_CONN_TYPE
#define VSDERR_INVALID_CONNECTOR_TYPE       MCIERR_INVALID_CONNECTOR_TYPE
#define VSDERR_INVALID_CONNECTOR_INDEX      MCIERR_INVALID_CONNECTOR_INDEX
#define VSDERR_CANNOT_MODIFY_CONNECTOR      MCIERR_CANNOT_MODIFY_CONNECTOR
#define VSDERR_INVALID_CONNECTOR            MCIERR_INVALID_CONNECTOR
#define VSDERR_MISSING_PARAMETER            MCIERR_MISSING_PARAMETER
#define VSDERR_INSTANCE_INACTIVE            MCIERR_INSTANCE_INACTIVE
#define VSDERR_DRIVER_PROC_NOT_FOUND        MCIERR_DRIVER_PROC_NOT_FOUND
#define VSDERR_INTERNAL                     MCIERR_INTERNAL
#define VSDERR_UNSUPP_FORMAT_TAG            MCIERR_UNSUPP_FORMAT_TAG
#define VSDERR_UNSUPP_BITSPERSAMPLE         MCIERR_UNSUPP_BITSPERSAMPLE
#define VSDERR_UNSUPP_CHANNELS              MCIERR_UNSUPP_CHANNELS
#define VSDERR_UNSUPP_SAMPLESPERSEC         MCIERR_UNSUPP_SAMPLESPERSEC
#define VSDERR_UNSUPP_FORMAT_MODE           MCIERR_UNSUPP_FORMAT_MODE
#define VSDERR_RESOURCE_NOT_AVAILABLE       MCIERR_RESOURCE_NOT_AVAILABLE
#define VSDERR_CANNOT_LOAD_DSP_MOD          MCIERR_CANNOT_LOAD_DSP_MOD
#define VSDERR_UNSUPP_CLASS                 MCIERR_UNSUPP_CLASS

#define VSDERR_UNSUPPORTED_FLAG             MCIERR_UNSUPPORTED_FLAG
#define VSDERR_BASE                         USERERR_BASE

typedef MCIDRV_RESTORE_PARMS VSD_RESTORE_PARMS;
typedef VSD_RESTORE_PARMS *PVSD_RESTORE_PARMS;
#endif



/* Device Capabilities flag */
#define DC_TRUE                 1  /* Supports/Has this function             */
#define DC_FALSE                0  /* Does not Support/Have this function    */


/* Device Capabilities Index to flag */
#define DC_HASAUDIO              0
#define DC_HASVIDEO              1
#define DC_HASVOLUME             2
#define DC_HASDISPLAY            3
#define DC_HASAUDIOATTRIBUTES    4
#define DC_HASAUDIOCAPABILITIES  5
#define DC_HASVIDEOATTRIBUTES    6
#define DC_HASIMAGEATTRIBUTES    7
#define DC_HASTTSATTRIBUTES      8
#define DC_HASCDCAPS             9
#define DC_HASESCAPE            10
#define DC_CANPLAY              11
#define DC_CANRECORD            12
#define DC_HASINPUTLEVEL        13
#define DC_HASSTATUSLEVEL       14
#define DC_HASMONITOR           15
#define DC_CANREAD              16
#define DC_CANWRITE             17
#define DC_HASDATATYPES         18
#define DC_CANEJECT             19
#define DC_CANLOAD              20
#define DC_HASDOOR              21
#define DC_HASTRACKS            22
#define DC_HASMEDIA             23
#define DC_HASCOMMSETTINGS      24
#define DC_HASMEDIATYPE         25
#define DC_HASKEYLOCK           26
#define DC_HASVARIABLERATES     27
#define DC_CANCUE               28
#define DC_HASCOUNTER           29
#define DC_CANRESET             30
#define DC_HASPOSITION          31
#define DC_HASLENGTH            32
#define DC_CANPARK              33
#define DC_HASCONNECTOR         34
#define DC_HASEVENTS            35
#define DC_HASDIRECTION         36
#define DC_HASVIEWPORT          37
#define DC_CANCAPTUREIMAGE      38
#define DC_CANRESTOREIMAGE      39
#define DC_HASRAM               40
#define DC_AUDIOSETTINGS        41
#define DC_IMAGESETTINGS        42
#define DC_QUERYMINSEEKTIME     43
#define DC_QUERYTIME            44
#define DC_SEEK                 45
#define DC_STEP                 46
#define DC_CAN_MIX              47
#define DC_HASCLOCK             48
#define DC_HASPOWER             49
#define DC_HASZOOM              50
#define DC_HASEDIT              51
#define DC_HASFOCUS             52
#define DC_HASCHANNELS          53
#define DC_HASTEXTTOSPEECH      54
#define DC_CANSTREAM            55
#define DC_CANSCALE             56
#define DC_CANDISTORT           57
#define DC_CANSTRETCH           58
#define DC_HASTUNER             59
#define DC_HASTELETEX           60
#define DC_HASAFC               61
#define DC_HASPOLARIZATION      62
#define DC_HASINLAY             63

/************************* W A R N I N G  ******************************/
/** Insert new Items Above *** and  *** Change Last Used below        **/
/* *********************** W A R N I N G  ******************************/
#define DC_MAX_DEVCAP           64 /* Maximum Number of entries supported */


typedef PVOID HVSD;

/* VSD entry point                                                                       */
typedef ULONG (APIENTRY FNVSDENTRY)(HVSD hVsd, ULONG ulFunc, ULONG ulFlags, PVOID pParms);
typedef FNVSDENTRY * PFNVSDENTRY;

/* VSD_OPEN request parameter block */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulCategory;                      /* Input - Name of unit to open */
      CHAR      szInstallName[MAX_DEVICE_NAME];  /* Install Name of MCD */
      CHAR      szPDDName[MAX_DEVICE_NAME];      /* Name of PDD to Open */
      HVSD      hvsd;                            /* Output - handle to the VSD driver */
      PFN       pfEvent;                         /* function ptr to VSD event proc */
      PVOID     pDevInfo;                        /* Device specific information */
      ULONG     ulDDStream;                      /* Device Driver Stream Identifier */
   } VSD_OPEN_PARMS;

typedef VSD_OPEN_PARMS   * PVSD_OPEN_PARMS;

#define VSD_OPEN_Len      sizeof(VSD_OPEN_PARMS)

/* Define for Image Rectangle */
typedef struct
  {
  USHORT X_left;
  USHORT Y_top;
  USHORT X_width;
  USHORT Y_height;
  } IRECT;


/* VSD_GETDEVCAPS request parameter block */
   typedef struct {
      ULONG     ulLength;                 /* length of the structure                */
      ULONG     ulMax_Caps;               /* returns Maximun Capabilities Supported */
      ULONG     ulNum_Caps;               /* returns Number of Capabilities returned*/
      ULONG     ulResv01;                 /* Unused Reserved                        */
      ULONG     ulResv02;                 /* Unused Reserved                        */
      ULONG     ulResv03;                 /* Unused Reserved                        */
      ULONG     ulResv04;                 /* Unused Reserved                        */
      BOOL      bSupports[DC_MAX_DEVCAP]; /* Capability Entries                     */
   } VSD_GETDEVCAPS_PARMS;

typedef VSD_GETDEVCAPS_PARMS    *PVSD_GETDEVCAPS_PARMS;

/* sub-flags for devcaps */
#define VSD_MIXCAPS         1
#define VSD_MIXLINECAPS     2
#define VSD_MODECAPS        3


#define VSD_GETDEVCAPS_Len   sizeof(VSD_GETDEVCAPS_PARMS)


/* VSD_CAPTUREIMAGE and VSD_RESTOREIMAGE request parameter block */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure               */
      ULONG     ulImg_Buff_Size;                 /* Size of the Image Buffer              */
      PVOID     pImg_Buff;                       /* Pointer to Image buffer               */
      IRECT     irectSource;                     /* Portion of Source rectangle to use    */
      IRECT     irectDest;                       /* Size of Destination rectangle         */
   } VSD_CAPIMAGE_PARMS;
#define VSD_CAPIMAGE_Len   sizeof(VSD_CAPIMAGE_PARMS)


/* VSD_LOAD device with Microcode/data parameter block */
   typedef struct {
      ULONG     ulLength;                     /* length of the structure    */
      ULONG     ulBuff_Size;                  /* Size of the data Buffer    */
      PVOID     pBuff;                        /* Pointer to data buffer     */
      ULONG     ulLoadID;                     /* Load Identifier (returned) */
   } VSD_LOAD_PARMS;
#define VSD_LOAD_Len   sizeof(VSD_LOAD_PARMS)


/* VSD_QUERY_LOAD Ask Device if MicroLoad is needed */
   typedef struct {
      ULONG     ulLength;                     /* length of the structure    */
      CHAR      szLoadName[256];               // Name of Load File
   } VSD_QUERY_LOAD_PARMS;
#define VSD_QUERY_LOAD_Len   sizeof(VSD_QUERY_LOAD_PARMS)


/* VSD_UNLOAD device with Microcode/data parameter block */
   typedef struct {
      ULONG     ulLength;                     /* length of the structure    */
      ULONG     ulHandle;                     /* Load Identifier to unload  */
   } VSD_UNLOAD_PARMS;
#define VSD_UNLOAD_Len   sizeof(VSD_UNLOAD_PARMS)


/* Common ulFlags for VSDEntry (sub-functions/command)                                    */
#define VSD_NONE                      0          /* no subfunction requested/supported    */

/* VSD_FREEZE ulFlags for VSDEntry (sub-functions/commands)                               */
/*  the pRequest parameter is not uses on this call                                       */
/*  defines to FREEZE or UNFREEZE the image captured by the capture device                */
#define VSD_FREEZE_DISABLE            0         /* UnFreeze the image */
#define VSD_FREEZE_ENABLE             1         /* Freeze the image */

/* VSD_SET ulFlags for VSDEntry (sub-functions/commands)                                  */
#define VSD_SETVIDEO                            1
#define VSD_SETVOLUME                           2
#define VSD_SETDISPLAY                          3
#define VSD_SETVIDEOATTRIBUTES                  4
#define VSD_SETIMAGEATTRIBUTES                  5
#define VSD_SETTSATTRRIBUTES                    6
#define VSD_SETINPUTLEVEL                       7
#define VSD_SETMONITOR                          8
#define VSD_SETDATATYPE                         9
#define VSD_SETDOOR                            10
#define VSD_SETCOMMSETTINGS                    11
#define VSD_SETRATE                            12
#define VSD_SETKEYLOCK                         13
#define VSD_SETCUE                             14
#define VSD_SETCOUNTER                         15
#define VSD_SETPOSITION                        16
#define VSD_SETCONNECTOR                       17
#define VSD_SETDIRECTION                       18
#define VSD_SETVIEWPORTPOSITION                19
#define VSD_SETVIEWPORT                        20
#define VSD_SETBANK                            21
#define VSD_SETVIDEORECT                       22
#define VSD_SETIMAGEFORMAT                     23
#define VSD_SETFPS                             24
#define VSD_SETTRANSPARENTCOLOR                25
#define VSD_SETCOLORKEYING                     26
#define VSD_SETAUDIOATTRIBUTES                 27
#define VSD_SETMIXSTATE                        28
#define VSD_SETMIXCONNECTIONS                  29
#define VSD_SETMIXCONTROL                      30
                                                /* KLL Tuner Support - Start */
#define VSD_SETTUNERCHANNEL                    31
                                                /* KLL Tuner Support - End   */
#define VSD_SETCLIPLIST                        32
#define VSD_SETCONNECTION                      33
/* VSD_SETVIDEOATTRIBUTES and VSD_QUERYVIDEOATTRIBUTES Request parameter block */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulBrightness;                    /* Brightness */
      ULONG     ulHue;                           /* Hue */
      ULONG     ulSaturation;                    /* Staturation */
      ULONG     ulContrast;                      /* Contrast */
      ULONG     ulSharpness;                     /* Sharpness */
   } VSD_VIDEOATTRIBUTES_PARMS;
#define VSD_VIDEOATTRIBUTES_Len   sizeof(VSD_VIDEOATTRIBUTES_PARMS)

/* Set Brigthness, Hue, Saturation, ... on VSD_SETVIDEOATTRIBUTES */
#define SETVIDEO_NoChange      0xFFFFFFFF
#define SETVIDEO_ToDefault     0xFFFFFFFE
#define QUERYVIDEO_unsupported 0xFFFFFFFF

#ifndef INCL_AUDIO_VSD
/* VSD_SETVIDEORECT and VSD_QUERYVIDEORECT Request parameter block    */
/* Specifies the Source and Destination rectangles                    */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      IRECT     irectSource;                     /* Portion of Source rectangle to use */
      IRECT     irectDest;                       /* Size of Destination rectangle */
   } VSD_VIDEORECT_PARMS;
#define VSD_VIDEORECT_Len   sizeof(VSD_VIDEORECT_PARMS)

/* VSD_SETCLIPLIST Request parameter block                            */
/* Specifies the Visible regions of the Destination Rectangle.        */
/* This Command is only issued by the MCD if DC_HASINLAY is true.     */
/* Note: The number of valid regions will be set to zero when windows */
/* are moving. At this point the driver should suspend all blits to   */
/* its window until this command is reissued with at list one visible */
/* region.                                                            */
/* When windows change the command sequence will be a follows:        */
/*  1) One (or more) VSD_SETCLIPLIST will ulNum_Rects = 0              */
/*  2) VSD_SETVIDEORECT with the new Source/Destination Rectangles.   */
/*     This command may not be issued if the devices window is        */
/*     unchanged.                                                     */
/*  3) VSD_SETCLIPLIST with 1 or more valid regions                   */

   typedef struct {
      ULONG     ulLength;      /* length of the structure             */
      ULONG     ulFlags;       /* Reserved and set to Zero            */
      ULONG     ulNum_Rects;   /* Number of rectanlges in array below */
      IRECT     VisRect[1];    /* Array of n Visible Retangle         */
   } VSD_SETCLIPLIST_PARMS;
#define VSD_SETCLIPLIST_Len   sizeof(VSD_SETCLIPLIST_PARMS)
#endif

/* VSD_SETVIEWPORT and VSD_QUERYVIEWPORT Request parameter block                  */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      BOOL      bViewPort;                       /* TRUE = View Port Enabled */
   } VSD_VIEWPORT_PARMS;
#define VSD_VIEWPORT_Len   sizeof(VSD_VIEWPORT_PARMS)

/* VSD_SETTRANSPARENTCOLOR and VSD_QUERYTRANSPARNETCOLOR Request parameter block                  */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulTransparentColor;              /* Transparent Color */
   } VSD_TRANSPARENTCOLOR_PARMS;
#define VSD_TRANSPARENTCOLOR_Len   sizeof(VSD_TRANSPARENTCOLOR_PARMS)

/* VSD_SETCOLORKEYING and VSD_QUERYCOLORKEYING Request parameter block                  */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      BOOL      bColorKeying;                    /* TRUE = Transparent Color Enable */
   } VSD_COLORKEYING_PARMS;
#define VSD_COLORKEYING_Len   sizeof(VSD_COLORKEYING_PARMS)

/*****************************************************************************************************/
   typedef struct {
      ULONG          ulType;                     /* Data Type */
      ULONG          ulSubType;                  /* Streaming Specific Info */
      ULONG          ulReqBufSize;               /* Required/Recommened buffer size */
      ULONG          ulQueueSize;                /* Number of Queued Elements */
      ULONG          ulOperation;                /* (play, record, etc.) */
      ULONG          ulSettings;                 /* Device specific structure */
      ULONG          ulFlags;                    /* Device specific flags */
   } VSD_DATATYPE;

/* VSD_SETDATATYPE and VSD_QUERY_DATATYPE Request Parameter Block */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulNumDataTypes;             /* Number of Data Types */
      VSD_DATATYPE   vsdDataType[1];             /* Specific info for each Data Type */
   } VSD_DATATYPES_PARMS;


/* Supported DataTypes - used by VSD_SETDATATYPES, VSD_QUERYDATATYPES and      */
/*                               VSD_RESOURCE                                  */
#define VSD_DATATYPE_ADPCMAVC        1 /* AVC type ADPCM                      */
#define VSD_DATATYPE_PCM             2 /* PCM                                 */
#define VSD_DATATYPE_MU_LAW          3 /* Mu-Law                              */
#define VSD_DATATYPE_MIDI            4 /* MIDI                                */
#define VSD_DATATYPE_ALAW            5 /* A-Law                               */
#define VSD_DATATYPE_SOURCEMIX       6 /* External analog audio source        */
#define VSD_DATATYPE_SPV2            7 /* Speech Viewer/2                     */
#define VSD_DATATYPE_ADPCMXA         8 /* XA CD ROM                           */
#define VSD_DATATYPE_CVSD           11 /* CVSD                                */
#define VSD_DATATYPE_TEXT_TO_SPEECH 12 /* Text to speech                      */
#define VSD_DATATYPE_CDDA           13 /* Green Book format                   */
#define VSD_DATATYPE_CDXA           14 /* The other XA mode is an audio mode  */
#define VSD_DATATYPE_MPEG1          15 /* MPEG1 */
#define VSD_DATATYPE_MPEG2          16 /* MPEG2 */

/* VSD_IMAGEFORMAT use by VSD_SETIMAGEFORMAT and VSD_QUERYIMAGEFORMAT */
   typedef struct {
      ULONG          ulFormat;                   /* Image Format */
      USHORT         usBitsPerPEL;               /* Bits Per PEL */
   } VSD_IMAGEFORMAT;

/* defines for supported Formats - used by VSD_SETIMAGEFORMAT and VSD_QUERYIMAGEFORMAT. */
#define VSD_IMAGEFORMAT_RGB_565        1 /* bits 0-4 =Red, 5-10 =Green, 11-15 Blue   */
#define VSD_IMAGEFORMAT_YUV_411        2 /* 4 bytes of Y, 1 Byte of U, 1 Byte of V   */
#define VSD_IMAGEFORMAT_OS2_BITMAP2    3 /* OS/2 Bitmap                              */
#define VSD_IMAGEFORMAT_YUV_422        '224Y'/* 4 bytes of Y, 2 Byte of U, 2 Byte of V   */
                                             /* all 8 bit values in this form YUYVYUYV   */

/* defines for supported Bits Per PEL - used by VSD_SETIMAGEFORMAT and VSD_QUERYIMAGEFORMAT. */
#define VSD_BITSPERPEL_8              8  /*  8 Bits Per PEL                   */
#define VSD_BITSPERPEL_16            16  /* 16 Bits Per PEL                   */
#define VSD_BITSPERPEL_24            24  /* 24 Bits Per PEL                   */


/* VSD_SETIMAGEFORMAT Request Parameter Block */
   typedef struct {
      ULONG           ulLength;                   /* length of the structure */
      VSD_IMAGEFORMAT SetFormat;                  /* Image Format to Set */
   } VSD_SETIMAGEFORMAT_PARMS;
#define VSD_SETIMAGEFORMAT_Len   sizeof(VSD_SETIMAGEFORMAT_PARMS)

/* VSD_QUERYIMAGEFORMAT Request Parameter Block */
   typedef struct {
      ULONG           ulLength;                 /* length of the structure */
      VSD_IMAGEFORMAT CurFormat;                /* Current Image Format */
      VSD_IMAGEFORMAT NativeFormat;             /* Native  Image Format */
      ULONG           ulNumFormats;             /* Number of image Formats supported */
      VSD_IMAGEFORMAT Format[1];                /* List of Image Formats supported */
   } VSD_QUERYIMAGEFORMAT_PARMS;
#define VSD_QUERYIMAGEFORMAT_Len   sizeof(VSD_QUERYIMAGEFORMAT_PARMS)


/* defines for ulFlags - used by VSD_SETRIMAGEFORMAT and VSD_QUERYRIMAGEFORMAT. */
#define VSD_NATIVE_FORMAT      1  /*  Native Format Supported by the Device  */

/* VSD_RIMAGEFORMAT use by VSD_SETRIMAGEFORMAT and VSD_QUERYRIMAGEFORMAT */
   typedef struct {
      ULONG        FourCC;                     /* Image Format */
      USHORT       usFlags;                    /* Flags 1 = Native */
   } VSD_RIMAGEFORMAT;

/* VSD_QUERYRESTOREIMAGEFORMAT Request Parameter Block */
   typedef struct {
      ULONG            ulLength;          /* length of the structure */
      ULONG            CurFourCC;         /* Current Image Format */
      ULONG            ulNumFormats;      /* Number of image Formats supported */
      VSD_RIMAGEFORMAT Format[1];         /* List of Image Formats supported */
   } VSD_QUERYRESTOREIMAGEFORMAT_PARMS;
#define VSD_QUERYRESTOREIMAGEFORMAT_Len sizeof(VSD_QUERYRESTOREIMAGEFORMAT_PARMS)

/* VSD_SETFPS and VSD_QUERYFPS Request Parameter Block  (set Frames Per Second) */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulFPS;                      /* Set Current Frames Per Second to */
      ULONG          ulFlags;                    /* Flags */
   } VSD_SETFPS_PARMS;
#define VSD_SETFPS_Len   sizeof(VSD_SETFPS_PARMS)


/* defines for VSD_QUERYFPS and VSD_SETFPS Flags            */
#define VSD_FPS_Frames           0
#define VSD_FPS_MicroSecond      1


/* VSD_QUERYIMAGEEXTENTS maximun image size that Capture Image can return */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulX_Extent;                 /* Maximum X on Image Rect */
      ULONG          ulY_Extent;                 /* Maximum Y on Image Rect */
   } VSD_QUERYIMAGEEXTENTS_PARMS;
#define VSD_QUERYIMAGEEXTENTS_Len   sizeof(VSD_QUERYIMAGEEXTENTS_PARMS)

/* VSD_QUERYOVERLAYEXTENTS maximun Overlay size that Capture Image can support */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulX_Extent;                 /* Maximum X on Overlay Rect */
      ULONG          ulY_Extent;                 /* Maximum Y on Overlay Rect */
   } VSD_QUERYOVERLAYEXTENTS_PARMS;
#define VSD_QUERYOVERLAYEXTENTS_Len   sizeof(VSD_QUERYOVERLAYEXTENTS_PARMS)

/* VSD_QUERYVIDEOEXTENTS  for Overlay Video Devices Overlay Video Window */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulX_Extent;                 /* Maximum X on Video Rect */
      ULONG          ulY_Extent;                 /* Maximum Y on Video Rect */
   } VSD_QUERYVIDEOEXTENTS_PARMS;
#define VSD_QUERYVIDEOEXTENTS_Len   sizeof(VSD_QUERYVIDEOEXTENTS_PARMS)


/* VSD_DEVUNITS Request Parameter Block */
   typedef struct {
      ULONG          ulDevType;                  /* Unit Device Type (e.g.video, cd, amp_mix etc.) */
      CHAR           szUNIT[10];                 /* Unit Name (abstract or device name COM1:, F:) */
   } VSD_UNIT;

   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      ULONG          ulUnits;                    /* Number of Units that can be opened */
      VSD_UNIT       VSD_Unit[1];                /* Unit Info */
   } VSD_DEVUNITS_PARMS;

typedef VSD_DEVUNITS_PARMS  *PVSD_DEVUNITS_PARMS;

#define VSD_DEVUNITS_Len   sizeof(VSD_DEVUNITS_PARMS)

/* defines for DevType (device Types)                 */
#define VSD_DEVTYPE_VIDEOTAPE           1
#define VSD_DEVTYPE_VIDEODISK           2
#define VSD_DEVTYPE_CD_AUDIO            3
#define VSD_DEVTYPE_DAT                 4
#define VSD_DEVTYPE_AUDIO_TAPE          5
#define VSD_DEVTYPE_OTHER               6
#define VSD_DEVTYPE_WAVEFORM_AUDIO      7
#define VSD_DEVTYPE_SEQUENCER           8
#define VSD_DEVTYPE_AUDIO_AMPMIX        9
#define VSD_DEVTYPE_OVERLAY            10
#define VSD_DEVTYPE_ANIMATION          11
#define VSD_DEVTYPE_DIGITAL_VIDEO      12
#define VSD_DEVTYPE_SPEAKER            13
#define VSD_DEVTYPE_MONITOR            14
#define VSD_DEVTYPE_CDXA               15
#define VSD_DEVTYPE_TEXT_TO_SPEECH     16
#define VSD_DEVTYPE_USER_BASE          17
#define VSD_DEVTYPE_USER_MAX           18


/* VSD_RESOURCE Request Parameter Block */
   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      CHAR           szPDDName[MAX_DEVICE_NAME]; /* Name of PDD */
      ULONG          ulDevType;                  /* Device Type */
      ULONG          ulDataType;                 /* Supported Data Type */
      ULONG          ulResUnits;                 /* # of Resource Units this would consume */
      ULONG          ulClass;                    /* Resource Class */
   } VSD_RESOURCE_PARMS;

typedef VSD_RESOURCE_PARMS *PVSD_RESOURCE_PARMS;

#define VSD_RESOURCE_Len   sizeof(VSD_RESOURCE_PARMS)

/* VSD_GETPRODINFO Request Parameter Block */
   typedef struct {
      CHAR           szProdInfo[30];             /* Product Info */
      CHAR           szManufacture[30];          /* Manufacturer */
      CHAR           szVersion[10];              /* Version */
   } VSD_PRODINFO;

   typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      VSD_PRODINFO   VSD;                        /* Product Info for VSD */
      VSD_PRODINFO   PDD;                        /* Product Info for PDD */
   } VSD_GETPRODINFO_PARMS;

typedef VSD_GETPRODINFO_PARMS  *PVSD_GETPRODINFO_PARMS;

#define VSD_GETPRODINFO_Len   sizeof(VSD_GETPRODINFO_PARMS)

/* VSD_CONNECTION_PARMS parameter block use by VSD_SETCONNECTION  and       */
/* by VSD_QUERYCONNECTION                                                   */
/* Connect/disconnect a video output connector (ulIndex2) to/from a video   */
/* Input connector (ulIndex1).                                              */
/* Query if a output connector is connectored to a specific Input Connector.*/
/*  On a SET all fields are input.                                          */
/*  On a QUERY all fields are input except fEnabled2 which returns if a     */
/*  output connector is enabled with a specific input connector.            */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulConn_Type1;                    /* Type of Connector */
      ULONG     ulIndex1;                        /* Connector Number */
      ULONG     ulConn_Type2;                    /* Type of Connector */
      ULONG     ulIndex2;                        /* Connector Number */
      BOOL      fEnabled2;                       /* TRUE = Connector Enable */
   } VSD_CONNECTION_PARMS;

typedef VSD_CONNECTION_PARMS *PVSD_CONNECTION_PARMS;
#define VSD_CONNECTION_Len      sizeof(VSD_CONNECTION_PARMS)

/* VSD_CONNECTOR_PARMS parameter block use by VSD_QUERYCONNECTOR and    */
/* VSD_SETCONNECTOR.                                                    */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulConn_Type;                     /* Type of Connector */
      ULONG     ulIndex;                         /* Connector Number */
      BOOL      fEnabled;                        /* TRUE = Connector Enable */
   } VSD_CONNECTOR_PARMS;

typedef VSD_CONNECTOR_PARMS *PVSD_CONNECTOR_PARMS;
#define VSD_CONNECTOR_Len      sizeof(VSD_CONNECTOR_PARMS)

/* part of the VSD_CONNECTORTYPES_PARMS parameter block  */
   typedef struct {
      ULONG     ulConn_Type;                     /* Type of Connector */
      ULONG     ulConn_Number;                   /* Number of this Connector */
   } CONNUM_TYPE;

/* VSD_CONNECTORTYPES_PARMS parameter block use by VSD_QUERYCONNECTORTYPES   */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulNumConnectors;                 /* Number of Connector */
      CONNUM_TYPE Conn_Info[1];                  /* Array of Connector Info */
   } VSD_CONNECTORTYPES_PARMS;
#define VSD_CONNECTORTYPES_Len      sizeof(VSD_CONNECTORTYPES_PARMS)


typedef VSD_CONNECTORTYPES_PARMS   *LPVSD_CONNECTORTYPES_PARMS;
typedef VSD_CONNECTORTYPES_PARMS   *PVSD_CONNECTORTYPES_PARMS;

/* VSD_USER request parameter block */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulLenUserInfo;                   /* Length of User's Info */
      PVOID     pUserInfo;                       /* Pointer to User's Info */
   } VSD_USER_PARMS;
#define VSD_USER_Len      sizeof(VSD_USER_PARMS)


/* VSD_USER ulFlags for VSDEntry (sub-functions/commands)                                  */
/* The defintion of each of these fields are up to the VSD */
/* to define each VSD may have a different uses for each   */
#define VSD_USER1                               1
#define VSD_USER2                               2
#define VSD_USER3                               3
#define VSD_USER4                               4
#define VSD_USER5                               5
#define VSD_USER6                               6
#define VSD_USER7                               7
#define VSD_USER8                               8
#define VSD_USER9                               9
#define VSD_USER10                             10


/* VSD_QUERY ulFlags for VSDEntry (sub-functions/commands)  */

#define VSD_QUERYAPERTURESIZE                   1
#define VSD_QUERYAUDIOATTRIBUTES                2
#define VSD_QUERYAUDIOCAPABILITIES              3
#define VSD_QUERYBANK                           4
#define VSD_QUERYBANKPOINTER                    5
#define VSD_QUERYBANKS                          6
#define VSD_QUERYCDCAPS                         7
#define VSD_QUERYCOMMSETTINGS                   8
#define VSD_QUERYCOMPATIBLECONNECTORS           9
#define VSD_QUERYCONNECTOR                     10
#define VSD_QUERYCONNECTORTYPES                11
#define VSD_QUERYCOUNTER                       12
#define VSD_QUERYCUE                           13
#define VSD_QUERYDATATYPE                      14
#define VSD_QUERYDIRECTION                     15
#define VSD_QUERYDISPLAY                       16
#define VSD_QUERYDOOR                          17
#define VSD_QUERYEVENTLIST                     18
#define VSD_QUERYFREEZE                        19
#define VSD_QUERYIMAGEATTRIBUTES               20
#define VSD_QUERYINPUTLEVEL                    21
#define VSD_QUERYINPUTLEVELS                   22
#define VSD_QUERYKEYLOCK                       23
#define VSD_QUERYLABEL                         24
#define VSD_QUERYLENGTH                        25
#define VSD_QUERYMEDIATYPE                     26
#define VSD_QUERYMONITOR                       27
#define VSD_QUERYPOSITION                      28
#define VSD_QUERYPREROLLTYPE                   29
#define VSD_QUERYPROCESSINTERNAL               30
#define VSD_QUERYRATE                          31
#define VSD_QUERYRATELEVELS                    32
#define VSD_QUERYSTATUSLEVEL                   33
#define VSD_QUERYSUPPORTEDEVENTS               34
#define VSD_QUERYTIME                          35
#define VSD_QUERYTOC                           36
#define VSD_QUERYTRACKS                        37
#define VSD_QUERYTTSATTRIBUTES                 38
#define VSD_QUERYUPC                           39
#define VSD_QUERYVIDEO                         40
#define VSD_QUERYVIDEOATTRIBUTES               41
#define VSD_QUERYVIEWPORT                      42
#define VSD_QUERYVIEWPORTPOSITION              43
#define VSD_QUERYVOLUME                        44
#define VSD_QUERYVIDEORECT                     45
#define VSD_QUERYFPS                           46
#define VSD_QUERYIMAGEFORMAT                   47
#define VSD_QUERYIMAGEEXTENTS                  48
#define VSD_QUERYVIDEOEXTENTS                  49
#define VSD_QUERYOVERLAYEXTENTS                50
#define VSD_QUERYTRANSPARENTCOLOR              51
#define VSD_QUERYCOLORKEYING                   52
#define VSD_QUERYREADY                         53
#define VSD_QUERYMIXSTATE                      54
#define VSD_QUERYMIXCONNECTIONS                55
#define VSD_QUERYMIXCONTROL                    56
#define VSD_QUERYMIXLINE                       57

#define VSD_QUERYTUNERCHANNEL                  58
#define VSD_QUERYCONNECTORSIGNAL               59
#define VSD_QUERYLOAD                          60
#define VSD_QUERYCONNECTION                    61

/* VSD_TUNERCHANNEL Request parameter block  for Set and Query */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulOption;                        /* Options */
      USHORT    usResv01;                        /* Reserved */
      USHORT    usResv02;                        /* Reseverd */
      LONG      lFineTune;                       /* FineTune */
      ULONG     ulFrequency;                     /* Frequency */
   } VSD_TUNERCHANNEL_PARMS;

#define VSD_TUNERCHANNEL_Len   sizeof(VSD_TUNERCHANNEL_PARMS)


/* VSD_QUERYCONNECTORSIGNAL_PARMS parameter block   */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulConn_Type;                     /* Type of Connector */
      ULONG     ulIndex;                         /* Connector Number */
      BOOL      fEnabled;                        /* TRUE = Connector Enable */
      ULONG     ulEnabled;                       /*  Active Signal present */
   } VSD_QUERYCONNECTORSIGNAL_PARMS;
#define VSD_QUERYCONNECTORSIGNAL_Len      sizeof(VSD_QUERYCONNECTORSIGNAL_PARMS)

/* VSD_QUERYCONNECTORSIGNAL_PARMS  ulEnabled states */
#define SIGNAL_NOT_PRESENT   0
#define SIGNAL_PRESENT       1
#define SIGNAL_INDETERMINATE 2


/*      Tuner Types     */

#define M_TUNER         0
#define BG_TUNER        1
#define L_TUNER         2



/*  ulOption - Tuner Frequency Set/Query of TunerChannel/Frequency    */
/*             and QueryTunerChannel/frequency                        */
/*  Note: these field is bit sensitive...or bit in                    */
#define TUC_AFC_On             4
#define TUC_AFC_Off            0
#define TUC_Frequency          8
#define TUC_Polarization_Vert 16
#define TUC_Polarization_Hori 32


/*      SetTunerChannel and QueryTunerChannel ulOptions Values    */

#define TUNER_NOMUTE            0x01
#define TUNER_NOWAITTUNER       0x02
#define TUNER_NOWAITSIGNAL      0x04
#define TUNER_UP                0x80
#define TUNER_DOWN              0x40
                                                /* KLL Tuner Support - End   */
                                              /* KLL Playback Support- Start */
/* VSD_PLAY/VSD_RECORD request parameter block */
   typedef struct {
      ULONG     ulLength;                        /* length of the structure */
      ULONG     ulFlags;                         /* Flags */
      ULONG     ulSizeData;                      /* Size of Data below */
      PVOID     pData;                           /* Data for the Device */
      ULONG     ulSCR;
      ULONG     ulPTS;
      ULONG     ulAudioTime;
   } VSD_STREAM_PARMS;
#define VSD_STREAM_Len      sizeof(VSD_STREAM_PARMS)


/* VSD_PLAY and VSD_RECORD ulFlags for VSDEntry (sub-functions/commands)     */
#define VSD_PLAYSTART              0x01
#define VSD_PLAYDATA               0x02
#define VSD_PLAYSTOP               0x04
#define VSD_PLAYFLUSH              0x08
#define VSD_PLAYPAUSE              0x03
#define VSD_PLAYRESUME             0x05
#define VSD_RECORDSTART            0x10
#define VSD_RECORDDATA             0x20
#define VSD_RECORDSTOP             0x40

                                              /* KLL Playback Support- End   */


typedef ULONG        HCONN;

#define   VSD_VOLUME             0x00000001
#define   VSD_MASTERVOLUME       0x00000002
#define   VSD_MUTE               0x00000004


   typedef struct {
      ULONG          ulLength;        /* length of the structure */
      ULONG          ulFlags;         /* Which volume item to operate on */
      ULONG          ulRequest;       /* mastervol or vol */
      ULONG          ulVectoredVol;   /* Length to do effect */
      ULONG          ulMasterAudio;   /* masteraudio volume */
      ULONG          ulVolume;        /* Volume for connector */
      ULONG          hConn;           /* handle to connector to operate on */
      BOOL           fMute;           /* Is the connector muted? */
   } VSD_VOLUME_PARMS;

   typedef VSD_VOLUME_PARMS  *PVSD_VOLUME_PARMS;

  typedef struct {
     ULONG ulLength;        /* length of the structure */
     ULONG ulFlags;         /* mask of attributes supported/specified */
     ULONG ulValue;         /* value of the attribute specified in ulFlags */
     ULONG ulDelay;         /* delay before effect takes place. */
  } VSD_AUDIOATTRIBUTES_PARMS;

typedef  VSD_AUDIOATTRIBUTES_PARMS *PVSD_AUDIOATTRIBUTES_PARMS;

/* Audio attribute mask defines */

  #define VSD_BALANCE         0x00020000
  #define VSD_BASS            0x00040000
  #define VSD_GAIN            0x00060000
  #define VSD_TREBLE          0x00000004
  #define VSD_PITCH           0x00000008

/* VSD CONNECTOR defines */
  #define VSD_ENABLE          0x00000001
  #define VSD_DISABLE         0x00000000


/* Defines for audio datatypes */

#define  VSD_MODE             MCI_WAVE_SET_FORMATTAG
#define  VSD_CHANNELS         MCI_WAVE_SET_CHANNELS
#define  VSD_SAMPLESPERSEC    MCI_WAVE_SET_SAMPLESPERSEC
#define  VSD_AVGBYTESPERSEC   MCI_WAVE_SET_AVGBYTESPERSEC
#define  VSD_BITSPERSAMPLE    MCI_WAVE_SET_BITSPERSAMPLE
#define  VSD_OPERATION        0x20000000

/* Defines for vsd ulOperation field */

#define VSD_CONSUME           1
#define VSD_PRODUCE           2


typedef struct {
  ULONG ulLength;
  ULONG ulFlags;
  ULONG ulSamplingRate;      /* floating quantity for certain cards */
  ULONG ulBitsPerSample;     /* float for cards like SB which support 2.6 bitspersamp */
  ULONG ulChannels;          /* mask of channels supported in this sampling rate */
  ULONG ulBlockAlignment;
  ULONG ulAverageBytesPerSec;
  ULONG ulDataType;
  ULONG ulDataSubType;
  ULONG ulReserved1;
  ULONG ulOperation;         /* VSD_PRODUCE or VSD_CONSUME */
  ULONG ulDeviceID;          /* Device ID of the amp-mixer */
  PVOID pDevInfo;
} VSD_AUDIODATATYPE_PARMS;

typedef VSD_AUDIODATATYPE_PARMS *PVSD_AUDIODATATYPE_PARMS;

typedef struct {
  ULONG ulLength;            /* length of the structure */
  ULONG ulFlags;             /* ???? */
  ULONG ulSamplingRate;      /* Sampling rate */
  ULONG ulBitsPerSample;     /* bitspersample */
  ULONG ulChannels;          /* channels for this mode */
  ULONG ulBlockAlignment;    /* block alignment for data */
  ULONG ulAverageBytesPerSec;/* Average bytes/second of data */
  ULONG ulDataType;          /* RIFF datatype -- see os2medef.h */
  ULONG ulDataSubType;       /* RIFF subdatatype -- see os2medef.h */
  ULONG hidSource;           /* source stream handler for VSD */
  ULONG hidTarget;           /* target stream handler for VSD */
  ULONG ulOperation;         /* VSD_PRODUCE or VSD_CONSUME */
  ULONG ulReserved1;         /* reserved for future use */
  ULONG ulDeviceID;          /* Device ID of the amp-mixer */

  PVOID pHeader;             /* Header which describes data (option)--see MMIOOS2.H */
  PCHAR pParmString;         /* Pointer to device specific parm string in MMPM2.INI file */
  PVOID pDevInfo;            /* Device specific information */
  ULONG ulReserved2;         /* reserved for future use */
  PCHAR pResourceDLL;        /* Resource DLL from ini file */
  ULONG ulResourceId;        /* Resource ID to use in Resource DLL. */
  PVOID pVhnd;               /* Video VSD info needed by MPEG VSD   */


} VSD_AUDIOOPEN_PARMS;

typedef VSD_AUDIOOPEN_PARMS *PVSD_AUDIOOPEN_PARMS;

#define VSD_AUDIO_OPEN      0x10010001

typedef struct {
     ULONG ulLength;         /* length of the structure */
     PVOID   pBuffer;        /* Pointer to escape buffer */
     ULONG ulBufferLength;   /* Buffer length */
  } VSD_ESCAPE_PARMS;

 typedef struct {
      ULONG          ulLength;                   /* length of the structure */
      BOOL           fEnabled;                   /* TRUE = Enabled */
   } VSD_BOOLITEM_PARMS;


#endif                       /* _VSDCMDS_H_ */

#ifdef __cplusplus
}
#endif


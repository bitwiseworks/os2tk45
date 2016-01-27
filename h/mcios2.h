#ifdef __cplusplus
   extern "C" {
   #endif
   
   #ifndef MCIOS2
      #define MCIOS2
      
      /****************************************************************************/
      /*                                                                          */
      /* Module Name: MCIOS2.h                                                    */
      /*                                                                          */
      /* This is the multimedia include file that has the typedefs, defines and   */
      /* function prototyes for Media Control Interface of MMPM/2 (tm) using      */
      /* the OS2 notation.                                                        */
      /*                                                                          */
      /*         Copyright (c) International Business Machines Corporation        */
      /*                          1991, 1992, 1993                                */
      /*                        All Rights Reserved                               */
      /*                                                                          */
      /****************************************************************************/
      #define MCI_TRUE                        1
      #define MCI_FALSE                       0
      
      /*********************************************************************/
      /*                                                                   */
      /*             MCI command message identifiers                       */
      /*                                                                   */
      /*********************************************************************/
      #define MCI_OPEN                        1
      #define MCI_CLOSE                       2
      #define MCI_ESCAPE                      3
      #define MCI_PLAY                        4
      #define MCI_SEEK                        5
      #define MCI_STOP                        6
      #define MCI_PAUSE                       7
      #define MCI_INFO                        8
      #define MCI_GETDEVCAPS                  9
      #define MCI_STATUS                      10
      #define MCI_SPIN                        11
      #define MCI_SET                         12
      #define MCI_STEP                        13
      #define MCI_RECORD                      14
      #define MCI_SYSINFO                     15
      #define MCI_SAVE                        16
      #define MCI_CUE                         17
      #define MCI_UPDATE                      18
      #define MCI_SET_CUEPOINT                19
      #define MCI_SET_POSITION_ADVISE         20
      #define MCI_SET_SYNC_OFFSET             21
      #define MCI_LOAD                        22
      #define MCI_ACQUIREDEVICE               23
      #define MCI_RELEASEDEVICE               24
      #define MCI_MASTERAUDIO                 25
      #define MCI_GETTOC                      26
      #define MCI_DEVICESETTINGS              27
      #define MCI_CONNECTOR                   28
      #define MCI_RESUME                      29
      
      #define MCI_CONNECTORINFO               31
      #define MCI_DEFAULT_CONNECTION          32
      #define MCI_CONNECTION                  33
      #define MCI_GROUP                       34
      #define MCI_NETWORK_DEFAULT_CONNECTION  35
      
      /*************************************************/
      /* MCI command message identifiers reserved for  */
      /*  Digital Video and Video Overlay              */
      /*************************************************/
      
      #define MCI_CAPTURE                     40
      #define MCI_FREEZE                      41
      #define MCI_GETIMAGEBUFFER              42
      #define MCI_GETIMAGEPALETTE             43
      #define MCI_PUT                         44
      #define MCI_REALIZE                     45
      #define MCI_REWIND                      46
      #define MCI_RESTORE                     47
      #define MCI_SETIMAGEBUFFER              48
      #define MCI_SETIMAGEPALETTE             49
      #define MCI_UNFREEZE                    50
      #define MCI_WHERE                       51
      #define MCI_WINDOW                      52
      
      #define MCI_DELETE                      53
      #define MCI_CUT                         54
      #define MCI_PASTE                       55
      #define MCI_COPY                        56
      #define MCI_REDO                        57
      #define MCI_UNDO                        58
      #define MCI_MIXNOTIFY                   59
      #define MCI_SETTUNER                    60
      #define MCI_FILTER                      61
      #define MCI_BUFFER                      62
      #define MCI_MIXSETUP                    63
      
      #define MCI_MAX_COMMAND                 64
      
      #define RECT                            ULONG
      #define HDCT                            ULONG
      /*********************************************************************/
      /* this and all subsequent message ID's are reserved for the user    */
      /*********************************************************************/
      #define MCI_USER_MESSAGES               2000
      
      /*********************************************************************/
      /* Special Device ID for "all"                                       */
      /*********************************************************************/
      #define MCI_ALL_DEVICE_ID               0xFFFF
      
      /*********************************************************************/
      /* MCI implementation limits                                         */
      /*********************************************************************/
      #define MAX_DEVICE_NAME                 20
      #define MAX_ALIAS_NAME                  20
      #define MAX_PRODINFO                    40
      #define MAX_EXTENSION_NAME              4
      #define MAX_DEV_PARAMS                  128
      #define MAX_VERSION_NUMBER              6
      #define MAX_PDD_NAME                    9
      #define MAX_DLL_NAME                    260
      #define MAX_CLASSES                     10
      #define MAX_CONNECTORS                  10
      #define MAX_EXTENSIONS                  25
      #define MAX_TYPEBUFFER                  256
      
      /*********************************************************************/
      /* MCI Device Type Constants                                         */
      /*********************************************************************/
      #define MCI_DEVTYPE_VIDEOTAPE           1
      #define MCI_DEVTYPE_VIDEODISC           2
      #define MCI_DEVTYPE_CD_AUDIO            3
      #define MCI_DEVTYPE_DAT                 4
      #define MCI_DEVTYPE_AUDIO_TAPE          5
      #define MCI_DEVTYPE_OTHER               6
      #define MCI_DEVTYPE_WAVEFORM_AUDIO      7
      #define MCI_DEVTYPE_SEQUENCER           8
      #define MCI_DEVTYPE_AUDIO_AMPMIX        9
      #define MCI_DEVTYPE_OVERLAY             10
      #define MCI_DEVTYPE_ANIMATION           11
      #define MCI_DEVTYPE_DIGITAL_VIDEO       12
      #define MCI_DEVTYPE_SPEAKER             13
      #define MCI_DEVTYPE_HEADPHONE           14
      #define MCI_DEVTYPE_MICROPHONE          15
      #define MCI_DEVTYPE_MONITOR             16
      #define MCI_DEVTYPE_CDXA                17
      #define MCI_DEVTYPE_FILTER              18
      #define MCI_DEVTYPE_TTS                 19
      
      /*********************************************************************/
      /* MCI Device Type Names                                             */
      /*********************************************************************/
      #define MCI_DEVTYPE_VIDEOTAPE_NAME          "Videotape"
      #define MCI_DEVTYPE_VIDEODISC_NAME          "Videodisc"
      #define MCI_DEVTYPE_CD_AUDIO_NAME           "CDaudio"
      #define MCI_DEVTYPE_DAT_NAME                "DAT"
      #define MCI_DEVTYPE_AUDIO_TAPE_NAME         "Audiotape"
      #define MCI_DEVTYPE_OTHER_NAME              "Other"
      #define MCI_DEVTYPE_WAVEFORM_AUDIO_NAME     "Waveaudio"
      #define MCI_DEVTYPE_SEQUENCER_NAME          "Sequencer"
      #define MCI_DEVTYPE_AUDIO_AMPMIX_NAME       "Ampmix"
      #define MCI_DEVTYPE_OVERLAY_NAME            "Overlay"
      #define MCI_DEVTYPE_ANIMATION_NAME          "Animation"
      #define MCI_DEVTYPE_DIGITAL_VIDEO_NAME      "Digitalvideo"
      #define MCI_DEVTYPE_SPEAKER_NAME            "Speaker"
      #define MCI_DEVTYPE_HEADPHONE_NAME          "Headphone"
      #define MCI_DEVTYPE_MICROPHONE_NAME         "Microphone"
      #define MCI_DEVTYPE_MONITOR_NAME            "Monitor"
      #define MCI_DEVTYPE_CDXA_NAME               "CDXA"
      #define MCI_DEVTYPE_FILTER_NAME             "Filter"
      #define MCI_DEVTYPE_TTS_NAME                "Texttospeech"
      #define MCI_MAX_SYSTEM_DEVICE_NAMES         19
      
      
      /*********************************************************************/
      /* Getdevcaps, set, and status item base values                      */
      /*********************************************************************/
      
      #define MCI_AMP_ITEM_BASE                   0x00001000L
      #define MCI_CD_ITEM_BASE                    0x00002000L
      #define MCI_CDXA_ITEM_BASE                  0x00003000L
      #define MCI_VD_ITEM_BASE                    0x00004000L
      #define MCI_SEQ_ITEM_BASE                   0x00005000L
      #define MCI_WAVE_ITEM_BASE                  0x00006000L
      #define MCI_VID_ITEM_BASE                   0x00007000L
      #define MCI_DGV_ITEM_BASE                   0x00008000L
      #define MCI_OVLY_ITEM_BASE                  0x00009000L
      
      /*********************************************************************/
      /* Flags for mciDriverNotify                                         */
      /*********************************************************************/
      #define MCI_NOTIFY_SUCCESSFUL               0x0000
      #define MCI_NOTIFY_SUPERSEDED               0x0001
      #define MCI_NOTIFY_ABORTED                  0x0002
      #define MCI_NOTIFY_ERROR                    0x0003
      
      /*********************************************************************/
      /* mciDriverNotify  Message Types                                    */
      /*********************************************************************/
      #define MM_MCINOTIFY                        0x0500
      #define MM_MCIPASSDEVICE                    0x0501
      #define MM_MCIPOSITIONCHANGE                0x0502
      #define MM_MCICUEPOINT                      0x0503
      #define MM_MCIPLAYLISTMESSAGE               0x0504
      #define MM_MCIEVENT                         0x0505
      #define MM_MCISYNCH                         0x0506
      
      #define MCI_LOSING_USE                      0x00000001L
      #define MCI_GAINING_USE                     0x00000002L
      
      /*********************************************************************/
      /* Common message flags                                              */
      /*     0x000000XX are reserved for common flags                      */
      /*********************************************************************/
      #define MCI_NOTIFY                          0x00000001L
      #define MCI_WAIT                            0x00000002L
      #define MCI_FROM                            0x00000004L
      #define MCI_TO                              0x00000008L
      #define MCI_DOS_QUEUE                       0x00000008L
      #define MCI_MILLISECONDS                    0x00000010L
      #define MCI_TRACK                           0x00000020L
      #define MCI_OVER                            0x00000040L
      #define MCI_TEST                            0x00000080L
      #define MCI_TO_BUFFER                       0x00000100L
      #define MCI_FROM_BUFFER                     0x00000200L
      #define MCI_CONVERT_FORMAT                  0x00000400L
      
      
      /*********************************************************************/
      /* Time formats                                                      */
      /*********************************************************************/
      #define MCI_FORMAT_MILLISECONDS             0x00000001L
      #define MCI_FORMAT_MMTIME                   0x00000002L
      #define MCI_FORMAT_MSF                      0x00000005L
      #define MCI_FORMAT_TMSF                     0x00000006L
      #define MCI_FORMAT_CHAPTERS                 0x00000007L
      #define MCI_FORMAT_FRAMES                   0x00000008L
      #define MCI_FORMAT_HMS                      0x00000009L
      #define MCI_FORMAT_TRACKS                   0x0000000AL
      #define MCI_FORMAT_BYTES                    0x0000000BL
      #define MCI_FORMAT_SAMPLES                  0x0000000CL
      #define MCI_FORMAT_HMSF                     0x0000000DL
      #define MCI_FORMAT_SET_SMPTE_24             0x0000000EL
      #define MCI_FORMAT_SET_SMPTE_25             0x0000000FL
      #define MCI_FORMAT_SET_SMPTE_30             0x00000010L
      #define MCI_FORMAT_SET_SMPTE_30DROP         0x00000011L
      #define MCI_FORMAT_SET_SONGPTR              0x00000012L
      #define MCI_FORMAT_USEC                     0x00000013L
      
      /*********************************************************************/
      /* Speed formats                                                     */
      /*********************************************************************/
      #define MCI_FORMAT_PERCENTAGE               0x00000003L
      #define MCI_FORMAT_FPS                      0x00000004L
      
      
      /*********************************************************************/
      /*                                                                   */
      /*  Time format conversions macros                                   */
      /*                                                                   */
      /*********************************************************************/
      #define MSECTOMM(value)      (((value) > (0xFFFFFFFF /3)) ? 0L : \
      ((value) * 3))
      
      #define MSECFROMMM(value)    (((value)+1) / 3)
      
      #define REDBOOKTOMM(value)   ((((value) & 0x000000FF) * 60 * 3000) + \
      (((value) & 0x0000FF00) / 0x100 * 3000) + \
      (((value) & 0x00FF0000) / 0x10000 * 3000/75))
      
      #define FPS24TOMM(value) ((((value) & 0x000000FF) * 60 * 60 * 3000) + \
      (((value) & 0x0000FF00) / 0x100 * 60 * 3000) + \
      (((value) & 0x00FF0000) / 0x10000 * 3000) + \
      (((value) & 0xFF000000) / 0x1000000 * 3000/24))
      
      #define FPS25TOMM(value) ((((value) & 0x000000FF) * 60 * 60 * 3000) + \
      (((value) & 0x0000FF00) / 0x100 * 60 * 3000) + \
      (((value) & 0x00FF0000) / 0x10000 * 3000) + \
      (((value) & 0xFF000000) / 0x1000000 * 3000/25))
      
      #define FPS30TOMM(value) ((((value) & 0x000000FF) * 60 * 60 * 3000) + \
      (((value) & 0x0000FF00) / 0x100 * 60 * 3000) + \
      (((value) & 0x00FF0000) / 0x10000 * 3000) + \
      (((value) & 0xFF000000) / 0x1000000 * 3000/30))
      
      #define HMSTOMM(value)   ((((value) & 0x000000FF) * 60 * 60 * 3000) + \
      (((value) & 0x0000FF00) / 0x100 * 60 * 3000) + \
      (((value) & 0x00FF0000) / 0x10000 * 3000))
      
      
      /* The +20 is used for rounding purposes.  It is derived by:           */
      /*   1/2 * ((300 MMTIME/SEC / (75 FRAMES/SEC)) = 20 MMTIME/HALF-FRAME  */
      #define REDBOOKFROMMM(value) ((((value)+20) >= 0x100*60*3000) ? 0L : \
      ((((value)+20) / (60*3000)) + \
      (((value)+20) % (60*3000) / 3000 << 8) + \
      (((value)+20) / (3000/75) % 75 << 16)))
      
      #define FPS24FROMMM(value) ((((value)+63) >= 0x100*60*60*3000) ? 0L : \
      (((((((value)+63) % 3000)/(3000/24)) << 24) & 0xFF000000) | \
      ((((((value)+63) / 3000) % 60) << 16) & 0x00FF0000)      | \
      (((((((value)+63) / 3000) / 60) % 60) << 8) & 0x0000FF00) | \
      ((((((value)+63) / 3000) / 60) / 60)  & 0x000000FF)))
      
      #define FPS25FROMMM(value) ((((value)+60) >= 0x100*60*60*3000) ? 0L : \
      (((((((value)+60) % 3000)/(3000/25)) << 24) & 0xFF000000) | \
      ((((((value)+60) / 3000) % 60) << 16) & 0x00FF0000)      | \
      (((((((value)+60) / 3000) / 60) % 60) << 8) & 0x0000FF00) | \
      ((((((value)+60) / 3000) / 60) / 60)  & 0x000000FF)))
      
      #define FPS30FROMMM(value) ((((value)+50) >= 0x100*60*60*3000) ? 0L : \
      (((((((value)+50) % 3000)/(3000/30)) << 24) & 0xFF000000) | \
      ((((((value)+50) / 3000) % 60) << 16) & 0x00FF0000)      | \
      (((((((value)+50) / 3000) / 60) % 60) << 8) & 0x0000FF00) | \
      ((((((value)+50) / 3000) / 60) / 60)  & 0x000000FF)))
      
      #define HMSFROMMM(value) ((((value)+50) >= 0x100*60*60*3000) ? 0L : \
      (((((((value)+50) / 3000) % 60) << 16) & 0x00FF0000)      | \
      (((((((value)+50) / 3000) / 60) % 60) << 8) & 0x0000FF00) | \
      ((((((value)+50) / 3000) / 60) / 60)  & 0x000000FF)))
      
      #define TMSF_TRACK(time)     (*(BYTE *)(&(time)))
      #define TMSF_MINUTE(time)    (*((BYTE *)(&(time))+1))
      #define TMSF_SECOND(time)    (*((BYTE *)(&(time))+2))
      #define TMSF_FRAME(time)     (*((BYTE *)(&(time))+3))
      #define MSF_MINUTE(time)     (*(BYTE *)(&(time)))
      #define MSF_SECOND(time)     (*((BYTE *)(&(time))+1))
      #define MSF_FRAME(time)      (*((BYTE *)(&(time))+2))
      
      /*********************************************************************/
      /*                                                                   */
      /*  BYTE ACCESS WITH A DWORD MACROS                                  */
      /*                                                                   */
      /*********************************************************************/
      #define ULONG_LWLB(ul)  (*(BYTE *)((ULONG *)(&ul)))       /* Low  Word Low Byte  */
      #define ULONG_LWHB(ul)  (*((BYTE *)((ULONG *)(&ul))+1))   /* Low  Word High Byte */
      #define ULONG_HWLB(ul)  (*((BYTE *)((ULONG *)(&ul))+2))   /* High Word Low Byte  */
      #define ULONG_HWHB(ul)  (*((BYTE *)((ULONG *)(&ul))+3))   /* High Word High Byte */
      #define ULONG_LOWD(ul)  (*(USHORT *)((ULONG *)(&ul)))     /* Low  Word           */
      #define ULONG_HIWD(ul)  (*((USHORT *)((ULONG *)(&ul))+1)) /* High Word           */
      
      
      #pragma pack(1)
      
      /*********************************************************************/
      /* parameters for default command messages with empty parameter lists*/
      /*********************************************************************/
      typedef struct _MCI_GENERIC_PARMS  
      {
         HWND   hwndCallback;     /* PM window handle for MCI notify message */
      } MCI_GENERIC_PARMS;
      typedef MCI_GENERIC_PARMS   *PMCI_GENERIC_PARMS;
      
      
      /*********************************************************************/
      /* flags for the MCI_ACQUIREDEVICE message                           */
      /*     0x00000X00 are reserved for MCI_ACQUIREDEVICE flags           */
      /*********************************************************************/
      #define MCI_EXCLUSIVE                       0x00000100L
      #define MCI_EXCLUSIVE_INSTANCE              0x00000200L
      #define MCI_ACQUIRE_QUEUE                   0x00000400L
      
      
      /*********************************************************************/
      /* flags for the MCI_CAPTURE message                                 */
      /*     0x0000XX00 are reserved for MCI_CAPTURE flags                 */
      /*     MCI_CONVERT is used by MCI_GETIMAGEBUFFER, MCI_SETIMAGEBUFFER */
      /*                            MCI_CAPTURE, and MCI_RESTORE           */
      /*********************************************************************/
      #define MCI_CAPTURE_RECT                    0x00000100L
      #define MCI_CONVERT                         0x00001000L
      
      /*********************************************************************/
      /* parameter structure for the MCI_CAPTURE message                   */
      /*********************************************************************/
      typedef struct _MCI_CAPTURE_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         RECTL   rect;            /* Rectangle array specifying the area to  */
         /* be captured                             */
      } MCI_CAPTURE_PARMS;
      typedef MCI_CAPTURE_PARMS   *PMCI_CAPTURE_PARMS;
      
      
      /*********************************************************************/
      /* flags for MCI_CONNECTOR and MCI_CONNECTION messages               */
      /*********************************************************************/
      #define MCI_ENABLE_CONNECTOR                0x00000100L
      #define MCI_DISABLE_CONNECTOR               0x00000200L
      #define MCI_QUERY_CONNECTOR_STATUS          0x00000400L
      #define MCI_CONNECTOR_TYPE                  0x00000800L
      #define MCI_CONNECTOR_INDEX                 0x00001000L
      #define MCI_TO_CONNECTOR_INDEX              0x00002000L
      #define MCI_TO_CONNECTOR_TYPE               0x00004000L
      #define MCI_QUERY_CONNECTION                0x00008000L
      #define MCI_MAKE_CONNECTION                 0x00010000L
      #define MCI_SET_CONNECTION                  MCI_MAKE_CONNECTION
      #define MCI_BREAK_CONNECTION                0x00020000L
      #define MCI_ENUMERATE_CONNECTORS            0x00040000L
      #define MCI_QUERY_CONNECTOR_TYPE            0x00080000L
      #define MCI_QUERY_VALID_CONNECTION          0x00100000L
      #define MCI_CONNECTOR_ALIAS                 0x00200000L
      #define MCI_EXTERNAL_CONNECTION             0x00400000L
      
      
      /*********************************************************************/
      /* Connector types                                                   */
      /*********************************************************************/
      #define MCI_MIDI_STREAM_CONNECTOR           0x00000001L
      #define MCI_CD_STREAM_CONNECTOR             0x00000002L
      #define MCI_WAVE_STREAM_CONNECTOR           0x00000003L
      #define MCI_AMP_STREAM_CONNECTOR            0x00000004L
      #define MCI_XA_STREAM_CONNECTOR             0x00000005L
      #define MCI_HEADPHONES_CONNECTOR            0x00000006L
      #define MCI_SPEAKERS_CONNECTOR              0x00000007L
      #define MCI_MICROPHONE_CONNECTOR            0x00000008L
      #define MCI_LINE_IN_CONNECTOR               0x00000009L
      #define MCI_LINE_OUT_CONNECTOR              0x0000000aL
      #define MCI_VIDEO_IN_CONNECTOR              0x0000000bL
      #define MCI_VIDEO_OUT_CONNECTOR             0x0000000cL
      #define MCI_PHONE_SET_CONNECTOR             0x0000000dL
      #define MCI_PHONE_LINE_CONNECTOR            0x0000000eL
      #define MCI_AUDIO_IN_CONNECTOR              0x0000000fL
      #define MCI_AUDIO_OUT_CONNECTOR             0x00000010L
      #define MCI_UNIVERSAL_CONNECTOR             0x00000011L
      #define MCI_INTERNAL_AUDIO_CONNECTOR        0x00000012L
      #define MCI_MIDI_IN_CONNECTOR               0x00000013L
      #define MCI_MIDI_OUT_CONNECTOR              0x00000014L
      #define MCI_NULL_CONNECTOR                  0x00000015L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_CONNECTION message                */
      /*********************************************************************/
      typedef struct _MCI_CONNECTION_PARMS  
      {
         HWND    hwndCallback;       /* PM window handle for MCI notify message */
         ULONG   ulConnectorType;    /* Connector type                          */
         ULONG   ulConnectorIndex;   /* Connector index                         */
         PSZ     pszAlias;           /* Connection device alias                 */
         USHORT  usToDeviceID;       /* Connected to device ID                  */
         USHORT  usReserved0;        /* Reserved field                          */
         ULONG   ulToConnectorType;  /* Connection to Connector type            */
         ULONG   ulToConnectorIndex; /* Connection to Connector index           */
      } MCI_CONNECTION_PARMS;
      typedef MCI_CONNECTION_PARMS   *PMCI_CONNECTION_PARMS;
      
      /*********************************************************************/
      /* parameter structure for the MCI_CONNECTOR message                 */
      /*********************************************************************/
      typedef struct _MCI_CONNECTOR_PARMS  
      {
         HWND   hwndCallback;     /* PM window handle for MCI notify message    */
         ULONG  ulReturn;         /* Return information                         */
         ULONG  ulConnectorType;  /* If specified, ulConnectorIndex is relative */
         /* to the specified connector type            */
         ULONG  ulConnectorIndex; /* Connector number                           */
      } MCI_CONNECTOR_PARMS;
      typedef MCI_CONNECTOR_PARMS   *PMCI_CONNECTOR_PARMS;
      
      
      /*********************************************************************/
      /* Flags for use with ulParam1                                       */
      /*********************************************************************/
      
      
      #define MCI_MIXSETUP_INIT            0x00010000L
      #define MCI_MIXSETUP_DEINIT          0x00020000L
      #define MCI_MIXSETUP_QUERYMODE       0x00040000L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_BUFFER    message                 */
      /*********************************************************************/
      typedef struct _MCI_MIX_BUFFER  
      {
         ULONG      ulStructLength;   /* Length of the structure          */
         PVOID      pBuffer;          /* Pointer to a buffer              */
         ULONG      ulBufferLength;   /* Length of the buffer             */
         ULONG      ulFlags;          /* Flags                            */
         ULONG      ulUserParm;       /* Caller parameter                 */
         ULONG      ulTime;           /* OUT--Current time in MS          */
         ULONG      ulReserved1;      /* Unused.                          */
         ULONG      ulReserved2;      /* Unused.                          */
      } MCI_MIX_BUFFER;
      
      typedef MCI_MIX_BUFFER *PMCI_MIX_BUFFER;
      
      
      /*********************************************************************/
      /* valid options for the ulFlags field of MCI_MIX_BUFFER             */
      /*********************************************************************/
      
      
      #define MIX_BUFFER_EOS                0x00000001L
      
      
      
      typedef LONG (APIENTRY MIXERPROC)
                    ( ULONG            ulHandle,
                    PMCI_MIX_BUFFER  pBuffer,
                    ULONG            ulFlags        );
      
      typedef MIXERPROC *PMIXERPROC;
      typedef LONG (APIENTRY MIXEREVENT)
                    ( ULONG            ulStatus,
                    PMCI_MIX_BUFFER  pBuffer,
                    ULONG            ulFlags        );
      
      typedef MIXEREVENT  *PMIXEREVENT;
      
      /*********************************************************************/
      /* valid returns for the ulFlags param of the MIXEREVENT entry       */
      /*********************************************************************/
      
      
      #define MIX_STREAM_ERROR             0x00000080L
      #define MIX_READ_COMPLETE            0x00000001L
      #define MIX_WRITE_COMPLETE           0x00000002L
      
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_MIXSETUP_PARMS message            */
      /*********************************************************************/
      typedef struct _MCI_MIXSETUP_PARMS  
      {
         HWND         hwndCallback;     /* PM window handle for MCI notify message      */
         ULONG        ulBitsPerSample;  /* IN Number of Bits per Sample                 */
         ULONG        ulFormatTag;      /* IN Format Tag                                */
         ULONG        ulSamplesPerSec;  /* IN Sampling Rate                             */
         ULONG        ulChannels;       /* IN Number of channels                        */
         ULONG        ulFormatMode;     /* IN Either MCI_RECORD or MCI_PLAY             */
         ULONG        ulDeviceType;     /* IN MCI_DEVTYPE (i.e. DEVTYPE_WAVEFORM etc.)  */
         ULONG        ulMixHandle;      /* OUT--mixer returns handle for write/read     */
         PMIXERPROC   pmixWrite;        /* OUT-Mixer Write Routine entry point          */
         PMIXERPROC   pmixRead;         /* OUT-Mixer Read Routine entry point           */
         PMIXEREVENT  pmixEvent;        /* IN--Mixer Read Routine entry point           */
         PVOID        pExtendedInfo;    /* Ptr to extended wave information             */
         ULONG        ulBufferSize;     /* OUT--suggested buffer size for current mode  */
         ULONG        ulNumBuffers;     /* OUT--suggested # of buffers for current mode */
      } MCI_MIXSETUP_PARMS;
      
      typedef MCI_MIXSETUP_PARMS   *PMCI_MIXSETUP_PARMS;
      
      /*********************************************************************/
      /* Flags for use with ulParam1                                       */
      /*********************************************************************/
      
      
      
      #define  MCI_BUFFER_QUERY         0x00010000L
      #define  MCI_BUFFER_SET           0x00020000L
      #define  MCI_ALLOCATE_MEMORY      0x00040000L
      #define  MCI_DEALLOCATE_MEMORY    0x00080000L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_BUFFER    message                 */
      /*********************************************************************/
      typedef struct _MCI_BUFFER_PARMS  
      {
         HWND       hwndCallback;     /* PM window handle for MCI notify message    */
         ULONG      ulStructLength;   /* Length of the MCI Buffer command           */
         ULONG      ulNumBuffers;     /* Number of buffers MCI driver should use    */
         ULONG      ulBufferSize;     /* Size of buffers MCI driver should use      */
         ULONG      ulMinToStart;     /* Min number of buffers to create a stream.  */
         ULONG      ulSrcStart;       /* # of EMPTY buffers required to start Source*/
         ULONG      ulTgtStart;       /* # of FULL buffers required to start Target */
         
         PVOID      pBufList;         /* Pointer to a list of buffers               */
         
      } MCI_BUFFER_PARMS;
      typedef MCI_BUFFER_PARMS   *PMCI_BUFFER_PARMS;
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_CONNECTORINFO message             */
      /*********************************************************************/
      typedef struct _MCI_CONNECTORINFO_PARMS  
      {
         HWND   hwndCallback;      /* PM window handle for MCI notify message    */
         ULONG  ulReturn;          /* Return information                         */
         ULONG  ulDeviceTypeID;    /* MCI device type                            */
         ULONG  ulConnectorType;   /* If specified, ulConnectorIndex is relative */
         /* to the specified connector type            */
         ULONG  ulConnectorIndex;  /* Connector number                           */
         ULONG  ulToConnectorType; /* Connector type to test if                  */
         /* MCI_QUERY_VALID_CONNECTION is specified    */
      } MCI_CONNECTORINFO_PARMS;
      typedef MCI_CONNECTORINFO_PARMS   *PMCI_CONNECTORINFO_PARMS;
      
      /*********************************************************************/
      /* flags for the MCI_CUE message                                     */
      /*     0x00000x00 are reserved for MCI_CUE flags                     */
      /*********************************************************************/
      #define MCI_CUE_INPUT                       0x00000100L
      #define MCI_CUE_OUTPUT                      0x00000200L
      #define MCI_SHOW                            0x00000400L
      #define MCI_NOSHOW                          0x00000800L
      
      /*************************************************/
      /* Additional flags for Wave Audio for MCI_CUE   */
      /*************************************************/
      #define MCI_WAVE_INPUT                      0x00001000L
      #define MCI_WAVE_OUTPUT                     0x00002000L
      
      
      /*****************************************************************************/
      /* the MCI_CONNLIST structure used in the NETWORK_DEFAULT_CONNECTION message */
      /*****************************************************************************/
      typedef struct _MCI_CONNLIST  
      {
         ULONG ulFromDevice;  /* Ordinal position of device within pDevices array */
         ULONG ulSrcConnType; /* Type of source connector                         */
         ULONG ulSrcConnNum;  /* Source Connector Number                          */
         ULONG ulToDevice;    /* Ordinal position of device within pDevices array */
         ULONG ulTgtConnType; /* Type of target connector                         */
         ULONG ulTgtConnNum;  /* Target Connector Number                          */
      } MCI_CONNLIST;
      typedef MCI_CONNLIST *PMCI_CONNLIST;
      
      
      /*****************************************************************************/
      /* parameter structure for the MCI_NETWORK_DEFAULT_CONNECTION message        */
      /*****************************************************************************/
      typedef struct _MCI_NETWORK_DEFAULT_CONNECTION_PARMS  
      {
         HWND            hwndCallback;                   /* PM window handle for MCI notify message    */
         CHAR            szInstallName[MAX_DEVICE_NAME]; /* Name of section containing default conn.   */
         ULONG           ulNumDevices;                   /* Number of additional devices in connection */
         ULONG           ulNumPlayConnections;           /* Number of Play connections                 */
         ULONG           ulNumRecordConnections;         /* Number of Record connections               */
         CHAR           (*pDevices)[MAX_DEVICE_NAME];    /* Pointer to array of device names to open   */
         PMCI_CONNLIST   pPlayConnections;               /* Pointer to array of connections to "make"  */
         PMCI_CONNLIST   pRecordConnections;             /* Pointer to array of connections to "make"  */
      } MCI_NETWORK_DEFAULT_CONNECTION_PARMS;
      typedef MCI_NETWORK_DEFAULT_CONNECTION_PARMS *PMCI_NETWORK_DEFAULT_CONNECTION_PARMS;
      
      
      /*******************************************************/
      /* Additional flags for MCI_NETWORK_DEFAULT_CONNECTION */
      /*******************************************************/
      #define MCI_RECORD_DEFAULTS             0x00000004L
      #define MCI_PLAY_DEFAULTS               0x00000008L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_DEFAULT_CONNECTION message        */
      /*********************************************************************/
      typedef struct _MCI_DEFAULT_CONNECTION_PARMS  
      {
         HWND  hwndCallback;       /* PM window handle for MCI notify message    */
         PSZ   pszDevice;          /* Device name                                */
         ULONG ulConnectorType;    /* If specified, ulConnectorIndex is relative */
         /* to the specified connector type            */
         ULONG ulConnectorIndex;   /* Connector number                           */
         PSZ   pszToDevice;        /* Return device name to which the connection */
         /* exists                                     */
         ULONG ulToConnectorType;  /* Connector type                             */
         ULONG ulToConnectorIndex; /* Connector number                           */
      } MCI_DEFAULT_CONNECTION_PARMS;
      typedef MCI_DEFAULT_CONNECTION_PARMS    *PMCI_DEFAULT_CONNECTION_PARMS;
      
      /*********************************************************************/
      /* parameter structure for the MCI_DEVICESETTINGS message            */
      /*********************************************************************/
      typedef struct _MCI_DEVICESETTINGS_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message    */
         HWND    hwndNotebook;    /* Hwhd to notebook window                    */
         USHORT  usDeviceType;    /* Device type                                */
         PSZ     pszDeviceName;   /* Device name                                */
      } MCI_DEVICESETTINGS_PARMS;
      typedef MCI_DEVICESETTINGS_PARMS   *PMCI_DEVICESETTINGS_PARMS;
      
      /**********************************************************************/
      /* parameter structure for the MCI_CUT / COPY / DELETE / UNDO / etc...*/
      /**********************************************************************/
      typedef struct _MCI_EDIT_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message     */
         ULONG   ulStructLen;     /* Length of the Structure                     */
         ULONG   ulFrom;          /* Beginning Point of Range                    */
         ULONG   ulTo;            /* ending point of range                       */
         PVOID   pBuff;           /* user buffer                                 */
         ULONG   ulBufLen;        /* length of user buffer                       */
         PVOID   pHeader;         /* header which describes the buffer           */
      } MCI_EDIT_PARMS;
      typedef MCI_EDIT_PARMS   *PMCI_EDIT_PARMS;
      
      /*********************************************************************/
      /* flags and parameter structure for the MCI_ESCAPE message          */
      /*     0x00000X00 are reserved for MCI_ESCAPE flags                  */
      /*********************************************************************/
      #define MCI_ESCAPE_STRING                   0x00000100L
      
      typedef struct _MCI_ESCAPE_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message    */
         PSZ     pszCommand;      /* Command to send to the device              */
      } MCI_ESCAPE_PARMS;
      typedef MCI_ESCAPE_PARMS   *PMCI_ESCAPE_PARMS;
      
      /*********************************************************************/
      /* flags for the MCI_FREEZE message                                  */
      /*     0x0000XX00 are reserved for MCI_FREEZE flags                  */
      /*********************************************************************/
      #define MCI_OVLY_FREEZE_RECT                0x00000100L
      #define MCI_OVLY_FREEZE_RECT_OUTSIDE        0x00000200L
      
      /*********************************************************************/
      /* flags for the MCI_GETDEVCAPS message                              */
      /*     0x00XXXX00 are reserved for MCI_GETDEVCAPS flags              */
      /*********************************************************************/
      #define MCI_GETDEVCAPS_MESSAGE              0x00000100L
      #define MCI_GETDEVCAPS_ITEM                 0x00000200L
      #define MCI_GETDEVCAPS_EXTENDED             0x00000400L
      
      /*************************************************/
      /* General MCI_GETDEVCAPS item values            */
      /*************************************************/
      #define MCI_GETDEVCAPS_CAN_EJECT            0x00000001L
      #define MCI_GETDEVCAPS_CAN_LOCKEJECT        0x00000002L
      #define MCI_GETDEVCAPS_CAN_PLAY             0x00000003L
      #define MCI_GETDEVCAPS_CAN_PROCESS_INTERNAL 0x00000004L
      #define MCI_GETDEVCAPS_CAN_RECORD           0x00000005L
      #define MCI_GETDEVCAPS_CAN_RECORD_INSERT    0x00000006L
      #define MCI_GETDEVCAPS_CAN_SAVE             0x00000007L
      #define MCI_GETDEVCAPS_CAN_SETVOLUME        0x00000008L
      #define MCI_GETDEVCAPS_CAN_STREAM           0x00000009L
      #define MCI_GETDEVCAPS_DEVICE_TYPE          0x0000000AL
      #define MCI_GETDEVCAPS_HAS_AUDIO            0x0000000BL
      #define MCI_GETDEVCAPS_HAS_VIDEO            0x0000000CL
      #define MCI_GETDEVCAPS_PREROLL_TIME         0x0000000DL
      #define MCI_GETDEVCAPS_PREROLL_TYPE         0x0000000EL
      #define MCI_GETDEVCAPS_USES_FILES           0x0000000FL
      #define MCI_GETDEVCAPS_HAS_IMAGE            0x00000010L
      #define MCI_GETDEVCAPS_WAVE_FORMAT          0x00000011L
      #define MCI_GETDEVCAPS_CAN_CLOSE_DOOR       0x00000012L
      
      /***********************************************/
      /* return ID's for videodisc MCI_GETDEVCAPS    */
      /***********************************************/
      #define MCI_VD_MEDIA_CLV                    0x00000001L
      #define MCI_VD_MEDIA_CAV                    0x00000002L
      #define MCI_VD_MEDIA_OTHER                  0x00000003L
      
      /*************************************************/
      /* MCI_GETDEVCAPS item values for video devices  */
      /*************************************************/
      #define MCI_VID_GETDEVCAPS_CAN_STRETCH       MCI_VID_ITEM_BASE
      #define MCI_VID_GETDEVCAPS_CAN_DISTORT       (MCI_VID_ITEM_BASE+1)
      #define MCI_VID_GETDEVCAPS_VIDEO_X_EXTENT    (MCI_VID_ITEM_BASE+2)
      #define MCI_VID_GETDEVCAPS_VIDEO_Y_EXTENT    (MCI_VID_ITEM_BASE+3)
      #define MCI_VID_GETDEVCAPS_IMAGE_X_EXTENT    (MCI_VID_ITEM_BASE+4)
      #define MCI_VID_GETDEVCAPS_IMAGE_Y_EXTENT    (MCI_VID_ITEM_BASE+5)
      #define MCI_VID_GETDEVCAPS_MAX_WINDOWS       (MCI_VID_ITEM_BASE+6)
      #define MCI_VID_GETDEVCAPS_CAN_FREEZE        (MCI_VID_ITEM_BASE+7)
      #define MCI_VID_GETDEVCAPS_OVERLAY_GRAPHICS  (MCI_VID_ITEM_BASE+8)
      
      #define MCI_VID_GETDEVCAPS_CAN_REVERSE       (MCI_VID_ITEM_BASE+9)
      #define MCI_VID_GETDEVCAPS_FAST_RATE         (MCI_VID_ITEM_BASE+10)
      #define MCI_VID_GETDEVCAPS_SLOW_RATE         (MCI_VID_ITEM_BASE+11)
      #define MCI_VID_GETDEVCAPS_NORMAL_RATE       (MCI_VID_ITEM_BASE+12)
      #define MCI_VID_GETDEVCAPS_MINIMUM_RATE      (MCI_VID_ITEM_BASE+13)
      #define MCI_VID_GETDEVCAPS_MAXIMUM_RATE      (MCI_VID_ITEM_BASE+14)
      
      /*************************************************/
      /* MCI_GETDEVCAPS flag values for Videodisc      */
      /*************************************************/
      #define MCI_VD_GETDEVCAPS_CLV               0x00000400L
      #define MCI_VD_GETDEVCAPS_CAV               0x00000800L
      
      /*************************************************/
      /* MCI_GETDEVCAPS item values for Videodisc      */
      /*************************************************/
      
      #define MCI_VD_GETDEVCAPS_CAN_REVERSE       MCI_VID_GETDEVCAPS_CAN_REVERSE
      #define MCI_VD_GETDEVCAPS_FAST_RATE         MCI_VID_GETDEVCAPS_FAST_RATE
      #define MCI_VD_GETDEVCAPS_SLOW_RATE         MCI_VID_GETDEVCAPS_SLOW_RATE
      #define MCI_VD_GETDEVCAPS_NORMAL_RATE       MCI_VID_GETDEVCAPS_NORMAL_RATE
      #define MCI_VD_GETDEVCAPS_MINIMUM_RATE      MCI_VID_GETDEVCAPS_MINIMUM_RATE
      #define MCI_VD_GETDEVCAPS_MAXIMUM_RATE      MCI_VID_GETDEVCAPS_MAXIMUM_RATE
      
      /************************************************/
      /* MCI_GETDEVCAPS item values for Digital Video */
      /************************************************/
      #define MCI_DGV_GETDEVCAPS_CAN_REVERSE      MCI_VID_GETDEVCAPS_CAN_REVERSE
      #define MCI_DGV_GETDEVCAPS_CAN_STRETCH      MCI_VID_GETDEVCAPS_CAN_STRETCH
      #define MCI_DGV_GETDEVCAPS_CAN_DISTORT      MCI_VID_GETDEVCAPS_CAN_DISTORT
      #define MCI_DGV_GETDEVCAPS_FAST_RATE        MCI_VID_GETDEVCAPS_FAST_RATE
      #define MCI_DGV_GETDEVCAPS_SLOW_RATE        MCI_VID_GETDEVCAPS_SLOW_RATE
      #define MCI_DGV_GETDEVCAPS_NORMAL_RATE      MCI_VID_GETDEVCAPS_NORMAL_RATE
      #define MCI_DGV_GETDEVCAPS_MINIMUM_RATE     MCI_VID_GETDEVCAPS_MINIMUM_RATE
      #define MCI_DGV_GETDEVCAPS_MAXIMUM_RATE     MCI_VID_GETDEVCAPS_MAXIMUM_RATE
      #define MCI_DGV_GETDEVCAPS_VIDEO_X_EXTENT   MCI_VID_GETDEVCAPS_VIDEO_X_EXTENT
      #define MCI_DGV_GETDEVCAPS_VIDEO_Y_EXTENT   MCI_VID_GETDEVCAPS_VIDEO_Y_EXTENT
      #define MCI_DGV_GETDEVCAPS_IMAGE_X_EXTENT   MCI_VID_GETDEVCAPS_IMAGE_X_EXTENT
      #define MCI_DGV_GETDEVCAPS_IMAGE_Y_EXTENT   MCI_VID_GETDEVCAPS_IMAGE_Y_EXTENT
      #define MCI_DGV_GETDEVCAPS_MAX_WINDOWS      MCI_VID_GETDEVCAPS_MAX_WINDOWS
      #define MCI_DGV_GETDEVCAPS_OVERLAY_GRAPHICS MCI_VID_GETDEVCAPS_OVERLAY_GRAPHICS
      #define MCI_DGV_GETDEVCAPS_HAS_TUNER        (MCI_DGV_ITEM_BASE)
      #define MCI_DGV_GETDEVCAPS_HAS_TELETEX      (MCI_DGV_ITEM_BASE+1)
      #define MCI_DGV_GETDEVCAPS_HAS_AFC          (MCI_DGV_ITEM_BASE+2)
      
      /*************************************************/
      /* MCI_GETDEVCAPS item values for Video Overlay  */
      /*************************************************/
      #define MCI_OVLY_GETDEVCAPS_CAN_STRETCH     MCI_VID_GETDEVCAPS_CAN_STRETCH
      #define MCI_OVLY_GETDEVCAPS_CAN_DISTORT     MCI_VID_GETDEVCAPS_CAN_DISTORT
      #define MCI_OVLY_GETDEVCAPS_VIDEO_X_EXTENT  MCI_VID_GETDEVCAPS_VIDEO_X_EXTENT
      #define MCI_OVLY_GETDEVCAPS_VIDEO_Y_EXTENT  MCI_VID_GETDEVCAPS_VIDEO_Y_EXTENT
      #define MCI_OVLY_GETDEVCAPS_IMAGE_X_EXTENT  MCI_VID_GETDEVCAPS_IMAGE_X_EXTENT
      #define MCI_OVLY_GETDEVCAPS_IMAGE_Y_EXTENT  MCI_VID_GETDEVCAPS_IMAGE_Y_EXTENT
      #define MCI_OVLY_GETDEVCAPS_MAX_WINDOWS     MCI_VID_GETDEVCAPS_MAX_WINDOWS
      #define MCI_OVLY_GETDEVCAPS_CAN_FREEZE      MCI_VID_GETDEVCAPS_CAN_FREEZE
      #define MCI_OVLY_GETDEVCAPS_OVERLAY_GRAPHICS MCI_VID_GETDEVCAPS_OVERLAY_GRAPHICS
      
      
      #define MCI_PREROLL_NOTIFIED                0x00000001L
      #define MCI_PREROLL_DETERMINISTIC           0x00000002L
      #define MCI_PREROLL_NONE                    0x00000003L
      
      /*********************************************************************/
      /* parameter structure for the MCI_GETDEVCAPS message                */
      /*********************************************************************/
      typedef struct _MCI_GETDEVCAPS_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulReturn;        /* Return field                            */
         ULONG   ulItem;          /* Item field for GETDEVCAPS item to query */
         USHORT  usMessage;       /* Field to hold MCI message to query      */
         USHORT  usReserved0;     /* Reserved field                          */
      } MCI_GETDEVCAPS_PARMS;
      typedef MCI_GETDEVCAPS_PARMS   * PMCI_GETDEVCAPS_PARMS;
      
      /*********************************************************************/
      /* parameter structure for the MCI_WAVE_GETDEVCAPS                   */
      /*********************************************************************/
      typedef struct _MCI_WAVE_GETDEVCAPS_PARMS  
      {
         HWND    hwndCallback;     /* PM window handle for MCI notify message */
         ULONG   ulReturn;         /* Return field                            */
         ULONG   ulItem;           /* Item field for GETDEVCAPS item to query */
         USHORT  usMessage;        /* Field to hold MCI message to query      */
         USHORT  usReserved0;      /* Reserved field                          */
         ULONG   ulLength;         /* Length of structure in ULONGS           */
         ULONG   ulBitsPerSample;  /* Number of Bits per Sample               */
         ULONG   ulFormatTag;      /* Format Tag                              */
         ULONG   ulSamplesPerSec;  /* Sampling Rate                           */
         ULONG   ulChannels;       /* Number of channels                      */
         ULONG   ulFormatMode;     /* Either MCI_RECORD or MCI_PLAY           */
      } MCI_WAVE_GETDEVCAPS_PARMS;
      typedef MCI_WAVE_GETDEVCAPS_PARMS   * PMCI_WAVE_GETDEVCAPS_PARMS;
      
      /*********************************************************************/
      /* potential values for the ulValue field of MCI_AMP_GETDEVCAPS  */
      /*********************************************************************/
      #define  MCI_AMP_CAN_SET_MONITOR           MCI_AMP_SET_MONITOR
      #define  MCI_AMP_CAN_SET_BALANCE           MCI_AMP_SET_BALANCE
      #define  MCI_AMP_CAN_SET_ALC               MCI_AMP_SET_ALC
      #define  MCI_AMP_CAN_SET_MONITOR           MCI_AMP_SET_MONITOR
      #define  MCI_AMP_CAN_SET_CROSSOVER         MCI_AMP_SET_CROSSOVER
      #define  MCI_AMP_CAN_SET_LOUDNESS          MCI_AMP_SET_LOUDNESS
      #define  MCI_AMP_CAN_SET_MUTE              MCI_AMP_SET_MUTE
      #define  MCI_AMP_CAN_SET_REVERB            MCI_AMP_SET_REVERB
      #define  MCI_AMP_CAN_SET_STEREOENHANCE     MCI_AMP_SET_STEREOENHANCE
      #define  MCI_AMP_CAN_SET_CUSTOM1           MCI_AMP_SET_CUSTOM1
      #define  MCI_AMP_CAN_SET_CUSTOM2           MCI_AMP_SET_CUSTOM2
      #define  MCI_AMP_CAN_SET_CUSTOM3           MCI_AMP_SET_CUSTOM3
      #define  MCI_AMP_CAN_SET_LRVOLUME          MCI_AMP_SET_LRVOLUME
      #define  MCI_AMP_CAN_SET_BASS              MCI_AMP_SET_BASS
      #define  MCI_AMP_CAN_SET_MID               MCI_AMP_SET_MID
      #define  MCI_AMP_CAN_SET_TREBLE            MCI_AMP_SET_TREBLE
      #define  MCI_AMP_CAN_SET_PITCH             MCI_AMP_SET_PITCH
      #define  MCI_AMP_CAN_SET_GAIN              MCI_AMP_SET_GAIN
      #define  MCI_AMP_CAN_SET_CHORUS            MCI_AMP_SET_CHORUS
      #define  MCI_AMP_CAN_SET_VOLUME            MCI_AMP_SET_VOLUME
      
      
      /*************************************************************************/
      /* potential values for the ulExtended field of MCI_AMP_GETDEVCAPS_PARMS */
      /*************************************************************************/
      #define MCI_MIXER_LINE    0x00000001L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_AMP_GETDEVCAPS                    */
      /*********************************************************************/
      typedef struct _MCI_AMP_GETDEVCAPS_PARMS  
      {
         ULONG   hwndCallback;     /* PM window handle for MCI notify message */
         ULONG   ulReturn;         /* Return field                            */
         ULONG   ulItem;           /* Item field for GETDEVCAPS item to query */
         USHORT  usMessage;        /* Field to hold MCI message to query      */
         USHORT  usReserved0;      /* Reserved field                          */
         ULONG   ulLength;         /* Length of structure in ULONGS           */
         ULONG   ulValue;          /* Value to determine caps                 */
         ULONG   ulAttribute;      /* Flags to modified the extended parms    */
         ULONG   ulExtended;       /* Extended flags field                    */
      } MCI_AMP_GETDEVCAPS_PARMS;
      typedef MCI_AMP_GETDEVCAPS_PARMS   * PMCI_AMP_GETDEVCAPS_PARMS;
      
      
      /*********************************************************************/
      /* values for the ulFlags field of MCI_MIXEVENT_PARMS                */
      /*********************************************************************/
      #define MCI_MIX_ATTRIBUTE       0x00000001L
      #define MCI_MIX_CONNECTOR       0x00000002L
      
      /*********************************************************************/
      /* values for the lParam field for mix_notify                        */
      /*********************************************************************/
      
      #define MCI_MIXNOTIFY_ON        0x00000004L
      #define MCI_MIXNOTIFY_OFF       0x00000008L
      
      /*********************************************************************/
      /* value to indicate type of MM_MCIEVENT                             */
      /*********************************************************************/
      
      #define MCI_MIXEVENT            0x00000001L
      
      
      /*********************************************************************/
      /* parameter structure for the MCI_MIXEVENT_PARMS                    */
      /*********************************************************************/
      typedef struct _MCI_MIXEVENT_PARMS
      {
         ULONG  ulLength;        /* Length of struct                          */
         HWND   hwndMixer;       /* window to inform of mixer changes         */
         ULONG  ulFlags;         /* Either MCI_MIX_ATTRIBUTE                  */
         /*      or MCI_MIX_CONNECTOR                 */
         USHORT usDeviceID;      /* device id to notify of the change         */
         ULONG  ulDeviceType;    /* Device Type which generated the change    */
         ULONG  ulDeviceOrdinal; /* Ordinal of device Type                    */
         ULONG  ulAttribute;     /* Attribute that changed(volume, bass etc.) */
         ULONG  ulValue;         /* new value of the attribute which changed  */
         ULONG  ulConnectorType; /* Connector Type                            */
         ULONG  ulConnectorIndex;/* Connector Index                           */
         ULONG  ulConnStatus;    /* Is connector enabled/disabled.            */
         
      } MCI_MIXEVENT_PARMS;
      
      typedef MCI_MIXEVENT_PARMS *PMCI_MIXEVENT_PARMS;
      
      
      /*********************************************************************/
      /* flag values for the MCI_GETIMAGEBUFFER and MCI_SETIMAGEBUFFER msg */
      /*     0x0000XX00 are reserved for MCI_GETIMAGEBUFFER and            */
      /*       MCI_SETIMAGEBUFFER Flags                                    */
      /*     MCI_CONVERT is defined by MCI_CAPTURE message as 0x00001000L  */
      /*********************************************************************/
      #define MCI_USE_HW_BUFFER                   0x00000400L
      #define MCI_GET_HW_BUFFER_PTR               0x00000800L
      
      /***************************************************/
      /* parm structure for MCI_GETIMAGEBUFFER and       */
      /*                    MCI_SETIMAGEBUFFER messages  */
      /***************************************************/
      typedef  struct _MCI_IMAGE_PARMS  
      {
         HWND    hwndCallback;       /* PM window handle for MCI notify message */
         ULONG   ulPelFormat;        /* Format of the image data returned       */
         USHORT  usBitCount;         /* Number of bitsimage data returned       */
         USHORT  usReserved0;        /* Reserved field                          */
         ULONG   ulImageCompression; /*                                         */
         RECTL   rect;               /* Image area to get/set                   */
         PVOID   pPelBuffer;         /* Pel data buffer                         */
         ULONG   ulPelBufferHeight;  /* Number of rows in buffer                */
         ULONG   ulPelBufferWidth;   /* Pels per row in buffer                  */
         ULONG   ulBufLen;           /* Pel data buffer length in bytes         */
         
      } MCI_IMAGE_PARMS;
      typedef MCI_IMAGE_PARMS   *PMCI_IMAGE_PARMS;
      
      /*********************************************************************/
      /* flags for the MCI_GETIMAGEPALETTE message                         */
      /*     0x0000XX00 are reserved for MCI_GETIMAGEPALETTE flags         */
      /*********************************************************************/
      #define MCI_FIND_BEST_REGISTERED            0x00000100L
      #define MCI_QUERY_REGISTERED_MAP            0x00000200L
      #define MCI_QUERY_REGISTERED_MAP_SIZE       0x00000400L
      
      
      /***************************************************/
      /* parameter structure for MCI_GETIMAGEPALETTE and */
      /*    MCI_SETIMAGEPALETTE messages                 */
      /***************************************************/
      typedef  struct _MCI_PALETTE_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message   */
         USHORT  usRegisteredMap; /* Id of the palette from the registed color */
         /* map                                       */
         USHORT  usReserved0;     /* Reserved field                            */
         ULONG   ulPalEntries;    /* size of the palette returned              */
         PVOID   pPalette;        /* pointer to the palette                    */
      } MCI_PALETTE_PARMS;
      typedef MCI_PALETTE_PARMS   *PMCI_PALETTE_PARMS;
      
      
      /*********************************************************************/
      /* Parameter structure for the MCI_GETTOC message                    */
      /*********************************************************************/
      typedef struct _MCI_TOC_REC  
      {
         BYTE      TrackNum;      /* Returned Track Number                          */
         ULONG     ulStartAddr;   /* Starting address of the track in MMTIME format */
         ULONG     ulEndAddr;     /* Ending address of the track in MMTIME format   */
         BYTE      Control;       /* Track Control information                      */
         USHORT    usCountry;     /* Country                                        */
         ULONG     ulOwner;       /* Owner                                          */
         ULONG     ulSerialNum;   /* Serial Number                                  */
      } MCI_TOC_REC;
      typedef MCI_TOC_REC   *PTOCREC;
      
      typedef struct _MCI_TOC_PARMS  
      {
         HWND      hwndCallback;  /* PM window handle for MCI notify message    */
         PTOCREC   pBuf;          /* Pointer to array MCI_TOC_REC structures to */
         /* be filled in by the mci device             */
         ULONG     ulBufSize;     /* Size of the array                          */
      } MCI_TOC_PARMS;
      typedef MCI_TOC_PARMS   * PMCI_TOC_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_GROUP message                              */
      /*     0x00XXXX00 are reserved for MCI_GROUP flags                   */
      /*********************************************************************/
      #define MCI_SYNCHRONIZE                     0x00000100L
      #define MCI_NOPIECEMEAL                     0x00000200L
      #define MCI_GROUP_MAKE                      0x00000400L
      #define MCI_GROUP_DELETE                    0x00000800L
      #define MCI_GROUP_ALIAS                     0x00001000L
      #define MCI_GROUP_MASTER                    0x00002000L
      
      typedef struct _MCI_GROUP_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message.    */
         USHORT  usGroupID;       /* GroupID                                     */
         USHORT  usReserved0;     /* RESERVED                                    */
         ULONG   ulStructLength;  /* Length of Structure in ULONGS.              */
         USHORT  usMasterID;      /* ID of Master device in Synchrounous groups. */
         USHORT  usReserved1;     /* RESERVED                                    */
         PSZ     pszGroupAlias;   /* Pointer to Alias Name.                      */
         ULONG   ulNumDevices;    /* Number of Devices in group.                 */
         PULONG  paulDeviceID;    /* Array of Device ids in the group.           */
      } MCI_GROUP_PARMS;
      typedef MCI_GROUP_PARMS *PMCI_GROUP_PARMS;
      
      
      /*********************************************************************/
      /* flags for the MCI_INFO message                                    */
      /*     0x00000X00 are reserved for MCI_INFO flags                    */
      /*********************************************************************/
      #define MCI_INFO_PRODUCT                    0x00000100L
      #define MCI_INFO_FILE                       0x00000200L
      
      /************************************************/
      /* CD Audio                MCI_INFO             */
      /************************************************/
      #define MCI_CD_INFO_ID                      0x00010000L
      #define MCI_CD_INFO_UPC                     0x00020000L
      
      /************************************************/
      /* Videodisc               MCI_INFO             */
      /************************************************/
      #define MCI_VD_INFO_LABEL                   0x00001000L
      
      /************************************************/
      /* Digital Video           MCI_INFO             */
      /************************************************/
      #define MCI_DGV_INFO_VIDEO_FILE             0x00001000L
      #define MCI_DGV_INFO_IMAGE_FILE             0x00002000L
      #define MCI_DGV_INFO_REGION                 0x00004000L
      #define MCI_DGV_INFO_REGION_TEXT            0x00008000L
      #define MCI_DGV_INFO_TEXT                   0x00010000L
      
      /************************************************/
      /* Video Overlay           MCI_INFO             */
      /************************************************/
      #define MCI_OVLY_INFO_TEXT                  0x00010000L
      
      
      typedef struct _MCI_INFO_PARMS  
      {
         HWND      hwndCallback;  /* PM window handle for MCI notify message */
         PSZ       pszReturn;     /* Pointer to return buffer                */
         ULONG     ulRetSize;     /* Return buffer size                      */
      } MCI_INFO_PARMS;
      typedef MCI_INFO_PARMS   *PMCI_INFO_PARMS;
      
      /*********************************************************************/
      /* parameters and flags for the MCI_LOAD message                     */
      /*     0x00000X00 are reserved for MCI_LOAD flags                    */
      /*********************************************************************/
      typedef struct _MCI_LOAD_PARMS  
      {
         HWND   hwndCallback;     /* PM window handle for MCI notify message */
         PSZ    pszElementName;   /* File name to loads                      */
      } MCI_LOAD_PARMS;
      typedef MCI_LOAD_PARMS   *PMCI_LOAD_PARMS;
      
      /*********************************************************************/
      /* parameters and flags for the MCI_MASTERAUDIO                      */
      /*     0x000XXX00 are reserved for MCI_MASTERAUDIO  flags            */
      /*********************************************************************/
      #define MCI_ON                              0x00000100L
      #define MCI_OFF                             0x00000200L
      #define MCI_HEADPHONES                      0x00000400L
      #define MCI_SPEAKERS                        0x00000800L
      #define MCI_MASTERVOL                       0x00001000L
      #define MCI_SAVESETTING                     0x00002000L
      #define MCI_QUERYSAVEDSETTING               0x00004000L
      #define MCI_QUERYCURRENTSETTING             0x00008000L
      
      
      typedef struct _MCI_MASTERAUDIO_PARMS  
      {
         HWND    hwndDummyCallback; /* Notify not allowed for this message     */
         ULONG   ulReturn;          /* Return field for query information      */
         ULONG   ulMasterVolume;    /* Master volume field                     */
      } MCI_MASTERAUDIO_PARMS;
      typedef MCI_MASTERAUDIO_PARMS   *PMCI_MASTERAUDIO_PARMS;
      
      /*********************************************************************/
      /* parameters and flags for the MCI_OPEN message                     */
      /*     0x0000XX00 are reserved for MCI_OPEN flags                    */
      /*********************************************************************/
      #define MCI_OPEN_ELEMENT                    0x00000100L
      #define MCI_OPEN_ALIAS                      0x00000200L
      #define MCI_OPEN_ELEMENT_ID                 0x00000400L
      #define MCI_OPEN_PLAYLIST                   0x00000800L
      #define MCI_OPEN_TYPE_ID                    0x00001000L
      #define MCI_OPEN_SHAREABLE                  0x00002000L
      #define MCI_OPEN_MMIO                       0x00004000L
      #define MCI_READONLY                        0x00008000L
      
      /***************************************************/
      /* parameters and flags for the MCI_OPEN message   */
      /* Digital Video, Video Overlay specific           */
      /***************************************************/
      #define MCI_VID_OPEN_PARENT                 0x01000000L
      #define MCI_DGV_OPEN_PARENT                 MCI_VID_OPEN_PARENT
      #define MCI_OVLY_OPEN_PARENT                MCI_VID_OPEN_PARENT
      
      typedef struct _MCI_OPEN_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         USHORT  usDeviceID;      /* Device ID returned to user              */
         USHORT  usReserved0;     /* Reserved                                */
         PSZ     pszDeviceType;   /* Device name from SYSTEM.INI             */
         PSZ     pszElementName;  /* Typically a file name or NULL           */
         PSZ     pszAlias;        /* Optional device alias                   */
      } MCI_OPEN_PARMS;
      typedef MCI_OPEN_PARMS   *PMCI_OPEN_PARMS;
      
      /**********************************************************/
      /* parameters for the AMP MCI_OPEN message                */
      /**********************************************************/
      typedef struct _MCI_AMP_OPEN_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         USHORT  usDeviceID;      /* Device ID returned to user              */
         USHORT  usReserved0;     /* Reserved field                          */
         PSZ     pszDeviceType;   /* Device name from SYSTEM.INI             */
         PSZ     pszElementName;  /* Typically a file name or NULL           */
         PSZ     pszAlias;        /* Optional device alias                   */
         PVOID   pDevDataPtr;     /* Pointer to device data                  */
      } MCI_AMP_OPEN_PARMS;
      typedef MCI_AMP_OPEN_PARMS   *PMCI_AMP_OPEN_PARMS;
      
      /**********************************************************/
      /* parameters for MCI_OPEN message for video devices      */
      /**********************************************************/
      
      typedef  struct _MCI_VID_OPEN_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         USHORT  usDeviceID;      /* Device ID returned to user              */
         USHORT  usReserved0;     /* Reserved field                          */
         PSZ     pszDeviceType;   /* Device name from SYSTEM.INI             */
         PSZ     pszElementName;  /* Typically a file name or NULL           */
         PSZ     pszAlias;        /* Optional device alias                   */
         HWND    hwndParent;      /* Parent window handle                    */
      } MCI_VID_OPEN_PARMS;
      
      typedef MCI_VID_OPEN_PARMS   MCI_DGV_OPEN_PARMS;
      typedef MCI_DGV_OPEN_PARMS   *PMCI_DGV_OPEN_PARMS;
      
      typedef MCI_VID_OPEN_PARMS    MCI_OVLY_OPEN_PARMS;
      typedef MCI_OVLY_OPEN_PARMS   *PMCI_OVLY_OPEN_PARMS;
      
      /***************************************************/
      /* MCI_PLAY flag values for videodisc              */
      /***************************************************/
      #define MCI_VD_PLAY_REVERSE                 0x00001000L
      #define MCI_VD_PLAY_FAST                    0x00002000L
      #define MCI_VD_PLAY_SPEED                   0x00004000L
      #define MCI_VD_PLAY_SCAN                    0x00008000L
      #define MCI_VD_PLAY_SLOW                    0x00010000L
      
      /***************************************************/
      /* MCI_PLAY flag values for digital video          */
      /***************************************************/
      #define MCI_DGV_PLAY_REVERSE                0x00001000L
      #define MCI_DGV_PLAY_FAST                   0x00002000L
      #define MCI_DGV_PLAY_SPEED                  0x00004000L
      #define MCI_DGV_PLAY_SCAN                   0x00008000L
      #define MCI_DGV_PLAY_SLOW                   0x00010000L
      #define MCI_DGV_PLAY_REPEAT                 0x00020000L
      
      typedef struct _MCI_PLAY_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulFrom;          /* Play from this position                 */
         ULONG   ulTo;            /* Play to this position                   */
      } MCI_PLAY_PARMS;
      typedef MCI_PLAY_PARMS   *PMCI_PLAY_PARMS;
      
      typedef struct _MCI_VD_PLAY_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulFrom;          /* Play from this position                 */
         ULONG   ulTo;            /* Play to this position                   */
         ULONG   ulFactor;        /* Play at this speed factor               */
      } MCI_VD_PLAY_PARMS;
      typedef MCI_VD_PLAY_PARMS   *PMCI_VD_PLAY_PARMS;
      
      typedef  struct _MCI_DGV_PLAY_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulFrom;          /* Play from this position                 */
         ULONG   ulTo;            /* Play to this position                   */
         ULONG   ulSpeed;         /* Specifies the play rate in frames per second */
      } MCI_DGV_PLAY_PARMS;
      typedef MCI_DGV_PLAY_PARMS   *PMCI_DGV_PLAY_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_PUT  message                               */
      /*     0x00000X00 are reserved for MCI_PUT  flags                    */
      /*********************************************************************/
      #define MCI_VID_PUT_RECT                    0x00000100L
      #define MCI_VID_PUT_DESTINATION             0x00000200L
      #define MCI_VID_PUT_SOURCE                  0x00000400L
      #define MCI_VID_PUT_WINDOW_MOVE             0x00000800L
      #define MCI_VID_PUT_WINDOW_SIZE             0x00001000L
      
      #define MCI_DGV_PUT_RECT                    MCI_VID_PUT_RECT
      #define MCI_DGV_PUT_DESTINATION             MCI_VID_PUT_DESTINATION
      #define MCI_DGV_PUT_SOURCE                  MCI_VID_PUT_SOURCE
      #define MCI_DGV_PUT_WINDOW_MOVE             MCI_VID_PUT_WINDOW_MOVE
      #define MCI_DGV_PUT_WINDOW_SIZE             MCI_VID_PUT_WINDOW_SIZE
      #define MCI_DGV_MONITOR                     0x00010000L
      #define MCI_DGV_RECORD                      0x00020000L
      
      #define MCI_OVLY_PUT_RECT                   MCI_VID_PUT_RECT
      #define MCI_OVLY_PUT_DESTINATION            MCI_VID_PUT_DESTINATION
      #define MCI_OVLY_PUT_SOURCE                 MCI_VID_PUT_SOURCE
      #define MCI_OVLY_PUT_WINDOW_MOVE            MCI_VID_PUT_WINDOW_MOVE
      #define MCI_OVLY_PUT_WINDOW_SIZE            MCI_VID_PUT_WINDOW_SIZE
      
      /***************************************************/
      /* Rectangle parameters for MCI_PUT, MCI_WHERE,    */
      /* MCI_FREEZE, and MCI_UNFREEZE                    */
      /***************************************************/
      
      typedef  struct _MCI_VID_RECT_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         RECTL   rc;              /* rectangle array specifying the offset   */
         /* and size of a rectangle                 */
      } MCI_VID_RECT_PARMS;
      
      typedef MCI_VID_RECT_PARMS   MCI_DGV_RECT_PARMS;
      typedef MCI_DGV_RECT_PARMS   *PMCI_DGV_RECT_PARMS;
      
      typedef MCI_VID_RECT_PARMS   MCI_OVLY_RECT_PARMS;
      typedef MCI_OVLY_RECT_PARMS  *PMCI_OVLY_RECT_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_RECORD message                             */
      /*     0x00000X00 are reserved for MCI_RECORD flags                  */
      /*********************************************************************/
      #define MCI_RECORD_INSERT                   0x00000100L
      #define MCI_RECORD_OVERWRITE                0x00000200L
      
      #define MCI_DGV_RECORD_RECT                 0x00000400L
      
      typedef struct _MCI_RECORD_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulFrom;          /* Record from position                    */
         ULONG   ulTo;            /* Record to position                      */
      } MCI_RECORD_PARMS;
      typedef MCI_RECORD_PARMS   *PMCI_RECORD_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_RELEASEDEVICE message                      */
      /*     0xXXXXXX00 are reserved for MCI_RELEASEDEVICE message         */
      /*********************************************************************/
      #define MCI_RETURN_RESOURCE                 0x00000100L
      
      /*********************************************************************/
      /* parameters for the MCI_RESTORE message                            */
      /*     0x0000XX00 are reserved for MCI_RESTORE flags                 */
      /*     MCI_CONVERT is defined by MCI_CAPTURE message as 0x00001000L  */
      /*********************************************************************/
      #define MCI_RESTORE_SRC_RECT                0x00000100L
      #define MCI_RESTORE_DEST_RECT               0x00000200L
      
      typedef struct _MCI_RESTORE_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message     */
         RECTL   SrcRect;         /* Rectangle array specifying the source       */
         /* area to be restored                         */
         RECTL   DestRect;        /* Rectangle array specifying the destination  */
         /* area to be restored                         */
      } MCI_RESTORE_PARMS;
      typedef MCI_RESTORE_PARMS   *PMCI_RESTORE_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_SAVE  message                              */
      /*     0x00000X00 are reserved for MCI_SAVE  flags                   */
      /*********************************************************************/
      #define MCI_SAVE_FILE                       0x00000100L
      
      /*************************************************/
      /* Digital Video               MCI_SAVE          */
      /*************************************************/
      
      #define MCI_DGV_SAVE_VIDEO_FILE             0x00001000L
      #define MCI_DGV_SAVE_IMAGE_FILE             0x00002000L
      
      typedef struct _MCI_SAVE_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message     */
         PSZ     pszFileName;     /* Filename to save data to                    */
      } MCI_SAVE_PARMS;
      typedef MCI_SAVE_PARMS   *PMCI_SAVE_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_SEEK message                               */
      /*     0x00000X00 are reserved for MCI_SEEK flags                    */
      /*********************************************************************/
      #define MCI_TO_START                        0x00000100L
      #define MCI_TO_END                          0x00000200L
      
      /*************************************************/
      /* Digital Video               MCI_SEEK          */
      /*************************************************/
      
      #define MCI_DGV_SEEK_TO_NEAREST_IFRAME      0x00100000L
      
      /*************************************************/
      /* Videodisc                   MCI_SEEK          */
      /*************************************************/
      #define MCI_VD_SEEK_REVERSE                 0x00001000L
      
      typedef struct _MCI_SEEK_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message     */
         ULONG   ulTo;            /* Seek to this position                       */
      } MCI_SEEK_PARMS;
      typedef MCI_SEEK_PARMS   *PMCI_SEEK_PARMS;
      
      /*********************************************************************/
      /* Flags for the MCI_SET message                                     */
      /*     0x00XXXX00 are reserved for MCI_SET  flags                    */
      /*********************************************************************/
      #define MCI_SET_ITEM                        0x00000100L
      #define MCI_SET_ON                          0x00000200L
      #define MCI_SET_OFF                         0x00000400L
      #define MCI_SET_VIDEO                       0x00000800L
      #define MCI_SET_AUDIO                       0x00001000L
      #define MCI_SET_DOOR_OPEN                   0x00002000L
      #define MCI_SET_DOOR_CLOSED                 0x00004000L
      #define MCI_SET_SPEED_FORMAT                0x00008000L
      #define MCI_SET_TIME_FORMAT                 0x00010000L
      #define MCI_SET_DOOR_LOCK                   0x00020000L
      #define MCI_SET_DOOR_UNLOCK                 0x00040000L
      #define MCI_SET_VOLUME                      0x00080000L
      #define MCI_SET_ITEM_FOURCC                 0x00100000L
      #define MCI_SET_REVERSE                     0x00200000L
      
      /*******************************************************/
      /* Values for the ulAudio field                        */
      /*******************************************************/
      #define MCI_SET_AUDIO_ALL                   0x00000000L
      #define MCI_SET_AUDIO_LEFT                  0x00000001L
      #define MCI_SET_AUDIO_RIGHT                 0x00000002L
      
      /*******************************************************/
      /* MCI_SET flags for amp/mixer                         */
      /*******************************************************/
      #define MCI_AMP_SET_BALANCE                 0x00100000L
      #define MCI_AMP_SET_PITCH                   0x00200000L
      #define MCI_AMP_SET_TREBLE                  0x00400000L
      #define MCI_AMP_SET_BASS                    0x00800000L
      #define MCI_AMP_SET_GAIN                    0x01000000L
      
      #define MCI_AMP_SET_ALC                     0x00000004L
      #define MCI_AMP_SET_CROSSOVER               0x00000010L
      #define MCI_AMP_SET_LOUDNESS                0x00000020L
      #define MCI_AMP_SET_MUTE                    0x00000040L
      #define MCI_AMP_SET_REVERB                  0x00000080L
      #define MCI_AMP_SET_STEREOENHANCE           0x00000100L
      #define MCI_AMP_SET_CUSTOM1                 0x00000200L
      #define MCI_AMP_SET_CUSTOM2                 0x00000400L
      #define MCI_AMP_SET_CUSTOM3                 0x00000800L
      #define MCI_AMP_SET_LRVOLUME                0x00001000L
      #define MCI_AMP_SET_MID                     0x00020000L
      #define MCI_AMP_SET_CHORUS                  0x00004000L
      #define MCI_AMP_SET_VOLUME                  MCI_SET_VOLUME
      
      /*******************************************************/
      /* General MCI_SET item values                         */
      /*******************************************************/
      #define MCI_SET_MONITOR                     0x00000001L
      #define MCI_SET_MONITOR_AUDIO               0x00000002L
      #define MCI_SET_MONITOR_VIDEO               0x00000003L
      #define MCI_SET_RECORD                      0x00000004L
      
      /*******************************************************/
      /* MCI_SET item values for amp/mixer                   */
      /*******************************************************/
      #define MCI_AMP_SET_MONITOR                 MCI_SET_MONITOR_AUDIO
      #define MCI_AMP_SET_AUDIO                   0x00000008L
      
      
      /*******************************************************/
      /* MCI_SET flags for CDXA                              */
      /*******************************************************/
      #define MCI_CDXA_SET_CHANNEL                0x01000000L
      #define MCI_CDXA_AUDIO_DEVICE               0x10000000L
      #define MCI_CDXA_AUDIO_BUFFER               0x02000000L
      #define MCI_CDXA_VIDEO_BUFFER               0x04000000L
      #define MCI_CDXA_DATA_BUFFER                0x08000000L
      
      /*******************************************************/
      /* MCI_SET flags for videodisc                         */
      /*******************************************************/
      #define MCI_VD_SET_ON                       MCI_SET_ON
      #define MCI_VD_SET_OFF                      MCI_SET_OFF
      #define MCI_VD_SET_VIDEO                    MCI_SET_VIDEO
      #define MCI_VD_SET_CHANNEL                  0x00100000L
      #define MCI_VD_SET_DISPLAY                  0x00200000L
      
      /*******************************************************/
      /* Generic MCI_SET item values for video devices       */
      /*******************************************************/
      #define MCI_VID_SET_VIDEO_COMPRESSION           MCI_VID_ITEM_BASE
      #define MCI_VID_SET_IMAGE_COMPRESSION           (MCI_VID_ITEM_BASE+1)
      #define MCI_VID_SET_IMAGE_BITSPERPEL            (MCI_VID_ITEM_BASE+2)
      #define MCI_VID_SET_IMAGE_PELFORMAT             (MCI_VID_ITEM_BASE+3)
      #define MCI_VID_SET_BRIGHTNESS                  (MCI_VID_ITEM_BASE+4)
      #define MCI_VID_SET_CONTRAST                    (MCI_VID_ITEM_BASE+5)
      #define MCI_VID_SET_HUE                         (MCI_VID_ITEM_BASE+6)
      #define MCI_VID_SET_SATURATION                  (MCI_VID_ITEM_BASE+7)
      #define MCI_VID_SET_SHARPNESS                   (MCI_VID_ITEM_BASE+8)
      #define MCI_VID_SET_GREYSCALE                   (MCI_VID_ITEM_BASE+9)
      #define MCI_VID_SET_IMAGE_QUALITY               (MCI_VID_ITEM_BASE+10)
      #define MCI_VID_SET_VIDEO_QUALITY               (MCI_VID_ITEM_BASE+11)
      #define MCI_VID_SET_IMAGE_COMPRESSION_METHOD    (MCI_VID_ITEM_BASE+12)
      #define MCI_VID_SET_MINIMUM_VIDEO_REFRESH_RATE  (MCI_VID_ITEM_BASE+13)
      #define MCI_VID_SET_IMAGE_FILE_FORMAT           (MCI_VID_ITEM_BASE+14)
      #define MCI_VID_SET_REF_INTERVAL                (MCI_VID_ITEM_BASE+15)
      #define MCI_VID_SET_MAXDATARATE                 (MCI_VID_ITEM_BASE+16)
      #define MCI_VID_SET_VIDEO_FILE_FORMAT           (MCI_VID_ITEM_BASE+17)
      
      /*******************************************************/
      /* MCI_SET item values for video overlay               */
      /*******************************************************/
      #define MCI_OVLY_SET_IMAGE_COMPRESSION          MCI_VID_SET_IMAGE_COMPRESSION
      #define MCI_OVLY_SET_IMAGE_BITSPERPEL           MCI_VID_SET_IMAGE_BITSPERPEL
      #define MCI_OVLY_SET_IMAGE_PELFORMAT            MCI_VID_SET_IMAGE_PELFORMAT
      #define MCI_OVLY_SET_BRIGHTNESS                 MCI_VID_SET_BRIGHTNESS
      #define MCI_OVLY_SET_CONTRAST                   MCI_VID_SET_CONTRAST
      #define MCI_OVLY_SET_HUE                        MCI_VID_SET_HUE
      #define MCI_OVLY_SET_SATURATION                 MCI_VID_SET_SATURATION
      #define MCI_OVLY_SET_SHARPNESS                  MCI_VID_SET_SHARPNESS
      #define MCI_OVLY_SET_GREYSCALE                  MCI_VID_SET_GREYSCALE
      #define MCI_OVLY_SET_IMAGE_QUALITY              MCI_VID_SET_IMAGE_QUALITY
      #define MCI_OVLY_SET_IMAGE_COMPRESSION_METHOD   MCI_VID_SET_IMAGE_COMPRESSION_METHOD
      #define MCI_OVLY_SET_MINIMUM_VIDEO_REFRESH_RATE MCI_VID_SET_MINIMUM_VIDEO_REFRESH_RATE
      #define MCI_OVLY_SET_IMAGE_FILE_FORMAT          MCI_VID_SET_IMAGE_FILE_FORMAT
      
      /*******************************************************/
      /* MCI_SET item values for digital video               */
      /*******************************************************/
      #define MCI_DGV_SET_VIDEO_COMPRESSION           MCI_VID_SET_VIDEO_COMPRESSION
      #define MCI_DGV_SET_IMAGE_COMPRESSION           MCI_VID_SET_IMAGE_COMPRESSION
      #define MCI_DGV_SET_IMAGE_BITSPERPEL            MCI_VID_SET_IMAGE_BITSPERPEL
      #define MCI_DGV_SET_IMAGE_PELFORMAT             MCI_VID_SET_IMAGE_PELFORMAT
      #define MCI_DGV_SET_BRIGHTNESS                  MCI_VID_SET_BRIGHTNESS
      #define MCI_DGV_SET_CONTRAST                    MCI_VID_SET_CONTRAST
      #define MCI_DGV_SET_HUE                         MCI_VID_SET_HUE
      #define MCI_DGV_SET_SATURATION                  MCI_VID_SET_SATURATION
      #define MCI_DGV_SET_SHARPNESS                   MCI_VID_SET_SHARPNESS
      #define MCI_DGV_SET_GREYSCALE                   MCI_VID_SET_GREYSCALE
      #define MCI_DGV_SET_IMAGE_QUALITY               MCI_VID_SET_IMAGE_QUALITY
      #define MCI_DGV_SET_VIDEO_QUALITY               MCI_VID_SET_VIDEO_QUALITY
      #define MCI_DGV_SET_IMAGE_COMPRESSION_METHOD    MCI_VID_SET_IMAGE_COMPRESSION_METHOD
      #define MCI_DGV_SET_MONITOR                     MCI_SET_MONITOR
      #define MCI_DGV_SET_MINIMUM_VIDEO_REFRESH_RATE  MCI_VID_SET_MINIMUM_VIDEO_REFRESH_RATE
      #define MCI_DGV_SET_IMAGE_FILE_FORMAT           MCI_VID_SET_IMAGE_FILE_FORMAT
      #define MCI_DGV_SET_REF_INTERVAL                MCI_VID_SET_REF_INTERVAL
      #define MCI_DGV_SET_MAXDATARATE                 MCI_VID_SET_MAXDATARATE
      #define MCI_DGV_SET_VIDEO_FILE_FORMAT           MCI_VID_SET_VIDEO_FILE_FORMAT
      
      #define MCI_DGV_SET_AUDIO_COMPRESSION           MCI_DGV_ITEM_BASE
      #define MCI_DGV_SET_AUDIO_QUALITY               (MCI_DGV_ITEM_BASE+1)
      #define MCI_DGV_SET_CHANNELS                    (MCI_DGV_ITEM_BASE+2)
      #define MCI_DGV_SET_BITSPERSAMPLE               (MCI_DGV_ITEM_BASE+3)
      #define MCI_DGV_SET_SAMPLESPERSEC               (MCI_DGV_ITEM_BASE+4)
      #define MCI_DGV_SET_FORMATTAG                   (MCI_DGV_ITEM_BASE+5)
      #define MCI_DGV_SET_BLOCKALIGN                  (MCI_DGV_ITEM_BASE+6)
      #define MCI_DGV_SET_AVGBYTESPERSEC              (MCI_DGV_ITEM_BASE+7)
      
      #define MCI_DGV_SET_VIDEO_COMPRESSION_SUBTYPE   (MCI_DGV_ITEM_BASE+8)
      #define MCI_DGV_SET_VIDEO_RECORD_RATE           (MCI_DGV_ITEM_BASE+9)
      #define MCI_DGV_SET_VIDEO_RECORD_FRAME_DURATION (MCI_DGV_ITEM_BASE+10)
      #define MCI_DGV_SET_RECORD_AUDIO                (MCI_DGV_ITEM_BASE+11)
      #define MCI_DGV_SET_TRANSPARENT_COLOR           (MCI_DGV_ITEM_BASE+12)
      #define MCI_DGV_SET_GRAPHIC_TRANSPARENT_COLOR   (MCI_DGV_ITEM_BASE+12)
      #define MCI_DGV_SET_AUDIOSYNC                   (MCI_DGV_ITEM_BASE+13)
      #define MCI_DGV_SET_VIDEO_TRANSPARENT_COLOR     (MCI_DGV_ITEM_BASE+14)
      
      /*******************************************************/
      /* Video file format                                   */
      /*******************************************************/
      #define MCI_VID_FILE_FORMAT_AVI       0x20495641L
      
      /*******************************************************/
      /* Audio/video/image compression types                 */
      /*******************************************************/
      #define MCI_AUD_COMP_ADPCM4          0x00000001L
      #define MCI_AUD_COMP_PCM8            0x00000002L
      
      #define MCI_VID_COMP_RTV_2_0         0x00000101L
      #define MCI_VID_COMP_RTV_2_1         0x00000102L
      #define MCI_VID_COMP_ULTI            0x49544C55L
      #define MCI_VID_COMP_RT21            0x31325452L
      #define MCI_VID_COMP_NONE            0x20424944L   /* This is 'DIB ' now.    */
      
      #define MCI_IMG_COMP_NONE            0x00000201L
      #define MCI_IMG_COMP_PIC9_1_0        0x00000202L
      #define MCI_IMG_COMP_PIC16_1_0       0x00000203L
      #define MCI_IMG_COMP_JPEG9           0x00000204L
      #define MCI_IMG_COMP_JPEG9_1_0       MCI_IMG_COMP_JPEG9
      #define MCI_IMG_COMP_BMP_RLE4        0x00000205L
      #define MCI_IMG_COMP_BMP_RLE8        0x00000206L
      #define MCI_IMG_COMP_JPEGN           0x00000207L
      
      /*******************************************************/
      /* PEL Formats (video/image)                           */
      /*******************************************************/
      #define MCI_VID_PALETTE              0x00000001L
      #define MCI_VID_RGB                  0x00000002L
      #define MCI_VID_YUV                  0x00000003L
      
      #define MCI_IMG_PALETTE              MCI_VID_PALETTE
      #define MCI_IMG_RGB                  MCI_VID_RGB
      #define MCI_IMG_YUV                  MCI_VID_YUV
      
      
      /*******************************************************/
      /* Audio/video/image quality levels                    */
      /*******************************************************/
      #define MCI_AUD_QUALITY_HIGH         0x00000001L
      #define MCI_AUD_QUALITY_MED          0x00000002L
      #define MCI_AUD_QUALITY_LOW          0x00000003L
      
      #define MCI_VID_QUALITY_HIGH         0x00000101L
      #define MCI_VID_QUALITY_MED          0x00000102L
      #define MCI_VID_QUALITY_LOW          0x00000103L
      #define MCI_VID_QUALITY_BEST         MCI_VID_QUALITY_HIGH
      #define MCI_VID_QUALITY_CDROM        MCI_VID_QUALITY_MED
      #define MCI_VID_QUALITY_COMPACT      MCI_VID_QUALITY_LOW
      #define MCI_VID_QUALITY_BETTER       MCI_VID_QUALITY_MED
      #define MCI_VID_QUALITY_GOOD         MCI_VID_QUALITY_LOW
      
      #define MCI_IMG_QUALITY_HIGH         0x00000201L
      #define MCI_IMG_QUALITY_MED          0x00000202L
      #define MCI_IMG_QUALITY_LOW          0x00000203L
      
      /*******************************************************/
      /* MCI_SET flags for wave audio                        */
      /*******************************************************/
      #define MCI_WAVE_SET_SAMPLESPERSEC          0x01000000L
      #define MCI_WAVE_SET_AVGBYTESPERSEC         0x02000000L
      #define MCI_WAVE_SET_BLOCKALIGN             0x04000000L
      #define MCI_WAVE_SET_FORMATTAG              0x08000000L
      #define MCI_WAVE_SET_CHANNELS               0x10000000L
      #define MCI_WAVE_SET_BITSPERSAMPLE          0x80000000L
      
      /*******************************************************/
      /* Wave format tag defines                             */
      /*******************************************************/
      #define MCI_WAVE_FORMAT_PCM                 DATATYPE_WAVEFORM
      #define MCI_WAVE_FORMAT_ADPCM               0x0002L
      #define MCI_WAVE_FORMAT_IBM_CVSD            0x0005L
      #define MCI_WAVE_FORMAT_ALAW                DATATYPE_RIFF_ALAW
      #define MCI_WAVE_FORMAT_MULAW               DATATYPE_RIFF_MULAW
      #define MCI_WAVE_FORMAT_OKI_ADPCM           0x0010L
      #define MCI_WAVE_FORMAT_DVI_ADPCM           0x0011L
      #define MCI_WAVE_FORMAT_DIGISTD             0x0015L
      #define MCI_WAVE_FORMAT_DIGIFIX             0x0016L
      #define MCI_WAVE_FORMAT_AVC_ADPCM           DATATYPE_ADPCM_AVC
      #define MCI_WAVE_FORMAT_IBM_ADPCM           DATATYPE_ADPCM_AVC
      #define MCI_WAVE_FORMAT_IBM_MULAW           DATATYPE_MULAW
      #define MCI_WAVE_FORMAT_IBM_ALAW            DATATYPE_ALAW
      #define MCI_WAVE_FORMAT_CT_ADPCM            DATATYPE_CT_ADPCM
      #define MCI_WAVE_FORMAT_MPEG1               DATATYPE_MPEG1AUDIO
      
      /*******************************************************/
      /* MCI_SET flags for sequencer                         */
      /*******************************************************/
      #define MCI_SEQ_SET_TEMPO                   0x00100000L
      #define MCI_SEQ_SET_PORT                    0x00200000L
      #define MCI_SEQ_ENABLE_PORT_MAPPER          0x00400000L
      #define MCI_SEQ_DISABLE_PORT_MAPPER         0x00800000L
      #define MCI_SEQ_SET_OFFSET                  0x01000000L
      #define MCI_SEQ_SET_MASTER                  0x02000000L
      #define MCI_SEQ_SET_SLAVE                   0x04000000L
      
      /**********************************************/
      /* SEQ time formats                           */
      /**********************************************/
      #define MCI_SEQ_SET_SMPTE_24             0x00000100L
      #define MCI_SEQ_SET_SMPTE_25             0x00000200L
      #define MCI_SEQ_SET_SMPTE_30             0x00000300L
      #define MCI_SEQ_SET_SMPTE_30DROP         0x00000400L
      #define MCI_SEQ_SET_SONGPTR              0x00000500L
      
      /**********************************************/
      /* SEQ synchronization types                  */
      /**********************************************/
      #define MCI_SEQ_MIDI                     0x00000001L
      #define MCI_SEQ_SMPTE                    0x00000002L
      #define MCI_SEQ_FILE                     0x00000003L
      #define MCI_SEQ_NONE                     0x00000004L
      
      /**********************************************/
      /* SEQ PORT TYPES                             */
      /**********************************************/
      #define MCI_SET_NONE                     0x10000000L
      #define MIDI_MAPPER                      0x20000000L
      #define MCI_MIDI_MAPPER                  MIDI_MAPPER
      
      typedef struct _MCI_SET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulTimeFormat;    /* Time format to be used by the device           */
         ULONG   ulSpeedFormat;   /* Speed format used by this device               */
         ULONG   ulAudio;         /* Channel number for this operation              */
         /* (MCI_SET_AUDIO_LEFT, MCI_SET_AUDIO_RIGHT,      */
         /*  MCI_SET_AUDIO_ALL)                            */
         ULONG   ulLevel;         /* Volume, treble or bass level as % of max.      */
         ULONG   ulOver;          /* Delay time for vectored change in milliseconds */
         ULONG   ulItem;          /* Item field for set item flags                  */
         ULONG   ulValue;         /* Value associated with item flag                */
      } MCI_SET_PARMS;
      typedef MCI_SET_PARMS       *PMCI_SET_PARMS;
      
      typedef MCI_SET_PARMS       MCI_AMP_SET_PARMS;
      typedef MCI_AMP_SET_PARMS   *PMCI_AMP_SET_PARMS;
      
      typedef MCI_SET_PARMS       MCI_DGV_SET_PARMS;
      typedef MCI_DGV_SET_PARMS   *PMCI_DGV_SET_PARMS;
      
      typedef MCI_SET_PARMS       MCI_OVLY_SET_PARMS;
      typedef MCI_OVLY_SET_PARMS  *PMCI_OVLY_SET_PARMS;
      
      typedef struct _MCI_CDXA_SET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulTimeFormat;    /* Time format to be used by the device           */
         ULONG   ulSpeedFormat;   /* Speed format used by this device               */
         ULONG   ulAudio;         /* Channel number for this operation              */
         /* (MCI_SET_AUDIO_LEFT, MCI_SET_AUDIO_RIGHT,      */
         /*  MCI_SET_AUDIO_ALL)                            */
         ULONG   ulLevel;         /* Volume, treble or bass level as % of max.      */
         ULONG   ulOver;          /* Delay time for vectored change in milliseconds */
         ULONG   ulItem;          /* Item field for set item flags                  */
         ULONG   ulValue;         /* Value associated with item flag                */
         ULONG   ulChannel;       /* Channel number                                 */
         PVOID   pPlayList;       /* Pointer to play list                           */
         ULONG   ulPlayListSize;  /* Play list size                                 */
      } MCI_CDXA_SET_PARMS;
      typedef MCI_CDXA_SET_PARMS   *PMCI_CDXA_SET_PARMS;
      
      typedef struct _MCI_VD_SET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulTimeFormat;    /* Time format to be used by the device           */
         ULONG   ulSpeedFormat;   /* Speed format used by this device               */
         ULONG   ulAudio;         /* Channel number for this operation              */
         /* (MCI_SET_AUDIO_LEFT, MCI_SET_AUDIO_RIGHT,      */
         /*  MCI_SET_AUDIO_ALL)                            */
         ULONG   ulLevel;         /* Volume, treble or bass level as % of max.      */
         ULONG   ulOver;          /* Delay time for vectored change in milliseconds */
         ULONG   ulItem;          /* Item field for set item flags                  */
         ULONG   ulValue;         /* Value associated with item flag                */
         ULONG   ulChannel;       /* Videodisc channel                              */
      } MCI_VD_SET_PARMS;
      typedef MCI_VD_SET_PARMS   *PMCI_VD_SET_PARMS;
      
      typedef struct _MCI_WAVE_SET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulTimeFormat;    /* Time format to be used by the device           */
         ULONG   ulSpeedFormat;   /* Speed format used by this device               */
         ULONG   ulAudio;         /* Channel number for this operation              */
         /* (MCI_SET_AUDIO_LEFT, MCI_SET_AUDIO_RIGHT,      */
         /*  MCI_SET_AUDIO_ALL)                            */
         ULONG   ulLevel;         /* Volume, trebble or bass level as % of max.     */
         ULONG   ulOver;          /* Delay time for vectored change in milliseconds */
         ULONG   ulItem;          /* Item field for set item flags                  */
         ULONG   ulValue;         /* Value associated with item flag                */
         USHORT  usInput;         /* Channel for input                              */
         USHORT  usReserved0;     /* Reserved field                                 */
         USHORT  usOutput;        /* Channel for output                             */
         USHORT  usReserved1;     /* Reserved field                                 */
         USHORT  usFormatTag;     /* Format tag                                     */
         USHORT  usReserved2;     /* Reserved field                                 */
         USHORT  usChannels;      /* mono(1) or stereo(2)                           */
         USHORT  usReserved3;     /* Reserved field                                 */
         ULONG   ulSamplesPerSec; /* Samples per seconds                            */
         ULONG   ulAvgBytesPerSec;/* Bytes per seconds                              */
         USHORT  usBlockAlign;    /* Block alignment of data                        */
         USHORT  usReserved4;     /* Reserved field                                 */
         USHORT  usBitsPerSample; /* Bits per seconds                               */
         USHORT  usReserved5;     /* Reserved field                                 */
      } MCI_WAVE_SET_PARMS;
      typedef MCI_WAVE_SET_PARMS   *PMCI_WAVE_SET_PARMS;
      
      typedef struct _MCI_SEQ_SET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulTimeFormat;    /* Time format to be used by the device           */
         ULONG   ulSpeedFormat;   /* Speed format used by this device               */
         ULONG   ulAudio;         /* Channel number for this operation              */
         /* (MCI_SET_AUDIO_LEFT, MCI_SET_AUDIO_RIGHT,      */
         /*  MCI_SET_AUDIO_ALL)                            */
         ULONG   ulLevel;         /* Volume, trebble or bass level as % of max.     */
         ULONG   ulOver;          /* Delay time for vectored change in milliseconds */
         ULONG   ulItem;          /* Item field for set item flags                  */
         ULONG   ulValue;         /* Value associated with item flag                */
         ULONG   ulTempo;         /* Specified the tempo                            */
         ULONG   ulPort;          /* Output port                                    */
         ULONG   ulSlave;         /* Unused field                                   */
         ULONG   ulMaster;        /* Unused field                                   */
         ULONG   ulOffset;        /* Specified the data offset                      */
      } MCI_SEQ_SET_PARMS;
      typedef MCI_SEQ_SET_PARMS   *PMCI_SEQ_SET_PARMS;
      
      
      /*********************************************************************/
      /* parameters for the MCI_SET_CUEPOINT message                       */
      /*     0x00000x00 are reserved for MCI_SET_CUEPOINT flags            */
      /*********************************************************************/
      #define MCI_SET_CUEPOINT_ON                 0x00000100L
      #define MCI_SET_CUEPOINT_OFF                0x00000200L
      
      typedef struct _MCI_CUEPOINT_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulCuepoint;      /* Specifies the cuepoint location                */
         USHORT  usUserParm;      /* User parmameter returned on cuepoint           */
         /* notify message                                 */
         USHORT  usReserved0;     /* Reserved field                                 */
      } MCI_CUEPOINT_PARMS;
      typedef MCI_CUEPOINT_PARMS   *PMCI_CUEPOINT_PARMS;
      
      /*********************************************************************/
      /* parameters for the MCI_SETIMAGEPALETTE message                    */
      /*     0x000XXX00 are reserved for MCI_SETIMAGEPALETTE flags         */
      /*********************************************************************/
      #define MCI_SET_REGISTERED                  0x00000100L
      
      /*********************************************************************/
      /* flags and parameter structure for the MCI_SET_POSITION_ADVISE msg */
      /*     0x00000X00 are reserved for MCI_SET_POSITION_ADVISE flags     */
      /*********************************************************************/
      #define MCI_SET_POSITION_ADVISE_ON          0x00000100L
      #define MCI_SET_POSITION_ADVISE_OFF         0x00000200L
      
      typedef struct _MCI_POSITION_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulUnits;         /* Specifies position change notifiication        */
         /* granularity                                    */
         USHORT  usUserParm;      /* User parameter returned on position change     */
         /* notification message.                          */
         USHORT  Reserved0;       /* Reserved field                                 */
         ULONG   Reserved1;       /* Reserved field                                 */
      } MCI_POSITION_PARMS;
      typedef MCI_POSITION_PARMS   *PMCI_POSITION_PARMS;
      
      /*********************************************************************/
      /* parameter structure for the MCI_SET_SYNC_OFFSET message           */
      /*********************************************************************/
      typedef struct _MCI_SYNC_OFFSET_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message        */
         ULONG   ulOffset;        /* Specifies the device media position offset     */
         /* in the currently specified device units.       */
      } MCI_SYNC_OFFSET_PARMS;
      typedef MCI_SYNC_OFFSET_PARMS   *PMCI_SYNC_OFFSET_PARMS;
      
      /*********************************************************************/
      /* flags for the MCI_SPIN message                                    */
      /*     0x00000X00 are reserved for MCI_SPIN flags                    */
      /*********************************************************************/
      #define MCI_SPIN_UP                         0x00000100L
      #define MCI_SPIN_DOWN                       0x00000200L
      
      /*********************************************************************/
      /* MCI_STATUS message flags                                          */
      /*     0x000XXX00 are reserved for MCI_STATUS flags                  */
      /*     MCI_VOLUME and MCI_VOLUME_DELAY are 0x00000100 and 0x00000200 */
      /*********************************************************************/
      #define MCI_STATUS_ITEM                     0x00000100L
      #define MCI_STATUS_START                    0x00000200L
      #define MCI_STATUS_CONNECTOR                0x00000400L
      
      /*************************************************/
      /* General MCI_STATUS item values                */
      /*************************************************/
      #define MCI_STATUS_CURRENT_TRACK            0x00000001L
      #define MCI_STATUS_LENGTH                   0x00000002L
      #define MCI_STATUS_MODE                     0x00000003L
      #define MCI_STATUS_NUMBER_OF_TRACKS         0x00000004L
      #define MCI_STATUS_POSITION                 0x00000005L
      #define MCI_STATUS_POSITION_IN_TRACK        0x00000006L
      #define MCI_STATUS_MEDIA_PRESENT            0x00000007L
      #define MCI_STATUS_VOLUME                   0x00000008L
      #define MCI_STATUS_READY                    0x00000009L
      #define MCI_STATUS_TIME_FORMAT              0x0000000AL
      #define MCI_STATUS_SPEED_FORMAT             0x0000000BL
      #define MCI_STATUS_MONITOR                  0x0000000CL
      #define MCI_STATUS_AUDIO                    0x0000000DL
      #define MCI_STATUS_VIDEO                    0x0000000EL
      #define MCI_STATUS_CLIPBOARD                0x0000000FL
      #define MCI_STATUS_CAN_PASTE                0x00000010L
      #define MCI_STATUS_CAN_REDO                 0x00000020L
      #define MCI_STATUS_CAN_UNDO                 0x00000030L
      
      /*************************************************/
      /* ulValue field values for MCI_STATUS_AUDIO     */
      /*************************************************/
      #define MCI_STATUS_AUDIO_ALL                   0x00000000L
      #define MCI_STATUS_AUDIO_LEFT                  0x00000001L
      #define MCI_STATUS_AUDIO_RIGHT                 0x00000002L
      
      /*************************************************/
      /* MCI_STATUS item values for amp/mixer          */
      /*************************************************/
      #define MCI_AMP_STATUS_PITCH                MCI_AMP_ITEM_BASE
      #define MCI_AMP_STATUS_TREBLE               (MCI_AMP_ITEM_BASE+1)
      #define MCI_AMP_STATUS_BASS                 (MCI_AMP_ITEM_BASE+2)
      #define MCI_AMP_STATUS_BALANCE              (MCI_AMP_ITEM_BASE+3)
      #define MCI_AMP_STATUS_GAIN                 (MCI_AMP_ITEM_BASE+4)
      #define MCI_AMP_STATUS_MONITOR              (MCI_AMP_ITEM_BASE+5)
      #define MCI_AMP_STATUS_MID                  (MCI_AMP_ITEM_BASE+6)
      #define MCI_AMP_STATUS_VOLUME               MCI_STATUS_VOLUME
      #define MCI_AMP_STATUS_LOUDNESS             (MCI_AMP_ITEM_BASE+7)
      #define MCI_AMP_STATUS_CROSSOVER            (MCI_AMP_ITEM_BASE+8)
      #define MCI_AMP_STATUS_REVERB               (MCI_AMP_ITEM_BASE+9)
      #define MCI_AMP_STATUS_ALC                  (MCI_AMP_ITEM_BASE+10)
      #define MCI_AMP_STATUS_CHORUS               (MCI_AMP_ITEM_BASE+11)
      #define MCI_AMP_STATUS_CUSTOM1              (MCI_AMP_ITEM_BASE+12)
      #define MCI_AMP_STATUS_CUSTOM2              (MCI_AMP_ITEM_BASE+13)
      #define MCI_AMP_STATUS_CUSTOM3              (MCI_AMP_ITEM_BASE+14)
      #define MCI_AMP_STATUS_MUTE                 (MCI_AMP_ITEM_BASE+15)
      #define MCI_AMP_STATUS_STEREOENHANCE        (MCI_AMP_ITEM_BASE+16)
      
      
      /*************************************************/
      /* MCI_STATUS item values for cd audio           */
      /*************************************************/
      #define MCI_CD_STATUS_TRACK_TYPE            MCI_CD_ITEM_BASE
      #define MCI_CD_STATUS_TRACK_COPYPERMITTED   (MCI_CD_ITEM_BASE+1)
      #define MCI_CD_STATUS_TRACK_CHANNELS        (MCI_CD_ITEM_BASE+2)
      #define MCI_CD_STATUS_TRACK_PREEMPHASIS     (MCI_CD_ITEM_BASE+3)
      
      /************************************************/
      /* return values for CD MCI_STATUS message with */
      /*   MCI_CD_STATUS_TRACK_TYPE set               */
      /************************************************/
      #define MCI_CD_TRACK_AUDIO                  0x00000001L
      #define MCI_CD_TRACK_DATA                   0x00000002L
      #define MCI_CD_TRACK_OTHER                  0x00000003L
      
      /*************************************************/
      /* MCI_STATUS item values for CDXA               */
      /*************************************************/
      #define MCI_CDXA_STATUS_CHANNEL             MCI_CDXA_ITEM_BASE
      
      /************************************************/
      /*returned from call for MCI_CDXA_STATUS_CHANNEL*/
      /************************************************/
      #define MCI_CDXA_NONE                       0x00000000L
      
      
      /*************************************************/
      /* MCI_STATUS item values for sequencer          */
      /*************************************************/
      #define MCI_SEQ_STATUS_TEMPO                MCI_SEQ_ITEM_BASE
      #define MCI_SEQ_STATUS_OFFSET               (MCI_SEQ_ITEM_BASE+1)
      #define MCI_SEQ_STATUS_DIVTYPE              (MCI_SEQ_ITEM_BASE+2)
      #define MCI_SEQ_STATUS_MASTER               (MCI_SEQ_ITEM_BASE+3)
      #define MCI_SEQ_STATUS_PORT                 (MCI_SEQ_ITEM_BASE+4)
      #define MCI_SEQ_STATUS_SLAVE                (MCI_SEQ_ITEM_BASE+5)
      
      /**********************************************/
      /* Return value for current division type     */
      /**********************************************/
      #define MCI_SEQ_DIV_PPQN                    0x00000001L
      #define MCI_SEQ_DIV_SMPTE_24                0x00000002L
      #define MCI_SEQ_DIV_SMPTE_25                0x00000003L
      #define MCI_SEQ_DIV_SMPTE_30DROP            0x00000004L
      #define MCI_SEQ_DIV_SMPTE_30                0x00000005L
      
      /*************************************************/
      /* MCI_STATUS items for videodisc                */
      /*************************************************/
      #define MCI_VD_STATUS_SPEED                 MCI_VD_ITEM_BASE
      #define MCI_VD_STATUS_FORWARD               (MCI_VD_ITEM_BASE+1)
      #define MCI_VD_MEDIA_TYPE                   (MCI_VD_ITEM_BASE+2)
      #define MCI_VD_STATUS_SIDE                  (MCI_VD_ITEM_BASE+3)
      #define MCI_VD_STATUS_DISC_SIZE             (MCI_VD_ITEM_BASE+4)
      
      /***********************************************/
      /* return values for videodisc status command  */
      /***********************************************/
      #define MCI_VD_SPEED_NORMAL                 0x00000000L
      #define MCI_VD_SPEED_FAST                   0x00000001L
      #define MCI_VD_SPEED_SLOW                   0x00000002L
      
      /*************************************************/
      /* MCI_STATUS items for wave audio               */
      /*************************************************/
      #define MCI_WAVE_STATUS_LEVEL               MCI_WAVE_ITEM_BASE
      #define MCI_WAVE_STATUS_SAMPLESPERSEC       (MCI_WAVE_ITEM_BASE+1)
      #define MCI_WAVE_STATUS_AVGBYTESPERSEC      (MCI_WAVE_ITEM_BASE+2)
      #define MCI_WAVE_STATUS_BLOCKALIGN          (MCI_WAVE_ITEM_BASE+3)
      #define MCI_WAVE_STATUS_FORMATTAG           (MCI_WAVE_ITEM_BASE+4)
      #define MCI_WAVE_STATUS_CHANNELS            (MCI_WAVE_ITEM_BASE+5)
      #define MCI_WAVE_STATUS_BITSPERSAMPLE       (MCI_WAVE_ITEM_BASE+6)
      
      /*************************************************/
      /* Common video MCI_STATUS items                 */
      /*************************************************/
      #define MCI_VID_STATUS_HWND                 MCI_VID_ITEM_BASE
      #define MCI_VID_STATUS_AUDIO_COMPRESSION    (MCI_VID_ITEM_BASE+1)
      #define MCI_VID_STATUS_VIDEO_COMPRESSION    (MCI_VID_ITEM_BASE+2)
      #define MCI_VID_STATUS_IMAGE_COMPRESSION    (MCI_VID_ITEM_BASE+3)
      #define MCI_VID_STATUS_AUDIO_QUALITY        (MCI_VID_ITEM_BASE+4)
      #define MCI_VID_STATUS_VIDEO_QUALITY        (MCI_VID_ITEM_BASE+5)
      #define MCI_VID_STATUS_IMAGE_QUALITY        (MCI_VID_ITEM_BASE+6)
      #define MCI_VID_STATUS_IMAGE_BITSPERPEL     (MCI_VID_ITEM_BASE+7)
      #define MCI_VID_STATUS_IMAGE_PELFORMAT      (MCI_VID_ITEM_BASE+8)
      #define MCI_VID_STATUS_FORWARD              (MCI_VID_ITEM_BASE+9)
      #define MCI_VID_STATUS_NORMAL_RATE          (MCI_VID_ITEM_BASE+10)
      #define MCI_VID_STATUS_VIDEO_X_EXTENT       (MCI_VID_ITEM_BASE+11)
      #define MCI_VID_STATUS_VIDEO_Y_EXTENT       (MCI_VID_ITEM_BASE+12)
      #define MCI_VID_STATUS_IMAGE_X_EXTENT       (MCI_VID_ITEM_BASE+13)
      #define MCI_VID_STATUS_IMAGE_Y_EXTENT       (MCI_VID_ITEM_BASE+14)
      #define MCI_VID_STATUS_BRIGHTNESS           (MCI_VID_ITEM_BASE+15)
      #define MCI_VID_STATUS_CONTRAST             (MCI_VID_ITEM_BASE+16)
      #define MCI_VID_STATUS_HUE                  (MCI_VID_ITEM_BASE+17)
      #define MCI_VID_STATUS_SATURATION           (MCI_VID_ITEM_BASE+18)
      #define MCI_VID_STATUS_GREYSCALE            (MCI_VID_ITEM_BASE+19)
      #define MCI_VID_STATUS_SHARPNESS            (MCI_VID_ITEM_BASE+20)
      #define MCI_VID_STATUS_SPEED                (MCI_VID_ITEM_BASE+21)
      #define MCI_VID_STATUS_IMAGE_FILE_FORMAT    (MCI_VID_ITEM_BASE+22)
      #define MCI_VID_STATUS_TRANSPARENT_TYPE     (MCI_VID_ITEM_BASE+23)
      #define MCI_VID_STATUS_REF_INTERVAL         (MCI_VID_ITEM_BASE+24)
      #define MCI_VID_STATUS_MAXDATARATE          (MCI_VID_ITEM_BASE+25)
      #define MCI_VID_STATUS_VIDEO_FILE_FORMAT    (MCI_VID_ITEM_BASE+26)
      
      /*************************************************/
      /* Status Transparent Type returns               */
      /* MCI_VID_PALETTE                               */
      /* MCI_VID_RGB                                   */
      /* MCI_VID_YUV                                   */
      /*************************************************/
      #define MCI_VID_STATUS_TRANSPARENT_COLOR    (MCI_VID_ITEM_BASE+24)
      
      /*************************************************/
      /* MCI_STATUS items for digital video            */
      /*************************************************/
      #define MCI_DGV_STATUS_HWND                 MCI_VID_STATUS_HWND
      #define MCI_DGV_STATUS_AUDIO_COMPRESSION    MCI_VID_STATUS_AUDIO_COMPRESSION
      #define MCI_DGV_STATUS_VIDEO_COMPRESSION    MCI_VID_STATUS_VIDEO_COMPRESSION
      #define MCI_DGV_STATUS_IMAGE_COMPRESSION    MCI_VID_STATUS_IMAGE_COMPRESSION
      #define MCI_DGV_STATUS_AUDIO_QUALITY        MCI_VID_STATUS_AUDIO_QUALITY
      #define MCI_DGV_STATUS_VIDEO_QUALITY        MCI_VID_STATUS_VIDEO_QUALITY
      #define MCI_DGV_STATUS_IMAGE_QUALITY        MCI_VID_STATUS_IMAGE_QUALITY
      #define MCI_DGV_STATUS_IMAGE_BITSPERPEL     MCI_VID_STATUS_IMAGE_BITSPERPEL
      #define MCI_DGV_STATUS_IMAGE_PELFORMAT      MCI_VID_STATUS_IMAGE_PELFORMAT
      #define MCI_DGV_STATUS_FORWARD              MCI_VID_STATUS_FORWARD
      #define MCI_DGV_STATUS_NORMAL_RATE          MCI_VID_STATUS_NORMAL_RATE
      #define MCI_DGV_STATUS_VIDEO_X_EXTENT       MCI_VID_STATUS_VIDEO_X_EXTENT
      #define MCI_DGV_STATUS_VIDEO_Y_EXTENT       MCI_VID_STATUS_VIDEO_Y_EXTENT
      #define MCI_DGV_STATUS_IMAGE_X_EXTENT       MCI_VID_STATUS_IMAGE_X_EXTENT
      #define MCI_DGV_STATUS_IMAGE_Y_EXTENT       MCI_VID_STATUS_IMAGE_Y_EXTENT
      #define MCI_DGV_STATUS_BRIGHTNESS           MCI_VID_STATUS_BRIGHTNESS
      #define MCI_DGV_STATUS_CONTRAST             MCI_VID_STATUS_CONTRAST
      #define MCI_DGV_STATUS_HUE                  MCI_VID_STATUS_HUE
      #define MCI_DGV_STATUS_SATURATION           MCI_VID_STATUS_SATURATION
      #define MCI_DGV_STATUS_SPEED                MCI_VID_STATUS_SPEED
      #define MCI_DGV_STATUS_SHARPNESS            MCI_VID_STATUS_SHARPNESS
      #define MCI_DGV_STATUS_REF_INTERVAL         MCI_VID_STATUS_REF_INTERVAL
      #define MCI_DGV_STATUS_MAXDATARATE          MCI_VID_STATUS_MAXDATARATE
      #define MCI_DGV_STATUS_VIDEO_FILE_FORMAT    MCI_VID_STATUS_VIDEO_FILE_FORMAT
      
      #define MCI_DGV_STATUS_CHANNELS             (MCI_DGV_ITEM_BASE+13)
      #define MCI_DGV_STATUS_BITSPERSAMPLE        (MCI_DGV_ITEM_BASE+14)
      #define MCI_DGV_STATUS_SAMPLESPERSEC        (MCI_DGV_ITEM_BASE+15)
      #define MCI_DGV_STATUS_FORMATTAG            (MCI_DGV_ITEM_BASE+16)
      #define MCI_DGV_STATUS_BLOCKALIGN           (MCI_DGV_ITEM_BASE+17)
      #define MCI_DGV_STATUS_AVGBYTESPERSEC       (MCI_DGV_ITEM_BASE+18)
      #define MCI_DGV_STATUS_VIDEO_COMPRESSION_SUBTYPE   (MCI_DGV_ITEM_BASE+19)
      #define MCI_DGV_STATUS_VIDEO_RECORD_RATE           (MCI_DGV_ITEM_BASE+20)
      #define MCI_DGV_STATUS_VIDEO_RECORD_FRAME_DURATION (MCI_DGV_ITEM_BASE+21)
      #define MCI_DGV_STATUS_RECORD_AUDIO         (MCI_DGV_ITEM_BASE+22)
      #define MCI_DGV_STATUS_TRANSPARENT_COLOR    (MCI_DGV_ITEM_BASE+23)
      #define MCI_DGV_STATUS_GRAPHIC_TRANSPARENT_COLOR (MCI_DGV_ITEM_BASE+23)  /* MUST BE SAME AS TRANSPARENT COLOR */
      #define MCI_DGV_STATUS_HWND_MONITOR         (MCI_DGV_ITEM_BASE+24)
      #define MCI_DGV_STATUS_DROPPED_FRAME_PCT    (MCI_DGV_ITEM_BASE+25)
      #define MCI_DGV_STATUS_AUDIOSYNC            (MCI_DGV_ITEM_BASE+26)
      #define MCI_DGV_STATUS_AUDIOSYNC_DIRECTION  (MCI_DGV_ITEM_BASE+27)
      #define MCI_DGV_STATUS_VIDEO_TRANSPARENT_COLOR (MCI_DGV_ITEM_BASE+28)  /* MUST BE SAME AS TRANSPARENT COLOR */
      #define MCI_DGV_STATUS_TUNER_TV_CHANNEL     (MCI_DGV_ITEM_BASE+29)
      #define MCI_DGV_STATUS_TUNER_LOW_TV_CHANNEL (MCI_DGV_ITEM_BASE+30)
      #define MCI_DGV_STATUS_TUNER_HIGH_TV_CHANNEL (MCI_DGV_ITEM_BASE+31)
      #define MCI_DGV_STATUS_TUNER_REGION          (MCI_DGV_ITEM_BASE+32)
      #define MCI_DGV_STATUS_TUNER_FINETUNE        (MCI_DGV_ITEM_BASE+33)
      #define MCI_DGV_STATUS_TUNER_FREQUENCY       (MCI_DGV_ITEM_BASE+34)
      #define MCI_DGV_STATUS_TUNER_AUDIO_CHANNEL   (MCI_DGV_ITEM_BASE+35)
      #define MCI_DGV_STATUS_TUNER_AFC             (MCI_DGV_ITEM_BASE+36)
      #define MCI_DGV_STATUS_VALID_SIGNAL          (MCI_DGV_ITEM_BASE+37)
      
      /*************************************************/
      /* MCI_STATUS item values for video overlay      */
      /*************************************************/
      #define MCI_OVLY_STATUS_HWND                 MCI_VID_STATUS_HWND
      #define MCI_OVLY_STATUS_IMAGE_COMPRESSION    MCI_VID_STATUS_IMAGE_COMPRESSION
      #define MCI_OVLY_STATUS_IMAGE_BITSPERPEL     MCI_VID_STATUS_IMAGE_BITSPERPEL
      #define MCI_OVLY_STATUS_IMAGE_PELFORMAT      MCI_VID_STATUS_IMAGE_PELFORMAT
      #define MCI_OVLY_STATUS_IMAGE_X_EXTENT       MCI_VID_STATUS_IMAGE_X_EXTENT
      #define MCI_OVLY_STATUS_IMAGE_Y_EXTENT       MCI_VID_STATUS_IMAGE_Y_EXTENT
      #define MCI_OVLY_STATUS_BRIGHTNESS           MCI_VID_STATUS_BRIGHTNESS
      #define MCI_OVLY_STATUS_CONTRAST             MCI_VID_STATUS_CONTRAST
      #define MCI_OVLY_STATUS_HUE                  MCI_VID_STATUS_HUE
      #define MCI_OVLY_STATUS_SATURATION           MCI_VID_STATUS_SATURATION
      #define MCI_OVLY_STATUS_GREYSCALE            MCI_VID_STATUS_GREYSCALE
      #define MCI_OVLY_STATUS_IMAGE_QUALITY        MCI_VID_STATUS_IMAGE_QUALITY
      #define MCI_OVLY_STATUS_SHARPNESS            MCI_VID_STATUS_SHARPNESS
      #define MCI_OVLY_STATUS_IMAGE_FILE_FORMAT    MCI_VID_STATUS_IMAGE_FILE_FORMAT
      #define MCI_OVLY_STATUS_TRANSPARENT_TYPE     MCI_VID_STATUS_TRANSPARENT_TYPE
      #define MCI_OVLY_STATUS_TRANSPARENT_COLOR    MCI_VID_STATUS_TRANSPARENT_COLOR
      
      /*************************************************/
      /* Status Mode return values                     */
      /*************************************************/
      #define MCI_MODE_NOT_READY                  0x00000001L
      #define MCI_MODE_PAUSE                      0x00000002L
      #define MCI_MODE_PLAY                       0x00000003L
      #define MCI_MODE_STOP                       0x00000004L
      #define MCI_MODE_RECORD                     0x00000005L
      #define MCI_MODE_SEEK                       0x00000006L
      
      /*************************************************/
      /* Status Direction return values                */
      /*************************************************/
      #define MCI_FORWARD                         0x00000000L
      #define MCI_REVERSE                         0x00000001L
      
      typedef struct _MCI_STATUS_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulReturn;        /* Return field                            */
         ULONG   ulItem;          /* Item field for STATUS item to query     */
         ULONG   ulValue;         /* Status value field (this used to be)    */
         /*  ulTrack but was changed in Rel 1.1     */
         /*  to extend the status structure.        */
         /*  See the programming reference on when  */
         /*  ulValue is used and how...             */
      } MCI_STATUS_PARMS;
      typedef MCI_STATUS_PARMS   *PMCI_STATUS_PARMS;
      
      
      typedef struct _MCI_CDXA_STATUS_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulReturn;        /* Return field                            */
         ULONG   ulItem;          /* Item field for STATUS item to query     */
         ULONG   ulValue;         /* Status value field (this used to be)    */
         /*  ulTrack but was changed in Rel 1.1     */
         /*  to extend the status structure.        */
         /*  See the programming reference on when  */
         /*  ulValue is used and how...             */
         ULONG   ulChannel;       /* Channel                                 */
      } MCI_CDXA_STATUS_PARMS;
      typedef MCI_CDXA_STATUS_PARMS   *PMCI_CDXA_STATUS_PARMS;
      
      /*********************************************************************/
      /* flags and parameter structure for the MCI_STEP message            */
      /*     0x00000X00 are reserved for MCI_STEP flags                    */
      /*********************************************************************/
      #define MCI_STEP_FRAMES                     0x00000100L
      #define MCI_STEP_REVERSE                    0x00000200L
      
      typedef struct _MCI_STEP_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         ULONG   ulStep;          /* amount to step in current time format   */
      } MCI_STEP_PARMS;
      typedef MCI_STEP_PARMS   *PMCI_STEP_PARMS;
      
      /*********************************************************************/
      /* flags for the MCI_SYSINFO message                                 */
      /*     0xXXXXXX00 are reserved for MCI_SYSINFO flags                 */
      /*********************************************************************/
      #define MCI_SYSINFO_QUANTITY                0x00000100L
      #define MCI_SYSINFO_OPEN                    0x00000200L
      #define MCI_SYSINFO_NAME                    0x00000400L
      #define MCI_SYSINFO_INSTALLNAME             0x00000800L
      #define MCI_SYSINFO_ITEM                    0x00001000L
      #define MCI_SYSINFO_INI_LOCK                0x00002000L
      
      /********************************************************/
      /* Extended SYSINFO functions                           */
      /********************************************************/
      #define MCI_SYSINFO_INSTALL_DRIVER      0x00000001L
      #define MCI_SYSINFO_QUERY_DRIVER        0x00000002L
      #define MCI_SYSINFO_DELETE_DRIVER       0x00000004L
      #define MCI_SYSINFO_SET_PARAMS          0x00000008L
      #define MCI_SYSINFO_QUERY_PARAMS        0x00000010L
      #define MCI_SYSINFO_SET_CONNECTORS      0x00000020L
      #define MCI_SYSINFO_QUERY_CONNECTORS    0x00000040L
      #define MCI_SYSINFO_SET_EXTENSIONS      0x00000080L
      #define MCI_SYSINFO_QUERY_EXTENSIONS    0x00000100L
      #define MCI_SYSINFO_SET_ALIAS           0x00000200L
      #define MCI_SYSINFO_QUERY_NAMES         0x00000400L
      #define MCI_SYSINFO_SET_DEFAULT         0x00000800L
      #define MCI_SYSINFO_QUERY_DEFAULT       0x00001000L
      #define MCI_SYSINFO_SET_TYPES           0x00002000L
      #define MCI_SYSINFO_QUERY_TYPES         0x00004000L
      
      /********************************************************/
      /* Device Flags                                         */
      /********************************************************/
      #define MCI_SYSINFO_DEV_CONTROLLABLE    0x00000001L
      #define MCI_SYSINFO_DEV_NONCONTROLLABLE 0x00000002L
      #define MCI_SYSINFO_DEVICESETTINGS      0x00000004L
      
      /*********************************************************************/
      /* parameter structures for the MCI_SYSINFO message                  */
      /*********************************************************************/
      typedef struct _MCI_SYSINFO_PARMS  
      {
         HWND    hwndDummyCallback; /* NOTIFY not allowed for SYSINFO          */
         PSZ     pszReturn;         /* Pointer to return buffer                */
         ULONG   ulRetSize;         /* Return buffer size                      */
         ULONG   ulNumber;          /* Ordinal Number                          */
         USHORT  usDeviceType;      /* MCI device type number                  */
         USHORT  usReserved0;       /* Reserved field                          */
         ULONG   ulItem;            /* Used to indicate the MCI_SYSINFO        */
         /* extended function to perform            */
         PVOID   pSysInfoParm;
      } MCI_SYSINFO_PARMS;
      typedef MCI_SYSINFO_PARMS   *PMCI_SYSINFO_PARMS;
      
      typedef struct _MCI_SYSINFO_LOGDEVICE  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name            */
         USHORT  usDeviceType;                         /* Device type number             */
         ULONG   ulDeviceFlag;                         /* Flag indicating whether device */
         /*  device is controllable or not */
         CHAR    szVersionNumber[MAX_VERSION_NUMBER];  /* INI file version number        */
         CHAR    szProductInfo[MAX_PRODINFO];          /* Textual product description    */
         CHAR    szMCDDriver[MAX_DEVICE_NAME];         /* MCI Driver dll name            */
         CHAR    szVSDDriver[MAX_DEVICE_NAME];         /* VSD dll name                   */
         CHAR    szPDDName[MAX_PDD_NAME];              /* Device PDD name                */
         CHAR    szMCDTable[MAX_DEVICE_NAME];          /* Device type command table      */
         CHAR    szVSDTable[MAX_DEVICE_NAME];          /* Device specific command table  */
         USHORT  usShareType;                          /* Device sharing mode            */
         CHAR    szResourceName[MAX_DEVICE_NAME];      /* Resource name                  */
         USHORT  usResourceUnits;                      /* Total resource units available */
         /*  for this device               */
         USHORT  usResourceClasses;                    /* Number of resource classes for */
         /*  this device                   */
         USHORT  ausClassArray[MAX_CLASSES];           /* Maximum number of resource     */
         /*  units for each class          */
         USHORT  ausValidClassArray[MAX_CLASSES][MAX_CLASSES]; /* Valid class combinations */
      } MCI_SYSINFO_LOGDEVICE;
      typedef MCI_SYSINFO_LOGDEVICE  *PMCI_SYSINFO_LOGDEVICE;
      
      
      typedef struct _MCI_SYSINFO_DEVPARAMS  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name            */
         CHAR    szDevParams[MAX_DEV_PARAMS];          /* Device specific parameters     */
      } MCI_SYSINFO_DEVPARAMS;
      typedef MCI_SYSINFO_DEVPARAMS *PMCI_SYSINFO_DEVPARAMS;
      
      typedef struct _CONNECT  
      {
         USHORT usConnectType;                         /* Connector type                 */
         CHAR   szToInstallName[MAX_DEVICE_NAME];      /* Install name this connector    */
         /*  is connected to               */
         USHORT usToConnectIndex;                      /* Connector index this connector */
         /*  is connected to               */
      } CONNECT;
      typedef CONNECT *PCONNECT;
      
      
      typedef struct _MCI_SYSINFO_CONPARAMS  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name            */
         USHORT  usNumConnectors;                      /* Number of device connectors    */
         CONNECT ConnectorList[MAX_CONNECTORS];        /* Connector list array           */
      } MCI_SYSINFO_CONPARAMS;
      typedef MCI_SYSINFO_CONPARAMS *PMCI_SYSINFO_CONPARAMS;
      
      typedef struct _MCI_SYSINFO_EXTENSION  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name            */
         USHORT  usNumExtensions;                      /* Number of extensions           */
         CHAR    szExtension[MAX_EXTENSIONS][MAX_EXTENSION_NAME];
         /* Extension name array           */
      } MCI_SYSINFO_EXTENSION;
      typedef MCI_SYSINFO_EXTENSION *PMCI_SYSINFO_EXTENSION;
      
      typedef struct _MCI_SYSINFO_ALIAS  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name  */
         CHAR    szAliasName[MAX_ALIAS_NAME];          /* Alias name           */
      } MCI_SYSINFO_ALIAS;
      typedef MCI_SYSINFO_ALIAS *PMCI_SYSINFO_ALIAS;
      
      typedef struct _MCI_SYSINFO_DEFAULTDEVICE  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name  */
         USHORT  usDeviceType;                         /* Device type number   */
      } MCI_SYSINFO_DEFAULTDEVICE;
      typedef MCI_SYSINFO_DEFAULTDEVICE *PMCI_SYSINFO_DEFAULTDEVICE;
      
      typedef struct _MCI_SYSINFO_QUERY_NAME  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name  */
         CHAR    szLogicalName[MAX_DEVICE_NAME];       /* Logical device name  */
         CHAR    szAliasName[MAX_ALIAS_NAME];          /* Alias name           */
         USHORT  usDeviceType;                         /* Device type number   */
         USHORT  usDeviceOrd;                          /* Device type ordinal  */
      } MCI_SYSINFO_QUERY_NAME;
      typedef MCI_SYSINFO_QUERY_NAME *PMCI_SYSINFO_QUERY_NAME;
      
      typedef struct _MCI_SYSINFO_TYPES  
      {
         CHAR    szInstallName[MAX_DEVICE_NAME];       /* Device install name  */
         CHAR    szTypes[MAX_TYPEBUFFER+1];            /* EA types             */
      } MCI_SYSINFO_TYPES;
      typedef MCI_SYSINFO_TYPES *PMCI_SYSINFO_TYPES;
      
      
      /*********************************************************************/
      /* flags for the MCI_UNFREEZE message                                */
      /*     0x00000X00 are reserved for MCI_UNFREEZE flags                */
      /*********************************************************************/
      #define MCI_OVLY_UNFREEZE_RECT              0x00000100L
      #define MCI_OVLY_UNFREEZE_RECT_OUTSIDE      0x00000200L
      
      /*********************************************************************/
      /* flags for the MCI_WHERE message                                   */
      /*     0x0000XX00 are reserved for MCI_WHERE flags                   */
      /*********************************************************************/
      #define MCI_VID_WHERE_DESTINATION           0x00000100L
      #define MCI_VID_WHERE_SOURCE                0x00000200L
      #define MCI_VID_WHERE_WINDOW                0x00000400L
      
      #define MCI_DGV_WHERE_DESTINATION           MCI_VID_WHERE_DESTINATION
      #define MCI_DGV_WHERE_SOURCE                MCI_VID_WHERE_SOURCE
      #define MCI_DGV_WHERE_WINDOW                MCI_VID_WHERE_WINDOW
      #define MCI_DGV_WHERE_ADJUSTED              0x00000800L
      
      #define MCI_OVLY_WHERE_DESTINATION          MCI_VID_WHERE_DESTINATION
      #define MCI_OVLY_WHERE_SOURCE               MCI_VID_WHERE_SOURCE
      #define MCI_OVLY_WHERE_WINDOW               MCI_VID_WHERE_WINDOW
      
      /*********************************************************************/
      /* flags and parameter structure for the MCI_WINDOW message          */
      /*     0x0000XX00   are reserved for MCI_WINDOW flags                */
      /*********************************************************************/
      #define MCI_VID_WINDOW_HWND                 0x00000100L
      #define MCI_VID_WINDOW_STATE                0x00000200L
      #define MCI_VID_WINDOW_TEXT                 0x00000400L
      #define MCI_VID_WINDOW_DEFAULT              0x00000800L
      
      #define MCI_DGV_WINDOW_HWND                 MCI_VID_WINDOW_HWND
      #define MCI_DGV_WINDOW_STATE                MCI_VID_WINDOW_STATE
      #define MCI_DGV_WINDOW_TEXT                 MCI_VID_WINDOW_TEXT
      #define MCI_DGV_WINDOW_DEFAULT              MCI_VID_WINDOW_DEFAULT
      
      #define MCI_OVLY_WINDOW_HWND                MCI_VID_WINDOW_HWND
      #define MCI_OVLY_WINDOW_STATE               MCI_VID_WINDOW_STATE
      #define MCI_OVLY_WINDOW_TEXT                MCI_VID_WINDOW_TEXT
      #define MCI_OVLY_WINDOW_DEFAULT             MCI_VID_WINDOW_DEFAULT
      
      typedef  struct _MCI_VID_WINDOW_PARMS  
      {
         HWND    hwndCallback;    /* PM window handle for MCI notify message */
         HWND    hwndDest;        /* Handle to the client window used for    */
         /* the destination of the video image      */
         USHORT  usCmdShow;       /* Specifies how the window is displayed   */
         USHORT  usReserved1;     /* Reserved                                */
         PSZ     pszText;         /* The text to use as the window caption   */
         PSZ     pszAlias;        /* The window alias for the display window */
      } MCI_VID_WINDOW_PARMS;
      
      typedef MCI_VID_WINDOW_PARMS   MCI_DGV_WINDOW_PARMS;
      typedef MCI_DGV_WINDOW_PARMS   *PMCI_DGV_WINDOW_PARMS;
      
      typedef MCI_VID_WINDOW_PARMS   MCI_OVLY_WINDOW_PARMS;
      typedef MCI_OVLY_WINDOW_PARMS  *PMCI_OVLY_WINDOW_PARMS;
      
      
      /*********************************************************************/
      /* flags and parameter structure for the MCI_TUNER  message          */
      /*********************************************************************/
      #define MCI_DGV_AUDIO_CHANNEL               0x00010000L
      #define MCI_DGV_TV_CHANNEL                  0x00020000L
      #define MCI_DGV_FREQUENCY                   0x00040000L
      #define MCI_DGV_FINETUNE_PLUS               0x00080000L
      #define MCI_DGV_FINETUNE_MINUS              0x00100000L
      #define MCI_DGV_REGION                      0x00200000L
      #define MCI_DGV_AFC_ON                      0x00400000L
      #define MCI_DGV_AFC_OFF                     0x00800000L
      #define MCI_DGV_POLARIZATION                0x00800000L
      #define MCI_DGV_QUERY                       0x01000000L
      
      typedef struct _MCI_DGV_TUNER_PARMS  
      {
         HWND   hwndCallback;     /* PM window handle for MCI notify message    */
         ULONG  ulFrequency;      /* Tuner Frequency                            */
         ULONG  ulAudioChannel;   /* Audio Channel                              */
         ULONG  ulTVChannel;      /* TV Channel                                 */
         LONG   lFineTune;        /* Fine tuning adjustments.                   */
         PSZ    pszRegion;        /* TV Channel Region                          */
         ULONG  ulReserved1;      /* Reserved 1                                 */
         ULONG  ulReserved2;      /* Reserved 2                                 */
      } MCI_DGV_TUNER_PARMS;
      typedef MCI_DGV_TUNER_PARMS   *LPMCI_DGV_TUNER_PARMS;
      
      #pragma pack()
      /*********************************************************************/
      /*                                                                   */
      /*  MCI system value defines                                         */
      /*                                                                   */
      /*********************************************************************/
      #define MSV_CLOSEDCAPTION                   0x0
      #define MSV_MASTERVOLUME                    0x1
      #define MSV_HEADPHONES                      0x2
      #define MSV_SPEAKERS                        0x3
      #define MSV_WORKPATH                        0x4
      #define MSV_SYSQOSVALUE                     0x5
      #define MSV_SYSQOSERRORFLAG                 0x6
      
      #define MSV_MAX                             0x7
      
      
      /*********************************************************************/
      /* Playlist defines                                                  */
      /*********************************************************************/
      #define DATA_OPERATION                      0
      #define BRANCH_OPERATION                    1
      #define LOOP_OPERATION                      2
      #define CALL_OPERATION                      3
      #define RETURN_OPERATION                    4
      #define EXIT_OPERATION                      5
      #define NOP_OPERATION                       6
      #define MESSAGE_OPERATION                   7
      #define CUEPOINT_OPERATION                  8
      #define SEMWAIT_OPERATION                   9
      #define SEMPOST_OPERATION                  10
      
      /*********************************************************************/
      /*                                                                   */
      /*  MCI Function prototypes                                          */
      /*                                                                   */
      /*********************************************************************/
      
      
      ULONG APIENTRY mciSendCommand   (USHORT   usDeviceID,
                                       USHORT   usMessage,
                                       ULONG    ulParam1,
                                       PVOID    pParam2,
                                       USHORT   usUserParm);
      
      ULONG APIENTRY  mciSendString   (PSZ      pszCommandBuf,
                                       PSZ      pszReturnString,
                                       USHORT   usReturnLength,
                                       HWND     hwndCallBack,
                                       USHORT   usUserParm );
      
      ULONG APIENTRY mciGetErrorString(ULONG   ulError,
                                       PSZ     pszBuffer,
                                       USHORT  usLength);
      
      ULONG APIENTRY mciMakeGroup     (PUSHORT pusDeviceGroupID,
                                       USHORT  usDeviceCount,
                                       PUSHORT pausDeviceList,
                                       ULONG   ulFlags,
                                       ULONG   ulMMTime);
      
      
      ULONG APIENTRY mciDeleteGroup   (USHORT   usGroupID);
      
      
      
      BOOL APIENTRY mciSetSysValue (USHORT iSysValue,
                                    PVOID pValue);
      
      BOOL  APIENTRY mciQuerySysValue (USHORT iSysValue,
                                       PVOID pValue);
      
      ULONG  APIENTRY mciGetDeviceID  (PSZ  pszName);
      
   #endif
   
   #ifdef __cplusplus
   }
#endif


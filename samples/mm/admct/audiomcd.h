/***************************************************************************
*
* Module Name: AUDIOMCD.H
*
* OS/2 2.0 Multimedia Extensions Audio MCI Driver Instance Data
* Block defintions.
*
* Copyright (c) International Business Machines Corporation 1991,1993
*                         All Rights Reserved
*
\***************************************************************************/

/*
#ifdef _TORONTO
    typedef  ULONG   NULL;
#endif
*/

/* Performance stub for pagetuned pragma header files */
#ifndef PERFSTUB
  #include "perfstub.h"
  #define  PERFSTUB
#endif

/*************************************/
  // Definitons of sizes
/************************************/
#define  MAX_ERROR_LENGTH         128
#define  MAX_MODULE_NAME          10
#define  MAX_FILE_NAME            16
#define  HANDLER_NAMES_BUFLEN     128
#define  NOTIFY_THREAD_STACKSIZE  32768
#define  HEAP_SIZE                4096
#define  INDEFNITE_PERIOD          -1
#define  ONE_MINUTE               60000
#define  MAX_CUEPOINTS            20
#define  DEFAULT_CLIP_SIZE        1000000
#define  NUM_TAPS                 2
#define  DELAY_LINE_LENGTH        2

/************************************************
* Default Wave attributes
************************************************/
#define DEFAULT_SOURCE_HANDLER_NAME         "FSSH"
#define MEM_PLAYLIST_SH                     "MEMSH"
#define DEFAULT_SOURCE_PDD_NAME             " "
#define DEFAULT_TARGET_HANDLER_NAME         "AUDIOSH$"
#define DEFAULT_DEVDLL_NAME                 "AUDIOIF"
#define DEFAULT_BLOCK_ALIGN                  1

/* IO Procedure capabilities */

#define CAN_INSERT     0x00000001
#define CAN_DELETE     0x00000002
#define CAN_UNDOREDO   0x00000004
#define CAN_RECORD     0x00000008
#define CAN_SAVE       0x00000010

#define TEN_SECONDS    10000

/* Undo List */

typedef struct _UNDO_LIST {
   ULONG                ulNumLogicalActions;
   struct _UNDO_LIST    *pNextList;
   } UNDOLIST;

typedef UNDOLIST  *PUNDOLIST;

/*********************************************************************
*   Modified EVCB just has an Implicit Evcb and MCD Instance
*   in a Single Block.
*********************************************************************/
typedef struct MEVCB {
      IMPL_EVCB           evcb;                /* Implicit EVCB           */
      ULONG               ulpInstance;         /* Instance Ptr            */
      USHORT              usUserParm;          /* Returned on Cue Points  */
      } MEVCB; /* Modified EVCB */

/********************************************************************
*   Modified Time EVCB Block Contains a Time EVCB with MCD Instance,
*   and other useful Parameters
********************************************************************/
typedef struct MTIMEEVCB {
    TIME_EVCB            evcb;                /* Time EVCB                */
    USHORT               usDeviceID;           /* Cue Point Notifications  */
    HWND                 hwndCallback;          /* Cue Point Call back Hndl */
    ULONG                ulpInstance;         /* Instance Ptr             */
    USHORT               usCueUsrParm;        /* CuePt User Parm          */
    MMTIME               mmCuePt;             /* CuePt in MMTIME          */
    struct MTIMEEVCB     *pNextEVCB;          /* Pointer to next evcb    */
    HEVENT               HCuePtHndl;          /* Array of CuePt Event hndls*/

    } MTIME_EVCB;  /* Modified Event Control Block */

/*****************************************************************
*   Streaming Instance is a MCI Driver Layer Data Block
*   defining information needed to Use SSM SPIs. This structure
*   is Instance based. This Structure defines Streaming Info only.
*****************************************************************/
typedef struct _strInstance {
    HID         hidASource;                    /* Source Handler ID         */
    HID         hidATarget;                    /* STarget Handler ID        */
    HID         hidBSource;                    /* Target as Source ID       */
    HID         hidBTarget;                    /* Target as Target ID       */

// 6421--we definitely don't need the second variable--what about the first?

    char        pszAHandlerName[15];           /* Source Handler Name       */
    char        pszBHandlerName[15];           /* Target Handler Name       */
    HEVENT      hEvent;                        /* Implicit Event Handle     */
    HEVENT      hPosEvent;                     /* Position Event Handle     */
    HEVENT      hPlayToEvent;                  /* Cue Point Event handle    */
    MMTIME      mmStreamTime;                  /* Stream Time               */
    ULONG       ulState;                       /* Stream Pause,Play etc     */
    HSTREAM     hStream;                       /* Address of stream handle  */
    MEVCB       Evcb;                          /* Pointer to ECB            */
    MTIME_EVCB  TimeEvcb;                      /* Cue Point Time EVCB       */
    MTIME_EVCB  PosAdvEvcb;                    /* Time Events for PosAdvisue */
    MTIME_EVCB  *pMCuePtEVCB;
    PVOID       EventRtn;                      /* Event handler entry point */
    SPCBKEY     SpcbKey;                       /* SPCB key with Datatype    */
    ACB_MMIO    acbmmio;                       /* Associate Control Block   */
    ACB_MEM_PLAYL acbPlayList;                 /* Associate Control Block   */
    // 6421--we don't need this info anymore

//    DCB_AUDIOSH AudioDCB;                      /* AudioSH Device Control Bl */
    STATUS_LEVEL_EVCB StatusLevelEvcb;         /* Status_level */
    SPCB        spcb;                          /* SPCB for networking info  */
    HEVENT      hStatusLevelEvent;             /* Status_Level */
    } STRINSTANCE;                             /* Streaming Instance Struct */

/********************************************************************
*   This Structure defines the data items that are needed to be
*   maintained per MCI Device instance. The Instance structure
*   has a pointer to the Streaming Information Structure defined
*   above. Memory For This data structure is allocated dynamically
*   off a Global heap.
********************************************************************/

typedef struct _Instance {
    USHORT              usAmpDeviceID;             /* Amp Mixer Device ID  */
    USHORT              usWaveDeviceID;            /* Wave Device ID       */
    HEV                 hEventSem;                 /* Event Sem  Handle    */
    HEV                 hThreadSem;                /* Thread Control Sem   */
    HMTX                hmtxDataAccess;            /* Instance Data Access */
    HMTX                hmtxNotifyAccess;          /* Notify status access */
    HMTX                hmtxCloseAccess;           /* Control close access */
    HMTX                hmtxSaveAccess;            /* Control drv_save     */
    HMODULE             hModHandle;                /* Handle of above      */
    HMODULE             hModIOProc;                /* Custom IO Proc Hndl  */
    HMMIO               hmmio;                     /* MMIO file ID         */
    HFILE               hTempFile;                 /* Temp File Handle     */
    ULONG               ulDataSize;                /* size of audio file   */
    CHAR                pszAudioFile[CCHMAXPATH];  /* WAV File Name        */
// 6421--don't keep name
//    CHAR                szAudioDevName[9];         /* Audio Device Name    */
    ULONG               ulNotifyAborted;
    ULONG               ulTimeUnits;               /* Current Time Units   */
    ULONG               ulSyncOffset;              /* Synchronizatn offst  */
    ULONG               ulCreateFlag;              /* Flag for Strm Crtn   */
    ULONG               ulCreatedName;             /* Indicates we created name */
    BOOL                fFileExists;               /* Flag for file load   */
    ULONG               StreamEvent;               /* Stream Event         */
    BOOL                usNotifyPending;           /* Notify Pending Flag  */
    USHORT              usNotPendingMsg;           /* Msg for above        */
    HWND                hwndCallBack;              /* Call Back Handle     */
    HWND                hwndOldCallBack;           /* Temp callback handle */
    HWND                hwndOpenCallBack;          /* Call Back handle for open */
    BOOL                fToEvent;                  /* Play/Record to point */
    USHORT              usUserParm;                /* User Parameter       */
    BOOL                usSaveFlag;                /* Drv Save             */
    STRINSTANCE         StreamInfo;                /* Streaming Info       */

// 6421 --no need to keep this variable.

//    MCI_AMP_INSTANCE    AmpInstance;               /* VSDI driver Struct   */
    PVOID               pPlayList;                 /* Pointer to Play List */
    BOOL                usPlayLstStrm;             /* Flag for stream Crtn */
    BOOL                usPosAdvise;               /* Position Advise Flag */
    BOOL                usCuePt;                   /* CuePoint On Flag     */
    MMAUDIOHEADER       mmAudioHeader;             /* Audio format info    */
    USHORT              usPosUserParm;             /* User Parm Returned   */
    BOOL                fOpenMMIO;                 /* OPEN_MMIO used to load */
    BOOL                fRecdInsert;               /* Record Insert Flag   */
    BOOL                usWaitPending;             /* Pending Wait Flag    */
    BOOL                fFakeNotify;               /* Asynchronous notify  */
    USHORT              usWaitMsg;                 /* Pending Wait Msg     */
    ULONG               ulAbortedNotify;           /* indicates notify aborted */
    ULONG               ulCapabilities;            /* Capabilities of the io proc */
    ULONG               ulUsingTemp;               /* Using temp files     */
    ULONG               ulOpenTemp;                /* Open with temp files */
    ULONG               ulBlockAlignment;          /* Blk Algnment for wave*/
    ULONG               ulAverageBytesPerSec;      /* Holds average bps    */
    ULONG               ulOldStreamPos;            /* holds destroyed pos  */
//    LONG (* APIENTRY pfnVSD) (PMCI_AMP_INSTANCE, LONG, LONG, LONG, LONG);
//    HAB                 hab;
//    HMQ                 hmq;
//    QMSG                qmsg;
//    HWND                hwndClipWin;               /* Clipboard win handle */
    ULONG               ulBytes;           /* Scaled for Time Calcs    */
    ULONG               ulMMTime;          /* Scaled for Time Calcs    */
    LONG                lDefaultFormat;             /* Default value from INI file */
    LONG                lDefaultSRate;              /* Default value from INI file */
    LONG                lDefaultBPS;                /* Default value from INI file */
    LONG                lDefaultChannels;           /* Default value from INI file */
    LONG                lDefaultOperation;
    LONG                lQosValue;                  /* Network quality of service var */
    LONG                lQOSReporting;              /* Network error reporting flag */
    ULONG               ulHoldState;

   ULONG               ulRealTimeTranslation;

    ULONG               ulNumUndo;
    ULONG               ulNumRedo;
    PUNDOLIST           pUndoList;
    PUNDOLIST           pRedoList;
    BOOL                fInstalledProtocol;        /* Did we install an SpiProtocol? */
    PVOID               pcodecini;
    MMAUDIOHEADER       mmRealHeader;              /* Holds actual file header, not */
                                                   /* translated header. */
    PVOID               pcodecopen;
    ULONG               ulCodecDescription;        /* Direction of codec */
    BOOL                ulFlags;                    /* Generic flags */
    CHAR                szDevParm[MAX_DEV_PARAMS]; /* Device specific parameters */

    ULONG               ulOperation;
    BOOL                fConnected;                /* State of MCI Connection */
    BOOL                fCanStreamFrom;            /* Whether connected device can record */

// CONNECTION WORK
    HMTX                hmtxStreamAccess;          /* Access to streaming functions */
    PVOID               pdcb;                      /* Device Control Block */
    BOOL                fFoundMixer;               /* Mixer in network */
    HID                 hidAttached;               /* HID of attached device */

    ULONG              ulStatusLevel;              /* Status_Level SJB */
    ULONG              ulStatusLevelState;         /* Status_Level STATUS_LEVEL_ */


    } INSTANCE;         /* Audio MCD MCI Instance Block */

#define STATUS_LEVEL_OFF    0
#define STATUS_LEVEL_ON     1



/*********************************************************************
* Function Parameter Block defines the parameters needed by each MCI
* Message processing function to process the message.
**********************************************************************/
typedef struct {
    PVOID       ulpInstance;       /*  Pointer to Instance               */
    USHORT      usMessage;         /*  The Message                       */
    ULONG       ulParam1;          /*  Flags for the Message             */
    ULONG       ulParam2;          /*  Data for The Message              */
    USHORT      usUserParm;        /*  User Parameter Returned on Notify */
    ULONG       ulNotify;          /*  Notification flag                 */
    INSTANCE    *pInstance;        /*  Shall be Modified later           */
    HWND        hwndCallBack;      /*  Window Handle                     */
    }FUNCTION_PARM_BLOCK;          /*  Audio MCD Function Paramter Block */


/*********************************************************************
* Function Parameter Block defines the parameters needed by each MCI
* Message processing function to process the message.
**********************************************************************/


typedef struct {
    QOSInfo     QOSParms;
    QOS         QOSItem[ 6 ];
    } CUSTOM_QOS;




/************************************************/
        // MASKS for MCI_WAIT and MCI_NOTIFY
/************************************************/
#define NOTIFY_MASK     ~(MCI_NOTIFY | MCI_WAIT)
#define WAIT_MASK       0xffffff00L
#define   VSDI          "AUDIOIFDriverEntry"

/************************************
* Streaming STATE defines
*************************************/
#define CREATE_STATE        0x080
#define PREROLL_STATE       0x081
#define SAVED_STATE         0x082
#define RESTOR_STATE        0x083
#define ACTIVE              0x084
#define UNUSED              0x085
#define STREAM_STOPPED      0x086
#define PLAY_STREAM         0x087
#define RECORD_STREAM       0x088
#define DESTROYED_STATE     0x089
#define CUEPLAY_STATE       0x090
#define CUERECD_STATE       0x091
#define SAVEPAUS_STATE      0x09a
#define OPEN_FATAL          0x09b
#define EVENT_ENABLED       0x09c
#define ASYNCH_MESSAGE      0x09d
#define STREAM_SET_STATE    0x09e
#define STOP_PAUSED         0x09f
#define NO_STATE            0x100
#define NOT_INTIALIZED      -1
#define FILE_LENGTH         -1
#define WAVE_IO_PROC        01
#define AVC_IO_PROC         02

/************************************
* Current Time Unit TAGS
************************************/
#define lMMTIME         01
#define lMILLISECONDS   02
#define lBYTES          03
#define lSAMPLES        04

/************************************
*  Internal Error States
************************************/
#define MCI_WAVEERR_BASE        6000
#define NULLPOINTER             (MCI_WAVEERR_BASE +51)
#define NUM_DATATYPES           42
#define SCALE_FACTOR            100

#define XWAVHDR      (ulpInstance->mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo)
#define WAVEHDR      (ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader)
#define CUEPOINT     (ulpInstance->StreamInfo.pMCuePtEVCB)
#define STRMSTATE    (ulpInstance->StreamInfo.ulState)
#define STREAM       (ulpInstance->StreamInfo)
#define AMPMIX       (ulpInstance->AmpInstance)

/* CODEC defines */
#define NON_PCM          0x00000001
#define FORMAT_TAG_SET   0x00000002
#define FOURCC_WAV                       mmioFOURCC('W','A','V','E')
#define SOURCE_CODEC     0x00000001
#define TARGET_CODEC     0x00000002
#define MAKE_SOURCE      0x00000004
#define MAKE_TARGET      0x00000008
#define MMIO_RAWHEADER   0x00000001


/* DEVCAPS stuff */
#define DEVICE_CAN_RECORD  0x0000001
#define DEVICE_CAP_UNKNOWN 0x0000002


/* Device specific parms information *. */
#define OBTAINDEFAULTS     0x00000001
#define FIRST_RESTORE      0x00000002
#define READ_ONLY_FILE     0x00000004

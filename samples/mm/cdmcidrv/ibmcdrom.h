/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  IBMCDROM.H                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Include file for IBM CD ROM Drive Hardware dependent  */
/*                    code for CD Audio MCI Driver (IBMCDROM.C)             */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1990 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the macro definition used in the           */
/*            CD Audio MCI Driver, IBM specific hardware.                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

/*** Values used ***/

#define TRUE            1
#define FALSE           0

#define IBMCD010       10       // Hardware is the IBM 3510 CD-ROM Drive.
#define IBMCD019       19       // Hardware is unknown, treat like CD01.
#define CD01_CAPS     CDVSD_CAP_HAS_AUDIO | CDVSD_CAP_CAN_LOCK |   \
                      CDVSD_CAP_HAS_DAC | CDVSD_CAP_CAN_EJECT |    \
                      CDVSD_CAP_CAN_VOLUME

#define RBMODE          1       // Redbook Addressing Mode
#define IS_DATA_TRK  0x40       // Data Track bit track control information
#define NO_MEDIA     0x08       // No disc present bit for Device Status call
#define IS_PLAYING   0x10       // Playing Audio bit for Device Status call
#define WAS_STOPPED  0x01       // Paused Audio bit for Audio Status call
#define OPENFLAG        1L      // Open flag, open if exists else fail
#define OPENMODE   0x0000A0C0L  // Open mode for shareing, exclusive = 0xA090L
#define SPEC_START   6000       // 2 sec start address, external spec says so.
#define REG_INST        0       // Register only to instance
#define REG_PARAM2      1       // Register only to ulParam2
#define REG_BOTH        2       // Register to both instance and ulParam2
#define INFO_SIZE       9       // Size of INFO product string.
#define MODEL_SIZE     25       // Max Size of Model, Vendor + Product + NULL
#define SSM_LOAD_SIZE 100       // Size of error buffer when loading SSM
#define ONE_MMTIME     40       // MMTIME units per Redbook frame
#define SYNC_LATENCY ONE_MMTIME // Assume One frame elapsed send sync originated
#define SYNC_TOLERANCE 1500     // +/- half a second
#define WAIT_NOTIFY_MASK (0xFFFFFFFF ^ (MCI_WAIT | MCI_NOTIFY))
#define THREAD_STACK_SZ 65536L  // Stack size for MCI_NOTIFY thread.

/* Wait values for the Timer Play Loop */
#define WAIT_TIME_MAX 1000      // Timer wait max interval in milliseconds.
#define WAIT_TIME_MIN  160      // Timer wait min interval in milliseconds.
#define HALF_TIME_MIN  WAIT_TIME_MIN / 2  // half of the min value.
#define WAIT_FOREVER   -1L      // Suspend until released


/* Flags */
#define TIMER_AVAIL        0    // PlayFlag Timer available.
#define TIMER_PLAYING      1    // PlayFlag Timer is Playing.
#define TIMER_EXIT_SUPER   2    // Timer terminate from a PLAY w/ MCI_NOTIFY.
#define TIMER_EXIT_ABORTED 3    // Timer terminated from another command.
#define TIMER_EXIT_CHANGED 4    // Disc changed on a restore, terminate timer.
#define TIMER_PLAY_SUSPEND 5    // PlayFlag Timer is Playing but suspended.
#define TIMER_PLAY_SUSP_2  6    // PlayFlag suspended, restored paused.

/*** Field positions for data[] and param[] structures used in IOCTLs ***/

#define ADRMODEFLD      4       // Address mode Field in param for Play/Read
#define AUDSTATFLD      0       // Audio Paused Field in data for Audio Status
#define END_MM_FLD     11       // End Min Field in param for Play Audio
#define END_SS_FLD     10       // End Sec Field in param for Play Audio
#define END_FF_FLD      9       // End Frame Field in param for Play Audio
#define END_00_FLD     12       // End Null Field in param for Play Audio
#define LOC_MM_FLD      2       // Current Min Field in data for Location
#define LOC_SS_FLD      1       // Current Sec Field in data for Location
#define LOC_FF_FLD      0       // Current Frame Field in data for Location
#define LOCKDORFLD      4       // Door Field in param for Lock/Unlock Door
#define READLOWFLD      5       // Sector Count Low Field in param for Read
#define READHI_FLD      6       // Sector Count High Field in param for Read
#define READ_MMFLD      9       // Start Min Field in param for Play Audio
#define READ_SSFLD      8       // Start Sec Field in param for Play Audio
#define READ_FFFLD      7       // Start Frame Field in param for Play Audio
#define READ_00FLD     10       // Start Null Field in param for Play Audio
#define READRESFLD     11       // Reserved Field in param for Read
#define READ_1_FLD     12       // Interleave Size Field in param for Read
#define READ_2_FLD     13       // Interleave Skip Field in param for Read
#define SEEK_MMFLD      7       // Start Min Field in param for Seek
#define SEEK_SSFLD      6       // Start Sec Field in param for Seek
#define SEEK_FFFLD      5       // Start Frame Field in param for Seek
#define SEEK_00FLD      8       // Start Null Field in param for Seek
#define STARTMMFLD      7       // Start Min Field in param for Play Audio
#define STARTSSFLD      6       // Start Sec Field in param for Play Audio
#define STARTFFFLD      5       // Start Frame Field in param for Play Audio
#define START00FLD      8       // Start Null Field in param for Play Audio
#define STATAUDFLD      1       // Audio Field in param for Device Status
#define STATDRVFLD      0       // Drive Field in param for Device Status
#define TRACKFIELD      4       // Track Number Field in param for Track Info
#define TRKSTATFLD      4       // Track Status Field in data for Track Info
#define TRACKMMFLD      2       // Start Min Field in data for Track Info
#define TRACKSSFLD      1       // Start Sec Field in data for Track Info
#define TRACKFFFLD      0       // Start Frame Field in data for Track Info
#define TRCKLOWFLD      0       // Low Track Number Field in data for Disk Info
#define TRCKHI_FLD      1       // High Track Number Field in data for Disk Info
#define TRCKENDADR      2       // End Track Address Field in data for Disk Info
#define TRENDMMFLD      4       // End Track Min Field in data for Disk Info
#define TRENDSSFLD      3       // End Track Sec Field in data for Disk Info
#define TRENDFFFLD      2       // End Track Frame Field in data for Disk Info
#define VOL_LT_FLD      1       // Left Volume Field in data for Audio Channel
#define VOL_RT_FLD      3       // Right Volume Field in data for Audio Channel
#define CD_UPC_FLD      1       // UPC Field in data for UPC Code Info


/*** Array lengths for data[] and param[] structures used in IOCTLs ***/

#define STANDRD_DMAX    0       // Standard length for Data, length of NULL
#define STANDRD_PMAX    4       // Standard length for Param, length of "CD01"
#define IDCDROM_DMAX    4       // Max range for data in Identify CD-ROM
#define DSKTRCK_DMAX    6       // Max range for data in Disk and Track Info
#define DSKTRCK_PMAX    5       // Max range for param in Disk and Track Info
#define LOCATON_DMAX   10       // Max range for data in Location IOCTL call
#define LOCATON_PMAX    5       // Max range for param in Location IOCTL call
#define LOCKDOR_PMAX    5       // Max range for param in Lock Door IOCTL call
#define READAUD_PMAX   14       // Max range for param in Read Audio
#define PLAYAUD_PMAX   13       // Max range for param in Play Audio
#define SEEKSTATDMAX    4       // Max range for data in Seek & Status call
#define SEEKSTATPMAX    9       // Max range for param in Seek & Status call
#define AUDSTAT_DMAX   10       // Max range for data in Audio Status Info.
#define QVOLUME_DMAX    8       // Max range for data in Audio Channel Info.
#define UPCINFO_DMAX   10       // Max range for data in UPC Code Info.


/*** Values used for function and category IDs used in IOCTLs ***/

#define CDDRIVE_CAT  0x80       // CD drive and disk category IOCTL calls
#define RESET_DRIVE  0x40       // Reset CD-ROM Drive
#define EJECT__DISK  0x44       // Eject CD Disc
#define LOCK___DOOR  0x46       // Lock / Unlock Door
#define SEEK_POSITN  0x50       // Seek to a position
#define DEV__STATUS  0x60       // Device Status Information
#define ID___CD_ROM  0x61       // Identify CD-ROM Driver
#define Q__LOCATION  0x70       // Query Location
#define READ__AUDIO  0x72       // Read Audio
#define CD_UPC_INFO  0x79       // UPC of the CD

#define CDAUDIO_CAT  0x81       // CD drive and disk category IOCTL calls
#define AUD_CH_CTRL  0x40       // Set Audio Channel Control
#define PLAY__AUDIO  0x50       // Play Audio
#define STOP__AUDIO  0x51       // Stop Audio
#define RESUMEAUDIO  0x52       // Resume Audio
#define AUD_CH_INFO  0x60       // Query Audio Channel Info
#define DISK___INFO  0x61       // Audio Disk Information
#define TRACK__INFO  0x62       // Audio Track Information
#define AUD__STATUS  0x65       // Audio Status Information


/*********************************************************************/
/*  TIME Conversions Macros                                          */
/*********************************************************************/
#define REDBOOK2TOMM(value)   (((((value) & 0x00FF0000) >> 16) * 60 * 3000) + \
                               ((((value) & 0x0000FF00) >>  8) * 3000) + \
                                (((value) & 0x000000FF) * 3000/75))

  /* The +20 is used for rounding purposes.  It is derived by:           */
  /*   1/2 * ((300 MMTIME/SEC / (75 FRAMES/SEC)) = 20 MMTIME/HALF-FRAME  */
#define REDBOOK2FROMMM(value) ((((value)+20) >= 0x100*60*3000) ? 0L : \
                              (((((value)+20) / (60*3000)) << 16) | \
                                (((value)+20) % (60*3000) / 3000 << 8) | \
                                (((value)+20) / (3000/75) % 75)))


typedef struct
{
   ULONG     ulEvent;                     // event in MMTIME
   HWND      hwndCallback;                // call back window address
   USHORT    usUserParm;                  // Passed in user parameter
} CUEPOINT_REC;

/**** Instance structure ****/

struct instance_state
{
   HFILE      hDrive;                      // drive handle
   CHAR       Drive,                       // Drive letter
              szModel[MODEL_SIZE];         // Model name
   USHORT     usHWType,                    // Hardware Type
              usPlayFlag;                  // Play flag, acts as semaphore
   USHORT     usDeviceID;                  // Logical device ID from MDM
   HMTX       hIOSem,                      // semaphore for IOCTLs
              hInstSem;                    // semaphore for instance table
   HEV        hReturnSem,                  // lets hInstSem be released
              hTimeLoopSem;                // block polling while not active
   MCI_CD_ID  DiscID;                      // CD disc ID
   ULONG      ulCurPos,                    // logical current position
              ulEndPos;                    // end play position
   CUEPOINT_REC  qptPosAdvise,             // Position Advise Frequency
              arrCuePoint[CDMCD_CUEPOINT_MAX];      // Cue Point records
   ULONG      ulCDMCDID;                   // CD MCD ID for instance
   PFNCDMCD   pCDMCDReturn;                // Addr of CD MCD Return function
   BYTE       StreamMaster;                // Master in sync group, TRUE/FALSE
   PVOID      pSyncInst;                   // Sync handle
   GID        GroupID;                     // Sync group
   SYNC_EVCB  evcbSync;                    // Sync event control block
   PARM_EVENT RptEvent;                    // Event reporting parameter
   PFN        pSyncNotify;                 // Notify sync pulse function
   ULONG      ulPlayTID;                   // Play thread ID
   HWND       hwndPlayCallback;            // PM call back handle for MCI_PLAY
   USHORT     usPlayUserParm;              // User Parm for Play command
   USHORT     usPlayNotify;                // Play notify flag
};

typedef struct instance_state *PINST;


/* Prototypes */
/* ibmcdrom.c */
static ULONG process_msg(PINST, USHORT, ULONG *, PVOID, USHORT);
static ULONG CDAudClose(PINST, ULONG);
static ULONG CDAudErrRecov(PINST);
static ULONG CDAudInfo(PINST, ULONG, MCI_INFO_PARMS *);
static ULONG CDAudOpen(ULONG, MMDRV_OPEN_PARMS *);
       ULONG CDAudRegDisc(PINST, USHORT, MCI_CD_REGDISC_PARMS *);
static ULONG CDAudRegDrive(PINST, MCI_CD_REGDRIVE_PARMS *);
static ULONG CDAudSet(PINST, ULONG *, MCI_SET_PARMS *);
static ULONG CDAudSetVerify(ULONG);
static ULONG CDAudStatus(PINST, ULONG, MCI_STATUS_PARMS *);
       ULONG CDAudStatCVol(ULONG *);

/* ibmcdpro.c */
ULONG CD01_Cue(PINST);
ULONG CD01_CuePoint(PINST, ULONG, MCI_CUEPOINT_PARMS *);
ULONG CD01_GetCaps(ULONG, MCI_GETDEVCAPS_PARMS *);
ULONG CD01_GetDiscInfo(PINST, ULONG, MCI_STATUS_PARMS *);
ULONG CD01_GetID(PINST, MCI_CD_ID *, BYTE *, BYTE *);
ULONG CD01_GetPosition(PINST, ULONG *);
ULONG CD01_GetState(PINST, MCI_STATUS_PARMS *);
ULONG CD01_GetTOC(PINST, MCI_TOC_PARMS *);
ULONG CD01_GetVolume(PINST, ULONG *);
ULONG CD01_LockDoor(PINST, USHORT);
ULONG CD01_Open(PINST);
ULONG CD01_Play(PINST, ULONG *, ULONG, ULONG, USHORT, HWND);
ULONG CD01_PlayCont(PINST, ULONG, ULONG);
ULONG CD01_PosAdvise(PINST, ULONG, MCI_POSITION_PARMS *);
ULONG CD01_RegTracks(PINST, MCI_CD_REGTRACKS_PARMS *);
ULONG CD01_Restore(PINST, MCIDRV_CD_SAVE_PARMS *);
ULONG CD01_Resume(PINST);
ULONG CD01_Save(PINST, MCIDRV_CD_SAVE_PARMS *);
ULONG CD01_Seek(PINST, ULONG);
ULONG CD01_SetVolume(PINST, ULONG);
ULONG CD01_Stop(PINST, USHORT);

/* ibmcdmsc.c */
ULONG CD01_StartPlay(PINST);
ULONG CD01_Sync(PINST, ULONG, MCIDRV_SYNC_PARMS *);
ULONG CD01_Timer(PINST);
ULONG CD01_TimerNotify(PINST, ULONG *, ULONG *, ULONG *);
VOID  GetTableName(CHAR *);
ULONG OpenFirstTime(PINST);
VOID  QueryTable(PINST);
ULONG CallIOCtl(PINST, ULONG, ULONG, PVOID, ULONG, ULONG *,
                                     PVOID, ULONG, ULONG *);

/* cdmccomn.c */
       VOID   parse_DevParm(CHAR *, CHAR **, CHAR **);
static CHAR * get_token(CHAR *);


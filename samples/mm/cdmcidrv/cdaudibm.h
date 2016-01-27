/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  CDAUDIBM.H                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Include file for CD Audio MCI Driver (CDAUDIO.C)      */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1990 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the macro definition used in the           */
/*            CD Audio MCI Driver.                                          */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#define TRUE            1
#define FALSE           0

/* Instance status, must be in this order */
#define UNLOADED  0        // initial state
#define OPENED    1        // lower MCI Driver was opened
#define REGDRIVE  2        // registered drive
#define REGDISC   3        // registered disc
#define REGTRACK  4        // registered tracks
#define NODISC    5        // no disc present, will go to STOPPED or REGDRIVE.
#define PLAYING   6
#define STOPPED   7
#define PAUSED    8
#define SUSPEND  20        // instance lost control

/* MODES */
/* addressing mode */
#define TIME_MODE       0x0000000F    // time mode, byte 0.
#define TIME_MODE_SET   0xFFFFFFF0    // time mode, byte 0.
#define CDMC_MMTIME     0x00000001    // Multimedia Time (3000 / sec.)
#define CDMC_MILLSEC    0x00000002    // Millisecond     (1000 / sec.)
#define CDMC_REDBOOK    0x00000003    // Red Book (MSF)  (  75 / sec.)
#define CDMC_TMSF       0x00000004    // Track + MSF     (  75 / sec.)

/* streaming mode */
#define STREAM_MODE     0x00000030    // stream mode, byte 1, low nibble.
#define STREAM_MODE_SET 0xFFFFFFCF    // stream mode, byte 1, low nibble.
#define CDMC_INTDAC     0x00000010    // process internal DAC
#define CDMC_STREAM     0x00000020    // processing via stream

/* stream capability mode, of instance & drive */
#define STREAM_CAP      0x000000C0    // stream cap mode, byte 1, high nibble.
#define STREAM_CAP_SET  0xFFFFFF3F    // stream cap mode, byte 1, high nibble.
#define CDMC_CAN_DAC    0x00000040    // has internal DAC
#define CDMC_CAN_STREAM 0x00000080    // has streaming capabilities

/* channel type enabled mode */
#define CHAN_TYPE       0x00000F00    // channel mode, byte 2.
#define CHAN_TYPE_SET   0xFFFFF0FF    // channel mode, byte 2.
#define CDMC_HEADPHONE  0x00000100    // headphone channel enabled
#define CDMC_SPEAKER    0x00000200    // speaker channel enabled

/* channel enabled mode */
#define CHAN_MODE       0x0000F000    // channel mode, byte 3.
#define CHAN_MODE_SET   0xFFFF0FFF    // channel mode, byte 3.
#define CDMC_LFT_CH     0x00001000    // left channel enabled
#define CDMC_RGT_CH     0x00002000    // right channel enabled
#define CDMC_ALL_CH     0x00003000    // both channel enabled, must be LFT | RGT

/* misc flags */
#define CDMC_PERCENT    0x00010000    // speed format in %, 0 = frames/sec
#define CDMC_SHAREABLE  0x00020000    // opened as shareable

/* Volume Setting Macros */
#define VOL_LEFT(level)     (*(USHORT *)((ULONG *)(&level)))
#define VOL_RIGHT(level)    (*((USHORT *)((ULONG *)(&level))+1))

/* Other definition */
#define VALIDID "CDDA"     // valid id, used to validate pointer
#define VALLEN    4        // length of VALIDID
#define MMT_FRAME          40   // MMTIME units per Redbook frame
#define LOAD_MOD_BUF_LEN  100   // length of DLL load module buffer
#define IS_PRE_EMPH      0x10   // Pre-emphasis bit track control information
#define IS_COPYABLE      0x20   // Digital copying is permitted.
#define IS_DATA_TRK      0x40   // Data Track bit track control information
#define HAS_4_CHANS      0x80   // 4 Channel bit track control information
#define IS_OTHER_TRK     0x90   // Other Track bits track control information
#define WAIT_NOTIFY_MASK (0xFFFFFFFF ^ (MCI_WAIT | MCI_NOTIFY))
#define WAIT_FOREVER      -1L   // Suspend until released


typedef struct instance_state *PINST;

/***********************************************/
/* Instance State structure and Instance Table */
/***********************************************/

#define IDSIZE sizeof(MCI_CD_ID)


struct instance_state
{
   CHAR   valid[VALLEN];                 // marker to validate pointer
   ULONG  ulMode;                        // Stream and Addressing mode
   USHORT usStatus;                      // instance status
   ULONG  ulCur_pos,                     // current position of CD head
          ulEnd_play,                    // end address of PLAY
          ulStart_disk,                  // start address of playable Disc
          ulEnd_disk,                    // end address of playable Disc
          ulOffset,                      // Synchronization Offset
          ulMasterVolume,                // master volume level, in %
          ulLevel;                       // component volume level, R:L in %
   PVOID  hHWMCID;                       // hardware specific MCI Driver handle
   PFNCDVSD  pMCIDriver;                 // pointer to HW specific (VSD) entry
   HMODULE hMod;                         // handle to VSD module
   MCIDRV_CD_SAVE_PARMS   recSave;       // Save information record
   USHORT usDeviceOrd;                   // The device ordinal number
   USHORT usCaps;                        // drive capabilities
   ULONG  ulPrerollType,                 // drive preroll type
          ulPrerollTime,                 // drive preroll time
          ulMinStartTime;                // drive minimum seek address
   MCI_CD_REGDISC_PARMS   recDisc;       // Disc info record
   MCI_CD_REGTRACKS_PARMS recTrack;      // Track info record
   PVOID  pTrackInfo;                    // Track info buffer
   ULONG  ulTrackInfoSize;               // Track info buffer size
   USHORT usDeviceID;                    // Device ID from MDM
   ULONG  arrCuePoint[CDMCD_CUEPOINT_MAX];   // array of cuepoints
   BYTE   SyncMaster;                    // Master in sync group, TRUE/FALSE
   HMTX   hInstSem;                      // semaphore for instance table
};

/* PROTOTYPES */

/*    cdaudio.c      */
ULONG pre_process_msg(PINST, USHORT *, ULONG);
ULONG process_msg(PINST, USHORT, ULONG *, PVOID, USHORT);
ULONG verify_entry(PINST, USHORT, ULONG, PVOID *);
VOID  QMAudio(PINST);
ULONG Register(PINST);
ULONG ReRegister(PINST);
VOID  VSDReturn(ULONG, ULONG, ULONG);
ULONG SetTrackInst(PINST, USHORT);
ULONG ValPointer(PVOID, ULONG);


/*    cdaudpro.c     */
ULONG ProcClose(PINST, ULONG *, PVOID, USHORT);
ULONG ProcConnector(PINST, ULONG, MCI_CONNECTOR_PARMS *);
ULONG ProcCue(PINST, ULONG);
ULONG ProcCuePoint(PINST, ULONG, MCI_CUEPOINT_PARMS *);
ULONG ProcGeneral(PINST, USHORT, ULONG *, PVOID, USHORT);
ULONG ProcCaps(PINST, ULONG, MCI_GETDEVCAPS_PARMS *);
ULONG ProcInfo(PINST, ULONG, MCI_INFO_PARMS *);
ULONG ProcMAudio(PINST, ULONG, MCI_MASTERAUDIO_PARMS *);
ULONG ProcOpen(ULONG *, MMDRV_OPEN_PARMS *, USHORT);
ULONG ProcPause(PINST, ULONG);
ULONG ProcPlay(PINST, ULONG *, MCI_PLAY_PARMS *, USHORT);
ULONG ProcPosAdvise(PINST, ULONG, MCI_POSITION_PARMS *);
ULONG ProcRestore(PINST);
ULONG ProcResume(PINST, ULONG);
ULONG ProcSave(PINST);
ULONG ProcSeek(PINST, ULONG, MCI_SEEK_PARMS *);
ULONG ProcSet(PINST, ULONG *, MCI_SET_PARMS *);
ULONG ProcSetSync(PINST, ULONG, MCI_SYNC_OFFSET_PARMS *);
ULONG ProcStatus(PINST, ULONG, MCI_STATUS_PARMS *);
ULONG ProcStop(PINST, ULONG);
ULONG ProcSync(PINST, ULONG, MCIDRV_SYNC_PARMS *);


/*    cdaudutl.c     */
ULONG SetAudio(PINST, ULONG *, MCI_SET_PARMS *, ULONG *, ULONG *);
ULONG SetConnector(PINST, ULONG);
ULONG SetCuePoint(PINST, MCI_CUEPOINT_PARMS *);
ULONG StatusMCD(PINST, ULONG, MCI_STATUS_PARMS *, ULONG, USHORT);
ULONG StatusMCDDef(PINST, ULONG, MCI_STATUS_PARMS *, ULONG);
ULONG StatusVSD(PINST, ULONG, MCI_STATUS_PARMS *, ULONG);
VOID  DisableEvents(PINST);
ULONG GetTimeAddr(PINST, ULONG, USHORT);
ULONG GetTimeAddrRC(PINST, ULONG);
MCI_CD_REGTRACK_REC * GetTrackInfo(PINST, BYTE, ULONG);
ULONG ValAddress(PINST, ULONG *, ULONG *, USHORT);
ULONG ValState(PINST);
ULONG vsdResponse(PINST, ULONG);


/* cdmccomn.c */
       VOID   parse_DevParm(CHAR *, CHAR **, CHAR **);
static CHAR * get_token(CHAR *);


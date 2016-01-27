/****************************************************************************/
/*                                                                          */
/* Module Name: AUDIO.H                                                     */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions Audio Structures and definitions.         */
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


/*-------------------------------------------------------------------------*
* AUDIODD version level
*-------------------------------------------------------------------------*/

#define CURRENT_VERSION  0x01020000

/*-------------------------------------------------------------------------*
* Control definitions
*-------------------------------------------------------------------------*/

#define AUDIO_IOCTL_CAT   0x80

#define AUDIO_INIT        0x40
#define AUDIO_STATUS      0x41
#define AUDIO_CONTROL     0x42
#define AUDIO_BUFFER      0x43
#define AUDIO_LOAD        0x44
#define AUDIO_WAIT        0x45
#define AUDIO_HPI         0x46
#define AUDIO_CAPABILITY  0x48

#define MIX_GETCONNECTIONS      0x60
#define MIX_SETCONNECTIONS      0x61
#define MIX_GETLINEINFO         0x62
#define MIX_GETCONTROL          0x63
#define MIX_SETCONTROL          0x64

/*-------------------------------------------------------------------------*
* AUDIO_INIT declarations and defines
*-------------------------------------------------------------------------*/

#define LOAD_PATH       260

/* Values for AUDIO_INIT.sMode  */

#define  ADPCM    1                       /* AVC type ADPCM                */
#define  PCM      2                       /* Pulse Coded Modulation        */
#define  MU_LAW   3                       /* mu-law                        */
#define  MIDI     4                       /* MIDI data                     */
#define  A_LAW    5                       /* a-law                         */
#define  SOURCE_MIX 6                     /* External audio source         */
#define  SPV2     7                       /* Speech Viewer/2               */
#define  ADPCMXA  8                       /* XA CD ROM                     */
#define  SPV2BCPCM      25                /* Speech Viewer/2               */
#define  SPV2PCM        26
#define  SPV2NONE       27
#define  IDLE           999
#define  CLAIM_HDWR 32000                 /* Serialize access to hardware  */

/* Values for AUDIO_INIT.ulFlags        */

#define  FIXED             0x00000001l    /* Fixed length data             */
#define  LEFT_ALIGNED      0x00000002l    /* Left align bits on byte bndry */
#define  RIGHT_ALIGNED     0x00000004l    /* Right align bits on byte bndry*/
#define  TWOS_COMPLEMENT   0x00000008l    /* 2's complement data           */
#define  SIGNED            0x00000010l    /* Signed data                   */
#define  BIG_ENDIAN        0x00000020l    /* MSB's first (motorola format) */
#define  RIFF_DATATYPE     0x00000040l    /* sMode contains a RIFF datatype*/
#define  PITCH             0x00100000l    /* Pitch control is supported    */
#define  INPUT             0x00200000l    /* Input select is supported     */
#define  OUTPUT            0x00400000l    /* Output select is supported    */
#define  MONITOR           0x00800000l    /* Monitor is supported          */
#define  VOLUME            0x01000000l    /* Volume control is supported   */
#define  VOLUME_DELAY      0x02000000l    /* Volume delay is supported     */
#define  BALANCE           0x04000000l    /* Balance control is supported  */
#define  BALANCE_DELAY     0x08000000l    /* Balance delay is supported    */
#define  TREBLE            0x10000000l    /* Treble control is supported   */
#define  BASS              0x20000000l    /* Bass control supported        */
#define  BESTFIT_PROVIDED  0x40000000l    /* bestfit returned              */
#define  LOAD_CODE         0x80000000l    /* DSP load needed               */

/* Values for AUDIO_INIT.ulOperation    */

#define OPERATION_PLAY     1
#define OPERATION_RECORD   2
#define PLAY_AND_RECORD    3
#define  ANALYSIS          6              /* Speech Viewer/2               */
#define  DISTANCE          7              /* Speech Viewer/2               */
#define  MIGRATION         8              /* Speech Viewer/2               */

/* Values for AUDIO_INIT.sReturnCode    */

#define NO_PLAY                 1
#define NO_RECORD               2
#define NO_RECORD_AND_PLAY      3
#define INVALID_REQUEST         4
#define CONFLICT                5
#define OVERLOADED              6
#define  DOWNLEVEL_DD                  7  /* DD is down level from appl.   */
#define  DSP_LOAD_PENDING_ON_OTHER_TRK 8  /* Other trk hasn't loaded dsp   */

#define AUDIO_IGNORE                    -1

/* Values for AUDIO_INIT.sDeviceID      */

#define MINIDD                                  0
#define ACPA                    1
#define MACPA                   2
#define MPU401                  3
#define SOUND_BLASTER           4
#define IMF                     5
#define PS1                     6
#define PAS16           7

/* XLATOFF */
#pragma pack(1)
/* XLATON */

/* AUDIO_INIT structure declaration     */

typedef struct _init {  /* init */
        LONG lSRate;
        LONG lBitsPerSRate;
        LONG lBsize;
        SHORT sMode;
        SHORT sChannels;
        LONG lResolution;
        CHAR abLoadPath[LOAD_PATH];
        ULONG ulFlags;
        ULONG ulOperation;
        SHORT sReturnCode;
        SHORT sSlotNumber;
        SHORT sDeviceID;
        VOID FAR *pvReserved;       /* MMPM2 uses this to pass back sysfilenum */
        ULONG ulVersionLevel;
        } MCI_AUDIO_INIT;

typedef MCI_AUDIO_INIT FAR *LPMCI_AUDIO_INIT;

/*-------------------------------------------------------------------------*
* AUDIO_DEVID declarations and defines
*-------------------------------------------------------------------------*/

typedef struct _MCI_AUDIO_DEVID { /* devid */
        ULONG ulDevType;
        ULONG ulDevNum;
        } MCI_AUDIO_DEVID;

typedef MCI_AUDIO_DEVID FAR *LPMCI_AUDIO_DEVID;

/* Input devices        */

#define  NULL_INPUT           0
#define  STEREO_LINE_INPUT    1
#define  LEFT_LINE_INPUT      2
#define  RIGHT_LINE_INPUT     3
#define  MIC_INPUT            4
#define  BOOSTED_MIC_INPUT    5
#define  PHONE_LINE_INPUT     6
#define  HANDSET_INPUT        7
#define  SYNTH_INPUT          8
#define  DIGITAL_PHONE_LINE_INPUT 9
#define  DIGITAL_HANDSET_INPUT 10
#define  MIDI_IN_PORT         11
#define  LOOPBACK             11
#define  DEFAULT_INPUT        0XFFFFFFFF

/* Output devices       */

#define  NULL_OUTPUT          0
#define  STEREO_LINE_OUTPUT   1
#define  LEFT_LINE_OUTPUT     2
#define  RIGHT_LINE_OUTPUT    3
#define  SPEAKER_OUTPUT       4
#define  HEADSET_OUTPUT       5
#define  PHONE_LINE_OUTPUT    6
#define  HANDSET_OUTPUT       7
#define  SYNTH_OUTPUT         8
#define  DIGITAL_PHONE_LINE_OUTPUT 9
#define  DIGITAL_HANDSET_OUTPUT 10
#define  MIDI_OUT_PORT         11
#define  DEFAULT_OUTPUT        0XFFFFFFFF

/* Defined values for DevNum    */

#define  DEFAULT_DEVICE       0
#define  DEVICE_1             1
#define  DEVICE_2             2

/*-------------------------------------------------------------------------*
* Valid Return codes for the ulSupport field of MCI_AUDIO_CAPS
*-------------------------------------------------------------------------*/
#define SUPPORT_SUCCESS         0x00000000
#define UNSUPPORTED_RATE        0x00000001
#define UNSUPPORTED_CHANNELS    0x00000002
#define UNSUPPORTED_BPS         0x00000004
#define UNSUPPORTED_DATATYPE    0x00000008
#define UNSUPPORTED_OPERATION   0x00000010

typedef struct _MCI_AUDIO_CAPS { /* */
    ULONG ulLength;                 /* in     Structure length                    */
    ULONG ulSamplingRate;           /* in out Sampling rate to query              */
    ULONG ulChannels;               /* in out Channels to query                   */
    ULONG ulBitsPerSample;          /* in out BPS to query                        */
    ULONG ulDataType;               /* in out RIFF Datatype to query              */
    ULONG ulOperation;              /* in out OPERATION_PLAY or OPERATION_RECORD  */
    ULONG ulSupport;                /*    out BOOLEAN-does DD support this mode   */
    ULONG ulDataSubType;            /*    out Data-subtype to use                 */
    ULONG ulResourceUnits;          /*    out Resource units this mode            */
    ULONG ulResourceClass;          /*    out Resource class for this mode        */
    ULONG ulBlockAlign;             /*    out Block alignment for this mode.      */
    ULONG fCanRecord;               /*    out Is recording possbile - this mode   */
    ULONG ulFlags;                  /*    out                                     */
    ULONG ulCapability;             /*    out Capability of the device.           */
} MCI_AUDIO_CAPS;

typedef MCI_AUDIO_CAPS FAR *PAUDIO_CAPS;


/*-------------------------------------------------------------------------*
* Valid Return codes for the ul field of MCI_AUDIO_CAPS
*-------------------------------------------------------------------------*/
#define SUPPORT_MIX                 0x00000001 /* supports mixer functions           */
#define SUPPORT_RIFF_MODES          0x00000002 /* supports RIFF modes for AUDIO_INIT */
#define SUPPORT_CAP                 0x80000000 /* Capability IOCTL supported         */


/*-------------------------------------------------------------------------*
* AUDIO_CHANGE declarations and defines
*-------------------------------------------------------------------------*/

/* Values for AUDIO_CHANGE.lMonitor     */

#define MONITOR_OFF                     0
#define MONITOR_UNCOMPRESSED    1
#define MONITOR_COMPRESSED              2

/* Values for AUDIO_CHANGE.lInput       */

#define HIGH_GAIN_MIC           0
#define LINE_1                  1
#define LINE_2                  2
#define LINES_1AND2             3
#define LOW_GAIN_MIC            4
#define ALL_LINES               0xFFFF
#define INPUTS_LISTED                   0x5555

/* Values for AUDIO_CHANGE.lOutput      */

#define EXTERNAL_SPEAKER        1
#define INTERNAL_SPEAKER        2
#define OUTPUT_1                                4
#define OUTPUTS_LISTED                  0x5555

/* AUDIO_CHANGE structure declaration   */

typedef struct _MCI_AUDIO_CHANGE {  /* chang */
        VOID FAR *pvDevInfo;
        LONG lInput;
        LONG lOutput;
        LONG lMonitor;
        LONG lVolume;
        LONG lVolumeDelay;
        LONG lBalance;
        LONG lBalanceDelay;
        LONG lTreble;
        LONG lBass;
        LONG lPitch;
        MCI_AUDIO_DEVID rInputList[8];
        MCI_AUDIO_DEVID rOutputList[8];

        LPMCI_AUDIO_DEVID prMoreInputs;
        LPMCI_AUDIO_DEVID prMoreOutputs;
        LONG lGain;
        VOID FAR *pvModeInfo;
        } MCI_AUDIO_CHANGE;

typedef MCI_AUDIO_CHANGE FAR *LPMCI_AUDIO_CHANGE;


typedef struct _mode_info { /* mode_info */
      SHORT    sTempo;
      SHORT    sCPQN;
      SHORT    sMidiSwitches;
      SHORT    sReserved[5];
      } MIDI_INFO;

/*************************************************************************
*  MIDI Switches
***************************************************************************/
#define  MIDI_THRU_THRU 0
#define  MIDI_THRU_OUT  1


/*-------------------------------------------------------------------------*
* AUDIO_STATUS declarations and defines
*-------------------------------------------------------------------------*/

/* Values for AUDIO_STATUS.ulOperation  */

#define STOPPED                 0
#define PLAYING                 1
#define RECORDING               2
#define PLAYING_AND_RECORDING   3
#define UNITIALIZED             0xFFFFFFFF

/* AUDIO_STATUS structure declaration   */

typedef struct _status {  /* status */
        LONG lSRate;
        LONG lBitsPerSRate;
        LONG lBsize;
        SHORT sMode;
        SHORT sChannels;
        ULONG ulFlags;
        ULONG ulOperation;
        MCI_AUDIO_CHANGE rAudioChange;
        } MCI_AUDIO_STATUS;

typedef MCI_AUDIO_STATUS FAR *LPMCI_AUDIO_STATUS;

/*-------------------------------------------------------------------------*
* AUDIO_CONTROL declarations and defines
*-------------------------------------------------------------------------*/

/* Values for AUDIO_CONTROL.usIOCtlRequest      */

#define AUDIO_CHANGE            0
#define AUDIO_START             1
#define AUDIO_STOP              2
#define AUDIO_PAUSE             3
#define AUDIO_RESUME            4

/* Values for AUDIO_CONTROL.sReturnCode */

#define  AC_UNINITED          1       /* Device must be init'ed or loaded first    */
#define  FULL_QUEUE           2       /* Maximum # requests exceeded        */
#define  AC_UNPAUSED          3       /* Resume issued, but dev not paused  */
#define  INVALID_REQUEST      4       /* bad audio_control.ioctl_request    */
#define  AC_UNSTARTED         5       /* Device must be started first       */
#define  INVALID_INPUT_LIST   7       /* invalid change.input_list entry    */
#define  INVALID_OUTPUT_LIST  8       /* invalid change.output_list entry   */

/* AUDIO_CONTROL structure declaration  */

typedef struct _contr {  /* contr */
        USHORT usIOCtlRequest;
        VOID FAR *pbRequestInfo;
        ULONG ulPosition;
        SHORT sReturnCode;
        } MCI_AUDIO_CONTROL;

typedef MCI_AUDIO_CONTROL FAR *LPMCI_AUDIO_CONTROL;

/*-------------------------------------------------------------------------*
* AUDIO_BUFFER declarations and defines
*-------------------------------------------------------------------------*/

/* Values for AUDIO_BUFFER.ulFlags      */

#define AUDIO_UNDERRUN  1
#define AUDIO_OVERRUN   2

/* Values for AUDIO_BUFFER.ulPositionType       */

#define POS_MSECS       0
#define MIDI_CLOCKS     1
#define SMPTE_24       24
#define SMPTE_25       25
#define SMPTE_30DF     29
#define SMPTE_30       30

typedef struct _buffer {  /* buffer */
        ULONG ulFlags;
        ULONG ulReadBufSize;
        ULONG ulWriteBufSize;
        ULONG ulReadBufTime;
        ULONG ulWriteBufTime;
        ULONG ulReadBufMax;
        ULONG ulWriteBufMax;
        ULONG ulPosition;
        ULONG ulPositionType;
        LONG lReadBufCap;
        LONG lWriteBufCap;
        LONG lRequestBufCap;
        } MCI_AUDIO_BUFFER;

typedef MCI_AUDIO_BUFFER FAR *LPMCI_AUDIO_BUFFER;

/*-------------------------------------------------------------------------*
* AUDIO_LOAD declarations and defines
*-------------------------------------------------------------------------*/

/* Values for AUDIO_LOAD.ulFlags        */

#define LOAD_START              0x01
#define LOAD_END                0x02
#define LOAD_32BIT              0x10

typedef struct _load {  /* load */
        CHAR FAR *pbBuffer;
        ULONG ulSize;
        ULONG ulFlags;
        } MCI_AUDIO_LOAD;

typedef MCI_AUDIO_LOAD FAR *LPMCI_AUDIO_LOAD;

/*-------------------------------------------------------------------------*
* Track info declarations and defines
*-------------------------------------------------------------------------*/

/* Track Info structure declaration     */

typedef struct _info {  /* info */
        USHORT usMasterVolume;
        USHORT usDitherPct;             /* Percent of a bit dither during record  */
        USHORT usMasterVolumeDelay;
        USHORT usMasterBalance;
        USHORT usMasterBalanceDelay;
        } MCI_TRACK_INFO;

typedef MCI_TRACK_INFO FAR *LPMCI_TRACK_INFO;


/*-------------------------------------------------------------------------*
* IOBUFFER declarations and defines
*-------------------------------------------------------------------------*/

#define STARTED  1
#define PAUSED   2

typedef struct _MCI_AUDIO_IOBUFFER {  /* iobuffer */
        ULONG lSize;
        CHAR FAR *pHead;
        CHAR FAR *pTail;
        LONG lCount;
        ULONG ulPosition;
        LONG lDelay;
        USHORT usRunFlags;
        USHORT usSelInc;
        CHAR FAR *pBuffer;
        } MCI_AUDIO_IOBUFFER;

typedef MCI_AUDIO_IOBUFFER FAR *LPMCI_AUDIO_IOBUFFER;

/*-------------------------------------------------------------------------*
* AUDIO_HPI declarations and defines
*-------------------------------------------------------------------------*/

#define CBXMIT  1
#define CBREC   2
#define CRTIMER 4

#define EP_OPEN         0
#define EP_CLOSE        1
#define EP_READ         2
#define EP_WRITE        3
#define EP_INIT         4
#define EP_STATUS       5
#define EP_CONTROL      6
#define EP_BUFFER       7
#define EP_LOAD         8
#define EP_WAIT         9


typedef struct _hpi {  /* hpi */
        VOID (FAR *pvEntry)();
        VOID (FAR *pvCallBack)();
        LPMCI_AUDIO_IOBUFFER prXBuff;
        LPMCI_AUDIO_IOBUFFER prRBuff;
        USHORT usFlags;
        } MCI_AUDIO_HPI;

typedef MCI_AUDIO_HPI FAR *LPMCI_AUDIO_HPI;

/**************************/
/* AUDIO_UPDATE Structure */
/**************************/
typedef struct _audio_update { /* update */

  CHAR iobuf_type;              /* 0 - XMITIO, 1 - RECIO to be updated      */
  CHAR FAR *buffer_address;     /* address to buffer to be added to array   */
  ULONG buffer_length;          /* length of buffer to be added             */
  USHORT rc;                    /* return code                              */
  void FAR *reserved;           /* future use                               */
  } AUDIO_UPDATE;

typedef AUDIO_UPDATE FAR *UPDATE;

/* audio_update.iobuf_type definitions                                      */
#define  XMIT_IOBUF 0
#define  REC_IOBUF  1

/* audio_update.rc definitions                                              */
#define  MAX_NUM_BUFFERS_REACHED 9
#define  UPDATE_GENERAL_FAILURE  10
#define  INVALID_BUFFER_LENGTH   11

/* XLATOFF */
#pragma pack()
/* XLATON */

/* NOINC */
#ifdef __cplusplus
}
#endif
/* INC */


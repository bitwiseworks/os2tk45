/****************************************************************************/
/*                                                                          */
/* Module Name:  CDAUDIO.H                                                  */
/*                                                                          */
/* FUNCTION:  This file contains the macro definition and common record     */
/*            structures used between the CD Audio MCI Driver, its VSDs     */
/*            (Vendor Specific Drivers), and the CD look-up table,          */
/*            MMPMCD.INI.                                                   */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1991 - 1993    */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif


/*******************************************************************/
/* CD MCD and VSD values and inter-modual communications           */
/*******************************************************************/


/**********************************/
/* Macro Definitions              */
/**********************************/

#define UPC_SIZE              7     /* UPC code size, CD serial number */
#define CDMCD_CUEPOINT_MAX   20     /* maximum number of cuepoints */
#define CDROM_SPEC_START   6000     /* 2 seconds, greatest min start address */


/*************************************************************************/
/* Internal messages between the MCI Driver and the VSD.                 */
/*************************************************************************/

#define MCIDRV_REGISTER_DISC    MCI_INTERNAL_MESSAGES_START + 1
#define MCIDRV_REGISTER_DRIVE   MCI_INTERNAL_MESSAGES_START + 2
#define MCIDRV_REGISTER_TRACKS  MCI_INTERNAL_MESSAGES_START + 3
#define MCIDRV_CD_READ_LONG     MCI_INTERNAL_MESSAGES_START + 4
#define MCIDRV_CD_SET_VERIFY    MCI_INTERNAL_MESSAGES_START + 5
#define MCIDRV_CD_STATUS_CVOL   MCI_INTERNAL_MESSAGES_START + 6


/*************************************************************************/
/* Internal callback routine from the VSD to the MCI Driver.             */
/*************************************************************************/

typedef VOID (*PFNCDMCD) (DWORD, DWORD, DWORD);


/***********************************/
/* CDAudRegister record structures */
/***********************************/
#pragma pack(1)

typedef struct   /* ID a disc, used to verify a disc change                 */
{                /* must stay at 8 bytes to equal size of UPC               */
   BYTE   Mode;                           /* mode, 0=UPC, 1=ID              */
   WORD   wTrack1;                        /* address of track one in MMTIME */
   BYTE   NumTracks;                      /* number of tracks               */
   DWORD  dwLeadOut;                      /* address of lead out track      */
} MCI_CD_ID;

typedef struct /* CD-ROM Drive information, determines capabilities */
{
   WORD   wCaps;                          /* capabilities                   */
   DWORD  dwPrerollType;                  /* preroll type                   */
   DWORD  dwPrerollTime;                  /* preroll time                   */
   DWORD  dwMinStartTime;                 /* minimum starting time          */
   DWORD  dwCDMCDID;                      /* CD MCD ID for instance         */
   PFNCDMCD pCDMCDReturn;                 /* addr of CD MCD Return function */
} MCI_CD_REGDRIVE_PARMS;

/*************************************************/
/* These flags are valid for the wCaps Field     */
/*************************************************/
#define CDVSD_CAP_CAN_RECORD  0x0001    /* Can record audio               */
#define CDVSD_CAP_HAS_AUDIO   0x0002    /* Can play audio                 */
#define CDVSD_CAP_HAS_VIDEO   0x0004    /* Can play video                 */
#define CDVSD_CAP_CAN_CLSDOOR 0x0008    /* Can retract tray/close door    */
#define CDVSD_CAP_CAN_EJECT   0x0010    /* Can eject disc                 */
#define CDVSD_CAP_CAN_STREAM  0x0020    /* Can stream                     */
#define CDVSD_CAP_HAS_DAC     0x0040    /* Can process internal           */
#define CDVSD_CAP_CAN_LOCK    0x0080    /* Can disable manual eject       */
#define CDVSD_CAP_CAN_VOLUME  0x0100    /* Can manipulate volume settings */
#define CDVSD_CAP_CAN_REVERSE 0x0200    /* Can play in reverse            */
#define CDVSD_CAP_CAN_V_SPEED 0x0400    /* Can vary play speed            */
#define CDVSD_CAP_CAN_CUE     0x0800    /* Can read sequent. after break  */
                                        /* Used for Cue, Pause, and Seek  */


typedef struct /* Information about the disc  */
{
   BYTE   LowestTrackNum;                 /* lowest track number  */
   BYTE   HighestTrackNum;                /* highest track number */
   BYTE   UPC[UPC_SIZE];                  /* upc, 13 BCD + 4bit 0 */
   MCI_CD_ID DiscID;                      /* Disc ID              */
} MCI_CD_REGDISC_PARMS;

typedef struct /* Information about each track */
{
   BYTE   TrackNum;                       /* track number               */
   DWORD  dwStartAddr;                    /* starting address in MMTIME */
   DWORD  dwEndAddr;                      /* ending address             */
   BYTE   TrackControl;                   /* track control information  */
} MCI_CD_REGTRACK_REC;

typedef struct /* Track information, used to verify address */
{
   MCI_CD_REGTRACK_REC *TrackRecArr;      /* ptr to array of track recs  */
   DWORD  dwBufSize;                      /* size of buffer              */
} MCI_CD_REGTRACKS_PARMS;


/********************************************/
/* Environment settings to save and restore */
/********************************************/

typedef struct
{
   DWORD dwPosition;                      /* current position in MMTIME       */
   DWORD dwEndPlay;                       /* end play position                */
   DWORD dwMode;                          /* Play mode: playing, paused, etc. */
   DWORD dwLevel;                         /* volume levels                    */
   DWORD dwVSDData;                       /* Extra VSD data storage area      */
} MCIDRV_CD_SAVE_PARMS;

/********************************************/
/* Read Long (2352-Byte) Sectors            */
/********************************************/

/**************************************************************************/
/* Flag for the MCIDRV_CD_READ_LONG message                               */
/*   Default addressing mode for the dwFrom field is Hardware Red Book.   */
/*   Absolute sector addressing for ISO-9660, or High Sierra Group        */
/*   requires a flag, as does an MMTIME address.                          */
/**************************************************************************/

#define MCI_CD_READLONG_HSG          0x00000100L
#define MCI_CD_READLONG_MMTIME       0x00000200L

typedef struct
{
   DWORD   dwFrom;                        /* read from this position   */
   WORD    wCount;                        /* Number of sectors to read */
   LPSTR   lpstrReturn;                   /* Pointer to return buffer  */
   DWORD   dwRetSize;                     /* Return buffer size        */
} MCI_CD_READLONG_PARMS;

#pragma pack()


/*******************************************************************/
/* CD Table INI file, MMPMCD.INI, values                           */
/*******************************************************************/

/*******************************************************************/
/* Capability Flags I from the CD drive look-up table, MMPMCD.INI  */
/*******************************************************************/

#define CDHW_CAP_CAN_EJECT    0x00000001  /* Can software eject disc         */
#define CDHW_CAP_CAN_LOCK     0x00000002  /* Can lock drive/disable eject    */
#define CDHW_CAP_READS_RAW    0x00000004  /* Can read raw sectors            */
#define CDHW_CAP_CAN_WRITE    0x00000008  /* Can write to disc               */
#define CDHW_CAP_CAN_PLAY     0x00000010  /* Can play CD-DA tracks           */
#define CDHW_CAP_CAN_INTERLEV 0x00000020  /* Supports ISO-9660 interleaving  */
#define CDHW_CAP_CAN_PREFETCH 0x00000080  /* Can prefetch internally         */
#define CDHW_CAP_MANIP_AUDIO  0x00000100  /* Can manipulte audio channels    */
#define CDHW_CAP_USES_REDBOOK 0x00000200  /* Can use Red Book mode           */
#define CDHW_CAP_READS_XA     0x00000400  /* Can read CD-ROM/XA data         */
#define CDHW_CAP_CONT_READS   0x00000800  /* Continues to read DA after stop */
#define CDHW_CAP_CAN_REVERSE  0x00001000  /* Can play in reverse             */
#define CDHW_CAP_READS_CDDA   0x40000000  /* Can read CD-DA audio tracks     */


/*******************************************************************/
/* Capability Flags II from the CD drive look-up table, MMPMCD.INI */
/*******************************************************************/

#define CDHW_VOL_DEP_MASK     0x00000007  /* Volume dependency mask        */
#define CDHW_VOL_DEP_NONE              0  /* Volume without dependency     */
#define CDHW_VOL_DEP_HIGH              2  /* Volume is highest value       */
#define CDHW_VOL_DEP_LOW               3  /* Volume is lowest value        */
#define CDHW_VOL_DEP_LEFT              4  /* Volume is left volume         */
#define CDHW_VOL_DEP_RIGHT             5  /* Volume is right volume        */
#define CDHW_VOL_INDEP_MUTE   0x00000008  /* Mute has no dependency        */
#define CDHW_CAP_LOCK_MOUNT   0x00000010  /* Can only lock when mounted    */
#define CDHW_CAP_PLAY_VIDEO   0x00000020  /* Can play video                */
#define CDHW_CAP_MODAL_ONLY   0x00000040  /* Stop req to interrupt PLAY    */
#define CDHW_CAP_SUP_SEEK     0x00000080  /* Supports SEEK IOCTL           */
#define CDHW_CAP_SUP_UPC      0x00000100  /* Supports UPC IOCTL            */
#define CDHW_CAP_SUP_SUBCHAN  0x00000200  /* Supports sub-channel IOCTL    */
#define CDHW_CAP_CAN_CLS_TRAY 0x00000400  /* Can close door/retract caddy  */
#define CDHW_CAP_NO_STREAM    0x00001000  /* Cannot stream CD-DA when PDD  */
                                          /*   says it can                 */
#define CDHW_CAP_VAR_PLAY     0x80000000  /* Supports variable speeds      */

/* INI file record structure */
#define CDINI_APPSIZE                  8  /* Application name size    */
#define CDINI_KEYSIZE                 16  /* Key name size            */
#define CDINI_VSD_NAME_SIZE            8  /* size of file name        */
#define VOLUME_CONTROL               101  /* num of values, 0% - 100% */

#pragma pack(1)

typedef struct
{
   USHORT usEntryVer;                     /* Entry version                 */
   ULONG  ulCaps1;                        /* Capability flag I             */
   ULONG  ulCaps2;                        /* Capability flag II            */
   CHAR   VSDName[CDINI_VSD_NAME_SIZE];   /* DLL name for VSD              */
   ULONG  ulMinStart;                     /* Min starting addr, 0x00MMSSFF */
   USHORT usVolCnt;                       /* volume level counter          */
   USHORT ausVolValues[VOLUME_CONTROL];   /* volume control values         */
} MMPMCD_REC;

#pragma pack()


#ifdef __cplusplus
}
#endif


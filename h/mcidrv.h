/****************************************************************************/
/*                                                                          */
/* Module Name: MCIDRV.H                                                    */
/*                                                                          */
/* This is the multimedia include file that has the typedefs, defines and   */
/* function prototyes for MCI drivers.                                      */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1991, 1992     */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _MCIDRV_H
#define _MCIDRV_H

#ifndef _SSM_H_
#include <ssm.h>
#endif

typedef    USHORT   GID;

/****************************************************************************/
/*             MCI Driver  message identifiers                              */
/****************************************************************************/
#define MCIDRV_SAVE                 900
#define MCIDRV_RESTORE              901
#define MCIDRV_SYNC                 902
#define MCIDRV_CHANGERESOURCE       903
#define MCIDRV_CONNECT              904
#define MCIDRV_ASSOCIATE            905
#define MCIDRV_DISCONNECT           906
#define MCIDRV_TEST_CONNECTION      907
#define MCIDRV_START                908
#define MCIDRV_SEEK                 909
#define MCIDRV_STOP                 910
#define MCIDRV_MODIFY_CONNECTION    911
#define MCIDRV_INIT_CONNECTION      912
#define MCIDRV_DEINIT_CONNECTION    913
#define MCIDRV_ENABLE_EVENT         915
#define MCIDRV_GETTIME              917

/*********************************************************************/
/* The following messages are used by the MCIDRV_CONNECT message     */
/*********************************************************************/

#define MCIDRV_CONNECT_TARGET       0x00010000
#define MCIDRV_CONNECT_SOURCE       0x00020000
#define MCIDRV_SET_CONNNECTOR_TYPE  0x00040000
#define MCIDRV_CHANGE_SEM           0x00080000

/*********************************************************************/
/* The following messages are used by MCIDRV_MODIFY_CONNECTION       */
/*********************************************************************/


#define MCIDRV_MODIFY_10            0x00100000  // back level MCI driver.
#define MCIDRV_MIX_STANDALONE       0x00200000L


/*********************************************************************/
/* The following messages are used by MCIDRV_START                   */
/*********************************************************************/

#define MCIDRV_CUE_PLAYBACK         0x00010000
#define MCIDRV_CUE_RECORD           0x00020000
#define MCIDRV_START_PLAYBACK       0x00040000
#define MCIDRV_START_RECORD         0x00080000
#define MCIDRV_START_PREROLL        0x00100000

/*********************************************************************/
/* The following messages are used by MCIDRV_ASSOCIATE               */
/*********************************************************************/

#define MCIDRV_OUTPUT               0x00010000
#define MCIDRV_INPUT                0x00020000

/*********************************************************************/
/* The following messages are used by MCIDRV_STOP                    */
/*********************************************************************/

#define MCIDRV_PAUSE_STREAM_NETWORK   0x00010000
#define MCIDRV_FLUSH_STREAM_NETWORK   0x00020000
#define MCIDRV_DISCARD_STREAM_NETWORK 0x00040000



/*********************************************************************/
/* The following messages are used by MCIDRV_DEINIT_CONNECTION       */
/*********************************************************************/

#define MCIDRV_MODIFY_HANDLER         0x00010000



/*********************************************************************/
/* The following range of message ID's are reserved for internal use */
/*  by MCI drivers                                                   */
/*********************************************************************/
#define MCI_INTERNAL_MESSAGES_START    1000
#define MCI_INTERNAL_MESSAGES_END      1999

/*********************************************************************/
/* Flags used by the string parser for command tables                */
/* NOTE: All flags must have an "L" suffix in order to be parsed as  */
/* DWORDs by the resource compiler                                   */
/*********************************************************************/
#define MCI_COMMAND_HEAD                 0x00000001L
#define MCI_END_COMMAND                  0x00000002L
#define MCI_END_COMMAND_LIST             0x00000003L
#define MCI_RETURN                       0x00000004L
#define MCI_STRING                       0x00000005L
#define MCI_FLAG                         0x00000006L
#define MCI_INTEGER                      0x00000007L
#define MCI_CONSTANT                     0x00000008L
#define MCI_CONSTANT_INTEGER             0x00000009L
#define MCI_CONSTANT_STRING              0x0000000AL
#define MCI_END_CONSTANT                 0x0000000BL
#define MCI_DEFAULT_STRING               0x0000000CL
#define MCI_DEFAULT_INTEGER              0x0000000DL
#define MCI_RETURN_TYPE                  0x0000000EL
#define MCI_RETURN_TYPE_STRING           0x0000000FL
#define MCI_END_RETURN_TYPE              0x00000010L
#define MCI_CONSTANT_2                   0x00000011L
#define MCI_END_CONSTANT_2               0x00000012L
#define MCI_CONSTANT_PAIR                0x00000013L
#define MCI_END_CONSTANT_PAIR            0x00000014L
#define MCI_CONSTANT_INTEGER_PAIR        0x00000015L
#define MCI_CONSTANT_STRING_PAIR         0x00000016L
#define MCI_RECTL                        0x00000017L
#define MCI_CONSTANT_4                   0x00000018L
#define MCI_END_CONSTANT_4               0x00000019L
#define MCI_FOURCC                       0x0000001AL
#define MCI_OR                           0x0000001BL
#define MCI_END_OR                       0x0000001CL
#define MCI_STRING_LIST                  0x0000001DL

/*********************************************************************/
/* Return types supported by mciSendString                           */
/*   Values 0x0000 -> 0xFF00  are reserved by MMPM2                  */
/*   Values 0xFF01 -> 0xFFFF  are user definable                     */
/*********************************************************************/
#define MCI_INTEGER_RETURNED                     0x1000
#define MCI_COLONIZED2_RETURN                    0x2000
#define MCI_COLONIZED3_RETURN                    0x3000
#define MCI_COLONIZED4_RETURN                    0x4000
#define MCI_TRUE_FALSE_RETURN                    0x5000
#define MCI_ON_OFF_RETURN                        0x6000
#define MCI_DEVICENAME_RETURN                    0x7000
#define MCI_TIME_FORMAT_RETURN                   0x8000
#define MCI_SPEED_FORMAT_RETURN                  0x9000
#define MCI_MODE_RETURN                          0xA000
#define MCI_MEDIA_TYPE_RETURN                    0xB000
#define MCI_TRACK_TYPE_RETURN                    0xC000
#define MCI_CONNECTOR_TYPE_RETURN                0xD000
#define MCI_CDXA_CHANNEL_DESTINATION_RETURN      0xE000
#define MCI_PREROLL_TYPE_RETURN                  0xF000
#define MCI_FORMAT_TAG_RETURN                    0xF100
#define MCI_SEQ_SYNCHRONIZATION_RETURN           0xF200
#define MCI_VIDEO_QUALITY_RETURN                 0xF300
#define MCI_AUDIO_QUALITY_RETURN                 0xF400
#define MCI_IMAGE_QUALITY_RETURN                 0xF500
#define MCI_VIDEO_COMPRESSION_RETURN             0xF600
#define MCI_AUDIO_COMPRESSION_RETURN             0xF700
#define MCI_IMAGE_COMPRESSION_RETURN             0xF800
#define MCI_RECTL_RETURN                         0xF900
#define MCI_FOURCC_RETURN                        0xFA00
#define MCI_IMAGE_PELFORMAT_RETURN               0xFB00
#define MCI_DIRECTION_RETURN                     0xFC00
#define MCI_SIGNED_INTEGER_RETURN                0xFD00

#define MCI_USER_RETURN                          0xFF01

/*********************************************************************/
/* End of msg text used by mciGetErrorString                         */
/*********************************************************************/
#define MCIERR_MSG_TABLE_END    MCIERR_BASE + 7000

/*********************************************************************/
/*                                                                   */
/*  MCI DEVICE shareability categories                               */
/*                                                                   */
/*********************************************************************/
#define FIXEDSINGLECONTEXT            0x0001
#define DYNAMICSINGLECONTEXT          0x0002
#define LIMITEDMULTIPLECONTEXT        0x0003
#define UNLIMITEDMULTIPLECONTEXT      0x0004

/*********************************************************************/
/* MCI driver flag for close during exit list processing             */
/*********************************************************************/
#define MCI_CLOSE_EXIT                 0x10000000L

/*********************************************************************/
/* MCI driver specific error table resource number base              */
/*********************************************************************/
#define MMERROR_TABLE_BASE      500

/*********************************************************************/
/*                                                                   */
/*  MCIDRV_CHANGERESOURCE  message flags                             */
/*                                                                   */
/*********************************************************************/
#pragma pack(1)

typedef struct _MCIDRV_CHANGERESOURCE_PARMS {
   PVOID      pInstance;             /* pointer to device instance   */
   USHORT     usResourceUnits;       /* required resource units      */
   USHORT     usResourceClass;       /* resource class               */
   USHORT     usResourcePriority;    /* resource priority            */
} MCIDRV_CHANGERESOURCE_PARMS;
typedef MCIDRV_CHANGERESOURCE_PARMS  *PMCIDRV_CHANGERESOURCE_PARMS;


/*********************************************************************/
/*                                                                   */
/*  MCIDRV_ENABLE_EVENT message flags                                */
/*                                                                   */
/*********************************************************************/

typedef struct _MCIDRV_EVENT_PARMS {
   ULONG   ulLength;   /* Length of event parms                     */
   PEVCB   pevcb;      /* pointer to event control block            */
   PHEVENT phevent;    /* pointer to the event to enable in network */

} MCIDRV_EVENT_PARMS;
typedef MCIDRV_EVENT_PARMS  *PMCIDRV_EVENT_PARMS;

/*********************************************************************/
/*                                                                   */
/*  MCIDRV_SYNC  message flags                                       */
/*                                                                   */
/*********************************************************************/
#define MCIDRV_SYNC_ENABLE           0x00000100L
#define MCIDRV_SYNC_DISABLE          0x00000200L
#define MCIDRV_SYNC_REC_PULSE        0x00000400L
#define MCIDRV_SYNC_MASTER           0x00000800L
#define MCIDRV_SYNC_SET_MASTER       0x00001000L

typedef struct _MCIDRV_SYNC_PARMS {
    HSTREAM   *hStreams;        /* Pointer to list of stream IDs     */
    ULONG      ulNumStreams;    /* number of stream IDs              */
    MMTIME     mmTime;          /* Sync Pulse Time                   */
    BOOL       NonStreamMaster; /* TRUE if this device can be master */
    PSYNC_EVCB pevcbSyncPulse;  /* pointer to EVCB address (output)  */
    HID        hidMaster;       /* Master NULL sh ID (valid only on  */
                                /*    MCIDRV_SYNC_MASTER             */
    ULONG      ulReserved1;     /* Reserved field                    */
    ULONG      ulReserved2;     /* Reserved field                    */
} MCIDRV_SYNC_PARMS;
typedef MCIDRV_SYNC_PARMS *PMCIDRV_SYNC_PARMS;






/*********************************************************************/
/* contains information for open message for MCI drivers             */
/*********************************************************************/
typedef struct __MMDRV_OPEN_PARMS{
    DWORD   dwCallback;    /* call back handle                       */
    WORD    wDeviceID;     /* The device ID assigned to this instance*/
    USHORT  usDeviceType;  /* The device type number                 */
    USHORT  usDeviceOrd;   /* The device ordinal number              */
    PVOID   pInstance;     /* pointer to the instance structure allocated */
                           /* initialized by the driver. The MCI driver   */
                           /* will fill in this parameter.                */
    CHAR    szDevDLLName[260]; /* Character string containing the device  */
                               /* specific DLL name to call for the open. */
                               /* (ie. ACPA.DLL)                          */
    LPSTR   lpstrElementName;/* typically a file name or NULL             */
    USHORT  usDevParmLen;  /* Device parameters data block length.        */
    PVOID   pDevParm;      /* Device parameters data block. This data     */
                           /* block is unique to each type of device.     */
                           /* (ie. LVD "COM1 9600 N 7 1").                */
    ULONG   ulStructLength;           /* size of MMDRV_OPEN_PARMS */
    USHORT  usResourceUnitsRequired;  /* number of resource units this instance */
                                      /* requires.                              */
    USHORT  usResourceClass;          /* resource class this instance belongs to*/
    USHORT  usResourcePriority;       /* resource priority for this instance    */
    DWORD   dwParam2;                 /* Pointer to MCI_OPEN structure          */
    CHAR    szPDDName[ MAX_PDD_NAME + 1 ];  /* Name of device driver */
    CHAR    szProductInfo[MAX_PRODINFO+1];  /* Product info for the driver */
    ULONG   ulPID;
} MMDRV_OPEN_PARMS;
typedef MMDRV_OPEN_PARMS  *PMMDRV_OPEN_PARMS;


/*********************************************************************/
/*                                                                   */
/*  MCIDRV_RESTORE message flags                                     */
/*                                                                   */
/*********************************************************************/

typedef struct _MCIDRV_RESTORE_PARMS {
   ULONG   ulLength;        /* Length of restore parms               */
   ULONG   ulMasterVolume;  /* Master volume value                   */
} MCIDRV_RESTORE_PARMS;
typedef MCIDRV_RESTORE_PARMS  *PMCIDRV_RESTORE_PARMS;


#pragma pack()
DWORD APIENTRY mdmDriverNotify (WORD    wDeviceID,
                                HWND    hwnd,
                                WORD    wMsgType,
                                WORD    wUserParm,
                                DWORD   dwMsgParm);


#endif                          /* _MCIDRV_H */

#ifdef __cplusplus
}
#endif


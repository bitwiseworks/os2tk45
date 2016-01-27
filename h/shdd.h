/* NOINC */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SHDD_H_      /* notforh2inc */
/* INC */

#define _SHDD_H_ 
/***************************************************************************\
*
* Module Name: SHDD.H
*
* OS/2 2.0 Multimedia Extensions Stream Handler Device Driver Interfaces
* Block defintions.
*
* Copyright (c) International Business Machines Corporation 1990,1991
*                         All Rights Reserved
*
\***************************************************************************/

/****************************************************************************/
/*                         D E F I N E S                                    */
/****************************************************************************/

/* XLATOFF */
typedef RC (FAR *PSHDFN)        (PVOID pParmIn);
typedef RC (FAR *PDDCMDFN)      (PVOID pParmIn);
/* XLATON */
#ifdef XXXXX                                 /* Picked up by H2INC only    */
typedef PVOID PSHDFN;                        /* Because previous function  */
typedef PVOID PDDCMDFN;                      /* prototypes generate errors */
                                             /* when parsed by H2INC.      */
#endif                                       /*                            */





/****************************************************************************/
/*                       S T R U C T U R E S                                */
/****************************************************************************/

/* XLATOFF */
#pragma pack(4)
/* XLATON */

/****************************************************************************/
/****   DDCMD Interface                                                    **/
/****************************************************************************/

/* DDCMD Defines */
#define     DDCMD_SETUP         0L
#define     DDCMD_READ          1L
#define     DDCMD_WRITE         2L
#define     DDCMD_STATUS        3L
#define     DDCMD_CONTROL       4L
#define     DDCMD_REG_STREAM    5L
#define     DDCMD_DEREG_STREAM  6L

typedef struct  _ddcmd_common_parm {            /* comm  - common to all DD cmds */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;        /* data stream instance     */
                      } DDCMDCOMMON;
typedef DDCMDCOMMON    FAR     *PDDCMDCOMMON;

typedef struct  _ddcmd_setup_parm {             /* setu */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;
                        PVOID   pSetupParm;      /* see SETUP_PARM struct   */
                        ULONG   ulSetupParmSize; /* see SETUP_PARM struct   */
                      } DDCMDSETUP;
typedef DDCMDSETUP     FAR     *PDDCMDSETUP;

                /**********************************************/
                /* SETUP_PARM structure                       */
                /**********************************************/
typedef struct  _SETUP_PARM { /*  setu_parm  - DDCMDSETUP parameter block    */
                        ULONG   ulStreamTime; /* stream time in milliseconds */
                        ULONG   ulFlags;      /* various flags (input/output)*/
                        /* NOTE: new fields will be added here               */
                      } SETUP_PARM;
typedef SETUP_PARM  FAR  *PSETUP_PARM;

                /**********************************************/
                /* ulFlag defines                             */
                /**********************************************/
#define SETUP_RECURRING_EVENTS    0x00000001
                /* the device driver sets this flag on return from the     */
                /* DDCMD_SETUP command if the device driver assumes events */
                /* are recurring events---in this case, the stream handler */
                /* will not have to re-enable a recurring event each time  */
                /* the event occurs by sending a DDCMD_CONTROL command to  */
                /* the device driver.  this is useful when CUE_TIME or     */
                /* DATA_CUE events are expected to be used as RECURRING    */

typedef struct  _ddcmd_readwrite_parm {         /* read */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;
                        PVOID   pBuffer;
                        ULONG   ulBufferSize;
                        PVOID   pProcessLin;
                        BOOL    fEOS;           /* Is this the EOS buffer???*/
                        ULONG   ulParm1;        /* Reserved for future use  */
                        ULONG   ulParm2;        /* Reserved for future use  */
                        ULONG   ulLength;
                      } DDCMDREADWRITE;
typedef DDCMDREADWRITE FAR     *PDDCMDREADWRITE;
typedef PDDCMDREADWRITE FAR     *PPDDCMDREADWRITE;

typedef struct  _ddcmd_status_parm {            /* stat */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;
                        PVOID   pStatus;        /* (output)ptr to current position time */
                        ULONG   ulStatusSize;   /* (output)size of position time        */
                      } DDCMDSTATUS;
typedef DDCMDSTATUS    FAR     *PDDCMDSTATUS;

                /**********************************************/
                /* STATUS_PARM structure                      */
                /**********************************************/
typedef struct  _STATUS_PARM { /*  stat_parm  - DDCMDSTATUS parameter block                */
                        ULONG   ulTime;       /* current position time in milliseconds     */
                        /* NOTE: new fields will be added here                             */
                      } STATUS_PARM;
typedef STATUS_PARM  FAR  *PSTATUS_PARM;

typedef struct  _ddcmd_control_parm {           /* cont */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;
                        HEVENT  hEvent;         /* used for EVENTS only                 */
                        ULONG   ulCmd;
                        PVOID   pParm;          /* see CONTROL_PARM structure  */
                        ULONG   ulParmSize;     /* see CONTROL_PARM structure */
                      } DDCMDCONTROL;
typedef DDCMDCONTROL   FAR     *PDDCMDCONTROL;

                /**********************************************/
                /* ulCmd defines                              */
                /**********************************************/
                                          /* CHANGE and WAIT not implemented                   */
#define DDCMD_START         1             /* start device                                      */
#define DDCMD_STOP          2             /* stop device and return current position in pParm  */
#define DDCMD_PAUSE         3             /* pause device and return current position in pParm */
#define DDCMD_RESUME        4             /* resume device                                     */
#define DDCMD_ENABLE_EVENT  5             /* ask PDD to create this event                      */
#define DDCMD_DISABLE_EVENT 6             /* ask PDD to purge this event                       */
#define DDCMD_PAUSE_TIME    7             /* pause time keeping, but not the stream            */
#define DDCMD_RESUME_TIME   8             /* resume time keeping.                              */

                /**********************************************/
                /* CONTROL_PARM structure                     */
                /**********************************************/
typedef struct  _CONTROL_PARM { /*  cont_parm  - DDCMDCONTROL parameter block               */
                        ULONG   ulTime;       /* time in milliseconds                       */
                                              /* SH sets cuetime when ulCmd is ENABLE_EVENT */
                                              /* PDD returns current time for STOP, PAUSE   */
                        TIME_EVCB evcb;

                      } CONTROL_PARM;
typedef CONTROL_PARM  FAR  *PCONTROL_PARM;

                /*******************************************************************************/
                /* This structure is filled in by the amp mixer based on the instance          */
                /* data.                                                                       */
                /*******************************************************************************/
typedef struct  _AUDIOMODE {    /* audio mode fields for DDCMDRegister */
        LONG lSRate;            /* sample rate              */
        LONG lBitsPerSRate;     /* bits per sample          */
        SHORT sChannels;        /* Channels                 */
                    } AUDIOMODE;
typedef AUDIOMODE    FAR     *PAUDIOMODE;

typedef struct  _ddcmd_register_parm {          /* regi */
                        ULONG   ulFunction;     /* Function requested by SH                    */
                        HSTREAM hStream;        /* Stream handle needed @ interrupt time       */
                        ULONG   ulSysFileNum;   /* Device Handle so pdd can map device instance to hstream */
                        PSHDFN  pSHDEntryPoint; /* Stream handler entry point                  */
                        ULONG   ulStreamOperation; /* SH input Record or play                  */
                        SPCBKEY spcbkey;
                        ULONG   ulBufSize;      /* PDD output (optional) buffer size in bytes for SPCB */
                        ULONG   ulNumBufs;      /* PDD output (optional) # of buffers for SPCB         */
                        ULONG   ulAddressType;  /* PDD output (required) addr ptr type to data buffer  */
                        ULONG   ulBytesPerUnit; /*PDD output (required)                                */
                        MMTIME  mmtimePerUnit;  /*PDD output (required)                                */
                        E_DCB   dcbAudio;       /* SH input Device Control Block */
                        HID     hid;            /* SH input stream handler id */
                        AUDIOMODE AudioMode;    /* PPC - Audio mode data structure */
                      } DDCMDREGISTER;
typedef DDCMDREGISTER  FAR     *PDDCMDREGISTER;

                /*****************************/
                /* ulStreamOperation  defines*/
                /*****************************/
#define STREAM_OPERATION_MASK     0xC000
#define STREAM_OPERATION_CONSUME  0x8000
#define STREAM_OPERATION_PRODUCE  0xC000

                /*******************************************************************************/
                /* ulAddressType  defines                                                      */
                /* The PDD will tell the SH what type of address ptr it expects the data buffer*/
                /* to be.  The SH will then request this address type to the SSM, so that the  */
                /* SSM will send the correct type of each buffer request.                      */
                /*******************************************************************************/
#define ADDRESS_TYPE_VIRTUAL    0L
#define ADDRESS_TYPE_PHYSICAL   1L              /*default                  */
#define ADDRESS_TYPE_LINEAR     2L

typedef struct  _ddcmd_deregister_parm {        /* dere */
                        ULONG   ulFunction;     /* Function requested by SH */
                        HSTREAM hStream;        /* Stream handle needed @ interrupt time */
                      } DDCMDDEREGISTER;
typedef DDCMDDEREGISTER FAR    *PDDCMDDEREGISTER;



/****************************************************************************/
/**** RING 0 Stream Handler SHD Interfaces                                ***/
/****************************************************************************/

/* RING 0 Stream Handler  SHD's:                                            */
#define     SHD_REPORT_INT      0L      /* PDD reports interrupt has arrived*/
#define     SHD_REPORT_EVENT    1L      /* PDD reports cuetime has arrived  */

typedef struct  _shd_common_parm {              /* shdc */
                        ULONG   ulFunction;     /* Function requested by PDD               */
                        HSTREAM hStream;        /* Stream handle needed @ interrupt time   */
                        }SHD_COMMON;
typedef SHD_COMMON      FAR  *PSHD_COMMON;

typedef struct  _shd_reportint_parm {           /* repi */
                        ULONG   ulFunction;     /* Function requested by PDD*/
                        HSTREAM hStream;        /* so SH knows which stream to process     */
                        PVOID   pBuffer;        /* return ptr to last used buffer          */
                        ULONG   ulFlag;         /* reason for interrupt     */
                        ULONG   ulStatus;       /* rc or bytes read/written */
                        ULONG   ulStreamTime;   /* time in milliseconds of stream position  */
                      } SHD_REPORTINT;
typedef SHD_REPORTINT   FAR  *PSHD_REPORTINT;

                /*******************/
                /* ulFlag settings */
                /*******************/
#define ERROR               0x80L
#define STREAM_STOP_NOW     0x40L
#define SHD_READ_COMPLETE   1L
#define SHD_WRITE_COMPLETE  2L

typedef struct  _shd_reportevent_parm {         /* repe */
                        ULONG   ulFunction;     /* Function requested by PDD*/
                        HSTREAM hStream;        /* so SH knows which stream to process     */
                        HEVENT  hEvent;         /* event handle pass back to stream handler*/
                        ULONG   ulStreamTime;   /* time in milliseconds of stream position  */
                      } SHD_REPORTEVENT;
typedef SHD_REPORTEVENT FAR  *PSHD_REPORTEVENT;



/****************************************************************************/
/**** RING 3 Stream Handler SHD Interfaces                                ***/
/****************************************************************************/

/* RING 3 Stream Handler  SHD's: (Uses SpiSendMsg as interface) */
#define     SHC_REPORT_INT      0x80000000L   /* ulMsgType for SHD_REPORT_INT */
#define     SHC_REPORT_EVENT    0x80000001L   /* ulMsgType for SHD_REPORT_EVENT */

/**********************************************
 *
 * MSG_REPORTINT Control Block.
 *  pMsg field of SpiSendMsg api
 *
 **********************************************/
typedef struct  _MSG_REPORTINT { /* msgrepi */
   ULONG   ulMsgLen;       /* Length of structure                     */
   PVOID   pBuffer;        /* return ptr to last used buffer          */
   ULONG   ulFlag;         /* reason for interrupt                    */
   ULONG   ulStatus;       /* rc or bytes read/written                */
   ULONG   ulStreamTime;   /* time in milliseconds of stream position */
   } MSG_REPORTINT, FAR  *PMSG_REPORTINT;

   /* For ulFlag defines, refer to the _shd_reportint_parm structure */



/**********************************************
 *
 * MSG_REPORTEVENT Control Block.
 *  pMsg field of SpiSendMsg api
 *
 **********************************************/
typedef struct  _MSG_REPORTEVENT { /* msgrepe */
   ULONG   ulMsgLen;       /* Length of structure                      */
   HEVENT  hevent;         /* event handle pass back to stream handler */
   ULONG   ulStreamTime;   /* time in milliseconds of stream position  */
   } MSG_REPORTEVENT, FAR  *PMSG_REPORTEVENT;



/* XLATOFF */
#pragma pack()
/* XLATON */

/****************************************************************************/
/*              F U N C T I O N   P R O T O T Y P E S                       */
/****************************************************************************/

/* XLATOFF */
RC  FAR         SHDEntryPoint(PSHD_COMMON pCommon);
RC  FAR         DDCMDEntryPoint(PDDCMDCOMMON pCommon);
/* XLATON  */

/* NOINC */
#endif                /* notforh2inc */

#ifdef __cplusplus
}
#endif
/* INC */            

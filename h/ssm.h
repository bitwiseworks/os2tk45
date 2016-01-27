/* NOINC */
#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************/
/*                                                                          */
/* Module Name: SSM.H                                                       */
/*                                                                          */
/* OS/2 Multimedia Extensions Sync/Stream Manager Definitions file          */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1991-1994      */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/

#ifndef _SSM_H_
#define _SSM_H_
/* INC */

#define MAX_SPI_NAME  9      /* Maximum ASCIIZ stream handler name/class  */
                             /* and device driver name                    */


/* XLATOFF */
#pragma pack(4)
/* XLATON */

/****************************************************************************/
/*                                                                          */
/*  Global Type Declarations                                                */
/*                                                                          */
/****************************************************************************/
typedef ULONG  HSTREAM;               /* Stream Handle                      */
typedef HSTREAM FAR *PHSTREAM;        /* Pointer to Stream Handle           */

typedef ULONG  HNETWORK;              /* Network Handle                     */
typedef HNETWORK FAR *PHNETWORK;      /* Pointer to Network Handle          */

typedef ULONG  HEVENT;                /* Event Handle                       */
typedef HEVENT FAR *PHEVENT;          /* Pointer to Event Handle            */

typedef ULONG  HID;                   /* Stream Handler ID                  */
/* The following define was deleted because it conflicts with an OS/2 PM define */
/*typedef HID  FAR *PHID;                Pointer to Stream Handler ID       */

typedef struct _LOCKH {               /* Memory locking handle (used by PDD)*/
   BYTE     lock[16];
   } LOCKH;
typedef LOCKH           FAR  *PLOCKH;            /* Ptr to a LOCKH        */

#define RC ULONG                      /* Used for all return codes          */

typedef struct  _SLAVE { /* slave */
   HSTREAM       hstreamSlave;          /* Handle for this slave            */
   MMTIME        mmtimeStart;           /* Offset in master stream time to start */
   } SLAVE;                             /*  this slave.                     */
typedef SLAVE           FAR  *PSLAVE;            /* Ptr to a SLAVE        */

typedef struct  _MASTER { /* master */
   HSTREAM       hstreamMaster;         /* Handle for this master           */
   } MASTER;
typedef MASTER           FAR  *PMASTER;            /* Ptr to a MASTER       */

/* XLATOFF */
#pragma pack(1)
/* XLATON */

typedef struct  _HAND { /* hand */
   SZ            szHandlerClass[MAX_SPI_NAME]; /* Stream handler class      */
   SZ            szHandlerName[MAX_SPI_NAME];  /* Stream handler name       */
   } HAND;
typedef HAND            FAR  *PHAND;             /* Ptr to a HAND         */

/* XLATOFF */
#pragma pack()
/* XLATON */


#include <spcb.h>
#include <evcb.h>
#include <acb.h>
#include <dcb.h>


/* XLATOFF */
/****************************************************************************/
/*                                                                          */
/*  Function Prototypes for Application event entry routine, SHC entry      */
/*  routine, SMH entry routine. All use the Pure C calling convention       */
/*                                                                          */
/****************************************************************************/
typedef ULONG (APIENTRY EVFN) (PEVCB  pevcb);
typedef EVFN *PEVFN;

typedef ULONG (APIENTRY SHCFN) (PVOID pParmIn);
typedef SHCFN *PSHCFN;

typedef ULONG (APIENTRY SMHFN) (PVOID pParmIn);
typedef SMHFN *PSMHFN;

/* XLATON */
#ifdef XXXXX
typedef PVOID PEVFN;
typedef PVOID PSHCFN;
typedef PVOID PSMHFN;
#endif


#ifdef CONNECTION
#include <concb.h>
#endif


/******************
 * ulNetworkState:
 ******************/
#define  NET_ACTIVE_MASK       0x00000FFFL    /* Network running mask */
#define  NET_RUNNING           0x00000000L    /* Network is running */
#define  NET_PAUSED            0x00000001L    /* Network is paused */
#define  NET_STOPPED           0x00000002L    /* Network is stopped (flush/discard) */
#define  NET_PAUSE_PENDING     0x00000004L    /* Network is pending a pause stop */
#define  NET_FLUSH_PENDING     0x00000010L    /* Network is pending a flush stop */
#define  NET_DISCARD_PENDING   0x00000020L    /* Network is pending a discard stop */
#define  NET_CLOSE_PENDING     0x00000040L    /* Network is pending a CLOSE */
#define  NET_SEEK_PENDING      0x00000040L    /* Network is pending a seek operation */
#define  NET_OPEN              0x00001000L    /* Network has been opened */
#define  NET_ASSEMBLED         0x00004000L    /* Network has been assembled */


/********************
 * ulActivationState:
 ********************/
#define  STREAM_ACTIVE           0x00000010L    /* Stream is active */
#define  STREAM_NOT_ACTIVE       0x00000020L    /* Stream is not active */



/****************************************************************************/
/* INI file support defines                                                 */
/****************************************************************************/
/* RCDATA defines:                                                          */
#define SPI_RESOURCE   1               /* RCDATA resource number            */
#define SH_DLL_TYPE    0               /* DLL type stream handler           */
#define SH_PDD_TYPE    1               /* PDD type stream handler           */
#define SPCBSIZE       72L             /* SPCB length, the Resource compiler*/
                                       /*   doesn't allow sizeof(SPCB).     */

/****************************************************************************/
/*                                                                          */
/*  Function Prototypes for SPI functions                                   */
/*  All use Pure C calling convention                                       */
/*  Six major categories:                                                   */
/*     - stream creation                                                    */
/*     - stream control                                                     */
/*     - event handling                                                     */
/*     - sync handling                                                      */
/*     - configuration                                                      */
/*     - stream protocol programming                                        */
/****************************************************************************/


/* XLATOFF */

/****************************************************************************/
/* Stream Creation                                                          */
/****************************************************************************/

ULONG APIENTRY SpiAssociate ( HSTREAM hstream,
                              HID hid,
                              PACB pacb );


ULONG APIENTRY SpiEnumerateHandlers ( PHAND pahand,       /* ptr to array handler name/class */
                                      PULONG pulNumHand );


ULONG APIENTRY SpiGetHandler ( PSZ pszHName,
                               HID *phidSrc,
                               HID *phidTgt );

ULONG APIENTRY SpiCreateStream ( HID hidSrc,        /* input                   */
                                 HID hidTgt,        /* input                   */
                                 PSPCBKEY pspcbkey, /* input                   */
                                 PDCB pdcbSrc,      /* input - Src device info */
                                 PDCB pdcbTgt,      /* input - Tgt device info */
                                 PIMPL_EVCB pevcb,  /* input                   */
                                 PEVFN EventEntry,  /* input                   */
                                 HSTREAM hstreamBuf,/* input                   */
                                 PHSTREAM phstream, /* output                  */
                                 PHEVENT phevent ); /* output - implicit hevent*/

ULONG APIENTRY SpiSendMsg ( HSTREAM hstream,
                            HID hid,
                            ULONG ulMsgType,
                            PVOID pMsg );

/* XLATON */
/* The first thing in the pMsg must be the length of the structure! */
typedef struct _MSG_COMMON { /* msg */
     ULONG   ulMsgLen;              /* length of structure */
   } MSG_COMMON;
/* XLATOFF */


/****************************************************************************/
/* Stream Control                                                           */
/****************************************************************************/


ULONG APIENTRY SpiDestroyStream ( HSTREAM hstream );


ULONG APIENTRY SpiGetTime ( HSTREAM hstream,
                            PMMTIME pmmtime );

ULONG APIENTRY SpiSeekStream ( HSTREAM hstream,
                               ULONG ulFlags,
                               LONG  lSeekPoint );  /*mmtime or other          */

/* SpiSeekStream() ulFlags defines:                                         */
#define SPI_SEEK_ABSOLUTE   0x0000L         /* Seek from beginning of stream*/
#define SPI_SEEK_RELATIVE   0x0001L         /* Seek from current location   */
#define SPI_SEEK_FROMEND    0x0002L         /* TEMP                         */
#define SPI_SEEK_SLAVES     0x0010L         /* Seek all slave streams       */

#define SPI_SEEK_MMTIME     0x0000L         /* lSeekPoint == mmtime (default)*/
#define SPI_SEEK_BYTES      0x0100L         /* lSeekPoint == bytes          */

#define SPI_SEEK_IFRAME     0x1000L         /* Seek to nearest IFRAME       */

#define VALIDSEEKFLAGS      ( SPI_SEEK_RELATIVE |           \
                              SPI_SEEK_ABSOLUTE |           \
                              SPI_SEEK_FROMEND  |           \
                              SPI_SEEK_MMTIME |             \
                              SPI_SEEK_SLAVES |             \
                              SPI_SEEK_IFRAME |             \
                              SPI_SEEK_BYTES )


ULONG APIENTRY SpiStartStream ( HSTREAM hstream,
                                ULONG ulFlags );

/* SpiStartStream() ulFlags defines:                                        */
#define SPI_START_STREAM    0x0000L         /* Only start this one stream   */
#define SPI_START_SLAVES    0x0001L         /* Start this master and any slaves */
#define SPI_START_PREROLL   0x0002L         /* Start the stream, but do preroll */

#define VALIDSTARTFLAGS     ( SPI_START_STREAM |            \
                              SPI_START_SLAVES |            \
                              SPI_START_PREROLL )


ULONG APIENTRY SpiStopStream ( HSTREAM hstream,
                               ULONG ulFlags );

/* XLATON */
/* SpiStopStream() ulFlags defines:                                         */
#define SPI_STOP_STREAM    0x0000L         /* Only stop this one stream     */
                                           /* The default is to pause the data          */
                                           /*  stream. No change to data buffers.       */
                                           /*  synchronous call (no event notification) */
#define SPI_STOP_SLAVES    0x0001L         /* Stop this master and any slaves           */
#define SPI_STOP_FLUSH     0x0002L         /* Stop and flush all filled buffers.        */
                                           /*  Stops the Src & Tgt SH, lets the         */
                                           /*  Tgt SH run until out of buffers          */
                                           /*  thereby, "flushing" all buffers.         */
                                           /*  Tgt SH stops when out of buffers         */
                                           /*  (This is a ASYNCHRONOUS call, event      */
                                           /*   notification sent to appl when done.)   */
#define SPI_STOP_DISCARD   0x0004L         /* Stop and discard all filled buffers.      */
                                           /*  Stops the Src & Tgt SH                   */
                                           /*  then discards all buffers current for    */
                                           /*  this stream.                             */
                                           /*  (This is a ASYNCHRONOUS call, event      */
                                           /*   notification sent to appl when done.)   */

#define SPI_STOP_PAUSE     (SPI_STOP_STREAM)

/* XLATOFF */
#define VALIDSTOPFLAGS     ( SPI_STOP_STREAM |              \
                             SPI_STOP_SLAVES |              \
                             SPI_STOP_FLUSH |               \
                             SPI_STOP_DISCARD )


/****************************************************************************/
/* Event Handling                                                           */
/****************************************************************************/


ULONG APIENTRY SpiDisableEvent ( HEVENT hevent );


ULONG APIENTRY SpiEnableEvent ( PEVCB pevcb,
                                PHEVENT phevent );


/****************************************************************************/
/* Sync Handling                                                            */
/****************************************************************************/


ULONG APIENTRY SpiDisableSync  ( HSTREAM hstreamMaster );


ULONG APIENTRY SpiEnableSync ( HSTREAM hstreamMaster,
                               PSLAVE paslaveList,
                               ULONG ulNumSlaves,
                               MMTIME mmtimeSync );


ULONG APIENTRY SpiDetermineSyncMaster ( PHSTREAM phstreamMaster,
                                        PMASTER  paMasterList,
                                        ULONG    ulNumMasters );


/****************************************************************************/
/* Protocol manipulation                                                    */
/****************************************************************************/

ULONG APIENTRY SpiGetProtocol ( HID hid,
                                PSPCBKEY pspcbkey,  /* input key               */
                                PSPCB pspcb );      /* output SPCB             */


ULONG APIENTRY SpiInstallProtocol ( HID hid,
                                    PSPCBKEY pspcbkey,  /* input key           */
                                    PSPCB pspcb ,       /* input SPCB          */
                                    ULONG ulFlags );    /* install flags       */

/* SpiInstallProtocol() ulFlags defines:                                    */
#define SPI_INSTALL_PROTOCOL   0x0000L         /* Install this protocol     */
#define SPI_DEINSTALL_PROTOCOL 0x0001L         /* DeInstall this protocol   */

#define VALIDINSTALLFLAGS     (SPI_INSTALL_PROTOCOL | SPI_DEINSTALL_PROTOCOL)

ULONG APIENTRY SpiEnumerateProtocols ( HID hid,
                                       PVOID    paSPCBKeys,        /* ptr to buffer to fill with SPCB keys */
                                       PULONG   pulNumSPCBKeys );  /* # of entries buffer on input,        */
                                                               /*  # of SPCB key on output.            */


/* XLATON */
/****************************************************************************/
/*                                                                          */
/*  Function Prototypes for SMH functions                                   */
/*  All use Pure C calling convention                                       */
/* NOTE ABOUT POINTERS:                                                     */
/*  0:32 linear addresses for ring 3 (DLL) stream handlers                  */
/*  16:16 selector/offset addresses for ring 0 (PDD) stream handlers        */
/****************************************************************************/

ULONG APIENTRY SMHEntryPoint ( PVOID pParmIn );

typedef struct _SMH_COMMON { /* smhfunc */
      ULONG    ulFunction;        /* SMH command function                   */
      HID      hid;               /* handler ID                             */
   } SMH_COMMON;
typedef SMH_COMMON      FAR  *PSMH_COMMON;       /* Ptr to common part    */


/****************************************************************************/
#define SMH_REGISTER           0L              /* (Valid only from ring 3 stream */
                                               /*  handler)                 */
     typedef struct  _PARM_REG { /* smhreg */
        ULONG     ulFunction;                  /* SMH command function      */
        PSZ       pszSHName;                   /* Max size = 8 characters   */
        HID FAR  *phidSrc;                     /* (9 including null)        */
        HID FAR  *phidTgt;
        PSHCFN    pshcfnEntry;                 /* Handler entry point for SHC's  */
                                               /*  Only used for Ring 0 handlers.*/
        ULONG     ulFlags;
        ULONG     ulMaxNumStreams;             /* Max # of streams          */
        ULONG     ulMaxNumEvents;              /* Max # of events/stream    */
        } PARM_REG;
typedef PARM_REG        FAR  *PPARM_REG;

#define REGISTER_TGT_HNDLR     0x0001L         /* This handler is Target.   */
#define REGISTER_SRC_HNDLR     0x0002L         /* This handler is Source.   */
                                               /* Handlr can be both.       */
#define REGISTER_NONSTREAMING  0x0004L         /* Handler is non-streaming. */
#define VALIDREGISTERFLAGS     (REGISTER_TGT_HNDLR | REGISTER_SRC_HNDLR | REGISTER_NONSTREAMING)


/* Notes:                                                                     */
/*  - Since ring 0 (DD) stream handlers can not use the IDC interface to the  */
/*    sync/stream manager during DD initialization time, it must use the IOCTL*/
/*    function to perform the SMH_REGISTER function. The addresses are 16:16  */
/*    for this IOCTl instead of 0:32, because it comes form a device driver.  */
#define SSMDD_CATEGORY            0x0081    /* Category (For ring 0 stream handler) */
#define IOCTL_FUNC_SMH_REGISTER   0x0040    /* Function (For ring 0 stream handler) */


/****************************************************************************/
#define SMH_DEREGISTER         1L              /* SMH_Deregister()          */
     typedef struct  _PARM_DEREG { /* smhder */
        ULONG     ulFunction;                  /* SMH command function      */
        PSZ       pszSHName;                   /* Max size = 8 char, 9 includes */
        } PARM_DEREG;                          /* the null.                 */
typedef PARM_DEREG      FAR  *PPARM_DEREG;


/****************************************************************************/
#define SMH_REPORTEVENT        2L              /* SMH_ReportEvent()         */
     typedef struct  _PARM_EVENT { /* smhrev */
        ULONG     ulFunction;                  /* SMH command function      */
        HID       hid;
        HEVENT    hevent;                      /* Used only for explicit events   */
                                               /*  Must be 0 for sync and implicit*/
                                               /*  events (i.e. error events)     */
        PEVCB     pevcbEvent;                  /* Event status & HSTREAM          */
        } PARM_EVENT;
typedef PARM_EVENT      FAR  *PPARM_EVENT;


/****************************************************************************/
#define SMH_NOTIFY             3L              /* SMH_Notify()              */
     typedef struct  _PARM_NOTIFY { /* smhnot */
        ULONG     ulFunction;       /* Input - SMH command function         */
        HID       hid;              /* Input                                */
        HSTREAM   hstream;          /* Input                                */
        ULONG     ulFlags;          /* Input/Output                         */
        ULONG     ulGetNumEntries;  /* Input/Output - # of entries in table */
        ULONG     ulRetNumEntries;  /* Input/Output - # of entries in table */
        PVOID     pGetBufTab;       /* Output - Ptr to buffer/record table  */
        PVOID     pRetBufTab;       /* Input - Ptr to buffer/record table   */
        } PARM_NOTIFY;
typedef PARM_NOTIFY     FAR  *PPARM_NOTIFY;

     typedef struct _SRCBUFTAB { /* srcbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        PVOID    pRecord;      /* Ptr to record in buffer (split-streams only)  */
        ULONG    ulLength;     /* Max buf length on GetEmpty, Filled        */
                               /* (actual) rec/buf length on ReturnFull.    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        } SRCBUFTAB;
typedef SRCBUFTAB       FAR  *PSRCBUFTAB;        /* Ptr to a buffer entry */

     typedef struct _TGTBUFTAB { /* tgtbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        ULONG    ulBufId;      /* Buffer ID. Passed to SH on GetFull, must  */
                               /* be passed back to SSM on ReturnEmpty.     */
        ULONG    ulLength;     /* Filled (actual) buffer length on GetFull, */
                               /* unused on ReturnEmpty.                    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        } TGTBUFTAB;
typedef TGTBUFTAB       FAR  *PTGTBUFTAB;        /* Ptr to a buffer entry */

/* EXTENDED SMH_NOTIFY Structures */

     typedef struct  _PARM_ENOTIFY { /* smhenot */
        ULONG     ulFunction;       /* Input - SMH command function         */
        HID       hid;              /* Input                                */
        HSTREAM   hstream;          /* Input                                */
        ULONG     ulFlags;          /* Input/Output                         */
        ULONG     ulGetNumEntries;  /* Input/Output - # of entries in table */
        ULONG     ulRetNumEntries;  /* Input/Output - # of entries in table */
        PVOID     pGetBufTab;       /* Output - Ptr to buffer/record table  */
        PVOID     pRetBufTab;       /* Input - Ptr to buffer/record table   */
        ULONG     ulParm1;          /* Reserved for future use */
        ULONG     ulParm2;          /* Reserved for future use */
        } PARM_ENOTIFY;
typedef PARM_ENOTIFY     FAR  *PPARM_ENOTIFY;

     typedef struct _ESRCBUFTAB { /* esrcbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        PVOID    pRecord;      /* Ptr to record in buffer (split-streams only)  */
        ULONG    ulLength;     /* Max buf length on GetEmpty, Filled        */
                               /* (actual) rec/buf length on ReturnFull.    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        ULONG    ulParm1;      /* Record or buffer specific data */
        ULONG    ulParm2;      /* Record or buffer specific data */
        } ESRCBUFTAB;
typedef ESRCBUFTAB       FAR  *PESRCBUFTAB;        /* Ptr to a buffer entry */

     typedef struct _ETGTBUFTAB { /* etgtbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        ULONG    ulBufId;      /* Buffer ID. Passed to SH on GetFull, must  */
                               /* be passed back to SSM on ReturnEmpty.     */
        ULONG    ulLength;     /* Filled (actual) buffer length on GetFull, */
                               /* unused on ReturnEmpty.                    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        ULONG    ulParm1;      /* Buffer specific data */
        ULONG    ulParm2;      /* Buffer specific data */
        } ETGTBUFTAB;
typedef ETGTBUFTAB       FAR  *PETGTBUFTAB;        /* Ptr to a buffer entry */

     typedef struct _EPSRCBUFTAB { /* epsrcbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        PVOID    pRecord;      /* Ptr to record in buffer (split-streams only)  */
        ULONG    ulLength;     /* Max buf length on GetEmpty, Filled        */
                               /* (actual) rec/buf length on ReturnFull.    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        ULONG    ulParm1;      /* Record or buffer specific data */
        ULONG    ulParm2;      /* Record or buffer specific data */
        PVOID    pProcessLin;  /* Process Linear record ptr */
        } EPSRCBUFTAB;
typedef EPSRCBUFTAB       FAR  *PEPSRCBUFTAB;       /* Ptr to a buffer entry */

     typedef struct _EPTGTBUFTAB { /* eptgtbuftab */
        PVOID    pBuffer;      /* Ptr to buffer                             */
        ULONG    ulBufId;      /* Buffer ID. Passed to SH on GetFull, must  */
                               /* be passed back to SSM on ReturnEmpty.     */
        ULONG    ulLength;     /* Filled (actual) buffer length on GetFull, */
                               /* unused on ReturnEmpty.                    */
        ULONG    ulMessageParm; /* Message to passed to application         */
        MMTIME   mmtimeOffset; /* mmtime offset from beginning of buffer    */
        ULONG    ulParm1;      /* Buffer specific data */
        ULONG    ulParm2;      /* Buffer specific data */
        PVOID    pProcessLin;  /* Process Linear buffer ptr */
        } EPTGTBUFTAB;
typedef EPTGTBUFTAB       FAR  *PEPTGTBUFTAB;       /* Ptr to a buffer entry */


/*SMH_ulFlags:                                                              */
#define BUF_GETEMPTY       0x00000001L   /* Ptr returned in SMH_pGetBuffer  */
#define BUF_RETURNFULL     0x00000002L   /* Ptr passed in SMH_pRetBuffer    */
#define BUF_GETFULL        0x00000004L   /* Ptr returned in SMH_pGetBuffer  */
#define BUF_RETURNEMPTY    0x00000008L   /* Ptr passed in SMH_pRetBuffer    */
#define BUF_GIVEBUF        0x00000010L   /* Ptr passed in SMH_pRetBuffer    */
#define BUF_EOS            0x00000100L   /* End of stream. Source sets this on */
                                         /*  last buffer. Target sets it after */
                                         /*  consuming the laset buffer.       */
#define BUF_RECORDS        0x00004000L   /* One or more records (Split-stream) */
#define BUF_LASTRECORD     0x00008000L   /* (BUF_RETURNFULL only) Mark         */
                                         /* these record(s) as being the       */
                                         /* last in the buffer. So the SSM     */
                                         /* knows the maximum # of records     */
                                         /* produced for this buffer.          */
                                         /* (Split stream only)                */
/*SMH_ulFlags:                                                                   */
#define BUF_LINEAR         0x00010000L   /* Ptrs are Global linear               */
#define BUF_PHYSICAL       0x00020000L   /* Ptrs are physical                    */
                                         /* (valid only for phyically            */
                                         /*  contiguous buffers)                 */
#define BUF_RESERVED       0x00040000L   /* Reserved                             */
#define BUF_GDT            0x00080000L   /* Ptrs are GDT sel:offset              */
#define BUF_EXTENDED       0x80000000L   /* Use Extended SMH_NOTIFY structures   */
#define BUF_EXTENDEDPTR    0x40000000L   /* Use Extendedptr SMH_NOTIFY structures*/
                                         /*  Implies BUF_EXTENDED                */

#define VALIDSOURCEFLAGS (BUF_RETURNFULL|BUF_GETEMPTY|BUF_EOS|BUF_LASTRECORD|BUF_PHYSICAL|BUF_LINEAR|BUF_GDT|BUF_RECORDS|BUF_GIVEBUF|BUF_EXTENDED|BUF_EXTENDEDPTR)
#define VALIDTARGETFLAGS (BUF_RETURNEMPTY|BUF_GETFULL|BUF_PHYSICAL|BUF_LINEAR|BUF_GDT|BUF_EOS|BUF_EXTENDED|BUF_EXTENDEDPTR)


/****************************************************************************/
#define SMH_LOCKMEM            4L              /* SMH_LockMem()             */
     typedef struct  _PARM_LOCKM { /* smhloc */
        ULONG     ulFunction;                  /* SMH command function      */
        PVOID     pBuffer;
        ULONG     ulBufSize;
        PLOCKH    plockh;
        ULONG     ulFlags;
        } PARM_LOCKM;
typedef PARM_LOCKM      FAR  *PPARM_LOCKM;

/*SMH_ulFlags:                                                              */
#define SSM_LOCKMEM            0x0000L         /* Lock Memory.              */
#define SSM_UNLOCKMEM          0x0001L         /* Unlock memory.            */
#define SSM_CONTIGLOCK         0x0002L         /* Lock memory contiguously. */
#define SSM_LOCKMEM_RESERVED1  0x0100L         /* reserved                  */
#define SSM_LOCKMEM_RESERVED2  0x0200L         /* reserved                  */
/****************************************************************************/



/****************************************************************************/
/*                                                                          */
/*  Function Prototypes for SHC functions (Stream Handler Functions)        */
/*  All use Pure C calling convention                                       */
/*                                                                          */
/* NOTE ABOUT POINTERS:                                                     */
/*  0:32 linear addresses for ring 3 (DLL) stream handlers                  */
/*  16:16 selector/offset addresses for ring 0 (PDD) stream handlers        */
/****************************************************************************/
typedef struct _SHC_COMMON { /* shcfunc */
      ULONG    ulFunction;        /* Handler command function               */
      HID      hid;               /* handler ID                             */
   } SHC_COMMON;
typedef SHC_COMMON      FAR  *PSHC_COMMON;       /* Ptr to common part    */


/****************************************************************************/
#define SHC_ASSOCIATE             0L
     typedef struct _PARM_ASSOC { /* shcass */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        PACB     pacb;              /* pointer to associate control block   */
     } PARM_ASSOC;
typedef PARM_ASSOC      FAR  *PPARM_ASSOC;


/****************************************************************************/
#define SHC_CLOSE                 1L
     typedef struct _PARM_CLOSE { /* shcclo */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
     } PARM_CLOSE;
typedef PARM_CLOSE      FAR  *PPARM_CLOSE;


/****************************************************************************/
#define SHC_CREATE                2L
     typedef struct _PARM_CREATE { /* shccrt */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        SPCBKEY  spcbkey;           /* type of stream to create             */
        PSPCB    pspcb;             /* Pointer to SPCB from handler         */
        HSTREAM  hstreamBuf;        /* Used for split streams or to associate*/
                                    /* another stream with this stream.     */
        PDCB     pdcb;              /* pointer to device control block      */
     } PARM_CREATE;
typedef PARM_CREATE     FAR  *PPARM_CREATE;


/****************************************************************************/
#define SHC_DESTROY               3L
     typedef struct _PARM_DESTROY { /* shcdes */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
     } PARM_DESTROY;
typedef PARM_DESTROY    FAR  *PPARM_DESTROY;


/****************************************************************************/
#define SHC_START                 4L
     typedef struct _PARM_START { /* shcstr */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        ULONG    ulFlags;           /* Start flags                          */
     } PARM_START;
typedef PARM_START      FAR  *PPARM_START;

/* ulFlags:                                                                 */
#define FLUSH_START      0x0001L    /* This start is a result of a Flush Stop command. */


/****************************************************************************/
#define SHC_STOP                  5L
     typedef struct _PARM_STOP { /* shcstp */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        ULONG    ulFlags;           /* Stop flags (refer to ulFlags defines under */
                                    /*  SpiStopStream() call.               */
     } PARM_STOP;
typedef PARM_STOP       FAR  *PPARM_STOP;


/****************************************************************************/
#define SHC_SEEK                  6L
     typedef struct _PARM_SEEK { /* shcsek */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        ULONG    ulFlags;           /* Seek flag (refer to ulFlags defines  */
                                    /*  under SpiSeekStream() call).        */
        LONG     lSeekPoint;        /* seek to point , mmtime or other      */
     } PARM_SEEK;
typedef PARM_SEEK       FAR  *PPARM_SEEK;


/****************************************************************************/
#define SHC_ENABLE_EVENT          7L
     typedef struct _PARM_ENEVENT { /* shceev */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HEVENT   hevent;            /* handle of event to enable            */
        PEVCB    pevcbUser;         /* User event information (e.g. hstream */
                                    /*  EventType, EventSubType)            */
     } PARM_ENEVENT;
typedef PARM_ENEVENT    FAR  *PPARM_ENEVENT;


/****************************************************************************/
#define SHC_DISABLE_EVENT         8L
     typedef struct _PARM_DISEVENT { /* shcdev */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle to stream for this event      */
        HEVENT   hevent;            /* handle of event to disable           */
     } PARM_DISEVENT;
typedef PARM_DISEVENT   FAR  *PPARM_DISEVENT;


/****************************************************************************/
#define SHC_ENABLE_SYNC           9L
     typedef struct _PARM_ENSYNC { /* shcesy */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        ULONG    ulFlags;           /* Sync flags                           */
        MMTIME   mmtimeSync;        /* granularity of sync interval (if null, use default) */
        PSYNC_EVCB pevcbSyncPulse;  /* Sync pulse EVCB information          */
        ULONG    ulSyncPulseSem;    /* Optional 16 bit system               */
                                    /* Semaphore for handler                */
     } PARM_ENSYNC;
typedef PARM_ENSYNC     FAR  *PPARM_ENSYNC;

/* ulFlags:                                                                 */
#define SYNC_MASTER      0x0000L    /* The handler will be a master for this sync*/
#define SYNC_SLAVE       0x0001L    /* The handler will be a slave for this sync */


/****************************************************************************/
#define SHC_DISABLE_SYNC          10L
     typedef struct _PARM_DISSYNC { /* shcdsy */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
     } PARM_DISSYNC;
typedef PARM_DISSYNC    FAR  *PPARM_DISSYNC;


/****************************************************************************/
#define SHC_GET_TIME              11L
     typedef struct _PARM_GTIME { /* shcgti */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        MMTIME   mmtimeCurrent;     /* Returns current stream time          */
     } PARM_GTIME;
typedef PARM_GTIME      FAR  *PPARM_GTIME;


/****************************************************************************/
#define SHC_GET_PROTOCOL          12L
     typedef struct _PARM_GPROT { /* shcgpt */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        SPCBKEY  spcbkey;           /* key of SPCB                          */
        PSPCB    pspcb;             /* ptr to SPCB (output)                 */
     } PARM_GPROT;
typedef PARM_GPROT      FAR  *PPARM_GPROT;


/****************************************************************************/
#define SHC_INSTALL_PROTOCOL      13L
     typedef struct _PARM_INSTPROT { /* shcipt */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        SPCBKEY  spcbkey;           /* key of SPCB                          */
        PSPCB    pspcb;             /* ptr to SPCB to install               */
        ULONG    ulFlags;           /* Install / Deinstall flags            */
     } PARM_INSTPROT;
typedef PARM_INSTPROT   FAR  *PPARM_INSTPROT;


/****************************************************************************/
#define SHC_ENUMERATE_PROTOCOLS   14L
     typedef struct _PARM_ENUMPROT { /* shcqpi */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        PVOID    paSPCBKeys;        /* ptr to buffer to fill with SPCB keys */
        PULONG   pulNumSPCBKeys;    /* # of entries buffer on input,        */
                                    /*  # of SPCB key on output.            */
     } PARM_ENUMPROT;
typedef PARM_ENUMPROT   FAR  *PPARM_ENUMPROT;


/****************************************************************************/
#define SHC_NEGOTIATE_RESULT      15L
     typedef struct _PARM_NEGOTIATE { /* shcneg */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        PSPCB    pspcb;             /* Ptr to negotiated SPCB               */
        ULONG    ulErrorStatus;     /* On error, indicates which field in the*/
     } PARM_NEGOTIATE;              /* SPCB failed the negotiation.         */
typedef PARM_NEGOTIATE  FAR  *PPARM_NEGOTIATE;


/****************************************************************************/
#define SHC_SENDMSG              16L
     typedef struct _PARM_SNDMSG { /* shcsndmsg */
        ULONG    ulFunction;        /* Handler command function             */
        HID      hid;               /* handler ID                           */
        HSTREAM  hstream;           /* handle of stream instance            */
        ULONG    ulMsgType;         /* Stream handler message type          */
        PVOID    pMsg;              /* Pointer to Message control block     */
     } PARM_SNDMSG;
typedef PARM_SNDMSG  FAR  *PPARM_SNDMSG;


/****************************************************************************/



/* XLATOFF */
#pragma pack()
/* XLATON */

/* NOINC */
#endif          /* _SSM_H_ */

#ifdef __cplusplus
}
#endif
/* INC */

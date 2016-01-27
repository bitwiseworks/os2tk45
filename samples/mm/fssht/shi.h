/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHI.H                                                  */
/*                                                                          */
/* This file contains the stream handler internal definitions and types.    */
/*                                                                          */
/****************************************************************************/
#include <shifwd.h>

/*
 *   Miscellaneous defines
 */
#define EXTLSTADD_ORDER       0x6201   /* Exit list order for us */
/* #define CREATE_SUSPENDED   1L          DosCreateThread suspended      */
#define STACKSIZE             65536L   /* Stack size for new thread */
#define STACKSIZE32           1024*64L /* Stack size for new thread */
#define HEAPSIZE              4096L    /* Heap size for control blocks */
#define SPI_SEEK_MODE_MASK    (SPI_SEEK_ABSOLUTE+SPI_SEEK_RELATIVE+SPI_SEEK_FROMEND)
#define TEMPORARY_ERROR       0L
#define RECOVERABLE_ERROR     1L
#define NONRECOVERABLE_ERROR  2L


/*
 * SleepNCheck input/Output bit flags
 */
#define DO_SLEEP                0x00000000L     /* Input Flags */

#ifdef INCL_NEWSH
#define DONT_SLEEP              0x00000004L
#define DONT_SLEEP_FLUSH        0x00000008L
#define DONT_SLEEP_DISCARD      0x00000010L

#define AWAKEN_BY_OTHER         0x00000000L     /* Output Flags */
#define AWAKEN_BY_DISCARD_STOP  0x00010000L
#define AWAKEN_BY_FLUSH_STOP    0x00020000L


#else
#define DONT_SLEEP            5L
#define DONT_SLEEP_FLUSH      6L
#endif

#define IOTHREAD_PRIORITYCLASS       PRTYC_TIMECRITICAL
#define IOTHREAD_PRIORITYDELTA       02L

/*
 *   Structure declarations
 */

/*
 * E_EVCB - Extended Event Control Blocks.  These are used
 *          to chain multiple EVCBs in a linked list
 *          the extra fields are used to support report time
 *          for both the recurring and non recurring events
 */
typedef struct _E_EVCB { /* e_evcb */
          struct _E_EVCB *pNext_e_evcb; /* pointer to the next EVCB */
          HEVENT      hevent;                /* saved event handle */
          TIME_EVCB   time_evcb;             /* time event control block */
          MMTIME      mmtimeDynamicTime;     /* single event report time (dynamic time line) */
          MMTIME      mmtimeStaticTime;      /* period of notification, and  */
                                             /* in single events contains */
                                             /* copy of above for generic */
                                             /* chain adding purposed     */
          ULONG       ulState;               /* event state */                                          /*WPOS*/
          MMTIME      mmReferenceTime;       /* current stream time for recurring events */             /*WPOS*/
          MMTIME      mmIntervalTime;        /* needed to retain the  "sticky" cuepoint from walking */ /*WPOS*/
} E_EVCB, *PE_EVCB;

/*
 *  ulState
 */
#define EVENT_PASSED    0x0000L              /* this event has already taken place */                   /*WPOS*/
#define EVENT_WAITING   0x0001L              /* this event is pending for cue to happen */              /*WPOS*/

/*
 * EVENTS - Event substructure
 */
typedef struct _EVENTBLK { /* event */
          BOOL        fAnyEventsEnabled;     /* quick test so see if any events enabled */
          MMTIME      mmtimeGranularity;     /* see SetGranularity () below (optional if a granularity factor is in the stream )*/
          HEVENT      heventDataUnderRun;    /* data under run event handle NULL if disabled */
          HEVENT      heventDataOverRun;     /* data over run event handle NULL if disabled */    /*WPOS*/
          PE_EVCB     pActiveRecurrEvcbChain;/* Pointer to the active recurring EVCB chain */
          PE_EVCB     pActiveSingleEvcbChain;/* Pointer to the active single shot EVCB chain */
          PE_EVCB     pInactiveEvcbChain;    /* Pointer to the inactive EVCB chain */
        } EVENTBLK, *PEVENTBLK;

/*
 *   Extended Stream Protocol Control Block (ESPCB)
 */
typedef struct espcb {
          struct espcb *pnxtESPCB;  /* rtn for events of this datatype */
          SPCB         spcb;        /* Stream Protocol Control Block */
        } ESPCB, *PESPCB;

/*
 * Stream Instance Block
 */
typedef struct sib {                   /* Stream Instance Block */
           struct sib *pnxtSIB;        /* Pointer to next SIB */
           ESPCB       espcb;          /* Extended SPCB */
           HSTREAM     hStream;        /* Stream handle for this instance */
           HID         HandlerID;      /* Handler ID (src or tgt) */
           ULONG       ulStatus;       /* Status of this instance */
           ULONG       ulActionFlags;  /* Flags that indicate that the */
                                       /*  read/write thread needs to do */
                                       /*  some special action. */
           TID         ThreadID;       /* ID of I/O thread */
           PID         ProcessID;      /* Process ID that created the stream */
           ULONG       ulAssocP1;      /* Associate Parm 1 */
           ULONG       ulAssocP2;      /* Associate Parm 2 */
           ULONG       aulInfo[5];     /* Array for misc information */
           MMTIME      mmtimeCurrentTime; /* current mmtime of the stream */
           HEV         hevStop;
           HEV         hevCmdSync;     /* sem for synchronizing command */
           BOOL        fSyncFlag;      /* sync pulse flag (indicates whether enabled or not) */
           SYNC_EVCB   syncEvcb;       /* generic sync evcb support for master & slave SH's */
           EVENTBLK    eventBlk;       /* generic event support linked lists */
        }  SIB, *PSIB;

/*
 * SIB fSyncFlag states
 */
#define SYNC_ENABLED           0x0001L    /* enable the adaptive stuff */
#define SYNC_DISABLED          0x0000L    /* Default state for the sync evcb */

/*
 *   SIB ulStatus States
 */
#define SIB_INITIALIZED          1L
#define SIB_NEGOTIATED           2L
#define SIB_READY_TO_RUN         3L
#define SIB_RUNNING              4L

#ifdef INCL_NEWSH
#define SIB_STOPPED              8L
#define SIB_PAUSED               9L
#else
#define SIB_PAUSED               5L
#endif

/*
 * SIB ulActionFlag bit meanings
 */
#define SIBACTFLG_KILL         0x0001L     /* Kill the read/write thread */
#define SIBACTFLG_STOP_FLUSH   0x0002L     /* Stop and flush all filled buffers. */
                                           /*  This flag is the same value as the */
                                           /*  Stop stream flag bit value. */
#define SIBACTFLG_STOP_DISCARD 0x0004L     /* Stop and discard all filled buffers. */
                                           /*  This flag is the same value as the */
                                           /*  Stop stream flag bit value. */
#define SIBACTFLG_STOP_PAUSE   0x0008L     /* Stop Pause the IO thread */
                                           /*  This is only used for stream handlers that the IO */
                                           /*  thread gets the global data mutex sem. */
#define SIBACTFLG_THREAD_DEAD  0x0010L     /* The IO Thread has exited.  This could be due */
                                           /*  to several reasons including exit list processing. */
                                           /*  The synchronous commands should not wait on the */
                                           /*  io thread to post an event if this flag is set. */

/*
 *   External Global Data Definitions
 */
extern PESPCB pESPCB_ListHead;         /* Pointer to list of Extended SPCB's */
extern ULONG ulProcessCount;           /* # of processes using this stream */
                                       /*  handler */
extern PSIB pSIB_list_head;            /* Pointer to list of SIB's in use */
                                       /*  for this process. */
extern PSZ pszHandlerName;             /* Name of this stream handler */
extern ULONG ulHandlerVersion;         /* Version of this handler */
extern HID SrcHandlerID;               /* Handler ID returned from Register */
extern HID TgtHandlerID;               /* Handler ID returned from Register */
extern HHUGEHEAP hHeap;                /* Handle of heap for SIB's and EVCB's */
extern PSZ pszProcCntMutexName;        /* Name of semaphore to control the */
                                       /*  usProcessCount variable. */
extern HMTX hmtxProcCnt;               /* Handle of semaphore to control the */
                                       /*  usProcessCount variable. */
extern HMTX hmtxGlobalData;            /* Handle of semaphore to control the */
                                       /*  Global Data structures. */
extern PFNTHREAD ShWriteRtn;           /* variable to hold @ of write routine */
extern PFNTHREAD ShReadRtn;            /* variable to hold @ of read routine. */
extern PFNTHREAD ShRdWrRtn;            /* variable to hold read/write routine. */

typedef RC (* _System PFNSHINSTALLCHECK)(PPARM_INSTPROT pInstProt);
extern PFNSHINSTALLCHECK pfnShInstallCheck;          /* variable to hold @ of install check routine */
typedef RC (* _System PFNSHCSENDMSG)(PPARM_SNDMSG pSndMsg);
extern PFNSHCSENDMSG pfnShcSendMsg;              /* variable to hold @ of ShcSendMsg routine */
typedef RC (* _System PFNSHCGETTIME)(PPARM_GTIME pGetTime);
extern PFNSHCGETTIME pfnShcGetTime;              /* variable to hold @ of ShcGetTime routine */
typedef RC (* _System PFNSHCENABLEEVENT)(PPARM_ENEVENT pEnableEvent);
extern PFNSHCENABLEEVENT pfnShcEnableEvent;          /* variable to hold @ of ShcEnableEvent routine */
typedef RC (* _System PFNSHCDISABLEEVENT)(PPARM_DISEVENT pDisableEvent);
extern PFNSHCDISABLEEVENT pfnShcDisableEvent;         /* variable to hold @ of ShcDisableEvent routine */
typedef RC (* _System PFNSHCENABLESYNC)(PPARM_ENSYNC pEnableSync);
extern PFNSHCENABLESYNC pfnShcEnableSync;           /* variable to hold @ of ShcEnableSync routine */
typedef RC (* _System PFNSHCDISABLESYNC)(PPARM_DISSYNC pDisableSync);
extern PFNSHCDISABLESYNC pfnShcDisableSync;          /* variable to hold @ of ShcDisableSync routine */

extern ULONG ulHandlerFlags;           /* Stream handler specific flags */


/*
 * fHandlerFlag Defines - flags that describe and alter functionality of stream
 *                        handlers using this common code
 */
#define  HANDLER_CAN_BE_SOURCE     0x001L    /* Stream handler can be a source stream handler */
#define  HANDLER_CAN_BE_TARGET     0x002L    /* Stream handler can be a target stream handler */
#define  HANDLER_INSTALL_CHECKS    0x004L    /* Stream handler does additional checks when */
                                             /*  processing SHC_INSTALL_PROTOCOL */
#define  HANDLER_IS_NONSTREAMING   0x008L    /* Stream handler is non streaming */
#define  HANDLER_SHC_SENDMSG       0x100L    /* Stream handler supports SHC_SENDMSG api */
#define  HANDLER_SHC_GET_TIME      0x200L    /* Stream handler supports SHC_GET_TIME api */
#define  HANDLER_SHC_EVENT_CALLS   0x400L    /* Stream handler supports shc event apis */
#define  HANDLER_SHC_SYNC_CALLS    0x800L    /* Stream handler supports shc sync apis */

/*
 *  ShFindESPCB() ulDefaults defines
 */
#define ESPCB_DEFAULT_LAST_GENERIC    0x0001L     /* If no match, use the last installed GENERIC espcb */
#define ESPCB_IGNORE_INTKEY           0x8000L     /* Ignore the ulIntKey field when matching */
#define ESPCB_IGNORE_DATASUBTYPE      0x8001L     /* Ignore the ulDataSubType field when matching */

/*
 * Macros
 */
#define ENTERCRITX(rc)         if (rc = DosRequestMutexSem(hmtxGlobalData, SEM_INDEFINITE_WAIT)) return(rc);
#define ENTERCRIT(rc)          (rc = DosRequestMutexSem(hmtxGlobalData, SEM_INDEFINITE_WAIT))
#define EXITCRIT               (DosReleaseMutexSem(hmtxGlobalData))

#define DEAD(psib)             (psib->ulActionFlags & SIBACTFLG_THREAD_DEAD)

#define WAIT_FOR_DEAD_IO(psib) {                                              \
                               if (!DEAD(psib))                               \
                                  DosWaitThread(&psib->ThreadID, DCWW_WAIT);  \
                               }

#define RESUME_IO(psib,status) {                                                          \
                               if (psib->ulStatus == SIB_PAUSED) {                        \
                                  if (status)                                             \
                                     psib->ulStatus = status;                             \
                                  DosResumeThread(psib->ThreadID);                        \
                                  }                                                       \
                               }

#define SUSPEND_IO(psib)       {                                                                        \
                               if ((psib->ThreadID) && !(psib->ulActionFlags & SIBACTFLG_THREAD_DEAD))  \
                                   rc = DosSuspendThread(psib->ThreadID);                               \
                               }

#define RECEIVE_MSG(psib,rc)   {                                                          \
                               rc = DosWaitEventSem(psib->hevStop, SEM_INDEFINITE_WAIT);  \
                               }

#define SEND_MSG(psib,cmd,status) {                                \
                               psib->ulActionFlags |= cmd;         \
                               DosPostEventSem(psib->hevStop);     \
                               RESUME_IO(psib,status);             \
                               }

#define SYNC_SEND_MSG(psib,cmd,status) {                           \
                               RESET_REPLY(psib);                  \
                               SEND_MSG(psib,cmd,status);          \
                               RECEIVE_REPLY(psib);                \
                               }

#define RESET_MSG(psib)        {                                                          \
                               ULONG ulPostCount;                                         \
                               DosResetEventSem(psib->hevStop, &ulPostCount);             \
                               }

#define SEND_REPLY(psib)       {                                   \
                               DosPostEventSem(psib->hevCmdSync);  \
                               }

#define RECEIVE_REPLY(psib)    {                                                          \
                               if (!DEAD(psib))                                           \
                                  DosWaitEventSem(psib->hevCmdSync, SEM_INDEFINITE_WAIT); \
                               }

#define RESET_REPLY(psib)      {                                                          \
                               ULONG ulPostCount;                                         \
                               DosResetEventSem(psib->hevCmdSync, &ulPostCount);          \
                               }



/*
 * Prototype statements
 */

  /* SHC commands that must be supported */
RC ShcAssociate(PPARM_ASSOC);                         /* SH specific */
RC ShcCreate(PPARM_CREATE);                           /* SH specific */
RC ShcDestroy(PPARM_DESTROY);                         /* shdestry.c or SH specific */
RC ShcEnumerateProtocols(PPARM_ENUMPROT);             /* sheprot.c */
RC ShcGetProtocol(PPARM_GPROT);                       /* shgprot.c */
RC ShcInstallProtocol(PPARM_INSTPROT);                /* shiprot.c */
RC ShcNegotiateResult(PPARM_NEGOTIATE);               /* shnegot.c or SH specific */
RC ShcSeek(PPARM_SEEK);                               /* SH specific */
RC ShcStart(PPARM_START);                             /* shstart.c  or SH specific */
RC ShcStop(PPARM_STOP);                               /* shstop.c or SH specific */

  /* Optional SHC Commands to support */
RC _System ShcDisableEvent(PPARM_DISEVENT);                   /* shevent.c or SH specific */
RC _System ShcDisableSync(PPARM_DISSYNC);                     /* shsync.c or SH specific */
RC _System ShcEnableEvent(PPARM_ENEVENT);                     /* shevent.c or SH specific */
RC _System ShcEnableSync(PPARM_ENSYNC);                       /* shsync.c or SH specific */
RC _System ShcGetTime(PPARM_GTIME);                           /* shtime.c or SH specific */
RC _System ShcSendMsg(PPARM_SNDMSG);                          /* shmsg.c or SH specific */

  /* In shinit.c */
ULONG _DLL_InitTerm( HMODULE hmod, ULONG fTerm);
VOID APIENTRY ShExitList(VOID);

  /* IN shrouter.c */
SHCFN ShcRouter;

  /* In shmisc.c */
RC ShFindSib(PSIB*, HSTREAM, HID);
PESPCB ShFindEspcb(SPCBKEY);
PESPCB ShFindESPCB(SPCBKEY, ULONG);
#ifdef FIXSH
PESPCB ShSearchESPCBList(PSPCBKEY, ULONG, PESPCB *);
#else
PESPCB ShSearchESPCBList(PSPCBKEY, ULONG, PESPCB);
#endif
RC DestroySIB (PSIB psib);

  /* In shioutil.c */
RC ReportEvent(PSIB, RC, ULONG, ULONG, ULONG);
RC CheckNSleep(PSIB);
RC SleepNCheck(PSIB, PULONG);
RC ShIOError(PSIB, PARM_NOTIFY, RC);

  /* In shevent.c */
RC CheckEvents       ( PSIB );
RC ReCalcEventChains ( PEVENTBLK, MMTIME, BOOL);
RC SetGranularity    ( PSIB, MMTIME );


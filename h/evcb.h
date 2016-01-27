/****************************************************************************/
/*                                                                          */
/* Module Name: EVCB.H                                                      */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions Sync/Stream Manager Event Control         */
/* Block defintitions.                                                      */
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



/* XLATOFF */
#pragma pack(4)
/* XLATON */

/**********************************************
 * List of Implicit events:
 *    EVENT_EOS                 - Use IMPL_EVCB
 *    EVENT_ERROR               - Use IMPL_EVCB
 *    EVENT_STREAM_STOPPED      - Use IMPL_EVCB
 *    EVENT_SYNC_PREROLLED      - Use IMPL_EVCB
 *    EVENT_PLAYLISTMESSAGE     - Use PLAYL_EVCB
 *    EVENT_PLAYLISTCUEPOINT    - Use PLAYL_EVCB
 *    EVENT_QUEUE_OVERFLOW      - Use IMPL_EVCB
 **********************************************/

/**********************************************
 * List of Explicit events:
 *    EVENT_SYNCOVERRUN         - Use OVRU_EVCB
 *    EVENT_CUE_TIME            - Use TIME_EVCB
 *    EVENT_CUE_DATA            - Use DATA_EVCB
 *    EVENT_DATAUNDERRUN        - Use EVCB
 *    EVENT_DATAOVERRUN         - Use EVCB
 *    EVENT_CUE_TIME_PAUSE      - Use TIME_EVCB
 **********************************************/
#define EVENT_SYNCOVERRUN       4L
#define EVENT_CUE_TIME          5L       /* Cue point in terms of stream time*/
#define EVENT_CUE_DATA          6L       /* Cue point in terms of data items */
#define EVENT_DATAUNDERRUN      7L       /* data underrun event from SH    */
#define EVENT_DATAOVERRUN       8L       /* data underrun event from SH    */
#define EVENT_CUE_TIME_PAUSE    9L       /* Pause when cue-point reached.  */
#define EVENT_STATUS_LEVEL      10L      /* Report status level            */



/**********************************************
 * SYNC_EVCB - Sync Pulse Event Control Block
 *             (This event is only seen by
 *              stream handlers)
 **********************************************/
#define EVENT_SYNC              1L     /* Synchronization point in stream  */
typedef struct  _SYNC_EVCB { /* syevcb                                     */
   ULONG   ulType;                     /* EVENT_SYNC                       */
   ULONG   ulSubType;                  /* Not used                         */
   ULONG   ulSyncFlags;                /* 0 (input),status (output)        */
   HSTREAM hstream;                    /* handle to stream for this event  */
   HID     hid;                        /* Handler id                       */
   ULONG   ulStatus;                   /* Event status (output)            */
   MMTIME  mmtimeStart;                /* Filled in by Sync/Stream manager */
                                       /*  at SpiEnableSync time. (input)  */
   MMTIME  mmtimeMaster;               /* Filled in by Master SH (input)   */
   MMTIME  mmtimeSlave;                /* Filled in by slave SH. (output)  */
   } SYNC_EVCB;
typedef SYNC_EVCB  FAR  *PSYNC_EVCB;             /* Ptr to a  EVCB       */

/* ulSyncFlags:                                                            */
#define  SYNCOVERRUN        0x0001L    /* Set by SSM if sync pulse comes in before    */
                                       /*  a has processed the current sync pulse.    */
#define  SYNCPOLLING        0x0002L    /* SSM set this after filling the handler EVCB.*/
                                       /*  Handler resets it after it processes the   */
                                       /*  contents of EVCB. The SSM must check this  */
                                       /*  bit before modifying EVCB. If bit already  */
                                       /*  set, set the SYNC overrun bit.             */


#define EVENT_RESERVED          2L     /* RESERVED                         */


/**********************************************
 * IMPL_EVCB - Implicit Event Control Block
 *             (This is a parameter on the
 *              SpiCreatStrem API)
 **********************************************/
#define EVENT_IMPLICIT_TYPE     3L     /* These events are always supported*/
typedef struct  _IMPL_EVCB { /* imevcb                                     */
   ULONG   ulType;                     /* EVENT_IMPLICIT_TYPE              */
   ULONG   ulSubType;                  /* One of the implicit events (input) */
   ULONG   ulFlags;                    /* 0 (Input), status (Output)         */
   HSTREAM hstream;                    /* handle to stream for this event (input/output) */
   HID     hid;                        /* 0 (input), handler Id (output)     */
   ULONG   ulStatus;                   /* Event status/error return code (output) */
   ULONG   unused1;                    /* 0 (input)                        */
   ULONG   unused2;                    /* 0 (input)                        */
   ULONG   unused3;                    /* 0 (input)                        */
   } IMPL_EVCB;
typedef IMPL_EVCB FAR *PIMPL_EVCB;

/* ulSubtype:                                                              */
#define EVENT_EOS               1L     /* End of Stream                    */
#define EVENT_ERROR             2L     /* Error in Stream handler or device driver        */
#define EVENT_STREAM_STOPPED    3L     /* Stream is in a stopped state (Discard or Flush) */
#define EVENT_SYNC_PREROLLED    4L     /* All streams are prerolled (buffers   */
                                       /*  have been filled by the Source SH's.*/
#define EVENT_PLAYLISTMESSAGE   5L     /* Memory SH playlist message event     */
#define EVENT_PLAYLISTCUEPOINT  6L     /* Memory SH playlist cue point         */
#define EVENT_QUEUE_OVERFLOW    7L     /* Event Queue overflow. Indicates lost */
                                       /*  events. Application must use this   */
                                       /*  to clear any waiting conditions.    */
#define EVENT_START             8L     /* Start stream                         */


/**********************************************
 * PLAYL_EVCB - Playlist Message Event Control Block
 **********************************************/
/* (IMPLICIT EVENT)                                                        */
typedef struct  _PLAYL_EVCB { /* plevcb                                    */
   ULONG   ulType;                     /* EVENT_IMPLICIT_TYPE              */
   ULONG   ulSubType;                  /* EVENT_PLAYLISTMESSAGE or EVENT_PLAYLISTCUEPOINT */
   ULONG   ulFlags;                    /* 0 (input), status (output)                      */
   HSTREAM hstream;                    /* handle to stream for this event (input/output)  */
   HID     hid;                        /* 0 (input), handler Id (output)                  */
   ULONG   ulStatus;                   /* Playlist Instruction number (output)            */
   ULONG   ulMessageParm;              /* 0 (input), Message from playlist (output)       */
   ULONG   unused1;                    /* 0 (input)                        */
   ULONG   unused2;                    /* 0 (input)                        */
   } PLAYL_EVCB;
typedef PLAYL_EVCB  FAR  *PPLAYL_EVCB;             /* Ptr to a  EVCB       */


/**********************************************
 * OVRU_EVCB - Sync Overrun Event Control Block
 *             (Applications can request to be
 *              notified whenever a Sync overrun
 *              occurs.
 **********************************************/
typedef struct  _OVRU_EVCB { /* ovevcb */
   ULONG   ulType;                     /* EVENT_SYNCOVERRUN                */
   ULONG   ulSubType;                  /* Not used                         */
   ULONG   ulFlags;                    /* 0 (input), status (output)       */
   HSTREAM hstream;                    /* handle to stream for this event (input/output) */
   HID     hid;                        /* handler Id (input/output)        */
   ULONG   ulStatus;                   /* Event status/error return code (output) */
   MMTIME  mmtimeSlave;                /* 0 (input), Slave stream time (output)   */
   MMTIME  mmtimeStart;                /* 0 (input), Slave start offset (offset)  */
   MMTIME  mmtimeMaster;               /* 0 (input), Master stream time (output)  */
   } OVRU_EVCB;
typedef OVRU_EVCB  FAR  *POVRU_EVCB;             /* Ptr to a  EVCB       */


/**********************************************
 * TIME_EVCB - Cue Time Event Control Block
 *             (Applications can request to be
 *              notified for a time cue point)
 **********************************************/
typedef struct  _TIME_EVCB { /* tievcb                                     */
   ULONG   ulType;                     /* EVENT_CUE_TIME                   */
   ULONG   ulSubType;                  /* Not used                         */
   ULONG   ulFlags;                    /* Single/Recurring(input/output)   */
   HSTREAM hstream;                    /* handle to stream for this event (input/output)*/
   HID     hid;                        /* handler Id (input/output)                     */
   ULONG   ulStatus;                   /* Event status/error return code (output)       */
   MMTIME  mmtimeStream;               /* Stream time (input/output)       */
   ULONG   unused1;                    /* 0 (input)                        */
   ULONG   unused2;                    /* 0 (input)                        */
   } TIME_EVCB;
typedef TIME_EVCB  FAR  *PTIME_EVCB;             /* Ptr to a  EVCB       */

/*                                                                         */
/* ulFlags:                                                                */
#define EVENT_SINGLE            0L     /* This is single one time event    */
#define EVENT_RECURRING         1L     /* This is recurring event          */


/**********************************************
 * DATA_EVCB - Cue Data Event Control Block
 *             (Applications can request to be
 *              notified for a data cue point)
 **********************************************/
typedef struct  _DATA_EVCB { /* daevcb                                     */
   ULONG   ulType;                     /* EVENT_CUE_DATA                   */
   ULONG   ulSubType;                  /* Event SubType                    */
   ULONG   ulFlags;                    /* Single/Recurring(input/output)   */
   HSTREAM hstream;                    /* handle to stream for this event (input/output) */
   HID     hid;                        /* handler Id (input/output)                      */
   ULONG   ulStatus;                   /* Event status/error return code (output)        */
   MMTIME  mmtimeStream;               /* Stream time (input/output)                     */
   ULONG   ulEventParm1;               /* Data to Cue on (input/output)                  */
                                       /*  This is interpreted by the stream handler     */
   ULONG   ulEventParm2;               /* Length of data cue buffer (input/output)       */
   } DATA_EVCB;
typedef DATA_EVCB  FAR  *PDATA_EVCB;             /* Ptr to a  EVCB       */

/* ulFlags:                                                                */
/*#define EVENT_SINGLE          0L        This is single one time event    */
/*#define EVENT_RECURRING       1L        This is recurring event          */
#define EVENT_DATAPTR           2L     /* ulEventParm1 if ptr to buffer and*/
                                       /*  ulEventParm2 is a length of buffer, */
                                       /*  else ulEventParm1 is data.          */


/******************
 * EVCB_ulType:  (Stream Handler defined types)
 ******************/
#define SH_TYPE           0x80000000L  /* Stream Handler defined types must */
                                       /* have the high order bit set in the*/
                                       /* ulType field.                     */
                                       /* events: 0H - 7FFFFFFFH are reserved!*/

/**********************************************
 * EVCB - Generic Event Control Block (Use the
 *        specific one in the SSM.H if possible)
 **********************************************/
typedef struct  _EVCB { /* evcb */
   ULONG   ulType;                     /* Event type (input)               */
   ULONG   ulSubType;                  /* Event SubType (input)            */
   ULONG   ulFlags;                    /* 0 (Input), status (Output)       */
   HSTREAM hstream;                    /* handle to stream for this event  */
   HID     hid;                        /* handler Id (input/output)        */
   ULONG   ulStatus;                   /* Event status (output)            */
   ULONG   ulEventParm1;               /* Event parameters (input), HID if implicit event */
   ULONG   ulEventParm2;               /* Event parameters (input)         */
   ULONG   ulEventParm3;               /* Event parameters (input)         */
   } EVCB;
typedef EVCB  FAR  *PEVCB;             /* Ptr to a  EVCB       */

/**********************************************
 * STATUS_LEVEL_EVCB - Status Level Event Control Block
 **********************************************/
typedef struct  _STATUS_LEVEL_EVCB { /* stevcb */
   /* standard part */
   ULONG   ulType;                     /* Event type (input)               */
   ULONG   ulSubType;                  /* Event SubType (input)            */
   ULONG   pMCDInstance;               /* for MCD use */
   HSTREAM hstream;                    /* handle to stream for this event  */
   HID     hid;                        /* handler Id (input/output)        */
   /* custom part */
   ULONG   ulSample[4];                /* Samples */
   } STATUS_LEVEL_EVCB;
typedef STATUS_LEVEL_EVCB  FAR  *PSTATUS_EVCB;     /* Ptr to a  EVCB       */

/* XLATOFF */
#pragma pack()
/* XLATON */

/* NOINC */
#ifdef __cplusplus
}
#endif
/* INC */


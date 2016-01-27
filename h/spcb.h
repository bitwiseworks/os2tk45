/****************************************************************************/
/*                                                                          */
/* Module Name: SPCB.H                                                      */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions Sync/Stream Manager Stream Protocol       */
/* Control Block Definitions.                                               */
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

#ifndef  _SPCBKEY_DEFINED
#define  _SPCBKEY_DEFINED
/**********************************************
 *
 * SPCBKEY - Stream Protocol Key
 *
 **********************************************/
typedef struct _SPCBKEY { /* spcbkey SPCB key (Stream data type) */
   ULONG    ulDataType;
   ULONG    ulDataSubType;
   ULONG    ulIntKey;                 /* generated internal key            */
   } SPCBKEY;
typedef SPCBKEY FAR *PSPCBKEY;
#endif


/**********************************************
 *
 * SPCB - Stream Protocol Control Block
 *
 **********************************************/
typedef struct  _SPCB { /* spcb */
   ULONG   ulSPCBLen;             /* SPCB struture length                   */
   SPCBKEY spcbkey;
   ULONG   ulDataFlags;           /* Data type flags                        */
   ULONG   ulNumRec;              /* Max # records/buffer (Only used for Split streams) */
   ULONG   ulBlockSize;           /* Block alignment in bytes.                   */
   ULONG   ulBufSize;             /* Buffer size (SSM allocated buffers) must be */
                                  /*  a multiple of Block size                   */
   ULONG   ulMinBuf;              /* Minimum number of buffers needed to stream  */
   ULONG   ulMaxBuf;              /* Maximum number of buffers needed to stream  */
   ULONG   ulSrcStart;            /* # of EMPTY buffers required to start Source */
   ULONG   ulTgtStart;            /* # of FULL buffers required to start Target  */
   ULONG   ulBufFlags;            /* Handler Protocol negotiation flags          */
   ULONG   ulHandFlags;           /* Handler Protocol flags                      */
   MMTIME  mmtimeTolerance;       /* Sync tolerance value...Used as check by SSM */
                                  /*  to determine whether to send a sync pulse  */
                                  /*  to this specific slave Stream Handler.     */
                                  /*  Algorithm:                                 */
                                  /*   diff = abs(master time - slave time)      */
                                  /*   if  diff >= tolerance                     */
                                  /*     then send sync pulse                    */
                                  /* (Valid only for Slave stream handlers)      */
   MMTIME  mmtimeSync;            /* Used to save sync pulse generation granularity */
                                  /*  if the master uses the Stream Manager Timer.  */
                                  /*  if SpiSetSync passed NULL value for sync gran */
                                  /*   then use this default value. (since hardware */
                                  /*   must have it's own interrupt time interval.  */
                                  /* (Valid only for Master stream handlers)        */
   ULONG   ulBytesPerUnit;        /* Bytes/unit of time. This is used to do seeks   */
                                  /* on linear data that is not compressed or       */
                                  /* of variable length.                            */
   MMTIME  mmtimePerUnit;         /* The amount of mmtime each unit represents.     */
                                  /* A unit can be a second, minute or whatever.    */
   } SPCB;
typedef SPCB FAR *PSPCB;

#define SPCB_MAX_BUF_SIZE      (1024L*1024L)   /* Largest buffer size in bytes  */

/*******************
 * SPCB_ulDataFlags:
 *******************/
/* XLATOFF */
#define  VALIDDATAFLAGS        ( SPCBDATA_CUETIME |           \
                                 SPCBDATA_CUEDATA |           \
                                 SPCBDATA_YIELDTIME |         \
                                 SPCBDATA_NOSEEK )
/* XLATON */
#define  SPCBDATA_CUETIME      0x0002L /* This data type can support time cue points*/
#define  SPCBDATA_CUEDATA      0x0004L /* This data type can support data cue points*/

#define  SPCBDATA_SEEK         0x0000L /* Seeks can be performed on this data type. */
#define  SPCBDATA_NOSEEK       0x0008L /* NO seeks can be performed on this data type.*/
#define  SPCBDATA_YIELDTIME    0x0010L /* The ulBytes per unit field is used for a millisecond value    */
                                       /* It represents the amount of yield time between reads of       */
                                       /* each buffer. If this bit is set, the value of ulBytesPerUnit  */
                                       /* is used as input to DosSleep to yield for that period of time */

/********************
 * SPCB_ulBufFlags:
 ********************/
/* XLATOFF */
#define  VALIDBUFFLAGS         ( SPCBBUF_USERPROVIDED |       \
                                 SPCBBUF_FIXEDBUF |           \
                                 SPCBBUF_NONCONTIGUOUS |      \
                                 SPCBBUF_INTERLEAVED |        \
                                 SPCBBUF_16MEG |              \
                                 SPCBBUF_FIXEDBLOCK |         \
                                 SPCBBUF_MAXSIZE )
/* XLATON */
#define  SPCBBUF_USERPROVIDED  0x0001L /* User provides buffers for stream        */
                                       /* SSMgr will not allocate buffers,        */
                                       /* but must lock down provided buffers,    */
                                       /* so this will affect performance!!       */
                                       /* (Source Handler only)                   */
#define  SPCBBUF_FIXEDBUF      0x0002L /* Buffer size is fixed for this data type */
                                       /*  in this handler. Can not be used       */
                                       /*  with the SPCBBUF_USERPROVIDED flags.   */
#define  SPCBBUF_NONCONTIGUOUS 0x0004L /* Buffer do not need to be contiguous     */
                                       /*  in physical memory.                    */
#define  SPCBBUF_INTERLEAVED   0x0008L /* Stream buffers can be interleaved.      */
                                       /*  Can not be use with the                */
                                       /*  SPCBBUF_USERPROVIDED flag.             */
                                       /*  (Source Handler only)                  */
#define  SPCBBUF_MAXSIZE       0x0010L /* ulBufSize is the maximum size that      */
                                       /*  can be used by this stream handler     */
#define  SPCBBUF_16MEG         0x0020L /* The Stream buffers may be allocated     */
                                       /*  above the 16 Meg line. This is used    */
                                       /*  by stream handlers that can support    */
                                       /*  greater than 16 Megabyte addresses.    */
#define  SPCBBUF_FIXEDBLOCK    0x0040L /* Use the ulBlockSize field to represent  */
                                       /* the size of the IO reads that should    */
                                       /* be performed (Interleaved streams)      */

/*******************
 * SPCB_ulHandFlags:
 *******************/
/* XLATOFF */
#define  VALIDHANDFLAGS        ( SPCBHAND_GENSYNC |           \
                                 SPCBHAND_RCVSYNC |           \
                                 SPCBHAND_TIMER |             \
                                 SPCBHAND_NONSTREAM |         \
                                 SPCBHAND_GENTIME |           \
                                 SPCBHAND_NOPREROLL |         \
                                 SPCBHAND_NOSYNC |            \
                                 SPCBHAND_PHYS_SEEK )
/* XLATON */
#define  SPCBHAND_GENSYNC      0x0001L /* (INPUT FROM HANDLER ONLY)               */
                                       /* This handler can generate sync pulses   */
                                       /* if it is the master.                    */
#define  SPCBHAND_RCVSYNC      0x0002L /* (INPUT FROM HANDLER ONLY)               */
                                       /* This handler can receive sync pulses    */
                                       /* if it is a slave                        */
#define  SPCBHAND_TIMER        0x0004L /* Use Stream Manager Timer for Sync       */
#define  SPCBHAND_NONSTREAM    0x0008L /* Stream Handler is non-streaming         */
#define  SPCBHAND_GENTIME      0x0010L /* This handler contains real              */
                                       /* stream time. The handler that           */
                                       /* supports the SpiGetTime, data/time      */
                                       /* cue point calls for this stream.        */
#define  SPCBHAND_NOPREROLL    0x0020L /* This stream can not be prerolled.       */
                                       /* (i.e. recording streams)                */
#define  SPCBHAND_NOSYNC       0x0040L /* This stream can be group into a         */
                                       /* sync group, but does not send or        */
                                       /* receive sync pulses.                    */
#define  SPCBHAND_PHYS_SEEK    0x0080L /* This handler does a seek to a physical  */
                                       /* device or memory not just a time adjustment.*/
                                       /* (ssm will always call this handler first on */
                                       /*  an SpiSeekStream call).                    */

/* XLATOFF */
#pragma pack()
/* XLATON */

/* NOINC */
#ifdef __cplusplus
}
#endif
/* INC */


/*==============================================================*\
 *  queue.h - Sample queue apps include file
 *      Copyright 1991 IBM Corp.
 *--------------------------------------------------------------
 *
 *  This file contains constant and function
 *  definitions for queues.
 *
\*==============================================================*/
#define TIMEOUT_LEN  (ULONG)1000
#define Q_NAME       "BUYIBM"
#define Q_MSG_DATE   1
#define Q_MSG_TEXT   2

typedef struct _Q_DATE {
   USHORT  usDay;
   USHORT  usMonth;
   USHORT  usYear;
} Q_DATE;

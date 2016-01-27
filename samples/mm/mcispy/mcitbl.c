/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MCITBL.C
*
* DESCRIPTION: MCISPY helper routines.
*
*              Copyright (c) IBM Corporation   1993
*                        All Rights Reserved
*
*
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_WIN                        /* OS/2 2.x Win include            */
#define INCL_OS2MM                      /* OS/2 2.x Win include            */
#include <os2.h>                        /* OS/2 2.x System include         */
#include <stdio.h>                      /* C Standard funcs include        */
#include <string.h>                     /* C String Functions include      */
#include <os2me.h>                      /* Multimedia System include       */
#include "mcispy.h"                     /* Component Private Definitions   */

   MCIMessageTable   MCIMsgTbl[]={
   23,   "MCI_ACQUIREDEVICE",          FALSE,
   40,   "MCI_CAPTURE",                FALSE,
   2,    "MCI_CLOSE",                  FALSE,
   33,   "MCI_CONNECTION",             FALSE,
   28,   "MCI_CONNECTOR",              FALSE,
   31,   "MCI_CONNECTORINFO",          FALSE,
   56,   "MCI_COPY",                   FALSE,
   17,   "MCI_CUE",                    FALSE,
   54,   "MCI_CUT",                    FALSE,
   32,   "MCI_DEFAULT_CONNECTION",     FALSE,
   53,   "MCI_DELETE",                 FALSE,
   27,   "MCI_DEVICESETTINGS",         FALSE,
   3,    "MCI_ESCAPE",                 FALSE,
   41,   "MCI_FREEZE",                 FALSE,
   9,    "MCI_GETDEVCAPS",             FALSE,
   42,   "MCI_GETIMAGEBUFFER",         FALSE,
   43,   "MCI_GETIMAGEPALETTE",        FALSE,
   26,   "MCI_GETTOC",                 FALSE,
   34,   "MCI_GROUP",                  FALSE,
   8,    "MCI_INFO",                   FALSE,
   22,   "MCI_LOAD",                   FALSE,
   25,   "MCI_MASTERAUDIO",            FALSE,
   58,   "MCI_MAX_COMMAND",            FALSE,
   1,    "MCI_OPEN",                   FALSE,
   55,   "MCI_PASTE",                  FALSE,
   7,    "MCI_PAUSE",                  FALSE,
   4,    "MCI_PLAY",                   FALSE,
   44,   "MCI_PUT",                    FALSE,
   45,   "MCI_REALIZE",                FALSE,
   14,   "MCI_RECORD",                 FALSE,
   57,   "MCI_REDO",                   FALSE,
   24,   "MCI_RELEASEDEVICE",          FALSE,
   47,   "MCI_RESTORE",                FALSE,
   29,   "MCI_RESUME",                 FALSE,
   46,   "MCI_REWIND",                 FALSE,
   16,   "MCI_SAVE",                   FALSE,
   5,    "MCI_SEEK",                   FALSE,
   12,   "MCI_SET",                    FALSE,
   48,   "MCI_SETIMAGEBUFFER",         FALSE,
   49,   "MCI_SETIMAGEPALETTE",        FALSE,
   19,   "MCI_SET_CUEPOINT",           FALSE,
   20,   "MCI_SET_POSITION_ADVISE",    FALSE,
   21,   "MCI_SET_SYNC_OFFSET",        FALSE,
   11,   "MCI_SPIN",                   FALSE,
   10,   "MCI_STATUS",                 FALSE,
   13,   "MCI_STEP",                   FALSE,
   6,    "MCI_STOP",                   FALSE,
   15,   "MCI_SYSINFO",                FALSE,
   58,   "MCI_UNDO",                   FALSE,
   50,   "MCI_UNFREEZE",               FALSE,
   18,   "MCI_UPDATE",                 FALSE,
   51,   "MCI_WHERE",                  FALSE,
   52,   "MCI_WINDOW",                 FALSE,
   1283, "MM_MCICUEPOINT",            FALSE,
   1285, "MM_MCIEVENT",               FALSE,
   1280, "MM_MCINOTIFY",              FALSE,
   1281, "MM_MCIPASSDEVICE",          FALSE,
   1284, "MM_MCIPLAYLISTMESSAGE",     FALSE,
   1282, "MM_MCIPOSITIONCHANGE",      FALSE,
   1286, "MM_MCISYNCH",               FALSE
   };

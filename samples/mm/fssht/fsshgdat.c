/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME:  FSSHGDAT.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  File System Stream Handler Global Data                */
/*                                                                          */
/* NOTES: This is a DLL global data file. It contains the global data       */
/*        declarations for this stream handler.                             */
/*                                                                          */
/* ENTRY POINTS: None                                                       */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>
#include <fssh.h>

#pragma data_seg (SHR_SEG)               /* Put this into a seperate data seg */

PESPCB pESPCB_ListHead = NULL;           /* Pointer to list of Extended SPCB's */

ULONG  ulProcessCount = 0;               /* # of processes using this stream */
                                         /*  handler.  Access to this variable */
                                         /*  is controlled by the hmtxProcCnt */
                                         /*  semaphore. */
PSIB pSIB_list_head = NULL;              /* Pointer to list of SIB's in use */
                                         /*  for this process. Access to */
                                         /*  this list is controlled by the */
                                         /*  hmtxGlobalData semaphore. */
PSZ pszHandlerName = (PSZ)FSSH_HANDLER_NAME;  /* Name of this stream handler */
ULONG ulHandlerVersion = FSSH_VERSION;   /* Version of this handler */
HID SrcHandlerID = 0;                    /* Handler ID returned from Register */
HID TgtHandlerID = 0;                    /* Handler ID returned from Register */
HHUGEHEAP hHeap = NULL;                  /* Handle of heap for SIB's and EVCB's */
PSZ pszProcCntMutexName = (PSZ)FSSH_PROCCNT_MTX; /* Name of semaphore to control the */
                                         /*  usProcessCount variable. */
HMTX hmtxProcCnt = 0;                    /* Handle of semaphore to control the */
                                         /*  usProcessCount variable. */
HMTX hmtxGlobalData = 0;                 /* Handle of semaphore to control the */
                                         /*  Global Data structures. */
PFNTHREAD ShWriteRtn = (PFNTHREAD) FsshWrite;        /* Write routine */
PFNTHREAD ShReadRtn = (PFNTHREAD) FsshRead;          /* Read routine */

PFNSHINSTALLCHECK           pfnShInstallCheck = NULL;  /* variable to hold @ of install check routine */
PFNSHCSENDMSG               pfnShcSendMsg = NULL;      /* variable to hold @ of ShcSendMsg routine */
PFNSHCGETTIME               pfnShcGetTime = NULL;      /* variable to hold @ of ShcGetTime routine */
PFNSHCENABLEEVENT           pfnShcEnableEvent = NULL;  /* variable to hold @ of ShcEnableEvent routine */
PFNSHCDISABLEEVENT          pfnShcDisableEvent = NULL; /* variable to hold @ of ShcDisableEvent routine */
PFNSHCENABLESYNC            pfnShcEnableSync = NULL;   /* variable to hold @ of ShcEnableSync routine */
PFNSHCDISABLESYNC           pfnShcDisableSync = NULL;  /* variable to hold @ of ShcDisableSync routine */

ULONG         ulHandlerFlags = HANDLER_CAN_BE_SOURCE |
                               HANDLER_CAN_BE_TARGET ;

#pragma data_seg ()

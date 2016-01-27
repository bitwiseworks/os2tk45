/********************** START OF SPECIFICATIONS ****************************/
/*                                                                         */
/* Module Name: MInstall.h                                                 */
/*                                                                         */
/* DESCRIPTIVE NAME: Defines for creation of Installation Device Specific  */
/*                   Dlls.                                                 */
/*                                                                         */
/* ABSTRACT: This file contains information necessary for the creation of  */
/*           device specific Dlls. A message based interface exists so     */
/*           that theses Dlls can do such things as change config.sys,     */
/*           update the MME.INI file, and create WPS Objects.              */
/*                                                                         */
/* Copyright (c) International Business Machines Corporation 1990,1995     */
/*                         All Rights Reserved                             */
/*                                                                         */
/*********************** END OF SPECIFICATIONS *****************************/
#ifdef __cplusplus
   extern "C" {
#endif


#ifndef __MINSTALL_H_
#define __MINSTALL_H_

#define MAX_SETUP_STRING      1000     /* Longest setup string including   */
                                       /*   expanded macros                */

#define MAX_FOLDER_NAME       150      /* Longest desktop folder name      */

#define MAX_RESPONSE_LENGTH   256      /* Max device specific DLL response */

/* XLATOFF */
#pragma pack(4)
/* XLATON */

/***************************************************************************/
/* This is a function pointer prototype that describes the entry point     */
/* into the device specific DLL.                                           */
/*                                                                         */
/* Parameters:  HWND - Owner handle.                                       */
/*              PSZ  - Source path.                                        */
/*              PSZ  - Destination Drive (drive letter, colon - eg.  "X:") */
/*              PSZ  - Dll input parms in script file (DLL specific)       */
/*              HWND - Object window that receives message to do MCI and   */
/*                     CONFIG.SYS work.                                    */
/*              PSZ  - PSZ used for response file. (Is a CHAR [256] and    */
/*                     used for IN/OUT)                                    */
/***************************************************************************/
typedef ULONG APIENTRY VENDORFUNCTION(HWND,
                                          PSZ,
                                          PSZ,
                                          PSZ,
                                          HWND,
                                          PSZ);

typedef VENDORFUNCTION  * PVENDORFUNCTION;

/***************************************************************************/
/* MM_DISPATCHVARS and MM_DISPATCHMESSAGES() are two macros used by Device */
/* specific DLL's to keep the user interface responsive while they are     */
/* doing updates to MME.INI, config.sys, etc . . . This is necessary       */
/* because the device specific dll's are executing in install's msg queue  */
/* thread (thread 1).                                                      */
/*                                                                         */
/* MM_DISPATCHVARS should be declared in the scope required for the        */
/* MM_DISPATCHMESSAGES() macro to use them.                                */
/*                                                                         */
/* MM_DISPATHMESSAGES() should be used before and after every I/O intensive*/
/* operation.                                                              */
/*                                                                         */
/* An example would be:                                                    */
/*                                                                         */
/* void IOIntensive()                                                      */
/*    {                                                                    */
/*    MM_DISPATCHVARS;                                                     */
/*                                                                         */
/*    MM_DISPATHMESSAGES();                                                */
/*    DiskIO();                                                            */
/*    MM_DISPATHMESSAGES();                                                */
/*    }                                                                    */
/*                                                                         */
/* Please set the mouse pointer to SPTR_WAIT during operations lasting     */
/* than 1 second.                                                          */
/***************************************************************************/
#define MM_DISPATCHVARS        HAB MM_hab_ = WinQueryAnchorBlock(HWND_DESKTOP);\
                               QMSG MM_qmsg_

#define MM_DISPATCHMESSAGES()  while (WinPeekMsg(MM_hab_,                   \
                                                 &MM_qmsg_,                 \
                                                 (HWND)NULL,                \
                                                 0,                         \
                                                 0,                         \
                                                 PM_NOREMOVE))              \
                                  {                                         \
                                  WinGetMsg(MM_hab_, &MM_qmsg_, NULL, 0, 0);\
                                  WinDispatchMsg(MM_hab_, &MM_qmsg_);       \
                                  }

/***************************************************************************/
/* IM_MCI_EXTENDED_SYSINFO - Maps to MCI_SYSINFO.                          */
/*                                                                         */
/* mp1 = msg;                                                              */
/* mp2 = pvoid;                                                            */
/*                                                                         */
/* This will map to:                                                       */
/*         sysinfo.dwItem       = (ULONG)LONGFROMMP(mp1);                  */
/*         sysinfo.pSysInfoParm = PVOIDFROMMP(mp2);                        */
/*         mciSendCommand((USHORT)0,                                       */
/*                        (USHORT)MCI_SYSINFO,                             */
/*                        (ULONG)MCI_SYSINFO_ITEM,                         */
/*                        (ULONG)&sysinfo,                                 */
/*                        (USHORT)0);                                      */
/***************************************************************************/
#define IM_MCI_EXTENDED_SYSINFO     0x057E

/* XLATOFF */
#pragma pack(1)
/* XLATON */

typedef struct _INSTIOPROC
   {
   CHAR  fccIOProc[5];
   CHAR  szDLLName[CCHMAXPATH];
   CHAR  szProcName[32];  /* [128] */
   ULONG ulFlags;                          /* feature 5508 */
   ULONG ulExtendLen;                      /* feature 5508 */
   ULONG ulMediaType;                      /* feature 5508 */
   ULONG ulIOProcType;                     /* feature 5508 */
   CHAR  szDefExt[4];                      /* feature 5508 */
   } INSTIOPROC;

typedef INSTIOPROC *PINSTIOPROC;

typedef struct _INSTCODECINIFILEINFO         /* codecinifileinfo            */
   {                                                       /* feature 5572 */
   ULONG       ulStructLen;                  /* length of this structure    */
                                                           /* feature 5572 */
   CHAR        fcc[5];                       /* File Format ID              */
                                                           /* feature 5572 */
   CHAR        szDLLName[CCHMAXPATH];        /* DLL name string             */
                                                           /* feature 5572 */
   CHAR        szProcName[32];               /* Procedure name string       */
                                                           /* feature 5572 */
/* ULONG       ulCompressType;   */          /* Compression Type            */
   union
                                                           /* feature 5572 */
      {
      ULONG  ulCodecCompType;
                                                           /* feature 5572 */
      CHAR   fccCodecCompType[5];
      } x;
                                                           /* feature 5572 */
   ULONG       ulCompressSubType;            /* Compression SubType         */
                                                           /* feature 5572 */
   ULONG       ulMediaType;                  /* Media type                  */
                                                           /* feature 5572 */
   ULONG       ulCapsFlags;                  /* capabilities flags          */
                                                           /* feature 5572 */
   ULONG       ulFlags;                      /* flags                       */
                                                           /* feature 5572 */
   CHAR        szHWID[32];                   /* specific information        */
                                                           /* feature 5572 */
   ULONG       ulMaxSrcBufLen;               /* max source buffer length    */
                                                           /* feature 5572 */
   ULONG       ulSyncMethod;                  /* Synchronization method      */
                                                           /* feature 5572 */
   ULONG       fccPreferredFormat;           /* Preferred output format     */
                                                           /* feature 5572 */
   ULONG       ulXalignment;                 /* x alignment - video only    */
                                                           /* feature 5572 */
   ULONG       ulYalignment;                 /* y alignment - video only    */
                                                           /* feature 5572 */
   CHAR        szSpecInfo[32];               /* specific information        */
                                                           /* feature 5572 */
   } INSTCODECINIFILEINFO;
                                                           /* feature 5572 */

typedef INSTCODECINIFILEINFO * PINSTCODECINIFILEINFO;
                                                           /* feature 5572 */

/* XLATOFF */
#pragma pack( )
/* XLATON */

/***************************************************************************/
/* IM_MMIO_INSTALL - Install an IO-Proc                                    */
/*                                                                         */
/* mp1 = 0;                        ** Not used                             */
/* mp2 = (PINSTIOPROC)&instioproc; ** pointer to the INSTIOPROC struct     */
/***************************************************************************/
#define IM_MMIO_INSTALL    0x057F

/***************************************************************************/
/* IM_SPI_INSTALL - Install stream protocol information                    */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = (PSZ)pszDllPath;  ** Fully qualified path of a SPI resource DLL   */
/***************************************************************************/
#define IM_SPI_INSTALL     0x0580

#define INST_MAXCLASSNAME   256
#define INST_MAXTITLE       256
#define INST_MAXSETUPSTRING 1024
#define INST_MAXLOCATION    256


typedef struct _INSTOBJECTDATA
   {
   CHAR  achClassName[INST_MAXCLASSNAME];
   CHAR  achTitle[INST_MAXTITLE];
   CHAR  achSetupString[INST_MAXSETUPSTRING];
   CHAR  achLocation[INST_MAXLOCATION];
   ULONG ulFlags;
   } INSTOBJECTDATA;

typedef INSTOBJECTDATA *PINSTOBJECTDATA;

/***************************************************************************/
/* IM_CREATE_WPS_OBJECT - Install a folder and its contents                */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(pinstOBJECTdata);                                         */
/***************************************************************************/
#define IM_CREATE_WPS_OBJECT  0x0571

/***************************************************************************/
/* IM_DESTROY_WPS_OBJECT - Install a folder and its contents               */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(HOBJECT); ** or "<OBJECT_ID>"                             */
/***************************************************************************/
#define IM_DESTROY_WPS_OBJECT  0x0572

/***************************************************************************/
/* IM_LOG_ERROR - Write a message to the log file.                         */
/*                                                                         */
/* mp1 = (PSZ)pszStatement;   ** Should end with a '\n' and be NULL        */
/*                            **  terminated                               */
/* mp2 = 0;                   ** Unused                                    */
/***************************************************************************/
#define IM_LOG_ERROR       0x0573

/***************************************************************************/
/* CONFIGDATA - Pointer to this structure is passed in mp1 in all CONFIG   */
/*              messages.                                                  */
/***************************************************************************/
typedef struct _CONFIGDATA
   {
   LONG lLine;
   LONG lBufferLen;
   PSZ  pszBuffer;
   BOOL fAdd;
   } CONFIGDATA;

typedef CONFIGDATA *PCONFIGDATA;

/***************************************************************************/
/* IM_CONFIGENUMERATE - Get a line from the config.sys file.               */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGENUMERATE 0x0574

/***************************************************************************/
/* IM_CONFIGUPDATE - Update an entry in the config.sys file.               */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGUPDATE    0x0575

/***************************************************************************/
/* IM_CONFIGMERGE - Merge data into an existing config.sys entry.          */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGMERGE     0x0576

/***************************************************************************/
/* IM_CONFIGEREPLACE - Replace an entry that exists in config.sys.         */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGREPLACE   0x0577

/***************************************************************************/
/* IM_CONFIGNEW - Add a line to the config.sys file.                       */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGNEW       0x0578

/***************************************************************************/
/* IM_CONFIGDELETE - Delete a line (REM it out) from the config.sys file.  */
/*                                                                         */
/* mp1 = (PCONFIGDATA)pconfdata;                                           */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGDELETE    0x0579

/***************************************************************************/
/* IM_CONFIGQUERYCHANGED - Returns TRUE if config.sys has changed else     */
/*                         FALSE.                                          */
/*                                                                         */
/* mp1 = 0                                                                 */
/* mp2 = 0                                                                 */
/***************************************************************************/
#define IM_CONFIGQUERYCHANGED    0x057A


typedef struct _MIDIMAPINSTALLDATA
   {
   CHAR  achIniName[CCHMAXPATH];
   CHAR  achAppName[1024];
   CHAR  achKeyName[1024];
   CHAR  achDllName[CCHMAXPATH];
   ULONG ulResourceID;
   } MIDIMAPINSTALLDATA;

typedef MIDIMAPINSTALLDATA *PMIDIMAPINSTALLDATA;

/***************************************************************************/
/* IM_MIDIMAP_INSTALL - Install a midi map.                                */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(pmidimapinstalldata);                                     */
/*                                                                         */
/* Return 0 on success, else error occurred.                               */
/***************************************************************************/
#define IM_MIDIMAP_INSTALL 0x057B

typedef struct _INSTMCISENDCOMMAND
   {
   USHORT wDeviceID;
   USHORT wMessage;
   ULONG  dwParam1;
   ULONG  dwParam2;
   USHORT wUserParm;
   } INSTMCISENDCOMMAND;

typedef INSTMCISENDCOMMAND *PINSTMCISENDCOMMAND;

/***************************************************************************/
/* IM_MCI_SEND_COMMAND - Send an MCI command                               */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(PINSTMCISENDCOMMAND);                                     */
/*                                                                         */
/* Return mciSendCommand result.                                           */
/***************************************************************************/
#define IM_MCI_SEND_COMMAND         0x057C

typedef struct _INSTEAJOIN
   {
   CHAR achFileName[CCHMAXPATH];
   CHAR achEAFileName[CCHMAXPATH];
   } INSTEAJOIN;

typedef INSTEAJOIN *PINSTEAJOIN;

/***************************************************************************/
/* IM_EA_JOIN                                                              */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(PINSTEAJOIN);                                             */
/*                                                                         */
/* Return EA join result.                                                  */
/***************************************************************************/
#define IM_EA_JOIN                  0x057D

typedef struct _INSTEALONGNAMEJOIN
   {
   CHAR achLongName[CCHMAXPATH];
   CHAR achLongFileName[CCHMAXPATH];
   CHAR achEALongFileName[CCHMAXPATH];
   } INSTEALONGNAMEJOIN;

typedef INSTEALONGNAMEJOIN *PINSTEALONGNAMEJOIN;

/***************************************************************************/
/* IM_EA_LONG_NAME_JOIN                                                    */
/*                                                                         */
/* mp1 = 0;                ** Not used                                     */
/* mp2 = MPFROMP(PINSTEALONGNAMEJOIN);                                     */
/*                                                                         */
/* Return EA long name creation result.                                    */
/***************************************************************************/
#define IM_EA_LONG_NAME_JOIN                  0x0581
                                                           /* feature 6302 */
#define IM_QUERYPATH                          0x0582


/***************************************************************************/
/* IM_CODECINSTALL - Install a Codec-Proc                                  */
/*                                                                         */
/* mp1 = 0;                        ** Not used                             */
/* mp2 = (PINSTCODECINIFILEINFO)&instioproc;                               */
/*                                 ** pointer to the INSTIOPROC struct     */
/*                                                            feature 5572 */
/***************************************************************************/
#define IM_CODEC1INSTALL                       0x0583
                                                           /* feature 5572 */
#define IM_CODEC2INSTALL                       0x0584
                                                           /* feature 5572 */
#define IM_CODECDELINSTALL                     0x0585
                                                           /* 14650 */

/***************************************************************************/
/* IM_PROFILESTRINGINSTALL - add a profile string to an INI file           */
/*                                                                         */
/* mp1 = 0;                     ** Not used                                */
/* mp2 = MPFROMP(PPROFILESTRINGDATA)                                       */
/*                              ** pointer to the PROFILESTRING struct     */
/*                                                           feature 12057 */
/***************************************************************************/
typedef struct _PROFILESTRINGDATA  /* feature 12057 */
   {                               /* feature 12057 */
   CHAR  achInisName[CCHMAXPATH];  /* feature 12057 */
   CHAR  achAppsName[1024];        /* feature 12057 */
   CHAR  achKeysName[1024];        /* feature 12057 */
   CHAR  achDatasName[1024];        /* feature 12057 */
   } PROFILESTRINGDATA;            /* feature 12057 */

typedef PROFILESTRINGDATA *PPROFILESTRINGDATA; /* feature 12057 */

#define IM_PROFILESTRINGINSTALL                     0x0586  /* feature 12057 */

/******************************************************
/* feature 12060 add class regestering and changeing */
/*****************************************************/
typedef struct _CLASSCHANGEDATA                             /* feature 12060 */
   {                                                        /* feature 12060 */
   CHAR  achClassNewName[1024];                             /* feature 12060 */
   CHAR  achDllName[CCHMAXPATH];                            /* feature 12060 */
   CHAR  achReplaceClass[1024];                             /* feature 12060 */
   } CLASSCHANGEDATA;                                       /* feature 12060 */

typedef CLASSCHANGEDATA *PCLASSCHANGEDATA;                  /* feature 12060 */

#define IM_CLASSCHANGEINSTALL                       0x0587  /* feature 12060 */

#define INST_NAME  1024                                     /* defect 15663 */

typedef struct _PRFAPPENDDATA                               /* defect 15663 */
   {                                                        /* defect 15663 */
   CHAR    achIniFile[CCHMAXPATH]; /* INI file name */      /* defect 15663 */
           /* allowed constants are             */          /* defect 15663 */
           /*    "hini_user" for OS2.INI        */          /* defect 15663 */
           /*    "hini_system" for OS2SYS.INI   */          /* defect 15663 */
   CHAR    achAppName[INST_NAME];  /* application name */   /* defect 15663 */
   CHAR    achKeyName[INST_NAME];  /* key word */           /* defect 15663 */
   CHAR    achDefault[INST_NAME];  /* string to append */   /* defect 15663 */
   }  PRFAPPENDDATA;                                        /* defect 15663 */

typedef PRFAPPENDDATA * PPRFAPPENDDATA;                     /* defect 15663 */

/***************************************************************************/
/* IM_APPEND_PRF_STRING - ensure that a sub-string resides within a key    */
/*                        of an INI file, append if not present            */
/*                                                                         */
/* mp1 = 0;                        ** Not used                             */
/* mp2 = MPFROMP(PPRFAPPENDDATA);                                          */
/*                                 ** pointer to the PRFAPPENDDATA struct  */
/***************************************************************************/
#define IM_APPEND_PRF_STRING                   0x0588       /* defect 15663 */


#ifndef NO_ERROR
#define NO_ERROR 0
#endif

#define CFGERR_BASE                    0xFF00
#define CFGERR_START                   (CFGERR_BASE + 1)

#define CFGERR_OPENFAILED              (CFGERR_START +  1)
#define CFGERR_OUTOFMEMORY             (CFGERR_START +  2)
#define CFGERR_INVALIDPARAMETER        (CFGERR_START +  3)
#define CFGERR_INVALIDENUMFLAG         (CFGERR_START +  4)
#define CFGERR_ATTOP                   (CFGERR_START +  5)
#define CFGERR_ATBOTTOM                (CFGERR_START +  6)
#define CFGERR_BUFFEROVERFLOW          (CFGERR_START +  7)
#define CFGERR_LINENOTFOUND            (CFGERR_START +  8)
#define CFGERR_CANTPARSEBUFFER         (CFGERR_START +  9)
#define CFGERR_VARIABLENOTFOUND        (CFGERR_START + 10)

/* Line positions */
#define CONFIG_TOP      -1
#define CONFIG_BOTTOM   -2
#define CONFIG_NEXT     -3
#define CONFIG_PREV     -4
#define CONFIG_CURRENT  -5

/* XLATOFF */
#pragma pack( )
/* XLATON */

#endif /* #ifndef __MINSTALL_H_ */

#ifdef __cplusplus
}
#endif


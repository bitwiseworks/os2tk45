/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MCISPY.H
*
* DESCRIPTIVE NAME: MCI Spy Dialog Definitions
*
*
* FUNCTION: Component Global Data Structures, and Constant definitions
*
*
*********************** END OF SPECIFICATIONS ********************************/
#define DEVICECHARS             9   /* Max Chars in Device Name       */
#define HM_RESOURCEID           0   /* Help Resource ID               */
#define MAX_SYSINFO_RETURN      512 /* Maximum sysinfo return size    */
#define MAXDEVICES              20  /* Max Number of physical devices */
#define MAX_MCI_MSGS            59  /* Max Number of MCI Messages     */
#define NUM_COMMON_FLAGS         9  /* Max Number of Common Flags     */
#define MAXOPENS                20  /* Maximum Number of Simult Opens */

/* Device Specific data table */

typedef struct _deviceData
  {
   SZ     szPhysicalDevName[DEVICECHARS];/* Physical device name     */
   SZ     szProductInfo[MAX_PRODINFO];   /* Product Information      */
   SZ     szLogDevName[30];              /* Logical Device Name      */
  }PHYSDEV_DATA;

typedef struct _MCIMessageTable {
    USHORT  usMsg;                      /* MCI Message Value         */
    SZ      szMessage[80];              /* MCI Message String        */
    BOOL    fExclude;                   /* Filter Toggle Flag        */
}MCIMessageTable ;

typedef struct _MCIDevTypTable {
    USHORT  usDevType;                 /* Device Type ID             */
    SZ      szDevType[24] ;            /* Device Type String         */
}MCIDevTypTable ;

typedef struct _OpenPID {
  ULONG  ulDevId;                     /* Open Device Id      */
  ULONG  ulPID;                       /* Process Id          */
  BOOL   fExclude;                    /* Filter Toggle Flag  */
  USHORT usDevTyp;                    /* Device Type Const   */
}OPENPID;

typedef struct _OpenPIDAl {
  SZ     szAlias[124];                /* Open Device Alias   */
  ULONG  ulPID;                       /* Process Id          */
  BOOL   fExclude;                    /* Filter Toggle Flag  */
  SZ     szDevTyp[30];                /* Device Type String  */
}OPENPIDAL;

/* Maximum number of open devices is assumed to be 30. Modify
** this number as necessary
*/
typedef struct _OpenList {
  OPENPID      ulOpenList[20];   /* Device Ids of open list    */
  OPENPIDAL    szAlias[20];      /* Alias(es) of  open list    */
  ULONG      ulNumOpen;          /* Total Number of Opens      */
}OPEN_LIST;

/* Shared Memory Packet structure. This Structure is allocated by the
** stub MDM and passed to the application.
*/

typedef struct _ThreadBlk {
  USHORT    wMsg;                      /* MCI Message     */
  USHORT    wDevID;                    /* Device ID       */
  ULONG     dwParam1;                  /* Message Flags   */
  ULONG     dwParam2;                  /* Message Data    */
  USHORT    wUserParm;                 /* User parameter  */
  SZ        szCmd[1024];               /* String Command  */
  PVOID     pvNotPacket;               /* Notify Packet Base Address */
  ULONG     ulPID;                     /* Process Id      */
  ULONG     ulReturnCode;              /* Return Code     */
  HEV       SysSndsEvtSem;             /* System Sounds   */
  HEV       SysSndsSyncSem;            /* System Sounds   */
}THREAD_BLOCK;

typedef struct _spyblock {
  HWND            hwndc1, hwnd1;                   /* Window Handles        */
  HWND            hwndMLE;                         /* Cmnd monitor MLE      */
  HWND            hwndHelpInstance;                /* Help Inst habdle      */
  HAB             hab;                             /* Anchor Block          */
  QMSG            qmsg;                            /* Message Queue         */
  HMQ             hmq;                             /* Handle to Queue       */
  HPS             hps;                             /* Presentation Space    */
  IPT             ipt;                             /* MLE Insertion point   */
  HFILE           hfileLog;                        /* Script file handle    */
  HMTX            hmtxPrntSem;                     /* Print Semaphore       */
  CHAR            szLogFile[254];                  /* Log File name         */
  PVOID           pvPacket;                        /* Shared Memory pointer */
  FONTMETRICS     fm;                              /* Font metrics struct   */
  PHYSDEV_DATA    PhysicalDevData[MAXDEVICES] ;    /* Audio Device Info     */
  USHORT          usNumDevices ;                   /* Total Number of devices */
  OPEN_LIST       OpenList;                        /* List of Open Instances */
  BOOL            fNoCmd,fNoStr,fNoDrv;            /* API Filter Flags      */
  BOOL            fNoLog;                          /* Message Logging Flag  */
  BOOL            fDevTypFil;                      /* Device Type     Flag  */
  BOOL            fDevIdFil;                       /* Device Id       Flag  */
  ULONG           ulDevTypFil[MAXDEVICES];         /* Filter Dev Types      */
  SZ              szDevTypFil[MAXDEVICES][30];     /* Filter Dev Types      */
  SZ              szTitle[160];                    /* Title Text            */
  USHORT          usNumFilDevs;                    /* Number Of Filter Devs */
  LONG (* APIENTRY QueryOpenInstance) (OPEN_LIST *, BOOL);/* PFN QueryOpenInstance*/
  LONG (* APIENTRY SystemSounds) (BOOL);           /* PFN SystemSounds  */
}MCISPY_BLOCK;

/* MCISPY Internal function prototypes  */

FNWP   AboutDlgProc           ;     /* Product Info Dialog Procedure       */
FNWP   FilterMsgDlgProc       ;     /* Filter Messages Dialog Procedure    */
FNWP   FilterDevTypDlgProc    ;     /* Filter Device Type Dialog Procedure */
FNWP   FilterDevIdDlgProc     ;     /* Filter Device Id  Dialog Procedure  */
FNWP   SpyWinProc             ;     /* Main Window Procedure               */
FNWP   ViewInstDevDlgProc     ;     /* Installed Devices Dialog Procedure  */
FNWP   ShowFlagDlgProc        ;     /* Show MCI Flag Value Dialog Proc     */
VOID   Initialize(MCISPY_BLOCK * ); /* MCISPY Application Initialization   */
VOID   Terminate (HMQ hmq);         /* MCISPY Application Termination      */


VOID   RegisterSpyWithMDM (MCISPY_BLOCK *)  ;     /* MCISPY Application Registration     */

PSZ    LongToString (LONG );             /* Conversion helper     */

LONG   mprintf(PSZ format, ...);         /* Error Print routine   */

VOID   GetSharedMemory(MCISPY_BLOCK *);  /* Obtain Shared Memory  */

void   DisplayMCIMsg (MCISPY_BLOCK *);   /* Display MCI Messages  */

void   DisplayMCIString (MCISPY_BLOCK * pSpyBlock); /* String Interface */

APIRET OpenFile(PSZ szFileName,ULONG *pFileHandle); /* Open Log File   */

APIRET Write2File(HFILE FileHandle,PSZ buffer);     /* Write to a Log file */

RC     OpenFileDlg (HWND hwndOwner,PSZ pszTitle,PSZ FileName, PSZ FullFile);

VOID   QueryInstalledDevices (MCISPY_BLOCK * pSpyBlock);

/* Application Constant Definitions */

#define WND_FRM                 2000
#define IDM_PRODUCTINFO         106                    /* Product Info    */
#define IDR_BITMAP              108                    /* Bitmap          */
#define IDC_BITMAP              109                    /* Bitmap          */
#define IDM_HELP                300                    /* Help Menu       */
#define IDM_HELP_GEN            301                    /* General help    */
#define IDM_HELP_KEYS           302                    /* Keys Help       */
#define IDM_HELP_PRODINFO       303                    /* Product Info    */
#define IDM_HELP_USING          304                    /* Using Help      */
#define IDM_HELP_INDEX          305                    /* Help Index      */
#define ID_ACTLIST              003                    /* List box ID     */
#define IDM_OPEN                325                    /* Menu Script     */
#define IDM_SAVE                326                    /* Menu Script     */
#define IDM_CLOSE               327                    /* Menu Script     */
#define IDM_LOG                 328                    /* Menu Script     */
#define IDM_FILE                329                    /* Menu Script     */
#define IDM_IDEVS               333                    /* Menu Script     */
#define IDM_VFILMSG             334                    /* Menu Script     */
#define IDM_VIEW                336                    /* Menu Options    */
#define IDM_INSTDATA            338                    /* Menu Options    */
#define IDM_FILMSG              340                    /* Menu Script     */
#define IDM_FILDEVTYP           341                    /* Menu Script     */
#define IDM_FILDEVID            342                    /* Menu Script     */
#define IDM_EDIT                344                    /* Menu Script     */
#define IDM_EDCOPY              345                    /* Menu Script     */
#define IDM_EDCLEAR             346                    /* Menu Script     */
#define IDM_EDDISCARD           347                    /* Menu Script     */
#define IDM_EDFONT              348                    /* Menu Script     */
#define IDM_NOSTR               349                    /* Menu Script     */
#define IDM_NOCMD               350                    /* Menu Script     */
#define IDM_NONOTIFY            351                    /* Menu Script     */
#define IDM_FLAGS               352                    /* Menu Script     */
#define IDM_NOSYS               353                    /* Menu Script     */
#define ID_INSTLDHNDLRS         759                     /* Instl Hndlrs    */
#define ID_DEVNAMESDLG          760                     /* Instl Hndlr Dlg */
#define ID_FILTERMSG            761                     /* Instl Hndlr Dlg */
#define ID_INSTINFODLG          762                     /* Instl Hndlr Dlg */
#define ID_INSTLSTDLG           763                     /* Instl Hndlr Dlg */
#define ID_FILTERDEVIDS         766                     /* Instl Hndlr Dlg */
#define ID_FILTERDEVTYP         767                     /* Instl Hndlr Dlg */
#define ID_FLAGSDLG             768                     /* Instl Hndlr Dlg */
#define LI_FILTERMSG            400                    /* Protocol List ID */
#define LI_DEVNAMES             401                    /* Protocol List ID */
#define LI_OPENINST             402                    /* Active Hndlrs LID*/
#define LI_MSGS                 403                    /* Active Hndlrs LID*/
#define LI_FLAGS                404                    /* Active Hndlrs LID*/
#define LI_FILTERDEVS           405                    /* Active Hndlrs LID*/
#define PB_HELP                 505                    /* Help Button      */
#define PB_OK                   506                    /* OK Button        */
#define PB_ALL                  510                    /* OK Button        */
#define PB_NONE                 511                    /* OK Button        */
#define PB_TIMERON              512                    /* OK Button        */
#define PB_TIMEROFF             513                    /* OK Button        */
#define PB_ACTON                514                    /* OK Button        */
#define PB_ACTOFF               515                    /* OK Button        */
#define PB_CLEAR                516                    /* OK Button        */
#define EF_FLAG                 700                    /* Static Text ID   */

#define MSG_BOX_ID              623                    /* Msg Box ID        */
#define SPY_NOTIFY              0x5001                 /* Command Interface */
#define SPY_STRING              0x5002                 /* String Interface  */
#define SPY_DRIVER              0x5003                 /* Driver Notify     */
#define SPY_ERRRC               0x5004                 /* Error Conditions  */
#define SPY_DEVICE              0x5005                 /* Resource Managemt */

/* MCISPY Help Manager  definitions */

#define  MAINHELPTABLE           700    /* Main Help Table ID    */
#define  HELP_MAIN               703
#define  INDEXHELP               801
#define  GENERALHELP             802
#define  USINGHELP               803
#define  HELPPRODINFO            804
#define  GENHLP                  29
#define  KEYSHELP                23
#define  FILTERMSGHLP            30
#define  FILTERDEVTYPHLP         31
#define  FILTERDEVIDHLP          32
#define  SHOWFLAGHLP             33
#define  INSTDEVHLP              34
#define  STDHLP                  35
#define  HELP_SUBTABLE_SPY       36
#define  SPYHLP                  37

#define HELP_TABLE              807  //Window ID

#define  InsertListItem(LboxID,     \
                        InsertOrder,\
                        Text)       \
         (WinSendDlgItemMsg (hwnd,                      \
                             LboxID,                    \
                             LM_INSERTITEM,             \
                             MPFROMSHORT(InsertOrder),  \
                             MPFROMP(Text)))
#define  InitMLE(hwnd,     \
                 MLEBuffer) \
         (WinSendMsg (hwnd,                \
                      MLM_SETIMPORTEXPORT, \
                      MPFROMP(MLEBuffer),  \
                      MPFROMSHORT((USHORT)sizeof(MLEBuffer))))

#define  SetMLEFormat(hwnd)         \
         (WinSendMsg (hwnd,         \
                      MLM_FORMAT,   \
                      MPFROMSHORT(MLFIE_NOTRANS),\
                      MPFROMSHORT(0)))


#define ImportMLE(hwnd,            \
                  pt,              \
                  MLEBuffer)       \
        (WinSendMsg (hwnd,         \
                     MLM_IMPORT,   \
                     MPFROMP(&pt), \
                     MPFROMSHORT((USHORT)strlen(MLEBuffer))))


#define DeleteMLE(hwnd,                       \
                  MLEBuffer)                  \
        (WinSendMsg (hwnd,                    \
                     MLM_DELETE,              \
                     MPFROMLONG(ipt),         \
                     MPFROMLONG((ULONG)sizeof(MLEBuffer))))



#define CopyMLE(hwnd)                        \
        (WinSendMsg (hwnd,                   \
                     MLM_COPY,               \
                     MPFROMLONG(0),          \
                     MPFROMLONG(0)))

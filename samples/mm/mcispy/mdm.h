/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MDM.H
*
* DESCRIPTIVE NAME: Media Device Manager Component Definitions.
*
* FUNCTION:  Define Component Data Structures.
*
*
*********************** END OF SPECIFICATIONS ********************************/
#define  MAXSIZE      1024
#define  MAXOPENS     20
#define  SPY_STRING   0x5002                 /* String Interface Const   */
#define  SPY_NOTIFY   0x5001                 /* Command Interface Const  */
#define  SPY_DRIVER   0x5003                 /* Driver Notify  Const     */
#define  SPY_ERRRC    0x5004                 /* Error Conditions Const   */
#define  SPY_DEVICE   0x5005                 /* Resouce Change Msgs Const*/
#define  FAILURE      0L                     /* Loader Indication Flag   */
#define  SUCCESS      1L                     /* Loader Indication Flag   */
#define INTIALIZE     0                      /* Loader Indication Flag   */

/* Send Command Dynamic PFN */
typedef ULONG (APIENTRY fDRIVERENTRY) (USHORT lpInstance,
                                       USHORT wMessage,
                                       ULONG dwParam1,
                                       PVOID dwParam2,
                                       USHORT wUserParm);
typedef fDRIVERENTRY   *DRIVERENTRY;

/* Send String Dynamic Function Prototype */

typedef ULONG (APIENTRY  fSENDSTR)   (PSZ lpstrCommandBuf,
                                      PSZ lpstrReturnString,
                                      USHORT  wReturnLength,
                                      HWND  hCallBack,
                                      USHORT  wUserParm );


typedef fSENDSTR  *SENDSTR;

/*
** The Thread Block is a shared memory structure that is identical
** between the stub DLL and the Application (MCISPY.EXE). This structure
** Should remain identical on both sides.
*/

typedef struct _ThreadBlk {
  USHORT      wMsg;                   /* MCI Message     */
  USHORT      wDevID;                 /* Device ID       */
  ULONG       dwParam1;               /* Message Flags   */
  ULONG       dwParam2;               /* Message Data    */
  USHORT      wUserParm;              /* User parameter  */
  SZ          szCmd[1024];            /* String Command  */
  PVOID       pvNotPacket;            /* Notify Packet Base Address */
  ULONG       ulPID;                  /* Process Id      */
  ULONG       ulReturnCode;           /* Return Code     */
  HEV         SysSndsEvtSem;          /* System Sounds   */
  HEV         SysSndsSyncSem;         /* System Sounds   */
}THREAD_BLOCK;

typedef struct _OpenPID {
  ULONG  ulDevId;                /* Open Device Id      */
  ULONG  ulPID;                  /* Process Id          */
  BOOL   fExclude;               /* Filter Toggle Flag  */
  USHORT usDevTyp;               /* Device Type Const   */
}OPENPID;

typedef struct _OpenPIDAl {
  SZ     szAlias[124];           /* Open Device Alias   */
  ULONG  ulPID;                  /* Process Id          */
  BOOL   fExclude;               /* Filter Toggle Flag  */
  SZ     szDevTyp[30];           /* Device Type String  */
}OPENPIDAL;

/* This structure is identical between the application and the
** Stub DLL.
*/

typedef struct _OpenList {
  OPENPID      ulOpenList[20];   /* Device Ids of open list    */
  OPENPIDAL    szAlias[20];      /* Alias(es) of  open list    */
  ULONG        ulNumOpen;        /* Total Number of Opens      */
}OPEN_LIST;

/* Function Prototypes */

ULONG APIENTRY mciSendCommand  (USHORT   wDeviceID,
                                USHORT   wMessage,
                                ULONG    dwParam1,
                                PVOID    dwParam2,
                                USHORT   wUserParm);


ULONG APIENTRY  mciSendString   (PSZ lpstrCommandBuf,
                                 PSZ lpstrReturnString,
                                 USHORT  wReturnLength,
                                 HWND  hCallBack,
                                 USHORT  wUserParm );


int _CRT_init(void);          /* C Runtime Initialization */
int _CRT_term(void);          /* C Runtime Termination    */

/* DLL Initilaztion/Termination  Routine */
unsigned long _System _DLL_InitTerm (ULONG hModule,ULONG ulFlag);

/* SPY Registration Routine   */
APIRET APIENTRY RegisterSpy (HWND hwndspy,ULONG ulApiMode);

/* Query Instance Information Routine */

APIRET APIENTRY QueryOpenInstance (OPEN_LIST * pulList, BOOL fAlias);

/* Sytem Input Hook Procedure to monitor Resource Change Messages */

BOOL APIENTRY  InputHookProc   (HAB   hab,  PQMSG pQmsg,ULONG ulfs);

VOID _Optlink SystemSoundsThread(PVOID ptemp);

APIRET APIENTRY SpySystemSounds (BOOL fAlias) ;

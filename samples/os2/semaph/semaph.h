/*static char *SCCSID = "@(#)semaph.h   6.12 92/02/19";*/
/*==============================================================*\
 *                                                              *
 *  SEMAPH.H Sample PM application header file                  *
 *      (C) Copyright IBM Corporation 1992.                     *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide constants    *
 *  and structure definitions.                                  *
 *                                                              *
\*==============================================================*/
#define INCL_WIN
#define INCL_GPI
#define INCL_DEV
#define INCL_AVIO
#define INCL_PIC
#define INCL_HELP
#define INCL_ORDERS
#define INCL_BITMAPFILEFORMAT
#define INCL_FONTFILEFORMAT
#define INCL_WINMESSAGEMGR
#define INCL_DOSPROCESS
#define INCL_ERRORS
#define INCL_BASE

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IDR_RESOURCE                    1000     /* resource identifier */
#define ID_TIMER                           1
#define ID_NULL                           -1
#define IDR_BITMAP                         2
#define IDC_BITMAP                         3

/*--------------------------------------------------------------*\
 *  Menu item ids                                               *
\*--------------------------------------------------------------*/
#define IDM_OPTIONS_NTHRDS              4100
#define IDM_OPTIONS_NTHRDS_2            4102
#define IDM_OPTIONS_NTHRDS_4            4104
#define IDM_OPTIONS_NTHRDS_8            4108
#define IDM_OPTIONS_NTHRDS_16           4116
#define IDM_OPTIONS_TIMEOUT             4200

#define IDM_SEMSTART                    4211
#define IDM_SEMSTART_MANUAL             4212
#define IDM_SEMSTART_AUTO               4213
#define IDM_SEMSTOP                     4214
#define IDM_SEM_RESUMEAUTO              4215
#define IDM_POST                        4216
#define IDM_MSGBOX                      4217

#define IDM_HELP                        4230
#define IDM_HELPINDEX                   4231
#define IDM_GENERALHELP                 4232
#define IDM_TUTORIAL                    4233
#define IDM_USINGHELP                   4234
#define IDM_HELPPRODUCTINFO             4235

/* private message */
#define IDM_STOPFINISHED                4240

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
         /* for help menu module */
#define IDS_APPNAME                      101
#define IDS_HELPLIBRARYNAME              102
#define IDS_HELPWINDOWTITLE              103
#define IDS_TITLE                        104
#define IDS_UNTITLED                     105

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define SEMAPH_HELP_TABLE                   9000

/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define SUBTABLE_MAIN                       9100
#define PANEL_MAIN                          9110
#define PANEL_HELP                          9120
#define PANEL_GENERALHELP                   9130
#define PANEL_USINGHELP                     9140
#define PANEL_TUTORIAL                      9150
#define PANEL_HELPINDEX                     9160
#define PANEL_HELPPRODUCTINFO               9170
#define PANEL_KEYSHELP                      9180

#define PANEL_NTHRDSHELP                    9181
#define PANEL_THRDS2HELP                    9182
#define PANEL_THRDS4HELP                    9183
#define PANEL_THRDS8HELP                    9184
#define PANEL_THRDS16HELP                   9185

#define PANEL_STARTHELP                     9186
#define PANEL_AUTOHELP                      9187
#define PANEL_MANUALHELP                    9188

#define PANEL_TIMEOUTHELP                   9189
#define PANEL_STOPHELP                      9191
#define PANEL_RESUMEHELP                    9192
#define PANEL_POSTHELP                      9193

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_PRODUCTINFODLG             9200
#define PANEL_PRODUCTINFODLG                9210
#define PANEL_PRODUCTINFO_OK                9211

#define ENTERTEXTDLG                      0x1000
#define DEMOMSGBOX                        0x1006
#define TIMERBOX                          0x1001
#define REFRESHBOX                        0x1002

#define WM_USERCHANGED               (WM_USER+1)
/*--------------------------------------------------------------*\
 *  Message item ids                                            *
\*--------------------------------------------------------------*/
#define IDD_PRODUCTINFO                      152
#define IDD_ENTRY                            153
#define IDD_MLE                              154
#define IDD_RADIO1                           155
#define IDD_RADIO2                           156
#define IDD_RADIO3                           157
#define IDD_PUSHBUTTON                       158
#define IDD_3STATE                           159
#define IDD_CHECKBOX                         160
#define IDD_LISTBOX                          161
#define IDD_MULTISELLISTBOX                  162
#define IDD_SIMPLE                           163
#define IDD_DROPDOWN                         164
#define IDD_DROPDOWNLIST                     165
#define IDD_STATICTEXT                       166
#define IDD_GROUPBOX                         167
#define IDD_ICON                             168
#define IDD_STATICFRAME                      169
#define IDD_STATICRECT                       170
#define IDD_FORECOLORLIST                    171
#define IDD_BACKCOLORLIST                    172
#define IDD_FONTLIST                         173
#define IDD_SAMPLETEXT                       174
#define IDD_DONE                             175

/*--------------------------------------------------------------*\
 *  Messagetable ids                                            *
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                     301
#define IDMSG_MAINWINCREATEFAILED            302
#define IDMSG_CANNOTGETHPS                   303
#define IDMSG_CANNOTLOADSTRING               304
#define IDMSG_CANNOTLOADEXITLIST             305
#define IDMSG_HELPLOADERROR                  306
#define IDMSG_HELPDISPLAYERROR               307
#define IDMSG_CANNOTSTART                    308

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define HELPLIBRARYNAMELEN 20  /* The length of library name */
#define MESSAGELEN        150  /* maximum length for messages */
#define RETURN_SUCCESS      0   /* successful return in DosExit */
#define RETURN_ERROR        1        /* error return in DosExit */
#define BEEP_WARN_FREQ     60      /* frequency of warning beep */
#define BEEP_WARN_DUR     100       /* duration of warning beep */
#define MAXWIDTH           80
#define NUMLINES           25
#define MAXTEXTLEN         40    /* maximum text length for window */
#define MAXAPPNAMELEN      15

#define STACKSIZE     0X10000L
#define TIMEOUTPERIOD      33L
#define EVENTSEM            0
#define STOPSEM            -1
#define BASETEN            10

#define UNOWNED            -1
#define NUMUSERS            8
#define MAXUSERS           16
#define CNT_SQUARES_EDGE    8
#define MAXRESOURCES        (CNT_SQUARES_EDGE*CNT_SQUARES_EDGE)
#define CNT_POINTS_EDGE     (CNT_SQUARES_EDGE+1)
#define DIRTY               1
#define CNT_SZHITS          5

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
HWND         hwndMainFrame;            /* handle to the main frame window */
HWND         hwndMain;                /* handle to the main client window */
HAB          hab;                         /* anchor block for the process */
HMQ          hmq;                 /* handle to the process' message queue */
CHAR         szAppName[MAXAPPNAMELEN];     /* buffer for application name */
USHORT       usConsumerThreadsCreated;
TID          tidAutoThread;
ULONG        ulTimeout;
CHAR         szUntitled[MESSAGELEN];      /* buffer for "Untitled" string */
      /* for help menu module */
BOOL         fHelpEnabled;        /* flag to determine if help is enabled */
static CHAR  szLibName[HELPLIBRARYNAMELEN];
static CHAR  szWindowTitle[HELPLIBRARYNAMELEN];
static HWND  hwndHelpInstance;

CHAR         szWindowText[MAXTEXTLEN];
USHORT       cNumUsers;
USHORT       fSemaphoreStarted;
USHORT       fSemaphoreWasStarted;
ULONG        fAutoMode;
HEV          hevStop, hevStopAuto, hevItem;
HMTX         hmtxOwnResource;
HMUX         hmuxResource;

/*--------------------------------------------------------------*\
 *  Data structures and typedefs                                *
\*--------------------------------------------------------------*/
struct _WINDOWDATA  {
    LONG clrFore;
    LONG clrBack;
    UCHAR *szText[MAXTEXTLEN + 1];
};
typedef struct _WINDOWDATA WINDOWDATA;
typedef WINDOWDATA * PWINDOWDATA;

typedef struct _thr {
    TID   tid;
    LONG  lHits;
    RECTL rcl;
} THRDATA;

typedef struct _sq {           /* square */
    RECTL  rcl;
    HEV    hev;
    USHORT usOwner;
    } SQ;

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
SQ           aSquares[MAXRESOURCES];
THRDATA      thrConsumers[MAXUSERS];

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/
/* from sem_main.c*/
INT main(VOID);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
VOID MessageBox(HWND, LONG, BOOL);
VOID MainCommand(HWND, MPARAM, MPARAM);

/* from sem_init.c*/
BOOL Init(VOID);
VOID APIENTRY ExitProc(ULONG);

/* from sem_user.c */
VOID UserCommand(MPARAM, MPARAM);

/* from sem_pnt.c */
VOID MainPaint(HWND hwnd);
VOID DrawResource(PRECTL, COLOR);
VOID DrawRects(HPS);
VOID DrawStats(ULONG);
VOID SetRectPositions(SHORT, SHORT, SHORT, SHORT);
VOID InfoLine (HPS);

/* from semaph.c */
VOID InitSemaphExample(VOID);
VOID UpdateStats (VOID);
VOID SemError(PSZ,ULONG);
ULONG  CreateAllSems(VOID);
INT StartSemExample(VOID);
VOID SignalUserEvent(PULONG);
ULONG SetAutoMode(VOID);
VOID BeginStop (PULONG);
VOID ThreadConsumer(PVOID);
VOID RunAuto(PVOID);
VOID StopSemaphore(PVOID);

/* from sem_help.c */
VOID InitHelp(VOID);
VOID HelpHelpForHelp(MPARAM);
VOID HelpExtended(MPARAM);
VOID HelpKeys(MPARAM);
VOID HelpIndex(MPARAM);
VOID HelpAbout(MPARAM);
VOID DisplayHelpPanel(LONG);
VOID DestroyHelpInstance(VOID);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
VOID FixSysMenu(HWND);

/* from sem_dlg.c */
MRESULT EXPENTRY TimeDlgProc(HWND, ULONG, MPARAM, MPARAM);

/******************************************************
 *
 * Cruiser version define
 *
 ******************************************************/
#define     VER678

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif

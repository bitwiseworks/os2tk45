/*static char *SCCSID = "@(#)dllapi.h	6.13 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  DLLAPI.H - DLL Sample PM application header file            *
 *      (C) Copyright IBM Corporation 1992.                     *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide constants    *
 *  and structure definitions.                                  *
 *                                                              *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#define  INCL_WIN
#define  INCL_HELP
#define  INCL_WINDIALOGS            /* For dialog box functions */
#define  INCL_WINMENU               /* For menu control function */
#define  INCL_WINERRORS
#define  INCL_GPI
#define  INCL_DOSEXCEPTIONS         /* For exception management */
#define  INCL_DOSPROCESS
#define  INCL_DOSSIGNALS
#define  INCL_DOSMISC
#define  INCL_DOSMEMMGR             /* For memory management    */
#define  INCL_DOSERRORS             /* For error code definition */
#define  INCL_DOSNLS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllcomm.h"

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define ID_RESOURCE                           1  /* resource identifier */
#define ID_CHILDWIN                           2
#define ID_NULL                               0
#define IDR_BITMAP                            5
#define IDC_BITMAP                            6

/*--------------------------------------------------------------*\
 *  Menu item ids                                               *
\*--------------------------------------------------------------*/
#define IDM_HELPINDEX                       1010
#define IDM_GENERALHELP                     1011
#define IDM_TUTORIAL                        1012
#define IDM_USINGHELP                       1013
#define IDM_HELPPRODUCTINFO                 1014

#define IDM_MSGBOX                          1020
#define IDM_VIEWSTRING                      1021
#define IDM_SEARCH                          1022
#define IDM_OPEN                            1023
#define IDM_CALCUL                          1024

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define DLL_HELP_TABLE                      4000

/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define SUBTABLE_MAIN                       4100
#define PANEL_MAIN                          4110
#define PANEL_HELP                          4120
#define PANEL_GENERALHELP                   4130
#define PANEL_USINGHELP                     4140
#define PANEL_TUTORIAL                      4150
#define PANEL_HELPINDEX                     4160
#define PANEL_HELPPRODUCTINFO               4170
#define PANEL_KEYSHELP                      4180

#define PANEL_SEARCHHELP                    4190
#define PANEL_OPENHELP                      4191
#define PANEL_CALCULHELP                    4192

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_PRODUCTINFODLG             4200
#define PANEL_PRODUCTINFODLG                4210
#define PANEL_PRODUCTINFO_OK                4220

#define SUBTABLE_OPENBOXDLG                 4230
#define PANEL_OPENBOXDLG                    4231
#define PANEL_OPENBOX_OK                    4232

#define SUBTABLE_CALCULBOXDLG               4235
#define PANEL_CALCULBOXDLG                  4236
#define PANEL_CALCULBOX_OK                  4237

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
#define IDS_APPNAME                          101
#define IDS_HELPLIBRARYNAME                  102
#define IDS_HELPWINDOWTITLE                  103
#define IDS_TITLE                            104
#define IDS_UNTITLED                         105

/*--------------------------------------------------------------*\
 *  Message item ids
\*--------------------------------------------------------------*/
#define IDD_PRODUCTINFO                      152
#define IDD_GETPATTERN                       153
#define IDD_PATH                             154
#define IDD_PATTERN                          155
#define IDD_PATHTEXT                         156
#define IDD_PATTERNTEXT                      157
#define IDD_OPENFILE                         158
#define IDD_DISPLAYTEXT                      159
#define IDD_TEXT                             160
#define IDD_GETMATHINFO                      170
#define IDD_OPERAND1TEXT                     171
#define IDD_OPERATIONTEXT                    172
#define IDD_OPERAND2TEXT                     173
#define IDD_OPERAND1                         174
#define IDD_OPERATION                        175
#define IDD_OPERAND2                         176
#define IDD_DISPLAYVALUE                     177
#define IDD_VALUE                            178
#define IDD_DSPFILESTRUCT                    179
#define IDD_ACCESSDIRT                       180
#define IDD_ACCESSBACKUP                     190

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
#define IDMSG_ERRORSETPATH                   308
#define IDMSG_FILENOTFOUND                   309
#define IDMSG_DRIVELOCKED                    310
#define IDMSG_PATHNOTFOUND                   311
#define IDMSG_BUFFEROVERFLOW                 312
#define IDMSG_OPENFAILED                     313
#define IDMSG_ACCESSGUARDPAGE                314
#define IDMSG_COMMITTTOREAD                  315
#define IDMSG_PROTECTIONERROR                316
#define IDMSG_ERRORINVALIDDRIVE              317
#define IDMSG_ACCESSDENIED                   318
#define IDMSG_RETURNERROR                    319
#define IDMSG_DSPTEXTERROR                   320
#define IDMSG_DIVIDEZERO                     321
#define IDMSG_ERROROPERAND                   322
#define IDMSG_ERRORDIGIT                     323

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define RETURN_SUCCESS            0      /* successful return in DosExit */
#define RETURN_ERROR              1           /* error return in DosExit */
#define BEEP_WARN_FREQ           60         /* frequency of warning beep */
#define BEEP_WARN_DUR           100          /* duration of warning beep */
#define MAXAPPNAMELEN            15
#define HELPLIBRARYNAMELEN       20        /* The length of library name */

#define MAX_EDIT_BUFF            21    /* length of string to query size */

#define MAXTEXTLEN               40    /* maximum text length for window */
#define MESSAGELEN              200       /* maximum length for messages */
#define HORZ_INDENT              10
#define VERT_TEXT_POS            15     /* from top to display text info */

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
CHAR  szWindowText[MAXTEXTLEN];                  /* text drawn in window */
HWND  hwndMainFrame;                  /* handle to the main frame window */
HWND  hwndMain;                      /* handle to the main client window */
HWND  hwndChild;                           /* handle to the child window */
HAB   hab;                               /* anchor block for the process */
HMQ   hmq;                       /* handle to the process' message queue */
CHAR  szAppName[MAXAPPNAMELEN];    /* buffer for application name string */
CHAR  szUntitled[MESSAGELEN];            /* buffer for "Untitled" string */
      /* for help menu module */
BOOL   fHelpEnabled;             /* flag to determine if help is enabled */
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance;

OBJSTRUCT  ObjInfo;          /* Store Driver, Path & Pattern information */
PFILELIST  pFindFile, pBackup, pFlList;           /* List of found files */
SHORT      sStatus;                             /* Keep status of window */
FILEINFO   FileInfo;                   /* Store file's text after opened */
MATHELEMENT MathInfo;             /* Store calculator's element & result */

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/
/* from dllapi.c */
INT     main(VOID);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
VOID    MainCommand(HWND, MPARAM, MPARAM);
VOID    MainPaint(HWND);
VOID    MainControl(HWND, MPARAM, MPARAM);
VOID    UserCommand(HWND, MPARAM, MPARAM);
VOID    MessageBox(HWND, LONG, PSZ, LONG, BOOL);
ULONG   GPFHandler(EXCEPTIONREPORTRECORD *,
                   EXCEPTIONREGISTRATIONRECORD *,
                   PCONTEXTRECORD,
                   PVOID);
/* from dlapi_dg.c */
MRESULT EXPENTRY GetPatternDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY DispFileDlgProc (HWND, ULONG, MPARAM, MPARAM);
VOID    DisplayFile (HWND, RECTL, PPFILELIST);
MRESULT EXPENTRY OpenDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY DisplayDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY MathDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ResultDlgProc(HWND, ULONG, MPARAM, MPARAM);

/* from dlapi_in.c */
BOOL    Init(VOID);
VOID APIENTRY ExitProc(ULONG);

/* from dlapi_hp.c */
VOID    InitHelp(VOID);
VOID    HelpHelpForHelp(MPARAM);
VOID    HelpExtended(MPARAM);
VOID    HelpKeys(MPARAM);
VOID    HelpIndex(MPARAM);
VOID    HelpAbout(MPARAM);
VOID    DisplayHelpPanel(LONG);
VOID    DestroyHelpInstance(VOID);
VOID    FixSysMenu(HWND);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);

/*static char *SCCSID = "@(#)vmm.h      6.16 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  VMM.H - Sample PM application header file                   *
 *      (C) Copyright IBM Corporation 1992                      *
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
#define  INCL_WINDIALOGS
#define  INCL_WINMENU
#define  INCL_GPI
#define  INCL_DOSEXCEPTIONS
#define  INCL_DOSPROCESS
#define  INCL_DOSSIGNALS
#define  INCL_DOSMISC
#define  INCL_DOSMEMMGR
#define  INCL_DOSERRORS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define IDR_RESOURCE                        1000  /* resource identifier */
#define ID_NULL                               -1
#define IDR_BITMAP                             2
#define IDC_BITMAP                             3

/*--------------------------------------------------------------*\
 *  Menu item ids                                               *
\*--------------------------------------------------------------*/
#define IDM_VMMALLOC                        1011
#define IDM_VMMFREE                         1012
#define IDM_VMMSET                          1013
#define IDM_VMMWRITE                        1014
#define IDM_VMMREAD                         1015
#define IDM_MSGBOX                          1016

#define IDM_HELPINDEX                       1020
#define IDM_GENERALHELP                     1021
#define IDM_TUTORIAL                        1022
#define IDM_USINGHELP                       1023
#define IDM_HELPPRODUCTINFO                 1024

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define VMM_HELP_TABLE                      4000

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

#define PANEL_HELPALLOC                     4190
#define PANEL_HELPFREE                      4191
#define PANEL_HELPSET                       4192
#define PANEL_HELPWRITE                     4193
#define PANEL_HELPREAD                      4194

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_PRODUCTINFODLG             4200
#define PANEL_PRODUCTINFODLG                4210
#define PANEL_PRODUCTINFO_OK                4220

#define SUBTABLE_ALLOCBOXDLG                4230
#define PANEL_ALLOCBOXDLG                   4231
#define PANEL_ALLOCBOX_OK                   4232

#define SUBTABLE_SETBOXDLG                  4235
#define PANEL_SETBOXDLG                     4236
#define PANEL_SETBOX_OK                     4237

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
         /* for help menu module */
#define IDS_APPNAME                          101
#define IDS_HELPLIBRARYNAME                  102
#define IDS_HELPWINDOWTITLE                  103
#define IDS_TITLE                            104
#define IDS_UNTITLED                         105

/*--------------------------------------------------------------*\
 *  Message item ids
\*--------------------------------------------------------------*/
#define IDD_PRODUCTINFO                      152
#define IDD_EDITTEXT                         153
#define IDD_EDITSIZE                         154
#define IDD_EDITUNIT                         155
#define IDD_ACCESS                           156
#define IDD_READ                             157
#define IDD_WRITE                            158
#define IDD_EXECUTE                          159
#define IDD_GUARD                            160
#define IDD_ATTRIBUTES                       161
#define IDD_TILE                             162
#define IDD_COMMIT                           163
#define IDD_DECOMMIT                         164
#define IDD_DEFAULT                          165

#define IDD_ALLOCMEM                         166
#define IDD_SETMEM1                          167
#define IDD_SETMEM2                          168
#define IDD_FREEMEM                          169
#define IDD_WRITEMEM                         170
#define IDD_READMEM                          171
#define IDD_ADDRESS                          172
#define IDD_ADDRESSTEXT                      173
#define IDD_DATA                             174
#define IDD_TITLE                            175

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
#define IDMSG_ACCESSGUARDPAGE                308
#define IDMSG_COMMITTTOWRITE                 309
#define IDMSG_PROTECTIONERROR                310
#define IDMSG_SUCCESSFREE                    311
#define IDMSG_UNSETREADERROR                 312
#define IDMSG_UNSETREADERROR2                313
#define IDMSG_ERRORADDRESS                   314

/*--------------------------------------------------------------*\
 *  Error Constants                                             *
\*--------------------------------------------------------------*/
#define VMERR_MAX_ALLOCATED             0xFF10
#define VMERR_ZERO_ALLOCATED            0xFF11
#define VMERR_TILE_ONLY                 0xFF12
#define VMERR_COMMIT_AND_TILE_ONLY      0xFF13
#define VMERR_ALREADY_FREED             0xFF20
#define VMERR_NOT_BASEPAGE              0xFF21
#define VMERR_DECOMMIT_RESERVED         0xFF30
#define VMERR_ACCESS_AND_DECOMMIT       0xFF31
#define VMERR_ACCESS_ON_RESERVED        0xFF32
#define VMERR_COMMIT_ON_COMMITTED       0xFF33
#define VMERR_COMMIT_ONLY               0xFF34
#define VMERR_SET_NO_PARMS              0xFF35
#define VMERR_SET_ZERO_SIZE             0xFF36
#define VMERR_SET_ON_FREE               0xFF37
#define VMERR_DEFAULT                   0xFFFF

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define MAXTEXTLEN               40    /* maximum text length for window */
#define HELPLIBRARYNAMELEN       20    /* The length of library name     */
#define MESSAGELEN              150       /* maximum length for messages */
#define MAXAPPNAMELEN            15
#define MAXPAGES                 10
#define PAGESIZE               4096
#define BUFF_SIZE               200 /* size of buffers for various stuff */
#define MAXHOSIZE               256   /* siae of buffers for dlgbox text */
       /* size and positioning of boxes in 0.1 millimeters (PU_LOMETRIC) */
#define BOX_WIDTH               450
#define BOX_HEIGHT              180
#define SMALL_BOX_HEIGHT         90
#define VERT_INDENT              30
#define HORZ_INDENT              30
#define HORZ_SPACING             50
#define VERT_SPACING             50

#define RETURN_SUCCESS            0      /* successful return in DosExit */
#define RETURN_ERROR              1           /* error return in DosExit */
#define BEEP_WARN_FREQ           60         /* frequency of warning beep */
#define BEEP_WARN_DUR           100          /* duration of warning beep */
#define MAX_EDIT_BUFF            21    /* length of string to query size */

#define SZDEFAULT_SIZE          "1"       /* default number of bytes to set
                     attributes for when setting attributes of a block, this
                        will cause the attributes to be set for one page */

/*--------------------------------------------------------------*\
 *  Data structures and typedefs                                *
\*--------------------------------------------------------------*/
struct _WINDOWDATA  {
    LONG clrFore;
    LONG clrBack;
    UCHAR *szText[MAXTEXTLEN + 1];
};

typedef struct _OBJSTRUCT{
    ULONG ulSize;
    ULONG ulAttr;
    PVOID pvAddress;
    } OBJSTRUCT;

typedef OBJSTRUCT FAR * POBJSTRUCT;

typedef struct _WINDOWDATA WINDOWDATA;
/* hungarian notation: wdata */
typedef WINDOWDATA * PWINDOWDATA;

typedef struct _PAGEENTRY {
    PVOID   pvAddress;
    BOOL    fBaseAddr;
    } PAGEENTRY;
/* hungarian notation: pgentry */

typedef struct _MSGENTRY {
    ULONG   ulMsgNum;
    CHAR    szMsgText[BUFF_SIZE];
    USHORT  usMsgIcon;
    } MSGENTRY;
/* hungarian notation: msgentry */

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
CHAR  szWindowText[MAXTEXTLEN];                  /* text drawn in window */
HWND  hwndMainFrame;                  /* handle to the main frame window */
HWND  hwndMain;                      /* handle to the main client window */
HWND  hwndVscroll;                  /* handle to the vertical scroll bar */
HAB   hab;                               /* anchor block for the process */
HMQ   hmq;                       /* handle to the process' message queue */
CHAR  szAppName[MAXAPPNAMELEN];    /* buffer for application name string */
ULONG       ulFreePage;        /* First free page entry in array; same as
                                  the number of data pages existed       */
CHAR        szBuffer[BUFF_SIZE];
OBJSTRUCT   ObjAlloc;
CHAR        szUntitled[MESSAGELEN];      /* buffer for "Untitled" string */
      /* for help menu module */
BOOL   fHelpEnabled;             /* flag to determine if help is enabled */
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance;
static LONG sVscrollPos;
PAGEENTRY   apgentry[MAXPAGES];                /* Application page table */

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/
/* from vmm_main.c */
INT     main(VOID);
ULONG   GPFHandler(PEXCEPTIONREPORTRECORD);
VOID    MessageBox(HWND, LONG, PSZ, LONG, BOOL);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
VOID    MainCommand(MPARAM, MPARAM);
VOID    MainPaint(HWND);
VOID    DisplayPage(HPS, PPOINTL, ULONG);
VOID    EnableMenuItem(HWND, LONG, BOOL);

/* from vmm_init.c */
BOOL Init(VOID);
VOID APIENTRY ExitProc(ULONG);

/* from vmm_user.c */
VOID UserCommand(MPARAM, MPARAM);
VOID VMM_Error(PSZ, ULONG);
VOID CleanUpArray(PVOID);
VOID StoreInArray(PVOID, ULONG);

      /* for help menu module */
VOID  InitHelp(VOID);
VOID  HelpHelpForHelp(MPARAM);
VOID  HelpExtended(MPARAM);
VOID  HelpKeys(MPARAM);
VOID  HelpIndex(MPARAM);
VOID  HelpAbout(MPARAM);
VOID  DisplayHelpPanel(LONG);
VOID  DestroyHelpInstance(VOID);
VOID  FixSysMenu(HWND);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);

/* from vmm_dlg.c */
MRESULT EXPENTRY AllocMemDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY SetMem1DlgProc( HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY SetMem2DlgProc( HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ReadMemDlgProc( HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY WriteMemDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY FreeMemDlgProc( HWND, ULONG, MPARAM, MPARAM);

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif

#define     VER630

/*static char *SCCSID = "@(#)eas.h	6.12 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  EAS.H - Sample PM application eas header file               *
 *      (C) Copyright IBM Corporation 1992.                     *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide constants    *
 *  and structure definitions.                                  *
 *                                                              *
\*==============================================================*/
#define INCL_WIN
#define INCL_HELP
#define INCL_WINHEAP
#define INCL_WINMENU               /* For menu control function */
#define INCL_WINDIALOGS
#define INCL_WINMESSAGEMGR
#define INCL_DOSPROCESS                    /* For init function */
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS

#include <os2.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define ID_RESOURCE                            1
#define IDR_BITMAP                             2
#define ID_BMP                                 3
#define ID_LOGOTEXT                            4
#define IDC_BITMAP                             5

/*--------------------------------------------------------------*\
 *  Menu item ids                                               *
\*--------------------------------------------------------------*/
#define IDM_HELPINDEX                       1010
#define IDM_GENERALHELP                     1011
#define IDM_TUTORIAL                        1012
#define IDM_USINGHELP                       1013
#define IDM_HELPPRODUCTINFO                 1014
#define IDM_MSGBOX                          1015

#define IDM_OPEN                            1016
#define IDM_SAVE                            1017
#define IDM_EDIT                            1018
#define IDM_ADD                             1019
#define IDM_DELETE                          1020

/*--------------------------------------------------------------*\
 *  Message item ids                                            *
\*--------------------------------------------------------------*/
#define IDD_ABOUTBOX                         151
#define IDD_MULTILIST                        152
#define IDD_ASCIIZ                           153
#define IDD_MENU_MAIN                        154
#define IDD_OPENBOX                          155
#define IDD_PATH                             156
#define IDD_FILEEDIT                         157
#define IDD_DIRLIST                          158
#define IDD_FILELIST                         159
#define IDD_WINLBOX                          160
#define IDD_LBOX                             161
#define IDD_ADD                              162
#define IDD_DELETE                           163
#define IDD_EDIT                             164
#define IDD_EATYPE                           165
#define IDD_ADDEA                            166
#define IDD_EANAME                           167
#define IDD_NEW                              168
#define IDD_EXISTING                         169
#define IDD_ASCIIEDIT                        170
#define IDD_EAVALUE                          171
#define IDD_LOGO                             172
#define IDD_ICON                             173
#define IDD_MULTIBOX                         174
#define DID_DONE                             175
#define IDD_TITLE                            176
#define IDD_LPDATA                           177
#define IDD_NEEDBIT                          178
#define IDD_CODEPAGE                         179
#define IDD_PRODUCTINFO                      180

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define EAS_HELP_TABLE                      4000

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

#define PANEL_OPENHELP                      4190
#define PANEL_SAVEHELP                      4191
#define PANEL_ADDHELP                       4193
#define PANEL_DELETEHELP                    4194
#define PANEL_EDITHELP                      4195

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_ABOUTBOXDLG                4200
#define PANEL_ABOUTBOXDLG                   4210
#define PANEL_ABOUTBOX_OK                   4220

#define SUBTABLE_OPENBOXDLG                 4225
#define PANEL_OPENBOXDLG                    4226
#define PANEL_OPENBOX_OK                    4227

#define SUBTABLE_ADDBOXDLG                  4230
#define PANEL_ADDBOXDLG                     4231
#define PANEL_ADDBOX_OK                     4232

#define SUBTABLE_MULTIBOXDLG                4235
#define PANEL_MULTIBOXDLG                   4236
#define PANEL_MULTIBOX_OK                   4237

#define SUBTABLE_PRODUCTINFODLG             4240
#define PANEL_PRODUCTINFODLG                4241
#define PANEL_PRODUCTINFO_OK                4242

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
#define IDS_APPNAME                          101
#define IDS_HELPLIBRARYNAME                  102
#define IDS_HELPWINDOWTITLE                  103
#define IDS_TITLE                            104
#define IDS_UNTITLED                         105
/*--------------------------------------------------------------*\
 *  Messagetable ids                                            *
\*--------------------------------------------------------------*/
#define IDMSG_FILECHANGED                    301
#define IDMSG_ERROREANAME                    302
#define IDMSG_DUPLICATENAME                  303
#define IDMSG_ERRORVALUE                     304
#define IDMSG_MEMORYFULL                     305
#define IDMSG_ERRORSELECT                    306
#define IDMSG_NOFILEOPEN                     307
#define IDMSG_ERROREATYPE                    308
#define IDMSG_WRITEERROR                     309
#define IDMSG_WRITEOK                        310
#define IDMSG_ERRORMULTIVALUE                311
#define IDMSG_INITFAILED                     312
#define IDMSG_MAINWINCREATEFAILED            313
#define IDMSG_CANNOTGETHPS                   314
#define IDMSG_CANNOTLOADSTRING               315
#define IDMSG_CANNOTLOADEXITLIST             316
#define IDMSG_HELPLOADERROR                  317
#define IDMSG_HELPDISPLAYERROR               318
#define IDMSG_ERRORMULTITYPE                 319
#define IDMSG_ERRORFILEOPEN                  320

/*--------------------------------------------------------------*\
 *  Macro constants                                             *
\*--------------------------------------------------------------*/
/* The HoldFEA is used to hold individual EAs.  The member names correspond
   directly to those of the FEA structure.  Note however, that both szName
   and aValue are pointers to the values.  An additional field, next, is
   used to link the HoldFEA's together to form a linked list. */

struct _HoldFEA
{
   ULONG   oNextEntryOffset;                    /* new field */
   BYTE       fEA;       /* Flag byte */
   BYTE       cbName;
   USHORT     cbValue;
   CHAR      *szName;
   CHAR      *aValue;
   struct _HoldFEA *next;
};
typedef struct _HoldFEA HOLDFEA;

/* The DeleteList is used to hold the names of EAs that need to be explicitly
   deleted before the current EAs are written.  This is necessary because
   there does not exist a way to automatically delete all existing EAs
   associated with a file.  The next field allows the structures to be
   linked. */

struct _DeleteList
{
   CHAR *EAName;
   struct _DeleteList *next;
};
typedef struct _DeleteList DELETELIST;

/* The PassData struct is used to pass data, especially as the user data
   parameter for dialog box calls.  The Point field points to several types
   of data throughout the program.  Usually it points to either an asciiz
   name or a HoldData structure.  The rest of the fields are used for m-m
   recursive calls.  cbMulti is the length of the m-m field currently being
   considered.  usMultiOffset is the offset from the beginning of the
   EA (->aValue) to the start of the current m-m field.  usIndex is a count
   of the number of sub-fields in the current m-m.   */

struct _PassData
{
   CHAR   *Point;
   USHORT cbMulti;
   USHORT usMultiOffset;
   USHORT usIndex;
   BYTE   fFlag;
};
typedef struct _PassData PASSDATA;
typedef PASSDATA FAR *PPASSDATA;

struct _MVSTdata
{
   CHAR   *szName;
   USHORT usType;
};
typedef struct _MVSTdata MVSTDATA;

/* The ReEnter structure is used to keep track of the static data for
   MultiTypeProc.  This structure is necessary since the proc is recursive
   and the static data is only available during the initialize message.
   The structure holds the two static variables and has a next field to
   allow the list to be linked. */

struct _ReEnter
{
   HOLDFEA *pFEA;
   PASSDATA FAR *pPDat;
   struct _ReEnter *next;
};
typedef struct _ReEnter REENTER;

/* This struct holds the static data that allows translation between the
   EA type and descriptor string, etc.  Data is global. */

struct _EADATA
{
   USHORT usPrefix;
   CHAR   szFormat[36];
   USHORT usFldType;
};
typedef struct _EADATA EADATA;

#define FreeMem(p)      DosFreeMem(p)

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/
/* from easinit.c */
BOOL    Init(VOID);
VOID APIENTRY ExitProc(ULONG);

/* from eashelp.c */
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

/* from eas.c */
INT     main(int argc, char *argv[]);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
VOID    MainCommand(HWND, MPARAM, MPARAM);
VOID    MainPaint(HWND);
VOID    MainControl(HWND, MPARAM, MPARAM);
VOID    UserCommand(HWND, MPARAM, MPARAM);
VOID    ProcExit(HWND, MPARAM);
LONG    MessageBox(HWND, LONG, PSZ, LONG, BOOL);

BOOL OpenFile(HWND,ULONG);
BOOL AddEA(HWND);
BOOL QueryEAs(CHAR *);
BOOL EditEAValue(HWND, PPASSDATA);
BOOL EAExists(CHAR *);
BOOL EditEA(HWND);
BOOL CheckEAIntegrity(CHAR *, ULONG);
MRESULT EXPENTRY OpenFileProc  (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY MainDlgProc   (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AddEAProc     (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AsciiEditProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY IconDlgProc   (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY MultiTypeProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY MultiValueProc(HWND, ULONG, MPARAM, MPARAM);
VOID    FillDirListBox  (HWND,CHAR *);
VOID    FillFileListBox (HWND);
VOID    Free_FEAList(HOLDFEA *,DELETELIST *);
VOID    ShowEAType(HWND);
VOID    DeleteCurEA(HWND);
VOID    WriteEAs(VOID);
VOID    ChangeName(HOLDFEA *,CHAR *);
VOID    MultiAdd(HWND, HOLDFEA *, PPASSDATA);
LONG    ParseFileName (CHAR *, CHAR *);
ULONG   LookupEAType(ULONG);
ULONG   CurEAType(HOLDFEA *);
USHORT   GetUSHORT(HOLDFEA *,ULONG);
HOLDFEA *GetCurFEA(HWND, HOLDFEA *);
CHAR    *MultiTypeIndex(CHAR *, ULONG);
CHAR    *EAValueString(CHAR *);
VOID    GetMem(PVOID *pvMessage, ULONG ulSize);
VOID    ResizeMem(PVOID *, ULONG, ULONG);

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define RETURN_SUCCESS        0          /* successful return in DosExit    */
#define RETURN_ERROR          1          /* error return in DosExit         */
#define BEEP_WARN_FREQ       60          /* frequency of warning beep       */
#define BEEP_WARN_DUR       100          /* duration of warning beep        */
#define MAXAPPNAMELEN        15
#define HELPLIBRARYNAMELEN   20          /* The length of library name      */
#define MAX_EDIT_BUFF        21          /* length of string to query size  */
#define BUFF_SIZE           200

#define MAXTEXTLEN           40          /* maximum text length for window  */
#define MESSAGELEN           80          /* maximum length for messages     */
#define HORZ_INDENT          10
#define VERT_TEXT_POS        15          /* from top to display text info   */

#define ARGFILE               1
#define MAX_GEA            500L  /* Max size for a GEA List                 */
#define Ref_ASCIIZ            1  /* Reference type for DosEnumAttribute     */
#define MAXEANAME           255  /* Maximum length an EA Name can be        */
#define MAXEAVALUE         2048  /* Arbitrary max edit fld len of EA Val    */
#define EATABLESIZE          12  /* Number of entries in ConvTable          */
#define GROWSIZE            512  /* Minimum size to increase the heap by    */
#define FILE_ALL         0x0007  /* Read-only, sys, hidden, & normal        */
#define MAXSHOWSIZE          34  /* Number of chars to place in desc lbox   */
#define LENUSHORTBUF          6  /* Buf size needed to for ascii USHORT     */
#define MESSAGELEN           80  /* Maximum length for messages             */
#define MSNUM                 5  /* Number of MVST data list                */

/* Return values from ParseFileName */
#define FILE_INVALID          0  /* The filename was invalid                */
#define FILE_PATH             1  /* The filename was a path                 */
#define FILE_VALID            2  /* The filename was valid                  */

/* definition of level specifiers. required for File Info */

#define GetInfoLevel1         1            /* Get info from SFT             */
#define GetInfoLevel2         2            /* Get size of FEAlist           */
#define GetInfoLevel3         3            /* Get FEAlist given the GEAlist */
#define GetInfoLevel4         4            /* Get whole FEAlist             */
#define GetInfoLevel5         5            /* Get FSDname                   */

#define SetInfoLevel1         1            /* Set info in SFT               */
#define SetInfoLevel2         2            /* Set FEAlist                   */

#define     OPENMODE    OM_DENY_NONE+OM_READ_WRITE
#define     OPENFLAG    OF_OPEN_FILE
#define     CREATFLAG   OF_CREATE_FILE+OF_FAIL
#define     REPLFLG     OF_REPLACE_FILE

#define EA_LPBINARY      0xfffe       /* Length preceeded binary            */
#define EA_LPASCII       0xfffd       /* Length preceeded ascii             */
#define EA_ASCIIZ        0xfffc       /* Asciiz                             */
#define EA_LPBITMAP      0xfffb       /* Length preceeded bitmap            */
#define EA_LPMETAFILE    0xfffa       /* metafile                           */
#define EA_LPICON        0xfff9       /* Length preceeded icon              */
#define EA_ASCIIZFN      0xffef       /* Asciiz file name of associated dat */
#define EA_ASCIIZEA      0xffee       /* Asciiz EA of associated data       */
#define EA_MVMT          0xffdf       /* Multi-value multi-typed field      */
#define EA_MVST          0xffde       /* Multy value single type field      */
#define EA_ASN1          0xffdd       /* ASN.1 field                        */

#define HM_VALIDFLAGS    0x0003

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
CHAR  szWindowText[MAXTEXTLEN];                     /* text drawn in window */
HWND  hwndMainFrame;                     /* handle to the main frame window */
HWND  hwndMain;                         /* handle to the main client window */
HWND  hwndChild1;                                 /* handle to child window */
HAB   hab;                                  /* anchor block for the process */
HMQ   hmq;                          /* handle to the process' message queue */
CHAR  szAppName[MAXAPPNAMELEN];       /* buffer for application name string */
CHAR  szUntitled[MESSAGELEN];               /* buffer for "Untitled" string */
      /* for help menu module */
BOOL   fHelpEnabled;                /* flag to determine if help is enabled */
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance;

CHAR    szFileName[CCHMAXPATH];                    /* Holds current EA file */
CHAR    szEAName[MAXEANAME+1];    /* Used to return the EA name +1 for NULL */
CHAR    szEAType[36];                              /* Store current EA type */
USHORT  usRetEAType;                     /* Used to return selected EA Type */
BOOL    FILE_ISOPEN;                                      /* File Open flag */
BOOL    FILE_CHANGED;                                  /* File Changed flag */
BOOL    COMMAND_LINE_FILE;     /* Flag to determine if a file was on the CL */
PVOID   szAscii,szScratch;      /* 2 Pointer used to return misc. strings */
CHAR    *pAlloc,*szEditBuf;
HOLDFEA *pHoldFEA;                         /* Global EA linked-list pointer */
DELETELIST *pDelList;                   /* Global ptr to l-l of deleted EAs */
HAB     hab;                                /* Anchor block for the process */
HMQ     hmq;
MVSTDATA MSdata[MSNUM];                         /* MVST name & type stored */

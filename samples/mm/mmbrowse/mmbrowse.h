#include "mmioos2.h"
  #ifndef MMIOM_QUERYIMAGE
   #define MMIOM_QUERYIMAGE         MMIOM_START + 39
   #define MMIOM_QUERYIMAGECOUNT    MMIOM_START + 40
   #define MMIOM_SETIMAGE           MMIOM_START + 41
#endif
#define ERR_CANCELLED                 1
#define ERR_SOURCEOPEN                2
#define ERR_TARGETOPEN                3
#define ERR_SOURCEHEADER              4
#define ERR_TARGETHEADER              5
#define ERR_SOURCEWRITE               6
#define ERR_TARGETWRITE               7
#define ERR_SOURCECLOSE               8
#define ERR_TARGETCLOSE               9
#define ERR_MEMORY                   10
#define HELP_GET_FOURCC              3000
#define ID_HELP_REMOVE_PERM_LIST_BOX 3001
#define MAX_FILENAME_SIZE         256
#define STRING_LENGTH             80
#define ID_WINDOW                 256
#define ID_POINTER                257
#define ID_FILE                   260
#define ID_FILE_OPEN              261
#define ID_FILE_SAVEAS            262
#define ID_FILE_PRINT             263
#define ID_FILE_ABOUT             264
#define ID_FILE_EDIT              265
#define ID_CONFIG                 270
#define ID_CONFIG_INST_TEMP       271
#define ID_CONFIG_INST_PERM       272
#define ID_CONFIG_REM_TEMP        273
#define ID_CONFIG_REM_PERM        274
#define ID_CONFIG_QUERY_ALL       275
#define ID_HELP                   280
#define ID_HELP_FOR_HELP          281
#define ID_HELP_KEYS              282
#define ID_HELP_INDEX             283
#define ID_HELP_PRODUCTINFO       284
#define ID_EXITPROG               289
#define IDS_MMBROWSER             290
#define IDS_OPEN                  291
#define IDS_PRINT                 292
#define IDS_INSTALL               293
#define IDS_SAVEAS                294
#define IDS_REMOVE                295
#define IDS_QUERY                 296
#define IDS_HELP_INDEX            297
#define IDS_HELP_GENERAL          298
#define IDS_HELP_USING            299
#define IDS_HELP_KEYS             300
#define IDS_HELP_INFO             301
#define IDS_OPEN_OK_BUTTON        400
#define IDS_OPEN_DIALOG_TITLE     401
#define IDS_OPEN_DIALOG_FILE_EXT  402
#define IDS_SAVEAS_BUTTON         403
#define IDS_SAVEAS_DIALOG_TITLE   404
#define IDS_SAVEAS_DIALOG_FILE_EXT 405
#define IDS_INSTALL_TITLE         500
#define IDS_INSTALL_FILE_EXT      501
#define IDS_INSTALL_OK_BUTTON     502
#define IDS_INSTALL_EXIT          503
#define IDS_INSTALL_EXIT_BUTTON   504
#define IDS_INSTALL_NOTE          505
#define IDS_REMOVE_TITLE          600
#define IDS_REMOVE_FILE_EXT       601
#define IDS_REMOVE_OK_BUTTON      602
#define IDS_IOPROC_NAME           700
#define IDS_INSTALL_IOPROC_FAILED 701
#define GET_FOURCC                800
#define GET_FOURCC_INSTRUCTIONS   801
#define GET_FOURCC_TEXT           802
#define GET_FOURCC_OK_BUTTON      803
#define GET_FOURCC_CANCEL_BUTTON  804
#define GET_FOURCC_HELP_BUTTON    805
#define QUERY_ALL_LIST_BOX                  1000
#define QUERY_ALL_LIST_BOX_ENTRIES          1001
#define QUERY_ALL_LIST_BOX_OK               1002
#define QUERY_ALL_LIST_BOX_CANCEL           1003
#define QUERY_ALL_LIST_BOX_HELP             1004
#define REMOVE_PERM_LIST_BOX            1100
#define REMOVE_PERM_LIST_BOX_ENTRIES    1101
#define REMOVE_PERM_LIST_BOX_OK         1102
#define REMOVE_PERM_LIST_BOX_CANCEL     1103
#define REMOVE_PERM_LIST_BOX_HELP       1104
#define ID_RESOURCE    1
#define IDR_BITMAP     2
#define IDC_BITMAP     3
#define IDM_START  (WM_USER+0)
#define IDM_STOP   (WM_USER+1)
#define IDM_SET    (WM_USER+2)
#define IDM_DONE   (WM_USER+3) /* Message posted when thread terminates */
#define IDM_ON     (WM_USER+4)
#define IDM_OFF    (WM_USER+5)
#define IDD_SETCOUNT                           1
#define IDD_ENTRYFLD                           2
#define IDD_MSGBOX                             3
#define IDD_PRODUCTINFO                        4
#define IDD_HELP                               5
#define IDM_HELPUSINGHELP                   9100
#define IDM_HELP                            9300
#define IDM_HELPGENERAL                     9200
#define IDM_HELPINDEX                       9400
#define IDM_HELPPRODUCTINFO                 9600
/*
 *  Help table and subtables
 */
#define MMBROWSER_HELP_TABLE                    1000
/*
 *  Main window help panels
 */
#define SUBTABLE_MAIN                       2000
#define PANEL_MAIN                          2100
#define PANEL_HELP                          2910
#define PANEL_HELPUSINGHELP                 2920
#define PANEL_HELPGENERAL                   2930
#define PANEL_HELPINDEX                     2950
#define PANEL_HELPPRODUCTINFO               2960
/*
 *  Enter text dialog help subtable
 */
#define SUBTABLE_PRODUCTINFODLG             3900
#define PANEL_PRODUCTINFODLG                3910
#define PANEL_PRODUCTINFO_OK                3920
/*
 * Function prototypes
 */

HBITMAP GetBitMap(HWND, PSZ);
MRESULT EXPENTRY MyWindowProc ( HWND, USHORT, MPARAM, MPARAM );
MRESULT EXPENTRY GetFOURCCDialogProc ( HWND, ULONG, MPARAM, MPARAM );
VOID FixSysMenu(HWND);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY QueryAllDialogProc ( HWND, USHORT, MPARAM, MPARAM );
MRESULT EXPENTRY RemovePermDialogProc ( HWND, USHORT, MPARAM, MPARAM );
ULONG DisplayImageFile ( HWND, PSZ );
VOID DisplayMessageBox ( HWND, USHORT, USHORT, USHORT );
BOOL OpenFileDialog ( HWND, PSZ, PSZ, PSZ, PSZ );
VOID FourccToString( FOURCC, PSZ );
VOID MakeItBusy ( BOOL );
VOID main ( int argc, char *argv[], char *envp[] );
VOID DrawBitMap ( HWND );
VOID CenterDialog ( HWND );
VOID FixSysMenu (HWND);
BOOL InstallTempIOProc ( HWND );
HBITMAP LoadBitmap (HAB, HDC, HPS *, PSZ );
BOOL DoConvert(PSZ, FOURCC, PSZ, FOURCC );
BOOL SaveAsDialog ( HWND, PSZ, PSZ, PSZ, PSZ );

/*
 * Global variables.
 */
BOOL      Cancel = FALSE;
BOOL      FileOnCommandLine = FALSE;
CHAR      szCommandLineFilename[ 256 ];
HAB       hab;
HBITMAP   hBmp;
HPS       *hps;
HPS       hpsMemory;
HDC       hdcMemory;
BOOL      bBusy = FALSE;
BOOL      bSuspendIdentify = FALSE;
BOOL      bHelpEnabled=FALSE;
CHAR      szFOURCC[ STRING_LENGTH ];
PCHAR     pszFileName;
PCHAR     pszTarget;
CHAR      szSource[MAX_FILENAME_SIZE];
CHAR      szInstallFileExt[ STRING_LENGTH ];
CHAR      szInstallTitle[ STRING_LENGTH ];
CHAR      szInstallOKButton[ STRING_LENGTH ];
CHAR      szRemoveFileExt[ STRING_LENGTH ];
CHAR      szRemoveTitle[ STRING_LENGTH ];
CHAR      szRemoveOKButton[ STRING_LENGTH ];
CHAR      szString[STRING_LENGTH];
CHAR      szInstall[ STRING_LENGTH ];
CHAR      szOpenDialogTitle[ STRING_LENGTH ];
CHAR      szOpenDialogFileExt[ STRING_LENGTH ];
CHAR      szOpenOKButton[ STRING_LENGTH ];
CHAR      szIOProcName[STRING_LENGTH];
CHAR      szSaveAsDialogTitle[ STRING_LENGTH ];
CHAR      szSaveAsDialogFileExt[ STRING_LENGTH ];
CHAR      szSaveAsButton[ STRING_LENGTH ];
HPOINTER  hPointer;
HELPINIT  hmiHelpStructure;                 /* Help init. structure. */
HWND      hwndHelpInstance;                  /* Handle to Help window.            */
BOOL      fHelpEnabled;

VOID SaveFile (HWND hwndClient, PSZ pszSource, FOURCC fccSourceIOProc );
VOID InitializeHelp( HWND hwndFrame) ;

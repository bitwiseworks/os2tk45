/************ GRAPHIC C Sample Program Header File (.H) ******************
 *  Copyright (C) 1991, 1993 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 ******************************************************************************/

/*  Macros */

#define DRIVERNAME_LENGTH    128
#define DESCRIPTION_LENGTH   64

#define LOGOTIME_APPLNAME    "PM_ControlPanel"
#define LOGOTIME_KEYNAME     "LogoDisplayTime"

#define STACK_SIZE           8192

#define MSGBOXID             222      /* message box id */

#define HELP_FILE            "GRAPHIC.HLP"

#define MAXSTRLEN            200

#define WM_USER_DEFINED_CODE (WM_USER + 1)
#define WM_USER_END_ON_ERROR (WM_USER + 2)

#define STOP_THREAD          (WM_USER + 3)
#define SET_TRANSFORM        (WM_USER + 4)
#define DRAW_ORDERS          (WM_USER + 5)
#define SIZING               (WM_USER + 6)
#define DUMMY_COMMAND        (WM_USER + 7)
#define FLUSH_COMMAND        (WM_USER + 8)
#define STOP_DRAWING         (WM_USER + 9)
#define SET_PRINT_STATUS     (WM_USER + 10)
#define DRAW_GPI             (WM_USER + 12)
#define DRAW_META            (WM_USER + 13)
#define ENABLE_INFO_WND      (WM_USER + 14)
#define POS_INFO_WND         (WM_USER + 15)
#define ACTIVATE_DRAW_THREAD (WM_USER + 16)
#define DISPLAY_MESSAGE      (WM_USER + 17)

#define DEVINFOSIZE 39

#define DRAW_OK       GPI_OK
#define DRAW_STOPPED  2

#define CHECK_ITEM     0x0001
#define INIT_MLE       0x0003

#define ALLOC_STRING    1
#define STATIC_STRING   0

/* Structure-Definitions */

typedef struct _PRTTXT_DATA {
    PSZ       *Text;
    CHAR       QueueName[12];
    CHAR       DriverName[DRIVERNAME_LENGTH];
    PVOID      DriverData;         /* Driver-Data for DevOpenDC     */
    HWND       hWndFrame;
    CHAR       Jobname[8];
} PRTTXT_DATA;
typedef PRTTXT_DATA *PPRTTXT_DATA;

/* Structure for saving defaults */
typedef struct _PPL_DEFAULTS {
    ULONG      cy;
    ULONG      cx;
    CHAR       DriverName[DRIVERNAME_LENGTH];
} APPL_DEFAULTS;
typedef APPL_DEFAULTS *PAPPL_DEFAULTS;

typedef struct _RESOLUTION {
    LONG       x;
    LONG       y;
} RESOLUTION;
typedef  RESOLUTION *PRESOLUTION;

/* Draw Parameter */
typedef struct _DRAW_PARM {
    RECTL      rclBounds;         /* Bounding rectangle of the picture */
    BOOL       fAllValid;         /* hrgnInvalid is empty          */
    BOOL       fDrawing;          /* Asynchronous thread can draw  */
    HRGN       hrgnInvalid;       /* Region used for accumulating  */
                                  /* invalid regions in the        */
                                  /* client area                   */
    HMF        hmfPicture;
    HAB        hABDraw;
} DRAW_PARM;
typedef DRAW_PARM *PDRAW_PARM;

/* Info window structure */
typedef struct _INFO_STRUC {
    HWND       hWnd;
    BOOL       EnableWindow;
    BOOL       Enabled;
    LONG       SizeX;
    LONG       SizeY;
    LONG       DrawLine;
} INFO_STRUC;

/* Main Parameter-Structure */
typedef  struct _MAIN_PARM {
    HAB        hAB;
    PSZ        ProgramTitle;
    HWND       hWndFrame;
    HWND       hWndGRAPHICHelp;   /* Handle to Help window         */
    HWND       hWndClient;
    HMQ        hMQPrint;
    HMQ        hMQ;
    HMQ        hMQDraw;
    HPS        hpsPrint;
    HPS        hpsPaint;          /* Presentation space for use    */
                                  /* on the main thread            */
    HPS        hpsClient;         /* Client-area graphics          */
    TID        DrawThreadID;
    TID        PrintThreadID;
    HEV        hevsemPrintEnds;
    HEV        hevsemTerminate;
    HEV        hevsemStop;
    HDC        hdcClient;         /* Window device-context handle  */
    HDC        hdcPrinter;
    HMTX       hmtxsemStoppable;  /* Mutex semaphore "Stoppable"   */
    HPOINTER   hptrGlass;
    HPOINTER   hptrArrow;
    PTIB       ptibMain;          /* Main thread and process IDs   */
    PPIB       ppibMain;          /* Main thread and process IDs   */
    CHAR       ErrorMsg[200];
    CHAR       QueueName[12];
    CHAR       DriverName[DRIVERNAME_LENGTH];
    CHAR       MetafilePath[200];
    CHAR       Message[200];
    PVOID      DriverData;         /* Driver-Data for DevOpenDC     */
    LONG       CheckedItem;
    ULONG      DriverDataLength;   /* Length of DriverData          */
    BOOL       PrintStatus;       /* flag set while printing       */
    BOOL       PrintEnabled;
    BOOL       PrintPropertiesEnabled;
    BOOL       fNotReady;         /* Let the system handle         */
                                  /*    client-area painting       */
    BOOL       fStoppable;        /* Asynchronous thread is        */
                                  /*    stoppable                  */
    BOOL       DrawThreadActivated;
    BOOL       ResizeWindow;
    BOOL       fWaiting;          /* The main thread cannot        */
                                  /*    issue GpiSetStopDraw       */
    BOOL       fBusy;
    DRAW_PARM  DrawParm;
    INFO_STRUC InfoWnd;
} MAIN_PARM;
typedef MAIN_PARM *PMAIN_PARM;

/* Structure for Queue-Selection */
typedef struct _QUEUE_SEL {
    CHAR       QueueName[12];
    CHAR       DriverName[DRIVERNAME_LENGTH];
    CHAR       QueueDescription[DESCRIPTION_LENGTH];
    PVOID      DriverData;           /* Pointer to Driver Data ptr */
    ULONG      DriverDataLength;     /* Length of DriverData       */
} QUEUE_SEL;
typedef QUEUE_SEL *PQUEUE_SEL;

/* Parameter-Structure for Printer Selection Dialog */
typedef struct _PRINTER_SEL_PARM {
    ULONG      ulLen;                /* length of structure        */
    ULONG      ulType;               /* reserved, must be NULL     */
    PQUEUE_SEL QueueSelTable;        /* Queue Selection Table      */
    ULONG      QueuesCount;          /* Entries in Table           */
    LONG       SelectedItem;         /* Selected item, -1 = none   */
    LONG       SaveSelectedItem;     /* Selected item, -1 = none   */
    HAB        hAB;                  /* handle anchor block        */
} PRINTER_SEL_PARM;
typedef PRINTER_SEL_PARM *PPRINTER_SEL_PARM;

/*  Function Prototypes */
MRESULT  EXPENTRY   WndProc(HWND hWnd,
                            ULONG message,
                            MPARAM mp1,
                            MPARAM mp2);

MRESULT  EXPENTRY   WndProcI(HWND hWndI,
                             ULONG Msg,
                             MPARAM mp1,
                             MPARAM mp2);
extern VOID            DispErrorMsg(HAB hab,
                                    HWND hwndFrame,
                                    PCH FileName,
                                    LONG LineNum);
#define DispError(a,b) DispErrorMsg(a,b,(PCH)__FILE__,(LONG)__LINE__)

VOID       CheckPulldown(PMAIN_PARM,
                         LONG);
BOOL       Initialize(VOID);
VOID       ReportError(VOID);
VOID       EndProgram(VOID);
VOID       AdjustWindowSize(PMAIN_PARM);
VOID       DrawThread(PMAIN_PARM);
VOID       PrintThread(PMAIN_PARM);

BOOL       SendCommand(PMAIN_PARM,
                       ULONG,
                       ULONG);
MRESULT    WndProcCreate(HWND hwnd,
                         PMAIN_PARM Parm);
VOID       SetPrintItems(PMAIN_PARM Parm);
VOID       SetTransform(PMAIN_PARM Parm,
                        MPARAM mp1);
MRESULT    WndProcPaint(PMAIN_PARM Parm);
VOID       Transform(PDRAW_PARM Parm,
                     HPS hpsXForm,
                     PRECTL prclXform);
LONG       DoDraw(HPS hpsDraw,
                  PDRAW_PARM Parm,
                  HRGN rgn,
                  HPS InfoWnd);
BOOL       Circles(HPS hpsDraw,
                   PPOINTL pptlCur,
                   FIXED fxSize);
VOID       StopDrawing(PMAIN_PARM Parm);
VOID       PrintGpiMeta(PMAIN_PARM Parm,
                        ULONG Message);
VOID       DisplayMessage(HWND hwndDraw,
                          PSZ Message);
VOID       SetTitleBarText(PMAIN_PARM Parm,
                           PSZ Filename);
PSZ        GetString(HAB hAB,
                     ULONG ID,
                     BOOL Alloc);
PSZ        StringWithVarText(HAB hAB,
                             ULONG ID,
                             PSZ VarText,
                             PSZ Result);
VOID       DisplayHelpPanel(SHORT idPanel);
BOOL       OpenFileDialog(HWND hwndOwner,
                          PSZ pszTitle,
                          PSZ pszFileExt,
                          PSZ pszFilename);
MRESULT EXPENTRY AboutDlg(HWND,
                          USHORT,
                          MPARAM,
                          MPARAM);
MRESULT EXPENTRY TemplateOpenFilterProc(HWND hwnd,
                                        ULONG msg,
                                        MPARAM mp1,
                                        MPARAM mp2);
VOID DisplayErrorMessage(HWND hwndDraw, PSZ Message);

extern   BOOL       QueryPrinterQueue(HAB hAB,
                                      HWND hWnd,
                                      PVOID *DriverData,
                                      PULONG DriverDataLength,
                                      PSZ QueueName,
                                      PSZ DriverName);
extern   BOOL       QueryDefaultQueue(PSZ, PSZ);
extern   BOOL       QueryJobProperties(HAB hAB,
                                       PVOID *DriverData,
                                       PULONG DriverDataLength,
                                       PSZ QueueDriverName,
                                       PSZ QueueName,
                                       PSZ PrtName,
                                       BOOL Flag);

/*  Standard Dialog box ids */
#define IDD_ABOUTBOX                 1002
#define ID_GRAPHIC                   1001
#define PINFO_ICON                   1019

/*  Dialog item ids */
#define ID_ABOUT                     1003
#define DLG_OK                       2010

/*  Menu item ids */
#define IDM_FILE                      200
#define IDM_F_GPI                     206
#define IDM_F_METAFILE                208
#define IDM_F_METAFILE_LOAD           210
#define IDM_F_METAFILE_PREVIOUS       211
#define IDM_F_PRINT                   212
#define IDM_F_TEXT_PREVIOUS           216
#define IDM_H_ABOUT                   218
#define IDM_H_HELP                    219
#define IDM_H_HELPINDEX               220
#define IDM_H_HELPGEN                 221
#define IDM_H_HELPUSING               222
#define IDM_H_HELPKEYS                223
#define IDM_INFO_OK                   228

/*  Help item ids */
#define HELP_INDEX                  64044
#define HELP_GEN                    20409
#define HELP_KEYS                   20410
#define HELP_USING                  64027
#define PANEL_HELPKEYS              20818
#define HID_FS_OPEN_DLG_HELP_PANEL  20809

#define IDM_OPTIONS                   224
#define IDM_O_PRINTPROPERTIES         226

/*  Messagetable ids */
#define  ERRMSG_REGISTER_FAILED      3004
#define  ERRMSG_PRINT_THREAD         3005
#define  ERRMSG_CREATE_WINDOW        3006
#define  ERRMSG_INFO_WINDOW          3008
#define  ERRMSG_DRAW_THREAD          3009
#define  ERRMSG_NO_HELP              3010
#define  ERRMSG_METAFILE_FAILED      3011
#define  ERRMSG_HELP_TERMINATED      3012
#define  ERRMSG_HELP_ERROR           3013
#define  ERRMSG_HELPDISPLAYERROR     3014
#define  ERRMSG_DRAWTHR_MSG2         3033
#define  IDS_ERRORMSG                27
#define  IDS_UNKNOWNMSG              28

/*  Stringtable ids */
#define  APPLICATION_NAME            3037
#define  FILEDLG_META_HEADER         3039
#define  TEXT_SPOOLING_MESSAGE       3046
#define  TEXT_DRAWTHR_SIZEGPI        3053
#define  TEXT_DRAWTHR_SIZEMETA       3054

/*  Help table and subtables */
#define SUBTABLE_MAIN                2000
#define SUBTABLE_INFO                3000

/*  Main window help panels */
#define PANEL_MAIN                   2100
#define PANEL_INFO                   3100

/*  Main window subtable, includes menu item help */
#define PANEL_FILE                  20801
#define PANEL_FILELOAD              20803
#define PANEL_PRINT                 20804

#define PANEL_HELP                  64022
#define PANEL_HELPINDEX             64026
#define PANEL_HELPGENERAL           64024
#define PANEL_HELPUSING             64023
#define PANEL_HELPABOUT             64096

/*  Message box help resource entry points */
#define PANEL_INFO_OK               20805
/************************** End of GRAPHIC.H ******************************/

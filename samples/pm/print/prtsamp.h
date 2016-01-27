/**************************************************************************
 *  File name  :  prtsamp.h
 *
 *  Description:  The PRTSAMP include file defines symbolic constants
 *                and data structures used by print sample.
 *
 *  Concepts   :  [none]
 *
 *  API's      :  [none]
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/

/* constants */

#define QUEUENAME_LENGTH    10
#define QUEUEDESC_LENGTH    64
#define DEVICENAME_LENGTH   32
#define DRIVERNAME_LENGTH   128


/* string definitions */

#define CLASSNAME            "PRTSAMP_CLIENTWINDOW"
#define OBJECTCLASSNAME      "PRTSAMP_OBJECTWINDOW"
#define HELP_FILE            "PRTSAMP.HLP"



/* length definitions */

#define LEN_STACK             6*4096
#define LEN_WORKSTRING        256
#define LEN_PLAYMETAFILEOPTS  PMF_DEFAULTS+1
#define LEN_PLAYMETAFILEDESCS 256
#define LEN_DEATH_TIMER       12000        /* milliseconds */
#define LEN_SIZE_TIMER         250         /* milliseconds */


#define ID_SIZE_TIMER           1
#define ID_DEATH_TIMER         13


/* translation offset in twips for default view matrix for both x, y */
#define OFFSET_XY_TWIPS        360

/* maximum number of forms that can be handled in page setup dialog */
#define MAX_FORMS              20

/* categories */

#define FLAGS_SCREEN     0
#define FLAGS_PRINTER    1
#define FLAGS_INFO       2

#define PAGINATE_EOF              0    /* drew nothing on the page, eof   */
#define PAGINATE_NOT_EOF          1    /* drew something, and not eof yet */
#define PAGINATE_EOF_PART_PAGE    2    /* drew something, and eof         */

#define VIEW_FULLSIZE             0   /* default */
#define VIEW_HALFSIZE             1

#define MODE_TEXT            0
#define MODE_BITMAP          1
#define MODE_METAFILE        2
#define MODE_UNKNOWN         3

/* user-defined messages */

#define WM_USER_CLOSE                  (  WM_USER +  0 )
#define WM_USER_ACK                    (  WM_USER +  1 )
#define WM_USER_ENABLE_CLIENT          (  WM_USER +  2 )
#define WM_USER_DISABLE_CLIENT         (  WM_USER +  3 )
#define WM_USER_SLEEP                  (  WM_USER +  4 )
#define WM_USER_NEW_MODE               (  WM_USER +  5 )
#define WM_USER_LOAD_TEXT              (  WM_USER +  6 )
#define WM_USER_LOAD_BITMAP            (  WM_USER +  7 )
#define WM_USER_LOAD_METAFILE          (  WM_USER +  8 )
#define WM_USER_PAINT_BITMAP           (  WM_USER +  9 )
#define WM_USER_PAINT_METAFILE         (  WM_USER + 10 )
#define WM_USER_PAGINATE               (  WM_USER + 11 )
#define WM_USER_PRINT                  (  WM_USER + 12 )
#define WM_USER_NEW_PAGE               (  WM_USER + 13 )
#define WM_USER_SKIP_PAGE              (  WM_USER + 14 )
#define WM_USER_PAINT_DEFAULT_SCREEN   (  WM_USER + 15 )


#define WM_NACK_FILE_READING_ERROR     (  WM_USER +  50 )
#define WM_NACK_BITMAP_NOT_SUPPORTED   (  WM_USER +  51 )
#define WM_NACK_BITMAP_ERROR           (  WM_USER +  52 )

/* user-defined structures */

/*
 * Structure of form
 */
typedef struct _FORM {
   CHAR        achFormName[32];
   FIXED       fxxWidth;         /* form width in fractional inches */
   FIXED       fxyHeight;        /* form height */
   FIXED       fxLeftMargin;     /* left margin in fractional inches */
                                 /* always multiple of 1/20 inch */
   FIXED       fxRightMargin;
   FIXED       fxTopMargin;
   FIXED       fxBottomMargin;
} FORM;
typedef FORM PFORM;


/*
 * Structure of program settings that are saved in the INI.
 * They are used as default setting upon the next invocation of the program.
 */

typedef struct _PRTSAMP_PROFILE {

    ULONG       flOptions;
    ULONG       cy;
    ULONG       cx;
    ULONG       y;
    ULONG       x;

    FIXED       fxPointSize;
    FATTRS      fAttrs;

    ULONG       ulNextMode;                     /* next mode to use     */
    CHAR        szNextFilename[ CCHMAXPATH ];   /* next filename to use */

    FORM        form;                           /* next form to use     */

    CHAR        achQueueName[QUEUENAME_LENGTH];
    CHAR        achDriverName[DRIVERNAME_LENGTH];
    LONG        cbDriverDataLen;

    USHORT      usCopies;                          /* number of copies       */
    BOOL        fPrintAllPages;       /* flag is all pages are to be printed */
    USHORT      usFirstPage;                       /* first page to print    */
    USHORT      usLastPage;                        /* last page to print     */
} PRTSAMP_PROFILE;
typedef PRTSAMP_PROFILE PPRTSAMP_PROFILE;


/*
 * block of main program parameters
 * the variable that points to this block is usually named pmp
 */

typedef  struct _MAIN_PARM {
    HAB        hab;               /* hab obtained on thread 1                */

    HWND       hwndFrame;         /* frame handle                            */
    HWND       hwndClient;        /* client window handle                    */
    HWND       hwndMenubar;       /* menu handle, child of frame             */
    HWND       hwndTitlebar;      /* title bar handle, child of frame        */
    HWND       hwndObject;        /* object window handle on thread 2        */
    HWND       hwndHScroll;       /* horizontal scroll bar handle            */
    HWND       hwndVScroll;       /* vertical scroll bar handle              */
    HWND       hwndHelp;          /* help instance window handle             */

    HDC        hdcClient;         /* client window device-context handle     */
    HPS        hpsClient;         /* client window presentation space handle */

    HDC        hdcMemory;         /* memory dc to hold bitmap                */
    HPS        hpsMemory;         /* memory ps to hold bitmap                */
    HDC        hdcMem4Printer;    /* memory dc to hold bitmap when printing  */
    HPS        hpsMem4Printer;    /* memory ps to hold bitmap when printing  */

    HDC        hdcPrinter;        /* current printer dc                      */
    HPS        hpsPrinter;        /* current printer ps                      */

    HDC        hdcPrinterInfo;    /* dc for printer info: type OD_INFO       */
    HPS        hpsPrinterInfo;    /* ps for printer info                     */

    HBITMAP    hbm;               /* handle to current bitmap in use         */
    HBITMAP    hbm4Printer;       /* handle to bitmap in use when printing   */
    FILE *     f;                 /* handle to current text file in use      */
    HMF        hmf;               /* handle to current metafile in use       */

    BOOL       fBusy;               /* object window is busy with a task     */
    BOOL       fVisitedFontDialog;  /* just visited the font dialog          */

    ULONG      ulMode;                        /* current operating mode      */
    CHAR       szFilename[ CCHMAXPATH ];      /* current drive/path/filename */

    ULONG      ulNextMode;                    /* next mode to use            */
    CHAR       szNextFilename[ CCHMAXPATH ];  /* next filename to use        */

    FILEDLG     filedlg;        /* for WinFileDlg                            */
    FONTDLG     fontdlg;        /* for WinFontDlg                            */

    PRTSAMP_PROFILE Profile;    /* settings read/written from/to os2.ini     */

    PSZ         pszTitle;       /* app title initialized in prtcreat.c       */

    CHAR        szFamilyname[LEN_WORKSTRING]; /* fontdlg.pszFamilyname core  */

    MATRIXLF    matlfDefView;   /* default view matrix for zooming text pages */

    SIZEL       sizelPage;      /* size of page user wants, in twips          */
    SIZEL       sizelClient;    /* size of client in twips; see WM_SIZE       */

    SHORT       sHScrollPos;    /* horizontal scroll bar position             */
    SHORT       sHScrollRange;  /* horizontal scroll bar range                */
    SHORT       sVScrollPos;    /* vertical scroll bar position               */
    SHORT       sVScrollRange;  /* vertical scroll bar range                  */
    float       floatScale;     /* current scaling factor for screen viewing  */

    /* current bitmap information */
    ULONG       cxBitmap;
    ULONG       cyBitmap;
    ULONG       cxBitmapRes;
    ULONG       cyBitmapRes;

    /* variables used by print queue selection/printing dialogs */
    BOOL        fCancel;    /* flag to indicate user wants to cancel printing */
    PPRQINFO3   pQueueInfo;               /* temp work space for a dialog */
    LONG        lQueueCount;              /* temp work space for a dialog */
    USHORT      usCopies;
    CHAR        achQueueName[QUEUENAME_LENGTH];    /* currently-selected queue*/
    CHAR        achQueueDesc[QUEUEDESC_LENGTH];    /* and description        */
    CHAR        achDriverName[DRIVERNAME_LENGTH];  /* and driver             */
    LONG        cbDriverDataLen;                   /* driver data length     */
    PDRIVDATA   pDriverData;                       /* driver data            */
    HWND        hwndPrinting;                      /* print confirmation hwnd*/

    FORM        form;                              /* form to use            */
    LONG        cForms;                            /* number of forms defined*/
    HCINFO      hcinfo[MAX_FORMS];                 /* form names from printer*/
    ULONG       iForm;                             /* index to form chosen   */
    ULONG       cLinesinFile;         /* number of lines in the text file    */
    ULONG       cLinesonPage;         /* number of lines on a page           */
    BOOL        fPrintAllPages;       /* flag is all pages are to be printed */
    USHORT      usFirstPage;                       /* first page to print    */
    USHORT      usLastPage;                        /* last page to print     */
    TID         tidObjectThread;      /* thread id for object window         */
    CHAR        szArgFilename[LEN_WORKSTRING];     /* filename from main args*/
} MAIN_PARM;
typedef MAIN_PARM *PMAIN_PARM;

/* Function prototypes used throughout print sample */
void _Optlink threadmain( void * );
PMAIN_PARM Create( HWND hwnd );
MRESULT EXPENTRY ObjectWinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY WinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT Menu( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
ULONG APIENTRY FileTypeDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
ULONG GetEAFileType( HFILE hfile, HAB hab);
MRESULT APIENTRY MyFileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT APIENTRY MyFontDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
PSZ trim( PSZ s );
MRESULT EXPENTRY ProdInfoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID SetSysMenu( HWND hwnd );

BOOL QueryPrintQueue( PMAIN_PARM pmp, BOOL fNeedDialog );
BOOL Print(HWND hwnd, PMAIN_PARM pmp);
VOID ProcessUserPrint(HWND hwnd, PMAIN_PARM pmp);
VOID GetProfile(PMAIN_PARM pmp);
VOID SaveProfile(PMAIN_PARM pmp);

VOID CalibrateVerticalScrollBar( PMAIN_PARM pmp );
VOID CalibrateHorizontalScrollBar( PMAIN_PARM pmp );
LONG FindForm( PSZ pszFormName, PHCINFO pForms, LONG lCount);
ULONG FixedInchesToTwips(FIXED fxInch);
FIXED FixedPointsToTwips(FIXED fxPointSize);
VOID PaintBitmap( PMAIN_PARM pmp, ULONG flag );
VOID PaintMetaFile( PMAIN_PARM pmp, ULONG flag );
BOOL QueryForm( PMAIN_PARM pmp, BOOL fNeedDialog );
LONG QueryPrinterForms( PMAIN_PARM pmp, HCINFO hcinfo[], ULONG cMaxForms );
BOOL ValidateFilename( PMAIN_PARM pmp, PSZ pszFilename, HWND hwndOwner);

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
/***************************  End of prtsamp.h ****************************/

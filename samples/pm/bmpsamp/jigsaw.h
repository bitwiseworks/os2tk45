/******************************************************************************
*
*  File Name   : JIGSAW.H
*
*  Description : Jigsaw's main header file
*
*  Copyright (C) 1992 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
******************************************************************************/

#define INCL_BITMAPFILEFORMAT

#define INCL_DOSMEMMGR
#define INCL_DOSINFOSEG
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES

#define INCL_DEV
#define INCL_BASE

#define INCL_WINSYS
#define INCL_WINSTDFILE
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINDIALOGS
#define INCL_WINFRAMEMGR
#define INCL_WINPOINTERS
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WINRECTANGLES
#define INCL_WINSCROLLBARS
#define INCL_WINSWITCHLIST
#define INCL_WINENTRYFIELDS

#define INCL_GPIPATHS
#define INCL_GPILCIDS
#define INCL_GPIREGIONS
#define INCL_GPICONTROL
#define INCL_GPIBITMAPS
#define INCL_GPIPRIMITIVES
#define INCL_GPITRANSFORMS

#define INCL_ERRORS

#include <os2.h>

#define MSGBOXID                222    /* message box id */
#define IDR_MAIN                1      /* action bar     */

/*
 *   Bitmap resource IDs
 */

#define IDB_BACK_1      41

/*
 *   Menu/Submenu resource IDs
 */

#define IDM_FILE                100
#define IDM_LOAD                150
#define IDM_SWITCH              170
#define IDM_OPTIONS             200
#define IDM_HIDE                220
#define IDM_JUMBLE              300

#define IDM_SIZE_MENU           930
#define IDM_SIZE_SMALL          940
#define IDM_SIZE_MEDIUM         950
#define IDM_SIZE_LARGE          960
#define IDM_SIZE_FULL           970

#define IDM_HELP                90
#define IDM_HELPHELPFORHELP     91
#define IDM_HELPEXTENDED        92
#define IDM_HELPINDEX           93
#define IDM_HELPABOUT           94
#define IDM_HELPTUTORIAL        99

/*
 *   String resource IDs
 */

#define IDS_TITLEBAR            1
#define IDS_TERMINATE           2
#define IDS_OPENDLGHEADER       7
#define IDS_OPENDLGBUTTON       8
#define IDS_FILEOPENEXTENSION   9
#define IDS_CANNOTLOADSTRING    10
#define IDS_CANNOTRUNFILEDLG    11
#define IDS_HELPLIBRARYNAME     20
#define IDS_HELPWINDOWTITLE     21
#define IDS_HELPLOADERROR       22
#define IDS_HELPDISPLAYERROR    23
#define IDS_HELPMANAGERERROR    24
#define IDS_OPTIONJUMBLE        25
#define IDS_LOADBITMAP          26
#define IDS_ERRORMSG            27
#define IDS_UNKNOWNMSG          28
#define IDS_ERROR_READ          29
#define IDS_ERROR_OUTOFMEMORY   30
#define IDS_ERROR_TITLE         31

/*
 *   Dialog Box/Controls IDs
 */

#define IDD_ABOUTBOX            1001
#define IDC_OK                  1
#define IDC_CANCEL              2
#define IDC_HELP                3
#define IDC_ICON                4

/*
 *   Other defines
 */

#define ADD_HEAD_SEG            1
#define ADD_TAIL_SEG            2
#define DEL_SEG                 3
#define MAKE_TAIL_SEG           4
#define MAKE_HEAD_SEG           5
#define PICTURE_CREATE          0
#define PICTURE_UPDATE          1
#define ZOOM_MAX                8
#define UNITY                   65536L         /* resets picture to full size */
#define STACKSIZE               8192           /* async thread stack size     */
#define MESSAGELEN              80             /* length for message buffers  */
#define FILEPROTOSIZE           16             /* length for file prototypes  */


/*----------------------- helper macros --------------------------------------*/


/*
 *   Calculate number of colors in bitmap color table based on number of
 *   bits per scan line.
 */

#define CCOLORS(bc)  (((bc) == 24) ? 0 : (1 << (bc)))

/*
 *   Macros to round numbers up or down for specific byte alignment
 *   for current display.
 */

#define ROUND_DOWN_MOD( arg, mod)       \
    {                                       \
        if( arg < 0)                        \
        {                                   \
            arg -= mod - 1;                 \
            arg += arg % mod;               \
        } else                              \
            arg -= arg % mod;               \
    }

#define ROUND_UP_MOD( arg, mod)        \
    {                                       \
        if( arg < 0)                        \
        {                                   \
            arg -= arg % mod;               \
        } else                              \
        {                                   \
            arg += mod - 1;                 \
            arg -= arg % mod;               \
        }                                   \
    }

/*
 *   shorthand cast of BITMAPFILEHEADER2 to BITMAPFILEHEADER
 */

#define PBFH1(A)      ((PBITMAPFILEHEADER) (A))

/*
 *   shorthand cast of BITMAPINFOHEADER2 to BITMAPINFOHEADER
 */

#define PBMP1(A)      ((PBITMAPINFOHEADER) (A))

/*----------------------- inter-thread messages ------------------------------*/

#define UM_DIE        WM_USER+1       /* instruct async thread to terminate  */
#define UM_DRAW       WM_USER+2       /* draw the current picture            */
#define UM_VSCROLL    WM_USER+3       /* perform scroll by recalculating the */
                                      /* default viewing transform           */
#define UM_HSCROLL    WM_USER+4       /* perform scroll by recalculating the */
                                      /* default viewing transform           */
#define UM_SIZING     WM_USER+5       /* perform sizing by recalculating the */
                                      /* default viewing transform           */
#define UM_ZOOM       WM_USER+6       /* zoom the picture by recalculating   */
                                      /* the default viewing transform       */
#define UM_REDRAW     WM_USER+8       /* redraw the entire client window     */
#define UM_JUMBLE     WM_USER+9       /* scatter the pieces on the window    */
#define UM_LOAD       WM_USER+10      /* load a bitmap from disk             */
#define UM_LEFTDOWN   WM_USER+11      /* correlate and prepare to drag       */
#define UM_MOUSEMOVE  WM_USER+12      /* remove, reposition, and redraw      */
#define UM_LEFTUP     WM_USER+13      /* stop dragging                       */
#define UM_CHAR       WM_USER+14      /* a keyboard key                      */
#define UM_LAST       WM_USER+15


/*------------------------- correlation parameters ---------------------------*/

#define HITS   1L             /* maximum number of hits to return    */
#define DEPTH   2L            /* max depth of seg calls to return    */

/*-------------------------- old-style bitmap header -------------------------*/

typedef struct
{
    USHORT    wType;
    ULONG     dwSize;
    int       xHotspot;
    int       yHotspot;
    ULONG     dwBitsOffset;
    USHORT    bmWidth;
    USHORT    bmHeight;
    USHORT    bmPlanes;
    USHORT    bmBitcount;
} RCBITMAP;
typedef RCBITMAP *PRCBITMAP;

/*-------------------------- bitmap-related data -----------------------------*/

typedef struct _LOADINFO   /* li */
{
   HFILE   hf;
   CHAR    szFileName[CCHMAXPATH];
} LOADINFO, *PLOADINFO;   /* pli */



/*-------------------------- segment list ------------------------------------*/

typedef struct _SEGLIST   /* sl */
{
  LONG         lSegId;                 /* unique id for this pseudo-segment  */
  struct _SEGLIST * pslPrev;           /* previous piece in seglist          */
  struct _SEGLIST * pslNext;           /* next piece in seglist              */
  POINTL       ptlLocation;            /* piece location, world coordinates  */
  RECTL        rclCurrent;             /* segment bounding box, model coords */
  RECTL        rclBitBlt;              /* segment bounding box, world coords */
  POINTL       aptlBitBlt[4];          /* BitBlt parameters                  */
  LONG         lAdjacent[8];           /* adjacent segments in original pict */
  struct _SEGLIST * pslNextIsland;     /* next member in island list         */
  BOOL         fIslandMark;            /* used for marking island members    */
  POINTL       aptlSides[12];          /* control points for piece outline   */
  HDC          hdcHole;                /* hdc used to punch hole in target   */
  HPS          hpsHole;                /* hps used to punch hole in target   */
  HBITMAP      hbmHole;                /* hbm used to punch hole in target   */
  HDC          hdcFill;                /* hdc used to fill hole              */
  HPS          hpsFill;                /* hps used to fill hole              */
  HBITMAP      hbmFill;                /* hbm used to fill hole              */
  POINTL       ptlModelXlate;          /* lM31 and lM31 for piece            */
  BOOL         fVisible;               /* TRUE iff should be drawn           */
} SEGLIST ;
typedef SEGLIST *PSEGLIST;     /* psl  */
typedef PSEGLIST *PPSEGLIST;   /* ppsl */


/*------------------------- Function Prototypes ------------------------------*/

/*
 *   External prototypes
 */

#pragma linkage (DosGetThreadInfo, system)
APIRET DosGetThreadInfo(PTIB *pptib,PPIB *pppib);

/*
 *   Jigsaw funciton prototypes
 */

extern  VOID  CalcBounds( VOID);
extern  VOID  CalcSize( MPARAM mp1, MPARAM mp2);
extern  VOID  CalcTransform( HWND hwnd);
extern  VOID  CheckMenu(ULONG id, BOOL fCheck);
extern  VOID  CheckPsl( PSEGLIST psl);
extern  MRESULT EXPENTRY
              ClientWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM  mp2);
extern  PSEGLIST   Correlate( PPOINTL pptl);
extern  BOOL  CreateBitmapHdcHps( PHDC phdc, PHPS phps);
extern  BOOL  CreatePicture( SHORT sUpdate);
extern  VOID  DispErrorMsg(HAB hab, HWND hwndFrame, PCH FileName, LONG LineNum);
extern  VOID  DisplayZoomFactor(LONG lScaleFactor);
extern  BOOL  DoDraw( HPS hps, HRGN hrgn, BOOL fPaint);
extern  VOID  DoHorzScroll( VOID);
extern  VOID  DoVertScroll( VOID);
extern  VOID  DrawPiece( HPS hps, PSEGLIST psl, BOOL fFill);
extern  BOOL  DumpPicture( VOID);
extern  VOID  EnableMenu(ULONG id, BOOL fEnable);
extern  VOID  Finalize( VOID);
extern  VOID  HelpInit(VOID);
extern  BOOL  Initialize( VOID);
extern  VOID  Jumble( VOID);
extern  VOID  LeftDown( MPARAM mp);
extern  VOID  LeftUp( VOID);
extern  VOID  Load( PLOADINFO pli);
extern  VOID  main( VOID);
extern  VOID  MarkIsland( PSEGLIST pslMark, BOOL fMark);
extern  ULONG MessageBox(HWND hwndOwner, ULONG idMsg, ULONG fsStyle, BOOL fBeep);
extern  VOID  MouseMove( MPARAM mp);
extern  VOID  _Optlink NewThread(PVOID);
extern  BOOL  PrepareBitmap( VOID);
extern  BOOL  ReadBitmap( HFILE hfile);
extern  VOID  Redraw( VOID);
extern  VOID  ReportError( HAB hab);
extern  VOID  ResetScrollBars( VOID);
extern  BOOL  SegListCheck( INT iLoc);
extern  PSEGLIST   SegListGet( LONG lSeg);
extern  PSEGLIST   SegListUpdate( USHORT usOperation, PSEGLIST pslUpdate);
extern  BOOL  SendCommand( ULONG ulCommand, MPARAM mp1, MPARAM mp2);
extern  VOID  SetDVTransform(FIXED fx11, FIXED fx12, FIXED fx21,
                             FIXED fx22, FIXED l31,FIXED l32, FIXED lType);
extern  VOID  SetRect( PSEGLIST psl);
extern  VOID  SizeBitmap(LONG);
extern  VOID  ToBottom( PSEGLIST pslDown);
extern  MRESULT    WndProcCommand(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
extern  MRESULT    WndProcCreate( HWND hwnd);
extern  MRESULT    WndProcPaint( VOID);
extern  VOID  Zoom(VOID);

/*
 *   IPF handling routines and standard dialog front ends
 */

extern  VOID  DestroyHelpInstance(VOID);
extern  VOID  DisplayHelpPanel(SHORT nPanelId);
extern  BOOL  FileOpen( VOID );
extern  VOID  HelpAbout(MPARAM mp2);
extern  VOID  HelpExtended( VOID );
extern  VOID  HelpHelpForHelp(MPARAM mp2);
extern  VOID  HelpIndex(MPARAM mp2);
extern  VOID  HelpTutorial(MPARAM mp2);
extern  VOID  InitHelp(VOID);
extern  MRESULT EXPENTRY
              JigsawOpenFilterProc( HWND   hwnd, ULONG message,
                                       MPARAM mp1,  MPARAM mp2);

/*
 *   Macro frontend to error message box.  We only want this particular
 *   message box for debugging.  If DEBUG is not defined (when shipped
 *   Jigsaw application is built), the macro expands to nothing, and
 *   the real function isn't called.
 */

#if defined(DEBUG)
   #define DispError(a,b)   DispErrorMsg(a,b,(PCH)__FILE__,(LONG)__LINE__)
#else
   #define DispError(a,b)
#endif

/******************************** end JIGSAW.H *******************************/

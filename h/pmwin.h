/****************************** Module Header ******************************\
*
* Module Name: PMWIN.H
*
* OS/2 Presentation Manager Window Manager include file.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1993
*
* ===========================================================================
*
* The folowing symbols are used in this file for conditional sections.
*
* If INCL_WIN is defined, all subcomponents are included.
*
* Subcomponents marked with "+" are partially included by default:
*
*   #define:                To include:
*
* + INCL_WINWINDOWMGR       General window management
* + INCL_WINMESSAGEMGR      Message management
* + INCL_WININPUT           Mouse and keyboard input
* + INCL_WINDIALOGS         Dialog boxes
* + INCL_WINSTATICS         Static controls
* + INCL_WINBUTTONS         Button controls
* + INCL_WINENTRYFIELDS     Entry Fields
*   INCL_WINMLE             Multiple Line Entry Fields
* + INCL_WINLISTBOXES       List box controls
* + INCL_WINMENUS           Menu controls
* + INCL_WINSCROLLBARS      Scroll bar controls
* + INCL_WINFRAMEMGR        Frame manager
*   INCL_WINFRAMECTLS       Frame controls (title bars & size border)
*   INCL_WINRECTANGLES      Rectangle routines
*   INCL_WINSYS             System values (and colors)
*   INCL_WINTIMER           Timer routines
* + INCL_WINACCELERATORS    Keyboard accelerators
*   INCL_WINTRACKRECT       WinTrackRect() function
*   INCL_WINCLIPBOARD       Clipboard manager
* + INCL_WINCURSORS         Text cursors
* + INCL_WINPOINTERS        Mouse pointers
*   INCL_WINHOOKS           Hook manager
* + INCL_WINSWITCHLIST      Shell Switch List API
*   INCL_WINPROGRAMLIST     Shell Program List API
*   INCL_WINSHELLDATA       Shell Data (?)
*   INCL_WINCOUNTRY         Country support
*   INCL_WINHEAP            Heap Manager
*   INCL_WINATOM            Atom Manager
*   INCL_WINERRORS          Error code definitions
*   INCL_NLS                DBCS window manager definition
* + INCL_WINHELP            Help Manager definitions
*   INCL_WINSEI             Set Error Info API
*   INCL_WINLOAD            Load/Delete Library/Procedure
*   INCL_WINTYPES           Definitions for Datatypes
*   INCL_WINTHUNKAPI        Thunk Procedure API
*   INCL_WINDESKTOP         Desktop API
*   INCL_WINPROP            Window Property API
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between NOINC
* and INC comments.
*
\***************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( none )
   #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#define WIN_INCLUDED




/*** include everything if requested ***************************************/
#ifdef INCL_WIN
   #define INCL_WINWINDOWMGR
   #define INCL_WINMESSAGEMGR
   #define INCL_WININPUT
   #define INCL_WINDIALOGS
   #define INCL_WINSTATICS
   #define INCL_WINBUTTONS
   #define INCL_WINENTRYFIELDS
   #define INCL_WINMLE
   #define INCL_WINLISTBOXES
   #define INCL_WINMENUS
   #define INCL_WINSCROLLBARS
   #define INCL_WINFRAMEMGR
   #define INCL_WINFRAMECTLS
   #define INCL_WINRECTANGLES
   #define INCL_WINSYS
   #define INCL_WINTIMER
   #define INCL_WINACCELERATORS
   #define INCL_WINTRACKRECT
   #define INCL_WINCLIPBOARD
   #define INCL_WINCURSORS
   #define INCL_WINPOINTERS
   #define INCL_WINHOOKS
   #define INCL_WINSWITCHLIST
   #define INCL_WINPROGRAMLIST
   #define INCL_WINSHELLDATA
   #define INCL_WINCOUNTRY
   #define INCL_WINHEAP
   #define INCL_WINATOM
   #define INCL_WINERRORS
   #define INCL_WINDDE
   #define INCL_WINHELP
   #define INCL_WINSEI
   #define INCL_WINLOAD
   #define INCL_WINTYPES
   #define INCL_WINDESKTOP
   #define INCL_WINPALETTE
   #define INCL_WINTHUNKAPI
   #define INCL_WINSTDDLGS
   #define INCL_WINBIDI
#else /* INCL_WIN */
   /*** include resource compiler required sections ************************/
   #ifdef RC_INVOKED
      #define INCL_WININPUT
      #define INCL_WINDIALOGS
      #define INCL_WINSTATICS
      #define INCL_WINBUTTONS
      #define INCL_WINENTRYFIELDS
      #define INCL_WINLISTBOXES
      #define INCL_WINMENUS
      #define INCL_WINSCROLLBARS
      #define INCL_WINFRAMEMGR
      #define INCL_WINFRAMECTLS
      #define INCL_WINACCELERATORS
      #define INCL_WINPOINTERS
      #define INCL_WINMESSAGEMGR
      #define INCL_WINMLE
      #define INCL_WINHELP
      #define INCL_WINSTDDLGS
      #define INCL_WINSYS
   #endif /* RC_INVOKED */
#endif /* INCL_WIN */

/*** ensure standard entry field defintions if MLE is defined **************/
#ifdef INCL_WINMLE
   #ifndef INCL_WINENTRYFIELDS
      #define INCL_WINENTRYFIELDS
   #endif /* INCL_WINENTRYFIELDS */
#endif /* INCL_WINMLE */

/*** INCL_WINCOMMON compatability ******************************************/
#ifdef INCL_WINCOMMON
   #define INCL_WINWINDOWMGR
#endif /* INCL_WINCOMMON */

/*** include PMWIN errors if all errors requested **************************/
#ifdef INCL_ERRORS
   #define INCL_WINERRORS
#endif  /* INCL_ERRORS */

/***************************************************************************/
/***        General Window Management types, constants and macros        ***/
/***************************************************************************/

/*** Macros to make an MPARAM from standard types. *************************/

#define MPVOID                     ((MPARAM)0L)
#define MPFROMP(p)                 ((MPARAM)((ULONG)(p)))
#define MPFROMHWND(hwnd)           ((MPARAM)(HWND)(hwnd))
#define MPFROMCHAR(ch)             ((MPARAM)(USHORT)(ch))
#define MPFROMSHORT(s)             ((MPARAM)(USHORT)(s))
#define MPFROM2SHORT(s1, s2)       ((MPARAM)MAKELONG(s1, s2))
#define MPFROMSH2CH(s, uch1, uch2) ((MPARAM)MAKELONG(s, MAKESHORT(uch1, uch2)))
#define MPFROMLONG(l)              ((MPARAM)(ULONG)(l))

/*** Macros to extract standard types from an MPARAM ***********************/

#define PVOIDFROMMP(mp)            ((VOID *)(mp))
#define HWNDFROMMP(mp)             ((HWND)(mp))
#define CHAR1FROMMP(mp)            ((UCHAR)((ULONG)mp))
#define CHAR2FROMMP(mp)            ((UCHAR)((ULONG)mp >> 8))
#define CHAR3FROMMP(mp)            ((UCHAR)((ULONG)mp >> 16))
#define CHAR4FROMMP(mp)            ((UCHAR)((ULONG)mp >> 24))
#define SHORT1FROMMP(mp)           ((USHORT)(ULONG)(mp))
#define SHORT2FROMMP(mp)           ((USHORT)((ULONG)mp >> 16))
#define LONGFROMMP(mp)             ((ULONG)(mp))

/*** Macros to make an MRESULT from standard types. ************************/

#define MRFROMP(p)                 ((MRESULT)(VOID *)(p))
#define MRFROMSHORT(s)             ((MRESULT)(USHORT)(s))
#define MRFROM2SHORT(s1, s2)       ((MRESULT)MAKELONG(s1, s2))
#define MRFROMLONG(l)              ((MRESULT)(ULONG)(l))

/*** Macros to extract standard types from an MRESULT **********************/

#define PVOIDFROMMR(mr)            ((VOID *)(mr))
#define SHORT1FROMMR(mr)           ((USHORT)((ULONG)mr))
#define SHORT2FROMMR(mr)           ((USHORT)((ULONG)mr >> 16))
#define LONGFROMMR(mr)             ((ULONG)(mr))


/****************************************************************************
* This is the standard function definition for window procedures.
* Typically they are names like "XxxxxxxxWndProc", where the prefix
* "Xxxxxxxxx" is replaced by some name descriptive of the window procedure
* being declared.  Window procedures must be EXPORTED in the definitions
* file used by the linker.
*
* MRESULT EXPENTRY MyclassWndProc(HWND hwnd,   * window handle        *
*                                 ULONG msg,   * message number       *
*                                 MPARAM mp1,  * 1st (packed) parms   *
*                                 MPARAM mp2); * 2nd (packed) parms   *
*
****************************************************************************/

/*** Pointer to a window procedure function ********************************/
typedef MRESULT (EXPENTRY FNWP)(HWND, ULONG, MPARAM, MPARAM);
typedef FNWP *PFNWP;

/*** Predefined window handles *********************************************/
#ifndef INCL_SAADEFS
   #define HWND_DESKTOP               (HWND)1
   #define HWND_OBJECT                (HWND)2
#endif /* !INCL_SAADEFS */

#define HWND_TOP                      (HWND)3
#define HWND_BOTTOM                   (HWND)4

#ifndef INCL_SAADEFS
   #define HWND_THREADCAPTURE         (HWND)5
#endif /* !INCL_SAADEFS */

/*** Standard Window Classes ***********************************************/

#define WC_FRAME             ((PSZ)0xffff0001L)
#define WC_COMBOBOX          ((PSZ)0xffff0002L)
#define WC_BUTTON            ((PSZ)0xffff0003L)
#define WC_MENU              ((PSZ)0xffff0004L)
#define WC_STATIC            ((PSZ)0xffff0005L)
#define WC_ENTRYFIELD        ((PSZ)0xffff0006L)
#define WC_LISTBOX           ((PSZ)0xffff0007L)
#define WC_SCROLLBAR         ((PSZ)0xffff0008L)
#define WC_TITLEBAR          ((PSZ)0xffff0009L)
#define WC_MLE               ((PSZ)0xffff000AL)
/* 000B to 000F reserved */
#define WC_APPSTAT           ((PSZ)0xffff0010L)
#define WC_KBDSTAT           ((PSZ)0xffff0011L)
#define WC_PECIC             ((PSZ)0xffff0012L)
#define WC_DBE_KKPOPUP       ((PSZ)0xffff0013L)
/* 0014 to 001F reserved */
#define WC_SPINBUTTON        ((PSZ)0xffff0020L)
/* 0021 to 0024 reserved */
#define WC_CONTAINER         ((PSZ)0xffff0025L)
#define WC_SLIDER            ((PSZ)0xffff0026L)
#define WC_VALUESET          ((PSZ)0xffff0027L)
#define WC_NOTEBOOK          ((PSZ)0xffff0028L)
/* 0029 to 002C used by PEN */
#define WC_PENFIRST          ((PSZ)0xffff0029L)
#define WC_PENLAST           ((PSZ)0xffff002CL)
/* 002D to 0030 reserved */
/* 0030 to 003F reserved */
#define WC_MMPMFIRST         ((PSZ)0xffff0040L)
#define WC_CIRCULARSLIDER    ((PSZ)0xffff0041L)
#define WC_MMPMLAST          ((PSZ)0xffff004fL)
/* 0050 to 005f reserved */
#define WC_PRISTDDLGFIRST    ((PSZ)0xffff0050L)
#define WC_PRISTDDLGLAST     ((PSZ)0xffff0057L)
#define WC_PUBSTDDLGFIRST    ((PSZ)0xffff0058L)
#define WC_PUBSTDDLGLAST     ((PSZ)0xffff005fL)
/* 0060 to 006f reserved */

/*** Standard Window Styles ************************************************/

#define WS_VISIBLE                 0x80000000L
#define WS_DISABLED                0x40000000L
#define WS_CLIPCHILDREN            0x20000000L
#define WS_CLIPSIBLINGS            0x10000000L
#define WS_PARENTCLIP              0x08000000L
#define WS_SAVEBITS                0x04000000L
#define WS_SYNCPAINT               0x02000000L
#define WS_MINIMIZED               0x01000000L
#define WS_MAXIMIZED               0x00800000L
#define WS_ANIMATE                 0x00400000L

/*** Dialog manager styles *************************************************/

#define WS_GROUP                   0x00010000L
#define WS_TABSTOP                 0x00020000L
#define WS_MULTISELECT             0x00040000L

/*** Class styles *********************************************************/

#define CS_MOVENOTIFY              0x00000001L
#define CS_SIZEREDRAW              0x00000004L
#define CS_HITTEST                 0x00000008L
#define CS_PUBLIC                  0x00000010L
#define CS_FRAME                   0x00000020L
#define CS_CLIPCHILDREN            0x20000000L
#define CS_CLIPSIBLINGS            0x10000000L
#define CS_PARENTCLIP              0x08000000L
#define CS_SAVEBITS                0x04000000L
#define CS_SYNCPAINT               0x02000000L

/***************************************************************************/
/*** Window Manager Subsection part 1 **************************************/
/***************************************************************************/

#if (defined(INCL_WINWINDOWMGR) || !defined(INCL_NOCOMMON))
   
#ifdef __cplusplus
   BOOL    APIENTRY WinRegisterClass(HAB hab,
                                     PCSZ  pszClassName,
                                     PFNWP pfnWndProc,
                                     ULONG flStyle,
                                     ULONG cbWindowData);
#else
   BOOL    APIENTRY WinRegisterClass(HAB hab,
                                     PSZ pszClassName,
                                     PFNWP pfnWndProc,
                                     ULONG flStyle,
                                     ULONG cbWindowData);
#endif
   
   MRESULT APIENTRY WinDefWindowProc(HWND hwnd,
                                     ULONG msg,
                                     MPARAM mp1,
                                     MPARAM mp2);
   
   BOOL    APIENTRY WinDestroyWindow(HWND hwnd);
   
   BOOL    APIENTRY WinShowWindow(HWND hwnd,
                                  BOOL fShow);
   
   BOOL    APIENTRY WinQueryWindowRect(HWND hwnd,
                                       PRECTL prclDest);
   
   HPS     APIENTRY WinGetPS(HWND hwnd);
   
   BOOL    APIENTRY WinReleasePS(HPS hps);
   
   BOOL    APIENTRY WinEndPaint(HPS hps);
   
   #ifndef INCL_SAADEFS
      
      HPS   APIENTRY WinGetClipPS(HWND hwnd,
                                  HWND hwndClip,
                                  ULONG fl);
      
      BOOL  APIENTRY WinIsWindowShowing(HWND hwnd);
      
   #endif /* !INCL_SAADEFS */
   
   HPS   APIENTRY WinBeginPaint(HWND hwnd,
                                HPS hps,
                                PRECTL prclPaint);
   HDC   APIENTRY WinOpenWindowDC(HWND hwnd);
   
   LONG APIENTRY WinScrollWindow(HWND hwnd,
                                 LONG dx,
                                 LONG dy,
                                 PRECTL prclScroll,
                                 PRECTL prclClip,
                                 HRGN hrgnUpdate,
                                 PRECTL prclUpdate,
                                 ULONG rgfsw);
   
   /*** WinGetClipPS() flags ***********************************************/
   
   #ifndef INCL_SAADEFS
      #define PSF_LOCKWINDOWUPDATE       0x0001
      #define PSF_CLIPUPWARDS            0x0002
      #define PSF_CLIPDOWNWARDS          0x0004
      #define PSF_CLIPSIBLINGS           0x0008
      #define PSF_CLIPCHILDREN           0x0010
      #define PSF_PARENTCLIP             0x0020
   #endif /* !INCL_SAADEFS */
   
   /*** WinScrollWindow() flags ********************************************/
   
   #define SW_SCROLLCHILDREN          0x0001
   #define SW_INVALIDATERGN           0x0002
   
   BOOL  APIENTRY WinFillRect(HPS hps,
                              PRECTL prcl,
                              LONG lColor);
   
   /*** WinInitialize/WinTerminate Interface declarations ******************/
   
   typedef struct _QVERSDATA    /* qver */
   {
      USHORT   environment;
      USHORT   version;
   } QVERSDATA;
   typedef QVERSDATA *PQVERSDATA;
   
   #define QV_OS2                     0x0000
   #define QV_CMS                     0x0001
   #define QV_TSO                     0x0002
   #define QV_TSOBATCH                0x0003
   #define QV_OS400                   0x0004
   
   
   ULONG  APIENTRY WinQueryVersion(HAB hab);
   
   HAB    APIENTRY WinInitialize(ULONG flOptions);
   
   BOOL   APIENTRY WinTerminate(HAB hab);
   
   HAB    APIENTRY WinQueryAnchorBlock(HWND hwnd);
   
#endif /* INCL_WINWINDOWMGR | !INCL_NOCOMMON */

/***************************************************************************/
/***  End of Window Manager COMMON section *********************************/
/***************************************************************************/


#ifdef __cplusplus
HWND    APIENTRY WinCreateWindow(HWND hwndParent,
                                 PCSZ  pszClass,
                                 PCSZ  pszName,
                                 ULONG flStyle,
                                 LONG x,
                                 LONG y,
                                 LONG cx,
                                 LONG cy,
                                 HWND hwndOwner,
                                 HWND hwndInsertBehind,
                                 ULONG id,
                                 PVOID pCtlData,
                                 PVOID pPresParams);
#else
HWND    APIENTRY WinCreateWindow(HWND hwndParent,
                                 PSZ pszClass,
                                 PSZ pszName,
                                 ULONG flStyle,
                                 LONG x,
                                 LONG y,
                                 LONG cx,
                                 LONG cy,
                                 HWND hwndOwner,
                                 HWND hwndInsertBehind,
                                 ULONG id,
                                 PVOID pCtlData,
                                 PVOID pPresParams);
#endif

BOOL    APIENTRY WinEnableWindow(HWND hwnd,
                                 BOOL fEnable);

BOOL    APIENTRY WinIsWindowEnabled(HWND hwnd);

BOOL    APIENTRY WinEnableWindowUpdate(HWND hwnd,
                                       BOOL fEnable);

BOOL    APIENTRY WinIsWindowVisible(HWND hwnd);

LONG    APIENTRY WinQueryWindowText(HWND hwnd,
                                    LONG cchBufferMax,
                                    PCH pchBuffer);

#ifdef __cplusplus
BOOL    APIENTRY WinSetWindowText(HWND hwnd,
                                  PCSZ  pszText);
#else
BOOL    APIENTRY WinSetWindowText(HWND hwnd,
                                  PSZ pszText);
#endif

LONG    APIENTRY WinQueryWindowTextLength(HWND hwnd);

HWND    APIENTRY WinWindowFromID(HWND hwndParent,
                                 ULONG id);

BOOL    APIENTRY WinIsWindow(HAB hab,
                             HWND hwnd);

HWND    APIENTRY WinQueryWindow(HWND hwnd,
                                LONG cmd);

LONG    APIENTRY WinMultWindowFromIDs(HWND hwndParent,
                                      PHWND prghwnd,
                                      ULONG idFirst,
                                      ULONG idLast);

/*** WinQueryWindow() codes ************************************************/

#define QW_NEXT         0
#define QW_PREV         1
#define QW_TOP          2
#define QW_BOTTOM       3
#define QW_OWNER        4
#define QW_PARENT       5
#define QW_NEXTTOP      6
#define QW_PREVTOP      7
#define QW_FRAMEOWNER   8

BOOL   APIENTRY WinSetParent(HWND hwnd,
                             HWND hwndNewParent,
                             BOOL fRedraw);

BOOL   APIENTRY WinIsChild(HWND hwnd,
                           HWND hwndParent);
BOOL   APIENTRY WinSetOwner(HWND hwnd,
                            HWND hwndNewOwner);
#ifndef INCL_SAADEFS
   BOOL   APIENTRY WinQueryWindowProcess(HWND hwnd,
                                         PPID ppid,
                                         PTID ptid);
#endif /* !INCL_SAADEFS */

HWND   APIENTRY WinQueryObjectWindow(HWND hwndDesktop);
HWND   APIENTRY WinQueryDesktopWindow(HAB hab,
                                      HDC hdc);

/*** Window positioning functions */

/* WinSetMultWindowPos() structure */

typedef struct _SWP    /* swp */
{
   ULONG   fl;
   LONG    cy;
   LONG    cx;
   LONG    y;
   LONG    x;
   HWND    hwndInsertBehind;
   HWND    hwnd;
   ULONG   ulReserved1;
   ULONG   ulReserved2;
} SWP;
typedef SWP *PSWP;

BOOL   APIENTRY WinSetWindowPos(HWND hwnd,
                                HWND hwndInsertBehind,
                                LONG x,
                                LONG y,
                                LONG cx,
                                LONG cy,
                                ULONG fl);
BOOL   APIENTRY WinSetMultWindowPos(HAB hab,
                                    PSWP pswp,
                                    ULONG cswp);
BOOL   APIENTRY WinQueryWindowPos(HWND hwnd,
                                  PSWP pswp);

/* Values returned from WM_ADJUSTWINDOWPOS and passed to WM_WINDOWPOSCHANGED */

#define AWP_MINIMIZED              0x00010000L
#define AWP_MAXIMIZED              0x00020000L
#define AWP_RESTORED               0x00040000L
#define AWP_ACTIVATE               0x00080000L
#define AWP_DEACTIVATE             0x00100000L

/* WinSetWindowPos() flags */

#define SWP_SIZE                   0x0001
#define SWP_MOVE                   0x0002
#define SWP_ZORDER                 0x0004
#define SWP_SHOW                   0x0008
#define SWP_HIDE                   0x0010
#define SWP_NOREDRAW               0x0020
#define SWP_NOADJUST               0x0040
#define SWP_ACTIVATE               0x0080
#define SWP_DEACTIVATE             0x0100
#define SWP_EXTSTATECHANGE         0x0200
#define SWP_MINIMIZE               0x0400
#define SWP_MAXIMIZE               0x0800
#define SWP_RESTORE                0x1000
#define SWP_FOCUSACTIVATE          0x2000
#define SWP_FOCUSDEACTIVATE        0x4000
#define SWP_NOAUTOCLOSE            0x8000    /* Valid in PROGDETAILS struct only */

/* Window painting */

BOOL  APIENTRY WinUpdateWindow(HWND hwnd);

BOOL  APIENTRY WinInvalidateRect(HWND hwnd,
                                 PRECTL pwrc,
                                 BOOL fIncludeChildren);

BOOL  APIENTRY WinInvalidateRegion(HWND hwnd,
                                   HRGN hrgn,
                                   BOOL fIncludeChildren);


/* Drawing helpers */

BOOL  APIENTRY WinInvertRect(HPS hps,
                             PRECTL prcl);
BOOL  APIENTRY WinDrawBitmap(HPS hpsDst,
                             HBITMAP hbm,
                             PRECTL pwrcSrc,
                             PPOINTL pptlDst,
                             LONG clrFore,
                             LONG clrBack,
                             ULONG fl);

/* WinDrawBitmap() flags */

#define DBM_NORMAL                 0x0000
#define DBM_INVERT                 0x0001
#define DBM_HALFTONE               0x0002
#define DBM_STRETCH                0x0004
#define DBM_IMAGEATTRS             0x0008
#define DBM_XOR                    0x0010



LONG    APIENTRY WinDrawText(HPS hps,
                             LONG cchText,
                             PCH lpchText,
                             PRECTL prcl,
                             LONG clrFore,
                             LONG clrBack,
                             ULONG flCmd);


/*
* WinDrawText()/WinDrawTabbedText() codes:
* From DT_LEFT to DT_EXTERNALLEADING, the codes are designed to be OR'ed with
* SS_TEXT to create variations of the basic text static item.
*/
#define DT_LEFT                    0x00000000
#define DT_QUERYEXTENT             0x00000002
#define DT_UNDERSCORE              0x00000010
#define DT_STRIKEOUT               0x00000020
#define DT_TEXTATTRS               0x00000040
#define DT_EXTERNALLEADING         0x00000080
#define DT_CENTER                  0x00000100
#define DT_RIGHT                   0x00000200
#define DT_TOP                     0x00000000
#define DT_VCENTER                 0x00000400
#define DT_BOTTOM                  0x00000800
#define DT_HALFTONE                0x00001000
#define DT_MNEMONIC                0x00002000
#define DT_WORDBREAK               0x00004000
#define DT_ERASERECT               0x00008000



BOOL APIENTRY WinDrawBorder(HPS hps,
                            PRECTL prcl,
                            LONG cx,
                            LONG cy,
                            LONG clrFore,
                            LONG clrBack,
                            ULONG flCmd);


/* WinDrawBorder() flags */

#define DB_PATCOPY                 0x0000
#define DB_PATINVERT               0x0001
#define DB_DESTINVERT              0x0002
#define DB_AREAMIXMODE             0x0003

#define DB_ROP                     0x0007
#define DB_INTERIOR                0x0008
#define DB_AREAATTRS               0x0010
#define DB_STANDARD                0x0100
#define DB_DLGBORDER               0x0200



/** Resource loading functions */

#ifdef __cplusplus
LONG    APIENTRY WinLoadString(HAB hab,
                               HMODULE hmod,
                               ULONG id,
                               LONG cchMax,
                               PCSZ  pchBuffer);
#else
LONG    APIENTRY WinLoadString(HAB hab,
                               HMODULE hmod,
                               ULONG id,
                               LONG cchMax,
                               PSZ pchBuffer);
#endif
#ifndef INCL_SAADEFS
#ifdef __cplusplus
   LONG    APIENTRY WinLoadMessage(HAB hab,
                                   HMODULE hmod,
                                   ULONG id,
                                   LONG cchMax,
                                   PCSZ  pchBuffer);
#else
   LONG    APIENTRY WinLoadMessage(HAB hab,
                                   HMODULE hmod,
                                   ULONG id,
                                   LONG cchMax,
                                   PSZ pchBuffer);
#endif
   
   
#endif /* !INCL_SAADEFS */

/***************************************************************************/
/****                 Window Manager Subsection part 2                  ****/
#if (defined(INCL_WINWINDOWMGR) || !defined(INCL_NOCOMMON))
   
   BOOL APIENTRY WinSetActiveWindow(HWND hwndDesktop,
                                    HWND hwnd);
   
#endif /* Window Manager COMMON subsection */

#ifdef INCL_WINWINDOWMGR
   /* WM_CREATE structure */
   
   typedef struct _CREATESTRUCT    /* crst */
   {
      PVOID   pPresParams;
      PVOID   pCtlData;
      ULONG   id;
      HWND    hwndInsertBehind;
      HWND    hwndOwner;
      LONG    cy;
      LONG    cx;
      LONG    y;
      LONG    x;
      ULONG   flStyle;
      PSZ     pszText;
      PSZ     pszClass;
      HWND    hwndParent;
   } CREATESTRUCT;
   typedef CREATESTRUCT *PCREATESTRUCT;
   
   /* WinQueryClassInfo() structure */
   
   typedef struct _CLASSINFO    /* clsi */
   {
      ULONG   flClassStyle;
      PFNWP   pfnWindowProc;
      ULONG   cbWindowData;
   } CLASSINFO;
   typedef CLASSINFO *PCLASSINFO;
   
   #ifndef INCL_SAADEFS
      PFNWP  APIENTRY WinSubclassWindow(HWND hwnd,
                                        PFNWP pfnwp);
   #endif /* !INCL_SAADEFS */
   
   LONG    APIENTRY WinQueryClassName(HWND hwnd,
                                      LONG cchMax,
                                      PCH pch);
#ifdef __cplusplus
   BOOL   APIENTRY WinQueryClassInfo(HAB hab,
                                     PCSZ  pszClassName,
                                     PCLASSINFO pClassInfo);
#else
   BOOL   APIENTRY WinQueryClassInfo(HAB hab,
                                     PSZ pszClassName,
                                     PCLASSINFO pClassInfo);
#endif
   
   HWND   APIENTRY WinQueryActiveWindow(HWND hwndDesktop);
   
   #ifndef INCL_SAADEFS
      BOOL   APIENTRY WinIsThreadActive(HAB hab);
   #endif /* !INCL_SAADEFS */
   HWND   APIENTRY WinQuerySysModalWindow(HWND hwndDesktop);
   
   
   BOOL   APIENTRY WinSetSysModalWindow(HWND hwndDesktop,
                                        HWND hwnd);
   
   
   #ifndef INCL_SAADEFS
      USHORT APIENTRY WinQueryWindowUShort(HWND hwnd,
                                           LONG index);
      BOOL   APIENTRY WinSetWindowUShort(HWND hwnd,
                                         LONG index,
                                         USHORT us);
      ULONG  APIENTRY WinQueryWindowULong(HWND hwnd,
                                          LONG index);
      BOOL   APIENTRY WinSetWindowULong(HWND hwnd,
                                        LONG index,
                                        ULONG ul);
      PVOID  APIENTRY WinQueryWindowPtr(HWND hwnd,
                                        LONG index);
      BOOL   APIENTRY WinSetWindowPtr(HWND hwnd,
                                      LONG index,
                                      PVOID p);
      BOOL   APIENTRY WinSetWindowBits(HWND hwnd,
                                       LONG index,
                                       ULONG flData,
                                       ULONG flMask);
      
      /*** Standard WinQueryWindowUShort/ULong() indices *******************/
      #define QWS_USER                   0
      #define QWS_ID                     (-1)
      #define QWS_MIN                    (-1)
      
      #define QWL_USER                   0
      #define QWL_STYLE                  (-2)
      #define QWP_PFNWP                  (-3)
      #define QWL_HMQ                    (-4)
      #define QWL_RESERVED               (-5)
      
      #define QWL_PENDATA                (-7)
      
      #define QWL_BD_ATTR                (-9)
      #define QWL_BD_STAT                (-10)
      #define QWL_KBDLAYER               (-11)
      
      #define QWL_MIN                    (-11)
      
      /*** WC_FRAME WinQueryWindowUShort/ULong() indices *******************/
      #define QWL_HHEAP                  0x0004
      #define QWL_HWNDFOCUSSAVE          0x0018
      #define QWL_DEFBUTTON              0x0040
      #define QWL_PSSCBLK                0x0048
      #define QWL_PFEPBLK                0x004c
      #define QWL_PSTATBLK               0x0050
      
      #define QWS_FLAGS                  0x0008
      #define QWS_RESULT                 0x000a
      #define QWS_XRESTORE               0x000c
      #define QWS_YRESTORE               0x000e
      #define QWS_CXRESTORE              0x0010
      #define QWS_CYRESTORE              0x0012
      #define QWS_XMINIMIZE              0x0014
      #define QWS_YMINIMIZE              0x0016
      
      
      /*** Window enumeration **********************************************/
      typedef LHANDLE HENUM;  /* henum */
      
      HENUM APIENTRY WinBeginEnumWindows(HWND hwnd);
      HWND  APIENTRY WinGetNextWindow(HENUM henum);
      BOOL  APIENTRY WinEndEnumWindows(HENUM henum);
      
   #endif /* !INCL_SAADEFS */
   
   HWND  APIENTRY WinWindowFromPoint(HWND hwnd,
                                     PPOINTL pptl,
                                     BOOL fChildren);
   
   BOOL  APIENTRY WinMapWindowPoints(HWND hwndFrom,
                                     HWND hwndTo,
                                     PPOINTL prgptl,
                                     LONG cwpt);
   
   
   /* More window painting functions */
   
   BOOL  APIENTRY WinValidateRect(HWND hwnd,
                                  PRECTL prcl,
                                  BOOL fIncludeChildren);
   
   BOOL  APIENTRY WinValidateRegion(HWND hwnd,
                                    HRGN hrgn,
                                    BOOL fIncludeChildren);
   
   #ifndef INCL_SAADEFS
      HWND  APIENTRY WinWindowFromDC(HDC hdc);
      
      HDC   APIENTRY WinQueryWindowDC(HWND hwnd);
      
      HPS   APIENTRY WinGetScreenPS(HWND hwndDesktop);
      
      BOOL  APIENTRY WinLockWindowUpdate(HWND hwndDesktop,
                                         HWND hwndLockUpdate);
      
      BOOL  APIENTRY WinLockVisRegions(HWND hwndDesktop,
                                       BOOL fLock);
   #endif /* !INCL_SAADEFS */
   
   BOOL  APIENTRY WinQueryUpdateRect(HWND hwnd,
                                     PRECTL prcl);
   
   LONG APIENTRY WinQueryUpdateRegion(HWND hwnd,
                                      HRGN hrgn);
   
   LONG APIENTRY WinExcludeUpdateRegion(HPS hps,
                                        HWND hwnd);
   
   /* WinQueryClipRegion return codes */
   #define QCRGN_ERROR                 0
   #define QCRGN_OK                    1
   #define QCRGN_NO_CLIP_REGION        2
   
   LONG APIENTRY WinQueryClipRegion(HWND hwnd,
                                    HRGN hrgnClip);
   
   BOOL APIENTRY WinSetClipRegion(HWND hwnd,
                                  HRGN hrgnClip);
   
#endif /* INCL_WINWINDOWMGR */

#if (defined(INCL_WINMESSAGEMGR) || !defined(INCL_NOCOMMON))
   
   /* QMSG structure */
   typedef struct _QMSG    /* qmsg */
   {
      HWND    hwnd;
      ULONG   msg;
      MPARAM  mp1;
      MPARAM  mp2;
      ULONG   time;
      POINTL  ptl;
      ULONG   reserved;
   } QMSG;
   typedef QMSG *PQMSG;
   
   /*** Standard Window Messages *******************************************/
   #define WM_NULL                    0x0000
   #define WM_CREATE                  0x0001
   #define WM_DESTROY                 0x0002
   
   #ifndef INCL_SAADEFS
   #endif /* !INCL_SAADEFS */
   
   #define WM_ENABLE                  0x0004
   #define WM_SHOW                    0x0005
   #define WM_MOVE                    0x0006
   #define WM_SIZE                    0x0007
   #define WM_ADJUSTWINDOWPOS         0x0008
   
   #define WM_CALCVALIDRECTS          0x0009
   
   #define WM_SETWINDOWPARAMS         0x000a
   #define WM_QUERYWINDOWPARAMS       0x000b
   #define WM_HITTEST                 0x000c
   #define WM_ACTIVATE                0x000d
   #define WM_SETFOCUS                0x000f
   #define WM_SETSELECTION            0x0010
   
   /* language support Winproc */
   #define WM_PPAINT                  0x0011
   #define WM_PSETFOCUS               0x0012
   #define WM_PSYSCOLORCHANGE         0x0013
   #define WM_PSIZE                   0x0014
   #define WM_PACTIVATE               0x0015
   #define WM_PCONTROL                0x0016
   
   #define WM_COMMAND                 0x0020
   #define WM_SYSCOMMAND              0x0021
   #define WM_HELP                    0x0022
   #define WM_PAINT                   0x0023
   
   #ifndef INCL_SAADEFS
      #define WM_TIMER                0x0024
      #define WM_SEM1                 0x0025
      #define WM_SEM2                 0x0026
      #define WM_SEM3                 0x0027
      #define WM_SEM4                 0x0028
   #endif /* !INCL_SAADEFS */
   
   #define WM_CLOSE                   0x0029
   #define WM_QUIT                    0x002a
   #define WM_SYSCOLORCHANGE          0x002b
   #define WM_SYSVALUECHANGED         0x002d
   #define WM_APPTERMINATENOTIFY      0x002e
   #define WM_PRESPARAMCHANGED        0x002f
   
   /* Control notification messages */
   #define WM_CONTROL                 0x0030
   #define WM_VSCROLL                 0x0031
   #define WM_HSCROLL                 0x0032
   #define WM_INITMENU                0x0033
   #define WM_MENUSELECT              0x0034
   #define WM_MENUEND                 0x0035
   #define WM_DRAWITEM                0x0036
   #define WM_MEASUREITEM             0x0037
   #define WM_CONTROLPOINTER          0x0038
   #define WM_QUERYDLGCODE            0x003a
   #define WM_INITDLG                 0x003b
   #define WM_SUBSTITUTESTRING        0x003c
   #define WM_MATCHMNEMONIC           0x003d
   #define WM_SAVEAPPLICATION         0x003e
   
   #define WM_SEMANTICEVENT           0x0490
   
   
   /*************************************************************************
   * Reserve a range of messages for help manager.  This range includes
   * public messages, defined below, and private ones, which need to be
   * reserved here to prevent clashing with application messages
   *************************************************************************/
   
   #define WM_HELPBASE                0x0F00 /* Start of msgs for help manager   */
   #define WM_HELPTOP                 0x0FFF /* End of msgs for help manager     */
   
   #define WM_USER                    0x1000
   
   /*** WM_COMMAND msg source codes ****************************************/
   #define CMDSRC_PUSHBUTTON      1
   #define CMDSRC_MENU            2
   #define CMDSRC_ACCELERATOR     3
   #define CMDSRC_FONTDLG         4
   #define CMDSRC_FILEDLG         5
   #define CMDSRC_PRINTDLG        6
   #define CMDSRC_COLORDLG        7
   #define CMDSRC_OTHER           0
   
   #pragma pack(1) /* force structure alignment packing */
   
   /*
   * The following structure and macro are used to access the
   * WM_COMMAND, WM_HELP, and WM_SYSCOMMAND message parameters:
   */
   typedef struct _COMMANDMSG    /* commandmsg */
   {
      USHORT  cmd;              /* mp1 */
      USHORT  unused;
      USHORT  source;           /* mp2 */
      USHORT  fMouse;
   } CMDMSG;
   typedef CMDMSG *PCMDMSG;
   
   #define COMMANDMSG(pmsg) \
   ((PCMDMSG)((PBYTE)pmsg + sizeof(MPARAM) ))
   
   #pragma pack()  /* restore packing to default */
   
   /*** The following structure is used by the WinQueryQueueInfo() routine */
   typedef struct _MQINFO    /* mqi */
   {
      ULONG   cb;
      PID     pid;
      TID     tid;
      ULONG   cmsgs;
      PVOID   pReserved;
   } MQINFO;
   typedef MQINFO *PMQINFO;
   
   MRESULT APIENTRY WinSendMsg(HWND hwnd,
                               ULONG msg,
                               MPARAM mp1,
                               MPARAM mp2);
   
   HMQ     APIENTRY WinCreateMsgQueue(HAB hab,
                                      LONG cmsg);
   
   BOOL    APIENTRY WinDestroyMsgQueue(HMQ hmq);
   
   BOOL    APIENTRY WinQueryQueueInfo(HMQ hmq,
                                      PMQINFO pmqi,
                                      ULONG cbCopy);
   
   #ifndef INCL_SAADEFS
      BOOL    APIENTRY WinCancelShutdown(HMQ hmq,
                                         BOOL fCancelAlways);
   #endif /* INCL_SAADEFS */
   
   BOOL    APIENTRY WinGetMsg(HAB hab,
                              PQMSG pqmsg,
                              HWND hwndFilter,
                              ULONG msgFilterFirst,
                              ULONG msgFilterLast);
   
   BOOL    APIENTRY WinPeekMsg(HAB hab,
                               PQMSG pqmsg,
                               HWND hwndFilter,
                               ULONG msgFilterFirst,
                               ULONG msgFilterLast,
                               ULONG fl);
   
   MRESULT APIENTRY WinDispatchMsg(HAB hab,
                                   PQMSG pqmsg);
   
   BOOL    APIENTRY WinPostMsg(HWND hwnd,
                               ULONG msg,
                               MPARAM mp1,
                               MPARAM mp2);
   
   BOOL    APIENTRY WinRegisterUserMsg(HAB hab,
                                       ULONG msgid,
                                       LONG datatype1,
                                       LONG dir1,
                                       LONG datatype2,
                                       LONG dir2,
                                       LONG datatyper);
   
   BOOL    APIENTRY WinRegisterUserDatatype(HAB hab,
                                            LONG datatype,
                                            LONG count,
                                            PLONG types);
#ifdef __cplusplus
   BOOL    APIENTRY WinSetMsgMode(HAB hab,
                                  PCSZ  classname,
                                  LONG control);
#else
   BOOL    APIENTRY WinSetMsgMode(HAB hab,
                                  PSZ classname,
                                  LONG control);
#endif
   
   BOOL    APIENTRY WinSetSynchroMode(HAB hab,
                                      LONG mode);
   
   /*** WinPeekMsg() constants *********************************************/
   #define PM_REMOVE                  0x0001
   #define PM_NOREMOVE                0x0000
   
   /*** WinRegisterUserDatatype datatypes defined in separate file *********/
   #ifdef INCL_WINTYPES
      #include <pmtypes.h>
   #endif /*INCL_WINTYPES*/
   
   /*** WinRegisterUserMsg direction codes *********************************/
   #define RUM_IN                     1
   #define RUM_OUT                    2
   #define RUM_INOUT                  3
   
   /*** WinSetMsgMode constants *******************************************/
   #define SMD_DELAYED                0x0001
   #define SMD_IMMEDIATE              0x0002
   
   /*** WinSetSynchroMode constants ***************************************/
   #define SSM_SYNCHRONOUS            0x0001
   #define SSM_ASYNCHRONOUS           0x0002
   #define SSM_MIXED                  0x0003
   
#endif /* WINMESSAGEMGR || !INCL_NOCOMMON       */

#ifdef INCL_WINMESSAGEMGR
   
   /*** WM_CALCVALIDRECTS return flags *************************************/
   #define CVR_ALIGNLEFT               0x0001
   #define CVR_ALIGNBOTTOM             0x0002
   #define CVR_ALIGNRIGHT              0x0004
   #define CVR_ALIGNTOP                0x0008
   #define CVR_REDRAW                  0x0010
   
   /*** WM_HITTEST return codes ********************************************/
   #define HT_NORMAL                   0
   #define HT_TRANSPARENT              (-1)
   #define HT_DISCARD                  (-2)
   #define HT_ERROR                    (-3)
   
   /*** WM_SET/QUERYWINDOWPARAMS structures and flags **********************/
   typedef struct _WNDPARAMS    /* wprm */
   {
      ULONG   fsStatus;
      ULONG   cchText;
      PSZ     pszText;
      ULONG   cbPresParams;
      PVOID   pPresParams;
      ULONG   cbCtlData;
      PVOID   pCtlData;
   } WNDPARAMS;
   typedef WNDPARAMS *PWNDPARAMS;
   
   #define WPM_TEXT                   0x0001
   #define WPM_CTLDATA                0x0002
   #define WPM_PRESPARAMS             0x0004
   #define WPM_CCHTEXT                0x0008
   #define WPM_CBCTLDATA              0x0010
   #define WPM_CBPRESPARAMS           0x0020
   
   #ifndef INCL_SAADEFS
      BOOL    APIENTRY WinInSendMsg(HAB hab);
   #endif /* INCL_SAADEFS */
   
   BOOL    APIENTRY WinBroadcastMsg(HWND hwnd,
                                    ULONG msg,
                                    MPARAM mp1,
                                    MPARAM mp2,
                                    ULONG rgf);
   
   /*** WinBroadcastMsg codes **********************************************/
   #define BMSG_POST                  0x0000
   #define BMSG_SEND                  0x0001
   #define BMSG_POSTQUEUE             0x0002
   #define BMSG_DESCENDANTS           0x0004
   #define BMSG_FRAMEONLY             0x0008
   
   BOOL  APIENTRY WinWaitMsg(HAB hab,
                             ULONG msgFirst,
                             ULONG msgLast);
   
   #ifndef INCL_SAADEFS
      ULONG APIENTRY WinQueryQueueStatus(HWND hwndDesktop);
      
      /*** WinQueryQueueStatus() constants *********************************/
      #define QS_KEY                     0x0001
      #define QS_MOUSEBUTTON             0x0002
      #define QS_MOUSEMOVE               0x0004
      #define QS_MOUSE                   0x0006   /* QS_MOUSEMOVE|QS_MOUSEBUTTON */
      #define QS_TIMER                   0x0008
      #define QS_PAINT                   0x0010
      #define QS_POSTMSG                 0x0020
      #define QS_SEM1                    0x0040
      #define QS_SEM2                    0x0080
      #define QS_SEM3                    0x0100
      #define QS_SEM4                    0x0200
      #define QS_SENDMSG                 0x0400
      #define QS_MSGINPUT                0x0800
      
      BOOL  APIENTRY WinQueryMsgPos(HAB hab,
                                    PPOINTL pptl);
      
      ULONG APIENTRY WinQueryMsgTime(HAB hab);
      
      #ifndef INCL_DOSSEMAPHORES
         #ifndef __HEV__
            #define __HEV__
            typedef ULONG HEV;    /* hev */
         #endif
         typedef ULONG HMTX;   /* hmtx */
         typedef ULONG HMUX;   /* hmux */
      #endif
      
      APIRET APIENTRY WinWaitEventSem(HEV hev,
                                      ULONG ulTimeout);
      
      APIRET APIENTRY WinRequestMutexSem(HMTX hmtx,
                                         ULONG ulTimeout);
      
      APIRET APIENTRY WinWaitMuxWaitSem(HMUX hmux,
                                        ULONG ulTimeout,
                                        PULONG pulUser);
      
      
   #endif /* !INCL_SAADEFS */
   
   BOOL  APIENTRY WinPostQueueMsg(HMQ hmq,
                                  ULONG msg,
                                  MPARAM mp1,
                                  MPARAM mp2);
   
   /*** WinSetMsgInterest()/WinSetClassMsgInterest() constants *************/
   #define SMIM_ALL                   0x0EFF
   #define SMI_NOINTEREST             0x0001
   #define SMI_INTEREST               0x0002
   #define SMI_RESET                  0x0004
   #define SMI_AUTODISPATCH           0x0008
   
   BOOL  APIENTRY WinSetMsgInterest(HWND hwnd,
                                    ULONG msg_class,
                                    LONG control);
   
#ifdef __cplusplus
   BOOL  APIENTRY WinSetClassMsgInterest(HAB hab,
                                         PCSZ  pszClassName,
                                         ULONG msg_class,
                                         LONG control);
#else
   BOOL  APIENTRY WinSetClassMsgInterest(HAB hab,
                                         PSZ pszClassName,
                                         ULONG msg_class,
                                         LONG control);
#endif
   
#endif /* INCL_WINMESSAGEMGR */

/*** Keyboard and mouse ****************************************************/

#if (defined(INCL_WININPUT) || !defined(INCL_NOCOMMON))
   
   /*** Keyboard and mouse input COMMON subsection *************************/
   BOOL  APIENTRY WinSetFocus(HWND hwndDesktop,
                              HWND hwndSetFocus);
   
   BOOL  APIENTRY WinFocusChange(HWND hwndDesktop,
                                 HWND hwndSetFocus,
                                 ULONG flFocusChange);
   
   BOOL  EXPENTRY WinLockupSystem(HAB hab);
   
   BOOL  EXPENTRY WinUnlockSystem(HAB hab, PSZ pszPassword);
   
   #define FC_NOSETFOCUS              0x0001
   #define FC_NOBRINGTOTOP            FC_NOSETFOCUS
   #define FC_NOLOSEFOCUS             0x0002
   #define FC_NOBRINGTOPFIRSTWINDOW   FC_NOLOSEFOCUS
   #define FC_NOSETACTIVE             0x0004
   #define FC_NOLOSEACTIVE            0x0008
   #define FC_NOSETSELECTION          0x0010
   #define FC_NOLOSESELECTION         0x0020
   
   #define QFC_NEXTINCHAIN            0x0001
   #define QFC_ACTIVE                 0x0002
   #define QFC_FRAME                  0x0003
   #define QFC_SELECTACTIVE           0x0004
   #define QFC_PARTOFCHAIN            0x0005
   
#endif  /* Keyboard and mouse input COMMON subsection */

/*
** @86823; added WM_VRNDISABLED, WM_VRNENABLED,
**               WinSetVisibleRegionNotify, WinQueryVisibleRegion
*/
#define WM_VRNDISABLED             0x007e
#define WM_VRNENABLED              0x007f

BOOL APIENTRY WinSetVisibleRegionNotify( HWND hwnd,
                                        BOOL fEnable);

ULONG APIENTRY WinQueryVisibleRegion( HWND hwnd,
                                     HRGN hrgn);


#ifdef INCL_WININPUT
   #ifndef INCL_SAADEFS
      BOOL  APIENTRY WinSetCapture(HWND hwndDesktop,
                                   HWND hwnd);
   #endif /* !INCL_SAADEFS */
   
   HWND  APIENTRY WinQueryCapture(HWND hwndDesktop);
   
   BOOL  APIENTRY WinCheckInput(HAB hab);
   
   /*** Mouse input messages ***********************************************/
   #ifndef INCL_SAADEFS
      #define WM_MOUSEFIRST           0x0070
      #define WM_MOUSELAST            0x0079
      #define WM_BUTTONCLICKFIRST     0x0071
      #define WM_BUTTONCLICKLAST      0x0079
   #endif /* !INCL_SAADEFS */
   #define WM_MOUSEMOVE               0x0070
   
   #define WM_BUTTON1DOWN             0x0071
   
   #define WM_BUTTON1UP               0x0072
   #define WM_BUTTON1DBLCLK           0x0073
   #define WM_BUTTON2DOWN             0x0074
   #define WM_BUTTON2UP               0x0075
   #define WM_BUTTON2DBLCLK           0x0076
   #ifndef INCL_SAADEFS
      #define WM_BUTTON3DOWN          0x0077
      #define WM_BUTTON3UP            0x0078
      #define WM_BUTTON3DBLCLK        0x0079
      
      #define WM_MOUSEMAP             0x007D
      
      #define WM_EXTMOUSEFIRST        0x0410
      #define WM_EXTMOUSELAST         0x0419
      
      #define WM_CHORD                0x0410
      #define WM_BUTTON1MOTIONSTART   0x0411
      #define WM_BUTTON1MOTIONEND     0x0412
      #define WM_BUTTON1CLICK         0x0413
      #define WM_BUTTON2MOTIONSTART   0x0414
      #define WM_BUTTON2MOTIONEND     0x0415
      #define WM_BUTTON2CLICK         0x0416
      #define WM_BUTTON3MOTIONSTART   0x0417
      #define WM_BUTTON3MOTIONEND     0x0418
      #define WM_BUTTON3CLICK         0x0419
      
      /* Messages 0x041A - 0x041F are reserved */
      
      #define WM_MOUSETRANSLATEFIRST  0x0420
      #define WM_MOUSETRANSLATELAST   0x0428
      
      #define WM_BEGINDRAG            0x0420
      #define WM_ENDDRAG              0x0421
      #define WM_SINGLESELECT         0x0422
      #define WM_OPEN                 0x0423
      #define WM_CONTEXTMENU          0x0424
      #define WM_CONTEXTHELP          0x0425
      #define WM_TEXTEDIT             0x0426
      #define WM_BEGINSELECT          0x0427
      #define WM_ENDSELECT            0x0428
      #define WM_PICKUP               0x0429
      
      #define WM_PENFIRST             0x04C0
      #define WM_PENLAST              0x04FF
      #define WM_MMPMFIRST            0x0500
      #define WM_MMPMLAST             0x05FF
      #define WM_STDDLGFIRST          0x0600
      #define WM_STDDLGLAST           0x065F
      
      /* Messages 0x0BD0 - 0x0BFF are reserved */
      /* for bidirectional language support.   */
      
      #define  WM_BIDI_FIRST           0x0BD0
      #define  WM_BIDI_LAST            0x0BFF
      
   #endif /* !INCL_SAADEFS */
   
   HWND  APIENTRY WinQueryFocus(HWND hwndDesktop);
   
   
   /*** Key/Character input messages ***************************************/
   #define WM_CHAR                    0x007a
   #define WM_VIOCHAR                 0x007b
   
   /* WM_CHAR fs field bits */
   #define KC_NONE                    0x0000     /* Reserved */
   #define KC_CHAR                    0x0001
   #define KC_VIRTUALKEY              0x0002
   #define KC_SCANCODE                0x0004
   
   #define KC_SHIFT                   0x0008
   #define KC_CTRL                    0x0010
   #define KC_ALT                     0x0020
   #define KC_KEYUP                   0x0040
   #define KC_PREVDOWN                0x0080
   #define KC_LONEKEY                 0x0100
   #define KC_DEADKEY                 0x0200
   #define KC_COMPOSITE               0x0400
   #define KC_INVALIDCOMP             0x0800
   
   #ifndef INCL_SAADEFS
      #define KC_TOGGLE               0x1000
      #define KC_INVALIDCHAR          0x2000
      #define KC_DBCSRSRVD1           0x4000
      #define KC_DBCSRSRVD2           0x8000
   #endif /* !INCL_SAADEFS */
   
   #pragma pack(1) /* force structure alignment packing */
   
   /*
   * The following structure and macro are used to access the
   * WM_MOUSEMOVE, and WM_BUTTON message parameters
   */
   typedef struct _MOUSEMSG    /* mousemsg */
   {
      SHORT   x;             /* mp1      */
      SHORT   y;
      USHORT  codeHitTest;   /* mp2      */
      USHORT  fsInp;         /* input flags */
   } MSEMSG;
   typedef MSEMSG *PMSEMSG;
   
   #define MOUSEMSG(pmsg) \
   ((PMSEMSG)((PBYTE)pmsg + sizeof(MPARAM) ))
   
   typedef struct _CHARMSG    /* charmsg */
   {
      USHORT  fs;           /* mp1     */
      UCHAR   cRepeat;
      UCHAR   scancode;
      USHORT  chr;          /* mp2     */
      USHORT  vkey;
   } CHRMSG;
   typedef CHRMSG *PCHRMSG;
   
   #define CHARMSG(pmsg) \
   ((PCHRMSG)((PBYTE)pmsg + sizeof(MPARAM) ))
   
   #pragma pack()  /* restore packing to default */
   
   #define INP_NONE                   0x0000
   #define INP_KBD                    0x0001
   #define INP_MULT                   0x0002
   #define INP_RES2                   0x0004
   #define INP_SHIFT                  0x0008
   #define INP_CTRL                   0x0010
   #define INP_ALT                    0x0020
   #define INP_RES3                   0x0040
   #define INP_RES4                   0x0080
   #define INP_IGNORE                 0xFFFF
   
   /*** Virtual key values *************************************************/
   #define VK_BUTTON1                 0x01
   #define VK_BUTTON2                 0x02
   #define VK_BUTTON3                 0x03
   #define VK_BREAK                   0x04
   #define VK_BACKSPACE               0x05
   #define VK_TAB                     0x06
   #define VK_BACKTAB                 0x07
   #define VK_NEWLINE                 0x08
   #define VK_SHIFT                   0x09
   #define VK_CTRL                    0x0A
   #define VK_ALT                     0x0B
   #define VK_ALTGRAF                 0x0C
   #define VK_PAUSE                   0x0D
   #define VK_CAPSLOCK                0x0E
   #define VK_ESC                     0x0F
   #define VK_SPACE                   0x10
   #define VK_PAGEUP                  0x11
   #define VK_PAGEDOWN                0x12
   #define VK_END                     0x13
   #define VK_HOME                    0x14
   #define VK_LEFT                    0x15
   #define VK_UP                      0x16
   #define VK_RIGHT                   0x17
   #define VK_DOWN                    0x18
   #define VK_PRINTSCRN               0x19
   #define VK_INSERT                  0x1A
   #define VK_DELETE                  0x1B
   #define VK_SCRLLOCK                0x1C
   #define VK_NUMLOCK                 0x1D
   #define VK_ENTER                   0x1E
   #define VK_SYSRQ                   0x1F
   #define VK_F1                      0x20
   #define VK_F2                      0x21
   #define VK_F3                      0x22
   #define VK_F4                      0x23
   #define VK_F5                      0x24
   #define VK_F6                      0x25
   #define VK_F7                      0x26
   #define VK_F8                      0x27
   #define VK_F9                      0x28
   #define VK_F10                     0x29
   #define VK_F11                     0x2A
   #define VK_F12                     0x2B
   #define VK_F13                     0x2C
   #define VK_F14                     0x2D
   #define VK_F15                     0x2E
   #define VK_F16                     0x2F
   #define VK_F17                     0x30
   #define VK_F18                     0x31
   #define VK_F19                     0x32
   #define VK_F20                     0x33
   #define VK_F21                     0x34
   #define VK_F22                     0x35
   #define VK_F23                     0x36
   #define VK_F24                     0x37
   #define VK_ENDDRAG                 0x38
   #define VK_CLEAR                   0x39
   #define VK_EREOF                   0x3A
   #define VK_PA1                     0x3B
   #define VK_ATTN                    0x3C
   #define VK_CRSEL                   0x3D
   #define VK_EXSEL                   0x3E
   #define VK_COPY                    0x3F
   #define VK_BLK1                    0x40
   #define VK_BLK2                    0x41
   
   
   #define VK_MENU                    VK_F10
   #ifdef INCL_NLS
      #define VK_DBCSFIRST            0x0080
      #define VK_DBCSLAST             0x00ff
      
      /* VK_ vluaes 0xE0 - 0xFF are reserved   */
      /* for bidirectional language support.   */
      
      #define VK_BIDI_FIRST           0xE0
      #define VK_BIDI_LAST            0xFF
      
   #endif /* INCL_NLS */
   
   #define VK_USERFIRST               0x0100
   #define VK_USERLAST                0x01ff
   
   #ifndef INCL_SAADEFS
      
      LONG APIENTRY WinGetKeyState(HWND hwndDesktop,
                                   LONG vkey);
      LONG APIENTRY WinGetPhysKeyState(HWND hwndDesktop,
                                       LONG sc);
      BOOL  APIENTRY WinEnablePhysInput(HWND hwndDesktop,
                                        BOOL fEnable);
      BOOL  APIENTRY WinIsPhysInputEnabled(HWND hwndDesktop);
      BOOL  APIENTRY WinSetKeyboardStateTable(HWND hwndDesktop,
                                              PBYTE pKeyStateTable,
                                              BOOL fSet);
      
      
      /* Journal Notification messages  */
      #define WM_JOURNALNOTIFY           0x007c
      
      /*** Define the valid commands (lParm1) for journal notify message */
      #define JRN_QUEUESTATUS            0x00000001L
      #define JRN_PHYSKEYSTATE           0x00000002L
      
   #endif /* !INCL_SAADEFS */
#endif /* INCL_WININPUT */

/**** Dialog Manager */

#if (defined(INCL_WINDIALOGS) || !defined(INCL_NOCOMMON))
   /**** Dialog Manager COMMON subsection */
   
   BOOL    APIENTRY WinGetDlgMsg(HWND hwndDlg,
                                 PQMSG pqmsg);
   
   HWND    APIENTRY WinLoadDlg(HWND hwndParent,
                               HWND hwndOwner,
                               PFNWP pfnDlgProc,
                               HMODULE hmod,
                               ULONG idDlg,
                               PVOID pCreateParams);
   ULONG  APIENTRY WinDlgBox(HWND hwndParent,
                             HWND hwndOwner,
                             PFNWP pfnDlgProc,
                             HMODULE hmod,
                             ULONG idDlg,
                             PVOID pCreateParams);
   
   BOOL    APIENTRY WinDismissDlg(HWND hwndDlg,
                                  ULONG usResult);
   
   BOOL    APIENTRY WinQueryDlgItemShort(HWND hwndDlg,
                                         ULONG idItem,
                                         PSHORT pResult,
                                         BOOL fSigned);
   BOOL    APIENTRY WinSetDlgItemShort(HWND hwndDlg,
                                       ULONG idItem,
                                       USHORT usValue,
                                       BOOL fSigned);
#ifdef __cplusplus
   BOOL    APIENTRY WinSetDlgItemText(HWND hwndDlg,
                                      ULONG idItem,
                                      PCSZ  pszText);
#else
   BOOL    APIENTRY WinSetDlgItemText(HWND hwndDlg,
                                      ULONG idItem,
                                      PSZ pszText);
#endif
#ifdef __cplusplus
   ULONG  APIENTRY WinQueryDlgItemText(HWND hwndDlg,
                                       ULONG idItem,
                                       LONG cchBufferMax,
                                       PCSZ  pchBuffer);
#else
   ULONG  APIENTRY WinQueryDlgItemText(HWND hwndDlg,
                                       ULONG idItem,
                                       LONG cchBufferMax,
                                       PSZ pchBuffer);
#endif
   LONG   APIENTRY WinQueryDlgItemTextLength(HWND hwndDlg,
                                             ULONG idItem);
   
   MRESULT APIENTRY WinDefDlgProc(HWND hwndDlg,
                                  ULONG msg,
                                  MPARAM mp1,
                                  MPARAM mp2);
   
   /* Special item IDs */
   
   #ifndef INCL_SAADEFS
      #define DID_OK      1
      #define DID_CANCEL  2
      #define DID_ERROR   0xffff
   #endif /* !INCL_SAADEFS */
   
   BOOL APIENTRY WinAlarm(HWND hwndDesktop,
                          ULONG rgfType);
   
   /* WinAlarm Codes */
   
   #define WA_WARNING                 0
   #define WA_NOTE                    1
   #define WA_ERROR                   2
   #define WA_CWINALARMS             13     /* count of valid alarms     53304*/
   
   
#ifdef __cplusplus
   ULONG APIENTRY WinMessageBox(HWND hwndParent,
                                HWND hwndOwner,
                                PCSZ  pszText,
                                PCSZ  pszCaption,
                                ULONG idWindow,
                                ULONG flStyle);
#else
   ULONG APIENTRY WinMessageBox(HWND hwndParent,
                                HWND hwndOwner,
                                PSZ pszText,
                                PSZ pszCaption,
                                ULONG idWindow,
                                ULONG flStyle);
#endif
   
   
   
   
   #define MAX_MBDTEXT               70
   
   /* NOINC */
   #pragma pack(4)
   
   typedef struct _MB2D
   {
      CHAR   achText[MAX_MBDTEXT + 1]; /* Text of the button. eg. "~Cancel"    */
      ULONG  idButton;                  /* Button ID returned when user chooses */
      /*  button                              */
      LONG   flStyle;                   /* Button style or'ed with internal     */
      /*  styles                              */
   } MB2D;
   
   typedef MB2D * PMB2D;
   
   typedef struct _MB2INFO
   {
      ULONG    cb;                  /* Size of fixed part of structure          */
      HPOINTER hIcon;               /* Icon handle                              */
      ULONG    cButtons;            /* Number of buttons                        */
      ULONG    flStyle;             /* Icon style flags (MB_ICONQUESTION, etc...)*/
      HWND     hwndNotify;          /* Reserved                                 */
      MB2D     mb2d[1];             /* Array of button definitions              */
   } MB2INFO;
   
   typedef MB2INFO * PMB2INFO;
   
   #pragma pack()
   
   /************************************************************************/
   /* WinMessageBox2                                                       */
   /*                                                                      */
   /* Parameters: HWND   hwndParent   - handle of the parent window.       */
   /*             HWND   hwndOwner    - handle of the owner window.        */
   /*             PSZ    pszText      - message text.                      */
   /*             PSZ    pszCaption   - title of the message box.          */
   /*             ULONG  idWindow     - Message box id                     */
   /*             PMB2INFO pmb2info   - pointer to button/icon info        */
   /************************************************************************/
   ULONG  EXPENTRY  WinMessageBox2(HWND     hwndParent,
   HWND     hwndOwner,
   PSZ      pszText,
   PSZ      pszCaption,
   ULONG    idWindow,
   PMB2INFO pmb2info);
   
   /* INC */
   
   /* Message box types */
   
   #define MB_OK                      0x0000
   #define MB_OKCANCEL                0x0001
   #define MB_RETRYCANCEL             0x0002
   #define MB_ABORTRETRYIGNORE        0x0003
   #define MB_YESNO                   0x0004
   #define MB_YESNOCANCEL             0x0005
   #define MB_CANCEL                  0x0006
   #define MB_ENTER                   0x0007
   #define MB_ENTERCANCEL             0x0008
   
   #define MB_NOICON                  0x0000
   #define MB_CUANOTIFICATION         0x0000
   #define MB_ICONQUESTION            0x0010
   #define MB_ICONEXCLAMATION         0x0020
   #define MB_CUAWARNING              0x0020
   #define MB_ICONASTERISK            0x0030
   #define MB_ICONHAND                0x0040
   #define MB_CUACRITICAL             0x0040
   #define MB_QUERY                   MB_ICONQUESTION
   #define MB_WARNING                 MB_CUAWARNING
   #define MB_INFORMATION             MB_ICONASTERISK
   #define MB_CRITICAL                MB_CUACRITICAL
   #define MB_ERROR                   MB_CRITICAL
   #define MB_CUSTOMICON              0x0080
   
   #define MB_DEFBUTTON1              0x0000
   #define MB_DEFBUTTON2              0x0100
   #define MB_DEFBUTTON3              0x0200
   
   #define MB_APPLMODAL               0x0000
   #define MB_SYSTEMMODAL             0x1000
   #define MB_HELP                    0x2000
   #define MB_MOVEABLE                0x4000
   #define MB_NONMODAL                0x8000
   
   
   /* Message box return codes */
   
   #define MBID_OK                    1
   #define MBID_CANCEL                2
   #define MBID_ABORT                 3
   #define MBID_RETRY                 4
   #define MBID_IGNORE                5
   #define MBID_YES                   6
   #define MBID_NO                    7
   #define MBID_HELP                  8
   #define MBID_ENTER                 9
   #define MBID_ERROR                 0xffff
   
   
   /* Useful macros */
   
   /***************************************************************************
   * ULONG/USHORT WinCheckButton(HWND hwndDlg, USHORT id, USHORT checkState)
   *    sets the check state of button #id.
   *    returns the previous check state.
   ***************************************************************************/
   #define WinCheckButton(hwndDlg, id, usCheckState) \
   ((ULONG)WinSendDlgItemMsg(hwndDlg, id, BM_SETCHECK, \
   MPFROMSHORT(usCheckState), (MPARAM)NULL))
   
   /***************************************************************************
   * ULONG/USHORT WinQueryButtonCheckstate(HWND hwndDlg, USHORT id)
   *    returns the check state of buttin #id.
   *
   ***************************************************************************/
   #define WinQueryButtonCheckstate(hwndDlg, id) \
   ((ULONG)WinSendDlgItemMsg(hwndDlg, id, BM_QUERYCHECK, \
   (MPARAM)NULL, (MPARAM)NULL))
   
   /***************************************************************************
   * BOOL WinEnableControl(HWND hwndDlg, USHORT id, BOOL fEnable)
   *    sets the enable state of button #id.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinEnableControl(hwndDlg, id, fEnable) \
   WinEnableWindow(WinWindowFromID(hwndDlg, id), fEnable)
   
   /***************************************************************************
   * BOOL WinShowControl(HWND hwndDlg, USHORT id, BOOL fShow)
   *    sets the show state of control #id.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinShowControl(hwndDlg, id, fShow) \
   WinShowWindow(WinWindowFromID(hwndDlg, id), fShow)
   
   /***************************************************************************
   * BOOL WinIsControlEnabled(HWND hwndDlg, ULONG id)
   *    returns the enable state of control #id.
   *
   ***************************************************************************/
   #define WinIsControlEnabled(hwndDlg, id) \
   ((BOOL)WinIsWindowEnabled(WinWindowFromID(hwndDlg, id)))
   
#endif /* Dialog Manager COMMON subsection */


#ifdef INCL_WINDIALOGS
   
   /* Dialog codes: returned by WM_QUERYDLGCODE msg     */
   
   #define DLGC_ENTRYFIELD    0x0001  /* Entry field item understands EM_SETSEL) */
   #define DLGC_BUTTON        0x0002  /* Button item                             */
   #define DLGC_RADIOBUTTON   0x0004  /* Radio button                            */
   #define DLGC_STATIC        0x0008  /* Static item                             */
   #define DLGC_DEFAULT       0x0010  /* Default push button                     */
   #define DLGC_PUSHBUTTON    0x0020  /* Normal (Non-default) push button        */
   #define DLGC_CHECKBOX      0x0040  /* Check box button control                */
   #define DLGC_SCROLLBAR     0x0080  /* Scroll bar                              */
   #define DLGC_MENU          0x0100  /* Menu                                    */
   #define DLGC_TABONCLICK    0x0200
   #define DLGC_MLE           0x0400  /* Multiple Line Entry                     */
   
   
   ULONG  APIENTRY WinProcessDlg(HWND hwndDlg);
   MRESULT APIENTRY WinSendDlgItemMsg(HWND hwndDlg,
                                      ULONG idItem,
                                      ULONG msg,
                                      MPARAM mp1,
                                      MPARAM mp2);
   BOOL    APIENTRY WinMapDlgPoints(HWND hwndDlg,
                                    PPOINTL prgwptl,
                                    ULONG cwpt,
                                    BOOL fCalcWindowCoords);
   HWND    APIENTRY WinEnumDlgItem(HWND hwndDlg,
                                   HWND hwnd,
                                   ULONG code);
#ifdef __cplusplus
   LONG   APIENTRY WinSubstituteStrings(HWND hwnd,
                                        PCSZ  pszSrc,
                                        LONG cchDstMax,
                                        PCSZ  pszDst);
#else
   LONG   APIENTRY WinSubstituteStrings(HWND hwnd,
                                        PSZ pszSrc,
                                        LONG cchDstMax,
                                        PSZ pszDst);
#endif
   
   
   /* WinEnumDlgItem() constants */
   
   #define EDI_FIRSTTABITEM           0
   #define EDI_LASTTABITEM            1
   #define EDI_NEXTTABITEM            2
   #define EDI_PREVTABITEM            3
   #define EDI_FIRSTGROUPITEM         4
   #define EDI_LASTGROUPITEM          5
   #define EDI_NEXTGROUPITEM          6
   #define EDI_PREVGROUPITEM          7
   
   /*** Dialog template definitions */
   
   /*** Variable-sized dialog template items: ******************************/
   
   #pragma pack(2) /* force structure alignment packing */
   
   /*************************************************************************
   *
   *  We pack the structures _DLGTITEM and _DLGTEMPLATE so that the
   *  structures are identical in the 32-bit and 16-bit worlds. We have
   *  to do this because is has been documented that one can pass a
   *  pointer to 'the binary resource format' when calling WinCreateDlg.
   *
   *************************************************************************/
   
   /* Binary resources may be bound into application executables or */
   /* passed as a parameter when creating a window or dialog        */
   /* Currently they must be the same for both 16-bit and 32-bit    */
   /* so we pack the structures.                                    */
   typedef struct _DLGTITEM    /* dlgti */
   {
      USHORT  fsItemStatus;
      USHORT  cChildren;
      USHORT  cchClassName;
      USHORT  offClassName;
      USHORT  cchText;
      USHORT  offText;
      ULONG   flStyle;
      SHORT   x;
      SHORT   y;
      SHORT   cx;
      SHORT   cy;
      USHORT  id;
      USHORT  offPresParams;
      USHORT  offCtlData;
   } DLGTITEM;
   typedef DLGTITEM *PDLGTITEM;
   
   /*** Dialog Template structure ******************************************/
   
   /* Binary resources may be bound into application executables or */
   /* passed as a parameter when creating a window or dialog        */
   /* Currently they must be the same for both 16-bit and 32-bit    */
   /* so we pack the structures.                                    */
   typedef struct _DLGTEMPLATE    /* dlgt */
   {
      USHORT   cbTemplate;
      USHORT   type;
      USHORT   codepage;
      USHORT   offadlgti;
      USHORT   fsTemplateStatus;
      USHORT   iItemFocus;
      USHORT   coffPresParams;
      DLGTITEM adlgti[1];
   } DLGTEMPLATE;
   typedef DLGTEMPLATE *PDLGTEMPLATE;
   
   #pragma pack()  /* restore packing to default */
   
   HWND   APIENTRY WinCreateDlg(HWND hwndParent,
                                HWND hwndOwner,
                                PFNWP pfnDlgProc,
                                PDLGTEMPLATE pdlgt,
                                PVOID pCreateParams);
   
   
#endif /* INCL_WINDIALOGS */


#ifdef INCL_WINSTATICS
   
   /*** Static Control Manager */
   
   /* Static control styles:
   *
   * NOTE: the top 9 bits of the LOWORD of the window flStyle are used for
   * DT_* flags.  The lower 7 bits are for SS_* styles.  This gives us up
   * to 128 distinct static control types (we currently use 11 of them).
   */
   #define SS_TEXT                    0x0001L
   #define SS_GROUPBOX                0x0002L
   
   #ifndef INCL_SAADEFS
      #define SS_ICON                    0x0003L
      #define SS_BITMAP                  0x0004L
   #endif /* !INCL_SAADEFS */
   #define SS_FGNDRECT                0x0005L
   #ifndef INCL_SAADEFS
      #define SS_HALFTONERECT            0x0006L
   #endif /* !INCL_SAADEFS */
   #define SS_BKGNDRECT               0x0007L
   #define SS_FGNDFRAME               0x0008L
   #ifndef INCL_SAADEFS
      #define SS_HALFTONEFRAME           0x0009L
   #endif /* !INCL_SAADEFS */
   #define SS_BKGNDFRAME              0x000aL
   #define SS_SYSICON                 0x000bL
   #define SS_AUTOSIZE         0x0040L
   
   
   /* Static control messages */
   
   #define SM_SETHANDLE               0x0100
   #define SM_QUERYHANDLE             0x0101
   
   /* Message Box 2 notification messages */
   #define WM_MSGBOXINIT              0x010E
   #define WM_MSGBOXDISMISS           0x010F
#endif /* INCL_WINSTATICS */


#ifdef INCL_WINBUTTONS
   /**** Button Controls Subsection */
   
   /* Button control styles */
   
   #define BS_PUSHBUTTON              0L
   #define BS_CHECKBOX                1L
   #define BS_AUTOCHECKBOX            2L
   #define BS_RADIOBUTTON             3L
   #define BS_AUTORADIOBUTTON         4L
   #define BS_3STATE                  5L
   #define BS_AUTO3STATE              6L
   
   #ifndef INCL_SAADEFS
      #define BS_USERBUTTON              7L
   #endif /* !INCL_SAADEFS */
   #define BS_NOTEBOOKBUTTON          8L
   #define BS_PRIMARYSTYLES       0x000fL
   
   #define BS_TEXT                 0x0010
   #define BS_MINIICON             0x0020
   #define BS_BITMAP           0x0040L
   #define BS_ICON             0x0080L
   #define BS_HELP                    0x0100L
   #define BS_SYSCOMMAND              0x0200L
   #define BS_DEFAULT                 0x0400L
   #define BS_NOPOINTERFOCUS          0x0800L
   #define BS_NOBORDER                0x1000L
   #define BS_NOCURSORSELECT          0x2000L
   #define BS_AUTOSIZE             0x4000L
   
   
   
   #ifndef INCL_SAADEFS
      #pragma pack(2) /* force structure alignment packing */
      
      /* Binary resources may be bound into application executables or */
      /* passed as a parameter when creating a window or dialog        */
      /* Currently they must be the same for both 16-bit and 32-bit    */
      /* so we pack the structures.                                    */
      typedef struct _BTNCDATA    /* btncd */
      {
         USHORT  cb;
         USHORT  fsCheckState;
         USHORT  fsHiliteState;
         LHANDLE hImage;
      } BTNCDATA;
      typedef BTNCDATA *PBTNCDATA;
      
      #pragma pack()  /* restore packing to default */
      
   #endif /* !INCL_SAADEFS */
   
   /*** User button structure (passed in WM_CONTROL msg) *******************/
   
   #ifndef INCL_SAADEFS
      typedef struct _USERBUTTON    /* ubtn */
      {
         HWND    hwnd;
         HPS     hps;
         ULONG   fsState;
         ULONG   fsStateOld;
      } USERBUTTON;
      typedef USERBUTTON *PUSERBUTTON;
   #endif /* !INCL_SAADEFS */
   
   /* Button control messages */
   
   #define BM_CLICK                   0x0120
   #define BM_QUERYCHECKINDEX         0x0121
   #define BM_QUERYHILITE             0x0122
   #define BM_SETHILITE               0x0123
   #define BM_QUERYCHECK              0x0124
   #define BM_SETCHECK                0x0125
   #define BM_SETDEFAULT              0x0126
   #define BM_AUTOSIZE                0x0128
   
   /* Button notification codes */
   
   #define BN_CLICKED                 1
   #define BN_DBLCLICKED              2
   #define BN_PAINT                   3
   
   /* BN_PAINT button draw state codes (must be in high byte) */
   
   #ifndef INCL_SAADEFS
      #define BDS_HILITED                0x0100
      #define BDS_DISABLED               0x0200
      #define BDS_DEFAULT                0x0400
      
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINBUTTONS */


#ifdef INCL_WINENTRYFIELDS
   /**** Entryfield controls Subsection */
   
   /* Entry field  styles */
   
   #define ES_LEFT                    0x00000000L
   #define ES_CENTER                  0x00000001L
   #define ES_RIGHT                   0x00000002L
   
   #ifndef INCL_SAADEFS
      #define ES_AUTOSCROLL              0x00000004L
   #endif /* !INCL_SAADEFS */
   
   #define ES_MARGIN                  0x00000008L
   #define ES_AUTOTAB                 0x00000010L
   #define ES_READONLY                0x00000020L
   #define ES_COMMAND                 0x00000040L
   #define ES_UNREADABLE              0x00000080L
   #define ES_AUTOSIZE                0x00000200L
   
   #ifdef INCL_NLS
      #define ES_ANY                     0x00000000L
      #define ES_SBCS                    0x00001000L
      #define ES_DBCS                    0x00002000L
      #define ES_MIXED                   0x00003000L
   #endif /* INCL_NLS */
   
   
   /*
   * combo box styles
   */
   #define CBS_SIMPLE                 0x0001L
   #define CBS_DROPDOWN               0x0002L
   #define CBS_DROPDOWNLIST           0x0004L
   
   /*
   * Use this bit for drop down combo boxes that do not want to
   * receive a CBN_ENTER on a single click in their list boxes.
   * This is for compatibility with releases prior to OS/2 2.0 which
   * did not send this message
   */
   #define CBS_COMPATIBLE             0x0008L
   
   /*
   * The following edit and listbox styles may be used in conjunction
   * with CBS_ styles
   * ES_AUTOTAB ES_ANY ES_SBCS ES_DBCS ES_MIXED LS_HORZSCROLL
   */
   
   /*
   * IDs of combobox entry field and listbox.
   */
   #define CBID_LIST                  0x029A
   #define CBID_EDIT                  0x029B
   
   #define CBM_SHOWLIST               0x0170
   #define CBM_HILITE                 0x0171
   #define CBM_ISLISTSHOWING          0x0172
   
   #define CBN_EFCHANGE               1
   #define CBN_EFSCROLL               2
   #define CBN_MEMERROR               3
   #define CBN_LBSELECT               4
   #define CBN_LBSCROLL               5
   #define CBN_SHOWLIST               6
   #define CBN_ENTER                  7
   
   
   #ifndef INCL_SAADEFS
      #pragma pack(2) /* force structure alignment packing */
      
      /* Binary resources may be bound into application executables or */
      /* passed as a parameter when creating a window or dialog        */
      /* Currently they must be the same for both 16-bit and 32-bit    */
      /* so we pack the structures.                                    */
      
      /*******************************************************************/
      /* Combobox control data structure                                 */
      /*******************************************************************/
      typedef struct _COMBOCDATA          /* combocdata */
      {
         ULONG   cbSize;             /* Size of control block             */
         ULONG   reserved;           /* reserved field                    */
         PVOID   pHWXCtlData;        /* reserved for Pen CtlData (penpm.h)*/
      } COMBOCDATA;
      typedef COMBOCDATA *PCOMBOCDATA;
      
      typedef struct _ENTRYFDATA    /* efd */
      {
         USHORT  cb;
         USHORT  cchEditLimit;
         USHORT  ichMinSel;
         USHORT  ichMaxSel;
         PVOID   pHWXCtlData;    /* reserved for Pen CtlData (penpm.h)  */
      } ENTRYFDATA;
      typedef ENTRYFDATA *PENTRYFDATA;
      
      #pragma pack()  /* restore packing to default */
      
   #endif /* !INCL_SAADEFS */
   
   /* Entry Field  messages */
   
   #define EM_QUERYCHANGED            0x0140
   #define EM_QUERYSEL                0x0141
   #define EM_SETSEL                  0x0142
   #define EM_SETTEXTLIMIT            0x0143
   
   #ifndef INCL_SAADEFS
      #define EM_CUT                     0x0144
      #define EM_COPY                    0x0145
   #endif /* !INCL_SAADEFS */
   #define EM_CLEAR                   0x0146
   #ifndef INCL_SAADEFS
      #define EM_PASTE                   0x0147
   #endif /* !INCL_SAADEFS */
   #define EM_QUERYFIRSTCHAR          0x0148
   #define EM_SETFIRSTCHAR            0x0149
   #ifndef INCL_SAADEFS
      #define EM_QUERYREADONLY           0x014a
      #define EM_SETREADONLY             0x014b
      #define EM_SETINSERTMODE           0x014c
   #endif /* !INCL_SAADEFS */
   
   /* Entry Field notification messages */
   
   #define EN_SETFOCUS                0x0001
   #define EN_KILLFOCUS               0x0002
   #define EN_CHANGE                  0x0004
   #define EN_SCROLL                  0x0008
   #ifndef INCL_SAADEFS
      #define EN_MEMERROR                0x0010
      #define EN_OVERFLOW                0x0020
      #define EN_INSERTMODETOGGLE        0x0040
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINENTRYFIELDS */


/*  Multiple Line Entrys */
#ifdef INCL_WINMLE
   #include <pmmle.h>
#endif /* !INCL_WINMLE */
#ifdef INCL_WINLISTBOXES
   
   /**** Listboxes */
   
   /* List box styles */
   
   #define LS_MULTIPLESEL             0x00000001L
   
   #ifndef INCL_SAADEFS
      #define LS_OWNERDRAW               0x00000002L
   #endif /* !INCL_SAADEFS */
   
   #define LS_NOADJUSTPOS             0x00000004L
   #define LS_HORZSCROLL              0x00000008L
   
   #define LS_EXTENDEDSEL             0x00000010L
   
   
   /* List box notification messages */
   
   #define LN_SELECT                  1
   #define LN_SETFOCUS                2
   #define LN_KILLFOCUS               3
   #define LN_SCROLL                  4
   #define LN_ENTER                   5
   
   /* List box messages */
   
   #define LM_QUERYITEMCOUNT          0x0160
   #define LM_INSERTITEM              0x0161
   #define LM_SETTOPINDEX             0x0162
   #define LM_DELETEITEM              0x0163
   #define LM_SELECTITEM              0x0164
   #define LM_QUERYSELECTION          0x0165
   #define LM_SETITEMTEXT             0x0166
   #define LM_QUERYITEMTEXTLENGTH     0x0167
   #define LM_QUERYITEMTEXT           0x0168
   
   #define LM_SETITEMHANDLE           0x0169
   #define LM_QUERYITEMHANDLE         0x016a
   #define LM_SEARCHSTRING            0x016b
   #define LM_SETITEMHEIGHT           0x016c
   #define LM_QUERYTOPINDEX           0x016d
   #define LM_DELETEALL               0x016e
   #define LM_INSERTMULTITEMS         0x016f
   #define LM_SETITEMWIDTH            0x0660
   
   
   /* List box constants */
   
   #define LIT_CURSOR         (-4)
   #define LIT_ERROR                  (-3)
   #define LIT_MEMERROR               (-2)
   #define LIT_NONE                   (-1)
   #define LIT_FIRST                  (-1)
   
   /* For LM_INSERTITEM msg */
   
   #define LIT_END                    (-1)
   #define LIT_SORTASCENDING          (-2)
   #define LIT_SORTDESCENDING         (-3)
   
   /* For LM_SEARCHSTRING msg */
   
   #define LSS_SUBSTRING              0x0001
   #define LSS_PREFIX                 0x0002
   #define LSS_CASESENSITIVE          0x0004
   
   /*******************************************************************/
   /* Structure for use with LM_INSERTMULTITEMS                       */
   /*******************************************************************/
   
   typedef struct _LBOXINFO         /* lboxinfo */
   {
      LONG  lItemIndex;            /* Item index */
      ULONG ulItemCount;           /* Item count */
      ULONG reserved;              /* Reserved - must be zero */
      ULONG reserved2;             /* Reserved - must be zero */
   } LBOXINFO;
   
   typedef LBOXINFO * PLBOXINFO;
   
   /* Useful macros */
   
   /***************************************************************************
   * LONG/SHORT WinDeleteLboxItem(HWND hwndLbox, LONG/SHORT index)
   *    deletes item index from the listbox.
   *    returns the number of items left.
   ***************************************************************************/
   #define WinDeleteLboxItem(hwndLbox, index) \
   ((LONG)WinSendMsg(hwndLbox, LM_DELETEITEM, MPFROMLONG(index), \
   (MPARAM)NULL))
   
   /***************************************************************************
   * LONG/SHORT WinInsertLboxItem(HWND hwndLbox, LONG/SHORT index, PSZ psz)
   *    inserts psz into a listbox at index.  Index may be an LIT_ constant.
   *    returns the actual index where it was inserted.
   ***************************************************************************/
   #define WinInsertLboxItem(hwndLbox, index, psz) \
   ((LONG)WinSendMsg(hwndLbox, LM_INSERTITEM, MPFROMLONG(index), \
   MPFROMP(psz)))
   
   /***************************************************************************
   * LONG/SHORT WinQueryLboxCount(HWND hwndLbox)
   *    returns the number of items in the listbox.
   *
   ***************************************************************************/
   #define WinQueryLboxCount(hwndLbox) \
   ((LONG)WinSendMsg(hwndLbox, LM_QUERYITEMCOUNT, (MPARAM)NULL, \
   (MPARAM)NULL))
   
   /***************************************************************************
   * LONG/SHORT WinQueryLboxItemText(HWND hwndLbox, LONG/SHORT index, PSZ psz, SHORT cchMax)
   *    fills psz with the text if item index.
   *    returns the text length copied.
   ***************************************************************************/
   #define WinQueryLboxItemText(hwndLbox, index, psz, cchMax) \
   ((LONG)WinSendMsg(hwndLbox, LM_QUERYITEMTEXT, \
   MPFROM2SHORT((index), (cchMax)), MPFROMP(psz)))
   
   /***************************************************************************
   * LONG/SHORT WinQueryLboxItemTextLength(HWND hwndLbox, LONG/SHORT index)
   *    returns the length of item index.
   *
   ***************************************************************************/
   #define WinQueryLboxItemTextLength(hwndLbox, index)         \
   ((SHORT)WinSendMsg(hwndLbox, LM_QUERYITEMTEXTLENGTH,    \
   MPFROMSHORT(index), (MPARAM)NULL))
   
   /***************************************************************************
   * BOOL WinSetLboxItemText(HWND hwndLbox, LONG/SHORT index, PSZ psz)
   *    sets the text of item index to psz.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinSetLboxItemText(hwndLbox, index, psz) \
   ((BOOL)WinSendMsg(hwndLbox, LM_SETITEMTEXT, \
   MPFROMLONG(index), MPFROMP(psz)))
   
   /***************************************************************************
   * LONG/SHORT WinQueryLboxSelectedItem(HWND hwndLbox)
   *    returns the item index selected. (for single selection only)
   *
   ***************************************************************************/
   #define WinQueryLboxSelectedItem(hwndLbox) \
   ((LONG)WinSendMsg(hwndLbox, LM_QUERYSELECTION, MPFROMLONG(LIT_FIRST), \
   (MPARAM)NULL))
   
#endif /* INCL_WINLISTBOXES */


#ifdef INCL_WINMENUS
   
   /**** Menu Manager Subsection */
   
   /* Menu control styles */
   
   #define MS_ACTIONBAR               0x00000001L
   #define MS_TITLEBUTTON             0x00000002L
   #define MS_VERTICALFLIP            0x00000004L
   
   #define MS_CONDITIONALCASCADE      0x00000040L
   
   HWND APIENTRY WinLoadMenu(HWND hwndFrame,
                             HMODULE hmod,
                             ULONG idMenu);
   
   
   /* Menu control messages */
   
   #define MM_INSERTITEM              0x0180
   #define MM_DELETEITEM              0x0181
   #define MM_QUERYITEM               0x0182
   #define MM_SETITEM                 0x0183
   #define MM_QUERYITEMCOUNT          0x0184
   #define MM_STARTMENUMODE           0x0185
   #define MM_ENDMENUMODE             0x0186
   #define MM_REMOVEITEM              0x0188
   #define MM_SELECTITEM              0x0189
   #define MM_QUERYSELITEMID          0x018a
   #define MM_QUERYITEMTEXT           0x018b
   #define MM_QUERYITEMTEXTLENGTH     0x018c
   #define MM_SETITEMHANDLE           0x018d
   #define MM_SETITEMTEXT             0x018e
   #define MM_ITEMPOSITIONFROMID      0x018f
   #define MM_ITEMIDFROMPOSITION      0x0190
   #define MM_QUERYITEMATTR           0x0191
   #define MM_SETITEMATTR             0x0192
   #define MM_ISITEMVALID             0x0193
   #define MM_QUERYITEMRECT           0x0194
   
   #define MM_QUERYDEFAULTITEMID      0x0431
   #define MM_SETDEFAULTITEMID        0x0432
   
   #ifndef MTI_INCLUDED
      #define MTI_INCLUDED
      
      typedef struct _mti     /* mti */
      {
         USHORT afStyle;      /* Style Flags      */
         USHORT pad;          /* pad for template */
         USHORT idItem;       /* Item ID          */
         CHAR   c[2];         /*                  */
      } MTI;
      
      typedef struct _mt      /* mt */
      {
         ULONG  len;          /* Length of template in bytes */
         USHORT codepage;     /* Codepage                    */
         USHORT reserved;     /* Reserved.                   */
         USHORT cMti;         /* Count of template items.    */
         MTI    rgMti[1];     /* Array of template items.    */
      } MT;
      typedef MT *LPMT;
      
   #endif
   
   HWND APIENTRY WinCreateMenu(HWND hwndParent,
                               PVOID lpmt);
   
   /* Owner Item Structure (Also used for listboxes) */
   
   #ifndef INCL_SAADEFS
      typedef struct _OWNERITEM    /* oi */
      {
         HWND    hwnd;
         HPS     hps;
         ULONG   fsState;
         ULONG   fsAttribute;
         ULONG   fsStateOld;
         ULONG   fsAttributeOld;
         RECTL   rclItem;
         LONG    idItem; /* This field contains idItem for menus, iItem for lb. */
         ULONG   hItem;
      } OWNERITEM;
      typedef OWNERITEM *POWNERITEM;
   #endif /* !INCL_SAADEFS */
   
   /* Menu item */
   
   #pragma pack(2) /* force structure alignment packing */
   
   /* Binary resources may be bound into application executables or */
   /* passed as a parameter when creating a window or dialog        */
   /* Currently they must be the same for both 16-bit and 32-bit    */
   /* so we pack the structures.                                    */
   typedef struct _MENUITEM    /* mi */
   {
      SHORT   iPosition;
      USHORT  afStyle;
      USHORT  afAttribute;
      USHORT  id;
      HWND    hwndSubMenu;
      ULONG   hItem;
   } MENUITEM;
   typedef MENUITEM *PMENUITEM;
   
   #pragma pack()  /* restore packing to default */
   
   #define MIT_END                    (-1)
   #define MIT_NONE                   (-1)
   #define MIT_MEMERROR               (-1)
   #define MIT_ERROR                  (-1)
   #define MIT_FIRST                  (-2)
   #define MIT_LAST                   (-3)
   #define MID_NONE                   MIT_NONE
   #define MID_ERROR                  (-1)
   
   /* Menu item styles & attributes */
   
   #define MIS_TEXT                   0x0001
   
   #ifndef INCL_SAADEFS
      #define MIS_BITMAP                 0x0002
   #endif /* !INCL_SAADEFS */
   #define MIS_SEPARATOR              0x0004
   
   #ifndef INCL_SAADEFS
      #define MIS_OWNERDRAW              0x0008
   #endif /* !INCL_SAADEFS */
   
   #define MIS_SUBMENU                0x0010
   #define MIS_MULTMENU               0x0020      /* multiple choice submenu     */
   #define MIS_SYSCOMMAND             0x0040
   #define MIS_HELP                   0x0080
   #define MIS_STATIC                 0x0100
   #define MIS_BUTTONSEPARATOR        0x0200
   #define MIS_BREAK                  0x0400
   #define MIS_BREAKSEPARATOR         0x0800
   #define MIS_GROUP                  0x1000      /* multiple choice group start */
   /* In multiple choice submenus a style of 'single' denotes the item is a
   ** radiobutton.  Absence of this style defaults the item to a checkbox.       */
   #define MIS_SINGLE                 0x2000
   
   #define MIA_NODISMISS              0x0020
   #define MIA_FRAMED                 0x1000
   #define MIA_CHECKED                0x2000
   #define MIA_DISABLED               0x4000
   #define MIA_HILITED                0x8000
   
   
   BOOL APIENTRY WinPopupMenu(HWND hwndParent,
                              HWND hwndOwner,
                              HWND hwndMenu,
                              LONG x,
                              LONG y,
                              LONG idItem,
                              ULONG fs);
   
   /* Values of fs in WinPopupMenu call */
   
   #define PU_POSITIONONITEM          0x0001 /* Need idItem parameter */
   
   #define PU_HCONSTRAIN              0x0002 /* Keep menu on left and right edge */
   #define PU_VCONSTRAIN              0x0004 /* Keep menu on top and bottom edge */
   
   #define PU_NONE                    0x0000 /* If invoked by keyboard */
   #define PU_MOUSEBUTTON1DOWN        0x0008 /* If invoked by button 1 */
   #define PU_MOUSEBUTTON2DOWN        0x0010 /* If invoked by button 2 */
   #define PU_MOUSEBUTTON3DOWN        0x0018 /* If invoked by button 3 */
   
   #define PU_SELECTITEM              0x0020 /* Set selected item (use with kbd) */
   
   #define PU_MOUSEBUTTON1            0x0040 /* If button1 use allowed */
   #define PU_MOUSEBUTTON2            0x0080 /* If button2 use allowed */
   #define PU_MOUSEBUTTON3            0x0100 /* If button3 use allowed */
   #define PU_KEYBOARD                0x0200 /* If keyboard use allowed */
   
   
   /* Useful macros */
   
   /***************************************************************************
   * BOOL WinCheckMenuItem(HWND hwndMenu, ULONG/USHORT id, BOOL fcheck)
   *    Sets the check state of menuitem #id to fcheck.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinCheckMenuItem(hwndMenu, id, fcheck) \
   ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMATTR, \
   MPFROM2SHORT(id, TRUE), \
   MPFROM2SHORT(MIA_CHECKED, (USHORT)(fcheck) ? MIA_CHECKED : 0)))
   
   /***************************************************************************
   * BOOL WinIsMenuItemChecked(HWND hwndMenu, USHORT/ULONG id)
   *    returns the check state of menuitem #id.
   *
   ***************************************************************************/
   #define WinIsMenuItemChecked(hwndMenu, id) \
   ((BOOL)WinSendMsg(hwndMenu, MM_QUERYITEMATTR, \
   MPFROM2SHORT(id, TRUE), \
   MPFROMLONG(MIA_CHECKED)))
   
   /***************************************************************************
   * BOOL WinEnableMenuItem(HWND hwndMenu, ULONG/USHORT id, BOOL fenable)
   *    sets the enable state of menuitem #id to fenable.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinEnableMenuItem(hwndMenu, id, fEnable) \
   ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMATTR, \
   MPFROM2SHORT(id, TRUE), \
   MPFROM2SHORT(MIA_DISABLED, (USHORT)(fEnable) ? 0 : MIA_DISABLED)))
   
   /***************************************************************************
   * BOOL WinIsMenuItemEnabled(HWND hwndMenu, USHORT/ULONG id)
   *    returns the enable state of menuitem #id.
   *
   ***************************************************************************/
   #define WinIsMenuItemEnabled(hwndMenu, id)  \
   (!(BOOL)WinSendMsg(hwndMenu, MM_QUERYITEMATTR, \
   MPFROM2SHORT(id, TRUE), \
   MPFROMLONG(MIA_DISABLED)))
   
   /***************************************************************************
   * BOOL WinSetMenuItemText(HWND hwndMenu, ULONG/USHORT id, PSZ psz)
   *    sets the text of menuitem #id.
   *    returns fSuccess.
   ***************************************************************************/
   #define WinSetMenuItemText(hwndMenu, id, psz) \
   ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMTEXT, \
   MPFROMLONG(id), MPFROMP(psz)))
   
   /***************************************************************************
   * BOOL WinIsMenuItemValid(HWND hwndMenu, ULONG/USHORT id)
   *    Returns TRUE if the specified item is a valid choice.
   ***************************************************************************/
   #define WinIsMenuItemValid(hwndMenu, id) \
   ((BOOL)WinSendMsg(hwndMenu, MM_ISITEMVALID, \
   MPFROM2SHORT(id, TRUE), MPFROMLONG(FALSE)))
   
#endif /* INCL_WINMENUS */


#ifdef INCL_WINSCROLLBARS
   
   /*** Scroll Bar controls Subsection */
   
   /* Scroll Bar styles */
   
   #define SBS_HORZ                   0L
   #define SBS_VERT                   1L
   #define SBS_THUMBSIZE              2L
   #define SBS_AUTOTRACK              4L
   #define SBS_AUTOSIZE        0x2000L
   
   
   
   /* Scroll Bar messages */
   
   #define SBM_SETSCROLLBAR           0x01a0
   #define SBM_SETPOS                 0x01a1
   #define SBM_QUERYPOS               0x01a2
   #define SBM_QUERYRANGE             0x01a3
   #define SBM_SETTHUMBSIZE           0x01a6
   
   /* Scroll Bar Commands */
   
   #define SB_LINEUP                  1
   #define SB_LINEDOWN                2
   #define SB_LINELEFT                1
   #define SB_LINERIGHT               2
   #define SB_PAGEUP                  3
   #define SB_PAGEDOWN                4
   #define SB_PAGELEFT                3
   #define SB_PAGERIGHT               4
   #define SB_SLIDERTRACK             5
   #define SB_SLIDERPOSITION          6
   #define SB_ENDSCROLL               7
   
   
   #ifndef INCL_SAADEFS
      #pragma pack(2) /* force structure alignment packing */
      
      /* Binary resources may be bound into application executables or */
      /* passed as a parameter when creating a window or dialog        */
      /* Currently they must be the same for both 16-bit and 32-bit    */
      /* so we pack the structures.                                    */
      typedef struct _SBCDATA    /* sbcd */
      {
         USHORT  cb;
         USHORT  sHilite;      /* reserved, should be set to zero */
         SHORT   posFirst;
         SHORT   posLast;
         SHORT   posThumb;
         SHORT   cVisible;
         SHORT   cTotal;
      } SBCDATA;
      typedef SBCDATA *PSBCDATA;
      
      #pragma pack()  /* restore packing to default */
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINSCROLLBARS */


#if (defined(INCL_WINFRAMEMGR) || !defined(INCL_NOCOMMON))
   /*** Frame Manager Common subsection */
   
   #pragma pack(2) /* force structure alignment packing */
   
   /* Binary resources may be bound into application executables or */
   /* passed as a parameter when creating a window or dialog        */
   /* Currently they must be the same for both 16-bit and 32-bit    */
   /* so we pack the structures.                                    */
   typedef struct _FRAMECDATA    /* fcdata */
   {
      USHORT  cb;
      ULONG   flCreateFlags;
      USHORT  hmodResources;
      USHORT  idResources;
   } FRAMECDATA;
   typedef FRAMECDATA *PFRAMECDATA;
   
   #pragma pack()  /* restore packing to default */
   
   /* Frame window styles */
   /* All unused FCF_xxx bits are reserved */
   #define FCF_TITLEBAR               0x00000001L
   #define FCF_SYSMENU                0x00000002L
   #define FCF_MENU                   0x00000004L
   #define FCF_SIZEBORDER             0x00000008L
   #define FCF_MINBUTTON              0x00000010L
   #define FCF_MAXBUTTON              0x00000020L
   #define FCF_MINMAX                 0x00000030L /* minmax means BOTH buttons */
   #define FCF_VERTSCROLL             0x00000040L
   #define FCF_HORZSCROLL             0x00000080L
   #define FCF_DLGBORDER              0x00000100L
   #define FCF_BORDER                 0x00000200L
   #define FCF_SHELLPOSITION          0x00000400L
   #define FCF_TASKLIST               0x00000800L
   #define FCF_NOBYTEALIGN            0x00001000L
   #define FCF_NOMOVEWITHOWNER        0x00002000L
   #define FCF_ICON                   0x00004000L
   #define FCF_ACCELTABLE             0x00008000L
   #define FCF_SYSMODAL               0x00010000L
   #define FCF_SCREENALIGN            0x00020000L
   #define FCF_MOUSEALIGN             0x00040000L
   #define FCF_HIDEBUTTON             0x01000000L
   #define FCF_HIDEMAX                0x01000020L /* hidemax means BOTH buttons */
   #define FCF_CLOSEBUTTON            0x04000000L /* use when no other min/max  */
   /*   button is present        */
   #ifdef INCL_NLS
      #define FCF_DBE_APPSTAT         0x80000000L
   #endif /* INCL_NLS */
   #define FCF_AUTOICON               0x40000000L
   
   /* FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU | FCF_SIZEBORDER | FCF_MINMAX |
   FCF_ICON | FCF_ACCELTABLE | FCF_SHELLPOSITION | FCF_TASKLIST */
   #define FCF_STANDARD            0x0000CC3FL
   
   
   #define FS_ICON                    0x00000001L
   #define FS_ACCELTABLE              0x00000002L
   
   #ifndef INCL_SAADEFS
      #define FS_SHELLPOSITION        0x00000004L
   #endif /* !INCL_SAADEFS */
   
   #define FS_TASKLIST                0x00000008L
   #define FS_NOBYTEALIGN             0x00000010L
   #define FS_NOMOVEWITHOWNER         0x00000020L
   #define FS_SYSMODAL                0x00000040L
   #define FS_DLGBORDER               0x00000080L
   #define FS_BORDER                  0x00000100L
   #define FS_SCREENALIGN             0x00000200L
   #define FS_MOUSEALIGN              0x00000400L
   #define FS_SIZEBORDER              0x00000800L
   #define FS_AUTOICON                0x00001000L
   #ifdef INCL_NLS
      #define FS_DBE_APPSTAT          0x00008000L
   #endif /* INCL_NLS */
   
   /* FS_ICON | FS_ACCELTABLE | FS_SHELLPOSITION | FS_TASKLIST */
   #define FS_STANDARD                0x0000000FL
   
   
   /* Frame Window Flags accessed via WinSet/QueryWindowUShort(QWS_FLAGS) */
   
   #ifndef INCL_SAADEFS
      #define FF_FLASHWINDOW             0x0001
      #define FF_ACTIVE                  0x0002
      #define FF_FLASHHILITE             0x0004
      #define FF_OWNERHIDDEN             0x0008
      #define FF_DLGDISMISSED            0x0010
      #define FF_OWNERDISABLED           0x0020
      #define FF_SELECTED                0x0040
      #define FF_NOACTIVATESWP           0x0080
      #define FF_DIALOGBOX               0x0100
      
   #endif /* !INCL_SAADEFS */
   
   
#ifdef __cplusplus
   HWND  APIENTRY WinCreateStdWindow(HWND hwndParent,
                                     ULONG flStyle,
                                     PULONG pflCreateFlags,
                                     PCSZ  pszClientClass,
                                     PCSZ  pszTitle,
                                     ULONG styleClient,
                                     HMODULE hmod,
                                     ULONG idResources,
                                     PHWND phwndClient);
#else
   HWND  APIENTRY WinCreateStdWindow(HWND hwndParent,
                                     ULONG flStyle,
                                     PULONG pflCreateFlags,
                                     PSZ pszClientClass,
                                     PSZ pszTitle,
                                     ULONG styleClient,
                                     HMODULE hmod,
                                     ULONG idResources,
                                     PHWND phwndClient);
#endif
   
   
#endif /* Frame Manager Common subsection */


#ifdef INCL_WINFRAMEMGR
   
   BOOL  APIENTRY WinFlashWindow(HWND hwndFrame,
                                 BOOL fFlash);
   
   /* Frame window related messages */
   
   #define WM_FLASHWINDOW             0x0040
   #define WM_FORMATFRAME             0x0041
   #define WM_UPDATEFRAME             0x0042
   #define WM_FOCUSCHANGE             0x0043
   
   #define WM_SETBORDERSIZE           0x0044
   #define WM_TRACKFRAME              0x0045
   #define WM_MINMAXFRAME             0x0046
   #define WM_SETICON                 0x0047
   #define WM_QUERYICON               0x0048
   #define WM_SETACCELTABLE           0x0049
   #define WM_QUERYACCELTABLE         0x004a
   #define WM_TRANSLATEACCEL          0x004b
   #define WM_QUERYTRACKINFO          0x004c
   #define WM_QUERYBORDERSIZE         0x004d
   #define WM_NEXTMENU                0x004e
   #define WM_ERASEBACKGROUND         0x004f
   #define WM_QUERYFRAMEINFO          0x0050
   #define WM_QUERYFOCUSCHAIN         0x0051
   #define WM_OWNERPOSCHANGE          0x0052
   #define WM_CALCFRAMERECT           0x0053
   /* Note 0x0054 is reserved */
   #define WM_WINDOWPOSCHANGED        0x0055
   #define WM_ADJUSTFRAMEPOS          0x0056
   #define WM_QUERYFRAMECTLCOUNT      0x0059
   #ifndef INCL_SAADEFS
      /* Note 0x005A is reserved */
      #define WM_QUERYHELPINFO        0x005B
      #define WM_SETHELPINFO          0x005C
      #define WM_ERROR                0x005D
      #define WM_REALIZEPALETTE       0x005E
      
      
      /* WM_QUERYFRAMEINFO constants */
      
      #define FI_FRAME                0x00000001L
      #define FI_OWNERHIDE            0x00000002L
      #define FI_ACTIVATEOK           0x00000004L
      #define FI_NOMOVEWITHOWNER      0x00000008L
      
      
   #endif /* !INCL_SAADEFS */
   
   
#ifdef __cplusplus
   BOOL  APIENTRY WinCreateFrameControls(HWND hwndFrame,
                                         PFRAMECDATA pfcdata,
                                         PCSZ  pszTitle);
#else
   BOOL  APIENTRY WinCreateFrameControls(HWND hwndFrame,
                                         PFRAMECDATA pfcdata,
                                         PSZ pszTitle);
#endif
   
   BOOL  APIENTRY WinCalcFrameRect(HWND hwndFrame,
                                   PRECTL prcl,
                                   BOOL fClient);
   
   BOOL  APIENTRY WinGetMinPosition(HWND hwnd,
                                    PSWP pswp,
                                    PPOINTL pptl);
   #ifndef INCL_SAADEFS
      BOOL  APIENTRY WinGetMaxPosition(HWND hwnd,
                                       PSWP pswp);
   #endif /* !INCL_SAADEFS */
   
   typedef LHANDLE HSAVEWP;      /* hsvwp */
   
   BOOL APIENTRY WinSaveWindowPos(HSAVEWP hsvwp,
                                  PSWP pswp,
                                  ULONG cswp);
   
   /* Frame control IDs    */
   
   #define FID_SYSMENU                0x8002
   #define FID_TITLEBAR               0x8003
   #define FID_MINMAX                 0x8004
   #define FID_MENU                   0x8005
   #define FID_VERTSCROLL             0x8006
   #define FID_HORZSCROLL             0x8007
   #define FID_CLIENT                 0x8008
   /* Note 0x8009 is reserved */
   
   #define FID_DBE_APPSTAT            0x8010
   #define FID_DBE_KBDSTAT            0x8011
   #define FID_DBE_PECIC              0x8012
   #define FID_DBE_KKPOPUP            0x8013
   
   /* Standard WM_SYSCOMMAND command values */
   
   #define SC_SIZE                    0x8000
   #define SC_MOVE                    0x8001
   #define SC_MINIMIZE                0x8002
   #define SC_MAXIMIZE                0x8003
   #define SC_CLOSE                   0x8004
   #define SC_NEXT                    0x8005
   #define SC_APPMENU                 0x8006
   #define SC_SYSMENU                 0x8007
   #define SC_RESTORE                 0x8008
   #define SC_NEXTFRAME               0x8009
   #define SC_NEXTWINDOW              0x8010
   #ifndef INCL_SAADEFS
      #define SC_TASKMANAGER             0x8011
      #define SC_HELPKEYS                0x8012
      #define SC_HELPINDEX               0x8013
      #define SC_HELPEXTENDED            0x8014
      #define SC_SWITCHPANELIDS          0x8015
      #define SC_DBE_FIRST               0x8018
      #define SC_DBE_LAST                0x801F
      
      #define SC_BEGINDRAG               0x8020
      #define SC_ENDDRAG                 0x8021
      #define SC_SELECT                  0x8022
      #define SC_OPEN                    0x8023
      #define SC_CONTEXTMENU             0x8024
      #define SC_CONTEXTHELP             0x8025
      #define SC_TEXTEDIT                0x8026
      #define SC_BEGINSELECT             0x8027
      #define SC_ENDSELECT               0x8028
      #define SC_WINDOW                  0x8029
      #define SC_HIDE                    0x802a
      
      
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINFRAMEMGR */

/*** Frame controls */

#ifdef INCL_WINFRAMECTLS
   
   /** Title bar controls */
   
   
   /* Title bar control messages */
   
   #define TBM_SETHILITE              0x01e3
   #define TBM_QUERYHILITE            0x01e4
   
   
#endif /* INCL_WINFRAMECTLS */

#ifdef INCL_WINRECTANGLES
   /*** Rectangle routines */
   
   BOOL APIENTRY WinCopyRect(HAB hab,
                             PRECTL prclDst,
                             PRECTL prclSrc);
   
   #ifndef INCL_SAADEFS
      BOOL APIENTRY WinSetRect(HAB hab,
                               PRECTL prcl,
                               LONG xLeft,
                               LONG yBottom,
                               LONG xRight,
                               LONG yTop);
      BOOL APIENTRY WinIsRectEmpty(HAB hab,
                                   PRECTL prcl);
      BOOL APIENTRY WinEqualRect(HAB hab,
                                 PRECTL prcl1,
                                 PRECTL prcl2);
      BOOL APIENTRY WinSetRectEmpty(HAB hab,
                                    PRECTL prcl);
      BOOL APIENTRY WinOffsetRect(HAB hab,
                                  PRECTL prcl,
                                  LONG cx,
                                  LONG cy);
      BOOL APIENTRY WinInflateRect(HAB hab,
                                   PRECTL prcl,
                                   LONG cx,
                                   LONG cy);
      BOOL APIENTRY WinPtInRect(HAB hab,
                                PRECTL prcl,
                                PPOINTL pptl);
      BOOL APIENTRY WinIntersectRect(HAB hab,
                                     PRECTL prclDst,
                                     PRECTL prclSrc1,
                                     PRECTL prclSrc2);
      BOOL APIENTRY WinUnionRect(HAB hab,
                                 PRECTL prclDst,
                                 PRECTL prclSrc1,
                                 PRECTL prclSrc2);
      BOOL APIENTRY WinSubtractRect(HAB hab,
                                    PRECTL prclDst,
                                    PRECTL prclSrc1,
                                    PRECTL prclSrc2);
      BOOL APIENTRY WinMakeRect(HAB hab,
                                PRECTL pwrc);
      BOOL APIENTRY WinMakePoints(HAB hab,
                                  PPOINTL pwpt,
                                  ULONG cwpt);
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINRECTANGLES */


#ifdef INCL_WINSYS
   
   /*** System values */
   
   LONG APIENTRY WinQuerySysValue(HWND hwndDesktop,
                                  LONG iSysValue);
   BOOL  APIENTRY WinSetSysValue(HWND hwndDesktop,
                                 LONG iSysValue,
                                 LONG lValue);
   
   #define SV_SWAPBUTTON              0
   #define SV_DBLCLKTIME              1
   #define SV_CXDBLCLK                2
   #define SV_CYDBLCLK                3
   #define SV_CXSIZEBORDER            4
   #define SV_CYSIZEBORDER            5
   #define SV_ALARM                   6
   
   #ifndef INCL_SAADEFS
      #define SV_RESERVEDFIRST1          7
      #define SV_RESERVEDLAST1           8
   #endif /* !INCL_SAADEFS */
   
   #define SV_CURSORRATE              9
   #define SV_FIRSTSCROLLRATE         10
   #define SV_SCROLLRATE              11
   #define SV_NUMBEREDLISTS           12
   #define SV_WARNINGFREQ             13
   #define SV_NOTEFREQ                14
   #define SV_ERRORFREQ               15
   #define SV_WARNINGDURATION         16
   #define SV_NOTEDURATION            17
   #define SV_ERRORDURATION           18
   
   #ifndef INCL_SAADEFS
      #define SV_RESERVEDFIRST           19
      #define SV_RESERVEDLAST            19
   #endif /* !INCL_SAADEFS */
   
   #define SV_CXSCREEN                20
   #define SV_CYSCREEN                21
   #define SV_CXVSCROLL               22
   #define SV_CYHSCROLL               23
   #define SV_CYVSCROLLARROW          24
   #define SV_CXHSCROLLARROW          25
   #define SV_CXBORDER                26
   #define SV_CYBORDER                27
   #define SV_CXDLGFRAME              28
   #define SV_CYDLGFRAME              29
   #define SV_CYTITLEBAR              30
   #define SV_CYVSLIDER               31
   #define SV_CXHSLIDER               32
   #define SV_CXMINMAXBUTTON          33
   #define SV_CYMINMAXBUTTON          34
   #define SV_CYMENU                  35
   #define SV_CXFULLSCREEN            36
   #define SV_CYFULLSCREEN            37
   #define SV_CXICON                  38
   #define SV_CYICON                  39
   #define SV_CXPOINTER               40
   #define SV_CYPOINTER               41
   
   #define SV_DEBUG                   42
   #define SV_CMOUSEBUTTONS           43
   #define SV_CPOINTERBUTTONS         43
   #define SV_POINTERLEVEL            44
   #define SV_CURSORLEVEL             45
   #define SV_TRACKRECTLEVEL          46
   
   #ifndef INCL_SAADEFS
      #define SV_CTIMERS                 47
   #endif /* !INCL_SAADEFS */
   
   #define SV_MOUSEPRESENT            48
   
   #define SV_CXBYTEALIGN             49
   #define SV_CXALIGN                 49
   #define SV_CYBYTEALIGN             50
   #define SV_CYALIGN                 50
   
   /* The following value enables any greater value to be set by WinSetSysValue. */
   /* This is to enable the setting of SV_EXTRAKEYBEEP by applications.          */
   
   #define SV_NOTRESERVED             56
   #define SV_EXTRAKEYBEEP            57
   
   /* The following system value controls whether PM controls the keyboard      */
   /* lights for light key keystrokes (else applications will)                  */
   #define SV_SETLIGHTS               58
   #define SV_INSERTMODE              59
   
   
   #define SV_MENUROLLDOWNDELAY       64
   #define SV_MENUROLLUPDELAY         65
   #define SV_ALTMNEMONIC             66
   #define SV_TASKLISTMOUSEACCESS     67
   
   #define SV_CXICONTEXTWIDTH         68
   #define SV_CICONTEXTLINES          69
   
   #define SV_CHORDTIME               70
   #define SV_CXCHORD                 71
   #define SV_CYCHORD                 72
   #define SV_CXMOTIONSTART           73
   #define SV_CYMOTIONSTART           74
   
   #define SV_BEGINDRAG               75
   #define SV_ENDDRAG                 76
   #define SV_SINGLESELECT            77
   #define SV_OPEN                    78
   #define SV_CONTEXTMENU             79
   #define SV_CONTEXTHELP             80
   #define SV_TEXTEDIT                81
   #define SV_BEGINSELECT             82
   #define SV_ENDSELECT               83
   
   #define SV_BEGINDRAGKB             84
   #define SV_ENDDRAGKB               85
   #define SV_SELECTKB                86
   #define SV_OPENKB                  87
   #define SV_CONTEXTMENUKB           88
   #define SV_CONTEXTHELPKB           89
   #define SV_TEXTEDITKB              90
   #define SV_BEGINSELECTKB           91
   #define SV_ENDSELECTKB             92
   
   #define SV_ANIMATION               93
   #define SV_ANIMATIONSPEED          94
   
   #define SV_MONOICONS               95
   
   #define SV_KBDALTERED              96
   
   #define SV_PRINTSCREEN             97
   
   #define SV_LOCKSTARTINPUT          98
   
   #define SV_CSYSVALUES              108
   
   /*
   * Presentation parameter structures.
   */
   typedef struct _PARAM    /* param */
   {
      ULONG   id;
      ULONG   cb;
      BYTE    ab[1];
   } PARAM;
   typedef PARAM *NPPARAM;
   typedef PARAM *PPARAM;
   
   typedef struct _PRESPARAMS    /* pres */
   {
      ULONG   cb;
      PARAM   aparam[1];
   } PRESPARAMS;
   typedef PRESPARAMS *NPPRESPARAMS;
   typedef PRESPARAMS *PPRESPARAMS;
   
   /*
   * Presentation parameter APIs
   */
   BOOL  APIENTRY WinSetPresParam(HWND hwnd,
                                  ULONG id,
                                  ULONG cbParam,
                                  PVOID pbParam);
   ULONG APIENTRY WinQueryPresParam(HWND hwnd,
                                    ULONG id1,
                                    ULONG id2,
                                    PULONG pulId,
                                    ULONG cbBuf,
                                    PVOID pbBuf,
                                    ULONG fs);
   BOOL  APIENTRY WinRemovePresParam(HWND hwnd,
                                     ULONG id);
   
   /*
   * Presentation parameter types.
   */
   
   #define PP_FOREGROUNDCOLOR                      1L
   #define PP_FOREGROUNDCOLORINDEX                 2L
   #define PP_BACKGROUNDCOLOR                      3L
   #define PP_BACKGROUNDCOLORINDEX                 4L
   #define PP_HILITEFOREGROUNDCOLOR                5L
   #define PP_HILITEFOREGROUNDCOLORINDEX           6L
   #define PP_HILITEBACKGROUNDCOLOR                7L
   #define PP_HILITEBACKGROUNDCOLORINDEX           8L
   #define PP_DISABLEDFOREGROUNDCOLOR              9L
   #define PP_DISABLEDFOREGROUNDCOLORINDEX         10L
   #define PP_DISABLEDBACKGROUNDCOLOR              11L
   #define PP_DISABLEDBACKGROUNDCOLORINDEX         12L
   #define PP_BORDERCOLOR                          13L
   #define PP_BORDERCOLORINDEX                     14L
   #define PP_FONTNAMESIZE                         15L
   #define PP_FONTHANDLE                           16L
   #define PP_RESERVED                             17L
   #define PP_ACTIVECOLOR                          18L
   #define PP_ACTIVECOLORINDEX                     19L
   #define PP_INACTIVECOLOR                        20L
   #define PP_INACTIVECOLORINDEX                   21L
   #define PP_ACTIVETEXTFGNDCOLOR                  22L
   #define PP_ACTIVETEXTFGNDCOLORINDEX             23L
   #define PP_ACTIVETEXTBGNDCOLOR                  24L
   #define PP_ACTIVETEXTBGNDCOLORINDEX             25L
   #define PP_INACTIVETEXTFGNDCOLOR                26L
   #define PP_INACTIVETEXTFGNDCOLORINDEX           27L
   #define PP_INACTIVETEXTBGNDCOLOR                28L
   #define PP_INACTIVETEXTBGNDCOLORINDEX           29L
   #define PP_SHADOW                               30L
   #define PP_MENUFOREGROUNDCOLOR                  31L
   #define PP_MENUFOREGROUNDCOLORINDEX             32L
   #define PP_MENUBACKGROUNDCOLOR                  33L
   #define PP_MENUBACKGROUNDCOLORINDEX             34L
   #define PP_MENUHILITEFGNDCOLOR                  35L
   #define PP_MENUHILITEFGNDCOLORINDEX             36L
   #define PP_MENUHILITEBGNDCOLOR                  37L
   #define PP_MENUHILITEBGNDCOLORINDEX             38L
   #define PP_MENUDISABLEDFGNDCOLOR                39L
   #define PP_MENUDISABLEDFGNDCOLORINDEX           40L
   #define PP_MENUDISABLEDBGNDCOLOR                41L
   #define PP_MENUDISABLEDBGNDCOLORINDEX           42L
   #define PP_SHADOWTEXTCOLOR                      43L
   #define PP_SHADOWTEXTCOLORINDEX                 44L
   #define PP_SHADOWHILITEFGNDCOLOR                45L
   #define PP_SHADOWHILITEFGNDCOLORINDEX           46L
   #define PP_SHADOWHILITEBGNDCOLOR                47L
   #define PP_SHADOWHILITEBGNDCOLORINDEX           48L
   #define PP_ICONTEXTBACKGROUNDCOLOR              49L
   #define PP_ICONTEXTBACKGROUNDCOLORINDEX         50L
   #define PP_BORDERLIGHTCOLOR                     51L
   #define PP_BORDERDARKCOLOR                      52L
   #define PP_BORDER2COLOR                         53L
   #define PP_BORDER2LIGHTCOLOR                    54L
   #define PP_BORDER2DARKCOLOR                     55L
   #define PP_BORDERDEFAULTCOLOR                   56L
   #define PP_FIELDBACKGROUNDCOLOR                 57L
   #define PP_BUTTONBACKGROUNDCOLOR                58L
   #define PP_BUTTONBORDERLIGHTCOLOR               59L
   #define PP_BUTTONBORDERDARKCOLOR                60L
   #define PP_ARROWCOLOR                           61L
   #define PP_ARROWBORDERLIGHTCOLOR                62L
   #define PP_ARROWBORDERDARKCOLOR                 63L
   #define PP_ARROWDISABLEDCOLOR                   64L
   #define PP_CHECKLIGHTCOLOR                      65L
   #define PP_CHECKMIDDLECOLOR                     66L
   #define PP_CHECKDARKCOLOR                       67L
   #define PP_PAGEFOREGROUNDCOLOR                  68L
   #define PP_PAGEBACKGROUNDCOLOR                  69L
   #define PP_MAJORTABFOREGROUNDCOLOR              70L
   #define PP_MAJORTABBACKGROUNDCOLOR              71L
   #define PP_MINORTABFOREGROUNDCOLOR              72L
   #define PP_MINORTABBACKGROUNDCOLOR              73L
   
   /* PP_ values 0x100 - 0x012F are reserved*/
   /* for bidirectional language support.   */
   
   #define PP_BIDI_FIRST                           0x0100L
   #define PP_BIDI_LAST                            0x012FL
   
   #define PP_USER                                 0x8000L
   
   /*
   * Flags for WinQueryPresParams()
   */
   #define QPF_NOINHERIT            0x0001 /* Don't inherit                      */
   #define QPF_ID1COLORINDEX        0x0002 /* Convert id1 color index into RGB   */
   #define QPF_ID2COLORINDEX        0x0004 /* Convert id2 color index into RGB   */
   #define QPF_PURERGBCOLOR         0x0008 /* Return pure RGB colors             */
   #define QPF_VALIDFLAGS           0x000F /* Valid WinQueryPresParams() flags.  */
   
   /*** System color functions */
   
   LONG APIENTRY WinQuerySysColor(HWND hwndDesktop,
                                  LONG clr,
                                  LONG lReserved);
   BOOL APIENTRY WinSetSysColors(HWND hwndDesktop,
                                 ULONG flOptions,
                                 ULONG flFormat,
                                 LONG clrFirst,
                                 ULONG cclr,
                                 PLONG pclr);
   
   #define SYSCLR_SHADOWHILITEBGND         (-50L)
   #define SYSCLR_SHADOWHILITEFGND         (-49L)
   #define SYSCLR_SHADOWTEXT               (-48L)
   #define SYSCLR_ENTRYFIELD               (-47L)
   #define SYSCLR_MENUDISABLEDTEXT         (-46L)
   #define SYSCLR_MENUHILITE               (-45L)
   #define SYSCLR_MENUHILITEBGND           (-44L)
   #define SYSCLR_PAGEBACKGROUND           (-43L)
   #define SYSCLR_FIELDBACKGROUND          (-42L)
   #define SYSCLR_BUTTONLIGHT              (-41L)
   #define SYSCLR_BUTTONMIDDLE             (-40L)
   #define SYSCLR_BUTTONDARK               (-39L)
   #define SYSCLR_BUTTONDEFAULT            (-38L)
   #define SYSCLR_TITLEBOTTOM              (-37L)
   #define SYSCLR_SHADOW                   (-36L)
   #define SYSCLR_ICONTEXT                 (-35L)
   #define SYSCLR_DIALOGBACKGROUND         (-34L)
   #define SYSCLR_HILITEFOREGROUND         (-33L)
   #define SYSCLR_HILITEBACKGROUND         (-32L)
   #define SYSCLR_INACTIVETITLETEXTBGND    (-31L)
   #define SYSCLR_ACTIVETITLETEXTBGND      (-30L)
   #define SYSCLR_INACTIVETITLETEXT        (-29L)
   #define SYSCLR_ACTIVETITLETEXT          (-28L)
   #define SYSCLR_OUTPUTTEXT               (-27L)
   #define SYSCLR_WINDOWSTATICTEXT         (-26L)
   #define SYSCLR_SCROLLBAR                (-25L)
   #define SYSCLR_BACKGROUND               (-24L)
   #define SYSCLR_ACTIVETITLE              (-23L)
   #define SYSCLR_INACTIVETITLE            (-22L)
   #define SYSCLR_MENU                     (-21L)
   #define SYSCLR_WINDOW                   (-20L)
   #define SYSCLR_WINDOWFRAME              (-19L)
   #define SYSCLR_MENUTEXT                 (-18L)
   #define SYSCLR_WINDOWTEXT               (-17L)
   #define SYSCLR_TITLETEXT                (-16L)
   #define SYSCLR_ACTIVEBORDER             (-15L)
   #define SYSCLR_INACTIVEBORDER           (-14L)
   #define SYSCLR_APPWORKSPACE             (-13L)
   #define SYSCLR_HELPBACKGROUND           (-12L)
   #define SYSCLR_HELPTEXT                 (-11L)
   #define SYSCLR_HELPHILITE               (-10L)
   
   #define SYSCLR_CSYSCOLORS               41L
   
   /*
   *  Control color type constants
   */
   
   #define CCT_STATIC                      1L
   #define CCT_STATICTEXT                  2L
   #define CCT_GROUPBOX                    3L
   #define CCT_PUSHBUTTON                  4L
   #define CCT_CHECKBOX                    5L
   #define CCT_RADIOBUTTON                 6L
   #define CCT_ENTRYFIELD                  7L
   #define CCT_LISTBOX                     8L
   #define CCT_COMBOBOX                    9L
   #define CCT_SCROLLBAR                   10L
   #define CCT_FRAME                       11L
   #define CCT_MENU                        12L
   #define CCT_TITLEBAR                    13L
   #define CCT_SPINBUTTON                  14L
   #define CCT_SLIDER                      15L
   #define CCT_CIRCULARSLIDER              16L
   #define CCT_VALUESET                    17L
   #define CCT_MLE                         18L
   #define CCT_CONTAINER                   19L
   #define CCT_NOTEBOOK                    20L
   
   #define CCT_MAXTYPE                     20L
   
   /*
   *  Control color index constants
   */
   
   #define CCI_FOREGROUND                  1L
   #define CCI_FOREGROUNDREADONLY          2L
   #define CCI_BACKGROUND                  3L
   #define CCI_BACKGROUNDDIALOG            4L
   #define CCI_DISABLEDFOREGROUND          5L
   #define CCI_DISABLEDFOREGROUNDREADONLY  6L
   #define CCI_DISABLEDBACKGROUND          7L
   #define CCI_DISABLEDBACKGROUNDDIALOG    8L
   #define CCI_HIGHLIGHTFOREGROUND         9L
   #define CCI_HIGHLIGHTBACKGROUND         10L
   #define CCI_HIGHLIGHTBACKGROUNDDIALOG   11L
   #define CCI_INACTIVEFOREGROUND          12L
   #define CCI_INACTIVEFOREGROUNDDIALOG    13L
   #define CCI_INACTIVEBACKGROUND          14L
   #define CCI_INACTIVEBACKGROUNDTEXT      15L
   #define CCI_ACTIVEFOREGROUND            16L
   #define CCI_ACTIVEFOREGROUNDDIALOG      17L
   #define CCI_ACTIVEBACKGROUND            18L
   #define CCI_ACTIVEBACKGROUNDTEXT        19L
   #define CCI_PAGEBACKGROUND              20L
   #define CCI_PAGEFOREGROUND              21L
   #define CCI_FIELDBACKGROUND             22L
   #define CCI_BORDER                      23L
   #define CCI_BORDERLIGHT                 24L
   #define CCI_BORDERDARK                  25L
   #define CCI_BORDER2                     26L
   #define CCI_BORDER2LIGHT                27L
   #define CCI_BORDER2DARK                 28L
   #define CCI_BORDERDEFAULT               29L
   #define CCI_BUTTONBACKGROUND            30L
   #define CCI_BUTTONFOREGROUND            31L
   #define CCI_BUTTONBORDERLIGHT           32L
   #define CCI_BUTTONBORDERDARK            33L
   #define CCI_ARROW                       34L
   #define CCI_DISABLEDARROW               35L
   #define CCI_ARROWBORDERLIGHT            36L
   #define CCI_ARROWBORDERDARK             37L
   #define CCI_CHECKLIGHT                  38L
   #define CCI_CHECKMIDDLE                 39L
   #define CCI_CHECKDARK                   40L
   #define CCI_ICONFOREGROUND              41L
   #define CCI_ICONBACKGROUND              42L
   #define CCI_ICONBACKGROUNDDESKTOP       43L
   #define CCI_ICONHILITEFOREGROUND        44L
   #define CCI_ICONHILITEBACKGROUND        45L
   #define CCI_MAJORTABFOREGROUND          46L
   #define CCI_MAJORTABBACKGROUND          47L
   #define CCI_MINORTABFOREGROUND          48L
   #define CCI_MINORTABBACKGROUND          49L
   
   #define CCI_MAXINDEX                    49L
   
   /*
   *  Control color flags
   */
   
   #define CCF_GLOBAL                      0x0000
   #define CCF_APPLICATION                 0x0001
   #define CCF_COUNTCOLORS                 0x0010
   #define CCF_ALLCOLORS                   0x0020
   
   /*
   *  Control color values
   */
   
   #define CCV_NOTFOUND                    (-125L)
   #define CCV_IGNORE                      (-126L)
   #define CCV_DEFAULT                     (-127L)
   
   /*
   *  Control color structure
   */
   
   typedef struct _CTLCOLOR    /* ctlclr */
   {
      LONG clrIndex;
      LONG clrValue;
   } CTLCOLOR;
   
   typedef CTLCOLOR *PCTLCOLOR;
   
   /*
   *  Control color functions
   */
   
   LONG APIENTRY WinQueryControlColors(HWND hwnd,
                                       LONG clrType,
                                       ULONG flCtlColor,
                                       ULONG cCtlColor,
                                       PCTLCOLOR pCtlColor);
   
   LONG APIENTRY WinSetControlColors(HWND hwnd,
                                     LONG clrType,
                                     ULONG flCtlColor,
                                     ULONG cCtlColor,
                                     PCTLCOLOR pCtlColor);
   
   /*
   * Control color messages
   */
   
   #define WM_CTLCOLORCHANGE          0x0129
   #define WM_QUERYCTLTYPE            0x0130
   
#endif /* INCL_WINSYS */


#ifdef INCL_WINTIMER
   /**** Timer manager */
   
   #ifndef INCL_SAADEFS
      
      
      ULONG APIENTRY WinStartTimer(HAB hab,
                                   HWND hwnd,
                                   ULONG idTimer,
                                   ULONG dtTimeout);
      BOOL   APIENTRY WinStopTimer(HAB hab,
                                   HWND hwnd,
                                   ULONG idTimer);
      
      
      ULONG  APIENTRY WinGetCurrentTime(HAB hab);
      
      #define TID_CURSOR          0xffff  /* Reserved cursor timer ID              */
      #define TID_SCROLL          0xfffe  /* Reserved scrolling timer ID           */
      #define TID_FLASHWINDOW     0xfffd  /* Reserved for window flashing timer ID */
      #define TID_USERMAX         0x7fff  /* Maximum user timer ID                 */
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINTIMER */

typedef LHANDLE HACCEL; /* haccel */

#ifdef INCL_WINACCELERATORS
   /**** Accelerator functions */
   
   /* ACCEL fs bits
   *
   * NOTE: the first six AF_ code bits have the same value
   * as their KC_ counterparts
   */
   #ifndef INCL_SAADEFS
      #define AF_CHAR                    0x0001
      #define AF_VIRTUALKEY              0x0002
      #define AF_SCANCODE                0x0004
      #define AF_SHIFT                   0x0008
      #define AF_CONTROL                 0x0010
      #define AF_ALT                     0x0020
      #define AF_LONEKEY                 0x0040
      #define AF_SYSCOMMAND              0x0100
      #define AF_HELP                    0x0200
   #endif /* !INCL_SAADEFS */
   
   #pragma pack(2) /* force structure alignment packing */
   
   /* Binary resources may be bound into application executables or */
   /* passed as a parameter when creating a window or dialog        */
   /* Currently they must be the same for both 16-bit and 32-bit    */
   /* so we pack the structures.                                    */
   typedef struct _ACCELTABLE    /* acct  */
   {
      USHORT  cAccel;
      USHORT  codepage;
      ACCEL   aaccel[1];
   } ACCELTABLE;
   typedef ACCELTABLE *PACCELTABLE;
   
   #pragma pack()  /* restore packing to default */
   
   HACCEL APIENTRY WinLoadAccelTable(HAB hab,
                                     HMODULE hmod,
                                     ULONG idAccelTable);
   ULONG APIENTRY WinCopyAccelTable(HACCEL haccel,
                                    PACCELTABLE pAccelTable,
                                    ULONG cbCopyMax);
   HACCEL APIENTRY WinCreateAccelTable(HAB hab,
                                       PACCELTABLE pAccelTable);
   BOOL   APIENTRY WinDestroyAccelTable(HACCEL haccel);
   BOOL   APIENTRY WinTranslateAccel(HAB hab,
                                     HWND hwnd,
                                     HACCEL haccel,
                                     PQMSG pqmsg);
   BOOL   APIENTRY WinSetAccelTable(HAB hab,
                                    HACCEL haccel,
                                    HWND hwndFrame);
   HACCEL APIENTRY WinQueryAccelTable(HAB hab,
                                      HWND hwndFrame);
   
#endif /* INCL_WINACCELERATORS */

/**** Extended Attribute Flags (Association Table) */

#define EAF_DEFAULTOWNER           0x0001
#define EAF_UNCHANGEABLE           0x0002
#define EAF_REUSEICON              0x0004

/*** WinTrackRect() information */

#ifdef INCL_WINTRACKRECT
   
   /* WinTrackRect() tracking information structure */
   
   typedef struct _TRACKINFO    /* ti */
   {
      LONG    cxBorder;
      LONG    cyBorder;
      LONG    cxGrid;
      LONG    cyGrid;
      LONG    cxKeyboard;
      LONG    cyKeyboard;
      RECTL   rclTrack;
      RECTL   rclBoundary;
      POINTL  ptlMinTrackSize;
      POINTL  ptlMaxTrackSize;
      ULONG   fs;
   } TRACKINFO;
   typedef TRACKINFO *PTRACKINFO;
   
   #ifndef INCL_SAADEFS
      BOOL APIENTRY WinTrackRect(HWND hwnd,
                                 HPS hps,
                                 PTRACKINFO pti);
      BOOL APIENTRY WinShowTrackRect(HWND hwnd,
                                     BOOL fShow);
      
      /* WinTrackRect() flags */
      
      #define TF_LEFT                    0x0001
      #define TF_TOP                     0x0002
      #define TF_RIGHT                   0x0004
      #define TF_BOTTOM                  0x0008
      /* TF_MOVE = TF_LEFT | TF_TOP | TF_RIGHT | TF_BOTTOM */
      #define TF_MOVE                    0x000F
      
      #define TF_SETPOINTERPOS           0x0010
      #define TF_GRID                    0x0020
      #define TF_STANDARD                0x0040
      #define TF_ALLINBOUNDARY           0x0080
      #define TF_VALIDATETRACKRECT       0x0100
      #define TF_PARTINBOUNDARY          0x0200
      
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINTRACKRECT */


/**** Clipboard Manager */

#if (defined(INCL_WINCLIPBOARD) || defined(INCL_WINDDE))
   #ifndef INCL_SAADEFS
      
      /* Clipboard messages */
      
      #define WM_RENDERFMT               0x0060
      #define WM_RENDERALLFMTS           0x0061
      #define WM_DESTROYCLIPBOARD        0x0062
      #define WM_PAINTCLIPBOARD          0x0063
      #define WM_SIZECLIPBOARD           0x0064
      #define WM_HSCROLLCLIPBOARD        0x0065
      #define WM_VSCROLLCLIPBOARD        0x0066
      #define WM_DRAWCLIPBOARD           0x0067
      
      /*
      * Standard Clipboard formats
      */
      #define CF_TEXT                    1
      #define CF_BITMAP                  2
      #define CF_DSPTEXT                 3
      #define CF_DSPBITMAP               4
      #define CF_METAFILE                5
      #define CF_DSPMETAFILE             6
      #define CF_PALETTE                 9
      #define CF_MMPMFIRST              10
      #define CF_MMPMLAST               19
      /*
      * standard DDE and clipboard format stings
      */
      #define SZFMT_TEXT                   "#1"
      #define SZFMT_BITMAP                 "#2"
      #define SZFMT_DSPTEXT                "#3"
      #define SZFMT_DSPBITMAP              "#4"
      #define SZFMT_METAFILE               "#5"
      #define SZFMT_DSPMETAFILE            "#6"
      #define SZFMT_PALETTE                "#9"
      #define SZFMT_SYLK                   "Sylk"
      #define SZFMT_DIF                    "Dif"
      #define SZFMT_TIFF                   "Tiff"
      #define SZFMT_OEMTEXT                "OemText"
      #define SZFMT_DIB                    "Dib"
      #define SZFMT_OWNERDISPLAY           "OwnerDisplay"
      #define SZFMT_LINK                   "Link"
      #define SZFMT_METAFILEPICT           "MetaFilePict"
      #define SZFMT_DSPMETAFILEPICT        "DspMetaFilePict"
      #define SZFMT_CPTEXT                 "Codepage Text"
      #define SZDDEFMT_RTF                 "Rich Text Format"
      #define SZDDEFMT_PTRPICT             "Printer_Picture"
      
      #pragma pack(2) /* force structure alignment packing */
      
      /* Metafiles are the same no matter whether they are used in     */
      /* 16-bit or 32-bit applications. While not strictly necessary   */
      /* because of inherent alignment we force packing on a 2 byte    */
      /* boundary to avoid any confusion.                              */
      typedef struct _MFP    /* mfp */
      {
         POINTL sizeBounds;  /* metafile notional grid size      */
         POINTL sizeMM;      /* metafile size high metric units  */
         ULONG  cbLength;    /* length of metafile data          */
         USHORT mapMode;     /* a PM metaflie map mode           */
         USHORT reserved;
         BYTE   abData[1];   /* metafile Data                    */
      } MFP;
      typedef MFP *PMFP;
      
      /* CPTEXT DDE data are the same no matter whether they are used  */
      /* in 16-bit or 32-bit applications. While not strictly necessary*/
      /* because of inherent alignment we force packing on a 2 byte    */
      /* boundary to avoid any confusion.                              */
      typedef struct _CPTEXT    /* cptxt */
      {
         USHORT idCountry;
         USHORT usCodepage;
         USHORT usLangID;
         USHORT usSubLangID;
         BYTE   abText[1];   /* text string starts here          */
      } CPTEXT;
      typedef CPTEXT *PCPTEXT;
      
      #pragma pack()  /* restore packing to default */
      
   #endif /* not INCL_SAADEFS */
#endif /* INCL_WINCLIPBOARD | INCL_WINDDE */

#ifdef INCL_WINCLIPBOARD
   #ifndef INCL_SAADEFS
      BOOL   APIENTRY WinSetClipbrdOwner(HAB hab,
                                         HWND hwnd);
      BOOL   APIENTRY WinSetClipbrdData(HAB hab,
                                        ULONG ulData,
                                        ULONG fmt,
                                        ULONG rgfFmtInfo);
      ULONG  APIENTRY WinQueryClipbrdData(HAB hab,
                                          ULONG fmt);
      BOOL   APIENTRY WinQueryClipbrdFmtInfo(HAB hab,
                                             ULONG fmt,
                                             PULONG prgfFmtInfo);
      BOOL   APIENTRY WinSetClipbrdViewer(HAB hab,
                                          HWND hwndNewClipViewer);
      
      /* WinSetClipbrdData() flags */
      
      #define CFI_OWNERFREE              0x0001
      #define CFI_OWNERDISPLAY           0x0002
      #define CFI_POINTER                0x0400
      #define CFI_HANDLE                 0x0200
      
   #endif /* !INCL_SAADEFS */
   
   ULONG APIENTRY WinEnumClipbrdFmts(HAB hab,
                                     ULONG fmt);
   BOOL   APIENTRY WinEmptyClipbrd(HAB hab);
   BOOL   APIENTRY WinOpenClipbrd(HAB hab);
   BOOL   APIENTRY WinCloseClipbrd(HAB hab);
   
   HWND   APIENTRY WinQueryClipbrdOwner(HAB hab);
   HWND   APIENTRY WinQueryClipbrdViewer(HAB hab);
   
#endif /* INCL_WINCLIPBOARD */


#if (defined(INCL_WINCURSORS) || !defined(INCL_NOCOMMON))
   /**** Cursor manager common subsection */
   
   BOOL APIENTRY WinDestroyCursor(HWND hwnd);
   BOOL APIENTRY WinShowCursor(HWND hwnd,
                               BOOL fShow);
   BOOL APIENTRY WinCreateCursor(HWND hwnd,
                                 LONG x,
                                 LONG y,
                                 LONG cx,
                                 LONG cy,
                                 ULONG fs,
                                 PRECTL prclClip);
   
   /* WinCreateCursor() flags */
   
   #define CURSOR_SOLID               0x0000
   #define CURSOR_HALFTONE            0x0001
   #define CURSOR_FRAME               0x0002
   #define CURSOR_FLASH               0x0004
   #define CURSOR_SETPOS              0x8000
   
   /* Cursor flags 0x0100 and 0x0200 reserved */
   /* for bidirectional language support.     */
   
   #define CURSOR_BIDI_FIRST          0x0100
   #define CURSOR_BIDI_LAST           0x0200
   
   
#endif /* Cursor manager common subsection */

#ifdef INCL_WINCURSORS
   
   typedef struct _CURSORINFO    /* csri */
   {
      HWND    hwnd;
      LONG    x;
      LONG    y;
      LONG    cx;
      LONG    cy;
      ULONG   fs;
      RECTL   rclClip;
   } CURSORINFO;
   typedef CURSORINFO *PCURSORINFO;
   
   BOOL APIENTRY WinQueryCursorInfo(HWND hwndDesktop,
                                    PCURSORINFO pCursorInfo);
   
#endif /* INCL_WINCURSORS */


#ifdef INCL_WINPOINTERS
   /**** Pointer manager */
   
   BOOL     APIENTRY WinSetPointer(HWND hwndDesktop,
                                   HPOINTER hptrNew);
   BOOL     APIENTRY WinSetPointerOwner(HPOINTER hptr,
                                        PID pid,
                                        BOOL fDestroy);
   BOOL     APIENTRY WinShowPointer(HWND hwndDesktop,
                                    BOOL fShow);
   BOOL     APIENTRY WinLockPointerUpdate(HWND hwndDesktop,
                                          HPOINTER hptrNew,
                                          ULONG ulTimeInterval);
   HPOINTER APIENTRY WinQuerySysPointer(HWND hwndDesktop,
                                        LONG iptr,
                                        BOOL fLoad);
   
   BOOL APIENTRY WinQuerySysPointerData(HWND hwndDesktop,
                                        ULONG iptr,
                                        PICONINFO pIconInfo);
   BOOL APIENTRY WinSetSysPointerData(HWND hwndDesktop,
                                      ULONG iptr,
                                      PICONINFO pIconInfo);
   
   /* System pointers (NOTE: these are 1-based) */
   
   #define SPTR_ARROW                 1
   #define SPTR_TEXT                  2
   #define SPTR_WAIT                  3
   #define SPTR_SIZE                  4
   #define SPTR_MOVE                  5
   #define SPTR_SIZENWSE              6
   #define SPTR_SIZENESW              7
   #define SPTR_SIZEWE                8
   #define SPTR_SIZENS                9
   #define SPTR_APPICON               10
   
   #define SPTR_ICONINFORMATION       11
   #define SPTR_ICONQUESTION          12
   #define SPTR_ICONERROR             13
   #define SPTR_ICONWARNING           14
   
   #define SPTR_ILLEGAL               18
   #define SPTR_FILE                  19
   #define SPTR_FOLDER                20
   #define SPTR_MULTFILE              21
   #define SPTR_PROGRAM               22
   #define SPTR_DISPLAY_PTRS          22
   #define SPTR_PENFIRST              23
   #define SPTR_PENLAST               39
   #define SPTR_CPTR                  39  /* total # of system pointers */
   
   /* backward compatibility */
   #define SPTR_HANDICON         SPTR_ICONERROR
   #define SPTR_QUESICON         SPTR_ICONQUESTION
   #define SPTR_BANGICON         SPTR_ICONWARNING
   #define SPTR_NOTEICON         SPTR_ICONINFORMATION
   
   
   HPOINTER APIENTRY WinLoadPointer(HWND hwndDesktop,
                                    HMODULE hmod,
                                    ULONG idres);
   HPOINTER APIENTRY WinCreatePointer(HWND hwndDesktop,
                                      HBITMAP hbmPointer,
                                      BOOL fPointer,
                                      LONG xHotspot,
                                      LONG yHotspot);
   BOOL     APIENTRY WinSetPointerPos(HWND hwndDesktop,
                                      LONG x,
                                      LONG y);
   BOOL     APIENTRY WinDestroyPointer(HPOINTER hptr);
   
   HPOINTER APIENTRY WinQueryPointer(HWND hwndDesktop);
   BOOL     APIENTRY WinQueryPointerPos(HWND hwndDesktop,
                                        PPOINTL pptl);
   
   typedef struct _POINTERINFO    /* ptri */
   {
      ULONG   fPointer;
      LONG    xHotspot;
      LONG    yHotspot;
      HBITMAP hbmPointer;
      HBITMAP hbmColor;
      HBITMAP hbmMiniPointer;
      HBITMAP hbmMiniColor;
   } POINTERINFO;
   typedef POINTERINFO *PPOINTERINFO;
   
   HPOINTER APIENTRY WinCreatePointerIndirect(HWND hwndDesktop,
                                              PPOINTERINFO pptri);
   BOOL     APIENTRY WinQueryPointerInfo(HPOINTER hptr,
                                         PPOINTERINFO pPointerInfo);
   BOOL     APIENTRY WinDrawPointer(HPS hps,
                                    LONG x,
                                    LONG y,
                                    HPOINTER hptr,
                                    ULONG fs);
   
   
   
   /* WinDrawPointer() constants */
   
   #define DP_NORMAL                  0x0000
   #define DP_HALFTONED               0x0001
   #define DP_INVERTED                0x0002
   #define DP_MINI                    0x0004
   
   
   HBITMAP APIENTRY WinGetSysBitmap(HWND hwndDesktop,
                                    ULONG ibm);
   
   /* System bitmaps (NOTE: these are 1-based) */
   
   #define SBMP_OLD_SYSMENU           1
   #define SBMP_OLD_SBUPARROW         2
   #define SBMP_OLD_SBDNARROW         3
   #define SBMP_OLD_SBRGARROW         4
   #define SBMP_OLD_SBLFARROW         5
   #define SBMP_MENUCHECK             6
   #define SBMP_OLD_CHECKBOXES        7
   #define SBMP_BTNCORNERS            8
   #define SBMP_OLD_MINBUTTON         9
   #define SBMP_OLD_MAXBUTTON         10
   #define SBMP_OLD_RESTOREBUTTON     11
   #define SBMP_OLD_CHILDSYSMENU      12
   #define SBMP_DRIVE                 15
   #define SBMP_FILE                  16
   #define SBMP_FOLDER                17
   #define SBMP_TREEPLUS              18
   #define SBMP_TREEMINUS             19
   #define SBMP_PROGRAM               22
   #define SBMP_MENUATTACHED          23
   #define SBMP_SIZEBOX               24
   
   #define SBMP_SYSMENU               25
   #define SBMP_MINBUTTON             26
   #define SBMP_MAXBUTTON             27
   #define SBMP_RESTOREBUTTON         28
   #define SBMP_CHILDSYSMENU          29
   #define SBMP_SYSMENUDEP            30
   #define SBMP_MINBUTTONDEP          31
   #define SBMP_MAXBUTTONDEP          32
   #define SBMP_RESTOREBUTTONDEP      33
   #define SBMP_CHILDSYSMENUDEP       34
   #define SBMP_SBUPARROW             35
   #define SBMP_SBDNARROW             36
   #define SBMP_SBLFARROW             37
   #define SBMP_SBRGARROW             38
   #define SBMP_SBUPARROWDEP          39
   #define SBMP_SBDNARROWDEP          40
   #define SBMP_SBLFARROWDEP          41
   #define SBMP_SBRGARROWDEP          42
   #define SBMP_SBUPARROWDIS          43
   #define SBMP_SBDNARROWDIS          44
   #define SBMP_SBLFARROWDIS          45
   #define SBMP_SBRGARROWDIS          46
   #define SBMP_COMBODOWN             47
   #define SBMP_CHECKBOXES            48
   #define SBMP_HIDE                  50
   #define SBMP_HIDEDEP               51
   #define SBMP_CLOSE                 52
   #define SBMP_CLOSEDEP              53
   
#endif /* INCL_WINPOINTERS */


/**** Hook manager */

#ifdef INCL_WINHOOKS
   
   #ifndef INCL_SAADEFS
      BOOL APIENTRY WinSetHook(HAB hab,
                               HMQ hmq,
                               LONG iHook,
                               PFN pfnHook,
                               HMODULE hmod);
      BOOL APIENTRY WinReleaseHook(HAB hab,
                                   HMQ hmq,
                                   LONG iHook,
                                   PFN pfnHook,
                                   HMODULE hmod);
      BOOL APIENTRY WinCallMsgFilter(HAB hab,
                                     PQMSG pqmsg,
                                     ULONG msgf);
      
      
      /**********************************************************************
      * Hook codes
      *
      * Whether the hook receives a (U)SHORT or (U)LONG parameter depends
      * on the memory model that the hook executes in. 32-bit hook will
      * receive long parameters
      *
      **********************************************************************/
      
      #define HK_SENDMSG                 0
      /* VOID EXPENTRY SendMsgHook(HAB hab,               ** installer's hab
      *                            PSMHSTRUCT psmh,       ** p send msg struct
      *                            BOOL fInterTask);      ** between threads */
      
      #define HK_INPUT                   1
      /* BOOL EXPENTRY InputHook(HAB hab,                 ** installer's hab
      *                          PQMSG pQmsg,             ** p qmsg
      *                          ULONG/USHORT fs);        ** remove/noremove */
      
      #define HK_MSGFILTER               2
      /* BOOL EXPENTRY MsgFilterHook(HAB hab,             ** installer's hab
      *                              PQMSG pQmsg,         ** p qmsg
      *                              ULONG/USHORT msgf);  ** filter flag */
      
      #define HK_JOURNALRECORD           3
      /* VOID EXPENTRY JournalRecordHook(HAB hab,         ** installer's hab
      *                                  PQMSG pQmsg);    ** p qmsg */
      
      #define HK_JOURNALPLAYBACK         4
      /* ULONG EXPENTRY JournalPlaybackHook(HAB hab,      **installer's hab
      *                                     BOOL fSkip,   ** skip messages
      *                                     PQMSG pQmsg); ** p qmsg */
      
      #define HK_HELP                    5
      /* BOOL EXPENTRY HelpHook(HAB hab,                  ** installer's hab
      *                         ULONG/USHORT usMode,      ** mode
      *                         ULONG/USHORT idTopic,     ** main topic
      *                         ULONG/USHORT idSubTopic,  ** sub topic
      *                         PRECTL prcPosition);      ** associated position */
      
      #define HK_LOADER                  6
      /* BOOL EXPENTRY LoaderHook(HAB hab,                ** installer's hab
      *                           LONG/USHORT idContext,  ** who called hook
      *                           PSZ pszLibname,         ** lib name string
      *                           PHLIB hlib,             ** p to lib handle
      *                           PSZ pszProcname,        ** procedure name
      *                           PFNWP wndProc,          ** window procedure
      *                           PBOOL pfSuccess);       ** success indicator */
      
      #define HK_REGISTERUSERMSG         7
      /* BOOL EXPENTRY RegisterUserHook(HAB hab,              ** installer's hab
      *                                 LONG/SHORT idContext, ** who called hook
      *                                 ULONG/USHORT msg,     ** message
      *                                 LONG/SHORT type1,     ** data type
      *                                 LONG/SHORT dir1,      ** msg direction
      *                                 LONG/SHORT type2,     ** data type
      *                                 LONG/SHORT dir2,      ** msg direction
      *                                 LONG/SHORT typer,     ** result data type
      *                                 LONG/SHORT cUshort,   ** number elements
      *                                 PLONG/PSHORT arRMP,   ** element data types
      *                                 PBOOL fRegistered);   ** msg parms already reg*/
      
      #define HK_MSGCONTROL              8
      /* BOOL EXPENTRY MsgControlHook(HAB hab,            ** installer's hab
      *                               LONG/SHORT idContext,** who called hook
      *                               HWND hwnd,          ** SEI window handle
      *                               PSZ pszClassname,   ** window class name
      *                               ULONG/USHORT usMsgclass,** interested msg class **
      *                               LONG/SHORT idControl,** SMI_*
      *                               PBOOL fSuccess);    ** mode already set */
      
      #define HK_PLIST_ENTRY             9
      /* BOOL EXPENTRY ProgramListEntryHook(HAB hab,      ** installer's hab
      *                                     PPRFHOOKPARMS pProfileHookParams,** data
      *                                     PBOOL fNoExecute);  ** cease hook processing*/
      
      #define HK_PLIST_EXIT              10
      /* BOOL EXPENTRY ProgramListExitHook(HAB hab,       ** installer's hab
      *                                    PPRFHOOKPARMS pProfileHookParams);** data*/
      
      #define HK_FINDWORD                11
      /* BOOL EXPENTRY FindWordHook(USHORT usCodepage,    ** code page to use
      *                             PSZ pszText,          ** text to break
      *                             ULONG cb,             ** maximum text size
      *                             ULONG ich,            ** break 'near' here
      *                             PULONG pichStart,     ** where break began
      *                             PULONG pichEnd,       ** where break ended
      *                             PULONG pichNext);     ** where next word begin*/
      
      #define HK_CODEPAGECHANGED         12
      /* VOID EXPENTRY CodePageChangedHook(HMQ hmq,       ** msg q handle
      *                                    ULONG/USHORT usOldCodepage, ** old code page
      *                                    ULONG/USHORT usNewCodepage);** new code page*/
      
      #define HK_WINDOWDC                15
      /* BOOL EXPENTRY WindowDCHook(HAB  hab,             ** installer's hab
      *                             HDC  hdc,             ** current hdc
      *                             HWND hwnd,            ** current hwnd
      *                             BOOL);                ** association flag */
      
      #define HK_DESTROYWINDOW           16
      /* BOOL EXPENTRY DestroyWindowHook (HAB  hab,       ** installer's hab
      *                                   HWND hwnd,      ** destroyed win hwnd
      *                                   ULONG Reserved);** association flag  */
      
      #define HK_CHECKMSGFILTER          20
      /* BOOL EXPENTRY CheckMsgFilterHook(HAB  hab,       ** installer's hab
      *                                   PQMSG pQmsg,    ** p qmsg
      *                                   ULONG/USHORT usFirst,  ** first msg
      *                                   ULONG/USHORT usLast,   ** last msg
      *                                   ULONG/USHORT fOptions);** flags  */
      
      #define HK_MSGINPUT                21
      /* BOOL EXPENTRY MsgInputHook (HAB  hab,            ** installer's hab
      *                              PQMSG pQmsg,         ** p qmsg
      *                              BOOL fSkip,          ** skip msg
      *                              PBOOL pfNoRecord);   ** journal record event */
      
      #define HK_LOCKUP                  23
      /* BOOL EXPENTRY LockupHook (HAB  hab,              ** installer's hab
      *                            HWND hwndLockupFrame); ** hwnd     */
      
      #define HK_FLUSHBUF                24
      /* BOOL EXPENTRY FlushBufHook (HAB  hab);           ** installer's hab */
      
      /*** Current message queue constant **********************************/
      #define HMQ_CURRENT          ((HMQ)1)
      
      /*** WH_MSGFILTER context codes **************************************/
      #define MSGF_DIALOGBOX             1
      #define MSGF_MESSAGEBOX            2
      #define MSGF_TRACK                 8
      #define MSGF_DDEPOSTMSG            3
      
      /*** HK_HELP Help modes **********************************************/
      #define HLPM_FRAME              (-1)
      #define HLPM_WINDOW             (-2)
      #define HLPM_MENU               (-3)
      
      /*** HK_SENDMSG structure ********************************************/
      #define PM_MODEL_1X      0
      #define PM_MODEL_2X      1
      
      typedef struct _SMHSTRUCT      /* smhs */
      {
         MPARAM mp2;
         MPARAM mp1;
         ULONG  msg;
         HWND   hwnd;
         ULONG  model;
      } SMHSTRUCT;
      typedef SMHSTRUCT *PSMHSTRUCT;
      
      /*HK_LOADER context codes */
      
      #define LHK_DELETEPROC             1
      #define LHK_DELETELIB              2
      #define LHK_LOADPROC               3
      #define LHK_LOADLIB                4
      
      /*HK_MSGCONTROL context codes */
      
      #define MCHK_MSGINTEREST           1
      #define MCHK_CLASSMSGINTEREST      2
      #define MCHK_SYNCHRONISATION       3
      #define MCHK_MSGMODE               4
      
      /*HK_REGISTERUSERMSG conext codes */
      
      #define RUMHK_DATATYPE             1
      #define RUMHK_MSG                  2
      
   #endif /* INCL_SAADEFS */
   
#endif /* INCL_WINHOOKS */


#ifdef INCL_WINTHUNKAPI
   
#ifdef __cplusplus
   BOOL    APIENTRY WinSetClassThunkProc(PCSZ  pszClassname,
                                         PFN pfnThunkProc);
#else
   BOOL    APIENTRY WinSetClassThunkProc(PSZ pszClassname,
                                         PFN pfnThunkProc);
#endif
#ifdef __cplusplus
   PFN     APIENTRY WinQueryClassThunkProc(PCSZ  pszClassname);
#else
   PFN     APIENTRY WinQueryClassThunkProc(PSZ pszClassname);
#endif
   BOOL    APIENTRY WinSetWindowThunkProc(HWND hwnd,
                                          PFN pfnThunkProc);
   PFN     APIENTRY WinQueryWindowThunkProc(HWND hwnd);
   LONG   APIENTRY WinQueryWindowModel(HWND hwnd);
   
#endif /* INCL_WINTHUNKAPI */

/*
* Include Shell API
*/
#ifndef INCL_SAADEFS
   #include <pmshl.h>      /* OS/2 Shell definitions */
#endif /* !INCL_SAADEFS */

#ifdef INCL_WINCOUNTRY
   
   ULONG  APIENTRY WinQueryCp(HMQ hmq);
   
   #ifndef INCL_SAADEFS
      BOOL    APIENTRY WinSetCp(HMQ hmq,
                                ULONG idCodePage);
      
      ULONG  APIENTRY WinQueryCpList(HAB hab,
                                     ULONG ccpMax,
                                     PULONG prgcp);
      
#ifdef __cplusplus
      BOOL    APIENTRY WinCpTranslateString(HAB hab,
                                            ULONG cpSrc,
                                            PCSZ  pszSrc,
                                            ULONG cpDst,
                                            ULONG cchDestMax,
                                            PCSZ  pchDest);
#else
      BOOL    APIENTRY WinCpTranslateString(HAB hab,
                                            ULONG cpSrc,
                                            PSZ pszSrc,
                                            ULONG cpDst,
                                            ULONG cchDestMax,
                                            PSZ pchDest);
#endif
      
      UCHAR   APIENTRY WinCpTranslateChar(HAB hab,
                                          ULONG cpSrc,
                                          UCHAR chSrc,
                                          ULONG cpDst);
      
#ifdef __cplusplus
      ULONG  APIENTRY WinUpper(HAB hab,
                               ULONG idcp,
                               ULONG idcc,
                               PCSZ  psz);
#else
      ULONG  APIENTRY WinUpper(HAB hab,
                               ULONG idcp,
                               ULONG idcc,
                               PSZ psz);
#endif
      
      ULONG  APIENTRY WinUpperChar(HAB hab,
                                   ULONG idcp,
                                   ULONG idcc,
                                   ULONG c);
      
#ifdef __cplusplus
      PCSZ      APIENTRY WinNextChar(HAB hab,
                                   ULONG idcp,
                                   ULONG idcc,
                                   PCSZ  psz);
#else
      PSZ     APIENTRY WinNextChar(HAB hab,
                                   ULONG idcp,
                                   ULONG idcc,
                                   PSZ psz);
#endif
      
#ifdef __cplusplus
      PCSZ      APIENTRY WinPrevChar(HAB hab,
                                   ULONG idcp,
                                   ULONG idcc,
                                   PCSZ  pszStart,
                                   PCSZ  psz);
#else
      PSZ     APIENTRY WinPrevChar(HAB hab,
                                   ULONG idcp,
                                   ULONG idcc,
                                   PSZ pszStart,
                                   PSZ psz);
#endif
      
#ifdef __cplusplus
      ULONG  APIENTRY WinCompareStrings(HAB hab,
                                        ULONG idcp,
                                        ULONG idcc,
                                        PCSZ  psz1,
                                        PCSZ  psz2,
                                        ULONG reserved);
#else
      ULONG  APIENTRY WinCompareStrings(HAB hab,
                                        ULONG idcp,
                                        ULONG idcc,
                                        PSZ psz1,
                                        PSZ psz2,
                                        ULONG reserved);
#endif
      
      #define WCS_ERROR                  0
      #define WCS_EQ                     1
      #define WCS_LT                     2
      #define WCS_GT                     3
      
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINCOUNTRY */



/* Heap Manager Interface declarations */

#ifdef INCL_WINHEAP
   
   
#endif  /* INCL_WINHEAP */


/*** Atom Manager Interface declarations */

#ifdef INCL_WINATOM
   
   #ifndef INCL_SAADEFS
      typedef LHANDLE  HATOMTBL;
      typedef ULONG   ATOM;
      
      HATOMTBL APIENTRY WinQuerySystemAtomTable(VOID);
      HATOMTBL APIENTRY WinCreateAtomTable(ULONG cbInitial,
                                           ULONG cBuckets);
      HATOMTBL APIENTRY WinDestroyAtomTable(HATOMTBL hAtomTbl);
#ifdef __cplusplus
      ATOM     APIENTRY WinAddAtom(HATOMTBL hAtomTbl,
                                   PCSZ  pszAtomName);
#else
      ATOM     APIENTRY WinAddAtom(HATOMTBL hAtomTbl,
                                   PSZ pszAtomName);
#endif
#ifdef __cplusplus
      ATOM     APIENTRY WinFindAtom(HATOMTBL hAtomTbl,
                                    PCSZ  pszAtomName);
#else
      ATOM     APIENTRY WinFindAtom(HATOMTBL hAtomTbl,
                                    PSZ pszAtomName);
#endif
      ATOM     APIENTRY WinDeleteAtom(HATOMTBL hAtomTbl,
                                      ATOM atom);
      ULONG   APIENTRY WinQueryAtomUsage(HATOMTBL hAtomTbl,
                                         ATOM atom);
      ULONG   APIENTRY WinQueryAtomLength(HATOMTBL hAtomTbl,
                                          ATOM atom);
#ifdef __cplusplus
      ULONG   APIENTRY WinQueryAtomName(HATOMTBL hAtomTbl,
                                        ATOM atom,
                                        PCSZ  pchBuffer,
                                        ULONG cchBufferMax);
#else
      ULONG   APIENTRY WinQueryAtomName(HATOMTBL hAtomTbl,
                                        ATOM atom,
                                        PSZ pchBuffer,
                                        ULONG cchBufferMax);
#endif
      
      #define MAKEINTATOM(a)  ((PCH)MAKEULONG(a, 0xffff))
   #endif /* !INCL_SAADEFS */
   
#endif /* INCL_WINATOM */

/*** Window Property Declarations */


#ifdef INCL_WINERRORS
   #include <pmerr.h>
   
   /* Error codes for debugging support                                       */
   /* 0x1001 - 0x1021, 0x1034, 0x1036 - 0x1050 are reserved                   */
   
   #define WINDBG_HWND_NOT_DESTROYED           0x1022
   #define WINDBG_HPTR_NOT_DESTROYED           0x1023
   #define WINDBG_HACCEL_NOT_DESTROYED         0x1024
   #define WINDBG_HENUM_NOT_DESTROYED          0x1025
   #define WINDBG_VISRGN_SEM_BUSY              0x1026
   #define WINDBG_USER_SEM_BUSY                0x1027
   #define WINDBG_DC_CACHE_BUSY                0x1028
   #define WINDBG_HOOK_STILL_INSTALLED         0x1029
   #define WINDBG_WINDOW_STILL_LOCKED          0x102a
   #define WINDBG_UPDATEPS_ASSERTION_FAIL      0x102b
   #define WINDBG_SENDMSG_WITHIN_USER_SEM      0x102c
   #define WINDBG_USER_SEM_NOT_ENTERED         0x102d
   #define WINDBG_PROC_NOT_EXPORTED            0x102e
   #define WINDBG_BAD_SENDMSG_HWND             0x102f
   #define WINDBG_ABNORMAL_EXIT                0x1030
   #define WINDBG_INTERNAL_REVISION            0x1031
   #define WINDBG_INITSYSTEM_FAILED            0x1032
   #define WINDBG_HATOMTBL_NOT_DESTROYED       0x1033
   #define WINDBG_WINDOW_UNLOCK_WAIT           0x1035
   
   /* Get/Set Error Information Interface declarations */
   
   typedef struct _ERRINFO    /* erri */
   {
      ULONG   cbFixedErrInfo;
      ERRORID idError;
      ULONG   cDetailLevel;
      ULONG   offaoffszMsg;
      ULONG   offBinaryData;
   } ERRINFO;
   typedef ERRINFO *PERRINFO;
   
   ERRORID     APIENTRY    WinGetLastError(HAB hab);
   PERRINFO    APIENTRY    WinGetErrorInfo(HAB hab);
   BOOL        APIENTRY    WinFreeErrorInfo(PERRINFO perrinfo);
   
#endif  /* INCL_WINERRORS */

#ifndef INCL_SAADEFS
   /* include WinSetErrorInfo */
   #ifdef INCL_WINSEI
      #ifndef SEI_PMWINP
         #define SEI_PMWIN
         #include <pmsei.h>
      #endif
   #endif
#endif

#ifndef INCL_SAADEFS
   #ifdef  INCL_WINDDE
      
      /*
      * DDE standard system topic and item strings
      */
      #define SZDDESYS_TOPIC                  "System"
      #define SZDDESYS_ITEM_TOPICS            "Topics"
      #define SZDDESYS_ITEM_SYSITEMS          "SysItems"
      #define SZDDESYS_ITEM_RTNMSG            "ReturnMessage"
      #define SZDDESYS_ITEM_STATUS            "Status"
      #define SZDDESYS_ITEM_FORMATS           "Formats"
      #define SZDDESYS_ITEM_SECURITY          "Security"
      #define SZDDESYS_ITEM_ITEMFORMATS       "ItemFormats"
      #define SZDDESYS_ITEM_HELP              "Help"
      #define SZDDESYS_ITEM_PROTOCOLS         "Protocols"
      #define SZDDESYS_ITEM_RESTART           "Restart"
      #define SZDDESYS_ITEM_ITEMLIST          "TopicItemList"
      
      /*
      * Dynamic Data Exchange (DDE) Structure Declarations
      */
      typedef struct _CONVCONTEXT    /* cctxt */
      {
         ULONG  cb;          /* sizeof(CONVCONTEXT) */
         ULONG fsContext;
         ULONG idCountry;
         ULONG usCodepage;
         ULONG usLangID;
         ULONG usSubLangID;
      } CONVCONTEXT;
      typedef CONVCONTEXT *PCONVCONTEXT;
      
      #define DDECTXT_CASESENSITIVE    0x0001
      
      typedef struct _DDEINIT    /* ddei */
      {
         ULONG   cb;         /* sizeof(DDEINIT) */
         PSZ     pszAppName;
         PSZ     pszTopic;
         ULONG   offConvContext;
      } DDEINIT;
      typedef DDEINIT *PDDEINIT;
      
      typedef struct _DDESTRUCT    /* dde */
      {
         ULONG   cbData;
         USHORT  fsStatus;
         USHORT  usFormat;
         USHORT  offszItemName;
         USHORT  offabData;
      } DDESTRUCT;
      typedef DDESTRUCT *PDDESTRUCT;
      
      /* DDE constants for wStatus field */
      #define DDE_FACK                   0x0001
      #define DDE_FBUSY                  0x0002
      #define DDE_FNODATA                0x0004
      #define DDE_FACKREQ                0x0008
      #define DDE_FRESPONSE              0x0010
      #define DDE_NOTPROCESSED           0x0020
      #define DDE_FRESERVED              0x00C0
      #define DDE_FAPPSTATUS             0xFF00
      
      /* old DDE public formats - new ones are CF_ constants */
      
      #define DDEFMT_TEXT                0x0001
      
      /* Dynamic Data Exchange (DDE) Routines */
      
#ifdef __cplusplus
      BOOL    APIENTRY WinDdeInitiate(HWND hwndClient,
                                      PCSZ  pszAppName,
                                      PCSZ  pszTopicName,
                                      PCONVCONTEXT pcctxt);
#else
      BOOL    APIENTRY WinDdeInitiate(HWND hwndClient,
                                      PSZ pszAppName,
                                      PSZ pszTopicName,
                                      PCONVCONTEXT pcctxt);
#endif
      
#ifdef __cplusplus
      MRESULT APIENTRY WinDdeRespond(HWND hwndClient,
                                     HWND hwndServer,
                                     PCSZ  pszAppName,
                                     PCSZ  pszTopicName,
                                     PCONVCONTEXT pcctxt);
#else
      MRESULT APIENTRY WinDdeRespond(HWND hwndClient,
                                     HWND hwndServer,
                                     PSZ pszAppName,
                                     PSZ pszTopicName,
                                     PCONVCONTEXT pcctxt);
#endif
      
      BOOL    APIENTRY WinDdePostMsg(HWND hwndTo,
                                     HWND hwndFrom,
                                     ULONG wm,
                                     PDDESTRUCT pddest,
                                     ULONG flOptions);
      
      #define DDEPM_RETRY     0x00000001L
      #define DDEPM_NOFREE    0x00000002L
      
      /* Dynamic Data Exchange (DDE) Messages */
      
      #define WM_DDE_FIRST               0x00A0
      #define WM_DDE_INITIATE            0x00A0
      #define WM_DDE_REQUEST             0x00A1
      #define WM_DDE_ACK                 0x00A2
      #define WM_DDE_DATA                0x00A3
      #define WM_DDE_ADVISE              0x00A4
      #define WM_DDE_UNADVISE            0x00A5
      #define WM_DDE_POKE                0x00A6
      #define WM_DDE_EXECUTE             0x00A7
      #define WM_DDE_TERMINATE           0x00A8
      #define WM_DDE_INITIATEACK         0x00A9
      #define WM_DDE_LAST                0x00AF
      
      /* DDE helper macros */
      
      #define DDES_PSZITEMNAME(pddes) \
      (((PSZ)pddes) + ((PDDESTRUCT)pddes)->offszItemName)
      
      #define DDES_PABDATA(pddes)       \
      (((PBYTE)pddes) + ((PDDESTRUCT)pddes)->offabData)
      
      #define DDEI_PCONVCONTEXT(pddei)        \
      ((PCONVCONTEXT)((PBYTE)pddei + pddei->offConvContext))
      
      
   #endif /* INCL_WINDDE */
#endif /* !INCL_SAADEFS */

#ifdef INCL_WINWINDOWMGR
   #define WM_QUERYCONVERTPOS         0x00b0   /* WM_DBCSFIRST */
   
   /* Return values for WM_QUERYCONVERTPOS */
   #define QCP_CONVERT                0x0001
   #define QCP_NOCONVERT              0x0000
   
#endif  /* INCL_WINWINDOWMGR */

#ifdef INCL_WINHELP
   #include <pmhelp.h>
#endif /* INCL_WINHELP */

/*Load/Delete Library/Procedure */

typedef HMODULE HLIB;
typedef PHMODULE PHLIB;

#ifdef INCL_WINLOAD
   BOOL    APIENTRY WinDeleteProcedure(HAB hab,
                                       PFNWP wndproc);
   BOOL    APIENTRY WinDeleteLibrary(HAB hab,
                                     HLIB libhandle);
#ifdef __cplusplus
   PFNWP   APIENTRY WinLoadProcedure(HAB hab,
                                     HLIB libhandle,
                                     PCSZ  procname);
#else
   PFNWP   APIENTRY WinLoadProcedure(HAB hab,
                                     HLIB libhandle,
                                     PSZ procname);
#endif
#ifdef __cplusplus
   HLIB    APIENTRY WinLoadLibrary(HAB hab,
                                   PCSZ  libname);
#else
   HLIB    APIENTRY WinLoadLibrary(HAB hab,
                                   PSZ libname);
#endif
#endif /* INCL_WINLOAD */

#ifndef INCL_SAADEFS
   #ifdef INCL_WINDESKTOP
      
      /*** Desktop API definitions ***/
      
      typedef struct _DESKTOP          /* dsk */
      {
         ULONG    cbSize;
         HBITMAP  hbm;
         LONG     x;
         LONG     y;
         ULONG    fl;
         LONG     lTileCount;
         CHAR     szFile[260];
      } DESKTOP;
      typedef DESKTOP *PDESKTOP;  /* pdsk */
      
      HBITMAP APIENTRY WinSetDesktopBkgnd(HWND hwndDesktop,
                                          PDESKTOP pdskNew);
      BOOL    APIENTRY WinQueryDesktopBkgnd(HWND hwndDesktop,
                                            PDESKTOP pdsk);
      
      #define SDT_DESTROY         0x0001L
      #define SDT_NOBKGND         0x0002L
      #define SDT_TILE            0x0004L
      #define SDT_SCALE           0x0008L
      #define SDT_PATTERN         0x0010L
      #define SDT_CENTER          0x0020L
      #define SDT_RETAIN          0x0040L
      #define SDT_LOADFILE        0x0080L
      
   #endif  /* INCL_WINDESKTOP */
#endif

#ifndef INCL_SAADEFS
   #ifdef INCL_WINPALETTE
      
      /*** Palette Manager API definitions ***/
      
      LONG APIENTRY WinRealizePalette(HWND hwnd,
                                      HPS hps,
                                      PULONG pcclr);
      
   #endif  /* INCL_WINPALETTE */
#endif

#ifdef INCL_REMAPDLL
   #define STR_DLLNAME "keyremap"
#endif /*INCL_REMAPDLL*/

#ifdef INCL_NLS
   #define WM_DBCSFIRST               0x00b0
   #define WM_DBCSLAST                0x00cf
   
   #include <os2nls.h>
#endif /* INCL_NLS */

#ifdef INCL_WINBIDI
   #include <pmbidi.h>
#endif /* INCL_WINBIDI */

/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( restore )
   #endif
   #pragma info( restore )
#endif
/* INC */

/**************************** end of file **********************************/

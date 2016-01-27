/******************************* Module Header ******************************
 *
 * Module Name: OS2WIN.H
 *
 * OS/2 Developer API Extensions Include File
 *
 * Copyright (c) International Business Machines Corporation 1995
 *
 ***************************************************************************/
#ifndef _OS2WIN_H
#define _OS2WIN_H


#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( none )
   #endif
   #pragma info( restore )
#endif

#pragma pack(4)


/* Base definitions and error codes defined in separate files
 */
#include "os2wdef.h"
#include "os2werr.h"


/* Macro Definitions
 */
#define MAKEINTRESOURCE(i)   ((LPSTR)(ULONG)(USHORT)(i))

#define MAKEWORD(a, b)       ((WORD)(((BYTE)(a)) | (((WORD)((BYTE)(b))) << 8)))
#define LOWORD(l)            ((WORD)(l))
#define HIWORD(l)            ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)            ((BYTE)(w))
#define HIBYTE(w)            ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define MAKELONG(a, b)       ((LONG)(((WORD)(a)) | (((DWORD)((WORD)(b))) << 16)))

#define POINTTOPOINTS(pt)    (MAKELONG((short)((pt).x), (short)((pt).y)))
#define MAKEWPARAM(l, h)     (WPARAM)MAKELONG(l, h)
#define MAKELPARAM(l, h)     (LPARAM)MAKELONG(l, h)
#define MAKELRESULT(l, h)    (LRESULT)MAKELONG(l, h)

#define POINTSTOPOINT(pt, pts) \
                             { (pt).x=(LONG)(SHORT)LOWORD(*(PLONG)&pts);   \
                               (pt).y=(LONG)(SHORT)HIWORD(*(PLONG)&pts); }

#define GetRValue(rgb)       ((BYTE)(rgb))
#define GetGValue(rgb)       ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)       ((BYTE)((rgb)>>16))
#define RGB(r,g,b)           ( (COLORREF) (((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)    (0x02000000 | RGB(r,g,b))
#define PALETTEINDEX(i)      ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))

#define MINCHAR              0x80
#define MAXCHAR              0x7f
#define MINSHORT             0x8000
#define MAXSHORT             0x7fff
#define MINLONG              0x80000000
#define MAXLONG              0x7fffffff
#define MAXBYTE              0xff
#define MAXWORD              0xffff
#define MAXDWORD             0xffffffff
#define THREAD_PRIORITY_ERROR_RETURN     MAXLONG

#define MAKELANGID(p, s)       ((((WORD  )(s)) << 10) | (WORD  )(p))
#define PRIMARYLANGID(lgid)    ((WORD  )(lgid) & 0x3ff)
#define SUBLANGID(lgid)        ((WORD  )(lgid) >> 10)
#define NLS_VALID_LOCALE_MASK  0x000fffff
#define MAKELCID(lgid, srtid)  ((DWORD)((((DWORD)((WORD  )(srtid))) << 16) |  \
                                         ((DWORD)((WORD)(lgid)))))
#define LANGIDFROMLCID(lcid)   ((WORD  )(lcid))
#define SORTIDFROMLCID(lcid)   ((WORD  )((((DWORD)(lcid)) & NLS_VALID_LOCALE_MASK) >> 16))

/* Types
 */
typedef USHORT               ATOM;
typedef ULONG                COLORREF;

typedef HANDLE               HACCEL;
typedef HANDLE               HBITMAP;
typedef HANDLE               HBRUSH;
typedef HANDLE               HCONV;
typedef DWORD                HCONVLIST;
typedef HANDLE               HCURSOR;
typedef HANDLE               HDC;
typedef HANDLE               HDDEDATA;
typedef HANDLE               HDROP;
typedef HANDLE               HDWP;
typedef HANDLE               HENHMETAFILE;
typedef HANDLE               HFILE;
typedef HANDLE               HFONT;
typedef HANDLE               HGDIOBJ, *PHGDIOBJ;
typedef HANDLE               HGLOBAL;
typedef HANDLE               HHOOK;
typedef HANDLE               HICON;
typedef HANDLE               HINSTANCE;
typedef DWORD                HKEY, *PHKEY;
typedef HANDLE               HKL;
typedef HANDLE               HLOCAL;
typedef HANDLE               HMENU;
typedef HANDLE               HMETAFILE;
typedef HANDLE               HMODULE;
typedef HANDLE               HPALETTE;
typedef HANDLE               HPEN;
typedef HANDLE               HRGN;
typedef HANDLE               HRSRC;
typedef HANDLE               HSTR;
typedef HANDLE               HSZ;
typedef HANDLE               HWND;
typedef HANDLE               GLOBALHANDLE;
typedef HANDLE               LOCALHANDLE;

typedef DWORD                CALID;
typedef DWORD                CALTYPE;
typedef WORD                 LANGID;
typedef DWORD                LCID;
typedef DWORD                LCTYPE;

#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define HFILE_ERROR          ((HANDLE)-1)
#define HWND_DESKTOP         ((HWND)0)
#define HWND_BROADCAST       ((HWND)0xFFFF)

#define STD_INPUT_HANDLE         ((DWORD)-10)
#define STD_OUTPUT_HANDLE        ((DWORD)-11)
#define STD_ERROR_HANDLE         ((DWORD)-12)

/* WM_SIZE Types
 */
#define SIZE_RESTORED        0
#define SIZE_MINIMIZED       1
#define SIZE_MAXIMIZED       2
#define SIZE_MAXSHOW         3
#define SIZE_MAXHIDE         4

#define SIZEFULLSCREEN       SIZE_MAXIMIZED
#define SIZEICONIC           SIZE_MINIMIZED
#define SIZENORMAL           SIZE_RESTORED
#define SIZEZOOMHIDE         SIZE_MAXHIDE
#define SIZEZOOMSHOW         SIZE_MAXSHOW


/* Window Styles
 */
#define WS_OVERLAPPED        0x00000000L
#define WS_POPUP             0x80000000L
#define WS_CHILD             0x40000000L
#define WS_MINIMIZE          0x20000000L
#define WS_VISIBLE           0x10000000L
#define WS_DISABLED          0x08000000L
#define WS_CLIPSIBLINGS      0x04000000L
#define WS_CLIPCHILDREN      0x02000000L
#define WS_MAXIMIZE          0x01000000L
#define WS_BORDER            0x00800000L
#define WS_DLGFRAME          0x00400000L
#define WS_VSCROLL           0x00200000L
#define WS_HSCROLL           0x00100000L
#define WS_SYSMENU           0x00080000L
#define WS_THICKFRAME        0x00040000L
#define WS_GROUP             0x00020000L
#define WS_TABSTOP           0x00010000L

#define WS_MINIMIZEBOX       0x00020000L
#define WS_MAXIMIZEBOX       0x00010000L

#define WS_CHILDWINDOW       WS_CHILD
#define WS_ICONIC            WS_MINIMIZE
#define WS_SIZEBOX           WS_THICKFRAME
#define WS_TILED             WS_OVERLAPPED
#define WS_TILEDWINDOW       WS_OVERLAPPEDWINDOW

#define WS_CAPTION           ( WS_BORDER       | WS_DLGFRAME )
#define WS_OVERLAPPEDWINDOW  ( WS_OVERLAPPED   | WS_CAPTION    | \
                                 WS_SYSMENU      | WS_THICKFRAME | \
                                 WS_MINIMIZEBOX  | WS_MAXIMIZEBOX )
#define WS_POPUPWINDOW       ( WS_POPUP        | WS_BORDER     | \
                                 WS_SYSMENU )

#define WS_EX_TOPMOST        0x00000008L
#define WS_EX_ACCEPTFILES    0x00000010L

#define CW_USEDEFAULT        ((int)0x80000000)


/* Class Styles
 */
#define CS_VREDRAW           0x0001
#define CS_HREDRAW           0x0002
#define CS_KEYCVTWINDOW      0x0004
#define CS_DBLCLKS           0x0008
#define CS_OWNDC             0x0020
#define CS_CLASSDC           0x0040
#define CS_PARENTDC          0x0080
#define CS_NOKEYCVT          0x0100
#define CS_NOCLOSE           0x0200
#define CS_SAVEBITS          0x0800
#define CS_BYTEALIGNCLIENT   0x1000
#define CS_BYTEALIGNWINDOW   0x2000
#define CS_GLOBALCLASS       0x4000


/* Window bytes needed for private dialogs
 */
#define DLGWINDOWEXTRA         30


/* MessageBox Flags
 */
#define MB_OK                0x00000000L
#define MB_OKCANCEL          0x00000001L
#define MB_ABORTRETRYIGNORE  0x00000002L
#define MB_YESNOCANCEL       0x00000003L
#define MB_YESNO             0x00000004L
#define MB_RETRYCANCEL       0x00000005L

#define MB_ICONHAND          0x00000010L
#define MB_ICONQUESTION      0x00000020L
#define MB_ICONEXCLAMATION   0x00000030L
#define MB_ICONASTERISK      0x00000040L

#define MB_ICONINFORMATION   MB_ICONASTERISK
#define MB_ICONSTOP          MB_ICONHAND

#define MB_DEFBUTTON1        0x00000000L
#define MB_DEFBUTTON2        0x00000100L
#define MB_DEFBUTTON3        0x00000200L

#define MB_APPLMODAL         0x00000000L
#define MB_SYSTEMMODAL       0x00001000L

#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_SERVICE_NOTIFICATION     0x00040000L

#define MB_TYPEMASK          0x0000000FL
#define MB_ICONMASK          0x000000F0L
#define MB_DEFMASK           0x00000F00L
#define MB_MODEMASK          0x00003000L
#define MB_MISCMASK          0x0000C000L


/* Predefined Resource Types
 */
#define RT_CURSOR            MAKEINTRESOURCE(1)
#define RT_BITMAP            MAKEINTRESOURCE(2)
#define RT_ICON              MAKEINTRESOURCE(3)
#define RT_MENU              MAKEINTRESOURCE(4)
#define RT_DIALOG            MAKEINTRESOURCE(5)
#define RT_STRING            MAKEINTRESOURCE(6)
#define RT_FONTDIR           MAKEINTRESOURCE(7)
#define RT_FONT              MAKEINTRESOURCE(8)
#define RT_ACCELERATOR       MAKEINTRESOURCE(9)
#define RT_RCDATA            MAKEINTRESOURCE(10)
#define RT_MESSAGETABLE      MAKEINTRESOURCE(11)
#define RT_DLGINCLUDE        MAKEINTRESOURCE(17)


/* System Icons
 */
#define IDI_APPLICATION      MAKEINTRESOURCE(32512)
#define IDI_HAND             MAKEINTRESOURCE(32513)
#define IDI_QUESTION         MAKEINTRESOURCE(32514)
#define IDI_EXCLAMATION      MAKEINTRESOURCE(32515)
#define IDI_ASTERISK         MAKEINTRESOURCE(32516)

/* System cursors
 */
#define IDC_ARROW            MAKEINTRESOURCE(32512)
#define IDC_IBEAM            MAKEINTRESOURCE(32513)
#define IDC_WAIT             MAKEINTRESOURCE(32514)
#define IDC_CROSS            MAKEINTRESOURCE(32515)
#define IDC_UPARROW          MAKEINTRESOURCE(32516)
#define IDC_SIZE             MAKEINTRESOURCE(32640)
#define IDC_ICON             MAKEINTRESOURCE(32641)
#define IDC_SIZENWSE         MAKEINTRESOURCE(32642)
#define IDC_SIZENESW         MAKEINTRESOURCE(32643)
#define IDC_SIZEWE           MAKEINTRESOURCE(32644)
#define IDC_SIZENS           MAKEINTRESOURCE(32645)
#define IDC_SIZEALL          MAKEINTRESOURCE(32646)
#define IDC_NO               MAKEINTRESOURCE(32648)
#define IDC_APPSTARTING      MAKEINTRESOURCE(32650)

/* System Bitmaps
 */
#define OBM_CLOSE           32754
#define OBM_UPARROW         32753
#define OBM_DNARROW         32752
#define OBM_RGARROW         32751
#define OBM_LFARROW         32750
#define OBM_REDUCE          32749
#define OBM_ZOOM            32748
#define OBM_RESTORE         32747
#define OBM_REDUCED         32746
#define OBM_ZOOMD           32745
#define OBM_RESTORED        32744
#define OBM_UPARROWD        32743
#define OBM_DNARROWD        32742
#define OBM_RGARROWD        32741
#define OBM_LFARROWD        32740
#define OBM_MNARROW         32739
#define OBM_COMBO           32738
#define OBM_UPARROWI        32737
#define OBM_DNARROWI        32736
#define OBM_RGARROWI        32735
#define OBM_LFARROWI        32734
#define OBM_OLD_CLOSE       32767
#define OBM_SIZE            32766
#define OBM_OLD_UPARROW     32765
#define OBM_OLD_DNARROW     32764
#define OBM_OLD_RGARROW     32763
#define OBM_OLD_LFARROW     32762
#define OBM_BTSIZE          32761
#define OBM_CHECK           32760
#define OBM_CHECKBOXES      32759
#define OBM_BTNCORNERS      32758
#define OBM_OLD_REDUCE      32757
#define OBM_OLD_ZOOM        32756
#define OBM_OLD_RESTORE     32755

/* Device Capabilities Indices
 */
#define DRIVERVERSION        0
#define TECHNOLOGY           2
#define HORZSIZE             4
#define VERTSIZE             6
#define HORZRES              8
#define VERTRES              10
#define BITSPIXEL            12
#define PLANES               14
#define NUMFONTS             22
#define NUMCOLORS            24
#define PDEVICESIZE          26
#define CURVECAPS            28
#define LINECAPS             30
#define POLYGONALCAPS        32
#define TEXTCAPS             34
#define CLIPCAPS             36
#define RASTERCAPS           38
#define ASPECTX              40
#define ASPECTY              42
#define ASPECTXY             44
#define LOGPIXELSX           88
#define LOGPIXELSY           90
#define SIZEPALETTE          104
#define NUMRESERVED          106
#define COLORRES             108
#define PHYSICALWIDTH        110
#define PHYSICALHEIGHT       111
#define PHYSICALOFFSETX      112
#define PHYSICALOFFSETY      113
#define SCALINGFACTORX       114
#define SCALINGFACTORY       115

/* Device Technologies
 */
#define DT_PLOTTER           0
#define DT_RASDISPLAY        1
#define DT_RASPRINTER        2
#define DT_RASCAMERA         3
#define DT_METAFILE          5

/* Raster Capabilities
 */
#define RC_BITBLT            0x0001
#define RC_BANDING           0x0002
#define RC_SCALING           0x0004
#define RC_BITMAP64          0x0008
#define RC_DI_BITMAP         0x0080
#define RC_PALETTE           0x0100
#define RC_DIBTODEV          0x0200
#define RC_STRETCHBLT        0x0800
#define RC_FLOODFILL         0x1000
#define RC_STRETCHDIB        0x2000

#define CC_NONE              0
#define CC_CIRCLES           1
#define CC_PIE               2
#define CC_CHORD             4
#define CC_ELLIPSES          8
#define CC_WIDE              16
#define CC_STYLED            32
#define CC_WIDESTYLED        64
#define CC_INTERIORS         128
#define CC_ROUNDRECT         256

#define LC_NONE              0
#define LC_POLYLINE          2
#define LC_MARKER            4
#define LC_POLYMARKER        8
#define LC_WIDE              16
#define LC_STYLED            32
#define LC_WIDESTYLED        64
#define LC_INTERIORS         128

#define PC_NONE              0
#define PC_POLYGON           1
#define PC_RECTANGLE         2
#define PC_WINDPOLYGON       4
#define PC_TRAPEZOID         4
#define PC_SCANLINE          8
#define PC_WIDE              16
#define PC_STYLED            32
#define PC_WIDESTYLED        64
#define PC_INTERIORS         128

#define TC_CR_90             0x00000008
#define TC_CR_ANY            0x00000010
#define TC_SF_X_YINDEP       0x00000020
#define TC_SA_INTEGER        0x00000080
#define TC_SA_CONTIN         0x00000100
#define TC_IA_ABLE           0x00000400
#define TC_UA_ABLE           0x00000800
#define TC_SO_ABLE           0x00001000
#define TC_RA_ABLE           0x00002000
#define TC_VA_ABLE           0x00004000
#define TC_RESERVED          0x00008000
#define TC_SCROLLBLT         0x00010000


/* GetWindow Constants
 */
#define GW_HWNDFIRST         0
#define GW_HWNDLAST          1
#define GW_HWNDNEXT          2
#define GW_HWNDPREV          3
#define GW_OWNER             4
#define GW_CHILD             5
#define GW_MAX               5

/* ShowWindow() Constants
 */
#define SW_HIDE              0
#define SW_NORMAL            1
#define SW_SHOWNORMAL        1
#define SW_SHOWMINIMIZED     2
#define SW_MAXIMIZE          3
#define SW_SHOWMAXIMIZED     3
#define SW_SHOWNOACTIVATE    4
#define SW_SHOW              5
#define SW_MINIMIZE          6
#define SW_SHOWMINNOACTIVE   7
#define SW_SHOWNA            8
#define SW_RESTORE           9
#define SW_MAX               10
#define SW_SHOWDEFAULT       10

#define HIDE_WINDOW          0
#define SHOW_OPENWINDOW      1
#define SHOW_ICONWINDOW      2
#define SHOW_FULLSCREEN      3
#define SHOW_OPENNOACTIVATE  4

#define WA_INACTIVE          0
#define WA_ACTIVE            1
#define WA_CLICKACTIVE       2

/* SetWindowPos() Constants
 */
#define SWP_NOSIZE           0x0001
#define SWP_NOMOVE           0x0002
#define SWP_NOZORDER         0x0004
#define SWP_NOREDRAW         0x0008
#define SWP_NOACTIVATE       0x0010
#define SWP_FRAMECHANGED     0x0020
#define SWP_SHOWWINDOW       0x0040
#define SWP_HIDEWINDOW       0x0080
#define SWP_NOCOPYBITS       0x0100
#define SWP_NOOWNERZORDER    0x0200

#define SWP_DRAWFRAME        SWP_FRAMECHANGED
#define SWP_NOREPOSITION     SWP_NOOWNERZORDER

/* WINDOWPOS Structure sent with WM_WINDOWPOSCHANGING */
typedef struct   _tagWINDOWPOS {
   HWND hwnd;
   HWND hwndInsertAfter;
   int      x;
   int      y;
   int      cx;
   int      cy;
   UINT  flags;
   } WINDOWPOS, *PWINDOWPOS, *LPWINDOWPOS;

#define HWND_TOP             ((HWND)0)
#define HWND_BOTTOM          ((HWND)1)
#define HWND_TOPMOST         ((HWND)-1)
#define HWND_NOTOPMOST       ((HWND)-2)


/* Menu flags for Add/Check/EnableMenuItem
 */
#define MF_BYCOMMAND         0x00000000L
#define MF_ENABLED           0x00000000L
#define MF_INSERT            0x00000000L
#define MF_STRING            0x00000000L
#define MF_UNCHECKED         0x00000000L
#define MF_UNHILITE          0x00000000L
#define MF_GRAYED            0x00000001L
#define MF_DISABLED          0x00000002L
#define MF_BITMAP            0x00000004L
#define MF_CHECKED           0x00000008L
#define MF_POPUP             0x00000010L
#define MF_MENUBARBREAK      0x00000020L
#define MF_MENUBREAK         0x00000040L
#define MF_CHANGE            0x00000080L
#define MF_END               0x00000080L
#define MF_HILITE            0x00000080L
#define MF_APPEND            0x00000100L
#define MF_OWNERDRAW         0x00000100L
#define MF_DELETE            0x00000200L
#define MF_USECHECKBITMAPS   0x00000200L
#define MF_BYPOSITION        0x00000400L
#define MF_SEPARATOR         0x00000800L
#define MF_REMOVE            0x00001000L
#define MF_SYSMENU           0x00002000L
#define MF_HELP              0x00004000L
#define MF_MOUSESELECT       0x00008000L


/* Get/SetWindowLong Offsets
 */
#define GWL_WNDPROC          (-4)
#define GWL_HINSTANCE        (-6)
#define GWL_HWNDPARENT       (-8)
#define GWL_ID               (-12)
#define GWL_STYLE            (-16)
#define GWL_EXSTYLE          (-20)
#define GWL_USERDATA         (-21)

/* Get/SetWindowWord Offsets
 */
#define GWW_HINSTANCE        (-6)
#define GWW_HWNDPARENT       (-8)
#define GWW_ID               (-12)

/* Get/SetClassLong Offsets
 */
#define GCL_MENUNAME         (-8)
#define GCL_HBRBACKGROUND    (-10)
#define GCL_HCURSOR          (-12)
#define GCL_HICON            (-14)
#define GCL_HMODULE          (-16)
#define GCL_CBWNDEXTRA       (-18)
#define GCL_CBCLSEXTRA       (-20)
#define GCL_WNDPROC          (-24)
#define GCL_STYLE            (-26)
#define GCW_ATOM             (-32)

/* Get/SetClassWord Offsets
 */
#define GCW_HBRBACKGROUND    (-10)
#define GCW_HCURSOR          (-12)
#define GCW_HICON            (-13)
#define GCW_HMODULE          (-14)
#define GCW_CBWNDEXTRA       (-18)
#define GCW_CBCLSEXTRA       (-20)
#define GCW_STYLE            (-26)
#define GCW_ATOM             (-32)

/* Get/SetWindowLong Offsets for Dialog Windows
 */
#define DWL_MSGRESULT        0
#define DWL_DLGPROC          4
#define DWL_USER             8


/* Messages
 */
#define WM_NULL                  0x0000
#define WM_CREATE                0x0001
#define WM_DESTROY               0x0002
#define WM_MOVE                  0x0003
#define WM_SIZE                  0x0005
#define WM_ACTIVATE              0x0006
#define WM_SETFOCUS              0x0007
#define WM_KILLFOCUS             0x0008
#define WM_ENABLE                0x000A
#define WM_SETREDRAW             0x000B
#define WM_SETTEXT               0x000C
#define WM_GETTEXT               0x000D
#define WM_GETTEXTLENGTH         0x000E
#define WM_PAINT                 0x000F
#define WM_CLOSE                 0x0010
#define WM_QUERYENDSESSION       0x0011
#define WM_QUIT                  0x0012
#define WM_QUERYOPEN             0x0013
#define WM_ERASEBKGND            0x0014
#define WM_SYSCOLORCHANGE        0x0015
#define WM_ENDSESSION            0x0016
#define WM_SHOWWINDOW            0x0018
#define WM_WININICHANGE          0x001A
#define WM_DEVMODECHANGE         0x001B
#define WM_ACTIVATEAPP           0x001C
#define WM_FONTCHANGE            0x001D
#define WM_TIMECHANGE            0x001E
#define WM_CANCELMODE            0x001F
#define WM_SETCURSOR             0x0020
#define WM_MOUSEACTIVATE         0x0021
#define WM_CHILDACTIVATE         0x0022
#define WM_QUEUESYNC             0x0023
#define WM_GETMINMAXINFO         0x0024
#define WM_PAINTICON             0x0026
#define WM_ICONERASEBKGND        0x0027
#define WM_NEXTDLGCTL            0x0028
#define WM_SPOOLERSTATUS         0x002A
#define WM_DRAWITEM              0x002B
#define WM_MEASUREITEM           0x002C
#define WM_DELETEITEM            0x002D
#define WM_VKEYTOITEM            0x002E
#define WM_CHARTOITEM            0x002F
#define WM_SETFONT               0x0030
#define WM_GETFONT               0x0031
#define WM_SETHOTKEY             0x0032
#define WM_GETHOTKEY             0x0033
#define WM_QUERYDRAGICON         0x0037
#define WM_COMPAREITEM           0x0039
#define WM_COMPACTING            0x0041
#define WM_WINDOWPOSCHANGING     0x0046
#define WM_WINDOWPOSCHANGED      0x0047
#define WM_POWER                 0x0048
#define WM_COPYDATA              0x004A
#define WM_CANCELJOURNAL         0x004B
#define WM_HELP                  0x0053
#define WM_NCCREATE              0x0081
#define WM_NCDESTROY             0x0082
#define WM_NCCALCSIZE            0x0083
#define WM_NCHITTEST             0x0084
#define WM_NCPAINT               0x0085
#define WM_NCACTIVATE            0x0086
#define WM_GETDLGCODE            0x0087
#define WM_NCMOUSEMOVE           0x00A0
#define WM_NCLBUTTONDOWN         0x00A1
#define WM_NCLBUTTONUP           0x00A2
#define WM_NCLBUTTONDBLCLK       0x00A3
#define WM_NCRBUTTONDOWN         0x00A4
#define WM_NCRBUTTONUP           0x00A5
#define WM_NCRBUTTONDBLCLK       0x00A6
#define WM_NCMBUTTONDOWN         0x00A7
#define WM_NCMBUTTONUP           0x00A8
#define WM_NCMBUTTONDBLCLK       0x00A9
#define EM_GETSEL                0x00B0
#define EM_SETSEL                0x00B1
#define EM_GETRECT               0x00B2
#define EM_SETRECT               0x00B3
#define EM_SETRECTNP             0x00B4
#define EM_SCROLL                0x00B5
#define EM_LINESCROLL            0x00B6
#define EM_SCROLLCARET           0x00B7
#define EM_GETMODIFY             0x00B8
#define EM_SETMODIFY             0x00B9
#define EM_GETLINECOUNT          0x00BA
#define EM_LINEINDEX             0x00BB
#define EM_SETHANDLE             0x00BC
#define EM_GETHANDLE             0x00BD
#define EM_GETTHUMB              0x00BE
#define EM_LINELENGTH            0x00C1
#define EM_REPLACESEL            0x00C2
#define EM_GETLINE               0x00C4
#define EM_LIMITTEXT             0x00C5
#define EM_CANUNDO               0x00C6
#define EM_UNDO                  0x00C7
#define EM_FMTLINES              0x00C8
#define EM_LINEFROMCHAR          0x00C9
#define EM_SETTABSTOPS           0x00CB
#define EM_SETPASSWORDCHAR       0x00CC
#define EM_EMPTYUNDOBUFFER       0x00CD
#define EM_GETFIRSTVISIBLELINE   0x00CE
#define EM_SETREADONLY           0x00CF
#define EM_SETWORDBREAKPROC      0x00D0
#define EM_GETWORDBREAKPROC      0x00D1
#define EM_GETPASSWORDCHAR       0x00D2
#define SBM_SETPOS               0x00E0
#define SBM_GETPOS               0x00E1
#define SBM_SETRANGE             0x00E2
#define SBM_GETRANGE             0x00E3
#define SBM_ENABLE_ARROWS        0x00E4
#define SBM_SETRANGEREDRAW       0x00E6
#define WM_KEYFIRST              0x0100
#define WM_KEYDOWN               0x0100
#define WM_KEYUP                 0x0101
#define WM_CHAR                  0x0102
#define WM_DEADCHAR              0x0103
#define WM_SYSKEYDOWN            0x0104
#define WM_SYSKEYUP              0x0105
#define WM_SYSCHAR               0x0106
#define WM_SYSDEADCHAR           0x0107
#define WM_KEYLAST               0x0108
#define WM_INITDIALOG            0x0110
#define WM_COMMAND               0x0111
#define WM_SYSCOMMAND            0x0112
#define WM_TIMER                 0x0113
#define WM_HSCROLL               0x0114
#define WM_VSCROLL               0x0115
#define WM_INITMENU              0x0116
#define WM_INITMENUPOPUP         0x0117
#define WM_MENUSELECT            0x011F
#define WM_MENUCHAR              0x0120
#define WM_ENTERIDLE             0x0121
#define WM_CTLCOLORMSGBOX        0x0132
#define WM_CTLCOLOREDIT          0x0133
#define WM_CTLCOLORLISTBOX       0x0134
#define WM_CTLCOLORBTN           0x0135
#define WM_CTLCOLORDLG           0x0136
#define WM_CTLCOLORSCROLLBAR     0x0137
#define WM_CTLCOLORSTATIC        0x0138
#define CB_GETEDITSEL            0x0140
#define CB_LIMITTEXT             0x0141
#define CB_SETEDITSEL            0x0142
#define CB_ADDSTRING             0x0143
#define CB_DELETESTRING          0x0144
#define CB_DIR                   0x0145
#define CB_GETCOUNT              0x0146
#define CB_GETCURSEL             0x0147
#define CB_GETLBTEXT             0x0148
#define CB_GETLBTEXTLEN          0x0149
#define CB_INSERTSTRING          0x014A
#define CB_RESETCONTENT          0x014B
#define CB_FINDSTRING            0x014C
#define CB_SELECTSTRING          0x014D
#define CB_SETCURSEL             0x014E
#define CB_SHOWDROPDOWN          0x014F
#define CB_GETITEMDATA           0x0150
#define CB_SETITEMDATA           0x0151
#define CB_GETDROPPEDCONTROLRECT 0x0152
#define CB_SETITEMHEIGHT         0x0153
#define CB_GETITEMHEIGHT         0x0154
#define CB_SETEXTENDEDUI         0x0155
#define CB_GETEXTENDEDUI         0x0156
#define CB_GETDROPPEDSTATE       0x0157
#define CB_FINDSTRINGEXACT       0x0158
#define CB_SETLOCALE             0x0159
#define CB_GETLOCALE             0x015A
#define CB_MSGMAX                0x015B
#define STM_SETICON              0x0170
#define STM_GETICON              0x0171
#define STM_MSGMAX               0x0172
#define LB_ADDSTRING             0x0180
#define LB_INSERTSTRING          0x0181
#define LB_DELETESTRING          0x0182
#define LB_SELITEMRANGEEX        0x0183
#define LB_RESETCONTENT          0x0184
#define LB_SETSEL                0x0185
#define LB_SETCURSEL             0x0186
#define LB_GETSEL                0x0187
#define LB_GETCURSEL             0x0188
#define LB_GETTEXT               0x0189
#define LB_GETTEXTLEN            0x018A
#define LB_GETCOUNT              0x018B
#define LB_SELECTSTRING          0x018C
#define LB_DIR                   0x018D
#define LB_GETTOPINDEX           0x018E
#define LB_FINDSTRING            0x018F
#define LB_GETSELCOUNT           0x0190
#define LB_GETSELITEMS           0x0191
#define LB_SETTABSTOPS           0x0192
#define LB_GETHORIZONTALEXTENT   0x0193
#define LB_SETHORIZONTALEXTENT   0x0194
#define LB_SETCOLUMNWIDTH        0x0195
#define LB_ADDFILE               0x0196
#define LB_SETTOPINDEX           0x0197
#define LB_GETITEMRECT           0x0198
#define LB_GETITEMDATA           0x0199
#define LB_SETITEMDATA           0x019A
#define LB_SELITEMRANGE          0x019B
#define LB_SETANCHORINDEX        0x019C
#define LB_GETANCHORINDEX        0x019D
#define LB_SETCARETINDEX         0x019E
#define LB_GETCARETINDEX         0x019F
#define LB_SETITEMHEIGHT         0x01A0
#define LB_GETITEMHEIGHT         0x01A1
#define LB_FINDSTRINGEXACT       0x01A2
#define LB_SETLOCALE             0x01A5
#define LB_GETLOCALE             0x01A6
#define LB_SETCOUNT              0x01A7
#define LB_MSGMAX                0x01A8
#define WM_MOUSEFIRST            0x0200
#define WM_MOUSEMOVE             0x0200
#define WM_LBUTTONDOWN           0x0201
#define WM_LBUTTONUP             0x0202
#define WM_LBUTTONDBLCLK         0x0203
#define WM_RBUTTONDOWN           0x0204
#define WM_RBUTTONUP             0x0205
#define WM_RBUTTONDBLCLK         0x0206
#define WM_MBUTTONDOWN           0x0207
#define WM_MBUTTONUP             0x0208
#define WM_MBUTTONDBLCLK         0x0209
#define WM_MOUSELAST             0x0209
#define WM_PARENTNOTIFY          0x0210
#define WM_ENTERMENULOOP         0x0211
#define WM_EXITMENULOOP          0x0212
#define WM_MDICREATE             0x0220
#define WM_MDIDESTROY            0x0221
#define WM_MDIACTIVATE           0x0222
#define WM_MDIRESTORE            0x0223
#define WM_MDINEXT               0x0224
#define WM_MDIMAXIMIZE           0x0225
#define WM_MDITILE               0x0226
#define WM_MDICASCADE            0x0227
#define WM_MDIICONARRANGE        0x0228
#define WM_MDIGETACTIVE          0x0229
#define WM_MDISETMENU            0x0230
#define WM_DROPFILES             0x0233
#define WM_MDIREFRESHMENU        0x0234
#define WM_CUT                   0x0300
#define WM_COPY                  0x0301
#define WM_PASTE                 0x0302
#define WM_CLEAR                 0x0303
#define WM_UNDO                  0x0304
#define WM_RENDERFORMAT          0x0305
#define WM_RENDERALLFORMATS      0x0306
#define WM_DESTROYCLIPBOARD      0x0307
#define WM_DRAWCLIPBOARD         0x0308
#define WM_PAINTCLIPBOARD        0x0309
#define WM_VSCROLLCLIPBOARD      0x030A
#define WM_SIZECLIPBOARD         0x030B
#define WM_ASKCBFORMATNAME       0x030C
#define WM_CHANGECBCHAIN         0x030D
#define WM_HSCROLLCLIPBOARD      0x030E
#define WM_QUERYNEWPALETTE       0x030F
#define WM_PALETTEISCHANGING     0x0310
#define WM_PALETTECHANGED        0x0311
#define WM_HOTKEY                0x0312
#define WM_DDE_FIRST             0x03E0
#define WM_DDE_INITIATE          (WM_DDE_FIRST)
#define WM_DDE_TERMINATE         (WM_DDE_FIRST + 1)
#define WM_DDE_ADVISE            (WM_DDE_FIRST + 2)
#define WM_DDE_UNADVISE          (WM_DDE_FIRST + 3)
#define WM_DDE_ACK               (WM_DDE_FIRST + 4)
#define WM_DDE_DATA              (WM_DDE_FIRST + 5)
#define WM_DDE_REQUEST           (WM_DDE_FIRST + 6)
#define WM_DDE_POKE              (WM_DDE_FIRST + 7)
#define WM_DDE_EXECUTE           (WM_DDE_FIRST + 8)
#define WM_DDE_LAST              (WM_DDE_FIRST + 8)
#define WM_USER                  0x0400

#define WM_CHOOSEFONT_GETLOGFONT (WM_USER + 1)


/* MDI client style bits
 */
#define MDIS_ALLCHILDSTYLES  0x0001

/* wParam Flags for WM_MDITILE and WM_MDICASCADE messages.
 */
#define MDITILE_VERTICAL     0x0000
#define MDITILE_HORIZONTAL   0x0001
#define MDITILE_SKIPDISABLED 0x0002


/* WM_MOUSEACTIVATE Return Codes
 */
#define MA_ACTIVATE          1
#define MA_ACTIVATEANDEAT    2
#define MA_NOACTIVATE        3
#define MA_NOACTIVATEANDEAT  4


/* Dialog and Message Box IDs
 */
#define IDOK                 1
#define IDCANCEL             2
#define IDABORT              3
#define IDRETRY              4
#define IDIGNORE             5
#define IDYES                6
#define IDNO                 7
#define IDCLOSE              8
#define IDHELP               9


/* Font Families
 */
#define FF_DONTCARE          0x0000
#define FF_ROMAN             0x0010
#define FF_SWISS             0x0020
#define FF_MODERN            0x0030
#define FF_SCRIPT            0x0040
#define FF_DECORATIVE        0x0050

/* Font Bold Weight Values
 */
#define FW_DONTCARE          0
#define FW_THIN              100
#define FW_EXTRALIGHT        200
#define FW_LIGHT             300
#define FW_NORMAL            400
#define FW_MEDIUM            500
#define FW_SEMIBOLD          600
#define FW_BOLD              700
#define FW_EXTRABOLD         800
#define FW_HEAVY             900

#define FW_BLACK             FW_HEAVY
#define FW_DEMIBOLD          FW_SEMIBOLD
#define FW_REGULAR           FW_NORMAL
#define FW_ULTRABOLD         FW_EXTRABOLD
#define FW_ULTRALIGHT        FW_EXTRALIGHT


/* ChooseColor flag definitions
 */
#define CC_RGBINIT               0x00000001L
#define CC_FULLOPEN              0x00000002L
#define CC_PREVENTFULLOPEN       0x00000004L
#define CC_SHOWHELP              0x00000008L
#define CC_ENABLEHOOK            0x00000010L
#define CC_ENABLETEMPLATE        0x00000020L
#define CC_ENABLETEMPLATEHANDLE  0x00000040L

/* FindText and ReplaceText flag definitions
 */
#define FR_DOWN                  0x00000001L
#define FR_WHOLEWORD             0x00000002L
#define FR_MATCHCASE             0x00000004L
#define FR_FINDNEXT              0x00000008L
#define FR_REPLACE               0x00000010L
#define FR_REPLACEALL            0x00000020L
#define FR_DIALOGTERM            0x00000040L
#define FR_SHOWHELP              0x00000080L
#define FR_ENABLEHOOK            0x00000100L
#define FR_ENABLETEMPLATE        0x00000200L
#define FR_NOUPDOWN              0x00000400L
#define FR_NOMATCHCASE           0x00000800L
#define FR_NOWHOLEWORD           0x00001000L
#define FR_ENABLETEMPLATEHANDLE  0x00002000L
#define FR_HIDEUPDOWN            0x00004000L
#define FR_HIDEMATCHCASE         0x00008000L
#define FR_HIDEWHOLEWORD         0x00010000L

/* Common Dialog Error Codes
 */
#define CDERR_GENERALCODES       0x0000
#define CDERR_STRUCTSIZE         0x0001
#define CDERR_INITIALIZATION     0x0002
#define CDERR_NOTEMPLATE         0x0003
#define CDERR_NOHINSTANCE        0x0004
#define CDERR_LOADSTRFAILURE     0x0005
#define CDERR_FINDRESFAILURE     0x0006
#define CDERR_LOADRESFAILURE     0x0007
#define CDERR_LOCKRESFAILURE     0x0008
#define CDERR_MEMALLOCFAILURE    0x0009
#define CDERR_MEMLOCKFAILURE     0x000A
#define CDERR_NOHOOK             0x000B
#define CDERR_REGISTERMSGFAIL    0x000C
#define PDERR_PRINTERCODES       0x1000
#define PDERR_SETUPFAILURE       0x1001
#define PDERR_PARSEFAILURE       0x1002
#define PDERR_RETDEFFAILURE      0x1003
#define PDERR_LOADDRVFAILURE     0x1004
#define PDERR_GETDEVMODEFAIL     0x1005
#define PDERR_INITFAILURE        0x1006
#define PDERR_NODEVICES          0x1007
#define PDERR_NODEFAULTPRN       0x1008
#define PDERR_DNDMMISMATCH       0x1009
#define PDERR_CREATEICFAILURE    0x100A
#define PDERR_PRINTERNOTFOUND    0x100B
#define PDERR_DEFAULTDIFFERENT   0x100C
#define CFERR_CHOOSEFONTCODES    0x2000
#define CFERR_NOFONTS            0x2001
#define CFERR_MAXLESSTHANMIN     0x2002
#define FNERR_FILENAMECODES      0x3000
#define FNERR_SUBCLASSFAILURE    0x3001
#define FNERR_INVALIDFILENAME    0x3002
#define FNERR_BUFFERTOOSMALL     0x3003
#define FRERR_FINDREPLACECODES   0x4000
#define FRERR_BUFFERLENGTHZERO   0x4001
#define CCERR_CHOOSECOLORCODES   0x5000
#define CDERR_DIALOGFAILURE      0xFFFF

/* ChooseFont flags.
 */
#define CF_SCREENFONTS           0x00000001
#define CF_PRINTERFONTS          0x00000002
#define CF_SHOWHELP              0x00000004
#define CF_ENABLEHOOK            0x00000008
#define CF_ENABLETEMPLATE        0x00000010
#define CF_ENABLETEMPLATEHANDLE  0x00000020
#define CF_INITTOLOGFONTSTRUCT   0x00000040
#define CF_USESTYLE              0x00000080
#define CF_EFFECTS               0x00000100
#define CF_APPLY                 0x00000200
#define CF_ANSIONLY              0x00000400
#define CF_NOVECTORFONTS         0x00000800
#define CF_NOSIMULATIONS         0x00001000
#define CF_LIMITSIZE             0x00002000
#define CF_FIXEDPITCHONLY        0x00004000
#define CF_WYSIWYG               0x00008000
#define CF_FORCEFONTEXIST        0x00010000
#define CF_SCALABLEONLY          0x00020000
#define CF_TTONLY                0x00040000
#define CF_NOFACESEL             0x00080000
#define CF_NOSTYLESEL            0x00100000
#define CF_NOSIZESEL             0x00200000
#define CF_BOTH                  (CF_SCREENFONTS | CF_PRINTERFONTS)
#define CF_NOOEMFONTS            CF_NOVECTORFONTS

/* Common Dialog User Registered Messages
 */
#define COLOROKSTRING            "commdlg_ColorOK"
#define FILEOKSTRING             "commdlg_FileNameOK"
#define FINDMSGSTRING            "commdlg_FindReplace"
#define HELPMSGSTRING            "commdlg_help"
#define LBSELCHSTRING            "commdlg_LBSelChangedNotify"
#define SETRGBSTRING             "commdlg_SetRGBColor"
#define SHAREVISTRING            "commdlg_ShareViolation"

/* Font types
 */
#define SIMULATED_FONTTYPE       0x8000
#define PRINTER_FONTTYPE         0x4000
#define SCREEN_FONTTYPE          0x2000
#define BOLD_FONTTYPE            0x0100
#define ITALIC_FONTTYPE          0x0200
#define REGULAR_FONTTYPE         0x0400

/* Character sets
 */
#define ANSI_CHARSET             0
#define SYMBOL_CHARSET           2
#define OEM_CHARSET              255

/* Output precision
 */
#define OUT_DEFAULT_PRECIS       0
#define OUT_STRING_PRECIS        1
#define OUT_CHARACTER_PRECIS     2
#define OUT_STROKE_PRECIS        3

/* Clip precision
 */
#define CLIP_DEFAULT_PRECIS      0
#define CLIP_CHARACTER_PRECIS    1
#define CLIP_STROKE_PRECIS       2

/* Font quality
 */
#define DEFAULT_QUALITY          0
#define DRAFT_QUALITY            1
#define PROOF_QUALITY            2

/* PitchAndFamily values for CreateFont
 */
#define DEFAULT_PITCH            0
#define FIXED_PITCH              1
#define VARIABLE_PITCH           2

/* GDI Escapes
 */
#define NEWFRAME                     1
#define ABORTDOC                     2
#define NEXTBAND                     3
#define SETCOLORTABLE                4
#define GETCOLORTABLE                5
#define FLUSHOUTPUT                  6
#define DRAFTMODE                    7
#define QUERYESCSUPPORT              8
#define SETABORTPROC                 9
#define STARTDOC                     10
#define ENDDOC                       11
#define GETPHYSPAGESIZE              12
#define GETPRINTINGOFFSET            13
#define GETSCALINGFACTOR             14
#define MFCOMMENT                    15
#define GETPENWIDTH                  16
#define SETCOPYCOUNT                 17
#define SELECTPAPERSOURCE            18
#define DEVICEDATA                   19
#define PASSTHROUGH                  19
#define GETTECHNOLGY                 20
#define GETTECHNOLOGY                20
#define SETLINECAP                   21
#define SETLINEJOIN                  22
#define SETMITERLIMIT                23
#define BANDINFO                     24
#define DRAWPATTERNRECT              25
#define GETVECTORPENSIZE             26
#define GETVECTORBRUSHSIZE           27
#define ENABLEDUPLEX                 28
#define GETSETPAPERBINS              29
#define GETSETPRINTORIENT            30
#define ENUMPAPERBINS                31
#define SETDIBSCALING                32
#define EPSPRINTING                  33
#define ENUMPAPERMETRICS             34
#define GETSETPAPERMETRICS           35
#define POSTSCRIPT_DATA              37
#define POSTSCRIPT_IGNORE            38
#define MOUSETRAILS                  39
#define GETDEVICEUNITS               42

#define GETEXTENDEDTEXTMETRICS       256
#define GETEXTENTTABLE               257
#define GETPAIRKERNTABLE             258
#define GETTRACKKERNTABLE            259
#define EXTTEXTOUT                   512
#define GETFACENAME                  513
#define DOWNLOADFACE                 514
#define ENABLERELATIVEWIDTHS         768
#define ENABLEPAIRKERNING            769
#define SETKERNTRACK                 770
#define SETALLJUSTVALUES             771
#define SETCHARSET                   772

#define STRETCHBLT                   2048
#define GETSETSCREENPARAMS           3072
#define QUERYDIBSUPPORT              3073
#define BEGIN_PATH                   4096
#define CLIP_TO_PATH                 4097
#define END_PATH                     4098
#define EXT_DEVICE_CAPS              4099
#define RESTORE_CTM                  4100
#define SAVE_CTM                     4101
#define SET_ARC_DIRECTION            4102
#define SET_BACKGROUND_COLOR         4103
#define SET_POLY_MODE                4104
#define SET_SCREEN_ANGLE             4105
#define SET_SPREAD                   4106
#define TRANSFORM_CTM                4107
#define SET_CLIP_BOX                 4108
#define SET_BOUNDS                   4109
#define SET_MIRROR_MODE              4110
#define OPENCHANNEL                  4110
#define DOWNLOADHEADER               4111
#define CLOSECHANNEL                 4112
#define POSTSCRIPT_PASSTHROUGH       4115
#define ENCAPSULATED_POSTSCRIPT      4116

#define DS_ABSALIGN              0x001L
#define DS_SYSMODAL              0x002L
#define DS_LOCALEDIT             0x020L
#define DS_SETFONT               0x040L
#define DS_MODALFRAME            0x080L
#define DS_NOIDLEMSG             0x100L
#define DS_SETFOREGROUND         0x200L

#define DM_GETDEFID                (WM_USER + 0)
#define DM_SETDEFID                (WM_USER + 1)

#define DC_HASDEFID              0x534B

#define DLGC_WANTARROWS          0x0001
#define DLGC_WANTTAB             0x0002
#define DLGC_WANTALLKEYS         0x0004
#define DLGC_WANTMESSAGE         0x0004
#define DLGC_HASSETSEL           0x0008
#define DLGC_DEFPUSHBUTTON       0x0010
#define DLGC_UNDEFPUSHBUTTON     0x0020
#define DLGC_RADIOBUTTON         0x0040
#define DLGC_WANTCHARS           0x0080
#define DLGC_STATIC              0x0100
#define DLGC_BUTTON              0x2000



/* OpenFile mode flags
 */
#define OF_READ                      0x00000000
#define OF_WRITE                     0x00000001
#define OF_READWRITE                 0x00000002
#define OF_SHARE_COMPAT              0x00000000
#define OF_SHARE_EXCLUSIVE           0x00000010
#define OF_SHARE_DENY_WRITE          0x00000020
#define OF_SHARE_DENY_READ           0x00000030
#define OF_SHARE_DENY_NONE           0x00000040
#define OF_PARSE                     0x00000100
#define OF_DELETE                    0x00000200
#define OF_VERIFY                    0x00000400
#define OF_CANCEL                    0x00000800
#define OF_CREATE                    0x00001000
#define OF_PROMPT                    0x00002000
#define OF_EXIST                     0x00004000
#define OF_REOPEN                    0x00008000

/* Generic access flags for CreateFile
 */
#define GENERIC_READ                 0x80000000
#define GENERIC_WRITE                0x40000000
#define GENERIC_EXECUTE              0x20000000
#define GENERIC_ALL                  0x10000000

/* Share & Attribute flags for CreateFile
 */
#define FILE_SHARE_READ              0x00000001
#define FILE_SHARE_WRITE             0x00000002
#define FILE_ATTRIBUTE_READONLY      0x00000001
#define FILE_ATTRIBUTE_HIDDEN        0x00000002
#define FILE_ATTRIBUTE_SYSTEM        0x00000004
#define FILE_ATTRIBUTE_DIRECTORY     0x00000010
#define FILE_ATTRIBUTE_ARCHIVE       0x00000020
#define FILE_ATTRIBUTE_NORMAL        0x00000080
#define FILE_ATTRIBUTE_TEMPORARY     0x00000100
#define FILE_ATTRIBUTE_ATOMIC_WRITE  0x00000200
#define FILE_ATTRIBUTE_XACTION_WRITE 0x00000400
#define FILE_ATTRIBUTE_COMPRESSED    0x00000800
#define FILE_ATTRIBUTE_HAS_EMBEDDING 0x00001000

/* Create option for CreateFile
 */
#define CREATE_NEW                   1
#define CREATE_ALWAYS                2
#define OPEN_EXISTING                3
#define OPEN_ALWAYS                  4
#define TRUNCATE_EXISTING            5

/* Access file flags
 */
#define FILE_FLAG_WRITE_THROUGH      0x80000000
#define FILE_FLAG_OVERLAPPED         0x40000000
#define FILE_FLAG_NO_BUFFERING       0x20000000
#define FILE_FLAG_RANDOM_ACCESS      0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN    0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE    0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS   0x02000000
#define FILE_FLAG_POSIX_SEMANTICS    0x01000000

/* SetFilePointer Flags
 */
#define FILE_BEGIN                   0
#define FILE_CURRENT                 1
#define FILE_END                     2

/* GetFileType Flags
 */
#define FILE_TYPE_UNKNOWN            0x00000000
#define FILE_TYPE_DISK               0x00000001
#define FILE_TYPE_CHAR               0x00000002
#define FILE_TYPE_PIPE               0x00000003
#define FILE_TYPE_REMOTE             0x00008000

/* GetVolumeInformation Flags
 */
#define FS_CASE_IS_PRESERVED         FILE_CASE_PRESERVED_NAMES
#define FS_CASE_SENSITIVE            FILE_CASE_SENSITIVE_SEARCH
#define FS_UNICODE_STORED_ON_DISK    FILE_UNICODE_ON_DISK


/* DllEntryPoint flags
 */
#define DLL_PROCESS_DETACH           0
#define DLL_PROCESS_ATTACH           1


/* GetQueueStatus Flags
 */
#define QS_KEY           0x01
#define QS_MOUSEMOVE     0x02
#define QS_MOUSEBUTTON   0x04
#define QS_POSTMESSAGE   0x08
#define QS_TIMER         0x10
#define QS_PAINT         0x20
#define QS_SENDMESSAGE   0x40
#define QS_HOTKEY        0x80
#define QS_MOUSE        (QS_MOUSEMOVE | QS_MOUSEBUTTON)
#define QS_INPUT        (QS_MOUSE     | QS_KEY)
#define QS_ALLEVENTS    (QS_INPUT     | QS_POSTMESSAGE   | QS_TIMER         | \
                           QS_PAINT     | QS_HOTKEY)
#define QS_ALLINPUT     (QS_SENDMESSAGE  | QS_PAINT      | QS_TIMER         | \
                           QS_POSTMESSAGE  | QS_MOUSEBUTTON | QS_MOUSEMOVE     | \
                           QS_HOTKEY       | QS_KEY)


/* Maximum path length
 */
#define  MAX_PATH                    260

/* OPENFILENAME struct flags
 */
#define OFN_READONLY                 0x00000001
#define OFN_OVERWRITEPROMPT          0x00000002
#define OFN_HIDEREADONLY             0x00000004
#define OFN_NOCHANGEDIR              0x00000008
#define OFN_SHOWHELP                 0x00000010
#define OFN_ENABLEHOOK               0x00000020
#define OFN_ENABLETEMPLATE           0x00000040
#define OFN_ENABLETEMPLATEHANDLE     0x00000080
#define OFN_NOVALIDATE               0x00000100
#define OFN_ALLOWMULTISELECT         0x00000200
#define OFN_EXTENSIONDIFFERENT       0x00000400
#define OFN_PATHMUSTEXIST            0x00000800
#define OFN_FILEMUSTEXIST            0x00001000
#define OFN_CREATEPROMPT             0x00002000
#define OFN_SHAREAWARE               0x00004000
#define OFN_NOREADONLYRETURN         0x00008000
#define OFN_NOTESTFILECREATE         0x00010000
#define OFN_NONETWORKBUTTON          0x00020000
#define OFN_NOLONGNAMES              0x00040000

/* SHAREVISTRING Return Values
 */
#define OFN_SHAREWARN                0
#define OFN_SHARENOWARN              1
#define OFN_SHAREFALLTHROUGH         2

/* Values for high word of lparam of LBSELCHSTRING messages
 */
#define CD_LBSELNOITEMS             -1
#define CD_LBSELCHANGE               0
#define CD_LBSELSUB                  1
#define CD_LBSELADD                  2

/* PeekMessage Options
 */
#define PM_NOREMOVE          0x0000
#define PM_REMOVE            0x0001
#define PM_NOYIELD           0x0002

/* Edit Control Styles
 */
#define ES_LEFT              0x0000L
#define ES_CENTER            0x0001L
#define ES_RIGHT             0x0002L
#define ES_MULTILINE         0x0004L
#define ES_UPPERCASE         0x0008L
#define ES_LOWERCASE         0x0010L
#define ES_PASSWORD          0x0020L
#define ES_AUTOVSCROLL       0x0040L
#define ES_AUTOHSCROLL       0x0080L
#define ES_NOHIDESEL         0x0100L
#define ES_OEMCONVERT        0x0400L
#define ES_READONLY          0x0800L
#define ES_WANTRETURN        0x1000L

/* Edit Control Notification Codes
 */
#define EN_SETFOCUS          0x0100
#define EN_KILLFOCUS         0x0200
#define EN_CHANGE            0x0300
#define EN_UPDATE            0x0400
#define EN_ERRSPACE          0x0500
#define EN_MAXTEXT           0x0501
#define EN_HSCROLL           0x0601
#define EN_VSCROLL           0x0602


/* Listbox Styles
 */
#define LBS_NOTIFY               0x0001L
#define LBS_SORT                 0x0002L
#define LBS_NOREDRAW             0x0004L
#define LBS_MULTIPLESEL          0x0008L
#define LBS_OWNERDRAWFIXED       0x0010L
#define LBS_OWNERDRAWVARIABLE    0x0020L
#define LBS_HASSTRINGS           0x0040L
#define LBS_USETABSTOPS          0x0080L
#define LBS_NOINTEGRALHEIGHT     0x0100L
#define LBS_MULTICOLUMN          0x0200L
#define LBS_WANTKEYBOARDINPUT    0x0400L
#define LBS_EXTENDEDSEL          0x0800L
#define LBS_DISABLENOSCROLL      0x1000L
#define LBS_NODATA               0x2000L
#define LBS_STANDARD             ( LBS_NOTIFY | LBS_SORT | \
                                     WS_VSCROLL | WS_BORDER )

/* Listbox Return Values
 */
#define LB_ERRSPACE          (-2)
#define LB_ERR               (-1)
#define LB_OKAY              0

/* Listbox Notification Codes
 */
#define LBN_ERRSPACE         (-2)
#define LBN_SELCHANGE        1
#define LBN_DBLCLK           2
#define LBN_SELCANCEL        3
#define LBN_SETFOCUS         4
#define LBN_KILLFOCUS        5

/* DlgDirList, DlgDirListComboBox flags values
 */
#define DDL_READWRITE        0x0000
#define DDL_READONLY         0x0001
#define DDL_HIDDEN           0x0002
#define DDL_SYSTEM           0x0004
#define DDL_DIRECTORY        0x0010
#define DDL_ARCHIVE          0x0020

#define DDL_POSTMSGS         0x2000
#define DDL_DRIVES           0x4000
#define DDL_EXCLUSIVE        0x8000

/* Combo Box return Values
 */
#define CB_ERRSPACE          (-2)
#define CB_ERR               (-1)
#define CB_OKAY              0

/* Combo Box Notification Codes
 */
#define CBN_ERRSPACE         (-1)
#define CBN_SELCHANGE        1
#define CBN_DBLCLK           2
#define CBN_SETFOCUS         3
#define CBN_KILLFOCUS        4
#define CBN_EDITCHANGE       5
#define CBN_EDITUPDATE       6
#define CBN_DROPDOWN         7
#define CBN_CLOSEUP          8
#define CBN_SELENDOK         9
#define CBN_SELENDCANCEL     10

/* Combo Box styles
 */
#define CBS_SIMPLE            0x0001L
#define CBS_DROPDOWN          0x0002L
#define CBS_DROPDOWNLIST      0x0003L
#define CBS_OWNERDRAWFIXED    0x0010L
#define CBS_OWNERDRAWVARIABLE 0x0020L
#define CBS_AUTOHSCROLL       0x0040L
#define CBS_OEMCONVERT        0x0080L
#define CBS_SORT              0x0100L
#define CBS_HASSTRINGS        0x0200L
#define CBS_NOINTEGRALHEIGHT  0x0400L
#define CBS_DISABLENOSCROLL   0x0800L

/* Scroll Bar Constants
 */
#define SB_HORZ              0
#define SB_VERT              1
#define SB_CTL               2
#define SB_BOTH              3

#define ESB_ENABLE_BOTH      0
#define ESB_DISABLE_LTUP     1
#define ESB_DISABLE_RTDN     2
#define ESB_DISABLE_BOTH     3

/* Scroll Bar Commands
 */
#define SB_LINEUP            0
#define SB_LINELEFT          0
#define SB_LINEDOWN          1
#define SB_LINERIGHT         1
#define SB_PAGEUP            2
#define SB_PAGELEFT          2
#define SB_PAGEDOWN          3
#define SB_PAGERIGHT         3
#define SB_THUMBPOSITION     4
#define SB_THUMBTRACK        5
#define SB_TOP               6
#define SB_LEFT              6
#define SB_BOTTOM            7
#define SB_RIGHT             7
#define SB_ENDSCROLL         8

/* Scroll Bar Styles
 */
#define SBS_HORZ                    0x0000L
#define SBS_VERT                    0x0001L
#define SBS_TOPALIGN                0x0002L
#define SBS_LEFTALIGN               0x0002L
#define SBS_BOTTOMALIGN             0x0004L
#define SBS_RIGHTALIGN              0x0004L
#define SBS_SIZEBOXTOPLEFTALIGN     0x0002L
#define SBS_SIZEBOXBOTTOMRIGHTALIGN 0x0004L
#define SBS_SIZEBOX                 0x0008L


/* Static Control Constants
 */
#define SS_LEFT              0x00L
#define SS_CENTER            0x01L
#define SS_RIGHT             0x02L
#define SS_ICON              0x03L
#define SS_BLACKRECT         0x04L
#define SS_GRAYRECT          0x05L
#define SS_WHITERECT         0x06L
#define SS_BLACKFRAME        0x07L
#define SS_GRAYFRAME         0x08L
#define SS_WHITEFRAME        0x09L
#define SS_USERITEM          0x0AL
#define SS_SIMPLE            0x0BL
#define SS_LEFTNOWORDWRAP    0x0CL
#define SS_NOPREFIX          0x80L


/* Button Control Styles
 */
#define BS_PUSHBUTTON        0x00L
#define BS_DEFPUSHBUTTON     0x01L
#define BS_CHECKBOX          0x02L
#define BS_AUTOCHECKBOX      0x03L
#define BS_RADIOBUTTON       0x04L
#define BS_3STATE            0x05L
#define BS_AUTO3STATE        0x06L
#define BS_GROUPBOX          0x07L
#define BS_USERBUTTON        0x08L
#define BS_AUTORADIOBUTTON   0x09L
#define BS_OWNERDRAW         0x0BL
#define BS_LEFTTEXT          0x20L


/* User Button Notification Codes
 */
#define BN_CLICKED           0
#define BN_PAINT             1
#define BN_HILITE            2
#define BN_UNHILITE          3
#define BN_DISABLE           4
#define BN_DOUBLECLICKED     5

/* Button Control Messages
 */
#define BM_GETCHECK          0x00F0
#define BM_SETCHECK          0x00F1
#define BM_GETSTATE          0x00F2
#define BM_SETSTATE          0x00F3
#define BM_SETSTYLE          0x00F4

/* Owner draw control types
 */
#define ODT_MENU        1
#define ODT_LISTBOX     2
#define ODT_COMBOBOX    3
#define ODT_BUTTON      4
#define ODT_STATIC      5

/* Owner draw actions
 */
#define ODA_DRAWENTIRE  0x0001
#define ODA_SELECT      0x0002
#define ODA_FOCUS       0x0004

/* Owner draw state
 */
#define ODS_SELECTED    0x0001
#define ODS_GRAYED      0x0002
#define ODS_DISABLED    0x0004
#define ODS_CHECKED     0x0008
#define ODS_FOCUS       0x0010
#define ODS_DEFAULT     0x0020
#define ODS_COMBOBOXEDIT 0x1000


/* Stock Logical Objects
 */
#define WHITE_BRUSH          0
#define LTGRAY_BRUSH         1
#define GRAY_BRUSH           2
#define DKGRAY_BRUSH         3
#define BLACK_BRUSH          4
#define NULL_BRUSH           5
#define WHITE_PEN            6
#define BLACK_PEN            7
#define NULL_PEN             8
#define OEM_FIXED_FONT       10
#define ANSI_FIXED_FONT      11
#define ANSI_VAR_FONT        12
#define SYSTEM_FONT          13
#define DEVICE_DEFAULT_FONT  14
#define DEFAULT_PALETTE      15
#define SYSTEM_FIXED_FONT    16
#define STOCK_LAST           16
#define HOLLOW_BRUSH         NULL_BRUSH

/* Brush Styles
 */
#define BS_SOLID             0
#define BS_NULL              1
#define BS_HATCHED           2
#define BS_PATTERN           3
#define BS_INDEXED           4
#define BS_DIBPATTERN        5
#define BS_DIBPATTERNPT      6
#define BS_PATTERN8X8        7
#define BS_DIBPATTERN8X8     8
#define BS_HOLLOW            BS_NULL

/* Hatch Styles
 */
#define HS_HORIZONTAL        0
#define HS_VERTICAL          1
#define HS_FDIAGONAL         2
#define HS_BDIAGONAL         3
#define HS_CROSS             4
#define HS_DIAGCROSS         5
#define HS_SOLIDCLR          6
#define HS_DITHEREDCLR       7
#define HS_SOLIDTEXTCLR      8
#define HS_DITHEREDTEXTCLR   9
#define HS_SOLIDBKCLR        10
#define HS_DITHEREDBKCLR     11
#define HS_API_MAX           12

/* Pen Styles
 */
#define PS_SOLID             0
#define PS_DASH              1
#define PS_DOT               2
#define PS_DASHDOT           3
#define PS_DASHDOTDOT        4
#define PS_NULL              5
#define PS_INSIDEFRAME       6
#define PS_USERSTYLE         7
#define PS_ALTERNATE         8
#define PS_STYLE_MASK        0x0000000F

#define PS_COSMETIC          0x00000000
#define PS_ENDCAP_ROUND      0x00000000
#define PS_JOIN_ROUND        0x00000000
#define PS_ENDCAP_SQUARE     0x00000100
#define PS_ENDCAP_FLAT       0x00000200
#define PS_ENDCAP_MASK       0x00000F00
#define PS_JOIN_BEVEL        0x00001000
#define PS_JOIN_MITER        0x00002000
#define PS_JOIN_MASK         0x0000F000
#define PS_GEOMETRIC         0x00010000
#define PS_TYPE_MASK         0x000F0000

/* Region Flags
 */
#define ERROR                0
#define NULLREGION           1
#define SIMPLEREGION         2
#define COMPLEXREGION        3
#define RGN_ERROR            ERROR

#define GDI_ERROR            0xFFFFFFFF
#define CLR_INVALID          0xFFFFFFFF


/* CombineRgn Styles
 */
#define RGN_AND              1
#define RGN_OR               2
#define RGN_XOR              3
#define RGN_DIFF             4
#define RGN_COPY             5
#define RGN_MIN              RGN_AND
#define RGN_MAX              RGN_COPY

/* PolyFill Modes
 */
#define ALTERNATE            1
#define WINDING              2

/* Binary Raster Operations
 */
#define R2_BLACK            1
#define R2_NOTMERGEPEN      2
#define R2_MASKNOTPEN       3
#define R2_NOTCOPYPEN       4
#define R2_MASKPENNOT       5
#define R2_NOT              6
#define R2_XORPEN           7
#define R2_NOTMASKPEN       8
#define R2_MASKPEN          9
#define R2_NOTXORPEN        10
#define R2_NOP              11
#define R2_MERGENOTPEN      12
#define R2_COPYPEN          13
#define R2_MERGEPENNOT      14
#define R2_MERGEPEN         15
#define R2_WHITE            16
#define R2_LAST             16

/* Ternary raster operations
 */
#define SRCCOPY              0x00CC0020
#define SRCPAINT             0x00EE0086
#define SRCAND               0x008800C6
#define SRCINVERT            0x00660046
#define SRCERASE             0x00440328
#define NOTSRCCOPY           0x00330008
#define NOTSRCERASE          0x001100A6
#define MERGECOPY            0x00C000CA
#define MERGEPAINT           0x00BB0226
#define PATCOPY              0x00F00021
#define PATPAINT             0x00FB0A09
#define PATINVERT            0x005A0049
#define DSTINVERT            0x00550009
#define BLACKNESS            0x00000042
#define WHITENESS            0x00FF0062

/* Helper macro for MaskBlt()
 */

#define MAKEROP4(a,b) (DWORD)((((b) << 8) & 0xFF000000) | (a))


/* Spooler Error Codes
 */
#define SP_OUTOFMEMORY       (-5)
#define SP_OUTOFDISK         (-4)
#define SP_USERABORT         (-3)
#define SP_APPABORT          (-2)
#define SP_ERROR             (-1)
#define SP_NOTREPORTED       0x4000

/* Predefined Clipboard Formats
 */
#define CF_TEXT              1
#define CF_BITMAP            2
#define CF_METAFILEPICT      3
#define CF_SYLK              4
#define CF_DIF               5
#define CF_TIFF              6
#define CF_OEMTEXT           7
#define CF_DIB               8
#define CF_PALETTE           9
#define CF_PENDATA           10
#define CF_RIFF              11
#define CF_WAVE              12
#define CF_UNICODETEXT       13
#define CF_ENHMETAFILE       14

#define CF_OWNERDISPLAY      0x0080
#define CF_DSPTEXT           0x0081
#define CF_DSPBITMAP         0x0082
#define CF_DSPMETAFILEPICT   0x0083
#define CF_DSPENHMETAFILE    0x008E
#define CF_PRIVATEFIRST      0x0200
#define CF_PRIVATELAST       0x02FF
#define CF_GDIOBJFIRST       0x0300
#define CF_GDIOBJLAST        0x03FF

/* GetSystemMetrics() codes
 */
#define SM_CXSCREEN          0
#define SM_CYSCREEN          1
#define SM_CXVSCROLL         2
#define SM_CYHSCROLL         3
#define SM_CYCAPTION         4
#define SM_CXBORDER          5
#define SM_CYBORDER          6
#define SM_CXDLGFRAME        7
#define SM_CYDLGFRAME        8
#define SM_CYVTHUMB          9
#define SM_CXHTHUMB          10
#define SM_CXICON            11
#define SM_CYICON            12
#define SM_CXCURSOR          13
#define SM_CYCURSOR          14
#define SM_CYMENU            15
#define SM_CXFULLSCREEN      16
#define SM_CYFULLSCREEN      17
#define SM_CYKANJIWINDOW     18
#define SM_MOUSEPRESENT      19
#define SM_CYVSCROLL         20
#define SM_CXHSCROLL         21
#define SM_DEBUG             22
#define SM_SWAPBUTTON        23
#define SM_RESERVED1         24
#define SM_RESERVED2         25
#define SM_RESERVED3         26
#define SM_RESERVED4         27
#define SM_CXMIN             28
#define SM_CYMIN             29
#define SM_CXSIZE            30
#define SM_CYSIZE            31
#define SM_CXFRAME           32
#define SM_CYFRAME           33
#define SM_CXMINTRACK        34
#define SM_CYMINTRACK        35
#define SM_CXDOUBLECLK       36
#define SM_CYDOUBLECLK       37
#define SM_MENUDROPALIGNMENT 40
#define SM_CMOUSEBUTTONS     43
#define SM_CMETRICS          44


/* Flags for TrackPopupMenu
 */
#define TPM_LEFTALIGN        0x0000
#define TPM_LEFTBUTTON       0x0000
#define TPM_RIGHTBUTTON      0x0002
#define TPM_CENTERALIGN      0x0004
#define TPM_RIGHTALIGN       0x0008

/* SetWindowsHook codes
 */
#define WH_MIN               (-1)
#define WH_MSGFILTER         (-1)
#define WH_JOURNALRECORD     0
#define WH_JOURNALPLAYBACK   1
#define WH_KEYBOARD          2
#define WH_GETMESSAGE        3
#define WH_CALLWNDPROC       4
#define WH_CBT               5
#define WH_SYSMSGFILTER      6
#define WH_MOUSE             7
#define WH_HARDWARE          8
#define WH_DEBUG             9
#define WH_SHELL             10
#define WH_FOREGROUNDIDLE    11
#define WH_MAX               11

/* Hook Codes
 */
#define HC_ACTION            0
#define HC_GETNEXT           1
#define HC_SKIP              2
#define HC_NOREMOVE          3
#define HC_SYSMODALON        4
#define HC_SYSMODALOFF       5
#define HC_NOREM             HC_NOREMOVE

#define DRIVE_UNKNOWN        0
#define DRIVE_NO_ROOT_DIR    1
#define DRIVE_REMOVABLE      2
#define DRIVE_FIXED          3
#define DRIVE_REMOTE         4
#define DRIVE_CDROM          5
#define DRIVE_RAMDISK        6


/* Color Types
 */
#define CTLCOLOR_MSGBOX      0
#define CTLCOLOR_EDIT        1
#define CTLCOLOR_LISTBOX     2
#define CTLCOLOR_BTN         3
#define CTLCOLOR_DLG         4
#define CTLCOLOR_SCROLLBAR   5
#define CTLCOLOR_STATIC      6
#define CTLCOLOR_MAX         8

#define COLOR_SCROLLBAR              0
#define COLOR_BACKGROUND             1
#define COLOR_ACTIVECAPTION          2
#define COLOR_INACTIVECAPTION        3
#define COLOR_MENU                   4
#define COLOR_WINDOW                 5
#define COLOR_WINDOWFRAME            6
#define COLOR_MENUTEXT               7
#define COLOR_WINDOWTEXT             8
#define COLOR_CAPTIONTEXT            9
#define COLOR_ACTIVEBORDER           10
#define COLOR_INACTIVEBORDER         11
#define COLOR_APPWORKSPACE           12
#define COLOR_HIGHLIGHT              13
#define COLOR_HIGHLIGHTTEXT          14
#define COLOR_BTNFACE                15
#define COLOR_BTNSHADOW              16
#define COLOR_GRAYTEXT               17
#define COLOR_BTNTEXT                18
#define COLOR_INACTIVECAPTIONTEXT    19
#define COLOR_BTNHIGHLIGHT           20

/* Enhanced PM SysColors
 */
#define SYSCLR_SHADOWHILITEBGND       (-50L)
#define SYSCLR_SHADOWHILITEFGND       (-49L)
#define SYSCLR_SHADOWTEXT             (-48L)
#define SYSCLR_ENTRYFIELD             (-47L)
#define SYSCLR_MENUDISABLEDTEXT       (-46L)
#define SYSCLR_MENUHILITE             (-45L)
#define SYSCLR_MENUHILITEBGND         (-44L)
#define SYSCLR_PAGEBACKGROUND         (-43L)
#define SYSCLR_FIELDBACKGROUND        (-42L)
#define SYSCLR_BUTTONLIGHT            (-41L)
#define SYSCLR_BUTTONMIDDLE           (-40L)
#define SYSCLR_BUTTONDARK             (-39L)
#define SYSCLR_BUTTONDEFAULT          (-38L)
#define SYSCLR_TITLEBOTTOM            (-37L)
#define SYSCLR_SHADOW                 (-36L)
#define SYSCLR_ICONTEXT               (-35L)
#define SYSCLR_DIALOGBACKGROUND       (-34L)
#define SYSCLR_HILITEFOREGROUND       (-33L)
#define SYSCLR_HILITEBACKGROUND       (-32L)
#define SYSCLR_INACTIVETITLETEXTBGND  (-31L)
#define SYSCLR_ACTIVETITLETEXTBGND    (-30L)
#define SYSCLR_INACTIVETITLETEXT      (-29L)
#define SYSCLR_ACTIVETITLETEXT        (-28L)
#define SYSCLR_OUTPUTTEXT             (-27L)
#define SYSCLR_WINDOWSTATICTEXT       (-26L)
#define SYSCLR_SCROLLBAR              (-25L)
#define SYSCLR_BACKGROUND             (-24L)
#define SYSCLR_ACTIVETITLE            (-23L)
#define SYSCLR_INACTIVETITLE          (-22L)
#define SYSCLR_MENU                   (-21L)
#define SYSCLR_WINDOW                 (-20L)
#define SYSCLR_WINDOWFRAME            (-19L)
#define SYSCLR_MENUTEXT               (-18L)
#define SYSCLR_WINDOWTEXT             (-17L)
#define SYSCLR_TITLETEXT              (-16L)
#define SYSCLR_ACTIVEBORDER           (-15L)
#define SYSCLR_INACTIVEBORDER         (-14L)
#define SYSCLR_APPWORKSPACE           (-13L)
#define SYSCLR_HELPBACKGROUND         (-12L)
#define SYSCLR_HELPTEXT               (-11L)
#define SYSCLR_HELPHILITE             (-10L)


/* Draw Text flags
 */
#define DT_TOP               0x00000000
#define DT_LEFT              0x00000000
#define DT_CENTER            0x00000001
#define DT_RIGHT             0x00000002
#define DT_VCENTER           0x00000004
#define DT_BOTTOM            0x00000008
#define DT_WORDBREAK         0x00000010
#define DT_SINGLELINE        0x00000020
#define DT_EXPANDTABS        0x00000040
#define DT_TABSTOP           0x00000080
#define DT_NOCLIP            0x00000100
#define DT_EXTERNALLEADING   0x00000200
#define DT_CALCRECT          0x00000400
#define DT_NOPREFIX          0x00000800
#define DT_VALIDFLAGS        0x00000FFF

/* Text Alignment Options
 */
#define TA_LEFT              0
#define TA_NOUPDATECP        0
#define TA_TOP               0
#define TA_UPDATECP          1
#define TA_RIGHT             2
#define TA_CENTER            6
#define TA_BOTTOM            8
#define TA_BASELINE          24
#define TA_MASK              ( TA_BASELINE + TA_CENTER + TA_UPDATECP )

#define VTA_BASELINE         TA_BASELINE
#define VTA_LEFT             TA_BOTTOM
#define VTA_RIGHT            TA_TOP
#define VTA_CENTER           TA_CENTER
#define VTA_BOTTOM           TA_RIGHT
#define VTA_TOP              TA_LEFT


/* Memory APIs
 */
#define PAGE_NOACCESS            0x00000001
#define PAGE_READONLY            0x00000002
#define PAGE_READWRITE           0x00000004
#define PAGE_WRITECOPY           0x00000008
#define PAGE_EXECUTE             0x00000010
#define PAGE_EXECUTE_READ        0x00000020
#define PAGE_EXECUTE_READWRITE   0x00000040
#define PAGE_EXECUTE_WRITECOPY   0x00000080
#define PAGE_GUARD               0x00000100
#define PAGE_NOCACHE             0x00000200
#define MEM_COMMIT               0x00001000
#define MEM_RESERVE              0x00002000
#define MEM_DECOMMIT             0x00004000
#define MEM_RELEASE              0x00008000
#define MEM_FREE                 0x00010000
#define MEM_PRIVATE              0x00020000
#define MEM_MAPPED               0x00040000
#define MEM_TOP_DOWN             0x00100000
#define SEC_FILE                 0x00800000
#define SEC_IMAGE                0x01000000
#define SEC_RESERVE              0x04000000
#define SEC_COMMIT               0x08000000
#define SEC_NOCACHE              0x10000000
#define MEM_IMAGE                SEC_IMAGE

/* Global Memory Flags
 */
#define GMEM_FIXED           0x0000
#define GMEM_MOVEABLE        0x0002
#define GMEM_NOCOMPACT       0x0010
#define GMEM_NODISCARD       0x0020
#define GMEM_ZEROINIT        0x0040
#define GMEM_MODIFY          0x0080
#define GMEM_LOCKCOUNT       0x00FF
#define GMEM_DISCARDABLE     0x0100
#define GMEM_NOT_BANKED      0x1000
#define GMEM_SHARE           0x2000
#define GMEM_DDESHARE        0x2000
#define GMEM_NOTIFY          0x4000
#define GMEM_DISCARDED       0x4000
#define GMEM_INVALID_HANDLE  0x8000
#define GMEM_LOWER           GMEM_NOT_BANKED
#define GHND                 ( GMEM_MOVEABLE | GMEM_ZEROINIT )
#define GPTR                 ( GMEM_FIXED    | GMEM_ZEROINIT )

/* Local Memory Flags
 */
#define LMEM_FIXED           0x0000
#define LMEM_MOVEABLE        0x0002
#define LMEM_NOCOMPACT       0x0010
#define LMEM_NODISCARD       0x0020
#define LMEM_ZEROINIT        0x0040
#define LMEM_MODIFY          0x0080
#define LMEM_LOCKCOUNT       0x00FF
#define LMEM_DISCARDABLE     0x0F00
#define LMEM_VALID_FLAGS     0x0F72
#define LMEM_DISCARDED       0x4000
#define LMEM_INVALID_HANDLE  0x8000
#define LHND                 ( LMEM_MOVEABLE | LMEM_ZEROINIT )
#define LPTR                 ( LMEM_FIXED    | LMEM_ZEROINIT )
#define NONZEROLHND          ( LMEM_MOVEABLE )
#define NONZEROLPTR          ( LMEM_FIXED )


/* constants for HeapAlloc() flags
 */
#define HEAP_NO_SERIALIZE                0x00000001
#define HEAP_GROWABLE                    0x00000002
#define HEAP_GENERATE_EXCEPTIONS         0x00000004
#define HEAP_ZERO_MEMORY                 0x00000008
#define HEAP_REALLOC_IN_PLACE_ONLY       0x00000010
#define HEAP_TAIL_CHECKING_ENABLED       0x00000020
#define HEAP_FREE_CHECKING_ENABLED       0x00000040
#define HEAP_DISABLE_COALESCE_ON_FREE    0x00000080


/* System API Flags
 */
#define INFINITE                     ((DWORD)-1)

#define DELETE                       0x00010000
#define READ_CONTROL                 0x00020000
#define WRITE_DAC                    0x00040000
#define WRITE_OWNER                  0x00080000
#define SYNCHRONIZE                  0x00100000

#define SPECIFIC_RIGHTS_ALL          0x0000FFFF
#define STANDARD_RIGHTS_READ         (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE        (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE      (READ_CONTROL)
#define STANDARD_RIGHTS_REQUIRED     0x000F0000
#define STANDARD_RIGHTS_ALL          0x001F0000

/* Thread Creation Flags
 */
#define DEBUG_PROCESS                0x00000001
#define DEBUG_ONLY_THIS_PROCESS      0x00000002
#define CREATE_SUSPENDED             0x00000004
#define DETACHED_PROCESS             0x00000008
#define CREATE_NEW_CONSOLE           0x00000010
#define NORMAL_PRIORITY_CLASS        0x00000020
#define IDLE_PRIORITY_CLASS          0x00000040
#define HIGH_PRIORITY_CLASS          0x00000080
#define REALTIME_PRIORITY_CLASS      0x00000100
#define CREATE_NEW_PROCESS_GROUP     0x00000200
#define CREATE_UNICODE_ENVIRONMENT   0x00000400
#define CREATE_SEPARATE_WOW_VDM      0x00000800
#define CREATE_SHARE_WOW_VDM         0x00001000
#define CREATE_DEFAULT_ERROR_MODE    0x04000000
#define CREATE_NO_WINDOW             0x08000000

#define PROCESS_TERMINATE            0x00000001
#define PROCESS_CREATE_THREAD        0x00000002
#define PROCESS_VM_OPERATION         0x00000008
#define PROCESS_VM_READ              0x00000010
#define PROCESS_VM_WRITE             0x00000020
#define PROCESS_DUP_HANDLE           0x00000040
#define PROCESS_CREATE_PROCESS       0x00000080
#define PROCESS_SET_QUOTA            0x00000100
#define PROCESS_SET_INFORMATION      0x00000200
#define PROCESS_QUERY_INFORMATION    0x00000400
#define PROCESS_ALL_ACCESS           (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0xFFF)

#define THREAD_TERMINATE             0x00000001
#define THREAD_SUSPEND_RESUME        0x00000002
#define THREAD_GET_CONTEXT           0x00000008
#define THREAD_SET_CONTEXT           0x00000010
#define THREAD_SET_INFORMATION       0x00000020
#define THREAD_QUERY_INFORMATION     0x00000040
#define THREAD_SET_THREAD_TOKEN      0x00000080
#define THREAD_IMPERSONATE           0x00000100
#define THREAD_DIRECT_IMPERSONATION  0x00000200
#define THREAD_ALL_ACCESS            (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3FF)

#define TLS_MINIMUM_AVAILABLE        64

#define STILL_ACTIVE                 0x00000103

#define THREAD_BASE_PRIORITY_IDLE   -15
#define THREAD_BASE_PRIORITY_MIN    -2
#define THREAD_BASE_PRIORITY_MAX     2
#define THREAD_BASE_PRIORITY_LOWRT   15

#define THREAD_PRIORITY_IDLE         -15
#define THREAD_PRIORITY_LOWEST        -2
#define THREAD_PRIORITY_BELOW_NORMAL  -1
#define THREAD_PRIORITY_NORMAL         0
#define THREAD_PRIORITY_ABOVE_NORMAL   1
#define THREAD_PRIORITY_HIGHEST        2
#define THREAD_PRIORITY_TIME_CRITICAL 15

/* Synchronization APIs
 */
#define MAXIMUM_WAIT_OBJECTS         128
#define STATUS_ABANDONED_WAIT_0      0x00000080
#define STATUS_TIMEOUT               0x00000102
#define STATUS_WAIT_0                0x00000000
#define WAIT_ABANDONED               STATUS_ABANDONED_WAIT_0
#define WAIT_ABANDONED_0             STATUS_ABANDONED_WAIT_0
#define WAIT_FAILED                  ((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0                STATUS_WAIT_0
#define WAIT_TIMEOUT                 STATUS_TIMEOUT

#define EVENT_MODIFY_STATE           0x00000002
#define EVENT_ALL_ACCESS             (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|EVENT_MODIFY_STATE)

#define MUTEX_ALL_ACCESS             (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE)

#define SEMAPHORE_MODIFY_STATE       0x00000002
#define SEMAPHORE_ALL_ACCESS         (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|SEMAPHORE_MODIFY_STATE)

/* File IO APIs
 */
#define FILE_READ_DATA               0x00000001
#define FILE_LIST_DIRECTORY          0x00000001
#define FILE_WRITE_DATA              0x00000002
#define FILE_ADD_FILE                0x00000002
#define FILE_APPEND_DATA             0x00000004
#define FILE_ADD_SUBDIRECTORY        0x00000004
#define FILE_CREATE_PIPE_INSTANCE    0x00000004
#define FILE_READ_EA                 0x00000008
#define FILE_READ_PROPERTIES         0x00000008
#define FILE_WRITE_EA                0x00000010
#define FILE_WRITE_PROPERTIES        0x00000010
#define FILE_EXECUTE                 0x00000020
#define FILE_TRAVERSE                0x00000020
#define FILE_DELETE_CHILD            0x00000040
#define FILE_READ_ATTRIBUTES         0x00000080
#define FILE_WRITE_ATTRIBUTES        0x00000100
#define FILE_ALL_ACCESS              (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)
#define FILE_GENERIC_READ            (STANDARD_RIGHTS_READ     |\
                                        FILE_READ_DATA           |\
                                        FILE_READ_ATTRIBUTES     |\
                                        FILE_READ_EA             |\
                                        SYNCHRONIZE)
#define FILE_GENERIC_WRITE           (STANDARD_RIGHTS_WRITE    |\
                                        FILE_WRITE_DATA          |\
                                        FILE_WRITE_ATTRIBUTES    |\
                                        FILE_WRITE_EA            |\
                                        FILE_APPEND_DATA         |\
                                        SYNCHRONIZE)
#define FILE_GENERIC_EXECUTE         (STANDARD_RIGHTS_EXECUTE  |\
                                        FILE_READ_ATTRIBUTES     |\
                                        FILE_EXECUTE             |\
                                        SYNCHRONIZE)

#define FILE_SHARE_READ                  0x00000001
#define FILE_SHARE_WRITE                 0x00000002
#define FILE_ATTRIBUTE_READONLY          0x00000001
#define FILE_ATTRIBUTE_HIDDEN            0x00000002
#define FILE_ATTRIBUTE_SYSTEM            0x00000004
#define FILE_ATTRIBUTE_DIRECTORY         0x00000010
#define FILE_ATTRIBUTE_ARCHIVE           0x00000020
#define FILE_ATTRIBUTE_NORMAL            0x00000080
#define FILE_ATTRIBUTE_TEMPORARY         0x00000100
#define FILE_ATTRIBUTE_ATOMIC_WRITE      0x00000200
#define FILE_ATTRIBUTE_XACTION_WRITE     0x00000400
#define FILE_ATTRIBUTE_COMPRESSED        0x00000800
#define FILE_NOTIFY_CHANGE_FILE_NAME     0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME      0x00000002
#define FILE_NOTIFY_CHANGE_ATTRIBUTES    0x00000004
#define FILE_NOTIFY_CHANGE_SIZE          0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE    0x00000010
#define FILE_NOTIFY_CHANGE_SECURITY      0x00000100
#define FILE_CASE_SENSITIVE_SEARCH       0x00000001
#define FILE_CASE_PRESERVED_NAMES        0x00000002
#define FILE_UNICODE_ON_DISK             0x00000004
#define FILE_PERSISTENT_ACLS             0x00000008
#define FILE_FILE_COMPRESSION            0x00000010
#define FILE_VOLUME_IS_COMPRESSED        0x00008000
#define IO_COMPLETION_MODIFY_STATE       0x00000002
#define IO_COMPLETION_ALL_ACCESS         (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3)
#define DUPLICATE_CLOSE_SOURCE           0x00000001
#define DUPLICATE_SAME_ACCESS            0x00000002


/* Exceptions
*/
#define STATUS_NO_MEMORY             (0xC0000017)
#define STATUS_ACCESS_VIOLATION      (0xC0000005)


/* Virtual Keys, Standard Set
 */
#define VK_LBUTTON           0x01
#define VK_RBUTTON           0x02
#define VK_CANCEL            0x03
#define VK_MBUTTON           0x04
#define VK_BACK              0x08
#define VK_TAB               0x09
#define VK_CLEAR             0x0C
#define VK_RETURN            0x0D
#define VK_SHIFT             0x10
#define VK_CONTROL           0x11
#define VK_MENU              0x12
#define VK_PAUSE             0x13
#define VK_CAPITAL           0x14
#define VK_ESCAPE            0x1B
#define VK_SPACE             0x20
#define VK_PRIOR             0x21
#define VK_NEXT              0x22
#define VK_END               0x23
#define VK_HOME              0x24
#define VK_LEFT              0x25
#define VK_UP                0x26
#define VK_RIGHT             0x27
#define VK_DOWN              0x28
#define VK_SELECT            0x29
#define VK_PRINT             0x2A
#define VK_EXECUTE           0x2B
#define VK_SNAPSHOT          0x2C
#define VK_INSERT            0x2D
#define VK_DELETE            0x2E
#define VK_HELP              0x2F
#define VK_LWIN              0x5B
#define VK_RWIN              0x5C
#define VK_APPS              0x5D
#define VK_NUMPAD0           0x60
#define VK_NUMPAD1           0x61
#define VK_NUMPAD2           0x62
#define VK_NUMPAD3           0x63
#define VK_NUMPAD4           0x64
#define VK_NUMPAD5           0x65
#define VK_NUMPAD6           0x66
#define VK_NUMPAD7           0x67
#define VK_NUMPAD8           0x68
#define VK_NUMPAD9           0x69
#define VK_MULTIPLY          0x6A
#define VK_ADD               0x6B
#define VK_SEPARATOR         0x6C
#define VK_SUBTRACT          0x6D
#define VK_DECIMAL           0x6E
#define VK_DIVIDE            0x6F
#define VK_F1                0x70
#define VK_F2                0x71
#define VK_F3                0x72
#define VK_F4                0x73
#define VK_F5                0x74
#define VK_F6                0x75
#define VK_F7                0x76
#define VK_F8                0x77
#define VK_F9                0x78
#define VK_F10               0x79
#define VK_F11               0x7A
#define VK_F12               0x7B
#define VK_F13               0x7C
#define VK_F14               0x7D
#define VK_F15               0x7E
#define VK_F16               0x7F
#define VK_F17               0x80
#define VK_F18               0x81
#define VK_F19               0x82
#define VK_F20               0x83
#define VK_F21               0x84
#define VK_F22               0x85
#define VK_F23               0x86
#define VK_F24               0x87
#define VK_NUMLOCK           0x90
#define VK_SCROLL            0x91
#define VK_LSHIFT            0xA0
#define VK_RSHIFT            0xA1
#define VK_LCONTROL          0xA2
#define VK_RCONTROL          0xA3
#define VK_LMENU             0xA4
#define VK_RMENU             0xA5
#define VK_ATTN              0xF6
#define VK_CRSEL             0xF7
#define VK_EXSEL             0xF8
#define VK_EREOF             0xF9
#define VK_PLAY              0xFA
#define VK_ZOOM              0xFB
#define VK_NONAME            0xFC
#define VK_PA1               0xFD
#define VK_OEM_CLEAR         0xFE


/* constants for Get/SetSystemPaletteUse()
 */
#define SYSPAL_ERROR         0
#define SYSPAL_STATIC        1
#define SYSPAL_NOSTATIC      2


/* RedrawWindow() flags
 */
#define RDW_INVALIDATE       0x0001
#define RDW_INTERNALPAINT    0x0002
#define RDW_ERASE            0x0004
#define RDW_VALIDATE         0x0008
#define RDW_NOINTERNALPAINT  0x0010
#define RDW_NOERASE          0x0020
#define RDW_NOCHILDREN       0x0040
#define RDW_ALLCHILDREN      0x0080
#define RDW_UPDATENOW        0x0100
#define RDW_ERASENOW         0x0200
#define RDW_FRAME            0x0400
#define RDW_NOFRAME          0x0800

/* Bounds Accumulation APIs
 */
#define DCB_RESET             0x0001
#define DCB_ACCUMULATE        0x0002
#define DCB_ENABLE            0x0004
#define DCB_DISABLE           0x0008
#define DCB_DIRTY             DCB_ACCUMULATE
#define DCB_SET               (DCB_RESET | DCB_ACCUMULATE)

/* SetErrorMode() flags
 */
#define SEM_FAILCRITICALERRORS      0x0001
#define SEM_NOGPFAULTERRORBOX       0x0002
#define SEM_NOALIGNMENTFAULTEXCEPT  0x0004
#define SEM_NOOPENFILEERRORBOX      0x8000

/* Parameter for SystemParametersInfo()
 */
#define SPI_GETBEEP                              1
#define SPI_SETBEEP                              2
#define SPI_GETMOUSE                             3
#define SPI_SETMOUSE                             4
#define SPI_GETBORDER                            5
#define SPI_SETBORDER                            6
#define SPI_GETKEYBOARDSPEED                    10
#define SPI_SETKEYBOARDSPEED                    11
#define SPI_LANGDRIVER                          12
#define SPI_ICONHORIZONTALSPACING               13
#define SPI_GETSCREENSAVETIMEOUT                14
#define SPI_SETSCREENSAVETIMEOUT                15
#define SPI_GETSCREENSAVEACTIVE                 16
#define SPI_SETSCREENSAVEACTIVE                 17
#define SPI_GETGRIDGRANULARITY                  18
#define SPI_SETGRIDGRANULARITY                  19
#define SPI_SETDESKWALLPAPER                    20
#define SPI_SETDESKPATTERN                      21
#define SPI_GETKEYBOARDDELAY                    22
#define SPI_SETKEYBOARDDELAY                    23
#define SPI_ICONVERTICALSPACING                 24
#define SPI_GETICONTITLEWRAP                    25
#define SPI_SETICONTITLEWRAP                    26
#define SPI_GETMENUDROPALIGNMENT                27
#define SPI_SETMENUDROPALIGNMENT                28
#define SPI_SETDOUBLECLKWIDTH                   29
#define SPI_SETDOUBLECLKHEIGHT                  30
#define SPI_GETICONTITLELOGFONT                 31
#define SPI_SETDOUBLECLICKTIME                  32
#define SPI_SETMOUSEBUTTONSWAP                  33
#define SPI_SETICONTITLELOGFONT                 34
#define SPI_GETFASTTASKSWITCH                   35
#define SPI_SETFASTTASKSWITCH                   36
#define SPI_SETDRAGFULLWINDOWS                  37
#define SPI_GETDRAGFULLWINDOWS                  38
#define SPI_GETKEYBOARDLAYOUT                   39
#define SPI_SETKEYBOARDLAYOUT                   40
#define SPI_GETNONCLIENTMETRICS                 41
#define SPI_SETNONCLIENTMETRICS                 42
#define SPI_GETMINIMIZEDMETRICS                 43
#define SPI_SETMINIMIZEDMETRICS                 44
#define SPI_GETICONMETRICS                      45
#define SPI_SETICONMETRICS                      46
#define SPI_GETWORKAREA                         48
#define SPI_SETPENWINDOWS                       49
#define SPI_GETHIGHCONTRAST                     66
#define SPI_SETHIGHCONTRAST                     67
#define SPI_GETKEYBOARDPREF                     68
#define SPI_SETKEYBOARDPREF                     69
#define SPI_GETSCREENREADER                     70
#define SPI_SETSCREENREADER                     71
#define SPI_GETANIMATION                        72
#define SPI_SETANIMATION                        73
#define SPI_GETFONTSMOOTHING                    74
#define SPI_SETFONTSMOOTHING                    75
#define SPI_SETDRAGWIDTH                        76
#define SPI_SETDRAGHEIGHT                       77
#define SPI_SETHANDHELD                         78
#define SPI_GETLOWPOWERTIMEOUT                  79
#define SPI_GETPOWEROFFTIMEOUT                  80
#define SPI_SETLOWPOWERTIMEOUT                  81
#define SPI_SETPOWEROFFTIMEOUT                  82
#define SPI_GETLOWPOWERACTIVE                   83
#define SPI_GETPOWEROFFACTIVE                   84
#define SPI_SETLOWPOWERACTIVE                   85
#define SPI_SETPOWEROFFACTIVE                   86
#define SPI_SETCURSORS                          87
#define SPI_SETICONS                            88
#define SPI_GETDEFAULTINPUTLANG                 89
#define SPI_SETDEFAULTINPUTLANG                 90
#define SPI_SETLANGTOGGLE                       91
#define SPI_GETWINDOWSEXTENSION                 92
#define SPI_SETMOUSETRAILS                      93
#define SPI_GETMOUSETRAILS                      94
#define SPI_GETFILTERKEYS                       50
#define SPI_SETFILTERKEYS                       51
#define SPI_GETTOGGLEKEYS                       52
#define SPI_SETTOGGLEKEYS                       53
#define SPI_GETMOUSEKEYS                        54
#define SPI_SETMOUSEKEYS                        55
#define SPI_GETSHOWSOUNDS                       56
#define SPI_SETSHOWSOUNDS                       57
#define SPI_GETSTICKYKEYS                       58
#define SPI_SETSTICKYKEYS                       59
#define SPI_GETACCESSTIMEOUT                    60
#define SPI_SETACCESSTIMEOUT                    61
#define SPI_GETSERIALKEYS                       62
#define SPI_SETSERIALKEYS                       63
#define SPI_GETSOUNDSENTRY                      64
#define SPI_SETSOUNDSENTRY                      65

/* Flags
 */
#define SPIF_UPDATEINIFILE         0x0001
#define SPIF_SENDWININICHANGE      0x0002
#define SPIF_SENDCHANGE            SPIF_SENDWININICHANGE


#define DM_UPDATE           1
#define DM_COPY             2
#define DM_PROMPT           4
#define DM_MODIFY           8

#define DM_IN_BUFFER        DM_MODIFY
#define DM_IN_PROMPT        DM_PROMPT
#define DM_OUT_BUFFER       DM_COPY
#define DM_OUT_DEFAULT      DM_UPDATE

/* device capabilities indices
 */
#define DC_FIELDS               1
#define DC_PAPERS               2
#define DC_PAPERSIZE            3
#define DC_MINEXTENT            4
#define DC_MAXEXTENT            5
#define DC_BINS                 6
#define DC_DUPLEX               7
#define DC_SIZE                 8
#define DC_EXTRA                9
#define DC_VERSION              10
#define DC_DRIVER               11
#define DC_BINNAMES             12
#define DC_ENUMRESOLUTIONS      13
#define DC_FILEDEPENDENCIES     14
#define DC_TRUETYPE             15
#define DC_PAPERNAMES           16
#define DC_ORIENTATION          17
#define DC_COPIES               18
#define DC_BINADJUST            19
#define DC_EMF_COMPLIANT        20
#define DC_DATATYPE_PRODUCED    21

/* ExtFloodFill style flags
 */
#define  FLOODFILLBORDER   0
#define  FLOODFILLSURFACE  1

/* StretchBlt Modes
 */
#define STRETCH_ANDSCANS    1
#define STRETCH_ORSCANS     2
#define STRETCH_DELETESCANS 3
#define STRETCH_HALFTONE    4
#define MAXSTRETCHBLTMODE   4

#define BLACKONWHITE        STRETCH_ANDSCANS
#define WHITEONBLACK        STRETCH_ORSCANS
#define COLORONCOLOR        STRETCH_DELETESCANS
#define HALFTONE            STRETCH_HALFTONE

/* PolyDraw and GetPath point types
 */
#define PT_CLOSEFIGURE      0x01
#define PT_LINETO           0x02
#define PT_BEZIERTO         0x04
#define PT_MOVETO           0x06

/* color usage
 */
#define DIB_RGB_COLORS      0
#define DIB_PAL_COLORS      1


/* GDI object types
 */
#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13

/* ScrollWindowEx() scroll flags
 */
#define SW_SCROLLCHILDREN   0x0001
#define SW_INVALIDATE       0x0002
#define SW_ERASE            0x0004


/* Enhanced Metafile structures
 */
#define ENHMETA_SIGNATURE       0x464D4520
#define ENHMETA_STOCK_OBJECT    0x80000000

#define EMR_HEADER                      1
#define EMR_POLYBEZIER                  2
#define EMR_POLYGON                     3
#define EMR_POLYLINE                    4
#define EMR_POLYBEZIERTO                5
#define EMR_POLYLINETO                  6
#define EMR_POLYPOLYLINE                7
#define EMR_POLYPOLYGON                 8
#define EMR_SETWINDOWEXTEX              9
#define EMR_SETWINDOWORGEX              10
#define EMR_SETVIEWPORTEXTEX            11
#define EMR_SETVIEWPORTORGEX            12
#define EMR_SETBRUSHORGEX               13
#define EMR_EOF                         14
#define EMR_SETPIXELV                   15
#define EMR_SETMAPPERFLAGS              16
#define EMR_SETMAPMODE                  17
#define EMR_SETBKMODE                   18
#define EMR_SETPOLYFILLMODE             19
#define EMR_SETROP2                     20
#define EMR_SETSTRETCHBLTMODE           21
#define EMR_SETTEXTALIGN                22
#define EMR_SETCOLORADJUSTMENT          23
#define EMR_SETTEXTCOLOR                24
#define EMR_SETBKCOLOR                  25
#define EMR_OFFSETCLIPRGN               26
#define EMR_MOVETOEX                    27
#define EMR_SETMETARGN                  28
#define EMR_EXCLUDECLIPRECT             29
#define EMR_INTERSECTCLIPRECT           30
#define EMR_SCALEVIEWPORTEXTEX          31
#define EMR_SCALEWINDOWEXTEX            32
#define EMR_SAVEDC                      33
#define EMR_RESTOREDC                   34
#define EMR_SETWORLDTRANSFORM           35
#define EMR_MODIFYWORLDTRANSFORM        36
#define EMR_SELECTOBJECT                37
#define EMR_CREATEPEN                   38
#define EMR_CREATEBRUSHINDIRECT         39
#define EMR_DELETEOBJECT                40
#define EMR_ANGLEARC                    41
#define EMR_ELLIPSE                     42
#define EMR_RECTANGLE                   43
#define EMR_ROUNDRECT                   44
#define EMR_ARC                         45
#define EMR_CHORD                       46
#define EMR_PIE                         47
#define EMR_SELECTPALETTE               48
#define EMR_CREATEPALETTE               49
#define EMR_SETPALETTEENTRIES           50
#define EMR_RESIZEPALETTE               51
#define EMR_REALIZEPALETTE              52
#define EMR_EXTFLOODFILL                53
#define EMR_LINETO                      54
#define EMR_ARCTO                       55
#define EMR_POLYDRAW                    56
#define EMR_SETARCDIRECTION             57
#define EMR_SETMITERLIMIT               58
#define EMR_BEGINPATH                   59
#define EMR_ENDPATH                     60
#define EMR_CLOSEFIGURE                 61
#define EMR_FILLPATH                    62
#define EMR_STROKEANDFILLPATH           63
#define EMR_STROKEPATH                  64
#define EMR_FLATTENPATH                 65
#define EMR_WIDENPATH                   66
#define EMR_SELECTCLIPPATH              67
#define EMR_ABORTPATH                   68
#define EMR_GDICOMMENT                  70
#define EMR_FILLRGN                     71
#define EMR_FRAMERGN                    72
#define EMR_INVERTRGN                   73
#define EMR_PAINTRGN                    74
#define EMR_EXTSELECTCLIPRGN            75
#define EMR_BITBLT                      76
#define EMR_STRETCHBLT                  77
#define EMR_MASKBLT                     78
#define EMR_PLGBLT                      79
#define EMR_SETDIBITSTODEVICE           80
#define EMR_STRETCHDIBITS               81
#define EMR_EXTCREATEFONTINDIRECTW      82
#define EMR_EXTTEXTOUTA                 83
#define EMR_EXTTEXTOUTW                 84
#define EMR_POLYBEZIER16                85
#define EMR_POLYGON16                   86
#define EMR_POLYLINE16                  87
#define EMR_POLYBEZIERTO16              88
#define EMR_POLYLINETO16                89
#define EMR_POLYPOLYLINE16              90
#define EMR_POLYPOLYGON16               91
#define EMR_POLYDRAW16                  92
#define EMR_CREATEMONOBRUSH             93
#define EMR_CREATEDIBPATTERNBRUSHPT     94
#define EMR_EXTCREATEPEN                95
#define EMR_POLYTEXTOUTA                96
#define EMR_POLYTEXTOUTW                97
#define EMR_SETICMMODE                  98
#define EMR_CREATECOLORSPACE            99
#define EMR_SETCOLORSPACE              100
#define EMR_DELETECOLORSPACE           101

#define EMR_MIN                         1
#define EMR_MAX                        101


/* Flags for MultiByteToWideChar() and WideCharToMultiByte()
 */
#define MB_PRECOMPOSED       0x00000001
#define MB_COMPOSITE         0x00000002
#define MB_USEGLYPHCHARS     0x00000004
#define MB_ERR_INVALID_CHARS 0x00000008

#define WC_DEFAULTCHECK      0x00000100
#define WC_COMPOSITECHECK    0x00000200
#define WC_DISCARDNS         0x00000010
#define WC_SEPCHARS          0x00000020
#define WC_DEFAULTCHAR       0x00000040

/*  Character type flags
 */
#define CT_CTYPE1            0x00000001
#define CT_CTYPE2            0x00000002
#define CT_CTYPE3            0x00000004

/*  CTYPE1 flags
 */
#define C1_UPPER             0x0001
#define C1_LOWER             0x0002
#define C1_DIGIT             0x0004
#define C1_SPACE             0x0008
#define C1_PUNCT             0x0010
#define C1_CNTRL             0x0020
#define C1_BLANK             0x0040
#define C1_XDIGIT            0x0080
#define C1_ALPHA             0x0100

/*  CTYPE2 flags
 */
#define C2_LEFTTORIGHT       0x0001
#define C2_RIGHTTOLEFT       0x0002
#define C2_EUROPENUMBER      0x0003
#define C2_EUROPESEPARATOR   0x0004
#define C2_EUROPETERMINATOR  0x0005
#define C2_ARABICNUMBER      0x0006
#define C2_COMMONSEPARATOR   0x0007
#define C2_BLOCKSEPARATOR    0x0008
#define C2_SEGMENTSEPARATOR  0x0009
#define C2_WHITESPACE        0x000A
#define C2_OTHERNEUTRAL      0x000B
#define C2_NOTAPPLICABLE     0x0000

/*  CTYPE3 flags
 */
#define C3_NONSPACING        0x0001
#define C3_DIACRITIC         0x0002
#define C3_VOWELMARK         0x0004
#define C3_SYMBOL            0x0008
#define C3_KATAKANA          0x0010
#define C3_HIRAGANA          0x0020
#define C3_HALFWIDTH         0x0040
#define C3_FULLWIDTH         0x0080
#define C3_IDEOGRAPH         0x0100
#define C3_KASHIDA           0x0200
#define C3_LEXICAL           0x0400
#define C3_ALPHA             0x8000
#define C3_NOTAPPLICABLE     0x0000

/*  String Compare / Map flags
 */
#define NORM_IGNORECASE         0x00000001
#define NORM_IGNORENONSPACE     0x00000002
#define NORM_IGNORESYMBOLS      0x00000004
#define NORM_IGNOREKANATYPE     0x00010000
#define NORM_IGNOREWIDTH        0x00020000

/*  Flags for LCMapString()
 */
#define LCMAP_LOWERCASE         0x00000100
#define LCMAP_UPPERCASE         0x00000200
#define LCMAP_SORTKEY           0x00000400
#define LCMAP_BYTEREV           0x00000800
#define LCMAP_HIRAGANA          0x00100000
#define LCMAP_KATAKANA          0x00200000
#define LCMAP_HALFWIDTH         0x00400000
#define LCMAP_FULLWIDTH         0x00800000

/*  Flags for EnumSystemLocales() and IsValidLocale()
 */
#define LCID_INSTALLED          0x00000001
#define LCID_SUPPORTED          0x00000002

/*  Flags for EnumSystemCodePages()
 */
#define CP_INSTALLED            0x00000001
#define CP_SUPPORTED            0x00000002

/* Sort flags
 */
#define SORT_STRINGSORT         0x00001000

/*  Codepage types
 */
#define CP_ACP               0
#define CP_OEMCP             1
#define CP_MACCP             2


/*  Country codes
 */
#define CTRY_DEFAULT                     0
#define CTRY_UNITED_STATES               1
#define CTRY_CANADA                      2
#define CTRY_RUSSIA                      7
#define CTRY_GREECE                      30
#define CTRY_NETHERLANDS                 31
#define CTRY_BELGIUM                     32
#define CTRY_FRANCE                      33
#define CTRY_SPAIN                       34
#define CTRY_HUNGARY                     36
#define CTRY_ITALY                       39
#define CTRY_ROMANIA                     40
#define CTRY_SWITZERLAND                 41
#define CTRY_CZECH                       42
#define CTRY_SLOVAK                      42
#define CTRY_AUSTRIA                     43
#define CTRY_UNITED_KINGDOM              44
#define CTRY_DENMARK                     45
#define CTRY_SWEDEN                      46
#define CTRY_NORWAY                      47
#define CTRY_POLAND                      48
#define CTRY_GERMANY                     49
#define CTRY_MEXICO                      52
#define CTRY_BRAZIL                      55
#define CTRY_AUSTRALIA                   61
#define CTRY_NEW_ZEALAND                 64
#define CTRY_SINGAPORE                   65
#define CTRY_JAPAN                       81
#define CTRY_SOUTH_KOREA                 82
#define CTRY_PRCHINA                     86
#define CTRY_TURKEY                      90
#define CTRY_PORTUGAL                    351
#define CTRY_IRELAND                     353
#define CTRY_ICELAND                     354
#define CTRY_FINLAND                     358
#define CTRY_BULGARIA                    359
#define CTRY_CROATIA                     385
#define CTRY_SLOVENIA                    386
#define CTRY_HONG_KONG                   852
#define CTRY_TAIWAN                      886

/*  Locale types
 */
#define LOCALE_NOUSEROVERRIDE       0x80000000
#define LOCALE_USE_CP_ACP           0x40000000
#define LOCALE_ILANGUAGE            0x00000001
#define LOCALE_SLANGUAGE            0x00000002
#define LOCALE_SENGLANGUAGE         0x00001001
#define LOCALE_SABBREVLANGNAME      0x00000003
#define LOCALE_SNATIVELANGNAME      0x00000004
#define LOCALE_ICOUNTRY             0x00000005
#define LOCALE_SCOUNTRY             0x00000006
#define LOCALE_SENGCOUNTRY          0x00001002
#define LOCALE_SABBREVCTRYNAME      0x00000007
#define LOCALE_SNATIVECTRYNAME      0x00000008
#define LOCALE_IDEFAULTLANGUAGE     0x00000009
#define LOCALE_IDEFAULTCOUNTRY      0x0000000A
#define LOCALE_IDEFAULTCODEPAGE     0x0000000B
#define LOCALE_IDEFAULTANSICODEPAGE 0x00001004
#define LOCALE_SLIST                0x0000000C
#define LOCALE_IMEASURE             0x0000000D
#define LOCALE_SDECIMAL             0x0000000E
#define LOCALE_STHOUSAND            0x0000000F
#define LOCALE_SGROUPING            0x00000010
#define LOCALE_IDIGITS              0x00000011
#define LOCALE_ILZERO               0x00000012
#define LOCALE_INEGNUMBER           0x00001010
#define LOCALE_SNATIVEDIGITS        0x00000013
#define LOCALE_SCURRENCY            0x00000014
#define LOCALE_SINTLSYMBOL          0x00000015
#define LOCALE_SMONDECIMALSEP       0x00000016
#define LOCALE_SMONTHOUSANDSEP      0x00000017
#define LOCALE_SMONGROUPING         0x00000018
#define LOCALE_ICURRDIGITS          0x00000019
#define LOCALE_IINTLCURRDIGITS      0x0000001A
#define LOCALE_ICURRENCY            0x0000001B
#define LOCALE_INEGCURR             0x0000001C
#define LOCALE_SDATE                0x0000001D
#define LOCALE_STIME                0x0000001E
#define LOCALE_SSHORTDATE           0x0000001F
#define LOCALE_SLONGDATE            0x00000020
#define LOCALE_STIMEFORMAT          0x00001003
#define LOCALE_IDATE                0x00000021
#define LOCALE_ILDATE               0x00000022
#define LOCALE_ITIME                0x00000023
#define LOCALE_ITIMEMARKPOSN        0x00001005
#define LOCALE_ICENTURY             0x00000024
#define LOCALE_ITLZERO              0x00000025
#define LOCALE_IDAYLZERO            0x00000026
#define LOCALE_IMONLZERO            0x00000027
#define LOCALE_S1159                0x00000028
#define LOCALE_S2359                0x00000029
#define LOCALE_ICALENDARTYPE        0x00001009
#define LOCALE_IOPTIONALCALENDAR    0x0000100B
#define LOCALE_IFIRSTDAYOFWEEK      0x0000100C
#define LOCALE_IFIRSTWEEKOFYEAR     0x0000100D
#define LOCALE_SDAYNAME1            0x0000002A
#define LOCALE_SDAYNAME2            0x0000002B
#define LOCALE_SDAYNAME3            0x0000002C
#define LOCALE_SDAYNAME4            0x0000002D
#define LOCALE_SDAYNAME5            0x0000002E
#define LOCALE_SDAYNAME6            0x0000002F
#define LOCALE_SDAYNAME7            0x00000030
#define LOCALE_SABBREVDAYNAME1      0x00000031
#define LOCALE_SABBREVDAYNAME2      0x00000032
#define LOCALE_SABBREVDAYNAME3      0x00000033
#define LOCALE_SABBREVDAYNAME4      0x00000034
#define LOCALE_SABBREVDAYNAME5      0x00000035
#define LOCALE_SABBREVDAYNAME6      0x00000036
#define LOCALE_SABBREVDAYNAME7      0x00000037
#define LOCALE_SMONTHNAME1          0x00000038
#define LOCALE_SMONTHNAME2          0x00000039
#define LOCALE_SMONTHNAME3          0x0000003A
#define LOCALE_SMONTHNAME4          0x0000003B
#define LOCALE_SMONTHNAME5          0x0000003C
#define LOCALE_SMONTHNAME6          0x0000003D
#define LOCALE_SMONTHNAME7          0x0000003E
#define LOCALE_SMONTHNAME8          0x0000003F
#define LOCALE_SMONTHNAME9          0x00000040
#define LOCALE_SMONTHNAME10         0x00000041
#define LOCALE_SMONTHNAME11         0x00000042
#define LOCALE_SMONTHNAME12         0x00000043
#define LOCALE_SMONTHNAME13         0x0000100E
#define LOCALE_SABBREVMONTHNAME1    0x00000044
#define LOCALE_SABBREVMONTHNAME2    0x00000045
#define LOCALE_SABBREVMONTHNAME3    0x00000046
#define LOCALE_SABBREVMONTHNAME4    0x00000047
#define LOCALE_SABBREVMONTHNAME5    0x00000048
#define LOCALE_SABBREVMONTHNAME6    0x00000049
#define LOCALE_SABBREVMONTHNAME7    0x0000004A
#define LOCALE_SABBREVMONTHNAME8    0x0000004B
#define LOCALE_SABBREVMONTHNAME9    0x0000004C
#define LOCALE_SABBREVMONTHNAME10   0x0000004D
#define LOCALE_SABBREVMONTHNAME11   0x0000004E
#define LOCALE_SABBREVMONTHNAME12   0x0000004F
#define LOCALE_SABBREVMONTHNAME13   0x0000100F
#define LOCALE_SPOSITIVESIGN        0x00000050
#define LOCALE_SNEGATIVESIGN        0x00000051
#define LOCALE_IPOSSIGNPOSN         0x00000052
#define LOCALE_INEGSIGNPOSN         0x00000053
#define LOCALE_IPOSSYMPRECEDES      0x00000054
#define LOCALE_IPOSSEPBYSPACE       0x00000055
#define LOCALE_INEGSYMPRECEDES      0x00000056
#define LOCALE_INEGSEPBYSPACE       0x00000057
#define LOCALE_FONTSIGNATURE        0x00000058

/*  Flags for GetTimeFormat()
 */
#define TIME_NOMINUTESORSECONDS    0x00000001
#define TIME_NOSECONDS             0x00000002
#define TIME_NOTIMEMARKER          0x00000004
#define TIME_FORCE24HOURFORMAT     0x00000008

/*  Flags for GetDateFormat()
 */
#define DATE_SHORTDATE             0x00000001
#define DATE_LONGDATE              0x00000002
#define DATE_USE_ALT_CALENDAR      0x00000004

/*  Calendar types
 */
#define CAL_ICALINTVALUE           0x00000001
#define CAL_SCALNAME               0x00000002
#define CAL_IYEAROFFSETRANGE       0x00000003
#define CAL_SERASTRING             0x00000004
#define CAL_SSHORTDATE             0x00000005
#define CAL_SLONGDATE              0x00000006
#define CAL_SDAYNAME1              0x00000007
#define CAL_SDAYNAME2              0x00000008
#define CAL_SDAYNAME3              0x00000009
#define CAL_SDAYNAME4              0x0000000a
#define CAL_SDAYNAME5              0x0000000b
#define CAL_SDAYNAME6              0x0000000c
#define CAL_SDAYNAME7              0x0000000d
#define CAL_SABBREVDAYNAME1        0x0000000e
#define CAL_SABBREVDAYNAME2        0x0000000f
#define CAL_SABBREVDAYNAME3        0x00000010
#define CAL_SABBREVDAYNAME4        0x00000011
#define CAL_SABBREVDAYNAME5        0x00000012
#define CAL_SABBREVDAYNAME6        0x00000013
#define CAL_SABBREVDAYNAME7        0x00000014
#define CAL_SMONTHNAME1            0x00000015
#define CAL_SMONTHNAME2            0x00000016
#define CAL_SMONTHNAME3            0x00000017
#define CAL_SMONTHNAME4            0x00000018
#define CAL_SMONTHNAME5            0x00000019
#define CAL_SMONTHNAME6            0x0000001a
#define CAL_SMONTHNAME7            0x0000001b
#define CAL_SMONTHNAME8            0x0000001c
#define CAL_SMONTHNAME9            0x0000001d
#define CAL_SMONTHNAME10           0x0000001e
#define CAL_SMONTHNAME11           0x0000001f
#define CAL_SMONTHNAME12           0x00000020
#define CAL_SMONTHNAME13           0x00000021
#define CAL_SABBREVMONTHNAME1      0x00000022
#define CAL_SABBREVMONTHNAME2      0x00000023
#define CAL_SABBREVMONTHNAME3      0x00000024
#define CAL_SABBREVMONTHNAME4      0x00000025
#define CAL_SABBREVMONTHNAME5      0x00000026
#define CAL_SABBREVMONTHNAME6      0x00000027
#define CAL_SABBREVMONTHNAME7      0x00000028
#define CAL_SABBREVMONTHNAME8      0x00000029
#define CAL_SABBREVMONTHNAME9      0x0000002a
#define CAL_SABBREVMONTHNAME10     0x0000002b
#define CAL_SABBREVMONTHNAME11     0x0000002c
#define CAL_SABBREVMONTHNAME12     0x0000002d
#define CAL_SABBREVMONTHNAME13     0x0000002e

/*  Type for EnumCalendarInfo()
 */
#define ENUM_ALL_CALENDARS         0xffffffff

/*  Calendar types
 */
#define CAL_GREGORIAN        1
#define CAL_GREGORIAN_US     2
#define CAL_JAPAN            3
#define CAL_TAIWAN           4
#define CAL_KOREA            5

/*  Primary language IDs
 */
#define LANG_NEUTRAL                     0x00
#define LANG_ARABIC                      0x01
#define LANG_BULGARIAN                   0x02
#define LANG_CATALAN                     0x03
#define LANG_CHINESE                     0x04
#define LANG_CZECH                       0x05
#define LANG_DANISH                      0x06
#define LANG_GERMAN                      0x07
#define LANG_GREEK                       0x08
#define LANG_ENGLISH                     0x09
#define LANG_SPANISH                     0x0a
#define LANG_FINNISH                     0x0b
#define LANG_FRENCH                      0x0c
#define LANG_HEBREW                      0x0D
#define LANG_HUNGARIAN                   0x0e
#define LANG_ICELANDIC                   0x0f
#define LANG_ITALIAN                     0x10
#define LANG_JAPANESE                    0x11
#define LANG_KOREAN                      0x12
#define LANG_NORWEGIAN                   0x14
#define LANG_POLISH                      0x15
#define LANG_PORTUGUESE                  0x16
#define LANG_CROATIAN                    0x1a
#define LANG_DUTCH                       0x13
#define LANG_ROMANIAN                    0x18
#define LANG_RUSSIAN                     0x19
#define LANG_SLOVAK                      0x1b
#define LANG_ALBANIAN                    0x1c
#define LANG_SWEDISH                     0x1d
#define LANG_THAI                        0x1e
#define LANG_TURKISH                     0x1f
#define LANG_INDONESIAN                  0x21
#define LANG_UKRAINIAN                   0x22
#define LANG_BYELORUSSIAN                0x23
#define LANG_SLOVENIAN                   0x24
#define LANG_ESTONIAN                    0x25
#define LANG_LATVIAN                     0x26
#define LANG_LITHUANIAN                  0x27
#define LANG_FARSI                       0x29
#define LANG_BASQUE                      0x2d
#define LANG_SORBIAN                     0x2e
#define LANG_AFRIKAANS                   0x36
#define LANG_FAEROESE                    0x38

/*  SubLanguage IDs
 */
#define SUBLANG_NEUTRAL                  0x00
#define SUBLANG_DEFAULT                  0x01
#define SUBLANG_SYS_DEFAULT              0x02
#define SUBLANG_ARABIC                   0x01
#define SUBLANG_ARABIC_IRAQ              0x02
#define SUBLANG_ARABIC_EGYPT             0x03
#define SUBLANG_ARABIC_LIBYA             0x04
#define SUBLANG_ARABIC_ALGERIA           0x05
#define SUBLANG_ARABIC_MOROCCO           0x06
#define SUBLANG_ARABIC_TUNISIA           0x07
#define SUBLANG_ARABIC_OMAN              0x08
#define SUBLANG_ARABIC_YEMEN             0x09
#define SUBLANG_ARABIC_SYRIA             0x10
#define SUBLANG_ARABIC_JORDAN            0x11
#define SUBLANG_ARABIC_LEBANON           0x12
#define SUBLANG_ARABIC_KUWAIT            0x13
#define SUBLANG_ARABIC_UAE               0x14
#define SUBLANG_ARABIC_BAHRAIN           0x15
#define SUBLANG_ARABIC_QATAR             0x16
#define SUBLANG_CHINESE_TRADITIONAL      0x01
#define SUBLANG_CHINESE_SIMPLIFIED       0x02
#define SUBLANG_CHINESE_HONGKONG         0x03
#define SUBLANG_CHINESE_SINGAPORE        0x04
#define SUBLANG_DUTCH                    0x01
#define SUBLANG_DUTCH_BELGIAN            0x02
#define SUBLANG_ENGLISH_US               0x01
#define SUBLANG_ENGLISH_UK               0x02
#define SUBLANG_ENGLISH_AUS              0x03
#define SUBLANG_ENGLISH_CAN              0x04
#define SUBLANG_ENGLISH_NZ               0x05
#define SUBLANG_ENGLISH_EIRE             0x06
#define SUBLANG_ENGLISH_SAFRICA          0x07
#define SUBLANG_ENGLISH_JAMAICA          0x08
#define SUBLANG_ENGLISH_CARRIBEAN        0x09
#define SUBLANG_FRENCH                   0x01
#define SUBLANG_FRENCH_BELGIAN           0x02
#define SUBLANG_FRENCH_CANADIAN          0x03
#define SUBLANG_FRENCH_SWISS             0x04
#define SUBLANG_FRENCH_LUXEMBOURG        0x05
#define SUBLANG_GERMAN                   0x01
#define SUBLANG_GERMAN_SWISS             0x02
#define SUBLANG_GERMAN_AUSTRIAN          0x03
#define SUBLANG_GERMAN_LUXEMBOURG        0x04
#define SUBLANG_GERMAN_LIECHTENSTEIN     0x05
#define SUBLANG_ITALIAN                  0x01
#define SUBLANG_ITALIAN_SWISS            0x02
#define SUBLANG_KOREAN                   0x01
#define SUBLANG_KOREAN_JOHAB             0x02
#define SUBLANG_NORWEGIAN_BOKMAL         0x01
#define SUBLANG_NORWEGIAN_NYNORSK        0x02
#define SUBLANG_PORTUGUESE               0x02
#define SUBLANG_PORTUGUESE_BRAZILIAN     0x01
#define SUBLANG_SPANISH                  0x01
#define SUBLANG_SPANISH_MEXICAN          0x02
#define SUBLANG_SPANISH_MODERN           0x03
#define SUBLANG_SPANISH_GUATEMALA        0x04
#define SUBLANG_SPANISH_COSTARICA        0x05
#define SUBLANG_SPANISH_PANAMA           0x06
#define SUBLANG_SPANISH_DOMINICAN        0x07
#define SUBLANG_SPANISH_VENEZUELA        0x08
#define SUBLANG_SPANISH_COLOMBIA         0x09
#define SUBLANG_SPANISH_PERU             0x10
#define SUBLANG_SPANISH_ARGENTINA        0x11
#define SUBLANG_SPANISH_ECUADOR          0x12
#define SUBLANG_SPANISH_CHILE            0x13
#define SUBLANG_SPANISH_URUGUAY          0x14
#define SUBLANG_SPANISH_PARAGUAY         0x15
#define SUBLANG_SPANISH_BOLIVIA          0x16

/*  Sort IDs
 */
#define SORT_DEFAULT                     0x0
#define SORT_JAPANESE_XJIS               0x0
#define SORT_JAPANESE_UNICODE            0x1
#define SORT_CHINESE_BIG5                0x0
#define SORT_CHINESE_UNICODE             0x1
#define SORT_KOREAN_KSC                  0x0
#define SORT_KOREAN_UNICODE              0x1

/*  Default Language / Locale
 */
#define LANG_SYSTEM_DEFAULT    (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
#define LANG_USER_DEFAULT      (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
#define LOCALE_SYSTEM_DEFAULT  (MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT))
#define LOCALE_USER_DEFAULT    (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))

/*
 * Keyboard Layout API
 */
#define HKL_PREV            0
#define HKL_NEXT            1

#define KLF_ACTIVATE        0x00000001
#define KLF_SUBSTITUTE_OK   0x00000002
#define KLF_UNLOADPREVIOUS  0x00000004
#define KLF_REORDER         0x00000008
#define KLF_REPLACELANG     0x00000010
#define KLF_NOTELLSHELL     0x00000080


/* Forward Declaration
 */
typedef struct _tagLOGFONT      LOGFONT,    *PLOGFONT, *LPLOGFONT;
typedef struct _tagENUMLOGFONT      ENUMLOGFONT,    *PENUMLOGFONT, *LPENUMLOGFONT;

typedef struct _tagTEXTMETRIC   TEXTMETRIC, *PTEXTMETRIC, *LPTEXTMETRIC;
typedef struct _tagNEWTEXTMETRIC   NEWTEXTMETRIC, *PNEWTEXTMETRIC, *LPNEWTEXTMETRIC;


/* WinMain/WinProcs/TimerProcs
 */
typedef int                            (* EXPENTRY FNMAIN )( HINSTANCE, HINSTANCE, LPSTR, int );
typedef FNMAIN                          PFNMAIN;
typedef LRESULT                      (* EXPENTRY WNDPROC ) ( HWND, UINT, WPARAM, LPARAM );
typedef int                            (* EXPENTRY FARPROC ) ( );
typedef UINT                         (* EXPENTRY LPOFNHOOKPROC ) ( HWND, UINT, WPARAM, LPARAM );
typedef BOOL                         (* EXPENTRY DLGPROC ) ( HWND, UINT, WPARAM, LPARAM );
typedef void                           (* EXPENTRY TIMERPROC)( HWND, UINT, UINT, DWORD );
typedef BOOL                         (* EXPENTRY ABORTPROC)( HDC, int);
typedef UINT                         (* APIENTRY LPPRINTHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef UINT                         (* APIENTRY LPSETUPHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef LRESULT                      (* EXPENTRY HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
typedef DWORD                        (* EXPENTRY PTHREAD_START_ROUTINE)(PVOID lpThreadParameter);
typedef DWORD                        (* EXPENTRY LPTHREAD_START_ROUTINE)(PVOID lpThreadParameter);

typedef int                            (* EXPENTRY GOBJENUMPROC)(PVOID, LPARAM);
typedef VOID                         (* EXPENTRY LINEDDAPROC)(int, int, LPARAM);
typedef int                            (* EXPENTRY OLDFONTENUMPROC)( PLOGFONT, PTEXTMETRIC, DWORD, LPARAM );
typedef int                            (* EXPENTRY FONTENUMPROC)( PENUMLOGFONT, PNEWTEXTMETRIC, DWORD, LPARAM );
typedef BOOL                         (* EXPENTRY WNDENUMPROC)( HWND, LPARAM );
typedef BOOL                         (* EXPENTRY PROPENUMPROC)( HWND, LPCTSTR, HANDLE );
typedef UINT                         (* EXPENTRY PFRHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef BOOL                         (* EXPENTRY GRAYSTRINGPROC)(HDC, LPARAM, int);
typedef UINT                         (* EXPENTRY LPCCHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef UINT                         (* EXPENTRY LPCFHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef HDDEDATA                     (  EXPENTRY FNCALLBACK)(UINT, UINT, HCONV, HSZ, HSZ, HDDEDATA, DWORD, DWORD );
typedef BOOL                         (* EXPENTRY PROPENUMPROCEX)( HWND, LPCTSTR, HANDLE, DWORD );
typedef BOOL                         (* EXPENTRY LOCALE_ENUMPROC)( LPSTR );
typedef FNCALLBACK                    *PFNCALLBACK;

/* Structure
 */
typedef struct _tagPOINT  {
   LONG                              x;
   LONG                              y;
} POINT,  *PPOINT,  *LPPOINT, POINTL, *PPOINTL, *LPPOINTL;

typedef struct _tagMSG   {
   HWND                              hwnd;
   UINT                              message;
   WPARAM                            wParam;
   LPARAM                            lParam;
   DWORD                             time;
   POINT                             pt;
} MSG, *PMSG, *LPMSG;

typedef struct _tagRECT  {
   LONG                              left;
   LONG                              top;
   LONG                              right;
   LONG                              bottom;
} RECT, RECTL, *PRECT, *PRECTL, *LPRECT, *LPRECTL;

typedef struct _tagWNDCLASS {
   UINT                              style;
   WNDPROC                           lpfnWndProc;
   int                                 cbClsExtra;
   int                                 cbWndExtra;
   HINSTANCE                         hInstance;
   HICON                             hIcon;
   HCURSOR                           hCursor;
   HBRUSH                            hbrBackground;
   LPCTSTR                           lpszMenuName;
   LPCTSTR                           lpszClassName;
} WNDCLASS, *PWNDCLASS, *LPWNDCLASS;

#pragma pack(1)
typedef struct _tagOFN {
   DWORD                             lStructSize;
   HWND                              hwndOwner;
   HINSTANCE                         hInstance;
   LPCTSTR                           lpstrFilter;
   LPTSTR                            lpstrCustomFilter;
   DWORD                             nMaxCustFilter;
   DWORD                             nFilterIndex;
   LPTSTR                            lpstrFile;
   DWORD                             nMaxFile;
   LPTSTR                            lpstrFileTitle;
   DWORD                             nMaxFileTitle;
   LPCTSTR                           lpstrInitialDir;
   LPCTSTR                           lpstrTitle;
   DWORD                             Flags;
   WORD                              nFileOffset;
   WORD                              nFileExtension;
   LPCTSTR                           lpstrDefExt;
   DWORD                             lCustData;
   LPOFNHOOKPROC                     lpfnHook;
   LPCTSTR                           lpTemplateName;
} OPENFILENAME, *POPENFILENAME, *LPOPENFILENAME;
#pragma pack(4)

#define OFS_MAXPATHNAME    128
typedef struct _tagOFSTRUCT {
   BYTE                              cBytes;
   BYTE                              fFixedDisk;
   WORD                              nErrCode;
   WORD                              Reserved1;
   WORD                              Reserved2;
   CHAR                              szPathName[OFS_MAXPATHNAME];
} OFSTRUCT, *POFSTRUCT, *LPOFSTRUCT;

#define CCHDEVICENAME 32
typedef struct _tagDeviceMode {
   BYTE                              dmDeviceName[CCHDEVICENAME];
   WORD                              dmSpecVersion;
   WORD                              dmDriverVersion;
   WORD                              dmSize;
   WORD                              dmDriverExtra;
   DWORD                             dmFields;
   short                               dmOrientation;
   short                               dmPaperSize;
   short                               dmPaperLength;
   short                               dmPaperWidth;
   short                               dmScale;
   short                               dmCopies;
   short                               dmDefaultSource;
   short                               dmPrintQuality;
   short                               dmColor;
   short                               dmDuplex;
   short                               dmYResolution;
   short                               dmTTOption;
   short                               dmCollate;
   BYTE                              dmFormName[32];
   DWORD                             dmLogPixels;
   DWORD                             dmBitsPerPel;
   DWORD                             dmPelsWidth;
   DWORD                             dmPelsHeight;
   DWORD                             dmDisplayFlags;
   DWORD                             dmDisplayFrequency;
} DEVMODE, *PDEVMODE, *LPDEVMODE;

typedef UINT (* EXPENTRY LPFNDEVMODE)(HWND, HMODULE, LPDEVMODE, LPSTR, LPSTR, LPDEVMODE, LPSTR, UINT);


#define DM_SPECVERSION      0x0320

#define DM_ORIENTATION      0x00000001L
#define DM_PAPERSIZE        0x00000002L
#define DM_PAPERLENGTH      0x00000004L
#define DM_PAPERWIDTH       0x00000008L
#define DM_SCALE            0x00000010L
#define DM_COPIES           0x00000100L
#define DM_DEFAULTSOURCE    0x00000200L
#define DM_PRINTQUALITY     0x00000400L
#define DM_COLOR            0x00000800L
#define DM_DUPLEX           0x00001000L
#define DM_YRESOLUTION      0x00002000L
#define DM_TTOPTION         0x00004000L
#define DM_COLLATE          0x00008000L
#define DM_FORMNAME         0x00010000L

#define DMORIENT_PORTRAIT   1
#define DMORIENT_LANDSCAPE  2

#define DMPAPER_FIRST                  DMPAPER_LETTER
#define DMPAPER_LETTER                 1
#define DMPAPER_LETTERSMALL            2
#define DMPAPER_TABLOID                3
#define DMPAPER_LEDGER                 4
#define DMPAPER_LEGAL                  5
#define DMPAPER_STATEMENT              6
#define DMPAPER_EXECUTIVE              7
#define DMPAPER_A3                     8
#define DMPAPER_A4                     9
#define DMPAPER_A4SMALL               10
#define DMPAPER_A5                    11
#define DMPAPER_B4                    12
#define DMPAPER_B5                    13
#define DMPAPER_FOLIO                 14
#define DMPAPER_QUARTO                15
#define DMPAPER_10X14                 16
#define DMPAPER_11X17                 17
#define DMPAPER_NOTE                  18
#define DMPAPER_ENV_9                 19
#define DMPAPER_ENV_10                20
#define DMPAPER_ENV_11                21
#define DMPAPER_ENV_12                22
#define DMPAPER_ENV_14                23
#define DMPAPER_CSHEET                24
#define DMPAPER_DSHEET                25
#define DMPAPER_ESHEET                26
#define DMPAPER_ENV_DL                27
#define DMPAPER_ENV_C5                28
#define DMPAPER_ENV_C3                29
#define DMPAPER_ENV_C4                30
#define DMPAPER_ENV_C6                31
#define DMPAPER_ENV_C65               32
#define DMPAPER_ENV_B4                33
#define DMPAPER_ENV_B5                34
#define DMPAPER_ENV_B6                35
#define DMPAPER_ENV_ITALY             36
#define DMPAPER_ENV_MONARCH           37
#define DMPAPER_ENV_PERSONAL          38
#define DMPAPER_FANFOLD_US            39
#define DMPAPER_FANFOLD_STD_GERMAN    40
#define DMPAPER_FANFOLD_LGL_GERMAN    41
#define DMPAPER_LAST                  DMPAPER_FANFOLD_LGL_GERMAN
#define DMPAPER_USER                 256

#define DMBIN_FIRST         DMBIN_UPPER
#define DMBIN_UPPER         1
#define DMBIN_ONLYONE       1
#define DMBIN_LOWER         2
#define DMBIN_MIDDLE        3
#define DMBIN_MANUAL        4
#define DMBIN_ENVELOPE      5
#define DMBIN_ENVMANUAL     6
#define DMBIN_AUTO          7
#define DMBIN_TRACTOR       8
#define DMBIN_SMALLFMT      9
#define DMBIN_LARGEFMT      10
#define DMBIN_LARGECAPACITY 11
#define DMBIN_CASSETTE      14
#define DMBIN_FORMSOURCE    15
#define DMBIN_LAST          DMBIN_FORMSOURCE

#define DMBIN_USER          256

#define DMRES_DRAFT         (-1)
#define DMRES_LOW           (-2)
#define DMRES_MEDIUM        (-3)
#define DMRES_HIGH          (-4)

#define DMCOLOR_MONOCHROME  1
#define DMCOLOR_COLOR       2

#define DMDUP_SIMPLEX       1
#define DMDUP_VERTICAL      2
#define DMDUP_HORIZONTAL    3

#define DMTT_BITMAP         1
#define DMTT_DOWNLOAD       2
#define DMTT_SUBDEV         3

#define DMCOLLATE_FALSE     0
#define DMCOLLATE_TRUE      1

#define DM_GRAYSCALE  0x00000001
#define DM_INTERLACED 0x00000002



typedef struct _tagSIZE   {
   LONG                              cx;
   LONG                              cy;
}  SIZE,  *PSIZE,  *LPSIZE, SIZEL, *PSIZEL, *LPSIZEL;

typedef struct _tagPOINTS {
   SHORT   x;
   SHORT   y;
} POINTS, *PPOINTS, *LPPOINTS;

#define MAKEPOINTS(l)       (*((PPOINTS)&(l)))

typedef struct _tagCREATESTRUCT {
    LPVOID                           lpCreateParams;
    HANDLE                           hInstance;
    HMENU                            hMenu;
    HWND                             hwndParent;
    int                                cy;
    int                                cx;
    int                                y;
    int                                x;
    LONG                             style;
    LPCTSTR                          lpszName;
    LPCTSTR                          lpszClass;
    DWORD                            dwExStyle;
} CREATESTRUCT, *PCREATESTRUCT, *LPCREATESTRUCT;

typedef struct _tagCLIENTCREATESTRUCT {
    HANDLE                           hWindowMenu;
    UINT                             idFirstChild;
} CLIENTCREATESTRUCT, *PCLIENTCREATESTRUCT, *LPCLIENTCREATESTRUCT;

typedef struct _tagMDICREATESTRUCT {
    LPTSTR                         szClass;
    LPTSTR                         szTitle;
    HANDLE                         hOwner;
    int                              x;
    int                              y;
    int                              cx;
    int                              cy;
    DWORD                          style;
    LPARAM                         lParam;
} MDICREATESTRUCT, *PMDICREATESTRUCT, *LPMDICREATESTRUCT;

typedef struct _tagSECURITY_ATTRIBUTES {
    DWORD                            nLength;
    LPVOID                           lpSecurityDescriptor;
    BOOL                             bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef PVOID PSECURITY_DESCRIPTOR;

typedef struct _tagPAINTSTRUCT {
    HDC                              hdc;
    BOOL                             fErase;
    RECT                             rcPaint;
    BOOL                             fRestore;
    BOOL                             fIncUpdate;
    BYTE                             rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *LPPAINTSTRUCT;


/* Palette Entry Flags
 */
#define PC_RESERVED     0x01
#define PC_EXPLICIT     0x02
#define PC_NOCOLLAPSE   0x04


typedef struct _tagPALETTEENTRY
{
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;

typedef struct _tagLOGPALETTE
{
  WORD                                palVersion;
  WORD                                palNumEntries;
  PALETTEENTRY                        palPalEntry[1];
} LOGPALETTE, *PLOGPALETTE, *LPLOGPALETTE, *NPLOGPALETTE;

typedef struct  _tagXFORM
{
    FLOAT eM11;
    FLOAT eM12;
    FLOAT eM21;
    FLOAT eM22;
    FLOAT eDx;
    FLOAT eDy;
} XFORM, *PXFORM, *LPXFORM;

typedef struct _tagBITMAPCOREHEADER {
   DWORD   bcSize;
   WORD    bcWidth;
   WORD    bcHeight;
   WORD    bcPlanes;
   WORD    bcBitCount;
} BITMAPCOREHEADER, *LPBITMAPCOREHEADER, *PBITMAPCOREHEADER;


typedef struct _tagBITMAPINFOHEADER {
        DWORD    biSize;
        LONG     biWidth;
        LONG     biHeight;
        WORD     biPlanes;
        WORD     biBitCount;
        DWORD    biCompression;
        DWORD    biSizeImage;
        LONG     biXPelsPerMeter;
        LONG     biYPelsPerMeter;
        DWORD    biClrUsed;
        DWORD    biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER, *LPBITMAPINFOHEADER;

#pragma pack(2)
typedef struct _tagBITMAPFILEHEADER {
        WORD       bfType;
        DWORD      bfSize;
        WORD       bfReserved1;
        WORD       bfReserved2;
        DWORD      bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(4)

/* biCompression Field Constants
 */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

typedef struct _tagRGBTRIPLE {
        BYTE    rgbtBlue;
        BYTE    rgbtGreen;
        BYTE    rgbtRed;
} RGBTRIPLE;

typedef struct tagBITMAPCOREINFO {
    BITMAPCOREHEADER    bmciHeader;
    RGBTRIPLE           bmciColors[1];
} BITMAPCOREINFO, *LPBITMAPCOREINFO, *PBITMAPCOREINFO;

typedef struct _tagRGBQUAD {
        BYTE  rgbBlue;
        BYTE  rgbGreen;
        BYTE  rgbRed;
        BYTE  rgbReserved;
} RGBQUAD, *PRGBQUAD, *LPRGBQUAD;


typedef struct _tagBITMAPINFO {
    BITMAPINFOHEADER                bmiHeader;
    RGBQUAD                         bmiColors[1];
} BITMAPINFO, *PBITMAPINFO, *LPBITMAPINFO;


/* Xform FLAGS
 */
#define MWT_IDENTITY        1
#define MWT_LEFTMULTIPLY    2
#define MWT_RIGHTMULTIPLY   3

/* Mapping Modes */
#define MM_TEXT             1
#define MM_LOMETRIC         2
#define MM_HIMETRIC         3
#define MM_LOENGLISH        4
#define MM_HIENGLISH        5
#define MM_TWIPS            6
#define MM_ISOTROPIC        7
#define MM_ANISOTROPIC      8

typedef struct _tagDOCINFO {
    int      cbSize;
    LPCSTR lpszDocName;
    LPCSTR lpszOutput;
} DOCINFO, *PDOCINFO, *LPDOCINFO;

typedef struct _tagDevNames
{
    WORD wDriverOffset;
    WORD wDeviceOffset;
    WORD wOutputOffset;
    WORD wDefault;
} DEVNAMES, *PDEVNAMES, *LPDEVNAMES;

#pragma pack(1)
typedef struct _tagPDA {
   DWORD              lStructSize;
   HWND               hwndOwner;
   HGLOBAL            hDevMode;
   HGLOBAL            hDevNames;
   HDC                hDC;
   DWORD              Flags;
   WORD               nFromPage;
   WORD               nToPage;
   WORD               nMinPage;
   WORD               nMaxPage;
   WORD               nCopies;
   HINSTANCE          hInstance;
   LPARAM             lCustData;
   LPPRINTHOOKPROC    lpfnPrintHook;
   LPSETUPHOOKPROC    lpfnSetupHook;
   LPCTSTR            lpPrintTemplateName;
   LPCTSTR            lpSetupTemplateName;
   HGLOBAL            hPrintTemplate;
   HGLOBAL            hSetupTemplate;
}  PRINTDLG, *PPRINTDLG, *LPPRINTDLG;
#pragma pack(4)

/* PrintDlg options
 */
#define PD_ALLPAGES                  0x00000000
#define PD_SELECTION                 0x00000001
#define PD_PAGENUMS                  0x00000002
#define PD_NOSELECTION               0x00000004
#define PD_NOPAGENUMS                0x00000008
#define PD_COLLATE                   0x00000010
#define PD_PRINTTOFILE               0x00000020
#define PD_PRINTSETUP                0x00000040
#define PD_NOWARNING                 0x00000080
#define PD_RETURNDC                  0x00000100
#define PD_RETURNIC                  0x00000200
#define PD_RETURNDEFAULT             0x00000400
#define PD_SHOWHELP                  0x00000800
#define PD_ENABLEPRINTHOOK           0x00001000
#define PD_ENABLESETUPHOOK           0x00002000
#define PD_ENABLEPRINTTEMPLATE       0x00004000
#define PD_ENABLESETUPTEMPLATE       0x00008000
#define PD_ENABLEPRINTTEMPLATEHANDLE 0x00010000
#define PD_ENABLESETUPTEMPLATEHANDLE 0x00020000
#define PD_USEDEVMODECOPIES          0x00040000
#define PD_DISABLEPRINTTOFILE        0x00080000
#define PD_HIDEPRINTTOFILE           0x00100000
#define PD_NONETWORKBUTTON           0x00200000
#define PD_PAGESETUP                 0x00400000

/* Logical Brush (or Pattern)
 */
typedef struct _tagLOGBRUSH
{
    UINT      lbStyle;
    COLORREF  lbColor;
    LONG      lbHatch;
} LOGBRUSH, *PLOGBRUSH, *LPLOGBRUSH;

/* Logical Font
 */
#define LF_FACESIZE         32
#define LF_FULLFACESIZE     64

typedef struct _tagLOGFONT
{
    LONG    lfHeight;
    LONG    lfWidth;
    LONG    lfEscapement;
    LONG    lfOrientation;
    LONG    lfWeight;
    BYTE    lfItalic;
    BYTE    lfUnderline;
    BYTE    lfStrikeOut;
    BYTE    lfCharSet;
    BYTE    lfOutPrecision;
    BYTE    lfClipPrecision;
    BYTE    lfQuality;
    BYTE    lfPitchAndFamily;
    TCHAR   lfFaceName[LF_FACESIZE];
};

typedef struct _tagENUMLOGFONT
{
    LOGFONT   elfLogFont;
    BYTE      elfFullName[LF_FULLFACESIZE];
    BYTE      elfStyle[LF_FACESIZE];
};

/* Logical Pen
 */
typedef struct _tagLOGPEN {
   UINT      lopnStyle;
   POINT     lopnWidth;
   COLORREF  lopnColor;
} LOGPEN, *PLOGPEN, *LPLOGPEN;

typedef struct _tagEXTLOGPEN {
    UINT      elpPenStyle;
    UINT      elpWidth;
    UINT      elpBrushStyle;
    COLORREF  elpColor;
    LONG      elpHatch;
    DWORD     elpNumEntries;
    DWORD     elpStyleEntry[1];
} EXTLOGPEN, *PEXTLOGPEN, *LPEXTLOGPEN;


/* constants for CreateDIBitmap */
#define CBM_CREATEDIB   0x02L   /* create DIB bitmap */
#define CBM_INIT        0x04L   /* initialize bitmap */

/* ExtTexOut options   */
#define ETO_GRAYED                   1
#define ETO_OPAQUE                   2
#define ETO_CLIPPED                  4

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
#define BKMODE_LAST         2


/* Bitmap Header Definition */
typedef struct _tagBITMAP
{
    LONG      bmType;
    LONG      bmWidth;
    LONG      bmHeight;
    LONG      bmWidthBytes;
    WORD      bmPlanes;
    WORD      bmBitsPixel;
    LPVOID    bmBits;
} BITMAP, *PBITMAP, *LPBITMAP;


typedef struct _tagTEXTMETRIC
{
    LONG      tmHeight;
    LONG      tmAscent;
    LONG      tmDescent;
    LONG      tmInternalLeading;
    LONG      tmExternalLeading;
    LONG      tmAveCharWidth;
    LONG      tmMaxCharWidth;
    LONG      tmWeight;
    LONG      tmOverhang;
    LONG      tmDigitizedAspectX;
    LONG      tmDigitizedAspectY;
    BYTE      tmFirstChar;
    BYTE      tmLastChar;
    BYTE      tmDefaultChar;
    BYTE      tmBreakChar;
    BYTE      tmItalic;
    BYTE      tmUnderlined;
    BYTE      tmStruckOut;
    BYTE      tmPitchAndFamily;
    BYTE      tmCharSet;
};

/* tmPitchAndFamily values returned by GetTextMetrics in bits 0 -3.  Note
   the name FIXED_PITCH has the exact opposite meaning.  If set, the font
   is variable pitch.
 */
#define TMPF_FIXED_PITCH        0x01
#define TMPF_VECTOR             0x02
#define TMPF_TRUETYPE           0x04
#define TMPF_DEVICE             0x08

typedef struct _tagNEWTEXTMETRIC
{
    LONG      tmHeight;
    LONG      tmAscent;
    LONG      tmDescent;
    LONG      tmInternalLeading;
    LONG      tmExternalLeading;
    LONG      tmAveCharWidth;
    LONG      tmMaxCharWidth;
    LONG      tmWeight;
    LONG      tmOverhang;
    LONG      tmDigitizedAspectX;
    LONG      tmDigitizedAspectY;
    BYTE      tmFirstChar;
    BYTE      tmLastChar;
    BYTE      tmDefaultChar;
    BYTE      tmBreakChar;
    BYTE      tmItalic;
    BYTE      tmUnderlined;
    BYTE      tmStruckOut;
    BYTE      tmPitchAndFamily;
    BYTE      tmCharSet;
/* up to this point, this structure is identical to the TEXTMETRIC structure.  The following */
/* fields differentiate a NEWTEXTMETRIC from a TEXTMETRIC structure. */
    DWORD     ntmFlags;
    UINT      ntmSizeEM;
    UINT      ntmCellHeight;
    UINT      ntmAvgWidth;
};


typedef struct _GLYPHMETRICS {
    UINT      gmBlackBoxX;
    UINT      gmBlackBoxY;
    POINT   gmptGlyphOrigin;
    short       gmCellIncX;
    short       gmCellIncY;
} GLYPHMETRICS, *PGLYPHMETRICS, *LPGLYPHMETRICS;

/* GetGlyphOutline constants
 */
#define GGO_METRICS        0
#define GGO_BITMAP         1
#define GGO_NATIVE         2


typedef struct _FIXED {
    WORD  fract;
    short   value;
} FIXED;

typedef struct _MAT2 {
     FIXED  eM11;
     FIXED  eM12;
     FIXED  eM21;
     FIXED  eM22;
} MAT2, *PMAT2, *LPMAT2;

/* Clipboard Metafile Picture Structure
 */
typedef struct _tagHANDLETABLE
{
    HGDIOBJ     objectHandle[1];
} HANDLETABLE, *PHANDLETABLE, *LPHANDLETABLE;

typedef struct _tagMETARECORD
{
    DWORD     rdSize;
    WORD      rdFunction;
    WORD      rdParm[1];
} METARECORD, *PMETARECORD, *LPMETARECORD;

typedef int     (* EXPENTRY MFENUMPROC)(HDC, PHANDLETABLE, PMETARECORD, int, LPARAM);


/* Dialog Template & Item Structures
 */
#pragma pack(2)
typedef struct _tagDLGTEMPLATE {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD  cdit;
    WORD  x;
    WORD  y;
    WORD  cx;
    WORD  cy;
} DLGTEMPLATE, *PDLGTEMPLATE, *LPDLGTEMPLATE;

typedef struct _tagDLGITEMTEMPLATE {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD  x;
    WORD  y;
    WORD  cx;
    WORD  cy;
    WORD  id;
} DLGITEMTEMPLATE, *PDLGITEMTEMPLATE, *LPDLGITEMTEMPLATE;
#pragma pack(4)


/* Window Placement Structure
 */
typedef struct _tagWINDOWPLACEMENT {
    UINT    length;
    UINT    flags;
    UINT    showCmd;
    POINT   ptMinPosition;
    POINT   ptMaxPosition;
    RECT    rcNormalPosition;
} WINDOWPLACEMENT, *PWINDOWPLACEMENT, *LPWINDOWPLACEMENT;


/* Filetime Structure
 */
typedef struct _tagFILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

/* Systemtime Structure
 */
typedef struct _tagSYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;


/* OSVersion Platform defines
 */
#define VER_PLATFORM_WIN32_NT 2

/* OSVersion Information Structure
 */
typedef struct _tagOSVERSIONINFO
{
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  TCHAR szCSDVersion[ OFS_MAXPATHNAME ];
} OSVERSIONINFO, *POSVERSIONINFO, *LPOSVERSIONINFO;


/* Access Mask
 */
typedef DWORD       ACCESS_MASK;
typedef ACCESS_MASK REGSAM;


/* Printer Defaults
 */
typedef struct _tagPRINTER_DEFAULTS {
    LPTSTR         pDatatype;
    PDEVMODE       pDevMode;
    ACCESS_MASK    DesiredAccess;
} PRINTER_DEFAULTS, *PPRINTER_DEFAULTS, *LPPRINTER_DEFAULTS;

/* METAFILEPICT Structure
 */
typedef struct _tagMETAFILEPICT {
    LONG        mm;
    LONG        xExt;
    LONG        yExt;
    HMETAFILE   hMF;
} METAFILEPICT, *PMETAFILEPICT, *LPMETAFILEPICT;

#pragma pack(2)
typedef struct _tagMETAHEADER
{
    WORD        mtType;
    WORD        mtHeaderSize;
    WORD        mtVersion;
    DWORD       mtSize;
    WORD        mtNoObjects;
    DWORD       mtMaxRecord;
    WORD        mtNoParameters;
} METAHEADER, *PMETAHEADER, *LPMETAHEADER;
#pragma pack(4)


typedef struct _tagOVERLAPPED {
    DWORD    Internal;
    DWORD    InternalHigh;
    DWORD    Offset;
    DWORD    OffsetHigh;
    HANDLE   hEvent;
} OVERLAPPED, *POVERLAPPED, *LPOVERLAPPED;


typedef struct _tagMEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
    DWORD dwTotalPageFile;
    DWORD dwAvailPageFile;
    DWORD dwTotalVirtual;
    DWORD dwAvailVirtual;
} MEMORYSTATUS, *PMEMORYSTATUS, *LPMEMORYSTATUS;

typedef struct _tagMEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    DWORD RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

#define ANSI_CHARSET            0
#define DEFAULT_CHARSET         1
#define SYMBOL_CHARSET          2
#define SHIFTJIS_CHARSET        128
#define HANGEUL_CHARSET         129
#define GB2312_CHARSET          134
#define CHINESEBIG5_CHARSET     136
#define OEM_CHARSET             255
#define JOHAB_CHARSET           130
#define HEBREW_CHARSET          177
#define ARABIC_CHARSET          178
#define GREEK_CHARSET           161
#define TURKISH_CHARSET         162
#define THAI_CHARSET            222
#define EASTEUROPE_CHARSET      238
#define RUSSIAN_CHARSET         204

/* EnumFonts Masks
 */
#define RASTER_FONTTYPE     0x0001
#define DEVICE_FONTTYPE     0x002
#define TRUETYPE_FONTTYPE   0x004

#define OUT_TT_PRECIS        4
#define OUT_DEVICE_PRECIS    5
#define OUT_RASTER_PRECIS    6
#define OUT_TT_ONLY_PRECIS   7
#define OUT_OUTLINE_PRECIS   8

#define CLIP_MASK            0xf
#define CLIP_LH_ANGLES       0x0010
#define CLIP_TT_ALWAYS       0x0020
#define CLIP_EMBEDDED        0x0080


/* MEASUREITEMSTRUCT for ownerdraw
 */
typedef struct _tagMEASUREITEMSTRUCT {
    UINT     CtlType;
    UINT     CtlID;
    UINT     itemID;
    UINT     itemWidth;
    UINT     itemHeight;
    DWORD    itemData;
} MEASUREITEMSTRUCT, *PMEASUREITEMSTRUCT, *LPMEASUREITEMSTRUCT;

/* DRAWITEMSTRUCT for ownerdraw
 */
typedef struct _tagDRAWITEMSTRUCT {
    UINT      CtlType;
    UINT      CtlID;
    UINT      itemID;
    UINT      itemAction;
    UINT      itemState;
    HWND      hwndItem;
    HDC       hDC;
    RECT      rcItem;
    DWORD     itemData;
} DRAWITEMSTRUCT, *PDRAWITEMSTRUCT, *LPDRAWITEMSTRUCT;

/* DELETEITEMSTRUCT for ownerdraw
 */
typedef struct _tagDELETEITEMSTRUCT {
    UINT     CtlType;
    UINT     CtlID;
    UINT     itemID;
    HWND     hwndItem;
    UINT     itemData;
} DELETEITEMSTRUCT, *PDELETEITEMSTRUCT, *LPDELETEITEMSTRUCT;

/* COMPAREITEMSTUCT for ownerdraw sorting
 */
typedef struct _tagCOMPAREITEMSTRUCT {
    UINT      CtlType;
    UINT      CtlID;
    HWND      hwndItem;
    UINT      itemID1;
    DWORD     itemData1;
    UINT      itemID2;
    DWORD     itemData2;
} COMPAREITEMSTRUCT, *PCOMPAREITEMSTRUCT, *LPCOMPAREITEMSTRUCT;

/*
 *  CodePage Info.
 */
#define MAX_LEADBYTES        12
#define MAX_DEFAULTCHAR      2

typedef struct _tagCPINFO {
    UINT    MaxCharSize;
    BYTE    DefaultChar[MAX_DEFAULTCHAR];
    BYTE    LeadByte[MAX_LEADBYTES];
} CPINFO, *LPCPINFO;


/*
 *  Locale Structures
 */
typedef struct _tagCURRENCYFMT {
    UINT    NumDigits;
    UINT    LeadingZero;
    UINT    Grouping;
    LPSTR   lpDecimalSep;
    LPSTR   lpThousandSep;
    UINT    NegativeOrder;
    UINT    PositiveOrder;
    LPSTR   lpCurrencySymbol;
} CURRENCYFMT, *LPCURRENCYFMT;

typedef struct _tagNUMBERFMT {
    UINT    NumDigits;
    UINT    LeadingZero;
    UINT    Grouping;
    LPSTR   lpDecimalSep;
    LPSTR   lpThousandSep;
    UINT    NegativeOrder;
} NUMBERFMT, *LPNUMBERFMT;

/* Registry Definitions
 */
#define REGH_SYSINFO              0xFFFFFFFDL
#define REGH_WINOS2INI            0xFFFFFFFEL
#define REGH_INIMAPPING           0xFFFFFFFFL

#define HKEY_LOCAL_MACHINE      0xFFFFFFEFL
#define HKEY_CURRENT_USER       0xFFFFFFEEL
#define HKEY_USERS              0xFFFFFFEDL
#define HKEY_CLASSES_ROOT       0xFFFFFFECL

#define KEY_QUERY_VALUE         0x0001
#define KEY_SET_VALUE           0x0002
#define KEY_CREATE_SUB_KEY      0x0004
#define KEY_ENUMERATE_SUB_KEYS  0x0008
#define KEY_NOTIFY              0x0010
#define KEY_CREATE_LINK         0x0020
#define REG_OPTION_NON_VOLATILE 0x00000000L
#define REG_OPTION_VOLATILE     0x00000001L
#define REG_CREATED_NEW_KEY     0x00000001L
#define REG_OPENED_EXISTING_KEY 0x00000002L

#define KEY_READ         READ_CONTROL | KEY_QUERY_VALUE |\
                           KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY

#define KEY_WRITE        READ_CONTROL | KEY_SET_VALUE | KEY_CREATE_SUB_KEY

#define KEY_EXECUTE      KEY_READ

#define KEY_ALL_ACCESS   STANDARD_RIGHTS_ALL | KEY_QUERY_VALUE |\
                           KEY_SET_VALUE | KEY_CREATE_SUB_KEY |\
                           KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY |\
                           KEY_CREATE_LINK

#define REG_NONE                  0
#define REG_SZ                    1
#define REG_EXPAND_SZ             2
#define REG_BINARY                3
#define REG_DWORD                 4
#define REG_DWORD_LITTLE_ENDIAN   4
#define REG_DWORD_BIG_ENDIAN      5
#define REG_LINK                  6
#define REG_MULTI_SZ              7
#define REG_RESOURCE_LIST         8


#define EWX_LOGOFF                0
#define EWX_SHUTDOWN              1
#define EWX_REBOOT                2
#define EWX_FORCE                 4
#define EWX_POWEROFF              8


#pragma pack(1)
typedef struct _tagCHOOSECOLOR {
   DWORD           lStructSize;
   HWND            hwndOwner;
   HWND            hInstance;
   COLORREF        rgbResult;
   COLORREF       *lpCustColors;
   DWORD           Flags;
   LPARAM          lCustData;
   LPCCHOOKPROC    lpfnHook;
   LPCTSTR         lpTemplateName;
} CHOOSECOLOR, *PCHOOSECOLOR;

typedef struct _tagCHOOSEFONT {
   DWORD         lStructSize;
   HWND          hwndOwner;
   HDC           hDC;
   LPLOGFONT     lpLogFont;
   INT           iPointSize;
   DWORD         Flags;
   DWORD         rgbColors;
   DWORD         lCustData;
   LPCFHOOKPROC  lpfnHook;
   LPCTSTR       lpTemplateName;
   HINSTANCE     hInstance;
   LPTSTR        lpszStyle;
   WORD          nFontType;
   WORD          __MISSING_ALIGNMENT__;
   INT           nSizeMin;
   INT           nSizeMax;
} CHOOSEFONT, *LPCHOOSEFONT;

typedef struct _tagFINDREPLACE {
   DWORD          lStructSize;
   HWND           hwndOwner;
   HINSTANCE      hInstance;
   DWORD          Flags;
   LPTSTR         lpstrFindWhat;
   LPTSTR         lpstrReplaceWith;
   WORD           wFindWhatLen;
   WORD           wReplaceWithLen;
   LPARAM         lCustData;
   PFRHOOKPROC    lpfnHook;
   LPCTSTR        lpTemplateName;
} FINDREPLACE, *PFINDREPLACE, *LPFINDREPLACE;
#pragma pack(4)

typedef struct _ABC {
    int     abcA;
    UINT  abcB;
    int     abcC;
} ABC, *PABC;

typedef struct _tagENHMETAHEADER
{
    DWORD    iType;
    DWORD    nSize;
    RECTL    rclBounds;
    RECTL    rclFrame;
    DWORD    dSignature;
    DWORD    nVersion;
    DWORD    nBytes;
    DWORD    nRecords;
    WORD     nHandles;
    WORD     sReserved;
    DWORD    nDescription;
    DWORD    offDescription;
    DWORD    nPalEntries;
    SIZEL    szlDevice;
    SIZEL    szlMillimeters;
} ENHMETAHEADER, *PENHMETAHEADER, *LPENHMETAHEADER;


typedef struct _tagKERNINGPAIR {
    WORD   wFirst;
    WORD   wSecond;
    int      iKernAmount;
} KERNINGPAIR, *PKERNINGPAIR, *LPKERNINGPAIR;

typedef struct _tagPANOSE {
    BYTE   bFamilyType;
    BYTE   bSerifStyle;
    BYTE   bWeight;
    BYTE   bProportion;
    BYTE   bContrast;
    BYTE   bStrokeVariation;
    BYTE   bArmStyle;
    BYTE   bLetterform;
    BYTE   bMidline;
    BYTE   bXHeight;
} PANOSE, *PPANOSE, *LPPANOSE;

typedef struct _tagOUTLINETEXTMETRIC {
    UINT         otmSize;
    TEXTMETRIC   otmTextMetrics;
    BYTE         otmFiller;
    PANOSE       otmPanoseNumber;
    UINT         otmfsSelection;
    UINT         otmfsType;
    int            otmsCharSlopeRise;
    int            otmsCharSlopeRun;
    int            otmItalicAngle;
    UINT         otmEMSquare;
    int            otmAscent;
    int            otmDescent;
    UINT         otmLineGap;
    UINT         otmsCapEmHeight;
    UINT         otmsXHeight;
    RECT         otmrcFontBox;
    int            otmMacAscent;
    int            otmMacDescent;
    UINT         otmMacLineGap;
    UINT         otmusMinimumPPEM;
    POINT        otmptSubscriptSize;
    POINT        otmptSubscriptOffset;
    POINT        otmptSuperscriptSize;
    POINT        otmptSuperscriptOffset;
    UINT         otmsStrikeoutSize;
    int            otmsStrikeoutPosition;
    int            otmsUnderscoreSize;
    int            otmsUnderscorePosition;
    PSTR         otmpFamilyName;
    PSTR         otmpFaceName;
    PSTR         otmpStyleName;
    PSTR         otmpFullName;
} OUTLINETEXTMETRIC, *POUTLINETEXTMETRIC;

typedef struct _RASTERIZER_STATUS {
    short   nSize;
    short   wFlags;
    short   nLanguageID;
} RASTERIZER_STATUS, *PRASTERIZER_STATUS;

/* bits defined in wFlags of RASTERIZER_STATUS */
#define TT_AVAILABLE    0x0001
#define TT_ENABLED      0x0002

#define TT_POLYGON_TYPE   24
#define TT_PRIM_LINE       1
#define TT_PRIM_QSPLINE    2

typedef struct _tagPOINTFX
{
    FIXED x;
    FIXED y;
} POINTFX, *PPOINTFX, *LPPOINTFX;

typedef struct _tagTTPOLYCURVE
{
    WORD     wType;
    WORD     cpfx;
    POINTFX  apfx[1];
} TTPOLYCURVE, *PTTPOLYCURVE, *LPTTPOLYCURVE;

typedef struct _tagTTPOLYGONHEADER
{
    DWORD    cb;
    DWORD    dwType;
    POINTFX  pfxStart;
} TTPOLYGONHEADER, *PTTPOLYGONHEADER, *LPTTPOLYGONHEADER;


#define WPF_SETMINPOSITION      0x0001
#define WPF_RESTORETOMAXIMIZED  0x0002


typedef struct _tagWIN32_FIND_DATA {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    dwReserved0;
    DWORD    dwReserved1;
    TCHAR    cFileName[ MAX_PATH ];
    TCHAR    cAlternateFileName[ 14 ];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;


/* Menu item resource format
 */
typedef struct _tagMENUITEMTEMPLATEHEADER {
    WORD versionNumber;
    WORD offset;
} MENUITEMTEMPLATEHEADER, *PMENUITEMTEMPLATEHEADER;

typedef struct _tagMENUITEMTEMPLATE {
    WORD  mtOption;
    WORD  mtID;
    WCHAR mtString[1];
} MENUITEMTEMPLATE, *PMENUITEMTEMPLATE;

#define STARTF_USESHOWWINDOW    0x00000001
#define STARTF_USEPOSITION      0x00000002
#define STARTF_USESIZE          0x00000004

typedef struct _STARTUPINFO {
    DWORD  cb;
    LPTSTR lpReserved;
    LPTSTR lpDesktop;
    LPTSTR lpTitle;
    DWORD  dwX;
    DWORD  dwY;
    DWORD  dwXSize;
    DWORD  dwYSize;
    DWORD  dwXCountChars;
    DWORD  dwYCountChars;
    DWORD  dwFillAttribute;
    DWORD  dwFlags;
    WORD   wShowWindow;
    WORD   cbReserved2;
    LPBYTE lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFO, *PSTARTUPINFO;
typedef struct _STARTUPINFO *LPSTARTUPINFO;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD  dwProcessId;
    DWORD  dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;


typedef struct _tagCONVCONTEXT {
   UINT  cb;
   UINT  wFlags;
   UINT  wCountryID;
   int     iCodePage;
   DWORD dwLangID;
   DWORD dwSecurity;
   DWORD dwUnused[4];
} CONVCONTEXT, *PCONVCONTEXT, *LPCONVCONTEXT;

typedef struct _tagCONVINFO {
    DWORD       cb;
    DWORD       hUser;
    HCONV       hConvPartner;
    HSZ         hszSvcPartner;
    HSZ         hszServiceReq;
    HSZ         hszTopic;
    HSZ         hszItem;
    UINT        wFmt;
    UINT        wType;
    UINT        wStatus;
    UINT        wConvst;
    UINT        wLastError;
    HCONVLIST   hConvList;
    CONVCONTEXT ConvCtxt;
    HWND        hwnd;
    HWND        hwndPartner;
} CONVINFO, *PCONVINFO;


typedef struct _tagCRITICAL_SECTION {
    ULONG     ulReserved[8];
} CRITICAL_SECTION, *PCRITICAL_SECTION, *LPCRITICAL_SECTION;


typedef struct _BY_HANDLE_FILE_INFORMATION {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    dwVolumeSerialNumber;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    nNumberOfLinks;
    DWORD    nFileIndexHigh;
    DWORD    nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION;


typedef struct _tagACCEL {
    BYTE  fVirt;
    WORD  key;
    WORD  cmd;
} ACCEL, *PACCEL;


typedef struct _tagEMR
{
    DWORD   iType;
    DWORD   nSize;
} EMR, *PEMR;

typedef struct _tagEMRTEXT
{
    POINTL    ptlReference;
    DWORD     nChars;
    DWORD     offString;
    DWORD     fOptions;
    RECTL     rcl;
    DWORD     offDx;
} EMRTEXT, *PEMRTEXT;

typedef struct _tagABORTPATH
{
    EMR     emr;
} EMRABORTPATH      , *PEMRABORTPATH     ,
  EMRBEGINPATH      , *PEMRBEGINPATH     ,
  EMRENDPATH        , *PEMRENDPATH       ,
  EMRCLOSEFIGURE    , *PEMRCLOSEFIGURE   ,
  EMRFLATTENPATH    , *PEMRFLATTENPATH   ,
  EMRWIDENPATH      , *PEMRWIDENPATH     ,
  EMRSETMETARGN     , *PEMRSETMETARGN    ,
  EMRSAVEDC         , *PEMRSAVEDC        ,
  EMRREALIZEPALETTE , *PEMRREALIZEPALETTE;

typedef struct _tagEMRSELECTCLIPPATH
{
    EMR     emr;
    DWORD   iMode;
} EMRSELECTCLIPPATH    ,  *PEMRSELECTCLIPPATH    ,
  EMRSETBKMODE         ,  *PEMRSETBKMODE         ,
  EMRSETMAPMODE        ,  *PEMRSETMAPMODE        ,
  EMRSETPOLYFILLMODE   ,  *PEMRSETPOLYFILLMODE   ,
  EMRSETROP2           ,  *PEMRSETROP2           ,
  EMRSETSTRETCHBLTMODE ,  *PEMRSETSTRETCHBLTMODE ,
  EMRSETICMMODE        ,  *PEMRSETICMMODE        ,
  EMRSETTEXTALIGN      ,  *PEMRSETTEXTALIGN      ;

typedef struct _tagEMRSETMITERLIMIT
{
    EMR   emr;
    float   eMiterLimit;
} EMRSETMITERLIMIT, *PEMRSETMITERLIMIT;

typedef struct _tagEMRRESTOREDC
{
    EMR     emr;
    LONG    iRelative;
} EMRRESTOREDC, *PEMRRESTOREDC;

typedef struct _tagEMRSETARCDIRECTION
{
    EMR     emr;
    DWORD   iArcDirection;

} EMRSETARCDIRECTION, *PEMRSETARCDIRECTION;

typedef struct _tagEMRSETMAPPERFLAGS
{
    EMR       emr;
    DWORD     dwFlags;
} EMRSETMAPPERFLAGS, *PEMRSETMAPPERFLAGS;

typedef struct _tagEMRSETTEXTCOLOR
{
    EMR      emr;
    COLORREF crColor;
} EMRSETBKCOLOR,   *PEMRSETBKCOLOR,
  EMRSETTEXTCOLOR, *PEMRSETTEXTCOLOR;

typedef struct _tagEMRSELECTOBJECT
{
    EMR     emr;
    DWORD   ihObject;
} EMRSELECTOBJECT, *PEMRSELECTOBJECT,
  EMRDELETEOBJECT, *PEMRDELETEOBJECT;

typedef struct _tagEMRSELECTCOLORSPACE
{
    EMR     emr;
    DWORD   ihCS;
} EMRSELECTCOLORSPACE, *PEMRSELECTCOLORSPACE,
  EMRDELETECOLORSPACE, *PEMRDELETECOLORSPACE;

typedef struct _tagEMRSELECTPALETTE
{
    EMR     emr;
    DWORD   ihPal;
} EMRSELECTPALETTE, *PEMRSELECTPALETTE;

typedef struct _tagEMRRESIZEPALETTE
{
    EMR     emr;
    DWORD   ihPal;
    DWORD   cEntries;
} EMRRESIZEPALETTE, *PEMRRESIZEPALETTE;

typedef struct _tagEMRSETPALETTEENTRIES
{
    EMR          emr;
    DWORD        ihPal;
    DWORD        iStart;
    DWORD        cEntries;
    PALETTEENTRY aPalEntries[1];
} EMRSETPALETTEENTRIES, *PEMRSETPALETTEENTRIES;

typedef struct _tagCOLORADJUSTMENT
{
    WORD  caSize;
    WORD  caFlags;
    WORD  caIlluminantIndex;
    WORD  caRedGamma;
    WORD  caGreenGamma;
    WORD  caBlueGamma;
    WORD  caReferenceBlack;
    WORD  caReferenceWhite;
    SHORT caContrast;
    SHORT caBrightness;
    SHORT caColorfulness;
    SHORT caRedGreenTint;
} COLORADJUSTMENT, *PCOLORADJUSTMENT;

typedef struct _tagEMRSETCOLORADJUSTMENT
{
    EMR             emr;
    COLORADJUSTMENT ColorAdjustment;
} EMRSETCOLORADJUSTMENT, *PEMRSETCOLORADJUSTMENT;

typedef struct _tagEMRGDICOMMENT
{
    EMR   emr;
    DWORD cbData;
    BYTE  Data[1];
} EMRGDICOMMENT, *PEMRGDICOMMENT;

typedef struct _tagEMREOF
{
    EMR     emr;
    DWORD   nPalEntries;
    DWORD   offPalEntries;
    DWORD   nSizeLast;
} EMREOF, *PEMREOF;

typedef struct _tagEMRLINETO
{
    EMR    emr;
    POINT  ptl;
} EMRLINETO,   *PEMRLINETO, EMRMOVETOEX, *PEMRMOVETOEX;

typedef struct _tagEMROFFSETCLIPRGN
{
    EMR   emr;
    POINT  ptlOffset;
} EMROFFSETCLIPRGN, *PEMROFFSETCLIPRGN;

typedef struct _tagEMRFILLPATH
{
    EMR   emr;
    RECT   rclBounds;
} EMRFILLPATH         , *PEMRFILLPATH,
  EMRSTROKEANDFILLPATH, *PEMRSTROKEANDFILLPATH,
  EMRSTROKEPATH       , *PEMRSTROKEPATH;

typedef struct _tagEMREXCLUDECLIPRECT
{
    EMR   emr;
    RECT   rclClip;
} EMREXCLUDECLIPRECT,   *PEMREXCLUDECLIPRECT,
  EMRINTERSECTCLIPRECT, *PEMRINTERSECTCLIPRECT;

typedef struct _tagEMRSETVIEWPORTORGEX
{
    EMR     emr;
    POINT    ptlOrigin;
} EMRSETVIEWPORTORGEX , *PEMRSETVIEWPORTORGEX ,
  EMRSETWINDOWORGEX   , *PEMRSETWINDOWORGEX   ,
  EMRSETBRUSHORGEX    , *PEMRSETBRUSHORGEX    ;

typedef struct _tagEMRSETVIEWPORTEXTEX
{
    EMR   emr;
    SIZEL szlExtent;
} EMRSETVIEWPORTEXTEX, *PEMRSETVIEWPORTEXTEX,
  EMRSETWINDOWEXTEX,   *PEMRSETWINDOWEXTEX;

typedef struct _tagEMRSCALEVIEWPORTEXTEX
{
    EMR   emr;
    LONG  xNum;
    LONG  xDenom;
    LONG  yNum;
    LONG  yDenom;
} EMRSCALEVIEWPORTEXTEX, *PEMRSCALEVIEWPORTEXTEX,
  EMRSCALEWINDOWEXTEX,   *PEMRSCALEWINDOWEXTEX;

typedef struct _tagEMRSETWORLDTRANSFORM
{
    EMR     emr;
    XFORM   xform;
} EMRSETWORLDTRANSFORM, *PEMRSETWORLDTRANSFORM;

typedef struct _tagEMRMODIFYWORLDTRANSFORM
{
    EMR     emr;
    XFORM   xform;
    DWORD     iMode;
} EMRMODIFYWORLDTRANSFORM, *PEMRMODIFYWORLDTRANSFORM;

typedef struct _tagEMRSETPIXELV
{
    EMR       emr;
    POINT      ptlPixel;
    COLORREF  crColor;
} EMRSETPIXELV, *PEMRSETPIXELV;

typedef struct _tagEMREXTFLOODFILL
{
    EMR     emr;
    POINT  ptlStart;
    COLORREF crColor;
    DWORD   iMode;
} EMREXTFLOODFILL, *PEMREXTFLOODFILL;

typedef struct _tagEMRELLIPSE
{
    EMR     emr;
    RECT   rclBox;
} EMRELLIPSE,  *PEMRELLIPSE,
  EMRRECTANGLE, *PEMRRECTANGLE;

typedef struct _tagEMRROUNDRECT
{
    EMR   emr;
    RECT  rclBox;
    SIZEL szlCorner;
} EMRROUNDRECT, *PEMRROUNDRECT;

typedef struct _tagEMRARC
{
    EMR     emr;
    RECT   rclBox;
    POINT  ptlStart;
    POINT  ptlEnd;
} EMRARC,   *PEMRARC,
  EMRARCTO, *PEMRARCTO,
  EMRCHORD, *PEMRCHORD,
  EMRPIE,   *PEMRPIE;

typedef struct _tagEMRANGLEARC
{
    EMR     emr;
    POINT  ptlCenter;
    DWORD   nRadius;
    float   eStartAngle;
    float   eSweepAngle;
} EMRANGLEARC, *PEMRANGLEARC;

typedef struct _tagEMRPOLYLINE
{
    EMR     emr;
    RECT   rclBounds;
    DWORD   cptl;
    POINT  aptl[1];
} EMRPOLYLINE,     *PEMRPOLYLINE,
  EMRPOLYBEZIER,   *PEMRPOLYBEZIER,
  EMRPOLYGON,      *PEMRPOLYGON,
  EMRPOLYBEZIERTO, *PEMRPOLYBEZIERTO,
  EMRPOLYLINETO,   *PEMRPOLYLINETO;

typedef struct _tagEMRPOLYLINE16
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cpts;
    POINTS  apts[1];
} EMRPOLYLINE16,     *PEMRPOLYLINE16,
  EMRPOLYBEZIER16,   *PEMRPOLYBEZIER16,
  EMRPOLYGON16,      *PEMRPOLYGON16,
  EMRPOLYBEZIERTO16, *PEMRPOLYBEZIERTO16,
  EMRPOLYLINETO16,   *PEMRPOLYLINETO16;

typedef struct _tagEMRPOLYDRAW
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cptl;
    POINT   aptl[1];
    BYTE    abTypes[1];
} EMRPOLYDRAW, *PEMRPOLYDRAW;

typedef struct _tagEMRPOLYDRAW16
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cpts;
    POINTS  apts[1];
    BYTE    abTypes[1];
} EMRPOLYDRAW16, *PEMRPOLYDRAW16;

typedef struct _tagEMRPOLYPOLYLINE
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   nPolys;
    DWORD   cptl;
    DWORD   aPolyCounts[1];
    POINTL  aptl[1];
} EMRPOLYPOLYLINE, *PEMRPOLYPOLYLINE,
  EMRPOLYPOLYGON,  *PEMRPOLYPOLYGON;

typedef struct _tagEMRPOLYPOLYLINE16
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   nPolys;
    DWORD   cpts;
    DWORD   aPolyCounts[1];
    POINTS  apts[1];
} EMRPOLYPOLYLINE16, *PEMRPOLYPOLYLINE16,
  EMRPOLYPOLYGON16,  *PEMRPOLYPOLYGON16;

typedef struct _tagEMRINVERTRGN
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cbRgnData;
    BYTE    RgnData[1];
} EMRINVERTRGN, *PEMRINVERTRGN,
  EMRPAINTRGN,  *PEMRPAINTRGN;

typedef struct _tagEMRFILLRGN
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cbRgnData;
    DWORD   ihBrush;
    BYTE    RgnData[1];
} EMRFILLRGN, *PEMRFILLRGN;

typedef struct _tagEMRFRAMERGN
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   cbRgnData;
    DWORD   ihBrush;
    SIZEL   szlStroke;
    BYTE    RgnData[1];
} EMRFRAMERGN, *PEMRFRAMERGN;

typedef struct _tagEMREXTSELECTCLIPRGN
{
    EMR     emr;
    DWORD   cbRgnData;
    DWORD   iMode;
    BYTE    RgnData[1];
} EMREXTSELECTCLIPRGN, *PEMREXTSELECTCLIPRGN;

typedef struct _tagEMREXTTEXTOUTA
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   iGraphicsMode;
    float     exScale;
    float     eyScale;
    EMRTEXT emrtext;
} EMREXTTEXTOUTA, *PEMREXTTEXTOUTA,
  EMREXTTEXTOUTW, *PEMREXTTEXTOUTW;

typedef struct _tagEMRPOLYTEXTOUTA
{
    EMR     emr;
    RECT    rclBounds;
    DWORD   iGraphicsMode;
    float     exScale;
    float     eyScale;
    LONG    cStrings;
    EMRTEXT aemrtext[1];
} EMRPOLYTEXTOUTA, *PEMRPOLYTEXTOUTA,
  EMRPOLYTEXTOUTW, *PEMRPOLYTEXTOUTW;

typedef struct _tagEMRBITBLT
{
    EMR     emr;
    RECT    rclBounds;
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;
    COLORREF crBkColorSrc;
    DWORD   iUsageSrc;
    DWORD   offBmiSrc;
    DWORD   cbBmiSrc;
    DWORD   offBitsSrc;
    DWORD   cbBitsSrc;
} EMRBITBLT, *PEMRBITBLT;

typedef struct _tagEMRSTRETCHBLT
{
    EMR      emr;
    RECT     rclBounds;
    LONG     xDest;
    LONG     yDest;
    LONG     cxDest;
    LONG     cyDest;
    DWORD    dwRop;
    LONG     xSrc;
    LONG     ySrc;
    XFORM    xformSrc;
    COLORREF crBkColorSrc;
    DWORD    iUsageSrc;
    DWORD    offBmiSrc;
    DWORD    cbBmiSrc;
    DWORD    offBitsSrc;
    DWORD    cbBitsSrc;
    LONG     cxSrc;
    LONG     cySrc;
} EMRSTRETCHBLT, *PEMRSTRETCHBLT;

typedef struct _tagEMRMASKBLT
{
    EMR     emr;
    RECT    rclBounds;
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;
    COLORREF crBkColorSrc;
    DWORD   iUsageSrc;
    DWORD   offBmiSrc;
    DWORD   cbBmiSrc;
    DWORD   offBitsSrc;
    DWORD   cbBitsSrc;
    LONG    xMask;
    LONG    yMask;
    DWORD   iUsageMask;
    DWORD   offBmiMask;
    DWORD   cbBmiMask;
    DWORD   offBitsMask;
    DWORD   cbBitsMask;
} EMRMASKBLT, *PEMRMASKBLT;

typedef struct _tagEMRPLGBLT
{
    EMR     emr;
    RECT    rclBounds;
    POINTL  aptlDest[3];
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    XFORM   xformSrc;
    COLORREF crBkColorSrc;
    DWORD   iUsageSrc;
    DWORD   offBmiSrc;
    DWORD   cbBmiSrc;
    DWORD   offBitsSrc;
    DWORD   cbBitsSrc;
    LONG    xMask;
    LONG    yMask;
    DWORD   iUsageMask;
    DWORD   offBmiMask;
    DWORD   cbBmiMask;
    DWORD   offBitsMask;
    DWORD   cbBitsMask;
} EMRPLGBLT, *PEMRPLGBLT;

typedef struct _tagEMRSETDIBITSTODEVICE
{
    EMR     emr;
    RECT    rclBounds;
    LONG    xDest;
    LONG    yDest;
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    DWORD   offBmiSrc;
    DWORD   cbBmiSrc;
    DWORD   offBitsSrc;
    DWORD   cbBitsSrc;
    DWORD   iUsageSrc;
    DWORD   iStartScan;
    DWORD   cScans;
} EMRSETDIBITSTODEVICE, *PEMRSETDIBITSTODEVICE;

typedef struct _tagEMRSTRETCHDIBITS
{
    EMR     emr;
    RECT    rclBounds;
    LONG    xDest;
    LONG    yDest;
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    DWORD   offBmiSrc;
    DWORD   cbBmiSrc;
    DWORD   offBitsSrc;
    DWORD   cbBitsSrc;
    DWORD   iUsageSrc;
    DWORD   dwRop;
    LONG    cxDest;
    LONG    cyDest;
} EMRSTRETCHDIBITS, *PEMRSTRETCHDIBITS;

typedef struct tagLOGFONTW
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    WCHAR     lfFaceName[LF_FACESIZE];
} LOGFONTW, *PLOGFONTW;

#define ELF_VENDOR_SIZE   4

typedef struct _tagEXTLOGFONTW
{
    LOGFONTW  elfLogFont;
    WCHAR     elfFullName[LF_FULLFACESIZE];
    WCHAR     elfStyle[LF_FACESIZE];
    DWORD     elfVersion;
    DWORD     elfStyleSize;
    DWORD     elfMatch;
    DWORD     elfReserved;
    BYTE      elfVendorId[ELF_VENDOR_SIZE];
    DWORD     elfCulture;
    PANOSE    elfPanose;
} EXTLOGFONTW, *PEXTLOGFONTW;

typedef struct _tagEMREXTCREATEFONTINDIRECTW
{
    EMR         emr;
    DWORD       ihFont;
    EXTLOGFONTW elfw;
} EMREXTCREATEFONTINDIRECTW, *PEMREXTCREATEFONTINDIRECTW;

typedef struct _tagEMRCREATEPALETTE
{
    EMR        emr;
    DWORD      ihPal;
    LOGPALETTE lgpl;
} EMRCREATEPALETTE, *PEMRCREATEPALETTE;

typedef struct _tagEMRCREATEPEN
{
    EMR     emr;
    DWORD   ihPen;
    LOGPEN  lopn;
} EMRCREATEPEN, *PEMRCREATEPEN;

typedef struct _tagEMREXTCREATEPEN
{
    EMR       emr;
    DWORD     ihPen;
    DWORD     offBmi;
    DWORD     cbBmi;
    DWORD     offBits;
    DWORD     cbBits;
    EXTLOGPEN elp;
} EMREXTCREATEPEN, *PEMREXTCREATEPEN;

typedef struct _tagEMRCREATEBRUSHINDIRECT
{
    EMR      emr;
    DWORD    ihBrush;
    LOGBRUSH lb;
} EMRCREATEBRUSHINDIRECT, *PEMRCREATEBRUSHINDIRECT;

typedef struct _tagEMRCREATEMONOBRUSH
{
    EMR     emr;
    DWORD   ihBrush;
    DWORD   iUsage;
    DWORD   offBmi;
    DWORD   cbBmi;
    DWORD   offBits;
    DWORD   cbBits;
} EMRCREATEMONOBRUSH, *PEMRCREATEMONOBRUSH;

typedef struct _tagEMRCREATEDIBPATTERNBRUSHPT
{
    EMR     emr;
    DWORD   ihBrush;
    DWORD   iUsage;
    DWORD   offBmi;
    DWORD   cbBmi;
    DWORD   offBits;
    DWORD   cbBits;
} EMRCREATEDIBPATTERNBRUSHPT, *PEMRCREATEDIBPATTERNBRUSHPT;

typedef struct _tagEMRFORMAT
{
    DWORD   dSignature;
    DWORD   nVersion;
    DWORD   cbData;
    DWORD   offData;
} EMRFORMAT, *PEMRFORMAT;


#define GDICOMMENT_IDENTIFIER           0x43494447
#define GDICOMMENT_WINDOWS_METAFILE     0x80000001
#define GDICOMMENT_BEGINGROUP           0x00000002
#define GDICOMMENT_ENDGROUP             0x00000003
#define GDICOMMENT_MULTIFORMATS         0x40000004
#define EPS_SIGNATURE                   0x46535045


typedef struct _tagENHMETARECORD
{
    DWORD iType;
    DWORD nSize;
    DWORD dParm[1];
} ENHMETARECORD, *PENHMETARECORD, *LPENHMETARECORD;


typedef int (* EXPENTRY ENHMFENUMPROC)(HDC, PHANDLETABLE, const PENHMETARECORD, int, LPARAM);


#define RDH_RECTANGLES  1

typedef struct _tagRGNDATAHEADER {
    DWORD dwSize;
    DWORD iType;
    DWORD nCount;
    DWORD nRgnSize;
    RECT   rcBound;
} RGNDATAHEADER, *PRGNDATAHEADER, *LPRGNDATAHEADER;

typedef struct _tagRGNDATA {
    RGNDATAHEADER rdh;
    char            Buffer[1];
} RGNDATA, *PRGNDATA, *LPRGNDATA;


typedef struct _tagICONINFO {
    BOOL    fIcon;
    DWORD   xHotspot;
    DWORD   yHotspot;
    HBITMAP hbmMask;
    HBITMAP hbmColor;
} ICONINFO, *PICONINFO;


/* Arc direction */
#define AD_COUNTERCLOCKWISE 1
#define AD_CLOCKWISE        2


typedef UINT      MMRESULT;

typedef struct _tagMMTIME
{
   WORD          wType;

   union {
      DWORD     ms;
      DWORD     sample;
      DWORD     cb;
      DWORD     ticks;

      struct {
         BYTE  hour;
         BYTE  min;
         BYTE  sec;
         BYTE  frame;
         BYTE  fps;
         BYTE  dummy;
      } smpte;
      struct {
         DWORD songptrpos;
      } midi;
   } u;
} MMTIME, *PMMTIME;

/* Types for wType field in MMTIME struct
 */
#define TIME_MS         0x0001
#define TIME_SAMPLES    0x0002
#define TIME_BYTES      0x0004
#define TIME_SMPTE      0x0008
#define TIME_MIDI       0x0010
#define TIME_TICKS      0x0020


typedef struct _tagTIME_ZONE_INFORMATION {
    LONG       Bias;
    WCHAR      StandardName[ 32 ];
    SYSTEMTIME StandardDate;
    LONG       StandardBias;
    WCHAR      DaylightName[ 32 ];
    SYSTEMTIME DaylightDate;
    LONG       DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION;

#define TIME_ZONE_ID_UNKNOWN  0
#define TIME_ZONE_ID_STANDARD 1
#define TIME_ZONE_ID_DAYLIGHT 2

#define HINSTANCE_ERROR 32

/* LoadModule structure
 */
typedef struct _tagLOADPARMS32 {
    LPSTR lpEnvAddress;
    LPSTR lpCmdLine;
    LPSTR lpCmdShow;
    DWORD dwReserved;
} LOADPARMS32, *PLOADPARMS32;

/* DC Graphics Mode
 */
#define GM_COMPATIBLE       1
#define GM_ADVANCED         2

#define DCX_WINDOW                    0x00000001L
#define DCX_CACHE                     0x00000002L
#define DCX_NORESETATTRS              0x00000004L
#define DCX_CLIPCHILDREN              0x00000008L
#define DCX_CLIPSIBLINGS              0x00000010L
#define DCX_PARENTCLIP                0x00000020L
#define DCX_EXCLUDERGN                0x00000040L
#define DCX_INTERSECTRGN              0x00000080L
#define DCX_EXCLUDEUPDATE             0x00000100L
#define DCX_INTERSECTUPDATE           0x00000200L
#define DCX_LOCKWINDOWUPDATE          0x00000400L
#define DCX_VALIDATE                  0x00200000L

/* Defines for the fVirt field of the Accelerator table structure (ACCEL).
 */
#define FVIRTKEY  TRUE
#define FNOINVERT 0x02
#define FSHIFT    0x04
#define FCONTROL  0x08
#define FALT      0x10

typedef struct {
   unsigned bAppReturnCode:8;
   unsigned reserved      :6;
   unsigned fBusy         :1;
   unsigned fAck          :1;
} DDEACK;

typedef struct {
   unsigned reserved :14;
   unsigned fDeferUpd:1;
   unsigned fAckReq  :1;
   short    cfFormat;
} DDEADVISE;

typedef struct {
   unsigned unused   :12;
   unsigned fResponse:1;
   unsigned fRelease :1;
   unsigned reserved :1;
   unsigned fAckReq  :1;
   short    cfFormat;
   BYTE     Value[1];
} DDEDATA;

typedef struct {
   unsigned   unused   :13;
   unsigned   fRelease :1;
   unsigned   fReserved:2;
   short      cfFormat;
   BYTE     Value[1];
} DDEPOKE;

typedef struct {
   unsigned unused   :13;
   unsigned fRelease :1;
   unsigned fDeferUpd:1;
   unsigned fAckReq  :1;
   short    cfFormat;
} DDELN;

typedef struct {
   unsigned unused   :12;
   unsigned fAck     :1;
   unsigned fRelease :1;
   unsigned fReserved:1;
   unsigned fAckReq  :1;
   short    cfFormat;
   BYTE   rgb[1];
} DDEUP;

/* The following is a list of valid 'Transaction Type' flags:
 */
#define  XTYPF_NOBLOCK         0x0002  /* transaction cannot be blocked    */
#define  XTYPF_NODATA          0x0004  /* no data on advise notifications  */
#define  XTYPF_ACKREQ          0x0008  /* acknowledge receipt of message   */

/* The following is a list of valid 'Transaction Class' values and indicate
 * the type of value returned from a 'Callback' function:
 */
#define  XCLASS_BOOL           0x1000  /* TRUE or FALSE                    */
#define  XCLASS_DATA           0x2000  /* HDDEDATA,CBR_BLOCK or NULLHANDLE */
#define  XCLASS_FLAGS          0x4000  /* DDE_FACK,_FBUSY or _FNOTPROCESSED*/
#define  XCLASS_NOTIFICATION   0x8000  /* return value is ignored          */
#define  XCLASS_MASK           0xFC00

/* DDE flags indicating transaction results.
 */
#define DDE_FACK               0x8000
#define DDE_FBUSY              0x4000
#define DDE_FNOTPROCESSED      0x0000

/* The following is a list of valid 'Transaction Types':
 */
#define  XTYP_ERROR           (0x0000 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)
#define  XTYP_ADVDATA         (0x0010 | XCLASS_FLAGS)
#define  XTYP_ADVREQ          (0x0020 | XCLASS_DATA         | XTYPF_NOBLOCK)
#define  XTYP_ADVSTART        (0x0030 | XCLASS_BOOL)
#define  XTYP_ADVSTOP         (0x0040 | XCLASS_NOTIFICATION)
#define  XTYP_EXECUTE         (0x0050 | XCLASS_FLAGS)
#define  XTYP_CONNECT         (0x0060 | XCLASS_BOOL         | XTYPF_NOBLOCK)
#define  XTYP_CONNECT_CONFIRM (0x0070 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)
#define  XTYP_XACT_COMPLETE   (0x0080 | XCLASS_NOTIFICATION)
#define  XTYP_POKE            (0x0090 | XCLASS_FLAGS)
#define  XTYP_REGISTER        (0x00A0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)
#define  XTYP_REQUEST         (0x00B0 | XCLASS_DATA)
#define  XTYP_DISCONNECT      (0x00C0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)
#define  XTYP_UNREGISTER      (0x00D0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)
#define  XTYP_WILDCONNECT     (0x00E0 | XCLASS_DATA         | XTYPF_NOBLOCK)
#define  XTYP_MONITOR         (0x00F0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK)

/* Conversation values:
 */
#define  ST_CONNECTED          0x0001  /* a conversation is established    */
#define  ST_ADVISE             0x0002  /* One or more data links active    */
#define  ST_ISLOCAL            0x0004  /* both sides are using DDEML       */
#define  ST_BLOCKED            0x0008  /* conversation is blocked          */
#define  ST_CLIENT             0x0010  /* client side of conversation      */
#define  ST_TERMINATED         0x0020  /* conversation has terminated      */
#define  ST_INLIST             0x0040  /* conversation is part of a list   */
#define  ST_BLOCKNEXT          0x0080  /* block after next callback        */
#define  ST_ISSELF             0x0100  /* both sides are in same thread    */

/* Conversation values:
 */
#define  XST_NULL               0   /* initial state of a conversation     */
#define  XST_INCOMPLETE         1   /* last transaction failed             */
#define  XST_CONNECTED          2   /* no active transactions              */
#define  XST_INIT1              3   /* waiting initiate acknowledgement    */
#define  XST_INIT2              4   /* initiate acknowledgement received   */
#define  XST_REQSENT            5   /* waiting Request acknowledgement     */
#define  XST_DATARCVD           6   /* requested data has been received    */
#define  XST_POKESENT           7   /* waiting Poke acknowledgement        */
#define  XST_POKEACKRCVD        8   /* Poke acknowledgment received        */
#define  XST_EXECSENT           9   /* waiting Execute acknowledgement     */
#define  XST_EXECACKRCVD       10   /* Execute acknowledgement received    */
#define  XST_ADVSENT           11   /* waiting Advise acknowledgement      */
#define  XST_UNADVSENT         12   /* waiting Unadvise acknowledgement    */
#define  XST_ADVACKRCVD        13   /* Advise acknowledgement received     */
#define  XST_UNADVACKRCVD      14   /* Unadvise acknowledgement received   */
#define  XST_ADVDATASENT       15   /* waiting Advise Data acknowledgement */
#define  XST_ADVDATAACKRCVD    16   /* Advise Data acknowledgement received*/

#define SZDDE_ITEM_ITEMLIST     "TopicItemList"
#define SZDDESYS_TOPIC          "System"
#define SZDDESYS_ITEM_TOPICS    "Topics"
#define SZDDESYS_ITEM_SYSITEMS  "SysItems"
#define SZDDESYS_ITEM_RTNMSG    "ReturnMessage"
#define SZDDESYS_ITEM_STATUS    "Status"
#define SZDDESYS_ITEM_FORMATS   "Formats"
#define SZDDESYS_ITEM_HELP      "Help"

typedef struct _tagHSZPAIR
{
   HSZ         hszSvc;
   HSZ         hszTopic;
} HSZPAIR, *PHSZPAIR;

#define  HDATA_APPOWNED             0x0001

/* Monitor flags:
 */
#define  MF_HSZ_INFO                ((ULONG)0x01000000L)
#define  MF_SENDMSGS                ((ULONG)0x02000000L)
#define  MF_POSTMSGS                ((ULONG)0x04000000L)
#define  MF_CALLBACKS               ((ULONG)0x08000000L)
#define  MF_ERRORS                  ((ULONG)0x10000000L)
#define  MF_LINKS                   ((ULONG)0x20000000L)
#define  MF_CONV                    ((ULONG)0x40000000L)
#define  MF_MASK                    ((ULONG)0xFF000000L)

/* Callback filter flags:
 */
#define  CBF_FAIL_SELFCONNECTIONS   ((ULONG)0x00001000L)
#define  CBF_FAIL_CONNECTIONS       ((ULONG)0x00002000L)
#define  CBF_FAIL_ADVISES           ((ULONG)0x00004000L)
#define  CBF_FAIL_EXECUTES          ((ULONG)0x00008000L)
#define  CBF_FAIL_POKES             ((ULONG)0x00010000L)
#define  CBF_FAIL_REQUESTS          ((ULONG)0x00020000L)
#define  CBF_FAIL_ALLSVRXACTIONS    ((ULONG)0x0003f000L)

#define  CBF_SKIP_CONNECT_CONFIRMS  ((ULONG)0x00040000L)
#define  CBF_SKIP_REGISTRATIONS     ((ULONG)0x00080000L)
#define  CBF_SKIP_UNREGISTRATIONS   ((ULONG)0x00100000L)
#define  CBF_SKIP_DISCONNECTS       ((ULONG)0x00200000L)
#define  CBF_SKIP_ALLNOTIFICATIONS  ((ULONG)0x003c0000L)

#define  CBF_MASK                   ((ULONG)0x00FFF000L)

/* Application flags:
 */
#define  APPCMD_CLIENTONLY          ((ULONG)0x00000010L)
#define  APPCMD_FILTERINITS         ((ULONG)0x00000020L)
#define  APPCMD_MASK                ((ULONG)0x00000FF0L)

/* Application class flags:
 */
#define  APPCLASS_STANDARD          ((ULONG)0x00000000L)
#define  APPCLASS_MONITOR           ((ULONG)0x00000001L)
#define  APPCLASS_MASK              ((ULONG)0x0000000FL)

/* Miscellaneous DDEML definitions:
 */
#define  CBR_BLOCK                  ((ULONG)-1L)
#define  QID_SYNC                   ((ULONG)-1L)
#define  MSGF_DDEMGR                0x8001
#define  TIMEOUT_ASYNC              ((ULONG)-1L)
#define  CADV_LATEACK               0xFFFF

/*
 */
#define  DNS_REGISTER               ((ULONG)0x00000001L)
#define  DNS_UNREGISTER             ((ULONG)0x00000002L)
#define  DNS_FILTERON               ((ULONG)0x00000004L)
#define  DNS_FILTEROFF              ((ULONG)0x00000008L)

/*
 */
#define  EC_ENABLEALL               0
#define  EC_ENABLEONE               ST_BLOCKNEXT
#define  EC_DISABLE                 ST_BLOCKED
#define  EC_QUERYWAITING            2

typedef struct _tagMONCBSTRUCT
{
   UINT        cb;
   WORD        wReserved;
   DWORD       dwTime;
   HANDLE      hTask;
   DWORD       dwRet;
   UINT        wType;
   UINT        wFmt;
   HCONV       hConv;
   HSZ         hsz1;
   HSZ         hsz2;
   HDDEDATA    hData;
   DWORD       dwData1;
   DWORD       dwData2;
   CONVCONTEXT cc;
   DWORD       cbData;
   DWORD       Data[8];
} MONCBSTRUCT, *PMONCBSTRUCT;

typedef struct _tagMONCONVSTRUCT
{
   UINT        cb;
   BOOL        fConnect;
   DWORD       dwTime;
   HANDLE      hTask;
   HSZ         hszSvc;
   HSZ         hszTopic;
   HCONV       hConvClient;
   HCONV       hConvServer;
}  MONCONVSTRUCT, *PMONCONVSTRUCT;

typedef struct _tagMONERRSTRUCT
{
   UINT        cb;
   UINT        wLastError;
   DWORD       dwTime;
   HANDLE      hTask;
}  MONERRSTRUCT, *PMONERRSTRUCT;

typedef struct _tagMONHSZSTRUCT
{
   UINT        cb;
   BOOL        fsAction;
   DWORD       dwTime;
   HSZ         hsz;
   HANDLE      hTask;
   WORD        wReserved;
   char        str[1];
}  MONHSZSTRUCT, *PMONHSZSTRUCT;

#define  MH_CREATE      1
#define  MH_KEEP        2
#define  MH_DELETE      3
#define  MH_CLEANUP     4

typedef struct _tagMONLINKSTRUCT
{
   UINT        cb;
   DWORD       dwTime;
   HANDLE      hTask;
   BOOL        fEstablished;
   BOOL        fNoData;
   HSZ         hszSvc;
   HSZ         hszTopic;
   HSZ         hszItem;
   UINT        wFmt;
   BOOL        fServer;
   HCONV       hConvServer;
   HCONV       hConvClient;
}  MONLINKSTRUCT, *PMONLINKSTRUCT;

typedef struct _tagDDEML_MSG_HOOK_DATA
{
   UINT        uiLo;
   UINT        uiHigh;
   DWORD       cbData;
   DWORD       Data[8];
}  DDEML_MSG_HOOK_DATA, *PDDEML_MSG_HOOK_DATA;

typedef struct _tagMONMSGSTRUCT
{
   UINT        cb;
   HWND        hwndTo;
   DWORD       dwTime;
   HANDLE      hTask;
   UINT        wMsg;
   WPARAM      wParam;
   LPARAM      lParam;
   DDEML_MSG_HOOK_DATA dmhd;
}  MONMSGSTRUCT, *PMONMSGSTRUCT;

/* Dynamic Data Exchange Managment Library error values:
 */
#define  DMLERR_NO_ERROR            0

#define  DMLERR_FIRST               0x4000
#define  DMLERR_ADVACKTIMEOUT       0x4000
#define  DMLERR_BUSY                0x4001
#define  DMLERR_DATAACKTIMEOUT      0x4002
#define  DMLERR_DLL_NOT_INITIALIZED 0x4003
#define  DMLERR_DLL_USAGE           0x4004
#define  DMLERR_EXECACKTIMEOUT      0x4005
#define  DMLERR_INVALIDPARAMETER    0x4006
#define  DMLERR_LOW_MEMORY          0x4007
#define  DMLERR_MEMORY_ERROR        0x4008
#define  DMLERR_NOTPROCESSED        0x4009
#define  DMLERR_NO_CONV_ESTABLISHED 0x400a
#define  DMLERR_POKEACKTIMEOUT      0x400b
#define  DMLERR_POSTMSG_FAILED      0x400c
#define  DMLERR_REENTRANCY          0x400d
#define  DMLERR_SERVER_DIED         0x400e
#define  DMLERR_SYS_ERROR           0x400f
#define  DMLERR_UNADVACKTIMEOUT     0x4010
#define  DMLERR_UNFOUND_QUEUE_ID    0x4011
#define  DMLERR_LAST                0x4011

/* Dynamic Data Exchange Managment Library function prototypes:
 */

/* Default codepage
 */
#define CP_WINANSI      1004


#define SC_SIZE         0xF000
#define SC_MOVE         0xF010
#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_PREVWINDOW   0xF050
#define SC_CLOSE        0xF060
#define SC_VSCROLL      0xF070
#define SC_HSCROLL      0xF080
#define SC_MOUSEMENU    0xF090
#define SC_KEYMENU      0xF100
#define SC_ARRANGE      0xF110
#define SC_RESTORE      0xF120
#define SC_TASKLIST     0xF130
#define SC_SCREENSAVE   0xF140
#define SC_HOTKEY       0xF150
#define SC_ICON         SC_MINIMIZE
#define SC_ZOOM         SC_MAXIMIZE


#define META_SETBKCOLOR              0x0201
#define META_SETBKMODE               0x0102
#define META_SETMAPMODE              0x0103
#define META_SETROP2                 0x0104
#define META_SETRELABS               0x0105
#define META_SETPOLYFILLMODE         0x0106
#define META_SETSTRETCHBLTMODE       0x0107
#define META_SETTEXTCHAREXTRA        0x0108
#define META_SETTEXTCOLOR            0x0209
#define META_SETTEXTJUSTIFICATION    0x020A
#define META_SETWINDOWORG            0x020B
#define META_SETWINDOWEXT            0x020C
#define META_SETVIEWPORTORG          0x020D
#define META_SETVIEWPORTEXT          0x020E
#define META_OFFSETWINDOWORG         0x020F
#define META_SCALEWINDOWEXT          0x0410
#define META_OFFSETVIEWPORTORG       0x0211
#define META_SCALEVIEWPORTEXT        0x0412
#define META_LINETO                  0x0213
#define META_MOVETO                  0x0214
#define META_EXCLUDECLIPRECT         0x0415
#define META_INTERSECTCLIPRECT       0x0416
#define META_ARC                     0x0817
#define META_ELLIPSE                 0x0418
#define META_FLOODFILL               0x0419
#define META_PIE                     0x081A
#define META_RECTANGLE               0x041B
#define META_ROUNDRECT               0x061C
#define META_PATBLT                  0x061D
#define META_SAVEDC                  0x001E
#define META_SETPIXEL                0x041F
#define META_OFFSETCLIPRGN           0x0220
#define META_TEXTOUT                 0x0521
#define META_BITBLT                  0x0922
#define META_STRETCHBLT              0x0B23
#define META_POLYGON                 0x0324
#define META_POLYLINE                0x0325
#define META_ESCAPE                  0x0626
#define META_RESTOREDC               0x0127
#define META_FILLREGION              0x0228
#define META_FRAMEREGION             0x0429
#define META_INVERTREGION            0x012A
#define META_PAINTREGION             0x012B
#define META_SELECTCLIPREGION        0x012C
#define META_SELECTOBJECT            0x012D
#define META_SETTEXTALIGN            0x012E
#define META_CHORD                   0x0830
#define META_SETMAPPERFLAGS          0x0231
#define META_EXTTEXTOUT              0x0a32
#define META_SETDIBTODEV             0x0d33
#define META_SELECTPALETTE           0x0234
#define META_REALIZEPALETTE          0x0035
#define META_ANIMATEPALETTE          0x0436
#define META_SETPALENTRIES           0x0037
#define META_POLYPOLYGON             0x0538
#define META_RESIZEPALETTE           0x0139
#define META_DIBBITBLT               0x0940
#define META_DIBSTRETCHBLT           0x0b41
#define META_DIBCREATEPATTERNBRUSH   0x0142
#define META_STRETCHDIB              0x0f43
#define META_EXTFLOODFILL            0x0548
#define META_DELETEOBJECT            0x01f0
#define META_CREATEPALETTE           0x00f7
#define META_CREATEPATTERNBRUSH      0x01F9
#define META_CREATEPENINDIRECT       0x02FA
#define META_CREATEFONTINDIRECT      0x02FB
#define META_CREATEBRUSHINDIRECT     0x02FC
#define META_CREATEREGION            0x06FF


typedef struct _tagMINMAXINFO {
   POINT ptReserved;
   POINT ptMaxSize;
   POINT ptMaxPosition;
   POINT ptMinTrackSize;
   POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;

#define MSGF_DIALOGBOX      0
#define MSGF_MESSAGEBOX     1
#define MSGF_MENU           2
#define MSGF_MOVE           3
#define MSGF_SIZE           4
#define MSGF_SCROLLBAR      5
#define MSGF_NEXTWINDOW     6
#define MSGF_MAINLOOP       8
#define MSGF_MAX            8
#define MSGF_USER        4096

#define HELP_CONTEXT            0x0001L
#define HELP_QUIT               0x0002L
#define HELP_INDEX              0x0003L
#define HELP_CONTENTS           0x0003L
#define HELP_HELPONHELP         0x0004L
#define HELP_SETINDEX           0x0005L
#define HELP_SETCONTENTS        0x0005L
#define HELP_CONTEXTPOPUP       0x0008L
#define HELP_FORCEFILE          0x0009L
#define HELP_KEY                0x0101L
#define HELP_COMMAND            0x0102L
#define HELP_PARTIALKEY         0x0105L
#define HELP_MULTIKEY           0x0201L
#define HELP_SETWINPOS          0x0203L

#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010

#define ST_BEGINSWP         0
#define ST_ENDSWP           1

#define HTERROR             (-2)
#define HTTRANSPARENT       (-1)
#define HTNOWHERE           0
#define HTCLIENT            1
#define HTCAPTION           2
#define HTSYSMENU           3
#define HTGROWBOX           4
#define HTSIZE              HTGROWBOX
#define HTMENU              5
#define HTHSCROLL           6
#define HTVSCROLL           7
#define HTMINBUTTON         8
#define HTMAXBUTTON         9
#define HTLEFT              10
#define HTRIGHT             11
#define HTTOP               12
#define HTTOPLEFT           13
#define HTTOPRIGHT          14
#define HTBOTTOM            15
#define HTBOTTOMLEFT        16
#define HTBOTTOMRIGHT       17
#define HTBORDER            18
#define HTREDUCE            HTMINBUTTON
#define HTZOOM              HTMAXBUTTON
#define HTSIZEFIRST         HTLEFT
#define HTSIZELAST          HTBOTTOMRIGHT

#define SMTO_NORMAL         0x0000
#define SMTO_BLOCK          0x0001
#define SMTO_ABORTIFHUNG    0x0002

#define HELPINFO_WINDOW    0x0001
#define HELPINFO_MENUITEM  0x0002
typedef struct tagHELPINFO
{
    UINT    cbSize;
    int     iContextType;
    int     iCtrlId;
    HANDLE  hItemHandle;
    DWORD   dwContextId;
    POINT   MousePos;
}  HELPINFO, *LPHELPINFO;


/* Hook Related structures ...
 */
typedef struct _CWPSTRUCT {

    LPARAM lParam;
    WPARAM wParam;
    DWORD  message;
    HWND   hwnd;
} CWPSTRUCT;
typedef CWPSTRUCT          *PCWPSTRUCT;

typedef struct _DEBUGHOOKINFO {
    DWORD  idThread;
    LPARAM reserved;
    LPARAM lParam;
    WPARAM wParam;
    int      code;
} DEBUGHOOKINFO;
typedef DEBUGHOOKINFO      *PDEBUGHOOKINFO;

typedef struct _EVENTMSG {
    UINT  message;
    UINT  paramL;
    UINT  paramH;
    DWORD time;
    HWND  hwnd;
} EVENTMSG;
typedef EVENTMSG           *PEVENTMSG;

typedef struct _MOUSEHOOKSTRUCT {
    POINT pt;
    HWND  hwnd;
    UINT  wHitTestCode;
    DWORD dwExtraInfo;
} MOUSEHOOKSTRUCT;
typedef MOUSEHOOKSTRUCT    *PMOUSEHOOKSTRUCT;

#define MakeProcInstance(proc, hinst) (proc)
#define FreeProcInstance(proc) (proc)
#define FreeModule(hLibModule) FreeLibrary((hLibModule))
#define Yield()

#define GetWindowTask(hWnd) ((HANDLE)GetWindowThreadProcessId(hWnd, NULL))


/* Macro'd APIs
 */
#define CreateWindow(lpClassName, lpWindowName, dwStyle, x, y,\
                        nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\
        CreateWindowEx(0, lpClassName, lpWindowName, dwStyle, x, y,\
                          nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)

/* TBD: This must be removed before release
 */
#ifndef NO_HEAPSIZE_MACRO
#define HeapSize(handle, flags, pMem) AweHeapSize(handle, flags, pMem)
#endif

#define AnsiToOem     CharToOem
#define OemToAnsi     OemToChar
#define AnsiToOemBuff CharToOemBuff
#define OemToAnsiBuff OemToCharBuff
#define AnsiUpper     CharUpper
#define AnsiUpperBuff CharUpperBuff
#define AnsiLower     CharLower
#define AnsiLowerBuff CharLowerBuff
#define AnsiNext      CharNext
#define AnsiPrev      CharPrev
#define GetStringTypeA GetStringTypeEx


/* Name Mapped API's
 */
#define CreateBitmapIndirect            _CreateBitmapIndirect
#define CreateDialogIndirect            _CreateDialogIndirect
#define CreateDialogIndirectParam       _CreateDialogIndirectParam
#define CreateIconIndirect              _CreateIconIndirect
#define DialogBoxIndirect               _DialogBoxIndirect
#define DialogBoxIndirectParam          _DialogBoxIndirectParam
#define LoadMenuIndirect                _LoadMenuIndirect
#define LoadResource                    _LoadResource


/* API Prototypes
 */
int       WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int );

int       WINAPI AbortDoc( HDC );

BOOL    WINAPI AbortPath( HDC );

ATOM    WINAPI AddAtom( LPCSTR );

int       WINAPI AddFontResource( LPCSTR );

BOOL    WINAPI AdjustWindowRect( PRECT, DWORD, BOOL );

BOOL    WINAPI AdjustWindowRectEx( PRECT, DWORD, BOOL, DWORD );

BOOL    WINAPI AngleArc( HDC, int, int, DWORD, float startAngle, float sweepAngle );

BOOL    WINAPI AnimatePalette( HPALETTE, UINT, UINT, const PALETTEENTRY *);

BOOL    WINAPI AppendMenu( HMENU, UINT, UINT, LPCSTR );

BOOL    WINAPI Arc( HDC, int, int, int, int, int, int, int, int );

BOOL    WINAPI ArcTo( HDC, int, int, int, int, int xRad1, int yRad1, int xRad2, int yRad2);

UINT    WINAPI ArrangeIconicWindows( HWND );

BOOL    WINAPI Beep( DWORD, DWORD );

HDWP    WINAPI BeginDeferWindowPos( int );

HDC     WINAPI BeginPaint( HWND, PPAINTSTRUCT );

BOOL    WINAPI BeginPath( HDC );

BOOL    WINAPI BitBlt( HDC , int, int, int, int, HDC , int, int, DWORD );

BOOL    WINAPI BringWindowToTop( HWND );

BOOL    WINAPI CallMsgFilter( PMSG, int );

LRESULT WINAPI CallNextHookEx( HHOOK, int, WPARAM, LPARAM );

LRESULT WINAPI CallWindowProc( WNDPROC, HWND, UINT, WPARAM, LPARAM );

BOOL    WINAPI ChangeClipboardChain( HWND, HWND );

LPSTR   WINAPI CharLower( LPSTR );

DWORD   WINAPI CharLowerBuff( LPSTR, DWORD );

LPSTR   WINAPI CharUpper( LPSTR );

DWORD   WINAPI CharUpperBuff( LPSTR, DWORD );

BOOL    WINAPI CheckDlgButton( HWND, int, UINT );

DWORD   WINAPI CheckMenuItem( HMENU, UINT, UINT );

BOOL    WINAPI CheckRadioButton( HWND, int, int, int );

HWND    WINAPI ChildWindowFromPoint( HWND, POINT );

BOOL    WINAPI ChooseColor( PCHOOSECOLOR );

BOOL    WINAPI ChooseFont( LPCHOOSEFONT );

BOOL    WINAPI Chord( HDC , int, int, int, int, int xRadial1, int yRadial1, int xRadial2, int yRadial2);

BOOL    WINAPI ClientToScreen( HWND, PPOINT );

BOOL    WINAPI ClipCursor( const RECT *);

BOOL    WINAPI CloseClipboard( VOID );

HENHMETAFILE WINAPI CloseEnhMetaFile( HDC );

BOOL    WINAPI CloseFigure( HDC );

BOOL    WINAPI CloseHandle( HANDLE );

HMETAFILE WINAPI CloseMetaFile( HDC );

BOOL    WINAPI CloseWindow( HWND );

int       WINAPI CombineRgn( HRGN, HRGN hrgnSrc1, HRGN hrgnSrc2, int );

DWORD   WINAPI CommDlgExtendedError( VOID );

LONG    WINAPI CompareFileTime( const FILETIME *, const FILETIME * );

int       WINAPI CopyAcceleratorTable( HACCEL, PACCEL, int );

LCID    WINAPI ConvertDefaultLocale( LCID );

HCURSOR WINAPI CopyCursor( HCURSOR );

HENHMETAFILE WINAPI CopyEnhMetaFile( HENHMETAFILE, LPCSTR );

BOOL    WINAPI CopyFile( LPCSTR, LPCSTR, BOOL );

HICON   WINAPI CopyIcon( HICON );

HMETAFILE WINAPI CopyMetaFile( HMETAFILE, LPCSTR );

BOOL    WINAPI CopyRect( PRECT, const RECT *);

int       WINAPI CountClipboardFormats( VOID );

HACCEL  WINAPI CreateAcceleratorTable( PACCEL, int );

HBITMAP WINAPI CreateBitmap( int, int, UINT, UINT, const void * );

HBITMAP WINAPI CreateBitmapIndirect( const BITMAP * );

HBRUSH  WINAPI CreateBrushIndirect( PLOGBRUSH );

BOOL    WINAPI CreateCaret( HWND, HBITMAP, int, int );

HBITMAP WINAPI CreateCompatibleBitmap( HDC , int, int );

HDC     WINAPI CreateCompatibleDC( HDC );

HCURSOR WINAPI CreateCursor( HINSTANCE, int, int, int, int, const VOID *, const VOID *);

HDC     WINAPI CreateDC( LPCSTR, LPCSTR, LPCSTR, const DEVMODE *);

HBRUSH  WINAPI CreateDIBPatternBrushPt( const VOID *, UINT );

HBITMAP WINAPI CreateDIBitmap( HDC, const BITMAPINFOHEADER *, DWORD, const void *, const BITMAPINFO *, UINT );

HWND    WINAPI CreateDialog( HINSTANCE, LPCSTR, HWND, DLGPROC );

HWND    WINAPI CreateDialogParam( HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM );

HWND    WINAPI CreateDialogIndirect( HINSTANCE, const DLGTEMPLATE *, HWND, DLGPROC );

HWND    WINAPI CreateDialogIndirectParam( HINSTANCE, const DLGTEMPLATE *, HWND, DLGPROC, LPARAM );

BOOL    WINAPI CreateDirectory( LPCSTR, PSECURITY_ATTRIBUTES );

HRGN    WINAPI CreateEllipticRgn( int, int, int, int );

HRGN    WINAPI CreateEllipticRgnIndirect( const RECT * );

HDC     WINAPI CreateEnhMetaFile( HDC, LPCSTR, const RECT *, LPCSTR );

HANDLE  WINAPI CreateEvent( PSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR );

HANDLE  WINAPI CreateFile( LPCSTR, DWORD, DWORD, PSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE );

HFONT   WINAPI CreateFont( int, int, int, int, int, DWORD, DWORD, DWORD, DWORD,
                                  DWORD, DWORD, DWORD, DWORD, LPCSTR );

HFONT   WINAPI CreateFontIndirect( const  LOGFONT *);

HBRUSH  WINAPI CreateHatchBrush( int, COLORREF );

HDC     WINAPI CreateIC( LPCSTR, LPCSTR, LPCSTR, const DEVMODE *);

HICON   WINAPI CreateIcon( HINSTANCE, int, int, BYTE, BYTE, const BYTE *, const BYTE * );

HICON   WINAPI CreateIconFromResource( PBYTE, DWORD, BOOL, DWORD );

HICON   WINAPI CreateIconIndirect( PICONINFO );

HMENU   WINAPI CreateMenu( void);

HDC     WINAPI CreateMetaFile( LPCSTR );

HWND    WINAPI CreateMDIWindow( LPSTR, LPSTR, DWORD, int, int, int, int, HWND, HINSTANCE, LPARAM );

HANDLE  WINAPI CreateMutex( PSECURITY_ATTRIBUTES, BOOL, LPCSTR );

HPALETTE WINAPI CreatePalette( const LOGPALETTE * );

HBRUSH  WINAPI CreatePatternBrush( HBITMAP );

HPEN    WINAPI CreatePen( int, int, COLORREF );

HPEN    WINAPI CreatePenIndirect( const LOGPEN * );

HRGN    WINAPI CreatePolyPolygonRgn( const POINT *, const INT *, int, int );

HRGN    WINAPI CreatePolygonRgn( const POINT *, int, int );

HMENU   WINAPI CreatePopupMenu( VOID );

BOOL    WINAPI CreateProcess( LPCSTR, LPCSTR, PSECURITY_ATTRIBUTES, PSECURITY_ATTRIBUTES,
                         BOOL, DWORD, PVOID, LPCSTR, PSTARTUPINFO,
                         PPROCESS_INFORMATION );

HRGN    WINAPI CreateRectRgn( int, int, int, int );

HRGN    WINAPI CreateRectRgnIndirect( const RECT * );

HRGN    WINAPI CreateRoundRectRgn( int, int, int, int, int, int );

HANDLE  WINAPI CreateSemaphore( PSECURITY_ATTRIBUTES, LONG, LONG, LPSTR );

HBRUSH  WINAPI CreateSolidBrush( COLORREF );

HANDLE  WINAPI CreateThread( PSECURITY_ATTRIBUTES, DWORD, PTHREAD_START_ROUTINE, PVOID, DWORD, PDWORD );

HWND    WINAPI CreateWindowEx( DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, PVOID );

BOOL    WINAPI DPtoLP( HDC, PPOINT, int );

BOOL    WINAPI DdeAbandonTransaction( DWORD, HCONV, DWORD );

PBYTE   WINAPI DdeAccessData(HDDEDATA, PDWORD );

HDDEDATA WINAPI DdeAddData( HDDEDATA, LPBYTE, DWORD, DWORD );

HDDEDATA WINAPI DdeClientTransaction( LPBYTE, DWORD, HCONV,
                          HSZ, UINT, UINT, DWORD, LPDWORD );

int       WINAPI DdeCmpStringHandles( HSZ hsz1, HSZ hsz2);

HCONV   WINAPI DdeConnect( DWORD, HSZ, HSZ, PCONVCONTEXT );

HCONVLIST WINAPI DdeConnectList(DWORD, HSZ, HSZ, HCONVLIST, PCONVCONTEXT );

HDDEDATA WINAPI DdeCreateDataHandle(DWORD, LPBYTE, DWORD, DWORD, HSZ, UINT, UINT );

HSZ     WINAPI DdeCreateStringHandle(DWORD, LPTSTR, int );

BOOL    WINAPI DdeDisconnect(HCONV );

BOOL    WINAPI DdeDisconnectList( HCONVLIST );

BOOL    WINAPI DdeEnableCallback(DWORD, HCONV, UINT );

BOOL    WINAPI DdeFreeDataHandle( HDDEDATA );

BOOL    WINAPI DdeFreeStringHandle(DWORD, HSZ );

DWORD   WINAPI DdeGetData( HDDEDATA, LPBYTE, DWORD, DWORD );

UINT    WINAPI DdeGetLastError(DWORD );

UINT    WINAPI DdeInitialize(PDWORD, PFNCALLBACK, DWORD, DWORD );

BOOL    WINAPI DdeKeepStringHandle(DWORD, HSZ );

HDDEDATA WINAPI DdeNameService( DWORD, HSZ hsz1, HSZ hsz2, UINT );

BOOL    WINAPI DdePostAdvise(DWORD, HSZ, HSZ );

UINT    WINAPI DdeQueryConvInfo( HCONV, DWORD, PCONVINFO );

HCONV   WINAPI DdeQueryNextServer( HCONVLIST, HCONV );

DWORD   WINAPI DdeQueryString(DWORD, HSZ, LPSTR, DWORD, int );

HCONV   WINAPI DdeReconnect( HCONV );

BOOL    WINAPI DdeSetUserHandle( HCONV, DWORD, DWORD );

BOOL    WINAPI DdeUnaccessData( HDDEDATA );

BOOL    WINAPI DdeUninitialize(DWORD );

LRESULT WINAPI DefDlgProc( HWND, UINT, WPARAM mp1, LPARAM mp2 );

HDWP    WINAPI DeferWindowPos( HDWP, HWND, HWND hWndInsertAfter , int, int, int, int, UINT );

LRESULT WINAPI DefFrameProc( HWND, HWND, UINT, WPARAM, LPARAM );

LRESULT WINAPI DefMDIChildProc( HWND, UINT, WPARAM, LPARAM );

LRESULT WINAPI DefWindowProc( HWND, UINT, WPARAM mp1, LPARAM mp2 );

ATOM    WINAPI DeleteAtom( ATOM );

VOID    WINAPI DeleteCriticalSection( PCRITICAL_SECTION );

BOOL    WINAPI DeleteDC( HDC );

BOOL    WINAPI DeleteEnhMetaFile( HENHMETAFILE );

BOOL    WINAPI DeleteFile( LPCSTR );

VOID    WINAPI DeleteHDCObjects( HDC );

BOOL    WINAPI DeleteMenu( HMENU, UINT, UINT );

BOOL    WINAPI DeleteMetaFile( HMETAFILE );

BOOL    WINAPI DeleteObject( HANDLE );

BOOL    WINAPI DestroyAcceleratorTable( HACCEL );

BOOL    WINAPI DestroyCaret( VOID );

BOOL    WINAPI DestroyCursor( HCURSOR );

BOOL    WINAPI DestroyIcon( HICON );

BOOL    WINAPI DestroyMenu( HMENU );

BOOL    WINAPI DestroyWindow( HWND );

DWORD   WINAPI DeviceCapabilities( LPCTSTR, LPCTSTR, WORD, LPTSTR, const DEVMODE *);

int       WINAPI DialogBox( HINSTANCE, LPCSTR, HWND, DLGPROC );

int       WINAPI DialogBoxParam( HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM );

int       WINAPI DialogBoxIndirect( HINSTANCE, LPDLGTEMPLATE, HWND, DLGPROC );

int       WINAPI DialogBoxIndirectParam( HINSTANCE, LPDLGTEMPLATE, HWND, DLGPROC, LPARAM );

LONG    WINAPI DispatchMessage( const MSG * );

int       WINAPI DlgDirList( HWND, LPSTR, int, int, UINT );

int       WINAPI DlgDirListComboBox( HWND, LPSTR, int, int, UINT );

BOOL    WINAPI DlgDirSelectEx( HWND, LPSTR, int, int );

BOOL    WINAPI DlgDirSelectComboBoxEx( HWND, LPSTR, int, int );

BOOL    WINAPI DllEntryPoint( HINSTANCE, DWORD, LPVOID );

BOOL    WINAPI DosDateTimeToFileTime( WORD, WORD, PFILETIME );

VOID    WINAPI DragAcceptFiles( HWND, BOOL );

VOID    WINAPI DragFinish( HDROP );

UINT    WINAPI DragQueryFile( HDROP, UINT, LPTSTR, UINT );

BOOL    WINAPI DragQueryPoint( HDROP, PPOINT );

BOOL    WINAPI DrawFocusRect( HDC, const RECT * );

BOOL    WINAPI DrawIcon( HDC, int, int, HICON );

BOOL    WINAPI DrawMenuBar( HWND );

int       WINAPI DrawText( HDC, LPCSTR, int, PRECT, UINT );

BOOL    WINAPI DuplicateHandle( HANDLE, HANDLE, HANDLE, PHANDLE, DWORD, BOOL, DWORD );

BOOL    WINAPI Ellipse( HDC, int, int, int, int );

BOOL    WINAPI EmptyClipboard( void);

BOOL    WINAPI EnableMenuItem( HMENU, UINT, UINT );

BOOL    WINAPI EnableScrollBar( HWND, UINT, UINT );

BOOL    WINAPI EnableWindow( HWND, BOOL );

BOOL    WINAPI EndDeferWindowPos( HDWP );

BOOL    WINAPI EndDialog( HWND, int );

int       WINAPI EndDoc( HDC );

int       WINAPI EndPage( HDC );

BOOL    WINAPI EndPath( HDC );

BOOL    WINAPI EndPaint( HWND, const PAINTSTRUCT *);

VOID    WINAPI EnterCriticalSection( PCRITICAL_SECTION );

BOOL    WINAPI EnumChildWindows( HWND, WNDENUMPROC, LPARAM );

UINT    WINAPI EnumClipboardFormats( UINT );

BOOL    WINAPI EnumEnhMetaFile( HDC , HENHMETAFILE, ENHMFENUMPROC, PVOID, const RECT * );

int       WINAPI EnumFonts( HDC , LPCSTR, OLDFONTENUMPROC, LPARAM );

int       WINAPI EnumFontFamilies( HDC , LPCSTR, FONTENUMPROC, LPARAM );

BOOL    WINAPI EnumMetaFile( HDC , HMETAFILE, MFENUMPROC, LPARAM );

int       WINAPI EnumObjects( HDC , int, GOBJENUMPROC, LPARAM );

BOOL    WINAPI EnumPrinters( DWORD, LPTSTR, DWORD, LPBYTE, DWORD, LPDWORD, LPDWORD);

int       WINAPI EnumProps( HWND, PROPENUMPROC );

int       WINAPI EnumPropsEx( HWND, PROPENUMPROCEX, LPARAM );

BOOL    WINAPI EnumSystemLocales( LOCALE_ENUMPROC, DWORD );

BOOL    WINAPI EnumThreadWindows( DWORD, WNDENUMPROC, LPARAM );

BOOL    WINAPI EnumWindows( WNDENUMPROC, LPARAM );

BOOL    WINAPI EqualRect( const RECT *, const RECT * );

BOOL    WINAPI EqualRgn( HRGN, HRGN );

int       WINAPI Escape( HDC, int, int, LPCSTR, PVOID );

int       WINAPI ExcludeClipRect( HDC, int, int, int, int );

BOOL    WINAPI ExcludeUpdateRgn( HDC, HWND );

VOID    WINAPI ExitProcess( UINT );

VOID    WINAPI ExitThread( DWORD );

BOOL    WINAPI ExitWindows( DWORD, UINT );

BOOL    WINAPI ExitWindowsEx( UINT, DWORD );

HPEN    WINAPI ExtCreatePen( DWORD, DWORD, const LOGBRUSH *, DWORD, const DWORD * );

HRGN    WINAPI ExtCreateRegion( const XFORM *, DWORD, const RGNDATA * );

BOOL    WINAPI ExtFloodFill( HDC, int, int, COLORREF, UINT );

HICON   WINAPI ExtractIcon ( HINSTANCE , LPCSTR, UINT );

int       WINAPI ExtSelectClipRgn( HDC, HRGN, int );

BOOL    WINAPI ExtTextOut( HDC, int, int, UINT, const RECT *, LPCSTR, UINT, const int * );

VOID    WINAPI FatalAppExit( UINT, LPCSTR );

VOID    WINAPI FatalExit( UINT );

BOOL    WINAPI FileTimeToDosDateTime( const FILETIME *, PWORD, PWORD );

BOOL    WINAPI FileTimeToLocalFileTime( const FILETIME *, PFILETIME );

BOOL    WINAPI FileTimeToSystemTime( const FILETIME *, PSYSTEMTIME );

BOOL    WINAPI FillPath( HDC );

int       WINAPI FillRect( HDC, const RECT *, HBRUSH );

BOOL    WINAPI FillRgn( HDC, HRGN, HBRUSH );

ATOM    WINAPI FindAtom( LPCSTR );

BOOL    WINAPI FindClose( HANDLE );

HANDLE  WINAPI FindFirstFile( LPCSTR, PWIN32_FIND_DATA );

BOOL    WINAPI FindNextFile( HANDLE, PWIN32_FIND_DATA );

HRSRC   WINAPI FindResource( HINSTANCE, LPCSTR, LPCSTR );

HWND    WINAPI FindText( PFINDREPLACE );

HWND    WINAPI FindWindow( LPCSTR lpClassName , LPCSTR );

BOOL    WINAPI FlashWindow( HWND, BOOL );

BOOL    WINAPI FlattenPath( HDC );

BOOL    WINAPI FlushFileBuffers( HANDLE );

int       WINAPI FrameRect( HDC, const RECT *, HBRUSH );

BOOL    WINAPI FrameRgn( HDC, HRGN, HBRUSH, int, int );

BOOL    WINAPI FreeDDElParam( UINT, LONG );

BOOL    WINAPI FreeLibrary( HINSTANCE );

UINT    WINAPI GetACP( VOID );

HWND    WINAPI GetActiveWindow( void);

int       WINAPI GetArcDirection( HDC );

BOOL    WINAPI GetAspectRatioFilterEx( HDC, LPSIZE );

SHORT   WINAPI GetAsyncKeyState( int );

UINT    WINAPI GetAtomName( ATOM, LPTSTR, int );

LONG    WINAPI GetBitmapBits( HBITMAP, LONG, PVOID );

BOOL    WINAPI GetBitmapDimensionEx( HBITMAP, PSIZE );

COLORREF WINAPI GetBkColor( HDC );

int       WINAPI GetBkMode( HDC );

UINT    WINAPI GetBoundsRect( HDC, LPRECT, UINT );

BOOL    WINAPI GetBrushOrgEx( HDC, LPPOINT );

HWND    WINAPI GetCapture( void);

UINT    WINAPI GetCaretBlinkTime( VOID );

BOOL    WINAPI GetCaretPos( PPOINT );

BOOL    WINAPI GetCharABCWidths( HDC, UINT, UINT, PABC );

BOOL    WINAPI GetCharWidth( HDC, UINT, UINT, LPINT );

BOOL    WINAPI GetClassInfo( HINSTANCE hInstance , LPCTSTR, LPWNDCLASS );

DWORD   WINAPI GetClassLong( HWND, int );

int       WINAPI GetClassName( HWND, LPTSTR, int );

WORD    WINAPI GetClassWord( HWND, int );

BOOL    WINAPI GetClientRect( HWND, LPRECT );

HANDLE  WINAPI GetClipboardData( UINT );

int       WINAPI GetClipboardFormatName( UINT, LPTSTR, int );

HWND    WINAPI GetClipboardOwner( void);

HWND    WINAPI GetClipboardViewer( VOID );

int       WINAPI GetClipBox( HDC, LPRECT );

BOOL    WINAPI GetClipCursor( LPRECT );

int       WINAPI GetClipRgn( HDC, HRGN );

LPTSTR   WINAPI GetCommandLine( VOID );

DWORD   WINAPI GetCurrentDirectory( DWORD, LPTSTR );

HGDIOBJ WINAPI GetCurrentObject( HDC, UINT );

BOOL    WINAPI GetCurrentPositionEx( HDC, LPPOINT );

HANDLE  WINAPI GetCurrentProcess( VOID );

DWORD   WINAPI GetCurrentProcessId( VOID );

HANDLE  WINAPI GetCurrentThread( VOID );

DWORD   WINAPI GetCurrentThreadId( VOID );

DWORD   WINAPI GetCurrentTime( VOID );

HCURSOR WINAPI GetCursor( VOID );

BOOL    WINAPI GetCursorPos( PPOINT );

HDC     WINAPI GetDC( HWND );

HDC     WINAPI GetDCEx( HWND, HRGN, DWORD );

BOOL    WINAPI GetDCOrgEx( HDC, LPPOINT );

int       WINAPI GetDIBits( HDC, HBITMAP, UINT, UINT, LPVOID, LPBITMAPINFO, UINT );

int       WINAPI GetDeviceCaps( HDC, int );

LONG    WINAPI GetDialogBaseUnits( void);

BOOL    WINAPI GetDiskFreeSpace( LPCTSTR, LPDWORD, LPDWORD, LPDWORD, PDWORD );

int       WINAPI GetDlgCtrlID( HWND );

HWND    WINAPI GetDlgItem( HWND, int );

UINT    WINAPI GetDlgItemInt( HWND, int, PBOOL, BOOL );

UINT    WINAPI GetDlgItemText( HWND, int, LPTSTR, int );

UINT    WINAPI GetDoubleClickTime( VOID );

UINT    WINAPI GetDriveType( LPCTSTR );

HENHMETAFILE WINAPI GetEnhMetaFile( LPCTSTR );

UINT    WINAPI GetEnhMetaFileBits( HENHMETAFILE, UINT, LPBYTE );

UINT    WINAPI GetEnhMetaFileHeader( HENHMETAFILE, UINT, PENHMETAHEADER );

UINT    WINAPI GetEnhMetaFileDescription( HENHMETAFILE, UINT, LPTSTR );

UINT    WINAPI GetEnhMetaFilePaletteEntries( HENHMETAFILE, UINT, PPALETTEENTRY );

PVOID   WINAPI GetEnvironmentStrings( VOID );

DWORD   WINAPI GetEnvironmentVariable(LPCTSTR, LPTSTR, DWORD );

BOOL    WINAPI GetExitCodeProcess( HANDLE, LPDWORD );

BOOL    WINAPI GetExitCodeThread( HANDLE, LPDWORD );

DWORD   WINAPI GetFileAttributes( LPCTSTR );

BOOL    WINAPI GetFileInformationByHandle( HANDLE, PBY_HANDLE_FILE_INFORMATION );

DWORD   WINAPI GetFileSize( HANDLE, LPDWORD );

BOOL    WINAPI GetFileTime( HANDLE, PFILETIME, PFILETIME, PFILETIME );

short     WINAPI GetFileTitle( LPCSTR, LPSTR, WORD );

DWORD   WINAPI GetFileType( HANDLE );

HWND    WINAPI GetFocus( VOID );

HWND    WINAPI GetForegroundWindow( VOID );

DWORD   WINAPI GetFullPathName( LPCSTR, DWORD, LPSTR, LPSTR * );

int       WINAPI GetGraphicsMode(HDC);

BOOL    WINAPI GetIconInfo( HICON, PICONINFO );

DWORD   WINAPI GetKerningPairs( HDC, DWORD, PKERNINGPAIR );

HKL     WINAPI GetKeyboardLayout( DWORD );

int       WINAPI GetKeyboardType( int );

BOOL    WINAPI GetKeyboardState( PBYTE );

BOOL    WINAPI SetKeyboardState( LPBYTE );

int       WINAPI GetKeyNameText( LPARAM, LPSTR, int );

SHORT   WINAPI GetKeyState( int );

HWND    WINAPI GetLastActivePopup( HWND );

DWORD   WINAPI GetLastError( VOID );

int       WINAPI GetLocaleInfo( LCID, LCTYPE, LPSTR, int );

VOID    WINAPI GetLocalTime( PSYSTEMTIME );

DWORD   WINAPI GetLogicalDriveStrings( DWORD, LPSTR );

DWORD   WINAPI GetLogicalDrives( VOID );

int       WINAPI GetMapMode( HDC);

HMENU   WINAPI GetMenu( HWND );

LONG    WINAPI GetMenuCheckMarkDimensions(void);

DWORD   WINAPI GetMenuContextHelpId( HMENU);

int       WINAPI GetMenuItemCount( HMENU );

UINT    WINAPI GetMenuItemID( HMENU, int );

UINT    WINAPI GetMenuState( HMENU, UINT, UINT );

int       WINAPI GetMenuString( HMENU, UINT, LPSTR, int, UINT );

BOOL    WINAPI GetMessage( LPMSG, HWND hwnd , UINT, UINT );

LONG    WINAPI GetMessageExtraInfo( VOID );

DWORD   WINAPI GetMessagePos( VOID );

LONG    WINAPI GetMessageTime( VOID );

HMETAFILE WINAPI GetMetaFile( LPCTSTR );

UINT    WINAPI GetMetaFileBitsEx( HMETAFILE, UINT, LPVOID );

BOOL    WINAPI GetMiterLimit( HDC, PFLOAT);

DWORD   WINAPI GetModuleFileName( HMODULE, LPTSTR, DWORD );

HMODULE WINAPI GetModuleHandle( LPCTSTR );

COLORREF WINAPI GetNearestColor( HDC, COLORREF );

UINT    WINAPI GetNearestPaletteIndex( HPALETTE, COLORREF );

HWND    WINAPI GetNextDlgGroupItem( HWND, HWND, BOOL );

HWND    WINAPI GetNextDlgTabItem( HWND, HWND, BOOL );

HWND    WINAPI GetNextWindow( HWND, UINT );

UINT    WINAPI GetOEMCP( VOID );

int       WINAPI GetObject( HGDIOBJ, int, void * );

DWORD   WINAPI GetObjectType( HGDIOBJ );

HWND    WINAPI GetOpenClipboardWindow( VOID );

BOOL    WINAPI GetOpenFileName( POPENFILENAME );

UINT    WINAPI GetOutlineTextMetrics( HDC, UINT, POUTLINETEXTMETRIC );

BOOL    WINAPI GetOverlappedResult(HANDLE, LPOVERLAPPED, LPDWORD, BOOL );

UINT    WINAPI GetPaletteEntries( HPALETTE, UINT, UINT, PPALETTEENTRY );

HWND    WINAPI GetParent( HWND );

UINT    WINAPI GetPath( HDC, LPPOINT, LPBYTE, int );

COLORREF WINAPI GetPixel( HDC, int, int );

int       WINAPI GetPolyFillMode( HDC);

DWORD   WINAPI GetPriorityClass( HANDLE );

int       WINAPI GetPriorityClipboardFormat( PUINT, int );

UINT    WINAPI GetPrivateProfileInt( LPCTSTR, LPCTSTR, INT, LPCTSTR );

DWORD   WINAPI GetPrivateProfileString( LPCTSTR, LPCTSTR, LPCTSTR, LPTSTR, DWORD, LPCTSTR );

FARPROC WINAPI GetProcAddress( HMODULE, LPCSTR );

UINT    WINAPI GetProfileInt( LPCTSTR, LPCTSTR, INT );

DWORD   WINAPI GetProfileString( LPCTSTR, LPCTSTR, LPCTSTR, LPTSTR, DWORD );

HANDLE  WINAPI GetProp( HWND, LPCSTR );

DWORD   WINAPI GetQueueStatus( UINT );

int       WINAPI GetROP2( HDC );

BOOL    WINAPI GetRasterizerCaps( PRASTERIZER_STATUS, UINT );

int       WINAPI GetRgnBox( HRGN, PRECT );

DWORD   WINAPI GetRegionData( HRGN, DWORD, PRGNDATA );

BOOL    WINAPI GetSaveFileName( POPENFILENAME );

int       WINAPI GetScrollPos( HWND, int );

BOOL    WINAPI GetScrollRange( HWND, int, LPINT, LPINT );

HANDLE  WINAPI GetStdHandle( DWORD );

HGDIOBJ WINAPI GetStockObject( int );

int       WINAPI GetStretchBltMode( HDC );

BOOL    WINAPI GetStringTypeEx( LCID, DWORD, LPCSTR, int, LPWORD );

HMENU   WINAPI GetSubMenu( HWND, int );

DWORD   WINAPI GetSysColor( int );

UINT    WINAPI GetSystemDirectory( LPSTR, UINT );

HMENU   WINAPI GetSystemMenu( HWND, BOOL );

int       WINAPI GetSystemMetrics( int );

UINT    WINAPI GetSystemPaletteEntries( HDC, UINT, UINT, PPALETTEENTRY );

void      WINAPI GetSystemTime( PSYSTEMTIME );

DWORD   WINAPI GetTabbedTextExtent( HDC, LPCSTR, int, int, int * );

UINT    WINAPI GetTempFileName( LPCSTR, LPCSTR, UINT, LPSTR );

DWORD   WINAPI GetTempPath( DWORD, LPSTR );

UINT    WINAPI GetTextAlign( HDC);

int       WINAPI GetTextCharacterExtra( HDC);

COLORREF WINAPI GetTextColor( HDC );

BOOL    WINAPI GetTextExtentExPoint( HDC, LPCTSTR, INT, INT, LPINT, LPINT, LPSIZE );

BOOL    WINAPI GetTextExtentPoint( HDC, LPCTSTR, int, LPSIZE );

BOOL    WINAPI GetTextExtentPoint32( HDC, LPCTSTR, int, LPSIZE );

int       WINAPI GetTextFace( HDC, int, LPSTR );

BOOL    WINAPI GetTextMetrics( HDC, LPTEXTMETRIC );

LCID    WINAPI GetThreadLocale( void );

DWORD   WINAPI GetThreadPriority( HANDLE );

DWORD   WINAPI GetTickCount( VOID );

DWORD   WINAPI GetTimeZoneInformation( PTIME_ZONE_INFORMATION );

HWND    WINAPI GetTopWindow( HWND );

BOOL    WINAPI GetUpdateRect( HWND, LPRECT, BOOL );

int       WINAPI GetUpdateRgn( HWND, HRGN, BOOL );

LCID    WINAPI GetUserDefaultLCID( void );

LANGID  WINAPI GetUserDefaultLangID( void );

DWORD   WINAPI GetVersion( VOID );

BOOL    WINAPI GetVersionEx( LPOSVERSIONINFO );

BOOL    WINAPI GetViewportExtEx( HDC, LPSIZE );

BOOL    WINAPI GetViewportOrgEx( HDC, LPPOINT );

BOOL    WINAPI GetVolumeInformation( LPCSTR, LPSTR, DWORD, PDWORD,
                       PDWORD, PDWORD, LPSTR, DWORD );

UINT    WINAPI GetWinMetaFileBits( HENHMETAFILE, UINT, PBYTE, int, HDC);

HWND    WINAPI GetWindow( HWND, UINT );

HDC     WINAPI GetWindowDC( HWND );

BOOL    WINAPI GetWindowExtEx( HDC, PSIZE );

LONG    WINAPI GetWindowLong( HWND, int );

BOOL    WINAPI GetWindowOrgEx( HDC, LPPOINT );

BOOL    WINAPI GetWindowPlacement( HWND, PWINDOWPLACEMENT );

BOOL    WINAPI GetWindowRect( HWND, LPRECT );

UINT    WINAPI GetWindowsDirectory( LPSTR, UINT );

int       WINAPI GetWindowText( HWND, LPSTR, int );

int       WINAPI GetWindowTextLength( HWND );

DWORD   WINAPI GetWindowThreadProcessId(HWND, PDWORD );

WORD    WINAPI GetWindowWord( HWND, int );

BOOL    WINAPI GetWorldTransform( HDC, LPXFORM );

ATOM    WINAPI GlobalAddAtom( LPCTSTR );

HGLOBAL WINAPI GlobalAlloc( UINT, DWORD );

ATOM    WINAPI GlobalDeleteAtom( ATOM );

HGLOBAL WINAPI GlobalDiscard( HGLOBAL );

ATOM    WINAPI GlobalFindAtom( LPCSTR );

UINT    WINAPI GlobalFlags( HGLOBAL );

HGLOBAL WINAPI GlobalFree( HGLOBAL );

UINT    WINAPI GlobalGetAtomName( ATOM, LPSTR, int );

HGLOBAL WINAPI GlobalHandle( PVOID memPtr );

PVOID   WINAPI GlobalLock( HGLOBAL hglbMem );

VOID    WINAPI GlobalMemoryStatus( PMEMORYSTATUS );

HGLOBAL WINAPI GlobalReAlloc( HGLOBAL, DWORD, UINT );

DWORD   WINAPI GlobalSize( HGLOBAL );

BOOL    WINAPI GlobalUnlock( HGLOBAL hglbMem );

LPVOID  WINAPI HeapAlloc( HANDLE, DWORD, DWORD );

HANDLE  WINAPI HeapCreate( DWORD, DWORD, DWORD );

BOOL    WINAPI HeapDestroy( HANDLE );

BOOL    WINAPI HeapFree( HANDLE, DWORD, PVOID );

LPVOID  WINAPI HeapReAlloc( HANDLE, DWORD, LPVOID, DWORD );

DWORD   WINAPI HeapSize( HANDLE, DWORD, LPCVOID );

BOOL    WINAPI HideCaret( HWND );

BOOL    WINAPI HiliteMenuItem( HWND, HMENU, UINT, UINT );

HDC     WINAPI HPSToHDC( HWND, HDC, LPCSTR, LPRECT );

BOOL    WINAPI InflateRect( LPRECT, int, int );

BOOL    WINAPI InSendMessage( VOID );

BOOL    WINAPI InitAtomTable( DWORD );

VOID    WINAPI InitializeCriticalSection( PCRITICAL_SECTION );

BOOL    WINAPI InsertMenu( HMENU, UINT, UINT, UINT, LPCTSTR );

LONG    WINAPI InterlockedDecrement( PLONG );

LONG    WINAPI InterlockedExchange( PLONG, LONG );

LONG    WINAPI InterlockedIncrement( PLONG );

int       WINAPI IntersectClipRect( HDC, int, int, int, int );

BOOL    WINAPI IntersectRect( LPRECT, const RECT *, const RECT *);

BOOL    WINAPI InvalidateRect( HWND, const RECT *, BOOL );

BOOL    WINAPI InvalidateRgn( HWND, HRGN, BOOL );

BOOL    WINAPI InvertRect( HDC, const RECT *);

BOOL    WINAPI InvertRgn( HDC, HRGN );

BOOL    WINAPI IsBadCodePtr( FARPROC );

BOOL    WINAPI IsBadHugeReadPtr( const VOID *, UINT );

BOOL    WINAPI IsBadHugeWritePtr( LPVOID, UINT );

BOOL    WINAPI IsBadReadPtr( const VOID *, UINT );

BOOL    WINAPI IsBadStringPtr( LPCTSTR, UINT );

BOOL    WINAPI IsBadWritePtr( LPVOID, UINT );

BOOL    WINAPI IsChild( HWND, HWND );

BOOL    WINAPI IsClipboardFormatAvailable( UINT );

BOOL    WINAPI IsDBCSLeadByte( BYTE );

BOOL    WINAPI IsDialogMessage( HWND, LPMSG );

UINT    WINAPI IsDlgButtonChecked( HWND, int );

BOOL    WINAPI IsIconic( HWND );

BOOL    WINAPI IsMenu( HMENU );

BOOL    WINAPI IsRectEmpty( const RECT *);

BOOL    WINAPI IsValidLocale( LCID, DWORD );

BOOL    WINAPI IsWindow( HWND );

BOOL    WINAPI IsWindowEnabled( HWND );

BOOL    WINAPI IsWindowVisible( HWND );

BOOL    WINAPI IsZoomed( HWND );

BOOL    WINAPI KillTimer(HWND, UINT );

BOOL    WINAPI LPtoDP( HDC, LPPOINT, int );

VOID    WINAPI LeaveCriticalSection( PCRITICAL_SECTION );

BOOL    WINAPI LineDDA( int, int, int, int, LINEDDAPROC, LPARAM );

BOOL    WINAPI LineTo( HDC, int, int );

HACCEL  WINAPI LoadAccelerators( HINSTANCE, LPCSTR );

HBITMAP WINAPI LoadBitmap( HINSTANCE, LPCSTR );

HCURSOR WINAPI LoadCursor( HINSTANCE, LPCSTR );

HICON   WINAPI LoadIcon( HINSTANCE, LPCSTR );

HINSTANCE WINAPI LoadLibrary( LPCSTR );

HMENU   WINAPI LoadMenu( HINSTANCE, LPCSTR );

HMENU   WINAPI LoadMenuIndirect( const MENUITEMTEMPLATEHEADER * );

DWORD   WINAPI LoadModule( LPCSTR, PVOID );

HGLOBAL WINAPI LoadResource( HINSTANCE, HRSRC );

PVOID   WINAPI LockResource( HGLOBAL );

int       WINAPI LoadString( HINSTANCE, UINT, LPSTR, int );

HLOCAL  WINAPI LocalAlloc( UINT, UINT );

HLOCAL  WINAPI LocalDiscard( HLOCAL );

UINT    WINAPI LocalFlags( HLOCAL );

BOOL    WINAPI LocalFileTimeToFileTime( const FILETIME *, PFILETIME );

HLOCAL  WINAPI LocalFree( HLOCAL );

HLOCAL  WINAPI LocalHandle( LPCVOID );

LPVOID  WINAPI LocalLock( HLOCAL );

HLOCAL  WINAPI LocalReAlloc( HLOCAL, UINT, UINT );

UINT    WINAPI LocalSize( HLOCAL );

BOOL    WINAPI LocalUnlock( HLOCAL hlocal);

BOOL    WINAPI LockFile( HANDLE, DWORD, DWORD, DWORD, DWORD );

BOOL    WINAPI LockWindowUpdate( HWND );

BOOL    WINAPI MapDialogRect( HWND, LPRECT );

UINT    WINAPI MapVirtualKey( UINT, UINT );

int       WINAPI MapWindowPoints( HWND, HWND, PPOINT, int );

BOOL    WINAPI MaskBlt( HDC, int, int, int, int, HDC, int, int, HBITMAP, int, int, DWORD );

int       WINAPI MessageBox( HWND hwnd , LPCSTR, LPCSTR, UINT );

BOOL    WINAPI MessageBeep( UINT );

DWORD   WINAPI MsgWaitForMultipleObjects( DWORD, LPHANDLE, BOOL, DWORD, DWORD );

BOOL    WINAPI ModifyMenu( HMENU, UINT, UINT, UINT, LPCTSTR );

BOOL    WINAPI ModifyWorldTransform( HDC, const XFORM *, DWORD );

BOOL    WINAPI MoveFile( LPCTSTR, LPCTSTR );

BOOL    WINAPI MoveToEx( HDC, int, int, LPPOINT );

BOOL    WINAPI MoveWindow( HWND, int, int, int, int, BOOL );

int       WINAPI MulDiv(int, int, int );

int       WINAPI MultiByteToWideChar( UINT, DWORD, LPCSTR, int, LPWSTR, int );

int       WINAPI OffsetClipRgn( HDC, int, int );

BOOL    WINAPI OffsetRect( LPRECT, int, int );

int       WINAPI OffsetRgn( HRGN, int, int );

BOOL    WINAPI OffsetViewportOrgEx( HDC, int, int, LPPOINT );

BOOL    WINAPI OffsetWindowOrgEx( HDC, int, int, LPPOINT );

BOOL    WINAPI OpenClipboard( HWND );

HANDLE  WINAPI OpenEvent( DWORD, BOOL, LPCTSTR );

HFILE   WINAPI OpenFile( LPCSTR, POFSTRUCT, UINT );

HANDLE  WINAPI OpenMutex( DWORD, BOOL, LPCTSTR );

HANDLE  WINAPI OpenProcess( DWORD, BOOL, DWORD );

HANDLE  WINAPI OpenSemaphore( DWORD, BOOL, LPCTSTR );

VOID    WINAPI OutputDebugString( LPCSTR );

LONG    WINAPI PackDDElParam(UINT, UINT, UINT );

BOOL    WINAPI PaintRgn( HDC, HRGN );

BOOL    WINAPI PatBlt( HDC, int, int, int, int, DWORD );

HRGN    WINAPI PathToRegion( HDC);

BOOL    WINAPI PeekMessage( LPMSG, HWND, UINT, UINT, UINT );

BOOL    WINAPI Pie( HDC, int, int, int, int, int, int, int, int );

BOOL    WINAPI PlayEnhMetaFile( HDC, HENHMETAFILE, const RECT * );

BOOL    WINAPI PlayEnhMetaFileRecord( HDC, LPHANDLETABLE, const ENHMETARECORD *, UINT );

BOOL    WINAPI PlayMetaFile( HDC, HMETAFILE );

BOOL    WINAPI PlayMetaFileRecord( HDC, LPHANDLETABLE, LPMETARECORD, UINT );

BOOL    WINAPI PolyBezier( HDC, const POINT *, DWORD );

BOOL    WINAPI PolyBezierTo( HDC, const POINT *, DWORD );

BOOL    WINAPI PolyDraw( HDC, const POINT *, const BYTE *, int );

BOOL    WINAPI Polygon( HDC, const POINT *, int );

BOOL    WINAPI Polyline( HDC, const POINT *, int );

BOOL    WINAPI PolylineTo( HDC, const POINT *, DWORD );

BOOL    WINAPI PolyPolygon( HDC, const POINT *, LPINT, int );

BOOL    WINAPI PolyPolyline( HDC, const POINT *, const DWORD *, DWORD );

VOID    WINAPI PostQuitMessage( int );

BOOL    WINAPI PostMessage( HWND, UINT, WPARAM mp1, LPARAM mp2 );

BOOL    WINAPI PostThreadMessage( DWORD, UINT, WPARAM, LPARAM );

BOOL    WINAPI PtInRect( const RECT *, POINT );

BOOL    WINAPI PtInRegion( HRGN, int, int );

BOOL    WINAPI PtVisible( HDC, int, int );

BOOL    WINAPI PrintDlg( PPRINTDLG );

BOOL    WINAPI PulseEvent ( HANDLE );

BOOL    WINAPI ReadFile( HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED );

UINT    WINAPI RealizePalette( HDC);

BOOL    WINAPI RectInRegion( HRGN, const RECT * );

BOOL    WINAPI RectVisible( HDC, const RECT *);

BOOL    WINAPI Rectangle( HDC, int, int, int, int );

BOOL    WINAPI RedrawWindow( HWND, const RECT *, HRGN, DWORD );

LONG    WINAPI   RegCloseKey( HKEY );

LONG    WINAPI    RegCreateKey( HKEY, LPCTSTR, PHKEY );

LONG    WINAPI    RegCreateKeyEx( HKEY, LPCTSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, PDWORD );

LONG    WINAPI    RegDeleteKey( HKEY, LPCTSTR );

LONG    WINAPI    RegDeleteValue( HKEY, LPSTR );

LONG    WINAPI   RegEnumKey( HKEY, DWORD, LPSTR, DWORD );

LONG    WINAPI   RegEnumKeyEx( HKEY, DWORD, LPSTR, PDWORD, PDWORD, LPSTR, PDWORD, PFILETIME );

LONG    WINAPI   RegEnumValue( HKEY, DWORD, LPSTR, PDWORD, PDWORD, PDWORD, LPBYTE, PDWORD );

LONG    WINAPI   RegFlushKey( HKEY );

LONG    WINAPI   RegOpenKey( HKEY, LPCTSTR, PHKEY );

LONG    WINAPI   RegOpenKeyEx( HKEY, LPCTSTR, DWORD, REGSAM, PHKEY );

LONG    WINAPI   RegQueryInfoKey( HKEY, LPSTR, PDWORD, PDWORD, PDWORD, PDWORD,
                                      PDWORD, PDWORD, PDWORD, PDWORD, PDWORD, PFILETIME );

LONG    WINAPI   RegQueryValue( HKEY, LPCSTR, LPSTR, PLONG );

LONG    WINAPI   RegQueryValueEx( HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD );

LONG    WINAPI   RegSetValue( HKEY, LPCTSTR, DWORD, LPCTSTR, DWORD );

LONG    WINAPI   RegSetValueEx( HKEY, LPCTSTR, DWORD, DWORD, const BYTE *, DWORD );

WORD    WINAPI RegisterClass( const WNDCLASS *);

UINT    WINAPI RegisterClipboardFormat( LPCSTR );

UINT    WINAPI RegisterWindowMessage( LPCSTR );

BOOL    WINAPI ReleaseCapture( void );

int       WINAPI ReleaseDC( HWND, HDC );

BOOL    WINAPI ReleaseMutex( HANDLE );

BOOL    WINAPI ReleaseSemaphore( HANDLE, LONG, LPLONG );

BOOL    WINAPI RemoveDirectory( LPCTSTR );

BOOL    WINAPI RemoveFontResource( LPCSTR );

BOOL    WINAPI RemoveMenu( HMENU, UINT, UINT );

HANDLE  WINAPI RemoveProp( HWND, LPCSTR );

HWND    WINAPI ReplaceText( PFINDREPLACE );

BOOL    WINAPI ReplyMessage( LRESULT );

BOOL    WINAPI ResetDC( HDC, const DEVMODE * );

BOOL    WINAPI ResetEvent( HANDLE );

BOOL    WINAPI ResizePalette( HPALETTE, UINT );

BOOL    WINAPI RestoreDC( HDC, int );

DWORD   WINAPI ResumeThread( HANDLE );

LONG    WINAPI ReuseDDElParam( LONG, UINT, UINT, UINT, UINT );

BOOL    WINAPI RoundRect( HDC, int, int, int, int, int, int );

int       WINAPI SaveDC( HDC );

BOOL    WINAPI ScaleViewportExtEx( HDC, int, int, int, int, LPSIZE );

BOOL    WINAPI ScaleWindowExtEx( HDC, int, int, int, int, LPSIZE );

BOOL    WINAPI ScreenToClient( HWND, LPPOINT );

BOOL    WINAPI ScrollDC( HDC, int, int, const RECT *, const RECT *, HRGN, LPRECT );

BOOL    WINAPI ScrollWindow( HWND, int, int, const RECT *, const RECT * );

int       WINAPI ScrollWindowEx( HWND, int, int, const RECT *, const RECT *, HRGN, LPRECT, UINT );

DWORD   WINAPI SearchPath( LPCSTR, LPCSTR, LPCSTR, DWORD, LPSTR, LPSTR *);

BOOL    WINAPI SelectClipPath( HDC, int );

int       WINAPI SelectClipRgn( HDC, HRGN );

HGDIOBJ WINAPI SelectObject( HDC, HGDIOBJ );

HPALETTE WINAPI SelectPalette( HDC, HPALETTE, BOOL );

LONG    WINAPI SendDlgItemMessage( HWND, int, UINT, WPARAM, LPARAM );

LRESULT WINAPI SendMessage( HWND, UINT, WPARAM mp1, LPARAM mp2 );

int       WINAPI SetAbortProc( HDC, ABORTPROC );

HWND    WINAPI SetActiveWindow( HWND );

int       WINAPI SetArcDirection( HDC, int );

LONG    WINAPI SetBitmapBits( HBITMAP, DWORD, const VOID * );

BOOL    WINAPI SetBitmapDimensionEx( HBITMAP, int, int, LPSIZE );

COLORREF WINAPI SetBkColor( HDC, COLORREF );

int       WINAPI SetBkMode( HDC, int );

LONG    WINAPI SetBoundsRect( HDC, const RECT *, UINT );

BOOL    WINAPI SetBrushOrgEx( HDC, int, int, LPPOINT );

HWND    WINAPI SetCapture( HWND );

BOOL    WINAPI SetCaretBlinkTime( UINT );

BOOL    WINAPI SetCaretPos( int, int );

DWORD   WINAPI SetClassLong( HWND, int, LONG );

WORD    WINAPI SetClassWord( HWND, int, WORD );

HANDLE  WINAPI SetClipboardData( UINT, HANDLE );

HWND    WINAPI SetClipboardViewer( HWND );

BOOL    WINAPI SetCurrentDirectory(  LPCTSTR );

HCURSOR WINAPI SetCursor( HCURSOR );

BOOL    WINAPI SetCursorPos( int, int );

int       WINAPI SetDIBits( HDC, HBITMAP, UINT, UINT, const VOID *, const BITMAPINFO *, UINT );

int       WINAPI SetDIBitsToDevice( HDC, int, int, int, int, int, int, UINT, UINT, PVOID, PBITMAPINFO, UINT );

BOOL    WINAPI SetDlgItemInt( HWND, int, UINT, BOOL );

BOOL    WINAPI SetDlgItemText( HWND, int, LPCTSTR );

BOOL    WINAPI SetDoubleClickTime( UINT );

BOOL    WINAPI SetEndOfFile( HANDLE );

HENHMETAFILE WINAPI SetEnhMetaFileBits( UINT, const BYTE * );

BOOL    WINAPI SetEnvironmentVariable( LPCTSTR, LPCTSTR );

BOOL    WINAPI SetEvent( HANDLE );

BOOL    WINAPI SetFileAttributes( LPCTSTR, DWORD );

DWORD   WINAPI SetFilePointer( HANDLE, LONG, PLONG, DWORD );

BOOL    WINAPI SetFileTime( HANDLE, const FILETIME *, const FILETIME *, const FILETIME * );

HWND    WINAPI SetFocus( HWND );

BOOL    WINAPI SetForegroundWindow( HWND );

int       WINAPI SetGraphicsMode(HDC, int );

UINT    WINAPI SetHandleCount( UINT );

VOID    WINAPI SetLastError( DWORD );

BOOL    WINAPI SetLocaleInfo( LCID, LCTYPE, LPCSTR );

BOOL    WINAPI SetLocalTime( const SYSTEMTIME * );

int       WINAPI SetMapMode( HDC, int );

DWORD   WINAPI SetMapperFlags( HDC, DWORD );

BOOL    WINAPI SetMenu( HWND, HMENU );

BOOL    WINAPI SetMenuContextHelpId( HMENU, DWORD);

BOOL    WINAPI SetMenuItemBitmaps( HMENU, UINT, UINT, HBITMAP, HBITMAP );

HMETAFILE WINAPI SetMetaFileBitsEx( UINT, PBYTE );

BOOL    WINAPI SetMiterLimit( HDC, FLOAT, PFLOAT );

UINT    WINAPI SetPaletteEntries( HPALETTE, UINT, UINT, const PALETTEENTRY *);

HWND    WINAPI SetParent( HWND, HWND );

COLORREF
          WINAPI SetPixel( HDC, int, int, COLORREF );

int       WINAPI SetPolyFillMode( HDC, int );

BOOL    WINAPI SetPriorityClass( HANDLE, DWORD );

BOOL    WINAPI SetProp( HWND, LPCSTR, HANDLE );

BOOL    WINAPI SetRect( LPRECT, int, int, int, int );

BOOL    WINAPI SetRectEmpty( LPRECT );

BOOL    WINAPI SetRectRgn( HRGN, int, int, int, int );

int       WINAPI SetROP2( HDC, int );

int       WINAPI SetScrollPos( HWND, int, int, BOOL );

BOOL    WINAPI SetScrollRange( HWND, int, int, int, BOOL );

BOOL    WINAPI SetStdHandle( DWORD, HANDLE );

int       WINAPI SetStretchBltMode( HDC, int );

BOOL    WINAPI SetSysColors( int, const INT *, const COLORREF * );

BOOL    WINAPI SetSystemTime( const SYSTEMTIME * );

BOOL    WINAPI SetTimeZoneInformation( const TIME_ZONE_INFORMATION * );

UINT    WINAPI SetTextAlign( HDC, UINT );

int       WINAPI SetTextCharacterExtra( HDC, int );

COLORREF WINAPI SetTextColor( HDC, COLORREF );

BOOL    WINAPI SetTextJustification( HDC, int, int );

BOOL    WINAPI SetThreadPriority( HANDLE, int );

UINT    WINAPI SetTimer( HWND, UINT, UINT, TIMERPROC );

BOOL    WINAPI SetViewportExtEx( HDC, int, int, LPSIZE );

BOOL    WINAPI SetViewportOrgEx( HDC, int, int, LPPOINT );

BOOL    WINAPI SetVolumeLabel( LPCTSTR, LPCTSTR );

BOOL    WINAPI SetWindowExtEx( HDC, int, int, LPSIZE );

LONG    WINAPI SetWindowLong( HWND, int, LONG );

BOOL    WINAPI SetWindowOrgEx( HDC, int, int, LPPOINT );

BOOL    WINAPI SetWindowPlacement( HWND, const WINDOWPLACEMENT * );

BOOL    WINAPI SetWindowPos( HWND, HWND, int, int, int, int, UINT );

HHOOK   WINAPI SetWindowsHookEx( int, HOOKPROC, HINSTANCE, DWORD );

BOOL    WINAPI SetWindowText( HWND, LPCSTR );

WORD    WINAPI SetWindowWord( HWND, int, WORD );

HENHMETAFILE WINAPI SetWinMetaFileBits( UINT, const BYTE *, HDC, const METAFILEPICT * );

BOOL    WINAPI GdiComment(HDC, UINT, CONST BYTE *);

BOOL    WINAPI SetWorldTransform( HDC, const XFORM * );

HINSTANCE    WINAPI ShellExecute( HWND, LPCTSTR, LPCTSTR, LPTSTR, LPCTSTR, INT );

BOOL    WINAPI ShowCaret( HWND );

int       WINAPI ShowCursor( BOOL );

BOOL    WINAPI ShowOwnedPopups( HWND, BOOL );

BOOL    WINAPI ShowScrollBar( HWND, int, BOOL );

BOOL    WINAPI ShowWindow( HWND, int );

DWORD   WINAPI SizeofResource( HINSTANCE, HRSRC );

VOID    WINAPI Sleep( DWORD );

int       WINAPI StartDoc( HDC, PDOCINFO );

int       WINAPI StartPage( HDC );

int       WINAPI StretchDIBits( HDC, int, int, int, int, int, int, int, int, void *, PBITMAPINFO, UINT, DWORD );

BOOL    WINAPI StretchBlt( HDC, int, int, int, int, HDC, int, int, int, int,  DWORD );

BOOL    WINAPI StrokeAndFillPath( HDC );

BOOL    WINAPI StrokePath( HDC );

BOOL    WINAPI SubtractRect( LPRECT, const RECT *, const RECT *);

DWORD   WINAPI SuspendThread( HANDLE );

BOOL    WINAPI SwapMouseButton( BOOL );

BOOL    WINAPI SystemParametersInfo( UINT, UINT, PVOID, UINT );

BOOL    WINAPI SystemTimeToFileTime( const SYSTEMTIME *, PFILETIME );

BOOL    WINAPI SystemTimeToTzSpecificLocalTime( PTIME_ZONE_INFORMATION, PSYSTEMTIME, PSYSTEMTIME );

LONG    WINAPI TabbedTextOut( HDC, int, int, LPCSTR, int, int, int *, int );

BOOL    WINAPI TerminateProcess( HANDLE, UINT );

BOOL    WINAPI TerminateThread( HANDLE, DWORD );

BOOL    WINAPI TextOut( HDC, int, int, LPCSTR, int );

DWORD   WINAPI TlsAlloc( VOID );

BOOL    WINAPI TlsFree( DWORD );

LPVOID  WINAPI TlsGetValue( DWORD );

BOOL    WINAPI TlsSetValue( DWORD, LPVOID );

BOOL    WINAPI TrackPopupMenu( HMENU, UINT, int, int, int, HWND, const RECT * );

int       WINAPI TranslateAccelerator( HWND, HACCEL, PMSG );

BOOL    WINAPI TranslateMDISysAccel( HWND, PMSG );

BOOL    WINAPI TranslateMessage( const MSG * );

BOOL    WINAPI UnhookWindowsHookEx( HHOOK );

BOOL    WINAPI UnionRect( LPRECT, const RECT *, const RECT * );

BOOL    WINAPI UnlockFile( HANDLE, DWORD, DWORD, DWORD, DWORD );

BOOL    WINAPI UnpackDDElParam(UINT, LONG, PUINT, PUINT );

BOOL    WINAPI UnrealizeObject( HGDIOBJ );

BOOL    WINAPI UnregisterClass( LPCTSTR, HINSTANCE );

BOOL    WINAPI UpdateWindow( HWND );

BOOL    WINAPI ValidateRect( HWND, const RECT *);

BOOL    WINAPI ValidateRgn( HWND, HRGN );

LPVOID  WINAPI VirtualAlloc( LPVOID, DWORD, DWORD, DWORD );

BOOL    WINAPI VirtualFree( LPVOID, DWORD, DWORD );

DWORD   WINAPI VirtualQuery( LPVOID, PMEMORY_BASIC_INFORMATION, DWORD );

SHORT   WINAPI VkKeyScan( char keyScan);

DWORD   WINAPI WaitForInputIdle( HANDLE, DWORD );

DWORD   WINAPI WaitForMultipleObjects( DWORD, const HANDLE *, BOOL, DWORD );

DWORD   WINAPI WaitForSingleObject( HANDLE, DWORD );

BOOL    WINAPI WaitMessage( void);

int       WINAPI WideCharToMultiByte( UINT, DWORD, LPCWSTR, int, LPSTR, int, LPCSTR, LPBOOL );

BOOL    WINAPI WidenPath( HDC);

UINT    WINAPI WinExec( LPCSTR, UINT );

BOOL    WINAPI WinHelp( HWND, LPCSTR, UINT, DWORD );

HWND    WINAPI WindowFromPoint( POINT );

HWND    WINAPI WindowFromDC( HDC );

BOOL    WINAPI WriteFile( HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED );

BOOL    WINAPI WritePrivateProfileString( LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR );

BOOL    WINAPI WriteProfileString( LPCTSTR, LPCTSTR, LPCTSTR );

VOID    WINAPI ZeroMemory( PVOID, DWORD );

HFILE   WINAPI _lclose( HFILE );

HFILE   WINAPI _lcreat( LPCSTR, int );

HFILE   WINAPI _lopen( LPCSTR, int );

UINT    WINAPI _lread( HFILE, PVOID, UINT );

LONG    WINAPI _llseek( HFILE, LONG, int );

UINT    WINAPI _lwrite( HFILE, const VOID *, UINT );

MMRESULT WINAPI timeGetSystemTime( PMMTIME, UINT );

DWORD   WINAPI timeGetTime( VOID );

int       WINAPI wsprintf( LPSTR, LPCSTR, ... );

int       WINAPI wvsprintf( LPSTR, LPCSTR, const VOID *);

LPSTR   WINAPI CharNext( LPCSTR );

LPSTR   WINAPI CharPrev( LPCSTR, LPCSTR );

HWND    WINAPI GetDesktopWindow( VOID );

BOOL    WINAPI CharToOem( LPCSTR, LPSTR );

BOOL    WINAPI OemToChar( LPCSTR, LPSTR );

BOOL    WINAPI CharToOemBuff( LPCSTR, LPSTR, DWORD );

BOOL    WINAPI OemToCharBuff( LPCSTR, LPSTR, DWORD );

BOOL    WINAPI FloodFill(HDC, int, int, COLORREF );

BOOL    WINAPI IsCharAlpha( wchar_t );

BOOL    WINAPI IsCharAlphaNumeric( wchar_t );

BOOL    WINAPI IsCharLower( wchar_t );

BOOL    WINAPI IsCharUpper( wchar_t );

LPSTR   WINAPI lstrcat( LPSTR, LPCSTR );

int       WINAPI lstrcmp( LPCSTR, LPCSTR );

int       WINAPI lstrcmp2( LPCSTR, LPCSTR );

int       WINAPI lstrcmpi( LPCSTR, LPCSTR );

LPSTR   WINAPI lstrcpy( LPSTR, LPCSTR );

int       WINAPI lstrlen( LPCSTR );

int       WINAPI lstrcmpW( LPCWSTR, LPCWSTR );

/* -----------------------------------------------------------------*/


int       WINAPI WinCallWinMain(int, char *[], FNMAIN, int);

/* The following functions are used for translating data between the native
 * Presentation Manager format and the Developer API Extensions equivalents.
 */
typedef enum { WINX2PM, PM2WINX} XLATEDIR;

BOOL    WINAPI WinDeleteGraphicsObjectHandle( HGDIOBJ );
void      WINAPI WinInitializeOpen32( ULONG );
void      WINAPI WinTerminateOpen32( ULONG );
BOOL    WINAPI WinTranslateDevicePoints( HDC, HWND, PPOINT, INT, XLATEDIR );
BOOL    WINAPI WinTranslateDeviceRects ( HDC, HWND, PRECT,  INT, XLATEDIR );
ULONG   WINAPI WinTranslateGraphicsObjectHandle( HGDIOBJ, XLATEDIR, ULONG );
ULONG   WINAPI WinTranslateMnemonicString( LPCSTR, LPSTR *, ULONG, XLATEDIR );

DWORD   WINAPI WinQueryTranslateMode( VOID );
BOOL    WINAPI WinSetTranslateMode( DWORD );



/* Reset the packing mode to that specified at compile time via
 * the compiler switch.
 */
#pragma pack()

#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( restore )
   #endif
   #pragma info( restore )
#endif

#endif    /* ifdef _OS2WIN_H */

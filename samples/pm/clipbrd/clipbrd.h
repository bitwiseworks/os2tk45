/*********** CLIPBRD C Sample Program Include file (.H) *******************
 *
 *  The CLIPBRD include file defines symbolic constants used
 *  in the CLIPBRD.C and .RC files.
 *  CLIPBRD local procedure declarations may appear in this file to
 *  ensure they have been declared before being used.
 *
 *  NOTE: If using the Dialog Editor "DLGEDIT.EXE" ALL comments
 *          (including this) MUST be removed from this file.
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
 ******************************************************************************/

#define ERRORX   1
#define SUCCESSX 0

#define CCHMAXSTRING    200

#define AMENU           1
#define ATABLE          1
#define AICON           1

#define IDC_OK              1
#define IDC_BITMAP          5
#define IDR_BITMAP          6

#define ID_BITMAP       10

#define TERMINATE       11
#define HELPPANEL       12
#define HELPINDEX       13
#define EXTHELP         14
#define KEYSHELP        15
#define ABOUT1          16
#define ABOUT2          17
#define ABOUT3          18
#define ABOUT4          19
#define ABOUT5          20
#define GENHELP         21


#define SM_CLIPBRD      100
#define MENU_COPY       101
#define MENU_CUT        102
#define MENU_PASTE      103
#define MENU_CLEAR      104
#define SM_HELP         201
#define MENU_HELPINDEX  202
#define MENU_EXTHELP    203
#define MENU_KEYSHELP   204
#define MENU_PRODINFO   205
#define MENU_HELPUSING  206

#define IDD_PRODUCTINFO 300

int  main( VOID );
VOID ClipbrdHelp ( HWND, USHORT );
VOID ClearRoutine( VOID );
VOID CopyRoutine( VOID );
VOID CutRoutine( VOID );
VOID DrawRect( VOID );
VOID EraseRect( VOID );
VOID ExitRoutine( HWND );
VOID Finalize( VOID );
HBITMAP GetBitmap( VOID );
BOOL Initialize( VOID );
VOID MenuInitialization( MPARAM, MPARAM);
VOID MouseButtonDown( HWND, MPARAM);
VOID PaintRoutine( HWND );
VOID PasteRoutine( VOID );
VOID RemoveRect( VOID );
VOID Report_Error( HAB );
VOID WindowDestroy( VOID );
VOID WindowInitialization( HWND );
VOID ReportAPIError( PSZ );
MRESULT ProductInfoDlgProc( HWND, ULONG, MPARAM, MPARAM );
VOID  HelpProductInfo( HWND );
VOID SetSysMenu( HWND );
/***************************  End of clipbrd.h  **************************/

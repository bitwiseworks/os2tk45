/*static char *SCCSID = "@(#)img_xtrn.h	6.8 92/02/10";*/
/**************************************************************************
 *  File name  :  img_xtrn.h
 *
 *  Description:   This header file contains the application wide function
 *                 declarations and global external variables.
 *
 *  Concepts   :  function prototypes
 *
 *  API's      :  [none]
 *
 *  Required
 *    Files    :  [none]
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
/*
 *  Global variables
 */
extern HAB      vhab;                    /* anchor block handle */
extern HDC      vhdcMem;        /* memory device context handle */
extern HDC      vhdc;           /* window device context handle */
extern HPS      vhps;                        /* image PS handle */
extern HPS      vhpsMem;                    /* memory PS handle */
extern BOOL     vfImgLoaded;      /* initially image not loaded */
extern BOOL     vfDetail;        /* view non-detailed initially */
extern BOOL     vfMaximized;  /* initially window not maximized */
extern BOOL     vfHelpEnabled;    /* set when help installed ok */
extern HWND     vhwndFrame;        /* image window frame handle */
extern HWND     vhwndClient;       /* main window client handle */
extern HWND     vhwndVScroll;      /* vertical scrollbar handle */
extern HWND     vhwndHScroll;    /* horizontal scrollbar handle */
extern HWND     vhwndMenu;                   /* menu bar handle */
extern LONG     vlXScreen;                      /* screen width */
extern LONG     vlYScreen;                     /* screen height */
extern LONG     vlcxVScroll;         /* vertical scrollbar size */
extern LONG     vlcyHScroll;       /* horizontal scrollbar size */
extern HWND     vhwndViewForeClr;      /* foregrnd color handle */
extern HWND     vhwndViewBackClr;      /* backgrnd color handle */
extern LONG     vlcyTitle;                   /* titlebar height */
extern LONG     vlcyMenu;                     /* menubar height */
extern LONG     vlcxBorder;                /* size border width */
extern LONG     vlcyBorder;               /* size border height */
extern LONG     vlForeClr;          /* initial foreground color */
extern LONG     vlBackClr;          /* initial background color */

#if (defined(PORT_16) || defined(PORT_32))
   extern PBYTE vpbImgBuf;                 /* image storage buffer */
#elif PORT_S132
   extern BYTE _far16 *vpbImgBuf;          /* image storage buffer */
#else
                                 /* not defined for 16-bit port */
#endif

extern PFNWP    vpfnwpFrame;     /* main frame-window procedure */
extern SIZEL    vsizlImg;      /* width and height of the image */
extern ULONG    vulScrollYMax;        /* maximum Y scroll range */
extern ULONG    vulScrollXMax;        /* maximum X scroll range */
extern POINTL   vptlSave;              /* save/restore position */
extern HBITMAP  vhbm;                         /* bit-map handle */
extern HPOINTER vhptrWait;          /* system hourglass pointer */
extern HPOINTER vhptrArrow;             /* system arrow pointer */
#ifndef PORT_16
extern FILEDLG  vfdg;                  /* file dialog structure */
#endif
/*
 *  Entry point declarations
 */
/* from img_pnt.c */
VOID PaintUnSizedImage(VOID);
VOID PaintSizedImage(VOID);
VOID PaintDrawImage(VOID);

/* from img_init.c */
#ifdef PORT_16
USHORT Init(VOID);
#endif
#ifdef PORT_32
ULONG Init(VOID);
#endif
VOID  ExitProc(USHORT usTermCode);

/* from img_size.c */
VOID SizeMainWindow(VOID);
BOOL SizeCalculateMaxWindow(PRECTL prclBound);
VOID SizePositionImage(MPARAM mp2);
VOID SizeVScroll(MPARAM mp2, RECTL rcl, POINTL ptl);
VOID SizeHScroll(MPARAM mp2, RECTL rcl, POINTL ptl);

/* from img_main.c */
int main(void);
#ifdef PORT_16
ULONG MessageBox(HWND hwndOwner, USHORT idMsg, USHORT idCaption,
                 USHORT fsStyle, BOOL fBeep);
MRESULT EXPENTRY MainWndProc(HWND hwnd,  USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY FrameWndProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
#endif
#ifdef PORT_32
ULONG MessageBox(HWND hwndOwner, ULONG idMsg, ULONG idCaption,
                 ULONG fsStyle, BOOL fBeep);
MRESULT EXPENTRY MainWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY FrameWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
#endif

/* from img_dlg.c */
#ifdef PORT_16
MRESULT EXPENTRY ColorDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
#endif
#ifdef PORT_32
MRESULT EXPENTRY ColorDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
#endif
VOID             SetSysMenu(HWND hDlg);

/* from img_menu.c */
VOID MenuEnableItem(HWND hwndMenu, SHORT idItem, BOOL fEnable);
VOID  MenuInit(MPARAM mp1, MPARAM mp2);
SHORT MenuGetColorItemId(SHORT idBase);
VOID  MenuCheckItem(HWND hwndMenu, SHORT idItem, BOOL fCheck);

/* from img_view.c */
VOID ViewSwitchMode(VOID);
VOID ViewChangeColor(USHORT idColor);
VOID ViewSavePosition(VOID);
VOID ViewRestorePosition(VOID);

/* from img_file.c */
BOOL FileOpen(MPARAM mp2);

/* from img_help.c */
VOID HelpInit(VOID);
VOID HelpUsingHelp(VOID);
VOID HelpGeneral(VOID);
VOID HelpIndex(VOID);
VOID HelpProductInfo(VOID);
VOID HelpDestroyInstance(VOID);
VOID DisplayHelpPanel(ULONG);

/* from img_util.c */
#if (defined(PORT_16) || defined(PORT_32))
   LONG UtilFindFileSize(PSZ pszFilename);
   BOOL UtilMemoryAllocate(USHORT cbSize, PBYTE FAR *ppByte);
   BOOL UtilGetFileHandle(PSZ pszFilename, PHFILE phfile);
   VOID UtilUpdateTitleText(HAB hab, HWND hwnd, PSZ pszFullFile);
#endif

#ifdef PORT_S132
LONG _far16 pascal UtilFindFileSize(CHAR _far16 *pszFilename);
BOOL _far16 pascal UtilMemoryAllocate(USHORT cbSize, BYTE _far16 * _far16 *ppByte);
BOOL _far16 pascal UtilGetFileHandle(CHAR _far16 *pszFilename, HFILE _far16 *phfile);
VOID _far16 pascal UtilUpdateTitleText(HAB hab, HWND hwnd, CHAR _far16 *pszFullFile);
#endif

#ifdef PORT_16
   VOID UtilMemoryFree(SEL sel);
#elif PORT_S132
   VOID _far16 pascal UtilMemoryFree(SEL sel);
#elif PORT_32
   VOID UtilMemoryFree(PBYTE pByte);
#else
#endif
/***************************  End of img_xtrn.h  *************************/

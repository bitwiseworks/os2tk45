/**************************************************************************
 *  File name  :  img_data.c
 *
 *  Description:  This module contains global data declarations for the
 *                PM Porting Sample.
 *
 *  Concepts   :  global data
 *
 *  API's      :  [none]
 *
 *  Required
 *    Files    :  OS2.H
 *
 *  Copyright (C) 1991-1993 IBM Corporation
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

#define INCL_WINPOINTERS
#define INCL_WINSTDFILE
#include <os2.h>

/*
 * Global data declarations
 */

HAB      vhab;                              /* anchor block handle */
HDC      vhdcMem;                  /* memory device context handle */
HDC      vhdc;                     /* window device context handle */
HPS      vhps       = (HPS)NULLHANDLE;          /* image PS handle */
HPS      vhpsMem    = (HPS)NULLHANDLE;         /* memory PS handle */
BOOL     vfImgLoaded  = FALSE;       /* initially image not loaded */
BOOL     vfDetail     = FALSE;      /* view non-detailed initially */
BOOL     vfMaximized  = FALSE;   /* initially window not maximized */
BOOL     vfHelpEnabled = FALSE;      /* set when help installed ok */
HWND     vhwndFrame = (HWND)NULLHANDLE; /* img window frame handle */
HWND     vhwndClient;                 /* main window client handle */
HWND     vhwndVScroll;                /* vertical scrollbar handle */
HWND     vhwndHScroll;              /* horizontal scrollbar handle */
HWND     vhwndViewForeClr = (HWND)NULLHANDLE; /* foreground color  */
HWND     vhwndViewBackClr = (HWND)NULLHANDLE; /* background color  */
HWND     vhwndMenu;                             /* menu bar handle */
LONG     vlXScreen;                                /* screen width */
LONG     vlYScreen;                               /* screen height */
LONG     vlcxVScroll;                   /* vertical scrollbar size */
LONG     vlcyHScroll;                 /* horizontal scrollbar size */
LONG     vlcyTitle;                             /* titlebar height */
LONG     vlcyMenu;                               /* menubar height */
LONG     vlcxBorder;                          /* size border width */
LONG     vlcyBorder;                         /* size border height */
LONG     vlForeClr = CLR_WHITE;        /* initial foreground color */
LONG     vlBackClr = CLR_BLACK;        /* initial background color */
#if (defined(PORT_16) || defined(PORT_32))
PBYTE vpbImgBuf;                           /* image storage buffer */
#else
BYTE _far16 *vpbImgBuf;                    /* image storage buffer */
#endif
PFNWP    vpfnwpFrame;               /* main frame-window procedure */
SIZEL    vsizlImg;                /* width and height of the image */
ULONG    vulScrollYMax;                  /* maximum Y scroll range */
ULONG    vulScrollXMax;                  /* maximum X scroll range */
POINTL   vptlSave = {0L, 0L};             /* save/restore position */
HBITMAP  vhbm;                                   /* bit-map handle */
HPOINTER vhptrWait;                    /* system hourglass pointer */
HPOINTER vhptrArrow;                       /* system arrow pointer */
#ifndef PORT_16
FILEDLG  vfdg;                            /* file dialog structure */
#endif
/***************************  End of img_data.c  *************************/

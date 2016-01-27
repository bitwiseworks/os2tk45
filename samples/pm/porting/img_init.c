/**************************************************************************
 *  File name  :  img_init.c
 *
 *  Description:  This module contains the code for application
 *                initialization as well as the code for exit list
 *                processing.
 *                Functions contained here:
 *
 *                Init()
 *                InitMainWindow()
 *                InitClientArea(hwnd)
 *                InitGlobalVars()
 *
 *  Concepts   :  initialization, subclassing
 *
 *  API's      :  DosExitList
 *                DosExit
 *                WinRegisterClass
 *                WinLoadString
 *                WinCreateStdWindow
 *                WinWindowFromID
 *                WinSetParent
 *                WinSendMsg
 *                WinSubclassWindow
 *                WinOpenWindowDC
 *                GpiCreatePS
 *                GpiSetColor
 *                GpiSetBackColor
 *                GpiSetBackMix
 *                WinQuerySysValue
 *                WinQuerySysPointer
 *
 *  Required
 *    Files    :  OS2.H, IMG_MAIN.H, IMG_XTRN.H
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
 *  Include files, macros, defined constants, and externs
 */
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINSYS
#define INCL_WINPOINTERS
#define INCL_WINSTDFILE
#define INCL_GPIPRIMITIVES

#include <os2.h>

#include "img_main.h"
#include "img_xtrn.h"

/*
 *  Entry point declarations
 */
BOOL InitMainWindow(VOID);
BOOL InitGlobalVars(VOID);
BOOL InitClientArea(HWND hwnd);

/*
 *  static variables
 */
CHAR szAppName[CCHAPPNAME];                /* application title */
#define RETURN_ERROR        1        /* error return in DosExit */

/**************************************************************************
 *
 *  Name       : Init()
 *
 *  Description: Performs initialization functions.
 *
 *  Concepts   : Called once before the main window is created.
 *               - register all window classes
 *               - setup main application window
 *               - set global variables
 *
 *  API's      :  DosExitList
 *                DosExit
 *                WinRegisterClass
 *                WinLoadString
 *
 *  Parameters :  [none]
 *
 *  Return     :  NO_ERROR  - initiaization is successful
 *                String_ID - initialization failed
 *
 *************************************************************************/
#ifdef PORT_16
USHORT Init(VOID)
#endif
#ifdef PORT_32
ULONG Init(VOID)
#endif
{
    /* Add ExitProc to the exit list to handle the exit processing.  If
     * there is an error, then terminate the process since there have
     * not been any resources allocated yet
     */
   if (DosExitList(EXLST_ADD, (PFNEXITLIST)ExitProc))
   {
       MessageBox(HWND_DESKTOP,
                  IDMSG_CANNOTLOADEXITLIST, 0,
                  MB_OK | MB_ERROR,
                  TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }

   /* load application name from resource file */
   if(0==WinLoadString(vhab, (HMODULE)NULL, IDS_APPNAME, CCHAPPNAME, szAppName))
       return IDMSG_CANNOTLOADSTRING;

   /* register the main client window class */
   if (!WinRegisterClass(vhab,
                szAppName,
                (PFNWP)MainWndProc,
                0L,
                0))
       return IDMSG_INITFAILED;

   /*
    * create main application window & detach scrollbars
    */
   if (!InitMainWindow())
       return IDMSG_MAINWINCREATEFAILED;

   /*
    * set up globals used for sizing & system pointers
    */
   if (!InitGlobalVars())
      return IDMSG_INITFAILED;

   /*
    * this function prepares the application for loading images
    */
   InitClientArea(vhwndClient);

   /*
    * initialize help mechanism
    */
#ifdef HELP_MANAGER_ENABLED
   HelpInit();
#endif

   return NO_ERROR;
}  /* End of Init */

/**************************************************************************
 *
 *  Name       : InitMainWindow()
 *
 *  Description: Creates the application window and puts it into
 *               its initial state.
 *
 *  Concepts   : Called once by the Init() routine
 *               - create main application window
 *               - detach scrollbars from window
 *               - subclass frame window procedure
 *
 *  API's      :  WinCreateStdWindow
 *                WinWindowFromID
 *                WinSetParent
 *                WinSendMsg
 *                WinRegisterClass
 *                WinSubclassWindow
 *
 *  Parameters :  [none]
 *
 *  Return     :  TRUE - window successfully created
 *                FALSE - window creation failed
 *
 *************************************************************************/
BOOL InitMainWindow(VOID)
{
   ULONG ctlData = FCF_STANDARD | FCF_VERTSCROLL | FCF_HORZSCROLL;

   /*
    * Create a window with standard controls.
    */
   vhwndFrame = WinCreateStdWindow(
                         HWND_DESKTOP,
                         WS_VISIBLE,
                         (PULONG)&ctlData,
                         (PSZ)szAppName,
                         (PSZ)NULL,
                         WS_VISIBLE,
                         (HMODULE)NULL,
                         IDR_MAIN,
                         (PHWND)&vhwndClient);
   if (!vhwndFrame)
      return FALSE;

   /*
    * for the time being detach the scrollbars from the main
    * window - but remember their handles for later
    */
   vhwndVScroll = WinWindowFromID(vhwndFrame, FID_VERTSCROLL);
   vhwndHScroll = WinWindowFromID(vhwndFrame, FID_HORZSCROLL);
   WinSetParent(vhwndVScroll, HWND_OBJECT, FALSE);
   WinSetParent(vhwndHScroll, HWND_OBJECT, FALSE);
   WinSendMsg(vhwndFrame, WM_UPDATEFRAME,
              MPFROMLONG(FCF_VERTSCROLL | FCF_HORZSCROLL), (MPARAM)NULL);

   /* save menubar handle */
   vhwndMenu = WinWindowFromID(vhwndFrame, FID_MENU);

   /*
    * the frame window procedure is subclassed, so that frame-sizing
    * restrictions can be implemented.
    */
   if (!WinRegisterClass(vhab, "SUBFRAME", (PFNWP)FrameWndProc, 0L, 0))
      return FALSE;

   vpfnwpFrame = WinSubclassWindow(vhwndFrame, (PFNWP)FrameWndProc);

   return TRUE;
} /* End of InitMainWindow() */

/**************************************************************************
 *
 *  Name       : InitClientArea()
 *
 *  Description: Prepares the client area to accept the images
 *
 *  Concepts   : Called once by the Init() routine
 *               - obtain a window device context
 *               - define the image presentation space
 *               - associate the two
 *               - set foreground/background colours &
 *                 background mix for the presentation space
 *
 *  API's      :  WinOpenWindowDC
 *                GpiCreatePS
 *                GpiSetColor
 *                GpiSetBackColor
 *                GpiSetBackMix
 *
 *  Parameters :  hwnd = client window handle
 *
 *  Return     :  TRUE - client area successfully set up
 *                FALSE - client area setup failed
 *
 *************************************************************************/
BOOL InitClientArea(HWND hwnd)
{
   SIZEL  sizl;

   sizl.cx = 0L;                /* set size to default for device    */
   sizl.cy = 0L;                /*  (full screen)                    */

   vhdc = WinOpenWindowDC(hwnd);
   if (!vhdc)
       return FALSE;

   vhps = GpiCreatePS(vhab,
                      vhdc,
                      &sizl,
                      (ULONG)PU_PELS | GPIT_NORMAL | GPIA_ASSOC
                      );
   if (!vhps)
       return FALSE;

   GpiSetColor(vhps, vlForeClr);
   GpiSetBackColor(vhps, vlBackClr);
   GpiSetBackMix(vhps, BM_OVERPAINT);

   return TRUE;
}   /* End of InitClientArea */

/**************************************************************************
 *
 *  Name       : InitGlobalVars()
 *
 *  Description: Performs initialization of the application  s
 *               global variables
 *
 *  Concepts   : Called once by the Init() routine
 *
 *  API's      :  WinQuerySysValue
 *                WinQuerySysPointer
 *
 *  Parameters :  [none]
 *
 *  Return     :  TRUE - initialization is successful
 *                FALSE - initialization failed
 *
 *************************************************************************/
BOOL InitGlobalVars(VOID)
{
   /* load system sizes */
   vlXScreen = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
   vlYScreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
   vlcxVScroll = WinQuerySysValue(HWND_DESKTOP, SV_CXVSCROLL);
   vlcyHScroll = WinQuerySysValue(HWND_DESKTOP, SV_CYHSCROLL);
   vlcyTitle = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
   vlcyMenu = WinQuerySysValue(HWND_DESKTOP, SV_CYMENU);
   vlcxBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   vlcyBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);

   /* load system pointers */
   vhptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE);
   vhptrWait = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
   return TRUE;
} /* End of InitGlobalVars  */
/***************************  End of img_init.c  *************************/

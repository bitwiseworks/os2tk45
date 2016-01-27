/**************************************************************************
 *  File name  :  img_main.c
 *
 *  Description:  This application serves as an example of porting a 16-bit
 *                application to 32-bit.  It modifies a 16-bit application
 *                which was built using the headers/libraries/tools
 *                from the OS/2 1.3 toolkit and produces a pure 32-bit
 *                application.
 *
 *                This source file contains the following functions:
 *
 *                main()
 *                MainWndProc(hwnd, msg, mp1, mp2)
 *                FrameWndProc(hwnd, msg, mp1, mp2)
 *                MessageBox(hwnd idMsg, fsStyle, fBeep)
 *                MainCommand(mp1, mp2)
 *                ExitProc(usTermCode)
 *
 *  Concepts   :  porting
 *
 *  API's      :  WinInitialize
 *                DosBeep
 *                WinCreateMsgQueue
 *                WinGetMsg
 *                WinTerminate
 *                WinDispatchMsg
 *                WinBeginPaint
 *                WinFillRect
 *                WinEndPaint
 *                WinInvalidateRect
 *                GpiAssociate
 *                GpiDestroyPS
 *                GpiSetBitmap
 *                GpiDeleteBitmap
 *                GpiDestroyPS
 *                DevCloseDC
 *                WinIsChild
 *                WinSetParent
 *                WinPostMsg
 *                WinQueryWindowRect
 *                GpiQueryCurrentPosition
 *                WinMapWindowPoints
 *                WinCalcFrameRect
 *                WinLoadMessage
 *                WinAlarm
 *                WinIsWindow
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *                DosExitList
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDIO.H, IMG_MAIN.H, IMG_XTRN.H,
 *                IMG_HELP.H
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
#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINSYS
#define INCL_WINTRACKRECT
#define INCL_WINHELP
#define INCL_WINSTDFILE
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS

/*
 *  Include files, macros, defined constants, and externs
 */
#include <os2.h>
#include <string.h>
#include <stdio.h>

#include "img_main.h"
#include "img_xtrn.h"
#include "img_help.h"

/*
 *  error defines
 */
#define RETURN_SUCCESS       0  /* successful return in DosExit */
#define RETURN_ERROR         1  /* error return in DosExit      */
#define BEEP_WARN_FREQ      60  /* frequency of warning beep    */
#define BEEP_WARN_DUR      100  /* duration of warning beep     */

/*
 *  Entry point declarations
 */
VOID MainCommand(MPARAM mp1, MPARAM mp2);

/*
 *  Static variables
 */
HMQ    hmq;           /* application queue handle */

/**************************************************************************
 *
 *  Name       : main()
 *
 *  Description: Initializes the PM environment, calls the
 *               initialization routine, creates the main
 *               window,  and polls the message queue
 *
 *  Concepts   : - obtains anchor block handle and creates message
 *                   queue
 *               - calls the initialization routine
 *               - creates the main frame window which creates the
 *                   main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  API's      :  WinInitialize
 *                DosBeep
 *                WinCreateMsgQueue
 *                WinGetMsg
 *                WinTerminate
 *                WinDispatchMsg
 *
 *  Parameters :  [none]
 *
 *  Return     :  1 - if successful execution completed
 *                0 - if error
 *
 *************************************************************************/
int main(void)
{
   QMSG   qmsg;          /* message structure */
#ifdef PORT_16
   USHORT rc;
#endif
#ifdef PORT_32
   ULONG  rc;
#endif

   vhab = WinInitialize(0);
   if(!vhab)
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       return RETURN_ERROR;
   }

   /* find a define for this v */
   hmq = WinCreateMsgQueue(vhab, 0);
   if(!hmq)
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       WinTerminate(vhab);
       return RETURN_ERROR;
   }

   if(rc = Init())
   {
       MessageBox(HWND_DESKTOP, rc, 0, MB_OK | MB_ERROR, TRUE);
   }
   else
   {
       /* get/dispatch message loop */
       while (WinGetMsg(vhab, &qmsg, (HWND)NULL, 0, 0))
          WinDispatchMsg(vhab, &qmsg);
   }

  /*
   * release any allocated resource - no need to check return codes
   */
#ifdef HELP_MANAGER_ENABLED
    /* destroy the help instance */
   HelpDestroyInstance();
#endif
   return RETURN_SUCCESS;
}   /* End of main */

/**************************************************************************
 *
 *  Name       : MainWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Processes the messages sent to the main client
 *               window.  This routine processes the basic
 *               messages all client windows should process
 *               and passes all others onto UserWndProc where
 *               the developer can process any others.
 *
 *  Concepts   : Called for each message placed in the main
 *               window's message queue
 *
 *               A switch statement branches to the routines to be
 *               performed for each message processed.  Any messages
 *               not specifically process are passed to the user's
 *               message processing procedure UserWndProc().
 *
 *  API's      :  WinBeginPaint
 *                WinFillRect
 *                WinEndPaint
 *                WinInvalidateRect
 *                GpiQueryCurrentPosition
 *                WinQueryWindowRect
 *                GpiAssociate
 *                GpiDestroyPS
 *                GpiSetBitmap
 *                GpiDeleteBitmap
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *                WinIsChild
 *                WinSetParent
 *                WinPostMsg
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  values are determined by each message
 *
 *************************************************************************/
MRESULT EXPENTRY MainWndProc(
             HWND hwnd,      /* handle of window */
#ifdef PORT_16
             USHORT  msg,     /* id of message */
#endif
#ifdef PORT_32
             ULONG  msg,     /* id of message */
#endif
             MPARAM mp1,     /* first message parameter */
             MPARAM mp2)     /* second message parameter */
{
   PSWP    pswp;
   BOOL    fHScroll;
   RECTL   rcl;
   POINTL  ptl;

   switch (msg)
   {
       case WM_CREATE:
          /*
           * set application title to 'Untitled'
           */
           UtilUpdateTitleText(vhab,
                               ((PCREATESTRUCT)PVOIDFROMMP(mp2))->hwndParent,
                               "");

           /* return FALSE to continue window creation, TRUE to abort it */
           return (MRESULT)FALSE;
           break;

       case WM_INITMENU:

          /*
           * disable/enable menu items as appropriate
           */
           MenuInit(mp1, mp2);
           break;

       case WM_ERASEBACKGROUND:

          /*
           * The client window is cleared to SYSCLR_WINDOW
           */
           return (MRESULT)TRUE;
           break;

       case WM_MINMAXFRAME:

          /*
           * the window can be maximized by double clicking on the
           * title bar, clicking on the maximize icon, or by
           * selecting the maximize option in the System Menu.
           * The SizeCalculateMaxWindow() function is invoked to set the
           * window to a size that is no larger than the loaded image.
           */
           pswp = (PSWP)mp1;
#if (defined(PORT_S132) || defined(PORT_32))
           if (pswp->fl & SWP_MAXIMIZE) {
#else
           if (pswp->fs & SWP_MAXIMIZE) {
#endif

              /*
               * disable options which aren't valid when
               * the window is maximized
               */
               vfMaximized = TRUE;

               if (vfDetail)
               {
                   SizeCalculateMaxWindow(&rcl);
                   pswp->cy = (SHORT)(rcl.yTop - rcl.yBottom);
                   pswp->cx = (SHORT)(rcl.xRight - rcl.xLeft);
                   pswp->y  = (SHORT)rcl.yBottom;
                   pswp->x  = (SHORT)rcl.xLeft;
               }
           }
           else

              /*
               * re-enable options which were invalid when
               * the window was maximized
               */
               vfMaximized = FALSE;
           break;

       case WM_PAINT:

           WinBeginPaint(hwnd, vhps, (PRECTL)&rcl);
           if (vfImgLoaded && vfDetail)

              /*
               * If an image has been loaded and the user has selected
               * the 'Detail' view, the PaintUnSizedImage() function is
               * invoked to draw the image data in the client window.
               * The image is drawn unscaled, and clipped if necessary.
               */
               PaintUnSizedImage();

           else if (vfImgLoaded && !vfDetail)

              /*
               * If an image has been loaded and the user has selected
               * the 'Non-detailed' view, the PaintSizedImage() function is
               * invoked to size the image and draw it.
               * The image is drawn scaled to fit the window.
               */
               PaintSizedImage();

           else
              /*
               * If no image data has been loaded, clear the window to
               * the default system color.
               */
               WinFillRect(vhps, &rcl, SYSCLR_WINDOW);

           WinEndPaint(vhps);
           break;

       case WM_SIZE:

          /*
           * If the window size is altered and the image is being
           * viewed in detail, the WndSize function is invoked to restrict
           * the new window size and scroll-bar range to image limits.
           * If the 'Non-detailed' view has been selected and the window
           * size is altered, the image has to be redrawn to fit the window.
           */
           if (vfImgLoaded && vfDetail)
               SizePositionImage(mp2);

           WinInvalidateRect(hwnd, (PRECTL)NULL, FALSE);
           break;

       case WM_HSCROLL:
       case WM_VSCROLL:

          /*
           * scrolling is implemented only if an image has been
           * loaded, the 'Detail' view has been selected, and there is
           * image data still lying outside window limits.
           */
           fHScroll = (BOOL)(msg == WM_HSCROLL);
           if (vfImgLoaded && vfDetail &&
               (fHScroll ? vulScrollXMax > 0 : vulScrollYMax > 0))
           {
              GpiQueryCurrentPosition(vhps, &ptl);
              WinQueryWindowRect(hwnd, &rcl);

               /* call the appropriate scroll routine */
              (fHScroll ? SizeHScroll(mp2, rcl, ptl) :
                          SizeVScroll(mp2, rcl, ptl));
           }
           break;

       case WM_DESTROY:

          /*
           * delete the image graphics presentation space
           */
           GpiAssociate(vhps, (HDC)NULL);
           GpiDestroyPS(vhps);

           if (vfImgLoaded)
           {
              /*
               * Free any allocated memory
               */
#if (defined(PORT_16) || defined(PORT_S132))
               UtilMemoryFree(SELECTOROF(vpbImgBuf));
#else
               UtilMemoryFree(vpbImgBuf);
#endif

              /*
               * Release all bit-map resources and close the memory
               * device context
               */
               GpiSetBitmap(vhpsMem, (HBITMAP)NULL);
               GpiDeleteBitmap(vhbm);
               GpiAssociate(vhpsMem, (HDC)NULL);
               GpiDestroyPS(vhpsMem);
               DevCloseDC(vhdcMem);
           }

          /*
           * ensure that the scroll bars are child windows of the
           * frame, so that they will be destroyed automatically.
           */
           if (!WinIsChild(vhwndHScroll, vhwndFrame))
           {
               WinSetParent(vhwndVScroll, vhwndFrame, FALSE);
               WinSetParent(vhwndHScroll, vhwndFrame, FALSE);
           }
           break;

       case WM_CLOSE:
           WinPostMsg(hwnd, WM_QUIT, (MPARAM)NULL, (MPARAM)NULL);
           break;

       case WM_COMMAND:

          /*
           * processing of menu options is required
           */
           MainCommand(mp1, mp2);
           break;

#ifdef HELP_MANAGER_ENABLED

       case HM_ERROR:
          /*
           * an error has occurred while using IPF help - report
           * the error and destroy the help instance
           */
           MessageBox(vhwndFrame, IDMSG_HELPMANAGERERROR, 0,
                      MB_OK | MB_ERROR, TRUE);
           vfHelpEnabled = FALSE;
           break;
#endif
       default:

           /*
            * default must call WinDefWindowProc()
            */
          return WinDefWindowProc(hwnd, msg, mp1, mp2);
          break;
   }
   return (MRESULT)NULL; /* window procedures should return 0 as a default */
}   /* End of MainWndProc */

/**************************************************************************
 *
 *  Name       : FrameWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description: The purpose of the frame-window subclass procedure
 *               is to restrict frame-window sizing so that it is
 *               in step with the size of the client-window
 *               presentation space.  The size of the image
 *               determines the size of the maximized window.
 *               Messages intended for the frame-window procedure
 *               are sent here first.
 *
 *  Concepts   :  Called for each message placed in the main
 *                window's message queue
 *
 *               A switch statement branches to the routines to be
 *               performed for each message processed.  Any messages
 *               not specifically processed are passed to the user's
 *               message processing procedure MainWndProc(). The
 *               message trapped by this wndproc() are
 *               WM_QUERYTRACKINFO & WM_ADJUSTWINDOWPOS which are
 *               both posted to the frame procedure initially to
 *               set up the default values.
 *
 *  API's      :  WinQueryWindowRect(vhwn
 *                GpiQueryCurrentPosition
 *                WinMapWindowPoints
 *                WinCalcFrameRect
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  values are determined by each message
 *
 *************************************************************************/
MRESULT EXPENTRY FrameWndProc(
             HWND hwnd,      /* handle of window */
#ifdef PORT_16
             USHORT msg,     /* id of message (16-bit)  */
#endif
#ifdef PORT_32
             ULONG  msg,     /* id of message (32-bit)   */
#endif
             MPARAM mp1,     /* first message parameter */
             MPARAM mp2)     /* second message parameter */
{
   SWP        swp;
   PSWP       pswp;
   RECTL      rcl;
   ULONG      rc;
   POINTL     ptl;
   PTRACKINFO ptrack;

   switch(msg)
   {
       case WM_ADJUSTWINDOWPOS:

          /*
           * restrict the size of the window so that it never exceeds
           * the maximum size of the image - only handle explicitly
           * for the 'Detail' mode
           */
           if (vfImgLoaded && vfDetail)
           {
               rc = (ULONG)(*vpfnwpFrame)(hwnd, msg, mp1, mp2);
               pswp = (PSWP)mp1;
               swp = *pswp;

              /*
               * Find the maximum allowed size, and ensure that
               * the window size never exceeds this.
               */
               SizeCalculateMaxWindow(&rcl);
               if (swp.cx > (SHORT)rcl.xRight)
                   swp.cx = (SHORT)rcl.xRight;
               if (swp.cy > (SHORT)rcl.yTop)
                   swp.cy = (SHORT)rcl.yTop;
               *pswp = swp;
               return (MRESULT)rc;
           }
           else

               /* let the system handle the message */
               return (*vpfnwpFrame)(hwnd, msg, mp1, mp2);

           break;

       case WM_QUERYTRACKINFO:

          /*
           * Invoke the normal frame-window procedure first in order
           * to update the tracking rectangle to the new position.
           */
           (*vpfnwpFrame)(hwnd, msg, mp1, mp2);
           ptrack = (PTRACKINFO)mp2;

           if (vfImgLoaded && vfDetail)
           {
              /*
               * Limit the size of the bounding rectangle only if the
               * window is being sized.
               * fs and rclBoundary are tracking information structure
               * elements.
               */
               if (((ptrack->fs & TF_MOVE) != TF_MOVE) &&
                   ((ptrack->fs & TF_MOVE)             ||
                    (ptrack->fs & TF_SETPOINTERPOS  )))
               {
                   WinQueryWindowRect(vhwndClient, &rcl);
                   GpiQueryCurrentPosition(vhps, &ptl);

                   ptrack->rclBoundary.yTop    = rcl.yBottom + ptl.y;
                   ptrack->rclBoundary.yBottom = rcl.yBottom + ptl.y -
                                                 vsizlImg.cy;
                   ptrack->rclBoundary.xLeft   = rcl.xLeft + ptl.x;
                   ptrack->rclBoundary.xRight  = rcl.xLeft + ptl.x +
                                                 vsizlImg.cx;

                  /*
                   * Convert client boundary coordinates to screen
                   * coordinates.
                   */
                   WinMapWindowPoints(vhwndClient,
                                      HWND_DESKTOP,
                                      (PPOINTL)&ptrack->rclBoundary,
                                      sizeof(RECTL)/sizeof(POINTL));

                  /*
                   * Calculate equivalent frame boundary from client
                   * boundary data.
                   */
                   WinCalcFrameRect(vhwndFrame,
                                    (PRECTL)&ptrack->rclBoundary,
                                    FALSE);

                   ptrack->fs |= TF_ALLINBOUNDARY;
               }
           }
           return (MRESULT)TRUE;
       break;

   default:

      /*
       * return using the normal frame window procedure
       */
       return (*vpfnwpFrame)(hwnd, msg, mp1, mp2);
       break;

   }
   return (MRESULT)NULL; /* window procedures should return 0 as a default */
}   /* End of FrameWndProc */

/**************************************************************************
 *
 *  Name       : MessageBox(hwndOwner, idMsg, idCaption, fsStyle, fBeep)
 *
 *  Description: Displays the message box with the message
 *               given in idMsg retrived from the message table
 *               and using the style flags in fsStyle
 *
 *  Concepts   : Called whenever a MessageBox is to be displayed
 *               - Message string is loaded from the process'
 *                   message table
 *               - Alarm beep is sounded if desired
 *               - Message box with the message is displayed
 *               - WinMessageBox return value is returned
 *
 *  API's      :  WinLoadMessage
 *                WinAlarm
 *                WinMessageBox
 *
 *  Parameters :  hwnd      =  handle of the owner of the message box
 *                idMsg     =  i.d. of the message in the message table
 *                idCaption =  i.d. of the caption in the message table
 *                fsStyle   =  style of the message box
 *                fBeep     =  whether to beep before displaying the box
 *
 *  Return     :  the values from WinMessageBox()
 *
 *************************************************************************/
ULONG MessageBox(
         HWND hwndOwner,     /* handle of the message box's owner */
#ifdef PORT_16
         USHORT idMsg,        /* id of the message in the message table */
         USHORT idCaption,    /* id of the caption in the message table */
         USHORT fsStyle,      /* style of the message box */
#endif
#ifdef PORT_32
         ULONG idMsg,        /* id of the message in the message table */
         ULONG idCaption,    /* id of the caption in the message table */
         ULONG fsStyle,      /* style of the message box */
#endif
         BOOL fBeep)         /* if TRUE, beep before message box is displayed */
{
   CHAR szText[MESSAGELEN];
   CHAR szCaption[MESSAGELEN];

   if (!WinLoadMessage(vhab,
                       (HMODULE)NULL,
                       idMsg,
                       MESSAGELEN,
                       (PSZ)szText))
   {
       WinAlarm(HWND_DESKTOP, WA_ERROR);
       return MBID_ERROR;
   }

  /*
   * if idCaption specified load string else use default
   * caption 'Error!'
   */
   if (idCaption)
   {
       if (!WinLoadMessage(vhab,
                           (HMODULE)NULL,
                           idCaption,
                           MESSAGELEN,
                          (PSZ)szCaption))
       {
           WinAlarm(HWND_DESKTOP, WA_ERROR);
           return MBID_ERROR;
       }
   }

   if (fBeep)
       WinAlarm(HWND_DESKTOP, WA_ERROR);

   return WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        (idCaption ? szCaption : (PSZ)NULL),
                        MSGBOXID,
                        fsStyle);
}   /* End of MessageBox */

/**************************************************************************
 *
 *  Name       : MainCommand(mp1, mp2)
 *
 *  Description: Calls the appropriate procedures that deal with
 *               the selected menu item.
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window.
 *
 *               A switch statement branches on the id of the
 *               menu item that posted the message and the
 *               appropriate action for that item is taken.  Any
 *               menu ids that are not part of the standard menu
 *               set are passed onto the user defined WM_COMMAND
 *               processing procedure.
 *
 *  API's      :  [none]
 *
 *  Parameters :  mp1       =  first message parameter
 *                mp2       =  second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID MainCommand(MPARAM mp1, MPARAM mp2)
{
   switch (SHORT1FROMMP(mp1))
   {
       case IDM_FILEOPEN:

          /*
           * Open file and load/draw the selected image
           */
           FileOpen(mp2);
           break;

       case IDM_VIEWDETAIL:

          /*
           * toggle between Detail and Non-Detail mode
           */
           ViewSwitchMode();
           break;

       case IDM_VIEWSAVEPOSITION:

          /*
           * save the current image position
           */
           ViewSavePosition();
           break;

       case IDM_VIEWRESTOREPOSITION:

          /*
           * save the current image position
           */
           ViewRestorePosition();
           break;

       case IDM_VIEWFORECOLORBLACK:
       case IDM_VIEWFORECOLORWHITE:
       case IDM_VIEWFORECOLORBLUE:
       case IDM_VIEWFORECOLORGREEN:
       case IDM_VIEWFORECOLORYELLOW:
       case IDM_VIEWFORECOLORRED:
       case IDM_VIEWBACKCOLORBLACK:
       case IDM_VIEWBACKCOLORWHITE:
       case IDM_VIEWBACKCOLORBLUE:
       case IDM_VIEWBACKCOLORGREEN:
       case IDM_VIEWBACKCOLORYELLOW:
       case IDM_VIEWBACKCOLORRED:
           ViewChangeColor(SHORT1FROMMP(mp1));
           break;

#ifdef HELP_MANAGER_ENABLED
       case IDM_HELPUSINGHELP:
           HelpUsingHelp();
           break;

       case IDM_HELPGENERAL:
           HelpGeneral();
           break;

       case IDM_HELPINDEX:
           HelpIndex();
           break;
#endif
       case IDM_HELPPRODUCTINFO:
           HelpProductInfo();
           break;
    }
}   /* End of MainCommand */

/**************************************************************************
 *
 *  Name       : ExitProc(usTermCode)
 *
 *  Description: Cleans up certain resources when the application ends
 *
 *  Concepts   : Routine is called by DosExitList when the
 *               application exits
 *
 *               Global resources, such as the main window and
 *               message queue, are destroyed and any system
 *               resources used are freed.
 *
 *  API's      : WinIsWindow
 *               WinDestroyWindow
 *               WinDestroyMsgQueue
 *               WinTerminate
 *               DosExitList
 *
 *  Parameters : usTermCode = the termination code
 *
 *  Return     : EXLST_EXIT to the DosExitList handler
 *
 *************************************************************************/
VOID  ExitProc(USHORT usTermCode)
{
   /* destroy the main window if it exists */
   if (WinIsWindow(vhab, vhwndFrame))
       WinDestroyWindow(vhwndFrame);

   WinDestroyMsgQueue(hmq);
   WinTerminate(vhab);

   DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);   /* termination complete */

   /* This routine currently doesn't use the usTermCode parameter so
    *  it is referenced here to prevent an 'Unreferenced Parameter'
    *  warning at compile time
    */
   usTermCode;

}   /* End of ExitProc */
/***************************  End of img_main.c  *************************/

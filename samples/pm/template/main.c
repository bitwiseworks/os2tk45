/**************************************************************************
 *  File name  :  main.c
 *
 *  Description:  This application serves as a template than can be
 *                modified by an application developer.  The source
 *                files are organized so that the overhead code, which
 *                should be in all applications, is located in the same
 *                files, so that these files do not need to be modified.
 *                The routines that deal with application-specific code
 *                are also locatedin their own modules.  An application
 *                developer need only change these files in order to
 *                modify this template for his application.
 *
 *                This source file contains the following functions:
 *
 *                main()
 *                MainWndProc(hwnd, msg, mp1, mp2)
 *                MessageBox(hwnd idMsg, fsStyle, fBeep)
 *                MainCommand(mp1, mp2)
 *
 *  Concepts   :  Presentation Manager
 *
 *  API's      :  WinInitialize
 *                DosBeep
 *                WinCreateMsgQueue
 *                WinTerminate
 *                WinCreateStdWindow
 *                WinOpenWindowDC
 *                WinGetMsg
 *                WinDispatchMsg
 *                WinPostMsg
 *                WinLoadMessage
 *                WinAlarm
 *                WinMessageBox
 *
 *    Files    :  OS2.H, MAIN.H, HELP.H, XTRN.H
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

#define  INCL_WINHELP

#include <os2.h>
#include "main.h"
#include "help.h"
#include "xtrn.h"


/*
 *  Global variables
 */
HWND hwndMainFrame = NULLHANDLE;    /* handle to the main frame window */
HWND hwndMain;                      /* handle to the main client window */
HDC  hdcMain;                       /* handle to the DC of the client */
HAB  hab;                           /* anchor block for the process */
HMQ  hmq;                           /* handle to the process' message queue */
CHAR szAppName[MAXNAMEL];           /* buffer for application name string */
CHAR szUntitled[MESSAGELEN];        /* buffer for "Untitled" string */
BOOL fPrintEnabled;                 /* flag to determine if we can print */
BOOL fHelpEnabled;                  /* flag to determine if help is enabled */

/*
 *  Entry point declarations
 */

/**************************************************************************
 *
 *  Name       : main()
 *
 *  Description: Initializes the PM environment, calls the
 *               initialization routine, creates the main
 *               window,  and polls the message queue
 *
 *  Concepts:    - obtains anchor block handle and creates message
 *                    queue
 *               - calls the initialization routine
 *               - creates the main frame window which creates the
 *                    main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  API's      :  WinInitialize
 *                DosBeep
 *                WinCreateMsgQueue
 *                WinTerminate
 *                WinCreateStdWindow
 *                WinOpenWindowDC
 *                WinGetMsg
 *                WinDispatchMsg
 *
 *  Parameters :  [none]
 *
 *  Return     :  0 - if successful execution completed
 *                1 - if error
 *
 *************************************************************************/
int main(VOID)
{
   QMSG qmsg;          /* message structure */
   ULONG flCtlData;    /* frame control data */

   hab = WinInitialize(0);

   if(NULLHANDLE == hab)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return(RETURN_ERROR);
   }

   hmq = WinCreateMsgQueue(hab, 0);

   if(NULLHANDLE == hmq)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }

   if(!Init())
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_INITFAILED,
                 MB_OK | MB_ERROR,
                 TRUE);
      return(RETURN_ERROR);
   }

   /* NOTE:  clean up from here is handled by the DosExitList processing */


   /* create the main window */
   flCtlData = FCF_STANDARD;

   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      &flCtlData,
                                      (PSZ)szAppName,
                                      (PSZ)NULL,
                                      WS_VISIBLE,
                                      (HMODULE)NULL,
                                      IDR_MAIN,
                                      (PHWND)&hwndMain);

   if(NULLHANDLE == hwndMainFrame)
   {
       MessageBox(HWND_DESKTOP,
                  IDMSG_MAINWINCREATEFAILED,
                  MB_OK | MB_ERROR,
                  TRUE);
       return(RETURN_ERROR);
   }

   hdcMain = WinOpenWindowDC(hwndMain);

   InitHelp();

   /* Get/Dispatch Message loop */
   while(WinGetMsg(hab, (PQMSG)&qmsg, (HWND)NULL, (ULONG)NULL, (ULONG)NULL))
       WinDispatchMsg(hab, (PQMSG)&qmsg);

   /* destroy the help instance */
   DestroyHelpInstance();

#ifdef BACKGROUND_THREAD
    /* see main.h for comment on using a background thread */
   DestroyBackgroundThread();
#endif

   return(RETURN_SUCCESS);
}   /* End of main   */


/**************************************************************************
 *
 *  Name       : MainWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  Processes the messages sent to the main client
 *                window.  This routine processes the basic
 *                messages all client windows should process
 *                and passes all others onto UserWndProc where
 *                the developer can process any others.
 *
 *  Concepts:     Called for each message placed in the main
 *                window's message queue
 *
 *                A switch statement branches to the routines to be
 *                performed for each message processed.  Any messages
 *                not specifically process are passed to the user's
 *                message processing procedure UserWndProc().
 *
 *  API's      :  WinPostMsg
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message i.d.
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  value is determined by each message
 *
 *************************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
   case WM_CREATE:
      return(InitMainWindow(hwnd, mp1, mp2));
      break;

   case WM_PAINT:
       MainPaint(hwnd);
       break;

   case WM_COMMAND:
       MainCommand(mp1, mp2);
       break;

   case WM_INITMENU:
       InitMenu(mp1, mp2);
       break;

   case HM_QUERY_KEYS_HELP:
       return (MRESULT)PANEL_HELPKEYS;   /* return id of key help panel */
       break;

   case TM_THREADINITFAILED:
       /*  The message is received if the background thread
        *  initialization fails.
        *  A message box is displayed and the application is
        *  terminated.
        */
       MessageBox(HWND_DESKTOP,
                  IDMSG_INITFAILED,
                  MB_OK | MB_ERROR,
                  TRUE);

       WinPostMsg(hwnd, WM_CLOSE, NULL, NULL);
       break;

   /*
    *      Any messages not processed are passed on
    *      to the user's window proc.  It is
    *      responsible for passing any messages it
    *      doesn't handle onto WinDefWindowProc()
    */

   default:
      return UserWndProc(hwnd, msg, mp1, mp2);
      break;

   }
   return (MRESULT)NULL;  /* all window procedures return 0 as a default */
}   /* End of MainWndProc   */

/**************************************************************************
 *
 *  Name       : MessageBox(hwndOwner, nIdMsg, fsStyle, fBeep)
 *
 *  Description:  Displays the message box with the message
 *                given in idMsg retrieved from the message table
 *                and using the style flags in fsStyle.
 *
 *  Concepts:     Called whenever a message box is to be displayed
 *
 *                - Message string is loaded from the process'
 *                    message table
 *                - Alarm beep is sounded if desired
 *                - Message box with the message is displayed
 *                - WinMessageBox return value is returned
 *
 *  API's      :  WinLoadMessage
 *                WinAlarm
 *                WinMessageBox
 *
 *  Parameters :  hwndOwner = window handle of the owner
 *                nIdMsg    = message i.d.
 *                fsStyle   = style flags for the message box
 *                fBeep     = should an alarm be sounded?
 *
 *  Return     :  return value from WinMessageBox
 *
 *************************************************************************/
ULONG MessageBox(HWND hwndOwner, ULONG idMsg, ULONG fsStyle, BOOL fBeep)
{
   CHAR szText[MESSAGELEN];

   if(!WinLoadMessage(hab,
                     (HMODULE)NULL,
                     idMsg,
                     MESSAGELEN,
                     (PSZ)szText))
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return MBID_ERROR;
   }

   if(fBeep)
      WinAlarm(HWND_DESKTOP, WA_ERROR);

   return(WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        (PSZ)NULL,
                        MSGBOXID,
                        fsStyle));

}   /* End of MessageBox   */

/**************************************************************************
 *
 *  Name       : MainCommand(mp1, mp2)
 *
 *  Description: Calls the appropriate procedures that deal with
 *               the selected menu item.
 *
 *  Concepts:    Routine is called whenever a WM_COMMAND message
 *               is posted to the main window.
 *
 *               A switch statement branches on the id of the
 *               menu item that posted the message and the
 *               appropriate action for that item is taken.  Any
 *               menu ids that are not part of the standard menu
 *               set are passed onto the user defined WM_COMMAND
 *               processing procedure.
 *
 *  API's      : [none]
 *
 *  Parameters : mp1 = first message parameter
 *               mp2 = second message parameter
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MainCommand(MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {
   case IDM_FILENEW:
      FileNew(mp2);
      break;

   case IDM_FILEOPEN:
      FileOpen(mp2);
      break;

   case IDM_FILESAVE:
      FileSave(mp2);
      break;

   case IDM_FILESAVEAS:
      FileSaveAs(mp2);
      break;

   case IDM_EDITUNDO:
      EditUndo(mp2);
      break;

   case IDM_EDITCUT:
      EditCut(mp2);
      break;

   case IDM_EDITCOPY:
      EditCopy(mp2);
      break;

   case IDM_EDITPASTE:
      EditPaste(mp2);
      break;

   case IDM_EDITCLEAR:
      EditClear(mp2);
      break;

   case IDM_HELPINDEX:
      HelpIndex();
      break;

   case IDM_HELPGENERAL:
      HelpGeneral();
      break;

   case IDM_HELPUSINGHELP:
      HelpUsingHelp();
      break;

   case IDM_HELPKEYS:
      HelpKeys();
      break;

   case IDM_HELPTUTORIAL:
      HelpTutorial();
      break;

   case IDM_HELPPRODUCTINFO:
      HelpProductInfo();
      break;

   /*
    *      User command processing routine is called
    *      here so any ids not processed here can be
    *      processed.
    */
   default:
      UserCommand(mp1, mp2);
      break;
   }
}   /* End of MainCommand  */
/***************************  End of main.c  ****************************/

/*==============================================================*\
 *  svrpmain.c - Pipe Server end of Tic-Tac-Toe sample app
 *      Copyright 1992 IBM Corp.
 *--------------------------------------------------------------
 *
 *  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *  sample code created by IBM Corporation. This sample code is not
 *  part of any standard or IBM product and is provided to you solely
 *  for  the purpose of assisting you in the development of your
 *  applications.  The code is provided "AS IS", without
 *  warranty of any kind.  IBM shall not be liable for any damages
 *  arising out of your use of the sample code, even if they have been
 *  advised of the possibility of   such damages.                                                     *
 *
 *--------------------------------------------------------------
 *
 *  This c file contains the window creation routines and
 *  manages the PM message queue.
 *
\*==============================================================*/



/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_DOSPROCESS
#define INCL_DOSNMPIPES
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINMENUS
#define INCL_WINFRAMEMGR

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "svrpmain.h"
#include "svrphelp.h"
#include "svrpxtrn.h"

#define  THRD_EXIT_TIMEOUT  10

/*--------------------------------------------------------------*\
 *  Global variables
\*--------------------------------------------------------------*/

HWND   hwndMainFrame = NULLHANDLE;    /* handle to the main frame window */
HWND   hwndMain;                      /* handle to the main client window */
HAB    hab;                           /* anchor block for the process */
HMQ    hmq;                           /* handle to the process' message queue */
CHAR   szAppName[MAXNAMEL];           /* buffer for application name string */
BOOL   fHelpEnabled;                  /* flag to determine if help is enabled */
SHORT  sTotalMsgs;
CHAR   PipeMsgs[MAX_MESSAGES][MESSAGE_LEN];
HPIPE  hpStartPipe;
BOOL   fAppExit=FALSE;                /* application ending flag */
BOOL   fThrdsDead=FALSE;              /* notification of thread exitting */

unsigned int  iIndex=0;               /* beginning of circular buffer */


/****************************************************************\
 *  Main routine
 *--------------------------------------------------------------
 *
 *  Name:   main()
 *
 *  Purpose: Initializes the PM environment, calls the
 *              initialization routine, creates the main
 *              window,  and polls the message queue
 *
 *  Usage:
 *
 *  Method:
 *          - obtains anchor block handle and creates message
 *              queue
 *          - calls the initialization routine
 *          - creates the main frame window which creates the
 *              main client window
 *          - polls the message queue via Get/Dispatch Msg loop
 *          - upon exiting the loop, exits
 *
 *  Returns:
 *          0 - if successful execution completed
 *          1 - if error
\****************************************************************/
INT main(int argc, char *argv[])
{
   QMSG qmsg;          /* message structure */

   hab = WinInitialize(0);

   if(!hab)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return(RETURN_ERROR);
   }

   hmq = WinCreateMsgQueue(hab, 0);

   if(!hmq)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }

   if(!Init(argc, argv))
   {
      if(hwndMainFrame == NULLHANDLE)
      {
          MessageBox(HWND_DESKTOP,
                     IDMSG_MAINWINCREATEFAILED,
                     MB_OK | MB_ERROR,
                     TRUE);
      }

      else
      {
         MessageBox(HWND_DESKTOP,
                    IDMSG_INITFAILED,
                    MB_OK | MB_ERROR,
                    TRUE);

      }
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      if (WinIsWindow(hab, hwndMainFrame))
      {
         WinDestroyWindow(hwndMainFrame);
      }
      WinDestroyMsgQueue(hmq);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }

   /* Get-Dispatch Message loop */
   while(WinGetMsg(hab, (PQMSG)&qmsg, NULLHANDLE, 0, 0))
      WinDispatchMsg(hab, (PQMSG)&qmsg);

   /* destroy the help instance */
   DestroyHelpInstance();

   if (WinIsWindow(hab, hwndMainFrame))
   {
      WinDestroyWindow(hwndMainFrame);
   }
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return(RETURN_SUCCESS);

}   /* main() */


/****************************************************************\
 *  Main client window procedure
 *--------------------------------------------------------------
 *
 *  Name:   MainWndProc(hwnd, msg, mp1, mp2)
 *
 *  Purpose: Processes the messages sent to the main client
 *              window.  This routine processes the basic
 *              messages all client windows should process
 *              and passes all others onto WinDefWindowProc.
 *
 *  Usage:  Called for each message placed in the main
 *          window's message queue
 *
 *  Method: a switch statement branches to the routines to be
 *          performed for each message processed.
 *
 *  Returns:  Return values are determined by each message
 *
\****************************************************************/
MRESULT EXPENTRY MainWndProc(HWND   hwnd,      /* handle of window */
                             ULONG  msg,     /* id of message */
                             MPARAM mp1,     /* first message parameter */
                             MPARAM mp2)     /* second message parameter */
{
   static int  iCloseCount=0;

   switch(msg)
   {
      case WM_CREATE:
         return(InitMainWindow(hwnd, mp1, mp2));
         break;

      case WM_PAINT:
         MainPaint(hwnd, PipeMsgs, iIndex, sTotalMsgs);
         break;

      case WM_SIZE:
         MainSize(hwnd, sTotalMsgs, mp1, mp2);
         break;

      case WM_HSCROLL:
         MainHorizScroll(hwnd, mp2);
         break;

      case WM_VSCROLL:
         MainVertScroll(hwnd, mp2);
         break;

      case WM_CHAR:
         switch (SHORT2FROMMP(mp2))
         {
            case VK_LEFT:
            case VK_RIGHT:
               return MainCharHScroll (hwnd, msg, mp1, mp2) ;
            case VK_UP:
            case VK_DOWN:
            case VK_PAGEUP:
            case VK_PAGEDOWN:
               return MainCharVScroll (hwnd, msg, mp1, mp2) ;
         }
         break ;

      case WM_COMMAND:
         MainCommand(mp1, mp2);
         break;

      case HM_QUERY_KEYS_HELP:
         return (MRESULT)PANEL_HELPKEYS;   /* return id of key help panel */
         break ;

      case WM_PIPEMSG:
         /* copy message into circular array, with index being next free spot in array */
         strcpy(PipeMsgs[iIndex], (CHAR *)PVOIDFROMMP(mp1));

         /* increment index, or reset to 0 as appropriate */
         iIndex = (iIndex < MAX_MESSAGES-1) ? iIndex+1 : 0;

         /* increment total message count until buffer is filled */
         sTotalMsgs = min(MAX_MESSAGES, sTotalMsgs+1);

         MainUpdateMsg(hwnd, PipeMsgs, iIndex, sTotalMsgs);
         break;

      case WM_MSG:
         /* display messages from threads w/o a message queue */
         MessageBox(hwndMain, SHORT1FROMMP(mp1), MB_OK | MB_ICONEXCLAMATION, TRUE);
         break;

      case WM_CLOSE:
         fAppExit = TRUE;
         DosClose(hpStartPipe);  /* break start pipe */

         /* give other thread a chance to exit */
         /* but don't wait around forever for it */

         for (iCloseCount = 0; FALSE == fThrdsDead && iCloseCount < THRD_EXIT_TIMEOUT; iCloseCount++)
         {
            DosSleep(0);
         }

         /* else process the WM_CLOSE */

   /*--------------------------------------------------*\
    *      Any messages not processed are passed on
    *      to WinDefWindowProc.
   \*--------------------------------------------------*/

      default:
         return(WinDefWindowProc(hwnd, msg, mp1, mp2));
         break;
   }

   return (MRFROMLONG(0));      /* all window procedures should return 0 as a default */

}   /* MainWndProc() */


/****************************************************************\
 *  Message Box procedure
 *--------------------------------------------------------------
 *
 *  Name:   MessageBox(hwndOwner, nIdMsg, fsStyle, fBeep)
 *
 *  Purpose: Displays the message box with the message
 *              given in idMsg retrieved from the message table
 *              and using the style flags in fsStyle
 *
 *  Usage:  Called whenever a MessageBox is to be displayed
 *
 *  Method: - Message string is loaded from the process'
 *              message table
 *          - Alarm beep is sounded if desired
 *          - Message box with the message is displayed
 *          - WinMessageBox return value is returned
 *
 *  Returns: return value from WinMessageBox()
 *
\****************************************************************/

ULONG MessageBox(HWND  hwndOwner,  /* handle of the message box's owner */
                 ULONG idMsg,      /* id if the message in the message table */
                 ULONG fsStyle,    /* style of the message box */
                 BOOL  fBeep)      /* if TRUE, beep before message box is displayed */
{
   CHAR szText[TITLE_LEN];

   if(!WinLoadMessage(hab,
                     (HMODULE)NULL,
                     idMsg,
                     TITLE_LEN,
                     (PSZ)szText))
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return MBID_ERROR;
   }

   if(fBeep)  {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
   }

   return(WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        "Server Error",
                        MSGBOXID,
                        fsStyle));

}   /* MessageBox() */


/****************************************************************\
 *  Main window WM_COMMAND processing procedure
 *--------------------------------------------------------------
 *
 *  Name:   MainCommand(mp1, mp2)
 *
 *  Purpose: Calls the appropriate procedures that deal with
 *              the selected menu item.
 *
 *  Usage:  Routine is called whenever a WM_COMMAND message
 *          is posted to the main window.
 *
 *  Method: a switch statement branches on the id of the
 *          menu item that posted the message and the
 *          appropriate action for that item is taken.
 *
 *  Returns:
 *
\****************************************************************/
VOID MainCommand(MPARAM mp1, MPARAM mp2)
{

   switch(SHORT1FROMMP(mp1))
   {
      case IDM_FILEEXIT:
         FileExit();
         break;

      case IDM_OPTIONCLEAR:
         iIndex = sTotalMsgs = 0;
         MainPurgeWindow();
         break;

      case IDM_HELPHELPFORHELP:
         HelpHelpForHelp(mp2);
         break;

      case IDM_HELPEXTENDED:
         HelpExtended(mp2);
         break;

      case IDM_HELPKEYS:
         HelpKeys(mp2);
         break;

      case IDM_HELPINDEX:
         HelpIndex(mp2);
         break;

      case IDM_HELPTUTORIAL:
         HelpTutorial(mp2);
         break;

      case IDM_HELPABOUT:
         HelpAbout(mp2);
         break;

      default:
         break;
    }

}   /* MainCommand() */


/****************************************************************\
 *  Initialization routine
 *--------------------------------------------------------------
 *
 *  Name:   Init()
 *
 *  Purpose: Performs initialization functions.
 *
 *  Usage:  Called once before the message queue is queried.
 *
 *  Method:
 *          - starts processing thread
 *          - registers all window classes
 *
 *  Returns:
 *          TRUE - initialization is successful
 *          FALSE - initialization failed
\****************************************************************/
BOOL Init(int argc, char *argv[])
{
   ULONG  flCtlData;    /* frame control data */
   CHAR   pszBuff[CCHMAXPATH];
   RESULTCODES  resc;



   /* load application name from resource file */
   if(!WinLoadString(hab, 0, IDS_APPNAME, MAXNAMEL, szAppName))
   {
      return FALSE;
   }

   /* register the main client window class */
   if(!WinRegisterClass(hab,
                        (PSZ)szAppName,
                        MainWndProc,
                        CS_SIZEREDRAW | CS_CLIPCHILDREN,
                        0))
   {
      return FALSE;
   }

   flCtlData = FCF_STANDARD | FCF_VERTSCROLL | FCF_HORZSCROLL;

   if (NULLHANDLE == (hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                                         WS_VISIBLE,
                                                         (PULONG)&flCtlData,
                                                         (PSZ)szAppName,
                                                         NULL,
                                                         WS_VISIBLE,
                                                         (HMODULE)NULL,
                                                         IDR_MAIN,
                                                         (PHWND)&hwndMain)))
      return FALSE;

   WinSetWindowText(hwndMainFrame, szAppName);

   if (!(BOOL)(hpStartPipe=InitThreads()))
   {
      return FALSE;
   }

   if (argc > 1)
   {
      if ((BOOL)DosExecPgm(pszBuff, CCHMAXPATH, EXEC_ASYNC, NULL, NULL, &resc, argv[1]))
      {
         MessageBox(hwndMain, IDMSG_CANNOT_EXEC_CLIENT, MB_CUAWARNING | MB_OK, TRUE);
      }
   }

   InitHelp();

   return TRUE;
}  /* Init() */


/****************************************************************\
 *  Exit routine
 *--------------------------------------------------------------
 *
 *  Name:   FileExit(mp2)
 *
 *  Purpose: Processes the File menu's Exit item.
 *
 *  Usage:  called whenever Exit from the file menu is
 *          selected
 *
 *  Method:  Routine posts a WM_CLOSE message to the main
 *           application window.
 *
 *  Returns:
 *
\****************************************************************/
VOID FileExit()
{

   WinPostMsg(hwndMain, WM_CLOSE, (MPARAM)NULL, (MPARAM)NULL);

}   /* FileExit() */

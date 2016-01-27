/*==============================================================*\
 *
 *  cliqmain.c - sample queue application main source file
 *      Copyright 1996, IBM Corp.
 *
 *--------------------------------------------------------------
 *
 *  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *  sample code created by IBM Corporation. This sample code is not
 *  part of any standard or IBM product and is provided to you solely
 *  for  the purpose of assisting you in the development of your
 *  applications.  The code is provided "AS IS", without
 *  warranty of any kind.  IBM shall not be liable for any damages
 *  arising out of your use of the sample code, even if they have been
 *  advised of the possibility of such damages.
 *
 *--------------------------------------------------------------
 *
 *  This file contains the source for the main thread
 *  which services the PM message queue.
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_WINHELP
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSQUEUES
#define INCL_GPILCIDS
#define INCL_WIN

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cliqmain.h"
#include "queue.h"
#include "cliqxtrn.h"
#include "cliqhelp.h"

#define  ENTRY_STRING     "Queue Message"
#define  PRIORITY_STRING  "Message Priority"
#define  DAY_STRING       "Day"
#define  MONTH_STRING     "Month"
#define  YEAR_STRING      "Year"

#define  ENTRY_LEN        14
#define  PRIORITY_LEN     17
#define  DAY_LEN           4
#define  MONTH_LEN         6
#define  YEAR_LEN          5


/*--------------------------------------------------------------*\
 *  Global variables
\*--------------------------------------------------------------*/

HWND   hwndMainFrame = NULLHANDLE;    /* handle to the main frame window */
HWND   hwndMain;                      /* handle to the main client window */
HAB    hab;                           /* anchor block for the process */
HMQ    hmq;                           /* handle to the process' message queue */
CHAR   szAppName[MAXNAMEL];           /* buffer for application name string */
BOOL   fHelpEnabled;                  /* flag to determine if help is enabled */
HQUEUE hqQ;
LONG   cxChar, cyChar;
RECTL  rcl;
int    index;

BOOL         fThrdsDead = FALSE;
HEV          hevSend;
CHAR         pszMessage[MESSAGE_LEN];
ULONG        ulMsgSize, ulMsgType;
USHORT       usPriority;
Q_DATE       qdDate;
PVOID        pvdQMsg;
THREADPARAM  tpThrdParm;


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
INT main()
{
   QMSG qmsg;              /* message structure */
   int  iTimeout = 1000;   /* thread exit timeout */

   hab = WinInitialize(0);

   if(!hab)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return RETURN_ERROR;
   }

   hmq = WinCreateMsgQueue(hab, 0);

   if(!hmq)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return RETURN_ERROR;
   }

   if(!Init())
   {
      if(hwndMainFrame == NULLHANDLE)
      {
          MessageBox(HWND_DESKTOP,
                     IDMSG_MAINWINCREATEFAILED,
                     0,
                     MB_OK | MB_ERROR,
                     TRUE);
          return RETURN_ERROR;
      }

      else
      {
         MessageBox(HWND_DESKTOP,
                 IDMSG_INITFAILED,
                 0,
                 MB_OK | MB_ERROR,
                 TRUE);
         if (WinIsWindow(hab, hwndMainFrame))
         {
            WinDestroyWindow(hwndMainFrame);
         }
      }

      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinDestroyMsgQueue(hmq);
      WinTerminate(hab);
      return RETURN_ERROR;
   }

   /* Get-Dispatch Message loop */
   while(WinGetMsg(hab, (PQMSG)&qmsg, NULLHANDLE, 0UL, 0UL))
       WinDispatchMsg(hab, (PQMSG)&qmsg);

   /* destroy the help instance */
   DestroyHelpInstance();

   if (WinIsWindow(hab, hwndMainFrame))
   {
      WinDestroyWindow(hwndMainFrame);
   }

   WinDestroyMsgQueue(hmq);

   /* give the other thread a chance to exit */
   while (!fThrdsDead && iTimeout--)
   {
      DosSleep(0UL);
   }

   DosCloseEventSem(hevSend);
   WinTerminate(hab);
   return RETURN_SUCCESS;
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
MRESULT EXPENTRY MainWndProc(HWND   hwnd,    /* handle of window */
                             ULONG  msg,     /* id of message */
                             MPARAM mp1,     /* first message parameter */
                             MPARAM mp2)     /* second message parameter */
{
   HPS    hps;
   FONTMETRICS   fm;
   static POINTL ptl;

   switch(msg)
   {
      case WM_CREATE:
         hps = WinGetPS(hwnd);
         GpiQueryFontMetrics(hps, (LONG)sizeof fm, &fm);
         WinReleasePS(hps);
         cyChar = fm.lMaxBaselineExt;
         cxChar = fm.lAveCharWidth;
         rcl.yTop = 0;
         break;

      case WM_PAINT:
         if (0 == rcl.yTop)
         {
            WinQueryWindowRect(hwndMain, &rcl);
         }
         ptl.y = (4.0 / 5.0 * rcl.yTop + 1.0 / 2.0 * cxChar);
         ptl.x = 4 * cxChar;
         hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
         GpiErase(hps);
         GpiCharStringAt(hps, &ptl, ENTRY_LEN, ENTRY_STRING);

         ptl.y = ((2.0 / 5.0) * rcl.yTop + cxChar);
         GpiCharStringAt(hps, &ptl, PRIORITY_LEN, PRIORITY_STRING);

         ptl.y = (3.0 / 5.0 * rcl.yTop + 1.0 / 2.0 * cxChar);
         ptl.x = 1.0 / 4.0 * rcl.xRight - (DAY_LEN+1) * cxChar;
         GpiCharStringAt(hps, &ptl, DAY_LEN, DAY_STRING);

         ptl.y = (3.0 / 5.0 * rcl.yTop + 1.0 / 2.0 * cxChar);
         ptl.x = 2.0 / 4.0 * rcl.xRight - (MONTH_LEN + 1) * cxChar;
         GpiCharStringAt(hps, &ptl, MONTH_LEN, MONTH_STRING);

         ptl.y = (3.0 / 5.0 * rcl.yTop + 1.0 / 2.0 * cxChar);
         ptl.x = 3.0 / 4.0 * rcl.xRight - (YEAR_LEN + 1) * cxChar;
         GpiCharStringAt(hps, &ptl, YEAR_LEN, YEAR_STRING);
         WinEndPaint(hps);
         break;

      case WM_COMMAND:
          MainCommand(mp1, mp2);
          break;

      case WM_MSG:
         MessageBox(hwndMain,
                    (ULONG)SHORT1FROMMP(mp1),
                    LONGFROMMP(mp2),
                    MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                    TRUE);
         break;

      case HM_QUERY_KEYS_HELP:
          return (MRESULT)PANEL_HELPKEYS;
          break ;

      case WM_CLOSE:
         /* send thread a closure message */
         usPriority = 99;
         DosPostEventSem(hevSend);

      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
         break;
   }

   return MRFROMLONG(0);      /* all window procedures should return 0 as a default */

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
 *  Method: - Message string is loaded from the process's message table
 *          - Alarm beep is sounded if desired
 *          - Message box with the message is displayed
 *          - WinMessageBox return value is returned
 *
 *  Returns: return value from WinMessageBox()
 *
\****************************************************************/
ULONG MessageBox(HWND  hwndOwner,  /* handle of the message box's owner */
                 ULONG idMsg,      /* id of the message in the message table */
                 LONG  lRC,        /* return code of failing function */
                 ULONG fsStyle,    /* style of the message box */
                 BOOL  fBeep)      /* if TRUE, beep before message box is displayed */
{
   CHAR szText[TITLE_LEN + 18];    /* allow space for chars in sprintf below  */

   if(!WinLoadMessage(hab,
                    NULLHANDLE,
                    idMsg,
                    TITLE_LEN,
                    (PSZ)szText))
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return MBID_ERROR;
   }

   if (lRC)
   {
      sprintf(szText, "%s\n rc = %lu", szText, lRC);
   }

   if(fBeep)
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
   }

   return WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        (PSZ)NULL,
                        MSGBOXID,
                        fsStyle);

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
   ULONG  ulPosts;

   switch(SHORT1FROMMP(mp1))
   {
      case IDM_FILEEXIT:
         usPriority = 99;
         DosPostEventSem(hevSend);
         FileExit();
         break;

      case WID_SEND_DATE:
         /* still waiting to send last message? */
         DosQueryEventSem(hevSend, &ulPosts);

         if (0UL == ulPosts)
         {
            /* message type */
            ulMsgType = Q_MSG_DATE;
            pvdQMsg = (VOID *)&qdDate;

            /* get date */
            WinSendMsg(WinWindowFromID(hwndMain, WID_DAY),
                       SPBM_QUERYVALUE,
                       (MPARAM)&(qdDate.usDay),
                       MPFROM2SHORT(NULL, SPBQ_ALWAYSUPDATE));

            WinSendMsg(WinWindowFromID(hwndMain, WID_MONTH),
                       SPBM_QUERYVALUE,
                       (MPARAM)&(qdDate.usMonth),
                       MPFROM2SHORT(NULL, SPBQ_ALWAYSUPDATE));

            WinSendMsg(WinWindowFromID(hwndMain, WID_YEAR),
                       SPBM_QUERYVALUE,
                       (MPARAM)&(qdDate.usYear),
                       MPFROM2SHORT(NULL, SPBQ_ALWAYSUPDATE));

            /* message size */
            ulMsgSize = sizeof(Q_DATE);

            /* get priority from spinbutton */
            WinSendMsg(WinWindowFromID(hwndMain, WID_Q_PRIORITY),
                       SPBM_QUERYVALUE,
                       (MPARAM)&usPriority,
                       MPFROM2SHORT(NULL, SPBQ_ALWAYSUPDATE));

            /* post message */
            DosPostEventSem(hevSend);
         }

         else
         {
            /* didn't post message */
            DosBeep(100,100);
         }
         break;

      case WID_SEND_MSG:
         /* still waiting to send last message? */
         DosQueryEventSem(hevSend, &ulPosts);

         if (0UL == ulPosts)
         {
            /* message type */
            ulMsgType = Q_MSG_TEXT;
            pvdQMsg = (VOID *)pszMessage;

            /* get text */
            WinQueryWindowText(WinWindowFromID(hwndMain, WID_Q_MSG), MESSAGE_LEN, pszMessage);

            /* message size */
            ulMsgSize = strlen(pszMessage) + 1UL;

            /* get priority from spinbutton */
            WinSendMsg(WinWindowFromID(hwndMain, WID_Q_PRIORITY),
                       SPBM_QUERYVALUE,
                       (MPARAM)&usPriority,
                       MPFROM2SHORT(NULL, SPBQ_ALWAYSUPDATE));

            /* post message */
            DosPostEventSem(hevSend);
         }

         else
         {
            /* didn't post message */
            DosBeep(100UL, 100UL);
         }
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
 *  Purpose: Performs initialization functions required
 *              before the application can be executed.
 *
 *  Usage:  Called once before the message queue is serviced.
 *
 *  Method:
 *          - registers all window classes.
 *          - initializes processing thread.
 *
 *  Returns:
 *          TRUE - initialization is successful
 *          FALSE - initialization failed
\****************************************************************/
BOOL Init()
{
   ULONG  flCtlData;    /* frame control data */
   LONG   xLeft;
   HWND   hwndSpin;
   static USHORT  ausPriority[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

   /* load application name from resource file */
   if(!WinLoadString(hab, NULLHANDLE, IDS_APPNAME, MAXNAMEL, (PSZ)szAppName))
   {
      return FALSE;
   }

   /* register the main client window class */
   if(!WinRegisterClass(hab,
                       (PSZ)szAppName,
                       MainWndProc,
                       CS_SIZEREDRAW | CS_CLIPCHILDREN,
                       0UL))
   {
      return FALSE;
   }

   /* create the main window */
   flCtlData = FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU |
               FCF_MINMAX | FCF_ICON | FCF_BORDER | FCF_ACCELTABLE |
               FCF_ICON | FCF_SHELLPOSITION | FCF_TASKLIST;

   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      &flCtlData,
                                      (PSZ)szAppName,
                                      NULL,
                                      WS_VISIBLE,
                                      NULLHANDLE,
                                      IDR_MAIN,
                                      &hwndMain);
   if (NULLHANDLE == hwndMainFrame)
   {
      return FALSE;
   }

   WinSetWindowText(hwndMainFrame, szAppName);

   if (DosCreateEventSem(NULL, &hevSend, DC_SEM_SHARED, FALSE))
   {
      return FALSE;
   }

   if (0 == rcl.yTop)
   {
      WinQueryWindowRect(hwndMain, &rcl);
   }
   xLeft = rcl.xRight / 5;
   xLeft = max(xLeft, (cxChar * (ENTRY_LEN + 7)));
   xLeft = max(xLeft, (cxChar * (PRIORITY_LEN + 7)));

   if (!InitQThrd(hevSend, &pvdQMsg, &usPriority, &ulMsgSize, &ulMsgType))
   {
      DosCloseEventSem(hevSend);
      return FALSE;
   }

   WinCreateWindow(hwndMain,
                   WC_ENTRYFIELD,
                   "Queue Message",
                   (WS_VISIBLE | ES_MARGIN | WS_TABSTOP),
                   xLeft,
                   ((4.0 / 5.0) * rcl.yTop),
                   ((rcl.xRight) - xLeft - 4 * cxChar),
                   (cyChar + 2),
                   hwndMain,
                   HWND_BOTTOM,
                   WID_Q_MSG,
                   NULL,
                   NULL);

   hwndSpin = WinCreateWindow(hwndMain,
                              WC_SPINBUTTON,
                              "Priority",
                              (ULONG)(WS_VISIBLE | SPBS_MASTER | SPBS_NUMERICONLY | SPBS_JUSTRIGHT | WS_TABSTOP),
                              xLeft,
                              ((2.0 / 5.0) * rcl.yTop),
                              (cxChar * 10),
                              (cyChar + 2),
                              hwndMain,
                              HWND_BOTTOM,
                              WID_Q_PRIORITY,
                              NULL,
                              NULL);

   WinSendMsg(hwndSpin, SPBM_SETLIMITS, MPFROMLONG(15), MPVOID);

   hwndSpin = WinCreateWindow(hwndMain,
                              WC_SPINBUTTON,
                              "Day",
                              (WS_VISIBLE | SPBS_MASTER | SPBS_NUMERICONLY | SPBS_JUSTRIGHT | WS_TABSTOP),
                              (1.0 / 4.0 * rcl.xRight),
                              ((3.0 / 5.0) * rcl.yTop),
                              (cxChar * 10),
                              (cyChar + 2),
                              hwndMain ,
                              HWND_BOTTOM,
                              WID_DAY,
                              NULL,
                              NULL);

   WinSendMsg(hwndSpin, SPBM_SETLIMITS, MPFROMLONG(31), MPFROMLONG(1));

   hwndSpin = WinCreateWindow(hwndMain,
                              WC_SPINBUTTON,
                              "Month",
                              (ULONG)(WS_VISIBLE | SPBS_MASTER | SPBS_NUMERICONLY | SPBS_JUSTRIGHT | WS_TABSTOP),
                              (2.0 / 4.0 * rcl.xRight),
                              ((3.0 / 5.0) * rcl.yTop),
                              (cxChar*10),
                              (cyChar + 2),
                              hwndMain,
                              HWND_BOTTOM,
                              WID_MONTH,
                              NULL,
                              NULL);

   WinSendMsg(hwndSpin, SPBM_SETLIMITS, MPFROMLONG(12), MPFROMLONG(1));

   hwndSpin = WinCreateWindow(hwndMain,
                              WC_SPINBUTTON,
                              "Year",
                              (WS_VISIBLE | SPBS_MASTER | SPBS_NUMERICONLY | SPBS_JUSTRIGHT | WS_TABSTOP),
                              (3.0 / 4.0 * rcl.xRight),
                              ((3.0 / 5.0) * rcl.yTop),
                              (cxChar * 10),
                              (cyChar + 2),
                              hwndMain,
                              HWND_BOTTOM,
                              WID_YEAR,
                              NULL,
                              NULL);

   WinSendMsg(hwndSpin, SPBM_SETLIMITS, MPFROMLONG(2000), MPFROMLONG(1900));

   WinCreateWindow(hwndMain,
                   WC_BUTTON,
                   "Send Msg",
                   (WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP),
                   ((SHORT)(rcl.xRight/(LONG)3) - cxChar*(SHORT)8),
                   (rcl.yTop / 6),
                   (cxChar * 16),
                   (2 * cyChar),
                   hwndMain,
                   HWND_BOTTOM,
                   WID_SEND_MSG,
                   NULL, NULL);

   WinCreateWindow(hwndMain,
                   WC_BUTTON,
                   "Send Date",
                   (WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP),
                   ((2 * rcl.xRight/3) - cxChar * 9),
                   (rcl.yTop / 6),
                   (cxChar * 18),
                   (2 * cyChar),
                   hwndMain,
                   HWND_BOTTOM,
                   WID_SEND_DATE,
                   NULL, NULL);

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
   WinPostMsg(hwndMain, WM_CLOSE, MPVOID, MPVOID);
}   /* FileExit() */


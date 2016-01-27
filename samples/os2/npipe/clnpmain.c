/*==============================================================*\
 *  clnpmain.c - Pipe Server end of Tic-Tac-Toe sample app
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
#define INCL_DOSPROCESS
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_WININPUT
#define INCL_WINDIALOGS
#define INCL_DOSSEMAPHORES
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "clnpmain.h"
#include "clnphelp.h"
#include "clnpxtrn.h"
#include "namepipe.h"

#define  THRD_EXIT_TIMEOUT  10

/*--------------------------------------------------------------*\
 *  Global variables
\*--------------------------------------------------------------*/

HWND   hwndMainFrame = NULLHANDLE;    /* handle to the main frame window */
HWND   hwndMain;                      /* handle to the main client window */
HWND   hwndMenu;
HAB    hab;                           /* anchor block for the process */
HMQ    hmq;                           /* handle to the process' message queue */
CHAR   szAppName[MAXNAMEL];           /* buffer for application name string */
BOOL   fHelpEnabled=FALSE;            /* flag to determine if help is enabled */
BOOL   fAppExit=FALSE;                /* flag to signal application exit */
BOOL   fMyTurn=FALSE;                 /* flag allowing client to move */
USHORT usWhosX=CLIENT_NUM;            /* flag denoting who is X piece */
ULONG  ulActiveSquare=0;              /* window id of selected child */
HEV    hevWritePipe;                  /* event semaphore notifying pipe thread of message */
PIPE_MSG  pmsgPipeMsg;                /* message to be written to pipe */
BOOL   fThrdsDead=FALSE;              /* notification of thread exitting */


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
   QMSG   qmsg;          /* message structure */
   ULONG  ulPosts;

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

   if(!Init())
   {
      if(hwndMainFrame == NULLHANDLE)
      {
          MessageBox(HWND_DESKTOP,
                     IDMSG_MAINWINCREATEFAILED,
                     0,
                     MB_OK | MB_ERROR,
                     NULL,
                     TRUE);
      }

      else
      {
         MessageBox(HWND_DESKTOP,
                    IDMSG_INITFAILED,
                     0,
                    MB_OK | MB_ERROR,
                    NULL,
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
MRESULT EXPENTRY MainWndProc(HWND   hwnd,    /* handle of window */
                             ULONG  msg,     /* id of message */
                             MPARAM mp1,     /* first message parameter */
                             MPARAM mp2)     /* second message parameter */
{
   USHORT       usMsg=IDMSG_CLIENT_WIN;
   LONG         xBlock, yBlock;
   CHAR         *szChildClass = "TicTacToeSquare", *pszWinMsg="YOU WON ! !";
   int          i, j;
   HWND         hwndSquare;
   ULONG        ulOldSquare;
   static HWND  hwndChild[DIVISIONS][DIVISIONS];
   static int   iCloseCount=0;

   switch(msg)
   {
      case WM_CREATE:

         WinRegisterClass (hab, szChildClass, ChildWndProc,
                           CS_SIZEREDRAW, sizeof(ULONG)*2) ;

         for (i = 0 ; i < DIVISIONS ; i++)
         {
            for (j = 0 ; j < DIVISIONS ; j++)
            {
               hwndChild [i][j] = WinCreateWindow (
                                               hwnd,          /* Parent window   */
                                               szChildClass,  /* Window class    */
                                               NULL,          /* Window text     */
                                               WS_VISIBLE |   /* Window style    */
                                               WS_SYNCPAINT,
                                               0, 0, 0, 0,    /* Position & size */
                                               hwnd,          /* Owner window    */
                                               HWND_BOTTOM,   /* Placement       */
                                               (i*DIVISIONS+j), /* Child window ID */
                                       (PVOID) NULL,          /* Control data    */
                                       (PVOID) NULL) ;        /* Pres. Params    */
               /* set window id in window data area */
               if (NULLHANDLE == hwndChild[i][j] ||
                   FALSE == WinSetWindowULong(hwndChild[i][j],
                                              sizeof(ULONG),
                                              (i * DIVISIONS + j)))
               {
                  return (MRFROMLONG(1));
               }
            }
         }

         break;

      case WM_BUTTON1DOWN:
      case WM_BUTTON1DBLCLK:
           WinAlarm (HWND_DESKTOP, WA_WARNING) ;
           break ;

      case WM_SIZE:
         xBlock = SHORT1FROMMP (mp2) / DIVISIONS ;
         yBlock = SHORT2FROMMP (mp2) / DIVISIONS ;

         for (i = 0 ; i < DIVISIONS ; i++)
         {
            for (j = 0 ; j < DIVISIONS ; j++)
            {
               if (FALSE == WinSetWindowPos (hwndChild[j][i], NULLHANDLE,
                                             (i * xBlock), (j * yBlock),
                                             xBlock, yBlock,
                                             (SWP_MOVE | SWP_SIZE)))
               {
                  return (MRFROMLONG(1));
               }
            }
         }
         break;

      case WM_ERASEBACKGROUND:
           return MRFROMLONG(1);

      case WM_CHAR:
         if (SHORT1FROMMP(mp1) & KC_KEYUP)
            break;

         if (!(SHORT1FROMMP(mp1) & KC_VIRTUALKEY))
            break;

         ulOldSquare = ulActiveSquare;

         switch (SHORT2FROMMP(mp2))
         {
            case VK_TAB:
               /* make next client window active window */
               ulActiveSquare = (((DIVISIONS*DIVISIONS-1) == ulActiveSquare) ? 0 : ulActiveSquare+1);
               break;

            case VK_BACKTAB:
               /* make previous client window active window */
               ulActiveSquare = ((0 == ulActiveSquare) ? (DIVISIONS*DIVISIONS-1) : ulActiveSquare-1);
               break;

            case VK_LEFT:
               /* make client window to left active window */
               ulActiveSquare = ((((ulActiveSquare-1)/DIVISIONS) == (ulActiveSquare/DIVISIONS)) ? ulActiveSquare-1 : ulActiveSquare+(DIVISIONS-1));
               break;

            case VK_RIGHT:
               /* make client window to right active window */
               ulActiveSquare = ((((ulActiveSquare+1)/DIVISIONS) == (ulActiveSquare/DIVISIONS)) ? ulActiveSquare+1 : ulActiveSquare+1-DIVISIONS);
               break;

            case VK_UP:
               /* make client window above active window */
               ulActiveSquare = ((ulActiveSquare+DIVISIONS < (DIVISIONS*DIVISIONS)) ? ulActiveSquare+DIVISIONS : ulActiveSquare-((DIVISIONS-1)*DIVISIONS));
               break;

            case VK_DOWN:
               /* make client window below active window */
               /* ulActiveSquares is unsigned, so instead of testing for */
               /* less than zero, test for greater than it is allowed to be */
               ulActiveSquare = (((ulActiveSquare-DIVISIONS) < (DIVISIONS*DIVISIONS)) ? ulActiveSquare-DIVISIONS : ulActiveSquare+((DIVISIONS-1)*DIVISIONS));
               break;

            case VK_NEWLINE:
            case VK_ENTER:
            case VK_SPACE:
               WinPostMsg(WinWindowFromID(hwnd, ulActiveSquare), WM_BUTTON1DOWN, MPVOID, MPVOID);
               break;
         }

         /* have squares repaint themselves */
         WinInvalidateRect(WinWindowFromID(hwnd, ulOldSquare), NULL, FALSE);
         WinInvalidateRect(WinWindowFromID(hwnd, ulActiveSquare), NULL, FALSE);
         break ;

      case WM_COMMAND:
         MainCommand(mp1, mp2);
         break;

      case HM_QUERY_KEYS_HELP:
         return (MRESULT)PANEL_HELPKEYS;   /* return id of key help panel */
         break ;

      case WM_START:
         WinEnableMenuItem(hwndMenu, IDM_FILESTART, TRUE);
         break;

      case WM_SERVER_MOVE:
         /* set window to server playing piece */
         WinSetWindowULong((hwndSquare=WinWindowFromID(hwnd, SHORT1FROMMP(mp1))),
                           0,
                           SERVER_NUM);

         /* repaint square */
         WinInvalidateRect(hwndSquare, NULL, FALSE);
         fMyTurn = TRUE;
         break;

      case WM_DRAW:
         usMsg = IDMSG_DRAW;
         pszWinMsg = "Game is a Draw";
         /* NO BREAK */

      case WM_WIN_SERVER:
         if (SHORT1FROMMP(mp1) < DIVISIONS*DIVISIONS)
         {
            /* display winning move */
            WinSetWindowULong((hwndSquare=WinWindowFromID(hwnd, SHORT1FROMMP(mp1))),
                              0,
                              SERVER_NUM);
            WinInvalidateRect(hwndSquare, NULL, FALSE);
            if (msg != WM_DRAW)
            {
               usMsg = IDMSG_SERVER_WIN;
               pszWinMsg = "I WON ! !";
            }
         }
         /* NO BREAK */

      case WM_WIN_CLIENT:
         if (MBID_YES == MessageBox(hwndMain, usMsg, 0, MB_YESNO | MB_ICONQUESTION, pszWinMsg, TRUE))
         {
            InitBoard();
         }

         else
         {
            WinPostMsg(hwndMain, WM_COMMAND, MPFROMLONG(IDM_FILEEXIT), MPVOID);
         }
         break;

      case WM_MSG:
         /* default message box */
         if (MPVOID == mp2)
         {
            MessageBox(hwndMain, SHORT1FROMMP(mp1), LONGFROMMP(mp2), MB_OK | MB_ICONEXCLAMATION, NULL, TRUE);
         }

         /* custom message box */
         else
         {
            MessageBox(hwndMain, SHORT1FROMMP(mp1), LONGFROMMP(mp2), SHORT2FROMMP(mp1), NULL, TRUE);
         }
         break;

      case WM_CLOSE:
         fAppExit = TRUE;
         PostPipeMsg(CLIENT_QUIT, 0);

         /* give other thread a chance to exit */
         /* but don't wait around forever for it */

         for (iCloseCount = 0; FALSE == fThrdsDead && iCloseCount < THRD_EXIT_TIMEOUT; iCloseCount++)
         {
            DosSleep(0);
         }

         /* process the WM_CLOSE */

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
 *  Name:   MessageBox(hwndOwner, nIdMsg, lRC, fsStyle, pszTitle, fBeep)
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

ULONG MessageBox(HWND  hwndOwner, /* handle of the message box's owner */
                 ULONG idMsg,     /* id if the message in the message table */
                 LONG  lRC,       /* return code of failing function */
                 ULONG fsStyle,   /* style of the message box */
                 PSZ   pszTitle,  /* title of message box */
                 BOOL  fBeep)     /* if TRUE, beep before message box is displayed */
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

   if (NULL == pszTitle)
   {
      pszTitle = "Client Error";
   }

   if (lRC)
   {
      sprintf(szText, "%s\n rc = %lu", szText, lRC);
   }

   if(fBeep)  {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
   }

   return(WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        pszTitle,
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

      case IDM_FILESTART:
         /* ask player who should move first, perhaps with a message box */
         WinEnableMenuItem(hwndMenu, IDM_FILESTART, FALSE);
         if (MBID_YES == MessageBox(hwndMain, IDMSG_ME_FIRST, 0, MB_ICONQUESTION | MB_YESNO, "", FALSE))
         {
            fMyTurn = TRUE;
         }

         else
         {
            PostPipeMsg(YOU_FIRST, 0);
         }
         break;

      case IDM_OPTIONSWAPPIECES:
         usWhosX = (USHORT)((SERVER_NUM == usWhosX) ? CLIENT_NUM : SERVER_NUM);
         WinInvalidateRect(hwndMain, NULL, TRUE);
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


/*
 *   Window procedure for each tic-tac-toe square.
 */
MRESULT EXPENTRY ChildWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   HPS     hps ;
   RECTL   rcl ;
   USHORT  usWinState;
   ULONG   ulOldSquare;

   switch (msg)
   {
      case WM_CREATE:
         /* initialize square as empty */
         WinSetWindowULong (hwnd, 0, 0) ;
         break;

      case WM_BUTTON1DOWN:
         if (fMyTurn && 0 == (BOOL)WinQueryWindowULong (hwnd, 0))
         {
            fMyTurn = FALSE;
            PostPipeMsg(CLIENT_MOVE, WinQueryWindowULong(hwnd, sizeof(ULONG)));
            ulOldSquare = ulActiveSquare;
            ulActiveSquare = WinQueryWindowULong(hwnd, sizeof(ULONG));
            WinSetWindowULong (hwnd, 0, CLIENT_NUM);
            WinInvalidateRect(WinWindowFromID(hwndMain, ulOldSquare), NULL, FALSE);
            WinInvalidateRect(hwnd, NULL, FALSE);
            WinSetActiveWindow (HWND_DESKTOP, hwnd);
         }

         else
         {
            WinAlarm(HWND_DESKTOP, WA_ERROR);
         }
         break;

      case WM_PAINT:
         WinQueryWindowRect (hwnd, &rcl) ;

         hps = WinBeginPaint (hwnd, NULLHANDLE, NULL) ;

         if (WinQueryWindowULong(hwnd, sizeof(ULONG)) == ulActiveSquare)
         {
            WinDrawBorder (hps, &rcl, 2, 2, CLR_NEUTRAL, CLR_BACKGROUND,
                           DB_STANDARD | DB_INTERIOR) ;
         }

         else
         {
            WinDrawBorder (hps, &rcl, 1, 1, CLR_NEUTRAL, CLR_BACKGROUND,
                           DB_STANDARD | DB_INTERIOR) ;
         }

         if ((BOOL)(usWinState = (USHORT)WinQueryWindowULong (hwnd, 0)))
         {
            DrawPiece(hps, usWinState, rcl);
         }

         WinEndPaint (hps) ;
         break;

      case WM_CHAR:
         switch (SHORT2FROMMP(mp2))
         {
            case VK_NEWLINE:
            case VK_ENTER:
            case VK_SPACE:
               if (fMyTurn && 0 == (BOOL)WinQueryWindowULong (hwnd, 0))
               {
                  fMyTurn = FALSE;
                  PostPipeMsg(CLIENT_MOVE, WinQueryWindowULong(hwnd, sizeof(ULONG)));
                  WinSetWindowULong (hwnd, 0, CLIENT_NUM);
                  WinInvalidateRect (hwnd, NULL, FALSE);
               }

               else
               {
                  WinAlarm(HWND_DESKTOP, WA_ERROR);
               }

               break;

            default:
               /* let the main window deal with it */
               WinPostMsg(hwndMain, msg, mp1, mp2);
         }
         break;

      default:
         return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
   }
   return MRFROMLONG(0);
}


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
BOOL Init()
{
   ULONG  flCtlData;    /* frame control data */


   /* load application name from resource file */
   if(!WinLoadString(hab, 0, IDS_APPNAME, MAXNAMEL, szAppName))
   {
      return FALSE;
   }

   /* register the main client window class */
   if(!WinRegisterClass(hab,
                        szAppName,
                        MainWndProc,
                        CS_SIZEREDRAW | CS_CLIPCHILDREN,
                        0))
   {
      return FALSE;
   }

   flCtlData = FCF_STANDARD;

   if (NULLHANDLE ==
       (hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                           WS_VISIBLE,
                                           &flCtlData,
                                           szAppName,
                                           NULL,
                                           WS_VISIBLE,
                                           NULLHANDLE,
                                           IDR_MAIN,
                                           &hwndMain)))
   {
      return FALSE;
   }

   WinSetWindowText(hwndMainFrame, szAppName);

   InitHelp();

   if (!InitThread())
   {
      return FALSE;
   }

   hwndMenu = WinWindowFromID(hwndMainFrame, FID_MENU);

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

   fAppExit = TRUE;
   WinPostMsg(hwndMain, WM_CLOSE, (MPARAM)NULL, (MPARAM)NULL);
}   /* FileExit() */


/*
 *   Draw X or O in square.
 */
VOID DrawPiece(HPS hps, USHORT usPiece, RECTL rcl)
{
   POINTL ptl;
   LONG   xAxis, yAxis;

   if (usWhosX == usPiece)
   {
      /* Draw X */
      ptl.x = rcl.xLeft;
      ptl.y = rcl.yBottom;
      GpiMove (hps, &ptl);
      ptl.x = rcl.xRight;
      ptl.y = rcl.yTop;
      GpiLine (hps, &ptl);

      ptl.x = rcl.xLeft;
      ptl.y = rcl.yTop;
      GpiMove (hps, &ptl);
      ptl.x = rcl.xRight;
      ptl.y = rcl.yBottom;
      GpiLine (hps, &ptl);
   }

   else
   {
      /* Draw O */
      ptl.x = rcl.xLeft;
      ptl.y = rcl.yBottom;
      GpiMove (hps, &ptl);
      xAxis = rcl.xRight - rcl.xLeft;
      yAxis = rcl.yTop - rcl.yBottom;
      ptl.x = rcl.xRight;
      ptl.y = rcl.yTop;
      GpiBox(hps, DRO_OUTLINE, &ptl, xAxis, yAxis);
   }
}


/*
 *   Copy message to buffer and post event semaphore.
 */
VOID PostPipeMsg(USHORT usMsg, USHORT usData)
{

   pmsgPipeMsg.usMsgType = usMsg;
   pmsgPipeMsg.usMsgData = usData;
   DosPostEventSem(hevWritePipe);
}


/*
 *   Erase game board.
 */
VOID InitBoard()
{
   int i, j;

   fMyTurn = FALSE;
   for (i = 0 ; i < DIVISIONS ; i++)
   {
      for (j = 0 ; j < DIVISIONS ; j++)
      {
         WinSetWindowULong(WinWindowFromID(hwndMain, (DIVISIONS*i+j)), 0, 0);
      }
   }

   WinInvalidateRect(hwndMain, NULL, TRUE);
   WinEnableMenuItem(hwndMenu, IDM_FILESTART, TRUE);
}

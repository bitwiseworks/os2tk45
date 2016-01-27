/*static char *SCCSID = "@(#)sem_main.c	6.16 92/02/19";*/
/*==============================================================*\
 *  SEM_MAIN.C - Sample PM application                          *
 *     (C) Copyright IBM Corporation 1992.                      *
 *--------------------------------------------------------------*
 *                                                              *
 *  This application serves two distinct purposes.  First, it   *
 *  demonstrates the standard menus and dialogs that most PM    *
 *  applications will contain.  The Demo menu calls dialog      *
 *  boxes that demonstrate all of the controls available for    *
 *  use in a dialog box.  There is also a dialog box which      *
 *  demonstrates the setting and removing of control            *
 *  Presentation Parameters.  Finally, the demo menu also has   *
 *  menu items that demonstrate the various types of message    *
 *  boxes that the system supports.                             *
 *                                                              *
 *  This application also serves as a template than can be      *
 *  easily modified by an application developer.  The source    *
 *  files are organized so that the overhead code that should   *
 *  be in all applications is located in the same files so      *
 *  that these files do not need to be modified.  The routines  *
 *  that deal with application specific code are also located   *
 *  in their own modules.  An application developer need only   *
 *  change these files in order to modify this template for     *
 *  his application.                                            *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *   main() - main routine                                      *
 *   MainWndProc(hwnd, msg, mp1, mp2) - main window procedure   *
 *   MessageBox(hwnd nId, bBeep) - Warning box routine          *
 *   MainCommand(mp1, mp2) - WM_COMMAND processing of Main      *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

/****************************************************************\
 *  Main routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   main()                                              *
 *                                                              *
 *  Purpose: Initializes the PM environment, calls the          *
 *              initialization routine, creates the main        *
 *              window,  and polls the message queue            *
 *                                                              *
 *  Usage:                                                      *
 *                                                              *
 *  Method:                                                     *
 *          - obtains anchor block handle and creates message   *
 *              queue                                           *
 *          - calls the initialization routine                  *
 *          - creates the main frame window which creates the   *
 *              main client window                              *
 *          - polls the message queue via Get/Dispatch Msg loop *
 *          - upon exiting the loop, exits                      *
 *                                                              *
 *  Returns: (via DosExit)                                      *
 *          1 - if sucessful execution completed                *
 *          0 - if error                                        *
\****************************************************************/
INT main(VOID)
{
   QMSG qmsg;          /* message structure */
   ULONG ctlData;      /* frame control data */

   strcpy(szWindowText,"Sample");
   cNumUsers=NUMUSERS;
   fSemaphoreStarted=FALSE;
   fSemaphoreWasStarted=FALSE;
   fAutoMode=FALSE;
   ulTimeout = TIMEOUTPERIOD;

   hab = WinInitialize(0);
   if(!hab) {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return(RETURN_ERROR);
   }
   hmq = WinCreateMsgQueue(hab, 0);
   if(!hmq) {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }
   if(!Init()) {
      MessageBox(HWND_DESKTOP, IDMSG_INITFAILED, TRUE);
      DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
                                         /* create the main window */
   ctlData = FCF_STANDARD;
   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      (PVOID)&ctlData,
                                      (PSZ)szAppName,
                                      (PSZ)NULL,
                                      WS_VISIBLE,
                                      (HMODULE)NULL,
                                      IDR_RESOURCE,
                                      (PHWND)&hwndMain);

   if(!hwndMainFrame)
   {
       MessageBox(HWND_DESKTOP, IDMSG_MAINWINCREATEFAILED, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
   InitHelp();

   while(WinGetMsg(hab, (PQMSG)&qmsg, (HWND)0, 0, 0))
       WinDispatchMsg(hab, (PQMSG)&qmsg);
                                          /* destroy the help instance */
   DestroyHelpInstance();
                                    /* will normally be put in ExitProc */
   DosExit(EXIT_PROCESS, RETURN_SUCCESS);
   return 0L;
}

/****************************************************************\
 *  Main client window procedure                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainWndProc(hwnd, msg, mp1, mp2)                    *
 *                                                              *
 *  Purpose: Processes the messages sent to the main client     *
 *              window.  This routine processes the basic       *
 *              messages all client windows should process      *
 *              and passes all others onto UserWndProc where    *
 *              the developer can process any others.           *
 *                                                              *
 *  Usage:  Called for each message placed in the main          *
 *          window's message queue                              *
 *                                                              *
 *  Method: a switch statement branches to the routines to be   *
 *          performed for each message processed.  Any messages *
 *          not specifically process are passed to the user's   *
 *          message processing procedure UserWndProc().         *
 *                                                              *
 *  Returns:  Return values are determined by each message      *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MainWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    FONTMETRICS  fntmet;
    HPS          hPS;
    static SHORT xClient, yClient;
    static SHORT yChar, yDesc;
    MRESULT sRC;

    switch(msg)
    {
       case WM_CREATE:
           hPS = WinGetPS (hwnd);
           GpiQueryFontMetrics (hPS, (LONG) sizeof fntmet, &fntmet);

           yChar = fntmet.lMaxBaselineExt;
           yDesc = fntmet.lMaxDescender;

           WinReleasePS (hPS);
           break;

       case WM_PAINT:
           MainPaint(hwnd);
           break;

       case WM_SIZE:
           xClient = LOUSHORT (mp2);   /* width of window  */
           yClient = HIUSHORT (mp2);   /* height of window */
           WinPostMsg (hwndMain, WM_USERCHANGED, (MPARAM)0, (MPARAM)0);
           break;
                                       /* fall thru to ... */
       case WM_USERCHANGED:            /* number of threads changed */
                                       /* so x and y are already known */
           SetRectPositions(xClient, yClient, yChar, yDesc);
           WinInvalidateRect (hwnd, NULL, TRUE);
           break;

       case WM_COMMAND:
           MainCommand(hwnd, mp1, mp2);
           break;

        /*-----------------------------------------------*\
         *   Any messages not processed are passed on    *
         *   to the user's window proc.  It is           *
         *   responsible for passing any messages it     *
         *   doesn't handle onto WinDefWindowProc()      *
        \*-----------------------------------------------*/
       case HM_QUERY_KEYS_HELP:
           return ((MRESULT)PANEL_KEYSHELP);

       default:
           sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
           return sRC;

    }
    return (MRESULT)0L;
                  /* all window procedures should return 0 as a default */
}                                                      /* MainWndProc() */

/****************************************************************\
 *  Main window WM_COMMAND processing procedure                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainCommand(hwnd, mp1, mp2)                         *
 *                                                              *
 *  Purpose: Calls the appropriate procedures that deal with    *
 *              the selected menu item.                         *
 *                                                              *
 *  Usage:  Routine is called whenever a WM_COMMAND message     *
 *          is posted to the main window.                       *
 *                                                              *
 *  Method: a switch statement branches on the id of the        *
 *          menu item that posted the message and the           *
 *          appropriate action for that item is taken.  Any     *
 *          menu ids that are not part of the standard menu     *
 *          set are passed onto the user defined WM_COMMAND     *
 *          processing procedure.                               *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID MainCommand(HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    switch(SHORT1FROMMP(mp1))
    {
        case IDM_GENERALHELP:
           HelpExtended(mp2);
           break;

        case IDM_USINGHELP:
           HelpHelpForHelp(mp2);
           break;

        case IDM_TUTORIAL:
           HelpKeys(mp2);
           break;

        case IDM_HELPINDEX:
           HelpIndex(mp2);
           break;

        case IDM_HELPPRODUCTINFO:
           HelpAbout(mp2);
           break;

    /*--------------------------------------------------*\
     *      User command processing routine is called   *
     *      here so any ids not procecessed here can be *
     *      processed                                   *
    \*--------------------------------------------------*/
        default:
            UserCommand(mp1, mp2);
            break;
    }
    return;
}                                             /* MainCommand() */

/****************************************************************\
 *  Message Box procedure                                       *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MessageBox(hwndOwner, nIdMsg, bBeep)                *
 *                                                              *
 *  Purpose: Displays the warning message box with the message  *
 *              given in nIdMsg retrived from the message table *
 *                                                              *
 *  Usage:  Called whever an error occurs and a message wishes  *
 *          to be displayed to the user                         *
 *                                                              *
 *  Method: - Message string is loaded from the process'        *
 *              message table                                   *
 *          - Alarm beep is sounded if desired                  *
 *          - Message box with the message is displayed         *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID MessageBox( HWND hwndOwner, /* handle of the message box's owner      */
                 LONG nIdMsg,   /* id if the message in the message table */
                 BOOL bBeep ) /* TRUE, beep before message box is displayed*/
{
   CHAR szText[MESSAGELEN];

   if(!WinLoadMessage(hab, (HMODULE)NULL, nIdMsg, MESSAGELEN, (PSZ)szText))
   {
       WinAlarm(HWND_DESKTOP, WA_ERROR);
       return;
   }

   if(bBeep)
       WinAlarm(HWND_DESKTOP, WA_ERROR);

   WinMessageBox(HWND_DESKTOP,
                 hwndOwner,
                 szText,
                 (PSZ)NULL,
                 IDM_MSGBOX,
                 MB_OK | MB_ERROR);
   return;
}                                                        /* MessageBox() */

/*static char *SCCSID = "@(#)dllapi.c	6.13 92/02/18";*/
/*==============================================================*\
 *  DLLAPI.C - Sample PM application                            *
 *                                                              *
 *      (C) Copyright IBM Corporation 1992.                     *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This DLL application program consists of a DLL file that    *
 *  provides a 32-bit DOS called functions, and an API's EXE    *
 *  file that uses the dynamic linking library to handle some   *
 *  operations. The DLL uses protected memory to protect its    *
 *  shared data, and exception management to validate the API's *
 *  pointer parameters, handle the errors returned.             *
 *  This application also serves as a template that can be      *
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
 *      main() - main routine                                   *
 *      MainWndProc(hwnd, msg, mp1, mp2) - main window procedure*
 *      MainCommand(hwnd, mp1, mp2) - WM_COMMAND processing     *
 *      MessageBox(hwnd nId, pszMsg, fsStyle, bBeep)            *
 *                                                              *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/

#include "dllapi.h"

/****************************************************************\
 *  Main routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    main()                                             *
 *                                                              *
 *  Purpose: Initializes the PM environment, calls the          *
 *           initialization routine, creates the main window,   *
 *           and polls the message queue.                       *
 *                                                              *
 *  Usage:                                                      *
 *                                                              *
 *  Method:  -obtains anchor block handle and creates message   *
 *              queue                                           *
 *           -calls the initialization routine                  *
 *           -creates the main frame window which creates the   *
 *              main client window                              *
 *           -polls the message queue via Get/Dispatch Msg loop *
 *           -upon exiting the loop, exits                      *
 *                                                              *
 *  Returns: 1 - if sucessful execution completed               *
 *           0 - if error                                       *
 *                                                              *
\****************************************************************/
INT main(VOID)
{
   QMSG qmsg;                                      /* message structure */
   ULONG ctlData=FCF_STANDARD;               /* main frame control data */
   EXCEPTIONREGISTRATIONRECORD ExceptionStruct;

   strcpy(szWindowText,"DLLAPI Sample");
   hwndChild = (HWND)0;
   hab = WinInitialize(0);
   if(!hab)
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
   hmq = WinCreateMsgQueue(hab, 0);
   if(!hmq)
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       WinTerminate(hab);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
   if(!Init())
   {
       MessageBox(HWND_DESKTOP, IDMSG_INITFAILED, "Error !",
                           MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
                                              /* create the main window */
   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      (PVOID)&ctlData,
                                      (PSZ)szAppName,
                                      (PSZ)NULL,
                                      WS_VISIBLE,
                                      (HMODULE)NULL,
                                      ID_RESOURCE,
                                      (PHWND)&hwndMain);
   if(!hwndMainFrame)
   {
       MessageBox(HWND_DESKTOP, IDMSG_MAINWINCREATEFAILED, "Error !",
                           MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
   InitHelp();
   ExceptionStruct.ExceptionHandler = (_ERR *)&GPFHandler;
   DosSetExceptionHandler(&ExceptionStruct) ;

   while(WinGetMsg(hmq, (PQMSG)&qmsg, 0L, 0L, 0L))
           WinDispatchMsg(hmq, (PQMSG)&qmsg);

   DosUnsetExceptionHandler(&ExceptionStruct) ;
                                           /* destroy the help instance */
   DestroyHelpInstance();
                                    /* will normally be put in ExitProc */
   DosExit(EXIT_PROCESS, RETURN_SUCCESS);
   return 0;
}                                                              /* main() */

/****************************************************************\
 *  Main client window procedure                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainWndProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: Processes the messages sent to the main client     *
 *           window. This routine processes the basic messages  *
 *           all client windows should process and passes all   *
 *           others onto UserWndProc where the developer can    *
 *           process any others.                                *
 *                                                              *
 *  Usage:   Called for each message placed in the main         *
 *           window's message queue                             *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed. Any messages *
 *           not specifically process are passed to the user's  *
 *           message processing procedure UserWndProc().        *
 *                                                              *
 *  Returns: Return values are determined by each message       *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg) {
       case WM_PAINT:
           MainPaint(hwnd);
           break;

       case WM_COMMAND:
           MainCommand(hwnd, mp1, mp2);
           break;

   /*--------------------------------------------------*\
    *      Any messages not processed are passed on    *
    *      to the user's window proc.  It is           *
    *      responsible for passing any messages it     *
    *      doesn't handle onto WinDefWindowProc()      *
   \*--------------------------------------------------*/
       case HM_QUERY_KEYS_HELP:
           return ((MRESULT)PANEL_KEYSHELP);

       default:
           sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
           return sRC;
   }
   return (MRESULT)0L; /*all window procedures should return 0 as a default*/
}                                                        /* MainWndProc() */

/****************************************************************\
 *  Main client painting routine                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainPaint(hwnd)                                    *
 *                                                              *
 *  Purpose: Paints the main client window.                     *
 *                                                              *
 *  Usage:   Routine is called whenver the client window        *
 *           procedure receives a WM_PAINT message              *
 *                                                              *
 *  Method:  -begins painting by calling WinBeginPaint          *
 *              and retriving the HPS for the window            *
 *           -paint page info boxes                             *
 *                                                              *
 *           -ends painting by calling WinEndPaint              *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainPaint(HWND hwnd)
{
   HPS    hps;                          /* Handle for painting           */
   RECTL  rect;                         /* Rectangle struct for painting */
   CHAR   szBuffer[BUFF_SIZE];
   POINTL ptl;

   hps = WinBeginPaint(hwnd, 0L, (PRECTL)&rect);
   WinFillRect(hps, (PRECTL)&rect, CLR_BACKGROUND);
                              /* Fill update rectangle with window color */
   WinQueryWindowRect(hwndMain, (PRECTL)&rect);
   switch (sStatus) {
   case IDM_SEARCH:
      ptl.x = HORZ_INDENT;
      ptl.y = rect.yTop - VERT_TEXT_POS;
      memset(szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, "Searching Path: [");
      strcat(szBuffer, ObjInfo.szCurDriver);
      strcat(szBuffer, ObjInfo.szCurPath);
      strcat(szBuffer, "]");

      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

      ptl.x = HORZ_INDENT;
      ptl.y = rect.yTop - VERT_TEXT_POS * 2;
      memset(szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, "Searching Pattern: [");
      strcat(szBuffer, ObjInfo.szCurPattern);
      strcat(szBuffer, "]");

      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

      break;
   default:
      break;
   }                                                       /* endswitch */
   WinEndPaint(hps);
   return;
}                                                        /* MainPaint() */

/****************************************************************\
 *  Main window WM_COMMAND processing procedure                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainCommand(hwnd, mp1, mp2)                         *
 *                                                              *
 *  Purpose:Calls the appropriate procedures that deal with     *
 *           the selected menu item.                            *
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
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
    switch(SHORT1FROMMP(mp1))
    {
        case IDM_GENERALHELP:
           HelpExtended(mp2);
           return;

        case IDM_USINGHELP:
           HelpHelpForHelp(mp2);
           return;

        case IDM_TUTORIAL:
           HelpKeys(mp2);
           return;

        case IDM_HELPINDEX:
           HelpIndex(mp2);
           return;

        case IDM_HELPPRODUCTINFO:
           HelpAbout(mp2);
           return;

    /*--------------------------------------------------*\
     *      User command processing routine is called   *
     *      here so any ids not procecessed here can be *
     *      processed                                   *
    \*--------------------------------------------------*/
        default:
            UserCommand(hwnd, mp1, mp2);
            break;
    }
    return;
}                                                /* MainCommand() */


/****************************************************************\
 *  Non-standard menu item command processing procedure         *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    UserCommand(hwnd, mp1, mp2)                        *
 *                                                              *
 *  Purpose: Process any WM_COMMAND messages send to hwndMain   *
 *              that are not processed by MainCommand           *
 *                                                              *
 *  Usage:   Routine is called for each WM_COMMAND that is      *
 *           not posted by a standard menu item                 *
 *                                                              *
 *  Method:  A switch statement branches control based upon     *
 *           the id of the control which posted the message     *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID UserCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
   HPS         hps;
   RECTL       rect;
   CHAR        szBuffer[BUFF_SIZE];
   APIRET      Ret;
   POINTL      ptl;

   switch (SHORT1FROMMP(mp1))
   {
       case IDM_SEARCH:
           if (WinDlgBox(HWND_DESKTOP,
                         hwnd,
                         (PFNWP)GetPatternDlgProc,
                         (HMODULE)0,
                         (ULONG)IDD_GETPATTERN,
                         (PVOID)&ObjInfo) )
           {
              sStatus = IDM_SEARCH;
              hps = WinGetPS(hwnd);
              WinQueryWindowRect(hwnd, &rect);
              WinFillRect(hps, (PRECTL)&rect, CLR_BACKGROUND);
              ptl.x = HORZ_INDENT;
              ptl.y = rect.yTop - VERT_TEXT_POS;
              memset(szBuffer, 0, sizeof(szBuffer));
              strcpy(szBuffer, "Searching Path: [");
              strcat(szBuffer, ObjInfo.szNewDriver);
              strcat(szBuffer, ObjInfo.szNewPath);
              strcat(szBuffer, "]");

              GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

              ptl.x = HORZ_INDENT;
              ptl.y = rect.yTop - VERT_TEXT_POS * 2;
              memset(szBuffer, 0, sizeof(szBuffer));
              strcpy(szBuffer, "Searching Pattern: [");
              strcat(szBuffer, ObjInfo.szNewPattern);
              strcat(szBuffer, "]");

              GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);
              WinReleasePS(hps);

              Ret=SearchFile(&ObjInfo, &pFindFile);
              if (Ret)
              {
                  switch(Ret) {
                  case ERROR_FILE_NOT_FOUND:
                       MessageBox(hwnd, IDMSG_FILENOTFOUND, "Warning !",
                                MB_OK | MB_INFORMATION | MB_MOVEABLE, TRUE);
                       break;
                  case ERROR_DRIVE_LOCKED:
                       MessageBox(hwnd, IDMSG_DRIVELOCKED, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       break;
                  case ERROR_PATH_NOT_FOUND:
                       MessageBox(hwnd, IDMSG_PATHNOTFOUND, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       break;
                  case RETURN_ERROR:
                       MessageBox(hwnd, IDMSG_ERRORSETPATH, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       break;
                  case ERROR_BUFFER_OVERFLOW:
                       MessageBox(hwnd, IDMSG_BUFFEROVERFLOW, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       break;
                  case ERROR_INVALID_DRIVE:
                       MessageBox(hwnd, IDMSG_ERRORINVALIDDRIVE, "Error !",
                                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                       break;
                  }                       /* End of switch(Ret)   */
              }                           /* Endif of DisplayFile */
              else
              {
                 memset(ObjInfo.szCurDriver,0,sizeof(ObjInfo.szCurDriver));
                 memset(ObjInfo.szCurPath,0,sizeof(ObjInfo.szCurPath));
                 memset(ObjInfo.szCurPattern,0,sizeof(ObjInfo.szCurPattern));
                 strcpy(ObjInfo.szCurDriver,ObjInfo.szNewDriver);
                 strcpy(ObjInfo.szCurPath,ObjInfo.szNewPath);
                 strcpy(ObjInfo.szCurPattern,ObjInfo.szNewPattern);
                 rect.yTop = rect.yTop-VERT_TEXT_POS*2-5;

                 if(WinDlgBox(HWND_DESKTOP,
                           hwnd,
                           (PFNWP)DispFileDlgProc,
                           (HMODULE)0,
                           (ULONG)IDD_DSPFILESTRUCT,
                           (PVOID)NULL) )

                     DisplayFile(hwnd, rect, &pFlList);
              }
           }                           /* End of case(IDM_SEARCH) */
           break;

       case IDM_OPEN:
           if (WinDlgBox(HWND_DESKTOP,
                         hwnd,
                         (PFNWP)OpenDlgProc,
                         (HMODULE)0,
                         (ULONG)IDD_OPENFILE,
                         (PVOID)&FileInfo) )
           {
              Ret=ReadFileProc(&FileInfo);
              if(Ret)
              {
                 switch(Ret) {
                 case ERROR_PATH_NOT_FOUND:
                      MessageBox(hwnd, IDMSG_PATHNOTFOUND, "Error !",
                               MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                      break;
                 case ERROR_OPEN_FAILED:
                      MessageBox(hwnd, IDMSG_OPENFAILED, "Error !",
                               MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                      break;
                 case ERROR_FILE_NOT_FOUND:
                      MessageBox(hwnd, IDMSG_FILENOTFOUND, "Error !",
                               MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                      break;
                 case ERROR_ACCESS_DENIED:
                      MessageBox(hwnd, IDMSG_ACCESSDENIED, "Error !",
                               MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                      break;
                 default:
                      MessageBox(hwnd, IDMSG_RETURNERROR, "Error !",
                               MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
                      break;
                 }
              }
              else
              {
                 if(!WinDlgBox(HWND_DESKTOP,
                              hwnd,
                              (PFNWP)DisplayDlgProc,
                              (HMODULE)0,
                              (ULONG)IDD_DISPLAYTEXT,
                              (PVOID)&FileInfo) )
                 MessageBox(hwnd, IDMSG_DSPTEXTERROR, "Error !",
                          MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
              }
           }
           break;
       case IDM_CALCUL:
           if (WinDlgBox(HWND_DESKTOP,
                         hwnd,
                         (PFNWP)MathDlgProc,
                         (HMODULE)0,
                         (ULONG)IDD_GETMATHINFO,
                         (PVOID)&MathInfo) )
           {
              if(*(MathInfo.pszOperation) != '\000')
              {
                 CalCulProc(&MathInfo);

                 if(!WinDlgBox(HWND_DESKTOP,
                              hwnd,
                              (PFNWP)ResultDlgProc,
                              (HMODULE)0,
                              (ULONG)IDD_DISPLAYVALUE,
                              (PVOID)&MathInfo) )
                 MessageBox(hwnd, IDMSG_DSPTEXTERROR, "Error !",
                          MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
              }
           }
           break;

       default:
         break;
   }
    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'  *
    \*  warning at compile time.                                      */
   return;
}

/****************************************************************\
 *  Message Box procedure                                       *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MessageBox(hwndOwner,IdMsg,pszMsg,fsStyle,bBeep)   *
 *                                                              *
 *  Purpose: Displays the warning message box with the message  *
 *              given in nIdMsg retrived from the message table *
 *                                                              *
 *  Usage:   Called whever an error occurs and a message wishes *
 *           to be displayed to the user                        *
 *                                                              *
 *  Method:  - Message string is loaded from the process'       *
 *              message table                                   *
 *           - Alarm beep is sounded if desired                 *
 *           - Message box with the message is displayed        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MessageBox(HWND hwndOwner, LONG IdMsg, PSZ pszMsg, LONG fsStyle,
                     BOOL bBeep)
{
   CHAR szText[MESSAGELEN];

   if(!WinLoadMessage(hab, (HMODULE)NULL, IdMsg, MESSAGELEN, (PSZ)szText))
   {
       WinAlarm(HWND_DESKTOP, WA_ERROR);
       return;
   }
   if(bBeep)
       WinAlarm(HWND_DESKTOP, WA_ERROR);

   WinMessageBox(HWND_DESKTOP,
                 hwndOwner,
                 szText,
                 (PSZ)pszMsg,
                 IDM_MSGBOX,
                 fsStyle);
   return;
}                                                  /* MessageBox() */

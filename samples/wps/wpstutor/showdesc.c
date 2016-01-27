/*************************************************************************
*
*  File Name  : SHOWDESC.C
*
*  Description: Show Method Description Sample Program
*
*               This program displays the names and descriptions for Workplace
*               Shell methods in a multiline entry field window.  This sample
*               program is invoked by the WPSTUTOR.DLL Workplace Shell class.
*               For every method that is invoked in that class, information is
*               sent to this program (via a named pipe) that indicates the
*               method name and description to be displayed.
*
*
*  Copyright (C) 1993 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
************************************************************************/

/*-----------------------------------------------------------------------------
*    INCLUDE FILES
*----------------------------------------------------------------------------*/
#define INCL_DOS
#define INCL_ERRORS
#define INCL_WINCURSORS
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINMLE
#define INCL_WINSTDFILE
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINWINDOWMGR
#define INCL_WINMENUS

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "showdesc.h"

/*-----------------------------------------------------------------------------
*    DEFINES
*----------------------------------------------------------------------------*/
#define RETURN_SUCCESS      0            /* Indicates successful program     */
                                         /*   execution                      */
#define RETURN_ERROR        1            /* Indicates unsuccessful program   */
                                         /*   execution                      */

/*-----------------------------------------------------------------------------
*    GLOBAL DATA
*----------------------------------------------------------------------------*/
HWND hwndMainFrame;                 /* Main frame window handle              */
HWND hwndMain;                      /* Main client window handle             */
HWND hwndMLE;                       /* Multiline entry-field window handle   */

HAB hab;                            /* Anchor block for the process          */
HMQ hmq;                            /* Handle to the process' message queue  */

CHAR szAppName[MAXNAMEL];           /* Application name buffer               */

LONG lClrDefaultForeground = CLR_BLACK;
                                    /* Foreground color for MLE window text  */
LONG lClrDefaultBackground = CLR_WHITE;
                                    /* Background color for MLE window text  */

IPT iptOffset = 0;                  /* MLE insert position that represents   */
                                    /*    the point at which the method name */
                                    /*    and description text is to be      */
                                    /*    inserted into the MLE              */


/******************************************************************************
*
*  Name:        main
*
*  Description: Entry point of program.
*
*               This function initializes the required PM resources in order
*               to run the 'Show Method Description' program.
*
*  Parameters:  NONE
*
*  Returns:     RETURN_SUCCESS - Program execution completed successfully
*               RETURN_ERROR   - Error occurred during execution
*
******************************************************************************/
int main (VOID)
{
   QMSG      qmsg;             /* Message structure used to get/dispatch     */
                               /*   messages                                 */
   ULONG     ulCtlData;        /* Frame window creation flags                */
   SWCNTRL   SwitchControls;   /* Switch data for a new Window List entry    */
   PID       pid;              /* Process ID of the thread that created the  */
                               /*   frame window                             */
   TID       tid;              /* Thread ID of the thread that created the   */
                               /*   frame window                             */

   /*--------------------------------------------------------------------------
   *    Obtain an anchor block for the PM program.  The program ends
   *    unsuccessfully if an anchor block cannot be obtained.
   *-------------------------------------------------------------------------*/
   hab = WinInitialize(0);
   if(!hab)
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return(RETURN_ERROR);
   }

   /*--------------------------------------------------------------------------
   *    Obtain a message queue for the PM program.  The program ends
   *    unsuccessfully if a message queue cannot be obtained.
   *-------------------------------------------------------------------------*/
   hmq = WinCreateMsgQueue(hab, 0);
   if(!hmq)
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }

   /*--------------------------------------------------------------------------
   *    Set up the exit list processing for the program, get the application's
   *    name, and register the window class with PM.  The program ends
   *    unsuccessfully if this initialization fails.
   *-------------------------------------------------------------------------*/
   if ( !Init() )
   {
      MessageBox(HWND_DESKTOP, IDMSG_INITFAILED, MB_OK | MB_ERROR, TRUE);
      return(RETURN_ERROR);
   }

   /*--------------------------------------------------------------------------
   *    NOTE:  Program clean up is handled by the DosExitList processing from
   *    this point forward.
   *-------------------------------------------------------------------------*/

   /*--------------------------------------------------------------------------
   *    Create the frame and client windows.
   *-------------------------------------------------------------------------*/
   ulCtlData = FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_SIZEBORDER |
               FCF_ICON | FCF_SHELLPOSITION | FCF_MENU | FCF_ACCELTABLE;

   hwndMainFrame = WinCreateStdWindow(
      HWND_DESKTOP,                           /* Parent-window handle        */
      0,                                      /* Frame-window style          */
      (PVOID)&ulCtlData,                      /* Frame-creation flags        */
      (PSZ)szAppName,                         /* Client-window class name    */
      (PSZ)szAppName,                         /* Title-bar text              */
      0,                                      /* Client-window style         */
      (HMODULE)NULL,                          /* Resource identifier         */
      IDR_MAIN,                               /* Frame-window identifier     */
      (PHWND)&hwndMain);                      /* Client-window handle        */

   /*--------------------------------------------------------------------------
   *    We'll position the window based on the location stored from previous
   *    program executions.  NOTE: If there is no stored location, the window
   *    position will be determined by the shell due to the FCF_SHELLPOSITION
   *    frame creation flag.
   *-------------------------------------------------------------------------*/
   WinRestoreWindowPos(szAppName, szAppName, hwndMainFrame);

   /*--------------------------------------------------------------------------
   *    Add the 'Show Method Description' program to the Window List.
   *-------------------------------------------------------------------------*/
   WinQueryWindowProcess(hwndMainFrame, &pid, &tid);

   SwitchControls.hwnd = hwndMainFrame;
   SwitchControls.hwndIcon = 0;
   SwitchControls.hprog = NULLHANDLE;
   SwitchControls.idProcess = pid;
   SwitchControls.idSession = NULLHANDLE;
   SwitchControls.uchVisibility = SWL_VISIBLE;
   SwitchControls.fbJump = SWL_JUMPABLE;
   strcpy(SwitchControls.szSwtitle, szAppName);
   SwitchControls.bProgType = SSF_TYPE_PM;

   WinAddSwitchEntry(&SwitchControls);

   /*--------------------------------------------------------------------------
   *    Process the get/dispatch message loop.
   *-------------------------------------------------------------------------*/
   while(WinGetMsg(hab, (PQMSG)&qmsg, 0, 0, 0))
       WinDispatchMsg(hab, (PQMSG)&qmsg);

   /*--------------------------------------------------------------------------
   *    The program has completed successfully.
   *-------------------------------------------------------------------------*/
   return(RETURN_SUCCESS);
}


/******************************************************************************
*
*  Name:        MainWndProc
*
*  Description: Window procedure for the main client window.
*
*               This function processes the messages sent to the 'Show
*               Method Description' client window.
*
*  Parameters:  hwnd - Window handle to which message is addressed
*               msg  - Message type
*               mp1  - First message parameter
*               mp2  - Second message parameter
*
*  Returns:     Return values are based on the message processed.
*
******************************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{

   /*--------------------------------------------------------------------------
   *    Process the message that caused the window procedure to be invoked.
   *-------------------------------------------------------------------------*/
   switch(msg)
   {
      case WM_CREATE:
         /*--------------------------------------------------------------------
         *    Initialize the PM window by creating the MLE window and creating
         *    the thread that communicates with WPSTUTOR.DLL.
         *-------------------------------------------------------------------*/
         return(InitMainWindow(hwnd, mp1, mp2));
         break;

      case WM_PAINT:
      {
         RECTL rclUpdate;        /* Rectangle to be painted                  */
         HPS   hps;              /* Presentation space handle                */

         /*--------------------------------------------------------------------
         *    Repaint the window to be updated.
         *-------------------------------------------------------------------*/
         hps = WinBeginPaint(hwnd, NULLHANDLE, (PRECTL)&rclUpdate);
         WinFillRect(hps, (PRECTL)&rclUpdate, SYSCLR_WINDOW);
         WinEndPaint(hps);
         break;
      }

      case WM_SIZE:
         /*--------------------------------------------------------------------
         *    Re-size the MLE to be the same width and height as the client
         *    window.
         *-------------------------------------------------------------------*/
         WinSetWindowPos(hwndMLE, HWND_TOP, 0, 0, SHORT1FROMMP(mp2),
                         SHORT2FROMMP(mp2), SWP_SIZE);
         break;

      case WM_INITMENU:
      {
         /*--------------------------------------------------------------------
         *    Process the menu to be activated as necessary.
         *-------------------------------------------------------------------*/
         switch(SHORT1FROMMP(mp1))
         {
            case IDM_EDIT:
            {
               /*--------------------------------------------------------------
               *    Enable and disable the menu items in the "Edit" menu as
               *    appropriate.
               *-------------------------------------------------------------*/
               BOOL    bEnable;   /* Flag indicating if a menu item should   */
                                  /*   enabled or not                        */
               MRESULT mr1,       /* Result of a WinSendMsg call             */
                       mr2;       /* Result of a WinSendMsg call             */
               IPT     iptText;   /* Length of the text currently in the MLE */


               /*--------------------------------------------------------------
               *    Determine the minimum and maximum insertion points of
               *    selection.  If there is text selected, enable the 'Copy'
               *    and 'Clear' menu items.  If no text is selected, disable
               *    the items.
               *-------------------------------------------------------------*/
               mr1 = WinSendMsg(hwndMLE, MLM_QUERYSEL,
                                 MPFROMSHORT(MLFQS_MINSEL), NULL);

               mr2 = WinSendMsg(hwndMLE, MLM_QUERYSEL,
                                 MPFROMSHORT(MLFQS_MAXSEL), NULL);

               if (mr1 != mr2)
                  bEnable = TRUE;
               else
                  bEnable = FALSE;

               WinEnableMenuItem(HWNDFROMMP(mp2), IDM_EDITCOPY, bEnable);
               WinEnableMenuItem(HWNDFROMMP(mp2), IDM_EDITCLEAR, bEnable);

               iptText = (IPT) WinSendMsg(hwndMLE, MLM_QUERYTEXTLENGTH, NULL,
                                          NULL);

               /*--------------------------------------------------------------
               *    Determine if there is any text in the MLE.  If there is,
               *    enable the 'Clear All' menu item.  Otherwise, disable the
               *    item.
               *-------------------------------------------------------------*/
               if ( iptText != 0 )
                  bEnable = TRUE;
               else
                  bEnable = FALSE;

               WinEnableMenuItem(HWNDFROMMP(mp2), IDM_EDITCLEARALL, bEnable);

               break;
            }

            default:
                break;
         }
         break;
      }

      case WM_COMMAND:
      {
         /*--------------------------------------------------------------------
         *    Process the command selected by the user.
         *-------------------------------------------------------------------*/
         switch ( SHORT1FROMMP(mp1) )
         {
            case IDM_EDITCLEAR:
            {
               /*--------------------------------------------------------------
               *    Remove the selected text from the MLE.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_CLEAR, NULL, NULL);
               break;
            }

            case IDM_EDITCLEARALL:
            {
               /*--------------------------------------------------------------
               *    Remove all text from the MLE.
               *
               *    Disable redrawing of the MLE so the text doesn't "flash"
               *    when the MLE is cleared.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_DISABLEREFRESH, NULL, NULL);

               /*--------------------------------------------------------------
               *    Clear the MLE by selecting all of the text and clearing it.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_SETSEL, MPFROMSHORT(NULL),
                          (MPARAM) WinSendMsg(hwndMLE, MLM_QUERYTEXTLENGTH,
                                              NULL, NULL));

               WinSendMsg(hwndMLE, MLM_CLEAR, NULL, NULL);

               /*--------------------------------------------------------------
               *    Reset the insertion point used to display the method names
               *    and descriptions in the MLE.
               *-------------------------------------------------------------*/
               iptOffset = 0;

               /*--------------------------------------------------------------
               *    Reset the changed flag for the MLE.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT(FALSE), NULL);

               /*--------------------------------------------------------------
               *    Enable redrawing of the MLE.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_ENABLEREFRESH, NULL, NULL);
               break;
            }

            case IDM_EDITCOPY:
            {
               /*--------------------------------------------------------------
               *    Copy the selected text to the clipboard.
               *-------------------------------------------------------------*/
               WinSendMsg(hwndMLE, MLM_COPY, NULL, NULL);
               break;
            }

            default :
            {
               break;
            }
         }
         break;
      }

      case IDM_SHOW_METHOD:
      {
         /*--------------------------------------------------------------------
         *    Information was sent from WPSTUTOR.DLL to this program.  This
         *    data represents a method for which the name and description is
         *    to be displayed.
         *-------------------------------------------------------------------*/
         ShowMethodDescription(LONGFROMMP(mp1));

         break;
      }

      case WM_CLOSE:
      {
         /*--------------------------------------------------------------------
         *    This message is sent to a frame window to indicate that the
         *    window is being closed by the user.
         *
         *    Store the window position for the next time around and close the
         *    window.
         *-------------------------------------------------------------------*/
         WinStoreWindowPos(szAppName, szAppName, hwndMainFrame);
         WinPostMsg(hwnd, WM_QUIT, (MPARAM)0, (MPARAM)0);
         break;
      }

      default:
         /*--------------------------------------------------------------------
         *    We're not processing any of the messages that get this far.
         *    We'll tell the system to process these messages by calling
         *    WinDefWindowProc.
         *-------------------------------------------------------------------*/
         return(WinDefWindowProc(hwnd, msg, mp1, mp2));
         break;

   }

   /*--------------------------------------------------------------------------
   *    All window procedures should return '0' as the default.
   *-------------------------------------------------------------------------*/
   return (MRESULT)0;
}

/******************************************************************************
*
*  Name:        MessageBox
*
*  Description: This function displays a warning message box with a specified
*               message that is retrieved from the message table.  This
*               function is called when an error that the user should know
*               about occurs.
*
*
*  Parameters:  hwndOwner - Window handle of the owner of the message box
*               idMsg     - ID of message to be retrieved from resource file
*               fsStyle   - Message box style
*               fBeep     - Flag to sound alarm
*
*  Returns:     SHORT - See WinMessageBox return codes for more information.
*
******************************************************************************/

SHORT MessageBox(HWND hwndOwner, SHORT idMsg, SHORT fsStyle, BOOL fBeep)
{
   CHAR szText[MESSAGELEN];
   CHAR szTitle[MESSAGELEN];
   PSZ  pszTitle = NULL;

   /*--------------------------------------------------------------------------
   *    If we can't load the message from the message table, beep for the user.
   *-------------------------------------------------------------------------*/
   if ( !WinLoadMessage(hab, (HMODULE)NULL, idMsg, MESSAGELEN, (PSZ)szText) )
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return MBID_ERROR;
   }

   /*--------------------------------------------------------------------------
   *    If the caller wants a beep, then beep.
   *-------------------------------------------------------------------------*/
   if ( fBeep )
      WinAlarm(HWND_DESKTOP, WA_ERROR);

   /*--------------------------------------------------------------------------
   *    Load the title of the message box from the string table.
   *-------------------------------------------------------------------------*/
   if ( (fsStyle & MB_WARNING)
       && WinLoadString(hab, (HMODULE)0, IDS_WARNING, MESSAGELEN, szTitle) )
   {
         pszTitle = szTitle;
   }
   else
      pszTitle = NULL;

   /*--------------------------------------------------------------------------
   *    Display the message box for the user.
   *-------------------------------------------------------------------------*/
   return(WinMessageBox(HWND_DESKTOP, hwndOwner, szText, pszTitle,
          IDD_MSGBOX, fsStyle));
}

/******************************************************************************
*
*  Name:        Init
*
*  Description: This function performs initialization functions, including
*               setting up exit list processing, obtaining the name of the
*               application to be started, and registering the main client's
*               window class.
*
*  Parameters:  NONE
*
*  Returns:     TRUE  - Initialization is successful
*               FALSE - Error occurred during initialization.
*
******************************************************************************/

BOOL Init( VOID )
{
    /*-------------------------------------------------------------------------
    *    Add 'ExitProc' to the exit list to handle exit processing.  If there
    *    is an error, then terminate the process since there have not been any
    *    resources allocated yet.
    *------------------------------------------------------------------------*/
    if ( DosExitList(EXLST_ADD, (PFNEXITLIST)ExitProc) )
    {
       MessageBox(HWND_DESKTOP,
                  IDMSG_CANNOTLOADEXITLIST,
                  MB_OK | MB_ERROR,
                  TRUE);

       return(FALSE);
    }

    /*-------------------------------------------------------------------------
    *    Load the application's name from the resource file.
    *------------------------------------------------------------------------*/
    if ( !WinLoadString(hab, (HMODULE)0, IDS_APPNAME, MAXNAMEL, szAppName) )
    {
       return FALSE;
    }

    /*-------------------------------------------------------------------------
    *    Register the main client window class
    *------------------------------------------------------------------------*/
    if ( !WinRegisterClass(hab,
                           (PSZ)szAppName,
                           (PFNWP)MainWndProc,
                           CS_SIZEREDRAW | CS_CLIPCHILDREN,
                           0) )
    {
       return FALSE;
    }

    return TRUE;
}

/******************************************************************************
*
*  Name:        InitMainWindow
*
*  Description: This function performs the initialization required to create
*               the main window.
*
*  Parameters:  hwnd - Client window handle
*               mp1 - First message parameter
*               mp2 - Second message parameter
*
*  Returns:     TRUE  - Window initialization failed.
*               FALSE - Window initialization is successful.
*
******************************************************************************/

MRESULT InitMainWindow( HWND hwnd, MPARAM mp1, MPARAM mp2 )
{
    RECTL     rcl;        /* Window rectangle used to determine the size     */
                          /*   of the new MLE                                */
    TID       ThreadID;   /* Thread ID for the thread that communicates with */
                          /*   WSPTUT.DLL                                    */

    /*-------------------------------------------------------------------------
    *    Create a multiline entry-field (MLE) window to be the same size as
    *    the client.  The MLE is used to display the method names and
    *    descriptions.
    *------------------------------------------------------------------------*/
    WinQueryWindowRect(hwnd, (PRECTL)&rcl);

    hwndMLE = WinCreateWindow(
       hwnd,                           /* Parent-window handle               */
       WC_MLE,                         /* Registered-class name              */
       (PSZ)NULL,                      /* Window text                        */
       MLS_READONLY | MLS_VSCROLL |    /* Window style                       */
         MLS_WORDWRAP | WS_VISIBLE,
       rcl.xLeft,                      /* X-coordinate of the window position*/
       rcl.yBottom,                    /* Y-coordinate of the window position*/
       rcl.xRight,                     /* Width of window                    */
       rcl.yTop,                       /* Height of window                   */
       hwnd,                           /* Owner-window handle                */
       HWND_TOP,                       /* Sibling-window handle              */
       ID_MLE,                         /* Window identifier                  */
       NULL,                           /* Pointer to control data            */
       NULL);                          /* Window handle                      */

    if (!hwndMLE)
        return (MRESULT)TRUE;

    /*-------------------------------------------------------------------------
    *    Set the foreground and background colors for the MLE window.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_SETTEXTCOLOR, MPFROMLONG(lClrDefaultForeground),
               NULL);
    WinSendMsg(hwndMLE, MLM_SETBACKCOLOR, MPFROMLONG(lClrDefaultBackground),
               NULL);

    /*-------------------------------------------------------------------------
    *    Reset the undo state of the MLE.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_RESETUNDO, NULL, NULL);

    /*-------------------------------------------------------------------------
    *    Set the import/export format for the MLE to use LF for line
    *    delineation.  This format guarantees that any text imported into the
    *    MLE in this format can be recovered in exactly the same form on
    *    export.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_FORMAT, MPFROMSHORT(MLFIE_NOTRANS), NULL);

    /*-------------------------------------------------------------------------
    *    Reset the MLE changed flag.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT((BOOL)FALSE), NULL);

    /*-------------------------------------------------------------------------
    *    Create the thread used to communicate with WPSTUTOR.DLL via a named
    *    pipe.
    *------------------------------------------------------------------------*/
    DosCreateThread(&ThreadID, (PFNTHREAD)MonitorMethodQueue, 0L, 0L,
                    STACKSIZE);

    /*-------------------------------------------------------------------------
    *    Return FALSE to continue window creation and TRUE to abort it.
    *------------------------------------------------------------------------*/
    return (MRESULT)FALSE;

    /*-------------------------------------------------------------------------
    *    This routine currently doesn't use the mp1 and mp2 parameters so
    *    it is referenced here to prevent an 'Unreferenced Parameter'
    *    warning at compile time.
    *------------------------------------------------------------------------*/
    mp1;
    mp2;
}

/******************************************************************************
*
*  Name:        ExitProc
*
*  Description: This function cleans up program resources when the
*               application terminates.
*
*  Parameters:  usTermCode - Program termination code
*
*  Returns:     NONE
*
******************************************************************************/

VOID APIENTRY ExitProc(USHORT usTermCode)
{
    /*-------------------------------------------------------------------------
    *    Destory the main frame window if it exists.
    *------------------------------------------------------------------------*/
    if (WinIsWindow(hab, hwndMainFrame))
       WinDestroyWindow(hwndMainFrame);

    /*-------------------------------------------------------------------------
    *    Any other system resources used should be freed here.
    *------------------------------------------------------------------------*/

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

    /*-------------------------------------------------------------------------
    *    Complete termination of the program.
    *------------------------------------------------------------------------*/
    DosExitList(EXLST_EXIT, (PFNEXITLIST)ExitProc);

    /*-------------------------------------------------------------------------
    *    This routine currently doesn't use the usTermCode parameter so
    *    it is referenced here to prevent an 'Unreferenced Parameter'
    *    warning at compile time.
    *------------------------------------------------------------------------*/
    usTermCode;
}

/******************************************************************************
*
*  Name:   ShowMethodDescription
*
*  Description: This function builds a string to be displayed in the MLE that
*               has the following format:
*
*                  <method name><separator><method description><break>
*
*               The pieces of the string are obtained from the message table.
*               This string is appended to the application's MLE.
*
*  Parameters:  ulMethodID - ID that indicates the method name and description
*                            to be displayed
*
*  Returns:     TRUE  - Successful execution.
*               FALSE - Error occurred during processing.
*
******************************************************************************/

MRESULT ShowMethodDescription(ULONG ulMethodID)
{
    ULONG ulNumBytes;                        /* Number of bytes appended to  */
                                             /*   the MLE window             */
    CHAR  szMethodDescription[METHODLENGTH]; /* Method description to be     */
                                             /*   displayed                  */
    CHAR  szMethodName[TITLELENGTH +
                       TITLESEPARATORLEN +
                       METHODLENGTH +
                       (2 * HARD_BREAK_LENGTH)];
                                             /* String to be appended to the */
                                             /*   MLE window                 */
    CHAR  szMethodSep[TITLESEPARATORLEN];    /* Buffer for text used to      */
                                             /*   separate method name and   */
                                             /*   description                */
    CHAR  szHardBreak[HARD_BREAK_LENGTH];    /* Buffer for text used to      */
                                             /*   separate method            */
                                             /*   descriptions               */

    /*-------------------------------------------------------------------------
    *    Disable redrawing of the MLE so the text doesn't "flash" when the MLE
    *    is updated.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_DISABLEREFRESH, NULL, NULL);

    /*-------------------------------------------------------------------------
    *    Get the name of the method to be displayed based on the data sent
    *    from WPSTUTOR.DLL.  If the method name is not found, load an
    *    "Unidentified Method" text string.
    *------------------------------------------------------------------------*/
    if ( !WinLoadMessage(hab, (HMODULE)0, (ulMethodID), TITLELENGTH,
                         szMethodName) )
    {
       WinLoadMessage(hab, (HMODULE)0, IDM_UNIDENTIFIED_METHOD, TITLELENGTH,
                      szMethodName);
    }

    /*-------------------------------------------------------------------------
    *    Load the text used to separate the method name and its description,
    *    and append that text to the method name buffer.
    *------------------------------------------------------------------------*/
    if ( !WinLoadString(hab, (HMODULE)0, IDS_TITLEBARSEPARATOR,
                        TITLESEPARATORLEN, szMethodSep) )
    {
       return FALSE;
    }

    strcat(szMethodName, szMethodSep);

    /*-------------------------------------------------------------------------
    *    Get the name of the description to be displayed.  If the method name
    *    is not found, load an "There is no description available for this
    *    method."  text string.  Append that text to the method name buffer.
    *------------------------------------------------------------------------*/
    if ( !WinLoadMessage(hab, (HMODULE)0, (ulMethodID + 1), METHODLENGTH,
                         szMethodDescription) )
    {
        WinLoadMessage(hab, (HMODULE)0, IDM_UNIDENTIFIED_METHOD_MSG,
                       METHODLENGTH, szMethodDescription);
    }

    strcat(szMethodName, szMethodDescription);

    /*-------------------------------------------------------------------------
    *    Load the text used to separate the method descriptions, and append it
    *    to the method name buffer.
    *------------------------------------------------------------------------*/
    if ( !WinLoadString(hab, (HMODULE)0, IDS_HARD_BREAK, HARD_BREAK_LENGTH,
                        szHardBreak) )
    {
       return FALSE;
    }

    strcat(szMethodName, szHardBreak);
    strcat(szMethodName, " ");
    strcat(szMethodName, szHardBreak);

    /*-------------------------------------------------------------------------
    *   Set the transfer buffer for the MLE to be the method name buffer.
    *------------------------------------------------------------------------*/
    (BOOL) WinSendMsg(hwndMLE, MLM_SETIMPORTEXPORT,
                      MPFROMP((PBYTE)szMethodName),
                      MPFROMSHORT(strlen(szMethodName)));

    /*-------------------------------------------------------------------------
    *    Import the text using the insertion point offset we're tracking.
    *    If none of the information was set into the MLE, exit the function.
    *------------------------------------------------------------------------*/
    ulNumBytes = (ULONG) WinSendMsg(hwndMLE, MLM_IMPORT, MPFROMP(&iptOffset),
                                    MPFROMSHORT(strlen(szMethodName)));

    if ( !ulNumBytes )
    {
       return FALSE;
    }

    /*-------------------------------------------------------------------------
    *    Update the insertion point offset by the number of bytes that were
    *    added to the MLE.
    *------------------------------------------------------------------------*/
    iptOffset = iptOffset + ulNumBytes + 1;

    /*--------------------------------------------------------------
    *    Reset the changed flag for the MLE.
    *-------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_SETCHANGED, MPFROMSHORT(FALSE), NULL);

    /*-------------------------------------------------------------------------
    *    Enable redrawing of the MLE.
    *------------------------------------------------------------------------*/
    WinSendMsg(hwndMLE, MLM_ENABLEREFRESH, NULL, NULL);

    /*-------------------------------------------------------------------------
    *    Make the main frame window visible so the user can see that the MLE
    *    has been updated.
    *------------------------------------------------------------------------*/
    WinShowWindow(hwndMainFrame, TRUE);

    return (MRESULT)TRUE;
}

/******************************************************************************
*
*  Name:        MonitorMethodQueue
*
*  Description: This function is started as a thread.  This thread monitors the
*               named pipe used to communicate with the WPSTutorial Object Class
*               (WPSTUTOR.DLL).  After the pipe is opened, the function
*               continually waits for a method ID from the object class.
*               When an ID is sent via the named pipe, a message is posted to
*               the application's client window so that the corresponding
*               method's name and description will be displayed to the user.
*
*  Parameters:  NONE
*
*  Returns:     NONE
*
******************************************************************************/

VOID MonitorMethodQueue(VOID)
{
   HPIPE       hNPipe;           /* Named pipe handle                        */
   ULONG       ulActionTaken;    /* Describes action taken by DosOpen        */
   ULONG       ulMethodID;       /* ID representing WPS method information   */
                                 /*   to be displayed                        */
   ULONG       ulBytesRead;      /* Number of bytes read off the named pipe  */
   APIRET      rc;               /* Return code from DosXXX function         */

   /*--------------------------------------------------------------------------
   *    Open the named pipe used to communicate with WPSTUTOR.DLL.
   *-------------------------------------------------------------------------*/
   rc = DosOpen(PIPE_NAME,
                &hNPipe,
                &ulActionTaken,
                0,
                FILE_NORMAL,
                OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE,
                (PEAOP2)NULL);

   /*--------------------------------------------------------------------------
   *    We'll continue to read method IDs from the named pipe and pass them
   *    directly to the window procedure.
   *-------------------------------------------------------------------------*/
   while (!rc)
   {
      /*-----------------------------------------------------------------------
      *    Get the ID of the method description to be displayed.
      *----------------------------------------------------------------------*/
      rc = DosRead(hNPipe,
                   &ulMethodID,
                   sizeof(ulMethodID),
                   &ulBytesRead);

      if ( !rc )
      {
         /*--------------------------------------------------------------------
         *    We were able to read something from the named pipe, so we'll pass
         *    the information to the window procedure.
         *-------------------------------------------------------------------*/
         WinPostMsg(hwndMain, IDM_SHOW_METHOD, MPFROMLONG(ulMethodID),
                    (MPARAM)NULL);
      }
   }
}

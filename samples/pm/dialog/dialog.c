/*************************************************************************
*
*  File Name   : Dialog.c
*
*  Description : This program demonstrates how to associate a pop-up
*                window with a standard window.  The pop-up window
*                is defined as a dialog template in a resource file.
*
*  Concepts    : This program displays a standard window.  The action bar
*                contains a single choice:  Options.  The Options
*                pull-down contains a choice that displays a pop-up
*                window which contains an entry field, and OK and Cancel
*                pushbuttons.  When the user types a place name in the
*                entry field and selects OK, the pop-up window
*                disappears, and the place name is displayed at the end
*                of the text "You live in".
*
*  API's       :
*  -------------
*  GpiCharStringAt       WinDestroyWindow    WinInvalidateRegion
*  GpiSetColor           WinDismissDlg       WinLoadString
*  WinBeginPaint         WinDispatchMsg      WinPostMsg
*  WinCreateMsgQueue     WinDlgBox           WinQueryWindowText
*  WinCreateStdWindow    WinEndPaint         WinRegisterClass
*  WinDefDlgProc         WinFillRect         WinSendMsg
*  WinDefWindowProc      WinGetMsg           WinTerminate
*  WinDestroyMsgQueue    WinInitialize       WinWindowFromID
*
*  Copyright (C) 1992 IBM Corporation
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

#define INCL_WINSYS                     /* Selectively include          */
#define INCL_WINFRAMEMGR                /* relevant parts of            */
#define INCL_WINMENUS                   /* the PM header file           */
#define INCL_WINDIALOGS
#define INCL_GPI

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dialog.h"                     /* Resource symbolic identifiers*/

#define LENGTH_STRING  33               /* Length of string             */
#define RETURN_ERROR    1               /* return value for exit        */
#define BEEP_WARN_FREQ      60          /* frequency of warning beep    */
#define BEEP_WARN_DUR      100          /* duration of warning beep     */
#define MAXITEMS      8                 /* numer of menu items in FCF_SYSMENU*/

/* Function prototypes                                                  */
int main(VOID);
MRESULT EXPENTRY MyWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY MyDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY ProdInfoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID DisplayMessage( PCH str );
VOID SetSysMenu(HWND hwnd);
                                        /* Define parameters by type    */
HAB   hab;                              /* Anchor block handle          */
HMQ   hmq;                              /* Message queue handle         */
HWND  hwndClient;                       /* Client window handle         */
ULONG flCreate = FCF_STANDARD;          /* Frame control flag           */
BOOL  bComplete = FALSE;                /* Switch for first time through*/
HWND  hwndFrame;                        /* Frame window handle          */
CHAR  szLocation[LENGTH_STRING] = "";   /* To hold text entered by user */
CHAR  szResponse[LENGTH_STRING] = "";   /* To hold "You live in"        */

/****************************************************************
 *  Name:   main()
 *
 *  Description: Entry point of program.
 *
 *  Concepts: Obtains anchor block handle and creates message
 *            queue.  Calls the initialization routine.
 *            Creates the main frame window which creates the
 *            main client window.  Polls the message queue
 *            via Get/Dispatch Msg loop.  Upon exiting the
 *            loop, exits.
 *
 *  API's   :  WinInitilize
 *             DosBeep
 *             WinCreateMsgQueue
 *             WinTerminate
 *             WinRegisterClass
 *             WinCreateStdWindow
 *             WinSetWindowText
 *             WinGetMsg
 *             WinDispatchMsg
 *             WinDestroyWindow
 *             WinDestroyMsgQueue
 *
 *  Parameters: None
 *
 *  Returns:
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/
int main(VOID)
{
  QMSG  qmsg;
  BOOL success;

  hab = WinInitialize( 0 );
  if(!hab)
  {
    DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
    exit(RETURN_ERROR);
  }

  hmq = WinCreateMsgQueue( hab, 0 );
  if(!hmq)
  {
    DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
    WinTerminate(hab);
    exit(RETURN_ERROR);
  }

  success = WinDlgBox( HWND_DESKTOP,    /* Place anywhere on desktop    */
                       HWND_DESKTOP,    /* Owned by desk top            */
                       (PFNWP)ProdInfoDlgProc,   /* Addr. of procedure  */
                       (HMODULE)0,      /* Module handle                */
                       ID_PRODINFO,     /* Dialog identifier in resource*/
                       NULL);           /* Initialization data          */
  if (!success)
  {
    DisplayMessage("Could Not Register Class");
    exit(RETURN_ERROR);
  }

  success = WinRegisterClass(           /* Register window class        */
       hab,                             /* Anchor block handle          */
       "MyWindow",                      /* Window class name            */
       MyWindowProc,                    /* Address of window procedure  */
       CS_SIZEREDRAW,                   /* Class style                  */
       0                                /* No extra window words        */
       );
  if (!success)
  {
    DisplayMessage("Could Not Register Class");
    exit(RETURN_ERROR);
  }

  hwndFrame = WinCreateStdWindow(
        HWND_DESKTOP,                   /* Desktop window is parent     */
        WS_VISIBLE,                     /* Frame style                  */
        &flCreate,                      /* Frame control flag           */
        "MyWindow",                     /* Window class name            */
        "",                             /* Window title                 */
        0L,                             /* Client style of VISIBLE      */
        (HMODULE)NULL,                  /* Resource is in .EXE file     */
        ID_MAINWND,                     /* Frame window identifier      */
        &hwndClient);                   /* Client window handle         */

  if (!hwndFrame)
  {
    DisplayMessage("Could Not Create Main Window");
    exit(RETURN_ERROR);
  }

  WinSetWindowText(hwndFrame, "Dialog - Sample Program");

/*
 * Get and dispatch messages from the application message queue
 * until WinGetMsg returns FALSE, indicating a WM_QUIT message.
 */
  while( WinGetMsg( hab, &qmsg, (HWND)NULL, 0, 0 ) )
    WinDispatchMsg( hab, &qmsg );

  WinDestroyWindow( hwndFrame );        /* Tidy up...                   */
  WinDestroyMsgQueue( hmq );            /* and                          */
  WinTerminate( hab );                  /* terminate the application    */
  return(0);
}
/***********************  End of main procedure  ************************/

/****************************************************************
 *  Name:   MyWndProc
 *
 *  Description : Window procedure for the main clent window.
 *
 *  Concepts : Processes the messages sent to the main client
 *             window.
 *
 *  API's : WinLoadString
 *          WinDlgBox
 *          WinInvalidateRegion
 *          WinPostMsg
 *          WinDefDlgProc
 *          WinBeginPaint
 *          WinFillRect
 *          GpiCharStringAt
 *          GpiSetColor
 *          WinEndPaint
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns:  Return values are determined by each message
 *
 ****************************************************************/
MRESULT EXPENTRY MyWindowProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg )
  {
    case WM_CREATE:
      /*
       * Window initialization - load "You live in" string from resource
       */
      WinLoadString( hab,
                     (HMODULE)NULL,        /* Resource is in .EXE file  */
                     ID_RESPONSE,          /* String identifier         */
                     sizeof( szResponse ), /* Size of buffer            */
                     (PSZ)szResponse);     /* Buffer                    */
      break;
    case WM_COMMAND:
      /*
       * When the user chooses "Where..." from the Options pull-down,
       * the application displays a dialog box. WinDlgBox calls the
       * dialog procedure below. WinDlgBox does not return until the
       * dialog procedure returns.
       * WinInvalidateRegion sends a WM_PAINT message.
       */
      switch ( SHORT1FROMMP( mp1 ) )
      {
        case ID_WHERE:

         WinDlgBox( HWND_DESKTOP,       /* Place anywhere on desktop    */
                     hwndFrame,         /* Owned by frame               */
                     MyDlgProc,         /* Address of dialog procedure  */
                     (HMODULE)0,        /* Module handle                */
                     ID_MYDIALOG,       /* Dialog identifier in resource*/
                     NULL);             /* Initialization data          */

          WinInvalidateRegion( hwnd, NULLHANDLE, FALSE ); /* Force a repaint */

          break;
        case ID_EXITPROGRAM:

          WinPostMsg( hwnd, WM_CLOSE, NULL, NULL);

          break;
        default:
      return WinDefWindowProc( hwnd, msg, mp1, mp2 );
      }
      break;
    case WM_PAINT:
      /*
       * Window contents are drawn here. First time through, bComplete
       * is FALSE, so window is simply filled with SYSCLR_WINDOW.
       * On subsequent passes, if bComplete has been set to TRUE in the
       * dialog procedure, GpiCharStringAt draws the text.
       */
      {
       HPS    hps;                      /* Presentation space handle    */
       POINTL pt;                       /* String screen coordinates    */
       RECTL  rc;                       /* Window rectangle             */

        hps = WinBeginPaint( hwnd, (HPS)NULL, &rc );
        WinFillRect( hps, &rc, SYSCLR_WINDOW );
        if( bComplete)
        {
          GpiSetColor( hps, CLR_NEUTRAL ); /* Text color same as PM       */
          pt.x = 50L; pt.y = 50L;         /* Coordinates of "You live in" */
          GpiCharStringAt( hps, &pt, (LONG)strlen( szResponse ), szResponse );
          pt.x = 50L; pt.y = 30L;         /* Coordinates of location      */
          GpiCharStringAt( hps, &pt, (LONG)strlen( szLocation ), szLocation );
        }
        WinEndPaint( hps );               /* Drawing is complete          */
      }
      break;
    case WM_CLOSE:
      WinPostMsg( hwnd, WM_QUIT, NULL, NULL);     /* Cause termination    */
      break;
    default:
      /*
       * Must have WinDefWindowProc in your window procedure
       */
      return WinDefWindowProc( hwnd, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}
/*********************** End of window procedure  ***********************/

/*********************************************************************
 *  Name : MyDlgProc
 *
 *  Description : Processes all messages sent to the dialog.
 *
 *  Concepts : Called for each message sent to the dialog box.
 *             A switch statement branches to the routines to be
 *             performed for each message processed.  Any messages
 *             not specifically process are passed to the default
 *             window procedure WinDefDlgProc().
 *
 *  API's : WinQueryWindowText
 *          WinDismissDlg
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns : Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY MyDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch ( msg )
  {
    case WM_INITDLG:
       SetSysMenu(hwndDlg);             /* remove grayed items from sys menu*/
       break;

    case WM_COMMAND:                    /* Posted by pushbutton or key  */
      /*
       * PM sends a WM_COMMAND message when the user presses either
       * the Enter or Escape pushbuttons.
       */
      switch( SHORT1FROMMP( mp1 ) )     /* Extract the command value    */
      {
        case DID_OK:                    /* The Enter pushbutton or key. */
                                        /* WinQueryWindowText copies the*/
                                        /* contents of the entry field  */
                                        /* into szLocation.             */
          WinQueryWindowText( WinWindowFromID( hwndDlg, ID_ENTRYFIELD ),
                              LENGTH_STRING,
                              szLocation );
          bComplete = TRUE;             /* Set switch so strings are    */
                                        /* drawn in WM_PAINT processing.*/
        case DID_CANCEL:         /* The Cancel pushbutton or Escape key */
          WinDismissDlg( hwndDlg, TRUE );  /* Removes the dialog box    */
          return (MRESULT) FALSE;
        default:
          break;
      }
      break;
    default:
      /*
       * Any event messages that the dialog procedure has not processed
       * come here and are processed by WinDefDlgProc.
       * This call MUST exist in your dialog procedure.
       */
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
  }
  return (MRESULT) FALSE;
}
/********************** End of dialog procedure  ************************/

/*********************************************************************
 *  Name : ProdInfoDlgProc
 *
 *  Description : Processes all messages sent to the Product
 *                Information dialog box.
 *
 *  Concepts : Called for each message sent to the Product
 *             Information dialog box.  The Product
 *             Information box only has a button control so
 *             this routine only processes WM_COMMAND
 *             messages.  Any WM_COMMAND posted must have come
 *             from the Ok button so we dismiss the dialog
 *             upon receiving it.
 *
 *  API's : WinDismissDlg
 *          WinDefDlgProc
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns : Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY ProdInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,
                                  MPARAM mp2)
{
   switch(msg)
   {
      case WM_INITDLG:
         SetSysMenu(hwnd);              /* remove grayed items from sys menu*/
         break;

      case WM_COMMAND:
         /*
          * No matter what the command, close the dialog
          */
         WinDismissDlg(hwnd, TRUE);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;
   }
   return (MRESULT)0;
}
/********************** End ofProdinfo dialog procedure ****************/

/****************************************************************
 *  Name:   DisplayMessage
 *
 *  Description : Displays error messages.
 *
 *  Concepts : Displays a message box containing an error
 *             message each time called.
 *
 *  API's : WinMessageBoxNon
 *
 * Parameters   : str - error message to display
 *
 *  Returns:  Void
 *
 ****************************************************************/
VOID DisplayMessage(PCH str)
{
  /*
   * This routine calls WinMessageBox to dsplay error messages to the
   * user.
   */

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PCH) str,
     (PCH) "Error Messages For The Dialog Sample", ID_MESSAGEBOX,
     MB_OK | MB_APPLMODAL | MB_MOVEABLE | MB_ERROR);
}
/******************* End of DisplayMessage procedure ********************/
/***********************************************************
 * Name         : SetSysMenu
 *
 * Description  : Procedure to remove grayed items from
 *                all dialog window system menus.
 *
 * Concepts     : This routine obtains the handle for the system
 *                menu and sends a message to delete the item
 *                from the System Menu using the pre-defined
 *                menuitem value. The item is deleted only if
 *                it is disabled.
 *
 *                It is recommended that the system menu not
 *                be altered by the programmer.
 *
 * API's        : WinWindowFromID, WinSendMsg.
 *
 * Parameters   : None
 *
 * Return       : None
 *
 **************************************************************/
VOID SetSysMenu(HWND hwndDlg)
{
  HWND  hwndSysMenu;
  MENUITEM menuItem;
  SHORT sItemCount;
  SHORT sMenu[MAXITEMS] = {SC_HIDE,
                           SC_RESTORE,
                           SC_SIZE,
                           SC_MOVE,
                           SC_MINIMIZE,
                           SC_MAXIMIZE,
                           SC_CLOSE,
                           SC_TASKMANAGER};


  /* Obtain the system menu handle  */
  hwndSysMenu = WinWindowFromID(hwndDlg,
                                FID_SYSMENU);

  for (sItemCount = 0;sItemCount < MAXITEMS ;sItemCount ++ )
  {
     /* Obtain the MENUITEM structure for the menu item */
     WinSendMsg(hwndSysMenu,
                MM_QUERYITEM,
                MPFROM2SHORT(sMenu[sItemCount],TRUE),
                MPFROMP(&menuItem));

     /* If the menu item is disabled - delete it. */
     if (menuItem.afAttribute == MIA_DISABLED) 
     {
        WinSendMsg(hwndSysMenu,
                   MM_DELETEITEM,
                   MPFROM2SHORT(sMenu[sItemCount],TRUE),
                   NULL);
     }
  }
}
/******************* End of SetSysMenu() procedure ********************/

/******************************************************************************
*
*  File Name   : JIGHELP.C
*
*  Description : This module contains all the routines for interfacing with
*                the IPF help manager.
*
*  Concepts    : Using the Information Presentation Facility for help
*
*  Entry Points:
*                AboutBoxDlgProc()
*                DestroyHelpInstance()
*                DisplayHelpPanel()
*                HelpAbout()
*                HelpExtended()
*                HelpHelpForHelp()
*                HelpIndex()
*                HelpInit()
*                HelpTutorial()
*                ShowDlgHelp()
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
******************************************************************************/

/*--------------------------------------------------------------
 *  Include files, macros, defined constants, and externs
  --------------------------------------------------------------*/

/*
 * If DEBUG is defined, then the help panels will display their
 *  id values on their title bar.  This is useful for determining
 *  which help panels are being shown for each dialog item.  When
 *  the DEBUG directive is not defined, then the panel ids are not
 *  displayed.
 */

/*
   A better method of defining this directive is in a parameter to the
   compiler (-DDEBUG).

#define  DEBUG
*/

#include "jigsaw.h"
#include "globals.h"
#include "jighelp.h"
#include <string.h>

/*--------------------------------------------------------------*\
 *  Local Static Variables
\*--------------------------------------------------------------*/
static HWND hwndHelpInstance;
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static BOOL fHelpEnabled = FALSE;


/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/
MRESULT EXPENTRY AboutBoxDlgProc(HWND hwnd, ULONG msg,
             MPARAM mp1, MPARAM mp2);
VOID ShowDlgHelp(HWND hwnd);


/****************************************************************\
 *  Routine for initializing the help manager
 *--------------------------------------------------------------
 *
 *  Name:   HelpInit()
 *
 *  Purpose: Initializes the IPF help facility
 *
 *  Usage:  Called once during initialization of the program
 *
 *  Method: Initializes the HELPINIT structure and creates the
 *       help instance.  If successful, the help instance
 *       is associated with the main window
 *
 *  Returns:
 *
\****************************************************************/
VOID HelpInit(VOID)
{
    HELPINIT hini;

    /* if we return because of an error, Help will be disabled */
    fHelpEnabled = FALSE;

    /* inititalize help init structure */
    hini.cb = sizeof(HELPINIT);
    hini.ulReturnCode = 0L;

    hini.pszTutorialName = (PSZ)NULL;   /* if tutorial added, add name here */

    hini.phtHelpTable = (PHELPTABLE)MAKELONG(JIGSAW_HELP_TABLE, 0xFFFF);
    hini.hmodHelpTableModule = (HMODULE)0;
    hini.hmodAccelActionBarModule = (HMODULE)0;
    hini.idAccelTable = 0;
    hini.idActionBar = 0;

    if(!WinLoadString(habMain,
            (HMODULE)0,
            IDS_HELPWINDOWTITLE,
            HELPLIBRARYNAMELEN,
            (PSZ)szWindowTitle))  {

   MessageBox(hwndFrame, IDS_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
   return;
    }

    hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

    /* if debugging, show panel ids, else don't */
#ifdef DEBUG
    hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
    hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif

    if(!WinLoadString(habMain,
            (HMODULE)0,
            IDS_HELPLIBRARYNAME,
            HELPLIBRARYNAMELEN,
            (PSZ)szLibName))   {

   MessageBox(hwndFrame, IDS_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
   return;
    }

    hini.pszHelpLibraryName = (PSZ)szLibName;

    /* creating help instance */
    hwndHelpInstance = WinCreateHelpInstance(habMain, &hini);

    if(!hwndHelpInstance || hini.ulReturnCode)   {
   MessageBox(hwndFrame,
         IDS_HELPLOADERROR,
         MB_OK | MB_ERROR,
         TRUE);
   return;
    }

    /* associate help instance with main frame */
    if(!WinAssociateHelpInstance(hwndHelpInstance, hwndFrame)) {
   MessageBox(hwndFrame,
         IDS_HELPLOADERROR,
         MB_OK | MB_ERROR,
         TRUE);
   return;
    }

    /* help manager is successfully initialized so set flag to TRUE */
    fHelpEnabled = TRUE;

}   /* HelpInit() */


/****************************************************************\
 *  Processes the Help for Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:   HelpHelpForHelp(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         Help for Help item of the Help menu
 *
 *  Usage:  Called from MainCommand when the Help for Help
 *       menu item is selected
 *
 *  Method: Sends an HM_DISPLAY_HELP message to the help
 *       instance so that the default Help For Help is
 *       displayed.
 *
 *  Returns:
 *
\****************************************************************/
VOID  HelpHelpForHelp(mp2)
MPARAM mp2;   /* second parameter of WM_COMMAND message */
{

    /* this just displays the system help for help panel */
    if(fHelpEnabled)
   if(WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP, NULL, NULL))
       MessageBox(hwndFrame,
             IDS_HELPDISPLAYERROR,
             MB_OK | MB_ERROR,
             FALSE);

    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'
    \*   warning at compile time.                  */
    mp2;

}   /* HelpHelpForHelp() */


/****************************************************************\
 *  Processes the Extended Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:   HelpExtended( VOID )
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         Extended Help item of the Help menu
 *
 *  Usage:  Called from MainCommand when the Extended Help
 *       menu item is selected
 *
 *  Method: Sends an HM_EXT_HELP message to the help
 *       instance so that the default Extended Help is
 *       displayed.
 *
 *  Returns:
 *
\****************************************************************/
VOID  HelpExtended( VOID )
{

    /* this just displays the system extended help panel */
    if(fHelpEnabled)

/*
    if (WinSendMsg(hwndHelpInstance,PANEL_EXTENDED_CONTENTS,
                 MPFROMSHORT(PANEL_EXTENDED_CONTENTS), NULL))
       MessageBox(hwndFrame,
             IDS_HELPDISPLAYERROR,
             MB_OK | MB_ERROR,
             FALSE);
*/
    if(WinSendMsg(hwndHelpInstance, HM_EXT_HELP, NULL, NULL))
       MessageBox(hwndFrame,
             IDS_HELPDISPLAYERROR,
             MB_OK | MB_ERROR,
             FALSE);
}   /* HelpExtended() */



/****************************************************************\
 *  Processes the Keys Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:   HelpKeys(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         Keys Help item of the Help menu
 *
 *  Usage:  Called from MainCommand when the Keys Help
 *       menu item is selected
 *
 *  Method: Sends an HM_KEYS_HELP message to the help
 *       instance so that the default Keys Help is
 *       displayed.
 *
 *  Returns:
 *
\****************************************************************/
/*VOID  HelpKeys(mp2)
MPARAM mp2;    second parameter of WM_COMMAND message
{                                                      */

    /* this just displays the system keys help panel
    if(fHelpEnabled)
   if(WinSendMsg(hwndHelpInstance, PANEL_KEYS_CONTENTS, MPFROMSHORT(PANEL_KEYS_CONTENTS), NULL))
       MessageBox(hwndFrame,
             IDS_HELPDISPLAYERROR,
             MB_OK | MB_ERROR,
             FALSE);
                                                      */

    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'
    \*   warning at compile time.                  */
 /*   mp2;

}    HelpKeys() */


/****************************************************************\
 *  Processes the Index Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:   HelpIndex(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         Index Help item of the Help menu
 *
 *  Usage:  Called from MainCommand when the Index Help
 *       menu item is selected
 *
 *  Method: Sends an HM_INDEX_HELP message to the help
 *       instance so that the default Index Help is
 *       displayed.
 *
 *  Returns:
 *
\****************************************************************/
VOID  HelpIndex(mp2)
MPARAM mp2;   /* second parameter of WM_COMMAND message */
{

    /* this just displays the system help index panel */
    if(fHelpEnabled)
   if(WinSendMsg(hwndHelpInstance, HM_HELP_INDEX, NULL, NULL))
       MessageBox(hwndFrame,
             IDS_HELPDISPLAYERROR,
             MB_OK | MB_ERROR,
             FALSE);


    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'
    \*   warning at compile time.                  */
    mp2;

}   /* HelpIndex() */


/****************************************************************\
 *  Processes the Tutorial Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:   HelpTutorial(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         Tutorial Help item of the Help menu.  While the
 *         standard template application does not include a
 *         Tutorial menu item, you can add one if your
 *         application has a tutorial.
 *
 *  Usage:  Called from MainCommand when the Tutorial Help
 *       menu item is selected
 *
 *  Method:
 *
 *  Returns:
 *
\****************************************************************/
VOID  HelpTutorial(mp2)
MPARAM mp2;   /* second parameter of WM_COMMAND message */
{

   /*--------------------------------------------------------------*\
    *  Insert code for any tutorial here
   \*--------------------------------------------------------------*/


    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'
    \*   warning at compile time.                  */
    mp2;

}   /* HelpTutorial() */

/****************************************************************\
 *  Processes the About command from the Help menu
 *--------------------------------------------------------------
 *
 *  Name:   HelpAbout(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the
 *         About item of the Help menu
 *
 *  Usage:  Called from MainCommand when the About
 *       menu item is selected
 *
 *  Method: Calls WinDlgBox to display the about box dialog.
 *
 *  Returns:
 *
\****************************************************************/
VOID  HelpAbout(mp2)
MPARAM mp2;   /* second parameter of WM_COMMAND message */
{
    /* display the AboutBox dialog */
    WinDlgBox(HWND_DESKTOP,
         hwndFrame,
         (PFNWP)AboutBoxDlgProc,
         (HMODULE)0,
         IDD_ABOUTBOX,
         (PVOID)NULL);

    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'
    \*   warning at compile time.                  */
    mp2;

}   /* HelpAbout() */


/****************************************************************\
 *  Displays the help panel indicated
 *--------------------------------------------------------------
 *
 *  Name:   DisplayHelpPanel(idPanel)
 *
 *  Purpose: Displays the help panel whose id is given
 *
 *  Usage:  Called whenever a help panel is desired to be
 *       displayed, usually from the WM_HELP processing
 *       of the dialog boxes
 *
 *  Method: Sends HM_DISPLAY_HELP message to the help instance
 *
 *  Returns:
 *
\****************************************************************/
VOID DisplayHelpPanel(SHORT idPanel) /* ID of help panel to display */
{
    if (fHelpEnabled)
      if (WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP,
                     MPFROM2SHORT(idPanel, NULL), MPFROMSHORT(HM_RESOURCEID)))

         MessageBox(hwndFrame, IDS_HELPDISPLAYERROR, MB_OK | MB_ERROR, TRUE);

}   /* DisplayHelpPanel() */



/****************************************************************\
 *  Destroys the help instance
 *--------------------------------------------------------------
 *
 *  Name:   DestroyHelpInstance(VOID)
 *
 *  Purpose: Destroys the help instance for the application
 *
 *  Usage:  Called after exit from message loop
 *
 *  Method: Calls WinDestroyHelpInstance() to destroy the
 *       help instance
 *
 *  Returns:
 *
\****************************************************************/
VOID DestroyHelpInstance(VOID)
{
    if(hwndHelpInstance)  {
   WinDestroyHelpInstance(hwndHelpInstance);
    }

}   /* DestroyHelpInstance() */
/****************************************************************\
 *  Dialog procedure for the About dialog box
 *--------------------------------------------------------------
 *
 *  Name:   AboutBoxDlgProc(hwnd, msg, mp1, mp2)
 *
 *  Purpose: Processes all messages sent to the About Box
 *
 *  Usage:  Called for each message sent to the About Box
 *       dialog box.
 *
 *  Method: the about box only has a button control so this
 *       routine only processes WM_COMMAND messages.  Any
 *       WM_COMMAND posted must have come from the Ok
 *       button so we dismiss the dialog upon receiving it.
 *
 *  Returns: Dependent upon message sent
 *
\****************************************************************/
MRESULT EXPENTRY AboutBoxDlgProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2)
{
    switch(msg)  {
   case WM_COMMAND:
       /* no matter what the command, close the dialog */
       WinDismissDlg(hwnd, TRUE);
       break;
/**********  Remove this code for now
   case WM_HELP:
       ShowDlgHelp(hwnd);
       break;             ************/

   default:
       return(WinDefDlgProc(hwnd, msg, mp1, mp2));
       break;
    }

    return 0L;

}   /* AboutBoxDlgProc() */
/****************************************************************\
 *  Shows the help panel for the given dialog window
 *--------------------------------------------------------------
 *
 *  Name:   ShowDlgHelp(hwnd)
 *
 *  Purpose: Displays the help panel for the current selected
 *        item in the dialog window
 *
 *  Usage:  Called each time a WM_HELP message is posted to
 *       a dialog
 *
 *  Method: gets the id value of the window and determine which
 *       help panel to display.  Then sends a message to
 *       the help instance to display the panel.  If the dialog
 *       or item is not included here, then the unknown dialog
 *       or unknown item panel is displayed.
 *
 *  Returns:
 *
\****************************************************************/
VOID ShowDlgHelp(hwnd)
HWND hwnd;   /* handle of list box window */
{
    SHORT idDlg, idItem;
    HWND  hwndFocus;

    /* get the id of the dialog box */
    idDlg = WinQueryWindowUShort(hwnd, QWS_ID);

    /* finds which window has the focus and gets its id */
    hwndFocus = WinQueryFocus(HWND_DESKTOP);
    idItem = WinQueryWindowUShort(hwndFocus, QWS_ID);


}   /* ShowDlgHelp() */

/******************************  END JIGHELP.C  ******************************/

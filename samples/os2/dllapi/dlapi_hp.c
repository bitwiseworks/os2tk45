/*static char *SCCSID = "@(#)dlapi_hp.c	6.10 92/02/18";*/
/*==============================================================*\
 *  DLAPI_HP.C - routines for help information                  *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains all the routines for interfacing with  *
 *  the IPF help manager.                                       *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *          InitHelp()                                          *
 *          HelpHelpForHelp(mp2)                                *
 *          HelpExtended(mp2)                                   *
 *          HelpKeys(mp2)                                       *
 *          HelpIndex(mp2)                                      *
 *          HelpAbout(mp2)                                      *
 *          DisplayHelpPanel(idPanel)                           *
 *          DestroyHelpInstance(VOID)                           *
 *          AboutDlgProc(hwnd, msg, mp1, mp2)                   *
 *          FixSysMenu()                                        *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "dllapi.h"

/****************************************************************\
 *  Routine for initializing the help manager
 *--------------------------------------------------------------
 *
 *  Name:    InitHelp()
 *
 *  Purpose: Initializes the IPF help facility
 *
 *  Usage:   Called once during initialization of the program
 *
 *  Method:  Initializes the HELPINIT structure and creates the help
 *           instance. If successful, the help instance is associated
 *           with the main window
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID InitHelp(VOID)
{
   HELPINIT hini;

    /* If we return because of an error, Help will be disabled */
   fHelpEnabled = FALSE;
                             /* Initialize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0;
                           /* If tutorial added, add name here */
   hini.pszTutorialName = (PSZ)NULL;

   hini.phtHelpTable = (PHELPTABLE)MAKELONG(DLL_HELP_TABLE, 0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   if(!WinLoadString(hab,
                     0,
                     IDS_HELPWINDOWTITLE,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szWindowTitle))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, "Error !",
                             MB_OK | MB_ERROR, TRUE);
       return;
   }
   hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

                   /* If debugging, show panel ids, else don't */
#ifdef DEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif

   if(!WinLoadString(hab,
                     0,
                     IDS_HELPLIBRARYNAME,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szLibName))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, "Error !",
                             MB_OK | MB_ERROR, TRUE);
       return;
   }
   hini.pszHelpLibraryName = (PSZ)szLibName;
                                     /* Creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(hwndHelpInstance == 0L || hini.ulReturnCode)
   {
       MessageBox(hwndMainFrame, IDMSG_HELPLOADERROR, "Error !",
                                    MB_OK | MB_ERROR, TRUE);
       return;
   }
                    /* Associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndMainFrame))
   {
       MessageBox(hwndMainFrame, IDMSG_HELPLOADERROR, "Error !",
                                 MB_OK | MB_ERROR, TRUE);
       return;
   }
       /* Help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
   return;
}                                                 /* InitHelp() */

/****************************************************************\
 *  Processes the Help for Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpHelpForHelp(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Help for
 *           Help item of the Help menu
 *
 *  Usage:   Called from MainCommand when the Help for Help menu item
 *           is selected
 *
 *  Method:  Sends an HM_DISPLAY_HELP message to the help instance so
 *           that the default Help For Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpHelpForHelp(MPARAM mp2)
{
          /* This just displays the system help for help panel */
   if(fHelpEnabled)
     if((LONG)WinSendMsg(hwndHelpInstance,HM_DISPLAY_HELP,(MPARAM)0,(MPARAM)0))
           MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, "Error !",
                      MB_OK | MB_ERROR, TRUE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'*
   \*  warning at compile time.                                    */
   return;
}                                          /* HelpHelpForHelp() */

/****************************************************************\
 *  Processes the Extended Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpExtended(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Extended
 *           Help item of the Help menu.
 *
 *  Usage:   Called from MainCommand when the Extended Help menu item
 *           is selected.
 *
 *  Method:  Sends an HM_EXT_HELP message to the help instance so that
 *           the default Extended Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpExtended(MPARAM mp2)
{
          /* This just displays the system extended help panel */
   if(fHelpEnabled)
     if((LONG)WinSendMsg(hwndHelpInstance,HM_EXT_HELP,(MPARAM)0L,(MPARAM)0L))
           MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, "Error !",
                      MB_OK | MB_ERROR, TRUE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'*
   \*  warning at compile time.                                    */
   return;
}                                             /* HelpExtended() */

/****************************************************************\
 *  Processes the Keys Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpKeys(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Keys Help
 *           item of the Help menu.
 *
 *  Usage:   Called from MainCommand when the Keys Help menu item is
 *           selected
 *
 *  Method:  Sends an HM_KEYS_HELP message to the help instance so that
 *           the default Keys Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpKeys(MPARAM mp2)
{
              /* This just displays the system keys help panel */
   if(fHelpEnabled)
     if((LONG)WinSendMsg(hwndHelpInstance,HM_KEYS_HELP,(MPARAM)0L,(MPARAM)0L))
          MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, "Error !",
                      MB_OK | MB_ERROR, TRUE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'*
   \*  warning at compile time.                                    */
   return;
}                                                 /* HelpKeys() */

/****************************************************************\
 *  Processes the Index Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpIndex(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Index Help
 *           item of the Help menu
 *
 *  Usage:   Called from MainCommand when the Index Help menu item
 *           is selected
 *
 *  Method:  Sends an HM_INDEX_HELP message to the help instance so
 *           that the default Index Help is displayed.
 *
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpIndex(MPARAM mp2)
{
              /* This just displays the system help index panel */
   if(fHelpEnabled)
     if((LONG)WinSendMsg(hwndHelpInstance,HM_HELP_INDEX,(MPARAM)0L,(MPARAM)0L))
           MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, "Error !",
                      MB_OK | MB_ERROR, TRUE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'*
   \*  warning at compile time.                                    */
   return;
}                                                /* HelpIndex() */

/****************************************************************\
 *  Processes the About command from the Help menu
 *--------------------------------------------------------------
 *
 *  Name:    HelpAbout(mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the About item
 *           of the Help menu
 *
 *  Usage:   Called from MainCommand when the About menu item is selected
 *
 *  Method:  Calls WinDlgBox to display the about box dialog.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpAbout(MPARAM mp2)
{
                                /* Display the AboutBox dialog */
   WinDlgBox(HWND_DESKTOP,
                hwndMain,
                AboutDlgProc,
                (HMODULE)0,
                IDD_PRODUCTINFO,
                NULL);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'*
   \*  warning at compile time.                                    */
   return;
}                                                /* HelpAbout() */

/****************************************************************\
 *  Displays the help panel indicated
 *--------------------------------------------------------------
 *
 *  Name:    DisplayHelpPanel(idPanel)
 *
 *  Purpose: Displays the help panel whose id is given
 *
 *  Usage:   Called whenever a help panel is desired to be displayed,
 *           usually from the WM_HELP processing of the dialog boxes
 *
 *  Method:  Sends HM_DISPLAY_HELP message to the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DisplayHelpPanel(LONG idPanel)
{
   if(fHelpEnabled)
       if((LONG)WinSendMsg(hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMLONG(MAKELONG(idPanel, NULL)),
                     MPFROMSHORT(HM_RESOURCEID)))

           MessageBox(hwndMainFrame, IDMSG_HELPDISPLAYERROR, "Error !",
                      MB_OK | MB_ERROR, TRUE);
   return;
}                                         /* DisplayHelpPanel() */

/****************************************************************\
 *  Destroys the help instance
 *--------------------------------------------------------------
 *
 *  Name:    DestroyHelpInstance(VOID)
 *
 *  Purpose: Destroys the help instance for the application
 *
 *  Usage:   Called after exit from message loop
 *
 *  Method:  Calls WinDestroyHelpInstance() to destroy the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DestroyHelpInstance(VOID)
{
   if(hwndHelpInstance != 0L)
   {
       WinDestroyHelpInstance(hwndHelpInstance);
   }
   return;
}                                      /* DestroyHelpInstance() */

/****************************************************************\
 *
 * Name:    AboutDlgProc(hwnd, msg, mp1, mp2)
 *
 * Purpose: Handles all the messages associated with the About dialog.
 *
 * Usage:   Called from HelpAbout() when about menu be selected.
 *
 * Returns:
 *
\****************************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         return (MRESULT)0L;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)0L;

      default:
         sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   return (MRESULT)0L;
}

/****************************************************************\
 * Name:    FixSysMenu()                                         *
 *                                                               *
 * Purpose: This routine removes the Restore, Size, Minimize, and*
 *          Maximize options from the system menu of a dialog.   *
 *                                                               *
 * Usage  : Called during the WM_INITDLG of a dialog procedure.  *
 *                                                               *
 * Returns: VOID                                                 *
 *                                                               *
\****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
   HWND hwndMenu;                                   /* handle to system menu */

   hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);

   WinSendMsg(hwndMenu,               /* delete Restore from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_RESTORE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,                  /* delete Size from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_SIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,              /* delete Minimize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MINIMIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,              /* delete Maximize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MAXIMIZE, TRUE),
              MPFROMP(NULL));
   return;
}

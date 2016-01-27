/**************************************************************************
 *  File name  :  draghelp.c
 *
 *  Description:  This file contains all the routines for interfacing
 *                with the IPF help manager.
 *
 *                This source file contains the following functions:
 *
 *                InitHelp(hab, hwnd)
 *                HelpUsingHelp(hwnd)
 *                HelpGeneral(hwnd)
 *                HelpIndex(hwnd)
 *                HelpProductInfo(hwnd)
 *                DestroyHelpInstance(hwnd)
 *                ProductInfoDlgProc(hwnd, msg, mp1, mp2)
 *                SetSysMenu(hwnd)
 *
 *  Concepts   :  IPF Help Manager
 *
 *  API's      :  WinLoadString
 *                WinCreateHelpInstance
 *                WinAssociateHelpInstance
 *                WinSendMsg
 *                WinDlgBox
 *                WinDestroyHelpInstance
 *                WinDefDlgProc
 *                WinWindowFromID
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDIO.H, DRAGDROP.H, DRAGHELP.H
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
#define  INCL_WINSTDDRAG
#define  INCL_WINMENUS
#define  INCL_WINFRAMEMGR

#include <os2.h>
#include <string.h>
#include <stdio.h>
#include "dragdrop.h"
#include "draghelp.h"

#define HELPLIBRARYNAMELEN  20

/*
 *  Global variables
 */
static HWND hwndHelpInstance;
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static BOOL fHelpEnabled;

/*
 *  Entry point declarations
 */
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd,
                                    USHORT msg,
                                    MPARAM mp1,
                                    MPARAM mp2);

/**************************************************************************
 *
 *  Name       :  InitHelp(hab, hwnd)
 *
 *  Description: Initializes the IPF help facility
 *               Called once during initialization of the program
 *
 *  Concepts   : Initializes the HELPINIT structure and creates the
 *               help instance.  If successful, the help instance
 *               is associated with the main window queue
 *
 *  API's      :   WinLoadString
 *                 WinCreateHelpInstance
 *                 WinAssociateHelpInstance
 *
 *  Parameters :  hab      = anchor block handle
 *                hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID InitHelp(HAB hab, HWND hwndMain)
{
   HELPINIT hini;

   /* If we return because of an error, Help will be disabled. */
   fHelpEnabled = FALSE;

   /* initialize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0L;
   hini.pszTutorialName = (PSZ)NULL;
   hini.phtHelpTable = (PHELPTABLE)MAKELONG(DRAGDROP_HELP_TABLE, 0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   if(!WinLoadString(hab,
                     0,
                     IDS_CLIENTCLASS,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szWindowTitle))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
       return;
   }

   hini.pszHelpWindowTitle = (PSZ)szWindowTitle;
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;

   if(!WinLoadString(hab,
                     0,
                     IDS_HELPLIBRARYNAME,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szLibName))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
       return;
   }

   hini.pszHelpLibraryName = (PSZ)szLibName;

                   /* creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(!hwndHelpInstance)
   {
       MessageBox(hwndMain, IDMSG_HELPNOTAVAILABLE, MB_OK | MB_ERROR, TRUE);
       return;
   }

   if(hini.ulReturnCode)
   {
       MessageBox(hwndMain, IDMSG_HELPCREATEERROR, MB_OK | MB_ERROR, TRUE);
       return;
   }

                /* associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndMain))
   {
       MessageBox(hwndMain, IDMSG_HELPASSOCIATEERROR, MB_OK | MB_ERROR, TRUE);
       return;
   }

   /* help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
}   /*  End of InitHelp  */

/**************************************************************************
 *
 *  Name       :  HelpUsingHelp(hwnd)
 *
 *  Description: Processes the Using help choice from the Help pulldown
 *
 *  Concepts   : Processes the WM_COMMAND message posted by the
 *               Using Help item of the Help menu
 *               Called from MainCommand when the Using Help
 *               menu item is selected
 *               Sends an HM_DISPLAY_HELP message to the help
 *               instance so that the default Using Help is
 *               displayed.
 *
 *  API's      :   WinSendMsg
 *
 *  Parameters :  hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID  HelpUsingHelp(HWND hwndMain)
{
    /* this just displays the system using help panel */
    if(fHelpEnabled)
       if (NULL != WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP,
                          (MPARAM)NULL, (MPARAM)NULL))
        {
            MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, MB_OK | MB_ERROR,
                       FALSE);
        }
}   /* End of HelpUsingHelp   */


/**************************************************************************
 *
 *  Name       :  HelpGeneral(hwnd)
 *
 *  Description: Processes the General help choice from the Help pulldown
 *
 *  Concepts   : Processes the WM_COMMAND message posted by the
 *               General Help item of the Help menu
 *               Called from MainCommand when the General Help
 *               menu item is selected
 *               Sends an HM_EXT_HELP message to the help
 *               instance so that the default Extended Help is
 *               displayed.
 *
 *  API's      :   WinSendMsg
 *
 *  Parameters :  hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID  HelpGeneral(HWND hwndMain)
{
    /* this just displays the system general help panel */
    if(fHelpEnabled)
        if(NULL != WinSendMsg(hwndHelpInstance, HM_EXT_HELP,
                      (MPARAM)NULL, (MPARAM)NULL))
            MessageBox(hwndMain,
                       IDMSG_HELPDISPLAYERROR,
                       MB_OK | MB_ERROR,
                       FALSE);
}   /* End of HelpGeneral   */


/**************************************************************************
 *
 *  Name       :  HelpIndex(hwnd)
 *
 *  Description: Processes the Help index choice from the Help pulldown
 *
 *  Concepts   : Processes the WM_COMMAND message posted by the
 *               Help index item of the Help menu
 *               Called from MainCommand when the Help index
 *               menu item is selected
 *               Sends an HM_INDEX_HELP message to the help
 *               instance so that the Help index is displayed.
 *
 *  API's      :  WinSendMsg
 *
 *  Parameters :  hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID  HelpIndex(HWND hwndMain)
{
    /* this just displays the system help index panel */
    if(fHelpEnabled)
        if(NULL != WinSendMsg(hwndHelpInstance, HM_HELP_INDEX,
                      (MPARAM)NULL, (MPARAM)NULL))
            MessageBox(hwndMain,
                       IDMSG_HELPDISPLAYERROR,
                       MB_OK | MB_ERROR,
                       FALSE);
}   /* End of HelpIndex   */


/**************************************************************************
 *
 *  Name       :  HelpProductInfo(hwndMain)
 *
 *  Description: Processes the Product information choice from the Help
 *               pulldown
 *
 *  Concepts   : Processes the WM_COMMAND message posted by the
 *               Product information item of the Help menu
 *               Called from MainCommand when the Product information
 *               menu item is selected
 *               Calls WinDlgBox to display the Product information
 *               dialog.
 *
 *  API's      :  WinSendMsg
 *
 *  Parameters :  hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID  HelpProductInfo(HWND hwndMain)
{
    /* display the Product Information dialog. */
    if(DID_ERROR == WinDlgBox(HWND_DESKTOP,
                              hwndMain,
                              (PFNWP)ProductInfoDlgProc,
                              0,
                              IDD_PRODUCTINFO,
                              (PVOID)NULL))

        MessageBox(HWND_DESKTOP,
                   IDMSG_CANTSTARTDIALOG ,
                   MB_OK | MB_ERROR,
                   TRUE);
}   /*  End of HelpProductInfo   */

/**************************************************************************
 *
 *  Name       :  DestroyHelpInstance()
 *
 *  Description:  Deletes the application's help instance
 *
 *  Concepts   : Calls WinDestroyHelp Instance to delete the instance
 *
 *  API's      :  WinDestroyHelpInstance
 *
 *  Parameters :  [none]
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID DestroyHelpInstance(VOID)
{
    if(hwndHelpInstance != 0L)
    {
        WinDestroyHelpInstance(hwndHelpInstance);
    }
}   /* End of DestroyHelpInstance    */


/**************************************************************************
 *
 *  Name       :  ProductInfoDlgProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  Processes all messages sent to the Product
 *                information dialog
 *
 *  Concepts   :  Called for each message sent to the Product information
 *                dialog box.
 *
 *                the Product information dialog has only a button control, so this
 *                routine processes only WM_COMMAND messages.  Any
 *                WM_COMMAND posted must have come from the OK
 *                button, so we dismiss the dialog upon receiving it.
 *
 *  API's      :  WinDefDlgProc
 *
 *  Parameters :  [none]
 *
 *  Return     :  dependent on message sent
 *
 *************************************************************************/
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd,
                                    USHORT msg,
                                    MPARAM mp1,
                                    MPARAM mp2)
{
    switch (msg)
    {
    case WM_INITDLG:
       SetSysMenu(hwnd);       /* system menu for this dialog  */
          return (MRESULT)NULL;

    default:
       return WinDefDlgProc(hwnd, msg, mp1, mp2);
    }
    return (MRESULT)NULL;
}   /*  End of ProductInfoDlgProc  */

/**************************************************************************
 *
 *  Name       :  SetSysMenu(hDlg)
 *
 *  Description:  Enables only the Move and Close items in the
 *                system menu pulldown for any dialog
 *
 *  Concepts   :
 *
 *  API's      :  WinWindowFromID
 *                WinSendMsg
 *
 *  Parameters :  hDlg = dialog handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SetSysMenu( HWND hDlg )
{
    HWND     hSysMenu;
    MENUITEM Mi;
    SHORT    Pos;
    ULONG    Id;
    SHORT    cItems;

    /*
     *  We only want Move and Close in the system menu.
     */
    hSysMenu = WinWindowFromID(hDlg, FID_SYSMENU);
    WinSendMsg( hSysMenu, MM_QUERYITEM
              , MPFROM2SHORT(SC_SYSMENU, FALSE), MPFROMP((PCH) & Mi));
    Pos = 0;
    cItems = (SHORT)WinSendMsg( Mi.hwndSubMenu, MM_QUERYITEMCOUNT,
                                (MPARAM)NULL, (MPARAM)NULL);
    while (cItems--)
    {
        Id = (ULONG) WinSendMsg( Mi.hwndSubMenu, MM_ITEMIDFROMPOSITION
                          , MPFROM2SHORT(Pos, TRUE), (MPARAM)NULL);
        switch (SHORT1FROMMP(Id))
        {
        case SC_MOVE:
        case SC_CLOSE:
            Pos++;  /* Don't delete that one. */
            break;
        default:
            WinSendMsg( Mi.hwndSubMenu, MM_DELETEITEM
                      , MPFROM2SHORT(Id, TRUE), (MPARAM)NULL);
        }
    }
} /*  End of SetSysMenu  */
/***************************  End of draghelp.c  *************************/

/**************************************************************************
 *  File name  :  img_help.c
 *
 *  Description:  This module contains all the routines for interfacing with
 *                the IPF help manager.
 *
 *                HelpInit()
 *                HelpUsingHelp()
 *                HelpGeneral()
 *                HelpIndex()
 *                HelpProductInfo()
 *                HelpDestroyInstance()
 *
 *  Concepts   :  online help
 *
 *  API's      :  WinLoadString
 *                WinCreateHelpInstance
 *                WinAssociateHelpInstance
 *                WinSendMsg
 *                WinDestroyHelpInstance
 *                WinDlgBox
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, IMG_MAIN.H, IMG_XTRN.H, IMG_DLG.H,
 *                   IMG_HELP.H
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
#define INCL_WINHELP
#define INCL_WINDIALOGS
#define INCL_WINSTDFILE

#include <os2.h>
#include <string.h>

#include "img_main.h"
#include "img_xtrn.h"
#include "img_dlg.h"
#include "img_help.h"

#define HELPLIBRARYNAMELEN  20

/*
 *  Global variables
 */
static HWND hwndHelpInstance;

#ifdef HELP_MANAGER_ENABLED

/**************************************************************************
 *
 *  Name       : HelpInit()
 *
 *  Description: Initializes the IPF help facility
 *
 *  Concepts   : Called once during initialization of the program
 *
 *               Initializes the HELPINIT structure and creates the
 *               help instance.  If successful, the help instance
 *               is associated with the main window.
 *
 *  API's      : WinLoadString
 *               WinCreateHelpInstance
 *               WinAssociateHelpInstance
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID HelpInit(VOID)
{
   HELPINIT hini;
   CHAR szLibName[HELPLIBRARYNAMELEN];
   CHAR szWindowTitle[HELPLIBRARYNAMELEN];

   /* inititalize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0;

   hini.pszTutorialName = (PSZ)NULL;   /* if tutorial added, add name here */

   hini.phtHelpTable = (PHELPTABLE)(0xFFFF0000 | IMAGE_HELP_TABLE);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

#if (defined(PORT_16) || defined(PORT_S116))
#ifdef DEBUG
   hini.usShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.usShowPanelId = CMIC_HIDE_PANEL_ID;
#endif  /* DEBUG       */
#else   /* NOT PORT_16 */
#ifdef DEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif  /* DEBUG       */
#endif  /* NOT PORT_16 */

   if (!WinLoadString(vhab,
                      (HMODULE)0,
                      IDS_HELPWINDOWTITLE,
                      HELPLIBRARYNAMELEN,
                      (PSZ)szWindowTitle))
   {
       MessageBox(vhwndFrame, IDMSG_CANNOTLOADSTRING, 0,
                  MB_OK | MB_ERROR, FALSE);
       return;
   }
   hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

   if (!WinLoadString(vhab,
                      (HMODULE)0,
                      IDS_HELPLIBRARYNAME,
                      HELPLIBRARYNAMELEN,
                      (PSZ)szLibName))
   {
       MessageBox(vhwndFrame, IDMSG_CANNOTLOADSTRING, 0,
                  MB_OK | MB_ERROR, FALSE);
       return;
   }
   hini.pszHelpLibraryName = (PSZ)szLibName;


   /* creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(vhab, &hini);

   if (!hwndHelpInstance || hini.ulReturnCode)
   {
       MessageBox(vhwndFrame, IDMSG_HELPLOADERROR, 0,
                  MB_OK | MB_ERROR, FALSE);
       return;
   }

   /* associate help instance with main frame */
   if (!WinAssociateHelpInstance(hwndHelpInstance, vhwndFrame))
   {
       MessageBox(vhwndFrame, IDMSG_HELPLOADERROR, 0,
                  MB_OK | MB_ERROR, FALSE);
       return;
   }

   /* set flag to enable Help menu items */
   vfHelpEnabled = TRUE;
}   /* End of HelpInit */

/**************************************************************************
 *
 *  Name       : HelpUsingHelp()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Using help item of the Help menu
 *
 *  Concepts   : Called from MainCommand when the Using help
 *               menu item is selected
 *
 *               Sends an HM_DISPLAY_HELP message to the help
 *               instance so that the default Using help is
 *               displayed.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID  HelpUsingHelp(VOID)
{
   /* this just displays the system help for help panel */
   if (NULL != WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP,
                          (MPARAM)NULL, (MPARAM)NULL))
       MessageBox(vhwndFrame, IDMSG_HELPDISPLAYERROR, 0, MB_OK | MB_ERROR,
                  FALSE);
}   /* End of HelpUsingHelp   */

/**************************************************************************
 *
 *  Name       : HelpGeneral()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               General help item of the Help menu
 *
 *  Concepts   : Called from MainCommand when the General help
 *               menu item is selected
 *
 *               Sends an HM_EXT_HELP message to the help
 *               instance so that the default General help is
 *               displayed.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID HelpGeneral(VOID)
{
   /* this just displays the system general help panel */
   if (NULL != WinSendMsg(hwndHelpInstance, HM_EXT_HELP,
                          (MPARAM)NULL, (MPARAM)NULL))
         MessageBox(vhwndFrame, IDMSG_HELPDISPLAYERROR, 0, MB_OK | MB_ERROR,
                    FALSE);
}   /* End of HelpGeneral() */

/**************************************************************************
 *
 *  Name       : HelpIndex()
 *
 *  Description: Processes the WM_COMMAND message posted by the
 *               Help index item of the Help menu
 *
 *  Concepts   : Called from MainCommand when the Help index
 *               menu item is selected
 *
 *               Sends an HM_INDEX_HELP message to the help
 *               instance so that the Help Index is displayed.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID HelpIndex(VOID)
{
    /* this just displays the system help index panel */
   if (NULL != WinSendMsg(hwndHelpInstance, HM_HELP_INDEX,
                          (MPARAM)NULL, (MPARAM)NULL))
       MessageBox(vhwndFrame, IDMSG_HELPDISPLAYERROR, 0, MB_OK | MB_ERROR,
                  FALSE);
}   /* End of HelpIndex() */

/**************************************************************************
 *
 *  Name       : HelpDestroyInstance()
 *
 *  Description: Destroys the help instance for the application
 *
 *  Concepts   : Called during Exit list processing
 *
 *               Calls WinDestroyHelpInstance() to destroy the
 *               help instance
 *
 *  API's      : WinDestroyHelpInstance
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID HelpDestroyInstance(VOID)
{
   if(hwndHelpInstance != 0L)
   {
      WinDestroyHelpInstance(hwndHelpInstance);
   }
   vfHelpEnabled = FALSE;
}   /* End of HelpDestroyInstance */

#endif

/**************************************************************************
 *
 *  Name       : HelpProductInfo()
 *
 *  Description:  Processes the WM_COMMAND message posted by the
 *                Product information item of the Help menu
 *
 *  Concepts   : Called from MainCommand when the Product information
 *               menu item is selected
 *
 *               Calls WinDlgBox to display the Product information
 *
 *  API's      : WinDlgBox
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID  HelpProductInfo(void)
{
    WinDlgBox(HWND_DESKTOP, vhwndClient, ProductInfoDlgProc, (HMODULE)0,
              IDD_PRODUCTINFO, (PVOID)NULL);
}   /* End of HelpProductInfo */

/**************************************************************************
 *
 *  Name       : DisplayHelpPanel(idPanel)
 *
 *  Description:  Displays the help panel whose id is given
 *
 *  Concepts   : Called whenever a help panel is desired to be
 *               displayed, usually from the WM_HELP processing
 *               of the dialog boxes
 *
 *               Sends HM_DISPLAY_HELP message to the help instance
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : idPanel = code number of the help panel to be displayed
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID DisplayHelpPanel(ULONG idPanel)
{
   if(vfHelpEnabled)
       if(NULL != WinSendMsg(hwndHelpInstance,
                             HM_DISPLAY_HELP,
                             MPFROMLONG(idPanel),
                             MPFROMSHORT(HM_RESOURCEID)))
   {
           MessageBox(vhwndFrame,
                      IDMSG_HELPDISPLAYERROR,
                      0,
                      MB_OK | MB_ERROR,
                      TRUE);
    }
}   /* End of DisplayHelpPanel() */
/***************************  End of img_help.c  *************************/

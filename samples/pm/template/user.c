/**************************************************************************
 *  File name  :  user.c
 *
 *  Description:  This module contains the code for processing messages
 *                sent to the standard window that the standard window does
 *                not process.  The application developer need only modify
 *                this file in order to implement new menu items or process
 *                messages not handled by the standard message routine.
 *
 *                This source file contains the following functions:
 *
 *                UserWndProc(hwnd, msg, mp1, mp2)
 *                UserCommand(mp1, mp2)
 *
 *  Concepts   :  message processing
 *
 *  API's      :  WinDefWindowProc
 *                WinSendMsg
 *
 *    Files    :  OS2.H, MAIN.H, DLG.H, XTRN.H
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

#define INCL_WINMENUS

#include <os2.h>
#include "main.h"
#include "dlg.h"
#include "xtrn.h"

/*
 *  Global variables
 */


/*
 *  Entry point declarations
 */


/**************************************************************************
 *
 *  Name       : UserWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Process any messages sent to hwndMain that
 *               are not processed by the standard window procedure.
 *
 *  Concepts:    Routine is called for each message MainWndProc
 *               does not process.
 *
 *               A switch statement branches control based upon
 *               the message passed.  Any messages not processed
 *               here must be passed onto WinDefWindowProc().
 *
 *  API's      : WinDefWindowProc
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message i.d.
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  Return value dependent upon the message processed
 *
 *************************************************************************/
MRESULT UserWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
   /*
    *  Add case statements for message ids you wish to process.
    */

   default:    /* default must call WinDefWindowProc() */
      return(WinDefWindowProc(hwnd, msg, mp1, mp2));
      break;
   }
   return (MRESULT)0;
}   /* End of UserWndProc   */


/**************************************************************************
 *
 *  Name       : UserCommand(mp1, mp2)
 *
 *  Description: Process any WM_COMMAND messages sent to hwndMain
 *               that are not processed by MainCommand.
 *
 *  Concepts:    Routine is called for each WM_COMMAND that is
 *               not posted by a standard menu item.
 *
 *               A switch statement branches control based upon
 *               the id of the control which posted the message.
 *
 *  API's      :  [none]
 *
 *  Parameters :  mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID UserCommand(MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {

   /*
    *  Add case statements for menuitem ids you wish to process.
    */

   default:
      break;
   }

   /* This routine currently doesn't use the mp2 parameter but
    *  it is referenced here to prevent an 'Unreferenced Parameter'
    *  warning at compile time.
    */
   mp2;
}   /* End of UserCommand  */

/**************************************************************************
 *
 *  Name       : InitMenu()
 *
 *  Description: Processes the WM_INITMENU message for the main window,
 *               disabling any menus that are not active.
 *
 *  Concepts:    Routine is called each time a menu is dropped.
 *
 *               A switch statement branches control based upon
 *               the id of the menu that is being displayed.
 *
 *  API's      :  [none]
 *
 *  Parameters :  mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID InitMenu(MPARAM mp1, MPARAM mp2)
{
              /* define a shorthand way of denoting the menu handle */
#define hwndMenu        HWNDFROMMP(mp2)

   switch(SHORT1FROMMP(mp1))
   {
   case IDM_FILE:
      break;

   case IDM_HELP:
            /*
             * Enable or disable the Help menu depending upon whether the
             * help manager has been enabled
             */
        EnableMenuItem(hwndMenu, IDM_HELPUSINGHELP, fHelpEnabled);
        EnableMenuItem(hwndMenu, IDM_HELPGENERAL, fHelpEnabled);
        EnableMenuItem(hwndMenu, IDM_HELPKEYS, fHelpEnabled);
        EnableMenuItem(hwndMenu, IDM_HELPINDEX, fHelpEnabled);

         /*  REMEMBER: add a case for IDM_HELPTUTORIAL if you include
          *  the Tutorial menu item.
          */

       break;

    default:
       break;
    }

#undef hwndMenu
}   /* End of InitMenu   */

/**************************************************************************
 *
 *  Name       : EnableMenuItem(hwndMenu, idItem, fEnable)
 *
 *  Description: Enables or disables the menu item
 *
 *  Concepts:    Called whenever a menu item is to be enabled or
 *               disabled
 *
 *               Sends a MM_SETITEMATTR to the menu with the
 *               given item id.  Sets the MIA_DISABLED attribute
 *               flag if the item is to be disabled, clears the flag
 *               if enabling.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters :  hwndmenu = menu window handle
 *                idItem   = menu item i.d.
 *                fEnable  = enable (yes) or disable (no)
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID EnableMenuItem(HWND hwndMenu, USHORT idItem, BOOL fEnable)
{
   SHORT fsFlag;

   if(fEnable)
      fsFlag = 0;
   else
      fsFlag = MIA_DISABLED;

   WinSendMsg(hwndMenu,
              MM_SETITEMATTR,
              MPFROM2SHORT(idItem, TRUE),
              MPFROM2SHORT(MIA_DISABLED, fsFlag));

}   /* End of EnableMenuItem() */
/***************************  End of user.c  ****************************/

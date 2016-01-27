/*static char *SCCSID = "@(#)img_dlg.c	6.6 92/02/10";*/
/**************************************************************************
 *  File name  :  img_dlg.c
 *
 *  Description:  This module contains the code for handling the
 *                Product information dialog box.
 *                This source file contains the following functions:
 *
 *                ProductInfoDlgProc(hwnd, msg, mp1, mp2)
 *                SetSysMenu(hwnd)
 *
 *  Concepts   :  This is used only to allow deletion of certain
 *                System Menu items when the dialog is started.
 *
 *  API's      :  WinDefDlgProc
 *                WinSendMsg
 *                WinWindowFromID
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, IMG_MAIN.H, IMG_XTRN.H, IMG_DLG.H
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
#define INCL_WINFRAMEMGR
#define INCL_WINSTDFILE
#include <os2.h>
#include <string.h>

#include "img_main.h"
#include "img_xtrn.h"
#include "img_dlg.h"

/**************************************************************************
 *
 *  Name       : ProductInfoDlgProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Processes messages sent to the Product information dialog
 *
 *  Concepts   : the Product information dialog has only a button control,
 *               so this routine processes only WM_COMMAND messages.  Any
 *               WM_COMMAND posted must have come from the OK
 *               button, so we dismiss the dialog upon receiving it.
 *
 *  API's      : WinDefDlgProc
 *
 *  Parameters : hwnd = the dialog box handle
 *               msg = the message sent to the Product information dialog
 *               mp1 = the first message parameter
 *               mp2 = the second message parameter
 *
 *  Return     : Dependent upon message sent;  NULL = successful
 *
\*************************************************************************/
#ifdef PORT_16
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1,MPARAM mp2)
#endif
#ifdef PORT_32
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1,MPARAM mp2)
#endif
{
    switch (msg)
    {
        case WM_INITDLG:
            SetSysMenu(hwnd);       /* system menu for this dialog  */
            return MRFROMSHORT(FALSE);

        default:
            return WinDefDlgProc(hwnd, msg, mp1, mp2);
    }
    return MRFROMSHORT(0);
} /* End of ProductInfoDlgProc  */

/**************************************************************************
 *
 *  Name       : SetSysMenu(HWND hDlg)
 *
 *  Description: This sets only Move and Close menu items for any dialog.
 *
 *  Concepts   : Unless particular menu items are deleted, the System Menu
 *               will show disabled items.
 *
 *  API's      : WinSendMsg
 *               WinWindowFromID
 *
 *  Parameters : hDlg = the dialog box handle
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID SetSysMenu(HWND hDlg)
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
    cItems = SHORT1FROMMR(WinSendMsg(Mi.hwndSubMenu, MM_QUERYITEMCOUNT,
                                (MPARAM)NULL, (MPARAM)NULL));
    while (cItems--)
    {
        Id = (ULONG) WinSendMsg( Mi.hwndSubMenu, MM_ITEMIDFROMPOSITION
                          , MPFROM2SHORT(Pos, TRUE), (MPARAM)NULL);
        switch (SHORT1FROMMP(Id))
        {
        case SC_MOVE:
        case SC_CLOSE:
            Pos++;              /* Don't delete that one. */
            break;
        default:
            WinSendMsg( Mi.hwndSubMenu, MM_DELETEITEM
                      , MPFROM2SHORT(Id, TRUE), (MPARAM)NULL);
        }
    }
} /* End of SetSysMenu  */
/***************************  End of img_dlg.c   *************************/


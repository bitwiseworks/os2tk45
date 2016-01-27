/**************************************************************************
 *  File name  :  img_view.c
 *
 *  Description:  This module contains the code for handling the functions
 *                offered on the View submenu. These functions are called
 *                from the MainCommand() routine:
 *
 *                ViewSwitchMode()
 *                ViewChangeColor()
 *                ViewSavePosition()
 *                ViewRestorePosition()
 *
 *  Concepts   :  display modes, image processing
 *
 *  API's      :  WinSetParent
 *                WinSendMsg
 *                WinInvalidateRect
 *                GpiSetBackCol
 *                GpiSetColor
 *                GpiSetBackMix
 *                WinQueryWindowRect
 *                GpiSetCurrentPosition
 *
 *  Required
 *    Files    :  OS2.H, STDLIB.H, IMG_MAIN.H, IMG_XTRN.H, IMG_DLG.H
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
#define INCL_WINSCROLLBARS
#define INCL_WINFRAMEMGR
#define INCL_WINSTDFILE
#define INCL_GPIPRIMITIVES

/*
 *  Include files, macros, defined constants, and externs
 */
#include <os2.h>
#include <stdlib.h>

#include "img_main.h"
#include "img_xtrn.h"
#include "img_dlg.h"

/*
 *  Static variables
 */
/* foreground/background color choices */
LONG  lColorArray[COLOR_COUNT] = {CLR_BLACK,
                                  CLR_WHITE,
                                  CLR_BLUE,
                                  CLR_GREEN,
                                  CLR_YELLOW,
                                  CLR_RED};

/**************************************************************************
 *
 *  Name       : ViewSwitchMode()
 *
 *  Description: Toggles the mode of the application from Non-Detail
 *               to Detail mode and visa versa
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window in response to
 *               the user (de)selecting the Detail item
 *               from the View submenu.
 *
 *               Since Non-Detail mode is the opposite to Detail mode,
 *               toggle the menu settings, and ownership of the
 *               scrollbars, and redraw. If entering Detail mode
 *               it is necessary to size the main window as well.
 *
 *  API's      :  WinSetParent
 *                WinSendMsg
 *                WinInvalidateRect
 *
 *  Parameters :  [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ViewSwitchMode()
{
   vfDetail = (BOOL)(!vfDetail);

   /*
    * detach/attach scrollbars to main window accordingly
    */
   WinSetParent(vhwndVScroll,
                 vfDetail ? vhwndFrame : HWND_OBJECT,
                 FALSE);
   WinSetParent(vhwndHScroll,
                 vfDetail ? vhwndFrame : HWND_OBJECT,
                 FALSE);

   WinSendMsg(vhwndFrame,
               WM_UPDATEFRAME,
               MPFROMLONG(FCF_VERTSCROLL | FCF_HORZSCROLL),
               (MPARAM)NULL);

    /* resize main window if in 'Detail' mode */
   if (vfDetail)
        SizeMainWindow();

   WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);

}   /* End of ViewSwitchMode  */

/**************************************************************************
 *
 *  Name       : ViewChangeColor(idColor)
 *
 *  Description: Changes the foreground/background colors of the
 *               selected image.
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window in response to
 *               the user selecting a Color item from either the
 *               'Foreground'/'Background' Color items on the View submenu
 *
 *  API's      :  GpiSetBackColor
 *                GpiSetColor
 *                GpiSetBackMix
 *                WinInvalidateRect
 *
 *  Parameters :  idColor = identifier of the selected color
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ViewChangeColor(USHORT idColor)
{
   LONG lForeClr, lBackClr;
   BOOL fSameClr;
   USHORT idItem;

   /*
    * change either the foreground/background color and save
    * previous settings so that the appropriate menuitem can
    * be reset
    */
   if (idColor < IDM_VIEWBACKCOLORBLACK)
   {
       lForeClr = lColorArray[idColor - COLOR_BASE];
       fSameClr = (BOOL)(lForeClr == vlBackClr);
       idItem = MenuGetColorItemId(IDM_VIEWFORECOLORBLACK);
   }
   else
   {
       lBackClr = lColorArray[idColor - COLOR_BASE - COLOR_COUNT];
       fSameClr = (BOOL)(lBackClr == vlForeClr);
       idItem = MenuGetColorItemId(IDM_VIEWBACKCOLORBLACK);
   }
   /*
    * if background color is the same as the foreground color
    * - ensure this is correct and get out if not
    */
   if (fSameClr)
   {
       if (MessageBox(vhwndFrame,
                      IDMSG_BACKEQFORE, IDMSG_WARNING,
                      MB_APPLMODAL | MB_YESNO | MB_CUAWARNING,
                      TRUE) == MBID_NO)
           return;
   }

    /* cancel previously checked item  & save new values */
   if (idColor < IDM_VIEWBACKCOLORBLACK)
   {
       MenuEnableItem(vhwndViewForeClr, idItem, TRUE);
       vlForeClr = lForeClr;
   }
   else
   {
       MenuEnableItem(vhwndViewBackClr, idItem, TRUE);
       vlBackClr = lBackClr;
   }

   /*
    * set colors appropriately and force redraw
    */
   GpiSetBackColor(vhps, vlBackClr);
   GpiSetColor(vhps, vlForeClr);
   GpiSetBackMix(vhps, BM_OVERPAINT);
   WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);

}   /* End of ViewChangeColor() */

/**************************************************************************
 *
 *  Name       : ViewRestorePosition()
 *
 *  Description: Restores the saved position of the image
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window in response to
 *               the user selecting the Restore Position item
 *
 *  API's      : WinQueryWindowRect
 *               GpiSetCurrentPosition
 *               WinSendMsg
 *               WinInvalidateRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ViewRestorePosition(void)
{
   RECTL rcl;

    /* only valid if image is loaded & in Detail mode */
   if (vfImgLoaded && vfDetail)
   {
       WinQueryWindowRect(vhwndClient, &rcl);
       GpiSetCurrentPosition(vhps, &vptlSave);
       WinSendMsg(vhwndVScroll,
                  SBM_SETPOS,
                  MPFROMLONG(vptlSave.y - rcl.yTop),
                  (MPARAM)NULL);
       WinSendMsg(vhwndHScroll,
                  SBM_SETPOS,
                  MPFROMLONG(labs(vptlSave.x)),
                  (MPARAM)NULL);
       WinInvalidateRect(vhwndClient, (PRECTL)NULL, FALSE);
   }
}   /* End of ViewRestorePosition */

/**************************************************************************
 *
 *  Name       : ViewSavePosition()
 *
 *  Description: Saves the saved position of the image
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window in response to
 *               the user selecting the Save Position item
 *
 *  API's      : GpiQueryCurrentPosition
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ViewSavePosition(void)
{
   if (vfImgLoaded && vfDetail)
       GpiQueryCurrentPosition(vhps, &vptlSave);
}   /* ViewSavePosition() */
/***************************  End of img_view.c  *************************/

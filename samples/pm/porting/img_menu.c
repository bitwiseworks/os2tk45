/**************************************************************************
 *  File name  :  img_menu.c
 *
 *  Description:  This module contains the code for initializing menus as a
 *                result of a WM_INITMENU message, and also code for
 *                disabling/enabling single menu items.  Functions in this
 *                file:
 *
 *                MenuInit(mp1, mp2)
 *                MenuGetColorItemId(idBase)
 *                MenuEnableItem(hwndMenu, idItem, fEnabled)
 *                MenuCheckItem(hwndMenu, idItem)
 *
 *  Concepts   :  menu management
 *
 *  API's      :  WinSendMsg
 *
 *  Required
 *    Files    :  OS2.H, IMG_MAIN.H, IMG_XTRN.H
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

#include "img_main.h"
#include "img_xtrn.h"

/*
 *  Entry point declarations
 */

/**************************************************************************
 *
 *  Name       : MenuInit(mp1, mp2)
 *
 *  Description: Processes the WM_INITMENU message for the main window,
 *               disabling any menus that are not active
 *
 *  Concepts   : Routine is called each time a menu is dropped.
 *
 *               A switch statement branches control based upon
 *               the id of the menu which is being displayed.
 *
 *  API's      :  [none]
 *
 *  Parameters :  mp1 = first message parameter
 *                mp2 = second message parameter
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MenuInit(MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {
      case IDM_FILE:

          /*
           * set the items on the File sub-menu appropriately
           */
           MenuEnableItem(HWNDFROMMP(mp2), IDM_FILEOPEN, (BOOL)!vfMaximized);
           break;

      case IDM_VIEW:

          /*
           * set the items on the View sub-menu appropriately
           */
         if (!vfImgLoaded)
             MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWDETAIL, FALSE);
         else
         {
             MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWDETAIL, TRUE);
             MenuCheckItem(HWNDFROMMP(mp2), IDM_VIEWDETAIL, vfDetail);
         }

         MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWFOREGROUNDCOLOR,
                        vfImgLoaded);

         MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWBACKGROUNDCOLOR,
                        vfImgLoaded);

         MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWSAVEPOSITION,
                        (BOOL)(vfImgLoaded && vfDetail));

         MenuEnableItem(HWNDFROMMP(mp2), IDM_VIEWRESTOREPOSITION,
                        (BOOL)(vfImgLoaded && vfDetail));

         break;

      /*
       * check the currently selected foreground color &
       * set global variable if previously not set
       */
      case IDM_VIEWFOREGROUNDCOLOR:
         MenuEnableItem(HWNDFROMMP(mp2),
                         MenuGetColorItemId(IDM_VIEWFORECOLORBLACK), FALSE);

         if (!vhwndViewForeClr)
            vhwndViewForeClr = HWNDFROMMP(mp2);
         break;

      /*
       * check the currently selected background color &
       * set global variable if previously not set
       */
      case IDM_VIEWBACKGROUNDCOLOR:
         MenuEnableItem(HWNDFROMMP(mp2),
                         MenuGetColorItemId(IDM_VIEWBACKCOLORBLACK), FALSE);

         if (!vhwndViewBackClr)
            vhwndViewBackClr = HWNDFROMMP(mp2);
         break;

      case IDM_HELP:
           /*
            * Enable or disable the Help menu depending upon whether the
            * help manager has been enabled
            */
          MenuEnableItem(HWNDFROMMP(mp2),
                         IDM_HELPUSINGHELP, vfHelpEnabled);

          MenuEnableItem(HWNDFROMMP(mp2),
                         IDM_HELPGENERAL, vfHelpEnabled);

          MenuEnableItem(HWNDFROMMP(mp2),
                         IDM_HELPKEYS, vfHelpEnabled);

          MenuEnableItem(HWNDFROMMP(mp2),
                         IDM_HELPINDEX, vfHelpEnabled);

           /** REMEMBER: add a case for IDM_HELPTUTORIAL if you include
               the menu item   **/

          break;
   }
}   /* End of MenuInit */

/**************************************************************************
 *
 *  Name       : MenuGetColorItemId(idBase)
 *
 *  Description: Returns ID for the currently selected foreground/
 *               background color value
 *
 *  Concepts   : Called whenever either the foreground/background
 *               floating color menu is to be displayed.
 *
 *  API's      : [none]
 *
 *  Parameters : idBase = identifier of base color, foreground/background
 *
 *  Return     : id of menu item
 *
 *************************************************************************/
SHORT MenuGetColorItemId(SHORT idBase) /* Id of base color, foregrnd/backgrnd */
{
   LONG lColor;

   if (idBase == IDM_VIEWFORECOLORBLACK)
       lColor = vlForeClr;
   else
       lColor = vlBackClr;

   /*
    * calculate menu item id based on current color value
    */
   switch ((SHORT)lColor)
   {
       case CLR_BLACK:
           return idBase;
           break;
       case CLR_WHITE:
           return (SHORT)(idBase + 1);
           break;
       case CLR_BLUE:
           return (SHORT)(idBase + 2);
           break;
       case CLR_GREEN:
           return (SHORT)(idBase + 3);
           break;
       case CLR_YELLOW:
           return (SHORT)(idBase + 4);
           break;
       case CLR_RED:
           return (SHORT)(idBase + 5);
           break;
   }
}   /* End of MenuGetColorItemId */

/**************************************************************************
 *
 *  Name       : MenuEnableItem(hwndMenu, idItem, fEnable)
 *
 *  Description: Enables or disables the menu item
 *
 *  Concepts   : Called whenever a menu item is to be enabled or disabled
 *               Sends a MM_SETITEMATTR to the menu with the
 *               given item id.  Sets the MIA_DISABLED attribute
 *               flag if the item is to be disabled, clears the flag
 *               if enabling.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : hwndMenu = menu handle
 *               idItem   = menu item identifier
 *               fEnable  = flag to enable (TRUE) or disable (FALSE)
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MenuEnableItem(
            HWND hwndMenu,      /* Handle to the menu */
            SHORT idItem,       /* Id of the menu item to be enabled/disabled */
            BOOL fEnable)       /* flag to set enable or disable bit */
{
   WinSendMsg(hwndMenu,
              MM_SETITEMATTR,
              MPFROM2SHORT(idItem, TRUE),
              MPFROM2SHORT(MIA_DISABLED,
                           (fEnable ? 0 : MIA_DISABLED)));
}   /* End of MenuEnableItem */

/**************************************************************************
 *
 *  Name       : MenuCheckItem(hwndMenu, idItem)
 *
 *  Description: Checks a menu item
 *
 *  Concepts   : Called whenever a menu item is to be checked
 *               Sends a MM_SETITEMATTR to the menu with the
 *               given item id.  Sets the MIA_CHECKED attribute
 *               flag if the item is to be checked.
 *
 *  API's      : WinSendMsg
 *
 *  Parameters : hwndMenu = menu handle
 *               idItem   = menu item identifier
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MenuCheckItem(
            HWND hwndMenu,      /* Handle to the menu */
            SHORT idItem,       /* Id of the menu item to be enabled/disabled */
            BOOL fCheck)        /* Check/uncheck item */
{
   WinSendMsg(hwndMenu,
              MM_SETITEMATTR,
              MPFROM2SHORT(idItem, TRUE),
              MPFROM2SHORT(MIA_CHECKED,
                           (fCheck ? MIA_CHECKED : ~MIA_CHECKED)));

}   /* End of MenuCheckItem() */
/***************************  End of img_menu.c  *************************/

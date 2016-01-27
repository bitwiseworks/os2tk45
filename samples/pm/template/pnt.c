/**************************************************************************
 *  File name  :  pnt.c
 *
 *  Description:  This module contains the code for the main client
 *                window painting.
 *
 *                This source file contains the following functions:
 *
 *                MainPaint(hwnd)
 *
 *  Concepts   :  window painting
 *
 *  API's      :  WinBeginPaint
 *                WinFillRect
 *                WinEndPaint
 *
 *  Required
 *    Files    :  OS2.H, MAIN.H, XTRN.H
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

#define  INCL_WINSYS

#include <os2.h>
#include "main.h"
#include "xtrn.h"

/*
 *  Global variables
 */

/*
 *  Entry point declarations
 */


/**************************************************************************
 *
 *  Name       : MainPaint(hwnd)
 *
 *  Description: Paints the main client window.
 *
 *  Concepts:  Routine is called whenver the client window
 *             procedure receives a WM_PAINT message
 *
 *             - begins painting by calling WinBeginPaint
 *                 and retrieving the HPS for the window
 *             - performs any painting desired
 *             - ends painting by calling WinEndPaint
 *
 *  API's      :  WinBeginPaint
 *                WinFillRect
 *                WinEndPaint
 *
 *  Parameters :  hwnd     = window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID MainPaint(HWND hwnd)
{
   RECTL rclUpdate;
   HPS hps;

   hps = WinBeginPaint(hwnd, NULLHANDLE, &rclUpdate);

   /* fill update rectangle with window color */
   WinFillRect(hps, &rclUpdate, SYSCLR_WINDOW);

   /*
    *      Add painting routines here.  Hps is now
    *      the HPS for the window and rclUpdate
    *      contains the update rectangle.
    */

   WinEndPaint(hps);
}   /* End of MainPaint   */
/***************************  End of pnt.c  ****************************/

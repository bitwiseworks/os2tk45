/**************************************************************************
 *  File name  :  img_size.c
 *
 *  Description:  This module contains the code for sizing the main window,
 *                and scrolling the image inside the window.
 *                Functions contained here:
 *
 *                SizeMainWindow()
 *                SizeCalculateMaxWindow(prclBound)
 *                SizePositionImage(mp2)
 *                SizeVScroll(mp2, rcl, ptl)
 *                SizeHScroll(mp2, rcl, ptl)
 *                ScrollBars(usWidth, usHeight)
 *                Scroll(lThmbpos, lDeltaSc, sScrollType, prclInvalid)
 *
 *  Concepts   :  sizable windows
 *
 *  API's      :  WinSetWindowPos
 *                WinQueryWindowRect
 *                GpiSetCurrentPosition
 *                WinSendMsg
 *                WinWindowFromID
 *                WinScrollWindow
 *                WinInvalidateRect
 *                GpiQueryCurrentPosition
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDLIB.H, IMG_MAIN.H, IMG_XTRN.H
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

#include <os2.h>
#include <string.h>
#include <stdlib.h>

#include "img_main.h"
#include "img_xtrn.h"

/*
 *  Static variables
 */
ULONG ulScrollXPage;
ULONG ulScrollYPage;
ULONG ulScrollYLine  = LINESCROLL;
ULONG ulScrollXLine  = LINESCROLL;

/*
 *  Entry point declarations
 */
VOID ScrollBars(USHORT usWidth, USHORT usHeight);
VOID Scroll(LONG lThmbpos, LONG lDeltaSc, SHORT sScrollType,
            PRECTL prclInvalid);

/**************************************************************************
 *
 *  Name       : SizeMainWindow()
 *
 *  Description: Maximizes the amount of the image that is
 *               visible . This can only be done after the image
 *               has been loaded, as the size of the window is
 *               directly related to the size of the image.
 *
 *  Concepts   : Routine is called whenever a WM_COMMAND message
 *               is posted to the main window as a result of
 *               selecting the 'Detail' item on the 'View'
 *               pulldown.
 *
 *                Sets the size and position of the main window
 *                whenever 'Detail' mode is selected.
 *
 *  API's      :  WinSetWindowPos
 *                WinQueryWindowRect
 *                GpiSetCurrentPosition
 *
 *  Parameters :  [none]
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SizeMainWindow(void)
{
    RECTL  rcl, rclCWM;

    /* limit the window size */
    SizeCalculateMaxWindow(&rclCWM);
    WinSetWindowPos(vhwndFrame, (HWND)NULL,
                    (SHORT)rclCWM.xLeft,
                    (SHORT)rclCWM.yBottom,
                    (SHORT)(rclCWM.xRight - rclCWM.xLeft),
                    (SHORT)(rclCWM.yTop - rclCWM.yBottom),
                    SWP_MOVE | SWP_SIZE);

   /*
    * The position of the top-left corner of the window rectangle is
    * recorded and selected as the current position before the image is
    * drawn.
    */

    WinQueryWindowRect(vhwndClient, &rcl);
    vptlSave.x = rcl.xLeft;
    vptlSave.y = rcl.yTop;
    GpiSetCurrentPosition(vhps, &vptlSave);

   /*
    * set the scroll-bar range and slider position and size
    */
    ScrollBars((USHORT)(rcl.xRight - rcl.xLeft),
               (USHORT)(rcl.yTop - rcl.yBottom));

}   /* End of SizeMainWindow */

/**************************************************************************
 *
 *  Name       : Scroll()
 *
 *  Description: Scrolls the window contents horizontally or
 *               vertically.
 *
 *  Concepts   : Routine is called from SizeVScroll() and
 *               SizeHScroll()
 *               - reposition slider
 *               - scroll the window contents in the required
 *                 direction by the required amount.
 *
 *  API's      :  WinSendMsg
 *                WinWindowFromID
 *                WinScrollWindow
 *
 *  Parameters :  lThmbPos =  new thumb position
 *                lDeltaSc =  scrolled amount
 *                sScrollType =  horizontal/vertical scroll ?
 *                prclInvalid) = invalidated area
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID Scroll(
            LONG   lThmbPos,        /* new thumb position */
            LONG   lDeltaSc,        /* scrolled amount */
            SHORT  sScrollType,     /* horizontal/vertical scroll ? */
            PRECTL prclInvalid)     /* invalidated area */
{
   unsigned Fid;

   /* determine type of scroll operation */
   Fid = sScrollType == VERTSCROLL ? FID_VERTSCROLL : FID_HORZSCROLL;

  /*
   * reposition the slider
   */
   WinSendMsg(WinWindowFromID(vhwndFrame, Fid),
              SBM_SETPOS,
              MPFROMLONG(lThmbPos),
              (MPARAM)NULL);

  /*
   * scroll the window contents
   */

   if (sScrollType == VERTSCROLL)

       /* vertical scroll */
       WinScrollWindow(vhwndClient,
                       0,
                       (SHORT)lDeltaSc,
                       (PRECTL)NULL,
                       (PRECTL)NULL,
                       (HRGN)NULL,
                       (PRECTL)prclInvalid,
                       0);
   else

       /* horizontal scroll */
       WinScrollWindow(vhwndClient,
                       (SHORT)lDeltaSc,
                       0,
                       (PRECTL)NULL,
                       (PRECTL)NULL,
                       (HRGN)NULL,
                       (PRECTL)prclInvalid,
                       0);

}   /* End of Scroll */


/**************************************************************************
 *
 *  Name       : ScrollBars()
 *
 *  Description: Prepares the main window for scrolling (i.e.
 *               set up the scrollbars appropriately)
 *
 *  Concepts   : Routine is called from WndSize() and SizeMain()
 *               - set scrollbar ranges
 *               - set slider size relative to the current window
 *                 size
 *
 *  API's      :  WinSendMsg
 *                GpiQueryCurrentPosition
 *
 *  Parameters :  usWidth  =  width
 *                usHeight =  height
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID ScrollBars(USHORT usWidth, USHORT usHeight)
{
   POINTL ptlCur;

  /*
   * calculate the full scrolling range
   */
   vulScrollXMax = vsizlImg.cx - usWidth;
   vulScrollYMax = vsizlImg.cy - usHeight;

  /*
   * The page-scrolling action scrolls the image by half of the window
   * extent in both axes.
   */
   ulScrollXPage = usWidth  / 2;
   ulScrollYPage = usHeight / 2;

  /*
   * get current position, which is at the top-left of the image
   */
   GpiQueryCurrentPosition(vhps, &ptlCur);

  /*
   * The scroll-bar slider is positioned according to the position
   * of the image in the newly-sized window.
   */
   WinSendMsg(vhwndHScroll,
              SBM_SETSCROLLBAR,
              MPFROMLONG(-ptlCur.x),                  /* slider position */
              MPFROM2SHORT(0, (SHORT)vulScrollXMax));   /* range         */

   WinSendMsg(vhwndVScroll,
              SBM_SETSCROLLBAR,
              MPFROMLONG(ptlCur.y - usHeight),        /* slider position */
              MPFROM2SHORT(0, (SHORT)vulScrollYMax));   /* range         */

   WinSendMsg(vhwndHScroll,
              SBM_SETTHUMBSIZE,
              MPFROM2SHORT(usWidth, (SHORT)(vsizlImg.cx)),
              (MPARAM)0L);

   WinSendMsg(vhwndVScroll,
              SBM_SETTHUMBSIZE,
              MPFROM2SHORT(usHeight, (SHORT)(vsizlImg.cy)),
              (MPARAM)0L);

}  /* End of ScrollBars */

/**************************************************************************
 *
 *  Name       : SizeCalculateMaxWindow()
 *
 *  Description: Calculates the maximized window size, which
 *               must not exceed the size of the loaded image.
 *
 *  Concepts   : Routine is called from SizeMain(), LoadImage(),
 *               MainWndProc() and FrameWndProc()
 *
 *               max_window_size = min(screen_size, image_size)
 *
 *  API's      :  [none]
 *
 *  Parameters :  prclBound = points to the bounding rectangle
 *
 *  Return     :   TRUE  - if image loaded and in 'Detail' mode
 *                 FALSE - if either the image isn't loaded or
 *                       the application is not in 'Detail' mode
 *
 *************************************************************************/
BOOL SizeCalculateMaxWindow(prclBound)
PRECTL prclBound;
{
  /*
   * Calculate the maximized window size. If an image has been loaded
   * in 'Detail' mode, the maximized window is the smaller of the
   * screen and the image.
   */

   if (vfImgLoaded && vfDetail)
   {
       if (vsizlImg.cx > (vlYScreen - vlcxVScroll))
       {
           prclBound->xLeft  = -vlcxBorder;
           prclBound->xRight = prclBound->xLeft + vlYScreen +
                               (2*vlcxBorder);
       }
       else
       {
           prclBound->xLeft  = 0;
           prclBound->xRight = prclBound->xLeft +
                               vsizlImg.cx + vlcxVScroll +
                               (2*vlcxBorder);
       }

       if (vsizlImg.cy >
           (vlYScreen - vlcyHScroll - vlcyMenu - vlcyTitle))
       {
           prclBound->yBottom = -vlcyBorder;
           prclBound->yTop = prclBound->yBottom + vlYScreen +
                             (2*vlcyBorder);
       }
       else
       {
           prclBound->yTop = vlYScreen;
           prclBound->yBottom = prclBound->yTop -
                                (vsizlImg.cy + vlcyTitle + vlcyMenu +
                                 vlcyHScroll + (2*vlcyBorder));
       }
       return  TRUE;
   }
   else
       /* error if not in 'Detail' mode */
       return FALSE;
}   /* End of SizeCalculateMaxWindow */

/**************************************************************************
 *
 *  Name       : SizeVScroll(mp2, rcl, ptl)
 *
 *  Description: Implements vertical scrolling in 'Detail' mode
 *
 *  Concepts   : Routine is called MainWndProc() and it invokes
 *               Scroll().
 *
 *               -adjust current position
 *               -redraw image at new position
 *
 *  API's      : GpiSetCurrentPosition
 *               WinInvalidateRect
 *
 *  Parameters :  mp2 = second message parameter
 *                rcl = rectangle
 *                ptl = new position
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SizeVScroll(MPARAM mp2, RECTL rcl, POINTL ptl)
{
   LONG   lDeltaScr;

   /* act on the type of vertical scroll message */
   switch (SHORT2FROMMP(mp2))
   {
       case SB_LINEDOWN:

          /*
           * line-down processing
           */
           if ((ULONG)(ptl.y - rcl.yTop) < vulScrollYMax)
           {
               lDeltaScr = ptl.y;
               ptl.y +=
                   ((ULONG)(ptl.y - rcl.yTop) + ulScrollYLine < vulScrollYMax) ?
                   ulScrollYLine : vulScrollYMax - (ptl.y - rcl.yTop);
               lDeltaScr = ptl.y - lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll((ptl.y-rcl.yTop), lDeltaScr, VERTSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_LINEUP:

          /*
           * line-up processing
           */
           if (ptl.y > rcl.yTop)
           {
               lDeltaScr = ptl.y;
               ptl.y -= (ptl.y > rcl.yTop + ulScrollYLine) ?
                         ulScrollYLine : ptl.y - rcl.yTop;
               lDeltaScr = ptl.y - lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll((ptl.y-rcl.yTop), lDeltaScr, VERTSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_PAGEDOWN:

          /*
           * page-down processing
           */
           if ((ULONG)(ptl.y - rcl.yTop) < vulScrollYMax)
           {
               lDeltaScr =
                   ((ULONG)(ptl.y - rcl.yTop) + ulScrollYPage < vulScrollYMax) ?
                    ulScrollYPage : vulScrollYMax -
                    (SHORT)(ptl.y - rcl.yTop);
               ptl.y += lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll((ptl.y-rcl.yTop), lDeltaScr, VERTSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_PAGEUP:

          /*
           * page-up processing
           */
           if (ptl.y > rcl.yTop)
           {
               lDeltaScr = (ptl.y > rcl.yTop + ulScrollYPage) ?
                            ulScrollYPage : (SHORT)(ptl.y - rcl.yTop);
               ptl.y -= lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll((ptl.y-rcl.yTop), -lDeltaScr, VERTSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_SLIDERPOSITION:

          /*
           * adjust the slider position
           */
           lDeltaScr = ptl.y;
           ptl.y = rcl.yTop + SHORT1FROMMP(mp2);
           lDeltaScr = ptl.y - lDeltaScr;
           GpiSetCurrentPosition(vhps, &ptl);
           Scroll((LONG)(SHORT1FROMMP(mp2)), lDeltaScr, VERTSCROLL, &rcl);
           WinInvalidateRect(vhwndClient, &rcl, FALSE);
           break;

   }
}   /* End of SizeVScroll() */

/**************************************************************************
 *
 *  Name       : SizeHScroll(mp2, rcl, ptl)
 *
 *  Description: Implements horizontal scrolling in 'Detail' mode
 *
 *  Concepts   : Routine is called MainWndProc() and it invokes
 *               Scroll().
 *
 *               -adjust current position
 *               -redraw image at new position
 *
 *  API's      : GpiSetCurrentPosition
 *               WinInvalidateRect
 *
 *  Parameters :  mp2 = second message parameter
 *                rcl = rectangle
 *                ptl = new position
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SizeHScroll(MPARAM mp2, RECTL rcl, POINTL ptl)
{
   LONG   lDeltaScr;

   /* act on the type of horizontal scroll message */
   switch(SHORT2FROMMP(mp2))
   {
       case SB_LINERIGHT:

          /*
           * line-right processing
           */
           if ((vsizlImg.cx + ptl.x) > rcl.xRight)
           {
               lDeltaScr = ptl.x;
               ptl.x -=
                   ((vsizlImg.cx + ptl.x) > rcl.xRight+ulScrollXLine) ?
                   ulScrollXLine : ptl.x + vsizlImg.cx - rcl.xRight;
               lDeltaScr = ptl.x - lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll(-ptl.x, lDeltaScr, HORZSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_LINELEFT:

          /*
           * line-left processing
           */
           if (ptl.x < rcl.xLeft)
           {
               lDeltaScr = ptl.x;
               ptl.x += ((ULONG)(rcl.xLeft - ptl.x) > ulScrollXLine) ?
                         ulScrollXLine : rcl.xLeft - ptl.x;
               lDeltaScr = ptl.x - lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll(-ptl.x, lDeltaScr, HORZSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_PAGERIGHT:

          /*
           * page-right processing
           */
           if ((vsizlImg.cx + ptl.x) > rcl.xRight)
           {
               lDeltaScr =
                  ((vsizlImg.cx+(LONG)ptl.x) > rcl.xRight+ulScrollXPage) ?
                  ulScrollXPage : (vsizlImg.cx + ptl.x - rcl.xRight);
               ptl.x -= (LONG)lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll(-ptl.x, -lDeltaScr, HORZSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_PAGELEFT:

          /*
           * page-left processing
           */
           if (ptl.x < rcl.xLeft)
           {
               lDeltaScr = ((ULONG)(rcl.xLeft - ptl.x) > ulScrollXPage) ?
                            ulScrollXPage : (SHORT)(rcl.xLeft - ptl.x);
               ptl.x += lDeltaScr;
               GpiSetCurrentPosition(vhps, &ptl);
               Scroll(-ptl.x, lDeltaScr, HORZSCROLL, &rcl);
               WinInvalidateRect(vhwndClient, &rcl, FALSE);
           }
           break;

       case SB_SLIDERPOSITION:

          /*
           * adjust the slider position
           */
           lDeltaScr = ptl.x;
           ptl.x = (LONG)rcl.xLeft - (LONG)SHORT1FROMMP(mp2);
           lDeltaScr = ptl.x - lDeltaScr;
           GpiSetCurrentPosition(vhps, &ptl);
           Scroll((LONG)(SHORT1FROMMP(mp2)), lDeltaScr, HORZSCROLL, &rcl);
           WinInvalidateRect(vhwndClient, &rcl, FALSE);
           break;

       default:
           break;
   }
}   /* End of SizeHScroll */

/**************************************************************************
 *
 *  Name       : SizePositionImage(mp2)
 *
 *  Description: Positions the image correctly within a sized window.

 *  Concepts   : Routine is called MainWndProc() and it invokes
 *               ScrollBars().
 *
 *               - adjust current position of image
 *               - adjust range & position of scrollbars
 *
 *  API's      : GpiSetCurrentPosition
 *               WinQueryWindowRect
 *               GpiQueryCurrentPosition
 *
 *  Parameters :  mp2 = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SizePositionImage(MPARAM mp2)
{
   RECTL  rcl;            /* new window rectangle              */
   POINTL ptl;            /* new current position              */

   WinQueryWindowRect(vhwndClient, &rcl);
   GpiQueryCurrentPosition(vhps, &ptl);

  /*
   * The WM_QUERYTRACKINFO processing ensures that window size never
   * exceeds image size. Here, however, image position in relation to
   * window limits is checked.
   *
   * Set current position such that the image top and right edges are
   * at least at the window limits.
   */

   ptl.y = (ptl.y < rcl.yTop) ? rcl.yTop : ptl.y;
   ptl.x = ((ptl.x + vsizlImg.cx) < rcl.xRight) ?
            (rcl.xRight - rcl.xLeft) - vsizlImg.cx : ptl.x;
   GpiSetCurrentPosition(vhps, &ptl);

  /*
   * adjust scroll-bar positioning and range to new window size
   */
   ScrollBars(SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
}  /* End of SizePositionImage() */
/***************************  End of img_size.c  *************************/

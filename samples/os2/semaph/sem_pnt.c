/*static char *SCCSID = "@(#)sem_pnt.c	6.12 92/02/19";*/
/*==============================================================*\
 *  SEM_PNT.C - routines for the painting of the main window    *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the code for the main client window    *
 *  painting                                                    *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *      DrawResource(PRECTL, COLOR) - Draws colored square      *
 *      DrawStats(ULONG)  update the statistics for a thread    *
 *      ChangeResource(COLOR) - Changes resource color          *
 *      InitSemaphExample(VOID) - Sets initial colors           *
 *      SetRectPositions(HWND) - Calculate rectangle locations  *
 *      MyOffsetRect(PRECTL, LONG, LONG) - Offsets consumer     *
 *                                         rectangles from      *
 *                                         resource             *
 *      MainPaint(hwnd) - main WM_PAINT processing routine      *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
/* static variables - only visible within this file */
static LONG  yStatsTop, yStatsBtm, xStatsRight, yBoardTop, yDescGlobal;
SHORT  xWidth;

LONG colors[MAXUSERS]= {
    CLR_WHITE, CLR_BLACK, CLR_BLUE, CLR_RED, CLR_PINK, CLR_GREEN,
    CLR_CYAN, CLR_YELLOW, CLR_DARKGRAY, CLR_DARKBLUE, CLR_DARKRED,
    CLR_DARKPINK, CLR_DARKGREEN, CLR_DARKCYAN, CLR_BROWN, CLR_PALEGRAY};

LONG clrText[MAXUSERS]= {
    CLR_BLACK, CLR_WHITE, CLR_YELLOW, CLR_GREEN, CLR_BLACK, CLR_YELLOW,
    CLR_WHITE, CLR_RED, CLR_WHITE, CLR_BLUE, CLR_RED,
    CLR_PINK, CLR_GREEN, CLR_CYAN, CLR_WHITE, CLR_DARKGRAY};

/****************************************************************\
 * Routine to draw the resource rectangle.                      *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     DrawResource(PRECTL, COLOR)                       *
 *                                                              *
 *  Purpose:  Color 1 of the 64 rectangles on the playing board.*
 *                                                              *
 *  Usage:    Called from ThreadConsumer in semaph.c when       *
 *            resource becomes owned by a thread.               *
 *                                                              *
 *  Method:   Get the PS from global hwndMain and  WinFillRect. *
 *            This rountine is included so that no access       *
 *            to the resouce is made outside of paint.c.        *
 *            This is to produce code which is more             *
 *            object orientated.                                *
 *                                                              *
 *  Returns:  none                                              *
 *                                                              *
\****************************************************************/
VOID DrawResource(PRECTL prcl, COLOR color)
{
   HPS hps;

   hps = WinGetPS(hwndMain);
   /* this code is related to the WinFillRect in DrawRects.
    * they should stay in synch with each other.
    */
   WinFillRect(hps,prcl,color);
   WinReleasePS(hps);
   return;
}

/****************************************************************\
 * update the statistics (number of hits) for a thread          *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     DrawStats(ULONG)                                  *
 *                                                              *
 *  Purpose:  update statistics display for for a thread        *
 *                                                              *
 *  Usage:    Called from ThreadConsumer in semaph.c when       *
 *            resource becomes owned by a thread.               *
 *                                                              *
 *                                                              *
 *  Method:   Get the PS, set colors, print number of hits for  *
 *            the thread identified by usMyID.                  *
 *            This rountine is included so that no access       *
 *            to the resouce is made outside of paint.c.        *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID DrawStats(ULONG usMyID)
{
   HPS hps;
   POINTL  ptl;
   THRDATA *pThr = &thrConsumers[usMyID];
   CHAR    szBuf [CNT_SZHITS];

   ptl.x = pThr->rcl.xLeft;
   ptl.y = pThr->rcl.yBottom;
   /* sprintf is non-reentrant, so it is used only when
    * have the mutex.
    */
   sprintf (szBuf, "%lu", pThr->lHits);
   hps = WinGetPS(hwndMain);
   GpiSetBackColor (hps, colors [usMyID]);
   GpiSetColor (hps, clrText [usMyID]);
   GpiCharStringPosAt (hps, &ptl, &pThr->rcl, CHS_CLIP | CHS_OPAQUE,
    strlen (szBuf), szBuf, NULL);
   WinReleasePS(hps);
   return;
}

/****************************************************************\
 *  Routine to paint rectangles in main window.                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     DrawRects(HPS)                                    *
 *                                                              *
 *  Purpose:  Does painting of rectangles.                      *
 *                                                              *
 *  Usage:    Called from MainPaint when a paint message        *
 *            is received after the semaphore sample has        *
 *            been started.                                     *
 *                                                              *
 *  Method:   Uses WinFillRect API to color all owned squares on*
 *            the square "playing board". Updates stats for     *
 *            every active consumer thread. Prints the static   *
 *            informational text.                               *
 *                                                              *
 *  Returns:  none.                                             *
 *                                                              *
\****************************************************************/
VOID DrawRects(HPS hps)
{
   INT i;
   POINTL  ptl;
                            /* Fill all rectangles in consumer array....
                                as well as rectangle for resource.       */
   if (fSemaphoreWasStarted)
      {
       for ( i = 0; i < MAXRESOURCES; i++ )
       {
           if (aSquares[i].usOwner < MAXUSERS &&
            aSquares[i].usOwner != (USHORT)UNOWNED) {
               /* this line is identical to that in DrawResource.
               * they should stay in synch with each other.
               */
               WinFillRect (hps, &aSquares[i].rcl, colors[aSquares[i].usOwner]);
           }
       }
   }
                        /* print dividing line at bottom of stat display */
   ptl.x = 0;
   ptl.y = yStatsBtm - 2;
   GpiMove (hps, &ptl);
   ptl.x = xWidth;
   GpiLine (hps, &ptl);

   /* print statistics for each user */
   for (i = 0; i < (INT) cNumUsers; i++)
   {
       DrawStats ((USHORT) i);
   }
   return;
}

/****************************************************************\
 * Routine to size and set the positions of rectangle.          *
 *--------------------------------------------------------------*
 *                                                              *
 * Name:    SetRectPositions (xClient, yClient, yChar, yDesc)   *
 *          SHORT   xClient;    width of client window          *
 *          SHORT   yClient;    height of client window         *
 *          SHORT   yChar;      maximum baseline extender       *
 *          SHORT   yDesc;      maximum char descender          *
 *                                                              *
 * Purpose: Given above information about the window, calculate *
 *          the area need for the square game board, the        *
 *          statistics display, and informative text.           *
 *                                                              *
 * Usage:   Called from MainWndProc whenever a size message     *
 *          is received.                                        *
 *                                                              *
 * Method:  Determines space needed for statistics display from *
 *          the font information given as parameters to this    *
 *          procedure.  Then calculates the 18 values which     *
 *          describe a chess by distances from a corner along 2 *
 *          adjacent sides. These are stored in arrays aX and aY*
 *          Given these values, rectangles are calculated for   *
 *          each square on the grid, and stored as part of the  *
 *          information about those squares. Then the size of   *
 *          the rectangles used for the stat display at the top *
 *          of the window are calculated, and stored in the     *
 *          struct describing the thread. Finally, the position *
 *          of the line of informative text which divides the   *
 *          two regions of colored squares is derived.          *
 *                                                              *
 * Returns: none                                                *
 *                                                              *
\****************************************************************/
VOID SetRectPositions(SHORT xClient, SHORT yClient, SHORT yChar, SHORT yDesc)
{
   INT     i;
   INT     dx;
   LONG aX [CNT_POINTS_EDGE];
   LONG aY [CNT_POINTS_EDGE];
   SHORT xScale, yScale;
                                                 /* top of stat display */
   yStatsTop = yClient;
                                              /* bottom of stat display */
   yStatsBtm = yClient - yChar;
                                               /* width of stat display */
   xStatsRight = xClient;

               /* dx is the "scaling factor" for the statistics display */
   dx = (INT) ( (double)xStatsRight / (double)cNumUsers );

                                     /* rectangles for the stat display */
   for (i = 0; i < (INT) cNumUsers; i++)
   {
      thrConsumers[i].rcl.yBottom = yStatsBtm;
      thrConsumers[i].rcl.yTop = yStatsTop;
      thrConsumers[i].rcl.xLeft = i * dx;
      thrConsumers[i].rcl.xRight = (i+1) * dx;
   }
                                              /* consume any unused edge */
   thrConsumers [cNumUsers - 1].rcl.xRight = xClient;
   yClient = yStatsBtm - 2;
          /* the scaling factors determine how large each square will be */
   xScale =  xClient / CNT_SQUARES_EDGE;
   yScale =  yClient / CNT_SQUARES_EDGE;

                                /* the entire game board is described by
                                   the points that are calculated here.  */
   for (i = 0; i < CNT_POINTS_EDGE; i++)
   {
       aX[i] = i * xScale;
       aY[i] = i * yScale;
   }
                                           /* consume any unused portion */
   aX[CNT_SQUARES_EDGE] = aX[CNT_SQUARES_EDGE] + xClient % CNT_SQUARES_EDGE;
   aY[CNT_SQUARES_EDGE] = aY[CNT_SQUARES_EDGE] + yClient % CNT_SQUARES_EDGE - 1;

                                /* now plug the points just calculated into
                                   the rectangles that make up the board */
   for (i = 0; i < MAXRESOURCES; i++)
   {
       aSquares[i].rcl.xLeft = aX [i % CNT_SQUARES_EDGE];
       aSquares[i].rcl.xRight = aX [(i % CNT_SQUARES_EDGE) + 1];
       aSquares[i].rcl.yBottom = aY [i / CNT_SQUARES_EDGE];
       aSquares[i].rcl.yTop = aY [(i / CNT_SQUARES_EDGE) + 1];
   }
                                           /* store the descender for later */
   yDescGlobal = yDesc;
                                  /* store top of board for later reference */
   yBoardTop = aY[CNT_SQUARES_EDGE];
       /* finally, store the width of the client window for later reference */
   xWidth = xClient;
   return;
}

/****************************************************************\
 *   client painting routine                                    *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainPaint(hwnd)                                     *
 *                                                              *
 *  Purpose: Paints the main client window.                     *
 *                                                              *
 *  Usage:  Routine is called whenver the client window         *
 *          procedure receives a WM_PAINT message               *
 *                                                              *
 *  Method:                                                     *
 *          - begins painting by calling WinBeginPaint          *
 *            and retriving the HPS for the window              *
 *          - performs any painting desired.  Currently the     *
 *            only painting it does is to draw rectangles on    *
 *            the screen if semaphore is started.               *
 *          - ends painting by calling WinEndPaint              *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID MainPaint( HWND hwnd )
{
   HPS hps=0L;

   hps = WinBeginPaint(hwnd, 0L, 0L);
   GpiErase (hps);
   DrawRects (hps);
   WinEndPaint(hps);
   return;
}                                               /* MainPaint() */

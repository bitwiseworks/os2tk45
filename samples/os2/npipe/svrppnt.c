/*==============================================================*\
 *  svrppnt.c - routines for the painting of the main window
 *      Copyright 1992, IBM Corp.
 *--------------------------------------------------------------
 *
 *  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *  sample code created by IBM Corporation. This sample code is not
 *  part of any standard or IBM product and is provided to you solely
 *  for  the purpose of assisting you in the development of your
 *  applications.  The code is provided "AS IS", without
 *  warranty of any kind.  IBM shall not be liable for any damages
 *  arising out of your use of the sample code, even if they have been
 *  advised of the possibility of   such damages.                                                     *
 *
 *--------------------------------------------------------------
 *
 *  This module contains the code for the main client window
 *  painting
 *
\*==============================================================*/



/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_GPICONTROL
#define INCL_GPIPRIMITIVES
#define INCL_GPILCIDS
#define INCL_DOSPROCESS
#define INCL_WINFRAMEMGR
#define INCL_WINSCROLLBARS

#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include "svrpmain.h"
#include "svrpglbl.h"
/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

#include "svrpxtrn.h"


/* returns offset from beginning of circular array */
#define  BUFF_OFFSET(index, offset)  (((index)+(offset) < MAX_MESSAGES) ? (index)+(offset) : (index)+(offset)-MAX_MESSAGES)

/*--------------------------------------------------------------*\
 *  Global (module) variables
\*--------------------------------------------------------------*/

HWND  hwndHscroll, hwndVscroll ;
LONG  sHscrollMax=0, sVscrollMax=0, sHscrollPos=0, sVscrollPos=0, cxChar,
      cxCaps, cyChar, cyDesc, cxClient, cyClient, cxTextTotal;


/****************************************************************\
 *  Main client painting routine
 *--------------------------------------------------------------
 *
 *  Name:   MainPaint(hwnd)
 *
 *  Purpose: Paints the main client window.
 *
 *  Usage:  Routine is called whenver the client window
 *          procedure receives a WM_PAINT message
 *
 *  Method:
 *          - begins painting by calling WinBeginPaint
 *              and retrieving the HPS for the window
 *          - performs any painting desired
 *          - ends painting by calling WinEndPaint
 *
 *  Returns:
 *
\****************************************************************/
VOID MainPaint(HWND hwnd,                                 /* handle to window */
               CHAR PipeMsgs[MAX_MESSAGES][MESSAGE_LEN],  /* array of messages */
               unsigned int uiIndex,                      /* beginning of circular array */
               SHORT sTotalMsgs)                          /* total number of messages */
{
   RECTL         rclInvalid;
   HPS           hps;
   LONG          sPaintBeg, sPaintEnd, sLine;
   POINTL        ptl;
   unsigned int  uiOffset;

   uiOffset = (sTotalMsgs < MAX_MESSAGES) ? 0 : uiIndex;

   hps = WinBeginPaint(hwnd, NULLHANDLE, (PRECTL)&rclInvalid);
   GpiErase(hps);

   sPaintBeg = max(0, sVscrollPos + (cyClient - rclInvalid.yTop) / cyChar);
   sPaintEnd = min(sTotalMsgs, sVscrollPos + (cyClient - rclInvalid.yBottom) / cyChar + 1);

   for (sLine = sPaintBeg ; sLine < sPaintEnd ; sLine++)
   {
      ptl.x = cxCaps - sHscrollPos ;
      ptl.y = cyClient - cyChar * (BUFF_OFFSET(uiOffset,sLine) + 1 - sVscrollPos) + cyDesc ;

      GpiCharStringAt (hps, &ptl,
                       (LONG) strlen (PipeMsgs[BUFF_OFFSET(uiOffset,sLine)]),
                       PipeMsgs[BUFF_OFFSET(uiOffset,sLine)]) ;
   }

   WinEndPaint (hps) ;
}   /* MainPaint() */


/****************************************************************\
 *  Initialization routine
 *--------------------------------------------------------------
 *
 *  Name:   InitMainWindow(hwnd, mp1, mp2)
 *
 *  Purpose: Performs initialization functions required
 *              when the main window is created.
 *
 *  Usage:  Called once during the WM_CREATE processing when
 *          the main window is created.
 *
 *  Method:
 *
 *  Returns: value to be returned from the WM_CREATE message:
 *          TRUE - window creation should stop
 *          FALSE - window creation should continue
\****************************************************************/

MRESULT InitMainWindow(HWND hwnd,      /* handle to the main client window */
                       MPARAM mp1,     /* first parameter of WM_CREATE message */
                       MPARAM mp2)     /* second parameter of WM_CREATE message */
{
   FONTMETRICS fm;
   HPS         hps;

   hps = WinGetPS (hwnd) ;
   GpiQueryFontMetrics (hps, (LONG) sizeof fm, &fm) ;

   cxChar = fm.lAveCharWidth ;
   cxCaps = fm.lEmInc ;
   cyChar = fm.lMaxBaselineExt ;
   cyDesc = fm.lMaxDescender ;

   WinReleasePS (hps) ;

   cxTextTotal = MESSAGE_LEN * cxChar ;

   hwndHscroll = WinWindowFromID (
                       WinQueryWindow (hwnd, QW_PARENT),
                       FID_HORZSCROLL) ;

   hwndVscroll = WinWindowFromID (
                       WinQueryWindow (hwnd, QW_PARENT),
                       FID_VERTSCROLL) ;

   return (MRFROMLONG(0));
}


/****************************************************************\
 *  Window sizing routine
 *--------------------------------------------------------------
 *
 *  Name:   InitMainWindow(hwnd, mp1, mp2)
 *
 *  Purpose: Adjusts scroll bars and window parameters
 *              when the main window is resized.
 *
 *  Usage:  Called when the client window receives a WM_SIZE
 *          message.
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

MRESULT MainSize(HWND hwnd,      /* handle to the main client window */
                 SHORT sTotalMsgs,
                 MPARAM mp1,     /* first parameter of WM_SIZE message */
                 MPARAM mp2)     /* second parameter of WM_SIZE message */
{

   cxClient = SHORT1FROMMP (mp2) ;
   cyClient = SHORT2FROMMP (mp2) ;

   sHscrollMax = max (0, cxTextTotal - cxClient) ;
   sHscrollPos = min (sHscrollPos, sHscrollMax) ;

   WinSendMsg (hwndHscroll, SBM_SETSCROLLBAR,
                            MPFROM2SHORT (sHscrollPos, 0),
                            MPFROM2SHORT (0, sHscrollMax)) ;

   WinEnableWindow (hwndHscroll, (BOOL)((sHscrollMax != 0) ? TRUE : FALSE)) ;

   sVscrollMax = max (0, sTotalMsgs - cyClient / cyChar) ;
   sVscrollPos = min (sVscrollPos, sVscrollMax) ;

   WinSendMsg (hwndVscroll, SBM_SETSCROLLBAR,
                            MPFROM2SHORT (sVscrollPos, 0),
                            MPFROM2SHORT (0, sVscrollMax)) ;

   WinEnableWindow (hwndVscroll, (BOOL)((sVscrollMax != 0) ? TRUE : FALSE)) ;

   return (MRFROMLONG(0));
}


/****************************************************************\
 *  Horizontal scrolling routine
 *--------------------------------------------------------------
 *
 *  Name:   MainHorizScroll(hwnd, mp1, mp2)
 *
 *  Purpose: Scrolls window horizontally.
 *
 *  Usage:  Called when the client window receives a WM_HSCROLL
 *          message.
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

MRESULT MainHorizScroll(HWND hwnd,   /* handle to the main client window */
                        MPARAM mp2)
{
   LONG sHscrollInc;

   switch (SHORT2FROMMP (mp2))
   {
      case SB_LINELEFT:
         sHscrollInc = -cxCaps ;
         break ;

      case SB_LINERIGHT:
         sHscrollInc = cxCaps ;
         break ;

      case SB_PAGELEFT:
         sHscrollInc = -8 * cxCaps ;
         break ;

      case SB_PAGERIGHT:
         sHscrollInc = 8 * cxCaps ;
         break ;

      case SB_SLIDERPOSITION:
         sHscrollInc = SHORT1FROMMP (mp2) - sHscrollPos;
         break ;

      default:
         sHscrollInc = 0;
         break ;
   }

   sHscrollInc = max (-sHscrollPos,
                 min (sHscrollInc, sHscrollMax - sHscrollPos)) ;

   if (sHscrollInc != 0)
   {
      sHscrollPos += sHscrollInc ;
      WinScrollWindow ((HWND)hwnd, -sHscrollInc, 0, (PRECTL)NULL,
                       (PRECTL)NULL, (HRGN)NULL, (PRECTL)NULL,
                       SW_INVALIDATERGN);

      WinSendMsg (hwndHscroll, SBM_SETPOS,
                  MPFROMSHORT (sHscrollPos), MPVOID) ;
   }
   return (MRFROMLONG(0));
}


/****************************************************************\
 *  Vertical scrolling routine
 *--------------------------------------------------------------
 *
 *  Name:   MainVertScroll(hwnd, mp2)
 *
 *  Purpose: Scrolls window vertically.
 *
 *  Usage:  Called when the client window receives a WM_VSCROLL
 *          message.
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

MRESULT MainVertScroll(HWND hwnd,      /* handle to the main client window */
                       MPARAM mp2)
{
   LONG sVscrollInc;

   switch (SHORT2FROMMP (mp2))
   {
      case SB_LINEUP:
         sVscrollInc = -1 ;
         break ;

      case SB_LINEDOWN:
         sVscrollInc = 1 ;
         break ;

      case SB_PAGEUP:
         sVscrollInc = min (-1, -cyClient / cyChar) ;
         break ;

      case SB_PAGEDOWN:
         sVscrollInc = max (1, cyClient / cyChar) ;
         break ;

      case SB_SLIDERTRACK:
         sVscrollInc = SHORT1FROMMP (mp2) - sVscrollPos;
         break ;

      default:
         sVscrollInc = 0;
         break ;
   }

   sVscrollInc = max (-sVscrollPos,
                 min (sVscrollInc, sVscrollMax - sVscrollPos)) ;

   if (sVscrollInc != 0)
   {
      sVscrollPos += sVscrollInc ;
      WinScrollWindow ((HWND)hwnd, 0, (cyChar * sVscrollInc),
                     (PRECTL)NULL, (PRECTL)NULL, (HRGN)NULL,
                     (PRECTL)NULL, SW_INVALIDATERGN);

      WinSendMsg (hwndVscroll, SBM_SETPOS,
                  MPFROMSHORT (sVscrollPos), MPVOID) ;
      WinUpdateWindow (hwnd) ;
   }
   return (MRFROMLONG(0));
}


/****************************************************************\
 *
 *  Name:   MainCharHScroll(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose:
 *
 *  Usage:
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

MRESULT MainCharHScroll(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

   return (WinSendMsg(hwndHscroll, msg, mp1, mp2));
}


/****************************************************************\
 *
 *  Name:   MainCharVScroll(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose:
 *
 *  Usage:
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

MRESULT MainCharVScroll(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

   return (WinSendMsg(hwndVscroll, msg, mp1, mp2));
}


/****************************************************************\
 *  Update messages
 *--------------------------------------------------------------
 *
 *  Name:   MainUpdateMsg(hwnd)
 *
 *  Purpose: Update portion of screen with new messages.
 *
 *  Usage:  Called when a new message is read
            from or written to the pipe.
 *
 *  Method:
 *
 *  Returns:
\****************************************************************/

VOID MainUpdateMsg(HWND hwnd,
                   CHAR PipeMsgs[MAX_MESSAGES][MESSAGE_LEN],
                   unsigned int index,
                   SHORT sTotalMsgs)
{
   RECTL rclInvalid;

   /* if it will fit in the window */
   if (sTotalMsgs <= cyClient/cyChar + sVscrollPos + 1)
   {
      rclInvalid.yBottom = max(0, cyClient - sTotalMsgs * cyChar);
      rclInvalid.yTop = (cyClient - (sTotalMsgs - sVscrollPos - 1) * cyChar);
      rclInvalid.xLeft = 0;
      rclInvalid.xRight = cxClient;
      WinInvalidateRect(hwnd, &rclInvalid, FALSE);
      MainPaint(hwnd, PipeMsgs, index, sTotalMsgs);
   }

   /* if lines exceed window */
   if (sTotalMsgs > cyClient/cyChar)
   {
      WinEnableWindow (hwndVscroll, TRUE) ;
      sVscrollMax = sTotalMsgs - cyClient / cyChar;
      sVscrollPos = min (sVscrollPos, sVscrollMax) ;

      WinSendMsg (hwndVscroll, SBM_SETSCROLLBAR,
                               MPFROM2SHORT (sVscrollPos, 0),
                               MPFROM2SHORT (0, sVscrollMax)) ;

      /* if new msg is just off screen */
      if (sTotalMsgs == cyClient/cyChar + sVscrollPos + 1)
      {
         MainVertScroll(hwnd, MPFROM2SHORT(0, SB_LINEDOWN));
      }

      /* tell user message was received */
      else
      {
         DosBeep(1000, 100);
      }
   }
}


/*
 *  Empty Client window of messages
 */
VOID MainPurgeWindow()
{

   sVscrollMax = 0;
   sVscrollPos = 0;
   WinSendMsg (hwndVscroll, SBM_SETSCROLLBAR,
                            MPFROM2SHORT (sVscrollPos, 0),
                            MPFROM2SHORT (0, sVscrollMax)) ;

   WinEnableWindow (hwndVscroll, FALSE) ;
   WinInvalidateRect(hwndMain, NULL, FALSE);
}

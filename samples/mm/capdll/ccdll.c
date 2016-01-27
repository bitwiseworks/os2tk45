/******************************************************************************
 * File Name   :  CCDLL.C
 *
 * Description :  This file contains the C source code required for the
 *                caption DLL.
 *
 * Concepts    :  This caption DLL is part of MMPM/2 sample closed captioning
 *                system.  Sample captioning system is composed of three
 *                parts: Caption Creation Utility, Caption DLL (this DLL),
 *                and Caption Sample Application. The Caption Creation
 *                Utility creates a "captioned" file.  This is a text file
 *                with timing information relating to it's associated audio
 *                file. The The Caption DLL (this DLL) provides 3 API's that
 *                drive the display and management of a "caption window" in a
 *                PM application.  The Caption Sample Application illustrates
 *                how an application uses the 3 API's provided by the caption
 *                DLL (this DLL) to take advantage of it's services.
 *
 *                This sample captioning system is provided to illustrate one
 *                of the many ways that captioning may be implemented using
 *                MMPM/2. As with all MMPM/2 samples, this may be used as it
 *                is provided, or it may be modified in any way you prefer.
 *                Please refer to the Application Programmers Reference for
 *                more information on this sample captioning system.
 *
 * MMPM/2 API's:  List of all MMPM/2 API's that are used in
 *                this module
 *
 *                mciSendString
 *                   MCI_SET_POSITION_ADVISE
 *
 * Required
 *    Files    :  ccdll.c           Source Code.
 *                captions.h        Include file.
 *                makefile          Make file.
 *                cap.def           Linker definition file.
 *
 * Copyright (C) IBM 1993
 ******************************************************************************/

#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_WIN
#define INCL_DOSPROCESS
#define INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WINDIALOGS
#define INCL_DOSMODULEMGR

#include <stdlib.h>                  /*---------------------------------------*/
#include <string.h>                  /*   Include standard c-library files    */
#include <stdio.h>                   /*---------------------------------------*/

#include <os2.h>                     /*-----  OS/2 system header files -------*/

#include <os2me.h>                   /*-- Headers for multimedia extensions --*/

#include "captions.h"                /*--- General closed-captioning header --*/


/*------------------------- Local definitions --------------------------------*/

#define CC_TIMER_ID           1110
#define CC_TIMER_FREQ         0
#define CC_DEFAULT_TEXT_LINES 3      /* Default window height in text lines   */
#define CC_DEFAULT_CHARS_WIDE 35     /* Default window width in characters    */

#define MAX_TEXT_LINES        500

#define CHAR_NULL             0   /*------------------------------------------*/
#define CHAR_RETURN           13  /* Control characters present in ASCII files*/
#define CHAR_EOF              26  /*------------------------------------------*/

#define CC_FILE               "CC_FILE"


/*------------ Values for ulFlags in CCINSTANCE strucuture -------------------*/

#define CC_TIMER_RUNNING   0x02   /* Set if the timer is running              */
#define CC_CAPTIONS_ACTIVE 0x04   /* Set when we are actually doing captioning*/
#define CC_DRAG_MODE       0x08   /* Set when the cap. win. is being dragged  */
#define CC_USER_POSITION   0x10   /* Set when user has set win pos            */


/*------- This is the instance data for the closed-caption window ------------*/

typedef _Packed struct _CCINSTANCE
{
   HAB      hab;               /* Anchor block for this window                */
   HWND     hwndCaption;       /* Handle of the caption window                */
   HWND     hwndParent;        /* Window in which caption window is displayed */
   HWND     hwndPosition;      /* Send position messages here                 */
   PFNWP    pfnOldWinProc;     /* Old window procedure of the parent window   */
   RECTL    rclWin;            /* Latest dimensions of caption window         */
   RECTL    rclText;           /* Size of the backup bitmap for the cap. win. */
   HPS      hpsText;           /* Pres. space handle for the backup bitmap    */
   HBITMAP  hbmText;           /* Bitmap handle for the backup bitmap         */
   HDC      hdcText;           /* Device context for the backup bitmap        */
   PVOID    pvLinedataTable;   /* ^ to table of strings and times to display  */
   PVOID    pvText;            /* Pointer to the caption file text            */
   LONG     lScrollOffset;     /* Offset if scrolling new line into window    */
   LONG     lScrollInc;        /* Scroll step size                            */
   LONG     lLineSpacing;      /* Y-pixels for each line                      */
   LONG     lLeftMargin;       /* X-pixel start for each line                 */
   LONG     lBottomLineY;      /* Y-coordinate of bottom-most line in cap. win*/
   LONG     lWindowColor;      /* Background color of window                  */
   LONG     lTextColor;        /* Color of text in window                     */
   LONG     lWindowXcoord;     /* Cap. win. position if CC_USER_POSITION set  */
   LONG     lWindowYcoord;     /* Cap. win. position if CC_USER_POSITION set  */
   USHORT   usWindowLines;     /* Number of lines to display in window        */
   USHORT   usWindowChars;     /* Width of window, in text characters         */
   USHORT   usNextlineReq;     /* Number of queued line requests              */
   USHORT   usDisplayLine;     /* Index of BOTTOM-most line in capaption win  */
   USHORT   usLineCount;       /* Number of lines in caption file             */
   ULONG    ulTimePrev;        /* Time of previous position change message    */
   MPARAM   mpMouseCoords;     /* Mouse coords at start of drag               */
   HPOINTER hptrMove;          /* Handle of the SPTR_MOVE pointer             */
   ULONG    ulFlags;           /* Flags for TRUE/FALSE data                   */
}
CCINSTANCE;

typedef CCINSTANCE FAR *PCCINSTANCE;
                                  /*------------- Line data structure --------*/
                                  /*                                          */
typedef _Packed struct _LINEDATA  /* There is one for each caption line       */
{                                 /*                                          */
   PSZ    szText;                 /* <-Pointer to a NULL-terminated text str. */
   LONG   lTextLen;               /* <-Number of characters in the string     */
   ULONG  ulTime;                 /* <-Time (MM units) to display the string  */
}                                 /*                                          */
LINEDATA;                         /* An array of structures is maintained     */
                                  /* in a table pointed to by pvLinedataTable.*/
typedef LINEDATA FAR *PLINEDATA;  /*                                          */
                                  /*------------------------------------------*/

/*----------------- Function prototype statements ----------------------------*/

MRESULT EXPENTRY CaptionWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static  APIRET   StartCaptioning( PCCINSTANCE pid, CHAR szFilename[],
                                     HWND hwndPosition, CHAR szDeviceName[] );
static  VOID     FormatTextBitmap( PCCINSTANCE pid, USHORT usLastLine );
static  APIRET   CreateBitmap( PCCINSTANCE pid );
static  APIRET   LoadCaptionFile( PCCINSTANCE pid, CHAR szCaptionFile[] );
static  VOID     StopCaptioning( PCCINSTANCE pid );
static  void     PositionCaptionWindow( PCCINSTANCE pid );

#define CC_WINCLASS     "CapClass"


/*------------------------------- Global data area ---------------------------*/
HMODULE hmodule;      /* The module handle of the captioning DLL              */

/******************************************************************************
 * Name         : ccInitialize
 *
 * Description  : This API creates a captioning window and returns the handle
 *                to the application. Call this function once to associate a
 *                captioning window with a user window.
 *
 * Parameters   : hwndParent - The handle of the user window in where the
 *                             captioning window will appear.
 *
 * Return       : ULONG      - The handle of the captioning window.  If the
 *                             function was unsuccessful, this will be NULL.
 *
 ******************************************************************************/
ULONG APIENTRY ccInitialize ( HWND hwndParent )
{
   HAB     hab;                           /* Parent window's anchor block     */
   PFNWP   pfnCaptionWindowProc;          /* Pointer to DLL entry             */
   HWND    hwndCaption;                   /* Handle of the caption window     */


   if ( hwndParent == (HWND)NULL )
      return( CCERR_INVALID_WINDOW_HANDLE );

   /*---------------- Create the captioning window ---------------------------*/
   /*                                                                         */
   /* The captioning window is created as a child of the specified user       */
   /* window.  It does not become visible until the user issues a CC_START    */
   /* message.  Note that the captioning window may be overpainted by the     */
   /* user's parent window unless the user's window has the WS_CLIPCHILDREN   */
   /* style set.                                                              */
   /*-------------------------------------------------------------------------*/

   hab = WinQueryAnchorBlock ( (HWND) hwndParent );

   WinRegisterClass ( hab, CC_WINCLASS, CaptionWindowProc,
                                        CS_SYNCPAINT | CS_SIZEREDRAW, 4 );

   hwndCaption = WinCreateWindow  ( (HWND) hwndParent, CC_WINCLASS, NULL,
                                     0, 0, 0, 1, 1, (HWND) hwndParent,
                                     HWND_TOP, CC_WINDOW_ID, NULL, NULL );

   return (ULONG) hwndCaption;
}

/******************************************************************************
 * Name         : ccSendCommand
 *
 * Description  : This API is used to control the captioning window once it
 *                has been created.
 *
 * Parameters   : usMsg - Captioning command.
 *                mp1   - Window handle of caption window.
 *                mp2   - Varies depending on command.  See commands below.
 *
 * Return       : ULONG - Captioning DLL's response to the message.
 *
 ******************************************************************************/
ULONG APIENTRY ccSendCommand ( USHORT usMsg, MPARAM mp1, MPARAM mp2 )
{
   USHORT  msg;

   switch ( usMsg )
   {                        /*---------------------- CC_START ----------------*/
      case CC_START:        /*  mp1 - Reserved.  Must be set to zero.         */
         msg = UMCC_START;  /*  mp2 - Pointer to a CC_START_PARMS structure   */
         break;             /*------------------------------------------------*/

                            /*---------------------- CC_STOP -----------------*/
      case CC_STOP:         /*  mp1 - Reserved.  Must be set to zero.         */
         msg = UMCC_STOP;   /*  mp2 - Reserved.  Must be set to zero.         */
         break;             /*------------------------------------------------*/

                            /*----------------------- CC_SET -----------------*/
      case CC_SET:          /*  mp1 - Reserved.  Must be set to zero.         */
         msg = UMCC_SET;    /*  mp2 - Pointer to a CC_SET_PARMS structure.    */
         break;             /*------------------------------------------------*/

                            /*------------- CC_STATUS ------------------------*/
      case CC_STATUS:       /*  mp1 - Reserved.  Must be set to zero.         */
         msg = UMCC_STATUS; /*  mp2 - Pointer to a CC_STATUS_PARMS structure. */
         break;             /*------------------------------------------------*/

      default:
         msg = UMCC_ERROR; /*  mp2 - Pointer to a CC_STATUS_PARMS structure. */
         break;
   }

   /*
    * After assigning the appropriate value to message, send it over to the
    * window procedure to be processed.  Obtain the handle of the caption
    * window by extracting it from message parameter 1, where it was passed in.
    */
   return (ULONG) WinSendMsg ( HWNDFROMMP(mp1), msg, mp1, mp2 );
}

/******************************************************************************
 * Name         : ccTerminate
 *
 * Description  : This API closes the captioning system and releases any
 *                resources.  Call this API once at the end of the appliaction.
 *
 * Parameters   : hwndCaption - window handle of caption window.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID APIENTRY ccTerminate (HWND hwndCaption)
{
   WinDestroyWindow ( hwndCaption );
   DosFreeModule    ( hmodule );
}

/******************************************************************************
 * Name         : CaptionWindowProc
 *
 * Description  : This function controls the caption window.  It will handle
 *                received messages from the user application.
 *
 * Concepts     : - How to handle notification messages.
 *
 * MMPM/2 API's :
 *
 * Parameters   : hwnd - Handle for the text window
 *                msg  - Message received by the text dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY CaptionWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   PCCINSTANCE      pid;         /* Pointer to the instance data for this win.*/
   PLINEDATA        pld;         /* Pointer used to scan Line Data Table      */
   PCREATESTRUCT    pcs;         /* Pointer to window creation structure      */
   PCC_START_PARMS  psp;         /* Pointer to CC_START_PARMS                 */
   PCC_SET_PARMS    pSetParms;   /* Pointer to CC_SET_PARMS structure         */
   PCC_STATUS_PARMS pStatusParms;/* Pointer to CC_STATUS_PARMS structure      */

   HPS              hpsPaint;    /*-------------------------------------------*/
   RECTL            rclPaint;    /* Variables used in painting caption win.   */
   POINTL           aptlPaint[4];/*-------------------------------------------*/

   USHORT           usOutputLine;/*--------  These variables are used  -------*/
   ULONG            ulTime;      /*          when calculating which           */
   USHORT           usTestLine;  /*--------  caption line to show next -------*/

   ULONG            ulMemreq;    /* Bytes of memory needed for instance data. */
   RECTL            rclParent;   /*     Updated dimensions of parent window   */
   LONG             lXcoord, lYcoord;/* Updated origin of captioning window   */
   APIRET           apiret;      /*  Value recturned with UMCC_START message  */
   SWP              swp;

                                                      /*----------------------*/
   if ( msg != WM_CREATE )                            /* Get a pointer to the */
      pid = (PCCINSTANCE) WinQueryWindowPtr(hwnd, 0); /* window instance data */
                                                      /*----------------------*/
   switch ( msg )
   {
      case WM_CREATE:
         DosAllocMem ( (PPVOID) &pid, sizeof(CCINSTANCE), fALLOC );
         WinSetWindowPtr ( hwnd, 0, pid );
         pcs = (PCREATESTRUCT) PVOIDFROMMP(mp2);

         memset ( pid, 0, (size_t) sizeof(CCINSTANCE) );  /*------------------*/
         pid->hab           = WinQueryAnchorBlock( hwnd );/*                  */
         pid->hwndCaption   = hwnd;                       /*                  */
         pid->hwndParent    = pcs->hwndParent;            /*                  */
         pid->usWindowLines = CC_DEFAULT_TEXT_LINES;      /* During WM_CREATE */
         pid->usWindowChars = CC_DEFAULT_CHARS_WIDE;      /* processing, we   */
         pid->lWindowColor  = CLR_WHITE;                  /* allocate and     */
         pid->lTextColor    = CLR_DARKBLUE;               /* initialize the   */
         pid->lLeftMargin   = 10;                         /* window instance  */
         pid->lScrollInc    = 3;                          /* data.            */
         pid->rclWin.xRight = pcs->cx;                    /*                  */
         pid->rclWin.yTop   = pcs->cy;                    /*------------------*/
         pid->hptrMove      =
                     WinQuerySysPointer( HWND_DESKTOP, SPTR_MOVE, FALSE );
         break;


      case WM_SIZE:                               /*--------------------------*/
         WinQueryWindowRect( hwnd, &pid->rclWin );/* Update the instance data */
         break;                                   /* every time the window is */
                                                  /* resized.                 */
                                                  /*--------------------------*/

      /*---------------- Dragging the caption window -------------------------*/
      /*                                                                      */
      /*  The next three messages (WM_BUTTON1DOWN, WM_MOUSEMOVE, WM_BUTTON1UP)*/
      /*  are used for dragging the caption window.                           */
      /*                                                                      */
      /*  (1) When we receive a WM_BUTTON1DOWN message, we store the          */
      /*      coordinates of the pointer (relative to the Caption Window) in  */
      /*      the window instance data.                                       */
      /*                                                                      */
      /*  (2) When we receive a WM_MOUSEMOVE message, we check to see if the  */
      /*      pointer is still at the same relative position in the Caption   */
      /*      Window, and...                                                  */
      /*                                                                      */
      /*  (3) If the pointer has moved relative to the window, we move the    */
      /*      window enough to restore the original pointer position.         */
      /*                                                                      */
      /*  (4) When the user releases the left mouse button, we end drag mode  */
      /*      and...                                                          */
      /*                                                                      */
      /*  (5) Save the new window position for future captioning requests     */
      /*                                                                      */
      /*----------------------------------------------------------------------*/

      case WM_BUTTON1DOWN:
         WinSetCapture ( HWND_DESKTOP, hwnd );
         WinSetPointer ( HWND_DESKTOP, pid->hptrMove );
         pid->ulFlags |= CC_DRAG_MODE;
         pid->mpMouseCoords = mp1;               /* (1) Save pointer coords   */
         return 0;


      case WM_MOUSEMOVE:
         if ( pid->ulFlags & CC_DRAG_MODE )
         {
            if ( mp1 != pid->mpMouseCoords )     /* (2) Check for alignment   */
            {
               WinQueryWindowPos ( hwnd, &swp );
               swp.x += ( SHORT1FROMMP(mp1) - SHORT1FROMMP(pid->mpMouseCoords));
               swp.y += ( SHORT2FROMMP(mp1) - SHORT2FROMMP(pid->mpMouseCoords));
               WinSetWindowPos(hwnd, 0, swp.x, swp.y, 0, 0, SWP_MOVE );/*-(3)-*/
            }
            WinSetPointer ( HWND_DESKTOP, pid->hptrMove );
            return 0;
         }
         break;


      case WM_BUTTON1UP:
         if ( pid->ulFlags & CC_DRAG_MODE )
         {
            pid->ulFlags &= ~CC_DRAG_MODE;        /* (4) End drag mode        */
            WinSetCapture ( HWND_DESKTOP, 0 );

            WinQueryWindowPos ( hwnd, &swp );     /* (5) Store position for   */
            pid->lWindowXcoord = swp.x;           /*     future requests.     */
            pid->lWindowYcoord = swp.y;
            pid->ulFlags |= CC_USER_POSITION;
         }
         break;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  When a WM_PAINT message is received, update the caption window from the   */
/*  text bitmap.  In calculating the SOURCE blit rectangle, it is necessary to*/
/*  take out account:                                                         */
/*                                                                            */
/*    (a) The window may not be the same size as the bitmap ( horiz & vert )  */
/*    (b) The window may be scrolled with respect to the bitmap ( vert only ) */
/*                                                                            */
/*  Note:  In this iteration of the program, the window and the backup bitmap */
/*         always copy pixel-for-pixel.  However, in future versions this     */
/*         might be so.  Therefore I have put in scaling logic to accommodate */
/*         any future version which might need it.                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

      case WM_PAINT:
         hpsPaint = WinBeginPaint ( hwnd, 0, &rclPaint );

         aptlPaint[0].x = rclPaint.xLeft;  /*---------------------------------*/
         aptlPaint[0].y = rclPaint.yBottom;/* The TARGET blit rect. is always */
         aptlPaint[1].x = rclPaint.xRight; /* the same as the paint rectangle */
         aptlPaint[1].y = rclPaint.yTop;   /*---------------------------------*/

         aptlPaint[2].x =
             rclPaint.xLeft   * pid->rclText.xRight / pid->rclWin.xRight;
         aptlPaint[2].y =
             rclPaint.yBottom * pid->rclText.yTop   / pid->rclWin.yTop
                                                    + pid->lScrollOffset;
         aptlPaint[3].x =
             rclPaint.xRight  * pid->rclText.xRight / pid->rclWin.xRight;
         aptlPaint[3].y =
             rclPaint.yTop    * pid->rclText.yTop   / pid->rclWin.yTop
                                                    + pid->lScrollOffset;

         GpiSetColor    ( hpsPaint, pid->lWindowColor );/* Normal Window color*/
         GpiSetBackColor( hpsPaint, pid->lTextColor   );/* Normal Text color  */

         GpiBitBlt      ( hpsPaint, pid->hpsText, 4, aptlPaint,
                                                     ROP_SRCCOPY, BBO_IGNORE );
         WinEndPaint    ( hpsPaint );

         return (MRESULT) 0;


/*----------------------------------------------------------------------------*/
/*    We get a WM_TIMER message every time it is necessary to scroll the      */
/* captions.  The scroll increment is specified by lScrollInc in the window   */
/* instance data.  The caption window is repainted as part of the (1)         */
/* WinScrollWindow operation, because of its CS_SYNCPAINT class style.        */
/*    After each scroll operation, we (2) check to see if we have scrolled    */
/* one entire text line.  If this is so, stop the timer and (3) check         */
/* usNextlineReq to see if there are any more lines queued up to be scrolled. */
/*----------------------------------------------------------------------------*/

      case WM_TIMER:
         pid->lScrollOffset -= pid->lScrollInc;
         WinScrollWindow( hwnd, 0, pid->lScrollInc, NULL, /* (1) Scroll window*/
                          NULL, 0, NULL, SW_INVALIDATERGN );
         if ( ! pid->lScrollOffset )                    /* (2) Text line done?*/
         {
            WinStopTimer ( pid->hab, hwnd, CC_TIMER_ID );
            pid->ulFlags &= ~CC_TIMER_RUNNING;
            if ( pid->usNextlineReq ) pid->usNextlineReq--;
            if ( pid->usNextlineReq )                     /* (3) Check for    */
               WinPostMsg ( hwnd, UMCC_NEXTLINE, 0, 0 );  /*     queued lines.*/
         }
         break;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* There are two cases in processing an MM_MCIPOSITIONCHANGE message:         */
/*                                                                            */
/*    (1) The time in the message is LATER than the previous message (saved   */
/*        in the window instance data ).  In this case, check to see if it's  */
/*        time to scroll another line into the caption window.                */
/*                                                                            */
/*    (2) The time in the message is EARLIER than the previous message.  This */
/*        means that the user has rewound or backed up the audio file.  Stop  */
/*        any current scrolling operation; reposition the usDisplayLine       */
/*        pointer, and show the repositioned captions.                        */
/*                                                                            */
/* In any case, check to see if the user has requested us to forward the      */
/* position messages to his window.                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

      case MM_MCIPOSITIONCHANGE:
         if ( pid->ulFlags & CC_CAPTIONS_ACTIVE )
         {
            ulTime = (ULONG) LONGFROMMP(mp2);
            if ( ulTime >= pid->ulTimePrev )       /********* CASE 1 **********/
            {
               usTestLine = pid->usDisplayLine + pid->usNextlineReq + 1;
               if ( usTestLine >= (pid->usLineCount + 5) ) break;
               pld = (PLINEDATA) pid->pvLinedataTable + usTestLine;
               while ((usTestLine<pid->usLineCount) && (pld->ulTime<=ulTime))
               {
                  pid->usNextlineReq++;
                  if ( pid->usNextlineReq == 1 )
                     WinPostMsg(hwnd,UMCC_NEXTLINE,MPFROMLONG((LONG)ulTime), 0);
                  pld++;
                  usTestLine++;
               }
            }
            else
            {                                      /********* CASE 2 **********/
               if ( pid->ulFlags & CC_TIMER_RUNNING )
               {
                  WinStopTimer ( pid->hab, hwnd, CC_TIMER_ID );
                  pid->ulFlags &= ~CC_TIMER_RUNNING;
               }
               pld = (PLINEDATA) pid->pvLinedataTable;
               usTestLine = 0;
               while ((usTestLine<pid->usLineCount) && (pld->ulTime<=ulTime))
               {
                  usTestLine++;
                  pld++;
               }
               FormatTextBitmap ( pid, usTestLine );
               pid->usNextlineReq = 0;
               pid->usDisplayLine = usTestLine;
               WinInvalidateRect ( hwnd, 0, FALSE );
            }

            pid->ulTimePrev = ulTime;              /* Forward msg if requested*/
            if ( pid->hwndPosition != 0 )
               return WinSendMsg ( pid->hwndPosition, msg, mp1, mp2 );
         }
         return 0;


      /*-------------- User request to start captioning ----------------------*/

      case UMCC_START:
         psp = (PCC_START_PARMS) PVOIDFROMMP(mp2);
         apiret = StartCaptioning( pid, (PCHAR)psp->pszCaptionFile,
                            (HWND)psp->hwndOwner, (PCHAR)psp->pszDeviceName   );
         return (MRESULT) apiret;


      /*--------------- User request to stop captioning ----------------------*/

      case UMCC_STOP:
         StopCaptioning ( pid );
         return 0;

      /*--------------- When we receive an invalid status request.------------*/

      case UMCC_ERROR:
         return (MRESULT) CCERR_INVALID_COMMAND;

      /*----------------------------------------------------------------------*/
      /* When we receive a status request, move the requested information     */
      /* from the window instance data to the ulReturn field of the status    */
      /* parameters.                                                          */
      /*----------------------------------------------------------------------*/

      case UMCC_STATUS:
         pStatusParms = (PCC_STATUS_PARMS) PVOIDFROMMP(mp2);

         switch ( pStatusParms->ulItem )
         {
            case CC_STATUS_TEXT_COLUMNS:
               pStatusParms->ulReturn = (ULONG) pid->usWindowChars;
               break;

            case CC_STATUS_TEXT_ROWS:
               pStatusParms->ulReturn = (ULONG) pid->usWindowLines;
               break;

            case CC_STATUS_TEXT_COLOR:
               pStatusParms->ulReturn = (ULONG) pid->lTextColor;
               break;

            case CC_STATUS_BACKGROUND_COLOR:
               pStatusParms->ulReturn = (ULONG) pid->lWindowColor;
               break;

            case CC_STATUS_X_POSITION:
               pStatusParms->ulReturn = (ULONG) pid->lWindowXcoord;
               break;

            case CC_STATUS_Y_POSITION:
               pStatusParms->ulReturn = (ULONG) pid->lWindowYcoord;

            default:
              pStatusParms->ulReturn = (ULONG) CCERR_INVALID_STATUS_REQUEST;
         }
         return 0;


      /*--------------- Setting captioning parameters ------------------------*/
      /*                                                                      */
      /*  There are two phases involved in setting captioning parameters:     */
      /*                                                                      */
      /*  (1) Move the data from the UMCC_SET message to the appropriate      */
      /*      area(s) in the instance data for the captioning window.         */
      /*                                                                      */
      /*  (2) If captioning is currently in progress, update the captioning   */
      /*      window to reflect these changes...                              */
      /*                                                                      */
      /*      (2a) When changing window colors, force a repaint of the window.*/
      /*                                                                      */
      /*      (2b) When changing the number of columns or rows, we have to    */
      /*           destroy the old backup text bitmap and create a new one.   */
      /*           The captioning window must be hidden during this operation */
      /*           so it doesn't get any WM_PAINT message while the backup    */
      /*           bitmap is in limbo.  For the same reason, scrolling        */
      /*           operations and new line requests must be shut off.  Once   */
      /*           the new bitmap is made, reposition and show the resized    */
      /*           captioning window.                                         */
      /*                                                                      */
      /*      (2c) When changing the window position, move the captioning     */
      /*           window                                                     */
      /*                                                                      */
      /*----------------------------------------------------------------------*/

      case UMCC_SET:
         pSetParms = (PCC_SET_PARMS) PVOIDFROMMP(mp2);
         pid->usWindowChars = (USHORT) pSetParms->ulColumns;
         pid->usWindowLines = (USHORT) pSetParms->ulRows;
         pid->lTextColor = (LONG) pSetParms->ulTextColor;
         pid->lWindowColor = (LONG) pSetParms->ulBackgroundColor;
         pid->ulFlags |= CC_USER_POSITION;
         pid->lWindowXcoord = (LONG) pSetParms->ulXposition;
         pid->ulFlags |= CC_USER_POSITION;
         pid->lWindowYcoord = (LONG) pSetParms->ulYposition;

         if (pid->ulFlags & CC_CAPTIONS_ACTIVE) /*(2)Update caption window, if*/
         {
            WinInvalidateRect ( hwnd, NULL, FALSE );

            WinSetWindowPos ( hwnd, 0, 0, 0, 0, 0, SWP_HIDE );
            if ( pid->ulFlags & CC_TIMER_RUNNING )
            {
                WinStopTimer ( pid->hab, hwnd, CC_TIMER_ID );
                pid->ulFlags &= ~CC_TIMER_RUNNING;
            }
            pid->lScrollOffset = 0;
            pid->usNextlineReq = 0;
            apiret = CreateBitmap ( pid );

            if (apiret)
              return (MRESULT) apiret;

            FormatTextBitmap ( pid, pid->usDisplayLine );
            PositionCaptionWindow ( pid );
            WinSetWindowPos ( hwnd, 0, 0, 0, 0, 0, SWP_SHOW );

            PositionCaptionWindow ( pid );
         }

         return 0;

      /*----------------------------------------------------------------------*/
      /*  We receive this message when it is time to scroll to the next line. */
      /*  First make sure that (1) the audio file has not been backspaced     */
      /*  since the messagewas issued.  If the file has not been backspaced,  */
      /*  go ahead and (2) start the scroll operation.                        */
      /*----------------------------------------------------------------------*/

      case UMCC_NEXTLINE:
         if ( pid->ulFlags & CC_CAPTIONS_ACTIVE )
         {
            ulTime = (ULONG) LONGFROMMP(mp1);
            if (ulTime <= pid->ulTimePrev) /*(1)Check to see if audio backspcd*/
            {
               pid->usDisplayLine++;
               if ( pid->usDisplayLine < pid->usLineCount )
               {
                  FormatTextBitmap(pid, pid->usDisplayLine);/*(2)Initiate scrl*/
                  pid->lScrollOffset = pid->lLineSpacing;
                  WinStartTimer ( pid->hab, hwnd, CC_TIMER_ID, CC_TIMER_FREQ );
                  pid->ulFlags |= CC_TIMER_RUNNING;
               }
            }
         }
         return 0;

      case WM_DESTROY:
         StopCaptioning    ( pid );
         DosFreeMem        ( pid );
         break;
   }

   return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
}

/******************************************************************************
 * Name         :  StartCaptioning
 *
 * Description  :  This procedure will initiate captioning and loads the
 *                 caption file. Initializes internal data structures and the
 *                 caption window.
 *
 * MMPM/2 API's :  mciSendString
 *
 * Parameters   :  pid             Pointer to Instance Data for caption window
 *
 *                 szFilename      Caption file name (NULL terminated)
 *
 *                 hwndOwner       If you want the captioning system to pass
 *                                 position advise messages to one of your
 *                                 windows, pass the handle of that window
 *                                 here.  Otherwise pass NULL.
 *
 *                 szDeviceName    Alias/Device name of the audio device. Pass
 *                                 this so that the caption system can request
 *                                 MM_MCIPOSITIONCHANGE messages from this
 *                                 device.
 *
 * Return       :  0      -  if the operation was initiated without error.
 *                 apiret -  if an error occurred.
 *
 ******************************************************************************/
static APIRET StartCaptioning ( PCCINSTANCE pid, CHAR szFilename[],
                                     HWND hwndOwner, CHAR szDeviceName[] )
{
   APIRET apiret;                     /* Values returned from DOS operations  */
   ULONG  ulBytesReq;                 /* Bytes requested for DosAllocMem calls*/
   CHAR   szCommandString[CCHMAXPATH] =
            "setpositionadvise ";
   LONG   lSendStringRC = 0;


/*---------- Check for device ID. Return error if not found.------------------*/

   if (strlen(szDeviceName) == 0)
      return (APIRET) CCERR_NO_DEVICE_NAME;

/*---------- Stop any captioning that is currently underway ------------------*/

   if ( pid->ulFlags & CC_CAPTIONS_ACTIVE )
      StopCaptioning ( pid );
   apiret = CreateBitmap( pid );   /* Create the backup bitmap for painting   */
                                           /* captions.                       */
   if (apiret)
      return (APIRET) apiret;


/*----- Load the caption file, creating Text and Line Data Tables ------------*/

   ulBytesReq = (ULONG) ( MAX_TEXT_LINES * sizeof(LINEDATA) );
   DosAllocMem ( &pid->pvLinedataTable, ulBytesReq, fALLOC );
   apiret = LoadCaptionFile ( pid, szFilename );
   if ( apiret )
   {                                       /*---------------------------------*/
      if ( pid->pvText )                   /*                                 */
      {                                    /*  If we were unable to load the  */
         DosFreeMem ( pid->pvText );       /*  caption file, then clean up    */
         pid->pvText = (PVOID) NULL;       /*  any resources we allocated, and*/
      }                                    /*  return an error code.          */
      DosFreeMem ( pid->pvLinedataTable ); /*                                 */
      pid->pvLinedataTable = (PVOID) NULL; /*---------------------------------*/
      return apiret;
   }


/*---------------- Format the first few lines of text ------------------------*/

   FormatTextBitmap ( pid, 0 ); /* Write the first line to text to the bitmap.*/


/*---------------- Set instance data to initial values -----------------------*/

   pid->lScrollOffset = 0;
   pid->usDisplayLine = 0;/* Index of line currently at bottom of the cap. win*/
   pid->usNextlineReq = 0;
   pid->ulTimePrev    = 0;
   pid->hwndPosition  = hwndOwner;


/*--- Ask MMPM/2 for position advise messages on the device ------------------*/

   strcat( szCommandString, szDeviceName);
   strcat( szCommandString, " ");
   strcat( szCommandString, "on every 1500 notify");

   lSendStringRC =
        mciSendString( (PSZ)szCommandString,
                       (PSZ)NULL,
                       CCHMAXPATH,
                       pid->hwndCaption,
                       0 );

   if (lSendStringRC != 0)
      return (APIRET) lSendStringRC;

   PositionCaptionWindow ( pid );
   pid->ulFlags |= CC_CAPTIONS_ACTIVE;
   WinSetWindowPos ( pid->hwndCaption, 0, 0, 0, 0, 0, SWP_SHOW );

   return (APIRET) 0;
}

/******************************************************************************
 * Name         :  PositionCaptionWindow
 *
 * Description  :  This procedure will position the caption window in its
 *                 parent window and display it.
 *
 * Parameters   :  pid          Pointer to Instance Data for caption window
 *
 * Return       :  none
 *
 ******************************************************************************/
static void PositionCaptionWindow ( PCCINSTANCE pid )
{
   RECTL rclParent;  /* Receives dimensions of the parent of the caption win  */
   LONG  lXcoord;    /* Computed position of caption window - X coordinate    */
   LONG  lYcoord;    /* Computed position of caption window - Y coordinate    */


/*------------------------ Locic summary -------------------------------------*/
/*                                                                            */
/*  (1) Calculate horizontal position.  If the user has specified a position, */
/*      then use that position.  If not, then center the Captioning Window    */
/*      horizontally in its parent window.                                    */
/*                                                                            */
/*  (2) Calculate the vertical position. If the user has specified a position,*/
/*      then use that position.  If not, then place the captioning window near*/
/*      the bottom of the parent window.                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   WinQueryWindowRect ( pid->hwndParent, &rclParent );

   if ( pid->ulFlags & CC_USER_POSITION )          /* (1) Horizontal position */
      lXcoord = pid->lWindowXcoord;
   else
   {
      lXcoord = ( rclParent.xRight - pid->rclText.xRight ) / 2;
      pid->lWindowXcoord = lXcoord;
   }

   if ( pid->ulFlags & CC_USER_POSITION )          /* (2) Vertical position   */
      lYcoord = pid->lWindowYcoord;
   else
   {
      lYcoord = rclParent.yTop / 12;
      pid->lWindowYcoord = lYcoord;
   }

   pid->rclWin = pid->rclText;

   WinSetWindowPos( pid->hwndCaption, 0, lXcoord, lYcoord, pid->rclText.xRight,
                            pid->rclText.yTop, SWP_MOVE | SWP_SIZE );
}

/******************************************************************************
 * Name         :  CreateBitmap
 *
 * Description  :  This procedure will create a backup text bitmap for the
 *                 captioning window.  Data from the bitmap will be blitted
 *                 to the caption window whenever it receives a WM_PAINT
 *                 message.
 *
 * Parameters   :  pid  -  Pointer to Instance Data for caption window
 *
 * Return       :  none
 *
 ******************************************************************************/
static APIRET CreateBitmap ( PCCINSTANCE pid )
{
   FONTMETRICS        fmText;          /* Font metrics for backup bitmap font */
   SIZEL              sizelText;       /* Needed to initialize the bitmap     */
   BITMAPINFOHEADER2  bmh2;            /* Needed to initialize the bitmap     */
   LONG               lModulo;
   APIRET             apiret;


   /*----------------------- Logic overview ----------------------------------*/
   /*                                                                         */
   /*  (1) If a backup bitmap already exists for this window, destroy it      */
   /*      before creating the new bitmap.  (We can't just use the old bitmap */
   /*      because the user might have decided to resize the window.)         */
   /*                                                                         */
   /*  (2) Open a new device context and create a new presentation space for  */
   /*      the backup bitmap.                                                 */
   /*                                                                         */
   /*  (3) Get the font metrics for the default font for this presentation    */
   /*      space. Use this data in conjunction with the window instance data  */
   /*      (usWindowLines, usWindowChars) to determine the dimensions of the  */
   /*      bitmap.  The bitmap will be taller than the window by the height   */
   /*      of one text line.  This is so the caption window can be repainted  */
   /*      correctly while it is scrolling.                                   */
   /*                                                                         */
   /*  (4) Create the backup bitamp.  This is a monochrome bitmap; the colors */
   /*      of the text and background are determined during the WM_PAINT      */
   /*      message for the caption window.                                    */
   /*                                                                         */
   /*-------------------------------------------------------------------------*/

   if ( pid->hpsText )
   {
      GpiDestroyPS   ( pid->hpsText );/*--------------------------------------*/
      DevCloseDC     ( pid->hdcText );/* (1)Destroy any existing backup bitmap*/
      GpiDeleteBitmap( pid->hbmText );/*--------------------------------------*/
   }


   /*--------- (2) Open device context and create presentation space ---------*/

   pid->hdcText           = DevOpenDC ( pid->hab, OD_MEMORY, "*", 0, NULL, 0 );

   sizelText.cx           = 0;
   sizelText.cy           = 0;
   pid->hpsText           = GpiCreatePS( pid->hab, pid->hdcText, &sizelText,
                              PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC);


   /*--------- (3) Use the font metrics to determine size of bitmap ----------*/

   GpiQueryFontMetrics ( pid->hpsText, (LONG) sizeof(fmText), &fmText );

   pid->lLineSpacing  = fmText.lMaxBaselineExt       /*-----------------------*/
                      + fmText.lExternalLeading      /* Force the line spacing*/
                      + pid->lScrollInc;             /* to be a multiple of   */
   lModulo = pid->lLineSpacing % pid->lScrollInc;    /* the scroll increment. */
   if (lModulo)                                      /*-----------------------*/
      pid->lLineSpacing += pid->lScrollInc - lModulo;

   pid->rclText.yTop  = pid->lLineSpacing          /* Height of caption window*/
                      * (LONG) pid->usWindowLines  /* ( The bitmap is one text*/
                      + pid->lScrollInc;           /* line greater in height.)*/

   pid->rclText.xRight =  fmText.lAveCharWidth * pid->usWindowChars;
   pid->lBottomLineY  = pid->rclText.yTop -
                        ( pid->lLineSpacing * pid->usWindowLines ) +
                        fmText.lMaxDescender;


   /*--------------------- (4) Create the bitmap -----------------------------*/

   bmh2.cbFix           = sizeof (BITMAPINFOHEADER2);
   bmh2.cx              = pid->rclText.xRight;
   bmh2.cy              = pid->rclText.yTop+pid->lLineSpacing;/* Bitmap height*/
   bmh2.cPlanes         = 1;
   bmh2.cBitCount       = 1;                                    /* Monochrome */
   bmh2.ulCompression   = BCA_UNCOMP;
   bmh2.cbImage         = 0;
   bmh2.cxResolution    = 30000;
   bmh2.cyResolution    = 30000;
   bmh2.cclrUsed        = 2;
   bmh2.cclrImportant   = 2;
   bmh2.usUnits         = BRU_METRIC;
   bmh2.usReserved      = 0;
   bmh2.usRecording     = BRA_BOTTOMUP;
   bmh2.usRendering     = BRH_NOTHALFTONED;
   bmh2.cSize1          = 0;
   bmh2.cSize2          = 0;
   bmh2.ulColorEncoding = BCE_RGB;
   bmh2.ulIdentifier    = 0;
   pid->hbmText         = GpiCreateBitmap(pid->hpsText, &bmh2, 0L, 0L, NULL);

   if (pid->hbmText == 0)
      return (APIRET) CCERR_CANNOT_CREATE_BITMAP;

   GpiSetBitmap           ( pid->hpsText, pid->hbmText );
   GpiErase               ( pid->hpsText );

   return(APIRET) 0;
}

/******************************************************************************
 * Name         :  FormatTextBitmap
 *
 * Description  :  This procedure will write text to the backup bitmap beginning
 *                 with the bottom line and working up.
 *
 * Parameters   :  pid        -  Pointer to Instance Data for caption window
 *                 usLastLine - Index of bottom-most text line in bitmap.
 *
 * Return       :  none
 *
 ******************************************************************************/
static VOID FormatTextBitmap ( PCCINSTANCE pid, USHORT usLastLine )
{
   PLINEDATA pld;        /* Pointer to LINEDATA structure for current line    */
   POINTL    ptlText;    /* Coordinate of text                                */
   USHORT    usCurrLine; /* Index of line currently being written to bitmap   */
   USHORT    usLineCount;/* Counter for number of lines written to text bitmap*/


   GpiErase ( pid->hpsText );
   pld        = (PLINEDATA) pid->pvLinedataTable + usLastLine;
   ptlText.x  = pid->lLeftMargin;
   ptlText.y  = pid->lBottomLineY;
   usCurrLine = usLastLine;

   for ( usLineCount = 0; usLineCount <= pid->usWindowLines; usLineCount++ )
   {
      GpiCharStringAt(pid->hpsText, &ptlText, pld->lTextLen, (PCH)pld->szText);
      if ( usCurrLine == 0 ) break;
      ptlText.y += pid->lLineSpacing;
      pld--;
      usCurrLine--;
   }
}

/******************************************************************************
 * Name         :  LoadCaptionFile
 *
 * Description  :  This procedure will load a caption file for processing.
 *
 * Parameters   :  pid           -  Pointer to Instance Data for caption window
 *                 szCaptionFile -  NULL-terminated caption file name.
 *
 * Return       :  0      -  if the operation was initiated without error.
 *                 apiret -  if an error occurred.
 *
 ******************************************************************************/
static APIRET LoadCaptionFile( PCCINSTANCE pid, CHAR szCaptionFile[] )
{
   APIRET    apiret;          /* DOS return code                              */
   HFILE     hfile;           /* Text file handle                             */
   ULONG     ulAction;        /* Parameter returned by DOS API functions      */
   ULONG     ulFileSize;      /* Expected file size, used as a read check     */
   ULONG     ulBytesRemaining;/* Bytes in file remaining to be scanned        */
   PLINEDATA pld;             /* Points to "current" line in Line Data Table  */
   UCHAR     ucEndChar;       /* Used to save char immed. following MM time   */
   PSZ       psz;             /* Pointer to "current line" in caption file    */

   PVOID     pvPositions;     /* Points to table used by GpiQueryCharPosAt    */
   PPOINTL   pptl;            /* (Same as above, but cast to a  useful type)  */
   POINTL    ptlStart;        /* Set to co-or of leftmost char in caption win */
   PLINEDATA pldNew;          /* Points to "remainder" of line after word-wrap*/
   SHORT     sIndex;        /* Used to scan line for a logical breaking point.*/
   PSZ       pszEnd;        /* Points to first memory position after text data*/

/*----------------------------------------------------------------------------*/

   if ( pid->pvText )              /*-----------------------------------------*/
   {                               /*                                         */
      DosFreeMem ( pid->pvText );  /* Free any text that is currently loaded. */
      pid->pvText = NULL;          /*                                         */
   }                               /*-----------------------------------------*/


/*----------------------- Open the text file ---------------------------------*/

   apiret = DosOpen ( (PSZ) szCaptionFile, &hfile, &ulAction, 0,
                      FILE_NORMAL | FILE_READONLY, FILE_OPEN,
                      OPEN_ACCESS_READONLY | OPEN_SHARE_DENYREADWRITE, NULL );

   if ( apiret ) return apiret;


/*------------ Read the entire file into a memory area -----------------------*/

   DosSetFilePtr ( hfile, 0, FILE_END, &ulFileSize ); /* Determine file size. */
   apiret =
      DosAllocMem( &pid->pvText, ulFileSize, fALLOC );/* Alloc memory for it. */

   if ( apiret)
   {
      DosClose ( hfile );
      return apiret;
   }

   DosSetFilePtr ( hfile, 0, FILE_BEGIN, &ulAction );/* Read file image into */
   apiret = DosRead ( hfile, pid->pvText, ulFileSize, &ulAction );/* buffer. */
   DosClose ( hfile );
   if ( ! apiret && ( ulFileSize != ulAction ) ) apiret = CCERR_UNEXPECTED_EOF;
   pszEnd = (PSZ) pid->pvText + ulFileSize;

   if ( apiret ) return apiret;


/*----------------------------------------------------------------------------*/
/*  Make sure the file is in the correct format, by checking the first line   */
/*  for the constant "CC_FILE".                                               */
/*----------------------------------------------------------------------------*/

   if ( strncmp( (char *) pid->pvText, "CC_FILE", 7 ) )/* Check for format ID */
      return(APIRET) CCERR_FILE_FORMAT;                /* Return error if not */
                                                       /* found.              */
   psz = (PSZ) memchr ( pid->pvText, CHAR_RETURN, (size_t) ulFileSize );
   if ( psz == NULL ) return(APIRET) CCERR_FILE_FORMAT;/* Bump pointer psz    */
   psz += 2;                                           /* to the start of the */
   if ( ( *psz == CHAR_EOF ) || ( psz >= pszEnd ) )    /* first line of data. */
      return (APIRET) CCERR_FILE_FORMAT;


/*----------------------------------------------------------------------------*/
/*  Allocate memory to hold a character-position table. This table will be    */
/*  used to determine if and when it will be necessary to split a line to fit */
/*  into the captioning window.                                               */
/*----------------------------------------------------------------------------*/

   DosAllocMem ( &pvPositions, 256 * sizeof(POINTL), fALLOC );
   pptl       = (PPOINTL) pvPositions;
   ptlStart.x = pid->lLeftMargin;                      /* Get memory for char */
   ptlStart.y = pid->lBottomLineY;                     /* position table.     */


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  Now loop through every line in the caption file.  For each line build an  */
/*  entry in the Line Data Table.  There will be AT LEAST as many entries in  */
/*  the Line Data Table as there are lines in the caption file.               */
/*                                                                            */
/*  (1) The first eight characters of the line is an ASCII number representing*/
/*      the time ( in MM units ) when the caption is to be scrolled into the  */
/*      window. The text (if any) begins in column ten.                       */
/*                                                                            */
/*  (2) The caption text runs from column ten to the next carriage return.    */
/*      Determine its length and place that in pld->lTextLen.                 */
/*                                                                            */
/*  (3) Wrap text lines that are wider than the caption window.  Create extra */
/*      entries in the Line Data Table for the extra lines that are needed.   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

   pld = (PLINEDATA) pid->pvLinedataTable;
   for ( pid->usLineCount = 1; pid->usLineCount < MAX_TEXT_LINES;
                    pid->usLineCount++ )
   {
      ucEndChar     = psz[8];                   /*----------------------------*/
      psz[8]        = CHAR_NULL;                /* (1) Convert the time to a  */
      pld->ulTime   = (ULONG) atol((PCHAR) psz);/*  long integer and setup a  */
      pld->szText   = psz + 9;                  /*  default text len of zero  */
      pld->lTextLen = 0;                        /*----------------------------*/

      if ( ucEndChar != CHAR_RETURN )        /* (2) If text present, find its */
      {                                      /*     length.                   */
         ulBytesRemaining = (ULONG) ( (ULONG) pszEnd - (ULONG) psz );
         psz =
           (PSZ) memchr( pld->szText, CHAR_RETURN, (size_t) ulBytesRemaining );
         if ( psz == NULL ) break;
         *psz = CHAR_NULL;
         pld->lTextLen = (LONG) strlen ( pld->szText );
         psz += 2;

         /*-------------------------------------------------------------------*/
         /*                                                                   */
         /* (3) Wrap text lines that are too wide to fit into the caption     */
         /*     window. The overall strategy of this logic is as follows:  If */
         /*     the line istoo wide, divide it into two parts--a part that    */
         /*     will fit and a part which contains everything else; then      */
         /*     apply this same strategy to the second part of the line. Keep */
         /*     repeating this logic until we are left a line which will fit  */
         /*     without any left-over parts.                                  */
         /*                                                                   */
         /*     In detail:                                                    */
         /*                                                                   */
         /*        (3a) Determine the width of the current line. If it fits   */
         /*             in the caption window, we are done!  Otherwise...     */
         /*                                                                   */
         /*        (3b) We will have to divide the line.  Make a new entry in */
         /*             the Line Data Table.  We will use this for the second */
         /*             part of the line.                                     */
         /*                                                                   */
         /*        (3c) Loop backwards through the Character Position Table   */
         /*             until we find the last space character that will fit  */
         /*             in the window. (If there are no blanks in the first   */
         /*             portion, just chop the line off at the window width   */
         /*             for a "Procrustean" fit.)                             */
         /*                                                                   */
         /*        (3d) Make the second portion of the original line the new  */
         /*             "current" line and repeat the operation.              */
         /*                                                                   */
         /*-------------------------------------------------------------------*/

         for ( ; ; )
         {                                           /* (3a) Does it fit yet?*/
            GpiQueryCharStringPosAt( pid->hpsText, &ptlStart, 0, pld->lTextLen,
                                               (PCH) pld->szText, NULL, pptl );
            if ( pptl[pld->lTextLen].x <= pid->rclText.xRight )
               break;

            pid->usLineCount++;                   /*--------------------------*/
            if (pid->usLineCount>=MAX_TEXT_LINES) /* (3b) Make sure there is  */
            {                                     /* enough room in the Line  */
               DosFreeMem ( pvPositions );        /* Data Table for new line. */
               return(APIRET)CCERR_TOO_MANY_LINES;/* If so, use the existing  */
            }                                     /* Line Data entry as the   */
            pldNew  = pld + 1;                    /* basis for the new one.   */
            *pldNew = *pld;                       /*--------------------------*/

            for ( sIndex = pld->lTextLen; sIndex > 0; sIndex-- )
               if ( pptl[sIndex].x <= pid->rclText.xRight ) break;
            pld->lTextLen = (LONG) sIndex;
            sIndex--;                             /*--------------------------*/
            while ( sIndex > 0 )                  /* (3c) Truncate existing   */
            {                                     /* line at the last space   */
               if ( pld->szText[sIndex] == ' ' )  /* that will fit.           */
                  break;                          /*--------------------------*/
               sIndex--;
            }
            if ( sIndex ) pld->lTextLen = (LONG) sIndex + 1;

            /*-----------------------------------------------*/
            /* (3d) Begin new line where old line ends.      */
            /*-----------------------------------------------*/
            pldNew->lTextLen -= pld->lTextLen;
            pldNew->szText = pld->szText + pld->lTextLen;
            pld = pldNew;
          }
      }
      else
         psz += 10;       /* (Take this branch of the if in case there */
                              /*   is no caption text) */

/*------------ Break out if the loop when we reach end of file ---------------*/

      if ( ( *psz == CHAR_EOF ) || ( psz >= pszEnd ) )
         break;

      pld++;
   }

   DosFreeMem( pvPositions );/* Free memory used for the Char. Position Table */


/*---------------------------- Make some final checks ------------------------*/
/*                                                                            */
/*   (1) Make sure the file ends in a standard EOF character.                 */
/*   (2) See if we had enough room in the Line Data Table to hold the file.   */
/*----------------------------------------------------------------------------*/

   if ( psz == NULL )
      return (APIRET) CCERR_FILE_FORMAT;

   if ( pid->usLineCount == MAX_TEXT_LINES )
      return (APIRET) CCERR_TOO_MANY_LINES;

   return 0;
}

/******************************************************************************
 * Name         :  StopCaptioning
 *
 * Description  :  This procedure will stop the current captioning operation.
 *
 * Parameters   :  pid   -  Pointer to Instance Data for caption window
 *
 * Return       :  none
 *
 ******************************************************************************/
static VOID StopCaptioning ( PCCINSTANCE pid )
{
   pid->ulFlags &= ~CC_CAPTIONS_ACTIVE;
   WinSetWindowPos ( pid->hwndCaption, 0, 0, 0, 0, 0, SWP_HIDE );
   if ( pid->ulFlags & CC_TIMER_RUNNING )
   {
      WinStopTimer ( pid->hab, pid->hwndCaption, CC_TIMER_ID );
      pid->ulFlags &= ~CC_TIMER_RUNNING;
   }
   if ( pid->hpsText )
   {
      GpiDestroyPS    ( pid->hpsText ); /*------------------------------------*/
      DevCloseDC      ( pid->hdcText ); /* Destroy any existing backup bitmap */
      GpiDeleteBitmap ( pid->hbmText ); /*------------------------------------*/
      pid->hpsText = (HPS) 0;
   }
   if ( pid->pvText )
   {
      DosFreeMem ( pid->pvText );
      pid->pvText = (PVOID) NULL;
   }
   if ( pid->pvLinedataTable )
   {
      DosFreeMem ( pid->pvLinedataTable );
      pid->pvLinedataTable = (PVOID) NULL;
   }
}

/**************************************************************************
 * OS/2 Sample Print Application PRTSAMP
 *
 * Name: prtobj.c
 *
 * Description:   The object window procedure on thread 2.
 *
 *             Tasks asked of the object window are not bound by 1/10
 *             second rule.  Tasks are given to the object window to
 *             perform via WM_USER_* messages.
 *
 *             When tasks are completed, the object window posts a
 *             WM_USER_ACK message to the window that requested the task.
 *
 *             This source file contains the following functions:
 *
 *             threadmain(pv)
 *             ObjectWinProc(hwnd, msg, mp1, mp2)
 *             CalculateDrawingArea(pmp, flag, pptlBegin, pptlEnd)
 *             DrawBorder(hps, pmp, ptlBegin, ptlEnd)
 *             FixedPointsToTwips(fxPointSize)
 *             FixedInchesToTwips(fxInch)
 *             LoadBitmap( pmp, flag )
 *             MMToTwips(lMM)
 *             PaintBitmap(pmp, flag)
 *             PaintDefaultScreen(pmp, flag)
 *             PaintMetaFile(pmp, flag)
 *             PaintText(pmp, flag)
 *             SetClipPath(hps, pmp, ptlBegin, ptlEnd)
 *             trim(s)
 *
 * Concepts:   second thread, object window
 *
 * API's:       WinInitialize
 *              WinCreateMsgQueu
 *              WinRegisterClass
 *              WinCreateWindow
 *              WinPostMsg
 *              WinGetMsg
 *              WinDispatchMsg
 *              WinDestroyWindow
 *              WinDestroyMsgQueue
 *              WinTerminate
 *              WinQueryAnchorBlock
 *              WinQueryWindowULong
 *              WinSetWindowULong
 *              DosOpen
 *              DosQueryFileInfo
 *              DosRead
 *              DosClose
 *              DevOpenDC
 *              GpiCreatePS
 *              GpiCreateBitmap
 *              GpiSetBitmap
 *              GpiWCBitBlt
 *              GpiQueryDefaultViewMatrix
 *              GpiSetDrawControl
 *              GpiResetBoundaryData
 *              GpiPlayMetaFile
 *              GpiScale
 *              GpiTranslate
 *              GpiSetViewingTransformMatrix
 *              GpiResetPS
 *              GpiSetBitmapBits
 *              WinInvalidateRect
 *              GpiLoadMetaFile
 *              WinSendMsg
 *              GpiSetDrawingMode
 *              WinGetLastError
 *              DosSleep
 *              GpiErase
 *              GpiDeleteSegment
 *              GpiOpenSegment
 *              GpiSetCurrentPosition
 *              GpiBox
 *              GpiSetColor
 *              GpiCharString
 *              GpiCloseSegment
 *              WinQueryWindowRect
 *              GpiQueryBitmapParameters
 *              GpiBitBlt
 *              GpiQueryPS
 *              GpiSetPS
 *              WinDefWindowProc
 *              GpiSetCharSet
 *              GpiDeleteSetId
 *              GpiCreateLogFont
 *              GpiQueryFontMetrics
 *              GpiSetCharBox
 *
 *   Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PMASSERT.H
 *
 *  Copyright (C) 1991-1993 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *
 ****************************************************************************/

/* os2 includes */
#define INCL_DEV
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPICORRELATION
#define INCL_GPIERRORS
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPIPATHS
#define INCL_GPIPRIMITIVES
#define INCL_GPISEGMENTS
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINMLE
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
#include <os2.h>

/* c language includes */
#include <ctype.h>
#include <memory.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\types.h>

/* application includes */
#include "pmassert.h"
#include "prtsamp.h"
#include "prtsdlg.h"
#include "prtshlp.h"

/* local function prototypes */

VOID CalculateDrawingArea( PMAIN_PARM pmp, ULONG flag,
                           PPOINTL pptlBegin, PPOINTL pptlEnd );
VOID DrawBorder( HPS hps, PMAIN_PARM pmp, POINTL ptlBegin, POINTL ptlEnd );
ULONG LoadBitmap( PMAIN_PARM pmp, ULONG flag );
LONG MMToTwips( LONG lMM );
VOID PaintDefaultScreen( PMAIN_PARM pmp, ULONG flag );
ULONG PaintText( PMAIN_PARM pmp, ULONG flag );
VOID SetClipPath( HPS hps, PMAIN_PARM pmp, POINTL ptlBegin, POINTL ptlEnd );


/*************************************************************************
 *
 * Name: threadmain
 *
 * Description: Similar in nature to main(), except this occurs on thread 2.
 *              Called by _beginthread() in prtcreat.c
 *
 * API's:       WinInitialize
 *              WinCreateMsgQueu
 *              WinRegisterClass
 *              WinCreateWindow
 *              WinPostMsg
 *              WinGetMsg
 *              WinDispatchMsg
 *              WinDestroyWindow
 *              WinDestroyMsgQueue
 *              WinTerminate
 *
 * Parameters: pv, a pointer to the main block of program parameters
 *
 * Return:  [none]
 *
 **************************************************************************/
void _Optlink threadmain( void *pv )
{

   PMAIN_PARM pmp;
   BOOL       bOK;
   HAB        hab;
   HMQ        hmq;
   QMSG       qmsg;

   /* copy and convert pvoid parmeter to a pointer to the main parameter block */
   pmp = (PMAIN_PARM) pv;

   /* thread initialization */
   hab = WinInitialize( 0 );
   hmq = WinCreateMsgQueue( hab, 0 );

   /*
    * ensure that object window does not receive a WM_QUIT on system shutdown
    */
   WinCancelShutdown(hmq, TRUE);

   bOK = WinRegisterClass( hab,
                                OBJECTCLASSNAME,
                                ObjectWinProc,
                                0L,
                                sizeof( PMAIN_PARM ) );
   pmassert( hab, bOK );

   /*
    * create a worker window where the parent is the PM object window,
    * it operates on thread 2,
    * has no visible windows on the desktop,
    * and is not bound by the 1/10 second message processing rule.
    */
   pmp->hwndObject = WinCreateWindow(
                       HWND_OBJECT,       /* parent */
                       OBJECTCLASSNAME,   /* class name */
                       "",                /* no caption needed */
                       0L,                /* style */
                       0L,                /* x */
                       0L,                /* y */
                       0L,                /* cx */
                       0L,                /* cy */
                       HWND_OBJECT,       /* owner */
                       HWND_BOTTOM,       /* position (nop) */
                       0L,                /* id (nop) */
                       (PVOID)pmp,        /* parms passed to wm_create case */
                       (PVOID)NULL);      /* presparams */

   pmassert( hab, pmp->hwndObject );

   /*
    * at this point, application has completely initialized
    * wm_create processing in the object window procedure has completed.
    * let client window know about it
    */
   bOK = WinPostMsg( pmp->hwndClient, WM_USER_ACK, (MPARAM)0L, (MPARAM)0L );
   pmassert( pmp->hab, bOK );


    /* The create processing put values in for next mode and next filename.
       Post this message to activate those new settings */
    bOK = WinPostMsg( pmp->hwndClient, WM_USER_NEW_MODE, 0, 0 );
    pmassert( pmp->hab, bOK );


   /*
    * dispatch messages; these messages will be mostly user-defined messages
    * processed on thread 2
    */
   while( WinGetMsg ( hab, &qmsg, (HWND)NULLHANDLE, 0L, 0L ))
   {
      WinDispatchMsg ( hab, &qmsg );
   }


   /* Use the new-mode processing to close current file before exiting */
   pmp->ulNextMode = MODE_UNKNOWN;
   WinPostMsg( pmp->hwndClient, WM_USER_NEW_MODE, 0, 0 );


   /* make thread one terminate */
   WinPostMsg( pmp->hwndClient, WM_QUIT, (MPARAM)0L, (MPARAM)0L );

   /* clean up */
   WinDestroyWindow( pmp->hwndObject );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

   /* end this thread */
   _endthread();

}  /*  end of threadmain()  */


/**************************************************************************
 *
 * Name: ObjectWinProc
 *
 * Description: a window procedure like most others, except it is not
 *              responsible for any visible windows or presentation. It
 *              exists to perform lengthy tasks on thread2 of the
 *              application.  WM_* messages appear here in alphabetical order.
 *
 * API's:       GpiLoadMetaFile
 *              WinDefWindowProc
 *              WinInvalidateRect
 *              WinPostMsg
 *              WinQueryWindowULong
 *              WinSendMsg
 *              WinSetWindowULong
 *              WinQueryAnchorBlock
 *
 * Parameters: mp1 = window handle to acknowledge upon completion of the task
 *             (hwndToAck)
 *             mp2 is an extra parameter and depends on the task
 *
 * returns: an acknowlegement of completion of the task using WinPostMsg
 *   if success: WinPostMsg( hwndToAck, WM_USER_ACK,  msg, rc );
 *               return rc;
 *
 *   if not:     WinPostMsg( hwndToAck, WM_NACK_*   , msg, rc );
 *               return rc;
 *
 *   where msg was the WM_USER_* message that was posted to the object window
 *   and rc is a return code. Depending on the hwndToAck, returning
 *   the result code can be as important as posting it; in particular,
 *   the object window may send synchronous messages to itself and may
 *   check the result code via the return code of WinSendMsg()
 *
 ***************************************************************************/
MRESULT EXPENTRY ObjectWinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   HAB                hab;
   HWND               hwndToAck;
   LONG               lRC;
   PMAIN_PARM         pmp;
   ULONG              nackmsg;


   /* store the handle of the window to ack upon task completion; */
   hwndToAck = (HWND)mp1;
   hab = WinQueryAnchorBlock( hwnd );
   pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );


   switch( msg )
   {

   case WM_CREATE:
     /* mp1 is pointer to main paramters; save it in object window words */
     pmp = (PMAIN_PARM)mp1;
     WinSetWindowULong( hwnd, QWL_USER, (ULONG) pmp );

     /* do more startup processing whilst on thread 2 */

     /* Read .ini for filename, mode, printer, and driver data */
     GetProfile( pmp );

     /* if filename supplied, use it instead */
     if (pmp->szArgFilename && *pmp->szArgFilename)
     {
        ValidateFilename( pmp, pmp->szArgFilename, (HWND)NULLHANDLE);
     }
     /*
      * Try to set the print destination to that which was stored in INI.
      * Parm 2 == FALSE means do not display a dialog
      */
     QueryPrintQueue( pmp, FALSE );

     /*
      * Try to set the form to that which was stored in INI.
      * Parm 2 == FALSE means do not display a dialog
      */
     QueryForm( pmp, FALSE );
     return (MRESULT)NULL;


   case WM_USER_CLOSE:
     /* write settings to ini */
     SaveProfile(pmp);

     /*
      * post a quit to object window;
      * when msg loop falls out in threadmain, then post wm_quit to client
      */
     WinPostMsg( hwnd, WM_QUIT, 0, 0 );
     break;


   case WM_USER_LOAD_BITMAP:
     /*
      * THIS MESSAGE HAS A PARAMETER:
      * mp2 is a flag that indicates whether the memory ps for the bitmap
      * is to be compatible with the screen or the current printer setup
      */


     /* load new bitmap */
     nackmsg = LoadBitmap( pmp, (ULONG)mp2 );
     if (! nackmsg )
     {
        WinPostMsg( hwndToAck, nackmsg, (MPARAM)msg, (MPARAM)0L );
     }

     /* invalidate client area to force a paint */
     WinInvalidateRect( pmp->hwndClient, NULL, FALSE );

     /* done loading bitmap */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, (MPARAM)0L );
     return (MRESULT)TRUE;


   case WM_USER_LOAD_METAFILE:
     /* load new metafile */
     pmp->hmf = GpiLoadMetaFile( hab, pmp->szFilename );
     if( GPI_ERROR  == pmp->hmf )
     {
       /* load error */
       pmp->hmf  = (HMF)NULLHANDLE;
       WinPostMsg( hwndToAck, WM_NACK_FILE_READING_ERROR,
                       (MPARAM)msg, (MPARAM)0L );
       return (MRESULT)NULL;
     }

     /* invalidate client area to force a paint */
     WinInvalidateRect( pmp->hwndClient, NULL, FALSE );

     /* done loading metafile */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, (MPARAM)0L );
     return (MRESULT)NULL;


   case WM_USER_LOAD_TEXT:
     pmp->f = fopen( pmp->szFilename, "r" );
     if( !pmp->f )
     {
       WinPostMsg( hwndToAck, WM_NACK_FILE_READING_ERROR,
                                 (MPARAM)msg, (MPARAM)0L );
       return (MRESULT)NULL;
     }

     /* has user visited the font dialog? */
     if( pmp->fontdlg.fAttrs.usRecordLength == (USHORT)0 )
     {
       /* no font selected yet; chain to font selection dialog now
        * after user selects a font (prtmenu.c), the program invokes
        * the pagination routine                                     */
       WinPostMsg( pmp->hwndClient, WM_COMMAND, (MPARAM)IDM_SETFONT, (MPARAM)0L);
     }
     else
     {
       /* a font is already selected; invoke the pagination routine  */
       WinSendMsg( hwnd, WM_USER_PAGINATE, (MPARAM)hwnd,
                   (MPARAM)FLAGS_SCREEN );
     }

     /* done loading text */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, (MPARAM)0L );
     return (MRESULT)NULL;


   case WM_USER_PAGINATE:
     /* draw text into presentation space */
     lRC = PaintText( pmp, (ULONG)mp2 );
     /* ack and return with result code */
     WinPostMsg( hwndToAck, WM_USER_ACK,(MPARAM)msg,(MPARAM)lRC );
     return (MRESULT) lRC;


   case WM_USER_PAINT_DEFAULT_SCREEN:
     /* draw default screen into presentation space */
     PaintDefaultScreen( pmp, FLAGS_SCREEN );
     /* ack and return with result code */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, 0 );
     return (MRESULT)NULL;

   case WM_USER_PAINT_BITMAP:
     /* draw bitmap into presentation space */
     PaintBitmap( pmp, FLAGS_SCREEN );
     /* ack and return with result code */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, 0 );
     return (MRESULT)NULL;


   case WM_USER_PAINT_METAFILE:
     /* Play metafile into client area */
     PaintMetaFile( pmp, FLAGS_SCREEN );
     /* ack and return with result code */
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, (MPARAM)0L );
     return (MRESULT)NULL;


   case WM_USER_PRINT:
     ProcessUserPrint(hwnd, pmp);
     WinPostMsg( hwndToAck, WM_USER_ACK, (MPARAM)msg, (MPARAM)0L );
     return (MRESULT)NULL;
   }

   /* default: */
   return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}  /* end of ObjectWinProc() */


/*************************************************************************
 *
 * name: CalculateDrawingArea
 *
 * Description:  Calculate the drawing area of the screen or printer
 *
 * API's:  [none]
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *             PPOINTL     pointer to begin point of drawing area
 *             PPOINTL     pointer to end point of drawing area
 *
 * Return: [none]
 *
 **************************************************************************/
VOID CalculateDrawingArea( PMAIN_PARM pmp, ULONG flag,
                           PPOINTL pptlBegin, PPOINTL pptlEnd )
{
     /* define drawing area on screen or printer */
     pptlBegin->x = FixedInchesToTwips(pmp->form.fxLeftMargin);
     pptlBegin->y = FixedInchesToTwips(pmp->form.fxBottomMargin);
     pptlEnd->x = pptlBegin->x + pmp->sizelPage.cx;
     pptlEnd->y = pptlBegin->y + pmp->sizelPage.cy;
     if (flag != FLAGS_SCREEN)
     {
        /* take hardware clip limits into account */
        pptlBegin->x -= MMToTwips( pmp->hcinfo[pmp->iForm].xLeftClip );
        pptlBegin->x = max( pptlBegin->x, 0 );
        pptlBegin->y -= MMToTwips( pmp->hcinfo[pmp->iForm].yBottomClip );
        pptlBegin->y = max( pptlBegin->y, 0 );
        pptlEnd->x -= MMToTwips( pmp->hcinfo[pmp->iForm].xLeftClip );
        pptlEnd->y -= MMToTwips( pmp->hcinfo[pmp->iForm].yBottomClip );
     }
} /* end of CalculateDrawingArea */


/*************************************************************************
 *
 * name: DrawBorder
 *
 * Description:  Clears drawing area and draws form border
 *
 * API's:  [none]
 *
 * Parameters:  HPS         presentation space to use
 *              PMAIN_PARM  pointer to the global data structure
 *              POINTL      pointer to begin point of drawing area
 *              POINTL      pointer to end point of drawing area
 *
 * Return: [none]
 *
 **************************************************************************/
VOID DrawBorder( HPS hps, PMAIN_PARM pmp, POINTL ptlBegin, POINTL ptlEnd )
{
   BOOL     bOK;
   ULONG    lRC;
   HAB      hab;
   POINTL   pointl;

   hab = pmp->hab;

   /* white out area of presentation space */
   bOK = GpiSetColor( hps, CLR_WHITE );
   pmassert( hab, bOK );
   bOK = GpiSetPattern( hps, PATSYM_SOLID );
   pmassert( hab, bOK );
   pointl.x = -65535;
   pointl.y = -65535;
   bOK = GpiSetCurrentPosition( hps, &pointl );
   pmassert( hab, bOK );
   pointl.x = 65535;
   pointl.y = 65535;
   lRC = GpiBox( hps, DRO_FILL, &pointl, 0L, 0L );
   pmassert( hab, lRC != GPI_ERROR );

   /* draw margins of form as a red border */
   bOK = GpiBeginPath( hps, 1);
   pmassert( hab, bOK );
   bOK = GpiSetColor( hps, CLR_RED);
   pmassert( hab, bOK );
   bOK = GpiSetPattern( hps, PATSYM_DENSE4 );
   pmassert( hab, bOK );
   pointl.x = 0;
   pointl.y = 0;
   bOK = GpiSetCurrentPosition( hps, &pointl );
   pmassert( hab, bOK );
   pointl.x = FixedInchesToTwips(pmp->form.fxxWidth);
   pointl.y = FixedInchesToTwips(pmp->form.fxyHeight);
   lRC = GpiBox( hps, DRO_OUTLINE, &pointl, 0L, 0L );
   pmassert( hab, lRC != GPI_ERROR );
   bOK = GpiSetCurrentPosition( hps,  &ptlBegin );
   pmassert( hab, bOK );
   lRC = GpiBox( hps, DRO_OUTLINE, &ptlEnd, 0L, 0L );
   pmassert( hab, lRC != GPI_ERROR );
   bOK = GpiEndPath( hps );
   pmassert( hab, bOK );
   bOK = GpiFillPath( hps, 1L, FPATH_ALTERNATE );
   pmassert( hab, bOK );

   /* draw a black box around the page */
   bOK = GpiSetColor( hps, CLR_BLACK);
   pmassert( hab, bOK );
   pointl.x = 0;
   pointl.y = 0;
   bOK = GpiSetCurrentPosition( hps, &pointl );
   pmassert( hab, bOK );
   pointl.x = FixedInchesToTwips(pmp->form.fxxWidth);
   pointl.y = FixedInchesToTwips(pmp->form.fxyHeight);
   lRC = GpiBox( hps, DRO_OUTLINE, &pointl, 0L, 0L );
   pmassert( hab, lRC != GPI_ERROR );

} /* end of DrawBorder */


/*************************************************************************
 *
 * name: FixedInchesToTwips
 *
 * Description:  converts inches to TWIPS
 *
 * API's:  [none]
 *
 * Parameters:  fxInch = the size in inches as a fixed number
 *
 * Return:  Twips as a ULONG number
 *
 **************************************************************************/
ULONG FixedInchesToTwips(FIXED fxInch)
{
    ULONG    integer;
    ULONG    fraction;

    integer = FIXEDINT(fxInch) * 1440L;
    fraction = FIXEDFRAC(fxInch);
    fraction = (ULONG)(((LONG)fraction) * 1440L / 0x10000);
    return (integer+fraction);

} /*  end of FixedInchesToTwips()  */


/*************************************************************************
 *
 * name: FixedPointsToTwips
 *
 * Description:  converts a point size
 *
 * API's:  [none]
 *
 * Parameters:  fxPointSize = the point size as a fixed number
 *
 * Return:  Twips as a fixed number
 *
 **************************************************************************/
FIXED FixedPointsToTwips(FIXED fxPointSize)
{
    USHORT      integer;
    USHORT      fraction;

    integer = FIXEDINT(fxPointSize);
    integer *= 20;
    fraction = FIXEDFRAC(fxPointSize);
    fraction = (USHORT)(((LONG)fraction) * 20L / 0x10000);
    integer += fraction;

    return MAKEFIXED(integer, 0);
} /*  end of FixedPointsToTwips()  */


/*************************************************************************
 *
 * name: LoadBitmap
 *
 * Description:  loads a bitmap into a screen or printer PS
 *
 * API's:  [none]
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *
 * Return:  0 is sucessful otherwise a NACK message
 *
 **************************************************************************/
ULONG LoadBitmap( PMAIN_PARM pmp, ULONG flag )
{
   FILESTATUS3        filestatus3;
   HBITMAP            hbm;
   HDC                hdcCompatible;
   HFILE              hfile;
   PBITMAPFILEHEADER2 pbmpfileheader2;
   PBITMAPFILEHEADER2 pbmpfileheader2Work;
   PBITMAPINFOHEADER2 pbmp2;
   PBYTE              pBits;
   PBYTE              pBitsAligned;
   PHBITMAP           phbm;
   PHDC               phdc;
   PHPS               phps;
   SIZEL              sizel;
   ULONG              rc, ulAction, cBytes, cScans;
   ULONG              cbImageData;
   LONG               lRC;
   HAB                hab;

   hab = pmp->hab;

   /* open bitmap file */
   rc = DosOpen(  pmp->szFilename,
                  &hfile,
                  &ulAction,
                  0L,
                  FILE_NORMAL,
                  FILE_OPEN,
                  OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                  NULL );
   if( rc != 0 )
   {
     return WM_NACK_FILE_READING_ERROR;
   }

   /* query file size; returned in filestatus3.cbFile */
   rc = DosQueryFileInfo( hfile, FIL_STANDARD,
                                 (PVOID)&filestatus3, sizeof( filestatus3 ));
   pmassert( hab, rc == 0 );

   /* alloc memory for bitmap file read */
   pbmpfileheader2 = (PBITMAPFILEHEADER2) malloc( filestatus3.cbFile );
   pmassert( hab, pbmpfileheader2 );

   /* read bitmap file into memory */
   rc = DosRead( hfile, (PVOID)pbmpfileheader2, filestatus3.cbFile, &cBytes );
   if( rc != 0  ||  cBytes == 0 )
   {
     free( pbmpfileheader2 );
     DosClose( hfile );
     return WM_NACK_FILE_READING_ERROR;
   }

   DosClose( hfile );

   switch( pbmpfileheader2->usType )
   {
   case BFT_BITMAPARRAY:
     /* default to the first bitmap in the array */
     pbmpfileheader2Work = &((PBITMAPARRAYFILEHEADER2)pbmpfileheader2)->bfh2;
     pmassert( hab, pbmpfileheader2Work );
     pbmp2 = &pbmpfileheader2Work->bmp2;
     cbImageData = filestatus3.cbFile - pbmpfileheader2Work->offBits;
     pBits = (PBYTE)pbmpfileheader2 + pbmpfileheader2Work->offBits;
     break;

   case BFT_BMAP:
     /* set pbmp2 to point to the BITMAPINFO2 structure within the file */
     pbmp2 = &pbmpfileheader2->bmp2;
     cbImageData = filestatus3.cbFile - pbmpfileheader2->offBits;
     pBits = (PBYTE)pbmpfileheader2 + pbmpfileheader2->offBits;
     break;

   case BFT_ICON:
   case BFT_POINTER:
   case BFT_COLORICON:
   case BFT_COLORPOINTER:
     /* not supported by print sample */
     free( pbmpfileheader2 );
     return WM_NACK_BITMAP_NOT_SUPPORTED;

   default:
     /* invalid file format */
     free( pbmpfileheader2 );
     return WM_NACK_BITMAP_ERROR;
   }

   /* pBits should be sensible */
   pmassert( hab, pbmp2 );
   pmassert( hab, pBits );

   /* open a new memory dc with compatibility as required per mp2 argument */
   switch( flag )
   {
   case FLAGS_SCREEN:
     hdcCompatible = pmp->hdcClient;
     phdc          = &pmp->hdcMemory;
     phps          = &pmp->hpsMemory;
     phbm          = &pmp->hbm;
     break;

   case FLAGS_PRINTER:
     hdcCompatible = pmp->hdcPrinter;
     phdc          = &pmp->hdcMem4Printer;
     phps          = &pmp->hpsMem4Printer;
     phbm          = &pmp->hbm4Printer;
     break;

   default:
     /* this assert will fail and remind the programmer of a problem */
     pmassert( hab, NULL == "expected FLAGS_*" );
   }


   /* previous uses of hbm, hps, and hdc are properly closed and tidy */
   pmassert( hab, *phps == (HPS)NULLHANDLE );
   pmassert( hab, *phdc == (HDC)NULLHANDLE );
   pmassert( hab, *phbm == (HBITMAP)NULLHANDLE );


   *phdc = DevOpenDC(pmp->hab, OD_MEMORY, "*",0, NULL, hdcCompatible);
   pmassert( hab, *phdc );

   /* create the ps in memory to hold the bitmap */
   sizel.cx = 0;
   sizel.cy = 0;
   *phps = GpiCreatePS( pmp->hab, *phdc, &sizel,
                        PU_PELS | GPIA_ASSOC | GPIT_NORMAL );
   pmassert( hab, *phps );

   /* create a bitmap */
   *phbm = GpiCreateBitmap( *phps,
                            pbmp2,
                            0,                  /* options */
                            (PBYTE)NULL,        /* init table */
                            (PBITMAPINFO2)NULL );  /* bitmap info */

   /* check that bitmap has been created properly */
   if (*phbm == GPI_ERROR)
   {
      /* error creating bitmap */
      GpiAssociate( pmp->hpsMem4Printer, (HDC)NULLHANDLE );
      GpiDestroyPS( pmp->hpsMem4Printer );
      DevCloseDC( pmp->hdcMem4Printer );
      return WM_NACK_BITMAP_ERROR;
   }

   /* set bitmap into the memory ps */
   hbm = GpiSetBitmap( *phps, *phbm );
   pmassert( hab, hbm != HBM_ERROR );

   /*
    * determine if this is a 1.x or 2.x bitmap
    * by looking at the length of the BITMAPINFOHEADER structure
    */
   if( pbmp2->cbFix == sizeof(BITMAPINFOHEADER))
   {
      /* this is an OS2 PM 1.x bitmap */
      cScans = (ULONG) ((PBITMAPINFOHEADER)pbmp2)->cy;
      pmp->cyBitmap = ((PBITMAPINFOHEADER)pbmp2)->cy;
      pmp->cxBitmap = ((PBITMAPINFOHEADER)pbmp2)->cx;
      pmp->cxBitmapRes = pmp->cyBitmapRes = 0;
   }
   else
   {
      /* default to Windows and OS2 2.x PM bitmap */
      cScans = pbmp2->cy;
      pmp->cyBitmap = pbmp2->cy;
      pmp->cxBitmap = pbmp2->cx;
      pmp->cxBitmapRes = pbmp2->cxResolution;
      pmp->cyBitmapRes = pbmp2->cyResolution;
   }


   /*
    * Some print drivers expect the bit image data to be dword
    * aligned.  Allocate a new memory object and copy bit data
    * to it.
    */
   pBitsAligned = (PBYTE) malloc( cbImageData );
   pmassert( hab, pBitsAligned );
   pmassert( hab, ((ULONG)pBitsAligned & 3 ) == 0 );
   memcpy( pBitsAligned, pBits, cbImageData );


   /* take bitmap bits from file buffer and place into memory ps for bitmap */
   lRC = GpiSetBitmapBits( *phps,
                           0,
                           (LONG)cScans,
                           pBitsAligned,
                           (PBITMAPINFO2)pbmp2  );
   pmassert( hab, lRC != GPI_ERROR );

   /* clean up */
   free( pBitsAligned );
   free( pbmpfileheader2 );
   return 0;
} /* end of LoadBitmap */



/*************************************************************************
 *  Function   :  MMToTwips
 *
 *  Description:  Convert Millimetres to Twips
 *
 *  Parameters :  LONG       millimetres
 *
 *  API's      :  [none]
 *
 *  Return     :  LONG       twips
 *
 *************************************************************************/
LONG MMToTwips( LONG lMM )
{
   return ( (LONG)( ((float)lMM) / 25.4 * 1440.0) );
} /* end of MMToTwips */


/*************************************************************************
 *
 * name: PaintBitmap
 *
 * Description:  Paints a loaded bitmap onto the screen or printer
 *
 * API's:   GpiSetBitmap
 *          GpiWCBitBlt
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *
 * Return: [none]
 *
 **************************************************************************/
VOID PaintBitmap( PMAIN_PARM pmp, ULONG flag )
{
    HPS              hps, hpsBitmap;
    HBITMAP          hbm, hbmOld;
    POINTL           aptl[4];
    LONG             lResult;
    double           xRatio, yRatio;
    POINTL           pointlBegin, pointlEnd;

    pmassert( pmp->hab, pmp->hbm );

    /* define drawing area on screen or printer */
    CalculateDrawingArea( pmp, flag, &pointlBegin, &pointlEnd );

    /* setup handles for screen or printer */
    switch( flag )
    {
    case FLAGS_SCREEN:
        hps = pmp->hpsClient;
        hbm = pmp->hbm;
        hpsBitmap = pmp->hpsMemory;
        /* draw border around bitmap */
        DrawBorder( hps, pmp, pointlBegin, pointlEnd );
        break;

    case FLAGS_PRINTER:
        hps = pmp->hpsPrinter;
        hbm = pmp->hbm4Printer;
        hpsBitmap = pmp->hpsMem4Printer;
        break;

    default:
        pmassert( pmp->hab, NULL == "bad flag in PaintBitmap" );
    }

    /*
     * No clip path in defined for a bitmap as the bitmap is scaled to
     * fit the available drawing area
     */

    /* setup the source bitblt rectangle */
    aptl[2].x = 0;
    aptl[2].y = 0;
    aptl[3].x = pmp->cxBitmap;
    aptl[3].y = pmp->cyBitmap;

    /* Set up target bitblt rectangles */
    aptl[0].x = pointlBegin.x;
    aptl[0].y = pointlBegin.y;

    if (pmp->cxBitmapRes && pmp->cyBitmapRes)
    {
        /*
         * Since the intended resolution for the bitmap is known,
         * the actual size of the bitmap can be calculated, in
         * twips by converting pels/meter to pels/twip
         */
        aptl[1].x = ((double)pmp->cxBitmap)/((double)pmp->cxBitmapRes)
                    * 1000.0 / 25.4 * 1440.0;
        aptl[1].y = ((double)pmp->cyBitmap)/((double)pmp->cyBitmapRes)
                    * 1000.0 / 25.4 * 1440.0;
    }
    else
    {
       /*
        * Assume the bitmap aspect ratio is 1.0
        */
       xRatio = ((double)pmp->sizelPage.cx) / ((double)aptl[3].x);
       yRatio = ((double)pmp->sizelPage.cy) / ((double)aptl[3].y);
       if (yRatio > xRatio)
       {
           aptl[1].x = xRatio * (double) (aptl[3].x);
           aptl[1].y = xRatio * (double) (aptl[3].y);
       }
       else
       {
           aptl[1].x = yRatio * (double) (aptl[3].x);
           aptl[1].y = yRatio * (double) (aptl[3].y);
       }
    }

    /* move start position so bitmap is top of the page */
    aptl[0].y = pointlEnd.y - aptl[1].y;

    /* calculate final target top/right end-point */
    aptl[1].x = aptl[0].x + aptl[1].x;
    aptl[1].y = aptl[0].y + aptl[1].y;

    /* Deselect bitmap from memory */
    hbmOld = GpiSetBitmap( hpsBitmap, 0L );
    pmassert( pmp->hab, hbmOld != HBM_ERROR );

    /* Bitblt the source bitmap onto the printer presentation space */
    lResult = GpiWCBitBlt( hps,
                           hbm,
                           4L,
                           (PPOINTL)aptl,
                           ROP_SRCCOPY,
                           BBO_IGNORE );
    pmassert( pmp->hab, lResult == GPI_OK );

    return;
} /* end of PaintBitmap */


/*************************************************************************
 *
 * name: PaintDefaultScreen
 *
 * Description:  Paints the default screen
 *
 * API's:  [none]
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *
 * Return: [none]
 *
 **************************************************************************/
VOID PaintDefaultScreen( PMAIN_PARM pmp, ULONG flag )
{
    HPS              hps;
    POINTL           pointlBegin, pointlEnd;

    /* define drawing area on screen or printer */
    CalculateDrawingArea( pmp, flag, &pointlBegin, &pointlEnd );

    /* setup handles for screen or printer */
    switch( flag )
    {
    case FLAGS_SCREEN:
        hps = pmp->hpsClient;
        /* draw border around bitmap */
        DrawBorder( hps, pmp, pointlBegin, pointlEnd );
        break;

    default:
        pmassert( pmp->hab, NULL == "bad flag in PaintDefaultScreen" );
    }
    return;
} /* end of PaintDefaultScreen */


/*************************************************************************
 *
 * name: PaintMetaFile
 *
 * Description:  Paints a loaded metafile onto the screen or printer
 *
 * API's:  GpiQueryDefaultViewMatrix
 *         GpiSetDrawControl
 *         GpiResetBoundaryData
 *         GpiPlayMetaFile
 *         GpiScale
 *         GpiTranslate
 *         GpiSetViewingTransformMatrix
 *         GpiResetPS
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *
 * Return: [none]
 *
 * Reference: Graham C.E. Winn, "OS/2 PM GPI", VNR Computer Library
 *
 **************************************************************************/
VOID PaintMetaFile( PMAIN_PARM pmp, ULONG flag )
{
    HAB              hab;
    HPS              hps;
    LONG             lRC;
    BOOL             bOK;
    POINTL           pointlBegin, pointlEnd, ptl;
    BYTE             abDesc[ LEN_PLAYMETAFILEDESCS ];
    LONG             alOptions[ LEN_PLAYMETAFILEOPTS ];
    LONG             lCount;
    RECTL            rclBoundary;
    FIXED            afxScale[2];
    MATRIXLF         matlfXform, matlfOld, matlfModel;
    static MATRIXLF matlfIdentity = {0x10000,0,0,0,0x10000,0,0L,0L,1L };

    pmassert( pmp->hab, pmp->hmf );

    hab = pmp->hab;

    /* define drawing area on screen or printer */
    CalculateDrawingArea( pmp, flag, &pointlBegin, &pointlEnd );

    /* setup handles for screen or printer */
    switch( flag )
    {
    case FLAGS_SCREEN:
        hps = pmp->hpsClient;
        /* draw border around bitmap */
        DrawBorder( hps, pmp, pointlBegin, pointlEnd );
        break;

    case FLAGS_PRINTER:
        hps = pmp->hpsPrinter;
        break;

    default:
        pmassert( hab, NULL == "bad flag in PaintMetaFile" );
    }

    /*
     * No clip path in defined for a bitmap as the bitmap is scaled to
     * fit the available drawing area
     */

    /* query the default viewing transform */
    bOK = GpiQueryDefaultViewMatrix( hps, 9, &matlfOld );
    pmassert( hab, bOK );

    /* set metafile play options */
    memset( alOptions, 0, sizeof( alOptions ));
    alOptions[ PMF_SEGBASE             ] = 0;
    alOptions[ PMF_LOADTYPE            ] = LT_NOMODIFY;
    alOptions[ PMF_RESOLVE             ] = 0;
    alOptions[ PMF_LCIDS               ] = LC_LOADDISC;
    alOptions[ PMF_RESET               ] = RES_NORESET;
    alOptions[ PMF_SUPPRESS            ] = SUP_NOSUPPRESS;
    alOptions[ PMF_COLORTABLES         ] = CTAB_REPLACE;
    alOptions[ PMF_COLORREALIZABLE     ] = CREA_NOREALIZE;
    alOptions[ PMF_DEFAULTS            ] = DDEF_LOADDISC;

    /* find out the metafile bounding rectangle */
    bOK = GpiSetDrawControl( hps, DCTL_DISPLAY, DCTL_OFF );
    pmassert( hab, bOK );
    bOK = GpiSetDrawControl( hps, DCTL_BOUNDARY, DCTL_ON );
    pmassert( hab, bOK );
    bOK = GpiResetBoundaryData( hps );
    pmassert( hab, bOK );
    lRC = GpiPlayMetaFile( hps,
                           pmp->hmf,
                           LEN_PLAYMETAFILEOPTS,
                           alOptions,
                           &lCount,
                           LEN_PLAYMETAFILEDESCS,
                           abDesc );
    pmassert( hab, lRC != GPI_ERROR );
    bOK = GpiQueryBoundaryData( hps, &rclBoundary );
    pmassert( hab, bOK );
    /* set draw mode */
    bOK = GpiSetDrawControl( hps, DCTL_DISPLAY, DCTL_ON );
    pmassert( hab, bOK );
    bOK = GpiSetDrawControl( hps, DCTL_BOUNDARY, DCTL_OFF);
    pmassert( hab, bOK );

    /*
     * Determine scale parameters to scale from boundary dimensions to target
     * dimensions about bottom left of boundary. Ensure that both scale
     * factors are equal and set to the smaller of the two possible values
     * (to preserve the aspect ratio of the metafile).
     */
    if (rclBoundary.xRight > rclBoundary.xLeft)
    {
       afxScale[0] = ((double)pmp->sizelPage.cx) * 0x10000 /
                     ((double)(rclBoundary.xRight - rclBoundary.xLeft));
    }
    else
    {
       afxScale[0] = 0x10000;
    }
    if (rclBoundary.yTop > rclBoundary.yBottom)
    {
       afxScale[1] = ((double)pmp->sizelPage.cy) * 0x10000 /
                     ((double)(rclBoundary.yTop - rclBoundary.yBottom));
    }
    else
    {
       afxScale[1] = 0x10000;
    }
    if (afxScale[0] < afxScale[1])
    {
       afxScale[1] = afxScale[0];
    }
    else
    {
       afxScale[0] = afxScale[1];
    }

    /* scale metafile around boundary bottom,left corner */
    ptl.x = rclBoundary.xLeft;
    ptl.y = rclBoundary.yBottom;
    bOK = GpiScale( hps, &matlfXform, TRANSFORM_REPLACE, afxScale, &ptl );
    pmassert( hab, bOK );

    /* translate metafile to correct position */
    ptl.x = pointlBegin.x - rclBoundary.xLeft;
    /* move start position so metafile is top of the page */
    ptl.y = pointlEnd.y - rclBoundary.yBottom -
            (LONG)( ((double)(afxScale[1])) / 65536.0 *
                    ((double)(rclBoundary.yTop - rclBoundary.yBottom)) );

    bOK = GpiTranslate( hps, &matlfXform, TRANSFORM_ADD, &ptl );
    pmassert( hab, bOK );

    /* set default view transform to scale metafile to PS */
    bOK = GpiSetViewingTransformMatrix( hps, 9, &matlfXform,
                                        TRANSFORM_REPLACE);
    pmassert( hab, bOK );

    /* draw the metafile */
    alOptions[PMF_LCIDS] = LC_NOLOAD;
    alOptions[PMF_DEFAULTS] = DDEF_IGNORE;
    lRC = GpiPlayMetaFile( hps,
                           pmp->hmf,
                           LEN_PLAYMETAFILEOPTS,
                           alOptions,
                           &lCount,            /* segment count */
                           LEN_PLAYMETAFILEDESCS,
                           abDesc );
    pmassert( hab, lRC != GPI_ERROR );

    if (flag == FLAGS_SCREEN)
    {
       bOK = GpiResetPS( hps, GRES_ALL );
       pmassert( pmp->hab, bOK );
    }

    /* restore the default viewing transform */
    bOK = GpiSetDefaultViewMatrix( hps, 9, &matlfOld, TRANSFORM_REPLACE);
    pmassert( hab, bOK );

    return;
} /* end of PaintMetaFile() */


/*************************************************************************
 *
 * name: PaintText
 *
 * Description:  Paints the text to the screen or printer
 *
 * API's:  [none]
 *
 * Parameters: PMAIN_PARM  pointer to the global data structure
 *             ULONG       flag to indicate screen or printer
 *
 * Return: EOF flag    PAGINATE_EOF             end of file, no text drawn
 *                     PAGINATE_NOT_EOF         not end of file, text drawn
 *                     PAGINATE_EOF_PART_PAGE   end of file, text drawn
 *
 **************************************************************************/
ULONG PaintText( PMAIN_PARM pmp, ULONG flag )
{
    BOOL             bOK;
    CHAR             szWork[ LEN_WORKSTRING ];
    FONTMETRICS      fm;
    HAB              hab;
    HPS              hps;
    LONG             lCharHeight, lLeading, cLinesOnPage;
    LONG             lRC;
    POINTL           pointl;
    POINTL           pointlBegin, pointlEnd;
    PSZ              pszOK;
    SIZEF            sizef;

    hab = pmp->hab;

    pmassert( hab, pmp->f );
    pmassert( hab, pmp->ulMode == MODE_TEXT );

    /* check for end of file */
    if ( !fgets( szWork, LEN_WORKSTRING, pmp->f) )
    {
      if (flag == FLAGS_SCREEN)
      {
        /* for screen reset back the beginning and read first line */
        fseek( pmp->f, 0, SEEK_SET );
        fgets( szWork, LEN_WORKSTRING, pmp->f);
      }
      else
      {
        /* for printer, return PAGINATE_EOF */
        return PAGINATE_EOF;
      }
    }

    /* define drawing area on screen or printer */
    CalculateDrawingArea( pmp, flag, &pointlBegin, &pointlEnd );

    /* prepare to draw on screen or printer */
    switch( flag )
    {
    case FLAGS_SCREEN:
       hps = pmp->hpsClient;
       /*
        * Paginate the text into a retained segment. The text drawing
        * orders can then be played back at WM_PAINT time with a call
        * to GpiDrawChain.
        */
       bOK = GpiSetDrawingMode( hps, DM_RETAIN );
       pmassert( hab, bOK );

       GpiErase( hps );

       /* delete any previous segment; segment 1 is all we use */
       GpiDeleteSegment( hps, 1 );

       /* open a new segment for the text using segment ID 1 */
       bOK = GpiOpenSegment( hps, 1 );
       pmassert( hab, bOK );

       /* clear PS and draw form border */
       DrawBorder( hps, pmp, pointlBegin, pointlEnd );

       break;

    case FLAGS_PRINTER:
       hps = pmp->hpsPrinter;
       break;

    case FLAGS_INFO:
       hps = pmp->hpsPrinterInfo;
       break;

    default:
       pmassert( hab, NULL == "bad flag in WM_USER_PAGINATE" );
    }

    pmassert( hab, hps );

    /*
     * Set clipping path to page size defined by user.
     * If user picks a font too big, he'll know this visually.
     */
    SetClipPath( hps, pmp, pointlBegin, pointlEnd );

    /* use black color for text */
    bOK = GpiSetColor( hps, CLR_BLACK );
    pmassert( hab, bOK );


    /* Discard LCID (logical character identifier) 1 if it exists. */
    if( GpiSetCharSet( hps, 1 ))
    {
        /* select font 0 and delete font 1   */
        GpiSetCharSet(hps, 0 );
        GpiDeleteSetId(hps, 1 );
    }

    /* select default font 0 */
    GpiSetCharSet(hps, 0 );



    /*
     *  Create the logical font with fAttrs from the font dialog.
     *
     *  This version of the print sample only requests vector (outline)
     *  fonts of the font dialog. (Reference: PRTCREAT.C, the
     *  initialization of the pmp->fontdlg structure.)
     *
     *  Create and select the requested font into the PS and query its
     *  font metrics.
     */

    lRC = GpiCreateLogFont( hps, NULL, 1, &pmp->fontdlg.fAttrs );
    pmassert(hab, lRC != GPI_ERROR );

    if( lRC == FONT_DEFAULT )
    {
      /*
       * This can happen on Postscript device fonts which are outline fonts,
       * and also LaserJet PCL 5 scalable printer device fonts. These
       * have to be emulated on the screen. Give a warning to user that
       * some default font is in use, and carry on.
       *
       * Only warn the user once after each visit to the font dialog.
       * Don't re-enable the client yet. That message is still to come.
       */
      if( pmp->fVisitedFontDialog )
      {
         /* only one warning per font dialog visit */
         pmp->fVisitedFontDialog  = FALSE;

         bOK = WinLoadString( pmp->hab, (HMODULE)0,
                      ERRMSG_DEFAULT_FONT, LEN_WORKSTRING, szWork );
         pmassert( pmp->hab, bOK );
         WinMessageBox( HWND_DESKTOP,
                        pmp->hwndFrame,
                        szWork,
                        pmp->pszTitle,
                        (USHORT)0,
                        MB_OK | MB_MOVEABLE | MB_CUACRITICAL | MB_APPLMODAL);
      }
    }

    bOK = GpiSetCharSet( hps, 1 );
    pmassert( hab, bOK );

    bOK = GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), &fm );
    pmassert(hab, bOK);



    /*
     *   GpiSetCharBox
     *
     *   "Char box" the font to the point size desired by the user.
     *   When sizef.cx == sizef.cy, the result is a normally-proportioned font.
     *   Setting sizef.cx to one-half of sizef.cy yields a compressed font,
     *   and setting sizef.cx to twice sizef.cy yields an expanded font.
     *   Experiment with these settings; this is one of the most flexible
     *   aspects of the Adobe font rasterizing technology in the OS2 graphics
     *   engine. It is true WYSIWYG and very device independent.
     *
     *   Although not demonstrated in this version of the print sample,
     *   GpiSetCharBox will play an important role in emulating printer device
     *   fonts on the screen device for WYSIWYG output.
     */

    sizef.cy = sizef.cx = FixedPointsToTwips( pmp->fontdlg.fxPointSize );
    bOK = GpiSetCharBox(hps, &sizef);
    pmassert(hab, bOK);


    /*
     *  Set Line Space
     *
     *  Here are three methods for determining the vertical
     *  line spacing (also known as line increment or leading)
     *  between lines of text.
     *
     *  1) Add two values from the fontmetrics structure:
     *  lMaxBaselineExt + lExternalLeading.
     *
     *  2) Use fontmetrics values lEmHeight * 1.20 + lExternalLeading
     *  lExternalLeading is zero for PM fonts.
     *
     *  3) The typesetters' rule of thumb: lead is pointsize plus two points.
     *
     * Method 1 and 2 require A) a re-read of the font metrics after the call
     * to GpiSetCharBox because those metrics will change as a result of
     * GpiSetCharBox, or B) some computation given the current font metrics.
     * Method 3 is simplest because the only variable is point size, and that
     * was specified by the user.
     *
     * 1 point = 20 twips.
     */

    lCharHeight =  20 * FIXEDINT( pmp->fontdlg.fxPointSize ) +
                   20 * FIXEDFRAC( pmp->fontdlg.fxPointSize ) / 0x10000;
    lLeading =  lCharHeight + 40;

    /* compute lines on page (leave one line at the bottom for descenders) */
    cLinesOnPage = (pmp->sizelPage.cy / lLeading) - 1;

    /* start at the upper left corner of page */
    pointl.x = pointlBegin.x;
    pointl.y = pointlEnd.y - lLeading;

    /*
     * Read from text file at current file position and draw as much of it
     * as will fit on the page.
     */
    do
    {
        /* remove trailing white space */
        trim( szWork );

        /* move to the spot on the page */
        bOK = GpiSetCurrentPosition( hps,  &pointl   );
        pmassert( hab, bOK );

        lRC = GpiCharString( hps, strlen( szWork ), szWork );
        pmassert( hab, lRC != GPI_ERROR );

        /* move down the page */
        pointl.y -= lLeading;

        /* have reached bottom of page? */
        if( --cLinesOnPage == 0  )
        {
            /* yes, bottom of page; break out of this while loop */
            break;
        }
        pszOK = fgets( szWork, LEN_WORKSTRING, pmp->f );
    } while( pszOK );

    /* reset the clip path */
    bOK = GpiSetClipPath( hps, 0L, SCP_RESET );
    pmassert( pmp->hab, bOK );

    if( flag == FLAGS_SCREEN )
    {

      /* close the drawing segment; only the screen uses retained segments */
      bOK = GpiCloseSegment( hps );
      pmassert( hab, bOK );
      bOK = GpiSetDrawingMode( hps, DM_DRAW );
      pmassert( hab, bOK );
    }

    /* something WAS drawn on the page, yet could be at end of file now */
    lRC = (pszOK) ? PAGINATE_NOT_EOF : PAGINATE_EOF_PART_PAGE;

    return lRC;
}


/*************************************************************************
 *
 * name: SetClipPath
 *
 * Description:  Sets the clipping path for the PS
 *
 * API's:  [none]
 *
 * Parameters:  HPS         presentation space to use
 *              PMAIN_PARM  pointer to the global data structure
 *              PPOINTL     pointer to begin point of drawing area
 *              PPOINTL     pointer to end point of drawing area
 *
 * Return: [none]
 *
 **************************************************************************/
VOID SetClipPath( HPS hps, PMAIN_PARM pmp, POINTL ptlBegin, POINTL ptlEnd )
{
   BOOL     bOK;
   LONG     lRC;
   HAB      hab;
   POINTL   ptl;

   hab = pmp->hab;
   bOK = GpiBeginPath( hps, 1);
   pmassert( hab, bOK );
   /* clip path is inclusive at bottom, left */
   ptl.x = ptlBegin.x;
   ptl.y = ptlBegin.y;
   bOK = GpiSetCurrentPosition( hps,  &ptl );
   pmassert( hab, bOK );
   /* clip path is inclusive at top, right */
   ptl.x = ptlEnd.x;
   ptl.y = ptlEnd.y;
   lRC = GpiBox( hps, DRO_OUTLINE, &ptl, 0L, 0L );
   pmassert( hab, lRC != GPI_ERROR );
   bOK = GpiEndPath( hps );
   pmassert( hab, bOK );
   bOK = GpiSetClipPath( hps, 1L, SCP_AND );
   pmassert( hab, bOK );

} /* end of SetClipPath */


/*************************************************************************
*
* Name: trim
*
* Description: removes trailing whitespace characters from a string
*
* Parameters: psz = a string
*
* Returns: a pointer to the same string passed in
*
**************************************************************************/
PSZ trim( PSZ s )
{
  PCH p;

  /* point p to null-terminator byte of string s */
  p =  s + strlen( s );

  /* work backward and remove white space characters at end of string */
  p--;
  while( s <= p && ( *p == '\r' || *p == ' ' || *p == '\n' || *p == '\t' ))
  {
    *p = 0;
    p--;
  }

  return s;
}  /*  end of trim()  */

/***************************  End of prtobj.c ****************************/

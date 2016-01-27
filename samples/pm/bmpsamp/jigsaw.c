/******************************************************************************
*
*  File Name   : JIGSAW.C
*
*  Description : main function and code for the Jigsaw application
*
*     This program provides a jigsaw puzzle, based on a decomposition of an
*     arbitrary bitmap loaded from a file.  The user can jumble the pieces,
*     then drag them individually by means of the mouse.  The image can be
*     zoomed in and out and scrolled up/down and left/right.
*
*     JIGSAW uses GpiBitBlt with clip paths to create a collection of picture
*     fragments which are the puzzle pieces.  In earlier versions of the
*     program, each of these pieces was associated with a single retain-mode
*     graphics segment.  The retain-mode technique, however, proved to be
*     too slow, so subsequent versions of the program used retain-mode APIs
*     for fewer and fewer operations.  The current version eliminates
*     retain-mode graphics altogether.  Instead, the drawing data for each
*     piece is stored in _SEGLIST data structure defined in JIGSAW.H.
*     This structure contains all the data needed to draw a piece, including
*     pointers to the previous and next pieces.  The _SEGLIST nodes are
*     arranged in drawing priority order, so the picture can be reconstructed
*     by traversing the list in sequence, drawing each piece as its
*     corresponding structure is encountered.  Where the comments in the
*     rest of the program refer to a "segment," they are simply referring to
*     a piece of the puzzle as defined by a record in this data structure.
*
*     To retain responsiveness to user requests, the real work is done in a
*     second thread, with work requests transmitted from the main thread in
*     the form of messages.  This arrangement makes it possible for the user
*     to override lengthy drawing operations with a higher-priority request
*     (eg. program termination, magnification change, etc.).
*
*     Individual pieces are made to "move" by changing their model transforms.
*     Scrolling and zooming of the whole picture is done by changing the
*     default viewing transform.  The points in model space associated with
*     each piece (control points for the bounding curve, corners of the
*     bounding box, etc.) are converted via GpiConvert into points in device
*     space prior to use with GpiBitBlt, etc.
*
*  Concepts    : Illustrates the use of GPI
*                Illustrates the use of off-screen bitmaps
*
*  Entry Points:
*                CalcSize()
*                CheckMenu()
*                CheckPsl()
*                CreatePicture()
*                DispErrorMsg()
*                DoHorzScroll()
*                DosGetThreadInfo()
*                DoVertScroll()
*                DumpPicture()
*                EnableMenu()
*                NewThread()
*                Finalize()
*                Initialize()
*                Jumble()
*                LeftDown()
*                LeftUp()
*                Load()
*                main()
*                MouseMove()
*                PrepareBitmap()
*                ReadBitmap()
*                ReportError()
*                ResetScrollBars()
*                SegListCheck()
*                ToBottom()
*                Zoom()
*
*  API's       : GpiBeginPath
*                GpiEndPath
*                GpiFillPath
*                GpiSetClipPath
*                GpiSetClipRegion
*
*                GpiCreateBitmap
*                GpiDeleteBitmap
*                GpiSetBitmap
*                GpiSetBitmapBits
*                GpiBitBlt
*
*                GpiConvert
*
*                GpiCreateRegion
*                GpiCombineRegion
*                GpiSetRegion
*                GpiDestroyRegion
*                GpiQueryRegionBox
*
*                GpiSetAttrMode
*                GpiSetColor
*
*                GpiQueryDefaultViewMatrix
*                GpiSetDefaultViewMatrix
*
*  Copyright (C) 1992 IBM Corporation
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
******************************************************************************/

#include "jigsaw.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/******************************************************************************
*
*  Name        : main
*
*  Description : Main thread will initialize the process for PM services and
*                process the application message queue until a WM_QUIT message
*                is received.  It will then destroy all PM resources and
*                terminate.  Any error during initialization will be reported
*                and the process terminated.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : VOID
*
*  Return      :
*
******************************************************************************/

VOID main(VOID)
{
  QMSG  qmsg;

  if( Initialize())
      while( WinGetMsg( habMain, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg( habMain, &qmsg);
  else
      ReportError( habMain);
  Finalize();

}   /* end main() */

/******************************************************************************
*
*  Name        : Initialize
*
*  Description :
*
*  The Initialize function will initialize the PM interface,
*  create an application message queue, a standard frame window and a new
*  thread to control drawing operations.  It will also initialize static
*  strings.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : VOID
*
*  Return      : BOOL
*
******************************************************************************/

BOOL Initialize(VOID)
{
  ULONG    flCreate;
  PID      pid;
  TID      tid;

   /*
    * create all semaphores for mutual exclusion and event timing
    */
                   /* (anonomous,            ,    private,  reset) */

   if (DosCreateEventSem( NULL,  &hevDrawOn,          0,    FALSE) ||
       DosCreateEventSem( NULL,  &hevMouse,           0,    FALSE) ||
       DosCreateEventSem( NULL,  &hevLoadingBitmap,   0,    FALSE) ||
       DosCreateEventSem( NULL,  &hevTerminate,       0,    FALSE) ||
       DosCreateEventSem( NULL,  &hevKillDraw,        0,    FALSE))
   {
       return (FALSE);                       /* failed to create a semaphore */
   }

   WinShowPointer( HWND_DESKTOP, TRUE);
   habMain = WinInitialize( 0);
   if( !habMain)
       return( FALSE);

   hmqMain = WinCreateMsgQueue( habMain,0);
   if( !hmqMain)
       return( FALSE);

   WinLoadString( habMain, 0, IDS_TITLEBAR, sizeof(szTitle), szTitle);
   WinLoadString( habMain, 0, IDS_ERROR_TITLE, sizeof(szErrorTitle), szErrorTitle);
   if( !WinRegisterClass( habMain
                        , (PCH)szTitle
                        , ClientWndProc
                        , CS_SIZEREDRAW
                        , 0 ))
       return( FALSE);

   flCreate =   (FCF_STANDARD | FCF_VERTSCROLL | FCF_HORZSCROLL)
              & ~(ULONG)FCF_TASKLIST;
   hwndFrame =
   WinCreateStdWindow(
      HWND_DESKTOP,                       /* handle of the parent window     */
      WS_VISIBLE,                         /* frame-window style              */
      &flCreate,                          /* creation flags                  */
      szTitle,                            /* client-window class name        */
      szTitle,                            /* address of title-bar text       */
      WS_VISIBLE,                         /* client-window style             */
      0,                                  /* handle of the resource module   */
      IDR_MAIN,                           /* frame-window identifier         */
      &hwndClient);                       /* address of client-window handle */

   if( !hwndFrame)
       return( FALSE);

   sizlMaxClient.cx = WinQuerySysValue( HWND_DESKTOP, SV_CXFULLSCREEN);
   sizlMaxClient.cy = WinQuerySysValue( HWND_DESKTOP, SV_CYFULLSCREEN);

   lByteAlignX = WinQuerySysValue( HWND_DESKTOP, SV_CXBYTEALIGN);
   lByteAlignY = WinQuerySysValue( HWND_DESKTOP, SV_CYBYTEALIGN);

   hdcClient = WinOpenWindowDC( hwndClient);
   hpsClient = GpiCreatePS( habMain
                          , hdcClient
                          , &sizlMaxClient
                          , GPIA_ASSOC | PU_PELS );
   if( !hpsClient)
   {
    DispError(habMain,hwndClient);
    return( TRUE);
   }
   if (GpiSetAttrMode( hpsClient, AM_PRESERVE) == GPI_ERROR)
       DispError(habMain,hwndClient);

   hwndHorzScroll = WinWindowFromID( hwndFrame, FID_HORZSCROLL);

   hwndVertScroll = WinWindowFromID( hwndFrame, FID_VERTSCROLL);

   if ((hpsPaint = GpiCreatePS( habMain, NULLHANDLE, &sizlMaxClient, PU_PELS)) == GPI_ERROR)
                  DispError(habMain,hwndClient);

   hrgnInvalid = GpiCreateRegion( hpsClient, 0L, NULL);

   WinQueryWindowProcess( hwndFrame, &pid, &tid);
   swctl.hwnd      = hwndFrame;
   swctl.idProcess = pid;
   strcpy( swctl.szSwtitle, szTitle);
   hsw = WinAddSwitchEntry( &swctl);

   hwndMenu = WinWindowFromID( hwndFrame, FID_MENU);

   if (tidAsync = _beginthread(NewThread,
                    0,
                    STACKSIZE,
                    0) == -1)
      return(FALSE);
   if( !CreateBitmapHdcHps( &hdcBitmapFile, &hpsBitmapFile))
       return( FALSE);
   if( !CreateBitmapHdcHps( &hdcBitmapSize, &hpsBitmapSize))
       return( FALSE);
   if( !CreateBitmapHdcHps( &hdcBitmapBuff, &hpsBitmapBuff))
       return( FALSE);
   if( !CreateBitmapHdcHps( &hdcBitmapSave, &hpsBitmapSave))
       return( FALSE);

   ResetScrollBars();
   /*
    * initialise help mechanism
    */

   HelpInit();
   return( TRUE);

}   /* end Initialize() */

/******************************************************************************
*
*  Name        : Finalize
*
*  Description :
*
* Finalize will destroy the asynchronous drawing thread, all Presentation
* Manager resources, and terminate the process.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : VOID
*
*  Return      : VOID
*
******************************************************************************/

VOID Finalize(VOID)
{
  ULONG ulPostCt;

  if( tidAsync)
  {
      DosResetEventSem( hevDrawOn, &ulPostCt);
      DosPostEventSem( hevKillDraw);
      DosPostEventSem( hevTerminate);
      SendCommand( UM_DIE, (MPARAM)NULL, (MPARAM)NULL);
  }

  while( pslHead != NULL )
  {
    if (GpiSetBitmap( pslHead->hpsFill, NULLHANDLE) == (HBITMAP)BMB_ERROR)
      DispError(habMain,hwndClient);
    if (GpiDeleteBitmap( pslHead->hbmFill) == GPI_ERROR)
      DispError(habMain,hwndClient);
    if (GpiDestroyPS( pslHead->hpsFill) == GPI_ERROR)
      DispError(habMain,hwndClient);
    DevCloseDC( pslHead->hdcFill);

    if (GpiSetBitmap( pslHead->hpsHole, NULLHANDLE)== (HBITMAP)BMB_ERROR)
      DispError(habMain,hwndClient);
    if (GpiDeleteBitmap( pslHead->hbmHole) == GPI_ERROR)
      DispError(habMain,hwndClient);
    if (GpiDestroyPS( pslHead->hpsHole) == GPI_ERROR)
      DispError(habMain,hwndClient);
    DevCloseDC( pslHead->hdcHole);

    SegListUpdate( DEL_SEG, pslHead);
  }

  if( hrgnInvalid)
      GpiDestroyRegion( hpsClient, hrgnInvalid);
  if( hpsClient)
  {
      GpiAssociate( hpsClient, NULLHANDLE);
      GpiDestroyPS( hpsClient);
  }
  if( hpsPaint)
      GpiDestroyPS( hpsPaint);

  if( hpsBitmapFile)
  {
      GpiSetBitmap( hpsBitmapFile, NULLHANDLE);
      GpiDeleteBitmap( hbmBitmapFile);
      GpiDestroyPS( hpsBitmapFile);
      DevCloseDC( hdcBitmapFile);
  }
  if( hpsBitmapSize)
  {
      GpiSetBitmap( hpsBitmapSize, NULLHANDLE);
      GpiDeleteBitmap( hbmBitmapSize);
      GpiDestroyPS( hpsBitmapSize);
      DevCloseDC( hdcBitmapSize);
  }
  if( hpsBitmapBuff)
  {
      GpiSetBitmap( hpsBitmapBuff, NULLHANDLE);
      GpiDeleteBitmap( hbmBitmapBuff);
      GpiDestroyPS( hpsBitmapBuff);
      DevCloseDC( hdcBitmapBuff);
  }
  if( hpsBitmapSave)
  {
      GpiSetBitmap( hpsBitmapSave, NULLHANDLE);
      GpiDeleteBitmap( hbmBitmapSave);
      GpiDestroyPS( hpsBitmapSave);
      DevCloseDC( hdcBitmapSave);
  }

  DestroyHelpInstance();

  if( hwndFrame)
      WinDestroyWindow( hwndFrame);
  if( hmqMain)
      WinDestroyMsgQueue( hmqMain);
  if( habMain)
      WinTerminate( habMain);

  DosExit( EXIT_PROCESS, 0);

}   /* end Finalize() */

/******************************************************************************
*
*  Name        : ReportError
*
*  Description :
*
* ReportError  will display the latest error information for the required
* thread. No resources to be loaded if out of memory error.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : HAB hab
*
*  Return      : VOID
*
******************************************************************************/

VOID ReportError(HAB hab)
{
PERRINFO  perriBlk;
PSZ       pszErrMsg;
PSZ       pszOffSet;

  if (!fErrMem){
      if ((perriBlk = WinGetErrorInfo(hab)) != (PERRINFO)NULL){
          pszOffSet = ((PSZ)perriBlk) + perriBlk->offaoffszMsg;
          pszErrMsg = ((PSZ)perriBlk) + *((PULONG)pszOffSet);
          WinMessageBox(HWND_DESKTOP,
                        hwndFrame,
                       (PSZ)(pszErrMsg),
                       (PSZ)szTitle,
                        0,
                        MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL );
          WinFreeErrorInfo(perriBlk);
          return;
      }
  } /* endif */

  MessageBox(                                                       /* ERROR */
     hwndFrame,
     IDS_ERROR_OUTOFMEMORY,
     MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL,
     TRUE);

}   /* end ReportError() */

/******************************************************************************
*
*  Name        : ResetScrollBars
*
*  Description :
*
* Reset the scroll bars to be in the middle of their range
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : VOID
*
*  Return      : VOID
*
******************************************************************************/

VOID ResetScrollBars(VOID)
{
    RECTL     rclClient;

    WinQueryWindowRect( hwndClient, &rclClient);
    ptsScrollMax.x = (SHORT)(rclClient.xRight - rclClient.xLeft);
    ptsHalfScrollMax.x = ptsScrollMax.x >> 1;
    ptsScrollPage.x = ptsScrollMax.x >> 3;
    ROUND_DOWN_MOD( ptsScrollPage.x, (SHORT)lByteAlignX);
    ptsScrollLine.x = ptsScrollMax.x >> 5;
    ROUND_DOWN_MOD( ptsScrollLine.x, (SHORT)lByteAlignX);
    ptsScrollPos.x = ptsHalfScrollMax.x;
    ptsOldScrollPos.x = ptsHalfScrollMax.x;
    WinSendMsg( hwndHorzScroll
              , SBM_SETSCROLLBAR
              , MPFROMSHORT( ptsScrollPos.x)
              , MPFROM2SHORT( 1, ptsScrollMax.x) );

    ptsScrollMax.y = (SHORT)(rclClient.yTop - rclClient.yBottom);
    ptsHalfScrollMax.y = ptsScrollMax.y >> 1;
    ptsScrollPage.y = ptsScrollMax.y >> 3;
    ROUND_DOWN_MOD( ptsScrollPage.y, (SHORT)lByteAlignY);
    ptsScrollLine.y = ptsScrollMax.y >> 5;
    ROUND_DOWN_MOD( ptsScrollLine.y, (SHORT)lByteAlignY);
    ptsScrollPos.y = ptsHalfScrollMax.y;
    ptsOldScrollPos.y = ptsHalfScrollMax.y;
    WinSendMsg( hwndVertScroll
              , SBM_SETSCROLLBAR
              , MPFROMSHORT( ptsScrollPos.y)
              , MPFROM2SHORT( 1, ptsScrollMax.y) );

    WinSendMsg( hwndHorzScroll
              , SBM_SETTHUMBSIZE
              , MPFROM2SHORT(ptsScrollPage.x, ptsScrollMax.x)
              , 0                                );

    WinSendMsg( hwndVertScroll
              , SBM_SETTHUMBSIZE
              , MPFROM2SHORT(ptsScrollPage.y, ptsScrollMax.y)
              , NULL                             );

}   /* end ResetScrollBars() */

/******************************************************************************
*
*  Name        : Load
*
*  Description : Load a bitmap
*
*  Parameters  : PLOADINFO pli - pointer to file information (name, handle...)
*
*  Return      : VOID
*
******************************************************************************/

VOID Load(PLOADINFO pli)
{
 ULONG     ulPostCt;
 PCHAR     pFilename;
 UCHAR     szWindowText[CCHMAXPATH];

   DosResetEventSem( hevLoadingBitmap, &ulPostCt);  /* begin loading bitmap */

   /*
    *   Disable bitmap manipulation submenus
    */

   EnableMenu(IDM_SIZE_MENU,   FALSE);
   EnableMenu(IDM_SIZE_SMALL,  FALSE);
   EnableMenu(IDM_SIZE_MEDIUM, FALSE);
   EnableMenu(IDM_SIZE_LARGE,  FALSE);
   EnableMenu(IDM_SIZE_FULL,   FALSE);
   EnableMenu(IDM_JUMBLE,      FALSE);

   CheckMenu(Currentchecked, FALSE);                    /* uncheck old menu */

   if( hbmBitmapFile)
   {
      if (GpiSetBitmap( hpsBitmapFile, NULLHANDLE) == (HBITMAP)BMB_ERROR)
          DispError(habMain,hwndClient);
      if (GpiDeleteBitmap( hbmBitmapFile) == GPI_ERROR)
          DispError(habMain,hwndClient);
   }

   if (!ReadBitmap( pli->hf))
   {
      MessageBox(hwndClient, IDS_ERROR_READ, MB_OK | MB_ERROR, TRUE);
      DosPostEventSem( hevLoadingBitmap);             /* end loading bitmap */
      if (!fFirstLoad)
      {
         /*
          *   Re-Enable bitmap manipulation submenus
          */

         EnableMenu(IDM_SIZE_MENU,   TRUE);
         EnableMenu(IDM_SIZE_SMALL,  TRUE);
         EnableMenu(IDM_SIZE_MEDIUM, TRUE);
         EnableMenu(IDM_SIZE_LARGE,  TRUE);
         EnableMenu(IDM_SIZE_FULL,   TRUE);
         EnableMenu(IDM_JUMBLE,      TRUE);

         CheckMenu(Currentchecked, TRUE);                /* recheck old menu */

      }   /* end if (!fFirstLoad) */

      return;
   }

   /*
    *   Change Title Bar text and Window List text to include filename.
    */

   strcpy(swctl.szSwtitle, szTitle);
   strcat(swctl.szSwtitle, " - ");
   strcpy(szWindowText, swctl.szSwtitle);            /* save duplicate part */
   pFilename = strrchr(pli->szFileName, '\\');         /* contain pathname? */

   if (pFilename == NULL)
      pFilename = pli->szFileName;         /* filename doesn't contain path */
   else
      ++pFilename;                            /* point past last back-slash */
   strcat( swctl.szSwtitle, pFilename);
   WinChangeSwitchEntry( hsw, &swctl);

   strcat(szWindowText, pli->szFileName);    /* use full filename in window */
   WinSetWindowText( hwndFrame, szWindowText);
   ResetScrollBars();

   if (fFirstLoad ||
       (PBMP1(pbmp2BitmapFile)->cx        >  PBMP1(pbmp2BitmapFileRef)->cx ||
        PBMP1(pbmp2BitmapFile)->cy        >  PBMP1(pbmp2BitmapFileRef)->cy ||
        PBMP1(pbmp2BitmapFile)->cPlanes   != PBMP1(pbmp2BitmapFileRef)->cPlanes ||
        PBMP1(pbmp2BitmapFile)->cBitCount != PBMP1(pbmp2BitmapFileRef)->cBitCount))
   {
     if( !fFirstLoad)
              DumpPicture();
     if( !PrepareBitmap() )
     {
        MessageBox(hwndClient, IDS_ERROR_READ, MB_OK | MB_ERROR, TRUE);
        DosPostEventSem( hevLoadingBitmap);      /* end loading bitmap */
        if (!fFirstLoad)
        {
           /*
            *   Re-Enable bitmap manipulation submenus
            */

           EnableMenu(IDM_SIZE_MENU,   TRUE);
           EnableMenu(IDM_SIZE_SMALL,  TRUE);
           EnableMenu(IDM_SIZE_MEDIUM, TRUE);
           EnableMenu(IDM_SIZE_LARGE,  TRUE);
           EnableMenu(IDM_SIZE_FULL,   TRUE);
           EnableMenu(IDM_JUMBLE,      TRUE);

           CheckMenu(Currentchecked, TRUE);             /* recheck old menu */

        }   /* end if (!fFirstLoad) */

        return;
     }
     CreatePicture( PICTURE_CREATE);
     bmp2BitmapFileRef = bmp2BitmapFile;
   } else
   {
     CreatePicture( PICTURE_UPDATE);
   }

   lScale = 0;

   CalcBounds();
   ptlScaleRef.x = ptlScaleRef.y = 0L;
   CalcTransform( hwndClient);

   DosPostEventSem( hevLoadingBitmap);                /* end loading bitmap */

   fFirstLoad = FALSE;
   fPictureAssembled = TRUE;
   BmpLoaded = TRUE;

   /*
    *   Enable bitmap manipulation submenus
    */
   EnableMenu(IDM_SIZE_MENU,   TRUE);
   EnableMenu(IDM_SIZE_SMALL,  TRUE);
   EnableMenu(IDM_SIZE_MEDIUM, TRUE);
   EnableMenu(IDM_SIZE_LARGE,  TRUE);
   EnableMenu(IDM_SIZE_FULL,   TRUE);
   EnableMenu(IDM_JUMBLE,      TRUE);

   CheckMenu(IDM_SIZE_FULL, TRUE);                        /* check full size */
   Currentchecked = IDM_SIZE_FULL;

}   /* end Load() */

/******************************************************************************/
/* Throw the pieces around the screen.                                        */
/******************************************************************************/
VOID Jumble(VOID)
{
  LONG      lWidth, lHeight;
  DATETIME  date;
  POINTL    ptl;
  RECTL     rclClient;
  PSEGLIST  psl;

  if( WinQueryWindowRect( hwndClient, &rclClient) )
  {
    lWidth  = rclClient.xRight - rclClient.xLeft;
    lHeight = rclClient.yTop   - rclClient.yBottom;
    if( (lWidth > 0) && (lHeight > 0) )
    {
       DosGetDateTime( &date);
       srand( (USHORT)date.hundredths);
       for( psl = pslHead; psl != NULL; psl = psl->pslNext)
       {
               psl->pslNextIsland = psl;    /* reset island pointer                   */
               psl->fIslandMark = FALSE;    /* clear island mark                      */
               ptl.x = rclClient.xLeft   + (rand() % lWidth);
               ptl.y = rclClient.yBottom + (rand() % lHeight);
               if (GpiConvert( hpsClient, CVTC_DEVICE, CVTC_MODEL, 1L, &ptl) == GPI_ERROR)
                 DispError(habMain,hwndClient);
               ptl.x = 50 * (ptl.x / 50) - 250;
               ptl.y = 50 * (ptl.y / 50) - 250;
               psl->ptlModelXlate.x = ptl.x - psl->ptlLocation.x;
               psl->ptlModelXlate.y = ptl.y - psl->ptlLocation.y;
               SetRect( psl);
      }
    }
    fPictureAssembled = FALSE;
  }
}

/******************************************************************************/
/*                                                                                                                                                  */
/*                                                                                                                                                  */
/*                                                                                                                                                  */
/******************************************************************************/
VOID ToBottom( pslDown)
PSEGLIST pslDown;
{
  BOOL      fFirst;
  PSEGLIST  psl;

  for( psl = pslDown, fFirst = TRUE
     ; (psl != pslDown) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
    SegListUpdate( MAKE_HEAD_SEG, psl);       /* at head => lowest priority   */
}


/******************************************************************************/
/*                                                                                                                                                  */
/* NewThread is the asynchronous drawing thread. It is responsible for all    */
/* drawing.  It will initialize its PM interface and create an application    */
/* message queue.  It will then monitor its message queue and process any     */
/* commands received.                                                                                                           */
/*                                                                                                                                                  */
/******************************************************************************/
VOID NewThread(PVOID pvData)
{
  QMSG      qmsgAsync, qmsgPeek;
  BOOL      fDone;
  POINTL    aptlDraw[3];
  USHORT    usChar, fsKeyFlags;
  PSEGLIST  psl;
  ULONG     ulPostCt;

  /****************************************************************************/
  /* Initialize the PM interface.  If it fails, terminate both threads.       */
  /****************************************************************************/
  habAsync = WinInitialize( 0);
  if( !habAsync)
  {
      WinPostMsg( hwndClient, WM_QUIT, NULL, NULL);
      DosExit( EXIT_THREAD, 0);
  }

  /****************************************************************************/
  /* Create a message queue.  If it fails, terminate both threads.               */
  /****************************************************************************/
  hmqAsync = WinCreateMsgQueue( habAsync, 150);
  if( !hmqAsync)
  {
      WinPostMsg( hwndClient, WM_QUIT, NULL, NULL);
      WinTerminate( habAsync);
      DosExit( EXIT_THREAD, 0);
  }
  else
  {
     WinCancelShutdown( hmqAsync, TRUE);   /* don't wait around for shutdown */
  }

  DosSetPriority( PRTYS_THREAD, PRTYC_NOCHANGE, sPrty, (TID)NULL);


  while( TRUE)
  {
    WinGetMsg( habAsync, &qmsgAsync, NULLHANDLE, 0, 0);

    /*
     *   If we're exiting, give that message highest priority.
     */

    if( WinPeekMsg(habAsync, &qmsgPeek, NULLHANDLE, UM_DIE, UM_DIE, PM_NOREMOVE))
            qmsgAsync = qmsgPeek;
    /*
     *   Disallow drawing and mouse tracking as needed
     */

    if( (qmsgAsync.msg < UM_SIZING) || (qmsgAsync.msg > UM_LOAD))
            DosPostEventSem( hevMouse);             /* enable mouse tracking */
    else
            DosResetEventSem( hevMouse, &ulPostCt); /* disable mouse tracking */

    if( WinPeekMsg( habAsync, &qmsgPeek, NULLHANDLE, UM_SIZING, UM_LOAD, PM_NOREMOVE))
    {
       DosResetEventSem( hevDrawOn, &ulPostCt);           /* disable drawing */
       DosResetEventSem( hevMouse, &ulPostCt);     /* disable mouse tracking */
    }
    else
    {
       DosPostEventSem( hevDrawOn);                        /* enable drawing */
       DosPostEventSem( hevMouse);                  /* enable mouse tracking */
    }

    /**************************************************************************/
    /* process the commands                                                   */
    /**************************************************************************/

    switch( qmsgAsync.msg)
    {
      /************************************************************************/
      case UM_CHAR:
            fsKeyFlags = (USHORT)SHORT1FROMMP(qmsgAsync.mp1);
            usChar         = (USHORT)SHORT1FROMMP(qmsgAsync.mp2);
            if(   (fsKeyFlags & KC_CHAR)
               && ((usChar == 'b') || (usChar == 'B')))
            {
              if( psl = Correlate( &ptlMouse))
              {
                ToBottom( psl);
                Redraw();
              }
            }
            break;

      /************************************************************************/
      case UM_LOAD:
              Load( (PLOADINFO)qmsgAsync.mp1);
              Redraw();
        break;

      /************************************************************************/
      case UM_JUMBLE:
        Jumble();
        Redraw();
        break;

      /************************************************************************/
      case UM_REDRAW:
        Redraw();
        break;

      /************************************************************************/
      /* DRAW will use the passed region containing the invalidated area of   */
      /* the screen, repaint it and then destroy the region.                  */
      /************************************************************************/
      case UM_DRAW:

        if (!BmpLoaded)
             break;
        if( qmsgAsync.mp1)
        {
          DoDraw( hpsBitmapBuff, (HRGN)qmsgAsync.mp1, TRUE);
          if (GpiQueryRegionBox( hpsClient, (HRGN)qmsgAsync.mp1, (PRECTL)aptlDraw) == RGN_ERROR)
                 DispError(habMain,hwndClient);
          if (GpiDestroyRegion( hpsClient, (HRGN)qmsgAsync.mp1) == GPI_ERROR)
                 DispError(habMain,hwndClient);
          WinMapWindowPoints( hwndClient, HWND_DESKTOP, aptlDraw, 3);
          ROUND_DOWN_MOD( aptlDraw[0].x, lByteAlignX);    /* round down       */
          ROUND_DOWN_MOD( aptlDraw[0].y, lByteAlignY);    /* round down       */
          ROUND_UP_MOD(   aptlDraw[1].x, lByteAlignX);    /* round up         */
          ROUND_UP_MOD(   aptlDraw[1].y, lByteAlignY);    /* round up         */
          WinMapWindowPoints( HWND_DESKTOP, hwndClient, aptlDraw, 3);
          aptlDraw[2] = aptlDraw[0];
          if (GpiBitBlt( hpsClient
                       , hpsBitmapBuff
                       , 3L
                       , aptlDraw
                       , ROP_SRCCOPY
                       , BBO_IGNORE ) == GPI_ERROR)
              DispError(habMain,hwndClient);
        }
        break;


      /************************************************************************/
      /* Get new scroll posn from command ( i.e. +/-1 +/-page) or new         */
      /* absolute position from parameter, update scroll posn, change the     */
      /* transform and update the thumb posn.  Finally update the window.     */
      /************************************************************************/
      case UM_HSCROLL:
        switch( SHORT2FROMMP( qmsgAsync.mp1) )
        {
            case SB_LINEUP:
                ptsScrollPos.x -= ptsScrollLine.x;
                break;
            case SB_LINEDOWN:
                ptsScrollPos.x += ptsScrollLine.x;
                break;
            case SB_SLIDERTRACK:
            case SB_SLIDERPOSITION:
                for( fDone = FALSE; !fDone ;)
                {
                  if( WinPeekMsg( habAsync
                                , &qmsgPeek
                                , NULLHANDLE
                                , UM_HSCROLL
                                , UM_HSCROLL
                                , PM_NOREMOVE))
                      if(   (SHORT2FROMMP( qmsgPeek.mp1) == SB_SLIDERTRACK)
                          ||(SHORT2FROMMP( qmsgPeek.mp1) == SB_SLIDERPOSITION))
                          WinPeekMsg( habAsync
                                    , &qmsgAsync
                                    , NULLHANDLE
                                    , UM_HSCROLL
                                    , UM_HSCROLL
                                    , PM_REMOVE);
                      else
                          fDone = TRUE;
                  else
                      fDone = TRUE;
                }
                ptsScrollPos.x = SHORT1FROMMP( qmsgAsync.mp1);
                ROUND_DOWN_MOD( ptsScrollPos.x, (SHORT)lByteAlignX);
                break;
            case SB_PAGEUP:
                ptsScrollPos.x -= ptsScrollPage.x;
                break;
            case SB_PAGEDOWN:
                ptsScrollPos.x += ptsScrollPage.x;
                break;
            case SB_ENDSCROLL:
                break;
            default:
                break;
        }
        DoHorzScroll();
        break;

      case UM_VSCROLL:
        switch( SHORT2FROMMP( qmsgAsync.mp1) )
        {
            case SB_LINEUP:
                ptsScrollPos.y -= ptsScrollLine.y;
                break;
            case SB_LINEDOWN:
                ptsScrollPos.y += ptsScrollLine.y;
                break;
            case SB_SLIDERTRACK:
            case SB_SLIDERPOSITION:
                for( fDone = FALSE; !fDone ;)
                {
                  if( WinPeekMsg( habAsync
                                , &qmsgPeek
                                , NULLHANDLE
                                , UM_VSCROLL
                                , UM_VSCROLL
                                , PM_NOREMOVE))
                      if(   (SHORT2FROMMP( qmsgPeek.mp1) == SB_SLIDERTRACK)
                          ||(SHORT2FROMMP( qmsgPeek.mp1) == SB_SLIDERPOSITION))
                          WinPeekMsg( habAsync
                                    , &qmsgAsync
                                    , NULLHANDLE
                                    , UM_VSCROLL
                                    , UM_VSCROLL
                                    , PM_REMOVE);
                      else
                          fDone = TRUE;
                  else
                      fDone = TRUE;
                }
                ptsScrollPos.y = SHORT1FROMMP( qmsgAsync.mp1);
                ROUND_DOWN_MOD( ptsScrollPos.y, (SHORT)lByteAlignY);
                break;
            case SB_PAGEUP:
                ptsScrollPos.y -= ptsScrollPage.y;
                break;
            case SB_PAGEDOWN:
                ptsScrollPos.y += ptsScrollPage.y;
                break;
            case SB_ENDSCROLL:
                break;
            default:
                break;
        }
        DoVertScroll();
        break;

      /************************************************************************/
      /* the window is being resized                                          */
      /************************************************************************/
      case UM_SIZING:
        CalcBounds();
        CalcTransform( hwndClient);
        break;

      /************************************************************************/
      /* adjust zoom factor                                                   */
      /************************************************************************/
      case UM_ZOOM:
        if( WinPeekMsg( habAsync
                      , &qmsgPeek
                      , NULLHANDLE
                      , UM_SIZING
                      , UM_LOAD
                      , PM_NOREMOVE))
            DosResetEventSem( hevDrawOn, &ulPostCt);
        else {
            DosPostEventSem( hevDrawOn);
            }

        Zoom();
        break;

      /************************************************************************/
      /* Button down will cause a correlate on the picture to test for a hit. */
      /* Any selected segment will be highlighted and redrawn as dynamic.     */
      /************************************************************************/
      case UM_LEFTDOWN:
        if( !fButtonDownAsync)
        {
            fButtonDownAsync = TRUE;
       if(!fPictureAssembled)
               LeftDown( qmsgAsync.mp1);
        }
        break;

      /************************************************************************/
      /* if a segment is being dragged it will be redrawn in a new posn       */
      /************************************************************************/
      case UM_MOUSEMOVE:
#ifdef fred
        if( !fButtonDownAsync)
            break;
#endif
        for( fDone = FALSE; !fDone ;)
        {
          if( WinPeekMsg( habAsync            /* look through first button-up */
                        , &qmsgPeek
                        , NULLHANDLE
                        , UM_MOUSEMOVE
                        , UM_LEFTUP
                        , PM_NOREMOVE))
              if( qmsgPeek.msg == UM_MOUSEMOVE) /* only collapse move msgs    */
                  WinPeekMsg( habAsync
                            , &qmsgAsync
                            , NULLHANDLE
                            , UM_MOUSEMOVE
                            , UM_MOUSEMOVE
                            , PM_REMOVE);
              else
                  fDone = TRUE;
          else
              fDone = TRUE;
        }
        MouseMove( qmsgAsync.mp1);      /* process last move before button-up */
        break;

      /************************************************************************/
      /* if a segment is being dragged it will be redrawn as normal           */
      /************************************************************************/
      case UM_LEFTUP:
        if( fButtonDownAsync)
        {
            LeftUp();
            fButtonDownAsync = FALSE;
        }
        break;

      /************************************************************************/
      /* destroy resources and terminate                                                           */
      /************************************************************************/
      case UM_DIE:
              WinDestroyMsgQueue( hmqAsync);
              WinTerminate( habAsync);
              DosExit( EXIT_THREAD, 0);
              break;

      /************************************************************************/
      default:
        break;
    }
  }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
VOID CalcSize( mp1, mp2)
MPARAM mp1;
MPARAM mp2;
{
  ptsScrollMax.y = SHORT2FROMMP( mp2);
  ptsHalfScrollMax.y = ptsScrollMax.y >> 1;
  ptsScrollPage.x = ptsScrollMax.x >> 3;
  ROUND_DOWN_MOD( ptsScrollPage.x, (SHORT)lByteAlignX);
  ptsScrollLine.x = ptsScrollMax.x >> 5;
  ROUND_DOWN_MOD( ptsScrollLine.x, (SHORT)lByteAlignX);
  ptsScrollPos.y = (SHORT)(
                           (  (LONG)ptsScrollPos.y
                            * (LONG)SHORT2FROMMP(mp2)
                           )/ (LONG)SHORT2FROMMP(mp1)
                          );
  ptsOldScrollPos.y = (SHORT)(
                              (  (LONG)ptsOldScrollPos.y
                               * (LONG)SHORT2FROMMP(mp2)
                              )/ (LONG)SHORT2FROMMP(mp1)
                             );
  WinSendMsg( hwndVertScroll
            , SBM_SETSCROLLBAR
            , MPFROMSHORT( ptsScrollPos.y)
            , MPFROM2SHORT( 1, ptsScrollMax.y) );

  ptsScrollMax.x = SHORT1FROMMP( mp2);
  ptsHalfScrollMax.x = ptsScrollMax.x >> 1;
  ptsScrollPage.y = ptsScrollMax.y >> 3;
  ROUND_DOWN_MOD( ptsScrollPage.y, (SHORT)lByteAlignY);
  ptsScrollLine.y = ptsScrollMax.y >> 5;
  ROUND_DOWN_MOD( ptsScrollLine.y, (SHORT)lByteAlignY);
  ptsScrollPos.x = (SHORT)(
                           (  (LONG)ptsScrollPos.x
                            * (LONG)SHORT1FROMMP(mp2)
                           )/(LONG)SHORT1FROMMP(mp1)
                          );
  ptsOldScrollPos.x = (SHORT)(
                              (  (LONG)ptsOldScrollPos.x
                               * (LONG)SHORT1FROMMP(mp2)
                              )/ (LONG)SHORT1FROMMP(mp1)
                             );
  WinSendMsg( hwndHorzScroll
            , SBM_SETSCROLLBAR
            , MPFROMSHORT( ptsScrollPos.x)
            , MPFROM2SHORT( 1, ptsScrollMax.x) );
    WinSendMsg( hwndHorzScroll
              , SBM_SETTHUMBSIZE
              , MPFROM2SHORT(ptsScrollPage.x, ptsScrollMax.x)
              , NULL                             );

    WinSendMsg( hwndVertScroll
              , SBM_SETTHUMBSIZE
              , MPFROM2SHORT(ptsScrollPage.y, ptsScrollMax.y)
              , NULL                             );

}

/******************************************************************************/
/* button down will cause one segment to be indicated and made dynamic        */
/******************************************************************************/
VOID LeftDown(MPARAM mp)
{
  POINTL    ptl;
  HRGN      hrgn, hrgnUpdt, hrgnUpdtDrag;
  RECTL     rcl;
  BOOL      fFirst;
  PSEGLIST  psl;

  ptl.x = (LONG)(SHORT)SHORT1FROMMP( mp);
  ptl.y = (LONG)(SHORT)SHORT2FROMMP( mp);

  /****************************************************************************/
  pslPicked = Correlate( &ptl);
  if( pslPicked)
    lPickedSeg   = pslPicked->lSegId;
  else
  {
    fButtonDownAsync = FALSE;
    return;
  }
  if( (lPickedSeg < 1) || (lPickedSeg > lLastSegId) )
  {
     #if defined(DEBUG)
     {
       CHAR      szMsg[40];

        sprintf( szMsg, "Segment id out of range: %x", lPickedSeg);
        WinMessageBox(HWND_DESKTOP, hwndFrame, szMsg, szErrorTitle, 0,
                       MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
     }
     #endif

     fButtonDownAsync = FALSE;
     return;
  }

  /****************************************************************************/
  ptlOffStart = pslPicked->ptlModelXlate;
  ptlMoveStart = ptl;
  if (GpiConvert( hpsClient, CVTC_DEVICE, CVTC_MODEL, 1L, &ptlMoveStart) == GPI_ERROR)
      DispError(habMain,hwndClient);
  ptlMoveStart.x = (ptlMoveStart.x / 50) * 50;
  ptlMoveStart.y = (ptlMoveStart.y / 50) * 50;
  ptlUpdtRef = ptlMoveStart;
  if (GpiConvert( hpsClient, CVTC_MODEL, CVTC_DEVICE, 1L, &ptlUpdtRef) == GPI_ERROR)
      DispError(habMain,hwndClient);

  /****************************************************************************/
  hrgnUpdt = GpiCreateRegion( hpsClient, 0L, NULL);
  for( psl = pslPicked, fFirst = TRUE
     ; (psl != pslPicked) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
  {
    rcl = psl->rclCurrent;   /* get model space bounding box of piece         */
    if (GpiConvert( hpsClient, CVTC_MODEL, CVTC_DEVICE, 2L, (PPOINTL)&rcl) == GPI_ERROR)
        DispError(habMain,hwndClient);
    rcl.xRight++;            /* adjust rectangle for conversion to dev space  */
    rcl.yTop++;
    rcl.xRight  += 2;                            /* should not need           */
    rcl.yTop    += 2;                            /* should not need           */
    rcl.xLeft   -= 4;                            /* should not need           */
    rcl.yBottom -= 4;                            /* should not need           */
    hrgn = GpiCreateRegion( hpsClient, 1L, &rcl);
    if (GpiCombineRegion( hpsClient, hrgnUpdt, hrgnUpdt, hrgn, CRGN_OR) == RGN_ERROR)
        DispError(habMain,hwndClient);
    if (GpiDestroyRegion( hpsClient, hrgn) == GPI_ERROR)
        DispError(habMain,hwndClient);
    psl->fVisible = FALSE;
  }

  if (GpiQueryRegionBox( hpsClient, hrgnUpdt, (PRECTL)aptlUpdt) == GPI_ERROR)
      DispError(habMain,hwndClient);
  WinMapWindowPoints( hwndClient, HWND_DESKTOP, aptlUpdt, 3);
  ROUND_DOWN_MOD( aptlUpdt[0].x, lByteAlignX);            /* round down       */
  ROUND_DOWN_MOD( aptlUpdt[0].y, lByteAlignY);            /* round down       */
  ROUND_UP_MOD(   aptlUpdt[1].x, lByteAlignX);            /* round up         */
  ROUND_UP_MOD(   aptlUpdt[1].y, lByteAlignY);            /* round up         */
  WinMapWindowPoints( HWND_DESKTOP, hwndClient, aptlUpdt, 3);
  hrgnUpdtDrag = GpiCreateRegion( hpsBitmapBuff, 1L, (PRECTL)aptlUpdt);

  aptlUpdt[2] = aptlUpdt[0];
  DoDraw( hpsBitmapBuff, hrgnUpdtDrag, TRUE);
  if (GpiDestroyRegion( hpsClient, hrgnUpdt) == GPI_ERROR)
      DispError(habMain,hwndClient);
  if (GpiDestroyRegion( hpsBitmapBuff, hrgnUpdtDrag) == GPI_ERROR)
      DispError(habMain,hwndClient);
  if (GpiBitBlt( hpsBitmapSave
               , hpsBitmapBuff
               , 3L
               , aptlUpdt
               , ROP_SRCCOPY
               , BBO_IGNORE ) == GPI_ERROR)
      DispError(habMain,hwndClient);

  /****************************************************************************/
  for( psl = pslPicked, fFirst = TRUE
     ; (psl != pslPicked) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
  {
    psl->fVisible = TRUE;
    DrawPiece( hpsBitmapBuff, psl, TRUE);
  }
  if (GpiBitBlt( hpsClient
           , hpsBitmapBuff
           , 3L
           , aptlUpdt
           , ROP_SRCCOPY
           , BBO_IGNORE ) ==  GPI_ERROR)
      DispError(habMain,hwndClient);
  WinSetCapture( HWND_DESKTOP, hwndClient);
}




/******************************************************************************/
/*                                                                            */
/* move the segment                                                           */
/*                                                                            */
/******************************************************************************/
VOID MouseMove( mp)
MPARAM mp;
{
  RECTL     rcl;
  POINTL    ptl, ptlModel, ptlDevice;
  POINTL    aptlUpdtRef[3], aptlUpdtNew[3];
  PSEGLIST  psl;
  BOOL      fFirst;

  ptl.x = (LONG)(SHORT)SHORT1FROMMP( mp);
  ptl.y = (LONG)(SHORT)SHORT2FROMMP( mp);

  /****************************************************************************/
  /* clip mouse coords to client window                                       */
  /****************************************************************************/
  WinQueryWindowRect( hwndClient, &rcl);
  if (rcl.xLeft > ptl.x)
    ptl.x = rcl.xLeft;
  if (rcl.xRight <= ptl.x)
    ptl.x = rcl.xRight;
  if (rcl.yBottom > ptl.y)
    ptl.y = rcl.yBottom;
  if (rcl.yTop <= ptl.y)
    ptl.y = rcl.yTop;
  ptlMouse = ptl;

  if( !lPickedSeg || !pslPicked || !fButtonDownAsync)
    return;

  ptlModel = ptl;
  if (GpiConvert( hpsClient, CVTC_DEVICE, CVTC_MODEL, 1L, &ptlModel) == GPI_ERROR)
      DispError(habMain,hwndClient);
  ptlModel.x = 50 * (ptlModel.x / 50);
  ptlModel.y = 50 * (ptlModel.y / 50);
  if( (ptlModel.x == ptlOldMouse.x) && (ptlModel.y == ptlOldMouse.y))
    return;
  ptlOldMouse.x = ptlModel.x;
  ptlOldMouse.y = ptlModel.y;
  ptlDevice = ptlModel;
  if (GpiConvert( hpsClient, CVTC_MODEL, CVTC_DEVICE, 1L, &ptlDevice) == GPI_ERROR)
      DispError(habMain,hwndClient);

  if (GpiBitBlt( hpsBitmapBuff
           , hpsBitmapSave
           , 3L
           , aptlUpdt
           , ROP_SRCCOPY
           , BBO_IGNORE ) == GPI_ERROR)
      DispError(habMain,hwndClient);
  aptlUpdtRef[0] = aptlUpdt[0];
  aptlUpdtRef[1] = aptlUpdt[1];

  aptlUpdt[0].x += ptlDevice.x - ptlUpdtRef.x;
  aptlUpdt[0].y += ptlDevice.y - ptlUpdtRef.y;
  aptlUpdt[1].x += ptlDevice.x - ptlUpdtRef.x;
  aptlUpdt[1].y += ptlDevice.y - ptlUpdtRef.y;
  WinMapWindowPoints( hwndClient, HWND_DESKTOP, aptlUpdt, 3);
  ROUND_DOWN_MOD( aptlUpdt[0].x, lByteAlignX);            /* round down       */
  ROUND_DOWN_MOD( aptlUpdt[0].y, lByteAlignY);            /* round down       */
  ROUND_UP_MOD(   aptlUpdt[1].x, lByteAlignX);            /* round up         */
  ROUND_UP_MOD(   aptlUpdt[1].y, lByteAlignY);            /* round up         */
  WinMapWindowPoints( HWND_DESKTOP, hwndClient, aptlUpdt, 3);
  aptlUpdt[2] = aptlUpdt[0];
  ptlUpdtRef = ptlDevice;
  if (GpiBitBlt( hpsBitmapSave
               , hpsBitmapBuff
               , 3L
               , aptlUpdt
               , ROP_SRCCOPY
               , BBO_IGNORE ) == GPI_ERROR)
      DispError(habMain,hwndClient);


  pslPicked->ptlModelXlate.x = ptlOffStart.x + ptlModel.x - ptlMoveStart.x;
  pslPicked->ptlModelXlate.y = ptlOffStart.y + ptlModel.y - ptlMoveStart.y;

  for( psl = pslPicked, fFirst = TRUE
     ; (psl != pslPicked) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
  {
    psl->ptlModelXlate = pslPicked->ptlModelXlate;
    DrawPiece( hpsBitmapBuff, psl, TRUE);
  }

  WinUnionRect( habMain
              , (PRECTL)aptlUpdtNew
              , (PRECTL)aptlUpdt
              , (PRECTL)aptlUpdtRef);
  WinMapWindowPoints( hwndClient, HWND_DESKTOP, aptlUpdtNew, 2);
  ROUND_DOWN_MOD( aptlUpdtNew[0].x, lByteAlignX);         /* round down       */
  ROUND_DOWN_MOD( aptlUpdtNew[0].y, lByteAlignY);         /* round down       */
  ROUND_UP_MOD(   aptlUpdtNew[1].x, lByteAlignX);         /* round up         */
  ROUND_UP_MOD(   aptlUpdtNew[1].y, lByteAlignY);         /* round up         */
  WinMapWindowPoints( HWND_DESKTOP, hwndClient, aptlUpdtNew, 2);
  aptlUpdtNew[2] = aptlUpdtNew[0];
  if (GpiBitBlt( hpsClient
               , hpsBitmapBuff
               , 3L
               , aptlUpdtNew
               , ROP_SRCCOPY
               , BBO_IGNORE ) == GPI_ERROR)
      DispError(habMain,hwndClient);
}


/******************************************************************************/
/*                                                                                                                         */
/* The dragged segment is being unselected.  Return it to its normal state.   */
/*                                                                                                                         */
/******************************************************************************/
VOID LeftUp(VOID)
{
  PSEGLIST   psl, pslTemp;
  POINTL     ptlShift;
  BOOL       fFirst;
  LONG       l;

  if (!BmpLoaded)
    {
           MessageBox(hwndFrame, IDS_LOADBITMAP, MB_OK | MB_INFORMATION, TRUE);
      return;
    }
  else if(fPictureAssembled)
    {
           MessageBox(hwndFrame, IDS_OPTIONJUMBLE, MB_OK | MB_INFORMATION, TRUE);
      return;
    }

  if( !lPickedSeg || !pslPicked)
    return;

  for( psl = pslPicked, fFirst = TRUE
     ; (psl != pslPicked) || fFirst
     ; psl = psl->pslNextIsland, fFirst = FALSE )
  {

    SetRect( psl);
    SegListUpdate( MAKE_TAIL_SEG, psl);       /* at tail => highest priority  */
    psl->fIslandMark = TRUE;                  /* mark as island member        */
  }
  ptlShift = pslPicked->ptlModelXlate;

  for( psl = pslHead; psl != NULL; psl = psl->pslNext)
    if( !psl->fIslandMark)
      for( l = 0; l < 8; l++)
        if( pslPicked->lAdjacent[l] == psl->lSegId)
          if(   (ptlShift.x == psl->ptlModelXlate.x)
             && (ptlShift.y == psl->ptlModelXlate.y))
          {
            DosBeep( 600, 100);
            DosBeep( 1200, 50);
            MarkIsland( psl, TRUE);           /* mark the whole new island    */
            pslTemp = psl->pslNextIsland;     /* swap island ptrs             */
            psl->pslNextIsland = pslPicked->pslNextIsland;
            pslPicked->pslNextIsland = pslTemp;
          }
  MarkIsland( pslPicked, FALSE);              /* unmark the island            */

  pslPicked = NULL;
  lPickedSeg = 0;

  WinSetCapture( HWND_DESKTOP, (HWND)NULL);
}


/******************************************************************************/
/*                                                                                                                    */
/* DoHorzScroll will horizontally scroll the current contents of                    */
/* the client area and redraw the invalidated area                                           */
/*                                                                                                                         */
/******************************************************************************/
VOID DoHorzScroll(VOID)
{
  POINTL    aptlClient[3];
  HRGN      hrgn;
  MATRIXLF  matlf;

  if( ptsScrollPos.x > ptsScrollMax.x)     /* clip to range of scroll param   */
      ptsScrollPos.x = ptsScrollMax.x;
  if( ptsScrollPos.x < 0)
      ptsScrollPos.x = 0;

  if( ptsOldScrollPos.x != ptsScrollPos.x) /* only process change in position */
      WinSendMsg( hwndHorzScroll
                , SBM_SETPOS
                , MPFROM2SHORT( ptsScrollPos.x, 0)
                , MPFROMLONG( NULL));

  /****************************************************************************/
  /* Scroll the window the reqd amount, using bitblt'ing (ScrollWindow)       */
  /* if any of the screen still in view, and paint into uncovered region;     */
  /* else repaint the whole client area.                                      */
  /****************************************************************************/
  hrgn = GpiCreateRegion( hpsClient, 0L, NULL);
  WinQueryWindowRect( hwndClient, (PRECTL)aptlClient);
  if( abs( ptsScrollPos.x - ptsOldScrollPos.x) <= ptsScrollMax.x)
  {
      WinScrollWindow( hwndClient
                     , (SHORT) (ptsOldScrollPos.x - ptsScrollPos.x)
                     , (SHORT) 0
                     , (PRECTL) NULL
                     , (PRECTL) NULL
                     , (HRGN) hrgn
                     , (PRECTL) NULL
                     , 0);
  } else
  {
      if (GpiSetRegion( hpsClient, hrgn, 1L, (PRECTL)aptlClient) == GPI_ERROR)
          DispError(habMain,hwndClient);
  }
  /****************************************************************************/
  /* adjust the default view matrix                                           */
  /****************************************************************************/
  if (GpiQueryDefaultViewMatrix( hpsClient, 9L, &matlf ) == GPI_ERROR)
      DispError(habMain,hwndClient);
  matlf.lM31 -= ptsScrollPos.x - ptsOldScrollPos.x;
  if (GpiSetDefaultViewMatrix( hpsClient, 9L, &matlf, TRANSFORM_REPLACE) == GPI_ERROR)
      DispError(habMain,hwndClient);

  DoDraw( hpsClient, hrgn, TRUE);     /* paint into the client area           */
  ptsOldScrollPos.x = ptsScrollPos.x;
  if (GpiDestroyRegion( hpsClient, hrgn) == GPI_ERROR)
      DispError(habMain,hwndClient);

  aptlClient[2] = aptlClient[0];
  if (!fFirstLoad)                  /* don't bother if no bitmap was read in */
  {
     if (GpiBitBlt( hpsBitmapBuff      /* update the off-screen client image */
                  , hpsClient
                  , 3L
                  , aptlClient
                  , ROP_SRCCOPY
                  , BBO_IGNORE ) == GPI_ERROR)
         DispError(habMain,hwndClient);
  }   /* end if () */
}

/******************************************************************************/
/*                                                                            */
/* DoVertScroll will vertically scroll the current contents of                */
/* the client area and redraw the invalidated area                            */
/*                                                                            */
/******************************************************************************/
VOID DoVertScroll(VOID)
{
  POINTL    aptlClient[3];
  HRGN      hrgn;
  MATRIXLF  matlf;

  if( ptsScrollPos.y > ptsScrollMax.y)
      ptsScrollPos.y = ptsScrollMax.y;
  if( ptsScrollPos.y < 0)
      ptsScrollPos.y = 0;

  if( ptsOldScrollPos.y != ptsScrollPos.y)
      WinSendMsg( hwndVertScroll
                , SBM_SETPOS
                , MPFROM2SHORT( ptsScrollPos.y, 0)
                , MPFROMLONG( NULL));

  /****************************************************************************/
  /* Scroll the window the reqd amount, using bitblt'ing (ScrollWindow)       */
  /* if any of the screen still in view, and paint into uncovered region;     */
  /* else repaint the whole client area.                                      */
  /****************************************************************************/
  hrgn = GpiCreateRegion( hpsClient, 0L, NULL);
  WinQueryWindowRect( hwndClient, (PRECTL)aptlClient);
  if( abs( ptsScrollPos.y - ptsOldScrollPos.y) <= ptsScrollMax.y)
  {
      WinScrollWindow( hwndClient
                     , 0
                     , (SHORT) (ptsScrollPos.y - ptsOldScrollPos.y)
                     , (PRECTL) NULL
                     , (PRECTL) NULL
                     , (HRGN) hrgn
                     , (PRECTL) NULL
                     , 0);
  } else
  {
      if (GpiSetRegion( hpsClient, hrgn, 1L, (PRECTL)aptlClient) == GPI_ERROR)
          DispError(habMain,hwndClient);
  }
  if (GpiQueryDefaultViewMatrix( hpsClient, 9L, &matlf ) == GPI_ERROR)
      DispError(habMain,hwndClient);
  matlf.lM32 += ptsScrollPos.y - ptsOldScrollPos.y;
  if (GpiSetDefaultViewMatrix( hpsClient, 9L, &matlf, TRANSFORM_REPLACE) == GPI_ERROR)
      DispError(habMain,hwndClient);

  DoDraw( hpsClient, hrgn, TRUE);
  ptsOldScrollPos.y = ptsScrollPos.y;
  if (GpiDestroyRegion( hpsClient, hrgn) == GPI_ERROR)
      DispError(habMain,hwndClient);

  aptlClient[2] = aptlClient[0];
  if (!fFirstLoad)                  /* don't bother if no bitmap was read in */
  {
     if (GpiBitBlt( hpsBitmapBuff
                  , hpsClient
                  , 3L
                  , aptlClient
                  , ROP_SRCCOPY
                  , BBO_IGNORE ) == GPI_ERROR)
         DispError(habMain,hwndClient);
  }
}

/******************************************************************************
*
*  Name        : EnableMenu
*
*  Description : Enable/Disable bitmap size submenus
*
*  Parameters  : ULONG id        - menu id (ASSUME: id = submenu of FID_MAIN
*                BOOL  fEnable   - enable/disable?
*
*  Return      : VOID
*
******************************************************************************/

VOID EnableMenu(ULONG id, BOOL fEnable)
{
    WinSendMsg(hwndMenu,                          /* global main menu handle */
       MM_SETITEMATTR,                                 /* set menu attribute */
       MPFROM2SHORT(id, TRUE),                                    /* menu id */
       MPFROM2SHORT(MIA_DISABLED,                      /* mask = disable bit */
                    fEnable ? ~MIA_DISABLED : MIA_DISABLED)); /* turn off/on */

}   /* end EnableMenu() */

/******************************************************************************
*
*  Name        : CheckMenu
*
*  Description : Check/Uncheck bitmap size submenus
*
*  Parameters  : ULONG id       - menu id  (ASSUME: id = submenu of FID_MAIN
*                BOOL  fCheck   - check/unckeck?
*
*  Return      : VOID
*
******************************************************************************/

VOID CheckMenu(ULONG id, BOOL fCheck)
{
    WinSendMsg(hwndMenu,                          /* global main menu handle */
       MM_SETITEMATTR,                                 /* set menu attribute */
       MPFROM2SHORT(id, TRUE),                                    /* menu id */
       MPFROM2SHORT(MIA_CHECKED,                         /* mask = check bit */
                    fCheck ? MIA_CHECKED : ~MIA_CHECKED));    /* turn on/off */

}   /* end CheckMenu() */


/******************************************************************************/
/*                                                                                                                         */
/* adjust zoom factor and recalc the picture transform, then do a redraw of   */
/* whole screen                                                                                                   */
/*                                                                                                               */
/******************************************************************************/
VOID Zoom( VOID )
{
  ULONG  ulPostKillDraw, ulPostCt;

  DosQueryEventSem(hevKillDraw, &ulPostKillDraw);

  CalcBounds();
  DosQueryEventSem(hevKillDraw, &ulPostCt);
  if (ulPostKillDraw != ulPostCt)
  {
     DosResetEventSem(hevKillDraw, &ulPostCt);
     return;
  }
  CalcTransform( hwndClient);
  DosQueryEventSem(hevKillDraw, &ulPostCt);
  if (ulPostKillDraw != ulPostCt)
  {
     DosResetEventSem(hevKillDraw, &ulPostCt);
     return;
  }
  Redraw();
}

/******************************************************************************/
/*                                                                            */
/* Check the segment list for obvious errors.                                 */
/*                                                                            */
/******************************************************************************/
BOOL SegListCheck(INT iLoc)
{
  PSEGLIST   psl;
  CHAR       szMsg[50];

  szMsg[0] = '\0';
  for( psl = pslHead; psl != NULL; psl = psl->pslNext)
    if( (psl->lSegId < 1) || (psl->lSegId > lLastSegId) )
    {
      #if defined(DEBUG)

         sprintf( szMsg, "Bad head segment list, location %d", iLoc);
         WinMessageBox(HWND_DESKTOP, hwndFrame, szMsg, szErrorTitle, 0,
                        MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
      #endif

      return( FALSE);
    }
  for( psl = pslTail; psl != NULL; psl = psl->pslPrev)
    if( (psl->lSegId < 1) || (psl->lSegId > lLastSegId) )
    {
      #if defined(DEBUG)

         sprintf( szMsg, "Bad head segment list, location %d", iLoc);
         WinMessageBox(HWND_DESKTOP, hwndFrame, szMsg, szErrorTitle, 0,
                        MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
      #endif

      return( FALSE);
    }
  return( TRUE);
}

/******************************************************************************/
/*                                                                            */
/* DumpPicture will free the list and segment store for the picture           */
/*                                                                            */
/******************************************************************************/
BOOL DumpPicture(VOID)
{
  while( pslHead != NULL )
  {
    if (GpiSetBitmap( pslHead->hpsFill, NULLHANDLE) == (HBITMAP)BMB_ERROR)
        DispError(habMain,hwndClient);
    if (GpiDeleteBitmap( pslHead->hbmFill) == GPI_ERROR)
        DispError(habMain,hwndClient);
    if (GpiDestroyPS( pslHead->hpsFill) == GPI_ERROR)
        DispError(habMain,hwndClient);
    DevCloseDC( pslHead->hdcFill);

    if (GpiSetBitmap( pslHead->hpsHole, NULLHANDLE) == (HBITMAP)BMB_ERROR)
        DispError(habMain,hwndClient);
    if (GpiDeleteBitmap( pslHead->hbmHole) == GPI_ERROR)
        DispError(habMain,hwndClient);
    if (GpiDestroyPS( pslHead->hpsHole) == GPI_ERROR)
        DispError(habMain,hwndClient);
    DevCloseDC( pslHead->hdcHole);

    SegListUpdate( DEL_SEG, pslHead);
  }

  if( hbmBitmapSize)
  {
      if (GpiSetBitmap( hpsBitmapSize, NULLHANDLE) == (HBITMAP)BMB_ERROR)
          DispError(habMain,hwndClient);
      if (GpiDeleteBitmap( hbmBitmapSize) == GPI_ERROR)
          DispError(habMain,hwndClient);
  }
  if( hbmBitmapBuff)
  {
      if (GpiSetBitmap( hpsBitmapBuff, NULLHANDLE) == (HBITMAP)BMB_ERROR)
          DispError(habMain,hwndClient);
      if (GpiDeleteBitmap( hbmBitmapBuff) == GPI_ERROR)
          DispError(habMain,hwndClient);
  }
  if( hbmBitmapSave)
  {
      if (GpiSetBitmap( hpsBitmapSave, NULLHANDLE) == (HBITMAP)BMB_ERROR)
          DispError(habMain,hwndClient);
      if (GpiDeleteBitmap( hbmBitmapSave) == GPI_ERROR)
          DispError(habMain,hwndClient);
  }

  return( TRUE);
}

/******************************************************************************/
/*                                                                            */
/* Draw the picture into segment store.                                       */
/*                                                                            */
/******************************************************************************/
BOOL CreatePicture(SHORT sUpdate)
{
  POINTL             ptl, aptlSides[12], aptlControl[12];
  SEGLIST            sl;
  PSEGLIST           psl;
  LONG               l, lMinor, lNeighbor, alFuzz[36][4];
  SIZEL              sizl;
  BITMAPINFOHEADER2  bmp2;
  PBITMAPINFOHEADER2 pbmp2 = &bmp2;
  DATETIME           date;
  ULONG              ulPostCt;

  /****************************************************************************/
  /* compute some fuzz for the control points                                 */
  /****************************************************************************/
  DosGetDateTime( &date);
  srand( (USHORT)date.hundredths);
  for( l = 0; l < 36; l++)
    for( lMinor = 0; lMinor < 4; lMinor++)
      alFuzz[l][lMinor] = 50 * (rand() % 10);

  /****************************************************************************/
  /* reset the default viewing transform to identity                          */
  /****************************************************************************/
  SetDVTransform( (FIXED)UNITY
                , (FIXED)0
                , (FIXED)0
                , (FIXED)UNITY
                , 0L
                , 0L
                , TRANSFORM_REPLACE);

  /****************************************************************************/
  /* draw the pieces                                                          */
  /****************************************************************************/
  lLastSegId = 0;
  for( ptl.x = ptlBotLeft.x; ptl.x < ptlTopRight.x; ptl.x += 500)
  {
    DosQueryEventSem( hevTerminate, &ulPostCt);
    if( ulPostCt)
      break;
    for( ptl.y = ptlBotLeft.y; ptl.y < ptlTopRight.y; ptl.y += 500)
    {
      DosQueryEventSem( hevTerminate, &ulPostCt);
      if( ulPostCt)
              break;
      lLastSegId++;

      /************************************************************************/
      /* compute the piece outline control points                             */
      /************************************************************************/
      aptlControl[0].x = 250L;
      aptlControl[0].y = 500L;
      aptlControl[1].x = 250;
      aptlControl[1].y = -500L;
      aptlControl[2].x = 500L;
      aptlControl[2].y = 0L;

      aptlControl[3].x = 0L;
      aptlControl[3].y = 250L;
      aptlControl[4].x = 1000L;
      aptlControl[4].y = 250L;
      aptlControl[5].x = 500L;
      aptlControl[5].y = 500L;

      aptlControl[6].x = 250L;
      aptlControl[6].y = 0L;
      aptlControl[7].x = 250L;
      aptlControl[7].y = 1000L;
      aptlControl[8].x = 0L;
      aptlControl[8].y = 500L;

      aptlControl[9].x  = 500L;
      aptlControl[9].y  = 250L;
      aptlControl[10].x = -500L;
      aptlControl[10].y = 250L;
      aptlControl[11].x = 0L;
      aptlControl[11].y = 0L;

      if( ptl.y == ptlBotLeft.y)
      {
              aptlControl[0].y = 0L;
              aptlControl[1].y = 0L;
      }

      if( (ptl.x + 500) == ptlTopRight.x)
      {
        aptlControl[3].x = 500L;
              aptlControl[4].x = 500L;
      }

      if( (ptl.y + 500) == ptlTopRight.y)
      {
              aptlControl[6].y = 500L;
        aptlControl[7].y = 500L;
      }

      if( ptl.x == ptlBotLeft.x)
      {
              aptlControl[ 9].x = 0L;
              aptlControl[10].x = 0L;
      }

      /************************************************************************/
      /* compute the adjacent segments                                        */
      /************************************************************************/
      sl.lAdjacent[0] = lLastSegId - 7;
      sl.lAdjacent[1] = lLastSegId - 6;
      sl.lAdjacent[2] = lLastSegId - 5;
      sl.lAdjacent[3] = lLastSegId - 1;
      sl.lAdjacent[4] = lLastSegId + 1;
      sl.lAdjacent[5] = lLastSegId + 5;
      sl.lAdjacent[6] = lLastSegId + 6;
      sl.lAdjacent[7] = lLastSegId + 7;
      if( ptl.x == ptlBotLeft.x)
      {
              sl.lAdjacent[0] = 0;
              sl.lAdjacent[1] = 0;
              sl.lAdjacent[2] = 0;
      }
      if( ptl.y == ptlBotLeft.y)
      {
              sl.lAdjacent[0] = 0;
              sl.lAdjacent[3] = 0;
              sl.lAdjacent[5] = 0;
      }
      if( (ptl.x + 500) == ptlTopRight.x)
      {
              sl.lAdjacent[5] = 0;
              sl.lAdjacent[6] = 0;
              sl.lAdjacent[7] = 0;
      }
      if( (ptl.y + 500) == ptlTopRight.y)
      {
              sl.lAdjacent[2] = 0;
              sl.lAdjacent[4] = 0;
              sl.lAdjacent[7] = 0;
      }

      /************************************************************************/
      /* throw in some fuzz                                                   */
      /************************************************************************/
      if( sl.lAdjacent[3])
      {
              aptlControl[0].y  -= alFuzz[lLastSegId - 1][0];
              aptlControl[1].y  += alFuzz[lLastSegId - 1][1];
      }

      if( sl.lAdjacent[1])
      {
        aptlControl[9].x  -= alFuzz[lLastSegId - 1][2];
              aptlControl[10].x += alFuzz[lLastSegId - 1][3];
      }

      if( lNeighbor = sl.lAdjacent[4])
      {
        aptlControl[7].y  -= alFuzz[lNeighbor - 1][0];
              aptlControl[6].y  += alFuzz[lNeighbor - 1][1];
      }

      if( lNeighbor = sl.lAdjacent[6])
      {
              aptlControl[4].x  -= alFuzz[lNeighbor - 1][2];
              aptlControl[3].x  += alFuzz[lNeighbor - 1][3];
      }

      /************************************************************************/
      /* compute the piece control points in world coordinates                */
      /************************************************************************/
      for( l=0; l<12; l++)
      {
              aptlSides[l].x = ptl.x + aptlControl[l].x;
              aptlSides[l].y = ptl.y + aptlControl[l].y;
              sl.aptlSides[l] = aptlSides[l];
      }

      /************************************************************************/
      /* compute the dimensions of the matching rects for BitBlt              */
      /************************************************************************/
      sl.rclBitBlt.xLeft   = ptl.x - 250;
      sl.rclBitBlt.yBottom = ptl.y - 250;
      sl.rclBitBlt.xRight  = ptl.x + 750;
      sl.rclBitBlt.yTop    = ptl.y + 750;
      if( ptl.x == ptlBotLeft.x)
              sl.rclBitBlt.xLeft += 250;
      if( ptl.y == ptlBotLeft.y)
              sl.rclBitBlt.yBottom += 250;
      if( (ptl.x + 500) == ptlTopRight.x)
        sl.rclBitBlt.xRight -= 250;
      if( (ptl.y + 500) == ptlTopRight.y)
              sl.rclBitBlt.yTop -= 250;

      /************************************************************************/
      /* store the piece location                                             */
      /************************************************************************/
      sl.ptlLocation = ptl;

      /************************************************************************/
      /* create the masks                                                     */
      /************************************************************************/
      if( sUpdate == PICTURE_CREATE)
      {
              bmp2      = bmp2BitmapFile;
      if (bmp2.cbFix  == sizeof(BITMAPINFOHEADER))
         {
              sizl.cx = 2 +  ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cx
                       * (sl.rclBitBlt.xRight - sl.rclBitBlt.xLeft)
                       / (ptlTopRight.x - ptlBotLeft.x);
              sizl.cy = 2 +  ((PBITMAPINFOHEADER)pbmp2BitmapFile)->cy
                       * (sl.rclBitBlt.yTop - sl.rclBitBlt.yBottom)
                       / (ptlTopRight.y - ptlBotLeft.y);
            ((PBITMAPINFOHEADER)pbmp2)->cx = LOUSHORT( sizl.cx);
            ((PBITMAPINFOHEADER)pbmp2)->cy = LOUSHORT( sizl.cy);
         }
      else
         {
              sizl.cx = 2 + ((pbmp2BitmapFile->cx
                       * (sl.rclBitBlt.xRight - sl.rclBitBlt.xLeft))
                       / (ptlTopRight.x - ptlBotLeft.x));
              sizl.cy = 2 + ((pbmp2BitmapFile->cy
                       * (sl.rclBitBlt.yTop - sl.rclBitBlt.yBottom))
                       / (ptlTopRight.y - ptlBotLeft.y));
             (pbmp2)->cx = LOUSHORT( sizl.cx);
             (pbmp2)->cy = LOUSHORT( sizl.cy);
        }

              if ((sl.hdcHole = DevOpenDC( habMain
                              , OD_MEMORY
                              , "*"
                              , 3L
                              , (PDEVOPENDATA)&dop
                              , NULLHANDLE)) == DEV_ERROR)
                 DispError(habMain,hwndClient);
              if ((sl.hpsHole = GpiCreatePS( habMain
                                , sl.hdcHole
                                , &sizl
                                , PU_PELS | GPIA_ASSOC | GPIT_MICRO )) == GPI_ERROR)
                 DispError(habMain,hwndClient);

              if ((sl.hbmHole = GpiCreateBitmap( sl.hpsHole
                                    , pbmp2
                                    , 0L
                                    , NULL
                                    , NULL)) == (HBITMAP)GPI_ERROR)
                 DispError(habMain,hwndClient);


              if ((GpiSetBitmap( sl.hpsHole, sl.hbmHole)) == (HBITMAP)BMB_ERROR)
                 DispError(habMain,hwndClient);


              if ((sl.hdcFill = DevOpenDC( habMain
                              , OD_MEMORY
                              , "*"
                              , 3L
                              , (PDEVOPENDATA)&dop
                              , NULLHANDLE)) == DEV_ERROR)
                 DispError(habMain,hwndClient);
              if ((sl.hpsFill = GpiCreatePS( habMain
                                , sl.hdcFill
                                , &sizl
                                , PU_PELS | GPIA_ASSOC | GPIT_MICRO )) == GPI_ERROR)
                 DispError(habMain,hwndClient);
              if ((sl.hbmFill = GpiCreateBitmap( sl.hpsFill
                                    , pbmp2
                                    , 0L
                                    , NULL
                                    , NULL)) == (HBITMAP)GPI_ERROR)
                 DispError(habMain,hwndClient);

              if ((GpiSetBitmap( sl.hpsFill, sl.hbmFill)) == (HBITMAP)BMB_ERROR)
                 DispError(habMain,hwndClient);
      }


      sl.fVisible        = TRUE;
      sl.lSegId          = lLastSegId;
      sl.fIslandMark     = FALSE;
      sl.ptlModelXlate.x = sl.ptlModelXlate.y = 0L;
      if( sUpdate == PICTURE_CREATE)
      {
              sl.pslNext           = NULL;
              sl.pslPrev           = NULL;
              SetRect( &sl);
              psl = SegListUpdate( ADD_TAIL_SEG, &sl);
       } else
       {
              psl = SegListGet( lLastSegId);
              psl->fIslandMark = FALSE;
              for( l=0; l<12; l++)
                psl->aptlSides[l] = aptlSides[l];
              psl->ptlModelXlate = sl.ptlModelXlate;
              SetRect( psl);
       }
       psl->pslNextIsland = psl;        /* point to self ==> island of one    */
    }
  }
  if( ulPostCt)
      SendCommand( UM_DIE, (MPARAM)NULL, (MPARAM)NULL);

  return( TRUE);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
VOID CheckPsl( psl)
PSEGLIST  psl;
{
  SHORT   s;

  for( s=2; s<12; s+=3)
    if( !WinPtInRect( habAsync, &psl->rclBitBlt, &psl->aptlSides[s]))
      break;
}

/******************************************************************************/
/*                                                                            */
/* Create the Size, Save and Buff bitmaps.                                    */
/*                                                                            */
/******************************************************************************/
BOOL PrepareBitmap(VOID)
{
  hbmBitmapSize    = GpiCreateBitmap( hpsBitmapSize
                                    , pbmp2BitmapFile
                                    , 0L
                                    , NULL
                                    , NULL);
  if( !hbmBitmapSize)
    {
    DispError(habMain,hwndClient);
    return( FALSE);
    }
  if (GpiSetBitmap( hpsBitmapSize, hbmBitmapSize) == (HBITMAP)BMB_ERROR)
     DispError(habMain,hwndClient);

  bmp2BitmapSave    = bmp2BitmapFile;
  if (pbmp2BitmapSave->cbFix  == sizeof(BITMAPINFOHEADER))
      {
      ((PBITMAPINFOHEADER)pbmp2BitmapSave)->cx = LOUSHORT( sizlMaxClient.cx);
      ((PBITMAPINFOHEADER)pbmp2BitmapSave)->cy = LOUSHORT( sizlMaxClient.cy);
      }
  else
      {
      (pbmp2BitmapSave)->cx = LOUSHORT(sizlMaxClient.cx);
      (pbmp2BitmapSave)->cy = LOUSHORT(sizlMaxClient.cy);
      }
  hbmBitmapSave     = GpiCreateBitmap( hpsBitmapSave
                                    , pbmp2BitmapSave
                                    , 0L
                                    , NULL
                                    , NULL);
  if( !hbmBitmapSave)
    {
     DispError(habMain,hwndClient);
     return( FALSE);
    }
  if (GpiSetBitmap( hpsBitmapSave, hbmBitmapSave) == (HBITMAP)BMB_ERROR)
     DispError(habMain,hwndClient);


  hbmBitmapBuff     = GpiCreateBitmap( hpsBitmapBuff
                                    , pbmp2BitmapSave
                                    , 0L
                                    , NULL
                                    , NULL);
  if( !hbmBitmapBuff)
    {
    DispError(habMain,hwndClient);
    return( FALSE);
    }
  if (GpiSetBitmap( hpsBitmapBuff, hbmBitmapBuff) == (HBITMAP)BMB_ERROR)
    DispError(habMain,hwndClient);

  return( TRUE);
}

/******************************************************************************
*
*  Name        : ReadBitmap
*
*  Description :
*
* Get the bitmap from disk.
* Note that there are 2 formats for bitmap files, one of which is archaic.
* Both formats are supported here.  All new bitmaps should follow the format
* in BITMAPFILEHEADER.
*
*  Concepts    :
*
*  API's       :
*
*  Parameters  : HFILE hfile
*
*  Return      : BOOL
*
*  Notes:
*
*   File Layout - size and layout of following map are dependent on the
*   type of bitmap.  The cbFix field of the structures is used to determine
*   the size of the structures which in turn identifies them.
*
*
*           SINGLE BITMAP FILE FORMAT
*
*   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ offset 0
*   ³  Bitmap File Header             (bfh2)  ³ pbfh2->offBits contains data Ä¿
*   ³                                         ³ offset (from begin of file)   ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ³
*   ³  Bitmap Information Header      (bmp2)  ³                               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ³
*   ³  Color Table of RGB Structures  (argb2) ³                               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ <ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*   ³  Bitmap Data (scan lines)               ³
*   ³             .                           ³
*   ³             .                           ³
*   ³             .                           ³
*   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*
*
*         BITMAP-ARRAY FILE FORMAT
*
*   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ offset 0
*   ³  Bitmap Array File Header       (baf2)  ³
*   ³  (only for bitmap arrays)               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
*   ³  Bitmap File Header             (bfh2)  ³ pbfh2->offBits contains data Ä¿
*   ³                                         ³ offset (from begin of file)   ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ³
*   ³  Bitmap Information Header      (bmp2)  ³                               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ³
*   ³  Color Table of RGB Structures  (argb2) ³                               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ³
*   ³     next Bitmap Array File Header       ³                               ³
*   ³             .                           ³                               ³
*   ³             .                           ³                               ³
*   ³             .                           ³                               ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ <ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*   ³  1st Bitmap Data (scan lines)           ³
*   ³                                         ³
*   ³                                         ³
*   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
*   ³     next Bitmap Data (scan lines)       ³
*   ³             .                           ³
*   ³             .                           ³
*   ³             .                           ³
*   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*
******************************************************************************/

BOOL ReadBitmap(HFILE hfile)
{
   APIRET     rc;                                         /* API return code */
   BOOL       fRet = FALSE;                         /* Function return code. */
   FILESTATUS fsts;
   PBITMAPFILEHEADER2 pbfh2;                   /* can address any file types */
   PBITMAPINFOHEADER2 pbmp2;                     /* address any info headers */
   PBYTE  pFileBegin = NULL;                /* beginning of bitmap file data */
   ULONG  cbRead;                        /* Number of bytes read by DosRead. */
   ULONG  cScans, cScansRet;          /* number of scan lines in bitmap (cy) */

   /*
    *   Use Loop to avoid duplicate cleanup code.  If any errors, a break
    *   statement will jump directly to error cleanup code.
    */

   do
   {
      /*
       *   Find out how big the file is, allocate that much memory, and read
       *   in the entire bitmap.
       */

      rc =
      DosQueryFileInfo(hfile, 1, &fsts, sizeof(fsts));
      if (rc)
         break;                        /* jump to error code outside of loop */

      rc =
      DosAllocMem(
         (PPVOID) &pFileBegin,
         (ULONG)  fsts.cbFile,
         (ULONG)  PAG_READ | PAG_WRITE | PAG_COMMIT);

      if (rc)
         break;                        /* jump to error code outside of loop */

      if (DosRead( hfile, (PVOID)pFileBegin, fsts.cbFile, &cbRead))
         break;                        /* jump to error code outside of loop */

      /*
       *   If it's a bitmap-array, point to common file header.  Otherwise,
       *   point to beginning of file.
       */

      pbfh2 = (PBITMAPFILEHEADER2) pFileBegin;
      pbmp2 = NULL;                   /* only set this when we validate type */

      switch (pbfh2->usType)
      {
         case BFT_BITMAPARRAY:

            /*
             *   If this is a Bitmap-Array, adjust pointer to the normal
             *   file header.  We'll just use the first bitmap in the
             *   array and ignore other device forms.
             */

            pbfh2 = &(((PBITMAPARRAYFILEHEADER2) pFileBegin)->bfh2);
            pbmp2 = &pbfh2->bmp2;    /* pointer to info header (readability) */
            break;

         case BFT_BMAP:

            pbmp2 = &pbfh2->bmp2;    /* pointer to info header (readability) */
            break;

         default:      /* these formats aren't supported; don't set any ptrs */
         case BFT_ICON:
         case BFT_POINTER:
         case BFT_COLORICON:
         case BFT_COLORPOINTER:

            break;

      }   /* end switch (pbfh2->usType) */

      if (pbmp2 == NULL)
         break;        /* File format NOT SUPPORTED: break out to error code */

      /*
       *   Check to see if BMP file has an old structure, a new structure, or
       *   Windows structure.  Capture the common data and treat all bitmaps
       *   generically with pointer to new format.  API's will determine format
       *   using cbFixed field.
       *
       *   Windows bitmaps have the new format, but with less data fields
       *   than PM.  The old strucuture has some different size fields,
       *   though the field names are the same.
       *
       *
       *   NOTE: bitmap data is located by offsetting the beginning of the file
       *         by the offset contained in pbfh2->offBits.  This value is in
       *         the same relatie location for different format bitmap files.
       */

      if (pbmp2->cbFix == sizeof(BITMAPINFOHEADER))           /* old format? */
      {
         cScans = (ULONG) ((PBITMAPINFOHEADER)pbmp2)->cy;
      }
      else                               /* new PM format, Windows, or other */
      {
         cScans = pbmp2->cy;
      }

      memcpy(                      /* copy bitmap info into global structure */
         (PVOID) pbmp2BitmapFile,
         (PVOID) pbmp2,
         pbmp2->cbFix);      /* only copy specified size (varies per format) */

      hbmBitmapFile =
      GpiCreateBitmap(
         hpsBitmapFile,                         /* presentation-space handle */
         pbmp2BitmapFile,            /* address of structure for format data */
         0L,                                                      /* options */
         NULL,                            /* address of buffer of image data */
         NULL);                 /* address of structure for color and format */

      if (!hbmBitmapFile)
      {
         DispError(habMain,hwndClient);
         break;                        /* jump to error code outside of loop */
      }

      if (GpiSetBitmap( hpsBitmapFile, hbmBitmapFile) == (HBITMAP)BMB_ERROR)
      {
         DispError(habMain,hwndClient);
         break;                        /* jump to error code outside of loop */
      }

      /*
       *   Tell GPI to put the bits into the thread's PS. The function returns
       *   the number of scan lines of the bitmap that were copied.  We want
       *   all of them at once.
       */

      cScansRet =
      GpiSetBitmapBits(
         hpsBitmapFile,                         /* presentation-space handle */
         0L,                                     /* index of first scan line */
         cScans,                                     /* number of scan lines */
         pFileBegin + pbfh2->offBits,              /* address of bitmap data */
         (PBITMAPINFO2) pbmp2);            /* address of bitmap header table */

      if (cScansRet != cScans)                       /* original # of scans? */
      {
         DispError(habMain,hwndClient);
         break;                        /* jump to error code outside of loop */
      }

      DosFreeMem( pFileBegin);
      DosClose( hfile);
      return TRUE;                                    /* function successful */

   } while (FALSE); /* fall through loop first time */

    /*
     *   Close the file, free the buffer space and leave.  This is an error exit
     *   point from the function.  Cleanup code is here to avoid duplicate code
     *   after each operation.
     */

    if (pFileBegin != NULL)
       DosFreeMem( pFileBegin);
    DosClose( hfile);

    return FALSE;                                         /* function failed */

}   /* end ReadBitmap() */


/**************************************************************************/
/* DispError -- report an error returned from an API service.             */
/*                                                                        */
/* The error message is displayed using a message box                     */
/*                                                                        */
/**************************************************************************/
VOID DispErrorMsg(HAB hab, HWND hwndFrame, PCH FileName, LONG LineNum)
{
 PERRINFO  pErrInfoBlk;
 PSZ       pszOffSet, pszErrMsg;
 ERRORID   ErrorId;
 PCH       ErrorStr;
 CHAR      szbuff[125];

   DosBeep(800,10);
   #if defined(DEBUG)
      DosBeep(800,10);
      DosBeep(800,10);
      DosBeep(800,10);
      DosBeep(800,10);
      DosBeep(800,10);
   #endif   /* defined(DEBUG) */

   if (!hab)
   {                                     /* Non-PM Error */
      WinLoadString( habMain,0, IDS_UNKNOWNMSG, sizeof(szbuff), (PSZ)szbuff);
      ErrorStr = malloc(strlen(szbuff)+strlen(FileName)+10);
      sprintf(ErrorStr, szbuff, FileName, LineNum);
      WinMessageBox(HWND_DESKTOP,         /* Parent window is desk top */
                    hwndFrame,            /* Owner window is our frame */
                    (PSZ)ErrorStr,        /* PMWIN Error message       */
                    szErrorTitle,         /* Title bar message         */
                    MSGBOXID,             /* Message identifier        */
                    MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); /* Flags */
      free(ErrorStr);
      return;
   }

   ErrorId = WinGetLastError(hab);

   if ((pErrInfoBlk = WinGetErrorInfo(hab)) != (PERRINFO)NULL)
   {
      pszOffSet = ((PSZ)pErrInfoBlk) + pErrInfoBlk->offaoffszMsg;
      pszErrMsg = ((PSZ)pErrInfoBlk) + *((PULONG)pszOffSet);

      WinLoadString( habMain,0, IDS_ERRORMSG, sizeof(szbuff), (PSZ)szbuff);
      ErrorStr = malloc(strlen(szbuff)+strlen(pszErrMsg)+strlen(FileName)+10);
      sprintf(ErrorStr, szbuff, pszErrMsg, FileName, LineNum);

      WinMessageBox(HWND_DESKTOP,         /* Parent window is desk top */
                    hwndFrame,            /* Owner window is our frame */
                    (PSZ)ErrorStr,        /* PMWIN Error message       */
                    szErrorTitle,         /* Title bar message         */
                    MSGBOXID,             /* Message identifier        */
                    MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); /* Flags */

      free(ErrorStr);

      WinFreeErrorInfo(pErrInfoBlk);
   }
}


/********************************************************************/

/* DosGetThreadInfo Call Only Temp Call for the LX Toronto Compiler */

/********************************************************************/

APIRET DosGetThreadInfo(PTIB *pptib,PPIB *pppib)
{

      return( DosGetInfoBlocks(pptib,pppib) );
}

/*******************************  END JIGSAW.C  ******************************/

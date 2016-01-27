/***************************************************************************
*
*  File name       : BEEHIVE.C
*
* Description      : This program demonstrates the use of the sprite compiler
*                    and the advantages of compiled sprites over normal
*                    sprite alorithms.
*
* Entry Points     :
*                    Initialize()
*                    SetupDive()
*                    SetupFullScreen()
*                    ReadFile()
*                    Main()
*                    MyWindowProc()
*
* DIVE API's       :
*                    DiveBlitImage
*                    DiveAllocImageBuffer
*                    DiveOpen
*                    DiveFreeImageBuffer
*                    DiveClose
*                    DiveSetupBlitter
*
* Copyright        : COPYRIGHT IBM CORPORATION, 1991, 1992, 1993, 1994, 1995
*
*        DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*        sample code created by IBM Corporation. This sample code is not
*        part of any standard or IBM product and is provided to you solely
*        for  the purpose of assisting you in the development of your
*        applications.  The code is provided "AS IS", without
*        warranty of any kind.  IBM shall not be liable for any damages
*        arising out of your use of the sample code, even if they have been
*        advised of the possibility of such damages.
*
****************************************************************************/



#define INCL_DOS
#define INCL_GPI
#define INCL_WIN
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define  _MEERROR_H_
#include <mmioos2.h>
#include <dive.h>
#include <fourcc.h>

#include "beehive.h"



// Global definitions
//
HAB       hab;                         // PM anchor block handle
HDIVE     hDive;
HWND      hwndFrame;                   // Frame window handle
HWND      hwndClient;                  // Client window handle
HPAL      hpal;                        // Handle to new palette
HMTX      hmtx;                        // used to serialize buffer access

BOOL      fVrnDisabled = TRUE;
BOOL      fEndThread = FALSE;
BOOL      fUseCompiledBlit = TRUE;
ULONG     ulrc;

BMPDATA     bmpScene;
BMPDATA     bmpImage1,                 // Bee moving to the right
            bmpImage1b,                // Bee moving to the right with wing
            bmpImage2,                 // Bee moving to the left
            bmpImage2b,                // Bee moving to the left with wing
            bmpBackGround;             // Back Ground

SPRITEDATA  FirstSprite;

DIVE_CAPS DiveCaps = {0};

BOOL      fFSBase = TRUE;               // FS DIVE support in base OS


//**************************************************************************
// Name           : SetupFullScreen
//
// Description    : This procedure provides for full screen dive support
//                  by loading the GAMESRVR dll and retrieving the address
//                  of the init procedure.
//
// Returns        : 1 = Fail
//                  0 = Successful
//
//**************************************************************************

ULONG SetupFullScreen( VOID )
{
    PFN       pfnInitGameFrameProc;     // GameSrvr
    UCHAR     szErrorBuf[256];          // String used for error msgs
    HMODULE   hmodGameSrvr;             // Module handle for GAMESRVR

    ULONG     aulVersion[2];            // OS/2 version number
    ULONG     pvmi;
    ULONG     ul;



    DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR, &aulVersion, 8 );

    if (aulVersion[0] == 20 && aulVersion[1] <= 30 )
    {
       fFSBase = FALSE;

       if ( 0 == DosLoadModule( szErrorBuf, 256, "GAMESRVR", &hmodGameSrvr ) )
       {
           if ( 0 == DosQueryProcAddr( hmodGameSrvr, 1, 0, &pfnInitGameFrameProc ) )
           {
               ( pfnInitGameFrameProc )( hwndFrame, 0 );
           }
       }
       else
       {
           WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)"usage: BEEHIVE failed to load GAMESRVR.DLL.",
                   (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );

            return( 1 );
       }
    }
    else
    {
       fFSBase = TRUE;
       DosLoadModule( szErrorBuf, 256, "PMMERGE", &hmodGameSrvr );
       if ( 0 == DosQueryProcAddr( hmodGameSrvr, 6099, 0, &pfnInitGameFrameProc ) )
       {
           ( pfnInitGameFrameProc )( hwndFrame, 0 );
       }
       else
       {
           WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)"usage: BEEHIVE failed to access FS functions.",
                   (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
            return( 1 );
       }
    }

  WinSendMsg( hwndFrame, WM_GetVideoModeTable, (MPARAM)&pvmi, (MPARAM)&ul );

   return( 0 );
}



//***************************************************************************
//     Name:      SetupDive
//
//    Description:
//
//       This routine will check that the system can support DIVE. One of
//    the requirements is that the display must be setup for at least 256
//    colors. If successful, this routine will open one dive instance. If
//    unsuccessful, a dialog box will be displayed and the routine will
//    return a non zero value.
//
//    Parameters:    NONE
//
//    Returns:       0 = successful
//                   1 = failed
//
//***************************************************************************

ULONG SetupDive ( VOID )
{
//   DIVE_CAPS DiveCaps = {0};
   FOURCC    fccFormats[100] = {0};        // Color format code

   // Get the screen capabilities, and if the system support only 16 colors
   // the sample should be terminated.
   //
   DiveCaps.pFormatData = fccFormats;
   DiveCaps.ulFormatLength = 120;
   DiveCaps.ulStructLen = sizeof(DIVE_CAPS);

   DiveQueryCaps ( &DiveCaps, DIVE_BUFFER_SCREEN );

   if ( DiveCaps.ulDepth < 8 )
   {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
          (PSZ)"usage: The sample program can not run on this system environment.",
          (PSZ)"PARALAX.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
      return ( 1 );
   }

   // Get an instance of DIVE APIs.
   //
   if ( DiveOpen ( &hDive, FALSE, 0 ) )
   {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
               (PSZ)"usage: Unable to open a DIVE instance.",
               (PSZ)"PARALAX.EXE - Game Sample", 0, MB_OK | MB_INFORMATION );
      return ( 1 );
   }

   return( 0 );

}

//***************************************************************************
//
// Description   : The main "off screen" image buffer is allocated here and
//                 all of the required images are read from local pcx files.
//                 Also, the first sprite in the animation is initialized.
//
// Parameters    : None
//
// Return        : 0 = Successful
//                 1 = Failed
//
//***************************************************************************

ULONG Initialize( VOID )
{
   // Create a dive instance
   //
   ulrc = SetupDive();
   if( ulrc )
      return ulrc;

   ulrc = ReadFile( "bee1.pcx", &bmpImage1 );     // bee moving right
   ulrc += ReadFile( "bee1b.pcx", &bmpImage1b );   // bee moving right with wing
   ulrc += ReadFile( "bee2.pcx", &bmpImage2 );     // bee moving left
   ulrc += ReadFile( "bee2b.pcx", &bmpImage2b );   // bee moving left with wing
   ulrc += ReadFile( "honey.pcx", &bmpBackGround );
   if( ulrc )
      return ulrc;

   // Associate dos buffer with dive.
   //
   bmpScene.ulWidth = bmpBackGround.ulWidth;
   bmpScene.ulHeight = bmpBackGround.ulHeight;

   // Allocate memory for scene buffer
   //
   ulrc = DosAllocMem ( (PPVOID)&bmpScene.pbBuffer,
                        bmpScene.ulWidth * bmpScene.ulHeight,
                        MY_ALLOC_FLAGS );
   if( ulrc ) return( ulrc );

   memset( bmpScene.pbBuffer, 0, bmpScene.ulWidth * bmpScene.ulHeight );

   bmpScene.ulImage = 0;
   ulrc = DiveAllocImageBuffer ( hDive,
                          &bmpScene.ulImage,
                          FOURCC_LUT8,
                          bmpScene.ulWidth,
                          bmpScene.ulHeight,
                          0,
                          bmpScene.pbBuffer );
   if( ulrc ) return ulrc;

   // Generate a blitter for images
   //
   ulrc += GenBlitSprite( (PPVOID)&bmpImage1.pfnBlit,   // pointer to blitter
                  bmpImage1.pbBuffer,           // Pointer to image data
                  bmpImage1.ulWidth,            // Width of image
                  bmpImage1.ulHeight,           // Height of image
                  bmpScene.ulWidth,             // Width of scene buffer
                  0 );                          // Transparent color

   ulrc += GenBlitSprite( (PPVOID)&bmpImage1b.pfnBlit,   // pointer to blitter
                  bmpImage1b.pbBuffer,           // Pointer to image data
                  bmpImage1b.ulWidth,            // Width of image
                  bmpImage1b.ulHeight,           // Height of image
                  bmpScene.ulWidth,             // Width of scene buffer
                  0 );                          // Transparent color

   ulrc += GenBlitSprite( (PPVOID)&bmpImage2.pfnBlit,   // pointer to blitter
                  bmpImage2.pbBuffer,           // Pointer to image data
                  bmpImage2.ulWidth,            // Width of image
                  bmpImage2.ulHeight,           // Height of image
                  bmpScene.ulWidth,             // Width of scene buffer
                  0 );                          // Transparent color

   ulrc += GenBlitSprite( (PPVOID)&bmpImage2b.pfnBlit,   // pointer to blitter
                  bmpImage2b.pbBuffer,           // Pointer to image data
                  bmpImage2b.ulWidth,            // Width of image
                  bmpImage2b.ulHeight,           // Height of image
                  bmpScene.ulWidth,             // Width of scene buffer
                  0 );                          // Transparent color

   if( ulrc ) return ulrc;

   // Initialize sprite control structure
   //
   FirstSprite.pbmp = &bmpImage1;         // Set first frame of sprite
   FirstSprite.ulPositionX = 0;           // Start sprite at upper left
   FirstSprite.ulPositionY = 0;
   FirstSprite.lDeltaX    = 4;            // Default velocity
   FirstSprite.lDeltaY    = 2;
   FirstSprite.pNextSprite = NULL;

   // Create a mutex semaphore for serializing access to video buffers.
   //
   DosCreateMutexSem( (PSZ)NULL, &hmtx, 0UL, FALSE );

   return( 0 );

}



//***************************************************************************
//
// Name          : MyWindowProc
//
// Description   : Window procedure.
//
// Concepts      :
//
// Parameters    : Message parameter 1
//                 Message parameter 2
//
// Return        : calling WinDefWindowProc
//
//***************************************************************************

MRESULT EXPENTRY MyWindowProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   HRGN     hrgn;                   // Region handle
   RGNRECT  rgnCtl;                 // Processing control structure
   RECTL    rclBound;               // Bounding rectangle for blitting
   POINTL   pointl;                 // Lower left corner of window
   SWP      swp;                    // Standard window position
   RECTL    rcls[50];               // Visible rectangles
   ULONG    ulNumRcls;

   USHORT   fsKeyFlags;             // Virtual key message flags.
   PSPRITEDATA  pSprite,            // Sprite data structure
                pNewSprite;

   SETUP_BLITTER SetupBlitter;      // structure for DiveSetupBlitter

   static HDC  hdc;                 // Device context for client window
   static HPS  hps;                 // Presentation space handle
   static BOOL fSwitching = FALSE;  // TRUE when switchine to/from full screen
   static BOOL fFullScreen = FALSE; // TRUE when in full screen

   PQMSG     pqmsg;

   switch( msg )
   {
   case WM_CREATE:

      // Create a presentation space
      //
      hdc = WinOpenWindowDC( hwnd );
      hps = WinGetPS ( hwnd );
      break;

   case WM_NotifyVideoModeChange:

      // If mp1 == 0 then GAMESRVR is about to switch to full screen mode
      // and we need to turn off blitting. If mp1 is non zero then GAMSRVR
      // has just finished a transition and we have to check what mode we
      // are now in. The current mode is indicated by mp2 where 0 or 1 is
      // desktop mode and anything else is full screen mode. Currently,
      // full screen mode only supports mode 13. This will be extended
      // in the future.
      //
      if( (ULONG)mp1 == 0 )
      {
         fSwitching = TRUE;
         fVrnDisabled = TRUE;

         printf( "beginning switch screen mode \n ");

         // Grab the mutex semaphore to stop blitting
         //
         DosRequestMutexSem( hmtx, SEM_INDEFINITE_WAIT );

         printf( "recieved mutex sem \n" );
      }
      else
      {
         // Determine the mode we are now in
         //
         if( ( (ULONG)mp2 == 0 ) || ( (ULONG)mp2 == 1 ) )
         {
            fFullScreen = FALSE;
         printf( "done switching to desktop mode\n" );
         }

         else
         {
            fFullScreen = TRUE;
         printf( "done switching to full screen \n" );
         }

         // Indicate that we are done switching modes
         //
         fSwitching = FALSE;

         // Release the mutex semaphore to enable blitting
         //
         DosReleaseMutexSem( hmtx );

         printf( "mutex sem released\n" );

         // Post a message to setup dive
         //
         WinPostMsg( hwndFrame, WM_VRNENABLED, 0L, 0L );
      }
      break;

   case WM_COMMAND:

      switch ( SHORT1FROMMP ( mp1 ) )
      {
      case ID_EXIT:

         // Make sure that we are in desk top mode
         //
         WinSendMsg( hwndFrame, WM_SetVideoMode, (MPARAM)WS_DesktopDive, 0 );

         // Post to quit the dispatch message loop.
         //
         WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
         break;

      case ID_NEWTEXT:

         // Write new text string to the title bar
         //
         WinSetWindowText ( hwndFrame, (PSZ)mp2 );
         break;

      case ID_FULLSCRN:

         // Indicate that we are switching to full screen mode
         // and disable blitting.
         //
         fVrnDisabled = TRUE;
         fSwitching = TRUE;

         // Post the message to switch to full screen mode.
         //
         WinPostMsg( hwndFrame,
                     WM_SetVideoMode,
                     (MPARAM)WS_FullScreenDive,
                     0 );
         break;

      case ID_NORMAL:
         // The user has requested that we switch to normal blit
         // mode. Setting this flag will cause the animation module
         // to use a normal blit routine instead of the compiled
         // blit routines.
         //
         fUseCompiledBlit = FALSE;
         break;

      case ID_COMPILED:
         // The user has requested that we use the compiled blit routines
         // Setting this flag will cause the animation module to use the
         // compiled blit routines.
         //
         fUseCompiledBlit = TRUE;
         break;


      default:

         // Let PM handle this message.
         //
         return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
      }
      break;


   case WM_VRNDISABLED:

      DiveSetupBlitter ( hDive, 0 );
      fVrnDisabled = TRUE;
      break;

   case WM_VRNENABLED:

      // Break if we are in the process of switching modes.
      //
      if( fSwitching ) break;

      // If we are in full screen mode then the blitter parameters are
      // set to accomodate mode 13. Other wise we need the blitter
      // parameters to match the window properties.
      //
      if( fFullScreen )
      {
         swp.cx = 320;              // Set width for mode 13.
         swp.cy = 200;              // Set height for mode 13.
         pointl.x = 0;              // Set window corner to origin
         pointl.y = 0;
         ulNumRcls = 1;             // Only one rectangle in full screen
         rcls[0].xLeft = 0;
         rcls[0].xRight = 320;
         rcls[0].yBottom = 0;
         rcls[0].yTop = 200;
      }
      else
      {
         // Find the window position and size, relative to parent.
         //
         WinQueryWindowPos ( hwndClient, &swp );

         // Convert the point to offset from desktop lower left.
         //
         pointl.x = swp.x;
         pointl.y = swp.y;

         WinMapWindowPoints ( hwndFrame, HWND_DESKTOP,
                              &pointl, 1 );

         hrgn = GpiCreateRegion ( hps, 0L, NULL );
         if ( hrgn )
         {
            // NOTE: If mp1 is zero, then this was just a move message.
            // Illustrate the visible region on a WM_VRNENABLE.
            //
            WinQueryVisibleRegion ( hwnd, hrgn );
            rgnCtl.ircStart     = 0;
            rgnCtl.crc          = 50;
            rgnCtl.ulDirection  = 1;

            // Get the all visible rectangles
            //
            if( !GpiQueryRegionRects( hps, hrgn, NULL, &rgnCtl, rcls ))
            {
               DiveSetupBlitter ( hDive, 0 );
               GpiDestroyRegion ( hps, hrgn );
               break;
            }

            ulNumRcls = rgnCtl.crcReturned;
            GpiDestroyRegion( hps, hrgn );

            // Release presentation space
            //
            WinReleasePS( hps );

         } // End if

      } // End else

      SetupBlitter.ulStructLen = sizeof ( SETUP_BLITTER );
      SetupBlitter.fccSrcColorFormat = FOURCC_LUT8;
      SetupBlitter.ulSrcWidth = bmpScene.ulWidth;
      SetupBlitter.ulSrcHeight = bmpScene.ulHeight;
      SetupBlitter.ulSrcPosX = 0;
      SetupBlitter.ulSrcPosY = 0;
      SetupBlitter.fInvert = FALSE;
      SetupBlitter.ulDitherType = 1;

      SetupBlitter.fccDstColorFormat = FOURCC_SCRN;
      SetupBlitter.ulDstWidth = swp.cx;
      SetupBlitter.ulDstHeight = swp.cy;
      SetupBlitter.lDstPosX = 0;
      SetupBlitter.lDstPosY = 0;
      SetupBlitter.lScreenPosX = pointl.x;
      SetupBlitter.lScreenPosY = pointl.y;
      SetupBlitter.ulNumDstRects = ulNumRcls;
      SetupBlitter.pVisDstRects = rcls;
      DiveSetupBlitter ( hDive, &SetupBlitter );

      // Enable blitting
      //
      fVrnDisabled = FALSE;
      break;

   case WM_CLOSE:

      // Post to quit the dispatch message loop.
      //
      WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
      break;

   case WM_CHAR:
      // If the user presses the 'h' or 'H' key, then start a timer that
      // causes sprites to be generated. When the user releases the key
      // then stop the timer.

      // Character input: first two byte of message is the character code.
      //
      fsKeyFlags = (USHORT) SHORT1FROMMP ( mp1 );

      // If the key is released then stop the timer
      //
      if( fsKeyFlags & KC_KEYUP )
      {
         WinStopTimer( hab, hwnd, ID_TIMER );
         break;
      }

      // Throw away key repeate messages
      //
      if( fsKeyFlags & KC_PREVDOWN ) break;

      switch ( SHORT1FROMMP ( mp2 ) )
      {
      case 'h':
      case 'H':
         // User has just pressed the right key so start
         // a 100 ms timer.
         //
         WinStartTimer( hab, hwnd, ID_TIMER, 100 );
         break;
      case 'f':
      case 'F':
           if ( fFullScreen )
           {
               WinSendMsg( hwndFrame, WM_SetVideoMode, (MPARAM)WS_DesktopDive, 0 );
               fFullScreen = FALSE;
           }
           else
           {
               // Indicate that we are switching to full screen mode
               // and disable blitting.
               fVrnDisabled = TRUE;
               fSwitching = TRUE;
               WinSendMsg( hwndFrame, WM_SetVideoMode, (MPARAM)WS_FullScreenDive, 0 );
               fFullScreen = TRUE;
           }
         break;
      } // end switch

   case WM_TIMER:
      // This timer message will cause a sprite to be generated.
      //
      if( SHORT1FROMMP( mp1 ) == ID_TIMER )
      {

         // Create another sprite and add it to the linked list
         //
         pSprite = &FirstSprite;

         // Travers the list until we reach the last sprite
         //
         while( pSprite->pNextSprite != NULL )
            pSprite = pSprite->pNextSprite;

         // Allocate space for a new sprite.
         //
         ulrc = DosAllocMem ( (PPVOID)&pNewSprite,
                              sizeof( SPRITEDATA ),
                              MY_ALLOC_FLAGS );
         if( ulrc ) break;

         pNewSprite->pbmp = &bmpImage1;         // Set first frame of sprite

         pNewSprite->ulPositionX = 0;           // Set sprite position
         pNewSprite->ulPositionY = 0;

         pNewSprite->lDeltaX = ( 6 * rand() ) / RAND_MAX + 1;
         pNewSprite->lDeltaY = ( 3 * rand() ) / RAND_MAX + 1;

         pNewSprite->pNextSprite = NULL;

         // Add the new sprite to the linked list.
         //
         pSprite->pNextSprite = pNewSprite;

      }  // end if

      break;

   }  // end switch

   return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
}



//**************************************************************************
//
// Name           : main
//
// Description    : After initializing, a second thread is created that
//                  is responsible for continuously updating the output
//                  window for the next frame of the animation.
//
// Parameters     : None
//
//**************************************************************************




ULONG main ( VOID )
   {
   PSZ      pszMyWindow = "MyWindow";     // Window class name
   PSZ      pszTitleText = "PARALAX";     // Title bar text

   TID      tidBlitThread;
   HMQ      hmq;                 // Message queue handle
   QMSG     qmsg;                // Message from message queue
   ULONG    flCreate;            // Window creation control flags
   ULONG    i;                   // Index for the files to open
   LONG     lCx, lCy;            // System values for screen extents

   PSPRITEDATA pSpriteData,
               pNextSprite;

   extern APIENTRY Animation( void );

   /* Initialize the presentation manager, and create a message queue.
   */
   hab = WinInitialize ( 0 );
   hmq = WinCreateMsgQueue ( hab, 0 );

   ulrc = Initialize();
   if( !ulrc )
   {

      // Register a window class, and create a standard window.
      //
      WinRegisterClass ( hab, pszMyWindow, MyWindowProc, 0, 0 );

      flCreate = FCF_STANDARD & ~FCF_SHELLPOSITION & ~FCF_ACCELTABLE;

      hwndFrame = WinCreateStdWindow ( HWND_DESKTOP,
                                       0,
                                       &flCreate,
                                       pszMyWindow,
                                       pszTitleText,
                                       WS_SYNCPAINT | WS_VISIBLE,
                                       NULLHANDLE,
                                       ID_MAINWND,
                                       &hwndClient );

      // Setup full screen dive
      //
      SetupFullScreen();

      // Center the window on the desktop
      //
      lCx = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
      lCy = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );

      WinSetWindowPos( hwndFrame,
                     NULLHANDLE,
                     ( lCx - 640 )/2,
                     ( lCy - 480 )/2,
                     640,
                     480,
                     SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE );

      //WinSetActiveWindow( HWND_DESKTOP, hwndFrame );

      // Turn on visible region notification.
      //
      WinSetVisibleRegionNotify ( hwndClient, TRUE );

      // Send an invlaidation message to the client.
      //
      WinPostMsg ( hwndFrame, WM_VRNENABLED, 0L, 0L );

      // Create animation thread
      //
      DosCreateThread ( &tidBlitThread,
                        (PFNTHREAD) Animation,
                        0L,
                        0L,
                        8192L);

      // While there are still messages, dispatch them.
      //
      while ( WinGetMsg ( hab, &qmsg, 0, 0, 0 ) )
         WinDispatchMsg ( hab, &qmsg );
   }

   fEndThread = TRUE;
   DosWaitThread ( &tidBlitThread, DCWW_WAIT );

   // Turn off visible region notificationm tidy up, and terminate.
   //
   WinSetVisibleRegionNotify ( hwndClient, FALSE );

   // Release allocated buffers
   //
   pSpriteData = FirstSprite.pNextSprite;

   while( pSpriteData != NULL )                 // While there are sprites..
   {
      pNextSprite = pSpriteData->pNextSprite;   // Save pointer to next sprite
      DosFreeMem( pSpriteData );                // Free sprite structure
      pSpriteData = pNextSprite;                // Get pointer to next sprite
   }

   // Free Image buffers
   //
   DiveFreeImageBuffer( hDive, bmpScene.ulImage );
   DosFreeMem( bmpScene.pbBuffer );
   DosFreeMem( bmpBackGround.pbBuffer );
   DosFreeMem( bmpImage1.pbBuffer );
   DosFreeMem( bmpImage1b.pbBuffer );
   DosFreeMem( bmpImage2.pbBuffer );
   DosFreeMem( bmpImage2b.pbBuffer );

   // Free generated sprite code
   //
   DosFreeMem( ( PVOID )bmpImage1.pfnBlit );
   DosFreeMem( ( PVOID )bmpImage1b.pfnBlit );
   DosFreeMem( ( PVOID )bmpImage2.pfnBlit );
   DosFreeMem( ( PVOID )bmpImage2b.pfnBlit );

   WinDestroyWindow ( hwndFrame );
   WinDestroyMsgQueue ( hmq );
   WinTerminate ( hab );

   return ( 0 );
   }

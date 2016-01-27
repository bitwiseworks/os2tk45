/***************************************************************************
*
* File name        : ANIMATE.C
*
* Description      : This module contains the procedure that will maintain
*                    the sprites and update the scene buffer. Also, the
*                    procedure that provides for the "normal sprit" algorithm
*                    is also here
*
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

#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dive.h>
#include "beehive.h"


extern BMPDATA bmpScene,
               bmpImage1,
               bmpImage1b,
               bmpImage2,
               bmpImage2b,
               bmpBackGround;

extern SPRITEDATA FirstSprite;

extern BOOL    fEndThread;
extern HDIVE   hDive;
extern HWND    hwndFrame;
extern BOOL    fVrnDisabled;
extern BOOL    fUseCompiledBlit;
extern HMTX    hmtx;


//************************************************************************
//
// Name        : BlitSprite
//
// Description : This function is provided to demonstrate the
//               diference between conventional sprite alogrithms
//               and a compiled sprite. When called, this routine
//               will copy the passed sprite to an arbitrary location
//               in a destination buffer. The user can select which
//               routine the application will use from the options
//               menu.
//
//************************************************************************

VOID  BlitSprite( PBMPDATA pbmpSprite,       // Pointer to sprite structure
                  PBMPDATA pbmpScene,        // Pointer to target structure.
                  ULONG    ulPositionX,      // Sprite X position in target
                  ULONG    ulPositionY,      // Sprite Y position in target
                  BYTE     bTransparentColor )
{
   PBYTE    pbTarget,
            pbSource;

   ULONG    ulIndex1,
            ulIndex2;

   // Copy the source pointer.
   //
   pbSource = pbmpSprite->pbBuffer;

   // Copy the destination pointer and adjust for the position of the
   // sprite.
   //
   pbTarget = pbmpScene->pbBuffer +
              ulPositionY * pbmpScene->ulWidth +  ulPositionX;

   // Process each scan line in the sprite.
   //
   for( ulIndex1 = 0; ulIndex1 < pbmpSprite->ulHeight; ulIndex1++ )
   {
      // Traverse one line of the sprite while checking each pixel
      // for transparency before copying.
      //
      for( ulIndex2 = 0; ulIndex2 < pbmpSprite->ulWidth; ulIndex2++ )
      {
         // Check if this pixel is a transparent color. If not, then copy the
         // pixel to the target and update the pointers.
         //
         if( *pbSource != bTransparentColor )
            *pbTarget++ = *pbSource++;
         else
         {
            // Here if the pixel was transparent so we don't copy the pixel
            // and we just increment the buffer pointers.
            //
            *pbTarget++;
            *pbSource++;
         }
      }  // end for loop

      // Adjust the target buffer pointer for the next scan line
      //
      pbTarget += pbmpScene->ulWidth - pbmpSprite->ulWidth;

   } // end for loop
}




//************************************************************************
//
// Name        : Animation
//
// Description : This procedure is spawned in a separate thread and
//               is responsible for updating the scene buffer.
//
// Returns     : none
//
//************************************************************************

VOID APIENTRY Animation ( void )
{
   CHAR     szFrameRate[256];
   PSPRITEDATA pSprite;
   ULONG    ulFramesToTime = 16,
            rc,
            ulNumFrames = 0,
            ulPostCount,
            ulTime0 = 0,
            ulTime1 = 0,
            ulSpriteCount;


   while( !fEndThread )
   {
      //Clear the scene buffer
      //
      memcpy( bmpScene.pbBuffer,
              bmpBackGround.pbBuffer,
              bmpScene.ulWidth * bmpScene.ulHeight );

      // Get the first sprite in the sprite list
      //
      pSprite = &FirstSprite;

      // Zero the sprite counter
      //
      ulSpriteCount = 0;

      // Add each sprite from the list to the scene buffer
      //
      do
      {
         ulSpriteCount++;

         // The fUseCompiledBlit flag determins what blit routine we will
         // use
         if( fUseCompiledBlit == TRUE )
         {
            // Add sprite to image buffer using the compiled blitter
            //
            pSprite->pbmp->pfnBlit( pSprite->pbmp->pbBuffer,
                                    bmpScene.pbBuffer,
                                    pSprite->ulPositionX,
                                    pSprite->ulPositionY );
         }
         else
         {
            // Add the sprite to the image buffer using the normal blitter.
            //
            BlitSprite( pSprite->pbmp,
                        &bmpScene,
                        pSprite->ulPositionX,
                        pSprite->ulPositionY,
                        0 );
         }

         // Upate the sprite image
         //
         if( pSprite->lDeltaX > 0 )
         {
            if( pSprite->ulState == 1 )
            {
               pSprite->ulState = 2;
               pSprite->pbmp = &bmpImage1b;
            }
            else
            {
               pSprite->ulState = 1;
               pSprite->pbmp = &bmpImage1;
            }
         }
         else
         {
            if( pSprite->ulState == 1 )
            {
               pSprite->ulState = 2;
               pSprite->pbmp = &bmpImage2b;
            }
            else
            {
               pSprite->ulState = 1;
               pSprite->pbmp = &bmpImage2;
            }
         }

         // Update the position of this sprite
         //
         pSprite->ulPositionX += pSprite->lDeltaX;
         pSprite->ulPositionY += pSprite->lDeltaY;

         // Check for collision with the walls
         //
         if( pSprite->ulPositionX >= ( bmpScene.ulWidth - bmpImage1.ulWidth ) )
         {
            pSprite->lDeltaX *= -1;
            pSprite->ulPositionX += pSprite->lDeltaX;

            // Set the correct sprite frame
            //
            if( pSprite->lDeltaX < 0 )
               pSprite->pbmp = &bmpImage2;
            else
               pSprite->pbmp = &bmpImage1;
         }

         if( pSprite->ulPositionY >= ( bmpScene.ulHeight - bmpImage1.ulHeight ) )
         {
            pSprite->lDeltaY *= -1;
            pSprite->ulPositionY += pSprite->lDeltaY;
         }

         pSprite = pSprite->pNextSprite;

      } while( pSprite != NULL );


      // Make sure that the buffers are free.
      //
      DosRequestMutexSem( hmtx, SEM_INDEFINITE_WAIT );

      // Blit the scene to the screen
      //
      if( !fVrnDisabled )
         DiveBlitImage( hDive, bmpScene.ulImage, DIVE_BUFFER_SCREEN );

      if( !ulNumFrames++ )
         DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &ulTime0, 4L );

      // Update the title bar with the appropriate frame rate and sprite
      // count.
      //
      if( ulNumFrames == ulFramesToTime )
      {
         DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &ulTime1, 4L );
         ulTime1 -= ulTime0;

         sprintf( szFrameRate,
                  "F.P.S. = %d, Sprite Count = %d ",
                  ulFramesToTime * 1000 / ulTime1,
                  ulSpriteCount );
         ulNumFrames = 0;
         WinPostMsg( hwndFrame, WM_COMMAND, (PVOID)ID_NEWTEXT, szFrameRate );
      }

      DosReleaseMutexSem( hmtx );

      // This call will create a small hit on the frame rate but will make
      // the application much more friendly to other applications.
      //
      DosSleep( 0 );
   }

}

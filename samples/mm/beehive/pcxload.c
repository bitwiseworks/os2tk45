
/***************************************************************************
*
* File name        : PCXLOAD.C
*
* Description      : This module contains procedures that can be used for
*                    loading and processing PCX image files
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
#include <mmioos2.h>
#include <dive.h>
#include <fourcc.h>
#include "beehive.h"


extern DIVE_CAPS DiveCaps;
extern HDIVE     hDive;

ULONG    ulrc;

//***************************************************************************
//
// Each PCX image file contains palette information. This procedure
// will extract this information from the image file passed and store
// it in a global buffer called pbPalette. The pbPalette buffer is
// a 1024 byte standard OS/2 BMP palette that must be allocated by
// the calling routine.
//
//***************************************************************************

ULONG ExtractPalette( PSZ pszFile, PBYTE pbPalette )
{
   PBYTE    pbTmpPalette,
            pbRawPalette,
            pbTmpRawPalette;
   FILE     *pFile;
   ULONG    i;


   // Open image file
   //
   if( ( pFile = fopen ( pszFile, "rb" ) ) == NULL )
      return( 1 );

   // Seek to the beginning of the palette data
   //
   i = fseek ( pFile, -768L, SEEK_END );

   // Allocate temp buffers for palette data
   //
   ulrc = DosAllocMem ( (PPVOID)&pbRawPalette, 768, MY_ALLOC_FLAGS );
   if( ulrc ) return( ulrc );

   // Save buffer pointers
   //
   pbTmpPalette = pbPalette;
   pbTmpRawPalette = pbRawPalette;

   // Read raw palette data
   fread( pbTmpRawPalette, sizeof( CHAR ), 768L, pFile );

   // Convert to 4 bytes per color
   //
   for( i = 0; i < 256; i++ )
   {
      *pbTmpPalette++ = *(pbTmpRawPalette + 2);
      *pbTmpPalette++ = *(pbTmpRawPalette + 1);
      *pbTmpPalette++ = *pbTmpRawPalette;
      *pbTmpPalette++ = 0;

      pbTmpRawPalette += 3;
   }

   // Clean up and close file
   //
   DosFreeMem( pbRawPalette );

   fclose( pFile );

   return( 0 );
}



//**********************************************************************
//
// When dive blits an image to the screen, the color space translation
// can slow down the blitter. By prepocessing all of the images before
// blitting them to the screen, this overhead is eliminated.
//
// Note: This is only relavent when the display color depth is set to 8
//       8 bit.
//
//**********************************************************************

ULONG  ConvertImage( PBMPDATA pbmpData, PBYTE pbPalette )
{
   SETUP_BLITTER     SetupBlitter;
   RECTL             rcl;
   extern HDIVE      hDive;
   PBYTE             pbTargetBuffer;
   ULONG             ulTargetImage;


   // Associate source buffer with dive
   //
   pbmpData->ulImage = 0;
   ulrc = DiveAllocImageBuffer ( hDive,
                          &pbmpData->ulImage,
                          FOURCC_LUT8,
                          pbmpData->ulWidth,
                          pbmpData->ulHeight,
                          0,
                          pbmpData->pbBuffer );
   if( ulrc ) return( ulrc );

   // Allocate and associate target buffer with dive. The size of the
   // target buffers are dictated by the size of the scene buffer.
   //
   ulrc = DosAllocMem ( (PPVOID)&pbTargetBuffer,
                        pbmpData->ulWidth * pbmpData->ulHeight,
                        MY_ALLOC_FLAGS );
   if( ulrc ) return( ulrc );


   ulTargetImage = 0;
   ulrc = DiveAllocImageBuffer ( hDive,
                          &ulTargetImage,
                          FOURCC_SCRN,
                          pbmpData->ulWidth,
                          pbmpData->ulHeight,
                          0,
                          pbTargetBuffer );
   if( ulrc ) return( ulrc );

   rcl.xLeft = 0;
   rcl.yBottom = 0;
   rcl.xRight = pbmpData->ulWidth;
   rcl.yTop= pbmpData->ulHeight;

   // Setup the blitter
   //
   SetupBlitter.ulStructLen = sizeof ( SETUP_BLITTER );
   SetupBlitter.fccSrcColorFormat = FOURCC_LUT8;
   SetupBlitter.ulSrcWidth = pbmpData->ulWidth;
   SetupBlitter.ulSrcHeight = pbmpData->ulHeight;
   SetupBlitter.ulSrcPosX = 0;
   SetupBlitter.ulSrcPosY = 0;
   SetupBlitter.fInvert = FALSE;
   SetupBlitter.ulDitherType = 1;

   SetupBlitter.fccDstColorFormat = FOURCC_SCRN;
   SetupBlitter.ulDstWidth = pbmpData->ulWidth;
   SetupBlitter.ulDstHeight = pbmpData->ulHeight;
   SetupBlitter.lDstPosX = 0;
   SetupBlitter.lDstPosY = 0;
   SetupBlitter.lScreenPosX = 0;
   SetupBlitter.lScreenPosY = 0;
   SetupBlitter.ulNumDstRects = 1;
   SetupBlitter.pVisDstRects = &rcl;
   DiveSetupBlitter ( hDive, &SetupBlitter );

   // Setup the palette
   //
   DiveSetSourcePalette ( hDive, 0, 256, pbPalette );

   // Blit the image
   //
   DiveBlitImage( hDive, pbmpData->ulImage, ulTargetImage );

   // Disassociate buffers from dive
   //
   DiveFreeImageBuffer( hDive, ulTargetImage );
   DiveFreeImageBuffer( hDive, pbmpData->ulImage );

   // Copy the converted image to the old buffer and free the temp buffer
   //
   memcpy( pbmpData->pbBuffer,
            pbTargetBuffer,
            pbmpData->ulWidth * pbmpData->ulHeight );

   DosFreeMem( pbTargetBuffer );

   // Reset the dive palette
   //
   DiveSetSourcePalette( hDive, 0, 0, 0 );

   return( 0 );
}


/****************************************************************************
 *
 * Name          : ReadFile
 *
 *
 * Parameters    : Index for the file to open
 *                 Pointer to the file name to open
 *                 Pointer to window data
 *
 * Return        : 0 - succeed
 *                 1 - fail
 *
 ****************************************************************************/


ULONG ReadFile ( PSZ pszFile, PBMPDATA pbmpData )

   {
   ULONG       i;
   PCX_HEADER  pcxHeader;
   ULONG       ulOpenParms;
   FILE       *pFile;
   ULONG       ulBytesRead;
   ULONG       ulDataSize;
   CHAR        chData;
   ULONG       ulRunLength;
   ULONG       ulRemBytesInLine;
   ULONG       ulLinesToRead;
   ULONG       ulBytesToRead;
   ULONG       ulError;
   PBYTE       pbTmpDest;
   PBYTE       pbPalette;

   // Open image file
   //
   if( ( pFile = fopen ( pszFile, "rb" ) ) == NULL )
      return( 1 );

   // Read PCX header from image file
   //
   ulBytesRead = fread( (VOID *)&pcxHeader, sizeof( CHAR ),
                        sizeof( PCX_HEADER ), pFile );

   if( ulBytesRead != sizeof( PCX_HEADER ) )
      {
      fclose( pFile );
      return( 1 );
      }

   // Set bitmap width and height in pels
   //
   pbmpData->ulHeight = pcxHeader.shHeight + 1;
   pbmpData->ulWidth = (ULONG)pcxHeader.shBytesPerLine;

   ulDataSize = pbmpData->ulWidth * pbmpData->ulHeight;

   // Allcate buffer for image data
   //
   ulrc = DosAllocMem ( (PPVOID)&pbmpData->pbBuffer, ulDataSize, MY_ALLOC_FLAGS );

   if( ulrc )
      {
      fclose( pFile );
      return( ulrc );
      }

   // Clear the image buffer
   //
   memset( pbmpData->pbBuffer, 0, pbmpData->ulWidth * pbmpData->ulHeight );

   pbTmpDest = pbmpData->pbBuffer;
   ulBytesRead = 0;

   // Read and convert PCX data
   //
   while( ulBytesRead < pbmpData->ulWidth * pbmpData->ulHeight )
      {

      if( ( chData = fgetc( pFile ) ) == EOF ) break;

      // Check if this byte is the beginning of a data run
      //
      if( chData > 192 )
         {
         ulRunLength = chData - 192;

         // Get color data
         //
         chData = fgetc( pFile );

         // Duplicate color for length of run
         //
         for( i = 0; i < ulRunLength; i++ )
            {
            *pbTmpDest++ = chData;
            ulBytesRead++;
            }
         }
      else
         {
         *pbTmpDest++ = chData;
         ulBytesRead++;
         }
      }

   if( ulBytesRead != ulDataSize )
      {
      fclose( pFile );
      DosFreeMem( pbmpData->pbBuffer );
      return( 1 );
      }

   // Done extracting image data.. close file
   //
   fclose( pFile );

   // Allocate a 1024 byte buffer for an OS/2 palette and fill it
   // with the palette data from the passed image.
   //
   ulrc = DosAllocMem ( (PPVOID)&pbPalette, 1024, MY_ALLOC_FLAGS );
   if( ulrc ) return( ulrc );

   ExtractPalette( pszFile, pbPalette );

   // Use the extracted palette to convert the image to the current
   // OS/2 palette.
   //
   if( DiveCaps.ulDepth == 8 )
   {
      ConvertImage( pbmpData, pbPalette );

      // Free the palette buffer
      //
      DosFreeMem( pbPalette );
   }
   else
      DiveSetSourcePalette ( hDive, 0, 256, pbPalette );

   return( 0 );
   }


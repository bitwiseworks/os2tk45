
/***************************************************************************
*
* File name        : BLITGEN.C
*
* Description      : This module contains the procedure that will process
*                    a sprite and generate a specialized blitter specific
*                    to that sprite ( compiled sprites ).
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


#define  INCL_DOS

#include <os2.h>
#include "beehive.h"

//**************************************************************************
//
// This routine will generate another routine that will copy a given
// image from it's source buffer to an arbitrary location in given
// destination image buffer. The generated routine will be optimized
// specifically for the image data that is passed to this routine and
// will only work with images that have identical properties.
//
// The gererated routine will be of the type:
//   VOID   ( * _System pfnBlitSprite )( PBYTE pSourceBuffer,
//                                        PBYTE pDestinationBuffer,
//                                        ULONG ulPositionX,
//                                        ULONG ulPositionY );
//
// When using the generated routine, care must be taken to insure that
// the target location is valid. This generated routine will not check
// for boundary conditions.
//
// Passing a transparent color value that is higher than 255 will have the
// affect of producing a completely opaque blit.
//
//***************************************************************************


ULONG GenBlitSprite( PPVOID ppfnCode,  // Pointer to generated code pointer
                     PBYTE pbSource,   // Pointer to sprite color data
                     ULONG ulSizeX,    // Width of sprite
                     ULONG ulSizeY,    // Height of sprite
                     ULONG ulTargetWidth,   // Width of target video buffer
                     ULONG ulTransparentColor )
{
   PBYTE    pbBuffer,
            pfnCode;

   ULONG    ulCodeSize = 0,            // Total size of generated code
            ulDoubleWordMoves = 0,     // Number of double word moves
            ulAdjustments = 0,         // Number of pointer adjustments
            ulBlanks = 0,
            ulScanLineDelta = 0,
            ulBytes = 0,
            i,
            j,
            ulrc;


   pbBuffer = pbSource;    // copy source buffer pointer

   // First we calculate the required buffer size to contain the generated
   // code. This is done by counting the number of byte and double word move
   // instructions as well as the number of transparent byte runs.

   for(  j = 0; j < ulSizeY; j++ )
   {
      for( i = 0; i < ulSizeX; i++ )
      {
         if( (ULONG)*(pbBuffer + i) != ulTransparentColor )
         {
            // If previous bytes were transparent then adjust pointers
            // This will require 2, 6 byte ADD instructions to adjust
            // the EDI and ESI registers.

            if( ulBlanks )
            {
               ulCodeSize += 6;
               ulBlanks = 0;
            }

            if( ulBlanks || ulScanLineDelta )
            {
               ulCodeSize += 6;
               ulScanLineDelta = 0;
            }

            ulBytes++;

            // If possible, use the faster, double word move

            if( ulBytes == 4 )
            {
               ulDoubleWordMoves++;
               ulBytes = 0;
            }
         }
         else
         {
            ulBlanks++;

            // First use the REP instruction to copy all of the required
            // double words. This is a 7 byte instruction because we first
            // have to set the value of the ECX register for counting.

            if( ulDoubleWordMoves > 1 )
               ulCodeSize += 7;

            if( ulDoubleWordMoves == 1 )
               ulCodeSize++;

            ulDoubleWordMoves = 0;


            // If there are any bytes left to move then we need a
            // series of single byte move instructions.

            for( ; ulBytes > 0; ulBytes-- )
            {
               ulCodeSize++;
            }

         }  // End else

      }  // End for loop

      // This is the end of a scan line. If the target and destination
      // widths are not the same, then we have to generate a blit for
      // each scan line. Otherwise, we wait until the end of the source
      // image.
      //
      // First use the REP instruction to copy all of the required
      // double words. This is a 7 byte instruction because we first
      // have to set the value of the ECX register.

      if( ( ulSizeX != ulTargetWidth ) || ( j == ( ulSizeY - 1 ) ) )
      {
         if( ulDoubleWordMoves > 1 )
            ulCodeSize += 7;

         if( ulDoubleWordMoves == 1 )
            ulCodeSize++;

         ulDoubleWordMoves = 0;

         // If there are any bytes left to move then we need a
         // series of single byte move instructions.

         for( ; ulBytes > 0; ulBytes-- )
         {
            ulCodeSize++;
         }

         ulScanLineDelta++;
      }
      // Go to next line in source buffer
      //
      pbBuffer += ulSizeX;

   }  // End for loop

   // Account for initialization code in the calculated code size

   ulCodeSize += 36;

   pbBuffer = pbSource;    // copy source buffer pointer

   // Allocate space for generated blit routine
   //
   ulrc = DosAllocMem ( ppfnCode, ulCodeSize, MY_ALLOC_FLAGS );
   if( ulrc != 0 ) return ( ulrc );

   pfnCode = *ppfnCode;

   // Generate initiallization code. This code will push the required
   // registers to the stack and then extract the passed parameters
   // from the stack.

   *(PULONG)(pfnCode) = 0x53525150;       // PUSH EAX, ECX, EDX, EBX

   *(PULONG)(pfnCode + 4) = 0x00005756;   // PUSH ESI, EDI
   pfnCode += 6;

   // MOV ESI, pbSourceBuffer
   *(PULONG)(pfnCode) = 0x0024748b + ( ( SAVED_REGS * 4 + 4 ) << 24 );

   // MOV EDI, pbDestinationBuffer
   *(PULONG)(pfnCode + 4) = 0x00247c8b + ( ( SAVED_REGS * 4 + 8 ) << 24 );
   pfnCode += 8;

   // Calculate the offset into the target buffer
   //
   // MOV EAX, ulTargetWidth
   *(pfnCode) = 0xb8;
   *(PULONG)(pfnCode + 1) = ulTargetWidth;
   pfnCode += 5;

   // MUL EAX, ulPositionY
   *(PULONG)(pfnCode) = 0x002464f7 + ( ( SAVED_REGS * 4 + 16 ) << 24 );

   // ADD EAX, ulPositionX
   *(PULONG)(pfnCode + 4) = 0x00244403 + ( ( SAVED_REGS * 4 + 12 ) << 24 );

   // ADD EDI, EAX
   *(PULONG)(pfnCode + 8) = 0x0000f803;
   pfnCode += 10;

   // Now we make another pass at the color data while generating the code.
   //
   ulBlanks = 0;
   ulBytes = 0;
   ulDoubleWordMoves = 0;
   ulScanLineDelta = 0;

   for(  j = 0; j < ulSizeY; j++ )
   {
      for( i = 0; i < ulSizeX; i++ )
      {
         if( (ULONG)*(pbBuffer + i) != ulTransparentColor )
         {
            // If previous bytes were transparent then adjust pointers

            if( ulBlanks )
            {
               // ADD ESI, ulBlanks
               *(PULONG)(pfnCode) = 0x0000c681;
               *(PULONG)(pfnCode + 2) = ulBlanks;
               pfnCode += 6;
            }

            if( ulBlanks || ulScanLineDelta )
            {
               // ADD EDI, ulBlanks
               *(PULONG)(pfnCode) = 0x0000c781;
               *(PULONG)(pfnCode + 2) = ulBlanks + ulScanLineDelta *
                                         ( ulTargetWidth - ulSizeX );
               pfnCode += 6;
               ulScanLineDelta = 0;
            }

            ulBlanks = 0;

            ulBytes++;

            // If possible, use the faster, double word move

            if( ulBytes == 4 )
            {
               ulDoubleWordMoves++;
               ulBytes = 0;
            }
         }
         else
         {
            ulBlanks++;

            // If we have more than 1 double move then use a REP MOVSD to
            // copy all of the counted double words. To do this, we have
            // to first set the value of the CX register which is used
            // as a loop counter.

            if( ulDoubleWordMoves > 1 )
            {
               // MOV CX, ulDoubleWordMoves
               *(pfnCode) = 0xb9;
               *(PULONG)(pfnCode + 1) = ulDoubleWordMoves;
               pfnCode += 5;

               // REP MOVS
               *(PULONG)(pfnCode) = 0x0000a5f3;
               pfnCode += 2;

            }

            // If there is only one double move than we can use a single
            // MOVD. The result is less code than the REP MOVSD and it will
            // save a couple of clock cycles.

            if( ulDoubleWordMoves == 1 )
            {
               //MOVSD
               *(pfnCode++) = 0xa7;
            }

            ulDoubleWordMoves = 0;

            // If there are any bytes left to move then we need a
            // series of single byte MOVS instructions.

            for( ; ulBytes > 0; ulBytes-- )
            {
               *(pfnCode++) = 0xa4;
            }

         }  // End else

      }  // End for loop

      // The inner loop has fallen through but there may still be some
      // image data left on this scan line to blit. If there is then we
      // generate code to blit it before going to the next line.
      //

      // If we have more than 1 double move then use a REP MOVSD to
      // copy all of the counted double words. To do this, we have
      // to first set the value of the CX register which is used
      // as a loop counter.

      if( ( ulSizeX != ulTargetWidth ) || ( j == ( ulSizeY - 1 ) ) )
      {

         if( ulDoubleWordMoves > 1 )
         {

            // MOV CX, ulDoubleWordMoves
            *(pfnCode) = 0xb9;
            *(PULONG)(pfnCode + 1) = ulDoubleWordMoves;
            pfnCode += 5;

            // REP MOVS
            *(PULONG)(pfnCode) = 0x0000a5f3;
            pfnCode += 2;

         }

         // If there is only one double move than we can use a single
         // MOVD. The result is less code than the REP MOVSD and it will
         // save a couple of clock cycles.

         if( ulDoubleWordMoves == 1 )
         {
            //MOVSD
            *(pfnCode++) = 0xa7;
         }

         ulDoubleWordMoves = 0;

         // If there are any bytes left to move then we need a
         // series of single byte move instructions.

         for( ; ulBytes > 0; ulBytes-- )
         {
            *(pfnCode++) = 0xa4;
         }

         if( ulSizeX != ulTargetWidth )
            ulScanLineDelta++;
      }

      pbBuffer += ulSizeX;

   }  // End for loop

   *(PULONG)(pfnCode) = 0x5a5b5e5f;       // POP EDI, ESI, EBX, EDX
   *(PULONG)(pfnCode + 4) = 0x00005859;   // POP ECX, EAX
   pfnCode += 6;

   // RET
   *(pfnCode) = 0xc3;

   return( 0 );

}

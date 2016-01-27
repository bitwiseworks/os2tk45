/*****************************************************************************/
/*                                                                           */
/* SAMPENCO.C: Sample Codec Compression Function Definitions.                */
/*             Your decompression procedure declatations will replace these. */
/*                                                                           */
/* Copyright (c) IBM Corporation 1991,1994             All Rights Reserved   */
/*                                                                           */
/*****************************************************************************/

#define  INCL_WIN
#define  INCL_GPI
#define  INCL_DOS
#define  INCL_OS2MM
#define  INCL_MMIO_CODEC
#include <os2.h>
#include <os2me.h>
#include "sampinst.h"

/* Declarations to look-up tables defined at the bottom of the file.         */
BYTE bLookYh[256], bLookYl[256];



/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: CompressKeyFromRGB565                                    */
/*     This routine does no real compression.  It converts from RGB565 to    */
/*     luminance only (8-bits per pel).                                      */
/*                                                                           */
/*****************************************************************************/

ULONG CompressKeyFromRGB565 ( PBYTE pbSrc, PBYTE pbDst,
                              ULONG ulWidth, ULONG ulHeight )
   {
   ULONG ulImageX, ulImageY;

   /* Loop though each pel of the image, converting each to luminance.       */
   ulImageY = ulHeight;
   while ( ulImageY-- )
      {
      ulImageX = ulWidth;
      while ( ulImageX-- )
         {
         *pbDst++= * ( bLookYl + *pbSrc ) + * ( bLookYh + *(pbSrc+1) );
         pbSrc += 2;
         }
      }

   /* Return 8 bits, or 1 byte per pixel final image size.                   */
   return ( ulWidth * ulHeight );
   }





/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: CompressKeyFromYUV411                                    */
/*     This routine is the same as the above compression excepting that      */
/*     the source data format in this one is YUV411.                         */
/*                                                                           */
/*****************************************************************************/

ULONG CompressKeyFromYUV411 ( PBYTE pbSrc, PBYTE pbDst,
                              ULONG ulWidth, ULONG ulHeight )
   {
   ULONG ulImageX, ulImageY;

   /* Loop though each pel of the image, converting each to luminance.       */
   ulImageY = ulHeight;
   while ( ulImageY-- )
      {
      ulImageX = ulWidth;
      while ( ulImageX-- )
         {
         *pbDst++= *pbSrc;
         pbSrc += 2;
         }
      }

   /* Return 8 bits, or 1 byte per pixel final image size.                   */
   return ( ulWidth * ulHeight );
   }





/*****************************************************************************/
/*                                                                           */
/* SUBROUTINE NAME: Compressor_CompressFrame                                 */
/*     This routine is called from SAMPMAIN.C during a frame compression     */
/*     call.  This example calls the appropriate compression routine based   */
/*     on the type of frame (delta versus key) and the type of input         */
/*     color space format.                                                   */
/*                                                                           */
/*****************************************************************************/

ULONG CompressFrame ( PMAIN_INST pMainInst,
                      PBYTE      pbSrcBuffer,
                      PBYTE      pbDstBuffer )
   {
   /* Check to see if we are compressing from RGB16 or YUV411.               */
   if ( pMainInst->ulColorEncoding == MMIO_RGB_5_6_5 )

      /* Note that I'm actually calling the key frame compression            */
      /* routine instead of the delta frame compression routine.  If you     */
      /* are supporting delta frame compression, yours goes here.            */
      return ( CompressKeyFromRGB565 ( pbSrcBuffer, pbDstBuffer,
                      pMainInst->ulWidth, pMainInst->ulHeight ) );
   else

      /* Ditto, this is in place of an actual delta frame compression.       */
      return ( CompressKeyFromYUV411 ( pbSrcBuffer, pbDstBuffer,
                      pMainInst->ulWidth, pMainInst->ulHeight ) );
   }




/* Following is the defines for the two most popular                         */
/* source formats, RGB16 and YUV411.                                         */

#pragma data_seg (PROCESS_SHARED_DATA_SEGMENT)

/* RGB16 (RGB565) format:  loaded as a WORD, each pixel looks like:          */
/*    R4:R3:R2:R1:R0:G5:G4:G3 G2:G1:G0:B4:B3:B2:B1:B0                        */

/* YUV411 format: loaded as four consecutive WORDs, each word looks like:    */
/*    Cb6:Cb5:Cr6:Cr5:???:???:???:???  Ya6:Ya5:Ya4:Ya3:Ya2:Ya1:Ya0:???       */
/*    Cb4:Cb3:Cr4:Cr3:???:???:???:???  Yb6:Yb5:Yb4:Yb3:Yb2:Yb1:Yb0:???       */
/*    Cb2:Cb1:Cr2:Cr1:???:???:???:???  Yc6:Yc5:Yc4:Yc3:Yc2:Yc1:Yc0:???       */
/*    Cb0:???:Cr0:???:???:???:???:???  Yd6:Yd5:Yd4:Yd3:Yd2:Yd1:Yd0:???       */
/*                                            ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          */
/*                                            ³        Cr         ³          */
/*                                            ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´          */
/* Where the four pixels in a row look like:  ³        Cb         ³          */
/*                                            ÃÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄ´          */
/*                                            ³ Ya ³ Yb ³ Yc ³ Yd ³          */
/*                                            ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ          */





/* Following is two tables that assist in the conversion of RGB565 to Y8.    */
/* To get luminance, you look up the first byte RRRRRGGG in bLookYh to get   */
/* part of the luminance information and add it to the second byte GGGBBBBB  */
/* in bLookYl to get the complete luminance value in the range {0..255}.     */

BYTE bLookYh[256]= {
             0,  19,  38,  57,  77,  96, 115, 134,
             2,  22,  41,  60,  79,  98, 117, 136,
             5,  24,  43,  62,  81, 101, 120, 139,
             7,  26,  46,  65,  84, 103, 122, 141,
            10,  29,  48,  67,  86, 105, 125, 144,
            12,  31,  50,  70,  89, 108, 127, 146,
            15,  34,  53,  72,  91, 110, 129, 149,
            17,  36,  55,  74,  94, 113, 132, 151,
            19,  39,  58,  77,  96, 115, 134, 153,
            22,  41,  60,  79,  98, 118, 137, 156,
            24,  43,  63,  82, 101, 120, 139, 158,
            27,  46,  65,  84, 103, 122, 142, 161,
            29,  48,  68,  87, 106, 125, 144, 163,
            32,  51,  70,  89, 108, 127, 146, 166,
            34,  53,  72,  92, 111, 130, 149, 168,
            37,  56,  75,  94, 113, 132, 151, 170,
            39,  58,  77,  96, 116, 135, 154, 173,
            41,  61,  80,  99, 118, 137, 156, 175,
            44,  63,  82, 101, 120, 140, 159, 178,
            46,  65,  85, 104, 123, 142, 161, 180,
            49,  68,  87, 106, 125, 144, 164, 183,
            51,  70,  89, 109, 128, 147, 166, 185,
            54,  73,  92, 111, 130, 149, 168, 188,
            56,  75,  94, 113, 133, 152, 171, 190,
            58,  78,  97, 116, 135, 154, 173, 192,
            61,  80,  99, 118, 137, 157, 176, 195,
            63,  82, 102, 121, 140, 159, 178, 197,
            66,  85, 104, 123, 142, 161, 181, 200,
            68,  87, 106, 126, 145, 164, 183, 202,
            71,  90, 109, 128, 147, 166, 185, 205,
            73,  92, 111, 130, 150, 169, 188, 207,
            76,  95, 114, 133, 152, 171, 190, 209 };

BYTE bLookYl[256]= {
             0,   1,   2,   3,   4,   5,   6,   7,
             7,   8,   9,  10,  11,  12,  13,  14,
            15,  16,  17,  18,  19,  20,  20,  21,
            22,  23,  24,  25,  26,  27,  28,  29,
             2,   3,   4,   5,   6,   7,   8,   9,
            10,  11,  12,  13,  14,  14,  15,  16,
            17,  18,  19,  20,  21,  22,  23,  24,
            25,  26,  27,  27,  28,  29,  30,  31,
             5,   6,   7,   8,   8,   9,  10,  11,
            12,  13,  14,  15,  16,  17,  18,  19,
            20,  21,  22,  22,  23,  24,  25,  26,
            27,  28,  29,  30,  31,  32,  33,  34,
             7,   8,   9,  10,  11,  12,  13,  14,
            15,  16,  16,  17,  18,  19,  20,  21,
            22,  23,  24,  25,  26,  27,  28,  29,
            29,  30,  31,  32,  33,  34,  35,  36,
            10,  10,  11,  12,  13,  14,  15,  16,
            17,  18,  19,  20,  21,  22,  23,  24,
            24,  25,  26,  27,  28,  29,  30,  31,
            32,  33,  34,  35,  36,  37,  37,  38,
            12,  13,  14,  15,  16,  17,  18,  18,
            19,  20,  21,  22,  23,  24,  25,  26,
            27,  28,  29,  30,  31,  31,  32,  33,
            34,  35,  36,  37,  38,  39,  40,  41,
            14,  15,  16,  17,  18,  19,  20,  21,
            22,  23,  24,  25,  25,  26,  27,  28,
            29,  30,  31,  32,  33,  34,  35,  36,
            37,  38,  39,  39,  40,  41,  42,  43,
            17,  18,  19,  20,  20,  21,  22,  23,
            24,  25,  26,  27,  28,  29,  30,  31,
            32,  33,  33,  34,  35,  36,  37,  38,
            39,  40,  41,  42,  43,  44,  45,  46 };

#pragma data_seg ()

/***************************************************************************\
*
* Module Name: DIVE.H
*
* OS/2 2.1 Multimedia Extensions Display Engine API data structures
*
* Copyright (c) International Business Machines Corporation 1993, 1994
*                         All Rights Reserved
*
*
\****************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _DIVE_H_
#define _DIVE_H_

#define MAX_DIVE_INSTANCES    64

#define FOURCC ULONG
#define HDIVE ULONG

#define DIVE_SUCCESS                                     0x00000000
#define DIVE_ERR_INVALID_INSTANCE                        0x00001000
#define DIVE_ERR_SOURCE_FORMAT                           0x00001001
#define DIVE_ERR_DESTINATION_FORMAT                      0x00001002
#define DIVE_ERR_BLITTER_NOT_SETUP                       0x00001003
#define DIVE_ERR_INSUFFICIENT_LENGTH                     0x00001004
#define DIVE_ERR_TOO_MANY_INSTANCES                      0x00001005
#define DIVE_ERR_NO_DIRECT_ACCESS                        0x00001006
#define DIVE_ERR_NOT_BANK_SWITCHED                       0x00001007
#define DIVE_ERR_INVALID_BANK_NUMBER                     0x00001008
#define DIVE_ERR_FB_NOT_ACQUIRED                         0x00001009
#define DIVE_ERR_FB_ALREADY_ACQUIRED                     0x0000100a
#define DIVE_ERR_ACQUIRE_FAILED                          0x0000100b
#define DIVE_ERR_BANK_SWITCH_FAILED                      0x0000100c
#define DIVE_ERR_DEACQUIRE_FAILED                        0x0000100d
#define DIVE_ERR_INVALID_PALETTE                         0x0000100e
#define DIVE_ERR_INVALID_DESTINATION_RECTL               0x0000100f
#define DIVE_ERR_INVALID_BUFFER_NUMBER                   0x00001010
#define DIVE_ERR_SSMDD_NOT_INSTALLED                     0x00001011
#define DIVE_ERR_BUFFER_ALREADY_ACCESSED                 0x00001012
#define DIVE_ERR_BUFFER_NOT_ACCESSED                     0x00001013
#define DIVE_ERR_TOO_MANY_BUFFERS                        0x00001014
#define DIVE_ERR_ALLOCATION_ERROR                        0x00001015
#define DIVE_ERR_INVALID_LINESIZE                        0x00001016
#define DIVE_ERR_FATAL_EXCEPTION                         0x00001017
#define DIVE_ERR_INVALID_CONVERSION                      0x00001018
#define DIVE_ERR_VSD_ERROR                               0x00001019
#define DIVE_ERR_COLOR_SUPPORT                           0x0000101a
#define DIVE_ERR_OUT_OF_RANGE                            0x0000101b
#define DIVE_WARN_NO_SIZE                                0x00001100

#define DIVE_BUFFER_SCREEN                               0x00000000
#define DIVE_BUFFER_GRAPHICS_PLANE                       0x00000001
#define DIVE_BUFFER_ALTERNATE_PLANE                      0x00000002

#define DIVE_FULLY_VISIBLE                               0xffffffff


/* Notes:
      Associated/Allocated memory buffers start at:      0x00000010

      Specifing DIVE_BUFFER_GRAPHICS_PLANE results in the image being
            transferred to the graphics plane.
      Specifing DIVE_BUFFER_ALTERNATE_PLANE results in the image being
            transferred to the alternate (e.g. overlay) plane.  If your
            hardware doesn't support such a plane, this is an error.
      Specifing DIVE_BUFFER_SCREEN will result in the image being
            transferred to either the graphics plane buffer or the alternate
            plane buffer based on if an alternate buffer exists and based on
            the suitability the overlay plane to accelerate the scaling of
            the image.  If DIVE chooses to use the alternate buffer, it
            will also paint the overlay "key" color on the graphics plane.
            This automatic painting does not occur if the alternate plane
            is explicitly specified.
*/



typedef struct _DIVE_CAPS {

   ULONG  ulStructLen;            /* Set equal to sizeof(DIVE_CAPS)          */
   ULONG  ulPlaneCount;           /* Number of defined planes.               */

   /* Info returned in the following fields pertains to ulPlaneID.           */
   BOOL   fScreenDirect;          /* TRUE if can get addressability to vram. */
   BOOL   fBankSwitched;          /* TRUE if vram is bank-switched.          */
   ULONG  ulDepth;                /* Number of bits per pixel.               */
   ULONG  ulHorizontalResolution; /* Screen width in pixels.                 */
   ULONG  ulVerticalResolution;   /* Screen height in pixels.                */
   ULONG  ulScanLineBytes;        /* Screen scan line size in bytes.         */
   FOURCC fccColorEncoding;       /* Colorspace encoding of the screen.      */
   ULONG  ulApertureSize;         /* Size of vram aperture in bytes.         */

   ULONG  ulInputFormats;         /* Number of input color formats.          */
   ULONG  ulOutputFormats;        /* Number of output color formats.         */
   ULONG  ulFormatLength;         /* Length of format buffer.                */
   PVOID  pFormatData;            /* Pointer to color format buffer FOURCC's.*/

   } DIVE_CAPS;
typedef DIVE_CAPS *PDIVE_CAPS;




typedef struct _SETUP_BLITTER {

     /* Set the ulStructLen field equal to the amount of the structure used. */
     /* allowable: blank lines below mark sizes of 8, 28, 32, 52, 60, or 68. */
   ULONG  ulStructLen;
     /* Set the ulInvert flags based on the following:                       */
     /* b0001 = d01 = h01 = flip the image in the horizontal direction.      */
     /* b0010 = d02 = h02 = flip the image in the vertical direction.        */
     /* All other bits ignored.                                              */
   ULONG  fInvert;

     /* This is the color format of the source data.  See "FOURCC.H"         */
   FOURCC fccSrcColorFormat;
     /* This is the width of the source image in pixels.                     */
   ULONG  ulSrcWidth;
     /* This is the height of the source image in pixels.                    */
   ULONG  ulSrcHeight;
     /* This is the horizontal offset from which to start displaying for     */
     /* use in displaying a sub-portion of the source image.                 */
   ULONG  ulSrcPosX;
     /* This is the vertical offset from which to start displaying.          */
   ULONG  ulSrcPosY;

     /* This is the dither type to use.  0 defines no dither and 1           */
     /* defines 2x2 dither (all others ignored).  Note: dithering is only    */
     /* supported in direct color to LUT8 conversions.                       */
   ULONG  ulDitherType;

     /* This is the color format of the destinaion data.  See "FOURCC.H"     */
   FOURCC fccDstColorFormat;
     /* This is the width of the destination image in pixels.                */
   ULONG  ulDstWidth;
     /* This is the height of the destination image in pixels.               */
   ULONG  ulDstHeight;
     /* This is the horizontal offset from which to start displaying for     */
     /* use in displaying to sub-portion of the destination image.           */
   LONG   lDstPosX;
     /* This is the vertical offset from which to start displaying.          */
   LONG   lDstPosY;

     /* This is the world screen horizontal position, where 0 is left.       */
     /* These are ignored if the destination is not the screen.              */
   LONG   lScreenPosX;
     /* This is the world screen vertical position, where 0 is bottom.       */
   LONG   lScreenPosY;

     /* This is the number of visible rectangular regions being passed in.   */
     /* These are ignored if the destination is not the screen.              */
     /* Also, if you application *KNOWS* that the region is fully visible    */
     /* (like not going to the screen), the you can use DIVE_FULLY_VISIBLE   */
     /* instead of making up a bogus visible region structure.               */
   ULONG  ulNumDstRects;
     /* This points to an array of visible regions which defines what        */
     /* portions of the source image are to be displayed.                    */
   PRECTL pVisDstRects;           /* Pointer to array of visible rectangles. */

   } SETUP_BLITTER;
typedef SETUP_BLITTER *PSETUP_BLITTER;



ULONG APIENTRY DiveQueryCaps ( PDIVE_CAPS pDiveCaps,
                               ULONG      ulPlaneBufNum );

ULONG APIENTRY DiveOpen ( HDIVE *phDiveInst,
                          BOOL   fNonScreenInstance,
                          PVOID  ppFrameBuffer );

ULONG APIENTRY DiveSetupBlitter ( HDIVE          hDiveInst,
                                  PSETUP_BLITTER pSetupBlitter );

ULONG APIENTRY DiveBlitImage ( HDIVE hDiveInst,
                               ULONG ulSrcBufNumber,
                               ULONG ulDstBufNumber );

#ifdef INCL_MM_OS2
/* Same as DiveBlitImage, except pbLineMask points to one byte per line      */
/* in source image buffer such that 0: unchanged, 0xFF: changed              */
ULONG APIENTRY DiveBlitImageLines ( HDIVE hDiveInst,
                                    ULONG ulSrcBufNumber,
                                    ULONG ulDstBufNumber,
                                    PBYTE pbLineMask );
#endif

ULONG APIENTRY DiveClose ( HDIVE hDiveInst );

ULONG APIENTRY DiveAcquireFrameBuffer ( HDIVE   hDiveInst,
                                        PRECTL  prectlDst );

ULONG APIENTRY DiveSwitchBank ( HDIVE hDiveInst,
                                ULONG ulBankNumber );

ULONG APIENTRY DiveDeacquireFrameBuffer ( HDIVE hDiveInst );

ULONG APIENTRY DiveCalcFrameBufferAddress ( HDIVE  hDiveInst,
                                            PRECTL prectlDest,
                                            PBYTE *ppDestinationAddress,
                                            PULONG pulBankNumber,
                                            PULONG pulRemLinesInBank );

/* Notes on DiveAllocImageBuffer:
      If pbImageBuffer is not NULL, the buffer is associated rather than
      allocated.  If pbImageBuffer is not NULL and the buffer number
      pointed to by pulBufferNumber is non-zero, a new buffer pointer is
      associated with the buffer number.  Even though no memory is
      allocated by DiveAllocImageBuffer when user-allocated buffers are
      associated, DiveFreeImageBuffer should be called to release the
      buffer association to avoid using up available buffer indexes.
      The specified line size will be used if a buffer is allocated in
      system memory, or if a user buffer is associated.  If the
      specified line size is zero, the allocated line size is rounded up
      to the nearest DWORD boundry.
*/

ULONG APIENTRY DiveAllocImageBuffer ( HDIVE  hDiveInst,
                                      PULONG pulBufferNumber,
                                      FOURCC fccColorSpace,
                                      ULONG  ulWidth,
                                      ULONG  ulHeight,
                                      ULONG  ulLineSizeBytes,
                                      PBYTE  pbImageBuffer );

ULONG APIENTRY DiveFreeImageBuffer ( HDIVE hDiveInst,
                                     ULONG ulBufferNumber );

ULONG APIENTRY DiveBeginImageBufferAccess ( HDIVE  hDiveInst,
                                            ULONG  ulBufferNumber,
                                            PBYTE *ppbImageBuffer,
                                            PULONG pulBufferScanLineBytes,
                                            PULONG pulBufferScanLines );

ULONG APIENTRY DiveEndImageBufferAccess ( HDIVE hDiveInst,
                                          ULONG ulBufferNumber );



/* Notes on palettes:
      Neither DiveSetSourcePalette nor DiveSetDestinationPalette API's will set
      the physical palette.  If your application MUST set the PHYSICAL palette,
      try using no more than 236 entries (the middle 236: 10-245, thus leaving
      the top and bottom 10 entries for the Workplace Shell).  If your
      application MUST use ALL 256 entries, it must do so as a full-screen
      (i.e. maximized) application.  Remember, No WM_REALIZEPALETTE message
      will be sent to other running applications, meaning they will not redraw
      and their colors will be all wrong.  It is not recommended that a
      developer use these commands:

   To set physical palette, do the following:
            hps = WinGetPS ( HWND_DESKTOP );
            hdc = GpiQueryDevice ( hps );
            GpiCreateLogColorTable ( hps, LCOL_PURECOLOR | LCOL_REALIZABLE,
                           LCOLF_CONSECRGB, 0, 256, (PLONG)plRGB2Entries );
            Gre32EntrY3 ( hdc, 0L, 0x000060C6L );
            WinInvalidateRect ( HWND_DESKTOP, (PRECTL)NULL, TRUE );
            WinReleasePS ( hps );

   To reset physical palette, do the following:
            hps = WinGetPS ( HWND_DESKTOP );
            hdc = GpiQueryDevice ( hps );
            Gre32EntrY3 ( hdc, 0L, 0x000060C7L );
            WinInvalidateRect ( HWND_DESKTOP, (PRECTL)NULL, TRUE );
            WinReleasePS ( hps );
*/


/* Use either of the two defines as the pRGB2Entries pointer to have DIVE    */
/* query and set the physical or default palette as source or destination.   */

#define DIVE_PALETTE_PHYSICAL                     (PBYTE)0x00000000
#define DIVE_PALETTE_DEFAULT                      (PBYTE)0xffffffff

ULONG APIENTRY DiveSetDestinationPalette ( HDIVE hDiveInst,
                                           ULONG ulStartIndex,
                                           ULONG ulNumEntries,
                                           PBYTE pbRGB2Entries );

ULONG APIENTRY DiveSetSourcePalette ( HDIVE hDiveInst,
                                      ULONG ulStartIndex,
                                      ULONG ulNumEntries,
                                      PBYTE pbRGB2Entries );

#ifdef INCL_MM_OS2
ULONG APIENTRY DiveSetTransparentBlitMode ( HDIVE hDiveInst,
                                            ULONG ulTransBlitMode,
                                            ULONG ulValue1,
                                            ULONG ulValue2 );

/* The following transparent blitting modes are supported:                   */

#define DIVE_TBM_NONE                              0x0
/* No transparency, i.e. all pixels are transferred (default)                */

#define DIVE_TBM_EXCLUDE_SOURCE_VALUE              0x01
/* Source pixels with values that exactly match the value specified in       */
/* ulValue1 are not transferred.                                             */

#define DIVE_TBM_EXCLUDE_SOURCE_RGB_RANGE          0x02
/* Source pixels with values that lie within the range specified in RGB      */
/* color space specified by ulValue1 (minimum) and ulValue2 (maximum)        */
/* are not transferred by DiveBlitImage.                                     */

#define DIVE_TBM_INCLUDE_SOURCE_RGB_RANGE          0x03
/* Source pixels with values that lie outside the range specified in RGB     */
/* color space specified by ulValue1 (minimum) and ulValue2 (maximum)        */
/* are not transferred by DiveBlitImage.                                     */

#define DIVE_TBM_EXCLUDE_SOURCE_YUV_RANGE          0x04
/* Source pixels with values that lie within the range specified in RGB      */
/* color space specified by ulValue1 (minimum) and ulValue2 (maximum)        */
/* are not transferred by DiveBlitImage.                                     */

#define DIVE_TBM_INCLUDE_SOURCE_YUV_RANGE          0x05
/* Source pixels with values that lie outside the range specified in RGB     */
/* color space specified by ulValue1 (minimum) and ulValue2 (maximum)        */
/* are not transferred by DiveBlitImage.                                     */

/* Notes on transparent blitting:
   Supported transparent blitting functions are all based on source pixel
   values.  A pixel in the destination image buffer is not modified if the
   corresponding pixel in the source buffer is "transparent".  The color
   values or color value ranges specified in ulValue1 and ulValue2 are
   dependent on the source image color format (fccSrcColorFormat) and the
   color space in which the range comparison is taking place.

   FOURCC_LUT8:
      The color value is specified in the low 8 bits of parameter

   FOURCC_Y888, FOURCC_Y2X2, FOURCC_Y4X4, FOURCC_YUV9, FOURCC_Y644, FOURCC_Y422:
      23:8 - Y, 15:8 - U, 7:8 - V  (bits 31:8 ignored)

   FOURCC_R565, FOURCC_R555, FOURCC_R664, FOURCC_RGB3, FOURCC_BGR3, FOURCC_RGB4,
   FOURCC_BGR4:
      23:8 - R, 15:8 - G, 7:8 - B  (bits 31:8 ignored)
      R, G, and B components are specified with 8 bit significance, regardless
      of significance in the source data.

   Transparent blitting of other source image formats is not supported.

   For range comparisons in RGB or YUV, the three components are compared
   independently against the minimum and maximum values specified by the
   ulValue1 and ulValue2 parameters respectively.  A value is considered to
   within the specified range if all three components satisfy:
   min <= value <= max.

   For EXCLUDE_SOURCE_VALUE tranparent blitting, the specified value in
   ulValue1 is assumed to be in the source color space as described above.
   For range comparisons, the values specified in ulValue1 and ulValue2 are
   assumed to be in the color space in which the range comparison is to be
   performed, either YUV or RGB.

   - For range comparisons in YUV where the source data format is a YUV
     form, the values in ulValue1 and ulValue2 are in the source YUV
     format.

   - For range comparisons in YUV where the source data format is RGB,
     conversion of source data format from RGB to YUV using standard
     CCIR601 equations is assumed (refer to fourcc.h).

   - For range comparisons in RGB where the source data format is an RGB
     form, the values in ulValue1 and ulValue2 specify the RGB range with
     5 bits significance in R, 6 bits in G, and 5 bits in B.

   - For range comparisons in RGB where the source data format is YUV,
     conversion of source data format from YUV to RGB using standard
     CCIR601 equations is assumed (refer to fourcc.h).



*/

#endif

#endif
#ifdef __cplusplus
}
#endif


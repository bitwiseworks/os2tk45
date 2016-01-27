/********************** START OF SPECIFICATIONS *************************/
/*                                                                      */
/* SOURCE FILE NAME: MMOTFUNC.C                                        */
/*                                                                      */
/* DESCRIPTIVE NAME: Multi-Media I/O Procedure for M-Motion Video       */
/*                                                                      */
/* COPYRIGHT:                                                           */
/*              Copyright (c) IBM Corporation  1990, 1993               */
/*                        All Rights Reserved                           */
/*                                                                      */
/* ABSTRACT: This file contains misc functions for use with the         */
/*           M-Motion IOProc (file MMOT.C)                              */
/*                                                                      */
/*********************** END OF SPECIFICATIONS **************************/
/************************************************************************/
/* Put all #defines here                                                */
/************************************************************************/

#define INCL_32                          * force 32 bit compile
#define INCL_GPIBITMAPS
#define INCL_DOSFILEMGR
#define INCL_DOSRESOURCES
#define INCL_DOSMODULEMGR
#define INCL_WIN
#define INCL_GPI
#define INCL_PM                         /* force 32 bit compile */

/************************************************************************/
/* Put all #includes here                                               */
/************************************************************************/

#include <limits.h>
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <os2medef.h>
#include <mmioos2.h>
#include "mmotproc.h"


int _CRT_init( VOID );

/************************************************************************
 * Function : GetYUV
 * Description :
 *   This function takes 3 raw YUV values and creates the 6 discrete
 *   Y, U, and V values necessary to convert to RGB.
 ************************************************************************/
void GetYUV (PYUV pYuv,         /* Pointer to packed YUV buffer   */
             SHORT Y[],         /* Separated Y components         */
             SHORT *U1,         /* Shared U component             */
             SHORT *V1)         /* Shared V component             */
    {
    Y[0] = (SHORT)((pYuv->YUVWord1 & 0x0FE0) >> 4);
    Y[1] = (SHORT)((pYuv->YUVWord2 & 0xFE00) >> 8);
    Y[2] = (SHORT)(((pYuv->YUVWord2 & 0x000F) << 4) +
               ((pYuv->YUVWord3 & 0xE000) >> 12));
    Y[3] = (SHORT)(pYuv->YUVWord3 & 0x00FE);

    *U1 = (SHORT)(((pYuv->YUVWord1 & 0xC000) >> 8) +
                  ((pYuv->YUVWord1 & 0x000C) << 2) +
                  ((pYuv->YUVWord2 & 0x00C0) >> 4) +
                 ((pYuv->YUVWord3 & 0x0800) >> 10));

    *V1 = (SHORT)(((pYuv->YUVWord1 & 0x3000) >> 6) +
                  ((pYuv->YUVWord1 & 0x0003) << 4) +
                  ((pYuv->YUVWord2 & 0x0030) >> 2) +
                  ((pYuv->YUVWord3 & 0x0200) >> 8));

    if (*U1 > 127)
        *U1 -= 256;
    if (*V1 > 127)
        *V1 -= 256;
    }

/************************************************************************
 * Function : YUVtoRGB
 * Description:
 *    This function takes 6 discrete YUV values and processes it into 4
 *    24 bpp RGB pixels.  It also requires an address to store the processed
 *    RGB values.
 ************************************************************************/
void YUVtoRGB (SHORT Y[],       /* Array of 4 Y values, 1 per pel             */
               SHORT U1,        /* Shared U value                             */
               SHORT V1,        /* Shared V value                             */
               PBYTE lpRGBBuf)  /* Pointer to RGB buffer where 4 pels will go */
    {
    /********************************************************************
     * Declare Local Variables.
     ********************************************************************/
    USHORT          i;
    LONG            lR, lG, lB;

    /********************************************************************
     * Processing and writing out 4 pel block.
     ********************************************************************/
    for (i = 0; i < 4; i++)
        {
        /****************************************
         * no saturation accounted for.
         *  lR = (V1 * 179L / 127L) + Y[i];
         *  lB = (U1 * 227L / 127L) + Y[i];

         * Calculate Red and Blue values...

         * R = (V * 179/127 * 80%) + Y
         *   = (V * 179/127 * 8/10) + Y
         *   = (V * 1432/1270) + Y
         ****************************************/
        lR = (((LONG)V1 * 179L) / 127L) + Y[i];

        /****************************************
         * B = (U * 227/127 * 80%) + Y
         *   = (U * 227/127 * 8/10) + Y
         *   = (U * 1816/1270) + Y
         ****************************************/
        lB = (((LONG)U1 * 227L) / 127L) + Y[i];

        lG = (Y[i]*170L - lR*51L - lB*19L) / 100L;

        /****************************************************************
         * Determine the BLUE byte and put it into buffer.
         ****************************************************************/
        if (lB>255L)
            *lpRGBBuf++=255;
        else if (lB<0L)
            *lpRGBBuf++=0;
        else
            *lpRGBBuf++=(BYTE)lB;

        /****************************************************************
         * Determine the GREEN byte and put it into buffer.
         ****************************************************************/
        if (lG>255L)
            *lpRGBBuf++=255;
        else if (lG<0L)
            *lpRGBBuf++=0;
        else
            *lpRGBBuf++=(BYTE)lG;

        /****************************************************************
         * Determine the RED byte and put it into buffer.
         ****************************************************************/
        if (lR>255L)
            *lpRGBBuf++=255;
        else if (lR<0L)
            *lpRGBBuf++=0;
        else
            *lpRGBBuf++=(BYTE)lR;
        }
    }

/************************************************************************
 * Function : RGBtoYUV
 * Description:
 *    Take twelve bytes of RGB data (4 pels) and use them to create the
 *    6 discrete Y, U, and V values needed to build the 4 pixels (6 bytes)
 *    of YUV data.
 ************************************************************************/
void RGBtoYUV (SHORT YO[],      /* Array of 4 Y values to be created    */
               SHORT *U1,       /* Shared U value to be created         */
               SHORT *V1,       /* Shared V value to be created         */
               PBYTE lpRGBBuf)  /* Pointer to RGB buf holding 4 pels    */
    {
    /********************************************************************
     * Declare Local Variables.
     ********************************************************************/
    PRGB4       prgb4;

    /********************************************************************
     * Set up and cast rgb pointer to correct type.
     ********************************************************************/
    prgb4 = (PRGB4) lpRGBBuf;

    /********************************************************************
     * Calculate Y, U, and V values.
     ********************************************************************/

    YO[0] = (SHORT)(((100 * (USHORT)prgb4->bG1) +
             (51  * (USHORT)prgb4->bR1) +
             (19  * (USHORT)prgb4->bB1)) / 170);
    YO[1] = (SHORT)(((100 * (USHORT)prgb4->bG2) +
             (51  * (USHORT)prgb4->bR2) +
             (19  * (USHORT)prgb4->bB2)) / 170);
    YO[2] = (SHORT)(((100 * (USHORT)prgb4->bG3) +
             (51  * (USHORT)prgb4->bR3) +
             (19  * (USHORT)prgb4->bB3)) / 170);
    YO[3] = (SHORT)(((100 * (USHORT)prgb4->bG4) +
             (51  * (USHORT)prgb4->bR4) +
             (19  * (USHORT)prgb4->bB4)) / 170);

    /************************************************************
     * Calculate the shared U value - it is the average of the
     *   U values of each of the 4 pels
     * U = (B - Y) * 127/227
     ************************************************************/
    *U1 = (SHORT)((((((LONG)prgb4->bB1 - YO[0])*127)/227)+
                   ((((LONG)prgb4->bB2 - YO[1])*127)/227)+
                   ((((LONG)prgb4->bB3 - YO[2])*127)/227)+
                   ((((LONG)prgb4->bB4 - YO[3])*127)/227)) >>2);

    /************************************************************
     * Calculate the shared V value - it is the average of the
     *   V values of each of the 4 pels
     * V = (R - Y) * 127/179
     ************************************************************/
    *V1 = (SHORT)((((((LONG)prgb4->bR1 - YO[0])*127)/179)+
                   ((((LONG)prgb4->bR2 - YO[1])*127)/179)+
                   ((((LONG)prgb4->bR3 - YO[2])*127)/179)+
                   ((((LONG)prgb4->bR4 - YO[3])*127)/179)) >>2);
    }

/************************************************************************
 * Function : PutYUV
 * Description:
 *    This function will take an array of 4 Y values and a separate U and
 *    V value and process it into 3 packed USHORT YUV values (6 bytes).
 ************************************************************************/
void PutYUV (PBYTE lpTempBuf,   /* Packed YUV buffer to place data into */
             SHORT YO[],        /* Array of 4 Y values to use           */
             SHORT U1,          /* Shared U value                       */
             SHORT V1)          /* Shared V value                       */
    {
    /********************************************************************
     * Declare Local Variables.
     ********************************************************************/
    USHORT          YUVWord1, YUVWord2, YUVWord3;

    /********************************************************************
     * Breakdown 6 values into (3) USHORTs
     ********************************************************************/
    YUVWord1 = (USHORT)(((U1 & 0x00C0) << 8) +
               ((V1 & 0x00C0) << 6) +
               (YO[0] << 4) +
               ((U1 & 0x0030) >> 2) +
               ((V1 & 0x0030) >> 4));

    YUVWord2 = (USHORT)((YO[1] << 8) +
               ((U1 & 0x000C) << 4) +
               ((V1 & 0x000C) << 2) +
               ((YO[2] & 0x00F0) >> 4));

    YUVWord3 = (USHORT)(((YO[2] & 0x000E) << 12) +
                         ((U1 & 0x0002) << 10) +
                         ((V1 & 0x0002) << 8) + YO[3]);

    *lpTempBuf++= LOBYTE(YUVWord1);
    *lpTempBuf++= HIBYTE(YUVWord1);
    *lpTempBuf++= LOBYTE(YUVWord2);
    *lpTempBuf++= HIBYTE(YUVWord2);
    *lpTempBuf++= LOBYTE(YUVWord3);
    *lpTempBuf++= HIBYTE(YUVWord3);
    }

/************************************************************************
 * Function : InitFileStruct
 * Description:
 *    This function will reset the fields of the current FileStatus
 *    structure to initialized values.
 ************************************************************************/
void InitFileStruct (PMMFILESTATUS pVidInfo)  /* MMotion IOProc data struct */

    {
    pVidInfo->lpRGBBuf                         = 0L;
    pVidInfo->lpImgBuf                         = 0L;
    pVidInfo->lImgBytePos                      = 0L;
    pVidInfo->ulRGBTotalBytes                  = 0L;
    pVidInfo->ulImgTotalBytes                  = 0L;
    pVidInfo->mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount    = 24L;
    pVidInfo->ulFlags                          = 0L;
    pVidInfo->bSetHeader                       = FALSE;
    }

/************************************************************************
 * Function : RGBBytesToPels
 * Description :
 *    This function will return the number of pels that the input parameter
 *    bytes maps out to.
 ************************************************************************/
ULONG RGBBytesToPels (ULONG ulBytes,    /* Number of bytes to evaluate        */
                      USHORT usBitCount) /* Number of bits/pel for these bytes */
    {

    switch (usBitCount)
        {
        case 24:
            return (ulBytes / 3);   /* 1 pel represented by 3 bytes */
        case 8:
            return (ulBytes);       /* 1 pel represented by 1 byte  */
        case 4:
            return (ulBytes << 1);  /* 1 pel represented by 4 bits. */
                                    /* Bytes multiplied by 2        */
        case 1:
            return (ulBytes << 3);  /* 1 pel represented by 1 bit.  */
                                    /* Bytes multiplied by 8        */

        default:
            return( 0L );
        }

    }

/************************************************************************
 * Function : PelsToRGBBytes
 * Description :
 *    This function will return the number of bytes that the input parameter
 *    pels maps out to.
 ************************************************************************/
ULONG PelsToRGBBytes (ULONG ulPels,     /* Number of pels to evaluate   */
                      USHORT usBitCount) /* Number of bits/pel          */
    {

    switch (usBitCount)
        {
        case 24:
            return (ulPels * 3);    /* 1 pel represented by 3 bytes     */
        case 8:
            return (ulPels);        /* 1 pel represented by 1 byte      */
        case 4:
            return (ulPels  >> 1);  /* 1 pel represented by 4 bits.     */
                                    /* Pels divided by 2                */
        case 1:
            return (ulPels  >> 3);  /* 1 pel represented by 1 bit.      */
                                    /* Pels divided by 8                */

        default:
            return( 0L );
        }
    }

/************************************************************************
 * Function : Convert1BitTo24Bit
 * Description:
 *    This function will convert a buffer of 1 bit palettized data
 *    into 24 bit RGB data.
 ************************************************************************/
void Convert1BitTo24Bit (PBYTE  lpInputPtr,   /* Ptr to 1 bpp data to convert */
                         PRGB   RGBBufPtr,    /* Ptr to RGB buffer to write to*/
                         PRGB   Palette,      /* Palette to use for 1 bpp     */
                         ULONG  ulInputBytes) /* # of bytes to process        */
    {
    ULONG       ulPalValues;
    ULONG       ulByteCnt = 0;

    /********************************************************************
     * Loop through each byte in the 1 bpp source.  Each bit is one pel's
     *    palette reference.
     * Obtain the RGB data for each pel, place the RGB info into the
     *    the RGB buffer.
     ********************************************************************/
    for (ulByteCnt = 0; ulByteCnt < ulInputBytes; ulByteCnt++)
        {
        /****************************************************************
         * Get the current palette index.
         ****************************************************************/
        ulPalValues = (ULONG)lpInputPtr[ulByteCnt];

        /****************************************************************
         * Advance through the RGBLine pointer placing a 3 byte palette
         * value into each element.
         ****************************************************************/
        RGBBufPtr[0] = Palette[(ulPalValues & 0x80) >> 7]; /* Bit #7 */
        RGBBufPtr[1] = Palette[(ulPalValues & 0x40) >> 6]; /* Bit #6 */
        RGBBufPtr[2] = Palette[(ulPalValues & 0x20) >> 5]; /* Bit #5 */
        RGBBufPtr[3] = Palette[(ulPalValues & 0x10) >> 4]; /* Bit #4 */
        RGBBufPtr[4] = Palette[(ulPalValues & 0x08) >> 3]; /* Bit #3 */
        RGBBufPtr[5] = Palette[(ulPalValues & 0x04) >> 2]; /* Bit #2 */
        RGBBufPtr[6] = Palette[(ulPalValues & 0x02) >> 1]; /* Bit #1 */
        RGBBufPtr[7] = Palette[(ulPalValues & 0x01)];      /* Bit #0 */

        /****************************************************************
         * Make sure the pointer is now set 8 pels further so we can
         * come back in again to the correct place.
         ****************************************************************/
        RGBBufPtr += 8;
        }
    }

/************************************************************************
 * Function : Convert4BitTo24Bit
 * Description:
 *    This function will convert a buffer of 4 bit palettized data
 *    into 24 bit RGB data.
 ************************************************************************/
void Convert4BitTo24Bit (PBYTE  lpInputPtr,   /* Ptr to 4 bpp data to convert  */
                         PRGB   RGBBufPtr,    /* Ptr to RGB buffer to write to */
                         PRGB   Palette,      /* Palette to use for 1 bpp      */
                         ULONG  ulInputBytes) /* # of bytes to process         */
    {
    ULONG       ulPalValues;
    ULONG       ulByteCnt = 0;

    /********************************************************************
     * Loop through each pel in the source and look it up in the palette
     * table and then put the looked up value into the position RGBBufPtr.
     ********************************************************************/
    for (ulByteCnt = 0; ulByteCnt < ulInputBytes; ulByteCnt++)

        {
        /****************************************************************
         * Get the current palette index.
         ****************************************************************/
        ulPalValues = (ULONG)lpInputPtr[ulByteCnt];

        /****************************************************************
         * Get first pel (held in xxxx0000, first half of byte)
         ****************************************************************/
        RGBBufPtr[0] = Palette[ulPalValues >> 4];

        /****************************************************************
         * Get second pel (held in 0000xxxx, second half of byte)
         ****************************************************************/
        RGBBufPtr[1] = Palette[ulPalValues++ & 0x0F];

        /****************************************************************
         * Bump RGBBufPtr pointer by 2 and start again.
         ****************************************************************/
        RGBBufPtr += 2;
        }
    }

/************************************************************************
 * Function : Convert8BitTo24Bit
 * Description:
 *    This function will convert a buffer of 8 bit palettized data
 *    into 24 bit RGB data.
 ************************************************************************/
void Convert8BitTo24Bit (PBYTE  lpInputPtr,   /* Ptr to 8 bpp data to convert  */
                         PRGB   RGBBufPtr,    /* Ptr to RGB buffer to write to */
                         PRGB   Palette,      /* Palette to use for 1 bpp      */
                         ULONG  ulInputBytes) /* # of bytes to process         */
    {
    BYTE        bPalValue;
    ULONG       ulByteCnt;

    /********************************************************************
     * Loop through each pel in the source and look it up in the palette
     * table and then put the looked up value into the destination.
     ********************************************************************/
    for (ulByteCnt = 0; ulByteCnt < ulInputBytes; ulByteCnt++)

        {
        /****************************************************************
         * Get the current palette index.
         ****************************************************************/
        bPalValue = lpInputPtr[ulByteCnt];

        /****************************************************************
         * Store the one pel and increment pointer for next pass.
         ****************************************************************/
        *RGBBufPtr++ = Palette[bPalValue];
        }
    }

/************************************************************************
 * Function : RGB2_To_RGB
 * Description :
 *    This function will take a RGB2 Palette which is in BGRX order and
 *    convert it to an RGB Palette in BGR order.  It accepts a pointer
 *    to both arrays as well as a number of colors that are in the
 *    original palette.
 *
 *    We will ignore the extra flag byte in the RGB2 data structure at
 *    this time.
 ************************************************************************/
void RGB2_To_RGB (PRGB2     rgb2Palette,     /* B-G-R-flags palette       */
                  PRGB      rgbPalette,      /* B-G-R palette             */
                  USHORT    usNumEntries)    /* # entries in palette to do*/
    {
    USHORT      i;

    /* Go through each array element in the RGB2 palette and set    */
    /* correct values in the RGB palette.                           */
    for (i = 0; i < usNumEntries; i++)
        {
        rgbPalette[i].bRed   = rgb2Palette[i].bRed;
        rgbPalette[i].bGreen = rgb2Palette[i].bGreen;
        rgbPalette[i].bBlue  = rgb2Palette[i].bBlue;
        }
    }

/************************************************************************
 * Function : ConvertOneLineYUVtoRGB
 * Description :
 *    This function takes one line of YUV data and converts it into RGB.
 ************************************************************************/
void ConvertOneLineYUVtoRGB (PBYTE  lpYUVBuf,          /* Pointer to line of YUV */
                             PBYTE  lpRGBBuf,          /* Pointer to line of RGB */
                             ULONG  ulYUVBytesPerLine) /* # bytes to process     */
    {
    ULONG       ulYUVByteCtr;
    PBYTE       lpRGBBufPtr;
    PYUV        pYuv;
    SHORT       Y[4], U1, V1;

    lpRGBBufPtr = lpRGBBuf;
    pYuv = (PYUV) lpYUVBuf;

    for (ulYUVByteCtr = 0;
         ulYUVByteCtr < ulYUVBytesPerLine;
         ulYUVByteCtr += 6)
        {
        /* Change a 3 USHORTs/4 pel YUV Block into separate Y, U & V values. */
        GetYUV (pYuv, Y, &U1, &V1);

        /* Change 4 pels Y, U & V into 4 pels RGB          */
        YUVtoRGB (Y, U1, V1, lpRGBBufPtr);

        /* move to next YUV 4-pel block                    */
        pYuv++;

        /* move lpRGBBufPtr past the 4 pels just created   */
        lpRGBBufPtr += 12;
        }
    }

/************************************************************************
 * Function : ConvertOneLineRGBtoYUV
 * Description :
 *    This function takes one line of RGB data and converts it into YUV.
 ************************************************************************/
void ConvertOneLineRGBtoYUV (PBYTE  lpRGBBuf,      /* Pointer to 1 line RGB */
                             PBYTE  lpYUVBuf,      /* Pointer to 1 line YUV */
                             ULONG  ulYUVPelWidth) /* # bytes to process    */
    {
    ULONG       ulPelCtr;
    PRGB4       prgb4;
    SHORT       Y[4], U1, V1;

    prgb4 = (PRGB4) lpRGBBuf;

    for (ulPelCtr = 0;
         ulPelCtr < ulYUVPelWidth;
         ulPelCtr += 4)

        {
        /* convert 12 bytes RGB to 6 Y, U & V values.     */
        RGBtoYUV (Y, &U1, &V1, (PBYTE) prgb4);

        /* pack 6 Y, U & V values into a 3 USHORT format. */
        PutYUV (lpYUVBuf, Y, U1, V1);

        /* move up lpFullYUVBuf for next block            */
        lpYUVBuf += 6;

        prgb4++;
        }
    }

/************************************************************************
 * Function : ImgBufferFlip
 * Description :
 *    This function will take a full image buffer and reverse the order of
 *    the lines, effectively flipping the image vertically.
 *    This is needed because MMotion stores the YUV image data from the
 *    top down, and PM Bitmaps store data from bottom up.
 *
 *    THE IOPROC IS EXPECTED TO PASS DATA TO, AND ACCEPT DATA FROM THE
 *    APPLICATION IN BOTTOM UP ORDER!!
 ************************************************************************/
BOOL ImgBufferFlip (PBYTE   lpImgBuf,       /* Buffer to flip             */
                    ULONG   ulBytesPerRow,  /* Total bytes - w/ pads, etc */
                    ULONG   ulHeight)       /* Rows high                  */

    {
    PBYTE       lpTempBuf;
    ULONG       ulRowCount;
    ULONG       ulImgTotalBytes;
    ULONG       ulLastLine;
    PBYTE       lpFrom;
    PBYTE       lpTo;

    ulImgTotalBytes = (ulBytesPerRow * ulHeight);
    ulLastLine      = ulImgTotalBytes - ulBytesPerRow;

    if (DosAllocMem ((PPVOID) &lpTempBuf,
                     ulBytesPerRow,
                     fALLOC))

        return (FALSE);

    lpFrom = (PBYTE)lpImgBuf;
    lpTo   = (PBYTE)&lpImgBuf[ulLastLine];

    for (ulRowCount = 0;
         ulRowCount < (ulHeight >> 1);
         ulRowCount++)

        {
        memcpy (lpTempBuf,
                lpFrom,
                ulBytesPerRow);

        memcpy (lpFrom,
                lpTo,
                ulBytesPerRow);

        memcpy (lpTo,
                lpTempBuf,
                ulBytesPerRow);

        lpFrom += ulBytesPerRow;
        lpTo   -= ulBytesPerRow;

        }   /* end of FOR loop   */

    return (TRUE);
    }   /* end of function call  */



/************************************************************************
 * Function : ImgBytesToRGBBytes
 * Description :
 *    This function will convert the number of bytes (lImgBytes) that
 *    is specified relative in a particular color space (usBitCount) into the
 *    number of bytes in 24-bit RGB color space required to represent
 *    the same number of "pels".
 ************************************************************************/
LONG ImgBytesToRGBBytes (LONG lImgBytes,        /* Length of image buffer   */
                         USHORT usBitCount)     /* bits/pel in image buffer */
   {
   switch (usBitCount)
      {
      case 24:
         /* 1 RGB bytes/1 Img byte */
         return (lImgBytes);

      case  8:
         /* 3 RGB bytes/1 Img byte */
         return (lImgBytes * 3);

      case  4:
         /* 6 RGB bytes/1 Img byte */
         return ((lImgBytes << 1) * 3);

      case  1:
         /* 24 RGB bytes/1 Img byte */
         return ((lImgBytes << 3) * 3);

      default:
         return( 0L );

      }
   }

/*************************************************************************************
 * Routine is called on each DLL instantiation and termination.
 * Caller assures that no two instances execute this code
 * at the same time.
 * 00) Change name to _DLL_InitTerm and use the linkage pragma.
 *************************************************************************************/
/*************************************************************************************
 * Variable holds DLL module handle.  Gets set at initialization.
 * Data area is separate for each using process, so this value
 * could be different (or same) per process.
 *************************************************************************************/
HMODULE hModuleHandle;

#pragma linkage (_DLL_InitTerm, system)
/* #pragma checkout (suspend)       /* Prevent unreferenced parameter messages */
ULONG _DLL_InitTerm (ULONG hModHandle, ULONG fTerm, BOOL FirstInstance)
{
   hModuleHandle = hModHandle;  /* Remember for NLS lookup */
   if (!fTerm) {
      if (_CRT_init())
         return (0L);
   } /* endif */

   return (1L);                 /* Success */
} /* DLL_InitTerm */
/* #pragma checkout (resume) */


/**************************************************************************
 *   GetFormatString                                                     **
 **************************************************************************
 *
 * ARGUMENTS:
 *
 *     lSearchId         - Table search key
 *     pszFormatString   - Address where to return string.
 *                         If null, then string is not returned.
 *     lBytes            - Number of bytes to copy.
 *
 * RETURN:
 *
 *     Returns 0 if string not found, or the number of characters (bytes)
 *     copied to the callers buffer.
 *     Note, returned string is not ASCII nul terminated
 *
 * DESCRIPTION:
 *
 *     This function will retrieve the format string for the specified
 *     IOProc from the resource file that contains the strings.
 *
 ***************************************************************************/
LONG GetFormatString (LONG lSearchId,
                      PSZ  pszFormatString,   /* Null, or dest address     */
                      LONG lBytes)            /* Caller provided maximum   */
{
   PVOID   pResourceData;
   CHAR    *pTemp;
   LONG    lStringLen;      /* Length of format string  */
   LONG    lRetVal = 0;     /* Function return value    */
   LONG    lSearchTemp;

   if (DosGetResource(hModuleHandle,
                      RT_RCDATA,
                      MMOTION_IOPROC_NAME_TABLE,
                      &pResourceData))
      {
      return (MMIO_ERROR);
      }

   /*
    * The resource table is of the form : FOURCC String\0
    * Loop until a match is found, then return the string.
    * Copy up to the number of bytes specified.
    */

   lStringLen = 0;
   pTemp = (CHAR *)pResourceData;

   while (pTemp)
      {
      memmove(&lSearchTemp, pTemp, sizeof(LONG));

      if (lSearchTemp == 0L)
         {
         break;  /* End of table, search item not found */
         }

      if (lSearchTemp == lSearchId)   /* Search item found?               */
         {
         pTemp += sizeof(LONG);       /* Bypass numeric search id         */
         lStringLen = strlen(pTemp);  /* Determine length of found string */
         if (lStringLen >= lBytes)    /* Truncate, if necessary           */
            {
            if (lBytes > 0)
               lStringLen = lBytes;   /* Number of characters to return   */
            else
               {
               /* Callers buffer has zero size.  Cannot return any data   */
               lRetVal = 0;           /* Size of returned data            */
               break;                 /* Done searching                   */
               }
            }
         if (pszFormatString != NULL)
            {
            memcpy(pszFormatString, pTemp, lStringLen); /* Copy string to caller */
            }
         lRetVal = lStringLen;        /* Function return value            */
         break;                       /* We're done searching             */
         }

      pTemp += sizeof(FOURCC);
      pTemp += (strlen(pTemp) + 1);   /* Advance to next search key       */
      }

   DosFreeResource( pResourceData );

   return (lRetVal);  /* Zero or strlen */
}


/**************************************************************************
 *   GetFormatStringLength                                               **
 **************************************************************************
 *
 * ARGUMENTS:
 *
 *     lSearchId         - Table search key
 *     plNameLength      - Address where to return string length.
 *
 * RETURN:
 *
 *     Length of the format string not including the terminating '\0'.
 *     That is, the same value as would be returned from strlen().
 *
 * DESCRIPTION:
 *
 *     This function will retrieve the length of the format string
 *     for the specified IOProc from the resource file that contains
 *     the strings.
 *
 ***************************************************************************/
LONG GetFormatStringLength (LONG  lSearchId,
                            PLONG plNameLength)
{
   LONG  lStringSize;
   LONG  lRetVal;

   lStringSize = GetFormatString (lSearchId, NULL, LONG_MAX);
   if (lStringSize > 0)             /* String found?                      */
      {
      *plNameLength = lStringSize;  /* yes, return strlen                 */
      lRetVal = 0;                  /* and indicate success to caller     */
      }
   else
      {
      *plNameLength = 0;            /* no, error.  Return zero for length */
      lRetVal = lStringSize;        /* and error code from called routine */
      }
   return (lRetVal);
}


/**************************************************************************
 *   GetNLSData                                                          **
 **************************************************************************
 *
 * ARGUMENTS:
 *
 *     pulCodePage       - Address where to return the code page/country.
 *     pulLanguage       - Address where to return the language/dialect.
 *
 * RETURN:
 *
 *     Error code or 0.
 *
 * DESCRIPTION:
 *
 *     This function will retrieve the NLS information for the IOProc
 *     strings contained in the resource file.
 *
 ***************************************************************************/

ULONG APIENTRY GetNLSData( PULONG pulCodePage,
                           PULONG pulLanguage )
{
   PVOID   pResourceData;
   CHAR    *pTemp;

   if (DosGetResource( hModuleHandle,
                       RT_RCDATA,
                       MMOTION_NLS_CHARSET_INFO,
                       &pResourceData ))
      {
      return (MMIO_ERROR);
      }

   /*
    * The resource table is of the form :
    *   usCodePage     Low
    *   usCountryCode  High
    *   usLanguage     Low
    *   usDialect      High
    */

   pTemp = (CHAR *)pResourceData;

   while (pTemp)
      {
      memmove( pulCodePage, pTemp, sizeof(ULONG) );
      pTemp += sizeof(ULONG);

      if (pTemp == NULL)
         {
         break;
         }

      memmove( pulLanguage, pTemp, sizeof(ULONG) );

      break;
      }

   DosFreeResource( pResourceData );

   return (MMIO_SUCCESS);
}

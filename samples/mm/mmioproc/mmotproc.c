/********************** START OF SPECIFICATIONS *************************/
/*                                                                      */
/* SOURCE FILE NAME: MMOTPROC.C                                         */
/*                                                                      */
/* DESCRIPTIVE NAME: Multi-Media I/O Procedure for M-Motion Video       */
/*                                                                      */
/* COPYRIGHT:                                                           */
/*              Copyright (c) IBM Corporation  1990, 1993               */
/*                        All Rights Reserved                           */
/*                                                                      */
/* STATUS:   MMPM/2 Release 1.10                                        */
/*                                                                      */
/* ABSTRACT: This file contains the MMIO                                */
/*           IOProc for M-Motion Still Video files.                     */
/*                                                                      */
/*           This IOProc is intended to be a sample for how IOprocs     */
/*           can be written, and is most applicable to Image data.      */
/*           Note that there are many areas that are specific to the    */
/*           M-Motion Still Video Image file format.  These sections    */
/*           and structures should be changed as necessary for use      */
/*           with other image file formats.                             */
/*                                                                      */
/*           Note also that this image IOProc uses large buffering      */
/*           schemes, due to the ordering of image data (top-down)      */
/*           in the file, whereas the MMIO function and messages rely   */
/*           on this data being made available in the normal OS/2 PM    */
/*           bottom-up form.  This forces the IOproc to pre-read,       */
/*           buffer and flip image data before it can supply the data   */
/*           to the application.  It must perform the reverse order     */
/*           when writing (get the data, buffer it, flip and write to   */
/*           to disk.                                                   */
/*                                                                      */
/*********************** END OF SPECIFICATIONS **************************/

/************************************************************************/
/* Put all #defines here                                                */
/************************************************************************/

#define INCL_32                         /* force 32 bit compile */
#define INCL_GPIBITMAPS
#define INCL_DOSFILEMGR
#define INCL_WIN
#define INCL_GPI
#define INCL_PM                         

#define MEMCHECK

/************************************************************************/
/* Put all #includes here                                               */
/************************************************************************/

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os2medef.h>
#include <mmioos2.h>
#include "mmotproc.h"

/************************************************************************/
/* MMOT IOProc                                                          */
/*                                                                      */
/* ARGUMENTS:                                                           */
/*                                                                      */
/*     PSZ pmmioStr - pointer to MMIOINFO block                         */
/*     USHORT usMsg - MMIO message being sent                           */
/*     LONG lParam1 - filename or other parameter depending on message  */
/*     LONG lParam2 - used with some messages as values                 */
/*                                                                      */
/*                                                                      */
/*  RETURN:                                                             */
/*                                                                      */
/*      MMIOM_OPEN                                                      */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_READ                                                      */
/*          Success           - Returns the number of bytes actually    */
/*                              read.  Return 0L if no more bytes can   */
/*                              be read.                                */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_WRITE                                                     */
/*          Success           - Returns the number of bytes actually    */
/*                              written.                                */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_SEEK                                                      */
/*          Success           - Returns the new file position           */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_CLOSE                                                     */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*          Other             - MMIO_WARNING, file was closed but the   */
/*                              IOProc expected additional data         */
/*                                                                      */
/*      MMIOM_GETFORMATNAME                                             */
/*          Success           - Returns the number of bytes read into   */
/*                              the buffer (size of format name)        */
/*          Failure           - Return 0                                */
/*                                                                      */
/*      MMIOM_GETFORMATINFO                                             */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_QUERYHEADERLENGTH                                         */
/*          Success           - Returns the size of the header in bytes */
/*          Failure           - Return 0                                */
/*                                                                      */
/*      MMIOM_IDENTIFYFILE                                              */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_GETHEADER                                                 */
/*          Success           - Returns number of bytes copied into     */
/*                              the header structure.                   */
/*          Failure           - Return 0                                */
/*          Other             - If length passed in was not large       */
/*                              enough to hold header then,             */
/*                              MMIOERR_INVALID_BUFFER_LENGTH is set    */
/*                              in ulErrorRet.                          */
/*                            - If header is bad,                       */
/*                              MMIOERR_INVALID_STRUCTURE is set in     */
/*                              ulErrorRet                              */
/*                                                                      */
/*      MMIOM_SETHEADER                                                 */
/*          Success           - Returns number of bytes written         */
/*          Failure           - Return 0                                */
/*          Other             - If header is bad,                       */
/*                              MMIOERR_INVALID_STRUCTURE is set in     */
/*                              ulErrorRet                              */
/*                                                                      */
/*  DESCRIPTION:                                                        */
/*                                                                      */
/*      This routine will translate IBM M-Motion YUV Video data into    */
/*      OS/2 2.0 memory bitmap data and back again.  The IOProc is part */
/*      of OS/2 MultiMedia Extentions File Format Conversion Utility.   */
/*                                                                      */
/*  GLOBAL VARS REFERENCED:                                             */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  GLOBAL VARS MODIFIED:                                               */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  NOTES:                                                              */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  SIDE EFFECTS:                                                       */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/************************************************************************/

LONG EXPENTRY IOProc_Entry (PVOID  pmmioStr,
                             USHORT usMsg,
                             LONG   lParam1,
                             LONG   lParam2)

    {
    PMMIOINFO   pmmioinfo;                      /* MMIOINFO block */

    pmmioinfo = (PMMIOINFO) pmmioStr;

    switch (usMsg)
        {
        /*#############################################################*
         * When Closing the file, perform the following:
         * 1) Setup Variables
         * 2) Process the Image buffer
         * 3) Compress the Image to appropriate format
         *#############################################################*/
        case MMIOM_CLOSE:
            {
            /************************************************************
             * Declare local variables.
             ************************************************************/
            PMMFILESTATUS   pVidInfo;         /* MMotionIOProc instance data */

            ULONG           ulHeight;         /* Image height                */
            USHORT          usBitCount;
            /* Image width, including overflow in 1bpp & 4bpp                */
            ULONG           ulImgPelWidth;
            PBYTE           lpYUVLine;        /* One line of packed YUV      */
            LONG            lYUVBytesPerLine;

            ULONG           ulMaxPelWidth;    /* # pels on 4-pel boundaries  */
            /* # pels on a YUV line in the output file                       */
            ULONG           ulYUVPelWidth;
            ULONG           ulRGBMaxBytesPerLine; /* #bytes on 4-pel bounds  */
            PBYTE           lpRGBLine;        /* One line of 24bit RGB       */

            PBYTE           lpImgBufPtr;      /* Current loc in RGB image buf*/
            LONG            lRGBBytesPerLine; /* #bytes on a line in image   */
            ULONG           ulRowCount;       /* loop counter                */
            LONG            lBytesWritten;    /* #bytes output on a write    */
            LONG            lRetCode;
            USHORT          rc;

            /***********************************************************
             * Check for valid MMIOINFO block.
             ***********************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /***********************************************************
             * Set up our working file status variable.
             ***********************************************************/
            pVidInfo = (PMMFILESTATUS)pmmioinfo->pExtraInfoStruct;

            /***********************************************************
             * Assume success for the moment....
             ***********************************************************/
            lRetCode = MMIO_SUCCESS;

            /************************************************************
             * see if we are in Write mode and have a buffer to write out.
             *    We have no image buffer in UNTRANSLATED mode.
             ************************************************************/
            if ((pmmioinfo->ulFlags & MMIO_WRITE) && (pVidInfo->lpImgBuf))
                {
                /*******************************************************
                 * The buffer should be in palettized or 24-bit RGB
                 * We must convert it to YUV to be written to the file.
                 *******************************************************
                 * The buffer should be complete.  If not, then we
                 * should still close, but can flag an error to the
                 * user that the data may be corrupted.  The only way
                 * we can estimate if this is true is to check the final
                 * position.  If not at the end...
                 *******************************************************/
                if (pVidInfo->lImgBytePos !=
                                          (LONG)pVidInfo->ulImgTotalBytes)
                    {
                    lRetCode = MMIO_WARNING;
                    }

                /*******************************************************
                 * Set up width and height of image in the buffer.
                 *******************************************************/
                ulHeight = pVidInfo->mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
                usBitCount =
                    pVidInfo->mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;

                /*******************************************************
                 * Get the line width in YUV pels and packed bytes.
                 *******************************************************/
                ulYUVPelWidth = pVidInfo->mmotHeader.mmXlen;
                lYUVBytesPerLine = (LONG)(ulYUVPelWidth * 3) >> 1;


                /*******************************************************
                 * This changes from OS/2 PM bottom-up bitmap form
                 *    to M-Motion's top-down form.  Flip all pad, boundary
                 *    bytes as well
                 *******************************************************/
                ImgBufferFlip ((PBYTE)pVidInfo->lpImgBuf,
                                        pVidInfo->ulImgPaddedBytesPerLine,
                                        ulHeight);

                /*******************************************************
                 * Determine number of POSSIBLE pels on a line, tho some
                 *   may be overflow in 1bpp and 4bpp modes.
                 *
                 * From that, we can calc the number of RGB pels we will
                 *   create to represent this line.
                 *******************************************************/
                ulImgPelWidth = (pVidInfo->ulImgPelBytesPerLine << 3) /
                                             usBitCount;
                lRGBBytesPerLine = ulImgPelWidth * 3;

                /*******************************************************
                 * Ensure the width is on a 4-pel boundary, necessary for
                 *     M-Motion.  We will buffer with black
                 *    *** THIS IS ONLY NECESSARY FOR M-MOTION IMAGES
                 *******************************************************/
                if (ulImgPelWidth % 4)
                    ulMaxPelWidth = (((ulImgPelWidth >> 2) + 1) << 2);
                else
                    ulMaxPelWidth = ulImgPelWidth;

                /* #RGB bytes/line = #pels * 3 bytes/pel */
                ulRGBMaxBytesPerLine = ulMaxPelWidth * 3;


                /*******************************************************
                 * Create a buffer for one line of RGB data, accounting for
                 *   the 4-pel boundary required.  Extra bytes won't be used.
                 *******************************************************/
                if (DosAllocMem ((PPVOID) &lpRGBLine,
                                 ulRGBMaxBytesPerLine,
                                 fALLOC))
                    return (MMIO_ERROR);

                /*******************************************************
                 * Create a buffer for one line of YUV data.
                 *******************************************************/
                if (DosAllocMem ((PPVOID) &lpYUVLine,
                                 lYUVBytesPerLine,
                                 fALLOC))
                    return (MMIO_ERROR);

                /*******************************************************
                 * Zero out RGB buffer to cover for any extra black pels
                 * needed at the end.
                 *******************************************************/
                memset (lpRGBLine, 0, ulRGBMaxBytesPerLine);

                /*******************************************************
                 * Initialize start position of RGB buffer.
                 *******************************************************/
                lpImgBufPtr = pVidInfo->lpImgBuf;

                /*******************************************************
                 * Process Image Buffer - Save to file
                 * Place "your" processing code here, if full image
                 *      buffering is performed.
                 * For MMotion:
                 *    Loop
                 *       1.Convert and copy a line of 1bpp, 4bpp, 8 bpp or
                 *            24bpp data into a temporary 24 bpp RGB line.
                 *            This line may contain overflow garbage
                 *            pels from 1bpp and 4bpp modes (where
                 *            width does not fall on even byte boundaries.)
                 *       2.Convert the temporary RGB line contents into a
                 *           YUV line.  ONLY that data necessary converted.
                 *           Overflow from bitmap data ignored.
                 *       3.Write the YUV line to the file
                 *******************************************************/
                for (ulRowCount = 0;
                     ulRowCount < ulHeight;
                     ulRowCount++)
                    {
                    /***************************************************
                     * Convert 1 line of Image data into RGB data
                     ***************************************************/
                   switch (usBitCount)
                       {
                       case 1:
                           {
                           /* Convert 1bpp padded image buffer into 24-bit */
                           /*    RGB line buffer, w/pads                   */
                           Convert1BitTo24Bit (
                                   (PBYTE)lpImgBufPtr,
                                   (PRGB) lpRGBLine,
                                   (PRGB) &(pVidInfo->rgbPalette),
                                   pVidInfo->ulImgPelBytesPerLine);
                           break;
                           }

                       case 4:
                           {
                           /* Convert data from app buffer into 24-bit and */
                           /*    copy into image buffer                    */
                           Convert4BitTo24Bit (
                                   (PBYTE)lpImgBufPtr,
                                   (PRGB) lpRGBLine,
                                   (PRGB) &(pVidInfo->rgbPalette),
                                   pVidInfo->ulImgPelBytesPerLine);
                           break;
                           }

                       case 8:
                           {
                           /* Convert data from app buffer into 24-bit and */
                           /*    copy into image buffer                    */
                           Convert8BitTo24Bit (
                                   (PBYTE)lpImgBufPtr,
                                   (PRGB) lpRGBLine,
                                   (PRGB) &(pVidInfo->rgbPalette),
                                   pVidInfo->ulImgPelBytesPerLine);
                           break;
                           }

                       case 24:
                           {
                           /* Copy raw RGB data from the image buffer into */
                           /*    the temporary                             */
                           /*   RGB line.  Only copy those pels necessary. */
                           /*    No conversion required                    */
                           memcpy ((PVOID) lpRGBLine,
                                          (PVOID) lpImgBufPtr,
                                          ulYUVPelWidth * 3);
                           break;
                           }

                       }   /* end of Switch for Bit Conversion block */

                    /***************************************************
                     * Convert one line at a time from RGB to YUV.
                     ***************************************************/
                    ConvertOneLineRGBtoYUV (lpRGBLine,
                                            lpYUVLine,
                                            ulYUVPelWidth);

                    /***************************************************
                     * Write out line of YUV data to the file.
                     ***************************************************/
                    lBytesWritten  = mmioWrite (pVidInfo->hmmioSS,
                                                (PVOID) lpYUVLine,
                                                lYUVBytesPerLine);

                    /* Check if error or EOF  */
                    if (lBytesWritten != lYUVBytesPerLine)
                       {
                       lRetCode = lBytesWritten;
                       break;
                       }

                    /***************************************************
                     * Make sure bitmap image buffer pointer is correct
                     *    for next line to be converted.  Move forward ALL
                     *    the bytes in the bitmap line, including overflow
                     *    and pad bytes.
                     ***************************************************/
                    lpImgBufPtr += pVidInfo->ulImgPaddedBytesPerLine;
                    }

                /*******************************************************
                 * Free temp buffers.
                 *******************************************************/
                if (lpRGBLine)
                   {
                   DosFreeMem ((PVOID) lpRGBLine);
                   }

                if (lpYUVLine)
                   {
                   DosFreeMem ((PVOID) lpYUVLine);
                   }
                }  /* end IF WRITE & IMAGE BUFFER block */

            /***********************************************************
             * Free the RGB buffer, if it exists, that was created
             * for the translated READ operations.
             ***********************************************************/
            if (pVidInfo->lpRGBBuf)
               {
               DosFreeMem ((PVOID) pVidInfo->lpRGBBuf);
               }

            /***********************************************************
             * Free the IMG buffer, if it exists, that was created
             * for the translated  WRITE operations.
             ***********************************************************/
            if (pVidInfo->lpImgBuf)
               {
               DosFreeMem ((PVOID) pVidInfo->lpImgBuf);
               }

            /***********************************************************
             * Close the file with mmioClose.
             ***********************************************************/
            rc = mmioClose (pVidInfo->hmmioSS, 0);

            DosFreeMem ((PVOID) pVidInfo);

            if (rc != MMIO_SUCCESS)
               return (rc);

            return (lRetCode);
            }  /* end case of MMIOM_CLOSE */

        /*#############################################################*
         * Get the NLS format Information.
         *#############################################################*/
        case MMIOM_GETFORMATINFO:
            {
            /***********************************************************
             * Declare local variables.
             ***********************************************************/
            PMMFORMATINFO       pmmformatinfo;

            /************************************************************
             * Set pointer to MMFORMATINFO structure.
             ************************************************************/
            pmmformatinfo = (PMMFORMATINFO) lParam1;

            /************************************************************
             * Fill in the values for the MMFORMATINFO structure.
             ************************************************************/
            pmmformatinfo->ulStructLen  = sizeof (MMFORMATINFO);
            pmmformatinfo->fccIOProc    = FOURCC_MMOT;
            pmmformatinfo->ulIOProcType = MMIO_IOPROC_FILEFORMAT;
            pmmformatinfo->ulMediaType  = MMIO_MEDIATYPE_IMAGE;

            pmmformatinfo->ulFlags      = MMIO_CANREADTRANSLATED        |
                                          MMIO_CANREADUNTRANSLATED      |
                                          MMIO_CANWRITETRANSLATED       |
                                          MMIO_CANWRITEUNTRANSLATED     |
                                          MMIO_CANREADWRITEUNTRANSLATED |
                                          MMIO_CANSEEKTRANSLATED        |
                                          MMIO_CANSEEKUNTRANSLATED;

            strcpy ((PSZ) pmmformatinfo->szDefaultFormatExt, pszMotionExt);
            if (GetNLSData( &pmmformatinfo->ulCodePage,
                            &pmmformatinfo->ulLanguage ))
               {
               return( -1L );
               }

            if (GetFormatStringLength( FOURCC_MMOT,
                                       &(pmmformatinfo->lNameLength) ))
               {
               return( -1L );
               }


            /************************************************************
             * Return success back to the application.
             ************************************************************/
            return (MMIO_SUCCESS);
            } /* end case of MMIOM_GETFORMATINFO */

        /*#############################################################*
         * Get the NLS format name.
         *#############################################################*/
        case MMIOM_GETFORMATNAME:
            {
            LONG lBytesCopied;

            /************************************************************
             * Copy the M-Motion format string into buffer supplied by
             * lParam1.  Only put in the amount of my string up to the
             * allocated amount which is in lParam2.  Leave enough room
             * for the NULL termination.
             ************************************************************/
            lBytesCopied = GetFormatString( FOURCC_MMOT,
                                            (char *)lParam1,
                                            lParam2 );

            return (lBytesCopied);
            } /* end case of MMIOM_GETFORMATNAME */

        /*#############################################################*
         * Get the file header.
         *#############################################################*/
        case MMIOM_GETHEADER:
            {
            /************************************************************
             * Declare local variables.
             ************************************************************/
            PMMFILESTATUS       pVidInfo;

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (0);

            /************************************************************
             * Set up our working file status variable.
             ************************************************************/
            pVidInfo = (PMMFILESTATUS)pmmioinfo->pExtraInfoStruct;

            /**************************************************
             * Getheader only valid in READ or READ/WRITE mode.
             * There is no header to get in WRITE mode.  We
             * must also have a valid file handle to read from
             **************************************************/
            if ((pmmioinfo->ulFlags & MMIO_WRITE) ||
                (!(pVidInfo->hmmioSS)))
               return (0);

            /************************************************************
             * Check for Translation mode.
             ************************************************************/
            if (!(pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER))
                {
                /********************************************************
                 * Translation is off.
                 ********************************************************/
                if (lParam2 < sizeof (MMOTIONHEADER))
                    {
                    pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
                    return (0);
                    }

                if (!lParam1)
                    {
                    pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
                    return (0);
                    }

                /********************************************************
                 * Read in first 16 bytes to fill up MMotion header.
                 ********************************************************/
                memcpy ((PVOID) lParam1,
                        (PVOID)&pVidInfo->mmotHeader,
                        sizeof (MMOTIONHEADER));

                /* Indicate that the header has been set, which    */
                /* is meaningless in read mode, but allows the     */
                /* application to do writes in read/write mode     */
                pVidInfo->bSetHeader = TRUE;

                return (sizeof (MMOTIONHEADER));
                }   /* end IF NOT TRANSLATED block */

            /******************
             * TRANSLATION IS ON
             ******************/
            if (lParam2 < sizeof (MMIMAGEHEADER))
                {
                pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
                return (0);
                }

            if (!lParam1)
                {
                pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
                return (0);
                }

            memcpy ((PVOID)lParam1,
                    (PVOID)&pVidInfo->mmImgHdr,
                    sizeof (MMIMAGEHEADER));

            return (sizeof (MMIMAGEHEADER));
            } /* end case of MMIOM_GETHEADER */

        /*#############################################################*
         * Identify whether this file can be processed.
         *#############################################################*/
        case MMIOM_IDENTIFYFILE:
            {
            /************************************************************
             * Declare local variables.
             ************************************************************/
            MMOTIONHEADER   mmotHeader;    /* MMotion structure variable*/
            HMMIO           hmmioTemp;     /* MMIO File Handle          */
            ULONG           ulWidth;
            ULONG           ulHeight;
            ULONG           ulRequiredFileLength;
            ULONG           ulActualFileLength;
            BOOL             fValidMMotionFile = FALSE;

            ULONG           ulTempFlags = MMIO_READ | MMIO_DENYWRITE |
                                          MMIO_NOIDENTIFY;
                                           /* flags used for temp open  */
                                           /* and close                 */

            /************************************************************
             * We need either a file name (lParam1) or file handle (lParam2)
             ************************************************************/
            if (!lParam1 && !lParam2)
                return (MMIO_ERROR);

            /* Copy the file handle, assuming one was provided... */
            hmmioTemp = (HMMIO)lParam2;

            /************************************************************
             * If no handle, then open the file using the string name
             ************************************************************/
            if (!hmmioTemp)
               {
               if (!(hmmioTemp = mmioOpen ((PSZ) lParam1,
                                          NULL,
                                          ulTempFlags)))
                   {
                   return (MMIO_ERROR);
                   }
               }

            /************************************************************
             * Read in enough bytes to check out file.
             ************************************************************/
            if (sizeof (MMOTIONHEADER) !=
                            mmioRead (hmmioTemp,
                                      (PVOID) &mmotHeader,
                                      (ULONG) sizeof (MMOTIONHEADER)))
                {
                /********************************************************
                 * Fail so close file and then return.
                 ********************************************************/
                if (!lParam2) /* Don't close handle if provided to us  */
                   mmioClose (hmmioTemp, 0);
                return (MMIO_ERROR);
                }

            /************************************************************
             * Close file before returning.
             ************************************************************/
            if (!lParam2) /* Don't close handle if provided to us  */
               mmioClose (hmmioTemp, 0);

            /************************************************************
             * Check validity of file and return result.
             ************************************************************/
            if (memcmp (mmotHeader.mmID, "YUV12C", 6) == 0)
                {
                ulWidth = mmotHeader.mmXlen;
                ulHeight = mmotHeader.mmYlen;

                /* Calculate what the length of the file SHOULD be based on the */
                /*   header contents                                            */
                ulRequiredFileLength = (((ulWidth >> 2) * 6) * ulHeight) +
                                                       sizeof (MMOTIONHEADER);

                /* Query what the ACTUAL length of the file is                  */
                ulActualFileLength = (ULONG)mmioSeek (hmmioTemp, 0, SEEK_END);

                /* If these don't match, then it isn't a VALID M-Motion file    */
                /*     - regardless of what the header says.                    */
                if (ulRequiredFileLength == ulActualFileLength)
                   fValidMMotionFile = TRUE;
                else
                   fValidMMotionFile = FALSE;
                }  /* end header check block */

            /************************************************************
             * Close file before returning.
             ************************************************************/
            if (!lParam2)  /* Don't close handle if provided to us      */
               mmioClose (hmmioTemp, 0);

            if (fValidMMotionFile)
               return (MMIO_SUCCESS);
            else
               return (MMIO_ERROR);
            } /* end case of MMIOM_IDENTIFYFILE */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_OPEN:
            {
            /************************************************************
             * Declare local variables
             ************************************************************/
            PMMFILESTATUS   pVidInfo;   /* pointer to a MMOTION file    */
                                        /* status structure that we will*/
                                        /* use for this file instance   */
            MMIMAGEHEADER   MMImgHdr;
            ULONG           ulRequiredFileLength;
            ULONG           ulActualFileLength;
            ULONG           ulWidth;
            ULONG           ulHeight;
            PBYTE           lpYUVBuf;
            ULONG           ulRowCount;
            ULONG           ulRGBBytesPerLine;
            ULONG           ulYUVBytesPerLine;
            LONG            rc;
            HMMIO           hmmioSS;
            PBYTE           lpRGBBufPtr;
            FOURCC          fccStorageSystem;   /* SS I/O Proc FOURCC       */
            MMIOINFO        mmioinfoSS;         /* I/O info block for SS ref*/
            PSZ pszFileName = (CHAR *)lParam1;  /* get the filename from    */
                                                /* parameter                */

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /************************************************************
             * If flags show read and write then send back an error.  We
             * only support reading or writing but not both at the same
             * time on the same file.
             ************************************************************/
            if ((pmmioinfo->ulFlags & MMIO_READWRITE) &&
                ((pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA) ||
                 (pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER)))
                {
                return (MMIO_ERROR);
                }

            /*********************************************************
             * Determine the storage system/child IOProc that actually
             *   obtains the data for us.   The M-Motion data may be contained
             *   in a memory (RAM) file, as a component in a database or
             *   library (a Compound file), or as a stand-alone disk file.
             *
             * While the Application uses this M-Motion IOProc to obtain
             *   untranslated (MMotion) or translated (Bitmap) data,
             *   the IOProc must obtain it's data from something that
             *   reads and writes to a storage media.  The exact storage
             *   media is immaterial - so long as the read and write
             *   operations generate data that LOOKS like it is part
             *   of a standard file.
             *********************************************************/
            if (!pmmioinfo->fccChildIOProc)
               {
               /* Need to determine SS if create from pmmioinfo and filename. */
               if (pmmioinfo->ulFlags & MMIO_CREATE)
                  {
                  if (mmioDetermineSSIOProc( pszFileName,
                                             pmmioinfo,
                                             &fccStorageSystem,
                                             NULL ))
                     {
                     fccStorageSystem = FOURCC_DOS;
                     }
                  }
               else
                  {
                  if (mmioIdentifyStorageSystem( pszFileName,
                                                 pmmioinfo,
                                                 &fccStorageSystem ))
                     {
                     return (MMIO_ERROR);
                     }

                  }

               if (!fccStorageSystem)
                  {
                  return (MMIO_ERROR);
                  }
               else
                  {
                  pmmioinfo->fccChildIOProc = fccStorageSystem;
                  }
               } /* end storage system identification block */

            /**************************************************************
             * Direct the open to the specific storage system necessary
             **************************************************************/
            memset( &mmioinfoSS, '\0', sizeof(MMIOINFO));
            memmove( &mmioinfoSS, pmmioinfo, sizeof(MMIOINFO));
            mmioinfoSS.pIOProc = NULL;
            mmioinfoSS.fccIOProc = pmmioinfo->fccChildIOProc;
            mmioinfoSS.ulFlags |= MMIO_NOIDENTIFY;

            /************************************************************
             * Try to open the file.  Add the NO IDENTIFY flag to
             *    ENSURE THAT WE DON'T LOOP RECURSIVELY!!!
             ************************************************************/
            hmmioSS = mmioOpen (pszFileName,
                                &mmioinfoSS,
                                mmioinfoSS.ulFlags);

            /************************************************************
             * Check if a DELETE was requested - mmioOpen returns a 1,
             *    so we much check this separately
             ************************************************************/
            if (pmmioinfo->ulFlags & MMIO_DELETE)
               {
               /* was the delete successful?         */
               if (!hmmioSS)
                  {
                  pmmioinfo->ulErrorRet = MMIOERR_DELETE_FAILED;
                  return (MMIO_ERROR);
                  }
               else
                  {
                  return (MMIO_SUCCESS);
                  }
               }

            /************************************************************
             * Check the return code from the open call for an error.
             *    If not delete, then the open should have worked.
             ************************************************************/
            if (!hmmioSS)
                return (MMIO_ERROR);

            /************************************************************
             * Allocate memory for one MMotion FileStatus structures
             ************************************************************/
            DosAllocMem ((PPVOID) &pVidInfo,
                                  sizeof (MMFILESTATUS),
                                  fALLOC);

            /************************************************************
             * Ensure the allocate was successful.  If not, then
             *   close the file and return open as unsucessful...
             ************************************************************/
            if (!pVidInfo)
               {
               mmioClose (hmmioSS, 0);
               return (MMIO_ERROR);
               }

            pVidInfo->hmmioSS = hmmioSS;

            /************************************************************
             * Store pointer to our MMFILESTATUS structure in
             * pExtraInfoStruct field that is provided for our use.
             ************************************************************/
            pmmioinfo->pExtraInfoStruct = (PVOID)pVidInfo;

            /************************************************************
             * Set the fields of the FileStatus structure that the
             * IOProc is responsible for.
             ************************************************************/
            InitFileStruct (pVidInfo);

            /*********************************************************
             * If this is a read, we need to check that is a M-Motion
             *    file and perhaps get the data.
             *********************************************************/
            if (pmmioinfo->ulFlags & MMIO_READ)
               {
               /*********************************************************
                * First we must get some basic information from the file
                * Read in data to fill up the MMOTIONHEADER structure.
                *
                * If the read is unsuccessful, this is not a M-Motion file
                *    and we should return a failure on the open
                *********************************************************/
               if (sizeof (MMOTIONHEADER) !=
                                   mmioRead (pVidInfo->hmmioSS,
                                             (PVOID) &pVidInfo->mmotHeader,
                                             (ULONG) sizeof (MMOTIONHEADER)))
                   {
                   mmioClose (pVidInfo->hmmioSS, 0);
                   DosFreeMem ((PVOID) pVidInfo);
                   return (MMIO_ERROR);
                   }

               /* Ensure this IS an MMotion file header before we continue  */
               if (strcmp (pVidInfo->mmotHeader.mmID, "YUV12C"))
                   {
                   mmioClose (pVidInfo->hmmioSS, 0);
                   DosFreeMem ((PVOID) pVidInfo);
                   return (MMIO_ERROR);
                   }

               /********************************************************
                * Set up width and height of image.
                ********************************************************/
               ulWidth  = (ULONG)pVidInfo->mmotHeader.mmXlen;
               ulHeight = (ULONG)pVidInfo->mmotHeader.mmYlen;

               /* Calculate what the length of the file SHOULD be based on the */
               /*   header contents                                            */
               ulRequiredFileLength = (((ulWidth >> 2) * 6) * ulHeight) +
                                                      sizeof (MMOTIONHEADER);

               /* Query what the ACTUAL length of the file is,                 */
               /*   then move back to just after the header.                   */
               ulActualFileLength = (ULONG)mmioSeek (pVidInfo->hmmioSS,
                                                    0, SEEK_END);

               mmioSeek (pVidInfo->hmmioSS, sizeof (MMOTIONHEADER), SEEK_SET);

               /* If these don't match, then it isn't a VALID M-Motion file    */
               /*    - regardless of what the header says.                     */
               if (ulRequiredFileLength != ulActualFileLength)
                   {
                   mmioClose (pVidInfo->hmmioSS, 0);
                   DosFreeMem ((PVOID) pVidInfo);
                   return (MMIO_ERROR);
                   }

               /************************************************************
                * If the app intends to read in translation mode, we must
                * allocate and set-up the buffer that will contain the RGB data
                *
                * We must also read in the data to insure that the first
                * read, seek, or get-header operation will have data
                * to use.  This is ONLY NECESSARY FOR TRANSLATED MODE
                * operations, since we must process reads/writes pretending
                * the image is stored from the Bottom-up.
                *
                ************************************************************
                ************************************************************
                * Fill out the MMIMAGEHEADER structure.
                ************************************************************/
               MMImgHdr.ulHeaderLength = sizeof (MMIMAGEHEADER);
               MMImgHdr.ulContentType  = MMIO_IMAGE_PHOTO;
               MMImgHdr.ulMediaType    = MMIO_MEDIATYPE_IMAGE;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cbFix =
                           sizeof (BITMAPINFOHEADER2);
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cx              = ulWidth;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cy              = ulHeight;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cPlanes         = 1;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount       = 24;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.ulCompression   =
                   BCA_UNCOMP;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cbImage         =
                   ulWidth * ulHeight * 3;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cxResolution    = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cyResolution    = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cclrUsed        = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cclrImportant   = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.usUnits         = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.usReserved      = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.usRecording     =
                   BRA_BOTTOMUP;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.usRendering     =
                   BRH_NOTHALFTONED;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cSize1          = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.cSize2          = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.ulColorEncoding = 0L;
               MMImgHdr.mmXDIBHeader.BMPInfoHeader2.ulIdentifier    = 0L;

               /************************************************************
                * Copy the image header into private area for later use.
                * This will be returned on a mmioGetHeader () call
                ************************************************************/
               pVidInfo->mmImgHdr = MMImgHdr;

               /********************************************************
                * YUV Bytes/Line are = 1 1/2 times the number of pels
                ********************************************************/
               ulYUVBytesPerLine = ulWidth + (ulWidth >> 1);

               /********************************************************
                * RGB Bytes/Line are = 2* YUV bytes/line
                ********************************************************/
               ulRGBBytesPerLine = (ulYUVBytesPerLine << 1);

               /********************************************************
                * Determine total bytes in image
                ********************************************************/
               pVidInfo->ulRGBTotalBytes = ulWidth * ulHeight * 3;

               /********************************************************
                * MMotion Images are ALWAYS on 4-pel boundaries, which also
                *   makes them on 4-byte/LONG boundaries which is used for
                *   Bitmaps.  Therefore, there are no extra pad bytes necessary
                ********************************************************/
               pVidInfo->ulImgPaddedBytesPerLine = ulWidth * 3;
               pVidInfo->ulImgTotalBytes = pVidInfo->ulRGBTotalBytes;

               /*********************************************************
                * For translated data READ mode, we must allocate a buffer,
                *    then get the YUV data from the file and load the RGB
                *    buffer.
                * PLACE FORMAT SPECIFIC CODE HERE TO LOAD IMAGE INTO BUFFER
                * The code below is M-Motion format specific.
                *********************************************************/
               if (pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA)
                  {
                  /********************************************************
                   * Get space for full image buffer.
                   * This will be retained until the file is closed.
                   ********************************************************/
                  if (DosAllocMem ((PPVOID) &(pVidInfo->lpRGBBuf),
                                    pVidInfo->ulRGBTotalBytes,
                                    fALLOC))
                      {
                      mmioClose (pVidInfo->hmmioSS, 0);
                      DosFreeMem ((PVOID) pVidInfo);
                      return (MMIO_ERROR);
                      }

                  /********************************************************
                   * Get temporary space for one line YUV Buffer.
                   ********************************************************/
                  if (DosAllocMem ((PPVOID) &lpYUVBuf,
                                   ulYUVBytesPerLine,
                                   fALLOC))
                      {
                      mmioClose (pVidInfo->hmmioSS, 0);
                      DosFreeMem ((PVOID) pVidInfo);
                      return (MMIO_ERROR);
                      }

                  /********************************************************
                   * Initialize beginning buffer position
                   ********************************************************/
                  lpRGBBufPtr = pVidInfo->lpRGBBuf;

                  /********************************************************
                   * Read in YUV data one line at a time, converting
                   * from YUV to RGB, then placing in the image buffer
                   ********************************************************/
                  for (ulRowCount = 0;
                       ulRowCount < ulHeight;
                       ulRowCount++)

                      {
                      /****************************************************
                       * Read in one line.
                       ****************************************************/
                      rc = mmioRead (pVidInfo->hmmioSS,
                                     (PVOID) lpYUVBuf,
                                     (ULONG) ulYUVBytesPerLine);

                      /****************************************************
                       * Convert one line at a time.
                       ****************************************************/
                      ConvertOneLineYUVtoRGB (lpYUVBuf,
                                              lpRGBBufPtr,
                                              ulYUVBytesPerLine);

                      /****************************************************
                       * Make sure buffer ptr is correct for next convert.
                       ****************************************************/
                      lpRGBBufPtr += (LONG)ulRGBBytesPerLine;
                      }   /* end of FOR loop to read YUV data */

                  DosFreeMem (lpYUVBuf);

                  /********************************************************
                   * This changes from M-Motion's top-down form to OS/2
                   * PM's bottom-up bitmap form.
                   ********************************************************/
                  ImgBufferFlip (pVidInfo->lpRGBBuf,
                                 pVidInfo->ulImgPaddedBytesPerLine,
                                 ulHeight);

                  /********************************************************
                   * RGB Buffer now full, set position pointers to the
                   * beginning of the buffer.
                   ********************************************************/
                  pVidInfo->lImgBytePos =  0;
                  }   /*  end IF TRANSLATED block */
               } /* end IF READ block */

            return (MMIO_SUCCESS);
            } /* end case of MMIOM_OPEN */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_QUERYHEADERLENGTH:
            {
            /************************************************************
             * If there is no MMIOINFO block then return an error.
             ************************************************************/
            if (!pmmioinfo)
                return (0);

            /************************************************************
             * If header is in translated mode then return the media
             * type specific structure size.
             ************************************************************/
            if (pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER)
                return (sizeof (MMIMAGEHEADER));

            else
                /********************************************************
                 * Header is not in translated mode so return the size
                 * of the MMotion header.
                 ********************************************************/
                return (sizeof (MMOTIONHEADER));

            break;
            } /* end case of MMIOM_QUERYHEADERLENGTH */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_READ:
            {
            /************************************************************
             * Declare Local Variables
             ************************************************************/
            PMMFILESTATUS   pVidInfo;
            LONG            rc;
            LONG            lBytesToRead;

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /************************************************************
             * Set up our working file status variable.
             ************************************************************/
            pVidInfo = (PMMFILESTATUS)pmmioinfo->pExtraInfoStruct;

            /************************************************************
             * Is Translate Data off?
             ************************************************************/
            if (!(pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA))
                {
                /********************************************************
                 * Since no translation, provide exact number of bytes req
                 ********************************************************/
                if (!lParam1)
                    return (MMIO_ERROR);

                rc = mmioRead (pVidInfo->hmmioSS,
                               (PVOID) lParam1,
                               (ULONG) lParam2);

                return (rc);
                }

            /************************************************************
             * Otherwise, Translate Data is on...
             ************************************************************/

            /************************************************************
             * Ensure we do NOT write more data out than is remaining
             *    in the buffer.  The length of read was requested in
             *    image bytes, so confirm that there are that many of
             *    virtual bytes remaining.
             ************************************************************/
            if ((ULONG)(pVidInfo->lImgBytePos + lParam2) >
                 pVidInfo->ulImgTotalBytes)
               lBytesToRead =
                   pVidInfo->ulImgTotalBytes - pVidInfo->lImgBytePos;
            else
               lBytesToRead = (ULONG)lParam2;

            /************************************************************
             * Perform this block on ALL reads.  The image data should
             * be in the RGB buffer at this point, and can be handed
             * to the application.
             *
             * Conveniently, the virtual image position is the same
             *    as the RGB buffer position, since both are 24 bit-RGB
             ************************************************************/
            memcpy ((PVOID)lParam1,
                    &(pVidInfo->lpRGBBuf[pVidInfo->lImgBytePos]),
                    lBytesToRead);

            /************************************************************
             * Move RGB Buffer pointer forward by number of bytes read.
             * The Img buffer pos is identical, since both are 24 bits
             ************************************************************/
            pVidInfo->lImgBytePos += lBytesToRead;

            return (lBytesToRead);
            }   /* end case  of MMIOM_READ */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_SEEK:
            {
            /************************************************************
             * Set up locals.
             ************************************************************/
            PMMFILESTATUS   pVidInfo;
            LONG            lNewFilePosition;
            LONG            lPosDesired;
            SHORT           sSeekMode;

            /************************************************************
             * Check to make sure MMIOINFO block is valid.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /************************************************************
             * Set up our working file status variable.
             ************************************************************/
            pVidInfo = (PMMFILESTATUS)pmmioinfo->pExtraInfoStruct;

            lPosDesired = lParam1;
            sSeekMode = (SHORT)lParam2;

            /************************************************************
             * Is Translate Data on?
             ************************************************************/
            if (pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA)
                {
                /********************************************************
                 * Attempt to move the Image buffer pointer to the
                 *    desired location.  App sends SEEK requests in
                 *    positions relative to the image planes & bits/pel
                 * We must also convert this to RGB positions
                 ********************************************************/
                switch (sSeekMode)
                    {
                    case SEEK_SET:
                        {
                        lNewFilePosition = lPosDesired;
                        break;
                        }
                    case SEEK_CUR:
                        {
                        lNewFilePosition = pVidInfo->lImgBytePos + lPosDesired;
                        break;
                        }
                    case SEEK_END:
                        {

                        lNewFilePosition =
                                     pVidInfo->ulImgTotalBytes += lPosDesired;
                        break;
                        }

                    default :
                       return (MMIO_ERROR);
                    }

                /********************************************************
                 * Make sure seek did not go before start of file.
                 * If so, then don't change anything, just return an error
                 ********************************************************/
                if (lNewFilePosition < 0)
                   {
                   return (MMIO_ERROR);
                   }

                /********************************************************
                 * Make sure seek did not go past the end of file.
                 ********************************************************/
                if (lNewFilePosition > (LONG)pVidInfo->ulImgTotalBytes)
                    lNewFilePosition = pVidInfo->ulImgTotalBytes;

                pVidInfo->lImgBytePos = lNewFilePosition;

                return (pVidInfo->lImgBytePos);
                } /* end  IF DATA TRANSLATED */

            /************************************************************
             * Translate Data is OFF...
             ************************************************************
             * if this is a seek from the beginning of the file,
             *    we must account for and pass the header
             ************************************************************/
            if (lParam2==SEEK_SET)
               lPosDesired += MMOTION_HEADER_SIZE;

            lNewFilePosition = mmioSeek (pVidInfo->hmmioSS,
                                         lPosDesired,
                                         sSeekMode);

            /********************************************
             * Ensure we did not move to within the header
             ********************************************/
            if ((lNewFilePosition != MMIO_ERROR) &&
                (lNewFilePosition < MMOTION_HEADER_SIZE))
               {
               lNewFilePosition = mmioSeek (pVidInfo->hmmioSS,
                                             (LONG)MMOTION_HEADER_SIZE,
                                             SEEK_SET);
               }

            /************************************************************
             * Return new position.  Always remove the length of the
             *    header from the this position value
             ************************************************************/
            if (lNewFilePosition != MMIO_ERROR)
               lNewFilePosition -= MMOTION_HEADER_SIZE;

            return (lNewFilePosition);
            }  /* end case of MMIOM_SEEK */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_SETHEADER:
            {
            /************************************************************
             * Declare local variables.
             ************************************************************/
            PMMIMAGEHEADER          pMMImgHdr;
            PMMFILESTATUS           pVidInfo;
            USHORT                  usNumColors;
            ULONG                   ulImgBitsPerLine;
            ULONG                   ulImgBytesPerLine;
            ULONG                   ulBytesWritten;
            ULONG                   ulWidth;
            ULONG                   ul4PelWidth;
            ULONG                   ulHeight;
            USHORT                  usPlanes;
            USHORT                  usBitCount;
            USHORT                  usPadBytes;

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /************************************************************
             * Set up our working variable MMFILESTATUS.
             ************************************************************/
            pVidInfo = (PMMFILESTATUS) pmmioinfo->pExtraInfoStruct;

            /************************************************************
             * Only allow this function if we are in WRITE mode
             * And only if we have not already set the header
             ************************************************************/
            if ((!(pmmioinfo->ulFlags & MMIO_WRITE)) ||
                (!(pVidInfo->hmmioSS)) ||
                (pVidInfo->bSetHeader))
                return (0);

            /********************************************************
             * Make sure lParam1 is a valid pointer
             ********************************************************/
            if (!lParam1)
                {
                pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
                return (0);
                }

            /************************************************************
             * Header is not in translated mode.
             ************************************************************/
            if (!(pmmioinfo->ulTranslate & MMIO_TRANSLATEHEADER))
                {
                /********************************************************
                 * Make sure lParam2 is correct size
                 ********************************************************/
                if (lParam2 != MMOTION_HEADER_SIZE)
                    {
                    pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
                    return (0);
                    }

                /********************************************************
                 * Ensure that the header at least begins with "YUV12C"
                 ********************************************************/
                if (strncmp ((char *)lParam1, "YUV12C", 6))
                   {
                   pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
                   return (0);
                   }

                /********************************************************
                 * Take 16 byte buffer (lParam1), write to file and .
                 *    copy to internal structure.
                 ********************************************************/
                memcpy ((PVOID)&pVidInfo->mmotHeader,
                        (PVOID)lParam1, (ULONG) MMOTION_HEADER_SIZE);
                ulBytesWritten = mmioWrite (pVidInfo->hmmioSS,
                                            (PVOID) lParam1,
                                            (ULONG) MMOTION_HEADER_SIZE);

                /*********************************
                 * Check for an error on the write..
                 *********************************/
                if (ulBytesWritten != MMOTION_HEADER_SIZE)
                    return (0);   /* 0 indicates error */

                /*********************************
                 * Success...
                 *********************************/
                pVidInfo->bSetHeader = TRUE;
                return (sizeof (MMOTIONHEADER));
                }  /* end IF NOT TRANSLATED block */

            /************************************************************
             * Header is translated.
             ************************************************************/

            /************************************************************
             * Create local pointer media specific structure.
             ************************************************************/
            pMMImgHdr = (PMMIMAGEHEADER) lParam1;


            /************************************************************
             * Check for validity of header contents supplied
             ************************************************************
             *  --  Length must be that of the standard header
             *  --  NO Compression
             *      1 plane
             *      24, 8, 4 or 1 bpp
             ************************************************************/
            usBitCount = pMMImgHdr->mmXDIBHeader.BMPInfoHeader2.cBitCount;
            if ((pMMImgHdr->mmXDIBHeader.BMPInfoHeader2.ulCompression !=
                 BCA_UNCOMP) ||
                (pMMImgHdr->mmXDIBHeader.BMPInfoHeader2.cPlanes != 1) ||
                (! ((usBitCount == 24) || (usBitCount == 8) ||
                    (usBitCount == 4) || (usBitCount == 1)))
               )
                {
                pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
                return (0);
                }

            if (lParam2 != sizeof (MMIMAGEHEADER))
               {
               pmmioinfo->ulErrorRet = MMIOERR_INVALID_BUFFER_LENGTH;
               return (0);
               }

            /************************************************************
             * Complete MMOTIONHEADER.
             ************************************************************/
            memcpy ((PVOID)&pVidInfo->mmotHeader.mmID, "YUV12C", 6);
            pVidInfo->mmotHeader.mmXorg  = 0;
            pVidInfo->mmotHeader.mmYorg  = 0;

            /********************************************************
             * Ensure we will save stuff on 4-pel boundaries when
             *     we actually convert to YUV and pack the bits.
             * We DON'T change what the user is actually going to
             *     give us.  HE thinks he is on 1-pel boundaries,
             *     and that is how we buffer the RGB data.
             ********************************************************/
            ulWidth  = pMMImgHdr->mmXDIBHeader.BMPInfoHeader2.cx;
            ulHeight = pMMImgHdr->mmXDIBHeader.BMPInfoHeader2.cy;
            if (ulWidth % 4)
                ul4PelWidth = (((ulWidth >> 2) + 1) << 2);
            else
                ul4PelWidth = ulWidth;
            pVidInfo->mmotHeader.mmXlen  = (USHORT) ul4PelWidth;
            pVidInfo->mmotHeader.mmYlen  = (USHORT) ulHeight;

            /************************************************************
             * Write the MMotion Header.
             ************************************************************/
            ulBytesWritten = mmioWrite (pVidInfo->hmmioSS,
                                        (PVOID) &pVidInfo->mmotHeader,
                                        (ULONG) MMOTION_HEADER_SIZE);

            /***************************************
             * Check for an error on the write...
             ***************************************/
            if (ulBytesWritten != MMOTION_HEADER_SIZE)
                return (0);

            /************************************************************
             * Flag that MMIOM_SETHEADER has been done.  It can only
             *    be done ONCE for a file.  All future attempts will
             *    be flagged as errors.
             ************************************************************/
            pVidInfo->bSetHeader = TRUE;

            /************************************************************
             * Create copy of MMIMAGEHEADER for future use.
             ************************************************************/
            pVidInfo->mmImgHdr = *pMMImgHdr;

            /************************************************************
             * Check bitcount, set palette if less than 24.
             ************************************************************/
            if (usBitCount < 24)
               {
               /*********************************************************
                * Find out how many colors are in the palette.
                *********************************************************/
               usNumColors = (USHORT)(1 << usBitCount);

               /*********************************************************
                * Take the RGB2 Palette and convert it to an RGB palette
                *    Place the converted palette in MMFILESTATUS struct
                *********************************************************/
               RGB2_To_RGB (pVidInfo->mmImgHdr.bmiColors,
                            (PRGB) &(pVidInfo->rgbPalette),
                            usNumColors);
               }

            /*********************************************************
             * We must allocate the buffer.  The app will load the
             * buffer on subsequent write calls
             *********************************************************/
            usPlanes = pVidInfo->mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cPlanes;

            /********************************************************
             * Determine total Image size
             ********************************************************
             * Find bits-per-line BEFORE padding and 1bpp or 4bpp pel overflow
             ********************************************************/
            ulImgBitsPerLine = ulWidth * usPlanes * usBitCount;
            ulImgBytesPerLine = ulImgBitsPerLine >> 3;

            /********************************************************
             * Account for extra pels not on an even byte boundary
             *   for 1bpp and 4bpp
             ********************************************************/
            if (ulImgBitsPerLine % 8)
               ulImgBytesPerLine ++;

            pVidInfo->ulImgPelBytesPerLine = ulImgBytesPerLine;

            /********************************************************
             * Ensure the row length in bytes accounts for byte padding.
             *   All bitmap data rows are aligned on LONG/4-BYTE boundaries.
             *   The data FROM an application should always appear in this form
             ********************************************************/
            usPadBytes = (USHORT)(ulImgBytesPerLine % 4);
            if (usPadBytes)
               ulImgBytesPerLine += 4 - usPadBytes;

            pVidInfo->ulImgPaddedBytesPerLine  = ulImgBytesPerLine;
            pVidInfo->ulImgTotalBytes = ulImgBytesPerLine * ulHeight;

            /********************************************************
             * Get space for full image buffer.
             ********************************************************/
            if (DosAllocMem ((PPVOID) &(pVidInfo->lpImgBuf),
                              pVidInfo->ulImgTotalBytes,
                              fALLOC))
                return (MMIO_ERROR);

            /********************************************************
             * Set up initial pointer value within RGB buffer & image
             ********************************************************/
            pVidInfo->lImgBytePos = 0;

            return (sizeof (MMIMAGEHEADER));
            }  /* end case of MMIOM_SETHEADER */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_WRITE:
            {
            /************************************************************
             * Declare Local Variables.
             ************************************************************/
            PMMFILESTATUS       pVidInfo;
            USHORT              usBitCount;
            LONG                lBytesWritten;
            ULONG               ulImgBytesToWrite;

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /************************************************************
             * Set up our working variable MMFILESTATUS.
             ************************************************************/
            pVidInfo = (PMMFILESTATUS) pmmioinfo->pExtraInfoStruct;

            /************************************************************
             * See if a SetHeader has been done on this file.
             ************************************************************/
            if ((!pVidInfo) || (!pVidInfo->bSetHeader))
                {
                return (MMIO_ERROR);
                }

            if (!(pmmioinfo->ulTranslate & MMIO_TRANSLATEDATA))
                {
                /********************************************************
                 * Translation is off, take amount of bytes sent and
                 * write to the file.
                 ********************************************************
                 * Ensure that there is a data buffer to write from
                 ********************************************************/
                if (!lParam1)
                    return (MMIO_ERROR);

                lBytesWritten = mmioWrite (pVidInfo->hmmioSS,
                                           (PVOID) lParam1,
                                           (ULONG) lParam2);

                return (lBytesWritten);
                }

            /************************************************************
             ************************************************************
             * Translation is on.
             ************************************************************
             ************************************************************
             ************************************************************
             * Set up local variables
             ************************************************************/
            usBitCount =
               pVidInfo->mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;


            /************************************************************
             * Ensure we do not attempt to write past the end of the
             *    buffer...
             ************************************************************/
            if ((ULONG)(pVidInfo->lImgBytePos + lParam2) >
               pVidInfo->ulImgTotalBytes)
               ulImgBytesToWrite =
                   pVidInfo->ulImgTotalBytes - pVidInfo->lImgBytePos;
            else
               ulImgBytesToWrite = (ULONG)lParam2;

            /************************************************************
             * Write the data into the image buffer.  It will be converted to
             *   RGB, then YUV  when the file is closed.  This allows the
             *   application to seek to arbitrary  positions within the
             *   image in terms of the bits/pel, etc they are writing.
             ************************************************************/
            memcpy (&(pVidInfo->lpImgBuf[pVidInfo->lImgBytePos]),
                    (PVOID)lParam1,
                    ulImgBytesToWrite);

            /* Update current position in the image buffer */
            pVidInfo->lImgBytePos += ulImgBytesToWrite;

            return (ulImgBytesToWrite);
            }   /* end case of MMIOM_WRITE */

        /* 
         * If the IOProc has a child IOProc, then pass the message on to the Child, otherwise 
         * return Unsupported Message
         */
        default: 
            {
             /*
              * Declare Local Variables.
              */
             PMMFILESTATUS       pVidInfo;
             LONG                lRC;

             /************************************************************
              * Check for valid MMIOINFO block.
              ************************************************************/
             if (!pmmioinfo)
                 return (MMIO_ERROR);
 
             /************************************************************
              * Set up our working variable MMFILESTATUS.
              ************************************************************/
             pVidInfo = (PMMFILESTATUS) pmmioinfo->pExtraInfoStruct;

             if (pVidInfo != NULL && pVidInfo->hmmioSS)
                {
                 lRC = mmioSendMessage (pVidInfo->hmmioSS,
                                        usMsg,
                                        lParam1,
                                        lParam2);
                 if (!lRC)
                    pmmioinfo->ulErrorRet = mmioGetLastError (pVidInfo->hmmioSS);
                 return (lRC);
                }
            else
               {
                if (pmmioinfo != NULL) 
                   pmmioinfo->ulErrorRet = MMIOERR_UNSUPPORTED_MESSAGE;
                return (MMIOERR_UNSUPPORTED_MESSAGE);
               }

            }   /* end case of Default */

        } /* end SWITCH statement for MMIO messages */

    return (0);
    }      /* end of window procedure */

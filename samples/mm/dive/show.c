 /**************************************************************************
 *
 * File Name        : SHOW.C
 *
 * Description      : This program provides the sample code of using DIVE APIs.
 *
 *                    Direct Interface Video Extension is developed for
 *                    the entertainment, education, and games software
 *                    to give the high-speed perforemance.
 *
 *
 *
 * Concepts         :
 *
 * Entry Points     : DirectMoveMem()
 *                    ReadFile()
 *                    Main()
 *                    MyWindowProc()
 *
 * DIVE API's       :
 *                    DiveSetSourcePalette
 *                    DiveBlitImage
 *                    DiveAllocImageBuffer
 *                    DiveBeginImageBufferAccess
 *                    DiveEndImageBufferAccess
 *                    DiveOpen
 *                    DiveSetDestinationPalette
 *                    DiveFreeImageBuffer
 *                    DiveClose
 *                    DiveSetupBlitter
 *
 * Copyright        : COPYRIGHT IBM CORPORATION, 1991, 1992, 1993
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
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  _MEERROR_H_
#include <mmioos2.h>                   /* It is from MMPM toolkit           */
#include <dive.h>
#include <fourcc.h>
#include "show.h"


/* Function prototypes
*/
MRESULT EXPENTRY MyWindowProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY MyDlgProc ( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 );

/* Global definitions
*/
HAB       hab;                          /* PM anchor block handle            */
ULONG     ulImage[MAX_FILE];            /* Image buffers from DIVE           */
PVOID     pbImage[MAX_FILE];            /* Pointer to DIVE image buffer      */
PVOID     pbSrcImage[MAX_FILE];         /* Pointer to Dos image buffer       */
PBYTE     pPalette[MAX_FILE];           /* Pointer to bitmap palette area    */
PSZ       pszMyWindow = "MyWindow";     /* Window class name                 */
PSZ       pszTitleText = "DIVE SAMPLE"; /* Title bar text                    */
ULONG     ulToEnd = 0;                  /* Controls the display thread       */
DIVE_CAPS DiveCaps = {0};
FOURCC    fccFormats[100] = {0};        /* Color format code                 */
ULONG     ulFramesToTime=8;             /* Interval of frames to get time    */
ULONG     ulNumFrames=0;                /* Frame counter                     */

/*  Default bitmap file name definitions
**       These files are used only when EXE is called without parameter.
*/
PSZ  pszDefFile[]   = { {"TPG20000.BMP"},
                        {"TPG20001.BMP"},
                        {"TPG20002.BMP"},
                        {"TPG20003.BMP"},
                        {"TPG20004.BMP"},
                        {"TPG20005.BMP"},
                        {"TPG20006.BMP"},
                        {"TPG20007.BMP"},
                        {"TPG20008.BMP"},
                        {"TPG20009.BMP"},
                        {"TPG20010.BMP"},
                        {"TPG20011.BMP"},
                        {"TPG20012.BMP"},
                        {"TPG20013.BMP"},
                        {"TPG20014.BMP"},
                        {"TPG20015.BMP"} };

/*
**  It is the definition how many time draw one bitmap continuously to
**  look the picture better.
*/
ULONG ulDrawCnt[] =   { 1,                       /* For TPG20000.BMP  */
                        1,                       /* For TPG20001.BMP  */
                        1,                       /* For TPG20002.BMP  */
                        1,                       /* For TPG20003.BMP  */
                        1,                       /* For TPG20004.BMP  */
                        1,                       /* For TPG20005.BMP  */
                        1,                       /* For TPG20006.BMP  */
                        1,                       /* For TPG20007.BMP  */
                        1,                       /* For TPG20008.BMP  */
                        1,                       /* For TPG20009.BMP  */
                        1,                       /* For TPG20010.BMP  */
                        1,                       /* For TPG20011.BMP  */
                        1,                       /* For TPG20012.BMP  */
                        1,                       /* For TPG20013.BMP  */
                        1,                       /* For TPG20014.BMP  */
                        1 };                     /* For TPG20015.BMP  */

/****************************************************************************
 *
 * Name          : DirectMoveMem
 *
 * Description   : It is calling DIVE bitblt function periodically.
 *                 And also it calculates how many frames per a second
 *                 is able to be blit to the screen.
 *
 * Concepts      :
 *
 * Parameters    : Pointer to window data
 *
 * Return        : VOID
 *
 ****************************************************************************/

VOID APIENTRY DirectMoveMem ( ULONG parm1 )
   {
   ULONG    ulTime0, ulTime1;     /* output buffer for DosQierySysInfo      */
   ULONG    ulIndexImage = 0;     /* Index of bitmap data                   */
   CHAR     achFrameRate[48];     /* string buffer for WinSetWindowText     */
   ULONG    ulFirstTime=0;        /* very first time flag for DiveSetSour.. */

   PWINDATA pwinData;             /* pointer to window data                 */
   ULONG    ulCount = 0;          /* Counter for each bitmap                */
   PBYTE    pDestAddress;         /* destination VRAM adrress               */
   PBYTE    pSrcAddress;          /* source btimap data address             */

   BOOL     fFirstLineInRect;     /* Indicates start of drawing sequence    */
   RECTL    rcl;                  /* Current rectangle to draw              */
   ULONG    ulMoreLine;           /* Number of lines left to draw           */
   ULONG    ulRemLinesInBank;     /* Number of lines left in bank           */
   ULONG    ulBankNumber;         /* Bank number returned from DIVE         */
   ULONG    ulCopyLengthBytes;    /* Length of scan line for current window */
   ULONG    i;

   pwinData =(PWINDATA)parm1;

   while ( !ulToEnd )
      {

      /* Check if it's time to start the blit-time counter.
      */
      if ( !ulNumFrames++ )
         DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &ulTime0, 4L );

      /* Check if need to change the source palette.
      **
      ** In this sample case, this API should be called only once at
      ** very first.  After this, it will not be called, because there is
      ** assumption that all bitmap data have the sample palette.
      */
      if (( pwinData->fChgSrcPalette ) || (( !ulFirstTime ) &&
            ( pwinData->ulnumColor  == 256 )) )
         {
         DiveSetSourcePalette ( pwinData->hDive, 0,
                                  pwinData->ulnumColor,
                                  pPalette[ulIndexImage] );
         ulFirstTime = 0xFFFF;
         }

      /* Check to see if images are in process of being converted
      ** If not, then continue updating display
      */
      if ( ! pwinData->fDataInProcess )
         {
         /* Check if direct or blit mode
         */
         if (pwinData->fDirect)
            {
            /* Write image to frame buffer directly
            */
            if (!pwinData->fVrnDisabled)
               {
               /* Copy bitmap data for each rectangle
               */
               for (i = 0; i < pwinData->ulNumRcls; i++)
                  {
                  pSrcAddress = (PBYTE)pbImage[ulIndexImage];
                  memcpy (&rcl, &pwinData->rcls[i], sizeof(RECTL) );

                  /* Size of scan line bounded by this rectangle
                  */
                  ulCopyLengthBytes = ( rcl.xRight - rcl.xLeft ) *
                                      pwinData->ulColorSizeBytes;

                  /* Number of lines contained in this rectangle
                  */
                  ulMoreLine = rcl.yTop - rcl.yBottom;

                  /* Start position in source bitmap
                  */
                  pSrcAddress += ( pwinData->swp.cy - rcl.yTop )
                                  * pwinData->ulLineSizeBytes +
                                 ( rcl.xLeft * pwinData->ulColorSizeBytes );

                  /* Map rectangle to desktop coordinates
                  */
                  rcl.yTop    += pwinData->cyWindowPos;
                  rcl.yBottom += pwinData->cyWindowPos;
                  rcl.xRight  += pwinData->cxWindowPos;
                  rcl.xLeft   += pwinData->cxWindowPos;

                 /* NOTICE !!! */
                 /**** TEMPORARY ADJUSTMENT  start ****************************
                  * If this adjustment is not here, DiveCalcFrameBufferAddress
                  * returns 1 dot higher position.
                  * Need to contact the devloper.
                  *************************************************************/
                  rcl.yTop --;
                  rcl.yBottom --;
                 /* end temporary adjustment */

                  fFirstLineInRect = TRUE;

                  while ( ulMoreLine )
                     {
                     if ( fFirstLineInRect )
                        {
                        /* Get VRAM start location
                        */
                        DiveCalcFrameBufferAddress ( pwinData->hDive,
                                                       &rcl,
                                                       &pDestAddress,
                                                       &ulBankNumber,
                                                       &ulRemLinesInBank );

                        if ( DiveAcquireFrameBuffer ( pwinData->hDive,
                                                        &rcl ))
                           break;

                        DiveSwitchBank ( pwinData->hDive, ulBankNumber );
                        fFirstLineInRect = FALSE;
                        }
                     /* Copy one Line to display
                     */
                     memcpy ( pDestAddress, pSrcAddress, ulCopyLengthBytes );

                     /* Update source address
                     */
                     pSrcAddress += pwinData->ulLineSizeBytes;

                     ulMoreLine--;
                     ulRemLinesInBank--;
                     rcl.yTop--;

                     /* Check if time to switch banks.
                     */
                     if ( !ulRemLinesInBank )
                        {
                        DiveCalcFrameBufferAddress ( pwinData->hDive,
                                                       &rcl,
                                                       &pDestAddress,
                                                       &ulBankNumber,
                                                       &ulRemLinesInBank );

                        DiveSwitchBank ( pwinData->hDive, ulBankNumber );
                        }
                     else
                        {
                        /* update destination address.
                        */
                        pDestAddress += DiveCaps.ulScanLineBytes;
                        }

                     } /* end while: go to next scan line */

                  DiveDeacquireFrameBuffer ( pwinData->hDive );
                  } /* end for: go to next rectangle */
               }/* end if */
            }
         else
            {
            /* Blit the image using DiveBlit
            */
            DiveBlitImage ( pwinData->hDive,
                            ulImage[ulIndexImage],
                            DIVE_BUFFER_SCREEN );
         } /* endif */

         /* Updated the index for the bitmap data to be drawn next.
         */
         ulCount ++;
         if ( ulCount >=  ulDrawCnt[ulIndexImage] )
            {
            ulIndexImage ++;
            ulCount = 0;
            }
         if ( ulIndexImage >= pwinData->ulMaxFiles )
            ulIndexImage = 0;

         /* Check to see if we have enough frames for a fairly accurate read.
         */
         if ( ulNumFrames>=ulFramesToTime )
            {
            DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &ulTime1, 4L );
            ulTime1 -= ulTime0;
            if ( ulTime1 )
               sprintf ( achFrameRate, "%d: %d: %5.2f frames per second.",
                           pwinData->hDive, ulFramesToTime,
                           (float)ulFramesToTime * 1000.0 / (float)ulTime1 );
            else
               sprintf ( achFrameRate, "%d: %d: Lots of frames per second.",
                           pwinData->hDive, ulFramesToTime );
            WinPostMsg ( pwinData->hwndFrame, WM_COMMAND,
                           (PVOID)ID_NEWTEXT, achFrameRate );
            ulNumFrames = 0;

            /* Adjust number of frames to time based on last set.
            */
            if ( ulTime1 < 250 )
               ulFramesToTime <<= 1;
            if ( ulTime1 > 3000 )
               ulFramesToTime >>= 1;
            }

         /* Let other threads and processes have some time.
         */
         }
      DosSleep ( 0 );
      }

   return;
   }

/****************************************************************************
 *
 * Name          : ReadFile
 *
 * Description   : It opens the file, and reads bitmap header and bitmap
 *                 palette, and reads image data to the buffer allocated
 *                 by DIVE APIs.
 *
 * Concepts      :
 *
 * Parameters    : Index for the file to open
 *                 Pointer to the file name to open
 *                 Pointer to window data
 *
 * Return        : 0 - succeed
 *                 1 - fail
 *
 ****************************************************************************/


unsigned long ReadFile ( ULONG iFile, unsigned char *pszFile,
                                                       PWINDATA pwinData )
   {
   HFILE hFile;                 /* file handke                              */
   ULONG ulNumBytes;            /* output for number of bytes actually read */
   ULONG ulFileLength;          /* file length                              */
   PBYTE pbBuffer;              /* pointer to the image/ temp. palette data */
   ULONG ulScanLineBytes;       /* output for number of bytes a scan line   */
   ULONG ulScanLines;           /* output for height of the buffer          */
   PBITMAPFILEHEADER2 pImgHdr;  /* pointer to bitmapheader                  */
   ULONG i, j;
   PBYTE pbTmpDst;              /* temporaly destination pointer            */

   /* Attempt to open up the passed filename.
   */
   if ( DosOpen ( pszFile, &hFile, &ulNumBytes, 0L, FILE_NORMAL,
                    OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
                    OPEN_ACCESS_READONLY | OPEN_SHARE_DENYREADWRITE |
                    OPEN_FLAGS_SEQUENTIAL | OPEN_FLAGS_NO_CACHE, 0L ) )
      return ( 1 );

   /* Get the legnth of the file.
   */
   DosSetFilePtr ( hFile, 0L, FILE_END, &ulFileLength );
   DosSetFilePtr ( hFile, 0L, FILE_BEGIN, &ulNumBytes );

   /* Allocate memory for bitmap file header
   */
   if ( DosAllocMem ( (PPVOID)&pImgHdr, sizeof(BITMAPFILEHEADER2),
                        (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
      {
      DosClose ( hFile );
      return ( 1 );
      }

   /* Read from the beginning of the header to cbFix in BITMAPINFOHEADER
   ** to know the length of the header.
   */
   if ( DosRead ( hFile, pImgHdr,
                    sizeof(BITMAPFILEHEADER2) - sizeof(BITMAPINFOHEADER2) +
                    sizeof(ULONG),
                    &ulNumBytes ))
      {
      DosFreeMem ( (PVOID)pImgHdr );
      DosClose ( hFile );
      return ( 1 );
      }

   /* Read the rest of the header.
   */
   if ( DosRead ( hFile, (PBYTE)pImgHdr + ulNumBytes,
                    pImgHdr->bmp2.cbFix - sizeof(ULONG),
                    &ulNumBytes ))
      {
      DosFreeMem ( (PVOID)pImgHdr );
      DosClose ( hFile );
      return ( 1 );
      }

    pwinData->ulnumColor = 1;

   /* Check the bitmap header format --  new or old one.
   */
   if ( pImgHdr->bmp2.cbFix != sizeof(BITMAPINFOHEADER) )
      {
      /*  Bitmap has new format (BITMAPFILEHEADER2)
      */

      /* Check bitmap header to see if it can support.
      */

      /* Set how many color bitmap data is supporting
      */
      pwinData->ulnumColor <<= pImgHdr->bmp2.cBitCount;

      /* Set bitmap width and height in pels.
      */
      pwinData->ulWidth  = pImgHdr->bmp2.cx;
      pwinData->ulHeight = pImgHdr->bmp2.cy;

      /* Calculate source line size.  It should be double word boundary.
      */
      pwinData->ulSrcLineSizeBytes = ((( pImgHdr->bmp2.cx *
                              ( pImgHdr->bmp2.cBitCount >> 3 )) + 3 ) / 4 ) * 4;

      /* Set bitmap coclor format.
      */
      switch ( pImgHdr->bmp2.cBitCount )
         {
         case 8:
            pwinData->fccColorFormat = FOURCC_LUT8;
            /* Alloate buffer for palette data in bitmap file.
            */
            if ( DosAllocMem ( (PPVOID) &(pPalette[iFile]),
                                 pwinData->ulnumColor * sizeof(ULONG),
                                 (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
               {
               DosFreeMem ( (PVOID)pImgHdr );
               DosClose ( hFile );
               return ( 1 );
               }

            /* Read palette data.
            */
            if ( DosRead ( hFile, pPalette[iFile],
                             pwinData->ulnumColor * sizeof(ULONG),
                             &ulNumBytes ))
               {
               DosFreeMem ( (PVOID)pImgHdr );
               DosFreeMem ( (PVOID)(pPalette[iFile]) );
               DosClose ( hFile );
               return ( 1 );
               }
            break;
         case 16:
            pwinData->fccColorFormat = FOURCC_R565;
            break;
         case 24:
            pwinData->fccColorFormat = FOURCC_BGR3;
            break;
         default:
            DosFreeMem ( (PVOID)pImgHdr );
            DosClose ( hFile );
            return ( 1 );
         } /* endswitch */

      }
   else
      {
      /*  Bitmap has old format (BITMAPFILEHEADER)
      */

       /* Set how many color bitmap data is supporting
      */
      pwinData->ulnumColor <<= ((PBITMAPFILEHEADER)pImgHdr)->bmp.cBitCount;

      /* Set bitmap width and height in pels.
      */
      pwinData->ulWidth  = ((PBITMAPFILEHEADER)pImgHdr)->bmp.cx;
      pwinData->ulHeight = ((PBITMAPFILEHEADER)pImgHdr)->bmp.cy;

      /* Calculate source line size.  It should be double word boundary.
      */
      pwinData->ulSrcLineSizeBytes = ((( ((PBITMAPFILEHEADER)pImgHdr)->bmp.cx *
           ( ((PBITMAPFILEHEADER)pImgHdr)->bmp.cBitCount >> 3 )) + 3 ) / 4 ) * 4;

      /* Set bitmap coclor format.
      */
      switch ( ((PBITMAPFILEHEADER)pImgHdr)->bmp.cBitCount )
         {
         case 8:
            pwinData->fccColorFormat = FOURCC_LUT8;
            /* Alloate buffer for temporally palette data in bitmap file
            */
            if ( DosAllocMem ( (PPVOID) &pbBuffer,
                                 pwinData->ulnumColor * sizeof(RGB),
                                 (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
               {
               DosFreeMem ( (PVOID)pImgHdr );
               DosClose ( hFile );
               return ( 1 );
               }
            if ( DosAllocMem ( (PPVOID) &(pPalette[iFile]),
                                 pwinData->ulnumColor * sizeof(ULONG),
                                 (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
               {
               DosFreeMem ( (PVOID)pImgHdr );
               DosFreeMem( (PVOID)pbBuffer );
               DosClose ( hFile );
               return ( 1 );
               }

            /* Read palette data
            */
            if ( DosRead ( hFile, pbBuffer,
                             pwinData->ulnumColor * sizeof(RGB),
                             &ulNumBytes ))
               {
               DosFreeMem ( (PVOID)pImgHdr );
               DosFreeMem( (PVOID)pbBuffer );
               DosFreeMem( (PVOID)pPalette[iFile] );
               DosClose ( hFile );
               return ( 1 );
               }

            /* Make each color from 3 bytes to 4 bytes.
            */
            pbTmpDst = pPalette[iFile];
            for ( i = 0; i < pwinData->ulnumColor; i++ )
               {
               *pbTmpDst ++= *pbBuffer++;
               *pbTmpDst ++= *pbBuffer++;
               *pbTmpDst ++= *pbBuffer++;
               *pbTmpDst ++= 0;
               } /* endfor */

            DosFreeMem( (PVOID)pbBuffer );
            break;
         case 16:
            pwinData->fccColorFormat = FOURCC_R565;
            break;
         case 24:
            pwinData->fccColorFormat = FOURCC_BGR3;
            break;
         default:
            {
            DosFreeMem ( (PVOID)pImgHdr );
            DosClose ( hFile );
            return ( 1 );
            }
         } /* endswitch */


      }


   /* Allocate a buffer for image data
   */

   if ( DosAllocMem ( (PPVOID)&pbSrcImage[iFile], pwinData->ulSrcLineSizeBytes *
                      pwinData->ulHeight, (ULONG) PAG_COMMIT | PAG_READ |
                      PAG_WRITE) )
      {
      if ( pwinData->fccColorFormat == FOURCC_LUT8 )
         DosFreeMem( (PVOID)pPalette[iFile] );
      DosClose ( hFile );
      return( 1 );
      }

   /* Read image data
   */
   if ( DosRead ( hFile, pbSrcImage[iFile],
                    pwinData->ulSrcLineSizeBytes * pwinData->ulHeight,
                    &ulNumBytes ))
      {
      DosFreeMem ( pbSrcImage[iFile] );
      if ( pwinData->fccColorFormat == FOURCC_LUT8 )
         DosFreeMem( (PVOID)pPalette[iFile] );
      DosClose ( hFile );
      return( 1 );
      }

   /* Close the file and release the access to the image buffer
   */
   DosFreeMem( (PVOID)pImgHdr );
   DosClose ( hFile );
   return ( 0 );
   }



/****************************************************************************
 *
 * Name          : Main
 *
 * Description   : This is main routine of this sample program.
 *
 * Concepts      :
 *
 * Parameters    : At command prompt, when the user start this EXE without
 *                 any parameters, like
 *                    SHOW
 *                 it shows 16 default bitmaps which has a sequence.
 *                 When the user specifies the names of bitmap file, like
 *                    SHOW XXXXXX.BMP YYYYYY.BMP
 *                 it shows the bitmaps specified at command line in turn.
 *
 * Return        : 1 - fail
 *                 0 - suceed
 *
 ****************************************************************************/

main ( int argc, char *argv[] )
   {
   TID       tidBlitThread;        /* Thread ID for DirectMemMove          */
   HMQ       hmq;                  /* Message queue handle                 */
   QMSG      qmsg;                 /* Message from message queue           */
   ULONG     flCreate;             /* Window creation control flags        */
   ULONG     i;                    /* Index for the files to open          */
   PWINDATA  pwinData;             /* Pointer to window data               */
   PLONG     pPal;                 /* Pointer to system physical palette   */


   /* Initialize the presentation manager, and create a message queue.
   */
   hab = WinInitialize ( 0 );
   hmq = WinCreateMsgQueue ( hab, 0 );

   /* Allocate a buffer for the window data
   */
   pwinData = (PWINDATA) malloc (sizeof(WINDATA));

   /* Get the screen capabilities, and if the system support only 16 colors
   ** the sample should be terminated.
   */
   DiveCaps.pFormatData = fccFormats;
   DiveCaps.ulFormatLength = 120;
   DiveCaps.ulStructLen = sizeof(DIVE_CAPS);

   if ( DiveQueryCaps ( &DiveCaps, DIVE_BUFFER_SCREEN ))
      {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
          (PSZ)"usage: The sample program can not run on this system environment.",
          (PSZ)"SHOW.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
      free ( pwinData );
      WinDestroyMsgQueue ( hmq );
      WinTerminate ( hab );
      return ( 1 );
      }

   if ( DiveCaps.ulDepth < 8 )
      {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
          (PSZ)"usage: The sample program can not run on this system environment.",
          (PSZ)"SHOW.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
      free ( pwinData );
      WinDestroyMsgQueue ( hmq );
      WinTerminate ( hab );
      return ( 1 );
      }

   /* Calculate number of bytes per pell
   */
   pwinData->ulColorSizeBytes = DiveCaps.ulScanLineBytes/
                                DiveCaps.ulHorizontalResolution;

   /* Get an instance of DIVE APIs.
   */
   if ( DiveOpen ( &(pwinData->hDive), FALSE, 0 ) )
      {

      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       (PSZ)"usage: The sample program can not run on this system environment.",
                       (PSZ)"SHOW.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
      free ( pwinData );
      WinDestroyMsgQueue ( hmq );
      WinTerminate ( hab );
      return ( 1 );
      }


   /* Read bitmap files
   */
   if ( argc == 1 )                               /* Default case           */
      {                                           /* Read DROI00XX.BMP file */
      for ( i = 0, pwinData->ulMaxFiles = 0; i < MAX_FILE; i ++ )
         {
         if ( ReadFile ( i, pszDefFile[i], pwinData ))
            {
            WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                             (PSZ)"usage: SHOW failed to open bitmaps.",
                             (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
            free ( pwinData );
            WinDestroyMsgQueue ( hmq );
            WinTerminate ( hab );
            return ( 1 );
            }
         else
            pwinData->ulMaxFiles ++;
         }
      }
   else                 /* The case which the user specify bitmap file names */
      {
      if (( argc > 1 ) && ( argc <= MAX_FILE + 1))
         {
         for ( i = 1; i < (ULONG)(argc); i++ )
             {
             if ( ReadFile ( i-1, (unsigned char *)argv[i], pwinData ))
                {
                WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                                 (PSZ)"usage: SHOW failed to open bitmaps.",
                                 (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
                free ( pwinData );
                WinDestroyMsgQueue ( hmq );
                WinTerminate ( hab );
                return ( 1 );
                }
             }
          pwinData->ulMaxFiles = argc-1;
         }
      else
         {
         WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
            (PSZ)"usage: SHOW failed to open bitmaps.",
            (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
         free ( pwinData );
         WinDestroyMsgQueue ( hmq );
         WinTerminate ( hab );
         return ( 1 );
         }
      }

   /* Register a window class, and create a standard window.
   */
   WinRegisterClass ( hab, pszMyWindow, MyWindowProc, 0, sizeof(ULONG) );
   flCreate = FCF_TASKLIST | FCF_SYSMENU  | FCF_TITLEBAR | FCF_ICON |
                  FCF_SIZEBORDER | FCF_MINMAX | FCF_MENU | FCF_SHELLPOSITION;
   pwinData->hwndFrame = WinCreateStdWindow ( HWND_DESKTOP,
                                              WS_VISIBLE, &flCreate,
                                              pszMyWindow,
                                              pszTitleText,
                                              WS_SYNCPAINT | WS_VISIBLE,
                                              0, ID_MAINWND,
                                              &(pwinData->hwndClient));

   WinSetWindowULong (pwinData->hwndClient, 0, (ULONG)pwinData);


   pwinData->cxWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   pwinData->cyWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
   pwinData->cyTitleBar    = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
   pwinData->cyMenu        = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYMENU);

   pwinData->cxWidthWindow  = pwinData->ulWidth + pwinData->cxWidthBorder * 2;
   pwinData->cyHeightWindow = pwinData->ulHeight + (pwinData->cyWidthBorder * 2 +
                                       pwinData->cyTitleBar + pwinData->cyMenu );

   pwinData->cxWindowPos   = ( (LONG)WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN)
                       - pwinData->cxWidthWindow ) / 2;
   pwinData->cyWindowPos   = ( (LONG)WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN)
                       - pwinData->cyHeightWindow ) / 2;


   /* Set the window position and size.
   */
   WinSetWindowPos (pwinData->hwndFrame,
                      HWND_TOP,
                      pwinData->cxWindowPos, pwinData->cyWindowPos,
                      pwinData->cxWidthWindow, pwinData->cyHeightWindow,
                      SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE);


   /* Turn on visible region notification.
   */
   WinSetVisibleRegionNotify ( pwinData->hwndClient, TRUE );

   /* set the flag for the first time simulation of palette of bitmap data
   */
   pwinData->fChgSrcPalette = FALSE;
   pwinData->fStartBlit = FALSE;
   pwinData->fDataInProcess = FALSE;
   pwinData->fDirect = FALSE;

   /* Send an invlaidation message to the client.
   */
   WinPostMsg ( pwinData->hwndFrame, WM_VRNENABLED, 0L, 0L );

   /* Start up the blitting thread.
   */
   if ( DosCreateThread ( &(pwinData->tidBlitThread),
                            (PFNTHREAD) DirectMoveMem,
                            (ULONG)pwinData, 0L, 8192L))
      {
      WinSetVisibleRegionNotify ( pwinData->hwndClient, FALSE );

      for ( i = 0; i < pwinData->ulMaxFiles; i++ )
          DiveFreeImageBuffer ( pwinData->hDive, ulImage[i] );

      DiveClose ( pwinData->hDive );
      WinDestroyWindow ( pwinData->hwndFrame );
      free ( pwinData);
      WinDestroyMsgQueue ( hmq );
      WinTerminate ( hab );
      return ( 1 );
      }

   /* Set the proiroty of the blitting thread
   */
   DosSetPriority ( PRTYS_THREAD, PRTYC_IDLETIME,
                      1, pwinData->tidBlitThread );

   /* While there are still messages, dispatch them.
   */
   while ( WinGetMsg ( hab, &qmsg, 0, 0, 0 ) )
      WinDispatchMsg ( hab, &qmsg );

   /* Set the variable to end the running thread, and wait for it to end.
   */
   ulToEnd = 1;
   DosWaitThread ( &(pwinData->tidBlitThread), DCWW_WAIT );

   /* Turn off visible region notificationm tidy up, and terminate.
   */
   WinSetVisibleRegionNotify ( pwinData->hwndClient, FALSE );

   /* Free the buffers allocated by DIVE and close DIVE
   */
   for  ( i = 0; i < pwinData->ulMaxFiles; i++ )
       DiveFreeImageBuffer ( pwinData->hDive, ulImage[i] );
   DiveClose ( pwinData->hDive );

   if ( pwinData->fccColorFormat == FOURCC_LUT8 )
      {
      for  ( i = 0; i < pwinData->ulMaxFiles; i++ )
          DosFreeMem ( pPalette[i] );
      }

   /* Process for termination
   */
   WinDestroyWindow ( pwinData->hwndFrame );
   free ( pwinData );
   WinDestroyMsgQueue ( hmq );
   WinTerminate ( hab );
   return ( 0 );
   }

/****************************************************************************
 *
 * Name          : MyWindowProc
 *
 * Description   : It is the window procedure of this program.
 *
 * Concepts      :
 *
 * Parameters    : Message parameter 1
 *                 Message parameter 2
 *
 * Return        : calling WinDefWindowProc
 *
 ****************************************************************************/

MRESULT EXPENTRY MyWindowProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
   {
   POINTL    pointl;                /* Point to offset from Desktop         */
   HRGN      hrgn;                  /* Region handle                        */
   HPS       hps;                   /* Presentation Space handle            */
   RECTL     rcl;                   /* Current widow rectangle              */
   ULONG     ulTmpImage;            /* For tmp image number                 */
   RGNRECT   rgnCtl;                /* Processing control structure         */
   PWINDATA  pwinData;              /* Pointer to window data               */
   SETUP_BLITTER SetupBlitter;      /* structure for DiveSetupBlitter       */
   ULONG     ulScanLineBytes;       /* Size of scan line for current window */
   ULONG     ulScanLines;           /* Number of scan lines in window       */
   ULONG     ulRemainder;           /* Used to check direct mode support    */
   ULONG     i, rc;

   /* Get the pointer to window data
   */
   pwinData = (PWINDATA)WinQueryWindowULong (hwnd, 0);
   if ( pwinData )
      {
      switch( msg )
         {
         case WM_COMMAND:
            switch ( SHORT1FROMMP ( mp1 ) )
               {
               case ID_SNAP:
                  {
                  /* Use the initial width and height of the window such that
                  ** the actual video area equals the source width and height.
                  */

                  /* Set the new size of the window, but don't move it.
                  */
                  WinSetWindowPos ( pwinData->hwndFrame, HWND_TOP,
                                      100, 100,
                                      pwinData->cxWidthWindow,
                                      pwinData->cyHeightWindow,
                                      SWP_SIZE | SWP_ACTIVATE | SWP_SHOW );
                  }
                  break;

               case ID_QUERY:

                  /* Display screen capablilities in the dialog box
                  */
                  WinDlgBox ( HWND_DESKTOP, pwinData->hwndClient,
                               MyDlgProc, (HMODULE)0,
                               ID_DIALOG, (PVOID)pwinData );

                  break;

               case ID_EXIT:
                  /* Post to quit the dispatch message loop.
                  */
                  WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
                  break;

               case ID_NEWTEXT:
                  /* Write new text string to the title bar
                  */
                  WinSetWindowText ( pwinData->hwndFrame, (PSZ)mp2 );
                  break;

               case ID_DIRECT:
                  /* Check to make sure that this system can support
                   * direct mode.
                   *
                   * Note: if the screen aperture size is not an even
                   *       multiple of the scan line size, then the
                   *       system will have bank breaks within the
                   *       scan line. This would make the direct mode
                   *       blit alorithm too complicated for this
                   *       sample. Therefore, if this condition exists
                   *       the sample will not go into direct mode
                   */

                  ulRemainder = DiveCaps.ulApertureSize %
                                DiveCaps.ulScanLineBytes;

                  if( !DiveCaps.fScreenDirect ||
                      ( DiveCaps.fBankSwitched && ulRemainder ) )
                  {
                     WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ)"usage: Direct mode not supported in this environment.",
                     (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );

                     break;
                  }

                  pwinData->fDataInProcess = TRUE;

                  /* Set flag for direct frame buffer access
                  */
                  pwinData->fDirect = TRUE;

                  /* Post invalidation message.
                  */
                  WinPostMsg ( hwnd, WM_VRNENABLED, 0L, 0L );
                  break;

               case ID_USEDIVE:
                  pwinData->fDataInProcess = TRUE;

                  /* Set flag for dive blit mode
                  */
                  pwinData->fDirect = FALSE;

                  /* Post invalidation message.
                  */
                  WinPostMsg ( hwnd, WM_VRNENABLED, 0L, 0L );
                  break;

               default:
                  /* Let PM handle this message.
                  */
                  return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
               }
            break;

         case WM_VRNDISABLED:

            pwinData->fDataInProcess = TRUE;
            DiveSetupBlitter ( pwinData->hDive, 0 );
            pwinData->fVrnDisabled = TRUE;
            break;

         case WM_VRNENABLED:
            pwinData->fDataInProcess = TRUE;
            hps = WinGetPS ( hwnd );
            if ( !hps )
               break;
            hrgn = GpiCreateRegion ( hps, 0L, NULL );
            if ( hrgn )
               {
               /* NOTE: If mp1 is zero, then this was just a move message.
               ** Illustrate the visible region on a WM_VRNENABLE.
               */
               WinQueryVisibleRegion ( hwnd, hrgn );
               rgnCtl.ircStart     = 0;
               rgnCtl.crc          = 50;
               rgnCtl.ulDirection  = 1;

               /* Get the all ORed rectangles
               */
               if ( GpiQueryRegionRects ( hps, hrgn, NULL,
                                          &rgnCtl, pwinData->rcls) )
                  {
                  pwinData->ulNumRcls = rgnCtl.crcReturned;

                  /* Now find the window position and size, relative to parent.
                  */
                  WinQueryWindowPos ( pwinData->hwndClient, &pwinData->swp );

                  rcl.xLeft   = 0;
                  rcl.yBottom = 0;

                  /* Convert the point to offset from desktop lower left.
                  */
                  pointl.x = pwinData->swp.x;
                  pointl.y = pwinData->swp.y;

                  WinMapWindowPoints ( pwinData->hwndFrame,
                                       HWND_DESKTOP, &pointl, 1 );

                  pwinData->cxWindowPos = pointl.x;
                  pwinData->cyWindowPos = pointl.y;


                  /* If the sample is in direct mode check to make sure
                  ** that the window is not too large. For direct mode the
                  ** window size is limited to 640x480. This is done to
                  ** avoide excesive swapping.
                  */

                  if ( pwinData->fDirect )
                     {
                     if ( pwinData->swp.cx > 640 )
                        {
                        WinSetWindowPos ( pwinData->hwndFrame, 0, 0, 0,
                                          640, pwinData->swp.cy, SWP_SIZE );
                        return ( FALSE );
                        }

                     if ( pwinData->swp.cy > 480 )
                        {
                        WinSetWindowPos ( pwinData->hwndFrame, 0, 0, 0,
                                          pwinData->swp.cx, 480, SWP_SIZE );
                        return ( FALSE );
                        }
                     }



                  /* Calculate size of scan line bounded by visible window
                  ** and round up to 4 pell increments.
                  */
                  i = pwinData->swp.cx * pwinData->ulColorSizeBytes;
                  pwinData->ulLineSizeBytes = i/4*4;
                  if (i - pwinData->ulLineSizeBytes)
                     pwinData->ulLineSizeBytes += 4;

                  }
               else
                  {
                  DiveSetupBlitter ( pwinData->hDive, 0 );
                  GpiDestroyRegion ( hps, hrgn );
                  break;
                  }

               GpiDestroyRegion( hps, hrgn );
               }
            WinReleasePS( hps );


            /* Tell DIVE about the new settings.
            */
            SetupBlitter.ulStructLen = sizeof ( SETUP_BLITTER );
            SetupBlitter.fccSrcColorFormat = pwinData->fccColorFormat;
            SetupBlitter.ulSrcWidth = pwinData->ulWidth;
            SetupBlitter.ulSrcHeight = pwinData->ulHeight;
            SetupBlitter.ulSrcPosX = 0;
            SetupBlitter.ulSrcPosY = 0;
            SetupBlitter.ulDitherType = 1;
            SetupBlitter.fInvert = TRUE;

            SetupBlitter.lDstPosX = 0;
            SetupBlitter.lDstPosY = 0;
            SetupBlitter.fccDstColorFormat = FOURCC_SCRN;
            SetupBlitter.lScreenPosX = 0;
            SetupBlitter.lScreenPosY = 0;
            SetupBlitter.ulNumDstRects = 1;

            /* If direct mode then stretch blit to video buffers
            */
            if ( pwinData->fDirect )
               {
               rcl.xRight  = pwinData->swp.cx;
               rcl.yTop    = pwinData->swp.cy;
               SetupBlitter.ulDstWidth = pwinData->swp.cx;
               SetupBlitter.ulDstHeight = pwinData->swp.cy;
               SetupBlitter.pVisDstRects = &rcl;
               }
            /* If blit mode then blit to video buffers without stretching
            */
            else
               {
               rcl.xRight = pwinData->ulWidth;
               rcl.yTop   = pwinData->ulHeight;
               SetupBlitter.ulDstWidth = pwinData->ulWidth;
               SetupBlitter.ulDstHeight = pwinData->ulHeight;
               SetupBlitter.pVisDstRects = &rcl;
               }

            DiveSetupBlitter ( pwinData->hDive, &SetupBlitter );

            for (i = 0; i < pwinData->ulMaxFiles ; i++ )
               {
               DiveFreeImageBuffer ( pwinData->hDive, ulImage[i] );


               /* Register source image buffer with DIVE
               */
               ulTmpImage = 0;
               if ( DiveAllocImageBuffer (pwinData->hDive,
                                          &ulTmpImage,
                                          pwinData->fccColorFormat,
                                          pwinData->ulWidth,
                                          pwinData->ulHeight,
                                          pwinData->ulSrcLineSizeBytes,
                                          (PBYTE)pbSrcImage[i] ) )
                  break;

               /* Allocate DIVE image buffer
               */
               if ( DiveAllocImageBuffer (pwinData->hDive,
                                          &(ulImage[i]),
                                          FOURCC_SCRN,
                                          rcl.xRight,
                                          rcl.yTop,
                                          0, 0) )
                  break;

               if ( DiveBeginImageBufferAccess ( pwinData->hDive,
                                                 ulImage[i],
                                                 (PPBYTE)&pbImage[i],
                                                 &ulScanLineBytes,
                                                 &ulScanLines ))
                  {
                  DiveFreeImageBuffer ( pwinData->hDive, ulTmpImage );
                  DiveFreeImageBuffer ( pwinData->hDive, ulImage[i] );
                  break;
                  }

               /* Blit source image to DIVE buffer
               */
               DiveBlitImage ( pwinData->hDive, ulTmpImage, ulImage[i] );

               DiveEndImageBufferAccess ( pwinData->hDive, ulImage[i] );
               DiveFreeImageBuffer ( pwinData->hDive, ulTmpImage );
               }

            /* Now setup blitter to blit to the screen
            ** Note: This code has no affect when in direct mode
            */
            SetupBlitter.fccSrcColorFormat = FOURCC_SCRN;
            SetupBlitter.lScreenPosX = pointl.x;
            SetupBlitter.lScreenPosY = pointl.y;
            SetupBlitter.fInvert = FALSE;
            SetupBlitter.ulDstWidth = pwinData->swp.cx;
            SetupBlitter.ulDstHeight = pwinData->swp.cy;
            SetupBlitter.ulNumDstRects = pwinData->ulNumRcls;
            SetupBlitter.pVisDstRects = pwinData->rcls;

            DiveSetupBlitter ( pwinData->hDive, &SetupBlitter );

            ulFramesToTime=4;
            ulNumFrames=1;
            pwinData->fDataInProcess = FALSE;
            pwinData->fVrnDisabled = FALSE;

            break;


         case WM_REALIZEPALETTE:
            /* This tells DIVE that the physical palette may have changed.
            */
            DiveSetDestinationPalette ( pwinData->hDive, 0, 0, 0 );

            break;

         case WM_CLOSE:
            /* Post to quit the dispatch message loop.
            */
            WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
            break;

         default:
            /* Let PM handle this message.
            */
            return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
         }
      }
   else
      /* Let PM handle this message.
      */
      return WinDefWindowProc ( hwnd, msg, mp1, mp2 );

   return ( FALSE );
   }

/****************************************************************************
 *
 * Name          : MyDlgProc
 *
 * Description   : It is the dialog procedure of this program.
 *
 * Concepts      :
 *
 * Parameters    : Message parameter 1
 *                 Message parameter 2
 *
 * Return        : calling WinDefDlgProc
 *
 ****************************************************************************/

MRESULT EXPENTRY MyDlgProc ( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
   {
   CHAR      string[10];
   CHAR      *pString;

   switch( msg )
      {
      case WM_INITDLG:

         WinSetFocus ( HWND_DESKTOP, hwndDlg );

         if ( !DiveCaps.fScreenDirect )
            WinSetDlgItemText ( hwndDlg, ID_EF_11, "NO" );
         else
            WinSetDlgItemText ( hwndDlg, ID_EF_11, "YES" );

         if ( !DiveCaps.fBankSwitched )
            WinSetDlgItemText ( hwndDlg, ID_EF_12, "NO" );
         else
            WinSetDlgItemText ( hwndDlg, ID_EF_12, "YES" );

         pString = _ultoa ( DiveCaps.ulDepth, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_13, pString );

         pString = _ultoa ( DiveCaps.ulHorizontalResolution,
                             string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_14, pString );

         pString = _ultoa ( DiveCaps.ulVerticalResolution, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_15, pString );

         pString = _ultoa ( DiveCaps.ulScanLineBytes, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_16, pString );

         switch (DiveCaps.fccColorEncoding)
            {
            case FOURCC_LUT8:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "256" );
               break;
            case FOURCC_R565:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "64K" );
               break;
            case FOURCC_R555:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "32K" );
               break;
            case FOURCC_R664:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "64K" );
               break;
            case FOURCC_RGB3:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "16M" );
               break;
            case FOURCC_BGR3:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "16M" );
               break;
            case FOURCC_RGB4:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "16M" );
               break;
            case FOURCC_BGR4:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "16M" );
               break;
            default:
               WinSetDlgItemText ( hwndDlg, ID_EF_17, "???" );
            } /* endswitch */

         pString = _ultoa ( DiveCaps.ulApertureSize, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_18, pString );

         pString = _ultoa ( DiveCaps.ulInputFormats, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_19, pString );

         pString = _ultoa ( DiveCaps.ulOutputFormats, string, 10 );
         WinSetDlgItemText ( hwndDlg, ID_EF_20, pString );

         break;

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mp1 ) )
            {
            case DID_OK:

               WinDismissDlg ( hwndDlg, TRUE );
               break;
            }

      default:
         return ( WinDefDlgProc (hwndDlg, msg, mp1, mp2) );

      }

   return( 0 );

   }


/**************************************************************************
*
* File Name        : FSDIVE.C
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
#define INCL_DOSMISC
#define INCL_GPI
#define INCL_WIN
#define INCL_VIO
#define INCL_GRE_COLORTABLE
#define INCL_GRE_DEVMISC3
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#define  _MEERROR_H_
#include <mmioos2.h>                    /* It is from MMPM toolkit           */
#include <dive.h>
#include <fourcc.h>
#include <builtin.h>
#include <pmddim.h>

#define FSBASE 1

#include "fsdive.h"


#define WS_DesktopDive    0x00000000L   // Desktop dive window style
#define WS_MaxDesktopDive 0x00000001L   // Maximized desktop dive window style
#define WS_FullScreenDive 0x00000002L   // Full-screen 320x200x256 dive style

// Function prototypes

MRESULT EXPENTRY MyWindowProc ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY MyDlgProc ( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 );

// Global definitions

HAB       hab;                          /* PM anchor block handle            */
ULONG     ulImage[MAX_FILE];            /* Image buffer numbers from Dive    */
PBYTE     pPalette[MAX_FILE];           /* Pointer to bitmap palette area    */
PSZ       pszMyWindow = "MyWindow";     /* Window class name                 */
PSZ       pszTitleText = "DIVE SAMPLE"; /* Title bar text                    */
ULONG     ulToEnd = 0;                  /*                                   */
DIVE_CAPS DiveCaps = {0};
FOURCC    fccFormats[100] = {0};
ULONG     ulFramesToTime=8;             /* Interval of frames to get time    */
ULONG     ulNumFrames=0;                /* Frame counter                     */
ULONG ulWindowStyle = WS_DesktopDive;   // current window style
BOOL      fFSBase = TRUE;               // FS DIVE support in base OS

//  Default bitmap file name definitions
//       These files are used only when EXE is called without parameter.

PSZ  pszDefFile[] =
{
    {"TPG20000.BMP"},
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
    {"TPG20015.BMP"}
};

//  It is the definition how many time draw each bitmap

ULONG ulDrawCnt[] =
{
    1,                                  /* For TPG20000.BMP  */
    1,                                  /* For TPG20001.BMP  */
    1,                                  /* For TPG20002.BMP  */
    1,                                  /* For TPG20003.BMP  */
    1,                                  /* For TPG20004.BMP  */
    1,                                  /* For TPG20005.BMP  */
    1,                                  /* For TPG20006.BMP  */
    1,                                  /* For TPG20007.BMP  */
    1,                                  /* For TPG20008.BMP  */
    1,                                  /* For TPG20009.BMP  */
    1,                                  /* For TPG20010.BMP  */
    1,                                  /* For TPG20011.BMP  */
    1,                                  /* For TPG20012.BMP  */
    1,                                  /* For TPG20013.BMP  */
    1,                                  /* For TPG20014.BMP  */
    1                                   /* For TPG20015.BMP  */
};

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
    ULONG    ulTime0, ulTime1;          /* output buffer for DosQierySysInfo      */
    ULONG    ulIndexImage = 0;          /* Index of bitmap data                   */
    CHAR     achFrameRate[48];          /* string buffer for WinSetWindowText     */
    ULONG    ulFirstTime=0;             /* very first time flag for DiveSetSour.. */

    PWINDATA pwinData;                  /* pointer to window data                 */
    ULONG    ulCount = 0;               /* Counter for each bitmap                */

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

        /* Blit the image.
        */
        DiveBlitImage ( pwinData->hDive,
                ulImage[ulIndexImage],
                DIVE_BUFFER_SCREEN );

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
    HFILE hFile;                        /* file handke                              */
    ULONG ulNumBytes;                   /* output for number of bytes actually read */
    ULONG ulFileLength;                 /* file length                              */
    PBYTE pbBuffer;                     /* pointer to the image/ temp. palette data */
    ULONG ulScanLineBytes;              /* output for number of bytes a scan line   */
    ULONG ulScanLines;                  /* output for height of the buffer          */
    PBITMAPFILEHEADER2 pImgHdr;         /* pointer to bitmapheader                  */
    ULONG i, j;
    PBYTE pbTmpDst;                     /* temporaly destination pointer            */

    /* Attempt to open up the passed filename.
    */
    if ( DosOpen ( pszFile, &hFile, &ulNumBytes, 0L, FILE_NORMAL,
         OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
         OPEN_ACCESS_READONLY | OPEN_SHARE_DENYREADWRITE |
         OPEN_FLAGS_SEQUENTIAL | OPEN_FLAGS_NO_CACHE, 0L ) )
        return( 1 );

    /* Get the legnth of the file.
    */
    DosSetFilePtr ( hFile, 0L, FILE_END, &ulFileLength );
    DosSetFilePtr ( hFile, 0L, FILE_BEGIN, &ulNumBytes );

                                        /* Allocate memory for bitmap file header
    */
    if ( DosAllocMem ( (PPVOID)&pImgHdr, sizeof(BITMAPFILEHEADER2),
         (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
    {
        DosClose( hFile );
        return( 1 );
    }

    /* Read from the beginning of the header to cbFix in BITMAPINFOHEADER
    ** to know the length of the header.
    */
    if ( DosRead ( hFile, pImgHdr,
         sizeof(BITMAPFILEHEADER2) - sizeof(BITMAPINFOHEADER2) +
         sizeof(ULONG),
         &ulNumBytes ))
    {
        DosFreeMem( (PVOID)pImgHdr );
        DosClose( hFile );
        return( 1 );
    }

    /* Read the rest of the header.
    */
    if ( DosRead ( hFile, (PBYTE)pImgHdr + ulNumBytes,
         pImgHdr->bmp2.cbFix - sizeof(ULONG),
         &ulNumBytes ))
    {
        DosFreeMem( (PVOID)pImgHdr );
        DosClose( hFile );
        return( 1 );
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
                DosFreeMem( (PVOID)pImgHdr );
                DosClose( hFile );
                return( 1 );
            }

            /* Read palette data.
            */
            if ( DosRead ( hFile, pPalette[iFile],
                 pwinData->ulnumColor * sizeof(ULONG),
                 &ulNumBytes ))
            {
                DosFreeMem( (PVOID)pImgHdr );
                DosFreeMem( (PVOID)(pPalette[iFile]) );
                DosClose( hFile );
                return( 1 );
            }
            break;
        case 16:
            pwinData->fccColorFormat = FOURCC_R565;
            break;
        case 24:
            pwinData->fccColorFormat = FOURCC_BGR3;
            break;
        default:
            DosFreeMem( (PVOID)pImgHdr );
            DosClose( hFile );
            return( 1 );
        }                               /* endswitch */

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
            if ( DosAllocMem( (PPVOID) &pbBuffer,
                 pwinData->ulnumColor * sizeof(RGB),
                 (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
            {
                DosFreeMem( (PVOID)pImgHdr );
                DosClose( hFile );
                return( 1 );
            }
            if ( DosAllocMem ( (PPVOID) &(pPalette[iFile]),
                 pwinData->ulnumColor * sizeof(ULONG),
                 (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE) )
            {
                DosFreeMem( (PVOID)pImgHdr );
                DosFreeMem( (PVOID)pbBuffer );
                DosClose( hFile );
                return( 1 );
            }

            /* Read palette data
            */
            if ( DosRead ( hFile, pbBuffer,
                 pwinData->ulnumColor * sizeof(RGB),
                 &ulNumBytes ))
            {
                DosFreeMem( (PVOID)pImgHdr );
                DosFreeMem( (PVOID)pbBuffer );
                DosFreeMem( (PVOID)pPalette[iFile] );
                DosClose( hFile );
                return( 1 );
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
            }   /* endfor */

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
                DosFreeMem( (PVOID)pImgHdr );
                DosClose( hFile );
                return( 1 );
            }
        } /* endswitch */


    }
    /* Allocate a buffer for image data
    */

    if ( DiveAllocImageBuffer( pwinData->hDive,
         &(ulImage[iFile]),
         pwinData->fccColorFormat,
         pwinData->ulWidth,
         pwinData->ulHeight,
         0, 0 ) )
    {
        if ( pwinData->fccColorFormat == FOURCC_LUT8 )
            DosFreeMem(( PVOID)pPalette[iFile] );
        DosClose( hFile );
        return( 1 );
    }
    if ( DiveBeginImageBufferAccess( pwinData->hDive,
         ulImage[iFile],
         &pbBuffer,
         &ulScanLineBytes,
         &ulScanLines ))
    {
        DiveFreeImageBuffer( pwinData->hDive, ulImage[iFile] );
        if ( pwinData->fccColorFormat == FOURCC_LUT8 )
            DosFreeMem( (PVOID)pPalette[iFile] );
        DosClose( hFile );
        return( 1 );
    }

    /* Read image data
    */
    if ( DosRead( hFile, pbBuffer,
         pwinData->ulSrcLineSizeBytes * pwinData->ulHeight,
         &ulNumBytes ) )
    {
        DiveEndImageBufferAccess( pwinData->hDive, ulImage[iFile] );
        DiveFreeImageBuffer( pwinData->hDive, ulImage[iFile] );
        if ( pwinData->fccColorFormat == FOURCC_LUT8 )
        {
            DosFreeMem( (PVOID)pPalette[iFile] );
        }
        DosClose( hFile );
        return( 1 );
    }

    /* Close the file and release the access to the image buffer
    */
    DosFreeMem( (PVOID)pImgHdr );
    DosClose( hFile );

    DiveEndImageBufferAccess ( pwinData->hDive, ulImage[iFile] );

    return( 0 );
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
 *                    FSDIVE
 *                 it shows 16 default bitmaps which has a sequence.
 *                 When the user specifies the names of bitmap file, like
 *                    FSDIVE XXXXXX.BMP YYYYYY.BMP
 *                 it shows the bitmaps specified at command line in turn.
 *
 * Return        : 1 - fail
 *                 0 - suceed
 *
 ****************************************************************************/

main( int argc, char **argv )
{
    TID       tidBlitThread;            /* Thread ID for DirectMemMove          */
    HMQ       hmq;                      /* Message queue handle                 */
    QMSG      qmsg;                     /* Message from message queue           */
    ULONG     flCreate;                 /* Window creation control flags        */
    ULONG     i;                        /* Index for the files to open          */
    PWINDATA  pwinData;                 /* Pointer to window data               */
    PLONG     pPal;                     /* Pointer to system physical palette   */
    LONG      cxWidthBorder;            /* Size of width of vertical border     */
    LONG      cyWidthBorder;            /* Size of width of horizontal border   */
    LONG      cyTitleBar;               /* Size of title bar                    */
    LONG      cyMenu;                   /* Size of menu                         */
    LONG      cxWindowPos;              /* X position of frame window           */
    LONG      cyWindowPos;              /* Y position of frmae window           */

    UCHAR     szErrorBuf[256];          // GameSrvr
    HMODULE   hmodGameSrvr;             // GameSrvr
    PFN       pfnInitGameFrameProc;     // GameSrvr

    ULONG     aulVersion[2];            // OS/2 version number


// Initialize the presentation manager, and create a message queue.

    hab = WinInitialize ( 0 );
    hmq = WinCreateMsgQueue ( hab, 0 );

// Allocate a buffer for the window data

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
                (PSZ)"FSDIVE.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
        free( pwinData );
        WinDestroyMsgQueue( hmq );
        WinTerminate( hab );
        return( 1 );
    }

    if ( DiveCaps.ulDepth < 8 )
    {
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                (PSZ)"usage: The sample program can not run on this system environment.",
                (PSZ)"FSDIVE.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
        free( pwinData );
        WinDestroyMsgQueue( hmq );
        WinTerminate( hab );
        return( 1 );
    }

    /* Get an instance of DIVE APIs.
    */
    if ( DiveOpen( &(pwinData->hDive), FALSE, 0 ) )
    {

        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                (PSZ)"usage: The sample program can not run on this system environment.",
                (PSZ)"FSDIVE.EXE - DIVE Sample", 0, MB_OK | MB_INFORMATION );
        free( pwinData );
        WinDestroyMsgQueue( hmq );
        WinTerminate( hab );
        return( 1 );
    }

    // Read bitmap files

    if ( argc == 1 )      // Default case
    {      // Read BMP file
        for ( i = 0, pwinData->ulMaxFiles = 0; i < MAX_FILE; i ++ )
        {
            if ( ReadFile ( i, pszDefFile[i], pwinData ))
            {
                WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                        (PSZ)"usage: FSDIVE failed to open bitmaps.",
                        (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );

                return( 1 );
            }
            else
                pwinData->ulMaxFiles ++;
        }
    }
    else    // The case which the user specify bitmap file names
    {
        if (( argc > 1 ) && ( argc <= MAX_FILE + 1))
        {
            for ( i = 1; i < (ULONG)(argc); i++ )
            {
                if ( ReadFile( i-1, (unsigned char *)argv[i], pwinData ) )
                {
                    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                            (PSZ)"usage: FSDIVE failed to open bitmaps.",
                            (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
                    free ( pwinData );
                    WinDestroyMsgQueue( hmq );
                    WinTerminate( hab );
                    return( 1 );
                }
            }
            pwinData->ulMaxFiles = argc-1;
        }
        else
        {
            WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                    (PSZ)"usage: FSDIVE failed to open bitmaps.",
                    (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
            free( pwinData );
            WinDestroyMsgQueue( hmq );
            WinTerminate( hab );
            return( 1 );
        }
    }

// Register a window class, and create a standard window.

    WinRegisterClass( hab, pszMyWindow, MyWindowProc, 0, sizeof(ULONG) );

    flCreate = FCF_TASKLIST | FCF_SYSMENU  | FCF_TITLEBAR | FCF_ICON |
    FCF_SIZEBORDER | FCF_MINMAX | FCF_MENU | FCF_SHELLPOSITION;

    pwinData->hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
            WS_VISIBLE,
            &flCreate,
            pszMyWindow,
            pszTitleText,
            WS_SYNCPAINT | WS_VISIBLE,
            0, ID_MAINWND,
            &(pwinData->hwndClient));

    WinSetWindowULong( pwinData->hwndClient, 0, (ULONG)pwinData );


    DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR, aulVersion, 8 );

    if (aulVersion[0] == 20 && aulVersion[1] <= 30 )
    {
       fFSBase = FALSE;

       // GameSrvr ----------------------------------------------------begin---

       if ( 0 == DosLoadModule( szErrorBuf, 256, "GAMESRVR", &hmodGameSrvr ) )
       {
           if ( 0 == DosQueryProcAddr( hmodGameSrvr, 1, 0, &pfnInitGameFrameProc ) )
           {
               ( pfnInitGameFrameProc )( pwinData->hwndFrame, 0 );
           }
           {
               ULONG pvmi;
               ULONG ul;

               WinSendMsg( pwinData->hwndFrame, WM_GetVideoModeTable, (MPARAM)&pvmi, (MPARAM)&ul );
           }
       }
       else
       {
           WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)"usage: FSDIVE failed to load GAMESRVR.DLL.",
                   (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
       }

       // GameSrvr ----------------------------------------------------end-----
    }
    else
    {
       fFSBase = TRUE;
       DosLoadModule( szErrorBuf, 256, "PMMERGE", &hmodGameSrvr );
       if ( 0 == DosQueryProcAddr( hmodGameSrvr, 6099, 0, &pfnInitGameFrameProc ) )
       {
           ( pfnInitGameFrameProc )( pwinData->hwndFrame, 0 );
       }
       else
       {
           WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)"usage: FSDIVE failed to access FS functions.",
                   (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
       }
    }


    cxWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
    cyWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
    cyTitleBar    = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
    cyMenu        = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYMENU);

    pwinData->cxWidthWindow  = pwinData->ulWidth + cxWidthBorder * 2;
    pwinData->cyHeightWindow = pwinData->ulHeight + (cyWidthBorder * 2 +
            cyTitleBar + cyMenu );

    cxWindowPos   = ( (LONG)WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN)
            - pwinData->cxWidthWindow ) / 2;
    cyWindowPos   = ( (LONG)WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN)
            - pwinData->cyHeightWindow ) / 2;


    /* Set the window position and size.
    */
    WinSetWindowPos (pwinData->hwndFrame,
            HWND_TOP,
            cxWindowPos, cyWindowPos,
            pwinData->cxWidthWindow, pwinData->cyHeightWindow,
            SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE);


    /* Turn on visible region notification.
    */
    WinSetVisibleRegionNotify( pwinData->hwndClient, TRUE );

    /* set the flag for the first time simulation of palette of bitmap data
    */
    pwinData->fChgSrcPalette = FALSE;

    /* Send an invlaidation message to the client.
    */
    WinPostMsg( pwinData->hwndFrame, WM_VRNENABLED, 0L, 0L );

    /* Start up the blitting thread.
    */
    if ( DosCreateThread( &(pwinData->tidBlitThread),
         (PFNTHREAD)DirectMoveMem,
         (ULONG)pwinData, 0L, 8192L))
    {
        WinSetVisibleRegionNotify( pwinData->hwndClient, FALSE );

        for ( i = 0; i < pwinData->ulMaxFiles; i++ )
        {
            DiveFreeImageBuffer( pwinData->hDive, ulImage[i] );
        }
        DiveClose( pwinData->hDive );
        WinDestroyWindow( pwinData->hwndFrame );
        free( pwinData );
        WinDestroyMsgQueue( hmq );
        WinTerminate( hab );
        return( 1 );
    }

// Set the proiroty of the blitting thread

    DosSetPriority( PRTYS_THREAD, PRTYC_IDLETIME,
            1, pwinData->tidBlitThread );

// While there are still messages, dispatch them.

    while ( WinGetMsg( hab, &qmsg, 0, 0, 0 ) )
    {
        WinDispatchMsg( hab, &qmsg );
    }

// Set the variable to end the running thread, and wait for it to end.

    ulToEnd = 1;
    DosWaitThread( &(pwinData->tidBlitThread), DCWW_WAIT );

// Turn off visible region notificationm tidy up, and terminate.

    WinSetVisibleRegionNotify( pwinData->hwndClient, FALSE );

// Free the buffers allocated by DIVE and close DIVE

    for  ( i = 0; i < pwinData->ulMaxFiles; i++ )
    {
        DiveFreeImageBuffer( pwinData->hDive, ulImage[i] );
    }
    DiveClose( pwinData->hDive );

    if ( pwinData->fccColorFormat == FOURCC_LUT8 )
    {
        for  ( i = 0; i < pwinData->ulMaxFiles; i++ )
        {
            DosFreeMem( pPalette[i] );
        }
    }

// Process termination

    WinDestroyWindow( pwinData->hwndFrame );
    free( pwinData );
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );
    return( 0 );
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
    POINTL    pointl;                   /* Point to offset from Desktop    */
    SWP       swp;                      /* Window position                 */
    HRGN      hrgn;                     /* Region handle                   */
    HPS       hps;                      /* Presentation Space handle       */
    RECTL     rcls[50];                 /* Rectangle coordinates           */
    RGNRECT   rgnCtl;                   /* Processing control structure    */
    PWINDATA  pwinData;                 /* Pointer to window data          */
    SETUP_BLITTER SetupBlitter;         /* structure for DiveSetupBlitter  */
    PLONG     pPal;
    HWND      hwndDialog;

    PQMSG     pqmsg;

    // Get the pointer to window data
    //
    pwinData = (PWINDATA)WinQueryWindowULong (hwnd, 0);
    if( pwinData  )
    {
        switch( msg )
        {
        case WM_TRANSLATEACCEL: // mp1 is pointer to QMSG with WM_CHAR

            if ( fFSBase )
            {
               pqmsg = (PQMSG)mp1;

               // Alt-Home to toggle between Full Screen DIVE and desk top

               if ( SHORT2FROMMP( pqmsg->mp2 ) == VK_HOME // Alt-Home key
                    && ( SHORT1FROMMP( pqmsg->mp1 )
                    & ( KC_ALT | KC_VIRTUALKEY | KC_KEYUP ) )
                    == ( KC_ALT | KC_VIRTUALKEY ) )
               {
                   if ( ulWindowStyle == WS_DesktopDive )
                   {
                       WinSendMsg( pwinData->hwndFrame, WM_SetVideoMode, (MPARAM)WS_FullScreenDive, 0 );
                       ulWindowStyle = WS_FullScreenDive;
                   }
                   else
                   {
                       WinSendMsg( pwinData->hwndFrame, WM_SetVideoMode, (MPARAM)WS_DesktopDive, 0 );
                       ulWindowStyle = WS_DesktopDive;
                   }
               }
               else
                  return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
            }
            else
               return WinDefWindowProc ( hwnd, msg, mp1, mp2 );

            break;

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

                // Get the screen capabilities
                // to display in the dialog box.

                DiveCaps.pFormatData = fccFormats;
                DiveCaps.ulFormatLength = 120;
                DiveCaps.ulStructLen = sizeof(DIVE_CAPS);

                if ( DiveQueryCaps ( &DiveCaps, DIVE_BUFFER_SCREEN ))
                {
                    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                            (PSZ)"usage: FSDIVE failed to get screen capabilities.",
                            (PSZ)"Error!", 0, MB_OK | MB_MOVEABLE );
                    break;
                }

                WinDlgBox ( HWND_DESKTOP, pwinData->hwndClient,
                        MyDlgProc, (HMODULE)0,
                        ID_DIALOG, (PVOID)pwinData );

                break;

            case ID_FULLSCR:            // GameSrvr

                WinSendMsg( pwinData->hwndFrame, WM_SetVideoMode, (MPARAM)WS_FullScreenDive, 0 ); // GameSrvr
                if ( fFSBase )
                   ulWindowStyle = WS_FullScreenDive;
                break;                  // GameSrvr

            case ID_EXIT:               // Post to quit the dispatch message loop.

                WinSendMsg( pwinData->hwndFrame, WM_SetVideoMode, (MPARAM)WS_DesktopDive, 0 ); // GameSrvr
                WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
                break;

            case ID_NEWTEXT:
                /* Write new text string to the title bar
                */
                WinSetWindowText ( pwinData->hwndFrame, (PSZ)mp2 );
                break;

            default:
                /* Let PM handle this message.
                */
                return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
            }
            break;

        case WM_VRNDISABLED:

            DiveSetupBlitter ( pwinData->hDive, 0 );
            break;

        case WM_VRNENABLED:

            hps = WinGetPS ( hwnd );

            if ( !hps )
                break;

            hrgn = GpiCreateRegion ( hps, 0L, NULL );

            if ( hrgn )
            {
            //      NOTE: If mp1 is zero, then this was just a move message.
            //      Illustrate the visible region on a WM_VRNENABLE.

                WinQueryVisibleRegion ( hwnd, hrgn );
                rgnCtl.ircStart     = 0;
                rgnCtl.crc          = 50;
                rgnCtl.ulDirection  = 1;

                // Get the all ORed rectangles

                if ( GpiQueryRegionRects ( hps, hrgn, NULL, &rgnCtl, rcls) )
                {

                // Now find the window position and size, relative to parent.

                    WinQueryWindowPos ( pwinData->hwndClient, &swp );

                // Convert the point to offset from desktop lower left.

                    pointl.x = swp.x;
                    pointl.y = swp.y;

                    WinMapWindowPoints ( pwinData->hwndFrame,
                            HWND_DESKTOP, &pointl, 1 );

                // Tell DIVE about the new settings.

                    SetupBlitter.ulStructLen       = sizeof( SETUP_BLITTER );
                    SetupBlitter.fccSrcColorFormat = pwinData->fccColorFormat;
                    SetupBlitter.ulSrcWidth        = pwinData->ulWidth;
                    SetupBlitter.ulSrcHeight       = pwinData->ulHeight;
                    SetupBlitter.ulSrcPosX         = 0;
                    SetupBlitter.ulSrcPosY         = 0;
                    SetupBlitter.fInvert           = TRUE;
                    SetupBlitter.ulDitherType      = 1;

                    SetupBlitter.fccDstColorFormat = FOURCC_SCRN;
                    SetupBlitter.ulDstWidth        = swp.cx;
                    SetupBlitter.ulDstHeight       = swp.cy;
                    SetupBlitter.lDstPosX          = 0;
                    SetupBlitter.lDstPosY          = 0;
                    SetupBlitter.lScreenPosX       = pointl.x;
                    SetupBlitter.lScreenPosY       = pointl.y;
                    SetupBlitter.ulNumDstRects     = rgnCtl.crcReturned;
                    SetupBlitter.pVisDstRects      = rcls;

                    DiveSetupBlitter ( pwinData->hDive, &SetupBlitter );

                    ulFramesToTime = 4;
                    ulNumFrames    = 1;
                }
                else
                {
                    DiveSetupBlitter ( pwinData->hDive, 0 );
                }

                GpiDestroyRegion( hps, hrgn );
            }
            WinReleasePS( hps );
            break;

        case WM_REALIZEPALETTE:
            /* This tells DIVE that the physical palette may have changed.
            */
            DiveSetDestinationPalette ( pwinData->hDive, 0, 0, 0 );

            break;

        case WM_CLOSE:
             // Post to quit the dispatch message loop.

            WinPostMsg ( hwnd, WM_QUIT, 0L, 0L );
            break;

        default:  // Let PM handle this message.

            return WinDefWindowProc( hwnd, msg, mp1, mp2 );
        }
    }
    else
    {                                   // Let PM handle this message.
        return WinDefWindowProc ( hwnd, msg, mp1, mp2 );
    }

    return( FALSE );
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

        if ( !DiveCaps.fScreenDirect )
            WinSetDlgItemText( hwndDlg, ID_EF_11, "NO" );
        else
            WinSetDlgItemText( hwndDlg, ID_EF_11, "YES" );

        if ( !DiveCaps.fBankSwitched )
            WinSetDlgItemText( hwndDlg, ID_EF_12, "NO" );
        else
            WinSetDlgItemText( hwndDlg, ID_EF_12, "YES" );

        pString = _ultoa ( DiveCaps.ulDepth, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_13, pString );

        pString = _ultoa ( DiveCaps.ulHorizontalResolution,
                string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_14, pString );

        pString = _ultoa ( DiveCaps.ulVerticalResolution, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_15, pString );

        pString = _ultoa ( DiveCaps.ulScanLineBytes, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_16, pString );

        switch (DiveCaps.fccColorEncoding)
        {
        case FOURCC_LUT8:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "256" );
            break;
        case FOURCC_R565:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "64K" );
            break;
        case FOURCC_R555:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "32K" );
            break;
        case FOURCC_R664:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "64K" );
            break;
        case FOURCC_RGB3:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "16M" );
            break;
        case FOURCC_BGR3:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "16M" );
            break;
        case FOURCC_RGB4:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "16M" );
            break;
        case FOURCC_BGR4:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "16M" );
            break;
        default:
            WinSetDlgItemText( hwndDlg, ID_EF_17, "???" );
        }                               /* endswitch */

        pString = _ultoa ( DiveCaps.ulApertureSize, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_18, pString );

        pString = _ultoa ( DiveCaps.ulInputFormats, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_19, pString );

        pString = _ultoa ( DiveCaps.ulOutputFormats, string, 10 );
        WinSetDlgItemText( hwndDlg, ID_EF_20, pString );

        break;

    case WM_COMMAND:
        switch ( SHORT1FROMMP ( mp1 ) )
        {
        case DID_OK:

            WinDismissDlg( hwndDlg, TRUE );
            break;
        }

    default:
        return( WinDefDlgProc (hwndDlg, msg, mp1, mp2) );

    }

   return( FALSE );
}

/*static char *SCCSID = "@(#)convproc.c	13.4 92/03/06";*/
#pragma title ("CONVPROC.C - Case Convert I/O Procedure")

/********************** START OF SPECIFICATIONS *****************************/
/*                                                                          */
/* SOURCE FILE NAME: CONVPROC.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Multi-Media I/O Procedure                              */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*              Copyright (c) IBM Corporation  1992, 1993                   */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: MMPM/2 Release 1.10                                              */
/*                                                                          */
/* ABSTRACT:                                                                */
/*                                                                          */
/*********************** END OF SPECIFICATIONS ******************************/

#define INCL_NOPMAPI                   // no PM include files required
#define INCL_DOSFILEMGR                // need for Dos File I/O API's
#define INCL_CONVPROC                  // use only include CONV proc info

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mmioos2.h>                   // public include files
#include "hhpheap.h"                   // MMPM/2 Heap Manager
#include "convproc.h"                  // header defs for this I/O proc

#pragma subtitle ("CONVPROC - Case Convert I/O procedure")
#pragma page ()

/**************************************************************************
**   CONVProc                                                            **
***************************************************************************
*
* ARGUMENTS:
*
*     PSZ pmmioStr - pointer to MMIOINFO block
*     USHORT usMsg - MMIO message being sent
*     LONG lParam1 - filename or other parameter depending on message
*     LONG lParam2 - used with some messages as values
*
*
* RETURN:
*
*     MMIOM_OPEN
*         Success           - MMIO_SUCCESS     (0)
*         Failure           - Return > 0
*
*     MMIOM_READ
*         Success           - Return code of x for number of bytes read
*                           - 0 if the end of the file was reached, or
*         Failure           - MMIO_ERROR      (-1)
*
*     MMIOM_WRITE
*         Success           - Return code of x for number of bytes written
*         Failure           - MMIO_ERROR      (-1)
*
*     MMIOM_SEEK
*         Success           - Return code of x indicates current file position
*         Failure           - MMIO_ERROR      (-1)
*
*     MMIOM_CLOSE
*         Success           - MMIO_SUCCESS     (0)
*         Failure           - Return > 0
*
*     MMIOM_GETFORMATNAME
*         Success           - Returns the number of bytes read into
*                             the buffer (size of format name)
*         Failure           - Return 0
*
*     MMIOM_GETFORMATINFO
*         Success           - MMIO_SUCCESS     (0)
*         Failure           - MMIO_ERROR      (-1)
*
*     MMIOM_QUERYHEADERLENGTH
*         Success           - Returns the size of the header in bytes
*         Failure           - Return 0
*
*     MMIOM_IDENTIFYFILE
*         Success           - MMIO_SUCCESS     (0)
*         Failure           - MMIO_ERROR      (-1)
*
*     MMIOM_GETHEADER
*         Success           - Returns number of bytes copied into
*                             the header structure.
*         Failure           - Return 0
*         Other             - If length passed in was not large
*                             enough to hold header then,
*                             MMIOERR_INVALID_BUFFER_LENGTH is set
*                             in ulErrorRet.
*                           - If header is bad,
*                             MMIOERR_INVALID_STRUCTURE is set in
*                             ulErrorRet
*
*     MMIOM_SETHEADER
*         Success           - Returns number of bytes written
*         Failure           - Return 0
*         Other             - If header is bad,
*                             MMIOERR_INVALID_STRUCTURE is set in
*                             ulErrorRet
*
*     CONV_TOUPPER - Returns buffer of uppercase-converted characters.
*     CONV_TOLOWER - Returns buffer of lowercase-converted characters.
*
* DESCRIPTION:
*
*     This routine will attempt I/O functions on a DOS type file, and
*     allow custom messages to be sent that provide case conversion of text.
*
* GLOBAL VARS REFERENCED:
*
* GLOBAL VARS MODIFIED:
*
* NOTES:
*
* SIDE EFFECTS:
*
***************************************************************************/

LONG APIENTRY CONVProc( PVOID pmmioStr,
                        USHORT  usMsg,
                        LONG lParam1,
                        LONG lParam2 )

{
   PCONVPROCINFO pInfo;             // Converter I/O Procedure Info Block
   PMMIOINFO pmmioinfo;             // I/O information block
   MMIOINFO mmioinfoSS;             // I/O info block for Storage System
   PMMFORMATINFO pmmformatinfo;     // formatinfo for this ioproc.
   PCONVHEADERINFO pconvHeaderInfo; // pointer to header struct for FFT files.
   PSZ pszFileName;                 // file name passed in from caller
   PSZ pszFormatString;             // format string to return.
   PSZ pszData;
   CHAR szHeaderLength[2];          // storage for header length (USHORT)
   PCHAR pTemp;                     // temp pointer for use in Header manip.
   HFILE hFileHandle;               // file handle created or passed in
   HMMIO hmmioSS;                   // handle for Storage System
   FOURCC fccStorageSystem;         // SS I/O Proc FOURCC
   USHORT usReturnCode;             // return code from mmioClose
   ULONG ulReturnCode;              // return code from mmio API calls
   LONG lReturnCode;                // return code from mmio API calls
   LONG lBytesCopied;               // num of bytes of format string.
   LONG lBytesRead;                 // return from mmioRead
   LONG lBytesWritten;              // return from mmioWrite
   LONG lHeaderLength;              // storage for CONV header length input
   LONG lFilePosition;              // return from mmioSeek
   LONG lSavedFilePosition;         // saved LFP for the file.
   ULONG ulTempFlags;               // temp flags for flags to be removed.

   /*
    *  Initalize local file handle and Return Codes.
    */

   hFileHandle = 0L;
   ulReturnCode = 0L;
   usReturnCode = 0;

   /*
    * Clear the error return before anything happens to insure valid results.
    */

   if (pmmioStr)
      {
      pmmioinfo = (PMMIOINFO) pmmioStr;
      pInfo = (PCONVPROCINFO) &pmmioinfo->aulInfo;
      pmmioinfo->ulErrorRet = MMIO_SUCCESS;
      }
   else
      {
      pmmioinfo = NULL;
      pInfo = NULL;
      }

   /*
    *  Route the MMIO message to the proper code handler.
    */

   switch( usMsg )
      {
      case MMIOM_OPEN:

         /*
          * Get the filename from parameter. Then create a File Format header
          * in memory since this message will use this structure.
          */

         pszFileName = (CHAR *)lParam1;
         pconvHeaderInfo =
            (PCONVHEADERINFO)HhpAllocBuffer( sizeof(CONVHEADERINFO), 0);
         if (!pconvHeaderInfo)
            {
            return (MMIO_ERROR);
            }

         /*
          * If no Storage System I/O proc was determined from mmioOpen,
          * either determine the SS from the name (CREATE case) or
          * search I/O proc list for SS type. If the file is being created
          * and the storage system cannot be determined from the name,
          * default the storage system to DOS.
          */

         if (!pmmioinfo->fccChildIOProc)
            {
            /*
             * Since no Storage system has been determined from mmioOpen, we
             * need to determine the Storage system from the filename. If
             * it cannot be determined from the name, since it's a create we
             * will default the storage system to DOS.
             */

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

               /*
                * The file already exists, so we need to determine the storage
                * system by looping through all the SS I/O procs until the SS
                * is determined or it is not. If it is not then this file
                * cannot be opened, so return an error.
                */

               if (mmioIdentifyStorageSystem( pszFileName,
                                              pmmioinfo,
                                              &fccStorageSystem ))
                  {
                  return (MMIO_ERROR);
                  }
               }

            /*
             * Now we either have a SS, and if so assign it to the SS I/O
             * proc field of the pmmioinfo sent in, or we don't so return.
             */

            if (!fccStorageSystem)
               {
               return (MMIO_ERROR);
               }
            else
               {
               pmmioinfo->fccChildIOProc = fccStorageSystem;
               }
            }

         /*
          * We have the Storage System FOURCC so open the SS.
          * To set up the mmioOpen call to the storage system :
          *
          *  1. Initialize the mmioinfo passed in for the Storage System.
          *  2. Set fccIOProc to the Storage System FOURCC.
          *  3. Save the hmmcf handle in aulInfo[1] if sent in.
          *  4. IMPORTANT: Use flags sent in EXCEPT Buffered I/O flags.
          *     VERY IMPORTANT: Set the NOIDENTIFY flag before calling the
          *     mmioOpen below to avoid and endless loop.
          *  5. The open call will handle the DELETE flag.
          *  6. Use the name passed in to this I/O proc for the open because
          *     at this point the SS I/O proc will know how to deal with it.
          */

         memset( &mmioinfoSS, '\0', sizeof(MMIOINFO));
         mmioinfoSS.fccIOProc = pmmioinfo->fccChildIOProc;
         memmove( &mmioinfoSS.aulInfo, pmmioinfo->aulInfo,(4*sizeof(ULONG)));
         mmioinfoSS.ulFlags = pmmioinfo->ulFlags;

         ulTempFlags = (MMIO_CREATE|MMIO_READ|MMIO_WRITE|MMIO_READWRITE|
                        MMIO_COMPAT|MMIO_EXCLUSIVE|MMIO_DENYWRITE|
                        MMIO_DENYREAD|MMIO_DENYNONE|MMIO_DELETE|MMIO_VERTBAR|
                        MMIO_APPEND|MMIO_USE_TEMP|MMIO_RWMODE|MMIO_SHAREMODE|
                        MMIO_NOTRANSLATE|MMIO_TRANSLATEDATA|
                        MMIO_TRANSLATEHEADER);

         mmioinfoSS.ulFlags &= ulTempFlags;
         mmioinfoSS.ulFlags |= MMIO_NOIDENTIFY;

         hmmioSS = mmioOpen( pszFileName, &mmioinfoSS, mmioinfoSS.ulFlags );

         if (hmmioSS)
            {
            /*
             * Handle a DELETE request for the file format by returning
             * success if ( hmmio = TRUE = 1).
             */

            if (pmmioinfo->ulFlags & MMIO_DELETE)
               {
               return (MMIO_SUCCESS);
               }
            pInfo->hmmioSS = hmmioSS;
            }
         else
            {
            return (mmioinfoSS.ulErrorRet);
            }

         /*
          * Get the header for the file if it already exists and
          * store it with the MMIOINFO for the file.
          */

         if (!(pmmioinfo->ulFlags & MMIO_CREATE))
            {
            /*
             * Seek the file to the beginning to read in the header.
             */

            lFilePosition = mmioSeek( hmmioSS,
                                      0L,
                                      SEEK_SET );

            if (lFilePosition < 0L)
               {
               mmioClose( hmmioSS, 0L );
               return (MMIO_ERROR);
               }

            /*
             * Read the header of the file into the provided buffer for the
             * given length.
             */

            lBytesRead = mmioRead( hmmioSS,
                                   (PSZ)pconvHeaderInfo,
                                   sizeof(CONVHEADERINFO) );

            if (lBytesRead < 0L)
               {
               mmioClose( hmmioSS, 0L );
               return (MMIO_ERROR);
               }
            }
         else
            {
            /*
             * Make a new header for the file and write it to the beginning.
             */

            pconvHeaderInfo->ulHeaderLength = sizeof(CONVHEADERINFO);
            strcpy( pconvHeaderInfo->szHeaderText, HEADER_STRING );

            lFilePosition = mmioSeek( hmmioSS,
                                      0L,
                                      SEEK_SET );

            if (lFilePosition < 0L)
               {
               mmioClose( hmmioSS, 0L );
               return (MMIO_ERROR);
               }

            /*
             * Write the newly created header to the file.
             */

            lBytesWritten = mmioWrite( hmmioSS,
                                       (PSZ)pconvHeaderInfo,
                                       sizeof(CONVHEADERINFO) );

            if (lBytesWritten < 0L)
               {
               mmioClose( hmmioSS, 0L );
               return (MMIO_ERROR);
               }
            }

         pconvHeaderInfo->ulFlags = 0;
         pmmioinfo->pExtraInfoStruct = (PVOID)pconvHeaderInfo;

         /*
          * Seek the file past the header to allow reads/writes to occur
          * at the first byte of non-header data if the file already exists.
          */

         lReturnCode = mmioSeek( hmmioSS,
                                 sizeof(CONVHEADERINFO),
                                 SEEK_SET );

         if (lReturnCode >= 0L)
            {
            pmmioinfo->lLogicalFilePos = lReturnCode;
            }
         else
            {
            mmioClose( hmmioSS, 0L );
            return (lReturnCode);
            }

         return (0L);

         break;

      case MMIOM_READ:
         /*
          * Call the read API with the Storage System handle using the
          * parameters that have been passed in to this I/O proc.
          */

         lBytesRead = mmioRead( pInfo->hmmioSS,
                                (CHAR *) lParam1,
                                lParam2 );

         /*
          *  Check the return code and determine if read was successful.
          *  Read must return:
          *    -1 - an error occurred with mmioRead or somewhere internally.
          *     x - number of bytes actually read by mmioRead.
          */

         if ( lBytesRead < 0L )
            {
            return( -1L );
            }
         else
            {
            return( lBytesRead );
            }

         break;

      case MMIOM_WRITE:
         /*
          * Call the write API with the Storage System handle using the
          * parameters that have been passed in to this I/O proc.
          */

         lBytesWritten = mmioWrite( pInfo->hmmioSS,
                                    (CHAR *) lParam1,
                                    lParam2 );

         /*
          *  Check the return code and determine if write was successful.
          *  Write must return:
          *    -1 - an error occurred with mmioWrite or somewhere internally.
          *     x - number of bytes actually written by mmioWrite.
          */

         if ( lBytesWritten < 0L )
            {
            return( -1L );
            }
         else if ( lBytesWritten != lParam2 )
            {
            pmmioinfo->ulErrorRet = MMIOERR_CANNOTWRITE;
            return( lBytesWritten );
            }
         else
            {

            /*
             * Set the Flags field in the CONV header to show it was modified.
             */

            pconvHeaderInfo = (PCONVHEADERINFO)pmmioinfo->pExtraInfoStruct;

            if (pconvHeaderInfo)
               pconvHeaderInfo->ulFlags = CONV_MODIFIED_FILE;

            return( lBytesWritten );
            }

         break;

      case MMIOM_SEEK:
         /*
          * Call the seek API with the Storage System handle using the
          * parameters that have been passed in to this I/O proc.
          */

         lReturnCode = mmioSeek( pInfo->hmmioSS,
                                 lParam1,
                                 lParam2 );

         /*
          *  Check the return code and determine if seek was successful.
          *  Seek must return:
          *    -1 - an error occurred with mmioSeek or somewhere internally.
          *     x - new current file postion from the beginning of the file.
          */

         if ( lReturnCode < 0L )
            {
            return( -1L );
            }

         return( lReturnCode );

         break;

      case MMIOM_CLOSE:
            /*
             * If the CONV header structure was maintained in pExtraInfoStruct,
             * write it back to the beginning of the file.
             */

            if (((pmmioinfo->ulFlags & MMIO_WRITE) ||
                 (pmmioinfo->ulFlags & MMIO_READWRITE)) &&
                 (pmmioinfo->pExtraInfoStruct))

               {
               pconvHeaderInfo = (PCONVHEADERINFO)pmmioinfo->pExtraInfoStruct;

               lReturnCode = mmioSeek( pInfo->hmmioSS,
                                       0L,
                                       SEEK_SET );

               if (lReturnCode < 0L)
                  {
                  return (lReturnCode);
                  }

               ulReturnCode = mmioSetHeader( pmmioinfo->hmmio,
                                             (PVOID)pmmioinfo->pExtraInfoStruct,
                                             sizeof(CONVHEADERINFO),
                                             &lBytesWritten,
                                             0L, 0L );
               if (ulReturnCode)
                  {
                  return (ulReturnCode);
                  }
               }

            if (pmmioinfo->pExtraInfoStruct)
               HhpFreeBuffer((PBYTE)pmmioinfo->pExtraInfoStruct);

            /*
             * Call the close API with the Storage System handle using any
             * parameters that have been passed in to this I/O proc.
             */

            usReturnCode = mmioClose( pInfo->hmmioSS, 0L );
            return ((ULONG)usReturnCode);

         break;

      case MMIOM_IDENTIFYFILE:
         /*
          * Get the filename from parameter. Then create a File Format header
          * in memory since this message use this structure.
          */

         pszFileName = (CHAR *)lParam1;  // get the filename from parameter.

         pconvHeaderInfo =
            (PCONVHEADERINFO)HhpAllocBuffer( sizeof(CONVHEADERINFO), 0);

         if (!pconvHeaderInfo)
            {
            return (MMIO_ERROR);
            }

         hmmioSS = (HMMIO)lParam2;       // get the SS handle to the file.

         if ( !hmmioSS )
            {
            HhpFreeBuffer((PBYTE)pconvHeaderInfo);
            return (MMIO_ERROR);
            }
         else
            {

            /*
             * Seek the file to the beginning to read in the file header.
             */

            lFilePosition = mmioSeek( hmmioSS,
                                      0L,
                                      SEEK_SET );

            if (lFilePosition < 0L)
               {
               HhpFreeBuffer((PBYTE)pconvHeaderInfo);
               return (MMIO_ERROR);
               }

            /*
             * Compare convHeaderInfo.szHeaderText with text string defined
             * in the convproc.h header file.
             */

            lBytesRead = mmioRead( hmmioSS,
                                   (PSZ)pconvHeaderInfo,
                                   sizeof(CONVHEADERINFO) );

            if ( lBytesRead <= 0L )
               {
               HhpFreeBuffer((PBYTE)pconvHeaderInfo);
               return( -1L );
               }

            pTemp = (CHAR *)pconvHeaderInfo;
            pTemp += 2 * sizeof(ULONG);

            if (!strncmp( pTemp, HEADER_STRING, strlen(HEADER_STRING) ))
               {
               HhpFreeBuffer((PBYTE)pconvHeaderInfo);
               return( 0L );
               }
            else
               {
               HhpFreeBuffer((PBYTE)pconvHeaderInfo);
               return( -1L );
               }
            }

         break;

      case MMIOM_GETFORMATINFO:

         /*
          * Fill in the mmformatinfo for the CONVProc.
          */

         pmmformatinfo = (PMMFORMATINFO)lParam1;

         if (pmmformatinfo == NULL)
            {
            return( -1L );
            }

         pmmformatinfo->ulStructLen = sizeof(MMFORMATINFO);
         pmmformatinfo->fccIOProc = FOURCC_FFT;
         pmmformatinfo->ulIOProcType = MMIO_IOPROC_FILEFORMAT;
         pmmformatinfo->ulMediaType = MMIO_MEDIATYPE_OTHER;
         pmmformatinfo->ulFlags = MMIO_CANREADUNTRANSLATED      |
                                  MMIO_CANWRITEUNTRANSLATED     |
                                  MMIO_CANREADWRITEUNTRANSLATED |
                                  MMIO_CANSEEKUNTRANSLATED;

         memset( pmmformatinfo->szDefaultFormatExt,  '\0',
            sizeof(pmmformatinfo->szDefaultFormatExt)  );
         strcpy( pmmformatinfo->szDefaultFormatExt,  "FFT" );

         if (convhlpGetNLSData( &pmmformatinfo->ulCodePage,
                                &pmmformatinfo->ulLanguage ))
            {
            return( -1L );
            }

         if (convhlpGetFormatStringLength( FOURCC_FFT,
                                           &(pmmformatinfo->lNameLength) ))
            {
            return( -1L );
            }

         return( 0L );
         break;

      case MMIOM_GETFORMATNAME:

         /*
          * The string is in a resource file (CONVPROC.RC) for NLS purposes.
          */

         pszFormatString = (CHAR *)lParam1;

         lBytesCopied = convhlpGetFormatString( FOURCC_FFT,
                                                pszFormatString,
                                                lParam2 );

         return( lBytesCopied );
         break;

      case MMIOM_QUERYHEADERLENGTH:

         /*
          * Save current file position for later restore. Then
          * seek the file to the beginning to read in the header.
          */

         lSavedFilePosition = pmmioinfo->lLogicalFilePos;

         lFilePosition = mmioSeek( pInfo->hmmioSS,
                                   0L,
                                   SEEK_SET );

         if (lFilePosition < 0L)
            {
            return (0L);
            }

         /*
          * Read in the header length for the file. It is the first 4 bytes.
          */

         lBytesRead = mmioRead( pInfo->hmmioSS,
                                szHeaderLength,
                                sizeof(ULONG) );

         lReturnCode = mmioSeek( pInfo->hmmioSS,
                                 lSavedFilePosition,
                                 SEEK_SET );

         if (lReturnCode != lSavedFilePosition)
            {
            return (0L);
            }

         if (lBytesRead <= 0L)
            {
            return (0L);
            }
         else
            {
            lHeaderLength = (LONG)(*((LONG *)szHeaderLength));
            return (lHeaderLength);
            }

         break;

      case MMIOM_GETHEADER:

         /*
          * Save current file position for later restore. Then
          * seek the file to the beginning to read in the header.
          */

         lSavedFilePosition = pmmioinfo->lLogicalFilePos;

         lFilePosition = mmioSeek( pInfo->hmmioSS,
                                   0L,
                                   SEEK_SET );

         if (lFilePosition < 0L)
            {
            return (0L);
            }

         /*
          * Read the header of the file into the provided buffer for the
          * given length. Then seek the file back to the saved position.
          */

         lBytesRead = mmioRead( pInfo->hmmioSS,
                                (CHAR *)lParam1,
                                lParam2 );

         lReturnCode = mmioSeek( pInfo->hmmioSS,
                                 lSavedFilePosition,
                                 SEEK_SET );

         if (lReturnCode != lSavedFilePosition)
            {
            return (0L);
            }

         if (lBytesRead <= 0L)
            {
            return (0L);
            }
         else
            {
            return (lBytesRead);
            }

         break;

      case MMIOM_SETHEADER:

         /*
          * Save current file position for later restore. Then
          * seek the file to the beginning to read in the header.
          */

         lSavedFilePosition = pmmioinfo->lLogicalFilePos;

         lFilePosition = mmioSeek( pInfo->hmmioSS,
                                   0L,
                                   SEEK_SET );

         if (lFilePosition < 0L)
            {
            return (0L);
            }

         /*
          * Write the entire header to the file. Then seek the file back
          * to the saved file position.
          */

         lBytesWritten = mmioWrite( pInfo->hmmioSS,
                                    (CHAR *)lParam1,
                                    lParam2 );

         lReturnCode = mmioSeek( pInfo->hmmioSS,
                                 lSavedFilePosition,
                                 SEEK_SET );

         if (lReturnCode != lSavedFilePosition)
            {
            return (0L);
            }

         if (lBytesWritten <= 0L)
            {
            return (0L);
            }
         else
            {
            return (lBytesWritten);
            }

         break;

      case CONVM_TOUPPER:

         pszData = (CHAR *)lParam1;
         convhlpToUpper( (PUCHAR)pszData );
         return (MMIO_SUCCESS);

         break;

      case CONVM_TOLOWER:

         pszData = (CHAR *)lParam1;
         convhlpToLower( (PUCHAR)pszData );
         return (MMIO_SUCCESS);

         break;

      default:

         /*
          * If an IO Proc has a child IO Proc, then instead of
          * returning UNSUPPORTED_MESSAGE, send the message to
          * the child IO Proc to see if it can understand and
          * process the message.
          *
          * Since message is unexpected, need to check for valid
          * pointers.
          */

         if (pInfo)
            {
            if (pInfo->hmmioSS)
               {
               lReturnCode = ( mmioSendMessage( pInfo->hmmioSS,
                                                usMsg,
                                                lParam1,
                                                lParam2 ));
           
               if (!lReturnCode)
                   pmmioinfo->ulErrorRet = mmioGetLastError(pInfo->hmmioSS);
               return (lReturnCode);
               }
            }
          else
            {
            if (pmmioinfo)
                pmmioinfo->ulErrorRet = MMIOERR_UNSUPPORTED_MESSAGE;
            }

          return (MMIOERR_UNSUPPORTED_MESSAGE);

      }

}

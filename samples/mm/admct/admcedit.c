/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCEDIT.C
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* DESCRIPTIVE: Digital Audio clipboard manipulation and error
*              detection source file
*
* FUNCTION:  Holds subroutines and flag checking routines for the
*            mci editing operations (e.g. cut/copy/paste/delete).
*
*
* NOTES:  Concepts illustrated in this source file:
*          A. How to process MCI_FROM / MCI_TO for edit operations
*             ( CheckEditFlags).
*          B. Determine default from/to positions.
*          C. Generic stream abort routine
*          D. Retrieving information from the clipboard.
*          E. Placing information into the clipboard.
*          F. Using mmio memory files to manipulate clipboard data.
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES:
*
********************** END OF SPECIFICATIONS **********************/

#define INCL_WINCLIPBOARD
#define INCL_WINWINDOWMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#define INCL_WINATOM
#define INCL_HEAP
#define INCL_DOSMEMMGR

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <stdio.h>
#include <bsememf.h>
#include <os2medef.h>                   // MME includes files.
#include <mmioos2.h>                    // MMIO Include.
#include <ssm.h>                        // SSM spi includes.
#include <mcios2.h>
#include <meerror.h>                    // MM Error Messages.
#include <mmdrvos2.h>                   // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes
#include "admcres.h"                    // Definition for bitmap resource
#ifndef TOOLKIT
#include <convert.h>                    // Data conversion routines
#endif
#include <checkmem.h>

#include <mmdsload.h>

/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: CheckEditFlags
*
* DESCRIPTIVE  Ensures that editing functions have valid flags.
*
* FUNCTION:  Checks to see if memory is correctly allocated, from/to
*            flags if the exist are valid, ensures that a file element
*            is loaded (if no buffer is passed) and aborts any in
*            progress commands.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_STOP message.
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/



RC CheckEditFlags( FUNCTION_PARM_BLOCK *pFuncBlock,
                   ULONG               ulMCIMajic )

{

  ULONG         ulParam1;           // Flags for this Msg
  ULONG         ulParam2;           // Data for this Msg
  ULONG         ulrc;               // Error Code Propogated
  ULONG         ulFromRequest;      // Position to operate from
  ULONG         ulToRequest = 0;    // Position to operate to
  ULONG         ulFileLength;       // File length--used in range checking

  ULONG         ulEditFlags;
  ULONG         ulAbortNotify=FALSE;// Flag to abort preceding commands
  ULONG         ulOldTimeFormat;    // Old Time Format

//  LONG          lTempVal;
//  LONG          lFromPosition;

  MCI_EDIT_PARMS *pEditParms;

  INSTANCE      *ulpInstance;         // Local Instance

  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulParam1 = pFuncBlock->ulParam1;
  ulParam2 = pFuncBlock->ulParam2;
  ulrc = MCIERR_SUCCESS;
  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;


  /*******************************************
  * Ensure that the caller passed valid
  * edit parms to us.
  ********************************************/

  if ( ulMCIMajic )
     {
     ulrc = CheckMem ( (PVOID) ulParam2,
                       sizeof (MCI_GENERIC_PARMS),
                       PAG_READ | PAG_WRITE );
     }
  else
     {
     ulrc = CheckMem ( (PVOID) ulParam2,
                       sizeof (MCI_EDIT_PARMS),
                       PAG_READ | PAG_WRITE );
     }

  if (ulrc != MCIERR_SUCCESS)
     {
     return (MCIERR_MISSING_PARAMETER);
     }

  pEditParms = (MCI_EDIT_PARMS *) ulParam2;

  /*********************************
  * Mask the flags the we support
  **********************************/

  ulEditFlags = pFuncBlock->ulParam1;

  if ( !ulMCIMajic )
     {
     ulEditFlags &= ~( MCI_FROM + MCI_TO + MCI_NOTIFY + MCI_WAIT +
                       MCI_CONVERT_FORMAT + MCI_FROM_BUFFER + MCI_TO_BUFFER );
     }
  else
     {
     ulEditFlags &= ~( MCI_WAIT + MCI_NOTIFY );
     }

  /* If the caller passed in an  unsupported flag--return an error */

  if ( ulEditFlags )
      return ( MCIERR_INVALID_FLAG );


  if ( ( ulParam1 & MCI_FROM_BUFFER ) && ( ulParam1 & MCI_TO_BUFFER ) )
     return ( MCIERR_INVALID_FLAG );



  /*********************************************
  * If the user passed in a buffer, ensure that
  * that it contains a valid pointer.
  *******************************************/



  if ( ( ulParam1 & MCI_FROM_BUFFER ) || ( ulParam1 & MCI_TO_BUFFER ) )
     {


     /********************************************************
     * When the buffer flags are passed in the caller is also
     * responsible for passing in a valid mmaudioheader in
     * the case of waveaudio.
     *********************************************************/

     ulrc = CheckMem ( (PVOID) pEditParms->pHeader,
                       sizeof ( MMAUDIOHEADER ),
                       PAG_READ | PAG_WRITE );

     if (ulrc != MCIERR_SUCCESS)
        {
        return ( MCIERR_INVALID_BUFFER );
        }


     ulrc = CheckMem ( (PVOID) pEditParms->pBuff,
                       sizeof ( pEditParms->ulBufLen ),
                       PAG_READ | PAG_WRITE );

     if (ulrc != MCIERR_SUCCESS)
        {
        return ( MCIERR_INVALID_BUFFER );
        }




     }

  /*******************************************
  * If a playlist is active, the only way to
  * use the MCI_EDITING functions is to
  * use a buffer since mmio commands do not
  * work on playlists.
  *******************************************/

  if ( ulpInstance->usPlayLstStrm &&
       ( !(ulParam1 & MCI_FROM_BUFFER ) || !(ulParam1 & MCI_TO_BUFFER ) ) )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

  /*******************************************
  * Ensure that there is a file to operate on
  * if this is not a playlist and the buffer
  * flag has not been sent.
  *******************************************/

  if ( ulpInstance->usPlayLstStrm == FALSE &&
       ulpInstance->fFileExists != TRUE)
    {
      return ( MCIERR_FILE_NOT_FOUND );
    }

  /*******************************************
  * Ensure that there is a file to operate on
  * if this is not a playlist and the buffer
  * flag has not been sent.
  *******************************************/

  if ( WAVEHDR.usFormatTag != DATATYPE_WAVEFORM )
    {
    return ( MCIERR_UNSUPPORTED_FUNCTION );
    }

  /*****************************************
  * To ensure proper syncronization, acquire
  * the semaphore which is used to control
  * who can check to see which processes are
  * active. This function will also tell us
  * if there is an operation to abort or
  * supercede.
  ******************************************/

  GetNotifyAbortAccess ( ulpInstance, &ulAbortNotify );

  /*******************************************
  * If there is an operation active (i.e. a
  * play, record or save) then post a message
  * stating that the command has been
  * aborted (record or play) or
  * wait for completion (save).
  ********************************************/

  if ( ulAbortNotify )
     {
     /* Stop the command on another thread */

     GenericThreadAbort( ulpInstance, pFuncBlock, 0 );

     }  /* There was a pending Notify */



  if ( STRMSTATE != MCI_STOP &&
       ulpInstance->ulCreateFlag == PREROLL_STATE )
     {
     ulrc = StopStream2( ulpInstance, MCIDRV_DISCARD_STREAM_NETWORK );

     if ( ulrc )
        {
        return( ulrc );
        }
     } /* If the stream is not in a stopped state */



  /*********************************************
  * In order to determine if the edit request
  * is valid, we must retrieve the length of
  * the file.
  *
  * Note: the length of the file is returned in
  * the time units we are using currently.
  *********************************************/

  ConvertTimeUnits ( ulpInstance,
                     &ulFileLength,
                     FILE_LENGTH);

  if ( ulParam1 & MCI_FROM )
     {
     /* The from request MUST be within the limits of the current file */

     if ( pEditParms->ulFrom  > ulFileLength )
        {
        return ( MCIERR_OUTOFRANGE );
        } /* request greater than file length */

     /* Convert the MCI_FROM position to bytes. */

     if ( ulpInstance->ulTimeUnits != lBYTES )
        {
        // investigate combining this into one routine

        ulOldTimeFormat = ulpInstance->ulTimeUnits;

        ulrc = ConvertToMM ( ulpInstance,
                             &ulFromRequest,
                             pEditParms->ulFrom );

        ulpInstance->ulTimeUnits = lBYTES;

        /* Convert number to bytes */
        ConvertTimeUnits ( ulpInstance,
                           &ulFromRequest,
                           ulFromRequest );

        ulpInstance->ulTimeUnits = ulOldTimeFormat;
        }
     else
        {
        ulFromRequest = pEditParms->ulFrom;
        }

//     lFromPosition = ulFromRequest;

     } /* MCI_FROM flag passed */


  /*******************************************************
  * In order to be valid, an MCI_TO request must
  *   A. Be less than the file length.
  *   B. Be less than the MCI_FROM position if specified.
  *   C. If no from flag is specified, be less than the
  *      current file position.
  ********************************************************/

  if ( ulParam1 & MCI_TO )
     {
     /* The TO request MUST be within the limits of the current file */


     if ( ulParam1 & MCI_FROM )
        {
        if ( pEditParms->ulTo <= pEditParms->ulFrom )
           {
           return ( MCIERR_OUTOFRANGE );
           }
        } /* MCI_FROM flag passed */

//     /********************************************
//     * If no from flag was passed, edit operations
//     * assume the current file position as the
//     * start--if so, then ensure that the to point
//     * is not behind our current position.
//     *********************************************/
//
//     if ( ulpInstance->ulCreateFlag == PREROLL_STATE )
//        {
//        /**********************************************
//        * Get the current stream time, it will be used
//        * for range checking below.
//        ***********************************************/
//
//        ulrc = SpiGetTime( STREAM.hStream,
//                           ( PMMTIME ) &( STREAM.mmStreamTime ) );
//
//        if ( ulrc )
//           {
//           return ( ulrc );
//           }
//
//        // investigate --combine into function.
//
//        ulOldTimeFormat = ulpInstance->ulTimeUnits;
//
//        ulpInstance->ulTimeUnits = lBYTES;
//
//        ConvertTimeUnits ( ulpInstance,
//                           &ulToPosition,
//                           STREAM.mmStreamTime );
//
//        ulpInstance->ulTimeUnits = ulOldTimeFormat;
//
//        }  /* if a stream has been created */
//     else
//        {
//        lTempVal = mmioSeek( ulpInstance->hmmio,
//                                 0,
//                                 SEEK_CUR );
//
//        if ( lTempVal == MMIO_ERROR )
//           {
//           return ( MMIOERR_SEEK_FAILED );
//           }
//
//        ulToPosition = ( ULONG ) lTempVal;
//        } /* else there is no stream active */

     /************************************************
     * Convert the MCI_TO request to mmtime,
     * then to bytes so we can compare it with
     * the current file position (which is in bytes).
     ************************************************/

     if ( ulpInstance->ulTimeUnits != lBYTES )
        {
        // investigate --combine into function.
        ulOldTimeFormat = ulpInstance->ulTimeUnits;

        ulrc = ConvertToMM ( ulpInstance,
                             &ulToRequest,
                             pEditParms->ulTo );

        ulpInstance->ulTimeUnits = lBYTES;

        ConvertTimeUnits ( ulpInstance,
                           &ulToRequest,
                           ulToRequest );

        ulpInstance->ulTimeUnits = ulOldTimeFormat;
        }
     else
        {
        ulToRequest = pEditParms->ulTo;
        }

// what does this do?

//     if ( ulToPosition < ulToRequest )
//        {
//       return ( MCIERR_OUTOFRANGE );
//        }


     if ( pEditParms->ulTo  > ulFileLength )
//          pEditParms->ulTo  > ulToRequest )
        {
        return ( MCIERR_OUTOFRANGE );
        } /* request greater than file length or current position */


     } /* The MCI_TO flag was passed in */



  /* If undo or redo call, we are done */

  if ( ulMCIMajic )
     {
     return ( MCIERR_SUCCESS );
     }

  /*********************************************
  * All of the edit routines operate on a byte
  * wise basis, therefore, convert the from/to
  * numbers to bytes here so that code will not
  * be replicated.
  **********************************************/

  if ( ulParam1 & MCI_FROM )
     {

     /*********************************************
     * If we have a 16 bit file and the request
     * starts in the midst of a sample, move to
     * the beginning of the sample.
     **********************************************/

     if ( WAVEHDR.usBitsPerSample == 16 &&
          ulFromRequest % 2 )
        {
        ulFromRequest--;
        }

     pEditParms->ulFrom = ulFromRequest;

     }

  if ( ulParam1 & MCI_TO )
     {

     /*********************************************
     * If we have a 16 bit file and the to request
     * ends in the midst of a sample, move to
     * the beginning of the sample.
     **********************************************/

     if ( WAVEHDR.usBitsPerSample == 16 &&
          ulToRequest % 2 )
        {
        ulToRequest--;
        }

     /* We had previously converted this number to bytes */

     pEditParms->ulTo = ulToRequest;
     }


  return ( MCIERR_SUCCESS );

} /* CheckEditFlags */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: GetFromToPosition
*
* DESCRIPTIVE  Returns the actual from/to position for edit operations.
*
* FUNCTION:  Will place the file the correct spot for from/to edit
*            operations.  If either of the flags is not set, then it
*            retrieves the defaults (current position for MCI_FROM, and
*            end of file for MCI_TO ).
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_STOP message.
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/

RC  GetFromToPosition(  LONG            lCurrFilePos,
                        ULONG           ulParam1,
                        MCI_EDIT_PARMS  *pEditParms,
                        INSTANCE        *ulpInstance,
                        PULONG          pulFromPosition,
                        PULONG          pulToPosition )


{
  LONG    lTempPosition;  // temporary file position used by seek


  if ( ulParam1 & MCI_FROM )
     {
     /********************************************************
     * If we are not currently at the place the caller wants
     * to copy from, seek there
     ********************************************************/

     if ( pEditParms->ulFrom != ( ULONG ) lCurrFilePos )
        {
        lTempPosition = mmioSeek( ulpInstance->hmmio,
                                  pEditParms->ulFrom,
                                  SEEK_SET );

        if ( lTempPosition == MMIO_ERROR )
           {
           return ( MMIOERR_SEEK_FAILED );
           }

        } /* if we are not at the from position */

     *pulFromPosition = pEditParms->ulFrom;

     } /* if the from flag was specified */
  else
     {
     *pulFromPosition = lCurrFilePos;

     } /* else the from flag was not specified */

  if ( ulParam1 & MCI_TO )
     {
     *pulToPosition = pEditParms->ulTo;

     } /* MCI_TO flag specified */

  else
     {
     /* if no MCI_TO, we will operate to the end of file */

     *pulToPosition = XWAVHDR.ulAudioLengthInBytes;
     } /* no MCI_TO flag */

  return ( MCIERR_SUCCESS );

} /* GetFromToPosition */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: GetClipboardData
*
* DESCRIPTIVE  Gets digital audio information from the clipboard.
*
* FUNCTION:  Checks to see if the correct audio format is in the
*            clipboard, if it is, a buffer is allocated and filled
*            with wave information.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_STOP message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES: spiStopStream ()   -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/

RC GetClipboardData( PVOID           *pBuffer,
                     INSTANCE        *pInstance,
                     ULONG           ulOperation,
                     PULONG          pulBuffLen,
                     ULONG           ulParam1,
                     PMMAUDIOHEADER  pmmaudioheader )

{
HAB            habClipboard;
HMQ            hmq;

BOOL           fCreatedMQ = TRUE;  /* message queue creation? */

HMMIO          hmmioMem;           /* handle to memory file   */

MMIOINFO       mmioinfo;           /* info struct for memory file */

ULONG          ulFormatInfo = 0;
ULONG          rc;
PULONG         pulDataSize;

LONG           lBytesRead;
LONG           lReturnCode;

MMAUDIOHEADER  mmaudioheader;
MMAUDIOHEADER  mmaudioheaderTemp;


PSZ            pTempBuffer;
PVOID          pNewBuffer;


  habClipboard = WinInitialize( 0 );
  if ( !habClipboard )
     {
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  hmq = WinCreateMsgQueue( habClipboard, 0 );

  if ( !hmq )
     {
     fCreatedMQ = FALSE;
     }

//  habClipboard = WinQueryAnchorBlock( HWND_DESKTOP );

  /*********************************************************************
  * Check to see if there is a wave ( CF_WAVE is the defined type) in
  * the clipboard.
  *********************************************************************/


  rc = WinQueryClipbrdFmtInfo( habClipboard,
                          CF_WAVE,
                          &ulFormatInfo );

//  if ( !rc )
//     {
//
//     /*********************************************************************
//     * If CF_WAVE is not in the clipboard, then it is possible that a
//     * Win/OS2 OLE link is in the clipboard.  Check and see.
//     *********************************************************************/
//
//    = WinFindAtom( WinQuerySystemAtomTable(),
//                             "OwnerLink" );
//     if ( !atomWave )
//        {
//        WinCloseClipbrd( habClipboard );
//        return ( MCIERR_CLIPBOARD_EMPTY );
//        }
//     else
//        {
//        ulOleLink = TRUE;
//        }
//
//     }

  if ( !WinOpenClipbrd( habClipboard ) )
     {
     WinCloseClipbrd( habClipboard );
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  pTempBuffer = ( PSZ ) WinQueryClipbrdData( habClipboard, CF_WAVE );

  if ( !pTempBuffer )
     {
     WinCloseClipbrd( habClipboard );
     return ( MCIERR_CLIPBOARD_ERROR );
     }


  /*********************************************************************
  * We need to find out how much data is in the file, so retrieve
  * the length of the riff chunk
  *********************************************************************/
  pulDataSize = ( PULONG ) pTempBuffer + 1;


  memset( &mmioinfo, '\0', sizeof( MMIOINFO ) );

  /*********************************************************************
  * Prepare to open a memory file--the buffer * in the clipboard contains
  * actual Riff-file  which the wave io proc already knows how
  * to parse--use it to retrieve the information  and keep the MCD from
  * file format dependence.
  *********************************************************************/


  mmioinfo.fccIOProc = mmioFOURCC( 'W', 'A', 'V', 'E' ) ;
  mmioinfo.fccChildIOProc = FOURCC_MEM;

  rc = CheckMem ( (PVOID) pulDataSize,
                    sizeof ( ULONG ),
                    PAG_READ | PAG_WRITE );

  if (rc != MCIERR_SUCCESS)
     {
     WinCloseClipbrd( habClipboard );
     return (MCIERR_OUT_OF_MEMORY );
     }


  mmioinfo.cchBuffer = ( *pulDataSize) + 8;
  mmioinfo.pchBuffer = pTempBuffer;


  hmmioMem = mmioOpen( NULL,
                       &mmioinfo,
                       MMIO_READ );


  if ( !hmmioMem )
     {
     WinCloseClipbrd( habClipboard );
     return ( mmioinfo.ulErrorRet );
     }

  /******************************************
  * Figure out how big the file is by
  * retrieving the audio header--remember that
  * although this is physical memory, it is
  * treated as a file by mmio.
  ******************************************/

  lReturnCode  = mmioGetHeader ( hmmioMem,
                                 (PVOID) &mmaudioheader,
                                 sizeof( mmaudioheader ),
                                 &lBytesRead,
                                 (ULONG) NULL,
                                 (ULONG) NULL);

  if ( lReturnCode == MMIO_SUCCESS )
     {
      /***********************************************
      * The current routines can only process PCM.
      * Sometime in the future, we will handle other
      * formats like ADPCM.  Note: what about alaw + mulaw?????????
      * should be easy to convert.
      **********************************************/

     if ( mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag !=
          DATATYPE_WAVEFORM )
        {
        WinCloseClipbrd( habClipboard );
        return ( MCIERR_CANNOT_CONVERT ); // is there invalid format ?
        }
      /***********************************************
      * Have the io proc parse file format and bring in
      * the entire amount in the clipboard.
      **********************************************/

     *pulBuffLen = mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes;

     *pBuffer = HhpAllocBuffer( mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes + 500,
                                0 );

     lReturnCode = mmioRead( hmmioMem,
                             ( PSZ ) *pBuffer,
                             mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes );

     if ( lReturnCode != mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes )
        {
        // return better error
        WinCloseClipbrd( habClipboard );
        lReturnCode = mmioGetLastError( hmmioMem );
        mmioClose( hmmioMem, ( WORD ) NULL );

        return( lReturnCode );
        }

     } /* if we successfully retrieved the header */
  else
     {
     // return better error
     WinCloseClipbrd( habClipboard );
     lReturnCode = mmioGetLastError( hmmioMem );

     mmioClose( hmmioMem, ( WORD ) NULL );
     return ( lReturnCode ) ;

     } /* else we cannot retrieve the header */


  WinCloseClipbrd( habClipboard );

 if ( ulParam1 & MCI_CONVERT_FORMAT )
     {
     memmove( &mmaudioheaderTemp, &pInstance->mmAudioHeader, sizeof( MMAUDIOHEADER ) );

#ifndef TOOLKIT
     rc = ConvertAudio( &mmaudioheader,
                        *pBuffer,
                        &mmaudioheaderTemp,
                        &pNewBuffer);

     if ( !rc )
        {

        return ( MCIERR_CANNOT_CONVERT );
        }
#endif

     rc = MCIERR_SUCCESS;

     if ( pNewBuffer )
        {
        HhpFreeBuffer( *pBuffer );
        *pBuffer = pNewBuffer;

        memmove( &mmaudioheader, &mmaudioheaderTemp, sizeof( MMAUDIOHEADER ) );
        *pulBuffLen = mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes;
        if ( pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample == 16 &&
             *pulBuffLen % 2 )
        *pulBuffLen = *pulBuffLen - 1;
        }

     }
  else
     {
     /*----------------------------------------------------------------------
     * If the to buffer flag is passed, the caller "should" be intelligent
     * enough to handle conversions.
     *---------------------------------------------------------------------*/

     if ( !(ulParam1 & MCI_TO_BUFFER ) )
        {
        /*----------------------------------------------------
        * If the clipboard contains a different format than
        * what is in the current file, return an error.
        *--------------------------------------------------*/

        if ( ( pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels != (SHORT )
               mmaudioheader.mmXWAVHeader.WAVEHeader.usChannels )                  ||
             ( pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec != ( LONG )
               mmaudioheader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec )             ||
             ( pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample != ( LONG )
               mmaudioheader.mmXWAVHeader.WAVEHeader.usBitsPerSample ) )

          {
          HhpFreeBuffer( *pBuffer );
          return (MCIERR_CANNOT_CONVERT );
          } /* if the clipboard + file contain different formats */

        } /* if the user did not request pasting to the buffer */

     } /* else the user does not want a conversion from the clipboard done */
  // is this needed?

  if ( ulParam1 & MCI_TO_BUFFER )
     {
     memmove( pmmaudioheader, &mmaudioheader, sizeof( MMAUDIOHEADER ) );
     }

  mmioClose( hmmioMem, ( WORD ) NULL );


  /*-----------------------------------------------------------
  * Message queue creation could have failed if the process has
  * already done it.
  *------------------------------------------------------------*/

  if ( fCreatedMQ )
     {
     WinDestroyMsgQueue( hmq );
     }

  WinTerminate( habClipboard );



  return ( rc );
} /* GetClipboardData */






/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: PutClipboardData
*
* DESCRIPTIVE  Puts digital audio information into the clipboard.
*
* FUNCTION:  Allocates memory for digital audio information, opens
*            a memory file with correct header then places a
*            bitmap and wave information into the clipboard, then
*            closes the clipboard.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_STOP message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES: spiStopStream ()   -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/

RC PutClipboardData( PVOID            *pBuffer,
                     INSTANCE         *pInstance,
                     ULONG            ulOperation,
                     ULONG            ulBuffLength )

{
HAB            habClipboard;
HMQ            hmq;

BOOL           fCreatedMQ = TRUE;  /* message queue creation? */

HBITMAP        hbmWave;
HPS            hps;

HMMIO          hmmioMem;           // handle to memory file

MMIOINFO       mmioinfo;           // info struct for memory file

ULONG          rc;
extern ULONG   hModuleHandle;        // module handle for dll
ULONG          hModuleMMPM;        // module handle for dll

//extern HWND    hwndClipWin;               /* Clipboard win handle */


LONG           lBytesRead;
LONG           lReturnCode;

MMAUDIOHEADER  mmaudioheader;

PCHAR          pWave;


  memset( &mmioinfo, '\0', sizeof( MMIOINFO ) );

  /*********************************************************************
  * Prepare to open a memory file--the buffer * in the clipboard contains
  * actual Riff-file  which the wave io proc already knows how
  * to parse--use it to retrieve the information  and keep the MCD from
  * file format dependence.
  *********************************************************************/


  mmioinfo.fccIOProc = mmioFOURCC( 'W', 'A', 'V', 'E' ) ;
  mmioinfo.fccChildIOProc = FOURCC_MEM;
  mmioinfo.cchBuffer = ( ulBuffLength) + 200;


  pWave = ( PCHAR ) HhpAllocBuffer( ( ( ulBuffLength ) + 200 ), HH_SHARED );

  mmioinfo.pchBuffer = pWave;

  hmmioMem = mmioOpen( NULL,
                       &mmioinfo,
                       MMIO_READWRITE | MMIO_CREATE );

  if ( !hmmioMem )
     {
     return ( mmioinfo.ulErrorRet );
     }

  /***********************************************
  * Copy the instance header into a structure we
  * can safely modify for this particular buffer
  **********************************************/

// hack for codecs in io procs
// should put in local instance



 memmove( &mmaudioheader, &pInstance->mmAudioHeader, sizeof ( MMAUDIOHEADER ));

 mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag       = pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usFormatTag;
 mmaudioheader.mmXWAVHeader.WAVEHeader.usChannels        = pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels;
 mmaudioheader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec   = pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
 mmaudioheader.mmXWAVHeader.WAVEHeader.usBitsPerSample   = (USHORT) pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;


 mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes = ulBuffLength;


  lReturnCode = mmioWrite( hmmioMem,
                           *pBuffer,
                           ulBuffLength );

  if ( lReturnCode != ulBuffLength )
     {
     lReturnCode = mmioGetLastError ( hmmioMem );
     mmioClose( hmmioMem, ( WORD ) NULL );
     return( lReturnCode );
     }

 lReturnCode  = mmioSetHeader ( hmmioMem,
                                (PVOID) &mmaudioheader,
                                sizeof( mmaudioheader ),
                                &lBytesRead,
                                (ULONG) NULL,
                                (ULONG) NULL);


  if ( lReturnCode != MMIO_SUCCESS )
     {
     lReturnCode = mmioGetLastError ( hmmioMem );
     mmioClose( hmmioMem, ( WORD ) NULL );
     return( lReturnCode );
     }

  habClipboard = WinInitialize( 0 );
  if ( !habClipboard )
     {
//     WinReleasePS( hps );
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  hmq = WinCreateMsgQueue( habClipboard, 0 );

  if ( !hmq )
     {
     fCreatedMQ = FALSE;
     }

  /********************************************************
  * The resource that is associated with this dll contains
  * a wave bitmap that we will place into the clipboard
  * along with the actual audio information, so obtain
  * a handle to the bitmap from PM.
  ********************************************************/

//  hps = WinGetPS( hwndClipWin );
  hps = WinGetPS( HWND_DESKTOP ) ;


  rc = MM_DosLoadModule( NULL, 0L, "MMPM", &hModuleMMPM );     //@LP01a
  if ( rc )
     {
     hModuleMMPM = hModuleHandle;
     }

  hbmWave = GpiLoadBitmap( hps, hModuleMMPM, WAVE_BMP, 0, 0 );

  if ( !hbmWave )
     {
     WinReleasePS( hps );
     mmioClose( hmmioMem, ( WORD ) NULL );
     return ( MCIERR_CLIPBOARD_ERROR );
     }



//  habClipboard = WinQueryAnchorBlock( HWND_DESKTOP );

  /***********************************************
  * Open the clipboard and prepare to copy  the
  * information into it.
  **********************************************/


  if ( !WinOpenClipbrd( habClipboard ) )
     {
     WinReleasePS( hps );
     mmioClose( hmmioMem, ( WORD ) NULL );
     return ( MCIERR_CLIPBOARD_ERROR );  // cant open clipboard
     }

  /***********************************************
  * Clear what was previously in the clipboard--
  * standard PM protocol.
  **********************************************/



  if ( !WinEmptyClipbrd( habClipboard ) )
     {
     WinCloseClipbrd( habClipboard );
     mmioClose( hmmioMem, ( WORD ) NULL );
     WinReleasePS( hps );
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  /**************************************************
  * Place the wave and the bitmap in the  clipboard
  * if possible.
  **************************************************/

  if ( !WinSetClipbrdData( habClipboard,
                           ( ULONG ) hbmWave,
                           CF_BITMAP,
                           CFI_HANDLE ) )
     {
     WinCloseClipbrd( habClipboard );
     mmioClose( hmmioMem, ( WORD ) NULL );
     WinReleasePS( hps );
     return ( MCIERR_CLIPBOARD_ERROR );  // cant open clipboard

     }

  if ( !WinSetClipbrdData( habClipboard,
                           ( ULONG ) pWave,
                           CF_WAVE,
                           CFI_POINTER  ) )
     {
     WinCloseClipbrd( habClipboard );
     WinReleasePS( hps );
     mmioClose( hmmioMem, ( WORD ) NULL );
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  /***************************
  * Perform cleanup if any.
  ***************************/

  mmioClose( hmmioMem, ( WORD ) NULL );

  /**************************************************
  * Close the clipboard as quickly as possible since
  * only one process at a time can be in it.
  **************************************************/

  if ( !WinCloseClipbrd( habClipboard ) )
     {
     WinReleasePS( hps );
     return ( MCIERR_CLIPBOARD_ERROR );
     }

  /*-----------------------------------------------------------
  * Message queue creation could have failed if the process has
  * already done it.
  *------------------------------------------------------------*/

  if ( fCreatedMQ )
     {
     WinDestroyMsgQueue( hmq );
     }

  WinReleasePS( hps );
  WinTerminate( habClipboard );

  return ( MCIERR_SUCCESS );
} /* PutClipboardData */



#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#define INCL_GPI

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <stdlib.h>                     // begin thread prototype
#include <stdio.h>
#include <os2medef.h>                   // MME includes files.
#include <mmioos2.h>                    // MMIO Include.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCPASTE.C
*
* DESCRIPTIVE: Digital Audio paste from the clipboard
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:  Check flags and retrieve from the clipboard the
*            digital audio information.  Paste it either into a buffer
*            or into a file.
*
*
* NOTES: Concepts illustrated in this source file.
*
*  1. Processing the MCI_FROM_BUFFER flag or MCI_TO_BUFFER.
*  2. Handling default positioning if no MCI_FROM/MCI_TO flags are passed
*  3. Using mmio to insert information into a file.
*
* INPUT: MCI_PASTE message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiStopStream()     - SSM Spi
*                      spiSeekStream()     - SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/

RC MCIPaste (FUNCTION_PARM_BLOCK *pFuncBlock)

{

  ULONG         ulParam1;           // Flags for this Msg
  ULONG         ulrc;               // Error Code Propogated
  ULONG         ulCount;


  INSTANCE      *pInstance;         // Local Instance

  MCI_EDIT_PARMS *pEditParms;       // Pointer to structure to edit

  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulParam1 = pFuncBlock->ulParam1;
  pEditParms = ( MCI_EDIT_PARMS * ) pFuncBlock->ulParam2;
  ulrc = MCIERR_SUCCESS;
  pInstance = (INSTANCE *) pFuncBlock->ulpInstance;


  /*******************************************
  * Check for valid flags
  ********************************************/

  ulrc = CheckEditFlags( pFuncBlock, FALSE );

  if ( ulrc != MCIERR_SUCCESS )
     {
     return ( ulrc );
     }

  /*******************************************
  * From and To are pointless with the buffer
  * flag.
  ********************************************/


  if ( ( ulParam1 & MCI_TO_BUFFER )  &&
       ( ( ulParam1 & MCI_FROM)  || ( ulParam1 & MCI_TO ) ) )
     {
     return ( MCIERR_INVALID_FLAG );
     }



  /*******************************************
  * We only operate on blocks of information
  * in the file.  As a result, certain time
  * numbers can work out to retrieve the same
  * byte--do not report an error.  See
  * ConvertToMM for more information.
  ********************************************/


  if ( pEditParms->ulFrom == pEditParms->ulTo &&
       ( ulParam1 & MCI_FROM )                &&
       ( ulParam1 & MCI_TO   ) )
     {
     return ( MCIERR_SUCCESS );
     }



  /*******************************************
  * Ensure the io proc that we have loaded has
  * the ability to undo/redo.
  ********************************************/

  if ( !(pInstance->ulCapabilities & CAN_INSERT )  )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

// note--must check if destination is not adpcm--do in checkeditflags


  /*******************************************
  * Ensure the io proc that we have loaded has
  * the ability to write information to the
  * file.
  ********************************************/

  if ( !pInstance->ulUsingTemp  )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

  /************************************************
  * If the caller used the MCI_WAIT flag, then all
  * of the processing can be done on one thread.
  ************************************************/
  if ( ulParam1 & MCI_WAIT )

    {
    // investigate--change to fWaitPending

    pInstance->usWaitPending = TRUE;
    pInstance->usWaitMsg= MCI_SAVE;

    ulrc = StartPaste( pFuncBlock );

    pInstance->usWaitPending = FALSE;

    }
  else
    /***************************************************
    ** To determine whether or not an operation must be
    ** done on a thread, use the following criteria:
    **
    **   A. Will the operation take a long time.
    **   B. Is there any way to make it appear as if
    **      a thread was active.
    **
    **  In the case of a save, it can potentially be a
    **  LONG operation and we do not want to bully the
    **  applications thread, therefore,
    **
    ** We must perform the paste on a thread since
    ** notify was requested (note, if neither notify
    ** or wait are specified, then operate like a notify
    ** except without positing the message.
    ****************************************************/

    {
    /* Let other threads know that a save is about to start */

    pInstance->usNotifyPending = TRUE;
    pInstance->usNotPendingMsg = MCI_SAVE;

    /********************************************
    * Save thread will post a semaphore when it
    * safe to do processing, so reset it,
    * start the thread, and wait till its safe
    * to go on.
    ********************************************/

    DosResetEventSem (pInstance->hThreadSem, &ulCount);

    ulrc = _beginthread ( (ADMCTHREAD)StartPaste,
                          0,
                          NOTIFY_THREAD_STACKSIZE,
                          (void *)pFuncBlock );

    /*************************************************
    * Wait for the save thread to indicate that it is
    * safe to continue.
    **************************************************/
    if ( ulrc != -1 )
       {
       DosWaitEventSem (pInstance->hThreadSem, -1);
       ulrc = MCIERR_SUCCESS;
       }
    else
       {
       ulrc = MCIERR_OUT_OF_MEMORY;
       }

    } /* this call is not a wait */

  DosResetEventSem (pInstance->hThreadSem, &ulCount);

  /* Ensure that we are in stopped state after a save */

  pInstance->StreamInfo.ulState = MCI_STOP;


  return ( ulrc );

} /* MCIPaste */







/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StartPaste
*
* DESCRIPTIVE NAME: Pastes data from the clipboard into a file.
*
* FUNCTION:
*
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: None
*
* EXTERNAL REFERENCES: mmioSendMessage
*                      mmioGetLastError
*                      DosDelete
*
*********************** END OF SPECIFICATIONS *******************************/

RC _Optlink StartPaste (VOID * pvoid)



{
  FUNCTION_PARM_BLOCK * pFuncBlockCopy = (FUNCTION_PARM_BLOCK *) pvoid;
  LONG                 lReturnCode;

  ULONG                ulrc;
  ULONG                ulParam1;
  ULONG                ulBuffLen;          // Length of the buffer
  ULONG                ulFromPosition;     // Position to paste from
  ULONG                ulToPosition;       // Position to paste to
  ULONG                ulPasteLen;         // Length to paste into
  ULONG                ulOldTimeFormat;    // old time format
  ULONG                ulNewPosition;      // new position of file after paste
  ULONG                ulNumUndoNeeded = 1;/* Some pastes may require multiple
                                              undoes since they delete and insert */
  LONG                 lCurrFilePos;


  FUNCTION_PARM_BLOCK  FuncBlock;
  PMCI_EDIT_PARMS      pEditParms;
  MCI_EDIT_PARMS       EditParms;

  PVOID                pBuffer;           // pointer to wave data from clipboard.

  MMAUDIOHEADER        mmaudioheader;      // header to the memory file

  INSTANCE             *pInstance;         // Local Instance



  memmove( &FuncBlock, pFuncBlockCopy, sizeof( FUNCTION_PARM_BLOCK ) );


  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulParam1 = FuncBlock.ulParam1;

  if ( ulParam1 & MCI_NOTIFY )
     {
     memmove( &EditParms,
              ( PMCI_EDIT_PARMS ) FuncBlock.ulParam2,
              sizeof( MCI_EDIT_PARMS ) );

     pEditParms = &EditParms;
     }
  else
     {
     pEditParms = ( PMCI_EDIT_PARMS ) FuncBlock.ulParam2;
     }

  ulrc = MCIERR_SUCCESS;
  pInstance = (INSTANCE *) FuncBlock.ulpInstance;


  /*********************************************
  * We can free the save thread now that we have
  * obtained any possible file names
  **********************************************/

  DosPostEventSem ( pInstance->hThreadSem );


  /*******************************************
  * Determine where we are before we start the
  * copy.  This will be useful if the FROM
  * flag is not specified, in which case our
  * current position will be the FROM position.
  ********************************************/

  /* Check to see if a stream has been created */

  if ( pInstance->ulCreateFlag == PREROLL_STATE )
     {

     /**********************************************
     * Get the current stream time, it may be used
     * as the default starting position if MCI_FROM
     * was not specified.
     ***********************************************/

     ulrc = SpiGetTime( pInstance->StreamInfo.hStream,
                        ( PMMTIME ) &( pInstance->StreamInfo.mmStreamTime ) );

     if ( ulrc )
        {
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     // investigate combining into one function

     ulOldTimeFormat = pInstance->ulTimeUnits;
     pInstance->ulTimeUnits = lBYTES;

     ConvertTimeUnits ( pInstance,
                        ( PULONG ) &lCurrFilePos,
                        pInstance->StreamInfo.mmStreamTime );

     pInstance->ulTimeUnits = ulOldTimeFormat;

     } /* if stream has been created */
  else
     {
     lCurrFilePos = 0;
     }


  lReturnCode = mmioSeek( pInstance->hmmio,
                          lCurrFilePos,
                          SEEK_SET );

  if ( lReturnCode == MMIO_ERROR )
     {
     PasteNotify( &FuncBlock, ulParam1, MMIOERR_SEEK_FAILED );
     return ( MMIOERR_SEEK_FAILED );
     }



  /* Retrieve the from and to positions to operate on */

  GetFromToPosition( lCurrFilePos,
                      ulParam1,
                      pEditParms,
                      pInstance,
                      &ulFromPosition,
                      &ulToPosition );



  /*******************************************
  * If the user passed in a buffer, use that
  * rather than allocating our own.
  ********************************************/

  if ( ulParam1 & MCI_FROM_BUFFER )
     {

     if ( pInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample == 16 &&
          pEditParms->ulBufLen % 2 )
        {
        pEditParms->ulBufLen--;
        }


     ulBuffLen = pEditParms->ulBufLen;

//     pBuffer = HhpAllocBuffer( ulBuffLen, 0 );
//
//     if ( !pBuffer )
//        {
//        PasteNotify( &FuncBlock, ulParam1, MCIERR_OUT_OF_MEMORY );
//        return ( MCIERR_OUT_OF_MEMORY );
//        }
//
//     memmove( pBuffer, pEditParms->pBuff,  pEditParms->ulBufLen );
     pBuffer = pEditParms->pBuff;
     memmove( &mmaudioheader, pEditParms->pHeader,  sizeof( MMAUDIOHEADER ) );

     }
  else
     {
     /*******************************************
     * Retrieve information from the clipboard
     ********************************************/

     ulrc = GetClipboardData( &pBuffer,
                              pInstance,
                              MCI_PASTE,
                              &ulBuffLen,
                              ulParam1,
                              &mmaudioheader );

     if ( ulrc )
        {
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }
     } /* if to buffer not passed */



  /*******************************************
  * Figure out how much information must be
  * removed before pasting the information
  * into the file.
  ********************************************/

  ulPasteLen = ulToPosition - ulFromPosition;


  /*******************************************
  * remove the information in the file if
  * from/to are specified.
  ********************************************/

  if ( ulParam1 & MCI_FROM || ulParam1 & MCI_TO )
     {
     lReturnCode = mmioSendMessage( pInstance->hmmio,
                                    MMIOM_DELETE,
                                    pEditParms->ulFrom,
                                    ulPasteLen );

     if ( lReturnCode != MMIO_SUCCESS )
        {
        ulrc = mmioGetLastError( pInstance->hmmio );
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     ulNumUndoNeeded++;

     } /* If delete is required */



  if ( !( ulParam1 & MCI_TO_BUFFER ) )
     {
     /*******************************************
     * Let the io proc know that the information is
     * about to be inserted into the file
     ********************************************/

     lReturnCode = mmioSendMessage( pInstance->hmmio,
                                    MMIOM_BEGININSERT,
                                    0,
                                    0 );

     if ( lReturnCode != MMIO_SUCCESS )
        {
        ulrc = mmioGetLastError( pInstance->hmmio );
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     /*******************************************
     * Write the information that we reteived from
     * the clipboard
     *******************************************/


     lReturnCode = mmioWrite( pInstance->hmmio,
                              ( PSZ ) pBuffer,
                              ulBuffLen );

     if ( lReturnCode == MMIO_ERROR )
        {
        ulrc = mmioGetLastError( pInstance->hmmio );
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     /*******************************************
     * We have finised inserted the information
     * the paste is complete
     ********************************************/


    lReturnCode = mmioSendMessage( pInstance->hmmio,
                                   MMIOM_ENDINSERT,
                                   0,
                                   0 );

    if ( lReturnCode != MMIO_SUCCESS )
       {
       ulrc = mmioGetLastError( pInstance->hmmio );
       PasteNotify( &FuncBlock, ulParam1, ulrc );
       return ( ulrc );
       }


    /********************************************
    * Update the header information in the file
    * to reflect the fact that information has
    * been inserted (and possibly deleted).
    *********************************************/

    ulrc = SetAudioHeader (pInstance);

    if ( ulrc )
       {
       PasteNotify( &FuncBlock, ulParam1, ulrc );
       return ( ulrc );
       }

    ulrc = GetAudioHeader ( pInstance, 0  );

    if ( ulrc )
       {
       PasteNotify( &FuncBlock, ulParam1, ulrc );
       return ( ulrc );
       }

    AddNode( pInstance, ulNumUndoNeeded );
    pInstance->ulNumUndo++;

    } /* if the buffer flag was not passed in */
  else
    {
    /*--------------------------------
    * ALWAYS return the audio header
    * even if the call fails so that
    * the caller will know that
    * datatype.
    *-------------------------------*/

    memmove( pEditParms->pHeader, &mmaudioheader, sizeof( MMAUDIOHEADER ) );

    if ( ( ulBuffLen ) > pEditParms->ulBufLen )
       {
       // let the caller know how big the buffer is
       pEditParms->ulBufLen = ulBuffLen;
       PasteNotify( &FuncBlock, ulParam1, MCIERR_INVALID_BUFFER );
       HhpFreeBuffer( pBuffer );
       return ( MCIERR_INVALID_BUFFER ); // is there a buffer too small erorr ?
       }

    pEditParms->ulBufLen = ulBuffLen;


    /* Place the buffer with the length in the edit parms */

    memmove( pEditParms->pBuff, pBuffer, ulBuffLen );

    } /* the buffer flag WAS passed in */

  /* Clean up memory allocated by clipboard operation */

  if ( !(ulParam1 & MCI_FROM_BUFFER ) )
     {
     HhpFreeBuffer( pBuffer );
     }

  if ( pInstance->ulCreateFlag == PREROLL_STATE && !( ulParam1 & MCI_TO_BUFFER ) )
     {
     ulOldTimeFormat = pInstance->ulTimeUnits;
     pInstance->ulTimeUnits = lBYTES;

     ulrc = ConvertToMM ( pInstance,
                          &ulNewPosition,
                          ulFromPosition + ulBuffLen );

     pInstance->ulTimeUnits = ulOldTimeFormat;

     ulrc = ADMCSeekStream ( pInstance,
                            SPI_SEEK_ABSOLUTE,
                            ulNewPosition );

     if ( ulrc )
        {
        PasteNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     } /* if stream was created already */

  PasteNotify( &FuncBlock, ulParam1, MCI_NOTIFY_SUCCESSFUL );



  return ( MCIERR_SUCCESS );

} /* StartPaste */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: PasteNotify
*
* FUNCTION: If a paste was done on via MCI_NOTIFY, alert caller to completion
*           or an error.
*
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: None
*
* EXTERNAL REFERENCES: mmioSendMessage
*                      mmioGetLastError
*                      DosDelete
*
*********************** END OF SPECIFICATIONS *******************************/

void PasteNotify ( FUNCTION_PARM_BLOCK *pFuncBlock,
                   ULONG               ulParam1,
                   ULONG               ulNotifyResult )

{
   if ( ulParam1 & MCI_NOTIFY )
      {
      PostMDMMessage (ulNotifyResult, MCI_PASTE, pFuncBlock);

      } /* if notify flag was passed */

  /*********************************************
  * If any operation sees that the save thread
  * is still processing, they will wait on the
  * thread semaphore.  Since we are done with
  * save processing post the semaphore so
  * waiting threads can continue.
  *********************************************/

  pFuncBlock->pInstance->usNotifyPending = FALSE;
  DosPostEventSem ( pFuncBlock->pInstance->hThreadSem );


} /* PasteNotify */


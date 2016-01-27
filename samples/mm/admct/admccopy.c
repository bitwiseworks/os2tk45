/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCCOPY.C
*
* DESCRIPTIVE: Digital Audio copy to the clipboard
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: This function accepts cut, copy, delete messages and
*           acts accordingly.
*           First, Check flags and paste audio information to the
*           clipboard (if not delete).
*           Copy either from a buffer or from a file.
*
* CONCEPTS ILLUSTRATED :
*  1. Default positioning for cut/copy/delete messages.
*  2. Using MMIOM_DELETE to remove information from a file.
*  3. Positioning after a cut/copy/delete
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_COPY message.
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
*********************** END OF SPECIFICATIONS **********************/

#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS



#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <stdlib.h>                     // begin thread prototype
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
#include <stdio.h>

RC MCICopy ( FUNCTION_PARM_BLOCK *pFuncBlock,
             ULONG               ulMessage )

{
  ULONG         ulCount;
  ULONG         ulParam1;           // Flags for this Msg
  ULONG         ulrc;               // Error Code Propogated
//  ULONG         ulFromPosition;     // position to copy from
//  ULONG         ulToPosition;       // position to copy to
//  ULONG         ulNewPosition;      // position after operation

//  ULONG         ulBuffLen;          // length of the buffer to copy
//  ULONG         ulOldTimeFormat;    //

//  LONG          lReturnCode;        // return code from mmio
//  LONG          lCurrFilePos;       // Get current file position
//  LONG          lSeekFlags;

  INSTANCE      *pInstance;       // Local Instance

  MCI_EDIT_PARMS *pEditParms;        // Pointer to structure to edit

//  PVOID          pBuffer;            // pointer to wave data from clipboard.

//  PMMAUDIOHEADER pmmaudioheader;

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
  * We only operate on blocks of information
  * in the file.  As a result, certain time
  * numbers can work out to retrieve the same
  * byte--do not report an error.  See
  * ConvertToMM for more information.
  ********************************************/

  if ( pEditParms->ulFrom == pEditParms->ulTo &&
       ( ( ulParam1 &  MCI_FROM ) && ( ulParam1 & MCI_TO )) )
     {
     return ( MCIERR_SUCCESS );
     }

  /*******************************************
  * Ensure the io proc that we have loaded has
  * the ability to delete.
  ********************************************/

  if ( !(pInstance->ulCapabilities & CAN_DELETE)  )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

  /*******************************************
  * Ensure the io proc that we have loaded has
  * the ability to cut information from the
  * file (i.e. everything but copy).
  ********************************************/

  if ( !pInstance->ulUsingTemp &&
       ulMessage != MCI_COPY )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }


  /*******************************************
  * From and To are pointless with the buffer
  * flag.
  ********************************************/


  if ( ( ulParam1 & MCI_FROM_BUFFER )  &&
       ( ( ulParam1 & MCI_FROM)  || ( ulParam1 & MCI_TO ) ) )
     {
     return ( MCIERR_INVALID_FLAG );
     }


  /*******************************************
  * Neither cut nor paste support the from buffer
  * flag since they operate on a file.
  ********************************************/

  if ( ( ulParam1 & MCI_FROM_BUFFER ) || ( ulParam1 & MCI_TO_BUFFER ) )
     if  (  (ulMessage == MCI_DELETE ) ||
            (ulMessage == MCI_CUT && (ulParam1 &  MCI_FROM_BUFFER )) )
     {
     return ( MCIERR_INVALID_FLAG );
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

    ulrc = StartCopy( pFuncBlock );

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

    ulrc = _beginthread ( (ADMCTHREAD) StartCopy,
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





  /* The stream is in a stopped state after cut/copy/delete */

  pInstance->StreamInfo.ulState = MCI_STOP;

  return ( MCIERR_SUCCESS );

} /* mcicopy */





/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIMagic
*
* DESCRIPTIVE: Will undo/redo the last logical action
*
* FUNCTION:  Undo will place the instance in the state before the
*            last logical action ( record, cut, copy, paste delete).
*            By contrast, redo will place the instance in the state
*            it was before an undo.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_UNDO message.
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
*********************** END OF SPECIFICATIONS **********************/



RC MCIMagic ( FUNCTION_PARM_BLOCK *pFuncBlock,
              ULONG               ulOperation )

{

  ULONG         ulrc;               // Error Code Propogated
//  ULONG         ulFrom;             // Position to start deleting
//  ULONG         ulLength;           // Length of info to delete
//  ULONG         ulMessage;
  ULONG         ulActionLoop;       // Some actions (like paste)
                                    // require multiple undo/redo
  ULONG         ulMaxLevels;

  LONG          lReturnCode;        // rc for mmio operations

  INSTANCE      *pInstance;       // Local Instance

//  MCI_EDIT_PARMS *pEditParms;        // Pointer to structure to edit

  PUNDOLIST     pTempList;

  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulrc = MCIERR_SUCCESS;
  pInstance = (INSTANCE *) pFuncBlock->ulpInstance;


  /*******************************************
  * Check for valid flags
  ********************************************/

  ulrc = CheckEditFlags( pFuncBlock, TRUE );

  if ( ulrc != MCIERR_SUCCESS )
     {
     return ( ulrc );
     }

  /*******************************************
  * Ensure the io proc that we have loaded has
  * the ability to undo/redo.
  ********************************************/

  if ( !(pInstance->ulCapabilities & CAN_UNDOREDO )  )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

  /*******************************************
  * We are currently using an array of undo/redo
  * levels.  Check to ensure that we will have
  * room to store the number of undoes
  ********************************************/
  if ( ulOperation == MMIOM_UNDO )
     {
     if  ( !pInstance->pUndoList )
        {
        return ( MCIERR_CANNOT_UNDO );
        }
     else
        {
        ulMaxLevels = pInstance->pUndoList->ulNumLogicalActions;
        }
     } /* If we are to undo */

  /*******************************************
  * We are currently using an array of undo/redo
  * levels.  Check to ensure that we will have
  * room to store the number of redoes
  ********************************************/
  if ( ulOperation == MMIOM_REDO )
     {
     if ( !pInstance->pRedoList )
        {
        return ( MCIERR_CANNOT_REDO );
        }
     else
        {
        ulMaxLevels = pInstance->pRedoList->ulNumLogicalActions;
        }
     } /* If redo */

  /**************************************************
  * Ask the io proc to undo or redo the last logical
  * action (record, cut, delete, paste ).
  **************************************************/

  for ( ulActionLoop = 0; ulActionLoop < ulMaxLevels; ulActionLoop++ )
     {
     lReturnCode = mmioSendMessage( pInstance->hmmio,
                                   ulOperation,
                                   0,
                                   0 );

     if ( lReturnCode == MMIO_ERROR )
        {
        if ( ulOperation == MMIOM_UNDO )
           {
           return ( MCIERR_CANNOT_UNDO );
           }
        else
           {
           return ( MCIERR_CANNOT_REDO );
           }

        }  /* if an error happened */
     } /* Undo all logical actions (i.e. paste may take more than one */

  ulrc = SetAudioHeader (pInstance);

  if ( ulrc )
     {
     return ( ulrc );
     }

  ulrc = GetAudioHeader ( pInstance, 0 );

  if ( ulrc )
     {
     return ( ulrc );
     }

  /*********************************
  * After an undo/redo, the stream
  * position is 0.
  *********************************/

  if ( pInstance->ulCreateFlag == PREROLL_STATE)
     {
     ulrc = ADMCSeekStream ( pInstance,
                            SPI_SEEK_ABSOLUTE,
                            0 );
     }

  /* The stream is in a stopped state after an undo/redo */

  pInstance->StreamInfo.ulState = MCI_STOP;

  /* Update our array of undo levels */

  if ( ulOperation == MMIOM_UNDO )
     {
     pInstance->ulNumUndo--;
     pInstance->ulNumRedo++;
     pTempList = pInstance->pUndoList;
     pInstance->pUndoList = pInstance->pUndoList->pNextList;
     pTempList->pNextList = pInstance->pRedoList;

     pInstance->pRedoList = pTempList;
     }
  else
     {
     pInstance->ulNumRedo--;
     pInstance->ulNumUndo++;
     pTempList = pInstance->pRedoList;
     pInstance->pRedoList = pInstance->pRedoList->pNextList;
     pTempList->pNextList = pInstance->pUndoList;
     pInstance->pUndoList = pTempList;

     }

  return ( ulrc );

} /* MCIMajic */





/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StartCopy
*
* DESCRIPTIVE NAME: Copies data to the clipboard from a file or to a buffer.
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

RC _Optlink StartCopy (VOID * pvoid)

{
  FUNCTION_PARM_BLOCK * pFuncBlockCopy = (FUNCTION_PARM_BLOCK *) pvoid;
  ULONG         ulParam1;           // Flags for this Msg
  ULONG         ulrc;               // Error Code Propogated
  ULONG         ulFromPosition;     // position to copy from
  ULONG         ulToPosition;       // position to copy to
  ULONG         ulNewPosition;      // position after operation

  ULONG         ulBuffLen;          // length of the buffer to copy
  ULONG         ulOldTimeFormat;    //
  ULONG         ulMessage;

  LONG          lReturnCode;        // return code from mmio
  LONG          lCurrFilePos;       // Get current file position
  LONG          lSeekFlags;

  INSTANCE      *pInstance;       // Local Instance

  MCI_EDIT_PARMS *pEditParms;        // Pointer to structure to edit
  MCI_EDIT_PARMS EditParms;

  PVOID          pBuffer;            // pointer to wave data from clipboard.

  PMMAUDIOHEADER pmmaudioheader;
  FUNCTION_PARM_BLOCK  FuncBlock;


  memmove( &FuncBlock, pFuncBlockCopy, sizeof( FUNCTION_PARM_BLOCK ) );


  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulParam1 = FuncBlock.ulParam1;
  ulMessage = FuncBlock.usMessage;


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
        CopyNotify( &FuncBlock, ulParam1, ulrc );
        return ( ulrc );
        }

     // investigate--combine the below routines into one function

     ulOldTimeFormat = pInstance->ulTimeUnits;
     pInstance->ulTimeUnits = lBYTES;

     ConvertTimeUnits ( pInstance,
                        ( PULONG ) &lCurrFilePos,
                        pInstance->StreamInfo.mmStreamTime );

     pInstance->ulTimeUnits = ulOldTimeFormat;

     if ( lCurrFilePos > pInstance->ulDataSize )
        {
        lCurrFilePos = 0;
        }

     lSeekFlags = SEEK_SET;

     } /* if stream has been created */
  else
     {
     lCurrFilePos = 0;
     lSeekFlags = SEEK_CUR;
     } /* if no stream has been created */

  /* Ensure that the file is at the position we think it is */

  lReturnCode = mmioSeek( pInstance->hmmio,
                          lCurrFilePos,
                          lSeekFlags );

  if ( lReturnCode == MMIO_ERROR )
     {
     CopyNotify( &FuncBlock, ulParam1, MMIOERR_SEEK_FAILED );
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
  * Read information into a buffer and prepare
  * to send it to the clipboard.
  ********************************************/

  ulBuffLen = ulToPosition - ulFromPosition;

  /*****************************************
  * But cut/copy place information into the
  * clipboard.  However, delete simply
  * removes data from the file w/o any
  * clipboard operations.
  ******************************************/

  if ( ulMessage != MCI_DELETE )
     {
     if ( ulParam1 & MCI_FROM_BUFFER )
        {
        /* Allocate a buffer to store the raw information */

        pBuffer = ( PVOID ) HhpAllocBuffer( pEditParms->ulBufLen + 200, 0 );

        memmove( pBuffer, pEditParms->pBuff, pEditParms->ulBufLen );

        ulBuffLen = pEditParms->ulBufLen;
        }
     else
        {
        if ( ulParam1 & MCI_TO_BUFFER )
           {
           /* -------------------------------------------------
           * Ensure that the buffer that the caller passed in
           * is large enough
           *--------------------------------------------------*/

           if ( ulBuffLen > pEditParms->ulBufLen )
              {
              pEditParms->ulBufLen = ulBuffLen;
              CopyNotify( &FuncBlock, ulParam1, MCIERR_INVALID_BUFFER );
              return ( MCIERR_INVALID_BUFFER );
              }

           /* Use the callers buffer rather than our own for efficiency */

           pBuffer = pEditParms->pBuff;

           }
        else
           {
           pBuffer = ( PVOID ) HhpAllocBuffer( (ulToPosition - ulFromPosition) + 100, 0 );
           }

        /* Read the info necessary to copy into the clipboard */

        lReturnCode = mmioRead( pInstance->hmmio,
                                ( PSZ ) pBuffer,
                                ulBuffLen ); // hard coded for now

        if ( lReturnCode == MMIO_ERROR )
           {
           ulrc = mmioGetLastError( pInstance->hmmio );
           CopyNotify( &FuncBlock, ulParam1, ulrc );
           return ( ulrc );
           }

     } /* else the buffer flag was not specified */

     /*******************************************
     * Place information in the clipboard.
     ********************************************/

     if ( ulParam1 & MCI_TO_BUFFER )
        {
        pEditParms->ulBufLen = ulBuffLen;
        memmove( pEditParms->pHeader,
                    &pInstance->mmAudioHeader, sizeof( MMAUDIOHEADER ) );

        pmmaudioheader = ( PMMAUDIOHEADER ) pEditParms->pHeader;
        pmmaudioheader->mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes = ulBuffLen;
        }
     else
        {
        ulrc = PutClipboardData( &pBuffer, pInstance, MCI_COPY, ulBuffLen );
        }

     }

  /**********************************************
  * If the caller wants to do a cut or delete, then
  * remove the information from the file.  Else
  * if the copy message was specified, no data
  * will be lost, just seek back to where we
  * were before the copy.
  **********************************************/

  if ( ulMessage != MCI_COPY )
    {
    /*******************************************
    * Remove the information from the file
    ********************************************/

    lReturnCode = mmioSendMessage( pInstance->hmmio,
                                   MMIOM_DELETE,
                                   ulFromPosition,
                                   ulBuffLen );

    if ( lReturnCode != MMIO_SUCCESS )
      {
      CopyNotify( &FuncBlock, ulParam1, lReturnCode );
      return ( lReturnCode );
      }

    /********************************************
    * Cut/Delete are required to place the
    * logical position of the file after the
    * MCI_FROM position, whereas copy is supposed
    * to return the file to the position where
    * it was before the copy.
    *********************************************/


    if ( pInstance->ulCreateFlag == PREROLL_STATE)
       {

       // investigate--combine the below routines into one function

       ulOldTimeFormat = pInstance->ulTimeUnits;
       pInstance->ulTimeUnits = lBYTES;

       ConvertToMM ( pInstance,
                     &ulNewPosition,
                     pEditParms->ulFrom );

       ulrc = ADMCSeekStream ( pInstance,
                              SPI_SEEK_ABSOLUTE,
                              ulNewPosition );

      if ( ulrc )
         {
         CopyNotify( &FuncBlock, ulParam1, ulrc );
         return ( ulrc );
         }


       pInstance->ulTimeUnits = ulOldTimeFormat;


       } /* if a stream has been created */

     /* Update the undo levels */

     AddNode( pInstance, 1 );
     pInstance->ulNumUndo++;


    /* Ensure that the file header contains the correct length */

    ulrc = SetAudioHeader (pInstance);

    if ( ulrc )
       {
       CopyNotify( &FuncBlock, ulParam1, ulrc );
       return ( ulrc );
       }

    ulrc = GetAudioHeader ( pInstance, 0  );

    if ( ulrc )
       {
       CopyNotify( &FuncBlock, ulParam1, ulrc );
       return ( ulrc );
       }


    } /* if cut message was specified */
  else
    {

    /*******************************************
    * Return to where we were before the copy
    * occurred
    ********************************************/

    lReturnCode = mmioSeek( pInstance->hmmio,
                            lCurrFilePos,
                            SEEK_SET );

    if ( lReturnCode == MMIO_ERROR )
       {
       CopyNotify( &FuncBlock, ulParam1, lReturnCode );
       return ( MMIOERR_SEEK_FAILED );
       }

    } /* Copy message was specified */

  CopyNotify( &FuncBlock, ulParam1, MCI_NOTIFY_SUCCESSFUL );

  return ( MCIERR_SUCCESS );

} /* StartCopy */


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

void CopyNotify ( FUNCTION_PARM_BLOCK *pFuncBlock,
                  ULONG               ulParam1,
                  ULONG               ulNotifyResult )

{
   if ( ulParam1 & MCI_NOTIFY )
      {
      PostMDMMessage (ulNotifyResult, pFuncBlock->usMessage, pFuncBlock);

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



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: AddNode
*
* DESCRIPTIVE
*
* FUNCTION:  Adds undo/redo levels to a linked list
*
* NOTES: This function illustrates how to extend an
*
* ENTRY POINTS:
*
* INPUT:
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
*********************** END OF SPECIFICATIONS **********************/
RC AddNode ( INSTANCE   *pInstance,
             ULONG      ulNumActions )

{
   PUNDOLIST   pUndoList;         // temporary pointer to item in linked list

   extern HHUGEHEAP     heap;                // Global MCD Heap

   AcquireProcSem ();

   if ( !(pUndoList = HhpAllocMem (heap, sizeof ( UNDOLIST ) ) ) )
      {
      return (MCIERR_OUT_OF_MEMORY);
      }

   ReleaseProcSem ();

   pUndoList->ulNumLogicalActions = ulNumActions;
   pUndoList->pNextList = pInstance->pUndoList;
   pInstance->pUndoList = pUndoList;

   return ( MCIERR_SUCCESS );

} /* AddNode */




/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: RemoveNode
*
* DESCRIPTIVE
*
* FUNCTION:  Removes a undo node from a linked list
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
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/
RC RemoveUndoNodes( INSTANCE     *pInstance )

{
   PUNDOLIST           pTempUndo;

   extern HHUGEHEAP     heap;                // Global MCD Heap

   pTempUndo = pInstance->pUndoList;

   /* If there is a valid pointer--remove this node from the undo list */

   while ( pTempUndo )
      {
      pInstance->pUndoList = pInstance->pUndoList->pNextList;
      CleanUp ((PVOID) pTempUndo );
      pTempUndo = pInstance->pUndoList;

      }

   pTempUndo = pInstance->pRedoList;

   /* If there is a valid pointer--remove this node from the redo list */

   while ( pTempUndo )
      {
      pInstance->pRedoList = pInstance->pRedoList->pNextList;
      CleanUp ((PVOID) pTempUndo );
      pTempUndo = pInstance->pRedoList;

      }

   return ( MCIERR_SUCCESS );

} /* RemoveNode */




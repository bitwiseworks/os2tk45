/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCSAVE.c
*
* DESCRIPTIVE NAME: Audio MCD WaveAudio Save Routines
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*
* FUNCTION:Save an Waveform Audio Element.
*
*  MCI_SAVE is not a required command, however, if a streaming MCD takes
*  advantage of temporary files, then it should use MCI_SAVE.
*
*
* NOTES:
*
*
*********************** END OF SPECIFICATIONS **********************/

#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <stdlib.h>                     // begin thread prototype
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                     // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISAVEFILE
*
* DESCRIPTIVE      Audio MCD Save File.
*
* FUNCTION:  Save Existing element to desired format.
*            This function first ensures that the flags are vaild.
*            Then it ensures that all pointers point to valid memory.
*            If any operations are currently active, they are then
*            aborted.
*            Finally, the save operation itself is then done.
*
* NOTES: This function illustrates how to use the mmioSendMessage
*        API to save a file.  It also explains when/why one would
*        want to spawn a thread for an operation.  Finally, it shows
*        how to smoothly interrupt a previous operation.
*
* ENTRY POINTS:
*
* INPUT: MCI_SAVE message.
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  StartSave.
*                       PostMDMMessage
*
* EXTERNAL REFERENCES:  DosWaitEventSem
*                       DosResetEventSem
*                       DosCreateThread
*
*********************** END OF SPECIFICATIONS **********************/
RC  MCISaveFile (FUNCTION_PARM_BLOCK * pFuncBlock)
{
  ULONG             ulrc;              // Error Value
  ULONG             ulParam1;
  ULONG             ulCheckFlags;
  ULONG             ulCnt;
  ULONG             ulAbortNotify = 0; // whether or not to abort a command

  INSTANCE          *ulpInstance;      // Local Instance
  PMCI_SAVE_PARMS   pSaveParms;       // Msg Data

//  PSZ               pszSaveName;

  /*****************************************
  * Initialize some variables
  *****************************************/

  ulParam1 =    pFuncBlock->ulParam1;
  pSaveParms = (PMCI_SAVE_PARMS) pFuncBlock->ulParam2;
  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

  /*****************************************
  * Ensure that the MCI_SAVE_PARMS contain
  * valid memory.
  *****************************************/

  ulrc = CheckMem ( (PVOID)pSaveParms,
                    sizeof (MCI_SAVE_PARMS),
                    PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
     {
     return ( MCIERR_MISSING_PARAMETER );
     }


  ulCheckFlags = ulParam1;

  ulCheckFlags &= ~( MCI_WAIT + MCI_NOTIFY + MCI_SAVE_FILE);

  /*****************************************
  * The save routine only permits MCI_WAIT,
  * MCI_NOTIFY + MCI_SAVE_FILE.  If any other
  * flags are passed in, then return an error
  ******************************************/

  if (ulCheckFlags > 0)
     {
     return ( MCIERR_INVALID_FLAG );
     }

  /********************************************
  * Check to see if the IO Proc we have loaded
  * has the ability to save files.  The riff
  * wave proc does, but others (like the AVC
  * IO Proc) do not.
  ********************************************/

  if ( !ulpInstance->ulCapabilities & CAN_SAVE )
    {
    return (MCIERR_UNSUPPORTED_FUNCTION);
    }

  /* If no file is loaded, a save is impossible */

  if ( ulpInstance->fFileExists != TRUE )
     {
     return (MCIERR_FILE_NOT_FOUND);
     }

  /*********************************************
  * If temp files are not active, then all the
  * changes have been made to the actual file
  * and a save is not practical nor necessary
  **********************************************/

  if ( !ulpInstance->ulUsingTemp )
    {
    return (MCIERR_UNSUPPORTED_FUNCTION);
    }


  /* Check to see if the user passed in a filename */

  if ( ulParam1 & MCI_SAVE_FILE )
    {
    /*****************************************
    * Ensure that they passed a valid pointer
    *****************************************/

    ulrc = CheckMem ( (PVOID) pSaveParms->pszFileName,
                      1,
                      PAG_READ ) ;

    if (ulrc != MCIERR_SUCCESS)
        return (MCIERR_MISSING_PARAMETER);

    /* Create a local copy of the pointer */

//    pszSaveName = (PSZ) pSaveParms->pszFileName;
    }
  else
    {

    /*****************************************
    * If the caller had us generate a filename
    * on the open, then save w/o a new filename
    * is invalid
    *****************************************/
    if  ( ulpInstance->ulCreatedName )
       {
       return ( MCIERR_MISSING_FLAG );
       }

//    pszSaveName = NULL;
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

    /**********************************************************
    * If there other commands alive (such as a play/save or
    * record, then they should be aborted by the save command
    * To abort the command it is necessary to post a message
    * a stop the stream
    ***********************************************************/
    if ( ulAbortNotify )

      {
      /* Stop any commands in progress */

      GenericThreadAbort( ulpInstance, pFuncBlock, 0 );

      /************************************************
      * Ensure that no other threads think an operation
      * is active by setting a flag.
      *************************************************/

      ulpInstance->usNotifyPending = FALSE;

    }

  /*****************************************
  * If the stream is in a state where a stop
  * is required (i.e. buffers have not been
  * flush to the disk) then stop the stream
  ******************************************/

  if ( STRMSTATE != MCI_STOP )
      {
      ulrc = ADMCStopStream (ulpInstance, SPI_STOP_DISCARD );

        if (!ulrc)
          {
          DosWaitEventSem (ulpInstance->hEventSem, (ULONG) ONE_MINUTE );
          }

      } /* If stream is not in stopped state */



  /************************************************
  * If the caller used the MCI_WAIT flag, then all
  * of the processing can be done on one thread.
  ************************************************/
  if ( ulParam1 & MCI_WAIT )

    {
    // investigate--change to fWaitPending

    ulpInstance->usWaitPending = TRUE;
    ulpInstance->usWaitMsg= MCI_SAVE;

    ulrc = StartSave (pFuncBlock);

    ulpInstance->usWaitPending = FALSE;

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
    ** We must perform the save on a thread since
    ** notify was requested (note, if neither notify
    ** or wait are specified, then operate like a notify
    ** except without positing the message.
    ****************************************************/

    {
    /* Let other threads know that a save is about to start */

    ulpInstance->usNotifyPending = TRUE;
    ulpInstance->usNotPendingMsg = MCI_SAVE;

    /********************************************
    * Save thread will post a semaphore when it
    * safe to do processing, so reset it,
    * start the thread, and wait till its safe
    * to go on.
    ********************************************/

    DosResetEventSem (ulpInstance->hThreadSem, &ulCnt);

    ulrc = _beginthread ( (ADMCTHREAD)StartSave,
                          0,
                          NOTIFY_THREAD_STACKSIZE,
                         (void *)pFuncBlock );

    /*************************************************
    * Wait for the save thread to indicate that it is
    * safe to continue.
    **************************************************/
    if ( ulrc != -1 )
       {
       DosWaitEventSem (ulpInstance->hThreadSem, -1);
       ulrc = MCIERR_SUCCESS;
       }
    else
       {
       ulrc = MCIERR_OUT_OF_MEMORY;
       }

    } /* this call is not a wait */

    DosResetEventSem (ulpInstance->hThreadSem, &ulCnt);

    /* Ensure that we are in stopped state after a save */

    STRMSTATE = MCI_STOP;

    /* Remove any undo nodes if they were allocated */

    RemoveUndoNodes( ulpInstance );

    return ( ulrc );

} /* MCISaveFile */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StartSave.
*
* DESCRIPTIVE NAME: Saves a file using mmio message MMIOM_SAVE
*
* FUNCTION:
*
*
* NOTES: This routine is called using  a separate thread spawned by
*        MCD on MCI Notify.
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
RC  _Optlink StartSave (VOID * pvoid)
{

   FUNCTION_PARM_BLOCK * pFuncBlockCopy = (FUNCTION_PARM_BLOCK *) pvoid;
   ULONG               ulrc = MCIERR_SUCCESS;
   ULONG               ulErr = MCI_NOTIFY_SUCCESSFUL;
   ULONG               ulParam1;

   INSTANCE            *ulpInstance;

   PMCI_SAVE_PARMS    pSaveParms;

   CHAR                chSaveName[ CCHMAXPATH ];
   PSZ                 pszSaveName;

  FUNCTION_PARM_BLOCK  FuncBlock;

  memmove( &FuncBlock, pFuncBlockCopy, sizeof( FUNCTION_PARM_BLOCK ) );


   /* Initialize some variable */

   ulpInstance = (INSTANCE *) FuncBlock.ulpInstance;
   pSaveParms = (PMCI_SAVE_PARMS) FuncBlock.ulParam2;
   ulParam1    = FuncBlock.ulParam1;

   /***************************************************
   * If a notify thread passed the filename pointer,
   * we must save the string since that thread could
   * return before we finish processing and the pointer
   * may no longer be valid.
   ***************************************************/

   if ( ulParam1 & MCI_SAVE_FILE )
      {
      strcpy( chSaveName, ( PSZ ) pSaveParms->pszFileName );
      pszSaveName = (PSZ) chSaveName;
      }
   else
      {
      pszSaveName = NULL;
      }

   /*********************************************
   * We can free the save thread now that we have
   * obtained any possible file names
   **********************************************/

   DosPostEventSem ( ulpInstance->hThreadSem );


   /**********************************************
   * This call illustrates the use of the
   * mmioSendMessage.  We are requesting that the
   * IO Proc that is currently loaded save all of
   * the changes that have been done to the file.
   ***********************************************/

   ulrc = mmioSendMessage( ulpInstance->hmmio,
                           MMIOM_SAVE,
                           (LONG) pszSaveName,
                           0 );

   /*********************************************
   * If there were no errors, then the filename the
   * caller requested is the default file name
   **********************************************/

   if ( ulParam1 & MCI_SAVE_FILE && !ulrc )
      {
      /* If we created a temp file, it is our duty to remove it */

      if ( ulpInstance->ulCreatedName )
         {
         DosDelete( ( PSZ ) ulpInstance->pszAudioFile );
         }

      /***************************************
      * Keep the name of the new file.  This
      * will be useful if the caller does an
      * MCI_INFO for the filename.
      ***************************************/

      strcpy( ( PSZ ) ulpInstance->pszAudioFile,
               chSaveName );

      }

   /**************************************************
   * Set flag which indicates that we did not create
   * the current element.  This flag is used by the
   * close and load routines to determine whether or
   * not it is necessary to remove a file that the
   * mcd created.
   **************************************************/

   ulpInstance->ulCreatedName = FALSE;



   /**********************************************
   * If an error occurred, then inform  the caller
   * of the problem.
   **********************************************/

   if ( ulrc )
      {
      /******************************************
      * Right now, we just know what error
      * occurred, mmioGetLastError will give
      * us more details on exactly what happened
      ******************************************/

      ulErr = mmioGetLastError( ulpInstance->hmmio );

      /*********************************
      * If the path is not valid return
      * file not found
      *********************************/

      if ( ulErr == ERROR_FILE_NOT_FOUND ||
           ulErr == ERROR_PATH_NOT_FOUND ||
           ulErr == ERROR_INVALID_DRIVE)
         {
         ulErr = MCIERR_FILE_NOT_FOUND;
         }

      /*******************************************
      * CANNOT_WRITE comes back when there is no
      * disk space
      ********************************************/

      else if ( ulErr == MMIOERR_CANNOTWRITE )
         {
         ulErr = MCIERR_TARGET_DEVICE_FULL;
         }

      /*******************************************
      * File attribute errors can happen when
      * one writes to a readonly device (like a CD)
      * or a readonly file etc.
      ********************************************/

      else if ( ulErr == ERROR_ACCESS_DENIED ||
                ulErr == ERROR_SHARING_VIOLATION  )
         {
         ulErr = MCIERR_FILE_ATTRIBUTE;
         }

      /* If notify, send message to application */

      if (ulParam1 & MCI_NOTIFY)
         {

         PostMDMMessage (ulErr, MCI_SAVE, &FuncBlock);
         }

      /* else return to caller with error */

      else
         {

         return ( ulErr );
         }
      }

   else if (ulParam1 & MCI_NOTIFY)
      {
      PostMDMMessage (ulErr, MCI_SAVE, &FuncBlock);
      }


   /* If this call was done with a wait, we are done */

   if ( ulParam1 & MCI_WAIT )
      {
      return ( MCIERR_SUCCESS );
      }


   /*********************************************
   * If any operation sees that the save thread
   * is still processing, they will wait on the
   * thread semaphore.  Since we are done with
   * save processing post the semaphore so
   * waiting threads can continue.
   *********************************************/

   ulpInstance->usNotifyPending = FALSE;
   DosPostEventSem ( ulpInstance->hThreadSem );

   return ( ulErr );

} /* Start Save */


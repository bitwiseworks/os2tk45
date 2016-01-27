/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIRECD.C
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* DESCRIPTIVE NAME: Audio MCD WaveAudio Recording Source.
*
* FUNCTION: Record into  Waveform Audio Element.
*
*  On MCI_RECORD, a streaming MCD should perform the following commands:
*
*  Always check flags and validate memory first.  This way if the flags
*   are invalid, the previous command will not be interrupted.
*  If there is a command active on another thread (i.e. and play, record
*   or save), then either abort (play or save) or superced (record) by
*   stopping the stream and sending a message to the caller.
*  If the stream is going the wrong way (e.g. it is setup for playback)
*   then destroy the stream.
*  If no stream has been created, then create one.  If the stream handler
*   needs to associate a data object, do it here.
*  If we destroyed a play back stream before creating the record stream,
*   seek to the same position in the record stream where the play back stream
*   was.
*  Enable any events (such as cuepoints or position advises).
*  Setup MMIO networking hooks.
*  Start stream.
*  Wait for a streaming event.
*  Stop the stream if necessary.
*  If MCI_NOTIFY was sent used, inform the caller of command completion.
*
* NOTES: This source file illustrates the following concepts:
*         A. Aborting or superceding a previous command
*         B. Transitioning from a play stream->record stream
*            so that the caller at the MCI Layer is unaware of the
*            change.
*         C. Processing a stream which was altered by the MCI_SET
*            command.
*         D. Proper creation of a stream.
*         E. Processing the MCI_FROM/MCI_TO flags in a streaming
*            environment.
*         F. Enable both cuepoints and position advises.
*         G. Processing record insert.
*         H. Determing when the creation of a thread is necessary
*            for an MCI_NOTIFY
*         I. How to use MMIO networking features (via MMIOM_BEGINSTREAM).
*         J. Proper handling of a record event procedure.
*         K. Proper termination of a record thread.
*         L. Communicating with MMIO for retrieving/seting file header.
*         M. Communicating with MMIO to process recording/inserting.
*
*
* ENTRY POINTS:
*
* INPUT: MCI_Record message.
*        MCI_FROM Flag
*        MCI_TO   Flag
*        MCI_RECORD_OVERWRITE   Flag
*        MCI_RECORD_INSERT      Flag
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*           The Original Element may be modified.
*           All Recording is lost if MCI_SAVE message is not specified.
*
*
* INTERNAL REFERENCES:   CreateNAssocStream ().
*                        DestroyStream ().
*                        AssocMemPlayToAudioStream ().
*                        CreateToEvent().
*                        ConvertTimeUnits ().
*                        StartRecord().
*                        ReEventProc().
*                        SetAudioDevice().
*                        SetWaveDeviceDefaults().
*                        OpenFile().
*                        CheckMem ().
*
* EXTERNAL REFERENCES:   DosResetEventSem ()        - OS/2 API
*                        DosPostEventSem  ()        - OS/2 API
*                        DosCreateThread ()         - OS/2 API
*                        SpiEnableEvent ()          - MME API
*                        SpiStopStream ()           - MME API
*                        SpiCreateStream ()         - MME API
*                        SpiAssociate()             - MME API
*                        SpiSeekStream ()           - MME API
*                        mdmDriverNotify ()         - MME API
*                        mmioSetHeader ()           - MME API
*
*********************** END OF SPECIFICATIONS **********************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <string.h>
#include <stdlib.h>                     // begin thread prototype
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                     // Mci Driver Include.
#include <mcd.h>                        // VSDIDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>

/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIRECD.C
*
* DESCRIPTIVE NAME: Waveform Record Routine.
*
* FUNCTION: Record into an Waveform File.
*
* NOTES:  hStream[2] = B --> A = Record stream.
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_PLAY message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  CreateNAssocStream ().
*                       ().
*                       OpenFile ().
*                       SetAudioDevice ().
*                       SetWaveDeviceDefaults ().
*                       AssocMemPlayToAudioStream ().
*
* EXTERNAL REFERENCES:  SpiStartStream ()    - SSM  Spi
*                       SpiStopStream  ()    - SSM  Spi
*
*********************** END OF SPECIFICATIONS **********************/
RC MCIRecd (FUNCTION_PARM_BLOCK *pFuncBlock)

{
  ULONG      ulrc;               // Propogated Error Code
  ULONG      ulParam1;           // Incoming MCI Flags
  ULONG      ulCnt;              // Sem Posting Count

  ULONG      ulAbortNotify = FALSE;


  INSTANCE   *ulpInstance;       // Local Instance

  BOOL       fInitNeeded;        // Must the card be reinited
  BOOL       fSeekNeeded = FALSE;// is a seek needed after a create

//  MCI_AMP_INSTANCE     BackupAmp;           // Hold old amp values

  LONG       rc;                 // thread it return

  /********************************
  * derefernce pointers
  *********************************/
  ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;
  ulParam1 = pFuncBlock->ulParam1;


  /*---------------------------------------------------------
  * Make a copy of the amp/mixer instance in case any errors
  * happened.
  *---------------------------------------------------------*/
// 6421--no more need of this
//  memmove( &BackupAmp, &AMPMIX, sizeof( MCI_AMP_INSTANCE ) );



  /*************************************************
  * Check to ensure that all of the flags are valid
  * memory is properly allocated and that the
  * requested action is possible (i.e. the from
  * position < to position, etc.)
  **************************************************/

  ulrc = CheckRecordFlags ( pFuncBlock );

  /*******************************************
  * If there are any errors, we must release
  * the access semaphore which will allow
  * other commands to process.  If we do not
  * release this semaphore, the next command
  * (i.e. MCI_PLAY, MCI_SET etc., will be
  * blocked waiting for it to be posted.
  *******************************************/

  if (ulrc)
     {
     return (ulrc);
     }


   /********************************************************
   * In order for a play to be possible, the straming MCD must
   * be connected to another MCD (i.e. the file's output must
   * go to another device.
   * Ensure that this is true.
   ********************************************************/

   CheckForConnection( ulpInstance );



  /****************************************
  * If no stream has been created, and the
  * card has not changed modes, there is no
  * need to reinit it!
  *****************************************/

  if ( ulpInstance->ulOperation == MCIDRV_OUTPUT  ||
       ulpInstance->StreamInfo.ulState == STREAM_SET_STATE )
     {
     fInitNeeded = TRUE;
     }

   /***********************************************
   * If all of the parameters are valid so far, then
   * obtain semaphore which allows us to abort or
   * supercede in progress commands.
   ************************************************/

   GetNotifyAbortAccess ( ulpInstance, &ulAbortNotify );

  /*******************************************
  * If there is an operation active (i.e. a
  * play, record or save) then post a message
  * stating that the command has been
  * aborted (play), superceded (record) or
  * wait for completion (save).
  ********************************************/

   if ( ulAbortNotify == TRUE)
      {
      ulrc = AbortInProgressNotify( ulpInstance, pFuncBlock, ulParam1, MCI_RECORD );
      }




  /**********************************************************************
  * Destroy the stream. If a play preceded, the direction of the stream is
  * reversed. The previous stream gets destroyed.
  **********************************************************************/

  if (ulpInstance->ulOperation == MCIDRV_OUTPUT)
      {
       /***********************************************
       * Since we are destroying an existing stream
       * retrieve the current stream position.  When
       * the record stream is created, we will
       * seek to this point.  Do this so that the
       * caller at the MCI Layer is ignorant of the
       * streaming operations.
       ***********************************************/


      GetOldStreamPosition( ulpInstance);
#ifndef CONNECTION
      DestroyStream ( ulpInstance);
      if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
         {
         ulpInstance->ulCodecDescription = MAKE_TARGET | TARGET_CODEC;
         DataTranslation( ulpInstance );
         }
#endif
      ulpInstance->ulCreateFlag = CREATE_STATE;




      if ( ulpInstance->ulOldStreamPos > 0 )
         {
         fSeekNeeded = TRUE;
         }
      }      /* Transition from PLAY State */

   /***********************************************
   * If a set was performed on an existing stream,
   * destroy the stream and get new spcb keys.  You
   * MUST recreate the stream after and MCI_SET has
   * been done since there is no way to communicate
   * to the stream handler the change in data rates
   * other than at spiCreate time.
   ***********************************************/

   DestroySetStream ( ulpInstance );

  /*********************************************************************
  * Create The Recording stream. This normally goes from audio stream
  * handler to the file system stream handler. Based on the associate
  * flag we can change the recording destination to File system or
  * memory (in case of playlist)
  *********************************************************************/

  if (ulpInstance->ulCreateFlag != PREROLL_STATE)

     {
     /*******************************
     * Do stream set up work and then
     * create the stream
     *******************************/

     ulrc = PrepareAndCreateStream( ulpInstance, MCIDRV_INPUT, fInitNeeded );

     if ( ulrc )
        {
        /* Ensure that our instance remains the same as before the load attempt */

        /* Truly--I have no idea why this code is here--6421 */

//        memmove( &AMPMIX, &BackupAmp, sizeof ( MCI_AMP_INSTANCE ) );

        return ( ulrc );
        }


     /***************************************************************
     * Set the stream up with the same position advises and cuepoints
     * as the previous stream had (if there any).
     ***************************************************************/

     ulrc = RememberStreamState( ulpInstance, ulParam1, fSeekNeeded );

     if ( ulrc )
        {
        return ( ulrc );
        }
     }  /* Create Flag != Preroll State */

  /**********************
  * Update State Flags
  ***********************/
  ulpInstance->ulCreateFlag = PREROLL_STATE;

  /*************************************************
  * Playlist currently do not support io procs, thus
  * they currently do not support MMIO calls.
  * Therefore, if we are currently using a playlist
  * then we cannot work with the audio header in the
  * file.
  **************************************************/

//  if (ulpInstance->usPlayLstStrm != TRUE)
//      {
//
//      /******************************************************************
//      * Before we start to record, set the header of the file via MMIO.
//      * This will allow applications which opened the file via OPEN_MMIO
//      * to allways have a correct view of what is in the file.
//      ******************************************************************/
//
//      ulrc = SetAudioHeader (ulpInstance);
//
//      } /* Non PlayList */


   /******************************************************************
   * Place the stream in the correct position if MCI_FROM is specified
   * and set the correct stopping point if MCI_TO is specfied
   ******************************************************************/

   ulrc = ProcessFromToFlags( pFuncBlock, ulpInstance, MCI_RECORD, ulParam1 );

   if ( ulrc )
      {
      return ( ulrc );
      }


  /***********************************************
  * If the caller requests that we record insert,
  * then let the IO Procedure know that we want
  * the record to be done via insert until we
  * turn this flag off (at the end of StartRecord)
  ************************************************/

  if (ulParam1 & MCI_RECORD_INSERT)
     {
     /* Not all IO Procs can insert, check capabilities first */

      if ( ulpInstance->ulCapabilities & CAN_INSERT )
         {
         /* Request record insert from mmio io proc */

         ulrc = mmioSendMessage( ulpInstance->hmmio,
                                 MMIOM_BEGININSERT,
                                 0,
                                 0);
         if (ulrc)
            {
            /* If an error, get a little more info via mmioGetLastError */

            ulrc = mmioGetLastError( ulpInstance->hmmio );

            return (ulrc);
            }

           /* Set flag indicating that record insert is active */

           ulpInstance->fRecdInsert = TRUE;

         } /* Current IO Proc can insert */
      else
         {

         return ( MCIERR_UNSUPPORTED_FLAG );

         } /* Current IO Proc cannot insert */

     } /* Record Insert flag sent */

  else if ( !ulpInstance->usPlayLstStrm )

     {
     ulrc = mmioSendMessage( ulpInstance->hmmio,
                             MMIOM_BEGINGROUP,
                             0,
                             0);
     if (ulrc)
        {
        ulrc = mmioGetLastError( ulpInstance->hmmio );

        return (ulrc);
        }


     } /* NORMAL RECORD */


  /***********************************************
  * Ensure that our avg. bytes per sec is accurate
  * since a best fit could have changed the field
  * on the fly.
  ***********************************************/

//  ulpInstance->ulAverageBytesPerSec = WAVEHDR.usChannels * WAVEHDR.ulSamplesPerSec * ( WAVEHDR.usBitsPerSample / 8 );
  ulpInstance->ulAverageBytesPerSec =
   ( ULONG ) ( WAVEHDR.ulSamplesPerSec * ( WAVEHDR.usBitsPerSample / 8 ) * WAVEHDR.usChannels );


  /****************************************
  * See if the network can support the
  * the file we are about to start streaming.
  *
  * ALL streaming MCD's should take advantage
  * of this MMIO api.
  *****************************************/

  ulrc = BeginQualityofService( ulpInstance, STREAM_WRITE );

  /*-----------------------------------------
  * If there is not a network io proc, we will
  * receive unsupported message.  In this case
  * we will ignore the error.
  *
  * If there is a network io proc we may have
  * to examine the error a little more carefully
  *--------------------------------------------*/

  if ( ulrc && ulrc != MMIOERR_UNSUPPORTED_MESSAGE )
     {
     /*--------------------------------------------
     * At open time, we retrieved a variable called
     * MSV_SYSQOSERRORFLAG which indicates if the
     * user wants to be notified if the streaming
     * operation will not be possible. It can
     * have the following values:
     *
     * A. ERROR_REPORT (report all errors).
     * B. ERROR_IGNORE (ignore all errors).
     *---------------------------------------------*/

     if ( ulpInstance->lQOSReporting == ERROR_REPORT )
        {
        /* Tell the caller of the network bandwidth problem */

        return ( ulrc );
        }
     } /* If there was a problem setting up network support */


  /************************************
  * Enable Position Advise if Needed
  *************************************/


  ulrc = EnableEvents( ulpInstance );



  if (!ulrc)
       /************************************************
       ** To determine whether or not an operation must be
       ** done on a thread, use the following criteria:
       **
       **   A. Will the operation take a long time.
       **   B. Is there any way to make it appear as if
       **      a thread was active.
       **
       ** We could do an spiStart, receive the event
       ** in our event proc and clean up on the SSM
       ** thread.  However, this thread will be a HIGH
       ** PRIORITY thread and we have too much processing
       ** to do.  Therefore, there is no way to fake it.
       ** Thus we must create a record thread if the notify
       ** flag is sent
       ************************************************/

      if (ulParam1 & MCI_NOTIFY)
         {
         ulpInstance->usNotifyPending = TRUE;
         ulpInstance->usNotPendingMsg = MCI_RECORD;

         DosResetEventSem (ulpInstance->hThreadSem, &ulCnt);

          /****************************************************
          * This thread is kicked off by the MCD mainly
          * to start the stream. When it is safe to continue
          * the play thread will post the thread semaphore and
          * this thread can continue.
          ****************************************************/

          rc = _beginthread ( (ADMCTHREAD) StartRecord,
                                 0,
                                 NOTIFY_THREAD_STACKSIZE,
                               (void *)pFuncBlock );

         /*************************************************
         * Wait for the record thread to indicate that it
         * is safe to continue.
         **************************************************/

           if ( rc != -1 )
              {
              DosWaitEventSem (ulpInstance->hThreadSem, -1);
              }
           else
              {
              /****************************************
              * Tell the network that we are done
              * streaming since an error occurred
              *****************************************/

              EndQualityofService( ulpInstance );

              ulrc = MCIERR_OUT_OF_MEMORY;
              }


         if ( rc > 0 )
            {
            DosWaitEventSem (ulpInstance->hThreadSem, -1);
            }
         }

      else
         {
         ulpInstance->usWaitPending = TRUE;
         ulpInstance->usWaitMsg= MCI_RECORD;

         /************************************************
         * Start record.  This function will kick
         * off the record, get an event (i.e. stream
         * complete, stopped etc.) and report the return
         * code.  We will return this return code to the
         * caller.
         ************************************************/


         ulrc = StartRecord (pFuncBlock);

         } /* MCI_WAIT flag was used */

  /**********************************
  * Release data Access Semaphore
  ***********************************/

   DosResetEventSem (ulpInstance->hThreadSem, &ulCnt);

  return (ulrc);
}



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: EventProc.
*
* DESCRIPTIVE NAME: SSM Event Handler
*
* FUNCTION:  Handle Streaming Event Notifications from SSM.
*
* NOTES: This routine is presumed to receive all types of event
*        notifications from the SSM. The types include Implicit
*        events, Cue point notifications in terms of both time
*        and data. In response to Cue point notifications an
*        MCI_CUEPOINT message is returned to MDM via mdmDriverNotify ()
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code and flError flag is set.
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*
* EXTERNAL REFERENCES: mdmDriverNotify ()  - MDM   API
*
*********************** END OF SPECIFICATIONS **********************/
RC APIENTRY RecordEventRoutine ( MEVCB  *pevcb)
{
  MTIME_EVCB        *pMTimeEVCB;    // Modified EVCB
  INSTANCE          *ulpInstance;   // Instance Ptr

  /***********************************************************
  * EventProc receives asynchronous SSM event notifications
  * When the event is received, the event semaphore is posted
  * which will wake up the MCD thread(s) blocked on this
  * semaphore.
  * The semaphore is not posted for time events like
  * cuepoint (TIME) and media position changes since they do
  * not alter the state of the stream.
  ************************************************************/


  switch (pevcb->evcb.ulType)
  {
  case EVENT_IMPLICIT_TYPE:

       /* Retrieve our instance from the EVCB */

       ulpInstance = (INSTANCE *)pevcb->ulpInstance;

       switch (pevcb->evcb.ulSubType)
       {
       // 6421--its possible for us to now get EOS (i.e. fssh->fssh ).
       case EVENT_EOS:
            ulpInstance->StreamEvent = EVENT_EOS;
            DosPostEventSem (ulpInstance->hEventSem);
           break;

       case EVENT_ERROR:
            ulpInstance->StreamEvent = EVENT_ERROR;

            /****************************************
            * Check for playlist specific error first
            *****************************************/

            /**************************************
            * End of PlayList event is received
            * as an implicit error event. It
            * is treated as a normal EOS
            ***************************************/
            if (ulpInstance->usPlayLstStrm == TRUE)
                if (pevcb->evcb.ulStatus == ERROR_END_OF_PLAYLIST)
                    ulpInstance->StreamInfo.Evcb.evcb.ulStatus = MMIOERR_CANNOTWRITE;

            DosPostEventSem (ulpInstance->hEventSem);
           break;

       case EVENT_STREAM_STOPPED:
            /****************************************
            * Event Stream Stopped. Release the
            * Blocked thread
            *****************************************/
            ulpInstance->StreamEvent = EVENT_STREAM_STOPPED;
            DosPostEventSem (ulpInstance->hEventSem);
           break;

       case EVENT_SYNC_PREROLLED:
            /******************************************
            * This event is received in reponse to a
            * preroll start. A Preroll start is done
            * on an MCI_CUE message.
            *******************************************/
            ulpInstance->StreamEvent = EVENT_SYNC_PREROLLED;
            DosPostEventSem (ulpInstance->hEventSem);
           break;

       case EVENT_PLAYLISTMESSAGE:
            /******************************************
            * We can receive this event if a playlist
            * parser hits the MESSAGE COMMAND.
            * NOTE: The MCD should return this message
            * with the callback handle specified on the
            * open.  This could be the source of much
            * grief if you return on the wrong handle.
            ******************************************/

            mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                              ulpInstance->hwndOpenCallBack,
                              MM_MCIPLAYLISTMESSAGE,
                              (USHORT) MAKEULONG(pevcb->evcb.ulStatus,
                                                 ulpInstance->usWaveDeviceID),
                              (ULONG)pevcb->evcb.unused1);


           break;

       /* Note: playlist cuepoints are not supported on record -- see docs */

       } /* SubType case of Implicit Events */
      break;

  case EVENT_CUE_TIME_PAUSE:
       /***************************************************
       * This event will arrive if we recorded to a certain
       * position in the stream.  Let the play thread know
       * that we have reached the desired point.
       ****************************************************/

      pMTimeEVCB = (MTIME_EVCB *)pevcb;
      ulpInstance = (INSTANCE *)pMTimeEVCB->ulpInstance;
      ulpInstance->StreamEvent = EVENT_CUE_TIME_PAUSE;

      DosPostEventSem (ulpInstance->hEventSem);
      break;
  case EVENT_CUE_TIME:

       /*************************************************
       * Single Events are Treated as Time Cue Points
       * Note: the caller is required to have a callback
       * specifically for this purpose which we stored in
       * our Time Event Control Block (EVCB).  See the
       * ADMCCUE.C file for more information on how to
       * manipulate your own EVCB and why you would want to
       **************************************************/

       pMTimeEVCB = (MTIME_EVCB *)pevcb;
       ulpInstance = (INSTANCE *)pMTimeEVCB->ulpInstance;

      if ( pMTimeEVCB->evcb.ulFlags == EVENT_SINGLE )
         {
         mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                           pMTimeEVCB->hwndCallback,
                           MM_MCICUEPOINT,
                           (USHORT) pMTimeEVCB->usCueUsrParm,
                           (ULONG) pMTimeEVCB->evcb.mmtimeStream);
         }

       /************************************************
       * Recurring events equate to position advise events
       * or media changed events.
       *
       * Note: the caller is required to have a callback
       * specifically for this purpose which we stored in
       * our Time Event Control Block (EVCB).  See the
       * ADMCCUE.C file for more information on how to
       * manipulate your own EVCB and why you would want to
       **************************************************/

      if ( pMTimeEVCB->evcb.ulFlags == EVENT_RECURRING )
         {
         mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                           ulpInstance->StreamInfo.PosAdvEvcb.hwndCallback,
                           MM_MCIPOSITIONCHANGE,
                           (USHORT) ulpInstance->usPosUserParm,
                           (ULONG)  pMTimeEVCB->evcb.mmtimeStream);
         }


      break;
#ifdef STATUS_LEVEL
  case EVENT_STATUS_LEVEL:
       StatusLevelEvent (ulpInstance, pevcb);
       break;
#endif
  default:
      break;

  }  /* All Events case */

  return (MCIERR_SUCCESS);

}              /* of Event Handler */





/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StartRecord.
*
* DESCRIPTIVE NAME:StartRecording
*
* FUNCTION: SpiStartStream().
*
*
* ENTRY POINTS:
*
* NOTES: This routine is called using caller' thread (MCI_WAIT)
*        or a separate thread spawned by MCD on MCI Notify.  Once the stream
*        is started, the event procedure above is called by SSM on a
*        high priority thread.  The event procedure will post a semaphore
*        awaking this function.
*
*        When a streaming operation is interuptted (usually by a stop)
*        the interuptting thread waits for the MCDs thread to complete
*        its remaing tasks. This wait is controlled via the instance based
*        thread semaphore.
*
*        Further, on a notify the Record Notify command does not return
*        until the newly created thread is ready to block itself. This
*        ensures that any other MCI messages are free to be processed
*        following the MCI_RECORD message operate on a running stream.
*        This also means there is minimum latency between the return of the
*        Record command to the application and start of audible sound.
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
* EXTERNAL REFERENCES: None
*
*********************** END OF SPECIFICATIONS *******************************/
RC _Optlink  StartRecord (VOID * pvoid)
{
  FUNCTION_PARM_BLOCK * pFuncBlockCopy = (FUNCTION_PARM_BLOCK *) pvoid;
  ULONG         ulrc;
  ULONG         ulCnt;
  ULONG         ulParam1;
  ULONG         ulErr;
//  ULONG         ulRecdToNotify;
  INSTANCE      *ulpInstance;

  FUNCTION_PARM_BLOCK  FuncBlock;

  BOOL          fPostMessage;      // should we inform the caller?

  memmove( &FuncBlock, pFuncBlockCopy, sizeof( FUNCTION_PARM_BLOCK ) );

   ulErr = MCI_NOTIFY_SUCCESSFUL;
   ulpInstance = (INSTANCE *) FuncBlock.ulpInstance;
   ulParam1 = FuncBlock.ulParam1;
   fPostMessage = TRUE;


  /**************************************
  * Reset the event semaphore used by the
  * RecordEventProc.  See comments below.
  ****************************************/

   DosResetEventSem (ulpInstance->hEventSem, &ulCnt);

   /* Update state to reflect the flact we are playing */

   ulpInstance->StreamInfo.ulState = MCI_RECORD;


  /****************************************
  * Set a flag so we can sense when someone
  * issues an aborted or superceded request
  *****************************************/

  ulpInstance->ulNotifyAborted = FALSE;



   /***********************
   * Start the stream
   ************************/
// CONNECTOR FEATURE
// ulrc = SpiStartStream (ulpInstance->StreamInfo.hStream, SPI_START_STREAM);
  ulrc = StartStream( ulpInstance, MCIDRV_START_RECORD );
// CONNECTOR FEATURE


  /*****************************************
  * If this is a notify thread, let the
  * caller know it is ok to continue
  *****************************************/

  if (ulParam1 & MCI_NOTIFY)
     {
     FuncBlock.pInstance->usUserParm = FuncBlock.usUserParm;
     DosPostEventSem (ulpInstance->hThreadSem);
     }

  /***********************************************
  * It is VERY important to check the error code
  * from the SpiStartStream before waiting on an
  * event from it.  If an error is returned and
  * we wait on an event from the event proc, we
  * will hang since an event will never come.
  ***********************************************/

   if ( !ulrc )
      {
      /**********************************************
      * Block this thread until an event happens in
      * the stream.  When it does, the PlayEventProc
      * will signal us via this semaphore.
      **********************************************/

      DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);

      /*******************************************************
      * Acquire semaphore which will prevent any thread from
      * aborting us.  Once we have this semaphore, enter a
      * critical section, update some key instance variables
      * and get out quickly.
      *******************************************************/

      DosRequestMutexSem( ulpInstance->hmtxNotifyAccess, -1 );

      /**********************************************
      * Let any other thread know that there no
      * longer is a command active which can be
      * aborted.  We are in the clean up stage now
      * and once we exit the critical section, other
      * commands can safely operate.
      ***********************************************/

      if (ulpInstance->usNotifyPending == TRUE)
          ulpInstance->usNotifyPending =FALSE;

      /**************************************
      * Ensure that if we are being aborted
      * by another thread, and we received
      * an event that we ignore the event
      * and let the other process post the
      * notify
      **************************************/

      if (ulpInstance->ulNotifyAborted == TRUE)
         {
         fPostMessage = FALSE;
         }

      /************************************************
      * We have set up the event proc to give us the
      * more detailed error in ulStatus if an EVENT
      * ERROR is returned.
      *************************************************/

      else if (ulpInstance->StreamEvent == EVENT_ERROR)
        {
        ulErr = ulpInstance->StreamInfo.Evcb.evcb.ulStatus;

        if (ulErr == MMIOERR_CANNOTWRITE)
           {
           ulErr = MCIERR_TARGET_DEVICE_FULL;
           }

        /***************************************************
        * Just because we have received an event error, it
        * does not mean that the stream is stopped.  If we
        * tried to seek in the current stream state, BAD
        * things will happen so stop the stream.
        ***************************************************/

        DosResetEventSem (ulpInstance->hEventSem, &ulCnt);

        /* An error happened, therefore do a discard--we're toast anyway */

        ulrc = ADMCStopStream (ulpInstance, SPI_STOP_DISCARD );

        if (!ulrc)
           {
           DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);
           }

         } /* if an event error happened */

#ifdef STATUS_LEVEL
      /* clear the status level */
      ulpInstance->ulStatusLevel = 0;               /* status_level */
#endif
      /************************************************
      * Store the fact that we were doing a record to
      * a certain point in the stream.  Other threads
      * may overwrite this variable so keep its value
      * while we are in the critical section.
      ************************************************/

      if (ulpInstance->fToEvent == TRUE)
         {
         ulpInstance->fToEvent = FALSE;

         /*************************************************
         * NOTE: it is very important that we disable this
         * event since following commands could receive a
         * bogus cuepoint when the stream by the to point.
         **************************************************/

         ulrc = ADMCDisableEvent(ulpInstance->StreamInfo.hPlayToEvent);

         DosResetEventSem (ulpInstance->hEventSem, &ulCnt);

         ulrc = ADMCStopStream (ulpInstance, SPI_STOP_FLUSH );

         if (!ulrc)
            {
            DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);
            }

         } /* Were we recording to a certain point */


      } /* if no error on starting a record */
   else  /* an error occurred on starting the stream */
      {
      /**********************************
      * Disable Notify Pending Flag
      ***********************************/

      if (ulpInstance->usNotifyPending == TRUE)
        ulpInstance->usNotifyPending =FALSE;

      /* Let the caller know things went VERY badly */

      if ( ulParam1 & MCI_NOTIFY )
         {
         PostMDMMessage (ulrc, MCI_RECORD, &FuncBlock);
         }

      return ( ulrc );
      }

  /******************************************************
  * Fix for MMPM/2 AVC--they do repeated open\closes and after
  * approximately 80 closes, due to some OS/2 scheduling
  * quirk, close would free the instance before the thread
  * finished processing.  Therefore, require close to
  * acquire the exclusive semaphore before freeing
  * instance.
  ********************************************************/

  DosRequestMutexSem( ulpInstance->hmtxCloseAccess, -1 );


   /* Ensure the file contains the information we just recorded */

   ulrc = UpdateFileHeader( ulpInstance );

   if ( ulrc )
      {
      /*************************************************
      * If an error previously occurred, we do not want
      * to overwrite this error.
      **************************************************/

      ulErr = ulrc;
      }


  /********************************************
  * After a record, we always do a full-fledged
  * stop, so update the stream state.
  *********************************************/
  ulpInstance->StreamInfo.ulState = MCI_STOP;

  if (ulpInstance->usPlayLstStrm != TRUE)
     {
     /****************************************
     * Tell the network that we are done
     * streaming.
     *****************************************/

     EndQualityofService( ulpInstance );


     /*------------------------------------------
     * Records can be un-done.  Update this fact
     * in our undo/redo level storage.
     *------------------------------------------*/
     AddNode( ulpInstance, 1 );
     ulpInstance->ulNumUndo++;
     }

  /****************************************
  * Post The Notification only after
  * File is saved in case of To
  *****************************************/

  if (ulpInstance->usWaitPending == TRUE)
     {
     ulpInstance->usWaitPending = FALSE;
     DosReleaseMutexSem( ulpInstance->hmtxNotifyAccess );
     DosReleaseMutexSem( ulpInstance->hmtxCloseAccess );

     return (ulErr);
     }


   /****************************************************
   * Inform the caller of the state of the stream. The
   * only scenario where we do not do this is when
   * another command (such as MCI_STOP ) aborts the
   * record ( in which case it is responsible for
   * posting the message itself.
   *****************************************************/

    if ( fPostMessage )
         {
         /*******************************************
         * Post The Notification Message for Record
         ********************************************/
         PostMDMMessage (ulErr, MCI_RECORD, &FuncBlock);
         }



   /**************************************************
   * Post The Semaphore to clear any Blocked threads
   **************************************************/
   DosPostEventSem (ulpInstance->hThreadSem);

   /***************************************
   * Allow other threads in the process to
   * continue and release exlcusive notify
   * semaphore.
   ***************************************/

   DosReleaseMutexSem( ulpInstance->hmtxNotifyAccess );

   DosReleaseMutexSem( ulpInstance->hmtxCloseAccess );

   return ( MCIERR_SUCCESS );

} /* StartRecord */

/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: UpdateFileHeader.
*
* DESCRIPTIVE NAME:
*
* FUNCTION: Updates the file header after a record
*
*
* ENTRY POINTS:
*
* NOTES: This function is responsible for terminating the logical
*        record action and updating the file header.
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
* EXTERNAL REFERENCES: None
*
*********************** END OF SPECIFICATIONS *******************************/



ULONG UpdateFileHeader ( INSTANCE    *ulpInstance )

{
ULONG ulrc;
LONG  lBytesRead;

   /***************************************************
   * Unlike playback, record modifies the length of the
   * file and the data in it, therefore we MUST update
   * let the io proc know that the opeartion is done.
   *
   * Note: This does not apply to playlist since it
   * operates on buffers rather than files and mmio
   * operations are not allowed on playlists.
   ****************************************************/

   if ( ulpInstance->usPlayLstStrm != TRUE )
      {
      /************************************************
      * If the RECORD_INSERT flag was used, tell the
      * io proc we are done inserting
      ************************************************/
      if ( ulpInstance->fRecdInsert )
        {

        ulrc = mmioSendMessage( ulpInstance->hmmio,
                                MMIOM_ENDINSERT,
                                0,
                                0);

        if (ulrc)
          {
          return ( mmioGetLastError( ulpInstance->hmmio ) );
          }

        /***************************************
        * Ensure that it no other record inserts
        * will be done unless specified
        ****************************************/

        ulpInstance->fRecdInsert = FALSE;

        }
      else

      /************************************************
      * Tell the io proc that the logical record is
      * complete.  This will allow the record to be
      * undone or redone.
      ***********************************************/

        {
        ulrc = mmioSendMessage( ulpInstance->hmmio,
                                MMIOM_ENDGROUP,
                                0,
                                0);

        if (ulrc)
          {
          return ( mmioGetLastError( ulpInstance->hmmio ) );
          }

        }  /* else we are not inserting */


      /***************************************************
      * Unlike playback, record modifies the length of the
      * file and the data in it, therefore we MUST update
      * the header.
      ****************************************************/

      if ( ulrc = SetAudioHeader( ulpInstance ) )
         {
         return ( ulrc );
         }


      /***********************************************
      * The io proc will update the size of the file
      * after we do a setheader, so do a getheader to
      * figure out what the new size of the file is
      ************************************************/
      ulrc = mmioGetHeader ( ulpInstance->hmmio,
                             (PVOID) &(ulpInstance->mmAudioHeader),
                             sizeof( MMAUDIOHEADER ),
                             &lBytesRead,
                             (ULONG) NULL,
                             (ULONG) NULL );

      if (ulrc != MMIO_SUCCESS )
         {
         return ( mmioGetLastError( ulpInstance->hmmio ) );
         }

      /* update our header size */

      ulpInstance->ulDataSize = XWAVHDR.ulAudioLengthInBytes;

      } /* update header if !playlist stream */

    return ( MCIERR_SUCCESS );

} /* UpdateFileHeader */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: CheckRecordFlags
*
* DESCRIPTIVE NAME: Process Error Conditions for  Record
*
* FUNCTION: Report Errors, Do PreProcessing for MCI Record
*
*
* ENTRY POINTS:
*
* NOTE: All MCI Functions should check to ensure that the flags are
*       valid before interrupting a previous operation.  This means
*       that if the caller supplied bad flags while a play was
*       operating, the play would not be interrupted.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: WaveIOPROC.
*
* EXTERNAL REFERENCES: DosQueryProcAddr - OS/2 API.
*
*********************** END OF SPECIFICATIONS *******************************/

RC CheckRecordFlags( FUNCTION_PARM_BLOCK *pFuncBlock )

{

  ULONG                ulrc;                  // RC
  ULONG                ulParam1;              // Incoming MCI Flags

  INSTANCE             *ulpInstance;          // Local Instance

  ULONG                ulRecordFlags;         // Mask for MCI Record
  ULONG                ulMMFileLength;        // Length of the File in MMTIME
  ULONG                ulTemp1;               // Scratch For Time Conversion
  ULONG                ulTempTO = 0;          // Scratch for Play Till
  ULONG                ulFromPosition = 0;    // holds where we will play from

  PMCI_RECORD_PARMS   pRecordParms;         // caller data struct



  /* Intialize local variables */

  ulParam1 = pFuncBlock->ulParam1;
  ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;



  ulRecordFlags = pFuncBlock->ulParam1;

  /****************************************
  * Mask out the flags we know about
  ****************************************/

  ulRecordFlags &= ~(MCI_FROM + MCI_TO + MCI_RECORD_INSERT +
                     MCI_RECORD_OVERWRITE + MCI_WAIT + MCI_NOTIFY);

  /* Return an error if there are any excess flags */

  if (ulRecordFlags > 0)
      return ( MCIERR_INVALID_FLAG );

  /***********************************************
  * Check for incompatible flags.
  ************************************************/

  if( (pFuncBlock->ulParam1 & MCI_RECORD_INSERT) &&
       (pFuncBlock->ulParam1 & MCI_RECORD_OVERWRITE))

        return ( MCIERR_FLAGS_NOT_COMPATIBLE );

  /************************************************
  * If neither overwrite or insert is specified,
  * then we will default to overwrite.
  * NOTE: this is in contrast to what the docs say
  * invesitigate.
  ************************************************/

  if (!(pFuncBlock->ulParam1 & MCI_RECORD_INSERT))
      pFuncBlock->ulParam1 |= MCI_RECORD_OVERWRITE;

  /***********************************************
  * If no element has been loaded, return an
  * error.
  ***********************************************/

  if ( !ulpInstance->fFileExists )
      return ( MCIERR_FILE_NOT_FOUND );

  /* The caller is required to pass record parms--ensure they are valid */

  pRecordParms= ( PMCI_RECORD_PARMS )pFuncBlock->ulParam2;

  ulrc = CheckMem ( (PVOID) pRecordParms,
                    sizeof (MCI_RECORD_PARMS), PAG_READ);


  if (ulrc != MCIERR_SUCCESS)
     {
     return MCIERR_MISSING_PARAMETER;
     }


//  if ( ulpInstance->fCanRecord & DEVICE_CAP_UNKNOWN )
//     {
//     /* init to 0 to save an if later */
//     MCI_GETDEVCAPS_PARMS  mcap;
//
//     ulpInstance->fCanRecord = FALSE;
//     mcap.ulItem = MCI_GETDEVCAPS_CAN_RECORD;
//
//     ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
//                             MCI_GETDEVCAPS,
//                             MCI_GETDEVCAPS_ITEM | MCI_WAIT,
//                             (PVOID) &mcap,
//                             pFuncBlock->usUserParm);
//
//     if ( ( ulrc & 0xffff) == MCIERR_SUCCESS )
//        {
//        ulpInstance->fCanRecord = mcap.ulReturn;
//        }
//     }

  /******************************************
  * If the io proc that we are currently
  * using does not support writing, return
  * an error (OpenFile shows how to determine
  * these capabilities).
  *******************************************/

// CONNECTION FEATURE
// 6421--Need to know if the HARDWARE can record--however, this info
// really is in the VSD--since the VSD is now off limits--how do I
// access it?????
//
// 1. Add a new flag to MCI_GETDEVCAPS saying another MCI driver is
// making the call??---barf :-(
// 2. Add new device specific parms (possible--but UGLY!!!!, this info
// is already in the resource file but I can't get at it!!!


// 6421--must receive device caps from mci_init structure

// should I use test connection in record mode?

// CONNECTION FEATURE --CanRecord is still undefined

  if ( ( !( ulpInstance->ulCapabilities & CAN_RECORD )  &&
         !ulpInstance->usPlayLstStrm ) ||
         CanPlayRecord( ulpInstance, MCIDRV_INPUT ) )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }

  if ( ulpInstance->usPlayLstStrm )
     {
     return ( MCIERR_SUCCESS );
     }


  ConvertTimeUnits ( ulpInstance,
                     &ulMMFileLength,
                     FILE_LENGTH);

  ulTemp1 = ulMMFileLength;

  /* what is this statement for? */

  ConvertToMM( ulpInstance, &ulMMFileLength, ulMMFileLength );


   /***********************************************
   * Unlike play processing, checking to see if
   * MCI_FROM on a record is valid is easy,
   * one just has to check to see if the FROM is
   * a valid position in the file.
   ***********************************************/

   if (ulParam1 & MCI_FROM)
      {
      /*********************************************
      * We cannot start recording from a position
      * which is greater than the length of the
      * file.
      *********************************************/
      // add code to avoid playlist problems.

      if ( pRecordParms->ulFrom > ulTemp1 )
        {
        return ( MCIERR_OUTOFRANGE );
        }

      /***********************************************
      * The record will start from the from position
      * If MCI_FROM is not specified, then the routines
      * below will assume the current file position for
      * the from.  Either way, convert the from position
      * to MM time and store it.
      ************************************************/

      ulrc = ConvertToMM ( ulpInstance,
                           (ULONG*) &ulFromPosition,
                           pRecordParms->ulFrom );

      }  /* Record From */

   /*****************************************************
   * If the MCI_TO flag is specified on a record, then
   * the following checks are required to ensure that
   * the to position is valid.
   *
   * A. The to position must be greater than 0.
   * B. If the from flag is specified, the to position
   *    must be greater than the from position.
   * C. If a stream has been created, then
   *    I. Ensure that the to position is not less than
   *       our current stream position.
   ******************************************************/

   if (ulParam1 & MCI_TO)
      {
      /* The to position must be greater than 0 */

      if ( pRecordParms->ulTo <= 0 )
        {
        return ( MCIERR_OUTOFRANGE );
        }

      if ( ulParam1 & MCI_FROM )
        {

        /* Recording behind the from position is illegal */

        if ( pRecordParms->ulTo <= pRecordParms->ulFrom )
           {
           return ( MCIERR_OUTOFRANGE );
           }

        } /* if the from flag specified */

      /* Check to see if a stream has been created */

      if ( ulpInstance->ulCreateFlag == PREROLL_STATE )
         {

         /**********************************************
         * Get the current stream time, it will be used
         * for range checking below.
         ***********************************************/

         ulrc = SpiGetTime( ulpInstance->StreamInfo.hStream,
                            ( PMMTIME ) &( ulpInstance->StreamInfo.mmStreamTime ) );

         if ( ulrc )
            {
            return ( ulrc );
            }

         /* Since the stream time is in mm time, convert the to point to MM time */

         ConvertToMM ( ulpInstance,
                       (ULONG*) &ulTempTO,
                       pRecordParms->ulTo );

         /*********************************************************
         * The documentation states that the record will start from
         * either the current position or the from point.
         * If the from flag was not passed in, then set the
         * from position to be equivalent to our current position
         * for simplicity purposes.
         * Note: ulFromPosition was filled in the from processing
         ********************************************************/

         if ( ulFromPosition == 0 &&
              !(ulParam1 & MCI_FROM ) )
            {
            ulFromPosition = ( ULONG ) ulpInstance->StreamInfo.mmStreamTime;
            }

         if ( ulTempTO <= ulpInstance->StreamInfo.mmStreamTime &&
              ulTempTO <= ulFromPosition         )
            {

            /********************************************************
            * it is possible that we had rounding problems so ensure
            * that user did indeed pass in an illegal value
            ********************************************************/
            if ( ( ulParam1 & MCI_FROM ) &&
                 !(pRecordParms->ulTo <= pRecordParms->ulFrom ) )
               {

               }
            else
               {
               return ( MCIERR_OUTOFRANGE );
               }
            }

         } /* if stream has been created */

      } /* MCI_TO was specified */

   /*******************************************************
   * Playlists to not support the record insert message to
   * io procs--so don't do it.
   *******************************************************/

   if (pFuncBlock->ulParam1 & MCI_RECORD_INSERT)
      {
      ulpInstance->fRecdInsert = TRUE;
      }   /* Insert Specified */

  return (MCIERR_SUCCESS);

} /* Check Record Flags */



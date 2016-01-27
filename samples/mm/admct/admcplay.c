/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCPLAY.C
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* DESCRIPTIVE NAME: Audio MCD WaveAudio Playback Source.
*
* FUNCTION:Play an Waveform Audio Element.
*
* On MCI_PLAY, a streaming MCD should perform the following commands:
*
* Always check flags and validate memory first.  This way if the flags
*  are invalid, the previous command will not be interrupted.
* If there is a command active on another thread (i.e. and play, record
*  or save), then either abort (record or save) or superced (play) by
*  stopping the stream and sending a message to the caller.
* If the stream is going the wrong way (e.g. it is setup for recording)
*  then destroy the stream.
* If no stream has been created, then create one.  If the stream handler
*  needs to associate a data object, do it here.
* If we destroyed a recording stream before creating the play back stream,
*  ensure that playback stream has the same position as the previous record
*  stream.
* Enable any events (such as cuepoints or position advises).
* Set up networking hooks,
* Start stream.
* Wait for a streaming event.
* Stop the stream if necessary.
* If MCI_NOTIFY was sent used, inform the caller of command completion.
*
* NOTES: This source file illustrates the following concepts:
*         A. Aborting or superceding a previous command
*         B. Transitioning from a record stream->playback stream
*            so that the caller at the MCI Layer is unaware of the
*            change.
*         C. Processing a stream which was altered by the MCI_SET
*            command.
*         D. Proper creation of a stream.
*         E. Processing the MCI_FROM/MCI_TO flags in a streaming
*            environment.
*         F. Enable both cuepoints and position advises.
*         G. How to use MMIO networking features.
*         H. Determing when the creation of a thread is necessary
*            for an MCI_NOTIFY
*         I. Proper handling of an event procedure.
*         J. Proper termination of a playback thread.
*
* ENTRY POINTS:
*
* INPUT: MCI_PLAY message.
*        MCI_FROM Flag
*        MCI_TO   Flag
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
*
* INTERNAL REFERENCES:   CreateNAssocStream ().
*                        DestroyStream ().
*                        AssocMemPlayToAudioStream ().
*                        PostMDMMessage ().
*                        CreateToEvent().
*                        ConvertTimeUnits ().
*                        StartPlay().
*                        PlEventProc().
*                        SetAudioDevice().
*                        SetWaveDeviceDefaults().
*                        OpenFile().
*                        CheckMem ().
*
* EXTERNAL REFERENCES:   DosResetEventSem ()        - OS/2 API
*                        DosPostEventSem  ()        - OS/2 API
*                        DosCreateThread  ()        - OS/2 API
*                        SpiEnableEvent   ()        - MME API
*                        SpiStopStream    ()        - MME API
*                        SpiCreateStream  ()        - MME API
*                        SpiAssociate     ()        - MME API
*                        SpiSeekStream    ()        - MME API
*                        mdmDriverNotify  ()        - MME API
*                        mmioGetHeader    ()        - MME API
*
*********************** END OF SPECIFICATIONS **********************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // Mci Driver Include.
#include <mcd.h>                        // VSDIDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: CheckPlayFlags
*
* DESCRIPTIVE NAME: Ensures that the play flags are valid.Record
*
* FUNCTION: Report errors, do preprocessing for MCI play
*
* ENTRY POINTS:
*
* NOTE: All MCI Functions should check to ensure that the flags are
*       valid before interrupting a previous operation.  This means
*       that if the caller supplied bad flags while a play was
*       operating, the play would not be interrupted.
*
* EXIT-NORMAL: Return Code 0.
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

RC CheckPlayFlags ( FUNCTION_PARM_BLOCK *pFuncBlock )

{

  ULONG                ulrc;                  // RC
  ULONG                ulParam1;              // Incoming MCI Flags

  INSTANCE             *ulpInstance;          // Local Instance

  ULONG                ulPlayFlags;           // Mask for MCI Play
  ULONG                ulFileLength;          // Length of the File in MMTIME
  ULONG                ulTemp1 = 0;           // Scratch For Time Conversion
  ULONG                ulTempTO = 0;          // Scratch for Play Till
  ULONG                ulFromPosition = 0;    // holds where we will play from

  PMCI_PLAY_PARMS     pPlayParms;           // caller data struct



  /* Intialize local variables */

  ulParam1 = pFuncBlock->ulParam1;
  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

      /*********************************
       Check For Illegal Flags
      **********************************/

      ulPlayFlags = pFuncBlock->ulParam1;
      ulPlayFlags &= ~(MCI_FROM + MCI_TO + MCI_NOTIFY + MCI_WAIT);

      /* If the caller passed in an  unsupported flag--return an error */

      if (ulPlayFlags > 0)
          return ( MCIERR_INVALID_FLAG );

      /*******************************************
      * Check to see If A Valid Element Specified
      *******************************************/

      if (ulpInstance->fFileExists != TRUE)
        return ( MCIERR_FILE_NOT_FOUND );

      /***********************************
      * Retrieve the callers play parms
      ***********************************/

      pPlayParms = ( PMCI_PLAY_PARMS )pFuncBlock->ulParam2;


      /****************************************
      * The caller is required to pass in
      * valid play parms if the MCI_TO, MCI_FROM
      * or if it is a notify.
      *****************************************/

      if ((pFuncBlock->ulParam1 & MCI_TO) ||
          (pFuncBlock->ulParam1 & MCI_FROM) ||
          (pFuncBlock->ulNotify))
          {
          ulrc = CheckMem ((PVOID)pPlayParms,
                           sizeof (MCI_PLAY_PARMS), PAG_READ);


          if (ulrc != MCIERR_SUCCESS)
              return ( MCIERR_MISSING_PARAMETER );

          } /* if play parms were passed in */


      if (ulpInstance->usPlayLstStrm == TRUE)
         {
         return ( MCIERR_SUCCESS );
         }


      /*********************************************
      * In order to determine if the play request
      * is valid, we must retrieve the length of
      * the file.
      * Note: the length of the file is returned in
      * the time units we are using currently.
      *********************************************/

      ConvertTimeUnits ( ulpInstance,
                         &ulFileLength,
                         FILE_LENGTH);

      ulTemp1 = ulFileLength;

      /* Convert file length to mmtime for stream comparisons later */

      ConvertToMM( ulpInstance, &ulFileLength, ulFileLength );


      /*****************************************************
      * To ensure that the MCI_FROM is valid, we need to
      * do the following checks:
      * If we currently have a playback stream then:
      *   A. Is the from greater than the file length?
      *
      * If a record stream is active then:
      *   A. If it has not been started, then is it
      *      shorter than the file length.
      *   A. Else it must be less than both the file
      *      length AND what we have recorded so far.
      ******************************************************/

      if (ulParam1 & MCI_FROM)
         {
         /*******************************************
         * If we have a playback stream and the
         * request is not less than the file length
         * then return an outofrange error.
         *******************************************/

         if ( pPlayParms->ulFrom > ulTemp1
              && ulpInstance->ulOperation != MCIDRV_INPUT )
            {
            return ( MCIERR_OUTOFRANGE );
            }
         else
            {
            /***************************************
            * This is a record stream, check if
            * a stream been created and started
            * by a play/record or cue?
            * If not, just ensure that the request
            * is within the limits of the file.
            ****************************************/

             if ( ulpInstance->ulCreateFlag != PREROLL_STATE )
                {
                if ( pPlayParms->ulFrom > ulTemp1 )
                  {
                  return ( MCIERR_OUTOFRANGE );
                  }

                } /* if not in preroll state */
             else

                /*******************************************************
                * We have a record stream which is active.  This is
                * special case since record can be continually updating
                * the file length.  Therefore we must assure that
                * the request is less than what has been recorded and
                * the file length.
                *******************************************************/

                {

                /* Determine where we are in the stream */

                ulrc = SpiGetTime( ulpInstance->StreamInfo.hStream,
                                   ( PMMTIME ) &( ulpInstance->StreamInfo.mmStreamTime ) );

                if ( ulrc )
                  {
                  return ( ulrc );
                  }

                if ( ulpInstance->ulOperation == MCIDRV_INPUT )
                   {

                   /*******************************************
                   * Convert the from request to mmtime so we
                   * can compare it with the stream time.
                   * The new from time will be place in ulTemp1
                   ********************************************/

                   ulrc = ConvertToMM ( ulpInstance,
                                        (PULONG) &ulTemp1,
                                        pPlayParms->ulFrom );

                   /************************************
                   * Our to point must be less than
                   * what we have recorded so far, and it
                   * must be less than the length of the
                   * file!
                   **************************************/

                   if ( ulTemp1 > (ULONG) ulpInstance->StreamInfo.mmStreamTime &&
                        ulTemp1 > ulFileLength )
                     {
                     return ( MCIERR_OUTOFRANGE);
                     }
                   } /* If the card is in record mode */

                else
                  {

                  /**********************************************************
                  * Save an mmtime version of the from position, so that the
                  * the MCI_TO checks below will ensure that to to position
                  * is valid.
                  **********************************************************/

                   ulrc = ConvertToMM ( ulpInstance,
                                        &ulFromPosition,
                                        pPlayParms->ulFrom );
                  }

                } /* If the stream is in a prerolled stated--i.e. buffers full */

            } /* else from < length && ! in record mode */

         } /* Play From flag was specified */

      /************************************************************
      * If the caller specified the MCI_TO flag, then the following
      * checks will be required:
      *
      * If we currently have a playback stream then:
      *   A. Is the to position greater than the file length?
      *   B. Is the to position greater than the from postion
      *      (if and only if) MCI_FROM was specified.
      *
      * If a record stream is active then:
      *   A. If it has not been started, then the to point
      *      must not be 0 and must be greater than the from.
      *   A. Else it must be less than both the file
      *      length AND what we have recorded so far.
      ************************************************************/

      if (ulParam1 & MCI_TO)
         {
         /***************************************
         * If the to position is greater than
         * the file length (using the currently
         * specfied time format) return an error.
         ****************************************/

         if ( pPlayParms->ulTo > ulTemp1 &&
              ulpInstance->ulOperation != MCIDRV_INPUT)
            {
            return ( MCIERR_OUTOFRANGE );
            }
         else
            {

            /* If to position is <= from position--it's an error */

            if ( ulParam1 & MCI_FROM )
               {
               if ( pPlayParms->ulTo <= pPlayParms->ulFrom )
                  {
                  return ( MCIERR_OUTOFRANGE );
                  }

               } /* if from flag was specified */

            /****************************************
            * Was the stream previously started by a
            * record, cue or play?
            *****************************************/

            if ( ulpInstance->ulCreateFlag != PREROLL_STATE )
               {

               /* As long as they don't want to play to 0 -- we are ok */

               if ( pPlayParms->ulTo == 0 )
                 {
                 return ( MCIERR_OUTOFRANGE );
                 }

               return ( MCIERR_SUCCESS );

               } /* Stream not in preroll state */

            /*****************************************
            * Determine our current stream time--use
            * as a reference point for future checks.
            ******************************************/

            ulrc = SpiGetTime( ulpInstance->StreamInfo.hStream,
                               ( PMMTIME ) &( ulpInstance->StreamInfo.mmStreamTime ) );

            if ( ulrc )
               {
               return ( ulrc );
               }
            /*********************************************************
            * The documentation states that the play will start from
            * either the current position or the from point.
            * If the from flag was not passed in, then set the
            * from position to be equivalent to our current position
            * for simplicity purposes.
            * Note: ulFromPosition was filled in the from processing
            *********************************************************/

            if ( ulFromPosition == 0 &&
                 !(ulParam1 & MCI_FROM ) )
               {
               ulFromPosition = ( ULONG ) ulpInstance->StreamInfo.mmStreamTime;
               }


            if ( ulpInstance->ulOperation == MCIDRV_INPUT )
               {
               /*******************************************
               * Convert the to request to mmtime so we
               * can compare it with the stream time.
               * The new TO time will be place in ulTempTO
               ********************************************/

               ulrc = ConvertToMM ( ulpInstance,
                                    &ulTempTO,
                                    pPlayParms->ulTo);

               /******************************************************
               * If we are past what we have recorded so far and past
               * the length of the file (in case of recording in the
               * middle or start of the file), then return an error
               *******************************************************/

               if ( ulTempTO > (ULONG ) ulpInstance->StreamInfo.mmStreamTime &&
                    ulTempTO > ulFileLength )
                 {
                 return ( MCIERR_OUTOFRANGE );
                 }
               } /*  if we are in record mode */

            else
               {
               /*******************************************
               * Convert the to request to mmtime so we
               * can compare it with the stream time.
               * The new TO time will be place in ulTempTO
               ********************************************/

               ulrc = ConvertToMM ( ulpInstance,
                                    &ulTempTO,
                                    pPlayParms->ulTo);

               /************************************************
               * We cannot play to a point which is behind our
               * current position or to a point which is less
               * than our from position.
               ************************************************/

               if ( ulTempTO <= ( ULONG ) ulpInstance->StreamInfo.mmStreamTime &&
                    ulTempTO <= ulFromPosition )
                 {
                 /********************************************************
                 * it is possible that we had rounding problems so ensure
                 * that user did indeed pass in an illegal value
                 ********************************************************/

                 if ( ( ulParam1 & MCI_FROM ) &&
                      !(pPlayParms->ulTo <= pPlayParms->ulFrom ) )
                    {
                    }
                 else
                    {
                    return ( MCIERR_OUTOFRANGE );
                    }

                 }
               }

            } /* else to < file length */

         } // Of Play Till XXXX



  return (MCIERR_SUCCESS);

} /* Check play flags */




/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIPlay
*
* DESCRIPTIVE NAME: Waveform Play Routine.
*
* FUNCTION: Play an Waveform File.
*
* NOTES:  hStream[1] = A --> B = Playback stream.
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
* INTERNAL REFERENCES:   ADMCERR (). CreateNAssocStream ().
*
* EXTERNAL REFERENCES:  spiStartStream ()    - SSM  Spi
*                       spiStopStream  ()    - SSM  Spi
*
*********************** END OF SPECIFICATIONS **********************/
RC MCIPlay (FUNCTION_PARM_BLOCK *pFuncBlock)

{

   ULONG         ulParam1;           // Flags for this Msg
   ULONG         ulrc;               // Error Code Propogated
   ULONG         lCnt;               // Number Of Posts
   ULONG         ulAbortNotify  = FALSE;    // whether or not to abort notify's

   INSTANCE      *ulpInstance;       // Local Instance

   BOOL          fInitNeeded = FALSE;// must we reinit the card?
   BOOL          fSeekNeeded = FALSE;// is a seek needed after a create
   LONG          rc;                 // thread id return code

   /**************************************
   * Intialize Variables on Stack
   **************************************/
   ulParam1 = pFuncBlock->ulParam1;

   /**************************************
   * Derefernce Pointers
   **************************************/
   ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;


   /*************************************************
   * Check to ensure that all of the flags are valid
   * memory is properly allocated and that the
   * requested action is possible (i.e. the from
   * position < to position etc.)
   **************************************************/

   ulrc = CheckPlayFlags ( pFuncBlock );

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


// CONNECTION FEATURE--is this needed anymore???

   /****************************************
   * If no stream has been created, and the
   * card has not changed modes, there is no
   * need to reinit it!
   *****************************************/

   if ( ulpInstance->ulOperation == MCIDRV_INPUT  ||
// CONNNECTION FEATURE
        ulpInstance->StreamInfo.ulState == STREAM_SET_STATE )
// CONNNECTION FEATURE
      {
      fInitNeeded = TRUE;
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
   * aborted (record), superceded (play) or
   * wait for completion (save).
   ********************************************/

   if ( ulAbortNotify == TRUE)
      {
      ulrc = AbortInProgressNotify( ulpInstance, pFuncBlock, ulParam1, MCI_PLAY );
      }


   /***************************************************
   * If the audio card and stream are setup for
   * recording, then retrieve the old stream position,
   * destroy the stream, and set a flag to indicate that
   * the stream must be created for playback.
   ****************************************************/

   if (ulpInstance->ulOperation == MCIDRV_INPUT)
       {

       /***********************************************
       * Since we are destroying an existing stream
       * retrieve the current stream position.  WHen
       * the playback stream is created, we will
       * seek to this point.  Do this so that the
       * caller at the MCI Layer is ignorant of the
       * streaming operations.
       ***********************************************/

       GetOldStreamPosition( ulpInstance );

// CONNECTOR FEATURE
#ifndef CONNECTION
       DestroyStream ( ulpInstance);

       if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
          {
          ulpInstance->ulCodecDescription = MAKE_SOURCE | SOURCE_CODEC;
          DataTranslation( ulpInstance );
          }


#endif
// CONNECTOR FEATURE
       if ( ulpInstance->ulOldStreamPos > 0 )
          {
          fSeekNeeded = TRUE;
          }
       ulpInstance->ulCreateFlag = CREATE_STATE;

       }      /* Transition from Recd State */



   /***********************************************
   * If a set was performed on an existing stream,
   * destroy the stream and get new spcb keys.  You
   * MUST recreate the stream after and MCI_SET has
   * been done since there is no way to communicate
   * to the stream handler the change in data rates
   * other than at spiCreate time.
   ***********************************************/

   DestroySetStream ( ulpInstance );

   /* Check to see if a stream has been created */

   if (ulpInstance->ulCreateFlag != PREROLL_STATE)
       {

       /*******************************
       * Do stream set up work and then
       * create the stream
       *******************************/

       ulrc = PrepareAndCreateStream( ulpInstance, MCIDRV_OUTPUT, fInitNeeded );

       if ( ulrc )
          {
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

       }  /* PreRoll State */

   /***************************
   * Set a flag which indicates
   * that a stream has been
   * created.
   ****************************/
   ulpInstance->ulCreateFlag = PREROLL_STATE;

   /******************************************************************
   * Place the stream in the correct position if MCI_FROM is specified
   * and set the correct stopping point if MCI_TO is specfied
   ******************************************************************/

   ulrc = ProcessFromToFlags( pFuncBlock, ulpInstance, MCI_PLAY, ulParam1 );

   if ( ulrc )
      {
      return ( ulrc );
      }


  /****************************************
  * See if the network can support the
  * the file we are about to start streaming.
  *****************************************/

  ulrc = BeginQualityofService( ulpInstance, STREAM_READ );

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


   /***************************************
   * Enable Position Advise if Needed
   ****************************************/

   ulrc = EnableEvents( ulpInstance );


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
   ** Thus we must create a play thread if the notify
   ** flag is sent
   ************************************************/

   if (!ulrc)
       {

       if (ulParam1 & MCI_NOTIFY)
          {
           ulpInstance->usNotifyPending = TRUE;
           ulpInstance->usNotPendingMsg = MCI_PLAY;

          /****************************************************
          * This thread is kicked off by the MCD mainly
          * to start the stream. When it is safe to continue
          * the play thread will post the thread semaphore and
          * this thread can continue.
          ****************************************************/

           DosResetEventSem (ulpInstance->hThreadSem, &lCnt);

           /* Note: I think this is too large */

           rc = _beginthread ( (ADMCTHREAD)StartPlay,
                               0,
                               NOTIFY_THREAD_STACKSIZE,
                               (void *)pFuncBlock );

           /*************************************************
           * Wait for the play thread to indicate that it is
           * safe to continue.
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
          }

       /* The caller used MCI_WAIT, so do everything on one thread */

       else
          {
          ulpInstance->usWaitPending = TRUE;
          ulpInstance->usWaitMsg = MCI_PLAY;

          /************************************************
          * Start playback.  This function will kick
          * off the play, get an event (i.e. stream
          * complete, stopped etc.) and report the return
          * code.  We will return this return code to the
          * caller.
          ************************************************/

          ulrc = StartPlay (pFuncBlock);
          }

       } /* If no errors so far */

   DosResetEventSem (ulpInstance->hThreadSem, &lCnt);


   return (ulrc);
}




/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: EventProc.
*
* DESCRIPTIVE NAME: SSM Event Notifications Receiever.
*
* FUNCTION:  Handle Streaming Event Notifications from SSM.
*
* NOTES: This routine is presumed to receive all types of event
*        notifications from SSM. The types include Implicit
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


RC APIENTRY PlayEventRoutine ( MEVCB     *pevcb)
{
  MTIME_EVCB        *pMTimeEVCB;      // Modified Time EVCB
  INSTANCE          * ulpInstance;    // Current Instance
  HWND              hWnd;             // CallBack Handle

  BOOL              fPlayListDone = FALSE;

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

       /* Retrieve the callback handle to post messages on */

       hWnd = ulpInstance->hwndCallBack;

       switch (pevcb->evcb.ulSubType)
       {
       case EVENT_EOS:
            ulpInstance->StreamEvent = EVENT_EOS;
            DosPostEventSem (ulpInstance->hEventSem);
           break;

       case EVENT_ERROR:


            /****************************************
            * Check for playlist specific error first
            *****************************************/

            if (ulpInstance->usPlayLstStrm == TRUE)
               {
               if ( pevcb->evcb.ulStatus == MCIERR_CUEPOINT_LIMIT_REACHED )
                  {
                  mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                                    hWnd,
                                    MM_MCINOTIFY,
                                    (USHORT )pevcb->evcb.unused1,
                                    MAKEULONG(MCI_PLAY, pevcb->evcb.ulStatus));
                  }
               fPlayListDone = TRUE;
               }

            /********************************************
            * Because the target stream handler is able
            * to recover from underruns, we will let the
            * stream continue, other error are lethal, so
            * alert the waiting thread that something has
            * seriously gone wrong.
            *********************************************/

            if (pevcb->evcb.ulStatus != ERROR_DEVICE_UNDERRUN)
               {
               ulpInstance->StreamEvent = EVENT_ERROR;
               DosPostEventSem (ulpInstance->hEventSem);
               }
            else if ( !fPlayListDone )
               {
               mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                                 hWnd,
                                 MM_MCINOTIFY,
                                 (USHORT) pevcb->evcb.unused1,
                                 MAKEULONG(MCI_PLAY, pevcb->evcb.ulStatus));
               }

           break;

       case EVENT_STREAM_STOPPED:
            /* Self explanatory--someone stopped the stream */

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
                              (USHORT) MAKEULONG(pevcb->evcb.ulStatus, ulpInstance->usWaveDeviceID),
                              (ULONG) pevcb->evcb.unused1);
           break;

       case EVENT_PLAYLISTCUEPOINT:

            /************************************************
            * We can receive this event if a playlist
            * parser hits the CUEPOINT COMMAND opcode
            * in the playlist.  This differs from a "normal"
            * cuepoint because it is detected by the source,
            * rather than the target stream handler.
            ************************************************/


            mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                              ulpInstance->hwndOpenCallBack,
                              MM_MCICUEPOINT,
                              (USHORT) MAKEULONG(pevcb->evcb.ulStatus, ulpInstance->usWaveDeviceID),
                              (ULONG) pevcb->evcb.unused1);
           break;


       } /* SubType case of Implicit Events */
      break;

  case EVENT_CUE_TIME_PAUSE:
       {
       /***************************************************
       * This event will arrive if we played to a certain
       * position in the stream.  Let the play thread know
       * that we have reached the desired point.
       ****************************************************/

       pMTimeEVCB = (MTIME_EVCB *)pevcb;
       ulpInstance = (INSTANCE *)pMTimeEVCB->ulpInstance;
       ulpInstance->StreamEvent = EVENT_CUE_TIME_PAUSE;

       DosPostEventSem (ulpInstance->hEventSem);
       }
       break;

  case EVENT_CUE_TIME:
       {
       pMTimeEVCB  = (MTIME_EVCB *)pevcb;
       ulpInstance = (INSTANCE *)pMTimeEVCB->ulpInstance;

       /*************************************************
       * Single Events are Treated as Time Cue Points
       * Note: the caller is required to have a callback
       * specifically for this purpose which we stored in
       * our Time Event Control Block (EVCB).  See the
       * ADMCCUE.C file for more information on how to
       * manipulate your own EVCB and why you would want to
       **************************************************/

       if ( pMTimeEVCB->evcb.ulFlags == EVENT_SINGLE)
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

       if (pMTimeEVCB->evcb.ulFlags == EVENT_RECURRING)
          {
          mdmDriverNotify ( ulpInstance->usWaveDeviceID,
                            ulpInstance->StreamInfo.PosAdvEvcb.hwndCallback,
                            MM_MCIPOSITIONCHANGE,
                            (USHORT) ulpInstance->usPosUserParm,
                            (ULONG) pMTimeEVCB->evcb.mmtimeStream);
          } /* Event Cue Time */

       }
       break;
#ifdef STATUS_LEVEL
  case EVENT_STATUS_LEVEL:
       StatusLevelEvent (ulpInstance, pevcb);
       break;

#endif
  } /* All Events case */

  return (MCIERR_SUCCESS);

} /* PlayEventProc */




/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StartPlay
*
* DESCRIPTIVE NAME:Start Play
*
* FUNCTION: Start the playback, wait till it is over and finish processing.
*           If MCI_NOTIFY is used, also post a message.
*
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
*        Further, on a notify the Play Notify command does not return
*        until the newly created thread is ready to block itself. This
*        ensures that any other MCI messages that are free to be intercepted
*        following the MCI_PLAY message operate on a running stream.
*        This also means there is minimum latency between the return of the
*        Play command to the application and start of audible sound.
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
* INTERNAL REFERENCES: None
*
* EXTERNAL REFERENCES: None
*
*********************** END OF SPECIFICATIONS *******************************/

RC _Optlink  StartPlay (VOID * pvoid)
{

  ULONG         ulrc;
  ULONG         lCnt;
  ULONG         ulParam1;


  INSTANCE      *ulpInstance;
  ULONG         ulErr;
  FUNCTION_PARM_BLOCK * pFuncBlockCopy = (FUNCTION_PARM_BLOCK *) pvoid;
  FUNCTION_PARM_BLOCK  FuncBlock;

  BOOL          fPostMessage;       // should the caller be informed
                                    // about the stream state


  memmove( &FuncBlock, pFuncBlockCopy, sizeof( FUNCTION_PARM_BLOCK ) );

  ulrc = MCIERR_SUCCESS;           // is this assignment needed?
  ulErr = MCI_NOTIFY_SUCCESSFUL;
  ulpInstance = (INSTANCE *) FuncBlock.pInstance;
  ulParam1 = FuncBlock.ulParam1;
  fPostMessage = TRUE;

  /**************************************
  * Reset the event semaphore used by the
  * PlayEventProc.  See comments below.
  ****************************************/

  DosResetEventSem (ulpInstance->hEventSem, &lCnt);


  /* Update state to reflect the flact we are playing */

  ulpInstance->StreamInfo.ulState = MCI_PLAY;

  /****************************************
  * Set a flag so we can sense when someone
  * issues an aborted or superceded request
  *****************************************/

  ulpInstance->ulNotifyAborted = FALSE;


  /****************************
  * Start Playing the Stream.
  *****************************/
// CONNECTOR FEATURE
//ulrc = SpiStartStream (ulpInstance->StreamInfo.hStream, SPI_START_STREAM);
  ulrc = StartStream( ulpInstance, MCIDRV_START_PLAYBACK );
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

  if ( ulrc )
     {
     /**********************************
     * Disable Notify Pending Flag
     ***********************************/

     if (ulpInstance->usNotifyPending == TRUE)
        {
        ulpInstance->usNotifyPending =FALSE;
        }

     /* Let the caller know things went VERY badly */

     if ( ulParam1 & MCI_NOTIFY )
        {
        PostMDMMessage (ulrc, MCI_PLAY, &FuncBlock);
        }

     return ( ulrc );
     }



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


  /* End of Stream (EOS) is good--the play is complete */

  if (ulpInstance->StreamEvent == EVENT_EOS)
     {
     ulErr = MCI_NOTIFY_SUCCESSFUL;
     }

  /************************************************
  * We have set up the event proc to give us the
  * more detailed error in ulStatus if an EVENT
  * ERROR is returned.
  *************************************************/

  else if (ulpInstance->StreamEvent == EVENT_ERROR)
     {
     ulErr = ulpInstance->StreamInfo.Evcb.evcb.ulStatus;
     DosResetEventSem (ulpInstance->hEventSem, &lCnt);

     /*********************************************
     * Because we are stopping the stream, we must
     * save the stream event that happened before
     * the stop, otherwise, the stopped event will
     * overwrite the old event and we will report
     * the wrong event via MDMDriverNotify
     **********************************************/

     ulrc = ADMCStopStream (ulpInstance, SPI_STOP_DISCARD );

     if (!ulrc)
       {
       DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);
       }

     }



  /************************************************
  * Store the fact that we were doing a play to
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


     ADMCDisableEvent(ulpInstance->StreamInfo.hPlayToEvent);

     /***************************************************
     * Just because we have received the cue time event
     * does not mean that the stream is stopped.  If we
     * tried to seek in the current stream state, BAD
     * things will happen so stop the stream.
     ***************************************************/

     ADMCStopStream (ulpInstance, SPI_STOP_STREAM );
     }
#ifdef STATUS_LEVEL
  /* clear the status level */
  ulpInstance->ulStatusLevel = 0;               /* status_level */
#endif

  /******************************************************
  * Fix for MMPM/2 AVC--they do repeated close and after
  * approximately 80 closes, due to some OS/2 scheduling
  * quirk, close would free the instance before the thread
  * finished processing.  Therefore, require close to
  * acquire the exclusive semaphore before freeing
  * instance.
  ********************************************************/

  DosRequestMutexSem( ulpInstance->hmtxCloseAccess, -1 );


  /*******************************************************
  * Play back stream will perform an spi stop_pause
  * to ensure that if someone requested another playback
  * no information would be lost.  If you do a stop
  * discard/flush, the next playback may lose several
  * bytes of information.  Most people won't hear it,
  * but some may.
  *******************************************************/

  ulpInstance->StreamInfo.ulState = STOP_PAUSED;

  /****************************************
  * Tell the network that we are done
  * streaming.
  *****************************************/

  EndQualityofService( ulpInstance );


  /*********************************************
  * If the caller used MCI_WAIT, then return
  * without sending a message via MDMDriverNotify
  *********************************************/

  if (ulpInstance->usWaitPending == TRUE)
     {
     ulpInstance->usWaitPending = FALSE;

     DosReleaseMutexSem( ulpInstance->hmtxNotifyAccess );

     DosReleaseMutexSem( ulpInstance->hmtxCloseAccess );
     return ( ulErr );
     }

  /****************************************************
  * If we received an eos or an event error, then this
  * means this thread is terminated--inform the caller.
  * Note: the posting of the MCI_TO case was handled
  * above.
  *****************************************************/


  if ( fPostMessage )
       {
       PostMDMMessage (ulErr, MCI_PLAY, &FuncBlock);
       }

  /********************************************************
  * If another command (such as a record, stop, seek etc.)
  * tries to abort this play thread, they will wait on
  * the thread semaphore to ensure that we cleaned up
  * properly.  Therefore, post the semaphore to indicate
  * that we are done.
  ********************************************************/

  DosPostEventSem (ulpInstance->hThreadSem);

  /***************************************
  * Allow other threads in the process to
  * continue and release exlcusive notify
  * semaphore.
  ***************************************/

  DosReleaseMutexSem( ulpInstance->hmtxNotifyAccess );

  DosReleaseMutexSem( ulpInstance->hmtxCloseAccess );

  return (MCIERR_SUCCESS);

} /* StartPlay */






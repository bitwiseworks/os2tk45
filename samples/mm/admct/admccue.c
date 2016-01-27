/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: admccue.c
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* DESCRIPTIVE NAME: Audio MCD WaveAudio cueing + event file
*
* FUNCTION:Cue an Waveform Audio Element for input or output
*          Set a cue point
*
* On an MCI_CUE, a streaming MCD should perform the following actions:
*
*  Check flags and validate pointers.
*  Check to see if the stream is already in cue state.  If it is, then
*   just return success.
*  If the caller wants to cue for output, execute the following:
*
*  Stop any in process commands.
*  If the stream is going in the wrong direction (i.e. record), destroy it.
*  Create the stream if necessary.
*  Preroll start the stream.
*
*  If the caller wants to cue for input, execute the following:
*
*  Stop any in process commands.
*  If the stream is going in the wrong direction, destroy it.
*  Create the stream if necessary.
*  Preroll start the stream.
*
*
* NOTES: Concepts illustrated in this source file.
*         A. Default flags for MCI_CUE processing.
*         B. When cueing is not necessary.
*         C. When to destroy a stream before a cue.
*         D. Why an MCI_SET before MCI_CUE requires stream
*            creation.
*         E. How to cue a stream for input or output.
*         F. How to create a cue point.
*         G. How/when to disable a cue point.
*
* ENTRY POINTS:
*
* INPUT:
*
*
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
*********************** END OF SPECIFICATIONS **********************/

#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCICUE..C
*
* DESCRIPTIVE NAME: Cue Waveform Device.
*
* FUNCTION: Prepare a Waveform Device or an Device Element for Record/Playback.
*           Applications will usually call this function in order to reduce
*           the latency of an MCI_RECORD or MCI_PLAY
*
* NOTES: Cueing a Waveform Device translates to prerolling of streams
*        at the SSM level.
*        Preroll means source handler fills the buffers and is
*        to roll. The Target handlers are not started, meaning buffers are
*        are not consumed.
*
* ENTRY POINTS:
*
* INPUT: MCI_CUE message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*
* EXTERNAL REFERENCES: AudioIFDriverEntry()       - AudioDevice MCD
*
*********************** END OF SPECIFICATIONS **********************/
RC MCICue (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG          ulrc;                 // Error Value
  ULONG          ulParam1;             // Msg Flags

  BOOL           fCueInput = FALSE;    // Direction to cue
  BOOL           fCueOutput = FALSE;   // "     "       "

  INSTANCE *     ulpInstance;          // Local Instance
  ULONG          ulCueFlags;           // Mask For Incoming MCI Flags

  PMCI_GENERIC_PARMS pCueParms;     // Msg data ptr

//  MCI_AMP_INSTANCE     BackupAmp;           // Hold old amp values



  /******************************
  * Derefernce pointers.
  ******************************/
  ulParam1   =    pFuncBlock->ulParam1;
  pCueParms = ( PMCI_GENERIC_PARMS ) pFuncBlock->ulParam2;
  ulpInstance= (INSTANCE *) pFuncBlock->ulpInstance;


  /*---------------------------------------------------------
  * Make a copy of the amp/mixer instance in case any errors
  * happened.
  *---------------------------------------------------------*/

//  memmove( &BackupAmp, &AMPMIX, sizeof( MCI_AMP_INSTANCE ) );


  /******************************
  * Check for Invalid Flags
  *******************************/
  ulCueFlags = ulParam1;
  ulCueFlags &= ~(MCI_NOTIFY     + MCI_WAIT +
                  MCI_CUE_INPUT  + MCI_CUE_OUTPUT +
                  MCI_WAVE_INPUT + MCI_WAVE_OUTPUT);

  /********************************************
  * If the caller passed any flags other
  * than the ones we masked out, return error
  ********************************************/

  if (ulCueFlags > 0)
      return ( MCIERR_INVALID_FLAG );

  /************************************************
  * Cue point parms are not required, however, if
  * they are there, ensure they are valid
  ************************************************/
  /*why do we check them then???????*/

  ulrc = CheckMem (((PVOID)pCueParms),
                   sizeof (MCI_GENERIC_PARMS), PAG_READ);

  /******************************************
  * Check for incompatible flags
  *******************************************/
  if (((ulParam1 & MCI_WAVE_INPUT)  || (ulParam1 & MCI_CUE_INPUT)) &&
      ((ulParam1 & MCI_WAVE_OUTPUT) || (ulParam1 & MCI_CUE_OUTPUT)))

      return (MCIERR_FLAGS_NOT_COMPATIBLE );

  /****************************************
  * If No Element return error
  *****************************************/

  if ( ulpInstance->fFileExists == FALSE )
     {
     return (MCIERR_FILE_NOT_FOUND);
     }

  /******************************************
  * If we do not have the ability to write then
  * cueing for input will be possible.
  *******************************************/

  if ( ( (ulParam1 & MCI_WAVE_INPUT) || (ulParam1 & MCI_CUE_INPUT) ) &&
       !ulpInstance->ulUsingTemp &&
       !ulpInstance->usPlayLstStrm )
    {
    return ( MCIERR_UNSUPPORTED_FUNCTION );
    }

  /******************************************
  * Waveaudio defaults to cueing for output
  * if no flags are passed.
  *******************************************/

  if (!(ulParam1 & MCI_WAVE_INPUT || ulParam1 & MCI_WAVE_OUTPUT ||
      ulParam1 & MCI_CUE_INPUT    || ulParam1 & MCI_CUE_OUTPUT))
      {
      ulParam1 &= MCI_WAVE_OUTPUT;

      }

  if ( ulParam1 & MCI_CUE_INPUT || ulParam1 & MCI_WAVE_INPUT )
     {
     fCueInput = TRUE;
     }
  else
     {
     fCueOutput = TRUE;
     }

  /************************************
  * If the stream is paused, there is no
  * sense in cueing it since the buffers
  * are full anyway
  **************************************/

  if ( STRMSTATE == MCI_PAUSE  ||
       STRMSTATE == STOP_PAUSED )

      {
      /***********************************************
      ** If the stream is going the right way
      ** then we have the ability to avoid the cue
      ** since the buffers have been filled up before
      ** we did the pause
      **********************************************/

      if ( ulpInstance->ulOperation == MCIDRV_INPUT &&
           fCueInput )
         {
         ulpInstance->ulCreateFlag = PREROLL_STATE;
         STRMSTATE = CUERECD_STATE;
         return ( MCIERR_SUCCESS );
         }

      /*************************************************
      * If the current stream is cued for playback and
      * we have a cue_output request, our work is done
      **************************************************/

      else if ( ulpInstance->ulOperation == MCIDRV_OUTPUT &&
                fCueOutput )
         {

         ulpInstance->ulCreateFlag = PREROLL_STATE;
         STRMSTATE = CUEPLAY_STATE;
         return ( MCIERR_SUCCESS );
         }

      }   /* If the stream may be in cue state */

  /**********************************************
  * Create and cue the playback stream A --> B
  ***********************************************/

  if ( fCueOutput )
     {
      ulrc = CueSetup( pFuncBlock, MCI_PLAY );

      STRMSTATE = CUEPLAY_STATE;

     } /* default case is wave output */

  /********************************************
  * If the caller requested a cue for input, then
  * create and cue the record stream B --> A
  *********************************************/

  else
     {
     ulrc = CueSetup( pFuncBlock, MCI_RECORD );

     STRMSTATE = CUERECD_STATE;

     }   /* Cue Input */


  if ( ulrc )
     {
     /* Ensure that our instance remains the same as before the cue attempt */

//     memmove( &AMPMIX, &BackupAmp, sizeof ( MCI_AMP_INSTANCE ) );

     }

  return (ulrc);
}


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: CueSetup
*
* DESCRIPTIVE NAME: Creates a stream and cues it.
*
* FUNCTION: This function abort any in progress commands and
*           cue a stream.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: INSTANCE ptr
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiCreateStream()
*                      spiStartStream()
*
*********************** END OF SPECIFICATIONS **********************/

ULONG CueSetup( FUNCTION_PARM_BLOCK *pFuncBlock,
                ULONG               ulMCIOperation )

{
ULONG        ulAbortNotify = FALSE;  // is there a command to abort?
ULONG        ulAmpDestroy;
ULONG        ulAmpOperation;
ULONG        ulrc;

INSTANCE     *pInstance;


   pInstance = (INSTANCE *) pFuncBlock->ulpInstance;

   if ( ulMCIOperation == MCI_RECORD )
      {
      ulAmpOperation = MCIDRV_INPUT;
      /* If the amp is in play mode, we must destroy the stream */
      ulAmpDestroy   = MCIDRV_OUTPUT;
      pInstance->ulCodecDescription = MAKE_TARGET | TARGET_CODEC;
      }
   else
      {
      ulAmpOperation = MCIDRV_OUTPUT;
      /* If the amp is in play mode, we must destroy the stream */
      ulAmpDestroy   = MCIDRV_INPUT;
      pInstance->ulCodecDescription = MAKE_SOURCE | SOURCE_CODEC;

      }

   /*****************************************
   * To ensure proper syncronization, acquire
   * the semaphore which is used to control
   * who can check to see which processes are
   * active. This function will also tell us
   * if there is an operation to abort or
   * supercede.
   ******************************************/

   GetNotifyAbortAccess ( pInstance, &ulAbortNotify );

   /*******************************************
   * If there is an operation active (i.e. a
   * play, record or save) then post a message
   * stating that the command has been
   * aborted ( play or record) or
   * wait for completion (save).
   ********************************************/

   if ( ulAbortNotify )
      {
      ulrc = CueAbortInProgressNotify( pInstance, pFuncBlock, ulMCIOperation );
      }

    /**********************************************
    * If the card is in record mode, or a record
    * stream is active, destroy the stream.
    ***********************************************/

    if (  pInstance->ulOperation == ulAmpDestroy )

         {
// CONNECTOR FEATURE
         StopStream2( pInstance, MCIDRV_DISCARD_STREAM_NETWORK );

         /******************************************
         * Stop The Previous stream
         *******************************************/
//         SpiStopStream ( pInstance->StreamInfo.hStream,
//                         SPI_STOP_DISCARD);

         /**************************************
         * Destroy the previous stream
         **************************************/
#ifndef CONNECTION
         DestroyStream ( pInstance);
#endif
// CONNECTOR FEATURE
         /*******************************************
         * Set Stream Creation flag to create state
         ********************************************/
         pInstance->ulCreateFlag = CREATE_STATE;

#ifndef CONNECTION
         if ( pInstance->ulRealTimeTranslation ==MMIO_REALTIME )
            {
            DataTranslation( pInstance );
            }
#endif

         }



   /***********************************************
   * If a set was performed on an existing stream,
   * destroy the stream and get new spcb keys
   ***********************************************/

   DestroySetStream ( pInstance );

    /***********************************************
    * Set the stream up and cue it for play back!
    ***********************************************/

    ulrc = CueStream( pInstance, ulAmpOperation );

    return ( ulrc );

} /* CueSetup */


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: CueStream
*
* DESCRIPTIVE NAME: Creates a stream and cues it.
*
* FUNCTION: This function will create and stream, and will
*           preroll start for playback.  Since record buffers
*           will be filled with garbage, it is pointless to
*           preroll start it.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: INSTANCE ptr
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiCreateStream()
*                      spiStartStream()
*
*********************** END OF SPECIFICATIONS **********************/

ULONG CueStream( INSTANCE *ulpInstance,
                 ULONG    ulOperation )


{

    ULONG    ulrc;               // return codes
    ULONG    ulCount;            // Semaphore variable
    BOOL     fInitNeeded = FALSE;// Must we initialize the card?

   /****************************************
   * If no stream has been created, and the
   * card has not changed modes, there is no
   * need to reinit it!
   *****************************************/

   if ( ulpInstance->ulOperation != ulOperation  ||
        ulpInstance->StreamInfo.ulState == STREAM_SET_STATE )
      {
      fInitNeeded = TRUE;
      }



    /*******************************
    * Do stream set up work and then
    * create the stream
    *******************************/

    ulrc = PrepareAndCreateStream( ulpInstance, ulOperation, fInitNeeded  );

    if ( ulrc )
       {
       return ( ulrc );
       }


     /*******************************
     * Preroll Start the stream.
     *******************************/
     DosResetEventSem (ulpInstance->hEventSem, &ulCount);

     /********************************************************
     * We do not want to preroll start a record stream
     * since the buffers willbe filled with bogus information
     * so only fill the play stream with a preroll start.
     *********************************************************/

     if ( ( STRMSTATE != CUEPLAY_STATE && ulOperation == MCIDRV_OUTPUT) )
         {
// CONNECTOR FEATURE
// is it possible to get here and need to test cueplay?????
//
           BeginQualityofService( ulpInstance, STREAM_READ );

           ulrc = StartStream( ulpInstance, MCIDRV_CUE_PLAYBACK );
//         ulrc = SpiStartStream (ulpInstance->StreamInfo.hStream,
//                                SPI_START_PREROLL);

// CONNECTOR FEATURE
         if (ulrc)
            {
            return ulrc;
            }

         /*************************************
         * Wait till you Recieve PREROLL Event
         *************************************/

         DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);
         EndQualityofService( ulpInstance );

         } /* Preroll Start */

     /*************************************
     * Update state To preroll
     **************************************/

     ulpInstance->ulCreateFlag = PREROLL_STATE;

     return ( ulrc );

} /* Cue Stream */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: AbortInProgressNotify
*
* DESCRIPTIVE NAME: Stops a notify which was already in progress
*
* FUNCTION: Stops save, record and playback notifies
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: INSTANCE ptr
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiStopStream()    - SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/

ULONG CueAbortInProgressNotify( INSTANCE             *ulpInstance,
                                FUNCTION_PARM_BLOCK  *pFuncBlock,
                                ULONG                ulMessage )


{
  ULONG ulrc = MCIERR_SUCCESS;

  /*****************************************************
  * If a stream is currently in use, destroy or stop it!
  ******************************************************/

     if ( ulpInstance->usNotPendingMsg == MCI_SAVE )
       {
       /***********************************
       * Can't interrupt a save because data
       * can be lost--wait till completion
       ************************************/

       DosWaitEventSem( ulpInstance->hThreadSem, (ULONG ) -1 );

       }
     else
       {
       PostMDMMessage ( MCI_NOTIFY_ABORTED,
                        ulpInstance->usNotPendingMsg,
                        pFuncBlock );

      /* Stop the pending thread */

      ThreadedStop ( ulpInstance );

       /**********************************************************
       * If we have a record stream and we are cueing for input
       * or if we have a playback stream and are cueing for output
       * then don't destroy the stream.  Just place it in a
       * stopped state.
       **********************************************************/

       if ( ulpInstance->usNotPendingMsg != ulMessage )
          {
// CONNECTOR FEATURE--destroys are no longer needed.
          /**************************************
          * Destroy the previous stream since it
          * was going the wrong way.
          ***************************************/
#ifndef CONNECTION
        DestroyStream ( ulpInstance);
#endif
// CONNECTOR FEATURE

          /***************************************
          * Update the stream state so that following
          * routines will be forced to create a new
          * stream.
          ****************************************/

          ulpInstance->ulCreateFlag = CREATE_STATE;

          } /* else this is a play stream */

        } /* notify pending was not a !save */


   return ( ulrc );

} /* CueAbortInProgressNotify */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISCPT.C
*
* DESCRIPTIVE NAME: Set Cue Point Waveform Device.
*
* FUNCTION: Establish Cue points during playback on Waveform Device.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_SET_CUEPOINT message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiEnableEvent()    - SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/


RC MCISetCuePoint (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG           ulrc;               // Propogated MME Error Code
  ULONG           ulParam1;           // Incoming MCI Flag
  ULONG           ulCuePointFlags;    // Mask For Incoming Flags
  INSTANCE        *ulpInstance;       // Local Instance

  PID             pid;
  TID             tid;

  HWND            hWnd;

  PMCI_CUEPOINT_PARMS pCueParms;    // Msg MCI Data Structure

  /****************************
  * Dereference pointers
  *****************************/

  ulParam1 = pFuncBlock->ulParam1;
  pCueParms = ( PMCI_CUEPOINT_PARMS ) pFuncBlock->ulParam2;
  ulpInstance= (INSTANCE *) pFuncBlock->ulpInstance;

  /**************************
  * Mask out supported flags
  **************************/
  ulCuePointFlags = ulParam1;
  ulCuePointFlags &= ~( MCI_WAIT            + MCI_NOTIFY +
                   MCI_SET_CUEPOINT_ON + MCI_SET_CUEPOINT_OFF );


  /* If the caller passed in an invalid flag return an error */

  if (ulCuePointFlags > 0 )
      return ( MCIERR_INVALID_FLAG );

  /******************************************
  * Check to see if CuePoint parms are good
  ******************************************/

  ulrc = CheckMem (((PVOID) pCueParms ),
                   sizeof (MCI_CUEPOINT_PARMS), PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
      return ( MCIERR_MISSING_PARAMETER );

  /**********************************************
  * It is the MCD's responsibility to ensure that
  * the callback handle that the caller passed in
  * is valid--so ask PM if it is valid.
  **********************************************/

  hWnd = pCueParms->hwndCallback;


  if ( !WinQueryWindowProcess(hWnd, &pid, &tid) )
      return (MCIERR_INVALID_CALLBACK_HANDLE);

  /************************************************
  * Check for valid flags but invalid combination
  *************************************************/

  if (ulParam1 & MCI_SET_CUEPOINT_ON && ulParam1 & MCI_SET_CUEPOINT_OFF)
      return ( MCIERR_FLAGS_NOT_COMPATIBLE );

  /***************************************************
  * The caller must either turn a cue point on or off
  ****************************************************/


  if (!(ulParam1 & MCI_SET_CUEPOINT_ON || ulParam1 & MCI_SET_CUEPOINT_OFF))
       return ( MCIERR_MISSING_FLAG );

  /*****************************************
  * No media element loaded--reject cuepoint
  ******************************************/

  // make file exists to ulong--set to true/false.

  if ( ulpInstance->fFileExists == FALSE )
     {
     return (MCIERR_FILE_NOT_FOUND);
     }

  /*************************
  * Check for range errors
  *************************/

  if (pCueParms->ulCuepoint <= 0)

      return (MCIERR_OUTOFRANGE);

  /**************************************
  * Enable cuepoints if on flag is passed
  **************************************/

  if (ulParam1 & MCI_SET_CUEPOINT_ON)
     {
     /**********************************************
     * Enable CuePoints and add to linked list
     **********************************************/

     ulrc = CuePointAdd( ulpInstance, pCueParms );

     } /* Set Cue Point On */


  /*************************************
  * Disable cuepoint if off flag is set
  *************************************/

  if (ulParam1 & MCI_SET_CUEPOINT_OFF)
     {
     /**********************************************
     * Disable CuePoints and remove from linked list
     **********************************************/

     ulrc = CuePointDelete( ulpInstance, pCueParms );

     } /* CuePoint Off */


  return ( ulrc );

} /* MCISetCuePoint */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: CuePointAdd
*
* DESCRIPTIVE
*
* FUNCTION:  Adds cuepoint to linked list of cue points
*
* NOTES: This function illustrates how to extend an
*        event control block to contain custom information.
*        For example, each cue point EVCB can contain its
*        own window callback handle.
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
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
RC CuePointAdd ( INSTANCE             *ulpInstance,
                 PMCI_CUEPOINT_PARMS pCueParms )

{
   MTIME_EVCB  *pTempMCuePtEVCB;   // temporary pointer to item in linked list

   ULONG       ulTempTime;         // time conversion variable
   ULONG       ulFoundCuePoint=FALSE; // Find flag
   ULONG       ulrc;

   extern HHUGEHEAP     heap;                // Global MCD Heap

   pTempMCuePtEVCB = CUEPOINT;

   /********************************************
   * Check to see if the cue point is already in
   * our linked list of cuepoints.  Continue
   * checking while we have a list and we have
   * not found the cuepoint.
   *********************************************/

   while ( pTempMCuePtEVCB && !ulFoundCuePoint )
      {

      /*****************************************************
      * If the current node is the same as the cuepoint
      * that the caller wants to set, then its a duplicate
      *****************************************************/

      if (pTempMCuePtEVCB->mmCuePt == pCueParms->ulCuepoint)
         {
         ulFoundCuePoint = TRUE;
         }

      /* Keep progressing through the list */

      pTempMCuePtEVCB = pTempMCuePtEVCB->pNextEVCB;

      }

   /* If we found the cuepoint in the list--its a duplicate */

   if ( ulFoundCuePoint )
      {
      return MCIERR_DUPLICATE_CUEPOINT;
      }

   AcquireProcSem ();


   if ( !(pTempMCuePtEVCB = HhpAllocMem (heap, sizeof ( MTIME_EVCB ) ) ) )
      {
      return (MCIERR_OUT_OF_MEMORY);
      }

   ReleaseProcSem ();

   /********************************************
   * Fill In the window handle and device id
   *********************************************/


   pTempMCuePtEVCB->hwndCallback = pCueParms->hwndCallback;
   pTempMCuePtEVCB->usDeviceID = ulpInstance->usWaveDeviceID;

   /********************************************
   * Fill In Time event control block for a
   * cue point ( all of these fields are REQUIRED)
   * Note: We have altered the evcb to contain
   * additional information (such as our callback
   * userParm, instance etc.).  See audiomcd.h
   * for an explanation of how this is done.
   *********************************************/

   pTempMCuePtEVCB->evcb.ulType = EVENT_CUE_TIME;
   pTempMCuePtEVCB->evcb.ulFlags = EVENT_SINGLE;
   pTempMCuePtEVCB->evcb.hstream = ulpInstance->StreamInfo.hStream;

   /*********************************************
   * Store the time at which the cue point
   * is to be generated ( this must be in MMTIME
   *********************************************/

   ulrc = ConvertToMM ( ulpInstance,
                        &ulTempTime,
                        pCueParms->ulCuepoint);

   pTempMCuePtEVCB->evcb.mmtimeStream = ulTempTime;

   /***************************************************
   * Store the callback handle that the caller wishes
   * the cuepoint notification to occur on.
   * Remember each cuepoint can have its own callback.
   ****************************************************/

   pTempMCuePtEVCB->mmCuePt = pCueParms->ulCuepoint;

   /*****************************************************
   * Stick In INSTANCE Pointer in The Time EVCB, this will
   * allow the event procedure to have access to our
   * instance--it ordinarily would not have access to it.
   * See comments in PlayEventProc and RecordEventProc.
   ******************************************************/

   pTempMCuePtEVCB->ulpInstance = (ULONG) ulpInstance;


   /*****************************************************
   * Copy CuePoint User Parm into EVCB Structure
   ******************************************************/
   pTempMCuePtEVCB->usCueUsrParm =pCueParms->usUserParm;

   /*****************************************************
   * Link this cue point into the list of cue points
   ******************************************************/

   pTempMCuePtEVCB->pNextEVCB = CUEPOINT;
   CUEPOINT = pTempMCuePtEVCB;

   /************************************************
   * We can only enable events after a stream has
   * been created--otherwise you will get errors.
   * If we cannot enable the event right now, store
   * this in a flag, and enable it before playback
   * or record.
   ************************************************/

   if (ulpInstance->ulCreateFlag == PREROLL_STATE)
      {
      ulrc = ADMCEnableEvent( (PEVCB) pTempMCuePtEVCB,
                             (PHEVENT) &pTempMCuePtEVCB->HCuePtHndl );

      ulpInstance->usCuePt = EVENT_ENABLED;
      }
   else
      {
      ulpInstance->usCuePt = TRUE;
      }

   return ( ulrc );

} /* CuePointAdd */




/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: CuePointDelete
*
* DESCRIPTIVE
*
* FUNCTION:  Removes cuepoint from linked list of cue points
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
RC CuePointDelete ( INSTANCE             *ulpInstance,
                    PMCI_CUEPOINT_PARMS pCueParms )

{
   MTIME_EVCB  *pTempMCuePtEVCB;         // temporary pointer to item in linked list
   MTIME_EVCB  *pPrevMCuePtEVCB;         // temporary pointer to item in linked list

   ULONG       ulFoundCuePoint = FALSE;  // Flag to indicate whether the cpt was found
   ULONG       ulrc = MCIERR_SUCCESS;    // @11889

   extern HHUGEHEAP     heap;                // Global MCD Heap

  /* If we have never enabled a cuepoint, we can't delete it */

  if ( !CUEPOINT )
     {
     return ( MCIERR_INVALID_CUEPOINT );
     }

  pTempMCuePtEVCB = pPrevMCuePtEVCB = CUEPOINT;

  /********************************************
  * Check to see if the cue point is already in
  * our linked list of cuepoints.  Continue
  * checking while we have a list and we have
  * not found the cuepoint.
  *********************************************/

  while ( pTempMCuePtEVCB && !ulFoundCuePoint )
    {
    if ( pTempMCuePtEVCB->mmCuePt == pCueParms->ulCuepoint)
       {

       /********************************************
       * If we found a cuepoint and the stream is
       * active, remove the cuepoint.  Simply
       * deleting it from our linked list is not
       * enough since the cuepoint will continue
       * to be reported until it is removed.
       ********************************************/

       if ( ulpInstance->usCuePt == EVENT_ENABLED )

          {
          /***********************************************
          * Check Validity of hCuePointEvent
          **********************************************/
          ulrc = ADMCDisableEvent ( pTempMCuePtEVCB->HCuePtHndl );

          } /* CreateFlag == PrerollState */

       ulFoundCuePoint = TRUE;

       /***********************************************
       * Ensure that the previous node points to the
       * next node.
       *********************************************/

       if ( pTempMCuePtEVCB != pPrevMCuePtEVCB )
          {
          pPrevMCuePtEVCB->pNextEVCB = pTempMCuePtEVCB->pNextEVCB;
          }
       else
          {
          /***********************************************
          * If we are deleting the first item in the list
          * ensure that we have a valid initial pointer
          **********************************************/

          CUEPOINT = pTempMCuePtEVCB->pNextEVCB;
          }

       /* Toast the node to delete */

       CleanUp( pTempMCuePtEVCB );


       } /* Found the CuePoint Block */

    /**********************************************
    * Else we didn't find the requested cue point
    * so advance a node
    **********************************************/

    else
       {
       if ( pTempMCuePtEVCB != pPrevMCuePtEVCB )
          {
          pTempMCuePtEVCB = pTempMCuePtEVCB->pNextEVCB;
          pPrevMCuePtEVCB = pPrevMCuePtEVCB->pNextEVCB;
          }
       else
          {
          pTempMCuePtEVCB = pTempMCuePtEVCB->pNextEVCB;
          }

       } /* else we haven't found the requested cue point */

    } /* endwhile */

    /* If we couldn't find the cuepoint--report the error */

    if ( ulFoundCuePoint == FALSE)
       {
       return ( MCIERR_INVALID_CUEPOINT );
       }

    return ( ulrc );

} /* CuePointDelete */



 /*********************** END OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME:  AUDIOMCD.C
*
* DESCRIPTIVE NAME:  WAVEFORM MCD (PLAYER)
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: Main switch routine for the Audio MCD.  ALL incoming messages
*           are processed in this routine.  Messages can be divided into
*           four types:
*
*   A. MCI_OPEN:  this is the FIRST message any MCD should receive.
*      Minimum processing should be done on the open, and except for
*      incorrect flags or some major catastrophy--it should succeed.
*
*   B. Messages directly from the Media Device Manager (MDM).
*      MCIDRV_RESTORE
*      MCIDRV_SAVE
*      These messages inform the media driver to pause/resume or
*      initialize itself on the attached device (in our case, the
*      audio card).
*      After an MCI_OPEN, an MCIDRV_RESTORE will be the second
*      command and MCD will receive.  If another instance of our MCD
*      or another MCD is using the audio card, MDM will issue an
*      MCIDRV_SAVE to inform us to save our state since we are being
*      removed from the card.
*
*   C. Necessary commands (this will vary from device to device--
*      in our case play/record--see the switch for more information).
*
*   D. Unnecessary commands (such as masteraudio) which we do no
*      processing.  Note: what may be necessary for one MCD may
*      be needed for another.
*
* NOTES:
*
*
* EXTERNAL ENTRY POINTS:
*
*
* INTERNAL ENTRY POINTS:
*
*
* EXTERNAL REFERENCES (system):
*
*
*
* MODIFICATION HISTORY:
* DATE      DEVELOPER         CHANGE DESCRIPTION
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>                        // OS2 defines.
#include <string.h>
#include <os2medef.h>                   // MME includes files.
#include <stdlib.h>                     // Math functions
#include <ssm.h>                        // SSM Spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mcipriv.h>                    // MCI Connection stuff
#include <mmdrvos2.h>                   // MCI Driver include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes.
#include <sw.h>
#include <checkmem.h>


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: mciDriverEntry
*
* DESCRIPTIVE NAME: Single Entry point into Waveform MCD Player.
*
* FUNCTION: Process MCI Waveform messages.
*
* NOTES: Device specific information is obtained via the
*        VSDIDriverEntry interface.
*
* ENTRY POINTS:  mciDriverEntry ()
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_MESSAGES.
*
* PARAMETERS:   ulpInstance  - pointer to waveform  instance structure.
*               usMessage    - MCI message to act on.
*               ulParam1     - MCI Flags for this message.
*               ulParam2     - Message  data.
*               usUserParm   - User Parameter Returned on Notifications.
*
* EXIT-NORMAL: Return Code 0
*
* EXIT_ERROR:  MME Error Code
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*                      ADMCINIT, MCIOPEN, MCDCAPS, MCISET,
*                      MCISTAT, MCICUE,  MCISEEK, MCIPLAY,
*                      MCISCPT, MCDINFO,  MCIPAUS, MCICLOS.
*                      MCISTOP
*
* EXTERNAL REFERENCES: mdmDriverNotify, SSM SPIs, MMIO APIs, AudioIFDriverEntry
*                      heap Allocation and management APIs
*                      OS/2 APIs
*
*********************** END OF SPECIFICATIONS ********************************/

/* This MCI driver is compiled with optlink linkage in C-SET/2
   the following pragma allows other dll's etc to call in */

ULONG APIENTRY  MM_mciDriverEntryADMC ( PVOID    ulpInstance,     // Instance Ptr
                                        USHORT   usMessage,       // Message
                                        ULONG    ulParam1,        // Flags
                                        ULONG    ulParam2,        // Data
                                        USHORT   usUserParm)      // Data
{

  ULONG                   ulrc;        // RC

  BOOL                    fNotify=TRUE;// should the main function handle
                                       // notification
  FUNCTION_PARM_BLOCK     ParamBlock;  // Encapsulate Parameters




  /****************************************
  * Return any unsupported functions or
  * functions which do nothing right away.
  *****************************************/

  if ( usMessage == MCI_SPIN           ||
       usMessage == MCI_DEVICESETTINGS ||
       usMessage == MCI_ESCAPE         ||
       usMessage == MCI_STEP           ||
       usMessage == MCI_UPDATE         ||
       usMessage == MCI_GETTOC )
     {
     return ( MCIERR_UNSUPPORTED_FUNCTION );
     }
  else if ( usMessage == MCI_MASTERAUDIO )
     {
     return ( MCIERR_SUCCESS );
     }

  /******************************************
  * Copy the mciDriverEntry parameters to a
  * structure which contains all of them.  This
  * simplifies calling functions etc. since we
  * can pass one parameter rather than 5 or 6.
  /******************************************/

  ParamBlock.usMessage   = usMessage;
  ParamBlock.ulpInstance = ulpInstance;
  ParamBlock.pInstance   = (INSTANCE *)ulpInstance;
  ParamBlock.usUserParm  = usUserParm;
  ParamBlock.ulParam1    = ulParam1;
  ParamBlock.ulParam2    = ulParam2;



  /************************************************************************
  * Request The Instance Data Access Sem and block all incoming Threads on
  * this semaphore. In case of MCI_PLAY and MCI_RECORD an earlier Release
  * of the Semaphore occurs. All other Incoming MCI Messages on different
  * threads are serialized.
  ************************************************************************/


  if ( usMessage != MCI_OPEN )
      {
      if ( !(usMessage == MCI_CLOSE  &&
           ulParam1 & MCI_CLOSE_EXIT ) )
         {
         AbortWaitOperation ((INSTANCE *)ulpInstance);

         /*------------------------------------------------------
         * We may potentially have to give up the data access
         * sem we acquired in MCD_EnterCrit in order to prevent
         * a hang with MCIDRV_SAVE (see SetAudioDevice for more
         * info on how this can happen).  Therefore, protect
         * the data access sem with an outer sem (saveaccess).
         *------------------------------------------------------*/
         if ( usMessage != MCIDRV_SAVE )
            {
            GetSaveSem ( (INSTANCE *) ulpInstance);
            }
         GetStreamingSemaphore( ulpInstance );
         MCD_EnterCrit ( (INSTANCE *) ulpInstance);
         }
      }

  /************************************
  * Prepare the instance for notify, wait
  * or asychronous operation
  *************************************/

  NotifyWaitSetup( &ParamBlock, usMessage, ulParam1, ulParam2 );

  switch (usMessage)
  {

  case MCI_OPEN:
       {
       /**************************************
       * Check that we have a valid pointer
       * in ulParam2 before attempting to use
       * it
       ***************************************/

       if (ulrc = CheckMem ((PVOID)ulParam2,
                            sizeof (ULONG),
                            PAG_READ))

           return ( MCIERR_MISSING_PARAMETER );

        ulrc = AllocateInstance( &ParamBlock );

        if ( ulrc )
           {
           return ( ulrc );
           }

         /*************************************************
         * Parse all of the flags and ensure that none are
         * conflicting.  Set up semaphores, memory, files,
         * stream hanlders necessary to perform work if
         * everything appears ok
         **************************************************/

         ulrc = MCIOpen ( &ParamBlock);

         if (ulrc)
           {

            /****************************************
            * Free Up all the Resources if Open Fails
            *****************************************/

            DosCloseEventSem (ParamBlock.pInstance->hEventSem);
            DosCloseEventSem (ParamBlock.pInstance->hThreadSem);
            DosCloseMutexSem (ParamBlock.pInstance->hmtxDataAccess);
            CleanUp ( ( PVOID ) ParamBlock.pInstance );
            return ( ulrc );

            } /* Error On Open */


       }
      break;

  case MCI_GETDEVCAPS:
       ulrc = MCICaps (&ParamBlock);
      break;
  case MCI_SET:
        ulrc = MCISet (&ParamBlock);
       break;

  case MCI_STATUS:

       ulrc = MCIStat (&ParamBlock);
      break;

  case MCI_CUE:
       ulrc = MCICue (&ParamBlock);
      break;

  case MCI_SEEK:
       ulrc = MCISeek(&ParamBlock);
      break;

  case MCI_PLAY:
       {

       /*************************************************
       * Do the necessary processing to prepare the
       * playback stream (i.e. creation, preparing the
       * card for playback) and start the play.
       **************************************************/

       ulrc = MCIPlay ( &ParamBlock);


       /* Play thread will handle notifications */
       fNotify = FALSE;

       }
      break;

  case MCI_RECORD:
       {
       /*************************************************
       * Do the necessary processing to prepare the
       * record stream (i.e. creation, preparing the
       * card for playback) and start the play.
       **************************************************/

       ulrc = MCIRecd ( &ParamBlock);

       /* Record thread will handle notifications */

       fNotify = FALSE;


       }
      break;

  case MCI_SET_CUEPOINT:
       ulrc = MCISetCuePoint (&ParamBlock);
      break;

  case MCI_INFO:

       ulrc = MCIInfo (&ParamBlock);
      break;

  case MCI_PAUSE:
       ulrc = MCIPaus (&ParamBlock);
      break;

  case MCI_CLOSE:
       /* Close function will handle notifications */
       fNotify = FALSE;
       ulrc = MCIClos (&ParamBlock);
      break;

  case MCI_CONNECTOR:
       ulrc = MCIConnector (&ParamBlock);
      break;

//  case MCI_TEMPCONNECTION:
//       ulrc = MCIConnection (&ParamBlock);
//      break;

  case MCI_STOP:
       ulrc = MCIStop (&ParamBlock);
      break;

  case MCIDRV_SAVE:
       ulrc = MCISave (&ParamBlock);

       /* Don't notify on save */
       fNotify = FALSE;
      break;

  case MCIDRV_RESTORE:
       ulrc = MCIRest (&ParamBlock);
       /* Don't notify on restore will handle notifications */
       fNotify = FALSE;

      break;
#ifdef CONNECTION
  case MCIDRV_CONNECT:
       ulrc = MCIMakeConnection(&ParamBlock);
       /* Don't notify on restore will handle notifications */
       fNotify = FALSE;

      break;
  case MCIDRV_TEST_CONNECTION:
       ulrc = MCITestConnection (&ParamBlock);
       /* Don't notify on restore will handle notifications */
       fNotify = FALSE;

      break;
  case MCIDRV_DISCONNECT:
       ulrc = MCIBreakConnection (&ParamBlock);
       /* Don't notify on restore will handle notifications */
       fNotify = FALSE;

      break;
  case MCIDRV_ASSOCIATE:
       ulrc = MCIAssociate (&ParamBlock);
       /* Don't notify on restore will handle notifications */
       fNotify = FALSE;

      break;
#endif
  case MCI_RESUME:
       ulrc = MCIResume (&ParamBlock);
      break;

 case MCI_SAVE:
       /* Don't notify on save */
       fNotify = FALSE;

       ulrc = MCISaveFile( &ParamBlock );
      break;

 case MCI_LOAD:
       ulrc = MCILoad (&ParamBlock);
      break;

  case MCI_SET_POSITION_ADVISE:
       ulrc = MCISetPositionAdvise (&ParamBlock);
      break;

//  case MCI_SET_SYNC_OFFSET:
//       ulrc = MCISync (ParamBlock);
//      break;

//  case MCIDRV_SYNC:
//       ulrc = MCISync (&ParamBlock);
//
//      break;


   case MCI_CUT :
      ulrc = MCICopy ( &ParamBlock, MCI_CUT  );
      /* Don't notify on paste */
      fNotify = FALSE;

      break;

  case MCI_COPY :
      ulrc = MCICopy( &ParamBlock, MCI_COPY );
      /* Don't notify on paste */
      fNotify = FALSE;

      break;
  case MCI_PASTE :
      ulrc = MCIPaste( &ParamBlock );

      /* Don't notify on paste */
      fNotify = FALSE;

      break;
  case MCI_UNDO :
      ulrc = MCIMagic( &ParamBlock, MMIOM_UNDO );
      break;
  case MCI_REDO :
      ulrc = MCIMagic( &ParamBlock, MMIOM_REDO );
      break;
  case MCI_DELETE :
      ulrc = MCICopy( &ParamBlock, MCI_DELETE );
      /* Don't notify on paste */
      fNotify = FALSE;

      break;

  default:
          ulrc =  MCIERR_UNRECOGNIZED_COMMAND;
      break;


  }   /* Switch */


  /***********************************************************************
  * Post The message if notify for synchronous messages only. Exclusive
  * messages are Open, Close, Play and Record. An Open is not complete
  * until it is restored. So a Restore Postes the Notification for an
  * Open Command.
  ***********************************************************************/

  if (ParamBlock.ulNotify == TRUE)
     {
     if ( fNotify )
         {
         if (LOBYTE(ulrc) == MCIERR_SUCCESS)
            {
            /****************************************
            * Tell MDM that the operation has been
            * completed and that the mdmdrivernotify
            * should be called.  MDM will do a
            * winpostmessage and the application will
            * be informed of the completion.
            *****************************************/

            PostMDMMessage ( MCI_NOTIFY_SUCCESSFUL, usMessage, &ParamBlock);
            }

         }   /* Exclusive Messages */

     }  /* Notify is On */

  /*********************************************************
  * Release all Blocked Threads (for example, if a MCI_SET
  * is being processed and an MCI_PLAY comes in, we want
  * the play to wait until the set has completed or
  * multiple threads could be operating on the instance.
  *********************************************************/

  if (usMessage != MCI_OPEN && usMessage != MCI_CLOSE )
     {
     GiveUpSaveSem ( (INSTANCE *) ulpInstance);
     ReleaseStreamingSemaphore( ulpInstance );

     MCD_ExitCrit ((INSTANCE *) ulpInstance);

     }



  return (ulrc);    /* Return to MDM */

} /* mciDriverEntry */





/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME:  MCISAVE
*
* DESCRIPTIVE NAME: Save Waveform Instance State.
*
* FUNCTION:
* A streaming MCD will receive a save message when it loses
* control of the device they are attached to (for example, someone
* started another application which takes over the waveaudio device).
* On a save, the MCD should perform the following commands.
*
* Check to see if we are currently streaming (either record or playback).
* If so, pause the stream, and set a flag stating that we were saved.
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
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES:  VSDIDriverInterface()  -  VSD
*
*********************** END OF SPECIFICATIONS **********************/
RC MCISave (FUNCTION_PARM_BLOCK *pFuncBlock)
{


  INSTANCE     * ulpInstance;
  ULONG        ulrc;

  ulpInstance= (INSTANCE *)pFuncBlock->ulpInstance;

  /******************************************
  * The only thing that a streaming MCD must
  * be concerned about on a save message is
  * saving the state of the stream.  If no
  * stream has been started, then our work is
  * done.  However, if a stream is active then
  * it must be paused. The create flag
  * indicates the stream status, preroll means
   * the stream is playing/recorded/cued etc.
  ******************************************/

  if (ulpInstance->ulCreateFlag == PREROLL_STATE)
      {

      /************************************************
      * Pause The Stream for Context Switching.  After
      * the pause has been done, MDM will tell the
      * the connected device (in most cases the amp/mixer)
      * to save the state of the audio card.  Once both
      * of these actions have been done, another instance
      * can operate on the audio card.
      **************************************************/

      if ( STRMSTATE == MCI_PLAY   ||
           STRMSTATE == MCI_RECORD )
         {
         ulrc = ADMCStopStream (ulpInstance, SPI_STOP_STREAM);

         if ( ulrc )
            {
            return ( ulrc );
            }

         // make this a BOOL

         ulpInstance->usSaveFlag = TRUE;
         } /* if the stream must be paused */

      } /* Stream Created */

  ulpInstance->ulHoldState = STRMSTATE;

  STRMSTATE = SAVEPAUS_STATE;


  return (MCIERR_SUCCESS); // Dont Return Error Conditions
}



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME:  MCIREST
*
* DESCRIPTIVE NAME: Restore Waveform Instance State.
*
* FUNCTION:
*  A streaming MCD will receive a restore message when it regains
*  control of the device they are attached to (for example, someone
*  quit another application causing us to gain use of the device).
*  On a restore, the MCD should perform the following commands:
*
*  Check to see if we are in paused state.  If so, resume the stream.
*
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
*********************** END OF SPECIFICATIONS **********************/

RC MCIRest (FUNCTION_PARM_BLOCK *pFuncBlock)
{


  INSTANCE        * ulpInstance;
  ULONG           ulrc = MCIERR_SUCCESS;


  ulpInstance= (INSTANCE *)pFuncBlock->ulpInstance;

  /**************************************************
  * Restore will only perform work when a stream has
  * been paused after it was started (see comments
  * in MCISave).  If the saveflag is true and the
  * createflag is in a preroll state then resume the
  * stream.
  ***************************************************/

  if (ulpInstance->usSaveFlag == TRUE)
      {
      if (ulpInstance->ulCreateFlag == PREROLL_STATE)
          {
          /*******************************************
          * If The Stream was in a running state
          * before Context switch start the stream
          * else just set the restored flag to true
          ********************************************/

          if (STRMSTATE == SAVEPAUS_STATE)
              {

              /***********************************************
              * Start the stream from this position
              ************************************************/
              ulrc = SpiStartStream (STREAM.hStream,
                                     SPI_START_STREAM);

              if (ulrc)
                  return (ulrc);

//              STRMSTATE = MCI_PLAY;
              } /* Save Pause State */

          } /* PreRoll State */
     /*--------------------------------------------------
     * Ensure that other restores will not falsely
     * restart the stream by setting a flag.
     *--------------------------------------------------*/

     ulpInstance->usSaveFlag = FALSE;

     } /* True Save Flag */

  if ( STRMSTATE == SAVEPAUS_STATE )
     {
     STRMSTATE = ulpInstance->ulHoldState;
     }

  /* If this is the first restore, get information from the mixer */

//  if ( ulpInstance->ulFlags & FIRST_RESTORE )
//     {
//     ulrc = GetMixerInfo( ulpInstance );
//     ulpInstance->ulFlags &= ~FIRST_RESTORE;
//     }
  return (ulrc); // Dont Return Error Conditions
}


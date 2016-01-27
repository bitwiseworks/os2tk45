/******************** START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIREST.c
*
* DESCRIPTIVE NAME: A couple of excess functions.
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:
*
* NOTES: The following concepts are illustrated in the source file.
*        C. How to pause/resume a stream
*        D. Support stream transition tables.
*        E. Processing INFO_PRODUCT + INFO_FILE
*
* ENTRY POINTS:
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
#include <mmdrvos2.h>                     // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISTOP.C
*
* DESCRIPTIVE
*
* FUNCTION:  Stop Playing/Recording.
*  On a stop, a streaming MCD should perform the following commands:
*
*  A. Verify that the flags passed are valid.
*  B. Stop any commands which are active on another thread.
*  C. If a stream had previously been created, ensure that it is in
*     stopped state.
*  D. If it is a paused stream, then do a STOP_PAUSE to ensure that
*     no data will be lost.
*
* NOTES:
*
* ENTRY POINTS:
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
* EXTERNAL REFERENCES: spiStopStream ()   -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/
RC MCIStop ( FUNCTION_PARM_BLOCK *pFuncBlock)
{
   ULONG           ulrc;                   // MME Error Value Propogated
   INSTANCE        *ulpInstance;           // Local Instance
   ULONG           ulParam1;               // Incoming MCI Flags
   ULONG           ulCnt;                  // Semaphore Posts Freq
   ULONG           ulAbortNotify = FALSE;  // whether or not to abort a notify
   ULONG           ulStopFlags;            // Mask for Incoming MCI Flags
   ULONG           ulSpiFlags;


   /*****************************
   * Do Some Flag Checking
   ******************************/
   ulParam1 = pFuncBlock->ulParam1;

   /* Mask out illegal file */

   ulStopFlags = ulParam1;
   ulStopFlags &= ~(MCI_WAIT + MCI_NOTIFY );

   /* If the caller passed in a flag we do not support--return an error */

   if (ulStopFlags > 0)
       return (MCIERR_INVALID_FLAG);

   ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;

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

   if (ulAbortNotify == TRUE)
     {
     if ( ulpInstance->usNotPendingMsg == MCI_SAVE )
        {
        /*****************************************************
        * Save cannot be interrupted or data will be lost, so
        * if there is a save pending, wait for it to complete
        ******************************************************/

        DosWaitEventSem( ulpInstance->hThreadSem, (ULONG ) -1 );

        }
     else
        {
        PostMDMMessage ( MCI_NOTIFY_ABORTED,
                         ulpInstance->usNotPendingMsg,
                         pFuncBlock);

        DosResetEventSem (ulpInstance->hEventSem, &ulCnt);
        DosResetEventSem (ulpInstance->hThreadSem, &ulCnt);

        /**********************************************
        * If we are recording information, we must do
        * a full stop so that no information will be
        * lost.  However, if this is a play back stream
        * do a pause so that the next play will resume
        * without audible breakup of information.
        ***********************************************/


        if ( ulpInstance->ulOperation == MCIDRV_INPUT )
          {
          ulSpiFlags = SPI_STOP_FLUSH;
          }
        else
          {
          ulSpiFlags = SPI_STOP_STREAM;
          }

        /***********************************
        * Stop The Stream
        ************************************/
        ulrc = ADMCStopStream (ulpInstance, ulSpiFlags);

        if ( ulpInstance->ulOperation == MCIDRV_INPUT )
          {
          if (!ulrc)
             {
             /*****************************
             * Wait for the stopped event
             ******************************/
             DosWaitEventSem (ulpInstance->hThreadSem, (ULONG) -1);
             }

          /****************************************
          * Record streams go into a stopped state
          *****************************************/
          STRMSTATE = MCI_STOP;

          }
        else
          {
          /*****************************************
          * Since a pause does not generate an event
          * create a fake one so our play thread can
          * clean up (if it is active).
          *****************************************/

          ulpInstance->StreamEvent = EVENT_STREAM_STOPPED;
          DosPostEventSem (ulpInstance->hEventSem);

          DosWaitEventSem (ulpInstance->hThreadSem, (ULONG) -1);

          /***************************************
          * Play streams go into a stopped/paused state
          ****************************************/

          STRMSTATE = STOP_PAUSED;
          }


        } /* if no pending save */

        ulpInstance->usNotifyPending = FALSE;

     } /* if notify pending */


   if (STRMSTATE != MCI_STOP && STRMSTATE != STOP_PAUSED )
      {
      DosResetEventSem (ulpInstance->hEventSem, &ulCnt);
      ADMCStopStream (ulpInstance, SPI_STOP_STREAM );
      STRMSTATE = STOP_PAUSED;
      }

   return (MCIERR_SUCCESS);

}


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIPAUS.C
*
* DESCRIPTIVE
*
* FUNCTION:  Waveform Pause.
*
* NOTES:
*  On a MCI_PAUSE, a streaming MCD should perform the following commands:
*
*  A. Ensure that no flags are passed in.
*  B. If we are currently streaming, pause the stream.
*  C. Set flag indicating that we are in paused state.
*
* ENTRY POINTS:
*
* INPUT: MCI_PAUSE message.
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
RC MCIPaus (FUNCTION_PARM_BLOCK *pFuncBlock)

{
    INSTANCE *      ulpInstance;
    ULONG           ulPauseFlags;

    ulPauseFlags = pFuncBlock->ulParam1;
    ulPauseFlags &= ~(MCI_WAIT + MCI_NOTIFY );

   /* If the caller passed in a flag we do not support--return an error */

    if (ulPauseFlags > 0 )
       {
       return MCIERR_INVALID_FLAG;
       }

    ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

    /****************************************************
    * The default action on an spiStopstream is pause.
    *****************************************************/
    if ((STRMSTATE == MCI_PLAY ) || (STRMSTATE == MCI_RECORD) )
       {

       ADMCStopStream (ulpInstance, SPI_STOP_STREAM);

       STRMSTATE = MCI_PAUSE;    /* update Instance */

       }

    return (MCIERR_SUCCESS);

} /* MCIPause */



/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIRESUME
*
* DESCRIPTIVE      Audio MCD Resume.
*
* FUNCTION:  Resume Playback/Record from Paused State.
*
* NOTES:
* On a MCI_RESUME, a streaming MCD should perform the following commands:
*
* A. Ensure that no flags are passed in.
* B. If we are paused, resume the stream.
*
* ENTRY POINTS:
*
* INPUT: MCI_RESUME message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES: SpiStartStream ()        -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/
RC  MCIResume (FUNCTION_PARM_BLOCK * pFuncBlock)
{

   ULONG                  ulResumeFlags;   // Incoming MCI Flags

   INSTANCE               * ulpInstance;   // Local Instance

   /*********************************
   * Check for invalid flags
   *********************************/

   ulResumeFlags = pFuncBlock->ulParam1;

   /* Mask out legal flags */

   ulResumeFlags &= ~( MCI_WAIT + MCI_NOTIFY );

   /* If the caller passed in a flag we do not support--return an error */

   if (ulResumeFlags > 0 )
           return ( MCIERR_INVALID_FLAG );

   /**********************************
   * Derefernce pointers.
   ***********************************/

   ulpInstance= (INSTANCE *) pFuncBlock->ulpInstance;


   /**********************************************************
   * No State Transition if already in States Play or Record
   **********************************************************/

   if ( STRMSTATE == MCI_PAUSE )
      {
// CONNECTOR FEATURE--need to add resume

         StartStream( ulpInstance, MCIDRV_START_PLAYBACK ); // MCIDRV_RESUME not in spec

//       SpiStartStream (ulpInstance->StreamInfo.hStream, SPI_START_STREAM);
// CONNECTOR FEATURE

       if ( ulpInstance->ulOperation == MCIDRV_OUTPUT )
          {
          STRMSTATE = MCI_PLAY;      // update Instance
          }
       else
          {
          STRMSTATE = MCI_RECORD;
          }

      } /* if in paused state */

   return (MCIERR_SUCCESS);

} /* MCIResume */




/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISTPA
*
* DESCRIPTIVE : Audio MCD Set Position Advice.
*
* FUNCTION: Create Media Position Advice Notification messages.
*
* NOTES:
*  To a streaming MCD, a postion advise simply is a cuepoint which
*  reoccurs every x time units.  To enable position advise, the following
*  steps should be followed:
.*
.
*  Check flags and validate and pointers.
*  If the caller has asked for position advise to be turned on then
*
*    If a stream has been created, then enable the recurring cuepoint event.
*    Else, set a flag and enable the event later.
*  If position advise is to be turned off, then disable the recurring
*  cuepoint event.
*
* ENTRY POINTS:
*
* INPUT: MCI_SET_POSITION_ADVISE message.
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES: spiStartStream ()         -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/

RC  MCISetPositionAdvise (FUNCTION_PARM_BLOCK * pFuncBlock)
{
  ULONG           ulrc;                      // Error Value
  ULONG           ulParam1;                  // Incoming MCI Flags
  ULONG           ulPosAdviseFlags;          // Mask For Incoming Flags
  ULONG           ulMMTime;                  // Converted Time

  INSTANCE        *ulpInstance;              // Local Instance

  PID             pid;
  TID             tid;
  HWND            hWnd;

  PMCI_POSITION_PARMS pPositionParms;      // MCI Msg Data

  /***************************
  * Intialize Pointers
  ****************************/

  ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;
  ulParam1 = pFuncBlock->ulParam1;
  ulPosAdviseFlags = ulParam1;
  pPositionParms = (PMCI_POSITION_PARMS) pFuncBlock->ulParam2;

  /**********************************
  * Check for Validity of Flags
  ***********************************/
  ulPosAdviseFlags &= ~( MCI_NOTIFY  + MCI_WAIT     +
                         MCI_SET_POSITION_ADVISE_ON +
                         MCI_SET_POSITION_ADVISE_OFF);

  /*************************************
  * Return error if flags are not
  * appropriate for the message
  *************************************/

  if (ulPosAdviseFlags > 0 )
      return ( MCIERR_INVALID_FLAG );

  /********************************
  * Null Parameter Conditions
  *********************************/
  ulrc = CheckMem (((PVOID)pPositionParms),
                   sizeof (MCI_POSITION_PARMS),
                   PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
     {
     return (MCIERR_MISSING_PARAMETER);
     }



  /******************************************************
  * Check for Invalid Combination of Valid Flags
  *******************************************************/
  if ((ulParam1 & MCI_SET_POSITION_ADVISE_ON) &&
      (ulParam1 & MCI_SET_POSITION_ADVISE_OFF))

      return (MCIERR_FLAGS_NOT_COMPATIBLE);

  /************************************************
  * Check for Invalid Combination of Valid Flags
  *************************************************/
  if (!(ulParam1 & MCI_SET_POSITION_ADVISE_ON ||
      ulParam1 & MCI_SET_POSITION_ADVISE_OFF))

      return (MCIERR_MISSING_FLAG);


  /*---------------------------------------
  * To enable position advise, a file or
  * playlist must be loaded.
  *---------------------------------------*/

  if (ulpInstance->fFileExists == FALSE )
    return (MCIERR_FILE_NOT_FOUND);

  /******************************************
  * Enable Position Advise
  *******************************************/

  if (ulParam1 & MCI_SET_POSITION_ADVISE_ON)
     {
     /************************************
     * Ensure that the callback handle that
     * the caller specified is valid.  This case
     * is somewhat special--MDM normally checks
     * the callback handles when MCI_NOTIFY is
     * specified.  However, callback handles can
     * be passed on position advise with the
     * MCI_WAIT flag.
     ************************************/

     hWnd = pPositionParms->hwndCallback;


     if (!WinQueryWindowProcess(hWnd, &pid, &tid))
          return(MCIERR_INVALID_CALLBACK_HANDLE);

     ulpInstance->StreamInfo.PosAdvEvcb.hwndCallback = hWnd;

     ulpInstance->StreamInfo.PosAdvEvcb.usDeviceID = ulpInstance->usWaveDeviceID;

     /**************************************************
     * If we have an existing stream and have already
     * enabled a position advise cuepoint, then
     * disable the previous position advise since the
     * new one has precendence.
     **************************************************/

     if ( ulpInstance->usPosAdvise == EVENT_ENABLED )

        {

        /* why don't we check ulrc? */

        ulrc = ADMCDisableEvent(ulpInstance->StreamInfo.hPosEvent);
        }

     /***************************************
     * Fill in Event Control Block to indicate
     * that we want a recurring cue point.
     ****************************************/
     ulpInstance->StreamInfo.PosAdvEvcb.evcb.ulType = EVENT_CUE_TIME;
     ulpInstance->StreamInfo.PosAdvEvcb.evcb.ulFlags = EVENT_RECURRING;
     ulpInstance->StreamInfo.PosAdvEvcb.evcb.hstream = ulpInstance->StreamInfo.hStream;

     /************************************************
     * Copy our instance into the position advise
     * event control block.  The event routine will
     * use this field to obtain a pointer to our
     * instance.  Once it has this pointer, it can
     * manipulate instance variables etc, necessary
     * to inform the caller that the event has
     * happened.  See the PlayEventProc in admcplay.c
     * and the RecordEventProc in admcrecd.c
     ***********************************************
     **/
     ulpInstance->StreamInfo.PosAdvEvcb.ulpInstance = (ULONG)ulpInstance;

     /********************************************
     * Copy Position UserParm into instance.
     * This User Parameter is returned on
     * Position change messages to the user.
     *********************************************/
     ulpInstance->usPosUserParm = pPositionParms->usUserParm;

     ulrc = ConvertToMM ( ulpInstance,
                          &ulMMTime,
                          pPositionParms->ulUnits);

     /**********************************************
     * Frequency of Position Change Messages
     **********************************************/

     ulpInstance->StreamInfo.PosAdvEvcb.evcb.mmtimeStream = ulpInstance->StreamInfo.PosAdvEvcb.mmCuePt = ulMMTime;

     /*********************************************
     * If ulUnits == 0 return MCIERR_OUTOFRANGE
     *********************************************/

     if ( ulMMTime == 0)
         return (MCIERR_OUTOFRANGE);

     /*********************************************
     * Enable the recurring time event.  Note:
     * We are utilizing an extended event control
     * block which we modified to contain window
     * handles, instance pointers etc.  In fact,
     * you can modify the evcb to contain whatever
     * data you wish.  Simply ensure the beginning
     * fields of the structure are what SSM expects
     * and the remainder can be used by the MCD.
     * See admcplay.c or audiosub.c for more info
     **********************************************/

     if (ulpInstance->ulCreateFlag == PREROLL_STATE)
       {
       ulrc = ADMCEnableEvent( (PEVCB) &ulpInstance->StreamInfo.PosAdvEvcb,
                              (PHEVENT) &ulpInstance->StreamInfo.hPosEvent);

       ulpInstance->usPosAdvise = EVENT_ENABLED; // Set The Flag and Return
       }
     else
       {
       ulpInstance->usPosAdvise = TRUE; // Set The Flag and Return
       }

     if (ulrc)
         return (ulrc);
     } /* Set Position Advise on */

  /*****************************************
  * Disable Position Advise
  ******************************************/

  if (ulParam1 & MCI_SET_POSITION_ADVISE_OFF)
     {
     /* Ensure that position advise has been turned on first */
     if ( !ulpInstance->usPosAdvise )
        {
        return ( MCIERR_INVALID_CUEPOINT );
        }
     /* If an event has been enabled, remove it */

     if ( ulpInstance->usPosAdvise == EVENT_ENABLED )
        {
        ulrc = ADMCDisableEvent (ulpInstance->StreamInfo.hPosEvent);
        }

     /* Set flag indicating that Position Advise is no longer in effect */

     ulpInstance->usPosAdvise = FALSE;

     } /* Set position advise off */

  return (ulrc);    // return RC
} /* MCISetPositionAdvise */


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISYNC
*
* DESCRIPTIVE      Audio MCD Set Sync Offset.
*
* FUNCTION:  Set Synchronization offset.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_SET_SYNC_OFFSET message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:  MCIERR ().
*
* EXTERNAL REFERENCES: spiStartStream ()         -   SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/
//RC MCISync (FUNCTION_PARM_BLOCK * pFuncBlock)
//{
//  INSTANCE             *pInstance;       // Local Instance
//  PMCIDRV_SYNC_PARMS   pSyncParms;
//
//  pInstance = (INSTANCE *) pFuncBlock->ulpInstance;
//  pSyncParms = ( PMCIDRV_SYNC_PARMS ) pFuncBlock->ulParam2;
//
//  pFuncBlock->ulParam1 &= ~(MCI_WAIT | MCI_NOTIFY );
//
//  switch ( pFuncBlock->ulParam1 )
//     {
//     case MCIDRV_SYNC_ENABLE  :
//        if ( !pInstance->StreamInfo.hStream )
//           {
//           return ( MCIERR_UNSUPPORTED_FUNCTION );
//           }
//        pSyncParms->ulNumStreams = 1;
//        pSyncParms->hStreams = &pInstance->StreamInfo.hStream;
//        break;
//     case MCIDRV_SYNC_DISABLE :
//        break;
//     default :
//        return ( MCIERR_UNSUPPORTED_FUNCTION );
//     }
//  /***********************************************
//  * Flag the offset condition as true.
//  ***********************************************/
//
//  return (MCIERR_SUCCESS);
//        // Forced RC
//}


/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME:              MCDINFO
*
* DESCRIPTIVE NAME: Information about Waveform Device.
*
* FUNCTION: Obtain Info about a  Waveform Device.
*
* NOTES:
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT: MCI_INFO message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/

RC MCIInfo (FUNCTION_PARM_BLOCK *pFuncBlock)
{

  ULONG            ulrc;               // Propogated MME Error Value
  ULONG            ulParam1;           // Incoming MCI Msg Flags
  ULONG            ulInfoFlags;        // Mask for Incoming Flags
  INSTANCE         *ulpInstance;       // Local Instance
  PMCI_INFO_PARMS  pInfoParms;        // Msg Data Ptr

  ulParam1 = pFuncBlock->ulParam1 & NOTIFY_MASK;
  pInfoParms = (PMCI_INFO_PARMS) pFuncBlock->ulParam2;

  ulInfoFlags = ulParam1;

  /*******************************************
  * Turn off Expected Flags Bits
  ********************************************/
  ulInfoFlags &= ~( MCI_INFO_FILE + MCI_INFO_PRODUCT);

  /************************************************
  * Return Error if any bits are still set
  ************************************************/

  if (ulInfoFlags > 0 )
      return (MCIERR_INVALID_FLAG);

  /****************************************************
  * Check For Valid Flags but Invalid combination
  *****************************************************/

  if (ulParam1 & MCI_INFO_FILE && ulParam1 & MCI_INFO_PRODUCT)
      return (MCIERR_FLAGS_NOT_COMPATIBLE);


  if (!(ulParam1 & MCI_INFO_FILE || ulParam1 & MCI_INFO_PRODUCT))
      return (MCIERR_MISSING_FLAG);

  /******************************************************
  * Derefernce Instance pointer from the function block
  ******************************************************/
  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

  /*************************************************
  * Check For valid MCI Data Struct pointer
  *************************************************/
  if (ulrc = CheckMem ((PVOID)pInfoParms,
                       sizeof (MCI_INFO_PARMS),
                       PAG_READ | PAG_WRITE) )
      {
      return (MCIERR_MISSING_PARAMETER);
      }


  if (ulParam1 & MCI_INFO_FILE)
      {
      /* An element must be loaded before this is valid */

      if (ulpInstance->fFileExists == FALSE )
          {
          return ( MCIERR_FILE_NOT_FOUND );
          }

      /************************************
      * Ensure the size of the buffer the
      * user passed is valid for the length
      * the caller says it is (i.e. they may
      * have said it's valid for 8 bytes).
      *************************************/

       if (ulrc = CheckMem ((PVOID)pInfoParms->pszReturn,
                            pInfoParms->ulRetSize,
                            PAG_READ | PAG_WRITE) )

         {
         return (MCIERR_INVALID_BUFFER);
         }
      /********************************************
      * If the filename is longer that what the
      * caller has allocated, return the length
      * that they should have allocated for the next
      * try.
      ***********************************************/

      if (strlen(ulpInstance->pszAudioFile) > pInfoParms->ulRetSize)
        {
        pInfoParms->ulRetSize = strlen (ulpInstance->pszAudioFile);
        return (MCIERR_INVALID_BUFFER);
        }
      else
        {
        /* return to the caller the filename */

        strcpy((PSZ)pInfoParms->pszReturn, ulpInstance->pszAudioFile);
        }

      } /* If File Info was Requested */

  /**********************************
  * Product Information
  ***********************************/

  if (ulParam1 & MCI_INFO_PRODUCT)
      {

      /*******************************
      * Get Product Information
      * From AudioIF which is the
      * device specific component.
      ********************************/

       ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                               MCI_INFO,
                               ulParam1,
                               (PVOID) pInfoParms,
                               pFuncBlock->usUserParm);

      }  /* INFO Product */

  return (ulrc);
} /* MCIInfo */

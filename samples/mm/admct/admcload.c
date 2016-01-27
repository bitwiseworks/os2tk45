/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCILOAD.C
*
* DESCRIPTIVE NAME: Audio MCD Load Element Routine.
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:Load an Waveform Element.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_LOAD message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:    CreateNAssocStream ().
*                         DestroyStream().
*                         SetAudioDevice().
*                         VSDInstToWaveSetParms().
*                         OpenFile().
*
* EXTERNAL REFERENCES:    SpiStopStream  ().
*                         SpiAssociate   ().
*                         SpiDisableEvent().
*                         SpiSeekStream  ().
*
*********************** END OF SPECIFICATIONS **********************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <string.h>
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // Mci Driver Include.
#include <mcipriv.h>                    // MCI Connection stuff
#include <mcd.h>                        // VSDIDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes
#include <sw.h>
#include <checkmem.h>

/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCILOAD.C
*
* DESCRIPTIVE NAME: Audio MCD Load Element Routine.
*
* FUNCTION:Load an Waveform Element.
*
* NOTES: The following concepts are illustrated in this file.
*  A. How to check flags on a load.
*  B. How to stop any commands which are in process.
*  C. Why cuepoints/positionadvises need to be turned off on a MCI_LOAD.
*  D. Handling OPEN_MMIO on an MCI_LOAD.
*  E. Why reassociation of a stream on MCI_LOAD is desirable and
*     when it is appropriate.
*
* ENTRY POINTS:
*
* INPUT: MCI_LOAD message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:DestroyStream(), ReadRIFFWaveHeaderInfo()
*                     SetAmpDefaults(), SetWaveDeviceDefaults().
*                     SetAudioDevice().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/


RC MCILoad ( FUNCTION_PARM_BLOCK *pFuncBlock)
{

  ULONG                ulrc;                // MME Propogated Error RC
  ULONG                ulHoldError;         /* Hold status of errors */
  ULONG                ulParam1;            // Incoming MCI Flags
  ULONG                ulAbortNotify = FALSE;// indicates whether to abort a previous
  ULONG                ulOldMode;           // indicates if stream must be destroyed
                                            // or reassociated
  ULONG                ulOldDataType;       // ditto--see comments below.
                                            // operation
  ULONG                ulOldBPS;
  ULONG                ulOldSampRate;
  ULONG                ulOldChannels;

  ULONG                ulLoadFlags;         // Incoming Flags Mask
//  ULONG                ulSetAll;            //  Reinit AudioIF

  BOOL                 fMustReinit = FALSE;
  INSTANCE             *ulpInstance;        // Local Instance//

  PMCI_LOAD_PARMS      pLoadParms;         // App MCI Data Struct
//  MCI_WAVE_SET_PARMS   SetParms;            // App MCI Data Struct

//  MCI_AMP_INSTANCE     BackupAmp;           // Hold old amp values

//  extern HHUGEHEAP     heap;                // Global MCD Heap
  extern HID           hidASource;          // hid's for FSSH
  extern HID           hidATarget;          // "     "      "

  MTIME_EVCB           *pMCuePtEVCB;
  MTIME_EVCB           *pTempCuePtEVCB;



  /**************************************
  * Dereference Pointers
  **************************************/
  ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;
  ulParam1 = pFuncBlock->ulParam1;
  ulParam1 &= ~MCI_OPEN_ELEMENT;
  ulLoadFlags = ulParam1;

  /*---------------------------------------------------------
  * Make a copy of the amp/mixer instance in case any errors
  * happened.
  *---------------------------------------------------------*/

//  memmove( &BackupAmp, &AMPMIX, sizeof( MCI_AMP_INSTANCE ) );


  /**************************************
  * Mask UnWanted Bits
  **************************************/
  ulLoadFlags &= ~(MCI_NOTIFY + MCI_WAIT + MCI_OPEN_MMIO + MCI_READONLY );


//  ulSetAll = MCI_WAVE_SET_BITSPERSAMPLE | MCI_WAVE_SET_FORMATTAG     |
//             MCI_WAVE_SET_CHANNELS      | MCI_WAVE_SET_SAMPLESPERSEC;


  if ( ulLoadFlags > 0 )
     {

     if ( ulParam1 & MCI_OPEN_PLAYLIST)
        {
        return ( MCIERR_UNSUPPORTED_FLAG );
        }

     return ( MCIERR_INVALID_FLAG );
     }



  /****************************************
  * Check For Valid Parameters and memory
  ****************************************/

  pLoadParms = (PMCI_LOAD_PARMS) pFuncBlock->ulParam2;

  ulrc = CheckMem ( (PVOID)pLoadParms,
                    sizeof (MCI_LOAD_PARMS),
                    PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
          return ( MCIERR_MISSING_PARAMETER );


  /*-------------------------------------------
  * If the caller used a readonly flag and
  * there was no filename, this is an
  * invalid combination.
  *------------------------------------------*/

  if ( ulParam1 & MCI_READONLY &&
       !pLoadParms->pszElementName )
     {
     return ( MCIERR_MISSING_PARAMETER );
     }

  /******************************************
  * If these variables change during the
  * load, then the stream must be destroyed
  * rather than reassociated since SSM's
  * buffer sizes can easily change.
  *******************************************/

// 6421--don't use VSD instance--maintain own local copy.

//  ulOldMode     = AMPMIX.ulOperation;
//  ulOldBPS      = AMPMIX.lBitsPerSRate;
//  ulOldSampRate = AMPMIX.lSRate;
//  ulOldDataType = AMPMIX.sMode;
//  ulOldChannels = AMPMIX.sChannels;

  ulOldMode     = ulpInstance->ulOperation;
  ulOldBPS      = WAVEHDR.usBitsPerSample;
  ulOldSampRate = WAVEHDR.ulSamplesPerSec;
  ulOldDataType = WAVEHDR.usFormatTag;
  ulOldChannels = WAVEHDR.usChannels;

  /****************************************************
  * Get the semaphore which allows us to abort pending
  * plays and records.
  ****************************************************/

  GetNotifyAbortAccess ( ulpInstance, &ulAbortNotify );

  /****************************************************
  * Interrupt any plays/records/saves which are pending
  * so the load can continue
  ****************************************************/

  ulrc = LoadAbortNotifies( ulpInstance, pFuncBlock, ulAbortNotify );

  if ( ulrc )
     {
     return ( ulrc );
     }

  ulpInstance->ulOldStreamPos  = 0;


  /********************************************
  * If the user has enabled cuepoints discard
  * them since this is a new file.  If we do not
  * do this, then cuepoints could pop up in the
  * new file which were set in the old file.
  ********************************************/

  if ((ulpInstance->usCuePt == TRUE) || (ulpInstance->usCuePt == EVENT_ENABLED))
     {
     pMCuePtEVCB = CUEPOINT;

     while ( pMCuePtEVCB )
        {
        if (ulpInstance->ulCreateFlag == PREROLL_STATE)
            {
            ADMCDisableEvent (pMCuePtEVCB->HCuePtHndl );
            }
        pTempCuePtEVCB = pMCuePtEVCB;
        pMCuePtEVCB = pMCuePtEVCB->pNextEVCB;
        CleanUp( pTempCuePtEVCB );
        } /* while there are cue points to remove */

     CUEPOINT = NULL;

     } /* if we have cue points in use */


  /****************************************************
  * Disable Position Advise (i.e. the user may have
  * turned on position advise in the previous stream
  * and since we can possibly reusing the stream, turn
  * off position advise.
  ****************************************************/

// why is there two states for this flag--why not just one?

  if ( (ulpInstance->usPosAdvise == TRUE) ||
       (ulpInstance->usPosAdvise == EVENT_ENABLED))
      {
      if (ulpInstance->ulCreateFlag == PREROLL_STATE)

          ADMCDisableEvent (ulpInstance->StreamInfo.hPosEvent);

      ulpInstance->usPosAdvise = FALSE;
      }

  /* Remove any undo nodes if they were allocated */

  RemoveUndoNodes( ulpInstance );

  /************************************************
  * If a file was previously opened then close it
  * (however, if it was opened with the OPEN_MMIO
  * flag, then don't close it.  That is the
  * applications responsibility).
  ************************************************/

  ulrc = CloseFile( ulpInstance );


  /**************************************************
  * Check for valid element names -- ONLY if the user
  * passed in a filename, not a file handle
  **************************************************/

  if ( !( ulParam1 & MCI_OPEN_MMIO )  )
     {

     ulrc = CheckForValidElement( ulpInstance,
                                  pLoadParms->pszElementName,
                                  ulParam1 );

     if ( ulrc )
        {
        ulpInstance->fFileExists = FALSE;
        return ( ulrc );
        }

     } /* if the user did not pass in a file handle instead of a file name */




  /******************************************************
  * If hmmio is passed in just update instance copy of
  * hmmio, reset filexists flag and return success
  *****************************************************/

  ulrc = OpenHandle( ulpInstance,
                     ulParam1,
                     (HMMIO) pLoadParms->pszElementName);

  if ( ulrc )
     {
     /* Set a flag to indicate that there no longer is a valid file loaded */

     ulpInstance->fFileExists = TRUE;
     return ( ulrc );
     }

  /**************************************************************
  * Temporary File creation Flags.  If the io proc cannot
  * perform temporary changes or the user specifically requested
  * that the file was opened read only then do not try to
  * perform temporary changes on the file (both of those actions
  * require the ability to write).
  ***************************************************************/

  if ( ulParam1 & MCI_READONLY )
     {
     ulpInstance->ulOpenTemp = MCI_FALSE;
     }
  else
     {
     ulpInstance->ulOpenTemp = MCI_TRUE;
     }


  ulpInstance->ulUsingTemp = MCI_FALSE;
  /* Set flag to indicate no codec is active */

  ulpInstance->ulRealTimeTranslation = MMIO_NONREALTIME;


  if ( !( ulParam1 & MCI_OPEN_MMIO ) )

      {
      ulrc = ProcessElement( ulpInstance, ulParam1, MCI_LOAD );

      if ( ulrc )
         {
         /* If processelements fails, it sets fFileExists to false */

         return ( ulrc );
         }

      } /* Element Specified on Load */


  /*******************************************
  * Copy audio device attributes into MCI
  * Wave Set structure
  ********************************************/
// 6421--no longer needed
//  VSDInstToWaveSetParms ( &SetParms, ulpInstance);

  /**********************************************
  * Set the Audio device attributes.
  * ulSetAll is a flag set to waveaudio extensions
  *************************************************/

  /**************************************************
  * If the audio card switched modes (i.e. from play
  * to record) because of the file load, then the
  * stream must be destroyed and there is no
  **************************************************/

//  if ( ulOldMode     != AMPMIX.ulOperation     ||   WAVEHDR.usBitsPerSample
//       ulOldDataType != ( ULONG ) AMPMIX.sMode ||   WAVEHDR.ulSamplesPerSec
//       ulOldBPS      != AMPMIX.lBitsPerSRate   ||   ;
//       ulOldSampRate != AMPMIX.lSRate          ||   WAVEHDR.usChannels;
//       ulOldChannels != AMPMIX.sChannels )

  if ( ulOldMode     != ulpInstance->ulOperation ||
       ulOldDataType != WAVEHDR.usFormatTag      ||
       ulOldBPS      != WAVEHDR.usBitsPerSample  ||
       ulOldSampRate != WAVEHDR.ulSamplesPerSec  ||
       ulOldChannels != WAVEHDR.usChannels )
     {
     fMustReinit = TRUE;

     /******************************************
     * Determine if the network can go into the
     * mode that matches the file we just loaded.
     ********************************************/
     ulrc = ModifyConnection( ulpInstance );
// CONNECTION FEATURE
//     ulrc = SetAudioDevice (ulpInstance, &SetParms, ulSetAll);
// CONNECTION FEATURE
     if (ulrc)

        {
        ulHoldError = ulrc;
#ifndef CONNECTION

        if ( ulrc == MCIERR_UNSUPP_FORMAT_TAG )
           {
           /******************************************
           * Connected device will update the the audio
           * header to reflect the mode that the device
           * should be opened in.
           ********************************************/
           ulrc = ModifyConnection( ulpInstance );
           }

#endif
        if ( ulrc )
           {
           /* Dont leave the file open */

           CloseFile( ulpInstance );

           /* Set a flag to indicate that there no longer is a valid file loaded */

           ulpInstance->fFileExists = FALSE;

           /* Ensure that our instance remains the same as before the load attempt */

           return ( ulHoldError );
           } /* if error on 2nd call */

        ulpInstance->ulRealTimeTranslation = MMIO_REALTIME;

        } /* If error on modify connection */
     else
        {
        ulpInstance->ulRealTimeTranslation = MMIO_NONREALTIME;
        }

     }
  else
     {
     fMustReinit = FALSE;
     }

  /*************************************************************
  * If a stream has previously been created, it will be simpler
  * and quicker to just reassociate the stream handlers.
  * reassociation requires that the stream be stoped however
  * before continuing.
  *************************************************************/

  if (ulpInstance->ulCreateFlag == PREROLL_STATE)
     {
     /**************************************************
     * If the audio card switched modes (i.e. from play
     * to record) because of the file load, then the
     * stream must be destroyed and there is no
     * way to reassociate it.  In addition, reassociation
     * is only possible if the data type, bits/sample
     * sampling rate and channels remain the same.  If
     * you have a non-audio streaming MCD, then reassociation
     * is only possible when the data type and subtype
     * remain the same.
     *
     * We also must recreate the stream if we had
     * previously installed a protocol with spiInstallProtocol
     **************************************************/

     if ( fMustReinit || ulpInstance->fInstalledProtocol )

         {
         /**********************************************
         * Destroy The Stream -- no hope to reassociate
         ***********************************************/
// CONNECTION FEATURE--what do I do with this???????
#ifndef CONNECTION
         DestroyStream ( ulpInstance);
#endif
// CONNECTION FEATURE--what do I do with this???????

         /*************************************
         * Set Stream Creation Flag-- so that
         * following routines know to create
         * the stream
         **************************************/

         ulpInstance->ulCreateFlag = CREATE_STATE;

         }
     else
         {
         /*************************************
         * Place the stream in a stopped state
         * which is required for reassociation.
         *************************************/
// CONNECTOR FEATURE
         ulrc = StopStream2( ulpInstance, MCIDRV_DISCARD_STREAM_NETWORK );
//         ulrc = ADMCStopStream (ulpInstance->StreamInfo.hStream, SPI_STOP_DISCARD );
//
//         if (!ulrc)
//            {
//            DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1 );
//            }
         } /* the audio card did not switch modes */
// CONNECTOR FEATURE

     }  /* Stream in PreRoll (started) State */



  /***********************************************************
  * perform this after the open because open file will modify
  * the state of the following flags
  ***********************************************************/

  if ( ulParam1 & MCI_READONLY )
     {
     ulpInstance->ulCapabilities &= ~( CAN_SAVE | CAN_RECORD );
     }


  /*****************************************************
  * Currently return true if a playlst strm was created
  *****************************************************/
  if (ulpInstance->usPlayLstStrm == TRUE)
      {
      ulpInstance->StreamInfo.hidASource = hidASource;
      ulpInstance->StreamInfo.hidATarget = hidATarget;

      ulpInstance->usPlayLstStrm = FALSE;

      }  /* Trash the old Stream Handler Handles */

  /*************************************************
  * Reassociate The Stream Handlers with the new
  * stream object if the stream has been created
  * in the correct direction already.
  *************************************************/
  if (ulpInstance->ulCreateFlag == PREROLL_STATE)
      {
      /*********************************************
      * Fill in Associate Control Block Info for
      * file system stream handler (FSSH).  FSSH will
      * use the mmio handle we are associating to
      * stream information.
      *********************************************/
      ulpInstance->StreamInfo.acbmmio.ulObjType = ACBTYPE_MMIO;
      ulpInstance->StreamInfo.acbmmio.ulACBLen = sizeof (ACB_MMIO);
      ulpInstance->StreamInfo.acbmmio.hmmio = ulpInstance->hmmio;

      /***********************************************
      * Associate FileSystem as source if Playing. Note
      * the association is always done with the file
      * system stream handler since it is invovled with
      * mmio operations.  If you try this with the
      * audio stream handler, you will get invalid
      * handle back.
      ***********************************************/

      if (ulpInstance->ulOperation == MCIDRV_OUTPUT )
         {
         ulrc = ADMCAssociate ( ulpInstance->StreamInfo.hStream,
                               ulpInstance->StreamInfo.hidASource,
                               (PVOID) &ulpInstance->StreamInfo.acbmmio );
         }
      /***********************************************
      * Associate FileSystem as target if recording
      ***********************************************/

      else
         {
         ulrc = ADMCAssociate ( ulpInstance->StreamInfo.hStream,
                               ulpInstance->StreamInfo.hidATarget,
                               (PVOID) &ulpInstance->StreamInfo.acbmmio );

         } /* else we are in record mode */

      /* If the SpiAssociate failed, return an error */

      if ( ulrc )
         {
         return ( ulrc );
         }

         /******************************************************************
         * We need to seek to 0 to reset the target stream handlers position
         * since an associate DOES NOT reset the current stream's time.
         *******************************************************************/

         ulrc = ADMCSeekStream ( ulpInstance,
                                SPI_SEEK_ABSOLUTE,
                                0L );

         if (ulrc)
            {
            return (ulrc);
            }

      } /* Preoll State Flag */

  /****************************************************
  * If the user did not pass in the read only flag then
  * tell the io proc that we have opened to to use temp
  * files (if it can).  Temp files will allow the user
  * to abort changes.
  *****************************************************/

  if ( ulpInstance->ulOpenTemp )
    {

    ulrc = SetupTempFiles( ulpInstance, ulParam1 );

    if ( ulrc )
       {
       ulpInstance->fFileExists = FALSE;
       return ( ulrc );
       }
    }


  /****************************************************
  * Inform the io proc whether or not the data should
  * be translated real-time or not.
  *****************************************************/

// ignore error--io procs may not support this
// CONNECTOR FEATURE
#ifndef CONNECTION
  ulpInstance->ulCodecDescription = MAKE_SOURCE | SOURCE_CODEC;

  ulrc = DataTranslation( ulpInstance );
  if ( ulrc )
     {
     ulpInstance->fFileExists = FALSE;
     return ( ulHoldError );
     }
#endif
// CONNECTOR FEATURE


  /********************************************
  * We will say that the stream is in a stopped
  * state so that subsequent operations know
  * that it will be safe to do operations on
  * the stream--such as a cue or a seek.
  ********************************************/

  STRMSTATE = MCI_STOP;

  /*---------------------------------------------------
  * If the file is readonly, we are guaranteed that
  * it will not change.  In addition, if the file
  * is small, SSM may be allocating too many buffers
  * (since they are unaware of the file size).
  * Check to see if we can reduce memory consumption.
  *--------------------------------------------------*/

  if ( ulParam1 & MCI_READONLY )
     {
     InstallProtocol( ulpInstance );
     }


  return (ulrc);

} /* MCILoad */







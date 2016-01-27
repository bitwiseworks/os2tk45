/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISTAT.C
*
* DESCRIPTIVE NAME: Audio MCD status routine
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: Get the status of several variables.
*
* NOTES: This source file illustrates the following concepts:
*        A. When/If to report media position within a stream.
*        B. How to determine the length of an existing file.
*        C. How to determine the length of a file which is currently
*           being recorded.
*        D. Communicating with the amp/mixer to determine volume,
*           and other amplifier specific commands.
*        E. Reporting our current mode.
*        F. Reporting the current time format.
*
*
* INTERNAL REFERENCES:
*                        ConvertTimeUnits ().
*                        ConVertToMM
*                        SetAudioDevice().
*                        SetWaveDeviceDefaults().
*                        CheckMem ().
*
* EXTERNAL REFERENCES:
*                        SpiGetTime       ()        - MME API
*                        mciSendCommand   ()        - MME API
*
*********************** END OF SPECIFICATIONS **********************/


#define INCL_BASE                       // Base Dos APIs.
#define INCL_ERRORS                     // All the errors.
#define INCL_WINCLIPBOARD
#define INCL_WINWINDOWMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#define INCL_WINATOM
#define INCL_HEAP
#define INCL_DOSMEMMGR


#include <os2.h>                        // OS2 includes.
#include <string.h>                     // String Functions
#include <math.h>                       // Math Functions
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // Mci Driver include.
#include <mcd.h>                        // AUDIO IF DriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions.
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include <admcfunc.h>                   // Function Prototypes
#include <checkmem.h>

/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: MCISTAT.C
*
* DESCRIPTIVE NAME: Waveform Status Routine.
*
* FUNCTION: Get Current Status of an Waveform Instance.
*
* NOTES: After the status is obtained from the device specific DLL
*        the corresponding field in the instance structure is updated
*        to reflect the most recent state.
*
* ENTRY POINTS:
*     LINKAGE:   CALL FAR
*
* INPUT:
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: VSDIDriverEntry().
*
* EXTERNAL REFERENCES: DosQueryProcAddr - OS/2 API.
*
*********************** END OF SPECIFICATIONS *******************************/

RC MCIStat (FUNCTION_PARM_BLOCK *pFuncBlock)

{

  ULONG               ulrc;                // Error Value
  ULONG               ulParam1;            // MCI Msg Flags
  ULONG               ulFormatInfo = 0;    // Format of Clipboard data
  INSTANCE*           ulpInstance;         // Local Instance
  ULONG               ulTemp1;             // Temporary Stuff
  ULONG               ulTemp2;             // Temporary Stuff
  ULONG               ulStatFlags;         // Mask For Incoming Flags

  PMCI_STATUS_PARMS   pParams;             // Msg Data Ptr

  HAB            habClipboard;

  /* Derefernce pointers */

  ulParam1    = pFuncBlock->ulParam1;
  pParams     = (PMCI_STATUS_PARMS) pFuncBlock->ulParam2;
  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

  /*********************************************
  * Mask out only flags that this MCD supports.
  **********************************************/
  ulStatFlags = ulParam1;

  ulStatFlags &= ~ (MCI_STATUS_ITEM + MCI_TRACK + MCI_WAIT + MCI_NOTIFY + MCI_STATUS_START );

  /**********************************
  * Return error if caller passed in
  * a flag we do not support.
  ***********************************/

  if (ulStatFlags > 0 )
     {
     return ( MCIERR_INVALID_FLAG );
     }

  /*******************************************
  * The caller is required to pass in valid
  * status parms--verify that this is true.
  ********************************************/

  ulrc = CheckMem ( (PVOID)pParams,
                    sizeof (MCI_STATUS_PARMS),
                    PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
          return ( MCIERR_MISSING_PARAMETER );

  /* This MCD does not support the track flag */

  if (ulParam1 & MCI_TRACK)
      return ( MCIERR_UNSUPPORTED_FLAG );


  /*****************************************************************
  * The waveaudio MCD only supports status requests with the
  * STATUS_ITEM flag set.  All other requests will return an error
  *****************************************************************/

  if (ulParam1 & MCI_STATUS_ITEM )
    {

    switch (pParams->ulItem)
      {
      /* the caller wants to know the current media position */
      case MCI_STATUS_POSITION_IN_TRACK:
      case MCI_STATUS_POSITION:
          {
          if ( ulParam1 & MCI_STATUS_START )
             {
             pParams->ulReturn = 0;  // No Stream Alive
             }
          else
             {
             /********************************************
             * If a stream has been previously created,
             * then determine our position within the
             * stream. If no stream has been created, (i.e.
             * a file has been loaded, however, there has
             * been actions after that.
             **********************************************/
   
             if (ulpInstance->ulCreateFlag != CREATE_STATE )
                {
                STREAM.mmStreamTime = 0;
   
                /********************************************
                * Query the stream for current stream time
                *********************************************/
   
                ulrc = SpiGetTime ( STREAM.hStream,
                                    (PMMTIME) &STREAM.mmStreamTime);
                if (!ulrc)
   
                   /*******************************************
                   * Convert MMTIME units to current time base
                   ********************************************/
                   {
                   ConvertTimeUnits ( ulpInstance,
                                      &ulTemp1,
                                      (ULONG) STREAM.mmStreamTime);
                   }
   
                pParams->ulReturn = (ULONG) ulTemp1;
                }
              else
                {
                pParams->ulReturn = 0;  // No Stream Alive
                }
             } /* not status start */

           ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);

          } /* case STATUS_POSITION */

          break;

      /* The caller wants to know the length of the file */

      case MCI_STATUS_LENGTH:
           {

           /* if we have not loaded a file, it can't have a length! */

           if (ulpInstance->fFileExists == FALSE)
               return (MCIERR_FILE_NOT_FOUND);

           /*************************************************
           * Playlist cannot report lengths, since they can
           * contain infinite loops.
           *************************************************/

           if (ulpInstance->usPlayLstStrm == TRUE)
               return (MCIERR_INDETERMINATE_LENGTH );

           /*********************************************
           * There is a big difference between record and
           * playback lengths.  In the case of play, the
           * file length will not change.  However, if
           * a record is active, the file length could
           * potentially be growing.
           **********************************************/

           if (ulpInstance->ulOperation == MCIDRV_OUTPUT )
              {
              ulrc = GetAudioHeader( ulpInstance, 0  );

              if ( ulrc )
                 {
                 return ( ulrc );
                 }

              /******************************************
              * the function ConvertTimeUnits also
              * returns media element length in the
              * current time units.
              ******************************************/

              ConvertTimeUnits (ulpInstance, &ulTemp1, FILE_LENGTH);

              pParams->ulReturn = ulTemp1;

              } /* the card is in playback mode */

           /* else if the card is in record mode */

           else
              {
              /* Test and see if the stream has been prerolled */

              if (ulpInstance->ulCreateFlag == PREROLL_STATE)
                   {
                   /************************************
                   * Are we currently recording?
                   * (cue could preroll the stream also
                   ************************************/

                   if ( STRMSTATE == MCI_RECORD || STRMSTATE == MCI_PAUSE )
                      {
                      ulrc = SpiGetTime ( STREAM.hStream,
                                          ( PMMTIME)&STREAM.mmStreamTime);

                      if (!ulrc)
                         {
                         ulTemp2 = STREAM.mmStreamTime;
                         }

                      ConvertTimeUnits (ulpInstance, &ulTemp1, ulTemp2);

                      /******************************************
                      * the function ConvertTimeUnits also
                      * returns media element length in the
                      * current time units.
                      ******************************************/
                      ConvertTimeUnits (ulpInstance, &ulTemp2, FILE_LENGTH);

                      /******************************************
                      * if the current record position is smaller
                      * tham the file length, then report the
                      * file length.
                      ******************************************/
                      if ( ulTemp1 < ulTemp2 )
                         {
                         ulTemp1 = ulTemp2;
                         }

                      pParams->ulReturn = ulTemp1;
                      } /* if we are currently recording */
                   else
                      {
                      ulrc = GetAudioHeader( ulpInstance, 0 );

                      if ( ulrc )
                         {
                         return ( ulrc );
                         }

                      /******************************************
                      * the function ConvertTimeUnits also
                      * returns media element length in the
                      * current time units.
                      *******************************************/
                      ConvertTimeUnits (ulpInstance, &ulTemp1, FILE_LENGTH);

                      pParams->ulReturn = ulTemp1;

                      } /* else we not are currently recording */

                   } /* Stream Created and Recording */

              /* The stream is not currently active */
              else
                 {
                 ulrc = GetAudioHeader( ulpInstance, 0  );

                 if ( ulrc )
                    {
                    return ( ulrc );
                    }

                  /******************************************
                  * the function ConvertTimeUnits also
                  * returns media element length in the
                  * current time units.
                  ******************************************/
                  ConvertTimeUnits (ulpInstance, &ulTemp1, FILE_LENGTH);

                  pParams->ulReturn = ulTemp1;

                 } /* else we are not currently recording */

              } /* else we are in record mode */

           ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);

           } /* status length */

           break;



      case MCI_STATUS_CURRENT_TRACK:
      case MCI_STATUS_NUMBER_OF_TRACKS:
             pParams->ulReturn = 1;
             ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);
             break;


      case MCI_STATUS_SPEED_FORMAT:

            return ( MCIERR_UNSUPPORTED_FLAG );

      /******************************************************
      * For status volume, and of the amp commands should
      * go the amp/mixer that we are connected to.
      ******************************************************/

      case MCI_STATUS_VOLUME:
            {
// CONNECTOR FEATURE--check if amp is there
#ifdef CONNECTION
            if ( ulpInstance->fConnected )
#endif
               {
               /********************************
               * It is possbile that although we
               * are connected, there is no amp
               * in the network to forward the
               * command to (i.e. cd->wave).
               * Check to make sure.
               *********************************/
#ifdef CONNECTION

               ulrc = FindAmp( ulpInstance);
               if ( ulrc )
                  {
                  return (ulrc);
                  }
#endif
               ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                       MCI_STATUS,
                                       MCI_STATUS_ITEM | MCI_WAIT,
                                       (PVOID) pParams,
                                       pFuncBlock->usUserParm);
               ulrc = MAKEULONG (ulrc, MCI_COLONIZED2_RETURN);
               }
#ifdef CONNECTION
            else
               {
               ulrc = MCIERR_NO_CONNECTION;
               }
#endif
// CONNECTOR FEATURE--check if amp is there
            } /* case status volume */
          break;
      case MCI_AMP_STATUS_BALANCE:
      case MCI_AMP_STATUS_BASS   :
      case MCI_AMP_STATUS_TREBLE :
      case MCI_AMP_STATUS_GAIN   :
// CONNECTOR FEATURE--check if amp is there
#ifdef CONNECTION
            if ( ulpInstance->fConnected )
#endif
               {



               /********************************
               * It is possbile that although we
               * are connected, there is no amp
               * in the network to forward the
               * command to (i.e. cd->wave).
               * Check to make sure.
               *********************************/
#ifdef CONNECTION

               ulrc = FindAmp( ulpInstance);
               if ( ulrc )
                  {
                  return (ulrc);
                  }
#endif

               ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                       MCI_STATUS,
                                       MCI_STATUS_ITEM | MCI_WAIT,
                                       (PVOID) pParams,
                                       pFuncBlock->usUserParm );
               ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED );
               }
#ifdef CONNECTION
            else
               {
               ulrc = MCIERR_NO_CONNECTION;
               }
#endif
// CONNECTOR FEATURE--check if amp is there

          break;

      case MCI_WAVE_STATUS_CHANNELS:
            if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
               {
               pParams->ulReturn =
                  ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.usChannels;
               }
            else
               {
               pParams->ulReturn = ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels;
               }
            ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);

          break;

      case MCI_WAVE_STATUS_SAMPLESPERSEC:
            if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
               {
               pParams->ulReturn =
                  ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
               }
            else
               {
               pParams->ulReturn =
                  ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
               }
            ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);

          break;

      case MCI_WAVE_STATUS_AVGBYTESPERSEC:

             pParams->ulReturn = ulpInstance->ulAverageBytesPerSec;

             ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);

          break;

      case MCI_WAVE_STATUS_BITSPERSAMPLE:
            if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
               {
               pParams->ulReturn =
                  ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;
               }
            else
               {
               pParams->ulReturn =
                  ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;
               }
            ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);
          break;

      case MCI_STATUS_CLIPBOARD :

         habClipboard = WinQueryAnchorBlock( HWND_DESKTOP );

         /*********************************************************************
         * Check to see if there is a wave ( CF_WAVE is the defined type) in
         * the clipboard.
         *********************************************************************/


          pParams->ulReturn = WinQueryClipbrdFmtInfo( habClipboard,
                                        CF_WAVE,
                                        &ulFormatInfo );

         ulrc = MAKEULONG(ulrc, MCI_TRUE_FALSE_RETURN);
         break;

      case MCI_WAVE_STATUS_LEVEL:
#ifdef STATUS_LEVEL
           switch (STRMSTATE)
              {
               case MCI_PLAY:
               case MCI_RECORD:
                    pParams->ulReturn = ulpInstance->ulStatusLevel;
                    break;

               default:
                    pParams->ulReturn = 0;
                    break;
              }
            ulpInstance->ulStatusLevelState = STATUS_LEVEL_ON;
#else
                    pParams->ulReturn = 0;
#endif
//            ulrc = MAKEULONG (ulrc, MCI_COLONIZED2_RETURN); // ?????


            ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);
          break;

      case MCI_WAVE_STATUS_FORMATTAG:
            if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
               {
               pParams->ulReturn =
                  ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.usFormatTag;
               }
            else
               {
               pParams->ulReturn =
                  ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usFormatTag;
               }
            ulrc = MAKEULONG (ulrc, MCI_FORMAT_TAG_RETURN);
          break;

      case MCI_STATUS_MEDIA_PRESENT:
            pParams->ulReturn = MCI_TRUE;
            ulrc = MAKEULONG(ulrc, MCI_TRUE_FALSE_RETURN);
          break;

      case MCI_WAVE_STATUS_BLOCKALIGN:
           if ( ulpInstance->ulRealTimeTranslation ==MMIO_REALTIME )
              {
              pParams->ulReturn =
                 ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.usBlockAlign;
              }
           else
              {
              pParams->ulReturn =
                 ulpInstance->mmAudioHeader.mmXWAVHeader.WAVEHeader.usBlockAlign;
              }
           ulrc = MAKEULONG(ulrc, MCI_INTEGER_RETURNED);
          break;

      case MCI_STATUS_MODE:
           {
              /********************************************
               * Always Return an Integer for this case
              ********************************************/
              ulrc = MAKEULONG (ulrc, MCI_MODE_RETURN);

              /* if no file is loaded, we are not ready */
             if (ulpInstance->fFileExists == FALSE)
                {
                pParams->ulReturn = MCI_MODE_NOT_READY;
                }
             else
                {
                  /* Our current mode depends on the action we are doing */

                  switch (STRMSTATE)
                     {
                      case MCI_PLAY:
                            pParams->ulReturn = MCI_MODE_PLAY;
                           break;

                      case MCI_RECORD:
                            pParams->ulReturn = MCI_MODE_RECORD;
                           break;

                      case MCI_STOP     :
                      case STOP_PAUSED  :
                      case CUEPLAY_STATE:
                      case CUERECD_STATE:
                            pParams->ulReturn = MCI_MODE_STOP;
                           break;

                      case MCI_PAUSE:
                            pParams->ulReturn = MCI_MODE_PAUSE;
                           break;

                      default:
                            pParams->ulReturn = MCI_MODE_NOT_READY;

                     } /* of Possible Modes (Switch) */
                }
           }  /* Status Mode */
          break;

      /* Return our current time format */

      case MCI_STATUS_TIME_FORMAT:
           {
            switch (ulpInstance->ulTimeUnits)
            {
             case lMMTIME:
                   pParams->ulReturn = MCI_FORMAT_MMTIME;
                  break;

             case lMILLISECONDS:
                   pParams->ulReturn = MCI_FORMAT_MILLISECONDS;
                  break;

             case lBYTES:
                   pParams->ulReturn = MCI_FORMAT_BYTES;
                  break;

/*case lSAMPLES:*/
             default :
                   pParams->ulReturn = MCI_FORMAT_SAMPLES;
                  break;
             } /* Switch */

             ulrc = MAKEULONG (ulrc, MCI_TIME_FORMAT_RETURN);

          }
          break;
// CONNECTOR FEATURE--check if amp is there
      case MCI_STATUS_READY:
            if (ulpInstance->fFileExists == MCI_TRUE
#ifdef CONNECTION

            && ulpInstance->fConnected)
#else
            )
#endif
                 {
                 /********************************
                 * It is possbile that although we
                 * are connected, there is no amp
                 * in the network to forward the
                 * command to (i.e. cd->wave).
                 * Check to make sure.
                 *********************************/
#ifdef CONNECTION

                 ulrc = FindAmp( ulpInstance);
                 if ( ulrc )
                    {
                    return (ulrc);
                    }
#endif
                 /*****************************************
                 * Check if Amp/Mixer is Ready and active
                 *****************************************/
                 ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                         MCI_STATUS,
                                         MCI_STATUS_ITEM | MCI_WAIT,
                                         (PVOID) pParams,
                                         pFuncBlock->usUserParm );
                 }
// CONNECTOR FEATURE--check if amp is there
            else
                 pParams->ulReturn = MCI_FALSE;

            ulrc = MAKEULONG(ulrc, MCI_TRUE_FALSE_RETURN);
          break;

      default:
             return (MCIERR_INVALID_FLAG);

      } /* end of switch */

    } /* Status Item */
  else
    {
    return ( MCIERR_MISSING_FLAG );

    } /* status item is not passed */

  return (ulrc);

} /* MCIStat */

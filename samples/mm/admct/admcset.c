/**************START OF SPECIFuICATulIONS ************u*********
*
* SUBROUTINE NAME: MCISet
*
* DESCRIPTIVE NAME: Audio MCD Set Routine
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: Set various audio attributes.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_STATUS message.
*
* EXIT-NORMAL:Lo Word Return  Code MCIERR_SUCCESS, HighWord Contains
*             constant defining type of quantity returned.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
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
#define INCL_MMIO_CODEC
#define INCL_AUDIO_CODEC_ONLY

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

// CODEC stuff
#include <codec.h>
//#include "wcodec.h"

/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME:  MCISET.C
*
* DESCRIPTIVE NAME: Set Waveform Device Parameters.
*
* FUNCTION:
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
RC MCISet (FUNCTION_PARM_BLOCK *pFuncBlock)

{
  ULONG                ulrc;           // Error Value
  ULONG                ulParam1;       // Msg Flags
//ULONG                ulFormatTag = 0;// Format tag flags--PCM etc.
  ULONG                ulHoldError;
  ULONG                ulHoldTag;       /* Backup copy of format tag */

  INSTANCE             *ulpInstance;   // Local Instance
  ULONG                ulSetFlags;     // Mask For Incoming MCI Flags
  MCI_AMP_SET_PARMS    AmpSetParms;    // Volume Cmnds
  PMCI_WAVE_SET_PARMS  pSetParms;      // Ptr to set Parms
//  MCI_WAVE_SET_PARMS   AmpSet;         // Ptr to set Parms
  MMAUDIOHEADER        mmaudioheader;  // mode to set the device into
//  BOOL                 fItemChanged = FALSE;  // Indicates if set item was modified.



  ulSetFlags = ulParam1 = pFuncBlock->ulParam1;

  pSetParms = ( PMCI_WAVE_SET_PARMS ) pFuncBlock->ulParam2;
  ulpInstance= (INSTANCE *) pFuncBlock->ulpInstance;

  /**************************************
  * Check For Validity of Flags
  * For example, it is impossible to set
  * something on AND off.
  **************************************/

  if (ulParam1 & MCI_SET_ON && ulParam1 & MCI_SET_OFF)
      return MCIERR_FLAGS_NOT_COMPATIBLE;


  /*************************************
  * The current wave driver does not
  * support the following flags
  *************************************/

  if ( ulParam1 & MCI_SET_DOOR_OPEN     ||
       ulParam1 & MCI_SET_DOOR_CLOSED   ||
       ulParam1 & MCI_SET_DOOR_LOCK     ||
       ulParam1 & MCI_SET_DOOR_UNLOCK   ||
       ulParam1 & MCI_SET_VIDEO         ||
       ulParam1 & MCI_SET_ITEM          ||
       ulParam1 &  MCI_WAVE_SET_BLOCKALIGN )

     {
     return (MCIERR_UNSUPPORTED_FLAG);
     }

  /***********************************************
  * The caller cannot use the wave driver to set
  * audio operations.  These must be done with the
  * amp/mixer.  The amp/mixer connected to the
  * wave driver can be accessed via mci_connection
  *************************************************/


  if ( ( ulParam1 & MCI_SET_AUDIO ) &&
       (( ulParam1 & MCI_AMP_SET_BALANCE ) ||
        ( ulParam1 & MCI_AMP_SET_TREBLE  ) ||
        ( ulParam1 & MCI_AMP_SET_BASS    ) ||
        ( ulParam1 & MCI_AMP_SET_GAIN    ) ||
        ( ulParam1 & MCI_AMP_SET_PITCH))  )
     {
     return MCIERR_UNSUPPORTED_FLAG;
     }



  if (ulParam1 & MCI_SET_TIME_FORMAT && ulParam1 & MCI_OVER )
      return ( MCIERR_FLAGS_NOT_COMPATIBLE );

  if ((ulParam1 & MCI_SET_VOLUME) && !(ulParam1 & MCI_SET_AUDIO))
     {
     return MCIERR_MISSING_FLAG;
     }


  /*********************************
  * The caller is required to send in
  * a valid pointer to mci_set_parms
  ***********************************/

  ulrc = CheckMem ((PVOID) pSetParms,
                   sizeof (MCI_WAVE_SET_PARMS),
                   PAG_READ);

  if (ulrc != MCIERR_SUCCESS)
      return ( MCIERR_MISSING_PARAMETER );

  ulSetFlags &= ~(MCI_WAIT + MCI_NOTIFY );

  if (ulSetFlags == 0)
      return MCIERR_MISSING_PARAMETER;


  /***************************************
  * Mask defining Known MCI Set Flags
  ****************************************/

  ulSetFlags &= ~( MCI_SET_AUDIO + MCI_SET_TIME_FORMAT + MCI_SET_VOLUME +
                   MCI_SET_ON    + MCI_SET_OFF         + MCI_WAVE_SET_CHANNELS +
                   MCI_WAVE_SET_BITSPERSAMPLE          + MCI_WAVE_SET_SAMPLESPERSEC +
                   MCI_WAVE_SET_FORMATTAG              + MCI_WAVE_SET_BLOCKALIGN +
                   MCI_WAVE_SET_AVGBYTESPERSEC         + MCI_SET_DOOR_OPEN +
                   MCI_SET_DOOR_CLOSED                 + MCI_SET_DOOR_LOCK +
                   MCI_SET_DOOR_UNLOCK + MCI_SET_VIDEO + MCI_OVER );


  /*******************************************
  * Return invalid if any other bits are set
  *******************************************/

  if (ulSetFlags > 0)
      return MCIERR_INVALID_FLAG;



  /*******************************************
  * We do allow certain audio sets to pass
  * on to the amp/mixer that we are connected
  * to (e.g. volume).  Thus crude audio control
  * is possible via the audio mcd, however, more
  * sophisticated stuff should be sent directly
  * to the amp.
  * Prepare and send the audio command to the
  * amp via the mcisendcommand.
  *******************************************/

  if (ulParam1 & MCI_SET_AUDIO)
     {
     /****************************
     * Copy The Info to Amp Set
     *****************************/

     AmpSetParms.ulLevel = pSetParms->ulLevel;
     AmpSetParms.ulAudio = pSetParms->ulAudio;
     AmpSetParms.ulOver = pSetParms->ulOver;

     /****************************
     * Copy The Info to Amp Set
     *****************************/
// CONNECTOR FEATURE
#ifdef CONNECTION
     if ( !ulpInstance->fFoundMixer )
        {
        /********************************
        * It is possbile that although we
        * are connected, there is no amp
        * in the network to forward the
        * command to (i.e. cd->wave).
        * Check to make sure.
        *********************************/
        ulrc = FindAmp( ulpInstance);
        if ( !ulrc )
           {
           return (ulrc);
           }
        }
#endif
// CONNECTOR FEATURE

     /**************************************
         * Send the Request To the Amp/Mixer
     ***************************************/

     ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                             MCI_SET,
                             ulParam1&~(MCI_NOTIFY) |MCI_WAIT,
                             (PVOID) &AmpSetParms,
                             pFuncBlock->usUserParm);

     if ( ulrc )
        {
        return ( ulrc );
        }

     } /* of Audio Flag */



  /******************************************
  * The audio driver currently supports
  * only mmtime, milliseconds, sampling rate
  * and bytes as the time format, other
  * requests will receive invalid time format
  *******************************************/

  if (ulParam1 & MCI_SET_TIME_FORMAT)
      {
      switch (pSetParms->ulTimeFormat)
         {
         case MCI_FORMAT_MMTIME:
              ulpInstance->ulTimeUnits = lMMTIME;
              break;

         case MCI_FORMAT_MILLISECONDS:
              ulpInstance->ulTimeUnits = lMILLISECONDS;
              break;

         case MCI_FORMAT_SAMPLES:
              ulpInstance->ulTimeUnits = lSAMPLES;
              break;

         case MCI_FORMAT_BYTES:
              ulpInstance->ulTimeUnits = lBYTES;
              break;

         default: return MCIERR_INVALID_TIME_FORMAT_FLAG;

         } /* Switch time format */

      } /* Time Format flag was passed in */

  /******************************
  * Check for wave specific flags
  *******************************/

  ulSetFlags = ulParam1 & (  MCI_WAVE_SET_CHANNELS
                           + MCI_WAVE_SET_BITSPERSAMPLE
                           + MCI_WAVE_SET_SAMPLESPERSEC
                           + MCI_WAVE_SET_FORMATTAG
                           + MCI_WAVE_SET_AVGBYTESPERSEC );


  /*************************************************************
  ** If any of the wave set flags are greater than 0, then it is
  ** possible we may have to destroy the stream to inform the
  ** stream handlers of the change in the data rates.  This is
  ** necessary since there is no method of to communicate the
  ** date rate other than at stream creation--thus these set
  ** operations can be rather expensive.
  *************************************************************/

  if ( ulSetFlags > 0 )
    {

    /*******************************************************
    ** If the stream has been created, then we must destroy,
    ** perform the set and set a flag to indicate that the
    ** stream must be created
    *******************************************************/

    if ( ulpInstance->ulCreateFlag == PREROLL_STATE )
       {
       /* if we are actually streaming, then we cannot perform the set */

       if ( STRMSTATE == MCI_PLAY   ||
            STRMSTATE == MCI_RECORD ||
            STRMSTATE == MCI_PAUSE )

          {
          return ( MCIERR_INVALID_MODE );
          }
       else
          {

          if ( !ulpInstance->ulOldStreamPos )
            {
            ulrc = SpiGetTime( STREAM.hStream,
                               ( PMMTIME ) &ulpInstance->ulOldStreamPos );

            /************************************************
            * if an error occurred, then don't remember our
            * position in the stream
            ************************************************/
            if ( ulrc )
              {
              ulpInstance->ulOldStreamPos = 0;
              }
            }
          /***********************************
          ** set the stream into create state
          ** following commands will recreate
          ***********************************/

//        ulpInstance->ulCreateFlag = CREATE_STATE;
          }

       } /* If a stream has already been created */

    /* Set up a structure to call the VSD with */
// CONNECTION FEATURE
//    memcpy (&AmpSet, pSetParms, sizeof(MCI_WAVE_SET_PARMS));

   if ( ulpInstance->ulRealTimeTranslation == MMIO_REALTIME )
      {
      memmove( &mmaudioheader, &ulpInstance->mmRealHeader, sizeof( MMAUDIOHEADER ) );
      }
   else
      {
      memmove( &mmaudioheader, &ulpInstance->mmAudioHeader, sizeof( MMAUDIOHEADER ) );
      }


    ulSetFlags = 0;

    if (ulParam1 &  MCI_WAVE_SET_CHANNELS )
       {
//       ulSetFlags = MCI_WAVE_SET_CHANNELS;
//       if ( mmaudioheader.mmXWAVHeader.WAVEHeader.usChannels != pSetParms->usChannels )
//          {
//          fItemChanged = TRUE;
//          }

       mmaudioheader.mmXWAVHeader.WAVEHeader.usChannels = pSetParms->usChannels ;
       }

    if (ulParam1 &  MCI_WAVE_SET_SAMPLESPERSEC)
      {
//      ulSetFlags |= MCI_WAVE_SET_SAMPLESPERSEC;
//      if ( mmaudioheader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec != pSetParms->ulSamplesPerSec )
//          {
//          fItemChanged = TRUE;
//          }


      mmaudioheader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec = pSetParms->ulSamplesPerSec;
      }



    if (ulParam1 & MCI_WAVE_SET_BITSPERSAMPLE)
       {
//       ulSetFlags |= MCI_WAVE_SET_BITSPERSAMPLE;
//       if ( mmaudioheader.mmXWAVHeader.WAVEHeader.usBitsPerSample != pSetParms->usBitsPerSample )
//          {
//          fItemChanged = TRUE;
//          }
       mmaudioheader.mmXWAVHeader.WAVEHeader.usBitsPerSample   =   pSetParms->usBitsPerSample;
       }

    if (ulParam1 & MCI_WAVE_SET_FORMATTAG)
       {
//       ulSetFlags |= MCI_WAVE_SET_FORMATTAG;
//       if ( mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag != pSetParms->usFormatTag )
//          {
//          fItemChanged = TRUE;
//          }
       mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag = pSetParms->usFormatTag;

// CONNECTOR FEATURE
// CODEC change

       /*-----------------------------------------------
       * If we have loaded a codec, it is no longer possible
       * to switch to another codec.  Or if there is existing
       * data in the file, it is not possible to switch.
       *------------------------------------------------*/


       if ( ulpInstance->ulRealTimeTranslation == MMIO_REALTIME &&
            ulpInstance->ulDataSize != 0 )
          {
          return ( MCIERR_INVALID_MODE );
          }
       /* Store the format tag we are changing */

       ulHoldTag = mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag;
       /*-----------------------------------------------
       * This flag tells us if we have to let the io proc
       * know that since we changed format tags, potentially
       * the codec will have to change too.
       *------------------------------------------------*/
//     if ( pSetParms->usFormatTag != DATATYPE_WAVEFORM )
//        {
//        ulFormatTag = NON_PCM;
//        }

// CODEC change
       }
    else
       {
       ulHoldTag = ulpInstance->mmRealHeader.mmXWAVHeader.WAVEHeader.usFormatTag;
       }

   /*-------------------------------------
   * There is no need to set the device into
   * a different mode, if the caller is setting
   * the same parameters.
   * ------------------------------------*/

//    if ( fItemChanged )
//       {
       /*-------------------------------------
       * The SetAudioDevice function will
       * be used to test and set the connection
       * to the amp-mixer.
       * ------------------------------------*/
   
       ulrc = SetAudioDevice( ulpInstance,
                              &mmaudioheader );
   
       if ( ulrc )
          {
          ulHoldError = ulrc;
    #ifndef CONNECTION
          /*---------------------------------------------
          * We will only perform this function if we
          * get a best-fit error.  If there is a best-fit
          * error, it will be one of the following:
          * MCIERR_UNSUPP_FORMAT_TAG
          * MCIERR_UNSUPP_SAMPLESPERSEC
          * MCIERR_UNSUPP_BITSPERSAMPLE
          * MCIERR_UNSUPP_CHANNELS
          * MCIERR_UNSUPP_FORMAT_MODE
          *---------------------------------------------*/
          if ( ULONG_LOWD( ulrc)  == MCIERR_UNSUPP_FORMAT_TAG )
             {
   
             /* ------------------------------------------
             * If a codec has already been loaded, and data
             * is in the file, we cannot switch codecs, else
             * the result would be confusion.
             *--------------------------------------------*/
   
             if ( ulpInstance->ulDataSize != 0 )
                {
                return ( MCIERR_INVALID_MODE );
                }
   
             /* ------------------------------------------
             * Connected device updated the mmaudioheader to
             * the nearest matching mode.
             *--------------------------------------------*/
   
             ulrc = SetAudioDevice( ulpInstance,
                                    &mmaudioheader );
   
             /*------------------------------------------
             * Since the user has changed the format tag
             * i.e. from pcm to dvi adpcm etc.
             * We may have to change the codec we are using--
             * so inform the io proc via choose CODEC message.
             *------------------------------------------*/
             ulrc = SetupCodec( ulpInstance, ulHoldTag );
   
             if ( ulrc )
                {
   
                return ( ulHoldError );
                }
   
   
             /****************************************************
             * Inform the io proc whether or not the data should
             * be translated real-time or not.
             *****************************************************/
             ulpInstance->ulRealTimeTranslation = MMIO_REALTIME;
             ulpInstance->ulCodecDescription = MAKE_TARGET | TARGET_CODEC;
   
             }
   
          else
    #endif
             {
             return ( ulHoldError );
             }
          }
       else
          {
          ulpInstance->ulRealTimeTranslation = MMIO_NONREALTIME;
   
          /* Connected MCD may update MMAUDIOHEADER for correct mode */
         memmove(  &ulpInstance->mmAudioHeader, &mmaudioheader,  sizeof( MMAUDIOHEADER ) );
   
          }
   
       // must still copy information from waveset parms to instance if ok
   
    // CONNECTOR FEATURE--changed meaning of SetAudioDevice
   
      if (ulParam1 &  MCI_WAVE_SET_AVGBYTESPERSEC)
        {
        if ( pSetParms->ulAvgBytesPerSec <
               ( ULONG ) ( WAVEHDR.ulSamplesPerSec * ( WAVEHDR.usBitsPerSample / 8 ) * WAVEHDR.usChannels ) )
           {
           return ( MCIERR_OUTOFRANGE );
           }
          else
           {
           ulpInstance->ulAverageBytesPerSec = pSetParms->ulAvgBytesPerSec;
           }
        } /* if average bytes per sec are passed in */
   
      else
        {
        ulpInstance->ulAverageBytesPerSec =
               ( ULONG ) ( WAVEHDR.ulSamplesPerSec * ( WAVEHDR.usBitsPerSample / 8 ) * WAVEHDR.usChannels );
        }
   
       STRMSTATE = STREAM_SET_STATE;
   
       // 6421--ensure that audio header structure is ALWAYS accurate.
   
       //    SyncAudioHeader(ulpInstance);
//       } /* If a set item changed */

    } /* if a wave set is requested */


  /******************************************************************
  * If the caller has opened with an MCI_OPEN_MMIO or if a record
  * has been done, the file header must be updated.  In the case of
  * open mmio, applications such as the wave header are dependent
  * on the information in the file header being valid at all times.
  *******************************************************************/
  if ( ulpInstance->fOpenMMIO &&
       ulpInstance->ulCapabilities & CAN_RECORD)

       {
       /**************************************
       * If the card is in record mode an a
       * file has been loaded, then we can do
       * the set header.
       ***************************************/

       ulrc = SetAudioHeader (ulpInstance);
       } /* Not Recording */




  return (ulrc);

} /* MCISet */

#ifndef CONNECTION
/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: SetupCodec
*
* FUNCTION: Inform Io proc of new codec changes
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: FUNCTION_PARM_BLOCK
*
* EXIT-NORMAL: MCIERR_SUCCESS
*
* EXIT_ERROR:
*
* EFFECTS:
*
* EXTERNAL REFERENCES: mmioSendMessage()    - MMIO function
*
*********************** END OF SPECIFICATIONS **********************/

ULONG   SetupCodec( INSTANCE      *pInstance,
                    USHORT        usFormatTag )

{
LONG          rc;
MMAUDIOHEADER mmaudheader;
  MMEXTENDINFO      mmExtInfo;
  PCODECOPEN        pcodecopen;
  CODECASSOC        codecAssoc;
  PCODECINIFILEINFO pcodecini;
  extern HHUGEHEAP     heap;                // Global MCD Heap


  if ( !pInstance->pcodecini )
     {

     pInstance->pcodecini = HhpAllocMem( heap,
                                           sizeof( MMEXTENDINFO) +
                                           sizeof( CODECINIFILEINFO ) +
                                           sizeof( CODECASSOC ) +
                                           sizeof( CODECOPEN ) +
                                           sizeof( MMAUDIOOPEN ) +
                                           4096 );    // Buffer 4096--can't determine how large.

     if ( !pInstance->pcodecini)
        {
        return( MCIERR_OUT_OF_MEMORY);
        }
     }

  pcodecini = pInstance->pcodecini;

  /***********************************************
  * We allocated enough memory for the codecini +
  * the codecopen structures.  Advanced (and store)
  * pointer to get this accomplished.
  ***********************************************/

  pInstance->pcodecopen = pcodecopen = ( PVOID ) ++pcodecini;


  memmove( &mmaudheader, &pInstance->mmAudioHeader, sizeof (MMAUDIOHEADER ) );
  mmaudheader.mmXWAVHeader.WAVEHeader.usFormatTag = usFormatTag;



  mmExtInfo.ulStructLen = sizeof( MMEXTENDINFO );

// note: need way to pass flag that codec must init own variables!!!
  mmExtInfo.ulFlags = MMIO_CODEC_ASSOC | MMIO_REALTIME_CODEC;


  mmExtInfo.ulNumCODECs = 1;
  mmExtInfo.pCODECAssoc = &codecAssoc;
  codecAssoc.pCodecOpen = pcodecopen;
  pcodecopen->pSrcHdr = ( PVOID ) &pInstance->mmRealHeader;
  memmove( &pInstance->mmRealHeader, &mmaudheader, sizeof( MMAUDIOHEADER ) );

  pcodecopen->pDstHdr = ( PVOID ) &pInstance->mmAudioHeader;
  pcodecopen->ulFlags = INIT_CODEC;

  codecAssoc.pCODECIniFileInfo = pcodecini = pInstance->pcodecini;


  memset( pcodecini, '\0', sizeof( CODECINIFILEINFO ));
  pcodecini->ulStructLen = sizeof( CODECINIFILEINFO );
  pcodecini->fcc = FOURCC_WAV;
  pcodecini->ulCompressType =
     mmaudheader.mmXWAVHeader.WAVEHeader.usFormatTag;
  pcodecini->ulCompressSubType = 0;
  pcodecini->ulMediaType = MMIO_MEDIATYPE_AUDIO;

  /* The Codec setup call informs the io proc of a new format tag */


  rc = mmioSet( pInstance->hmmio,
                &mmExtInfo,
                MMIO_SET_EXTENDEDINFO );


  /*-------------------------------------------------
  * If we will be doing real-time translations
  * then, it is important to examine the capabilities
  * of the io proc.
  *-------------------------------------------------*/
  if ( pcodecini->ulCapsFlags & CODEC_COMPRESS )
     {
     pInstance->ulCapabilities |= CAN_SAVE;
     }
  else
     {
     pInstance->ulCapabilities &= ~CAN_SAVE;
     }

  if ( pcodecini->ulCapsFlags & CODEC_COMP_REALTIME )
     {
     pInstance->ulCapabilities |= CAN_RECORD;
     }
  else
     {
     pInstance->ulCapabilities &= ~CAN_RECORD;
     }



//  rc = mmioSendMessage( pInstance->hmmio,
//                        MMIOM_SETUPCODEC,
//                        ( LONG ) &mmaudheader,
//                        0 );

  return ( rc );

} /* SetupCodec */

#endif

/********************* START OF SPECIFICATulIONS *********************
*
* SUBROUTINE NAME: MCIConnection
*
* DESCRIPTIVE NAME: Audio MCD Connection Routine
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: Processes connection messages from MCI drivers and applications.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_STATUS message.
*
* EXIT-NORMAL:Lo Word Return  Code MCIERR_SUCCESS
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
#include <mcipriv.h>

#ifdef CONNECTION
/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCIConnection
*
* FUNCTION: Process MCI_CONNECTION message.
*
* NOTES: This function can make an MCI or SSM connection.
*        it also can break an MCI or SSM connection.
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
ULONG MCIMakeConnection ( FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Error Value
//  ULONG                ulConnFlags;
  ULONG                ulParam1;       // Msg Flags

  INSTANCE             *pInstance;   // Local Instance

  PCONCB               pconcb = (PCONCB) pFuncBlock->ulParam2;

//  ulConnFlags = ulParam1 = pFuncBlock->ulParam1;

  pInstance= (INSTANCE *) pFuncBlock->ulpInstance;

   /*-------------------------------------
   * If the caller wants to specify a
   * connector type and its not digital
   * wave stream connection --error out.
   *------------------------------------*/

   if ( pconcb->ulConnectorType != MCI_WAVE_STREAM_CONNECTOR )
      {
      return ( MCIERR_INVALID_CONNECTOR_TYPE );
      }

// what about Index?????

   if ( ulParam1 == MCIDRV_CONNECT_TARGET )
      {
      pconcb->hid = pInstance->StreamInfo.hidATarget;
      pconcb->pevfn = (PEVFN)RecordEventRoutine;
      pInstance->ulOperation = MCIDRV_INPUT;
      }
   else
      {
      pconcb->hid = pInstance->StreamInfo.hidASource;
      pconcb->pevfn = (PEVFN) PlayEventRoutine;
      pInstance->ulOperation = MCIDRV_OUTPUT;
      }

   /* --------------------------------------------
   * Fill in the remainder of the CONCB struct
   * this includes:
   *    DCB (Device Control Block )
   *    SPCBKey (stream identifier )
   *    Event Routine (where spi events come )
   *    Event Control Block -- memory for events
   *----------------------------------------------*/

   pconcb->pdcb = NULL;
   pconcb->pspcbkey = NULL; // &pInstance->StreamInfo.SpcbKey;
   pconcb->pMediaHeader = (PVOID) &pInstance->mmAudioHeader;

   pconcb->pevcb = (PEVCB) &pInstance->StreamInfo.Evcb;
//   pconcb->phevent = &pInstance->StreamInfo.hEvent;


   /* Set state variable to indicate conneciton has been made */

   pInstance->fConnected = TRUE;

  pInstance->ulFlags &= ~HAVE_SPCBKEY;
  pInstance->pconcb = pFuncBlock->ulParam2;

  return ( ulrc );

} /* MCIConnection */
#endif


/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: BreakMCIConnection
*
* DESCRIPTIVE NAME: This function will break the connection to whatever
*                   device is connected to the audio MCD driver.  It will also
*                   destroy any streams between the two connections.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG MCIBreakConnection( FUNCTION_PARM_BLOCK *pFuncBlock)

{
   INSTANCE         *pInstance;   // Local Instance

   pInstance= (INSTANCE *) pFuncBlock->ulpInstance;


   pInstance->fConnected = FALSE;

   return ( MCIERR_SUCCESS );

} /* MCIBreakConnection */

#ifdef CONNECTION
/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: MCITestConnection
*
* DESCRIPTIVE NAME: This function will determine if a connection can be made.
*                   Caller must be using the wave stream connector and also pass
*                   in a valid media header (i.e. MMAUDIOHEADER ).
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG MCITestConnection( FUNCTION_PARM_BLOCK *pFuncBlock)

{
ULONG            ulrc;           // Error Value
ULONG            ulConnFlags;

INSTANCE         *pInstance;   // Local Instance

PCONCB           pconcb = (PCONCB) pFuncBlock->ulParam1;
PMMAUDIOHEADER   pmmaudioheader = pconcb->pMediaHeader;



ulConnFlags = pFuncBlock->ulParam1;

//pInstance= (INSTANCE *) pFuncBlock->ulpInstance;

   /*-------------------------------------
   * If the caller wants to specify a
   * connector type and its not digital
   * wave stream connection --error out.
   *------------------------------------*/

   if ( pconcb->ulConnectorType != MCI_WAVE_STREAM_CONNECTOR )
      {
      return ( MCIERR_INVALID_CONNECTOR_TYPE );
      }

   if ( pmmaudioheader->ulMediaType != MMIO_MEDIATYPE_AUDIO )
      {
      return ( MCIERR_INVALID_CONNECTOR_TYPE );
      }

   return ( MCIERR_SUCCESS );

} /* MCITestConnection */
#endif

/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: MCIAssociate.
*
* DESCRIPTIVE NAME: This function will be called by MDM to associate any
*                   audio specific information to our stream handler (usually
*                   the file system stream handler).
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG MCIAssociate ( FUNCTION_PARM_BLOCK *pFuncBlock)

{
  ULONG                ulrc;           // Error Value
  ULONG                ulConnFlags;

  INSTANCE             *pInstance;   // Local Instance

  PCONCB               pconcb = (PCONCB) pFuncBlock->ulParam2;
  extern HID           hidASource;
  extern HID           hidATarget;


  ulConnFlags = pFuncBlock->ulParam1;

  pInstance= (INSTANCE *) pFuncBlock->ulpInstance;

  /*-----------------------------------------------------
  * Indicate that we know that a stream has been created
  * and store the stream handle for stream destruction.
  *----------------------------------------------------*/
  pInstance->StreamInfo.hStream = pconcb->hstream;

  /* Save the fact that we know the stream is created */
  /***************************
  * Set a flag which indicates
  * that a stream has been
  * created.
  ****************************/
  pInstance->ulCreateFlag = PREROLL_STATE;

  /* Perform associations if necessary */

  pInstance->ulOperation = ulConnFlags & ( MCIDRV_INPUT | MCIDRV_OUTPUT );
  if ( pInstance->usPlayLstStrm )
     {
     ulrc = AssociatePlayList ( pInstance );
     }
  else
     {
     /**********************************************************
     * The stream must be associated with a data object
     * in our case a mmio file handle.  The file system
     * stream handler (FSSH) will always be the stream handler
     * that we want to associate the data object with,
     * therefore, if we have created a playback stream then
     * FSSH is the source, so associate with the source.  On a
     * record stream, FSSH is the target, so associate with
     * the target.
     **********************************************************/


     pInstance->StreamInfo.acbmmio.ulObjType = ACBTYPE_MMIO;
     pInstance->StreamInfo.acbmmio.ulACBLen = sizeof (ACB_MMIO);
     pInstance->StreamInfo.acbmmio.hmmio = pInstance->hmmio;
     if ( ulConnFlags & MCIDRV_OUTPUT )
         {

         ulrc = ADMCAssociate ( pconcb->hstream,
                               hidASource,
                               (PVOID) &pInstance->StreamInfo.acbmmio);


         }  /* Associating play stream */

     else // if ( pInstance->ulOperation == MCIDRV_INPUT )
         {
         ulrc = ADMCAssociate ( pconcb->hstream,
                                hidATarget,
                                (PVOID) &pInstance->StreamInfo.acbmmio);

         } /* Associating record stream */

     } /* else this is not a playlist */

  return ( ulrc );

} /* MCIAssociate */

/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: TestNetwork
*
* DESCRIPTIVE NAME: This function queries MDM to determine if the network
*                   we are attached to can record.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG TestNetwork( INSTANCE                    *pInstance,
                   PMCI_WAVE_GETDEVCAPS_PARMS  pExtDevCaps )

{
   CONCB         concb;
   MMAUDIOHEADER mmaudioheader;
   ULONG         ulDirection;
   ULONG         ulrc;

   if ( !pInstance->fConnected )
      {
      return (MCIERR_NO_CONNECTION );
      }

//  pExtDevCaps->ulBitsPerSample;


  mmaudioheader.mmXWAVHeader.WAVEHeader.usFormatTag       =  pExtDevCaps->ulFormatTag;
  mmaudioheader.mmXWAVHeader.WAVEHeader.usChannels        =  pExtDevCaps->ulChannels ;
  mmaudioheader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec   =  pExtDevCaps->ulSamplesPerSec;

  //  If mmaudioheader contains !0, then assume source, else must be target.

  if ( pExtDevCaps->ulFormatMode == MCI_PLAY )
     {
     mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInMS = 1;
     ulDirection = MCIDRV_CONNECT_TARGET;
     }
  else
     {
     mmaudioheader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInMS = 0;
     ulDirection = MCIDRV_CONNECT_SOURCE;
     }

  mmaudioheader.mmXWAVHeader.WAVEHeader.usBitsPerSample   =  pExtDevCaps->ulBitsPerSample;
  mmaudioheader.ulMediaType = MMIO_MEDIATYPE_AUDIO;
  mmaudioheader.ulHeaderLength = sizeof( MMAUDIOHEADER );

   concb.pMediaHeader = ( PVOID) &mmaudioheader;
#ifndef CONNECTION
   ulrc = mciSendCommand ( pInstance->usAmpDeviceID,
#else
   ulrc = mciSendCommand ( pInstance->usWaveDeviceID,
#endif
                           MCIDRV_TEST_CONNECTION,
                           ulDirection,
                           (PVOID) &concb,
                           0 );


   return ( ulrc );

} /* TestNetwork */



/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: CanPlayRecord
*
* DESCRIPTIVE NAME: This function queries MDM to determine if the network
*                   can either produce/consume information.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG CanPlayRecord( INSTANCE   *pInstance,
                     ULONG      ulDirection )

{
   CONCB   concb;
   ULONG   ulrc;


   if ( !pInstance->fConnected )
      {
      return (MCIERR_NO_CONNECTION );
      }

   concb.pMediaHeader = ( PVOID) &pInstance->mmAudioHeader;
   pInstance->mmAudioHeader.ulMediaType = MMIO_MEDIATYPE_AUDIO;

#ifndef CONNECTION
   ulrc = mciSendCommand ( pInstance->usAmpDeviceID,
#else
   ulrc = mciSendCommand ( pInstance->usWaveDeviceID,
#endif
                           MCIDRV_TEST_CONNECTION,
                           ulDirection,
                           (PVOID) &concb,
                           0 );


   return ( ulrc );

} /* CanPlayRecord */

/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: ModifyConnection
*
* DESCRIPTIVE NAME: This function queries MDM to determine if the network
*                   can either produce/consume information.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG ModifyConnection( INSTANCE   *pInstance )

{
extern HID   hidBSource;
extern HID   hidBTarget;


   CONCB   concb;
   ULONG   ulrc;

   if ( !pInstance->fConnected )
      {
      return (MCIERR_NO_CONNECTION );
      }

   concb.pMediaHeader = ( PVOID) &pInstance->mmAudioHeader;
   pInstance->mmAudioHeader.ulMediaType = MMIO_MEDIATYPE_AUDIO;


#ifndef CONNECTION
   concb.ulFlags = VSD_SET_ALL;

   ulrc = mciSendCommand ( pInstance->usAmpDeviceID,
#else
   memmove( &concb, pInstance->pconcb, sizeof( CONCB ) );
   concb.pMediaHeader = ( PVOID) &pInstance->mmAudioHeader;

   ulrc = mciSendCommand ( pInstance->usWaveDeviceID,
#endif
                           MCIDRV_MODIFY_CONNECTION,
                           pInstance->ulOperation,
                           (PVOID) &concb,
                           0 );

   if ( ulrc )
     return ( ULONG_LOWD(ulrc) );


#ifndef CONNECTION
   pInstance->StreamInfo.SpcbKey.ulDataType    = concb.pspcbkey->ulDataType;
   pInstance->StreamInfo.SpcbKey.ulDataSubType = concb.pspcbkey->ulDataSubType;
   pInstance->StreamInfo.SpcbKey.ulIntKey = 0;

   /*------------------------------------------------------
   * Get certain streaming information from the stream handler
   * we have loaded.
   *--------------------------------------------------------*/
   ulrc = SpiGetProtocol( concb.hid,
                          &pInstance->StreamInfo.SpcbKey,
                          &pInstance->StreamInfo.spcb );

   if ( ulrc )
      {
      return ( ulrc );
      }


   pInstance->ulBytes   = pInstance->StreamInfo.spcb.ulBytesPerUnit;
   pInstance->ulMMTime  = pInstance->StreamInfo.spcb.mmtimePerUnit;

   /* Save the hid from the connected device--use for spcbkeys etc. */

   if ( pInstance->ulOperation == MCIDRV_INPUT )
      {
      hidBSource = concb.hid;
      }
   else
      {
      hidBTarget = concb.hid;
      }

   memmove( &pInstance->StreamInfo.SpcbKey, concb.pspcbkey, sizeof( SPCBKEY) );
   pInstance->pdcb = concb.pdcb;
#else
  pInstance->ulFlags &= ~HAVE_SPCBKEY;
#endif
   return ( ulrc );

} /* ModifyConnection */


/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: FindAmp
*
* DESCRIPTIVE NAME: Gets the device ID of the first Amplifier/Mixer in the network
*                   All audio commands (such as volume, treble, bass etc.)
*                   we can't process will go there.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/
#ifdef CONNECTION
ULONG FindAmp( INSTANCE *pInstance )

{
//   MCISYSYINFO_ID_PARMS   idparms;

   ULONG   ulrc = MCIERR_SUCCESS;

   if ( !pInstance->fConnected )
      {
      return (MCIERR_NO_CONNECTION );
      }

   if ( pInstance->fFoundMixer )
      {
      return ( MCIERR_SUCCESS );
      }

   /* Tell system we want the first mixer */
#ifndef CONNECTION
   pInstance->fFoundMixer = TRUE;
#endif
//   idparms.ulDeviceType = MCI_DEVTYPE_AMPMIXER;
//   ulrc = mciSendCommand ( pInstance->usWaveDeviceID,
//                           MCIDRV_SYSINFO,
//                           SYSINFO_GET_ID,
//                           (PVOID) &idparms,
//                           0 );
   ulrc = MCIERR_SUCCESS;  // temp hack
   /* If we found a mixer, then these commands can be safely routed */

   if ( !ulrc )
      {
//      pInstance->usAmpDeviceID = idparms.usDeviceID;
      pInstance->fFoundMixer = TRUE;
      }
   return ( ulrc );

} /* FindAmp */
#endif

/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: CheckForConnection
*
* DESCRIPTIVE NAME: Determines if MCD is connected to another MCD.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/

ULONG CheckForConnection( INSTANCE *pInstance )

{
   if ( !pInstance->fConnected )
      {
      return (MCIERR_NO_CONNECTION );
      }

   return ( MCIERR_SUCCESS );

} /* CheckForConnection */


/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: GetStreamingSemaphore
*
* DESCRIPTIVE NAME: This function queries to acquire the streaming semaphore
*                   for cross MCD control.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/
ULONG GetStreamingSemaphore( INSTANCE *pInstance )

{
// return real semaphore when defined
return ( MCIERR_SUCCESS );
}

/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: ReleaseStreamingSemaphore
*
* DESCRIPTIVE NAME: This function releases the streaming semaphore
*                   for cross MCD control.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/
ULONG ReleaseStreamingSemaphore( INSTANCE *pInstance )

{
// return real semaphore when defined
return ( MCIERR_SUCCESS );
}



/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: StartPlayBack
*
* DESCRIPTIVE NAME: This function starts a streaming network.
*
* FUNCTION:
*
* ulStartFlags : MCIDRV_CUE_PLAYBACK
*                MCIDRV_CUE_RECORD
*                MCIDRV_START_PLAYBACK
*                MCIDRV_START_RECORD
*
*************************** END OF SPECIFICATIONS *************************/
ULONG StartStream( INSTANCE *pInstance,
                   ULONG    ulStartFlags )

{
//CONCB   concb;
ULONG   ulrc;
ULONG   ulSpiFlags = SPI_START_STREAM;
CONCB   concb;
#ifndef CONNECTION

   if ( ulStartFlags == MCIDRV_CUE_PLAYBACK )
      {
      ulSpiFlags = SPI_START_PREROLL;
      }
   ulrc = SpiStartStream(pInstance->StreamInfo.hStream, ulSpiFlags );
#else

  concb.pMediaHeader = ( PVOID ) &pInstance->mmAudioHeader;
  pInstance->mmAudioHeader.ulMediaType = MMIO_MEDIATYPE_AUDIO;

  ulrc = mciSendCommand ( pInstance->usWaveDeviceID,
          MCIDRV_START,
          ulStartFlags,
          &concb,
          0 );

#endif
  return ( ulrc );
}
#ifdef CONNECTION
/************************ START OF SPECIFICATIONS **************************
*
* SUBROUTINE NAME: GetSpcbkey
*
* DESCRIPTIVE NAME: This function starts a streaming network.
*
* FUNCTION:
*
*************************** END OF SPECIFICATIONS *************************/
ULONG GetSpcbKeyInfo( INSTANCE *pInstance )
{
  ULONG    ulrc;

  /*----------------------------------------------------------
  * Retrieve the mmtime and byte per unit numbers from the
  * SPCB key so that can calculate values for seeks etc.
  * This information will also be used to communicate network
  * usage parameters to network io procs.
  *---------------------------------------------------------*/
  ulrc = SpiGetProtocol( 2, //pInstance->pconcb->hid,
                         pInstance->pconcb->pspcbkey,
                         &pInstance->StreamInfo.spcb );

  if ( ulrc )
     {
     return ( ulrc );
     }

  pInstance->ulBytes  = pInstance->StreamInfo.spcb.ulBytesPerUnit;
  pInstance->ulMMTime = pInstance->StreamInfo.spcb.mmtimePerUnit;

  return ( ulrc );
}
#endif

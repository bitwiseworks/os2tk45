/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCCONN.C
*
* DESCRIPTIVE NAME: Audio MCD Connector routine.
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*
* FUNCTION:Enable, Disable and Query Connectors
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_PLAY message.
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*
* EXTERNAL REFERENCES:    mciSendCommand  ().
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
* SUBROUTINE NAME: MCICNCT.c
*
* DESCRIPTIVE NAME: Audio MCD Connections.
*
* FUNCTION:Process Connector Message.
*
* NOTES:
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
* INTERNAL REFERENCES:
*                     DestroyStream(),
*                     SetAmpDefaults(),
*                     SetWaveDeviceDefaults().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS **********************/
// This function has an absimal name--rename!!!!!

RC MCIConnector ( FUNCTION_PARM_BLOCK *pFuncBlock)
{
   ULONG            ulrc;              // RC
   ULONG            ulParam1;          // Incoming MCI Flags
   ULONG            ulConnectorType;   // Struct to hold connector flag
   ULONG            ulHoldConFlag;     // flag to hold enable/disable etc.
   ULONG            ulFlagCount;
   INSTANCE *       ulpInstance;        // Local Instance

   PMCI_CONNECTOR_PARMS pConParms;   // MCI Msg Data

   ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;
   ulParam1 = pFuncBlock->ulParam1;

   pConParms = (PMCI_CONNECTOR_PARMS) pFuncBlock->ulParam2;


   /* Check for a valid connector pointer */

   ulrc = CheckMem ( (PVOID) pConParms,
                     sizeof (MCI_CONNECTOR_PARMS),
                     PAG_READ);

   if ( ulrc != MCIERR_SUCCESS )
     {
     return ( MCIERR_MISSING_PARAMETER );
     }

   /* Remove excess flags */

   ulParam1 &= ~( MCI_NOTIFY + MCI_WAIT );

   /* Ensure a command flag was sent */

   if ( !ulParam1 )
     {
     return ( MCIERR_MISSING_FLAG );
     }

   ulConnectorType = ( ulParam1 & MCI_CONNECTOR_TYPE ) ? 1 : 0;

   ulHoldConFlag = ulParam1 & ~( MCI_CONNECTOR_TYPE| MCI_CONNECTOR_INDEX);

   ulFlagCount = ( ulParam1 & MCI_ENABLE_CONNECTOR ) ? 1 : 0;
   ulFlagCount += ( ulParam1 & MCI_DISABLE_CONNECTOR ) ? 1 : 0;
   ulFlagCount += ( ulParam1 & MCI_QUERY_CONNECTOR_STATUS ) ? 1 : 0;

   if ( ulFlagCount > 1 )
      return ( MCIERR_FLAGS_NOT_COMPATIBLE );

   /* To determine a connector's status--we must be connected */

#ifdef CONNECTION
   if ( !ulpInstance->fConnected )
      return ( MCIERR_NO_CONNECTION );
#endif


   /* if they want to set a type */

   if ( ulConnectorType )
         {
         // Investigate!!!!!!!!!
         /* I am not sure that I should be checking indexes, the amp should */
         // Investigate!!!!!!!!!!!!!!!

         if ( ulParam1 & MCI_CONNECTOR_INDEX && ( pConParms->ulConnectorIndex != 1) )
              {
              if ( pConParms->ulConnectorType != MCI_SPEAKERS_CONNECTOR )
                 {
                 return ( MCIERR_INVALID_CONNECTOR_INDEX );
                }
               else
                 {
                 if ( pConParms->ulConnectorIndex != 2 )
                    {
                    return ( MCIERR_INVALID_CONNECTOR_INDEX );
                    }
                 }
              }

          if ( ulHoldConFlag & ~( MCI_ENABLE_CONNECTOR  |
                                  MCI_DISABLE_CONNECTOR |
                                  MCI_QUERY_CONNECTOR_STATUS ) )
             {
             return ( MCIERR_INVALID_FLAG );
             }

         switch( ulHoldConFlag )
                 {
                 /*-------------------------------------------------------------*
                 * Enable connector
                 *-------------------------------------------------------------*/

                 case MCI_ENABLE_CONNECTOR:
                       switch( pConParms->ulConnectorType )
                               {
                               /*-----------------------------------------------------
                               * since we know that we are connected to an amp/mixer
                               * we can pass the command on
                               *----------------------------------------------------*/
                               case MCI_LINE_IN_CONNECTOR:
                               case MCI_AUDIO_IN_CONNECTOR   :
                               case MCI_MICROPHONE_CONNECTOR:
                               case MCI_LINE_OUT_CONNECTOR:
                               case MCI_AUDIO_OUT_CONNECTOR  :
                               case MCI_SPEAKERS_CONNECTOR:
                               case MCI_HEADPHONES_CONNECTOR:
                                  {
// CONNECTOR FEATURE
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
// CONNECTOR FEATURE
                                  ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                                         (USHORT) MCI_CONNECTOR,
                                                         ulParam1,
                                                         (PVOID) pConParms,
                                                         pFuncBlock->usUserParm );
                                  }
                                  break;
                               case MCI_WAVE_STREAM_CONNECTOR:

                                  ulrc = MCIERR_CANNOT_MODIFY_CONNECTOR;
                                  break;
                               case MCI_MIDI_STREAM_CONNECTOR:
                               case MCI_AMP_STREAM_CONNECTOR:
                               case MCI_CD_STREAM_CONNECTOR  :
                               case MCI_VIDEO_IN_CONNECTOR   :
                               case MCI_VIDEO_OUT_CONNECTOR  :
                               case MCI_PHONE_SET_CONNECTOR  :
                               case MCI_PHONE_LINE_CONNECTOR :
                               case MCI_UNIVERSAL_CONNECTOR  :

                                  ulrc = MCIERR_UNSUPPORTED_CONN_TYPE;
                                  break;
                               default:
                                       ulrc = MCIERR_INVALID_CONNECTOR_TYPE;
                                       break;
                               }
                       break;

                 /*-------------------------------------------------------------*
                 * Disable connector
                 *-------------------------------------------------------------*/

                 case MCI_DISABLE_CONNECTOR:
                       switch( pConParms->ulConnectorType )
                               {
                               case MCI_LINE_IN_CONNECTOR:
                               case MCI_AUDIO_IN_CONNECTOR   :
                               case MCI_MICROPHONE_CONNECTOR:
                               case MCI_LINE_OUT_CONNECTOR:
                               case MCI_AUDIO_OUT_CONNECTOR  :
                               case MCI_SPEAKERS_CONNECTOR:
                               case MCI_HEADPHONES_CONNECTOR:

// CONNECTOR FEATURE
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
// CONNECTOR FEATURE
                                  ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                                         (USHORT) MCI_CONNECTOR,
                                                         ulParam1,
                                                         (PVOID) pConParms,
                                                         pFuncBlock->usUserParm );
                                       break;
                               case MCI_WAVE_STREAM_CONNECTOR:

                                  ulrc = MCIERR_CANNOT_MODIFY_CONNECTOR;
                                  break;
                               case MCI_MIDI_STREAM_CONNECTOR:
                               case MCI_AMP_STREAM_CONNECTOR:
                               case MCI_CD_STREAM_CONNECTOR  :
                               case MCI_VIDEO_IN_CONNECTOR   :
                               case MCI_VIDEO_OUT_CONNECTOR  :
                               case MCI_PHONE_SET_CONNECTOR  :
                               case MCI_PHONE_LINE_CONNECTOR :
                               case MCI_UNIVERSAL_CONNECTOR  :

                                  ulrc = MCIERR_UNSUPPORTED_CONN_TYPE;
                                  break;

                               default:
                                       ulrc = MCIERR_INVALID_CONNECTOR_TYPE;
                                       break;
                               } /* switch the connector type */
                       break;
                 /*-------------------------------------------------------------*
                 * QUERY connector
                 *-------------------------------------------------------------*/

                 case MCI_QUERY_CONNECTOR_STATUS :
                       switch( pConParms->ulConnectorType )
                               {
                               case MCI_LINE_IN_CONNECTOR:
                               case MCI_AUDIO_IN_CONNECTOR   :
                               case MCI_MICROPHONE_CONNECTOR:
                               case MCI_LINE_OUT_CONNECTOR:
                               case MCI_AUDIO_OUT_CONNECTOR  :
                               case MCI_SPEAKERS_CONNECTOR:
                               case MCI_HEADPHONES_CONNECTOR:

// CONNECTOR FEATURE
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
// CONNECTOR FEATURE
                                  ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                                         (USHORT) MCI_CONNECTOR,
                                                         ulParam1,
                                                         (PVOID) pConParms,
                                                         pFuncBlock->usUserParm );
                                  break;

                               case MCI_WAVE_STREAM_CONNECTOR:
                                 if ( ulParam1 & MCI_CONNECTOR_INDEX )
                                   {
                                   if (pConParms->ulConnectorIndex != 1)

                                      {
                                      return MCIERR_INVALID_CONNECTOR_INDEX;
                                      }
                                 }
                                 pConParms->ulReturn = TRUE;
                                 ulrc = MAKEULONG( MCIERR_SUCCESS, MCI_TRUE_FALSE_RETURN );
                                 break;
                               case MCI_MIDI_STREAM_CONNECTOR:
                               case MCI_AMP_STREAM_CONNECTOR:
                               case MCI_CD_STREAM_CONNECTOR  :
                               case MCI_VIDEO_IN_CONNECTOR   :
                               case MCI_VIDEO_OUT_CONNECTOR  :
                               case MCI_PHONE_SET_CONNECTOR  :
                               case MCI_PHONE_LINE_CONNECTOR :
                               case MCI_UNIVERSAL_CONNECTOR  :

                                  ulrc = MCIERR_UNSUPPORTED_CONN_TYPE;
                                  break;

                               default:
                                       ulrc = MCIERR_INVALID_CONNECTOR_TYPE;
                                       break;
                               }

                       break;

                 /*-------------------------------------------------------------*
                 * Unknown request
                 *-------------------------------------------------------------*/

                 default:
                         ulrc = MCIERR_MISSING_FLAG;
                         break;
                 } /* switch the connector type */

         } /* if !ulrc */
  else if ( ulParam1 & MCI_CONNECTOR_INDEX )
     {
     if (pConParms->ulConnectorIndex != 1)

        {
        return MCIERR_INVALID_CONNECTOR_INDEX;
        }

     if (  ( ulParam1 & ~( MCI_CONNECTOR_INDEX + MCI_QUERY_CONNECTOR_STATUS) ) > 0  )
       {
       return ( MCIERR_INVALID_FLAG );
       }


     if ( !(ulParam1 & MCI_QUERY_CONNECTOR_STATUS)  )
        {
        return ( MCIERR_MISSING_FLAG );
        }

     pConParms->ulReturn = TRUE;
     ulrc = MAKEULONG( MCIERR_SUCCESS, MCI_TRUE_FALSE_RETURN );

     } /* see if the index only flag is set */
  else
     /*-----------------------------------
     * else there was neither a connector
     * index nor a connector type
     * must be one or both
     *------------------------------------*/

     {
     if ( !(ulParam1 & MCI_QUERY_CONNECTOR_STATUS)  )
        {
        return MCIERR_MISSING_FLAG;
        }

     pConParms->ulReturn = TRUE;
     ulrc = MAKEULONG( MCIERR_SUCCESS, MCI_TRUE_FALSE_RETURN );
     }


   return (ulrc);

} /* MCIConnector */



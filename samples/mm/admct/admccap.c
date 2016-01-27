/****************************************************************************
*
* SOURCE FILE NAME: ADMCCAP.C
*
* DESCRIPTIVE NAME: Reports device capabilities
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*
* NOTES:  This file illustrates the following concepts:
*  A. How to process the capability message commands.
*      These are messages which this MCD supports (such as play, close etc.)
*      Messages (or commands to the caller) such as sysinfo are
*      not supported by this MCD.
*  B. How to process the capability item flag.
*      Items describe particular features (such as the ability to record)
*      which the MCD either does or does not support.
*
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <os2medef.h>                   // MME includes files.
#include <math.h>                       // Standard Math Lib
#include <stdlib.h>                     // Standard Library
#include <ssm.h>                        // SSM spi includes.
#include <mcios2.h>                     // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mmdrvos2.h>                   // MCI Driver include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include <admcfunc.h>
#include <checkmem.h>



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME:              MCDCAPS
*
* DESCRIPTIVE NAME: Waveform Device Capabilities.
*
* FUNCTION: Get Waveform Device Static Capabilities.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_GETDEVCAPS message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES:
*
* EXTERNAL REFERENCES: None.
*
*********************** END OF SPECIFICATIONS ********************************/

RC MCICaps( FUNCTION_PARM_BLOCK *pFuncBlock )
{
  ULONG                  ulrc;                // MME Error Value
  ULONG                  ulParam1;            // Msg Flags
  INSTANCE               *ulpInstance;       // Local Instance
  PMCI_GETDEVCAPS_PARMS  pParams;             // Msg Data Ptr
  ULONG                  ulType;              // Msgs supported.
  ULONG                  ulCapsFlags;         // Mask for Incoming MCI Flags
  PMCI_WAVE_GETDEVCAPS_PARMS   pExtDevCaps;


  /**************************************
  * Derefernce Pointers.
  **************************************/
  ulParam1    = pFuncBlock->ulParam1;

  pParams     = ( PMCI_GETDEVCAPS_PARMS) pFuncBlock->ulParam2;

  ulpInstance = (INSTANCE *) pFuncBlock->ulpInstance;

  ulCapsFlags = ulParam1;

  /* Mask out all of the flags that we support */

  ulCapsFlags &= ~( MCI_WAIT + MCI_NOTIFY +
                    MCI_GETDEVCAPS_MESSAGE + MCI_GETDEVCAPS_ITEM +
                    MCI_GETDEVCAPS_EXTENDED );

  /************************************************
  * If there are any other flags, they are invalid.
  ************************************************/

  if (ulCapsFlags > 0 )
      return ( MCIERR_INVALID_FLAG );

  /********************************************
  * Check for Invalid Combination of flags
  ********************************************/

  if (ulParam1 & MCI_GETDEVCAPS_ITEM && ulParam1 & MCI_GETDEVCAPS_MESSAGE)
      return ( MCIERR_FLAGS_NOT_COMPATIBLE );

  /*********************************************
  * Ensure that the devcaps parms that the caller
  * used a valid.
  *********************************************/
  ulrc = CheckMem ( (PVOID)pParams,
                    sizeof (MCI_GETDEVCAPS_PARMS),
                    PAG_READ | PAG_WRITE );

  if (ulrc != MCIERR_SUCCESS)
      return ( MCIERR_MISSING_PARAMETER );

  ulType = ulParam1 & (MCI_GETDEVCAPS_MESSAGE | MCI_GETDEVCAPS_ITEM | MCI_GETDEVCAPS_EXTENDED);

  /************************************
  * The caller MUST specify either the
  * devcaps message or item flags.
  *************************************/

  if ( !ulType )
     {
     return (MCIERR_MISSING_FLAG);
     }

  if ( ulType == MCI_GETDEVCAPS_MESSAGE )
     {

     switch ( pParams->usMessage )
       {
       /*************************************************
       * The MCD currently supports the messages below:
       * If we support the messages return TRUE and
       * a true/false return.
       *************************************************/

       case MCI_RELEASEDEVICE :
       case MCI_ACQUIREDEVICE :
       case MCI_OPEN          :
       case MCI_PLAY          :
       case MCI_PAUSE         :
       case MCI_SEEK          :
       case MCI_RECORD        :
       case MCI_CLOSE         :
       case MCI_INFO          :
       case MCI_GETDEVCAPS    :
       case MCI_SET           :
       case MCI_STATUS        :
       case MCI_MASTERAUDIO   :
       case MCI_CUE           :
       case MCI_STOP          :
       case MCI_LOAD          :
       case MCI_RESUME        :
       case MCI_SET_POSITION_ADVISE:
       case MCI_SET_CUEPOINT  :
       case MCI_CONNECTOR     :
       case MCI_SET_SYNC_OFFSET:
       case MCI_SAVE          :

            pParams->ulReturn = MCI_TRUE;
            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
           break;

       /******************************
        * List Unsupported Functions
       ******************************/


      case MCI_DEVICESETTINGS:
      case MCI_STEP:
      case MCI_SYSINFO:
      case MCI_UPDATE:
      case MCI_GETTOC:
      case MCI_SPIN:
      case MCI_ESCAPE:
           pParams->ulReturn = MCI_FALSE;
           ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
          break;

      default:
          return ( MCIERR_UNRECOGNIZED_COMMAND );

      } /* switch of message */

    } /* message flag was sent */
  else if ( ulType & MCI_GETDEVCAPS_EXTENDED )
    {
    /*********************************************
    * Ensure that the devcaps parms that the caller
    * used a valid (these are wave specific).
    *********************************************/
    pExtDevCaps = ( PMCI_WAVE_GETDEVCAPS_PARMS ) pParams;
    ulrc = CheckMem ( (PVOID)pExtDevCaps,
                      sizeof (MCI_WAVE_GETDEVCAPS_PARMS ),
                      PAG_READ | PAG_WRITE );

    if (ulrc != MCIERR_SUCCESS)
        return ( MCIERR_MISSING_PARAMETER );

    /*********************************************
    * Determine if the connection network supports
    * the mode the caller wants to query.
    *********************************************/

    ulrc = TestNetwork( ulpInstance, pExtDevCaps );


// 6421--can't call VSD directly--how am I supposed to get this info??????
// solved: try the test connection call and look to see if it succeeds.

//    ulHoldOperation = ulpInstance->ulOperation;
// 6421--when operation_play is gone--so will the if/else statements

//    if ( pExtDevCaps->ulFormatMode == MCI_PLAY )
//       {
//       ulpInstance->ulOperation = OPERATION_PLAY;
//       }
//    else
//       {
////       ulpInstance->ulOperation = OPERATION_RECORD;
//       }
//   /* Call generic routine to fill ssm parms for test_connection call */
//
//   PackSSMStruct(ulpInstance, &mssm );
//
//   /* Setup the caller's parameters in the test connection spcb */
//
//   spcbkey.ulDataType    =  pExtDevCaps->ulFormatTag ;
//   spcbkey.ulDataType    += ( pExtDevCaps->ulChannels << 16);
//   spcbkey.ulDataType    += (pExtDevCaps->ulBitsPerSample << 19);
//   spcbkey.ulDataSubType =  pExtDevCaps->ulSamplesPerSec;
//
//   mssm.pspcbkey = &spcbkey;
//
//   mconn.ulConnectionScheme = MCI_SSM_CONNECTION;
//   mconn.ulConnectionInfo = ( ULONG ) &mssm;
//

//    ulrc = ulpInstance->pfnVSD ( &AMPMIX,
//                                 MCI_GETDEVCAPS,
//                                 pParams->ulItem,
//                                 (LONG)  &pExtDevCaps,
//                                 0L);

//   ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
//                           MCI_TEMPCONNECTION,
//                           MCI_WAIT | MCI_TEST_CONNECTION,
//                           (PVOID) &mconn,
//                           0 );

    /* Restore value as before */

//    ulpInstance->ulOperation = ulHoldOperation;
    pExtDevCaps->ulReturn = !( ulrc) ;

    if ( !ulrc )
       {
       ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
       }

    return ( ulrc );

    } /* extended devcaps */


  else /* if ( ulType == MCI_GETDEVCAPS_ITEM ) */

    {
       switch (pParams->ulItem)
       {

       case MCI_GETDEVCAPS_DEVICE_TYPE:
            pParams->ulReturn = MCI_DEVTYPE_WAVEFORM_AUDIO;
            ulrc = MAKEULONG (ulrc, MCI_DEVICENAME_RETURN);
            break;
// CONNECTION FEATURE--ask if network can stream
       case MCI_GETDEVCAPS_CAN_PLAY:
            {

            pParams->ulReturn = !(CanPlayRecord( ulpInstance, MCIDRV_CONNECT_TARGET ));
            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
            }
            break;
// CONNECTION FEATURE
       case MCI_GETDEVCAPS_CAN_RECORD:
             /***************************************
             * When we loaded the file, we found out
             * from the IO proc if it has the ability
             * to record so examine this flag.
             ****************************************/
// CONNECTION FEATURE--can I pass direction via call????

             if (ulpInstance->ulCapabilities & CAN_RECORD)
                 {
//// 6421--retrive value from init majic.
                 /***********************************************
                 * When the MCI driver connected, it was informed
                 * of the playback/record capabilities of the
                 * connected MCI driver.  Simply return this info.
                 ********************************************** */
//                 pParams->ulReturn = ulpInstance->fCanStreamFrom;
// conn feature--undefined function

                 pParams->ulReturn = !(CanPlayRecord( ulpInstance, MCIDRV_CONNECT_SOURCE ));
//                 ulrc = ulpInstance->pfnVSD ( &AMPMIX,
//                                              MCI_GETDEVCAPS,
//                                              pParams->ulItem,
//                                              (LONG)  &pParams->ulReturn,
//                                              0 );

                 }
             else
                 {
                 pParams->ulReturn = MCI_FALSE;
                 }
            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
            break;

       case MCI_GETDEVCAPS_CAN_SETVOLUME:
           {
#ifdef CONNECTION
           ulrc = FindAmp( ulpInstance );
           if ( ulrc )
              {
              return ( ulrc );
              }
#endif
           ulrc = mciSendCommand ( ulpInstance->usAmpDeviceID,
                                   MCI_GETDEVCAPS,
                                   ulParam1,
                                   (PVOID) pParams,
                                   pFuncBlock->usUserParm);
           }

//            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);

       break;

       /* We have the following static capabilities */

       case MCI_GETDEVCAPS_HAS_AUDIO:
       case MCI_GETDEVCAPS_USES_FILES:
       case MCI_GETDEVCAPS_CAN_STREAM:
            pParams->ulReturn = MCI_TRUE;
            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
            break;

       case MCI_GETDEVCAPS_HAS_VIDEO:
       case MCI_GETDEVCAPS_CAN_EJECT:
       case MCI_GETDEVCAPS_CAN_PROCESS_INTERNAL:
       case MCI_GETDEVCAPS_CAN_LOCKEJECT:
            pParams->ulReturn = MCI_FALSE;
            ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
            break;

       case MCI_GETDEVCAPS_CAN_SAVE:
             /***************************************
             * When we loaded the file, we found out
             * from the IO proc if it has the ability
             * to save so examine this flag.
             ****************************************/
             pParams->ulReturn = (ulpInstance->ulCapabilities & CAN_SAVE) ? 1 : 0;

             ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
             break;

       case MCI_GETDEVCAPS_CAN_RECORD_INSERT:
             /***************************************
             * When we loaded the file, we found out
             * from the IO proc if it has the ability
             * to save so examine this flag.
             ****************************************/
             pParams->ulReturn = (ulpInstance->ulCapabilities & CAN_INSERT) ? 1 : 0;

             ulrc = MAKEULONG (ulrc, MCI_TRUE_FALSE_RETURN);
             break;

       case MCI_GETDEVCAPS_PREROLL_TYPE:
            pParams->ulReturn = MCI_PREROLL_NOTIFIED;
            ulrc = MAKEULONG (ulrc, MCI_PREROLL_TYPE_RETURN );
            break;

       case MCI_GETDEVCAPS_PREROLL_TIME:
            pParams->ulReturn = 0;
            ulrc = MAKEULONG (ulrc, MCI_INTEGER_RETURNED);
            break;

       default:
            return ( MCIERR_UNSUPPORTED_FLAG );

       } /* items of item */

    } /* GETDEVCAPS item flag passed */
  return (ulrc);

} /* MCICaps */



/********************* STARTOF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: LOADSUBS.C
*
* DESCRIPTIVE NAME: Audio MCD Load subroutine file.
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION: Contain subroutines for MCI_LOAD/MCI_OPEN.
*
* NOTES: Concepts illustrated in this source module.
*
*         A. Creating temporary filenames (CheckForValidElement)
*         B. Aborting in process commands (LoadAbortNotify)
*         C. Processing the OPEN_MMIO flag (OpenHandle)
*         D. Rational as to when to open the card in record or
*            playback mode (OpenHandle) (ProcessElement).
*         E. Processing temp files (ProcessElement).
*         F. Opening a file with MMIO (ProcessElement).
*         G. Processing the MCI_READ_ONLY flag (ProcessElement).
*         H. Creating a temporary file ( SetupTempFiles).
*         I. Using mmioSendMessage API to talk to an IO Proc (SetupTempFiles).
*         J. Loading a Vendor Specific Device (OpenInit).
*         H. Retrieving a connection and opening the connected device.
*         I. Stream Handler Setup (StreamSetup)
*         J. Processing MCI_NOTIFY or MCI_WAIT and callback window handles
*            (NotifyWaitSetup).
*         K. Get streaming information from the amp-mixer (GetMixerInfo)
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
* FUNCTIONS DEFINED :  CopyParms
*                      CheckForValidElement
*                      LoadAbortNotifies
*                      OpenHandle
*                      ProcessElement
*                      SetupTempFiles
*                      OpenInit
*                      DetermineConnections
*                      ConnectToAmp
*                      StreamSetup
*                      NotifyWaitSetup
*                      CreateSemaphores
*                      AllocateInstance
*
*
* INTERNAL REFERENCES:    CreateNAssocStream ().
*
* EXTERNAL REFERENCES:    SpiStopStream  ().
*                         SpiAssociate   ().
*                         SpiDisableEvent().
*                         SpiSeekStream  ().
*                         mmioSendMessage().
*                         mmioClose ().
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
#include <vsdcmds.h>


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: CheckForValidElement
*
* DESCRIPTIVE NAME: Ensures that file names are valid
*
* FUNCTION: If the users passed in a file name for open/load this routine
*           will ensure that it is valid.  In addition, it will create a name
*           if the user passed in open element w/no file name
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/



ULONG CheckForValidElement( INSTANCE   *ulpInstance,
                            PSZ        pszFileName,
                            ULONG      ulParam1 )

{

  ULONG  ulrc;           // return code
  ULONG  ulPathLength;   // Contains the max length
  LONG   lReturnCode;

  /* Did the caller pass a filename on MCI_OPEN/MCI_LOAD */

  if ( pszFileName )
     {

     /* Ensure that this filename is valid */

     if (ulrc = CheckMem ( (PVOID) pszFileName, 1, PAG_WRITE))
        {
        return (MCIERR_MISSING_PARAMETER);
        }

     /*************************************************
     * Store the filename for future reference -- i.e.
     * MCI_INFO can request the name of the file
     *************************************************/

     strcpy ( ulpInstance->pszAudioFile, pszFileName);

     /************************************************************
     * Set flag to say that we did not create a new file--thus
     * we do not have to clean up if the user doesn't save
     ************************************************************/

     ulpInstance->ulCreatedName = FALSE;

     }
   else
     {
     /**************************************
     * if the user requests a read only file
     * and we must create it, return error
     **************************************/

     if ( ulParam1 & MCI_READONLY )
        {
        return ( MCIERR_MISSING_PARAMETER );
        }

     /**********************************************
     * Query the default path to place temp files and
     * generate a temporary file name
     **********************************************/

     ulrc = mciQuerySysValue( MSV_WORKPATH, &ulpInstance->pszAudioFile );

     if ( !ulrc )
        {
        return ( MCIERR_INI_FILE );
        }

     ulrc = MCIERR_SUCCESS;

     ulPathLength = CCHMAXPATH;

     /* Have mmio generate a unique filename */

     lReturnCode = DBCSGenerateUniqueFile( ulpInstance->pszAudioFile,
                                           &ulPathLength,
                                           &ulpInstance->hTempFile );

     if ( lReturnCode != MMIO_SUCCESS )
        {
        return ( MCIERR_FILE_NOT_FOUND );
        }

     /***************************************************
     * Because we did create a temporary file we will
     * be responsible for cleaning up in case the
     * caller never calls MCI_SAVE (e.g. a bunch of
     * temp files will be left in the workpath
     * otherwise, so set a flag to indicate this fact.
     **************************************************/

     ulpInstance->ulCreatedName = TRUE;

     ObtainDefaults( ulpInstance );

     } /* else the user did not pass in a name */

  return ( ulrc );


} /* check for valid element */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: LoadAbortNotifies
*
* DESCRIPTIVE NAME: Ensures that notifies are aborted
*
* FUNCTION: Load will stop existing operations (i.e. like play/record ) and
*           ensure that they can complete before loading the new file.
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/

ULONG LoadAbortNotifies( INSTANCE             *ulpInstance,
                         FUNCTION_PARM_BLOCK  *pFuncBlock,
                         ULONG                ulAbortNotify )

{

//  ULONG ulrc;


  if ( ulAbortNotify == TRUE)
     {

     /*****************************************************
     * Save cannot be interrupted or data will be lost, so
     * if there is a save pending, wait for it to complete
     ******************************************************/

     if ( ulpInstance->usNotPendingMsg == MCI_SAVE )
        {
        /* Wait for save to complete */

        DosWaitEventSem( ulpInstance->hThreadSem, (ULONG ) -1 );

        }
      else
        {
        /*****************************************
        * State that we are aborting the operation
        * in process (i.e. play/record).
        *****************************************/

        PostMDMMessage ( MCI_NOTIFY_ABORTED,
                         ulpInstance->usNotPendingMsg,
                         pFuncBlock);

        /******************************************
        * The previous command was most a streaming
        * command, so stop it from operating
        *******************************************/

        ThreadedStop( ulpInstance );

        STRMSTATE = MCI_STOP;

        } /* if pending message not save */


     } /* notify pending is true */

  else

     {
     /***********************************************
     * MCI_STOP may have done a stop_pause to prevent
     * the loss of data--see MCIStop for a more detailed
     * explanation of why--ensure that the stream is
     * stopped before continuing.  If you don't the
     * dreaded stream not stopped will result.
     *************************************************/

     if ( STRMSTATE == STOP_PAUSED )
        {
        StopStream2( ulpInstance, MCIDRV_OUTPUT );

        }

     }

  return ( MCIERR_SUCCESS );

} /* LoadAbortNotifies */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: OpenHandle
*
* DESCRIPTIVE NAME: Sets up for mmio handle passed in on the open
*
* FUNCTION: If a mmio handle is passed in on the open, this routine will
*           do the appropriate setup for it.
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG OpenHandle( INSTANCE  *ulpInstance,
                  ULONG     ulParam1,
                  HMMIO     hmmio )

{

  ULONG ulrc = MCIERR_SUCCESS;

  if (ulParam1 & MCI_OPEN_MMIO)
     {
      if ( !hmmio )
         {
         return ( MMIOERR_INVALID_HANDLE );
         }

      /* Copy the mmio handle to our instance for future use */

      ulpInstance->hmmio = hmmio;

      /*****************************************************
      * Set a flag which indicates that the caller provided
      * a file handle--therefore NEVER close this handle
      *****************************************************/

      ulpInstance->fOpenMMIO = TRUE;

      ulrc = GetAudioHeader (ulpInstance, MMIO_RAWHEADER);

      if ( ulrc == MMIOERR_UNSUPPORTED_MESSAGE )
         {
         ulrc = GetAudioHeader (ulpInstance, 0);
         }


      /***************************************************
      * If the file is non-existent (no data), open the
      * card in record mode so the user can hear if
      * anything is plugged in line-in or the
      * microphone jack (we will assume they want to
      * record since there is no data to play back).
      * By contrast, if there is data, then we will
      * assume that the user wants to be in playback mode
      ****************************************************/
      if ( ulpInstance->ulDataSize != 0 )
         {
//         AMPMIX.ulOperation = OPERATION_PLAY;

         /* 6421--instance variable rather than amp instance */
         ulpInstance->ulOperation = MCIDRV_OUTPUT;
         }
      else
         {
//         AMPMIX.ulOperation = OPERATION_RECORD;

         /* 6421--instance variable rather than amp instance */
         ulpInstance->ulOperation = MCIDRV_INPUT;
         }

      /* Set flag to state that an element has been loaded */

      ulpInstance->fFileExists = TRUE;

      /* Flag to indicate that card has been inited */

     ulpInstance->ulCapabilities =  ( CAN_INSERT | CAN_DELETE | CAN_UNDOREDO +
                                 CAN_SAVE   | CAN_INSERT | CAN_RECORD  );


     /*-------------------------------------------------------
     * Readonly means we cannot write to the file, therefore,
     * there is no need to create temporary files etc. since we
     * cannot change the original file.
     *--------------------------------------------------------*/

     if ( ulParam1 & MCI_READONLY )

        {
        ulpInstance->ulOpenTemp = MCI_FALSE;
        ulpInstance->ulCapabilities &= ~( CAN_SAVE | CAN_RECORD );
        }
     else
        {
        ulpInstance->ulOpenTemp = MCI_TRUE;
        } /* else read only flag was NOT specified */



     } /* if open_mmio is passed in */


  return ( ulrc );

} /* OpenHandle */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: Process Element
*
* DESCRIPTIVE NAME: Opens a file element
*
* FUNCTION: Opens a file and prepares a device to play and record.
*
*
* NOTES:
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
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/

ULONG ProcessElement( INSTANCE *ulpInstance,
                      ULONG    ulParam1,
                      ULONG    ulOperation )
{

   ULONG   ulrc;
   ULONG   ulmmioOpenFlag;

   /******************************************************
   * If the caller wants to open temp, then modify the flags
   * Temp files require that no one else be able to open
   * the same file--so use exclusive.  If we are not
   * using temp files (for instance, if the READ_ONLY flag
   * is passed) then allow anyone to open the file.
   ******************************************************/

   if ( ulpInstance->ulOpenTemp )
      {
      ulmmioOpenFlag = MMIO_READWRITE | MMIO_EXCLUSIVE;
      }
   else
      {
      ulmmioOpenFlag = MMIO_READ | MMIO_DENYNONE;
      }

   /* Ask mmio to open the file */

   ulrc = OpenFile( ulpInstance, ulmmioOpenFlag );

   /* If open preceded smoothly, then the file exists!! */

   if ( !ulrc )
     {
     /* If we created the file, it has no data so go into record mode */

     if ( ulpInstance->ulCreatedName )
        {
        ulpInstance->ulOperation = MCIDRV_INPUT;
        /* ensure that the header is up to date */

        ulrc = SetAudioHeader( ulpInstance );

        }

     else /* the file has data so default to playback mode */
        {
//        AMPMIX.ulOperation = OPERATION_PLAY;

        /* 6421--instance variable rather than amp instance */
        ulpInstance->ulOperation = MCIDRV_OUTPUT;

        }
     } /* if no error on OpenFile */
   else
     {
     /******************************************
     * We will get file not found error if the
     * file does not exist, so try to create it.
     *******************************************/

     if (ulrc == ERROR_FILE_NOT_FOUND)
        {
        /********************************
        * If we need to create a file, then
        * it is necessary to retrieve the
        * default wave header from the
        * ini file settings.
        *********************************/
        ObtainDefaults( ulpInstance );


        /********************************
        * if this is a read only file then
        * we cannot create a new one
        *********************************/

        if ( ulParam1 & MCI_READONLY )
           {
           return ( MCIERR_FILE_NOT_FOUND );
           }

//        AMPMIX.ulOperation = OPERATION_RECORD;
        /* 6421--instance variable rather than amp instance */
        ulpInstance->ulOperation = MCIDRV_INPUT;

        ulpInstance->ulCreateFlag = CREATE_STATE;
        ulmmioOpenFlag = MMIO_CREATE | MMIO_READWRITE | MMIO_EXCLUSIVE;

        /********************************
        * Try to create the element
        *******************************/

        ulrc = OpenFile ( ulpInstance, ulmmioOpenFlag);

        /* If we fail, try to return an MCI error */

        if (ulrc)
           {
           if ( ulrc == ERROR_FILE_NOT_FOUND )
              {
              return ( MCIERR_FILE_NOT_FOUND );
              }
           else
              {
              return (ulrc);
              }
           } /* if an error occurred */

        /* Ensure that the header is up to date */

        SetWaveDeviceDefaults (ulpInstance);
        ulpInstance->ulOperation = MCIDRV_INPUT;
        ulpInstance->mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes = 0;
        ulrc = SetAudioHeader( ulpInstance );

        }
     else
        {
        /* if open or load fail--we have no element */

        ulpInstance->fFileExists = FALSE;

        return ( ulrc );
        }

     } /* else there was an error on the load open */

  /* Ensure header has correct media type */

  ulpInstance->mmAudioHeader.ulMediaType = MMIO_MEDIATYPE_AUDIO;

   return ( ulrc );

} /* ProcessElement */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: SetupTempFiles
*
* DESCRIPTIVE NAME: Prepares for temporary file work
*
* FUNCTION: If the io proc supports temp files and the user has not requested
*           a readonly file, we will try to open a temp file
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG SetupTempFiles( INSTANCE   *ulpInstance,
                      ULONG      ulParam1 )


{

  ULONG ulrc;

  CHAR  TempPath[ CCHMAXPATH ]; // holds path for temp files


  /**********************************************
  * Query the default path to place temp files and
  * pass it on to the IO Proc
  **********************************************/

  ulrc = mciQuerySysValue( MSV_WORKPATH, TempPath );

  if ( !ulrc )
     {
     return (MCIERR_INI_FILE);
     }

  /*****************************************************
  * This message illustrates the use of mmioSendMessage:
  * we are asking the IO Proc that we have loaded to
  * make all subsequent changes temporary (i.e. if no
  * save message is sent, then the file will remain in
  * the original condition.
  *****************************************************/

  ulrc = mmioSendMessage( ulpInstance->hmmio,
                          MMIOM_TEMPCHANGE,
                          ( LONG ) TempPath,
                          0 );
  if (ulrc)
     {
     /* Use mmioGetLastError to get additional detail about the error */

     ulrc = mmioGetLastError( ulpInstance->hmmio );

     /* Cannot write means that the disk is full */

     if (ulrc == MMIOERR_CANNOTWRITE )
        {
        return MCIERR_TARGET_DEVICE_FULL;
        }
     else
        {
        return ( ulrc );
        }

     } /* if there is an error */

  /* Flag to indicate that temporary changes are active */

  ulpInstance->ulUsingTemp = MCI_TRUE;

  return ( ulrc );


} /* SetupTempFiles */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: OpenInit
*
* DESCRIPTIVE NAME: Sets up variables for open
*
* FUNCTION: Initializes variables to be used by the instance
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


void OpenInit( INSTANCE  *ulpInstance )


{

  extern     HID                 hidASource;
  extern     HID                 hidATarget;
  extern     HID                 hidBSource;
  extern     HID                 hidBTarget;

  ULONG      ulrc;


  ulpInstance->ulCapabilities = CAN_RECORD | CAN_SAVE | CAN_INSERT;

  /* Stream hid's */

  ulpInstance->StreamInfo.hidASource = hidASource;
  ulpInstance->StreamInfo.hidATarget = hidATarget;
  ulpInstance->StreamInfo.hidBSource = hidBSource;
  ulpInstance->StreamInfo.hidBTarget = hidBTarget;

  /************************************
  * Wave Record Defaults.
  ***********************************/

//  SetWaveDeviceDefaults (ulpInstance, MCIDRV_INPUT );
  ulpInstance->mmAudioHeader.ulMediaType = MMIO_MEDIATYPE_AUDIO;

  /*--------------------------------------------
  * In case no device is opened, default to
  * the mode requested in the ini file.
  *--------------------------------------------*/

//  AMPMIX.ulOperation = ulpInstance->lDefaultOperation;// Play or Record

  /* 6421--instance variable rather than amp instance */
  ulpInstance->ulOperation = ulpInstance->lDefaultOperation;


  STRMSTATE = NO_STATE;

  /*---------------------------------------------
  * The MMPM2.INI file contains two variables that
  * a streaming MCD should retrieve.  The first one
  * QOS_VALUE (Quality of Service) contains settings
  * which describe the quality of service that the
  * network the user is streaming from will try to
  * support (e.g. GUARANTEED or DONTCARE).  If this
  * quality of service is not available, then another
  * variable (QOSERRORFLAG) describes whether or not
  * to notify the caller.
  *--------------------------------------------------*/

// CONNECTOR FEATURE--can this be moved to DLL init???

  ulrc = mciQuerySysValue( MSV_SYSQOSVALUE, &ulpInstance->lQosValue );

  if ( !ulrc )
     {
     ulpInstance->lQosValue = DONTRESERVE;
     }

  ulrc = mciQuerySysValue( MSV_SYSQOSERRORFLAG, &ulpInstance->lQOSReporting );

  if ( !ulrc )
     {
     ulpInstance->lQOSReporting = ERROR_DEFAULT;
     }


} /* OpenInit */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: DetermineConnections
*
* DESCRIPTIVE NAME: Loads appropriate vendor specific device
*
* FUNCTION: In an ideal world, this function would determine who we are
*           connected to from MDM, load it and let it set up the stream
*           handlers( i.e. probably in the next release).  Currently,
*           it just loads the VSD (i.e. MAJOR hardcode )
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


//ULONG DetermineConnections( INSTANCE           *ulpInstance,
//                            MMDRV_OPEN_PARMS   *pDrvOpenParams )
//
//
//  {
//
//// 6421--note: this is no longer necessary--all handled by the mixer.
//
//  // should this be max path????
//
//  CHAR szLoadError[MAX_ERROR_LENGTH];  // DosLoadModule
//
//  ULONG ulrc;
//
//  /*****************************************************
//  * Note, this section will have to change when improved
//  * connector support is added!!!
//  ******************************************************/
//
//  /*******************************
//  * Load the Devspcfc DLL
//  ********************************/
//
//  if ( (ulrc = DosLoadModule ( szLoadError,
//                               sizeof( szLoadError ),
//                               pDrvOpenParams->szDevDLLName,
//                               &(ulpInstance->hModHandle))))
//     {
//     return ( ulrc );
//     }
//
////  strcpy ((PSZ)AMPMIX.szDriverName, pDrvOpenParams->szDevDLLName );
//
//  /*****************************************
//  * Get the AudioIF Driver Entry point
//  ******************************************/
//
//  ulrc = DosQueryProcAddr ( ulpInstance->hModHandle,
//                            0L,
//                            VSDI,
//                            (PFN *) &ulpInstance->pfnVSD) ;
//
//
//  return ( ulrc );
//
//} /* DetermineConnections */



// CONNECTION FEATURE--this function is no longer used--
// MDM handles all auto connections
#ifndef CONNECTION
/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: ConnectToAmp
*
* DESCRIPTIVE NAME: Connects wave audio to the amp mixer.
*
* FUNCTION: Do the work necessary to ensure that the ini file contains valid
*           information about who we are connected to.  In addition,
*           it will open the amp mixer and receive the associated stream
*           handler from it (next release)?
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG ConnectToAmp( INSTANCE           *ulpInstance,
                    MMDRV_OPEN_PARMS   *pDrvOpenParams,
                    ULONG              ulOpenFlags )

{

  ULONG                ulDeviceTypeID;         // device id of connected device
  ULONG                ulrc;                   // return code variable
//  ULONG                ulHoldError;
  USHORT               usConnLength;           // length of conn struct

  MCI_AMP_OPEN_PARMS   MCIAmpOpenParms;          // MCI AmpMixer Open Parameters
  DEFAULTCONNECTIONS2  DefCon;                   // MCI Connections Block
//  SPCB                 tempspcb;
  MMAUDIOHEADER        mmaudioheader;
  VSD_AUDIOOPEN_PARMS  vsdOpen;
  CONCB                concb;

  /* Inform the amp how we wish to initialize the device */

  memmove( &mmaudioheader, &ulpInstance->mmAudioHeader, sizeof( MMAUDIOHEADER ));
  vsdOpen.pHeader = ( PVOID) &mmaudioheader;
  vsdOpen.ulFlags = VSD_AUDIO_OPEN;
  vsdOpen.ulReserved2 = (ULONG) &concb;


  /*****************************
  * Obtain WaveAudio Device ID
  *****************************/
  ulpInstance->usWaveDeviceID = pDrvOpenParams->usDeviceID;

  ulDeviceTypeID = MAKEULONG ( MCI_DEVTYPE_WAVEFORM_AUDIO,
                               pDrvOpenParams->usDeviceOrd);

  /******************************************************
  * Ensure that the INI file contains the right device id
  ******************************************************/

  if ( pDrvOpenParams->usDeviceType != MCI_DEVTYPE_WAVEFORM_AUDIO )
     {
     return ( MCIERR_INI_FILE );
     }

  usConnLength = sizeof(DEFAULTCONNECTIONS2);

  ulrc =  mciQueryDefaultConnections ( ulDeviceTypeID,
                                       &DefCon,
                                       &usConnLength);

  /******************************************************
  * Ensure that the INI file says that we are connected
  * to an amp mixer.  If it says that we are connected
  * to ourselves, return an error.
  ******************************************************/

  if ( ULONG_LOWD( DefCon.dwDeviceTypeID2 ) == MCI_DEVTYPE_WAVEFORM_AUDIO )
     {
     return ( MCIERR_INI_FILE );
     }

  /******************************
  * Open an AMP/MIXER Instance
  ******************************/

  MCIAmpOpenParms.pszDeviceType = (PSZ) DefCon.dwDeviceTypeID2;

  MCIAmpOpenParms.hwndCallback = (ULONG) NULL;

// 6421 --no need to retrive this information any more
// we are truly device independent

//  GetPDDName (DefCon.dwDeviceTypeID2, szPDDName);

//  strcpy ( ulpInstance->szAudioDevName, szPDDName);

  /* Copy the name of the pdd + vsd dll to amp structure */

//  strcpy ((PSZ) AMPMIX.szDeviceName,
//          ulpInstance->szAudioDevName);

//  strcpy ((PSZ) ulpInstance->StreamInfo.AudioDCB.szDevName,
//          ulpInstance->szAudioDevName);

  /*******************************************
  * The current amp/mixer uses an undocumented
  * structure.  Pass this structure in on the
  * open also.
  ********************************************/

  MCIAmpOpenParms.pDevDataPtr = (PVOID) &vsdOpen;


  /* Open what we are connected to--usually the amp */

  ulrc = mciSendCommand (0,
                         MCI_OPEN,
                         MCI_OPEN_TYPE_ID| MCI_WAIT| ulOpenFlags,
                         (PVOID) &MCIAmpOpenParms,
                         0);

// CODEC change
   /*--------------------------------------------
   * It is possible that the VSD may require a
   * best fit.  Try with the best fit parameters.
   *-------------------------------------------*/
     if (ulrc)

        {
#ifndef CONNECTION
        if ( ulrc == MCIERR_UNSUPP_FORMAT_TAG )
           {
           /* Open what we are connected to--usually the amp */

           ulrc = mciSendCommand (0,
                                  MCI_OPEN,
                                  MCI_OPEN_TYPE_ID| MCI_WAIT| ulOpenFlags,
                                  (PVOID) &MCIAmpOpenParms,
                                  0);
           }
       if ( ulrc )
#endif
          {
          return ( ulrc );
          }
        ulpInstance->ulRealTimeTranslation = MMIO_REALTIME;
        memmove( &ulpInstance->mmAudioHeader, &mmaudioheader, sizeof( MMAUDIOHEADER ) );
        } /* If the amp-mixer could not deal with the data type */
     else
        {
        ulpInstance->ulRealTimeTranslation = MMIO_NONREALTIME;
        }
// CODEC feature--removed functionality


  /* Remember the amp device id so that we can close it */

  ulpInstance->usAmpDeviceID = MCIAmpOpenParms.usDeviceID;

#ifndef CONNECTION
  ulrc = GetMixerInfo( ulpInstance, &concb );

  if ( ulrc )
     {
     /*-----------------------------------------
     * Ensure that the amp-mixer is CLOSED!!!!!!
     * Else an error will lock the device.
     *------------------------------------------*/

     mciSendCommand ((WORD)ulpInstance->usAmpDeviceID,
                     MCI_CLOSE,
                     MCI_WAIT,
                     0,
                     0);
     }
#endif

  return ( ulrc );


} /* ConnectToAmp */
#endif


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: StreamSetup
*
* DESCRIPTIVE NAME: Gets the stream handlers
*
* FUNCTION: Do the work necessary to load the correct stream handlers.  Currently
*           this is hardcoded, however, in the future we will obtain this info
*           from connections/ampmix etc(?)
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG StreamSetup ( void )

{
extern     HID                 hidASource;
extern     HID                 hidATarget;

extern     HID                 hidBSource;
extern     HID                 hidBTarget;


  ULONG ulrc = MCIERR_SUCCESS;


  /*********************************************************
  * Get 'A' stream Handler Handles for Source & target operations
  * The file system stream handler is the default 'A' handler
  * but the memory stream handler will be used for playlists.
  **********************************************************/


     if (ulrc = SpiGetHandler((PSZ)DEFAULT_SOURCE_HANDLER_NAME,
                               &hidASource,
                               &hidATarget))
        {
        return ( ulrc );
        }

// CONNECTION FEATURE
  /***********************************************************
  * Get 'B' stream Handler Handles for Source & target operations
  * The audio stream handler is considered the B stream handler
  * since it will usually be the target.
  *************************************************************/

//  ulrc = SpiGetHandler( (PSZ)DEFAULT_TARGET_HANDLER_NAME,
//                        &hidBSource,
//                        &hidBTarget);
// CONNECTION FEATURE

  return ( ulrc );

} /* StreamSetup */



/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: CreateSemaphores
*
* DESCRIPTIVE NAME: Creates semaphores for instance
*
* FUNCTION: Creates all the semaphores needed for playing, recording and
*           serializing access to necessary instance data.
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG CreateSemaphores( FUNCTION_PARM_BLOCK   *pFuncBlock )


{

   ULONG        ulrc;
   INSTANCE     *pInstance = ( INSTANCE * ) pFuncBlock->pInstance;

   /*********************************************
   * Create an event semaphore used in the
   * streaming operations (see admcplay, admcrecd).
   *********************************************/

   ulrc = DosCreateEventSem ( (ULONG) NULL,
                              (PHEV) &pInstance->hEventSem,
                              DC_SEM_SHARED,
                              FALSE );

   if (ulrc)
      {
      return (ulrc);
      }

   /************************************************
   * Create an event semaphore used to synchronize
   * various threads (See admcplay.c + admcrecd.c).
   *************************************************/

   ulrc = DosCreateEventSem ( (ULONG) NULL,
                              (PHEV) &pInstance->hThreadSem,
                              DC_SEM_SHARED,
                              FALSE );
   if (ulrc)
      {
      return (ulrc);
      }

   ulrc = DosCreateMutexSem ( (ULONG) NULL,
                              (PHEV) &pInstance->hmtxDataAccess,
                              DC_SEM_SHARED,
                              FALSE);
   if (ulrc)
      {
      return (ulrc);      /* Failed Sem Create */
      }

   /************************************************
   * Create semaphore which will prevent MCI_CLOSE
   * from freeing instance before all threads are
   * done processing.
   ************************************************/

   ulrc = DosCreateMutexSem ( (ULONG) NULL,
                              (PHEV) &pInstance->hmtxCloseAccess,
                              DC_SEM_SHARED,
                              FALSE);
   if (ulrc)
      {
      return (ulrc);      /* Failed Sem Create */
      }



   /* Create semaphore to ensure that only one thread can abort another */

   ulrc = DosCreateMutexSem ( NULL,
                              &pInstance->hmtxNotifyAccess,
                              DC_SEM_SHARED,
                              FALSE);
   if (ulrc)
      {
      return (ulrc);      /* Failed Sem Create */
      }

   /* Create semaphore to ensure that MCIDRV_SAVE is never locked out*/

   ulrc = DosCreateMutexSem ( NULL,
                              &pInstance->hmtxSaveAccess,
                              DC_SEM_SHARED,
                              FALSE);


   /***********************************************
   * No need to check the return code, since we have
   * to do a return to leave anyway
   ************************************************/

   return ( ulrc );


} /* Create Semaphores */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: NotifyWaitSetup
*
* DESCRIPTIVE NAME: Prepares instance for either notify/wait/asynchronous operation
*
* FUNCTION: Stores callbacks and sets up the instance for either notify, wait
*           or asynchronous operation.
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


void NotifyWaitSetup( FUNCTION_PARM_BLOCK *pFuncBlock,
                      USHORT              usMessage,
                      ULONG               ulParam1,
                      ULONG               ulParam2 )

{


  if (!(ulParam1 & MCI_WAIT) )
      {

      if (usMessage != MCI_OPEN)
          {
          pFuncBlock->pInstance->fFakeNotify = FALSE;


          /*****************************
          * Default to a Notify
          ******************************/
          pFuncBlock->ulParam1 |= MCI_NOTIFY;


          /********************************
          * Turn The Notify Flag On
          *********************************/

          pFuncBlock->ulNotify = TRUE;

          /*********************************
          * Get The Window Callback Handle
          *********************************/

          if ( ulParam1 & MCI_NOTIFY )
             {
             if ( usMessage == MCI_PLAY || usMessage == MCI_RECORD )
               {
               pFuncBlock->pInstance->hwndOldCallBack = pFuncBlock->pInstance->hwndCallBack;
               pFuncBlock->pInstance->hwndCallBack = (((PMCI_GENERIC_PARMS)ulParam2)->hwndCallback);
               }
             else
               {
               pFuncBlock->hwndCallBack = (((PMCI_GENERIC_PARMS)ulParam2)->hwndCallback);
               }
             }
          else
          /********************************************************
          * Even if the caller did not specify the MCI_NOTIFY
          * flag, and they didn't specify wait, we will treat
          * this call just like a notify, EXCEPT that when we
          * call MDMDriverNotify, we will pass a bogus
          * callback.  This prevents us from having to do a
          * tremendous amount of additional processing and the
          * caller is happy because the work is done asychronously
          *********************************************************/

             {
             /*---------------------------------------------------------
             * Set flag to indicate that although we are going
             * to act like it is a notify, the caller did not
             * really pass in the notify flag.  This will be
             * important when we have a situation like a play with
             * a notify, followed by a play without a wait or notify.
             * In this scenario, an aborted message should be
             * posted by the first play.
             *--------------------------------------------------------*/

             pFuncBlock->pInstance->fFakeNotify = TRUE;

             if ( usMessage == MCI_PLAY || usMessage == MCI_RECORD )
               {
               pFuncBlock->pInstance->hwndOldCallBack = pFuncBlock->pInstance->hwndCallBack;
               pFuncBlock->pInstance->hwndCallBack = 0;
               }
             else
               {
               pFuncBlock->hwndCallBack = 0;
               }

             } /* if !MCI_NOTIFY */

          } /* Message .NE. MCI Open */

       } /* Wait flag is not on */

  return;

} /* NotifyWaitSetup */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: AllocateInstance
*
* DESCRIPTIVE NAME: allocates memory for the instance etc.
*
* FUNCTION: Stores callbacks and sets up the instance for either notify, wait
*           or asynchronous operation.
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG AllocateInstance( FUNCTION_PARM_BLOCK *pFuncBlock )



{

  extern HHUGEHEAP     heap;                // Global MCD Heap


  AcquireProcSem ();

  /**************************************
  * Waveform audio instance structure
  ****************************************/

  if (!(pFuncBlock->pInstance = HhpAllocMem ( heap,
                                              sizeof (INSTANCE))))
     {
     ReleaseProcSem ();
     return ( MCIERR_OUT_OF_MEMORY );
     }

  ReleaseProcSem ();

  return ( MCIERR_SUCCESS );

} /* AllocateInstance */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: ObtainDefaults
*
* DESCRIPTIVE NAME: Determine default values from MMPM2.INI (if necessary)
*
* FUNCTION: This function will retrieve the default wave settings from the device
*           specific settings of the MMPM2.ini file.
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/


ULONG ObtainDefaults(  INSTANCE   *ulpInstance)



{
 ULONG ulrc;

  if ( !(ulpInstance->ulFlags & OBTAINDEFAULTS ) )
     {
     ulrc = GetDefaults( ulpInstance->szDevParm, ulpInstance );

     if ( ulrc )
        {
        return ( ulrc );
        }

     SetWaveDeviceDefaults (ulpInstance );
     }
  ulpInstance->ulFlags |= OBTAINDEFAULTS;
  return ( MCIERR_SUCCESS );

} /* ObtainDefaults */


/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: GetMixerInfo
*
* DESCRIPTIVE NAME: Get streaming information from the amp-mixer
*
* FUNCTION: Obtain DCB, hid and other information necessary to create a stream
*           from the amp-mixer.
*
*
*
* NOTES:
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
* INTERNAL REFERENCES: HhpAllocMem().
*
* EXTERNAL REFERENCES:
*
*********************** END OF SPECIFICATIONS *******************************/

ULONG GetMixerInfo( INSTANCE   *ulpInstance, 
                    PCONCB     pconcb)
{
//  ULONG                ulConnectFlag;
  ULONG                ulrc;

//  CONCB                concb;

  extern     HID       hidBTarget;
  extern     HID       hidBSource;
  HID                  hidProtocol;



//  if ( ulpInstance->ulOperation == MCIDRV_INPUT )
//     {
//     ulConnectFlag = MCIDRV_CONNECT_SOURCE;
//     }
//  else
//     {
//     ulConnectFlag = MCIDRV_CONNECT_TARGET;
//     }
//
//  concb.pMediaHeader = (PVOID) &ulpInstance->mmAudioHeader;
//  concb.ulConnectorType = MCI_WAVE_STREAM_CONNECTOR;
//  concb.ulConnectorIndex = 1;
//
//  ulrc = mciSendCommand( ulpInstance->usAmpDeviceID,
//                       MCIDRV_CONNECT,
//                       ulConnectFlag,
//                       &concb,
//                       0);
//
//  if ( ULONG_LOWD(ulrc) != MCIERR_SUCCESS )
//       {
//       return ( ulrc );
//       }
//


  /* Save the hid from the connected device--use for spcbkeys etc. */

  if ( ulpInstance->ulOperation == MCIDRV_INPUT )
     {
     hidBSource = hidProtocol = pconcb->hid;
     }
  else
     {
     hidBTarget = hidProtocol = pconcb->hid;
     }

  memmove( &ulpInstance->StreamInfo.SpcbKey, pconcb->pspcbkey, sizeof( SPCBKEY) );
  ulpInstance->pdcb = pconcb->pdcb;

  /*----------------------------------------------------------
  * Retrieve the mmtime and byte per unit numbers from the
  * SPCB key so that can calculate values for seeks etc.
  * This information will also be used to communicate network
  * usage parameters to network io procs.
  *---------------------------------------------------------*/
  ulrc = SpiGetProtocol( hidProtocol, &STREAM.SpcbKey, &ulpInstance->StreamInfo.spcb );

  if ( ulrc )
     {
     return ( ulrc );
     }

  ulpInstance->ulBytes  = ulpInstance->StreamInfo.spcb.ulBytesPerUnit;
  ulpInstance->ulMMTime = ulpInstance->StreamInfo.spcb.mmtimePerUnit;


// CONNECTOR FEATURE--Generic Datatype
#ifdef CONNECTION
  ulpInstance->StreamInfo.SpcbKey.ulDataType = DATATYPE_GENERIC;
  ulpInstance->StreamInfo.SpcbKey.ulDataSubType = 0;
  ulpInstance->StreamInfo.SpcbKey.ulIntKey = 0;
#endif
// CONNECTOR FEATURE


  /* Stick in Instance Ptr in Modified EVCB */

  ulpInstance->StreamInfo.Evcb.ulpInstance = (ULONG)ulpInstance;
  /*---------------------------------------------------
  * If the file is readonly, we are guaranteed that
  * it will not change.  In addition, if the file
  * is small, SSM may be allocating too many buffers
  * (since they are unaware of the file size).
  * Check to see if we can reduce memory consumption.
  *--------------------------------------------------*/
  if ( ulpInstance->ulFlags & READ_ONLY_FILE )
     {
     InstallProtocol( ulpInstance );
     }

  return ( ulrc );

} /* GetMixerInfo */


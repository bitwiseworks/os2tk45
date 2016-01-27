/********************* START OF SPECIFICATIONS *********************
*
* SUBROU1TINE NAME:  MCICLOS.C
*
* DESCRIPTIVE NAME: Close Waveform Device.
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:  A streaming MCD should do the following on a close.
*
*  A. Stop all commands which are active on another thread(s).
*  B. Destroy all active streams.
*  C. Close all open files.
*  D. Delete any temporary files.
*  E. Close any connected devices (such as an amp-mixer).
*  F. If MCI_NOTIFY was used, notify the caller of completion.
*
* NOTES: When The Process is killed all threads Die.  Thus
*        we check if the MCI_CLOSE_EXIT flag is passed in.  If this
*        flag is passed, then will NOT do a stop before destroying
*        the stream.   A stop in this scenario will cause a hang since
*        the streaming subsystem has lost necessary threads.
*
* ENTRY POINTS:
*
* INPUT: MCI_CLOSE message.
*
* EXIT-NORMAL: Instance deleted, Device released.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS: None.
*
* INTERNAL REFERENCES: PostMDMMessage(), DestroyStream().
*
* EXTERNAL REFERENCES: SpiDestroyStream ()    - SSM SPI.
*                      mdmDriverNotify  ()    - MDM API.
*                      mciSendCommand   ()    - MDM API.
*                      mciConnection    ()    - MDM API.
*                      mmioClose        ()    - MMIO API.
*                      HhpFreeMem       ()    - Heap Manager.
*                      DosCloseEventSem ()    - OS/2 API.
*                      DosCloseMutexSem ()    - OS/2 API.
*                      DosFreeModule    ()    - OS/2 API.
*
*********************** END OF SPECIFICATIONS **********************/

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

RC MCIClos (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG               ulrc;           // RC
  ULONG               ulParam1;       // Incoming MCI Flags
  ULONG               ulAbortNotify = FALSE; // whether to abort play/record operation
//  ULONG               ulCount;
  INSTANCE            *ulpInstance;   // Active Instance
  ULONG               ulCloseFlags;   // Mask for MCI Flags

  MTIME_EVCB           *pCuePoint;
  MTIME_EVCB           *pTempCuePoint;

  extern     HID     hidBTarget;  /* Stream handler to install to */

  /*******************************************
  * Dererence Pointers From Thread Block
  ********************************************/

  ulParam1 =   pFuncBlock->ulParam1;
  ulpInstance = (INSTANCE *)pFuncBlock->ulpInstance;

  /***************************
  * Intialize Variables
  ***************************/
  ulCloseFlags = ulParam1;

  /************************************
  * Check for Illegal Flags
  *************************************/

  ulCloseFlags &= ~(MCI_WAIT + MCI_NOTIFY + MCI_CLOSE_EXIT) ;

  if (ulCloseFlags > 0)
      return MCIERR_INVALID_FLAG;

  /****************************************
  * If There are any Pending Notifies
  * Post Abort Message for Those Operations
  *****************************************/

  GetNotifyAbortAccess( ulpInstance, &ulAbortNotify );

  /******************************************************
  * Fix for MMPM/2 AVC--they do repeated close and after
  * approximately 80 closes, due to some OS/2 scheduling
  * quirk, close would free the instance before the thread
  * finished processing.  Therefore, require close to
  * acquire the exclusive semaphore before freeing
  * instance.
  ********************************************************/


   DosRequestMutexSem( ulpInstance->hmtxCloseAccess, -1 );

   /* Release semaphore since this will be the last command for this instance */

   DosReleaseMutexSem( ulpInstance->hmtxCloseAccess );



  if ( ulAbortNotify == TRUE)
     {

     /* Stop the command on another thread */

     GenericThreadAbort( ulpInstance, pFuncBlock, ulParam1  );


//     if ( ulpInstance->usNotPendingMsg == MCI_SAVE )
//        {
//        /******************************************
//        * Save is a non-interruptible operation
//        * wait for completion
//        ******************************************/
//
//        DosWaitEventSem( ulpInstance->hThreadSem, (ULONG ) -1 );
//
//        }
//      else
//        {
//        /******************************************
//        * If any other operation is processing, then
//        * terminate them (i.e. even if we are
//        * recording, the user does not want to save
//        * the file so just send the aborted message
//        ******************************************/
//
//        PostMDMMessage (MCI_NOTIFY_ABORTED,
//                        ulpInstance->usNotPendingMsg,
//                        pFuncBlock);
//
//        /***********************************************
//        * Both play and record will create a thread.
//        * We need to ensure that the thread will be
//        * removed on a close.
//        * If this is an exit list, a stop will cause a
//        * hang since some necessary threads will not
//        * exist anymore.
//        ************************************************/
//
//      if (  !(ulParam1 & MCI_CLOSE_EXIT)  )
//         {
//         /* Stop the command on another thread */
//
//         ThreadedStop( ulpInstance );
//         }
//
//        ulpInstance->usNotifyPending = FALSE;
//
//        }
     } /* Pending Notifies */

  /***************************************
  * Destroy the Stream if we have
  * created one
  ***************************************/
// CONNECTION FEATURE
#ifndef CONNECTION
  if (ulpInstance->StreamInfo.hStream != (ULONG) NULL)
     {
     //6421
     ulrc = DestroyStream ( ulpInstance);
     }
#endif
// CONNECTION FEATURE

  /* If we installed a protocol removed it */
  if ( ulpInstance->fInstalledProtocol )
     {
     /* Install the protocol */

     SpiInstallProtocol( hidBTarget,
                         &STREAM.SpcbKey,
                         &ulpInstance->StreamInfo.spcb,
                         SPI_DEINSTALL_PROTOCOL );

     }
  /************************************************
  * If a file was previously opened then close it
  * (however, if it was opened with the OPEN_MMIO
  * flag, then don't close it.  That is the
  * applications responsibility).
  ************************************************/

  ulrc = CloseFile( ulpInstance );

  if ( ulrc )
     {
     return ( ulrc );
     }

  /*------------------------------------------
  * Free the memory of any cuepoints we have
  * created.
  *------------------------------------------*/

  if ( (ulpInstance->usCuePt == TRUE) ||
       (ulpInstance->usCuePt == EVENT_ENABLED))
     {
     pCuePoint = CUEPOINT;

     while ( pCuePoint )
        {
        pTempCuePoint = pCuePoint;
        pCuePoint = pCuePoint->pNextEVCB;
        CleanUp ((PVOID) pTempCuePoint);
        } /* while there are cue points to remove */

     } /* if we have cue points in use */

  /* Remove any undo nodes if they were allocated */

  RemoveUndoNodes( ulpInstance );


  // 6421--we no longer always know if we are connected

// CONNECTION FEATURE
#ifndef CONNECTION
//  if ( ulpInstance->fConnected )
//     {
     /*******************************************
     * Break The Default Amp/Mixer Connection
     * The connected device will not allow a close
     * before the connection is broken.
     ********************************************/

     ulrc = mciConnection (ulpInstance->usWaveDeviceID,
                           1,
                           ulpInstance->usAmpDeviceID,
                           1,
                           MCI_BREAKCONNECTION);

     /****************************************
     * Close The what we are connected to.
     * Most often this will be the amp/mixer.
     * However, if we are connected to another
     * device, close it.  This call will remove
     * any ability to talk to the audio device.
     *****************************************/

     ulrc = mciSendCommand ((WORD)ulpInstance->usAmpDeviceID,
                            MCI_CLOSE,
                            MCI_WAIT,
                            0,
                            0);
//     }
#endif
// CONNECTION FEATURE

  /*******************************************
  * Close Internal Semaphores
  *******************************************/
  DosCloseEventSem (ulpInstance->hEventSem);
  DosCloseEventSem (ulpInstance->hThreadSem);
  DosCloseMutexSem (ulpInstance->hmtxDataAccess);
  DosCloseMutexSem (ulpInstance->hmtxNotifyAccess);
  DosCloseMutexSem (ulpInstance->hmtxSaveAccess);    //@12182

// avc
  DosCloseMutexSem (ulpInstance->hmtxCloseAccess);


  /*****************************************
  * Post Close Complete message If needed
  *****************************************/

  if (ulParam1 & MCI_NOTIFY)
     {
      PostMDMMessage ( MCI_NOTIFY_SUCCESSFUL, MCI_CLOSE, pFuncBlock);
     }

  /*******************************************
  * Free Thread Parm Block & Assoc Pointers
  *******************************************/
  if ( pFuncBlock->pInstance->pcodecini )
     {
     CleanUp( ( PVOID ) pFuncBlock->pInstance->pcodecini );
     }
  CleanUp ((PVOID) pFuncBlock->pInstance);

  return (MCIERR_SUCCESS);     // Return Success
} /* MCIClose */


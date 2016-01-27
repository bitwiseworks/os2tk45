/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  CDAUDIO.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD AUDIO MCI DRIVER                                   */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the hardware independent code that the     */
/*            CD Audio MCI Driver uses.  This includes the entry point for  */
/*            the MDM/MCI calls to the DLL and the global structures,       */
/*            utilities and parameter checking.  The Process Command        */
/*            functions are in CDAUDPRO.C                                   */
/*                                                                          */
/*                                                                          */
/* NOTES:  The hardware dependent code is found in file IBMCDROM.C.         */
/*         This MCD (MCI Driver) is an example of a non-streaming MCD       */
/*         with simple support.                                             */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       mciDriverEntry - performs component specific messages of this MCI  */
/*                        Driver.                                           */
/*                                                                          */
/* OTHER FUNCTIONS:                                                         */
/*       pre_process_msg - Pre-Process the requested command message.       */
/*       process_msg   - Process the requested command message.             */
/*       verify_entry  - Verify that entry parameters are valid.            */
/*       QMAudio       - Query master audio's current settings.             */
/*       Register      - Register drive.                                    */
/*       ReRegister    - Register disc and tracks.                          */
/*       VSDReturn     - Process return information from VSD.               */
/*       SetTrackInst  - Set Track info in Instance, seek to start.         */
/*       ValPointer    - Validate address to record structures.             */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES
#include <os2.h>
#include <string.h>
#define INCL_MCIOS2
#include <os2me.h>
#include <cdaudos2.h>
#include "cdaudibm.h"
#include "hhpheap.h"

extern PVOID          CDMC_hHeap;

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  mciDriverEntry                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI Driver Entry                                      */
/*                                                                          */
/* FUNCTION:  Processes the CD Audio component commands.                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PVOID  lpInstance -- Pointer to device handle.                      */
/*      USHORT usMessage  -- Command message.                               */
/*      ULONG  ulParam1   -- Flag for this message.                         */
/*      PVOID  pParam2    -- Pointer to data record structure.              */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*      MCIERR_INI_FILE   -- Corrupted file, invalid open or connection.    */
/*      MCIERR_DEVICE_NOT_READY   -- Device was not ready, no disc.         */
/*      MCIERR_DEVICE_OPEN        -- Error on open.                         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INVALID_BUFFER     -- Buffer size was too small.             */
/*      MCIERR_OUTOFRANGE         -- Invalid value or stream error.         */
/*      MCIERR_NO_CONNECTION      -- No way to play, no stream/DAC          */
/*      MCIERR_OUT_OF_MEMORY      -- Out of memory.                         */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid pointer in Parms.              */
/*      MCIERR_MISSING_PARAMETER  -- Invalid PARMS pointer.                 */
/*      MCIERR_INVALID_DEVICE_NAME  -- Illegal drive name.                  */
/*      MCIERR_UNSUPPORTED_FUNCTION -- Invalid command message.             */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*      MCIERR_CANNOT_LOAD_DRIVER -- Unable to load VSD.                    */
/*      MCIERR_INVALID_CUEPOINT       --  Unable to locate event.           */
/*      MCIERR_CUEPOINT_LIMIT_REACHED --  No more room to add events.       */
/*      MCIERR_DUPLICATE_CUEPOINT     --  Duplicate cuepoint.               */
/*      MCIERR_INVALID_ITEM_FLAG  -- Invalid item specified.                */
/*      MCIERR_INVALID_SPEED_FORMAT_FLAG -- Unknown speed type specified.   */
/*      MCIERR_INVALID_TIME_FORMAT_FLAG  -- Unknown time type specified.    */
/*      MCIERR_INVALID_CONNECTOR_INDEX -- Invalid connector specified.      */
/*      MCIERR_INVALID_CONNECTOR_TYPE  -- Invalid connector specified.      */
/*      MCIERR_UNSUPPORTED_CONN_TYPE   -- Unsupported connector specified.  */
/*      MCIERR_INVALID_CALLBACK_HANDLE -- Invalid call back handle.         */
/*      MCIERR_INVALID_AUDIO_FLAG -- Unknown audio flag specified.          */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*                                                                          */
/* NOTES:  Assumes that if NOT MCI_WAIT then it is MCI_NOTIFY, no check     */
/*         if no flag is passed.                                            */
/*                                                                          */
/****************************************************************************/

ULONG APIENTRY mciDriverEntry(PVOID lpInstance, USHORT usMessage,
                              ULONG ulParam1, PVOID pParam2, USHORT usUserParm)
{
   ULONG rc;

   /* verify inputs */
   rc = verify_entry((PINST)lpInstance, usMessage, ulParam1, &pParam2);

   if (!rc)
      rc = process_msg((PINST)lpInstance, usMessage, &ulParam1,
                       pParam2, usUserParm);

   /* if successful and notify is requested then send notification */
   if (!(ULONG_LOWD(rc)) && ulParam1 & MCI_NOTIFY)
   {
      mdmDriverNotify(((struct instance_state *)lpInstance)->usDeviceID,
               ((MCI_GENERIC_PARMS *)pParam2)->hwndCallback, MM_MCINOTIFY,
               usUserParm, MAKEULONG(usMessage, MCI_NOTIFY_SUCCESSFUL));

   }  /* of if no error and NOTIFY is set */

   return(rc);

}  /* of mciDriverEntry() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  pre_process_msg                                        */
/*                                                                          */
/* DESCRIPTIVE NAME:  Pre-process messages.                                 */
/*                                                                          */
/* FUNCTION:  The code for this function was in process_msg but was         */
/*            placed here to make it neater.                                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst       -- instance pointer.                             */
/*      USHORT *pusMessage -- requested action to be performed.             */
/*      ULONG  ulParam1    -- flag for this message.                        */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS       -- action completed without error.             */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG pre_process_msg(PINST pInst, USHORT *pusMessage, ULONG ulParam1)
{
   ULONG rc = MCIERR_SUCCESS;

   if (*pusMessage == MCIDRV_RESTORE)
   {
      /* process shareable flag */
      if (ulParam1 & MCI_OPEN_SHAREABLE)
         pInst->ulMode |= CDMC_SHAREABLE;
      else
         pInst->ulMode &= ~CDMC_SHAREABLE;

      if (pInst->usStatus < SUSPEND)
         *pusMessage = 0;                  //ignore restores without a save
   }  /* of id message is RESTORE */

   /* re-register if necessary */
   if (pInst->usStatus <= NODISC)       //try registering disc and tracks
      ReRegister(pInst);                //ignore RC, let message process state

   /* re-restore if necessary */
   if (pInst->usStatus >= SUSPEND && *pusMessage == MCIDRV_SAVE)
      *pusMessage = 0;                  //ignore saves when saved

   return(rc);

}  /* of pre_process_msg() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  process_msg                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Control Message                               */
/*                                                                          */
/* FUNCTION:  Processes the CD Audio commands.                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- instance pointer.                              */
/*      USHORT usMessage  -- requested action to be performed.              */
/*      ULONG  *pulParam1 -- pointer to flag for this message.              */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_INI_FILE   -- corrupted file, invalid open or connection.    */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_OUTOFRANGE         -- Invalid value.                         */
/*      MCIERR_NO_CONNECTION      -- No way to play, no stream/DAC.         */
/*      MCIERR_OUT_OF_MEMORY      -- Out of memory.                         */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*      MCIERR_INVALID_DEVICE_ID  -- invalid instance handle.               */
/*      MCIERR_INVALID_BUFFER       -- Buffer size was too small.           */
/*      MCIERR_UNSUPPORTED_FUNCTION -- Invalid command message.             */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*      MCIERR_INVALID_DEVICE_NAME  -- Illegal drive name.                  */
/*      MCIERR_INVALID_CUEPOINT       --  unable to locate event.           */
/*      MCIERR_CUEPOINT_LIMIT_REACHED --  no more room to add events.       */
/*      MCIERR_DUPLICATE_CUEPOINT     --  duplicate cuepoint.               */
/*      MCIERR_INVALID_ITEM_FLAG  -- Invalid item specified.                */
/*      MCIERR_INVALID_SPEED_FORMAT_FLAG -- Unknown speed type specified.   */
/*      MCIERR_INVALID_TIME_FORMAT_FLAG  -- Unknown time type specified.    */
/*      MCIERR_INVALID_CONNECTOR_INDEX -- Invalid connector specified.      */
/*      MCIERR_INVALID_CONNECTOR_TYPE  -- Invalid connector specified.      */
/*      MCIERR_UNSUPPORTED_CONN_TYPE   -- Unsupported connector specified.  */
/*      MCIERR_INVALID_CALLBACK_HANDLE -- invalid call back handle.         */
/*      MCIERR_INVALID_AUDIO_FLAG  -- Unknown audio flag specified.         */
/*      MCIERR_CANNOT_LOAD_DRIVER  -- Unable to load VSD.                   */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG process_msg(PINST pInst, USHORT usMessage, ULONG *pulParam1,
                  PVOID pParam2, USHORT usUserParm)
{
   ULONG rc;

   if (usMessage != MCI_OPEN)
      DosRequestMutexSem(pInst->hInstSem, WAIT_FOREVER);

   /* bypass restores if opening or closing */
   if (usMessage == MCI_CLOSE || usMessage == MCI_OPEN)
      rc = MCIERR_SUCCESS;
   else
   {
      if (rc = pre_process_msg(pInst, &usMessage, *pulParam1))
      {
         DosReleaseMutexSem(pInst->hInstSem);
         return(rc);
      }
   }  /* of else not close */

   switch(usMessage)
   {
      case 0 :                /* save or restore when already in that state */
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_CLOSE :
         rc = ProcClose(pInst, pulParam1, pParam2, usUserParm);
         break;
      case MCI_CONNECTOR :
         rc = ProcConnector(pInst, *pulParam1, (MCI_CONNECTOR_PARMS *)pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_CUE :
         rc = ProcCue(pInst, *pulParam1);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_ESCAPE :
         DosReleaseMutexSem(pInst->hInstSem);  /* can't provide protection */
         rc = ProcGeneral(pInst, usMessage, pulParam1, pParam2, usUserParm);
         break;
      case MCI_GETDEVCAPS :
         DosReleaseMutexSem(pInst->hInstSem);  /* doesn't need protection */
         rc = ProcCaps(pInst, *pulParam1, (MCI_GETDEVCAPS_PARMS *) pParam2);
         break;
      case MCI_GETTOC :
         DosReleaseMutexSem(pInst->hInstSem);  /* doesn't need protection */
         rc = ProcGeneral(pInst, usMessage, pulParam1, pParam2, usUserParm);
         break;
      case MCI_INFO :
         DosReleaseMutexSem(pInst->hInstSem);  /* doesn't need protection */
         rc = ProcInfo(pInst, *pulParam1, (MCI_INFO_PARMS *)pParam2);
         break;
      case MCI_MASTERAUDIO :
         rc = ProcMAudio(pInst, *pulParam1, (MCI_MASTERAUDIO_PARMS *)pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_OPEN :
         rc = ProcOpen(pulParam1, (MMDRV_OPEN_PARMS *)pParam2, usUserParm);
         break;
      case MCI_PAUSE :
         rc = ProcPause(pInst, *pulParam1);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_PLAY :
         rc = ProcPlay(pInst, pulParam1, (MCI_PLAY_PARMS *)pParam2,
                           usUserParm);
         break;
      case MCIDRV_RESTORE :
         ProcRestore(pInst);      //from MDM, return success
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_RESUME :
         rc = ProcResume(pInst, *pulParam1);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCIDRV_SAVE :
         ProcSave(pInst);         //from MDM, return success
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_SEEK :
         rc = ProcSeek(pInst, *pulParam1, (MCI_SEEK_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_SET :
         rc = ProcSet(pInst, pulParam1, (MCI_SET_PARMS *) pParam2);
         break;
      case MCI_SET_CUEPOINT :
         rc = ProcCuePoint(pInst, *pulParam1, (MCI_CUEPOINT_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_SET_POSITION_ADVISE :
         rc = ProcPosAdvise(pInst, *pulParam1, (MCI_POSITION_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_SET_SYNC_OFFSET :
         rc = ProcSetSync(pInst, *pulParam1, (MCI_SYNC_OFFSET_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_STATUS :
         rc = ProcStatus(pInst, *pulParam1, (MCI_STATUS_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCI_STOP :
         rc = ProcStop(pInst, *pulParam1);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      case MCIDRV_SYNC :
         rc = ProcSync(pInst, *pulParam1, (MCIDRV_SYNC_PARMS *) pParam2);
         DosReleaseMutexSem(pInst->hInstSem);
         break;
      default :
         DosReleaseMutexSem(pInst->hInstSem);  /* can't provide protection */
         rc = ProcGeneral(pInst, usMessage, pulParam1, pParam2, usUserParm);
   }  /* of switch */

   return(rc);

}  /* of process_msg() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  verify_entry                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Verify Entry.                                         */
/*                                                                          */
/* FUNCTION:  Verifies the input parameters from the mciDriverEntry         */
/*            point of entry.  The handle or instance pointer is verified   */
/*            to be a valid open instance.  Also the PARMS record pointer   */
/*            is validated to be in a writable data segment.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst     -- instance pointer.                               */
/*      USHORT usMessage -- requested action to be performed.               */
/*      ULONG  ulParam1  -- flag for this message.                          */
/*      PVOID  pParam2   -- pointer to structure (message dependent).       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_INI_FILE   -- open device type is not CD Audio.              */
/*      MCIERR_INVALID_DEVICE_ID -- invalid instance handle.                */
/*      MCIERR_INVALID_BUFFER    -- invalid pointer in o/p buffer.          */
/*      MCIERR_MISSING_PARAMETER -- invalid PARMS pointer, or i/p buffer.   */
/*      MCIERR_OUT_OF_MEMORY     -- out of memory.                          */
/*      MCIERR_UNSUPPORTED_FUNCTION -- Invalid command message.             */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG verify_entry(PINST pInst, USHORT usMessage,
                   ULONG ulParam1, PVOID *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;
   USHORT usFlag;

   if (usMessage == MCI_OPEN)      /* make sure correct device type is used */
   {
      if (((MMDRV_OPEN_PARMS *)*pParam2)->usDeviceType != MCI_DEVTYPE_CD_AUDIO)
         rc = MCIERR_INI_FILE;
   }
   else          /* verify that component handle exists and is valid */
   {
      if (!usMessage)
         rc = MCIERR_UNSUPPORTED_FUNCTION;

      if (ValPointer(pInst, VALLEN))                // is it a real pointer?
         rc = MCIERR_INVALID_DEVICE_ID;
      else
         if (strncmp(pInst->valid, VALIDID, VALLEN))   // does it point to
            rc = MCIERR_INVALID_DEVICE_ID;             // an instance?

      if (rc && (usMessage == MCI_DEVICESETTINGS))     // DevSets don't need an
         rc = MCIERR_UNSUPPORTED_FUNCTION;             // instance ptr

   }  /* of if not open */

   /* test *pParam2 is a valid pointer */
   if (!rc)
   {
      switch (usMessage)       /* does message require pParam2 ? */
      {
         case MCI_DEVICESETTINGS :
         case MCI_ESCAPE :       case MCI_GETDEVCAPS :
         case MCI_GETTOC :       case MCI_INFO :         case MCI_LOAD :
         case MCI_MASTERAUDIO :  case MCI_OPEN :         case MCI_RECORD :
         case MCI_SAVE:          case MCI_SET_CUEPOINT :
         case MCI_SET_POSITION_ADVISE :          case MCI_SET_SYNC_OFFSET :
         case MCI_STATUS :       case MCI_STEP :         case MCI_SYSINFO :
            usFlag = TRUE;
            break;
         case MCI_CONNECTOR :
            if (ulParam1 & (MCI_QUERY_CONNECTOR_STATUS |
                            MCI_CONNECTOR_TYPE | MCI_CONNECTOR_INDEX))
               usFlag = TRUE;
            else
               usFlag = FALSE;
            break;
         case MCI_PLAY :
            if (ulParam1 & MCI_FROM || ulParam1 & MCI_TO)
               usFlag = TRUE;
            else
               usFlag = FALSE;
            break;
         case MCI_SEEK :
            if (ulParam1 & MCI_TO)
               usFlag = TRUE;
            else
               usFlag = FALSE;
            break;
         case MCI_SET :
            if (ulParam1 & MCI_SET_AUDIO || ulParam1 & MCI_SET_SPEED_FORMAT ||
                ulParam1 & MCI_SET_TIME_FORMAT || ulParam1 & MCI_SET_VOLUME ||
                ulParam1 & MCI_OVER)
               usFlag = TRUE;
            else
               usFlag = FALSE;
            break;
         default :                       /* other commands are on their own */
            usFlag = FALSE;
      }  /* of switch */

      /* Verify that pParam2 is a valid pointer if necessary */
      rc = ValPointer(*pParam2, sizeof(ULONG));
      if (rc && (!(ulParam1 & MCI_NOTIFY) && !usFlag))
      {
         rc = MCIERR_SUCCESS;   //pParam2 not required
         *pParam2 = NULL;
      }
   }  /* of if no error */

   return(rc);

}  /* of verify_entry() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  QMAudio                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  Query Master Audio                                    */
/*                                                                          */
/* FUNCTION:  Query current volume and headphone/speaker settings from      */
/*            Master Audio.                                                 */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:   None                                                       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

VOID QMAudio(PINST pInst)
{
   ULONG rc;
   USHORT ulChanType = 0L;
   MCI_MASTERAUDIO_PARMS recMasterAudio;

   /* get current master volume */
   rc = mciSendCommand(pInst->usDeviceID, MCI_MASTERAUDIO,
                      MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_MASTERVOL,
                      &recMasterAudio, 0);
   if (ULONG_LOWD(rc))
   {
      if (pInst->ulMasterVolume == (ULONG) -1L)
         pInst->ulMasterVolume = 75L;     //if first time error set to default
   }
   else
      pInst->ulMasterVolume = recMasterAudio.ulReturn;

   /* get current speaker status */
   rc = mciSendCommand(pInst->usDeviceID, MCI_MASTERAUDIO,
                       MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_SPEAKERS,
                       &recMasterAudio, 0);
   if (ULONG_LOWD(rc))
      ulChanType = CDMC_SPEAKER;          //if error set to default
   else
      if (recMasterAudio.ulReturn)
         ulChanType = CDMC_SPEAKER;       //SPEAKERS are turned on

   /* get current headphone status */
   rc = mciSendCommand(pInst->usDeviceID, MCI_MASTERAUDIO,
                       MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_HEADPHONES,
                       &recMasterAudio, 0);
   if (ULONG_LOWD(rc))
      ulChanType |= CDMC_HEADPHONE;       //if error set to default
   else
      if (recMasterAudio.ulReturn)
         ulChanType |= CDMC_HEADPHONE;    //HEADPHONES are turned on

   pInst->ulMode = (pInst->ulMode & CHAN_TYPE_SET) | ulChanType;

}  /* of QMAudio() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  Register                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Register.                                             */
/*                                                                          */
/* FUNCTION:  Register the CD-ROM Drive for the MCI Driver.                 */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUT_OF_MEMORY  -- couldn't allocate memory for semaphore.    */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG Register(PINST pInst)
{
   ULONG rc;
   MCI_CD_REGDRIVE_PARMS recDrive;

   /* register drive */
   recDrive.ulCDMCDID = (ULONG) pInst;
   recDrive.pCDMCDReturn = VSDReturn;
   rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_REGISTER_DRIVE, 0L,
                          &recDrive, 0);
   if (!rc)
   {
      pInst->usStatus = REGDRIVE;
      pInst->usCaps = recDrive.usCaps;
      pInst->ulPrerollType  = recDrive.ulPrerollType;
      pInst->ulPrerollTime  = recDrive.ulPrerollTime;
      pInst->ulMinStartTime = recDrive.ulMinStartTime;

      /* STREAM_CAP field in ulMode was cleared in ProcOpen() */
      if (recDrive.usCaps & CDVSD_CAP_HAS_DAC)
      {
         pInst->ulMode |= CDMC_CAN_DAC;
         pInst->ulMode |= CDMC_INTDAC;
      }

      if (recDrive.usCaps & CDVSD_CAP_CAN_STREAM)
         pInst->ulMode |= CDMC_CAN_STREAM;
         /* just set potential to stream, don't go into stream mode until */
         /* the MCI_CONNECTOR command message is sent.                    */

      if (rc)         /* if error, clear stream mode */
         pInst->ulMode &= STREAM_MODE_SET;

   }  /* of if no error registering drive */

   return(rc);

}  /* of Register() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ReRegister                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Re-Register.                                          */
/*                                                                          */
/* FUNCTION:  Register the discs and tracks for the MCI Driver.             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ReRegister(PINST pInst)
{
   ULONG rc = MCIERR_SUCCESS;                 // assume the best
   ULONG ulSetParam1;
   ULONG ulInfoSize;
   USHORT usTemp, usChange = TRUE;
   MCI_SET_PARMS recSet;
   MCI_CD_ID DiscID;

   memcpy(&DiscID, &pInst->recDisc.DiscID, IDSIZE);
   /* Get new disk information */
   rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_REGISTER_DISC, 0L,
                          &pInst->recDisc, 0);

   if (rc == MCIERR_MEDIA_CHANGED)    // Get it again to make sure
      pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_REGISTER_DISC, 0L,
                        &pInst->recDisc, 0);

   /* if no error and previous disc check to see if it's the same */
   if (!rc && DiscID.Mode != (BYTE) -1)
      if (memcmp(&pInst->recDisc.DiscID, &DiscID, IDSIZE))
         rc = MCIERR_MEDIA_CHANGED;
      else
         usChange = FALSE;     //same disc perhaps

   if (rc && rc != MCIERR_MEDIA_CHANGED)
      pInst->usStatus = REGDRIVE;          //Reg Disc, failed
   else
   {  /* Disc was registered */

      /* set initial volume */
      if (pInst->ulMasterVolume == (ULONG) -1L)
      {
         /* if mastervolume is not set then set it */
         /* get current master volume */
         QMAudio(pInst);

         /* set local volume */
         usTemp = pInst->usStatus;
         pInst->usStatus = STOPPED;
         recSet.ulAudio = MCI_SET_AUDIO_LEFT;
         recSet.ulLevel = (ULONG) VOL_LEFT(pInst->ulLevel);
         recSet.ulOver  = 0L;
         ulSetParam1 = MCI_SET_AUDIO | MCI_SET_VOLUME | MCI_WAIT;
         ProcSet(pInst, &ulSetParam1, &recSet);
         pInst->usStatus = usTemp;

      }  /* of setting the device with initial volume setting */

      ulInfoSize = (pInst->recDisc.HighestTrackNum -
                    pInst->recDisc.LowestTrackNum + 1) *
                    sizeof(MCI_CD_REGTRACK_REC);

      /* make sure we have enough memory to hold new disc */
      if (ulInfoSize > pInst->ulTrackInfoSize)
      {
         /* free old pointer if one existed */
         if (pInst->ulTrackInfoSize)
            HhpFreeMem(CDMC_hHeap, pInst->pTrackInfo);
         pInst->pTrackInfo = HhpAllocMem(CDMC_hHeap, (size_t)ulInfoSize);

         if (pInst->pTrackInfo == NULL)
         {
            pInst->ulTrackInfoSize = 0;
            pInst->usStatus = REGDISC;          //No memory to reg tracks
         }
         else
            pInst->ulTrackInfoSize = ulInfoSize;
      }  /* of if need more memory */

      /* Get ready to get track info */
      pInst->recTrack.ulBufSize = pInst->ulTrackInfoSize;
      pInst->recTrack.TrackRecArr = (MCI_CD_REGTRACK_REC *)pInst->pTrackInfo;

      rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_REGISTER_TRACKS, 0L,
                             &pInst->recTrack, 0);

      if (rc)
         pInst->usStatus = REGDRIVE;     //Reg Tracks failed
      else
      {
         /* Set Track Info into instance & Seek to start of first audio track */
         pInst->usStatus = REGTRACK;
         rc = SetTrackInst(pInst, usChange);

         /* report change only if there was a previous disc */
         if (!rc && usChange && DiscID.Mode != (BYTE) -1)
            rc = MCIERR_MEDIA_CHANGED;

      }   /* of else no error registering tracks */
   }   /* of else no error registering disc */

   if (rc == MCIERR_MEDIA_CHANGED || rc == MCIERR_INVALID_MEDIA_TYPE)
      DisableEvents(pInst);
   if (rc == MCIERR_MEDIA_CHANGED)
      rc = MCIERR_SUCCESS;

   return(rc);

}  /* of ReRegister() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  VSDReturn                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  VSD Return                                            */
/*                                                                          */
/* FUNCTION:  Process return information from VSDs.  This function is       */
/*            called by the VSD to inform the MCD of changes in the         */
/*            component mode or state.  This is needed when the VSD         */
/*            creates a thread to process a command (ie MCI_PLAY with       */
/*            MCI_NOTIFY) and needs to inform the MCD of the results        */
/*            (ie PLAY terminated because of an error or PLAY completed     */
/*            and the MCD must now be in a STOPPED state).  This function   */
/*            is called also by the VSD when a non-standard message or      */
/*            MCI_ESCAPE alters the mode of the device (ie it stops is).    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG pInst     -- MCD Instance ID / pointer to instance.           */
/*      ULONG ulMsgParm -- Message Parm, same format as in mdmDriverNotify()*/
/*      ULONG ulMode    -- Mode the component is in.                        */
/*                                                                          */
/* EXIT CODES: None                                                         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

VOID VSDReturn(ULONG pInst, ULONG ulMsgParm, ULONG ulMode)
{
   USHORT usMessage, usErrorCode, usChange = FALSE;

   /* extract error and message codes */
   usMessage   = LOUSHORT(ulMsgParm);
   usErrorCode = HIUSHORT(ulMsgParm);

   switch(usMessage)
   {
      case MCI_PLAY :
         if (usErrorCode != MCI_NOTIFY_SUPERSEDED &&
             usErrorCode != MCI_NOTIFY_ABORTED)
         {
            usChange = TRUE;                   //PLAY completed or had error
            ulMode = MCI_MODE_STOP;
         }
         break;
      case MCI_CONNECTOR :    case MCI_GETDEVCAPS :   case MCI_GETTOC :
      case MCI_INFO :         case MCI_MASTERAUDIO :  case MCI_OPEN :
      case MCI_RECORD :       case MCI_SAVE:          case MCI_SEEK :
      case MCI_SET_CUEPOINT :
      case MCI_SET_POSITION_ADVISE :          case MCI_SET_SYNC_OFFSET :
      case MCI_STATUS :       case MCI_STEP :         case MCI_SYSINFO :
         break;                          // messages processed by MCD
      case MCI_LOAD :         case MCI_SET :
         usChange = TRUE;                // possible change, listen to VSD
         break;
      default :                          // unknown message, listen to VSD
         usChange = TRUE;

   }  /* of switch */

   if (usChange)
   {
      switch (ulMode)
      {
         case MCI_MODE_NOT_READY :
            ((PINST) pInst)->usStatus = NODISC;
            break;
         case MCI_MODE_PAUSE :
            ((PINST) pInst)->usStatus = PAUSED;
            break;
         case MCI_MODE_PLAY :
            ((PINST) pInst)->usStatus = PLAYING;
            break;
         case MCI_MODE_STOP :
            ((PINST) pInst)->usStatus = STOPPED;
            break;

      }  /* of switch */
   }  /* of if change was needed */
}  /* of VSDReturn() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  SetTrackInst                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Set Track Information of Instance.                    */
/*                                                                          */
/* FUNCTION:  Sets the Track related information in the instance structure. */
/*            It also seeks to the start of the first audio track.          */
/*            If any error occurs, it will update the usStatus field        */
/*            as an extension of the registration portion.                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst    -- pointer to instance.                             */
/*      USHORT usChange -- Change Disc flag.                                */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_MEDIA_CHANGED      -- different disc was inserted.           */
/*                                                                          */
/* NOTES:  MCIERR_MEDIA_CHANGED is returned when usChange is FALSE and      */
/*         a difference was found, otherwise it expects that the            */
/*         calling function is aware of the change and will return          */
/*         MCIERR_MEDIA_CHANGED if needed.                                  */
/*                                                                          */
/****************************************************************************/

ULONG SetTrackInst(PINST pInst, USHORT usChange)
{
   ULONG rc = MCIERR_SUCCESS;          // assume the best
   int i, cnt;
   MCI_SEEK_PARMS recSeek;
   MCI_STATUS_PARMS recStatus;
   ULONG ulParam1, ulDirt = FALSE, ulFlags;

   cnt =  (BYTE) (pInst->recDisc.HighestTrackNum -
                  pInst->recDisc.LowestTrackNum + 1);

   /* find the start of the audio section */
   for (i = 0; i < cnt; i++)
   {
      if (!((pInst->recTrack.TrackRecArr + i)->TrackControl & IS_DATA_TRK))
      {
         pInst->ulStart_disk = (pInst->recTrack.TrackRecArr + i)->ulStartAddr;
         break;
      }
   }   /* of for loop, finding first audio track */

   /* Did we get it ? */
   if (i == cnt)
   {
      pInst->ulStart_disk = 0L;
      pInst->ulEnd_disk = 0L;
      pInst->ulCur_pos = 0L;
      rc = MCIERR_INVALID_MEDIA_TYPE;
   }
   else
   {
      /* find last audio track */
      for (i = cnt-1; ; i--)
      {
         if (!((pInst->recTrack.TrackRecArr + i)->TrackControl & IS_DATA_TRK))
         {
            pInst->ulEnd_disk = (pInst->recTrack.TrackRecArr + i)->ulEndAddr;
            break;
         }
      }   /* of for loop, finding last audio track */

      if (usChange)
         /* now seek to start of playable disc */
         recSeek.ulTo = pInst->ulStart_disk;
      else   // we think its the same disc, reseek to current position
      {
         recStatus.ulItem = MCI_STATUS_POSITION;
         ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                                &ulFlags, &recStatus, 0);
         if (ULONG_LOWD(rc))
            rc = vsdResponse(pInst, rc);
         else
            recSeek.ulTo = recStatus.ulReturn;

         if (rc)  //error getting current position, try last known position
         {
            rc = MCIERR_SUCCESS;
            recSeek.ulTo = pInst->ulCur_pos;
            ulDirt = TRUE;
         }

         if (ValAddress(pInst, NULL, &recSeek.ulTo, FALSE))
         {
            recSeek.ulTo = pInst->ulStart_disk;
            ulDirt = TRUE;
         }
      }  /* of else assuming same disc */

      /* check hardware start limitations */
      if (recSeek.ulTo < pInst->ulMinStartTime)
         recSeek.ulTo = pInst->ulMinStartTime;

      ulParam1 = MCI_TO | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SEEK, &ulParam1, &recSeek, 0);

      /* just incase they changed the disc again, check it */
      if (rc)
         rc = vsdResponse(pInst, rc);
      else
      {
         pInst->usStatus = STOPPED;
         pInst->ulCur_pos = recSeek.ulTo;
         if (ulDirt)
            rc = MCIERR_MEDIA_CHANGED;

      }  /* of else no error seeking */
   }  /* of else disc contains playable tracks */

   return(rc);

}  /* of SetTrackInst() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ValPointer                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Validate Pointer                                      */
/*                                                                          */
/* FUNCTION:  Validate Pointers as writable addresses.                      */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PVOID  ptr     -- pointer, address to be checked.                   */
/*      ULONG  usLen   -- length to be checked.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_MISSING_PARAMETER -- invalid pointer.                        */
/*                                                                          */
/* NOTES:   If interrupted, pretend that it was successful.                 */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG ValPointer(PVOID ptr, ULONG ulLen)
{
   ULONG rc = MCIERR_SUCCESS;          // assume the best
   ULONG ulFlags;

   if (ptr == 0L)
      rc = MCIERR_MISSING_PARAMETER;
   else
   {
      rc = DosQueryMem(ptr, &ulLen, &ulFlags);

      if (rc != ERROR_INTERRUPT)            // ignore interruptions
         if (rc)
            rc = MCIERR_MISSING_PARAMETER;
         else
            if (!(ulFlags & PAG_WRITE))
               rc = MCIERR_MISSING_PARAMETER;
   }

   return(rc);

}  /* of ValPointer() */



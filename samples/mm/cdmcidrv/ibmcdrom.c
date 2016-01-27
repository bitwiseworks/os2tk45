/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  IBMCDROM.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  IBM CD-ROMs                                           */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1990 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the device dependent code for the CD Audio */
/*            Vendor Specific Driver (VSD).  This file is called by the     */
/*            general CD Audio MCI Driver, CDAUDIO.DLL or CDAUDIOT.DLL      */
/*                                                                          */
/* NOTES:  The hardware independent code is found in file CDAUDIO.C.  As    */
/*         more and more CD ROM drives are supported, hardware dependent    */
/*         files can be made into DLLs and register be registered in the    */
/*         MMPM2.INI file upon installation to be recognized.               */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       vsdDriverEntry     - entry point to the VSD                        */
/*                                                                          */
/*                                                                          */
/* OTHER FUNCTIONS:                                                         */
/*       process_msg        - Process the requested command message.        */
/*       CDAudErrRecov      - Error recovery routine.                       */
/*       CDAudClose         - Close an instance.                            */
/*       CDAudInfo          - Returns information about the component.      */
/*       CDAudOpen          - Open an instance.                             */
/*       CDAudRegDisc       - Register a disc for the multimedia component. */
/*       CDAudRegDrive      - Register a drive for the multimedia component.*/
/*       CDAudSet           - Set various attributes of the device.         */
/*       CDAudSetVerify     - Tests flags for the set command.              */
/*       CDAudStatus        - Returns the requested attribute.              */
/*       CDAudStatCVol      - Returns mapped volume levels for volume given.*/
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#define INCL_DOSERRORS
#define INCL_DOSDEVICES
#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSSEMAPHORES
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#define INCL_MCIOS2
#include <os2me.h>
#include <cdaudos2.h>
#include "ibmcdrom.h"
#include "hhpheap.h"

extern PVOID          CDMC_hHeap;

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  vsdDriverEntry                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  Hardware specific code entry point.                   */
/*                                                                          */
/* FUNCTION:  Receive command message from the general MCI Driver for CD    */
/*            Audio (CDAUDIOT.DLL).                                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PVOID  lpInstance -- Pointer to device handle.                      */
/*      USHORT usMessage  -- Command message.                               */
/*      ULONG  *pulParam1 -- Pointer to flag for this message.              */
/*      PVOID  pParam2    -- Pointer to data record structure.              */
/*      USHORT usUserParm -- User Parameter.                                */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_INI_FILE   -- corrupted INI file, drive is not CD-ROM drive. */
/*      MCIERR_DEVICE_NOT_READY      -- device was closed from an error.    */
/*      MCIERR_MEDIA_CHANGED         -- device was reopened, waiting for    */
/*                                      MCIDRV_REGISTER message.            */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- missing or multiple flags.          */
/*      MCIERR_UNRECOGNIZED_COMMAND  -- unknown command.                    */
/*      MCIERR_UNSUPPORTED_FUNCTION  -- unsupported command.                */
/*      MCIERR_UNSUPPORTED_FLAG      -- unsupported flag.                   */
/*      MCIERR_INVALID_FLAG          -- flag not supported by this VSD.     */
/*      MCIERR_OUT_OF_MEMORY         -- couldn't allocate instance.         */
/*      MCIERR_INVALID_ITEM_FLAG     -- Unknown item specified.             */
/*      MCIERR_OUTOFRANGE            -- Invalid value.                      */
/*      MCIERR_INVALID_MEDIA_TYPE    -- No audio tracks were found.         */
/*      MCIERR_CUEPOINT_LIMIT_REACHED -- no more room to add events.        */
/*      MCIERR_INVALID_CUEPOINT       -- unable to locate event.            */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG APIENTRY vsdDriverEntry(PVOID lpInstance, USHORT usMessage,
                              ULONG *pulParam1, PVOID pParam2,
                              USHORT usUserParm)
{
   ULONG  rc, ulP1Temp = MCI_WAIT;
   USHORT try = 1;

   if (pulParam1 == 0L)
      pulParam1 = &ulP1Temp;

   /* check to see if the drive is open, unless it is an Open message */
   if (usMessage == MCI_OPEN)
      rc = CDAudOpen(*pulParam1, (MMDRV_OPEN_PARMS *)pParam2);
   else
   {
      /* if the device is closed try reopening it unless you are closing */
      if (((PINST) lpInstance)->hDrive == 0 && usMessage != MCI_CLOSE)
      {
         rc = CD01_Open((PINST) lpInstance);
         /* Clear commands not needing an open hardware device */
         if (rc == MCIERR_DEVICE_NOT_READY)
            if ((usMessage == MCI_DEVICESETTINGS) ||
                (usMessage == MCI_GETDEVCAPS) ||
                (usMessage == MCI_INFO) ||
                (usMessage == MCIDRV_REGISTER_DRIVE) ||
                (usMessage == MCI_SET_CUEPOINT) ||
                (usMessage == MCI_SET_POSITION_ADVISE) ||
                (usMessage == MCIDRV_CD_STATUS_CVOL) ||
                (usMessage == MCIDRV_SYNC &&
                   !(*pulParam1 & MCIDRV_SYNC_REC_PULSE)))
               rc = MCIERR_SUCCESS;

      }  /* of if drive needs to be open */
      else      /* drive was opened */
         rc = MCIERR_SUCCESS;

      if (!rc)
         do
         {
            /* process message */
            rc = process_msg((PINST) lpInstance, usMessage,
                             pulParam1, pParam2, usUserParm);

            if (rc == MCIERR_DEVICE_NOT_READY ||       /* ERROR RECOVERY */
                rc == MCIERR_MEDIA_CHANGED)
            {
               if (((PINST)lpInstance)->Drive == '0')     /* drive is closed */
               {                                   /* don't reissue commands */
                  rc = MCIERR_SUCCESS;
                  break;
               }
               else
                  if (try == 2)
                     break;                         /* quit after 2 tries. */
                  else
                  {
                     rc = CDAudErrRecov((PINST) lpInstance);
                     if (rc)                   /* error is still there, exit */
                        break;
                     else
                        try++;
                  }  /* of else only tried the command once (try == 1) */

            }  /* of if the drive was not ready */
            else
               break;                          /* clear flag to exit */

         } while (try);  /* end of do loop and if no open error */

   } /* of else command was not MCI_OPEN */

   return(rc);

}  /* of vsdDriverEntry() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  process_msg                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process message command.                              */
/*                                                                          */
/* FUNCTION:  Process the command message received by vsdDriverEntry().     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- Pointer to instance structure.                 */
/*      USHORT usMessage  -- Command message.                               */
/*      ULONG  *pulParam1 -- Pointer to flag for this message.              */
/*      PVOID  pParam2    -- Pointer to data record structure.              */
/*      USHORT usUserParm -- User Parameter.                                */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_MEDIA_CHANGED         -- device was reopened, waiting for    */
/*                                      MCIDRV_REGISTER message.            */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- missing or multiple flags.          */
/*      MCIERR_UNRECOGNIZED_COMMAND  -- unknown command.                    */
/*      MCIERR_UNSUPPORTED_FUNCTION  -- unsupported command.                */
/*      MCIERR_UNSUPPORTED_FLAG      -- unsupported flag.                   */
/*      MCIERR_INVALID_BUFFER        -- Buffer size was too small.          */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*      MCIERR_INVALID_MEDIA_TYPE    -- No audio tracks were found.         */
/*      ERROR_TOO_MANY_EVENTS --  no more room to add events.               */
/*      ERROR_INVALID_EVENT   --  unable to locate event.                   */
/*      ERROR_INVALID_MMTIME  --  duplicate cuepoint.                       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG process_msg(PINST pInst, USHORT usMessage,
                         ULONG *pulParam1, PVOID pParam2, USHORT usUserParm)
{
   ULONG rc;

   DosRequestMutexSem(pInst->hInstSem, WAIT_FOREVER);
   if (usMessage != MCI_PLAY)
      DosReleaseMutexSem(pInst->hInstSem);       // No protection needed

   /* process message */
   switch(usMessage)
   {
      case MCI_CLOSE :
         rc = CDAudClose(pInst, *pulParam1);
         break;
      case MCI_CUE :                           /* Pre-roll */
         rc = CD01_Cue(pInst);
         break;
      case MCI_GETDEVCAPS :                    /* Get Device Capabilities */
         rc = CD01_GetCaps(*pulParam1, (MCI_GETDEVCAPS_PARMS *)pParam2);
         break;
      case MCI_GETTOC :                        /* Get Table of Contents */
         if (*pulParam1 & WAIT_NOTIFY_MASK)
            rc = MCIERR_INVALID_FLAG;
         else
            rc = CD01_GetTOC(pInst, (MCI_TOC_PARMS *)pParam2);
         break;
      case MCI_INFO :
         rc = CDAudInfo(pInst, *pulParam1, (MCI_INFO_PARMS *)pParam2);
         break;
      /* case MCI_OPEN :          open was already done in vsdDriverEntry() */

      case MCI_PAUSE :
         rc = CD01_Stop(pInst, TIMER_PLAY_SUSPEND);
         break;
      case MCI_PLAY :
         rc = CD01_Play(pInst, pulParam1,
                        ((MCI_PLAY_PARMS *)pParam2)->ulFrom,
                        ((MCI_PLAY_PARMS *)pParam2)->ulTo, usUserParm,
                        ((MCI_PLAY_PARMS *)pParam2)->hwndCallback);
         break;
      case MCIDRV_CD_READ_LONG :                   /* Read Long */
         rc = MCIERR_UNSUPPORTED_FUNCTION;
         break;
      case MCIDRV_REGISTER_DISC :                  /* Register Disc */
         rc = CDAudRegDisc(pInst, REG_BOTH, (MCI_CD_REGDISC_PARMS *)pParam2);
         break;
      case MCIDRV_REGISTER_DRIVE :                 /* Register Drive */
         rc = CDAudRegDrive(pInst, (MCI_CD_REGDRIVE_PARMS *)pParam2);
         break;
      case MCIDRV_REGISTER_TRACKS :                /* Register Tracks */
         rc = CD01_RegTracks(pInst, (MCI_CD_REGTRACKS_PARMS *)pParam2);
         break;
      case MCIDRV_RESTORE :
         rc = CD01_Restore(pInst, (MCIDRV_CD_SAVE_PARMS *)pParam2);
         break;
      case MCI_RESUME :                            /* Unpause */
         rc = CD01_Resume(pInst);
         break;
      case MCIDRV_SAVE :
         rc = CD01_Save(pInst, (MCIDRV_CD_SAVE_PARMS *)pParam2);
         break;
      case MCI_SEEK :
         rc = CD01_Seek(pInst, ((MCI_SEEK_PARMS *)pParam2)->ulTo);
         break;
      case MCI_SET :
         rc = CDAudSet(pInst, pulParam1, (MCI_SET_PARMS *)pParam2);
         break;
      case MCI_SET_CUEPOINT :
         rc = CD01_CuePoint(pInst, *pulParam1, (MCI_CUEPOINT_PARMS *)pParam2);
         break;
      case MCI_SET_POSITION_ADVISE :
         rc = CD01_PosAdvise(pInst, *pulParam1, (MCI_POSITION_PARMS *)pParam2);
         break;
      case MCIDRV_CD_SET_VERIFY :
         rc = CDAudSetVerify(*pulParam1);
         break;
      case MCI_STATUS :
         rc = CDAudStatus(pInst, *pulParam1, (MCI_STATUS_PARMS *)pParam2);
         break;
      case MCIDRV_CD_STATUS_CVOL :
         rc = CDAudStatCVol(&((MCI_STATUS_PARMS *)pParam2)->ulReturn);
         break;
      case MCI_STOP :
         rc = CD01_Stop(pInst, TIMER_EXIT_ABORTED);
         break;
      case MCIDRV_SYNC :
         rc = CD01_Sync(pInst, *pulParam1, (MCIDRV_SYNC_PARMS *)pParam2);
         break;
      default :
         if (usMessage <= MCI_MAX_COMMAND)
            rc = MCIERR_UNSUPPORTED_FUNCTION;
         else
            rc = MCIERR_UNRECOGNIZED_COMMAND;

   }  /* of switch */

   return(rc);

}  /* of process_msg() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudErrRecov                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Information.                                 */
/*                                                                          */
/* FUNCTION:  Returns string information from a device driver.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_INI_FILE  -- corrupted INI file, drive is not CD-ROM drive.  */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_MEDIA_CHANGED         -- device was reopened.                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudErrRecov(PINST pInst)
{
   ULONG rc;

   /* Close the device */
   DosClose(pInst->hDrive);
   pInst->hDrive = 0;                    /* mark the instance structure */

   /* Try to reopen the device */
   rc = CD01_Open(pInst);

   return(rc);

}  /* of CDAudErrRecov() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudClose                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Close                                        */
/*                                                                          */
/* FUNCTION:  Close a device dependent instance.                            */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_INVALID_FLAG -- flag not supported by this VSD.              */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudClose(PINST pInst, ULONG ulParam1)
{
   ULONG rc;

   if (ulParam1 & WAIT_NOTIFY_MASK)
      rc = MCIERR_INVALID_FLAG;
   else
   {
      pInst->Drive = '0';              /* destroy drive to prevent reopening */

      /* stop play & timer if one is going */
      switch(pInst->usPlayFlag)
      {
         case TIMER_PLAY_SUSPEND :                 //suspended, stop only timer
         case TIMER_PLAY_SUSP_2  :
            if (DosWaitThread(&pInst->ulPlayTID, 1L) ==
                                         ERROR_THREAD_NOT_TERMINATED)
            {
               pInst->usPlayFlag = TIMER_EXIT_ABORTED;
               DosPostEventSem(pInst->hTimeLoopSem);  //continue timer loop
            }
            else
               pInst->usPlayFlag = TIMER_AVAIL;
            break;
         case TIMER_PLAYING :                      //playing, stop device
            CD01_Stop(pInst, TIMER_EXIT_ABORTED);
            break;
      }  /* of switch() */

      /* wait for commands to terminate */
      while (pInst->usPlayFlag != TIMER_AVAIL)
         DosSleep(HALF_TIME_MIN);

      DosClose(pInst->hDrive);
      DosCloseMutexSem(pInst->hIOSem);
      DosCloseMutexSem(pInst->hInstSem);
      DosCloseEventSem(pInst->hTimeLoopSem);
      DosCloseEventSem(pInst->hReturnSem);
      HhpFreeMem(CDMC_hHeap, pInst);
      rc = MCIERR_SUCCESS;                  /* assume all will work right */
   }  /* of else flags were okay */

   return(rc);

}  /* of CDAudClose() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudInfo                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Information.                                 */
/*                                                                          */
/* FUNCTION:  Returns string information from a device driver.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*      MCI_INFO_PARMS *pParam2 -- Pointer to data record structure.        */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- Mis-match in flags.                 */
/*      MCIERR_INVALID_BUFFER        -- Buffer too small.                   */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*                                                                          */
/* NOTES:                                                                   */
/*      The CD ID and UPC is processed by the MCD.                          */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudInfo(PINST pInst, ULONG ulParam1, MCI_INFO_PARMS *pParam2)
{
   ULONG rc, ulLen;
   USHORT i;

   ulParam1 &= WAIT_NOTIFY_MASK;

   if (ulParam1 == MCI_INFO_PRODUCT)
   {
      if (pParam2->ulRetSize >= INFO_SIZE)
      {
         ulLen = INFO_SIZE;
         if (pParam2->ulRetSize > INFO_SIZE)   // if buffer is big enough
            ulLen++;                           // capture the NULL
         rc = MCIERR_SUCCESS;
      }
      else
      {
         ulLen = pParam2->ulRetSize;
         rc = MCIERR_INVALID_BUFFER;
      }

      /* fill in product information */
      if (ulLen)   //if 0, it may be an invalid address
         memcpy(pParam2->pszReturn, "IBMCD010", ulLen);
      pParam2->ulRetSize = INFO_SIZE;

   }
   else
      rc = MCIERR_INVALID_FLAG;

   if (rc && rc != MCIERR_INVALID_BUFFER)
      pParam2->ulRetSize = 0L;

   return(rc);

}  /* of CDAudInfo() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudOpen                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Open.                                        */
/*                                                                          */
/* FUNCTION:  Open a device dependent instance.                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*      MMDRV_OPEN_PARMS *pParam2 -- Pointer to data record structure.      */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_INI_FILE  -- corrupted INI file, drive is not CD-ROM drive.  */
/*      MCIERR_OUT_OF_MEMORY       -- couldn't allocate memory for instance.*/
/*      MCIERR_INVALID_FLAG -- flag not supported by this VSD.              */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudOpen(ULONG ulParam1, MMDRV_OPEN_PARMS *pParam2)
{
   ULONG rc;
   PINST pInst;
   BYTE i;
   ULONG ulDevParmLen;
   PVOID pVSDParm;
   CHAR *pDrive, *pModel;

   /* validate flags */
   /* this VSD will only support standard flags */
   ulParam1 &= WAIT_NOTIFY_MASK;
   if (ulParam1)
      rc = MCIERR_INVALID_FLAG;
   else
   {
      /* get memory */
      pInst = HhpAllocMem(CDMC_hHeap, sizeof(struct instance_state));
      if (pInst == NULL)
         rc = MCIERR_OUT_OF_MEMORY;
      else
      {
         /* create semaphore so only one IOCLT call will be made at a time */
         rc = DosCreateMutexSem((PSZ) NULL, &pInst->hIOSem, 0L, FALSE);
         if (!rc)
            rc = DosCreateEventSem((PSZ) NULL, &pInst->hTimeLoopSem, 0L, FALSE);
         if (!rc)
            rc = DosCreateMutexSem((PSZ) NULL, &pInst->hInstSem, 0L, FALSE);
         if (!rc)
            rc = DosCreateEventSem((PSZ) NULL, &pInst->hReturnSem, 0L, FALSE);
         if (rc)
         {
            DosCloseMutexSem(pInst->hIOSem);
            DosCloseMutexSem(pInst->hInstSem);
            DosCloseEventSem(pInst->hTimeLoopSem);
            DosCloseEventSem(pInst->hReturnSem);
            rc = MCIERR_OUT_OF_MEMORY;

         }  /* of if error creating semaphores */
      }  /* else no error creating instance structure's memory */

      /* init instance */
      if (!rc)
      {  /* instance entry created */
         /* init instance */
         pInst->hDrive = 0;                     // drive handle
         pInst->ulCurPos = 0L;                  // set current position;
         pInst->usDeviceID = pParam2->usDeviceID;

         /* validate and get drive & hardware type */
         /* copy string so that it doesn't get disturb */
         ulDevParmLen = strlen(pParam2->pDevParm) + 1;
         pVSDParm = HhpAllocMem(CDMC_hHeap, ulDevParmLen);
         strcpy(pVSDParm, pParam2->pDevParm);
         parse_DevParm((CHAR *)pVSDParm, &pDrive, &pModel);
         pInst->Drive = *pDrive;
         strncpy(pInst->szModel, pModel, MODEL_SIZE);
         pInst->szModel[MODEL_SIZE - 1] = '\0';     //make sure NULL terminated

         HhpFreeMem(CDMC_hHeap, pVSDParm);
         QueryTable(pInst);                   // id drive and get capabilities

         pInst->qptPosAdvise.ulEvent = 0L;
         for (i=0; i < CDMCD_CUEPOINT_MAX; i++)          // init arrays
            pInst->arrCuePoint[i].ulEvent = (ULONG) -1L;

         pInst->usPlayFlag = TIMER_AVAIL;
         pInst->ulPlayTID  = 0L;
         pInst->DiscID.ulLeadOut = 0L;         // used as a first time flag
         pInst->StreamMaster = FALSE;

         /* Try opening the device.  Ignore things like disc not present, */
         /* but report errors involving the INI file or that the drive    */
         /* was previously opened in an exclusive mode.                   */
         rc = CD01_Open(pInst);
         if (rc != MCIERR_INI_FILE && rc != MCIERR_DEVICE_LOCKED)
            rc = MCIERR_SUCCESS;        /* pretend that all went well */

      }  /* of if no errors getting memory */

      if (rc)                                  /* if error, free memory */
         HhpFreeMem(CDMC_hHeap, pInst);
      else
         pParam2->pInstance = pInst;           /* pass pointer back */

   }  /* of else no invalid flags */

   return(rc);

}  /* of CDAudOpen() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudRegDisc                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Register Compact Disc.                       */
/*                                                                          */
/* FUNCTION:  Register a CD media so that the disc may be recognized by     */
/*            the calling MCD.                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst     -- Instance structure.                             */
/*      USHORT usFlag    -- Action Flag to copy into pParam2.               */
/*      MCI_CD_REGDISC_PARMS *pParam2 -- Pointer to data record.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CDAudRegDisc(PINST pInst, USHORT usFlag, MCI_CD_REGDISC_PARMS *pParam2)
{
   ULONG rc;
   USHORT i;
   BYTE lowest, highest;
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = STANDRD_PMAX;
   MCI_CD_ID DiscID;

   memcpy(&DiscID, &pInst->DiscID, sizeof(MCI_CD_ID));  //save original

   if (usFlag == REG_BOTH || usFlag == REG_INST)
   {   /* register with the internal instance */
      rc = CD01_GetID(pInst, &pInst->DiscID, &lowest, &highest);
      if (!rc && usFlag == REG_BOTH)
         /* copy information to external source */
         memcpy(&pParam2->DiscID, &pInst->DiscID, sizeof(MCI_CD_ID));
   }   /* of if update instance structure */
   else      /* the flag is REG_PARAM2 */
      rc = CD01_GetID(pInst, &pParam2->DiscID, &lowest, &highest);

   /* complete registration for param2 */
   if (!rc && (usFlag == REG_BOTH || usFlag == REG_PARAM2))
   {
      /* get low and high track numbers */
      pParam2->LowestTrackNum  = lowest;
      pParam2->HighestTrackNum = highest;

      /* get upc code -- device cannot do this so set this to zero */
      for (i=0; i < UPC_SIZE; i++)
         pParam2->UPC[i] = 0;

   }  /* of if need to complete Param2 */

   return(rc);

}  /* of CDAudRegDisc() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudRegDrive                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Register CD-ROM Drive and its capabilities.  */
/*                                                                          */
/* FUNCTION:  Register a CD drive so that the disc may be recognized by     */
/*            the calling MCD.                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      MCI_CD_REGDRIVE_PARMS *pParam2 -- Pointer to data record.           */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudRegDrive(PINST pInst, MCI_CD_REGDRIVE_PARMS *pParam2)
{
   /* get the CD MCD return function information */
   pInst->ulCDMCDID    = pParam2->ulCDMCDID;
   pInst->pCDMCDReturn = pParam2->pCDMCDReturn;

   /* fill in capabilities */
   pParam2->usCaps = CD01_CAPS;       //This example supports the IBM 3510-001

   /* fill in preroll information */
   pParam2->ulPrerollType  = MCI_PREROLL_NONE;
   pParam2->ulPrerollTime  = 0L;
   pParam2->ulMinStartTime = SPEC_START;

   return(MCIERR_SUCCESS);

}  /* of CDAudRegDrive() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudSet                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Set.                                         */
/*                                                                          */
/* FUNCTION:  Set features and attributes of the CD-ROM drive.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- Instance structure.                            */
/*      ULONG  *pulParam1 -- Flag for this message.                         */
/*      MCI_SET_PARMS *pParam2   -- Pointer to data record structure.       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_UNSUPPORTED_FLAG      -- unsupported flag.                   */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:  Mutual Exclusive flags (LOCK/UNLOCK & OPEN/CLOSE DOOR) are       */
/*         tested in the MCD.                                               */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudSet(PINST pInst, ULONG *pulParam1, MCI_SET_PARMS *pParam2)
{
   ULONG rc;
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = STANDRD_PMAX;
   ULONG ulType;

   /* validate flags */
   rc = CDAudSetVerify(*pulParam1);

   if (!rc)
   {
      ulType = *pulParam1 & WAIT_NOTIFY_MASK;

      if (ulType & MCI_SET_DOOR_OPEN)
      {
         /* hardware does not support eject if playing, so stop it. */
         if (pInst->usPlayFlag == TIMER_PLAYING)
            rc = CD01_Stop(pInst, TIMER_EXIT_ABORTED);

         if (!rc)             // no error, proceed, else disc is already out
         {
            /* make sure that door is unlocked so eject can be successful */
            CD01_LockDoor(pInst, MCI_FALSE);
            CallIOCtl(pInst, CDDRIVE_CAT, EJECT__DISK,
                      "CD01", ulParamLen, &ulParamLen,
                       NULL,  ulDataLen,  &ulDataLen);
         }
         rc = MCIERR_SUCCESS;

      }  /* of if EJECTing disc */

      if (ulType & MCI_SET_DOOR_LOCK)
         rc = CD01_LockDoor(pInst, MCI_TRUE);
      else if (ulType & MCI_SET_DOOR_UNLOCK)
         rc = CD01_LockDoor(pInst, MCI_FALSE);

      if (ulType & MCI_SET_VOLUME)
         /* Hardware for example cannot support vectored volume */
         rc = CD01_SetVolume(pInst, pParam2->ulLevel);

   }  /* of if no error */

   return(rc);

}  /* of CDAudSet() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudSetVerify                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Set Verify                                   */
/*                                                                          */
/* FUNCTION:  Verify flags for SET command.  This function is called by     */
/*            the MCD to validate flags prior to calling other components,  */
/*            like the Amp/Mixer or SSM, to process their flags.            */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_UNSUPPORTED_FLAG      -- unsupported flag.                   */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*                                                                          */
/* NOTES:  Mutual Exclusive flags (LOCK/UNLOCK & OPEN/CLOSE DOOR) are       */
/*         tested in the MCD.                                               */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudSetVerify(ULONG ulParam1)
{
   ULONG rc = MCIERR_SUCCESS;

   /* check unsupported flags */
   if (ulParam1 & MCI_SET_DOOR_CLOSED)
      rc = MCIERR_UNSUPPORTED_FLAG;
   else
      /* check unknown flags */
      if (ulParam1 & ~(MCI_SET_DOOR_OPEN   | MCI_SET_DOOR_LOCK |
                       MCI_SET_DOOR_UNLOCK | MCI_SET_VOLUME | MCI_OVER |
                       MCI_NOTIFY | MCI_WAIT))
         rc = MCIERR_INVALID_FLAG;

   return(rc);

}  /* of CDAudSetVerify() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudStatus                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Status.                                      */
/*                                                                          */
/* FUNCTION:  Query and return the status of the features and attributes    */
/*            of the CD-ROM drive.                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*      MCI_STATUS_PARMS *pParam2   -- Pointer to data record structure.    */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- Mis-match in flags.                 */
/*      MCIERR_UNSUPPORTED_FUNCTION  -- Flags not supported.                */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*      MCIERR_INVALID_ITEM_FLAG     -- Unknown item specified.             */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
static ULONG CDAudStatus(PINST pInst,
                         ULONG ulParam1, MCI_STATUS_PARMS *pParam2)
{
   ULONG rc;

   ulParam1 &= WAIT_NOTIFY_MASK;

   /* verify flags */
   if (ulParam1 & MCI_STATUS_ITEM)
      if (ulParam1 != MCI_STATUS_ITEM)
         if (ulParam1 & MCI_STATUS_START && ulParam1 & MCI_TRACK)
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
         else
            rc = MCIERR_INVALID_FLAG;
      else
         rc = MCIERR_SUCCESS;
   else                                      // no ITEM flag
      if (ulParam1 & MCI_TRACK)
         rc = MCIERR_MISSING_FLAG;
      else if (ulParam1 & MCI_STATUS_START)
         rc = MCIERR_MISSING_FLAG;
      else
         rc = MCIERR_INVALID_FLAG;

   if (!rc)                         // ulParam1 == MCI_STATUS_ITEM only
   {
      switch(pParam2->ulItem)
      {
         case MCI_STATUS_POSITION :
            rc = CD01_GetPosition(pInst, &pParam2->ulReturn);
            break;
         case MCI_STATUS_VOLUME :
            rc = CD01_GetVolume(pInst, &pParam2->ulReturn);
            break;
         case MCI_STATUS_MEDIA_PRESENT :
         case MCI_STATUS_MODE :
         case MCI_STATUS_READY :
            rc = CD01_GetDiscInfo(pInst, pParam2->ulItem, pParam2);
            break;
         default :
            rc = MCIERR_INVALID_ITEM_FLAG;

      }  /* of item switch */
   }  /* of if valid flag */

   return(rc);

}  /* of CDAudStatus() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CDAudStatCVol                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Status Component Volume.                     */
/*                                                                          */
/* FUNCTION:  Remaps the component volume to what it would be from the      */
/*            hardware if master audio was at 100%.                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG *pulLevel  -- Pointer to volume level.                        */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CDAudStatCVol(ULONG *pulLevel)
{
   USHORT usLeft, usRight;

   usLeft = LOUSHORT(*pulLevel);
   usRight = HIUSHORT(*pulLevel);

   if (usLeft) usLeft = 100; else usLeft = 0;
   if (usRight) usRight = 100; else usRight = 0;

   /* Adjust for hardware limitations, VOL = MAX(Left, Right) */
   if (usLeft && usRight)
      if (usLeft > usRight)
         usRight = usLeft;
      else
         usLeft = usRight;

   *pulLevel = MAKEULONG(usLeft, usRight);

   return(MCIERR_SUCCESS);

}  /* of CDAudStatCVol() */




/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  CDAUDPRO.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD AUDIO MCI DRIVER PROCESS COMMANDS                  */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the hardware independent code that         */
/*            PROCESS commands for the CD Audio MCI Driver uses.            */
/*            The entry point to the DLL is CDAUDIO.C                       */
/*                                                                          */
/* NOTES:  The hardware dependent code is found in file IBMCDROM.C.         */
/*                                                                          */
/* OTHER FUNCTIONS:                                                         */
/*       ProcClose     - process MCI_CLOSE command.                         */
/*       ProcConnector - process MCI_CONNECTOR command.                     */
/*       ProcCue       - process MCI_CUE command.                           */
/*       ProcCuePoint  - process MCI_SET_CUEPOINT command.                  */
/*       ProcGeneral   - process pass through MCI commands.                 */
/*       ProcCaps      - process MCI_GETDEVCAPS command.                    */
/*       ProcInfo      - process MCI_INFO command.                          */
/*       ProcMAudio    - process MCI_MASTERAUDIO command.                   */
/*       ProcOpen      - process MCI_OPEN command.                          */
/*       ProcPause     - process MCI_PAUSE command.                         */
/*       ProcPlay      - process MCI_PLAY command.                          */
/*       ProcPosAdvise - process MCI_SET_POSITION_ADVISE command.           */
/*       ProcRestore   - process MCIDRV_RESTORE command.                    */
/*       ProcResume    - process MCI_RESUME command.                        */
/*       ProcSave      - process MCIDRV_SAVE command.                       */
/*       ProcSeek      - process MCI_SEEK command.                          */
/*       ProcSet       - process MCI_SET command.                           */
/*       ProcSetSync   - process MCI_SET_SYNC_OFFSET command.               */
/*       ProcStatus    - process MCI_STATUS command.                        */
/*       ProcStop      - process MCI_STOP command.                          */
/*       ProcSync      - process MCIDRV_SYNC command.                       */
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
#include <ctype.h>
#include "hhpheap.h"

extern PVOID          CDMC_hHeap;

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcClose                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Close.                                        */
/*                                                                          */
/* FUNCTION:  Process MCI_CLOSE command.                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- pointer to instance.                           */
/*      ULONG  *pulParam1 -- flag for this message.                         */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcClose(PINST pInst, ULONG *pulParam1, PVOID pParam2, USHORT usUserParm)
{
   ULONG rc, ulP1Temp;

   /* send close command to VSD, remove non-VSD flags */
   ulP1Temp = (*pulParam1 & ~(MCI_NOTIFY | MCI_CLOSE_EXIT)) | MCI_WAIT;
   rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_CLOSE, &ulP1Temp, pParam2, 0);

   if (!rc)
   {
      pInst->valid[0] = ' ';               //invalidate instance

      /* free up memory */
      DosCloseMutexSem(pInst->hInstSem);

      if (pInst->ulTrackInfoSize)
         HhpFreeMem(CDMC_hHeap, pInst->pTrackInfo);

      /* send notification before destroying device context information */
      if (*pulParam1 & MCI_NOTIFY)
      {
         *pulParam1 ^= MCI_NOTIFY;
         *pulParam1 |= MCI_WAIT;
         mdmDriverNotify(pInst->usDeviceID,
                  ((MCI_GENERIC_PARMS *)pParam2)->hwndCallback, MM_MCINOTIFY,
                  usUserParm, MAKEULONG(MCI_CLOSE, MCI_NOTIFY_SUCCESSFUL));
      }  /* of if notify */

      HhpFreeMem(CDMC_hHeap, pInst);
   }  /* of if no error from VSD */

   return(rc);

}  /* of ProcClose() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcConnector                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Connector.                                    */
/*                                                                          */
/* FUNCTION:  Process MCI_CONNECTOR command.                                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY  -- No Disc is present.                     */
/*      MCIERR_INSTANCE_INACTIVE -- Instance is suspended.                  */
/*      MCIERR_INVALID_CONNECTOR_INDEX -- Invalid connector specified.      */
/*      MCIERR_INVALID_CONNECTOR_TYPE  -- Invalid connector specified.      */
/*      MCIERR_UNSUPPORTED_CONN_TYPE   -- Unsupported connector specified.  */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*      MCIERR_MISSING_FLAG         -- Flag is needed.                      */
/*      MCIERR_CANNOT_LOAD_DRIVER -- Unable to load VSD.                    */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcConnector(PINST pInst, ULONG ulParam1, MCI_CONNECTOR_PARMS *pParam2)
{
   ULONG rc;
   USHORT cnt = 0;
   ULONG ulType, ulIndex;

   rc = ValState(pInst);
   if ((rc != MCIERR_INSTANCE_INACTIVE) &&
          (ulParam1 & MCI_DISABLE_CONNECTOR ||
           ulParam1 & MCI_QUERY_CONNECTOR_STATUS))
      rc = MCIERR_SUCCESS;

   /********* check flags **********/
   if (!rc)
   {
      /* check exclusive flags */
      if (ulParam1 & MCI_ENABLE_CONNECTOR)
         cnt++;
      if (ulParam1 & MCI_DISABLE_CONNECTOR)
         cnt++;
      if (ulParam1 & MCI_QUERY_CONNECTOR_STATUS)
         cnt++;

      /* validate flags */
      if (!cnt)
         rc = MCIERR_MISSING_FLAG;
      else if (cnt > 1)
         rc = MCIERR_FLAGS_NOT_COMPATIBLE;
      else
      {
         ulType = ulParam1 & WAIT_NOTIFY_MASK;
         if (ulType & (0xFFFFFFFF ^
                    (MCI_ENABLE_CONNECTOR | MCI_DISABLE_CONNECTOR |
                     MCI_QUERY_CONNECTOR_STATUS |
                     MCI_CONNECTOR_TYPE | MCI_CONNECTOR_INDEX)))
            rc = MCIERR_INVALID_FLAG;
      }
   }   /* of if no error */

   /********* get connector index **********/
   if (!rc)
   {
      if (ulType & MCI_CONNECTOR_INDEX)
         ulIndex = pParam2->ulConnectorIndex;
      else
         ulIndex = 1;                  // set default

      /* get type of channel */
      /* This example assumes that there is only one connector, */
      /* the internal DAC or headphone.                         */
      if (ulParam1 & MCI_CONNECTOR_TYPE)
         if (pParam2->ulConnectorType == MCI_HEADPHONES_CONNECTOR)
         {
            if (ulIndex != 1)
               rc = MCIERR_INVALID_CONNECTOR_INDEX;
         }
         else
            if (pParam2->ulConnectorType > MCI_UNIVERSAL_CONNECTOR)
               rc = MCIERR_INVALID_CONNECTOR_TYPE;
            else
               rc = MCIERR_UNSUPPORTED_CONN_TYPE;
      else             /* no type is declared, use absolute numbers */
         if (ulIndex != 1)
            rc = MCIERR_INVALID_CONNECTOR_INDEX;
   }  /* of if no error */

   /******  process request  ********/
   if (!rc)
   {
      if (ulType & MCI_QUERY_CONNECTOR_STATUS)
      {
         if (pInst->ulMode & CDMC_INTDAC)
            pParam2->ulReturn = MCI_TRUE;
         else
            pParam2->ulReturn = MCI_FALSE;
         ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;   // Adjust rc type
      }
      else
         rc = SetConnector(pInst, ulType);
   }  /* of if no error */

   return(rc);

}  /* of ProcConnector() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcCue                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Cue.                                          */
/*                                                                          */
/* FUNCTION:  Process MCI_CUE command.                                      */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*      MCIERR_INVALID_FLAG       -- Unknown flag.                          */
/*      MCIERR_UNSUPPORTED_FLAG   -- Flag not supported by this MCD.        */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcCue(PINST pInst, ULONG ulParam1)
{
   ULONG rc, ulFlags;

   rc = ValState(pInst);

   if (!rc)
   {
      /* The CD Audio MCI Driver does not support MCI_RECORD */

      ulFlags = ulParam1 & WAIT_NOTIFY_MASK;

      switch (ulFlags)
      {
         case MCI_CUE_OUTPUT :
            /* call VSD (Vendor Specific Driver), *
             * the hardware specific MCI Driver   */
            pInst->usStatus = STOPPED;
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_CUE, 0L, 0L, 0);
            if (rc)
               rc = vsdResponse(pInst, rc);

            break;
         case MCI_CUE_OUTPUT | MCI_CUE_INPUT :
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
            break;
         case MCI_CUE_INPUT :       // CD MCI Driver doesn't support MCI_RECORD
            rc = MCIERR_UNSUPPORTED_FLAG;
            break;
         case 0 :
            rc = MCIERR_MISSING_FLAG;
            break;
         default :
            rc = MCIERR_INVALID_FLAG;

      }  /* of switch */
   }  /* if drive is ready for command */

   return(rc);

}  /* of ProcCue() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcCuePoint                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Cue Point                                     */
/*                                                                          */
/* FUNCTION:  Process MCI_SET_CUEPOINT command.                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- event does not exist.                          */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*      MCIERR_INVALID_CUEPOINT       --  unable to locate event.           */
/*      MCIERR_CUEPOINT_LIMIT_REACHED --  no more room to add events.       */
/*      MCIERR_DUPLICATE_CUEPOINT     --  duplicate cuepoint.               */
/*      MCIERR_INVALID_CALLBACK_HANDLE -- invalid call back handle.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcCuePoint(PINST pInst, ULONG ulParam1, MCI_CUEPOINT_PARMS *pParam2)
{
   ULONG rc, ulTime, ulFlags;
   MCI_CUEPOINT_PARMS recCuePoint;
   MCI_STATUS_PARMS recStatus;
   int i;
   PID pid;
   TID tid;

   rc = ValState(pInst);

   /* Make sure disc is present before you rely on cache */
   if (!rc)
   {
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
         rc = vsdResponse(pInst, rc);
   }  /* of if no error */

   /* if flag doesn't require an active/ready state, process it */
   if (!(ulParam1 & MCI_SET_CUEPOINT_ON) && rc != MCIERR_INSTANCE_INACTIVE)
      rc = MCIERR_SUCCESS;

   /* validate callback handle */
   if (!rc && !(ulParam1 & MCI_NOTIFY))
      if (!WinQueryWindowProcess(pParam2->hwndCallback, &pid, &tid))
         rc = MCIERR_INVALID_CALLBACK_HANDLE;

   if (!ULONG_LOWD(rc))
   {
      ulParam1 &= WAIT_NOTIFY_MASK;
      memcpy(&recCuePoint, pParam2, sizeof(MCI_CUEPOINT_PARMS));

      switch(ulParam1)
      {
         case MCI_SET_CUEPOINT_ON :
            rc = SetCuePoint(pInst, &recCuePoint);
            break;
         case MCI_SET_CUEPOINT_OFF :
            /* get time */
            ulTime = GetTimeAddr(pInst, pParam2->ulCuepoint, TRUE);
            if (ulTime == (ULONG) -1L)
               rc = MCIERR_OUTOFRANGE;         // invalid track number
            else
            {
               ulTime += pInst->ulOffset;

               /* find requested cuepoint and disable */
               for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
                  if (pInst->arrCuePoint[i] == ulTime)
                     break;

               if (i == CDMCD_CUEPOINT_MAX)
                  rc = MCIERR_INVALID_CUEPOINT;
               else
               {
                  pInst->arrCuePoint[i] = (ULONG) -1L;
                  recCuePoint.ulCuepoint = ulTime;
                  ulFlags = MCI_SET_CUEPOINT_OFF | MCI_WAIT;
                  rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_CUEPOINT,
                                         &ulFlags, &recCuePoint, 0);

               }  /* of else found cuepoint */
            }  /* of else no error, find cuepoint */
            break;
         case MCI_SET_CUEPOINT_ON | MCI_SET_CUEPOINT_OFF :
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
            break;
         case 0 :
            rc = MCIERR_MISSING_FLAG;
            break;
         default :
            rc = MCIERR_INVALID_FLAG;

      }  /* of switch */
   }  /* of if no error */

   if (rc)
      rc = vsdResponse(pInst, rc);

   return(rc);

}  /* of ProcCuePoint() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcGeneral                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process General commands.                             */
/*                                                                          */
/* FUNCTION:  Process any MCI command messages that the MCI does not        */
/*            handle but are completely handled by the hardware specific    */
/*            MCI Driver.                                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- pointer to instance.                           */
/*      USHORT usMessage  -- requested action to be performed.              */
/*      ULONG  *pulParam1 -- flag for this message.                         */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcGeneral(PINST pInst, USHORT usMessage, ULONG *pulParam1,
                  PVOID pParam2, USHORT usUserParm)
{
   ULONG rc = MCIERR_SUCCESS;
   ULONG ulP1Temp;
   PVOID pTemp;

   /* validate pointers within the pParam2 structure, if known */
   switch (usMessage)
   {
      case MCI_ESCAPE :
         /* check string pointer for validity */
         rc = ValPointer(((MCI_ESCAPE_PARMS *)pParam2)->pszCommand,
                        sizeof(BYTE));
         break;
      case MCI_GETTOC :
         /* validate TOC buffer pointer for existence & space for 1 entry */
         if (ValPointer(((MCI_TOC_PARMS *)pParam2)->pBuf,
                          sizeof(MCI_TOC_REC)))
            ((MCI_TOC_PARMS *)pParam2)->ulBufSize = 0L;   //invalid pointer
         ulP1Temp   = *pulParam1;
         *pulParam1 = (*pulParam1 & WAIT_NOTIFY_MASK) | MCI_WAIT;
         break;
      case MCI_LOAD :
         pTemp = ((MCI_LOAD_PARMS *)pParam2)->pszElementName;
         if (pTemp != NULL)
            rc = ValPointer(pTemp, sizeof(BYTE));
         break;
   }  /* of switch */

   /* validate state for message */
   if (!rc)
   {
      rc = ValState(pInst);
      /* protect GET TOC, let others slip by */
      if (rc == MCIERR_DEVICE_NOT_READY && usMessage != MCI_GETTOC)
         rc = MCIERR_SUCCESS;
   }

   if (!rc)
   {
      /* call VSD (Vendor Specific Driver), the hardware specific MCI Driver */
      rc = pInst->pMCIDriver(pInst->hHWMCID, usMessage, pulParam1,
                             pParam2, usUserParm);
      if (ULONG_LOWD(rc))
         rc = vsdResponse(pInst, rc);

   }  /* if drive is ready for command */

   if (usMessage == MCI_GETTOC)
      *pulParam1 = ulP1Temp;      // Restore original flags

   return(rc);

}  /* of ProcGeneral() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcCaps                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Capabilities                                  */
/*                                                                          */
/* FUNCTION:  Process MCI_GETDEVCAPS command.                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY     -- No Disc is present.                  */
/*      MCIERR_INSTANCE_INACTIVE    -- Instance is suspended.               */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- invalid combinations.                */
/*      MCIERR_PARAM_OVERFLOW       -- Invalid PARMS pointer.               */
/*      MCIERR_INVALID_FLAG         -- No message value.                    */
/*      MCIERR_INVALID_ITEM_FLAG    -- Invalid capability item.             */
/*                                                                          */
/* NOTES:  This function returns the drives capabilities, not those of the  */
/*         component nor of the instance.                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcCaps(PINST pInst, ULONG ulParam1, MCI_GETDEVCAPS_PARMS *pParam2)
{
   ULONG rc;

   ulParam1 &= WAIT_NOTIFY_MASK;
   rc = MCIERR_SUCCESS;       /* assume the best */
   ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;  //Most will be T/F, adjust later

   switch(ulParam1)
   {
      case MCI_GETDEVCAPS_MESSAGE :
         switch(pParam2->usMessage)
         {  /* check empty case */
            case 0 :
               pParam2->ulReturn = MCI_FALSE;
               rc = MCIERR_INVALID_FLAG;
               break;
            /* return true on those commands that the MCI Driver Does */
            case MCI_CLOSE :                  case MCI_CONNECTOR :
            case MCI_GETDEVCAPS :             case MCI_INFO :
            case MCI_MASTERAUDIO :            case MCI_OPEN :
            case MCI_SET :                    case MCI_SET_SYNC_OFFSET :
            case MCI_STATUS :
               pParam2->ulReturn = MCI_TRUE;
               break;
            /* return false on commands that the MCI Driver cannot do */
            case MCI_RECORD :
               pParam2->ulReturn = MCI_FALSE;
               break;
            /* return true on commands MCD does when streaming */
            case MCI_CUE :                    case MCI_PLAY :
            case MCI_PAUSE :                  case MCI_RESUME :
            case MCI_SEEK :                   case MCI_SET_CUEPOINT :
            case MCI_SET_POSITION_ADVISE :    case MCI_STOP :
               if (pInst->ulMode & CDMC_CAN_STREAM)
               {
                  pParam2->ulReturn = MCI_TRUE;
                  break;
               }
               /* else drop down to default and ask VSD */
            default :       /* ask VSD */
               ulParam1 |= MCI_WAIT;
               rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_GETDEVCAPS,
                                      &ulParam1, pParam2, 0);
               if (!ULONG_LOWD(rc))
                  ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
         }  /* of message switch */
         break;
      case MCI_GETDEVCAPS_ITEM :
         switch(pParam2->ulItem)
         {  /* check empty case */
            case 0 :
               pParam2->ulReturn = MCI_FALSE;
               rc = MCIERR_INVALID_ITEM_FLAG;
               break;
            /* list the always TRUE */
            case MCI_GETDEVCAPS_CAN_PLAY :
                  pParam2->ulReturn = MCI_TRUE;
               break;
            /* list the always FALSE */
            case MCI_GETDEVCAPS_CAN_RECORD :
            case MCI_GETDEVCAPS_CAN_RECORD_INSERT :
            case MCI_GETDEVCAPS_CAN_SAVE :
            case MCI_GETDEVCAPS_USES_FILES :
            case MCI_GETDEVCAPS_HAS_VIDEO :
                  pParam2->ulReturn = MCI_FALSE;
               break;
            /* list the conditionals */
            case MCI_GETDEVCAPS_CAN_EJECT :
               if (pInst->usCaps & CDVSD_CAP_CAN_EJECT)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_HAS_AUDIO :
               if (pInst->usCaps & CDVSD_CAP_HAS_AUDIO)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_DEVICE_TYPE :
               pParam2->ulReturn = MCI_DEVTYPE_CD_AUDIO;
               ULONG_HIWD(rc) = MCI_DEVICENAME_RETURN;
               break;
            case MCI_GETDEVCAPS_CAN_LOCKEJECT :
               if (pInst->usCaps & CDVSD_CAP_CAN_LOCK)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_CAN_PROCESS_INTERNAL :
               if (pInst->usCaps & CDVSD_CAP_HAS_DAC)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_CAN_SETVOLUME :
               if (pInst->usCaps & CDVSD_CAP_CAN_VOLUME)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_CAN_STREAM :
               if (pInst->usCaps & CDVSD_CAP_CAN_STREAM)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
               break;
            case MCI_GETDEVCAPS_PREROLL_TYPE :
               pParam2->ulReturn = pInst->ulPrerollType;
               ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
               break;
            case MCI_GETDEVCAPS_PREROLL_TIME :
               pParam2->ulReturn = pInst->ulPrerollTime;
               ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
               break;
            default :               // unknown flag, call VSD
               ulParam1 |= MCI_WAIT;
               rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_GETDEVCAPS,
                                      &ulParam1, pParam2, 0);

         }  /* of item switch */
         break;
      case MCI_GETDEVCAPS_MESSAGE | MCI_GETDEVCAPS_ITEM :
         rc = MCIERR_FLAGS_NOT_COMPATIBLE;
         break;
      case 0 :
         rc = MCIERR_MISSING_FLAG;
         break;
      default :               // unknown flag, call VSD
         ulParam1 |= MCI_WAIT;
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_GETDEVCAPS,
                                &ulParam1, pParam2, 0);

   }  /* of ulParam1 switch */

   return(rc);

}  /* of ProcCaps() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcInfo                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process device Information.                           */
/*                                                                          */
/* FUNCTION:  Process MCI_INFO command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INVALID_BUFFER     -- Buffer size was too small.             */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcInfo(PINST pInst, ULONG ulParam1, MCI_INFO_PARMS *pParam2)
{
   ULONG rc, ulFlags;
   USHORT len=UPC_SIZE+1;
   BYTE  buf[UPC_SIZE+1], cnt = 0;
   MCI_CD_ID *ptr;
   MCI_STATUS_PARMS recStatus;

   /* validate buffer pointer */
   if (ValPointer(pParam2->pszReturn, sizeof(BYTE)))
      pParam2->ulRetSize = 0L;        //invalid pointer

   /* validate flags */
   rc = MCIERR_SUCCESS;
   ulParam1 &= WAIT_NOTIFY_MASK;

   if (ulParam1 & MCI_CD_INFO_ID)
      cnt++;
   if (ulParam1 & MCI_CD_INFO_UPC)
      cnt++;
   if (ulParam1 & MCI_INFO_PRODUCT)
      cnt++;
   if (cnt > 1)
      rc = MCIERR_FLAGS_NOT_COMPATIBLE;

   if (!rc)
   {
      /* If it is something that can be handled internally,  */
      /*    find the minimum length to return                */
      if (ulParam1 == MCI_CD_INFO_ID || ulParam1 == MCI_CD_INFO_UPC)
      {
         if (pInst->usStatus < REGDISC)
            rc = MCIERR_DEVICE_NOT_READY;

         /* Make sure disc is present before you rely on cache */
         else
         {
            recStatus.ulItem = MCI_STATUS_POSITION;
            ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                                   &ulFlags, &recStatus, 0);
            if (ULONG_LOWD(rc))
               rc = vsdResponse(pInst, rc);
         }  /* of else disc was registered */

         if (pParam2->ulRetSize < UPC_SIZE+1)
            len = (USHORT) pParam2->ulRetSize;
         else
            len = UPC_SIZE+1;
      }  /* of if internal process flag */
   }  /* of if no error */

   if (!rc)
      /* process flag */
      switch(ulParam1)
      {
         case MCI_CD_INFO_ID :
            /* formulate in the buffer */
            ptr = (MCI_CD_ID *)buf;
            ptr->Mode = 1;                                   // ID = 1
            ptr->usTrack1 = (USHORT) pInst->recTrack.TrackRecArr->ulStartAddr;
            ptr->NumTracks = (BYTE) (pInst->recDisc.HighestTrackNum -
                             pInst->recDisc.LowestTrackNum + 1);
                /* lead-out track is end address for last track */
            ptr->ulLeadOut = (pInst->recTrack.TrackRecArr +
                             (ptr->NumTracks - 1))->ulEndAddr;

            if (len)
               memcpy(pParam2->pszReturn, buf, len);

            if (len == UPC_SIZE+1)
               rc = MCIERR_SUCCESS;
            else
               rc = MCIERR_INVALID_BUFFER;
            pParam2->ulRetSize = UPC_SIZE+1;

            break;
         case MCI_CD_INFO_UPC :
            if (len)   //if zero then invalid buffer
            {
               buf[0] = 0;
               memcpy(buf+1, pInst->recDisc.UPC, UPC_SIZE);
               memcpy(pParam2->pszReturn, buf, len);
            }

            if (len == UPC_SIZE+1)
               rc = MCIERR_SUCCESS;
            else
               rc = MCIERR_INVALID_BUFFER;
            pParam2->ulRetSize = UPC_SIZE+1;
            break;
         case 0 :
            rc = MCIERR_MISSING_FLAG;
            break;
         default :
            ulParam1 |= MCI_WAIT;
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_INFO, &ulParam1,
                                   pParam2, 0);
            if (ULONG_LOWD(rc))
               rc = vsdResponse(pInst, rc);

      }  /* of switch */

   return(rc);

}  /* of ProcInfo() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcMAudio                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Master Audio                                  */
/*                                                                          */
/* FUNCTION:  Process MCI_MASTERAUDIO command.                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      MCI_MASTERAUDIO_PARMS *pParam2 -- pointer to structure.             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Flags not compatible.                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcMAudio(PINST pInst, ULONG ulParam1, MCI_MASTERAUDIO_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;
   ULONG ulSetParam1;
   ULONG ulType;
   MCI_SET_PARMS recSet;

   /* eliminate general reserved flags */
   ulType = ulParam1 & WAIT_NOTIFY_MASK;

   switch(ulType)
   {
      case MCI_MASTERVOL:
         if (pParam2->ulMasterVolume > 100)
            pInst->ulMasterVolume = 100;
         else
            pInst->ulMasterVolume = pParam2->ulMasterVolume;
         break;
      case MCI_SPEAKERS | MCI_ON:
         pInst->ulMode |= CDMC_SPEAKER;
         break;
      case MCI_SPEAKERS | MCI_OFF:
         if (pInst->ulMode & CDMC_SPEAKER)
            pInst->ulMode ^= CDMC_SPEAKER;
         break;
      case MCI_HEADPHONES | MCI_ON:
         pInst->ulMode |= CDMC_HEADPHONE;
         break;
      case MCI_HEADPHONES | MCI_OFF:
         if (pInst->ulMode & CDMC_HEADPHONE)
            pInst->ulMode ^= CDMC_HEADPHONE;
         break;
      default :
         if (ulType & MCI_ON && ulType & MCI_OFF)
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
         else
            rc = MCIERR_INVALID_FLAG;
   }  /* of switch */

   /* update volume settings if needed */
   if (!rc)
   {
      rc = ValState(pInst);
      if (!rc || rc == MCIERR_INVALID_MEDIA_TYPE)
      {
         /* fill in Set record structure so that ProcSet() will respond */
         recSet.ulAudio = MCI_SET_AUDIO_LEFT;
         recSet.ulLevel = (ULONG) VOL_LEFT(pInst->ulLevel);
         recSet.ulOver  = 0L;
         ulSetParam1 = MCI_SET_AUDIO | MCI_SET_VOLUME | MCI_WAIT;

         ProcSet(pInst, &ulSetParam1, &recSet);

      }  /* is state is such that volume can be adjusted */
      else
         rc = MCIERR_SUCCESS;      // okay if not setting hardware
   }  /* no error from validating flags */

   return(rc);

}  /* of ProcMAudio() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcOpen                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Open.                                         */
/*                                                                          */
/* FUNCTION:  Process MCI_OPEN command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG  *pulParam1 -- flag for this message.                         */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- user defined parameter.                        */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*      MCIERR_OUTOFRANGE -- Cannot stream on stream only hardware.         */
/*      MCIERR_INI_FILE   -- Corrupted INI file, drive is not CD-ROM drive. */
/*      MCIERR_OUT_OF_MEMORY -- Could not create instance or semaphore.     */
/*      MCIERR_INVALID_DEVICE_NAME -- Illegal drive name.                   */
/*      MCIERR_PARAM_OVERFLOW      -- Invalid PARMS pointer.                */
/*      MCIERR_CANNOT_LOAD_DRIVER  -- Unable to load VSD.                   */
/*      MCIERR_INVALID_FLAG  -- Flag not supported by the VSD.              */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcOpen(ULONG *pulParam1, MMDRV_OPEN_PARMS *pParam2, USHORT usUserParm)
{
   ULONG rc;
   ULONG rsp, ulDevParmLen, usNotify = FALSE;
   PINST pInst = NULL;
   CHAR  buf[LOAD_MOD_BUF_LEN];
   CHAR  *pMCDToken, *pDummy, CDDrive;
   PVOID pMCDParm;
   int i;

   /* validate pointers */
   rc = ValPointer(pParam2->pDevParm, sizeof(BYTE));

   /* MCI_OPEN is from MDM, accept all flags as valid */

   /* get memory and pointer to instance structure */
   if (!rc)
   {
      pInst = HhpAllocMem(CDMC_hHeap, sizeof(struct instance_state));

      /* create semaphore so only one command will access instance table */
      if (pInst == NULL)
         rc = MCIERR_OUT_OF_MEMORY;
      else
         if (DosCreateMutexSem((PSZ) NULL, &pInst->hInstSem, 0L, FALSE))
            rc = MCIERR_OUT_OF_MEMORY;
   }  /* of if no error to get memory */

   if (!rc)
   {  /* instance entry created */
      pInst->usDeviceID = pParam2->usDeviceID;
      pInst->ulMasterVolume = (ULONG) -1L;
      pInst->usStatus = UNLOADED;
      /* set all fields in ulMode */
      pInst->ulMode = CDMC_MMTIME | CDMC_ALL_CH;
      pInst->hHWMCID = NULL;
      pInst->ulOffset = 0L;
      pInst->ulLevel = 0x00640064;                   /* set to 100% volume */
      pInst->ulTrackInfoSize = 0L;
      pInst->usDeviceOrd = pParam2->usDeviceOrd;
      pInst->recDisc.DiscID.Mode = (BYTE) -1;     /* used as first time flag */
      CDDrive = 0;
      memcpy(pInst->valid, "CDDA", VALLEN);

      /* process shareable flag */
      if (*pulParam1 & MCI_OPEN_SHAREABLE)
      {
         pInst->ulMode |= CDMC_SHAREABLE;
         *pulParam1 ^= MCI_OPEN_SHAREABLE;       // remove non-VSD flag
      }

      /* init cuepoint array */
      for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
         pInst->arrCuePoint[i] = (ULONG) -1L;   //clear flag

      /* Get and Validate drive letter */
      /* copy string so that it doesn't get disturb for the VSD */
      ulDevParmLen = strlen(pParam2->pDevParm) + 2;
      pMCDParm = (PSZ)   HhpAllocMem(CDMC_hHeap, ulDevParmLen);
      strcpy((CHAR *)pMCDParm, pParam2->pDevParm);
      parse_DevParm((CHAR *)pMCDParm, &pMCDToken, &pDummy);
      if (*(pMCDToken + 1) == '\0' || *(pMCDToken + 1) == ':')
         CDDrive = *pMCDToken;
      HhpFreeMem(CDMC_hHeap, pMCDParm);

      if (CDDrive < 'A' || CDDrive >'Z')
         rc = MCIERR_INVALID_DEVICE_NAME;      /* of if invalid drive letter */
      else
      {
         /* load hardware specific MCI Driver (VSD -- Vendor Specific Driver)*/
         /* fail if does not load */
         rsp = DosLoadModule(buf, LOAD_MOD_BUF_LEN,
                             pParam2->szDevDLLName, &pInst->hMod);
         if (!rsp)
            rsp = DosQueryProcAddr(pInst->hMod, 0L, "vsdDriverEntry",
                                   (PFN *) &pInst->pMCIDriver);
         if (rsp)
            rc = MCIERR_CANNOT_LOAD_DRIVER;
         else
         {
            /* open Vendor Specific Driver */
            if (*pulParam1 & MCI_NOTIFY)
            {
               usNotify = TRUE;
               *pulParam1 ^= MCI_NOTIFY; //remove flag, don't send notify twice
            }

            *pulParam1 |= MCI_WAIT;
            rc = pInst->pMCIDriver(NULL, MCI_OPEN, pulParam1, pParam2, 0);

         }  /* of else VSD loaded */
      }  /* of else valid drive letter */

      if (!rc)
      {
         /* get and pass instance handles */
         pInst->hHWMCID = pParam2->pInstance;
         pParam2->pInstance = (PVOID) pInst;
         pInst->usStatus = OPENED;

         /* Register Hardware & streaming capabilities */
         rc = Register(pInst);

      }
   }   /* of if no memory problem creating instance structure */

   if (rc)
   {
      if (pInst != NULL)      // if memory was created
      {
         DosCloseMutexSem(pInst->hInstSem);
         HhpFreeMem(CDMC_hHeap, pInst);
      }
   }  /* of if error */
   else
   {
      pInst->usStatus += SUSPEND;  // go inactive until MDM issues a restore

      /*****************************************************************/
      /* If successful and notify is requested then send notification. */
      /* This is needed because the instance pointer (lpInstance)      */
      /* in mciDriverEntry() is not valid on MCI_OPEN.                 */
      /*****************************************************************/
      if (usNotify)
         mdmDriverNotify(pInst->usDeviceID, pParam2->hwndCallback,
                         MM_MCINOTIFY, usUserParm,
                         MAKEULONG(MCI_OPEN, MCI_NOTIFY_SUCCESSFUL));

   }  /* of else no error */

   return(rc);

}  /* of ProcOpen() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcPause                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Pause.                                        */
/*                                                                          */
/* FUNCTION:  Process MCI_PAUSE command.                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcPause(PINST pInst, ULONG ulParam1)
{
   ULONG rc;

   rc = ValState(pInst);

   /* if drive is not ready, ignore call */
   if (rc == MCIERR_DEVICE_NOT_READY)
      if (ulParam1 & WAIT_NOTIFY_MASK)
         return(MCIERR_INVALID_FLAG);
      else
         return(MCIERR_SUCCESS);

   /* validate flags */
   if (!rc && ulParam1 & WAIT_NOTIFY_MASK)
      rc = MCIERR_INVALID_FLAG;

   if (!rc)
   {
      switch(pInst->usStatus)
      {
         case PLAYING:
            pInst->usStatus = PAUSED;

            /* call VSD (Vendor Specific Driver) */
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_PAUSE, 0L, 0L, 0);
            if (rc)
            {
               pInst->usStatus = STOPPED;
               rc = vsdResponse(pInst, rc);    //if error, process response
            }
            break;
         case PAUSED:
            /* already pausing */
            rc = MCIERR_SUCCESS;
            break;
         default : rc = MCIERR_SUCCESS;    //pretend that we paused

      }   /* of switch */
   }  /* if drive is ready for command */

   return(rc);

}  /* of ProcPause() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcPlay                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Play.                                         */
/*                                                                          */
/* FUNCTION:  Process MCI_PLAY command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- pointer to instance.                           */
/*      ULONG  *pulParam1 -- flag for this message.                         */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_OUTOFRANGE         -- invalid track, or cannot reverse.      */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*      MCIERR_NO_CONNECTION      -- no way to play, no stream/DAC.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcPlay(PINST pInst, ULONG *pulParam1,
               MCI_PLAY_PARMS *pParam2, USHORT usUserParm)
{
   ULONG rc, ulFlags;
   MCI_STATUS_PARMS recStatus;
   MCI_PLAY_PARMS recPlay;

   rc = ValState(pInst);

   /* validate flags */
   if (!rc && *pulParam1 & (0xFFFFFFFF ^
                         (MCI_WAIT | MCI_NOTIFY | MCI_FROM | MCI_TO)))
      rc = MCIERR_INVALID_FLAG;

   if (!rc)
   {
      /* Make sure disc is present before you rely on cache */
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
      {
         rc = vsdResponse(pInst, rc);
         if (!rc)                    //Disc Change, get starting address
            recStatus.ulReturn = pInst->ulStart_disk;
      }
   }  /* of if no error */

   if (!rc)
   {
      /* get starting address */
      if (*pulParam1 & MCI_FROM)
      {
         recPlay.ulFrom = GetTimeAddr(pInst, pParam2->ulFrom, TRUE);
         if (recPlay.ulFrom == (ULONG) -1L)
            rc = MCIERR_OUTOFRANGE;            // invalid track number
         else
            recPlay.ulFrom += pInst->ulOffset;
      }  /* of if FROM */
      else
      {   /* start at current position */
         recPlay.ulFrom = recStatus.ulReturn;
      }

      if (!rc)
      {
         /* get ending address */
         if (*pulParam1 & MCI_TO)
         {
            recPlay.ulTo = GetTimeAddr(pInst, pParam2->ulTo, TRUE);
            if (recPlay.ulTo == (ULONG) -1L)
               rc = MCIERR_OUTOFRANGE;            // invalid track number
            else
               recPlay.ulTo += pInst->ulOffset;
         }
         else
            recPlay.ulTo = pInst->ulEnd_disk;         /* no TO flag, goto end */

         /* check hardware start limitations */
         if (recPlay.ulFrom < pInst->ulMinStartTime)
            recPlay.ulFrom = pInst->ulMinStartTime;
         if (recPlay.ulTo < pInst->ulMinStartTime)
            recPlay.ulTo = pInst->ulMinStartTime;

         /* validate address */
         rc = ValAddress(pInst, &recPlay.ulFrom, &recPlay.ulTo, TRUE);

      }  /* of if no error after getting starting address */
   }  /* of if no error from validating state */

   /* Call the play command */
   if (rc)
      DosReleaseMutexSem(pInst->hInstSem);
   else
   {
      pInst->ulEnd_play = recPlay.ulTo;
      pInst->ulCur_pos  = recPlay.ulFrom;  //update current position

      if (pInst->ulMode & CDMC_INTDAC)
      {
         /* update instance prior to call */
         pInst->usStatus = PLAYING;
         recPlay.hwndCallback = pParam2->hwndCallback;
         DosReleaseMutexSem(pInst->hInstSem);
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_PLAY, pulParam1,
                                &recPlay, usUserParm);
         if (ULONG_LOWD(rc))
            rc = vsdResponse(pInst, rc);  //if error, process response
      }
      else
      {
         rc = MCIERR_NO_CONNECTION;       //no internal DAC
         DosReleaseMutexSem(pInst->hInstSem);
      }

      /* if play terminated with an error then set to STOP */
      if (ULONG_LOWD(rc) && pInst->usStatus > NODISC)
         pInst->usStatus = STOPPED;

   }  /* of else no error after validating addresses */

   return(rc);

}  /* of ProcPlay() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcPosAdvise                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Position Advise                               */
/*                                                                          */
/* FUNCTION:  Process MCI_SET_POSITION_ADVISE command.                      */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst      -- pointer to instance.                           */
/*      ULONG  ulParam1   -- flag for this message.                         */
/*      PVOID  pParam2    -- pointer to structure (message dependent).      */
/*      USHORT usUserParm -- User passthru value.                           */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- No Disc is present.                    */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcPosAdvise(PINST pInst, ULONG ulParam1, MCI_POSITION_PARMS *pParam2)
{
   ULONG rc, ulTime, ulFlags;
   MCI_POSITION_PARMS recPosition;
   MCI_STATUS_PARMS recStatus;
   PID pid;
   TID tid;

   rc = ValState(pInst);

   /* Make sure disc is present before you rely on cache */
   if (!rc)
   {
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
         rc = vsdResponse(pInst, rc);
   }  /* of if no error */

   /* if flag doesn't require an active/ready state, process it */
   if (!(ulParam1 & MCI_SET_POSITION_ADVISE_ON) &&
         rc != MCIERR_INSTANCE_INACTIVE)
      rc = MCIERR_SUCCESS;

   /* validate callback handle */
   if (!rc && !(ulParam1 & MCI_NOTIFY))
      if (!WinQueryWindowProcess(pParam2->hwndCallback, &pid, &tid))
         rc = MCIERR_INVALID_CALLBACK_HANDLE;

   if (!rc)
   {
      ulParam1 &= WAIT_NOTIFY_MASK;

      switch(ulParam1)
      {
         case MCI_SET_POSITION_ADVISE_ON :
            /* validate time */
            ulTime = GetTimeAddr(pInst, pParam2->ulUnits, TRUE);

            /* check for invalid track number or no position specified */
            if ((ulTime == (ULONG) -1L) || (ulTime == 0L))
               rc = MCIERR_OUTOFRANGE;
            else
               if (ulTime > pInst->ulEnd_disk)
                  rc = MCIERR_OUTOFRANGE;

            /* enable position advise */
            if (!rc)
            {
               memcpy(&recPosition, pParam2, sizeof(MCI_POSITION_PARMS));
               recPosition.ulUnits = ulTime;
               ulParam1 |= MCI_WAIT;
               rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_POSITION_ADVISE,
                                      &ulParam1, &recPosition, 0);

            }   /* of if no error, enable it */
            break;
         case MCI_SET_POSITION_ADVISE_OFF :
            ulParam1 |= MCI_WAIT;
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_POSITION_ADVISE,
                                   &ulParam1, 0L, 0);
            break;
         case MCI_SET_POSITION_ADVISE_ON | MCI_SET_POSITION_ADVISE_OFF :
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
            break;
         case 0 :
            rc = MCIERR_MISSING_FLAG;
            break;
         default :
            rc = MCIERR_INVALID_FLAG;

      }  /* of switch */
   }  /* of if no error */

   if (rc)
      rc = vsdResponse(pInst, rc);

   return(rc);

}  /* of ProcPosAdvise() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcRestore                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Restore.                                      */
/*                                                                          */
/* FUNCTION:  Process MCIDRV_RESTORE command.                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_FILE_NOT_FOUND -- improper disc not loaded.                  */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_OUT_OF_MEMORY      -- Out of memory for thread.              */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcRestore(PINST pInst)
{
   ULONG rc;
   MCI_CD_REGDISC_PARMS recDisc;
   USHORT usSameDisc = TRUE, usOldStatus;

   /* Set player the way it was */
   pInst->usStatus -= SUSPEND;

   if (pInst->usStatus <= NODISC)
   {
      rc = MCIERR_SUCCESS;
      pInst->usStatus = REGDRIVE;         //reset because position is not known

      /* check if disc was inserted */
      ReRegister(pInst);
   }
   else         // there was a disc when we saved
   {
      usOldStatus = pInst->usStatus;      // retain original status

      /* Validate disc as same as old state */
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_REGISTER_DISC,
                             0L, &recDisc, 0);
      if (rc)
         usSameDisc = FALSE;
      else
         if (memcmp(&pInst->recDisc.DiscID, &recDisc.DiscID, IDSIZE))
            usSameDisc = FALSE;

      /* get current master volume settings */
      QMAudio(pInst);

      /* set parameters on restore */
      pInst->recSave.ulEndPlay = pInst->ulEnd_play;

      /* set volume */
      if (pInst->ulMode & CDMC_LFT_CH && pInst->ulMode & CDMC_HEADPHONE)
         VOL_LEFT(pInst->recSave.ulLevel) = (USHORT)
            ((ULONG)VOL_LEFT(pInst->ulLevel) * pInst->ulMasterVolume / 100);
      else
         VOL_LEFT(pInst->recSave.ulLevel) = 0;

      if (pInst->ulMode & CDMC_RGT_CH && pInst->ulMode & CDMC_HEADPHONE)
         VOL_RIGHT(pInst->recSave.ulLevel) = (USHORT)
            ((ULONG)VOL_RIGHT(pInst->ulLevel) * pInst->ulMasterVolume / 100);
      else
         VOL_RIGHT(pInst->recSave.ulLevel) = 0;

      /* set restoring mode */
      if (!usSameDisc)
         pInst->recSave.ulMode = MCI_MODE_NOT_READY;
      else if (pInst->ulMode & CDMC_STREAM)
         pInst->recSave.ulMode = MCI_MODE_STOP;
      else
         switch (usOldStatus)
         {
            case PLAYING:
               pInst->recSave.ulMode = MCI_MODE_PLAY;
               break;
            case PAUSED:
               pInst->recSave.ulMode = MCI_MODE_PAUSE;
               break;
            default :                           // must be status = STOPPED
               pInst->recSave.ulMode = MCI_MODE_STOP;

         }  /* of switch */

      rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_RESTORE, 0L,
                             &pInst->recSave, 0);

      if (rc)
         rc = vsdResponse(pInst, rc);       //if error, process response

      if (!usSameDisc)
         rc = ReRegister(pInst);            //accept and load new disc

   }  /* of else there was a disc when we saved */

   return(MCIERR_SUCCESS);

}  /* of ProcRestore() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcResume                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Resume                                        */
/*                                                                          */
/* FUNCTION:  Process MCI_RESUME command.  Unpause after a Pause.           */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcResume(PINST pInst, ULONG ulParam1)
{
   ULONG rc, ulFlags;
   MCI_STATUS_PARMS recStatus;

   rc = ValState(pInst);

   /* if drive is not ready, ignore call */
   if (rc == MCIERR_DEVICE_NOT_READY)
      if (ulParam1 & WAIT_NOTIFY_MASK)
         return(MCIERR_INVALID_FLAG);
      else
         return(MCIERR_SUCCESS);

   /* validate flags */
   if (!rc && ulParam1 & WAIT_NOTIFY_MASK)
      rc = MCIERR_INVALID_FLAG;

   if (!rc)   /* make sure we are in a PAUSED state */
      if (pInst->usStatus != PAUSED)
         return(MCIERR_SUCCESS);     // ignore call,

   if (!rc)   /* Make sure disc is valid before resuming */
   {
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
      {
         rc = vsdResponse(pInst, rc);
         return(rc);                //if rc==MCIERR_SUCCESS means Disc Change
      }  /* of if VSD reported an error */
   }  /* if no error */

   if (!rc)
   {
      pInst->usStatus = PLAYING;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_RESUME, 0L, 0L, 0);
      if (ULONG_LOWD(rc))
      {
         pInst->usStatus = STOPPED;
         rc = vsdResponse(pInst, rc);       //if error, process response
      }
   }  /* of if no error */

   return(rc);

}  /* of ProcResume() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcSave                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Save.                                         */
/*                                                                          */
/* FUNCTION:  Process MCI_SAVE command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcSave(PINST pInst)
{
   ULONG rc;

   pInst->usStatus += SUSPEND;

   /* tell the VSD that it is being saved and get values */
   pInst->recSave.ulLevel = pInst->ulLevel;
   rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_SAVE, 0L, &pInst->recSave, 0);

   pInst->ulCur_pos = pInst->recSave.ulPosition;

   if (rc)             /* info was not saved, restore will re-register */
      pInst->usStatus = NODISC + SUSPEND;

   return(MCIERR_SUCCESS);

}  /* of ProcSave() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcSeek                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Seek.                                         */
/*                                                                          */
/* FUNCTION:  Process MCI_SEEK command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_OUTOFRANGE         -- invalid track or address.              */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcSeek(PINST pInst, ULONG ulParam1, MCI_SEEK_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS, ulFlags;
   MCI_SEEK_PARMS recSeek;
   MCI_STATUS_PARMS recStatus;
   ULONG ulType;

   rc = ValState(pInst);

   /* validate flags */
   if (!rc)
      if (ulParam1 & (0xFFFFFFFF ^
                 (MCI_WAIT | MCI_NOTIFY | MCI_TO | MCI_TO_START | MCI_TO_END)))
         rc = MCIERR_INVALID_FLAG;
      else
      {
         /* Make sure disc is present before you rely on cache */
         recStatus.ulItem = MCI_STATUS_POSITION;
         ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                                &ulFlags, &recStatus, 0);
         if (ULONG_LOWD(rc))
            rc = vsdResponse(pInst, rc);
      }

   if (!rc)
   {
      ulType = ulParam1 & WAIT_NOTIFY_MASK;
      switch (ulType)
      {
         case MCI_TO_START :            /* seek to first position */
            recSeek.ulTo = pInst->ulStart_disk;
            break;
         case MCI_TO_END :              /* seek to last position */
            recSeek.ulTo = pInst->ulEnd_disk - MMT_FRAME;
            break;
         case 0L :                      /* No address specified, error */
            rc = MCIERR_MISSING_FLAG;
            break;
         case MCI_TO :                  /* seek to specified address */
            /*    make sure it is a playable address */
            recSeek.ulTo = GetTimeAddr(pInst, pParam2->ulTo, TRUE);
            if (recSeek.ulTo == (ULONG) -1L)
               rc = MCIERR_OUTOFRANGE;            // invalid track number
            else
            {
               recSeek.ulTo += pInst->ulOffset;
               rc = ValAddress(pInst, NULL, &recSeek.ulTo, FALSE);
            }
            break;
         default :                      /* unknown flag combination */
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
      }  /* of switch() */

      if (!rc)
      {                                          /* No error, set it up */
         /* check hardware start limitations */
         if (recSeek.ulTo < pInst->ulMinStartTime)
            recSeek.ulTo = pInst->ulMinStartTime;

         /* call VSD (Vendor Specific Driver) */
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SEEK, 0L, &recSeek, 0);

         pInst->usStatus = STOPPED;

         if (ULONG_LOWD(rc))
             rc = vsdResponse(pInst, rc);      //if error, process response

      }   /* of if no error */
   }  /* of if no error from validating state */

   return(rc);

}  /* of ProcSeek() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcSet                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Set.                                          */
/*                                                                          */
/* FUNCTION:  Process MCI_SET command.                                      */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- pointer to instance.                            */
/*      ULONG *pulParam1 -- flag for this message.                          */
/*      PVOID pParam2    -- pointer to structure (message dependent).       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*      MCIERR_UNSUPPORTED_FLAG   -- Flag not supported by this MCD.        */
/*      MCIERR_INVALID_FLAG       -- Unknown flag or value used.            */
/*      MCIERR_SPEED_FORMAT_FLAG  -- Unknown speed type specified.          */
/*      MCIERR_OUT_OF_MEMORY      -- Could not create thread.               */
/*      MCIERR_INVALID_AUDIO_FLAG -- Unknown audio flag specified.          */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Conflicting Flags.                   */
/*      MCIERR_INVALID_TIME_FORMAT_FLAG  -- Unknown time type specified.    */
/*                                                                          */
/* NOTES:  Flags needing a valid pParam2 are checked in verify_entry()      */
/*                                                                          */
/****************************************************************************/

ULONG ProcSet(PINST pInst, ULONG *pulParam1, MCI_SET_PARMS *pParam2)
{
   ULONG rc, ulLevel, ulP1Temp;
   USHORT SemHolding = TRUE;
   ULONG ulMode;
   MCI_SET_PARMS recSet;

   rc = ValState(pInst);
   if (rc == MCIERR_INVALID_MEDIA_TYPE)
      rc = MCIERR_SUCCESS;

   /* validate flags */
   if (!rc)
   {
      ulP1Temp = *pulParam1 & WAIT_NOTIFY_MASK;          //mask off N/W flags
      if (ulP1Temp & MCI_SET_VIDEO || ulP1Temp & MCI_SET_ITEM)
         rc = MCIERR_UNSUPPORTED_FLAG;
      else
         if (!(ulP1Temp) ||                              // no flags,
              (!(ulP1Temp & MCI_SET_AUDIO) &&            // check flags needing
                (ulP1Temp & MCI_SET_VOLUME ||            //   MCI_SET_AUDIO
                 ulP1Temp & MCI_SET_ON || ulP1Temp & MCI_SET_OFF)))
            rc = MCIERR_MISSING_FLAG;
   }  /* of if no error to validate first set of flags */

   if (!rc)
   {
      ulMode = pInst->ulMode;
      ulLevel = pInst->ulLevel;

      /* copy pParam2 structure to make changes */
      if (pParam2 != NULL)
         memcpy(&recSet, pParam2, sizeof(MCI_SET_PARMS));

      if (ulP1Temp & MCI_SET_SPEED_FORMAT)
      {
         ulP1Temp ^= MCI_SET_SPEED_FORMAT;        // remove non VSD flag
         switch (pParam2->ulSpeedFormat)
         {
            case MCI_FORMAT_PERCENTAGE :
               ulMode |= CDMC_PERCENT;
               break;
            case MCI_FORMAT_FPS :
               if (ulMode & CDMC_PERCENT)
                  ulMode ^= CDMC_PERCENT;
               break;
            default : rc = MCIERR_SPEED_FORMAT_FLAG;

         }  /* of switch time format */
      }  /* of if speed format */

      if (!rc && ulP1Temp & MCI_SET_TIME_FORMAT)
      {
         ulP1Temp ^= MCI_SET_TIME_FORMAT;        // remove non VSD flag
         switch (pParam2->ulTimeFormat)
         {
            case MCI_FORMAT_MILLISECONDS :
               ulMode = ulMode & TIME_MODE_SET | CDMC_MILLSEC;
               break;
            case MCI_FORMAT_MMTIME :
               ulMode = ulMode & TIME_MODE_SET | CDMC_MMTIME;
               break;
            case MCI_FORMAT_MSF :
               ulMode = ulMode & TIME_MODE_SET | CDMC_REDBOOK;
               break;
            case MCI_FORMAT_TMSF :
               ulMode = ulMode & TIME_MODE_SET | CDMC_TMSF;
               break;
            default : rc = MCIERR_INVALID_TIME_FORMAT_FLAG;

         }  /* of switch time format */
      }  /* of if time format */

      if (!rc && ulP1Temp & MCI_SET_DOOR_LOCK && ulP1Temp & MCI_SET_DOOR_UNLOCK)
         rc = MCIERR_FLAGS_NOT_COMPATIBLE;

      if (!rc && ulP1Temp & MCI_SET_DOOR_OPEN && ulP1Temp & MCI_SET_DOOR_CLOSED)
         rc = MCIERR_FLAGS_NOT_COMPATIBLE;

      if (!rc && ulP1Temp & MCI_SET_AUDIO)
         rc = SetAudio(pInst, &ulP1Temp, &recSet, &ulMode, &ulLevel);

      /* process VSD flags, if they exist */
      if (!rc && ulP1Temp)
      {
         ulP1Temp |= MCI_WAIT;
         rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SET, &ulP1Temp,
                                &recSet, 0);
         if (rc)
            rc = vsdResponse(pInst, rc);      //if error, process response
      }
   }  /* of if valid state */

   if (!rc)
   {
      pInst->ulMode = ulMode;
      pInst->ulLevel = ulLevel;
   }  /* of else no error */

   if (SemHolding)             //Release semaphore if not already done
      DosReleaseMutexSem(pInst->hInstSem);

   return(rc);

}  /* of ProcSet() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcSetSync                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Set Synchronization Offset                    */
/*                                                                          */
/* FUNCTION:  Process MCI_SET_SYNC_OFFSET command.                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS      -- action completed without error.              */
/*      MCIERR_INVALID_FLAG -- Unknown flag or value used.                  */
/*      MCIERR_OUTOFRANGE   -- invalid track specified.                     */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcSetSync(PINST pInst, ULONG ulParam1, MCI_SYNC_OFFSET_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;
   USHORT temp;

   ulParam1 &= WAIT_NOTIFY_MASK;

   if (ulParam1)
      rc = MCIERR_INVALID_FLAG;
   else
   {
      /* check if disc is needed, track info is needed for TMSF mode */
      if (pInst->ulMode & TIME_MODE == CDMC_TMSF)
      {
         temp = (USHORT)(pInst->usStatus % SUSPEND);
         if (temp <= NODISC)
            rc = MCIERR_DEVICE_NOT_READY;
      }

      if (!rc)
      {
         pInst->ulOffset = GetTimeAddr(pInst, pParam2->ulOffset, TRUE);
         if (pInst->ulOffset == (ULONG) -1)
         {                              // Invalid track specified for TMSF
            pInst->ulOffset = 0L;
            rc = MCIERR_OUTOFRANGE;
         }
      }  /* of if no error */
   }  /* no invalid flags */

   return(rc);

}  /* of ProcSetSync() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcStatus                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Status.                                       */
/*                                                                          */
/* FUNCTION:  Process MCI_STATUS command.                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- invalid track supplied.                        */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INVALID_ITEM_FLAG  -- Invalid item specified.                */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_PARAM_OVERFLOW     -- Invalid PARMS pointer.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcStatus(PINST pInst, ULONG ulParam1, MCI_STATUS_PARMS *pParam2)
{
   ULONG rc, rsp;

   rsp = ValState(pInst);

   /* validate flags */
   ulParam1 &= WAIT_NOTIFY_MASK;
   pParam2->ulReturn = 0L;

   switch(ulParam1)
   {
      case MCI_STATUS_ITEM :
         rc = MCIERR_SUCCESS;
         switch(pParam2->ulItem)
         {
            /* process items that the MCI Driver can do */

            case MCI_STATUS_AUDIO :
               ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
               switch (pParam2->ulValue)
               {
                  case MCI_STATUS_AUDIO_LEFT :
                     if (pInst->ulMode & CDMC_LFT_CH)
                        pParam2->ulReturn = MCI_TRUE;
                     else
                        pParam2->ulReturn = MCI_FALSE;
                     break;
                  case MCI_STATUS_AUDIO_RIGHT :
                     if (pInst->ulMode & CDMC_RGT_CH)
                        pParam2->ulReturn = MCI_TRUE;
                     else
                        pParam2->ulReturn = MCI_FALSE;
                     break;
                  case MCI_STATUS_AUDIO_ALL :
                     if (pInst->ulMode & CDMC_ALL_CH)
                        pParam2->ulReturn = MCI_TRUE;
                     else
                        pParam2->ulReturn = MCI_FALSE;
                     break;
                  default : rc = MCIERR_INVALID_AUDIO_FLAG;
               }  /* of switch */
               break;
            case MCI_STATUS_SPEED_FORMAT :
               ULONG_HIWD(rc) = MCI_SPEED_FORMAT_RETURN;
               if (pInst->ulMode & CDMC_PERCENT)
                  pParam2->ulReturn = MCI_FORMAT_PERCENTAGE;
               else
                  pParam2->ulReturn = MCI_FORMAT_FPS;
               break;
            case MCI_STATUS_TIME_FORMAT :
               ULONG_HIWD(rc) = MCI_TIME_FORMAT_RETURN;
               switch (pInst->ulMode & TIME_MODE)
               {
                  case CDMC_MILLSEC :
                     pParam2->ulReturn = MCI_FORMAT_MILLISECONDS;
                     break;
                  case CDMC_REDBOOK :
                     pParam2->ulReturn = MCI_FORMAT_MSF;
                     break;
                  case CDMC_TMSF :
                     pParam2->ulReturn = MCI_FORMAT_TMSF;
                     break;
                  default :              // must be CDMC_MMTIME
                     pParam2->ulReturn = MCI_FORMAT_MMTIME;
               }  /* of switch */
               break;

            /* Process commands that the MCD can do with the VSD */

            case MCI_STATUS_LENGTH :
            case MCI_STATUS_NUMBER_OF_TRACKS :
               rc = StatusMCD(pInst, ulParam1, pParam2, rsp, TRUE);
               break;
            case MCI_STATUS_POSITION :
            case MCI_STATUS_POSITION_IN_TRACK :
            case MCI_STATUS_CURRENT_TRACK :
            case MCI_CD_STATUS_TRACK_TYPE :
            case MCI_CD_STATUS_TRACK_COPYPERMITTED :
            case MCI_CD_STATUS_TRACK_CHANNELS :
            case MCI_CD_STATUS_TRACK_PREEMPHASIS :
               rc = StatusMCD(pInst, ulParam1, pParam2, rsp, FALSE);
               break;

            /* Process commands that the VSD can only do   */
            /* MCI_STATUS_MEDIA_PRESENT, MCI_STATUS_VOLUME */
            /* MCI_STATUS_MODE, MCI_STATUS_READY, others   */
            default :
               rc = StatusVSD(pInst, ulParam1, pParam2, rsp);

         }  /* of item switch */
         break;
      case MCI_STATUS_ITEM | MCI_TRACK :
         switch(pParam2->ulItem)
         {
            case MCI_STATUS_LENGTH :
            case MCI_CD_STATUS_TRACK_TYPE :
            case MCI_CD_STATUS_TRACK_COPYPERMITTED :
            case MCI_STATUS_POSITION :
            case MCI_CD_STATUS_TRACK_CHANNELS :
            case MCI_CD_STATUS_TRACK_PREEMPHASIS :
               rc = StatusMCD(pInst, ulParam1, pParam2, rsp, TRUE);
               break;
            default :
               rc = StatusVSD(pInst, ulParam1, pParam2, rsp);
         }  /* of item switch */
         break;
      case MCI_STATUS_ITEM | MCI_STATUS_START :
         switch(pParam2->ulItem)
         {
            case MCI_STATUS_POSITION :
               rc = StatusMCD(pInst, ulParam1, pParam2, rsp, FALSE);
               break;
            default :
               rc = MCIERR_INVALID_ITEM_FLAG;
         }  /* of item switch */
         break;
      case 0 :
         rc = MCIERR_MISSING_FLAG;
         break;
      default :
         if (rsp)
         {
            if (rsp == MCIERR_INSTANCE_INACTIVE)
               if (pInst->usStatus - SUSPEND <= NODISC)
                  rc = MCIERR_DEVICE_NOT_READY;
               else
                  rsp = MCIERR_SUCCESS;    //clear flag and let VSD process it
            else
               rc = rsp;
         }

         if (!rsp)        // unknown flag, query VSD
         {
            ulParam1 |= MCI_WAIT;
            rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                                   &ulParam1, pParam2, 0);
            if (ULONG_LOWD(rc))
               rc = vsdResponse(pInst, rc);      //if error, process response
         }
   }  /* of switch on flags */

   return(rc);

}  /* of ProcStatus() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcStop                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Stop.                                         */
/*                                                                          */
/* FUNCTION:  Process MCI_STOP command.                                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcStop(PINST pInst, ULONG ulParam1)
{
   ULONG rc;

   rc = ValState(pInst);

   /* if drive is not ready, ignore call */
   if (rc == MCIERR_DEVICE_NOT_READY)
      if (ulParam1 & WAIT_NOTIFY_MASK)
         return(MCIERR_INVALID_FLAG);
      else
         return(MCIERR_SUCCESS);

   /* validate flags */
   if (!rc && ulParam1 & WAIT_NOTIFY_MASK)
      rc = MCIERR_INVALID_FLAG;

   if (!rc)
   {
      /* call VSD (Vendor Specific Driver) */
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STOP, 0L, 0L, 0);
      if (rc)
          rc = vsdResponse(pInst, rc);          //if error, process response

      pInst->usStatus = STOPPED;

   }  /* if drive is ready for command */

   return(rc);

}  /* of ProcStop() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ProcSync                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Process Synchronization                               */
/*                                                                          */
/* FUNCTION:  Process MCIDRV_SYNC command.                                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*      MCIERR_INSTANCE_INACTIVE  -- Instance is suspended.                 */
/*      MCIERR_NO_CONNECTION      -- no way to play, no stream/DAC.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG ProcSync(PINST pInst, ULONG ulParam1, MCIDRV_SYNC_PARMS *pParam2)
{
   ULONG rc;
   USHORT cnt=0;

   rc = ValState(pInst);

   if (rc != MCIERR_INSTANCE_INACTIVE)
   {
      /* validate flags */
      ulParam1 &= WAIT_NOTIFY_MASK;
      if (ulParam1 & MCIDRV_SYNC_ENABLE)
         cnt++;
      if (ulParam1 & MCIDRV_SYNC_DISABLE)
         cnt++;
      if (ulParam1 & MCIDRV_SYNC_SET_MASTER)
         cnt++;
      if (ulParam1 & MCIDRV_SYNC_REC_PULSE)
         cnt++;
      if (cnt > 1)
         return(MCIERR_FLAGS_NOT_COMPATIBLE);

      switch (ulParam1)
      {
         case MCIDRV_SYNC_ENABLE :
            rc = MCIERR_SUCCESS;

            if (pInst->ulMode & CDMC_INTDAC)
            {
               ulParam1 |= MCI_WAIT;
               rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_SYNC,
                                      &ulParam1, pParam2, 0);
            }
            else
               rc = MCIERR_NO_CONNECTION;   //no internal DAC
            break;
         case MCIDRV_SYNC_DISABLE :
         case MCIDRV_SYNC_MASTER :
            rc = MCIERR_SUCCESS;
         case MCIDRV_SYNC_REC_PULSE :
            if (!rc)
               if (pInst->ulMode & CDMC_INTDAC)
               {
                  ulParam1 |= MCI_WAIT;
                  rc = pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_SYNC,
                                         &ulParam1, pParam2, 0);
               }
               else
                  if (!(pInst->ulMode & CDMC_STREAM))
                     rc = MCIERR_NO_CONNECTION;   //not streaming either
            break;
         case 0L :
            if (!rc)
               rc = MCIERR_MISSING_FLAG;
            break;
         default :
            if (!rc)
               rc = MCIERR_INVALID_FLAG;
      }  /* of switch */

      if (rc)
          rc = vsdResponse(pInst, rc);          //if error, process response

   }  /* if drive is ready for command */

   return(rc);

}  /* of ProcSync() */



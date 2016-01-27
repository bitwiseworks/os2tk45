/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  CDAUDUTL.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD AUDIO MCI DRIVER UTILITIES                         */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the hardware independent code that         */
/*            supplement the PROCESS commands in CDAUDPRO.C for the         */
/*            CD Audio MCI Driver uses.  The entry point to the DLL is      */
/*            in CDAUDIO.C                                                  */
/*                                                                          */
/* NOTES:  The hardware dependent code is found in file IBMCDROM.C.         */
/*                                                                          */
/* OTHER FUNCTIONS:                                                         */
/*       SetAudio      - Set audio information from MCI_SET.                */
/*       SetConnector  - Enable or disable connection.                      */
/*       SetCuePoint   - Enable cue point.                                  */
/*       StatusMCD     - Get status from MCD information.                   */
/*       StatusMCDDef  - Get status from MCD default information.           */
/*       StatusVSD     - Get status from VSD information.                   */
/*       DisableEvents - Disable cuepoints and position advise.             */
/*       GetTimeAddr   - Convert a time format to/from MMTIME.              */
/*       GetTimeAddrRC - Colinize return code to time format.               */
/*       GetTrackInfo  - Get Track info for specified track or address.     */
/*       ValAddress    - Validate address for play and seek commands.       */
/*       ValState      - Validate state, has disc and not suspended.        */
/*       vsdResponse   - Process VSD Response.                              */
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


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  SetAudio                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Set Audio                                             */
/*                                                                          */
/* FUNCTION:  Set Audio information when MCI_SET_AUDIO flag is used with    */
/*            MCI_SET command.  This function is called by ProcSet().       */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- pointer to instance.                            */
/*      ULONG *pulParam1 -- flag for this message.                          */
/*      MCI_SET_PARMS *pParam2 -- pointer to local SET structure.           */
/*      ULONG *pulMode   -- mode flags.                                     */
/*      ULONG *pulLevel  -- volume level.                                   */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Flags not compatible.                */
/*      MCIERR_INVALID_AUDIO_FLAG   -- Unknown audio flag specified.        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG SetAudio(PINST pInst, ULONG *pulParam1, MCI_SET_PARMS *pParam2,
               ULONG *pulMode, ULONG *pulLevel)
{
   ULONG rc = MCIERR_SUCCESS;          // assume the best
   USHORT usFlag = 0;
   ULONG  ulChan = 0;
   USHORT left, right;

   if (*pulParam1 & MCI_SET_ON && *pulParam1 & MCI_SET_OFF)
      rc = MCIERR_FLAGS_NOT_COMPATIBLE;
   else
      switch (pParam2->ulAudio)
      {
         case MCI_SET_AUDIO_LEFT :
            ulChan = CDMC_LFT_CH;
            break;
         case MCI_SET_AUDIO_RIGHT :
            ulChan = CDMC_RGT_CH;
            break;
         case MCI_SET_AUDIO_ALL :
            ulChan = CDMC_ALL_CH;
            break;
         default : rc = MCIERR_INVALID_AUDIO_FLAG;
      }  /* of switch */


   if (!rc)
   {
      *pulParam1 ^= MCI_SET_AUDIO;       // remove non VSD flag

      /* Remove flag for vectored volume, not supported in this example */
      if (*pulParam1 & MCI_OVER)
         *pulParam1 ^= MCI_OVER;

      if (*pulParam1 & MCI_SET_ON)
      {
         usFlag = 1;
         *pulParam1 ^= MCI_SET_ON;       // remove non VSD flag
         *pulMode |= ulChan;
      }  /* of else if ON */
      else if (*pulParam1 & MCI_SET_OFF)
      {
         usFlag = 1;
         *pulParam1 ^= MCI_SET_OFF;      // remove non VSD flag
         if (ulChan & CDMC_LFT_CH)
            *pulMode &= CHAN_MODE_SET | CDMC_RGT_CH;
         if (ulChan & CDMC_RGT_CH)
            *pulMode &= CHAN_MODE_SET | CDMC_LFT_CH;
      }  /* of else if OFF */

      if (*pulParam1 & MCI_SET_VOLUME)
      {
         usFlag = 1;
         /* get requested volume levels */
         left  = (USHORT) pParam2->ulLevel;
         right = (USHORT) pParam2->ulLevel;

         if (ulChan & CDMC_LFT_CH)
            if (left > 100)
               VOL_LEFT(*pulLevel) = 100;
            else
               VOL_LEFT(*pulLevel) = left;
         if (ulChan & CDMC_RGT_CH)
            if (right > 100)
               VOL_RIGHT(*pulLevel) = 100;
            else
               VOL_RIGHT(*pulLevel) = right;
      }  /* of setting volume */

      if (usFlag)                // CHANGE, SET VOLUME
      {
         /* Go ahead and prepare for call to VSD */
         if (*pulMode & CDMC_LFT_CH && pInst->ulMode & CDMC_HEADPHONE)
            left = (USHORT)((ULONG)VOL_LEFT(*pulLevel) *
                   pInst->ulMasterVolume / 100);
         else           /* else channel is muted */
            left = 0;
         if (*pulMode & CDMC_RGT_CH && pInst->ulMode & CDMC_HEADPHONE)
            right = (USHORT)((ULONG)VOL_RIGHT(*pulLevel) *
                   pInst->ulMasterVolume / 100);
         else           /* else channel is muted */
            right = 0;
         pParam2->ulLevel = MAKEULONG(left, right);
         *pulParam1 |= MCI_SET_VOLUME;      //make sure VSD flag is set

      }  /* of if change */
      else
         rc = MCIERR_MISSING_FLAG;
   }  /* of if no error with flags */

   return(rc);

}  /* of SetAudio() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  SetConnector                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Set Connector                                         */
/*                                                                          */
/* FUNCTION:  Enable or disable connectors from the MCI_CONNECTOR command.  */
/*            This function is called by ProcConnector().                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY  -- No Disc is present.                     */
/*      MCIERR_UNSUPPORTED_FLAG     -- cannot process constant.             */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Invalid flag combinations.           */
/*      MCIERR_INVALID_FLAG         -- Unknown or unsupported flag.         */
/*      MCIERR_CANNOT_LOAD_DRIVER -- Unable to load VSD.                    */
/*                                                                          */
/* NOTES:   ProcConnector() calls this function only when                   */
/*          MCI_ENABLE_CONNECTOR or MCI_DISABLE_CONNECTOR flags were used.  */
/*                                                                          */
/****************************************************************************/

ULONG SetConnector(PINST pInst, ULONG ulParam1)
{
   ULONG rsp, rc = MCIERR_SUCCESS;          // assume the best
   MCI_STATUS_PARMS recStatus;

   /* get current position -- will be placed in pInst->ulCur_pos */
   /* don't use recStatus.ulReturn, it will be adjusted with     */
   /*   sync offset and current time format setting              */
   recStatus.ulItem = MCI_STATUS_POSITION;
   rsp = ProcStatus(pInst, MCI_STATUS_ITEM, &recStatus);

   if (ulParam1 & MCI_ENABLE_CONNECTOR)          // *** ENABLE DAC ***
   {
      if (pInst->ulMode & CDMC_CAN_DAC)
      {
         /* do it if not already playing internally */
         if (!(pInst->ulMode & CDMC_INTDAC))
            pInst->ulMode = pInst->ulMode & STREAM_MODE_SET | CDMC_INTDAC;

      }  /* of if can enable DAC */
      else
         rc = MCIERR_UNSUPPORTED_FLAG;           //no internal DAC
   }  /* of if enable */
   else                             // *** DISABLE DAC ***
   {
      if (pInst->ulMode & CDMC_INTDAC)
      {
         ProcStop(pInst, MCI_WAIT);
         pInst->ulMode ^= CDMC_INTDAC;
      }   /* of if playing internal DAC */
   }  /* of if disable */

   return(rc);

}  /* of SetConnector() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  SetCuePoint                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Set Cue Point                                         */
/*                                                                          */
/* FUNCTION:  Set Cue Point from the MCI_SET_CUEPOINT command.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- invalid cuepoint position.                     */
/*      MCIERR_DEVICE_NOT_READY -- No Disc is present.                      */
/*      MCIERR_CUEPOINT_LIMIT_REACHED --  no more room to add events.       */
/*      MCIERR_DUPLICATE_CUEPOINT     --  duplicate cuepoint.               */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

ULONG SetCuePoint(PINST pInst, MCI_CUEPOINT_PARMS *pParam2)
{
   ULONG rc, ulTime, ulFlags;
   int i, found = -1;
   MCI_CUEPOINT_PARMS recCuePoint;

   /* validate time */
   ulTime = GetTimeAddr(pInst, pParam2->ulCuepoint, TRUE);
   if (ulTime == (ULONG) -1L)
      rc = MCIERR_OUTOFRANGE;         // invalid track number
   else
   {
      ulTime += pInst->ulOffset;
      if (ulTime < pInst->ulStart_disk ||
          ulTime > pInst->ulEnd_disk)
         rc = MCIERR_OUTOFRANGE;
      else
         rc = MCIERR_SUCCESS;
   }

   /* make sure cuepoint is unique */
   if (!rc)
   {
      for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
      {
         if (pInst->arrCuePoint[i] == (ULONG) -1L)
         {
            if (found < 0)
               found = i;      // save first available entry
         }
         else
            if (pInst->arrCuePoint[i] == ulTime)
            {
               rc = MCIERR_DUPLICATE_CUEPOINT;
               break;               // error end for loop
            }
      }  /* of for loop */

      if (!rc)
         if (found < 0)
            rc = MCIERR_CUEPOINT_LIMIT_REACHED;
   }  /* of if no error, test uniqueness */

   /* init record and relay message */
   if (!rc)
   {
      pInst->arrCuePoint[found] = ulTime;
      /* copy record so as not to change original from application */
      memcpy(&recCuePoint, pParam2, sizeof(MCI_CUEPOINT_PARMS));
      pParam2->ulCuepoint = ulTime;
      ulFlags = MCI_SET_CUEPOINT_ON | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_CUEPOINT,
                             &ulFlags, pParam2, 0);

      if (rc)                            // clear entry
         pInst->arrCuePoint[found] = -1L;

   }  /* of if no error, init record */

   return(rc);

}  /* of SetCuePoint() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  StatusMCD                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Status MCD                                            */
/*                                                                          */
/* FUNCTION:  Query the Status of MCD flags.  This function is called       */
/*            by ProcStatus().                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst    -- pointer to instance.                             */
/*      ULONG  ulParam1 -- flag for this message.                           */
/*      PVOID  pParam2  -- pointer to structure (message dependent).        */
/*      ULONG  rsp      -- state response.                                  */
/*      USHORT usIgnore -- ignore invalid media type.                       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- invalid track supplied.                        */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Flags not compatible.                */
/*      MCIERR_UNSUPPORTED_FUNCTION -- Unknown flag or value used.          */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES: ulParam1 = MCI_STATUS_ITEM or MCI_STATUS_ITEM | MCI_TRACK         */
/*                                   or MCI_STATUS_ITEM | MCI_STATUS_START  */
/*                                                                          */
/****************************************************************************/

ULONG StatusMCD(PINST pInst, ULONG ulParam1, MCI_STATUS_PARMS *pParam2,
                ULONG rsp, USHORT usIgnore)
{
   ULONG rc;
   ULONG ulAddr, ulTrack, ulFlags;
   MCI_CD_REGTRACK_REC *ptr1, *ptr2;
   MCI_STATUS_PARMS recStatus;
   ULONG ulMode;

   /* Make sure disc is present before you rely on cache */
   if (rsp)
   {
      if (rsp == MCIERR_INSTANCE_INACTIVE)
         if (pInst->usStatus - SUSPEND == REGTRACK)
            rc = MCIERR_INVALID_MEDIA_TYPE;
         else if (pInst->usStatus - SUSPEND <= NODISC)
            rc = MCIERR_DEVICE_NOT_READY;
         else
         {
            rc = MCIERR_SUCCESS;    // process inactive command
            recStatus.ulReturn = pInst->ulCur_pos;
         }
      else
         rc = rsp;
   }  /* of if known error */
   else
   {
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
      {
         rc = vsdResponse(pInst, rc);
         if (!rc)                    //Disc Change, get starting address
            recStatus.ulReturn = pInst->ulStart_disk;
      }  /* of if error */

      if (!ULONG_LOWD(rc))
         pInst->ulCur_pos = recStatus.ulReturn;    // valid value, keep it
   }  /* of else no error so check disc */

   if (rc == MCIERR_INVALID_MEDIA_TYPE && usIgnore)
      rc = MCIERR_SUCCESS;

   if (ULONG_LOWD(rc))
      return(rc);

   /*----------- End of checking presents of Disc -----------------*/

   switch (ulParam1)
   {
      case MCI_STATUS_ITEM | MCI_TRACK :
         rc = StatusMCDDef(pInst, ulParam1, pParam2, recStatus.ulReturn);
         break;
      case MCI_STATUS_ITEM | MCI_STATUS_START :
         /* item must be MCI_STATUS_POSITION from ProcStatus() */
         pParam2->ulReturn =  GetTimeAddr(pInst, pInst->ulStart_disk, FALSE);
         rc = GetTimeAddrRC(pInst, rc);
         break;
      default :                            // must be MCI_STATUS_ITEM alone
         switch(pParam2->ulItem)
         {
            case MCI_STATUS_POSITION :
               pParam2->ulReturn =  GetTimeAddr(pInst,
                         (ULONG)(recStatus.ulReturn - pInst->ulOffset), FALSE);
               rc = GetTimeAddrRC(pInst, rc);
               break;
            case MCI_STATUS_LENGTH :
               ulTrack = pInst->recTrack.ulBufSize /
                         sizeof(MCI_CD_REGTRACK_REC);
               ulAddr = (pInst->recTrack.TrackRecArr + ulTrack-1)->ulEndAddr -
                         pInst->recTrack.TrackRecArr->ulStartAddr;
               /* return MSF format for length if in TMSF format */
               if ((pInst->ulMode & TIME_MODE) == CDMC_TMSF)
               {
                  ulMode = pInst->ulMode;
                  pInst->ulMode = pInst->ulMode & TIME_MODE_SET | CDMC_REDBOOK;
                  pParam2->ulReturn = GetTimeAddr(pInst, ulAddr, FALSE);
                  rc = GetTimeAddrRC(pInst, rc);
                  pInst->ulMode = ulMode;
               }
               else
               {
                  pParam2->ulReturn = GetTimeAddr(pInst, ulAddr, FALSE);
                  rc = GetTimeAddrRC(pInst, rc);
               }
               break;
            case MCI_STATUS_NUMBER_OF_TRACKS :
               ptr1 = GetTrackInfo(pInst, 0, pInst->ulStart_disk);
               ptr2 = GetTrackInfo(pInst, 0, pInst->ulEnd_disk - 1);
               if (ptr1 && ptr2)         //if both pointers are valid
                  pParam2->ulReturn = ptr2->TrackNum - ptr1->TrackNum + 1;
               else
                  pParam2->ulReturn = 0L;
               ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
               break;
            default :
               rc = StatusMCDDef(pInst, ulParam1, pParam2, recStatus.ulReturn);

         }  /* of switch, known items */
   }  /* of switch, flags */

   return(rc);

}  /* of StatusMCD() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  StatusMCDDef                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Status MCD Default                                    */
/*                                                                          */
/* FUNCTION:  Query the Status of MCD default flags.  This function is      */
/*            called by ProcStatus().                                       */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*      ULONG ulAddr   -- current address.                                  */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- invalid track supplied.                        */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Flags not compatible.                */
/*      MCIERR_UNSUPPORTED_FUNCTION -- Unknown flag or value used.          */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES: ulParam1 = MCI_STATUS_ITEM or MCI_STATUS_ITEM | MCI_TRACK         */
/*                                   or MCI_STATUS_ITEM | MCI_STATUS_START  */
/*                                                                          */
/****************************************************************************/

ULONG StatusMCDDef(PINST pInst, ULONG ulParam1,
                   MCI_STATUS_PARMS *pParam2, ULONG ulAddr)
{
   ULONG rc = MCIERR_SUCCESS;
   ULONG ulTrack;
   ULONG ulMode;
   MCI_CD_REGTRACK_REC *ptr1;


   /****************** get track number and info ********************/

   if (ulParam1 & MCI_TRACK)
   {
      ulTrack = pParam2->ulValue;
      ptr1 = GetTrackInfo(pInst, (BYTE) ulTrack, 0);
      if (ptr1 == NULL)
         rc = MCIERR_OUTOFRANGE;
   }
   else   /* get current track */
   {
      ptr1 = GetTrackInfo(pInst, 0, ulAddr);   //valid addr from drive
      ulTrack = ptr1->TrackNum;
   }  /* of else get current track */

   if (!ULONG_LOWD(rc))
      switch(pParam2->ulItem)
      {
         case MCI_STATUS_NUMBER_OF_TRACKS : // with MCI_TRACK, w/o already done
            rc = MCIERR_FLAGS_NOT_COMPATIBLE;
         case MCI_STATUS_POSITION :     // with MCI_TRACK, w/o already done
            pParam2->ulReturn = GetTimeAddr(pInst,
                 (ULONG)(ptr1->ulStartAddr - pInst->ulOffset), FALSE);
            rc = GetTimeAddrRC(pInst, rc);
            break;
         case MCI_STATUS_POSITION_IN_TRACK :
            if (ulParam1 & MCI_TRACK)
               rc = MCIERR_FLAGS_NOT_COMPATIBLE;
            else
            {
               ulAddr -= ptr1->ulStartAddr + pInst->ulOffset;
               pParam2->ulReturn = GetTimeAddr(pInst, ulAddr, FALSE);
               rc = GetTimeAddrRC(pInst, rc);
            }
            break;
         case MCI_STATUS_LENGTH :       // with MCI_TRACK, w/o already done
            ulAddr = ptr1->ulEndAddr - ptr1->ulStartAddr;
            /* return MSF format for length if in TMSF format */
            if ((pInst->ulMode & TIME_MODE) == CDMC_TMSF)
            {
               ulMode = pInst->ulMode;
               pInst->ulMode = (pInst->ulMode & TIME_MODE_SET) | CDMC_REDBOOK;
               pParam2->ulReturn = GetTimeAddr(pInst, ulAddr, FALSE);
               rc = GetTimeAddrRC(pInst, rc);
               pInst->ulMode = ulMode;
            }
            else
            {
               pParam2->ulReturn = GetTimeAddr(pInst, ulAddr, FALSE);
               rc = GetTimeAddrRC(pInst, rc);
            }
            break;
         case MCI_STATUS_CURRENT_TRACK :
            if (ulParam1 & MCI_TRACK)
               rc = MCIERR_FLAGS_NOT_COMPATIBLE;
            else
            {
               pParam2->ulReturn = ulTrack;
               ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
            }
            break;
         case MCI_CD_STATUS_TRACK_TYPE :
            if (!(ptr1->TrackControl & IS_DATA_TRK))
               pParam2->ulReturn = MCI_CD_TRACK_AUDIO;
            else if (ptr1->TrackControl & IS_OTHER_TRK)
               pParam2->ulReturn = MCI_CD_TRACK_OTHER;
            else
               pParam2->ulReturn = MCI_CD_TRACK_DATA;
            ULONG_HIWD(rc) = MCI_TRACK_TYPE_RETURN;
            break;
         case MCI_CD_STATUS_TRACK_COPYPERMITTED :
            if (ptr1->TrackControl & IS_COPYABLE)
               pParam2->ulReturn = MCI_TRUE;
            else
               pParam2->ulReturn = MCI_FALSE;
            ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
            break;
         case MCI_CD_STATUS_TRACK_CHANNELS :
            if (!(ptr1->TrackControl & IS_DATA_TRK))      /* Is Audio ? */
               if (ptr1->TrackControl & HAS_4_CHANS)
                  pParam2->ulReturn = 4L;
               else
                  pParam2->ulReturn = 2L;
            else                                          /* Data */
               pParam2->ulReturn = 0L;
            ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
            break;
         case MCI_CD_STATUS_TRACK_PREEMPHASIS :
            if (!(ptr1->TrackControl & IS_DATA_TRK))      /* Is Audio ? */
               if (ptr1->TrackControl & IS_PRE_EMPH)
                  pParam2->ulReturn = MCI_TRUE;
               else
                  pParam2->ulReturn = MCI_FALSE;
            else                                          /* Data */
               pParam2->ulReturn = MCI_FALSE;
            ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
            break;
         /* no default value, all values were filtered in procStatus() */
      }  /* of switch, track info needed */

   return(rc);

}  /* of StatusMCDDef() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  StatusVSD                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Status VSD                                            */
/*                                                                          */
/* FUNCTION:  Query the Status of VSD only flags.  This function is called  */
/*            by ProcStatus().                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      ULONG ulParam1 -- flag for this message.                            */
/*      PVOID pParam2  -- pointer to structure (message dependent).         */
/*      ULONG rc       -- initial state, inactive, not ready, etc.          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES: ulParam1 = MCI_STATUS_ITEM or MCI_STATUS_ITEM | MCI_TRACK         */
/*                                                                          */
/****************************************************************************/

ULONG StatusVSD(PINST pInst, ULONG ulParam1, MCI_STATUS_PARMS *pParam2,
                ULONG rc)
{
   ULONG ulVol;
   USHORT usStatus;
   ULONG ulFlags, rsp = 0L;
   MCI_STATUS_PARMS recStatus;

   if (!rc)
   {
      /* Query position to release any Disc Changes in VSD */
      recStatus.ulItem = MCI_STATUS_POSITION;
      ulFlags = MCI_STATUS_ITEM | MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS,
                             &ulFlags, &recStatus, 0);
      if (ULONG_LOWD(rc))
         rc = vsdResponse(pInst, rc);
   }  /* of if no error */

   if (rc == MCIERR_INVALID_MEDIA_TYPE)
   {
      rsp = MCIERR_INVALID_MEDIA_TYPE;
      rc = MCIERR_SUCCESS;
   }

   if (!rc)
   {
      ulParam1 |= MCI_WAIT;
      rc = pInst->pMCIDriver(pInst->hHWMCID, MCI_STATUS, &ulParam1,
                             pParam2, 0);
      if (ULONG_LOWD(rc))
         rc = vsdResponse(pInst, rc);       //if error, process response
   }  /* of if no error */

   if (!(ulParam1 & MCI_TRACK))          //process known combinations flags
      switch(pParam2->ulItem)
      {
         case MCI_STATUS_MEDIA_PRESENT :
            switch ULONG_LOWD(rc)
            {
               case MCIERR_SUCCESS :                //vsd already set ulReturn
                  ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
                  if (pParam2->ulReturn == MCI_FALSE)
                     pInst->usStatus = NODISC;
                  else if (rsp)
                     rc = rsp;                     //report invalid media
                  break;
               case MCIERR_INSTANCE_INACTIVE :
                  rc = MCIERR_SUCCESS;
                  ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
                  usStatus = (USHORT)(pInst->usStatus - SUSPEND);
                  if (usStatus == REGTRACK)
                     rc = MCIERR_INVALID_MEDIA_TYPE;
                  else if (usStatus <= NODISC)
                     pParam2->ulReturn = MCI_FALSE;
                  else
                     pParam2->ulReturn = MCI_TRUE;
                  break;
               default :                            //return not present
                  rc = MCIERR_SUCCESS;
                  ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
                  pParam2->ulReturn = MCI_FALSE;
            }  /* of switch */
            ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
            break;
         case MCI_STATUS_MODE :
            if (ULONG_LOWD(rc))
            {
               pParam2->ulReturn = MCI_MODE_NOT_READY;
               rc = MCIERR_SUCCESS;
            }
            else
               if (pParam2->ulReturn == MCI_MODE_STOP)
               {
                  if (pInst->usStatus == PAUSED)
                     pParam2->ulReturn = MCI_MODE_PAUSE;
               }
               else
                  if (pParam2->ulReturn == MCI_MODE_NOT_READY)
                     pInst->usStatus = NODISC;

            ULONG_HIWD(rc) = MCI_MODE_RETURN;
            break;
         case MCI_STATUS_READY :
            if (ULONG_LOWD(rc))
            {
               pParam2->ulReturn = MCI_FALSE;
               rc = MCIERR_SUCCESS;
            }
            else
               if (pParam2->ulReturn == MCI_FALSE)
                  pInst->usStatus = NODISC;

            ULONG_HIWD(rc) = MCI_TRUE_FALSE_RETURN;
            break;
         case MCI_STATUS_VOLUME :
            if (ULONG_LOWD(rc))          // if error
               rsp = TRUE;
            else
            {
               rsp = FALSE;
               ULONG_HIWD(rc) = MCI_COLONIZED2_RETURN;

               /* adjust device volume to component volume */
               if (pInst->ulMasterVolume && pInst->ulMode & CDMC_HEADPHONE)
               {
                  ulVol = (ULONG)VOL_LEFT(pParam2->ulReturn) *
                        100 / pInst->ulMasterVolume;
                  if (ulVol > 100)
                     VOL_LEFT(pParam2->ulReturn) = 100;
                  else
                     VOL_LEFT(pParam2->ulReturn) = (BYTE)ulVol;

                  ulVol = (ULONG)VOL_RIGHT(pParam2->ulReturn) *
                           100 / pInst->ulMasterVolume;
                  if (ulVol > 100)
                     VOL_RIGHT(pParam2->ulReturn) = 100;
                  else
                     VOL_RIGHT(pParam2->ulReturn) = (BYTE)ulVol;

               }  /* of if non-zero */
               else
                  /* master volume is 0%, return requested volume */
                  rsp = TRUE;

            }  /* of else no error */

            if (rsp)  //unknown volume, ask VSD what component volume should be
            {
               if (rc == MCIERR_INSTANCE_INACTIVE)
                  pParam2->ulReturn = pInst->recSave.ulLevel;
               else
               {
                  ulFlags = MCI_WAIT;
                  recStatus.ulReturn = pInst->ulLevel;
                  pInst->pMCIDriver(pInst->hHWMCID, MCIDRV_CD_STATUS_CVOL,
                                    &ulFlags, &recStatus, 0);
                  pParam2->ulReturn = recStatus.ulReturn;
               }
               rc = MAKEULONG(MCIERR_SUCCESS, MCI_COLONIZED2_RETURN);

            }  /* of if unknown volume */

            break;
      }  /* of switch */

   return(rc);

}  /* of StatusVSD() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  DisableEvents             DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Disable Events                                        */
/*                                                                          */
/* FUNCTION:  Disable cuepoint and position advise events from the VSD.     */
/*            This is a requirement when the disc is changed.               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

VOID DisableEvents(PINST pInst)
{
   int i;
   ULONG ulParam1;
   MCI_CUEPOINT_PARMS recCuePoint;

   /* disable cuepoints */
   ulParam1 = MCI_SET_CUEPOINT_OFF | MCI_WAIT;
   for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
   {
      if (pInst->arrCuePoint[i] != -1L)
      {
         recCuePoint.ulCuepoint = pInst->arrCuePoint[i];
         pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_CUEPOINT,
                           &ulParam1, &recCuePoint, 0);
         pInst->arrCuePoint[i] = (ULONG) -1L;
      }  /* of if cuepoint is set */
   }  /* of for loop of cuepoints */

   /* disable position advise */
   ulParam1 = MCI_SET_POSITION_ADVISE_OFF | MCI_WAIT;
   pInst->pMCIDriver(pInst->hHWMCID, MCI_SET_POSITION_ADVISE,
                     &ulParam1, 0L, 0);

}  /* of DisableEvents() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetTimeAddr                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get Time Address                                      */
/*                                                                          */
/* FUNCTION:  Convert to and from MMTime and other time formats.            */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst   -- pointer to instance.                              */
/*      ULONG  ulAddr  -- input address.                                    */
/*      USHORT usTO    -- TRUE if to MMTime, FALSE if from MMTime.          */
/*                                                                          */
/* EXIT CODES:  ULONG  ulReturn -- the converted address.                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG GetTimeAddr(PINST pInst, ULONG ulAddr, USHORT usTO)
{
   ULONG ulReturn;
   MCI_CD_REGTRACK_REC *ptr;

   switch (pInst->ulMode & TIME_MODE)
   {
      case CDMC_MILLSEC :                          // milliseconds
         if (usTO)
            ulReturn = MSECTOMM(ulAddr);
         else
            ulReturn = MSECFROMMM(ulAddr);
         break;
      case CDMC_REDBOOK :                          // Red Book / MSF
         if (usTO)
            ulReturn = REDBOOKTOMM(ulAddr);
         else
            ulReturn = REDBOOKFROMMM(ulAddr);
         break;
      case CDMC_TMSF :                             // Track:Min:Sec:Frame
         if (usTO)
         {
            ptr = GetTrackInfo(pInst, TMSF_TRACK(ulAddr), 0L);
            if (ptr == NULL)
               ulReturn = (ULONG) -1L;            // invalid track number
            else
            {
               ulAddr = (ulAddr >> 8) & 0x00FFFFFF;     //TMSF -> MSF
               ulReturn = ptr->ulStartAddr + REDBOOKTOMM(ulAddr);
            }
         }  /* of TO MMTIME */
         else
         {
            ptr = GetTrackInfo(pInst, 0, ulAddr);
            if (ptr == NULL)
               ulReturn = (ULONG) -1L;            // invalid track number
            else
            {
               ulAddr -= ptr->ulStartAddr;
               ulReturn = (REDBOOKFROMMM(ulAddr) << 8) | (ULONG) ptr->TrackNum;
            }
         }  /* of else FROM MMTIME */
         break;
      case CDMC_MMTIME :                          // Multimedia Time
      default :
         ulReturn = ulAddr;
   }  /* on switch */

   return(ulReturn);

}  /* of GetTimeAddr() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetTimeAddrRC                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get Time Address Return Code                          */
/*                                                                          */
/* FUNCTION:  Return correct time format in the colinized code,             */
/*            high word of the return code.                                 */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst   -- pointer to instance.                              */
/*      ULONG  ulAddr  -- input address.                                    */
/*      USHORT usTO    -- TRUE if to MMTime, FALSE if from MMTime.          */
/*                                                                          */
/* EXIT CODES:  ULONG  rc  -- the converted return code.                    */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG GetTimeAddrRC(PINST pInst, ULONG rc)
{
   switch (pInst->ulMode & TIME_MODE)
   {
      case CDMC_REDBOOK :                          // Red Book / MSF
         ULONG_HIWD(rc) = MCI_COLONIZED3_RETURN;
         break;
      case CDMC_TMSF :                             // Track:Min:Sec:Frame
         ULONG_HIWD(rc) = MCI_COLONIZED4_RETURN;
         break;
      case CDMC_MILLSEC :                          // milliseconds
      case CDMC_MMTIME :                           // Multimedia Time
      default :
         ULONG_HIWD(rc) = MCI_INTEGER_RETURNED;
   }  /* on switch */

   return(rc);

}  /* of GetTimeAddrRC() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetTrackInfo                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get Track Information                                 */
/*                                                                          */
/* FUNCTION:  Find the Track entry for the given track or absolute address. */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst    -- pointer to instance.                              */
/*      BYTE  Track    -- specified track number.                           */
/*      ULONG ulAddr   -- specified absolute address, ignored if ulTrack    */
/*                        is non-zero.                                      */
/*                                                                          */
/* EXIT CODES:  MCI_CD_REGTRACK_REC *pRet -- return pointer of track info,  */
/*                                           NULL if error (invalid input). */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

MCI_CD_REGTRACK_REC * GetTrackInfo(PINST pInst, BYTE Track, ULONG ulAddr)
{
   MCI_CD_REGTRACK_REC *pRet;
   USHORT i, usOffset;                  //Number of Tracks - 1

   if (Track)                                         // if a track was used
      if (pInst->recDisc.LowestTrackNum <= Track &&      // and is valid
          pInst->recDisc.HighestTrackNum >= Track)
         pRet = pInst->recTrack.TrackRecArr +            // get track info
                  (Track - pInst->recDisc.LowestTrackNum);
      else
         pRet = NULL;                                    // else error
   else                                                // otherwise an address
   {
      usOffset = pInst->recDisc.HighestTrackNum - pInst->recDisc.LowestTrackNum;

      /* check if address is in a valid range */
      if (pInst->recTrack.TrackRecArr->ulStartAddr <= ulAddr &&
          (pInst->recTrack.TrackRecArr + usOffset)->ulEndAddr >= ulAddr)
      {
         /* for every track, check address bounds until ulAddr is found */
         /* if not found it must be the last track since the addr is valid */
         for (i= 0; i < usOffset; i++)
            if ((pInst->recTrack.TrackRecArr + i)->ulStartAddr <= ulAddr &&
                (pInst->recTrack.TrackRecArr + i)->ulEndAddr > ulAddr)
               break;
         pRet = pInst->recTrack.TrackRecArr + i;

      }  /* of if a valid address */
      else
         pRet = NULL;                                    // else error
   }  /* of else an address is used */

   return(pRet);

}  /* of GetTrackInfo() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ValAddress                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Validate Address                                      */
/*                                                                          */
/* FUNCTION:  Validate Redbook Audio address in MMTIME format.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst    -- pointer to instance.                             */
/*      ULONG  *pusFrom -- address to be tested and returned.               */
/*      ULONG  *pusTo   -- address to be tested and returned.               */
/*      USHORT isPlay   -- type of check (TRUE = play, FALSE - seek).       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_OUTOFRANGE -- cannot play in reverse.                        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG ValAddress(PINST pInst, ULONG *pulFrom, ULONG *pulTo, USHORT isPlay)
{
   ULONG rc;

   rc = MCIERR_SUCCESS;

   /* validate TO */
   if (*pulTo < pInst->ulStart_disk)
      rc = MCIERR_OUTOFRANGE;
   else
      if (isPlay)
      {
         if (*pulTo > pInst->ulEnd_disk)
            rc = MCIERR_OUTOFRANGE;
      }
      else
         /* ulEnd_disk is really the starting frame or sector of the next */
         /* track, usually the Lead Out Track.  Valid seeks should be     */
         /* contained within playable tracks.                             */
         if (*pulTo > pInst->ulEnd_disk - MMT_FRAME)
            rc = MCIERR_OUTOFRANGE;

   /* Validate from address if needed */
   if (!rc && isPlay)
   {
      if (*pulFrom < pInst->ulStart_disk)
         rc = MCIERR_OUTOFRANGE;
      else if (*pulTo > pInst->ulEnd_disk)
         rc = MCIERR_OUTOFRANGE;

      /* validate direction */
      /* can only go forward if HW can't play in reverse or if streaming */
      if (*pulFrom > *pulTo &&
             (!(pInst->usCaps & CDVSD_CAP_CAN_REVERSE) ||
               (pInst->ulMode & CDMC_STREAM)))
         rc = MCIERR_OUTOFRANGE;

   }  /* of if validating FROM */

   return(rc);

}  /* of ValAddress() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  ValState                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Validate State                                        */
/*                                                                          */
/* FUNCTION:  Validate State of an Instance.  Most commands need to have    */
/*            a disc and not be suspended.                                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst     -- pointer to instance.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- action completed without error.                */
/*      MCIERR_DEVICE_NOT_READY -- No Disc is present.                      */
/*      MCIERR_INSTANCE_INACTIVE -- Instance is suspended.                  */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG ValState(PINST pInst)
{
   ULONG rc;

   /* if tracks were registered then we should be stopped, */
   /*    unless no audio tracks exists.                    */
   if (pInst->usStatus == REGTRACK)
      rc = MCIERR_INVALID_MEDIA_TYPE;
   else if (pInst->usStatus <= NODISC)
      rc = MCIERR_DEVICE_NOT_READY;
   else if (pInst->usStatus >= SUSPEND)
      rc = MCIERR_INSTANCE_INACTIVE;
   else
      rc = MCIERR_SUCCESS;

   return(rc);

}  /* of ValState() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  vsdResponse                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  Vendor Supplied Driver Response                       */
/*                                                                          */
/* FUNCTION:  Process the response from a VSD.                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- pointer to instance.                            */
/*      ULONG ulResponse -- received response.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_DEVICE_NOT_READY   -- device was not ready, no disc.         */
/*      MCIERR_INVALID_MEDIA_TYPE -- No audio tracks were found.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

ULONG vsdResponse(PINST pInst, ULONG ulResponse)
{
   ULONG rc;

   switch (ULONG_LOWD(ulResponse))
   {
      case MCIERR_DEVICE_NOT_READY :
         pInst->usStatus = NODISC;
         rc = ulResponse;
         break;
      case MCIERR_MEDIA_CHANGED :
         rc = ReRegister(pInst);
         break;
      default :
         rc = ulResponse;
   }   /* of switch */

   return(rc);

}  /* of vsdResponse() */



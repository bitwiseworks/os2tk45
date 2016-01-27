/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  IBMCDPRO.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  IBM CD-ROMs PROCESS                                   */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1990 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the functions that require hardware        */
/*            processing.                                                   */
/*                                                                          */
/*                                                                          */
/* OTHER FUNCTIONS:                                                         */
/*       CD01_Cue           - Preroll a drive.                              */
/*       CD01_CuePoint      - Set up a cue point.                           */
/*       CD01_GetCaps       - Get device capabilities.                      */
/*       CD01_GetDiscInfo   - Get status info of the disc.                  */
/*       CD01_GetID         - Get the CD ID from the disc.                  */
/*       CD01_GetPosition   - Get the position of the head.                 */
/*       CD01_GetState      - Get the state of the device.                  */
/*       CD01_GetTOC        - Returns table of contents (MMTOC form) of CD. */
/*       CD01_GetVolume     - Get the volume settings of the drive.         */
/*       CD01_LockDoor      - Lock/Unlock the drive door.                   */
/*       CD01_Open          - Open specified device/drive.                  */
/*       CD01_Play          - Initiate a play operation.                    */
/*       CD01_PlayCont      - Continue a play operation.                    */
/*       CD01_PosAdvise     - Set up a position advise command.             */
/*       CD01_RegTracks     - Register tracks on the disc.                  */
/*       CD01_Restore       - Restore the saved instance.                   */
/*       CD01_Resume        - Unpause a CD Play operation.                  */
/*       CD01_Save          - Save the current instance.                    */
/*       CD01_Seek          - Seek to a particular redbook address.         */
/*       CD01_SetVolume     - Set the volume of the drive.                  */
/*       CD01_Stop          - Stop a CD Play operation.                     */
/*                                                                          */
/*       NOTE:  CD01_... refers to commands that are compatible with the    */
/*              IBM 3510 CD-ROM drive.  CD02_... may refer to commands      */
/*              that are compatible with the CD02 drives, which are         */
/*              not compatible with the CD01_... commands.  This way        */
/*              different hardware can share the same VSD.                  */
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


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Cue                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Cue.                                               */
/*                                                                          */
/* FUNCTION:  Cue up or preroll the drive.  To do this we seek to the       */
/*            current position.                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Cue(PINST pInst)
{
   ULONG rc, ulPos;

   /* test is disk is still present and get current position */
   rc = CD01_GetPosition(pInst, &ulPos);

   /* seek to current position to spin disc */
   if (!rc)                              // if no error
      rc = CD01_Seek(pInst, ulPos);

   return(rc);

}  /* of CD01_Cue() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_CuePoint                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Cue Point                                          */
/*                                                                          */
/* FUNCTION:  Set up the desired cuepoint.  To do this the cue point        */
/*            arrays will need to be updated.                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulParam1   -- Flag set for this message.                      */
/*      MCI_CUEPOINT_PARMS *pParam2 -- Pointer to data record structure.    */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_CUEPOINT_LIMIT_REACHED -- no more room to add events.        */
/*      MCIERR_INVALID_CUEPOINT       -- unable to locate event.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_CuePoint(PINST pInst, ULONG ulParam1, MCI_CUEPOINT_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;
   int i;

   if (ulParam1 & MCI_SET_CUEPOINT_ON)
   {
      /* valid cuepoint, find first available entry,  */
      /* CD Audio MCD will make sure that we have room  */
      /* and that cuepoint is unique */
      for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
         if (pInst->arrCuePoint[i].ulEvent == (ULONG) -1L)
            break;

      if (i == CDMCD_CUEPOINT_MAX)
         rc = MCIERR_CUEPOINT_LIMIT_REACHED;
      else
      {
         pInst->arrCuePoint[i].ulEvent = pParam2->ulCuepoint;
         pInst->arrCuePoint[i].hwndCallback = pParam2->hwndCallback;
         pInst->arrCuePoint[i].usUserParm = pParam2->usUserParm;
      }
   }  /* of if setting cuepoint */
   else
   {
      for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
      {
         if (pInst->arrCuePoint[i].ulEvent == pParam2->ulCuepoint)
            break;
      }
      if (i == CDMCD_CUEPOINT_MAX)
         rc = MCIERR_INVALID_CUEPOINT;
      else
         pInst->arrCuePoint[i].ulEvent = (ULONG) -1L;

   }   /* of else MCI_SET_CUEPOINT_OFF */

   return(rc);

}  /* of CD01_CuePoint() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetCaps                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Capabilities.                                  */
/*                                                                          */
/* FUNCTION:  Get information about the capabilities of the device and the  */
/*            VSD.                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      ULONG ulParam1   -- Flag set for this message.                      */
/*      MCI_GETDEVCAPS_PARMS *pParam2   -- Pointer to data record structure.*/
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- Mis-match in flags.                 */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*      MCIERR_INVALID_ITEM_FLAG     -- Unknown item specified.             */
/*                                                                          */
/* NOTES:  Most of the capabilities are processed in the general MCI Driver.*/
/*      The messages supported need to be listed in the VSD because VSDs    */
/*      may support different messages.                                     */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetCaps(ULONG ulParam1, MCI_GETDEVCAPS_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;

   ulParam1 &= WAIT_NOTIFY_MASK;

   switch (ulParam1)
   {
      case MCI_GETDEVCAPS_MESSAGE :
         switch (pParam2->usMessage)
         {
            case MCI_CLOSE :                  case MCI_CUE :
            case MCI_GETDEVCAPS :             case MCI_GETTOC :
            case MCI_INFO :                   case MCI_OPEN :
            case MCI_PAUSE :                  case MCI_PLAY :
                                              case MCI_RESUME :
            case MCI_SEEK :                   case MCI_SET :
            case MCI_SET_CUEPOINT :           case MCI_SET_POSITION_ADVISE :
            case MCI_STATUS :                 case MCI_STOP :
            case MCIDRV_SAVE :                case MCIDRV_RESTORE :
            case MCIDRV_SYNC :
            case MCIDRV_REGISTER_DISC :       case MCIDRV_REGISTER_DRIVE :
            case MCIDRV_REGISTER_TRACKS :
            case MCIDRV_CD_SET_VERIFY :       case MCIDRV_CD_STATUS_CVOL :
               pParam2->ulReturn = MCI_TRUE;
               break;
            default :
               pParam2->ulReturn = MCI_FALSE;
         }  /* of switch */
         break;
      case MCI_GETDEVCAPS_ITEM :            // MCD does all that VSD supports
         rc = MCIERR_INVALID_ITEM_FLAG;
         break;
      default :
         rc = MCIERR_INVALID_FLAG;
         break;
   }   /* of switch */

   return(rc);

}  /* of CD01_GetCaps() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetDiscInfo                                       */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Disc Information                               */
/*                                                                          */
/* FUNCTION:  Get information contained on the compact disc.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulType     -- Type flag for this message.                     */
/*      MCI_STATUS_PARMS *pParam2   -- Pointer to data record structure.    */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_FLAGS_NOT_COMPATIBLE  -- Mis-match in flags.                 */
/*                                                                          */
/* NOTES:  The function supports the MCI_STATUS_MEDIA_PRESENT,              */
/*      MCI_STATUS_MODE and MCI_STATUS_READY flags for the MCI_STATUS       */
/*      command message.                                                    */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetDiscInfo(PINST pInst, ULONG ulType, MCI_STATUS_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;
   BYTE data[SEEKSTATDMAX];
   ULONG ulDataLen = SEEKSTATDMAX, ulParamLen = STANDRD_PMAX;

   switch(ulType)
   {
      case MCI_STATUS_MEDIA_PRESENT :
         if (CallIOCtl(pInst, CDDRIVE_CAT, DEV__STATUS,
                       "CD01", ulParamLen, &ulParamLen,
                       data,   ulDataLen,  &ulDataLen))
            pParam2->ulReturn = MCI_FALSE;
         else
            if (data[STATAUDFLD] & NO_MEDIA)     // if No disc is present
               pParam2->ulReturn = MCI_FALSE;
            else
               pParam2->ulReturn = MCI_TRUE;
         break;
      case MCI_STATUS_MODE :
         CD01_GetState(pInst, pParam2);
         break;
      case MCI_STATUS_READY :
         CD01_GetState(pInst, pParam2);
         if (pParam2->ulReturn == MCI_MODE_NOT_READY)
            pParam2->ulReturn = MCI_FALSE;
         else
            pParam2->ulReturn = MCI_TRUE;
         break;
      default : rc = MCIERR_FLAGS_NOT_COMPATIBLE;

   }  /* of switch */

   return(rc);

}  /* of CD01_GetDiscInfo() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetID                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Disc ID.                                       */
/*                                                                          */
/* FUNCTION:  Get the Disc ID.  The disc ID is 8 bytes and format is:       */
/*            ÚÄÄÄÄÄÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿                     */
/*            ³   01   ³Addr of³Num of³Address of the ³                     */
/*            ³UPC = 00³Track 1³Tracks³Lead Out Track ³                     */
/*            ÀÄÄÄÄÄÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙ                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      MCI_CD_DISC_ID *pDiscID  -- Pointer to data record structure.       */
/*      BYTE  *LowTrack  -- Lowest track number.                            */
/*      BYTE  *HighTrack -- Highest track number.                           */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetID(PINST pInst, MCI_CD_ID *pDiscID,
                 BYTE *LowTrack, BYTE *HighTrack)
{
   ULONG rc;
   BYTE data[DSKTRCK_DMAX], param[DSKTRCK_PMAX] = "CD01";
   ULONG ulDataLen = DSKTRCK_DMAX, ulParamLen = STANDRD_PMAX;

   /* set mode */
   pDiscID->Mode = 1;

   /* get rest of data */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, DISK___INFO,
                  "CD01", ulParamLen, &ulParamLen,
                  data,   ulDataLen,  &ulDataLen);

   if (!rc)
   {
      /* set number of tracks */
      pDiscID->NumTracks = data[TRCKHI_FLD];
      *HighTrack = data[TRCKHI_FLD];

      /* set leadout track */
      pDiscID->ulLeadOut = REDBOOK2TOMM(*(ULONG *)&data[TRCKENDADR]);

      /* set first track */
      param[TRACKFIELD] = data[TRCKLOWFLD];
      *LowTrack = data[TRCKLOWFLD];

      /* get track information to get address of the first track */
      ulDataLen = DSKTRCK_DMAX;
      ulParamLen = DSKTRCK_PMAX;
      rc = CallIOCtl(pInst, CDAUDIO_CAT, TRACK__INFO,
                     param, ulParamLen, &ulParamLen,
                     data,  ulDataLen,  &ulDataLen);

      if (!rc)
         pDiscID->usTrack1 = (USHORT) REDBOOK2TOMM(*(ULONG *)&data[TRACKFFFLD]);

   }   /* of if no error on IOCTL */

   return(rc);

}  /* of CD01_GetID() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetPosition                                       */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Position.                                      */
/*                                                                          */
/* FUNCTION:  Get the current position of the CD-ROM drive.                 */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst        -- Instance structure.                           */
/*      ULONG *pulPosition -- ptr of the current position.                  */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_MEDIA_CHANGED         -- A disc change was reported.         */
/*                                                                          */
/* NOTES:  It is important to return the above three return codes because   */
/*         MCDs and Applications may rely on MCI_STATUS POSITION to verify  */
/*         that the same disc is still there.                               */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetPosition(PINST pInst, ULONG *pulPosition)
{
   ULONG rc;
   BYTE data[LOCATON_DMAX], param[LOCATON_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen = LOCATON_DMAX, ulParamLen = LOCATON_PMAX;

   /* get location of head in disc */
   rc = CallIOCtl(pInst, CDDRIVE_CAT, Q__LOCATION,
                  param, ulParamLen, &ulParamLen,
                  data,  ulDataLen,  &ulDataLen);

   if (!rc)
      if (pInst->usPlayFlag == TIMER_PLAYING)
      {
         *pulPosition = REDBOOK2TOMM(*(ULONG *)data);
         pInst->ulCurPos = *pulPosition;
      }  /* of if playing */
      else           /* if not playing then use logical location */
         *pulPosition = pInst->ulCurPos;

   return(rc);

}  /* of CD01_GetPosition() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetState                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get State.                                         */
/*                                                                          */
/* FUNCTION:  Get the state (playing, stopped, paused, etc.) of the device. */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      MCI_STATUS_PARMS *pParam2   -- Pointer to data record structure.    */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetState(PINST pInst, MCI_STATUS_PARMS *pParam2)
{
   ULONG rc;
   BYTE data[AUDSTAT_DMAX];      //values for the audio status command
   ULONG ulDataLen = AUDSTAT_DMAX, ulParamLen = STANDRD_PMAX;

   /* query the status */
   rc = CallIOCtl(pInst, CDDRIVE_CAT, DEV__STATUS,
                  "CD01", ulParamLen, &ulParamLen,
                  data,   ulDataLen,  &ulDataLen);

   if (rc)    /* error, manual eject */
      pParam2->ulReturn = MCI_MODE_NOT_READY;
   else
   {
      if (data[STATAUDFLD] & NO_MEDIA)               // if no disc
         pParam2->ulReturn = MCI_MODE_NOT_READY;
      else if (data[STATAUDFLD] & IS_PLAYING)        // if Playing
         pParam2->ulReturn = MCI_MODE_PLAY;
      else
         pParam2->ulReturn = MCI_MODE_STOP;

   }  /* of else no IOCTL error */

   return(rc);

}  /* of CD01_GetState() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetTOC                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Table of Contents.                             */
/*                                                                          */
/* FUNCTION:  Get the Table of Contents of the playable portion of the CD.  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      MCI_TOC_PARMS *pParam2   -- Pointer to data record structure.       */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_INVALID_BUFFER        -- Buffer too small.                   */
/*      MCIERR_INVALID_MEDIA_TYPE    -- No audio tracks were found.         */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetTOC(PINST pInst, MCI_TOC_PARMS *pParam2)
{
   ULONG rc;
   BYTE data[DSKTRCK_DMAX], param[DSKTRCK_PMAX] = "CD01";
   BYTE low, hi;             // low and high track values
   BYTE cnt = 0, check = 0;  // audio track counters
   ULONG end_adr, cur_adr;
   ULONG ulDataLen = DSKTRCK_DMAX, ulParamLen = DSKTRCK_PMAX;

   /*** GET DISK INFORMATION ***/
   /* get disk information */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, DISK___INFO,
                  param, ulParamLen, &ulParamLen,
                  data,  ulDataLen,  &ulDataLen);

   if (!rc)
      if (pParam2->ulBufSize < sizeof(MCI_TOC_REC))
         rc = MCIERR_INVALID_BUFFER;

   low = data[TRCKLOWFLD];
   hi  = data[TRCKHI_FLD];

   if (!rc)
   {                                            // okay, extract data
      /* get ending address */
      end_adr = REDBOOK2TOMM(*(ULONG *)&data[TRENDFFFLD]);

      /*** LOOP TO FILL TABLE ***/
      /******************************************************************
       *  CNT is a counter of audio tracks, LOW starts with the lowest  *
       *  track number and increments with each track, CHECK is a flag  *
       *  marking the previous audio track so that the ending address   *
       *  may be assigned.                                              *
       ******************************************************************/

      for (; low <= hi; low++)
      {
         param[TRACKFIELD] = low;

         ulDataLen = DSKTRCK_DMAX;
         ulParamLen = DSKTRCK_PMAX;
         rc = CallIOCtl(pInst, CDAUDIO_CAT, TRACK__INFO,
                        param, ulParamLen, &ulParamLen,
                        data,  ulDataLen,  &ulDataLen);

         if (rc)
            break;

         cur_adr = REDBOOK2TOMM(*(ULONG *)&data[TRACKFFFLD]);

         /* Enter address as ending address of previous track */
         if (cnt != check)                     // skip first and data tracks
         {                                     // add ending address
            (pParam2->pBuf + cnt-1)->ulEndAddr = cur_adr;
            check = cnt;
         }

         if (!(data[TRKSTATFLD] & IS_DATA_TRK))         // if audio track
         {  /* audio track has been found, is there room for it in buffer */
            /* check for valid buffer size */
            if (pParam2->ulBufSize < (ULONG)(cnt + 1) * sizeof(MCI_TOC_REC))
            {
               rc = MCIERR_INVALID_BUFFER;
               break;
            }

            (pParam2->pBuf + cnt)->TrackNum = low;            // track number
            (pParam2->pBuf + cnt)->ulStartAddr = cur_adr;     // add start loc
            (pParam2->pBuf + cnt)->Control = data[TRKSTATFLD];   // Control
            (pParam2->pBuf + cnt)->usCountry = 0;                // Country
            (pParam2->pBuf + cnt)->ulOwner = 0L;                 // Owner
            (pParam2->pBuf + cnt)->ulSerialNum = 0L;             // S/N

            if (low == hi)                       // last addr if last track
               (pParam2->pBuf + cnt)->ulEndAddr = end_adr;
            cnt++;

         }  /* of if not data track */

      }   /* of for loop */

      if (!cnt)
         rc = MCIERR_INVALID_MEDIA_TYPE;     // No audio tracks were found.

   }   /* of if no error getting audio disc information */

   /* find needed buffer size */
   if (rc == MCIERR_INVALID_BUFFER)
   {
      for (; low <= hi; low++)
      {
         param[TRACKFIELD] = low;

         ulDataLen = DSKTRCK_DMAX;
         ulParamLen = DSKTRCK_PMAX;
         if (CallIOCtl(pInst, CDAUDIO_CAT, TRACK__INFO,
                       param, ulParamLen, &ulParamLen,
                       data,  ulDataLen,  &ulDataLen))
         {
            rc = MCIERR_DEVICE_NOT_READY;
            break;
         }

         if (!(data[TRKSTATFLD] & IS_DATA_TRK))         // if audio track
            cnt++;

      }   /* of for loop */

   }  /* of if buffer too small */

   /* return buffer size */
   pParam2->ulBufSize = sizeof(MCI_TOC_REC) * cnt;

   return(rc);

}  /* of CD01_GetTOC() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_GetVolume                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Get Volume for the IBM 3510.                       */
/*                                                                          */
/* FUNCTION:  Get the left and right volume levels.                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG *pulVolume -- Retrieved volume.                               */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_GetVolume(PINST pInst, ULONG *pulVolume)
{
   ULONG rc;
   BYTE data[QVOLUME_DMAX];
   ULONG ulDataLen = QVOLUME_DMAX, ulParamLen = STANDRD_PMAX;

   /* audio channel information */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, AUD_CH_INFO,
                  "CD01", ulParamLen, &ulParamLen,
                  data,   ulDataLen,  &ulDataLen);

   if (!rc)
      /* convert volume levels from 0-FF to 0-100% */
      *pulVolume = (data[VOL_LT_FLD] * 100 / 0xFF) |        // Left Channel
                   (data[VOL_RT_FLD] * 100 / 0xFF << 16);   // Right Chan

   return(rc);

}  /* of CD01_GetVolume() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_LockDoor                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Lock Door                                          */
/*                                                                          */
/* FUNCTION:  Disable or enable the manual eject button.                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst     -- Instance structure.                             */
/*      USHORT Lockit    -- Is To Be LOCK flag.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_LockDoor(PINST pInst, USHORT LockIt)
{
   ULONG rc;
   BYTE param[LOCKDOR_PMAX] = "CD01";
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = LOCKDOR_PMAX;

   if (LockIt == MCI_TRUE)
      param[LOCKDORFLD] = 1;             //Eject button is disabled
   else
      param[LOCKDORFLD] = 0;             //Eject button is enabled

   rc = CallIOCtl(pInst, CDDRIVE_CAT, LOCK___DOOR,
                  param, ulParamLen, &ulParamLen,
                  NULL,  ulDataLen,  &ulDataLen);

   /* Drive cannot lock the first time after a disc is mounted. */
   /* Device driver reset hardware on first call, try again.    */
   if (rc)
   {
      ulDataLen = STANDRD_DMAX;
      ulParamLen = LOCKDOR_PMAX;

      rc = CallIOCtl(pInst, CDDRIVE_CAT, LOCK___DOOR,
                     param, ulParamLen, &ulParamLen,
                     NULL,  ulDataLen,  &ulDataLen);

   }  /* of if needed second try */

   return(rc);

}  /* of CD01_LockDoor() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Open                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Open.                                              */
/*                                                                          */
/* FUNCTION:  Open the CD-ROM drive.                                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_INI_FILE  -- corrupted INI file, drive is not CD-ROM drive.  */
/*      MCIERR_DEVICE_LOCKED -- CD-ROM drive, previously opened exclusively.*/
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_MEDIA_CHANGED         -- different disc was inserted.        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Open(PINST pInst)
{
   ULONG rc;

   CHAR drive[] = "A:";
   ULONG ulAction;
   HFILE hDev;
   MCI_CD_REGDISC_PARMS recParam2;
   BYTE data[LOCATON_DMAX], param[LOCATON_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen, ulParamLen;

   drive[0] = pInst->Drive;               /* Get drive letter */

   /* open device */
   rc = DosOpen(drive, &hDev, &ulAction, 0L, 0L, OPENFLAG, OPENMODE, 0L);

   if (rc)
   {
      switch (rc)
      {
         case ERROR_PATH_NOT_FOUND :     //network drive
         case ERROR_INVALID_DRIVE :      //invalid drive, not reg in boot-up(?)
            rc = MCIERR_INI_FILE;
            break;
         case ERROR_SHARING_VIOLATION :  //drive opened exclusively
            rc = MCIERR_DEVICE_LOCKED;
            break;
         case ERROR_NOT_READY :          //disc not in drive, drive powered off
         default :
            /* resume timer loop so that it can exit */
            DosPostEventSem(pInst->hTimeLoopSem);
            /* make play completed before returning */
            while (pInst->usPlayFlag != TIMER_AVAIL)
               DosSleep(HALF_TIME_MIN);
            rc = MCIERR_DEVICE_NOT_READY;
      }  /* of switch() */
   }  /* of if error */
   else                                   /* open was successful */
   {
      pInst->hDrive = hDev;

      if (pInst->DiscID.ulLeadOut == 0L)            // if New Open
         rc = OpenFirstTime(pInst);
      else
      {
         /** Previous Open, register separately and compare with old  *
           * disc.  If the same, pretend nothing happened, otherwise  *
           * return code to make general MCI Driver do a re-register  */
         rc = CDAudRegDisc(pInst, REG_PARAM2, &recParam2);

         if (!rc)
            if (memcmp(&pInst->DiscID, &recParam2.DiscID, sizeof(MCI_CD_ID)))
            {
               CD01_Stop(pInst, TIMER_EXIT_CHANGED);
               rc = MCIERR_MEDIA_CHANGED;           /* Different disc */
            }
      }  /* of else not a new open */
   }  /* of else device is opened */

   return(rc);

}  /* of CD01_Open() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Play                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Play.                                              */
/*                                                                          */
/* FUNCTION:  Initiate playing audio data to internal DAC(s).               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst        -- Instance pointer.                            */
/*      ULONG  *pulParam1   -- Flag for this message.                       */
/*      ULONG  ulFrom       -- From address.                                */
/*      ULONG  ulTo         -- To address in MMTIME.                        */
/*      USHORT usUserParm   -- User Parameter.                              */
/*      HWND   hwndCallback -- Call back handle.                            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY  -- device was not ready, no disc.          */
/*      MCIERR_MEDIA_CHANGED     -- Disc changed.                           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Play(PINST pInst, ULONG *pulParam1, ULONG ulFrom, ULONG ulTo,
                USHORT usUserParm, HWND hwndCallback)
{
   ULONG rc;
   ULONG ulThreadID;
   ULONG cnt;

   /* Stop drive before issuing next play command */
   if ((pInst->usPlayFlag == TIMER_PLAYING) ||
       (pInst->usPlayFlag == TIMER_PLAY_SUSPEND) ||
       (pInst->usPlayFlag == TIMER_PLAY_SUSP_2))
      if (*pulParam1 & MCI_NOTIFY)
         CD01_Stop(pInst, TIMER_EXIT_SUPER);
      else
         CD01_Stop(pInst, TIMER_EXIT_ABORTED);

   /* prepare for play call */
   pInst->ulCurPos = ulFrom;
   pInst->ulEndPos = ulTo;
   pInst->usPlayNotify = (USHORT)(*pulParam1 & (MCI_WAIT | MCI_NOTIFY));
   if (*pulParam1 & MCI_NOTIFY)
   {
      pInst->usPlayUserParm = usUserParm;
      pInst->hwndPlayCallback = hwndCallback;
      *pulParam1 ^= MCI_NOTIFY;
   }  /* notify flag was used */

   if (*pulParam1 & MCI_WAIT)
      rc = CD01_Timer(pInst);      /* returns when play commands end */
   else
   {
      DosResetEventSem(pInst->hReturnSem, &cnt);  //force a wait
      rc = DosCreateThread(&ulThreadID, (PFNTHREAD)CD01_Timer,
                           (ULONG)pInst, 0L, THREAD_STACK_SZ);
      if (rc)
      {
         rc = MCIERR_OUT_OF_MEMORY;
         DosPostEventSem(pInst->hReturnSem);
      }
      else  /* wait for new thread to process enough */
      {
         /* Let MCD know not to send notification by returning wait */
         *pulParam1 = (*pulParam1 & ~MCI_NOTIFY) | MCI_WAIT;

         /* wait for new thread to process enough */
         DosWaitEventSem(pInst->hReturnSem, WAIT_FOREVER);
      }

      DosReleaseMutexSem(pInst->hInstSem);

   }  /* else no wait flag was used */

   return(rc);

}  /* of CD01_Play() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_PlayCont                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Play Continue.                                     */
/*                                                                          */
/* FUNCTION:  Continue to play audio data to internal DAC(s) from a         */
/*            MCIDRV_RESTORE or MCIDRV_SYNC command.                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst       -- Instance pointer.                              */
/*      ULONG ulFrom      -- From address.                                  */
/*      ULONG ulTo        -- To address in MMTIME.                          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY  -- device was not ready, no disc.          */
/*      MCIERR_MEDIA_CHANGED     -- Disc changed.                           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_PlayCont(PINST pInst, ULONG ulFrom, ULONG ulTo)
{
   ULONG rc;
   BYTE param[PLAYAUD_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = PLAYAUD_PMAX;

   /* convert starting MM Time into Redbook 2 format */
   * (ULONG *)&param[STARTFFFLD] = REDBOOK2FROMMM(ulFrom);

   /* convert ending MM Time into Redbook 2 format */
   * (ULONG *)&param[END_FF_FLD] = REDBOOK2FROMMM(ulTo);

   /* Stop drive before issuing next play command */
   CD01_Stop(pInst, TIMER_PLAY_SUSPEND);

   /* play drive */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, PLAY__AUDIO,
                  param, ulParamLen, &ulParamLen,
                  NULL,  ulDataLen,  &ulDataLen);

   if (!rc)
      pInst->ulCurPos = ulFrom;

   /* if Timer was stopped, continue timer loop */
   if (pInst->usPlayFlag == TIMER_PLAY_SUSPEND)
      pInst->usPlayFlag = TIMER_PLAYING;
   DosPostEventSem(pInst->hTimeLoopSem);

   return(rc);

}  /* of CD01_PlayCont() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_PosAdvise                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Position Advise                                    */
/*                                                                          */
/* FUNCTION:  Set up the desired position advise.                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulParam1   -- Flag set for this message.                      */
/*      MCI_POSITION_PARMS *pParam2 -- Pointer to data record structure.    */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_FLAGS_NOT_COMPATIBLE -- Flags are mutually exclusive.        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_PosAdvise(PINST pInst, ULONG ulParam1, MCI_POSITION_PARMS *pParam2)
{
   ULONG rc = MCIERR_SUCCESS;

   if (ulParam1 & MCI_SET_POSITION_ADVISE_ON)
   {
      if (ulParam1 & MCI_SET_POSITION_ADVISE_OFF)
         rc = MCIERR_FLAGS_NOT_COMPATIBLE;
      else
      {
         pInst->qptPosAdvise.ulEvent = pParam2->ulUnits;
         pInst->qptPosAdvise.hwndCallback = pParam2->hwndCallback;
         pInst->qptPosAdvise.usUserParm = pParam2->usUserParm;
      }
   }  /* of if on */
   else
      if (ulParam1 & MCI_SET_POSITION_ADVISE_OFF)
         pInst->qptPosAdvise.ulEvent = 0L;

   return(rc);

}  /* of CD01_PosAdvise() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_RegTracks                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Audio Register Compact Disc.                       */
/*                                                                          */
/* FUNCTION:  Register a CD media so that the disc may be recognized by     */
/*            the calling MCD.                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      MCI_CD_REGTRACKS_PARMS *pParam2 -- Pointer to data record.          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_INVALID_BUFFER        -- Buffer size is too small.           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_RegTracks(PINST pInst, MCI_CD_REGTRACKS_PARMS *pParam2)
{
   ULONG rc;
   BYTE data[DSKTRCK_DMAX], param[DSKTRCK_PMAX] = "CD01";
   BYTE low, hi, cnt;       // low and high track values
   ULONG end_adr, cur_adr;
   ULONG ulDataLen = DSKTRCK_DMAX, ulParamLen = DSKTRCK_PMAX;


   /*** GET DISK INFORMATION ***/

   /* get disk information */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, DISK___INFO,
                  param, ulParamLen, &ulParamLen,
                  data,  ulDataLen,  &ulDataLen);

   if (!rc)
   {                                            // okay, extract data
      low = data[TRCKLOWFLD];
      hi  = data[TRCKHI_FLD];

      /* get ending address */
      end_adr = REDBOOK2TOMM(*(ULONG *)&data[TRENDFFFLD]);

      /* check for valid buffer size */
      if (pParam2->ulBufSize <
             (ULONG)(hi - low + 1) * sizeof(MCI_CD_REGTRACK_REC))
         rc = MCIERR_INVALID_BUFFER;
      else
      {
         rc = MCIERR_SUCCESS;

         /*** LOOP TO FILL TABLE ***/

         for (cnt = 0; low <= hi; cnt++, low++)
         {
            param[TRACKFIELD] = low;

            ulDataLen  = DSKTRCK_DMAX;
            ulParamLen = DSKTRCK_PMAX;
            if (CallIOCtl(pInst, CDAUDIO_CAT, TRACK__INFO,
                          param, ulParamLen, &ulParamLen,
                          data,  ulDataLen,  &ulDataLen))
            {
               rc = MCIERR_DEVICE_NOT_READY;
               break;
            }

            /* get track number */
            (pParam2->TrackRecArr + cnt)->TrackNum = low;

            /* get control byte */
            (pParam2->TrackRecArr + cnt)->TrackControl = data[TRKSTATFLD];

            /* get starting address */
            cur_adr = REDBOOK2TOMM(*(ULONG *)&data[TRACKFFFLD]);
            (pParam2->TrackRecArr + cnt)->ulStartAddr = cur_adr;

            if (cnt)
               /* get ending address */
               (pParam2->TrackRecArr + cnt-1)->ulEndAddr = cur_adr;

            /* get ending address if it is the last track */
            if (low == hi)
               (pParam2->TrackRecArr + cnt)->ulEndAddr = end_adr;

         }   /* of for loop */
      }   /* of else valid buffer size */
   }   /* of if no error getting audio disc information */

   /* return buffer size */
   if (rc)
      pParam2->ulBufSize = 0;
   else
      pParam2->ulBufSize = sizeof(MCI_CD_REGTRACK_REC) * cnt;

   return(rc);

}  /* of CD01_RegTracks() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Restore                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Restore.                                           */
/*                                                                          */
/* FUNCTION:  The device context or instance is being restored.  Restore    */
/*            the drive as listed in pParams.                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Pointer to instance.                            */
/*      MCIDRV_CD_SAVE_PARMS *pParam2 -- pointer to save structure.         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed.                               */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Restore(PINST pInst, MCIDRV_CD_SAVE_PARMS *pParam2)
{
   ULONG rc;

   /* reset volume */
   rc = CD01_SetVolume(pInst, pParam2->ulLevel);

   /* reset position and mode */
   if (!rc)
   {
      switch (pParam2->ulMode)
      {
         case MCI_MODE_STOP :
            rc = CD01_Seek(pInst, pParam2->ulPosition);
            break;
         case MCI_MODE_PLAY :
            rc = CD01_PlayCont(pInst, pParam2->ulPosition, pParam2->ulEndPlay);
            DosPostEventSem(pInst->hTimeLoopSem);  //continue timer loop
            break;
         case MCI_MODE_PAUSE :
            rc = CD01_Stop(pInst, TIMER_PLAY_SUSP_2);
            if (!rc)
            {
               pInst->ulCurPos = pParam2->ulPosition;
               pInst->ulEndPos = pParam2->ulEndPlay;
            }
            break;
         case MCI_MODE_NOT_READY :                /* disc changed on restore */
            rc = CD01_Stop(pInst, TIMER_EXIT_CHANGED);
            break;
         default :   /* error in saved state, try to accept current position */
            break;
      }  /* of switch */
   }  /* if no error setting volume */

   return(rc);

}  /* of CD01_Restore() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Resume                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Resume Playing.                                    */
/*                                                                          */
/* FUNCTION:  Unpause or resume the play command.                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Resume(PINST pInst)
{
   ULONG rc;
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = STANDRD_PMAX;

   if (pInst->usPlayFlag == TIMER_PLAY_SUSP_2)
   {
      rc = CD01_PlayCont(pInst, pInst->ulCurPos, pInst->ulEndPos);
      DosPostEventSem(pInst->hTimeLoopSem);
   }
   else
   {
      /* resume audio */
      rc = CallIOCtl(pInst, CDAUDIO_CAT, RESUMEAUDIO,
                     "CD01", ulParamLen, &ulParamLen,
                     NULL,   ulDataLen,  &ulDataLen);

      /* check if resume failed, a disc was ejected and reinserted */
      if (rc == MCIERR_OUTOFRANGE)
         rc = CD01_PlayCont(pInst, pInst->ulCurPos, pInst->ulEndPos);

      if (!rc)
      {
         if (pInst->usPlayFlag == TIMER_PLAY_SUSPEND)
            pInst->usPlayFlag = TIMER_PLAYING;
         DosPostEventSem(pInst->hTimeLoopSem);
      }
   }   /* of else resuming paused audio */

   return(rc);

}  /* of CD01_Resume() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Save                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Save.                                              */
/*                                                                          */
/* FUNCTION:  The device context or instance is being saved.  Stop the      */
/*            drive and the play timer if needed.  Save the current state.  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Pointer to instance.                            */
/*      MCIDRV_CD_SAVE_PARMS *pParam2 -- pointer to save structure.         */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed.                               */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Save(PINST pInst, MCIDRV_CD_SAVE_PARMS *pParam2)
{
   ULONG rc;

   /* cheek to see if it is playing. If it is, stop drive */
   if (pInst->usPlayFlag == TIMER_PLAYING)
      CD01_Stop(pInst, TIMER_PLAY_SUSPEND);     // Don't care about return code
   else
      if (pInst->usPlayFlag == TIMER_PLAY_SUSP_2)
         pInst->usPlayFlag = TIMER_PLAY_SUSPEND;

   /* get volume */
   CDAudStatCVol(&pParam2->ulLevel);

   /* get position */
   rc = CD01_GetPosition(pInst, &pParam2->ulPosition);

   return(rc);

}  /* of CD01_Save() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Seek                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Seek.                                              */
/*                                                                          */
/* FUNCTION:  Seek to the specified position.                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance pointer.                               */
/*      ULONG ulTo       -- address to seek.                                */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Seek(PINST pInst, ULONG ulTo)
{
   ULONG rc;
   BYTE data[SEEKSTATDMAX], param[SEEKSTATPMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen = SEEKSTATDMAX, ulParamLen = SEEKSTATPMAX;

   /* Stop drive before issuing SEEK command,         *
    *    drive will not SEEK Command if it is playing */
   rc = CallIOCtl(pInst, CDDRIVE_CAT, DEV__STATUS,
                  "CD01", ulParamLen, &ulParamLen,
                   data,  ulDataLen,  &ulDataLen);

   if (!rc)        /* stop drive even if it thinks that its playing */
      if ((data[STATAUDFLD] & IS_PLAYING) ||
          (pInst->usPlayFlag == TIMER_PLAYING) ||
          (pInst->usPlayFlag == TIMER_PLAY_SUSPEND) ||
          (pInst->usPlayFlag == TIMER_PLAY_SUSP_2))
         rc = CD01_Stop(pInst, TIMER_EXIT_ABORTED);

   /* issue hardware seek if no errors */
   if (!rc)
   {
      /* convert MM Time into Redbook 2 format */
      * (ULONG *)&param[SEEK_FFFLD] = REDBOOK2FROMMM(ulTo);

      /* Seek to new location */
      ulDataLen  = STANDRD_DMAX;
      ulParamLen = SEEKSTATPMAX;
      rc = CallIOCtl(pInst, CDDRIVE_CAT, SEEK_POSITN,
                     param, ulParamLen, &ulParamLen,
                     NULL,  ulDataLen,  &ulDataLen);

      /* Some drives cannot seek to some/all audio address, */
      /* the IBM 3510-001 can so a true VSD would not need these lines */
      if (rc == MCIERR_OUTOFRANGE)         // MCD makes sure address is correct
         rc = MCIERR_SUCCESS;              //    Mask CD & PDD errors

      if (!rc)
         pInst->ulCurPos = ulTo;

   }  /* of if no error preparing disk for seek */

   return(rc);

}  /* of CD01_Seek() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_SetVolume                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Set Volume.                                        */
/*                                                                          */
/* FUNCTION:  Set the left and right volume levels on the CD-ROM drive.     */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      ULONG ulVolume   -- Volume Level.                                   */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_SetVolume(PINST pInst, ULONG ulVolume)
{

   ULONG rc;
   BYTE data[QVOLUME_DMAX], left, right;
   ULONG ulDataLen = QVOLUME_DMAX, ulParamLen = STANDRD_PMAX;

   /* get current audio channel information */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, AUD_CH_INFO,
                  "CD01", ulParamLen, &ulParamLen,
                  data,   ulDataLen,  &ulDataLen);

   /* get requested volume levels */
   left  = (BYTE) ulVolume;
   right = (BYTE) (ulVolume >> 16);

   if (!rc)
   {
      /* Since this is the IBM 3510, reset left and right to 0 or FF */
      if (left)
         left = 0xFF;

      if (right)
         right = 0xFF;

      /* Save an IOCLT call by only resetting if a change occurred */

      if (data[VOL_LT_FLD] != left || data[VOL_RT_FLD] != right)
      {
         data[VOL_LT_FLD] = left;
         data[VOL_RT_FLD] = right;
         /* set volume */
         ulDataLen = QVOLUME_DMAX;
         ulParamLen = STANDRD_PMAX;
         rc = CallIOCtl(pInst, CDAUDIO_CAT, AUD_CH_CTRL,
                        "CD01", ulParamLen, &ulParamLen,
                        data,   ulDataLen,  &ulDataLen);

      } /* of if volume had changed */

   }  /* of else Audio Channel Info DevIOCtl worked */

   return(rc);

}  /* of CD01_SetVolume() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Stop                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Stop.                                              */
/*                                                                          */
/* FUNCTION:  Stop the play command.                                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST  pInst       -- Instance pointer.                             */
/*      USHORT usTimerFlag -- Set timer to this flag, if playing.           */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Stop(PINST pInst, USHORT usTimerFlag)
{
   ULONG rc;
   USHORT StopTimer = TRUE;
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = STANDRD_PMAX;

   /* stop timer if one is going */
   if (pInst->usPlayFlag == TIMER_PLAYING ||
       pInst->usPlayFlag == TIMER_PLAY_SUSPEND ||
       pInst->usPlayFlag == TIMER_PLAY_SUSP_2)
   {
      pInst->usPlayFlag = usTimerFlag;

      switch(usTimerFlag)
      {
         case TIMER_PLAY_SUSPEND :
         case TIMER_PLAY_SUSP_2 :
            StopTimer = FALSE;
      }  /* of switch */

      if (StopTimer)
      {
         if (DosWaitThread(&pInst->ulPlayTID, 1L) ==
                           ERROR_THREAD_NOT_TERMINATED)
         {
            /* resume timer loop so that it can exit */
            DosPostEventSem(pInst->hTimeLoopSem);

            /* make play completed before returning, race condition. */
            while (pInst->usPlayFlag != TIMER_AVAIL)
               DosSleep(HALF_TIME_MIN);

         }  /* of if thread exists */
         else
            pInst->usPlayFlag = TIMER_AVAIL;     //play thread doesn't exist

     }   /* of if Play Time needs to be stopped */
   }  /* of if timer is being used */

   /* stop drive */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, STOP__AUDIO,
                  "CD01", ulParamLen, &ulParamLen,
                   NULL,  ulDataLen,  &ulDataLen);

   return(rc);

}  /* of CD01_Stop() */




/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  IBMCDMSC.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  IBM CD-ROMs Miscellaneous                             */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1990 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains miscellaneous functions such as the timer  */
/*            routine for the MCI_PLAY command, querying the CD table, and  */
/*            packaging the IOCTL calls.                                    */
/*                                                                          */
/*                                                                          */
/* FUNCTIONS:                                                               */
/*       CD01_StartPlay     - Start the play operation.                     */
/*       CD01_Sync          - Sync to MDM request.                          */
/*       CD01_Timer         - Timer routine for play operation.             */
/*       CD01_TimerNotify   - Timer routine to setup and notify events.     */
/*       GetTableName       - Get the full path name of the CD table.       */
/*       OpenFirstTime      - First time device open tests.                 */
/*       QueryTable         - Query CD look-up table.                       */
/*       CallIOCtl          - Call the hardware via IOCTLs.                 */
/*                                                                          */
/*       NOTE:  CD01_... refers to commands that are compatible with the    */
/*              IBM 3510 CD-ROM drive.  CD02_... may refer to commands      */
/*              that are compatible with the CD02 drives, which are         */
/*              not compatible with the CD01_... commands.  This way        */
/*              different hardware can share the same VSD.                  */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSDEVICES
#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSSEMAPHORES
#define INCL_WINSHELLDATA
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#define INCL_MCIOS2
#include <os2me.h>
#include <cdaudos2.h>
#include "ibmcdrom.h"


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_StartPlay                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Start Play.                                        */
/*                                                                          */
/* FUNCTION:  Start playing audio data to internal DAC(s).                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst       -- Instance pointer.                              */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY  -- device was not ready, no disc.          */
/*      MCIERR_MEDIA_CHANGED     -- Disc changed.                           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_StartPlay(PINST pInst)
{
   ULONG rc;
   BYTE param[PLAYAUD_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen = STANDRD_DMAX, ulParamLen = PLAYAUD_PMAX;
   ULONG cnt;
   PTIB ptib;
   PPIB ppib;

   /* convert MM Time into Redbook 2 format */
   * (ULONG *)&param[STARTFFFLD] = REDBOOK2FROMMM(pInst->ulCurPos);
   * (ULONG *)&param[END_FF_FLD] = REDBOOK2FROMMM(pInst->ulEndPos);

   /* play drive */
   rc = CallIOCtl(pInst, CDAUDIO_CAT, PLAY__AUDIO,
                  param, ulParamLen, &ulParamLen,
                  NULL,  ulDataLen,  &ulDataLen);

   if (!rc)
   {
      /* set timer play flag */
      pInst->usPlayFlag = TIMER_PLAYING;
      DosGetInfoBlocks(&ptib, &ppib);
      pInst->ulPlayTID = ptib->tib_ptib2->tib2_ultid;
      DosResetEventSem(pInst->hTimeLoopSem, &cnt);  // force a wait in timer

   }  /* if no error */

   /* original thread owns Mutex sem,               */
   /* if not WAIT tell original thead to release it */
   if (pInst->usPlayNotify == MCI_WAIT)
      DosReleaseMutexSem(pInst->hInstSem);
   else
      DosPostEventSem(pInst->hReturnSem);

   return(rc);

}  /* of CD01_StartPlay() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Sync                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Synchronization.                                   */
/*                                                                          */
/* FUNCTION:  Process SYNC messages from MCIDRV_SYNC message.               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance pointer.                               */
/*      ULONG ulParam1   -- Flag for this message.                          */
/*      MCIDRV_SYNC_PARMS *pParam2 -- pointer to record structure.          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_INVALID_FLAG          -- Unknown flag.                       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Sync(PINST pInst, ULONG ulParam1, MCIDRV_SYNC_PARMS *pParam2)
{
   ULONG  rc = MCIERR_SUCCESS;
   ULONG  ulPosition;
   MMTIME mmTime;
   LONG   lDelta;
   CHAR   szBuf[SSM_LOAD_SIZE];
   HMODULE hMod;                         // handle to SSM module

   ulParam1 &= WAIT_NOTIFY_MASK;

   switch (ulParam1)
   {
      case MCIDRV_SYNC_ENABLE :
         pParam2->pevcbSyncPulse  = &pInst->evcbSync;
         pParam2->NonStreamMaster = TRUE;
         break;
      case MCIDRV_SYNC_DISABLE :
         pInst->StreamMaster = FALSE;
         break;
      case MCIDRV_SYNC_MASTER :
         pInst->StreamMaster = TRUE;                  // set master stream flag
         pInst->RptEvent.hid = pParam2->hidMaster;    // store id for SSM calls
         pInst->RptEvent.pevcbEvent = (PEVCB) &pInst->evcbSync;
         pInst->evcbSync.mmtimeMaster = (MMTIME) pInst->ulCurPos;   // set time

         /* get SSM loaded, if not yet loaded */
         if (pInst->pSyncNotify == NULL)
         {
            rc = DosLoadModule(szBuf, SSM_LOAD_SIZE, "SSM", &hMod);
            if (!rc)
               rc = DosQueryProcAddr(hMod, 0L, "SMHEntryPoint",
                               &pInst->pSyncNotify);
            if (rc)
            {
               pInst->pSyncNotify = NULL;
               rc = MCIERR_CANNOT_LOAD_DRIVER;
            }  /* of if error */
         }  /* of if need to get sync entry point */
         break;
      case MCIDRV_SYNC_REC_PULSE :
         /* get current position */
         CD01_GetPosition(pInst, &ulPosition);

         mmTime = pInst->evcbSync.mmtimeMaster;
         lDelta = ulPosition - mmTime - SYNC_LATENCY;
         if (lDelta < -SYNC_TOLERANCE || lDelta > SYNC_TOLERANCE)
            rc = CD01_PlayCont(pInst, (ULONG) mmTime, pInst->ulEndPos);
         pInst->evcbSync.mmtimeSlave = mmTime;         // reset current time
         /* Clear Sync Flags */
         pInst->evcbSync.ulSyncFlags &= ~(SYNCOVERRUN | SYNCPOLLING);
         break;
      default :
         rc = MCIERR_INVALID_FLAG;
   }  /* of switch */

   return(rc);

}  /* of CD01_Sync() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_Timer                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Timer                                              */
/*                                                                          */
/* FUNCTION:  Queries the device once a second to determine is a play       */
/*            command is still going on.                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance pointer.                               */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY  -- device was not ready, no disc.          */
/*      MCIERR_MEDIA_CHANGED     -- Disc changed on restore as reported by  */
/*                                  MCD.  Return error on play thread.      */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG CD01_Timer(PINST pInst)
{
   ULONG rc;
   ULONG ulDelay, ulPrevLimit;
   ULONG ulPosAdvise = 0L, ulSyncPos = 0L, ulOldPosAdvise = 0L;
   BYTE dataAud[AUDSTAT_DMAX];    //values for the audio status command
   BYTE dataPos[LOCATON_DMAX],
          param[LOCATON_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen, ulParamLen;
   ULONG cnt, DoLoop = TRUE;
   USHORT usNotifyType = MCI_NOTIFY_SUCCESSFUL;

   /* start playing */
   rc = CD01_StartPlay(pInst);
   ulPrevLimit = pInst->ulCurPos;

   if (!rc)
   do
   {
      /* check position advise before calling notification routine. */
      /* event must be multiple address relative to address 0.      */
      if (pInst->qptPosAdvise.ulEvent != ulOldPosAdvise)
      {
         ulOldPosAdvise = pInst->qptPosAdvise.ulEvent;
         if (ulOldPosAdvise)
            if (pInst->ulCurPos % ulOldPosAdvise)    //if past, get next one
               ulPosAdvise = (pInst->ulCurPos / ulOldPosAdvise + 1)
                              * ulOldPosAdvise;
            else
               ulPosAdvise = pInst->ulCurPos;
         else
            ulPosAdvise = 0L;
      }  /* of if position advise changed */

      ulDelay = CD01_TimerNotify(pInst, &ulPosAdvise, &ulSyncPos, &ulPrevLimit);

      /********************************************************************
       * wait on the semaphore.  If the instance is PAUSED or being SAVED *
       * (no longer active) then wait until RESUME or RESTORE clears the  *
       * semaphore.  If a destructive STOP is received, the wait will     *
       * terminate immediately.  Otherwise, wait as long as ulDelay.      *
       ********************************************************************/

      if (!DosWaitEventSem(pInst->hTimeLoopSem, ulDelay))
         /* semaphore is cleared.  Reset it incase we don't exit loop */
         DosResetEventSem(pInst->hTimeLoopSem, &cnt);

      /* get new position */
      ulDataLen  = LOCATON_DMAX;
      ulParamLen = LOCATON_PMAX;
      rc = CallIOCtl(pInst, CDDRIVE_CAT, Q__LOCATION,
                     param,   ulParamLen, &ulParamLen,
                     dataPos, ulDataLen,  &ulDataLen);

      /* The only way to receive a media change rc is when closing   *
       * an inactive instance after a disc change, process the abort */
      if (rc)
         if (rc == MCIERR_MEDIA_CHANGED)
            rc = MCIERR_SUCCESS;
         else   // an error occurred, break out of the loop
            break;

      switch (pInst->usPlayFlag)
      {
         case TIMER_EXIT_SUPER :
            usNotifyType = MCI_NOTIFY_SUPERSEDED;
            DoLoop = FALSE;
            break;
         case TIMER_EXIT_ABORTED :
            usNotifyType = MCI_NOTIFY_ABORTED;
            DoLoop = FALSE;
            break;
         case TIMER_EXIT_CHANGED :
            rc = MCIERR_MEDIA_CHANGED;
            DoLoop = FALSE;
            break;
         case TIMER_AVAIL :
            rc = MCIERR_DRIVER_INTERNAL;       // abondon ship
            DoLoop = FALSE;
            break;
         case TIMER_PLAY_SUSPEND :             // do nothing
         case TIMER_PLAY_SUSP_2  :
            break;
         case TIMER_PLAYING :
            /* update current location */
            pInst->ulCurPos = REDBOOK2TOMM(*(ULONG *)dataPos);

            DosRequestMutexSem(pInst->hIOSem, (ULONG)-1L);

            /* check if drive is still playing */
            ulDataLen  = AUDSTAT_DMAX;
            ulParamLen = STANDRD_PMAX;
            if (DosDevIOCtl(pInst->hDrive, CDDRIVE_CAT, DEV__STATUS,
                            "CD01",  ulParamLen, &ulParamLen,
                            dataAud, ulDataLen,  &ulDataLen))
            {                         // Error
               rc = MCIERR_DEVICE_NOT_READY;
               DoLoop = FALSE;
            }  /* of if error calling device */
            else
            {
               if (!(dataAud[STATAUDFLD] & IS_PLAYING))      // if NOT Playing
               {
                  /* check to see if PLAY terminated or was STOPPED/PAUSED */
                  ulDataLen  = AUDSTAT_DMAX;
                  ulParamLen = STANDRD_PMAX;
                  if (DosDevIOCtl(pInst->hDrive, CDAUDIO_CAT, AUD__STATUS,
                                  "CD01",  ulParamLen, &ulParamLen,
                                  dataAud, ulDataLen,  &ulDataLen))
                  {
                     rc = MCIERR_DEVICE_NOT_READY;
                     DoLoop = FALSE;
                  }  /* of if error calling device */
                  else
                     if (!(dataAud[AUDSTATFLD] & WAS_STOPPED))
                     {     /* PLAY command finished */
                        rc = MCIERR_SUCCESS;
                        DoLoop = FALSE;
                        pInst->ulCurPos = pInst->ulEndPos;
                     }  /* of else PLAY command terminated itself (finished) */
                     /* else it is paused, continue the clock */

               }  /* of if NOT Playing */
            }  /* of else no error querying status */

            DosReleaseMutexSem(pInst->hIOSem);
            break;
         default :
            rc = MCIERR_DRIVER_INTERNAL;       // unknown play flag
            DoLoop = FALSE;
            break;

      }   /* of switch on play flag */
   } while(DoLoop);  /* of do loop */

   if (!rc)         // if no error, report any events
      CD01_TimerNotify(pInst, &ulPosAdvise, &ulSyncPos, &ulPrevLimit);

   pInst->ulPlayTID  = 0L;                     // clear before exit or return

   if (pInst->usPlayNotify != MCI_WAIT)
   {
      if (rc)
         usNotifyType = (USHORT) rc;

      /* inform MDM that play has terminated */
      if (pInst->usPlayNotify == MCI_NOTIFY)
         mdmDriverNotify(pInst->usDeviceID, pInst->hwndPlayCallback,
                         MM_MCINOTIFY, pInst->usPlayUserParm,
                         MAKEULONG(MCI_PLAY, usNotifyType));

      /* inform MCD that play has terminated */
      pInst->pCDMCDReturn(pInst->ulCDMCDID, MAKEULONG(MCI_PLAY, usNotifyType),
                          0L);               //MCD ignores 3rd field for PLAY

      pInst->usPlayFlag = TIMER_AVAIL;
      DosExit(0L, 0L);
   }  /* of if notify was used */

   if (!rc && usNotifyType == MCI_NOTIFY_SUCCESSFUL)
      pInst->ulCurPos = pInst->ulEndPos;

   pInst->usPlayFlag = TIMER_AVAIL;

   return(rc);

}  /* of CD01_Timer() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CD01_TimerNotify                                       */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD Timer Notify                                       */
/*                                                                          */
/* FUNCTION:  Sets up the wait value to suspend the timer thread based on   */
/*            upcoming events.  It also sends notification of past or soon  */
/*            to pass events.                                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst         -- Instance pointer.                            */
/*      ULONG *pulPosAdvise -- Position for next Position Advise Event.     */
/*      ULONG *pSyncPos     -- Position for next Sync Pulse if master.      */
/*      ULONG *pulPrevLimit -- Last range checked.                          */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*                                                                          */
/* NOTES:  It is important to note that events are in MM Time and delay     */
/*         factors are in milliseconds.                                     */
/*                                                                          */
/****************************************************************************/
ULONG CD01_TimerNotify(PINST pInst, ULONG *pulPosAdvise,
                       ULONG *pSyncPos, ULONG *pulPrevLimit)
{
   ULONG ulDelay, ulUpperLimit, ulEvent;
   LONG lTemp;
   int i;

   /* find wait value by finding time to next event */
   ulDelay = WAIT_TIME_MAX;
   ulUpperLimit = pInst->ulCurPos + HALF_TIME_MIN * 3; // *3 is ms to mmtime

   /* process cuepoints */
   for (i=0; i < CDMCD_CUEPOINT_MAX; i++)
   {
      ulEvent = pInst->arrCuePoint[i].ulEvent;

      /* skip if no event */
      if (ulEvent == (ULONG) -1L)
         continue;

      /* check to see if event is in window */
      if (ulEvent >= *pulPrevLimit && ulEvent < ulUpperLimit)
      {
         mdmDriverNotify(pInst->usDeviceID,
                         pInst->arrCuePoint[i].hwndCallback,
                         MM_MCICUEPOINT, pInst->arrCuePoint[i].usUserParm,
                         ulEvent);
         continue;
      }
                                          // there is an implied ELSE here
      lTemp = (LONG)(ulEvent - pInst->ulCurPos) / 3;
      if (lTemp > 0 && (ULONG)lTemp < ulDelay)
         ulDelay = (ULONG)lTemp;

   }  /* of cuepoint for loop */

   /* process position advise */
   if (*pulPosAdvise)
   {
      /* loop for small advise frequencies */
      do
      {
         lTemp = (LONG)(*pulPosAdvise - pInst->ulCurPos) / 3;
         if (lTemp < HALF_TIME_MIN)
         {
            mdmDriverNotify(pInst->usDeviceID,
                            pInst->qptPosAdvise.hwndCallback,
                            MM_MCIPOSITIONCHANGE,
                            pInst->qptPosAdvise.usUserParm,
                            (ULONG) *pulPosAdvise);
            *pulPosAdvise += pInst->qptPosAdvise.ulEvent;
         }
      }  while (lTemp < HALF_TIME_MIN);

      if ((ULONG)lTemp < ulDelay)
         ulDelay = (ULONG)lTemp;
   }  /* of if process advise event pending */

   /* process sync pulse if VSD is the master */
   if (pInst->StreamMaster)
   {
      if (*pSyncPos)
      {
         lTemp = (LONG)(*pSyncPos - pInst->ulCurPos) / 3;
         if (lTemp < HALF_TIME_MIN)
         {
            /* set current position/time */
            pInst->evcbSync.mmtimeMaster = (MMTIME) pInst->ulCurPos;
            pInst->RptEvent.ulFunction   = SMH_REPORTEVENT;
            pInst->RptEvent.hevent       = 0;
            pInst->pSyncNotify(&pInst->RptEvent);
            *pSyncPos = pInst->ulCurPos + SYNC_TOLERANCE * 2 * 3;
         }
      }
      else      // new event just started
         *pSyncPos = pInst->ulCurPos + SYNC_TOLERANCE * 2 * 3;
   }  /* of if position advise is set */
   else
      *pSyncPos = 0L;

   *pulPrevLimit = ulUpperLimit;

   /* if suspeneded, set delay flag to forever */
   if (pInst->usPlayFlag == TIMER_PLAY_SUSPEND ||
       pInst->usPlayFlag == TIMER_PLAY_SUSP_2)
      ulDelay = (ULONG) WAIT_FOREVER;

   return(ulDelay);

}  /* of CD01_TimerNotify() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  GetTableName              DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get CD Table full path name.                          */
/*                                                                          */
/* FUNCTION:  Get the full path name of the CD look-up table, MMPMCD.INI.   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      CHAR *pName      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
VOID GetTableName(CHAR *pName)
{
   CHAR *ptr;

   /* find path */
   if (DosScanEnv("MMBASE", &ptr))
      strcpy(pName, "\\MMOS2\\MMPMCD.INI");
   else
   {
      strcpy(pName, ptr);
      ptr = strstr(pName, ";");
      if (ptr)
         *ptr = '\0';
      ptr = pName + strlen(pName);
      strcpy(ptr, "\\MMPMCD.INI");

   } /* of else environmental variable was found */

}  /* of GetTableName() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  OpenFirstTime             DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Open drive for the First Time.                        */
/*                                                                          */
/* FUNCTION:  Run tests on the drive after the first successful hardware    */
/*            open.                                                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
ULONG OpenFirstTime(PINST pInst)
{
   ULONG rc;
   BYTE data[LOCATON_DMAX], param[LOCATON_PMAX] = {'C', 'D', '0', '1', RBMODE};
   ULONG ulDataLen, ulParamLen, ulStatus;

   /* test to see if drive is really a CD-ROM Drive */
   ulDataLen  = IDCDROM_DMAX;
   ulParamLen = STANDRD_PMAX;
   if (CallIOCtl(pInst, CDDRIVE_CAT, ID___CD_ROM,
                 param, ulParamLen, &ulParamLen,
                 data,  ulDataLen,  &ulDataLen))
      rc = MCIERR_INI_FILE;
   else
      if (memcmp(data, "CD01", IDCDROM_DMAX))
         rc = MCIERR_INI_FILE;
      else
         rc = MCIERR_SUCCESS;

   if (!rc)
   {  /** Get current position and let the registration *
        * insert information to the instance structure. **/
      ulDataLen  = LOCATON_DMAX;
      ulParamLen = LOCATON_PMAX;
      rc = CallIOCtl(pInst, CDDRIVE_CAT, Q__LOCATION,
                     param, ulParamLen, &ulParamLen,
                     data,  ulDataLen,  &ulDataLen);
      pInst->ulCurPos = REDBOOK2TOMM(*(ULONG *)data);

      rc = CDAudRegDisc(pInst, REG_INST, NULL);

   }   /* of if no error identifying drive */

   return(rc);

}  /* of OpenFirstTime() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  QueryTable                DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Query Table from CD INI file.                         */
/*                                                                          */
/* FUNCTION:  Query the CD look-up table, MMPMCD.INI, and extract           */
/*            information or set default information if error.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance structure.                             */
/*      VVOL_SET_REC *pVVolRec -- pointer to Vector Volume record.          */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:                                                                   */
/*      This is an example of how to query the CD look-up table, file       */
/*      MMPMCD.INI, to get the capabilities and features of a CD-ROM drive. */
/*      A VSD designed to control a specific hardware device, such as this  */
/*      VSD, would not need to call the INI file because the capabilities   */
/*      should already be known.                                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
VOID QueryTable(PINST pInst)
{
   ULONG rc;
   HAB habTemp;
   HINI hiniFile;
   ULONG ulSize;
   CHAR szIniFile[CCHMAXPATH];
   CHAR szAppName[CDINI_APPSIZE+1], szKeyName[CDINI_KEYSIZE+1];
   MMPMCD_REC recCDEntry;
   BYTE i;
   int len;
   ULONG ulCaps1, ulCaps2, ulMinStart;
   BYTE VolCnt, VolValues[VOLUME_CONTROL];

   /* open INI file and get info */
   habTemp = WinInitialize(0);
   if (habTemp)                 // if sucessful, open INI file
   {
      /* get complete path name for INI file */
      GetTableName(szIniFile);

      hiniFile = PrfOpenProfile(habTemp, szIniFile);
      if (hiniFile)
      {
         memcpy(szAppName, pInst->szModel, CDINI_APPSIZE);
         szAppName[CDINI_APPSIZE] = '\0';
         memcpy(szKeyName, pInst->szModel + CDINI_APPSIZE, CDINI_KEYSIZE);
         szKeyName[CDINI_KEYSIZE] = '\0';

         /* check to see Key name needs padding, */
         /* trailing spaces are lost in MMPM2.INI */
         for (len = strlen(szKeyName); len < CDINI_KEYSIZE; len++)
            szKeyName[len] = ' ';
         ulSize = sizeof(MMPMCD_REC);

         /* make query */
         if (PrfQueryProfileData(hiniFile, szAppName, szKeyName, &recCDEntry,
                                 &ulSize ))
         {    /* if query is okay, extract data */
            ulCaps1    = recCDEntry.ulCaps1;
            ulCaps2    = recCDEntry.ulCaps2;
            ulMinStart = REDBOOK2TOMM(recCDEntry.ulMinStart);
            VolCnt     = (BYTE)recCDEntry.usVolCnt;
            for (i=0; i < VolCnt; i++)
               VolValues[i] = (BYTE)recCDEntry.ausVolValues[i];

            /******************************************************************
             *  In this sample VSD, the IBM 3510-001 CD-ROM Drive is assumed  *
             *  to be the associated CD-ROM drive.  Thus the capabilities of  *
             *  the drive are already known and the table information is      *
             *  ignored.                                                      *
             ******************************************************************/

         } /* of if successful query */

         PrfCloseProfile(hiniFile);

      } /* of if successful */

      WinTerminate(habTemp);

   } /* of if successful WinInit */

}  /* of QueryTable() */



/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  CallIOCtl                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Call DosDevIOCTL()                                    */
/*                                                                          */
/* FUNCTION:  Call the DosDevIOCTLM Time to Redbook address.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PINST pInst      -- Instance pointer.                               */
/*      ULONG ulCat      -- Category.                                       */
/*      ULONG ulFunction -- Function number.                                */
/*      PVOID pParam     -- pointer to Parameter array.                     */
/*      ULONG ulPLen     -- Parameter array length.                         */
/*      ULONG *pulPLen   -- pointer to Parameter array length.              */
/*      PVOID pData      -- pointer to Data array.                          */
/*      ULONG ulDLen     -- Data array length.                              */
/*      ULONG *pulDLen   -- pointer to Data array length.                   */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS   -- action completed without error.                 */
/*      MCIERR_DEVICE_NOT_READY      -- device was not ready, no disc.      */
/*      MCIERR_MEDIA_CHANGED         -- a disc was inserted.                */
/*      MCIERR_HARDWARE              -- unknown error occurred.             */
/*      MCIERR_OUTOFRANGE            -- Invalid value.                      */
/*      MCIERR_UNSUPPORTED_FUNCTION  -- unsupported command.                */
/*                                                                          */
/* NOTES:  DosDevIOCTL sometimes fail when called by a second thread.  To   */
/*      avoid this, a generated thread should have at least a 16K stack     */
/*      size.                                                               */
/*                                                                          */
/****************************************************************************/
ULONG CallIOCtl(PINST pInst,  ULONG ulCat,  ULONG ulFunction,
                PVOID pParam, ULONG ulPLen, ULONG *pulPLen,
                PVOID pData,  ULONG ulDLen, ULONG *pulDLen)
{
   ULONG rc;

   DosRequestMutexSem(pInst->hIOSem, (ULONG)-1L);
   rc = DosDevIOCtl(pInst->hDrive, ulCat, ulFunction,
                    pParam, ulPLen, pulPLen,
                    pData,  ulDLen, pulDLen);
   DosReleaseMutexSem(pInst->hIOSem);

   switch(rc)
   {
      case 0L :
         rc = MCIERR_SUCCESS;
         break;
      case 0xFF03 :
         rc = MCIERR_UNSUPPORTED_FUNCTION;
         break;
      case 0xFF06 :
      case 0xFF08 :
         rc = MCIERR_OUTOFRANGE;
         break;
      case 0xFF04 :
      case 0xFF0C :
         rc = MCIERR_HARDWARE;
         break;
      case 0xFF10 :
         rc = MCIERR_MEDIA_CHANGED;
         break;
      default :
         rc = MCIERR_DEVICE_NOT_READY;
   }

   return(rc);

}  /* of CallIOCtl() */


/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHMISC.C                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Misc. routines                         */
/*                                                                          */
/* FUNCTION: This file contains the supporting routines for the stream      */
/*           handlers.                                                      */
/*                                                                          */
/* ENTRY POINTS: ShFindSib                                                  */
/*               ShFindEspcb                                                */
/*               ShFindESPCB  - handles generic cases                       */
/*               ShSearchESPCBList - search list for match                  */
/*               DestroySIB                                                 */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSERRORS
#define  INCL_DOSPROCESS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShFindSib                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Find Stream Instance Block              */
/*                                                                          */
/* FUNCTION: Finds the SIB associated with the stream handle, and handler   */
/*           ID passed.                                                     */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShFindSib                                                   */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   psib       - stream instance block                                     */
/*   hstream    - stream handle                                             */
/*   hid        - handler id                                                */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*        ERROR_INVALID_STREAM                                              */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES: None                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     sib                                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShFindSib(psib, hstream, hid)
PSIB *psib;                               /* Output - pointer to SIB */
HSTREAM hstream;                          /* Input - stream handle */
HID hid;                                  /* Handler ID to identify */
                                          /*  source or target */

{ /* Start of ShFindSib */

RC rc = NO_ERROR;                       /* local return code */
PSIB psibtemp;                          /* Stream instance block */

  /*
   * Find our Stream Instance Block by searching the SIB list.  It is
   *  under semaphore control, so get it first.
   */
  ENTERCRITX(rc);
  psibtemp = pSIB_list_head;
  while ((psibtemp) &&
         !((psibtemp->hStream == hstream) && (psibtemp->HandlerID == hid)))
    {
      psibtemp = psibtemp->pnxtSIB;
    }
  if (psibtemp)
    {
      *psib = psibtemp;
    }
  else
    {
      rc = ERROR_INVALID_STREAM;
    }

  EXITCRIT;
  return(rc);

} /* End of ShFindSib */


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShFindEspcb                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Find Extended Stream Protocol Control   */
/*                   Block                                                  */
/*                                                                          */
/* FUNCTION: Finds the ESPCB described by the spcbkey passed.               */
/*                                                                          */
/* NOTES: The hmtxGlobalData semaphore must be obtained before calling      */
/*        this routine.                                                     */
/*                                                                          */
/* ENTRY POINT: ShFindEspcb                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   spcbkey    - SPCB key                                                  */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*        Return from SHSearchESPCBList                                     */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES: None                                                         */
/*        SHSearchESPCBList                                                 */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

PESPCB ShFindEspcb(spcbkey)
SPCBKEY spcbkey;                          /* Input - key of espcb to find */

{ /* Start of ShFindEspcb */

  return (ShSearchESPCBList(&spcbkey,0L, NULL));

} /* End of ShFindEspcb */


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShFindESPCB                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Find Extended Stream Protocol Control   */
/*                   Block with defaults specified.                         */
/*                                                                          */
/* FUNCTION: Finds the ESPCB described by the spcbkey passed.               */
/*                                                                          */
/* NOTES: The hmtxGlobalData semaphore must be obtained before calling      */
/*        this routine.                                                     */
/*                                                                          */
/* ENTRY POINT: ShFindESPCB                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   spcbkey    - SPCB key                                                  */
/*   ulDefaults - defaults flags                                            */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*        None                                                              */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES: None                                                         */
/*        SHSearchESPCBList                                                 */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

PESPCB ShFindESPCB(spcbkey,ulDefaults)
SPCBKEY spcbkey;                          /* Input - key of espcb to find */
ULONG   ulDefaults;

{ /* Start of ShFindESPCB */

PESPCB pTempEspcb;

  /*
   * Find our Extended SPCB by searching the ESPCB list.
   */

    { /* Search ESPCB list */
      pTempEspcb = ShSearchESPCBList(&spcbkey,ulDefaults,NULL);

      /*
       * If espcb not found then get the last generic spcb installed and use it if
       * ulDefaults flag is set to use it. (last = head of list)
       */
      if (!pTempEspcb)
        { /* Get Generic spcb */

          if (ulDefaults & ESPCB_DEFAULT_LAST_GENERIC)
            {
              spcbkey.ulDataType = DATATYPE_GENERIC;
              pTempEspcb = ShSearchESPCBList(&spcbkey,ESPCB_IGNORE_INTKEY | ESPCB_IGNORE_DATASUBTYPE,NULL);
            }
        }
    } /* Search ESPCB list */

  return(pTempEspcb);

} /* End of ShFindESPCB */


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShSearchESPCBList                                       */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Find Extended Stream Protocol Control   */
/*                   Block with defaults specified.                         */
/*                                                                          */
/* FUNCTION: Finds the ESPCB described by the spcbkey passed.               */
/*                                                                          */
/* NOTES: The hmtxGlobalData semaphore must be obtained before calling      */
/*        this routine.                                                     */
/*                                                                          */
/* ENTRY POINT: ShFindESPCB                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   pspcbkey   - pointer to SPCB key                                       */
/*   ulDefaults - defaults flags                                            */
/*   ppespcbPrevRtn - pointer to an ESPCB                                   */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*        None                                                              */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES: None                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

#ifdef FIXSH
PESPCB ShSearchESPCBList(pspcbkey,ulDefaults,ppespcbPrevRtn)
PSPCBKEY pspcbkey;                         /* Input - key of espcb to find */
ULONG    ulDefaults;
PESPCB   *ppespcbPrevRtn;
#else
PESPCB ShSearchESPCBList(pspcbkey,ulDefaults,pespcbPrevRtn)
PSPCBKEY pspcbkey;                         /* Input - key of espcb to find */
ULONG    ulDefaults;
PESPCB   pespcbPrevRtn;
#endif

{ /* Start of ShFindEspcb */

PESPCB pespcbTemp;
PESPCB pespcbPrev;
BOOL fMatchDataType;
BOOL fMatchDataSubType;
BOOL fMatchIntKey;

  /*
   * Find our Extended SPCB by searching the ESPCB list.
   */

    { /* Search ESPCB list */

      for (pespcbTemp = pESPCB_ListHead, pespcbPrev = NULL;
           pespcbTemp;
           pespcbPrev = pespcbTemp, pespcbTemp = pespcbTemp->pnxtESPCB)
        { /* Loop thru espcbs */

          fMatchDataType = (pspcbkey->ulDataType == pespcbTemp->spcb.spcbkey.ulDataType);
          fMatchDataSubType = ((pspcbkey->ulDataSubType == pespcbTemp->spcb.spcbkey.ulDataSubType) ||
                               (ulDefaults & ESPCB_IGNORE_INTKEY));
          fMatchIntKey = ((pspcbkey->ulIntKey == pespcbTemp->spcb.spcbkey.ulIntKey) ||
                          (ulDefaults & ESPCB_IGNORE_INTKEY));

          if (fMatchDataSubType && fMatchDataType && fMatchIntKey)
            {
#ifdef FIXSH
              if (ppespcbPrevRtn)
                *ppespcbPrevRtn = pespcbPrev;
#else
              if (pespcbPrevRtn)
                pespcbPrevRtn = pespcbPrev;
#endif
              break;
            }
        } /* Loop thru espcbs */

    } /* Search ESPCB list */

  return(pespcbTemp);

} /* End of ShSearchEspcb */



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: DestroySIB                                              */
/*                                                                          */
/* DESCRIPTIVE NAME: Deallocate the SIB and all other structures            */
/*                                                                          */
/* FUNCTION: Deallocate the SIB and other associated structures.            */
/*                                                                          */
/* ENTRY POINT: DestroySIB                                                  */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*         PSIB   psib      - Pointer to stream instance control block.     */
/*                                                                          */
/* EXIT-NORMAL: 0 NO_ERROR                                                  */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_STREAM                                                   */
/*   Errors from:                                                           */
/*     DosRequestMutexSem                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES: None                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC DestroySIB ( PSIB psib )

{
RC    rc = NO_ERROR;                       /* local return code */

   if (psib->ThreadID)
     {
       psib->ulActionFlags |= SIBACTFLG_KILL;
       SEND_MSG(psib,SIBACTFLG_KILL,0L);     /* Tell IO thread to kill itself */
       WAIT_FOR_DEAD_IO(psib)                /* Wait until IO thread is dead. */
     }

   if (psib->hevStop)
     {
       DosCloseEventSem(psib->hevStop);
     }

   if (psib->hevCmdSync)
     {
       DosCloseEventSem(psib->hevCmdSync);
     }

   ENTERCRIT(rc);
   HhpFreeMem(hHeap, psib);
   EXITCRIT;

   return (rc);
}

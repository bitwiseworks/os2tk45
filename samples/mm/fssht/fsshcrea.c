/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: FSSHCREA.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Create stream routine for file system  */
/*                    stream handler                                        */
/*                                                                          */
/* FUNCTION: This function is creates a stream instance.                    */
/*                                                                          */
/* ENTRY POINTS: ShcCreate                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcCreate                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Create stream routine           */
/*                                                                          */
/* FUNCTION: This routine creates a stream instance of the datatype and     */
/*   subtype passed.                                                        */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcCreate                                                   */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc create parameter block (PARM_CREATE) containing:   */
/*   ULONG   ulFunction  Handler command function SHC_CREATE                */
/*   HID     hid         handler ID                                         */
/*   HSTREAM hstream     handle of stream instance                          */
/*   ULONG   ulFlag      Seek flag with following possible Flags:           */
/*                         SPI_SEEK_ABSOLUTE                                */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_SPCBKEY                                                  */
/*   ERROR_ALLOC_RESOURCES                                                  */
/*   Errors from the external routines:                                     */
/*     DosRequestMutexSem                                                   */
/*     DosCreateThread                                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*   Stream Instance Block allocated and added to active list.              */
/*   Thread created suspended to perform I/O (read/write).                  */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShFindEspcb                                                          */
/*     HhpAllocMem                                                          */
/*     DosRequestMutexSem                                                   */
/*     DosCreateThread                                                      */
/*     DosReleaseMutexSem                                                   */
/*     DosSetPriority                                                       */
/*     DosResumeThread                                                      */
/*     DosWaitThread                                                        */
/*     HhpFreeMem                                                           */
/*     HhpGetPID                                                            */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     hmtxGlobalData                                                       */
/*     pESPCB_ListHead                                                      */
/*     hHeap                                                                */
/*     TgtHandlerID                                                         */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcCreate(pcrparm)
PPARM_CREATE pcrparm;

{ /* Start of ShcCreate */

RC      rc = NO_ERROR;                   /* local return code */
PSIB    psib = NULL;                     /* Stream instance block */
PESPCB  pEspcb;                          /* -> to espcb for spcb key passed */
int     notfound = TRUE;                 /* loop control */
BOOL    bIOThreadCreated = FALSE;        /* local var for error processing */
BOOL    bHevStopCreated = FALSE;
BOOL    bHevCmdSyncCreated = FALSE;

  /*
   * locate the correct spcb
   * allocate sib
   * create a thread to read or write
   * set thread priority
   * call smhinit
   * add sib to list
   */
  /*
   * Locate the spcb; if not found use the generic spcb
   * Note: there is not checking for if this espcb can support
   * being the requested source or target.
   */

  if (!(rc = DosRequestMutexSem(hmtxGlobalData, SEM_INDEFINITE_WAIT)))
    { /* we have the mutex sem */

      pEspcb = ShFindEspcb(pcrparm->spcbkey);

      /* If espcb not found then get the last generic spcb installed and use it. */

      if (!pEspcb)
        { /* Get Generic spcb */

          rc = ERROR_INVALID_SPCBKEY;
          pEspcb = pESPCB_ListHead;
          while (pEspcb && notfound)
            { /* Loop thru espcbs */
              if (pEspcb->spcb.spcbkey.ulDataType == DATATYPE_GENERIC)
                {
                  notfound = FALSE;
                  rc = NO_ERROR;
                }
              else
                {
                  pEspcb = pEspcb->pnxtESPCB;
                }
            } /* Loop thru espcbs */
        } /* Get Generic spcb */

      /* If we have an espcb alloc a stream instance block and copy the espcb */

      if (!rc)
        {
          /*
           * Allocate a SIB and copy ESPCB
           */
          psib = (PSIB)HhpAllocMem(hHeap, sizeof(SIB));
          if (psib)
            { /* SIB allocated */

              psib->espcb = *pEspcb;              /* copy entire structure */
              /* copy over data subtype.  SSM needs this particular field even if */
              /*  it is generic.  In most cases it is already the same. */
              psib->espcb.spcb.spcbkey.ulDataSubType = pcrparm->spcbkey.ulDataSubType;

            } /* SIB allocated */
          else
            { /* Error allocating sib */

              rc = ERROR_ALLOC_RESOURCES;

            } /* Error allocating sib */
        }
      DosReleaseMutexSem(hmtxGlobalData);
    } /* we have the mutex sem */

  if (!rc)
    { /* we have sib and espcb */

      /* Now create the semaphores for starting and stopping the IO thread */
      /* and for command synchronization. */

      rc = DosCreateEventSem(NULL, &psib->hevStop, 0L, FALSE);
      if (!rc)
        {
          bHevStopCreated = TRUE;
          rc = DosCreateEventSem(NULL, &psib->hevCmdSync, 0L, FALSE);
          if (!rc)
            bHevCmdSyncCreated = TRUE;
        }

      if (!rc)
        {
          psib->HandlerID = pcrparm->hid;
          psib->hStream = pcrparm->hstream;
          psib->ulActionFlags = 0;
          psib->ProcessID = HhpGetPID();
          pcrparm->pspcb = &psib->espcb.spcb;           /* return spcb address */
          psib->ulStatus = SIB_INITIALIZED;
          /*
           * Create the read/write thread and set it to Time Critical priority
           */
          if (pcrparm->hid == TgtHandlerID)
            {
              rc = DosCreateThread(&psib->ThreadID,
                                   ShWriteRtn,
                                   (ULONG)psib,
                                   0L,
                                   STACKSIZE);
            }
          else
            {
              rc = DosCreateThread(&psib->ThreadID,
                                   ShReadRtn,
                                   (ULONG)psib,
                                   0L,
                                   STACKSIZE);
            }
        }
      if (!rc)
        { /* IO Thread created */

          bIOThreadCreated = TRUE;
          DosSetPriority(PRTYS_THREAD,
                         PRTYC_TIMECRITICAL,
                         01L,
                         psib->ThreadID);
          /*
           * Now connect SIB to list, (the list is under semaphore
           * control).
           */
          rc = DosRequestMutexSem(hmtxGlobalData, SEM_INDEFINITE_WAIT);
          if (!rc)
            { /* Update SIB list */

              psib->pnxtSIB = pSIB_list_head;
              pSIB_list_head = psib;
              DosReleaseMutexSem(hmtxGlobalData);

            } /* Update SIB list */
        } /* IO Thread created */

      if (rc)
        { /* Error - Clean up */

          if (bIOThreadCreated)
            { /* kill the read write thread */

              psib->ulActionFlags |= SIBACTFLG_KILL;
              DosPostEventSem(psib->hevStop);
              DosWaitThread(&psib->ThreadID, DCWW_WAIT);

            } /* kill the read write thread */

          if (bHevStopCreated)
            {
              DosCloseEventSem(psib->hevStop);
            }

          if (bHevCmdSyncCreated)
            {
              DosCloseEventSem(psib->hevCmdSync);
            }

          DosRequestMutexSem(hmtxGlobalData, SEM_INDEFINITE_WAIT);
          HhpFreeMem(hHeap, psib);
          DosReleaseMutexSem(hmtxGlobalData);

        } /* Error - Clean up */

    } /* we have sib and espcb */

  return(rc);

} /* End of ShcCreate */

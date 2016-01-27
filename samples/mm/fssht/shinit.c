/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHINIT.C                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Dll Initialization routine             */
/*                                                                          */
/* FUNCTION: This function is called each time a new process loads this     */
/*           DLL.  If this is the first call, we must register with the     */
/*           SSM as a handler.  For each call, we need to allocate any      */
/*           resources needed on a process basis (GetSeg's, etc), and       */
/*           set up an exit list routine to deallocate these resources.     */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*   _DLL_InitTerm                                                          */
/*   ShExitList                                                             */
/*************************** END OF SPECIFICATIONS **************************/

#ifdef MMRAS
#define MMRAS_PTRACE
#endif
#ifdef PTRACE
#define MMRAS_PTRACE
#endif

#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS               /* For exit list defines        */
#define  INCL_DOSERRORS
#define  INCL_DOSMISC
#define  INCL_DOSDEVICES
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

#ifdef MMRAS_PTRACE
#include <ptrace.h>
#endif


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShInit                                                  */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Initialization routine                  */
/*                                                                          */
/* FUNCTION: DLL initialization routine, to set up control blocks, allocate */
/*           storage etc.                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: _DLL_InitTerm                                               */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   HMODULE hmod        Module handle                                      */
/*   ULONG   fTerm       DLL termination flag                               */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_ALLOC_RESOURCES                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     _CRT_init                                                            */
/*     DosCreateMutexSem                                                    */
/*     DosOpenMutexSem                                                      */
/*     DosRequestMutexSem                                                   */
/*     DosCloseMutexSem                                                     */
/*     SMHEntryPoint                                                        */
/*     HhpCreateHeap                                                        */
/*     HhpAccessHeap                                                        */
/*     DosExitList                                                          */
/*     HhpReleaseHeap                                                       */
/*     DosReleaseMutexSem                                                   */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     hmtxGlobalData                                                       */
/*     hHeap                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
INT _CRT_init(VOID);

ULONG _DLL_InitTerm( HMODULE hmod, ULONG fTerm)

{ /* Start of ShInit */

RC rc = NO_ERROR;                       /* local return code */
PARM_REG smhRegParm;                    /* Parameters for SMH_REGISTER */
int Registered = FALSE;
int HeapAllocated_Attached = FALSE;
int GlobDataMtxCreated = FALSE;
hmod;

  /*
   * Checking this parameter will insure that this routine will only
   * be run on an actual initialization.  Return success from the
   * termination.
   */

  if (fTerm)
     {
     return (1L);
     }

  if (_CRT_init())
     {
     return (0L);
     }

  /*
   * Get the semaphore controlling the process count & update the process
   * count if successful.  Then after we have the sem, if the count is
   * one, we are guaranteed that we must do the global initializations.
   *
   * There is a problem that to determine if we need to create or open the
   * semaphore we need to check the ProcessCount to see if this is the
   * first process, but since we don't have the semaphore we don't have
   * a guarentee the count won't change.  If we get caught in this window
   * then we will fail to get the semaphore and the rc will indicate error.
   */
  if (ulProcessCount == 0)
    { /* First process */

      if (!(rc = DosCreateMutexSem(pszProcCntMutexName,
                                   &hmtxProcCnt,
                                   0L,
                                   TRUE)))
        {
          ulProcessCount++;
        }
    } /* First process */
  else
    { /* not first process */
      /* if a process exits and decrements ProcessCount before we get  */
      /* the semaphore here, we will fail. */
      if (!(rc = DosOpenMutexSem(pszProcCntMutexName,
                                 &hmtxProcCnt)))
        {
          if (!(rc = DosRequestMutexSem(hmtxProcCnt,
                                        SEM_INDEFINITE_WAIT)))
            {
              ulProcessCount++;
            }
          else
            {
              DosCloseMutexSem(hmtxProcCnt);
            }
        }
    } /* not first process */

  if (!rc)
  { /* Semaphore ok, In critical section */
    /*
     * If this is the first time this init routine is called then we are
     * being brought in by the loader the first time, so we need to register
     * with the SSM.
     */
    if (ulProcessCount == 1)
      { /* First process */
        smhRegParm.ulFunction = SMH_REGISTER;
        smhRegParm.pszSHName = pszHandlerName;
        smhRegParm.phidSrc = &SrcHandlerID;
        smhRegParm.phidTgt = &TgtHandlerID;
        smhRegParm.pshcfnEntry = ( PSHCFN ) ShcRouter;

        smhRegParm.ulFlags = 0L;
        if (ulHandlerFlags & HANDLER_CAN_BE_SOURCE)
          {
            smhRegParm.ulFlags = REGISTER_SRC_HNDLR;
          }
        if (ulHandlerFlags & HANDLER_CAN_BE_TARGET)
          {
            smhRegParm.ulFlags |= REGISTER_TGT_HNDLR;
          }
        if (ulHandlerFlags & HANDLER_IS_NONSTREAMING)
          {
            smhRegParm.ulFlags |= REGISTER_NONSTREAMING;
          }

        rc = SMHEntryPoint((PVOID)&smhRegParm);
        /*
         * If ok then allocate our memory pool (heap), since it is under
         * semaphore control create and get the semaphore first.
         */
        if (!rc)
          { /* Register ok */
            Registered = TRUE;
            hHeap = HhpCreateHeap(HEAPSIZE,
                                  HH_SHARED);
            if (hHeap)
              { /* Heap Allocated */
                HeapAllocated_Attached = TRUE;
                if (!(rc = DosCreateMutexSem(NULL,
                                             &hmtxGlobalData,
                                             DC_SEM_SHARED,
                                             FALSE)))
                  {
                    GlobDataMtxCreated = TRUE;
                  }
              } /* Heap Allocated */
            else
              { /* Heap not allocated */
                rc = ERROR_ALLOC_RESOURCES;
              } /* Heap not allocated */
          } /* Register ok */
      } /* First Process */
    else
      { /* Not first process */
        if (!(rc = DosOpenMutexSem(NULL,
                                   &hmtxGlobalData)))
          { /* Global data semaphore opened */

            GlobDataMtxCreated = TRUE;

            if (!ENTERCRIT(rc))
              { /* Global Data Sem obtained */

                if (HhpAccessHeap(hHeap, HhpGetPID()))
                  { /* Error accessing heap */
                    rc = ERROR_ALLOC_RESOURCES;
                  } /* Error accessing heap */
                else
                  { /* Heap attached */
                    HeapAllocated_Attached = TRUE;
                  } /* Heap attached */

                EXITCRIT;

              } /* Global Data Sem obtained */
          } /* Global data semaphore opened */
      } /* Not first process */

    /*
     * If things are ok, Register an exit list handler and if that works
     * increment the process count.
     */
    if (!rc)
      {
        rc = DosExitList(EXTLSTADD_ORDER,
                         (PFNEXITLIST)ShExitList);
      }

    if (rc)
      { /* Error occurred - Clean Up */
        if (HeapAllocated_Attached)
          {
            HhpReleaseHeap(hHeap,
                           HhpGetPID());
          }
        if (GlobDataMtxCreated)
          {
            DosCloseMutexSem(hmtxGlobalData);
          }
        if (Registered)
          { /* Deregister */
            PARM_DEREG smhDeregParm;

            smhDeregParm.ulFunction = SMH_DEREGISTER;
            smhDeregParm.pszSHName = pszHandlerName;
            SMHEntryPoint(&smhDeregParm);
          } /* Deregister */
        ulProcessCount--;
        DosReleaseMutexSem(hmtxProcCnt);
        DosCloseMutexSem(hmtxProcCnt);
      } /* Error occurred - Clean Up */
    else
      {
#ifdef MMRAS_PTRACE
        InitPTrace()
#endif
        DosReleaseMutexSem(hmtxProcCnt);
      }

  } /* Semaphore ok, In critical section */
  /*
   * The return codes expected are:
   *  TRUE (any non-zero) - init routine worked
   *  FALSE (zero)        - init routine failed
   *
   * So we have to reverse the local rc before we pass it back.
   */
  return(!rc);

} /* End of SHInit */

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShExitList                                              */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Hanlder Exit List routine                       */
/*                                                                          */
/* FUNCTION: Clean up resources allocated for each process, and if this is  */
/*           the last process, then deregister with the SSM.                */
/*           In all cases we need to destroy all streams owned by this      */
/*           process.                                                       */
/*                                                                          */
/* ENTRY POINT:                                                             */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   None                                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     HhpGetPID                                                            */
/*     DosRequestMutexSem                                                   */
/*     DosCloseMutexSem                                                     */
/*     SMHEntryPoint                                                        */
/*     HhpShcDestroy                                                        */
/*     DosExitList                                                          */
/*     HhpReleaseHeap                                                       */
/*     DosReleaseMutexSem                                                   */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     hmtxGlobalData                                                       */
/*     hHeap                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

VOID APIENTRY ShExitList()

{ /* Start of ShExitList */

PSIB psib;                              /* Stream instance block */
PSIB psibToDestroy = NULL;              /* Stream instance block to destroy */
PARM_DESTROY dyparm;                    /* parameter block to destroy routine */
PID  pid;                               /* Process ID */
RC   rc;                                /* Return code */

  /*
   * There is nothing we can do if calls fail, so don't bother checking.
   */

  DosRequestMutexSem(hmtxProcCnt, SEM_INDEFINITE_WAIT);

  /*
   * If this is the last process we will be removed from memory, so
   * call SSM to deregister.
   */

  if (ulProcessCount == 1)
    {
      PARM_DEREG smhDeregParm;

      smhDeregParm.ulFunction = SMH_DEREGISTER;
      smhDeregParm.pszSHName = pszHandlerName;
      SMHEntryPoint(&smhDeregParm);
    }

  /* We need to call destroy for each stream created by this process. */
  /*   Get the PID, and the global data semaphore. */
  /*   Search the SIB chain for this process's stream instances. */
  /*   Call destroy to destroy the stream instances */
  /*   Release the global data semaphore */

  pid = HhpGetPID();

  ENTERCRIT(rc);
  psib = pSIB_list_head;
  while (psib)
    {
      if (psib->ProcessID == pid)
        {
          psibToDestroy = psib;
          psib = psib->pnxtSIB;
          if (psibToDestroy->ThreadID)
            {
              psibToDestroy->ulActionFlags |= SIBACTFLG_THREAD_DEAD;
            }
          dyparm.hstream = psibToDestroy->hStream;
          dyparm.hid = psibToDestroy->HandlerID;
          ShcDestroy(&dyparm);
        }
      else
        {
          psib = psib->pnxtSIB;
        }
    }

  /*
   * Unattach from the memory pool, and from the Global data semaphore
   */
  HhpReleaseHeap(hHeap, pid);
  EXITCRIT;
  DosCloseMutexSem(hmtxGlobalData);

  /* Clean up the process count variable and it's semaphore. */

  ulProcessCount--;
  DosReleaseMutexSem(hmtxProcCnt);
  DosCloseMutexSem(hmtxProcCnt);

  DosExitList(EXLST_EXIT, (PFNEXITLIST)ShExitList);

} /* End of ShExitList */

/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHDESTRY.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Destroy stream routine                 */
/*                                                                          */
/* FUNCTION: This function destroys a stream instance.                      */
/*                                                                          */
/* ENTRY POINTS: ShcDestroy                                                 */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcDestroy                                              */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Destroy stream routine          */
/*                                                                          */
/* FUNCTION: Destroys the specified stream.  Which means it takes the       */
/*           stream instance block off the active chain; it signals the     */
/*           worker thread to end (SIBACTFLG_KILL) and waits for it; then it*/
/*           deallocates the sib.  At this point the stream is gone from    */
/*           this stream handler.                                           */
/*                                                                          */
/* ENTRY POINT: ShcDestroy                                                  */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to Destroy SHC paramater block (PARM_DESTROY).  It        */
/*   contains:                                                              */
/*     ULONG    ulFunction;           Handler command function (SHC_DESTROY)*/
/*     HID      hid;                  handler ID                            */
/*     HSTREAM  hstream;              handle of stream instance             */
/*                                                                          */
/* EXIT-NORMAL: 0 NO_ERROR                                                  */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_STREAM                                                   */
/*   Errors from:                                                           */
/*     DosRequestMutexSem                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*   All information known to this stream handler about the stream is gone. */
/*   This includes the worker read/write thread and the stream instance     */
/*   block.  The stream is destroyed.                                       */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*    HhpFreeMem - Free memory (sib)                                        */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*    pHeap - Pointer to Stream handler heap.                               */
/*    hmtxGlobalData - handle of mutex semaphore to control global data     */
/*      access.  In this case the control of the sib chain, and control of  */
/*      the heap control blocks.                                            */
/*                                                                          */
/* NOTES: Filter stream handlers only have 1 worker thread for 2 streams.   */
/*   This is indicated by a NULL (0) sib.ThreadID for the second stream.    */
/*   So when destroy is called for this second stream (sib.threadid = 0)    */
/*   we skip the code to end the thread since there one does not exist.     */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcDestroy(pdyparm)
PPARM_DESTROY pdyparm;

{ /* Start of ShcDestroy */

RC rc = NO_ERROR;                       /* local return code */
PSIB psib;                              /* Stream instance block */
PSIB psibPrev = NULL;                   /* previous Stream instance block in */
                                        /* chain */

  /*
   * Find our Stream Instance Block and take it off the list
   */
  ENTERCRITX(rc);

  psib = pSIB_list_head;
  while ((psib) &&
         !((psib->hStream == pdyparm->hstream) && (psib->HandlerID == pdyparm->hid)))
    {
      psibPrev = psib;
      psib = psib->pnxtSIB;
    }

  /* If the sib was found take it off the chain.  If it was not found the */
  /* stream handle and id is invalid. */

  if (psib)
    { /* Take the sib off the chain */

      /* If previous sib in chain then take this sib out.  If no previous */
      /*  sib then this is the first sib in the chain so update the list */
      /*  head. */

      if (psibPrev)
        {
          psibPrev->pnxtSIB = psib->pnxtSIB;
        }
      else
        {
          pSIB_list_head = psib->pnxtSIB;
        }
    } /* Take the sib off the chain */
  else
    {
      rc = ERROR_INVALID_STREAM;
    }

  EXITCRIT;

  if (!rc)
    { /* We have SIB */

      /* First kill the read write thread */

      /* Check sib threadid to make sure there is a thread for this stream */
      /* before resume and waiting for it. */

      DestroySIB(psib);
    } /* We have SIB */

  return(rc);

} /* End of ShcDestroy */

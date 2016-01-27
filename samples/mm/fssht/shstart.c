/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHSTART.C                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Start stream routine                   */
/*                                                                          */
/* FUNCTION: This function starts a stream instance.                        */
/*                                                                          */
/* ENTRY POINTS: ShcStart                                                   */
/*                                                                          */
/* PvtComment: All lines that you do not want to go in the toolkit fssht    */
/* PvtComment: need to contain a string "PvtComment:"                       */
/* PvtComment: These lines are taken out using sed in fssht.mak - Vivek     */
/*                                                                          */
/* PvtComment: Change History                                                           */
/* PvtComment: DATE     DEVELOPER      CHANGE DESCRIPTION                               */
/* PvtComment: ????     ??????????     Original creation                                */
/* PvtComment: 09/17/93 Vivek Nirkhe   Def 10814 - thread suspend problem on pause      */
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
/* SUBROUTINE NAME: ShcStart                                                */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Start stream routine            */
/*                                                                          */
/* FUNCTION: This routine kick starts the IO thread to start reading or     */
/*   writing.  It may be called indirectly from SpiStartStream, or by the   */
/*   stream manager buffer control code.  If we had to block because no     */
/*   buffers were available, then the buffer control code will send a start.*/
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcStart                                                    */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc start parameter block (PARM_START) containing:     */
/*   ULONG   ulFunction  Handler command function SHC_START                 */
/*   HID     hid         handler ID                                         */
/*   HSTREAM hstream     handle of stream instance                          */
/*   ULONG   ulFlags     Stop flags (see SSM.H)                             */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_DATA_ITEM_NOT_SPECIFIED                                          */
/*   ERROR_INVALID_SEQUENCE                                                 */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*   The IO thread is unblocked.                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShFindSib                                                            */
/*     DosResumeThread                                                      */
/*     DosPostEventSem                                                      */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcStart(pstparm)
PPARM_START pstparm;

{ /* Start of ShcStart */

RC rc = NO_ERROR;                       /* local return code */
PSIB psib;                              /* Stream instance block */
BOOL bPaused;                           /* Is stream paused? */

  /*
   * Find our Stream Instance Block
   */
  if (!(rc = ShFindSib(&psib, pstparm->hstream, pstparm->hid)))
    { /* We have SIB */
      /* error if we don't have something to stream */
      if (psib->ulStatus < SIB_RUNNING)
        {
          rc = ERROR_DATA_ITEM_NOT_SPECIFIED;
        }
      else
        {
          if (DEAD(psib))
            {
              rc = ERROR_INVALID_SEQUENCE;
            }
          else
            { /* command sequence ok */

              bPaused = (psib->ulStatus == SIB_PAUSED);
              psib->ulStatus = SIB_RUNNING;
              if (psib->ThreadID)
                {
                  DosPostEventSem(psib->hevStop);
                  if (bPaused)
                    {
                      // PvtComment: Eventually all stream handlers that use common code should
                      // PvtComment: use SYNC_SEND_MSG. At that time, get rid of the else part.
                      // PvtComment: and remove the flag from makefiles.
#ifdef SYNCPAUSE
                      // PvtComment: Def 10814
                      // PvtComment: Using semaphore wait to stop the read thread and
                      // PvtComment: not suspending it. This should solve Mountain View problem
                      SEND_MSG(psib,0L,0L);
#else
                      // PvtComment: Old mechanism
                      DosResumeThread(psib->ThreadID);  //PvtComment:
#endif
                    }
                }
            } /* command sequence ok */
        }
    } /* We have SIB */

  return(rc);

} /* End of ShcStart */

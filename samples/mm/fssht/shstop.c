/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHSTOP.C                                               */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Stop stream routine                    */
/*                                                                          */
/* FUNCTION: This function stops a stream instance.                         */
/*                                                                          */
/* ENTRY POINTS: ShcStop                                                    */
/*                                                                          */
/* PvtComment: All lines that you do not want to go in the toolkit fssht    */
/* PvtComment: need to contain a string "PvtComment:"                       */
/* PvtComment: These lines are taken out using sed in fssht.mak - Vivek     */
/*                                                                          */
/* PvtComment: Change History                                                           */
/* PvtComment: DATE     DEVELOPER      CHANGE DESCRIPTION                               */
/* PvtComment: ????     ??????????     Original creation                                */
/* PvtComment: 09/17/93 Vivek Nirkhe   Def 10814 - thread suspend problem on pause      */
/* PvtComment: 10/14/93 Vivek Nirkhe   Def 11482 - Record to Notify problem with flush */
/*************************** END OF SPECIFICATIONS **************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

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
/* SUBROUTINE NAME: ShcStop                                                 */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Stop stream routine             */
/*                                                                          */
/* FUNCTION: This stops the specified stream.  The flags passed indicate    */
/*           if this should discard all buffers or not.                     */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcStop                                                     */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc stop parameter block (PARM_STOP) containing:       */
/*   ULONG   ulFunction  Handler command function SHC_STOP                  */
/*   HID     hid         handler ID                                         */
/*   HSTREAM hstream     handle of stream instance                          */
/*   ULONG   ulFlags     Stop flags (see SSM.H)                             */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   None                                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*   The IO thread is blocked.                                              */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShFindSib                                                            */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcStop(pstparm)
PPARM_STOP pstparm;

{ /* Start of ShcStop */

RC    rc = NO_ERROR;                    /* local return code */
PSIB  psib;                             /* Stream instance block */


  /*
   * Find our Stream Instance Block
   */
  if (!(rc = ShFindSib(&psib, pstparm->hstream, pstparm->hid)))
    { /* We have SIB */

      /* Don't bother checking most states since the stream manager does this */
      /* for us before the command gets here. */

      /* If this is stop Flush and we are the target then we will stop when we */
      /* run out of buffers.  So ignore the stop command. */

      if (!((pstparm->ulFlags & SPI_STOP_FLUSH) && (pstparm->hid == TgtHandlerID)))
        { /* not flush and target handler */
          if (pstparm->ulFlags & (SPI_STOP_DISCARD | SPI_STOP_FLUSH))
            { /* We must release all buffers */
              if (psib->ThreadID)
                {
                  /*  Update action flags to stop the independent thread, and wait on the */
                  /*  command sync semaphore to wait until it gets the command. */

                  if (pstparm->ulFlags & SPI_STOP_DISCARD)
                    SYNC_SEND_MSG(psib,SIBACTFLG_STOP_DISCARD,SIB_RUNNING);

                  if (pstparm->ulFlags & SPI_STOP_FLUSH)
                    SYNC_SEND_MSG(psib,SIBACTFLG_STOP_FLUSH,SIB_RUNNING);
                }

              else
                {
                  if (pstparm->ulFlags & SPI_STOP_DISCARD)
                    psib->ulActionFlags |= SIBACTFLG_STOP_DISCARD;
                  if (pstparm->ulFlags & SPI_STOP_FLUSH)
                    psib->ulActionFlags |= SIBACTFLG_STOP_FLUSH;
                }
            } /* We must release all buffers */
          else
            { /* Assume stop pause command*/

              if (psib->ulStatus == SIB_RUNNING)
                {
                  psib->ulStatus = SIB_PAUSED;
                  // PvtComment: Eventually all stream handlers that use common code should
                  // PvtComment: use SYNC_SEND_MSG. At that time, get rid of the else part.
                  // PvtComment: and remove the flag from makefiles.
#ifdef SYNCPAUSE
                  // PvtComment: Def 10814
                  // PvtComment: Using semaphore wait to stop the read thread and
                  // PvtComment: not suspending it. This should solve Mountain View problem
                  SYNC_SEND_MSG(psib,SIBACTFLG_STOP_PAUSE,0L);
#else
                  // PvtComment: Old mechanism
                  SUSPEND_IO(psib); // PvtComment:
#endif
                }
            } /* Assume stop pause command*/
        } /* not flush and target handler */

      else
        {
          SYNC_SEND_MSG(psib,SIBACTFLG_STOP_FLUSH,SIB_RUNNING);
          // PvtComment: Defect 11482 - Simple resume would not unblock the write thread
          // PvtComment: from the hevStop semaphore.
          // PvtComment: RESUME_IO(psib,SIB_RUNNING);            /* Flush the target */
        }
    } /* We have SIB */



  return(rc);

} /* End of ShcStop */

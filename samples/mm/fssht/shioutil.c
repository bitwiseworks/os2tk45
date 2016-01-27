/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHIOUTIL.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Input Output Utility routines.         */
/*                                                                          */
/* FUNCTION: Utility functions for ring 3 stream handlers.                  */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*   SleepNCheck                                                            */
/*   CheckNSleep                                                            */
/*   ReportEvent                                                            */
/*   ShIOError                                                              */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_DOSERRORS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: SleepNCheck                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Sleep and check action flags when woken  up.           */
/*                                                                          */
/* FUNCTION: Suspend execution and when resumed, do checks for action flags */
/*           of stop discard, flush, and check the kill flag.               */
/*                                                                          */
/* NOTES: This is called by the shread and shwrite routines.                */
/*                                                                          */
/* ENTRY POINT: SleepNCheck                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   psib - stream instance block                                           */
/*   pulStopped - pointer to stopped variable.                              */
/*                On input it may have 3 possible values.                   */
/*                  DO_SLEEP            - sleep in all cases                */
/*                  DONT_SLEEP          - do not sleep, just check flags    */
/*                  DONT_SLEEP_FLUSH    - Wake up for flush stops           */
/*                  DONT_SLEEP_DISCARD  - Wake up for discard stops         */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*   pulStopped - pointer to output stopped variable.                       */
/*                On output it may have the following output values:        */
/*                  AWAKEN_BY_OTHER        - woke up by kill action         */
/*                  AWAKEN_BY_FLUSH_STOP   - woke up by flush stop          */
/*                  AWAKEN_BY_DISCARD_STOP - woke up by discard stop        */
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
/*   ROUTINES: None                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     sib                                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#ifdef INCL_NEWSH

RC SleepNCheck ( PSIB psib,
                 PULONG pulStopped )

{ /* Start of SleepNCheck */

   RC    rc = 0L;
   BOOL  KeepSleeping = TRUE;
   ULONG ulStopped = *pulStopped;     /* Save Flags */

   *pulStopped = AWAKEN_BY_OTHER;     /* Reset the output */

   /**********************************************************************/
   /* While no error (or Destroy/Kill) and Stop specified (KeepSleeping) */
   /**********************************************************************/
   while (!rc && KeepSleeping) { /* Suspend */

      KeepSleeping = FALSE;

      if (!psib->ulActionFlags) {
         RECEIVE_MSG(psib,rc);

         if (!rc) {
            RESET_MSG(psib);
            }
         }

      if (!rc && psib->ulActionFlags) { /* Somethings up */

         if (psib->ulActionFlags & (SIBACTFLG_STOP_DISCARD | SIBACTFLG_STOP_FLUSH | SIBACTFLG_STOP_PAUSE)) {

            /************************************************************/
            /* If we were awakend and told to STOP then since we don't  */
            /* own any buffers just keep sleeping (loop and resuspend). */ 
            /* We need to clear the STOP flags so when we are started   */ 
            /* we don't just stop again.                                */ 
            /*                                                          */ 
            /* Also set the pulStopped to TRUE so our caller knows we   */ 
            /* were explictly stopped by the calling application.       */
            /************************************************************/

            if (psib->ulActionFlags & SIBACTFLG_STOP_FLUSH) {      /* FLUSH STOP */
               psib->ulActionFlags &= ~(SIBACTFLG_STOP_FLUSH);
               if (ulStopped & DONT_SLEEP_FLUSH)
                  *pulStopped = AWAKEN_BY_FLUSH_STOP;
               else
                  KeepSleeping = TRUE;
               }

            else if (psib->ulActionFlags & SIBACTFLG_STOP_DISCARD) {  /* DISCARD STOP */
               psib->ulActionFlags &= ~(SIBACTFLG_STOP_DISCARD);
               if (ulStopped & DONT_SLEEP_DISCARD)
                  *pulStopped = AWAKEN_BY_DISCARD_STOP;
               else
                  KeepSleeping = TRUE;
               }

            else {        /* PAUSE STOP */
               psib->ulActionFlags &= ~(SIBACTFLG_STOP_PAUSE);
               KeepSleeping = TRUE;
               }

            /*************************************************************/ 
            /* Let calling thread know we are done with the stop command */ 
            /*************************************************************/
            SEND_REPLY(psib);
            }

         /**********************************************************************/
         /* If Destroy was sent then the KILL flag should be set.  Check it    */
         /*  and set "rc" to SIBACTFLG_KILL so we will exit the main loop and  */
         /*  this function.  The ulActionFlags will still have the             */
         /*  SIBACTFLG_KILLset so the calling routine can tell if the rc was   */
         /*  a severe error or a Destroy KILL.                                 */
         /**********************************************************************/
         rc = (psib->ulActionFlags & SIBACTFLG_KILL);
         } /* Somethings up */
      } /* Suspend */

   return(rc);

} /* End of SleepNCheck */


#else
/*****************************************************************************/
RC SleepNCheck(psib, pulStopped)

PSIB psib;
PULONG pulStopped;            // were stop flags encountered.

{ /* Start of SleepNCheck */

  RC    rc = 0L;
  BOOL  KeepSleeping = TRUE;

  //
  // While no error (or Destroy/Kill) and Stop specified (KeepSleeping)
  //
  while (!rc && KeepSleeping)
    { /* Suspend */

      KeepSleeping = FALSE;
      if (!psib->ulActionFlags)
        {
          RECEIVE_MSG(psib,rc);
          if (!rc) {
             RESET_MSG(psib);
             }
        }
      if (!rc && psib->ulActionFlags)
        { /* Somethings up */
          if (psib->ulActionFlags & (SIBACTFLG_STOP_DISCARD | SIBACTFLG_STOP_FLUSH | SIBACTFLG_STOP_PAUSE))
            {
              //
              // If we were awakend and told to STOP then since we don't
              // own any buffers just keep sleeping (loop and resuspend).
              // We need to clear the STOP flags so when we are started
              // we don't just stop again.
              //
              // Also set the pulStopped to TRUE so our caller knows we
              // were explictly stopped by the calling application.
              //

              if (psib->ulActionFlags & SIBACTFLG_STOP_FLUSH)       /* FLUSH STOP */
                {
                  psib->ulActionFlags &= ~(SIBACTFLG_STOP_FLUSH);

                  if (*pulStopped != DONT_SLEEP_FLUSH)
                    {
                      KeepSleeping = TRUE;
                      *pulStopped = TRUE;
                    }
                }
              else
                { /* discard or pause */
                  KeepSleeping = TRUE;
                  if (psib->ulActionFlags & SIBACTFLG_STOP_DISCARD)
                    {
                      psib->ulActionFlags &= ~(SIBACTFLG_STOP_DISCARD);
                      *pulStopped = TRUE;
                    }
                  else
                    { /* stop pause */
                      psib->ulActionFlags &= ~(SIBACTFLG_STOP_PAUSE);
                      *pulStopped = FALSE;
                    } /* stop pause */
                } /* discard or pause*/

              //
              // Let calling thread know we are done with the stop command
              //
              SEND_REPLY(psib);
            }
          //
          // If Destroy was sent then the KILL flag should be set.  Check it
          //  and set "rc" to SIBACTFLG_KILL so we will exit the main loop and
          //  this function.  The ulActionFlags will still have the
          //  SIBACTFLG_KILLset so the calling routine can tell if the rc was
          //  a severe error or a Destroy KILL.
          //
          rc = (psib->ulActionFlags & SIBACTFLG_KILL);
        } /* Somethings up */
    } /* Suspend */

  return(rc);

} /* End of SleepNCheck */
#endif



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: CheckNSleep                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Check actions flags and sleep if stop command.         */
/*                                                                          */
/* FUNCTION: Check the action flags (stop discard, flush and kill) and then */
/*           suspend if needed.  When resumed, check the flags again.       */
/*                                                                          */
/* NOTES: This is called by the shread and shwrite routines.                */
/*  Also, this will not get a stop flush on a target thread since shcstop   */
/*  code filters that out and ignores it.                                   */
/*                                                                          */
/* ENTRY POINT: CheckNSleep                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   psib - stream instance block                                           */
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
/*   ROUTINES: None                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     sib                                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC CheckNSleep(psib)

PSIB psib;

{ /* Start of CheckNSleep */

  RC    rc;
  ULONG ulPostCount;

  rc = (psib->ulActionFlags & SIBACTFLG_KILL);
  /* suspend if stop command */
  while (!rc && (psib->ulActionFlags & (SIBACTFLG_STOP_DISCARD | SIBACTFLG_STOP_FLUSH | SIBACTFLG_STOP_PAUSE)))
    { /* Suspend */
      psib->ulActionFlags &= ~(SIBACTFLG_STOP_DISCARD | SIBACTFLG_STOP_FLUSH | SIBACTFLG_STOP_PAUSE);
      RESET_MSG(psib);
      SEND_REPLY(psib);
      RECEIVE_MSG(psib,rc);
      rc = (psib->ulActionFlags & SIBACTFLG_KILL);
    } /* Suspend */

  return(rc);

} /* End of CheckNSleep */

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ReportEvent                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Report an Error Event to the stream manager            */
/*                                                                          */
/* FUNCTION: Report an event of the type passed in to the stream manager.   */
/*                                                                          */
/* NOTES: This is called by the shread and shwrite routines.                */
/*                                                                          */
/* ENTRY POINT: ReportEvent                                                 */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   psib         - stream instance block                                   */
/*   ulEventRc    - Event return code                                       */
/*   ulEventType  - Event type                                              */
/*   ulUserParm   - User parameter                                          */
/*   ulFlags      - flags                                                   */
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
/*   ROUTINES: None                                                         */
/*    SMHEntryPoint                                                         */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ReportEvent(psib, ulEventRc, ulEventType, ulUserParm, ulFlags)

PSIB   psib;
RC     ulEventRc;
ULONG  ulEventType;
ULONG  ulUserParm;
ULONG  ulFlags;                     /* Error severity */

{ /* Start of ReportEvent */

  PARM_EVENT parmev;
  IMPL_EVCB  ImplEvcb;
  PLAYL_EVCB PlayEvcb;
  RC         rc;

  parmev.ulFunction = SMH_REPORTEVENT;
  parmev.hid = psib->HandlerID;
  parmev.hevent = 0L;

  if ((ulEventType == EVENT_PLAYLISTMESSAGE) ||
      (ulEventType == EVENT_PLAYLISTCUEPOINT))
    { /* use playlist evcb */

      parmev.pevcbEvent = (PEVCB)&PlayEvcb;
      PlayEvcb.ulType = EVENT_IMPLICIT_TYPE;
      PlayEvcb.ulSubType = ulEventType;
      PlayEvcb.ulFlags = ulFlags;
      PlayEvcb.hstream = psib->hStream;
      PlayEvcb.hid = psib->HandlerID;
      PlayEvcb.ulStatus = ulEventRc;
      PlayEvcb.ulMessageParm = ulUserParm;
      PlayEvcb.unused1 = 0L;
      PlayEvcb.unused2 = 0L;

    } /* use playlist evcb */
  else
    { /* use implicit evcb */

      parmev.pevcbEvent = (PEVCB)&ImplEvcb;
      ImplEvcb.ulType = EVENT_IMPLICIT_TYPE;
      ImplEvcb.ulSubType = ulEventType;
      ImplEvcb.ulFlags = ulFlags;
      ImplEvcb.hstream = psib->hStream;
      ImplEvcb.hid = psib->HandlerID;
      ImplEvcb.ulStatus = ulEventRc;
      ImplEvcb.unused1 = 0L;
      ImplEvcb.unused2 = 0L;
      ImplEvcb.unused3 = 0L;

    } /* use implicit evcb */
  rc = SMHEntryPoint(&parmev);

  return(rc);

} /* End of ReportEvent */

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShIOError                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Handle an IO Error in stream handler                   */
/*                                                                          */
/* FUNCTION: Routine to handle IO error in the ring3 stream handler read    */
/*           and write routines.                                            */
/*                                                                          */
/* NOTES: This is called by the shread and shwrite routines.                */
/*                                                                          */
/* ENTRY POINT: ShIOError                                                   */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*   psib         - stream instance block                                   */
/*   npret        - Notify parameter block                                  */
/*   retcode      - return code                                             */
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
/*   ROUTINES: None                                                         */
/*     SMHEntryPoint                                                        */
/*     CheckNSleep                                                          */
/*     SleepNCheck                                                          */
/*     ReportEvent                                                          */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     sib                                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShIOError(psib, npret, retcode)

PSIB        psib;
PARM_NOTIFY npret;
RC          retcode;

{ /* Start of ShIOError */

  RC         saverc;
  RC         rc = NO_ERROR;
  ULONG      ulStopped = DO_SLEEP;

  /* Return the buffer */

  saverc = SMHEntryPoint(&npret);

  if (psib->ulActionFlags)
    { /* Somethings up */

      rc = CheckNSleep(psib);

    } /* Somethings up */
  else
    { /* Report I/O error */

      /* Clear the stop sem, so if the app clears the error during */
      /* the report and sends a start, then we won't go to sleep */

      RESET_MSG(psib);

      if (!(rc = ReportEvent(psib,
                             retcode,          /* Return code */
                             EVENT_ERROR,      /* event type */
                             0L,               /* user info */
                             RECOVERABLE_ERROR)))  /* Must get start */
        { /* report event ok */

          rc = SleepNCheck(psib, &ulStopped);

        } /* report event ok */
    } /* Report I/O error */
  if (!rc)
    rc = saverc; /* restore return empty RC */

  return(rc);

} /* End of ShIOError */

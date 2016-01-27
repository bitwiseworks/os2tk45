/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: FSSHWRIT.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  File System Stream Handler Write routine              */
/*                                                                          */
/* FUNCTION: This function is the routine that writes an object to the      */
/*           file system.                                                   */
/*                                                                          */
/* ENTRY POINTS: FsshWrite                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_DOSERRORS
#define  INCL_MMIOOS2
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: FsshWrite                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: File System Stream Handler Write routine               */
/*                                                                          */
/* FUNCTION: Gets data from source stream handler (actually SSM) and writes */
/*           it to the file system.                                         */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: FsshWrite                                                   */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to sib (stream instance block)                            */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_BUFFER_NOT_AVAILABLE                                             */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*        ROUTINES:                                                         */
/*          DosWaitEventSem                                                 */
/*          CheckNSleep                                                     */
/*          DosResetEventSem                                                */
/*          SMHEntryPoint                                                   */
/*          SleepNCheck                                                     */
/*          mmioWrite                                                       */
/*          ShIOError                                                       */
/*          mmioGetLastError                                                */
/*          ReportEvent                                                     */
/*        DATA STRUCTURES:                                                  */
/*          npget                                                           */
/*          sib                                                             */
/*          hmtxGlobalData                                                       */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

VOID FsshWrite(psib)
PSIB psib;                                /* Stream Instance Block */

{ /* Start of FsshWrite */

RC          rc = NO_ERROR;                /* local return code */
char        NeedBuf;                      /* local loop boolean */
LONG        NumBytesIO;                   /* Number of bytes from mmio */
PARM_NOTIFY npget, npret;                 /* parms for SMH_NOTIFY calls */
TGTBUFTAB   TgtBufTab = {0};              /* Target buffer table */
ULONG       ulStopped = DONT_SLEEP_FLUSH; /* did we get stop discard or flush */
BOOL        bAtEOS = FALSE;               /* End of Stream indicator */
ULONG       ulPostCount;                /* Temp to hold count */

  /* Before we start lets do some init stuff: */

  npget.ulFunction = SMH_NOTIFY;
  npget.hid = psib->HandlerID;
  npget.hstream = psib->hStream;
  npget.ulFlags = BUF_GETFULL;
  npget.ulGetNumEntries = 1L;
  npget.ulRetNumEntries = 0L;
  npget.pGetBufTab = &TgtBufTab;
  npget.pRetBufTab = NULL;

  npret.ulFunction = SMH_NOTIFY;
  npret.hid = psib->HandlerID;
  npret.hstream = psib->hStream;
  npret.ulFlags = BUF_RETURNEMPTY;
  npret.ulGetNumEntries = 0L;
  npret.ulRetNumEntries = 1L;
  npret.pGetBufTab = NULL;
  npret.pRetBufTab = &TgtBufTab;

  /* Wait until we get the ShcStart */

  DosWaitEventSem(psib->hevStop, SEM_INDEFINITE_WAIT);

  /* We will loop forever getting a full buffer, calling the file system to */
  /* write it out and returning the empty buffer to the stream manager. */
  /* During each iteration of the loop we will check the action flags for */
  /* asynchronous requests to do things. */

  if (psib->ulActionFlags & SIBACTFLG_KILL)
    { /* Must have been a create error */
      rc = 1L;
    } /* Must have been a create error */

  /*  Start the main loop */

  while (!rc)
    { /* while no severe error */

    if (psib->ulActionFlags)
    {

        /* A STOP_PAUSE at this point needs to be handled a little   */
        /* differently.  Needs to SleepNCheck instead of CheckNSleep */
        /* so that if a STOP_FLUSH or STOP_DISCARD comes in, it is   */
        /* handled gracefully by waking up from the sleep.           */

        if (psib->ulActionFlags & SIBACTFLG_STOP_PAUSE)
        {
           /* we need to wake up for start/kill/flush/discard */
           ulStopped = DONT_SLEEP_DISCARD | DONT_SLEEP_FLUSH;

           psib->ulActionFlags = 0L;    /* Reset this so SleepNCheck will sleep */

           RESET_MSG(psib);
           SEND_REPLY(psib);            /* Acknowledge Stop Pause request */

           rc = SleepNCheck (psib, &ulStopped);
        }
        else
        {
           rc = CheckNSleep (psib);
        }
    }

    /*
     * Get a buffer
     */
    NeedBuf = TRUE;
    while ((NeedBuf) && (!rc))
      { /* while we don't have a buffer */

        /* Clear the stop sem, so if after we call ssm to get a buffer if */
        /* it returns none avail then we won't miss a SSMBuffer Start before */
        /* we go to sleep. */

        DosResetEventSem(psib->hevStop, &ulPostCount);

        /* Make sure only GetFull flag is on and call ssm to get a buffer */

        npget.ulFlags = BUF_GETFULL;
        rc = SMHEntryPoint(&npget); /* get a buffer */
        if (!rc)
          {
            NeedBuf = FALSE;
            /* set return buffer EOS flag if get EOS is on */
            npret.ulFlags |= npget.ulFlags & BUF_EOS;
          }
        else
          { /* return code from smhnotify */
            if (rc == ERROR_BUFFER_NOT_AVAILABLE)
              { /* buffer not available */

                /* the smhnotify resets the num entries when none avail */
                npget.ulGetNumEntries = 1L;
                ulStopped = DONT_SLEEP_FLUSH;
                rc = SleepNCheck(psib, &ulStopped);

              } /* buffer not available */
          } /* return code from smhnotify */
      } /* while we don't have a buffer */

    /* We have a buffer or an error */

    if (!rc)
      { /* have a buffer */

        /* Check to see if we got a cuepoint in the buf attribute field. */
        /* If so, we need to report this event. */

        if (TgtBufTab.ulMessageParm)
          {
            rc = ReportEvent(psib,
                             0L,                        /* no return code */
                             EVENT_PLAYLISTCUEPOINT,    /* event type */
                             TgtBufTab.ulMessageParm,   /* user supplied value */
                             0L);

          }

        /* Make sure attribute is clear in all cases so it won't be passed */
        /* around the system inadvertently */

        TgtBufTab.ulMessageParm = 0L;

        /* If there is data in the buffer we received then write it out. */

        NumBytesIO = 0L;
        if (TgtBufTab.ulLength)
          {
            NumBytesIO = mmioWrite((HMMIO)psib->ulAssocP1,
                                   (PCHAR)TgtBufTab.pBuffer,
                                   (LONG)TgtBufTab.ulLength);
          }
        if ((NumBytesIO == -1L) ||
            (NumBytesIO != (LONG)TgtBufTab.ulLength))

          { /* an error */

            /* get the real error code */
            rc = mmioGetLastError((HMMIO)psib->ulAssocP1);

            rc = ShIOError(psib, npret, rc);

          } /* an error */
        else
          { /* We wrote ok */

            /* Set up to return buffer to stream manager */

            if (npret.ulFlags & BUF_EOS)
              { /* End of stream */
                bAtEOS = TRUE;
                DosResetEventSem(psib->hevStop, &ulPostCount);
              } /* End of stream */

            /* Return the empty buffer to the stream manager */

            if (!(rc = SMHEntryPoint(&npret)))
              { /* buffer returned */
                if (bAtEOS)
                  {
                    bAtEOS = FALSE;
                    ulStopped = DONT_SLEEP_FLUSH;
                    rc = SleepNCheck(psib, &ulStopped);
                  }
              } /* buffer returned */
          } /* We wrote ok */

        /* Make sure EOS is turned off, so things will work if the */
        /* stream is restarted. */

        npret.ulFlags = BUF_RETURNEMPTY;

      } /* have a buffer */

    } /* while no severe error */

  /* We get here if an error has occurred or a kill has */
  /*  been sent.  In the case of the kill, reset the */
  /*  return code to 0 (no error) and exit the thread. */
  /*  Otherwise, report the error event and exit the */
  /*  thread. */

  if (psib->ulActionFlags & SIBACTFLG_KILL)
    {
      rc = 0L;
    }
  else
    {
      ReportEvent(psib,
                  rc,                   /* Return code */
                  EVENT_ERROR,          /* event type */
                  0L,                   /* user info */
                  NONRECOVERABLE_ERROR);  /* Severe Error */
    }

  /* Only set this flag when we no longer need access to the sib since */
  /* Destroy may get control and Free the sib. */

  psib->ulActionFlags |= SIBACTFLG_THREAD_DEAD;
  return;

} /* End of FsshWrite */

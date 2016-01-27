/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: FSSHREAD.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  File System Stream Handler Read routine               */
/*                                                                          */
/* FUNCTION: This function is the routine that reads an object from the     */
/*           file system.                                                   */
/*                                                                          */
/* ENTRY POINTS: FsshRead                                                   */
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
/* SUBROUTINE NAME: FsshRead                                                */
/*                                                                          */
/* DESCRIPTIVE NAME: File System Stream Handler Read routine                */
/*                                                                          */
/* FUNCTION: Reads object from the file system and supplies it to the       */
/*           target stream handler.                                         */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: FsshRead                                                    */
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
/*          mmioRead                                                        */
/*          ShIOError                                                       */
/*          mmioGetLastError                                                */
/*          ReportEvent                                                     */
/*        DATA STRUCTURES:                                                  */
/*          npget                                                           */
/*          sib                                                             */
/*          hmtxGlobalData                                                       */
/*                                                                          */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

VOID FsshRead(psib)
PSIB psib;                              /* Stream Instance Block */

{ /* Start of FsshRead */

RC          rc = NO_ERROR;              /* local return code */
char        NeedBuf;                    /* local loop boolean */
LONG        NumBytesIO;                 /* Number of bytes from mmio */
PARM_NOTIFY npget, npret;               /* parms for SMH_NOTIFY calls */
SRCBUFTAB   SrcBufTab = {0};            /* Source buffer table */
ULONG       ulStopped = DO_SLEEP;       /* did we get stop discard or flush */
BOOL        bAtEOS = FALSE;             /* End of Stream indicator */
ULONG       ulPostCount;                /* Temp to hold count */
BOOL        bFirstRead = TRUE;          /* This is the first read  */

  /* Before we start lets do some init stuff: */

  npget.ulFunction = SMH_NOTIFY;
  npget.hid = psib->HandlerID;
  npget.hstream = psib->hStream;
  npget.ulGetNumEntries = 1L;
  npget.ulRetNumEntries = 0L;
  npget.pGetBufTab = &SrcBufTab;
  npget.pRetBufTab = NULL;

  npret.ulFunction = SMH_NOTIFY;
  npret.hid = psib->HandlerID;
  npret.hstream = psib->hStream;
  npret.ulFlags = BUF_RETURNFULL;
  npret.ulGetNumEntries = 0L;
  npret.ulRetNumEntries = 1L;
  npret.pGetBufTab = NULL;
  npret.pRetBufTab = &SrcBufTab;

  /* Wait until we get the ShcStart */

  DosWaitEventSem(psib->hevStop, SEM_INDEFINITE_WAIT);

  /* We will loop forever getting an empty buffer, calling the device to */
  /* fill up the buffer, sending it to the consumer.  During each iteration */
  /* of the loop we will check the action flags for asynchronous requests */
  /* to do things. */

  if (psib->ulActionFlags & SIBACTFLG_KILL)
    { /* Must have been a create error */
      rc = 1L;
    } /* Must have been a create error */

  /*  Start the main loop */

  while (!rc)
    { /* while no severe error */

    if (psib->ulActionFlags)
      rc = CheckNSleep(psib);

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

        npget.ulFlags = BUF_GETEMPTY;
        rc = SMHEntryPoint(&npget); /* get a buffer */
        if (!rc)
          {
            NeedBuf = FALSE;
            /* make sure attribute is 0 so we don't pass around a bad value */
            SrcBufTab.ulMessageParm = 0L;
          }
        else
          { /* return code from smhnotify */
            if (rc == ERROR_BUFFER_NOT_AVAILABLE)
              { /* buffer not available */

                /* the smhnotify resets the num entries to 0 when none avail */
                npget.ulGetNumEntries = 1L;

                ulStopped = DO_SLEEP;
                rc = SleepNCheck(psib, &ulStopped);

              } /* buffer not available */
          } /* return code from smhnotify */
      } /* while we don't have a buffer */

    /* We have a buffer or an error */

    if (!rc)
      { /* have a buffer - do the read */

        NumBytesIO = mmioRead((HMMIO)psib->ulAssocP1,
                                (PCHAR)SrcBufTab.pBuffer,
                                (LONG)SrcBufTab.ulLength);

        if ((NumBytesIO == -1L) || ((NumBytesIO == 0L) && (bFirstRead == TRUE)))
          { /* an error */

            SrcBufTab.ulLength = 0L;
            /* get the real error code */
            rc = mmioGetLastError((HMMIO)psib->ulAssocP1);

            rc = ShIOError(psib, npret, rc);

          } /* an error */
        else
          { /* We have some data */
            bFirstRead = FALSE;
            if (NumBytesIO != (LONG)SrcBufTab.ulLength)
              { /* End of stream */
                npret.ulFlags |= BUF_EOS;
                bAtEOS = TRUE;
                DosResetEventSem(psib->hevStop, &ulPostCount);
                SrcBufTab.ulLength = NumBytesIO;
              } /* End of stream */

            /* Send the data to the stream manager */

            rc = SMHEntryPoint(&npret);
            if (!rc)
              { /* data sent ok */
                if (bAtEOS)
                  {
                    bAtEOS = FALSE;
                    ulStopped = DO_SLEEP;
                    rc = SleepNCheck(psib, &ulStopped);
                  }
              } /* data sent ok */
          } /* We have some data */

        /* Clear the EOS if it was set. And attribute */
        npret.ulFlags = BUF_RETURNFULL;
        SrcBufTab.ulMessageParm = 0L;

      } /* have a buffer - do the read */

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

} /* End of FsshRead */

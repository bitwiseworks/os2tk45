/*static char *SCCSID = "@(#)semaph.c   6.12 92/02/19";*/
/*==============================================================*\
 *  SEMAPH.C - routines for demonstrating semaphore API.        *
 *        (C) Copyright IBM Corporation 1992.                   *
 *--------------------------------------------------------------*
 *  This module contains code to demonstrate the use of         *
 *  semaphores to control access to a resource shared by        *
 *  multiple threads. Event semaphores are used to signal a     *
 *  thread is to give up a resource. A Mutex semaphore is used  *
 *  to provide exclusive access to the resource. A mux          *
 *  semaphore provides a method to check multiple event         *
 *  semaphores.                                                 *
 *                                                              *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *        VOID   SemError(PSZ,ULONG);                           *
 *        ULONG  CreateAllSems(VOID);                           *
 *        VOID   StartSemExample(VOID);                         *
 *        VOID   ThreadConsumer(PVOID);                         *
 *        VOID   SignalUserEvent(PULONG pfAutoMode);            *
 *        ULONG  SetAutoMode(VOID);                             *
 *        VOID   RunAuto(VOID);                                 *
 *        VOID   StopSemaphore(VOID);                           *
 *--------------------------------------------------------------*
 * History:                                                     *
 *                                                              *
 * DEFECT    OWNER       Description                            *
 * 84624     FHASAN      Mismatch data type     @mmdt           *
 *                                                              *
 *                                                              *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
extern LONG colors[MAXUSERS];
extern LONG clrText[MAXUSERS];
static VOID MyMove (ULONG ulMyID, ULONG ulUser);

/****************************************************************\
 * Procedure to print error messages to screen in a Message Box.*
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    SemError(pszAPIName,usErrorCode)                   *
 *                                                              *
 *  Purpose: Used to print name of API and error number when    *
 *           a return code other than 0 is returned from an     *
 *           API call.                                          *
 *                                                              *
 *  Usage:   Called by all procedures in this file whenever     *
 *           an API call fails.                                 *
 *                                                              *
 *  Method:  The error number is converted to a string.  String *
 *           functions are used to build the error message.     *
 *           The message is printed in a Message Box.           *
 *                                                              *
 *  NOTE:    This function is called by multiple threads,       *
 *           therefore, only re-entrant functions can be used.  *
 *           Note that string is build according to American    *
 *           English conventions using hard-coded strings, and  *
 *           so portability to other languages would require a  *
 *           more flexible approach.                            *
 *                                                              *
 *  Returns: none.                                              *
 *                                                              *
\****************************************************************/
VOID SemError(PSZ pszAPIName,ULONG usErrorCode)
{
   char acMessage[100],acErrorNumber[10];

   _itoa(usErrorCode,acErrorNumber,BASETEN);
   strcpy(acMessage,pszAPIName);
   strcat(acMessage,": error # ");
   strcat(acMessage,acErrorNumber);

   WinMessageBox(HWND_DESKTOP,
                 hwndMain,
                 acMessage,
                 szAppName,
                 1,
                 MB_OK);
   return;
}

/****************************************************************\
 * Routine to create  semaphores used in this file.             *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    CreateAllSems(VOID)                                *
 *                                                              *
 *  Purpose: Create semaphores needed by the consumer threads.  *
 *           Checks return codes from semaphore creation.       *
 *                                                              *
 *  Usage:   Called by StartSemExample.                         *
 *                                                              *
 *  Method:  Semaphores are all anonymous private semaphores    *
 *           since the semaphores are used by threads in the    *
 *           same process.                                      *
 *                                                              *
 *  Returns: 0 if all semaphores are created successfully.      *
 *           Otherwise returns error code for first create      *
 *           semaphore API to fail.                             *
 *                                                              *
\****************************************************************/
ULONG CreateAllSems(VOID)
{
   ULONG      rc;
   SEMRECORD   asr[MAXRESOURCES];
   INT         i;

   rc = DosCreateMutexSem((PSZ)NULL,&hmtxOwnResource, 0L,FALSE);
   if (rc)
   {
       SemError("DosCreateMutexSem",rc);
       return(rc);
   }

   for (i = 0; i < MAXRESOURCES; i++)
   {
      rc = DosCreateEventSem( (PSZ)NULL, &aSquares[i].hev,
                                      0L, FALSE);
      if (rc)
      {
          SemError("DosCreateEventSem",rc);
          return(rc);
      }
      else
      {
          asr[i].ulUser = i;
          asr[i].hsemCur = (VOID *)aSquares[i].hev;
      }
   }
                          /* this muxwait semaphore contains all of the
                          event semaphores created in the loop above. */
   rc = DosCreateMuxWaitSem((PSZ)NULL,&hmuxResource,
           MAXRESOURCES,asr, DCMW_WAIT_ANY);
   if (rc)
   {
      SemError("DosCreateMuxWaitSem",rc);
      return(rc);
   }
   rc = DosCreateEventSem((PSZ)NULL,&hevStop,0L,FALSE);
   if (rc)
   {
       SemError("DosCreateEventSem",rc);
       return(rc);
   }
   return(rc);
}

/****************************************************************\
 * Routine to start semaphore example                           *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     StartSemExample(VOID)                             *
 *                                                              *
 *  Purpose:  Calls routines to create semaphores and draw      *
 *            resources.  Creates consumer threads.             *
 *                                                              *
 *  Usage:    Called in file usercmd.c when the user selects    *
 *            start from the semaphore menu.                    *
 *                                                              *
 *  Method:   Uses routines in paint.c to draw consumers and    *
 *            resources.  This is done by creating a paint      *
 *            message, not calling the draw routines directly.  *
 *                                                              *
 *  Returns:  TRUE if start succeeds, FALSE if start fails      *
 *                                                              *
\****************************************************************/
INT   StartSemExample(VOID)
{
    TID         tid=0;
    ULONG       rc;
    INT         i;
    FONTMETRICS fntmet;
    HPS         hps;
    SWP         swp;

    InitSemaphExample();

    rc = CreateAllSems();

    if (rc)
        return FALSE;
    /****************************************************************\
     * Create consumer threads. Note that values can be passed to   *
     * threads in OS/2 2.0. We pass the ordinal number of the child *
     * to each child.                                               *
    \****************************************************************/

    for (usConsumerThreadsCreated = 0;
     usConsumerThreadsCreated < cNumUsers; usConsumerThreadsCreated++)
    {
        rc = DosCreateThread((PTID) &tid, (PFNTHREAD)ThreadConsumer,
             (ULONG) usConsumerThreadsCreated, (ULONG)1,(ULONG) STACKSIZE);
        if (rc)
        {
            SemError("DosCreateThread",rc);
            return FALSE;
        }
        else
        {
            thrConsumers[usConsumerThreadsCreated].tid = tid;
            thrConsumers[usConsumerThreadsCreated].lHits = 0L;
        }
    }
    for (i = 0; i < (INT) cNumUsers; i++)
    {
        DosResumeThread (thrConsumers[i].tid);
    }
    hps = WinGetPS (hwndMain);
    if (hps)
    {
        GpiQueryFontMetrics (hps, (LONG) sizeof fntmet, &fntmet);
        WinQueryWindowPos (hwndMain, &swp);
        SetRectPositions((SHORT)swp.cx, (SHORT)swp.cy,
                         (SHORT) fntmet.lMaxBaselineExt,
                         (SHORT) fntmet.lMaxDescender);
        DrawRects (hps);
        WinReleasePS (hps);
    }
    return TRUE;
}

/****************************************************************\
 * Routine to signal consumer to release resource.              *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     SignalUserEvent(pfAutoMode)                       *
 *                                                              *
 *  Purpose:  Posts user event semaphore to signal thread to    *
 *            release resource.  Also posts event to stop       *
 *            Auto mode if *pfAutoMode is true.                 *
 *                                                              *
 *  Usage:    Called in file usercmd.c when the user selects    *
 *            Event from the semaphore menu.                    *
 *                                                              *
 *  Method:   Turns off Auto mode, if present by posting auto   *
 *            semaphore.  User event is then posted.            *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID SignalUserEvent(PULONG pfAutoMode)
{
  ULONG rc;

                          /* If sample is in auto mode turn auto mode off. */
  if (*pfAutoMode)
  {
      rc = DosPostEventSem(hevStopAuto);
      if (rc)
      {
         SemError("DosPostEventSem Stop Auto",rc);
      }
                       /* Wait for auto mode thread to die, so we don't
                          end up with multiple copies of it later.    */

      rc = DosWaitThread(&tidAutoThread,0L);
      if (rc)
      {
         SemError("DosWaitThread",rc);
      }
      *pfAutoMode = FALSE;
      DosCloseEventSem (hevStopAuto);
  }

  /* If Auto mode haas already posted the event this is OK
     so we will not check error codes here. */

  DosPostEventSem(aSquares[rand() % MAXRESOURCES].hev);
  return;
}

/****************************************************************\
 * Routine to start Auto mode.                                  *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     SetAutoMode(VOID)                                 *
 *                                                              *
 *  Purpose:  Creates thread and semaphore needed to run auto   *
 *            mode.                                             *
 *                                                              *
 *  Usage:    Called in file usercmd.c when the user selects    *
 *            Auto from the semaphore menu.                     *
 *                                                              *
 *  Returns:  NO_ERROR on success, else error from api call.    *
 *                                                              *
\****************************************************************/
ULONG SetAutoMode()
{
    ULONG rc;

    rc = DosCreateEventSem((PSZ)NULL,&hevStopAuto,0L,FALSE);
    if (rc)
    {
        SemError("DosCreateEventSem",rc);
        return(rc);
    }

    rc = DosCreateThread((PTID)&tidAutoThread,(PFNTHREAD)RunAuto,0L,0L,STACKSIZE);
    if (rc)
    {
        DosCloseEventSem (hevStopAuto); /* close semaphore created above */
        SemError("DosCreateThread", rc);
        return(rc);
    }

    return(NO_ERROR);
}

/****************************************************************\
 * Routine to run Auto mode.                                    *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     RunAuto(PVOID pvArg)                              *
 *                                                              *
 *  Purpose:  Posts user event at fixed time interval to signal *
 *            consumers to release resource.                    *
 *                                                              *
 *  Usage:    Thread created by SetAutoMode.                    *
 *                                                              *
 *  Method:   Kills itself when StopAutoMode semaphore is       *
 *            posted.                                           *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID RunAuto( PVOID pvArg )
{
   ULONG rcWait;
   HAB habLocal;
   HMQ hmqLocal;
   INT i;

   /* Need a message queue for any thread that wants to print messages. */

   habLocal = WinInitialize(0);
   hmqLocal = WinCreateMsgQueue(habLocal,0);

   /* while stop auto semaphore not posted, post user event semaphore. */

      /***************************************************************
       * Don't check return code from DosPostEventSem(hevUserEvent)  *
       * since we just want the resource to change hands.  We don't  *
       * care if it changes hands exactly every time it goes through *
       * the loop.                                                   *
       *                                                             *
       * The event may already be posted if the system is busy and   *
       * the resource threads don't finish with it fast enough.      *
       * This is not a problem in this case.                         *
       ***************************************************************/

   do {                      /* if ulTimeout is zero, still waitevent
                                 for 1 msec to force yielding of CPU. */
       rcWait = DosWaitEventSem(hevStopAuto, max (ulTimeout, 1));
       if (rcWait == ERROR_TIMEOUT)
       {
           i = rand () % MAXRESOURCES;      /* generate it */
           DosPostEventSem (aSquares[i].hev);
       }
   } while (rcWait == ERROR_TIMEOUT);

   /* If there was an error, print a message */

   if (rcWait)
   {
      SemError("DosWaitEventSem",rcWait);
   }

   WinDestroyMsgQueue (hmqLocal);
   WinTerminate (habLocal);
   DosExit(EXIT_THREAD,0);

   return;
}

/****************************************************************\
 * Routine to stop semaphore example.                           *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     BeginStop(pfAutoMode)                             *
 *                                                              *
 *  Purpose:  Posts stop event semaphore to signal threads to   *
 *            die.  Also posts event to stop Auto mode if       *
 *            necessary. Then waits for threads to complete,    *
 *            Creates thread StopSemaphore which posts stop     *
 *            event and waits for child threads.                *
 *                                                              *
 *  Usage:    Called in file usercmd.c when the user selects    *
 *            Stop from the semaphore menu.                     *
 *                                                              *
 *  Method:   Execs thread to do waits so that message thread   *
 *            doesn't hang.                                     *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID BeginStop (PULONG pfAutoMode)
{
   ULONG rc;
   TID tidLocal;

   rc = DosPostEventSem(hevStop);
   if (rc)
   {
       SemError("DosPostEventSem",rc);
       return;
   }
   if (*pfAutoMode)
   {
       rc = DosPostEventSem(hevStopAuto);
       if (rc)
       {
          SemError("DosPostEventSem",rc);
       }
   }
   rc = DosCreateThread((PTID)&tidLocal, (PFNTHREAD)StopSemaphore,
                          (ULONG)pfAutoMode,(ULONG)0,(ULONG) STACKSIZE);
   if (rc)
   {
       SemError("DosCreateThread",rc);
   }
   return;
}

/****************************************************************\
 * Routine to really stop semaphore example.                    *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     StopSemaphore( PVOID pvArg )                      *
 *                                                              *
 *  Purpose:  Waits for threads to complete,                    *
 *            Sends message to message thread to indicate this  *
 *            has occurred, and exits.                          *
 *                                                              *
 *  Usage:    Exec'd from BeginStop when user selects Stop from *
 *            Semaphore menu.                                   *
 *                                                              *
 *  Method:   Turns off Auto mode, if present by posting auto   *
 *            semaphore.  Then stop event is posted.  Waits     *
 *            for threads to die.                               *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID StopSemaphore( PVOID pvArg )
{
   ULONG rc,usCount, i;
   PULONG pfAutoMode = (PULONG)pvArg;

   if (*pfAutoMode)
   {
      rc = DosWaitThread(&tidAutoThread,0L);
      if (rc && (rc != ERROR_INVALID_THREADID))
      {
         SemError("DosWaitThread",rc);
      }
      *pfAutoMode = FALSE;
   }
   /* Wait for usConsumer threads to die.  Order of death not important. */

   for (usCount = 0; usCount < usConsumerThreadsCreated; usCount++)
   {
      rc = DosWaitThread(&thrConsumers[usCount].tid,0L);

      /* rc is ERROR_INVALID_THREADID the thread is already dead.*\
      \* This is OK and not a error.                             */

      if (rc && (rc != ERROR_INVALID_THREADID))
      {
         SemError("DosWaitThread",rc);
      }
   }
                /* Threads dead so we don't need semaphores any more.  */
   DosCloseEventSem(hevStopAuto);
   DosCloseEventSem(hevStop);
   DosCloseMutexSem(hmtxOwnResource);
   for (i = 0; i < MAXRESOURCES; i++)
   {
      DosCloseEventSem(aSquares[i].hev);
   }
   DosCloseMuxWaitSem (hmuxResource);
   WinPostMsg (hwndMain, WM_COMMAND, (MPARAM)IDM_STOPFINISHED, (MPARAM)NULL);
   DosExit (EXIT_THREAD, 0);
   return;
}

/****************************************************************\
 * Routine for consumer threads.                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     ThreadConsumer(PVOID)                             *
 *                                                              *
 *  Purpose:  There are NUMUSERS copies of this thread to act   *
 *            as consumers of the resource. The thread waits for*
 *            exclusive access to the resource and colors it.   *
 *                                                              *
 *  Usage:    Threads created by StartSemExample.               *
 *                                                              *
 *  Method:   Waits for mutex to gain ownership of resource.    *
 *            Releases resource when user event. Dies when      *
 *            Stop event.                                       *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID ThreadConsumer( PVOID pvMyID )
{
   ULONG  ulPostCnt;
   ULONG  ulUser=0L;
   ULONG  rc;
   HAB    habb;
   HMQ    hmqq;

   /* Need a message queue for any thread that wants to print messages. */
   habb = WinInitialize(0);
   hmqq = WinCreateMsgQueue(habb,0);

   /* while the user has not selected stop ... */
   while ((DosWaitEventSem(hevStop,SEM_IMMEDIATE_RETURN)) == ERROR_TIMEOUT)
   {
      /* Wait for exclusive ownership of resource */

      DosRequestMutexSem(hmtxOwnResource,SEM_INDEFINITE_WAIT);

      /* the following check is necessary because the stop semaphore
       * may have been posted  while we were waiting on the mutex
       */
      if (DosWaitEventSem(hevStop, SEM_IMMEDIATE_RETURN) == ERROR_TIMEOUT)
      {
      /*********************************************************************\
       * an item is ready, which one?  don't wait forever because there is *
       * a possibility that the stop semaphore was posted and that no more *
       * resource is forthcoming. we wait twice as long as we think is     *
       * necessary.                                                        *
      \*********************************************************************/
         if (!DosWaitMuxWaitSem (hmuxResource, max (ulTimeout << 1,
            TIMEOUTPERIOD) , &ulUser))
         {
            MyMove ((ULONG) pvMyID, ulUser);
            DosResetEventSem(aSquares[ulUser].hev, &ulPostCnt);
         }
      }
                               /* Let some other thread have resource. */
      rc = DosReleaseMutexSem(hmtxOwnResource);
      if (rc)
      {
         SemError("DosReleaseMutexSem",rc);
      }
   }
   /* hevStop was posted, kill this thread */
   WinDestroyMsgQueue (hmqq);
   WinTerminate (habb);
   DosExit(EXIT_THREAD, 0);
   return;
}

/****************************************************************\
 *  called from ThreadConsumer to make and register move        *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name: MyMove (ulMyID, ulUser)                               *
 *        ULONG     ulMyID;     id of caller                    *
 *        ULONG     ulUser;     number of the square to "hit"   *
 *                                                              *
 *  Purpose: color a square on the game board 'my' color.       *
 *           update display of number of hits for 'my' thread.  *
 *                                                              *
 *  Usage: called only from ThreadConsumer                      *
 *                                                              *
 *  Method:                                                     *
 *                                                              *
 *  Returns: none                                               *
 *                                                              *
\****************************************************************/
static VOID MyMove (ULONG ulMyID, ULONG ulUser)
{
   if (aSquares [ulUser].usOwner != ulMyID)
   {
      aSquares[ulUser].usOwner = ulMyID;
      thrConsumers[ulMyID].lHits++;
      DrawResource (&aSquares[ulUser].rcl, colors [ulMyID]);
      DrawStats (ulMyID);
   }
   return;
}

/****************************************************************\
 * Routine to init game board                                   *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:     InitSemaphExample(VOID)                           *
 *                                                              *
 *  Purpose:  Initializes game board unowned, sets semaphores   *
 *            to 0.                                             *
 *  Usage:    Called from StartSemExample in semaph.c when      *
 *            user selects start from the semaphore menu.       *
 *  Method:                                                     *
 *                                                              *
 *  Returns:  none                                              *
 *                                                              *
\****************************************************************/
VOID InitSemaphExample(VOID)
{
   int i;

   for (i = 0; i < MAXRESOURCES; i++)
   {
      aSquares[i].hev = 0;
      aSquares[i].usOwner = UNOWNED;
   }
   hevStopAuto = 0L;
   hevStop = 0L;
   hmtxOwnResource = 0L;
   hmuxResource  = 0L;
   return;
}

/*static char *SCCSID = "@(#)alarmthd.c	6.8 92/02/18";*/

/*=========================================================================\
 *                                                                         *
 *       FILE:                                                             *
 *                                                                         *
 *       DESCRIPTION:    Presentation Manager Alarm Clock Application      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1992  IBM Corp.                                          *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_PM
#define INCL_WINMENUS
#define INCL_GPITRANSFORMS
#define INCL_WINHELP
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME
#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#define INCL_WINDIALOGS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include "clock.h"
#include "dialogs.h"
#include "clkdata.h"
#include "alarmthd.h"
#include "res.h"


/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

#define HOURS_24 ( (LONG) 1000L * 24L * 60L * 60L)


/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

APIRET AlarmInit(VOID);
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:AlarmThread()
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/
VOID AlarmThread(PVOID pvMessage)

{
     ULONG ulTimerValue,ulPostCount;
     SHORT usi;


     /*
      *create a event semaphore
      *for this timer
      */
     if( DosCreateEventSem(NULL,
                           &TimerResources.hTimerDev,
                           DC_SEM_SHARED,
                           FALSE) )
     {
          /*
           *generate error
           *message
           *back to the console
           */
     }

     /*
      *create our queue
      *and  semaphore
      */
      if(AlarmInit() )
      {
          /*
           *send a message to our
           *main thread,unable to start
           */

          MessageBox(hwndFrame, IDMSG_CANNOTLOADSTRING,
                     MB_OK | MB_ERROR, FALSE);
          _endthread();
      }
      ulTimerValue = TimerResources.ulTimerValue;

      /*
       *stay in our
       *loop until a abort message is received
       *from our main
       *thread
       */

      do
      {

          /*
           *sit here and
           *wait for the semaphore to post,
           *from one of two conditions
           * 1)either the timer expired
           * 2)or the pm thread cleared it for
           * a exit or the timer value has changed
           */
           if( DosWaitEventSem((HEV)TimerResources.hTimerDev,
                               SEM_INDEFINITE_WAIT) )
           {
               /*
                *error occurred
                *notify the pm thread
                */
                break;

           }
           if(fEndThread)
           {
               continue;
           }

           /*
            *reset the post count
            */
           DosResetEventSem( (HEV) TimerResources.hTimerDev,
                               &ulPostCount);
           /*
            *did the timer value get
            *changed
            */
            if(TimerResources.ulTimerValue != ulTimerValue)
            {
                     ulTimerValue = TimerResources.ulTimerValue;
                     /*
                      *non async timer
                      *needs to be stopped
                      */
                     if(cp.sTimerType == TIMER_CONT )
                     {
                         DosStopTimer(TimerResources.hTimer);
                     }
                     /*
                      *set the alarm for the initial time
                      *delay till the first
                      *alarm goes
                      *off
                      */

                     if(AlarmInit() )
                     {
                         /*
                          *generate error message
                          */
                          MessageBox(hwndFrame, IDMSG_CANNOTLOADSTRING,
                                     MB_OK | MB_ERROR, FALSE);
                          break;
                     }

            }
            /*
             *the timer
             *expired
             */
            else
            {
                /*
                 *is the alarm set
                 */
                if(cp.alarm.usMode & AM_ACTIVE )
                {
                    /*
                     *send a message
                     *that a alarm has gone off
                     */
                     if(cp.alarm.usMode & AM_AUDIO)
                     {
                        for (usi = 0; usi < 20; usi++)
                        {
                             DosBeep(300 + usi * 100, 50);
                        }
                     }
                     /*
                      *message box
                      */
                     if(cp.alarm.usMode & AM_MSGBOX)
                     {
                           WinPostMsg( hwndFrame,
                                       WM_COMMAND,
                                       MPFROM2SHORT(IDM_ALARM_EXPIRED,TRUE),
                                           MPVOID );
                     }
                     /*
                      *the alarm has expired,check and make sure
                      *its on a 24 hour boundary
                      */
                     if(ulTimerValue != HOURS_24 )
                     {
                         ulTimerValue = TimerResources.ulTimerValue =
                                   HOURS_24;
                         /*
                          *non async timer
                          *needs to be stopped
                          */
                         if(cp.sTimerType == TIMER_CONT )
                         {
                             DosStopTimer(TimerResources.hTimer);
                         }
                         if(AlarmInit() )
                         {
                             /*
                              *generate error message
                              */
                              break;
                         }
                     }
                 }
            }
     }while(!fEndThread);


     _endthread();


}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:AlarmInit();
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          NO_ERROR   (0) No Error occurred
 *          Non - Zero the api return value for the called function
\****************************************************************/
APIRET AlarmInit(VOID)
{
     APIRET ApiReturn = NO_ERROR;


     switch(cp.sTimerType)
     {
     case TIMER_CONT:
          if( ApiReturn = DosStartTimer(TimerResources.ulTimerValue,
                                        (HSEM)TimerResources.hTimerDev,
                                        &TimerResources.hTimer) )
          {
               return(ApiReturn);
          }
          break;
     case TIMER_ASYNC:
          if( ApiReturn = DosAsyncTimer(TimerResources.ulTimerValue,
                                        (HSEM)TimerResources.hTimerDev,
                                        &TimerResources.hTimer) )
          {
               return(ApiReturn);
          }
          break;
     }
     return(ApiReturn);
}
/*--------------------------------------------------------------*\
 *  End of file : alarmthd.c
\*--------------------------------------------------------------*/

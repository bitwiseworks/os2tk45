/*static char *SCCSID = "@(#)sem_user.c	6.10 92/03/25";*/
/*==============================================================*\
 *  SEM_USER.C - routines for handling messages not processed   *
 *              by the standard message processing routine      *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the code for processing messages sent  *
 *  to the standard window that the standard window does not    *
 *  process.  The application developer need only modify this   *
 *  file in order to implement new menu items or process        *
 *  messages not handled by the standard message routine.       *
 *                                                              *
 *  This module also contains some routines that demonstate the *
 *  various dialog box controls and message box types that can  *
 *  be used.  The sample code should be deleted when this       *
 *  module is modified for an application.  The demonstration   *
 *  code is identified by comments.                             *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *      UserCommand(mp1, mp2) - user WM_COMMAND processor       *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

/****************************************************************\
 *  Non-standard menu item command processing procedure         *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   UserCommand(mp1, mp2)                               *
 *                                                              *
 *  Purpose: Process any WM_COMMAND messages send to hwndMain   *
 *              that are not processed by MainCommand           *
 *                                                              *
 *  Usage:  Routine is called for each WM_COMMAND that is       *
 *          not posted by a standard menu item                  *
 *                                                              *
 *  Method: A switch statement branches control based upon      *
 *          the id of the control which posted the message      *
 *                                                              *
 *  Returns:                                                    *
\****************************************************************/
VOID UserCommand( MPARAM mp1, MPARAM mp2 )
{
    USHORT rc;
    HWND hwndSemMenu;

    hwndSemMenu = WinWindowFromID(hwndMainFrame, FID_MENU);

    switch(SHORT1FROMMP(mp1))
    {
       case IDM_SEMSTART_MANUAL:
           if (!fSemaphoreStarted)
           {   /* should never get this message unless sample started*/

               fSemaphoreStarted = TRUE;
               fSemaphoreWasStarted = TRUE;

               /* Turn on other menu items */
               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_SEMSTOP,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,FALSE));

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_POST,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,FALSE));

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_SEM_RESUMEAUTO,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,FALSE));

               /* turn off menu items to configure demo */
               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_OPTIONS_NTHRDS,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_OPTIONS_TIMEOUT ,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_SEMSTART,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

               if (StartSemExample() == FALSE) {
                   MessageBox (HWND_DESKTOP, IDMSG_CANNOTSTART, TRUE);
                   WinSendMsg (hwndMain, WM_CLOSE, 0L, 0L);
               }
           }
           break;

       case IDM_POST:
           if (fSemaphoreStarted)
           {   /* should never get this message unless sample started*/
               if (fAutoMode)
               {
                    WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_SEM_RESUMEAUTO,TRUE),
                     MPFROM2SHORT(MIA_DISABLED,FALSE));
               }
               SignalUserEvent(&fAutoMode);
           }
           break;

       case IDM_SEM_RESUMEAUTO:
       case IDM_SEMSTART_AUTO:
           if (!fSemaphoreStarted)
           {    /* should never get this message unless sample started*/
                fSemaphoreStarted = TRUE;
                fSemaphoreWasStarted = TRUE;

                /* Turn on other menu items */
                WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_SEMSTOP,TRUE),
                    MPFROM2SHORT(MIA_DISABLED,FALSE));

                WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_POST,TRUE),
                    MPFROM2SHORT(MIA_DISABLED,FALSE));

                /* turn off menu items to configure demo */
                WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_OPTIONS_NTHRDS,TRUE),
                    MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_OPTIONS_TIMEOUT ,TRUE),
                   MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

                StartSemExample();

           }

           if (fSemaphoreStarted && !fAutoMode)
           {
              rc = SetAutoMode();
              if (!rc)
              {
                 fAutoMode = TRUE;
                 /* disable menus */

                 WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_SEM_RESUMEAUTO,TRUE),
                     MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

                 WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_SEMSTART,TRUE),
                    MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
              }
           }
           break;

       case  IDM_SEMSTOP:
           if (fSemaphoreStarted)
           {
              BeginStop (&fAutoMode);

               /* don't allow them to select stop again */
              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_SEMSTOP,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_SEM_RESUMEAUTO,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));

              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_POST,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
           }
           break;
       case  IDM_STOPFINISHED:
           if (fSemaphoreStarted)
           {
              fSemaphoreStarted = FALSE;

              /* Reset option and semaphore/start menu items ... */
              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_SEMSTART,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,FALSE));

              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_OPTIONS_NTHRDS,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,FALSE));

              WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                  MPFROM2SHORT(IDM_OPTIONS_TIMEOUT ,TRUE),
                  MPFROM2SHORT(MIA_DISABLED,FALSE));
           }
           break;

       case IDM_OPTIONS_NTHRDS_2:
       case IDM_OPTIONS_NTHRDS_4:
       case IDM_OPTIONS_NTHRDS_8:
       case IDM_OPTIONS_NTHRDS_16:
           if (!fSemaphoreStarted) {
               INT oldNumUsers = cNumUsers;
               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_OPTIONS_NTHRDS+cNumUsers,TRUE),
                   MPFROM2SHORT(MIA_CHECKED,FALSE));
               cNumUsers = SHORT1FROMMP(mp1) - IDM_OPTIONS_NTHRDS;
               /* if we changed the number of threads to display */
               if (cNumUsers != oldNumUsers)
                   WinPostMsg (hwndMain, WM_USERCHANGED, (MPARAM)0, (MPARAM)0);

               WinSendMsg(hwndSemMenu,MM_SETITEMATTR,
                   MPFROM2SHORT(IDM_OPTIONS_NTHRDS+cNumUsers,TRUE),
                   MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));
           }
           break;

       case IDM_OPTIONS_TIMEOUT:
           if (!fSemaphoreStarted)
           {
               WinDlgBox (HWND_DESKTOP,
                          hwndMain,
                          TimeDlgProc,
                          0,
                          TIMERBOX,
                          NULL);
           }
           break;

       default:
               break;
    }

    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'  *
    \*  warning at compile time.                                      */
    return;

}                                                    /* UserCommand() */

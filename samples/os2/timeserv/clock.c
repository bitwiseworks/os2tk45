/*static char *SCCSID = "@(#)clock.c	6.9 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE:clock.c                                                      *
 *                                                                         *
 *       DESCRIPTION:    Presentation Manager Alarm Clock Application      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1989, 1990, 1992 IBM Corp.                               *
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
 *  This source file contains the following functions:
 *
 *  main()
 *  ClkInit()
 *  ClkFrameWndProc()
 *  MessageBox()
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/


#define INCL_PM
#define INCL_GPITRANSFORMS
#define INCL_WINHELP
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME
#define INCL_DOSDEVIOCTL
#define INCL_DOSPROCESS


#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "clock.h"
#include "res.h"
#include "clkdata.h"
#include "alarmthd.h"


/*--------------------------------------------------------------*\
 *Global definitions for this module
\*--------------------------------------------------------------*/

#define  STACK_SIZE_SECTHRD 8192
#ifndef  DEFAULT_QUEUE_SIZE
#define  DEFAULT_QUEUE_SIZE  5
#endif

/*--------------------------------------------------------------*\
 *  Global  variables for this module
\*--------------------------------------------------------------*/


TID  tidAlarmThread;
PFNWP pfnwpFrameWndProc;

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

VOID    ClkShowFrameControls (HWND);
MRESULT EXPENTRY ClkWndProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ClkFrameWndProc (HWND, ULONG, MPARAM, MPARAM);
INT     main (SHORT sArgc , CHAR *ppArgv[]);
BOOL    ClkInit (VOID);
/****************************************************************\
 *  Routine Name:
 *--------------------------------------------------------------
 *
 *  Name: main()
 *
 *  Purpose:Main entry point for  the clock program
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
 *
 *
\****************************************************************/
INT  main (SHORT sArgc,CHAR  *ppArgv[])
{
    QMSG qmsg ;

    /* have we been asked to start ourselves as an icon? */
    if (sArgc > 1)
    {
        if ( stricmp ( ppArgv [ 1 ] , "iconic" ) == 0 )
            fStartAsIcon = TRUE ;


    }

    if ( ClkInit ( ) )
    {

        while ( WinGetMsg ( hab , & qmsg , NULLHANDLE , 0 , 0 ) )
            WinDispatchMsg ( hab , & qmsg ) ;

        /* clean up and go home */
        #ifdef HELP
        if ( hwndHelpInstance != NULLHANDLE )
        {
            WinDestroyHelpInstance(hwndHelpInstance);
        }
        #endif

        /*
         *post the event semaphore
         *for our alarm thread
         */
        fEndThread = TRUE;
        DosStopTimer(TimerResources.hTimer);
        WinRemoveSwitchEntry ( hsw ) ;
        WinDestroyWindow ( hwndFrame ) ;
        WinDestroyMsgQueue ( hmq ) ;
        WinTerminate ( hab ) ;
    }

    return ( (INT) 0) ;
}/*main*/
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkInit()
 *
 *  Purpose:Clock Initialization.
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
BOOL ClkInit ( VOID )
{
    /* app name, switch list info, and frame creation flags */
    static PSZ pszClkName = "Clock";
    static SWCNTRL swctl = { NULLHANDLE , NULLHANDLE , NULLHANDLE , 0 , 0 ,
                             SWL_VISIBLE ,
                             SWL_JUMPABLE , "Clock" , 0 } ;
    static ULONG fcf = FCF_SIZEBORDER | FCF_TITLEBAR | FCF_MINMAX
                      | FCF_SYSMENU | FCF_ACCELTABLE;

    PID pid ;
    TID tid ;
    TimerResources.pszSemName   = SEM_TIMER_NAME;
    TimerResources.ulTimerValue = SEM_TIMER_VALUE;
    cp.sTimerType = TIMER_CONT;

    if ( ( hab = WinInitialize ( 0 ) ) == NULLHANDLE )
        return( FALSE );

    if ( ( hmq = WinCreateMsgQueue ( hab , DEFAULT_QUEUE_SIZE ) ) == NULLHANDLE )
    {
        WinTerminate ( hab ) ;
        return (FALSE );
    }

    if ( ! WinRegisterClass ( hab , pszClkName , ClkWndProc ,
                              CS_SIZEREDRAW , 0 ) )
    {
        WinDestroyMsgQueue ( hmq ) ;
        WinTerminate ( hab ) ;
        return (FALSE );
    }

    hwndFrame = WinCreateStdWindow ( HWND_DESKTOP , 0 , &fcf ,
                                     pszClkName , pszClkName ,
                                     0,
                                     NULLHANDLE , IDR_MAIN ,
                                     &hwndClient ) ;


    if (hwndFrame == NULLHANDLE)
    {
        WinDestroyMsgQueue (hmq);
        WinTerminate (hab);
        return (FALSE );
    }

    pfnwpFrameWndProc = WinSubclassWindow (hwndFrame, ClkFrameWndProc);

    /* add ourselves to the switch list */
    WinQueryWindowProcess (hwndFrame, &pid, &tid);
    swctl.hwnd = hwndFrame;
    swctl.idProcess = pid;
    hsw = WinAddSwitchEntry (&swctl);

    #ifdef HELP
    InitHelp();
    #endif
    /*
     *create our
     *second thread
     *to keep track of alarms
     *for us
     */
    if( tidAlarmThread = _beginthread(AlarmThread,
                                      NULL,
                                      STACK_SIZE_SECTHRD,
                                      NULL) == -1 )
    {
          return(FALSE);
    }

    return TRUE;
}/*ClkInit*/

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkFrameWndProc
 *
 *  Purpose: This routine handles our top level interface to the
 *           desktop manager.
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
MRESULT EXPENTRY ClkFrameWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    register USHORT cmd;
    USHORT cmdSrc;
    HWND hwndT;
    SWP swp;

    switch (msg)
    {
    case WM_DESTROY:
        if (cp.fControlsHidden)
            ClkShowFrameControls (hwnd);
        break;

    case WM_SYSCOMMAND:

        if (cp.fControlsHidden)
        {

            cmd = LOUSHORT(mp1);
            cmdSrc = LOUSHORT(mp2);

            /*
             * If the mouse has been captured, ignore the system command.
             */
            if(WinQueryCapture (HWND_DESKTOP) != NULLHANDLE)
            {
                return(MPVOID);
            }

            switch (cmd)
            {
            case SC_RESTORE:
            case SC_MAXIMIZE:
                /*
                 * Get the restore position that SetMultWindowPos will use.
                 */
                WinQueryWindowPos(hwnd, &swp);

                switch(cmd)
                {
                case SC_RESTORE:
                    swp.fl = SWP_RESTORE;
                    break;

                case SC_MAXIMIZE:
                    swp.fl = SWP_MAXIMIZE;
                    break;
                }

                /*
                 * If the control key is down, don't activate, or deactivate.
                 * Don't do this if this came from an accelerator.
                 */
                if (cmdSrc != CMDSRC_ACCELERATOR)
                {
                    if (WinGetKeyState(HWND_DESKTOP, VK_CTRL) & 0x8000)
                        swp.fl &= ~(SWP_DEACTIVATE | SWP_ACTIVATE);
                }

                WinSetMultWindowPos(NULLHANDLE, (PSWP)&swp, 1);
                break;

            case SC_MOVE:
            case SC_SIZE:
            case SC_CLOSE:
                switch (cmd)
                {
                case SC_SIZE:
                    /*
                     * Start keyboard sizing interface by sending a WM_TRACKFRAME
                     * message back to ourselves (the frame window).
                     */
                    WinSendMsg(hwnd, WM_TRACKFRAME,
                            MPFROMSHORT(TF_SETPOINTERPOS), MPVOID );
                    break;

                case SC_MOVE:
                    /*
                     * Start keyboard moving interface by sending a WM_TRACKFRAME
                     * message back to ourselves (the frame window).
                     */
                    WinSendMsg (hwnd, WM_TRACKFRAME,
                            MPFROMSHORT(TF_SETPOINTERPOS | TF_MOVE), MPVOID );
                    break;

                case SC_CLOSE:
                    if ((hwndT = WinWindowFromID (hwnd, FID_CLIENT)) == NULLHANDLE)
                        hwndT = hwnd;
                    WinSetParent (hwndTitleBar, hwndFrame, FALSE);
                    WinSetParent (hwndSysMenu, hwndFrame, FALSE);
                    WinSetParent (hwndMinMax, hwndFrame, FALSE);
                    WinSetParent (hwndMenu, hwndFrame, FALSE);

                    WinPostMsg (hwndT, WM_CLOSE, MPVOID, MPVOID );
                    break;
                }
                break;

            default:
                return (*pfnwpFrameWndProc)(hwnd, msg, mp1, mp2);
            }

            return (MPVOID);

        }
        else
        {
            return (*pfnwpFrameWndProc)(hwnd, msg, mp1, mp2);
        }
    }

    return (*pfnwpFrameWndProc)(hwnd, msg, mp1, mp2);
}/*ClkFrameWindowProc*/

/****************************************************************\
 *  Message Box procedure
 *--------------------------------------------------------------
 *
 *  Name:   MessageBox(hwndOwner, nIdMsg, fsStyle, fBeep)
 *
 *  Purpose: Displays the message box with the message
 *              given in idMsg retrieved from the message table
 *              and using the style flags in fsStyle
 *
 *  Usage:  Called whenever a MessageBox is to be displayed
 *
 *  Method: - Message string is loaded from the process'
 *              message table
 *          - Alarm beep is sounded if desired
 *          - Message box with the message is displayed
 *          - WinMessageBox return value is returned
 *
 *  Returns: return value from WinMessageBox()
 *
\****************************************************************/
APIRET MessageBox(HWND hwndOwner,SHORT idMsg,SHORT fsStyle, BOOL fBeep)
{
    CHAR szText[MESSAGELEN];
    APIRET ApiReturn = NO_ERROR;


    if(!WinLoadMessage(hab,
                      NULLHANDLE,
                      idMsg,
                      MESSAGELEN,
                      (PSZ)szText))
    {

        WinAlarm(HWND_DESKTOP, WA_ERROR);
        return MBID_ERROR;
    }

    if(fBeep)
    {
        WinAlarm(HWND_DESKTOP, WA_ERROR);
    }

   ApiReturn = WinMessageBox(HWND_DESKTOP,
                         hwndOwner,
                         szText,
                         (PSZ)NULL,
                         MSGBOXID,
                         fsStyle );

    return(ApiReturn);
}/* MessageBox() */

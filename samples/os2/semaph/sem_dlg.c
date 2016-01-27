/*static char *SCCSID = "@(#)sem_dlg.c	6.13 92/03/18";*/
/*==============================================================*\
 *  SEM_DLG.C - window procedures for the dialog boxes as well  *
 *              as utility procedures used by them              *
 *      (C) Copyright IBM Corporation 1992.                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the Dialog Procedures for the user     *
 *  defined dialogs as well as any support code they need       *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *           TimeDlgProc(hwnd, msg, mp1, mp2)                   *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

/****************************************************************\
 *  Dialog procedure for the Timeout entry dlg box              *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   TimeDlg(hwnd, msg, mp1, mp2)                        *
 *                                                              *
 *  Purpose: Processes all messages sent to the Timer Box       *
 *                                                              *
 *  Usage:  Called for each message sent to the Timer Box       *
 *          dialog box.                                         *
 *                                                              *
 *  Method: display box on init, and reset global ulTimeout on  *
 *          successful entry                                    *
 *                                                              *
 *                                                              *
 *                                                              *
 *  Returns: Dependent upon message sent                        *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY TimeDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    CHAR    szBuffer [MAXTEXTLEN];
    SHORT   sTimeout = 0;        /* local copy of timeout for dlg */
    static  HWND    hwndWork;
    MRESULT sRC;

    switch(msg)  {
            case WM_INITDLG:
                /* init the entry field w/current value */
                hwndWork = WinWindowFromID ( hwnd, IDD_ENTRY);
                WinSetWindowText (hwndWork, _itoa(ulTimeout, szBuffer, 10));
                WinSendMsg( hwndWork, EM_SETSEL,
                        MPFROM2SHORT(0, strlen(szBuffer)), (MPARAM)0 );
                FixSysMenu(hwnd);
                return (MRESULT)0L;

            case WM_COMMAND:
                /* no matter what the command, close the dialog */
                switch(SHORT1FROMMP(mp1))  {
                    case DID_OK:
                        if (WinQueryDlgItemShort(hwnd, IDD_ENTRY,
                                 &sTimeout, FALSE) == FALSE) {
                            WinSetDlgItemText (hwnd, IDD_ENTRY,
                                          _itoa(ulTimeout, szBuffer, 10));
                        }
                        else
                            ulTimeout = (ULONG)sTimeout;

                        WinDismissDlg(hwnd, TRUE);
                        return (MRESULT)0L;
                                                    /* fall thru to... */
                    case DID_CANCEL:
                        WinDismissDlg(hwnd, FALSE);
                        return (MRESULT)0L;

                    default:
                        sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                        return sRC;
                }
                break;

            case WM_CLOSE:
                WinDismissDlg(hwnd, FALSE);
                return (MRESULT)0L;

            default:
                sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
                return sRC;
    }
    return (MRESULT)0L;
}                                                      /* TimeDlgProc() */

/*static char *SCCSID = "@(#)vmm_init.c	6.12 92/02/18";*/
/*==============================================================*\
 *  VMM_INIT.C - routines for initialization and exit processing*
 *    (C) Copyright IBM Corporation 1992.                       *
 *--------------------------------------------------------------*
 *                                                              *
 *  This module contains the code for application initialization*
 *  as well as the code for exit list processing                *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *      Init() - initialization routines                        *
 *      ExitProc(usTermCode) - exit list processing procedure   *
 *      GPFHandler(parg) - GPFault and guard page excep handler *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "vmm.h"

/****************************************************************\
 *  Initialization routine                                      *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    Init()                                             *
 *                                                              *
 *  Purpose: Performs initialization functions required before  *
 *           the main window can be created.                    *
 *                                                              *
 *  Usage:   Called once before the main window is created.     *
 *                                                              *
 *  Method:  - installs the routine ExitProc into the           *
 *              DosExitList chain                               *
 *           - registers all window classes                     *
 *           - performs any command line processing             *
 *                                                              *
 *  Returns: TRUE - initialization is successful                *
 *           FALSE - initialization failed                      *
 *                                                              *
\****************************************************************/
BOOL Init(VOID)
{
    /* Add ExitProc to the exit list to handle the exit processing */
#ifndef VER630
    if(DosExitList(EXLST_ADD, ExitProc))
    {
        return FALSE;
    }
#endif

                       /* load application name from resource file */
    if(!WinLoadString(hab, (HMODULE)0, IDS_APPNAME, MAXAPPNAMELEN, szAppName))
        return FALSE;
    if(!WinLoadString(hab, 0, IDS_UNTITLED, MESSAGELEN, szUntitled))
        return FALSE;
                          /* register the main client window class */
    if(WinRegisterClass(hab,
                        (PSZ)szAppName,
                        (PFNWP)MainWndProc,
                        CS_SIZEREDRAW | CS_SYNCPAINT | CS_CLIPCHILDREN,
                        0) == 0)
        return FALSE;

    /* Turn off hard-error popup after GP-FAULT. No return codes
       checked as it is cosmetic and if it doesn't work the only side
       effect will be to have the default hard-error popup show up. */

    DosError(FERR_DISABLEEXCEPTION | FERR_DISABLEHARDERR);

    return TRUE;

}  /* Init() */

/****************************************************************\
 *  Exit list processing procedure                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    ExitProc(usTermCode)                               *
 *                                                              *
 *  Purpose: Cleans up certain resources when the application   *
 *           terminates                                         *
 *                                                              *
 *  Usage:   Routine is called by DosExitList when the          *
 *           application exits                                  *
 *                                                              *
 *  Method:  global resources, such as the main window and      *
 *           message queue, are destroyed and any system        *
 *           resources used are freed                           *
 *                                                              *
 *  Returns: Returns EXLST_EXIT to the DosExitList handler      *
 *                                                              *
\****************************************************************/
VOID APIENTRY ExitProc(ULONG usTermCode)
                                  /* code for the reason for termination */
{
   WinDestroyWindow(hwndMainFrame);
   /*--------------------------------------------------*\
    *      Any other system resources used             *
    *      (e.g. memory or files) should be freed here *
   \*--------------------------------------------------*/
   WinDestroyMsgQueue(hmq);

   WinTerminate(hab);

   DosExitList(EXLST_EXIT, (PFNEXITLIST)0L);    /* termination complete */

   /* This routine currently doesn't use the usTermCode parameter so *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'  *
   \*  warning at compile time                                       */
   return;
}                                                          /* ExitProc() */

/****************************************************************\
 *  Do nothing Guard Page Exception Handler                     *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    GPFHandler(PXCPT)                                  *
 *                                                              *
 *  Purpose: To notify user when a guard page entered exception *
 *           occurs                                             *
 *                                                              *
 *  Usage:   Routine is called when a guard page is accessed    *
 *                                                              *
 *  Method:  Whenever a guard page is accessed, a message       *
 *           box is put on the screen indicating this.          *
 *                                                              *
 *  Returns: Returns HANDLED if guard page exception,           *
 *           otherwise, returns NOT_HANDLED                     *
\****************************************************************/
ULONG GPFHandler(PEXCEPTIONREPORTRECORD parg)
{
    ULONG   flMemAttrs=0;
    ULONG   ulMemSize;
    CHAR    szText[MESSAGELEN];
    USHORT  usRet;

    /* Notify the user when they have accessed a guard page, and let
       the system default guard page handler handle the exception. */

    if(parg->ExceptionNum == XCPT_GUARD_PAGE_VIOLATION)
    {
        MessageBox(hwndMain, IDMSG_ACCESSGUARDPAGE, "Note !",
                        MB_MOVEABLE | MB_OK | MB_ICONASTERISK, FALSE);

        WinInvalidateRect(hwndMain,NULL,TRUE);

        return(XCPT_CONTINUE_EXECUTION);
    }
    if(parg->ExceptionNum == XCPT_ACCESS_VIOLATION)
    {
        if (parg->ExceptionInfo[1])
        {
            ulMemSize = 1L;
            DosQueryMem((PVOID)(parg->ExceptionInfo[1]),
                        &ulMemSize,&flMemAttrs);

            /* If the exception is due to accessing a page that has
               been allocated but not committed, then commit the page.
               Otherwise, GP Fault. */

            if (((flMemAttrs & PAG_FREE) || (flMemAttrs & PAG_COMMIT)) == 0)
            {
               WinLoadMessage(hab, (HMODULE)NULL, IDMSG_COMMITTTOWRITE,
                                    MESSAGELEN, (PSZ)szText);
               WinAlarm(HWND_DESKTOP, WA_ERROR);
               usRet = WinMessageBox(HWND_DESKTOP,
                                     hwndMain,
                                     szText,
                                     (PSZ)"Warning !",
                                     IDM_MSGBOX,
                                     MB_YESNO | MB_ICONASTERISK);
               switch(usRet)
               {
                  case MBID_YES:
                      if (DosSetMem((PVOID)(parg->ExceptionInfo[1]),
                                 1L,PAG_DEFAULT | PAG_COMMIT) != 0L)
                      {
                         DosExit(EXIT_PROCESS, RETURN_ERROR);
                         return(XCPT_CONTINUE_SEARCH);
                      }
                      else                        /* Commit successfully */
                         return(XCPT_CONTINUE_EXECUTION);

                  case MBID_NO:
                      DosExit(EXIT_PROCESS, RETURN_ERROR);
                      return(XCPT_CONTINUE_SEARCH);
               }
            }
        }
    /* At this point, we have a GP Fault. We cannot recover, so raise the
    terminate process exception and return (GP Fault) exception handled. */

        MessageBox(hwndMain, IDMSG_PROTECTIONERROR, "Error !",
                    MB_MOVEABLE | MB_OK | MB_ICONHAND, TRUE);

        DosExit(EXIT_PROCESS, RETURN_ERROR);
        return(XCPT_CONTINUE_SEARCH);
    }
 /* The exception is not one that we handle, so pass it on as NOT_HANDLED. */
    DosExit(EXIT_PROCESS, RETURN_ERROR);
    return(XCPT_CONTINUE_SEARCH);
}

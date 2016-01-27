/*static char *SCCSID = "@(#)dlapi_in.c	6.8 92/02/18";*/
/*==============================================================*\
 *  DLAPI_IN.C - routines for initialization and exit processing*
 *      (C) Copyright IBM Corporation 1992.                     *
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
#include "dllapi.h"

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
    if(DosExitList(EXLST_ADD, ExitProc))
    {
        return FALSE;
    }
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
}                                                         /* Init() */

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
ULONG GPFHandler(EXCEPTIONREPORTRECORD *parg,
                 EXCEPTIONREGISTRATIONRECORD *pRegisRecord,
                 PCONTEXTRECORD  pContextRecord,
                 PVOID pvSpare)
{
    LONG  sRet;
    CHAR  szText[MESSAGELEN];

    if(parg->ExceptionNum == XCPT_FLOAT_DIVIDE_BY_ZERO ||
       parg->ExceptionNum == XCPT_INTEGER_DIVIDE_BY_ZERO)
    {
       MessageBox(hwndMain, IDMSG_DIVIDEZERO, "Error !",
                MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
       return(XCPT_CONTINUE_SEARCH);
    }
    /* Notify the user when they have accessed a guard page, and let
       the system default guard page handler handle the exception. */
    if(parg->ExceptionNum == XCPT_GUARD_PAGE_VIOLATION)
    {
       WinLoadMessage(hab, (HMODULE)NULL, IDMSG_ACCESSGUARDPAGE, MESSAGELEN,
                      (PSZ)szText);
       WinAlarm(HWND_DESKTOP, WA_ERROR);

       sRet = WinMessageBox(HWND_DESKTOP, hwndMain, szText, "Note !",
                            IDM_MSGBOX, MB_MOVEABLE | MB_OKCANCEL | MB_ERROR);
       switch (sRet)
       {
          case MBID_OK:
             pFlList = pBackup;
             break;

          case MBID_CANCEL:
             DosExit(EXIT_PROCESS, RETURN_ERROR);
             break;
       }
       WinInvalidateRect(hwndMain,NULL,TRUE);
       return(XCPT_CONTINUE_EXECUTION);
    }
    return(XCPT_CONTINUE_SEARCH);
}

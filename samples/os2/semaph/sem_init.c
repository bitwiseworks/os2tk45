/*static char *SCCSID = "@(#)sem_init.c	6.10 92/02/19";*/
/*==============================================================*\
 *  SEM_INIT.C - routines for initialization and exit processing*
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
 *   Init() - initialization routines                           *
 *   ExitProc(usTermCode) - exit list processing procedure      *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "semaph.h"

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

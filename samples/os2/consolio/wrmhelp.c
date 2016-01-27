/*=========================================================================\
 *                                                                         *
 *       FILE:wrmhelp.c                                                    *
 *                                                                         *
 *       DESCRIPTION:                                                      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1996, IBM Corp.                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *    *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *                               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *--------------------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 * WormHelp
 * SuspendAllThreads
 * SaveScreen
 * ResumeAllThreads
 * RedisplayScreen
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define  INCL_DOSMEMMGR
#define  INCL_KBD
#define  INCL_VIO
#define  INCL_MOU
#define  INCL_DOSPROCESS
#define  INCL_DOSSEMAPHORES
#include <os2.h>
#include <string.h>
#include "worms.h"
#include "wrmthrd.h"
#include "wrmhelp.h"

/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

CHAR *pszHelp =
"                                                                              \
                                                                                \
                        Worms  Mixed Mode Overview                              \
                                                                                \
                                                                                \
     This program is a sample application   intended to demonstrate calling     \
     16 - bit API's from 32 bit code.   The Vio, Mou, and KBD subsystem are     \
     used for this example. Each thread is started with a unique color code     \
     and  will  randomly  move  about the screen. Each time another worm is     \
     encountered  the  worms  color  will  change to red, then revert back.     \
                                                                                \
                        Worms Mixed Mode Help                                   \
                                                                                \
     Additional threads (Worms) may be started by entering 'A' at the keyboard. \
     Worms may be Deleted by entering 'D' at the keyboard. The mouse may also   \
     be used to add worms or delete worms by using the mouse buttons.           \
     To exit from the program enter 'E'.                                        \
                                                                                \
     From the command line the following options may be  specified.             \
                                                                                \
                                                                                \
      worms -s         Silent Mode                                              \
      worms -x         Where x  is a value from 1 to 30 indicating the number   \
                       of worms to start of with. Default is 1.                 \
                                                                                \
                                                                                \
                       Copyright     IBM Corp.   1992                           \
                                                                                \
                                                                                \
                                                                                \
";

PVOID pvSaveWindBuf;


/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:WormHelp()
 *
 *  Purpose: This routine displays the help screen for the
 *           worms program.
 *
 *
 *  Usage:
 *
 *  Method:    This routine is the main flow control for the help
 *          -  routine. The other threads will be suspended,the
 *             current screen is saved in a allocated buffer,
 *          -  display the help screen, wait for a keystroke,
 *          -  restore the old screen,resume child threads and
 *             free buffer.
 *          -
 *          -
 *
 *  Returns:
 *          TRUE    - Error Occurred.
 *          FALSE     No Error Occurred
\****************************************************************/
BOOL
WormHelp(VOID )
{

    KBDKEYINFO    kbdkeyinfo;
    PTIB   ptib ;
    PPIB   ppib  ;
    ULONG  ulPosts;

    bHelpEnabled  = TRUE;

     /*
      *freeze all worms
      */
      if(SuspendAllThreads() )
      {
          return(TRUE);
      }

     /*
      *read the screen
      *and save it
      */
      if(SaveScreen(&pvSaveWindBuf) )
      {
          return(TRUE);
      }

     /*
      *display the help screen
      */
      DisplayHelp();

      DosGetInfoBlocks(&ptib,&ppib);
      /*
       *if we are not
       *calling from thread
       *one suspend thread,
       *so only one thread at a time
       *is hitting the keyboard queue
       */
      if(ptib->tib_ptib2->tib2_ultid != 1 )
      {
          DosWaitEventSem(hevWormSem,SEM_INDEFINITE_WAIT);
          DosResetEventSem(hevWormSem,&ulPosts);
      }
      else
      {
         /*
          *wait for any
          *response if thread 1
          */
          KbdCharIn(&kbdkeyinfo,IO_WAIT,
                         0) ;
      }



     /*
      *redisplay the saved screen
      */
      ReDisplayScreen(pvSaveWindBuf);



     /*
      *allow all child threads to resume
      */
     bHelpEnabled  = FALSE;


     ResumeAllThreads();
     return(FALSE);

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:SuspendAllThreads()
 *
 *  Purpose: This routine allow the calling thread to force the
 *           other threads that are drawing to the screen to wait
 *           until this thread is done. The worm (threads) will block
 *           on the hevDrawSem until it is cleared again. The calling
 *           thread will wait until the post count on the hevDrawOK
 *           sem is equal to the number of worms before returnning.
 *
 *  Returns:
 *          FALSE - if successful execution completed
 *          TRUE  - if error
\****************************************************************/
BOOL
SuspendAllThreads(VOID )
{
     BOOL fError = FALSE;
     ULONG ulPosts = 0LU;

     fDraw = FALSE;

     /*
      *make sure we don't get
      *pre-empted
      */
     DosEnterCritSec();
     DosResetEventSem(hevDrawOk,&ulPosts);
     ulPosts = 0;

     DosResetEventSem(hevDrawSem,&ulPosts);
     ulPosts = 0;
     DosExitCritSec();
     /*
      *wait till all of the
      *threads have
      *posted
      */
     while(ulPosts < CurrentThread)
     {
          DosWaitEventSem(hevDrawOk,SEM_INDEFINITE_WAIT);
          DosQueryEventSem(hevDrawOk,&ulPosts);

     }

     return(fError );

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: DisplayHelp()
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
 *         TRUE   - Error occurred
 *         FALSE    No error occurred
\****************************************************************/
BOOL
DisplayHelp(VOID )
{
     BYTE bCell[2];
     BYTE bhAttr =  MKATRB(WM_PALEGRAY,WM_BLUE);


     bCell[0] = 0x20;
     bCell[1] = MKATRB(WM_PALEGRAY,WM_BLUE);


     VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);
     VioWrtCharStrAtt(pszHelp,strlen(pszHelp),
                              TOP_ROW,LEFT_COL,&bhAttr,hvio );
     return TRUE;
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ResumeAllThreads()
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
 *          FALSE - is always returned
\****************************************************************/
BOOL
ResumeAllThreads(VOID )
{

     fDraw = TRUE;
     DosPostEventSem(hevDrawSem);
     return( FALSE );
}
/*--------------------------------------------------------------*\
 *  End of file :wrmhelp.c
\*--------------------------------------------------------------*/





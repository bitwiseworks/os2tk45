/*=========================================================================\
 *                                                                         *
 *       FILE:worms.c                                                      *
 *                                                                         *
 *       DESCRIPTION:    Full Screen Mixed-mode programming sample         *
 *                                                                         *
 *      Created 1991  IBM Corp.                                            *
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
 *-------------------------------------------------------------------------*
 *  This source file contains the following functions:
 *
 *  main
 *  WormsMainProcess
 *  ProcessCmdLine
 *  ErrorMessage
 *  WormsExit
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/


#define  INCL_VIO
#define  INCL_KBD
#define  INCL_MOU
#define  INCL_DOSPROCESS
#define  INCL_DOSSEMAPHORES


#include <os2.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "worms.h"
#include "wrmthrd.h"
#include "init.h"
#include "wrmhelp.h"

/*--------------------------------------------------------------*\
 *Global definitions for this module
\*--------------------------------------------------------------*/


#define STACK_SIZE_WORMTHRD    32678
#define BOX_ROW 6
#define BOX_COL 10
#define HEIGHT_BOX 9
#define BOX_WIDTH  70
#define DEBUG   1

/*--------------------------------------------------------------*\
 *  Global  variables for this module
\*--------------------------------------------------------------*/

INT    CurrentThread = 0;
BOOL   fThreadsContinue = TRUE;
BOOL   fSilent = FALSE;
INT    StartWorms = 1;
BOOL   fQuit  = FALSE;                        /*Global quit flag all threads*/
BOOL   fDraw  = TRUE;                         /*Global draw flag all threads*/
BOOL   bHelpEnabled  = FALSE;
BOOL   bMessage      = FALSE;

VIOCURSORINFO viociCursor;
VIOMODEINFO   VioModeInfo;
HVIO  hvio = 0 ;
THREAD_DATA ThreadData[] =
/*                                       background/text                           */
/*  TID Active Row           Col          Attribute               sCurrentDirection*/
{
    0L, FALSE, BOT_ROW  / 2 ,FIRST_COL  ,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL    ,MKATRB(WM_BROWN,WM_CYAN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL / 2,MKATRB(WM_MAGENTA,WM_PALEGRAY),0,
    0L, FALSE, FIRST_ROW + 5,RGT_COL / 4,MKATRB(WM_LRED,WM_GREEN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL - 3,MKATRB(WM_YELLOW,WM_WHITE)    ,0,
    0L, FALSE, BOT_ROW  - 4 ,FIRST_COL  ,MKATRB(WM_GREEN,WM_LBLUE)     ,0,
    0L, FALSE, FIRST_ROW + 4,RGT_COL    ,MKATRB(WM_BROWN,WM_YELLOW)    ,0,
    0L, FALSE, BOT_ROW  - 2 ,RGT_COL / 2,MKATRB(WM_DKGREY,WM_GREEN)    ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL / 4,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, BOT_ROW  - 3 ,RGT_COL - 3,MKATRB(WM_WHITE,WM_LBLUE)     ,0,

    0L, FALSE, BOT_ROW  / 2 ,FIRST_COL  ,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL    ,MKATRB(WM_BROWN,WM_CYAN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL / 2,MKATRB(WM_MAGENTA,WM_PALEGRAY),0,
    0L, FALSE, FIRST_ROW + 5,RGT_COL / 4,MKATRB(WM_LRED,WM_GREEN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL - 3,MKATRB(WM_YELLOW,WM_WHITE)    ,0,
    0L, FALSE, BOT_ROW  - 4 ,FIRST_COL  ,MKATRB(WM_GREEN,WM_LBLUE)     ,0,
    0L, FALSE, FIRST_ROW + 4,RGT_COL    ,MKATRB(WM_BROWN,WM_YELLOW)    ,0,
    0L, FALSE, BOT_ROW  - 2 ,RGT_COL / 2,MKATRB(WM_DKGREY,WM_GREEN)    ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL / 4,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, BOT_ROW  - 3 ,RGT_COL - 3,MKATRB(WM_WHITE,WM_LBLUE)     ,0,

    0L, FALSE, BOT_ROW  / 2 ,FIRST_COL  ,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL    ,MKATRB(WM_BROWN,WM_CYAN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL / 2,MKATRB(WM_MAGENTA,WM_PALEGRAY),0,
    0L, FALSE, FIRST_ROW + 5,RGT_COL / 4,MKATRB(WM_LRED,WM_GREEN)      ,0,
    0L, FALSE, BOT_ROW      ,RGT_COL - 3,MKATRB(WM_YELLOW,WM_WHITE)    ,0,
    0L, FALSE, BOT_ROW  - 4 ,FIRST_COL  ,MKATRB(WM_GREEN,WM_LBLUE)     ,0,
    0L, FALSE, FIRST_ROW + 4,RGT_COL    ,MKATRB(WM_BROWN,WM_YELLOW)    ,0,
    0L, FALSE, BOT_ROW  - 2 ,RGT_COL / 2,MKATRB(WM_DKGREY,WM_GREEN)    ,0,
    0L, FALSE, FIRST_ROW    ,RGT_COL / 4,MKATRB(WM_BLUE,WM_RED)        ,0,
    0L, FALSE, BOT_ROW  - 3 ,RGT_COL - 3,MKATRB(WM_WHITE,WM_LBLUE)     ,0,
};
CHAR *pszBox [HEIGHT_BOX]=
{
"ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออป",
"บ                                                        บ",
"บ               Process  Message:                        บ",
"บ                                                        บ",
"บ                                                        บ",
"บ                                                        บ",
"บ                                                        บ",
"บ                                                        บ",
"ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ",
};
CHAR *pszWormMessages [] =
{
     "Worms Mixed-Mode Program Normal Exit",
     "Undetermined Error Occurred",
     "Worms Mixed Mode Program Initialization Failure",
     "Maximum Number of Threads Reached for this Process",
     "Error Creating Thread",
     "Unable To Display Help Screen",
     "Unable To Create  Mouse Event Thread",
     "Hit Any Key to Exit Process",
     "Waiting for Threads To Exit",
     "Any Key to Continue",
};

#define    MAX_ERROR_MSGS (sizeof (pszErrorMessage) )
#define    MAX_THREADS    (( sizeof(ThreadData) / sizeof(THREAD_DATA)) - 1)
/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/
INT
main (SHORT sArgc , CHAR *ppArgv[]);
VOID
WormsExit(SHORT sExitStatus);
BOOL
WormsMainProcess(SHORT *psStatus);
BOOL
ProcessCmdLine(SHORT sArgc,CHAR **ppArgv);

/****************************************************************\
 *  Routine Name:main()
 *--------------------------------------------------------------
 *
 *  Name: main()
 *
 *  Purpose:Main entry point for  the worms program
 *
 *
 *
 *  Usage: Main entry point for the worms program.
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
INT
main (SHORT sArgc,CHAR  *ppArgv[])
{

    SHORT     sExitStatus = WRM_NORMAL_EXIT;

    for(;;)
    {
         /*
          *process the command line
          */
         if(ProcessCmdLine(sArgc,ppArgv) )
         {
               break;
         }

         /*
          *initialize resources
          */
         if(Init(&sExitStatus) )
         {
               ErrorMessage(sExitStatus,TRUE);
               break;
         }
         /*
          *start up the main process
          */
         if(WormsMainProcess(&sExitStatus) )
         {
               ErrorMessage(sExitStatus,TRUE);
               break;
         }
         break;
    };
    WormsExit(WRM_NORMAL_EXIT);

    return( (INT) sExitStatus);
}      /*  end of main()  */

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:WormsMainProcess
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
 *          FALSE - if sucessful execution completed
 *          TRUE  - if error ,See *psStatus for error definition
\****************************************************************/
BOOL
WormsMainProcess(SHORT *psStatus)
{
    BOOL fError = FALSE;
    KBDKEYINFO    kbdkeyinfo;
    USHORT      usId;
    ULONG ulPosts;


    do
    {
          /*
           *sit here and wait on
           *user input
           */
          KbdCharIn(&kbdkeyinfo,IO_WAIT,
                     0) ;
          /*
           *if help was enabled don't allow any other input
           */
          if(!bHelpEnabled && !bMessage)
          {
               /*
                *regular keys
                */
               if( kbdkeyinfo.chChar )
               {
                    switch(kbdkeyinfo.chChar)
                    {
                    case 'd':
                    case 'D':
                           WormDelete();
                           break;
                    case 'e':
                    case 'E':
                         fQuit = TRUE;
                         fDraw = FALSE;
                         break;
                    case 'a':
                    case 'A':
                         WormCreate();
                         break;
                    case 'h':
                    case 'H':
                         if( WormHelp() )
                         {
                              ErrorMessage(ERROR_DISP_HELP,TRUE);
                         }
                         break;
                    default:
                         DosBeep(1200,175);
                         break;
                    }
               }
               /*
                *extended keys
                */
               else
               {
                    switch(kbdkeyinfo.chScan )
                    {
                    case F1:
                         if( WormHelp() )
                         {
                              ErrorMessage(ERROR_DISP_HELP,TRUE);
                         }
                         break;
                    default:
                         DosBeep(1200,175);
                         break;
                    }
               }

          }
          /*
           *another threads  waiting for keyboard input
           *free them up
           */
          else
          {
               DosPostEventSem(hevWormSem);
          }

    }while(!fQuit );

    /*
     *set each thread as inactive
     */
    for(usId = 2; usId <= MAX_THREADS + 2; usId++)
    {
          ThreadData[usId - 2].fActive = FALSE;
    }

     /*
      *make sure we don't get
      *pre-empted
      */
     DosEnterCritSec();
     ulPosts = 0;
     DosResetEventSem(hevDrawOk,&ulPosts);
     DosExitCritSec();

     /*
      *wait till all of the
      *threads have
      *posted on the way out
      */
     while(ulPosts < CurrentThread)
     {
          DosWaitEventSem(hevDrawOk,SEM_INDEFINITE_WAIT);
          DosQueryEventSem(hevDrawOk,&ulPosts);
     }
    /*
     *display message that process is shutting down
     */
    Message(WAITING_THREADS_EXIT,FALSE,FALSE,TRUE,FALSE);
    return ( fError) ;
}/*main process loop */

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: WormCreate()
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
 *          TRUE   - Error occurred
 *          FALSE  - No error occurred
\****************************************************************/
BOOL
WormCreate(VOID )
{

    if(CurrentThread  == MAX_THREADS)
    {
          /*
           *thread limit exceeeded
           */
           ErrorMessage(MAX_THREADS_EXCEEDED,TRUE );
           return(TRUE);

    }
    ThreadData[CurrentThread].fActive = TRUE;
    ThreadData[CurrentThread].tidWorm =
                            _beginthread( WormThread,
                                       NULL,
                                       STACK_SIZE_WORMTHRD,
                                       &ThreadData[CurrentThread]);
    if(ThreadData[CurrentThread].tidWorm ==  -1 )
    {
          ErrorMessage(ERROR_CREATING_THREAD,TRUE );
          return(TRUE);
    }
    CurrentThread++;

    return(FALSE);

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: ErrorMessage()
 *
 *  Purpose: Display Error Messages to the user.
 *
 *
 *
 *  Usage: sErrorMessage - Message number to display.
 *         fBeep         - Beep or not.
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
 *  Returns:   VOID
\****************************************************************/
VOID ErrorMessage(SHORT sErrorMessage,BOOL fBeep)

{

    KBDKEYINFO    kbdkeyinfo;
    BYTE  bAttr  = MKATRB(WM_RED,WM_WHITE );
    USHORT usAttrib = VP_WAIT | VP_OPAQUE;
    USHORT  usRow,usIndex;
    USHORT  usStatus = MOUSE_DISABLED;
    BYTE bCell[2];
    PVOID pvSaveWindBuf;
    PTIB   ptib ;
    PPIB   ppib  ;
    ULONG  ulPosts;

    bCell[0] = 0x20;
    bCell[1] = WM_BACKGROUND;
    bMessage = TRUE;


    if(fBeep)
    {
          DosBeep(675,1200);
    }

     /*
      *freeze all threads
      *while pop-up is up
      */
     SuspendAllThreads();
     /*
      *make sure each thread has had enough time
      *to complete
      *what they are doin before waiting on the drawing semaphore
      */
//   DosSleep(PAUSE_TIME * 40);
     /*
      *read the screen and save
      *it
      */
     if(SaveScreen(&pvSaveWindBuf) )
     {
          return;
     }

     VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);


    /*
     *put up the  message box
     */
     for(usRow = BOX_ROW,usIndex = 0;usIndex < HEIGHT_BOX; usIndex++,
                                                          usRow++ )
     {
        VioWrtCharStrAtt(pszBox[usIndex],strlen(pszBox[usIndex]),
                     usRow,
                     BOX_COL,
                     &bAttr,
                     hvio );
     }

     /*
      *display
      *error message
      */
      VioWrtCharStrAtt( pszWormMessages[sErrorMessage],
                         strlen(pszWormMessages[sErrorMessage]),
                         BOX_ROW + 5,
                         ( ((LAST_COL - FIRST_COL ) -
                                    strlen(pszWormMessages[sErrorMessage])) / 2) ,
                         &bAttr,
                         hvio );
       /*
        *any key to continue
        */
       VioWrtCharStrAtt( pszWormMessages[ANY_KEY_TO_CONTINUE],
                         strlen(pszWormMessages[ANY_KEY_TO_CONTINUE]),
                         BOX_ROW + 6,
                         ( ((LAST_COL - FIRST_COL ) -
                                    strlen(pszWormMessages[ANY_KEY_TO_CONTINUE])) / 2) ,
                         &bAttr,
                         hvio );

      DosGetInfoBlocks(&ptib,&ppib);
      /*
       *if we are not
       *calling from thread
       *one suspend the calling thread
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

      VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);
      /*
       *restore the screen
       */
      ReDisplayScreen(pvSaveWindBuf);

       /*
        *resume all threads
        */
     ResumeAllThreads();

     bMessage = FALSE;
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: Message()
 *
 *  Purpose:Generic message routine. Allows all threads to continue,
 *          while message is up.
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
 *  Returns:   VOID
 *
 *
\****************************************************************/
VOID Message( SHORT sErrorMessage,BOOL fBeep,BOOL fKeyBoardWait,
                   BOOL fClsConsole,BOOL fSuspendThreads )
{
    KBDKEYINFO    kbdkeyinfo;
    BYTE  bAttr  = MKATRB(WM_RED,WM_WHITE );
    USHORT usAttrib = VP_WAIT | VP_OPAQUE;
    USHORT  usRow,usIndex;
    BYTE bCell[2];
    BYTE bhAttr = WM_BACKGROUND;

    bCell[0] = 0x20;
    bCell[1] = WM_BACKGROUND;

     /*
      *display
      *the error message to the
      *screen
      */
      if(fBeep)
      {
          DosBeep(675,1200);
      }

      if(fSuspendThreads)
      {
          SuspendAllThreads();
      }

      DosSleep(1000L);
      if(fClsConsole)
      {
          /*
           *clear the screen
           */
          VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);
      }

       /*
        *put up the  message box
        */

       for(usRow = BOX_ROW,usIndex = 0;usIndex < HEIGHT_BOX; usIndex++,
                                                             usRow++ )
       {
          VioWrtCharStrAtt(pszBox[usIndex],strlen(pszBox[usIndex]),
                        usRow,
                        BOX_COL,
                        &bAttr,
                        hvio );
       }

       /*
        *display
        *message
        */
       VioWrtCharStrAtt( pszWormMessages[sErrorMessage],
                         strlen(pszWormMessages[sErrorMessage]),
                         BOX_ROW + 5,
                         ( ((LAST_COL - FIRST_COL ) -
                                    strlen(pszWormMessages[sErrorMessage])) / 2) ,
                         &bAttr,
                         hvio );

       /*
        *sit here and wait on
        *user input
        */
       if(fKeyBoardWait)
       {

           VioWrtCharStrAtt( pszWormMessages[sErrorMessage],
                             strlen(pszWormMessages[sErrorMessage]),
                             BOX_ROW + 6,
                             ( ((LAST_COL - FIRST_COL ) -
                                        strlen(pszWormMessages[ANY_KEY_TO_CONTINUE])) / 2) ,
                             &bAttr,
                             hvio );
           KbdCharIn(&kbdkeyinfo,IO_WAIT,
                      0 ) ;
       }

}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:WormsExit()
 *
 *  Purpose: Generic exit routine. All exit processing should be done
 *           at this point.
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
 *  Returns:   VOID
 *
 *
\****************************************************************/
VOID
WormsExit(SHORT sExitStatus)
{

     USHORT usRow,usCol;
     BYTE bCell[2];
     BYTE bhAttr = MKATRB(WM_BLUE,WM_RED);

     bCell[0] = 0x20;
     bCell[1] = WM_BACKGROUND;


     /*
      *do any clean up
      *necessary
      */


     /*
      *clear the screen
      */
     VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);

     /*
      *reset the old
      *cursor
      */
     VioSetCurType(&viociCursor,hvio);
     /*
      *display the exit message
      */
     VioWrtCharStrAtt( pszWormMessages[sExitStatus],
                       strlen(pszWormMessages[sExitStatus]),
                       2,1,
                       &bhAttr,hvio );
     VioSetCurPos(3,2,hvio);

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ProcessCmdLine()
 *
 *  Purpose:Parse arguments passed on the command line.
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
 *        FALSE   - at present time
 *
\****************************************************************/
BOOL ProcessCmdLine(SHORT sArgc,CHAR **ppArgv)
{
     SHORT sArgnum = 1;
     BOOL  fError = FALSE;

     while(--sArgc )
     {
          if( *ppArgv[sArgnum] == '-'   ||
              *ppArgv[sArgnum] == '/' )
          {

               switch(*(ppArgv[sArgnum] + 1) )
               {
               case 'S':
               case 's':
                    fSilent = TRUE;
                    break;
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                    StartWorms = atoi(ppArgv[sArgnum] + 1);
                    if(StartWorms > MAX_THREADS )
                    {
                         StartWorms = MAX_THREADS;
                    }
                    break;
               }
          }
          sArgnum++;
     }
     return(fError);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:WormDelete()
 *
 *  Purpose:Set the active flag to false for the last active
 *  thread to FALSE.
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
 *          VOID
\****************************************************************/
VOID WormDelete(VOID)
{

      if(CurrentThread > 0)
      {
         ThreadData[--CurrentThread].fActive = FALSE;
      }
      else
      {
          DosBeep(675,1200);
      }
}    /*  end of WormDelete()  */




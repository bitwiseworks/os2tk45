/*=========================================================================\
 *                                                                         *
 *       FILE:init.c                                                       *
 *                                                                         *
 *       DESCRIPTION: This file handles all initialization necessary       *
 *                    for the worms program                                *
 *                                                                         *
 *      Copyright 1992 IBM Corp.                                           *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *    *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *                               *
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
 *  Init()
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define  INCL_VIO
#define  INCL_DOSPROCESS
#define  INCL_MOU
#define  INCL_DOSSEMAPHORES

#include <os2.h>
#include <stdlib.h>
#include "worms.h"
#include "disp.h"
#include "init.h"
#include "wmouse.h"




/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

#define CREATE_MOUSE  1
#define STACK_SIZE_MOUTHRD  32768
#define WORM_SEM_NAME  NULL
#define WORM_DRAW_NAME NULL
HEV   hevWormSem;
HEV   hevDrawSem;
HEV   hevDrawOk;





/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

BOOL
create_wmThread(VOID );

BOOL bStartWorms(VOID );
/****************************************************************\
 *  Routine Name:Init()
 *--------------------------------------------------------------
 *
 *  Name:
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
 *       TRUE  - Error Occurred
 *       FALSE - No error occurred
\****************************************************************/
BOOL
Init(PSHORT psErrorMessage)
{
     BOOL fError = FALSE;
     VIOCURSORINFO viociTemp;

     /*
      *get the current configuration
      */
     VioModeInfo.cb = sizeof(VioModeInfo);
     VioGetMode(&VioModeInfo,hvio );

     /*
      *save the cursor type
      */
     VioGetCurType(&viociCursor,hvio);
     VioGetCurType(&viociTemp,hvio);

     /*
      *turn off the cursor
      */
      viociTemp.attr = 0xFFFF;
      VioSetCurType(&viociTemp,hvio);



     /*
      *display the main default window
      */
     WindMainDisp();

     if(create_wmThread() )
     {
          ErrorMessage(ERROR_CREATING_WREADTHRD,TRUE);
     }

     /*
      *create a general purpose
      *semaphore
      */
     if(DosCreateEventSem(WORM_SEM_NAME,
                           &hevWormSem,
                           0,
                           FALSE ) )
     {
          fError = TRUE;
     }
     /*
      *create a sem
      *to contro the threads that
      *draw to the screen
      */
     if(DosCreateEventSem(WORM_DRAW_NAME,
                           &hevDrawSem,
                           0,
                           TRUE) )
     {
          fError = TRUE;
     }

     if(DosCreateEventSem(NULL,
                           &hevDrawOk,
                           DC_SEM_SHARED,
                           FALSE ) )
     {
          fError = TRUE;
     }

     /*
      *start up
      *the number of worms
      *specified on the command
      *line
      */
     bStartWorms();

     return(fError);

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:create_wmThread()
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
 *          TRUE  - if error
\****************************************************************/
BOOL
create_wmThread(VOID )
{
    SHORT sValue;

    if( _beginthread( WReadMouse,
                      NULL,
                      STACK_SIZE_MOUTHRD,
                      &sValue) == -1 )
    {
          return(TRUE);
    }

    return(FALSE);


}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:bStartWorms()
 *
 *  Purpose:Kick off the number off worms that was specified on the command
 *          line
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
 *          TRUE  - An error occurred
 *          FALSE - No error  occurred
\****************************************************************/
BOOL bStartWorms(VOID )
{

     INT Worms;


     for(Worms = 0; Worms < StartWorms; Worms++ )
     {
          if( WormCreate() )
          {
               return(TRUE);
          }
     }
     return(FALSE);
}







/*--------------------------------------------------------------*\
 *  End of file : init.c
\*--------------------------------------------------------------*/

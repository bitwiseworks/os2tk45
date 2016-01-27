/*=========================================================================\
 *                                                                         *
 *       FILE:wmouse.c                                                     *
 *                                                                         *
 *       DESCRIPTION:                                                      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Created 1991  IBM Corp.                                            *
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
#define  INCL_DOS
#define  INCL_VIO
#define  INCL_KBD
#define  INCL_MOU
#define  INCL_DOSPROCESS

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "worms.h"
#include "wrmthrd.h"
#include "wrmhelp.h"



/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

MOUEVENTINFO mouev;
HMOU hmou;
/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/
VOID WmouseFunctions(SHORT sCol);
/****************************************************************\
 *  Routine Name:
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
 *
 *
\****************************************************************/
VOID WReadMouse(PVOID pvMessageData)
{

     USHORT fWait = MOU_WAIT;
     NOPTRRECT mourt = { 0,0,24,79 };


     /*
      *open the handle
      *for the mouse
      */
     MouOpen(NULL,&hmou);
     MouDrawPtr(hmou);

     do
     {
          /*
           *sit and wait on
           *the event queue
           */
          MouReadEventQue(&mouev,&fWait,hmou);

          if(mouev.time && !bHelpEnabled)
          {
               if(mouev.fs & MOUSE_BN1_DOWN )
               {
                    MouRemovePtr(&mourt,hmou);
                    /*
                     *if the button is clicked
                     *on any of the available functions
                     */
                    if(mouev.row == (TOP_ROW + 2) )
                    {
                         WmouseFunctions(mouev.col);
                    }

               }
               MouDrawPtr(hmou);
          }

     }while(!fQuit );


     DosExit(EXIT_THREAD, 0L );
}

/****************************************************************\
 *
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
 *          VOID
 *
\****************************************************************/
VOID WmouseFunctions(SHORT sCol)
{

     /*
      *the add selection
      */
     if(sCol >= 5  && sCol <= 7)
     {
          WormCreate();
     }
     /*
      *the delete
      *key
      */
     else if(sCol >= 22 && sCol  <=  27)
     {
          WormDelete();
     }
     /*
      *exit the process
      */
     else if(sCol >= 47 && sCol  <=  50 )
     {

         /*
          *display hit any key to exit
          */
         fQuit = TRUE;
         Message(ENTER_TO_EXIT,FALSE,FALSE,TRUE,TRUE);
     }

     /*
      *the help menu
      */
     else if(sCol >= 62 && sCol  <=  65 )
     {

        if( WormHelp() )
        {
               ErrorMessage(ERROR_DISP_HELP,TRUE);
        }
     }
     else
     {
          DosBeep(600,175);
     }


}
/*--------------------------------------------------------------*\
 *  End of file : wmouse.c
\*--------------------------------------------------------------*/

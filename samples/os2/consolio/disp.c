/*=========================================================================\
 *                                                                         *
 *       FILE:disp.c                                                       *
 *                                                                         *
 *       DESCRIPTION:                                                      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*
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
 *-------------------------------------------------------------------------*
 *
 *  This source file contains the following functions:
 *
 *
 *  Disp()
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define  INCL_VIO
#define  INCL_KBD
#define  INCL_MOU
#define  INCL_DOSPROCESS
#include <os2.h>
#include <string.h>
#include "worms.h"
#include "wrmthrd.h"


/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/
/*
 *use a character cell
 *combo attribute for the
 *heading
 */

CHAR *pszMainWindow1=
" \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27W\27O\27R\27M\27S\27 \27 \27\
 \27 \27 \27 \27M\27I\27X\27E\27D\27 \27M\27O\27D\27E\27 \27 \27 \27 \27P\27R\27O\27G\27R\27A\27M\27 \27 \27 \27\
 \27 \27 \27 \27 \27 \27 \27 \27S\27A\27M\27P\27L\27E\27 \27 \27 \27 \27 \27 \27 \27 \27 \27";

CHAR *pszMainWindow2=
"Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27\
Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27\
Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27Ä\27";


CHAR *pszMainWindow3=
" \27 \27 \27 \27 \27A\24d\27d\27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27D\24e\27l\27e\27t\27e\27\
 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27 \27E\24x\27i\27t\27 \27 \27 \27 \27 \27\
 \27 \27 \27 \24 \24 \27H\24e\27l\27p\27 \27 \27 \27 \27 \27 \27 \27 \27 \27";

USHORT usSize;
#define SIZ_WIND_BUFFER      8192

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

/****************************************************************\
 *  Routine Name:DispMainWindow()
 *--------------------------------------------------------------
 *
 *  Name:WindMainDisp()
 *
 *  Purpose:
 *    Display the main window for the worms program.
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
VOID WindMainDisp( VOID )
{
     BYTE bCell[2];

     bCell[0] = 0x20;
     bCell[1] = ( WM_BLUE << 4 ) + WM_WHITE ;

     /*
      *clear the screen
      */
     VioScrollDn(TOP_ROW,LEFT_COL,0xFFFF,0xFFFF,0xFFFF,bCell,hvio);
     /*
      *display the main window
      */
     VioWrtCellStr(pszMainWindow1,strlen(pszMainWindow1),
                       TOP_ROW,LEFT_COL,hvio );

     VioWrtCellStr(pszMainWindow2,strlen(pszMainWindow2),
                       TOP_ROW + 1,LEFT_COL,hvio );

     VioWrtCellStr(pszMainWindow3,strlen(pszMainWindow3),
                       TOP_ROW + 2,LEFT_COL,hvio );
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:SaveScreen()
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
SaveScreen( PVOID *ppvWindow )
{


     usSize = SIZ_WIND_BUFFER;

     /*
      *allocate a buffer big enough to
      *save the full screen size plus the attribute
      */
     if(DosAllocMem(ppvWindow,(ULONG)usSize,fALLOC)  )
     {
          return(TRUE);

     }
     VioReadCellStr(*ppvWindow,&usSize,TOP_ROW,LEFT_COL,
                                   hvio );
     return(FALSE);

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ReDisplayScreen()
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
 *           VOID
 *
\****************************************************************/
VOID
ReDisplayScreen(PVOID pvWindBuf )
{


      VioWrtCellStr(pvWindBuf,usSize,TOP_ROW,LEFT_COL,
                                   hvio );

      /*
       *free up the memory we allocated
       */
      DosFreeMem(pvWindBuf);

}
/*--------------------------------------------------------------*\
 *  End of file : disp.c
\*--------------------------------------------------------------*/

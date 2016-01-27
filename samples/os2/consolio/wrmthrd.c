/*=========================================================================\
 *                                                                         *
 *       FILE: wrmthrd.c                                                   *
 *                                                                         *
 *       DESCRIPTION:                                                      *
 *                                                                         *
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
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *
 *  move_worm_up
 *  move_worm_down
 *  move_worm_right
 *  move_worm_left
 *  move_worm_zigup
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
#define  INCL_DOSSEMAPHORES

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "worms.h"
#include "wrmthrd.h"
/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/
CHAR *pszLeftWorm   =      "<<<------->";
CHAR *pszRightWorm  =      "<------->>>";
CHAR *pszBlank      =      "               ";
CHAR achUpWorm[10]  =      {'^','^','|','|','|','v'};
CHAR achDownWorm[10]=      {'^','^','|','|','|','v'};
SHORT asWidths   [] =      {2,4,8,16};
SHORT asHorizWidths  [] =  {HOR_WIDTH / 2, HOR_WIDTH /8,HOR_WIDTH,HOR_WIDTH / 3};
SHORT asVertHeights  [] =  {HEIGHT / 4,HEIGHT / 8, HEIGHT / 2, HEIGHT  };
INT   Direction = 0;
INT   VertHeight = 0;
SHORT sRefresh = 1;
#define DEBUG              0
#define MAX_LEN_WORM       48
#define FILE_NAME          "TEST.???"

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

VOID move_worm_up( USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                     PBYTE  pbAttribute,
                     SHORT *psCurrentDirection,
                     LONG  tidWorm);
VOID move_worm_down( USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                     PBYTE  pbAttribute,
                     SHORT *psCurrentDirection,
                     LONG  tidWorm);
VOID move_worm_right( USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                     PBYTE  pbAttribute,
                     SHORT *psCurrentDirection,
                     LONG  tidWorm);
VOID move_worm_left( USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                     PBYTE  pbAttribute,
                     SHORT *psCurrentDirection,
                     LONG  tidWorm);
VOID move_worm_zigup(USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                      PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG tidWorm);
/****************************************************************\
 *  Routine Name:wrmthrd()
 *--------------------------------------------------------------
 *
 *  Name:
 *
 *  Purpose: This routine allows each thread to draw to the screen.
 *           Each thread will stay in this loop, until the thread
 *           dies, or is terminated. Each thread may draw to the
 *           unless the semaphore is set by thread one or indicating
 *           that one of the parent threads wants control of the
 *           screeen.
 *
 *  Returns: There is no return value. The thread terminates.
 *
\****************************************************************/
VOID
WormThread(PVOID pvMessage)
{

     PTHREAD_DATA pThreadData;
     SHORT  sDummy;
     SHORT  asDirection [] = { UP,RIGHT,LEFT,DOWN};



     srand(sDummy);
     pThreadData =  (PTHREAD_DATA) pvMessage;
     /*
      *start the worm  off
      */
     if(Direction == ( sizeof(asDirection) / sizeof(SHORT) ) )
     {
          Direction = 0;
     }
     pThreadData->sCurrentDirection = asDirection[Direction++];
     do
     {
          switch(pThreadData->sCurrentDirection)
          {
          case UP:
               move_worm_up( &pThreadData->usCurrentRow,&pThreadData->usCurrentCol,
                             &pThreadData->bAttribute,
                             &pThreadData->sCurrentDirection,
                              pThreadData->tidWorm);
               break;
          case DOWN:
               move_worm_down( &pThreadData->usCurrentRow,&pThreadData->usCurrentCol,
                               &pThreadData->bAttribute,
                               &pThreadData->sCurrentDirection,
                                pThreadData->tidWorm);
               break;

          case LEFT:
               move_worm_left( &pThreadData->usCurrentRow,&pThreadData->usCurrentCol,
                               &pThreadData->bAttribute,
                               &pThreadData->sCurrentDirection,
                                pThreadData->tidWorm );
               break;
          case RIGHT:
               move_worm_right( &pThreadData->usCurrentRow,&pThreadData->usCurrentCol,
                                &pThreadData->bAttribute,
                                &pThreadData->sCurrentDirection,
                                 pThreadData->tidWorm );
               break;
          case ZIG_UP:
               move_worm_zigup( &pThreadData->usCurrentRow,&pThreadData->usCurrentCol,
                             &pThreadData->bAttribute,
                             &pThreadData->sCurrentDirection,
                              pThreadData->tidWorm );
               break;
          default:
               pThreadData->usCurrentRow = ( LAST_ROW - FIRST_ROW ) /2;
               pThreadData->usCurrentCol = ( LAST_COL - FIRST_COL ) /2;
               if(Direction == ( sizeof(asDirection) / sizeof(SHORT) ) )
               {
                    Direction = 0;
               }
               pThreadData->sCurrentDirection = asDirection[Direction++];
               break;
          }
          DosSleep(PAUSE_TIME);
          /*
           *if set,then the thread that services the console,or the
           *thread that services the mouse queue wants control of
           *the screen
           */
          DosPostEventSem(hevDrawOk);
          DosWaitEventSem(hevDrawSem,SEM_INDEFINITE_WAIT);

        }while(pThreadData->fActive);

     pThreadData->tidWorm = 0;
     /*
      *if the process is to exit
      *post the semaphore on the way out
      */
      if(fQuit)
      {
          DosPostEventSem(hevDrawOk);
      }

    /*
     *should alway be called
     *to free any runtime resources this thread
     *might be using
     */
    _endthread();

}/*WormThread*/
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:move_worm_down()
 *
 *  Purpose: Draw the worm down the screen vertically and set up to go to
 *           another direction.
 *  Returns: VOID
 *
\****************************************************************/
VOID move_worm_down(USHORT *pusCurrentRow,USHORT *pusCurrentCol,PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG  tidWorm )

{

     USHORT sRow;
     SHORT  sPos = 0;
     SHORT  sStartRow;
     CHAR  achCell[2];
     BYTE  bAttribute;
     USHORT  usCount;
     BOOL  fOnce = TRUE;
     BYTE  bBackGround = WM_BACKGROUND;

     if(VertHeight  == (sizeof(asVertHeights) / sizeof(SHORT ) ) )
     {
          VertHeight = 0;
     }
     if(*pusCurrentRow <= FIRST_ROW || *pusCurrentRow >= LAST_ROW )
     {
          *pusCurrentRow = ( LAST_ROW - asVertHeights[VertHeight++]) ;
     }

     if(*pusCurrentRow < FIRST_ROW || *pusCurrentRow > LAST_ROW )
     {
          *pusCurrentRow = (FIRST_ROW + 1);
     }
     sStartRow = *pusCurrentRow;
     usCount = sizeof(achCell);
     bAttribute = *pbAttribute;
     for(sRow = (*pusCurrentRow + WIDTH); *pusCurrentRow < LAST_ROW && fDraw; (*pusCurrentRow)++ )
     {
          /*
           *read the current attribute
           *on the screen
           *if another worm
           */
          VioReadCellStr(achCell,&usCount,*pusCurrentRow,*pusCurrentCol,hvio);
          if(!fSilent)
          {
               if(achCell[0] != ' ' && fOnce )
               {
                    DosBeep(600,175);
                    DosBeep(1200,175);
                    bAttribute = 0x4F;
                    fOnce = FALSE;
               }
          }
          VioWrtCharStrAtt( &achDownWorm[sPos],1,
                            *pusCurrentRow,*pusCurrentCol,&bAttribute,
                            hvio );
          DosSleep(PAUSE_TIME);
          if(!achUpWorm[sPos++] )
          {
                for(; sStartRow <= (*pusCurrentRow ) && fDraw;sStartRow++)
                {
                    VioWrtCharStrAtt( &pszBlank[0],1,
                                      sStartRow,*pusCurrentCol,&bBackGround,
                                      hvio );
                }
                sPos = 0;
          }

     }
     /*
      *clean off any tail we
      *left behind
      */
     if(sPos)
     {
          for(; sStartRow <= (*pusCurrentRow ) ;sStartRow++)
          {
              VioWrtCharStrAtt( &pszBlank[sPos],1,
                                sStartRow,*pusCurrentCol,&bBackGround,
                                hvio );
          }
     }

    /*
     *if we stopped drawing
     *come back to the same spot
     */
    if(fDraw)
    {
         *psCurrentDirection = (rand() % MAX_DIRECTIONS);
         *pusCurrentRow = (rand() % HEIGHT);
         *pusCurrentCol = ( (rand() % HOR_WIDTH) /  asWidths[rand() %  4] + 1);
    }

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:move_worm_up()
 *
 *  Purpose:Redraw the worm moving vertically up the screen.
 *          Draw until the bottom of the screen or fDraw flag has
 *          been reset.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID move_worm_up(USHORT *pusCurrentRow,USHORT *pusCurrentCol,PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG tidWorm )

{

     USHORT sRow;
     SHORT  sPos;
     SHORT  sStartRow;
     BYTE  bBackGround = WM_BACKGROUND;
     static INT VertHeight = 0;

     if(VertHeight  == (sizeof(asVertHeights) / sizeof(SHORT ) ) )
     {
          VertHeight = 0;
     }
     if(*pusCurrentRow <= FIRST_ROW || *pusCurrentRow >= LAST_ROW )
     {
          *pusCurrentRow = ( LAST_ROW - asVertHeights[VertHeight]) ;
     }
     sStartRow = *pusCurrentRow;
     for(sRow = (*pusCurrentRow - WIDTH),sPos = 0;
                            (*pusCurrentRow > FIRST_ROW )&& fDraw;
                            (*pusCurrentRow)--)
     {
          /*
           *read the current attribute
           *on the screen
           *if another worm beep
           */
          VioWrtCharStrAtt( &achUpWorm[sPos],1,
                            *pusCurrentRow,*pusCurrentCol,pbAttribute,
                            hvio );
          DosSleep(PAUSE_TIME);
          if(!achUpWorm[sPos++] )
          {
                for(; sStartRow >= (*pusCurrentRow ) && fDraw;sStartRow--)
                {
                    VioWrtCharStrAtt( &pszBlank[0],1,
                                      sStartRow,*pusCurrentCol,&bBackGround,
                                      hvio );
                }
                sPos = 0;
          }


     }
     /*
      *clean up any head on the screen
      */
    if(sPos)
    {
          for(; sStartRow >= (*pusCurrentRow ) ;sStartRow--)
          {
              VioWrtCharStrAtt( &pszBlank[0],1,
                                sStartRow,*pusCurrentCol,&bBackGround,
                                hvio );
          }
    }

    /*
     *if we stopped drawing
     *come back to the same spot
     */
    if(fDraw)
    {
         *psCurrentDirection = ( rand() % MAX_DIRECTIONS);
         *pusCurrentRow = ( (rand() % HEIGHT)  );
         *pusCurrentCol = ( (rand() % HOR_WIDTH)/  asWidths[rand() %  4] + 1) ;
    }
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:move_worm_right()
 *
 *  Purpose:Move the worm to the right side of the screen and wrap
 *          around.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID move_worm_right(USHORT *pusCurrentRow,USHORT *pusCurrentCol,PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG  tidWorm )

{
     SHORT sCol;
     USHORT usSize;
     BYTE  bAttribute;
     BOOL  fOnce = TRUE;
     static INT HorizWidth = 0;
     BYTE  bBackGround = WM_BACKGROUND;
     USHORT usOldRow,usOldCol;
     CHAR  achCell[ MAX_LEN_WORM ];
     USHORT usCount;


     if(*pusCurrentRow < FIRST_ROW || *pusCurrentRow > LAST_ROW )
     {
          *pusCurrentRow = (FIRST_ROW + 1);
     }
     if(HorizWidth == (sizeof(asHorizWidths) / sizeof(SHORT ) ) )
     {
          HorizWidth = 0;
     }

     usSize = sizeof(achCell);
     bAttribute = *pbAttribute;

     usOldRow = *pusCurrentRow;
     usOldCol = *pusCurrentCol;
     usCount = strlen(pszRightWorm);
     VioReadCellStr(achCell,&usCount,usOldRow,usOldCol,hvio);
     for(sCol = (*pusCurrentCol + asHorizWidths[HorizWidth++]);
                      (*pusCurrentCol) <= sCol && fDraw;
                           (*pusCurrentCol)++ )
     {
          /*
           *read the current attribute
           *on the screen
           *if another worm
           */
          VioReadCellStr(achCell,&usSize,*pusCurrentRow,*pusCurrentCol,hvio);
          if(!fSilent)
          {
               if(achCell[0] != ' ' && fOnce )
               {
                    DosBeep(600,175);
                    DosBeep(1200,175);
                    bAttribute = 0x4F;
                    fOnce = FALSE;
               }
          }

          VioWrtCharStrAtt( pszRightWorm,strlen(pszRightWorm),
                            *pusCurrentRow,*pusCurrentCol,&bAttribute,
                            hvio );
          /*
           *erase the last one
           *by setting the background
           *color
           */
          DosSleep(PAUSE_TIME);
          VioWrtCharStrAtt( pszBlank,strlen(pszRightWorm),
                       *pusCurrentRow,(*pusCurrentCol),&bBackGround,
                       hvio );


    }

    /*
     *if we stopped drawing
     *come back to the same spot
     */
    if(fDraw)
    {
         VioWrtCellStr(achCell,usCount,usOldRow,usOldCol,hvio);
         *psCurrentDirection = (rand() %  MAX_DIRECTIONS);
         *pusCurrentCol = ( (rand() % HOR_WIDTH) + 1/  asWidths[rand() %  4] + 1);
         *pusCurrentRow = ( (rand() % HEIGHT) );
    }

}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:move_worm_zigup
 *
 *  Purpose:Draw the worm in a diagonal direction moving upwards.
 *
 *  Returns:
 *          VOID
\****************************************************************/
VOID move_worm_zigup(USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                      PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG tidWorm )
{

     USHORT sRow;
     SHORT  sPos;
     SHORT  sStartRow,sStartCol;
     BYTE  bBackGround = WM_BACKGROUND;

     sStartRow = *pusCurrentRow;
     sStartCol = *pusCurrentCol;
     for(sRow = (*pusCurrentRow - WIDTH),sPos = 0;
                            (*pusCurrentRow > FIRST_ROW + 1) && fDraw;
                            (*pusCurrentRow) -=2,(*pusCurrentCol)++ )
     {
          VioWrtCharStrAtt( &achUpWorm[sPos],1,
                            *pusCurrentRow,*pusCurrentCol,pbAttribute,
                            hvio );
          DosSleep(PAUSE_TIME);
          if(!achUpWorm[sPos++] )
          {
                for(; sStartRow >= (*pusCurrentRow )&& fDraw;sStartRow -=2 ,sStartCol++ )
                {
                    VioWrtCharStrAtt( &pszBlank[0],1,
                                      sStartRow,sStartCol,&bBackGround,
                                      hvio );
                }
                sPos = 0;
          }


     }
     /*
      *clean up any head on the screen
      */
    if(sPos)
    {
          for(; sStartRow >= (*pusCurrentRow );sStartRow -=2 )
          {
              VioWrtCharStrAtt( &pszBlank[0],1,
                                sStartRow,sStartCol++,&bBackGround,
                                hvio );
          }
    }

    /*
     *if we stopped drawing
     *come back to the same spot
     */
    if(fDraw)
    {
         *psCurrentDirection = ( rand() % MAX_DIRECTIONS);
         *pusCurrentCol = ( (rand() % HOR_WIDTH)/  asWidths[rand() %  4] + 1);
         *pusCurrentRow = ( (rand() % HEIGHT) );
    }

}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:move_worm_left()
 *
 *  Purpose:Move the worm to the left side of the screen.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID move_worm_left(USHORT *pusCurrentRow,USHORT *pusCurrentCol,
                      PBYTE pbAttribute,
                      SHORT *psCurrentDirection,
                      LONG  tidWorm )

{

     USHORT sCol;
     static INT HorizWidth = 0;
     BYTE  bBackGround = WM_BACKGROUND;


     if(HorizWidth == (sizeof(asHorizWidths) / sizeof(SHORT ) ) )
     {
          HorizWidth = 0;
     }

     if(*pusCurrentRow < FIRST_ROW || *pusCurrentRow > LAST_ROW )
     {
          *pusCurrentRow = (LAST_ROW - 1);
     }
    sCol = rand()  % WIDTH;
    if( sCol > *pusCurrentCol)
    {
          sCol = sCol - *pusCurrentCol;
    }
    for( ;((*pusCurrentCol) >  sCol) && fDraw; (*pusCurrentCol)--)
    {
         /*
          *read the current attribute
          *on the screen
          *if another worm
          */

         VioWrtCharStrAtt( pszLeftWorm,strlen(pszLeftWorm),
                           *pusCurrentRow,*pusCurrentCol,pbAttribute,
                           hvio );

         DosSleep(PAUSE_TIME);
         if(!fDraw)
         {
               break;
         }
         VioWrtCharStrAtt( pszBlank,strlen(pszLeftWorm),
                           *pusCurrentRow,(*pusCurrentCol),&bBackGround,
                            hvio );

    }
    /*
     *if we stopped drawing
     *come back to the same spot
     */
    if(fDraw)
    {
          *psCurrentDirection = (rand() %  MAX_DIRECTIONS);
          *pusCurrentRow = ( (rand() % HEIGHT) );
          *pusCurrentCol = ( (rand() % HOR_WIDTH)/  asWidths[rand() %  4]  + 1);
    }
}

/*--------------------------------------------------------------*\
 *  End of file : wrmthrd.c
\*--------------------------------------------------------------*/

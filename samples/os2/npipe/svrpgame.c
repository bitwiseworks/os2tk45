/*==============================================================*\
 *  Svrpgame.c - game processing routine: processing of client
 *               messages and game algorithm.
 *      Copyright 1992, IBM Corp.
 *
 *  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *  sample code created by IBM Corporation. This sample code is not
 *  part of any standard or IBM product and is provided to you solely
 *  for  the purpose of assisting you in the development of your
 *  applications.  The code is provided "AS IS", without
 *  warranty of any kind.  IBM shall not be liable for any damages
 *  arising out of your use of the sample code, even if they have been
 *  advised of the possibility of   such damages.                                                     *
 *
 *
\*==============================================================*/


#define INCL_DOSPROCESS
#define INCL_DOSNMPIPES
#define INCL_DOSMEMMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR

#include <os2.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "namepipe.h"
#include "svrpglbl.h"
#include "svrpmain.h"
#include "svrpxtrn.h"


VOID GameThread(VOID *hpTempPipe)
{
   HPIPE   hpGamePipe;
   BOOL    fQuit=FALSE;
   PPIB    dummy;
   PTIB    ptibThrdInfo;
   ULONG   ulID;
   USHORT  usMsg, usData, GameBoard[DIVISIONS][DIVISIONS], usTotalMoves=0;

   /* save pipe handle in own memory */
   hpGamePipe = *(HPIPE *)hpTempPipe;

   /* get thread ID for use in displaying messages */
   if((BOOL)DosGetInfoBlocks(&ptibThrdInfo, &dummy))
   {
      ulID = 0;
   }

   else
   {
      ulID = ptibThrdInfo->tib_ptib2->tib2_ultid;
   }

   InitBoard(GameBoard);

   /* initialize random number generator */
   srand((unsigned) ulID);

   if(!(BOOL)DosConnectNPipe(hpGamePipe))
   {
      while (!fAppExit &&
             !fQuit &&
             !(BOOL)(Pipe_IO(CLIENT, hpGamePipe, &usMsg, &usData, ulID)) &&
             !fAppExit)
      {
         switch (usMsg)
         {
            case CLIENT_MOVE:
               /* enter move from message */
               *((USHORT *)GameBoard + usData) = CLIENT_NUM;
               usTotalMoves++;

               /* test for win if total moves >= DIVISIONS*2-1 */
               if (usTotalMoves >= DIVISIONS*2-1 &&
                   ((BOOL)(usData=WinTest(GameBoard)) ||
                    usTotalMoves == DIVISIONS*DIVISIONS))
               {
                  /* notify of win or draw, and break (switch) on return */
                  if (!usData)
                  {
                     usData = DIVISIONS*DIVISIONS;
                     usMsg = WIN_DRAW;
                  }

                  else
                  {
                     usMsg = WIN_CLIENT;
                  }

                  if ((BOOL)Pipe_IO(SERVER, hpGamePipe, &usMsg, &usData, ulID))
                  {
                     WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_PIPE_WRITE_FAILED), MPVOID);
                     fQuit = TRUE;
                     break;
                  }

                  /* call InitBoard on win */
                  InitBoard(GameBoard);
                  usTotalMoves = 0;

                  break;  /* switch */
               }

               /* NO BREAK */

            case YOU_FIRST:
               /* get move if there are moves left */
               usData = GetMove(GameBoard, usTotalMoves);
               usTotalMoves++;

               /* test for win if total moves >= DIVISIONS*2-1 */
               if (usTotalMoves >= DIVISIONS*2-1 &&
                   ((BOOL)(usMsg=WinTest(GameBoard)) ||
                    usTotalMoves == DIVISIONS*DIVISIONS))
               {
                  /* write game_won message with winning move */
                  if (!usMsg)
                  {
                     usMsg = WIN_DRAW;
                  }

                  else
                  {
                     usMsg = WIN_SERVER;
                  }

                  if ((BOOL)Pipe_IO(SERVER, hpGamePipe, &usMsg, &usData, ulID))
                  {
                     WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_PIPE_WRITE_FAILED), MPVOID);
                     fQuit = TRUE;
                     break;
                  }

                  /* call InitBoard on win */
                  InitBoard(GameBoard);
                  usTotalMoves = 0;
               }

               /* else */
               else
               {
                  /* write move to client */
                  usMsg = SERVER_MOVE;
                  if ((BOOL)Pipe_IO(SERVER, hpGamePipe, &usMsg, &usData, ulID))
                  {
                     WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_PIPE_WRITE_FAILED), MPVOID);
                     fQuit = TRUE;
                  }
               }
               break;

            case ERROR_MSG:
               /* post the error to message queue */
               WinPostMsg(hwndMain, WM_MSG, MPFROMSHORT(usData), MPVOID);

            case CLIENT_QUIT:
               /* quit while */
               fQuit = TRUE;
         }
      }

      DosDisConnectNPipe(hpGamePipe);
   }

   DosClose(hpGamePipe);
}


/*
 *  Check for pieces owning entire row, column, or diagonal.
 */
USHORT WinTest(USHORT GameBoard[DIVISIONS][DIVISIONS])
{
   USHORT  usWinner=0, usTotal1=0, usTotal2=0, usTotal3=0, usTotal4=0;
   int     i, j;

   for (i = 0; i < DIVISIONS && !(BOOL)usWinner; i++)
   {
      for (j = 0; j < DIVISIONS && !(BOOL)usWinner; j++)
      {
         usTotal1 += GameBoard[i][j];
         usTotal2 += GameBoard[j][i];
      }

      if (SERVER_NUM * DIVISIONS == usTotal1 || CLIENT_NUM * DIVISIONS == usTotal1)
      {
         usWinner = usTotal1/(USHORT)DIVISIONS;
      }

      else if (SERVER_NUM * DIVISIONS == usTotal2 || CLIENT_NUM * DIVISIONS == usTotal2)
      {
         usWinner = usTotal2/(USHORT)DIVISIONS;
      }

      else
      {
         usTotal1 = usTotal2 = 0;
         usTotal3 += GameBoard[i][i];
         usTotal4 += GameBoard[i][DIVISIONS-i-1];
      }
   }

   if (SERVER_NUM * DIVISIONS == usTotal3 || CLIENT_NUM * DIVISIONS == usTotal3)
   {
      usWinner = usTotal3/(USHORT)DIVISIONS;
   }

   else if (SERVER_NUM * DIVISIONS == usTotal4 || CLIENT_NUM * DIVISIONS == usTotal4)
   {
      usWinner = usTotal4/(USHORT)DIVISIONS;
   }

   return(usWinner);
}


USHORT GetMove(USHORT GameBoard[DIVISIONS][DIVISIONS], USHORT usTotalMoves)
{
   USHORT  usMovesLeft;
   int     i, j;

   usMovesLeft = (USHORT)((USHORT)DIVISIONS * (USHORT)DIVISIONS - usTotalMoves);

   /* a poor man's AI */
   usMovesLeft = (USHORT)((usMovesLeft * rand()) / (RAND_MAX + 1));

   for (i = 0; i < DIVISIONS; i++)
   {
      for (j = 0; j < DIVISIONS; j++)
      {
         if (0 == GameBoard[i][j] && 0 == usMovesLeft--)
         {
            GameBoard[i][j] = SERVER_NUM;
            return((USHORT)(i * DIVISIONS + j));
         }
      }
   }
}


/*
 *  Reset game board.
 */
VOID InitBoard(USHORT GameBoard[DIVISIONS][DIVISIONS])
{
   int i, j;

   for (i = 0; i < DIVISIONS; i++)
      for (j = 0; j < DIVISIONS; j++)
         GameBoard[i][j] = 0;
}

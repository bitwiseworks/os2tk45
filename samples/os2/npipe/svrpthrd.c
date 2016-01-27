/*==============================================================*\
 *  Svrpthrd.c - pipe processing routine: initialization and
 *               and message display threads.
 *
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
#define INCL_DOSSEMAPHORES
#define INCL_DOSMEMMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR

#include <os2.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "svrpglbl.h"
#include "svrpmain.h"
#include "svrpxtrn.h"
#include "namepipe.h"


#define STACKSIZE    8192


HEV    hevMsgRcvd;   /* event semaphore for displaying pipe messages */
HPIPE  hpStartPipe;  /* handle to pipe through which start requests are made */


/* setup and start threads to manage pipes */
HPIPE InitThreads()
{
   BOOL    fSuccess=FALSE;
   ULONG   ulErrorMsg=0;

   if ((BOOL)DosCreateEventSem(NULL, &hevMsgRcvd, 0L, FALSE))
   {
      ulErrorMsg = IDMSG_CREATE_SEM_FAILED;
   }

   else if((BOOL)DosCreateNPipe(START_PIPE_NAME,
                            &hpStartPipe,
                            FS_SVR_STARTPIPE_OPEN_MODE,
                            FS_SVR_STARTPIPE_PIPE_MODE,
                            sizeof(PIPE_MSG)*3,
                            sizeof(PIPE_MSG)*3,
                            PIPE_TIMEOUT))
   {
      ulErrorMsg = IDMSG_CREATE_PIPE_FAILED;
   }

   else if (0 > _beginthread(DisplayPipeMsg,
                             NULL,
                             STACKSIZE,
                             NULL))
   {
      ulErrorMsg = IDMSG_CREATE_THREAD_FAILED;
   }

   else if (0 > _beginthread(ConnectPlayer,
                             NULL,
                             STACKSIZE,
                             NULL))
   {
      ulErrorMsg = IDMSG_CREATE_THREAD_FAILED;
   }

   else
   {
      fSuccess = TRUE;
   }

   if (!fSuccess)
   {
      MessageBox(hwndMain, ulErrorMsg, MB_OK | MB_ERROR, TRUE);
      hpStartPipe = (HPIPE)0;                                   /* TEST CODE */
   }

   return (hpStartPipe);                                        /* TEST CODE */
}


VOID ConnectPlayer(VOID *dummy)
{
   USHORT  usMsgType, usErrorMsg=0;
   HPIPE   hpGamePipe[2];  /* buffer pipe handles */
   BOOL    fExit=FALSE;
   int     iPipeIndex=0;
   APIRET  apirc;

   while (!fAppExit &&  /* test for end before */
          !fExit &&
          !(BOOL)(apirc=DosConnectNPipe(hpStartPipe)) &&  /* wait to connect next player */
          !fAppExit)    /* and after waiting */
   {
      usMsgType = ERROR_MSG;

      if ((BOOL)DosCreateNPipe(GAME_PIPE_NAME,  /* create pipe for next player */
                       &hpGamePipe[iPipeIndex],
                       FS_SVR_GAMEPIPE_OPEN_MODE,
                       FS_SVR_GAMEPIPE_PIPE_MODE,
                       sizeof(PIPE_MSG)*3,
                       sizeof(PIPE_MSG)*3,
                       PIPE_TIMEOUT))
      {
         usErrorMsg = IDMSG_CREATE_PIPE_FAILED;
      }

      else if (0 > _beginthread(GameThread,   /* begin game thread */
                                NULL,
                                STACKSIZE,
                                (VOID *)&hpGamePipe[iPipeIndex]))
      {
         usErrorMsg = IDMSG_CREATE_THREAD_FAILED;
         DosClose(hpGamePipe[iPipeIndex]);  /* close the created pipe */
      }

      else
      {
         usMsgType = START_MSG;
      }

      /* write start message to client, if we have an error, notify him */
      /* and close pipe, 'cause we could care less about him now */
      if ((BOOL)Pipe_IO(SERVER, hpStartPipe, &usMsgType, &usErrorMsg, 0) &&
          usMsgType != ERROR_MSG)
      {
         usErrorMsg = IDMSG_PIPE_WRITE_FAILED;
      }

      /* read confirmation from client. if he has an error, this is how he notifies us */
      else if ((BOOL)Pipe_IO(CLIENT, hpStartPipe, &usMsgType, &usErrorMsg, 0))
      {
         usErrorMsg = IDMSG_PIPE_READ_FAILED;
      }

      else if (ERROR_MSG == usMsgType)  /* the client cannot use the game pipe */
      {
         /* notify the thread here.  We can write to its pipe like
            we were the client and tell it of a fatal error condition. */
         if (Pipe_IO(SERVER, hpGamePipe[iPipeIndex], &usMsgType, &usErrorMsg, 0))
         {
            /* if we cannot talk to the thread, we are in deep doo-doo, as the
               next client might try to grab the same busted pipe.  Disconnect
               it, hope for the best, and let the game thread do what it will */
            DosDisConnectNPipe(hpGamePipe[iPipeIndex]);
            DosClose(hpGamePipe[iPipeIndex]);
         }
      }

      else
      {
         iPipeIndex = 1 - iPipeIndex;  /* flip index value */
      }

      if ((BOOL)DosDisConnectNPipe(hpStartPipe))  /* disconnect start pipe */
      {
         usErrorMsg = IDMSG_START_PIPE_ERROR;
         fExit = TRUE;
      }

      if ((BOOL)usErrorMsg)
      {
         WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(usErrorMsg), MPVOID);
         usErrorMsg = 0;
      }
   }

   /* if the connect bombed, and not because we closed the pipe on it... */
   if (!fAppExit)
   {
      WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_CONNECT_FAILED), MPVOID);
      WinPostMsg(hwndMain, WM_COMMAND, MPFROMLONG(IDM_FILEEXIT), MPVOID);
      fAppExit = TRUE;
   }

   DosPostEventSem(hevMsgRcvd);  /* notify DisplayPipeMsg */
   DosClose(hpGamePipe[0]);      /* close what you can in case */
   DosClose(hpGamePipe[1]);      /*   the game thread can't.   */
   fThrdsDead = TRUE;            /* outta here... */
}


#define  BUFFSIZE  10

/* buffer for pipe messages to be formatted and displayed */
struct
   {
      BOOL      fServer;
      ULONG     ulID;
      PIPE_MSG  pmsgMsg;
      CHAR      pszMsgString[MESSAGE_LEN];
   }
   stMsgBuff[BUFFSIZE];


/* Write or read to given pipe and write message to display buffer.
 */
APIRET Pipe_IO(BOOL fServer, HPIPE hpPipe, USHORT *usType, USHORT *usData, ULONG ulID)
{
   static int iMsgIndex=0;
   PIPE_MSG   pmsgPipeMsg;
   APIRET     apiRC;
   ULONG      ulCount;

   if (fServer)  /* outgoing, from the server */
   {
      pmsgPipeMsg.usMsgType = *usType;
      pmsgPipeMsg.usMsgData = *usData;
      apiRC = DosWrite(hpPipe, (VOID *)&pmsgPipeMsg, sizeof(PIPE_MSG), &ulCount);

      DosEnterCritSec();   /* the update to the message buffer should be atomic */

      stMsgBuff[iMsgIndex].fServer = fServer;
      stMsgBuff[iMsgIndex].pmsgMsg.usMsgType = *usType;
      stMsgBuff[iMsgIndex].pmsgMsg.usMsgData = *usData;
      stMsgBuff[iMsgIndex].ulID = ulID;
      iMsgIndex = (iMsgIndex < BUFFSIZE-1) ? iMsgIndex+1 : 0;

      DosExitCritSec();

      DosPostEventSem(hevMsgRcvd);  /* tell displaypipemsg() to display */
   }

   else if(!(apiRC = DosRead(hpPipe, (VOID *)&pmsgPipeMsg, sizeof(PIPE_MSG), &ulCount))
           && ulCount > 0)
   {
      *usType = pmsgPipeMsg.usMsgType;
      *usData = pmsgPipeMsg.usMsgData;

      DosEnterCritSec();

      stMsgBuff[iMsgIndex].fServer = fServer;
      stMsgBuff[iMsgIndex].pmsgMsg.usMsgType = *usType;
      stMsgBuff[iMsgIndex].pmsgMsg.usMsgData = *usData;
      stMsgBuff[iMsgIndex].ulID = ulID;
      iMsgIndex = (iMsgIndex < BUFFSIZE-1) ? iMsgIndex+1 : 0;

      DosExitCritSec();

      DosPostEventSem(hevMsgRcvd);
   }

   /* DosRead returns an error code of 0 and 0 bytes read with a broken pipe */

   else if (0 == ulCount)
   {
      apiRC = ERROR_BROKEN_PIPE;
   }

   return(apiRC);
}


VOID DisplayPipeMsg(VOID *dummy)
{
   ULONG       ulPosts;
   static int  iMsgIndex=0;
   CHAR        *pszSender, *pszType;

   while (!DosWaitEventSem(hevMsgRcvd, SEM_INDEFINITE_WAIT) &&
          !fAppExit)
   {
      DosResetEventSem(hevMsgRcvd, &ulPosts);
      while (ulPosts-- > 0)              /* format all of the messages posted */
      {
         /* format pipe message here */
         if (stMsgBuff[iMsgIndex].fServer)
         {
            pszSender = "SERVER";
         }

         else
         {
            pszSender = "CLIENT";
         }

         switch (stMsgBuff[iMsgIndex].pmsgMsg.usMsgType)
         {
            case START_MSG:
               pszType = START_MSG_TEXT;
               break;

            case CLIENT_MOVE:
               pszType = CLIENT_MOVE_TEXT;
               break;

            case YOU_FIRST:
               pszType = YOU_FIRST_TEXT;
               break;

            case ERROR_MSG:
               pszType = ERROR_MSG_TEXT;
               break;

            case CLIENT_QUIT:
               pszType = CLIENT_QUIT_TEXT;
               break;

            case WIN_CLIENT:
               pszType = WIN_CLIENT_TEXT;
               break;

            case WIN_SERVER:
               pszType = WIN_SERVER_TEXT;
               break;

            case SERVER_MOVE:
               pszType = SERVER_MOVE_TEXT;
               break;

            case WIN_DRAW:
               pszType = WIN_DRAW_TEXT;
               break;

            default:
               pszType = "UNKNOWN";
         }

         /* format the text of the message */
         sprintf(stMsgBuff[iMsgIndex].pszMsgString, "Thread %03u, %s Message: %s: %u", stMsgBuff[iMsgIndex].ulID, pszSender, pszType, stMsgBuff[iMsgIndex].pmsgMsg.usMsgData);

         /* post pointer to said message to the PM thread */
         WinPostMsg(hwndMain, WM_PIPEMSG, MPFROMP(stMsgBuff[iMsgIndex].pszMsgString), MPVOID);

         /* increment circular list index */
         iMsgIndex = (iMsgIndex < BUFFSIZE-1) ? iMsgIndex+1 : 0;
      }
   }

   DosCloseEventSem(hevMsgRcvd);
}

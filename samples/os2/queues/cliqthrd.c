/*==============================================================*\
 *
 *  cliqthrd.c - thread source
 *      Copyright 1992, IBM Corp.
 *
 *--------------------------------------------------------------
 *
 *  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *  sample code created by IBM Corporation. This sample code is not
 *  part of any standard or IBM product and is provided to you solely
 *  for  the purpose of assisting you in the development of your
 *  applications.  The code is provided "AS IS", without
 *  warranty of any kind.  IBM shall not be liable for any damages
 *  arising out of your use of the sample code, even if they have been
 *  advised of the possibility of   such damages.
 *
 *--------------------------------------------------------------
 *
 *
 *  This file contains the source for the thread initialization
 *  and the source for the thread that services the queue.
 *
\*==============================================================*/


#define INCL_DOSPROCESS
#define INCL_DOSQUEUES
#define INCL_DOSMEMMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSERRORS
#define INCL_DOSSEMAPHORES

#include <os2.h>


#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "cliqmain.h"
#include "queue.h"
#include "cliqglbl.h"
#include "cliqxtrn.h"


#define  STACKSIZE      8192
#define  Q_BUFFER_SIZE  4000


PID          pidServer;
HQUEUE       hqQueue;
PVOID        *pvdQMemory;


BOOL InitQThrd(HEV hevSend, PVOID *ppvdMessage, PUSHORT pusPriority, PULONG pulMsgSize, PULONG pulMsgType)
{
   APIRET              apiRC;
   BOOL                fSuccess = TRUE;
   static THREADPARAM  tpThrdParm;

   if (ERROR_QUE_NAME_NOT_EXIST == (apiRC=DosOpenQueue(&pidServer,
                                                       &hqQueue,
                                                       "\\queues\\" Q_NAME)))
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_SVR_DOWN,
                 apiRC,
                 MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                 TRUE);
      fSuccess = FALSE;
   }

   else if (0 != apiRC)
   {
      MessageBox(hwndMain,
                 IDMSG_Q_ERR,
                 apiRC,
                 MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                 TRUE);
      fSuccess = FALSE;
   }

   else
   {
      if (apiRC=DosAllocSharedMem((VOID **)&pvdQMemory,
                            NULL,
                            (ULONG)Q_BUFFER_SIZE,
                            fALLOCSHR))
   /* fALLOC is equivalent to PAG_READ | PAG_WRITE | PAG_COMMIT | PAG_EXECUTE | OBJ_TILE
    * OBJ_TILE is necessary when allocating named shared memory
    */
      {
        MessageBox(hwndMain,
                   IDMSG_MEM_ERR,
                   apiRC,
                   MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                   TRUE);
        fSuccess = FALSE;
      }

      else if (apiRC=DosSubSetMem(pvdQMemory, DOSSUB_INIT, (ULONG)Q_BUFFER_SIZE))
      {
        MessageBox(hwndMain,
                   IDMSG_SUBALLOC_MEM_ERR,
                   apiRC,
                   MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                   TRUE);
        fSuccess = FALSE;
      }

      /* give memory to server */
      else if (apiRC=DosGiveSharedMem(pvdQMemory, pidServer, PAG_READ | PAG_WRITE))
      {
        MessageBox(hwndMain,
                   IDMSG_GIVE_MEM_ERR,
                   apiRC,
                   MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                   TRUE);
        fSuccess = FALSE;
      }

      else
      {
         tpThrdParm.hevSend = hevSend;          /* posted when user elects to send message */
         tpThrdParm.ppvdMessage = ppvdMessage;  /* message to be sent */
         tpThrdParm.pusPriority = pusPriority;  /* priority of said message */
         tpThrdParm.pulMsgSize = pulMsgSize;    /* size of message */
         tpThrdParm.pulMsgType = pulMsgType;    /* size of message */


         if (0 > _beginthread(PostQMessage,
                              NULL,
                              STACKSIZE,
                              (VOID *)&tpThrdParm))
         {
            fSuccess = FALSE;
         }
      }

      if (!fSuccess)
      {
         DosCloseQueue(hqQueue);
      }
   }

   return (fSuccess);
}


VOID PostQMessage(VOID *tpThrdParm)
{
   VOID    *pqmsg;
   APIRET  apiRC;

   while (!DosWaitEventSem(((THREADPARAM *)tpThrdParm)->hevSend, SEM_INDEFINITE_WAIT))
   {
      if (15 < *(((THREADPARAM *)tpThrdParm)->pusPriority))  /* exit notification */
      {
         break;
      }

      if (TRUE == fSetUpMessage(*(THREADPARAM *)tpThrdParm, &pqmsg))
      {
         if (0 != (apiRC=DosWriteQueue(hqQueue,
                                *(((THREADPARAM *)tpThrdParm)->pulMsgType),
                                *(((THREADPARAM *)tpThrdParm)->pulMsgSize),
                                (VOID *)pqmsg,
                                *(((THREADPARAM *)tpThrdParm)->pusPriority))))
         {
            WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_SVR_DOWN), MPFROMLONG(apiRC));
         }
      }
   }

   DosCloseQueue(hqQueue);
   DosFreeMem(pvdQMemory);
   fThrdsDead = TRUE;
}


BOOL fSetUpMessage(THREADPARAM tpThrdParm, PVOID *pqmsg)
{
   BOOL    fSuccess = FALSE;
   APIRET  apiRC;
   ULONG   ulPosts;

   if (apiRC=DosSubAllocMem(pvdQMemory, pqmsg, *tpThrdParm.pulMsgSize))
   {
      WinPostMsg(hwndMain, WM_MSG, MPFROMLONG(IDMSG_MEM_FULL), MPFROMLONG(apiRC));
   }

   else
   {
      if (memcpy(*pqmsg, *tpThrdParm.ppvdMessage, *tpThrdParm.pulMsgSize))
      {
         fSuccess = TRUE;
      }
   }

   DosResetEventSem(tpThrdParm.hevSend, &ulPosts);
   return (fSuccess);
}

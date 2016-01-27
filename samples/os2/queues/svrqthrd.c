/*==============================================================*\
 *  Svrqthrd.c - queue processing routine: initialization and
 *               processing thread.
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
\*==============================================================*/


#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSQUEUES
#define INCL_DOSMEMMGR
#define INCL_WINMESSAGEMGR
#define INCL_DOSERRORS

#include <os2.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "svrqglbl.h"
#include "svrqmain.h"
#include "queue.h"
#include "svrqxtrn.h"


#define STACKSIZE    8192

static HQUEUE hqQ;

HQUEUE InitQ(PID  pidParent,   /* proccess that will send WM_CLOSE command */
             HWND hwndParent)  /* window to post to */
{
   ULONG              ulErrorMsg;
   BOOL               fError=FALSE;
   static THREADPARAM tpThrdParm;

   if (!DosCreateQueue(&hqQ,
                       QUE_PRIORITY,
                       "\\queues\\" Q_NAME))
   {
      tpThrdParm.pidParent = pidParent;
      tpThrdParm.hwndParent = hwndParent;

      if( _beginthread(ReadQMessage,
                           NULL,
                           STACKSIZE,
                           (VOID *)&tpThrdParm) == -1 )
      {
         ulErrorMsg = IDMSG_CREATE_THREAD_FAILED;
         fError = TRUE;
      }
   }

   else
   {
      ulErrorMsg = IDMSG_CREATE_Q_FAILED;
      fError = TRUE;
   }

   if (fError)
   {
      MessageBox(HWND_DESKTOP,
                 ulErrorMsg,
                 MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL,
                 TRUE);
      return(0);
   }

   else
   {
      return (hqQ);
   }
}


#define  MAXBUFF  10

VOID ReadQMessage(VOID *tpThrdParm)
{
   PVOID        pvdQMsg;
   BYTE         bPriority;
   REQUESTDATA  reqQdata;
   ULONG        cbRead;
   BOOL         fClose=FALSE, fError=FALSE;
   int          index=0;
   QDATA        qdataBuffer[MAXBUFF];
   PID          pidParent;
   HWND         hwndParent;

   pidParent = ((THREADPARAM *)tpThrdParm)->pidParent;
   hwndParent = ((THREADPARAM *)tpThrdParm)->hwndParent;

   while (!fClose && !fError)
   {
      if (!(DosReadQueue(hqQ,             /* queue handle                               */
                         &reqQdata,       /* queue result, incl. process id and request */
                         &cbRead,         /* count of bytes in element                  */
                (VOID **)&pvdQMsg,        /* address of element                         */
                         0,               /* element number to read                     */
                         DCWW_WAIT,       /* wait for element                           */
                         &bPriority,      /* received element priority                  */
                         (HEV) NULL))) {  /* semaphore handle (not used)                */

         /* if parent process sends message to die... */

         if (pidParent == reqQdata.pid && WM_CLOSE == reqQdata.ulData)
         {
            fClose = TRUE;
         }

         /* copy the string to some internal buffer, and post a message to the window
          * to copy the new text to its own buffer and do what it will.
          */
         else
         {
            /* format the message */
            switch(reqQdata.ulData)
            {
               case Q_MSG_DATE:
                  sprintf(qdataBuffer[index].pszMsg,
                          "Date -- Day: %d Month: %d Year: %d",
                          ((Q_DATE *)pvdQMsg)->usDay,
                          ((Q_DATE *)pvdQMsg)->usMonth,
                          ((Q_DATE *)pvdQMsg)->usYear);
                  break;

               case Q_MSG_TEXT:
               default:
                  strncpy(qdataBuffer[index].pszMsg, (CHAR *)pvdQMsg, cbRead);
                  break;
            }

            qdataBuffer[index].pidProcess = reqQdata.pid;
            qdataBuffer[index].usPriority = bPriority;
            WinPostMsg(hwndParent, WM_NEWQMSG, MPFROMP(&(qdataBuffer[index])), MPVOID);
            index = (index < MAXBUFF-1) ? index+1 : 0;

         /* after posting the message, unmark the memory and start the loop again
          *
          * CAVEAT: We are getting the offset by zeroing out the lower 16 bits
          * of the block offset.  This can ONLY be done if the memory allocated
          * is less than 64K.  Otherwise, the client must somehow (in the queue
          * message would probably be a good idea) pass the offset to the server.
          *
          */
            DosSubFreeMem((VOID *)((ULONG)pvdQMsg & ~(ULONG)0xFFFF), pvdQMsg, cbRead);
/*            DosSubFreeMem(MAKEP(SELECTOROF(pvdQMsg), 0), pvdQMsg, cbRead);*/
         }
      }

      else
      {
         WinPostMsg(hwndMain, WM_MSG, MPFROMSHORT(IDMSG_READ_Q_ERROR), MPVOID);
         fError = TRUE;
      }
   }

   DosCloseQueue(hqQ);
   fThrdsDead=TRUE;
}

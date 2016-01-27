/**************************************************************************
 *  File name  :  thrd.c
 *
 *  Description:  This module contains the code for a background processing
 *                thread as well as the code for any routines used in that
 *                thread.
 *
 *                This thread uses an object window to facilitate
 *                communication with the main thread.  The main thread can
 *                communicate with the background thread through messages.
 *                You define your own window messages in main.h, post or send
 *                them to the background thread through the routines provided
 *                in this file, and then process the messages in the
 *                ObjectWndProc() routine.  The object window then sends
 *                other messages back to the main window to notify the main
 *                window as to the status of the task or with the final
 *                result.
 *
 *                An object window is used as the main window of the thread
 *                for several reasons.  The window is hidden, so it is
 *                never displayed on the screen.  Since it is hidden, the
 *                window does not have to process most of the window
 *                messages that are processed by visible windows, e.g.
 *                WM_PAINT, WM_SIZE, WM_SETFOCUS, etc.  This window will
 *                never receive the focus so it need not process input
 *                messages.  You can, however, send all mouse and keyboard
 *                messages to this window and have it process all input if
 *                you wish.  Since the window does not belong to any focus
 *                or activation chains, you can block this thread on a
 *                semaphore without having the system lock up or use the
 *                window manager semaphore routines such as
 *                WinMsgSemWait().
 *
 *                The BackgroundThread() routine is designed to create only
 *                one thread.  Should you try to create another one, the
 *                call will fail.  The thread uses several variables
 *                which are defined globally within this module.  In order
 *                to enable this routine to create multiple threads, you
 *                will need to make these variables local.  You will also
 *                need to differentiate between the different object
 *                windows created so that you can specify which thread will
 *                receive a posted message.
 *
 *                In order to enable the use of the second thread, define
 *                the BACKGROUND_THREAD constant found in main.h.
 *
 *                This source file contains the following functions:
 *
 *                BackgroundThread()
 *                CreateBackgroundThread()
 *                DestroyBackgroundThread()
 *                PostBkThreadMsg(msg, mp1, mp2)
 *                SendBkThreadMsg(msg, mp1, mp2)
 *                ObjectWndProc(hwnd, msg, mp1, mp2)
 *
 *  Concepts   :  background thread
 *
 *  API's      :  WinInitialize
 *                WinPostMsg
 *                DosExit
 *                WinCreateMsgQueue
 *                WinTerminate
 *                WinLoadString
 *                WinRegisterClass
 *                WinCreateWindow
 *                DosPostEventSem
 *                WinGetMsg
 *                WinDispatchMsg
 *                WinIsWindow
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *                DosCreateEventSem
 *                DosCreateThread
 *                WinWaitEventSem
 *                DosWaitThread
 *                WinDefWindowProc
 *
 *  Required
 *    Files    :  OS2.H, MAIN.H, XTRN.H
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/
/*
 *  Include files, macros, defined constants, and externs
 */

#define  INCL_DOSPROCESS
#define  INCL_DOSSEMAPHORES
#define  INCL_WINMESSAGEMGR
#include <os2.h>
#include "main.h"
#include "xtrn.h"

#define THREADSTACKSIZE     8192L
#define SEM_TIMEOUT        10000L

/*
 *  Global variables
 */
static HAB habBkThread;
static HMQ hmqBkThread;
static TID tidBkThread;
static HWND hwndObject;
static CHAR szObjectClass[MAXNAMEL];
static BOOL fThreadCreated = FALSE;
static HEV hevThreadInit;   /* semaphore for thread initialization */

VOID BackgroundThread (ULONG);

MRESULT EXPENTRY ObjectWndProc(HWND, ULONG, MPARAM, MPARAM);

/**************************************************************************
 *
 *  Name       : BackgroundThread(ulThreadParam)
 *
 *  Description: Routine is a background thread used for tasks to be
 *               completed in the background.
 *
 *  Concepts:    Routine is called at initialization time to create
 *               a background processing thread.
 *
 *               The routine initializes itself as a PM thread and creates
 *               a message queue.  It then creates an object window
 *               through which it will receive and send messages.
 *               It then polls through a message loop, processing any
 *               messages it receives in the object window's window
 *               procedure.  When the loop ends, it terminates.
 *
 *  API's      :  WinInitialize
 *                WinPostMsg
 *                DosExit
 *                WinCreateMsgQueue
 *                WinLoadString
 *                WinRegisterClass
 *                WinCreateWindow
 *                DosPostEventSem
 *                WinGetMsg
 *                WinDispatchMsg
 *                WinIsWindow
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *
 *  Parameters :  ulThreadParam = thread parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID BackgroundThread(ULONG ulThreadParam)
{
   QMSG qmsg;

   /* create message queue for thread */
   habBkThread = WinInitialize(0);
   if(habBkThread == NULLHANDLE)
   {
      WinPostMsg(hwndMain, TM_THREADINITFAILED, (MPARAM)NULL, (MPARAM)NULL);
      DosExit(EXIT_THREAD, 0L);
   }

   hmqBkThread = WinCreateMsgQueue(habBkThread, 0);
   if(hmqBkThread == NULLHANDLE)
   {
      WinPostMsg(hwndMain, TM_THREADINITFAILED, (MPARAM)NULL, (MPARAM)NULL);

      WinTerminate(habBkThread);
      DosExit(EXIT_THREAD, 0L);
   }

   /* load the string for the object window class and register the class */
   if(WinLoadString(habBkThread,
                    (HMODULE)NULLHANDLE,
                    IDS_OBJECTCLASS,
                    MAXNAMEL,
                    (PSZ)szObjectClass)
      &&
              /* register the main client window class */
      WinRegisterClass(habBkThread,
                       (PSZ)szObjectClass,
                       (PFNWP)ObjectWndProc,
                       0L,
                       0L)
      &&
            /* create the object window */
      (NULLHANDLE != (hwndObject = WinCreateWindow(HWND_OBJECT,
                                    (PSZ)szObjectClass,
                                    (PSZ)NULL,
                                    0L,
                                    0L,
                                    0L,
                                    0L,
                                    0L,
                                    (HWND)NULLHANDLE,
                                    HWND_TOP,
                                    OBJECTID,
                                    (PVOID)NULL,
                                    (PVOID)NULL))))
   {
      /* set thread created flag so another thread of this type
       * cannot be created.
       */
      fThreadCreated = TRUE;

      /* clear initialization semaphore */
      DosPostEventSem(hevThreadInit);

      /* message loop */
      while(WinGetMsg(habBkThread, &qmsg, (HWND)NULLHANDLE, 0L, 0L))
         WinDispatchMsg(habBkThread, &qmsg);
   }
   else
   {
      WinPostMsg(hwndMain, TM_THREADINITFAILED, NULL, NULL);
   }

   /* destroy object window, clean up message queue and terminate */
   if(WinIsWindow(habBkThread, hwndObject))
      WinDestroyWindow(hwndObject);

   WinDestroyMsgQueue(hmqBkThread);
   WinTerminate(habBkThread);

   /* If termination is due to an error initializing the thread, then
       clear the initialization semaphore so that the main thread
       can continue. */
   if(!fThreadCreated)
      DosPostEventSem(hevThreadInit);

   DosExit(EXIT_THREAD, 0L);

   /* the thread parameter is not currently used */
   ulThreadParam;
}   /* End of BackgroundThread */


/**************************************************************************
 *
 *  Name       : CreateBackgroundThread()
 *
 *  Description: Creates the background thread
 *
 *  Concepts:    Routine is called at initialization time to create
 *               a background processing thread.
 *
 *               The routine calls DosCreateThread with the Background
 *               thread routine.
 *
 *  API's      :  DosCreateEventSem
 *                DosCreateThread
 *                WinWaitEventSem
 *
 *  Parameters :  [none]
 *
 *  Return     :  TRUE = thread is created successfully
 *                FALSE = not, or if the thread was already created
 *
 *************************************************************************/
BOOL CreateBackgroundThread(VOID)
{
   ULONG sRet;

   if(!fThreadCreated)
   {
      DosCreateEventSem((PSZ)NULL, &hevThreadInit, 0L, FALSE);
      sRet = (ULONG)DosCreateThread(&tidBkThread,
                                    (PFNTHREAD)BackgroundThread,
                                    0L,
                                    0L,
                                    THREADSTACKSIZE);
   }
   else
      return FALSE;

   /* wait until the thread has finished initialization */
   if(WinWaitEventSem(hevThreadInit, SEM_TIMEOUT))
      return FALSE;

   return((BOOL)(sRet == 0L));
}   /* End of CreateBackgroundThread   */

/**************************************************************************
 *
 *  Name       : DestroyBackgroundThread()
 *
 *  Description: Destroys the background thread
 *
 *  Concepts:    Routine is called at exit time to destroy the
 *               background processing thread.
 *
 *               The routine posts a WM_CLOSE message to the object window
 *               to end its message loop.  It then waits to make sure that
 *               the thread has been terminated before it returns.
 *
 *  API's      :  DosWaitThread
 *
 *  Parameters :  [none]
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID DestroyBackgroundThread(VOID)
{
   PostBkThreadMsg(WM_CLOSE, (MPARAM)NULL, (MPARAM)NULL);
   DosWaitThread(&tidBkThread, 0L);
}   /* End of DestroyBackgroundThread   */


/**************************************************************************
 *
 *  Name       : PostBkThreadMsg(msg, mp1, mp2)
 *
 *  Description: Routine posts a message to the object window of the
 *               background thread.
 *
 *  Concepts:    Routine is called whenever a message is to be posted to
 *               the background processing thread.
 *
 *               The routine posts the message to the object window of the
 *               thread.
 *
 *  API's      :  WinPostMsg
 *
 *  Parameters :  msg = the message i.d.
 *                mp1 = first message parameter
 *                mp2 = second message parameter
 *
 *  Return     :  the return value from WinPostMsg().
 *
 *************************************************************************/
BOOL PostBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2)
{
   return(WinPostMsg(hwndObject, msg, mp1, mp2));
}   /* End of PostBkThreadMsg   */


/**************************************************************************
 *
 *  Name       : SendBkThreadMsg(msg, mp1, mp2)
 *
 *  Description: Routine sends a message to the object window of the
 *               background thread.
 *
 *  Concepts:    Routine is called whenever a message is to be sent to
 *               the background processing thread.
 *
 *               The routine sends the message to the object window of the
 *               thread.
 *
 *  API's      :  WinSendMsg
 *
 *  Parameters :  msg = the message i.d.
 *                mp1 = first message parameter
 *                mp2 = second message parameter
 *
 *  Return     :  the return value from WinSendMsg().
 *
 *************************************************************************/
MRESULT SendBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2)
{
   return(WinSendMsg(hwndObject, msg, mp1, mp2));
}   /* End of SendBkThreadMsg   */



/**************************************************************************
 *
 *  Name       : ObjectWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Processes all messages sent to the Object window
 *
 *  Concepts:    Called for each message sent to the Object window.
 *
 *               The Object window processes the messages that tell
 *               the background thread what action to take.  Since
 *               the object window is not visible, it will not
 *               process any of the standard window messages.
 *
 *  API's      :  WinDefWindowProc
 *
 *  Parameters :  hwnd= window handle
 *                msg = the message i.d.
 *                mp1 = first message parameter
 *                mp2 = second message parameter
 *
 *  Return     :  dependent on message sent
 *
 *************************************************************************/
MRESULT EXPENTRY ObjectWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
       /*
        *  Include any user-defined messages to determine which
        *  action the background thread should take.
        */

   default:
      return (WinDefWindowProc(hwnd, msg, mp1, mp2));
      break;
   }
   return (MRESULT)NULL;
}   /* End of ObjectWndProc   */
/***************************  End of thrd.c  ****************************/

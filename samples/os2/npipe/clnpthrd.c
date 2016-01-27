/*==============================================================*\
 *  clnpthrd.c - Pipe Server end of Tic-Tac-Toe sample app
 *      Copyright 1992 IBM Corp.
 *--------------------------------------------------------------
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
 *--------------------------------------------------------------
 *
 *  This c file contains the thread creation routines and
 *  manages the named pipe.
 *
\*==============================================================*/


/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_WINHELP
#define INCL_DOSPROCESS
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_DOSNMPIPES
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_DOSSEMAPHORES
#define INCL_WINDIALOGS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "namepipe.h"

static struct _THREADPARAM {
   CHAR  StartPipe[MAXPIPENAME];
   CHAR  GamePipe[MAXPIPENAME];
   BOOL  fMeFirst;
   };

typedef struct _THREADPARAM THREADPARAM;

#include "clnpmain.h"
#include "clnpxtrn.h"
#include "clnpglbl.h"

#define  STACKSIZE  8192

CHAR  pszServer[MAXSERVERNAME];

BOOL InitThread()
{
   static THREADPARAM  tpPipeNames;
   ULONG               usErrorMsg=0;

   /* load and open dialog box to get server name from user */
   if (IDC_OK == WinDlgBox(HWND_DESKTOP, hwndMainFrame, ServerDlgProc, 0, IDD_SERVER, NULL))
   {
      strcpy(tpPipeNames.StartPipe, "\\\\"); 
      strcat(tpPipeNames.StartPipe, pszServer);
      strcat(tpPipeNames.StartPipe, START_PIPE_NAME);
      strcpy(tpPipeNames.GamePipe, "\\\\");   
      strcat(tpPipeNames.GamePipe, pszServer);
      strcat(tpPipeNames.GamePipe, GAME_PIPE_NAME);
   }

   else
   {
      strcpy(tpPipeNames.StartPipe, START_PIPE_NAME);
      strcpy(tpPipeNames.GamePipe, GAME_PIPE_NAME);
   }

   /* create event semaphore to communicate with pipe thread */
   if ((BOOL)DosCreateEventSem(NULL, &hevWritePipe, DC_SEM_SHARED, FALSE))
   {
      usErrorMsg = IDMSG_CREATE_SEM_FAILED;
   }

   else if (0 > _beginthread(ManagePipe,
                             NULL,
                             STACKSIZE,
                             (VOID *)&tpPipeNames))
   {
      usErrorMsg = IDMSG_CREATE_THREAD_FAILED;
   }

   if (usErrorMsg != 0)
   {
      MessageBox(hwndMain, usErrorMsg, 0, MB_WARNING | MB_OK, NULL, TRUE);
   }

   return((BOOL)(!(BOOL)usErrorMsg));
}


MRESULT EXPENTRY ServerDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   ULONG        ulErrorReply=0, ulUserCommand;
   FILESTATUS   fs;
   APIRET       apiRC;
   UCHAR        PathName[MAXPIPENAME];

   switch (msg)
   {
      case WM_COMMAND:
         switch ((ulUserCommand=SHORT1FROMMP(mp1)))
         {
            case ID_OK:
               if (0 != WinQueryWindowText(WinWindowFromID(hwnd,
                                                           DID_PIPE_SVR),
                                           MAXSERVERNAME,
                                           pszServer)
                   && pszServer)
               {
                  strcpy(PathName, "\\\\");
                  strcat(PathName, pszServer);
                  strcat(PathName, GAME_PIPE_NAME);

                  while (ERROR_FILE_NOT_FOUND == (apiRC=DosQueryPathInfo(PathName,
                                                                  FIL_STANDARD,
                                                                  &fs,
                                                                  sizeof(FILESTATUS)))
                         || ERROR_PATH_NOT_FOUND == apiRC
                         || ERROR_BAD_NETPATH == apiRC)
                  {
                     if (MBID_RETRY!=(ulErrorReply=MessageBox(hwnd,
                                               IDMSG_INVALID_NAME,
                                               0,
                                               MB_CUAWARNING | MB_ABORTRETRYIGNORE,
                                               NULL,
                                               TRUE)))
                     {
                        if (MBID_ABORT == ulErrorReply)
                        {
                           ulUserCommand = ID_CANCEL;
                        }

                        break;
                     }
                  }
               }

               else
               {
                  ulUserCommand = ID_CANCEL;
               }

            case ID_CANCEL:
               WinDismissDlg(hwnd, ulUserCommand);
               return (MRFROMLONG(0));
         }
         break;
   }
   return(WinDefDlgProc(hwnd, msg, mp1, mp2));
}


VOID ManagePipe(VOID *tpPipeNames)
{
   BOOL    fSuccess=TRUE;
   USHORT  usMsg;
   ULONG   ulErrorMsg, ulIcon;
   HPIPE   hpPipe;
   PSZ     pszServer;
   ULONG   ulAction;
   APIRET  apiRC;

/*
 *   Attempt to open named pipe.  If the pipe is busy, wait on it, and when
 *   it becomes free try to open it again.  Continue until success or error.
 */

   do
   {
      apiRC = DosOpen(((THREADPARAM *)tpPipeNames)->StartPipe,  /* pointer to filename      */
                (PHFILE) &hpPipe,            /* pointer to variable for file handle          */
                &ulAction,                   /* pointer to variable for action taken         */
                0L,                          /* file size if created or truncated            */
                0L,                          /* file attribute                               */
                FILE_OPEN,                   /* action taken if file exists/does not exist   */
                FS_CLI_STARTPIPE_OPEN_MODE,  /* open mode of file                            */
                NULL);                       /* pointer to structure for extended attributes */
   }
   while (ERROR_PIPE_BUSY == apiRC &&
          !(BOOL)(apiRC=DosWaitNPipe(pszServer, NP_INDEFINITE_WAIT)));

   if (apiRC != 0)
   {
      ulErrorMsg = IDMSG_CONNECT_FAILED;
      ulIcon = MB_CUAWARNING | MB_OK;
      fSuccess = FALSE;
   }

   if (TRUE == fSuccess)
   {
      /* connect to the game pipe */
      if ((BOOL)(hpPipe = ConnectGamePipe(hpPipe,
                                          ((THREADPARAM *)tpPipeNames)->GamePipe)))
      {
         /* enable the File-Start menu choice */
         WinPostMsg(hwndMain, WM_START, MPVOID, MPVOID);

         while (WritePipe(hpPipe, &usMsg) &&
                usMsg != CLIENT_QUIT &&
                usMsg != ERROR_MSG)
         {
            if (!(BOOL)ReadPipe(hpPipe, &usMsg))
            {
               break;
            }
         }
      }

      else
      {
         ulErrorMsg = IDMSG_CONNECT_FAILED;
         ulIcon = MB_CUAWARNING | MB_OK;
         fSuccess = FALSE;
      }
   }

   if (FALSE == fSuccess)
   {
      WinPostMsg(hwndMain,
                 WM_MSG,
                 MPFROM2SHORT(ulErrorMsg, ulIcon),
                 MPFROMLONG(apiRC));
   }

   if (!fAppExit)
   {
      fAppExit = TRUE;
   }

   DosCloseEventSem(hevWritePipe);
   DosClose(hpPipe);
   fThrdsDead = TRUE;
}


/*
 *   open game pipe, notify server of success or failure, and close start pipe.
 */
HPIPE ConnectGamePipe(HPIPE hpPipe, PSZ pszGamePipe)
{
   HPIPE     hpGamePipe;
   USHORT    usMsg=ERROR_MSG;
   ULONG     ulCount, ulAction;
   PIPE_MSG  pmsgPipeMsg;
   APIRET    apiRC;

   /* block on read, and open game pipe if server sends START_MSG */
   if ((BOOL)ReadPipe(hpPipe, &usMsg) &&
       START_MSG == usMsg)
   {

      do
      {
         apiRC = DosOpen(pszGamePipe,         /* pointer to filename                          */
                  (PHFILE) &hpGamePipe,       /* pointer to variable for file handle          */
                  &ulAction,                  /* pointer to variable for action taken         */
                  0L,                         /* file size if created or truncated            */
                  0L,                         /* file attribute                               */
                  FILE_OPEN,                  /* action taken if file exists/does not exist   */
                  FS_CLI_GAMEPIPE_OPEN_MODE,  /* open mode of file                            */
                  NULL);                      /* pointer to structure for extended attributes */
      }
      while (ERROR_PIPE_BUSY == apiRC &&
             !(BOOL)(apiRC=DosWaitNPipe(pszGamePipe, NP_INDEFINITE_WAIT)));
   }

   if (apiRC != 0)
   {
      usMsg = ERROR_MSG;
      hpGamePipe = 0;
   }

   if (START_MSG == usMsg)
   {
      pmsgPipeMsg.usMsgType = usMsg;
      pmsgPipeMsg.usMsgData = 0;

      /* notify server of success or failure */
      DosWrite(hpPipe, (VOID *)&pmsgPipeMsg, sizeof(PIPE_MSG), &ulCount);
   }

   /* if it is not a start message, return an error */
   else
   {
      hpGamePipe = 0;
   }

   /* close start pipe */
   DosClose(hpPipe);
   return(hpGamePipe);
}


/*
 *   wait for PM thread to post message, and write it to pipe.
 */
BOOL WritePipe(HPIPE hpPipe, USHORT *usMsg)
{
   ULONG   ulCount;
   BOOL    fSuccess=TRUE;
   APIRET  apiRC;


   if (!(BOOL)DosWaitEventSem(hevWritePipe, SEM_INDEFINITE_WAIT))
   {
      DosResetEventSem(hevWritePipe, &ulCount);
      if ((BOOL)DosWrite(hpPipe, (VOID *)&pmsgPipeMsg, sizeof(PIPE_MSG), &ulCount))
      {
         WinPostMsg(hwndMain, WM_MSG, MPFROM2SHORT(IDMSG_PIPE_WRITE_FAILED, MB_CUAWARNING | MB_OK), MPFROMLONG(apiRC));
         fSuccess = FALSE;
      }

      *usMsg = pmsgPipeMsg.usMsgType;
   }

   else if (!fAppExit)
   {
      WinPostMsg(hwndMain, WM_MSG, MPFROM2SHORT(IDMSG_SEMAPHORE_ERROR, MB_CUAWARNING | MB_OK), MPVOID);
      fSuccess = FALSE;
   }

   return(fSuccess);
}


/*
 *   Read message from pipe, and post to PM thread.
 */
BOOL ReadPipe(HPIPE hpPipe, USHORT *usMsg)
{
   static PIPE_MSG  pmsgPipeMsg;
   BOOL             fSuccess=TRUE;
   ULONG            ulCount;
   APIRET           apiRC;

   /* DosRead returns an error code of 0 and 0 bytes read with a broken pipe */
   if (!(BOOL)DosRead(hpPipe, (VOID *)&pmsgPipeMsg, sizeof(PIPE_MSG), &ulCount)
       && ulCount > 0)
   {
      if (START_MSG != (*usMsg = pmsgPipeMsg.usMsgType))
      {
         WinPostMsg(hwndMain, (WM_USER+pmsgPipeMsg.usMsgType), MPFROMSHORT((SHORT)pmsgPipeMsg.usMsgData), MPVOID);
      }
   }

   else if (!fAppExit)
   {
      WinPostMsg(hwndMain, WM_MSG, MPFROM2SHORT(IDMSG_PIPE_READ_FAILED, MB_CUAWARNING | MB_OK), MPFROMLONG(apiRC));
      fSuccess = FALSE;
   }

   return(fSuccess);
}

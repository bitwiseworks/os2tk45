/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MDM.C
*
* DESCRIPTIVE NAME:  OS/2 2.x Media Device Manager Proxy DLL Source.
*
*              DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*              sample code created by IBM Corporation. This sample code is not
*              part of any standard or IBM product and is provided to you solely
*              for  the purpose of assisting you in the development of your
*              applications.  The code is provided "AS IS", without
*              warranty of any kind.  IBM shall not be liable for any damages
*              arising out of your use of the sample code, even if they have been
*              advised of the possibility of   such damages.
*
* FUNCTION:    The Source defines stub or proxy routines for three OS/2
*              APIs namely mciSendCommand(),mciSendString(), and
*              mdmDriverNotify(). The True Worker routines for these APIs
*              reside in MDM.DLL (Already provided by the system). When
*              this source is built another MDM.DLL is created, which
*              has forwarding entries to the System provided MDM.DLL.
*              Note that the above three APIs are not only exported here
*              but are also imported from the true MDM.DLL.
*
* INSTALLATION:
*              In order to begin monitoring MCI messages the existing
*              MDM.DLL must be renamed to MCI.DLL. To do this use a tool
*              called DLLRNAME. DLLRNAME is provided as part of IBM
*              CSET/2 compiler. ex DLLRNAME d:\mmos2\dll\mdm.dll mdm=mci
*              is the command you would actually type if the true MDM.DLL
*              resides on drive D:.
*              Then Copy the stub MDM.DLL (this source product) to d:\toolkt
*              \DLL\MDM.DLL.
*              If MDM.DLL cannot be renamed or replaced then the DLL may be
*              in use because of system sounds. Temporarily Deinstall System
*              sounds by running \mmos2\install\DINSTSND.CMD. Yoy may have
*              to reboot after this. After the reboot you can replace the
*              MDM.DLL.
*
*
* NOTES:       Linker Warnnings of imported export records can be ignored.
*              Save a copy of your MDM.DLL before you begin the replacement.
*
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_BASE                      /* OS/2 Base definitions           */
#define INCL_DOS                       /* DosCall Definitions include     */
#define INCL_WIN                       /* PM      Definitions include     */
#include <os2.h>                       /* OS/2 2.x System include         */
#include <string.h>                    /* C String Functions include      */
#include <stdio.h>                     /* C Standard Functions include    */
#include <stdlib.h>                    /* C Thread Functions include      */
#include <process.h>                   /* C Thread Functions include      */
#include <os2me.h>                     /* Multimedia System include       */
#include "mdm.h"                       /* Component Private Definitions   */

THREAD_BLOCK      *pThreadBlk;      /* A thread Parameter block struct   */
PIB               *PIDInfo;         /* Process Information Block         */
TIB               *TIDInfo;         /* Thread Information Block          */

#pragma data_seg(SHR_SEG)
HAB               hab=0;            /* Handle to an anchor block (thrd)  */
HWND              hwndSpyWin;       /* Spy Application Window Handle     */
OPEN_LIST         OpenList;         /* Device Ids of open list           */
DRIVERENTRY       mdmSendCmd;       /* SendCommand Address in true MDM   */
SENDSTR           mdmSendStr;       /* SendString Address in true MDM    */
HMTX              hmtxProcSem;      /* Thread Block Serialization Sem.   */
PVOID             pvNotPacket;      /* Shared Memory Packet              */
BOOL              fGlobalInit = FALSE; /* Initialization Flag       */
BOOL              fSpyRegistered = FALSE; /* Spy Application Registered State  */
BOOL              fHookInp = FALSE;       /* Spy Application Registered State  */
BOOL              fSpySysSounds = FALSE; /* Spy Application Registered State  */
ULONG             ulCount = 0;             /* Number of Attachments   */
ULONG             ulSysSndPID = 0;         /* Number of Attachments   */


/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    mciSendCommand() API Intercept Routine
*
* ARGUMENTS:
*
* RETURN:
*
* DESCRIPTION: Proxy For mciSendCommand. Worker is in MCI.DLL
*
*
* OS/2 API's USED:  mciSendCommand() MMPM/2 Command Interface API
**************************************************************************/
ULONG APIENTRY mciSendCommand  (USHORT   wDeviceID,
                                USHORT   wMessage,
                                ULONG    dwParam1,
                                PVOID    dwParam2,
                                USHORT   wUserParm)

{
  ULONG    rc = 0;                     /* API Return Code */
  USHORT   i;                          /* Counter         */
  ULONG    ulPostCount;

  /* At application initilization time (MCISPY.EXE) registers
  ** itself by calling the Exported API RegisterSpy, and passing
  ** the PM Window Handle for Notifications. The Stub DLL stores
  ** the Window handle and sets an intertnal flag to indicate
  ** the successful registration.
  */

  if (fSpyRegistered)
   {
      /* Grab the Serialization Semaphore */

      DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

      /* Fill up the thread block parameters in Shared memory */

      pThreadBlk->wDevID = wDeviceID;
      pThreadBlk->wMsg =   wMessage;
      pThreadBlk->dwParam1 =dwParam1;
      pThreadBlk->dwParam2 =(ULONG)dwParam2;
      pThreadBlk->wUserParm =wUserParm;

     /* Update Process Information */
      DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
      pThreadBlk->ulPID = PIDInfo->pib_ulpid;

      if (pThreadBlk->ulPID != ulSysSndPID)
       {
         /* Send the Message to the MCISPY application */
         WinSendMsg (hwndSpyWin,WM_COMMAND,
                     MPFROM2SHORT (SPY_NOTIFY,pThreadBlk->wMsg),(MPARAM) 0) ;
       }
      else
       {
         DosPostEventSem (pThreadBlk->SysSndsEvtSem);
         DosWaitEventSem (pThreadBlk->SysSndsSyncSem, SEM_INDEFINITE_WAIT);      /* Set Thread Sem */
         DosResetEventSem (pThreadBlk->SysSndsSyncSem, &ulPostCount);
       }
      /* Release The Serialization Semaphore    */

      DosReleaseMutexSem(hmtxProcSem);
   }

   /* Forward the Call to the true Worker in MCI.DLL */

   if (!rc)
    {
       rc = mdmSendCmd (wDeviceID,wMessage,dwParam1,dwParam2,wUserParm);
    }

    /*
    ** In order to provide useful application options like instance
    ** based filtering of MCI messages, MCI_OPEN and MCI_CLOSE messages
    ** are counted.
    ** CAVEATS. Automatic Identification of devices, Element Opens are
    **          Ignored.
    */

   if (! LOUSHORT(rc) && (wMessage == MCI_OPEN))
    {
       MCI_OPEN_PARMS  *pOpenParms;

       DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
       pOpenParms = (MCI_OPEN_PARMS *)dwParam2;
       if (dwParam1 & MCI_OPEN_TYPE_ID)
        {
          /* Store the Device Type  */
          OpenList.ulOpenList[OpenList.ulNumOpen].usDevTyp = LOUSHORT(pOpenParms->pszDeviceType);
        }
       /* Store the PID, Device ID */

       /* Update Process Information */
       DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
       OpenList.ulOpenList[OpenList.ulNumOpen].ulDevId = pOpenParms->usDeviceID;
       OpenList.ulOpenList[OpenList.ulNumOpen].ulPID = PIDInfo->pib_ulpid;
       OpenList.ulNumOpen++;
       DosReleaseMutexSem(hmtxProcSem);
    }
   if (! LOUSHORT(rc) && (wMessage == MCI_CLOSE))
    {
       DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
       for (i=0; i < OpenList.ulNumOpen; i++)
        {
         if (OpenList.ulOpenList[i].ulDevId == wDeviceID)
          {
             OpenList.ulOpenList[i].ulDevId = 0;
             OpenList.ulOpenList[i].fExclude = FALSE;
             break;
          }
        }
        OpenList.ulNumOpen--;
       DosReleaseMutexSem(hmtxProcSem);
    }
    if (fSpyRegistered && (LOUSHORT(rc) != MCIERR_SUCCESS))
     {
        DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
        pThreadBlk->ulReturnCode = rc;
        WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT (SPY_ERRRC,pThreadBlk->wMsg),
                    (MPARAM) 0) ;
        DosReleaseMutexSem(hmtxProcSem);
     }

   return (rc);
}


/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    mciSendString
*
* ARGUMENTS: Published API Arguments
*
* RETURN:
*
* DESCRIPTION: Proxy for mciSendString(). Worker is in MCI.DLL
*              .
*
* OS/2 API's USED:  mciSendString() MMPM/2 Command Interface API
*
**************************************************************************/
ULONG APIENTRY  mciSendString   (PSZ     lpstrCommandBuf,
                                 PSZ     lpstrReturnString,
                                 USHORT  wReturnLength,
                                 HWND    hCallBack,
                                 USHORT  wUserParm )

{

  ULONG    rc = 0;                      /* API return Code      */
  SZ       szTemp[2048];                /* Temporary Buffer     */
  PCHAR    pchWord;                     /* Tokenized Word Ptr   */
  PCHAR    pchDevType;                  /* Device Type          */
  char     tokensep[] = " \t,\n";       /* Token Seperators     */
  PCHAR    pchAlias;                    /* PCH Alias            */
  USHORT   i;                           /* Loop Index           */

  /* Grab The Serialization Semaphore  */

  DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

  /* Copy the InComing String Command for later use */

  strcpy (szTemp,(char *)lpstrCommandBuf);

  /* Release the Serialization Semaphore */

  DosReleaseMutexSem(hmtxProcSem);

  if (fSpyRegistered)
   {

      DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
      pThreadBlk->wUserParm = wUserParm;

     /* Update Process Information */
      DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
      pThreadBlk->ulPID = PIDInfo->pib_ulpid;

      strcpy (pThreadBlk->szCmd,(char *)lpstrCommandBuf);

      /* Send the Message to the MCISPY application */

      WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT (SPY_STRING,0),
                  (MPARAM) 0) ;

      DosReleaseMutexSem(hmtxProcSem);
   }

  /*
  ** Forward the call to the true API worker in MCI.DLL
  */
  if(!rc)
     rc = mdmSendStr (lpstrCommandBuf,lpstrReturnString,wReturnLength,
                      hCallBack,wUserParm );

  if (!LOUSHORT(rc))
   {
      DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

      /* Get The First token in the command string */
      pchWord = strtok (szTemp,tokensep);

      if (strcmpi (pchWord,"open") == 0)
      {
         /* Next Token is the device type */

         /* CAVEATS: Element opens will be ignored and the filter will not
         ** Work.
         */
         pchDevType = strtok(NULL,tokensep);

         while ((pchWord = strtok(NULL,tokensep)) != NULL)
           {
             if (strcmpi (pchWord,"alias") == 0)
              {
                /* look ahead to get the alias */

                pchAlias = strtok(NULL,tokensep);

                /* Retrieve the Alias, PID and Device Type */

                strcpy (OpenList.szAlias[OpenList.ulNumOpen].szAlias,pchAlias);
                strcpy (OpenList.szAlias[OpenList.ulNumOpen].szDevTyp,pchDevType);

                /* Update Process Information */
                DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
                pThreadBlk->ulPID = PIDInfo->pib_ulpid;
                OpenList.szAlias[OpenList.ulNumOpen].ulPID = PIDInfo->pib_ulpid;
                OpenList.ulNumOpen++;
                break;
              }
           }
      } /* "open" */
      else if (strcmpi (pchWord,"close") == 0)
      {
         pchWord = strtok (NULL,tokensep);   /* This should be the alias */

         for (i=0; i < OpenList.ulNumOpen; i++)
           {
            if (strcmpi (pchWord,OpenList.szAlias[i].szAlias) == 0)
             {
               strcpy (OpenList.szAlias[i].szAlias,"");
               OpenList.szAlias[i].fExclude = FALSE;
               OpenList.ulNumOpen--;
               break;
             }
           }
      }/* "close" */
      DosReleaseMutexSem(hmtxProcSem);
   }

  if (fSpyRegistered && (LOUSHORT(rc) != MCIERR_SUCCESS))
   {

      DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
      pThreadBlk->ulReturnCode = rc;
      WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT (SPY_ERRRC,pThreadBlk->wMsg),
                  (MPARAM) 0) ;
      DosReleaseMutexSem(hmtxProcSem);
   }
  return (rc);
}


#pragma linkage (RegisterSpy,system)

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    RegisterSpy()
*
* ARGUMENTS:  Window Handle and REGISTER/DEREGISTER Flag.
*
* RETURN:
*
* DESCRIPTION: Monitor System Queue to retrieve RM messages.
*
**************************************************************************/

APIRET APIENTRY RegisterSpy (HWND hwndspy,ULONG ulApiMode)
{

  if (ulApiMode && fSpyRegistered)
      return (100);


  if (ulApiMode)
   {
     DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);
     fSpyRegistered = TRUE;
     hwndSpyWin = hwndspy;
     DosReleaseMutexSem(hmtxProcSem);
   }
   else
   {
     /* Release Our Input hook. But the DLL Still can't be freed. See
     ** PM Refererence for more details.
     */
     fSpyRegistered = FALSE;
     hwndSpyWin = 0;
   }

   return (0);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    QueryOpenInstance()
*
* ARGUMENTS: Pointer to an OPEN_LIST structure.
*
* RETURN:
*
* DESCRIPTION: Return Open Instance Info to the application.
*              .
*
* OS/2 API's USED:
*
**************************************************************************/

APIRET APIENTRY QueryOpenInstance (OPEN_LIST * pulList,BOOL fAlias)
{
  USHORT i ;

  /* Obtain the serialization semaphore */
//  DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

  if (fAlias)
  {
     pulList->ulNumOpen =OpenList.ulNumOpen;

     for (i=0; i < MAXOPENS; i++)
       {
         pulList->ulOpenList[i].usDevTyp = OpenList.ulOpenList[i].usDevTyp;
         pulList->ulOpenList[i].ulDevId =  OpenList.ulOpenList[i].ulDevId;
         pulList->ulOpenList[i].ulPID =    OpenList.ulOpenList[i].ulPID;
       }
  }
  else
  {
     pulList->ulNumOpen =OpenList.ulNumOpen;
     for (i=0; i < MAXOPENS; i++)
       {
          strcpy (pulList->szAlias[i].szAlias,OpenList.szAlias[i].szAlias);
          strcpy (pulList->szAlias[i].szDevTyp,OpenList.szAlias[i].szDevTyp);
          pulList->szAlias[i].ulPID = OpenList.szAlias[i].ulPID ;
       }

  }
  /* Give up the Serialization Semaphore */
 // DosReleaseMutexSem(hmtxProcSem);
  return 0;

}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    _DLL_InitTerm()
*
* ARGUMENTS: ModuleHandle, Init or Term Flag
*
* RETURN:
*
* DESCRIPTION: Initilize the DLL Global Structures appropriately.
*              .
*
* OS/2 API's USED:
*
**************************************************************************/

#pragma linkage (_DLL_InitTerm,system)
unsigned long _System _DLL_InitTerm (ULONG hModule,ULONG ulFlag)
{
  ULONG    rc = 0;                     /* API Return Code    */
  SZ       LoadError[255];             /* DosLaodModule Err  */
  HMODULE  ModuleHandle;               /* Module Handle      */
  PFN      tProcAddr = NULL;           /* NULL PFN           */
  BOOL     fSysSndsInit = FALSE;       /* System Sounds      */

  switch (ulFlag)
    {
     if (ulFlag == 1) return (1L);

     case 0:

      /* Initialize The C Run Time Library Functions  */

       if (_CRT_init() != 0)
           return 0L;

       if (!fGlobalInit)
        {
           /* Create The Global Serialization Semaphore */
           DosCreateMutexSem(NULL,&hmtxProcSem,DC_SEM_SHARED,FALSE);

           /* Open the Serialization Semaphore   */
           DosOpenMutexSem (NULL,&hmtxProcSem);

           /* Obtain the serialization semaphore */
           DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

           rc = DosLoadModule(LoadError, sizeof(LoadError),"MCI",
                              &ModuleHandle);

           if (rc)
            {
              DosBeep (500,1500);
              DosExit (0,0);
            }
           rc = DosQueryProcAddr(ModuleHandle, 0L,"mciSendString", &(tProcAddr));

           if (rc)
            {
              DosBeep (500,1500);
              DosExit (0,0);
            }
           mdmSendStr = (SENDSTR)tProcAddr;
           tProcAddr = NULL;
           rc = DosQueryProcAddr(ModuleHandle, 0L,"mciSendCommand", &(tProcAddr));
           if (rc)
            {
              DosBeep (500,1500);
              DosExit (0,0);
            }
           mdmSendCmd = (DRIVERENTRY)tProcAddr;

           rc = DosAllocSharedMem (&pvNotPacket,(PSZ)"\\SHAREMEM\\SPYPACK.DAT",
                                   4096,OBJ_TILE |PAG_READ | PAG_WRITE | PAG_COMMIT );

           /*
            ** If error interrupt try again
            */
           if (rc == (ULONG)183)
            {
               rc = DosAllocSharedMem (&pvNotPacket,(PSZ)"\\SHAREMEM\\SPYPACK.DAT",
                                       4096,PAG_READ | PAG_WRITE);
            }
          fGlobalInit = TRUE;

          /* Obtain the Shared memory object */
          rc = DosGetNamedSharedMem ((PPVOID)&(pThreadBlk),
                                   (PSZ)"\\SHAREMEM\\SPYPACK.DAT",
                                   PAG_READ | PAG_WRITE );


          /* Update Process Information */

          DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
          pThreadBlk->ulPID = PIDInfo->pib_ulpid;

          /* Save System Sounds PID Information */

          ulSysSndPID =PIDInfo->pib_ulpid;

         /* This needs to be done because of System Sounds.
         ** You cannot install the hook at this DLL's initilization
         ** because desktop may not be created yet.
         */
          DosCreateEventSem(NULL,&(pThreadBlk->SysSndsEvtSem),DC_SEM_SHARED,FALSE);
          DosCreateEventSem(NULL,&(pThreadBlk->SysSndsSyncSem),DC_SEM_SHARED,FALSE);

         _beginthread(SystemSoundsThread,
                     (PVOID)NULL,
                     (ULONG)16000,
                     (PVOID)pThreadBlk);

          fSysSndsInit = TRUE;

          /* Set up the Global System Wide Input Hook */
          hab = WinInitialize(0);

          /* Resource Change messages are posted directly to the
          ** Application. In order to intercept those messages we
          ** register an Input hook on the system queue.
          */

          WinSetHook (hab,NULLHANDLE,HK_INPUT,(PFN)InputHookProc,hModule);
          fHookInp = TRUE;

         /* Give up the Serialization Semaphore */
         DosReleaseMutexSem(hmtxProcSem);
         }

      /* Open the Serialization Semaphore   */
      DosOpenMutexSem (NULL,&hmtxProcSem);

      /* Obtain the serialization semaphore */
      DosRequestMutexSem(hmtxProcSem, SEM_INDEFINITE_WAIT);

     if (!fSysSndsInit)
      {
         /* Obtain the Shared memory object */
         rc = DosGetNamedSharedMem ((PPVOID)&(pThreadBlk),
                                  (PSZ)"\\SHAREMEM\\SPYPACK.DAT",
                                  PAG_READ | PAG_WRITE );

         /* Update Process Information */
         DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
         pThreadBlk->ulPID = PIDInfo->pib_ulpid;
      }
     ulCount++;
     /* Give up the Serialization Semaphore */
     DosReleaseMutexSem(hmtxProcSem);

    break;

     case 1:
      _CRT_term();
       ulCount--;
      DosCloseMutexSem (hmtxProcSem);
      break;
    }
    return 1L;
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    InputHookProc()
*
* ARGUMENTS:  See Function.
*
* RETURN:
*
* DESCRIPTION: Monitor System Queue to retrieve RM messages.
*
**************************************************************************/
BOOL APIENTRY  InputHookProc   (HAB   hab,  PQMSG pQmsg, ULONG ulfs)
{

  if (fSpyRegistered)
   {
      if (fSpySysSounds)
       {
          /* Update the PID of the calling Process */
          DosGetInfoBlocks(&(TIDInfo), &(PIDInfo));
          pThreadBlk->ulPID = PIDInfo->pib_ulpid;
          pThreadBlk->wMsg     =  (USHORT)pQmsg->msg;
          pThreadBlk->dwParam1 =  (ULONG)pQmsg->mp1;
          pThreadBlk->dwParam2 =  (ULONG)pQmsg->mp2;

          switch (pQmsg->msg)
            {
             case MM_MCIPASSDEVICE:
               pThreadBlk->wDevID =    SHORT1FROMMP(pQmsg->mp1);
               pThreadBlk->dwParam2 =  SHORT1FROMMP(pQmsg->mp2);
              /* Send a Resource Change Message to the Application */
              WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT(SPY_DEVICE,0),
                          (MPARAM)pQmsg->mp2);
              break;

             case MM_MCINOTIFY:
             case MM_MCICUEPOINT:
             case MM_MCIPLAYLISTMESSAGE:
             case MM_MCIEVENT:
               pThreadBlk->wDevID =    SHORT1FROMMP(pQmsg->mp2);
               pThreadBlk->wUserParm = SHORT2FROMMP(pQmsg->mp1);
               pThreadBlk->dwParam1 =  SHORT1FROMMP(pQmsg->mp1);
              /* Send a Resource Change Message to the Application */
              WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT(SPY_NOTIFY,(USHORT)pQmsg->msg),
                          (MPARAM)pQmsg->mp2);
              break;

             case MM_MCIPOSITIONCHANGE:
               pThreadBlk->dwParam1 = (ULONG)pQmsg->mp2;
               pThreadBlk->dwParam2 = (ULONG)pQmsg->mp2;
               pThreadBlk->wUserParm = SHORT1FROMMP(pQmsg->mp1);
               pThreadBlk->wDevID =    SHORT2FROMMP(pQmsg->mp1);

              /* Send a Resource Change Message to the Application */
              WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT(SPY_NOTIFY,(USHORT)pQmsg->msg),
                          (MPARAM)pQmsg->mp2);
              break;
            }
       } /* SpySystemSounds */
   } /* Spy is Registered */
   return (FALSE);    /* let any other hook process it */
}/* Input Hook Proc */


/*********************************************************************/
/*********************************************************************/
VOID _Optlink SystemSoundsThread(PVOID ptemp)
{
  THREAD_BLOCK        *pThreadBlock;
  ULONG               ulPostCount;
  HMQ                 hmq=0;
  pThreadBlock = (THREAD_BLOCK *)ptemp;

  DosWaitEventSem (pThreadBlock->SysSndsEvtSem, SEM_INDEFINITE_WAIT);      /* Set Thread Sem */
  if (!hmq)
   {
      hmq = WinCreateMsgQueue(hab, 0); /* Create a msg queue to send SystemSounds Msgs */
   }
  /**************************************************************************/
  /* Loop and wait for work to do...                                        */
  /*                                                                        */
  /**************************************************************************/
  /* Run this code once, then block */
  while (1)
    {
    DosWaitEventSem (pThreadBlock->SysSndsEvtSem, SEM_INDEFINITE_WAIT);      /* Set Thread Sem */
    DosResetEventSem (pThreadBlock->SysSndsEvtSem, &ulPostCount);
    WinSendMsg (hwndSpyWin,WM_COMMAND,MPFROM2SHORT (SPY_NOTIFY,pThreadBlock->wMsg),
                (MPARAM) 0) ;
    DosPostEventSem (pThreadBlock->SysSndsSyncSem);
    }

}


/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    SpySystemSounds()
*
* ARGUMENTS: Toggle Flag to Enable/Disable System Sounds Monitor.
*
* RETURN:
*
* DESCRIPTION: Toggle Monitoring System Sounds.
*              .
**************************************************************************/

APIRET APIENTRY SpySystemSounds (BOOL fAlias)
{
  if (fAlias)
  {
    fSpySysSounds = TRUE;
  }
  else
  {
    fSpySysSounds = FALSE;
  }
  return 0;
}



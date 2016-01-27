/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MCISUBS.C
*
* DESCRIPTION: MCISPY helper routines.
*
*              Copyright (c) IBM Corporation   1993
*                        All Rights Reserved
*
*
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_WIN                        /* OS/2 2.x Win include            */
#define INCL_PM                         /* Presentation Manager include    */
#define INCL_DOS                        /* OS/2 2.x DOS API include        */
#include <os2.h>                        /* OS/2 2.x System include         */
#include <stdio.h>                      /* C Standard funcs include        */
#include <stdlib.h>                     /* C Standard library include      */
#include <string.h>                     /* C String Functions include      */
#include <stdarg.h>                     /* C Standard Arguments include    */
#define INCL_OS2MM                      /* OS/2 2.X MM Include             */
#include <os2me.h>                      /* Multimedia System include       */
#include "mcispy.h"                     /* Component Private Definitions   */
extern   MCISPY_BLOCK      SpyBlock;    /* MCISPY Application Data Block   */
extern   MCIMessageTable   MCIMsgTbl[]; /* Global Table of MCI Messages    */
LONG (* APIENTRY RegEntryProc) (HWND,ULONG);/* RegisterSpy() Func Pointer  */

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    GetSharedMemory
*
* DESCRIPTION: Obtain Shared Memory.
*
* OS/2 API's USED: DosGetSharedMem()
*
*****************************END OF SPECIFICATIONS ***************************/
void GetSharedMemory(MCISPY_BLOCK *pSpyBlock)
{
  ULONG rc;

  rc = DosGetNamedSharedMem (&pSpyBlock->pvPacket,
                            (PSZ) "\\SHAREMEM\\SPYPACK.DAT",
                             PAG_READ |PAG_WRITE);

  if (rc)
     mprintf ("Error Getting Shared Memory %d\n",rc);

}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    OpenFileDlg
*
* DESCRIPTION: Open File Dialog.
*
* OS/2 API's USED: WinFileDlg().
*
*****************************END OF SPECIFICATIONS ***************************/
RC OpenFileDlg (HWND hwndOwner,PSZ pszTitle,PSZ FileName, PSZ FullFile)
{
   FILEDLG  pfdFileDlg;                  /* File Dialog Structure */
   HWND     hwndFileDlg;                 /* Handle to the above   */

   /* Initialize the structure to 0 */

   memset (&pfdFileDlg,0,sizeof(FILEDLG));

   pfdFileDlg.cbSize   = sizeof(FILEDLG);
   pfdFileDlg.fl       = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG ;
   pfdFileDlg.pszTitle = pszTitle;

   strcpy (pfdFileDlg.szFullFile,FullFile);

   hwndFileDlg = WinFileDlg (HWND_DESKTOP,hwndOwner,&pfdFileDlg);

   if (hwndFileDlg == NULLHANDLE)
    {
       mprintf ("File Dialog Creation Error\n");
       return (TRUE);
    }

   if (hwndFileDlg && (pfdFileDlg.lReturn == DID_OK))
    {
       strcpy (FileName,pfdFileDlg.szFullFile);
       return (FALSE);
    }

   if (hwndFileDlg && (pfdFileDlg.lReturn == DID_CANCEL))
   {
       return (TRUE);
   }
   return (FALSE);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    OpenFile
*
* DESCRIPTION: Open File.
*
* OS/2 API's USED: DosOpen.
*
*****************************END OF SPECIFICATIONS ***************************/
APIRET OpenFile(PSZ szFileName,ULONG *pFileHandle)
{
   ULONG    ulAction;                 /* Action to be taken */
   APIRET   RetCode;                  /* API return code    */
   APIRET   rc;                       /* API return code    */
   char     szHeader[256];            /* LogFile Header     */
   HFILE    FileHandle;               /* Temp FileHandle    */
   ULONG    ulBytesWritten;           /* Hedaer Bytes       */

   RetCode = DosOpen (szFileName, pFileHandle, &ulAction, 0,
                      FILE_NORMAL,
                      OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW,
                      OPEN_FLAGS_WRITE_THROUGH | OPEN_FLAGS_NO_CACHE|
                      OPEN_FLAGS_SEQUENTIAL | OPEN_SHARE_DENYNONE|
                      OPEN_ACCESS_READWRITE,
                      0);

   if (!RetCode)
    {

       FileHandle = *pFileHandle;
       sprintf (szHeader,"%s\t\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n","Date","Time","PID","DeviceID",
               "MCI Message", "Flags","UserParam","Data");
       rc = DosWrite (FileHandle,szHeader,strlen(szHeader),&ulBytesWritten);

    }

   return (RetCode);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    Write2File
*
* ARGUMENTS:   HFILE FileHandle,   PSZ buffer
*
* RETURN:      RC
*
* DESCRIPTION: This function gets Date and Time and write it to the log file
*
* OS/2 API's USED: DosGetDateTime, DosWrite
*
* C FUNCTION CALLS: Sprintf
*
**************************************************************************/
APIRET Write2File(HFILE FileHandle,PSZ buffer)
{
   ULONG    ulBytesWritten;            /* Bytes Written   */
   APIRET   ReturnCode;                /* API return Code */
   CHAR     chEOL[2] = "\r\n";         /* End of Line     */
   CHAR     szTimeDate[32];            /* Time and Date   */
   DATETIME dt;                        /* Date and Time   */
   ULONG    rc;                        /* API Return      */


   if (FileHandle == (HFILE)NULL)
       return(1) ;

   if (buffer == (PSZ)NULL)
       return(1) ;

   ReturnCode = DosGetDateTime(&dt);
   if (ReturnCode)
   {
       strcpy (buffer, "DosGetDateTime failure:");
       DosWrite (FileHandle,buffer, strlen(buffer),&ulBytesWritten);
       return (ReturnCode);
   }

   sprintf (szTimeDate,"%04d-%02d-%02d  %02d:%02d:%04d  ",
            dt.year, dt.month,dt.day,dt.hours,dt.minutes,
            dt.seconds );


   ReturnCode = DosWrite (FileHandle,szTimeDate,
                          strlen(szTimeDate),&ulBytesWritten);

   ReturnCode = DosWrite (FileHandle,buffer,
                          strlen(buffer),&ulBytesWritten);

   ReturnCode = DosWrite (FileHandle, chEOL,
                          strlen(chEOL),&ulBytesWritten);

   return (ReturnCode);

}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    RegisterSpyWithMDM
*
*
* DESCRIPTION: Register MCISPY with MDM
*
**************************************************************************/
VOID  RegisterSpyWithMDM (MCISPY_BLOCK *pSpyBlock)
{
   APIRET                 rc ;              /* API Return Code    */
   char                   LoadError[255];   /* Load Errors        */
   HMODULE                ModuleHandle;     /* ModuleHandle       */


   rc = DosLoadModule ((PSZ)LoadError, sizeof(LoadError),
                       (PSZ)"MDM",
                       &ModuleHandle);
   if (rc)
    {
      mprintf ("Error Loading MDM.DLL %d\n",rc);
      DosExit (0,0);
    }

   rc = DosQueryProcAddr(ModuleHandle, 0L,
                         (PSZ)"RegisterSpy", (PFN *)&RegEntryProc);

   if (rc)
    {
      mprintf ("Error Proc Address(RegisterSpy)  %d\n",rc);
      DosExit (0,0);
    }

   rc = DosQueryProcAddr(ModuleHandle, 0L,
                         (PSZ)"QueryOpenInstance", (PFN *)&(pSpyBlock->QueryOpenInstance));

   if (rc)
    {
      mprintf ("Error Proc Address(QueryOpenInstance)  %d\n",rc);
      DosExit (0,0);
    }

   rc = DosQueryProcAddr(ModuleHandle, 0L,
                         (PSZ)"SpySystemSounds", (PFN *)&(pSpyBlock->SystemSounds));

   if (rc)
    {
      mprintf ("Error Proc Address(SystemSounds)  %d\n",rc);
      DosExit (0,0);
    }


   /* Register Spy with MDM */
   rc = RegEntryProc ((HWND)pSpyBlock->hwnd1,TRUE);


   if (rc)
    {
      mprintf ("Only One Instance Of MCISPY is allowed \n");
      DosExit (0,0);
    }

   /* Initially Disable Driver Notifications */
   pSpyBlock->SystemSounds (FALSE);

}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    DisplayMCIMsg
*
* ARGUMENTS:  Shared memory Data and Packet
*
* RETURN:
*
* DESCRIPTION: Display the message.
*
**************************************************************************/
void  DisplayMCIMsg (MCISPY_BLOCK * pSpyBlock)
 {
   USHORT          i,j;                         /* Loop Indicies     */
   char            szMsg[124];                  /* Message String    */
   char            Mlebuffer[1024];             /* Mle Buffer        */
   THREAD_BLOCK    *pNotPacket;                 /* Shared Mem Packet */
   USHORT          usDeviceType;                /* Device Type       */
   BOOL            fFoundMsg = FALSE;
   char     szSpace[11]= "    ";
   char     szSpace2[50]="                                            ";


   /* Derefernce the Shared memory packet   */

   pNotPacket = (THREAD_BLOCK *)pSpyBlock->pvPacket;
   memset (Mlebuffer,0,sizeof(Mlebuffer));

   switch (pNotPacket->wMsg)
     {
     case MM_MCIPOSITIONCHANGE:
          if (MCIMsgTbl[58].fExclude)
           {
              return ;
           }
          sprintf (Mlebuffer,"%03d %s %02d %s %s  %d %s %04d \n",
                   pNotPacket->ulPID,szSpace, pNotPacket->wDevID,szSpace,
                   "  MM_MCIPOSITIONCHANGE     Media Position = ",
                   pNotPacket->dwParam1,szSpace,  pNotPacket->wUserParm);
          fFoundMsg = TRUE;
      break;

      default:
           if (pNotPacket->wMsg == MM_MCIPASSDEVICE)
              return;
          /* First obtain the device type of this device id */
          if (pSpyBlock->fDevTypFil)
           {
              /* Update Open Instance Information */
              pSpyBlock->QueryOpenInstance ((OPEN_LIST *)&pSpyBlock->OpenList,FALSE);

              for (j=0; j < pSpyBlock->OpenList.ulNumOpen; j++)
                {
                   if (pSpyBlock->OpenList.ulOpenList[j].ulDevId == pNotPacket->wDevID)
                    {
                       usDeviceType =pSpyBlock->OpenList.ulOpenList[j].usDevTyp;
                    } /* Current Device Id = OpenList Id */
                } /* Number of Open Instances */

              /* Check if device type for this device type is on.
              ** If it is then we return without displaying this message.
              */
              for (i=0; i <MAXDEVICES; i++)
                {
                  if (pSpyBlock->ulDevTypFil[i] == usDeviceType)
                   {
                      return;
                   } /* device type match */
                } /* max Devices */
           } /* Device Type Filter is on */

          /* Check if Global MCI message filter for this message is on.
          ** If it is then we return without displaying this message.
          */
          for (i=0; i <MAX_MCI_MSGS; i++ )
            {
              if (MCIMsgTbl[i].usMsg == pNotPacket->wMsg)
               {
                  /* Global Message Filter is Active for this message */

                  if (MCIMsgTbl[i].fExclude)
                   {
                     return;
                   }

                  strcpy (szMsg,MCIMsgTbl[i].szMessage);
                  strncat (szMsg,szSpace2,27-strlen(szMsg));
                  fFoundMsg = TRUE;
                  break;
               }/* Found the Message */
            } /* Max MCI Msgs loop */
            if (pSpyBlock->fDevIdFil)
             {
                /* Update Open Instance Information */
                pSpyBlock->QueryOpenInstance ((OPEN_LIST *)&pSpyBlock->OpenList,FALSE);
                for (j=0; j < pSpyBlock->OpenList.ulNumOpen; j++)
                  {
                     if (pSpyBlock->OpenList.ulOpenList[j].ulDevId == pNotPacket->wDevID)
                      {
                        if (pSpyBlock->OpenList.ulOpenList[j].fExclude)
                         {
                           return;
                         }/* Exclude is true */
                      } /* Current Device Id = OpenList Id */
                  } /* Number of Open Instances */
             } /* Device Id Filter in use */

          if (pNotPacket->wDevID  == 65535)
               pNotPacket->wDevID = 0;
          if (fFoundMsg)
           {
             sprintf (Mlebuffer,"%03d %s %02d  %s  %s  0x%05X %s  %04d %s 0x%05X \n",
                      pNotPacket->ulPID, szSpace,pNotPacket->wDevID,szSpace,
                      szMsg, pNotPacket->dwParam1,szSpace,
                      pNotPacket->wUserParm,szSpace,pNotPacket->dwParam2);

           }
      break;

     }/* Switch */
   if (fFoundMsg)
    {
       WinSendMsg (pSpyBlock->hwndMLE,MLM_SETSEL,MPFROMLONG(pSpyBlock->ipt),
                   MPFROMLONG(pSpyBlock->ipt));

       WinSendMsg (pSpyBlock->hwndMLE,MLM_INSERT,MPFROMLONG(Mlebuffer),
                   (MPARAM)0);

       pSpyBlock->ipt= (IPT)WinSendMsg (pSpyBlock->hwndMLE,MLM_QUERYSEL,MPFROMSHORT(MLFQS_CURSORSEL),
                                        0L);

       if (pSpyBlock->fNoLog)
           Write2File (pSpyBlock->hfileLog,Mlebuffer);

    }/* Found Msg */
 }

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    mprintf
*
* ARGUMENTS:   Error Message String.
*
* RETURN:      None.
*
* DESCRIPTION: Write to Log File for command logging.
*
* OS/2 API's USED:   WinAlarm()
*                    WinMessageBox().
*
* C FUNCTION CALLS:  malloc()
*                     va_start();
*
*
* INTERNAL FUNCTION REFERENCES: None.
*
*****************************END OF SPECIFICATIONS ***************************/

LONG mprintf(PSZ format, ...)
{
   va_list pArg;
   SZ     buffer[513];
   LONG    lCount;

   WinAlarm( HWND_DESKTOP, WA_ERROR );


   va_start(pArg, format);

   lCount = vsprintf(buffer, format, pArg);

   WinMessageBox( HWND_DESKTOP,       /* parent window handle    */
                  HWND_DESKTOP,       /* owner window handle     */
                  buffer,             /* pointer to message text */
                  "MCISPY",            /* pointer to title text   */
                  MSG_BOX_ID,         /* message box identifier  */
                  MB_OK | MB_ERROR ); /* message box style       */

   return lCount;
}   /*  end of mprintf()  */

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION     LongToString
*
* C FUNCTION CALLS:     _ltoa()
*                       malloc()
*
* GLOBAL VARIABLES MODIFIED:
*
*****************************END OF SPECIFICATIONS ***************************/
PSZ LongToString(LONG l)
{
    PSZ   pszReturn;     /* Temporary Storage */
    CHAR  cString[34];   /* Buffer            */

    pszReturn = _ltoa(l, cString, 10);
    return ((PSZ)cString);

}  /* LongToString */

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION: Initialize()
*
* ARGUMENTS:    None.
*
* RETURN:       None.
*
* DESCRIPTION:
*
* OS/2 API's USED: WinCreateHelpInstance()
*                  WinDestroyHelpInstance()
*
* C FUNCTION CALLS:
*
* INTERNAL FUNCTION REFERENCES: None.
*
* GLOBAL VARIABLES REFERENCED: helpinit data structure.
*                              hwndHelpInstance.
*
* GLOBAL VARIABLES MODIFIED: helpinit data structure.
*                            hwndHelpInstance.
*
*****************************END OF SPECIFICATIONS ***************************/
VOID Initialize(MCISPY_BLOCK * pSpyBlock)
{
    int        i;          /* Loop Index  */
    APIRET     rc;         /* Return Code */
    HELPINIT   helpinit;   /* Help Init   */

    /* Initialize our default log and script file names */

    strcpy (pSpyBlock->szLogFile,"MCISPY.LOG");
    OpenFile (pSpyBlock->szLogFile,&pSpyBlock->hfileLog);


    /* Initilaize the Help Manager structures */

    pSpyBlock->fNoCmd = pSpyBlock->fNoStr = pSpyBlock->fNoDrv = FALSE;
    pSpyBlock->fNoLog = FALSE;
    memset (&helpinit,0,sizeof(HELPINIT));

    helpinit.cb                       = sizeof(HELPINIT);
    helpinit.ulReturnCode             = 0LU;
    helpinit.pszTutorialName          = (PSZ)NULL;
    helpinit.phtHelpTable      = (PHELPTABLE)MAKELONG(MAINHELPTABLE, 0xFFFF);
    helpinit.hmodAccelActionBarModule = NULLHANDLE;
    helpinit.idAccelTable             = 0;
    helpinit.idActionBar              = 0;
    helpinit.pszHelpWindowTitle       = "MultiMedia MCI Spy Program V1.0";
    helpinit.hmodHelpTableModule      = NULLHANDLE;
    helpinit.fShowPanelId             = CMIC_HIDE_PANEL_ID;
    helpinit.pszHelpLibraryName       = "MCISPY.HLP";

   // if((pSpyBlock->hwndHelpInstance = WinCreateHelpInstance(pSpyBlock->hab, &pSpyBlock->helpinit))==NULLHANDLE)
    if((pSpyBlock->hwndHelpInstance = WinCreateHelpInstance(WinQueryAnchorBlock(HWND_DESKTOP),  &helpinit))==NULLHANDLE)
     {
       mprintf ("Help Creation Error\n");
     }
    else
     if (helpinit.ulReturnCode)
      {
        mprintf ("Help Creation Error\n");
        WinDestroyHelpInstance(pSpyBlock->hwndHelpInstance);
        pSpyBlock->hwndHelpInstance = 0;
      }

     /* Associate Help */
     if (pSpyBlock->hwndHelpInstance )
        WinAssociateHelpInstance( pSpyBlock->hwndHelpInstance, pSpyBlock->hwnd1 );

     DosRequestMutexSem (pSpyBlock->hmtxPrntSem, SEM_INDEFINITE_WAIT);
    /* Register Our App with MDM for Spying MCI Messages */
    RegisterSpyWithMDM(pSpyBlock);

    /* Query Open Instance information */

    pSpyBlock->QueryOpenInstance ((OPEN_LIST *)&pSpyBlock->OpenList,TRUE);
    pSpyBlock->QueryOpenInstance ((OPEN_LIST *)&pSpyBlock->OpenList,FALSE);
    /*
    ** Give up the Print Semaphore
    */
    DosReleaseMutexSem (pSpyBlock->hmtxPrntSem);

}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    QueryInstalledDevices
*
* ARGUMENTS:
*
* RETURN:
*
* DESCRIPTION: To Query MMPM/2 and obtain list of all installed
*              audio devices and to initialize device specific info.
*
* OS/2 API's USED:  mciSendCommand() MMPM/2 Command Interface API
*
*
* C FUNCTION CALLS: memset()  malloc()
*                   strxxx()  free ()
*
**************************************************************************/
VOID QueryInstalledDevices (MCISPY_BLOCK * pSpyBlock)
{
    SZ                     pszAllDevNames[1024]; /* All MMPM/2 Device Names */
    SZ                     retbuffer[MAX_SYSINFO_RETURN];/* Return buffer      */
    CHAR                   tokensep[] = " ";     /* Token Seperators        */
    PCHAR                  pchWord;              /* Word pointer            */
    PSZ                    pszTemp ;             /* Temporary Storage       */
    USHORT                 i,j = 0;              /* Loop Indicies           */
    APIRET                 ReturnCode ;          /* API Return Code         */
    MCI_SYSINFO_PARMS      SysInfo ;             /* Generic Sys Info Struct */
    MCI_SYSINFO_QUERY_NAME SysInfoQName ;        /* Sys Info Qry Name Str   */
    MCI_SYSINFO_LOGDEVICE  SysInfoLogDev;        /* Sys Info Log Dev  Str   */

    /* Obtain The Serialization Semaphore */

    DosRequestMutexSem (pSpyBlock->hmtxPrntSem, SEM_INDEFINITE_WAIT);

    /* Initialize the SysInfo structures */
    memset (retbuffer,0,sizeof(retbuffer));
    memset (pszAllDevNames,0,sizeof(pszAllDevNames));
    memset ((PCH)&SysInfo,0,sizeof(MCI_SYSINFO_PARMS));
    memset ((PCH)&SysInfoQName,0,sizeof(MCI_SYSINFO_QUERY_NAME));
    memset ((PCH)&SysInfoLogDev,0,sizeof(MCI_SYSINFO_LOGDEVICE));

    SysInfo.pszReturn = (PSZ)retbuffer;
    SysInfo.ulRetSize = MAX_SYSINFO_RETURN;

    /* First obtain the number of devices installed */

    ReturnCode = mciSendCommand ((USHORT) MCI_ALL_DEVICE_ID,
                                 (USHORT) MCI_SYSINFO,
                                 (ULONG)  MCI_SYSINFO_QUANTITY | MCI_WAIT,
                                 (PVOID)  &SysInfo,
                                 (USHORT) NULL );

    /* Allocate memory for return information */

    if (!ReturnCode)
     {
        pSpyBlock->usNumDevices = (USHORT) atoi (retbuffer);
     }
    /* Obtain the names of devices installed */

    memset (&SysInfo,0,sizeof(MCI_SYSINFO_PARMS));
    SysInfo.pszReturn = (PSZ)pszAllDevNames;
    SysInfo.ulRetSize = 1024 ;

    ReturnCode = mciSendCommand ((USHORT) MCI_ALL_DEVICE_ID,
                                 (USHORT) MCI_SYSINFO,
                                 (ULONG)  MCI_SYSINFO_NAME | MCI_WAIT,
                                 (PVOID)  &SysInfo,
                                 (USHORT) NULL );

    if (!LOUSHORT(ReturnCode))
     {
       pchWord = strtok (pszAllDevNames,tokensep);

       for (i = 0; i < pSpyBlock->usNumDevices ; i++)
         {
          if (pchWord == NULL)
              break;

          memset ((PCH)&SysInfo,0,sizeof(MCI_SYSINFO_PARMS));
          memset ((PCH)&SysInfoQName,0,sizeof(MCI_SYSINFO_QUERY_NAME));

          strcpy (SysInfoQName.szLogicalName,pchWord);

          /* Get the logical name */

          SysInfo.ulItem = MCI_SYSINFO_QUERY_NAMES ;
          SysInfo.pSysInfoParm = &SysInfoQName ;
          ReturnCode = mciSendCommand ((USHORT) NULL,
                                       (USHORT) MCI_SYSINFO,
                                       (ULONG)  MCI_SYSINFO_ITEM | MCI_WAIT,
                                       (PVOID)  &SysInfo,
                                       (USHORT) NULL );

          if (LOUSHORT(ReturnCode))
            break;

          strcpy(pSpyBlock->PhysicalDevData[j].szLogDevName,
                 SysInfoQName.szLogicalName);


          /*
          ** Now Set Up Product Information and PDD Names
          ** of associated devices.
          */

           memset (&SysInfoLogDev,0,sizeof(MCI_SYSINFO_LOGDEVICE));
           SysInfo.ulItem = MCI_SYSINFO_QUERY_DRIVER ;
           SysInfo.pSysInfoParm = &SysInfoLogDev ;

           strcpy (SysInfoLogDev.szInstallName,
                   SysInfoQName.szInstallName);

           ReturnCode = mciSendCommand((USHORT) NULL,
                                       (USHORT) MCI_SYSINFO,
                                       (ULONG)  MCI_SYSINFO_ITEM | MCI_WAIT,
                                       (PVOID)  &SysInfo,
                                       (USHORT) NULL );


          if (LOUSHORT(ReturnCode))
            break;

           if (SysInfoLogDev.szPDDName !="")
               strcpy(pSpyBlock->PhysicalDevData[j].szPhysicalDevName,
                      SysInfoLogDev.szPDDName);

           if (SysInfoLogDev.szProductInfo !="")
               strcpy(pSpyBlock->PhysicalDevData[j].szProductInfo,
                  SysInfoLogDev.szProductInfo);

          j++;

         pchWord = strtok (NULL,tokensep); /* Get the Next Token */
         } /* For Loop */
     } /* No Return Code */

     /*
     ** Give up the Print Semaphore
     */
     DosReleaseMutexSem (pSpyBlock->hmtxPrntSem);
}

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION      Terminate
*
* ARGUMENTS:    None.
*
* RETURN:       None.
*
* DESCRIPTION: Terminate the application and release resourves.
*
* OS/2 API's USED: WinDestroyWindow()
*                  WinDestroyHelpInstance()
*                  WinDestroyMsgQueue()
*                  WinTerminate()
*
* C FUNCTION CALLS:
*
* INTERNAL FUNCTION REFERENCES: None.
*
* GLOBAL VARIABLES REFERENCED: hab,hwnd1,hmq,
*                              hwndHelpInstance.
*
* GLOBAL VARIABLES MODIFIED: hab,hwnd1,hmq,
*                            hwndHelpInstance.
*
*****************************END OF SPECIFICATIONS ***************************/
VOID Terminate (HMQ hmq)
{
  int i;

  /* DeRegister Spy with MDM */
  RegEntryProc ((HWND)SpyBlock.hwnd1,FALSE);

  /* Close the current Log File */
  DosClose (SpyBlock.hfileLog);

  /* Destroy the window */
  WinDestroyWindow(SpyBlock.hwnd1);

  /* Destroy the Help instance */
  if (SpyBlock.hwndHelpInstance)
      WinDestroyHelpInstance (SpyBlock.hwndHelpInstance);


  /* Destroy the message queue */
  WinDestroyMsgQueue(SpyBlock.hmq );

  /* Terminate and release resources */
  WinTerminate( SpyBlock.hab );

}



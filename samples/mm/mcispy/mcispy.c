/*********************** START OF SPECIFICATIONS *******************************
*
* SOURCE FILE NAME: MCISPY.C
*
* DESCRIPTIVE NAME: Multimedia MCI Message Spy
*
*              Copyright (c) IBM Corporation   1993
*                        All Rights Reserved
*
* FUNCTION: Monitor MCI Messages.
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
*
* INSTALLATION:
*              In order to begin monitoring MCI messages the existing
*              MDM.DLL must be renamed to MCI.DLL. To do this use a tool
*              called DLLRNAME. DLLRNAME is provided as part of IBM
*              CSET/2 compiler. ex DLLRNAME d:\mmos2\dll\mdm.dll mdm=mci
*              is the command you would actually type if the true MDM.DLL
*              resides on drive D:.
*              Then Copy the stub MDM.DLL (this source product) to d:\mmos2
*              \DLL\MDM.DLL.
*              If MDM.DLL cannot be renamed or replaced then the DLL may be
*              in use because of system sounds. Temporarily Deinstall System
*              sounds by running \mmos2\install\DINSTSND.CMD. Yoy may have
*              to reboot after this. After the reboot you can replace the
*              MDM.DLL.
*
* OS/2 APIs Used:
*
*           DosRequestMutexSem ()
*           DosReleaseMutexSem ()
*           DosCreateMutexSem  ()
*           DosClose ()
*           WinInitialize()
*           WinCreateMsgQueue()
*           WinRegisterClass()
*           WinCreateStdWindow()
*           WinAssociateHelpInstance()
*           WinGetMsg()
*           WinDispatchMsg()
*           WinSendMsg ()
*           WinWindowFromID()
*           WinQueryWindow()
*           WinGetScreenPS()
*           WinFontDlg ()
*           WinDlgBox ()
*           WinQueryWindowRect ()
*           WinCreateWindow()
*           WinSetWindowPos()
*           WinBeginPaint()
*           WinFillRect()
*           WinEndPaint()
*           WinDefWindowProc()
*
*
* WHAT YOU NEED TO COMPILE THIS PROGRAM:
* --------------------------------------
*
* REQUIRED FILES:
* ---------------
*
*   OS2.H          - Presentation Manager include file
*   STDLIB.H       - Standard library function declarations
*   OS2ME.H        - MultiMedia System include
*
* REQUIRED LIBRARIES:
* -------------------
*
*   OS2386.LIB     - OS/2 API Library
*   MMPM2.LIB      - MMPM/2 API Library
*
* REQUIRED PROGRAMS:
* ------------------
*
*   IBM C Set/2 Compiler (icc.exe    )
*   IBM Linker           (link386.exe)
*   Resource Compiler    (rc.exe     )
*   Help Compiler        (ipfc.exe   )
*
*********************** END OF SPECIFICATIONS ********************************/
#define INCL_DOS                       /* DosCall Definitions include     */
#define INCL_PM                        /* Presentation Manager include    */
#define INCL_WIN                       /* OS/2 2.x Win  include           */
#define INCL_GPI                       /* OS/2 2.x Win  include           */
#define INCL_OS2MM                     /* OS/2 2.x MM include             */
#include <os2.h>                       /* OS/2 2.x System include         */
#include <stdio.h>                     /* C Standard library include      */
#include <stdlib.h>                    /* C Standard library include      */
#include <string.h>                    /* C String Functions include      */
#include <stdarg.h>                    /* C Standard Arguments include    */
#include <os2me.h>                     /* Multimedia System include       */
#include "mcispy.h"                    /* Component Private Definitions   */

MCISPY_BLOCK    SpyBlock;               /* SPY Application Data struct    */
CHAR            szMainWin[]="MainWin";  /* Main window class title        */
CHAR            szTitle[]="MultiMedia MCI Message Spy V1.0"; /* Application title */
USHORT          cxChar,cyChar;          /* Character dimensions */
extern          MCIMessageTable   MCIMsgTbl[];/* Table of MCI Messages     */

/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION:    main
*
* ARGUMENTS:  argc, argv, envp.
*
* RETURN:     None.
*
* DESCRIPTION: MCISPY Application Main
*
* OS/2 API's USED:
*
*****************************END OF SPECIFICATIONS ***************************/
INT main(LONG argc,PSZ argv[])
{
   static ULONG flCreate;      /* Window Creation Flags        */
   APIRET rc;                  /* Return Code                  */

   /* initialize global variables */

   memset (&SpyBlock,0,sizeof(MCISPY_BLOCK));
  /* clear the fontmetrics structure */
   memset (&SpyBlock.fm,0,sizeof(FONTMETRICS));


   sprintf (SpyBlock.szTitle,"%s",
   "PID    DeviceID     MCI Message                 Flags       UserParm      Data");

   /* Intialize PM */
   SpyBlock.hab = WinInitialize((USHORT) NULL);

   /* Create default size message queue */
   SpyBlock.hmq = WinCreateMsgQueue(SpyBlock.hab,0);

   WinRegisterClass( SpyBlock.hab,szMainWin,
                     (PFNWP) SpyWinProc,CS_SIZEREDRAW|CS_CLIPCHILDREN |
                     CS_MOVENOTIFY | CS_PARENTCLIP,
                     0 );
   /* Set main window creation flags */

   flCreate = FCF_SIZEBORDER | FCF_MENU  | FCF_MINMAX |
              FCF_SYSMENU | FCF_TITLEBAR | FCF_TASKLIST |
              FCF_ICON;

   /* Create the main window */

   SpyBlock.hwnd1 = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE, &flCreate,
                                szMainWin, szTitle, 0L, NULLHANDLE,
                                WND_FRM, (PHWND) & SpyBlock.hwndc1  );

   WinSetWindowPos( SpyBlock.hwnd1,
                    HWND_TOP,
                    100, 100, 500, 250,
                    SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW );

   if (!SpyBlock.hwnd1)
    {
       mprintf ("Main Window Creation Error");
       DosExit (0,0);
    }

   /* Initialize Global Data Structures and Register Spy */

    Initialize(&SpyBlock);
    /* Message Loop */

    while ( WinGetMsg(SpyBlock.hab,(PQMSG) &SpyBlock.qmsg,(HWND) NULL,0,0))
       WinDispatchMsg(SpyBlock.hab,(PQMSG) &SpyBlock.qmsg );

    Terminate (SpyBlock.hmq);  /* Terminate Application */
    return(0);
}  /* of main */


/****************************START OF SPECIFICATIONS *************************
*
* FUNCTION: SpyWinProc()
*
* ARGUMENTS: Standard OS/2 PM Win Proc arguments
*
* RETURN:
*
* DESCRIPTION: This is the main window that interfaces with the user.
* it routes user choices to various internal routines to perform
* specific data streaming tasks.
*
* NOTES:
*
* SIDE EFFECTS:
*
*****************************END OF SPECIFICATIONS ***************************/
MRESULT EXPENTRY SpyWinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   RECTL           rectl,rectl1;      /* Client rectangles       */
   int             i ;                /* loop index              */
   APIRET          RetCode;           /* API return code         */
   THREAD_BLOCK    *pNotPacket;       /* IPC Notification packet */
   HWND            hwndFontDlg;       /* Font Dialog handle      */
   POINTL          pt;                /* GPI static text x,y     */
   BOOL            fReply ;           /* Internal Flag           */
   BOOL            fFoundDevTyp ;     /* Internal Flag           */
   BOOL            fGainorLose ;      /* Internal Flag           */
   static HPS      hps;               /* Presentation Space      */
   char            buffer[2048];      /* MLE buffer              */
   char            buffer2[1024];      /* MSG buffer              */
   static FONTDLG  FontDlg;           /* Font Dialog Structure   */
   PCHAR           pchAlias;          /* String Alias            */
   char            tokensep[]=" \t";  /* Token Seperators        */
   SZ              szDevTyp[30];      /* Device Type             */
   char            szSpace[11]= "    ";
   char            szSpace2[50]="                                                ";

   switch( msg )
     {
     case WM_CONTROL:
       if (SHORT2FROMMP(mp1) == MLN_TEXTOVERFLOW)
        {
           SpyBlock.ipt = 0;
           WinSendMsg (SpyBlock.hwndMLE,MLM_DELETE,MPFROMLONG(SpyBlock.ipt),
                       MPFROMLONG(65536));
           return (MRESULT)(TRUE);
        }
       break;

     case WM_COMMAND:
       switch ( SHORT1FROMMP( mp1 ) )
         {
         /* Recieved MCI Command through mciSendCommand() API.*/

         case SPY_NOTIFY:
           /*
           ** If Driver Notifications are disabled then just return
           ** without writing it to MLE.
           */
           if (SpyBlock.fNoCmd)
              break;

           /*
           ** Obtain the Serialization Semaphore.
           */
           DosRequestMutexSem (SpyBlock.hmtxPrntSem, SEM_INDEFINITE_WAIT);

           /* Obtain the Shared Memory Packet */
           GetSharedMemory(&SpyBlock);

           /* Display the MCI Message and other Info */
           DisplayMCIMsg(&SpyBlock);

           /*
           ** Give up the Serialization Semaphore.
           */
           DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
           return (MRESULT)(FALSE);
         break;

         /* Recieved String command through mciSendString() API.*/

         case SPY_STRING:
           /*
           ** If String Commands are disabled then just return
           ** without writing it to MLE.
           */
           if (SpyBlock.fNoStr)
              break;
           /*
           ** Obtain the Serialization Semaphore.
           */
           DosRequestMutexSem (SpyBlock.hmtxPrntSem, SEM_INDEFINITE_WAIT);

           /* Obtain the Shared Memory Packet */
           GetSharedMemory (&SpyBlock);

           /* Derefence the packet to a local pointer */
           pNotPacket = (THREAD_BLOCK *)SpyBlock.pvPacket;

           if (SpyBlock.fDevTypFil)
            {
               /* Update open instance information */

               SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,TRUE);

               strcpy (buffer,pNotPacket->szCmd);
               pchAlias = strtok (buffer,tokensep);

               /* Second Token Should be the Alias on the string interface */

               pchAlias = strtok (NULL,tokensep);

               /* In order to implement device type filtering we maintain
               ** a list of all open device ids and aliases together with
               ** associated devices. We look at the device type for this
               ** alias and compare it with the global device type filter
               ** in effect. If they match then we know that this device
               ** is on the filter list.
               */
               for (i=0; i < MAXOPENS; i++)
                 {
                    if (strcmpi (SpyBlock.OpenList.szAlias[i].szAlias,pchAlias) == 0)
                     {
                        strcpy (szDevTyp,SpyBlock.OpenList.szAlias[i].szDevTyp);
                        fFoundDevTyp = TRUE;
                       break;
                     } /* Found Alias */
                 } /* Num Open Devices */
               if (fFoundDevTyp)
                {
                   for (i=0; i < SpyBlock.usNumFilDevs; i++)
                     {
                        if (strnicmp(szDevTyp,SpyBlock.szDevTypFil[i],
                                      strlen (SpyBlock.szDevTypFil[i])) == 0)
                         {
                           /*
                           ** Give up the Serialization Semaphore.
                           */
                           DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
                           return (MRESULT)(0);
                         } /* Device Type on Filter list */
                     } /* Max Devices */
                } /* Found Device Type */
            } /* Device Type Filter in Effect */

           if (SpyBlock.fDevIdFil)
            {
               /* Update open instance information */

               SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,FALSE);

               strcpy (buffer,pNotPacket->szCmd);

               pchAlias = strtok (buffer,tokensep);

               /* Second Token Should be the Alias on the string interface */

               pchAlias = strtok (NULL,tokensep);

               for (i=0; i < MAXOPENS; i++)
                 {
                    if (stricmp (SpyBlock.OpenList.szAlias[i].szAlias,pchAlias) == 0)
                     {
                       /* There may be Multiple Aliases Across different PIDs */

                       if (pNotPacket->ulPID == SpyBlock.OpenList.szAlias[i].ulPID)
                        {
                           if (SpyBlock.OpenList.szAlias[i].fExclude)
                            {
                              /*
                              ** Give up the Serialization Semaphore.
                              */
                              DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
                              return (MRESULT)(0);
                            } /* Exclude was on */
                        } /* PIDs match on Alias */
                     }  /* Found the Alias */
                 } /* Number of Open Instances */
            } /* Device Id Filter in Use */

           /* Format the String */
           sprintf (buffer,  "%03d %s %s %s %04d\n", pNotPacket->ulPID, szSpace,
                              (char *)pNotPacket->szCmd,szSpace,pNotPacket->wUserParm);
           /* Write to MLE */

          WinSendMsg (SpyBlock.hwndMLE,MLM_SETSEL,MPFROMLONG(SpyBlock.ipt),
                      MPFROMLONG(SpyBlock.ipt));

          WinSendMsg (SpyBlock.hwndMLE,MLM_INSERT,MPFROMLONG(buffer),
                     (MPARAM)0);

          SpyBlock.ipt= (IPT)WinSendMsg (SpyBlock.hwndMLE,MLM_QUERYSEL,MPFROMSHORT(MLFQS_CURSORSEL),
                                         0L);
          if (SpyBlock.fNoLog)
              Write2File (SpyBlock.hfileLog,buffer);

           /*
           ** Give up the Serialization Semaphore.
           */
           DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
           return (FALSE);
         break;

          /*
          ** Recieved Device Notifications through our Input hook.
          */
         case SPY_DEVICE:
           /*
           ** Obtain the Serialization Semaphore.
           */
           DosRequestMutexSem (SpyBlock.hmtxPrntSem, SEM_INDEFINITE_WAIT);

           if (MCIMsgTbl[56].fExclude)
            {
               /*
               ** Give up the Print Semaphore
               */
               DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
               return (MRESULT)(FALSE);
            }
           /* Obtain the Shared Memory Packet */

           GetSharedMemory (&SpyBlock);

           pNotPacket = (THREAD_BLOCK *)SpyBlock.pvPacket;

           strcpy (buffer2,"  MM_MCIPASSDEVICE");
           if (pNotPacket->dwParam2 == MCI_LOSING_USE)
            {
               strcat (buffer2,"  Losing Use");
               fGainorLose = TRUE;
            }
           else
            {
              strcat (buffer2,"  Gaining Use");
              fGainorLose = TRUE;
            }
           if (fGainorLose)
            {
               sprintf (buffer,"%03d %s %02d %s %s\n",pNotPacket->ulPID,szSpace,
                               pNotPacket->wDevID,szSpace, buffer2);
               WinSendMsg (SpyBlock.hwndMLE,MLM_SETSEL,MPFROMLONG(SpyBlock.ipt),
                          MPFROMLONG(SpyBlock.ipt));

               WinSendMsg (SpyBlock.hwndMLE,MLM_INSERT,MPFROMLONG(buffer),
                         (MPARAM)0);

               SpyBlock.ipt= (IPT)WinSendMsg (SpyBlock.hwndMLE,MLM_QUERYSEL,MPFROMSHORT(MLFQS_CURSORSEL),
                                             0L);
            }
           /*
           ** Give up the Print Semaphore
           */
           DosReleaseMutexSem (SpyBlock.hmtxPrntSem);

           return (MRESULT)(FALSE);
         break;

         /* Recieved an Error Notification.*/
         case SPY_ERRRC:
           DosRequestMutexSem (SpyBlock.hmtxPrntSem, SEM_INDEFINITE_WAIT);
           GetSharedMemory (&SpyBlock);
           pNotPacket = (THREAD_BLOCK *)SpyBlock.pvPacket;
           sprintf (buffer,  "%03d %s %02d %s %s  %s %04d\n",pNotPacket->ulPID,szSpace,
                    pNotPacket->wDevID,szSpace,"Error: Return Code = ",szSpace,
                    pNotPacket->ulReturnCode);

           WinSendMsg (SpyBlock.hwndMLE,MLM_SETSEL,MPFROMLONG(SpyBlock.ipt),
                      MPFROMLONG(SpyBlock.ipt));

           WinSendMsg (SpyBlock.hwndMLE,MLM_INSERT,MPFROMLONG(buffer),
                       (MPARAM)0);

           SpyBlock.ipt= (IPT)WinSendMsg (SpyBlock.hwndMLE,MLM_QUERYSEL,MPFROMSHORT(MLFQS_CURSORSEL),
                                         0L);

          if (SpyBlock.fNoLog)
              Write2File (SpyBlock.hfileLog,buffer);

           /*
           ** Give up the Print Semaphore
           */
           DosReleaseMutexSem (SpyBlock.hmtxPrntSem);
           return (FALSE);
         break;
         /* File open */
         case IDM_OPEN:
           RetCode = OpenFileDlg (hwnd,"MCISPY: Open Log File",
                                 SpyBlock.szLogFile,"*.LOG");

           /* Close the Default LogFile */
           DosClose (SpyBlock.hfileLog);

           OpenFile (SpyBlock.szLogFile,&SpyBlock.hfileLog);
         break;

         /* Enable Logging (Toggle) */

         case IDM_LOG:
           fReply = (BOOL)WinSendMsg(WinWindowFromID(
                      WinQueryWindow(hwnd,QW_PARENT),
                      FID_MENU),
                      MM_QUERYITEMATTR,
                      MPFROM2SHORT(IDM_LOG,TRUE), /* item to check */
                      MPFROMLONG(MIA_CHECKED) );

           if (fReply)
            {
               /* Uncheck the item */
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_LOG,TRUE), /* item to uncheck */
                          MPFROM2SHORT(MIA_CHECKED,0) );

               SpyBlock.fNoLog = FALSE;
            }
            else
            {
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_LOG,TRUE), /* item to check */
                          MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) );

               SpyBlock.fNoLog = TRUE;
            }
         break;

         /* Enable System Sounds (Toggle) */

         case IDM_NOSYS:
           fReply = (BOOL)WinSendMsg(WinWindowFromID(
                      WinQueryWindow(hwnd,QW_PARENT),
                      FID_MENU),
                      MM_QUERYITEMATTR,
                      MPFROM2SHORT(IDM_NOSYS,TRUE), /* item to check */
                      MPFROMLONG(MIA_CHECKED) );

           if (fReply)
            {
               /* Uncheck the item */
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOSYS,TRUE), /* item to uncheck */
                          MPFROM2SHORT(MIA_CHECKED,0) );

               SpyBlock.SystemSounds(TRUE);

            }
            else
            {
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOSYS,TRUE), /* item to check */
                          MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) );

               SpyBlock.SystemSounds(FALSE);

            }
         break;

         /* Clear MLE Contents    */

         case IDM_EDCLEAR:
           SpyBlock.ipt = 0;
           WinSendMsg (SpyBlock.hwndMLE,MLM_DELETE,MPFROMLONG(SpyBlock.ipt),
                       MPFROMLONG(65536));
         break;

         /* Copy MLE contents to clipboard */

         case IDM_EDCOPY:
           CopyMLE (SpyBlock.hwndMLE);
         break;

        /* Discard MLE Selected Text */

         case IDM_EDDISCARD:
           WinSendMsg (SpyBlock.hwndMLE,MLM_CLEAR,MPFROMLONG(0),
                       MPFROMLONG(0));

         break;

         /* Set MLE Font   */

         case IDM_EDFONT:
           memset (&FontDlg,0,sizeof(FONTDLG));
           memset (buffer,0,sizeof(buffer));
           FontDlg.cbSize = sizeof(FONTDLG);
           FontDlg.hpsScreen = WinGetScreenPS(HWND_DESKTOP);
           strcpy (buffer,"10.System Monospaced");

           /* Initialize The FontDlg members */

           FontDlg.pszFamilyname = buffer;
           FontDlg.usFamilyBufLen = FACESIZE;
           FontDlg.fl = FNTS_CENTER | FNTS_INITFROMFATTRS;
           FontDlg.clrFore = CLR_BLACK;
           FontDlg.clrBack = CLR_WHITE;
           FontDlg.fAttrs.usRecordLength = sizeof(FATTRS);
           FontDlg.fAttrs.lMatch = 3;
           FontDlg.fAttrs.fsSelection = FM_SEL_OUTLINE;
           FontDlg.fAttrs.lMaxBaselineExt = 20;
           FontDlg.fAttrs.lAveCharWidth=9;
           FontDlg.fAttrs.fsFontUse=2;
           hwndFontDlg = WinFontDlg (HWND_DESKTOP,hwnd,(PFONTDLG)&FontDlg);
           if ((hwndFontDlg) && (FontDlg.lReturn == DID_OK))
            {
               WinSendMsg (SpyBlock.hwndMLE,MLM_SETFONT,MPFROMP((PFATTRS)&FontDlg.fAttrs),
                           MPFROMSHORT(0));
            }
         break;

         /* View Installed Multimedia Devices */
         case IDM_IDEVS:
           /* Query Multimedia Devices installed in system */
           QueryInstalledDevices (&SpyBlock);

           WinDlgBox ( HWND_DESKTOP, hwnd,(PFNWP) ViewInstDevDlgProc, NULLHANDLE,
                       ID_DEVNAMESDLG, NULL );

         break;

         /* Filter by MCI messages */

         case IDM_FILMSG:
           WinDlgBox (HWND_DESKTOP,hwnd,(PFNWP) FilterMsgDlgProc,NULLHANDLE,
                       ID_FILTERMSG, NULL );
         break;

         /* Filter by Open Device Ids */

         case IDM_FILDEVID:
           if (SpyBlock.QueryOpenInstance == NULL)
            {
              mprintf ("Null QueryOpenInstance Pointer \n");
              break;
            }

           /* Query Open Instance information */
           SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,TRUE);
           SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,FALSE);

           if (SpyBlock.OpenList.ulNumOpen == 0)
            {
              mprintf ("No Open Multimedia Instances Detected \n");
              break;
            }
           WinDlgBox (HWND_DESKTOP, hwnd,(PFNWP) FilterDevIdDlgProc, NULLHANDLE,
                       ID_FILTERDEVIDS, NULL );
         break;

         /* Filter by Device Types  */

         case IDM_FILDEVTYP:
           /* Query Open Instance information */
           SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,TRUE);
           SpyBlock.QueryOpenInstance ((OPEN_LIST *)&SpyBlock.OpenList,FALSE);
           WinDlgBox ( HWND_DESKTOP, hwnd,(PFNWP) FilterDevTypDlgProc, NULLHANDLE,
                       ID_FILTERDEVTYP, NULL );
         break;

         /* View MCI Flags */

         case IDM_FLAGS:
           WinDlgBox ( HWND_DESKTOP, hwnd,(PFNWP) ShowFlagDlgProc, NULLHANDLE,
                       ID_FLAGSDLG, NULL );
         break;

         /* Disable mciSendString() Interface */

         case IDM_NOSTR:
           fReply = (BOOL)WinSendMsg(WinWindowFromID(
                      WinQueryWindow(hwnd,QW_PARENT),
                      FID_MENU),
                      MM_QUERYITEMATTR,
                      MPFROM2SHORT(IDM_NOSTR,TRUE), /* item to check */
                      MPFROMLONG(MIA_CHECKED) );

           if (fReply)
            {
               /* Uncheck the item */
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOSTR,TRUE), /* item to uncheck */
                          MPFROM2SHORT(MIA_CHECKED,0) );

               SpyBlock.fNoStr = FALSE;
            }
            else
            {
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOSTR,TRUE), /* item to check */
                          MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) );

               SpyBlock.fNoStr = TRUE;
            }
         break;

         /* Disable mciSendCommand() Interface */

         case IDM_NOCMD:

           fReply = (BOOL)WinSendMsg(WinWindowFromID(
                      WinQueryWindow(hwnd,QW_PARENT),
                      FID_MENU),
                      MM_QUERYITEMATTR,
                      MPFROM2SHORT(IDM_NOCMD,TRUE), /* item to check */
                      MPFROMLONG(MIA_CHECKED) );

           if (fReply)
            {
               /* Uncheck the item */
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOCMD,TRUE), /* item to uncheck */
                          MPFROM2SHORT(MIA_CHECKED,0) );

               SpyBlock.fNoCmd = FALSE;
            }
            else
            {
               WinSendMsg(WinWindowFromID(
                          WinQueryWindow(hwnd,QW_PARENT),
                          FID_MENU),
                          MM_SETITEMATTR,
                          MPFROM2SHORT(IDM_NOCMD,TRUE), /* item to check */
                          MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) );

               SpyBlock.fNoCmd = TRUE;
            }

         break;

         case IDM_HELP_INDEX:
           if ( SpyBlock.hwndHelpInstance )
             WinSendMsg(SpyBlock.hwndHelpInstance, HM_HELP_INDEX, MPVOID, MPVOID);
         break;

         case IDM_HELP_GEN:
           if ( SpyBlock.hwndHelpInstance )
               WinSendMsg (SpyBlock.hwndHelpInstance,HM_DISPLAY_HELP,
                           MPFROM2SHORT(GENHLP, NULL),
                           MPFROMSHORT(HM_RESOURCEID));

         break;

         case IDM_HELP_KEYS:
           if ( SpyBlock.hwndHelpInstance )
               WinSendMsg (SpyBlock.hwndHelpInstance,HM_DISPLAY_HELP,
                           MPFROM2SHORT(KEYSHELP, NULL),
                           MPFROMSHORT(HM_RESOURCEID));
         break;

         case IDM_HELP_USING:
           if ( SpyBlock.hwndHelpInstance )
             WinSendMsg(SpyBlock.hwndHelpInstance, HM_DISPLAY_HELP, MPVOID, MPVOID);
         break;

         case IDM_HELP_PRODINFO:
           WinDlgBox ( HWND_DESKTOP, hwnd,(PFNWP) AboutDlgProc, NULLHANDLE,
                       IDM_PRODUCTINFO, NULL );
         break;

         } /* WM_COMMAND */
        break;
     case WM_HELP:
       if ( SpyBlock.hwndHelpInstance )
           WinSendMsg (SpyBlock.hwndHelpInstance,HM_DISPLAY_HELP,
                       MPFROM2SHORT(STDHLP, NULL),
                       MPFROMSHORT(HM_RESOURCEID));

         return (MRESULT)0;
       break;

     case WM_CLOSE:
       /******************************************************************/
       /* This is the place to put your termination routines             */
       /******************************************************************/
       WinPostMsg( hwnd, WM_QUIT, 0L, 0L );  /* Cause termination        */
       break;

     case WM_CREATE:
       WinQueryWindowRect (hwnd, &rectl);

        /* Get Character dimensions */
       SpyBlock.hps = WinGetPS (hwnd);
       GpiQueryFontMetrics (SpyBlock.hps,(LONG)sizeof(SpyBlock.fm),&SpyBlock.fm);

       /* Get Character dimensions   */

       cxChar = (SHORT)SpyBlock.fm.lAveCharWidth;
       cyChar = (SHORT)SpyBlock.fm.lMaxBaselineExt;

       /*
       ** Create the MLE and set it up as a
       ** client of the main window
       */
       SpyBlock.hwndMLE = WinCreateWindow( hwnd, WC_MLE, "",
                                    WS_VISIBLE | MLS_BORDER | MLS_VSCROLL |
                                    MLS_HSCROLL| MLS_READONLY,
                                    0, 0,
                                    (SHORT)rectl.xRight,
                                    (SHORT)rectl.yTop - 30 ,
                                    hwnd, HWND_BOTTOM, FID_CLIENT, 0L, NULL );

       /* Set the default font to system monospaced */

       strcpy (buffer2,"10.System Monospaced");

       /* Client MLE Font */
       WinSetPresParam(SpyBlock.hwndMLE,PP_FONTNAMESIZE,
                       (strlen(buffer2)+(ULONG)1),
                       (PSZ) buffer2);

       /* main Window Font */
       WinSetPresParam(hwnd,PP_FONTNAMESIZE,
                       (strlen(buffer2)+(ULONG)1),
                       (PSZ) buffer2);

       /* Create Print Active Information mutual exclusion semaphore */

       DosCreateMutexSem (NULL,(PHEV) &SpyBlock.hmtxPrntSem,DC_SEM_SHARED,
                         FALSE);
       /*
       ** Set up MLE import export transaction format
       */
       SetMLEFormat(SpyBlock.hwndMLE);
        break;

     case WM_SIZE:

       WinQueryWindowRect ( hwnd, &rectl );

      /* Set the window position  of the frames making them visible */

       WinSetWindowPos(SpyBlock.hwndMLE, HWND_TOP,(SHORT)0, (SHORT)0,
                       (SHORT)rectl.xRight, (SHORT)rectl.yTop - 30 ,
                       SWP_MOVE|SWP_SIZE | SWP_ACTIVATE |SWP_SHOW );

        break;

     case WM_PAINT:
       hps = WinBeginPaint( hwnd, (HPS) NULL, (PRECTL) &rectl );
       WinFillRect( hps, (PRECTL) &rectl, SYSCLR_WINDOW );
       WinQueryWindowRect (hwnd, &rectl1);
       pt .x = rectl1.xLeft;
       pt.y =  (rectl1.yTop - cyChar);
       /* Set color of the text */
       GpiSetColor( hps, CLR_BLUE );
       /* Format the Title columns */
       GpiCharStringAt(hps, &pt, strlen(SpyBlock.szTitle)+1, SpyBlock.szTitle);
       WinEndPaint( hps );
        break;

     default:
        return(WinDefWindowProc(hwnd,msg,mp1,mp2));
     }
  return(FALSE);
}





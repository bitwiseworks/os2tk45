/************ GRAPHIC C Sample Program Source Code File (.C) *****************
 *
 * PROGRAM NAME: GRAPHIC
 * -------------
 *  Presentation Manager Non-Retained Graphics C Sample Program
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  This program displays a selected metafile.
 *  The current contents of the client area can also be printed.
 *  All drawing is performed by a lower-priority asynchronous
 *  thread, thus allowing the program to monitor its message queue
 *  at regular intervals. The program does not test every call for
 *  an error return code.
 *
 * WHAT THIS PROGRAM DEMONSTRATES:
 * -------------------------------
 *  This program demonstrates the use of the default viewing
 *  transformation, metafile loading and playing, the use of
 *  dialog boxes, and file manipulation through Standard Open dialog.
 *  It also demonstrates the use of an asynchronous drawing thread and
 *  print thread.
 *
 * API CALLS FEATURED:
 * -------------------
 *
 *  WinAddSwitchEntry                     DosReleaseMutexSem
 *  WinAssociateHelpInstance              DosCloseMutexSem
 *  WinBeginPaint                         DosCloseEventSem
 *  WinCreateStdWindow                    DosWaitThread
 *  WinCreationFlags
 *  WinCreateHelpInstance                 DosResetEventSem
 *  WinCreateMsgQueue
 *  WinCreateWindow                       DosCreateThread
 *  WinDlgBox                             DosGetInfoBlocks
 *  WinDefFileDlgProc                     DosCreateMutexSem
 *  WinDestroyHelpInstance                DosCreateEventSem
 *  WinDestroyMsgQueue
 *  WinDestroyWindow                      GpiCharStringAt
 *  WinDispatchMsg                        GpiSetColor
 *  WinDefDlgProc                         GpiQueryFontMetrics
 *  WinDefWindowProc                      GpiCreateRegion
 *  WinDismissDlg                         GpiCreatePS
 *  WinEndPaint                           GpiQueryCharBox
 *  WinFillRect                           GpiAssociate
 *  WinGetPS                              GpiDestroyPS
 *  WinGetMsg                             GpiDestroyRegion
 *  WinInitialize                         GpiLoadMetaFile
 *  WinInvalidateRegion                   GpiResetPS
 *  WinInvalidateRect                     GpiSetStopDraw
 *  WinLoadString
 *  WinMessageBox
 *  WinOpenWindowDC
 *  WinPostMsg
 *  WinPostQueueMsg
 *  WinQuerySysValue
 *  WinQuerySysPointer
 *  WinQueryWindowDC
 *  WinQueryWindowPos
 *  WinQueryWindowProcess
 *  WinQueryWindowPtr
 *  WinQueryWindowRect
 *  WinRegisterClass
 *  WinReleasePS
 *  WinRequestMutexSem
 *  WinSendDlgItemMsg
 *  WinSendMsg
 *  WinSetWindowPos
 *  WinSetWindowPtr
 *  WinSetWindowText
 *  WinTerminate
 *  WinWaitEventSem
 *  WinWindowFromID
 *
 * WHAT YOU NEED TO COMPILE THIS PROGRAM:
 * --------------------------------------
 *
 *  REQUIRED FILES:
 *  ---------------
 *
 *    GRAPHIC.C     - Source code
 *    FILE.C        - Source code (File Handling)
 *    GRAPHIC.MAK   - Make file for this program
 *    GRAPHIC.DEF   - Module definition file
 *    GRAPHIC.H     - Application header file
 *    GRAPHIC.ICO   - Icon file
 *    GRAPHIC.L     - Linker automatic response file
 *    GRAPHIC.RC    - Resource file
 *
 *    OS2.H          - Presentation Manager include file
 *    STDLIB.H       - Miscellaneous function declarations
 *    STRING.H       - String function declarations
 *
 * EXPECTED INPUT:
 * ---------------
 *  User activity with the mouse or the keyboard, the standard
 *  frame controls, menus, and dialog boxes.
 *
 * EXPECTED OUTPUT:
 * ----------------
 *  Display of the center part of a metafile picture in a standard
 *  window. This has a frame with title bar, system and application
 *  menus, and maximize and minimize icons.
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
 ******************************************************************************/

/* Include the required sections of the PM header file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_GPICONTROL
#define INCL_GPIMETAFILES
#define INCL_GPIREGIONS
#define INCL_GPIPRIMITIVES
#define INCL_GPILCIDS
#define INCL_WINSTDFILE
#define INCL_WINSTDDLGS
#define INCL_WINHELP
#define INCL_WINPOINTERS
#define INCL_WINFRAMEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WINDIALOGS
#define INCL_WINSYS
#define INCL_WINMENUS
#define INCL_WINMLE
#define INCL_WINERRORS

#include <os2.h>
#include <pmstddlg.h>
#include <ctype.h>
#include <process.h>
#include <memory.h>
#include "graphic.h"

/*
 * Presentation Manager Program Main Body
 *
 * The following routine is the Presentation Manager program Main Body.
 * The Main Body of a PM program is concerned with associating the
 * application with the Presentation Manager system, creating its
 * message queue, registering and displaying its main window, servicing
 * its message queue during the time that the application is active,
 * and disassociating the application from PM when the user is finished
 * with the application. The remaining parts of this source module that
 * are concerned with the Presentation Manager are the application's
 * window procedures (main window procedure, child window procedures,
 * and dialog window procedures) that process the messages associated
 * with the application's various windows.
 */

/* static function definitions */
static   VOID       InvalidateTextRect(HWND hWnd);
static   MAIN_PARM  Parm;
static   PAPPL_DEFAULTS ApplDefaults;

/**************************************************************************
 * Function: main
 *
 * Initializes the structures and handle the messages for the main window
 *
 * Parameters: none
 *
 * Result: none
 *
 ***************************************************************************/
VOID main(VOID)
{
         QMSG       qmsg;           /* MSG structure to store the messages */
         BOOL       fRc;
         BOOL       FastQuit = FALSE;

/* Do the Initialization */
   if (!Initialize())
   {
      FastQuit = TRUE;
      ReportError();
      EndProgram();
   }


/* The following is the message loop for the application. */

   do
   {
      fRc = WinGetMsg(Parm.hAB, (PQMSG)&qmsg, 0, 0, 0);
      if (fRc)                   /* if not WM_QUIT, pass the message to the   */
         WinDispatchMsg(Parm.hAB, (PQMSG)&qmsg);     /* window procedure     */
   } while (fRc);

/* if printing is in process, cancel print job */
   if (Parm.PrintStatus)
   {
      if (GpiSetStopDraw(Parm.hpsPrint, (LONG)TRUE) == GPI_ERROR)
         DispError(Parm.hAB,Parm.hWndClient);
   }

   EndProgram();
   exit(0);
} /* End of main */

/**************************************************************************
 * Function: Initialize
 *
 * Initializing the structures and handle the messages for the main window
 *
 * Parameters: none
 *
 * Result:  BOOL              TRUE      Initialization ok
 *                            FALSE     Error in Initialization
 *
 ***************************************************************************/
BOOL Initialize(VOID)
{
         CHAR       szAppName[20];            /* class name of application */
static   CHAR       InfoWindow[] = "GRAPHIC"; /* class name of info window */
         INT        rc;
         HELPINIT   hiGRAPHICHelp;     /* Help initialization structure    */
         ULONG      WinCreationFlags;  /* window creation flags            */
         PID        pid;         /* Process identifier for adding name to  */
                                       /* switch list                      */
         TID        tid;               /* Thread identifier                */
         SWCNTRL    Swctl;
         ULONG     WindowPosOption;


/* Init Structure */
   memset((PVOID)&Parm, '\0', sizeof(MAIN_PARM));
   Parm.PrintEnabled = Parm.PrintPropertiesEnabled = Parm.fNotReady = TRUE;
   Parm.DrawThreadID = (TID)-1;
   Parm.CheckedItem = -1;
   Parm.DrawParm.rclBounds.xLeft = Parm.DrawParm.rclBounds.yBottom =
                                   Parm.DrawParm.rclBounds.xRight =
                                   Parm.DrawParm.rclBounds.yTop = 0xFFFF;
   Parm.DrawParm.fAllValid = Parm.fStoppable = Parm.DrawParm.fDrawing = TRUE;

   /* Create semaphores */
   DosCreateMutexSem(NULL, &Parm.hmtxsemStoppable, DC_SEM_SHARED, FALSE);
   DosCreateEventSem(NULL, &Parm.hevsemTerminate, DC_SEM_SHARED, TRUE);
   DosCreateEventSem(NULL, &Parm.hevsemPrintEnds, DC_SEM_SHARED, TRUE);
   DosCreateEventSem(NULL, &Parm.hevsemStop, DC_SEM_SHARED, TRUE);
   /*
    * The WinInitialize routine initializes the Presentation Manager
    * facilities for use by this application and returns a handle to the
    * anchor block assigned to the application by PM.
    */
   if ((Parm.hAB = WinInitialize(0)) == (HAB)0)
      return(FALSE);

   Parm.ProgramTitle = GetString(Parm.hAB, APPLICATION_NAME, ALLOC_STRING);

   /* The WinCreateMsgQueue call creates a message queue for this application */
   if ((Parm.hMQ = WinCreateMsgQueue(Parm.hAB, 16384)) == (HMQ)0)
      return(FALSE);

   /* Query System Pointer */
   Parm.hptrGlass = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
   Parm.hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE);


/* The following function registers the classes of all application windows */
   strcpy(szAppName, GetString(Parm.hAB, APPLICATION_NAME, STATIC_STRING));
   rc = WinRegisterClass(Parm.hAB,         /* Anchor block handle             */
                         (PCH)szAppName,   /* Name of class being registered  */
                         (PFNWP)WndProc,   /* Window procedure for class      */
                         CS_CLIPCHILDREN | CS_SIZEREDRAW | CS_SYNCPAINT,
                         0);

   if (rc == FALSE)
      return(FALSE);

   /* register a class for the information window */
   rc = WinRegisterClass(Parm.hAB,         /* Anchor block handle             */
                         (PCH)InfoWindow,  /* Name of class being registered  */
                         (PFNWP)WndProcI,  /* Window procedure for class      */
                         /* CS_SIZEREDRAW | CS_SYNCPAINT | CS_SAVEBITS, */
                         0L,
                         sizeof(PMAIN_PARM));

   if (rc == FALSE)
      {
      strcpy(Parm.ErrorMsg,
             GetString(Parm.hAB, ERRMSG_REGISTER_FAILED, STATIC_STRING));
      return(FALSE);
      }

   /* IPF Initialization Structure */

   hiGRAPHICHelp.cb = sizeof(HELPINIT);            /* size of init structure */
   hiGRAPHICHelp.ulReturnCode = 0;
   hiGRAPHICHelp.pszTutorialName = 0;              /* no tutorial            */
   hiGRAPHICHelp.phtHelpTable = (PVOID)(0xffff0000 | ID_GRAPHIC);
   hiGRAPHICHelp.hmodAccelActionBarModule = 0;
   hiGRAPHICHelp.idAccelTable = 0;
   hiGRAPHICHelp.idActionBar = 0;
   hiGRAPHICHelp.pszHelpWindowTitle = szAppName;
   hiGRAPHICHelp.hmodHelpTableModule = 0;
   hiGRAPHICHelp.fShowPanelId = 0;
   hiGRAPHICHelp.pszHelpLibraryName = HELP_FILE;

   /* Create Instance of IPF */

   Parm.hWndGRAPHICHelp = WinCreateHelpInstance(Parm.hAB, &hiGRAPHICHelp);

   if (!Parm.hWndGRAPHICHelp || hiGRAPHICHelp.ulReturnCode)
      {
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                    GetString(Parm.hAB, ERRMSG_NO_HELP, STATIC_STRING),
                    (PSZ)Parm.ProgramTitle,
                    1,
                    MB_OK | MB_APPLMODAL | MB_MOVEABLE);
      WinDestroyHelpInstance(Parm.hWndGRAPHICHelp);
      }
    /*
     * The CreateWindow function creates a frame window for this application's
     * top window, and sets the window's size and location as appropriate.
     */
   WinCreationFlags = FCF_TITLEBAR     |
                      FCF_SYSMENU      |
                      FCF_MINBUTTON    |
                      FCF_MAXBUTTON    |
                      FCF_SIZEBORDER   |
                      FCF_MENU         |
                      FCF_ACCELTABLE   |
                      FCF_ICON;

   WinCreationFlags |= FCF_SHELLPOSITION;

   /* Create the frame window */
   Parm.hWndFrame = WinCreateStdWindow(HWND_DESKTOP, /* parent of window      */
                                WS_VISIBLE,          /* frame window style    */
                                &WinCreationFlags,   /* frame flags           */
                                szAppName,           /* class name            */
                                Parm.ProgramTitle,   /* window title          */
                                WS_VISIBLE |         /* client window style   */
                                WS_CLIPCHILDREN,
                                (HMODULE)NULL,       /* module for resources  */
                                ID_GRAPHIC,          /* resource id           */
                                (HWND FAR *)&Parm.hWndClient); /* client handle*/
      /*
       * if hWndFrame is NULL, an error occured when opening the window,
       *     notify the user and exit this function
       */
   if (Parm.hWndFrame == (HWND)0)
      {
      strcpy(Parm.ErrorMsg, GetString(Parm.hAB, ERRMSG_CREATE_WINDOW,
                                       STATIC_STRING));
      return(FALSE);
      }

   /* Associate Instance of IPF */

   if (Parm.hWndGRAPHICHelp)
      WinAssociateHelpInstance(Parm.hWndGRAPHICHelp,
                               Parm.hWndFrame);
   /*
    * The following inline routine fills out the application's switch control
    * structure with the appropriate information to add the application's
    * name to the OS/2 Task Manager List, a list of the jobs currently
    * running on the computer.
    */
   WinQueryWindowProcess(Parm.hWndFrame, &pid, &tid);
   Swctl.hwnd = Parm.hWndFrame;                     /* Frame window handle    */
   Swctl.idProcess = pid;                           /* Process identifier     */
   Swctl.uchVisibility = SWL_VISIBLE;               /* visibility             */
   Swctl.fbJump = SWL_JUMPABLE;                     /* Jump indicator         */
   strcpy(Swctl.szSwtitle, Parm.ProgramTitle);      /* Frame window title     */
   WinAddSwitchEntry(&Swctl);

   /* Copy Application Defaults */
   if (ApplDefaults)
      {
      strcpy(Parm.DriverName, ApplDefaults->DriverName);
      }

   /* Query Default Printer and set print items (Print & Printer Properties) */
   SetPrintItems(&Parm);

   /* create information window */
     Parm.InfoWnd.hWnd = WinCreateWindow(Parm.hWndClient,
                                         InfoWindow,
                                         (PSZ)InfoWindow,
                                         WS_SAVEBITS | WS_CLIPCHILDREN,
                                         0,
                                         0,
                                         1,
                                         1,
                                         Parm.hWndClient,
                                         HWND_TOP,
                                         ID_GRAPHIC,
                                         (PVOID)&Parm,
                                         NULL);

   if (Parm.InfoWnd.hWnd == (HWND)0)
   {
      strcpy(Parm.ErrorMsg, GetString(Parm.hAB, ERRMSG_INFO_WINDOW, STATIC_STRING));
      return(FALSE);
   }

/*
 * Query current process ID, thread ID and the process ID of the parent
 * process
 */
   DosGetInfoBlocks(&Parm.ptibMain, &Parm.ppibMain);

/* create draw thread */
   if (DosCreateThread(&Parm.DrawThreadID,
                    (PFNTHREAD)DrawThread,
                    (ULONG)(PMAIN_PARM)&Parm,
                    (ULONG)0,
                    (ULONG)STACK_SIZE))
   {
      strcpy(Parm.ErrorMsg, GetString(Parm.hAB, ERRMSG_DRAW_THREAD, STATIC_STRING));
      return(FALSE);
   }

/* create print thread */
   if (DosCreateThread(&Parm.PrintThreadID,
                    (PFNTHREAD)PrintThread,
                    (ULONG)(PMAIN_PARM)&Parm,
                    (ULONG)0,
                    (ULONG)STACK_SIZE))
   {
      strcpy(Parm.ErrorMsg, GetString(Parm.hAB, ERRMSG_PRINT_THREAD,
             STATIC_STRING));
      return(FALSE);
   }

/* invalidate text rectangle */
   InvalidateTextRect(Parm.hWndClient);

} /* End of Initialize */

/**************************************************************************
 * Function: Report Error
 *
 * Display a message box with the error returned from the function
 * Initialize.
 *
 * Parameters: none
 *
 * Result:  none
 *
 ***************************************************************************/
VOID ReportError(VOID)

{
   if (Parm.ErrorMsg)
      WinMessageBox(HWND_DESKTOP,
                 (Parm.hWndFrame) ? Parm.hWndFrame : HWND_DESKTOP,
                 (PSZ)Parm.ErrorMsg,
                 (PSZ)NULL,
                 0,
                 MB_MOVEABLE | MB_CUACRITICAL | MB_OK);
} /* End of ReportError */

/*************************************************************************
 * Function: EndProgram
 *
 * End the threads and discard all windows
 *
 * Parameters: none
 *
 * Result:  none
 *
 ***************************************************************************/
VOID EndProgram(VOID)
{
   ULONG      ResetCount;
/*
 * Perform clean up before exiting application.
 * Destroy Instance of IPF
 */
   if (Parm.hWndGRAPHICHelp)
      WinDestroyHelpInstance(Parm.hWndGRAPHICHelp);

/* end the draw thread */
   if (Parm.DrawThreadID != -1)
   {
      WinRequestMutexSem(Parm.hmtxsemStoppable, (ULONG)-1);
      Parm.fStoppable = TRUE;
      DosReleaseMutexSem(Parm.hmtxsemStoppable);
      SendCommand(&Parm, (ULONG)FLUSH_COMMAND, (ULONG)NULL);
      DosResetEventSem(Parm.hevsemTerminate, &ResetCount);
      if (SendCommand(&Parm, (ULONG)STOP_THREAD, (ULONG)NULL))
         WinWaitEventSem(Parm.hevsemTerminate, (ULONG)-1);
   }

/* end the print thread */
   if (Parm.PrintThreadID != -1)
   {
      DosResetEventSem(Parm.hevsemPrintEnds, &ResetCount);
      WinPostQueueMsg(Parm.hMQPrint, STOP_THREAD, MPFROMSHORT(0), MPFROMSHORT(0));
      WinWaitEventSem(Parm.hevsemPrintEnds, (ULONG)-1);
   }

/* wait until Draw Thread has ended */
   DosWaitThread(&Parm.DrawThreadID, DCWW_WAIT);

/* wait until print thread has ended */
   DosWaitThread(&Parm.PrintThreadID, DCWW_WAIT);

   if (Parm.InfoWnd.hWnd)
      WinDestroyWindow(Parm.InfoWnd.hWnd);  /* Destroy the Infowindow            */
   if (Parm.hWndFrame)
      WinDestroyWindow(Parm.hWndFrame);     /* Destroy the frame window          */
   if (Parm.hMQ)
      WinDestroyMsgQueue(Parm.hMQ);  /* Destroy the application message queue */
   if (Parm.hAB)
      WinTerminate(Parm.hAB);        /* Terminate the application's use of   */
                                     /* Presentation Manager resources       */

   /* Close the semaphores */
   DosCloseMutexSem(Parm.hmtxsemStoppable);        /* new with 2.0 */
   DosCloseEventSem(Parm.hevsemTerminate);
   DosCloseEventSem(Parm.hevsemPrintEnds);
   DosCloseEventSem(Parm.hevsemStop);
} /* End of EndProgram */

/**************************************************************************
 * Function: WndProc
 *
 * Main Window Procedure
 *
 * This procedure provides service routines for the general PM events
 * (messages) that PM sends to the window, as well as the user
 * initiated events (messages) that are generated when the user selects
 * the action bar and pulldown menu controls or the corresponding
 * keyboard accelerators.
 *
 * The SWITCH statement shown below distributes the window messages to
 * the respective message service routines, which are set apart by the
 * CASE statements. The window procedures must provide an appropriate
 * service routine for its end user initiated messages, as well as the
 * general PM messages (like the WM_CLOSE message). If a message is
 * sent to this procedure for which there is no programmed CASE clause
 * (i.e., no service routine), the message is defaulted to the
 * WinDefWindowProc function, where it is disposed of by PM.
 *
 * Parameters: HWND         window handle
 *             USHORT       message
 *             MPARAM       message parameter 1
 *             MPARAM       message parameter 2
 *
 * Result:  MRESULT         message result
 *
 ***************************************************************************/
MRESULT EXPENTRY WndProc(HWND hWnd, ULONG message, MPARAM mp1, MPARAM mp2)
{
         HPS        hPS;      /* Handle for the Presentation Space         */
         RECTL      rClient;  /* Handle to rectangle formed by client area */
         PSWP       WindowPos;
static   BOOL       WindowMinimized = FALSE;
         HMF        hmfPicture;
static   CHAR       Filename[260];
static   CHAR       Filemask[260];
static   MENUITEM   mi =
                    {
                    1, MIS_TEXT, 0, IDM_F_TEXT_PREVIOUS, (HWND)0, (ULONG)0
                    };
static   BOOL       MenuUpdated = FALSE;
         PSZ        FileName;
         PSZ        DialogHeader;

   switch(message)
   {
   case SET_PRINT_STATUS:              /* set print status              */
        Parm.PrintStatus = (BOOL)SHORT1FROMMP(mp1);
        Parm.hpsPrint = (HPS)LONGFROMMP(mp2);
        WinSendMsg(Parm.InfoWnd.hWnd,  /* hide/show information window */
                   ENABLE_INFO_WND,
                   mp1,
                   MPFROMSHORT(0));
        SetPrintItems(&Parm);              /* set item "Print" in the menu */
        break;

   case WM_CREATE:
        return(WndProcCreate(hWnd, &Parm));
        break;

   case WM_ERASEBACKGROUND:
        /*
         * If the flag fNotReady is set, the default processing for this
         * message occurs.
         */
        if (Parm.fNotReady)
           return(MRESULT)(TRUE);
        else
           return(MRESULT)(FALSE);
        break;

   case WM_USER_END_ON_ERROR:          /* Display error and end program */
        WinMessageBox(HWND_DESKTOP,
                      hWnd,
                      (PSZ)PVOIDFROMMP(mp2),
                      (PSZ)PVOIDFROMMP(mp1),
                      (USHORT)0,
                      MB_OK | MB_MOVEABLE | MB_CUACRITICAL | MB_APPLMODAL);
        WinPostMsg(hWnd,
                   WM_QUIT,
                   MPFROMSHORT(0),
                   MPFROMSHORT(0));
        break;
   case WM_USER_DEFINED_CODE:          /* own messages                  */
        switch (SHORT1FROMMP(mp1))
           {
           case CHECK_ITEM:            /* check item in file menu       */
                break;
           }

   case WM_COMMAND:
        /*
         * The PM messages for action bar and pulldown menu items are
         * processed in this routine.
         */
        switch(SHORT1FROMMP(mp1))
           {
           case IDM_F_GPI:             /* menu item "GPI"               */
               *Parm.Message = '\0';
                if (Parm.CheckedItem != 1)
                {
                   StopDrawing(&Parm);
                   if (GpiResetPS(Parm.hpsClient, GRES_ALL) == GPI_ERROR)
                       DispError(Parm.hAB,Parm.hWndClient);
                   SendCommand(&Parm, (ULONG)SET_TRANSFORM, (ULONG)NULL);
                   SendCommand(&Parm, (ULONG)DRAW_ORDERS, (ULONG)NULL);
                   Parm.DrawThreadActivated = TRUE;
                   CheckPulldown(&Parm, 1);
                   SetTitleBarText(&Parm, "GPI");
                }
                break;

           case IDM_F_METAFILE_LOAD:/* load metafile from menu item "METAFILE"*/
                *Parm.Message = '\0';
                strcpy(Filemask, Parm.MetafilePath);
                FileName = strrchr(Filemask, (int)'\\');
                if (FileName == NULL)
                   FileName = Filemask;
                else
                   FileName++;
                strcpy(FileName, "*.MET");

                /* Display File Dialog */
                DialogHeader = GetString(Parm.hAB, FILEDLG_META_HEADER,
                                         ALLOC_STRING);
                if (OpenFileDialog(Parm.hWndFrame, DialogHeader,
                                   Filemask, Filename))
                   {
                      strcpy(Parm.MetafilePath, Filename);
                      WinSendMsg(hWnd,
                                 IDM_F_METAFILE,
                                 MPFROMSHORT(0),
                                 MPFROMSHORT(0));
                   }
                free(DialogHeader);
                break;

           case IDM_F_PRINT:              /* menu item "Print"          */
                switch (Parm.CheckedItem)
                   {
                   case 1:             /* print GPI */
                        PrintGpiMeta(&Parm, DRAW_GPI);
                        break;
                   case 2:             /* print Metafile */
                        PrintGpiMeta(&Parm, DRAW_META);
                        break;
                   }
                break;

           case IDM_O_PRINTPROPERTIES:   /*  menu item "Job properties" */
                QueryJobProperties(Parm.hAB,
                                   &Parm.DriverData,
                                   &Parm.DriverDataLength,
                                   Parm.DriverName,
                                   Parm.QueueName,
                                   NULL, TRUE);

                break;

           case IDM_H_HELPINDEX:        /* menu item "Help for Index"  */
                if (Parm.hWndGRAPHICHelp)
                   WinSendMsg(Parm.hWndGRAPHICHelp, HM_DISPLAY_HELP,
                             MPFROMSHORT(HELP_INDEX), 0L);
                break;

           case IDM_H_HELPGEN:          /* menu item "Help for General"*/
                if (Parm.hWndGRAPHICHelp)
                   WinSendMsg(Parm.hWndGRAPHICHelp, HM_DISPLAY_HELP,
                             MPFROMSHORT(HELP_GEN), 0L);
                break;

           case IDM_H_HELPUSING:        /* menu item "Help for Using"  */
                if (Parm.hWndGRAPHICHelp)
                   WinSendMsg(Parm.hWndGRAPHICHelp, HM_DISPLAY_HELP,
                             MPFROMSHORT(HELP_USING), 0L);
                break;

           case IDM_H_HELPKEYS:         /* menu item "Help for Keys"   */
                if (Parm.hWndGRAPHICHelp)
                   WinSendMsg(Parm.hWndGRAPHICHelp, HM_DISPLAY_HELP,
                              MPFROMSHORT(HELP_KEYS), 0L);
                break;

           case IDM_H_ABOUT:            /* menu item "About"           */
                WinDlgBox(HWND_DESKTOP, hWnd, (PFNWP)AboutDlg,
                          0, ID_ABOUT, (PBYTE)NULL);
                break;

           default:
                break; /* End of default case for switch(mp1) */
           }
         break; /* End of WM_COMMAND */

    case IDM_F_METAFILE:
         StopDrawing(&Parm);
         if (GpiResetPS(Parm.hpsClient, GRES_ALL) == GPI_ERROR)
             DispError(Parm.hAB,Parm.hWndClient);
         if (LONGFROMMP(mp1) == 0L)
            hmfPicture = GpiLoadMetaFile(Parm.hAB, Parm.MetafilePath);
         else
            hmfPicture = (HMF)LONGFROMMP(mp1);
         if (hmfPicture == (HMF)NULL)
            {
            StringWithVarText(Parm.hAB, ERRMSG_METAFILE_FAILED,
                              Parm.MetafilePath, Parm.Message),
            Parm.fNotReady = FALSE;
            DisplayErrorMessage(Parm.hWndClient, Parm.Message);
            }
         else
            {
            SendCommand(&Parm, (ULONG)SET_TRANSFORM,
                        (ULONG)hmfPicture);
            SendCommand(&Parm, (ULONG)DRAW_ORDERS, (ULONG)NULL);
            Parm.DrawThreadActivated = TRUE;
            CheckPulldown(&Parm, 2);
            if (LONGFROMMP(mp1) == 0L)
               SetTitleBarText(&Parm, Parm.MetafilePath);
            else
               SetTitleBarText(&Parm, "!Clipboard Metafile");
            }
         break;

    case WM_INITMENU:      /* message appears on selecting a pulldown menu */
         switch(SHORT1FROMMP(mp1))
            {
            case IDM_FILE:             /* set items "Print" and "Print      */
            case IDM_OPTIONS:          /* properties" when menu is selected */
                 SetPrintItems(&Parm);
                 break;
            case IDM_F_METAFILE:
                 /* test, if metafile data in the clipboard */
                 /* test, if a metafile was selected */
                   if (*Parm.MetafilePath != '\0')
                    {
                    FileName = strrchr(Parm.MetafilePath, (int)'\\');
                    if (FileName == NULL)
                       FileName = Parm.MetafilePath;
                    else
                       FileName++;
                    WinSendMsg(HWNDFROMMP(mp2),
                               MM_INSERTITEM,
                               MPFROMP(&mi),
                               MPFROMP(FileName));
                    MenuUpdated = TRUE;
                    }
                 break;
            }
         break;

   case WM_DESTROY:
      /*
       * The application has asked for the window to be destroyed.
       * Where appropriate, the region and the graphics presentation
       * spaces are destroyed.
       */
      if (Parm.DrawParm.hrgnInvalid != (HRGN)NULL)
         if (GpiDestroyRegion(Parm.hpsClient,
                              Parm.DrawParm.hrgnInvalid) == GPI_ERROR)
             DispError(Parm.hAB,Parm.hWndClient);
      if (Parm.hpsClient !=(HPS)NULL)
         {
          if (GpiAssociate(Parm.hpsClient, (HDC)NULL) == GPI_ERROR)
              DispError(Parm.hAB,Parm.hWndClient);
          if (GpiDestroyPS(Parm.hpsClient) == GPI_ERROR)
              DispError(Parm.hAB,Parm.hWndClient);
         }
      if (Parm.hpsPaint !=(HPS)NULL)
         if (GpiDestroyPS(Parm.hpsPaint) == GPI_ERROR)
             DispError(Parm.hAB,Parm.hWndClient);
      break;

   case WM_MINMAXFRAME:           /* minimize/maximize window */
        WindowPos = PVOIDFROMMP(mp1);
        if (WindowPos->fl & SWP_MINIMIZE)
           {
           WindowMinimized = TRUE;
           if (Parm.DrawThreadActivated)
              SendCommand(&Parm, (ULONG)FLUSH_COMMAND, (ULONG)NULL);
           }
        else
           {
           WindowMinimized = FALSE;
           }
        break;


   case HM_QUERY_KEYS_HELP:
        /*
         * If the user requests Keys Help from the help pulldown,
         * IPF sends the HM_QUERY_KEYS_HELP message to the application,
         * which should return the panel id of the Keys Help panel
         */
        return (MRESULT)PANEL_HELPKEYS;  /* return id of key help panel */
        break;

   case HM_ERROR:
        /*
         * If an IPF error occurs, an HM_ERROR message will be sent to
         * the application.
         */
        if (Parm.hWndGRAPHICHelp && ((ULONG)mp1) == HMERR_NO_MEMORY)
           {
           WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                         GetString(Parm.hAB, ERRMSG_HELP_TERMINATED,
                                   STATIC_STRING),
                         Parm.ProgramTitle,
                         1,
                         MB_OK | MB_APPLMODAL | MB_MOVEABLE);
           WinDestroyHelpInstance(Parm.hWndGRAPHICHelp);
           }
         else
           {
           strcpy(Parm.ErrorMsg,
                  GetString(Parm.hAB, ERRMSG_HELP_ERROR, STATIC_STRING));
           _ltoa(LONGFROMMP(mp1), &Parm.ErrorMsg[strlen(Parm.ErrorMsg)], 16);

           WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                         (PSZ)Parm.ErrorMsg,
                         (PSZ)Parm.ProgramTitle,
                         1,
                         MB_OK | MB_APPLMODAL | MB_MOVEABLE);
           }
        break;

   case WM_SIZE:     /* code for sizing client area */
        if (WindowMinimized)
           break;
        if (Parm.ResizeWindow)
           AdjustWindowSize(&Parm);
        else
           Parm.ResizeWindow = TRUE;
        if (Parm.DrawThreadActivated)  /* cancel any pending draw requests */
        {
           SendCommand(&Parm, (ULONG)FLUSH_COMMAND, (ULONG)NULL);
           SendCommand(&Parm, (ULONG)SIZING, (ULONG)NULL);
        }
        WinPostMsg(Parm.InfoWnd.hWnd,     /* reposition information window */
                   POS_INFO_WND,
                   MPFROMSHORT(0),
                   MPFROMSHORT(0));
        break;       /* End of WM_SIZE */

   case WM_PAINT:    /* code for the window's client area */
        if (Parm.DrawThreadActivated)
           return(WndProcPaint(&Parm));
        /* Obtain a handle to a cache presentation space */
        hPS = WinBeginPaint(hWnd, 0, 0);

        /* Determine the size of the client area */
        WinQueryWindowRect(hWnd, &rClient);

        /* Fill the background with the default background color */
        WinFillRect(hPS, &rClient, CLR_BACKGROUND);

        /* return presentation space to state before WinBeginPaint */
        WinEndPaint(hPS);

        break; /* End of WM_PAINT */

   default:
        /*
         * For any message which you don't specifically provide a
         * service routine, you should return the message to PM using
         * the WinDefWindowProc function.
         */
        return(WinDefWindowProc(hWnd, message, mp1, mp2));
        break;  /* End of default */
   }
return(0L);
} /* End of WndProc */

/*************************************************************************
 * Function: CheckPulldown
 *
 * Check one of the pulldown items "GPI", "METAFILE"
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *             ULONG       number of item to check
 *
 * Result:  none
 *
 ***************************************************************************/
VOID CheckPulldown(PMAIN_PARM Parm, LONG index)
{
static   ULONG     Items[] =
                    {
                    IDM_F_GPI,
                    IDM_F_METAFILE,
                    };

/* if item already checked, do nothing */
   if (Parm->CheckedItem != index)
   {
      if (index == 0)                  /* If text is selected */
      {
         WinSendMsg(Parm->hWndFrame,
                 WM_USER_DEFINED_CODE,
                 MPFROMSHORT(INIT_MLE),
                 MPFROMSHORT(TRUE));
      }
      if (Parm->CheckedItem == 0)      /* If text is deselected */
      {
         WinSendMsg(Parm->hWndFrame,
                 WM_USER_DEFINED_CODE,
                 MPFROMSHORT(INIT_MLE),
                 MPFROMSHORT(FALSE));
      }
      Parm->CheckedItem = index;
      if (!index)
         InvalidateTextRect(Parm->hWndClient);
   }
} /* End of CheckPullDown */

/*************************************************************************
 * Function: AdjustWindowSize
 *
 * resize frame window
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  none
 *
 ***************************************************************************/
VOID AdjustWindowSize(PMAIN_PARM Parm)
{
   SWP        SizeFrame;
   SWP        SizeClient;
   HPS        hPS;
   SIZEF      CharSizeClient;

   hPS = WinGetPS(Parm->hWndClient);         /* get PS of client area      */

   /* get size of character box */
   if (GpiQueryCharBox(hPS, (PSIZEF)&CharSizeClient) == GPI_ERROR)
       DispError(Parm->hAB,Parm->hWndClient);

   WinReleasePS(hPS);                        /* release presentation space */

   WinReleasePS(hPS);                        /* release presentation space */
   /*
    * resize window, so that at least the information line and one line of
    * the multiline edit field can be displayed
    */
   WinQueryWindowPos(Parm->hWndFrame,
                     &SizeFrame);

   WinQueryWindowPos(Parm->hWndClient,
                     &SizeClient);
} /* End of AdjustWindowSize */

/*************************************************************************
 * Function: WndProcCreate
 *
 * The WndProcCreate function obtains a device context for the client
 * area of the window, creates a normal graphics presentation space,
 * and associates the two. The presentation page is defined in pels,
 * and is the same size as the maximum client area.
 * A second graphics presentation space is defined for use by the main
 * thread. This is the same size and format as the first presentation
 * space.  WndProcCreate also defines an empty region for hpsClient.
 * This is used by the asynchronous thread to accumulate invalid
 * regions in the client area.
 *
 * Parameters: HWND         window handle of the frame window
 *             PMAIN_PARM   main parameter structure
 *
 * Result:  MRESULT         TRUE if an error appeared, otherwise false
 *
 ***************************************************************************/
MRESULT WndProcCreate(HWND hwnd, PMAIN_PARM Parm)
{
   SIZEL      sizlClient;

/* query size of Desktop */
   sizlClient.cx = WinQuerySysValue(HWND_DESKTOP, SV_CXFULLSCREEN);
   sizlClient.cy = WinQuerySysValue(HWND_DESKTOP, SV_CYFULLSCREEN);

   /* open device context for client area */
   Parm->hdcClient = WinOpenWindowDC(hwnd);
   /* create presentation space for client area  and associate it with DC */
   Parm->hpsClient = GpiCreatePS(Parm->hAB,
                                 Parm->hdcClient,
                                 &sizlClient,
                                 GPIA_ASSOC | PU_PELS);
   if (Parm->hpsClient == (HPS)NULL)
      return((MRESULT)TRUE);

   /* create presentation space for painting */
   Parm->hpsPaint = GpiCreatePS(Parm->hAB,
                                (HDC)NULL,
                                &sizlClient,
                                PU_PELS);

   /* create region for client area */
   Parm->DrawParm.hrgnInvalid = GpiCreateRegion(Parm->hpsClient,
                                                (ULONG)0, (PRECTL)NULL);

   return(FALSE);
} /* End of WndProcCreate */

/*************************************************************************
 * Function: SetPrintItems
 *
 * Test (default) printer queue and enable/disable menu items "Print" and
 * "Print properties".
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  none
 *
 ***************************************************************************/
VOID SetPrintItems(PMAIN_PARM Parm)
{
BOOL       QueuePredefined;

                                        /* test, if queue preselected */
   if (Parm->QueueName[0] == '\0')
      QueuePredefined = FALSE;
   else
      QueuePredefined = TRUE;
/*
 * if queue preselected, test if queue is already valid, otherwise query
 * default queue
 */
   if (QueryDefaultQueue(Parm->QueueName, Parm->DriverName))
   {
   /*
    * if item "Print" is disabled and no print request is running, enable
    * menu item "Print"
    */
      if (!Parm->PrintEnabled)
      {
         if (!Parm->PrintStatus)
         {
            WinSendMsg(WinWindowFromID(Parm->hWndFrame, (USHORT)FID_MENU),
                    MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_F_PRINT, TRUE),
                    MPFROM2SHORT(MIA_DISABLED, ~MIA_DISABLED));
            Parm->PrintEnabled = TRUE;
         }
      }
      else
   /*
    * if item "Print" is enabled and a print request is running, disable
    * menu item "Print"
    */
         if (Parm->PrintStatus)
         {
            WinSendMsg(WinWindowFromID(Parm->hWndFrame, (USHORT)FID_MENU),
                    MM_SETITEMATTR,
                    MPFROM2SHORT(IDM_F_PRINT, TRUE),
                    MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
            Parm->PrintEnabled = FALSE;
         }
   /* if menu item "Print properties" is disabled, enable item */
      if (!Parm->PrintPropertiesEnabled)
      {
         WinSendMsg(WinWindowFromID(Parm->hWndFrame, (USHORT)FID_MENU),
                 MM_SETITEMATTR,
                 MPFROM2SHORT(IDM_O_PRINTPROPERTIES, TRUE),
                 MPFROM2SHORT(MIA_DISABLED, ~MIA_DISABLED));
         Parm->PrintPropertiesEnabled = TRUE;
      }
   }
   else
   {
   /*
    * if printer queue is not valid, disable menu items "Print" and
    * "Print properties
    */
      if (Parm->PrintEnabled)
      {
         WinSendMsg(WinWindowFromID(Parm->hWndFrame, (USHORT)FID_MENU),
                 MM_SETITEMATTR,
                 MPFROM2SHORT(IDM_F_PRINT, TRUE),
                 MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
         Parm->PrintEnabled = FALSE;
      }
      if (Parm->PrintPropertiesEnabled)
      {
         WinSendMsg(WinWindowFromID(Parm->hWndFrame, (USHORT)FID_MENU),
                 MM_SETITEMATTR,
                 MPFROM2SHORT(IDM_O_PRINTPROPERTIES, TRUE),
                 MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
         Parm->PrintPropertiesEnabled = FALSE;
      }
      if (!QueuePredefined)
         *Parm->QueueName = '\0';
   }
} /* End of SetPrintItems */

/*************************************************************************
 * Function: StopDrawing
 *
 * Stop drawing "GPI or "METAFILE"  on request.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  none
 *
 ***************************************************************************/
VOID StopDrawing(PMAIN_PARM Parm)
{
ULONG      ResetCount;
    /*
     * if not "GPI" or "METAFILE"  do nothing
     * if ((Parm->CheckedItem < 1) || (Parm->CheckedItem > 3))
     */
   if (Parm->CheckedItem < 1)
      return;

   if (Parm->DrawThreadID != -1)
   {
      if (Parm->hpsClient != (HPS)NULL)
         if (GpiSetStopDraw(Parm->hpsClient, (LONG)TRUE) == GPI_ERROR)
            DispError(Parm->hAB,Parm->hWndClient);
      WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
      Parm->fStoppable = TRUE;
      DosReleaseMutexSem(Parm->hmtxsemStoppable);
      SendCommand(Parm, FLUSH_COMMAND, 0L);
      DosResetEventSem(Parm->hevsemStop, &ResetCount);
      if (SendCommand(Parm, STOP_DRAWING, 0L))
         WinWaitEventSem(Parm->hevsemStop, (ULONG)-1);
   }
} /* End of StopDrawing */

/*************************************************************************
 * Function: AboutDlg
 *
 * Dialog procedure for About dialog box.
 *
 * Parameters: HWND         window handle of dialog box
 *             USHORT       message
 *             MPARAM       message parameter 1
 *             MPARAM       message parameter 2
 *
 * Result:  MRESULT         message result
 *
 ***************************************************************************/
MRESULT EXPENTRY AboutDlg(HWND hWnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{

   switch (msg)
   {
   case DLG_OK:
        WinDismissDlg(hWnd, FALSE);
        break;
   default:
        return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
   return (MRESULT)FALSE;
} /* End of AboutDlg */

/**************************************************************************
 * Function: InvalidateTextRect
 *
 * Query size of information line and invalidate that rectangle.
 *
 * Parameters: HWND         window handle of client area
 *
 * Result:  none
 *
 ***************************************************************************/
static VOID InvalidateTextRect(HWND hWnd)
{
   HPS         hPS;
   FONTMETRICS ActFont;
   RECTL       Rectl;

   WinQueryWindowRect(hWnd, &Rectl);
   hPS = WinGetPS(hWnd);
   if (GpiQueryFontMetrics(hPS, (LONG)sizeof(FONTMETRICS), &ActFont) == GPI_ERROR)
       DispError(Parm.hAB,Parm.hWndClient);
   WinReleasePS(hPS);

   Rectl.yBottom = Rectl.yTop - ActFont.lMaxBaselineExt;
   WinInvalidateRect(hWnd, &Rectl, FALSE);
} /* End of InvalidateTextRect */

/*************************************************************************
 * Function: WndProcI
 *
 * Window procedure for information window.
 *
 * Parameters: HWND         window handle of information window
 *             USHORT       message
 *             MPARAM       message parameter 1
 *             MPARAM       message parameter 2
 *
 * Result:  MRESULT         message result
 *
 ***************************************************************************/
MRESULT EXPENTRY WndProcI(HWND hWndI, ULONG Msg, MPARAM mp1, MPARAM mp2)
{
         PMAIN_PARM Parm;
         FONTMETRICS ActFont;
         HPS        hPS;
         SWP        WinPos;
         RECTL      rInfo;
         POINTL     DrawPos;
static   PSZ        Message;

   if (Msg != WM_CREATE)
      Parm = WinQueryWindowPtr(hWndI, 0);

   switch (Msg)
   {
   case WM_CREATE:      /* Set window pointer to main parameter stucture   */
        Parm = (PMAIN_PARM)PVOIDFROMMP(mp1);
        Message = GetString(Parm->hAB, TEXT_SPOOLING_MESSAGE, ALLOC_STRING);
        WinSetWindowPtr(hWndI, 0, (PVOID)Parm);
        break;
   case ENABLE_INFO_WND:               /* show/hide information window     */
        if (Parm->InfoWnd.SizeX == 0)  /* if first call, calculate size of */
           {                           /* information window               */
           hPS = WinGetPS(hWndI);
           if (GpiQueryFontMetrics(hPS, (LONG)sizeof(FONTMETRICS), &ActFont) == GPI_ERROR)
               DispError(Parm->hAB,Parm->hWndClient);
           Parm->InfoWnd.SizeY = (LONG)ActFont.lMaxBaselineExt;
           Parm->InfoWnd.DrawLine = (LONG)ActFont.lMaxDescender;
           Parm->InfoWnd.SizeX = (LONG)12 * (LONG)ActFont.lAveCharWidth;
           WinReleasePS(hPS);
           WinSetWindowPos(hWndI,
                           (HWND)NULLHANDLE,
                           0L, 0L, Parm->InfoWnd.SizeX,
                           Parm->InfoWnd.SizeY,
                           SWP_SIZE | SWP_HIDE);
           }
        /* show/hide information window */
        Parm->InfoWnd.EnableWindow = SHORT1FROMMP(mp1);
        if (!Parm->InfoWnd.EnableWindow)
           {
           WinSetWindowPos(hWndI, (HWND)NULLHANDLE,
                           0L, 0L, 0L, 0L,
                           SWP_HIDE);
           }
        else
           {
           WinSendMsg(hWndI,
                      POS_INFO_WND,
                      MPFROMSHORT(0),
                      MPFROMSHORT(0));
           WinInvalidateRect(hWndI, (PRECTL)NULL, TRUE);
           }
        break;
   case POS_INFO_WND:                  /* reposition info window if frame */
        if (Parm->InfoWnd.EnableWindow)
        {
        WinQueryWindowPos(Parm->hWndClient, &WinPos); /* window is resized */
        WinSetWindowPos(hWndI, (HWND)HWND_TOP,
                        WinPos.cx - Parm->InfoWnd.SizeX,
                        WinPos.cy - Parm->InfoWnd.SizeY,
                        0L, 0L,
                        SWP_MOVE | SWP_SHOW);
        }
        break;
   case WM_PAINT:                            /* paint information window if*/
        if (Parm->InfoWnd.EnableWindow)      /* window is shown            */
           {
           hPS = WinBeginPaint(hWndI, 0, 0);
           WinQueryWindowRect(hWndI, &rInfo);
           WinFillRect(hPS, &rInfo, CLR_RED);
           GpiSetColor(hPS, CLR_WHITE);
           DrawPos.x = 4L;
           DrawPos.y = (LONG)Parm->InfoWnd.DrawLine;
           GpiCharStringAt(hPS, &DrawPos, strlen(Message) - 1L,
                           Message);
           WinEndPaint(hPS);
           }
        else
           return(WinDefWindowProc(hWndI, Msg, mp1, mp2));
        break;
   default:
        return(WinDefWindowProc(hWndI, Msg, mp1, mp2));
   }
   return (MRESULT)FALSE;
} /* End of WndProcI */

/*************************************************************************
 * Function: SetTitleBarText
 *
 * Change Text of Titlebar
 *
 * Parameters: PMAIN_PARM   Pointer to main parameter structure
 *             PSZ          filename to display
 *
 * Result:  none
 *
 ***************************************************************************/
VOID SetTitleBarText(PMAIN_PARM Parm, PSZ Filename)
{
static   CHAR       TitleBarText[80];
         PSZ        Ptr;

   strcpy(TitleBarText, Parm->ProgramTitle);
   if (Filename != (PSZ)NULL)
      if (*Filename == '!')
      {
         strcat(TitleBarText, " - ");
         strcat(TitleBarText, &Filename[1]);
      }
      else
         if (*Filename != '\0')
         {
            strcat(TitleBarText, " - \"");

         /* Set Ptr to first character after the path if possible */
            if ((Ptr = strrchr(Filename, '\\')) == NULL)
               Ptr = Filename;
            else
               Ptr++;

            strcat(TitleBarText, Ptr);
            strcat(TitleBarText, "\"");
         }

/* Set Title Bar Text */
   WinSetWindowText(WinWindowFromID(Parm->hWndFrame, FID_TITLEBAR),
                 TitleBarText);
} /* End of SetTitleBarText */

/**************************************************************************
 * Function: GetString
 *
 * Get String from resource file
 *
 * Parameters: HAB          Anchor block handle
 *             ULONG        ID of string
 *             BOOL         FALSE - Use static field
 *                          TRUE  - allocate space for the string
 *
 * Result:  PSZ        Pointer to the loaded string or NULL if error
 *
 ***************************************************************************/
PSZ GetString(HAB hAB, ULONG ID, BOOL Alloc)
{
   LONG       sLength;
   static PSZ Field[256];
   PSZ        Ptr;

   if ((sLength = WinLoadString(hAB,
                                (HMODULE)NULLHANDLE,
                                ID,
                                sizeof(Field),
                                (PSZ)Field)) == 0)
      return(NULL);

   if (Alloc)
   {
      Ptr = malloc(sLength + 1);
      memcpy(Ptr, Field, sLength + 1);
      return(Ptr);
   }

   return  (PSZ)Field ;
} /* End of GetString */

/**************************************************************************
 * Function: StringWithVarText
 *
 * Get String from resource file
 *
 * Parameters: HAB          Anchor block handle
 *             ULONG        ID of string
 *             PSZ          Pointer to variable text
 *             PSZ          Pointer to output buffer
 *
 * Result:  PSZ        Pointer to output buffer
 *
 ***************************************************************************/
PSZ StringWithVarText(HAB hAB, ULONG ID, PSZ VarText, PSZ Result)
{
   PSZ        Field;
   PSZ        Ptr;

   Field = GetString(hAB, ID, STATIC_STRING);

   if ((Ptr = strstr(Field, "<t>")) == NULL)
      Ptr = strstr(Field, "<T>");

   strcpy(Result, Field);

   if (Ptr != NULL)
   {
      strcpy(&Result[Ptr - Field], VarText);
      strcat(Result, &Ptr[3]);
   }

   return(Result);
} /* End of StringWithVarText */

/**************************************************************************
 * Function: TemplateOpenFilterProc
 *
 * TemplateOpenFilterProc - This is a procedure that will filter the help
 *                           messages to the open dialog.
 *
 * Parameters: HWND        window handle of information window
 *             USHORT      message
 *             MPARAM      message parameter 1
 *             MPARAM      message parameter 2
 *
 ***************************************************************************/
MRESULT EXPENTRY TemplateOpenFilterProc(HWND hwnd, ULONG message,
                                        MPARAM mp1, MPARAM mp2)
{

  if(message == WM_HELP)
  {
      DisplayHelpPanel(HID_FS_OPEN_DLG_HELP_PANEL);
       return FALSE ;
  }

   return WinDefFileDlgProc( hwnd, message, mp1, mp2 ) ;
} /* End of TemplateOpenFilterProc */

/**************************************************************************
 * Function: DisplayHelpPanel(idPanel)
 *
 * DisplayHelpPanel - Displays the help panel whose id is given
 *
 *  Usage:  Called whenever a help panel is desired to be
 *          displayed, usually from the WM_HELP processing
 *          of the dialog boxes
 *
 *  Method: Sends HM_DISPLAY_HELP message to the help instance
 *
 ***************************************************************************/
VOID DisplayHelpPanel(SHORT idPanel)
{
   if(WinSendMsg(Parm.hWndGRAPHICHelp,
                 HM_DISPLAY_HELP,
                 MPFROMLONG(MAKELONG(idPanel, NULL)),
                 MPFROMSHORT(HM_RESOURCEID)))
   {
      strcpy(Parm.ErrorMsg,
          GetString(Parm.hAB, ERRMSG_HELPDISPLAYERROR, STATIC_STRING));
   }

} /* End of DisplayHelpPanel */

/**************************************************************************
 * Function: DispError
 *
 *  DispError -- report an error returned from an API service.
 *
 *  Usage:  The error message is displayed using a message box
 *
 ***************************************************************************/
VOID DispErrorMsg(HAB hab, HWND hwndFrame, PCH FileName, LONG LineNum)
{
   PERRINFO  pErrInfoBlk;
   PSZ       pszOffSet, pszErrMsg;
   ERRORID   ErrorId;
   PCH       ErrorStr;
   CHAR      szbuff[125];

   DosBeep(800,10);

   if (!hab)
   {                                     /* Non-PM Error */
      WinLoadString(Parm.hAB, 0L, IDS_UNKNOWNMSG, sizeof(szbuff), (PSZ)szbuff);
      ErrorStr = malloc(strlen(szbuff)+strlen(FileName)+10);
      sprintf(ErrorStr, szbuff, FileName, LineNum);
      WinMessageBox(HWND_DESKTOP,         /* Parent window is desk top */
                   hwndFrame,            /* Owner window is our frame */
                   (PSZ)ErrorStr,        /* PMWIN Error message       */
                   "Error Message",      /* Title bar message         */
                   MSGBOXID,             /* Message identifier        */
                   MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); /* Flags */
      free(ErrorStr);
      return;
   }

   ErrorId = WinGetLastError(Parm.hAB);

   if ((pErrInfoBlk = WinGetErrorInfo(Parm.hAB)) != (PERRINFO)NULL)
   {
      pszOffSet = ((PSZ)pErrInfoBlk) + pErrInfoBlk->offaoffszMsg;
      pszErrMsg = ((PSZ)pErrInfoBlk) + *((PSHORT)pszOffSet);

      WinLoadString(Parm.hAB, 0L, IDS_ERRORMSG, sizeof(szbuff), (PSZ)szbuff);
      ErrorStr = malloc(strlen(szbuff)+strlen(pszErrMsg)+strlen(FileName)+10);
      sprintf(ErrorStr, szbuff, pszErrMsg, FileName, LineNum);

      WinMessageBox(HWND_DESKTOP,         /* Parent window is desktop */
                   hwndFrame,            /* Owner window is our frame */
                   (PSZ)ErrorStr,        /* PMWIN Error message       */
                   "Error Message",      /* Title bar message         */
                   MSGBOXID,             /* Message identifier        */
                   MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); /* Flags */

      free(ErrorStr);

      WinFreeErrorInfo(pErrInfoBlk);
   }
} /* End of DispErrorMsg */
/****************************** End of Graphic.c **************************/

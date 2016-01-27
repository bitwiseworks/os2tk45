/*==============================================================*\
 *  HANOI.C - Sample PM application
 *
 *
 *      Copyright  1995  IBM Corp.                                         *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *                               *
 *--------------------------------------------------------------
 *
 *  This program implements a tower of Hanoi program.  This
 *  sample app was written to demonstrate the use of a multi-
 *  threaded program.  The main thread handles the PM interface,
 *  the second thread is started to do the recursive execution
 *  of the Hanoi algorithm.
 *
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *      main(argc, *argv[])     - main routine
 *      MainWndProc(hwnd, msg, mp1, mp2) - main window procedure
 *      MainPaint(hwnd)         - WM_PAINT processing of main
 *      MainCommand(mp1, mp2)   - WM_COMMAND processing of main
 *      CalcThread()            - Sets up and terminates the secondary thread
 *      DrawDisk(hps, cPole, bLevel, fDraw) - Draws or erases a disk from poles
 *      MoveDisk(hps, bFrom, bTo) - Moves a disk from one pole to another
 *      Hanoi(bHeight, bFrom, bTo, bTemp) - Recursive routine for tower of Hanoi
 *      EnableMenuItem(hwnd, sMenuItem, fEnable) - Activates/deactivates
 *                                                 a menu choice
 *      EntryFldDlgProc(hwnd, msg, mp1, mp2) - Handles the set number of
 *                                             disks dialog box
 *      SetupTowers()        - Sets up the global tower data
 *      FixSysMenu(hwndDlg)  - Remove Restore, Size, Min, & Max from system menu
 *      AboutDlgProc(hwnd, msg, mp1, mp2) - processing about dialog box
 *      ObjectWndProc(hwnd, msg, mp1, mp2) -
 *      MessageBox(hwndOwner, IdMsg, fsStyle, fBeep) - process all messages
 *      Init(VOID)                      - iniitialize the process
 *      ExitProc(VOID)                  - terminate the process
 *      CreateBackgroundThread(VOID)    - process the background thread creating
 *      BackgroundThread(ulThreadParam) - handle the background thread process
 *      DestroyBackgroundThread(VOID)   - destroy background thread process
 *      InitHelp(VOID)                  - initialize the help process
 *      HelpHelpForHelp(mp2)            - process the help for help message
 *      HelpExtended(mp2)               - process the extended help message
 *      HelpKeys(mp2)                   - process the help menu of keys
 *      HelpIndex(mp2)                  - process the help index message
 *      HelpAbout(mp2)                  - process the about dialog box message
 *      DisplayHelpPanel(idPanel)       - process the help panel display
 *      DestroyHelpInstance(VOID)       - destroy the help instance
 *      PostBkThreadMsg(msg, mp1, mp2)  - handle the background thread message
 *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_WIN
#define INCL_HELP
#define INCL_WINHEAP
#define INCL_WINDIALOGS
#define INCL_WINWINDOWMGR
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_GPILCIDS
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hanoi.h"

/*--------------------------------------------------------------*\
 *  Global variables
\*--------------------------------------------------------------*/
CHAR   szClientClass[] = "Hanoi";
BYTE   abTowers[3][MAXDISKCNT];    /* Used to hold disk numbers on each post */
BYTE   abTowersNdx[3];             /* Current number of disks on each post   */
BYTE   cTowerSize = DEFAULTSIZE;   /* Holds the total number of disks        */
int    aPolePos[3]= {POSTOFFSET,  /* Holds offset drawing information       */
                       POSTOFFSET + POSTSPACE,
                       POSTOFFSET + 2 * POSTSPACE};
ULONG  ulIterations;
TID    tidCalcThread = 0;                         /* Secondary thread ID */
BOOL   fContinueCalc;
HWND   hwndMainFrame = NULLHANDLE;    /* Handle to the main frame window */
HWND   hwndMain;                     /* Handle to the main client window */
CHAR   szAppName[MAXNAMEL];        /* Buffer for application name string */
HAB    hab;                              /* Anchor block for the process */
HMQ    hmq;                      /* Handle to the process' message queue */
CHAR   szUntitled[MESSAGELEN];           /* buffer for "Untitled" string */
static BOOL fThreadCreated = FALSE;
static HEV hevThreadInit;         /* semaphore for thread initialization */
static HAB habBkThread;
static HMQ hmqBkThread;
static TID tidBkThread;
static HWND hwndObject;
static CHAR szObjectClass[MAXNAMEL];
BOOL   fHelpEnabled = FALSE;         /* flag to determine if help is enabled */
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance = NULLHANDLE;
BOOL   fViewOn = TRUE;                           /* flag to view on or off */
CHAR   *pszDisk       ="Disk Information";
CHAR   *pszDisksMoved ="Disks Moved:";

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY EntryFldDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ObjectWndProc(HWND, ULONG, MPARAM, MPARAM);
ULONG   MessageBox(HWND, LONG, LONG, BOOL);
VOID APIENTRY ExitProc(VOID);
BOOL Init(VOID);
VOID MainPaint(HWND);
LONG MainCommand(HWND, MPARAM, MPARAM);
VOID CalcThread(VOID);
VOID DrawDisk(HPS, BYTE, BYTE, BYTE);
VOID MoveDisk(HPS, BYTE, BYTE);
VOID Hanoi(BYTE, BYTE, BYTE, BYTE);
VOID EnableMenuItem(HWND, LONG, BOOL);
VOID SetupTowers(VOID);
VOID FixSysMenu(HWND);

BOOL CreateBackgroundThread(VOID);
VOID BackgroundThread(ULONG);
BOOL PostBkThreadMsg(ULONG, MPARAM, MPARAM);
VOID DestroyBackgroundThread(VOID);

VOID InitHelp(VOID);
VOID HelpHelpForHelp(MPARAM);
VOID HelpExtended(MPARAM);
VOID HelpKeys(MPARAM);
VOID HelpIndex(MPARAM);
VOID HelpAbout(MPARAM);
VOID DisplayHelpPanel(LONG);
VOID DestroyHelpInstance(VOID);

/****************************************************************\
 *  Main routine
 *--------------------------------------------------------------
 *
 *  Name:    main(argc, *argv[])
 *
 *  Purpose: Initializes the PM environment, calls the initialization
 *           routine, creates main window, and polls the message queue
 *
 *  Usage:
 *
 *  Method:  - obtains anchor block handle and creates message
 *             queue
 *           - calls the initialization routine
 *           - creates the main frame window which creates the
 *             main client window
 *           - polls the message queue via Get/Dispatch Msg loop
 *           - upon exiting the loop, performs exit housekeeping
 *             and then exits
 *
 *  Returns: 0 - if successful execution completed
 *           1 - if error
 *
\****************************************************************/
int main(LONG argc, CHAR *argv[])
{
   QMSG     qmsg;                      /* Message structure */
   ULONG    flCtlData = FCF_STANDARD;  /* Frame control data */

   /* If command line arg, use as the initial number of disks */
   if(argc > 1)
   {
      LONG sHold = (LONG)atoi(argv[1]);
      if(sHold > 0L && sHold <= MAXDISKCNT)
         cTowerSize = (BYTE)sHold;
   }

   SetupTowers();

   hab = WinInitialize(0UL);

   if(!hab)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return RETURN_ERROR;
   }

   hmq = WinCreateMsgQueue(hab, 0L);

   if(!hmq)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return RETURN_ERROR;
   }

   if(!Init())
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_INITFAILED,
                 MB_OK | MB_ERROR,
                 TRUE);

      return RETURN_ERROR;
   }
   /* Create the main window */

   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      &flCtlData,
                                      szAppName,
                                      NULL,
                                      WS_VISIBLE,
                                      (HMODULE)NULL,
                                      ID_RESOURCE,
                                      (PHWND)&hwndMain);
   if(hwndMainFrame == NULLHANDLE)
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_MAINWINCREATEFAILED,
                 MB_OK | MB_ERROR,
                 TRUE);

      return RETURN_ERROR;
   }
   InitHelp();
   /* Get/Dispatch Message loop */
   while(WinGetMsg(hab, &qmsg, NULLHANDLE, 0UL, 0UL))
       WinDispatchMsg(hab, &qmsg);

   /* Destroy the help instance */
   DestroyHelpInstance();

#ifdef BACKGROUND_THREAD
   DestroyBackgroundThread();
#endif

   /* Perform exit housekeeping */
   ExitProc();

   return RETURN_SUCCESS;
}   /* main() */

/****************************************************************\
 *  Initialization routine
 *--------------------------------------------------------------
 *
 *  Name:    Init(VOID)
 *
 *  Purpose: Performs initialization functions required before the main
 *           window can be created.
 *
 *  Usage:   Called once before the main window is created.
 *
 *  Method   - registers all window classes
 *           - performs any command line processing
 *
 *
 *  Returns: TRUE - initialization is successful
 *           FALSE - initialization failed
\****************************************************************/
BOOL Init(VOID)
{
   /* load application name from resource file */
   if(!WinLoadString(hab, NULLHANDLE, IDS_APPNAME, MAXNAMEL, szAppName))
      return FALSE;

   /* load "untitled" string */
   if(!WinLoadString(hab, NULLHANDLE, IDS_UNTITLED, MESSAGELEN, szUntitled))
      return FALSE;

   /* register the main client window class */
   if(!WinRegisterClass(hab,
                        (PSZ)szAppName,
                        (PFNWP)MainWndProc,
                        CS_SIZEREDRAW | CS_CLIPCHILDREN,
                        0UL))
   {
      return FALSE;
   }

   /* If you wish to create a thread for background processing, define
    *  the BACKGROUND_THREAD constant used by the following routine.
    * The routines for the background thread are included in this source
    *  file.
    */

#ifdef BACKGROUND_THREAD
   if(!CreateBackgroundThread())
      return FALSE;
#endif

   return TRUE;
}   /* Init() */

/****************************************************************\
 *  Exit housekeeping procedure
 *--------------------------------------------------------------
 *
 *  Name:    ExitProc()
 *
 *  Purpose: Cleans up certain resources when the application terminates
 *
 *  Usage:   Routine is called by main just before the application exits
 *
 *  Method:  Global resources, such as the main window and message queue,
 *           are destroyed and any system resources used are freed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID APIENTRY ExitProc()
{
    /* Destroy the secondary thread if it exists */
    if (tidCalcThread)
    {
       fContinueCalc = FALSE;                    /* Notify thread to quit */
       DosWaitThread(&tidCalcThread, DCWW_WAIT); /* Wait until complete */
    }

    /* destroy the main window if it exists */
    if(WinIsWindow(hab, hwndMainFrame))
       WinDestroyWindow(hwndMainFrame);

    /*--------------------------------------------------*\
     *      Any other system resources used
     *      (e.g. memory or files) should be freed here
    \*--------------------------------------------------*/
    WinDestroyMsgQueue(hmq);

    WinTerminate(hab);

    return;

}   /* ExitProc() */

/****************************************************************\
 *  Main client window procedure
 *--------------------------------------------------------------
 *
 *  Name:    MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Handles all the messages associated with the main window
 *           and calls the appropriate handling procedures.
 *
 *  Usage:   Called for each message placed in the window's message queue
 *
 *  Method:  Called only by main().  Note that when WM_PAINT executes,
 *           the secondary thread may change data during the update
 *           which may cause text to appear at the "old" position in the
 *           window.  However, this is NOT a write
 *           conflict, as only 1 thread does the writing.
 *
 *  Returns: Return values are determined by each message
 *
\****************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   CHAR   szMsg[MSGBUFSIZE];           /* Sprintf buffer */
   MRESULT sRC;

   switch(msg)
   {
      case WM_PAINT:
         MainPaint(hwnd);
         break;

      case WM_COMMAND:
         MainCommand(hwnd, mp1, mp2);
         break;

      case WM_SIZE:
//         WinSendMsg(hwnd, WM_ERASEBACKGROUND, ,,,,
         break;

      case IDM_DONE:
         EnableMenuItem(hwnd, IDM_START, TRUE);  /* Reenable Start & set  */
         EnableMenuItem(hwnd, IDM_SET, TRUE);
         EnableMenuItem(hwnd, IDM_STOP, FALSE);  /* Disable stop          */
                                                 /* Print msg             */
         if(ulIterations > 1)
            sprintf(szMsg,"%lu disk transfers were made.", ulIterations);
         else
            sprintf(szMsg,"%lu disk transfer was made.", ulIterations);
         WinMessageBox(HWND_DESKTOP,
                       hwnd,
                       szMsg,
                       "Done!",
                       0UL,
                       MB_OK | MB_MOVEABLE | MB_INFORMATION);
         SetupTowers();                          /* Reset towers          */
         WinInvalidateRect(hwnd, NULL, FALSE);   /* Force a screen redraw */
         break;

      case HM_QUERY_KEYS_HELP:
         return (MRESULT)PANEL_KEYSHELP;

      default:
         sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   /* all window procedures should return 0 as a default */
   return (MRESULT)NULL;
}   /* MainWndProc() */

/****************************************************************\
 *  Main client painting routine
 *--------------------------------------------------------------
 *
 *  Name:    MainPaint(HWND hwnd)
 *
 *  Purpose: Paints the main client window.
 *
 *  Usage:   Routine is called whenver the client window
 *           procedure receives a WM_PAINT message
 *
 *
 *  Method:  -begins painting by calling WinBeginPaint and retrieving
 *            the HPS for the window
 *           -performs any painting desired
 *           -ends painting by calling WinEndPaint
 *
 *  Returns: 0 - if successful execution completed
 *           1 - if error
 *
\****************************************************************/
VOID MainPaint(HWND hwnd)
{
   RECTL  rclUpdate, rect;               /* Rectangle struct for painting */
   HPS    hps = WinBeginPaint(hwnd, NULLHANDLE, (PRECTL)&rclUpdate);
   POINTL ptl;                           /* Point struct for painting     */
   BYTE   cPole, cnt;                    /* Utility variables             */
   CHAR   szBuffer[BUFF_SIZE];
   LONG   lVert, bHeight, bLeft;
   FONTMETRICS FontMetrics;

   /* Fill update rectangle with window color. */
   WinFillRect(hps, &rclUpdate, SYSCLR_WINDOW);
   WinQueryWindowRect(hwndMain, &rect);
   /*
    *     Get the character set size.
    */
   GpiQueryFontMetrics(hps, sizeof(FontMetrics), &FontMetrics);
   ptl.x = (rect.xRight -
            ((signed long)strlen(pszDisk) * FontMetrics.lAveCharWidth)) / 2;
   ptl.y = (rect.yTop - BASETHICK - 3 * FontMetrics.lEmHeight)  / 2;
   if(fViewOn)                    /*  view disk information  */
   {
      memset((void *)szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, pszDisk);
      GpiCharStringAt(hps, &ptl, (long)strlen(szBuffer), szBuffer);
   }
   /*
    *  Center the base in the middle of our screen.
    */
   bLeft = rect.xRight - BASELEN;
   if(bLeft < 0L)
      bLeft = 0L;
   DrawRect( bLeft / 2,
             (rect.yTop - BASETHICK) / 2,
             (rect.xRight + BASELEN) / 2,
             (rect.yTop + BASETHICK) / 2,
              CLR_DARKGREEN);

   /* Draw the 3 posts.  */
   lVert   = (rect.yTop - BASETHICK) / 2 + BASETHICK;
   bHeight = lVert + (cTowerSize * DISKSPACE + POSTEXTRAHT);
   for(cnt = 0; cnt < 3; cnt++)
   {
      int LeftPole;

      aPolePos[cnt] = ((rect.xRight - BASELEN) / 2) +
                       ( (BASELEN - (POSTHALF + POSTWIDTH) ) / 2 * cnt);
      LeftPole = aPolePos[cnt];
      if(LeftPole < 0)
         LeftPole = 0;
      if((aPolePos[cnt] + POSTHALF + POSTWIDTH) > 1)
      {
         DrawRect(LeftPole,
                  bHeight,
                  aPolePos[cnt] + POSTHALF + POSTWIDTH,
                  lVert,
                  CLR_DARKGREEN);
      }
      else
      {
         DrawRect(LeftPole,
                  bHeight,
                  aPolePos[cnt] + POSTHALF + POSTWIDTH,
                  lVert,
                  CLR_WHITE);
      }
   }

   /* Place the appropriate disks on each pole. */
   for(cPole = 0; cPole < 3; cPole++)
   {
      BYTE bCnt;                         /* Utility variable             */

      for(bCnt = 0; bCnt < abTowersNdx[cPole]; bCnt++)
      {
         DrawDisk(hps, cPole, bCnt, fDRAW);
      }
   }
   WinEndPaint(hps);
   return;
}   /* MainPaint() */

/****************************************************************\
 *  Main window WM_COMMAND processing procedure
 *--------------------------------------------------------------
 *
 *  Name:    MainCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Calls the appropriate procedures that deal with the
 *           selected menu item.
 *
 *  Usage:   Routine is called whenever a WM_COMMAND message is posted
 *           to the main window.
 *
 *  Method:  a switch statement branches on the id of the menu item
 *           that posted the message and the appropriate action for
 *           that item is taken.  Any menu ids that are not part of
 *           the standard menu set are passed onto the user defined
 *           WM_COMMAND processing procedure.
 *
 *  Returns: Return values are determined by each message
 *
\****************************************************************/
LONG MainCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {
      case IDM_START:
                              /* Set continuation fContinueCalc */
         fContinueCalc = TRUE;
         ulIterations = 0UL;            /* Zero iteration counter */
         if(DosCreateThread(&tidCalcThread, (PFNTHREAD)CalcThread,
                                   0UL, 0UL, STACK))
            DosBeep(440, 175);
         else               /* Disallow menu items that could change data */
         {                          /* while the second thread is running */
            EnableMenuItem(hwnd, IDM_START, FALSE);  /* Disable Start item */
            EnableMenuItem(hwnd, IDM_SET,   FALSE);  /* Disable Set item   */
            EnableMenuItem(hwnd, IDM_STOP,  TRUE);   /* Enable Stop item   */
         }
         break;

      case IDM_STOP:
         fContinueCalc = FALSE;       /* Notify thread to quit */
         break;

      case IDM_SET:                /* Pop up the query/set box */
         if(WinDlgBox(HWND_DESKTOP, hwnd,
                   (PFNWP)EntryFldDlgProc, NULLHANDLE, IDD_SETCOUNT, NULL))
         {
            SetupTowers();                     /* Reset towers */
            WinInvalidateRect(hwnd, NULL, FALSE);
         }
         break;

      case IDM_ON:
         fViewOn = TRUE;
         WinInvalidateRect(hwnd, NULL, FALSE);
         break;

      case IDM_OFF:
         fViewOn = FALSE;
         WinInvalidateRect(hwnd, NULL, FALSE);
         break;

      case IDM_HELPHELPFORHELP:
         HelpHelpForHelp(mp2);
         break;

      case IDM_HELPEXTENDED:
         HelpExtended(mp2);
         break;

      case IDM_HELPKEYS:
         HelpKeys(mp2);
         break;

      case HM_QUERY_KEYS_HELP:
         return PANEL_KEYSHELP;

      case IDM_HELPINDEX:
         HelpIndex(mp2);
         break;

      case IDM_HELPPRODUCTINFO:
         HelpAbout(mp2);
         break;
      default:
         break;
   }
   return 0L;
}         /*  MainCommand()  */

/****************************************************************\
 *  Creates the Background thread
 *--------------------------------------------------------------
 *
 *  Name:    CreateBackgroundThread(VOID)
 *
 *  Purpose: Routine creates the background thread
 *
 *  Usage:   Routine is called at initialization time to create a
 *           background processing thread.
 *
 *  Method:  The routine calls DosCreateThread with the Background
 *           thread routine.
 *
 *  Returns: TRUE if thread is created successfully.
 *           FALSE if not or if the thread was already created
 *
\****************************************************************/
BOOL CreateBackgroundThread(VOID)
{
   ULONG sRet;

   /*------------------------------------------------------------------*
    *  Note: C-runtime considerations:
    *  If you use non-reentrant C runtime functions in the second thread,
    *  then you will need to use _beginthread and _endthread instead of
    *  DosCreateThread and DosExit.
    *------------------------------------------------------------------*/
   if(!fThreadCreated)
   {
      DosCreateEventSem((PSZ)NULL, &hevThreadInit, 0UL, FALSE);
      sRet = DosCreateThread(&tidBkThread,
                             (PFNTHREAD)BackgroundThread,
                             0UL,
                             0UL,
                             THREADSTACKSIZE);
   }
   else
   {
      return FALSE;
   }

                  /* wait until the thread has finished initialization */
   if(DosWaitEventSem(hevThreadInit, SEM_TIMEOUT))
      return FALSE;

    return (BOOL)(sRet == 0UL);
}   /* CreateBackgroundThread() */

/****************************************************************\
 *  Destroys the Background thread
 *--------------------------------------------------------------
 *
 *  Name:    DestroyBackgroundThread(VOID)
 *
 *  Purpose: Routine destroys the background thread
 *
 *  Usage:   Routine is called at exit time to destroy the background
 *           processing thread.
 *
 *  Method:  The routine posts a WM_CLOSE message to the object window
 *           to end its message loop.  It then waits to make sure that
 *           the thread has been terminated before it returns.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DestroyBackgroundThread(VOID)
{
   PostBkThreadMsg(WM_CLOSE, MPVOID, MPVOID);

   DosWaitThread(&tidBkThread, 0UL);
   return;
}                                  /* DestroyBackgroundThread() */

/****************************************************************\
 *  DrawDisk procedure
 *--------------------------------------------------------------
 *
 *  Name:    DrawDisk(HPS hps, BYTE cPole, BYTE bLevel, BYTE fDraw)
 *
 *  Purpose: This routine takes a PS handle, the Hanoi spindle and
 *           disk level and draws an appropriately sized disk.
 *
 *  Usage:   Routine is called whenever the disk need to display.
 *
 *  Method:  Does not grab exclusive access to the screen before
 *           drawing.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DrawDisk(HPS hps, BYTE cPole, BYTE bLevel, BYTE fDraw)
{
   int Ystart;
   /* Calculate the disk's width */
   int Width = (MAXDISKWIDTH - MINDISKWIDTH) * abTowers[cPole][bLevel]
              / cTowerSize + MINDISKWIDTH;
   /* Center disk on pole */
   int Xstart = (aPolePos[cPole] - Width / 2) + DISKTHICK;
   int Xend   = Xstart + Width;
   POINTL ptl;
   RECTL  rect;             /* Rectangle struct for painting */

   /* Calculate Bottom of disk */
   WinQueryWindowRect(hwndMain, &rect);
   Ystart = (rect.yTop + BASETHICK) / 2 + (DISKSPACE * bLevel);

   if(aPolePos[cPole] < (Width / 2))
      Xstart = 0;             /* disk's left edge is outside space */

   if(fDraw == fDRAW)             /* If we are to draw the disk */
   {
      DrawRect(Xstart, Ystart, Xend,
               Ystart + DISKTHICK - 1, CLR_RED);
   }
   else       /* We are to erase the disk, then redraw the pole */
   {
      int LeftPole;

      DrawRect(Xstart, Ystart, Xend,
               Ystart + DISKTHICK - 1, CLR_WHITE);

      LeftPole = aPolePos[cPole];
      if(LeftPole < 0)
         LeftPole = 0;
      if((aPolePos[cPole] + POSTHALF + POSTWIDTH) > 0)
         DrawRect(LeftPole,
                  Ystart,
                  aPolePos[cPole] + POSTHALF + POSTWIDTH,
                  Ystart + DISKTHICK - 1,
                  CLR_DARKGREEN);
   }
   return;
}

/****************************************************************\
 *  CalcThread procedure
 *--------------------------------------------------------------
 *
 * Name:    CalcThread(VOID)
 *
 * Purpose: Calls the recursive Hanoi with initial setting of 0, 2, 1
 *          meaning from pole 0, to pole 2, using pole 1 as a temporary.
 *          Hanoi returns when finished, or the user says stop.
 *          This proc then sets a critical section so the posted
 *          message won't be handled until the thread is terminated.
 *
 * Usage:   Routine is called whenever a IDM_START message is posted to
 *          the main window.
 *
 * Method:
 *
 * Returns: VOID
 *
\****************************************************************/
VOID CalcThread(VOID)
{                                    /* Anchor block for the process */
   HAB habb = WinInitialize(0UL);     /* Called to increase Ring 2 stack size */

          /* Execute the recursive routine */
   Hanoi(cTowerSize, (BYTE)0, (BYTE)2, (BYTE)1);
   WinTerminate(habb);

   DosEnterCritSec(); /* Set critical so that IDM_DONE isn't processed */
                      /* until this thread has completely terminated   */
   WinPostMsg(hwndMain, IDM_DONE, MPVOID, MPVOID);

   DosExitCritSec();             /* Restore normal thread dispatching */

   DosExit(EXIT_THREAD, 0UL);    /* Terminate thread */
   return;
}       /*  CalcThread()  */

/****************************************************************\
 *  EnableMenuItem procedure
 *--------------------------------------------------------------
 *
 * Name:    EnableMenuItem(HWND hwnd, LONG sMenuItem, BOOL fEnable)
 *
 * Purpose: This routine handles enabling/disabling of menu items.  This
 *          is done by getting Parent and Menu hwnd handles then sending
 *          the appropriate message.
 *
 * Usage:   Routine is called to enable or disable one item from menu.
 *
 * Method:
 *
 * Returns: VOID
 *
\****************************************************************/
VOID EnableMenuItem(HWND hwnd, LONG sMenuItem, BOOL fEnable)
{
   HWND hwndParent = WinQueryWindow(hwnd, QW_PARENT);
   HWND hwndMenu   = WinWindowFromID(hwndParent, FID_MENU);

   WinSendMsg(hwndMenu, MM_SETITEMATTR,
              MPFROM2SHORT(sMenuItem, TRUE),
              MPFROM2SHORT(MIA_DISABLED, fEnable ? 0 : MIA_DISABLED));
   return;
}     /*  EnableMenuItem()  */

/****************************************************************\
 *  Hanoi recursion procedure
 *--------------------------------------------------------------
 *
 *  Name:    Hanoi(BYTE bHeight, BYTE bFrom, BYTE bTo, BYTE bTemp)
 *
 *  Purpose: This routine implements a recursive Hanoi program that
 *           works as follows:  By recursion, move all the disks,
 *           except for the bottom disk to the temporary stack.
 *           Then move the bottom disk to the target spindle.
 *           Now recursively move the stack on the temporary spindle
 *           to the target spindle.  The limiting case is when Hanoi()
 *           is called with a bHeight of 0 in which case the depth
 *           recursion is terminated.
 *
 *  Usage:
 *
 *  Method:  This routine checks the ->fContinueCalc flag, which is
 *           set by the main thread when the user selects stop, to
 *           see if the user wishes to abort the algorithm.  If so,
 *           it backs out and exits.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID Hanoi(BYTE bHeight, BYTE bFrom, BYTE bTo, BYTE bTemp)
             /* bHeight - The number of disks in the from stack to move */
             /* bFrom   - The from spindle number, 0-2 */
             /* bTo     - The to spindle number, 0-2 */
             /* bTemp   - The temporary spindle number */
{
   HPS    hps;                           /* Handle for painting */
   RECTL  rect;                /* Rectangle struct for painting */
   CHAR   szBuffer[BUFF_SIZE];
   LONG   lIncrement = 1L;
   POINTL ptl;
   int    Center;
   FONTMETRICS FontMetrics;

   if(bHeight <= 0 || !fContinueCalc)       /* Exit up if no more */
      return;                    /* disks or the user said Stop */

   Hanoi((BYTE)(bHeight - 1),         /* Move all but bottom disk */
         (BYTE)bFrom,
         (BYTE)bTemp,
         (BYTE)bTo);

   if(!fContinueCalc)                   /* If user said to stop */
      return;
   /* Display bFrom -> bTo */
   hps = WinGetPS(hwndMain);
   MoveDisk(hps, bFrom, bTo);             /* Move the bottom disk */
   if (fViewOn)
   {
      WinQueryWindowRect(hwndMain, &rect);
      GpiQueryFontMetrics(hps, sizeof(FontMetrics), &FontMetrics);
      /*
       *  Set center.
       */
      Center = rect.xRight / 2;
      /*
       *left hand side of text to blank out
       */
      rect.xLeft = ptl.x = (Center +
                            ((signed long)(strlen(pszDisksMoved)) / 2)
                             * FontMetrics.lAveCharWidth);

      /*
       *   Set bottom.
       */
      ptl.y = (rect.yTop - BASETHICK - 8 * FontMetrics.lEmHeight)  /2;
      /*
       *top
       */
      rect.yTop = (rect.yTop - BASETHICK - 6 * FontMetrics.lEmHeight)  /2;


      rect.yBottom = ptl.y;
      /*
       *  Clear off the previous numbers by drawing an opaque rectangle.
       */
      memset((void *)szBuffer, (int)' ', sizeof(szBuffer));

      GpiCharStringPosAt(hps,
                         &ptl,
                         &rect,
                         (ULONG)CHS_OPAQUE,
                         DELETE_WIDTH,
                         szBuffer,
                         &lIncrement);

      szBuffer[DELETE_WIDTH] = '\0';
      GpiCharStringAt(hps, &ptl, (signed long)strlen(szBuffer), szBuffer);
      strcpy(szBuffer, pszDisksMoved);
      /*
       *  Display the number of disks moved.
       */
      /*
       *  Initial text to draw is "Disks moved:"
       */
      ptl.x = Center - ((((int)strlen(pszDisksMoved) + MAX_DIGITS) / 2)* FontMetrics.lAveCharWidth);
      GpiCharStringAt(hps, &ptl, (signed long)strlen(szBuffer), szBuffer);
      sprintf(szBuffer, "%5lu", ulIterations + 1 );

      ptl.x = Center +
              ((((int)strlen(pszDisksMoved) / 2) + 1) * FontMetrics.lAveCharWidth);
      GpiCharStringAt(hps, &ptl, (int)strlen(szBuffer), szBuffer);
   }
   WinReleasePS(hps);
   ulIterations++;

   /* Move disks over */
   Hanoi((BYTE)(bHeight - 1), (BYTE)bTemp, (BYTE)bTo, (BYTE)bFrom);
   return;
}      /*  Hanoi()  */

/****************************************************************\
 *  SetupTowers procedure
 *--------------------------------------------------------------
 *
 *  Name:    SetupTowers(VOID)
 *
 *  Purpose: This routine initializes the global arrays that represent
 *           the Hanoi stacks.  This involves placing all the disks on
 *           the first peg, emptying the other 2 pegs and setting the
 *           associated counts.
 *
 *  Usage:   Routine is called when program started or restarted.
 *
 *  Method:  Calling uses the global variable cTowerSize to determine
 *           how many disks there are.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID SetupTowers(VOID)
{
   int cnt;

   for(cnt = 0; cnt < cTowerSize; cnt++)   /* Set up initial post with disks */
      abTowers[0][cnt] = (unsigned char)(cTowerSize - (BYTE)cnt - (BYTE)1);

   abTowersNdx[0] = (BYTE)cTowerSize;    /* Set disk count for initial post */
   abTowersNdx[1] = abTowersNdx[2] = (BYTE)0;     /* Zero other post counts */
   return;
}   /*  SetupTowers()  */

/****************************************************************\
 *  MoveDisk procedure
 *--------------------------------------------------------------
 *
 *  Name:    MoveDisk(HPS hps, BYTE bFrom, BYTE bTo)
 *
 *  Purpose: This routine moves the top disk from the bFrom spindle
 *           to the top of the bTo spindle.
 *
 *  Usage:
 *
 *  Method:  Does error checking for trying to move a disk from a
 *           spindle that does not have any disks on it.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID MoveDisk(HPS hps, BYTE bFrom, BYTE bTo)
                        /* hps - Handle for painting */
                        /* bFrom - The from spindle number, 0-2 */
                        /* bTo - The to spindle number, 0-2 */
{
   BYTE bDiskNum;                       /* Disk number to move */
   CHAR bTOSndx = (unsigned char)(abTowersNdx[bFrom] - 1);    /* Top of stack index */

   DrawDisk(hps, bFrom, bTOSndx, fERASE);  /* Remove disk off from stack */

   bDiskNum = abTowers[bFrom][bTOSndx]; /* Get move disk number */
   abTowersNdx[bFrom]--;                /* Decrease count on from spindle */

   /* Offset to place the disk at Place on stack in memory */
   bTOSndx = abTowersNdx[bTo]++;
   abTowers[bTo][bTOSndx] = bDiskNum;
                                        /* Draw disk on the to stack */
   DrawDisk(hps,bTo,bTOSndx,fDRAW);
   return;
}    /*  MoveDisk()  */

/****************************************************************\
 *  EntryFldDlgProc procedure
 *--------------------------------------------------------------
 *
 *  Name:   EntryFldDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *
 * Purpose: Handles all the messages associated with the set entry field
 *          and calls the appropriate handling procedures.  The purpose
 *          of this dialog box is to get a new number of disks for the
 *          Hanoi routine.
 *
 * Usage:
 *
 * Method:  If the value entered is valid, global cTowerSize is changed
 *          to the new value, and TRUE is returned.
 *
 * Returns: Terminates with a TRUE if a new valid Tower Size has been entered.
 *
\****************************************************************/
MRESULT EXPENTRY EntryFldDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   short NewSize = (short)0;                  /* Holds new number of disks */

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);

         WinSendDlgItemMsg(hwnd, IDD_ENTRYFLD, EM_SETTEXTLIMIT,
                           MPFROM2SHORT(2,0), MPVOID);  /* Limit length */
         WinSetDlgItemShort(hwnd, IDD_ENTRYFLD, cTowerSize, TRUE);
         return (MRESULT)NULL;           /* Allow normal focus setting */

      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
         {
            case DID_OK:
               WinQueryDlgItemShort(hwnd, IDD_ENTRYFLD, &NewSize, TRUE);
               if(NewSize > (short)0 && NewSize <= MAXDISKCNT) /* Set new Tower size */
               {
                  cTowerSize = (BYTE)NewSize;
                  WinDismissDlg(hwnd, TRUE);
               }
               else                    /* Invalid value */
                  MessageBox(hwndMainFrame,
                             IDMSG_DISKNUMBERERROR,
                             MB_OK | MB_ICONEXCLAMATION,
                             TRUE);
               return (MRESULT)NULL;

            case DID_CANCEL:
               WinDismissDlg(hwnd, FALSE);
               return (MRESULT)NULL;

            default:
               return WinDefDlgProc(hwnd, msg, mp1, mp2);
         }

      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   }
}    /*  EntryFldDlgProc()  */

/****************************************************************\
 *  Dialog procedure for the About dialog box
 *--------------------------------------------------------------
 *
 *  Name:    AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Handles all the messages associated with the About Box
 *
 *  Usage:   Called for each message sent to the About Box dialog box.
 *
 *  Method:  The about box only has a button control so this routine
 *           only processes WM_COMMAND messages. Any WM_COMMAND
 *           posted must have come from the Ok button so we dismiss
 *           the dialog upon receiving it.
 *
 *  Returns: Dependent upon message sent
 *
\****************************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         return (MRESULT)NULL;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)NULL;

      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   }
}        /*  AboutDlgProc()  */

/****************************************************************\
 *  FixSysMenu procedure
 *--------------------------------------------------------------
 *
 *  Name:    FixSysMenu(HWND hwndDlg)
 *
 *  Purpose: This routine removes the Restore, Size, Minimize, and
 *           Maximize options from the system menu of a dialog.
 *
 *  Usage:
 *
 *  Method:  Called during the WM_INITDLG of a dialog procedure.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
   HWND hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);

   WinSendMsg(hwndMenu,  /* Delete Restore from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_RESTORE, TRUE),
              MPVOID);

   WinSendMsg(hwndMenu,     /* Delete Size from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_SIZE, TRUE),
              MPVOID);

   WinSendMsg(hwndMenu, /* Delete Minimize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MINIMIZE, TRUE),
              MPVOID);

   WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MAXIMIZE, TRUE),
              MPVOID);
   return;
}             /*  FixSysMenu()  */

/****************************************************************\
 *  Message Box procedure
 *--------------------------------------------------------------
 *
 *  Name:    MessageBox(HWND hwndOwner, LONG IdMsg, LONG fsStyle, BOOL fBeep)
 *
 *  Purpose: Displays the message box with the message given in idMsg
 *           retrieved from the message table and using the style
 *           flags in fsStyle
 *
 *  Usage:   Called whenever a MessageBox is to be displayed
 *
 *  Method:  - Message string is loaded from the process' message table
 *           - Alarm beep is sounded if desired
 *           - Message box with the message is displayed
 *           - WinMessageBox return value is returned
 *
 *  Returns: Return value from WinMessageBox()
 *
\****************************************************************/
ULONG MessageBox(HWND hwndOwner, LONG IdMsg, LONG fsStyle, BOOL fBeep)
           /* hwndOwner - Handle of the message box's owner */
           /* idMsg - ID if the message in the message table */
           /* fsStyle - Style of the message box */
           /* fBeep - If TRUE, beep before message box is displayed */
{
   CHAR szText[MESSAGELEN];

   if(!WinLoadMessage(hab,
                      NULLHANDLE,
                      (unsigned long)IdMsg,
                      MESSAGELEN,
                      (PSZ)szText))
   {
      WinAlarm(HWND_DESKTOP, WA_ERROR);
      return RETURN_ERROR;
   }

   if(fBeep)
      WinAlarm(HWND_DESKTOP, WA_ERROR);

   return WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        (PSZ)NULL,
                        IDD_MSGBOX,
                        (unsigned long)fsStyle);
}   /* MessageBox() */

/****************************************************************\
 *  Routine for initializing the help manager
 *--------------------------------------------------------------
 *
 *  Name:    InitHelp(VOID)
 *
 *  Purpose: Initializes the IPF help facility
 *
 *  Usage:   Called once during initialization of the program
 *
 *  Method:  Initializes the HELPINIT structure and creates the help
 *           instance. If successful, the help instance is associated
 *           with the main window
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID InitHelp(VOID)
{
   HELPINIT hini;

   /* If we return because of an error, Help will be disabled */
   fHelpEnabled = FALSE;
                             /* Initialize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0UL;
                             /* If tutorial added, add name here */
   hini.pszTutorialName = (PSZ)NULL;

   hini.phtHelpTable = (PHELPTABLE)MAKELONG(HANOI_HELP_TABLE, 0xFFFF);
   hini.hmodHelpTableModule = NULLHANDLE;
   hini.hmodAccelActionBarModule = NULLHANDLE;
   hini.idAccelTable = 0;
   hini.idActionBar  = 0;

   if(!WinLoadString(hab,
                     NULLHANDLE,
                     IDS_HELPWINDOWTITLE,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szWindowTitle))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
      return;
   }
   hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

                             /* If debugging, show panel ids, else don't */
#ifdef DEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif

   if(!WinLoadString(hab,
                     NULLHANDLE,
                     IDS_HELPLIBRARYNAME,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szLibName))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
      return;
   }
   hini.pszHelpLibraryName = (PSZ)szLibName;
                   /* Creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(hwndHelpInstance == NULLHANDLE || hini.ulReturnCode)
   {
      MessageBox(hwndMainFrame, IDMSG_HELPLOADERROR, MB_OK | MB_ERROR, TRUE);
      return;
   }
                             /* Associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndMainFrame))
   {
      MessageBox(hwndMainFrame, IDMSG_HELPLOADERROR, MB_OK | MB_ERROR, TRUE);
      return;
   }

   /* Help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
   return;
}   /* InitHelp() */

/****************************************************************\
 *  Processes the Help for Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpHelpForHelp(MPARAM mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Help for
 *           Help item of the Help menu
 *
 *  Usage:   Called from MainCommand when the Help for Help menu item
 *           is selected
 *
 *  Method:  Sends an HM_DISPLAY_HELP message to the help instance so
 *           that the default Help For Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpHelpForHelp(MPARAM mp2)
{
           /* This just displays the system help for help panel */
   if(fHelpEnabled)
      if((LONG)WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP, MPVOID, MPVOID))
         MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, MB_OK | MB_ERROR, FALSE);

   /* This routine currently doesn't use the mp2 parameter but
    *  it is referenced here to prevent an 'Unreferenced Parameter'
    *  warning at compile time.
    */
   mp2;
   return;
}   /* HelpHelpForHelp() */

/****************************************************************\
 *  Processes the Extended Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpExtended(MPARAM mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Extended
 *           Help item of the Help menu.
 *
 *  Usage:   Called from MainCommand when the Extended Help menu item
 *           is selected.
 *
 *  Method:  Sends an HM_EXT_HELP message to the help instance so that
 *           the default Extended Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpExtended(MPARAM mp2)
{
           /* This just displays the system extended help panel */
   if(fHelpEnabled)
      if((LONG)WinSendMsg(hwndHelpInstance, HM_EXT_HELP, MPVOID, MPVOID))
         MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, MB_OK | MB_ERROR, FALSE);

    /* This routine currently doesn't use the mp2 parameter but
     *  it is referenced here to prevent an 'Unreferenced Parameter'
     *  warning at compile time.
     */
   mp2;
   return;
}   /* HelpExtended() */

/****************************************************************\
 *  Processes the Keys Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpKeys(MPARAM mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Keys Help
 *           item of the Help menu.
 *
 *  Usage:   Called from MainCommand when the Keys Help menu item is
 *           selected
 *
 *  Method:  Sends an HM_KEYS_HELP message to the help instance so that
 *           the default Keys Help is displayed.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpKeys(MPARAM mp2)
{
               /* This just displays the system keys help panel */
   if(fHelpEnabled)
      if((LONG)WinSendMsg(hwndHelpInstance, HM_KEYS_HELP, MPVOID, MPVOID))
         MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, MB_OK | MB_ERROR, FALSE);

    /* This routine currently doesn't use the mp2 parameter but
     *  it is referenced here to prevent an 'Unreferenced Parameter'
     *  warning at compile time.
     */
   mp2;
   return;
}   /* HelpKeys() */

/****************************************************************\
 *  Processes the Index Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpIndex(MPARAM mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the Index Help
 *           item of the Help menu
 *
 *  Usage:   Called from MainCommand when the Index Help menu item
 *           is selected
 *
 *  Method:  Sends an HM_INDEX_HELP message to the help instance so
 *           that the default Index Help is displayed.
 *
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpIndex(MPARAM mp2)
{
              /* This just displays the system help index panel */
   if(fHelpEnabled)
      if((LONG)WinSendMsg(hwndHelpInstance, HM_HELP_INDEX, MPVOID, MPVOID))
         MessageBox(hwndMain, IDMSG_HELPDISPLAYERROR, MB_OK | MB_ERROR, FALSE);

    /* This routine currently doesn't use the mp2 parameter but
     *  it is referenced here to prevent an 'Unreferenced Parameter'
     *  warning at compile time.
     */
   mp2;
   return;
}   /* HelpIndex() */

/****************************************************************\
 *  Processes the About command from the Help menu
 *--------------------------------------------------------------
 *
 *  Name:    HelpAbout(MPARAM mp2)
 *
 *  Purpose: Processes the WM_COMMAND message posted by the About item
 *           of the Help menu
 *
 *  Usage:   Called from MainCommand when the About menu item is selected
 *
 *  Method:  Calls WinDlgBox to display the about box dialog.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID HelpAbout(MPARAM mp2)
{
                                 /* Display the AboutBox dialog */
   WinDlgBox(HWND_DESKTOP,
             hwndMain,
             (PFNWP)AboutDlgProc,
             NULLHANDLE,
             IDD_PRODUCTINFO,
             NULL);

    /* This routine currently doesn't use the mp2 parameter but
     *  it is referenced here to prevent an 'Unreferenced Parameter
     *  warning at compile time.
     */
   return;
}   /* HelpAbout() */


/****************************************************************\
 *  Displays the help panel indicated
 *--------------------------------------------------------------
 *
 *  Name:    DisplayHelpPanel(LONG idPanel)
 *
 *  Purpose: Displays the help panel whose id is given
 *
 *  Usage:   Called whenever a help panel is desired to be displayed,
 *           usually from the WM_HELP processing of the dialog boxes
 *
 *  Method:  Sends HM_DISPLAY_HELP message to the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DisplayHelpPanel(LONG idPanel)
                        /* ID of the help panel to be displayed */
{
   if(fHelpEnabled)
      if(WinSendMsg(hwndHelpInstance,
                    HM_DISPLAY_HELP,
                    MPFROMLONG(MAKELONG(idPanel, NULL)),
                    MPFROMSHORT(HM_RESOURCEID)))
         MessageBox(hwndMainFrame,
                    IDMSG_HELPDISPLAYERROR,
                    MB_OK | MB_ERROR, TRUE);
    return;
}   /* DisplayHelpPanel() */


/****************************************************************\
 *  Destroys the help instance
 *--------------------------------------------------------------
 *
 *  Name:    DestroyHelpInstance(VOID)
 *
 *  Purpose: Destroys the help instance for the application
 *
 *  Usage:   Called after exit from message loop
 *
 *  Method:  Calls WinDestroyHelpInstance() to destroy the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DestroyHelpInstance(VOID)
{
    if(hwndHelpInstance != NULLHANDLE)
    {
       WinDestroyHelpInstance(hwndHelpInstance);
    }
    return;

}   /* DestroyHelpInstance() */


/****************************************************************\
 *  Posts a message to the Background thread
 *--------------------------------------------------------------
 *
 *  Name:    PostBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Routine posts a message to the object window of the
 *           background thread.
 *
 *  Usage:   Routine is called whenever a message is to be posted to
 *           the background processing thread.
 *
 *  Method:  The routine posts the message to the object window of the
 *           thread.
 *
 *  Returns:  the return value from WinPostMsg().
 *
\****************************************************************/
BOOL PostBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2)
{
   BOOL sRC = WinPostMsg(hwndObject, msg, mp1, mp2);

   return sRC;
}   /* PostBkThreadMsg() */


/****************************************************************\
 *  Background thread routine
 *--------------------------------------------------------------
 *
 *  Name:    BackgroundThread(ULONG ulThreadParam)
 *
 *  Purpose: Routine is a background thread used for tasks to be
 *           completed in the background.
 *
 *  Usage:   Routine is called at initialization time to create a
 *           background processing thread.
 *
 *  Method:  The routine initializes itself as a PM thread and creates
 *           a message queue.  It then creates an object window through
 *           which it will receive and send messages.  It then polls
 *           through a message loop, processing any messages it receives
 *           in the object window's window procedure. When the loop
 *           ends, it terminates.
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID BackgroundThread(ULONG ulThreadParam)
                     /* Parameter passed into DosCreateThread() */
{
   QMSG qmsg;
                            /* Create message queue for thread */
   habBkThread = WinInitialize(0UL);

   do
   {
      if(habBkThread == NULLHANDLE)
      {
         WinPostMsg(hwndMain, TM_THREADINITFAILED, MPVOID, MPVOID);
         DosExit(EXIT_THREAD, 0UL);
      }
      hmqBkThread = WinCreateMsgQueue(habBkThread, 0L);
      if(hmqBkThread == NULLHANDLE)
      {
         WinPostMsg(hwndMain, TM_THREADINITFAILED, MPVOID, MPVOID);
         WinTerminate(habBkThread);
         DosExit(EXIT_THREAD, 0UL);
      }

        /*
         *  Load the string for the object window class and register the class.
         */
      if(!WinLoadString(habBkThread,
                        NULLHANDLE,
                        IDS_OBJECTCLASS,
                        MAXNAMEL,
                        (PSZ)szObjectClass))
      {
         WinPostMsg(hwndMain, TM_THREADINITFAILED, MPVOID, MPVOID);
         break;
      }
        /* Register the main client window class */
      if(!WinRegisterClass(habBkThread,
                          (PSZ)szObjectClass,
                          (PFNWP)ObjectWndProc,
                          CS_SIZEREDRAW | CS_CLIPCHILDREN,
                          0UL))
      {
         WinPostMsg(hwndMain, TM_THREADINITFAILED, MPVOID, MPVOID);
         break;
      }

        /* Create the object window */
      hwndObject = WinCreateWindow(HWND_OBJECT,
                                     szObjectClass,
                                     NULL,
                                     0UL,
                                     0L,
                                     0L,
                                     0L,
                                     0L,
                                     NULLHANDLE,
                                     HWND_TOP,
                                     OBJECTID,
                                     NULL,
                                     NULL);
      if(hwndObject == NULLHANDLE)
      {
         WinPostMsg(hwndMain, TM_THREADINITFAILED, MPVOID, MPVOID);
         break;
      }

        /*
         *  Set thread created flag so another thread of this type
         *  cannot be created.
         */
      fThreadCreated = TRUE;

                   /* Clear initialization semaphore.   */
      DosPostEventSem(hevThreadInit);

                                                    /* Message loop */
      while(WinGetMsg(hmqBkThread, &qmsg, NULLHANDLE, 0UL, 0UL))
         WinDispatchMsg(hmqBkThread, &qmsg);

   }ONCE;

   /* Destroy object window, clean up message queue and terminate */
   if(WinIsWindow(habBkThread, hwndObject))
       WinDestroyWindow(hwndObject);

   WinDestroyMsgQueue(hmqBkThread);
   WinTerminate(habBkThread);

   /* If termination is due to an error initializing the thread, then clear
      the initialization semaphore so that the main thread can continue. */

   if(!fThreadCreated)
      DosPostEventSem(hevThreadInit);
   DosExit(EXIT_THREAD, 0UL);
                /* The thread parameter is not currently used */
   return;
}   /* BackgroundThread() */


/****************************************************************\
 *  Dialog procedure for the Object window
 *--------------------------------------------------------------
 *
 *  Name:    ObjectWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Processes all messages sent to the Object window
 *
 *  Usage:   Called for each message sent to the Object window.
 *
 *  Method:  The Object window processes the messages that tell the
 *           background thread what action to take. Since the object
 *           window is not visible, it will not process any of the
 *           standard window messages.
 *
 *  Returns: Dependent upon message sent
 *
\****************************************************************/
MRESULT EXPENTRY ObjectWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT  sRC;

   switch(msg)
   {
      /*-------------------------------------------------------*\
       *  Include here any user defined messages to determine
       *  which action the background thread should take.
      \*-------------------------------------------------------*/
   default:
      sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
   }
   return sRC;
}   /* ObjectWndProc() */

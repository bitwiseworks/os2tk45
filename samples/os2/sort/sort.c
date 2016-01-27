/*=========================================================================*\
 *                                                                         *
 * SORT.C - Multi-threaded PM sample.                                      *
 *                                                                         *
 *                                                                         *
 *      (C) Copyright IBM Corporation 1992.                                *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have      *
 *      been advised of the possibility of   such damages.                 *
 *-------------------------------------------------------------------------*
 *
 *   This program demonstrates the use of many threads by doing
 *   LISTCNT simultaneous sorts.  Each sorting algorithm runs
 *   from a separate thread, the routine which updates the display
 *   is run from another thread, and the main thread is used to
 *   handle the main window's messages.  The display thread is
 *   started when the program begins and is not terminated (allows
 *   default cleanup to terminate thread) as the display routine
 *   is used throughout the program's life.
 *   This code was written to allow easy modification.  To change
 *   the number of simultaneous sorts, change the LISTCNT #define
 *   and modify the global arrays which control the screen
 *   drawing color, the sort each thread is to use, and the sort name.
 *   Note:  This program was not intended to infer the relative
 *          speeds of the various sorting algorithms.
 *
 *---------------------------------------------------------------
 *
 *   Procedures in this file:
 *     main()             Sets up the PM environment and calls
 *                        the main dialog procedure ClientWndProc
 *     ClientWndProc()    Handles the main window messages
 *     CalcThread()       Generic stub that sets up, executes, and
 *                        terminates each sort in an aux. thread
 *     DispThread()       Updates the display during the sorts
 *     BubbleSort()       Implements a bubble sort
 *     InsertionSort()    Implements an insertion sort
 *     BatcherSort()      Stub that calls recursive Batcher sort
 *     BatcherSortR()     Implements a Batcher sort
 *     QuickSort()        Stub that calls recursive Quick sort
 *     QuickSortR()       Implements a Quick sort
 *     EnableMenuItem()   Activates/deactivates a menu choice
 *     EntryFldDlgProc()  Handles the set number of disks dialog box
 *     RandomizeData()    Randomizes the data arrays for the sorts
 *     FixSysMenu()       Remove Restore, Size, Min, & Max from system menu
 *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#define INCL_WIN
#define INCL_WINHEAP
#define INCL_HELP
#define INCL_WINDIALOGS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include "sort.h"

#ifdef RAND_MAX
   #undef  RAND_MAX
   #define RAND_MAX 32767
#endif

/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/

MRESULT EXPENTRY ClientWndProc  (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY EntryFldDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
LONG    MessageBox(HWND, LONG, LONG, BOOL);
VOID APIENTRY ExitProc(ULONG);
BOOL Init(VOID);

INT main(LONG ,CHAR **);
VOID _Optlink CalcThread(PVOID);
VOID _Optlink DispThread(PVOID);

VOID EnableMenuItem(HWND, LONG, BOOL);
VOID RandomizeData(ULONG);
VOID BubbleSort(PCALCPARAM);
VOID BatcherSort(PCALCPARAM);
VOID BatcherSortR(PCALCPARAM, ULONG, ULONG, ULONG, BOOL);
VOID QuickSort(PCALCPARAM);
VOID QuickSortR(PCALCPARAM, LONG, LONG);
VOID InsertionSort(PCALCPARAM);
VOID FixSysMenu(HWND);
VOID InitHelp(VOID);
VOID HelpHelpForHelp(MPARAM);
VOID HelpExtended(MPARAM);
VOID HelpKeys(MPARAM);
VOID HelpIndex(MPARAM);
VOID HelpAbout(MPARAM);
VOID DisplayHelpPanel(LONG);
VOID DestroyHelpInstance(VOID);
HEV  hev;
RECTL  rcl;              /* Rectangle struct for painting  */

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
CHAR szClientClass[40];
USHORT Data[LISTCNT][MAX_ELEMS];     /* Array that contains the data to sort  */

/* Global data that defines each of the sort routines */

                              /* Color of each of the LISTCNT data lists.  */
ULONG  ulColors[LISTCNT] = { CLR_RED,CLR_BLUE,CLR_DARKGREEN,CLR_CYAN };

          /* List of sorts to be executed for each of the LISTCNT threads. */
VOID (*pExecSub[LISTCNT])(PCALCPARAM)={BubbleSort,BatcherSort,QuickSort,
                           InsertionSort};
                                                   /* Ascii names of sorts */
CHAR *szSubNames[LISTCNT]={"Bubble Sort",
                           "Batcher Sort",
                           "Quick Sort",
                           "Insertion Sort"};

HAB    hab;                              /* Anchor block for the process */
HMQ    hmq;
BOOL   fHelpEnabled;             /* flag to determine if help is enabled */
CHAR   szUntitled[MESSAGELEN];           /* buffer for "Untitled" string */
HWND   hwndFrame, hwndClient;
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance;
RECTL  rectl;

/****************************************************************\
 * Function name: main(LONG sArgc,CHAR **ppArgv)
 *
 * Parameters:  NONE
 *
 * Returns: Always returns 0
 *
 * Purpose: Sets up the PM environment, calls the main window proc,
 *          handles the window's messages then cleans up and exits.
 *
 * Usage:
\****************************************************************/
INT main(LONG sArgc,CHAR **ppArgv)
{
   QMSG         qmsg;
   CHAR *pszName = "SORT";

   ULONG flFrameFlags = FCF_STANDARD;

   strncpy(szClientClass,pszName,strlen(pszName) );

   /* These PM calls should be error checked */
   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue(hab, 0);

   if(!Init())
   {
       MessageBox(HWND_DESKTOP,
                  IDMSG_INITFAILED,
                  MB_OK | MB_ERROR,
                  TRUE);
       DosExit(EXIT_PROCESS,RETURN_ERROR);
   }


   hwndFrame = WinCreateStdWindow(HWND_DESKTOP,
                                  WS_VISIBLE,
                                  &flFrameFlags,
                                  szClientClass,
                                  NULL,
                                  0L,
                                  (HMODULE)0,
                                  ID_RESOURCE,
                                  &hwndClient);
   if(hwndFrame == NULLHANDLE)
   {
       MessageBox(HWND_DESKTOP,
                  IDMSG_MAINWINCREATEFAILED,
                  MB_OK | MB_ERROR,
                  TRUE);
       DosExit(EXIT_PROCESS,RETURN_ERROR);
   }
   InitHelp();

   while(WinGetMsg(hab,&qmsg,0,0,0))        /* Message loop */
      WinDispatchMsg(hab,&qmsg);

   /* destroy the help instance */
   DestroyHelpInstance();

   DosExit(EXIT_PROCESS,RETURN_SUCCESS);
   return 0L;
}

/****************************************************************\
 *  Initialization routine
 *---------------------------------------------------------------
 *
 *  Name:    Init()
 *
 *  Purpose: Performs initialization functions required before the main
 *           window can be created.
 *
 *  Usage:   Called once before the main window is created.
 *
 *  Method:  - installs the routine ExitProc into the DosExitList chain
 *           - registers all window classes
 *           - performs any command line processing
 *
 *
 *  Returns: TRUE - initialization is successful
 *           FALSE - initialization failed
\****************************************************************/
BOOL Init(VOID)
{

    /* Add ExitProc to the exit list to handle the exit processing. If
     * there is an error, then terminate the process since there have
     * not been any resources allocated yet                          */

    if(DosExitList(EXLST_ADD, ExitProc))
    {
        MessageBox(HWND_DESKTOP,
                   IDMSG_CANNOTLOADEXITLIST,
                   MB_OK | MB_ERROR,
                   TRUE);
        DosExit(EXIT_PROCESS, RETURN_ERROR);
    }

    if(DosCreateEventSem(NULL,&hev,DC_SEM_SHARED,FALSE ) )
    {
        return(FALSE);
    }

     /* load application name from resource file */
    if(!WinLoadString(hab, 0, IDS_APPNAME, MAXNAMEL, szClientClass))
        return FALSE;

    /* load "untitled" string */
    if(!WinLoadString(hab, 0, IDS_UNTITLED, MESSAGELEN, szUntitled))
        return FALSE;
    /* register the main client window class */
    if(!WinRegisterClass(hab,
                        (PSZ)szClientClass,
                        (PFNWP)ClientWndProc,
                        CS_SIZEREDRAW | CS_CLIPCHILDREN,
                        0))
    {
         return FALSE;
    }
    return( TRUE);
}                                                   /* Init() */

/****************************************************************\
 *  Exit list processing procedure
 *--------------------------------------------------------------
 *
 *  Name:    ExitProc(usTermCode)
 *
 *  Purpose: Cleans up certain resources when the application terminates
 *
 *  Usage:   Routine is called by DosExitList when the application exits
 *
 *  Method:  Global resources, such as the main window and message queue,
 *           are destroyed and any system resources used are freed.
 *
 *  Returns: Returns EXLST_EXIT to the DosExitList handler
 *
\****************************************************************/
VOID APIENTRY ExitProc(ULONG usTermCode)
{
   /* destroy the main window if it exists */
   if(WinIsWindow(hab, hwndFrame))  {
       WinDestroyWindow(hwndFrame);
   }

   /*--------------------------------------------------*\
    *      Any other system resources used             *
    *      (e.g. memory or files) should be freed here *
   \*--------------------------------------------------*/

   WinDestroyMsgQueue(hmq);

   WinTerminate(hab);

   DosExitList(EXLST_EXIT, 0L);         /* termination complete */
   return;
}                                           /* ExitProc() */

/****************************************************************\
 * Function name: ClientWndProc()
 *
 * Parameters:  hwnd, msg, mp1, mp2.  Standard PM Dialog Proc params.
 *              No user data is expected in the WM_CREATE.
 *
 * Returns: Returns with WM_QUIT message
 *
 * Purpose: Handles all the messages associated with the main window
 *          and calls the appropriate handling procedures.
 *
 * Usage  : Called only by main()
\****************************************************************/
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   HPS    hps=0;                         /* Handle for painting            */
   POINTL ptl;                           /* Point struct for painting      */
   static CALCPARAM cp[LISTCNT];         /* Struct passed to sort threads  */
   static TID    tidCalc[LISTCNT];       /* Sort threads IDs               */
   static CALCPARAM cpDisp;              /* Struct passed to disp thread   */
   static TID    tidDisplay;             /* Secondary display thread ID    */
   static USHORT usWindowYCoord=1;       /* Holds current client window ht */
   static BYTE   cThreadCnt = 0;         /* Count of sort threads running  */
   static USHORT cSetSize=MAX_ELEMS;     /* User sort set size             */
   USHORT cnt,cnt2,usID;                 /* Utility counters               */
   ULONG  ulBlip;                        /* Holds the marker pos for Paint */
   MRESULT sRC;

   switch(msg)
   {
      case WM_CREATE:
         RandomizeData(cSetSize);        /* Initially randomize data set   */
         /* Start display thread */
         cpDisp.hwnd        = hwnd;
         cpDisp.Array       = &usWindowYCoord;
         cpDisp.pcThreadCnt = &cThreadCnt;
         cpDisp.pcSetSize   = &cSetSize;
         if((tidDisplay = _beginthread(DispThread,
#if __IBMC__
                                       NULL,
#endif
                                       STACKSIZE,
                                       (PVOID) &cpDisp)) == (USHORT)-1)
         {
            WinAlarm(HWND_DESKTOP, WA_ERROR);  /* can't start display thread */
            DosExit(EXIT_PROCESS,1);
         }
         /* Up the display priority an arbitrary amount to cause it to
            update more rapidly and to give it a higher priority than
            the sort threads.*/
         DosSetPriority(PRTYS_THREAD, PRTYC_REGULAR, 5, tidDisplay);

         return (MRESULT)0L;

      case WM_SIZE:
         WinInvalidateRect(hwnd,NULL,FALSE);     /* Force a redraw */
         sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
         return sRC;

      case WM_PAINT:
         hps = WinBeginPaint(hwnd,0,0);   /* Get paint handle      */
         WinQueryWindowRect(hwnd,&rcl);
         if(NELEMS < MAX_ELEMS )
         {
               cSetSize = NELEMS;
         }
         else
         {
               cSetSize = MAX_ELEMS;
         }
         RandomizeData(cSetSize);        /* Initially randomize data set   */
         usWindowYCoord = rcl.yTop;
          /* Draw if screen */
          /* is big enough  */
         if( (XOFFSET + (2 * COLUMNOFFSET))< (rcl.xRight-rcl.xLeft) &&
            (YOFFSET * ROWCNT) < (rcl.yTop-rcl.yBottom ) )
         {
            DrawRect(rcl.xLeft,rcl.yBottom,         /* White out the screen  */
                     rcl.xRight,rcl.yTop,CLR_WHITE);

            DrawLine(XOFFSET, YOFFSET,              /* Draw baseline         */
                     rcl.xRight,YOFFSET,CLR_BLACK);

            DrawLine(XOFFSET, YOFFSET,              /* Draw vertical line    */
                     XOFFSET,rcl.yTop,CLR_BLACK);

            for(cnt=0;cnt<LISTCNT;cnt++)            /* Draw data points      */
            {
               for(cnt2=0;cnt2<cSetSize;cnt2++)
               {
                  ulBlip = (rcl.yTop-YOFFSET-5) * Data[cnt][cnt2] / RAND_MAX
                           + YOFFSET+1;

                  Draw2Pel(XOFFSET+1 + cnt2, ulBlip, ulColors[cnt]);
               }
            }
            for(cnt=0,cnt2= 0;cnt<COLUMNCNT;cnt++)          /* Do bottom legend      */
            {
               if(cnt2 >= ROWCNT )
               {
                    cnt2 = 0;
               }
               ptl.x = XOFFSET + cnt2++ *COLUMNOFFSET;
               /*
                *next row after the last column
                */
               if(cnt >=  (COLUMNCNT / ROWCNT ) )
               {
                    ptl.y = YOFFSET - ( 2 * ROWOFFSET);
               }
               else
               {
                    ptl.y = YOFFSET - ROWOFFSET;
               }
               GpiSetColor(hps,ulColors[cnt]);
               GpiCharStringAt(hps,&ptl,
                               (LONG) strlen(szSubNames[cnt]),
                               szSubNames[cnt]);
            }
         }

         WinEndPaint(hps);
         return (MRESULT)0L;

      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
         {
            case IDM_START:

               /*
                *clear the drawing semaphore
                */
               DosPostEventSem(hev);
               /* Try to get stack space */
               for(cnt=0;cnt<LISTCNT;cnt++)
               {
                  cp[cnt].hwnd          = hwnd;  /* Set the static struct  */
                  cp[cnt].fContinueCalc = TRUE;
                  cp[cnt].pFunc         = pExecSub[cnt];
                  cp[cnt].usID          = cnt;
                  cp[cnt].Array         = Data[cnt];
                  cp[cnt].cArray        = cSetSize;

                  /* Try to start the thread */
                  if( (tidCalc[cnt] = _beginthread(CalcThread,
#if __IBMC__
                                              NULL,
#endif
                                              STACKSIZE,
                                              (PVOID)&cp[cnt])) == (USHORT)-1)
                  {
                     WinAlarm(HWND_DESKTOP, WA_ERROR);
                     return (MRESULT)0L;
                  }
                  if(cThreadCnt++ == 0)  /* When the first thread starts */
                  {
                     /* Disable Start, Set, and Randomize, enable Stop */
                     EnableMenuItem(hwnd,IDM_START,FALSE);
                     EnableMenuItem(hwnd,IDM_SET,FALSE);
                     EnableMenuItem(hwnd,IDM_RANDOM,FALSE);
                     EnableMenuItem(hwnd,IDM_STOP,TRUE);
                  }
               }
               return (MRESULT)0L;

            case IDM_STOP:
               for(cnt=0;cnt<LISTCNT;cnt++)
                  cp[cnt].fContinueCalc = FALSE;  /* Notify thread to quit  */
               return (MRESULT)0L;

            case IDM_RANDOM:
               RandomizeData(cSetSize);                /* Randomize data */
               WinInvalidateRect(hwnd,NULL,FALSE);     /* Force a redraw */
               return (MRESULT)0L;

            case IDM_SET:
               if(WinDlgBox(HWND_DESKTOP, hwnd, /* Pop up the query/set box */
                            EntryFldDlgProc,(HMODULE)0,IDD_SETCOUNT,
                            (VOID FAR *) &cSetSize))
               {
                  WinInvalidateRect(hwnd,NULL,FALSE);     /* Force a redraw */
               }
               return (MRESULT)0L;

            case IDM_GENERALHELP:
               HelpExtended(mp2);
               return (MRESULT)0L;

            case IDM_USINGHELP:
               HelpHelpForHelp(mp2);
               return (MRESULT)0L;

            case IDM_TUTORIAL:
               HelpKeys(mp2);
               return (MRESULT)0L;

            case IDM_HELPINDEX:
               HelpIndex(mp2);
               return (MRESULT)0L;

            case IDM_HELPPRODUCTINFO:
               HelpAbout(mp2);
               return (MRESULT)0L;

            default:
               sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
               return sRC;
         }
         break;

      case IDM_DONE:
         usID = SHORT1FROMMP(mp1);  /* Get ID of quit thread     */

         if(--cThreadCnt == 0)              /* If all quit, enable menus */
         {
            EnableMenuItem(hwnd,IDM_START,TRUE);
            EnableMenuItem(hwnd,IDM_SET,TRUE);
            EnableMenuItem(hwnd,IDM_RANDOM,TRUE);
            EnableMenuItem(hwnd,IDM_STOP,FALSE);
         }
         return (MRESULT)0L;

      case HM_QUERY_KEYS_HELP:
         return ((MRESULT)PANEL_KEYSHELP);

      case WM_CLOSE:

         DosCloseEventSem(hev);
         for(cnt=0;cnt<LISTCNT;cnt++)
         {
            DosKillThread(tidCalc[cnt]);
         }
         DosKillThread(tidDisplay);
         WinPostMsg(hwnd, WM_QUIT, 0L, 0L);
         return 0L;

      default:
         sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
         return sRC;
   }

   return (MRESULT)0L;
}

/****************************************************************\
 * Function name: CalcThread()
 *
 * Parameters:  pcp is a struct which contains the hwnd handle, the
 *              continue flag which is initially set to TRUE, the ID
 *              which is the
 *
 * Returns: VOID
 *
 * Purpose: This generic stub calls the passed "sort" function with a pointer
 *          to the data and an item count, then when the sort terminates, it
 *          cleans up by Posting a done message, then terminating the thread.
 *
 * Usage  : No DosExitCritSec() is called since _endthread()
 *                 clears the critical section when the thread is
 *                 terminated.
 *
\****************************************************************/
VOID CalcThread(PVOID pcp)
{
   (*((PCALCPARAM)pcp)->pFunc)(pcp);         /* Execute recurs routine */
                           /* Set Crit so the IDM_DONE isn't processed */
                        /* until this thread has completely terminated */
   DosEnterCritSec();

   WinPostMsg(((PCALCPARAM)pcp)->hwnd,IDM_DONE,
              MPFROMSHORT(((PCALCPARAM)pcp)->usID),NULL);/* Post done */

   /*
    *set the post count for each thread
    *that goes through
    */
   DosPostEventSem(hev);

   _endthread();                /* Terminate thread and exit crit sec */
   return;
}

/****************************************************************\
 * Function name: DispThread()
 *
 * Parameters:  pcp is a struct which contains the hwnd handle, the
 *              count of active threads, the ->Array element points to
 *              the current height of the window which can be dynamically
 *              resized by the user, the pcSetSize points to the current
 *              set size which can be changed by the user only when the
 *              sort is stopped.
 *              None of the other pcp members are used.
 *
 * Returns: VOID
 *
 * Purpose: This routine is run as a secondary thread to update the screen
 *          when the sort threads are running.  It works by making passes
 *          through the data lists and redrawing them.  A DosSleep(0) is
 *          called at the end of each pass to help sychronize things a bit.
 *
 * Usage  : Note that this thread is started when the program is
 *                 initialized, and is never terminated.  Note that
 *                 fDoUpdate is used in such a way that the screen is
 *                 ALWAYS updated once after all threads have terminated.
 *                 This avoids the possibility of the display not being
 *                 fully accurate when the sorts terminate.
 *
\****************************************************************/
VOID DispThread(PVOID pvData )
{
   USHORT cnt,cnt2;      /* Utility counters                            */
   HPS hps;              /* Presentation space handle                   */
   POINTL ptl;           /* Used for various drawing macros             */
   ULONG  ulBlip;        /* Holds the Y offset for a given data point   */
   PCALCPARAM pcp;       /* after all sort threads terminate            */
   USHORT *pusYCoord;
   ULONG  ulPostCount;

   pcp = (PCALCPARAM) pvData;
   pusYCoord = pcp->Array;
   /* Points to location that always    */
   /* contains current client wind hgt  */

   hps = WinGetPS( pcp->hwnd);

   while(TRUE)
   {
      /*
       *sit and wait on the clearing of the semaphore
       */
      DosWaitEventSem(hev,SEM_INDEFINITE_WAIT);
      DosQueryEventSem(hev,&ulPostCount);
      /*
       *wait on all 4 threads + the post used to start
       *to post
       */
      if(ulPostCount == 5)
      {
          ulPostCount = 0;
          DosResetEventSem(hev,&ulPostCount);
      }
      for(cnt=0;cnt<*((PCALCPARAM)pcp)->pcSetSize;cnt++)/* Update data set */
      {
         DrawLine(XOFFSET+1 + cnt,YOFFSET+1,      /* Erase vertical column */
                  XOFFSET+1 + cnt,*pusYCoord, CLR_WHITE);

         for(cnt2=0;cnt2<LISTCNT;cnt2++)          /* Draw each point       */
         {
            ulBlip = (ULONG) (*pusYCoord-YOFFSET-5)*Data[cnt2][cnt] / RAND_MAX
                      + YOFFSET+1;
            Draw2Pel(XOFFSET+1 + cnt, ulBlip, ulColors[cnt2]);
         }
      }

   }

   /* Note that these 3 lines NEVER get executed, but should the program be
      modified to end the thread, this is the appropriate termination code. */
   WinReleasePS(hps);
   _endthread();
   return;
}

/****************************************************************\
 * Function name: BubbleSort()
 *
 * Parameters:  pcp is a struct which contains the hwnd handle, a
 *              pointer to the data array, and a count of the items in
 *              the data array.
 *              None of the other pcp members are used.
 *
 * Returns: VOID
 *
 * Purpose:  Implements a bubble sort which is an O(n^2) algorithm.  It
 *           works by repeatedly going through the data and comparing
 *           consecutive elements and swapping them if they aren't in
 *           the correct order.  This guarantees that each pass will
 *           place at least one additional item in its appropriate
 *           place.
 *
 * Usage:
\****************************************************************/
VOID BubbleSort(PCALCPARAM pcp)
{
   BOOL fModified = FALSE; /* Set whenever a swap is done, if an entire */
                           /* pass doesn't set the flag, the we're done */
   INT    cnt;
   INT    cnt2;            /* Counters used for the 2-level loops       */
   USHORT usTemp;          /* Used to hold a data item during a swap    */

   for(cnt=(ULONG)(pcp->cArray-1);cnt>=0;cnt--) /*Set the max no. of passes*/
   {
      for(cnt2=0;cnt2<cnt;cnt2++) /* Only sort thru current cnt pass    */
      {
         if(!pcp->fContinueCalc)  /* User wishes to terminate the sort  */
            return;

         if(pcp->Array[cnt2]>pcp->Array[cnt2+1]) /* Items need to swap  */
         {
            fModified = TRUE;

            usTemp = pcp->Array[cnt2];
            pcp->Array[cnt2] = pcp->Array[cnt2+1];
            pcp->Array[cnt2+1]= usTemp;
         }
      }
      if(!fModified)          /* Nothing changed during the entire pass */
         break;
      fModified = FALSE;                       /* Reset the modify flag */
   }
   return;
}

/****************************************************************\
 * Function name: InsertionSort()
 *
 * Parameters:  pcp is a struct which contains the hwnd handle, a
 *              pointer to the data array, and a count of the items in
 *              the data array.
 *              None of the other pcp members are used.
 *
 * Returns: VOID
 *
 * Purpose:  Implements an insertion sort which is an O(n^2) algorithm.
 *           This sort works much faster and does not require that much
 *           additional code to implement.  It works by setting a sorted
 *           list to be just the first element, the working each
 *           successive item into the already sorted list.
 *
 * Usage:
\****************************************************************/
VOID InsertionSort(PCALCPARAM pcp)
{
   SHORT cnt,cnt2;            /* Counters used for the 2-level loops    */
   USHORT usTemp;             /* Used to hold a data item during a swap */

   for(cnt=1;cnt<(LONG)pcp->cArray;cnt++)  /* Insert each item in turn */
   {
      if(!pcp->fContinueCalc)      /* User wishes to terminate the sort */
         return;
      usTemp=pcp->Array[cnt];                   /* Hold value to insert */
      cnt2=(LONG)(cnt-1);
      while(pcp->Array[cnt2]>usTemp)    /* Move items down to make room */
      {
         pcp->Array[cnt2+1]=pcp->Array[cnt2];
         cnt2--;
         if(cnt2<0)
            break;
      }
      pcp->Array[cnt2+1] = usTemp;                   /* Insert the item */
   }
   return;
}

/****************************************************************\
 * Function name: BatcherSort()
 *
 * Parameters:  pcp is a struct which contains the continue flag,
 *              a pointer to the data array, and a count of the items in
 *              the data array.
 *              None of the other pcp members are used.
 *
 * Returns: VOID
 *
 * Purpose:  This routine is a stub that calls the recursive Batcher sort
 *           with the proper initial arguments.
 *
 * Usage:  It passes the pcp, the size of the array to be sorted,
 *                  the offset to start sorting at(0), the number number
 *                  of elements each item is from neighboring elements (1),
 *                  and Half flag to sort the halves (1=YES).
\****************************************************************/
VOID BatcherSort(PCALCPARAM pcp)
{
   BatcherSortR(pcp,pcp->cArray,0,1,1);
   return;
}

/****************************************************************\
 * Function name: BatcherSortR()
 *
 * Parameters:  pcp is a struct which contains the continue flag,
 *              a pointer to the data array.
 *              None of the other pcp members are used.
 *              usArrSize is the number of elements in the current sort set.
 *              usStart is the offset to the 1st element in the set.
 *              usSkip is the spacing between consecutive elements.
 *              fHalves sorts the 2 halves when set, otherwise skips the
 *                      1st 2 of the 4 sub-sorts.
 *
 * Returns: VOID
 *
 * Purpose:  Implements Batcher sort which is O(n lg n).  The advantage
 *           of the batcher sort is that the comparisons made do NOT
 *           depend upon the outcome of previous comparisons.  This makes
 *           it a good algorithm for parallelism.  The algorithm works as
 *           follows:  Sort the first half, sort the second half, sort the
 *           odd elements, sort the even elements, then compare swap
 *           elements 2/3, 4/5, ...
 *
 * Usage  :  There are several adaptations to the algorithm to
 *                  allow it to work with arbitrary sized data sets
 *                  (the original required a power of 2 sized data
 *                  set):  if the set size is less than 2, the routine
 *                  returns, the first "half" is always the largest
 *                  possible power of two, and the top value for the
 *                  final compare/swap is adjusted to round up in
 *                  case of an odd data set.
 *                  Another optimization is that involving the fHalves
 *                  flag.  This stems from the observation that when
 *                  the odd/even sort recurses, the first and second
 *                  halves are already sorted, thus the first 2
 *                  recursive calls are unnecessary in this case.
 *
\****************************************************************/
VOID BatcherSortR(PCALCPARAM pcp, ULONG usArrSize, ULONG usStart,
                  ULONG usSkip, BOOL fHalves)
{
   USHORT cnt,usUpper,usTemp; /* Utility variables */

   if(!pcp->fContinueCalc) /* User wishes to terminate the sort */
      return;

   if(usArrSize<2) /* No sorting needed if <2 items in the set */
      return;

   if(usArrSize==2) /* Do simple compare/swap if there are 2 elements */
   {
      if(pcp->Array[usStart]>pcp->Array[usStart+usSkip])
      {
         usTemp = pcp->Array[usStart];
         pcp->Array[usStart] = pcp->Array[usStart+usSkip];
         pcp->Array[usStart+usSkip]= usTemp;
      }
      return;
   }

   usTemp=1;                  /* usTemp ends up holding the smallest power */
   while(usTemp < usArrSize)  /* of 2 that is at least as big as usArrSize */
      usTemp *= 2;

   if(fHalves)  /* If the sort was NOT called by the odd/even recurses */
   {
      BatcherSortR(pcp,(ULONG)(usTemp/2),usStart,usSkip,(BOOL)1);    /* Sort 1st half */
      BatcherSortR(pcp,(ULONG)(usArrSize-usTemp/2),       /* Sort 2nd half */
                   (ULONG)(usStart+usTemp/2*usSkip),usSkip,(BOOL)1);
   }
   BatcherSortR(pcp,(ULONG)(usArrSize-usArrSize/2),usStart,(ULONG)(usSkip*2),(BOOL)0); /* Sort evens */
   BatcherSortR(pcp,(ULONG)(usArrSize/2),(ULONG)(usStart+usSkip),(ULONG)(usSkip*2),(BOOL)0);    /* Sort odds  */

   if(!pcp->fContinueCalc) /* User wishes to terminate the sort */
      return;

   usUpper=(ULONG)(usStart+usSkip+(usArrSize-usArrSize/2-1)*2*usSkip);
   for(cnt=usStart+usSkip;cnt<usUpper;cnt+=usSkip*2) /* Do final compares */
   {
      if(pcp->Array[cnt]>pcp->Array[cnt+usSkip])
      {
         usTemp = pcp->Array[cnt];
         pcp->Array[cnt] = pcp->Array[cnt+usSkip];
         pcp->Array[cnt+usSkip]= usTemp;
      }
      if(!pcp->fContinueCalc) /* User wishes to terminate the sort */
         return;
   }
   return;
}


/****************************************************************\
 * Function name: QuickSort()
 *
 * Parameters:  pcp is a struct which contains the continue flag,
 *              a pointer to the data array, and a count of the items in
 *              the data array.
 *              None of the other pcp members are used.
 *
 * Returns: VOID
 *
 * Purpose:  This routine is a stub that calls the recursive Quick sort
 *           with the proper initial arguments.
 *
 * Usage  :  It passes the pcp (Which contains the array ptr),
 *                  the offset to start sorting at(0), and the offset to
 *                  finish sorting at (Array size-1).
 *
\****************************************************************/
VOID QuickSort(PCALCPARAM pcp)
{
   QuickSortR(pcp,0,(pcp->cArray-1));
   return;
}


/****************************************************************\
 * Function name: QuickSortR()
 *
 * Parameters:  pcp is a struct which contains the continue flag,
 *              a pointer to the data array.
 *              None of the other pcp members are used.
 *              sLeft is the offset of the leftmost sort element (smallest).
 *              sRight is the offset of rightmost sort element (largest).
 *
 * Returns: VOID
 *
 * Purpose:  Implements Quick sort which is O(n lg n).  Quick sort is a
 *           good all-purpose sorting algorithm and is widely used.  This
 *           implementation works by placing the first element in the list
 *           into its correct place, by moving all elements smaller to its
 *           left, and all numbers larger to its right.  The Quick sort
 *           recurses on the smaller side of the properly placed element,
 *           and loops back through the same routine for the larger side.
 *
 *           NOTE: This routine does not recurse on both sides of the
 *                 partition element.  It only recurses on the smaller
 *                 of the two sides.  This was done in order to eliminate
 *                 the stack overrun when it tries to sort an already
 *                 sorted list.
 *
 * Usage:
 *
\****************************************************************/
VOID QuickSortR(PCALCPARAM pcp, LONG sLeft, LONG sRight)
{
   SHORT sTempLeft  = sLeft;    /* Holds lower bound on split position      */
   SHORT sTempRight = sRight;   /* Holds upper bound on split position      */
   SHORT sSplit;                /* Used to hold offset of positioned elem   */
   USHORT usTemp;               /* Holds temp element during positioning    */

   while(TRUE)
   {
      if(sLeft >= sRight)          /* If there is <=1 element, return */
         return;

      if(!pcp->fContinueCalc)      /* User wishes to terminate the sort */
         return;

      /* Split list, sSplit will contain the split element */
      usTemp = pcp->Array[sTempLeft];

      while(TRUE)    /* Loop thru 2 cases until the split position is found */
      {
         while(pcp->Array[sTempRight] > usTemp && /* Skip elems on right side  */
                           sTempRight > sLeft)    /* that are > the split elem */
            sTempRight--;

         if(sTempRight<=sTempLeft)  /* Left and Right have met, split is found */
         {
            pcp->Array[sTempLeft]=usTemp;  /* Place split elem at proper place */
            sSplit = sTempLeft;
            break;
         }
         pcp->Array[sTempLeft++] =
                       pcp->Array[sTempRight];    /* Move small element to LHS */

         while(pcp->Array[sTempLeft] < usTemp &&  /* Skip elems on left side   */
                           sTempLeft < sRight)    /* that are < the split elem */
            sTempLeft++;

         if(sTempRight<=sTempLeft)  /* Left and Right have met, split is found */
         {
            pcp->Array[sTempRight]=usTemp; /* Place split elem at proper place */
            sSplit = sTempRight;
            break;
         }
         pcp->Array[sTempRight--] =
                       pcp->Array[sTempLeft];     /* Move large element to RHS */
      }

      if ((sSplit - sLeft) > (sRight - sSplit)) {
         /* left half is larger than right half */
         QuickSortR(pcp, sSplit+1, sRight);   /* Sort the 2nd half of the list */
         if(!pcp->fContinueCalc)            /* User wishes to terminate sort */
           return;
         sTempLeft = sLeft;                 /* Set up to sort the 1st half */
         sTempRight = sRight = sSplit - 1;
      }
      else {
         /* right half is larger than left half */
         QuickSortR(pcp, sLeft, sSplit-1);    /* Sort the 1st half of the list */
         if(!pcp->fContinueCalc)            /* User wishes to terminate sort */
           return;
         sTempRight = sRight;               /* Set up to sort the 2nd half */
         sTempLeft = sLeft = sSplit + 1;
      }
   }
   return;
}

/****************************************************************\
 * Function name: EnableMenuItem()
 *
 * Parameters:  hwnd is a handle of the current window.
 *              sMenuItem is the ID of the item to Enable/Disable.
 *              fEnable will enable item if TRUE, otherwise disables it.
 *
 * Returns: VOID
 *
 * Purpose: This routine handles enabling/disabling of menu items.  This
 *          is done by getting Parent and Menu hwnd handles then sending
 *          the appropriate message.
 *
 * Usage:
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
}

/****************************************************************\
 * Function name: EntryFldDlgProc()
 *
 * Parameters:  hwnd, msg, mp1, mp2.  Standard PM Dialog Proc params.
 *              A pointer to the current set size is passed in WM_INITDLG.
 *
 * Returns: Terminates with a TRUE iff a new valid data set size is entered.
 *
 * Purpose: Handles all the messages associated with the set entry field
 *          and calls the appropriate handling procedures.  The purpose
 *          of this dialog box is to get a new data set size for the
 *          sort routines.
 *
 *
 * Usage  : If the value entered is valid, the proc will set the
 *                 variable passed in through the WM_INITDLG to the value.
\****************************************************************/
MRESULT EXPENTRY EntryFldDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   SHORT   sNewSize=0;                                /* Holds new set set */
   static  SHORT *pcSetSize;  /* Set to point to user-passed set size */
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         pcSetSize=PVOIDFROMMP(mp2);
         WinSendDlgItemMsg(hwnd, IDD_ENTRYFLD,EM_SETTEXTLIMIT,  /* Limit len */
                                 MPFROM2SHORT(4,0),NULL);
         WinSetDlgItemShort(hwnd, IDD_ENTRYFLD,
                              *pcSetSize,TRUE);
         return (MRESULT)0L;                           /* Allow normal focus setting */

      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
         {
            case DID_OK:
               WinQueryDlgItemShort(hwnd, IDD_ENTRYFLD,
                                    &sNewSize, TRUE); /* Get the short      */
               if(sNewSize>0 && sNewSize<=MAX_ELEMS)  /* Set new data set size */
               {
                  *pcSetSize =  sNewSize;
                  WinDismissDlg(hwnd,TRUE);
               }
               else                                   /* Invalid value      */
               {
                  MessageBox(hwnd, IDMSG_ERRORSIZE,
                                     MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)FALSE;
               }
               return (MRESULT)0L;

            case DID_CANCEL:
               WinDismissDlg(hwnd,FALSE);
               return (MRESULT)0L;

            default:
               sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
               return sRC;
         }
         break;

      default:
         sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   return (MRESULT)0L;
}

/****************************************************************\
 * Function name: AboutDlgProc()
 *
 * Parameters:  hwnd, msg, mp1, mp2.  Standard PM Dialog Proc params.
 *
 * Returns:
 *
 * Purpose: Handles all the messages associated with the About dialog.
 *
 * Usage:
 *
\****************************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         return (MRESULT)0L;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)0L;

      default:
         sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   return (MRESULT)0L;
}

/****************************************************************\
 * Function name: RandomizeData()
 *
 * Parameters:  cSetSize is the current size of the data set
 *
 * Returns: VOID
 *
 * Purpose: This routine randomizes the data arrays.
 *
 * Usage  : This routine assumes that LISTCNT and NELEMS define
 *                 the dimensions of the global Data array which is
 *                 randomized.
\****************************************************************/
VOID RandomizeData(ULONG cSetSize)
{
   USHORT cnt,cnt2;

   for(cnt=0;cnt<LISTCNT;cnt++)
   {
      for(cnt2=0;cnt2<cSetSize;cnt2++)
         Data[cnt][cnt2] = rand();
   }
   return;
}

/****************************************************************\
 * Function name: FixSysMenu()
 *
 * Parameters:  hwndDlg - handle to calling dialog
 *
 * Returns: VOID
 *
 * Purpose: This routine removes the Restore, Size, Minimize, and
 *          Maximize options from the system menu of a dialog.
 *
 * Usage  : Called during the WM_INITDLG of a dialog procedure.
 *
\****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
   HWND hwndMenu;                                   /* handle to system menu */

   hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);

   WinSendMsg(hwndMenu,               /* delete Restore from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_RESTORE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,                  /* delete Size from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_SIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,              /* delete Minimize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MINIMIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,              /* delete Maximize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MAXIMIZE, TRUE),
              MPFROMP(NULL));
   return;
}

/****************************************************************\
 *  Routine for initializing the help manager
 *--------------------------------------------------------------
 *
 *  Name:    InitHelp()
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
   hini.ulReturnCode = 0;
                           /* If tutorial added, add name here */
   hini.pszTutorialName = (PSZ)NULL;

   hini.phtHelpTable = (PHELPTABLE)MAKELONG(SORT_HELP_TABLE, 0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   if(!WinLoadString(hab,
                     0,
                     IDS_HELPWINDOWTITLE,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szWindowTitle))
   {
       MessageBox(hwndClient, IDMSG_CANNOTLOADSTRING,
                             MB_OK | MB_ERROR, FALSE);
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
                     0,
                     IDS_HELPLIBRARYNAME,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szLibName))
   {
       MessageBox(hwndClient, IDMSG_CANNOTLOADSTRING,
                             MB_OK | MB_ERROR, FALSE);
       return;
   }
   hini.pszHelpLibraryName = (PSZ)szLibName;
                                     /* Creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(hwndHelpInstance == 0 || hini.ulReturnCode)
   {
       MessageBox(hwndFrame, IDMSG_HELPLOADERROR, MB_OK | MB_ERROR, TRUE);
       return;
   }
                    /* Associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndFrame))
   {
       MessageBox(hwndFrame, IDMSG_HELPLOADERROR,
                                 MB_OK | MB_ERROR, TRUE);
       return;
   }
/* Help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
   return;
}                                                 /* InitHelp() */

/****************************************************************\
 *  Processes the Help for Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpHelpForHelp(mp2)
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
       if(WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP, NULL, NULL))
           MessageBox(hwndClient,
                      IDMSG_HELPDISPLAYERROR,
                      MB_OK | MB_ERROR,
                      FALSE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                    */
   return;
}                                          /* HelpHelpForHelp() */

/****************************************************************\
 *  Processes the Extended Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpExtended(mp2)
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
       if(WinSendMsg(hwndHelpInstance, HM_EXT_HELP, NULL, NULL))
           MessageBox(hwndClient,
                      IDMSG_HELPDISPLAYERROR,
                      MB_OK | MB_ERROR,
                      FALSE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                    */
   return;
}                                             /* HelpExtended() */

/****************************************************************\
 *  Processes the Keys Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpKeys(mp2)
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
       if(WinSendMsg(hwndHelpInstance, HM_KEYS_HELP, NULL, NULL))
           MessageBox(hwndClient,
                      IDMSG_HELPDISPLAYERROR,
                      MB_OK | MB_ERROR,
                      FALSE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                    */
   return;
}                                                 /* HelpKeys() */

/****************************************************************\
 *  Processes the Index Help command from the menu bar
 *--------------------------------------------------------------
 *
 *  Name:    HelpIndex(mp2)
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
       if(WinSendMsg(hwndHelpInstance, HM_HELP_INDEX, NULL, NULL))
           MessageBox(hwndClient,
                      IDMSG_HELPDISPLAYERROR,
                      MB_OK | MB_ERROR,
                      FALSE);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                    */
   return;
}                                                /* HelpIndex() */

/****************************************************************\
 *  Processes the About command from the Help menu
 *--------------------------------------------------------------
 *
 *  Name:    HelpAbout(mp2)
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
                hwndClient,
                AboutDlgProc,
                (HMODULE)0,
                IDD_PRODUCTINFO,
                NULL);

   /* This routine currently doesn't use the mp2 parameter but     *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                    */
   return;
}                                                /* HelpAbout() */

/****************************************************************\
 *  Displays the help panel indicated
 *--------------------------------------------------------------
 *
 *  Name:    DisplayHelpPanel(idPanel)
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
{
   if(fHelpEnabled)
       if(WinSendMsg(hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMLONG(MAKELONG(idPanel, NULL)),
                     MPFROMSHORT(HM_RESOURCEID)))

           MessageBox(hwndFrame,
                      IDMSG_HELPDISPLAYERROR,
                      MB_OK | MB_ERROR,
                      TRUE);
   return;
}                                         /* DisplayHelpPanel() */

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
   if(hwndHelpInstance != 0)
   {
       WinDestroyHelpInstance(hwndHelpInstance);
   }
   return;
}                                      /* DestroyHelpInstance() */

/****************************************************************\
 *  Message Box procedure
 *--------------------------------------------------------------
 *
 *  Name:    MessageBox(hwndOwner, nIdMsg, fsStyle, fBeep)
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
LONG MessageBox(HWND hwndOwner, LONG idMsg, LONG fsStyle, BOOL fBeep)
{
    CHAR szText[MESSAGELEN];
    LONG sRC;

    if(!WinLoadMessage(hab,
                      (HMODULE)NULL,
                      idMsg,
                      MESSAGELEN,
                      (PSZ)szText))
    {
        WinAlarm(HWND_DESKTOP, WA_ERROR);
        return RETURN_ERROR;
    }

    if(fBeep)
        WinAlarm(HWND_DESKTOP, WA_ERROR);

    sRC = WinMessageBox(HWND_DESKTOP,
                        hwndOwner,
                        szText,
                        (PSZ)NULL,
                        IDD_MSGBOX,
                        fsStyle);
    return sRC;
}                                      /* MessageBox() */

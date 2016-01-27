/************ FILE C Sample Program Source Code File (.C) *******************
 *
 * PROGRAM NAME: GRAPHIC
 * -------------
 *  Presentation Manager Non-Retained Graphics C Sample Program
 *
 * WHAT THIS PROGRAM DEMONSTRATES:
 * -------------------------------
 * Following are all the functions needed to print "GPI", "METAFILE"
 * in a separate thread.
 *
 *  Copyright (C) 1991, 1993 IBM Corporation
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

#include <stdlib.h>
#include <string.h>

#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_WINWINDOWMGR
#define INCL_GPICONTROL
#define INCL_WINPOINTERS
#define INCL_GPIMETAFILES
#define INCL_GPICORRELATION
#define INCL_WINMESSAGEMGR
#define INCL_GPIREGIONS
#define INCL_WINRECTANGLES
#define INCL_GPITRANSFORMS
#define INCL_GPIPRIMITIVES
#define INCL_WINERRORS
#define INCL_GPILCIDS
#define INCL_WINDIALOGS
#define INCL_DEV
#define INCL_WINSTDDLGS
#define INCL_DOSERRORS
#define INCL_GPIERRORS
#define INCL_WINSHELLDATA 

#include <os2.h>
#include <malloc.h>
#include <memory.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "graphic.h"

#define EXPORT_BUFFER_SIZE 1000

/* static function definitions */
static   VOID       PreparePrinting(PHPS hpsPrint,
                                    PHDC hdcPrinter,
                                    HAB habPrint,
                                    PSIZEL sizlPS,
                                    PRESOLUTION Resolution,
                                    PPRTTXT_DATA PrintData,
                                    PSIZEL DevCaps);
static   VOID       MetaOutput(PDRAW_PARM PrtParm,
                               PPRTTXT_DATA PrintData,
                               HAB habPrint);
static   int _Optlink SortQueues(const void *Queue1, const void *Queue2);
static   PSZ        GetQueueNamesString(PULONG);
static   CHAR       pm_spooler_queue_dd[] = "PM_SPOOLER_QUEUE_DD";
static   CHAR       Drive[]={0,0,0};

/*********************************************************************
 *
 *  Name:   OpenFileDialog
 *
 *  Purpose: open the standard file open dialog as file extention
 *           and return the filename
 *
 *  Usage:   called when the user needs to supply a name for
 *           the file to be opened
 *
 *  Method:  calls the standard file open dialog to get the
 *           file name.
 *
 *  Parameters: HWD hwnd         Handle of the owner window.
 *              PSZ szTitle      Title of open dialog.
 *              PSZ pszFileExt   File extention. (for example : *.txt)
 *              PSZ pszFullPath  PSZ for returning the file name.
 *
 *  Returns: TRUE if successful in getting a file name, FALSE
 *              if not in pushing CANCEL
 *           PSZ pszFullPath pointer to filename (full path)
 *
 **********************************************************************/
BOOL OpenFileDialog(HWND hwndOwner,
                    PSZ szTitle,
                    PSZ szFileExt,
                    PSZ szFullPath)
{
   static   PSZ        ppszdefaultdrivelist[] = {NULL};

            FILEDLG    fdg;               /* file dialog structure           */

   fdg.cbSize = sizeof(FILEDLG);          /* Size of FILEDLG.                */
   fdg.pszTitle = szTitle;                /* String to display in title bar. */
   fdg.pszOKButton = (PSZ)"Open";         /* String to display in OK button. */
   fdg.ulUser = 0L;                       /* User defined field.             */
   fdg.fl = FDS_HELPBUTTON |              /* FDS flags.                      */
            FDS_CENTER |
            FDS_OPEN_DIALOG;
   fdg.pfnDlgProc = (PFNWP)TemplateOpenFilterProc;/* Entry point to custom*/
                                                  /*         dialog proc. */
   fdg.lReturn = 0L;                      /* Result code from dialog dismissal. */
   fdg.lSRC = 0L;                         /* System return code.          */
   fdg.hMod = 0;                          /* Custom file dialog template. */
   fdg.usDlgId = 0;                       /* Custom file dialog ID.       */
   fdg.x = 0;                             /* X coordinate of the dialog.  */
   fdg.y = 0;                             /* Y coordinate of the dialog.  */

   /* set selected fully qualified path */
   strcpy( fdg.szFullFile, szFileExt);

   fdg.pszIType = NULL;                   /* Pointer to string containing   */
                                          /*   Initial EA type filter.      */
   fdg.papszITypeList = NULL;             /* Pointer to table of pointers   */
                                          /*  that point to null terminated */
   if (!Drive[0])                         /*  Type strings.                 */
       fdg.pszIDrive = NULL;              /* Pointer to string containing   */
   else                                   /*  initial drive.                */
       fdg.pszIDrive = Drive;

   fdg.papszIDriveList = (PAPSZ)ppszdefaultdrivelist;
                                          /* Pointer to table of pointers   */
                                          /*  that point to null terminated */
                                          /*  Drive strings.                */
   fdg.sEAType = 0;                       /* Selected file's EA Type.       */
   fdg.papszFQFilename = NULL;            /* Pointer to table of pointers   */
                                          /*  point to null terminated      */
                                          /*  FQFname strings.              */
   fdg.ulFQFCount = 0L;                   /* Numbers of file selected.      */

   /* get the file */

   if (!WinFileDlg(HWND_DESKTOP, hwndOwner, (PFILEDLG)&fdg))
      return FALSE;

   if (fdg.lReturn == DID_CANCEL)
      return FALSE;

   /* copy file name into file name buffer */

   strcpy(szFullPath, fdg.szFullFile);
   strcpy(szFileExt, fdg.szFullFile);
   strncpy(Drive,fdg.szFullFile,2);         /* Always Contains Drive letter */

   return TRUE;
} /* End of OpenFileDialog */

/**************************************************************************
 * Function: DrawThread
 *
 * DrawThread is the asynchronous drawing thread.  It performs all
 * drawing, whether to the screen or to the printer.
 * It obtains an anchor block handle and creates a message queue.
 * It then posts a private message to the main thread's client area
 * if successful. NewThread monitors its own message queue, and
 * processes the commands it receives.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  none
 *
 ***************************************************************************/
VOID DrawThread(PMAIN_PARM Parm)
{
static   CHAR       DrawThreadErrorTitle[] = "Draw Thread Error:";
static   CHAR       DrawThreadError1[] = "WinInitialize failed!";
static   CHAR       DrawThreadError2[] = "Cannot Create Message Queue.";
         QMSG       qmsgAsync;      /* Message structure for WinGetMsg  */
         QMSG       qmsgPeek;       /* Message structure for WinPeekMsg */
         BOOL       fFlushQueue;    /* Flush-queue indicator            */
         BOOL       fQueueFlush;
         RECTL      rclClient;      /* Bounding rect for picture xform  */
         BOOL       Loop = TRUE;
static   PSZ        dcdatablk[9] = {0,
                                    "MEMORY", /* display driver      */
                                    0, 0, 0, 0, 0, 0, 0 };

  /*
   * Obtain an anchor block handle. If this fails, the thread and
   * the application are terminated.
   */

   if ((Parm->DrawParm.hABDraw = WinInitialize((USHORT)0)) == (HAB)NULLHANDLE)
   {
      WinPostMsg(Parm->hWndFrame,
              WM_USER_END_ON_ERROR,
              MPFROMP(DrawThreadErrorTitle),
              MPFROMP(DrawThreadError1));
      DosExit(EXIT_THREAD, 0);
   }

  /*
   * Create a message queue. If this fails, the thread and the
   * application are terminated.
   */

   if ((Parm->hMQDraw = WinCreateMsgQueue(Parm->DrawParm.hABDraw, 0)) ==
                                          (HMQ)NULLHANDLE)
   {
      WinTerminate(Parm->DrawParm.hABDraw);
      WinPostMsg(Parm->hWndFrame,
              WM_USER_END_ON_ERROR,
              MPFROMP(DrawThreadErrorTitle),
              MPFROMP(DrawThreadError2));
      DosExit(EXIT_THREAD, 0);
   }

  /*
   * The SetTransform function is invoked to set up the transformation
   * for the default picture. The main thread's priority is adjusted
   * to be higher than that of the asynchronous thread. (GPI CIRCLES)
   */

   WinPostMsg(Parm->hWndFrame,
             WM_COMMAND,
             MPFROMP(IDM_F_GPI),
             MPFROMP(NULL));
   DosSetPriority(PRTYS_THREAD, PRTYC_NOCHANGE, 1L, 0L);
   while (Loop)
   {

   /*
    * The semaphore hsemStoppable is obtained so that the flag
    * fWaiting can be set to TRUE. This indicates that the thread is
    * waiting for a message, and prevents the main thread from
    * issuing GpiSetStopDraw.
    * To be sure that it will not be suspended, the asynchronous
    * thread sets the stop-draw condition off. It then reads its
    * message queue.
    */

      WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
      Parm->fWaiting = TRUE;
      DosReleaseMutexSem(Parm->hmtxsemStoppable);

      if (Parm->hpsClient != (HPS)NULLHANDLE)
         GpiSetStopDraw(Parm->hpsClient, (LONG)FALSE);

      WinGetMsg(Parm->DrawParm.hABDraw, &qmsgAsync, (HWND)NULLHANDLE, 0L, 0L);

   /*
    * When a message has been read from the queue, the flag fWaiting
    * is set the FALSE. This allows the main thread to issue the
    * GpiSetStopDraw call if necessary.
    */

      WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
      Parm->fWaiting = FALSE;
      DosReleaseMutexSem(Parm->hmtxsemStoppable);

   /*
    * Before doing any drawing, the thread examines its message queue
    * to see if it contains any other commands that would force
    * redrawing. If there is a flush command in the queue, all
    * commands, up to and including the flush command, are read from
    * the queue. A SET_TRANSFORM message is treated like a flush
    * command, because it also means that a new picture is required.
    */

      fFlushQueue = WinPeekMsg(Parm->DrawParm.hABDraw,
                            &qmsgPeek,
                            0L,
                            FLUSH_COMMAND,
                            FLUSH_COMMAND,
                            PM_NOREMOVE);

      fQueueFlush = WinPeekMsg(Parm->DrawParm.hABDraw,
                            &qmsgPeek,
                            0L,
                            SET_TRANSFORM,
                            SET_TRANSFORM,
                            PM_NOREMOVE);

      if (fFlushQueue|| fQueueFlush)
         do
         {
            WinGetMsg(Parm->DrawParm.hABDraw, &qmsgAsync, NULLHANDLE, 0L, 0L);
         } while (qmsgAsync.msg != FLUSH_COMMAND &&
                  qmsgAsync.msg != SET_TRANSFORM &&
                  qmsgAsync.msg != STOP_DRAWING
                  ); /* enddo */

      if (WinPeekMsg(Parm->DrawParm.hABDraw,
                  &qmsgPeek,
                  NULLHANDLE,
                  STOP_THREAD,
                  DUMMY_COMMAND,
                  PM_NOREMOVE))
         Parm->DrawParm.fDrawing = FALSE;
      else
         Parm->DrawParm.fDrawing = TRUE;

   /* Process the commands.   */

      switch (qmsgAsync.msg)
      {
       case SET_TRANSFORM:
            /*
             * Invoke the SetTransform function to define a default viewing
             * transformation for the picture.
             */
           if (*Parm->Message != '\0')
              break;
           SetTransform(Parm, qmsgAsync.mp1);
           break;

       case DRAW_ORDERS:
            /*
             * Invoke the DoDraw function. DoDraw uses the region definition
             * passed to it to repaint the invalidated area of the screen.
             * The region definition is then destroyed.
             */
           if (Parm->CheckedItem == 3)
           {
              break;
           }
           else
           {
              if (*Parm->Message != '\0')
                 break;
              DoDraw(Parm->hpsClient, &Parm->DrawParm, (HRGN)qmsgAsync.mp1,
                     Parm->InfoWnd.hWnd);
              if (qmsgAsync.mp1 != MPVOID)
                 GpiDestroyRegion(Parm->hpsClient, (HRGN)qmsgAsync.mp1);

              break;
           }
      case DISPLAY_MESSAGE:
           DisplayMessage(Parm->hWndClient,
                          Parm->Message);
           break;

      case SIZING:
           /* Recalculate the picture transformation. */

           if (*Parm->Message != '\0')
              break;
           WinQueryWindowRect(Parm->hWndClient, &rclClient);
           Transform(&Parm->DrawParm, Parm->hpsClient, &rclClient);
           break;

      case ACTIVATE_DRAW_THREAD:
           WinPostMsg(Parm->hWndFrame, ACTIVATE_DRAW_THREAD, MPVOID, MPVOID);
           break;

      case STOP_DRAWING:
           Parm->DrawThreadActivated = FALSE;
           DosPostEventSem(Parm->hevsemStop);
           break;

      case STOP_THREAD:
           /* StopThread is invoked to terminate the thread. */
           Loop = FALSE;
           break;

      case FLUSH_COMMAND:
           /*
            * Other commands have already been discarded. Set the fAllValid
            * flag to TRUE.
            */
           Parm->DrawParm.fAllValid = TRUE;
           break;
      }
   }

   DosSetPriority(PRTYS_THREAD, PRTYC_IDLETIME, 0L, 0L);

   WinDestroyMsgQueue(Parm->hMQDraw);  /* Destroy this thread's message queue */

   WinTerminate(Parm->DrawParm.hABDraw);

   DosPostEventSem(Parm->hevsemTerminate);

   DosExit(EXIT_THREAD, 0);
} /* End of DrawThread */

/**************************************************************************
 * Function: SendCommand
 *
 * The SendCommand function posts the required command and its
 * parameters to the asynchronous drawing thread's message queue.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *             usCommand    command to send
 *             ULONG        long value passed to draw thread as parameter1
 *
 * Result:  BOOL            TRUE is ok
 *
 ***************************************************************************/
BOOL SendCommand(PMAIN_PARM Parm, ULONG ulCommand, ULONG ulInfo)
{
   APIRET     Ret;

   if (Parm->hMQDraw == (HMQ)NULLHANDLE)
      return FALSE;
 /*
  * Communication between the main thread and the asynchronous thread
  * is controlled by the two flags fWaiting and fStoppable. Access to
  * the two flags is regulated by the semaphore hsemStoppable.
  * The main thread requests access to the semaphore, and waits until
  * it is available. If this command is one that causes drawing, the
  * main thread checks whether the asynchronous thread is waiting for
  * a message. If it is not waiting, the main thread issues
  * GpiSetStopDraw to suspend drawing. It then frees the semaphore,
  * and posts the message to the asynchronous thread's message queue.
  */
   do
   {
      Ret = WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
   } while (Ret == (APIRET)ERROR_INTERRUPT);

   if (ulCommand <= DUMMY_COMMAND)
      if (!Parm->fWaiting)
         GpiSetStopDraw(Parm->hpsClient, (ULONG)Parm->fStoppable);

   DosReleaseMutexSem(Parm->hmtxsemStoppable);

   return WinPostQueueMsg(Parm->hMQDraw,
                       ulCommand,
                       MPFROMLONG(ulInfo),
                       MPVOID);
} /* End of SendCommand */

/**************************************************************************
 * Function: SetTransform
 *
 * The SetTransform function calculates the original default viewing
 * transformation for the picture.  This function can be interrupted
 * only by another request to set the default viewing transformation,
 * or by a request to terminate the thread. SetTransform invokes the
 * DoDraw function to draw the picture, and then calculates the
 * bounding rectangle of the picture.
 * It then invokes the Transform function to set the appropriate
 * transformation.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *             MPARAM       message parameter  passed from SendCommand
 *
 * Result:  none
 *
 ***************************************************************************/
VOID SetTransform(PMAIN_PARM Parm, MPARAM mp1)
{
   LONG       lRc;
   RECTL      rclDest;
   RECTL      rclClient;
   POINTL     ptlWork;
   RECTL      rclWork;

   GpiErase(Parm->hpsClient);
   WinInvalidateRect(Parm->InfoWnd.hWnd, (PRECTL)NULL, TRUE);

   DisplayMessage(Parm->hWndClient,
                  GetString(Parm->DrawParm.hABDraw,
                            ((HMF)mp1 == (HMF)NULLHANDLE) ?
                                 TEXT_DRAWTHR_SIZEGPI : TEXT_DRAWTHR_SIZEMETA,
                            STATIC_STRING));

   Parm->fBusy = TRUE;
   if (WinQueryPointerPos(HWND_DESKTOP, &ptlWork))
       if (WinMapWindowPoints(HWND_DESKTOP, Parm->hWndClient, &ptlWork, 1))
           if (WinQueryWindowRect(Parm->hWndClient, &rclWork))
               if (WinPtInRect(Parm->DrawParm.hABDraw, &rclWork, &ptlWork))
                   WinSetPointer(HWND_DESKTOP, Parm->hptrGlass);

   if (Parm->DrawParm.hmfPicture != (HMF)NULLHANDLE)
      GpiDeleteMetaFile(Parm->DrawParm.hmfPicture);

   /* The current metafile is deleted before the next one is drawn.     */
   Parm->DrawParm.hmfPicture = (HMF)mp1;

   WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
   Parm->fStoppable = FALSE;
   DosReleaseMutexSem(Parm->hmtxsemStoppable);

   /*
    * The boundary data is cleared so that the transformation and the
    * presentation space are not reset after the GpiPlayMetaFile call.
    */
   Parm->DrawParm.rclBounds.xRight  = 0xFFFF;
   Parm->DrawParm.rclBounds.xLeft   = 0xFFFF;
   Parm->DrawParm.rclBounds.yTop    = 0xFFFF;
   Parm->DrawParm.rclBounds.yBottom = 0xFFFF;
   GpiResetBoundaryData(Parm->hpsClient);
   /*
    * The presentation space is disassociated from the device context.
    * Boundary-data accumulation is switched on, and the DoDraw
    * function is invoked to draw the picture into the disassociated
    * device context. The picture is drawn, but not displayed. The
    * boundary data is retrieved, and the presentation space is
    * reassociated with the device context. Boundary-data accumulation
    * is switched off.
    */
   GpiAssociate(Parm->hpsClient, (HDC)NULLHANDLE);
   GpiSetDrawControl(Parm->hpsClient, DCTL_BOUNDARY, DCTL_ON);

   GpiSetStopDraw(Parm->hpsClient, (ULONG)Parm->fStoppable);
   Parm->DrawParm.fDrawing = TRUE;
   lRc = DoDraw(Parm->hpsClient, &Parm->DrawParm, (HRGN)NULLHANDLE,
                                Parm->InfoWnd.hWnd);

   GpiQueryBoundaryData(Parm->hpsClient, &Parm->DrawParm.rclBounds);
   GpiAssociate(Parm->hpsClient, Parm->hdcClient);
   GpiSetDrawControl(Parm->hpsClient, DCTL_BOUNDARY, DCTL_OFF);
   /*
    * The presentation space is reset so that subsequent
    * GpiPlayMetaFile calls will succeed.
    */
   GpiResetPS(Parm->hpsClient, GRES_ALL);
   /*
    * If the DoDraw function was successful, the Transform function is
    * invoked to set the transformation. WinInvalidateRect invalidates
    * the client area and causes a WM_PAINT message to be issued.
    * The title bar is also updated.
    */
   if (lRc == DRAW_OK)
   {
      WinQueryWindowRect(Parm->hWndClient, &rclClient);
      Transform(&Parm->DrawParm, Parm->hpsClient, &rclClient);
      WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
      Parm->fNotReady = FALSE;
   /*
    * The application can now process paint and erase-background
    * requests.
    */
      DosReleaseMutexSem(Parm->hmtxsemStoppable);
      WinQueryWindowRect(Parm->hWndClient, &rclDest);
      WinInvalidateRect(Parm->hWndClient, &rclDest, FALSE);
   }
   else
   {
      strcpy(Parm->Message,
             GetString(Parm->DrawParm.hABDraw,
                       ERRMSG_DRAWTHR_MSG2,
                       STATIC_STRING));
      strcat(Parm->Message, Parm->MetafilePath);
      DisplayErrorMessage( Parm->hWndClient, Parm->Message);
      Parm->fNotReady = FALSE;
   }
   /*
    * If the DoDraw function failed, the main thread is prompted to
    * display a warning message and issue another SET_TRANSFORM
    * message for the default picture.
    */
   Parm->fBusy = FALSE;
   if (WinQueryPointerPos(HWND_DESKTOP, &ptlWork))
       if (WinMapWindowPoints(HWND_DESKTOP, Parm->hWndClient, &ptlWork, 1))
           if (WinQueryWindowRect(Parm->hWndClient, &rclWork))
               if (WinPtInRect(Parm->DrawParm.hABDraw, &rclWork, &ptlWork))
                   WinSetPointer(HWND_DESKTOP, Parm->hptrArrow);

   /* Allow GpiSetStopDraw to interrupt subsequent drawing.             */
   WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
   Parm->fStoppable = TRUE;
   DosReleaseMutexSem(Parm->hmtxsemStoppable);
   return;
} /* End of SetTransform */

/**************************************************************************
 * Function: WndProcPaint
 *
 * The WndProcPaint function is invoked from the window procedure
 * ClientWndProc to process the WM_PAINT message. WndProcPaint
 * in turn invokes the asynchronous thread to perform the window
 * painting. If the asynchronous thread has both a drawing and a
 * transformation to process, a region is created that is equal to the
 * update region. This is passed to the asynchronous thread and the
 * region is validated. Otherwise, no painting occurs.
 * If the asynchronous thread does not have time to paint the updated
 * regions, it accumulates them.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  MRESULT         message result
 *
 ***************************************************************************/
MRESULT WndProcPaint(PMAIN_PARM Parm)
{
   HRGN      hrgnUpdt;
   LONG      sRgnType;
   APIRET    Ret;
   ULONG     MsgTyp;

   do
   {
      Ret = WinRequestMutexSem(Parm->hmtxsemStoppable, (ULONG)-1);
   } while (Ret == (APIRET)ERROR_INTERRUPT);

   hrgnUpdt = GpiCreateRegion(Parm->hpsPaint, 0L, (PRECTL)NULL);
   sRgnType = WinQueryUpdateRegion(Parm->hWndClient, hrgnUpdt);
   if (!Parm->fNotReady)
   {
      if (*Parm->Message != '\0')
         MsgTyp = DISPLAY_MESSAGE;
      else
         MsgTyp = DRAW_ORDERS;

      DosReleaseMutexSem(Parm->hmtxsemStoppable);
      if ((sRgnType == RGN_RECT) ||
          (sRgnType == RGN_COMPLEX))
      {
         if (!SendCommand(Parm, MsgTyp, 0L))
            GpiDestroyRegion(Parm->hpsPaint, hrgnUpdt);
         WinValidateRegion(Parm->hWndClient, hrgnUpdt, FALSE);
      }
      else
      {
         SendCommand(Parm, MsgTyp, 0L);
         WinValidateRegion(Parm->hWndClient, hrgnUpdt, FALSE);
         GpiDestroyRegion(Parm->hpsPaint, hrgnUpdt);
      } /* endif */
   }
   else
   {
      DosReleaseMutexSem(Parm->hmtxsemStoppable);
      WinValidateRegion(Parm->hWndClient, hrgnUpdt, FALSE);
      GpiDestroyRegion(Parm->hpsPaint, hrgnUpdt);
   } /* endif */
   return(FALSE);
} /* End of WndProcPaint */

/**************************************************************************
 * Function: Transform
 *
 * The Transform function recalculates the default viewing
 * transformation when the picture is scrolled or sized, and when the
 * 'Best Fit' option is changed.
 * If the window handle is NULLHANDLE, the default viewing transformation
 * is applied to ensure that the picture is contained within the page
 * and that no graphics clipping occurs for printing.
 *
 * Parameters: PDRAW_PARM   draw parameter structure
 *             HPS          presentation space handle for drawing
 *             PRECTL       rectangle
 *
 * Result:  none
 *
 ***************************************************************************/
VOID Transform(PDRAW_PARM Parm, HPS hpsXForm, PRECTL prclXform)
{
         LONG       lxPictRight, lxPictLeft;
         LONG       lyPictTop, lyPictBottom; /* picture boundary information */
         LONG       clxPict, clyPict;        /* picture extents              */
         POINTL     ptlPictCenter;           /* center of picture            */
         LONG       clxBounds, clyBounds;    /* bounds area extent           */
         LONG       alScale[2];              /* scaling factors              */
         LONG       sxWork, syWork;          /* work areas                   */
         LONG       lxWork, lyWork;          /* work areas                   */
static   MATRIXLF   matlfViewTransform = {65536,0,0,0,65536,0,0,0,1};

/* Calculate the center of the bounding rectangle of the picture. */
   lxPictRight = Parm->rclBounds.xRight + 5;
   lxPictLeft = Parm->rclBounds.xLeft - 5;
   lyPictTop = Parm->rclBounds.yTop + 5;
   lyPictBottom = Parm->rclBounds.yBottom - 5;

   ptlPictCenter.x = (lxPictRight - lxPictLeft)/2 + lxPictLeft;
   ptlPictCenter.y = (lyPictTop - lyPictBottom)/2 + lyPictBottom;


   /* Get the picture extents. */
   clxPict = lxPictRight - lxPictLeft;
   clyPict = lyPictTop - lyPictBottom;

   /* Get the current bounding rectangle extent. */
   clxBounds = prclXform->xRight - prclXform->xLeft;
   clyBounds = prclXform->yTop - prclXform->yBottom;

   /* Calculate the x/y ratio of the current bounding area. */
   sxWork = (LONG)(clyBounds/clxBounds);
   lxWork = clyBounds%clxBounds;
   alScale[0] = MAKEULONG((USHORT)((lxWork * 0x10000)/clxBounds), sxWork);

   /* Calculate the x/y ratio of the picture. */
   syWork = clyPict/clxPict;
   lyWork = clyPict%clxPict;
   alScale[1] = MAKEULONG((USHORT)((lyWork * 0x10000)/clxPict), syWork);
   /*
    * Compare the two ratios to determine the dimension along which
    * scaling is to be provided, so that the picture fits into the
    * area and its aspect ratio is preserved.  The same scaling is
    * then applied along both the x and y axes.
    */
   if (alScale[0] > alScale[1])
   {
      sxWork = clxBounds/clxPict;
      lxWork = clxBounds%clxPict;
      alScale[0] = MAKEULONG((USHORT)((lxWork * 0x10000)/clxPict), sxWork);
      alScale[1] = alScale[0];
   }
   else
   {
      syWork = clyBounds/clyPict;
      lyWork = clyBounds%clyPict;
      alScale[1] = MAKEULONG((USHORT)((lyWork * 0x10000)/clyPict), syWork);
      alScale[0] = alScale[1];
   }

   GpiScale(hpsXForm,
            &matlfViewTransform,
            TRANSFORM_REPLACE,
            (PFIXED)alScale,
            &ptlPictCenter);
   /*
    * Translate the center of the picture to the center of the
    * bounding area, based on the page origin.
    */
   ptlPictCenter.x = (prclXform->xRight - prclXform->xLeft)/2 - ptlPictCenter.x;
   ptlPictCenter.y = (prclXform->yTop - prclXform->yBottom)/2 - ptlPictCenter.y;

   GpiTranslate(hpsXForm,
                &matlfViewTransform,
                TRANSFORM_ADD,
                &ptlPictCenter);

   /* Now set the default viewing transform. */
   GpiSetDefaultViewMatrix(hpsXForm,
                           9L,
                           &matlfViewTransform,
                           TRANSFORM_REPLACE);
/*
 * If the picture has been transformed for the screen, we need to
 * reset the scroll bar thumb size, so convert the picture height
 * from world to page coordinates and together with the rect area
 * height send the visible/total sizes to the scroll bar.
 */
} /* End of Transform */

/*************************************************************************
 * Function: DoDraw
 *
 * The DoDraw function draws the metafile contents. The supplied
 * update rectangle is used to define a clipping region, so that the
 * amount of drawing required is restricted.
 *
 * Parameters: HPS          presentation space handle for drawing
 *             PDRAW_PARM   draw parameter structure
 *             HRGN         region for  drawing
 *             HWND         window handle of information window
 *
 * Result:  LONG            return code
 *
 ***************************************************************************/
LONG DoDraw(HPS hpsDraw, PDRAW_PARM Parm, HRGN rgn, HWND InfoWnd)
{
         LONG       lRc;
         LONG       lOptionCount;
         LONG       alOptionArray[8];
         LONG       lSegCount;
         LONG       lDescLen;
         CHAR       achDescBuff[251];
         LONG       lSaveMode;
         HRGN       hrgnOld;
         MATRIXLF   matlfSaveTransform;
         FIXED      fxSize;
static   AREABUNDLE PatternAttr =
                    {CLR_BACKGROUND, 0L, 0, 0, 0, 0, 0L};
static   POINTL     ptlCurr = {0, 0};

   if (rgn != (HRGN)NULLHANDLE)
   {
  /*
   * A valid region has been passed to DoDraw. Depending on the
   * setting of the flag fAllValid, the region is either copied or
   * is combined with the current region using a logical OR
   * operation.
   */
      if (Parm->fAllValid)
      {
         GpiCombineRegion(hpsDraw,
                       Parm->hrgnInvalid,
                       rgn,
                       Parm->hrgnInvalid,
                       CRGN_COPY);
         Parm->fAllValid = FALSE;
      }
      else
      {
         GpiCombineRegion(hpsDraw,
                       Parm->hrgnInvalid,
                       Parm->hrgnInvalid,
                       rgn,
                       CRGN_OR);
      } /* endif */
   } /* endif */

  /* Stop here if the flag fDrawing is set to FALSE. */
   if (!Parm->fDrawing)
      return(FALSE);
 /*
  * If an invalid region exists, define a clipping region and clear
  * the area within the region. If there is no invalid region, the
  * whole client area is cleared.
  */
   if (rgn != (HRGN)NULLHANDLE)
   {
      lSaveMode = GpiQueryAttrMode(hpsDraw);
      GpiSetAttrMode(hpsDraw, AM_PRESERVE);
      GpiSetAttrs(hpsDraw,
               PRIM_AREA,
               ABB_COLOR,
               0L,
               (PBUNDLE)&PatternAttr);
      GpiPaintRegion(hpsDraw, Parm->hrgnInvalid);
      GpiSetAttrMode(hpsDraw, lSaveMode);
      GpiPop(hpsDraw, 1L);
      GpiSetClipRegion(hpsDraw, Parm->hrgnInvalid, &hrgnOld);
   }
   else
   {
      GpiErase(hpsDraw);
      if (InfoWnd != (HWND)NULLHANDLE)
      WinInvalidateRect(InfoWnd, (PRECTL)NULL, TRUE);
   } /* endif */
 /*
  * If a metafile has been loaded, it is played into the presentation
  * space. If GpiPlayMetaFile is successful, the setting of the
  * stop-draw flag is checked. If the flag is set, it is assumed that
  * GpiPlayMetaFile did not complete successfully.
  * If no metafile has been loaded, a simple default picture is drawn.
  */
if (Parm->hmfPicture != (HMF)NULLHANDLE)
   {
   lDescLen = 250;
   lOptionCount = 8L;
   alOptionArray[PMF_SEGBASE] = 0L;
   alOptionArray[PMF_LOADTYPE] = LT_DEFAULT;
   alOptionArray[PMF_RESOLVE] = RS_DEFAULT;
   alOptionArray[PMF_LCIDS] = LC_LOADDISC;
   alOptionArray[PMF_RESET] = RES_DEFAULT;
   alOptionArray[PMF_SUPPRESS] = SUP_DEFAULT;
   alOptionArray[PMF_COLORTABLES] = CTAB_REPLACE;
   alOptionArray[PMF_COLORREALIZABLE] = CREA_DEFAULT;
   lRc = GpiPlayMetaFile(hpsDraw,
                         Parm->hmfPicture,
                         lOptionCount,
                         (PLONG)alOptionArray,
                         &lSegCount,
                         lDescLen,
                         (PSZ)&achDescBuff[0]
                         );
   if (lRc == GPI_OK)
      if (LOUSHORT(WinGetLastError(Parm->hABDraw)) ==
          PMERR_STOP_DRAW_OCCURRED)
         lRc = DRAW_STOPPED;
     /*
      * Reset the presentation space, including the default viewing
      * transformation, and then reapply the required transformation.
      * This is to ensure that the metafile can be replayed without
      * causing errors due to stored segments or new character sets,
      * for example. The presentation space is not reset if called
      * from the SetTransform function, as that function needs the
      * accumulated boundary information.
      */
   if (Parm->rclBounds.xRight != 0xFFFF)
      {
      GpiQueryDefaultViewMatrix(hpsDraw,
                                9L,
                                &matlfSaveTransform);
      GpiResetPS(hpsDraw, GRES_ALL);
      GpiSetDefaultViewMatrix(hpsDraw,
                              9L,
                              &matlfSaveTransform,
                              TRANSFORM_REPLACE);
      }
   }
else
   {
   GpiSetColor(hpsDraw, CLR_BLACK);
   fxSize = 0x100000;
   if (Circles(hpsDraw, &ptlCurr, fxSize))
      {
      lRc = DRAW_OK;
      }
   else
      {
      lRc = DRAW_STOPPED;
      }
   } /* endif */

  /* Clear any clipping region defined earlier. */
   if (rgn != (HRGN)NULLHANDLE)
      GpiSetClipRegion(hpsDraw, (HRGN)NULLHANDLE, &hrgnOld);
    /*
     * If the drawing failed, the flag fAllValid is not set. This
     * ensures that, the next time through, the region will be combined
     * with the new invalid region.
     */
   if (lRc == DRAW_OK)
      if (rgn != (HRGN)NULLHANDLE)
         Parm->fAllValid = TRUE;
   return(lRc);
} /* End of DoDraw */

/*************************************************************************
 * Function: Circles
 *
 *  The Circles function draws a circle and then calls itself to
 *  draw four further circles within the first one. It will
 *  repeat this until the circles reach a certain size or if StopDraw
 *  has been set for the presentation space.
 *
 * Parameters: HPS          presentation space handle for drawing
 *             PPOINTL      point  structure
 *             FIXED        size
 *
 * Result:  BOOL            return code, TRUE if ok
 *
 ***************************************************************************/
BOOL Circles(HPS hpsDraw, PPOINTL pptlCur, FIXED fxSize)
{
         LONG      i;
         POINTL     ptlWork;
         FIXED      fxWork;
static   LONG       xOff[4] = {0, 1, 0, -1};
static   LONG       yOff[4] = {1, 0, -1, 0};
static   LONG       ColorTab[] =
                    {
                    CLR_BLACK,
                    CLR_RED,
                    CLR_GREEN,
                    CLR_BLUE
                    };

  /* Don't continue if StopDraw is set */
   if (GpiQueryStopDraw(hpsDraw) == SDW_ON)
      return(FALSE);
  /* Don't continue if the circles are getting too small */
   if (fxSize < 0x10000)
      return(TRUE);
  /* Draw a circle of the required size at the required position. */
   GpiSetCurrentPosition(hpsDraw, pptlCur);
   GpiFullArc(hpsDraw, DRO_OUTLINE, fxSize);
 /*
  * Now draw a further 4 circles, half the current size, within the
  * one just drawn.
  */
   fxWork = fxSize >> 1;
   for (i = 0; i < 4; i++)
   {
      ptlWork.x = pptlCur->x + xOff[i]*(FIXEDINT(fxSize)>>1);
      ptlWork.y = pptlCur->y + yOff[i]*(FIXEDINT(fxSize)>>1);
      GpiSetColor(hpsDraw, ColorTab[i]);
      if (!Circles(hpsDraw, &ptlWork, fxWork))
         return(FALSE);
   }
   return(TRUE);
} /* End of Circles */

/*************************************************************************
 * Function: DisplayMessage
 *
 * Display a message in the client area.
 *
 * Parameters: HWND         window handle
 *             PSZ          message to display
 *
 * Result:  none
 *
 ***************************************************************************/
VOID DisplayMessage(HWND hwndDraw, PSZ Message)
{
   RECTL      rDraw;
   PFONTMETRICS FontMetrics;
   POINTL     MessagePos;
   HPS        hpsDraw;

   WinQueryWindowRect(hwndDraw, &rDraw);               /* query window size */

   hpsDraw = WinGetPS(hwndDraw);

   WinFillRect(hpsDraw, &rDraw, CLR_BACKGROUND);          /* clear window   */

   FontMetrics = (PFONTMETRICS)malloc(sizeof(FONTMETRICS)); /* query fontsize */

   GpiQueryFontMetrics(hpsDraw, (LONG)sizeof(FONTMETRICS), FontMetrics);

   MessagePos.x = 4L;                                   /* set message position*/
   MessagePos.y = rDraw.yTop - FontMetrics->lMaxAscender;

   GpiCharStringAt(hpsDraw, &MessagePos,                     /* display message*/
                   (LONG)strlen(Message), Message);

   WinReleasePS(hpsDraw);

   free((PVOID)FontMetrics);
} /* End of DisplayMessage */

/*************************************************************************
 * Function: DisplayErrorMessage
 *
 * Display a error message in a message box.
 *
 * Parameters: HWND         window handle
 *             PSZ          message to display
 *
 * Result:  none
 *
 ***************************************************************************/
VOID DisplayErrorMessage( HWND hwndDraw, PSZ Message)
{
   RECTL      rDraw;
   HPS        hpsDraw;

   /* Display the error message */
   WinMessageBox( HWND_DESKTOP, hwndDraw, Message,
                                      "Graphics Sample Application", 0, MB_OK);

   /* Clear the client area */
   WinQueryWindowRect( hwndDraw, &rDraw);              /* query window size */

   hpsDraw = WinGetPS( hwndDraw);

   WinFillRect( hpsDraw, &rDraw, CLR_BACKGROUND);         /* clear window   */

   WinReleasePS(hpsDraw);
}  /* End of DisplayErrorMessage */

/**************************************************************************
 * Function: PrintGpiMeta
 *
 * Create a structure with the printer specific data and send a message to
 * the print thread to print GPI or METAFILE.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *             ULONG        identifier if GPI or METAFILE
 *
 * Result:  none
 *
 ***************************************************************************/
VOID PrintGpiMeta(PMAIN_PARM Parm, ULONG Message)
{
   PPRTTXT_DATA PrintData;
   PDRAW_PARM PrtParm;
   /* definitions for print */

   /* Allocate Space for PrintData */
   PrintData = malloc(sizeof(PRTTXT_DATA));

   /* Allocate Space for PrtParm which is used in Draw-Functions */
   PrtParm = (PDRAW_PARM)malloc(sizeof(DRAW_PARM));

   /* now copy all additional information to the print structure */
   strcpy(PrintData->QueueName, Parm->QueueName);
   strcpy(PrintData->DriverName, Parm->DriverName);

   /* Allocate space for DriverData */
   PrintData->DriverData = malloc(Parm->DriverDataLength);

   memcpy(PrintData->DriverData, Parm->DriverData, Parm->DriverDataLength);
   memcpy((PSZ)PrtParm, (PSZ)&Parm->DrawParm, sizeof(DRAW_PARM));
   PrtParm->fDrawing = TRUE;
   PrintData->hWndFrame = Parm->hWndFrame;

   strcpy(PrintData->Jobname, ((Message == DRAW_GPI) ? "GPI" : "METAFILE"));

   /* now send the print thread a message to print GPI or METAFILE */
   WinPostQueueMsg(Parm->hMQPrint, Message,
                MPFROMP(PrintData), MPFROMP(PrtParm));
} /* End of PrintGpiMeta */

/*************************************************************************
 * Function: PrintThread
 *
 * PrintThread is the asynchronous printing thread.  It performs all
 * printing, text and graphic.
 * It obtains an anchor block handle and creates a message queue.
 *
 * Parameters: PMAIN_PARM   main parameter structure
 *
 * Result:  none
 *
 ***************************************************************************/
VOID PrintThread(PMAIN_PARM Parm)
{
         HAB        habPrint;
static   CHAR       PrintThreadErrorTitle[] = "Print Thread Error:";
static   CHAR       PrintThreadError1[] = "WinInitialize failed!";
static   CHAR       PrintThreadError2[] = "Cannot Create Message Queue.";
         BOOL       Loop = TRUE;
         QMSG       qmsgAsync;      /* Message structure for WinGetMsg  */
 /*
  * Obtain an anchor block handle. If this fails, the thread and
  * the application are terminated.
  */
   if ((habPrint = WinInitialize(0L)) == (HAB)NULLHANDLE)
   {
      WinPostMsg(Parm->hWndFrame,
              WM_USER_END_ON_ERROR,
              MPFROMP(PrintThreadErrorTitle),
              MPFROMP(PrintThreadError1));
      DosExit(EXIT_THREAD, 0);
   }
 /*
  *Create a message queue. If this fails, the thread and the
  * application are terminated.
  */
   if ((Parm->hMQPrint = WinCreateMsgQueue(habPrint, 0L)) == (HMQ)NULLHANDLE)
   {
      WinTerminate(habPrint);
      WinPostMsg(Parm->hWndFrame,
              WM_USER_END_ON_ERROR,
              MPFROMP(PrintThreadErrorTitle),
              MPFROMP(PrintThreadError2));
      DosExit(EXIT_THREAD, 0);
   }

   DosSetPriority(PRTYS_THREAD, PRTYC_NOCHANGE, 1L, 0L);

   while (Loop)
   {
      WinGetMsg(habPrint, &qmsgAsync, (HWND)NULLHANDLE, 0L, 0L);
      switch (qmsgAsync.msg)
      {
      case STOP_THREAD:                /* end thread        */
           Loop = FALSE;
           break;
      case DRAW_GPI:                   /* print GPI/METAFILE   */
      case DRAW_META:
           MetaOutput((PDRAW_PARM)PVOIDFROMMP(qmsgAsync.mp2),
                      (PPRTTXT_DATA)PVOIDFROMMP(qmsgAsync.mp1),
                      habPrint);
           break;
      }
   }

   DosSetPriority(PRTYS_THREAD, PRTYC_IDLETIME, 0L, 0L);

   WinDestroyMsgQueue(Parm->hMQPrint); /* Destroy this thread's message queue */

   WinTerminate(habPrint);

   DosPostEventSem(Parm->hevsemPrintEnds);

   DosExit(EXIT_THREAD, 0);
} /* End of PrintThread */

/**************************************************************************
 * Function: PreparePrinting
 *
 * Create device context and presentation space for printing
 *
 * Parameters: PHPS         presentations space handle for printing(return)
 *             PHDC         device context handle for printing (return)
 *             HAB          anchor block handle of the print process
 *             PSIZEL       size of the presentation space (return)
 *             PRESOLUTION  resolution of the device (return)
 *             PPRTTXT_DATA print data structure
 *             PSIZEL       device capabilities
 *
 * Result:  none
 *
 ***************************************************************************/
static VOID PreparePrinting(PHPS hpsPrint,
                            PHDC hdcPrinter,
                            HAB habPrint,
                            PSIZEL sizlPS,
                            PRESOLUTION Resolution,
                            PPRTTXT_DATA PrintData,
                            PSIZEL DevCaps)
{
         DEVOPENSTRUC dopPrinter;
         ULONG     i1;
static   CHAR       Comment[] = "Graphic";
         LONG       alDevInfo[DEVINFOSIZE];

   /* initialize DEVOPENSTRUC */
   memset((PVOID)&dopPrinter, '\0', sizeof(DEVOPENSTRUC));
   dopPrinter.pszLogAddress = PrintData->QueueName;
   i1 = strcspn(PrintData->DriverName, ".");
   PrintData->DriverName[i1] = '\0';
   dopPrinter.pszDriverName = PrintData->DriverName;
   dopPrinter.pdriv = (PDRIVDATA)PrintData->DriverData;
   dopPrinter.pszDataType = NULL;

   dopPrinter.pszComment = Comment;
   /* Open device context for printer */
   *hdcPrinter = DevOpenDC(habPrint,
                           OD_QUEUED,
                           (PSZ)"*",
                           5L,
                           (PDEVOPENDATA)&dopPrinter,
                           (HDC)NULLHANDLE);
   /* Query Height and Width of the printer */

   DevQueryCaps(*hdcPrinter,
                0L,
                (LONG)DEVINFOSIZE,
                alDevInfo);

   sizlPS->cx = (alDevInfo[CAPS_WIDTH] * 10000) /
                alDevInfo[CAPS_HORIZONTAL_RESOLUTION];

   sizlPS->cy = (alDevInfo[CAPS_HEIGHT] * 10000) /
                alDevInfo[CAPS_VERTICAL_RESOLUTION];

   /* return device capabilities if pointer not NULL */
   if (DevCaps != (PSIZEL)NULL)
      {
      DevCaps->cx = alDevInfo[CAPS_WIDTH];
      DevCaps->cy = alDevInfo[CAPS_HEIGHT];
      }

   Resolution->x = (alDevInfo[CAPS_HORIZONTAL_FONT_RES] * 10000) /
                           alDevInfo[CAPS_HORIZONTAL_RESOLUTION];
   Resolution->y = (alDevInfo[CAPS_VERTICAL_FONT_RES] * 10000) /
                           alDevInfo[CAPS_VERTICAL_RESOLUTION];

   /* create presentation space */
   *hpsPrint = GpiCreatePS(habPrint,
                           *hdcPrinter,
                           sizlPS,
                           PU_LOMETRIC | GPIA_ASSOC | GPIT_NORMAL);
} /* End of PreparePrinting */

/**************************************************************************
 * Function: MetaOutput
 *
 * Create spoolfile for GPI or METAFILE.
 *
 * Parameters: PDRAW_PARM   draw parameter structure
 *             PPRTTXT_DATA print parameter structure
 *             HAB          anchor block handle of the print process
 *
 * Result:  none
 *
 ***************************************************************************/
static VOID MetaOutput(PDRAW_PARM PrtParm,
                       PPRTTXT_DATA PrintData,
                       HAB habPrint)
{
   HDC        hdcPrinter;
   RESOLUTION Resolution;
   SIZEL      sizlPS;
   HPS        hpsPrint;
   ULONG      JobNr;
   ULONG      Dummy;
   RECTL      rclPrinter;

   PrtParm->hABDraw = habPrint;

   /* create device context and presentations space for printing */
   PreparePrinting(&hpsPrint, &hdcPrinter, habPrint,
                   &sizlPS, &Resolution, PrintData, (PSIZEL)NULL);

   /* disable menuitem "Print" */
   WinPostMsg(PrintData->hWndFrame, SET_PRINT_STATUS, MPFROMSHORT(1),
              MPFROMLONG(hpsPrint));

   /* Start Document */
   DevEscape(hdcPrinter,
             DEVESC_STARTDOC,
             (LONG)strlen(PrintData->Jobname),
             (PSZ)PrintData->Jobname,
             0L,
             (PSZ)NULL);

   rclPrinter.xLeft = rclPrinter.yBottom = 0;
   rclPrinter.xRight = sizlPS.cx;
   rclPrinter.yTop = sizlPS.cy;

   /* draw GPI/METAFILE to the print presentation space */
   Transform(PrtParm, hpsPrint, &rclPrinter);

   if (DoDraw(hpsPrint, PrtParm, (HRGN)NULLHANDLE, (HWND)NULLHANDLE) != DRAW_OK)
      DevEscape(hdcPrinter,
                DEVESC_ABORTDOC,
                0L,
                (PSZ)NULL,
                (PLONG)NULL,
                (PSZ)NULL);
   else
      {
      DevEscape(hdcPrinter, DEVESC_NEWFRAME,
                0L, (PSZ)NULL, (PLONG)NULL, (PSZ)NULL);
      DevEscape(hdcPrinter,
                DEVESC_ENDDOC,
                0L,
                (PSZ)NULL,
                (PLONG)&Dummy,
                (PSZ)&JobNr);
      }

   GpiAssociate(hpsPrint, (HDC)NULLHANDLE);
   GpiDestroyPS(hpsPrint);
   DevCloseDC(hdcPrinter);

   /* Release Data fields */
   free((PVOID)PrintData->DriverData);

   WinPostMsg(PrintData->hWndFrame, SET_PRINT_STATUS, MPVOID, MPVOID);

   free((PVOID)PrintData);
} /* End of MetaOutput */

/*************************************************************************
 * Function: QueryPrinterQueue
 *
 * query all printer queues, display them in a listbox an do a selection.
 *
 * Parameters: HAB          anchor block handle of main process
 *             HWND         window handle of frame window
 *             PVOID        pointer to driver data structure
 *             PULONG       length of driver data
 *             PSZ          (pre)selected queue name
 *             PSZ          (pre)selected printer driver name
 *
 * Result:  BOOL            TRUE if printer changed, otherwise FALSE
 *
 ***************************************************************************/
BOOL QueryPrinterQueue(HAB hAB,
                       HWND hWnd,
                       PVOID *DriverData,
                       PULONG DriverDataLength,
                       PSZ QueueName,
                       PSZ DriverName)
{
         PSZ        pszQueueNames;
         PSZ        pszQueueName;
         CHAR       DefaultQueue[10];
static   CHAR       pm_spooler_queue_descr[] = "PM_SPOOLER_QUEUE_DESCR";
         PRINTER_SEL_PARM PrtSelParm;
         ULONG     i1;
         BOOL       rc;
         LONG      SelectedItem;

   /* initialize printer selection structure */
   PrtSelParm.ulLen = sizeof(PRINTER_SEL_PARM);
   PrtSelParm.ulType = 0L;                   /* reserved, must be zero     */
   /*
    * First query all printer queues with a default driver name defined on it
    * If it fails, bring up a Message Box
    */
   if ((pszQueueNames = GetQueueNamesString(&PrtSelParm.QueuesCount)) == NULL)
      {
      WinMessageBox(HWND_DESKTOP,
                    hWnd,
                    (PSZ)"There are no Printer Queues defined, use PrintManager to define at least one Queue and Printer.",
                    (PSZ)NULL,
                    0L,
                    MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
      return(FALSE);
      }

   /* if no Queue is connected to a Printer Driver, bring up a message */
   if (PrtSelParm.QueuesCount == 0)
      {
      WinMessageBox(HWND_DESKTOP,
                    hWnd,
                    (PSZ)"There are no Printer Queues connected to a Printer Driver, use PrintManager to connect a Printer to the Queue.",
                    (PSZ)NULL,
                    0L,
                    MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
      free(pszQueueNames);
      return(FALSE);
      }

   /* Allocate Printer Selection Table */
   PrtSelParm.QueueSelTable = malloc(PrtSelParm.QueuesCount * sizeof(QUEUE_SEL));
   /*
    * Fill Printer Selection Table with Queue-Names and connected Printer
    * Driver Names
    */
   for (pszQueueName = pszQueueNames, i1 = 0;
        *pszQueueName;
        pszQueueName = &pszQueueName[strlen(pszQueueName) + 1], i1++)
      {
      PrfQueryProfileString(HINI_PROFILE,
                            (PSZ)pm_spooler_queue_dd,
                            (PSZ)pszQueueName,
                            (PSZ)NULL,
                            (PVOID)PrtSelParm.QueueSelTable[i1].DriverName,
                            (ULONG)DRIVERNAME_LENGTH);

      strcpy(PrtSelParm.QueueSelTable[i1].QueueName, pszQueueName);
      /* eliminate ; */
      PrtSelParm.QueueSelTable[i1].DriverName[strlen(
             PrtSelParm.QueueSelTable[i1].DriverName) - 1] = '\0';

      /* Query Queue-Description */
      PrfQueryProfileString(HINI_PROFILE,
                            (PSZ)pm_spooler_queue_descr,
                            (PSZ)pszQueueName,
                            (PSZ)NULL,
                            (PVOID)PrtSelParm.QueueSelTable[i1].QueueDescription,
                            (ULONG)DESCRIPTION_LENGTH);
      /* eliminate ; */
      PrtSelParm.QueueSelTable[i1].QueueDescription[strlen(
             PrtSelParm.QueueSelTable[i1].QueueDescription) - 1] = '\0';
      PrtSelParm.QueueSelTable[i1].DriverData = NULL;
      PrtSelParm.QueueSelTable[i1].DriverDataLength = 0;
      }

   free(pszQueueNames);                /* free queue names */

   /* Sort Queue-Descriptions */
   qsort((void *)PrtSelParm.QueueSelTable,
         (size_t)PrtSelParm.QueuesCount,
         (size_t)sizeof(QUEUE_SEL),
          SortQueues);

   /* Define preselected queue */
   PrtSelParm.SelectedItem = -1;

   strcpy(DefaultQueue, QueueName);
   /*
    * if Queue Name not given or not found in table, query Default Queue
    * if queue name defined, test if the queue is valid
    */
   if (QueryDefaultQueue(DefaultQueue, (PSZ)NULL))
      {
      /* If default Queue defined and connected to Driver, search table */
      for (i1 = 0; i1 < PrtSelParm.QueuesCount; i1++)
         {
         /* if found in table, set Selected Item to entry */
         if (!strcmp(DefaultQueue, PrtSelParm.QueueSelTable[i1].QueueName))
            {
            PrtSelParm.SelectedItem = SelectedItem = i1;
            PrtSelParm.QueueSelTable[i1].DriverData = *DriverData;
            PrtSelParm.QueueSelTable[i1].DriverDataLength = *DriverDataLength;
            break;
            }
         }
      }

   /* if no queue defined or queue not valid, free driver data if allocated */
   if ((SelectedItem = PrtSelParm.SelectedItem) == -1)
      if (*DriverData != NULL)
      {
         free(*DriverData);
         *DriverDataLength = 0;
         *DriverData = NULL;
      }

   PrtSelParm.hAB = hAB;

   /* if an other printer is selected, copy the data to the function parameters */
   if (PrtSelParm.SelectedItem != SelectedItem)
      {
      rc = TRUE;
      strcpy(QueueName,
             PrtSelParm.QueueSelTable[PrtSelParm.SelectedItem].QueueName);
      strcpy(DriverName,
             PrtSelParm.QueueSelTable[PrtSelParm.SelectedItem].DriverName);
      *DriverData = PrtSelParm.QueueSelTable[PrtSelParm.SelectedItem].DriverData;
      *DriverDataLength = PrtSelParm.QueueSelTable[PrtSelParm.SelectedItem].DriverDataLength;
      }
   else
      rc = FALSE;

   /* free fields in queue selection table */
   for (i1 = 0; i1 < PrtSelParm.QueuesCount; i1++)
      if ((i1 != (ULONG)PrtSelParm.SelectedItem) &&
          (PrtSelParm.QueueSelTable[i1].DriverData != NULL))
      {
         free(PrtSelParm.QueueSelTable[i1].DriverData);
      }

   free(PrtSelParm.QueueSelTable);

   return(rc);
} /* End of QueryPrinterQueue */

/**************************************************************************
 * Function: SortQueues
 *
 * Compare routine for sorting the queue names table.
 *
 * Parameters: PQUEUE_SEL   pointer to queue selection structure 1
 *             PQUEUE_SEL   pointer to queue selection structure 2
 *
 * Result:  int             result of comparison
 *
 ***************************************************************************/
static int _Optlink SortQueues(const void *Queue1, const void *Queue2)
{
   return(strcmp(((PQUEUE_SEL)Queue1)->QueueDescription,
              ((PQUEUE_SEL)Queue2)->QueueDescription));
} /* End of SortQueues */

/**************************************************************************
 * Function: QueryDefaultQueue
 *
 * if queue name passed, test if queue is valid, otherwise query default
 * printer queue.
 *
 * Parameters: PSZ          queue name or nullstring
 *             PSZ          queue driver name or nullstring
 *
 * Result:  BOOL            TRUE if driver is connected to queue
 *
 ***************************************************************************/
BOOL QueryDefaultQueue(PSZ QueueName, PSZ QueueDriver)
{
         PSZ        pszQueueNames;
         PSZ        pszQueueName;
         CHAR       QueueNameTemp[DRIVERNAME_LENGTH];
         BOOL       rc = FALSE;
static   CHAR       pm_spooler[] = "PM_SPOOLER";

/* if no queue name passed, query default printer queue */
   if (!*QueueName)
   {
      if (!PrfQueryProfileString(HINI_PROFILE,
                              (PSZ)pm_spooler,
                              (PSZ)"QUEUE",
                              NULL,
                              (PSZ)QueueNameTemp,
                              (LONG)sizeof(QueueNameTemp)))
      {
         *QueueName = '\0';      /* if no default printer queue found,  */
         return(FALSE);          /* return FALSE                        */
      }

   /* cut ; */
      QueueNameTemp[strlen(QueueNameTemp) - 1] = '\0';

   /* return Queue Name */
      strcpy(QueueName, QueueNameTemp);
   }

/* Get String with all Queue Names to which a driver is connected */
   if ((pszQueueNames = GetQueueNamesString((PULONG)NULL)) != NULL)
   {
   /* Compare all Queue Names with default Queue Name */
      for (pszQueueName = pszQueueNames;
        *pszQueueName;
        pszQueueName = &pszQueueName[strlen(pszQueueName) + 1])
      {
         if (!strcmp(pszQueueName, QueueName))
         {
         /* if Queue found in string, set rc = TRUE */
            rc = TRUE;
            break;
         }
      }
   /* Free Queue Names string */
      free(pszQueueNames);
   }

/* if queue driver defined and Queue found, query queue driver name  */
   if ((QueueDriver != (PSZ)NULL) && rc)
   {
      PrfQueryProfileString(HINI_PROFILE,
                         (PSZ)pm_spooler_queue_dd,
                         (PSZ)QueueName,
                         NULL,
                         (PSZ)QueueNameTemp,
                         DRIVERNAME_LENGTH);
      QueueNameTemp[strlen(QueueNameTemp) - 1] = '\0';
      strcpy(QueueDriver, QueueNameTemp);
   }

   return(rc);
} /* End of QueryDefaultQueue */

/**************************************************************************
 * Function: GetQueueNamesString
 *
 * Get a string with all Queue Names with a connected Driver Name.
 *
 * Parameters: PULONG       pointer to ULONG for returning the queue count
 *
 * Result:  PSZ             string with all queuenames
 *
 ***************************************************************************/
static PSZ GetQueueNamesString(PULONG QCount)
{
   ULONG      QueueNamesLength;
   PSZ        pszQueueNames;
   PSZ        pszQueueNamesWithDriver;
   PSZ        pszQueueName;
   PSZ        pszQueueStringPtr;
   PSZ        pszQueueDriver;
   ULONG     QueuesCt;

   /* Query size of Queues-String, if not defined return NULL */
   if (!PrfQueryProfileSize(HINI_PROFILE,
                            (PSZ)pm_spooler_queue_dd,
                            (PSZ)NULL,
                            (PULONG)&QueueNamesLength))
      return((PSZ)NULL);

   /* Allocate Space for Queue Names, if it fails return NULL */
   if ((pszQueueNames = malloc(QueueNamesLength)) == NULL)
      return((PSZ)NULL);

   /* Allocate Space for Queue Names with driver, if it fails return NULL */
   if ((pszQueueNamesWithDriver = malloc(QueueNamesLength)) == NULL)
      {
      free(pszQueueNames);
      return((PSZ)NULL);
      }

   /* Get Queue Names */
   PrfQueryProfileString(HINI_PROFILE,
                         (PSZ)pm_spooler_queue_dd,
                         (PSZ)NULL,
                         (PSZ)NULL,
                         (PVOID)pszQueueNames,
                         QueueNamesLength);
   /*
    * Query Count of Queues to which a Printer Driver is connected and
    * copy all Queue Names to pszQueueNamesWithDriver
    * Allocate space for Queue Driver Name, if it fails return NULL
    */
   if ((pszQueueDriver = malloc(DRIVERNAME_LENGTH)) == NULL)
      {
      free(pszQueueNames);
      free(pszQueueNamesWithDriver);
      return((PSZ)NULL);
      }

   pszQueueStringPtr = pszQueueNamesWithDriver;

   for (pszQueueName = pszQueueNames, QueuesCt = 0;
        *pszQueueName;
        pszQueueName = &pszQueueName[strlen(pszQueueName) + 1])
   {
      /* Query Queue Driver */
      PrfQueryProfileString(HINI_PROFILE,
                            (PSZ)pm_spooler_queue_dd,
                            (PSZ)pszQueueName,
                            (PSZ)NULL,
                            (PVOID)pszQueueDriver,
                            DRIVERNAME_LENGTH);
      /*
       * if Queue Driver isn't a null-string, increment Queues Count
       * and copy queue name to string
       */
      if ((*pszQueueDriver != ';') && (*pszQueueDriver != '\0'))
      {
         strcpy(pszQueueStringPtr, pszQueueName);
         pszQueueStringPtr = &pszQueueStringPtr[strlen(pszQueueStringPtr) + 1];
         QueuesCt++;
      }
   }

   /* add a x'00' to the end of the string */
   *pszQueueStringPtr = '\0';

   /* free temporary fields */
   free(pszQueueDriver);
   free(pszQueueNames);

   /* Return Queues Count, if Pointer != NULL */
   if (QCount != (PULONG)NULL)
      *QCount = QueuesCt;

   /* return string with Queue Names */
   return(pszQueueNamesWithDriver);
} /* End of GetQueueNamesString */

/*************************************************************************
 * Function: QueryJobProperties
 *
 * Query job properties (print properties) of the printer driver.
 *
 * Parameters: HAB          anchor block handle of the process
 *             PVOID        pointer to driver data
 *             PULONG       driver data length
 *             PSZ          queue driver name
 *             PSZ          printer queue name
 *             PSZ          printer driver name
 *             BOOL         flag, if FALSE don't display the dialog
 *
 * Result:  BOOL            TRUE if ok
 *
 ***************************************************************************/
BOOL QueryJobProperties(HAB hAB,
                        PVOID *DriverData,
                        PULONG DriverDataLength,
                        PSZ QueueDriverName,
                        PSZ QueueName,
                        PSZ PrtName,
                        BOOL Flag)
{
         CHAR       DriverName[10];
         CHAR       DeviceName[64];
         LONG       rc;
         LONG      i1;
         ULONG      Length;
         PSZ        PrinterNames = NULL;
         PSZ        PrinterName;
         PSZ        PrinterInfo;
         PSZ        QueueInfo;
         PSZ        DriverInfo;
         PSZ        Queue;
         PSZ        Driver;
         BOOL       found;
         ULONG      flOptions;
static   CHAR       pm_spooler_printer[] = "PM_SPOOLER_PRINTER";
static   CHAR       separator1[] = ";";
static   CHAR       separator2[] = ",";

/* if flag=TRUE display the dialog, otherwise only query default properties */
   if (Flag)
      flOptions = DPDM_POSTJOBPROP;
   else
      flOptions = DPDM_QUERYJOBPROP;

   /* separate  driver name at the dot (e.g PLOTTERS.IBM6180) */
   if ((i1 = (LONG)strcspn(QueueDriverName, ".")) != 0L)
      {
      strncpy(DriverName, QueueDriverName, i1);
      DriverName[i1] = '\0';
      strcpy(DeviceName, &QueueDriverName[i1 + 1]);
      }
   else
      {
      strcpy(DriverName, QueueDriverName);
      *DeviceName = '\0';
      }
   /*
    * Query device name because some printer drivers need this information
    * in the DevPostDeviceModes call
    */

   /* Query the size needed to get the printer names */
   PrfQueryProfileSize(HINI_PROFILE,
                       (PSZ)pm_spooler_printer,
                       (PSZ)NULL,
                       (PULONG)&Length);

   /* Allocate space for printer names */
   PrinterNames = malloc((LONG)Length);

   /* Get the printer names */
   PrfQueryProfileString(HINI_PROFILE,
                         (PSZ)pm_spooler_printer,
                         (PSZ)NULL,
                         (PSZ)NULL,
                         (PVOID)PrinterNames,
                         Length);

   /* get Printer information and search for the specific queue */
   for (PrinterName = PrinterNames, found = FALSE;
        *PrinterName;
        PrinterName = &PrinterName[strlen(PrinterName) + 1])
      {
      /* Query the size needed to get the printer information */
      PrfQueryProfileSize(HINI_PROFILE,
                          (PSZ)pm_spooler_printer,
                          (PSZ)PrinterName,
                          (PULONG)&Length);

      PrinterInfo = malloc((ULONG)Length);

      PrfQueryProfileString(HINI_PROFILE,
                            (PSZ)pm_spooler_printer,
                            (PSZ)PrinterName,
                            (PSZ)NULL,
                            (PVOID)PrinterInfo,
                            Length);

      /* Get Driver Info and Queue Info of the string */
      DriverInfo = &PrinterInfo[strcspn(PrinterInfo, separator1) + 1];
      QueueInfo = &DriverInfo[strcspn(DriverInfo, separator1) + 1];
      QueueInfo[strcspn(QueueInfo, separator1)] = '\0';
      QueueInfo[-1] = '\0';

      /* Verify Queues with specific Queue Name */
      Queue = strchr(QueueInfo, separator2[0]);

      while (Queue)
         {
         /* if Queue found, compare Driver Names */
         if (!strcmp(Queue, QueueName))
            {
            Driver = strchr(DriverInfo, separator2[0]);
            while (Driver)
               {
               /* if Driver found, stop loop */
               if (!strcmp(Driver, QueueDriverName))
                  {
                  found = TRUE;
                  break;
                  }
               Driver = strchr(NULL, separator2[0]);
               }
            break;
            }
         Queue = strchr(NULL,separator2[0]);
         }

      free(PrinterInfo);

      if (found)
         break;
      }

   if (*DriverData == NULL)
      {
      /* Query length for Job-Properties */
      rc = DevPostDeviceModes(hAB,
                              NULL,
                              DriverName,
                              DeviceName,
                              PrinterName,
                              flOptions);

      if (rc <= 0L)
         {
         free(PrinterNames);
         return(FALSE);
         }

      *DriverData = malloc((LONG)rc);

      *DriverDataLength = (LONG)rc;
      }

   /* call DevPostDeviceModes */
   rc = DevPostDeviceModes(hAB,
                           *DriverData,
                           DriverName,
                           DeviceName,
                           PrinterName,
                           flOptions);

   if ((PrtName != (PSZ)NULL) && (PrinterName != NULL))
      strcpy(PrtName, PrinterName);

   if (PrinterNames != NULL)
      free(PrinterNames);

   return TRUE;
} /* End of QueryJobProperties */
/************************** End of FILE.C *********************************/

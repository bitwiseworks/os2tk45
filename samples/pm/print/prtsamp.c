/***************************************************************************
 *
 * File name   :  prtsamp.c
 *
 * Description :  This program demonstrates how to display and print ascii
 *                text, metafiles and bitmaps.  It also shows how to use
 *                the standard file dialog and the standard font dialog,
 *                how to display and change printer properties and how to
 *                display and select printer queues.
 *
 *                This source file contains the following functions:
 *
 *                main(argc, argv)
 *                WinProc(hwnd, msg, mp1, mp2)
 *                CalibrateHorizontalScrollBar(pmp)
 *                CalibrateVerticalScrollBar(pmp)
 *
 * Required
 *    files    :
 *                PRTSAMP.C        - Source code
 *                PRTCREAT.C       - Source code
 *                PRTMENU.C        - Source code
 *                PRTOBJ.C         - Source code
 *                PRTPRINT.C       - Source code
 *                PRTPROF.C        - Source code
 *                PRTSAMP.H        - Application header file
 *                PMASSERT.H       - Application header file
 *                PRTSDLG.H        - Application header file
 *                PRTSHLP.H        - Application header file
 *                PRTSAMP.RC       - Resource file
 *                PRTSAMP.DLG      - Dialog file
 *                PRTSAMP.IPF      - Help file
 *                PRTSAMP.MAK      - Make file for this program
 *                PRTSAMP.BMP      - Bitmap file
 *                PRTSAMP.TXT      - Text file
 *                PRTSAMP.MET      - Meta file
 *                PRTSAMP.ICO      - Print sample icon
 *
 *                OS2.H            - Presentation Manager include file
 *                STDIO.H          - Standard input/output c include file
 *                STDLIB.H         - Standard c include file
 *                STRING.H         - String handling function declarations
 *                CTYPE.H
 *                STDDEF.H
 *                PROCESS.H
 *                MEMORY.H
 *                SYS\TYPES.H
 *                SYS\STAT.H
 *
 *  Concepts   :  listboxes, directory search
 *
 *  API's      :  GpiAssociate
 *                GpiConvert
 *                GpiDeleteBitmap
 *                GpiDeleteMetafile
 *                GpiDestroyPS
 *                GpiDrawChain
 *                GpiSetDefaultViewMatrix
 *
 *                WinAddSwitchEntry
 *                WinAssociateHelpInstance
 *                WinBeginPaint
 *                WinCreateHelpInstance
 *                WinCreateMsgQueue
 *                WinCreateStdWindow
 *                WinDestroyHelpInstance
 *                WinDestroyMsgQueue
 *                WinDestroyWindow
 *                WinDispatchMsg
 *                WinEndPaint
 *                WinFillRect
 *                WinGetMsg
 *                WinInitialize
 *                WinInvalidateRect
 *                WinIsWindowEnabled
 *                WinLoadString
 *                WinMessageBox
 *                WinPostMsg
 *                WinQuerySysPointer
 *                WinQueryWindowRect
 *                WinQueryWindowULong
 *                WinRegisterClass
 *                WinRemoveSwitchEntry
 *                WinSendMsg
 *                WinSetPointer
 *                WinSetWindowText
 *                WinTerminate
 *
 *    Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PRTSHLP.H, PMASSERT.H
 *
 * Required
 *   libraries :  OS2386.LIB     - Presentation Manager/OS2 library
 *                C library
 *
 * Required
 *   programs  :  C Compiler
 *                Linker
 *                Resource Compiler
 *
 *  Copyright (C) 1991-1993 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *
 ******************************************************************************/

/* Include the required sections from the PM header files.  */
#define INCL_DEV
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_GPIMETAFILES
#define INCL_GPISEGMENTS
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINDIALOGS
#define INCL_WINFRAMEMGR
#define INCL_WINERRORS
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_WINMENUS
#define INCL_WINPOINTERS
#define INCL_WINSCROLLBARS
#define INCL_WINSTDDRAG
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINWINDOWMGR
#include <os2.h>

/* c language includes */
#include <ctype.h>
#include <memory.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\types.h>

/* application includes */
#include "pmassert.h"
#include "prtsamp.h"
#include "prtsdlg.h"
#include "prtshlp.h"

/**************************************************************************
 *
 *  Name       : main(argc, argv)
 *
 *  Description: Initializes the application, gets and dispatches messages
 *               for the client window on thread 1 and terminates the
 *               application when a WM_QUIT message is received.  It then
 *               saves the configuration in os2.ini, destroys all OS/2
 *               PM resources, and terminates.
 *
 *               The following routine is the Presentation Manager program
 *               main body. The main body of a PM program is concerned with
 *               associating the application with the Presentation Manager
 *               system, creating its message queue, registering and displaying
 *               its main window, servicing its message queue during the time
 *               that the application is active, and disassociating the
 *               application from PM when the user is finished with the
 *               it. The remaining parts of this source module that are
 *               concerned with the Presentation Manager are the application's
 *               window procedures (main window procedure, child window
 *               procedures, and dialog window procedures) that process the
 *               messages associated with the application's various windows.
 *
 *  Concepts   : - obtains anchor block handle and creates message
 *                   queue
 *               - creates the main frame window which creates the
 *                   main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  API's      :  WinInitialize
 *                WinCreateMsgQueue
 *                WinRegisterClass
 *                WinLoadString
 *                WinCreateHelpInstance
 *                WinMessageBox
 *                WinCreateStdWindow
 *                WinQueryWindowULong
 *                WinAssociateHelpInstance
 *                WinAddSwitchEntry
 *                WinGetMsg
 *                WinDispatchMsg
 *                WinRemoveSwitchEntry
 *                WinDestroyHelpInstance
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *                WinTerminate
 *
 *  Parameters :  [none]
 *
 *  Return     :  Always returns zero.  The ANSI standard requires that
 *                main be declared as an INT.
 *
 *************************************************************************/
INT main(INT argc, CHAR **argv)
{
                                        /* Define variables by type     */
   BOOL       bOK;                      /* Boolean used for return code */
   HAB        hab;                      /* PM anchor block handle       */
   HMQ        hmq;                      /* Message queue handle         */
   QMSG       qmsg;                     /* Message from message queue   */
   ULONG      ulCtlData;                /* Standard window create flags */
   HWND       hwndFrame;                /* Frame window handle          */
   HWND       hwndClient;               /* Client area window handle    */
   HWND       hwndHelp;                 /* Help window handle           */
   PMAIN_PARM pmp;                      /* Main parameters structure    */
   SWCNTRL    swctl;                    /* Struct to add to window list */
   HSWITCH    hsw;                      /* Window list handle ret'd     */
   HELPINIT   hmi;                      /* Struct for help creation     */
   CHAR       szWork[ LEN_WORKSTRING ]; /* General use string work area */
   PSZ        pszTitle;                 /* Pointer to program title     */


   /* normal PM application startup */
   hab = WinInitialize( 0 );
   hmq = WinCreateMsgQueue( hab, 0 );

   /*
    * Register a class for my client window's behavior.
    * This class has enough extra window words to hold a pointer.
    */
   bOK = WinRegisterClass(
                     hab,
                     CLASSNAME,
                     WinProc,
                     CS_SIZEREDRAW,
                     sizeof( PMAIN_PARM ));
   /*
    * Ensure WinRegisterClass worked ok; if not, present a message box.
    * ( See pmassert.h. )
    */
   pmassert( hab, bOK );


   /* Load program title and allocate a local copy. Use it in help creation. */
   WinLoadString( hab, (HMODULE)NULLHANDLE,
                  PROGRAM_TITLE, LEN_WORKSTRING, szWork );
   pszTitle = strdup( szWork );

   /* Create help. */
   memset( &hmi, 0, sizeof( hmi ));
   hmi.cb                 = sizeof( HELPINIT );
   hmi.phtHelpTable       = (PVOID)(0xffff0000 | ID_PRTSAMP);
   hmi.pszHelpWindowTitle = pszTitle;
   hmi.pszHelpLibraryName = HELP_FILE;
   hwndHelp = WinCreateHelpInstance( hab, &hmi );
   if( ! hwndHelp )
   {
      WinLoadString(hab, (HMODULE)NULLHANDLE,
                    ERRMSG_NO_HELP, LEN_WORKSTRING, szWork);
      WinMessageBox( HWND_DESKTOP,
                     HWND_DESKTOP,
                     szWork,
                     pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
   }

   /* done with local copy of title text */
   free( pszTitle );


   /* flags to control creation of window; use on call to WinCreateStdWindow */
   ulCtlData = FCF_SYSMENU | FCF_TITLEBAR | FCF_SIZEBORDER
                           | FCF_MINMAX | FCF_MENU | FCF_ACCELTABLE
                           | FCF_HORZSCROLL | FCF_VERTSCROLL;

   hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
                                   WS_VISIBLE | FS_ICON | FS_SHELLPOSITION,
                                   &ulCtlData,
                                   CLASSNAME,
                                   NULL,    /* title text set in prtcreat.c */
                                   0,       /* client style                 */
                                   (HMODULE)NULLHANDLE,  /* resources in exe */
                                   ID_PRTSAMP,
                                   &hwndClient  );

   pmassert( hab, hwndFrame );
   pmassert( hab, hwndClient );

   /* create.c placed pointer to main params in client's window words; get it */
   pmp = (PMAIN_PARM) WinQueryWindowULong( hwndClient, QWL_USER );
   /* store help instance handle in main parameter block */
   pmp->hwndHelp  = hwndHelp;

   bOK = WinAssociateHelpInstance( hwndHelp, hwndFrame );
   pmassert( hab, bOK );

   /* Add program to task list. */
   memset( &swctl, 0, sizeof( SWCNTRL ));
   strcpy( swctl.szSwtitle, pmp->pszTitle );
   swctl.hwnd          = hwndFrame;
   swctl.uchVisibility = SWL_VISIBLE;
   swctl.fbJump        = SWL_JUMPABLE;
   hsw = WinAddSwitchEntry( &swctl );

   /* first parameter could be a filename, so store for later use */
   if (argc > 1)
   {
      argv++;
      strcpy( pmp->szArgFilename, *argv);
   }

   /* message loop */
   while( WinGetMsg( hab, &qmsg, (HWND)NULLHANDLE, 0, 0 ))
   {
     WinDispatchMsg( hab, &qmsg );
   }

   /* clean up */
   WinRemoveSwitchEntry( hsw );
   WinDestroyHelpInstance( hwndHelp );
   WinDestroyWindow( hwndFrame );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

   DosWaitThread( &pmp->tidObjectThread, DCWW_WAIT );
   return 0;
}  /* End of main() */


/*************************************************************************
 *
 * Name       : WinProc(hwnd, msg, mp1, mp2)
 *
 * Description: Processes the messages sent to the main client
 *              window.  This routine processes the basic
 *              messages all client windows should process.
 *
 * Concepts   : This procedure provides service routines for the general
 *              PM events (messages) that PM sends to the window, as well
 *              as the user initiated events (messages) that are generated
 *              when the user selects the action bar and pulldown menu
 *              controls or the corresponding keyboard accelerators.
 *
 *              The switch statement shown below distributes the window
 *              messages to the respective message service routines, which
 *              are set apart by the case statements.  The window
 *              procedures must provide an appropriate service routine for
 *              its end user initiated messages, as well as the general PM
 *              messages (like the WM_CLOSE message). If a message is sent
 *              to this procedure for which there is no programmed case
 *              clause (i.e., no service routine), the message is defaulted
 *              function WinDefWindowProc, where it is disposed of by PM.
 *
 *              Time-consuming tasks are posted to the object window where
 *              they can be performed without the 1/10 second turnaround
 *              time imposed on the client window. When the object window
 *              is busy, the client window is usually disabled. The object
 *              window then posts an acknowledgement
 *              back to the client when the lengthy task is completed.
 *
 *              Cases under the big switch appear in alphabetical order.
 *
 *  API's     :   WinLoadString
 *                WinMessageBox
 *                WinQueryWindowULong
 *                WinSendMsg
 *                WinPostMsg
 *                WinIsWindowEnabled
 *                WinSetPointer
 *                WinQuerySysPointer
 *                WinBeginPaint
 *                WinQueryWindowRect
 *                WinFillRect
 *                WinEndPaint
 *                WinInvalidateRect
 *                WinSetWindowText
 *                GpiDrawChain
 *                GpiConvert
 *                GpiSetDefaultViewMatrix
 *                GpiAssociate
 *                GpiDestroyPS
 *                GpiDeleteBitmap
 *                GpiDeleteMetafile
 *
 * Parameters : HWND         window handle
 *              ULONG        message
 *              MPARAM       message parameter 1
 *              MPARAM       message parameter 2
 *
 * Result     : MRESULT      message result
 *
 *************************************************************************/
MRESULT EXPENTRY WinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   BOOL                bOK;
   PMAIN_PARM          pmp;
   HPS                 hps;
   RECTL               rectl;
   PSZ                 psz;
   CHAR                szWork[ LEN_WORKSTRING ];
   SHORT               sStep;
   ULONG               ulWork;
   SHORT               sNewPos;

   switch(msg)
   {
   case WM_CLOSE:
      /* obtain the main parameter pointer from window words */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      if( pmp->fBusy )
      {
         /* OK to close when object window is busy?  */
         WinLoadString( pmp->hab, (HMODULE)NULLHANDLE,
                   ERRMSG_CLOSE_QUESTION, LEN_WORKSTRING, szWork );
         ulWork = WinMessageBox( HWND_DESKTOP,
                                 pmp->hwndFrame,
                                 szWork,
                                 pmp->pszTitle,
                                 (USHORT)0,
                                 MB_YESNOCANCEL | MB_MOVEABLE |
                                 MB_CUANOTIFICATION | MB_APPLMODAL);

         if( ulWork == MBID_YES )
         {
            /* close down the application in spite of being busy */
            pmp->fCancel = TRUE;
            /* disable client during exit */
            WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0 , (MPARAM)0  );
            /* start a watchdog timer to ensure a timely death */
            WinStartTimer( pmp->hab, hwnd, ID_DEATH_TIMER, LEN_DEATH_TIMER );
            /* Tell object window to close, quit, and post me a WM_QUIT */
            WinPostMsg( pmp->hwndObject, WM_USER_CLOSE, (MPARAM)hwnd, (MPARAM)0 );
         }
      }
      else
      {
         /* not busy, so initiate closure by telling object window to close */
         WinPostMsg( pmp->hwndObject, WM_USER_CLOSE, (MPARAM)hwnd, (MPARAM)0 );
      }
      return (MRESULT)NULL;


   case WM_COMMAND:
      /* do menu activities; see menu.c */
      return Menu( hwnd, msg, mp1, mp2 );


   case WM_CREATE:
      /*
       * Do one-time, startup processing in PRTCREAT.C.
       * This function allocates the pmp, a pointer to the program's
       * main parameters. See the declaration of this block of
       * parameters in PRTSAMP.H, the MAIN_PARM structure.
       *
       * The Create() function allocates this structure and
       * begins to initialize it. Throughout all the code, the pmp->
       * pointer is usually obtainable with a call to WinQueryWindowULong.
       * Window word space for this pointer was reserved on the call
       * to WinRegisterClass.
       *
       * Create() allocates, initializes, and stores the pmp pointer
       * in the client window words. It then starts thread 2 of the
       * application on which the object window operates. The pmp
       * pointer is passed to thread 2 with _beginthread in PRTCREAT.C.
       * The pmp is passed to the object window on the call to
       * WinCreateWindow in PRTOBJ.C. Finally, the object window stores
       * the pmp in its window words under the WM_CREATE case of the
       * ObjectWinProc in PRTOBJ.C.
       *
       */
      Create( hwnd );
      return (MRESULT)NULL;


   case WM_HSCROLL:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /* Compute some fraction of scroll bar range for a unit of scrolling. */
      sStep  = pmp->sHScrollRange / 50;

      switch( SHORT2FROMMP( mp2 ))
      {
      case SB_LINELEFT:
         pmp->sHScrollPos -= sStep;
         break;
      case SB_PAGELEFT:
         pmp->sHScrollPos -= pmp->sizelClient.cx;
         break;
      case SB_LINERIGHT:
         pmp->sHScrollPos += sStep;
         break;
      case SB_PAGERIGHT:
         pmp->sHScrollPos += pmp->sizelClient.cx;
         break;
      case SB_SLIDERPOSITION:
      case SB_SLIDERTRACK:
         pmp->sHScrollPos = SHORT1FROMMP( mp2 );
         break;
      }

      /* Don't allow step assignments to exceed limits of zero to range. */
      pmp->sHScrollPos = max( (SHORT)0,
                              min( pmp->sHScrollPos, pmp->sHScrollRange ));

      if( pmp->sHScrollPos != SHORT1FROMMR(WinSendMsg( pmp->hwndHScroll,
          SBM_QUERYPOS, (MPARAM)0, (MPARAM)0 )))
      {
         /*
          * New scroll bar thumbbutton position is different than current.
          * Set a new X translation value to effect the scroll.
          * Current scale setting affects the X element of the matrix.
          */
         pmp->matlfDefView.lM31 = OFFSET_XY_TWIPS - (LONG)(pmp->floatScale *
                                                 (float)( pmp->sHScrollPos));
         bOK = GpiSetDefaultViewMatrix( pmp->hpsClient,
                                        9,
                                        &pmp->matlfDefView,
                                        TRANSFORM_REPLACE );
         pmassert( pmp->hab, bOK );
         CalibrateHorizontalScrollBar( pmp );
         WinInvalidateRect( hwnd, NULL, FALSE );
      }
      return (MRESULT) 0;


   case WM_MOUSEMOVE:
      /* display which pointer? -- could query pmp->fBusy or... */
      if( WinIsWindowEnabled( hwnd ))
      {
         /* not disabled; display regular pointer */
         WinSetPointer( HWND_DESKTOP,
                        WinQuerySysPointer( HWND_DESKTOP, SPTR_ARROW, FALSE ));
      }
      else
      {
         /* disabled; display hourglass because I'm busy */
         WinSetPointer( HWND_DESKTOP,
                        WinQuerySysPointer( HWND_DESKTOP, SPTR_WAIT, FALSE ));
      }
     return (MRESULT) 1;



   case WM_NACK_BITMAP_NOT_SUPPORTED:
      /*
       * Object window does not support this format of bitmap -
       * show a message box.
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      WinSendMsg( hwnd, WM_USER_ENABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
      bOK = WinLoadString( pmp->hab, (HMODULE)0,
                   ERRMSG_BITMAP_NOT_SUPPORTED, LEN_WORKSTRING, szWork );
      pmassert( pmp->hab, bOK );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUACRITICAL | MB_APPLMODAL);

      pmp->ulNextMode        = MODE_UNKNOWN;
      pmp->szNextFilename[0] = 0;
      WinPostMsg( hwnd, WM_USER_NEW_MODE, (MPARAM)0, (MPARAM)0 );
      return (MRESULT) 0;


   case WM_NACK_BITMAP_ERROR:
      /*
       * Object window had error loading the bitmap file -
       * show a message box
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      WinSendMsg( hwnd, WM_USER_ENABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           ERRMSG_BAD_BITMAP,
                           LEN_WORKSTRING,
                           szWork );
      pmassert( pmp->hab, bOK );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);

      pmp->ulNextMode        = MODE_UNKNOWN;
      pmp->szNextFilename[0] = 0;
      WinPostMsg( hwnd, WM_USER_NEW_MODE, (MPARAM)0, (MPARAM)0 );
      return (MRESULT) 0;


   case WM_NACK_FILE_READING_ERROR:
      /*
       * Object window had a problem with reading the disk -
       * show a message box.
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      WinSendMsg( hwnd, WM_USER_ENABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
      bOK = WinLoadString( pmp->hab, (HMODULE)NULLHANDLE,
                           ERRMSG_READ_ERROR, LEN_WORKSTRING, szWork );
      pmassert( pmp->hab, bOK );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUACRITICAL | MB_APPLMODAL);
      pmp->ulNextMode        = MODE_UNKNOWN;
      pmp->szNextFilename[0] = 0;
      WinPostMsg( hwnd, WM_USER_NEW_MODE, (MPARAM)0, (MPARAM)0 );
      return (MRESULT) 0;


   case WM_PAINT:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /* do not rely on client window rectangle being correct */
      WinQueryUpdateRect( hwnd, &rectl );
      WinQueryWindowRect( hwnd, &rectl );

      /* current mode of the program affects window painting */
      switch( pmp->ulMode )
      {
      case MODE_UNKNOWN:
         WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
         WinPostMsg( pmp->hwndObject, WM_USER_PAINT_DEFAULT_SCREEN,
                        (MPARAM)hwnd, (MPARAM)0 );
         /* must call default window proc for window validation */
         break;

      case MODE_TEXT:
         if( pmp->fBusy )
         {
            /*
             * Object window is busy with the client PS drawing into
             * retained segments. Use a cached micro PS to merely
             * fill the client window with a background wash.
             *
             * Proper painting of the text will occur in due time
             * because the WM_USER_ACK case below will
             * invalidate the client window and force a paint.
             * The object window won't be busy then.
             */
            hps = WinBeginPaint( hwnd, (HPS) 0, &rectl );
            pmassert( pmp->hab, hps );
            bOK = WinFillRect( hps, &rectl, SYSCLR_WINDOW );
            pmassert( pmp->hab, bOK );
            WinEndPaint( hps );
         }
         else
         {
            /* PS not busy. Use GpiDrawChain to repaint the text */
            hps = WinBeginPaint( hwnd, (HPS)pmp->hpsClient, &rectl );
            pmassert( pmp->hab, hps );
            bOK = WinFillRect( pmp->hpsClient, &rectl, SYSCLR_WINDOW );
            pmassert( pmp->hab, bOK );
            /*
             * GpiDrawChain re-plays the GpiCharString orders that were
             * retain in the WM_USER_PAGINATE case in prtobj.c
             */
            bOK = GpiDrawChain( pmp->hpsClient );
            pmassert( pmp->hab, bOK );
            bOK = WinEndPaint( pmp->hpsClient );
            pmassert( pmp->hab, bOK );
         }
         return (MRESULT) 0;

      case MODE_BITMAP:
         if( pmp->hbm )
         {
            WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
            WinPostMsg( pmp->hwndObject, WM_USER_PAINT_BITMAP,
                        (MPARAM)hwnd, (MPARAM)0 );
         }
         /* must call default window proc for window validation */
         break;

      case MODE_METAFILE:
         hps = WinBeginPaint( hwnd, (HPS) 0, &rectl );
         pmassert( pmp->hab, hps );
         WinFillRect( hps, &rectl, SYSCLR_WINDOW );
         if( pmp->hmf )
         {
            WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0, (MPARAM)0 );
            WinPostMsg( pmp->hwndObject, WM_USER_PAINT_METAFILE,
                        (MPARAM)hwnd, (MPARAM)0 );
         }
         WinEndPaint( hps );
         return (MRESULT) 0;

      default:
         pmassert( pmp->hab, NULL == "bad case in WM_PAINT" );
      }
      break;


   case WM_SIZE:
      /* Do size process if frame is not minimized  */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /*
       * If the object window is busy drawing, the GPI calls here can fail
       * because the PS is busy. Check for this situation and delay
       * the sizing operation by a few milliseconds.
       */
      if( pmp->fBusy )
      {
          WinStartTimer( pmp->hab, hwnd, ID_SIZE_TIMER, LEN_SIZE_TIMER );
          return (MRESULT) 0;
      }

      ulWork = WinQueryWindowULong( pmp->hwndFrame, QWL_STYLE );

      if( !( ulWork & WS_MINIMIZED ))
      {
         /* Frame is not minimized. Get window size in pels. */
         WinQueryWindowRect( hwnd, &rectl );

         /* how many twips will fit into the client window now? */
         bOK = GpiConvert( pmp->hpsClient, CVTC_DEVICE,
                           CVTC_WORLD, 2L, (PPOINTL)&rectl );
         pmassert( pmp->hab, bOK );

         /* compute client size in twips, store in pmp */
         pmp->sizelClient.cx = rectl.xRight - rectl.xLeft;
         pmp->sizelClient.cy = rectl.yTop   - rectl.yBottom;

         /* Try to keep the current position still in view by calculating */
         /* the difference between size required and client window. */
         /* The scroll position is then either 0 or the minimum of the */
         /* difference and the old scroll position */
         sNewPos = (LONG)FixedInchesToTwips(pmp->form.fxxWidth) +
                    (((float)(2 * OFFSET_XY_TWIPS)) / pmp->floatScale) -
                   pmp->sizelClient.cx;
         pmp->sHScrollPos = min( max( 0, sNewPos), pmp->sHScrollPos);
         sNewPos = (LONG)FixedInchesToTwips(pmp->form.fxyHeight) +
                    (((float)(2 * OFFSET_XY_TWIPS)) / pmp->floatScale) -
                   pmp->sizelClient.cy;
         pmp->sVScrollPos = min( max( 0, sNewPos), pmp->sVScrollPos);

         /* recalibrate the scroll bars */
         CalibrateHorizontalScrollBar( pmp );
         CalibrateVerticalScrollBar( pmp );

         /*
          * Modify def-view matrix translation to home the displayed page.
          * This depends on the current scaling value.
          */
         pmp->matlfDefView.lM31 = OFFSET_XY_TWIPS - (LONG)(pmp->floatScale *
                                                 (float)( pmp->sHScrollPos));
         pmp->matlfDefView.lM32 = OFFSET_XY_TWIPS + (LONG)(pmp->floatScale *
                          (float)( pmp->sVScrollPos - pmp->sVScrollRange ));
         bOK = GpiSetDefaultViewMatrix( pmp->hpsClient, 9,
                          &pmp->matlfDefView, TRANSFORM_REPLACE );
         pmassert( pmp->hab, bOK );

         /* force a paint */
         WinInvalidateRect( hwnd, NULL, FALSE );
      }
      return (MRESULT) 0;




   case WM_TIMER:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      switch( (ULONG) mp1 )
      {
      case ID_DEATH_TIMER:
         /* object window never posted a quit in allotted time. */
         WinPostMsg( hwnd, WM_QUIT, 0, 0 );
         break;
      case ID_SIZE_TIMER:
         /* object window was busy with the PS before; try sizing now */
         bOK = WinStopTimer( pmp->hab, hwnd, ID_SIZE_TIMER );
         pmassert( pmp->hab, bOK );
         WinSendMsg( hwnd, WM_SIZE, 0, 0 );
         break;
      }
      return (MRESULT) 0;





   case WM_USER_ACK:
      /*
       * Object window is done processing lengthy task.
       * mp1 contains the WM_USER msg originally posted to the object window
       * mp2 may contain a result code, depending on mp1
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /* reenable the client window */
      WinSendMsg( hwnd, WM_USER_ENABLE_CLIENT, (MPARAM)0, (MPARAM)0 );

      switch( (ULONG)mp1 )
      {
      case WM_USER_LOAD_BITMAP:
      case WM_USER_LOAD_METAFILE:
      case WM_USER_LOAD_TEXT:
         /*
          * Do size processing so that document will
          * "home" in the client window.
          */
         WinSendMsg( hwnd, WM_SIZE, (MPARAM)0, (MPARAM)0 );
         break;
      case WM_USER_PAGINATE:
         switch( (ULONG)mp2  )
         {
         case PAGINATE_EOF:
         case PAGINATE_EOF_PART_PAGE:
            /* seek top of file */
            fseek( pmp->f, 0, SEEK_SET );
         }
         WinInvalidateRect( hwnd, NULL, FALSE );
         break;
      }

      return (MRESULT) 0;


   case WM_USER_DISABLE_CLIENT:
      /*
       * usually disable before posting a task to the object window
       * this message may be sent; disable menu action bar as well
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      pmp->fBusy = TRUE;
      WinEnableWindow( pmp->hwndClient,  FALSE );
      WinEnableWindow( pmp->hwndMenubar, FALSE );
      WinEnableControl( pmp->hwndFrame, FID_HORZSCROLL,  FALSE );
      WinEnableControl( pmp->hwndFrame, FID_VERTSCROLL,  FALSE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_PAGEDOWN, FALSE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, FALSE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, FALSE );
      return (MRESULT)0;


   case WM_USER_ENABLE_CLIENT:
      /*
       * usually enable upon receipt of object window ack/nack
       * this message may be sent; enable menu actions if text mode
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      pmp->fBusy = FALSE;
      WinEnableWindow( pmp->hwndClient,  TRUE );
      WinEnableWindow( pmp->hwndMenubar, TRUE );
      WinEnableControl( pmp->hwndFrame, FID_HORZSCROLL,  TRUE );
      WinEnableControl( pmp->hwndFrame, FID_VERTSCROLL,  TRUE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, TRUE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, TRUE );
      if( pmp->ulMode == MODE_TEXT )
      {
         WinEnableMenuItem( pmp->hwndMenubar, IDM_PAGEDOWN, TRUE );
      }
      return (MRESULT) 0;



   case WM_USER_NEW_MODE:
      /*
       * The program now has a new file, file type, or printer, or
       * printer form, orientation, resolution, etc. The receipt
       * and processing of this message works to reset the program:
       * Old file, bitmap, or metafile handles are closed, and
       * new ones get opened. The titlebar shows the new filename.
       * This case works very much like a program reset.
       */
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /* close processing on current file */

      switch( pmp->ulMode )
      {
      case MODE_BITMAP:
        /* destroy previous memory dc, ps, and hbm */
        if( pmp->hpsMemory )
        {
          GpiAssociate( pmp->hpsMemory, (HDC)0 );
          GpiDestroyPS( pmp->hpsMemory );
          pmp->hpsMemory = (HPS) 0;
        }
        if( pmp->hdcMemory )
        {
          DevCloseDC( pmp->hdcMemory );
          pmp->hdcMemory = (HDC) 0;
        }
        if( pmp->hbm )
        {
          GpiDeleteBitmap( pmp->hbm );
          pmp->hbm = (HBITMAP) 0;
        }
        break;

      case MODE_METAFILE:
        /* destroy old metafile handle */
        if( pmp->hmf )
        {
          GpiDeleteMetaFile( pmp->hmf );
          pmp->hmf = (HMF) 0;
        }
        break;

      case MODE_TEXT:
        fclose( pmp->f );
        pmp->f = (FILE *) 0;
        /* turn off options for text mode */
        WinEnableMenuItem( pmp->hwndMenubar, IDM_PAGEDOWN, FALSE );
        WinEnableMenuItem( pmp->hwndMenubar, IDM_SETFONT,  FALSE );
        break;
      }

      /* turn off options for all modes */
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, FALSE );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, FALSE );
      WinEnableControl( pmp->hwndFrame, FID_HORZSCROLL,  FALSE );
      WinEnableControl( pmp->hwndFrame, FID_VERTSCROLL,  FALSE );

      /* copy over current values with the next values */
      pmp->ulMode = pmp->ulNextMode;
      strcpy( pmp->szFilename, pmp->szNextFilename );

      /* enable the print menu option if mode is known and there
         is a printer set up. */
      WinEnableMenuItem( pmp->hwndMenubar, IDM_PRINT,
       (pmp->ulMode != MODE_UNKNOWN && pmp->hpsPrinterInfo ) );


      /* update title bar text and show filename in use */
      if( *pmp->szFilename )
      {
         /* parse full-qualified filename to just get filename and extension */
         psz = strrchr( pmp->szFilename, '\\' );
         if (psz && *psz)
         {
             ++psz;
         }
         else
         {
             psz = pmp->szFilename;
         }
         sprintf( szWork, "%s - %s", pmp->pszTitle, psz );
      }
      else
      {
         strcpy( szWork, pmp->pszTitle );
      }
      WinSetWindowText( pmp->hwndTitlebar, szWork );

      /* enable options for all modes */
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, TRUE  );
      WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, TRUE  );
      WinEnableControl( pmp->hwndFrame, FID_HORZSCROLL,  TRUE  );
      WinEnableControl( pmp->hwndFrame, FID_VERTSCROLL,  TRUE  );

      /* process this new mode */
      switch( pmp->ulMode )
      {
      case MODE_BITMAP:
        /* load the bitmap into memory so it is compatible with the screen */
        WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, 0, 0 );
        WinPostMsg( pmp->hwndObject, WM_USER_LOAD_BITMAP,
                    (MPARAM)hwnd, (MPARAM)FLAGS_SCREEN );
        break;

      case MODE_METAFILE:
        /* make object window read metafile */
        WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, 0, 0 );
        WinPostMsg( pmp->hwndObject, WM_USER_LOAD_METAFILE, (MPARAM)hwnd, 0 );
        break;

      case MODE_TEXT:
        /* turn on options for text mode */
        WinEnableMenuItem( pmp->hwndMenubar, IDM_PAGEDOWN, TRUE  );
        WinEnableMenuItem( pmp->hwndMenubar, IDM_SETFONT,  TRUE  );
        /* reset view matrix that was last in effect for viewing text pages; */
        /* this gets lost after viewing a metafile */
        bOK = GpiSetDefaultViewMatrix( pmp->hpsClient, 9,
                                       &pmp->matlfDefView, TRANSFORM_REPLACE );
        pmassert( pmp->hab, bOK );
        /* disable until text loaded */
        WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, 0, 0 );
        /* tell object window to load the text file */
        bOK = WinPostMsg( pmp->hwndObject, WM_USER_LOAD_TEXT, (MPARAM)hwnd, 0 );
        pmassert( pmp->hab, bOK );
        break;


      case MODE_UNKNOWN:
        /* size screen to get correct scrollbars */
        WinPostMsg( pmp->hwndClient, WM_SIZE, 0, 0 );
        break;

      }
      return (MRESULT) 0;

   case WM_VSCROLL:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );

      /* Compute some fraction of scroll bar range for a unit of scrolling. */
      sStep  = pmp->sVScrollRange / 50;

      switch( SHORT2FROMMP( mp2 ))
      {
      case SB_LINEUP:
         pmp->sVScrollPos -= sStep;
         break;
      case SB_PAGEUP:
         pmp->sVScrollPos -= pmp->sizelClient.cy;
         break;
      case SB_LINEDOWN:
         pmp->sVScrollPos += sStep;
         break;
      case SB_PAGEDOWN:
         pmp->sVScrollPos += pmp->sizelClient.cy;
         break;
      case SB_SLIDERPOSITION:
      case SB_SLIDERTRACK:
         sNewPos          = SHORT1FROMMP( mp2 );
         pmp->sVScrollPos = sNewPos;
         break;
      }

      /* Don't allow step assignments to exceed limits of zero to range. */
      pmp->sVScrollPos = max( (SHORT)0,
                              min( pmp->sVScrollPos, pmp->sVScrollRange ));

      if( pmp->sVScrollPos != SHORT1FROMMR(WinSendMsg( pmp->hwndVScroll,
          SBM_QUERYPOS, (MPARAM)0, (MPARAM)0 )))
      {
         /*
          * New scroll bar thumbbutton position is different than current.
          * Set a new Y translation value to effect the scroll.
          * Current scale setting affects the Y element of the matrix.
          */
         pmp->matlfDefView.lM32 = OFFSET_XY_TWIPS + (LONG)(pmp->floatScale *
                          (float)( pmp->sVScrollPos - pmp->sVScrollRange ));

         pmp->matlfDefView.lM32 =  (LONG)(pmp->floatScale *
             (float)( OFFSET_XY_TWIPS +
                      pmp->sVScrollPos -
                      pmp->sVScrollRange ));

         bOK = GpiSetDefaultViewMatrix( pmp->hpsClient,
                                        9,
                                        &pmp->matlfDefView,
                                        TRANSFORM_REPLACE );
         pmassert( pmp->hab, bOK );
         CalibrateVerticalScrollBar( pmp );
         WinInvalidateRect( hwnd, NULL, FALSE );
      }
      return (MRESULT) 0;
   }
   return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}  /* End of WinProc */


 /************************************************************************
 *
 * Name        : CalibrateHorizontalScrollBar( pmp )
 *
 * Description : This function computes the HORZINTAL scroll bar range to be
 *               that portion of the text document that is clipped from the
 *               client window, then sends this range to the scroll bar.
 *               The thumb position used is that which is already in the pmp.
 *
 *               Thumbsize is then computed by the scroll bar to be a ratio
 *               of the visible part of the document / the entire document.
 *
 * Concepts    : Manipulation of thum position and scroll bar.
 *
 * API's       : WinSendMsg
 *
 * Parameters  : PMAIN_PARM      structure containing important data such
 *                                as window handles, device context handles,
 *                                and other bitmap, printing and program
 *                                information
 *
 * Returns     : [nothing]
 *
 **************************************************************************/
VOID CalibrateHorizontalScrollBar( PMAIN_PARM pmp )
{
   BOOL bOK;
   SHORT sFormWidth;
   SHORT sBorder;

   /*
    * The range of the scroll bar is the length of the form plus border
    * that is clipped by the client window. The zoom facter is taken into
    * account so that the border is a constant width
    */
   sFormWidth = (SHORT)FixedInchesToTwips(pmp->form.fxxWidth);
   sBorder = (SHORT)(((float)(2 * OFFSET_XY_TWIPS)) / pmp->floatScale);
   pmp->sHScrollRange = sFormWidth - pmp->sizelClient.cx + sBorder;

   /* Position the thumbbutton according to pmp->sHScrollPos. */
   bOK = (BOOL)WinSendMsg( pmp->hwndHScroll,
                     SBM_SETSCROLLBAR,
                     (MPARAM) pmp->sHScrollPos,
                     MPFROM2SHORT( (USHORT)0,  pmp->sHScrollRange ));
   pmassert( pmp->hab, bOK );

   /* Set thumbsize of the scroll bar. */

   bOK = (BOOL)WinSendMsg( pmp->hwndHScroll,
                     SBM_SETTHUMBSIZE,
                     MPFROM2SHORT( pmp->sizelClient.cx, sFormWidth + sBorder),
                     (MPARAM)0 );

   pmassert( pmp->hab, bOK );

}  /* End of CalibrateHorizontalScrollBar */


/************************************************************************
 *
 * Name        : CalibrateVerticalScrollBar( pmp )
 *
 * Description : This function computes the VERTICAL scroll bar range to be
 *               that portion of the text document that is clipped from the
 *               client window, then sends this range to the scroll bar.
 *               The thumb position used is that which is already in the pmp.
 *
 *               Thumbsize is then computed by the scroll bar to be a ratio
 *               of the visible part of the document / the entire document.
 *
 * Concepts    : Manipulation of thum position and scroll bar.
 *
 * API's       : WinSendMsg
 *
 * Parameters  : PMAIN_PARM      structure containing important data such
 *                                as window handles, device context handles,
 *                                and other bitmap, printing and program
 *                                information
 *
 * Returns     : [nothing]
 *
 **************************************************************************/
VOID CalibrateVerticalScrollBar( PMAIN_PARM pmp )
{
   BOOL bOK;
   SHORT sFormHeight;
   SHORT sBorder;

   /*
    * The range of the scroll bar is the length of the form plus border
    * that is clipped by the client window. The zoom facter is taken into
    * account so that the border is a constant width
    */
   sFormHeight = (SHORT)FixedInchesToTwips(pmp->form.fxyHeight);
   sBorder = (SHORT)(((float)(2 * OFFSET_XY_TWIPS)) / pmp->floatScale);
   pmp->sVScrollRange = sFormHeight - pmp->sizelClient.cy + sBorder;

   /* Position the thumbbutton according to pmp->sVScrollPos. */
   bOK = (BOOL)WinSendMsg( pmp->hwndVScroll,
                     SBM_SETSCROLLBAR,
                     (MPARAM) pmp->sVScrollPos,
                     MPFROM2SHORT( (USHORT)0,  pmp->sVScrollRange ));
   pmassert( pmp->hab, bOK );

   /* Set thumbsize of the scroll bar. */

   bOK = (BOOL)WinSendMsg( pmp->hwndVScroll,
                     SBM_SETTHUMBSIZE,
                     MPFROM2SHORT( pmp->sizelClient.cy, sFormHeight + sBorder),
                     (MPARAM)0 );

   pmassert( pmp->hab, bOK );

}  /* End of CalibrateVerticalScrollBar */

/***************************  End of prtsamp.c ****************************/

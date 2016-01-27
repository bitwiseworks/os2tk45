/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*| PROGRAM NAME: PALETTE                                                    |*/
/*| -------------                                                            |*/
/*|  A PM program demonstrating vector fonts, palette manager APIs, and IPF  |*/
/*|  help.                                                                   |*/
/*|                                                                          |*/
/*| COPYRIGHT:                                                               |*/
/*| ----------                                                               |*/
/*|  Copyright (C) International Business Machines Corp., 1992, 1996         |*/
/*|                                                                          |*/
/*| DISCLAIMER OF WARRANTIES:                                                |*/
/*| -------------------------                                                |*/
/*|  The following [enclosed] code is sample code created by IBM Corporation.|*/
/*|  This sample code is not part of any standard IBM product and is provided|*/
/*|  to you solely for the purpose of assisting you in the development of    |*/
/*|  your applications.  The code is provided "AS IS", without warranty of   |*/
/*|  any kind.  IBM shall not be liable for any damages arising out of your  |*/
/*|  use of the sample code, even if they have been advised of the           |*/
/*|  possibility of such damages.                                            |*/
/*|                                                                          |*/
/*| REVISION LEVEL: 1.0                                                      |*/
/*| ---------------                                                          |*/
/*|                                                                          |*/
/*| WHAT THIS PROGRAM DOES:                                                  |*/
/*| -----------------------                                                  |*/
/*|  This program displays a standard window and then draws the IBM logo     |*/
/*|  in the window.  The logo is done using a vector font.  Then a palette   |*/
/*|  is created and rotated in another thread.                               |*/
/*|                                                                          |*/
/*| WHAT THIS PROGRAM DEMONSTRATES:                                          |*/
/*| -------------------------------                                          |*/
/*|  This program demonstrates how to create and display a standard window,  |*/
/*|  use switched menus, use a second thread for drawing, modify a palette   |*/
/*|  using some basic GPI calls, and display vectored fonts.                 |*/
/*|                                                                          |*/
/*| APIs CALLED:                                                             |*/
/*| ------------                                                             |*/
/*|    WinInitialize                                                         |*/
/*|    WinCreateMsgQueue                                                     |*/
/*|    WinRegisterClass                                                      |*/
/*|    WinCreateHelpInstance                                                 |*/
/*|    WinDestroyHelpInstance                                                |*/
/*|    WinCreateStdWindow                                                    |*/
/*|    WinGetLastError                                                       |*/
/*|    WinShowWindow                                                         |*/
/*|    WinAssociateHelpInstance                                              |*/
/*|    WinGetMsg                                                             |*/
/*|    WinDispatchMsg                                                        |*/
/*|    WinReleasePS                                                          |*/
/*|    WinDestroyHelpInstance                                                |*/
/*|    WinDestroyWindow                                                      |*/
/*|    WinDestroyMsgQueue                                                    |*/
/*|    WinTerminate                                                          |*/
/*|    WinRequestMutexSem                                                    |*/
/*|    WinRealizePalette                                                     |*/
/*|    DosReleaseMutexSem                                                    |*/
/*|    DosCreateMutexSem                                                     |*/
/*|    WinOpenWindowDC                                                       |*/
/*|    GpiCreatePS                                                           |*/
/*|    DevQueryCaps                                                          |*/
/*|    DosCreateThread                                                       |*/
/*|    WinQueryWindowRect                                                    |*/
/*|    WinPostMsg                                                            |*/
/*|    WinWindowFromID                                                       |*/
/*|    WinSetMenuItemText                                                    |*/
/*|    WinInvalidateRect                                                     |*/
/*|    WinDlgBox                                                             |*/
/*|    WinSendMsg                                                            |*/
/*|    WinBeginPaint                                                         |*/
/*|    WinFillRect                                                           |*/
/*|    GpiQueryFonts                                                         |*/
/*|    GpiCreateLogFont                                                      |*/
/*|    GpiSetCharSet                                                         |*/
/*|    GpiSetCharBox                                                         |*/
/*|    GpiQueryTextBox                                                       |*/
/*|    GpiBeginPath                                                          |*/
/*|    GpiMove                                                               |*/
/*|    GpiCharString                                                         |*/
/*|    GpiEndPath                                                            |*/
/*|    GpiSetClipPath                                                        |*/
/*|    WinEndPaint                                                           |*/
/*|    WinDefWindowProc                                                      |*/
/*|    WinDismissDlg                                                         |*/
/*|    WinDefDlgProc                                                         |*/
/*|    GpiCreatePalette                                                      |*/
/*|    GpiSelectPalette                                                      |*/
/*|    GpiSetColor                                                           |*/
/*|    GpiLine                                                               |*/
/*|    DosSleep                                                              |*/
/*|    GpiAnimatePalette                                                     |*/
/*|    GpiDeletePalette                                                      |*/
/*|    DosExit                                                               |*/
/*|    WinAlarm                                                              |*/
/*|    WinMessageBox                                                         |*/
/*|                                                                          |*/
/*|                                                                          |*/
/*| WHAT YOU NEED TO COMPILE THIS PROGRAM:                                   |*/
/*| --------------------------------------                                   |*/
/*|                                                                          |*/
/*|  REQUIRED FILES:                                                         |*/
/*|  ---------------                                                         |*/
/*|                                                                          |*/
/*|    PALETTE.C      - Source code                                          |*/
/*|    PALETTE.MAK    - Make file for this program                           |*/
/*|    PALETTE.DEP    - Dependency file for this program                     |*/
/*|    PALETTE.DEF    - Module definition file                               |*/
/*|    PALETTE.H      - Application header file                              |*/
/*|    PALETTE.ICO    - Icon file                                            |*/
/*|    PALETTE.RC     - Resource file                                        |*/
/*|    PALETTE.IPF    - Documentation file for this program                  |*/
/*|    PALETTE.DLG    - Dialog file                                          |*/
/*|                                                                          |*/
/*|    OS2.H          - Presentation Manager include file                    |*/
/*|    STDLIB.H       - Standard library function declarations               |*/
/*|    ...                                                                   |*/
/*|                                                                          |*/
/*|  REQUIRED LIBRARIES:                                                     |*/
/*|  -------------------                                                     |*/
/*|                                                                          |*/
/*|    OS2386.LIB     - OS/2 API Library                                     |*/
/*|                                                                          |*/
/*|  REQUIRED PROGRAMS:                                                      |*/
/*|  ------------------                                                      |*/
/*|                                                                          |*/
/*|    IBM C Set/2 Compiler (icc.exe    )                                    |*/
/*|    IBM Linker           (ilink.exe  )                                    |*/
/*|    Resource Compiler    (rc.exe     )                                    |*/
/*|    Help Compiler        (ipfc.exe   )                                    |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/

/*+--------------------------------------------------------------------------+*/
/*| System and library header files.                                         |*/
/*+--------------------------------------------------------------------------+*/

#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WINHELP
#define INCL_WINFRAMEMGR
#define INCL_WINERRORS
#define INCL_WINPALETTE
#define INCL_GPICONTROL
#define INCL_WINMENUS
#define INCL_WINDIALOGS
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIPATHS
#define INCL_GPILOGCOLORTABLE
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DEV
#include <os2.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "palette.h"

/*
 * Globals
 */

HAB     hab;
HPAL    hPal = NULLHANDLE;
HRGN    hrgn = NULLHANDLE;
HDC     hdc =  NULLHANDLE;
HMQ     hmqPalette;
HWND    hwndPalette;
HWND    hwndPaletteFrame;
CHAR    szClassName[]   = "Palette";
CHAR    szMessage[]     = "Palette Manager Sample";


/*
 * Status Flags For the options menu
 */

#define SLOW_VALUE    100LU
#define MEDIUM_VALUE  10LU                  /* Set speed constants            */
#define FAST_VALUE     0LU

BOOL  speedSlow      = FALSE;
BOOL  speedMedium    = TRUE;                /* Initial Speed                  */
BOOL  speedFast      = FALSE;
ULONG animationSpeed = MEDIUM_VALUE;

BOOL  logoIBM        = FALSE;               /* Initial Text                   */
BOOL  logoOS2        = TRUE;

/*
 * Palette Drawing Thread Globals
 */

static LONG    lMessageCount = 0L;
static BOOL    fPaletteCaps = TRUE;    /* Palette Manager capabilities?  */
static SHORT   cxClient, cyClient;     /* client window X and Y size     */
static USHORT  threadRun = FALSE;      /* non-zero means draw lines      */
static HMTX    hmtxPS    = NULLHANDLE; /* PS access semaphore            */
static HPS     hpsBuffer = NULLHANDLE; /* Pres. space for memory buffer  */

#define PAL_COUNT 128L       /* For the purposes of this example we will be   */
ULONG alTable[PAL_COUNT];    /*        using 128 palette entries.             */

#define MSG_BOX_ID     256                  /* error message box id           */


/*
 * Required IPF Structure Declarations
 */

HELPINIT hmiHelpData;                       /* Help initialization structure  */
HWND     hwndHelpInstance;                  /* Handle to Help window          */


/*
 * Main routine...initializes window and message queue
 */
int main( )
{
   QMSG qmsg;
   ULONG ctldata;

   /*
    * Create your hab, hmq, and register a class.
    */

   hab = WinInitialize(0LU);

   hmqPalette = WinCreateMsgQueue(hab, 0L);

   WinRegisterClass( hab,
                     (PSZ)szClassName,
                     (PFNWP)PaletteWndProc,
                     CS_SIZEREDRAW,
                     0LU);

   /*+-----------------------------------------+*/
   /*| IPF Initialization Structure            |*/
   /*+-----------------------------------------+*/

   hmiHelpData.cb                       = sizeof(HELPINIT);
   hmiHelpData.ulReturnCode             = 0LU;
   hmiHelpData.pszTutorialName          = (PSZ)NULL;
   hmiHelpData.phtHelpTable  = (PHELPTABLE)MAKELONG(MAIN_HELPTABLE, 0xFFFF);
   hmiHelpData.hmodAccelActionBarModule = NULLHANDLE;
   hmiHelpData.idAccelTable             = 0;
   hmiHelpData.idActionBar              = 0;
   hmiHelpData.pszHelpWindowTitle       = "Palette Help";
   hmiHelpData.hmodHelpTableModule      = NULLHANDLE;
   hmiHelpData.fShowPanelId             = CMIC_HIDE_PANEL_ID;
   hmiHelpData.pszHelpLibraryName       = "PALETTE.HLP";


   /*+------------------------------------------+*/
   /*| Create Instance of IPF                   |*/
   /*|                                          |*/
   /*| pass Anchor Block handle and address of  |*/
   /*| IPF initialization structure, and check  |*/
   /*| that creation was successful.            |*/
   /*+------------------------------------------+*/

   if((hwndHelpInstance = WinCreateHelpInstance(hab, &hmiHelpData))==NULLHANDLE)
      mprintf("Help Creation Error");

   else if (hmiHelpData.ulReturnCode)
   {
      mprintf("Help ended because of error %ld", hmiHelpData.ulReturnCode);
      WinDestroyHelpInstance(hwndHelpInstance);
   }


   /*
    * Create a standard pm window and check
    * that creation was successful.
    */
   ctldata = FCF_STANDARD;

   hwndPaletteFrame = WinCreateStdWindow( HWND_DESKTOP,
                                          WS_VISIBLE,
                                          &ctldata,
                                          szClassName,
                                          szMessage,
                                          0LU,
                                          NULLHANDLE,
                                          ID_WINDOW,
                                          &hwndPalette );
   if (hwndPaletteFrame == NULLHANDLE)
   {
      mprintf("WinCreateStdWindow error: 0x%04x", WinGetLastError(hab));
      return 0;
   }
   WinShowWindow( hwndPaletteFrame, TRUE );


   /*+-----------------------------------------+*/
   /*| Associate Instance of IPF               |*/
   /*|                                         |*/
   /*| pass handle to Help Window and handle   |*/
   /*| to frame window with which Help is      |*/
   /*| to be associated.                       |*/
   /*+-----------------------------------------+*/

   if (hwndHelpInstance)
      WinAssociateHelpInstance(hwndHelpInstance, hwndPaletteFrame);


   /*
    * Poll messages from event queue
    */
   while( WinGetMsg( hab, (PQMSG)&qmsg, (HWND)NULLHANDLE, 0LU, 0LU ) )
      WinDispatchMsg( hab, (PQMSG)&qmsg );


   /*
    * Clean up
    */

   if(hpsBuffer)
      WinReleasePS( hpsBuffer );
   if(hwndHelpInstance)
      WinDestroyHelpInstance( hwndHelpInstance );
   if(hwndPaletteFrame)
      WinDestroyWindow( hwndPaletteFrame );
   if(hmqPalette)
      WinDestroyMsgQueue( hmqPalette );
   WinTerminate( hab );
   return 0;
} /*  end of main()  */


/*
 * This routine processes WM_COMMAND, WM_PAINT.  It passes
 * everything else to the Default Window Procedure.
 */
MRESULT PaletteWndProc(
   HWND   hwnd,
   ULONG  msg,
   MPARAM mp1,
   MPARAM mp2 )
{
   static  RECTL  rctlClient, rctlSize;       /* client rectl's               */
   TID     tidDrawing;                        /* drawing thread identifier    */
   ULONG   cclr;
   HPS     hps;
   HWND    hwndMenu;                          /* Handle for the Menu bar      */

   switch (msg)
   {

      /*+-----------------------------------------------------------------+*/
      /*| In this example, the WinRealizePalette call is issued in        |*/
      /*| response to a WM_REALIZEPALETTE. This ensures that the palette  |*/
      /*| is appropriately realized for all drawing operations.           |*/
      /*+-----------------------------------------------------------------+*/

   case WM_REALIZEPALETTE:
         WinRequestMutexSem( hmtxPS, SEM_INDEFINITE_WAIT );
         WinRealizePalette(hwnd, hpsBuffer, &cclr);
         DosReleaseMutexSem( hmtxPS );
      break;


   case WM_CREATE:
   {
      SIZEL sizl = { 0L, 0L };
      LONG  alCaps[CAPS_PHYS_COLORS];
      LONG lCount = CAPS_PHYS_COLORS;
      LONG lStart = CAPS_FAMILY;

      /*+-----------------------------------------------------------------+*/
      /*| The client window is being created.  Create the semaphore to    |*/
      /*| control access to the presentation space.  Then create the      |*/
      /*| thread that will draw the lines.                                |*/
      /*+-----------------------------------------------------------------+*/

      DosCreateMutexSem( (PSZ)NULL, &hmtxPS, 0UL, FALSE );

      hdc = WinOpenWindowDC(hwnd);

      /*+-----------------------------------------------------------------+*/
      /*| Create a non-cached presentation space.  We will not release    |*/
      /*| this PS, as we will be selecting a Palette to this PS and then  |*/
      /*| animating the palette.  Upon releasing a PS the palette is no   |*/
      /*| longer selected for obvious reasons.                            |*/
      /*+-----------------------------------------------------------------+*/

      hpsBuffer = GpiCreatePS( hab,
                               hdc,
                               &sizl,
                               PU_PELS | GPIF_DEFAULT |
                               GPIT_MICRO | GPIA_ASSOC );

      DevQueryCaps( hdc, lStart, lCount, alCaps );
      fPaletteCaps = alCaps[CAPS_ADDITIONAL_GRAPHICS] & CAPS_PALETTE_MANAGER;

      hwndPalette = hwnd;
      if ( PaletteInit() )
         break;

      threadRun = TRUE;

      DosCreateThread( &tidDrawing,
                       (PFNTHREAD) DrawingThread,
                       0UL,
                       0UL,
                       8192UL );

      break;
   }

   case WM_SIZE:
      /*+-----------------------------------------------------------------+*/
      /*| Window is being resized so get new client window size and       |*/
      /*| recompute the end points for the lines.                         |*/
      /*+-----------------------------------------------------------------+*/

      WinQueryWindowRect(hwnd,&rctlSize);

      cxClient = SHORT1FROMMP( mp2 );       /* Get new client window size     */
      cyClient = SHORT2FROMMP( mp2 );
      break;

   case WM_SHOW:
      if ( (lMessageCount == 0L) && !fPaletteCaps )
      {
         lMessageCount +=
         mprintf("Palette animation is not supported on this display.");
      }
      break;

   case WM_INITMENU:
      switch (LOUSHORT(mp1))
      {
         case IDM_OPTIONS:
            hwndMenu = WinWindowFromID( hwndPaletteFrame, /* Get Menu Handle  */
                                        FID_MENU );
            WinEnableMenuItem(hwndMenu, IDM_SPEED,     fPaletteCaps);
            WinEnableMenuItem(hwndMenu, IDM_STARTSTOP, fPaletteCaps);
            break;

         case IDM_SPEED:
            SetPulldownState(hwndPaletteFrame, IDM_SPEEDSLOW  , speedSlow);
            SetPulldownState(hwndPaletteFrame, IDM_SPEEDMEDIUM, speedMedium);
            SetPulldownState(hwndPaletteFrame, IDM_SPEEDFAST  , speedFast);
            break;

         case IDM_LOGO:
            SetPulldownState(hwndPaletteFrame, IDM_LOGOIBM    , logoIBM);
            SetPulldownState(hwndPaletteFrame, IDM_LOGOOS2    , logoOS2);
            break;
      }
      break;


   case WM_COMMAND:
      switch (LOUSHORT(mp1))
      {
         case IDM_EXIT:
            WinPostMsg(hwnd, WM_CLOSE, MPVOID, MPVOID);
            break;

         case IDM_STARTSTOP:
            hwndMenu = WinWindowFromID( hwndPaletteFrame, /* Get Menu Handle  */
                                        FID_MENU );

            if ( speedSlow || speedMedium || speedFast  ) /* If running->stop */
            {
               speedSlow   = FALSE;
               speedMedium = FALSE;                       /* Set no speed     */
               speedFast   = FALSE;

               WinSetMenuItemText( hwndMenu,              /* Change Menu Text */
                                   IDM_STARTSTOP,         /* ID# for old text */
                                   "Start" );             /* 'Change To' text */
            }
            else                                     /* If stopped then start */
            {
               if ( animationSpeed == SLOW_VALUE )
                    speedSlow   = TRUE;
               if ( animationSpeed == MEDIUM_VALUE )      /* Reset proper flag */
                    speedMedium = TRUE;
               if ( animationSpeed == FAST_VALUE )
                    speedFast   = TRUE;

               WinSetMenuItemText( hwndMenu,              /* Change Menu Text */
                                   IDM_STARTSTOP,         /* ID# for old text */
                                   "Stop" );              /* 'Change To' text */
            }
            break;


         case IDM_SPEEDSLOW:
            speedSlow   = TRUE;
            speedMedium = FALSE;
            speedFast   = FALSE;
            animationSpeed = SLOW_VALUE;
            break;

         case IDM_SPEEDMEDIUM:
            speedSlow   = FALSE;
            speedMedium = TRUE;
            speedFast   = FALSE;
            animationSpeed = MEDIUM_VALUE;
            break;

         case IDM_SPEEDFAST:
            speedSlow   = FALSE;
            speedMedium = FALSE;
            speedFast   = TRUE;
            animationSpeed = FAST_VALUE;
            break;

         case IDM_LOGOIBM:
            logoIBM = TRUE;
            logoOS2 = FALSE;
            WinInvalidateRect(hwnd, &rctlSize, FALSE);
            break;

         case IDM_LOGOOS2:
            logoIBM = FALSE;
            logoOS2 = TRUE;
            WinInvalidateRect(hwnd, &rctlSize, FALSE);
            break;

         case IDM_HELPABOUT:               /* DISPLAY HELP ABOUT DIALOG       */
            WinDlgBox(HWND_DESKTOP,        /* MAKE THE DESKTOP THE PARENT     */
                      hwnd,                /* OWNED BY THE CLIENT WINDOW      */
                      (PFNWP) AboutDlgProc,/* ADDRESS OF THE DIALOG PROCEDURE */
                      NULLHANDLE,          /* MODULE HANDLE                   */
                      IDD_PRODUCTINFO,     /* DIALOG IDENTIFIER IN RESOURCE   */
                      (PVOID)NULL);        /* INITIALIZATION DATA             */

            WinInvalidateRect(hwnd, &rctlSize, FALSE);      /* Refresh Window */
            break;

         case IDM_HELPFORHELP:
            if (hwndHelpInstance)
            {
               WinSendMsg( hwndHelpInstance, HM_DISPLAY_HELP, MPVOID, MPVOID);
            }
           break;

         case IDM_HELP_INDEX:
            if (hwndHelpInstance)
            {
               WinSendMsg(hwndHelpInstance, HM_HELP_CONTENTS, MPVOID, MPVOID);
            }
            break;

         case IDM_EXTENDED_HELP:
            if (hwndHelpInstance)
            {
               WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP,
               MPFROMSHORT(IDM_EXTENDED_HELP), HM_RESOURCEID);
            }
            break;

         case IDM_KEYS_HELP:
            if (hwndHelpInstance)
            {
               WinSendMsg( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROM2SHORT(IDM_KEYS_HELP, 0),
                           MPFROMSHORT(HM_RESOURCEID));
            }
            break;

         default:
            break;

      } /* end of switch within case WM_COMMAND */
      break;

   case WM_PAINT:

      /*+------------------------------------------------------+*/
      /*| Since there will be two threads painting on the same |*/
      /*| frame we will use a semaphore to resolve contention. |*/
      /*+------------------------------------------------------+*/

      WinRequestMutexSem( hmtxPS, SEM_INDEFINITE_WAIT );

      /*
       * Get a cached micro presentation space.
       */
      hps = WinBeginPaint( hwnd, NULLHANDLE, &rctlClient );

      /*
       * Fill the rectangle with the backgound color.
       */
      WinFillRect( hps, &rctlClient, CLR_BLACK );

      /*+-------------------------------------------------+*/
      /*| The following code segment loads a vector font  |*/
      /*| scales it to fit in the window and draws it     |*/
      /*| vertically and horizontally centered.           |*/
      /*+-------------------------------------------------+*/
      {
         POINTL       ptl;
         SIZEF        sizfx;
         FATTRS       fattrs;
         LONG         cFonts, lTemp = 0L;
         PFONTMETRICS pfm;
         LONG         lcid = 1L;
         CHAR         fontName[25],outText[25];
         USHORT       codePage;
         ULONG        min = (cxClient<cyClient) ? cxClient : cyClient;

         min /= 3;

         if(logoIBM)
         {
            strcpy(fontName,"Symbol Set");
            codePage = 65400;
            strcpy(outText,"Ž");
         }
         else
         {
            /*+-------------------------------------------+*/
            /*| Define "Helvetica Bold" vector font,      |*/
            /*| "Courier Bold" and "Times New Roman Bold" |*/
            /*| are examples of other vector fonts.       |*/
            /*+-------------------------------------------+*/
            strcpy(fontName,"Helvetica Bold");
            codePage = 850;
            strcpy(outText,"OS/2");
         }

         cFonts = GpiQueryFonts(hpsBuffer, QF_PUBLIC, fontName, &lTemp,
             (LONG) sizeof(FONTMETRICS), (PFONTMETRICS)NULL);

         if(cFonts > 0L)
         {
            pfm = malloc(sizeof(FONTMETRICS) * cFonts);
            GpiQueryFonts(hpsBuffer, QF_PUBLIC, fontName, &cFonts,
                          (LONG) sizeof(FONTMETRICS), pfm);
            fattrs.lMatch          = pfm[0].lMatch;
            strcpy(fattrs.szFacename, pfm[0].szFacename);
         }
         else
         {
            fattrs.lMatch = 0L;
            strcpy(fattrs.szFacename, "");
         }

         fattrs.usRecordLength  = sizeof(FATTRS);
         fattrs.fsSelection     = FATTR_SEL_OUTLINE;
         fattrs.idRegistry      = 0;
         fattrs.usCodePage      = codePage;
         fattrs.lMaxBaselineExt = 0L;
         fattrs.lAveCharWidth   = 0L;
         fattrs.fsType          = 0;
         fattrs.fsFontUse       = FATTR_FONTUSE_OUTLINE;

         GpiCreateLogFont(hpsBuffer, (PSTR8)NULL, lcid, &fattrs);
         GpiSetCharSet(hpsBuffer, lcid);

         sizfx.cx = MAKEFIXED(min, 0);
         sizfx.cy = MAKEFIXED(min, 0);
         GpiSetCharBox(hpsBuffer, &sizfx);

         /*+------------------------------------------------+*/
         /*| CALCULATE THE POINT TO START THE CENTERED TEXT |*/
         /*+------------------------------------------------+*/
         {
            POINTL aptl[TXTBOX_COUNT];

            GpiQueryTextBox(hpsBuffer,
                            strlen(outText),
                            outText,
                            TXTBOX_COUNT,      /* return maximum information  */
                            aptl);             /* array of coordinates points */
                                               /* in world coordinates.       */


            ptl.x = (cxClient - aptl[TXTBOX_TOPRIGHT].x) / 2;
            ptl.y = (cyClient - aptl[TXTBOX_TOPRIGHT].y) / 2;

         }


         /*+-------------------------------------------+*/
         /*| Now that we have our vector font and      |*/
         /*| location to place on the screen, let's    |*/
         /*| create a Path using the outline font.     |*/
         /*+-------------------------------------------+*/

         GpiBeginPath(hpsBuffer,1L);

         GpiMove(hpsBuffer,&ptl);
         GpiCharString(hpsBuffer, strlen(outText), outText);

         GpiEndPath(hpsBuffer);

         /*+-----------------------------------------+*/
         /*| Next let's erase the current clip path  |*/
         /*| and assign the above outline font path  |*/
         /*| as a clip path to the current hps.      |*/
         /*+-----------------------------------------+*/

         GpiSetClipPath(hpsBuffer,
                0L,
                FPATH_ALTERNATE);

         GpiSetClipPath(hpsBuffer,
                1L,
                FPATH_ALTERNATE | SCP_AND);


         /*+-------------------------------------------+*/
         /*| Lastly draw a series of lines which will  |*/
         /*| be clipped to attached clip path.         |*/
         /*| *Draw on the Char Box                     |*/
         /*+-------------------------------------------+*/

         DrawPaletteLines(rctlSize,
                          PAL_COUNT,
                          hpsBuffer);

         if(pfm)
            free(pfm);
      }


      /*
       * Release the cached micro presentation space.
       */
      WinEndPaint( hps );

      /*+------------------------------------------------------+*/
      /*| Release the semaphore, thereby allowing the other    |*/
      /*| thread to paint.                                     |*/
      /*+------------------------------------------------------+*/

      DosReleaseMutexSem( hmtxPS );
      break;

   default:
      return WinDefWindowProc(hwnd, msg, mp1, mp2);

   }
   return MRFROMLONG(0L);
}   /*  end of PaletteWndProc()  */



/*+--------------------------------------------------------------------------+*/
/*| AboutDlgProc - Help About Dialog Procedure                               |*/
/*+--------------------------------------------------------------------------+*/
MRESULT AboutDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch( msg )
   {
   case WM_COMMAND:          /* DISMISS DIALOG */
      WinDismissDlg( hwndDlg, TRUE );
      break;

   default:
      return (WinDefDlgProc( hwndDlg, msg, mp1, mp2 ));
   }
   return MRFROMLONG(0L);
}  /*  end of AboutDlgProc()  */



/*+--------------------------------------------------------------------------+*/
/*| PaletteInit - Initializes the palette in the client window.              |*/
/*+--------------------------------------------------------------------------+*/
LONG PaletteInit()
{
   LONG    i;
   LONG    ret = 0L;
   ULONG   cclr;

   WinRequestMutexSem( hmtxPS, SEM_INDEFINITE_WAIT );

   /*+--------------------------------------------------------------+*/
   /*| Create entry with shades of blue if animation is supported,  |*/
   /*| solid blue otherwise.                                        |*/
   /*+--------------------------------------------------------------+*/

   for (i = 0L; (ULONG)i < PAL_COUNT; i++)
   {
      LONG index = 222L;    /*  use if no animation  */

      /*+----------------------------------------------------+*/
      /*| INDEX = 128 POINTS EVENLY SPACED BETWEEN 1 AND 255 |*/
      /*+----------------------------------------------------+*/
      if(fPaletteCaps)
         index = ((float) (i + 1L) * 255.0) / (float) PAL_COUNT;

      alTable[i] = PC_RESERVED * 16777216 + LONGFromRGB(0, 0, index); /* BLUE */

   }

   hPal = GpiCreatePalette( hab,
                            (ULONG) LCOL_PURECOLOR,
                            (ULONG) LCOLF_CONSECRGB,
                            PAL_COUNT,
                            alTable );

   if( hPal == NULLHANDLE || hPal == GPI_ERROR)
   {
      DosReleaseMutexSem( hmtxPS );
      mprintf("GpiCreatePalette Error 0x%04x",ret = WinGetLastError(hab));
   }

   else if(GpiSelectPalette(hpsBuffer, hPal ) == PAL_ERROR)
   {
      DosReleaseMutexSem( hmtxPS );
      mprintf("GpiSelectPalette Error 0x%04x", ret = WinGetLastError(hab));
   }

   else if( WinRealizePalette(hwndPalette,hpsBuffer,&cclr) == PAL_ERROR)
   {
      DosReleaseMutexSem( hmtxPS );
      mprintf("WinRealizePalette Error 0x%04x", ret = WinGetLastError(hab));
   }

   else
   {
      DosReleaseMutexSem( hmtxPS );
   }

   return ret;
}  /*  end of PaletteInit()  */


/*+--------------------------------------------------------------------------+*/
/*| DrawPaletteLines - Procedure that draws the lines in the passed box.     |*/
/*+--------------------------------------------------------------------------+*/
VOID DrawPaletteLines(RECTL  rctl,
                      ULONG ulPalCount,
                      HPS hps)
{
   LONG  color = 0L;                 /* foreground (line) color      */
   POINTL ptl1,ptl2;

   ptl1.x = rctl.xLeft;
   ptl1.y = rctl.yTop;
   ptl2.x = rctl.xLeft;
   ptl2.y = rctl.yBottom;

   while(ptl1.x <= rctl.xRight)
   {
      if ( (ULONG) color >= ulPalCount )
         color = 0L;
      else
         color++;                   /* Increment foreground color   */

      ptl1.x ++;
      ptl2.x ++;

      GpiSetColor( hps, color );
      GpiMove( hps, &ptl1 );         /* Move to start point          */
      GpiLine( hps, &ptl2 );         /* Draw new line                */
    }
}  /*  end of DrawPaletteLines()  */



/*+--------------------------------------------------------------------------+*/
/*| DrawingThread - Procedure that animates the palette in the client window.|*/
/*+--------------------------------------------------------------------------+*/
static void DrawingThread( ULONG ulThreadArg )
{
   int   i;

   HAB habThread2 = WinInitialize( 0LU );                  /* Anchor block handle for thread */ 
   HAB hmqThread2 = WinCreateMsgQueue( habThread2, 0L );   /* Queue handle for thread        */ 

   while ( threadRun )
   {
      LONG temp;


/*+--------------------------------------------------------------------------+*/
/*|     The following if clause puts this thread to sleep as long as there   |*/
/*| is no speed flag set, i.e. the animation is 'Stop'ed.                    |*/
/*|     The else clause continues rotating the palette.                      |*/
/*+--------------------------------------------------------------------------+*/
      if (!speedSlow && !speedMedium && !speedFast )
      {
         DosSleep(500LU);
      }
      else
      {
         /*+-----------------------------------------------------------+*/
         /*| Request PS semaphore so that main thread does NOT use it. |*/
         /*+-----------------------------------------------------------+*/

         WinRequestMutexSem( hmtxPS, SEM_INDEFINITE_WAIT );

         temp = alTable[ 0 ];
         for(i = 0; (ULONG) i < PAL_COUNT - 1; i++)
         {
            alTable[ i ] = alTable[ i + 1 ];
         }
         alTable[ PAL_COUNT - 1 ] = temp;

         GpiAnimatePalette( hPal,
                            LCOLF_CONSECRGB,
                            0LU,
                            PAL_COUNT,
                            alTable);

         /*+-----------------------+*/
         /*| Release PS semaphore. |*/
         /*+-----------------------+*/

         DosReleaseMutexSem( hmtxPS );

         /*+---------------------------------------------+*/
         /* Surrender rest of timeslice to other threads. */
         /* Never do a DosSleep(0) as this will not give  */
         /* lower priority threads a time slice           */
         /*+---------------------------------------------+*/

         if(animationSpeed) DosSleep( animationSpeed );
      }
   }


   /*+----------------------------------------+*/
   /*| Clean up and terminate drawing thread. |*/
   /*+----------------------------------------+*/

   if(hPal)
      GpiDeletePalette( hPal );
   if(hmqThread2)
      WinDestroyMsgQueue( hmqThread2 );
   WinTerminate( habThread2 );
   DosExit( EXIT_THREAD, 0LU );
}     /*  end of DrawingThread()  */



/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*| FUNCTION NAME: mprintf                                                   |*/
/*| --------------                                                           |*/
/*|  A PM function for displaying a message box with a printf type format.   |*/
/*|                                                                          |*/
/*| EXPECTED INPUT:                                                          |*/
/*| ---------------                                                          |*/
/*|  See the definition of printf.                                           |*/
/*|                                                                          |*/
/*| EXPECTED OUTPUT:                                                         |*/
/*| ----------------                                                         |*/
/*|  Message Box                                                             |*/
/*|                                                                          |*/
/*| NOTES                                                                    |*/
/*| ----------------                                                         |*/
/*|  One must be careful not to call this within a paint routine, because    |*/
/*|  the closing of the message box will generate another paint message.     |*/
/*|                                                                          |*/
/*+--------------------------------------------------------------------------+*/
LONG mprintf(PSZ format, ...)
{
   va_list pArg;
   PSZ     buffer;
   LONG    lCount;

   WinAlarm( HWND_DESKTOP, WA_ERROR );

   if(!(buffer = malloc(513)))
      return 0L;

   va_start(pArg, format);

   lCount = vsprintf(buffer, format, pArg);

   WinMessageBox( HWND_DESKTOP,       /* parent window handle    */
                  HWND_DESKTOP,       /* owner window handle     */
                  buffer,             /* pointer to message text */
                  szClassName,        /* pointer to title text   */
                  MSG_BOX_ID,         /* message box identifier  */
                  MB_OK | MB_ERROR ); /* message box style       */

   free(buffer);
   
   return lCount;
}   /*  end of mprintf()  */



/*+--------------------------------------------------------------------------+*/
/*|                                                                          |*/
/*| FUNCTION NAME: SetPulldownState                                          |*/
/*| --------------                                                           |*/
/*|  A PM function for setting the check mark status of a menu item.         |*/
/*|                                                                          |*/
/*| EXPECTED INPUT:                                                          |*/
/*| ---------------                                                          |*/
/*|  Handle to the window containing the menu.                               |*/
/*|  Item id within the menu to check.                                       |*/
/*|  BOOL indicating state to set:  TRUE=CHECKED FALSE=UNCHECKED             |*/
/*|                                                                          |*/
/*| EXPECTED OUTPUT:                                                         |*/
/*| ----------------                                                         |*/
/*|  None                                                                    |*/
/*+--------------------------------------------------------------------------+*/
void SetPulldownState( HWND hwnd, USHORT item, BOOL state)
{
   WinCheckMenuItem(WinWindowFromID(hwnd, FID_MENU), item, state);
} /*  end of SetPulldownState()  */
/**************************  end of palette.c   *******************************/



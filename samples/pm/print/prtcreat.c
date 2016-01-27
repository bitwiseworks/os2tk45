/****************************************************************************
 * OS/2 Sample Print Application PRTSAMP
 *
 * File name: prtcreat.c
 *
 * Description: Contains code that the application does once at startup.
 *              These functions are called when client window procedure
 *              is called with the WM_CREATE messsage.
 *
 *                This source file contains the following functions:
 *
 *                Create(hwnd)
 *
 * Concepts:   initialization
 *
 * API's:      WinQueryAnchorBlock
 *             WinSetWindowULong
 *             WinQueryWindow
 *             WinWindowFromID
 *             WinSendMsg
 *             WinLoadString
 *             WinSetWindowText
 *             WinEnableMenuItem
 *             WinEnableControl
 *             WinOpenWindowDC
 *             GpiCreatePS
 *             GpiQueryDefaultViewMatrix
 *             WinCheckMenuItem
 *             GpiSetDefaultViewMatrix
 *
 *    Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PMASSERT.H
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
 ****************************************************************************/

/* os2 includes */
#define INCL_DEV
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
#include <os2.h>

/* c language includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <process.h>
#include <memory.h>
#include <sys\types.h>
#include <sys\stat.h>

/* application includes */
#include "prtsamp.h"
#include "prtsdlg.h"
#include "pmassert.h"


/**************************************************************************
*
* Name:   Create
*
* Description:  Does the WM_CREATE processing for the client window;
*               allocates memory for the block of main parameters and
*               starts to initialize it; creates thread 2 which operates
*               the object window.
*
* Concepts:     second thread
*
 * API's:      WinQueryAnchorBlock
 *             WinSetWindowULong
 *             WinQueryWindow
 *             WinWindowFromID
 *             WinSendMsg
 *             WinLoadString
 *             WinSetWindowText
 *             WinEnableMenuItem
 *             WinEnableControl
 *             WinOpenWindowDC
 *             GpiCreatePS
 *             GpiQueryDefaultViewMatrix
 *             WinCheckMenuItem
 *             GpiSetDefaultViewMatrix
 *
 * Parameters:   hwnd = client window handle
 *
 * Return:       a pointer to the main block of parameters
 *
 **************************************************************************/
PMAIN_PARM Create( HWND hwnd )
{
   BOOL            bOK;
   CHAR            szWork[ LEN_WORKSTRING ];
   HAB             hab;
   PMAIN_PARM      pmp;
   SIZEL           sizel;

   /* query hab */
   hab = WinQueryAnchorBlock( hwnd );

   /* allocate main block of parameters. see prtsamp.h */
   pmp = (PMAIN_PARM)calloc( 1, sizeof( MAIN_PARM ));
   pmassert( hab, pmp );

   /* set main parmaters pointer into client window words */
   bOK = WinSetWindowULong( hwnd, QWL_USER, (ULONG) pmp );
   pmassert( hab, bOK  );


   /* store hab and important window handles in the pmp */
   pmp->hab          = hab;
   pmp->hwndClient   = hwnd;
   pmp->hwndFrame    = WinQueryWindow( hwnd, QW_PARENT );
   pmp->hwndTitlebar = WinWindowFromID( pmp->hwndFrame, FID_TITLEBAR );
   pmp->hwndMenubar  = WinWindowFromID( pmp->hwndFrame, FID_MENU );
   pmp->hwndVScroll  = WinWindowFromID( pmp->hwndFrame, FID_VERTSCROLL);
   pmp->hwndHScroll  = WinWindowFromID( pmp->hwndFrame, FID_HORZSCROLL);

   pmassert( hab, pmp->hwndTitlebar );
   pmassert( hab, pmp->hwndMenubar  );
   pmassert( hab, pmp->hwndVScroll  );
   pmassert( hab, pmp->hwndHScroll  );


   /* disable client and menubar until application completely initializes. */
   WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0, (MPARAM)0 );

   /* set initial mode of operation to be unknown; shall know after file open */
   pmp->ulMode = pmp->ulNextMode = MODE_UNKNOWN;

   /* printing options */
   pmp->usCopies = 1;

   /* get program title; put in pmp and title bar */
   bOK = WinLoadString( pmp->hab, (HMODULE)NULLHANDLE,
                        PROGRAM_TITLE, LEN_WORKSTRING, szWork );
   pmassert( hab, bOK );
   pmp->pszTitle = strdup( szWork );
   pmassert( hab, pmp->pszTitle );
   bOK = WinSetWindowText( pmp->hwndTitlebar, pmp->pszTitle );
   pmassert( hab, bOK );

   /* gray certain menu items (this is a pmwin.h macro) */
   WinEnableMenuItem( pmp->hwndMenubar, IDM_PRINT,    FALSE );
   WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWFULL, FALSE );
   WinEnableMenuItem( pmp->hwndMenubar, IDM_VIEWHALF, FALSE );

   /* disable menu items; this will be enabled for text mode */
   WinEnableMenuItem( pmp->hwndMenubar, IDM_PAGEDOWN, FALSE );
   WinEnableMenuItem( pmp->hwndMenubar, IDM_SETFONT,  FALSE );

   /* disable scroll bar controls */
   WinEnableControl( pmp->hwndFrame, FID_HORZSCROLL, FALSE );
   WinEnableControl( pmp->hwndFrame, FID_VERTSCROLL, FALSE );

   /* store window dc for hwnd */
   pmp->hdcClient = WinOpenWindowDC( hwnd );
   pmassert( hab, pmp->hdcClient );

   /* create a normal screen ps for the screen dc in twips */
   /* 1440 twips == 1 inch; 20 twips == 1 point; 72 points per inch */
   sizel.cx = 0;
   sizel.cy = 0;
   pmp->hpsClient = GpiCreatePS(
                        hab,
                        pmp->hdcClient,
                        &sizel,
                        PU_TWIPS | GPIF_LONG | GPIT_NORMAL | GPIA_ASSOC  );
   pmassert( hab, pmp->hpsClient );


   /* set up null form, we will choose the default form later */
   strcpy(pmp->form.achFormName, "");

   /* set up default margins - 0.5 inch all round */
   pmp->form.fxLeftMargin = MAKEFIXED( 0, 0x8000 );
   pmp->form.fxRightMargin = MAKEFIXED( 0, 0x8000 );
   pmp->form.fxTopMargin = MAKEFIXED( 0, 0x8000 );
   pmp->form.fxBottomMargin = MAKEFIXED( 0, 0x8000 );

   /* query default view matrix */
   bOK = GpiQueryDefaultViewMatrix( pmp->hpsClient, 9, &pmp->matlfDefView );
   pmassert( pmp->hab, bOK );

   /* the scale of the matrix should be 1-to-1 */
   pmassert( hab, pmp->matlfDefView.fxM11 == MAKEFIXED( 1, 0 ));
   pmassert( hab, pmp->matlfDefView.fxM22 == MAKEFIXED( 1, 0 ));

   /* keep a floating point version of current scale in main parameters */
   pmp->floatScale = 1.0;
   /* set check on menu item that viewing is 1-to-1 scale */
   WinCheckMenuItem( pmp->hwndMenubar, IDM_VIEWFULL,  TRUE );

   /*
    * Set translations to zero, these will be updated when the window is
    * sized. Set default viewing matrix.
    */
   pmp->matlfDefView.lM31 = 0;
   pmp->matlfDefView.lM32 = 0;
   bOK = GpiSetDefaultViewMatrix( pmp->hpsClient, 9,
                                  &pmp->matlfDefView, TRANSFORM_REPLACE );
   pmassert( pmp->hab, bOK );

   /* set print dialog values to default values */
   pmp->usCopies = 1;
   pmp->fPrintAllPages = TRUE;
   pmp->usFirstPage = 1;
   pmp->usLastPage = 1;

   /* prepare the file dialog structure */
   pmp->filedlg.cbSize     = sizeof( FILEDLG );
   pmp->filedlg.fl         = FDS_OPEN_DIALOG | FDS_CENTER | FDS_HELPBUTTON;
   pmp->filedlg.pfnDlgProc = MyFileDlgProc;
   pmp->filedlg.ulUser     = (ULONG)pmp;

   /* and the font dialog structure */
   pmp->fontdlg.cbSize            = sizeof( FONTDLG );
   pmp->fontdlg.fl            =
          FNTS_CENTER | FNTS_HELPBUTTON | FNTS_VECTORONLY | FNTS_INITFROMFATTRS;
   pmp->fontdlg.usFamilyBufLen    = sizeof( pmp->szFamilyname );
   pmp->fontdlg.pszFamilyname     = pmp->szFamilyname;
   pmp->fontdlg.pfnDlgProc        = MyFontDlgProc;
   pmp->fontdlg.clrFore           = CLR_BLACK;
   pmp->fontdlg.clrBack           = CLR_WHITE;
   pmp->fontdlg.ulUser            = (ULONG)pmp;
   pmp->fontdlg.hpsScreen     = pmp->hpsClient;
   pmp->fontdlg.usWeight          = 5;                /* normal style */

   /* do more initialization on thread 2. See WM_CREATE case in prtobj.c */
   /* create object window which operates on thread 2 */
   /* see prtobj.c for thread 2 code and object window procedure */
   pmp->tidObjectThread = (TID)_beginthread( threadmain, NULL, LEN_STACK, pmp );
   pmassert( hab, pmp->tidObjectThread != 0 );

   return pmp;
}  /* end of Create()  */
/***************************  End of prtcreat.c ****************************/

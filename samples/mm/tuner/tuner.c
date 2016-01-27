
 /***************************************************************************
 *                                                                          *
 * File Name        : TUNER.C                                               *
 *                                                                          *
 * Description      : This program provides sample code for using the       *
 *                    Multimedia API's related to overlay cards that        *
 *                    contain tuners.                                       *
 *                                                                          *
 *                                                                          *
 * Concepts         :                                                       *
 *                                                                          *
 * Entry Points     : CheckArg()                                            *
 *                    GetOverlayColor()                                     *
 *                    CloseTv()                                             *
 *                    InitTv()                                              *
 *                    WmPaint()                                             *
 *                    SizeClient()                                          *
 *                    MainDialogProc()                                      *
 *                    main()                                                *
 *                                                                          *
 * Copyright        : COPYRIGHT IBM CORPORATION, 1991, 1992, 1993           *
 *                                                                          *
 *       DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *       sample code created by IBM Corporation. This sample code is not    *
 *       part of any standard or IBM product and is provided to you solely  *
 *       for  the purpose of assisting you in the development of your       *
 *       applications.  The code is provided "AS IS", without               *
 *       warranty of any kind.  IBM shall not be liable for any damages     *
 *       arising out of your use of the sample code, even if they have been *
 *       advised of the possibility of such damages.                        *
 *                                                                          *
 ****************************************************************************/

#define  INCL_DOS                   /* required to use Dos APIs.           */
#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tuner.h"                  /* Dialog Ids, Resource Ids etc        */


/****************************************************************************
 *                                                                          *
 * Name          : CheckArg                                                 *
 *                                                                          *
 * Description   : Reads the command line for an alternate digital video    *
 *                 device.                                                  *
 *                                                                          *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : ULONG   - 0 = Success, 1 = Fail                          *
 *                                                                          *
 ****************************************************************************/
void CheckArg( PSZ      pszArg,
                PAPPDATA pad )
{

    /*
     * The Device Name has been passed ( eg DigitalVideo02 )
     */
    if ( !strncmp( pszArg, "/d=", 3 )
      || !strncmp( pszArg, "/D=", 3 ) )
        strcpy( pad->szDevice, &pszArg[3] );

} /* endfunc CheckArg */




/****************************************************************************
 *                                                                          *
 * Name          : GetOverlayColor                                          *
 *                                                                          *
 * Description   : Gets the current overlay color from the device.          *
 *                                                                          *
 * Concept       : The overlay color is what the tuner card uses as a key   *
 *                 to determine where to overlay its video output. This     *
 *                 color can be changed from the Multimedia Setup program.  *
 *                                                                          *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : Value of overlay color                                   *
 *                                                                          *
 ****************************************************************************/
LONG  GetOverlayColor( HWND hwnd,
                       PSZ  pszDevice )
{

    USHORT      usUserParm       = (USHORT) NULL;
    ULONG       ulError;
    CHAR        szCommand[100],
                szReturn[100],
                szError[100];
    USHORT      usReservedLength = 100;
    HDC         hdcWin;
    HPS         hpsWin;
    LONG        aulColors[256];
    LONG        lTransColor     = 5,
                lIndexColor     = 5,
                lSysColors      = 256,
                lIndex;


    /*
     * Query the device overlay Color and match the physical color table
     * entry against logical color entry. This value is returned and stored
     * in the gloabal overlay color var.
     */
    sprintf( szCommand, "status tv1 transparent color wait" );
    ulError = mciSendString( szCommand,
                             szReturn,
                             usReservedLength,
                             hwnd,
                             usUserParm );
    if ( ulError )
    {
        sprintf( szError,
                 "No Overlay Color was returned for the device selected\n"
                 "Return Code = %d",
                 ulError );

        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       szError, "MCI Error",
                       ID_MSGBOX, MB_OK | MB_ERROR | MB_MOVEABLE);

    } /* endif */
    else
    {
        /* Convert the MCI Physical Color Table value */
        lIndex = atol(szReturn);

        /*
         * Calculate the Logical Color Table entry to be used as the
         * GPI overlay Color
         */
        hdcWin = WinOpenWindowDC( hwnd );
        DevQueryCaps( hdcWin, CAPS_COLORS, 1L, &lSysColors );

        hpsWin = WinGetPS( hwnd );

        if ( lSysColors <= 256 )
            GpiQueryRealColors( hpsWin,
                                0,        /* LCOLOPT_INDEX */
                                0,
                                lSysColors,
                                ( LONG * )aulColors );

        if ( lIndex >= lSysColors )
           lTransColor = lSysColors;

        switch ( lSysColors )
        {
             case 16:
             case 256:
                lTransColor = aulColors[lIndex];
                break;

             case 65536:
                lTransColor = ( ( (lIndex&0xf800) << 8 ) +
                                 ( (lIndex&0x07e0) << 5 ) +
                                 ( (lIndex&0x001f) << 3 ) );
                break;

             case 16777216:
                lTransColor = lIndex;
                break;
        } /* endswitch */

        /*
         * Query the Index version on the RGB derived
         */
        lIndexColor = GpiQueryColorIndex( hpsWin, 0L, lTransColor );

        WinReleasePS( hpsWin );
    } /* endelse */

    return( lIndexColor );

} /* endfunc GetOverlayColor */


/****************************************************************************
 *                                                                          *
 * Name          : CloseTv                                                  *
 *                                                                          *
 * Description   : Closes the digital video device.                         *
 *                                                                          *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : None.                                                    *
 *                                                                          *
 ****************************************************************************/
VOID CloseTv( HWND     hwnd,
              PAPPDATA pad )
{
    CHAR        szCommand[100],
                szReturn[100];
    USHORT      usReserved = 100,
                usUserParm = 0;


    sprintf( szCommand, "close tv1 wait" );
    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

} /* endfunc CloseTv */


/****************************************************************************
 *                                                                          *
 * Name          : InitTv                                                   *
 *                                                                          *
 * Description   : Opens the appropriate digital video device.              *
 *                                                                          *
 * Concepts      : The appropriate digitalvideo device is opened and the    *
 *                 monitor ( output ) window is set to the application      *
 *                 window.                                                  *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : ULONG   - 0 = success, 1 = fail                          *
 *                                                                          *
 ****************************************************************************/

ULONG InitTv( HWND     hwnd,
              PAPPDATA pad )
{
   CHAR        szCommand[100],
               szReturn[100],
               szEntry[100],
               szError[100];
   USHORT      usReserved = 100,
               usUserParm = 0;
   ULONG       ulError;


    /*
     * Open the required DigitalVideo Device
     */
    sprintf( szCommand,
             "open %s alias tv1 shareable wait", pad->szDevice );
    ulError = mciSendString( szCommand,
                             szReturn,
                             usReserved,
                             hwnd,
                             usUserParm );
    if ( ulError )
    {
        sprintf( szError,
                 "Could not open device: %s\nReturn Code = %u",
                 pad->szDevice, ulError );

        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       szError, "MCI Error",
                       ID_MSGBOX, MB_OK | MB_ERROR | MB_MOVEABLE );

        return( 1 );
    } /* endif */


   /*
    * Make sure that the device has a tuner.
    * Note: Some drivers don't allow sharing of the tuner even
    *       when the device was opened in sharable mode. In such
    *       a case, the second instance of the device will open
    *       with no tuner capablility.
    */

    sprintf( szCommand,
             "capability tv1 has tuner wait" );

    ulError = mciSendString( szCommand,
                             szReturn,
                             usReserved,
                             hwnd,
                             usUserParm );


    if ( strcmp( szReturn, "TRUE" ) != 0 )
    {
        sprintf( szError,
                 "Could not find a tuner \nfor device: %s",
                 pad->szDevice );

        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       szError, "MCI Error",
                       ID_MSGBOX, MB_OK | MB_ERROR | MB_MOVEABLE );

        CloseTv( hwnd, pad );

        return( 1 );
    } /* endif */


    /* Query tuner device for highest tv channel.
     */
    sprintf( szCommand,
             "status tv1 tuner high tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulHighChannel = atol( szReturn );

    /* Query tuner device for lowest tv channel
     */
    sprintf( szCommand,
             "status tv1 tuner low tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulLowChannel = atol( szReturn );

    /* Query tuner device for current tv channel
     */
    sprintf( szCommand,
             "status tv1 tuner tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulChannel = atol( szReturn );

    /* Set the text entry field to the current tv channel
     */
    WinSetDlgItemText ( hwnd, IDC_ENTRY, szReturn );


    /* Query tuner device for highest tv channel.
     */
    sprintf( szCommand,
             "status tv1 tuner high tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulHighChannel = atol( szReturn );

    /* Query tuner device for lowest tv channel
     */
    sprintf( szCommand,
             "status tv1 tuner low tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulLowChannel = atol( szReturn );

    /* Query tuner device for current tv channel
     */
    sprintf( szCommand,
             "status tv1 tuner tv channel wait" );

    mciSendString( szCommand,
                   szReturn,
                   usReserved,
                   hwnd,
                   usUserParm );

    pad->ulChannel = atol( szReturn );

    /* Set the text entry field to the current tv channel
     */
    WinSetDlgItemText ( hwnd, IDC_ENTRY, szReturn );


    /*
     * Set the client window as the monitor window
    */
    sprintf( szCommand,
             "window tv1 monitor handle %u wait",
             hwnd );
    ulError = mciSendString( szCommand,
                             szReturn,
                             usReserved,
                             hwnd,
                             usUserParm );

    /*
     * Turn on the monitor facility
     */
    sprintf( szCommand,
             "set tv1 monitor on wait" );
    ulError = mciSendString( szCommand,
                             szReturn,
                             usReserved,
                             hwnd,
                             usUserParm );

    /*
     * Get the overlay color for this device to use as the color to
     * paint the application window.
     */
    pad->ulOverlayColor = GetOverlayColor( hwnd, pad->szDevice );

    return( 0 );
} /* endfunc InitTv */



/****************************************************************************
 *                                                                          *
 * Name          : WmPaint                                                  *
 *                                                                          *
 * Description   : Called when the window needs to be repainted.            *
 *                                                                          *
 * Concepts      : When the window is resized or the visible region         *                      *
 *                 changes in any way, it needs to be repainted with        *
 *                 the appropriate overlay color.                           *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : None.                                                    *
 *                                                                          *
 ****************************************************************************/
VOID WmPaint ( PAPPDATA pad )

{
    RECTL   rectl;
    HPS     hps;

    /* Acquire a presentation space for the client window */
    hps = WinGetPS( pad->hwndMovie );

    /* Get the visible rectangles for the client window */
    WinQueryWindowRect( pad->hwndMovie, &rectl );
    WinFillRect( hps, &rectl, pad->ulOverlayColor );

}  /* endfunc WmPaint */



/****************************************************************************
 *                                                                          *
 * Name          : SizeClient                                               *
 *                                                                          *
 *                                                                          *
 * Description   : The first order of buisness is to make sure that the     *
 *                 new size for the frame window leaves at least enough     *
 *                 room for the control windows.                            *
 *                                                                          *
 *                 To account for the size of the button bar, it is         *
 *                 assumed that the two buttons and the text entry field    *
 *                 are of the same size and that they are aranged three     *
 *                 accross and one high.                                    *
 *                                                                          *
 *                 Once the size of all of the control windows is accounted *
 *                 for, the client/monitor window is resized to fit the     *
 *                 remaining top part of the frame window. The client       *
 *                 window is then painted with the overlay key color.       *
 *                                                                          *
 *                 Another potential problem is the fact that overlay       *
 *                 cards cant handle negative numbers for screen            *
 *                 coordinates. The way this program solves this is to      *
 *                 prevent the frame window from moving past the left edge  *
 *                 of the screen.                                           *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 * Parameters    : hwnd    -  Client window handle                          *
 *                 pad     -  Application data                              *
 *                                                                          *
 * Return        : None.                                                    *
 *                                                                          *
 ****************************************************************************/

VOID SizeClient( HWND hwnd, PAPPDATA pad )
{
   SWP               swp;                 /* Standard window rect       */
   static LONG       cxWidthBorder,       /* Size of top border         */
                     cyWidthBorder,       /* Size of side borders       */
                     cyTitleBar;          /* Size of title bar          */
   HWND              hwndButton;          /* handle for button window   */
   SWP               swpButton;           /* button window position     */
   ULONG             ulControlWindowCy;   /* Height of all control windows */
   ULONG             ulControlWindowCx;   /* Width of all control windows  */


   cxWidthBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   cyWidthBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
   cyTitleBar    = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

   /* Get handle for one of the button windows */
   hwndButton = WinWindowFromID( hwnd,          /* handle of parent  */
                                 IDC_UP );    /* ID of window      */

   /* Get the size of one of the button windows */
   WinQueryWindowPos( hwndButton, &swpButton ); /* Get button position */

   ulControlWindowCx = swpButton.x + swpButton.cx;
   ulControlWindowCy = swpButton.cy + 2 * cyWidthBorder + cyTitleBar;

   /* Get the size of the frame window */
   WinQueryWindowPos( hwnd, &swp );

   /* Make sure that the frame window is not too narow */
   if( swp.cy <= ulControlWindowCy )
      swp.cy = ulControlWindowCy + 1;

   /* Make sure that the frame window is not to short */
   if( swp.cx <= ulControlWindowCx )
      swp.cx = ulControlWindowCx + 1;

   /* Set the frame window size */
   WinSetWindowPos( hwnd,
                    HWND_TOP,
                    swp.x,
                    swp.y,
                    swp.cx,
                    swp.cy,
                    SWP_SIZE );

   /* Set the client window size and position to fit the top part of
    * the frame window
    */
   WinSetWindowPos(
      pad->hwndMovie,
      HWND_TOP,
      cxWidthBorder,
      cyWidthBorder + swpButton.cy,
      swp.cx - 2 * cxWidthBorder,
      swp.cy - 2 * cyWidthBorder - swpButton.cy - cyTitleBar,
      SWP_MOVE | SWP_SIZE );

} /* end SizeClient */






/*******************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will handle
 *               received messages such as pushbutton notifications, and
 *               entry field messages.
 *
 * Return      :
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND hwnd,
                                 ULONG msg,
                                 MPARAM mp1,
                                 MPARAM mp2)

{
   static PAPPDATA   pad;                       /* App data structure      */
   CHAR              szCommand[100],            /* sent to MciSendString   */
                     szReturn[100],             /* returned from MciSend.. */
                     szEntry[10];               /* Entry field text        */
   USHORT            usUserParm = (USHORT) NULL,
                     usReservedLength = 100;
   static ULONG      ulHeight,                  /* Size of desktop         */
                     ulWidth;
   PSWP              pswp;                      /* Standard window pos     */

   switch (msg)
   {
      case WM_INITDLG:
         /* Get the pointer to application defined data area
          */
         pad = ( PAPPDATA )mp2;

         ulHeight = WinQuerySysValue( HWND_DESKTOP,
                                      SV_CYSCREEN );

         ulWidth = WinQuerySysValue ( HWND_DESKTOP,
                                      SV_CXSCREEN );

         break;


      case WM_CONTROL:
         /*
          * If the channel text field has been manually changed
          * then update the tuner channel to match.
          */
         if(  SHORT1FROMMP( mp1 ) == IDC_ENTRY )
         {
            /* Get text from entry field */
            WinQueryWindowText( WinWindowFromID( hwnd, IDC_ENTRY ),
                                 sizeof( szEntry ),
                                 szEntry );

             pad->ulChannel = atol( szEntry );

            /* Make sure the selected channel is in range. If the
             * Channel is too low, it is assumed that the user has
             * not finished entering the channel and the program
             * will wait for the next digit.
             */
            if( pad->ulChannel < pad->ulLowChannel )
               break;

            /* If the selected channel is too high, the channel
             * is set to the highest supported channel number
             */
            if( pad->ulChannel > pad->ulHighChannel )
            {
               pad->ulChannel = pad->ulHighChannel;
               sprintf( szCommand,
                        "%d",
                        pad->ulChannel );

               WinSetDlgItemText ( hwnd, IDC_ENTRY, szCommand );
            }

            sprintf( szCommand,
                     "settuner tv1 tv channel %d wait",
                     pad->ulChannel );

            mciSendString( szCommand,
                           szReturn,
                           usReservedLength,
                           hwnd,
                           usUserParm );

         } /* end if */
         return 0;

      case WM_COMMAND:
         switch ( SHORT1FROMMP ( mp1 ) )
         {
            /* The user has selected the down button so
             * decrement the current channel.
             */
            case IDC_DOWN:
               pad->ulChannel--;
               break;

            /* The user has selected the up button so
             * increment the current channel
             */
            case IDC_UP:
               pad->ulChannel++;
               break;

         } /* end WM_COMMAND switch */

         /*
          * Make sure that the currently selected channel
          * is within the valid range of channels.
          */
         if( pad->ulChannel < pad->ulLowChannel )
            pad->ulChannel = pad->ulLowChannel;

         if( pad->ulChannel > pad->ulHighChannel )
            pad->ulChannel = pad->ulHighChannel;

         /* Set the tuner to the selected channel
          */
         sprintf ( szCommand,
                  "settuner tv1 tv channel %d wait",
                  pad->ulChannel );

         mciSendString ( szCommand,
                        szReturn,
                        usReservedLength,
                        hwnd,
                        usUserParm );

         /* Update the text entry field with the updated channel.
          */
         sprintf ( szReturn,
                  "%d",
                  pad->ulChannel );

         WinSetDlgItemText ( hwnd, IDC_ENTRY, szReturn );

         return 0;

      case WM_ADJUSTFRAMEPOS:
         /*
          * Most overlay cards do not handle clipping for a window that
          * is moved beyond the edges of the screen. This program handles
          * this limitation by not allowing the frame window to be moved
          * outside of the desktop. Another aproach is to resize the client
          * window so that it contained by the desktop, even if the frame
          * window is not. This would provide the illusion of allowing the
          * window to be moved outside of the desktop.
          */

         pswp = ( PSWP )mp1;

         /* Make sure that the window has not move past the left edge */
         if( pswp->x < 0 )
            pswp->x = 0;

         /* Make sure that the window has not move past the bottom edge */
         if( pswp->y < 0 )
            pswp->y = 0;


         /* Make sure that the window has not moved past the top edge */
         if( ( pswp->y + pswp->cy ) > ulHeight )
            pswp->y = ulHeight - pswp->cy;

         /* Make sure that the window has not moved past the right edge */
         if( ( pswp->x + pswp->cx) > ulWidth )
            pswp->x = ulWidth - pswp->cx;
         break;


      case WM_PAINT:
         /*
          * Call the procedure to resize the client window. The actual
          * painting of the client window is done later to allow the
          * frame window time to paint all of the control windows.
          */
         if( pad->hwndMovie != 0 )
            {
            SizeClient( hwnd, pad );
            WinPostMsg( hwnd, WM_PAINTMOVIE, 0L, 0L );
            }
         break;

      case WM_PAINTMOVIE:
         /*
          * Call the procedure to fill the client window with the
          * overlay card's key color.
          */
         WmPaint( pad );
         return 0;

      case WM_CLOSE:

         WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
         return 0;

   } /* end switch */

   /* Pass messages on to the frame window */
   return ( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );

}


 /******************************************************************************
 * Name         : main
 *
 * Description  : This function calls the Intialize procedure to prepare
 *                everything for the program's operation, enters the
 *                message loop, then call Finalize to shut everything down
 *                when the program is terminated.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : argc - Number of parameters passed into the program.
 *                argv - Command line parameters.
 *
 * Return       : None.
 *
 *************************************************************************/
void main(int argc, char *argv[])
{
   HAB         hab;
   HWND        hwndFrame;
   HMQ         hmq;
   QMSG        qmsg;
   CHAR        szCommand[20];
   ULONG       ulAllocFlags = PAG_COMMIT
                            | PAG_READ
                            | PAG_WRITE;
   PAPPDATA    pad;
   CHAR        chKey;

   /*
    * Set up the window specific data and set some defaults.
    * Note: The defaults set relate to the Hauppauge Celebrity card.
    */
   DosAllocMem ( ( PPVOID )&pad,
                 sizeof( APPDATA ),
                 ulAllocFlags );

   pad->ulConnector = 2;
   strcpy( pad->szDevice, "digitalvideo02" );
   pad->ulOverlayColor = 5;
   pad->hwndMovie = 0;


   /* Check if alternate device ordinal was passed from command line
    */
   if( argc > 1 )
      CheckArg( ( PSZ )argv[1], pad );


   /* Initialize PM */
   hab = WinInitialize(0);

   /* create a message queue for the window */
   hmq = WinCreateMsgQueue(hab,0);

   /* Load the Dialog - This will return the Handle to the Frame */
   hwndFrame = WinLoadDlg( HWND_DESKTOP,
                           HWND_DESKTOP,
                           ( PFNWP )MainDialogProc,
                           NULLHANDLE,
                           ID_DIALOG,
                           ( PVOID )pad);

   /* Create a client window that we can associate with the
    * digitalvideo device. This will be the output window for
    * the video overlay
    */
   WinRegisterClass( hab,
                     "MovieWindow",
                     (PFNWP) NULL,
                     CS_SIZEREDRAW | CS_MOVENOTIFY,
                     0 );

   pad->hwndMovie = WinCreateWindow( hwndFrame,
                                "MovieWindow",
                                NULL,
                                WS_VISIBLE,
                                0,
                                0,
                                0,
                                0,
                                hwndFrame,
                                HWND_TOP,
                                ID_MOVIEWINDOW,
                                NULL,
                                NULL );

   /* Update and size the client window
    */
   SizeClient( hwndFrame, pad );

   /* Initialise the overlay card.
    */
   if( InitTv ( pad->hwndMovie, pad ) == 0 )
   {
      /* Set dialog text field with startup value
       */
      sprintf ( szCommand,
               "%d",
               pad->ulChannel );

      WinSetDlgItemText ( hwndFrame, IDC_ENTRY, szCommand );

      /* Handle the messages: */
      while( ( WinGetMsg( hab, &qmsg, (HWND) NULL, 0, 0 ) != FALSE )
               && ( qmsg.msg != WM_QUIT ) )
      {
         /* Intercept WM_CHAR messages to filter non numeric characters
          * This is done to keep non numeric characters from being passed
          * to the text entry field
          */
         if( qmsg.msg == WM_CHAR )
         {
            chKey = SHORT1FROMMP( qmsg.mp2 );
            if(  ( ( chKey > 57 ) || ( chKey < 48 ) ) &&
                   ( SHORT2FROMMP( qmsg.mp2 ) != VK_BACKSPACE ) )
               continue;
         } /* end if */

         WinDispatchMsg( hab, &qmsg );

      }/* end while */

   }/* end if */

   CloseTv( hwndFrame, pad );
   WinDestroyWindow( pad->hwndMovie );
   WinDismissDlg( hwndFrame, TRUE );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

}/* end main */

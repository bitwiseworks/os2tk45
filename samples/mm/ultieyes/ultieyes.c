/*------------------------------------------------------------------------

   SOURCE FILE NAME:  ultieyes.c

                   Copyright (c) IBM Corporation 1981, 1993
                             All Rights Reserved

--------------------------------------------------------------------------*/

#define INCL_DOS
#define INCL_WINSHELLDATA
#define INCL_WIN
#define INCL_GPI
#define INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define INCL_32
#include <os2.h>
#include <os2me.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <dive.h>
#include <fourcc.h>
#include "ultieyes.h"

/***********************/
/*  Global Variables   */
/***********************/
POINTL    ptlOldMousePos;               /* save previous mouse position      */
BOOL      fRandomUp = TRUE;             /* random mode: moving up            */
BOOL      fRandomRight = TRUE;          /* random mode: moving right         */
ULONG     ulVideoX = 320;               /* width of video                    */
ULONG     ulVideoY = 240;               /* height of video                   */
UCHAR     szEyeFile[CCHMAXPATH];        /* file with frame table values      */
PSZ       pszMovieFile;                 /* movie file name                   */
ULONG     ulFrameNums[NUMFRAMENUMS];    /* frame table                       */
ULONG     ulCurrentFrame = 0;           /* save last frame displayed         */
double    EyeRatio = 0.125;             /* ratio of eye-nose to video width  */
CHAR      szReturn[CCHMAXPATH];         /* return value from mciSendString   */
HSWITCH   hSwitch;                      /* task list switch entry handle     */
SWCNTRL   switch_control;               /* for task switch entry             */
HWND      hwndFrame;                    /* regular standard frame            */
HWND      hwndNoFrame;                  /* frame-less frame                  */
HWND      hwndClient;                   /* client of hwndFrame               */
HWND      hwndNoFrameClient;            /* client of hwndNoFrame             */
BOOL      fNoFrame;                     /* which frame is visible?           */
BOOL      fRandom = FALSE;              /* random winks/blinks?              */
HPOINTER  vhCurrentPtr;                 /* save current mouse pointer        */
HINI      vhini = 0 ;                   /* profile handle                    */
HWND      hwndSysSubMenu;               /* frame system submenu              */

BOOL      fDebug = FALSE;               /* TRUE => display frame num in title*/

/************************************************************************/
/*  Name         :  Main                                                */
/*                                                                      */
/*  Description  :  Ultieyes main entry point.                          */
/*                                                                      */
/*  Logic        :  Initialize PM.                                      */
/*                  If frame table file name passed on command line,    */
/*                    open frame table file,                            */
/*                  else open default frame table file.                 */
/*                  Parse frame table file.                             */
/*                  Create two frame windows, one with no border or     */
/*                    title bar.                                        */
/*                  Add a switch entry for the process.                 */
/*                  Open the digitalvideo device.                       */
/*                  Load the .AVI file.                                 */
/*                  Start a timer for the client window.                */
/*                  Service the message queue.                          */
/*                  Destroy windows and terminate.                      */
/*                                                                      */
/*  Parameters   :  argc        - Command line parm count               */
/*                  argv        - Command line parms.  One optional     */
/*                                parameter is the frame table          */
/*                                file name.                            */
/*                                                                      */
/*  Return       :  N/A                                                 */
/************************************************************************/
main( int argc, char * argv[] )
  {
  HMQ       hmq;                        /* message queue handle              */
  HAB       hab;                        /* PM anchor block handle            */
  QMSG      qmsg;                       /* for message queue loop            */
  ULONG     rc;                         /* return code                       */
  ULONG     fSuccess;                   /* return bool                       */
  LONG      cyTitleBar;                 /* title bar height                  */
  ULONG     flCreate = FCF_TITLEBAR |   /* frame creation flags              */
                       FCF_SYSMENU  |
                       FCF_BORDER   |
                       FCF_ICON     |
                       FCF_NOBYTEALIGN;
  ULONG     flNoFrameCreate = FCF_NOBYTEALIGN; /* flag for frameless frame   */
  HFILE     FileHandle;                 /* for frame table file              */
  ULONG     Action;                     /* needed for DosOpen                */
  UCHAR     szMsgBuf[CCHMAXPATH];       /* for error messages                */
  SWP       swp;                        /* for recommended position          */
  LONG      cxWidthBorder;              /* system border width               */
  LONG      cyWidthBorder;              /* system border width               */
  ULONG     uSize;                      /* profile size                      */
  POINTL    ptlSave;                    /* position in profile               */



  /***************************************************************/
  /*  Initialize PM and create a message queue.                  */
  /***************************************************************/
  hab = WinInitialize( 0 );
  hmq = WinCreateMsgQueue( hab, 0 );


  /******************************************************************/
  /*  Query Video System capabilities to determine direct mode.     */
  /*  On VGA and other subsystems (8514) direct mode is unsupported */
  /*  ultieyes will not run on such systems.                        */
  /******************************************************************/

  if (!QueryDirectMode())
    {
       WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ)"Direct software motion video mode is not supported on this system.",
                      (PSZ)"Ultieyes - Direct Mode Support Detection",
                      0,
                      MB_ERROR | MB_OK );
       DosExit( EXIT_PROCESS, 0 );
    }

  /***************************************************************/
  /*  Open frame table file.                                     */
  /***************************************************************/
  strcpy( (PCHAR)szEyeFile, DEFAULT_EYE);
  if (argv[1])
    if (argv[1][0])
      strcpy( (PCHAR)szEyeFile, argv[1]);
  if ( DosOpen((PCHAR)szEyeFile,               /* File path name */
               &FileHandle,             /* File handle */
               &Action,                 /* Action taken */
               0,
               0,                       /* no attributes                     */
               OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_SHARE_DENYNONE,
               0))                      /* Reserved (must be zero) */
    {
    strcpy( (PCHAR)szMsgBuf, "Cannot open control file: ");
    strcat( (PCHAR)szMsgBuf, (PCHAR)szEyeFile);
    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)szMsgBuf, (PSZ)"UltiEyes - Error Opening Control File", 0,
                   MB_ERROR | MB_OK );
    DosExit( EXIT_PROCESS, 0 );
    }

  /***************************************************************/
  /*  Parse out movie file name and frame numbers.               */
  /***************************************************************/
  ParseControlFile( FileHandle, (ULONG *) &ulFrameNums);

  /***************************************************************/
  /*  Register class and create two standard windows.            */
  /*  fNoFrame is used during window initialization, and must be */
  /*  TRUE for the "frameless" frame and false for the regular   */
  /*  frame.                                                     */
  /***************************************************************/
  fSuccess = WinRegisterClass( hab, (PSZ)"Client",
                               (PFNWP)MainClientWindowProc,
                               CS_SIZEREDRAW | CS_MOVENOTIFY |
                               CS_CLIPCHILDREN, 4);
  fNoFrame = TRUE;                      /* Initialize frameless first        */
  hwndNoFrame = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE,
                                    &flNoFrameCreate,
                                    (PSZ)"Client", (PSZ)"UltiEyes",
                                    0, 0, 0, &hwndNoFrameClient);
  fNoFrame = FALSE;                     /* Now initialize framed window      */
  hwndFrame = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE, &flCreate,
                                  (PSZ)"Client", (PSZ)"UltiEyes",
                                  0, 0, ID_ICON, &hwndClient);
  WinGetLastError( hab );

  /***************************************************************/
  /*  Add a switch list entry.                                   */
  /***************************************************************/
  switch_control.hwnd = hwndFrame;
  switch_control.hwndIcon = 0;
  switch_control.hprog = 0;
  switch_control.idProcess = 0;
  switch_control.idSession = 0;
  switch_control.uchVisibility = SWL_VISIBLE;
  switch_control.fbJump = SWL_JUMPABLE;
  strcpy( switch_control.szSwtitle, "UltiEyes - ");
  strcat( switch_control.szSwtitle, (PCHAR)szEyeFile);
  hSwitch = WinAddSwitchEntry( &switch_control);
  WinGetLastError( hab );
  WinSetWindowText( hwndFrame, (PCHAR)szEyeFile);

  /***************************************************************/
  /*  Open the software motion video device.  If opened          */
  /*  successfully, set the time format to frames and load       */
  /*  the movie file.  If loaded okay, start the timer.          */
  /***************************************************************/
  if (!OpenTheDevice( hwndClient))
    rc = WinPostMsg( hwndClient, WM_CLOSE, 0, 0);
  else
    {
    fSuccess = SendString(hwndClient, (PCHAR)"acquire movie wait", 0);
    fSuccess = SendString(hwndClient, (PCHAR)"set movie time format frames wait", 0);
    if (!LoadTheFile(hwndClient))
      rc = WinPostMsg( hwndClient, WM_CLOSE, 0, 0);
    else
      rc = WinStartTimer( hab, hwndClient, 1, 10);
    fSuccess = SendString(hwndClient, (PCHAR)"release movie wait", 0);
    }                               /* end: opened device okay           */

  /***************************************************************/
  /*  Query title bar height.  Get the recommended position      */
  /*  from the system.  Get the size from the movie size plus    */
  /*  the title bar height.                                      */
  /***************************************************************/
  cyTitleBar = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
  cxWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CXBORDER);
  cyWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYBORDER);
  vhini = PrfOpenProfile (hab, (PSZ)"ultieyes.ini");
  if (!vhini) vhini = HINI_USER;
  if ( PrfQueryProfileSize (vhini, (PSZ)szEyeFile, (PSZ)szMinor, &uSize) &&       /*::*/
       uSize == sizeof(POINTL) &&
       PrfQueryProfileData (vhini, (PSZ)szEyeFile, (PSZ)szMinor,
                            (PVOID) &ptlSave, (PULONG) &uSize) )
    {
    swp.x = ptlSave.x;
    swp.y = ptlSave.y;
    }
  else
    {
    WinQueryTaskSizePos( hab, 0, &swp);
    }
  fSuccess = WinSetWindowPos(hwndFrame, HWND_TOP,
      swp.x,
      swp.y,
      ulVideoX + (2 * cxWidthBorder),
      ulVideoY + cyTitleBar + (2 * cyWidthBorder),
      SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_ACTIVATE);

  /***************************************************************/
  /*  Service the message queue.                                 */
  /***************************************************************/
  while( WinGetMsg( hab, &qmsg, 0, 0, 0 ) )
         WinDispatchMsg( hab, &qmsg );

  /***************************************************************/
  /*  Destroy windows and terminate PM.                          */
  /***************************************************************/
  if (vhini != HINI_USER) PrfCloseProfile (vhini);
  WinDestroyWindow( hwndFrame);
  WinDestroyWindow( hwndNoFrame);
  WinDestroyMsgQueue( hmq );
  WinTerminate( hab );
  DosExit( EXIT_PROCESS, 0 );
  return( 0 );
  } /* End main() */


/*****************************************************************/
/*  Name         :  Main                                         */
/*                                                               */
/*  Description  :  Ultieyes main entry point.                   */
/*                                                               */
/*  Logic        :  Switch on message:                           */
/*                    if create:                                 */
/*                      insert separator and NOFRAME into        */
/*                        system menu                            */
/*                    if button 1 click:                         */
/*                      if in left 3/8 of window                 */
/*                        play wink on left side                 */
/*                      else if in right 3/8 of window           */
/*                        play wink on right side                */
/*                      else                                     */
/*                        play blink                             */
/*                    if button 1 doubleclick:                   */
/*                      send NOFRAME command                     */
/*                    if close:                                  */
/*                      close digitalvideo device                */
/*                      post QUIT to message queue               */
/*                    if command NOFRAME:                        */
/*                      if currently showing frame               */
/*                        get current position                   */
/*                        map to desktop                         */
/*                        move frameless frame to same pos       */
/*                        hide frame                             */
/*                        show frameless frame                   */
/*                        start timer for frameless client       */
/*                        send window handle to MCI for          */
/*                          frameless client                     */
/*                        stop timer for frame client            */
/*                      else                                     */
/*                        hide frameless frame                   */
/*                        show frame                             */
/*                        start timer for frame client           */
/*                        send window handle to MCI for          */
/*                          frame client                         */
/*                        stop timer for frameless client        */
/*                    if timer:                                  */
/*                      get mouse position                       */
/*                      if changed since saved pos               */
/*                        save current pos                       */
/*                        map mouse pos to center of video       */
/*                        if in center of video                  */
/*                          select cross-eyed frame              */
/*                        else                                   */
/*                          calculate frame from angle           */
/*                        if calc'd frame has changed            */
/*                          if not doing next sequential frame   */
/*                            seek to calc'd frame               */
/*                          step movie by 1                      */
/*                          if fDebug                            */
/*                            display frame num in titlebar      */
/*                    call default window proc                   */
/*                                                               */
/*  Parameters   :  hwnd        - window handle                  */
/*                  msg         - PM message                     */
/*                  mp1         - parm 1                         */
/*                  mp2         - parm2                          */
/*                                                               */
/*  Return       :  value returned by default window proc        */
/*****************************************************************/

MRESULT EXPENTRY MainClientWindowProc( HWND hwnd, ULONG msg,
                                       MPARAM mp1, MPARAM mp2 )
  {
  MRESULT       mr = FALSE;
  ULONG         rc;                     /* return codes                      */
  BOOL          fSuccess;               /* return codes                      */
  POINTL        ptlMouse;               /* mouse pointer position            */
  HWND          hwndTitle;              /* handle of title bar window        */
  UCHAR         szBuf[40];              /* for debug string                  */
  double        Angle;                  /* angle of mouse to center of video */
  ULONG         ulFrame;                /* calculated frame number           */
  CHAR          szSeekString[CCHMAXPATH]; /* for building seek command       */
  CHAR          szSeekPos[10] = "";     /* frame to seek to                  */
  CHAR          szPlayString[CCHMAXPATH]; /* for building play command       */
  CHAR          szFromPos[10] = "";     /* frame to play frome               */
  CHAR          szToPos[10] = "";       /* frame to play to                  */
  SWP           swp;                    /* window position                   */
  HWND          hwndSysMenuMain;        /* frame menu                        */
  USHORT        idSysMenu;              /* menu item id                      */
  MENUITEM      MenuItem;               /* system submenu item               */
  POINTL        ptl;                    /* for coordinate mapping            */
  ULONG         ulRandom;               /* random number                     */

  switch( msg )
    {
    case WM_CREATE:
      if (!fNoFrame)
        {
        /*********************************************************/
        /*  If this is the client with a visible frame, get      */
        /*  the system menu and insert a separator and the       */
        /*  No Frame selection.                                  */
        /*********************************************************/
        hwndFrame = WinQueryWindow( hwnd, QW_PARENT);
        hwndSysMenuMain = WinWindowFromID (hwndFrame, FID_SYSMENU);
        idSysMenu = SHORT1FROMMR (WinSendMsg (hwndSysMenuMain,
                                  MM_ITEMIDFROMPOSITION, 0L, 0L));
        WinSendMsg (hwndSysMenuMain, MM_QUERYITEM,
                    MPFROM2SHORT (idSysMenu, FALSE), MPFROMP(&MenuItem));
        hwndSysSubMenu = MenuItem.hwndSubMenu;
        MenuItem.iPosition   = MIT_END;
        MenuItem.afStyle     = MIS_SEPARATOR;
        MenuItem.afAttribute = (ULONG)NULL;
        MenuItem.hwndSubMenu = (HWND)NULL;
        MenuItem.hItem       = (ULONG)NULL;
        MenuItem.id          = 0;
        WinSendMsg (hwndSysSubMenu,
                    MM_INSERTITEM,
                    (MPARAM)&MenuItem,
                    (MPARAM)"");
        WinGetLastError( WinQueryAnchorBlock( hwnd));
        MenuItem.iPosition   = MIT_END;
        MenuItem.afStyle     = MIS_TEXT;
        MenuItem.afAttribute = (ULONG)NULL;
        MenuItem.hwndSubMenu = (HWND)NULL;
        MenuItem.hItem       = (ULONG)NULL;
        MenuItem.id          = (ULONG)IDM_NOFRAME;
        WinSendMsg (hwndSysSubMenu,
                    MM_INSERTITEM,
                    (MPARAM)&MenuItem,
                    (MPARAM)"No ~Frame");
        WinGetLastError( WinQueryAnchorBlock( hwnd));
        MenuItem.iPosition   = MIT_END;
        MenuItem.afStyle     = MIS_TEXT;
        MenuItem.afAttribute = (ULONG)NULL;
        MenuItem.hwndSubMenu = (HWND)NULL;
        MenuItem.hItem       = (ULONG)NULL;
        MenuItem.id          = (ULONG)IDM_RANDOM;
        WinSendMsg (hwndSysSubMenu,
                    MM_INSERTITEM,
                    (MPARAM)&MenuItem,
                    (MPARAM)"R~andom");
        WinGetLastError( WinQueryAnchorBlock( hwnd));
        /***********************************************************/
        /*  Blow away saved mouse position and frame number to     */
        /*  force seek/step of appropriate frame after playing     */
        /*  wink or blink.                                         */
        /***********************************************************/
        ptlOldMousePos.x = ptlOldMousePos.y = -1;
        ulCurrentFrame = 99999;
        }
      break;

    case WM_BUTTON1CLICK:
      /***********************************************************/
      /*  Play a wink or a blink.                                */
      /***********************************************************/
      ptl.x = SHORT1FROMMP(mp1);
      ptl.y = SHORT2FROMMP(mp1);
      fSuccess = WinQueryWindowPos( hwnd, &swp);
      if (ptl.x <= swp.cx * 3 / 8)
        {
        _ultoa( ulFrameNums[NUMFRAMENUMS-6], szFromPos, 10);
        _ultoa( ulFrameNums[NUMFRAMENUMS-5], szToPos, 10);
        }
      else if (ptl.x >= swp.cx * 5 / 8)
        {
        _ultoa( ulFrameNums[NUMFRAMENUMS-4], szFromPos, 10);
        _ultoa( ulFrameNums[NUMFRAMENUMS-3], szToPos, 10);
        }
      else
        {
        _ultoa( ulFrameNums[NUMFRAMENUMS-2], szFromPos, 10);
        _ultoa( ulFrameNums[NUMFRAMENUMS-1], szToPos, 10);
        }
      strcpy( szPlayString, "play movie from ");
      strcat( szPlayString, szFromPos);
      strcat( szPlayString, " to ");
      strcat( szPlayString, szToPos);
      strcat( szPlayString, " wait");
      fSuccess = SendString(hwnd, (PCHAR)"acquire movie wait", 0);
      fSuccess = SendString(hwnd, szPlayString, 0);
      fSuccess = SendString(hwnd, (PCHAR)"release movie wait", 0);

      if (!fRandom)
        {
        /***********************************************************/
        /*  Blow away saved mouse position and frame number to     */
        /*  force seek/step of appropriate frame after playing     */
        /*  wink or blink.                                         */
        /***********************************************************/
        ptlOldMousePos.x = ptlOldMousePos.y = -1;
        ulCurrentFrame = 99999;
        }
      break;

    case WM_BUTTON1DBLCLK:
      /***********************************************************/
      /*  Same as if No Frame selected from system menu.         */
      /***********************************************************/
      WinSendMsg( hwnd, WM_COMMAND, (MPARAM)IDM_NOFRAME, 0);
      break;

    case WM_ADJUSTWINDOWPOS:
      /***********************************************************/
      /*  If no frame, keep window on bottom.                    */
      /***********************************************************/
      if (fNoFrame)
        {
        ((PSWP) mp1)->fl |= SWP_ZORDER;
        ((PSWP) mp1)->hwndInsertBehind = HWND_BOTTOM;
        }
      break;

    case WM_FOCUSCHANGE:
      /***********************************************************/
      /*  If hwndFrame is gaining focus, put it on top.          */
      /*  If hwndNoFrame is gaining focus, put it on bottom.     */
      /***********************************************************/
      if (SHORT1FROMMP(mp2))            /* if receiving focus                */
        {
        if (!fNoFrame)
          fSuccess = WinSetWindowPos(hwndFrame, HWND_TOP,
                                     0, 0, 0, 0, SWP_ZORDER);
        else
          fSuccess = WinSetWindowPos(hwndNoFrame, HWND_BOTTOM,
                                     0, 0, 0, 0, SWP_ZORDER);
        }
      break;

    case WM_CLOSE:
      /***********************************************************/
      /*  Close digitalvideo device and post a quit.             */
      /***********************************************************/
      CloseTheDevice();
      fSuccess = WinQueryWindowPos( hwndFrame, &swp);
      ptl.x = swp.x;
      ptl.y = swp.y;
      PrfWriteProfileData (vhini, (PSZ)szEyeFile, (PSZ)szMinor,
                           (PVOID)&ptl, sizeof(POINTL));
      rc = WinPostMsg( hwnd, WM_QUIT, 0, 0);
      break;

    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDM_RANDOM:
          fRandom = !fRandom;
          WinSendMsg(hwndSysSubMenu, MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_RANDOM, TRUE),
                     MPFROM2SHORT(MIA_CHECKED,
                                  (fRandom ? MIA_CHECKED : FALSE)));
          if (fRandom)
            {
            ulRandom = rand() * 10000 / RAND_MAX;
            rc = WinStartTimer( WinQueryAnchorBlock( hwnd),
                                hwndClient, 2, ulRandom);
            }
          else
            rc = WinStopTimer( WinQueryAnchorBlock( hwnd),
                               hwndClient, 2);
          break;

        case IDM_NOFRAME:
          if (!fNoFrame)
            {
            /*****************************************************/
            /*  If switching to frameless frame, set frameless   */
            /*  frame position to our current position.  Hide    */
            /*  visible frame, show frameless frame, stop one    */
            /*  timer, start the other timer, and send new       */
            /*  window handle to digitalvideo device.            */
            /*****************************************************/
            WinQueryWindowPos( hwnd, &swp );
            ptl.x = swp.x;
            ptl.y = swp.y;
            rc = WinMapWindowPoints( hwndFrame, HWND_DESKTOP, &ptl, 1);
            rc = WinSetWindowPos( hwndNoFrame, HWND_BOTTOM,
                             ptl.x, ptl.y, swp.cx, swp.cy,
                             SWP_MOVE | SWP_SIZE | SWP_ZORDER );
            rc = WinShowWindow( hwndFrame, FALSE);
            rc = WinShowWindow( hwndNoFrame, TRUE);
            fNoFrame = TRUE;
            WinGetLastError( WinQueryAnchorBlock( hwnd));
            rc = WinStartTimer( WinQueryAnchorBlock( hwnd),
                                hwndNoFrameClient, 1, 100);
            rc = SetWindowHandle( hwndNoFrameClient);
            switch_control.hwnd = hwndNoFrame;
            rc = WinChangeSwitchEntry( hSwitch, &switch_control);
            rc = WinStopTimer( WinQueryAnchorBlock( hwnd),
                               hwndClient, 1);
            }
          else
            {
            /*****************************************************/
            /*  If switching to visible frame, hide frameless    */
            /*  frame, show regular frame, stop one timer, start */
            /*  the other timer and send new window handel to    */
            /*  digitalvideo device.                             */
            /*****************************************************/
            fNoFrame = FALSE;
            rc = WinShowWindow( hwndNoFrame, FALSE);
            rc = WinShowWindow( hwndFrame, TRUE);
            rc = WinStartTimer( WinQueryAnchorBlock( hwnd),
                                hwndClient, 1, 100);
            rc = SetWindowHandle( hwndClient);
            switch_control.hwnd = hwndFrame;
            rc = WinChangeSwitchEntry( hSwitch, &switch_control);
            rc = WinStopTimer( WinQueryAnchorBlock( hwnd),
                               hwndNoFrameClient, 1);
            }
          break;                        /* end: IDM_NOFRAME                  */
        }                               /* end: WM_COMMAND                   */
      break;

    case WM_TIMER:
      if (mp1 == (MPARAM)1)
        {
        /*********************************************************/
        /*  If random mode, adjust simulated mouse position,     */
        /*  else see if mouse moved to get new frame.            */
        /*********************************************************/
        if (fRandom)
          {
          /***************************************************/
          /*  If too close to edge of screen, turn around.   */
          /***************************************************/
          fSuccess = WinQueryWindowPos( HWND_DESKTOP, &swp);
          if (fRandomUp && (swp.cy <= ptlOldMousePos.y + RANDOM_SPEED))
            fRandomUp = FALSE;
          else if (!fRandomUp && (RANDOM_SPEED > ptlOldMousePos.y))
            fRandomUp = TRUE;
          if (fRandomRight && (swp.cx <= ptlOldMousePos.x + RANDOM_SPEED))
            fRandomRight = FALSE;
          else if (!fRandomRight && (RANDOM_SPEED > ptlOldMousePos.x))
            fRandomRight = TRUE;
          ptlMouse.x = ptlOldMousePos.x + (fRandomRight ? RANDOM_SPEED :
                                                         -RANDOM_SPEED);
          ptlMouse.y = ptlOldMousePos.y + (fRandomUp ? RANDOM_SPEED :
                                                      -RANDOM_SPEED);
          sprintf( (PCHAR)szBuf, "(%d,%d)", ptlMouse.x, ptlMouse.y);
          }                             /* end: if random mode               */
        else
          rc = WinQueryPointerPos( HWND_DESKTOP, &ptlMouse);
        if ((ptlOldMousePos.x != ptlMouse.x) ||
            (ptlOldMousePos.y != ptlMouse.y))
          {
          memcpy( &ptlOldMousePos, &ptlMouse, sizeof(POINTL));
          fSuccess = WinQueryWindowPos( hwnd, &swp);
          WinMapWindowPoints( HWND_DESKTOP, hwnd, &ptlMouse, 1);
          ptlMouse.x -= swp.cx/2;
          ptlMouse.y -= swp.cy/2;

          /*******************************************************/
          /*  If mouse is near middle of video, used special     */
          /*  cross-eyed frame, else calculate frame based       */
          /*  on arc-tangent of mouse position.                  */
          /*******************************************************/
          if (CrossEyed( &ptlMouse, swp.cx))
            ulFrame = ulFrameNums[0];
          else
            {
            Angle = atan2( ptlMouse.y, ptlMouse.x );
            ulFrame = CalcFrame( Angle );
            }

          /*******************************************************/
          /*  If calculated frame is not the same as last        */
          /*  calculated frame...                                */
          /*******************************************************/
          if (ulFrame != ulCurrentFrame)
            {
            _ultoa(ulFrame,szSeekPos,10);
            fSuccess = SendString(hwnd, (PCHAR)"acquire movie wait", 0);

            /*****************************************************/
            /*  If calculated frame is next sequential frame     */
            /*  after last calculated frame, then we're          */
            /*  already on it and there's no need to seek.       */
            /*****************************************************/
            if (ulFrame != ulCurrentFrame+1)
              {
              strcpy( szSeekString, (PCHAR)"seek movie to ");
              strcat( szSeekString, szSeekPos);
              strcat( szSeekString, " wait");
              fSuccess = SendString(hwnd, szSeekString, 0);
              }

            /*****************************************************/
            /*  Step one frame to display video.                 */
            /*****************************************************/
            fSuccess = SendString(hwnd, (PCHAR)"step movie by 1 wait", 0);
            ulCurrentFrame = ulFrame;
            fSuccess = SendString(hwnd, (PCHAR)"release movie wait", 0);

            if (fDebug)
              {
              hwndTitle = WinWindowFromID( hwndFrame, FID_TITLEBAR);
              sprintf( (PCHAR)szBuf, "%d (%d,%d)", ulFrame, ptlMouse.x, ptlMouse.y);
              rc = WinSetWindowText( hwndTitle, (PCHAR)szBuf );
              }                         /* end: Debug                        */
            }                           /* end: frame changed                */
          }                             /* end: mouse has moved              */
        }                               /* end: this is our timer            */
      if (mp1 == (MPARAM)2)             /* timer for random blinks           */
        {
        rc = WinStopTimer( WinQueryAnchorBlock( hwnd),
                           hwndClient, 2);
        ulRandom = rand() * 4 / RAND_MAX;
        switch (ulRandom)
          {
          case 0:                       /* wink left side                    */
            WinSendMsg( fNoFrame ? hwndNoFrameClient : hwndClient,
                        WM_BUTTON1CLICK, MPFROM2SHORT( 0, 0), 0);
            break;
          case 1:                       /* wink right side                   */
            WinSendMsg( fNoFrame ? hwndNoFrameClient : hwndClient,
                        WM_BUTTON1CLICK, MPFROM2SHORT( ulVideoX-1, 0), 0);
            break;
          case 2:                       /* blink                             */
            WinSendMsg( fNoFrame ? hwndNoFrameClient : hwndClient,
                        WM_BUTTON1CLICK, MPFROM2SHORT( ulVideoX/2, 0), 0);
            break;
          }
        ulRandom = (rand() * 100000 / RAND_MAX);
        rc = WinStartTimer( WinQueryAnchorBlock( hwnd),
                            hwndClient, 2, ulRandom);
        }
      break;
    }                                   /* end: switch(msg)                  */

  mr = WinDefWindowProc( hwnd, msg, mp1, mp2 );
  /**************************************************************/
  /*  Don't allow the no-frame window to activate or get focus. */
  /**************************************************************/
  if (fNoFrame &&
      ((msg == WM_SETFOCUS)      ||
       (msg == WM_ACTIVATE)      ||
       (msg == WM_SINGLESELECT)  ||
       (msg == WM_SETSELECTION)  ||
       (msg == WM_BUTTON1UP)     ||
       (msg == WM_BUTTON1DOWN)   ||
       (msg == WM_BUTTON1CLICK)  ||
       (msg == WM_BUTTON2UP)     ||
       (msg == WM_BUTTON2DOWN)   ||
       (msg == WM_BUTTON2CLICK)))
    {
    fSuccess = WinSetWindowPos(hwndNoFrame, HWND_BOTTOM,
                               0, 0, 0, 0, SWP_ZORDER);
    return FALSE;
    }
  return mr;
  }                                     /* end: MainClientWindowProc         */


/*****************************************************************/
/*  Name         :  ParseControlFile                             */
/*                                                               */
/*  Description  :  Parse frame table file.                      */
/*                                                               */
/*  Logic        :  Read movie file name and frame number        */
/*                  table from frame table file.                 */
/*                                                               */
/*                  Frame table file contains 25 lines.  First   */
/*                  line contains the filename of the movie      */
/*                  file.  The next 24 lines are the frame       */
/*                  number table (0-23).  The frame numbers      */
/*                  are as follows (symbols refer to points of   */
/*                  the compass):                                */
/*                                                               */
/*                   0      - Cross-eyed frame.                  */
/*                   1      - E                                  */
/*                   2      - ENE                                */
/*                   3      - NE                                 */
/*                   4      - NNE                                */
/*                   5      - N                                  */
/*                   6      - NNW                                */
/*                   7      - NW                                 */
/*                   8      - WNW                                */
/*                   9      - W                                  */
/*                  10      - WSW                                */
/*                  11      - SW                                 */
/*                  12      - SSW                                */
/*                  13      - S                                  */
/*                  14      - SSE                                */
/*                  15      - SE                                 */
/*                  16      - ESE                                */
/*                  17      - E (again)                          */
/*                  18-19   - right-eye wink sequence            */
/*                            (left side of video)               */
/*                  20-21   - left-eye wink sequence             */
/*                            (right side of video)              */
/*                  22-23   - blink sequence                     */
/*                                                               */
/*  Parameters   :  FileHandle  - frame table file handle        */
/*                  ulFrameNums - pointer to frame table         */
/*                                                               */
/*  Return       :  none                                         */
/*****************************************************************/
VOID ParseControlFile( HFILE FileHandle, ULONG * ulFrameNums)
  {
  ULONG   BytesRead;     /* Bytes read (returned) */
  UCHAR   szMsgBuf[CCHMAXPATH];
  PSZ     buffer;
  USHORT  idx;
  PSZ     pszFrameNum;

  buffer = calloc( FILE_BUF_SIZE+1, 1);
  if (DosRead(FileHandle, buffer, FILE_BUF_SIZE, &BytesRead))
    {
    strcpy( (PCHAR)szMsgBuf, "Cannot read from control file");
    WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ)szMsgBuf, (PSZ)"UltiEyes - Error Reading Control File", 0,
                   MB_ERROR | MB_OK );
    }
  else
    {
    pszMovieFile = (PSZ) strtok( (PCHAR) buffer, " \r\n");
    for (idx=0; idx < NUMFRAMENUMS; idx++)
      {
      pszFrameNum = (PSZ) strtok( NULL, " \r\n");
      ulFrameNums[idx] = atoi( (PCHAR) pszFrameNum);
      }
    }                                   /* end: read ok                      */

  }                                     /* end: ParseControlFile             */

/*****************************************************************/
/*  Name         :  CalcFrame                                    */
/*                                                               */
/*  Description  :  Calculate frame number based on mouse        */
/*                  angle.                                       */
/*                                                               */
/*  Logic        :  If angle is >= 0                             */
/*                    interpolate frame number from table        */
/*                      using "northern" frames (1-9)            */
/*                  else                                         */
/*                    interpolate frame number from table        */
/*                      using "southern" frames (9-17)           */
/*                                                               */
/*  Parameters   :  Angle       - angle of mouse to video        */
/*                                center                         */
/*                                                               */
/*  Return       :  frame number                                 */
/*****************************************************************/
ULONG CalcFrame( double Angle)
  {
  double sector;
  ULONG ulBaseFrame;
  ULONG ulNewFrame;
  double remainder;

  sector = Angle * 8.0 / PI;
  if ( Angle >= 0.0)
    {
    ulBaseFrame = (ULONG) floor( sector) + 1;
    remainder = fmod( Angle, PI/8.0);
    ulNewFrame = ulFrameNums[ulBaseFrame] +
                 ((ulFrameNums[ulBaseFrame+1] -
                   ulFrameNums[ulBaseFrame]) * remainder * 8.0 / PI);
    }
  else
    {
    ulBaseFrame = (ULONG) floor( sector) + NUMFRAMENUMS-7;
    remainder = -fmod( Angle, PI/8.0);
    ulNewFrame = ulFrameNums[ulBaseFrame+1] -
                 ((ulFrameNums[ulBaseFrame+1] -
                   ulFrameNums[ulBaseFrame]) * remainder * 8.0 / PI);
    }
  return ulNewFrame;
  }                                     /* end: CalcFrame                    */

BOOL CrossEyed( PPOINTL pptlMouse, LONG lWidth)
  {
  double distance;

  distance = sqrt( (double)((pptlMouse->x * pptlMouse->x) +
                            (pptlMouse->y * pptlMouse->y)));
  if (distance < lWidth * EyeRatio)
    return TRUE;
  else
    return FALSE;
  }                                     /* end: CrossEyed                    */

/*************************************************************************
 * Name         :  SendString
 *
 * Description  :  This procedure will send string to MCI.
 *
 * Concepts     :
 *
 * MMPM/2 API's :  mciSendString
 *
 * Parameters   :  hwnd        - window handle.
 *                 pcMCIString - string command.
 *                 usUserParm  - user parameter.
 *
 * Return       :  TRUE  - if the operation was initiated without error.
 *                 FALSE - if an error occurred.
 *
 ******************************************************************************/
BOOL  SendString( HWND hwnd, PCHAR pcMCIString, USHORT usUserParm )
{
   LONG           lmciSendStringRC;    /* return value fromm mciSendString     */


// printf( "Executing:   %s ....\n", (PCHAR)pcMCIString);
   lmciSendStringRC =
       mciSendString( (PSZ)pcMCIString,
                      (PSZ)szReturn,
                      (USHORT)CCHMAXPATH,
                      (HWND)hwnd,
                      (USHORT)usUserParm );
// printf( "rc=%d\n", lmciSendStringRC);

   if (lmciSendStringRC != 0)
   {
      ShowMCIErrorMessage(lmciSendStringRC, hwnd, (PSZ)pcMCIString);
      return( FALSE );
   }

   return( TRUE );
}

/******************************************************************************
 * Name         :  ShowMCIErrorMessage
 *
 * Description  :  This window procedure displays an MCI error message
 *                 based upon a ulError return code.  The MCI function
 *                 mciGetErrorString is used to convert the error code into
 *                 a text string and the title is pulled from the resource
 *                 based upon a string id.
 *
 * Concepts     :  Using mciGetErrorString to convert an error code into
 *                 a textual message.
 *
 * MMPM/2 API's :  mciGetErrorString
 *
 * Parameters   :  ulError  -  MCI error code.
 *
 * Return       :  nothing
 *
 ******************************************************************************/
VOID  ShowMCIErrorMessage( ULONG ulError, HWND hwnd, PSZ pszCommand)
{
   CHAR  achBuffer[CCHMAXPATH];

   switch(mciGetErrorString( ulError, (PSZ)achBuffer,   sizeof( achBuffer)))
   {
      case MCIERR_SUCCESS:
         /*
          * This is what we want.  We were able to use mciGetErrorString to
          * retrieve a textual error message we can show in a message box.
          */
         WinMessageBox( HWND_DESKTOP,
                        hwnd,
                        (PSZ)achBuffer,
                        (PSZ)pszCommand,
                        0,
                        MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;

      case MCIERR_INVALID_DEVICE_ID:
      case MCIERR_OUTOFRANGE:
      case MCIERR_INVALID_BUFFER:
      default:
         WinMessageBox( HWND_DESKTOP,
                        hwnd,
                        (PSZ)"General MCI Error",
                        (PSZ)pszCommand,
                        0,
                        MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;
   }

   return;

}  /* end of ShowMCIErrorMessage */

/*************************************************************************
 * Name         :  CloseTheDevice
 *
 * Description  :  This procedure will close the Movie device.
 *
 * Concepts     :  Closing a device using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - close
 *
 * Parameters   :  None.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID CloseTheDevice( VOID)
{
  BOOL          fSuccess;

   /*
    * To stop the device , we will issue a string command using mciSendString.
    * This stop command is done for the alias.
    */
   fSuccess = SendString((HWND)NULL, "close movie", 0 );

   return;

}  /* end of CloseTheDevice */

/*************************************************************************
 * Name         :  OpenTheDevice
 *
 * Description  :  This procedure will open the Movie device.
 *
 * Concepts     :  Opening a device using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - open
 *
 * Parameters   :  None.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
BOOL OpenTheDevice( HWND hwnd)
{
  /******************************************************************/
  /* To open the device, we will issue MCI_OPEN command to the MCI  */
  /* for digital video.                                             */
  /******************************************************************/
  if ( SendString( hwnd,
                   "open digitalvideo alias movie wait shareable",
                   0 ) )
  {
     /* Open success, set the flag and return true */
     return(TRUE);
  }
  else
     return( FALSE );
}  /* end of OpenTheDevice */

BOOL LoadTheFile( HWND hwnd )
{
   LONG    lmciSendStringRC;          /* return code from SendString         */

   CHAR    szx[5]       = "";         /* string used for x position of window*/
   CHAR    szy[5]       = "";         /* string used for y position of window*/
   CHAR    szcx[5]      = "";        /* string used for cx position of window*/
   CHAR    szcy[5]      = "";        /* string used for cy position of window*/


   /*******************************************************************/
   /* The szWindowString and szPutString are used as a foundation     */
   /* for building a string command to send to sendstring             */
   /*******************************************************************/

   CHAR    szPutString[CCHMAXPATH] =
            "put movie destination at ";  /* string command to mciSendString */

   CHAR    szLoadString[CCHMAXPATH] =
            "load movie ";                /* string command to mciSendString */

   /**********************************************************************/
   /* Change pointer to a waiting pointer first, since this might take a */
   /* couple of seconds.                                                 */
   /**********************************************************************/

   StartWait( hwnd );

   lmciSendStringRC = SetWindowHandle( hwnd);

   /* Load the movie */
   strcat( szLoadString, (PCHAR)pszMovieFile );
   strcat (szLoadString, " ");
   strcat (szLoadString, "wait");
   if (!(lmciSendStringRC = SendString(hwnd, szLoadString, 0)))
     {
       ShowMCIErrorMessage(lmciSendStringRC, hwnd, (PSZ)szLoadString);
       return( FALSE );
     }

   /******************************************************/
   /* Convert the Frame windows sizes to strings so      */
   /* we can use the mciSendStringCommand to put the     */
   /* video in our application Frame window.             */
   /******************************************************/
   if (SendString( hwnd, (PCHAR)"status movie horizontal video extent wait", 0))
     ulVideoX = atoi( (PCHAR) szReturn);
   if (SendString( hwnd, (PCHAR)"status movie vertical video extent wait", 0))
     ulVideoY = atoi( (PCHAR) szReturn);
   WinSetWindowPos( hwnd, 0, 0, 0, ulVideoX, ulVideoY, SWP_SIZE);

   /******************************************************************/
   /* Now that we're done here, change the pointer back to the arrow.*/
   /******************************************************************/
   StopWait( hwnd );

   return( TRUE );

}  /* end of LoadTheFile */

LONG SetWindowHandle( HWND hwnd)
  {
  LONG    lmciSendStringRC;          /* return code from SendString         */
  CHAR    szHandle[20] = "";         /* string used for window handle       */
  CHAR    szWindowString[CCHMAXPATH] =
            "window movie handle ";      /* string command to mciSendString  */

   /******************************************************/
   /* Convert the Frame window handle to a string so    */
   /* we can use the mciSendStringCommand.               */
   /******************************************************/

   _ultoa(hwnd,szHandle,10);

   strcat (szWindowString, szHandle);  /* concatenate the converted handle to*/
   strcat (szWindowString, " ");     /* the window string so we can issue the*/
   strcat (szWindowString, "wait");  /* send string command                  */

   lmciSendStringRC = SendString(hwnd, szWindowString, 0);

   return( lmciSendStringRC );
  }                                     /* end: SetWindowHandle              */


/*****************************************************************
 * Name : Startwait
 *
 * Function: Change mouse pointer to hourglass
 *
 ******************************************************************/
VOID StartWait(HWND hwnd)
{
   HPOINTER hptrWait ;
   POINTL ptlPtr;

   vhCurrentPtr = WinQueryPointer(HWND_DESKTOP) ;

   /* If there is mouse pointer,                                */
   if (vhCurrentPtr)
   {

      /* Turn the pointer to an hourglass */
      hptrWait = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
      /* Set new mouse pointer                                  */
      WinSetPointer(HWND_DESKTOP,hptrWait) ;

      /* Force the pointer NOW to change */
      WinQueryPointerPos(HWND_DESKTOP, &ptlPtr);
      WinPostMsg( hwnd, WM_MOUSEMOVE,
                  MPFROM2SHORT(ptlPtr.x, ptlPtr.y),
                  MPFROM2SHORT(HT_NORMAL, KC_NONE));

   }

}  /* End StartWait() */


/*****************************************************************
 * Name : StopWait
 *
 * Function: Change mouse pointer from hourglass back to arror
 *
 ******************************************************************/
VOID StopWait(HWND hwnd)
{

   POINTL ptlPtr;

   /* If there is mouse pointer,                                */
   if (vhCurrentPtr)
   {

      /* Set new mouse pointer back to the original             */
      WinSetPointer(HWND_DESKTOP,vhCurrentPtr) ;

      /* Force the pointer NOW to change */
      WinQueryPointerPos(HWND_DESKTOP, &ptlPtr);
      WinPostMsg( hwnd, WM_MOUSEMOVE,
                  MPFROM2SHORT(ptlPtr.x, ptlPtr.y),
                  MPFROM2SHORT(HT_NORMAL, KC_NONE));
   }

}  /* End StopWait() */

/*****************************************************************
 * Name : QueryDirectMode
 *
 * Function: Determine if direct mode is supported.
 *
 ******************************************************************/

BOOL  QueryDirectMode( VOID )
{
    DIVE_CAPS    DiveCaps;             /* Dive Query Capabilities */
    ULONG        rc;                   /* API Return Code         */
    FOURCC       fccFormats[100] = {0};



    memset (&DiveCaps,0,sizeof(DiveCaps));
    DiveCaps.pFormatData = fccFormats;
    DiveCaps.ulFormatLength = 120;
    DiveCaps.ulStructLen = sizeof(DIVE_CAPS);
    rc = DiveQueryCaps( &DiveCaps, DIVE_BUFFER_SCREEN);
    if (!rc)
      {
        if (DiveCaps.fScreenDirect)
         {
           if (DiveCaps.ulDepth < 8)
              return (BOOL)(FALSE);
         }
         else
              return (BOOL)(FALSE);

      }
    return (BOOL)(TRUE);
}

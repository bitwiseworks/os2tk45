/*static char *SCCSID = "@(#)wndproc.c	6.11 92/03/10";*/

/*=========================================================================\
 *                                                                         *
 *       FILE:wndproc.c                                                    *
 *                                                                         *
 *       DESCRIPTION:    Window procedure for clock client window          *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1989-1993 IBM Corp.                                      *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation. This sample code is not    *
 *      part of any standard or IBM product and is provided to you solely  *
 *      for  the purpose of assisting you in the development of your       *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  IBM shall not be liable for any damages     *
 *      arising out of your use of the sample code, even if they have been *
 *      advised of the possibility of   such damages.                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_DEV
#define INCL_WININPUT
#define INCL_WINFRAMEMGR
#define INCL_WINTRACKRECT
#define INCL_WINMENUS
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINMESSAGEMGR
#define INCL_WINSHELLDATA
#define INCL_WINWINDOWMGR
#define INCL_WINHELP
#define INCL_GPICONTROL
#define INCL_GPIPRIMITIVES
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIBITMAPS
#define INCL_GPITRANSFORMS
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME


#include <os2.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "res.h"
#include "clkdata.h"
#include "help.h"

/*--------------------------------------------------------------*\
 *  Global variables and definitions for this file
\*--------------------------------------------------------------*/

HWND hwndHelpInstance;

/* Fields marked (RT) are filled in at run-time */

#define RESOURCEFLAG    0xFFFF0000

CHAR szHelpTitle [MAXTITLELENGTH + 1];


/*d-------------------------------------------------------------*\
 *  Entry point declarations
\*-------------------------------------------------------------k*/

MRESULT EXPENTRY ClkTicksDlgProc (HWND hWnd,ULONG usMessage,
                                  MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ClkColorsDlgProc(HWND hWnd,ULONG usMessage,
                                  MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY TimeDlgProc(HWND hWnd,ULONG usMessage,
                             MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY AlarmDlgProc (HWND hWnd,ULONG usMessage,
                               MPARAM mp1, MPARAM mp2);
VOID FAR SaveApplication (HWND hWnd);

VOID ClkPaint (HWND);
VOID ClkCreate (HWND);
VOID ClkDestroy (HWND);
VOID ClkMinmax (HWND, PSWP);
VOID ClkSize (HWND);
VOID ClkTimer (HWND);
VOID ClkCommand (HWND, MPARAM,MPARAM );
VOID GetArrangedDate (CHAR []);
VOID ClkDrawDate (HPS, USHORT);

VOID ClkHideFrameControls (HWND);
VOID ClkShowFrameControls (HWND);
VOID DrawDigitalTime (HWND);
VOID GetCountryDependent (VOID);
VOID SetDispMode (VOID);
VOID SetRGBColors (VOID);
VOID ShadeLight   (PLONG );
BYTE LightRGBByte (BYTE);
BYTE ShadeRGBByte (BYTE);
VOID InitMenu(MPARAM mp1, MPARAM mp2);
VOID EnableMenuItem(HWND hwndMenu, SHORT idItem, BOOL fEnable);
/****************************************************************\
 *  Routine Name:ClkWndProc()
 *--------------------------------------------------------------
 *
 *  Name:
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns: MRESULT()
 *
 *
\****************************************************************/

MRESULT EXPENTRY ClkWndProc (HWND hwnd, ULONG usMsg, MPARAM mp1, MPARAM mp2)
{
    switch (usMsg)
    {

    case WM_CREATE:
        ClkCreate (hwnd);
        return (WinDefWindowProc (hwnd, usMsg, mp1, mp2));

    case WM_DESTROY:
        ClkDestroy (hwnd);
        return (WinDefWindowProc (hwnd, usMsg, mp1, mp2));

    case WM_TIMER:
        ClkTimer (hwnd);
        if ( (BOOL) hDateTime)
            WinSendMsg (hDateTime, WM_TIMER, mp1, mp2);
        break;

    case WM_PAINT:
        ClkPaint (hwnd);
        break;

    case WM_MINMAXFRAME:
        ClkMinmax (hwnd, (PSWP)mp1);
        break;

    case WM_SIZE:
        ClkSize (hwnd);
        return (WinDefWindowProc (hwnd, usMsg, mp1, mp2));

    case WM_COMMAND:
        ClkCommand (hwnd, mp1,mp2);
        break;

    case WM_BUTTON1DOWN:
        return WinSendMsg (hwndFrame, WM_TRACKFRAME,
                           MPFROMSHORT((USHORT)mp2 | TF_MOVE), MPVOID);

    case WM_BUTTON1DBLCLK:
        if (cp.fControlsHidden)
            ClkShowFrameControls (hwndFrame);
        else
            ClkHideFrameControls (hwndFrame);
        break;

    case WM_TRANSLATEACCEL:
        return WinSendMsg(hwndFrame, usMsg, mp1, mp2);
        break;

    case WM_SAVEAPPLICATION:
        /***********************************************************\
        *  Task Manager option to Save Desktop sends this message.
        *  Save window state (MIN/RESTORE) and position in OS2.INI
        \***********************************************************/
        SaveApplication (hwndFrame);
        break;

     case WM_INITMENU:
         InitMenu(mp1, mp2);
         break;

    case HM_QUERY_KEYS_HELP:
        return (MRESULT)PANEL_HELPKEYS;   /* return id of key help panel */
        break;

    default:
        /* let default window procedure handle it. */
        return (WinDefWindowProc (hwnd, usMsg, mp1, mp2));
    }

    return (MRFROMLONG(0));
}



/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkCreate()
 *
 *  Purpose:Intialize a newly created client window
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/
VOID ClkCreate ( HWND hwnd )
{
    LONG cxScreen , cyScreen;  /* screen dimensions */
    LONG xLeft , yBottom ;      /* frame window location */
    ULONG cbBuf;
    LONG cyHeight;
    LONG cxWidth;

    hwndClient = hwnd;

    WinLoadString(hab, NULLHANDLE, IDS_TITLE,  0, (PSZ)szTitle   );
    WinLoadString(hab, NULLHANDLE, IDS_HELPTITLE, 256, (PSZ)szHelpTitle);
    GetCountryDependent();
    /* we are called before the global hwndFrame is valid */
    hwndFrame = WinQueryWindow ( hwnd , QW_PARENT) ;
    hwndTitleBar = WinWindowFromID ( hwndFrame , FID_TITLEBAR ) ;
    hwndSysMenu = WinWindowFromID ( hwndFrame , FID_SYSMENU ) ;
    hwndMinMax = WinWindowFromID ( hwndFrame , FID_MINMAX ) ;

    /* load our menus */
    hwndMenu = WinLoadMenu (hwndFrame, NULLHANDLE, IDR_MAIN);
    /* determine screen dimensions */
    /* open a device context and create a presentation space */

    hdc = WinOpenWindowDC (hwnd);
    hps = GpiCreatePS (hab, hdc, &sizl, PU_ARBITRARY | GPIT_MICRO |
            GPIA_ASSOC);

    /*
     * Create our off-screen 'buffer'.
     */
    hdcBuffer = DevOpenDC ( (HAB)0L, OD_MEMORY, "*", 0L, NULL, hdc);
    hpsBuffer = GpiCreatePS (hab, hdcBuffer, &sizl, PU_ARBITRARY |
                               GPIT_MICRO | GPIA_ASSOC);

    GpiCreateLogColorTable (hpsBuffer, 0, LCOLF_RGB, 0, 0, (PLONG)NULL);

    /* get the device resolutions so we can make the face appear circular */
    DevQueryCaps (hdc, (LONG)CAPS_VERTICAL_RESOLUTION,(LONG) 1L, &cyRes);
    DevQueryCaps (hdc, CAPS_HORIZONTAL_RESOLUTION, 1L, &cxRes);
    DevQueryCaps (hdc, CAPS_COLOR_PLANES, 1L, &cColorPlanes);
    DevQueryCaps (hdc, CAPS_COLOR_BITCOUNT, 1L, &cColorBitcount);

    cxScreen = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
    cyScreen = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);

    /*
     * Calculate an initial window position and size.
     */
    xLeft = cxScreen / 8;
    yBottom = cyScreen / 2;
    cxWidth = cxScreen / 3;
    cyHeight = cyScreen / 2;
    WinSetWindowPos (hwndFrame, NULLHANDLE, xLeft, yBottom,
                       cxWidth, cyHeight,
                       SWP_SIZE | SWP_MOVE | SWP_ACTIVATE);

    cbBuf = sizeof(cp);
    if (!PrfQueryProfileData(HINI_USER, SZ_APPNAME, SZ_KEYNAME, &cp, &cbBuf))
    {
        cp.usMajorTickPref = CLKTM_ALWAYS;
        cp.usMinorTickPref = CLKTM_NOTICONIC;
        cp.clrBackground = 0x00008080;
        cp.clrFace = 0x00008080;
        cp.clrHourHand = RGB_RED;
        cp.clrMinuteHand = RGB_RED;
        cp.fControlsHidden = FALSE;
        cp.usDispMode = DM_TIME | DM_ANALOG | DM_SECONDHAND;
        cp.alarm.uchHour = 0;
        cp.alarm.uchMinutes = 0;
        cp.alarm.usMode = 0;
        SetRGBColors();
        /* position the window and make it visible */
        WinSetWindowPos( hwndFrame , NULLHANDLE ,
                           xLeft , yBottom ,
                           cxWidth , cyHeight ,
                           SWP_SIZE | SWP_MOVE | SWP_ACTIVATE);
        WinQueryWindowPos(hwndFrame, &cp.swp);
    }
    else
    { /*Protect against garbage swp*/

        cp.swp.hwnd = hwndFrame;
        cp.swp.hwndInsertBehind = HWND_TOP;

        cp.swp.fl = (cp.swp.fl & (SWP_MINIMIZE | SWP_MAXIMIZE)) | SWP_SIZE | SWP_MOVE | SWP_ACTIVATE;
        SetRGBColors();
        WinSetMultWindowPos(hab, &cp.swp, 1);
    }

    if (cp.fControlsHidden)
        ClkHideFrameControls (hwndFrame);

    /*
     * Check relevant items.
     */
    WinSendMsg( hwndMenu,
                MM_SETITEMATTR,
                MPFROM2SHORT( IDM_TIME, TRUE),
                MPFROM2SHORT( MIA_CHECKED,
                              ( (cp.usDispMode & DM_TIME)?  MIA_CHECKED
                                      : ~MIA_CHECKED) ) );
    WinSendMsg( hwndMenu,
                MM_SETITEMATTR,
                MPFROM2SHORT( IDM_DATE, TRUE),
                MPFROM2SHORT( MIA_DISABLED,
                              ( (!(cp.usDispMode & DM_TIME))?  MIA_DISABLED
                                      : ~MIA_DISABLED) ) );
    WinSendMsg( hwndMenu,
                MM_SETITEMATTR,
                MPFROM2SHORT( IDM_DATE, TRUE),
                MPFROM2SHORT( MIA_CHECKED,
                              ( (cp.usDispMode & DM_DATE)?  MIA_CHECKED
                                      : ~MIA_CHECKED) ) );
    WinSendMsg( hwndMenu,
                MM_SETITEMATTR,
                MPFROM2SHORT( IDM_TIME, TRUE),
                MPFROM2SHORT( MIA_DISABLED,
                              ( ((!(cp.usDispMode & DM_DATE) || (cp.usDispMode & DM_ANALOG)))?  MIA_DISABLED
                                      : ~MIA_DISABLED) ) );
    WinSendMsg( hwndMenu,
                MM_SETITEMATTR,
                MPFROM2SHORT( (cp.usDispMode & DM_DIGITAL)
                                                          ?IDM_DIGITAL
                                                          :IDM_ANALOG,
                               TRUE),
                MPFROM2SHORT( MIA_CHECKED,
                              MIA_CHECKED));

    WinSendMsg(hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(IDM_SECONDHAND, TRUE),
                MPFROM2SHORT( MIA_CHECKED,
                              ( (cp.usDispMode & DM_SECONDHAND) ? MIA_CHECKED
                                      : ~MIA_CHECKED)));


    /*
     * Disable these items if the digital clock is visible
     * since they won't apply.
     */
    if (cp.usDispMode & DM_DIGITAL)
    {
        WinSendMsg (hwndMenu, MM_SETITEMATTR,
                MPFROM2SHORT(IDM_SECONDHAND, TRUE),
                MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));

        WinSendMsg (hwndMenu, MM_SETITEMATTR,
                MPFROM2SHORT( IDM_TICKS, TRUE), MPFROM2SHORT( MIA_DISABLED,
                MIA_DISABLED));
    }

    /* have we been asked to start as an icon? */
    if (fStartAsIcon)
        WinSetWindowPos(hwndFrame, NULLHANDLE, 0, 0, 0, 0, SWP_MINIMIZE);

    WinShowWindow(hwndFrame, TRUE);

    /* get the time in a format for dislaying */
    DosGetDateTime(&dt);
    dt.hours = (UCHAR )(dt.hours * (UCHAR) 5) % (UCHAR) 60 + dt.minutes / (UCHAR)12;

    /* start a timer */
    WinStartTimer (hab, hwnd, IDR_MAIN, 1000);

    WinLoadString(hab, NULLHANDLE, IDS_TITLE, 80, (PSZ)szTitle);
    GetCountryDependent();

}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkDestroy()
 *
 *  Purpose: Destroy clock face.
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:VOID
 *
 *
\****************************************************************/
VOID ClkDestroy (HWND hwnd)
{
    HBITMAP hbm;

    hbm = GpiSetBitmap (hpsBuffer, NULLHANDLE);

    if (hbm != NULLHANDLE)
        GpiDeleteBitmap (hbm);

    GpiDestroyPS (hpsBuffer);
    DevCloseDC (hdcBuffer);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: ClkSize()
 *
 *  Purpose:When the window has been sized, we calculate  a page
 *          rectangle which: (a) fills the window rectangle in either
 *          the x or y dimension, (b) appears square, and (c) is centered
 *          in the window rectangle
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *
 *
\****************************************************************/
VOID ClkSize (HWND hwnd)
{
    RECTL rclWindow;
    SIZEF sizef;
    LONG cxSquare, cySquare, cxEdge, cyEdge;
    LONG cyHeight;
    LONG cxWidth;
    HBITMAP hbm;
    BITMAPINFOHEADER bmp;

    /*
     * First get rid of any buffer bitmap already there.
     */
    hbm = GpiSetBitmap (hpsBuffer, NULLHANDLE);

    if (hbm != NULLHANDLE)
        GpiDeleteBitmap (hbm);

    /*
     * Get the width and height of the window rectangle.
     */
    WinQueryWindowRect (hwnd, &rclWindow);
    cxWidth = rclWindow.xRight - rclWindow.xLeft - 2;
    cyHeight = rclWindow.yTop - rclWindow.yBottom - 2;

    /*
     * Now create a bitmap the size of the window.
     */
    bmp.cbFix = sizeof(BITMAPINFOHEADER);
    bmp.cx = (SHORT)cxWidth;
    bmp.cy = (SHORT)cyHeight;
    bmp.cPlanes = (SHORT)cColorPlanes;
    bmp.cBitCount = (SHORT)cColorBitcount;
    hbm = GpiCreateBitmap(hpsBuffer, (PBITMAPINFOHEADER2)&bmp,
                          0x0000, (PBYTE)NULL, (PBITMAPINFO2)NULL);
    GpiSetBitmap (hpsBuffer, hbm);

    /*
     * Assume the size of the page rectangle is constrained in the y
     * dimension,compute the x size which would make the rectangle appear
     * square, then check the assumption and do the reverse calculation
     * if necessary.
     */
    cySquare = cyHeight - 2;
    cxSquare = ( cyHeight * cxRes ) / cyRes;

    if (cxWidth < cxSquare)
    {
        cxSquare = cxWidth - 2;
        cySquare = (cxWidth * cyRes) / cxRes;
    }

    /*
     * Fill in the page rectangle and set the page viewport.
     */
    cxEdge = (cxWidth - cxSquare ) / 2;
    cyEdge = (cyHeight - cySquare ) / 2;
    rclPage.xLeft = cxEdge;
    rclPage.xRight = cxWidth - cxEdge;
    rclPage.yBottom = cyEdge;
    rclPage.yTop = cyHeight - cyEdge;

    /*
     * Determine where to put the date. If we have room under the clock, we
     * put it there. If we have more room on the left we put it there. If we
     * have more room in the midlle, it goes there.
     */
    if (cp.usDispMode & DM_DATE)
    {
        vclrDate[SURFACE] = vclrBG[SHADE];
        vclrDate[LIGHT] = vclrBG[LIGHT];
        vclrDate[SHADE] = vclrBG[SHADE];
        vclrDate[BACKGROUND] = vclrBG[SURFACE];
        if (cyHeight > (cySquare*6/5)) { /*Goes under*/
            vmatlfDateTrans.lM31 = (LONG)15;            /*Horizontal*/
            vmatlfDateTrans.lM32 = -(LONG)17;    /*vertical*/
            vmatlfDateScale.fxM11 = MAKEFIXED(2,0x8000);
            vmatlfDateScale.fxM22 = MAKEFIXED(2,0x8000);
            rclPage. yTop += cyEdge;
            rclPage. yBottom += cyEdge;
            vusDatePos = DP_UNDER;
        }
        else
        {
            if (cxWidth > (cxSquare * 31/10 ))
            {
                vmatlfDateTrans.lM31 = -(LONG)53 ;            /*Horizontal*/
                vmatlfDateTrans.lM32 = (LONG)04;    /*vertical*/
                vmatlfDateScale.fxM11 = MAKEFIXED(7,0xe000);
                vmatlfDateScale.fxM22 = MAKEFIXED(7,0xe000);
                rclPage.xRight += cxEdge;
                rclPage.xLeft += cxEdge;
                vusDatePos = DP_LEFTMIDDLE;

            }
            else
            {
                if (cxWidth > (cxSquare * 2)) { /*Goes on the left*/
                    vmatlfDateTrans.lM31 = -(LONG)52 ;            /*Horizontal*/
                    vmatlfDateTrans.lM32 = (LONG)2;    /*vertical*/
                    vmatlfDateScale.fxM11 = MAKEFIXED(3,0xd000);
                    vmatlfDateScale.fxM22 = MAKEFIXED(3,0xd000);
                    rclPage.xRight += cxEdge;
                    rclPage.xLeft += cxEdge;
                    vusDatePos = DP_LEFTDOWN  ;

                }
                else
                {   /*Goes inside*/
                    vmatlfDateTrans.lM31 = (LONG)24 ;            /*Horizontal*/
                    vmatlfDateTrans.lM32 = (LONG)23;    /*vertical*/
                    vmatlfDateScale.fxM11 = MAKEFIXED(2,0);
                    vmatlfDateScale.fxM22 = MAKEFIXED(2,0);
                    vclrDate[SURFACE] = vclrFace[SHADE] ;
                    vclrDate[LIGHT] = vclrFace[LIGHT] ;
                    vclrDate[SHADE] = vclrFace[SHADE];
                    vclrDate[BACKGROUND] = vclrFace[SURFACE];
                    vusDatePos = DP_INSIDE;
                }
            }
        }
    }

    f = GpiSetPageViewport (hps, &rclPage);
    f = GpiSetPageViewport (hpsBuffer, &rclPage);

    /*
     * Are we iconic?
     */
    f = WinQueryWindowPos (hwndFrame, &swp);
    fIconic = (BOOL)(swp.fl & SWP_MINIMIZE );
    fShowSecondHand = (BOOL) !(fIconic);

    GpiQueryCharBox(hpsBuffer, &sizef);
    GpiSetCharBox(hpsBuffer, &sizef);

    fBufferDirty = TRUE;
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkMinMax()
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          VOID
 *
\****************************************************************/
VOID ClkMinmax (HWND hwnd, PSWP pswp)
{
    char achFinalDate[10];
    HWND hwndFrame;


    /*
     * We want the date as the icon text if the clock is
     * minimized and the user has selected both date and
     * time be displayed.
     */
    if ((cp.usDispMode & (DM_TIME | DM_DATE)) == (DM_TIME | DM_DATE))
    {
        hwndFrame = WinQueryWindow (hwnd, QW_PARENT);
        if (pswp->fl & SWP_MINIMIZE) {
            GetArrangedDate (achFinalDate);
            WinSetWindowText (hwndFrame, achFinalDate);
        } else {
            WinSetWindowText (hwndFrame, "Clock");
        }
    }
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkTimer()
 *
 *  Purpose: Handles window timer events
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/
VOID ClkTimer (HWND hwnd)
{
    DATETIME  dtNew;
    static LONG lTimeChangeCheck = 0L;
    LONG  lTime;
    DosGetDateTime ( &dtNew ) ;

    if (cp.usDispMode & DM_ANALOG)
    {

        /* get the new time */
        DosGetDateTime (&dtNew);

        /* adjust the hour hand */
        dtNew.hours = (dtNew.hours * (UCHAR) 5 ) % (UCHAR) 60
                      + dtNew.minutes / (UCHAR)12;

        /* if we must move the hour and minute hands, redraw it all */
        if (dtNew.minutes != dt.minutes)
        {

            ClkDrawFace(hpsBuffer);
            ClkDrawDate(hpsBuffer, DM_REDRAW);
            ClkDrawHand(hpsBuffer, HT_HOUR_SHADE, dtNew.hours);
            ClkDrawHand(hpsBuffer, HT_MINUTE_SHADE, dtNew.minutes);
            ClkDrawHand(hpsBuffer, HT_HOUR, dtNew.hours);
            ClkDrawHand(hpsBuffer, HT_MINUTE, dtNew.minutes);

            UpdateScreen (hps, NULL);

            if (fShowSecondHand && (cp.usDispMode & DM_SECONDHAND))
            {
                GpiSetMix(hps, FM_INVERT);
                ClkDrawHand(hps, HT_SECOND, dtNew.seconds);
            }
        }

        /* otherwise just undraw the old second hand and draw the new */
        else if (fShowSecondHand  && (cp.usDispMode & DM_SECONDHAND))
        {
            GpiSetMix(hps, FM_INVERT);
            ClkDrawHand(hps, HT_SECOND, dt.seconds);
            ClkDrawHand(hps, HT_SECOND, dtNew.seconds);
        }

        dt = dtNew ;
    }
    else /*Must be Digital*/
    {
        DrawDigitalTime(hwnd);
    }

    /*
     *if this is the first
     *time through init it
     */
    if(!lTimeChangeCheck)
    {
          time(&lTimeChangeCheck);

    }

    /*
     *a quick check to see if
     *any other session
     *adjusted our time
     *if so reset the timer
     */
    else
    {
          time(&lTime);
          /*
           *did the clock get changed by more than 60 seconds
           */
          if( lTime  > ( lTimeChangeCheck  + 60L ) ||
              lTimeChangeCheck > ( lTime   + 60L ) )
          {

               AlarmSetTimer(cp.alarm.uchHour,
                                   cp.alarm.uchMinutes);
          }
          lTimeChangeCheck = lTime;
     }

}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:DrawDigitalTime()
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/

VOID DrawDigitalTime(HWND hwnd)
{
    RECTL rcl;
    RECTL rclChar;
    RECTL rclCharOld;
    RECTL rclTime;
    RECTL rclAmPm;
    RECTL rclDate;
    HPS hpsWnd;
    SIZEF sizef;
    USHORT usi;
    ULONG  ulCharWidth,ulCharModulu;
    char achTime[9];
    char achFinalDate[9];
    time_t     tTime;
    struct tm  *pLocalTime;

    WinQueryWindowRect (hwnd, &rcl);
    hpsWnd = WinGetPS (hwnd);

    memset(achTime,0,sizeof(achTime) );
    memset(achFinalDate,0,sizeof(achFinalDate) );

    GpiCreateLogColorTable (hpsWnd, LCOL_RESET, LCOLF_RGB, 0, 0,
                                 (PLONG) NULL);

    /*
     *if black hands and black background
     *selected force the background to
     *blue
     */

    if( !cp.clrMinuteHand && !cp.clrBackground )
    {
          cp.clrBackground = RGB_BLUE;
    }



    switch (cp.usDispMode & (DM_TIME | DM_DATE))
    {
    case DM_DATE:
        rclDate = rcl;
        break;

    case DM_TIME | DM_DATE:
        if (!fIconic)
        {
            rclTime = rclDate = rcl;
            rclTime.yBottom = rclDate.yTop = rcl.yTop / 2;
            break;
        } /*else fall through*/

    case DM_TIME:
        rclTime = rcl;
        break;
    }

    if (cp.usDispMode & DM_TIME)
    {

        rclAmPm = rclTime;
        time(&tTime);
        pLocalTime = localtime(&tTime);
        if (bTwelveHourFormat)
        {
           strftime(achTime, sizeof(achTime), "%I %M %S", pLocalTime);
        }
        else
        {
           strftime(achTime, sizeof(achTime), "%H %M %S", pLocalTime);
        }

        /*insert country time separator*/
        achTime[2] = achTime[5] = szTimeSep[0];
        achTime[8] = '\000';
        /*Process 12 hours mode*/
        if (bTwelveHourFormat)
        {

            if (fIconic)
            {
                rclTime.yBottom = rclAmPm.yTop = rclTime.yTop / 2;
            }
            else
            {
                rclTime.xRight = rcl.xRight * 8 / 11;
                rclAmPm.xLeft = rclTime.xRight;
            }

            strcpy(achAmPm,szAnteMeridian);

            if (pLocalTime->tm_hour >= 12)
            {
                strcpy(achAmPm,szPostMeridian);
            }

        }

        GpiSetCharMode(hpsWnd, CM_MODE3);

        if (fIconic)
        {
            sizef.cx = MAKEFIXED(LOUSHORT((rclTime.xRight - rclTime.xLeft)
                    / 3), 0);
            sizef.cy = MAKEFIXED(LOUSHORT((rclTime.yTop - rclTime.yBottom)
                    * 1400 / 1000), 0);
        }
        else
        {
            sizef.cx = MAKEFIXED(LOUSHORT((rclTime.xRight - rclTime.xLeft)
                    / 6), 8000);
            sizef.cy = MAKEFIXED(LOUSHORT((rclTime.yTop - rclTime.yBottom)
                    * 1000 / 1500), 0);
        }

        GpiSetCharBox(hpsWnd, &sizef);

        if (bTwelveHourFormat)
        {

            if( strcmp(achAmPm,achOldAmPm) )
            {
                WinFillRect(hpsWnd, &rclAmPm, cp.clrBackground);
                WinDrawText(hpsWnd, sizeof(achAmPm) - 1, (PSZ)achAmPm,
                        (PRECTL)&rclAmPm, cp.clrMinuteHand, cp.clrBackground,
                        DT_CENTER | DT_VCENTER );
            }
        }

        if (!fIconic)
        {

            WinDrawText(hpsWnd, sizeof(achTime) - 1 , (PSZ)achTime,
                    (PRECTL)&rclTime, cp.clrMinuteHand, cp.clrBackground,
                    DT_CENTER | DT_VCENTER | DT_QUERYEXTENT);

            ulCharWidth = (rclTime.xRight - rclTime.xLeft) / (sizeof(achTime) - 1 );
            ulCharModulu = (rclTime.xRight - rclTime.xLeft) % (sizeof(achTime) - 1 );
            rclCharOld.xRight = rclTime.xLeft;

            rclChar.yTop = rclTime.yTop;
            rclChar.yBottom = rclTime.yBottom;

            for (usi = 0; usi < (sizeof(achTime)); usi++)
            {
                rclChar.xLeft = rclCharOld.xRight + (ULONG)1;
                rclChar.xRight = rclChar.xLeft + ulCharWidth +
                        ((ulCharModulu > 0L) ? 1L : 0L);

                if (ulCharModulu)
                    ulCharModulu--;

                if (achTime[usi] == szTimeSep[0])
                {
                    rclChar.xRight -= 3;
                }
                else
                {
                    rclChar.xRight += 1;
                }

                rclCharOld = rclChar;

                if (achTime[usi] != achOldTime[usi])
                {
                    WinFillRect (hpsWnd, &rclChar, cp.clrBackground);
                    if (!((usi == 0) && (achTime[0] == '0') &&
                            (bTwelveHourFormat)))
                        WinDrawText (hpsWnd, 1, (PSZ)&achTime[usi], &rclChar,
                                cp.clrMinuteHand, cp.clrBackground,
                                DT_CENTER | DT_VCENTER);
                }
            }
        }
        else
        {   /*Iconic. just draw if minute changed*/

            if (strncmp(achTime,achOldTime,5))
            {
                WinFillRect(hpsWnd,&rclTime,cp.clrBackground);
                WinDrawText(hpsWnd, sizeof(achTime) - 4 , (PSZ)achTime,
                        (PRECTL)&rclTime, cp.clrMinuteHand, cp.clrBackground,
                        DT_CENTER | DT_VCENTER);
            }
        }
    }

    if ((!(cp.usDispMode & DM_TIME)) || ((cp.usDispMode & DM_DATE) &&
            (!fIconic)))
    {
        GetArrangedDate(achFinalDate);
        if (strncmp(achFinalDate, achOldDate,
                sizeof(achFinalDate) - fIconic ? 4 : 1))
        {

            WinFillRect (hpsWnd, &rclDate, cp.clrBackground);
            GpiSetCharMode (hpsWnd, CM_MODE3);

            sizef.cx = MAKEFIXED(LOUSHORT(
                            (rclDate.xRight - rclDate.xLeft) / 5), 0);
            sizef.cy = MAKEFIXED(LOUSHORT(
                            (rclDate.yTop - rclDate.yBottom) * 1000 / 1500), 0);

            GpiSetCharBox(hpsWnd, &sizef);
            WinDrawText(hpsWnd,
                        (sizeof(achFinalDate) - (fIconic ? 4 : 1) ),
                        (PSZ)achFinalDate,(PRECTL)&rclDate,
                        (LONG)cp.clrMinuteHand,
                        (LONG)cp.clrBackground,
                        (ULONG )(DT_CENTER | DT_VCENTER ) );
        }
    }

    WinReleasePS(hpsWnd);

    strncpy(achOldTime,achTime,sizeof(achOldTime));
    strncpy(achOldAmPm,achAmPm,sizeof(achOldAmPm));
    strncpy(achOldDate,achFinalDate,sizeof(achOldDate));
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkCommand()
 *
 *  Purpose:Handle  WM_COMMAND events.
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          VOID
 *
\****************************************************************/
VOID ClkCommand ( HWND hwnd , MPARAM mp1, MPARAM mp2 )
{
    DATETIME  dtNew ;

    switch ( SHORT1FROMMP ( mp1 ) )
    {

        case IDM_TICKS :
            WinDlgBox ( HWND_DESKTOP , hwndFrame , ClkTicksDlgProc ,
                        NULLHANDLE ,
                        IDD_TICKS , (PVOID )NULL ) ;
            break;

        case IDM_EXIT:
            /*
             *post the event semaphore
             *for our alarm thread
             */
            fEndThread = TRUE;
            DosPostEventSem(TimerResources.hTimerDev);
            WinPostMsg(hwnd, WM_QUIT,MPVOID, MPVOID);


            break;

        case IDM_COLORS :
            WinDlgBox ( HWND_DESKTOP , hwndFrame ,ClkColorsDlgProc ,
                        NULLHANDLE ,
                        IDD_COLORS , NULL ) ;
            SetRGBColors();
            ClkSize(hwnd);
            break ;

        case IDM_TOGGLECONTROLS :
            if (cp.fControlsHidden) {
                ClkShowFrameControls( hwndFrame );
            } else {
                ClkHideFrameControls ( hwndFrame ) ;
            }
            break ;

        case IDM_DATETIME:
            if (hDateTime == NULLHANDLE)
            {
                WinDlgBox(HWND_DESKTOP,hwndFrame,TimeDlgProc,
                          NULLHANDLE,IDD_TIME ,NULL);
                dt.minutes += 2; /*Fool the ClkTimer proc. It will now think it have
                                  to redraw*/
            } else {
                WinSetFocus (HWND_DESKTOP, hDateTime);
            }
            break;
        case IDM_ALARM:
            if (hAlarmTime == (HWND)NULL)
            {
                WinDlgBox(HWND_DESKTOP,hwndFrame,AlarmDlgProc,
                          NULLHANDLE,IDD_ALARM ,NULL);
            }
            else
            {
                WinSetFocus(HWND_DESKTOP,hAlarmTime);
            }

            break;

        case IDM_SECONDHAND:
            cp.usDispMode = ((cp.usDispMode & DM_SECONDHAND) ?
                    (cp.usDispMode & (~DM_SECONDHAND)) : (cp.usDispMode | DM_SECONDHAND));

            GpiSetMix(hps, FM_INVERT);

            /*
             * Depending on the current mode draw or remove the second hand.
             */
            if (cp.usDispMode & DM_SECONDHAND) {
                DosGetDateTime(&dtNew);
                ClkDrawHand(hps, HT_SECOND, dtNew.seconds);
                dt.seconds = dtNew.seconds;
            } else {
                ClkDrawHand(hps, HT_SECOND, dt.seconds);
            }

            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_SECONDHAND, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                      ( (cp.usDispMode & DM_SECONDHAND)?  MIA_CHECKED
                                              : ~MIA_CHECKED) ) );
            break;

        case IDM_TIME:
            cp.usDispMode = ((cp.usDispMode & DM_TIME) ? (cp.usDispMode & (~DM_TIME))
                                                      : (cp.usDispMode | DM_TIME));

            WinSendMsg(hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(IDM_TIME, TRUE),
                    MPFROM2SHORT(MIA_CHECKED, ((cp.usDispMode & DM_TIME) ?
                    MIA_CHECKED : ~MIA_CHECKED)));

            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_DATE, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                      ( (!(cp.usDispMode & DM_TIME)) ? MIA_DISABLED
                                              : ~MIA_DISABLED) ) );
            ClkSize(hwnd);
            WinInvalidateRect(hwnd, NULL, TRUE);
            break;

        case IDM_DATE:
            cp.usDispMode = ((cp.usDispMode & DM_DATE) ? (cp.usDispMode & (~DM_DATE))
                                                      : (cp.usDispMode | DM_DATE));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_DATE, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                      ( (cp.usDispMode & DM_DATE)?  MIA_CHECKED
                                              : ~MIA_CHECKED) ) );
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TIME, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                      ( ((!(cp.usDispMode & DM_DATE)) || (cp.usDispMode & DM_ANALOG))?  MIA_DISABLED
                                              : ~MIA_DISABLED) ) );
            ClkSize(hwnd);
            WinInvalidateRect(hwnd,(PRECTL)NULL,TRUE);

            break;
        case IDM_DIGITAL:
            cp.usDispMode |=  DM_DIGITAL;
            cp.usDispMode &=  (~DM_ANALOG);
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_DIGITAL, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                      MIA_CHECKED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_ANALOG , TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                     ~MIA_CHECKED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_SECONDHAND, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        MIA_DISABLED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TICKS, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        MIA_DISABLED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TIME, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                      ( ((!(cp.usDispMode & DM_DATE)) || (cp.usDispMode & DM_ANALOG))?  MIA_DISABLED
                                              : ~MIA_DISABLED) ) );
            WinInvalidateRect(hwnd,(PRECTL)NULL,TRUE);
            break;
        case IDM_ANALOG :
            cp.usDispMode |=  DM_ANALOG;
            cp.usDispMode &=  (~DM_DIGITAL);
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_ANALOG, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                      MIA_CHECKED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_DIGITAL, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                     ~MIA_CHECKED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TIME, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        MIA_DISABLED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_DATE, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        ~MIA_DISABLED));

            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_SECONDHAND, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        ~MIA_DISABLED));
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TICKS, TRUE),
                        MPFROM2SHORT( MIA_DISABLED,
                                        ~MIA_DISABLED));

            cp.usDispMode |= DM_TIME;
            WinSendMsg( hwndMenu,
                        MM_SETITEMATTR,
                        MPFROM2SHORT( IDM_TIME, TRUE),
                        MPFROM2SHORT( MIA_CHECKED,
                                      MIA_CHECKED));
            WinInvalidateRect(hwnd,(PRECTL)NULL,TRUE);
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

        case IDM_HELPINDEX:
            HelpIndex(mp2);
            break;

        case IDM_HELPTUTORIAL:
            HelpTutorial(mp2);
            break;

        case IDM_HELPABOUT:
            HelpAbout(mp2);
            break;

     case IDM_ALARM_EXPIRED:
           WinMessageBox(HWND_DESKTOP,
                         hwndFrame,
                         (PSZ)"Alarm Expired",
                         (PSZ)"The Bells Are Ringing",
                         0,
                         MB_OK);
            break;
    }
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkHideFrameControls
 *
 *  Purpose:Hide the title bar and associted controls
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *
 *
\****************************************************************/
VOID ClkHideFrameControls ( HWND hwndFrame )
{

    WinSetParent ( hwndTitleBar , HWND_OBJECT , FALSE ) ;
    WinSetParent ( hwndSysMenu , HWND_OBJECT , FALSE ) ;
    WinSetParent ( hwndMinMax , HWND_OBJECT , FALSE ) ;
    WinSetParent ( hwndMenu , HWND_OBJECT , FALSE ) ;

    WinSendMsg ( hwndFrame , WM_UPDATEFRAME ,
                 MPFROMLONG( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_MENU ) ,
                 MPVOID) ;

    cp.fControlsHidden = TRUE ;
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkShowFrameControls
 *
 *  Purpose:Show the title bar and associated contols
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *          VOID
 *
\****************************************************************/


VOID ClkShowFrameControls ( HWND hwndFrame )
{

    WinSetParent ( hwndTitleBar , hwndFrame , FALSE ) ;
    WinSetParent ( hwndSysMenu , hwndFrame , FALSE ) ;
    WinSetParent ( hwndMinMax , hwndFrame , FALSE ) ;
    WinSetParent ( hwndMenu , hwndFrame , FALSE ) ;

    WinSendMsg ( hwndFrame , WM_UPDATEFRAME ,
                MPFROMLONG( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_MENU ) ,
                MPVOID) ;
    WinInvalidateRect ( hwndFrame , NULL , TRUE ) ;

    cp.fControlsHidden = FALSE ;
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:GetArrangedDate()
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID GetArrangedDate(CHAR achFinalDate[])
{
    time_t     tTime;
    struct tm  *pLocalTime;

    time(&tTime);
    pLocalTime = localtime(&tTime);
    switch (vusDateFormat)
    {
        case 2: /*YMD*/
            if (fIconic)
            {
               strftime(achFinalDate, sizeof(achFinalDate), "%m %d %y", pLocalTime);
            }
            else
            {
               strftime(achFinalDate, sizeof(achFinalDate), "%y %m %d", pLocalTime);
            }
    /*put date separators*/
            achFinalDate[2] = achFinalDate[5] = szDateSep[0];
            break;

        case 1: /*DMY*/
            strftime(achFinalDate, sizeof(achFinalDate), "%d %m %y", pLocalTime);
            achFinalDate[2] = achFinalDate[5] = szDateSep[0];
            break;

        default:
            strftime(achFinalDate, 99, "%x", pLocalTime);
            break;
    }
    achFinalDate[8] = '\0';
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:SaveApplication()
 *
 *  Purpose:Handle the   WM_SAVEAPPLICATION message from the desktop
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns: VOID
 *
 *
\****************************************************************/
VOID FAR SaveApplication( HWND hwnd )
{

    WinQueryWindowPos(hwnd, (PSWP)&cp.swp);
    PrfWriteProfileData(HINI_USER, SZ_APPNAME, SZ_KEYNAME, &cp, sizeof(cp));
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: SetRGBColors
 *
 *  Purpose: Set the RGB schema so that each time a user changes the
 *           color we update it here.
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:VOID
 *
 *
\****************************************************************/
VOID SetRGBColors(VOID)
{
    vclrFace[SURFACE] = cp.clrFace;
    vclrBG[SURFACE] = cp.clrBackground;
    vclrHands[SURFACE] = cp.clrMinuteHand;
    vclrMajorTicks[SURFACE] = cp.clrHourHand;

    /*Fill color tables*/
    ShadeLight(vclrMajorTicks);

    vclrMinorTicks[SURFACE] = vclrFace[SURFACE];
    ShadeLight(vclrMinorTicks);

    ShadeLight(vclrFace);

    vclrRing[SURFACE] = vclrFace[SURFACE];
    ShadeLight(vclrRing);

    #ifdef DISABLE
    ShadeLight(vclrHands);
    #endif
    vclrHands[SHADE] = RGB_BLACK;

    ShadeLight(vclrBG);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ShadeLight()
 *
 *  Purpose: Find the shade and light color index values and
 *           install them in the colors   array of the element
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *
 *
\****************************************************************/
VOID ShadeLight(PLONG nplColors)
{
   typedef  union  _RGBLONG
   {
        RGB rgb;
        LONG lng;
   } RGBLONG ;
   RGBLONG  rgbSurface,rgbShade,rgbLight;

   rgbSurface.lng = rgbShade.lng = rgbLight.lng = 0L;

   #ifdef DISABLE
   rgbSurface.lng = GpiQueryRGBColor(hps,0L,);
   #endif
   rgbSurface.lng = nplColors[SURFACE];
   rgbShade.rgb.bBlue = ShadeRGBByte(rgbSurface.rgb.bBlue);
   rgbShade.rgb.bRed = ShadeRGBByte(rgbSurface.rgb.bRed);
   rgbShade.rgb.bGreen = ShadeRGBByte(rgbSurface.rgb.bGreen);
   rgbLight.rgb.bBlue = LightRGBByte(rgbSurface.rgb.bBlue);
   rgbLight.rgb.bRed = LightRGBByte(rgbSurface.rgb.bRed);
   rgbLight.rgb.bGreen = LightRGBByte(rgbSurface.rgb.bGreen);
   nplColors[SHADE] = rgbShade.lng;
   nplColors[LIGHT] = rgbLight.lng;
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ShadeRGBByte
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *
 *
\****************************************************************/

BYTE ShadeRGBByte(BYTE brgb)
{
  #define SHADER   ( (BYTE) 0x50)

  if (brgb > SHADER)
  {
     return (brgb - SHADER);
  }
  else
  {
     return (0);
  }

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:LightRGBByte
 *
 *  Purpose:
 *
 *
 *
 *  Usage:
 *
 *  Method:
 *          -
 *
 *          -
 *          -
 *
 *          -
 *          -
 *
 *  Returns:
 *
 *
\****************************************************************/
BYTE LightRGBByte(BYTE brgb)
{

  #define LIGHTER  ( (BYTE) 0x40)

  if (brgb < (0xff - LIGHTER) )
  {
     return (brgb + LIGHTER);
  }

  else return (0xff);

}



/****************************************************************\
 *  Menu item intialization routine
 *--------------------------------------------------------------
 *
 *  Name:   InitMenu(mp1, mp2)
 *
 *  Purpose: Processes the WM_INITMENU message for the main window,
 *              disabling any menus that are not active
 *
 *  Usage:  Routine is called each time a menu is dropped
 *
 *  Method: A switch statement branches control based upon
 *          the id of the menu that is being displayed
 *
 *  Returns:
\****************************************************************/
VOID InitMenu(MPARAM mp1, MPARAM mp2)
{

    /* define a shorthand way of denoting the menu handle */
    #define hwndMenu        HWNDFROMMP(mp2)

    switch(SHORT1FROMMP(mp1))
    {

        case IDM_HELP:
            /*
             * Enable or disable the Help menu depending upon whether the
             * help manager has been enabled
             */
            EnableMenuItem(hwndMenu, IDM_HELPHELPFORHELP, fHelpEnabled);
            EnableMenuItem(hwndMenu, IDM_HELPEXTENDED, fHelpEnabled);
            EnableMenuItem(hwndMenu, IDM_HELPKEYS, fHelpEnabled);
            EnableMenuItem(hwndMenu, IDM_HELPINDEX, fHelpEnabled);

            /** REMEMBER: add a case for IDM_HELPTUTORIAL if you include
                the Tutorial menu item   **/

            break;

        default:
            break;
    }

    #undef hwndMenu

}   /* InitMenu() */


/****************************************************************\
 *  Enables/Disables the menu item of the given menu
 *--------------------------------------------------------------
 *
 *  Name:   EnableMenuItem(hwndMenu, idItem, fEnable)
 *
 *  Purpose: Enables or disables the menu item
 *
 *  Usage:  Called whenever a menu item is to be enabled or
 *          disabled
 *
 *  Method: Sends a MM_SETITEMATTR to the menu with the
 *          given item id.  Sets the MIA_DISABLED attribute
 *          flag if the item is to be disabled, clears the flag
 *          if enabling
 *
 *  Returns:
 *
\****************************************************************/
VOID EnableMenuItem(HWND hwndMenu, SHORT idItem, BOOL fEnable)
{
    SHORT fsFlag;

    if(fEnable)
        fsFlag = 0;
    else
        fsFlag = MIA_DISABLED;

    WinSendMsg(hwndMenu,
               MM_SETITEMATTR,
               MPFROM2SHORT(idItem, TRUE),
               MPFROM2SHORT(MIA_DISABLED, fsFlag));

}   /* EnableMenuItem() */


/*--------------------------------------------------------------*\
 *  End of file :wndproc.c
\*--------------------------------------------------------------*/

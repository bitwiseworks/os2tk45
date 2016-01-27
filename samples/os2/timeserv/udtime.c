/*static char *SCCSID = "@(#)udtime.c   6.7 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE: udtime.c                                                    *
 *                                                                         *
 *       DESCRIPTION:  miscellaneous time and date functions               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1992     IBM Corp.                                       *
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
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *      TimeDlgProc
 *      GetControlCoordinates
 *      PanelDlgFilter
 *      ChangeDateTime
 *      ControlMinmax
 *      PanelPaint
 *      PaintDateTime
 *      GetDate
 *      GetTime
 *      PanelVScrolls
 *      TimeInit
 *      MouseTrack
 *      GetCountryDependent
 *      BringUpArrows
 *      FixDate
 *      SetDate
 *      SetTime
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_WINDIALOGS
#define INCL_WINBUTTONS
#define INCL_WINSYS
#define INCL_WINMESSAGEMGR
#define INCL_WINSHELLDATA
#define INCL_WINFRAMEMGR
#define INCL_WINPOINTERS
#define INCL_WINSWITCHLIST
#define INCL_WIN
#define INCL_DOSNLS
#define INCL_GPITRANSFORMS
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include "clock.h"
#include "res.h"
#include "clkdata.h"




/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/


SHORT DaysPerMonth[13] =
       {0,31,28,31,30,31,30,31,31,30,31,30,31};

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

MRESULT EXPENTRY TimeDlgProc(HWND, ULONG, MPARAM, MPARAM);
VOID    GetControlCoordinates( HWND, HWND, SHORT *, SHORT *, SHORT *);
BOOL    PanelDlgFilter( HWND, USHORT, MPARAM, MPARAM);
VOID    ChangeDateTime( HPS, HWND, SHORT, CHAR );
VOID    ControlMinmax( MPARAM,HWND);
VOID    PanelPaint(  HPS );
VOID    PaintDateTime( HPS );
VOID    GetDate(void);
VOID    GetTime(void);
VOID    PanelVScrolls( HWND, USHORT,  MPARAM);
VOID    TimeInit(HWND);
SHORT   MouseTrack( MPARAM );
VOID    GetCountryDependent(VOID);
VOID    BringUpArrows( HWND, SHORT );
VOID    FixDate( SHORT );
VOID    SetDate(void);
VOID    SetTime(void);
/****************************************************************\
 *  Routine Name:
 *--------------------------------------------------------------
 *
 *  Name:TimeDlgProc()
 *
 *  Purpose:
 *  This routine will handle the  processing necessary for the
 *  time dialog box.
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
MRESULT
EXPENTRY TimeDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPS  hpsPanel;

    switch (msg)
    {

    case WM_INITDLG:
        /*
         * Dialog boxes have no way to tell pmwin to ignore the x,y pos.  So
         * we have to make pmwin decide anyway where to put this dialog box.
         */
        hDateTime = hwnd; /* put in a global var, so everybody know we
                             already have an opened dialog box */
        TimeInit (hwnd);
        return ( (MRESULT) 0L);
        break;

    case WM_CHAR:
        if (!bIsIconic)
            PanelDlgFilter( hwnd, msg, mp1, mp2);
        break;

    case WM_BUTTON1DOWN:
        if (bIsIconic)
            return (WinDefDlgProc( hwnd, msg, mp1, mp2));
        else
        {
            hpsPanel = WinGetPS(hwnd);
            ChangeDateTime( hpsPanel, hwnd, MouseTrack(mp1 ),0 );
            WinReleasePS( hpsPanel);
            return( WinDefDlgProc( hwnd, msg, mp1, mp2) );
        }
        break;

    case WM_COMMAND:
        switch (LOUSHORT(mp1))
        {
        case MBID_OK:
            if (fTimeFreez) SetTime();
            if (fDateFreez) SetDate();
            /*Fall through to cancel*/

        case MBID_CANCEL:
           /*The code here is executed also on MBID_OK*/
            hDateTime = (HWND)NULL;
            WinDismissDlg( hwnd, 0);
            break;
        }
        break;


    case WM_MINMAXFRAME:
        ControlMinmax( mp1, hwnd );
        break;

    case WM_PAINT:
        if ( bIsIconic )
            return( WinDefDlgProc( hwnd, msg, mp1, mp2) );
        else {
            WinDefDlgProc(hwnd, msg, mp1, mp2);
            hpsPanel = WinGetPS(hwnd);
            PanelPaint(  hpsPanel);
            WinReleasePS( hpsPanel);
        }
        break;
    case WM_TIMER:
        hpsPanel = WinGetPS(hwnd);
        if ( LOUSHORT( mp1) == IDR_MAIN ) {
           if ( !(fTimeFreez)) GetTime();
           if ( !(fDateFreez )) GetDate();
           PaintDateTime( hpsPanel);
        }
        WinReleasePS( hpsPanel);
        break;
    case WM_VSCROLL:
        if ( bIsIconic )
            break;
        PanelVScrolls( hwnd,
                       HIUSHORT(mp2),
                       mp1     );
        break;



    default:
        return(WinDefDlgProc(hwnd, msg, mp1, mp2));
        break;
    }
   return( (MRESULT)0L);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:GetControlCoordinates()
 *
 *  Purpose: Get coordinates for the current dialog box.
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
VOID
GetControlCoordinates( HWND hDlg, HWND hwnd, SHORT *px, SHORT *py,
                                 SHORT *px2)
{
    WRECT wrcT;

    WinQueryWindowRect(hwnd, (PRECTL)&wrcT);

    /* Convert rect. relative coord.s to dlg relative coord.s Joho.  */

    WinMapWindowPoints( hwnd, hDlg, (PPOINTL)&wrcT, 2);

    /* Convert window coord.s to dialog coord.s (==font transform)   */

    WinMapDlgPoints( hDlg, (PPOINTL)&wrcT, 2, FALSE);

   *px = wrcT.xLeft;
   *py = wrcT.yBottom;
   *px2 = wrcT.xRight;

}


/****************************************************************\
 *
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
 *  Returns: VOID
 *
 *
\****************************************************************/
VOID
TimeInit(HWND hwnd)
{
    SHORT       xTime, yTime;
    SHORT       xDate, yDate;
    SHORT       x2Time, x2Date;


    fTimeFreez = FALSE;
    fDateFreez = FALSE;



    /*****************************************************************/
    /*  Get focus items' handles                                     */
    /*****************************************************************/
    hTime = WinWindowFromID(hwnd,TIME);
    hDate = WinWindowFromID(hwnd,DATE);
    hTimeScroll = WinWindowFromID(hwnd,SCROLL_TIME);
    hDateScroll = WinWindowFromID(hwnd,SCROLL_DATE);



    /**************************************************/
    /*  Get coords of time and date                   */
    /**************************************************/

    GetControlCoordinates( hwnd, hTime, &xTime, &yTime, &x2Time);
    GetControlCoordinates( hwnd, hDate, &xDate, &yDate, &x2Date);

    hCurrentFocus = (HWND)NULL;        /* init focus to nowt        */


    /*****************************************************************/
    /*  Rectangles and locations for Hit-tests and drawing.          */
    /*****************************************************************/

    /*****************************************************************/
    /*  Date rectangles                                              */
    /*****************************************************************/
    rDateAssign[0].yTop =
    rDateAssign[1].yTop =
    rDateAssign[2].yTop =
    rDateSep[0].yTop    =
    rDateSep[1].yTop    = yDate + (UCHAR)15;

    rDateAssign[0].yBottom =
    rDateAssign[1].yBottom =
    rDateAssign[2].yBottom =
    rDateSep[0].yBottom    =
    rDateSep[1].yBottom    = yDate +  (UCHAR)3;

    rDateAssign[0].xLeft   = xDate + (UCHAR)4;

    rDateAssign[0].xRight = rDateSep[0].xLeft    = xDate + (SHORT)16;
    rDateSep[0].xRight    = rDateAssign[1].xLeft = xDate + (SHORT)20;
    rDateAssign[1].xRight = rDateSep[1].xLeft    = xDate + (SHORT)32;
    rDateSep[1].xRight    = rDateAssign[2].xLeft = xDate + (SHORT)36;

    rDateAssign[2].xRight = xDate + (SHORT)48;

    /*****************************************************************/
    /*  Now map the Dlg coord.s into Window coord.s ready for rect.s */
    /*****************************************************************/

    WinMapDlgPoints( hwnd, (PPOINTL)&rDateAssign[0], 2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateSep[0],    2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateAssign[1], 2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateSep[1],    2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateAssign[2], 2, TRUE);


    /*****************************************************************/
    /*  time Rectangles                                              */
    /*****************************************************************/
    rDateTime[HOUR].yTop   =
    rDateTime[MINUTE].yTop =
    rDateTime[SECOND].yTop =
    rTimeSep[0].yTop       =
    rTimeSep[1].yTop       =
    rTimeSep[2].yTop       = yTime + (SHORT)15;

    rDateTime[HOUR].yBottom   =
    rDateTime[MINUTE].yBottom =
    rDateTime[SECOND].yBottom =
    rTimeSep[0].yBottom       =
    rTimeSep[1].yBottom       =
    rTimeSep[2].yBottom       = yTime +  (SHORT)3;

    rDateTime[HOUR].xLeft = xTime + (SHORT)4;

    rDateTime[HOUR].xRight   = rTimeSep[0].xLeft       = xTime + (SHORT)16;
    rTimeSep[0].xRight       = rDateTime[MINUTE].xLeft = xTime + (SHORT)20;
    rDateTime[MINUTE].xRight = rTimeSep[1].xLeft       = xTime + (SHORT)32;
    rTimeSep[1].xRight       = rDateTime[SECOND].xLeft = xTime + (SHORT)36;

    rDateTime[SECOND].xRight = xTime + (SHORT)48;

    rTimeSep[AM_PM].xLeft    = xTime + (SHORT)50;
    rTimeSep[AM_PM].xRight   = xTime + (SHORT)64;

    /*****************************************************************/
    /*  Now map the Dlg coord.s into Window coord.s ready for rect.s */
    /*****************************************************************/

    WinMapDlgPoints( hwnd, (PPOINTL)&rDateTime[HOUR],   2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rTimeSep[0],       2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateTime[MINUTE], 2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rTimeSep[1],       2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rDateTime[SECOND], 2, TRUE);
    WinMapDlgPoints( hwnd, (PPOINTL)&rTimeSep[AM_PM],   2, TRUE);

    /*****************************************************************/
    /*  Get time repaint region                                      */
    /*****************************************************************/
    WinSetRect( hab,
                (PRECTL)&rRepaintTime,
                rDateTime[HOUR].xLeft,    rDateTime[HOUR].yBottom,
                rDateTime[SECOND].xRight, rDateTime[SECOND].yTop   );
    GetCountryDependent();




    /* for the time and date scroll bars (spin buttons), just set the slider */
    /* position in the middle of the range and never move it.    */
    WinSendMsg( hTimeScroll,SBM_SETSCROLLBAR,
                MPFROMLONG(2),
                MPFROM2SHORT(1,3));
    WinSendMsg( hDateScroll,SBM_SETSCROLLBAR,
                MPFROMLONG(2),
                MPFROM2SHORT(1,3));


}

/*********************************************************************/
/*  This routine does not actually filter the keystrokes, but        */
/*  merely observes them in passing. It calls the DefDlgProc BEFORE  */
/*  taking any action of its own, to prevent WinSetFocus calls (in   */
/*  routines such as BringUpArrows in ChangeDateTime) from effecting */
/*  the DlgProc behaviour.                                           */
/*********************************************************************/

BOOL
PanelDlgFilter( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{
   SHORT    Hit;                       /* item "hit", ala mouse hit */
   SHORT    Key, iter;
   HPS      hPS;
   CHAR     UserSpecifiedVal;          /* value user typed in for time or date field  */
   COUNTRYCODE DosCyCode;
   COUNTRYINFO DosCyInfo;
   BOOL        DosCy;
   USHORT      Country;
   ULONG       iTemp;
   SHORT       Position4;
   SHORT       Position6;

    /*****************************************************************/
    /*      Tabs                                                     */
    /*****************************************************************/

    Key = SHORT2FROMMP(mp2);           /* Keep the VKey around       */
    UserSpecifiedVal =  CHAR1FROMMP(mp2);   /* Keep the VKey around       */

    hCurrentFocus = WinQueryFocus( HWND_DESKTOP); /* Debug    */

    WinDefDlgProc( hwnd, msg, mp1, mp2); /* Process NOW **************/

    hCurrentFocus = WinQueryFocus( HWND_DESKTOP);

    if ( !(SHORT1FROMMP(mp1) & KC_KEYUP) &&
          (SHORT1FROMMP(mp1) & KC_VIRTUALKEY) &&
          ( (Key == VK_TAB) || (Key == VK_BACKTAB) ) ) {


        /*************************************************************/
        /*  Set time or date active if they get focus                */
        /*************************************************************/
        /*  NOTE that ChangeDateTime calls BringUpArrows which       */
        /*  does WinSetFocus.                                        */
        /*************************************************************/

        if (hCurrentFocus == hDate) {
            hPS = WinGetPS( hwnd);
            ChangeDateTime( hPS, hwnd, sFirstDate, 0 );
            WinReleasePS( hPS);
        }
        else if (hCurrentFocus == hTime) {
            hPS = WinGetPS( hwnd);
            ChangeDateTime( hPS, hwnd, sFirstTime, 0 );
            WinReleasePS( hPS);
        }

        return(TRUE);

    } /* endif it's a Tab/Backtab */


    /*****************************************************************/
    /*      Arrows                                                   */
    /*****************************************************************/

    if ( !(SHORT1FROMMP(mp1) & KC_KEYUP) &&
          (SHORT1FROMMP(mp1) & KC_VIRTUALKEY) &&
         !(SHORT1FROMMP(mp1) & KC_CHAR) &&
          ( (hCurrentFocus == hTime) || (hCurrentFocus == hDate) ) ) {

        Hit = Selected;                /* init ID-or-action          */

        /* determine which field to highlight by default for date field, */
        /* depending on the date format for the particular country. SNOR */

        DosCyCode.country = 0;             /* Get default                */
        DosCyCode.codepage = 0;            /* Get current process CP     */
        DosCy = (BOOL)!DosQueryCtryInfo( sizeof(COUNTRYINFO),
                                 (PCOUNTRYCODE)&DosCyCode,
                                 (PCOUNTRYINFO)&DosCyInfo,
                                 (PULONG)&iTemp           );

        /*****************************************************************/
        /*  If no info found by Dos, set USA                             */
        /*****************************************************************/
        Country = (USHORT)(DosCy ? DosCyInfo.country : COUNTRY_USA) ;

        /*****************************************************************/
        /* that cool way of doing date format                            */
        /*****************************************************************/
        iTemp = PrfQueryProfileInt( HINI_USER,
                                    (PSZ)"PM_National",
                                    (PSZ)"iDate",
                                    (SHORT ) ( DosCy ? DosCyInfo.fsDateFmt
                                               : 0 )  );
        switch(iTemp)
        {
        case 3:
            /*************************************************************/
            /*                      YDM                                  */
            /*************************************************************/
            Position4 = YEAR;
            Position6 = MONTH;
            break;
        case 2:
            /*************************************************************/
            /*                      YMD                                  */
            /*************************************************************/
            Position4 = YEAR;
            Position6 = DAY;
            break;
        case 1:
            /*************************************************************/
            /*                      DMY                                  */
            /*************************************************************/
            Position4 = DAY;
            Position6 = YEAR;
            break;
        case 0:
        default:
            /*************************************************************/
            /*                      MDY        USA == default            */
            /*************************************************************/
            Position4 = MONTH;
            Position6 = YEAR;
            break;
        }

        switch(Key)
        {


        case VK_UP:
            if (Selected == NOSELECT)
                Hit = ( (hCurrentFocus == hTime) ? (SHORT) HOUR : Position4);
            else
                Hit = ((hCurrentFocus == hTime) ? (SHORT)UPTIME : (SHORT)UPDATE);
            break;
        case VK_DOWN:
            if (Selected == NOSELECT)
                Hit = ((hCurrentFocus == hTime) ? (SHORT) SECOND : Position6);
            else
                Hit = ((hCurrentFocus == hTime) ? (SHORT) DOWNTIME : (SHORT)DOWNDATE);
            break;
        case VK_LEFT:
            if (Selected == NOSELECT)
                Hit = ((hCurrentFocus == hTime) ?  (SHORT)SECOND : Position4);
            break;
        case VK_RIGHT:
            if (Selected == NOSELECT)
                Hit = ((hCurrentFocus == hTime) ? (SHORT)HOUR : Position6);
            break;
        default:
            return(TRUE);
            break;
        }

        hPS = WinGetPS(hwnd);
        for ( iter = 0;
              iter < (SHORT)CHAR3FROMMP(mp1);
              iter++                   ) { /* repeatcount times      */

            if (Key == VK_LEFT) {
                Hit = asLeftOf[Hit];
            }
            if (Key == VK_RIGHT) {
                Hit = asRightOf[Hit];
            }

            ChangeDateTime(hPS, hwnd, Hit, 0);
        }
        WinReleasePS(hPS);
        return(TRUE);
    }

    /*****************************************************************/
    /*      Time or Date values                                      */
    /*****************************************************************/
    if ( ( (hCurrentFocus == hTime) || (hCurrentFocus == hDate) ) &&
         (!(SHORT1FROMMP(mp1) & KC_KEYUP)) &&
         (SHORT1FROMMP(mp1) & KC_CHAR)) {

        /* check to make sure the value typed in is numeric */
        /* if it is not numeric then beep and return true   */
        if ((UserSpecifiedVal > '9') || (UserSpecifiedVal < '0')) {
            WinAlarm (HWND_DESKTOP, WA_WARNING);
            return (TRUE);
        }

        /* otherwise it is a numeric value                        */
        /* now change it from an ascii value to its numeric value */
        UserSpecifiedVal -= 48;

        /*Freez time or date*/
        if (hCurrentFocus == hTime) fTimeFreez = TRUE;
        else fDateFreez = TRUE;

        /*Update value*/
        Hit = ( (hCurrentFocus == hTime) ? (SHORT)TYPETIME : (SHORT)TYPEDATE);
        hPS = WinGetPS(hwnd);
        ChangeDateTime(hPS, hwnd, Hit, UserSpecifiedVal);
        WinReleasePS(hPS);
        return(TRUE);
    }


    return(TRUE);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ChangeDateTime()
 *
 *  Purpose:
 *     This function handles the user interface for changing the date and
 *     time.
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
VOID
ChangeDateTime( HPS hpsPanel, HWND hwnd, SHORT HitRect, CHAR UserSpecifiedValue)
{
    SHORT onesdigit;
    SHORT newvalue;
    SHORT convertflag = FALSE;
    SHORT savevalue;
    COUNTRYCODE DosCyCode;
    COUNTRYINFO DosCyInfo;
    BOOL        DosCy;
    USHORT      Country;
    ULONG       iTemp;
    SHORT       Position4;
    SHORT       Position6;

    switch(HitRect)
    {

    case HOUR:
    case MINUTE:
    case SECOND:
        fTimeFreez = TRUE;
        if (Selected == NOSELECT)      /* nothing selected           */
            BringUpArrows(hwnd,HitRect);
        else {                         /* take care of prev seln     */
            if ( (Selected>=MONTH) && (Selected<=YEAR) ) {
                                       /* if date item prev selected */
                PaintDateTime(hpsPanel);
                BringUpArrows(hwnd,HitRect);
            }
            wPrevDateTime[Selected] = -1; /* unhilite hilited */
        }

        PreviousSelected = Selected;        /*save previously selected item */
        Selected = HitRect;
        PaintDateTime(hpsPanel);
        break;

    case MONTH:
    case DAY:
    case YEAR:
        fDateFreez = TRUE;
        if (Selected == NOSELECT)      /* nothing selected           */
            BringUpArrows(hwnd,HitRect);
        else {                         /* take care of prev seln     */
            if ( (Selected>=HOUR) && (Selected<=SECOND) ) {
                                       /* if time item prev selected */
                BringUpArrows(hwnd,HitRect);
            }
            wPrevDateTime[Selected] = -1; /* unhilite hilited */
        }
        PreviousSelected = Selected;        /* @SNOR EviL save previously selected item */
        Selected = HitRect;
        PaintDateTime(hpsPanel);
        break;

    case UPTIME:
    case DOWNTIME:
        if ( (Selected>=HOUR) && (Selected<=SECOND) ) {
                                       /* if time item hilited       */
            /* incr/decr */
            if (HitRect == UPTIME)
                wDateTime[Selected] += 1;
            else
                wDateTime[Selected] -= 1;

            /* take care of wraparound */
            if (wDateTime[Selected] > wModulos[Selected])
                wDateTime[Selected] = HOUR;
            if (wDateTime[Selected] < HOUR)
                wDateTime[Selected] = wModulos[Selected];

            PaintDateTime(hpsPanel);      /* show changed item          */
        }
        else
            ChangeDateTime( hpsPanel,hwnd,
                            (SHORT)( (HitRect == UPTIME) ? HOUR : SECOND), 0  );
        break;

    case UPDATE:
    case DOWNDATE:
        if ( (Selected>=MONTH) && (Selected<=YEAR) ) {
                                       /* if date item hilited       */

            /* incr/decr */
            if (HitRect == UPDATE)
                wDateTime[Selected] += 1;
            else
                wDateTime[Selected] -= 1;

            /* take care of wraparound */
            FixDate(Selected);

            PaintDateTime(hpsPanel);      /* show changed item       */
        }
        else
        {
            /* determine which field to highlight by default for date field, */
            /* depending on the date format for the particular country. */

            DosCyCode.country = 0;             /* Get default                */
            DosCyCode.codepage = 0;            /* Get current process CP     */
            DosCy = (BOOL) !DosGetCtryInfo( sizeof(COUNTRYINFO),
                                     (PCOUNTRYCODE)&DosCyCode,
                                     (PCOUNTRYINFO)&DosCyInfo,
                                     (PULONG)&iTemp           );

            /*****************************************************************/
            /*  If no info found by Dos, set USA                             */
            /*****************************************************************/
            Country = (USHORT) (DosCy ? DosCyInfo.country : COUNTRY_USA );

            /*****************************************************************/
            /* that cool way of doing date format            @SNOR           */
            /*****************************************************************/
            iTemp = PrfQueryProfileInt( HINI_USER,
                                        (PSZ)"PM_National",
                                        (PSZ)"iDate",
                                        (SHORT )( DosCy ? DosCyInfo.fsDateFmt
                                                 : 0 )  );
            switch(iTemp) {
            case 2:
                /*************************************************************/
                /*                      YMD                  @SNOR           */
                /*************************************************************/
                Position4 = YEAR;
                Position6 = DAY;
                break;
            case 1:
                /*************************************************************/
                /*                      DMY                  @SNOR           */
                /*************************************************************/
                Position4 = DAY;
                Position6 = YEAR;
                break;
            case 0:
            default:
                /*************************************************************/
                /*                      MDY        USA == default   @SNOR    */
                /*************************************************************/
                Position4 = MONTH;
                Position6 = YEAR;
                break;
            }

            ChangeDateTime( hpsPanel, hwnd,
                            (SHORT)
                             ( (HitRect == UPDATE) ? Position4 : Position6) ,
                                   0);
        }
        break;
    case TYPETIME:
        savevalue = wDateTime[Selected];

        /* if this is the first value typed in for this selected field, */
        /* then replace it in the ones digit of the field               */
        if (Selected != PreviousSelected) {

            /* treat hour with care if we are in 12 hr. format, */
            /* cuz it is stored in 24 hour format.              */
            /* convert it to 12 hour format, cuz that is what   */
            /* the user sees on the screen, and then later      */
            /* convert it back.                                 */
            if ((Selected == HOUR) && bTwelveHourFormat) {
                if (wDateTime[HOUR] > 12) {
                    wDateTime[HOUR] -= 12;
                    convertflag = TRUE;
                }
                if (wDateTime[HOUR] == 0) {
                    wDateTime[HOUR] = 12;
                    convertflag = TRUE;
                }
                if (wDateTime[HOUR] == 12) {
                    wDateTime[HOUR] = 0;
                    convertflag = TRUE;
                }
            }

            onesdigit = wDateTime[Selected] % (SHORT)10;
            wDateTime[Selected] -= onesdigit;
            wDateTime[Selected] += UserSpecifiedValue;

            /* if necessary, convert back to 24 hour format */
            if (convertflag) {
                if (wDateTime[HOUR] > 12) {   /* invalid time */
                    WinAlarm(HWND_DESKTOP, WA_WARNING);
                    convertflag = FALSE;
                    PreviousSelected = NOSELECT;
                    wDateTime[HOUR] = savevalue;
                    return;
                }
                else if (wDateTime[HOUR] == 12) {
                    wDateTime[HOUR] = 0;
                    convertflag = FALSE;
                }
                else {
                    wDateTime[HOUR] += 12;
                    convertflag = FALSE;
                }
            }
            PreviousSelected = Selected;
        }

        /* otherwise, if this isnt the first value typed in for this      */
        /* selected field, then insert it into the ones field             */
        else {


            /* treat hour with care if we are in 12 hr. format, */
            /* cuz it is stored in 24 hour format.              */
            /* convert it to 12 hour format, cuz that is what   */
            /* the user sees on the screen, and then later      */
            /* convert it back.                                 */
            if ((Selected == HOUR) && bTwelveHourFormat) {
                if (wDateTime[HOUR] > 12) {
                    wDateTime[HOUR] -= 12;
                    convertflag = TRUE;
                }
            }

            onesdigit           = wDateTime[Selected] % (SHORT)10;
            wDateTime[Selected] = ( (onesdigit* (SHORT)10)+UserSpecifiedValue);

            /* handle invalid settings here */
            if (Selected == HOUR){
                if ((bTwelveHourFormat) && (wDateTime[HOUR] > 12)){
                    WinAlarm(HWND_DESKTOP, WA_WARNING);
                    wDateTime[HOUR] = savevalue;
                    PreviousSelected = NOSELECT;
                    return;
                }
                else if ((!bTwelveHourFormat) && (wDateTime[HOUR] > 24)){
                    WinAlarm(HWND_DESKTOP, WA_WARNING);
                    wDateTime[HOUR] = savevalue;
                    PreviousSelected = NOSELECT;
                    return;
                }
            }
            if ((Selected == MINUTE) && (wDateTime[MINUTE] > 60)){
                WinAlarm(HWND_DESKTOP, WA_WARNING);
                wDateTime[MINUTE] = savevalue;
                PreviousSelected = NOSELECT;
                return;
            }
            if ((Selected == SECOND) && (wDateTime[SECOND] > 60)){
                WinAlarm(HWND_DESKTOP, WA_WARNING);
                wDateTime[SECOND] = savevalue;
                PreviousSelected = NOSELECT;
                return;
            }  /* if it passes those tests then it is a valid setting */

            /* if necessary, convert back to 24 hour format */
            if (convertflag) {
                if (wDateTime[HOUR] == 12) {
                    wDateTime[HOUR] = 0;
                    convertflag = FALSE;
                }
                else {
                    wDateTime[HOUR] += 12;
                    convertflag = FALSE;
                }
            }
        }

        /* finally, update screen                                          */
        PaintDateTime(hpsPanel);
        break;

    case TYPEDATE:                                          /* @SNOR EviL */
        savevalue = wDateTime[Selected];

        /* if this is the first value typed in for this selected field, */
        /* then replace it in the ones digit of the field               */
        if (Selected != PreviousSelected)
        {
                onesdigit = wDateTime[Selected] % (SHORT)10;
                wDateTime[Selected] -= onesdigit;
                wDateTime[Selected] += UserSpecifiedValue;
                PreviousSelected = Selected;
        }

        /* otherwise, if this isnt the first value typed in for this      */
        /* selected field, then insert it into the ones field             */
        else {

            /* take special care not to trash the century if this is the */
            /* year field.                                               */
            if (Selected == YEAR) {
                onesdigit = wDateTime[Selected] % (SHORT)10;
                newvalue  = ((onesdigit* (SHORT)10)+UserSpecifiedValue);

                if (newvalue >= (SHORT)80)
                    wDateTime[Selected] = (SHORT)1900 + newvalue;
                else
                    wDateTime[Selected] = (SHORT)2000 + newvalue;
            }
            else
            {
                onesdigit           = wDateTime[Selected] % (SHORT)10;
                wDateTime[Selected] = ((onesdigit* (SHORT)10)+UserSpecifiedValue);
            }
        }

        /* test for valid value */
        if ((Selected == MONTH) && (wDateTime[MONTH] > 12)){
            WinAlarm(HWND_DESKTOP, WA_WARNING);
            wDateTime[MONTH] = savevalue;
            PreviousSelected = NOSELECT;
            return;
        }
        if ((Selected == DAY) && (wDateTime[DAY] > 31)){
            WinAlarm(HWND_DESKTOP, WA_WARNING);
            wDateTime[DAY] = savevalue;
            PreviousSelected = NOSELECT;
            return;
        }

        /* finally, update screen                                          */
        PaintDateTime(hpsPanel);
        break;

    default:                           /* outside relevant date/time */
        if (Selected != NOSELECT) {    /* end "usSet" mode             */
            wPrevDateTime[Selected] = -1; /* unhilite hilited        */
            PreviousSelected = Selected;
            Selected = NOSELECT;
            PaintDateTime( hpsPanel);
        }
        break;

    }
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ControlMinMax()
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
VOID ControlMinmax( MPARAM mp1,HWND hwnd )
{

    bIsIconic = (BOOL)( (((PSWP)mp1)->fl & SWP_MINIMIZE) != (SHORT) 0);

    if ( bIsIconic )
    {
          /*
           *A bad thing in dialog boxes is that when they are minimized they
           *still draw there control windows over the supplied icon. So we
           *find if we are minimizing or restoring and hide the control window
           *in the left button, happen to be MBID_OK
           */
           WinShowWindow(WinWindowFromID(hwnd,MBID_OK),FALSE);


    }
}

/****************************************************************\
 *
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
 *  Returns:
 *
 *
\****************************************************************/
VOID  PanelPaint(  HPS hpsPanel)
{
   SHORT   iter;

    /*****************************************************************/
    /*  Write the date and time                                      */
    /*****************************************************************/

    /*****************************************************************/
    /*  Paint static fields                                          */
    /*****************************************************************/

    for(iter=0; iter<2; iter++)
    {
        WinDrawText( hpsPanel,
                     -1,
                     (PSZ)szTimeSep,
                     (PRECTL)&rTimeSep[iter],
                     SYSCLR_WINDOWTEXT,
                     SYSCLR_WINDOW,
                     DT_CENTER | DT_VCENTER );
        WinDrawText( hpsPanel,
                     -1,
                     (PSZ)szDateSep,
                     (PRECTL)&rDateSep[iter],
                     SYSCLR_WINDOWTEXT,
                     SYSCLR_WINDOW,
                     DT_CENTER | DT_VCENTER );
    }

    /*****************************************************************/
    /*  Paint varying fields                                         */
    /*****************************************************************/
    if (Selected == NOSELECT)
    {
       if (!fDateFreez) GetDate();
       if (!fTimeFreez) GetTime();
    }

    for (iter=0;iter<6;iter++)    /* ensure all fields get painted   */
       wPrevDateTime[iter] = -1;
    PaintDateTime( hpsPanel);


    /*****************************************************************/
    /*  This call was causing a problem where hitting Alt when       */
    /*  Time or Date selected didn't get rid of Menu highlight.      */
    /*  I'm not sure what it was doing here anyway...                */
    /*****************************************************************/

}   /* End of PanelPaint */
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:PaintDateTime()
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
 *
\****************************************************************/
VOID  PaintDateTime(HPS hpsPanel)
{
    SHORT   iter;
    CHAR  szNumber[4];                 /* ascii value                */
    CHAR  szPadding[4];                /* pad numbers on left        */
    SHORT   wHourSave;                   /* hours altered for 12hr fmt  */
    SHORT   wYearSave;                   /* year moduloed 100          */

    /*****************************************************************/
    /*  Save hours and year first. Outside here yields: Cur. held     */
    /*  as DOS uses; Prev. as drawn in panel. Inside here: both      */
    /*  Cur and Prev drawn as in Panel.                              */
    /*****************************************************************/
    wHourSave = wDateTime[HOUR];
    wYearSave = wDateTime[YEAR];

    if (bTwelveHourFormat)
    {
        if (wDateTime[HOUR] > 12)
            wDateTime[HOUR] -= 12;
        if (wDateTime[HOUR] == 0)
            wDateTime[HOUR] = 12;
    }

    /*****************************************************************/
    /*  Set 1159 & 2359 (i.e. "am" "pm" etc.).                       */
    /*****************************************************************/
    if (wPrevDateTime[HOUR] != wDateTime[HOUR])
    {
        WinFillRect( hpsPanel,
                     (PRECTL)&rTimeSep[AM_PM],
                     SYSCLR_WINDOW             );
        WinDrawText( hpsPanel,
                     -1,
                     ( (wHourSave < 12) ? (PSZ)szAnteMeridian
                                        : (PSZ)szPostMeridian ),
                     (PRECTL)&rTimeSep[AM_PM],
                     SYSCLR_WINDOWTEXT,
                     SYSCLR_WINDOW,
                     DT_CENTER | DT_VCENTER );
    }

    /*****************************************************************/
    /*  Save the century for DOS                                     */
    /*****************************************************************/
    wDateTime[YEAR] %= 100;

    /*****************************************************************/
    /*  Write each field if changed or selected                      */
    /*****************************************************************/
    for (iter=0;iter< (SHORT) 6;iter++)
    {              /* (hh)-mm-ss/mm-dd-yy */

        if ( (wDateTime[iter] != wPrevDateTime[iter])
             || (iter == Selected)                    )
        {

            /*********************************************************/
            /*  Get the digit string                                 */
            /*********************************************************/
            _itoa(wDateTime[iter],szNumber,10);

            /*********************************************************/
            /*  Pad if less than 2 digits; blank pad iff             */
            /*  !bLeadingZero and is leftmost of time or date        */
            /*  (remember date format is dependent on coutry format) */
            /*********************************************************/
            if( wDateTime[iter]<10 )
            {
                strcpy( szPadding,
                        ( !bLeadingZero
                          && ( (iter == HOUR) ||
                               ( rDateTime[iter].xLeft
                                 == rDateAssign[0].xLeft ) )
                          ? " "
                          : "0" )
                      );

                strcat( szPadding, szNumber);
                strcpy( szNumber, szPadding);

            }  /* endif <10 */

            /* draw field */
            WinFillRect( hpsPanel,
                         (PRECTL)&rDateTime[iter],
                         SYSCLR_WINDOW             );
            WinDrawText( hpsPanel,
                         -1,
                         (PSZ)szNumber,
                         (PRECTL)&rDateTime[iter],
                         SYSCLR_WINDOWTEXT,
                         SYSCLR_WINDOW,
                         DT_CENTER | DT_VCENTER );

            /* hilite if selected */
            if (iter == Selected)
                WinInvertRect( hpsPanel,
                               (PRECTL)&rDateTime[iter]);

        }  /* end if != ... */

        /*************************************************************/
        /*  Set new previous datetime                                */
        /*************************************************************/
        wPrevDateTime[iter] = wDateTime[iter];

    }    /* end for */

    /*****************************************************************/
    /*  Restore hours and year for setting date/time                  */
    /*****************************************************************/
   wDateTime[HOUR] = wHourSave;
   wDateTime[YEAR] = wYearSave;

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: GetTime
 *
 *  Purpose:
 *  Get the current date and time.
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
VOID  GetTime(VOID)
{
    struct _DATETIME DT;

    DosGetDateTime( (LPDT)&DT);
    wDateTime[HOUR]   = (USHORT)DT.hours;
    wDateTime[MINUTE] = (USHORT)DT.minutes;
    wDateTime[SECOND] = (USHORT)DT.seconds;
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:GetDate()
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
VOID  GetDate(VOID)
{
    struct _DATETIME DT;

    DosGetDateTime( (LPDT)&DT);
    wDateTime[MONTH] = (USHORT)DT.month;
    wDateTime[DAY]   = (USHORT)DT.day;
    wDateTime[YEAR]  =       DT.year; /* Year is two bytes          */
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:PanelVScrolls()
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
VOID  PanelVScrolls(HWND hwnd,USHORT ScrollMsg,MPARAM idScroll)
{
    HPS  hpsPanel;

    hpsPanel = WinGetPS(hwnd);
    switch(ScrollMsg)
    {
    case SB_LINEUP:                    /* line left                  */
    case SB_PAGEUP:                    /* page left                  */

        if (idScroll == (MPARAM)SCROLL_TIME)
            ChangeDateTime( hpsPanel, hwnd, UPTIME,  0);
        else
            ChangeDateTime( hpsPanel, hwnd, UPDATE, 0);
        WinReleasePS( hpsPanel);
        break;


    case SB_LINEDOWN:                  /* line right                 */
    case SB_PAGEDOWN:                  /* page right                 */

        if (idScroll == (MPARAM)SCROLL_TIME)
            ChangeDateTime( hpsPanel, hwnd, DOWNTIME, 0);
        else
            ChangeDateTime( hpsPanel, hwnd, DOWNDATE, 0);
        WinReleasePS( hpsPanel);
        break;

    default:
        WinReleasePS( hpsPanel);
        break;
    }
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:MouseTrack()
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
 *          Offset for mouse pointer if the mouse was hit
\****************************************************************/
SHORT MouseTrack(MPARAM mp1)
{
    POINTL  ptl;
    SHORT   iter;

    ptl.x = (ULONG)LOUSHORT( mp1);
    ptl.y = (ULONG)HIUSHORT( mp1);

    for (iter=0;iter<6;iter++)
        if ( WinPtInRect( hab,
                          (PRECTL)&rDateTime[iter],
                          &ptl)      )
            return(iter);

    return(NOSELECT);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:GetCountryDependent()
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
 *
\****************************************************************/
VOID GetCountryDependent(VOID )
{
    ULONG       iTemp;
    COUNTRYCODE DosCyCode;
    COUNTRYINFO DosCyInfo;
    USHORT      Country;
    BOOL        DosCy;

    /*****************************************************************/
    /*  Get the DOS country info to use as default if OS2.INI lacks. */
    /*****************************************************************/
    DosCyCode.country = 0;             /* Get default                */
    DosCyCode.codepage = 0;            /* Get current process CP     */
    DosCy = (BOOL) !DosGetCtryInfo( sizeof(COUNTRYINFO),
                             (PCOUNTRYCODE)&DosCyCode,
                             (PCOUNTRYINFO)&DosCyInfo,
                             (PULONG)&iTemp           );
    /*****************************************************************/
    /*  If no info found by Dos, set USA                             */
    /*****************************************************************/
    Country = (USHORT)( DosCy ? DosCyInfo.country : COUNTRY_USA) ;

    /*****************************************************************/
    /* get 12/24 hours format flag                                    */
    /*****************************************************************/
    iTemp = PrfQueryProfileInt( HINI_USER,
                                (PSZ)"PM_National",
                                (PSZ)"iTime",
                                (SHORT) (DosCy ? DosCyInfo.fsTimeFmt
                                         : 0  ) );
    /*****************************************************************/
    /* 0 == twelve hours format; 1 == twenty-four hours format         */
    /*****************************************************************/
    bTwelveHourFormat = (BOOL) !iTemp;

    /*****************************************************************/
    /* leading zero                                                  */
    /*****************************************************************/
    bLeadingZero = (BOOL)PrfQueryProfileInt( HINI_USER,
                                             (PSZ)"PM_National",
                                             (PSZ)"iLzero",
                                             0                  );


    /*****************************************************************/
    /* date separator string                                         */
    /*****************************************************************/
    PrfQueryProfileString( HINI_USER,
                           (PSZ)"PM_National",
                           (PSZ)"sDate",
                           DosCy ? (PSZ)DosCyInfo.szDateSeparator
                                 : (PSZ)"/",
                           (PSZ)szDateSep,
                           (ULONG)DATE_SEP_LEN+1                         );


    /*****************************************************************/
    /* time separator string                                         */
    /*****************************************************************/
    PrfQueryProfileString( HINI_USER,
                           (PSZ)"PM_National",
                           (PSZ)"sTime",
                           DosCy ? (PSZ)DosCyInfo.szTimeSeparator
                                 : (PSZ)":",
                           (PSZ)szTimeSep,
                           (ULONG)TIME_SEP_LEN+1                         );

    /*****************************************************************/
    /* AM/PM strings                                                 */
    /*****************************************************************/
    PrfQueryProfileString( HINI_USER,
                           (PSZ)"PM_National",
                           (PSZ)"s1159",
                           Country == COUNTRY_USA
                            ? (PSZ)"AM"
                            : Country == COUNTRY_UK
                              ? (PSZ)"am"
                              : (PSZ)"",
                           (PSZ)szAnteMeridian,
                           (ULONG)STRING_AM_LEN+1           );
    PrfQueryProfileString( HINI_USER,
                           (PSZ)"PM_National",
                           (PSZ)"s2359",
                           Country == COUNTRY_USA
                            ? (PSZ)"PM"
                            : Country == COUNTRY_UK
                              ? (PSZ)"pm"
                              : (PSZ)"",
                           (PSZ)szPostMeridian,
                           (ULONG)STRING_PM_LEN+1           );

    sFirstTime = HOUR;
    asLeftOf[HOUR] = SECOND;
    asLeftOf[MINUTE] = HOUR;
    asLeftOf[SECOND] = MINUTE;
    asRightOf[HOUR] = MINUTE;
    asRightOf[MINUTE] = SECOND;
    asRightOf[SECOND] = HOUR;

    /*****************************************************************/
    /* that cool way of doing date format                            */
    /*****************************************************************/
    vusDateFormat = iTemp = PrfQueryProfileInt( HINI_USER,
                                (PSZ)"PM_National",
                                (PSZ)"iDate",
                                (SHORT ) ( DosCy ? DosCyInfo.fsDateFmt
                                          : 0 )  );
    switch(iTemp)
    {
    case 2:
        /*************************************************************/
        /*                      YMD                                  */
        /*************************************************************/
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[YEAR],
                     (PRECTL)&rDateAssign[0]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[MONTH],
                     (PRECTL)&rDateAssign[1]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[DAY],
                     (PRECTL)&rDateAssign[2]    );
        sFirstDate = YEAR;
        asLeftOf[YEAR] = DAY;
        asLeftOf[MONTH] = YEAR;
        asLeftOf[DAY] = MONTH;
        asRightOf[YEAR] = MONTH;
        asRightOf[MONTH] = DAY;
        asRightOf[DAY] = YEAR;
        break;
    case 1:
        /*************************************************************/
        /*                      DMY                                  */
        /*************************************************************/
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[DAY],
                     (PRECTL)&rDateAssign[0]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[MONTH],
                     (PRECTL)&rDateAssign[1]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[YEAR],
                     (PRECTL)&rDateAssign[2]    );
        sFirstDate = DAY;
        asLeftOf[YEAR] = MONTH;
        asLeftOf[MONTH] = DAY;
        asLeftOf[DAY] = YEAR;
        asRightOf[YEAR] = DAY;
        asRightOf[MONTH] = YEAR;
        asRightOf[DAY] = MONTH;
        break;
    case 0:
    default:
        /*************************************************************/
        /*                      MDY        USA == default            */
        /*************************************************************/
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[MONTH],
                     (PRECTL)&rDateAssign[0]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[DAY],
                     (PRECTL)&rDateAssign[1]    );
        WinCopyRect( hab,
                     (PRECTL)&rDateTime[YEAR],
                     (PRECTL)&rDateAssign[2]    );
        sFirstDate = MONTH;
        asLeftOf[YEAR] = DAY;
        asLeftOf[MONTH] = YEAR;
        asLeftOf[DAY] = MONTH;
        asRightOf[YEAR] = MONTH;
        asRightOf[MONTH] = DAY;
        asRightOf[DAY] = YEAR;
        break;
    }


}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:BringUpArrows.
 *
 *  Purpose:
 *    Take care of up down arrows
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
VOID BringUpArrows(HWND hwnd,SHORT Item)
{

   if( (Item>=HOUR) && (Item<=SECOND) )
   {     /* time item           */

      /* Set focus to current groupbox if control panel has focus */
      if ( WinQueryWindow(WinQueryFocus(HWND_DESKTOP), QW_PARENT) == hwnd )
      {
         WinSetFocus(HWND_DESKTOP, hTime);
         hCurrentFocus = hTime;

      }
   } /* endif time */

   else if( (Item>=MONTH) && (Item<=YEAR) )
   {   /* date item         */

      /* Set focus to current groupbox if control panel has focus */
      if ( WinQueryWindow(WinQueryFocus(HWND_DESKTOP), QW_PARENT) == hwnd)
      {
         WinSetFocus(HWND_DESKTOP, hDate);
         hCurrentFocus = hDate;


      }
   } /* endif Date */

}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: FixDate
 *
 *  Purpose:
 *  Takes care of date wraparound
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
VOID  FixDate(SHORT Field)
{
    switch(Field)
    {
    case MONTH:
        if (wDateTime[MONTH] > 12)
            wDateTime[MONTH] = 1;
        if (wDateTime[MONTH] < 1)
            wDateTime[MONTH] = 12;
        FixDate(DAY);                    /* keep day in new bounds   */
        break;

    case DAY:
        /* check for leap year */
        if ( ( (wDateTime[YEAR] % 4 == 0 && wDateTime[YEAR] % 100 != 0)
               || wDateTime[YEAR] % 400 == 0
             ) && wDateTime[MONTH] == 2
           ) {
            if(wDateTime[DAY] > 29) wDateTime[DAY] = 1;
            if(wDateTime[DAY] < 1) wDateTime[DAY] = 29;
        }
        else
        {   /* non-leap year   */
            if (wDateTime[DAY] > DaysPerMonth[wDateTime[MONTH]])
                wDateTime[DAY] = 1;
            if (wDateTime[DAY] < 1)
                wDateTime[DAY] = DaysPerMonth[wDateTime[MONTH]];
        }
        break;

    case YEAR:
        if (wDateTime[YEAR] > 2079)
            wDateTime[YEAR] = 1980;
        if (wDateTime[YEAR] < 1980)
            wDateTime[YEAR] = 2079;
        FixDate(DAY);                    /* for leap yr. to non-leap */
        break;

   }

}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:Settime
 *
 *  Purpose:
 *  Sets the system time.
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
VOID  SetTime(VOID)
{
    struct _DATETIME DT;
    APIRET rc;

    DosGetDateTime( (LPDT)&DT);
    DT.hours   = (BYTE)wDateTime[HOUR]  ;
    DT.minutes = (BYTE)wDateTime[MINUTE];
    DT.seconds = (BYTE)wDateTime[SECOND];
    rc = DosSetDateTime( (LPDT)&DT);
    if (rc)
    {
          WinAlarm(HWND_DESKTOP, WA_WARNING);
    }
    /*
     *if the alarm
     *is set then update
     *it
     */
     if(cp.alarm.usMode & AM_ACTIVE )
     {
          AlarmSetTimer(cp.alarm.uchHour,
                              cp.alarm.uchMinutes);
     }

}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:SetDate()
 *
 *  Purpose:
 *  Set system date.
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
VOID SetDate(VOID)
{
    struct _DATETIME DT;
    APIRET rc;

    DosGetDateTime( (LPDT)&DT);
    DT.month = (BYTE)wDateTime[MONTH];
    DT.day   = (BYTE)wDateTime[DAY]  ;
    DT.year  =       wDateTime[YEAR] ; /* Year is two bytes          */
    rc = DosSetDateTime( (LPDT)&DT);
    if (rc)
    {
          WinAlarm(HWND_DESKTOP, WA_WARNING);
    }

    /*
     *if the alarm
     *is set then update
     *it
     */
     if(cp.alarm.usMode & AM_ACTIVE )
     {

          AlarmSetTimer(cp.alarm.uchHour,
                              cp.alarm.uchMinutes);
     }
}

/*--------------------------------------------------------------*\
 *  End of file :udtime.c
\*--------------------------------------------------------------*/

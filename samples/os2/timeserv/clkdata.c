/*static char *SCCSID = "@(#)clkdata.c	6.5 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE:clkdata.c                                                    *
 *                                                                         *
 *       DESCRIPTION:    Presentation Manager Alarm Clock Application      *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Copyright 1989, 1990, 1992 IBM Corp.                               *
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
 *  This source file contains no functional code, but is used
 *  to include global data variable for various modules of the
 *  sample clock program
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/
#define INCL_PM
#define INCL_DOSSEMAPHORES
#define INCL_DOSDATETIME


#include <os2.h>
#include <string.h>
#include "clock.h"
#include "res.h"




/*--------------------------------------------------------------*\
 *  Global variables
\*--------------------------------------------------------------*/
HWND hwndFrame;
HWND hwndTitleBar;
HWND hwndSysMenu;
HWND hwndMinMax;
HWND hwndMenu;
HAB  hab;
HMQ  hmq;
HSWITCH hsw;
BOOL fStartAsIcon = FALSE;
BOOL fHelpEnabled = FALSE;

CLOCKPREF cp;
HWND hwndHelpInstance;

PFNWP pfnwpFrameWndProc;


SHORT      Selected = NOSELECT;     /* currently hilited and selected field */
SHORT      PreviousSelected;        /* previously hilited and selected field */

UCHAR      lastSecondDrawn = -1;


BOOL bLeadingZero;                  /* pad date/time with zero */
BOOL bIsIconic = FALSE;


WRECT rDateTime[6];                /* placement of time and date matters */
WRECT rAlarmTime[3];               /* placement of time and date matters */
WRECT rRepaintTime;                /* repaint time region */
WRECT rDateAssign[3];              /* holds date locs, so can quickly
                                      reassign rDateTime[MONTH-YEAR]
                                      to diff date formats */
SHORT sFirstTime;                  /* first field in time */
SHORT sFirstDate;                  /* first field in date */
SHORT asLeftOf[6];                 /* for inc/dec inclds wrapping */
SHORT asRightOf[6];                /* for inc/dec inclds wrapping */
SHORT wDateTime[6];                /* values for first 6 date/time items */
SHORT wModulos[3] =                /* highest value for each; actually mod n+ 1 */
      { 23,59,59 };
SHORT wPrevDateTime[6];            /* only repaint fields if nec */

HWND  hDate, hTime;                /* handle to group boxes (for focus) */
HWND  hDateScroll;
HWND  hTimeScroll;
HWND  hCurrentFocus;               /* control with current focus */
HWND  hDateTime;
HWND  hAlarmTime;

BOOL bTwelveHourFormat;            /* 12 hr. versus military */
WRECT rTimeSep[3];                 /* posn of time separator strings and AM/PM */
CHAR  szTimeSep[TIME_SEP_LEN+1];     /* time separator string */
CHAR  szAnteMeridian[STRING_AM_LEN+1]; /* AM/PM */
CHAR  szPostMeridian[STRING_PM_LEN+1];
CHAR  szTitle [81];
CHAR  szErrClocks[PATHMAX+1];

WRECT rDateSep[2];                 /* posn of date separator strings */
CHAR  szDateSep[DATE_SEP_LEN+1];   /* date separator string */
BOOL fTimeFreez = FALSE;
BOOL fDateFreez = FALSE;
USHORT vusDateFormat;
MATRIXLF vmatlfDateTrans = {
     MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
     MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
     0L ,                      0L ,                      1L } ;
MATRIXLF vmatlfDateScale  = {
     MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
     MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
     0L ,                      0L ,                      1L } ;

SWP swp;
HPS hps;
HDC hdc;
RECTL rclPage;
DATETIME dt;
BOOL f;
BOOL fIconic, fShowSecondHand;
SIZEL sizl = { 200 , 200 };
ULONG cxRes , cyRes;
char achOldTime[9];
char achAmPm[3];
char achOldAmPm[3];
char achOldDate[9];
HPS hpsWnd;
LONG cxScreen , cyScreen;          /* screen dimensions */
LONG vclrBG[3];
LONG vclrFace[3];
LONG vclrRing[3];
LONG vclrHands[3];
LONG vclrMajorTicks[3];
LONG vclrMinorTicks[3];
LONG vclrDate[4];
USHORT vusDatePos;
BOOL vbInAlarm;
PFNWP pfnwpEntryField;
HWND hwndEntryField;

HWND hwndClient;
HPS hpsBuffer;
HDC hdcBuffer;
BOOL fBufferDirty = FALSE;

ULONG cColorPlanes, cColorBitcount;
BOOL fEndThread = FALSE;
TIMER_RESOURCES TimerResources;

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/




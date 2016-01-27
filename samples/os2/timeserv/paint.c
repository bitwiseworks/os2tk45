/*static char *SCCSID = "@(#)paint.c	6.6 92/02/18";*/
/*=========================================================================\
 *                                                                         *
 *       FILE:paint.c                                                      *
 *                                                                         *
 *       DESCRIPTION:   routines used in painting the clock face           *
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
 *--------------------------------------------------------------
 *
 *  This source file contains the following functions:
 *
 *   ClkPaint
 *   ClkDrawHand
 *   ClkDrawFace
 *   ClkDrawRing
 *   ClkDrawDate
 *   ClkDrawTicks
 *   DrawDigitalTime
 *   GetArrangedDate
 *   DrawTrapez
 *   DrawFullRing
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs
\*--------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
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


#define MATLF_SIZE ( sizeof ( MATRIXLF ) / sizeof ( LONG ) )

#define CLK_MAJORTICKS  0x0001
#define CLK_MINORTICKS  0x0002

#define XDATE 52L
#define YDATE 14L
#define ZDATE 1L

POINTL aptlFillet1[]= {{53L ,63L },{ 70L ,50L },{68L,47L}} ;
POINTL aptlFillet2[]= {{68L,47L},{ 70L, 50L },{56L ,67L }} ;
POINTL aptlFillet3[]= {{53L ,64L },{ 56L ,62L },{60L,58L}} ;
POINTL aptlFillet4[]= {{60L,58L},{ 60L, 63L },{56L ,67L }} ;
POINTL ptlLight= { -1L , 1L } ;
POINTL ptlShade= {  2L, -2L } ;
POINTL ptlFace = { 0L, 0L};

static FIXED fxSin [60] =
{
    0x00000000, 0x00001ac2, 0x00003539, 0x00004f1b, 0x0000681f, 0x00007fff,
    0x00009679, 0x0000ab4c, 0x0000be3e, 0x0000cf1b, 0x0000ddb3, 0x0000e9de,
    0x0000f378, 0x0000fa67, 0x0000fe98, 0x0000ffff, 0x0000fe98, 0x0000fa67,
    0x0000f378, 0x0000e9de, 0x0000ddb3, 0x0000cf1b, 0x0000be3e, 0x0000ab4c,
    0x00009679, 0x00008000, 0x00006820, 0x00004f1b, 0x00003539, 0x00001ac2,
    0x00000000, 0xffffe53e, 0xffffcac7, 0xffffb0e5, 0xffff97e1, 0xffff8001,
    0xffff6988, 0xffff54b5, 0xffff41c2, 0xffff30e5, 0xffff224d, 0xffff1622,
    0xffff0c88, 0xffff0599, 0xffff0168, 0xffff0001, 0xffff0167, 0xffff0599,
    0xffff0c88, 0xffff1622, 0xffff224d, 0xffff30e5, 0xffff41c2, 0xffff54b4,
    0xffff6987, 0xffff8000, 0xffff97e0, 0xffffb0e4, 0xffffcac6, 0xffffe53e
} ;


/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

VOID ClkPaint        (HWND hWind);
VOID ClkDrawHand     (HPS hpsSpace, SHORT sHand, SHORT sMany);
VOID ClkDrawFace     (HPS hpsSpace);
VOID ClkDrawRing     (HPS hpsSpace);
VOID ClkDrawDate     (HPS hpsSpace, USHORT usDate);
VOID ClkDrawTicks    (HPS hpsSpace, USHORT usMany);
VOID DrawDigitalTime (HWND hWind);
VOID GetArrangedDate (CHAR pszDate[]);
VOID DrawTrapez      (HPS ,POINTL *, LONG );
VOID DrawFullRing    (HPS hpsSpace, PPOINTL ppPoint, FIXED fixed1,
                       FIXED fixed2, LONG lSize);


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name: DrawClock()
 *
 *  Purpose:Draw the clock face ,hand and minute hand,
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
VOID DrawClock (HPS hpsDraw)
{
    RECTL rcl;

    WinQueryWindowRect (hwndClient, &rcl);
    WinFillRect (hpsBuffer, &rcl, cp.clrBackground);

    /* draw the face, the hour hand, and the minute hand */

    ClkDrawRing (hpsDraw);
    ClkDrawFace (hpsDraw);
    ClkDrawDate (hpsDraw, DM_REDRAW);
    ClkDrawHand (hpsDraw, HT_HOUR_SHADE, dt.hours);
    ClkDrawHand (hpsDraw, HT_MINUTE_SHADE, dt.minutes);
    ClkDrawHand (hpsDraw, HT_HOUR, dt.hours);
    ClkDrawHand (hpsDraw, HT_MINUTE, dt.minutes);

    /* draw the tick marks */
    if ((cp.usMajorTickPref == CLKTM_ALWAYS) ||
            ((cp.usMajorTickPref == CLKTM_NOTICONIC) && ! fIconic))
        ClkDrawTicks (hpsDraw, CLK_MAJORTICKS);

    if ((cp.usMinorTickPref == CLKTM_ALWAYS) ||
            ((cp.usMinorTickPref == CLKTM_NOTICONIC) && !fIconic))
        ClkDrawTicks (hpsDraw, CLK_MINORTICKS);
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:UpdateScreen()
 *
 *  Purpose: Update the screen area.
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
VOID UpdateScreen (HPS hps, RECTL *prclUpdate)
{
    POINTL aptl[3];

    if (prclUpdate != NULL)
    {
        aptl[0].x = prclUpdate->xLeft;
        aptl[0].y = prclUpdate->yBottom;
        aptl[1].x = prclUpdate->xRight;
        aptl[1].y = prclUpdate->yTop;
        aptl[2].x = prclUpdate->xLeft;
        aptl[2].y = prclUpdate->yBottom;
    }
    else
    {
        WinQueryWindowRect (hwndClient, (PRECTL)aptl);
        aptl[2].x =
        aptl[2].y = 0;
    }

    GpiBitBlt (hps, hpsBuffer, 3L, aptl, ROP_SRCCOPY, 0L);
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkPaint
 *
 *  Purpose: Paint the clock client window.
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
 *  Returns:  VOID
 *
 *
\****************************************************************/
VOID ClkPaint (HWND hwnd)
{
    RECTL rclUpdate;
    HPS hpsWnd;

    if (cp.usDispMode & DM_ANALOG)
    {

        WinBeginPaint (hwnd, hps, &rclUpdate);

        GpiCreateLogColorTable (hps, 0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);

        WinFillRect (hps, &rclUpdate, cp.clrBackground);

        if (fBufferDirty)
        {
            DrawClock (hpsBuffer);
            fBufferDirty = FALSE;
        }

        UpdateScreen (hps, &rclUpdate);

        /*
         * Draw the second hand last, so xor will work.
         */
        if (fShowSecondHand && (cp.usDispMode & DM_SECONDHAND))
            ClkDrawHand(hps, HT_SECOND, dt.seconds);

        WinEndPaint (hps);

    }
    else
    { /*For now, if it is not Analog, it must be digital*/

        hpsWnd = WinBeginPaint (hwnd, NULLHANDLE, &rclUpdate);

        GpiCreateLogColorTable(hpsWnd, 0L, LCOLF_RGB, 0L, 0L, (PLONG) NULL);

        WinFillRect (hpsWnd, &rclUpdate, cp.clrBackground);

        memset (achOldTime, 0, sizeof(achOldTime));
        memset (achOldAmPm, '0', sizeof(achOldAmPm));
        memset (achOldDate, '0', sizeof(achOldDate));

        DrawDigitalTime (hwnd);

        WinEndPaint (hpsWnd);
    }
}


/**************************************************************************\
*                                                                          *
*       ROUTINE:    ClkDrawHand                                            *
*                                                                          *
*       COMMENT:    Draws specified hand at specified hour in given PS     *
*                                                                          *
\**************************************************************************/

VOID ClkDrawHand ( HPS hps , SHORT sHandType , SHORT sAngle )
{
    static POINTL aptlHour [ ] = { { 6 , 0 } , { 0 , 62 } , { -6 , 0 } ,
                                   { 0 , -14 } , { 6 , 0 } } ;
    static POINTL aptlHourLine1 [] = {{0L,-10L},{0L,56}};

    static POINTL aptlMinute [ ] = { { 5 , 0 } , { 0 , 77 } , { -5 , 0 } ,
                                     { 0 , -14 } , { 5 , 0 } } ;
    static POINTL aptlMinuteLine1 [] = {{0L,-15L},{0L,72}};

    static POINTL aptlSecond [ ] = { { 0 , -15 } , { 0 , 74 } } ;
    static POINTL ptlOrigin = {0L,0L};

    static LONG cptlHour = sizeof ( aptlHour ) / sizeof ( POINTL ) ;
    static LONG cptlMinute = sizeof ( aptlMinute ) / sizeof ( POINTL ) ;
    static LONG cptlSecond = sizeof ( aptlSecond ) / sizeof ( POINTL ) ;
    BOOL f ;

    static MATRIXLF matlfModel =
    {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        100L ,                      100L ,                      1L } ;

    static MATRIXLF matlfShade =
    {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        3L ,                     -3L ,                      1L } ;

    /* prepare a rotation transform and set it into the ps */
    /*      cos x    - sin x    0     *\
    |       sin x      cos x    0      |
    \*      100        100      1     */

    matlfModel.fxM11 =
    matlfModel.fxM22 = fxSin[(sAngle + 15) % 60];
    matlfModel.fxM12 = fxSin[(sAngle + 30) % 60];
    matlfModel.fxM21 = fxSin[sAngle];

    f = GpiSetModelTransformMatrix(hps, (LONG)MATLF_SIZE, &matlfModel,
            TRANSFORM_REPLACE);

    /* draw the specified hand */

    switch ( sHandType )
    {

        case HT_HOUR:
            GpiSetColor ( hps , vclrHands[SURFACE] ) ;
            GpiBeginPath ( hps , 1L ) ;
            GpiSetCurrentPosition ( hps , aptlHour ) ;
            GpiPolyLine ( hps , cptlHour , aptlHour ) ;
            GpiEndPath ( hps ) ;
            GpiFillPath ( hps , 1L , FPATH_ALTERNATE ) ;
            GpiSetColor ( hps , vclrHands[SHADE]   ) ;
            GpiSetCurrentPosition ( hps , aptlHour ) ;
            GpiPolyLine ( hps , cptlHour , aptlHour ) ;
            GpiSetColor ( hps , vclrHands[SHADE]   ) ;
            GpiSetCurrentPosition ( hps , aptlHourLine1 ) ;
            GpiPolyLine ( hps , 1L , &(aptlHourLine1[1]) ) ;
            break;
        case HT_HOUR_SHADE:
            GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                              & matlfShade , TRANSFORM_ADD     ) ;
            GpiSetColor ( hps , vclrFace [SHADE]   ) ;
            GpiBeginPath ( hps , 1L ) ;
            GpiSetCurrentPosition ( hps , aptlHour ) ;
            GpiPolyLine ( hps , cptlHour , aptlHour ) ;
            GpiEndPath ( hps ) ;
            GpiFillPath ( hps , 1L , FPATH_ALTERNATE ) ;
            break;

        case HT_MINUTE:
            GpiSetColor ( hps , vclrHands[SURFACE] ) ;
            GpiBeginPath ( hps , 1L ) ;
            GpiSetCurrentPosition ( hps , aptlMinute ) ;
            GpiPolyLine ( hps , cptlMinute , aptlMinute ) ;
            GpiEndPath ( hps ) ;
            GpiFillPath ( hps , 1L , FPATH_ALTERNATE ) ;

            GpiSetColor ( hps , vclrHands[SHADE]   ) ;
            GpiSetCurrentPosition ( hps , aptlMinute ) ;
            GpiPolyLine ( hps , cptlMinute , aptlMinute ) ;
            GpiSetColor ( hps , vclrHands[SHADE]   ) ;
            GpiSetCurrentPosition ( hps , aptlMinuteLine1 ) ;
            GpiPolyLine ( hps , 1L , &(aptlMinuteLine1[1]) ) ;
            GpiSetCurrentPosition ( hps , & ptlOrigin) ;
            GpiFullArc ( hps , DRO_OUTLINEFILL , MAKEFIXED ( 2 , 0 ) ) ;
            break;
        case HT_MINUTE_SHADE:
            GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                              & matlfShade , TRANSFORM_ADD     ) ;
            GpiSetColor ( hps , vclrFace [SHADE]   ) ;
            GpiBeginPath ( hps , 1L ) ;
            GpiSetCurrentPosition ( hps , aptlMinute ) ;
            GpiPolyLine ( hps , cptlMinute , aptlMinute ) ;
            GpiEndPath ( hps ) ;
            GpiFillPath ( hps , 1L , FPATH_ALTERNATE ) ;
            break;

        case HT_SECOND:
            /* draw in XOR mixmode, so we can undraw later */
            GpiSetMix ( hps , FM_INVERT ) ;
            GpiSetCurrentPosition ( hps , aptlSecond ) ;
            GpiPolyLine ( hps , cptlSecond , aptlSecond ) ;
            GpiSetMix ( hps , FM_OVERPAINT ) ;
            break;
    }
}

/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkDrawFace()
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
VOID ClkDrawFace ( HPS hps )
{
    BOOL f ;
    static POINTL ptlLight= { -2L , 2L } ;
    static POINTL ptlShade= {  2L, -2L } ;
    static POINTL ptlFace = { 0L, 0L};
    /*        1         0      0     *\
    *         0         1      0      *
    \*      100       100      1     */

    static MATRIXLF matlfModel = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        100L ,                      100L ,                      1L } ;

    /* center at (100, 100) and draw the clock face */
    f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                     & matlfModel , TRANSFORM_REPLACE ) ;


    GpiSetColor ( hps , vclrFace[SURFACE]);
    GpiSetCurrentPosition ( hps , & ptlFace ) ;
    GpiFullArc ( hps , DRO_OUTLINEFILL , MAKEFIXED ( 80, 0 ) ) ;
}


VOID ClkDrawRing ( HPS hps )
{
    BOOL f ;
    static POINTL ptlLight= { -2L , 2L } ;
    static POINTL ptlShade= {  1L, -1L } ;
    static POINTL ptlFace = { 0L, 0L};
    static POINTL ptlShadeIn= { -3L , 3L } ;
    static POINTL ptlLightIn= {  1L, -1L } ;
    /*        1         0      0     *\
    *         0         1      0      *
    \*      100       100      1     */

    static MATRIXLF matlfModel = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        100L ,                      100L ,                      1L } ;

    /* center at (100, 100) and draw the clock face */
    f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                     & matlfModel , TRANSFORM_REPLACE ) ;

    DrawFullRing(hps,
                    &ptlShade,
                    MAKEFIXED(95,0),
                    MAKEFIXED(99 ,0),
                    vclrRing[SHADE]);

    DrawFullRing(hps,
                    &ptlLight,
                    MAKEFIXED(95,0),
                    MAKEFIXED(98 ,0),
                    vclrRing[LIGHT]   );

    DrawFullRing(hps,
                    &ptlLightIn,
                    MAKEFIXED(88,0),
                    MAKEFIXED(94,0),
                    vclrRing[LIGHT]   );


    DrawFullRing(hps,
                    &ptlShadeIn,
                    MAKEFIXED(86,0),
                    MAKEFIXED(93,0),
                    vclrRing[SHADE]);

    DrawFullRing(hps,
                    &ptlFace ,
                    MAKEFIXED(94,0),
                    MAKEFIXED(98 ,0),
                    vclrRing[SURFACE]);

    GpiSetColor ( hps , vclrFace[SURFACE]);
    GpiSetCurrentPosition ( hps , & ptlFace ) ;
    GpiFullArc ( hps , DRO_OUTLINEFILL , MAKEFIXED ( 91, 0 ) ) ;
}


/**************************************************************************\
*                                                                          *
*       ROUTINE:    ClkDrawDate ( )                                        *
*                                                                          *
*       COMMENT:    Draws Date when analog time is displayed               *
*                                                                          *
\**************************************************************************/
VOID ClkDrawDate (HPS hps, USHORT usDrawMode)
{
    char achFinalDate[9];
    SIZEF sizef;

    static POINTL ptl = { 2 * ZDATE, 3 * ZDATE };
    static POINTL aptlDateShade[ ] = { { 0L , 0L },
                                       { 0L , YDATE },
                                       { XDATE , YDATE },
                                       { XDATE - ZDATE , YDATE - ZDATE },
                                       { ZDATE , YDATE - ZDATE },
                                       { ZDATE, ZDATE }
                                     };
    static POINTL aptlDateLight[ ] = { { 0L , 0L },
                                       { XDATE , 0L },
                                       { XDATE , YDATE },
                                       { XDATE - ZDATE , YDATE - ZDATE },
                                       { XDATE - ZDATE, ZDATE },
                                       { ZDATE, ZDATE }
                                     };
    static POINTL aptlDateBG[] = { {0L ,0L},
                                   {XDATE,YDATE} };

    if ((cp.usDispMode & DM_DATE) && (!fIconic)) {

        GetArrangedDate (achFinalDate);

        GpiSetModelTransformMatrix (hps, (LONG)MATLF_SIZE, &vmatlfDateTrans,
                TRANSFORM_REPLACE);
        GpiSetModelTransformMatrix (hps, (LONG)MATLF_SIZE, &vmatlfDateScale,
                TRANSFORM_ADD);

        if (strncmp(achFinalDate, achOldDate, sizeof(achFinalDate) - 1)) {

            GpiSetColor (hps, vclrDate[BACKGROUND]);
            GpiSetCurrentPosition (hps, &aptlDateBG[0]);
            GpiBox (hps, DRO_FILL, &aptlDateBG[1], 0L, 0L);
            usDrawMode = DM_REDRAW;
        }

        if ((usDrawMode == DM_REDRAW) || (vusDatePos = DP_INSIDE)) {

#ifdef DISABLE
            WinSetRect(NULL, &rclDateString, 2, 2, XDATE - 2,
                       YDATE - 2);
            ClkDrawDigitalString(hps, "88888888", &rclDateString,
                    0x00101010, 0x00808080);

#else

            GpiSetColor (hps, CLR_BLACK);
#ifdef DISABLE
            GpiSetColor (hps, vclrDate[SURFACE]);
#endif
            GetArrangedDate (achFinalDate);
            GpiSetCharMode (hps,CM_MODE3);
            GpiSetCurrentPosition (hps, &ptl);
            sizef.cx = MAKEFIXED (10, 0);
            sizef.cy = MAKEFIXED (YDATE, 0);
            GpiSetCharBox(hps, &sizef);
            GpiCharString (hps, (ULONG)sizeof(achFinalDate), (PSZ)achFinalDate);
#endif

            GpiSetColor (hps, vclrDate[SHADE]);
            GpiSetCurrentPosition (hps, aptlDateShade);
            GpiBeginPath (hps, 1L);
            GpiPolyLine (hps, 5L, &(aptlDateShade[1]));
            GpiCloseFigure (hps);
            GpiEndPath (hps);
            GpiFillPath (hps, 1L, FPATH_ALTERNATE);

            GpiSetColor (hps, vclrDate[LIGHT]);
            GpiSetCurrentPosition (hps, aptlDateLight);
            GpiBeginPath (hps, 1L);
            GpiPolyLine (hps, 5L, &(aptlDateLight[1]));
            GpiCloseFigure (hps);
            GpiEndPath (hps);
            GpiFillPath (hps, 1L, FPATH_ALTERNATE);
        }
    } else {
        /*
         * If we're minimized then we need to update the
         * icontext with the date.
         */
        if (((cp.usDispMode & (DM_TIME | DM_DATE)) == (DM_TIME | DM_DATE)) &&
                (fIconic)) {
            GetArrangedDate (achFinalDate);
            WinSetWindowText (hwndFrame, achFinalDate);
        }
    }
}


/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:ClkDrawTicks()
 *
 *  Purpose:Draws Clock Ticks()
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
VOID ClkDrawTicks ( HPS hps , USHORT usTicks )
{
    BOOL f ;
    USHORT usAngle,usTrapez ;


    /* prepare a transform to use when rotating the ticks */
    /*      cos x    - sin x    0     *\
    |       sin x      cos x    0      |
    \*      100        100      1     */

    static MATRIXLF matlfModel = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        100L ,                      100L ,                      1L } ;

    /* define what the tick marks look like */
    static POINTL aptlMajorTick [ ] = { { -3 , 94 } , { 3 , 100 } } ;
    static BYTE   aclr [12][4] = {
            /*12*/        {SHADE,LIGHT,LIGHT,SHADE},
            /*1*/         {LIGHT,LIGHT,LIGHT,SHADE},
            /*2*/         {LIGHT,LIGHT,SHADE,SHADE},
            /*3*/         {LIGHT,LIGHT,SHADE,SHADE},
            /*4*/         {LIGHT,LIGHT,SHADE,LIGHT},
            /*5*/         {LIGHT,SHADE,SHADE,LIGHT},
            /*6*/         {LIGHT,SHADE,SHADE,LIGHT},
            /*7*/         {LIGHT,SHADE,SHADE,LIGHT},
            /*8*/         {LIGHT,SHADE,LIGHT,LIGHT},
            /*9*/         {SHADE,SHADE,LIGHT,LIGHT},
            /*9*/         {SHADE,SHADE,LIGHT,LIGHT},
            /*11*/        {SHADE,LIGHT,LIGHT,SHADE}
                                 };
    static  POINTL aptlMT  [4][4] = {
                                  { {-3,81},{-1,83},{1,83},{3,81  }} ,
                                  { {-3,81},{-1,83},{-1,87},{-3,89 }} ,
                                  { {-3,89},{-1,87},{1,87},{3,89 }} ,
                                  { {3,89},{1,87},{1,83},{3,81  }}
                                 };
    static POINTL aptlMajorTickShadow [ ] = { { -1 , 83 } , { 1 , 87  } } ;
    static POINTL aptlMinorTick [ ] = { { 0 , 83 } , { 0 , 85 } } ;


    /* have we been asked to draw the major ticks? */
    if ( usTicks & CLK_MAJORTICKS )
        for ( usAngle = 0 ; usAngle < 60 ; usAngle += 5 ) {

            /* set the rotation transform */
            matlfModel . fxM11 =
            matlfModel . fxM22 = fxSin [ ( usAngle + 15 ) % 60 ] ;
            matlfModel . fxM12 = fxSin [ ( usAngle + 30 ) % 60 ] ;
            matlfModel . fxM21 = fxSin [ usAngle ] ;
            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfModel ,
                                             TRANSFORM_REPLACE ) ;

            /* draw a major tick mark */
            for (usTrapez = 0; usTrapez < 4; usTrapez++) {
                DrawTrapez(hps,aptlMT[usTrapez],vclrMajorTicks[aclr[usAngle/5][usTrapez]]);
            }
            GpiSetColor ( hps , vclrMajorTicks[SURFACE]) ;
            GpiSetCurrentPosition ( hps , & aptlMajorTickShadow [ 0 ] ) ;
            GpiBox ( hps , DRO_FILL , & aptlMajorTickShadow [ 1 ] , 0L , 0L ) ;
    }

    /* have we been asked to draw the minor ticks? */
    /* draw in the default color */
    GpiSetColor ( hps ,vclrMinorTicks[SHADE]  ) ;
    if ( usTicks & CLK_MINORTICKS )
        for ( usAngle = 0 ; usAngle < 60 ; usAngle ++ ) {
            if ((usAngle % 5) != 0) {

                matlfModel . fxM11 =
                matlfModel . fxM22 = fxSin [ ( usAngle + 15 ) % 60 ] ;
                matlfModel . fxM12 = fxSin [ ( usAngle + 30 ) % 60 ] ;
                matlfModel . fxM21 = fxSin [ usAngle ] ;

                /* set the transform */
                f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                                 & matlfModel ,
                                                 TRANSFORM_REPLACE ) ;

                /* draw a minor tick mark */
                GpiSetCurrentPosition ( hps , & aptlMinorTick [ 0 ] ) ;
                GpiBox ( hps , DRO_OUTLINEFILL , & aptlMinorTick [ 1 ] , 0L , 0L ) ;
            }
    }
}
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:DrawTrapez()
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
VOID DrawTrapez(HPS hps,POINTL *aptl,LONG color)
{
    GpiSetColor ( hps,color) ;
    GpiBeginPath(hps, 1L);                  /* start the path bracket */
    GpiSetCurrentPosition ( hps ,  aptl  ) ;
    GpiPolyLine(hps, 3L,&(aptl[1]) );      /* draw the three sides   */
    GpiCloseFigure(hps);                    /* close the triangle     */
    GpiEndPath(hps);                        /* end the path bracket   */
    GpiFillPath(hps, 1L, FPATH_ALTERNATE);  /* draw and fill the path */
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
 *          1 - if sucessful execution completed
 *          0 - if error
\****************************************************************/
#ifdef orchik
VOID ClkDrawTicks ( HPS hps , USHORT usTicks )
{
    BOOL f ;
    USHORT usAngle ;

    /* prepare a transform to use when rotating the ticks */
    /*      cos x    - sin x    0     *\
    |       sin x      cos x    0      |
    \*      100        100      1     */

    static MATRIXLF matlfModel = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        100L ,                      100L ,                      1L } ;
    static MATRIXLF matlfLight = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
        -1L ,                       1L ,                      1L } ;
    static MATRIXLF matlfShade = {
        MAKEFIXED ( 1 , 0 ) ,       MAKEFIXED ( 0 , 0 ) ,       0L ,
        MAKEFIXED ( 0 , 0 ) ,       MAKEFIXED ( 1 , 0 ) ,       0L ,
         1L ,                      -1L ,                      1L } ;

    /* define what the tick marks look like */
    static POINTL aptlMajorTick [ ] = { { -2 , 91 } , { 2 , 97  } } ;
    static POINTL aptlMajorTickShade [ ] = { { -2 , 91 } , { 2 , 97  } } ;
    static POINTL aptlMajorTickLight [ ] = { { -2 , 91 } , { 2 , 970 } } ;
    static POINTL aptlMinorTick [ ] = { { 0 , 91 } , { 0 , 92 } } ;

    /* draw in the default color */
    GpiSetColor ( hps , CLR_DEFAULT ) ;

    /* have we been asked to draw the major ticks? */
    if ( usTicks & CLK_MAJORTICKS )
        for ( usAngle = 0 ; usAngle < 60 ; usAngle += 5 ) {

            /* set the rotation transform */
            matlfModel . fxM11 =
            matlfModel . fxM22 = fxSin [ ( usAngle + 15 ) % 60 ] ;
            matlfModel . fxM12 = fxSin [ ( usAngle + 30 ) % 60 ] ;
            matlfModel . fxM21 = fxSin [ usAngle ] ;
            /* draw a major tick mark shade*/
            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfModel ,
                                             TRANSFORM_REPLACE ) ;
            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfShade ,
                                             TRANSFORM_ADD     ) ;
            GpiSetColor (hps, 0x00C0C0C0);
            GpiSetCurrentPosition ( hps , & aptlMajorTickShade [ 0 ] ) ;
            GpiBox ( hps , DRO_FILL , & aptlMajorTick [ 1 ] , 2L , 2L ) ;

            /*Draw light*/

            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfModel ,
                                             TRANSFORM_REPLACE ) ;
            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfLight ,
                                             TRANSFORM_ADD     ) ;
            GpiSetColor(hps, RGB_WHITE);
            GpiSetCurrentPosition ( hps , & aptlMajorTickLight [ 0 ] ) ;
            GpiBox ( hps , DRO_FILL , & aptlMajorTick [ 1 ] , 2L , 2L ) ;

            /*Draw tick mark surface */

            f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                             & matlfModel ,
                                             TRANSFORM_REPLACE ) ;
            GpiSetColor(hps, 0x00808080);
            GpiSetCurrentPosition ( hps , & aptlMajorTick [ 0 ] ) ;
            GpiBox ( hps , DRO_FILL , & aptlMajorTick [ 1 ] , 2L , 2L ) ;
    }

    /* have we been asked to draw the minor ticks? */
    if ( usTicks & CLK_MINORTICKS )
        for ( usAngle = 0 ; usAngle < 60 ; usAngle ++ ) {
            if ((usAngle % 5) != 0) {
                matlfModel . fxM11 =
                matlfModel . fxM22 = fxSin [ ( usAngle + 15 ) % 60 ] ;
                matlfModel . fxM12 = fxSin [ ( usAngle + 30 ) % 60 ] ;
                matlfModel . fxM21 = fxSin [ usAngle ] ;

                /* set the transform */
                f = GpiSetModelTransformMatrix ( hps , ( LONG ) MATLF_SIZE ,
                                                 & matlfModel ,
                                                 TRANSFORM_REPLACE ) ;

                /* draw a minor tick mark */
                GpiSetCurrentPosition ( hps , & aptlMinorTick [ 0 ] ) ;
                GpiLine ( hps , & aptlMinorTick [ 1 ] ) ;
            }
    }
}

#endif
/****************************************************************\
 *
 *--------------------------------------------------------------
 *
 *  Name:DrawFullRing()
 *
 *  Purpose: This routine draws the ring for the clock face.
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
VOID DrawFullRing(HPS hps,PPOINTL pptlCenter,FIXED fxRadIn, FIXED fxRadOut,
                     LONG lColor)
{
    GpiSetColor(hps,lColor);
    GpiSetCurrentPosition ( hps , pptlCenter );
    GpiBeginPath(hps,1L);
    GpiFullArc ( hps , DRO_OUTLINE ,  fxRadIn  ) ;
    GpiFullArc ( hps , DRO_OUTLINE ,  fxRadOut  ) ;
    GpiCloseFigure(hps);
    GpiEndPath(hps);
    GpiFillPath ( hps , 1L , FPATH_ALTERNATE ) ;
}


/*--------------------------------------------------------------*\
 *  End of file :paint.c
\*--------------------------------------------------------------*/

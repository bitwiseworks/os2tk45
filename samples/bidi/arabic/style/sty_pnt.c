/*************************************************************************
*
*  File Name   : STY_PNT.C
*
*  Description :This module contains the code for the
*               main client window painting
*
*  Concepts    : Painting of main client window.
*                BIDI - Added: Bidirectional Language processing sample code.
*
*  API's       : WinBeginPaint
*                WinFillRect
*                WinEndPaint
*
*  Copyright (C) 1992 IBM Corporation
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
************************************************************************/

/*  Include files, macros, defined constants, and externs               */

#define  INCL_WINSYS
#define  INCL_GPI

#include <os2.h>
#include <pmbidi.h>

#include <string.h>
#include "sty_main.h"
#include "sty_xtrn.h"

/*  Global variables                                                    */

/*  Entry point declarations                                            */
static VOID CalcLineHeight(HPS hps, PLONG LineHeight, PLONG Decender );

extern CHAR szPasteText[];

/****************************************************************
 *  Name:   MainPaint
 *
 *  Description : Main client painting routine
 *
 *  Concepts :  Routine is called whenver the client window
 *              procedure receives a WM_PAINT message begins
 *              painting by calling WinBeginPaint and
 *              retriving the HPS for the window performs any
 *              painting desired ends painting by calling
 *              WinEndPaint
 *
 *  API's :  WinBeginPaint
 *           WinFillRect
 *           WinEndPaint
 *
 *  Parameters : hwnd - handle of window to paint
 *
 *  Returns :  VOID
 *
 ****************************************************************/

VOID MainPaint(HWND hwnd)
{
    RECTL  rclUpdate;
    RECTL  rclWnd;
    RECTL  rclDraw;

    HPS hps;
    POINTL pt;
    LONG        LineHeight;
    LONG        Descender;
    PCHAR  pText;
    LONG   ptTextStart;
static
    LONG   Inc[] = { 10, 10, 10,
                     12, 12, 12,
                     14, 14, 14,
                     16, 16, 16,
                     18, 18, 18,
                     20, 20, 20,
                     22, 22, 22,
                     24, 24, 24
                   };

//BIDI
static
    PCHAR   TextLines[] = {
            "~DrawText",             //0
            "CharString",            //1
            "CharStringAt",          //2
            "CharStringPos",         //3
            "CharStringPosAt",       //4
            "DrawText(QueryRect)",   //5
            "QueryCharStringPos",    //6
            "QueryCharStringPosAt",  //7
            "QueryTextBox"           //8
            };

static
    PCHAR   TextLines_API[] = {
            "WinDrawText:",                 //0
            "GpiCharString:",               //1
            "GpiCharStringAt:",             //2
            "GpiCharStringPos:",            //3
            "GpiCharStringPosAt:",          //4
            "WinDrawText(QueryRect):",      //5
            "GpiQueryCharStringPos:",       //6
            "GpiQueryCharStringPosAt:",     //7
            "GpiQueryTextBox:",             //8
            "Current Clipboard TextBuffer:" //9
            };

    ULONG   GpiBidiAttr;
    ULONG   NullBidiAttr = 0L;

    hps = WinBeginPaint(hwnd, NULLHANDLE, (PRECTL)&rclUpdate);

    // Get the bidi attribute of the presentation space, so that
    // we can flip/flip between a NULL bidiattribute and this one
    //
    GpiBidiAttr = GpiQueryBidiAttr(hps);


    /* fill update rectangle with window color */
    WinFillRect(hps, (PRECTL)&rclUpdate, SYSCLR_PAGEBACKGROUND );

    CalcLineHeight(hps, &LineHeight, &Descender);

    GpiSetColor    ( hps, CLR_BLUE ) ;
    GpiSetBackColor( hps, CLR_RED  ) ;

    WinQueryWindowRect(hwnd, (PRECTL)&rclWnd);
    ptTextStart = rclWnd.xRight * 2/5  ;

    // -------------------------------------
    // WinDrawText!
    // -------------------------------------
    pt.x = 0;
    pt.y = rclWnd.yTop - LineHeight + Descender;

    rclDraw.xLeft   = rclWnd.xLeft;
    rclDraw.xRight  = rclWnd.xRight;
    rclDraw.yBottom = pt.y - Descender;
    rclDraw.yTop    = rclDraw.yBottom + LineHeight ;

    GpiSetBidiAttr(hps, NullBidiAttr);

    pText = TextLines_API[0];
    WinDrawText ( hps, strlen(pText), pText, &rclDraw, 0L, 0L,
                  DT_LEFT | DT_ERASERECT | DT_TEXTATTRS | DT_MNEMONIC );

    rclDraw.xLeft = ptTextStart;

    GpiSetBidiAttr(hps, GpiBidiAttr);

    pText = TextLines[0];
    WinDrawText ( hps, strlen(pText), pText, &rclDraw, 0L, 0L,
                  DT_LEFT | DT_ERASERECT | DT_TEXTATTRS | DT_MNEMONIC );

    // -------------------------------------
    // GpiCharString
    // -------------------------------------
    pt.y -=  LineHeight;
    pt.x = 0;
    GpiSetCurrentPosition( hps, &pt ) ;
    pText = TextLines_API[1];
    GpiSetBidiAttr(hps, NullBidiAttr);
    GpiCharString(hps, strlen(pText), pText);

    pt.x = ptTextStart;
    GpiSetCurrentPosition( hps, &pt ) ;
    pText = TextLines[1];
    GpiSetBidiAttr(hps, GpiBidiAttr);
    GpiCharString(hps, strlen(pText), pText);

    // -------------------------------------
    //  GpiCharStringAt
    // -------------------------------------
    pt.y -=  LineHeight ;
    pt.x = 0;
    pText = TextLines_API[2];
    GpiSetBidiAttr(hps, NullBidiAttr);
    GpiCharStringAt(hps, &pt, strlen(pText), pText);

    pt.x = ptTextStart;
    pText = TextLines[2];
    GpiSetBidiAttr(hps, GpiBidiAttr);
    GpiCharStringAt(hps, &pt, strlen(pText), pText);

    // -------------------------------------
    //  GpiCharStringPos
    // -------------------------------------
    pt.y -=  LineHeight ;
    pt.x = 0;
    GpiSetCurrentPosition( hps, &pt ) ;

    rclDraw.xLeft   = rclWnd.xLeft;
    rclDraw.xRight  = ptTextStart - 4;
    rclDraw.yBottom = pt.y - Descender;
    rclDraw.yTop    = rclDraw.yBottom + LineHeight ;

    pText = TextLines_API[3];
    GpiSetBidiAttr(hps, NullBidiAttr);
    GpiCharStringPos(hps, &rclDraw, CHS_VECTOR | CHS_OPAQUE,
                     strlen(pText), pText, &Inc[0] );

    rclDraw.xLeft   = ptTextStart;
    rclDraw.xRight  = rclWnd.xRight;
    rclDraw.yBottom = pt.y - Descender  ;
    rclDraw.yTop    = rclDraw.yBottom + LineHeight ;

    pt.x            = ptTextStart;
    GpiSetCurrentPosition( hps, &pt ) ;

    pText = TextLines[3];
    GpiSetBidiAttr(hps, GpiBidiAttr);
    GpiCharStringPos(hps, &rclDraw, CHS_VECTOR | CHS_OPAQUE,
                     strlen(pText), pText, &Inc[0]);


    // -------------------------------------
    //  GpiCharStringPosAt
    // -------------------------------------
    pt.y -=  LineHeight ;
    pt.x = 0;

    rclDraw.xLeft   = rclWnd.xLeft;
    rclDraw.xRight  = ptTextStart - 4;
    rclDraw.yBottom = pt.y - Descender  ;
    rclDraw.yTop    = rclDraw.yBottom + LineHeight ;

    pText = TextLines_API[4];
    GpiSetBidiAttr(hps, NullBidiAttr);
    GpiCharStringPosAt(hps, &pt, &rclDraw, CHS_VECTOR | CHS_OPAQUE,
                       strlen(pText), pText, &Inc[0] );

    rclDraw.xLeft   = ptTextStart;
    rclDraw.xRight  = rclWnd.xRight;
    rclDraw.yBottom = pt.y - Descender  ;
    rclDraw.yTop    = rclDraw.yBottom + LineHeight ;
    pt.x            = ptTextStart;

    pText = TextLines[4];
    GpiSetBidiAttr(hps, GpiBidiAttr);
    GpiCharStringPosAt(hps, &pt, &rclDraw, CHS_VECTOR | CHS_OPAQUE,
                       strlen(pText), pText, &Inc[0]);

    // -------------------------------------
    //  Current Clipboard TextBuffer
    //
    //  This is used as a means of "looking inside" the
    //  clipbaord. (This is a clipboard viewer for the poor...)
    //  This part shows the text in szPasteText.
    //  To operate:
    //  - Copy/Cut text from anywhere to the clipbaord.
    //  - select:    Edit --> Paste --> No Conversion
    //  - The clipboard text will be shown (UNMODIFIED BUFFER)
    //    in our window.
    // -------------------------------------
    pt.y -=  LineHeight ;
    pt.x = 0;

    pText = TextLines_API[9];
    GpiSetBidiAttr(hps, NullBidiAttr);
    GpiCharStringAt(hps, &pt, strlen(pText), pText);

    pt.x            = ptTextStart;
    pText           = szPasteText;
    GpiCharStringAt(hps, &pt, strlen(pText), pText);

//BIDI- end

    /*
     * Add painting routines here.  Hps is now the HPS for
     * the window and rclUpdate contains the update rectangle.
     */

    WinEndPaint(hps);

}   /* MainPaint() */




static VOID CalcLineHeight(HPS hps, PLONG plLineHeight, PLONG plDescender )
{
    FONTMETRICS FontMetrics;
    GpiQueryFontMetrics(hps, sizeof(FontMetrics), &FontMetrics );

    *plLineHeight =  FontMetrics.lMaxBaselineExt  +
                     FontMetrics.lExternalLeading +
                     FontMetrics.lInternalLeading ;
    *plDescender  =  FontMetrics.lMaxDescender    +
                     FontMetrics.lExternalLeading +
                     FontMetrics.lInternalLeading ;
    return;

}   /*CalcLineHeight() */


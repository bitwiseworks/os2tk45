/******************************************************************************
*
*  File Name   : GLOBALS.H
*
*  Description : global data header for Jigsaw
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
******************************************************************************/

/* ------------------------ general definitions ----------------------------- */

extern HAB      habMain;          /* main thread anchor block handle*/
extern HMQ      hmqMain;          /* main thread queue handle*/
extern HWND     hwndFrame;        /* frame control handle*/
extern HWND     hwndClient;       /* client area handle*/
extern HWND     hwndMenu;         /* Menu handle*/
extern HDC      hdcClient;        /* window dc handle*/
extern HPS      hpsClient;        /* client area Gpi ps handle*/
extern SIZEL    sizlMaxClient;    /* max client area size*/
extern HPS      hpsPaint;         /* ps for use in Main Thread*/
extern HRGN     hrgnInvalid;      /* handle to the invalid region*/

extern HAB      habAsync;         /* async thread anchor block handle*/
extern HMQ      hmqAsync;         /* async thread queue handle*/
extern TID      tidAsync;         /* async thread id*/
extern SEL      selStack;         /* async thread stack selector*/
extern SHORT    sPrty;            /* async thread priority*/

extern HWND     hwndHorzScroll;   /* horizontal scroll bar window*/
extern HWND     hwndVertScroll;   /* vertical scroll bar window*/

extern POINTS   ptsScrollPos;
extern POINTS   ptsOldScrollPos;
extern POINTS   ptsScrollMax;
extern POINTS   ptsHalfScrollMax;
extern POINTS   ptsScrollLine;
extern POINTS   ptsScrollPage;

extern MATRIXLF matlfIdentity;
extern LONG     lScale;           /* current zoom level*/
extern POINTL   ptlScaleRef;      /* scalefactor, detects size change*/

extern POINTL   ptlOffset;
extern POINTL   ptlBotLeft;
extern POINTL   ptlTopRight;
extern POINTL   ptlMoveStart;     /* model space point at start of move*/
extern LONG     lLastSegId;       /* last segment id assigned to a piece*/
extern LONG     lPickedSeg;       /* seg id of piece selected for drag*/
extern POINTL   ptlOffStart;      /* segment xform xlate at move start*/
extern RECTL    rclBounds;        /* pict bounding box in model coords.*/
extern POINTL   ptlOldMouse;      /* current mouse posn*/
extern POINTL   ptlMouse;         /* current mouse posn*/
extern BOOL     fButtonDownMain;  /* only drag if mouse down*/
extern BOOL     fButtonDownAsync; /* only drag if mouse down*/

extern POINTL   ptlUpdtRef;
extern POINTL   aptlUpdt[3];
extern BOOL     fUpdtFirst;
extern BOOL     fFirstLoad;
extern BOOL     fPictureAssembled;

/* -------------------------- segment list --------------------------------- */

extern PSEGLIST pslHead;          /* head of the list*/
extern PSEGLIST pslTail;          /* tail of the list*/
extern PSEGLIST pslPicked;        /* picked segment's list member*/

/*-------------------------- bitmap-related data --------------------------- */

extern LOADINFO           li;
extern PLOADINFO          pli;

extern HPS                hpsBitmapFile;      /* bitmap straight from the file*/

extern HDC                hdcBitmapFile;
extern HBITMAP            hbmBitmapFile;

extern BITMAPINFOHEADER2  bmp2BitmapFile;
extern PBITMAPINFOHEADER2 pbmp2BitmapFile;
extern BITMAPINFOHEADER2  bmp2BitmapFileRef;
extern PBITMAPINFOHEADER2 pbmp2BitmapFileRef;

extern HPS                hpsBitmapSize;      /* bitmap sized to the current*/
                                              /* size*/
extern HDC                hdcBitmapSize;
extern HBITMAP            hbmBitmapSize;

extern HPS                hpsBitmapBuff;      /* image composed here, copied to*/
                                              /* screen*/
extern HDC                hdcBitmapBuff;
extern HBITMAP            hbmBitmapBuff;

extern HPS                hpsBitmapSave;      /* save part of screen during*/
                                              /* dragging*/
extern HDC                hdcBitmapSave;
extern HBITMAP            hbmBitmapSave;

extern BITMAPINFOHEADER2  bmp2BitmapSave;
extern PBITMAPINFOHEADER2 pbmp2BitmapSave;

extern DEVOPENSTRUC       dop;

/* -------------------------- Miscellaneous ------------------------------- */

/*
 *   These event semaphores are loosely used to signal events between Jigsaw's
 *   two threads.  No DosWaitEventSem() calls are made at this time, since,
 *   without reworking the code, this could block out a thread that should
 *   continually process the message queue.  Semaphores are "Reset" while
 *   being used (to signal event) and "Posted" when the event is over.
 */

extern HEV hevTerminate;    /* event semaphore - terminating Jigsaw...        */
extern HEV hevDrawOn;       /* enable/disable drawing                         */
extern HEV hevMouse;        /* enable/disable mouse tracking                  */
extern HEV hevLoadingBitmap;/* event semaphore - loading a bitmap file        */
extern HEV hevKillDraw;     /* event semaphore - stop drawing                 */

extern PSZ     pszBlankMsg;

extern SWCNTRL swctl;
extern HSWITCH hsw;                         /* handle to a switch list entry  */
extern char    szTitle[80];                 /* Title bar text                 */
extern char    szErrorTitle[80];            /* error Title bar text           */

extern BOOL    fErrMem;                     /* set if alloc async stack fails */

extern LONG    lByteAlignX, lByteAlignY;    /* memory alignment constants     */
extern BOOL    BmpLoaded;

extern USHORT  Currentchecked;


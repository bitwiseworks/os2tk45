/**************************************************************************
 *  File name  :  clipbrd.c
 *
 *  Description:  This program displays a standard window containing a
 *                picture.  The action bar contains the choices Edit and
 *                Help.  The Edit pull-down contains choices that let the
 *                user perform clipboard operations on the picture.
 *
 *                This source file contains the following functions:
 *
 *                  main( VOID );
 *                  ClipbrdHelp ( HWND, USHORT );
 *                  ClearRoutine( VOID );
 *                  CopyRoutine( VOID );
 *                  CutRoutine( VOID );
 *                  DrawRect( VOID );
 *                  EraseRect( VOID );
 *                  ExitRoutine( HWND );
 *                  Finalize( VOID );
 *                  GetBitmap( VOID );
 *                  Initialize( VOID );
 *                  MenuInitialization( MPARAM, MPARAM);
 *                  MouseButtonDown( HWND, MPARAM);
 *                  PaintRoutine( HWND );
 *                  PasteRoutine( VOID );
 *                  RemoveRect( VOID );
 *                  Report_Error( HAB );
 *                  WindowDestroy( VOID );
 *                  WindowInitialization( HWND );
 *                  ReportAPIError( PSZ );
 *                  ProductInfoDlgProc( HWND, ULONG, MPARAM, MPARAM );
 *                  HelpProductInfo( HWND );
 *                  SetSysMenu( HWND );
 *
 *  Concepts   :  clipboard
 *
 *  API's      :  DevOpenDC
 *                DevCloseDC
 *
 *                DosExit
 *
 *                GpiAssociate
 *                GpiBox
 *                GpiBitBlt
 *                GpiCreateBitmap
 *                GpiCreatePS
 *                GpiDestroyPS
 *                GpiDeleteBitmap
 *                GpiLoadBitmap
 *                GpiQueryBitmapParameters
 *                GpiQueryColor
 *                GpiQueryLineType
 *                GpiSetCurrentPosition
 *                GpiSetColor
 *                GpiSetLineType
 *                GpiSetBitmap
 *
 *                WinCreateStdWindow
 *                WinCreateMsgQueue
 *                WinCloseClipbrd
 *                WinDispatchMsg
 *                WinDefDlgProc
 *                WinDefWindowProc
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *                WinDlgBox
 *                WinFillRect
 *                WinFreeErrorInfo
 *                WinGetMsg
 *                WinGetErrorInfo
 *                WinIsRectEmpty
 *                WinInitialize
 *                WinLoadString
 *                WinMessageBox
 *                WinOpenClipbrd
 *                WinOpenWindowDC
 *                WinPostMsg
 *                WinQuerySysValue
 *                WinQueryClipbrdData
 *                WinQuerySysPointer
 *                WinQueryWindowText
 *                WinQueryWindowRect
 *                WinQueryUpdateRect
 *                WinRegisterClass
 *                WinSendMsg
 *                WinSetFocus
 *                WinSetPointer
 *                WinSetClipbrdData
 *                WinSetRect
 *                WinSetWindowText
 *                WinTerminate
 *                WinTrackRect
 *                WinValidateRect
 *                WinWindowFromID
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, CLIPBRD.H
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/

#define INCL_DEV
#define INCL_WINWINDOWMGR
#define INCL_WINDIALOGS
#define INCL_WININPUT
#define INCL_WINRECTANGLES
#define INCL_WINACCELERATORS
#define INCL_WINPOINTERS
#define INCL_WINSETERRORINFO
#define INCL_WINMESSAGEMGR
#define INCL_WINBUTTONS
#define INCL_WINEDITCTLS
#define INCL_WINLISTBOXES
#define INCL_WINMENUS
#define INCL_WINSCROLLBARS
#define INCL_WINFRAMEMGR
#define INCL_WINFRAMECTLS
#define INCL_WINSYS
#define INCL_WINTRACKRECT
#define INCL_WINCLIPBOARD
#define INCL_GPICONTROL
#define INCL_GPITRANSFORMS
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_GPICORRELATION
#define INCL_GPILCIDS
#define INCL_GPIBITMAPS
#define INCL_ERRORS
#define INCL_DOSPROCESS
#include <os2.h>
#include <string.h>
#include "clipbrd.h"

/*----------------- various window-dependent globals -------------------------*/

HAB     habMain;                       /* Anchor block handle                 */
HMQ     hmqMain;                       /* Application queue handle            */
HWND    hwndFrame=0L;                /* Frame control handle                */
HWND    hwndClient;                    /* Client area handle                  */
HPS     hpsClient=0L;                /* Client area gpi ps handle           */
HDC     hdcClient;                     /* Window dc handle                    */
HACCEL haccelMain;                     /* Accelerator table handle            */
HACCEL haccelTrack;                    /* Tracking rect accelerator table     */

/*-------------------------- memory bit map globals --------------------------*/

HDC     hdcMemory;
HBITMAP bmapMemory;
HPS     hpsMemory;

DRIVDATA    drvdata = {44L, 0L, "DISPLAY", 0L};

  struct {
    PSZ   address;
    PSZ   driver_name;
    PSZ   driver_data;
    PSZ   data_type;
    PSZ   comment;
    PSZ   proc_name;
    PSZ   proc_params;
    PSZ   spl_params;
    PSZ   network_params;
         }

    dcdatablk = {0L, "DISPLAY", 0L, 0L, 0L, 0L, 0L, 0L, 0L};

BITMAPINFOHEADER2 bmpMemory;           /* bitmap information header for
                                          create bitmap API                   */
POINTL  aptlCorners[4];                /* rectangle corners for BitBlting     */


/*---------------------- tracking rectangle info structure -------------------*/

TRACKINFO tiStruct;

/*----------------- only process WM_PAINT when we are ready ------------------*/

BOOL fPaintable = FALSE;

/*----------------------- Line attribute bundle ------------------------------*/

LINEBUNDLE lbndLineAttr = {(LONG)0,         /*  lColor;       */
                           (LONG)0,         /*  lBackColor;   */
                           FM_XOR,          /*  usMixMode;    */
                           (USHORT)0,       /*  usBackMixMode */
                           (FIXED)0,        /*  fxWidth;      */
                           (LONG)0,         /*  lGeomWidth;   */
                           (USHORT)0,       /*  usType;       */
                           (USHORT)0,       /*  usEnd;        */
                           (USHORT)0};      /*  usJoin;       */


/*---------------------------- loaded strings --------------------------------*/

char szTitleBar[CCHMAXSTRING];
char szTerminate[CCHMAXSTRING];
char szHelp[CCHMAXSTRING];
char szDefErrMesg[]  = "No further infomation";  /* message when no other     */
char szApiErrTitle[] = "API Error Return";       /* title for error message   */
LONG Status = SUCCESSX;
BOOL fErrMem=FALSE;

/*
 * Function prototypes
 */

MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

/**************************************************************************
 *
 *  Name       : main()
 *
 *  Description: Initializes the PM environment and processes
 *               the application message queue until a WM_QUIT
 *               message is received.  It then destroys all OS/2
 *               PM services and terminates.
 *
 *  Concepts   : - obtains anchor block handle and creates message
 *                   queue
 *               - calls the initialization routine
 *               - creates the main frame window which creates the
 *                   main client window
 *               - polls the message queue via Get/Dispatch Msg loop
 *               - upon exiting the loop, exits
 *
 *  API's      :   WinInitialize
 *                 WinCreateMsgQueue
 *                 WinGetMsg
 *                 WinDispatchMsg
 *                 WinTerminate
 *
 *  Parameters :  [none]
 *
 *  Return     :  1 - if successful execution completed
 *                0 - if error
 *
 *************************************************************************/
int main()
{
   QMSG qmsgMain;                             /* application queue structure */

   habMain = WinInitialize(0L);
   hmqMain = WinCreateMsgQueue(habMain,0);

  /*
   * Test to see if the queue handle is 0L. If it is, it probably
   * means that the program has been installed in Desktop Manager as
   * VIO or Full Screen. In this case, the WinInitialize works,
   * but creating the message queue will fail and if you continue
   * with anything else you will get invalid selectors.
   * You can't even put up a message box so the program will
   * not start and the user will not know why!
   */
   if (hmqMain != (HMQ)0L)
   {
      if (!Initialize())
      {
         Report_Error(habMain);
         Finalize();
      }

      while (WinGetMsg(habMain, &qmsgMain, 0L, 0,0))
         WinDispatchMsg(habMain, &qmsgMain);

      Finalize();
   }
   else
   {
       WinTerminate(habMain);
   }
   return SUCCESSX;
}   /* end of main  */

/**************************************************************************
 *
 *  Name       : ClientWndProc
 *
 *  Description: This is the window procedure associated with the client
 *               area in the standard frame window.  It processes all
 *               messages either sent or posted to the client area,
 *               depending on the message command and parameters.
 *
 *  Concepts   : messaging
 *
 *  API's      : WinMessageBox
 *               WinDefWindowProc
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message i.d.
 *                mp1  - first message parameter
 *                mp2  - second message parameter
 *
 *  Return     :  depends on type of message processed
 *
 *************************************************************************/
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch (msg)
   {
   case WM_CREATE:
      WindowInitialization(hwnd);
      break;

   case WM_DESTROY:
      WindowDestroy();
      break;

   case WM_HELP:
      WinMessageBox(HWND_DESKTOP,
                    hwnd,
                   (PSZ)szHelp,
                   (PSZ)szTitleBar,
                    0,
                    MB_MOVEABLE | MB_CUANOTIFICATION | MB_OK
                   );

      break;

   case WM_PAINT:
      PaintRoutine(hwnd);
      break;

   case WM_CLOSE:
      ExitRoutine(hwnd);
      break;

   case WM_INITMENU:
      MenuInitialization(mp1,mp2);
      break;

    /*
     * process menu item commands
     */
   case WM_COMMAND:
      switch (LOUSHORT(mp1))
      {
      case MENU_EXTHELP:
         ClipbrdHelp( hwnd, EXTHELP );
         break;
      case MENU_HELPINDEX:
         ClipbrdHelp( hwnd, HELPINDEX );
         break;
      case MENU_KEYSHELP:
         ClipbrdHelp( hwnd, KEYSHELP );
         break;
      case MENU_HELPUSING:
         ClipbrdHelp( hwnd, GENHELP );
         break;
      case MENU_PRODINFO:
         HelpProductInfo( hwnd );
         break;

      case MENU_COPY:
         CopyRoutine();
         break;
      case MENU_PASTE:
         PasteRoutine();
         break;
      case MENU_CUT:
         CutRoutine();
         break;
      case MENU_CLEAR:
         ClearRoutine();
         break;
        /*
         * Unrecognized WM_COMMAND IDs are processed in a default manner.
         */
      default:
         return(WinDefWindowProc(hwnd, msg, mp1, mp2));
      }
      break;

    /*
     * Clear an existing rectangle from the screen, and set the process into
     * track rectangle mode loop, after setting up the required parameters
     */
   case WM_BUTTON1DOWN:
      MouseButtonDown(hwnd,mp1);
      break;


   default:
      return(WinDefWindowProc(hwnd, msg, mp1, mp2));
   }
   return (MRESULT)0;
}  /*  end of ClientWndProc  */

/**************************************************************************
 *
 *  Name       : ClearRoutine
 *
 *  Description: erases a bounding rectangle from the screen
 *
 *  Concepts   :
 *
 *  API's      : WinIsRectEmpty
 *               WinSetRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ClearRoutine()
{
    /*
     * If a bounding rectangle has been defined, erase that part of the
     * picture from the screen and the memory bit map.
     */
   if(!WinIsRectEmpty(habMain, &tiStruct.rclTrack))
   {
      EraseRect();
      WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
   } /* endif */
   return;
}  /*  end of ClearRoutine */

/**************************************************************************
 *
 *  Name       : ClipbrdHelp
 *
 *  Description: displays the appropriate help string
 *
 *  Concepts   : help
 *
 *  API's      : WinLoadString
 *               WinMessageBox
 *
 *  Parameters : hwnd  = window handle
 *               HelpId = help string i.d.
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ClipbrdHelp(HWND hwnd, USHORT HelpId)
{
   CHAR szTemp[CCHMAXSTRING];

   /*
    * Get the appropriate help string.
    */
   if (!WinLoadString(habMain, (HMODULE)0L, HelpId, (SHORT)CCHMAXSTRING,
                          (PSZ)szTemp))
      ReportAPIError("Error loading Help String");

   /*
    * Display the help string.
    */
   WinMessageBox(HWND_DESKTOP,
                  hwnd,
                  (PSZ)szTemp,
                  (PSZ)szTitleBar,
                  0,
                  MB_MOVEABLE | MB_CUANOTIFICATION | MB_OK );
   return;
}  /*  end of ClipbrdHelp  */

/**************************************************************************
 *
 *  Name       : CopyRoutine
 *
 *  Description: copies a rectangle into a fresh bitmap
 *
 *  Concepts   :
 *
 *  API's      : WinOpenClipbrd
 *               WinSetClipbrdData
 *               WinCloseClipbrd
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID CopyRoutine()
{
   HBITMAP bmapTempMem;
    /*
     * If a bounding rectangle has been defined, copy that part of the
     * memory bit map into a newly-created bitmap.
     * Give the new bit map to the clipboard. Finally, clear the
     * bounding rectangle.
     */
   bmapTempMem = GetBitmap();
   if(bmapTempMem != (HBITMAP)0L)
   {
      WinOpenClipbrd(habMain);
      WinSetClipbrdData(habMain, (ULONG)bmapTempMem, CF_BITMAP, 0L);
      WinCloseClipbrd(habMain);
      RemoveRect();
   } /* endif */
   return;
}  /*  end of CopyRoutine  */

/**************************************************************************
 *
 *  Name       : CutRoutine
 *
 *  Description: cuts a rectangle to the clipboard and removes it from the
 *               window
 *
 *  Concepts   : clipboard
 *
 *  API's      : WinOpenClipbrd
 *               WinSetClipbrdData
 *               WinCloseClipbrd
 *               WinSetRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID CutRoutine()
{
  HBITMAP bmapTempMem;
    /*
     * If a bounding rectangle has been defined, copy that part of the
     * memory bit map into a newly created bitmap.
     * Give the new bitmap to the clipboard. Finally clear the
     * bounding rectangle and erase that part of the picture from the
     * screen and the memory bit map.
     */
   bmapTempMem = GetBitmap();
   if (bmapTempMem != (HBITMAP)0L)
   {
      WinOpenClipbrd(habMain);
      WinSetClipbrdData(habMain, (ULONG)bmapTempMem, CF_BITMAP, 0L);
      WinCloseClipbrd(habMain);
      EraseRect();
      WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
   } /* endif */
   return;
}  /*  end of CutRoutine  */

/**************************************************************************
 *
 *  Name       : DrawRect
 *
 *  Description: Draw a rectangle on the screen, using the last position
 *               and size of the tracking rectangle.
 *
 *  Concepts   :
 *
 *  API's      : GpiQueryLineType
 *               GpiSetLineType
 *               GpiQueryColor
 *               GpiSetColor
 *               GpiSetCurrentPosition
 *               GpiBox
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID DrawRect()
{
   POINTL ptlWork;
   LONG lSaveType;
   LONG lSaveColor;

   lSaveType = GpiQueryLineType(hpsClient);
   GpiSetLineType(hpsClient, LINETYPE_DOT);

   lSaveColor = GpiQueryColor(hpsClient);
   GpiSetColor(hpsClient,CLR_RED);

   ptlWork.x = tiStruct.rclTrack.xLeft;
   ptlWork.y = tiStruct.rclTrack.yBottom;
   GpiSetCurrentPosition(hpsClient, &ptlWork);

   ptlWork.x = tiStruct.rclTrack.xRight;
   ptlWork.y = tiStruct.rclTrack.yTop;
   GpiBox(hpsClient, DRO_OUTLINE, &ptlWork, 0L, 0L);

   GpiSetLineType(hpsClient, lSaveType);
   GpiSetColor(hpsClient,lSaveColor);
   return;
} /*  end of DrawRect  */

/**************************************************************************
 *
 *  Name       : EraseRect
 *
 *  Description: Erase a rectangle from the screen and from the memory
 *               bit map
 *
 *  Concepts   :
 *
 *  API's      : WinFillRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID EraseRect()
{
   RECTL rclErase;

   rclErase = tiStruct.rclTrack;
   WinFillRect(hpsMemory, &rclErase, CLR_WHITE);
   RemoveRect();
   return;
}  /*  end of EraseRect  */

/**************************************************************************
 *
 *  Name       : ExitRoutine
 *
 *  Description: called when PF3 is pushed, when Close is selected from
 *               the System Menu, or when Exit is selected
 *
 *  Concepts   :
 *
 *  API's      : WinFillRect
 *
 *  Parameters : hwnd = window handle
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ExitRoutine(HWND hwnd)
{
   WinPostMsg(hwnd, WM_QUIT, (MPARAM)0, (MPARAM)0);
   return;
}   /*  end of ExitRoutine  */

/**************************************************************************
 *
 *  Name       : Finalize
 *
 *  Description: destroy all Presentation Manager resources and end the
 *               process
 *
 *  Concepts   : termination
 *
 *  API's      : WinDestroyWindow
 *               WinDestroyMsgQueue
 *               WinTerminate
 *               DosExit
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID Finalize( VOID )
{
   WinDestroyWindow(hwndFrame);
   WinDestroyMsgQueue(hmqMain);
   WinTerminate(habMain);
   DosExit( Status, 0);
   return;
}   /*  end of Finalize  */

/**************************************************************************
 *
 *  Name       : GetBitmap
 *
 *  Description: If a bounding rectangle has been defined, create a screen
 *               compatible device context and a screen compatible bit map.
 *               Select the bit map into the DC, create a
 *               suitably sized GPI PS and associate it with the DC.
 *               BitBlt that part of the
 *               screen defined by the rectangle into the new bit map.
 *               Destroy all other resources but return the bit map handle
 *               (to be passed to the clipboard)
 *
 *  Concepts   :
 *
 *  API's      :  WinIsRectEmpty
 *                WinQuerySysPointer
 *                WinSetPointer
 *                DevOpenDC
 *                GpiCreatePS
 *                GpiCreateBitmap
 *                GpiSetBitmap
 *                GpiBitBlt
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *
 *  Parameters : [none]
 *
 *  Return     : bitmap handle
 *
 *************************************************************************/
HBITMAP GetBitmap()
{
   HDC   hdc;
   HPS   hps;
   HBITMAP bmap;
   register HPOINTER tptr;

   SIZEL sizlWork;

   if(WinIsRectEmpty(habMain, &tiStruct.rclTrack))
   {
      return((HBITMAP)0L);
   } /* endif */

   if ( (tptr = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE)) == 0 )
      ReportAPIError("Failed to get System pointer handle");
   WinSetPointer(HWND_DESKTOP, tptr);
  /*
   * It is a bounding rectangle, so we only want what is inside.
   */
   sizlWork.cx = tiStruct.rclTrack.xRight - tiStruct.rclTrack.xLeft - 1;
   sizlWork.cy = tiStruct.rclTrack.yTop - tiStruct.rclTrack.yBottom - 1;

   hdc = DevOpenDC(habMain,
                  OD_MEMORY,
                 (PSZ)"*",
                  8L,
                 (PDEVOPENDATA)&dcdatablk,
                 (HDC)0L
                 );
   if (hdc == 0) ReportAPIError("DevOpenDC Failure");

   bmpMemory.cbFix           = sizeof(bmpMemory);
   bmpMemory.cx              = (USHORT)sizlWork.cx;
   bmpMemory.cy              = (USHORT)sizlWork.cy;
   bmpMemory.cPlanes         = 1L;
   bmpMemory.cBitCount       = 24L;
   bmpMemory.ulCompression   = BCA_UNCOMP;
   bmpMemory.cxResolution    = 0;
   bmpMemory.cyResolution    = 0;
   bmpMemory.cclrUsed        = 0;
   bmpMemory.cclrImportant   = 0;
   bmpMemory.usUnits         = BRU_METRIC;
   bmpMemory.usRecording     = BRA_BOTTOMUP;
   bmpMemory.usRendering     = BRH_NOTHALFTONED;
   bmpMemory.cSize1          = 0;
   bmpMemory.cSize2          = 0;
   bmpMemory.ulColorEncoding = 0;
   bmpMemory.ulIdentifier    = 0;

   hps = GpiCreatePS(habMain, hdc, &sizlWork, GPIA_ASSOC | PU_PELS);
   if (hps == 0)
   {
     ReportAPIError("GpiCreatePS Failure");
   }

   bmap = GpiCreateBitmap(hps,
           (PBITMAPINFOHEADER2)&bmpMemory,
           0L,
           (PBYTE)NULL,
           (PBITMAPINFO2)NULL
            );
   if (bmap ==0)
   {
      ReportAPIError("GpiCreateBitmap Failure");
   }

   GpiSetBitmap(hps, bmap);

  /*
   * No adjustments to the top and right top (using lengths) since the
   * rectangles used for the BitBlt are non-inclusive (the bottom and
   * left are included, but not the top and right)
   */
   aptlCorners[0].x = 0;
   aptlCorners[0].y = 0;
   aptlCorners[1].x = sizlWork.cx;
   aptlCorners[1].y = sizlWork.cy;
  /*
   * Want to copy from inside the bounding rect. Increase the left & bottom
   * by one but leave the top & right since the rectangles used for the BitBlt
   * are non-inclusive (the bottom and left are included, but not the
   * top and right).
   */
   aptlCorners[2].x = tiStruct.rclTrack.xLeft + 1;
   aptlCorners[2].y = tiStruct.rclTrack.yBottom + 1;
   aptlCorners[3].x = tiStruct.rclTrack.xRight;
   aptlCorners[3].y = tiStruct.rclTrack.yTop;
   GpiBitBlt(hps, hpsMemory, 4L, aptlCorners, ROP_SRCCOPY, BBO_IGNORE);

   GpiSetBitmap(hps, (HBITMAP)0L);
   GpiAssociate(hps, (HDC)0L);
   GpiDestroyPS(hps);
   DevCloseDC(hdc);
   WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP,
                                                 SPTR_ARROW,
                                                 FALSE)
                                                 );
   return(bmap);
}  /*  end of GetBitmap  */

/**************************************************************************
 *
 *  Name       : Initialize
 *
 *  Description: initialize the PM interface,
 *               create an application message queue and a
 *               standard frame window
 *
 *  Concepts   :
 *
 *  API's      : WinLoadString
 *               WinRegisterClass
 *               WinSetPointer
 *               WinQuerySysPointer
 *               WinCreateStdWindow
 *               WinQueryWIndowText
 *               WinSetWindowText
 *
 *  Parameters : [none]
 *
 *  Return     : TRUE = successful initialization
 *               FALSE = error occurred
 *
 *************************************************************************/
BOOL Initialize()
{
   ULONG flCreate;
   BOOL  fErrTmp=FALSE;

   fErrMem = (BOOL)!WinLoadString(habMain,
                                (HMODULE)0L,
                                (USHORT)TERMINATE,
                                (SHORT)CCHMAXSTRING,
                                (PSZ)szTerminate
                                );
   fErrTmp = (BOOL)!WinLoadString(habMain,
                                (HMODULE)0L,
                                (USHORT)HELPPANEL,
                                (SHORT)CCHMAXSTRING,
                                (PSZ)szHelp
                                );
   if (fErrMem || fErrTmp)
      return(FALSE);

   if (!WinRegisterClass(habMain,
                         (PSZ)"My Class",
                         (PFNWP)ClientWndProc,
                         CS_SIZEREDRAW,
                         0))

      return(FALSE);

   WinSetPointer(HWND_DESKTOP,
                WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));

   flCreate = FCF_STANDARD;
   hwndFrame = WinCreateStdWindow(HWND_DESKTOP,
                                 WS_VISIBLE,
                                 &flCreate,
                                 (PSZ)"My Class",
                                 (PSZ)NULL,
                                 WS_VISIBLE,
                                 (HMODULE)0L,
                                 AMENU,
                                 &hwndClient
                                 );

   WinSetWindowText(hwndFrame, "CLIPBOARD SAMPLE");

   WinSetPointer(HWND_DESKTOP,
                WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));

   if (hwndFrame == (HWND)0L)
   {
      return(FALSE);
   } /* endif */

  /*
   * Get the window title given to the window by the system
   */
   WinQueryWindowText(hwndFrame, CCHMAXSTRING, (PSZ)szTitleBar);

   return(TRUE);
}  /*  end of Initialize  */

/**************************************************************************
 *
 *  Name       : MenuInitialization
 *
 *  Description: set beginning attributes (enabled or not) of menu items
 *
 *  Concepts   : menu
 *
 *  API's      : WinIsRectEmpty
 *               WinSendMsg
 *               WinOpenClipbrd
 *               WinQueryClipbrdData
 *               WinCloseClipbrd
 *
 *  Parameters : mp1 = first message parameter
 *               mp2 = second message parameter
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MenuInitialization(MPARAM mp1, MPARAM mp2)
{
   BOOL    fEmptyRect;
    /*
     * Menu items in the Edit pull-down may need to be grayed, depending on
     * the presence of a rectangle and the contents of the clipboard.
     */
   if (LOUSHORT(mp1) == SM_CLIPBRD)
   {
      fEmptyRect = WinIsRectEmpty(habMain, &tiStruct.rclTrack);
      WinSendMsg((HWND)mp2,
                    MM_SETITEMATTR,
                    MPFROM2SHORT(MENU_COPY, TRUE),
                    MPFROM2SHORT(MIA_DISABLED,
                                 fEmptyRect ? MIA_DISABLED : ~MIA_DISABLED)
                                );
      WinSendMsg((HWND)mp2,
                    MM_SETITEMATTR,
                    MPFROM2SHORT(MENU_CUT, TRUE),
                    MPFROM2SHORT(MIA_DISABLED,
                                 fEmptyRect ? MIA_DISABLED : ~MIA_DISABLED)
                                );
      WinSendMsg((HWND)mp2,
                    MM_SETITEMATTR,
                    MPFROM2SHORT(MENU_CLEAR, TRUE),
                    MPFROM2SHORT(MIA_DISABLED,
                                 fEmptyRect ? MIA_DISABLED : ~MIA_DISABLED)
                                );

      WinOpenClipbrd(habMain);

      if ((HBITMAP)WinQueryClipbrdData(habMain, CF_BITMAP) != (HBITMAP)0L &&
            !fEmptyRect)
      {
         fEmptyRect = TRUE;
      }
      else
      {
         fEmptyRect = FALSE;
      }
      WinSendMsg((HWND)mp2,
                 MM_SETITEMATTR,
                 MPFROM2SHORT(MENU_PASTE, TRUE),
                 MPFROM2SHORT(MIA_DISABLED,
                 fEmptyRect ? ~MIA_DISABLED : MIA_DISABLED)
                           );
      WinCloseClipbrd(habMain);
   }
}   /*  end of MenuInitialization  */

/**************************************************************************
 *
 *  Name       : MouseButtonDown
 *
 *  Description: process the mouse button interface
 *
 *  Concepts   : rectangle tracking
 *
 *  API's      : WinSetFocus
 *               WinIsRectEmpty
 *               WinSetRect
 *               WinQueryWindowRect
 *               WinTrackRect
 *
 *  Parameters : hwnd = window handle
 *               mp1 = message parameter
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID MouseButtonDown(HWND hwnd, MPARAM mp1)
{
    /*
     * As the application is handling the button message, set the focus
     * to the frame window.
     */
   WinSetFocus(HWND_DESKTOP,hwndClient);

    /*
     * Clear an existing rectangle from the screen, and set the process into
     * track rectangle mode loop, after setting up the required parameters.
     */
   if(!WinIsRectEmpty(habMain, &tiStruct.rclTrack))
   {
      RemoveRect();
      WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
   } /* endif */

   tiStruct.cxBorder = 1;
   tiStruct.cyBorder = 1;
   tiStruct.cxGrid = 0;
   tiStruct.cyGrid = 0;
   WinSetRect(habMain, &tiStruct.rclTrack,
             LOUSHORT(mp1),
             HIUSHORT(mp1),
             LOUSHORT(mp1),
             HIUSHORT(mp1));
   WinQueryWindowRect(hwnd, &tiStruct.rclBoundary);
   tiStruct.ptlMinTrackSize.x = 1;
   tiStruct.ptlMinTrackSize.y = 1;
   tiStruct.ptlMaxTrackSize.x = ++tiStruct.rclBoundary.xRight;
   tiStruct.ptlMaxTrackSize.y = ++tiStruct.rclBoundary.yTop;
   tiStruct.fs  = TF_STANDARD | TF_ALLINBOUNDARY | TF_RIGHT | TF_BOTTOM;
   WinTrackRect(hwnd, (HPS)0L, &tiStruct);
   if((tiStruct.rclTrack.xRight - tiStruct.rclTrack.xLeft) > 2 &&
       (tiStruct.rclTrack.yTop - tiStruct.rclTrack.yBottom) > 2)
   {
      DrawRect();
   }
   else
   {
      WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
   } /* endif */
}    /*  end of MouseButtonDown  */

/**************************************************************************
 *
 *  Name       : PaintRoutine
 *
 *  Description: display bits in the window
 *
 *  Concepts   :
 *
 *  API's      : WinQueryUpdateRect
 *               GpiBitBlt
 *               WinInvalidateRect
 *               WinIsRectEmpty
 *
 *
 *  Parameters : hwnd = window handle
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID PaintRoutine(HWND hwnd)
{
   RECTL   rclUpdt;
   BOOL    RC;
     /*
      * If we are in a position to update the window, get the invalidated
      * rectangle and use this to copy from the memory bit map to the screen,
      * using a BitBlt. Finally, validate the updated rectangle.
      */
   if (fPaintable)
   {
       WinQueryUpdateRect(hwnd, &rclUpdt);
   /*
    * Want the top & right-hand side, so increase by one, since the
    * rectangles used for the BitBlt are non-inclusive (the bottom
    * and left are included, but not the top and right).
    */
       aptlCorners[0].x = rclUpdt.xLeft;
       aptlCorners[0].y = rclUpdt.yBottom;
       aptlCorners[1].x = rclUpdt.xRight+1;
       aptlCorners[1].y = rclUpdt.yTop+1;
   /*
    * Want the top & right-hand side as well, so increase by one, since the
    * rectangles used for the BitBlt are non-inclusive (the bottom
    * and left are included, but not the top and right).
    */
       aptlCorners[2].x = rclUpdt.xLeft;
       aptlCorners[2].y = rclUpdt.yBottom;
       aptlCorners[3].x = rclUpdt.xRight+1;
       aptlCorners[3].y = rclUpdt.yTop+1;
       GpiBitBlt(hpsClient,
                 hpsMemory,
                 4L,
                 aptlCorners,
                 ROP_SRCCOPY,
                 BBO_IGNORE
                );
       RC = WinValidateRect(hwnd, &rclUpdt, FALSE);
       if (!WinIsRectEmpty(habMain, &tiStruct.rclTrack))
           DrawRect();
   }
}  /*  end of PaintRoutine  */

/**************************************************************************
 *
 *  Name       : PasteRoutine
 *
 *  Description: display a rectangle from the clipboard
 *
 *  Concepts   :
 *
 *  API's      : WinIsRectEmpty
 *               WinOpenClipbrd
 *               WinQueryClipbrdData
 *               GpiQueryBitmapParameters
 *               DevOpenDC
 *               GpiCreatePS
 *               GpiSetBitmap
 *               WinSetPointer
 *               GpiBitBlt
 *               GpiAssociate
 *               GpiDestroyPS
 *               DevCloseDC
 *               WinCloseClipbrd
 *               WinSetRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID PasteRoutine()
{
   HDC     hdcTempMem;
   HBITMAP bmapTempMem;
   HPS     hpsTempMem;
   BITMAPINFOHEADER bmpTemp;
   SIZEL   sizlWork;
     /*
      * If a bounding rectangle has been defined, open the clipboard and
      * see if it contains a bit map. If one exists, create temporary DCs
      * and GPI PSs to enable the clipboard bit map to be BitBlted into
      * the memory bit map and from there onto the screen. Destroy all the
      * temporary resources, close the clipboard and clear the
      * bounding rectangle.
      */
   if (!WinIsRectEmpty(habMain, &tiStruct.rclTrack))
   {
      WinOpenClipbrd(habMain);
      bmapTempMem = (HBITMAP)WinQueryClipbrdData(habMain, CF_BITMAP);
      if(bmapTempMem != (HBITMAP)0L)
      {
         GpiQueryBitmapParameters(bmapTempMem,
                                  &bmpTemp
                                  );
          /*
           * Want to paste inside the bounding rect. So increase the
           * left & bottom by one but leave the top & right since the
           * rectangles used for the BitBlt are non-inclusive (the
           * bottom and left are included, but not the top and right).
           */
          aptlCorners[0].x = tiStruct.rclTrack.xLeft+1;
          aptlCorners[0].y = tiStruct.rclTrack.yBottom+1;
          aptlCorners[1].x = tiStruct.rclTrack.xRight;
          aptlCorners[1].y = tiStruct.rclTrack.yTop;
          /*
           * No adjustments to the top & right top (using lengths)
           * since the rectangles used for the BitBlt are
           * non-inclusive (the bottom and left are included,
           * but not the top and right).
           */
          aptlCorners[2].x = 0;
          aptlCorners[2].y = 0;
          aptlCorners[3].x = bmpTemp.cx;
          aptlCorners[3].y = bmpTemp.cy;
          hdcTempMem = DevOpenDC(habMain,
                                 OD_MEMORY,
                                (PSZ)"*",
                                 8L,
                                (PDEVOPENDATA)&dcdatablk,
                                (HDC)0L
                                );
          sizlWork.cx = bmpTemp.cx;
          sizlWork.cy = bmpTemp.cy;
          hpsTempMem = GpiCreatePS(habMain,
                                   hdcTempMem,
                                   &sizlWork,
                                   GPIA_ASSOC | PU_PELS
                                  );
          GpiSetBitmap(hpsTempMem, bmapTempMem);
          WinSetPointer(HWND_DESKTOP,
                        WinQuerySysPointer(HWND_DESKTOP,
                                           SPTR_WAIT,
                                           FALSE)
                       );
          GpiBitBlt(hpsMemory,
                    hpsTempMem,
                    4L,
                    aptlCorners,
                    ROP_SRCCOPY,
                    BBO_IGNORE
                   );
          /*
           * Want a slightly bigger rectangle for the screen because
           * we need to remove the bounding rectangle as well as do
           * the paste.
           */
          aptlCorners[2].x =  (--aptlCorners[0].x);
          aptlCorners[2].y =  (--aptlCorners[0].y);
          aptlCorners[3].x =  (++aptlCorners[1].x);
          aptlCorners[3].y =  (++aptlCorners[1].y);
          GpiBitBlt(hpsClient,
                    hpsMemory,
                    4L,
                    aptlCorners,
                    ROP_SRCCOPY,
                    BBO_IGNORE
                   );
          WinSetPointer(HWND_DESKTOP,
                        WinQuerySysPointer(HWND_DESKTOP,
                                           SPTR_ARROW,
                                           FALSE)
                       );

          GpiSetBitmap(hpsTempMem, (HBITMAP)0L);
          GpiAssociate(hpsTempMem, (HDC)0L);
          GpiDestroyPS(hpsTempMem);
          DevCloseDC(hdcTempMem);
      } /* endif */
      WinCloseClipbrd(habMain);
      WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
   } /* endif */
}   /* end of PasteRoutine  */

/**************************************************************************
 *
 *  Name       : RemoveRect
 *
 *  Description: Update the screen from the memory bitmap, thus erasing
 *               the rectangle
 *
 *  Concepts   :  bitblt
 *
 *  API's      : GpiBitBlt
 *               WinSetRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID RemoveRect()
{
  /*
   * Want the top & right-hand side as well, so increase by one since the
   * rectangles used for the BitBlt are non-inclusive (the
   * bottom and left are included, but not the top and right).
   */
   aptlCorners[0].x = tiStruct.rclTrack.xLeft;
   aptlCorners[0].y = tiStruct.rclTrack.yBottom;
   aptlCorners[1].x = tiStruct.rclTrack.xRight+1;
   aptlCorners[1].y = tiStruct.rclTrack.yTop+1;

  /*
   * Want the top & right-hand side as well, so increase by one since the
   * rectangles used for the BitBlt are non-inclusive (the
   * bottom and left are included, but not the top and right).
   */
   aptlCorners[2].x = tiStruct.rclTrack.xLeft;
   aptlCorners[2].y = tiStruct.rclTrack.yBottom;
   aptlCorners[3].x = tiStruct.rclTrack.xRight+1;
   aptlCorners[3].y = tiStruct.rclTrack.yTop+1;
   GpiBitBlt(hpsClient,
             hpsMemory,
             4L,
             aptlCorners,
             ROP_SRCCOPY,
             BBO_IGNORE
            );


   WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);
}  /* end of RemoveRect */

/**************************************************************************
 *
 *  Name       : Report_Error
 *
 *  Description: Display a particular system error message, the latest one
 *               for the required thread.  No resources can be loaded in
 *               case of memory error.
 *
 *  Concepts   :
 *
 *  API's      : WinGetErrorInfo
 *               WinMessageBox
 *               WinFreeErrorInfo
 *
 *  Parameters : hab = anchor block handle
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID Report_Error(HAB hab)
{
   PERRINFO  perriBlk;
   PSZ       pszErrMsg;
   PSZ       pszOffSet;

   if (!fErrMem)
   {
      if((perriBlk = WinGetErrorInfo(hab)) != (PERRINFO)NULL)
      {
         pszOffSet = ((PSZ)perriBlk) + perriBlk->offaoffszMsg;
         pszErrMsg = ((PSZ)perriBlk) + *((PUSHORT)pszOffSet);

         WinMessageBox(HWND_DESKTOP,
                        hwndFrame,
                       (PSZ)(pszErrMsg),
                       (PSZ)szTitleBar,
                        0,
                        MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL );
          WinFreeErrorInfo(perriBlk);
          return;
      }
   } /* endif */
   WinMessageBox(HWND_DESKTOP,
                 hwndFrame,
                 (PSZ)"ERROR - Out Of Memory",
                 (PSZ)szTitleBar,
                 0,
                 MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL);
}  /* end of Report_Error */

/**************************************************************************
 *
 *  Name       : WindowDestroy
 *
 *  Description: destroy particular window data
 *
 *  Concepts   :
 *
 *  API's      : GpiSetBitmap
 *               GpiDeleteBitmap
 *               GpiAssociate
 *               GpiDestroyPS
 *               DevCloseDC
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID WindowDestroy()
{
    /*
     * Deselect the bit map from the screen-compatible device context, and
     * delete the bit map. Disassociate the two GPI PSs from their respective
     * DCs and destroy the PSs. Finally destroy the memory DC.
     */

   GpiSetBitmap(hpsMemory, (HBITMAP)0L);
   GpiDeleteBitmap(bmapMemory);
   GpiAssociate(hpsClient, (HDC)0L);
   GpiDestroyPS(hpsClient);
   GpiAssociate(hpsMemory, (HDC)0L);
   GpiDestroyPS(hpsMemory);
   DevCloseDC(hdcMemory);
   return;
}   /*  end of WindowDestroy  */

/**************************************************************************
 *
 *  Name       : WindowInitialization
 *
 *  Description: create window information
 *
 *  Concepts   :
 *
 *  API's      : WinQuerySysValue
 *               WinOpenWindowDC
 *               GpiCreatePS
 *               DevOpenDC
 *               GpiSetBitmap
 *               GpiLoadBitmap
 *               GpiSetRect
 *
 *  Parameters : [none]
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID WindowInitialization(HWND hwnd)
{
   SIZEL   sizlWork;

    /*
     * Get the maximum client area size. Create a window DC for the client
     * area and a normal GPI Presentation Space and associate the two. The
     * GPI PS will be the maximum client area size in pels. Create a
     * memory DC compatible with the screen, create a GPI PS and associate it
     * with the screen compatible DC. Load a bit map from the resource
     * file, and select the bit map into the screen-compatible DC.
     * Initialize the rectangle tracking and set a flag to enable window
     * painting.
     */
   sizlWork.cx = WinQuerySysValue(HWND_DESKTOP, SV_CXFULLSCREEN);
   sizlWork.cy = WinQuerySysValue(HWND_DESKTOP, SV_CYFULLSCREEN);

   hdcClient = WinOpenWindowDC(hwnd);
   hpsClient = GpiCreatePS(habMain,
                           hdcClient,
                           &sizlWork,
                           GPIA_ASSOC | PU_PELS
                          );

   dcdatablk.driver_data = (PSZ)&drvdata;
   hdcMemory = DevOpenDC(habMain,
                         OD_MEMORY,
                        (PSZ)"*",
                         8L,
                        (PDEVOPENDATA)&dcdatablk,
                        (HDC)0L
                        );

   hpsMemory = GpiCreatePS(habMain,
                           hdcMemory,
                           &sizlWork,
                           GPIA_ASSOC | PU_PELS
                          );

   bmapMemory = GpiLoadBitmap( hpsMemory
                             , (HMODULE)0L
                             , (USHORT)ID_BITMAP
                             , sizlWork.cx
                             , sizlWork.cy
                             );

   GpiSetBitmap(hpsMemory, bmapMemory);
   WinSetRect(habMain, &tiStruct.rclTrack, 0, 0, 0, 0);

   fPaintable = TRUE;
   return;
}   /*  end of WindowInitialization  */

/**************************************************************************
 *
 *  Name       : ReportAPIError
 *
 *  Description: display an error message returned from an API call
 *
 *  Concepts   :
 *
 *  API's      : WinGetErrorInfo
 *               WinMessageBox
 *               WinPostMsg
 *
 *  Parameters : what = message string
 *
 *  Return     : [none]
 *
 *************************************************************************/
VOID ReportAPIError(PSZ what)
{
   PERRINFO  pErrInfoBlk;
   PSZ       pszErrMsg;
   PSZ       pszOffSet;
   char      ErrorMsg[CCHMAXSTRING];                          /* error text buffer */

   pszErrMsg = &szDefErrMesg[0];               /* set default in case no err */
   strcpy(ErrorMsg, what);                     /* init message string        */
   strcat(ErrorMsg, "\n\n");                   /* double space for neatness  */
   if ((pErrInfoBlk = WinGetErrorInfo(habMain)) != (PERRINFO)NULL)
   {
       pszOffSet = ((PSZ)pErrInfoBlk) + pErrInfoBlk->offaoffszMsg;
       pszErrMsg = ((PSZ)pErrInfoBlk) + *((PUSHORT)pszOffSet);

       WinFreeErrorInfo(pErrInfoBlk);
   } /*  if ((pErrInfoBlk = WinGetErrorInfo(hab)) != (PCH)NULL) */
   strcat(ErrorMsg, pszErrMsg);                 /* combine specific text     */
   WinMessageBox(HWND_DESKTOP,                  /* parent window is desk top */
                 hwndFrame,                     /* owner window is our frame */
                (PSZ)&ErrorMsg[0],              /* text for message          */
                (PSZ)szApiErrTitle,             /* title is API error        */
                 0,                             /* null window id            */
                 MB_MOVEABLE | MB_CUACRITICAL | MB_CANCEL ); /* flags        */
   Status = ERRORX;
   WinPostMsg(hwndClient, WM_QUIT, (ULONG)0, (ULONG)0);
   return;
}  /* end of ReportAPIError  */

/**************************************************************************
 *
 *  Name       :  ProductInfoDlgProc(hwnd, msg, mp1, mp2)
 *
 *  Description:  Processes all messages sent to the Product
 *                information dialog
 *
 *  Concepts   :  Called for each message sent to the Product information
 *                dialog box.
 *
 *                the Product information dialog has only a button control, so this
 *                routine processes only WM_COMMAND messages.  Any
 *                WM_COMMAND posted must have come from the OK
 *                button, so we dismiss the dialog upon receiving it.
 *
 *  API's      :  WinDefDlgProc
 *
 *  Parameters :  [none]
 *
 *  Return     :  dependent on message sent
 *
 *************************************************************************/
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd,ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch (msg)
   {
   case WM_INITDLG:
      SetSysMenu(hwnd);       /* system menu for this dialog  */
         return (MRESULT)NULL;

   default:
      return WinDefDlgProc(hwnd, msg, mp1, mp2);
   }
   return (MRESULT)NULL;
}   /*  End of ProductInfoDlgProc  */

/**************************************************************************
 *
 *  Name       :  HelpProductInfo(hwndMain)
 *
 *  Description: Processes the Product information choice from the Help
 *               pulldown
 *
 *  Concepts   : Processes the WM_COMMAND message posted by the
 *               Product information item of the Help menu
 *               Called from MainCommand when the Product information
 *               menu item is selected
 *               Calls WinDlgBox to display the Product information
 *               dialog.
 *
 *  API's      :  WinDlgBox
 *
 *  Parameters :  hwndMain = main window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID  HelpProductInfo(HWND hwndMain)
{
    /* display the Product Information dialog. */
   if(DID_ERROR == WinDlgBox(HWND_DESKTOP,
                             hwndMain,
                             (PFNWP)ProductInfoDlgProc,
                             0,
                             IDD_PRODUCTINFO,
                             (PVOID)NULL))
      ReportAPIError("Can't start dialog");
   return;
}   /*  End of HelpProductInfo   */

/**************************************************************************
 *
 *  Name       :  SetSysMenu(hDlg)
 *
 *  Description:  Enables only the Move and Close items in the
 *                system menu pulldown for any dialog
 *
 *  Concepts   :
 *
 *  API's      :  WinWindowFromID
 *                WinSendMsg
 *
 *  Parameters :  hDlg = dialog handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID SetSysMenu(HWND hDlg)
{
   HWND     hSysMenu;
   MENUITEM Mi;
   SHORT    Pos;
   SHORT    Id;
   SHORT    cItems;

   /*
    *  We only want Move and Close in the system menu.
    */
   hSysMenu = WinWindowFromID(hDlg, FID_SYSMENU);
   WinSendMsg( hSysMenu, MM_QUERYITEM
             , MPFROM2SHORT(SC_SYSMENU, FALSE), MPFROMP((PCH) & Mi));
   Pos = 0;
   cItems = (SHORT)WinSendMsg( Mi.hwndSubMenu, MM_QUERYITEMCOUNT,
                               (MPARAM)NULL, (MPARAM)NULL);
   while (cItems--)
   {
      Id = SHORT1FROMMR(WinSendMsg( Mi.hwndSubMenu, MM_ITEMIDFROMPOSITION
                         , MPFROM2SHORT(Pos, TRUE), (MPARAM)NULL));
      switch (Id)
      {
      case SC_MOVE:
      case SC_CLOSE:
         Pos++;  /* Don't delete that one. */
         break;
      default:
         WinSendMsg( Mi.hwndSubMenu, MM_DELETEITEM
                     , MPFROM2SHORT(Id, TRUE), (MPARAM)NULL);
      }
   }
} /*  End of SetSysMenu  */
/***************************  End of clipbrd.c  **************************/

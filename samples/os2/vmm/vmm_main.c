/*static char *SCCSID = "@(#)vmm_main.c	6.19 92/02/18";*/
/*==============================================================*\
 *  VMM_MAIN.C - Sample PM application                          *
 *                                                              *
 *      (C) Copyright IBM Corporation 1992                      *
 *                                                              *
 *   DISCLAIMER OF WARRANTIES.  The following [enclosed] code is*
 *   sample code created by IBM Corporation. This sample code is*
 *   not part of any standard IBM product and is provided to you*
 *   solely forthe purpose of assisting you in the development  *
 *   of your applications.  The code is provided "AS IS",       *
 *   without warranty of any kind. No IBM shall be liable for   *
 *   any damages arising out of your use of the sample code,    *
 *   even if they have been advised of the possibility of       *
 *   such damages.                                              *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This application serves two distinct purposes. First, it    *
 *  demonstrates the standard menus and dialogs that most PM    *
 *  applications will contain.  The Demo menu calls dialog      *
 *  boxes that demonstrate all of the controls available for    *
 *  use in a dialog box.  There is also a dialog box which      *
 *  demonstrates the setting and removing of control            *
 *  Presentation Parameters.  Finally, the demo menu also has   *
 *  menu items that demonstrate the various types of message    *
 *  boxes that the system supports.                             *
 *                                                              *
 *  This application also serves as a template that can be      *
 *  easily modified by an application developer.  The source    *
 *  files are organized so that the overhead code that should   *
 *  be in all applications is located in the same files so      *
 *  that these files do not need to be modified.  The routines  *
 *  that deal with application specific code are also located   *
 *  in their own modules.  An application developer need only   *
 *  change these files in order to modify this template for     *
 *  his application.                                            *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This source file contains the following functions:          *
 *                                                              *
 *      main() - main routine                                   *
 *      MainWndProc(hwnd, msg, mp1, mp2) - main window procedure*
 *      MainCommand(mp1, mp2) - WM_COMMAND processing of Main   *
 *      MessageBox(hwnd nId, pszMsg, fsStyle, bBeep)            *
 *      DisplayPage(hps, pptl, ulPageIndex)                     *
 *      EnableMenuItem(hwnd, sMenuItem, fEnable)                *
 *                                                              *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "vmm.h"

/****************************************************************\
 *  Main routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    main()                                             *
 *                                                              *
 *  Purpose: Initializes the PM environment, calls the          *
 *              initialization routine, creates the main        *
 *              window,  and polls the message queue            *
 *                                                              *
 *  Usage:                                                      *
 *                                                              *
 *  Method:  -obtains anchor block handle and creates message   *
 *              queue                                           *
 *           -calls the initialization routine                  *
 *           -creates the main frame window which creates the   *
 *              main client window                              *
 *           -polls the message queue via Get/Dispatch Msg loop *
 *           -upon exiting the loop, exits                      *
 *                                                              *
 *  Returns: 1 - if sucessful execution completed               *
 *           0 - if error                                       *
\****************************************************************/
INT  main(VOID)
{
    QMSG qmsg;                                      /* message structure */
    ULONG ctlData;                                 /* frame control data */
    EXCEPTIONREGISTRATIONRECORD ExceptionStruct;

    ulFreePage = 0L;
    strcpy(szWindowText,"VMM Sample");
    hab = WinInitialize(0);
    if(!hab)
    {
        DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
        DosExit(EXIT_PROCESS, RETURN_ERROR);
    }
                                             /* find a define for this v */
    hmq = WinCreateMsgQueue(hab, 0);
    if(!hmq)
    {
        DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
        WinTerminate(hab);
        DosExit(EXIT_PROCESS, RETURN_ERROR);
    }
    if(!Init())
    {
        MessageBox(HWND_DESKTOP, IDMSG_INITFAILED, "Error !",
                            MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
        DosExit(EXIT_PROCESS, RETURN_ERROR);
    }
                                               /* create the main window */
    ctlData = FCF_STANDARD | FCF_VERTSCROLL;
    hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                       WS_VISIBLE,
                                       (PVOID)&ctlData,
                                       (PSZ)szAppName,
                                       (PSZ)NULL,
                                       WS_VISIBLE,
                                       (HMODULE)NULL,
                                       IDR_RESOURCE,
                                       (PHWND)&hwndMain);
    if(!hwndMainFrame)
    {
        MessageBox(HWND_DESKTOP, IDMSG_MAINWINCREATEFAILED, "Error !",
                            MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
        DosExit(EXIT_PROCESS, RETURN_ERROR);
    }
   InitHelp();
                         /* Invoke exception management, and direction user
                             exception handling function to GPFHandler() */
   ExceptionStruct.ExceptionHandler = (_ERR *)&GPFHandler;
   DosSetExceptionHandler(&ExceptionStruct) ;

                                            /* Get/Dispatch Message loop */
   while(WinGetMsg(hab, (PQMSG)&qmsg, 0L, 0L, 0L))
       WinDispatchMsg(hab, (PQMSG)&qmsg);

                                        /* Release exception managerment */
   DosUnsetExceptionHandler(&ExceptionStruct) ;

   DestroyHelpInstance();                   /* destroy the help instance */
#ifdef VER630
                                     /* will normally be put in ExitProc */
    WinDestroyWindow(hwndMainFrame);

    /*--------------------------------------------------*\
     *      Any other system resources used             *
     *      (e.g. memory or files) should be freed here *
    \*--------------------------------------------------*/
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

#endif

    DosExit(EXIT_PROCESS, RETURN_SUCCESS);
    return 0L;
}                                                              /* main() */

/****************************************************************\
 *  Main client window procedure                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainWndProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: Processes the messages sent to the main client     *
 *           window. This routine processes the basic messages  *
 *           all client windows should process and passes all   *
 *           others onto UserWndProc where the developer can    *
 *           process any others.                                *
 *                                                              *
 *  Usage:   Called for each message placed in the main         *
 *           window's message queue                             *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed. Any messages *
 *           not specifically process are passed to the user's  *
 *           message processing procedure UserWndProc().        *
 *                                                              *
 *  Returns: Return values are determined by each message       *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static LONG sVscrollMax;
   MRESULT sRC;

   switch(msg)
   {
       case WM_CREATE:

           sVscrollMax =  MAXPAGES*(BOX_HEIGHT+VERT_SPACING);

           hwndVscroll = WinWindowFromID(
                              WinQueryWindow(hwnd, QW_PARENT),
                              FID_VERTSCROLL);
           WinSendMsg(hwndVscroll, SBM_SETSCROLLBAR,
                                  MPFROM2SHORT(sVscrollPos, 0),
                                  MPFROM2SHORT(0, sVscrollMax) );
           break;

       case WM_VSCROLL:
           switch (SHORT2FROMMP(mp2))
           {
               case SB_LINEUP:
                   sVscrollPos -= SMALL_BOX_HEIGHT;
                   break;

               case SB_LINEDOWN:
                   sVscrollPos += SMALL_BOX_HEIGHT;
                   break;

               case SB_PAGEUP:
                   sVscrollPos -= BOX_HEIGHT+VERT_SPACING;
                   break;

               case SB_PAGEDOWN:
                   sVscrollPos += BOX_HEIGHT+VERT_SPACING;
                   break;

               case SB_SLIDERTRACK:
               case SB_SLIDERPOSITION:
                   sVscrollPos = SHORT1FROMMP(mp2);
                   break;
           }
           sVscrollPos = max(0, min(sVscrollPos, sVscrollMax) );

           if(sVscrollPos != SHORT1FROMMR(WinSendMsg(hwndVscroll,
                                     SBM_QUERYPOS, (MPARAM)NULL, (MPARAM)NULL)))
           {
              WinSendMsg(hwndVscroll, SBM_SETPOS,
                               MPFROMSHORT(sVscrollPos), NULL);
              WinInvalidateRect(hwnd, NULL, FALSE);
           }
           break;

       case WM_CHAR:
           switch (SHORT2FROMMP(mp2))
           {
               case VK_LEFT:
               case VK_UP:
                   sVscrollPos -= SMALL_BOX_HEIGHT;
                   break;

               case VK_DOWN:
               case VK_RIGHT:
                   sVscrollPos += SMALL_BOX_HEIGHT;
                   break;

               case VK_PAGEUP:
                   sVscrollPos -= BOX_HEIGHT+VERT_SPACING;
                   break;

               case VK_PAGEDOWN:
                   sVscrollPos += BOX_HEIGHT+VERT_SPACING;
                   break;
           }
           sVscrollPos = max(0, min(sVscrollPos, sVscrollMax) );

           if(sVscrollPos != SHORT1FROMMR(WinSendMsg(hwndVscroll,
                                     SBM_QUERYPOS, (MPARAM)NULL, (MPARAM)NULL)))
           {
              WinSendMsg(hwndVscroll, SBM_SETPOS,
                               MPFROMSHORT(sVscrollPos), NULL);
              WinInvalidateRect(hwnd, NULL, FALSE);
           }
           break;

       case WM_PAINT:
           MainPaint(hwnd);
           break;

       case WM_COMMAND:
           MainCommand(mp1, mp2);
           break;

   /*--------------------------------------------------*\
    *      Any messages not processed are passed on    *
    *      to the user's window proc.  It is           *
    *      responsible for passing any messages it     *
    *      doesn't handle onto WinDefWindowProc()      *
   \*--------------------------------------------------*/
       case HM_QUERY_KEYS_HELP:
           return ((MRESULT)PANEL_KEYSHELP);

       default:
           sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
           return sRC;
   }
   return ((MRESULT)0L);  /* window procedures should return 0 as a default */
}                                                        /* MainWndProc() */

/****************************************************************\
 *  Main client painting routine                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainPaint(hwnd)                                    *
 *                                                              *
 *  Purpose: Paints the main client window.                     *
 *                                                              *
 *  Usage:   Routine is called whenver the client window        *
 *           procedure receives a WM_PAINT message              *
 *                                                              *
 *  Method:  -begins painting by calling WinBeginPaint          *
 *              and retriving the HPS for the window            *
 *           -paint page info boxes                             *
 *                                                              *
 *           -ends painting by calling WinEndPaint              *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainPaint(HWND hwnd)
{
    HPS    hps;
    USHORT i;
    POINTL ptl;
    RECTL  rect;
    SIZEL  sizl;
    char   szBuff[BUFF_SIZE];

    sizl.cx = 0L;
    sizl.cy = 0L;

    hps = WinBeginPaint(hwnd, 0L, 0L);
    GpiSetPS(hps, &sizl, PU_LOMETRIC);

    WinQueryWindowRect(hwnd, &rect);
    WinFillRect(hps, (PRECTL)&rect, CLR_BACKGROUND);

    /* convert the coords of the window to world coords */
    GpiConvert(hps, CVTC_DEVICE, CVTC_WORLD, 2L, (PPOINTL) &rect);

    ptl.x = HORZ_INDENT*3;
    ptl.y = rect.yTop - (BOX_HEIGHT+VERT_SPACING) + sVscrollPos;
    GpiMove(hps, &ptl);

    ptl.x += BOX_WIDTH;
    ptl.y += BOX_HEIGHT;
    GpiSetColor(hps, CLR_GREEN);
    GpiBox(hps, DRO_OUTLINE, &ptl, 0L, 0L);

    ptl.x = HORZ_INDENT*3 + HORZ_SPACING*3 + BOX_WIDTH;
    ptl.y = rect.yTop - (BOX_HEIGHT+VERT_SPACING) + sVscrollPos;
    GpiMove(hps, &ptl);

    ptl.x += BOX_WIDTH;
    ptl.y += BOX_HEIGHT;
    GpiSetColor(hps, CLR_RED);
    GpiBox(hps, DRO_OUTLINE, &ptl, 0L, 0L);

    GpiSetColor(hps, CLR_BLACK);

    ptl.x = HORZ_INDENT*3+30;
    ptl.y = rect.yTop - (VERT_SPACING+SMALL_BOX_HEIGHT) + 30 + sVscrollPos;
    strcpy(szBuff,"Committed");
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);

    ptl.y -= SMALL_BOX_HEIGHT;
    strcpy(szBuff,"page");
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);


    ptl.x = HORZ_INDENT*3+30 + HORZ_SPACING*3 + BOX_WIDTH ;
    ptl.y = rect.yTop - (VERT_SPACING+SMALL_BOX_HEIGHT) + 30 + sVscrollPos;
    strcpy(szBuff,"Non-Committed");
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);

    ptl.y -= SMALL_BOX_HEIGHT;
    strcpy(szBuff,"page");
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);

    ptl.x = HORZ_INDENT;                           /* 1'st page location */
    ptl.y = rect.yTop - 2*(BOX_HEIGHT+VERT_SPACING) + sVscrollPos;

    for (i=0;((ULONG) i)<ulFreePage; i++)
    {
        DisplayPage(hps, &ptl, i);             /* Redisplay alloc status */
        ptl.x += BOX_WIDTH+HORZ_SPACING;

        if (ptl.x+BOX_WIDTH > rect.xRight)
        {                 /* If next page over window, move to next line */
           ptl.x = HORZ_INDENT+BOX_WIDTH+HORZ_SPACING;
           ptl.y -= (BOX_HEIGHT+VERT_SPACING);
        }
    /* If the next page is a base page, display it on a new line for clarity */

        if (apgentry[i+1].fBaseAddr)
        {
            ptl.x = HORZ_INDENT;
            ptl.y -= (BOX_HEIGHT+VERT_SPACING);
        }
    }
    WinEndPaint(hps);
    return;
}                                                   /* MainPaint() */

/****************************************************************\
 *  Main window WM_COMMAND processing procedure                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainCommand(mp1, mp2)                               *
 *                                                              *
 *  Purpose:Calls the appropriate procedures that deal with     *
 *           the selected menu item.                            *
 *                                                              *
 *  Usage:  Routine is called whenever a WM_COMMAND message     *
 *          is posted to the main window.                       *
 *                                                              *
 *  Method: a switch statement branches on the id of the        *
 *          menu item that posted the message and the           *
 *          appropriate action for that item is taken.  Any     *
 *          menu ids that are not part of the standard menu     *
 *          set are passed onto the user defined WM_COMMAND     *
 *          processing procedure.                               *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID MainCommand(MPARAM mp1, MPARAM mp2)
{
    switch(SHORT1FROMMP(mp1))
    {
        case IDM_GENERALHELP:
           HelpExtended(mp2);
           return;

        case IDM_USINGHELP:
           HelpHelpForHelp(mp2);
           return;

        case IDM_TUTORIAL:
           HelpKeys(mp2);
           return;

        case IDM_HELPINDEX:
           HelpIndex(mp2);
           return;

        case IDM_HELPPRODUCTINFO:
           HelpAbout(mp2);
           return;

    /*--------------------------------------------------*\
     *      User command processing routine is called   *
     *      here so any ids not procecessed here can be *
     *      processed                                   *
    \*--------------------------------------------------*/
        default:
            UserCommand(mp1, mp2);
            break;
    }
    return;
}                                                /* MainCommand() */

/****************************************************************\
 *  Exit routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    DisplayPage(hps, pptl, ulPageIndex)                *
 *                                                              *
 *  Purpose: Processes the VMM menu's Exit item.                *
 *                                                              *
 *  Usage:   called whenever Exit from the VMM menu is          *
 *           selected                                           *
 *                                                              *
 *  Method:  Routine posts a WM_CLOSE message to the main       *
 *           application window.                                *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID DisplayPage(HPS hps, PPOINTL pptl, ULONG ulPageIndex)
{
    POINTL ptl;
    MATRIXLF matrix;
    ULONG ulAccess=0;
    ULONG ulRegionSize;
    PVOID pMem;
    char szBuff[BUFF_SIZE];

    pMem = apgentry[ulPageIndex].pvAddress;

    /* We don't check the return code here since we assume that the array
       contains good addresses since we verified them when we put them in
       the array.  */                     /* Get memory size & attributes */
    DosQueryMem(pMem, &ulRegionSize, &ulAccess);

    matrix.fxM11 = MAKEFIXED(1,0);
    matrix.fxM12 = 0L;
    matrix.lM13  = 0L;
    matrix.fxM21 = 0L;
    matrix.fxM22 = MAKEFIXED(1,0);
    matrix.lM23  = 0L;
    matrix.lM31  = pptl->x;                             /* x translation */
    matrix.lM32  = pptl->y;                             /* y translation */
    matrix.lM33  = 1L;

    GpiSetDefaultViewMatrix(hps, 9L, &matrix,TRANSFORM_REPLACE);

    ptl.x = 0L;
    ptl.y = 0L;
    GpiMove(hps, &ptl);

    ptl.x = BOX_WIDTH;
    ptl.y = BOX_HEIGHT;

    /* set outline color to CLR_RED for uncommitted, CLR_GREEN for committed */
    if (ulAccess & PAG_COMMIT)
    {
        GpiSetColor(hps, CLR_GREEN);
    }
    else
        GpiSetColor(hps, CLR_RED);

    GpiBox(hps, DRO_OUTLINE, &ptl, 0L, 0L);

    /* should query sys values in case the default is not black ? */
    GpiSetColor(hps, CLR_BLACK);

    szBuff[0]='\000';
    if (ulAccess & PAG_READ)
        strcat(szBuff, "R");

    if (ulAccess & PAG_WRITE)
        strcat(szBuff, "W");

    if (ulAccess & PAG_EXECUTE)
        strcat(szBuff, "E");

    if (ulAccess & PAG_GUARD)
        strcat(szBuff, "G");

    if (ulAccess & PAG_DEFAULT)
        strcat(szBuff, "Def");

    if (ulAccess & PAG_BASE)
        strcat(szBuff, "Base");

    ptl.x = HORZ_INDENT;
    ptl.y = VERT_INDENT;
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);

    ptl.x = 0;
    ptl.y = SMALL_BOX_HEIGHT;

    GpiMove(hps, &ptl);

    ptl.x = BOX_WIDTH;
    ptl.y = SMALL_BOX_HEIGHT;
    GpiLine(hps, &ptl);

    /* Get base address of current page */
    sprintf(szBuff, "0x%p", pMem);

    ptl.x = HORZ_INDENT;
    ptl.y = SMALL_BOX_HEIGHT+VERT_INDENT;
    GpiCharStringAt(hps, &ptl, (LONG) strlen(szBuff), szBuff);
    return;
}

/****************************************************************\
 *  Message Box procedure                                       *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MessageBox(hwndOwner, nIdMsg,pszMsg,fsStyle, bBeep)*
 *                                                              *
 *  Purpose: Displays the warning message box with the message  *
 *              given in nIdMsg retrived from the message table *
 *                                                              *
 *  Usage:   Called whever an error occurs and a message wishes *
 *           to be displayed to the user                        *
 *                                                              *
 *  Method:  - Message string is loaded from the process'       *
 *              message table                                   *
 *           - Alarm beep is sounded if desired                 *
 *           - Message box with the message is displayed        *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
VOID MessageBox(HWND hwndOwner, LONG nIdMsg, PSZ pszMsg, LONG fsStyle,
                     BOOL bBeep)
{
    CHAR szText[MESSAGELEN];

    if(!WinLoadMessage(hab, (HMODULE)NULL, nIdMsg, MESSAGELEN, (PSZ)szText))
    {
        WinAlarm(HWND_DESKTOP, WA_ERROR);
        return;
    }
    if(bBeep)
        WinAlarm(HWND_DESKTOP, WA_ERROR);

    WinMessageBox(HWND_DESKTOP,
                  hwndOwner,
                  szText,
                  (PSZ)pszMsg,
                  IDM_MSGBOX,
                  fsStyle);
    return;
}                                                   /* MessageBox() */

/****************************************************************\
 *  EnableMenuItem procedure                                    *
 *--------------------------------------------------------------*
 *                                                              *
 * Name:    EnableMenuItem(hwnd, sMenuItem, fEnable)            *
 *                                                              *
 * Purpose: This routine handles enabling/disabling of menu     *
 *          items. This is done by getting Parent and Menu hwnd *
 *          handles then sending the appropriate message.       *                     *
 *                                                              *
 * Usage:   Routine is called when enable or disable one item   *
 *          from menu.                                          *
 *                                                              *
 * Method:                                                      *
 *                                                              *
 * Returns: VOID                                                *
 *                                                              *
\****************************************************************/
VOID EnableMenuItem(HWND hwnd, LONG sMenuItem, BOOL fEnable)
{
   HWND hwndParent = WinQueryWindow(hwnd, QW_PARENT);
   HWND hwndMenu   = WinWindowFromID(hwndParent,FID_MENU);

   WinSendMsg(hwndMenu, MM_SETITEMATTR,
              MPFROM2SHORT(sMenuItem, TRUE),
              MPFROM2SHORT(MIA_DISABLED, fEnable ? 0 : MIA_DISABLED));
   return;
}

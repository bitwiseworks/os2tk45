/******************************************************************************
*
*  File Name   : PROCS.C
*
*  Description : miscellaneous Jigsaw functions
*
*  Entry Points:
*                SendCommand()
*                ClientWndProc()
*                WndProcPaint()
*                WndProcCommand()
*                FileOpen()
*                SizeBitmap()
*                MessageBox()
*                JigsawOpenFilterProc()
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

#include "jigsaw.h"
#include "jighelp.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>

#define PB_STAT_CANCEL 1
#define PB_STAT_HELP   2
#define DEFAULT_DIR    "C:\\OS2\\BITMAP"

/******************************************************************************/
/*                                                                                                                         */
/*  Define forward procedure references                                                  */
/*                                                                                                                    */
/******************************************************************************/

/* list of EA type strings */
PSZ apszITL[]={"Jigsaw", (PSZ)NULL};


/******************************************************************************
*
*  Name        : SendCommand
*
*  Description :
*                SendCommand will attempt to post the required command and
*                parameters to the asynchronous drawing thread's message
*                queue.  The command will only be posted if the queue exists.
*
*  Parameters  : ULONG ulCommand, MPARAM mp1, MPARAM mp2
*
*  Return      : BOOL
*
******************************************************************************/

BOOL SendCommand(ULONG ulCommand, MPARAM mp1, MPARAM mp2)
{
   if (!hmqAsync)                          /* no message queue; don't bother */
      return( FALSE);

   switch (ulCommand)
   {
      case UM_DIE:
      case UM_LEFTDOWN:
      case UM_LEFTUP:
      case UM_MOUSEMOVE:
      case UM_DRAW:
      case UM_HSCROLL:
      case UM_VSCROLL:
      case UM_ZOOM:
      case UM_REDRAW:
      case UM_SIZING:
      case UM_JUMBLE:
      case UM_LOAD:
      case UM_CHAR:

         return( WinPostQueueMsg( hmqAsync    /* let worker thread handle it */
                                , ulCommand
                                , mp1
                                , mp2 ) );
         break;

      default:
         return( TRUE);

   }   /* end switch (ulCommand)  */

}   /* end SendCommand() */

/******************************************************************************
*
*  Name        : ClientWndProc
*
*  Description : ClientWndProc is the window procedure associated with the
*                client window.
*
*  Parameters  : HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2
*
*  Return      : MRESULT
*
******************************************************************************/


MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
  CHAR   szTemp[128];
  ULONG  ulSemPostCount;

  switch (msg)
  {
    case WM_CHAR:

      SendCommand( UM_CHAR, mp1, mp2);
      break;

    case WM_CLOSE:
      WinLoadString( habMain, 0, IDS_TERMINATE, sizeof(szTemp), (PSZ)szTemp );
      if( WinMessageBox( HWND_DESKTOP
                       , hwndFrame
                       , szTemp
                       , szTitle
                       , 0
                       , MB_CUAWARNING | MB_YESNO | MB_CUANOTIFICATION | MB_DEFBUTTON1)
               == MBID_YES)
          WinPostMsg( hwnd, WM_QUIT, NULL, NULL);
      break;

    case WM_PAINT:
      return( WndProcPaint());
      break;

    /**************************************************************************/
    /* Keys Help Message                                                                                                              */
    /**************************************************************************/
    case HM_QUERY_KEYS_HELP:
      return (MRESULT)PANEL_HELPKEYS;   /* return id of key help panel */
      break;

    /**************************************************************************/
    /*                                                                                                                */
    /**************************************************************************/
    case WM_ERASEBACKGROUND:
      return( ( MRESULT)TRUE);
      break;

    /**************************************************************************/
    /* Process menu item commands, and commands generated from the keyboard   */
    /* via the accelerator table. Most are handled by the async thread        */
    /**************************************************************************/
    case WM_COMMAND:
      return( WndProcCommand( hwnd, msg, mp1, mp2));
      break;

    /**************************************************************************/
    /* Scrolling is handled by the async drawing thread. Simply pass on the   */
    /* command and parameters                                                                            */
    /**************************************************************************/
    case WM_HSCROLL:
      SendCommand( UM_HSCROLL, mp2, 0);
      break;

    case WM_VSCROLL:
      SendCommand( UM_VSCROLL, mp2, 0);
      break;

    /************************************************************************/
    /* The client area is being resized.                                                         */
    /************************************************************************/
    case WM_SIZE:
      DosResetEventSem( hevDrawOn, &ulSemPostCount);     /* disallow drawing */
      SendCommand( UM_SIZING, mp1, mp2);
      break;

    /**************************************************************************/
    /* Mouse commands are handled by the async thread. Simply send on the     */
    /* command and parameters.                                                                        */
    /**************************************************************************/
    case WM_BUTTON2DBLCLK:
    case WM_BUTTON2DOWN:
      if( hwnd != WinQueryFocus( HWND_DESKTOP))
              WinSetFocus( HWND_DESKTOP, hwnd);
      if( !fButtonDownMain)
      {
              fButtonDownMain = TRUE;
              SendCommand( UM_LEFTDOWN, mp1, 0);
      }
      return( ( MRESULT)TRUE);
      break;

    case WM_BUTTON2UP:
      if( !fButtonDownMain)
              return( ( MRESULT)TRUE);
      if( SendCommand( UM_LEFTUP, mp1, 0))
              fButtonDownMain = FALSE;
      else
              WinAlarm( HWND_DESKTOP, WA_WARNING);
      return( ( MRESULT)TRUE);
      break;

    case WM_MOUSEMOVE:
      DosQueryEventSem( hevMouse, &ulSemPostCount);
      if( ulSemPostCount)                         /* mouse tracking enabled? */
              SendCommand( UM_MOUSEMOVE, mp1, 0);
      return( WinDefWindowProc( hwnd, msg, mp1, mp2));
      break;
    /**************************************************************************/
    /* Default for the rest                                                   */
    /**************************************************************************/
    default:
      return( WinDefWindowProc( hwnd, msg, mp1, mp2));

  }   /* end switch (msg)  */

  return( FALSE);

}   /* end ClientWndProc() */

/******************************************************************************/
/*                                                                                                                         */
/* WM_PAINT message                                                                                            */
/*                                                                                                               */
/******************************************************************************/
MRESULT WndProcPaint(VOID)
{
  HRGN   hrgnUpdt;
  SHORT  sRgnType;

  hrgnUpdt = GpiCreateRegion( hpsPaint, 0L, NULL);
  sRgnType = WinQueryUpdateRegion( hwndClient, hrgnUpdt);
  SendCommand( UM_DRAW, (MPARAM)hrgnUpdt, 0);
  WinValidateRegion( hwndClient, hrgnUpdt, FALSE);
  return( FALSE);
}

/******************************************************************************
*
*  Name        : WndProcCommand
*
*  Description : Process menu item commands, and commands generated from the
*                keyboard via the accelerator table.  Most are handled by
*                the async thread
*
*  Parameters  : HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2
*
*  Return      : MRESULT
*
******************************************************************************/

MRESULT WndProcCommand(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  ULONG     ulSemPostCount;
  ULONG     ulLoadPostCount;

   DosQueryEventSem( hevLoadingBitmap, &ulLoadPostCount); /* drawing enabled? */

   switch( SHORT1FROMMP(mp1))
   {
      case IDM_SIZE_SMALL:

         if (ulLoadPostCount == 0)    /* is a bitmap currently being loaded? */
            break;                    /* ignore message while loading bitmap */
         CheckMenu(Currentchecked, FALSE);       /* remove check on old menu */
         SizeBitmap(-8);                            /* change transform size */
         CheckMenu(IDM_SIZE_SMALL, TRUE);               /* add check to menu */
         Currentchecked = IDM_SIZE_SMALL;               /* save cur. menu id */

         break;

      case IDM_SIZE_MEDIUM:

         if (ulLoadPostCount == 0)    /* is a bitmap currently being loaded? */
            break;                    /* ignore message while loading bitmap */
         CheckMenu(Currentchecked, FALSE);       /* remove check on old menu */
         SizeBitmap(-4);                            /* change transform size */
         CheckMenu(IDM_SIZE_MEDIUM, TRUE);              /* add check to menu */
         Currentchecked = IDM_SIZE_MEDIUM;              /* save cur. menu id */

         break;

      case IDM_SIZE_LARGE:

         if (ulLoadPostCount == 0)    /* is a bitmap currently being loaded? */
            break;                    /* ignore message while loading bitmap */
         CheckMenu(Currentchecked, FALSE);       /* remove check on old menu */
         SizeBitmap(-2);                            /* change transform size */
         CheckMenu(IDM_SIZE_LARGE, TRUE);               /* add check to menu */
         Currentchecked = IDM_SIZE_LARGE;               /* save cur. menu id */

         break;

      case IDM_SIZE_FULL:

         if (ulLoadPostCount == 0)    /* is a bitmap currently being loaded? */
            break;                    /* ignore message while loading bitmap */
         CheckMenu(Currentchecked, FALSE);       /* remove check on old menu */
         SizeBitmap(0);                             /* change transform size */
         CheckMenu(IDM_SIZE_FULL, TRUE);                /* add check to menu */
         Currentchecked = IDM_SIZE_FULL;                /* save cur. menu id */

         break;

      case IDM_JUMBLE:

         DosResetEventSem( hevDrawOn, &ulSemPostCount);  /* disallow drawing */
         SendCommand( UM_JUMBLE, 0, 0);

         break;

      case IDM_LOAD:

         FileOpen();           /* Open file and load/draw the selected image */

         break;

      /**********************************************************************/
      /* Pass on the rest to the async thread.                              */
      /**********************************************************************/

      case IDM_HELPHELPFORHELP:

         HelpHelpForHelp(mp2);

         break;

      case IDM_HELPEXTENDED:

         DisplayHelpPanel(PANEL_EXTENDED_CONTENTS);

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


      /**********************************************************************/
      /* Unrecognized => default                                            */
      /**********************************************************************/

      default:
         return( WinDefWindowProc(hwnd, msg, mp1, mp2));
   }

   return( FALSE);

}   /* end WndProcCommand() */


/****************************************************************\
 *  Open file routine
 *--------------------------------------------------------------
 *
 *  Name:   FileOpen()
 *
 *  Purpose: Processes the File menu's Open item.
 *
 *  Usage:  called whenever New from the File menu is selected
 *
 *  Method:  calls the standard file open dialog to get the
 *          file name.  The file name is passed onto DosOpen
 *          which returns the handle to the file.  The file
 *          input procedure is called and then the file handle
 *          is closed.
 *
 *  Returns:
 *
\****************************************************************/
BOOL FileOpen( VOID )
{
 BOOL        fSuccess = FALSE;
 PCH         pch;
 FILEDLG     fdg;
 CHAR        szHeader[MESSAGELEN], szButton[MESSAGELEN];
 CHAR        szExtension[FILEPROTOSIZE];
 ULONG       ulDummy;                       /* local temporary variable  */
 ULONG       ulSemPostCount;
 FILEFINDBUF FileBuff;     /*  used to locate default directory */
 HDIR        hdir  = HDIR_SYSTEM;
 ULONG       Count = 1;

   /*
    * set up structure to pass to standard file dialog
    */
 fdg.cbSize = sizeof(FILEDLG);
 fdg.fl = FDS_CENTER | FDS_HELPBUTTON | FDS_OPEN_DIALOG;

 if (!WinLoadString(habMain, 0, IDS_OPENDLGHEADER, MESSAGELEN, szHeader))
   {
   MessageBox(hwndFrame, IDS_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
   return FALSE;
   }
 fdg.pszTitle = szHeader;                            /* dialog title */

 if (!WinLoadString(habMain, 0, IDS_OPENDLGBUTTON, MESSAGELEN, szButton))
   {
   MessageBox(hwndFrame, IDS_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
   return FALSE;
   }
 fdg.pszOKButton = szButton;                 /* <ENTER> button text */

 fdg.ulUser = 0L;
 fdg.pfnDlgProc = (PFNWP)JigsawOpenFilterProc;
 fdg.lReturn = 0L;
 fdg.lSRC = 0;
 fdg.hMod = 0;
 fdg.usDlgId = 0;
 fdg.x = 0;
 fdg.y = 0;

    /* setup EA type list & initial type */
 fdg.pszIType = (PSZ)NULL;
 fdg.papszITypeList = (PAPSZ)apszITL;

    /* setup drive list & initial drive */
 fdg.pszIDrive = (PSZ)NULL;
 fdg.papszIDriveList = (PAPSZ)NULL;

   /*
    * get the default file extension (i.e. *.BMP)
    */
 if (!WinLoadString(habMain,  0, IDS_FILEOPENEXTENSION, FILEPROTOSIZE,
                                                               szExtension))
   {
   MessageBox(hwndFrame, IDS_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
   return FALSE;
   }

   /*
    * if dialog has been previously invoked, then ensure the
    * dialog is brought with the last user-selected directory
    */
 if (*pli->szFileName)
   {
   pch = strrchr(pli->szFileName, '\\');
   strcpy(++pch, szExtension);
   strcpy(fdg.szFullFile, pli->szFileName);
   }
 else       /* search for \os2\bitmap directory */
   {
   if (!DosFindFirst(DEFAULT_DIR, &hdir, FILE_DIRECTORY, &FileBuff,
                              sizeof(FILEFINDBUF), &Count, FIL_STANDARD))
     sprintf(fdg.szFullFile, "%s\\%s", DEFAULT_DIR, szExtension);
   else
     strcpy(fdg.szFullFile, szExtension);
   }


 fdg.sEAType = -1;
 fdg.papszFQFilename = 0;
 fdg.ulFQFCount = 0;

   /*
    * invoke the standard file dialog and get a file
    */
 if (!WinFileDlg(HWND_DESKTOP, hwndFrame, (PFILEDLG)&fdg))
   {
   MessageBox(hwndFrame, IDS_CANNOTRUNFILEDLG,MB_OK | MB_ERROR, TRUE);
   return FALSE;
   }

   /*
    *  Upon sucessful return of a file, open it for reading
    */
 if (fdg.lReturn == DID_OK)
 {
   strcpy(pli->szFileName, fdg.szFullFile);
   DosOpen(
      pli->szFileName,       /* pointer to filename                          */
      &(pli->hf),            /* pointer to variable for file handle          */
      &ulDummy,              /* pointer to variable for action taken         */
      0,                     /* file size if created or truncated            */
      FILE_NORMAL,           /* file attribute                               */
      FILE_OPEN,             /* action taken if file exists/does not exist   */
      OPEN_ACCESS_READONLY | /* open mode of file                            */
       OPEN_SHARE_DENYWRITE,
      NULL);                 /* pointer to structure for extended attributes */

   DosResetEventSem( hevDrawOn, &ulSemPostCount);        /* disallow drawing */
   SendCommand( UM_LOAD, (MPARAM)pli, 0);
 }
}   /* end FileOpen() */


VOID SizeBitmap(LONG lTempScale )
{
   ULONG  ulSemPostCount;

        lScale = lTempScale;
        DosResetEventSem( hevDrawOn, &ulSemPostCount);   /* disallow drawing */
        DosPostEventSem( hevKillDraw);
        SendCommand( UM_ZOOM, MPVOID, MPVOID);
} /* SizeBitmap */



/****************************************************************\
 *  Message Box procedure
 *--------------------------------------------------------------
 *
 *  Name:   MessageBox(hwndOwner, idMsg, fsStyle, fBeep)
 *
 *  Purpose: Displays the message box with the message
 *              given in idMsg retrived from the message table
 *              and using the style flags in fsStyle
 *
 *  Usage:  Called whenever a MessageBox is to be displayed
 *
 *  Method: - Message string is loaded from the process'
 *              message table
 *          - Alarm beep is sounded if desired
 *          - Message box with the message is displayed
 *          - WinMessageBox return value is returned
 *
 *  Returns: return value from WinMessageBox()
 *
\****************************************************************/
ULONG MessageBox(HWND hwndOwner, ULONG idMsg, ULONG fsStyle, BOOL fBeep)
{
   CHAR szText[MESSAGELEN];
   PSZ  pszTitle;

   if (fsStyle & MB_ERROR)
      pszTitle = NULL;                                 /* default is "Error" */
   else
      pszTitle = szTitle;                     /* use "Jigsaw" for non-errors */

   if (!WinLoadString(habMain,
           (HMODULE)NULLHANDLE,
           idMsg,
           MESSAGELEN,
           (PSZ)szText))
   {
      if (idMsg == IDS_CANNOTLOADSTRING)
      {
         strcpy(szText, "Failed to load resource string");
      }
      else
      {
         WinAlarm(HWND_DESKTOP, WA_ERROR);
         return MBID_ERROR;
      }
   }

   if (fBeep)
      WinAlarm(HWND_DESKTOP, WA_ERROR);

   return WinMessageBox(HWND_DESKTOP,     /* handle of the parent window     */
             hwndOwner,                   /* handle of the owner window      */
             szText,                      /* address of text in message box  */
             pszTitle,                    /* address of title of message box */
             MSGBOXID,                    /* message-box identifier          */
             fsStyle);                    /* type of message box             */

}   /* end MessageBox() */

/***************************************************************************
 *** JigsawOpenFilterProc - This is a procedure that will filter the help
 *                           messages to the open dialog.
***************************************************************************/
MRESULT EXPENTRY JigsawOpenFilterProc(HWND hwnd,ULONG message,MPARAM mp1,MPARAM mp2 )
{
  if(message == WM_HELP)
  {
       DisplayHelpPanel(HID_FS_OPEN_DLG_HELP_PANEL);
       return FALSE ;
   }

   return WinDefFileDlgProc( hwnd, message, mp1, mp2 ) ;
}

/*******************************  END PROCS.C  *******************************/

/**************************************************************************
 *  File name  :  file.c
 *
 *  Description:  This module contains the code for the WM_COMMAND messages
 *                posted by the standard File menu.
 *
 *                This source file contains the following functions:
 *
 *                FileNew(mp2);
 *                FileOpen(mp2);
 *                FileSave(mp2);
 *                FileSaveAs(mp2);
 *                WriteFileToDisk(hf);
 *                GetFileName();
 *                UpdateTitleText(hwnd);
 *                TemplateOpenFilterProc(hwnd, msg, mp1, mp2)
 *                TemplateSaveFilterProc(hwnd, msg, mp1, mp2)
 *
 *  Concepts   :  file management
 *
 *  API's      :  WinLoadString
 *                WinFileDlg
 *                DosOpen
 *                DosClose
 *                WinSetWindowText
 *                WinWindowFromID
 *                WinDefFileDlgProc
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, MAIN.H, HELP.H, XTRN.H
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
/*
 *  Include files, macros, defined constants, and externs
 */

#define INCL_WINFRAMEMGR
#define INCL_WINSWITCHLIST
#define INCL_WINSTDFILE

#include <os2.h>
#include <string.h>
#include "main.h"
#include "xtrn.h"
#include "help.h"

/*
 *  Global variables
 */

CHAR szFullPath[CCHMAXPATH] = "";

/*
 *  Entry point declarations
 */


/**************************************************************************
 *
 *  Name       : FileNew(mp2)
 *
 *  Description: Processes the File menu's New item
 *
 *  Concepts:  called whenever NEW from the FILE menu is selected
 *
 *  API's      : [none]
 *
 *  Parameters :  mp2      = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID FileNew(MPARAM mp2)
{
    /*
     *  Enter routines for creating a new file and window
     */

    /* clear file name and reset the titlebar text */
   szFullPath[0] = '\0';
   UpdateTitleText(hwndMainFrame);

    /*  This routine currently doesn't use the mp2 parameter but
     *  it is referenced here to prevent an 'Unreferenced Parameter'
     *  warning at compile time.
     */
    mp2;
}   /* End of FileNew   */


/**************************************************************************
 *
 *  Name       : FileOpen(mp2)
 *
 *  Description: Processes the File menu's Open item
 *
 *  Concepts:  Called whenever OPEN from the FILE menu is selected
 *
 *             Calls the standard file open dialog to get the
 *             file name.  The file name is passed onto DosOpen
 *             which returns the handle to the file.  The file
 *             input procedure is called and then the file handle
 *             is closed.
 *
 *  API's      : WinLoadString
 *               WinFileDlg
 *               DosOpen
 *               DosClose
 *
 *  Parameters :  mp2      = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID FileOpen(MPARAM mp2)
{
   FILEDLG fdg;
   HFILE hfIn;
   ULONG ulAction;
   CHAR szTitle[MESSAGELEN], szButton[MESSAGELEN];

   fdg.cbSize = sizeof(FILEDLG);

   if(!WinLoadString(hab, (HMODULE)0, IDS_OPEN, MESSAGELEN, szTitle))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
      return;
   }

   if(!WinLoadString(hab, (HMODULE)0, IDS_OPEN, MESSAGELEN, szButton))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
      return;
   }

   fdg.pszTitle = szTitle;
   fdg.pszOKButton = szButton;
   fdg.ulUser = 0L;
   fdg.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG;
   fdg.pfnDlgProc = (PFNWP)TemplateOpenFilterProc;
   fdg.lReturn = 0L;
   fdg.lSRC = 0L;
   fdg.hMod = 0;
   fdg.usDlgId = FILEOPEN;
   fdg.x = 0;
   fdg.y = 0;

   if(!WinLoadString(hab, (HMODULE)0, IDS_FILEOPENEXT, CCHMAXPATH,
                     fdg.szFullFile))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
      return;
   }

   fdg.pszIType       = (PSZ)NULL;
   fdg.papszITypeList = (PAPSZ)NULL;
   fdg.pszIDrive      = (PSZ)NULL;
   fdg.papszIDriveList= (PAPSZ)NULL;
   fdg.sEAType        = (SHORT)0;
   fdg.papszFQFilename= (PAPSZ)NULL;
   fdg.ulFQFCount     = 0L;

   /* get the file */
   if(!WinFileDlg(HWND_DESKTOP, hwndMain, &fdg))
      return;

   /*
    *  Upon sucessful return of a file, open it for reading
    */

   if(fdg.lReturn == ID_OK)
   {
      if( DosOpen(fdg.szFullFile,     /* file name from Open dialog */
                  &hfIn,              /* file handle returned */
                  &ulAction,
                  0L,
                  FILE_NORMAL,
                  FILE_OPEN,
                  OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                  (PEAOP2)NULL))
      {
         MessageBox(hwndMain,
                      IDMSG_CANNOTOPENINPUTFILE,
                      MB_OK | MB_ERROR,
                      FALSE);

           /*
            *  NOTE:  You now have several options on how to proceed
            *      from this point:
            *          - You can abort the File Open by returning from
            *            this procedure.
            *          - You can bring up the File Open dialog again
            *            and have the user pick another file.
            *          - You can check the error code from the DosOpen,
            *            determine why the open failed, and take an
            *            action appropriate to the specific failure.
            */

         return;
      }

       /* copy file name into file name buffer */
      strcpy(szFullPath, fdg.szFullFile);

       /*
        *  Place routine for reading the file here.
        */

      DosClose(hfIn);

      UpdateTitleText(hwndMainFrame);
   }

   /* This routine currently doesn't use the mp2 parameter but       *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                      */
   mp2;
}   /* End of FileOpen   */

/**************************************************************************
 *
 *  Name       : FileSave(mp2)
 *
 *  Description: Processes the File menu's Save item
 *
 *  Concepts:  Called whenever SAVE from the FILE menu is selected
 *
 *             Routine opens the file for output, calls the
 *             application's save routine, and closes the file.
 *
 *  API's      : DosOpen
 *               DosClose
 *
 *  Parameters :  mp2      = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID FileSave(MPARAM mp2)
{
   HFILE hf;
   ULONG ulAction;
   /*
    * If the file currently is untitled, we will need to get a file
    * name from the user before we can open the file.  Getting a
    * file name is normally done during the FileSaveAs operation
    * so we will treat this save as a SaveAs and call FileSaveAs().
    * If the file is titled, then we save the file.
    *
    * NOTE:  This routine will be called by FileSaveAs(), but only
    *  after a valid file name has been obtained.  So, FileSaveAs()
    *  will not be called again from this routine.
    */
   if(szFullPath[0] == '\0')
   {
      FileSaveAs(mp2);
      return;
   }

   /* open the file */
   if( DosOpen(szFullPath,         /* file name of current document */
               &hf,                /* file handle of output file */
               &ulAction,
               0L,
               FILE_NORMAL,
               FILE_OPEN | FILE_CREATE,
               OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
               (PEAOP2)NULL))
   {
       MessageBox(hwndMain,
                  IDMSG_CANNOTOPENOUTPUTFILE,
                  MB_OK | MB_ERROR,
                  FALSE);
       return;
   }

   WriteFileToDisk(hf);

   DosClose(hf);

   /* This routine currently doesn't use the mp2 parameter but       *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'
   \*  warning at compile time.                                      */
   mp2;
}   /* End of FileSave   */

/**************************************************************************
 *
 *  Name       : FileSaveAs(mp2)
 *
 *  Description: Processes the File menu's Save as item
 *
 *  Concepts:  Called whenever SAVE AS from the FILE menu is selected
 *
 *             Routine prompts the user for the name of the file,
 *             then saves the file.
 *
 *  API's      : DosOpen
 *               DosClose
 *
 *  Parameters :  mp2      = second message parameter
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID FileSaveAs(MPARAM mp2)
{
   HFILE hf;
   ULONG ulAction;
   ULONG sT;

   while(TRUE)     /* infinite loop until we break out of it */
   {
      /* Get a name for the file */
      if(!GetFileName())
         return;

      /* See if the file exists.  If it does, then confirm that the
       * user wants to overwrite it.  If he doesn't, then get a new
       * file name
       */
      if( DosOpen(szFullPath,     /* file name from, GetFileName() */
                  &hf,            /* handle of opened file */
                  &ulAction,
                  0L,
                  FILE_NORMAL,
                  FILE_CREATE,
                  OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
                  (PEAOP2)NULL))
      {
         MessageBox(hwndMain,
                    IDMSG_CANNOTOPENOUTPUTFILE,
                    MB_OK | MB_ERROR,
                    FALSE);
         return;
      }
      else
         DosClose(hf);

      /* if file exists, ask if we want to overwrite it */
      if(ulAction == FILE_EXISTED)
      {
         sT = MessageBox(hwndMain,
                         IDMSG_OVERWRITEFILE,
                         MB_QUERY | MB_YESNOCANCEL,
                         FALSE);

         if(sT == MBID_CANCEL)
            return;

         if(sT == MBID_YES)
            break;

          /* if user selected no, repeat the sequence */
      }

   }   /* while(TRUE) */

   UpdateTitleText(hwndMainFrame);

   /*
    * Now that we have a valid file name, save the file.  This is
    * normally done under the File Save function so we can just
    * call the FileSave() function here.  Note that FileSave() will
    * not call FileSaveAs() back since there is a valid file name
    */
    FileSave(mp2);

   /*  This routine currently doesn't use the mp2 parameter but
    *  it is referenced here to prevent an 'Unreferenced Parameter'
    *  warning at compile time.
    */
   mp2;
}   /* End of FileSaveAs   */


/**************************************************************************
 *
 *  Name       : WriteFileToDisk(hfile)
 *
 *  Description: writes the current file to the file in hf
 *
 *  Concepts:  called from FileSave and FileSaveAs when a fileelected
 *             is to be saved to disk
 *
 *             NOTE:  This routine must not close the file.
 *
 *  API's      : [none]
 *
 *  Parameters :  hfile    = file handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID WriteFileToDisk(HFILE hf)                                          \
{
    /*
     *  Place routine to write a disk file here.
     */

    hf;
}   /* End of WriteFileToDisk   */


/**************************************************************************
 *
 *  Name       : GetFileName()
 *
 *  Description: gets the name of the save file
 *
 *  Concepts:    called when the user needs to supply a name for
 *               the file to be saved
 *
 *                calls the standard file open dialog to get the
 *               file name.
 *
 *  API's      : WinLoadString
 *               WinFileDlg
 *
 *  Parameters : [none]
 *
 *  Return     :  TRUE if successful in getting a file name
 *                FALSE if not successful in getting a file name
 *
 *************************************************************************/
BOOL GetFileName(VOID)
{
   FILEDLG fdg;
   CHAR szTitle[MESSAGELEN], szButton[MESSAGELEN];

   fdg.cbSize = sizeof(FILEDLG);

   if(!WinLoadString(hab, 0, IDS_SAVE, MESSAGELEN, szTitle))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
       return FALSE;
   }

   if(!WinLoadString(hab, 0, IDS_SAVE, MESSAGELEN, szButton))
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
       return FALSE;
   }

   fdg.pszTitle = szTitle;
   fdg.pszOKButton = szButton;

   fdg.ulUser = 0L;
   fdg.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_SAVEAS_DIALOG;
   fdg.pfnDlgProc = (PFNWP)TemplateSaveFilterProc;
   fdg.lReturn = 0L;
   fdg.lSRC = 0L;
   fdg.hMod = (HMODULE)NULL;
   fdg.usDlgId = FILESAVE;
   fdg.x = 0;
   fdg.y = 0;
   fdg.pszIType = (PSZ)NULL;
   fdg.papszITypeList = (PAPSZ)NULL;
   fdg.pszIDrive = (PSZ)NULL;
   fdg.papszIDriveList = (PAPSZ)NULL;
   fdg.sEAType = (SHORT)0;
   fdg.papszFQFilename = (PAPSZ)NULL;
   fdg.ulFQFCount = 0L;

   strcpy(fdg.szFullFile, szFullPath);

   /* get the file */
   if(!WinFileDlg(HWND_DESKTOP, hwndMain, &fdg))
       return FALSE;

   if(fdg.lReturn != ID_OK)
       return FALSE;

   /* copy file name and path returned into buffers */
   strcpy(szFullPath, fdg.szFullFile);

   return TRUE;
}   /* End of GetFileName   */

/**************************************************************************
 *
 *  Name       : UpdateTitleText
 *
 *  Description: Updates the text in the main window's title bar to
 *               display the app name, followed by the separator,
 *               followed by the file name.
 *
 *  Concepts :   Called at init time and when the text file is changed.
 *               Gets the program name, appends the separator, and
 *               appends the file name.
 *
 *  API's      : WinLoadString
 *               WinSetWindowText
 *               WinWindowFromID
 *
 *  Parameters : hwnd = window handle
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID UpdateTitleText(HWND hwnd)
{
   CHAR szBuf[MAXNAMEL+TITLESEPARATORLEN+CCHMAXPATH];
   CHAR szSeparator[TITLESEPARATORLEN+1];
   PSZ pszT;

   WinLoadString(hab, (HMODULE)0, IDS_APPNAME, MAXNAMEL, szBuf);
   WinLoadString(hab,
                 (HMODULE)0,
                 IDS_TITLEBARSEPARATOR,
                 TITLESEPARATORLEN,
                 szSeparator);

   strcat(szBuf, szSeparator);

   if(szFullPath[0] == '\0')
      pszT = szUntitled;
   else
      pszT = szFullPath;

   strcat(szBuf, pszT);

   WinSetWindowText(WinWindowFromID(hwnd, FID_TITLEBAR), szBuf);
}   /* End of UpdateTitleText   */

/**************************************************************************
 *
 *  Name       : TemplateOpenFilterProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Filters the help messages to the open dialog
 *
 *  Concepts :   If help is requested during the standard OPEN
 *               dialog, this routine will display an appropriate
 *               panel.
 *
 *  API's      : WinDefFileDlgProc
 *
 *  Parameters : hwnd = window handle
 *               message = message i.d.
 *               mp1     = first message parameter
 *               mp2     = second message parameter
 *
 *  Return     :  the result of the message
 *
 *************************************************************************/
MRESULT EXPENTRY TemplateOpenFilterProc(HWND hwnd, ULONG message,
                                        MPARAM mp1, MPARAM mp2 )
{
   if(message == WM_HELP)
   {
      DisplayHelpPanel(HID_FS_OPEN_DLG_HELP_PANEL);
      return (MRESULT)FALSE ;
   }

   return WinDefFileDlgProc( hwnd, message, mp1, mp2 ) ;
}  /*  End of TemplateSaveFilterProc  */


/**************************************************************************
 *
 *  Name       : TemplateSaveFilterProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Filters the help messages to the save dialog
 *
 *  Concepts :   If help is requested during the standard SAVEAS
 *               dialog, this routine will display an appropriate
 *               panel.
 *
 *  API's      : WinDefFileDlgProc
 *
 *  Parameters : hwnd = window handle
 *               message = message i.d.
 *               mp1     = first message parameter
 *               mp2     = second message parameter
 *
 *  Return     :  the result of the message
 *
 *************************************************************************/
MRESULT EXPENTRY TemplateSaveFilterProc(HWND hwnd,
                                        ULONG message,
                                        MPARAM mp1,
                                        MPARAM mp2 )
{
   if(message == WM_HELP)
   {
      DisplayHelpPanel(HID_FS_SAVE_DLG_HELP_PANEL);
      return (MRESULT)FALSE ;
   }

   return WinDefFileDlgProc( hwnd, message, mp1, mp2 );
}  /*  End of TemplateSaveFilterProc  */
/***************************  End of file.c  ***************************/

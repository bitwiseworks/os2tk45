/**************************************************************************
 *  File name  :  dragdrop.c
 *
 *  Description:  This sample creates a listbox in the client area and
 *                populates it with the members of the current directory.
 *                The user can navigate downward in the directory hierarchy
 *                by using a directory selection dialog or by
 *                double-clicking on a directory in the list.  The user can
 *                navigate upward by using the selection dialog.  Files can
 *                be moved between drives and/or directories by selecting
 *                one or more files from one running instance of the program
 *                (A) and pressing and holding mouse button 2 to drag them
 *                to another running instance of the program (B).  When
 *                button 2 is released over the target program, the files
 *                selected from A are moved to the directory currently
 *                displayed by B.
 *
 *                This source file contains the following functions:
 *
 *                main()
 *                ClientWndProc(hwnd, msg, mp1, mp2)
 *                OpenDlgProc(hwnd, msg, mp1, mp2)
 *                ChangeDir(pszDirectory)
 *                PopulateList(hwndContainer, pszDir)
 *                ListSubclassProc(hwnd, msg, mp1, mp2)
 *                Message(pszMsg)
 *                MessageBox(hwndOwner, idMsg, fsStyle, fBeep)
 *
 *  Concepts   :  listboxes, directory search
 *
 *  API's      :  WinInitialize
 *                DosBeep
 *                WinCreateMsgQueue
 *                WinTerminate
 *                WinLoadString
 *                WinRegisterClass
 *                WinCreateStdWindow
 *                WinGetMsg
 *                WinDispatchMsg
 *                WinDestroyWindow
 *                WinDestroyMsgQueue
 *                DosQueryCurrentDir
 *                WinQuerySysPointer
 *                WinLoadPointer
 *                WinPostMsg
 *                WinSetPointer
 *                WinDestroyPointer
 *                WinSetWindowPos
 *                WinQueryWindowRect
 *                WinCreateWindow
 *                WinSubclassWindow
 *                WinSetFocus
 *                WinWindowFromID
 *                WinBeginPaint
 *                GpiErase
 *                WinEndPaint
 *                WinDlgBox
 *                WinQueryLboxCount
 *                WinQueryLboxItemText
 *                WinDefWindowProc
 *                WinSendDlgItemMsg
 *                WinSetDlgItemText
 *                WinQueryWindowText
 *                WinWindowFromID
 *                WinDismissDlg
 *                WinDefDlgProc
 *                DosSetCurrentDir
 *                WinSetWindowText
 *                WinEnableWindowUpdate
 *                WinShowWindow
 *                WinMessageBox
 *                WinLoadMessage
 *                WinAlarm
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, STDIO.H, STDLIB.H, DRAGDROP.H
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
 *        Include the required sections from the PM header file
 */
#define INCL_DOSFILEMGR
#define INCL_WIN
#define INCL_WINSTDDRAG

/*
 *                     Include the PM header file
 */
#include <os2.h>

/*
 *                  Include the C library header files
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *                 Include the application header files
 */
#include "dragdrop.h"

/*
 * Variables global to file
 */
static HAB   hab;
static CHAR  szCurrentDir[CCHMAXPATH]; /* always contains the current dir   */
static CHAR  szTemp[CCHMAXPATH];
static CHAR  szClientClass[12];        /* "DragDrop"                        */
static HWND  hwndClient;               /* needed by change dir function     */
static PFNWP pfnwpList;                /* save listbox proc address         */
static HWND  hwndList;                 /* handle to list control            */

static HPOINTER hptrBusy;              /* Mouse Pointer to show when a      */
                                       /* drag is in progress               */

USHORT cDragFiles;

/**************************************************************************
 *
 *  Name       : main()
 *
 *  Description: Initializes the PM environment, calls the
 *               initialization routine, creates the main
 *               window,  and polls the message queue
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
 *                 DosBeep
 *                 WinCreateMsgQueue
 *                 WinTerminate
 *                 WinLoadString
 *                 WinRegisterClass
 *                 WinCreateStdWindow
 *                 WinGetMsg
 *                 WinDispatchMsg
 *                 WinDestroyWindow
 *                 WinDestroyMsgQueue
 *
 *  Parameters :  [none]
 *
 *  Return     :  1 - if successful execution completed
 *                0 - if error
 *
 *************************************************************************/
int main(void)
{
   ULONG flFrameFlags = FCF_STANDARD;
   HMQ   hmq;
   HWND  hwndFrame;
   QMSG  qmsg;

   hab = WinInitialize (0);
   if(!hab)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      return(RETURN_ERROR);
   }

   hmq = WinCreateMsgQueue (hab, 0);

   if(!hmq)
   {
      DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
      WinTerminate(hab);
      return(RETURN_ERROR);
   }

   if(!WinLoadString(hab, 0, IDS_CLIENTCLASS,
                    sizeof(szClientClass), szClientClass))
      return 0;
   WinRegisterClass (hab, szClientClass, ClientWndProc, 0L, 0L);

   if (0L != (hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_VISIBLE,
                                 &flFrameFlags, (PSZ)szClientClass, (PSZ)NULL,
                                 0L, (HMODULE)0L, ID_RESOURCE,
                                 &hwndClient)))

                /* Get/Dispatch Message loop */
   {
      InitHelp(hab, hwndFrame);
      while (WinGetMsg (hab, &qmsg, 0L, 0, 0))
         WinDispatchMsg (hab, &qmsg);

      WinDestroyWindow (hwndFrame);

    /* destroy the help instance */
      DestroyHelpInstance();
   }
   else
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_MAINWINCREATEFAILED,
                 MB_OK | MB_ERROR,
                 TRUE);
      return(RETURN_ERROR);
   }

   WinDestroyMsgQueue (hmq);
   WinTerminate (hab);
}   /* End of main */


/**************************************************************************
 *
 *  Name       : ClientWndProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Processes the messages sent to the main client
 *               window.  This routine processes the basic
 *               messages all client windows should process.
 *
 *  Concepts   : Called for each message placed in the main
 *               window's message queue
 *
 *               A switch statement branches to the routines to be
 *               performed for each message processed.  Any messages
 *               not specifically process are passed to the default
 *               window procedure.
 *
 *  API's      : DosQueryCurrentDir
 *               WinQuerySysPointer
 *               WinLoadPointer
 *               WinPostMsg
 *               WinSetPointer
 *               WinDestroyPointer
 *               WinSetWindowPos
 *               WinQueryWindowRect
 *               WinCreateWindow
 *               WinSetFocus
 *               WinBeginPaint
 *               GpiErase
 *               WinEndPaint
 *               WinDlgBox
 *               WinQueryLboxCount
 *               WinQueryLboxItemText
 *               WinDefWindowProc
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  values are determined by each message
 *
 *************************************************************************/
MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static HPOINTER hptrFile, hptrFolder, hptrMulti;
   SHORT           cxClient, cyClient;
   HPS             hps;
   RECTL           rClient;
   USHORT          Item;
   USHORT          i;
   ULONG           ulDiskNum, ulLogicalDisk;

   switch (msg)
   {
   case WM_CREATE:
      hwndClient = hwnd;

       /*
        * Initialize strings for title and default directory
        * and change the current directory to the default directory
        */
      DosQueryCurrentDisk(&ulDiskNum, &ulLogicalDisk);
      szCurrentDir[0] = (CHAR)('A' - 1 + ulDiskNum);
      szCurrentDir[1] = ':';
      szCurrentDir[2] = '\\';

      {
         PBYTE   pBuf   = szCurrentDir + 3;
         ULONG   cbBuf  = CCHMAXPATH - 3;
         PULONG  pcbBuf = &cbBuf;		

         DosQueryCurrentDir(0L, pBuf, pcbBuf);
         if(szCurrentDir[3] == '\\')
            szCurrentDir[3] = '\0';    /*  account for the root, if current */
      }
      ChangeDir(szCurrentDir);

       /*
        * Load the standard system pointers for files, folders, multi-files
        * and programs
        */
      hptrFile   = WinQuerySysPointer (HWND_DESKTOP, SPTR_FILE, FALSE);
      hptrFolder = WinQuerySysPointer (HWND_DESKTOP, SPTR_FOLDER, FALSE);
      hptrMulti  = WinQuerySysPointer (HWND_DESKTOP, SPTR_MULTFILE, FALSE);
      hptrBusy   = WinLoadPointer     (HWND_DESKTOP, (HMODULE)0L, ID_BUSY);

       /*
        * Post ourselves a message so that the list box can be created
        */
      WinPostMsg(hwnd, WM_CREATELIST, NULL, NULL);
      return ((MRESULT) FALSE);

   case WM_MOUSEMOVE:
      if (CheckBusy() || cDragFiles)
      {
         WinSetPointer(HWND_DESKTOP, hptrBusy);
         return ((MRESULT) TRUE);
      }
      break;

   case WM_CLOSE:
       /*
        * Check to see if a drag is in progress and do not allow a close if
        * that is the case.
        */
      return (CheckClose (hwnd));

   case WM_DESTROY:
      WinDestroyPointer (hptrFile);
      WinDestroyPointer (hptrFolder);
      WinDestroyPointer (hptrMulti);
      WinDestroyPointer (hptrBusy);
      return (NULL);

   case WM_SIZE:
       /*
        * Save our size information and re-size the list box.
        */
      if(hwndList != 0L)
      {
         cxClient = SHORT1FROMMP (mp2);
         cyClient = SHORT2FROMMP (mp2);
         WinSetWindowPos(hwndList, HWND_TOP, 0, 0, cxClient, cyClient,
                         SWP_MOVE | SWP_SIZE | SWP_SHOW);
         return ((MRESULT)FALSE);
      }
      break;

   case WM_CREATELIST:
       /*
        * Create the list control.
        */
      WinQueryWindowRect(hwnd, &rClient);
      cxClient = (SHORT) (rClient.xRight - rClient.xLeft);
      cyClient = (SHORT) (rClient.yTop - rClient.yBottom);
      hwndList = WinCreateWindow(hwnd, WC_LISTBOX, NULL,
                                   WS_VISIBLE | LS_MULTIPLESEL,
                                   0, 0, cxClient, cyClient,
                                   hwnd, HWND_TOP, LIST_ID, NULL, NULL);

       /*
        * If we couldn't create the listbox, display a message.
        */
      if (!hwndList)
      {
         MessageBox(hwnd, IDMSG_CANTCREATELIST, MB_OK | MB_ERROR, TRUE);
      }

       /*
        * Otherwise, the list was created successfully.
        */
      else
      {
         /*
          * Subclass the list in the client area in order to intercept
          * the mouse button 2 and drag messages. Then cause the list to
          * be populated by posting ourselves a refresh message.
          */
         pfnwpList = WinSubclassWindow (hwndList, ListSubclassProc);
         WinPostMsg (hwnd, WM_REFRESH, NULL, NULL);
         WinSetFocus (HWND_DESKTOP, hwndList);
      }
      return ((MRESULT)FALSE);

   case WM_REFRESH:
       /*
        * Refresh the contents of the list by getting rid of its current
        * contents and repopulating it.
        */
      WinDeleteLboxAll(hwndList);              /*  depopulate list  */
      PopulateList (hwndList, szCurrentDir);
      return ((MRESULT)FALSE);

   case WM_PAINT:
       /*
        * Just erase the background as there is no data in the client.
        */
      hps = WinBeginPaint (hwnd, 0L, NULL);
      GpiErase (hps);
      WinEndPaint (hps);
      return ((MRESULT)FALSE);

     /*
      * When the application is not minimized, drag messages will be sent to
      * the list box, as it occupies the entire client window. The client
      * window will see these as LN_drag messages. However, when the
      * application is minimized, the client will see DM_drag messages,
      * and must process these in the same way that the LN_drag messages are
      * processed.  Note that a drag cannot be initiated while the application
      * is minimized.
      */

   case DM_DROPHELP:
      return (DropHelp (hwnd, (PDRAGINFO) mp1));

   case DM_DRAGOVER:
      return (DragOver (hab, (PDRAGINFO) mp1, szCurrentDir));

   case DM_DRAGLEAVE:
      return (DragLeave ());

   case DM_DROP:
      return (Drop(hab, hwnd, (PDRAGINFO) mp1, szCurrentDir));

   case DM_ENDCONVERSATION:
       /*
        * The end conversation message will always be sent to the client
        * window. Just invoke the EndConversation function. It will take
        * care of freeing the draginfo structure when its appropriate.
        */
      return (EndConversation (hab, hwnd, mp1, mp2));

   case WM_TARGETCOMPLETE:
       /*
        * This message is posted whenever a target thread has completed
        * its processing. All resources allocated to the drag operation
        * will be freed and the source will be notified that the
        * operation(s) is complete.
        */
      return (EndTarget (hwnd, PVOIDFROMMP (mp1)));

   case WM_COMMAND:
       /*
        *  Handle action bar commands.
        */
      switch (SHORT1FROMMP(mp1))
      {
      case IDM_FILEOPEN:
         if (WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP)OpenDlgProc, (HMODULE)0L,
                        IDD_OPEN, NULL))
         {
            ChangeDir (szCurrentDir);
            WinPostMsg (hwnd, WM_REFRESH, NULL, NULL);
         }
         break;

      case IDM_WINDOWSELECTALL:
      case IDM_WINDOWDESELECTALL:
      {
         USHORT cItems;

         cItems = WinQueryLboxCount (hwndList);

         for (i=0; i<cItems; i++)
         {
             WinSelectLboxItem (hwndList, i,
                               ((SHORT1FROMMP(mp1)) == IDM_WINDOWSELECTALL));
         }
         break;
      }

      case IDM_WINDOWREFRESH:
         WinPostMsg (hwnd, WM_REFRESH, NULL, NULL);
         break;

      case IDM_HELPINDEX:
         HelpIndex(hwnd);
         break;

      case IDM_HELPGENERAL:
         HelpGeneral(hwnd);
         break;

      case IDM_HELPUSINGHELP:
         HelpUsingHelp(hwnd);
         break;

      case IDM_HELPPRODUCTINFO:
         HelpProductInfo(hwnd);
         break;
      }
      break;

   case WM_CONTROL:
       /*
        * Handle list notification messages.
        */
      if (SHORT1FROMMP (mp1) == LIST_ID)
      {
         switch (SHORT2FROMMP (mp1))
         {
             case LN_ENTER:
             {
                CHAR szTemp[CCHMAXPATH];

             /*
              * User wishes to open a directory entry
              *   - must be the first item selected
              *   - selected item must be a directory
              */
                Item = (USHORT) WinQueryLboxSelItemFirst (hwndList);

                if (Item != LIT_NONE)
                {
                   WinQueryLboxItemText (hwndList, Item, szTemp, sizeof(szTemp));

               /*
                * Is it a subdirectory?
                */
                   if (szTemp[0] == '\\')
                   {
                 /*
                  * Yes - it's a subdirectory.
                  * Remove final '\\' from current directory, if present
                  * and add the new subdirectory to the current directory.
                  */
                      RemoveFinalBackslash (szCurrentDir);
                      strcat (szCurrentDir, szTemp);
                      ChangeDir (szCurrentDir);
                      WinPostMsg (hwnd, WM_REFRESH, NULL, NULL);
                   }

               /*
                * No, show an error message.
                */
                   else
                   {
                      MessageBox(HWND_DESKTOP,
                                 IDMSG_NOTSUBDIRECTORY,
                                 MB_OK | MB_ERROR,
                                 TRUE);
                   }
                }
                else
                {
                   MessageBox(HWND_DESKTOP,
                              IDMSG_MUSTBESELECTED,
                              MB_OK | MB_ERROR,
                              TRUE);
                }
                break;
             }

             case LN_INITDRAG:
                if(InitDrag(hab, hwnd, hwndList, hptrFile,
                                 hptrFolder, hptrMulti, szCurrentDir))
                   return ((MRESULT)FALSE);
                else
                   return ((MRESULT)TRUE);

             case LN_DROPHELP:
                return (DropHelp (hwnd, (PDRAGINFO) mp2));

             case LN_DRAGOVER:
                return (DragOver (hab, (PDRAGINFO) mp2, szCurrentDir));

             case LN_DRAGLEAVE:
                return (DragLeave ());

             case LN_DROP:
                return (Drop(hab, hwnd, (PDRAGINFO) mp2, szCurrentDir));
          }
       }
       break;
   }
   return WinDefWindowProc (hwnd, msg, mp1, mp2);
} /* End of ClientWndProc */

/**************************************************************************
 *
 *  Name       : OpenDlgProc(hwnd, msg, mp1, mp2)
 *
 * Description: Supports drive and directory navigation through the
 *              file open pull down.  The complete path specification
 *              must be entered by the user.
 *
 *  Concepts   : open dialog
 *
 *  API's      : WinSendDlgItemMsg
 *               WinSetDlgItemText
 *               WinSetFocus
 *               WinWindowFromID
 *               WinQueryWindowText
 *               WinDismissDlg
 *               WinDefDlgProc
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  TRUE  - the directory and the title text for the
 *                window have been successfully changed
 *                FALSE - the input directory was invalid or the user
 *                pressed Cancel
 *
 *************************************************************************/
MRESULT EXPENTRY OpenDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static CHAR szOldDir[CCHMAXPATH];

   switch (msg)
   {
   case WM_INITDLG:
      SetSysMenu(hwnd);                     /* system menu for this dialog  */
      strcpy (szOldDir, szCurrentDir);          /* save directory in case   */
                                                /* new directory is invalid */
      WinSendDlgItemMsg (hwnd, IDD_EDIT, EM_SETTEXTLIMIT,
                         MPFROM2SHORT (256, 0), (MPARAM)NULL);
      WinSetDlgItemText (hwnd, IDD_EDIT, szCurrentDir);
      WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, IDD_EDIT));
      return ((MRESULT) TRUE);

   case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
      {
      case DID_OK:
          /*
           * Get and validate the user input directory.
           */
         WinQueryWindowText (WinWindowFromID (hwnd, IDD_EDIT),
                            sizeof (szCurrentDir), szCurrentDir);

         if (!ChangeDir(szCurrentDir))
         {
          /*
           * New dir is invalid.
           * Notify user and restore old directory.
           */
            MessageBox(HWND_DESKTOP,
                       IDMSG_BADDIRECTORY,
                       MB_OK | MB_ERROR,
                       TRUE);
            strcpy (szCurrentDir, szOldDir);
         }

         WinDismissDlg (hwnd, TRUE);
         return ((MRESULT) TRUE);

      case DID_CANCEL:
         WinDismissDlg (hwnd, FALSE);
         return MPFROMSHORT(FALSE);
      }
      break;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
} /*  End of OpenDlgProc  */

/**************************************************************************
 *
 *  Name       : ChangeDir(psz)
 *
 *  Description: Changes the current directory to the path specified
 *
 *  Concepts   : path
 *
 *  API's      : DosSetCurrentDir
 *               WinLoadString
 *               WinSetWindowText
 *
 *  Parameters :  pszSubdirectory = subdirectory path
 *
 *  Return     :  TRUE  - the directory and the titlebar have been
 *                changed
 *                FALSE - the directory and titlebar have not been
 *                changed
 *
 *************************************************************************/
BOOL ChangeDir (PSZ pszDirectory)
{
   CHAR  szTitle [CCHMAXPATH + 45];

  /*
   * Use DosSetCurrentDir function to determine if input path is valid.
   */

   if (!DosSetCurrentDir(pszDirectory))
   {
                     /* load application name from resource file */
      if(!WinLoadString(hab, 0, IDS_APPNAME, CCHMAXPATH, szTitle))
         return FALSE;
      strcat (szTitle, pszDirectory);
      WinSetWindowText (ParentOf (hwndClient), szTitle);
      return TRUE;
   }
   return (FALSE);
}  /*  End of ChangeDir  */

/**************************************************************************
 *
 *  Name       : PopulateList(hwndContainer, pszDir)
 *
 *  Description: Add all files in the directory to the list, except
 *               for the current and parent subdirectory entries
 *
 *  Concepts   : directory, list box
 *
 *  API's      : WinEnableWindowUpdate
 *               WinInsertLboxItem
 *               WinShowWindow
 *
 *  Parameters :  hwndContainer = handle of the listbox
 *                pszDir        = directory name string
 *
 *  Return     :  TRUE  - the list has been populated
 *                FALSE - the list was not populated, because the
 *                        directory string was empty
 *
 *************************************************************************/
BOOL PopulateList (HWND hwndContainer, PSZ pszDir)
{
   char            szFilename[CCHMAXPATH];
   PPUBFILEFINDBUF pffbFile;

   if (!pszDir || !*pszDir)
   {
      return (FALSE);
   }

  /*
   * Create the directory portion of the filename by concatenating a '\'
   * to the input directory, if there is not already a '\' present
   */
   strcpy (szFilename, pszDir);
   RemoveFinalBackslash (szFilename);

  /*
   * Search for all files, eliminating:
   * '.'  - current directory
   * '..' - parent  directory
   * and changing directories so that a '\' appears at the front to
   * provide a visual queue to the user and ease forward navigation.
   */

   strcat (szFilename, "\\*.*");               /* find all files            */

   WinEnableWindowUpdate(hwndContainer, FALSE);  /* disallow painting in   */
                                                 /* the list while it's    */
                                                 /* being populated        */
   pffbFile = FindFileFirst (szFilename,
                             FILE_NORMAL | FILE_SYSTEM | FILE_DIRECTORY);

   while (pffbFile != NULL)
   {
      if(strcmp(pffbFile->achName, "\\.") && strcmp(pffbFile->achName, "\\.."))
      /*
       * Anything related to insertion of an item goes here.
       */
         WinInsertLboxItem(hwndContainer, LIT_SORTASCENDING, pffbFile->achName);

      pffbFile = FindFileNext (pffbFile);
   }

   WinScrollLboxItemToTop (hwndContainer, 0);
   WinShowWindow (hwndContainer, TRUE);       /* allow painting again        */

   return TRUE;
} /* End of PopulateList  */


/**************************************************************************
 *
 *  Name       : ListSubclassProc(hwnd, msg, mp1, mp2)
 *
 *  Description: Intercepts certain messages:
 *
 *       WM_BUTTON2DOWN - sends WM_CONTROL (LN_INITDRAG) to owning window
 *
 *       mp2 in the following messages points to a DRAGINFO structure:
 *
 *       DM_DRAGOVER - sends WM_CONTROL (LN_DRAGOVER)  to owning window
 *       DM_DRAGLEAVE- sends WM_CONTROL (LN_DRAGLEAVE) to owning window
 *       DM_DROP     - sends WM_CONTROL (LN_DROP)      to owning window
 *       DM_DROPHELP - sends WM_CONTROL (LN_DROPHELP)  to owning window
 *
 *  Concepts   : subclassing
 *
 *  API's      : WinSetPointer
 *
 *  Parameters :  hwnd = window handle
 *                msg  = message code
 *                mp1  = first message parameter
 *                mp2  = second message parameter
 *
 *  Return     :  values are determined by each message
 *
 *************************************************************************/
MRESULT EXPENTRY ListSubclassProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   switch (msg)
   {
   case WM_MOUSEMOVE:
      if (CheckBusy() || cDragFiles)
      {
         WinSetPointer(HWND_DESKTOP, hptrBusy);
         return ((MRESULT) TRUE);
      }
      break;

   case WM_BUTTON2DOWN:
      return (SendOwner(hwnd, LN_INITDRAG, mp1, LIST_ID));

   case DM_DRAGOVER:
      return (SendOwner (hwnd, LN_DRAGOVER, mp1, LIST_ID));

   case DM_DRAGLEAVE:
      return (SendOwner (hwnd, LN_DRAGLEAVE, mp1, LIST_ID));

   case DM_DROPHELP:
      return (SendOwner (hwnd, LN_DROPHELP, mp1, LIST_ID));

   case DM_DROP:
      return (SendOwner (hwnd, LN_DROP, mp1, LIST_ID));
   }
   return (pfnwpList (hwnd, msg, mp1, mp2));
} /*  End of ListSubclassProc  */

/**************************************************************************
 *
 *  Name       : Message(pszMsg)
 *
 *  Description: Displays the given message text inside a box
 *
 *  Concepts   : message box
 *
 *  API's      : WinMessageBox
 *
 *  Parameters :  pszMsg = the message to be displayed
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID Message (PSZ pszMsg)
{
   WinMessageBox (HWND_DESKTOP, hwndClient, pszMsg, szClientClass, 0,
                 MB_MOVEABLE | MB_ICONEXCLAMATION);
} /*  End of Message  */

/**************************************************************************
 *
 *  Name       : MessageBox(hwndOwner, nIdMsg, fsStyle, fBeep)
 *
 *  Description: Displays the message box with the message
 *               given in idMsg retrieved from the message table
 *               and using the style flags in fsStyle
 *
 *               Called whenever a MessageBox is to be displayed
 *
 *  Concepts   : message box
 *               - Message string is loaded from the process'
 *                 message table
 *               - Alarm beep is sounded if desired
 *               - Message box with the message is displayed
 *               - WinMessageBox return value is returned
 *
 *  API's      : WinMessageBox
 *
 *  Parameters :  hwndOwner = handle of the owning window
 *                nIdMsg    = id of the message in the message table
 *                fsStyle   = word of style flags
 *                fBeep     = whether the message should also alarm
 *
 *  Return     :  the value returned from WinMwessageBox
 *
 *************************************************************************/
SHORT MessageBox(HWND hwndOwner,   /* handle of the message box's owner    */
                 SHORT idMsg,      /* id of the message in the message table */
                 SHORT fsStyle,    /* style of the message box              */
                 BOOL fBeep)       /* if TRUE, beep before msg is displayed */
{
   CHAR szText[MESSAGELEN];

   if(!WinLoadMessage(hab, (HMODULE)0L, idMsg, MESSAGELEN, (PSZ)szText))
   {
       WinAlarm(HWND_DESKTOP, WA_ERROR);
       return MBID_ERROR;
   }

   if(fBeep)
       WinAlarm(HWND_DESKTOP, WA_ERROR);

   return(WinMessageBox(HWND_DESKTOP, hwndOwner, szText, (PSZ)NULL,
                        MSGBOXID, fsStyle));
}   /* End of MessageBox  */
/***************************  End of dragdrop.c  *************************/

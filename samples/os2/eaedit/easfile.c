/*static char *SCCSID = "@(#)easfile.c	6.13 92/03/18";*/
/*==============================================================*\
 *                                                              *
 * EASFILE.C                                                    *
 *    (C) Copyright IBM Corporation 1992.                       *
 *--------------------------------------------------------------*
 *  This module contains subroutines for eas.c that specifically*
 *  deal with the Open file interface. Most of the routines were*
 *  taken from Charles Petzold's book "Programming the OS/2     *
 *  Presentation Manager" and were slightly modified.           *
 *                                                              *
 *--------------------------------------------------------------*
 *  Procedures in this file:                                    *
 *   OpenFile()        Asks user for new file name and opens it *
 *   OpenFileProc()    Dialog proc that prompts user file name  *
 *   FillDirListBox()  Fills the directory list box             *
 *   FillFileListBox() Fills the file list box                  *
 *                                                              *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "eas.h"

extern EADATA ConvTable[EATABLESIZE];

/****************************************************************\
 *                                                              *
 *  Name:    OpenFile(hwnd, usMode)                             *
 *                                                              *
 *  Purpose: This routine handles opening a new file.  It will  *
 *           also query the user for the disposition of the     *
 *           current file if it has been modified before loading*
 *           the new file.                                      *
 *                                                              *
 *  Usage  : This routine is called by MainDlgProc to deal with *
 *           the Open file interface.                           *
 *                                                              *
 *  Method : Routine should be bullet proof as it does its own  *
 *           error checking.  It assumes that hwnd points to the*
 *           correct window with the name listbox in it.        *
 *                                                              *
 *  Returns: TRUE if a file is open upon exit.                  *
 *                                                              *
\****************************************************************/
BOOL OpenFile(HWND hwnd, ULONG usMode)
{
   CHAR szOldFile[CCHMAXPATH];
   HFILE  FileHandle=0;
   ULONG  ulAct=0L;

   strcpy(szOldFile,szFileName);  /* Save name of the currently open file */

   if(usMode != ARGFILE)                /* It isn't the command line file */
   {
      if(!WinDlgBox(HWND_DESKTOP,      /* Get the file name from the user */
                    hwnd,
                    OpenFileProc,
                    '\0',
                    IDD_OPENBOX,
                    NULL))
      {
         strcpy(szFileName,szOldFile); /* They canceled, restore old file */
         return(FILE_ISOPEN);
      }
   }
                             /* Check filename availble or not */
   if( DosOpen(szFileName, &FileHandle, &ulAct, 0L, FILE_NORMAL,
                        FILE_OPEN, OPEN_SHARE_DENYWRITE, NULL) )
   {
      MessageBox(hwnd, IDMSG_ERRORFILEOPEN, "Error !",
                       MB_OK | MB_ERROR, TRUE);
      return(FILE_ISOPEN);
   }
   else                 /* File is existed and availble to use */
      DosClose(FileHandle);

   if(FILE_ISOPEN) /* Free up everything associated with the current file */
   {
      Free_FEAList(pHoldFEA,pDelList);
      FILE_ISOPEN = FALSE;
   }

   if(QueryEAs(szFileName))                         /* We were successful */
   {
      HOLDFEA *pFEA=pHoldFEA;

      FILE_ISOPEN = TRUE;
      FILE_CHANGED = FALSE;

      WinSendDlgItemMsg(hwnd, IDD_WINLBOX, LM_DELETEALL,0L,0L); /* Fill L-box */

      while(pFEA)
      {
         WinSendDlgItemMsg(hwnd, IDD_WINLBOX, LM_INSERTITEM,
                           MPFROM2SHORT(LIT_END,0),
                           MPFROMP(pFEA->szName));
         pFEA = pFEA->next;
      }
   }
   else                                      /* We couldn't query the EAs */
   {
      *szFileName = '\000';
      memset(szEAType, 0, sizeof(szEAType) );
      WinInvalidateRect(hwndMain, NULL, FALSE);
      FILE_ISOPEN = FALSE;
      return (FILE_ISOPEN);
   }
   memset(szEAType, 0, sizeof(szEAType) );
   WinInvalidateRect(hwndMain, NULL, FALSE);
   pDelList = NULL;
   return(TRUE);
}

/****************************************************************\
 *                                                              *
 *  Name:    OpenFileProc(hwnd, msg, mp1, mp2)                  *
 *                                                              *
 *  Purpose: This proc handles the user interface to select a   *
 *           file name. Some elementary checks are done to make *
 *           sure the filename is valid.                        *
 *                                                              *
 *  Usage  : This routine is called by OpenFile to open the file*
 *                                                              *
 *  Method : The interface is NOT foolproof as it is possible to*
 *           continue with a non-existant file name.  Also,     *
 *           users are not currently allowed to view/edit the   *
 *           EAs attached to a directory.                       *
 *                                                              *
 *  Returns: TRUE if selects OK, FALSE if Cancel is selected.   *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY OpenFileProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static CHAR szCurrentPath[CCHMAXPATH]={'.', '\\', '*', '\000'},
                szBuffer[CCHMAXPATH];
   CHAR        szParsedPath[CCHMAXPATH];
   SHORT       sSelect;
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         FillDirListBox(hwnd,szCurrentPath);
         FillFileListBox(hwnd);
         WinSendDlgItemMsg(hwnd, IDD_FILEEDIT,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(CCHMAXPATH,0),NULL);
         return (MRESULT)0L;

      case WM_CONTROL:                      /* An lbox item is selected */
         if(SHORT1FROMMP(mp1) == IDD_DIRLIST ||
            SHORT1FROMMP(mp1) == IDD_FILELIST)
         {                                        /* Get item->szBuffer */
            sSelect = (USHORT) WinSendDlgItemMsg(hwnd,
                               SHORT1FROMMP(mp1),
                               LM_QUERYSELECTION, 0L, 0L);

            WinSendDlgItemMsg(hwnd, SHORT1FROMMP(mp1),
                              LM_QUERYITEMTEXT,
                              MPFROM2SHORT(sSelect, sizeof(szBuffer)),
                              MPFROMP(szBuffer));
         }

         switch(SHORT1FROMMP(mp1))
         {
            case IDD_DIRLIST:          /* Item was in the directory lbox */
               switch(SHORT2FROMMP(mp1))
               {
                  case LN_ENTER:           /* Go to the select drive/dir */
                     if(*szBuffer == ' ')
                        DosSelectDisk(*(szBuffer+1) - '@');
                     else
                        DosSetCurrentDir(szBuffer);

                     FillDirListBox(hwnd, szCurrentPath);
                     FillFileListBox(hwnd);

                     WinSetDlgItemText(hwnd, IDD_FILEEDIT, "");
                     return (MRESULT)0L;
               }
               break;

            case IDD_FILELIST:             /* Item was in the file lbox */
               switch(SHORT2FROMMP(mp1))
               {
                  case LN_SELECT:          /* Copy name to entry field  */
                     WinSetDlgItemText(hwnd, IDD_FILEEDIT, szBuffer);
                     return (MRESULT)0L;

                  case LN_ENTER:              /* Try to query the file */
                     if(ParseFileName(szFileName, szBuffer) != FILE_VALID)
                        return 0;          /* Some error, don't finish */
                     WinDismissDlg(hwnd, TRUE);
                     return (MRESULT)0L;
               }
               break;
         }
         break;

      case WM_COMMAND:
         switch(LOUSHORT(mp1))
         {
            case DID_OK:       /* Try to query file in the entry field */
               memset(szBuffer, 0, sizeof(szBuffer) );
               WinQueryDlgItemText(hwnd, IDD_FILEEDIT,
                                   sizeof(szBuffer), szBuffer);
                                         /* Check filename availble or not */
               if(*szBuffer == '\000')
               {
                  MessageBox(hwnd, IDMSG_NOFILEOPEN, "Error !",
                                   MB_OK | MB_ERROR, TRUE);
                  return FALSE;
               }
               switch(ParseFileName(szParsedPath, szBuffer))
               {
                  case FILE_INVALID:            /* Can't open the file */
                     WinAlarm(HWND_DESKTOP, WA_ERROR);
                     FillDirListBox(hwnd, szCurrentPath);
                     FillFileListBox(hwnd);
                     return (MRESULT)0L;

                  case FILE_PATH:    /* It was an incomplete path name */
                     strcpy(szCurrentPath,szBuffer);
                     FillDirListBox(hwnd, szCurrentPath);
                     FillFileListBox(hwnd);
                     WinSetDlgItemText(hwnd, IDD_FILEEDIT, "");
                     return (MRESULT)0L;

                  case FILE_VALID:                     /* It was valid */
                     strcpy(szFileName, szParsedPath);
                     WinDismissDlg(hwnd, TRUE);
                     return (MRESULT)0L;
               }
               break;

            case DID_CANCEL:
               WinDismissDlg(hwnd, FALSE);
               return (MRESULT)0L;
         }
         break;

      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)0L;

   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *                                                              *
 *  Name:    FillDirListBox(hwnd, pcCurrentPath)                *
 *                                                              *
 *  Purpose: This routine is called by OpenFileProc to fill in  *
 *           the directory list box                             *
 *                                                              *
 *  Usage  : This routine is called by OpenFileProc to fill the *
 *           file list box.                                     *
 *                                                              *
 *  Method : Adequete error checking is NOT done on the return  *
 *           values of the system calls.  Also, it is remotely  *
 *           possible that the calls to add to the list box     *
 *           could fail.                                        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\* **************************************************************/
VOID FillDirListBox(HWND hwnd, CHAR *pcCurrentPath)
{
   static CHAR szDrive [] = "  :";
   FILEFINDBUF3 findbuf;
   HDIR        hDir = HDIR_CREATE;
   SHORT       sDrive;
   ULONG       ulCurPathLen, ulDriveMap=0L, ulSearchCount = 1, ulDriveNum=0L;

   memset(&findbuf,0,sizeof(FILEFINDBUF) );

   DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
   *pcCurrentPath     = (CHAR) (ulDriveNum + '@');
   *(pcCurrentPath+1) = ':';
   *(pcCurrentPath+2) = '\\';
   ulCurPathLen = CCHMAXPATH;
   DosQueryCurrentDir(0L, pcCurrentPath + 3, &ulCurPathLen);

   WinSetDlgItemText(hwnd, IDD_PATH, pcCurrentPath);
   WinSendDlgItemMsg(hwnd, IDD_DIRLIST, LM_DELETEALL, NULL, NULL);

   for(sDrive = ('A'-'A'); sDrive <= ('Z'-'A'); sDrive++)
   {
      if(ulDriveMap & (1L << sDrive))
      {
         *(szDrive+1) = (CHAR) (sDrive + 'A');

         WinSendDlgItemMsg(hwnd, IDD_DIRLIST, LM_INSERTITEM,
                           MPFROM2SHORT(LIT_END, 0),
                           MPFROMP(szDrive));
      }
   }
   if (DosFindFirst("*", &hDir, FILE_DIRECTORY, &findbuf,
                sizeof(findbuf), &ulSearchCount, FIL_STANDARD))
   {
      DosBeep(500,110);
      return;
   }
   while(ulSearchCount)
   {
      if(findbuf.attrFile & 0x0010 &&
         (findbuf.achName[0] != '.' || findbuf.achName[1]))

         WinSendDlgItemMsg(hwnd, IDD_DIRLIST, LM_INSERTITEM,
                           MPFROM2SHORT(LIT_SORTASCENDING, 0),
                           MPFROMP(findbuf.achName));

      if(DosFindNext(hDir, &findbuf, sizeof(findbuf), &ulSearchCount))
         break;
   }
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    FillFileListBox(hwnd)                              *
 *                                                              *
 *  Purpose: This routine is called by OpenFileProc to fill in  *
 *           the file list box with files in the current        *
 *           directory.                                         *
 *                                                              *
 *  Usage  : This routine is called by OpenFileProc to fill the *
 *           file list box.                                     *
 *                                                              *
 *  Method : Adequete error checking is NOT done on the return  *
 *           values of the system calls.  Also, it is remotely  *
 *           possible that the calls to add to the list box     *
 *           could fail.                                        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID FillFileListBox(HWND hwnd)
{
   FILEFINDBUF3 findbuf;
   HDIR        hDir = HDIR_CREATE;
   ULONG       ulSearchCount = 1; /* Read 1 entry at a time */

   memset(&findbuf,0,sizeof(FILEFINDBUF) );
   WinSendDlgItemMsg(hwnd, IDD_FILELIST, LM_DELETEALL, NULL, NULL);

   if (DosFindFirst("*", &hDir, FILE_NORMAL, &findbuf, sizeof(findbuf),
                       &ulSearchCount,FIL_STANDARD))
   {
      DosBeep(700,110);
      return;
   }
   while(ulSearchCount)
   {
      WinSendDlgItemMsg(hwnd, IDD_FILELIST, LM_INSERTITEM,
                        MPFROM2SHORT(LIT_SORTASCENDING, 0),
                        MPFROMP(findbuf.achName));

      if(DosFindNext(hDir, &findbuf, sizeof(findbuf), &ulSearchCount))
         break;
   }
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    ParseFileName(pcOut, pcIn)                         *
 *                                                              *
 *  Purpose: This routine changes drive and directory as per    *
 *           pcIn string.                                       *
 *                                                              *
 *  Usage:   Note that pcOut is only valid if FILE_VALID is     *
 *           returned. In place of strupr(), a codepage should  *
 *           be fetched and DosCaseMap() should be used to allow*
 *           for extended chars. This routine could use some    *
 *           cleanup work.                                      *
 *                                                              *
 *  Returns: FILE_INVALID if pcIn had invalid drive or no       *
 *                           directory                          *
 *           FILE_PATH    if pcIn was empty or had just a path/ *
 *                           no file name.                      *
 *           FILE_VALID   if pcIn point to good file.           *
 *                                                              *
\****************************************************************/
LONG ParseFileName(CHAR *pcOut, CHAR *pcIn)
{
   CHAR   *pcLastSlash, *pcFileOnly ;
   ULONG  ulDriveMap, ulDirLen = CCHMAXPATH, ulDriveNum=0L;

   strupr(pcIn);   /* Does NOT handle extended chars, should use DosCaseMap */

   if(*pcIn == '\000')              /* If string is empty, return FILE_PATH */

      return FILE_PATH;

                        /* Get drive from input string or use current drive */
   if(*(pcIn+1) == ':')                      /* Yup, they specified a drive */
   {
      if(DosSelectDisk(*pcIn - '@'))            /* Change to selected drive */
         return FILE_INVALID;
      pcIn += 2;
   }
   DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);      /* Get current drive */
   *pcOut++ = (CHAR) (ulDriveNum + '@');              /* Build drive letter */
   *pcOut++ = ':';
   *pcOut++ = '\\';

   if(*pcIn == '\000')  /* If rest of the string is empty, return FILE_PATH */
      return FILE_PATH;

       /* Search for the last backslash.  If none, it could be a directory. */
   if(!(pcLastSlash = strrchr(pcIn, '\\')))                  /* No slashes? */
   {
      if(!DosSetCurrentDir(pcIn))
         return FILE_PATH;                            /* It was a directory */
                                       /* Get current dir & attach input fn */
      DosQueryCurrentDir(0L, pcOut, &ulDirLen);

      if(*(pcOut+strlen(pcOut)-1) != '\\')
         strcat(pcOut++, "\\");

      strcat(pcOut, pcIn);
      return FILE_VALID;
   }
               /* If the only backslash is at the beginning, change to root */
   if(pcIn == pcLastSlash)
   {
      DosSetCurrentDir("\\");

      if(*(pcIn+1) == '\000')
         return FILE_PATH;

      strcpy(pcOut, pcIn+1);
      return FILE_VALID;
   }
                   /* Attempt to change directory -- Get current dir if OK */
  *pcLastSlash = '\0';

   if(DosSetCurrentDir(pcIn))
      return FILE_INVALID;

   DosQueryCurrentDir(0L, pcOut, &ulDirLen);
                                            /* Append input filename if any */
   pcFileOnly = pcLastSlash+1;

   if(*pcFileOnly == '\000')
      return FILE_PATH;

   if(*(pcOut+strlen(pcOut)-1) != '\\')
      strcat(pcOut++, "\\");

   strcat(pcOut, pcFileOnly);
   return FILE_VALID;
}
